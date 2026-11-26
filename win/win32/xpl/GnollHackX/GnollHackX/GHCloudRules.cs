using System;
using System.Collections.Generic;

namespace GnollHackX
{
    public enum GHCloudArea
    {
        SaveTransfer,
        Replay
    }

    public enum GHCloudAccess
    {
        Read,
        Write,
        Create,
        Delete
    }

    public enum GHCloudRefusal
    {
        None,
        Credentials,
        RateLimited,
        BadRequest,
        ServerError
    }

    /* Rules of the cloud storage client that need no network, Azure or app state; linked by the unit tests. */
    public static class GHCloudRules
    {
        public const int MaxPathsPerGrantRequest = 32;
        public const string XlogPostPath = "/xlogfile";
        public static readonly TimeSpan GrantExpiryMargin = TimeSpan.FromMinutes(1);
        public static readonly TimeSpan QueuedGamePostMaxAge = TimeSpan.FromHours(24);

        public static List<List<T>> SplitIntoBatches<T>(IList<T> items, int batchSize)
        {
            List<List<T>> batches = new List<List<T>>();
            if (items == null || items.Count == 0)
                return batches;
            if (batchSize < 1)
                batchSize = 1;

            List<T> current = null;
            int i;
            for (i = 0; i < items.Count; i++)
            {
                if (current == null || current.Count >= batchSize)
                {
                    current = new List<T>(Math.Min(batchSize, items.Count - i));
                    batches.Add(current);
                }
                current.Add(items[i]);
            }
            return batches;
        }

        public static bool IsGrantUsable(DateTimeOffset expiresOn, DateTimeOffset nowUtc)
        {
            return nowUtc < expiresOn - GrantExpiryMargin;
        }

        /* Null unless the address is https and ends with the xlog post path, which apiPath then replaces. */
        public static string TryDeriveEndpoint(string xlogPostAddress, string apiPath)
        {
            if (string.IsNullOrWhiteSpace(xlogPostAddress) || string.IsNullOrEmpty(apiPath))
                return null;
            if (!xlogPostAddress.StartsWith("https://", StringComparison.Ordinal))
                return null;
            if (!xlogPostAddress.EndsWith(XlogPostPath, StringComparison.Ordinal))
                return null;
            if (xlogPostAddress.Length <= "https://".Length + XlogPostPath.Length)
                return null;

            string endpoint = xlogPostAddress.Substring(0, xlogPostAddress.Length - XlogPostPath.Length) + apiPath;
            return Uri.IsWellFormedUriString(endpoint, UriKind.Absolute) ? endpoint : null;
        }

        public static bool CanUploadReplays(string customConnectionString, bool tournamentMode, string userName, string password)
        {
            if (!string.IsNullOrWhiteSpace(customConnectionString) && !tournamentMode)
                return true;
            return !string.IsNullOrWhiteSpace(userName) && !string.IsNullOrWhiteSpace(password);
        }

        public static GHCloudRefusal ClassifyServerResponse(int status)
        {
            if (status >= 200 && status < 300)
                return GHCloudRefusal.None;
            if (status == 401 || status == 403 || status == 412)
                return GHCloudRefusal.Credentials;
            if (status == 429)
                return GHCloudRefusal.RateLimited;
            if (status >= 400 && status < 500)
                return GHCloudRefusal.BadRequest;
            return GHCloudRefusal.ServerError;
        }

        /* Azure's answer to a create-only upload of a blob that exists; which of the two it gives is not documented. */
        public static bool IsLockHeld(int status, string errorCode)
        {
            if (status == 409)
                return string.Equals(errorCode, "BlobAlreadyExists", StringComparison.Ordinal);
            if (status == 403)
                return string.Equals(errorCode, "AuthorizationPermissionMismatch", StringComparison.Ordinal);
            return false;
        }

        public static bool IsExpiredGrant(int status, string errorCode)
        {
            return status == 403 && string.Equals(errorCode, "AuthenticationFailed", StringComparison.Ordinal);
        }

        public static bool IsStaleQueuedGamePost(DateTime queuedUtc, DateTime nowUtc)
        {
            return nowUtc - queuedUtc > QueuedGamePostMaxAge;
        }
    }
}
