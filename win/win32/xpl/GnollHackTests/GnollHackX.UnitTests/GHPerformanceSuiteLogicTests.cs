using System.Collections.Generic;
using GnollHackX.Performance;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers GHPerformanceSuiteLogic: the device key and the per-device baseline key,
       the sentence naming a baseline that differs from a suite's group only in its
       refresh rate or thermal settings, the thermal gate helpers and key segment, and
       the default arm label. */
    public class GHPerformanceSuiteLogicTests
    {
        private const string Key60 = "playback|0123abcd|100|shared|MapFPS60|60|m2";
        private const string Key120 = "playback|0123abcd|100|shared|MapFPS60|120|m2";
        private const string Device = "Android|Pixel 8|Tensor G3";

        [Fact]
        public void DeviceKey_UsesSocWhenNoCpu()
        {
            Dictionary<string, string> fp = new Dictionary<string, string>
            {
                { "os.platform", "Android" },
                { "hardware.deviceModel", "Pixel 8" },
                { "hardware.soc", "Tensor G3" }
            };

            Assert.Equal("Android|Pixel 8|Tensor G3", GHPerformanceSuiteLogic.DeviceKey(fp, "Other", "Other model"));
        }

        [Fact]
        public void DeviceKey_PrefersCpuOverSoc()
        {
            Dictionary<string, string> fp = new Dictionary<string, string>
            {
                { "os.platform", "Windows" },
                { "hardware.deviceModel", "Desktop" },
                { "hardware.cpu", "Test CPU" },
                { "hardware.soc", "Test SoC" }
            };

            Assert.Equal("Windows|Desktop|Test CPU", GHPerformanceSuiteLogic.DeviceKey(fp, null, null));
        }

        [Fact]
        public void DeviceKey_FallsBackToPlatformAndModel()
        {
            Assert.Equal("Android|Pixel 8|", GHPerformanceSuiteLogic.DeviceKey(null, "Android", "Pixel 8"));

            Dictionary<string, string> fp = new Dictionary<string, string> { { "hardware.cpu", "Test CPU" } };
            Assert.Equal("Windows|Desktop|Test CPU", GHPerformanceSuiteLogic.DeviceKey(fp, "Windows", "Desktop"));
        }

        [Fact]
        public void BaselineKey_Format()
        {
            Assert.Equal(Key60 + "|device=" + Device, GHPerformanceSuiteLogic.BaselineKey(Key60, Device));
        }

        [Fact]
        public void DescribeKeyMismatch_NamesRefresh()
        {
            string key = GHPerformanceSuiteLogic.BaselineKey(Key60, Device);
            List<string> baselines = new List<string> { GHPerformanceSuiteLogic.BaselineKey(Key120, Device) };

            Assert.Equal("a baseline exists for the same replay at 120 Hz (this suite: 60 Hz)",
                GHPerformanceSuiteLogic.DescribeKeyMismatch(key, baselines));
        }

        [Fact]
        public void DescribeKeyMismatch_PlainBaselineKey_NamesRefresh()
        {
            string key = GHPerformanceSuiteLogic.BaselineKey(Key60, Device);
            List<string> baselines = new List<string>
            {
                Key120,
                "playback|0123abcd|100|shared|MapFPS60|90|m2"
            };

            Assert.Equal("a baseline exists for the same replay at 90, 120 Hz (this suite: 60 Hz)",
                GHPerformanceSuiteLogic.DescribeKeyMismatch(key, baselines));
        }

        [Fact]
        public void DescribeKeyMismatch_OtherScenario_Null()
        {
            string key = GHPerformanceSuiteLogic.BaselineKey(Key60, Device);
            List<string> baselines = new List<string>
            {
                GHPerformanceSuiteLogic.BaselineKey("idle|0123abcd|100|shared|MapFPS60|120|m2", Device)
            };

            Assert.Null(GHPerformanceSuiteLogic.DescribeKeyMismatch(key, baselines));
        }

        [Fact]
        public void DescribeKeyMismatch_OtherDevice_Null()
        {
            string key = GHPerformanceSuiteLogic.BaselineKey(Key60, Device);
            List<string> baselines = new List<string>
            {
                GHPerformanceSuiteLogic.BaselineKey(Key120, "Android|Galaxy S24|Snapdragon")
            };

            Assert.Null(GHPerformanceSuiteLogic.DescribeKeyMismatch(key, baselines));
        }

        [Fact]
        public void DescribeKeyMismatch_SameRefreshOrOtherMetricsVersion_Null()
        {
            string key = GHPerformanceSuiteLogic.BaselineKey(Key60, Device);
            List<string> baselines = new List<string>
            {
                GHPerformanceSuiteLogic.BaselineKey(Key60, Device),
                "playback|0123abcd|100|shared|MapFPS60|120|m1",
                "playback|0123abcd|100|shared|MapFPS60|120"
            };

            Assert.Null(GHPerformanceSuiteLogic.DescribeKeyMismatch(key, baselines));
        }

        [Fact]
        public void DescribeKeyMismatch_NamesThermal()
        {
            string key = GHPerformanceSuiteLogic.BaselineKey(Key60 + "|tg=light,300", Device);
            List<string> baselines = new List<string> { GHPerformanceSuiteLogic.BaselineKey(Key60, Device) };

            Assert.Equal("a baseline exists for the same replay with thermal gate same as start, wait 300 s (this suite: Light or better, wait 300 s)",
                GHPerformanceSuiteLogic.DescribeKeyMismatch(key, baselines));
        }

        [Fact]
        public void DescribeKeyMismatch_ThermalOtherDevice_Null()
        {
            string key = GHPerformanceSuiteLogic.BaselineKey(Key60 + "|tg=light,300", Device);
            List<string> baselines = new List<string>
            {
                GHPerformanceSuiteLogic.BaselineKey(Key60, "Android|Galaxy S24|Snapdragon")
            };

            Assert.Null(GHPerformanceSuiteLogic.DescribeKeyMismatch(key, baselines));
        }

        [Fact]
        public void DescribeKeyMismatch_ThermalAndRefresh_Separate()
        {
            string key = GHPerformanceSuiteLogic.BaselineKey(Key60 + "|tg=light,300", Device);
            List<string> baselines = new List<string>
            {
                GHPerformanceSuiteLogic.BaselineKey(Key120 + "|tg=light,300", Device),
                GHPerformanceSuiteLogic.BaselineKey(Key60, Device)
            };

            Assert.Equal("a baseline exists for the same replay at 120 Hz (this suite: 60 Hz); "
                + "a baseline exists for the same replay with thermal gate same as start, wait 300 s (this suite: Light or better, wait 300 s)",
                GHPerformanceSuiteLogic.DescribeKeyMismatch(key, baselines));
        }

        [Fact]
        public void NormalizeThermalGate_Values()
        {
            Assert.Equal("start", GHPerformanceSuiteLogic.NormalizeThermalGate(null));
            Assert.Equal("start", GHPerformanceSuiteLogic.NormalizeThermalGate(""));
            Assert.Equal("start", GHPerformanceSuiteLogic.NormalizeThermalGate(" Start "));
            Assert.Equal("light", GHPerformanceSuiteLogic.NormalizeThermalGate("LIGHT"));
            Assert.Null(GHPerformanceSuiteLogic.NormalizeThermalGate("nominal"));
        }

        [Fact]
        public void ThermalGateDisplayName_Values()
        {
            Assert.Equal("Light or better", GHPerformanceSuiteLogic.ThermalGateDisplayName("light"));
            Assert.Equal("same as start", GHPerformanceSuiteLogic.ThermalGateDisplayName("start"));
            Assert.Equal("same as start", GHPerformanceSuiteLogic.ThermalGateDisplayName(null));
        }

        [Fact]
        public void ThermalGatePhase_Format()
        {
            Assert.Equal("cooling Moderate > Light 45/300 s", GHPerformanceSuiteLogic.ThermalGatePhase("Moderate", "Light", 45, 300));
        }

        [Fact]
        public void ThermalKeySegment_Values()
        {
            Assert.Equal("", GHPerformanceSuiteLogic.ThermalKeySegment("start", 300));
            Assert.Equal("", GHPerformanceSuiteLogic.ThermalKeySegment(null, 300));
            Assert.Equal("|tg=light,300", GHPerformanceSuiteLogic.ThermalKeySegment("light", 300));
            Assert.Equal("|tg=start,120", GHPerformanceSuiteLogic.ThermalKeySegment("start", 120));
        }

        [Fact]
        public void DefaultArmLabel_WithId()
        {
            Assert.Equal("4.2.0 abcdef0 m9f8e7d",
                GHPerformanceSuiteLogic.DefaultArmLabel("4.2.0", "abcdef0123456789", "9f8e7d6c5b4a"));
        }

        [Fact]
        public void DefaultArmLabel_WithoutId()
        {
            Assert.Equal("4.2.0 abcdef0", GHPerformanceSuiteLogic.DefaultArmLabel("4.2.0", "abcdef0123456789", null));
            Assert.Equal("4.2.0 abcdef0", GHPerformanceSuiteLogic.DefaultArmLabel("4.2.0", "abcdef0123456789", ""));
        }

        [Fact]
        public void DefaultArmLabel_NoCommit()
        {
            Assert.Equal("4.2.0", GHPerformanceSuiteLogic.DefaultArmLabel("4.2.0", null, null));
            Assert.Equal("4.2.0 m9f8e7d", GHPerformanceSuiteLogic.DefaultArmLabel("4.2.0", "", "9f8e7d6c5b4a"));
            Assert.Equal("", GHPerformanceSuiteLogic.DefaultArmLabel(null, null, null));
        }
    }
}
