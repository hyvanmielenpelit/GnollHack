using System;
using System.Collections.Generic;
using GnollHackX.Performance;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers GHBackgroundLoad: the derived other-CPU signal, the window summary
       (nearest-rank percentiles, coverage, NaN handling, spike share, memory counters),
       every verdict threshold just below and at its boundary, the unknown rules, the
       reason text and its suspects, the known-process table and the quiet gate. */
    public class GHBackgroundLoadTests
    {
        private const float OwnCpu = 5f;

        private static GHSystemLoadSample Sample(int second, float otherCpu, float diskBusy)
        {
            GHSystemLoadSample s = GHSystemLoadSample.Empty;
            s.TimestampTicks = second * TimeSpan.TicksPerSecond;
            s.SystemCpuPct = float.IsNaN(otherCpu) ? float.NaN : otherCpu + OwnCpu;
            s.OwnCpuPct = OwnCpu;
            s.DiskBusyPct = diskBusy;
            return s;
        }

        private static long Seconds(int s)
        {
            return s * TimeSpan.TicksPerSecond;
        }

        /* A summary that classifies Quiet: full coverage, low other CPU, every other
           signal unreported. */
        private static GHBackgroundSummary QuietSummary()
        {
            return new GHBackgroundSummary
            {
                Samples = 60,
                OtherCpuMeanPct = 1f,
                OtherCpuP90Pct = 2f,
                OtherCpuMaxPct = 4f,
                OtherCpuSpikeShare = 0f,
                Coverage = 1f
            };
        }

        private static GHBackgroundVerdict ClassifyAlone(GHBackgroundSummary s)
        {
            string reason;
            return GHBackgroundLoad.Classify(s, null, float.NaN, out reason);
        }

        private static GHProcessLoad[] Processes(params GHProcessLoad[] processes)
        {
            return processes;
        }

        [Fact]
        public void OtherCpuPct_SystemBelowOwn_ClampsToZero()
        {
            GHSystemLoadSample s = GHSystemLoadSample.Empty;
            s.SystemCpuPct = 3f;
            s.OwnCpuPct = 4f;

            Assert.Equal(0f, s.OtherCpuPct);
        }

        [Fact]
        public void OtherCpuPct_EitherInputNaN_IsNaN()
        {
            GHSystemLoadSample s = GHSystemLoadSample.Empty;
            s.SystemCpuPct = 30f;
            Assert.True(float.IsNaN(s.OtherCpuPct));

            s.SystemCpuPct = float.NaN;
            s.OwnCpuPct = 4f;
            Assert.True(float.IsNaN(s.OtherCpuPct));
        }

        [Fact]
        public void Empty_EverySignalUnsupported()
        {
            GHSystemLoadSample s = GHSystemLoadSample.Empty;

            Assert.True(float.IsNaN(s.SystemCpuPct));
            Assert.True(float.IsNaN(s.OwnCpuPct));
            Assert.True(float.IsNaN(s.DiskBusyPct));
            Assert.True(float.IsNaN(s.AvailableMemoryPct));
            Assert.True(float.IsNaN(s.HardFaultsPerSec));
            Assert.Equal(-1L, s.AvailableMemoryMB);
            Assert.False(s.LowMemory);
            Assert.Equal(0, s.MemoryPressureEvents);
        }

        [Fact]
        public void Summarize_TenSamples_NearestRankPercentilesAndMean()
        {
            GHSystemLoadSample[] samples = new GHSystemLoadSample[10];
            for (int i = 0; i < 10; i++)
                samples[i] = Sample(i, i + 1, 2 * (i + 1));

            GHBackgroundSummary s = GHBackgroundLoad.Summarize(samples, 10, 0, Seconds(10), 1000);

            Assert.Equal(10, s.Samples);
            Assert.Equal(5.5f, s.OtherCpuMeanPct, 4);
            Assert.Equal(9f, s.OtherCpuP90Pct);   /* rank ceil(0.9 * 10) = 9 */
            Assert.Equal(10f, s.OtherCpuMaxPct);
            Assert.Equal(0f, s.OtherCpuSpikeShare);
            Assert.Equal(11f, s.DiskBusyMeanPct, 4);
            Assert.Equal(18f, s.DiskBusyP90Pct);
            Assert.Equal(1f, s.Coverage);
            Assert.True(s.HasCpuSignal);
            Assert.False(s.HasMemorySignal);
        }

        [Fact]
        public void Summarize_NaNValuesSkippedPerSignal()
        {
            GHSystemLoadSample[] samples = new GHSystemLoadSample[4];
            samples[0] = Sample(0, 4f, 10f);
            samples[1] = Sample(1, float.NaN, float.NaN);
            samples[2] = Sample(2, 8f, 30f);
            samples[3] = Sample(3, 6f, float.NaN);
            samples[0].AvailableMemoryPct = 40f;
            samples[1].AvailableMemoryPct = 35f;
            samples[3].AvailableMemoryPct = 50f;
            samples[1].AvailableMemoryMB = 1000;
            samples[2].AvailableMemoryMB = 900;

            GHBackgroundSummary s = GHBackgroundLoad.Summarize(samples, 4, 0, Seconds(4), 1000);

            Assert.Equal(4, s.Samples);
            Assert.Equal(6f, s.OtherCpuMeanPct, 4);
            Assert.Equal(8f, s.OtherCpuMaxPct);
            Assert.Equal(20f, s.DiskBusyMeanPct, 4);
            Assert.Equal(30f, s.DiskBusyP90Pct);
            Assert.Equal(35f, s.AvailableMemoryMinPct);
            Assert.Equal(900L, s.AvailableMemoryMinMB);
            Assert.True(float.IsNaN(s.HardFaultsP90PerSec));
            Assert.True(s.HasCpuSignal);
            Assert.True(s.HasMemorySignal);
        }

        [Fact]
        public void Summarize_RangeIsHalfOpen()
        {
            GHSystemLoadSample[] samples = new GHSystemLoadSample[10];
            for (int i = 0; i < 10; i++)
                samples[i] = Sample(i, i, 0f);

            GHBackgroundSummary s = GHBackgroundLoad.Summarize(samples, 10, Seconds(2), Seconds(5), 1000);

            Assert.Equal(3, s.Samples);
            Assert.Equal(4f, s.OtherCpuMaxPct);
            Assert.Equal(1f, s.Coverage);
        }

        [Fact]
        public void Summarize_CountBelowArrayLength_IgnoresTheRest()
        {
            GHSystemLoadSample[] samples = new GHSystemLoadSample[10];
            for (int i = 0; i < 10; i++)
                samples[i] = Sample(i, i, 0f);

            GHBackgroundSummary s = GHBackgroundLoad.Summarize(samples, 4, 0, Seconds(10), 1000);

            Assert.Equal(4, s.Samples);
            Assert.Equal(3f, s.OtherCpuMaxPct);
        }

        [Fact]
        public void Summarize_HalfTheExpectedSamples_CoverageIsHalf()
        {
            GHSystemLoadSample[] samples = new GHSystemLoadSample[5];
            for (int i = 0; i < 5; i++)
                samples[i] = Sample(2 * i, 1f, 0f);

            GHBackgroundSummary s = GHBackgroundLoad.Summarize(samples, 5, 0, Seconds(10), 1000);

            Assert.Equal(0.5f, s.Coverage, 4);
        }

        [Fact]
        public void Summarize_NoSamplesInRange_ZeroCoverageAndNoSignals()
        {
            GHSystemLoadSample[] samples = { Sample(20, 5f, 5f) };

            GHBackgroundSummary s = GHBackgroundLoad.Summarize(samples, 1, 0, Seconds(10), 1000);

            Assert.Equal(0, s.Samples);
            Assert.Equal(0f, s.Coverage);
            Assert.False(s.HasCpuSignal);
            Assert.False(s.HasMemorySignal);
            Assert.Equal(-1L, s.AvailableMemoryMinMB);
            Assert.Equal(GHBackgroundVerdict.Unknown, ClassifyAlone(s));
        }

        [Fact]
        public void Summarize_SpikeShare_CountsSamplesAtOrAboveSpikeThreshold()
        {
            GHSystemLoadSample[] samples = new GHSystemLoadSample[10];
            for (int i = 0; i < 10; i++)
                samples[i] = Sample(i, 1f, 0f);
            samples[3] = Sample(3, 49f, 0f);
            samples[7] = Sample(7, 50f, 0f);

            GHBackgroundSummary s = GHBackgroundLoad.Summarize(samples, 10, 0, Seconds(10), 1000);

            Assert.Equal(0.1f, s.OtherCpuSpikeShare, 4);
            Assert.Equal(GHBackgroundVerdict.Busy, ClassifyAlone(s));
        }

        [Fact]
        public void Summarize_MemoryPressure_LastMinusFirstInRange_OrderIndependent()
        {
            GHSystemLoadSample s0 = Sample(0, 1f, 0f);
            GHSystemLoadSample s1 = Sample(1, 1f, 0f);
            GHSystemLoadSample s2 = Sample(2, 1f, 0f);
            GHSystemLoadSample s3 = Sample(3, 1f, 0f);
            GHSystemLoadSample outside = Sample(10, 1f, 0f);
            s0.MemoryPressureEvents = 3;
            s1.MemoryPressureEvents = 3;
            s2.MemoryPressureEvents = 4;
            s3.MemoryPressureEvents = 6;
            outside.MemoryPressureEvents = 9;
            GHSystemLoadSample[] samples = { s3, s0, outside, s2, s1 };

            GHBackgroundSummary s = GHBackgroundLoad.Summarize(samples, samples.Length, 0, Seconds(4), 1000);

            Assert.Equal(3, s.MemoryPressureEvents);
        }

        [Fact]
        public void Summarize_LowMemoryInAnySample_IsSet()
        {
            GHSystemLoadSample[] samples = { Sample(0, 1f, 0f), Sample(1, 1f, 0f) };
            samples[1].LowMemory = true;

            GHBackgroundSummary s = GHBackgroundLoad.Summarize(samples, 2, 0, Seconds(2), 1000);

            Assert.True(s.LowMemory);
        }

        [Fact]
        public void Summarize_NoCpuSignal_HasCpuSignalFalse()
        {
            GHSystemLoadSample[] samples = { Sample(0, float.NaN, 5f), Sample(1, float.NaN, 5f) };

            GHBackgroundSummary s = GHBackgroundLoad.Summarize(samples, 2, 0, Seconds(2), 1000);

            Assert.False(s.HasCpuSignal);
            Assert.True(float.IsNaN(s.OtherCpuMeanPct));
            Assert.True(float.IsNaN(s.OtherCpuSpikeShare));
            Assert.Equal(5f, s.DiskBusyP90Pct);
        }

        [Theory]
        [InlineData(9.9f, GHBackgroundVerdict.Quiet)]
        [InlineData(10f, GHBackgroundVerdict.Elevated)]
        [InlineData(24.9f, GHBackgroundVerdict.Elevated)]
        [InlineData(25f, GHBackgroundVerdict.Busy)]
        public void Classify_OtherCpuP90_Boundaries(float p90, GHBackgroundVerdict expected)
        {
            GHBackgroundSummary s = QuietSummary();
            s.OtherCpuP90Pct = p90;

            Assert.Equal(expected, ClassifyAlone(s));
        }

        [Theory]
        [InlineData(0.09f, GHBackgroundVerdict.Quiet)]
        [InlineData(0.1f, GHBackgroundVerdict.Busy)]
        public void Classify_SpikeShare_Boundaries(float share, GHBackgroundVerdict expected)
        {
            GHBackgroundSummary s = QuietSummary();
            s.OtherCpuSpikeShare = share;

            Assert.Equal(expected, ClassifyAlone(s));
        }

        [Theory]
        [InlineData(49.9f, GHBackgroundVerdict.Quiet)]
        [InlineData(50f, GHBackgroundVerdict.Elevated)]
        [InlineData(100f, GHBackgroundVerdict.Elevated)]
        public void Classify_DiskBusyP90_Boundaries_NeverBusy(float p90, GHBackgroundVerdict expected)
        {
            GHBackgroundSummary s = QuietSummary();
            s.DiskBusyP90Pct = p90;

            Assert.Equal(expected, ClassifyAlone(s));
        }

        [Theory]
        [InlineData(10f, GHBackgroundVerdict.Quiet)]
        [InlineData(9.9f, GHBackgroundVerdict.Elevated)]
        [InlineData(5f, GHBackgroundVerdict.Elevated)]
        [InlineData(4.9f, GHBackgroundVerdict.Busy)]
        public void Classify_AvailableMemoryMin_Boundaries(float minPct, GHBackgroundVerdict expected)
        {
            GHBackgroundSummary s = QuietSummary();
            s.AvailableMemoryMinPct = minPct;

            Assert.Equal(expected, ClassifyAlone(s));
        }

        [Theory]
        [InlineData(199.9f, GHBackgroundVerdict.Quiet)]
        [InlineData(200f, GHBackgroundVerdict.Elevated)]
        [InlineData(999.9f, GHBackgroundVerdict.Elevated)]
        [InlineData(1000f, GHBackgroundVerdict.Busy)]
        public void Classify_HardFaultsP90_Boundaries(float p90, GHBackgroundVerdict expected)
        {
            GHBackgroundSummary s = QuietSummary();
            s.HardFaultsP90PerSec = p90;

            Assert.Equal(expected, ClassifyAlone(s));
        }

        [Theory]
        [InlineData(9.9f, GHBackgroundVerdict.Quiet)]
        [InlineData(10f, GHBackgroundVerdict.Elevated)]
        [InlineData(29.9f, GHBackgroundVerdict.Elevated)]
        [InlineData(30f, GHBackgroundVerdict.Busy)]
        public void Classify_OtherGpu_Boundaries(float gpu, GHBackgroundVerdict expected)
        {
            string reason;
            GHBackgroundVerdict v = GHBackgroundLoad.Classify(QuietSummary(), null, gpu, out reason);

            Assert.Equal(expected, v);
        }

        [Fact]
        public void Classify_OtherGpuNaN_IsIgnored()
        {
            string reason;
            GHBackgroundVerdict v = GHBackgroundLoad.Classify(QuietSummary(), null, float.NaN, out reason);

            Assert.Equal(GHBackgroundVerdict.Quiet, v);
            Assert.Null(reason);
        }

        [Theory]
        [InlineData(0, GHBackgroundVerdict.Quiet)]
        [InlineData(1, GHBackgroundVerdict.Elevated)]
        public void Classify_MemoryPressureEvents_Boundaries(int events, GHBackgroundVerdict expected)
        {
            GHBackgroundSummary s = QuietSummary();
            s.MemoryPressureEvents = events;

            Assert.Equal(expected, ClassifyAlone(s));
        }

        [Fact]
        public void Classify_LowMemory_IsBusy()
        {
            GHBackgroundSummary s = QuietSummary();
            s.LowMemory = true;

            string reason;
            GHBackgroundVerdict v = GHBackgroundLoad.Classify(s, null, float.NaN, out reason);

            Assert.Equal(GHBackgroundVerdict.Busy, v);
            Assert.Equal("background load: low memory", reason);
        }

        [Theory]
        [InlineData(1.9f, GHBackgroundVerdict.Quiet)]
        [InlineData(2f, GHBackgroundVerdict.Elevated)]
        public void Classify_KnownActivityCpu_Boundaries(float cpu, GHBackgroundVerdict expected)
        {
            string reason;
            GHBackgroundVerdict v = GHBackgroundLoad.Classify(QuietSummary(),
                Processes(new GHProcessLoad("TiWorker", cpu, float.NaN)), float.NaN, out reason);

            Assert.Equal(expected, v);
        }

        [Fact]
        public void Classify_KnownActivity_SumsProcessesOfOneCategory()
        {
            string reason;
            GHBackgroundVerdict v = GHBackgroundLoad.Classify(QuietSummary(),
                Processes(new GHProcessLoad("TiWorker", 1f, float.NaN), new GHProcessLoad("MoUsoCoreWorker", 1f, float.NaN)),
                float.NaN, out reason);

            Assert.Equal(GHBackgroundVerdict.Elevated, v);
            Assert.StartsWith("background load: windows-update 2 %", reason);
        }

        [Fact]
        public void Classify_MeasurementAndUnknownProcesses_AreNotKnownActivities()
        {
            string reason;
            GHBackgroundVerdict v = GHBackgroundLoad.Classify(QuietSummary(),
                Processes(new GHProcessLoad("PresentMon", 40f, float.NaN), new GHProcessLoad("chrome", 5f, float.NaN)),
                float.NaN, out reason);

            Assert.Equal(GHBackgroundVerdict.Quiet, v);
            Assert.Null(reason);
        }

        [Theory]
        [InlineData(0.49f, GHBackgroundVerdict.Unknown)]
        [InlineData(0.5f, GHBackgroundVerdict.Quiet)]
        public void Classify_Coverage_Boundaries(float coverage, GHBackgroundVerdict expected)
        {
            GHBackgroundSummary s = QuietSummary();
            s.Coverage = coverage;

            Assert.Equal(expected, ClassifyAlone(s));
        }

        [Fact]
        public void Classify_LowCoverage_IsUnknownEvenWhenBusy()
        {
            GHBackgroundSummary s = QuietSummary();
            s.Coverage = 0.3f;
            s.OtherCpuP90Pct = 80f;
            s.LowMemory = true;

            Assert.Equal(GHBackgroundVerdict.Unknown, ClassifyAlone(s));
        }

        [Fact]
        public void Classify_NullSummary_IsUnknown()
        {
            string reason;
            Assert.Equal(GHBackgroundVerdict.Unknown, GHBackgroundLoad.Classify(null, null, float.NaN, out reason));
            Assert.Null(reason);
        }

        [Fact]
        public void Classify_NoCpuSignalAndNoMemoryRule_IsUnknown()
        {
            GHBackgroundSummary s = new GHBackgroundSummary { Samples = 60, Coverage = 1f, AvailableMemoryMinPct = 50f, DiskBusyP90Pct = 90f };

            Assert.False(s.HasCpuSignal);
            Assert.Equal(GHBackgroundVerdict.Unknown, ClassifyAlone(s));
        }

        [Fact]
        public void Classify_NoCpuSignal_MemoryRuleFires_UsesMemoryVerdict()
        {
            GHBackgroundSummary elevated = new GHBackgroundSummary { Samples = 60, Coverage = 1f, AvailableMemoryMinPct = 8f };
            GHBackgroundSummary busy = new GHBackgroundSummary { Samples = 60, Coverage = 1f, LowMemory = true };

            Assert.Equal(GHBackgroundVerdict.Elevated, ClassifyAlone(elevated));
            Assert.Equal(GHBackgroundVerdict.Busy, ClassifyAlone(busy));
        }

        [Fact]
        public void Classify_Busy_ReasonNamesBusyFactsAndTwoSuspects()
        {
            GHBackgroundSummary s = QuietSummary();
            s.OtherCpuP90Pct = 31f;
            GHProcessLoad[] processes = Processes(
                new GHProcessLoad("devenv", 5f, float.NaN),
                new GHProcessLoad("vmmemWSL", 22f, float.NaN),
                new GHProcessLoad("OneDrive", 3f, float.NaN));

            string reason;
            GHBackgroundVerdict v = GHBackgroundLoad.Classify(s, processes, float.NaN, out reason);

            Assert.Equal(GHBackgroundVerdict.Busy, v);
            Assert.Equal("background load: other CPU P90 31 %; wsl-vm (vmmemWSL 22 %), build-tools (devenv 5 %)", reason);
        }

        [Fact]
        public void Classify_GpuHeavySuspect_IsNamedWithItsGpuShare()
        {
            string reason;
            GHBackgroundVerdict v = GHBackgroundLoad.Classify(QuietSummary(),
                Processes(new GHProcessLoad("Game", 1f, 35f)), 35f, out reason);

            Assert.Equal(GHBackgroundVerdict.Busy, v);
            Assert.Equal("background load: other GPU 35 %; other (Game GPU 35 %)", reason);
        }

        [Fact]
        public void Classify_MeasurementToolsNeverNamedInReason()
        {
            GHBackgroundSummary s = QuietSummary();
            s.OtherCpuP90Pct = 40f;
            GHProcessLoad[] processes = Processes(
                new GHProcessLoad("PresentMon", 40f, float.NaN),
                new GHProcessLoad("typeperf#1", 30f, float.NaN),
                new GHProcessLoad("powershell", 20f, float.NaN),
                new GHProcessLoad("devenv", 5f, float.NaN));

            string reason;
            GHBackgroundVerdict v = GHBackgroundLoad.Classify(s, processes, float.NaN, out reason);

            Assert.Equal(GHBackgroundVerdict.Busy, v);
            Assert.Contains("build-tools (devenv 5 %)", reason);
            Assert.DoesNotContain("PresentMon", reason);
            Assert.DoesNotContain("typeperf", reason);
            Assert.DoesNotContain("powershell", reason);
            Assert.DoesNotContain(GHBackgroundLoad.CategoryMeasurement, reason);
        }

        /* The compositor's GPU use follows this app's own frames: a Windows laptop run with
           dwm as the whole other GPU share */
        [Fact]
        public void Classify_CompositorGpu_IsNotOtherLoadNorASuspect()
        {
            string reason;
            GHBackgroundVerdict v = GHBackgroundLoad.Classify(QuietSummary(),
                Processes(new GHProcessLoad("dwm", 0.429f, 16.135f), new GHProcessLoad("svchost", 0.491f, 0f)),
                16.135f, out reason);

            Assert.Equal(GHBackgroundVerdict.Quiet, v);
            Assert.Null(reason);
        }

        [Fact]
        public void Classify_CompositorExcluded_OtherAppGpuStillCounts()
        {
            GHBackgroundSummary s = QuietSummary();
            s.OtherCpuP90Pct = 12f;
            string reason;
            GHBackgroundVerdict v = GHBackgroundLoad.Classify(s,
                Processes(new GHProcessLoad("dwm.exe", 1f, 16f), new GHProcessLoad("vlc", 1f, 14f)),
                30f, out reason);

            Assert.Equal(GHBackgroundVerdict.Elevated, v);
            Assert.Contains("other GPU 14 %", reason);
            Assert.Contains("other (vlc GPU 14 %)", reason);
            Assert.DoesNotContain("dwm", reason);
            Assert.DoesNotContain(GHBackgroundLoad.CategoryCompositor, reason);
        }

        [Theory]
        [InlineData(16.135f, 16.135f, 0f)]
        [InlineData(30f, 16f, 14f)]
        [InlineData(10f, 16f, 0f)]      /* compositor spread over adapters: clamped */
        [InlineData(float.NaN, 16f, float.NaN)]
        public void OtherGpuWithoutCompositor_SubtractsDwm(float otherGpu, float dwmGpu, float expected)
        {
            float result = GHBackgroundLoad.OtherGpuWithoutCompositor(otherGpu,
                Processes(new GHProcessLoad("dwm", 0.5f, dwmGpu), new GHProcessLoad("chrome", 0.1f, 0.001f)));

            if (float.IsNaN(expected))
                Assert.True(float.IsNaN(result));
            else
                Assert.Equal(expected, result, 3);
        }

        [Fact]
        public void OtherGpuWithoutCompositor_RecognizesStoredOtherCategoryByName()
        {
            /* Records written before the compositor category store dwm as "other" */
            GHProcessLoad stored = new GHProcessLoad("dwm", 0.5f, 16f);
            stored.Category = GHBackgroundLoad.CategoryOther;

            Assert.Equal(4f, GHBackgroundLoad.OtherGpuWithoutCompositor(20f, Processes(stored)), 3);
            Assert.Equal(20f, GHBackgroundLoad.OtherGpuWithoutCompositor(20f, null), 3);
        }

        [Fact]
        public void CompositorAndMeasurement_AreIgnoredCategories()
        {
            Assert.Equal(GHBackgroundLoad.CategoryCompositor, GHBackgroundLoad.CategoryOf("DWM.exe"));
            Assert.True(GHBackgroundLoad.IsIgnoredCategory(GHBackgroundLoad.CategoryCompositor));
            Assert.True(GHBackgroundLoad.IsIgnoredCategory(GHBackgroundLoad.CategoryMeasurement));
            Assert.False(GHBackgroundLoad.IsIgnoredCategory(GHBackgroundLoad.CategoryOther));
            Assert.False(GHBackgroundLoad.IsIgnoredCategory(GHBackgroundLoad.CategoryAntivirus));

            List<GHBackgroundActivity> activities = GHBackgroundLoad.BuildActivities(
                Processes(new GHProcessLoad("dwm", 5f, 16f), new GHProcessLoad("MsMpEng", 3f, float.NaN)));
            Assert.Single(activities);
            Assert.Equal(GHBackgroundLoad.CategoryAntivirus, activities[0].Category);
        }

        [Fact]
        public void Classify_ManyFactsAndLongNames_ReasonWithinMaxLength()
        {
            GHBackgroundSummary s = QuietSummary();
            s.OtherCpuP90Pct = 90f;
            s.OtherCpuSpikeShare = 0.5f;
            s.LowMemory = true;
            s.AvailableMemoryMinPct = 1f;
            s.HardFaultsP90PerSec = 5000f;
            GHProcessLoad[] processes = Processes(
                new GHProcessLoad("ServiceHub.RoslynCodeAnalysisService", 30f, float.NaN),
                new GHProcessLoad(new string('x', 120), 25f, float.NaN));

            string reason;
            GHBackgroundVerdict v = GHBackgroundLoad.Classify(s, processes, 80f, out reason);

            Assert.Equal(GHBackgroundVerdict.Busy, v);
            Assert.StartsWith(GHBackgroundLoad.ReasonPrefix, reason);
            Assert.True(reason.Length <= GHBackgroundLoad.MaxReasonLength, "reason length " + reason.Length + ": " + reason);
        }

        [Fact]
        public void Classify_Quiet_ReasonIsNull()
        {
            string reason;
            GHBackgroundVerdict v = GHBackgroundLoad.Classify(QuietSummary(), null, 1f, out reason);

            Assert.Equal(GHBackgroundVerdict.Quiet, v);
            Assert.Null(reason);
        }

        [Theory]
        [InlineData("MsMpEng#2", "antivirus")]
        [InlineData("tiworker.exe", "windows-update")]
        [InlineData("TiWorker.EXE#1", "windows-update")]
        [InlineData("vmmemWSL", "wsl-vm")]
        [InlineData("cl", "build-tools")]
        [InlineData("ServiceHub.Host.dotnet.x64", "build-tools")]
        [InlineData("GoogleDriveFS", "sync")]
        [InlineData("DiagTrack", "telemetry")]
        [InlineData("SearchIndexer", "indexer")]
        [InlineData("PresentMon", "measurement")]
        [InlineData("clx", "other")]
        [InlineData("svchost#3", "other")]
        [InlineData("MsMpEng#x", "other")]
        [InlineData("devenv#", "other")]
        [InlineData("", "other")]
        [InlineData(null, "other")]
        public void CategoryOf_StripsInstanceSuffixAndExe(string name, string expected)
        {
            Assert.Equal(expected, GHBackgroundLoad.CategoryOf(name));
        }

        [Fact]
        public void BuildActivities_GroupsKnownCategories_SkipsOtherAndMeasurement()
        {
            GHProcessLoad[] processes = Processes(
                new GHProcessLoad("TiWorker", 3f, float.NaN),
                new GHProcessLoad("devenv", 4f, float.NaN),
                new GHProcessLoad("chrome", 20f, float.NaN),
                new GHProcessLoad("PresentMon", 10f, float.NaN),
                new GHProcessLoad("MoUsoCoreWorker", 1.5f, float.NaN));

            List<GHBackgroundActivity> activities = GHBackgroundLoad.BuildActivities(processes);

            Assert.Equal(2, activities.Count);
            Assert.Equal("windows-update", activities[0].Category);
            Assert.Equal(4.5f, activities[0].CpuPct, 4);
            Assert.Equal(2, activities[0].Processes.Count);
            Assert.Equal("TiWorker", activities[0].Processes[0]);
            Assert.Equal("MoUsoCoreWorker", activities[0].Processes[1]);
            Assert.Equal("build-tools", activities[1].Category);
            Assert.Equal(4f, activities[1].CpuPct, 4);
        }

        [Fact]
        public void BuildActivities_NullProcesses_IsEmpty()
        {
            Assert.Empty(GHBackgroundLoad.BuildActivities(null));
        }

        [Fact]
        public void VerdictName_RoundTripsThroughParseVerdict()
        {
            foreach (GHBackgroundVerdict v in new[] { GHBackgroundVerdict.Unknown, GHBackgroundVerdict.Quiet,
                                                      GHBackgroundVerdict.Elevated, GHBackgroundVerdict.Busy })
                Assert.Equal(v, GHBackgroundLoad.ParseVerdict(GHBackgroundLoad.VerdictName(v)));
            Assert.Equal("quiet", GHBackgroundLoad.VerdictName(GHBackgroundVerdict.Quiet));
            Assert.Equal(GHBackgroundVerdict.Unknown, GHBackgroundLoad.ParseVerdict(null));
        }

        private static GHSystemLoadSample[] BusyThenQuiet()
        {
            GHSystemLoadSample[] samples = new GHSystemLoadSample[15];
            for (int i = 0; i < 15; i++)
                samples[i] = Sample(i, i < 10 ? 40f : 2f, 5f);
            return samples;
        }

        [Fact]
        public void IsQuiet_LastFiveSecondsQuiet_IsTrueDespiteEarlierLoad()
        {
            float otherMean, diskMean;
            bool quiet = GHBackgroundLoad.IsQuiet(BusyThenQuiet(), 15, Seconds(14), out otherMean, out diskMean);

            Assert.True(quiet);
            Assert.Equal(2f, otherMean, 4);
            Assert.Equal(5f, diskMean, 4);
        }

        [Fact]
        public void IsQuiet_LastFiveSecondsBusy_IsFalse()
        {
            float otherMean, diskMean;
            bool quiet = GHBackgroundLoad.IsQuiet(BusyThenQuiet(), 15, Seconds(9), out otherMean, out diskMean);

            Assert.False(quiet);
            Assert.Equal(40f, otherMean, 4);
        }

        [Fact]
        public void IsQuiet_OtherCpuMeanAtThreshold_IsFalse()
        {
            GHSystemLoadSample[] samples = new GHSystemLoadSample[5];
            for (int i = 0; i < 5; i++)
                samples[i] = Sample(i, 10f, 0f);

            float otherMean, diskMean;
            Assert.False(GHBackgroundLoad.IsQuiet(samples, 5, Seconds(4), out otherMean, out diskMean));
        }

        [Fact]
        public void IsQuiet_DiskBusy_IsFalse_DiskUnreported_IsIgnored()
        {
            GHSystemLoadSample[] busyDisk = new GHSystemLoadSample[5];
            GHSystemLoadSample[] noDisk = new GHSystemLoadSample[5];
            for (int i = 0; i < 5; i++)
            {
                busyDisk[i] = Sample(i, 2f, 60f);
                noDisk[i] = Sample(i, 2f, float.NaN);
            }

            float otherMean, diskMean;
            Assert.False(GHBackgroundLoad.IsQuiet(busyDisk, 5, Seconds(4), out otherMean, out diskMean));
            Assert.True(GHBackgroundLoad.IsQuiet(noDisk, 5, Seconds(4), out otherMean, out diskMean));
            Assert.True(float.IsNaN(diskMean));
        }

        [Fact]
        public void IsQuiet_NoCpuSignal_IsFalseWithNaNMean()
        {
            GHSystemLoadSample[] samples = new GHSystemLoadSample[5];
            for (int i = 0; i < 5; i++)
                samples[i] = Sample(i, float.NaN, 1f);

            float otherMean, diskMean;
            Assert.False(GHBackgroundLoad.IsQuiet(samples, 5, Seconds(4), out otherMean, out diskMean));
            Assert.True(float.IsNaN(otherMean));
        }

        [Fact]
        public void IsQuiet_TooFewSamples_IsFalse()
        {
            GHSystemLoadSample[] samples = { Sample(3, 1f, 0f), Sample(4, 1f, 0f) };

            float otherMean, diskMean;
            Assert.False(GHBackgroundLoad.IsQuiet(samples, 2, Seconds(4), out otherMean, out diskMean));
            Assert.Equal(1f, otherMean, 4);
        }
    }
}
