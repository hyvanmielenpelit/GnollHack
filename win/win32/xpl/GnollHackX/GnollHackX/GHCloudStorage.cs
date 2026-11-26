using Azure;
using Azure.Storage.Blobs;
using Azure.Storage.Blobs.Models;
using Newtonsoft.Json;
using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Net.Http;
using System.Net.Http.Headers;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
#if GNH_MAUI
using GnollHackM;
#endif

namespace GnollHackX
{
    public sealed class GHCloudItem
    {
        public string Name;            /* Save transfers: relative to the player's folder. Replays: full path. */
        public bool IsPrefix;
        public long Length;
        public DateTimeOffset? CreatedOn;
        public DateTimeOffset? LastModified;
    }

    /* Save transfer paths are relative to the player's folder. Replay paths are full for Read and file names for Write. */
    public interface IGHCloudContainer
    {
        GHCloudArea Area { get; }
        Task<List<GHCloudItem>> ListAsync(string prefix, bool hierarchical, CancellationToken token);
        Task<Dictionary<string, BlobClient>> GetBlobsAsync(IList<string> paths, GHCloudAccess access, CancellationToken token);
        void Forget(string path, GHCloudAccess access);
    }

    /* The game API server refused a request; Message is the server's own text, shown to the player as is. */
    public sealed class GHCloudRefusedException : Exception
    {
        public int Status { get; private set; }
        public GHCloudRefusal Refusal { get; private set; }
        public TimeSpan? RetryAfter { get; private set; }

        public GHCloudRefusedException(int status, string message, TimeSpan? retryAfter)
            : base(message)
        {
            Status = status;
            Refusal = GHCloudRules.ClassifyServerResponse(status);
            RetryAfter = retryAfter;
        }
    }

    public sealed class GHCloudUnavailableException : Exception
    {
        public GHCloudUnavailableException(string message) : base(message)
        {
        }
    }

    /* A player's own storage account, reached with the connection string from Settings. */
    public sealed class ConnectionStringCloudContainer : IGHCloudContainer
    {
        private readonly BlobContainerClient _container;
        private readonly string _saveTransferPrefix;
        private readonly string _replayUploadFolder;
        private bool _ensured;

        public GHCloudArea Area { get; private set; }

        public ConnectionStringCloudContainer(GHCloudArea area, string connectionString, string containerName, string userName)
        {
            Area = area;
            _container = new BlobServiceClient(connectionString).GetBlobContainerClient(containerName);
            _saveTransferPrefix = (userName ?? "") + GHConstants.AzureBlobStorageDelimiter;
            _replayUploadFolder = string.IsNullOrEmpty(userName) ? GHConstants.AzureBlobStorageGeneralDirectoryName : userName;
        }

        private async Task EnsureContainerAsync(CancellationToken token)
        {
            if (_ensured)
                return;
            await _container.CreateIfNotExistsAsync(cancellationToken: token);
            _ensured = true;
        }

        private string BlobNameFor(string path, GHCloudAccess access)
        {
            if (Area == GHCloudArea.SaveTransfer)
                return _saveTransferPrefix + path;
            if (access == GHCloudAccess.Write)
                return _replayUploadFolder + GHConstants.AzureBlobStorageDelimiter + path;
            return path;
        }

        public async Task<List<GHCloudItem>> ListAsync(string prefix, bool hierarchical, CancellationToken token)
        {
            await EnsureContainerAsync(token);
            List<GHCloudItem> items = new List<GHCloudItem>();
            string strip = Area == GHCloudArea.SaveTransfer ? _saveTransferPrefix : "";
            string fullPrefix = strip + (prefix ?? "");
            if (fullPrefix == "")
                fullPrefix = null;

            if (hierarchical)
            {
                var pages = _container.GetBlobsByHierarchyAsync(prefix: fullPrefix, delimiter: GHConstants.AzureBlobStorageDelimiter, cancellationToken: token).AsPages();
                var enumerator = pages.GetAsyncEnumerator(token);
                try
                {
                    while (await enumerator.MoveNextAsync())
                    {
                        foreach (BlobHierarchyItem h in enumerator.Current.Values)
                        {
                            if (h.IsPrefix)
                                items.Add(new GHCloudItem { Name = StripPrefix(h.Prefix, strip), IsPrefix = true });
                            else if (h.Blob != null)
                                items.Add(FromBlob(h.Blob, strip));
                        }
                    }
                }
                finally
                {
                    await enumerator.DisposeAsync();
                }
            }
            else
            {
                var pages = _container.GetBlobsAsync(prefix: fullPrefix, cancellationToken: token).AsPages();
                var enumerator = pages.GetAsyncEnumerator(token);
                try
                {
                    while (await enumerator.MoveNextAsync())
                    {
                        foreach (BlobItem b in enumerator.Current.Values)
                            items.Add(FromBlob(b, strip));
                    }
                }
                finally
                {
                    await enumerator.DisposeAsync();
                }
            }
            return items;
        }

        private static GHCloudItem FromBlob(BlobItem b, string strip)
        {
            return new GHCloudItem
            {
                Name = StripPrefix(b.Name, strip),
                IsPrefix = false,
                Length = b.Properties?.ContentLength ?? 0,
                CreatedOn = b.Properties?.CreatedOn,
                LastModified = b.Properties?.LastModified
            };
        }

        private static string StripPrefix(string name, string strip)
        {
            if (string.IsNullOrEmpty(strip) || name == null || !name.StartsWith(strip, StringComparison.Ordinal))
                return name;
            return name.Substring(strip.Length);
        }

        public async Task<Dictionary<string, BlobClient>> GetBlobsAsync(IList<string> paths, GHCloudAccess access, CancellationToken token)
        {
            await EnsureContainerAsync(token);
            Dictionary<string, BlobClient> result = new Dictionary<string, BlobClient>();
            if (paths == null)
                return result;
            foreach (string path in paths)
            {
                if (path != null && !result.ContainsKey(path))
                    result[path] = _container.GetBlobClient(BlobNameFor(path, access));
            }
            return result;
        }

        public void Forget(string path, GHCloudAccess access)
        {
        }
    }

    /* Storage reached through the game API server, which hands out a short-lived SAS URL for each approved blob. */
    public sealed class ServerGrantCloudContainer : IGHCloudContainer
    {
        private sealed class CachedGrant
        {
            public string Url;
            public DateTimeOffset ExpiresOn;
        }

        private sealed class ListResponse
        {
            public List<ListItem> items = null;
            public string continuationToken = null;
            public bool truncated = false;
        }

        private sealed class ListItem
        {
            public string name = null;
            public bool isPrefix = false;
            public long length = 0;
            public DateTimeOffset? createdOn = null;
            public DateTimeOffset? lastModified = null;
        }

        private sealed class AccessResponse
        {
            public Dictionary<string, string> urls = null;
            public DateTimeOffset expiresOn = DateTimeOffset.MinValue;
        }

        private static readonly ConcurrentDictionary<string, CachedGrant> _grants = new ConcurrentDictionary<string, CachedGrant>();
        private static readonly HttpClient _httpClient = new HttpClient { Timeout = TimeSpan.FromSeconds(30) };

        private readonly ulong _versionCompat;

        public GHCloudArea Area { get; private set; }

        public ServerGrantCloudContainer(GHCloudArea area, ulong versionCompat)
        {
            Area = area;
            _versionCompat = versionCompat;
        }

        public static void ClearCache()
        {
            _grants.Clear();
        }

        private static string AccessName(GHCloudAccess access)
        {
            switch (access)
            {
            case GHCloudAccess.Write:
                return "write";
            case GHCloudAccess.Create:
                return "create";
            case GHCloudAccess.Delete:
                return "delete";
            default:
                return "read";
            }
        }

        private bool NeedsCredentials(GHCloudAccess? access)
        {
            if (Area == GHCloudArea.SaveTransfer)
                return true;
            return access.HasValue && access.Value == GHCloudAccess.Write;
        }

        private string CacheKey(GHCloudAccess access, string path)
        {
            string user = NeedsCredentials(access) ? (GHApp.XlogUserName ?? "") : "";
            return ((int)Area).ToString() + "|" + _versionCompat.ToString() + "|" + user + "|" + ((int)access).ToString() + "|" + path;
        }

        private string Endpoint(bool isList)
        {
            string apiPath = Area == GHCloudArea.SaveTransfer
                ? (isList ? GHConstants.CloudSaveTransferListPath : GHConstants.CloudSaveTransferAccessPath)
                : (isList ? GHConstants.CloudReplayListPath : GHConstants.CloudReplayAccessPath);
            string endpoint = GHCloudRules.TryDeriveEndpoint(GHApp.XlogPostAddress, apiPath);
            if (endpoint == null)
                throw new GHCloudUnavailableException("The GnollHack server address is not available for cloud storage.");
            return endpoint;
        }

        private static void AddField(MultipartFormDataContent form, string name, string value)
        {
            StringContent content = new StringContent(value ?? "", Encoding.UTF8, "text/plain");
            ContentDispositionHeaderValue cdhv = new ContentDispositionHeaderValue("form-data");
            cdhv.Name = name;
            content.Headers.ContentDisposition = cdhv;
            form.Add(content);
        }

        private MultipartFormDataContent NewForm(bool withCredentials)
        {
            MultipartFormDataContent form = new MultipartFormDataContent("-------------------boundary");
            if (withCredentials)
            {
                AddField(form, "UserName", GHApp.XlogUserName);
                AddField(form, "Password", GHApp.XlogPassword);
            }
            AddField(form, "AntiForgeryToken", GHApp.XlogAntiForgeryToken);
            AddField(form, "VersionCompat", _versionCompat.ToString());
            return form;
        }

        private async Task<string> PostAsync(bool isList, MultipartFormDataContent form, bool sentCredentials, CancellationToken token)
        {
            using (HttpResponseMessage response = await _httpClient.PostAsync(Endpoint(isList), form, token))
            {
                string body = await response.Content.ReadAsStringAsync();
                int status = (int)response.StatusCode;
                if (response.IsSuccessStatusCode)
                    return body;

                TimeSpan? retryAfter = response.Headers.RetryAfter?.Delta;
                if (sentCredentials && (status == 403 || status == 412))
                {
                    if (GHApp.XlogUserNameVerified)
                        GHApp.SetXlogUserNameVerified(false, null, null);
                    GHApp.XlogCredentialsIncorrect = true;
                }
                string message = string.IsNullOrWhiteSpace(body) || body.Length > 500
                    ? "The GnollHack server refused the request (" + status.ToString() + ")."
                    : body.Trim();
                throw new GHCloudRefusedException(status, message, retryAfter);
            }
        }

        public async Task<List<GHCloudItem>> ListAsync(string prefix, bool hierarchical, CancellationToken token)
        {
            List<GHCloudItem> items = new List<GHCloudItem>();
            bool withCredentials = NeedsCredentials(null);
            string continuation = "";
            int pageCount = 0;
            do
            {
                ListResponse page;
                using (MultipartFormDataContent form = NewForm(withCredentials))
                {
                    AddField(form, "Prefix", prefix ?? "");
                    if (Area == GHCloudArea.Replay)
                        AddField(form, "ContinuationToken", continuation);
                    string body = await PostAsync(true, form, withCredentials, token);
                    page = JsonConvert.DeserializeObject<ListResponse>(body);
                }
                if (page == null)
                    break;
                if (page.items != null)
                {
                    foreach (ListItem li in page.items)
                    {
                        if (li == null || string.IsNullOrEmpty(li.name))
                            continue;
                        items.Add(new GHCloudItem
                        {
                            Name = li.name,
                            IsPrefix = li.isPrefix,
                            Length = li.length,
                            CreatedOn = li.createdOn,
                            LastModified = li.lastModified
                        });
                    }
                }
                if (page.truncated)
                    GHApp.MaybeWriteGHLog("Cloud listing was truncated by the server at " + items.Count + " items.");
                continuation = Area == GHCloudArea.Replay ? (page.continuationToken ?? "") : "";
                pageCount++;
            } while (continuation != "" && pageCount < 1000);
            return items;
        }

        public async Task<Dictionary<string, BlobClient>> GetBlobsAsync(IList<string> paths, GHCloudAccess access, CancellationToken token)
        {
            Dictionary<string, BlobClient> result = new Dictionary<string, BlobClient>();
            if (paths == null || paths.Count == 0)
                return result;

            DateTimeOffset now = DateTimeOffset.UtcNow;
            List<string> missing = new List<string>();
            foreach (string path in paths)
            {
                if (path == null || result.ContainsKey(path) || missing.Contains(path))
                    continue;
                CachedGrant cached;
                if (_grants.TryGetValue(CacheKey(access, path), out cached) && GHCloudRules.IsGrantUsable(cached.ExpiresOn, now))
                    result[path] = new BlobClient(new Uri(cached.Url));
                else
                    missing.Add(path);
            }

            bool withCredentials = NeedsCredentials(access);
            foreach (List<string> batch in GHCloudRules.SplitIntoBatches(missing, GHCloudRules.MaxPathsPerGrantRequest))
            {
                AccessResponse grant;
                using (MultipartFormDataContent form = NewForm(withCredentials))
                {
                    AddField(form, "Access", AccessName(access));
                    foreach (string path in batch)
                        AddField(form, "Paths", path);
                    string body = await PostAsync(false, form, withCredentials, token);
                    grant = JsonConvert.DeserializeObject<AccessResponse>(body);
                }
                foreach (string path in batch)
                {
                    string url;
                    if (grant == null || grant.urls == null || !grant.urls.TryGetValue(path, out url) || string.IsNullOrEmpty(url))
                        throw new GHCloudRefusedException(502, "The GnollHack server did not grant access to " + path + ".", null);
                    _grants[CacheKey(access, path)] = new CachedGrant { Url = url, ExpiresOn = grant.expiresOn };
                    result[path] = new BlobClient(new Uri(url));
                }
            }
            return result;
        }

        public void Forget(string path, GHCloudAccess access)
        {
            CachedGrant removed;
            _grants.TryRemove(CacheKey(access, path), out removed);
        }
    }

    public static class GHCloudStorage
    {
        /* Runs one storage operation; a grant that Azure no longer accepts is replaced once and the operation retried. */
        public static async Task<T> WithGrantRetryAsync<T>(IGHCloudContainer container, string path, GHCloudAccess access,
            BlobClient granted, Func<BlobClient, Task<T>> operation, CancellationToken token)
        {
            BlobClient client = granted;
            if (client == null)
                client = (await container.GetBlobsAsync(new string[] { path }, access, token))[path];
            try
            {
                return await operation(client);
            }
            catch (RequestFailedException ex) when (GHCloudRules.IsExpiredGrant(ex.Status, ex.ErrorCode))
            {
                container.Forget(path, access);
                BlobClient fresh = (await container.GetBlobsAsync(new string[] { path }, access, token))[path];
                return await operation(fresh);
            }
        }

        public static async Task WithGrantRetryAsync(IGHCloudContainer container, string path, GHCloudAccess access,
            BlobClient granted, Func<BlobClient, Task> operation, CancellationToken token)
        {
            await WithGrantRetryAsync<bool>(container, path, access, granted, async c =>
            {
                await operation(c);
                return true;
            }, token);
        }
    }
}
