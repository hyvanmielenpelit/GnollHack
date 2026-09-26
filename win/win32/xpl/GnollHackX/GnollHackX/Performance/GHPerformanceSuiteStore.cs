using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.IO.Compression;
using System.Security.Cryptography;
using System.Text;
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
        public bool IsBaseline;                   /* the baseline label for this key equals ArmLabel */
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
       arm label per comparability key.

       The manifest's top level never carries a property named schemaVersion, smoothness
       or series, so the offline analyzer's "every *.json in the folder is a run record"
       scan cannot mistake it for one; manifestVersion is its own version tag instead.
       Every double written to a manifest maps NaN and Infinity to 0 (see R below), the
       same convention GHPerformanceRunRecord.R uses for the per-run JSON, so the file
       never carries Newtonsoft's non-standard NaN token.

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

        public const string OriginLocal = "local";
        public const string OriginImported = "imported";

        private const string SuitesFolderName = "suites";
        private const string ManifestFileName = "suite.json";
        private const string ReportFileName = "report.txt";
        private const string ComparisonFileName = "comparison.txt";
        private const string BaselinesFileName = "baselines.json";
        private const string ImportStagingPrefix = "import_";
        private const string ShareZipPrefix = "GnollHack_Performance_";

        private static readonly object _lock = new object();

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

        public static string SuiteDirectory(string suiteId)
        {
            return Path.Combine(SuitesDirectory, suiteId ?? "");
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
           status "running", environment captured now, and startedUtc now. Returns the new
           suiteId, or null on failure. */
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
                    string dir = SuiteDirectory(suiteId);
                    ManifestJson manifest = ReadManifest(dir);
                    if (manifest == null)
                        return;

                    RunJson run = new RunJson();
                    run.Index = context.RunIndex;
                    run.IsWarmUp = context.IsWarmUp;
                    run.TurnReached = context.TurnReached;

                    if (result != null)
                    {
                        run.RunFile = result.JsonPath != null ? Path.GetFileName(result.JsonPath) : null;
                        run.ExcludedReason = result.ExcludedReason;
                        run.ThermalBefore = BuildThermalJson(result.ThermalBefore);
                        run.ThermalAfter = BuildThermalJson(result.ThermalAfter);
                        run.Summary = result.Summary != null ? BuildSummaryJson(result.Summary) : null;
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

        /* Sets status and endedUtc, computes measuredRefreshHz/targetFps as the median
           over every run with a summary (warm-up and excluded runs included), computes
           comparabilityKey from that measured refresh rate, rewrites suite.json, and
           writes report.txt. */
        public static void FinishSuite(string suiteId, string status, string abortReason)
        {
            if (string.IsNullOrEmpty(suiteId))
                return;
            try
            {
                lock (_lock)
                {
                    string dir = SuiteDirectory(suiteId);
                    ManifestJson manifest = ReadManifest(dir);
                    if (manifest == null)
                        return;

                    manifest.Status = !string.IsNullOrEmpty(status) ? status : StatusComplete;
                    manifest.AbortReason = abortReason;
                    manifest.EndedUtc = ToIso(DateTime.UtcNow);

                    List<double> measured = new List<double>();
                    List<double> target = new List<double>();
                    for (int i = 0; i < manifest.Runs.Count; i++)
                    {
                        SummaryJson s = manifest.Runs[i].Summary;
                        if (s == null)
                            continue;
                        measured.Add(s.MeasuredRefreshHz);
                        target.Add(s.TargetFps);
                    }
                    double measuredMedian = Median(measured);
                    manifest.MeasuredRefreshHz = R(measuredMedian);
                    manifest.TargetFps = R(Median(target));
                    manifest.ComparabilityKey = ComputeComparabilityKey(manifest, measuredMedian);

                    WriteManifest(dir, manifest);
                }
                WriteReport(suiteId);
            }
            catch
            {
                /* Best effort; the suite's run files are unaffected */
            }
        }

        /* Every suite under SuitesDirectory that can be read, newest StartedUtc first;
           an unreadable folder is skipped rather than failing the whole listing. */
        public static List<GHPerformanceSuiteInfo> ListSuites()
        {
            List<GHPerformanceSuiteInfo> list = new List<GHPerformanceSuiteInfo>();
            try
            {
                string root = SuitesDirectory;
                if (!Directory.Exists(root))
                    return list;
                string[] dirs = Directory.GetDirectories(root);
                for (int i = 0; i < dirs.Length; i++)
                {
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
           when the suite cannot be read. */
        public static GHReportSuite LoadReportSuite(string suiteId)
        {
            try
            {
                ManifestJson manifest;
                lock (_lock)
                {
                    manifest = ReadManifest(SuiteDirectory(suiteId));
                }
                return manifest != null ? MapToReportSuite(manifest) : null;
            }
            catch
            {
                return null;
            }
        }

        public static string ReportPath(string suiteId)
        {
            return Path.Combine(SuiteDirectory(suiteId), ReportFileName);
        }

        /* Renders and writes report.txt for the suite; returns its path, or null on
           failure (including when the suite cannot be loaded). */
        public static string WriteReport(string suiteId)
        {
            try
            {
                GHReportSuite suite = LoadReportSuite(suiteId);
                if (suite == null)
                    return null;
                string text = GHPerformanceTextReport.SuiteReport(suite);
                string path = ReportPath(suiteId);
                string dir = Path.GetDirectoryName(path);
                if (!string.IsNullOrEmpty(dir) && !Directory.Exists(dir))
                    GHApp.CheckCreateDirectory(dir);
                File.WriteAllText(path, text, new UTF8Encoding(false));
                return path;
            }
            catch
            {
                return null;
            }
        }

        /* Records this suite's arm label as the baseline for its comparability key.
           False when the suite has no manifest, no setup or no comparabilityKey yet
           (i.e. it has not been finished). */
        public static bool SetBaseline(string suiteId)
        {
            try
            {
                ManifestJson manifest;
                lock (_lock)
                {
                    manifest = ReadManifest(SuiteDirectory(suiteId));
                }
                if (manifest == null || manifest.Setup == null || string.IsNullOrEmpty(manifest.ComparabilityKey))
                    return false;

                lock (_lock)
                {
                    BaselinesJson baselines = ReadBaselines();
                    baselines.Baselines[manifest.ComparabilityKey] = manifest.Setup.ArmLabel;
                    WriteBaselines(baselines);
                }
                return true;
            }
            catch
            {
                return false;
            }
        }

        /* The arm label recorded as the baseline for comparabilityKey, or null when none
           is set. */
        public static string GetBaselineLabel(string comparabilityKey)
        {
            if (string.IsNullOrEmpty(comparabilityKey))
                return null;
            try
            {
                BaselinesJson baselines;
                lock (_lock)
                {
                    baselines = ReadBaselines();
                }
                string label;
                return baselines.Baselines.TryGetValue(comparabilityKey, out label) ? label : null;
            }
            catch
            {
                return null;
            }
        }

        /* Compares this suite's arm against the recorded baseline arm for its
           comparability key, over every finished suite that shares the key. Refuses
           (returns false, with a short sentence in refusal) when the suite has not
           finished, no baseline is set for its key, the suite's own label is the
           baseline label, or either arm has no used runs. */
        public static bool TryCompareWithBaseline(string suiteId, out GHComparisonResult result, out string reportText, out string refusal)
        {
            result = null;
            reportText = null;
            refusal = null;
            try
            {
                ManifestJson manifest;
                lock (_lock)
                {
                    manifest = ReadManifest(SuiteDirectory(suiteId));
                }
                if (manifest == null || manifest.Setup == null)
                {
                    refusal = "This suite could not be read.";
                    return false;
                }
                if (string.Equals(manifest.Status, StatusRunning, StringComparison.Ordinal))
                {
                    refusal = "This suite has not finished running.";
                    return false;
                }
                string key = manifest.ComparabilityKey;
                string label = manifest.Setup.ArmLabel;
                if (string.IsNullOrEmpty(key))
                {
                    refusal = "This suite has no comparability key yet.";
                    return false;
                }

                string baselineLabel = GetBaselineLabel(key);
                if (string.IsNullOrEmpty(baselineLabel))
                {
                    refusal = "No baseline is set for this suite's comparability group.";
                    return false;
                }
                if (string.Equals(baselineLabel, label, StringComparison.Ordinal))
                {
                    refusal = "This suite belongs to the baseline arm.";
                    return false;
                }

                List<GHReportSuite> suitesA = LoadReportSuitesForArm(key, baselineLabel);
                List<GHReportSuite> suitesB = LoadReportSuitesForArm(key, label);
                List<GHSmoothnessSummary> usedA = UsedSummaries(suitesA);
                List<GHSmoothnessSummary> usedB = UsedSummaries(suitesB);
                if (usedA.Count == 0 || usedB.Count == 0)
                {
                    refusal = "One of the two arms has no used runs.";
                    return false;
                }

                double targetFps = MedianTargetFps(suitesA, suitesB);
                double targetPeriodMs = targetFps > 0 ? 1000.0 / targetFps : 1000.0 / 60.0;

                result = GHPerformanceComparison.CompareSmoothness(baselineLabel, usedA, label, usedB, targetPeriodMs,
                    GHPerformanceComparison.DefaultResamples, GHPerformanceComparison.DefaultSeed);
                reportText = GHPerformanceTextReport.ComparisonReport(result, suitesA, suitesB);
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
                    if (!Directory.Exists(dir))
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

        /* Zips every listed suite's directory (report.txt written first if missing, and
           a comparison.txt added when TryCompareWithBaseline succeeds for it) into
           GHPath/archive/GnollHack_Performance_<device>_<timestamp>.zip. Returns the zip
           path, or null on failure. The caller shares it through GHApp.ShareFile. */
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

                using (ZipArchive archive = ZipFile.Open(zipPath, ZipArchiveMode.Create))
                {
                    for (int i = 0; i < suiteIds.Count; i++)
                    {
                        string suiteId = suiteIds[i];
                        string dir = SuiteDirectory(suiteId);
                        if (!Directory.Exists(dir))
                            continue;

                        WriteReport(suiteId);

                        string[] files = Directory.GetFiles(dir);
                        for (int f = 0; f < files.Length; f++)
                        {
                            string entryName = suiteId + "/" + Path.GetFileName(files[f]);
                            archive.CreateEntryFromFile(files[f], entryName);
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
            j.DisplayedFps = R(s.DisplayedFps);
            j.HitchRatioMsPerSec = R(s.HitchRatioMsPerSec);
            j.PacingErrorRmsMs = R(s.PacingErrorRmsMs);
            j.PacingErrorP99Ms = R(s.PacingErrorP99Ms);
            j.JudderPct = R(s.JudderPct);
            j.DroppedCount = s.DroppedCount;
            j.DisplayedCount = s.DisplayedCount;
            j.GcCount = s.GcCount;
            j.GcPauseMs = R(s.GcPauseMs);
            j.GcPauseDataAvailable = s.GcPauseDataAvailable;
            j.MeasuredRefreshHz = R(s.MeasuredRefreshHz);
            j.TargetFps = R(s.TargetFps);
            j.CauseCount = (int[])s.CauseCount.Clone();
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
            double roundedHz = double.IsNaN(measuredRefreshHz) || double.IsInfinity(measuredRefreshHz)
                ? 0 : Math.Round(measuredRefreshHz);
            return OrEmpty(scenario) + "|" + OrEmpty(replaySha) + "|" + startTurn.ToString(CultureInfo.InvariantCulture)
                + "|" + OrEmpty(pageMode) + "|" + OrEmpty(mapRefreshSetting) + "|"
                + roundedHz.ToString(CultureInfo.InvariantCulture);
        }

        private static string OrEmpty(string s)
        {
            return s ?? "";
        }

        /* ---- reading suites back out ---- */

        private static GHPerformanceSuiteInfo TryBuildSuiteInfo(string dir)
        {
            try
            {
                ManifestJson manifest;
                lock (_lock)
                {
                    manifest = ReadManifest(dir);
                }
                if (manifest == null)
                    return null;

                GHPerformanceSuiteInfo info = new GHPerformanceSuiteInfo();
                info.SuiteId = manifest.SuiteId;
                info.Directory = dir;
                info.Scenario = manifest.Setup != null ? manifest.Setup.Scenario : null;
                info.ArmLabel = manifest.Setup != null ? manifest.Setup.ArmLabel : null;
                info.PageMode = manifest.Setup != null ? manifest.Setup.PageMode : null;
                info.StartedUtc = ParseIso(manifest.StartedUtc);
                info.Status = manifest.Status;
                info.AbortReason = manifest.AbortReason;
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
                }
                info.RunsUsed = used;
                info.MedianHitchRatioMsPerSec = Median(hitch);
                info.SizeBytes = DirectorySizeBytes(dir);
                info.IsBaseline = !string.IsNullOrEmpty(info.ComparabilityKey) && !string.IsNullOrEmpty(info.ArmLabel)
                    && string.Equals(GetBaselineLabel(info.ComparabilityKey), info.ArmLabel, StringComparison.Ordinal);
                return info;
            }
            catch
            {
                return null;
            }
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
                suite.ReplayFileName = setup.ReplayFileName;
                suite.ReplayBytes = setup.ReplayBytes;
                suite.ReplaySha256 = setup.ReplaySha256;
                suite.StartTurn = setup.StartTurn;
            }

            suite.Status = manifest.Status;
            suite.AbortReason = manifest.AbortReason;
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
                double parsed;
                suite.MapFpsSetting = double.TryParse(env.MapRefreshRateSetting, NumberStyles.Float,
                    CultureInfo.InvariantCulture, out parsed) ? parsed : 0;
            }
            suite.MeasuredRefreshHz = manifest.MeasuredRefreshHz;

            for (int i = 0; i < manifest.Runs.Count; i++)
            {
                RunJson r = manifest.Runs[i];
                GHReportRun run = new GHReportRun();
                run.Index = r.Index;
                run.IsWarmUp = r.IsWarmUp;
                run.RunFileName = r.RunFile;
                run.TurnReached = r.TurnReached;
                run.ExcludedReason = r.ExcludedReason;
                run.Summary = ReconstructSummary(r.Summary);
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
            s.DisplayedFps = j.DisplayedFps;
            s.HitchRatioMsPerSec = j.HitchRatioMsPerSec;
            s.PacingErrorRmsMs = j.PacingErrorRmsMs;
            s.PacingErrorP99Ms = j.PacingErrorP99Ms;
            s.JudderPct = j.JudderPct;
            s.DroppedCount = j.DroppedCount;
            s.DisplayedCount = j.DisplayedCount;
            s.GcCount = j.GcCount;
            s.GcPauseMs = j.GcPauseMs;
            s.GcPauseDataAvailable = j.GcPauseDataAvailable;
            s.MeasuredRefreshHz = j.MeasuredRefreshHz;
            s.TargetFps = j.TargetFps;
            CopyInto(s.CauseCount, j.CauseCount);
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

        private static List<GHSmoothnessSummary> UsedSummaries(List<GHReportSuite> suites)
        {
            List<GHSmoothnessSummary> list = new List<GHSmoothnessSummary>();
            for (int i = 0; i < suites.Count; i++)
            {
                GHReportSuite suite = suites[i];
                for (int j = 0; j < suite.Runs.Count; j++)
                {
                    GHReportRun r = suite.Runs[j];
                    if (!r.IsWarmUp && string.IsNullOrEmpty(r.ExcludedReason) && r.Summary != null)
                        list.Add(r.Summary);
                }
            }
            return list;
        }

        private static double MedianTargetFps(List<GHReportSuite> suitesA, List<GHReportSuite> suitesB)
        {
            List<double> values = new List<double>();
            CollectTargetFps(suitesA, values);
            CollectTargetFps(suitesB, values);
            double median = Median(values);
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

        /* Validates one folder's suite.json (present, parses, carries manifestVersion)
           and every runFile it names (present in the same folder, parses, schemaVersion
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
                    if (manifest == null || manifest.Runs == null)
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

        /* Extracts every file of every valid folder into stagingDir, refusing to write
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
            File.WriteAllText(path, json, new UTF8Encoding(false));
        }

        private static string BaselinesPath
        {
            get { return Path.Combine(RootDirectory, BaselinesFileName); }
        }

        private static BaselinesJson ReadBaselines()
        {
            try
            {
                string path = BaselinesPath;
                if (!File.Exists(path))
                    return NewBaselines();
                string json = File.ReadAllText(path, Encoding.UTF8);
                BaselinesJson baselines = JsonConvert.DeserializeObject<BaselinesJson>(json, _jsonSettings);
                if (baselines == null)
                    return NewBaselines();
                if (baselines.Baselines == null)
                    baselines.Baselines = new Dictionary<string, string>();
                return baselines;
            }
            catch
            {
                return NewBaselines();
            }
        }

        private static BaselinesJson NewBaselines()
        {
            BaselinesJson baselines = new BaselinesJson();
            baselines.ManifestVersion = CurrentManifestVersion;
            baselines.Baselines = new Dictionary<string, string>();
            return baselines;
        }

        private static void WriteBaselines(BaselinesJson baselines)
        {
            GHApp.CheckCreateDirectory(RootDirectory);
            string json = JsonConvert.SerializeObject(baselines, _jsonSettings);
            File.WriteAllText(BaselinesPath, json, new UTF8Encoding(false));
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

        /* Median of a small, unsorted list; NaN for an empty list. Sorts a copy. */
        private static double Median(List<double> values)
        {
            if (values == null || values.Count == 0)
                return double.NaN;
            double[] a = values.ToArray();
            Array.Sort(a);
            int n = a.Length;
            if (n % 2 == 1)
                return a[n / 2];
            return (a[n / 2 - 1] + a[n / 2]) / 2.0;
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

            [JsonProperty("excludedReason")]
            public string ExcludedReason;

            [JsonProperty("thermalBefore")]
            public ThermalJson ThermalBefore;

            [JsonProperty("thermalAfter")]
            public ThermalJson ThermalAfter;

            /* Null when the run produced no record */
            [JsonProperty("summary")]
            public SummaryJson Summary;
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

            [JsonProperty("gcPauseMs")]
            public double GcPauseMs;

            [JsonProperty("gcPauseDataAvailable")]
            public bool GcPauseDataAvailable;

            [JsonProperty("measuredRefreshHz")]
            public double MeasuredRefreshHz;

            [JsonProperty("targetFps")]
            public double TargetFps;

            [JsonProperty("causeCount")]
            public int[] CauseCount;

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

        /* baselines.json: one recorded baseline arm label per comparability key */
        private sealed class BaselinesJson
        {
            [JsonProperty("manifestVersion")]
            public int ManifestVersion;

            [JsonProperty("baselines")]
            public Dictionary<string, string> Baselines;
        }
    }
}
