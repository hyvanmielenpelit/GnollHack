#if SENTRY && (WINDOWS || (GNH_AVALONIA && LINUX))
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.IO;
using System.Net.Http;
using System.Net.Http.Headers;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Threading;
using System.Threading.Tasks;
using GnollHackX;

namespace GnollHackM
{
    // WORKAROUND(WA-SENTRY-0003): Sentry .NET captures no native crashes in JIT builds on Windows and Linux
    /* Native crash capture through sentry-native (inproc backend, no built-in transport): gnhsentry.dll on Windows,
       libsentry.so on Linux. Initialized before Sentry .NET, which starts its own native SDK only in trimmed or Native
       AOT builds. Crash envelopes go to an outbox folder and are uploaded at the next start. */
    internal static class GHSentryNative
    {
        /* sentry_value_t */
        [StructLayout(LayoutKind.Sequential)]
        internal struct SentryValue
        {
            public ulong Bits;
        }

#if WINDOWS
        private const string LibraryName = "gnhsentry";
#else
        private const string LibraryName = "sentry";
#endif

        private const string EnvelopeExtension = ".envelope";
        private const int MaxUploadsPerStart = 20;
        private const int MaxOutboxFiles = 50;
        private static readonly TimeSpan MaxEnvelopeAge = TimeSpan.FromDays(30);
        private static readonly TimeSpan UploadTimeout = TimeSpan.FromSeconds(30);

        private static readonly object _initLock = new object();
        private static bool _initAttempted = false;
        private static bool _initResult = false;
        private static bool _crashedLastRun = false;
        private static volatile bool _initialized = false;
        private static int _closed = 0;
        private static int _uploadStarted = 0;
        private static string _outboxDirectory = null;
        private static HttpClient _httpClient = null;

        internal static bool IsInitialized => _initialized;
        internal static bool IsClosed => Volatile.Read(ref _closed) != 0;

        [DllImport(LibraryName)]
        private static extern IntPtr sentry_options_new();

        [DllImport(LibraryName)]
        private static extern void sentry_options_free(IntPtr options);

        [DllImport(LibraryName)]
        private static extern void sentry_options_set_dsn(IntPtr options, [MarshalAs(UnmanagedType.LPUTF8Str)] string dsn);

        [DllImport(LibraryName)]
        private static extern void sentry_options_set_release(IntPtr options, [MarshalAs(UnmanagedType.LPUTF8Str)] string release);

        [DllImport(LibraryName)]
        private static extern void sentry_options_set_environment(IntPtr options, [MarshalAs(UnmanagedType.LPUTF8Str)] string environment);

        [DllImport(LibraryName)]
        private static extern void sentry_options_set_debug(IntPtr options, int debug);

        [DllImport(LibraryName)]
        private static extern void sentry_options_set_auto_session_tracking(IntPtr options, int value);

#if WINDOWS
        [DllImport(LibraryName, CharSet = CharSet.Unicode)]
        private static extern void sentry_options_set_database_pathw(IntPtr options, string path);
#else
        [DllImport(LibraryName)]
        private static extern void sentry_options_set_database_path(IntPtr options, [MarshalAs(UnmanagedType.LPUTF8Str)] string path);
#endif

        [DllImport(LibraryName)]
        private static extern void sentry_options_set_transport(IntPtr options, IntPtr transport);

        [DllImport(LibraryName)]
        private static extern unsafe IntPtr sentry_transport_new(delegate* unmanaged[Cdecl]<IntPtr, IntPtr, void> sendFunc);

        [DllImport(LibraryName)]
        private static extern int sentry_init(IntPtr options);

        [DllImport(LibraryName)]
        private static extern int sentry_close();

        [DllImport(LibraryName)]
        private static extern int sentry_get_crashed_last_run();

        [DllImport(LibraryName)]
        private static extern IntPtr sentry_envelope_serialize(IntPtr envelope, out UIntPtr sizeOut);

        [DllImport(LibraryName)]
        private static extern void sentry_envelope_free(IntPtr envelope);

        [DllImport(LibraryName)]
        private static extern void sentry_free(IntPtr ptr);

        [DllImport(LibraryName)]
        internal static extern void sentry_set_tag([MarshalAs(UnmanagedType.LPUTF8Str)] string key, [MarshalAs(UnmanagedType.LPUTF8Str)] string value);

        [DllImport(LibraryName)]
        internal static extern void sentry_remove_tag([MarshalAs(UnmanagedType.LPUTF8Str)] string key);

        [DllImport(LibraryName)]
        internal static extern void sentry_set_extra([MarshalAs(UnmanagedType.LPUTF8Str)] string key, SentryValue value);

        [DllImport(LibraryName)]
        internal static extern void sentry_remove_extra([MarshalAs(UnmanagedType.LPUTF8Str)] string key);

        [DllImport(LibraryName)]
        internal static extern void sentry_set_environment([MarshalAs(UnmanagedType.LPUTF8Str)] string environment);

        [DllImport(LibraryName)]
        internal static extern SentryValue sentry_value_new_object();

        [DllImport(LibraryName)]
        internal static extern SentryValue sentry_value_new_string([MarshalAs(UnmanagedType.LPUTF8Str)] string value);

        [DllImport(LibraryName)]
        internal static extern SentryValue sentry_value_new_breadcrumb([MarshalAs(UnmanagedType.LPUTF8Str)] string type, [MarshalAs(UnmanagedType.LPUTF8Str)] string message);

        [DllImport(LibraryName)]
        internal static extern int sentry_value_set_by_key(SentryValue value, [MarshalAs(UnmanagedType.LPUTF8Str)] string key, SentryValue v);

        [DllImport(LibraryName)]
        internal static extern void sentry_add_breadcrumb(SentryValue breadcrumb);

        [DllImport(LibraryName)]
        internal static extern void sentry_set_user(SentryValue user);

        [DllImport(LibraryName)]
        internal static extern void sentry_remove_user();

        /* Once, before SentrySdk.Init; false when native capture is unavailable, and the game then runs without it. A
           repeated call returns the outcome of the first one. */
        internal static bool Init(string dsn, string release, string environment, bool debug, string databaseDirectory, out bool crashedLastRun)
        {
            lock (_initLock)
            {
                if (!_initAttempted)
                {
                    _initAttempted = true;
                    _initResult = InitOnce(dsn, release, environment, debug, databaseDirectory);
                    if (!_initResult)
                        _crashedLastRun = false;
                }
                crashedLastRun = _crashedLastRun;
                return _initResult;
            }
        }

        // WORKAROUND(WA-SENTRY-0001): inproc's unhandled-exception filter does not chain to the filter it displaced
        private static unsafe bool InitOnce(string dsn, string release, string environment, bool debug, string databaseDirectory)
        {
            try
            {
                string outboxDirectory = Path.Combine(databaseDirectory, "outbox");
                Directory.CreateDirectory(outboxDirectory);
                _outboxDirectory = outboxDirectory;

                IntPtr options = sentry_options_new();
                if (options == IntPtr.Zero)
                {
                    Console.Error.WriteLine("GHSentryNative: cannot create sentry-native options");
                    return false;
                }

                try
                {
                    sentry_options_set_dsn(options, dsn);
                    if (!string.IsNullOrEmpty(release))
                        sentry_options_set_release(options, release);
                    if (!string.IsNullOrEmpty(environment))
                        sentry_options_set_environment(options, environment);
                    sentry_options_set_debug(options, debug ? 1 : 0);
                    /* Sentry .NET keeps the one release-health session */
                    sentry_options_set_auto_session_tracking(options, 0);
#if WINDOWS
                    sentry_options_set_database_pathw(options, databaseDirectory);
#else
                    sentry_options_set_database_path(options, databaseDirectory);
#endif
                    sentry_options_set_transport(options, sentry_transport_new(&SendEnvelope));
#if WINDOWS
                    /* Records the current unhandled-exception filter, which the wrapper keeps in the chain */
                    WindowsSentryNative.gnh_crash_prepare();
#else
                    GHAvaloniaSentryNative.ConfigureOptions(options);
#endif
                }
                catch
                {
                    sentry_options_free(options);
                    throw;
                }

                /* Takes ownership of the options */
                int result = sentry_init(options);
                if (result != 0)
                {
                    Console.Error.WriteLine("GHSentryNative: sentry_init failed with " + result);
                    return false;
                }
                _initialized = true;

                try
                {
                    _crashedLastRun = sentry_get_crashed_last_run() == 1;
#if WINDOWS
                    /* Unwrapped, sentry-native's filter would replace the runtime's and swallow unhandled managed
                       exceptions */
                    if (WindowsSentryNative.gnh_crash_install() == 0)
                    {
                        Log("GHSentryNative: cannot install the crash handlers in front of sentry-native's filter; native crash capture is off");
                        Close();
                        _initialized = false;
                        _crashedLastRun = false;
                        return false;
                    }
#else
                    GHAvaloniaSentryNative.AfterInit();
#endif
                }
                catch
                {
                    /* On Linux, without the SA_ONSTACK adjustment a caught DivideByZeroException would end the process */
                    Close();
                    _initialized = false;
                    _crashedLastRun = false;
                    throw;
                }
                return true;
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine("GHSentryNative: cannot initialize native crash capture: " + ex.GetType().Name + ": " + ex.Message);
                return false;
            }
        }

        /* Uninstalls the crash handlers; at most once. On Windows the filter wrapper goes first: sentry_close restores
           the previous filter only while its own filter is the current one. */
        internal static void Close()
        {
            if (!_initialized)
                return;
            if (Interlocked.Exchange(ref _closed, 1) != 0)
                return;

#if WINDOWS
            try
            {
                WindowsSentryNative.gnh_crash_uninstall();
            }
            catch (Exception ex)
            {
                WriteError("GHSentryNative: gnh_crash_uninstall failed: " + ex.Message);
            }
#endif
            try
            {
                sentry_close();
            }
            catch (Exception ex)
            {
                WriteError("GHSentryNative: sentry_close failed: " + ex.Message);
            }
        }

        /* Sentry .NET's rule: SENTRY_RELEASE, else <entry assembly name>@<informational version, else assembly
           version>; a version that already contains '@' is the release as it is */
        internal static string GetRelease()
        {
            string release = Environment.GetEnvironmentVariable("SENTRY_RELEASE");
            if (!string.IsNullOrWhiteSpace(release))
                return release;

            Assembly assembly = Assembly.GetEntryAssembly();
            if (assembly == null)
                return null;

            string name = assembly.GetName().Name;
            string version = null;
            try
            {
                version = assembly.GetCustomAttribute<AssemblyInformationalVersionAttribute>()?.InformationalVersion;
            }
            catch (Exception)
            {
                version = null;
            }
            if (string.IsNullOrEmpty(version))
                version = assembly.GetName().Version?.ToString();
            if (string.IsNullOrWhiteSpace(name) || string.IsNullOrWhiteSpace(version))
                return null;

            return version.Contains('@') ? version : name + "@" + version;
        }

        /* Sentry .NET's rule: SENTRY_ENVIRONMENT, else "debug" with a debugger attached, else "production" */
        internal static string GetEnvironment()
        {
            string environment = Environment.GetEnvironmentVariable("SENTRY_ENVIRONMENT");
            if (!string.IsNullOrWhiteSpace(environment))
                return environment;

            return Debugger.IsAttached ? "debug" : "production";
        }

        /* A crash writes its envelope into the database's run folder without calling the transport; the next
           sentry_init sends it through here, in a healthy process. Stores the bytes for the uploader; never throws and
           never does network I/O. */
        [UnmanagedCallersOnly(CallConvs = new[] { typeof(CallConvCdecl) })]
        private static void SendEnvelope(IntPtr envelope, IntPtr state)
        {
            try
            {
                string outboxDirectory = _outboxDirectory;
                IntPtr serialized = sentry_envelope_serialize(envelope, out UIntPtr size);
                if (serialized == IntPtr.Zero)
                    return;

                byte[] bytes;
                try
                {
                    bytes = new byte[checked((int)size.ToUInt64())];
                    Marshal.Copy(serialized, bytes, 0, bytes.Length);
                }
                finally
                {
                    sentry_free(serialized);
                }

                if (outboxDirectory == null)
                    return;
                string fileName = DateTime.UtcNow.ToString("yyyyMMddHHmmssfff", CultureInfo.InvariantCulture) + "-" + Guid.NewGuid().ToString("N") + EnvelopeExtension;
                File.WriteAllBytes(Path.Combine(outboxDirectory, fileName), bytes);
            }
            catch (Exception ex)
            {
                WriteError("GHSentryNative: cannot store an envelope: " + ex.Message);
            }
            finally
            {
                try
                {
                    sentry_envelope_free(envelope);
                }
                catch
                {
                    /* An unmanaged callback must not throw */
                }
            }
        }

        /* Uploads stored envelopes in the background, once per process */
        internal static void StartOutboxUpload(string dsn)
        {
            string outboxDirectory = _outboxDirectory;
            if (!_initialized || outboxDirectory == null || string.IsNullOrWhiteSpace(dsn))
                return;
            if (Interlocked.Exchange(ref _uploadStarted, 1) != 0)
                return;

            Task.Run(() => UploadOutboxAsync(dsn, outboxDirectory));
        }

        // WORKAROUND(WA-SENTRY-0004): SentrySdk.CaptureEnvelope can drop native-only event fields
        /* Posts the bytes unchanged: SentrySdk.CaptureEnvelope would re-parse the event and can drop native-only
           fields, such as registers, signal metadata and debug_meta details. Oldest first; a 4xx other than 429 can
           never succeed, so that file is deleted; 429, 5xx and network errors keep the rest for the next start. */
        private static async Task UploadOutboxAsync(string dsn, string outboxDirectory)
        {
            try
            {
                if (!TryGetEnvelopeEndpoint(dsn, out string endpoint, out string publicKey))
                {
                    Log("GHSentryNative: cannot parse the DSN; native crash reports are not uploaded");
                    return;
                }

                List<FileInfo> pending = PruneOutbox(outboxDirectory);
                if (pending.Count == 0)
                    return;

                if (_httpClient == null)
                    _httpClient = new HttpClient { Timeout = UploadTimeout };
                string auth = "Sentry sentry_version=7, sentry_key=" + publicKey + ", sentry_client=gnollhack-native/1.0";

                int sent = 0;
                int uploadCount = Math.Min(pending.Count, MaxUploadsPerStart);
                for (int i = 0; i < uploadCount; i++)
                {
                    FileInfo file = pending[i];
                    int status;
                    try
                    {
                        byte[] bytes = File.ReadAllBytes(file.FullName);
                        using (HttpRequestMessage request = new HttpRequestMessage(HttpMethod.Post, endpoint))
                        {
                            request.Content = new ByteArrayContent(bytes);
                            request.Content.Headers.ContentType = new MediaTypeHeaderValue("application/x-sentry-envelope");
                            request.Headers.TryAddWithoutValidation("X-Sentry-Auth", auth);
                            using (HttpResponseMessage response = await _httpClient.SendAsync(request).ConfigureAwait(false))
                            {
                                status = (int)response.StatusCode;
                            }
                        }
                    }
                    catch (Exception ex)
                    {
                        Log("GHSentryNative: native crash report upload failed: " + ex.GetType().Name + ": " + ex.Message);
                        break;
                    }

                    if (status >= 200 && status < 300)
                    {
                        TryDelete(file);
                        sent++;
                    }
                    else if (status == 429)
                    {
                        Log("GHSentryNative: native crash report upload rate limited");
                        break;
                    }
                    else if (status >= 400 && status < 500)
                    {
                        Log("GHSentryNative: native crash report " + file.Name + " rejected with status " + status + "; deleted");
                        TryDelete(file);
                    }
                    else
                    {
                        Log("GHSentryNative: native crash report upload failed with status " + status);
                        break;
                    }
                }

                if (sent > 0)
                    Log("GHSentryNative: uploaded " + sent + " native crash report(s)");
            }
            catch (Exception ex)
            {
                Log("GHSentryNative: native crash report upload failed: " + ex.Message);
            }
        }

        /* Deletes envelopes older than 30 days and all but the newest 50; returns the rest, oldest first */
        private static List<FileInfo> PruneOutbox(string outboxDirectory)
        {
            List<FileInfo> pending = new List<FileInfo>();
            DirectoryInfo directory = new DirectoryInfo(outboxDirectory);
            if (!directory.Exists)
                return pending;

            /* File names start with the UTC time stamp */
            FileInfo[] files = directory.GetFiles("*" + EnvelopeExtension);
            Array.Sort(files, (a, b) => string.CompareOrdinal(a.Name, b.Name));
            DateTime cutoff = DateTime.UtcNow - MaxEnvelopeAge;
            int firstKept = Math.Max(0, files.Length - MaxOutboxFiles);
            for (int i = 0; i < files.Length; i++)
            {
                if (i < firstKept || files[i].LastWriteTimeUtc < cutoff)
                    TryDelete(files[i]);
                else
                    pending.Add(files[i]);
            }
            return pending;
        }

        /* From <scheme>://<key>@<host>[:port]<prefix>/<project> to <scheme>://<host>[:port]<prefix>/api/<project>/envelope/ */
        private static bool TryGetEnvelopeEndpoint(string dsn, out string endpoint, out string publicKey)
        {
            endpoint = null;
            publicKey = null;
            if (!Uri.TryCreate(dsn.Trim(), UriKind.Absolute, out Uri uri))
                return false;

            string userInfo = Uri.UnescapeDataString(uri.UserInfo);
            int colon = userInfo.IndexOf(':');
            string key = colon >= 0 ? userInfo.Substring(0, colon) : userInfo;
            string path = uri.AbsolutePath.TrimEnd('/');
            int slash = path.LastIndexOf('/');
            string projectId = path.Substring(slash + 1);
            string prefix = slash > 0 ? path.Substring(0, slash) : "";
            if (string.IsNullOrEmpty(key) || string.IsNullOrEmpty(projectId))
                return false;

            endpoint = uri.Scheme + "://" + uri.Host + (uri.IsDefaultPort ? "" : ":" + uri.Port.ToString(CultureInfo.InvariantCulture))
                + prefix + "/api/" + projectId + "/envelope/";
            publicKey = key;
            return true;
        }

        private static void TryDelete(FileInfo file)
        {
            try
            {
                file.Delete();
            }
            catch (Exception ex)
            {
                Log("GHSentryNative: cannot delete " + file.Name + ": " + ex.Message);
            }
        }

        internal static void Log(string message)
        {
            try
            {
                GHApp.MaybeWriteGHLog(message);
            }
            catch
            {
                WriteError(message);
            }
        }

        internal static void WriteError(string message)
        {
            try
            {
                Console.Error.WriteLine(message);
            }
            catch
            {
                /* Standard error may be closed */
            }
        }
    }
}
#endif
