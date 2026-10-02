using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.IO.Compression;
using System.Security.Cryptography;
using System.Text;
using System.Threading;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
#if GNH_MAUI
using GnollHackM;
#endif

namespace GnollHackX.Performance
{
    /* A suite runner's setup, as chosen before CreateSuite is called. Runs is the number
       of measured runs, not counting the warm-up run; PageMode is "shared" or "fresh";
       Scenario is "idle", "minimap" or "playback". */
    public sealed class GHPerformanceSuiteSetup
    {
        public string ReplayPath;
        public int StartTurn;
        public string Scenario;        /* idle, minimap, playback */
        public int Runs;               /* measured runs, not counting the warm-up run */
        public string ArmLabel;
        public string PageMode;        /* shared, fresh */
        public bool WarmUpRun;
        public int WarmUpSeconds;
        public int WindowSeconds;
        public int CooldownSeconds;
        public string ThermalGate;     /* start, light */
        public int ThermalWaitSeconds;
    }

    /* One suite as listed by ListSuites: identity, provenance, and just enough of its
       result to sort and label a list without loading the full manifest again. */
    public sealed class GHPerformanceSuiteInfo
    {
        public string SuiteId;
        public string Directory;
        public string Scenario;
        public string ArmLabel;
        public string PageMode;
        public DateTime StartedUtc;
        public string Status;
        public string AbortReason;
        public string Origin;
        public int RunsUsed;
        public double MedianHitchRatioMsPerSec;   /* NaN when no used runs */
        public long SizeBytes;
        public string ComparabilityKey;
        public bool IsBaseline;                   /* the baseline label for this key and device equals ArmLabel */
        public bool HasBackgroundExclusion;       /* a run was excluded for background load */
        public bool EnvironmentChanged;           /* the environment changed during the suite */
        public bool IdMismatch;                   /* the manifest's suiteId differs from the folder name, SuiteId */
        public bool Unreadable;                   /* suite.json is missing or cannot be parsed; only SuiteId, Directory,
                                                     StartedUtc (the folder's last write) and SizeBytes are set */
    }

    /* What ImportZip hands back: the suite ids it moved into the store, the ones already
       present (left alone), and a short reason for every folder it refused. */
    public sealed class GHPerformanceImportSummary
    {
        public readonly List<string> Imported = new List<string>();
        public readonly List<string> Skipped = new List<string>();   /* ids already present */
        public readonly List<string> Errors = new List<string>();
    }

    /* Persistent storage for performance suites, under GHApp.GHPath/performance: one
       manifest (suite.json) per suite directory, alongside the run_*.json/csv files
       GHPerformanceRunRecord.EndWindowAndSave writes there directly, and a report.txt
       rendered by GHPerformanceTextReport. baselines.json, at the store root, records one
       arm label per comparability key and device (GHPerformanceSuiteLogic.BaselineKey);
       a key without a device part, as written by older builds, applies to any device
       that has no entry of its own.

       The manifest's top level never carries a property named schemaVersion, smoothness
       or series, so the offline analyzer's "every *.json in the folder is a run record"
       scan cannot mistake it for one; manifestVersion is its own version tag instead.
       Every double written to a manifest maps NaN and Infinity to 0 (see R below), the
       same convention GHPerformanceRunRecord.R uses for the per-run JSON, so the file
       never carries Newtonsoft's non-standard NaN token; the exception is a run's
       "background" object, where null marks a signal the platform did not report.

       A suite's id is its folder name, and every suite path is resolved through
       GHPerformanceSuiteId; a suite whose manifest suiteId differs from its folder is
       listed with IdMismatch set, one whose manifest cannot be read with Unreadable set,
       and either can only be deleted.

       suite.json, report.txt and baselines.json are written through GHAtomicFile, so a
       crash never leaves one half written. Before baselines.json is replaced, a copy
       of it that parsed is kept as baselines.json.bak; when baselines.json does not
       parse, the backup is read instead, and when neither does, setting a baseline is
       refused until ResetCorruptBaselines sets the unreadable file aside. The first
       ListSuites of a process deletes temp files older than StaleTempMaxAge left by a
       killed process.

       Every public member catches its own exceptions and fails soft (null, false, or an
       empty result) rather than throwing; manifest reads, edits and writes are serialized
       through one static lock, shared by every suite. Must compile under C# 7.3 (the
       legacy netstandard2.0 project) and under net10 MAUI alike. */
    public static class GHPerformanceSuiteStore
    {
        public const int CurrentManifestVersion = 1;

        public const string StatusRunning = "running";
        public const string StatusComplete = "complete";
        public const string StatusAborted = "aborted";

        /* The number the text ends with, e.g. 60 for "MapFPS60"; 0 when it ends in no digit */
        private static double TrailingNumber(string text)
        {
            if (string.IsNullOrEmpty(text))
                return 0;
            int start = text.Length;
            while (start > 0 && char.IsDigit(text[start - 1]))
                start--;
            double value;
            if (start == text.Length || !double.TryParse(text.Substring(start), NumberStyles.Integer, CultureInfo.InvariantCulture, out value))
                return 0;
            return value;
        }

        /* The manifest's status, except that a suite still marked running which the runner
           is not running was interrupted when the app stopped mid-suite */
        private static string EffectiveStatus(ManifestJson manifest, out string abortReason)
        {
            abortReason = manifest.AbortReason;
            if (manifest.Status == StatusRunning && manifest.SuiteId != GHPerformanceSuiteRunner.CurrentSuiteId)
            {
                abortReason = InterruptedReason;
                return StatusAborted;
            }
            return manifest.Status;
        }

        /* EffectiveStatus's abort reason for a suite interrupted when the app stopped */
        private const string InterruptedReason = "interrupted: the app stopped during the suite";

        public const string OriginLocal = "local";
        public const string OriginImported = "imported";

        private const string SuitesFolderName = "suites";
        private const string ManifestFileName = "suite.json";
        private const string ReportFileName = "report.txt";
        private const string ComparisonFileName = "comparison.txt";
        private const string BaselinesFileName = "baselines.json";
        private const string ImportStagingPrefix = "import_";
        private const string ShareZipPrefix = "GnollHack_Performance_";

        /* SetBaseline's error when baselines.json and its backup cannot be read; the page
           then offers ResetCorruptBaselines */
        public const string BaselinesUnreadableError = "baselines.json cannot be read.";

        private static readonly TimeSpan StaleTempMaxAge = TimeSpan.FromHours(1);

        /* How baselines.json was read: Recovered means it did not parse and its backup
           was read instead */
        private enum BaselinesState
        {
            Missing,
            Ok,
            Recovered,
            Corrupt
        }

        private static readonly object _lock = new object();
        private static int _staleTempsDeleted = 0;          /* 1 once the first ListSuites has deleted stale temps */
        private static int _baselinesRecoveryLogged = 0;    /* 1 once reading the backup has been logged */

        private static readonly JsonSerializerSettings _jsonSettings = new JsonSerializerSettings
        {
            Formatting = Formatting.Indented,
            Culture = CultureInfo.InvariantCulture,
            NullValueHandling = NullValueHandling.Include
        };

        /* GHPath/performance, created when missing */
        public static string RootDirectory
        {
            get
            {
                string root = Path.Combine(GHApp.GHPath, GHConstants.PerformanceDirectory);
                if (!Directory.Exists(root))
                    GHApp.CheckCreateDirectory(root);
                return root;
            }
        }

        /* RootDirectory/suites, created when missing */
        public static string SuitesDirectory
        {
            get
            {
                string dir = Path.Combine(RootDirectory, SuitesFolderName);
                if (!Directory.Exists(dir))
                    GHApp.CheckCreateDirectory(dir);
                return dir;
            }
        }

        /* SuitesDirectory/suiteId; null when suiteId is not a valid suite id
           (GHPerformanceSuiteId.IsValid) */
        public static string SuiteDirectory(string suiteId)
        {
            return GHPerformanceSuiteId.ResolveDirectory(SuitesDirectory, suiteId);
        }

        /* Lowercase hex SHA-256 of the file at path; null on any failure */
        public static string ComputeSha256(string path)
        {
            try
            {
                using (SHA256 sha = SHA256.Create())
                using (FileStream stream = File.OpenRead(path))
                {
                    byte[] hash = sha.ComputeHash(stream);
                    StringBuilder sb = new StringBuilder(hash.Length * 2);
                    for (int i = 0; i < hash.Length; i++)
                        sb.Append(hash[i].ToString("x2", CultureInfo.InvariantCulture));
                    return sb.ToString();
                }
            }
            catch
            {
                return null;
            }
        }

        /* Creates a new suite directory under SuitesDirectory and writes its manifest with
           status "running", environment captured now, and startedUtc now. The environment
           is provisional until UpdateEnvironment replaces it. Returns the new suiteId, or
           null on failure. */
        public static string CreateSuite(GHPerformanceSuiteSetup setup, string replaySha256, long replayBytes)
        {
            if (setup == null)
                return null;
            try
            {
                GHPerformanceEnvironmentFacts facts = GHPerformanceEnvironment.Capture();
                DateTime startedUtc = DateTime.UtcNow;
                string suiteId = AllocateSuiteId(startedUtc, setup.Scenario, facts.DeviceModel);
                string dir = SuiteDirectory(suiteId);
                if (dir == null)
                    return null;
                GHApp.CheckCreateDirectory(dir);

                ManifestJson manifest = new ManifestJson();
                manifest.ManifestVersion = CurrentManifestVersion;
                manifest.SuiteId = suiteId;
                manifest.Status = StatusRunning;
                manifest.Origin = OriginLocal;
                manifest.StartedUtc = ToIso(startedUtc);
                manifest.Setup = BuildSetupJson(setup, replaySha256, replayBytes);
                manifest.Environment = BuildEnvironmentJson(facts);
                manifest.Runs = new List<RunJson>();

                lock (_lock)
                {
                    WriteManifest(dir, manifest);
                }
                return suiteId;
            }
            catch
            {
                return null;
            }
        }

        /* Replaces the manifest's environment, fingerprint included, with one captured
           now and rewrites suite.json. The suite runner calls it once, when the suite's
           first window has been saved: the game page has painted by then, so the values
           set on the first paint are present. The suite id is unchanged. */
        public static void UpdateEnvironment(string suiteId)
        {
            if (string.IsNullOrEmpty(suiteId))
                return;
            try
            {
                GHPerformanceEnvironmentFacts facts = GHPerformanceEnvironment.Capture();
                lock (_lock)
                {
                    string dir;
                    ManifestJson manifest = ReadSuiteManifest(suiteId, out dir);
                    if (manifest == null)
                        return;
                    manifest.Environment = BuildEnvironmentJson(facts);
                    WriteManifest(dir, manifest);
                }
            }
            catch
            {
                /* The provisional environment from CreateSuite stays */
            }
        }

        /* Appends or replaces the run entry for context.RunIndex and rewrites suite.json.
           result is null when the run produced no record at all: the entry then carries no
           summary, and its excludedReason is context's own preset reason, or "no record". */
        public static void AddRun(string suiteId, GHPerformanceRunContext context, GHPerformanceRunResult result)
        {
            if (string.IsNullOrEmpty(suiteId) || context == null)
                return;
            try
            {
                lock (_lock)
                {
                    string dir;
                    ManifestJson manifest = ReadSuiteManifest(suiteId, out dir);
                    if (manifest == null)
                        return;

                    RunJson run = new RunJson();
                    run.Index = context.RunIndex;
                    run.IsWarmUp = context.IsWarmUp;
                    run.TurnReached = context.TurnReached;
                    if (context.TurnAtWindowStart >= 0)
                        run.TurnAtWindowStart = context.TurnAtWindowStart;
                    if (context.InputRecordsInWindow >= 0)
                        run.InputRecordsInWindow = context.InputRecordsInWindow;
                    run.Notes = context.Notes;

                    if (result != null)
                    {
                        run.RunFile = result.JsonPath != null ? Path.GetFileName(result.JsonPath) : null;
                        run.ExcludedReason = result.ExcludedReason;
                        run.ThermalBefore = BuildThermalJson(result.ThermalBefore);
                        run.ThermalAfter = BuildThermalJson(result.ThermalAfter);
                        run.Summary = result.Summary != null ? BuildSummaryJson(result.Summary) : null;
                        run.Background = BuildRunBackgroundJson(result.Background);
                    }
                    else
                    {
                        run.RunFile = null;
                        run.ExcludedReason = !string.IsNullOrEmpty(context.ExcludedReason) ? context.ExcludedReason : "no record";
                        run.ThermalBefore = BuildThermalJson(GHThermalProbe.Unknown);
                        run.ThermalAfter = BuildThermalJson(GHThermalProbe.Unknown);
                        run.Summary = null;
                    }

                    int existing = -1;
                    for (int i = 0; i < manifest.Runs.Count; i++)
                    {
                        if (manifest.Runs[i].Index == run.Index)
                        {
                            existing = i;
                            break;
                        }
                    }
                    if (existing >= 0)
                        manifest.Runs[existing] = run;
                    else
                        manifest.Runs.Add(run);

                    WriteManifest(dir, manifest);
                }
            }
            catch
            {
                /* The run's own JSON/CSV files are already saved regardless */
            }
        }

        /* FinishSuite with no fingerprint captured at the end */
        public static void FinishSuite(string suiteId, string status, string abortReason)
        {
            FinishSuite(suiteId, status, abortReason, null);
        }

        /* Sets status and endedUtc, computes measuredRefreshHz/targetFps as the median
           (GHPerformanceStatistics.MedianNearestRank) of the used runs' values above 0,
           computes comparabilityKey from that measured refresh rate, records
           fingerprintAtEnd and whether the environment changed since the suite start
           (any changed, added or
           removed key outside meta and code), rewrites suite.json, and writes
           report.txt. */
        public static void FinishSuite(string suiteId, string status, string abortReason,
            Dictionary<string, string> fingerprintAtEnd)
        {
            if (string.IsNullOrEmpty(suiteId))
                return;
            try
            {
                lock (_lock)
                {
                    string dir;
                    ManifestJson manifest = ReadSuiteManifest(suiteId, out dir);
                    if (manifest == null)
                        return;

                    manifest.Status =!string.IsNullOrEmpty(status) ? status : StatusComplete;
                    manifest.AbortReason = abortReason;
                    manifest.EndedUtc = ToIso(DateTime.UtcNow);

                    List<double> measured = new List<double>();
                    List<double> target = new List<double>();
                    for (int i = 0; i < manifest.Runs.Count; i++)
                    {
                        if (!IsUsedRun(manifest.Runs[i]))
                            continue;
                        SummaryJson s = manifest.Runs[i].Summary;
                        if (s.MeasuredRefreshHz > 0)
                            measured.Add(s.MeasuredRefreshHz);
                        if (s.TargetFps > 0)
                            target.Add(s.TargetFps);
                    }
                    double measuredMedian = GHPerformanceStatistics.MedianNearestRank(measured);
                    manifest.MeasuredRefreshHz = R(measuredMedian);
                    manifest.TargetFps = R(GHPerformanceStatistics.MedianNearestRank(target));
                    manifest.ComparabilityKey = ComputeComparabilityKey(manifest, measuredMedian);

                    if (fingerprintAtEnd != null && fingerprintAtEnd.Count > 0)
                    {
                        manifest.FingerprintAtEnd = fingerprintAtEnd;
                        Dictionary<string, string> atStart = manifest.Environment != null ? manifest.Environment.Fingerprint : null;
                        if (atStart != null && atStart.Count > 0)
                            manifest.EnvironmentChanged = EnvironmentDiffers(atStart, fingerprintAtEnd);
                    }

                    WriteManifest(dir, manifest);
                }
                WriteReport(suiteId);
            }
            catch
            {
                /* Best effort; the suite's run files are unaffected */
            }
        }

        /* Every suite under SuitesDirectory whose folder name is a valid suite id, newest
           StartedUtc first; one whose manifest cannot be read is listed with Unreadable
           set, and a folder that fails otherwise is skipped rather than failing the whole
           listing. The first call of a process also deletes the temp files older than
           StaleTempMaxAge in RootDirectory and in every suite folder. */
        public static List<GHPerformanceSuiteInfo> ListSuites()
        {
            List<GHPerformanceSuiteInfo> list = new List<GHPerformanceSuiteInfo>();
            try
            {
                bool deleteStaleTemps = Interlocked.Exchange(ref _staleTempsDeleted, 1) == 0;
                if (deleteStaleTemps)
                    GHAtomicFile.DeleteStaleTemps(RootDirectory, DateTime.UtcNow, StaleTempMaxAge);
                string root = SuitesDirectory;
                if (!Directory.Exists(root))
                    return list;
                string[] dirs = Directory.GetDirectories(root);
                for (int i = 0; i < dirs.Length; i++)
                {
                    if (deleteStaleTemps)
                        GHAtomicFile.DeleteStaleTemps(dirs[i], DateTime.UtcNow, StaleTempMaxAge);
                    GHPerformanceSuiteInfo info = TryBuildSuiteInfo(dirs[i]);
                    if (info != null)
                        list.Add(info);
                }
                list.Sort(delegate (GHPerformanceSuiteInfo a, GHPerformanceSuiteInfo b)
                {
                    return b.StartedUtc.CompareTo(a.StartedUtc);
                });
            }
            catch
            {
                /* Return whatever was gathered before the failure */
            }
            return list;
        }

        /* The manifest mapped to the shape the text report and the comparison read; null
           when the suite cannot be read or its manifest suiteId differs from suiteId. */
        public static GHReportSuite LoadReportSuite(string suiteId)
        {
            try
            {
                ManifestJson manifest;
                string dir;
                lock (_lock)
                {
                    manifest = ReadSuiteManifest(suiteId, out dir);
                }
                return manifest != null ? MapToReportSuite(manifest) : null;
            }
            catch
            {
                return null;
            }
        }

        /* null when suiteId is not a valid suite id */
        public static string ReportPath(string suiteId)
        {
            string dir = SuiteDirectory(suiteId);
            return dir != null ? Path.Combine(dir, ReportFileName) : null;
        }

        /* Renders and writes report.txt for the suite, with the previous comparable suite
           (FindPreviousSuite) when there is one; returns its path, or null on failure
           (including when the suite cannot be loaded or its manifest suiteId differs). */
        public static string WriteReport(string suiteId)
        {
            try
            {
                ManifestJson manifest;
                string dir;
                lock (_lock)
                {
                    manifest = ReadSuiteManifest(suiteId, out dir);
                }
                if (manifest == null)
                    return null;
                GHReportSuite suite = MapToReportSuite(manifest);
                suite.Previous = FindPreviousSuite(suite, manifest.ComparabilityKey);
                string text = GHPerformanceTextReport.SuiteReport(suite);
                string path = Path.Combine(dir, ReportFileName);
                lock (_lock)
                {
                    if (!Directory.Exists(dir))
                        GHApp.CheckCreateDirectory(dir);
                    GHAtomicFile.WriteAllText(path, text, null);
                }
                return path;
            }
            catch
            {
                return null;
            }
        }

        /* Records this suite's arm label as the baseline for its comparability key on its
           device (GHPerformanceSuiteLogic.BaselineKey); the other entries of
           baselines.json are kept. False, with a short sentence in error, when the suite
           cannot be read (no manifest, an unreadable one, or a manifest suiteId that
           differs), has no setup
           or no comparabilityKey yet (i.e. it has not been finished), when baselines.json
           and its backup cannot be read (error is then BaselinesUnreadableError), or when
           the file cannot be written. */
        public static bool SetBaseline(string suiteId, out string error)
        {
            error = null;
            try
            {
                ManifestJson manifest;
                string dir;
                lock (_lock)
                {
                    manifest = ReadSuiteManifest(suiteId, out dir);
                }
                if (manifest == null)
                {
                    error = "This suite could not be read.";
                    return false;
                }
                if (manifest.Setup == null || string.IsNullOrEmpty(manifest.ComparabilityKey))
                {
                    error = "This suite has no comparability key yet.";
                    return false;
                }

                lock (_lock)
                {
                    BaselinesState state;
                    BaselinesJson baselines = ReadBaselines(out state);
                    if (state == BaselinesState.Corrupt)
                    {
                        error = BaselinesUnreadableError;
                        return false;
                    }
                    baselines.Baselines[GHPerformanceSuiteLogic.BaselineKey(manifest.ComparabilityKey, DeviceKeyOf(manifest))]
                        = manifest.Setup.ArmLabel;
                    WriteBaselines(baselines, state == BaselinesState.Ok);
                }
                return true;
            }
            catch (Exception ex)
            {
                error = "The baseline could not be saved: " + ex.Message;
                return false;
            }
        }

        /* Sets an unreadable baselines.json aside as
           baselines.json.corrupt-<yyyyMMddTHHmmssZ>, so the next SetBaseline starts a new
           file. True when the file was set aside or baselines.json is no longer
           unreadable; false when it could not be renamed. */
        public static bool ResetCorruptBaselines()
        {
            try
            {
                lock (_lock)
                {
                    BaselinesState state;
                    ReadBaselines(out state);
                    if (state != BaselinesState.Corrupt)
                        return true;
                    string path = BaselinesPath;
                    if (!File.Exists(path))
                        return true;
                    string kept = path + GHAtomicFile.CorruptMarker
                        + DateTime.UtcNow.ToString("yyyyMMdd'T'HHmmss'Z'", CultureInfo.InvariantCulture);
                    File.Move(path, kept);
                    Log(BaselinesFileName + " could not be read; kept as " + Path.GetFileName(kept));
                    return true;
                }
            }
            catch (Exception ex)
            {
                Log("setting aside " + BaselinesFileName + " failed: " + ex.Message);
                return false;
            }
        }

        /* The arm label recorded as the baseline for comparabilityKey on the device
           deviceKey names (GHPerformanceSuiteLogic.DeviceKey), else for comparabilityKey
           alone, or null when neither is set. */
        public static string GetBaselineLabel(string comparabilityKey, string deviceKey)
        {
            BaselinesState state;
            BaselinesJson baselines;
            return GetBaselineLabel(comparabilityKey, deviceKey, out state, out baselines);
        }

        /* Same as GetBaselineLabel(comparabilityKey, deviceKey), with baselines.json as
           read and how it was read: Corrupt when it and its backup cannot be read */
        private static string GetBaselineLabel(string comparabilityKey, string deviceKey, out BaselinesState state,
            out BaselinesJson baselines)
        {
            state = BaselinesState.Missing;
            baselines = null;
            if (string.IsNullOrEmpty(comparabilityKey))
                return null;
            try
            {
                lock (_lock)
                {
                    baselines = ReadBaselines(out state);
                }
                string label;
                if (baselines.Baselines.TryGetValue(GHPerformanceSuiteLogic.BaselineKey(comparabilityKey, deviceKey), out label)
                    && !string.IsNullOrEmpty(label))
                    return label;
                return baselines.Baselines.TryGetValue(comparabilityKey, out label) ? label : null;
            }
            catch
            {
                return null;
            }
        }

        /* The device key of the suite: its fingerprint's, falling back to the
           manifest's platform and device model */
        private static string DeviceKeyOf(ManifestJson manifest)
        {
            EnvironmentJson env = manifest.Environment;
            if (env == null)
                return GHPerformanceSuiteLogic.DeviceKey(null, null, null);
            return GHPerformanceSuiteLogic.DeviceKey(env.Fingerprint, env.Platform, env.DeviceModel);
        }

        /* Compares this suite's arm against the recorded baseline arm for its
           comparability key and device (GetBaselineLabel), over every finished suite
           that shares the key and was measured on this suite's hardware (SameDevice).
           Refuses (returns false, with a short sentence in refusal) when the suite
           cannot be read, has not finished or was interrupted (EffectiveStatus), no
           baseline is set for its key (naming a baseline that differs only in the
           refresh rate or thermal settings, GHPerformanceSuiteLogic.DescribeKeyMismatch) or baselines.json
           cannot be read, the suite's own label is the baseline label, the baseline arm
           has no used runs on this device, or either arm has no used runs. The report also
           carries each arm's pooled fingerprint and the comparison recomputed without
           the runs whose background verdict was elevated. */
        public static bool TryCompareWithBaseline(string suiteId, out GHComparisonResult result, out string reportText, out string refusal)
        {
            result = null;
            reportText = null;
            refusal = null;
            try
            {
                ManifestJson manifest;
                string dir;
                lock (_lock)
                {
                    manifest = ReadSuiteManifest(suiteId, out dir);
                }
                if (manifest == null || manifest.Setup == null)
                {
                    refusal = "This suite could not be read.";
                    return false;
                }
                string abortReason;
                string status = EffectiveStatus(manifest, out abortReason);
                if (string.Equals(status, StatusRunning, StringComparison.Ordinal))
                {
                    refusal = "This suite has not finished running.";
                    return false;
                }
                if (string.Equals(status, StatusAborted, StringComparison.Ordinal)
                    && string.Equals(abortReason, InterruptedReason, StringComparison.Ordinal))
                {
                    refusal = "This suite was interrupted and cannot be compared.";
                    return false;
                }
                string key = manifest.ComparabilityKey;
                string label = manifest.Setup.ArmLabel;
                if (string.IsNullOrEmpty(key))
                {
                    refusal = "This suite has no comparability key yet.";
                    return false;
                }

                string deviceKey = DeviceKeyOf(manifest);
                BaselinesState baselinesState;
                BaselinesJson baselines;
                string baselineLabel = GetBaselineLabel(key, deviceKey, out baselinesState, out baselines);
                if (string.IsNullOrEmpty(baselineLabel))
                {
                    if (baselinesState == BaselinesState.Corrupt)
                    {
                        refusal = BaselinesFileName + " cannot be read, so no baseline is known. "
                            + "Set a baseline to start a new one.";
                        return false;
                    }
                    string mismatch = baselines != null
                        ? GHPerformanceSuiteLogic.DescribeKeyMismatch(GHPerformanceSuiteLogic.BaselineKey(key, deviceKey),
                            baselines.Baselines.Keys)
                        : null;
                    refusal = mismatch != null
                        ? "No baseline for this group on this device; " + mismatch + "."
                        : "No baseline is set for this suite's comparability group on this device.";
                    return false;
                }
                if (string.Equals(baselineLabel, label, StringComparison.Ordinal))
                {
                    refusal = "This suite belongs to the baseline arm.";
                    return false;
                }

                GHReportSuite candidate = MapToReportSuite(manifest);
                List<GHReportSuite> suitesA = LoadReportSuitesForArm(key, baselineLabel);
                List<GHReportSuite> suitesB = LoadReportSuitesForArm(key, label);
                List<GHReportSuite> otherDeviceA = RemoveOtherDevices(suitesA, candidate);
                RemoveOtherDevices(suitesB, candidate);
                List<GHSmoothnessSummary> usedA = UsedSummaries(suitesA, false);
                List<GHSmoothnessSummary> usedB = UsedSummaries(suitesB, false);
                if (usedA.Count == 0 && otherDeviceA.Count > 0)
                {
                    refusal = "Baseline measured on a different device (" + DeviceLabel(otherDeviceA[0])
                        + " vs " + DeviceLabel(candidate) + ").";
                    return false;
                }
                if (usedA.Count == 0 || usedB.Count == 0)
                {
                    refusal = "One of the two arms has no used runs.";
                    return false;
                }

                double targetFps = MedianTargetFps(suitesA, suitesB);
                double targetPeriodMs = targetFps > 0 ? 1000.0 / targetFps : 1000.0 / 60.0;

                result = GHPerformanceComparison.CompareSmoothness(baselineLabel, usedA, label, usedB, targetPeriodMs,
                    GHPerformanceComparison.DefaultResamples, GHPerformanceComparison.DefaultSeed);

                GHReportComparisonContext context = new GHReportComparisonContext();
                context.FingerprintA = GHPerformanceTextReport.PooledFingerprint(suitesA);
                context.FingerprintB = GHPerformanceTextReport.PooledFingerprint(suitesB);
                context.ElevatedRuns = CountElevatedUsedRuns(suitesA) + CountElevatedUsedRuns(suitesB);
                if (context.ElevatedRuns > 0)
                {
                    List<GHSmoothnessSummary> quietA = UsedSummaries(suitesA, true);
                    List<GHSmoothnessSummary> quietB = UsedSummaries(suitesB, true);
                    if (quietA.Count > 0 && quietB.Count > 0)
                        context.WithoutElevated = GHPerformanceComparison.CompareSmoothness(baselineLabel, quietA, label,
                            quietB, targetPeriodMs, GHPerformanceComparison.DefaultResamples, GHPerformanceComparison.DefaultSeed);
                }
                reportText = GHPerformanceTextReport.ComparisonReport(result, suitesA, suitesB, context);
                return true;
            }
            catch
            {
                result = null;
                reportText = null;
                refusal = "The comparison could not be computed.";
                return false;
            }
        }

        /* Deletes the suite's whole directory. Leaves baselines.json alone: a baseline
           label may still apply to other suites sharing its comparability key. */
        public static bool DeleteSuite(string suiteId)
        {
            try
            {
                lock (_lock)
                {
                    string dir = SuiteDirectory(suiteId);
                    if (dir == null || !Directory.Exists(dir))
                        return false;
                    Directory.Delete(dir, true);
                    return true;
                }
            }
            catch
            {
                return false;
            }
        }

        /* Deletes all performance data: the store root (suites, diagnostics, baselines.json),
           the archive's performance directory, share zips and import staging folders, and on
           Android the external export directory. False, with the first failure in error, when
           anything could not be deleted; the remaining locations are still attempted. */
        public static bool DeleteAllPerformanceData(out string error)
        {
            string firstError = null;
            lock (_lock)
            {
                string archiveDir = Path.Combine(GHApp.GHPath, GHConstants.ArchiveDirectory);
                List<string> directories = new List<string>();
                directories.Add(Path.Combine(GHApp.GHPath, GHConstants.PerformanceDirectory));
                directories.Add(Path.Combine(archiveDir, "performance"));
#if GNH_MAUI && ANDROID
                try
                {
                    Java.IO.File external = Android.App.Application.Context.GetExternalFilesDir(null);
                    if (external != null)
                        directories.Add(Path.Combine(external.AbsolutePath, "performance"));
                }
                catch (Exception ex)
                {
                    firstError = ex.Message;
                }
#endif
                try
                {
                    if (Directory.Exists(archiveDir))
                    {
                        directories.AddRange(Directory.GetDirectories(archiveDir, ImportStagingPrefix + "*"));
                        foreach (string zip in Directory.GetFiles(archiveDir, ShareZipPrefix + "*.zip"))
                        {
                            try
                            {
                                File.Delete(zip);
                            }
                            catch (Exception ex)
                            {
                                if (firstError == null)
                                    firstError = ex.Message;
                            }
                        }
                    }
                }
                catch (Exception ex)
                {
                    if (firstError == null)
                        firstError = ex.Message;
                }

                foreach (string dir in directories)
                {
                    try
                    {
                        if (Directory.Exists(dir))
                            Directory.Delete(dir, true);
                    }
                    catch (Exception ex)
                    {
                        if (firstError == null)
                            firstError = ex.Message;
                    }
                }
            }
            error = firstError;
            return firstError == null;
        }

        /* Zips every listed suite's directory (report.txt written first if missing, and
           a comparison.txt added when TryCompareWithBaseline succeeds for it) into
           GHPath/archive/GnollHack_Performance_<device>_<timestamp>.zip, leaving out
           temp, backup and set-aside corrupt files (GHAtomicFile.IsTransientName) and
           every suite that cannot be read. Returns the zip path, or null on failure or
           when no listed suite could be read. The caller shares it through
           GHApp.ShareFile. */
        public static string BuildShareZip(IList<string> suiteIds)
        {
            if (suiteIds == null || suiteIds.Count == 0)
                return null;
            try
            {
                string archiveDir = Path.Combine(GHApp.GHPath, GHConstants.ArchiveDirectory);
                GHApp.CheckCreateDirectory(archiveDir);

                GHPerformanceEnvironmentFacts facts = GHPerformanceEnvironment.Capture();
                string deviceToken = FoldToken(!string.IsNullOrEmpty(facts.DeviceModel) ? facts.DeviceModel : "unknown");
                string stamp = DateTime.UtcNow.ToString("yyyyMMdd_HHmmss", CultureInfo.InvariantCulture);
                string zipPath = Path.Combine(archiveDir, ShareZipPrefix + deviceToken + "_" + stamp + ".zip");
                if (File.Exists(zipPath))
                    File.Delete(zipPath);

                int sharedSuites = 0;
                using (ZipArchive archive = ZipFile.Open(zipPath, ZipArchiveMode.Create))
                {
                    for (int i = 0; i < suiteIds.Count; i++)
                    {
                        string suiteId = suiteIds[i];
                        string dir;
                        ManifestJson sharedManifest;
                        lock (_lock)
                        {
                            sharedManifest = ReadSuiteManifest(suiteId, out dir);
                        }
                        if (sharedManifest == null || !Directory.Exists(dir))
                            continue;

                        WriteReport(suiteId);
                        sharedSuites++;

                        string[] files = Directory.GetFiles(dir);
                        for (int f = 0; f < files.Length; f++)
                        {
                            string fileName = Path.GetFileName(files[f]);
                            if (GHAtomicFile.IsTransientName(fileName))
                                continue;
                            archive.CreateEntryFromFile(files[f], suiteId + "/" + fileName);
                        }

                        GHComparisonResult comparison;
                        string comparisonText;
                        string refusal;
                        if (TryCompareWithBaseline(suiteId, out comparison, out comparisonText, out refusal)
                            && !string.IsNullOrEmpty(comparisonText))
                        {
                            ZipArchiveEntry entry = archive.CreateEntry(suiteId + "/" + ComparisonFileName);
                            using (Stream entryStream = entry.Open())
                            using (StreamWriter writer = new StreamWriter(entryStream, new UTF8Encoding(false)))
                            {
                                writer.Write(comparisonText);
                            }
                        }
                    }
                }
                if (sharedSuites == 0)
                {
                    File.Delete(zipPath);
                    return null;
                }
                return zipPath;
            }
            catch
            {
                return null;
            }
        }

        /* Validates every top-level folder of the zip without extracting anything into
           the store (a valid suite.json, and every runFile it names present alongside it
           with a matching schemaVersion), then extracts only the valid folders into a
           fresh staging directory and moves each into SuitesDirectory unless a suite by
           that id already exists there. The staging directory is always removed. Never
           overwrites an existing suite. */
        public static GHPerformanceImportSummary ImportZip(string zipPath)
        {
            GHPerformanceImportSummary summary = new GHPerformanceImportSummary();
            if (string.IsNullOrEmpty(zipPath) || !File.Exists(zipPath))
            {
                summary.Errors.Add("Zip file not found.");
                return summary;
            }

            string archiveRoot = Path.Combine(GHApp.GHPath, GHConstants.ArchiveDirectory);
            string stagingDir = Path.Combine(archiveRoot, ImportStagingPrefix + DateTime.UtcNow.Ticks.ToString(CultureInfo.InvariantCulture));

            try
            {
                GHApp.CheckCreateDirectory(archiveRoot);

                Dictionary<string, List<string>> byFolder;
                List<string> invalidEntries;
                if (!GroupEntriesByFolder(zipPath, out byFolder, out invalidEntries))
                {
                    summary.Errors.Add("The zip could not be opened.");
                    return summary;
                }
                for (int i = 0; i < invalidEntries.Count; i++)
                    summary.Errors.Add(invalidEntries[i] + ": invalid path");

                List<string> validFolders = new List<string>();
                foreach (KeyValuePair<string, List<string>> entry in byFolder)
                {
                    string reason;
                    if (ValidateFolderManifest(zipPath, entry.Key, entry.Value, out reason))
                        validFolders.Add(entry.Key);
                    else
                        summary.Errors.Add(entry.Key + ": " + reason);
                }
                if (validFolders.Count == 0)
                    return summary;

                Directory.CreateDirectory(stagingDir);
                ExtractFolders(zipPath, byFolder, validFolders, stagingDir);

                for (int i = 0; i < validFolders.Count; i++)
                {
                    string folder = validFolders[i];
                    string stagedFolderPath = Path.Combine(stagingDir, folder);
                    string targetPath = SuiteDirectory(folder);
                    if (targetPath == null)
                    {
                        summary.Errors.Add(folder + ": invalid suite id");
                        continue;
                    }
                    try
                    {
                        if (Directory.Exists(targetPath))
                        {
                            summary.Skipped.Add(folder);
                            continue;
                        }
                        MarkImported(stagedFolderPath);
                        Directory.Move(stagedFolderPath, targetPath);
                        summary.Imported.Add(folder);
                    }
                    catch (Exception ex)
                    {
                        summary.Errors.Add(folder + ": " + ex.Message);
                    }
                }
            }
            catch (Exception ex)
            {
                summary.Errors.Add(ex.Message);
            }
            finally
            {
                try
                {
                    if (Directory.Exists(stagingDir))
                        Directory.Delete(stagingDir, true);
                }
                catch
                {
                    /* Best effort cleanup */
                }
            }
            return summary;
        }

        /* ---- suite id allocation ---- */

        private static string AllocateSuiteId(DateTime startedUtc, string scenario, string deviceModel)
        {
            string stamp = startedUtc.ToString("yyyyMMdd_HHmmss", CultureInfo.InvariantCulture);
            string scenarioToken = FoldToken(scenario);
            string deviceToken = FoldToken(!string.IsNullOrEmpty(deviceModel) ? deviceModel : "unknown");
            string baseId = stamp + "_" + scenarioToken + "_" + deviceToken;
            string candidate = baseId;
            int n = 2;
            while (Directory.Exists(SuiteDirectory(candidate)))
            {
                candidate = baseId + "_" + n.ToString(CultureInfo.InvariantCulture);
                n++;
            }
            return candidate;
        }

        /* Folds value to [A-Za-z0-9-], every other character becoming '-'; "unknown" for
           an empty value. */
        private static string FoldToken(string value)
        {
            if (string.IsNullOrEmpty(value))
                return "unknown";
            StringBuilder sb = new StringBuilder(value.Length);
            for (int i = 0; i < value.Length; i++)
            {
                char c = value[i];
                bool ok = (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '-';
                sb.Append(ok ? c : '-');
            }
            return sb.ToString();
        }

        /* ---- manifest construction ---- */

        private static SetupJson BuildSetupJson(GHPerformanceSuiteSetup setup, string replaySha256, long replayBytes)
        {
            SetupJson j = new SetupJson();
            j.ReplayFileName = !string.IsNullOrEmpty(setup.ReplayPath) ? Path.GetFileName(setup.ReplayPath) : null;
            j.ReplayBytes = replayBytes;
            j.ReplaySha256 = replaySha256;
            j.StartTurn = setup.StartTurn;
            j.Scenario = setup.Scenario;
            j.Runs = setup.Runs;
            j.ArmLabel = setup.ArmLabel;
            j.PageMode = setup.PageMode;
            j.WarmUpRun = setup.WarmUpRun;
            j.WarmUpSeconds = setup.WarmUpSeconds;
            j.WindowSeconds = setup.WindowSeconds;
            j.CooldownSeconds = setup.CooldownSeconds;
            j.ThermalGate = GHPerformanceSuiteLogic.NormalizeThermalGate(setup.ThermalGate) ?? GHPerformanceSuiteLogic.ThermalGateStart;
            j.ThermalWaitSeconds = setup.ThermalWaitSeconds;
            return j;
        }

        private static EnvironmentJson BuildEnvironmentJson(GHPerformanceEnvironmentFacts f)
        {
            EnvironmentJson j = new EnvironmentJson();
            j.Platform = f.Platform;
            j.DeviceModel = f.DeviceModel;
            j.DeviceOs = f.DeviceOs;
            j.AppVersion = f.AppVersion;
            j.GitCommit = f.GitCommit;
            j.BuildConfiguration = f.BuildConfiguration;
            j.RuntimeVersion = f.RuntimeVersion;
            j.FrameworkVersion = f.FrameworkVersion;
            j.UiFrameworkVersion = f.UiFrameworkVersion;
            j.SkiaSharpVersion = f.SkiaSharpVersion;
            j.FmodVersion = f.FmodVersion;
            j.MapRefreshRateSetting = f.MapRefreshRateSetting;
            j.Fingerprint = f.Fingerprint;
            return j;
        }

        /* The run's background verdict and headline values; null when there is no report */
        private static RunBackgroundJson BuildRunBackgroundJson(GHBackgroundReport r)
        {
            if (r == null)
                return null;
            RunBackgroundJson j = new RunBackgroundJson();
            j.Verdict = r.VerdictName;
            if (r.Window != null)
            {
                j.OtherCpuP90Pct = RN(r.Window.OtherCpuP90Pct);
                j.DiskBusyP90Pct = RN(r.Window.DiskBusyP90Pct);
                j.AvailableMemoryMinPct = RN(r.Window.AvailableMemoryMinPct);
            }
            j.Reason = r.Reason;
            return j;
        }

        private static ThermalJson BuildThermalJson(GHThermalReading r)
        {
            ThermalJson j = new ThermalJson();
            j.Status = GHThermalProbe.StatusName(r.Status);
            j.CpuPerformancePct = R(r.CpuPerformancePct);
            return j;
        }

        private static SummaryJson BuildSummaryJson(GHSmoothnessSummary s)
        {
            SummaryJson j = new SummaryJson();
            j.MetricsVersion = s.MetricsVersion;
            j.DisplayedFps = R(s.DisplayedFps);
            j.HitchRatioMsPerSec = R(s.HitchRatioMsPerSec);
            j.PacingErrorRmsMs = R(s.PacingErrorRmsMs);
            j.PacingErrorP99Ms = R(s.PacingErrorP99Ms);
            j.JudderPct = R(s.JudderPct);
            j.DroppedCount = s.DroppedCount;
            j.DisplayedCount = s.DisplayedCount;
            j.GcCount = s.GcCount;
            j.ForcedGcCount = s.ForcedGcCount;
            j.ForcedGcHitchCount = s.ForcedGcHitchCount;
            j.ForcedGcHitchMs = R(s.ForcedGcHitchMs);
            j.GcPauseMs = R(s.GcPauseMs);
            j.GcPauseDataAvailable = s.GcPauseDataAvailable;
            j.MeasuredRefreshHz = R(s.MeasuredRefreshHz);
            j.TargetFps = R(s.TargetFps);
            j.CompositorReportsLost = s.CompositorReportsLost;
            j.CompositorCoverage = s.CompositorCoverage < 0 ? (double?)null : R(s.CompositorCoverage);
            j.VsyncCorrectedShare = s.VsyncCorrectedShare < 0 ? (double?)null : R(s.VsyncCorrectedShare);
            j.SyncOffsetP50Ms = R(s.SyncOffsetP50Ms);
            j.LongStallCount = s.LongStallCount;
            j.LongStallMs = R(s.LongStallMs);
            j.CauseCount = (int[])s.CauseCount.Clone();
            j.CauseHitchCount = (int[])s.CauseHitchCount.Clone();
            j.CauseMs = RoundArray(s.CauseMs);
            j.EventGapCount = (int[])s.EventGapCount.Clone();
            j.EventHitchCount = (int[])s.EventHitchCount.Clone();
            j.QuietGapCount = s.QuietGapCount;
            j.QuietHitchCount = s.QuietHitchCount;
            return j;
        }

        private static double[] RoundArray(double[] values)
        {
            if (values == null)
                return new double[0];
            double[] result = new double[values.Length];
            for (int i = 0; i < values.Length; i++)
                result[i] = R(values[i]);
            return result;
        }

        /* ---- comparability key ---- */

        private static string ComputeComparabilityKey(ManifestJson manifest, double measuredRefreshHz)
        {
            string scenario = manifest.Setup != null ? manifest.Setup.Scenario : null;
            string replaySha = manifest.Setup != null ? manifest.Setup.ReplaySha256 : null;
            int startTurn = manifest.Setup != null ? manifest.Setup.StartTurn : 0;
            string pageMode = manifest.Setup != null ? manifest.Setup.PageMode : null;
            string mapRefreshSetting = manifest.Environment != null ? manifest.Environment.MapRefreshRateSetting : null;
            string thermalGate = manifest.Setup != null ? manifest.Setup.ThermalGate : null;
            int thermalWait = manifest.Setup != null && manifest.Setup.ThermalWaitSeconds.HasValue
                ? manifest.Setup.ThermalWaitSeconds.Value : GHPerformanceSuiteLogic.DefaultThermalWaitSeconds;
            double roundedHz = double.IsNaN(measuredRefreshHz) || double.IsInfinity(measuredRefreshHz)
                ? 0 : Math.Round(measuredRefreshHz);
            return OrEmpty(scenario) + "|" + OrEmpty(replaySha) + "|" + startTurn.ToString(CultureInfo.InvariantCulture)
                + "|" + OrEmpty(pageMode) + "|" + OrEmpty(mapRefreshSetting) + "|"
                + roundedHz.ToString(CultureInfo.InvariantCulture)
                + "|m" + GHSmoothnessMetrics.MetricsVersion.ToString(CultureInfo.InvariantCulture)
                + GHPerformanceSuiteLogic.ThermalKeySegment(thermalGate, thermalWait);
        }

        private static string OrEmpty(string s)
        {
            return s ?? "";
        }

        /* ---- reading suites back out ---- */

        /* The suite in dir, identified by its folder name; null when that name is not a
           valid suite id; an entry with Unreadable set when the manifest cannot be read */
        private static GHPerformanceSuiteInfo TryBuildSuiteInfo(string dir)
        {
            try
            {
                string folder = Path.GetFileName(dir);
                if (!GHPerformanceSuiteId.IsValid(folder))
                    return null;
                ManifestJson manifest;
                lock (_lock)
                {
                    manifest = ReadManifest(dir);
                }
                if (manifest == null)
                    return BuildUnreadableSuiteInfo(folder, dir);

                GHPerformanceSuiteInfo info = new GHPerformanceSuiteInfo();
                info.SuiteId = folder;
                info.IdMismatch = !string.Equals(manifest.SuiteId, folder, StringComparison.Ordinal);
                info.Directory = dir;
                info.Scenario = manifest.Setup != null ? manifest.Setup.Scenario : null;
                info.ArmLabel = manifest.Setup != null ? manifest.Setup.ArmLabel : null;
                info.PageMode = manifest.Setup != null ? manifest.Setup.PageMode : null;
                info.StartedUtc = ParseIso(manifest.StartedUtc);
                string abortReason;
                info.Status = EffectiveStatus(manifest, out abortReason);
                info.AbortReason = abortReason;
                info.Origin = manifest.Origin;
                info.ComparabilityKey = manifest.ComparabilityKey;

                List<double> hitch = new List<double>();
                int used = 0;
                for (int i = 0; i < manifest.Runs.Count; i++)
                {
                    RunJson r = manifest.Runs[i];
                    if (IsUsedRun(r))
                    {
                        used++;
                        hitch.Add(r.Summary.HitchRatioMsPerSec);
                    }
                    if (r != null && r.ExcludedReason != null
                        && r.ExcludedReason.StartsWith(GHBackgroundLoad.ReasonPrefix, StringComparison.Ordinal))
                        info.HasBackgroundExclusion = true;
                }
                info.EnvironmentChanged = manifest.EnvironmentChanged.HasValue && manifest.EnvironmentChanged.Value;
                info.RunsUsed = used;
                info.MedianHitchRatioMsPerSec = GHPerformanceStatistics.MedianNearestRank(hitch);
                info.SizeBytes = DirectorySizeBytes(dir);
                info.IsBaseline = !info.IdMismatch
                    && !string.IsNullOrEmpty(info.ComparabilityKey) && !string.IsNullOrEmpty(info.ArmLabel)
                    && string.Equals(GetBaselineLabel(info.ComparabilityKey, DeviceKeyOf(manifest)), info.ArmLabel,
                        StringComparison.Ordinal);
                return info;
            }
            catch
            {
                return null;
            }
        }

        /* A suite whose manifest cannot be read: listed so that it can be deleted, dated
           by its folder's last write */
        private static GHPerformanceSuiteInfo BuildUnreadableSuiteInfo(string folder, string dir)
        {
            GHPerformanceSuiteInfo info = new GHPerformanceSuiteInfo();
            info.SuiteId = folder;
            info.Directory = dir;
            info.Unreadable = true;
            info.MedianHitchRatioMsPerSec = double.NaN;
            try
            {
                info.StartedUtc = Directory.GetLastWriteTimeUtc(dir);
            }
            catch
            {
                info.StartedUtc = DateTime.MinValue;
            }
            info.SizeBytes = DirectorySizeBytes(dir);
            return info;
        }

        /* A used run: not the warm-up run, no exclusion reason, and a summary */
        private static bool IsUsedRun(RunJson r)
        {
            return r != null && !r.IsWarmUp && string.IsNullOrEmpty(r.ExcludedReason) && r.Summary != null;
        }

        private static long DirectorySizeBytes(string dir)
        {
            long total = 0;
            try
            {
                string[] files = Directory.GetFiles(dir);
                for (int i = 0; i < files.Length; i++)
                {
                    try
                    {
                        total += new FileInfo(files[i]).Length;
                    }
                    catch
                    {
                        /* Skip a file that vanished or cannot be stat'ed */
                    }
                }
            }
            catch
            {
                /* dir may have been deleted concurrently */
            }
            return total;
        }

        private static GHReportSuite MapToReportSuite(ManifestJson manifest)
        {
            GHReportSuite suite = new GHReportSuite();
            suite.SuiteId = manifest.SuiteId;

            SetupJson setup = manifest.Setup;
            if (setup != null)
            {
                suite.Scenario = setup.Scenario;
                suite.ArmLabel = setup.ArmLabel;
                suite.PageMode = setup.PageMode;
                suite.WarmUpRun = setup.WarmUpRun;
                suite.RunsRequested = setup.Runs;
                suite.WarmUpSeconds = setup.WarmUpSeconds;
                suite.WindowSeconds = setup.WindowSeconds;
                suite.CooldownSeconds = setup.CooldownSeconds;
                /* Suites recorded before the thermal settings existed waited for the start status for at most 300 s */
                suite.ThermalGate = GHPerformanceSuiteLogic.NormalizeThermalGate(setup.ThermalGate) ?? GHPerformanceSuiteLogic.ThermalGateStart;
                suite.ThermalWaitSeconds = setup.ThermalWaitSeconds ?? GHPerformanceSuiteLogic.DefaultThermalWaitSeconds;
                suite.ReplayFileName = setup.ReplayFileName;
                suite.ReplayBytes = setup.ReplayBytes;
                suite.ReplaySha256 = setup.ReplaySha256;
                suite.StartTurn = setup.StartTurn;
            }

            string abortReason;
            suite.Status = EffectiveStatus(manifest, out abortReason);
            suite.AbortReason = abortReason;
            suite.StartedUtc = ParseIso(manifest.StartedUtc);
            suite.Origin = manifest.Origin;

            EnvironmentJson env = manifest.Environment;
            if (env != null)
            {
                suite.Platform = env.Platform;
                suite.DeviceModel = env.DeviceModel;
                suite.DeviceOs = env.DeviceOs;
                suite.AppVersion = env.AppVersion;
                suite.GitCommit = env.GitCommit;
                suite.BuildConfiguration = env.BuildConfiguration;
                suite.RuntimeVersion = env.RuntimeVersion;
                suite.FrameworkVersion = env.FrameworkVersion;
                suite.UiFrameworkVersion = env.UiFrameworkVersion;
                suite.SkiaSharpVersion = env.SkiaSharpVersion;
                suite.FmodVersion = env.FmodVersion;
                suite.MapFpsSetting = TrailingNumber(env.MapRefreshRateSetting);
                /* A manifest written before fingerprints existed maps its per-field
                   environment onto the same keys */
                if (env.Fingerprint != null && env.Fingerprint.Count > 0)
                    suite.Fingerprint = env.Fingerprint;
                else
                    suite.Fingerprint = GHEnvironmentFingerprint.FromLegacyFields(env.AppVersion, env.GitCommit,
                        env.BuildConfiguration, env.RuntimeVersion, env.FrameworkVersion, env.UiFrameworkVersion,
                        env.SkiaSharpVersion, env.FmodVersion, env.Platform, env.DeviceOs, env.DeviceModel,
                        env.MapRefreshRateSetting, null, null);
            }
            suite.FingerprintAtEnd = manifest.FingerprintAtEnd;
            suite.EnvironmentChanged = manifest.EnvironmentChanged.HasValue && manifest.EnvironmentChanged.Value;
            string samplerSetting;
            if (suite.Fingerprint != null
                && suite.Fingerprint.TryGetValue(GHEnvironmentFingerprint.SettingsKey("backgroundSampler"), out samplerSetting))
                suite.BackgroundSamplerEnabled = samplerSetting == "true";
            /* The setting is a MapRefreshRateStyle name such as "MapFPS60"; without digits the
               suite's measured target rate stands in */
            if (suite.MapFpsSetting <= 0)
                suite.MapFpsSetting = manifest.TargetFps;
            suite.MeasuredRefreshHz = manifest.MeasuredRefreshHz;

            for (int i = 0; i < manifest.Runs.Count; i++)
            {
                RunJson r = manifest.Runs[i];
                GHReportRun run = new GHReportRun();
                run.Index = r.Index;
                run.IsWarmUp = r.IsWarmUp;
                run.RunFileName = r.RunFile;
                run.TurnReached = r.TurnReached;
                run.TurnAtWindowStart = r.TurnAtWindowStart.HasValue ? r.TurnAtWindowStart.Value : -1;
                run.InputRecordsInWindow = r.InputRecordsInWindow.HasValue ? r.InputRecordsInWindow.Value : -1;
                run.ExcludedReason = r.ExcludedReason;
                run.Summary = ReconstructSummary(r.Summary);
                run.Notes = r.Notes;
                if (r.Background != null)
                {
                    run.BackgroundVerdict = r.Background.Verdict;
                    run.OtherCpuP90Pct = r.Background.OtherCpuP90Pct.HasValue ? r.Background.OtherCpuP90Pct.Value : double.NaN;
                    run.BackgroundReason = r.Background.Reason;
                }
                suite.Runs.Add(run);
            }
            return suite;
        }

        /* Rebuilds just the fields the manifest stored (see BuildSummaryJson): enough for
           GHPerformanceTextReport and GHPerformanceComparison.CompareSmoothness, not a
           full GHSmoothnessSummary. */
        private static GHSmoothnessSummary ReconstructSummary(SummaryJson j)
        {
            if (j == null)
                return null;
            GHSmoothnessSummary s = new GHSmoothnessSummary();
            /* A manifest written before metrics version 2 has no version */
            s.MetricsVersion = j.MetricsVersion > 0 ? j.MetricsVersion : 1;
            s.DisplayedFps = j.DisplayedFps;
            s.HitchRatioMsPerSec = j.HitchRatioMsPerSec;
            s.PacingErrorRmsMs = j.PacingErrorRmsMs;
            s.PacingErrorP99Ms = j.PacingErrorP99Ms;
            s.JudderPct = j.JudderPct;
            s.DroppedCount = j.DroppedCount;
            s.DisplayedCount = j.DisplayedCount;
            s.GcCount = j.GcCount;
            s.ForcedGcCount = j.ForcedGcCount;
            s.ForcedGcHitchCount = j.ForcedGcHitchCount;
            s.ForcedGcHitchMs = j.ForcedGcHitchMs;
            s.GcPauseMs = j.GcPauseMs;
            s.GcPauseDataAvailable = j.GcPauseDataAvailable;
            s.MeasuredRefreshHz = j.MeasuredRefreshHz;
            s.TargetFps = j.TargetFps;
            s.CompositorReportsLost = j.CompositorReportsLost;
            s.CompositorCoverage = j.CompositorCoverage.HasValue ? j.CompositorCoverage.Value : -1;
            s.VsyncCorrectedShare = j.VsyncCorrectedShare.HasValue ? j.VsyncCorrectedShare.Value : -1;
            s.SyncOffsetP50Ms = j.SyncOffsetP50Ms;
            s.LongStallCount = j.LongStallCount;
            s.LongStallMs = j.LongStallMs;
            CopyInto(s.CauseCount, j.CauseCount);
            CopyInto(s.CauseHitchCount, j.CauseHitchCount);
            CopyInto(s.CauseMs, j.CauseMs);
            CopyInto(s.EventGapCount, j.EventGapCount);
            CopyInto(s.EventHitchCount, j.EventHitchCount);
            s.QuietGapCount = j.QuietGapCount;
            s.QuietHitchCount = j.QuietHitchCount;
            return s;
        }

        private static void CopyInto(int[] target, int[] source)
        {
            if (target == null || source == null)
                return;
            int n = Math.Min(target.Length, source.Length);
            for (int i = 0; i < n; i++)
                target[i] = source[i];
        }

        private static void CopyInto(double[] target, double[] source)
        {
            if (target == null || source == null)
                return;
            int n = Math.Min(target.Length, source.Length);
            for (int i = 0; i < n; i++)
                target[i] = source[i];
        }

        /* ---- comparison support ---- */

        private static List<GHReportSuite> LoadReportSuitesForArm(string comparabilityKey, string armLabel)
        {
            List<GHReportSuite> list = new List<GHReportSuite>();
            List<GHPerformanceSuiteInfo> all = ListSuites();
            for (int i = 0; i < all.Count; i++)
            {
                GHPerformanceSuiteInfo info = all[i];
                if (!string.Equals(info.ComparabilityKey, comparabilityKey, StringComparison.Ordinal)
                    || !string.Equals(info.ArmLabel, armLabel, StringComparison.Ordinal))
                    continue;
                GHReportSuite suite = LoadReportSuite(info.SuiteId);
                if (suite != null)
                    list.Add(suite);
            }
            return list;
        }

        /* The used runs' summaries; with skipElevated, without the runs whose background
           verdict was elevated */
        private static List<GHSmoothnessSummary> UsedSummaries(List<GHReportSuite> suites, bool skipElevated)
        {
            List<GHSmoothnessSummary> list = new List<GHSmoothnessSummary>();
            for (int i = 0; i < suites.Count; i++)
            {
                GHReportSuite suite = suites[i];
                for (int j = 0; j < suite.Runs.Count; j++)
                {
                    GHReportRun r = suite.Runs[j];
                    if (!r.IsWarmUp && string.IsNullOrEmpty(r.ExcludedReason) && r.Summary != null
                        && !(skipElevated && IsElevated(r)))
                        list.Add(r.Summary);
                }
            }
            return list;
        }

        private static bool IsElevated(GHReportRun r)
        {
            return r.BackgroundVerdict == GHBackgroundLoad.VerdictElevatedName;
        }

        private static int CountElevatedUsedRuns(List<GHReportSuite> suites)
        {
            int n = 0;
            for (int i = 0; i < suites.Count; i++)
            {
                GHReportSuite suite = suites[i];
                for (int j = 0; j < suite.Runs.Count; j++)
                {
                    GHReportRun r = suite.Runs[j];
                    if (!r.IsWarmUp && string.IsNullOrEmpty(r.ExcludedReason) && r.Summary != null && IsElevated(r))
                        n++;
                }
            }
            return n;
        }

        /* Removes from suites those not measured on candidate's device, returning them */
        private static List<GHReportSuite> RemoveOtherDevices(List<GHReportSuite> suites, GHReportSuite candidate)
        {
            List<GHReportSuite> removed = new List<GHReportSuite>();
            for (int i = suites.Count - 1; i >= 0; i--)
            {
                if (SameDevice(candidate, suites[i]))
                    continue;
                removed.Insert(0, suites[i]);
                suites.RemoveAt(i);
            }
            return removed;
        }

        /* Whether two suites ran on the same device: every hardware.* key and os.platform
           present in both fingerprints agrees; when no such key is present in both, the
           manifests' platform and device model agree. */
        private static bool SameDevice(GHReportSuite a, GHReportSuite b)
        {
            Dictionary<string, string> fa = a.Fingerprint;
            Dictionary<string, string> fb = b.Fingerprint;
            int compared = 0;
            if (fa != null && fb != null)
            {
                foreach (KeyValuePair<string, string> kv in fa)
                {
                    if (kv.Key != GHEnvironmentFingerprint.OsPlatformKey
                        && GHEnvironmentFingerprint.CategoryOf(kv.Key) != GHEnvironmentFingerprint.CategoryHardware)
                        continue;
                    string other;
                    if (!fb.TryGetValue(kv.Key, out other))
                        continue;
                    compared++;
                    if (!string.Equals(kv.Value, other, StringComparison.Ordinal))
                        return false;
                }
            }
            if (compared > 0)
                return true;
            return string.Equals(a.Platform, b.Platform, StringComparison.Ordinal)
                && string.Equals(a.DeviceModel, b.DeviceModel, StringComparison.Ordinal);
        }

        private static string DeviceLabel(GHReportSuite suite)
        {
            string model;
            if (suite.Fingerprint != null
                && suite.Fingerprint.TryGetValue(GHEnvironmentFingerprint.HardwareDeviceModelKey, out model)
                && !string.IsNullOrEmpty(model))
                return model;
            return !string.IsNullOrEmpty(suite.DeviceModel) ? suite.DeviceModel : "unknown device";
        }

        /* The most recent finished suite started before suite, with the same
           comparability key, on the same device; null when there is none or on failure */
        private static GHReportPreviousSuite FindPreviousSuite(GHReportSuite suite, string comparabilityKey)
        {
            if (suite == null || string.IsNullOrEmpty(comparabilityKey))
                return null;
            try
            {
                List<GHPerformanceSuiteInfo> all = ListSuites();
                for (int i = 0; i < all.Count; i++)
                {
                    GHPerformanceSuiteInfo info = all[i];
                    if (info.SuiteId == suite.SuiteId || info.Status == StatusRunning
                        || info.StartedUtc >= suite.StartedUtc
                        || !string.Equals(info.ComparabilityKey, comparabilityKey, StringComparison.Ordinal))
                        continue;
                    GHReportSuite previous = LoadReportSuite(info.SuiteId);
                    if (previous == null || !SameDevice(suite, previous))
                        continue;

                    GHReportPreviousSuite p = new GHReportPreviousSuite();
                    p.SuiteId = previous.SuiteId;
                    p.ArmLabel = previous.ArmLabel;
                    p.StartedUtc = previous.StartedUtc;
                    p.RunsUsed = info.RunsUsed;
                    p.MedianHitchRatioMsPerSec = info.MedianHitchRatioMsPerSec;
                    p.Changes.AddRange(GHEnvironmentFingerprint.Diff(previous.Fingerprint, suite.Fingerprint));
                    return p;
                }
            }
            catch
            {
                /* The report goes without the section */
            }
            return null;
        }

        /* Any changed, added or removed key outside meta and code */
        private static bool EnvironmentDiffers(Dictionary<string, string> atStart, Dictionary<string, string> atEnd)
        {
            List<GHFingerprintChange> changes = GHEnvironmentFingerprint.Diff(atStart, atEnd);
            for (int i = 0; i < changes.Count; i++)
            {
                GHFingerprintChange c = changes[i];
                if (c.Category == GHEnvironmentFingerprint.CategoryCode)
                    continue;
                if (c.Kind == GHFingerprintChangeKind.Changed || c.Kind == GHFingerprintChangeKind.Added
                    || c.Kind == GHFingerprintChangeKind.Removed)
                    return true;
            }
            return false;
        }

        private static double MedianTargetFps(List<GHReportSuite> suitesA, List<GHReportSuite> suitesB)
        {
            List<double> values = new List<double>();
            CollectTargetFps(suitesA, values);
            CollectTargetFps(suitesB, values);
            double median = GHPerformanceStatistics.MedianNearestRank(values);
            return double.IsNaN(median) ? 0 : median;
        }

        private static void CollectTargetFps(List<GHReportSuite> suites, List<double> values)
        {
            for (int i = 0; i < suites.Count; i++)
            {
                GHReportSuite suite = suites[i];
                for (int j = 0; j < suite.Runs.Count; j++)
                {
                    GHSmoothnessSummary s = suite.Runs[j].Summary;
                    if (s != null && s.TargetFps > 0)
                        values.Add(s.TargetFps);
                }
            }
        }

        /* ---- import validation and extraction ---- */

        /* Groups every non-directory zip entry by its top-level folder; an entry whose
           path is not exactly "folder/file" (deeper nesting, a rooted path, "." or "..",
           or an invalid path character) is reported in invalidEntries instead of grouped.
           False when the zip itself could not be opened. */
        private static bool GroupEntriesByFolder(string zipPath, out Dictionary<string, List<string>> byFolder,
            out List<string> invalidEntries)
        {
            byFolder = new Dictionary<string, List<string>>(StringComparer.Ordinal);
            invalidEntries = new List<string>();
            try
            {
                using (ZipArchive archive = ZipFile.OpenRead(zipPath))
                {
                    foreach (ZipArchiveEntry entry in archive.Entries)
                    {
                        string fullName = entry.FullName;
                        if (string.IsNullOrEmpty(fullName) || fullName.EndsWith("/", StringComparison.Ordinal))
                            continue; /* a directory entry */

                        string folder, file;
                        if (!SplitEntryPath(fullName, out folder, out file))
                        {
                            invalidEntries.Add(fullName);
                            continue;
                        }

                        List<string> files;
                        if (!byFolder.TryGetValue(folder, out files))
                        {
                            files = new List<string>();
                            byFolder[folder] = files;
                        }
                        files.Add(file);
                    }
                }
                return true;
            }
            catch
            {
                return false;
            }
        }

        private static readonly char[] _invalidPathChars = Path.GetInvalidFileNameChars();

        /* True when path is exactly "folder/file": two segments, neither "." or "..",
           neither containing an invalid file name character, and neither rooted. */
        private static bool SplitEntryPath(string path, out string folder, out string file)
        {
            folder = null;
            file = null;
            if (string.IsNullOrEmpty(path))
                return false;
            string[] parts = path.Replace('\\', '/').Split('/');
            if (parts.Length != 2)
                return false;
            folder = parts[0];
            file = parts[1];
            if (string.IsNullOrEmpty(folder) || string.IsNullOrEmpty(file))
                return false;
            if (folder == "." || folder == ".." || file == "." || file == "..")
                return false;
            if (folder.IndexOfAny(_invalidPathChars) >= 0 || file.IndexOfAny(_invalidPathChars) >= 0)
                return false;
            if (Path.IsPathRooted(folder) || Path.IsPathRooted(file))
                return false;
            return true;
        }

        /* Validates one folder's suite.json (present, parses, carries manifestVersion, and
           a suiteId equal to the folder name, which must be a valid suite id) and every
           runFile it names (present in the same folder, parses, schemaVersion
           equal to GHPerformanceRunRecord.CurrentSchemaVersion), without extracting
           anything. reason is set only on failure. */
        private static bool ValidateFolderManifest(string zipPath, string folder, List<string> files, out string reason)
        {
            reason = null;
            try
            {
                if (!files.Contains(ManifestFileName))
                {
                    reason = "missing " + ManifestFileName;
                    return false;
                }

                using (ZipArchive archive = ZipFile.OpenRead(zipPath))
                {
                    string manifestText = ReadEntryText(archive, folder + "/" + ManifestFileName);
                    if (manifestText == null)
                    {
                        reason = "could not read " + ManifestFileName;
                        return false;
                    }

                    JObject manifestObj;
                    try
                    {
                        manifestObj = JObject.Parse(manifestText);
                    }
                    catch
                    {
                        reason = ManifestFileName + " is not valid JSON";
                        return false;
                    }
                    if (manifestObj["manifestVersion"] == null)
                    {
                        reason = ManifestFileName + " has no manifestVersion";
                        return false;
                    }

                    ManifestJson manifest;
                    try
                    {
                        manifest = manifestObj.ToObject<ManifestJson>();
                    }
                    catch
                    {
                        reason = ManifestFileName + " does not match the expected shape";
                        return false;
                    }
                    if (!GHPerformanceSuiteId.MatchesFolder(manifest != null ? manifest.SuiteId : null, folder))
                    {
                        reason = GHPerformanceSuiteId.IsValid(folder) ? "suiteId in " + ManifestFileName
                            + " does not match the folder name" : "the folder name is not a valid suite id";
                        return false;
                    }
                    if (manifest.Runs == null)
                        return true;

                    for (int i = 0; i < manifest.Runs.Count; i++)
                    {
                        string runFile = manifest.Runs[i].RunFile;
                        if (string.IsNullOrEmpty(runFile))
                            continue;
                        if (!files.Contains(runFile))
                        {
                            reason = "run file " + runFile + " is missing";
                            return false;
                        }

                        string runText = ReadEntryText(archive, folder + "/" + runFile);
                        if (runText == null)
                        {
                            reason = "run file " + runFile + " could not be read";
                            return false;
                        }

                        JObject runObj;
                        try
                        {
                            runObj = JObject.Parse(runText);
                        }
                        catch
                        {
                            reason = "run file " + runFile + " is not valid JSON";
                            return false;
                        }
                        JToken schemaToken = runObj["schemaVersion"];
                        if (schemaToken == null || schemaToken.Type != JTokenType.Integer
                            || schemaToken.Value<int>() != GHPerformanceRunRecord.CurrentSchemaVersion)
                        {
                            reason = "run file " + runFile + " has an unexpected schemaVersion";
                            return false;
                        }
                    }
                }
                return true;
            }
            catch (Exception ex)
            {
                reason = ex.Message;
                return false;
            }
        }

        private static string ReadEntryText(ZipArchive archive, string entryPath)
        {
            ZipArchiveEntry entry = archive.GetEntry(entryPath);
            if (entry == null)
                return null;
            using (Stream stream = entry.Open())
            using (StreamReader reader = new StreamReader(stream, Encoding.UTF8))
            {
                return reader.ReadToEnd();
            }
        }

        /* Extracts every file of every valid folder into stagingDir, except temp, backup
           and set-aside corrupt files (GHAtomicFile.IsTransientName), refusing to write
           outside it even if an entry's name would otherwise resolve there. */
        private static void ExtractFolders(string zipPath, Dictionary<string, List<string>> byFolder,
            List<string> validFolders, string stagingDir)
        {
            string stagingFull = Path.GetFullPath(stagingDir);
            using (ZipArchive archive = ZipFile.OpenRead(zipPath))
            {
                for (int i = 0; i < validFolders.Count; i++)
                {
                    string folder = validFolders[i];
                    List<string> files = byFolder[folder];
                    string folderDir = Path.Combine(stagingDir, folder);
                    Directory.CreateDirectory(folderDir);

                    for (int f = 0; f < files.Count; f++)
                    {
                        string file = files[f];
                        if (GHAtomicFile.IsTransientName(file))
                            continue; /* a temp, backup or set-aside corrupt file */
                        ZipArchiveEntry entry = archive.GetEntry(folder + "/" + file);
                        if (entry == null)
                            continue;
                        string destPath = Path.Combine(folderDir, file);
                        string destFull = Path.GetFullPath(destPath);
                        if (!destFull.StartsWith(stagingFull, StringComparison.OrdinalIgnoreCase))
                            continue; /* would escape the staging directory */
                        entry.ExtractToFile(destPath, true);
                    }
                }
            }
        }

        private static void MarkImported(string stagedFolderPath)
        {
            string path = Path.Combine(stagedFolderPath, ManifestFileName);
            ManifestJson manifest = ReadManifestFromPath(path);
            if (manifest == null)
                return;
            manifest.Origin = OriginImported;
            manifest.ImportedUtc = ToIso(DateTime.UtcNow);
            WriteManifestToPath(path, manifest);
        }

        /* ---- manifest and baselines file I/O (caller holds _lock) ---- */

        private static ManifestJson ReadManifest(string dir)
        {
            return ReadManifestFromPath(Path.Combine(dir, ManifestFileName));
        }

        /* The manifest of the suite whose folder is suiteId, with dir set to that folder;
           null (dir null too when the id is not valid) when the manifest cannot be read or
           its suiteId differs from the folder name */
        private static ManifestJson ReadSuiteManifest(string suiteId, out string dir)
        {
            dir = SuiteDirectory(suiteId);
            if (dir == null)
                return null;
            ManifestJson manifest = ReadManifest(dir);
            if (manifest == null || !string.Equals(manifest.SuiteId, suiteId, StringComparison.Ordinal))
                return null;
            return manifest;
        }

        private static ManifestJson ReadManifestFromPath(string path)
        {
            try
            {
                if (!File.Exists(path))
                    return null;
                string json = File.ReadAllText(path, Encoding.UTF8);
                ManifestJson manifest = JsonConvert.DeserializeObject<ManifestJson>(json, _jsonSettings);
                if (manifest != null && manifest.Runs == null)
                    manifest.Runs = new List<RunJson>();
                return manifest;
            }
            catch
            {
                return null;
            }
        }

        private static void WriteManifest(string dir, ManifestJson manifest)
        {
            WriteManifestToPath(Path.Combine(dir, ManifestFileName), manifest);
        }

        private static void WriteManifestToPath(string path, ManifestJson manifest)
        {
            string json = JsonConvert.SerializeObject(manifest, _jsonSettings);
            GHAtomicFile.WriteAllText(path, json, null);
        }

        private static string BaselinesPath
        {
            get { return Path.Combine(RootDirectory, BaselinesFileName); }
        }

        private static string BaselinesBackupPath
        {
            get { return BaselinesPath + GHAtomicFile.BackupSuffix; }
        }

        /* baselines.json, with state saying how it was read: Missing (no file; empty
           baselines), Ok, Recovered (it did not parse, and baselines.json.bak was read
           instead) or Corrupt (neither parsed; empty baselines). Never throws. */
        private static BaselinesJson ReadBaselines(out BaselinesState state)
        {
            try
            {
                string path = BaselinesPath;
                if (!File.Exists(path))
                {
                    state = BaselinesState.Missing;
                    return NewBaselines();
                }
                BaselinesJson baselines = TryParseBaselines(path);
                if (baselines != null)
                {
                    state = BaselinesState.Ok;
                    return baselines;
                }
                baselines = TryParseBaselines(BaselinesBackupPath);
                if (baselines != null)
                {
                    if (Interlocked.Exchange(ref _baselinesRecoveryLogged, 1) == 0)
                        Log(BaselinesFileName + " could not be read; using "
                            + BaselinesFileName + GHAtomicFile.BackupSuffix);
                    state = BaselinesState.Recovered;
                    return baselines;
                }
                state = BaselinesState.Corrupt;
                return NewBaselines();
            }
            catch
            {
                state = BaselinesState.Corrupt;
                return NewBaselines();
            }
        }

        /* The baselines file at path; null when it is missing or does not parse */
        private static BaselinesJson TryParseBaselines(string path)
        {
            try
            {
                if (!File.Exists(path))
                    return null;
                string json = File.ReadAllText(path, Encoding.UTF8);
                BaselinesJson baselines = JsonConvert.DeserializeObject<BaselinesJson>(json, _jsonSettings);
                if (baselines == null)
                    return null;
                if (baselines.Baselines == null)
                    baselines.Baselines = new Dictionary<string, string>();
                return baselines;
            }
            catch
            {
                return null;
            }
        }

        private static BaselinesJson NewBaselines()
        {
            BaselinesJson baselines = new BaselinesJson();
            baselines.ManifestVersion = CurrentManifestVersion;
            baselines.Baselines = new Dictionary<string, string>();
            return baselines;
        }

        /* Replaces baselines.json; with backupCurrent, the current file, which must have
           parsed, is first kept as baselines.json.bak */
        private static void WriteBaselines(BaselinesJson baselines, bool backupCurrent)
        {
            GHApp.CheckCreateDirectory(RootDirectory);
            string json = JsonConvert.SerializeObject(baselines, _jsonSettings);
            GHAtomicFile.WriteAllText(BaselinesPath, json, backupCurrent ? BaselinesBackupPath : null);
        }

        private static void Log(string text)
        {
            try
            {
                GHApp.MaybeWriteGHLog("Performance suite store: " + text);
            }
            catch
            {
                /* Logging must never break the store */
            }
        }

        /* ---- small shared helpers ---- */

        private static string ToIso(DateTime utc)
        {
            return utc.ToString("o", CultureInfo.InvariantCulture);
        }

        private static DateTime ParseIso(string text)
        {
            DateTime value;
            if (!string.IsNullOrEmpty(text) && DateTime.TryParse(text, CultureInfo.InvariantCulture,
                DateTimeStyles.RoundtripKind, out value))
                return value;
            return DateTime.MinValue;
        }

        /* NaN and Infinity are written as 0. This mirrors GHPerformanceRunRecord.R's
           convention for the per-run JSON, and keeps every manifest double a plain
           number: no nullable-double sentinel and no Newtonsoft NaN token. */
        private static double R(double value)
        {
            if (double.IsNaN(value) || double.IsInfinity(value))
                return 0;
            return Math.Round(value, 3);
        }

        /* A background value: NaN and Infinity are null, not 0 */
        private static double? RN(float value)
        {
            if (float.IsNaN(value) || float.IsInfinity(value))
                return null;
            return Math.Round((double)value, 3);
        }

        /* ---- manifest JSON shape ---- */

        private sealed class ManifestJson
        {
            [JsonProperty("manifestVersion")]
            public int ManifestVersion;

            [JsonProperty("suiteId")]
            public string SuiteId;

            [JsonProperty("status")]
            public string Status;

            [JsonProperty("abortReason")]
            public string AbortReason;

            [JsonProperty("origin")]
            public string Origin;

            [JsonProperty("importedUtc")]
            public string ImportedUtc;

            [JsonProperty("startedUtc")]
            public string StartedUtc;

            [JsonProperty("endedUtc")]
            public string EndedUtc;

            [JsonProperty("setup")]
            public SetupJson Setup;

            [JsonProperty("environment")]
            public EnvironmentJson Environment;

            /* The fingerprint re-captured when the suite finished; omitted before that */
            [JsonProperty("fingerprintAtEnd", NullValueHandling = NullValueHandling.Ignore)]
            public Dictionary<string, string> FingerprintAtEnd;

            /* Whether it differs from environment.fingerprint outside meta and code;
               omitted when either fingerprint is missing */
            [JsonProperty("environmentChanged", NullValueHandling = NullValueHandling.Ignore)]
            public bool? EnvironmentChanged;

            [JsonProperty("comparabilityKey")]
            public string ComparabilityKey;

            [JsonProperty("measuredRefreshHz")]
            public double MeasuredRefreshHz;

            [JsonProperty("targetFps")]
            public double TargetFps;

            [JsonProperty("runs")]
            public List<RunJson> Runs = new List<RunJson>();
        }

        private sealed class SetupJson
        {
            [JsonProperty("replayFileName")]
            public string ReplayFileName;

            [JsonProperty("replayBytes")]
            public long ReplayBytes;

            [JsonProperty("replaySha256")]
            public string ReplaySha256;

            [JsonProperty("startTurn")]
            public int StartTurn;

            [JsonProperty("scenario")]
            public string Scenario;

            [JsonProperty("runs")]
            public int Runs;

            [JsonProperty("armLabel")]
            public string ArmLabel;

            [JsonProperty("pageMode")]
            public string PageMode;

            [JsonProperty("warmUpRun")]
            public bool WarmUpRun;

            [JsonProperty("warmUpSeconds")]
            public int WarmUpSeconds;

            [JsonProperty("windowSeconds")]
            public int WindowSeconds;

            [JsonProperty("cooldownSeconds")]
            public int CooldownSeconds;

            [JsonProperty("thermalGate", NullValueHandling = NullValueHandling.Ignore)]
            public string ThermalGate;

            [JsonProperty("thermalWaitSeconds", NullValueHandling = NullValueHandling.Ignore)]
            public int? ThermalWaitSeconds;
        }

        private sealed class EnvironmentJson
        {
            [JsonProperty("platform")]
            public string Platform;

            [JsonProperty("deviceModel")]
            public string DeviceModel;

            [JsonProperty("deviceOs")]
            public string DeviceOs;

            [JsonProperty("appVersion")]
            public string AppVersion;

            [JsonProperty("gitCommit")]
            public string GitCommit;

            [JsonProperty("buildConfiguration")]
            public string BuildConfiguration;

            [JsonProperty("runtimeVersion")]
            public string RuntimeVersion;

            [JsonProperty("frameworkVersion")]
            public string FrameworkVersion;

            [JsonProperty("uiFrameworkVersion")]
            public string UiFrameworkVersion;

            [JsonProperty("skiaSharpVersion")]
            public string SkiaSharpVersion;

            [JsonProperty("fmodVersion")]
            public string FmodVersion;

            [JsonProperty("mapRefreshRateSetting")]
            public string MapRefreshRateSetting;

            /* The environment fingerprint when the suite's first window ended (at its
               creation until then); omitted in older manifests */
            [JsonProperty("fingerprint", NullValueHandling = NullValueHandling.Ignore)]
            public Dictionary<string, string> Fingerprint;
        }

        private sealed class RunJson
        {
            [JsonProperty("index")]
            public int Index;

            [JsonProperty("isWarmUp")]
            public bool IsWarmUp;

            /* JSON file name only, no directory; null when the run produced no record */
            [JsonProperty("runFile")]
            public string RunFile;

            [JsonProperty("turnReached")]
            public int TurnReached;

            /* Replay turn when the window opened; omitted when unknown */
            [JsonProperty("turnAtWindowStart", NullValueHandling = NullValueHandling.Ignore)]
            public int? TurnAtWindowStart;

            /* Replay input records played during the window; omitted when unknown */
            [JsonProperty("inputRecordsInWindow", NullValueHandling = NullValueHandling.Ignore)]
            public long? InputRecordsInWindow;

            [JsonProperty("excludedReason")]
            public string ExcludedReason;

            [JsonProperty("thermalBefore")]
            public ThermalJson ThermalBefore;

            [JsonProperty("thermalAfter")]
            public ThermalJson ThermalAfter;

            /* Null when the run produced no record */
            [JsonProperty("summary")]
            public SummaryJson Summary;

            /* Omitted when the run has no background report */
            [JsonProperty("background", NullValueHandling = NullValueHandling.Ignore)]
            public RunBackgroundJson Background;

            /* e.g. a quiet gate timeout before the run; omitted when not set */
            [JsonProperty("notes", NullValueHandling = NullValueHandling.Ignore)]
            public string Notes;
        }

        /* A run's background verdict and headline window values; null where the platform
           did not report the signal */
        private sealed class RunBackgroundJson
        {
            [JsonProperty("verdict")]
            public string Verdict;

            [JsonProperty("otherCpuP90Pct")]
            public double? OtherCpuP90Pct;

            [JsonProperty("diskBusyP90Pct")]
            public double? DiskBusyP90Pct;

            [JsonProperty("availableMemoryMinPct")]
            public double? AvailableMemoryMinPct;

            [JsonProperty("reason")]
            public string Reason;
        }

        private sealed class ThermalJson
        {
            [JsonProperty("status")]
            public string Status;

            [JsonProperty("cpuPerformancePct")]
            public double CpuPerformancePct;
        }

        /* The subset of GHSmoothnessSummary a run needs for the text report and the
           baseline comparison; see ReconstructSummary. */
        private sealed class SummaryJson
        {
            /* GHSmoothnessMetrics.MetricsVersion of the analysis; absent (0) means 1 */
            [JsonProperty("metricsVersion")]
            public int MetricsVersion;

            [JsonProperty("displayedFps")]
            public double DisplayedFps;

            [JsonProperty("hitchRatioMsPerSec")]
            public double HitchRatioMsPerSec;

            [JsonProperty("pacingErrorRmsMs")]
            public double PacingErrorRmsMs;

            [JsonProperty("pacingErrorP99Ms")]
            public double PacingErrorP99Ms;

            [JsonProperty("judderPct")]
            public double JudderPct;

            [JsonProperty("droppedCount")]
            public int DroppedCount;

            [JsonProperty("displayedCount")]
            public int DisplayedCount;

            [JsonProperty("gcCount")]
            public int GcCount;

            /* Absent in older summaries, which reads as 0 */
            [JsonProperty("forcedGcCount")]
            public int ForcedGcCount;

            /* Absent in older summaries, which reads as 0 */
            [JsonProperty("forcedGcHitchCount")]
            public int ForcedGcHitchCount;

            /* Absent in older summaries, which reads as 0 */
            [JsonProperty("forcedGcHitchMs")]
            public double ForcedGcHitchMs;

            [JsonProperty("gcPauseMs")]
            public double GcPauseMs;

            [JsonProperty("gcPauseDataAvailable")]
            public bool GcPauseDataAvailable;

            [JsonProperty("measuredRefreshHz")]
            public double MeasuredRefreshHz;

            [JsonProperty("targetFps")]
            public double TargetFps;

            [JsonProperty("compositorReportsLost")]
            public int CompositorReportsLost;

            /* Share of GL-thread paints carried by a FrameMetrics report; null (or absent) where not applicable */
            [JsonProperty("compositorCoverage")]
            public double? CompositorCoverage;

            /* Share of ticks whose stale Choreographer frame time was corrected; null (or absent) where not applicable */
            [JsonProperty("vsyncCorrectedShare")]
            public double? VsyncCorrectedShare;

            [JsonProperty("syncOffsetP50Ms")]
            public double SyncOffsetP50Ms;

            [JsonProperty("longStallCount")]
            public int LongStallCount;

            [JsonProperty("longStallMs")]
            public double LongStallMs;

            [JsonProperty("causeCount")]
            public int[] CauseCount;

            /* Of causeCount, the hitches alone; absent (null) before metrics version 2 */
            [JsonProperty("causeHitchCount")]
            public int[] CauseHitchCount;

            [JsonProperty("causeMs")]
            public double[] CauseMs;

            [JsonProperty("eventGapCount")]
            public int[] EventGapCount;

            [JsonProperty("eventHitchCount")]
            public int[] EventHitchCount;

            [JsonProperty("quietGapCount")]
            public int QuietGapCount;

            [JsonProperty("quietHitchCount")]
            public int QuietHitchCount;
        }

        /* baselines.json: one recorded baseline arm label per comparability key and device
           (GHPerformanceSuiteLogic.BaselineKey), or per comparability key alone */
        private sealed class BaselinesJson
        {
            [JsonProperty("manifestVersion")]
            public int ManifestVersion;

            [JsonProperty("baselines")]
            public Dictionary<string, string> Baselines;
        }
    }
}
