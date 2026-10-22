#if GNH_AVALONIA && LINUX && SENTRY
using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Net.Http;
using System.Net.Http.Headers;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Threading;
using System.Threading.Tasks;
using GnollHackX;

namespace GnollHackM
{
    /* Native crash capture for the Linux build: sentry-native (libsentry.so, inproc backend, no built-in transport),
       initialized before Sentry .NET, which starts its own native SDK only in trimmed or Native AOT builds. Crash
       envelopes go to an outbox folder and are uploaded at the next start. */
    internal static class GHAvaloniaSentryNative
    {
        /* sentry_value_t */
        [StructLayout(LayoutKind.Sequential)]
        internal struct SentryValue
        {
            public ulong Bits;
        }

        private const int SENTRY_HANDLER_STRATEGY_CHAIN_AT_START = 1;

        /* x86_64 Linux signal numbers */
        private const int SIGILL = 4;
        private const int SIGTRAP = 5;
        private const int SIGABRT = 6;
        private const int SIGBUS = 7;
        private const int SIGFPE = 8;
        private const int SIGSEGV = 11;

        /* glibc's x86_64 struct sigaction: handler at 0, sa_mask (128 bytes) at 8, int sa_flags at 136, sa_restorer at
           144; 152 bytes, read into a larger zeroed buffer */
        private const int SigActionBufferSize = 256;
        private const int SigActionFlagsOffset = 136;
        private const int SA_ONSTACK = 0x08000000;

        private const string EnvelopeExtension = ".envelope";
        private const int MaxUploadsPerStart = 20;
        private const int MaxOutboxFiles = 50;
        private static readonly TimeSpan MaxEnvelopeAge = TimeSpan.FromDays(30);
        private static readonly TimeSpan UploadTimeout = TimeSpan.FromSeconds(30);

        private static readonly int[] _checkedSignals = new int[] { SIGILL, SIGTRAP, SIGABRT, SIGBUS, SIGFPE, SIGSEGV };
        private static readonly int[] _managedFaultSignals = new int[] { SIGILL, SIGTRAP, SIGBUS, SIGFPE };

        private static volatile bool _initialized = false;
        private static int _closed = 0;
        private static int _uploadStarted = 0;
        private static string _outboxDirectory = null;
        private static IntPtr[] _handlers = null;
        private static HttpClient _httpClient = null;

        [DllImport("sentry")]
        private static extern IntPtr sentry_options_new();

        [DllImport("sentry")]
        private static extern void sentry_options_free(IntPtr options);

        [DllImport("sentry")]
        private static extern void sentry_options_set_dsn(IntPtr options, [MarshalAs(UnmanagedType.LPStr)] string dsn);

        [DllImport("sentry")]
        private static extern void sentry_options_set_release(IntPtr options, [MarshalAs(UnmanagedType.LPStr)] string release);

        [DllImport("sentry")]
        private static extern void sentry_options_set_environment(IntPtr options, [MarshalAs(UnmanagedType.LPStr)] string environment);

        [DllImport("sentry")]
        private static extern void sentry_options_set_debug(IntPtr options, int debug);

        [DllImport("sentry")]
        private static extern void sentry_options_set_auto_session_tracking(IntPtr options, int value);

        [DllImport("sentry")]
        private static extern void sentry_options_set_database_path(IntPtr options, [MarshalAs(UnmanagedType.LPStr)] string path);

        [DllImport("sentry")]
        private static extern void sentry_options_set_handler_strategy(IntPtr options, int handlerStrategy);

        [DllImport("sentry")]
        private static extern void sentry_options_set_transport(IntPtr options, IntPtr transport);

        [DllImport("sentry")]
        private static extern unsafe IntPtr sentry_transport_new(delegate* unmanaged[Cdecl]<IntPtr, IntPtr, void> sendFunc);

        [DllImport("sentry")]
        private static extern int sentry_init(IntPtr options);

        [DllImport("sentry")]
        private static extern int sentry_close();

        [DllImport("sentry")]
        private static extern int sentry_get_crashed_last_run();

        [DllImport("sentry")]
        private static extern IntPtr sentry_envelope_serialize(IntPtr envelope, out UIntPtr sizeOut);

        [DllImport("sentry")]
        private static extern void sentry_envelope_free(IntPtr envelope);

        [DllImport("sentry")]
        private static extern void sentry_free(IntPtr ptr);

        [DllImport("sentry")]
        internal static extern void sentry_set_tag([MarshalAs(UnmanagedType.LPStr)] string key, [MarshalAs(UnmanagedType.LPStr)] string value);

        [DllImport("sentry")]
        internal static extern void sentry_remove_tag([MarshalAs(UnmanagedType.LPStr)] string key);

        [DllImport("sentry")]
        internal static extern void sentry_set_extra([MarshalAs(UnmanagedType.LPStr)] string key, SentryValue value);

        [DllImport("sentry")]
        internal static extern void sentry_remove_extra([MarshalAs(UnmanagedType.LPStr)] string key);

        [DllImport("sentry")]
        internal static extern void sentry_set_environment([MarshalAs(UnmanagedType.LPStr)] string environment);

        [DllImport("sentry")]
        internal static extern SentryValue sentry_value_new_object();

        [DllImport("sentry")]
        internal static extern SentryValue sentry_value_new_string([MarshalAs(UnmanagedType.LPStr)] string value);

        [DllImport("sentry")]
        internal static extern SentryValue sentry_value_new_breadcrumb([MarshalAs(UnmanagedType.LPStr)] string type, [MarshalAs(UnmanagedType.LPStr)] string message);

        [DllImport("sentry")]
        internal static extern int sentry_value_set_by_key(SentryValue value, [MarshalAs(UnmanagedType.LPStr)] string key, SentryValue v);

        [DllImport("sentry")]
        internal static extern void sentry_add_breadcrumb(SentryValue breadcrumb);

        [DllImport("sentry")]
        internal static extern void sentry_set_user(SentryValue user);

        [DllImport("sentry")]
        internal static extern void sentry_remove_user();

        [DllImport("libc", EntryPoint = "sigaction", SetLastError = true)]
        private static extern int SigAction(int signum, IntPtr act, IntPtr oldact);

        /* Once, before SentrySdk.Init; false when native capture is unavailable, and the game then runs without it */
        internal static unsafe bool Init(string dsn, string release, string environment, bool debug, out bool crashedLastRun)
        {
            crashedLastRun = false;
            if (_initialized)
                return true;

            try
            {
                /* Explicit: the default, .sentry-native in the current directory, is inside the read-only AppImage
                   mount, because AppRun changes into $APPDIR/gnollhack */
                string databaseDirectory = Path.Combine(GHAvaloniaPaths.DataDirectory, "sentry-native");
                string outboxDirectory = Path.Combine(databaseDirectory, "outbox");
                Directory.CreateDirectory(outboxDirectory);
                _outboxDirectory = outboxDirectory;

                IntPtr options = sentry_options_new();
                if (options == IntPtr.Zero)
                {
                    Console.Error.WriteLine("GHAvaloniaSentryNative: cannot create sentry-native options");
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
                    sentry_options_set_database_path(options, databaseDirectory);
                    /* CoreCLR turns faults in managed code, such as null references and integer division by zero, into
                       exceptions from its own signal handlers. CHAIN_AT_START runs that previous handler first and
                       reports a crash only when it returns without having moved the instruction or stack pointer. */
                    sentry_options_set_handler_strategy(options, SENTRY_HANDLER_STRATEGY_CHAIN_AT_START);
                    sentry_options_set_transport(options, sentry_transport_new(&SendEnvelope));
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
                    Console.Error.WriteLine("GHAvaloniaSentryNative: sentry_init failed with " + result);
                    return false;
                }
                _initialized = true;

                try
                {
                    ClearAlternateStackOnManagedFaultSignals();
                    crashedLastRun = sentry_get_crashed_last_run() == 1;
                    _handlers = ReadHandlers();
                }
                catch
                {
                    /* Without the SA_ONSTACK adjustment a caught DivideByZeroException would end the process */
                    Close();
                    _initialized = false;
                    crashedLastRun = false;
                    throw;
                }
                return true;
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine("GHAvaloniaSentryNative: cannot initialize native crash capture: " + ex.GetType().Name + ": " + ex.Message);
                return false;
            }
        }

        /* Uninstalls the signal handlers; at most once */
        internal static void Close()
        {
            if (!_initialized)
                return;
            if (Interlocked.Exchange(ref _closed, 1) != 0)
                return;

            try
            {
                sentry_close();
            }
            catch (Exception ex)
            {
                WriteError("GHAvaloniaSentryNative: sentry_close failed: " + ex.Message);
            }
        }

        /* Inproc installs its handlers with SA_ONSTACK, so CoreCLR's chained handler runs on the small alternate
           signal stack. CoreCLR dispatches managed exceptions from its SIGILL, SIGTRAP, SIGBUS and SIGFPE handlers on
           the current stack, and only its SIGSEGV handler leaves the alternate stack first, so a caught
           DivideByZeroException would overflow it. SIGSEGV keeps SA_ONSTACK for native stack overflows; SIGABRT is
           never chained. */
        private static unsafe void ClearAlternateStackOnManagedFaultSignals()
        {
            byte* buffer = stackalloc byte[SigActionBufferSize];
            foreach (int signal in _managedFaultSignals)
            {
                new Span<byte>(buffer, SigActionBufferSize).Clear();
                if (SigAction(signal, IntPtr.Zero, (IntPtr)buffer) != 0)
                {
                    Console.Error.WriteLine("GHAvaloniaSentryNative: cannot read the handler of signal " + signal + ": errno " + Marshal.GetLastPInvokeError());
                    continue;
                }

                int* flags = (int*)(buffer + SigActionFlagsOffset);
                *flags &= ~SA_ONSTACK;
                if (SigAction(signal, (IntPtr)buffer, IntPtr.Zero) != 0)
                    Console.Error.WriteLine("GHAvaloniaSentryNative: cannot update the handler of signal " + signal + ": errno " + Marshal.GetLastPInvokeError());
            }
        }

        private static unsafe IntPtr[] ReadHandlers()
        {
            IntPtr[] handlers = new IntPtr[_checkedSignals.Length];
            byte* buffer = stackalloc byte[SigActionBufferSize];
            for (int i = 0; i < _checkedSignals.Length; i++)
            {
                new Span<byte>(buffer, SigActionBufferSize).Clear();
                handlers[i] = SigAction(_checkedSignals[i], IntPtr.Zero, (IntPtr)buffer) == 0 ? *(IntPtr*)buffer : IntPtr.Zero;
            }
            return handlers;
        }

        /* Logs every crash signal whose handler is no longer the one installed at initialization; inproc cannot
           reinstall its handlers, so this only reports */
        internal static void CheckHandlers(string context)
        {
            try
            {
                IntPtr[] expected = _handlers;
                if (!_initialized || _closed != 0 || expected == null)
                    return;

                IntPtr[] current = ReadHandlers();
                for (int i = 0; i < _checkedSignals.Length; i++)
                {
                    if (current[i] != expected[i])
                    {
                        GHApp.MaybeWriteGHLog("GHAvaloniaSentryNative: signal " + _checkedSignals[i] + " handler changed " + context
                            + ": 0x" + expected[i].ToString("x") + " -> 0x" + current[i].ToString("x"), true, "native");
                    }
                }
            }
            catch (Exception ex)
            {
                WriteError("GHAvaloniaSentryNative: cannot check the signal handlers: " + ex.Message);
            }
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
                WriteError("GHAvaloniaSentryNative: cannot store an envelope: " + ex.Message);
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

        /* Posts the bytes unchanged: SentrySdk.CaptureEnvelope would re-parse the event and can drop native-only
           fields, such as registers, signal metadata and debug_meta details. Oldest first; a 4xx other than 429 can
           never succeed, so that file is deleted; 429, 5xx and network errors keep the rest for the next start. */
        private static async Task UploadOutboxAsync(string dsn, string outboxDirectory)
        {
            try
            {
                if (!TryGetEnvelopeEndpoint(dsn, out string endpoint, out string publicKey))
                {
                    Log("GHAvaloniaSentryNative: cannot parse the DSN; native crash reports are not uploaded");
                    return;
                }

                List<FileInfo> pending = PruneOutbox(outboxDirectory);
                if (pending.Count == 0)
                    return;

                if (_httpClient == null)
                    _httpClient = new HttpClient { Timeout = UploadTimeout };
                string auth = "Sentry sentry_version=7, sentry_key=" + publicKey + ", sentry_client=gnollhack-linux-native/1.0";

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
                        Log("GHAvaloniaSentryNative: native crash report upload failed: " + ex.GetType().Name + ": " + ex.Message);
                        break;
                    }

                    if (status >= 200 && status < 300)
                    {
                        TryDelete(file);
                        sent++;
                    }
                    else if (status == 429)
                    {
                        Log("GHAvaloniaSentryNative: native crash report upload rate limited");
                        break;
                    }
                    else if (status >= 400 && status < 500)
                    {
                        Log("GHAvaloniaSentryNative: native crash report " + file.Name + " rejected with status " + status + "; deleted");
                        TryDelete(file);
                    }
                    else
                    {
                        Log("GHAvaloniaSentryNative: native crash report upload failed with status " + status);
                        break;
                    }
                }

                if (sent > 0)
                    Log("GHAvaloniaSentryNative: uploaded " + sent + " native crash report(s)");
            }
            catch (Exception ex)
            {
                Log("GHAvaloniaSentryNative: native crash report upload failed: " + ex.Message);
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
                Log("GHAvaloniaSentryNative: cannot delete " + file.Name + ": " + ex.Message);
            }
        }

        private static void Log(string message)
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

        private static void WriteError(string message)
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
