using System;
using System.Collections.Generic;
using GnollHackX.Performance;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers GHPerformanceDiagnosis: the four health classes and their boundaries, every
       rule firing at its threshold and not just short of it, unknown (NaN and null)
       signals producing no finding, every cause location including the one by
       elimination, primary-cause ordering, the Checks table, and the report: its first
       line, the summary above the Result line, section order, line width, line endings,
       Facts and Environment blocks, the verbatim appendix, HeadlineOf and determinism. */
    public class GHPerformanceDiagnosisTests
    {
        private static readonly DateTime Now = new DateTime(2026, 9, 27, 14, 5, 0);

        /* A fully known, healthy, quiet run that produces no finding at all. Hitch-cause
           time is below the share gate (MinCauseMsPerSec over 30 s). Every gap of the
           1800 on-screen intervals is quiet, so PLAYER_INPUT stays silent. */
        private static GHDiagnosisFacts HealthyFacts()
        {
            GHDiagnosisFacts f = new GHDiagnosisFacts();
            f.WindowSeconds = 30f;
            f.NominalWindowSeconds = 30f;
            f.OnScreenIntervalCount = 1800;
            f.PausedGapCount = 0;
            f.QuietGapCount = 1800;
            f.QuietHitchCount = 2;
            f.EventGapCounts = new int[GHSmoothnessMetrics.ContentEventKinds];
            f.EventHitchCounts = new int[GHSmoothnessMetrics.ContentEventKinds];
            f.MetricsVersion = GHSmoothnessMetrics.MetricsVersion;
            f.LongStallCount = 0;
            f.LongStallMs = 0f;
            f.CompositorReportsLost = 0;
            f.PresentSource = GHPerformanceDiagnosis.PresentSourceEstimated;
            f.TargetFps = 60f;
            f.MeasuredRefreshHz = 60f;
            f.DisplayMaxRefreshHz = 60f;
            f.DisplayedFps = 59.5f;
            f.HitchRatioMsPerSec = 1f;
            f.HitchCount = 2;
            f.PacingErrorRmsMs = 0.4f;
            f.PaintP50Ms = 2.1f;
            f.PaintP99Ms = 5.3f;
            f.GcCount = 1;
            f.GcPauseMs = 0.8f;
            f.CauseMs = new double[GHSmoothnessMetrics.CauseCount];
            f.CauseMs[(int)GHHitchCause.Unattributed] = 10;

            f.ThermalRankBefore = 1;
            f.ThermalRankAfter = 1;
            f.HeadroomAfter = 0.3f;
            f.PowerStateKnown = true;
            f.IsCharging = true;

            f.BackgroundVerdict = GHBackgroundVerdict.Quiet;
            f.OtherCpuP90Pct = 2f;
            f.DiskBusyP90Pct = 3f;
            f.AvailableMemoryMinPct = 45f;
            f.AvailableMemoryMinMB = 7000;
            f.HardFaultsP90PerSec = 5f;
            f.OtherGpuPct = 1f;
            f.Processes = new List<GHProcessLoad> { new GHProcessLoad("explorer", 1f, float.NaN) };

            f.OwnMemoryBytes = 512L * 1024 * 1024;
            f.DeviceMemoryBytes = 16L * 1024 * 1024 * 1024;
            f.AllocationRateMBPerSec = 4f;

            f.MainCanvasGlRequested = true;
            f.MainCanvasGpuContextLive = true;
            f.GpuBackend = "OpenGL";
            f.RenderAdapterName = "NVIDIA GeForce RTX 3060";
            f.RenderAdapterIsIntegrated = false;
            f.RenderAdapterIsSoftware = false;
            f.DiscreteAdapterPresent = true;
            f.GpuPreference = "Dedicated";
            f.Adapters = new List<string> { "NVIDIA GeForce RTX 3060 (discrete)", "Intel(R) UHD Graphics 630 (integrated)" };

            f.NonDefaultPerformanceSettings = new List<string>();
            f.PlatformRenderLoopOn = true;
            f.CountdownShown = true;

            f.Fingerprint = new Dictionary<string, string>
            {
                { "meta.fingerprintVersion", "1" },
                { "code.appVersion", "4.1.3" },
                { "component.SkiaSharp", "3.119.0" },
                { "component.System.Text.Json", "10.0.0" },
                { "settings.powerMode", "Balanced" },
                { "settings.powerPlan", "Balanced" },
                { "os.pendingReboot", "false" }
            };
            return f;
        }

        /* Healthy facts measured below the healthy frame rate */
        private static GHDiagnosisFacts DegradedFacts()
        {
            GHDiagnosisFacts f = HealthyFacts();
            f.DisplayedFps = 50f;
            return f;
        }

        /* Cause times: every cause zero except the given (cause, ms) pairs. The ms values are
           written per 1 ms/s of share gate and scaled by MinCauseMsPerSec, so a pair list
           summing to 30 ms over the 30 s window sits exactly at the gate. */
        private static void Causes(GHDiagnosisFacts f, params object[] pairs)
        {
            f.CauseMs = new double[GHSmoothnessMetrics.CauseCount];
            for (int i = 0; i + 1 < pairs.Length; i += 2)
                f.CauseMs[(int)(GHHitchCause)pairs[i]] = Convert.ToDouble(pairs[i + 1]) * GHPerformanceDiagnosis.MinCauseMsPerSec;
        }

        private static GHDiagnosisFinding Find(GHDiagnosisResult r, string code)
        {
            for (int i = 0; i < r.Findings.Count; i++)
            {
                if (r.Findings[i].Code == code)
                    return r.Findings[i];
            }
            return null;
        }

        private static void AssertFires(GHDiagnosisFacts f, string code, GHFindingSeverity severity)
        {
            GHDiagnosisFinding x = Find(GHPerformanceDiagnosis.Diagnose(f), code);
            Assert.NotNull(x);
            Assert.Equal(severity, x.Severity);
        }

        private static void AssertSilent(GHDiagnosisFacts f, string code)
        {
            Assert.Null(Find(GHPerformanceDiagnosis.Diagnose(f), code));
        }

        private static string[] Lines(string report)
        {
            return report.Split('\n');
        }

        private static string Report(GHDiagnosisFacts f)
        {
            return GHPerformanceDiagnosis.BuildReport(f, GHPerformanceDiagnosis.Diagnose(f), Now);
        }

        /* The index of the first line starting with "Result:", -1 when there is none */
        private static int ResultIndex(string[] lines)
        {
            return Array.FindIndex(lines, l => l.StartsWith("Result:", StringComparison.Ordinal));
        }

        /* The report's Result line */
        private static string ResultLineOf(string report)
        {
            string[] lines = Lines(report);
            int i = ResultIndex(lines);
            Assert.True(i >= 0, "no Result line");
            return lines[i];
        }

        /* The summary block: the lines from "Summary:" up to the blank line before Result */
        private static string[] SummaryLines(string report)
        {
            string[] lines = Lines(report);
            int start = Array.FindIndex(lines, l => l.StartsWith("Summary:", StringComparison.Ordinal));
            int result = ResultIndex(lines);
            Assert.True(start >= 0 && result > start + 1, "no summary above the Result line");
            Assert.Equal("", lines[result - 1]);
            string[] block = new string[result - 1 - start];
            Array.Copy(lines, start, block, 0, block.Length);
            return block;
        }

        /* Wrapped lines trimmed and joined by single spaces: the text before wrapping */
        private static string Joined(string[] lines)
        {
            string[] trimmed = new string[lines.Length];
            for (int i = 0; i < lines.Length; i++)
                trimmed[i] = lines[i].Trim();
            return string.Join(" ", trimmed);
        }

        private static int CountCodes(GHDiagnosisResult r, string code)
        {
            int n = 0;
            for (int i = 0; i < r.Findings.Count; i++)
            {
                if (r.Findings[i].Code == code)
                    n++;
            }
            return n;
        }

        /* ------------------------------------------------------------------ Health */

        [Fact]
        public void HealthyFacts_NoFindingsHealthyLocationNone()
        {
            GHDiagnosisResult r = GHPerformanceDiagnosis.Diagnose(HealthyFacts());

            Assert.Empty(r.Findings);
            Assert.Equal(GHDiagnosisHealth.Healthy, r.Health);
            Assert.Equal(GHCauseLocation.None, r.Location);
            Assert.Null(r.Primary);
            Assert.Equal(GHPerformanceDiagnosis.HealthyConclusion, r.Conclusion);
        }

        [Theory]
        [InlineData(57f, 4.9f, GHDiagnosisHealth.Healthy)]
        [InlineData(56.9f, 1f, GHDiagnosisHealth.Degraded)]
        [InlineData(60f, 5f, GHDiagnosisHealth.Degraded)]
        [InlineData(45f, 1f, GHDiagnosisHealth.Degraded)]
        [InlineData(44.9f, 1f, GHDiagnosisHealth.Poor)]
        [InlineData(60f, 9.9f, GHDiagnosisHealth.Degraded)]
        [InlineData(60f, 10f, GHDiagnosisHealth.Degraded)]
        [InlineData(60f, 10.1f, GHDiagnosisHealth.Poor)]
        public void Health_Boundaries(float fps, float hitch, GHDiagnosisHealth expected)
        {
            GHDiagnosisFacts f = HealthyFacts();
            f.DisplayedFps = fps;
            f.HitchRatioMsPerSec = hitch;

            Assert.Equal(expected, GHPerformanceDiagnosis.Diagnose(f).Health);
        }

        /* A 120 fps target on a 60 Hz display: judged against 60 */
        [Fact]
        public void Health_JudgedAgainstLowerMeasuredRefresh()
        {
            GHDiagnosisFacts f = HealthyFacts();
            f.TargetFps = 120f;
            f.DisplayedFps = 59f;

            Assert.Equal(60.0, GHPerformanceDiagnosis.EffectiveTargetFps(f), 3);
            Assert.Equal(GHDiagnosisHealth.Healthy, GHPerformanceDiagnosis.Diagnose(f).Health);

            f.DisplayedFps = 44.9f;
            Assert.Equal(GHDiagnosisHealth.Poor, GHPerformanceDiagnosis.Diagnose(f).Health);

            f.MeasuredRefreshHz = float.NaN;
            f.DisplayedFps = 59f;
            Assert.Equal(GHDiagnosisHealth.Poor, GHPerformanceDiagnosis.Diagnose(f).Health);
        }

        [Fact]
        public void Health_UnknownWindowFacts_JudgedAsBefore()
        {
            GHDiagnosisFacts f = HealthyFacts();
            f.NominalWindowSeconds = float.NaN;
            f.OnScreenIntervalCount = -1;
            f.WindowSeconds = 10f;
            Assert.Equal(GHDiagnosisHealth.Healthy, GHPerformanceDiagnosis.Diagnose(f).Health);

            GHDiagnosisFacts d = DegradedFacts();
            d.NominalWindowSeconds = float.NaN;
            d.OnScreenIntervalCount = -1;
            d.WindowSeconds = 10f;
            Assert.Equal(GHDiagnosisHealth.Degraded, GHPerformanceDiagnosis.Diagnose(d).Health);
        }

        [Fact]
        public void Health_InconclusiveWhenActiveWindowShort()
        {
            GHDiagnosisFacts f = HealthyFacts();
            f.WindowSeconds = 23.9f;
            GHDiagnosisResult r = GHPerformanceDiagnosis.Diagnose(f);
            Assert.Equal(GHDiagnosisHealth.Inconclusive, r.Health);
            Assert.Equal("the map was covered or paused for 6.1 s", r.InconclusiveReason);

            f.WindowSeconds = 24f;
            Assert.Equal(GHDiagnosisHealth.Healthy, GHPerformanceDiagnosis.Diagnose(f).Health);
        }

        [Fact]
        public void Health_InconclusiveBelow100Intervals()
        {
            GHDiagnosisFacts f = HealthyFacts();
            f.OnScreenIntervalCount = 99;
            GHDiagnosisResult r = GHPerformanceDiagnosis.Diagnose(f);
            Assert.Equal(GHDiagnosisHealth.Inconclusive, r.Health);
            Assert.Equal("too few frames were shown (99)", r.InconclusiveReason);

            f.OnScreenIntervalCount = 0;
            Assert.Equal(GHDiagnosisHealth.Inconclusive, GHPerformanceDiagnosis.Diagnose(f).Health);

            f.OnScreenIntervalCount = 100;
            Assert.Equal(GHDiagnosisHealth.Healthy, GHPerformanceDiagnosis.Diagnose(f).Health);
        }

        [Theory]
        [InlineData("renderLoopOff")]
        [InlineData("fpsNaN")]
        [InlineData("excluded")]
        [InlineData("targetNaN")]
        [InlineData("hitchNaN")]
        [InlineData("debugBuild")]
        [InlineData("debugger")]
        public void Health_Inconclusive_LocationUnclearWithReason(string variant)
        {
            GHDiagnosisFacts f = HealthyFacts();
            if (variant == "renderLoopOff")
                f.PlatformRenderLoopOn = false;
            else if (variant == "fpsNaN")
                f.DisplayedFps = float.NaN;
            else if (variant == "excluded")
                f.ExcludedReason = "aborted: page pushed";
            else if (variant == "targetNaN")
                f.TargetFps = float.NaN;
            else if (variant == "debugBuild")
                f.IsDebugBuild = true;
            else if (variant == "debugger")
                f.DebuggerAttached = true;
            else
                f.HitchRatioMsPerSec = float.NaN;

            GHDiagnosisResult r = GHPerformanceDiagnosis.Diagnose(f);

            Assert.Equal(GHDiagnosisHealth.Inconclusive, r.Health);
            Assert.Equal(GHCauseLocation.Unclear, r.Location);
            Assert.Null(r.Primary);
            Assert.False(string.IsNullOrEmpty(r.InconclusiveReason));
            Assert.Contains(r.InconclusiveReason, r.Conclusion);
        }

        [Fact]
        public void AllSignalsUnknown_NoFindings()
        {
            GHDiagnosisFacts f = new GHDiagnosisFacts();
            f.MainCanvasGlRequested = true;

            GHDiagnosisResult r = GHPerformanceDiagnosis.Diagnose(f);

            Assert.Empty(r.Findings);
            Assert.Equal(GHDiagnosisHealth.Inconclusive, r.Health);
            Assert.Equal(GHCauseLocation.Unclear, r.Location);
            Assert.Equal(8, r.Checks.Count);
            for (int i = 0; i < r.Checks.Count; i++)
            {
                if (r.Checks[i].Area == GHFindingArea.GpuPath)
                    Assert.Equal(GHPerformanceDiagnosis.StatusOk, r.Checks[i].Status);
                else
                    Assert.Equal(GHPerformanceDiagnosis.StatusNotAvailable, r.Checks[i].Status);
            }
        }

        [Fact]
        public void NullFacts_TreatedAsUnknown()
        {
            GHDiagnosisResult r = GHPerformanceDiagnosis.Diagnose(null);

            Assert.Equal(GHDiagnosisHealth.Inconclusive, r.Health);
            Assert.Equal(GHCauseLocation.Unclear, r.Location);
        }

        /* ------------------------------------------------------------------ Heat */

        [Fact]
        public void ThermalThrottled_Moderate_Suspect()
        {
            GHDiagnosisFacts f = HealthyFacts();
            f.ThermalRankBefore = 3;
            f.ThermalRankAfter = 3;
            AssertFires(f, GHPerformanceDiagnosis.CodeThermalThrottled, GHFindingSeverity.Suspect);

            f.ThermalRankBefore = 2;
            f.ThermalRankAfter = 2;
            AssertSilent(f, GHPerformanceDiagnosis.CodeThermalThrottled);
        }

        [Fact]
        public void ThermalThrottled_Severe_Likely()
        {
            GHDiagnosisFacts f = HealthyFacts();
            f.ThermalRankBefore = 4;
            f.ThermalRankAfter = 4;
            AssertFires(f, GHPerformanceDiagnosis.CodeThermalThrottled, GHFindingSeverity.Likely);

            f.ThermalRankBefore = 3;
            AssertFires(f, GHPerformanceDiagnosis.CodeThermalThrottled, GHFindingSeverity.Likely);
        }

        [Fact]
        public void ThermalThrottled_SevereBefore_Likely()
        {
            GHDiagnosisFacts f = HealthyFacts();
            f.ThermalRankBefore = 4;
            f.ThermalRankAfter = 3;
            AssertFires(f, GHPerformanceDiagnosis.CodeThermalThrottled, GHFindingSeverity.Likely);
        }

        [Fact]
        public void ThermalThrottled_RoseTwoClasses_Likely()
        {
            GHDiagnosisFacts f = HealthyFacts();
            f.ThermalRankBefore = 1;
            f.ThermalRankAfter = 3;
            AssertFires(f, GHPerformanceDiagnosis.CodeThermalThrottled, GHFindingSeverity.Likely);
            AssertSilent(f, GHPerformanceDiagnosis.CodeThermalRising);
        }

        [Fact]
        public void ThermalThrottled_RoseTwoClassesWithClockDrop_Likely()
        {
            GHDiagnosisFacts f = HealthyFacts();
            f.ThermalRankBefore = 1;
            f.ThermalRankAfter = 3;
            f.CpuPerformancePctBefore = 95f;
            f.CpuPerformancePctAfter = 70f;
            AssertFires(f, GHPerformanceDiagnosis.CodeThermalThrottled, GHFindingSeverity.Likely);
            AssertFires(f, GHPerformanceDiagnosis.CodeCpuClockCapped, GHFindingSeverity.Suspect);
        }

        [Theory]
        [InlineData(1, 4, 0.95f, GHPerformanceDiagnosis.CodeThermalThrottled, GHFindingSeverity.Likely)]
        [InlineData(2, 3, 0.95f, GHPerformanceDiagnosis.CodeThermalThrottled, GHFindingSeverity.Suspect)]
        [InlineData(1, 2, 0.9f, GHPerformanceDiagnosis.CodeThermalRising, GHFindingSeverity.Suspect)]
        [InlineData(1, 2, float.NaN, GHPerformanceDiagnosis.CodeThermalRising, GHFindingSeverity.Info)]
        public void Thermal_CodesMutuallyExclusive(int before, int after, float headroom, string code,
                                                   GHFindingSeverity severity)
        {
            GHDiagnosisFacts f = HealthyFacts();
            f.ThermalRankBefore = before;
            f.ThermalRankAfter = after;
            f.HeadroomAfter = headroom;

            GHDiagnosisResult r = GHPerformanceDiagnosis.Diagnose(f);

            Assert.Equal(1, CountCodes(r, GHPerformanceDiagnosis.CodeThermalThrottled)
                + CountCodes(r, GHPerformanceDiagnosis.CodeThermalRising));
            Assert.Equal(severity, Find(r, code).Severity);
        }

        [Fact]
        public void ClassifyThrottleCpuDrop_IsClockCapNotThermal()
        {
            GHDiagnosisFacts f = HealthyFacts();
            f.CpuPerformancePctAfter = 89.9f;
            AssertFires(f, GHPerformanceDiagnosis.CodeCpuClockCapped, GHFindingSeverity.Suspect);
            AssertSilent(f, GHPerformanceDiagnosis.CodeThermalThrottled);

            f.CpuPerformancePctAfter = 90f;
            AssertSilent(f, GHPerformanceDiagnosis.CodeCpuClockCapped);
            AssertSilent(f, GHPerformanceDiagnosis.CodeThermalThrottled);
        }

        [Fact]
        public void Primary_BusyBackgroundOutranksClockDrop()
        {
            GHDiagnosisFacts f = DegradedFacts();
            f.CpuPerformancePctBefore = 92f;
            f.CpuPerformancePctAfter = 71f;                         /* Heat, Suspect */
            f.BackgroundVerdict = GHBackgroundVerdict.Busy;         /* Background, Likely */

            GHDiagnosisResult r = GHPerformanceDiagnosis.Diagnose(f);

            Assert.Equal(GHPerformanceDiagnosis.CodeBackgroundBusy, r.Primary.Code);
            AssertSilent(f, GHPerformanceDiagnosis.CodeThermalThrottled);
        }

        [Theory]
        [InlineData(1, 2, float.NaN, GHFindingSeverity.Info)]
        [InlineData(2, 2, float.NaN, null)]
        [InlineData(0, 2, float.NaN, null)]
        [InlineData(1, 1, 0.85f, GHFindingSeverity.Suspect)]
        [InlineData(1, 1, 0.849f, null)]
        public void ThermalRising_Boundaries(int before, int after, float headroom, GHFindingSeverity? expected)
        {
            GHDiagnosisFacts f = HealthyFacts();
            f.ThermalRankBefore = before;
            f.ThermalRankAfter = after;
            f.HeadroomAfter = headroom;

            if (expected.HasValue)
                AssertFires(f, GHPerformanceDiagnosis.CodeThermalRising, expected.Value);
            else
                AssertSilent(f, GHPerformanceDiagnosis.CodeThermalRising);
        }

        [Fact]
        public void CpuClockCapped_Boundary()
        {
            /* A steady clock, so only the absolute floor applies, not ClassifyThrottle's drop rule */
            GHDiagnosisFacts f = HealthyFacts();
            f.CpuPerformancePctBefore = 59.9f;
            f.CpuPerformancePctAfter = 59.9f;
            AssertFires(f, GHPerformanceDiagnosis.CodeCpuClockCapped, GHFindingSeverity.Info);

            f.CpuPerformancePctBefore = 60f;
            f.CpuPerformancePctAfter = 60f;
            AssertSilent(f, GHPerformanceDiagnosis.CodeCpuClockCapped);
        }

        [Fact]
        public void CpuClockCapped_DropIsSuspect()
        {
            GHDiagnosisFacts f = HealthyFacts();
            f.CpuPerformancePctBefore = 95f;
            f.CpuPerformancePctAfter = 80f;
            AssertFires(f, GHPerformanceDiagnosis.CodeCpuClockCapped, GHFindingSeverity.Suspect);
        }

        [Fact]
        public void CpuClockCapped_DropOfExactlyTen_IsNotSuspect()
        {
            GHDiagnosisFacts f = HealthyFacts();
            f.CpuPerformancePctBefore = 95f;
            f.CpuPerformancePctAfter = 85f;
            AssertSilent(f, GHPerformanceDiagnosis.CodeCpuClockCapped);
        }

        [Fact]
        public void CpuClockCapped_LowWithoutDropIsInfo()
        {
            GHDiagnosisFacts f = HealthyFacts();
            f.CpuPerformancePctBefore = 55f;
            f.CpuPerformancePctAfter = 55f;
            AssertFires(f, GHPerformanceDiagnosis.CodeCpuClockCapped, GHFindingSeverity.Info);
            Assert.Contains("normal under the Balanced plan at light load",
                Find(GHPerformanceDiagnosis.Diagnose(f), GHPerformanceDiagnosis.CodeCpuClockCapped).Evidence);
        }

        [Fact]
        public void CpuClockCapped_NoBefore_UsesClassifyThrottle()
        {
            GHDiagnosisFacts f = HealthyFacts();
            f.CpuPerformancePctBefore = float.NaN;
            f.CpuPerformancePctAfter = 89.9f;
            AssertFires(f, GHPerformanceDiagnosis.CodeCpuClockCapped, GHFindingSeverity.Suspect);

            f.CpuPerformancePctAfter = 55f;
            AssertFires(f, GHPerformanceDiagnosis.CodeCpuClockCapped, GHFindingSeverity.Suspect);

            f.CpuPerformancePctAfter = 90f;
            AssertSilent(f, GHPerformanceDiagnosis.CodeCpuClockCapped);
        }

        /* ------------------------------------------------------------------ Power */

        [Fact]
        public void LowPowerMode_OnlyWhenPowerStateKnown()
        {
            GHDiagnosisFacts f = HealthyFacts();
            f.IsLowPower = true;
            AssertFires(f, GHPerformanceDiagnosis.CodeLowPowerMode, GHFindingSeverity.Likely);

            f.PowerStateKnown = false;
            AssertSilent(f, GHPerformanceDiagnosis.CodeLowPowerMode);
        }

        [Theory]
        [InlineData("settings.powerMode", "Best power efficiency", true)]
        [InlineData("settings.powerPlan", "Power saver", true)]
        [InlineData("settings.powerMode", "Best performance", false)]
        [InlineData("settings.powerPlan", "High performance", false)]
        public void PowerEfficiencyMode_FingerprintValues(string key, string value, bool fires)
        {
            GHDiagnosisFacts f = HealthyFacts();
            f.Fingerprint[key] = value;

            if (fires)
                AssertFires(f, GHPerformanceDiagnosis.CodePowerEfficiencyMode, GHFindingSeverity.Suspect);
            else
                AssertSilent(f, GHPerformanceDiagnosis.CodePowerEfficiencyMode);
        }

        [Fact]
        public void PowerEfficiencyMode_NoFingerprint_Silent()
        {
            GHDiagnosisFacts f = HealthyFacts();
            f.Fingerprint = null;
            AssertSilent(f, GHPerformanceDiagnosis.CodePowerEfficiencyMode);
        }

        [Fact]
        public void OnBattery_InfoThenSuspectWithEfficiencyMode()
        {
            GHDiagnosisFacts f = HealthyFacts();
            f.IsCharging = false;
            AssertFires(f, GHPerformanceDiagnosis.CodeOnBattery, GHFindingSeverity.Info);

            f.Fingerprint["settings.powerMode"] = "Best power efficiency";
            AssertFires(f, GHPerformanceDiagnosis.CodeOnBattery, GHFindingSeverity.Suspect);

            f.PowerStateKnown = false;
            AssertSilent(f, GHPerformanceDiagnosis.CodeOnBattery);
        }

        /* ------------------------------------------------------------------ Background */

        [Fact]
        public void BackgroundVerdicts_MapToFindings()
        {
            GHDiagnosisFacts f = HealthyFacts();
            f.BackgroundVerdict = GHBackgroundVerdict.Busy;
            f.BackgroundReason = "background load: other CPU P90 30 %";
            GHDiagnosisResult r = GHPerformanceDiagnosis.Diagnose(f);
            Assert.Equal(GHFindingSeverity.Likely, Find(r, GHPerformanceDiagnosis.CodeBackgroundBusy).Severity);
            Assert.Equal(f.BackgroundReason, Find(r, GHPerformanceDiagnosis.CodeBackgroundBusy).Evidence);
            Assert.Null(Find(r, GHPerformanceDiagnosis.CodeBackgroundElevated));

            f.BackgroundVerdict = GHBackgroundVerdict.Elevated;
            AssertFires(f, GHPerformanceDiagnosis.CodeBackgroundElevated, GHFindingSeverity.Suspect);
            AssertSilent(f, GHPerformanceDiagnosis.CodeBackgroundBusy);

            f.BackgroundVerdict = GHBackgroundVerdict.Quiet;
            AssertSilent(f, GHPerformanceDiagnosis.CodeBackgroundElevated);
            f.BackgroundVerdict = GHBackgroundVerdict.Unknown;
            AssertSilent(f, GHPerformanceDiagnosis.CodeBackgroundElevated);
            AssertSilent(f, GHPerformanceDiagnosis.CodeBackgroundBusy);
        }

        [Fact]
        public void KnownActivity_ThresholdAndBusySeverity()
        {
            GHDiagnosisFacts f = HealthyFacts();
            f.Processes = new List<GHProcessLoad> { new GHProcessLoad("MsMpEng", 2f, float.NaN) };
            AssertFires(f, "BG_ANTIVIRUS", GHFindingSeverity.Info);

            f.BackgroundVerdict = GHBackgroundVerdict.Busy;
            AssertFires(f, "BG_ANTIVIRUS", GHFindingSeverity.Likely);

            f.Processes = new List<GHProcessLoad> { new GHProcessLoad("MsMpEng", 1.9f, float.NaN) };
            AssertSilent(f, "BG_ANTIVIRUS");
        }

        [Fact]
        public void KnownActivity_SuspectOnlyWithMagnitude()
        {
            GHDiagnosisFacts f = HealthyFacts();
            f.Processes = new List<GHProcessLoad> { new GHProcessLoad("MsMpEng", 5f, float.NaN) };
            AssertFires(f, "BG_ANTIVIRUS", GHFindingSeverity.Info);

            f.BackgroundVerdict = GHBackgroundVerdict.Elevated;
            AssertFires(f, "BG_ANTIVIRUS", GHFindingSeverity.Suspect);

            f.BackgroundVerdict = GHBackgroundVerdict.Quiet;
            f.Processes = new List<GHProcessLoad> { new GHProcessLoad("MsMpEng", 9.9f, float.NaN) };
            AssertFires(f, "BG_ANTIVIRUS", GHFindingSeverity.Info);

            f.Processes = new List<GHProcessLoad> { new GHProcessLoad("MsMpEng", 10f, float.NaN) };
            AssertFires(f, "BG_ANTIVIRUS", GHFindingSeverity.Suspect);

            f.BackgroundVerdict = GHBackgroundVerdict.Busy;
            AssertFires(f, "BG_ANTIVIRUS", GHFindingSeverity.Likely);
        }

        [Fact]
        public void KnownActivity_WindowsUpdatePendingReboot_AdviceSaysRestart()
        {
            GHDiagnosisFacts f = HealthyFacts();
            f.Processes = new List<GHProcessLoad> { new GHProcessLoad("TiWorker", 12f, float.NaN) };
            GHDiagnosisFinding x = Find(GHPerformanceDiagnosis.Diagnose(f), "BG_WINDOWS_UPDATE");
            Assert.NotNull(x);
            Assert.DoesNotContain("restart", x.Advice);

            f.Fingerprint["os.pendingReboot"] = "true";
            x = Find(GHPerformanceDiagnosis.Diagnose(f), "BG_WINDOWS_UPDATE");
            Assert.Contains("restart", x.Advice);
        }

        [Theory]
        [InlineData("windows-update", "BG_WINDOWS_UPDATE")]
        [InlineData("antivirus", "BG_ANTIVIRUS")]
        [InlineData("indexer", "BG_INDEXER")]
        [InlineData("sync", "BG_SYNC")]
        [InlineData("build-tools", "BG_BUILD_TOOLS")]
        [InlineData("wsl-vm", "BG_WSL_VM")]
        [InlineData("telemetry", "BG_TELEMETRY")]
        public void ActivityCode_UpperCaseWithUnderscores(string category, string code)
        {
            Assert.Equal(code, GHPerformanceDiagnosis.ActivityCode(category));
        }

        [Theory]
        [InlineData("chrome", 10f, true)]
        [InlineData("chrome", 9.9f, false)]
        [InlineData("powershell", 50f, false)]
        [InlineData("MsMpEng", 50f, false)]
        public void OtherProcess_OnlyOtherCategoryAtThreshold(string name, float cpu, bool fires)
        {
            GHDiagnosisFacts f = HealthyFacts();
            f.Processes = new List<GHProcessLoad> { new GHProcessLoad(name, cpu, float.NaN) };

            if (fires)
                AssertFires(f, GHPerformanceDiagnosis.CodeOtherProcess, GHFindingSeverity.Suspect);
            else
                AssertSilent(f, GHPerformanceDiagnosis.CodeOtherProcess);
        }

        [Fact]
        public void OtherProcess_EvidenceNamesTopProcess()
        {
            GHDiagnosisFacts f = HealthyFacts();
            f.Processes = new List<GHProcessLoad>
            {
                new GHProcessLoad("slack", 11f, float.NaN),
                new GHProcessLoad("chrome", 23f, float.NaN)
            };
            GHDiagnosisFinding x = Find(GHPerformanceDiagnosis.Diagnose(f), GHPerformanceDiagnosis.CodeOtherProcess);
            Assert.StartsWith("chrome ", x.Evidence);
        }

        [Theory]
        [InlineData(float.NaN, null)]
        [InlineData(9.9f, null)]
        [InlineData(10f, GHFindingSeverity.Suspect)]
        [InlineData(29.9f, GHFindingSeverity.Suspect)]
        [InlineData(30f, GHFindingSeverity.Likely)]
        public void OtherGpuLoad_Boundaries(float otherGpu, GHFindingSeverity? expected)
        {
            GHDiagnosisFacts f = HealthyFacts();
            f.OtherGpuPct = otherGpu;

            if (expected.HasValue)
                AssertFires(f, GHPerformanceDiagnosis.CodeOtherGpuLoad, expected.Value);
            else
                AssertSilent(f, GHPerformanceDiagnosis.CodeOtherGpuLoad);
        }

        /* ------------------------------------------------------------------ Memory */

        [Theory]
        [InlineData(false, 10f, 199.9f, 0, null)]
        [InlineData(true, 45f, 5f, 0, GHFindingSeverity.Likely)]
        [InlineData(false, 9.9f, 5f, 0, GHFindingSeverity.Suspect)]
        [InlineData(false, 5f, 5f, 0, GHFindingSeverity.Suspect)]
        [InlineData(false, 4.9f, 5f, 0, GHFindingSeverity.Likely)]
        [InlineData(false, 45f, 200f, 0, GHFindingSeverity.Suspect)]
        [InlineData(false, 45f, 999f, 0, GHFindingSeverity.Suspect)]
        [InlineData(false, 45f, 1000f, 0, GHFindingSeverity.Likely)]
        [InlineData(false, 45f, 5f, 1, GHFindingSeverity.Suspect)]
        [InlineData(false, float.NaN, float.NaN, 0, null)]
        public void LowMemory_Boundaries(bool low, float availablePct, float hardFaults, int pressure,
                                         GHFindingSeverity? expected)
        {
            GHDiagnosisFacts f = HealthyFacts();
            f.LowMemory = low;
            f.AvailableMemoryMinPct = availablePct;
            f.HardFaultsP90PerSec = hardFaults;
            f.MemoryPressureEvents = pressure;

            if (expected.HasValue)
                AssertFires(f, GHPerformanceDiagnosis.CodeLowMemory, expected.Value);
            else
                AssertSilent(f, GHPerformanceDiagnosis.CodeLowMemory);
        }

        /* ------------------------------------------------------------------ GPU path */

        [Fact]
        public void CpuRendering_SeverityByHealthAndPaintShare()
        {
            GHDiagnosisFacts f = HealthyFacts();
            AssertSilent(f, GHPerformanceDiagnosis.CodeCpuRendering);

            f.MainCanvasGlRequested = false;
            AssertFires(f, GHPerformanceDiagnosis.CodeCpuRendering, GHFindingSeverity.Info);

            Causes(f, GHHitchCause.PaintCpu, 29.0, GHHitchCause.Unattributed, 71.0);
            AssertFires(f, GHPerformanceDiagnosis.CodeCpuRendering, GHFindingSeverity.Info);

            Causes(f, GHHitchCause.PaintCpu, 30.0, GHHitchCause.Unattributed, 70.0);
            AssertFires(f, GHPerformanceDiagnosis.CodeCpuRendering, GHFindingSeverity.Likely);

            GHDiagnosisFacts d = DegradedFacts();
            d.MainCanvasGlRequested = false;
            AssertFires(d, GHPerformanceDiagnosis.CodeCpuRendering, GHFindingSeverity.Likely);
        }

        [Fact]
        public void CpuRendering_InconclusiveIsInfo()
        {
            GHDiagnosisFacts f = DegradedFacts();
            f.MainCanvasGlRequested = false;
            f.PlatformRenderLoopOn = false;
            Assert.Equal(GHDiagnosisHealth.Inconclusive, GHPerformanceDiagnosis.Diagnose(f).Health);
            AssertFires(f, GHPerformanceDiagnosis.CodeCpuRendering, GHFindingSeverity.Info);

            Causes(f, GHHitchCause.PaintCpu, 30.0, GHHitchCause.Unattributed, 70.0);
            AssertFires(f, GHPerformanceDiagnosis.CodeCpuRendering, GHFindingSeverity.Likely);
        }

        [Fact]
        public void CpuRendering_HasNoScore()
        {
            GHDiagnosisFacts f = DegradedFacts();
            f.MainCanvasGlRequested = false;
            Causes(f, GHHitchCause.PaintCpu, 60.0, GHHitchCause.Unattributed, 40.0);

            GHDiagnosisFinding x = Find(GHPerformanceDiagnosis.Diagnose(f), GHPerformanceDiagnosis.CodeCpuRendering);

            Assert.Equal(GHFindingSeverity.Likely, x.Severity);
            Assert.True(double.IsNaN(x.Score));
        }

        [Fact]
        public void GpuContextMissing_OnlyWhenRequestedAndKnownDead()
        {
            GHDiagnosisFacts f = HealthyFacts();
            f.MainCanvasGpuContextLive = false;
            AssertFires(f, GHPerformanceDiagnosis.CodeGpuContextMissing, GHFindingSeverity.Likely);

            f.MainCanvasGpuContextLive = null;
            AssertSilent(f, GHPerformanceDiagnosis.CodeGpuContextMissing);

            f.MainCanvasGpuContextLive = true;
            AssertSilent(f, GHPerformanceDiagnosis.CodeGpuContextMissing);

            f.MainCanvasGpuContextLive = false;
            f.MainCanvasGlRequested = false;
            AssertSilent(f, GHPerformanceDiagnosis.CodeGpuContextMissing);
        }

        [Fact]
        public void SoftwareAdapter_OnlyWhenTrue()
        {
            GHDiagnosisFacts f = HealthyFacts();
            f.RenderAdapterIsSoftware = true;
            f.RenderAdapterName = "Microsoft Basic Render Driver";
            AssertFires(f, GHPerformanceDiagnosis.CodeSoftwareAdapter, GHFindingSeverity.Likely);

            f.RenderAdapterIsSoftware = null;
            AssertSilent(f, GHPerformanceDiagnosis.CodeSoftwareAdapter);
        }

        [Fact]
        public void WrongGpu_IntegratedWithDiscretePresent()
        {
            GHDiagnosisFacts f = HealthyFacts();
            f.RenderAdapterName = "Intel(R) UHD Graphics 630";
            f.RenderAdapterIsIntegrated = true;
            AssertFires(f, GHPerformanceDiagnosis.CodeWrongGpu, GHFindingSeverity.Likely);

            f.DiscreteAdapterPresent = false;
            AssertSilent(f, GHPerformanceDiagnosis.CodeWrongGpu);

            f.DiscreteAdapterPresent = null;
            AssertSilent(f, GHPerformanceDiagnosis.CodeWrongGpu);

            f.DiscreteAdapterPresent = true;
            f.RenderAdapterIsIntegrated = null;
            AssertSilent(f, GHPerformanceDiagnosis.CodeWrongGpu);
        }

        [Theory]
        [InlineData("Auto", 2, true)]
        [InlineData("Not set", 2, true)]
        [InlineData("Dedicated", 2, false)]
        [InlineData(null, 2, false)]
        [InlineData("Auto", 1, false)]
        public void WrongGpuPossible_AdapterUnknown(string preference, int adapterCount, bool fires)
        {
            GHDiagnosisFacts f = HealthyFacts();
            f.RenderAdapterName = null;
            f.RenderAdapterIsIntegrated = null;
            f.RenderAdapterIsSoftware = null;
            f.GpuPreference = preference;
            f.Adapters = new List<string>();
            for (int i = 0; i < adapterCount; i++)
                f.Adapters.Add("adapter " + i);

            if (fires)
                AssertFires(f, GHPerformanceDiagnosis.CodeWrongGpuPossible, GHFindingSeverity.Suspect);
            else
                AssertSilent(f, GHPerformanceDiagnosis.CodeWrongGpuPossible);
        }

        [Fact]
        public void WrongGpuPossible_SilentWhenAdapterIdentified()
        {
            GHDiagnosisFacts f = HealthyFacts();
            f.GpuPreference = "Auto";
            AssertSilent(f, GHPerformanceDiagnosis.CodeWrongGpuPossible);
        }

        [Theory]
        [InlineData(30.0, 70.0, GHFindingSeverity.Suspect)]
        [InlineData(29.0, 71.0, null)]
        [InlineData(20.0, 9.9, null)]  /* 29.9 gate units over 30 s are below the share gate */
        [InlineData(20.0, 10.0, GHFindingSeverity.Likely)]
        public void GpuBound_ShareAndGate(double gpuMs, double otherMs, GHFindingSeverity? expected)
        {
            GHDiagnosisFacts f = HealthyFacts();
            Causes(f, GHHitchCause.Gpu, gpuMs, GHHitchCause.Unattributed, otherMs);

            if (expected.HasValue)
                AssertFires(f, GHPerformanceDiagnosis.CodeGpuBound, expected.Value);
            else
                AssertSilent(f, GHPerformanceDiagnosis.CodeGpuBound);
        }

        [Fact]
        public void Shares_UnknownWindowOrCauses_Silent()
        {
            GHDiagnosisFacts f = HealthyFacts();
            Causes(f, GHHitchCause.Gpu, 100.0);
            f.WindowSeconds = float.NaN;
            AssertSilent(f, GHPerformanceDiagnosis.CodeGpuBound);

            f.WindowSeconds = 30f;
            f.CauseMs = null;
            AssertSilent(f, GHPerformanceDiagnosis.CodeGpuBound);
        }

        /* A healthy Windows laptop run: 4 hitches, 1.9 ms/s, mostly PaintCpu */
        [Fact]
        public void Shares_HealthyRunFewHitches_NameNoCause()
        {
            GHDiagnosisFacts f = HealthyFacts();
            f.HitchRatioMsPerSec = 1.944f;
            f.CauseMs = new double[GHSmoothnessMetrics.CauseCount];
            f.CauseMs[(int)GHHitchCause.PaintCpu] = 41.642;
            f.CauseMs[(int)GHHitchCause.UiThreadLate] = 16.652;

            AssertSilent(f, GHPerformanceDiagnosis.CodePaintHeavy);
            AssertSilent(f, GHPerformanceDiagnosis.CodeUiThreadBusy);
        }

        [Fact]
        public void OtherGpuLoad_ExcludesCompositor()
        {
            GHDiagnosisFacts f = HealthyFacts();
            f.OtherGpuPct = 16.296f;
            f.Processes = new List<GHProcessLoad>
            {
                new GHProcessLoad("dwm", 0.452f, 16.294f),
                new GHProcessLoad("chrome", 0.052f, 0.001f)
            };
            AssertSilent(f, GHPerformanceDiagnosis.CodeOtherGpuLoad);

            f.Processes[0] = new GHProcessLoad("dwm.exe", 0.452f, 4f);   /* 12.3 % left for other apps */
            AssertFires(f, GHPerformanceDiagnosis.CodeOtherGpuLoad, GHFindingSeverity.Suspect);
        }

        [Fact]
        public void Report_OtherGpuShownWithoutCompositor()
        {
            GHDiagnosisFacts f = HealthyFacts();
            f.OtherGpuPct = 17.337f;
            f.Processes = new List<GHProcessLoad> { new GHProcessLoad("dwm", 0.367f, 17.182f) };
            string report = GHPerformanceDiagnosis.BuildReport(f, GHPerformanceDiagnosis.Diagnose(f), Now);

            Assert.Contains("other GPU 0 %, compositor 17 %", report);
            Assert.DoesNotContain("other GPU 17 %", report);

            f.Processes = new List<GHProcessLoad> { new GHProcessLoad("vlc", 1f, 17f) };
            report = GHPerformanceDiagnosis.BuildReport(f, GHPerformanceDiagnosis.Diagnose(f), Now);
            Assert.Contains("other GPU 17 %", report);
            Assert.DoesNotContain("compositor 17 %", report);
        }

        /* ------------------------------------------------------------------ Display */

        [Theory]
        [InlineData(30.0, 70.0, GHPerformanceDiagnosis.PresentSourceEstimated, GHFindingSeverity.Info)]
        [InlineData(30.0, 70.0, null, GHFindingSeverity.Info)]
        [InlineData(30.0, 70.0, GHPerformanceDiagnosis.PresentSourceMeasured, GHFindingSeverity.Suspect)]
        [InlineData(60.0, 40.0, GHPerformanceDiagnosis.PresentSourceMeasured, GHFindingSeverity.Suspect)]
        [InlineData(29.0, 71.0, GHPerformanceDiagnosis.PresentSourceEstimated, null)]
        [InlineData(29.0, 71.0, GHPerformanceDiagnosis.PresentSourceMeasured, null)]
        public void Compositor_Share(double compositorMs, double otherMs, string source, GHFindingSeverity? expected)
        {
            GHDiagnosisFacts f = HealthyFacts();
            f.PresentSource = source;
            Causes(f, GHHitchCause.Compositor, compositorMs, GHHitchCause.Unattributed, otherMs);

            if (!expected.HasValue)
            {
                AssertSilent(f, GHPerformanceDiagnosis.CodeCompositor);
                return;
            }
            AssertFires(f, GHPerformanceDiagnosis.CodeCompositor, expected.Value);
            GHDiagnosisFinding x = Find(GHPerformanceDiagnosis.Diagnose(f), GHPerformanceDiagnosis.CodeCompositor);
            if (expected.Value == GHFindingSeverity.Info)
            {
                Assert.Equal("Frames may be dropped after the app (inferred)", x.Title);
                Assert.Contains("; display times are estimated from vsync, not measured", x.Evidence);
                Assert.Contains("PresentMon", x.Advice);
                Assert.True(double.IsNaN(x.Score));
            }
            else
            {
                Assert.Equal(compositorMs / (compositorMs + otherMs), x.Score, 6);
            }
        }

        [Fact]
        public void Conclusion_InferredCompositor_IsUnclear()
        {
            GHDiagnosisFacts f = DegradedFacts();
            Causes(f, GHHitchCause.Compositor, 60.0, GHHitchCause.Unattributed, 40.0);

            GHDiagnosisResult r = GHPerformanceDiagnosis.Diagnose(f);

            Assert.Null(r.Primary);
            Assert.Equal(GHCauseLocation.Unclear, r.Location);
            Assert.Equal(GHPerformanceDiagnosis.InferredCompositorConclusion, r.Conclusion);
            Assert.Contains("\nLocation: unclear\n", Report(f));

            f.PresentSource = GHPerformanceDiagnosis.PresentSourceMeasured;
            r = GHPerformanceDiagnosis.Diagnose(f);
            Assert.Equal(GHPerformanceDiagnosis.CodeCompositor, r.Primary.Code);
            Assert.Equal(GHCauseLocation.Configuration, r.Location);
        }

        [Fact]
        public void Conclusion_NoCompositor_IsStillElimination()
        {
            GHDiagnosisFacts f = DegradedFacts();
            Causes(f, GHHitchCause.Compositor, 29.0, GHHitchCause.Unattributed, 71.0);

            GHDiagnosisResult r = GHPerformanceDiagnosis.Diagnose(f);

            Assert.Null(r.Primary);
            Assert.Equal(GHCauseLocation.InsideGame, r.Location);
            Assert.Equal(GHPerformanceDiagnosis.EliminationConclusion, r.Conclusion);
        }

        [Theory]
        [InlineData(90f, 100f, true)]
        [InlineData(90.1f, 100f, false)]
        [InlineData(60f, float.NaN, false)]
        [InlineData(float.NaN, 144f, false)]
        public void RefreshBelowMax_Boundary(float measured, float max, bool fires)
        {
            GHDiagnosisFacts f = HealthyFacts();
            f.MeasuredRefreshHz = measured;
            f.DisplayMaxRefreshHz = max;
            f.TargetFps = float.IsNaN(measured) ? 60f : measured;

            if (fires)
                AssertFires(f, GHPerformanceDiagnosis.CodeRefreshBelowMax, GHFindingSeverity.Suspect);
            else
                AssertSilent(f, GHPerformanceDiagnosis.CodeRefreshBelowMax);
        }

        /* A 144 Hz laptop panel at 120 Hz: only relevant when the Map FPS target reaches it */
        [Fact]
        public void RefreshBelowMax_OnlyWhenDisplayCapsFrameRate()
        {
            GHDiagnosisFacts f = HealthyFacts();
            f.MeasuredRefreshHz = 120.039f;
            f.DisplayMaxRefreshHz = 144f;

            f.TargetFps = 60f;
            AssertSilent(f, GHPerformanceDiagnosis.CodeRefreshBelowMax);
            AssertFires(f, GHPerformanceDiagnosis.CodeMapFpsCap, GHFindingSeverity.Info);

            f.TargetFps = 120f;
            f.DisplayedFps = 119f;
            AssertFires(f, GHPerformanceDiagnosis.CodeRefreshBelowMax, GHFindingSeverity.Suspect);
            AssertSilent(f, GHPerformanceDiagnosis.CodeMapFpsCap);

            f.TargetFps = float.NaN;
            AssertSilent(f, GHPerformanceDiagnosis.CodeRefreshBelowMax);
        }

        [Fact]
        public void PacingMismatch_FollowsFlag()
        {
            GHDiagnosisFacts f = HealthyFacts();
            AssertSilent(f, GHPerformanceDiagnosis.CodePacingMismatch);

            f.AssumedRefreshMismatch = true;
            AssertFires(f, GHPerformanceDiagnosis.CodePacingMismatch, GHFindingSeverity.Suspect);
        }

        /* ------------------------------------------------------------------ Settings */

        [Theory]
        [InlineData(53.9f, 60f, true)]
        [InlineData(54f, 60f, false)]
        [InlineData(30f, float.NaN, false)]
        public void MapFpsCap_Boundary(float target, float measured, bool fires)
        {
            GHDiagnosisFacts f = HealthyFacts();
            f.TargetFps = target;
            f.DisplayedFps = target;
            f.MeasuredRefreshHz = measured;

            if (fires)
                AssertFires(f, GHPerformanceDiagnosis.CodeMapFpsCap, GHFindingSeverity.Info);
            else
                AssertSilent(f, GHPerformanceDiagnosis.CodeMapFpsCap);
        }

        [Fact]
        public void SettingsNonDefault_OnlyWhenListed()
        {
            GHDiagnosisFacts f = HealthyFacts();
            AssertSilent(f, GHPerformanceDiagnosis.CodeSettingsNonDefault);

            f.NonDefaultPerformanceSettings = null;
            AssertSilent(f, GHPerformanceDiagnosis.CodeSettingsNonDefault);

            f.NonDefaultPerformanceSettings = new List<string> { "tile batching off" };
            AssertFires(f, GHPerformanceDiagnosis.CodeSettingsNonDefault, GHFindingSeverity.Suspect);
            Assert.Contains("tile batching off",
                Find(GHPerformanceDiagnosis.Diagnose(f), GHPerformanceDiagnosis.CodeSettingsNonDefault).Evidence);
        }

        /* ------------------------------------------------------------------ Inside the game */

        [Theory]
        [InlineData(20.0, 80.0, 4f, true)]
        [InlineData(19.0, 81.0, 4f, false)]
        [InlineData(0.0, 100.0, 50f, true)]
        [InlineData(0.0, 100.0, 49.9f, false)]
        [InlineData(0.0, 100.0, float.NaN, false)]
        public void GcPressure_ShareOrAllocationRate(double gcMs, double otherMs, float allocation, bool fires)
        {
            GHDiagnosisFacts f = HealthyFacts();
            Causes(f, GHHitchCause.UiThreadLateGc, gcMs, GHHitchCause.Unattributed, otherMs);
            f.AllocationRateMBPerSec = allocation;

            if (fires)
                AssertFires(f, GHPerformanceDiagnosis.CodeGcPressure, GHFindingSeverity.Suspect);
            else
                AssertSilent(f, GHPerformanceDiagnosis.CodeGcPressure);
        }

        [Theory]
        [InlineData(15.0, 15.0, 70.0, true)]
        [InlineData(14.0, 15.0, 71.0, false)]
        [InlineData(30.0, 0.0, 70.0, true)]
        public void UiThreadBusy_CombinedShare(double lateMs, double requestsMs, double otherMs, bool fires)
        {
            GHDiagnosisFacts f = HealthyFacts();
            Causes(f, GHHitchCause.UiThreadLate, lateMs, GHHitchCause.UiThreadRequests, requestsMs,
                GHHitchCause.Unattributed, otherMs);

            if (fires)
                AssertFires(f, GHPerformanceDiagnosis.CodeUiThreadBusy, GHFindingSeverity.Suspect);
            else
                AssertSilent(f, GHPerformanceDiagnosis.CodeUiThreadBusy);
        }

        [Fact]
        public void PaintHeavy_OnlyWithGpuOn()
        {
            GHDiagnosisFacts f = HealthyFacts();
            Causes(f, GHHitchCause.PaintCpu, 30.0, GHHitchCause.Unattributed, 70.0);
            AssertFires(f, GHPerformanceDiagnosis.CodePaintHeavy, GHFindingSeverity.Suspect);

            Causes(f, GHHitchCause.PaintCpu, 29.0, GHHitchCause.Unattributed, 71.0);
            AssertSilent(f, GHPerformanceDiagnosis.CodePaintHeavy);

            Causes(f, GHHitchCause.PaintCpu, 30.0, GHHitchCause.Unattributed, 70.0);
            f.MainCanvasGpuContextLive = false;
            AssertSilent(f, GHPerformanceDiagnosis.CodePaintHeavy);

            f.MainCanvasGpuContextLive = true;
            f.MainCanvasGlRequested = false;
            AssertSilent(f, GHPerformanceDiagnosis.CodePaintHeavy);
        }

        [Theory]
        [InlineData(GHHitchCause.UiThreadLateGc, GHPerformanceDiagnosis.CodeGcPressure)]
        [InlineData(GHHitchCause.UiThreadLate, GHPerformanceDiagnosis.CodeUiThreadBusy)]
        [InlineData(GHHitchCause.PaintCpu, GHPerformanceDiagnosis.CodePaintHeavy)]
        [InlineData(GHHitchCause.Gpu, GHPerformanceDiagnosis.CodeGpuBound)]
        public void ShareFindings_LikelyAtHalf(GHHitchCause cause, string code)
        {
            GHDiagnosisFacts f = HealthyFacts();
            Causes(f, cause, 50.0, GHHitchCause.Unattributed, 50.0);
            GHDiagnosisFinding x = Find(GHPerformanceDiagnosis.Diagnose(f), code);
            Assert.Equal(GHFindingSeverity.Likely, x.Severity);
            Assert.Equal(0.5, x.Score, 6);

            Causes(f, cause, 49.0, GHHitchCause.Unattributed, 51.0);
            x = Find(GHPerformanceDiagnosis.Diagnose(f), code);
            Assert.Equal(GHFindingSeverity.Suspect, x.Severity);
            Assert.Equal(0.49, x.Score, 6);
        }

        [Fact]
        public void AddedEvidence_NeverDemotes()
        {
            GHDiagnosisFacts f = DegradedFacts();
            Causes(f, GHHitchCause.UiThreadLateGc, 60.0, GHHitchCause.Unattributed, 40.0);
            GHDiagnosisResult r = GHPerformanceDiagnosis.Diagnose(f);
            Assert.Equal(GHFindingSeverity.Likely, r.Primary.Severity);
            Assert.Equal(GHPerformanceDiagnosis.CodeGcPressure, r.Primary.Code);

            /* An allocation rate over the bound adds evidence, but keeps the share's severity and score */
            f.AllocationRateMBPerSec = 80f;
            r = GHPerformanceDiagnosis.Diagnose(f);
            Assert.Equal(GHPerformanceDiagnosis.CodeGcPressure, r.Primary.Code);
            Assert.Equal(GHFindingSeverity.Likely, r.Primary.Severity);
            Assert.Equal(0.6, r.Primary.Score, 6);

            Causes(f, GHHitchCause.UiThreadLateGc, 30.0, GHHitchCause.Unattributed, 70.0);
            GHDiagnosisFinding x = Find(GHPerformanceDiagnosis.Diagnose(f), GHPerformanceDiagnosis.CodeGcPressure);
            Assert.Equal(GHFindingSeverity.Suspect, x.Severity);
            Assert.Equal(0.3, x.Score, 6);

            /* By allocation rate alone: Suspect without a score */
            Causes(f, GHHitchCause.Unattributed, 100.0);
            x = Find(GHPerformanceDiagnosis.Diagnose(f), GHPerformanceDiagnosis.CodeGcPressure);
            Assert.Equal(GHFindingSeverity.Suspect, x.Severity);
            Assert.True(double.IsNaN(x.Score));
        }

        /* ------------------------------------------------------------------ Instrument */

        [Fact]
        public void DebugOverhead_InfoOrSuspectForDebugger()
        {
            GHDiagnosisFacts f = HealthyFacts();
            AssertSilent(f, GHPerformanceDiagnosis.CodeDebugOverhead);

            f.IsDebugBuild = true;
            AssertFires(f, GHPerformanceDiagnosis.CodeDebugOverhead, GHFindingSeverity.Info);

            f.IsDebugBuild = false;
            f.VerboseLoggingOn = true;
            AssertFires(f, GHPerformanceDiagnosis.CodeDebugOverhead, GHFindingSeverity.Info);

            f.DebuggerAttached = true;
            AssertFires(f, GHPerformanceDiagnosis.CodeDebugOverhead, GHFindingSeverity.Suspect);
        }

        [Fact]
        public void PlayerInput_AnyContentEvent()
        {
            GHDiagnosisFacts f = HealthyFacts();
            AssertSilent(f, GHPerformanceDiagnosis.CodePlayerInput);

            f.ContentEventCount = 1;
            AssertFires(f, GHPerformanceDiagnosis.CodePlayerInput, GHFindingSeverity.Info);
        }

        [Fact]
        public void PlayerInput_SuspectWhenEventGapsHoldHalfTheHitches()
        {
            int mapUpdate = 9;   /* bit position of GHContentEvent.MapUpdate */
            GHDiagnosisFacts f = DegradedFacts();
            f.HitchCount = 4;
            f.QuietGapCount = 1780;
            f.QuietHitchCount = 2;
            f.ContentEventCount = 20;
            f.EventGapCounts[mapUpdate] = 20;
            f.EventHitchCounts[mapUpdate] = 2;

            GHDiagnosisResult r = GHPerformanceDiagnosis.Diagnose(f);
            GHDiagnosisFinding x = Find(r, GHPerformanceDiagnosis.CodePlayerInput);

            Assert.Equal(GHFindingSeverity.Suspect, x.Severity);
            Assert.Contains("MapUpdate 2 of 20", x.Evidence);
            Assert.Contains("0.1 % of quiet gaps", x.Evidence);
            Assert.Contains("10.0 % of gaps with events", x.Evidence);
            Assert.Null(r.Primary);

            f.QuietHitchCount = 3;
            AssertFires(f, GHPerformanceDiagnosis.CodePlayerInput, GHFindingSeverity.Info);

            f.HitchCount = 2;
            f.QuietHitchCount = 0;
            AssertFires(f, GHPerformanceDiagnosis.CodePlayerInput, GHFindingSeverity.Info);
        }

        [Fact]
        public void PlayerInput_UnknownQuietCount_StaysInfo()
        {
            GHDiagnosisFacts f = DegradedFacts();
            f.HitchCount = 10;
            f.QuietHitchCount = -1;
            f.ContentEventCount = 20;

            AssertFires(f, GHPerformanceDiagnosis.CodePlayerInput, GHFindingSeverity.Info);
            Assert.DoesNotContain("\n  Content: ", Report(f));
        }

        [Fact]
        public void SamplerOff_FollowsFlagAndChecksRowIsNotAvailable()
        {
            GHDiagnosisFacts f = HealthyFacts();
            f.BackgroundSamplerDisabled = true;
            AssertFires(f, GHPerformanceDiagnosis.CodeSamplerOff, GHFindingSeverity.Info);

            GHDiagnosisResult r = GHPerformanceDiagnosis.Diagnose(f);
            GHCheckRow row = r.Checks[(int)GHFindingArea.Background];
            Assert.Equal(GHFindingArea.Background, row.Area);
            Assert.Equal(GHPerformanceDiagnosis.StatusNotAvailable, row.Status);
        }

        /* ------------------------------------------------------------------ Location and primary */

        [Fact]
        public void Location_External_FromBackground()
        {
            GHDiagnosisFacts f = DegradedFacts();
            f.BackgroundVerdict = GHBackgroundVerdict.Busy;

            GHDiagnosisResult r = GHPerformanceDiagnosis.Diagnose(f);

            Assert.Equal(GHCauseLocation.External, r.Location);
            Assert.Equal(GHPerformanceDiagnosis.CodeBackgroundBusy, r.Primary.Code);
        }

        [Fact]
        public void Location_Configuration_FromWrongGpu()
        {
            GHDiagnosisFacts f = DegradedFacts();
            f.RenderAdapterIsIntegrated = true;

            GHDiagnosisResult r = GHPerformanceDiagnosis.Diagnose(f);

            Assert.Equal(GHCauseLocation.Configuration, r.Location);
            Assert.Equal(GHPerformanceDiagnosis.CodeWrongGpu, r.Primary.Code);
        }

        [Fact]
        public void Location_InsideGame_FromPaintHeavy()
        {
            GHDiagnosisFacts f = DegradedFacts();
            Causes(f, GHHitchCause.PaintCpu, 60.0, GHHitchCause.Unattributed, 40.0);

            GHDiagnosisResult r = GHPerformanceDiagnosis.Diagnose(f);

            Assert.Equal(GHCauseLocation.InsideGame, r.Location);
            Assert.Equal(GHPerformanceDiagnosis.CodePaintHeavy, r.Primary.Code);
        }

        [Fact]
        public void Location_InsideGameByElimination()
        {
            GHDiagnosisFacts f = DegradedFacts();

            GHDiagnosisResult r = GHPerformanceDiagnosis.Diagnose(f);

            Assert.Equal(GHDiagnosisHealth.Degraded, r.Health);
            Assert.Null(r.Primary);
            Assert.Equal(GHCauseLocation.InsideGame, r.Location);
            Assert.Equal(GHPerformanceDiagnosis.EliminationConclusion, r.Conclusion);
            string report = GHPerformanceDiagnosis.BuildReport(f, r, Now);
            Assert.Contains("  No external or configuration cause found:", report);
        }

        [Fact]
        public void Location_InstrumentAndInfoFindingsNeverPrimary()
        {
            GHDiagnosisFacts f = DegradedFacts();
            f.VerboseLoggingOn = true;
            f.IsCharging = false;
            f.HitchCount = 4;
            f.QuietHitchCount = 1;
            f.ContentEventCount = 30;

            GHDiagnosisResult r = GHPerformanceDiagnosis.Diagnose(f);

            Assert.Equal(GHDiagnosisHealth.Degraded, r.Health);
            Assert.Equal(GHFindingSeverity.Suspect, Find(r, GHPerformanceDiagnosis.CodePlayerInput).Severity);
            Assert.NotNull(Find(r, GHPerformanceDiagnosis.CodeDebugOverhead));
            Assert.NotNull(Find(r, GHPerformanceDiagnosis.CodeOnBattery));
            Assert.Null(r.Primary);
            Assert.Equal(GHCauseLocation.InsideGame, r.Location);
        }

        [Fact]
        public void Location_HealthyKeepsLikelyFindingsWithoutPrimary()
        {
            GHDiagnosisFacts f = HealthyFacts();
            f.IsLowPower = true;

            GHDiagnosisResult r = GHPerformanceDiagnosis.Diagnose(f);

            Assert.Equal(GHDiagnosisHealth.Healthy, r.Health);
            Assert.Null(r.Primary);
            Assert.Equal(GHCauseLocation.None, r.Location);
            Assert.NotNull(Find(r, GHPerformanceDiagnosis.CodeLowPowerMode));
            string report = GHPerformanceDiagnosis.BuildReport(f, r, Now);
            Assert.Contains("(" + GHPerformanceDiagnosis.CodeLowPowerMode + ", likely)", report);
            Assert.Contains("  " + GHPerformanceDiagnosis.HealthyConclusion + "\n", report);
            Assert.Contains("Analyze Hitches", report);
        }

        [Fact]
        public void Location_InconclusiveIgnoresFindings()
        {
            GHDiagnosisFacts f = DegradedFacts();
            f.BackgroundVerdict = GHBackgroundVerdict.Busy;
            f.PlatformRenderLoopOn = false;

            GHDiagnosisResult r = GHPerformanceDiagnosis.Diagnose(f);

            Assert.Equal(GHCauseLocation.Unclear, r.Location);
            Assert.Null(r.Primary);
            Assert.NotNull(Find(r, GHPerformanceDiagnosis.CodeBackgroundBusy));
        }

        [Fact]
        public void Primary_LikelyBeatsEarlierAreaSuspect()
        {
            GHDiagnosisFacts f = DegradedFacts();
            f.HeadroomAfter = 0.9f;                                 /* Heat, Suspect */
            f.BackgroundVerdict = GHBackgroundVerdict.Busy;         /* Background, Likely */

            GHDiagnosisResult r = GHPerformanceDiagnosis.Diagnose(f);

            Assert.Equal(GHPerformanceDiagnosis.CodeBackgroundBusy, r.Primary.Code);
        }

        [Fact]
        public void Primary_AreaPriorityBreaksSeverityTie()
        {
            GHDiagnosisFacts f = DegradedFacts();
            f.ThermalRankBefore = 4;
            f.ThermalRankAfter = 4;                                 /* Heat, Likely */
            f.BackgroundVerdict = GHBackgroundVerdict.Busy;         /* Background, Likely */

            GHDiagnosisResult r = GHPerformanceDiagnosis.Diagnose(f);

            Assert.Equal(GHPerformanceDiagnosis.CodeThermalThrottled, r.Primary.Code);
            Assert.Equal(GHCauseLocation.External, r.Location);
        }

        [Fact]
        public void Primary_CodeOrdinalBreaksAreaTie()
        {
            GHDiagnosisFacts f = DegradedFacts();
            f.BackgroundVerdict = GHBackgroundVerdict.Busy;
            f.Processes = new List<GHProcessLoad> { new GHProcessLoad("MsMpEng", 5f, float.NaN) };

            GHDiagnosisResult r = GHPerformanceDiagnosis.Diagnose(f);

            Assert.Equal(GHPerformanceDiagnosis.CodeBackgroundBusy, r.Findings[0].Code);
            Assert.Equal("BG_ANTIVIRUS", r.Findings[1].Code);
            Assert.Equal(GHPerformanceDiagnosis.CodeBackgroundBusy, r.Primary.Code);
        }

        /* Severity, then rank group, then score, then area, then code */
        [Fact]
        public void Findings_SortedBySeverityThenArea()
        {
            GHDiagnosisFacts f = StressFacts();
            Causes(f, GHHitchCause.Gpu, 35.0, GHHitchCause.UiThreadLate, 31.0, GHHitchCause.UiThreadLateGc, 21.0,
                GHHitchCause.Compositor, 5.0, GHHitchCause.Unattributed, 8.0);
            GHDiagnosisResult r = GHPerformanceDiagnosis.Diagnose(f);

            Assert.True(r.Findings.Count > 5);
            int scored = 0;
            for (int i = 0; i < r.Findings.Count; i++)
            {
                if (!double.IsNaN(r.Findings[i].Score))
                    scored++;
            }
            Assert.True(scored >= 3);
            for (int i = 1; i < r.Findings.Count; i++)
            {
                GHDiagnosisFinding a = r.Findings[i - 1];
                GHDiagnosisFinding b = r.Findings[i];
                int ga = GHPerformanceDiagnosis.RankGroup(a);
                int gb = GHPerformanceDiagnosis.RankGroup(b);
                bool sameScore = double.IsNaN(a.Score) ? double.IsNaN(b.Score) : a.Score == b.Score;
                Assert.True(a.Severity > b.Severity
                    || (a.Severity == b.Severity && ga < gb)
                    || (a.Severity == b.Severity && ga == gb && !double.IsNaN(a.Score)
                        && (double.IsNaN(b.Score) || a.Score > b.Score))
                    || (a.Severity == b.Severity && ga == gb && sameScore && a.Area < b.Area)
                    || (a.Severity == b.Severity && ga == gb && sameScore && a.Area == b.Area
                        && string.CompareOrdinal(a.Code, b.Code) < 0),
                    a.Code + " before " + b.Code);
            }
        }

        [Fact]
        public void Primary_GcShareOutranksSmallKnownActivity()
        {
            GHDiagnosisFacts f = DegradedFacts();
            Causes(f, GHHitchCause.UiThreadLateGc, 40.0, GHHitchCause.Unattributed, 60.0);
            f.Processes = new List<GHProcessLoad> { new GHProcessLoad("MsMpEng", 5f, float.NaN) };

            GHDiagnosisResult r = GHPerformanceDiagnosis.Diagnose(f);

            Assert.Equal(GHFindingSeverity.Info, Find(r, "BG_ANTIVIRUS").Severity);
            Assert.Equal(GHPerformanceDiagnosis.CodeGcPressure, r.Primary.Code);
            Assert.Equal(GHCauseLocation.InsideGame, r.Location);
        }

        [Fact]
        public void Primary_ShareOutranksSettingsSuspect()
        {
            GHDiagnosisFacts f = DegradedFacts();
            f.NonDefaultPerformanceSettings = new List<string> { "tile batching off" };   /* Settings, Suspect */
            Causes(f, GHHitchCause.PaintCpu, 40.0, GHHitchCause.Unattributed, 60.0);          /* share 0.40 */

            GHDiagnosisResult r = GHPerformanceDiagnosis.Diagnose(f);

            Assert.Equal(GHPerformanceDiagnosis.CodePaintHeavy, r.Primary.Code);
            Assert.Equal(GHCauseLocation.InsideGame, r.Location);
        }

        [Fact]
        public void Primary_ShareOutranksBackgroundSuspect()
        {
            GHDiagnosisFacts f = DegradedFacts();
            f.BackgroundVerdict = GHBackgroundVerdict.Elevated;                           /* Background, Suspect */
            Causes(f, GHHitchCause.UiThreadLate, 40.0, GHHitchCause.Unattributed, 60.0);  /* share 0.40 */

            GHDiagnosisResult r = GHPerformanceDiagnosis.Diagnose(f);

            Assert.Equal(GHPerformanceDiagnosis.CodeUiThreadBusy, r.Primary.Code);
        }

        [Fact]
        public void Primary_HigherShareFirst()
        {
            GHDiagnosisFacts f = DegradedFacts();
            Causes(f, GHHitchCause.Gpu, 35.0, GHHitchCause.PaintCpu, 45.0, GHHitchCause.Unattributed, 20.0);

            GHDiagnosisResult r = GHPerformanceDiagnosis.Diagnose(f);

            Assert.Equal(GHPerformanceDiagnosis.CodePaintHeavy, r.Findings[0].Code);
            Assert.Equal(GHPerformanceDiagnosis.CodeGpuBound, r.Findings[1].Code);
            Assert.Equal(GHPerformanceDiagnosis.CodePaintHeavy, r.Primary.Code);
        }

        [Fact]
        public void Primary_LikelyWrongGpuBeatsLikelyGpuShare()
        {
            GHDiagnosisFacts f = DegradedFacts();
            f.RenderAdapterIsIntegrated = true;                                   /* WRONG_GPU, Likely */
            Causes(f, GHHitchCause.Gpu, 60.0, GHHitchCause.Unattributed, 40.0);   /* GPU_BOUND, Likely */

            GHDiagnosisResult r = GHPerformanceDiagnosis.Diagnose(f);

            Assert.Equal(GHFindingSeverity.Likely, Find(r, GHPerformanceDiagnosis.CodeGpuBound).Severity);
            Assert.Equal(GHPerformanceDiagnosis.CodeWrongGpu, r.Primary.Code);
        }

        [Fact]
        public void Primary_HeatSuspectOutranksShare()
        {
            GHDiagnosisFacts f = DegradedFacts();
            f.HeadroomAfter = 0.9f;                                                   /* THERMAL_RISING, Suspect */
            Causes(f, GHHitchCause.PaintCpu, 40.0, GHHitchCause.Unattributed, 60.0);  /* PAINT_HEAVY, Suspect */

            GHDiagnosisResult r = GHPerformanceDiagnosis.Diagnose(f);

            Assert.Equal(GHPerformanceDiagnosis.CodeThermalRising, r.Primary.Code);
            Assert.Equal(GHCauseLocation.External, r.Location);
        }

        [Fact]
        public void Primary_LikelyBackgroundBeatsLikelyShare()
        {
            GHDiagnosisFacts f = DegradedFacts();
            f.BackgroundVerdict = GHBackgroundVerdict.Busy;                           /* BACKGROUND_BUSY, Likely */
            Causes(f, GHHitchCause.PaintCpu, 60.0, GHHitchCause.Unattributed, 40.0);  /* PAINT_HEAVY, Likely */

            GHDiagnosisResult r = GHPerformanceDiagnosis.Diagnose(f);

            Assert.Equal(GHFindingSeverity.Likely, Find(r, GHPerformanceDiagnosis.CodePaintHeavy).Severity);
            Assert.Equal(GHPerformanceDiagnosis.CodeBackgroundBusy, r.Primary.Code);
        }

        [Fact]
        public void Checks_OneRowPerAreaWithWorstStatus()
        {
            GHDiagnosisFacts f = DegradedFacts();
            f.BackgroundVerdict = GHBackgroundVerdict.Busy;
            f.HeadroomAfter = 0.9f;

            GHDiagnosisResult r = GHPerformanceDiagnosis.Diagnose(f);

            Assert.Equal(8, r.Checks.Count);
            for (int i = 0; i < r.Checks.Count; i++)
                Assert.Equal((GHFindingArea)i, r.Checks[i].Area);
            Assert.Equal(GHPerformanceDiagnosis.StatusWarn, r.Checks[(int)GHFindingArea.Heat].Status);
            Assert.Equal(GHPerformanceDiagnosis.StatusBad, r.Checks[(int)GHFindingArea.Background].Status);
            GHCheckRow gpu = r.Checks[(int)GHFindingArea.GpuPath];
            Assert.Equal(GHPerformanceDiagnosis.StatusOk, gpu.Status);
            Assert.Contains("NVIDIA GeForce RTX 3060 (discrete)", gpu.Detail);
        }

        /* ------------------------------------------------------------------ Report */

        /* Facts that fire many rules with long strings everywhere. The debug flags make the
           health Inconclusive; StressFacts(false) clears them for a Degraded report with a
           primary cause. */
        private static GHDiagnosisFacts StressFacts()
        {
            return StressFacts(true);
        }

        private static GHDiagnosisFacts StressFacts(bool debugFlags)
        {
            string longName = new string('x', 120);
            GHDiagnosisFacts f = DegradedFacts();
            f.ThermalRankBefore = 1;
            f.ThermalRankAfter = 4;
            f.HeadroomAfter = 0.95f;
            f.CpuPerformancePctBefore = 99f;
            f.CpuPerformancePctAfter = 55f;
            f.IsLowPower = true;
            f.IsCharging = false;
            f.Fingerprint["settings.powerMode"] = "Best power efficiency";
            f.Fingerprint["os.pendingReboot"] = "true";
            f.Fingerprint["driver.gpu0.name"] = new string('d', 300);
            f.Fingerprint["hardware." + new string('k', 100)] = "v";
            f.BackgroundVerdict = GHBackgroundVerdict.Busy;
            f.BackgroundReason = "background load: " + new string('r', 83);
            f.OtherGpuPct = 45f;
            f.LowMemory = true;
            f.AvailableMemoryMinPct = 3f;
            f.MemoryPressureEvents = 4;
            f.Processes = new List<GHProcessLoad>
            {
                new GHProcessLoad("TiWorker", 12f, float.NaN),
                new GHProcessLoad("MsMpEng", 9f, float.NaN),
                new GHProcessLoad("SearchIndexer", 3f, float.NaN),
                new GHProcessLoad("OneDrive", 4f, float.NaN),
                new GHProcessLoad("MSBuild", 20f, float.NaN),
                new GHProcessLoad("vmmem", 7f, float.NaN),
                new GHProcessLoad("CompatTelRunner", 2f, float.NaN),
                new GHProcessLoad(longName, 33f, 12f)
            };
            f.RenderAdapterName = "Intel(R) Iris(R) Xe Graphics " + longName;
            f.RenderAdapterIsIntegrated = true;
            f.MainCanvasGpuContextLive = false;
            f.AssumedRefreshMismatch = true;
            f.MeasuredRefreshHz = 60f;
            f.DisplayMaxRefreshHz = 165f;
            f.TargetFps = 60f;
            f.Adapters = new List<string> { longName, "Intel(R) Iris(R) Xe Graphics" };
            f.NonDefaultPerformanceSettings = new List<string> { "tile batching off", "text blob caching off", longName };
            Causes(f, GHHitchCause.Gpu, 40.0, GHHitchCause.Compositor, 35.0, GHHitchCause.UiThreadLateGc, 30.0,
                GHHitchCause.PaintCpu, 50.0, GHHitchCause.Unattributed, 10.0);
            f.AllocationRateMBPerSec = 80f;
            f.IsDebugBuild = debugFlags;
            f.DebuggerAttached = debugFlags;
            f.VerboseLoggingOn = true;
            f.ContentEventCount = 7;
            f.HitchCount = 40;
            f.QuietGapCount = 1793;
            f.QuietHitchCount = 5;
            for (int k = 0; k < GHSmoothnessMetrics.ContentEventKinds; k++)
            {
                f.EventGapCounts[k] = 7000 + k;
                f.EventHitchCounts[k] = 3000 + k;
            }
            f.LongStallMs = 12345.678f;
            f.PresentSource = longName;
            f.SceneBefore = new GHDiagnosisScene();
            f.SceneBefore.LevelText = "The Gnomish Mines " + longName;
            f.SceneBefore.ZoomMode = GHDiagnosisScene.ZoomMinimap;
            f.SceneBefore.MapFontSize = 123.456f;
            f.GpuPreference = "Integrated " + longName;
            f.FrameDetailText = "Worst hitches:\n  " + new string('w', 150) + "\n  second line";
            return f;
        }

        [Fact]
        public void Report_FirstLineAndResultLine()
        {
            GHDiagnosisFacts f = DegradedFacts();
            string report = GHPerformanceDiagnosis.BuildReport(f, GHPerformanceDiagnosis.Diagnose(f), Now);
            string[] lines = Lines(report);

            Assert.Equal("GnollHack performance test report v2 2026-09-27 14:05", lines[0]);
            Assert.Equal("", lines[1]);
            Assert.Equal("Summary: The game stuttered noticeably.", lines[2]);
            Assert.Equal("  Most likely: Something inside the game; no outside cause was found.", lines[3]);
            Assert.Equal("", lines[4]);
            Assert.Equal("Result: DEGRADED  50.0 of 60 target fps, hitches 1.0 ms/s", lines[5]);
            Assert.Equal("Location: inside the game", lines[6]);
        }

        [Fact]
        public void Report_SummaryAboveResult()
        {
            GHDiagnosisFacts f = DegradedFacts();
            f.BackgroundVerdict = GHBackgroundVerdict.Busy;
            GHDiagnosisResult r = GHPerformanceDiagnosis.Diagnose(f);
            string report = GHPerformanceDiagnosis.BuildReport(f, r, Now);
            string[] lines = Lines(report);
            string[] summary = SummaryLines(report);

            Assert.Equal(GHPerformanceDiagnosis.CodeBackgroundBusy, r.Primary.Code);
            Assert.Equal("Summary: The game stuttered noticeably.", lines[2]);
            Assert.Equal("Summary: The game stuttered noticeably.", summary[0]);
            Assert.Equal("  Most likely: Other programs keep the machine busy.", summary[1]);
            Assert.StartsWith("  Try: ", summary[2]);
            Assert.Equal("Summary: The game stuttered noticeably. Most likely: Other programs keep the machine busy. "
                + "Try: " + r.Primary.Advice, Joined(summary));
            for (int i = 0; i < summary.Length; i++)
                Assert.True(summary[i].Length <= GHPerformanceTextReport.MaxLineWidth, summary[i]);

            int result = ResultIndex(lines);
            Assert.Equal(2 + summary.Length + 1, result);
            Assert.StartsWith("Location:", lines[result + 1]);
            Assert.Equal(lines[result] + " | " + lines[result + 1], GHPerformanceDiagnosis.HeadlineOf(report));

            /* A title or advice that reads like the headline stays indented in the summary */
            r.Primary.Title = "Result: HEALTHY";
            r.Primary.Advice = "Location: none. " + new string('a', 70) + " Result: HEALTHY";
            report = GHPerformanceDiagnosis.BuildReport(f, r, Now);
            lines = Lines(report);
            result = ResultIndex(lines);

            Assert.Equal("  Most likely: Result: HEALTHY.", SummaryLines(report)[1]);
            Assert.StartsWith("Result: DEGRADED ", lines[result]);
            Assert.Equal(lines[result] + " | " + lines[result + 1], GHPerformanceDiagnosis.HeadlineOf(report));
            Assert.Equal("Location: external", lines[result + 1]);
        }

        [Fact]
        public void Report_SummaryInconclusive_SaysWhy()
        {
            GHDiagnosisFacts f = HealthyFacts();
            f.OnScreenIntervalCount = 50;
            string report = Report(f);
            string[] summary = SummaryLines(report);

            Assert.Equal("Summary: The test could not judge how the game ran: too few frames were shown (50). "
                + "Try: Run the test again.", Joined(summary));
            Assert.Equal("  Try: Run the test again.", summary[summary.Length - 1]);
            Assert.DoesNotContain("Most likely:", Joined(summary));
            for (int i = 0; i < summary.Length; i++)
                Assert.True(summary[i].Length <= GHPerformanceTextReport.MaxLineWidth, summary[i]);
        }

        [Fact]
        public void Report_SummaryHealthy_RanSmoothly()
        {
            string[] summary = SummaryLines(Report(HealthyFacts()));

            Assert.Equal(new string[] { "Summary: The game ran smoothly; no problem was measured." }, summary);
        }

        [Fact]
        public void ResultLine_ShowsDisplayCap()
        {
            GHDiagnosisFacts f = HealthyFacts();
            f.TargetFps = 120f;
            Assert.Equal("Result: HEALTHY  59.5 of 120 target fps, hitches 1.0 ms/s (display 60 Hz)",
                ResultLineOf(Report(f)));

            /* Too long with "target": left out */
            f.TargetFps = 144f;
            f.DisplayedFps = 59.9f;
            f.HitchRatioMsPerSec = 123.4f;
            f.PlatformRenderLoopOn = false;
            string line = ResultLineOf(Report(f));
            Assert.Equal("Result: INCONCLUSIVE  59.9 of 144 fps, hitches 123.4 ms/s (display 60 Hz)", line);
            Assert.True(line.Length <= GHPerformanceTextReport.MaxLineWidth);
        }

        [Fact]
        public void ResultLine_NoSuffixAt5994()
        {
            GHDiagnosisFacts f = HealthyFacts();
            f.MeasuredRefreshHz = 59.94f;

            Assert.Equal("Result: HEALTHY  59.5 of 60 target fps, hitches 1.0 ms/s", ResultLineOf(Report(f)));
        }

        [Fact]
        public void Report_SectionOrder()
        {
            GHDiagnosisFacts f = StressFacts();
            string report = GHPerformanceDiagnosis.BuildReport(f, GHPerformanceDiagnosis.Diagnose(f), Now);
            string[] sections =
            {
                "Summary:", "Result:", "Location:", "Most likely cause:", "Also found:", "Checks:", "Measurements:",
                "Hitch causes:", "Notes:", "Environment:", "Facts:", "Frame detail:"
            };

            int last = -1;
            for (int i = 0; i < sections.Length; i++)
            {
                int at = report.IndexOf("\n" + sections[i], StringComparison.Ordinal);
                Assert.True(at > last, sections[i] + " out of order");
                last = at;
            }
        }

        [Fact]
        public void Report_LinesFitWidthExceptAppendix_AndUseLf()
        {
            GHDiagnosisFacts[] cases =
            {
                StressFacts(), StressFacts(false), HealthyFacts(), DegradedFacts(), new GHDiagnosisFacts()
            };
            for (int c = 0; c < cases.Length; c++)
            {
                string report = GHPerformanceDiagnosis.BuildReport(cases[c], GHPerformanceDiagnosis.Diagnose(cases[c]), Now);
                Assert.DoesNotContain("\r", report);
                Assert.EndsWith("\n", report);
                string[] lines = Lines(report);
                bool appendix = false;
                for (int i = 0; i < lines.Length; i++)
                {
                    if (appendix)
                        break;
                    Assert.True(lines[i].Length <= GHPerformanceTextReport.MaxLineWidth,
                        "line " + i + " is " + lines[i].Length + " wide: " + lines[i]);
                    for (int k = 0; k < lines[i].Length; k++)
                        Assert.True(lines[i][k] < 128, "non-ASCII character in line " + i);
                    if (lines[i] == "Frame detail:")
                        appendix = true;
                }
                Assert.True(appendix);
            }
        }

        [Fact]
        public void Report_AppendixVerbatim()
        {
            GHDiagnosisFacts f = StressFacts();
            string report = GHPerformanceDiagnosis.BuildReport(f, GHPerformanceDiagnosis.Diagnose(f), Now);

            Assert.EndsWith("Frame detail:\n" + f.FrameDetailText + "\n", report);
        }

        [Fact]
        public void Report_NoAppendix_SaysNotAvailable()
        {
            GHDiagnosisFacts f = HealthyFacts();
            string report = GHPerformanceDiagnosis.BuildReport(f, GHPerformanceDiagnosis.Diagnose(f), Now);

            Assert.EndsWith("Frame detail:\n  n/a\n", report);
        }

        [Fact]
        public void Report_Deterministic()
        {
            string a = GHPerformanceDiagnosis.BuildReport(StressFacts(), GHPerformanceDiagnosis.Diagnose(StressFacts()), Now);
            string b = GHPerformanceDiagnosis.BuildReport(StressFacts(), GHPerformanceDiagnosis.Diagnose(StressFacts()), Now);
            string c = GHPerformanceDiagnosis.BuildReport(StressFacts(), null, Now);

            Assert.Equal(a, b);
            Assert.Equal(a, c);
        }

        [Fact]
        public void Report_FactsBlock_NaNAsNotAvailableAndFindingsLast()
        {
            GHDiagnosisFacts f = DegradedFacts();
            f.PacingErrorRmsMs = float.NaN;
            f.BackgroundVerdict = GHBackgroundVerdict.Busy;
            f.IsCharging = false;
            GHDiagnosisResult r = GHPerformanceDiagnosis.Diagnose(f);
            string report = GHPerformanceDiagnosis.BuildReport(f, r, Now);

            Assert.Contains("\nFacts:\n  formatVersion=2\n", report);
            Assert.Contains("\n  pacingErrorRmsMs=n/a\n", report);
            Assert.Contains("\n  windowSeconds=30\n", report);
            Assert.Contains("\n  displayedFps=50\n", report);
            Assert.Contains("\n  backgroundVerdict=busy\n", report);
            Assert.Contains("\n  mainCanvasGpuContextLive=true\n", report);
            Assert.Contains("\n  primary=" + GHPerformanceDiagnosis.CodeBackgroundBusy + "\n", report);

            List<string> codes = new List<string>();
            for (int i = 0; i < r.Findings.Count; i++)
                codes.Add(r.Findings[i].Code);
            string findingsLine = "  findings=" + string.Join(",", codes) + "\n";
            Assert.Contains(findingsLine + "\nFrame detail:", report);
        }

        [Fact]
        public void Report_FactsBlock_NoFindingsGivesEmptyList()
        {
            GHDiagnosisFacts f = HealthyFacts();
            string report = GHPerformanceDiagnosis.BuildReport(f, GHPerformanceDiagnosis.Diagnose(f), Now);

            Assert.Contains("\n  primary=none\n  findings=\n\nFrame detail:", report);
        }

        [Fact]
        public void Report_Environment_SortedWithoutMetaAndMostComponents()
        {
            GHDiagnosisFacts f = HealthyFacts();
            string report = GHPerformanceDiagnosis.BuildReport(f, GHPerformanceDiagnosis.Diagnose(f), Now);

            Assert.Contains("\nEnvironment:\n  code.appVersion: 4.1.3\n  component.SkiaSharp: 3.119.0\n"
                + "  os.pendingReboot: false\n  settings.powerMode: Balanced\n  settings.powerPlan: Balanced\n\n", report);
            Assert.DoesNotContain("meta.fingerprintVersion", report);
            Assert.DoesNotContain("System.Text.Json", report);
        }

        [Fact]
        public void Report_Environment_LongValueShortenedInTheMiddle()
        {
            GHDiagnosisFacts f = HealthyFacts();
            f.Fingerprint["driver.gpu0.version"] = "HEAD" + new string('m', 200) + "TAIL";
            string report = GHPerformanceDiagnosis.BuildReport(f, GHPerformanceDiagnosis.Diagnose(f), Now);

            string[] lines = Lines(report);
            string line = Array.Find(lines, l => l.StartsWith("  driver.gpu0.version: ", StringComparison.Ordinal));
            Assert.NotNull(line);
            Assert.Equal(GHPerformanceTextReport.MaxLineWidth, line.Length);
            Assert.Contains("HEAD", line);
            Assert.Contains("...", line);
            Assert.EndsWith("TAIL", line);
        }

        [Fact]
        public void Report_CheckRowsAndHitchCauses()
        {
            GHDiagnosisFacts f = DegradedFacts();
            Causes(f, GHHitchCause.PaintCpu, 60.0, GHHitchCause.Unattributed, 40.0);
            string report = GHPerformanceDiagnosis.BuildReport(f, GHPerformanceDiagnosis.Diagnose(f), Now);

            Assert.Contains("\n  GPU         ok    rendering on NVIDIA GeForce RTX 3060 (discrete)\n", report);
            Assert.Contains("\n  Game        BAD   Map drawing is CPU-heavy (PAINT_HEAVY)\n", report);
            Assert.Contains("\n  PaintCpu              300.0 ms    60 %\n", report);
            Assert.Contains("\n  Unattributed          200.0 ms    40 %\n", report);
            Assert.Contains("\n  total                 500.0 ms  (16.7 ms/s)\n", report);
        }

        [Fact]
        public void Report_NotesEndWithCaveat()
        {
            GHDiagnosisFacts f = HealthyFacts();
            f.ContentEventCount = 3;
            string report = GHPerformanceDiagnosis.BuildReport(f, GHPerformanceDiagnosis.Diagnose(f), Now);

            int notes = report.IndexOf("\nNotes:\n", StringComparison.Ordinal);
            int input = report.IndexOf(GHPerformanceDiagnosis.CodePlayerInput, notes, StringComparison.Ordinal);
            int caveat = report.IndexOf("  " + GHPerformanceDiagnosis.CaveatLine + "\n\nEnvironment:", StringComparison.Ordinal);
            Assert.True(notes >= 0 && input > notes && caveat > input);
        }

        [Fact]
        public void Report_SceneLine()
        {
            GHDiagnosisFacts f = HealthyFacts();
            string report = Report(f);
            Assert.Contains("\n  Scene: n/a\n", report);
            Assert.Contains("\n  scene.levelText=n/a\n  scene.zoomMode=n/a\n  scene.mapFontSize=n/a\n", report);

            f.SceneBefore = new GHDiagnosisScene();
            f.SceneBefore.LevelText = "Dlvl:3";
            f.SceneBefore.ZoomMode = GHDiagnosisScene.ZoomAlternate;
            f.SceneBefore.MapFontSize = 42.5f;
            report = Report(f);

            Assert.Contains("\n  Window: 30.0 s, target 60 fps, displayed 59.5 fps\n"
                + "  Scene: Dlvl:3, zoom alternate, map font 42.5\n", report);
            Assert.Contains("\n  scene.levelText=Dlvl:3\n  scene.zoomMode=alternate\n"
                + "  scene.mapFontSize=42.5\n", report);
        }

        [Fact]
        public void Report_FactsCarryVersionAndFrameMarker()
        {
            GHDiagnosisFacts f = HealthyFacts();
            string report = Report(f);

            Assert.Contains("\nFacts:\n  formatVersion=" + GHPerformanceDiagnosis.ReportFormatVersion + "\n", report);
            Assert.Contains("\n  nominalWindowSeconds=30\n", report);
            Assert.Contains("\n  onScreenIntervalCount=1800\n", report);
            Assert.Contains("\n  quietGapCount=1800\n  quietHitchCount=2\n", report);
            Assert.Contains("\n  eventGapCounts=0,0,0,0,0,0,0,0,0,0\n", report);
            Assert.Contains("\n  metricsVersion=" + GHSmoothnessMetrics.MetricsVersion + "\n", report);
            Assert.Contains("\n  presentSource=Estimated\n", report);
            Assert.Contains("\n  frameMarker=on\n", report);
            Assert.Contains("\n  Content: hitches in 0.1 % of 1800 quiet gaps, n/a of 0 with events\n", report);

            f.MetricsVersion = -1;
            f.OnScreenIntervalCount = -1;
            f.FrameMarkerOff = true;
            f.EventGapCounts = null;
            f.PresentSource = null;
            report = Report(f);

            Assert.Contains("\n  metricsVersion=n/a\n", report);
            Assert.Contains("\n  onScreenIntervalCount=n/a\n", report);
            Assert.Contains("\n  frameMarker=off\n", report);
            Assert.Contains("\n  presentSource=n/a\n", report);
            Assert.DoesNotContain("eventGapCounts=", report);
            Assert.Contains("\n  eventHitchCounts=", report);
        }

        [Fact]
        public void Report_FactsCarryCompositorCoverage()
        {
            GHDiagnosisFacts f = HealthyFacts();
            f.CompositorCoverage = -1;
            Assert.Contains("\n  compositorCoverage=n/a\n", Report(f));

            f.CompositorCoverage = 0.25;
            Assert.Contains("\n  compositorCoverage=0.25\n", Report(f));
        }

        [Fact]
        public void Report_FactsCarryVsyncCorrectedShare()
        {
            GHDiagnosisFacts f = HealthyFacts();
            f.VsyncCorrectedShare = -1;
            Assert.Contains("\n  vsyncCorrectedShare=n/a\n", Report(f));

            f.VsyncCorrectedShare = 0.5;
            Assert.Contains("\n  vsyncCorrectedShare=0.50\n", Report(f));
        }

        [Fact]
        public void Report_FactsCarryForcedGcCount()
        {
            GHDiagnosisFacts f = HealthyFacts();
            Assert.Contains("\n  gcCount=1\n  forcedGcCount=0\n", Report(f));

            f.ForcedGcCount = 3;
            Assert.Contains("\n  gcCount=1\n  forcedGcCount=3\n", Report(f));
        }

        [Fact]
        public void Report_AlsoLine_NamesTheOtherSide()
        {
            /* A scored primary names the strongest external finding */
            GHDiagnosisFacts f = DegradedFacts();
            f.BackgroundVerdict = GHBackgroundVerdict.Elevated;
            Causes(f, GHHitchCause.PaintCpu, 60.0, GHHitchCause.Unattributed, 40.0);
            GHDiagnosisResult r = GHPerformanceDiagnosis.Diagnose(f);
            string report = GHPerformanceDiagnosis.BuildReport(f, r, Now);

            Assert.Equal(GHPerformanceDiagnosis.CodePaintHeavy, r.Primary.Code);
            Assert.Contains("\n  Also: Background load is elevated (BACKGROUND_ELEVATED, suspect)\n\nAlso found:\n",
                report);
            string[] lines = Lines(report);
            int result = ResultIndex(lines);
            Assert.Equal(lines[result] + " | " + lines[result + 1], GHPerformanceDiagnosis.HeadlineOf(report));

            /* An unscored primary names the strongest share */
            f.BackgroundVerdict = GHBackgroundVerdict.Busy;
            Causes(f, GHHitchCause.PaintCpu, 40.0, GHHitchCause.Unattributed, 60.0);
            r = GHPerformanceDiagnosis.Diagnose(f);
            report = GHPerformanceDiagnosis.BuildReport(f, r, Now);

            Assert.Equal(GHPerformanceDiagnosis.CodeBackgroundBusy, r.Primary.Code);
            Assert.Contains("\n  Also: Map drawing is CPU-heavy (PAINT_HEAVY, suspect)\n\nAlso found:\n", report);

            /* Nothing on the other side: no line */
            f.BackgroundVerdict = GHBackgroundVerdict.Quiet;
            report = Report(f);
            Assert.DoesNotContain("\n  Also: ", report);
        }

        /* ------------------------------------------------------------------ HeadlineOf */

        [Fact]
        public void HeadlineOf_RoundTripsReport()
        {
            GHDiagnosisFacts[] cases =
            {
                StressFacts(), StressFacts(false), HealthyFacts(), DegradedFacts(), new GHDiagnosisFacts()
            };
            for (int c = 0; c < cases.Length; c++)
            {
                string report = GHPerformanceDiagnosis.BuildReport(cases[c], GHPerformanceDiagnosis.Diagnose(cases[c]), Now);
                string[] lines = Lines(report);
                int result = ResultIndex(lines);

                Assert.True(result > 2);
                Assert.StartsWith("Location:", lines[result + 1]);
                Assert.Equal(lines[result] + " | " + lines[result + 1], GHPerformanceDiagnosis.HeadlineOf(report));
            }
        }

        [Fact]
        public void HeadlineOf_CrLfText_Trimmed()
        {
            Assert.Equal("Result: POOR  20.0 of 60 target fps, hitches 40.0 ms/s | Location: external",
                GHPerformanceDiagnosis.HeadlineOf("title\r\n\r\nResult: POOR  20.0 of 60 target fps, hitches 40.0 ms/s  \r\n"
                    + "Location: external\r\n"));
        }

        [Theory]
        [InlineData(null)]
        [InlineData("")]
        [InlineData("GnollHack performance test report v1 2026-09-27 14:05\n")]
        [InlineData("Result: HEALTHY\n")]
        [InlineData("Location: none\n")]
        public void HeadlineOf_MissingLines_Null(string text)
        {
            Assert.Null(GHPerformanceDiagnosis.HeadlineOf(text));
        }
    }
}
