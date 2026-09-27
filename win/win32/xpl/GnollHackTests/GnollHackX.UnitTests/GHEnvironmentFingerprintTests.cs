using System.Collections.Generic;
using System.Text;
using GnollHackX.Performance;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers GHEnvironmentFingerprint: the category of a key, every change kind of Diff
       including the lazy-component and legacy rules, the attribution label, the
       one-setting check, pooling by common values, the legacy field mapping and its key
       parity with a new capture, the short hash and the width of the report lines. */
    public class GHEnvironmentFingerprintTests
    {
        private static void AssertNoLineExceedsMaxWidth(string report)
        {
            string[] lines = report.Split('\n');
            foreach (string line in lines)
                Assert.True(line.Length <= GHPerformanceTextReport.MaxLineWidth,
                    "line exceeds MaxLineWidth (" + line.Length + "): " + line);
        }

        private static Dictionary<string, string> Versioned(params string[] keyValues)
        {
            Dictionary<string, string> d = Legacy(keyValues);
            d[GHEnvironmentFingerprint.MetaFingerprintVersionKey] = GHEnvironmentFingerprint.FingerprintVersion;
            return d;
        }

        private static Dictionary<string, string> Legacy(params string[] keyValues)
        {
            Dictionary<string, string> d = new Dictionary<string, string>();
            for (int i = 0; i + 1 < keyValues.Length; i += 2)
                d[keyValues[i]] = keyValues[i + 1];
            return d;
        }

        private static GHFingerprintChange Change(string key, GHFingerprintChangeKind kind, string before, string after)
        {
            return new GHFingerprintChange
            {
                Key = key,
                Category = GHEnvironmentFingerprint.CategoryOf(key),
                Kind = kind,
                Before = before,
                After = after
            };
        }

        private static Dictionary<string, object> SampleToggles()
        {
            return new Dictionary<string, object>
            {
                { "useTileBatching", true },
                { "runtimeEffects", false },
                { "primaryGPUCacheLimit", 256L },
                { "secondaryGPUCacheLimit", 64 },
                { "screenLogging", null }
            };
        }

        private static Dictionary<string, string> SampleLegacy()
        {
            return GHEnvironmentFingerprint.FromLegacyFields("4.1.3", "abc123", "Release",
                ".NET 10.0.0", "net10.0-windows", "10.0.1", "3.119.0", "2.03.07",
                "Windows", "Windows 10.0.26200", "Contoso Laptop", "Fps60", "OpenGL", SampleToggles());
        }

        [Theory]
        [InlineData("os.build", "os")]
        [InlineData("component.Microsoft.Maui.Controls", "component")]
        [InlineData("meta.capturedUtc", "meta")]
        [InlineData("nodot", "nodot")]
        [InlineData("", "")]
        public void CategoryOf_PrefixBeforeFirstDot(string key, string expected)
        {
            Assert.Equal(expected, GHEnvironmentFingerprint.CategoryOf(key));
        }

        [Fact]
        public void Diff_DifferentValue_IsChanged()
        {
            List<GHFingerprintChange> changes = GHEnvironmentFingerprint.Diff(
                Versioned("os.build", "26200.6584"), Versioned("os.build", "26200.6725"));

            GHFingerprintChange c = Assert.Single(changes);
            Assert.Equal("os.build", c.Key);
            Assert.Equal("os", c.Category);
            Assert.Equal(GHFingerprintChangeKind.Changed, c.Kind);
            Assert.Equal("26200.6584", c.Before);
            Assert.Equal("26200.6725", c.After);
        }

        [Fact]
        public void Diff_EqualFingerprints_IsEmpty()
        {
            Assert.Empty(GHEnvironmentFingerprint.Diff(
                Versioned("os.build", "1", "code.appVersion", "4.1"), Versioned("code.appVersion", "4.1", "os.build", "1")));
        }

        [Fact]
        public void Diff_OneSidedKeysBothVersioned_AreAddedAndRemoved()
        {
            List<GHFingerprintChange> changes = GHEnvironmentFingerprint.Diff(
                Versioned("os.pendingReboot", "true"), Versioned("driver.gpu0.version", "32.0.101"));

            Assert.Equal(2, changes.Count);
            Assert.Equal("os.pendingReboot", changes[0].Key);
            Assert.Equal(GHFingerprintChangeKind.Removed, changes[0].Kind);
            Assert.Null(changes[0].After);
            Assert.Equal("driver.gpu0.version", changes[1].Key);
            Assert.Equal(GHFingerprintChangeKind.Added, changes[1].Kind);
            Assert.Null(changes[1].Before);
        }

        [Fact]
        public void Diff_OneSidedKeyWithLegacySide_IsUnknown()
        {
            List<GHFingerprintChange> changes = GHEnvironmentFingerprint.Diff(
                Legacy("code.appVersion", "4.1"), Versioned("code.appVersion", "4.1", "os.build", "26200.6584"));

            GHFingerprintChange c = Assert.Single(changes);
            Assert.Equal("os.build", c.Key);
            Assert.Equal(GHFingerprintChangeKind.Unknown, c.Kind);
        }

        [Fact]
        public void Diff_OneSidedComponent_IsNotCompared_EvenWhenVersioned()
        {
            List<GHFingerprintChange> changes = GHEnvironmentFingerprint.Diff(
                Versioned("component.SkiaSharp", "3.119.0"),
                Versioned("component.SkiaSharp", "3.119.0", "component.Newtonsoft.Json", "13.0.3"));

            GHFingerprintChange c = Assert.Single(changes);
            Assert.Equal(GHFingerprintChangeKind.NotCompared, c.Kind);
            Assert.Equal("component", c.Category);
        }

        [Fact]
        public void Diff_ComponentOnBothSidesWithDifferentValues_IsChanged()
        {
            List<GHFingerprintChange> changes = GHEnvironmentFingerprint.Diff(
                Versioned("component.SkiaSharp", "3.119.0"), Versioned("component.SkiaSharp", "3.119.1"));

            Assert.Equal(GHFingerprintChangeKind.Changed, Assert.Single(changes).Kind);
        }

        [Fact]
        public void Diff_MetaKeysAreNeverDiffed()
        {
            Dictionary<string, string> a = Versioned(GHEnvironmentFingerprint.MetaCapturedUtcKey, "2026-09-27T10:00:00Z");
            Dictionary<string, string> b = Versioned("meta.extra", "x");

            Assert.Empty(GHEnvironmentFingerprint.Diff(a, b));
        }

        [Fact]
        public void Diff_SortedByCategoryOrderThenKey()
        {
            Dictionary<string, string> a = Versioned("settings.b", "1", "code.x", "1", "os.a", "1",
                "hardware.cpu", "1", "settings.a", "1", "custom.z", "1", "toolchain.sdk", "1");
            Dictionary<string, string> b = Versioned("settings.b", "2", "code.x", "2", "os.a", "2",
                "hardware.cpu", "2", "settings.a", "2", "custom.z", "2", "toolchain.sdk", "2");

            List<GHFingerprintChange> changes = GHEnvironmentFingerprint.Diff(a, b);

            string[] expected = { "code.x", "toolchain.sdk", "os.a", "hardware.cpu", "settings.a", "settings.b", "custom.z" };
            Assert.Equal(expected.Length, changes.Count);
            for (int i = 0; i < expected.Length; i++)
                Assert.Equal(expected[i], changes[i].Key);
        }

        [Fact]
        public void Diff_NullSide_IsEmptyAndUnversioned()
        {
            List<GHFingerprintChange> changes = GHEnvironmentFingerprint.Diff(null, Versioned("os.build", "1"));

            Assert.Equal(GHFingerprintChangeKind.Unknown, Assert.Single(changes).Kind);
        }

        [Fact]
        public void AttributionLabel_NoCountedEntries_IsNone()
        {
            List<GHFingerprintChange> changes = new List<GHFingerprintChange>
            {
                Change("component.Foo", GHFingerprintChangeKind.NotCompared, null, "1"),
                Change("os.build", GHFingerprintChangeKind.Unknown, null, "1")
            };

            Assert.Equal("none", GHEnvironmentFingerprint.AttributionLabel(changes));
            Assert.Equal("none", GHEnvironmentFingerprint.AttributionLabel(new List<GHFingerprintChange>()));
            Assert.Equal("none", GHEnvironmentFingerprint.AttributionLabel(null));
        }

        [Fact]
        public void AttributionLabel_OnlyCode_IsCode()
        {
            List<GHFingerprintChange> changes = new List<GHFingerprintChange>
            {
                Change("code.gitCommit", GHFingerprintChangeKind.Changed, "a", "b"),
                Change("code.appVersion", GHFingerprintChangeKind.Changed, "1", "2")
            };

            Assert.Equal("code", GHEnvironmentFingerprint.AttributionLabel(changes));
        }

        [Fact]
        public void AttributionLabel_NoCode_IsEnvironmentInCategoryOrder()
        {
            List<GHFingerprintChange> changes = new List<GHFingerprintChange>
            {
                Change("driver.gpu0.version", GHFingerprintChangeKind.Changed, "1", "2"),
                Change("os.build", GHFingerprintChangeKind.Added, null, "2"),
                Change("driver.gpu0.date", GHFingerprintChangeKind.Removed, "x", null)
            };

            Assert.Equal("environment: os, driver", GHEnvironmentFingerprint.AttributionLabel(changes));
        }

        [Fact]
        public void AttributionLabel_CodeAndOthers_IsConfounded()
        {
            List<GHFingerprintChange> changes = new List<GHFingerprintChange>
            {
                Change("settings.useTileBatching", GHFingerprintChangeKind.Changed, "true", "false"),
                Change("code.gitCommit", GHFingerprintChangeKind.Changed, "a", "b"),
                Change("component.SkiaSharp", GHFingerprintChangeKind.NotCompared, null, "3")
            };

            Assert.Equal("confounded: code, settings", GHEnvironmentFingerprint.AttributionLabel(changes));
        }

        [Fact]
        public void SettingsViolation_OneChangedSetting_IsFalse_TwoAreTrue()
        {
            List<GHFingerprintChange> one = new List<GHFingerprintChange>
            {
                Change("settings.useTileBatching", GHFingerprintChangeKind.Changed, "true", "false"),
                Change("settings.powerPlan", GHFingerprintChangeKind.Added, null, "Balanced"),
                Change("os.build", GHFingerprintChangeKind.Changed, "1", "2")
            };
            List<GHFingerprintChange> two = new List<GHFingerprintChange>(one)
            {
                Change("settings.runtimeEffects", GHFingerprintChangeKind.Changed, "true", "false")
            };

            Assert.False(GHEnvironmentFingerprint.SettingsViolation(one));
            Assert.True(GHEnvironmentFingerprint.SettingsViolation(two));
            Assert.False(GHEnvironmentFingerprint.SettingsViolation(null));
        }

        [Fact]
        public void CommonValues_AgreeingKept_DisagreeingMixed_MissingDropped()
        {
            List<IDictionary<string, string>> inputs = new List<IDictionary<string, string>>
            {
                Versioned("os.build", "1", "hardware.cpu", "X", "settings.a", "true"),
                null,
                Versioned("os.build", "2", "hardware.cpu", "X"),
                Versioned("os.build", "1", "hardware.cpu", "X", "settings.a", "true")
            };

            Dictionary<string, string> common = GHEnvironmentFingerprint.CommonValues(inputs);

            Assert.Equal(3, common.Count);
            Assert.Equal(GHEnvironmentFingerprint.MixedValue, common["os.build"]);
            Assert.Equal("X", common["hardware.cpu"]);
            Assert.Equal(GHEnvironmentFingerprint.FingerprintVersion, common[GHEnvironmentFingerprint.MetaFingerprintVersionKey]);
            Assert.False(common.ContainsKey("settings.a"));
        }

        [Fact]
        public void FromLegacyFields_MapsEveryFieldToItsContractKey()
        {
            Dictionary<string, string> d = SampleLegacy();

            Assert.Equal("4.1.3", d["code.appVersion"]);
            Assert.Equal("abc123", d["code.gitCommit"]);
            Assert.Equal("Release", d["code.buildConfiguration"]);
            Assert.Equal(".NET 10.0.0", d["toolchain.runtime"]);
            Assert.Equal("net10.0-windows", d["toolchain.framework"]);
            Assert.Equal("10.0.1", d["component.Microsoft.Maui.Controls"]);
            Assert.Equal("3.119.0", d["component.SkiaSharp"]);
            Assert.Equal("2.03.07", d["component.native.fmod"]);
            Assert.Equal("Windows", d["os.platform"]);
            Assert.Equal("Windows 10.0.26200", d["os.version"]);
            Assert.Equal("Contoso Laptop", d["hardware.deviceModel"]);
            Assert.Equal("Fps60", d["settings.mapRefreshRate"]);
            Assert.Equal("OpenGL", d["settings.gpuBackend"]);
            Assert.Equal("true", d["settings.useTileBatching"]);
            Assert.Equal("false", d["settings.runtimeEffects"]);
            Assert.Equal("256", d["settings.primaryGPUCacheLimit"]);
            Assert.Equal("64", d["settings.secondaryGPUCacheLimit"]);
            Assert.Equal(17, d.Count);
        }

        [Fact]
        public void FromLegacyFields_SkipsNullAndEmpty_NeverAddsVersion()
        {
            Dictionary<string, string> d = GHEnvironmentFingerprint.FromLegacyFields("4.1.3", null, "",
                null, null, null, null, null, "Android", null, null, null, null, null);

            Assert.Equal(2, d.Count);
            Assert.Equal("4.1.3", d[GHEnvironmentFingerprint.CodeAppVersionKey]);
            Assert.Equal("Android", d[GHEnvironmentFingerprint.OsPlatformKey]);
            Assert.False(d.ContainsKey(GHEnvironmentFingerprint.MetaFingerprintVersionKey));
            Assert.False(SampleLegacy().ContainsKey("settings.screenLogging"));
        }

        [Fact]
        public void FormatValue_BooleansLowerCase_NumbersInvariant()
        {
            Assert.Equal("true", GHEnvironmentFingerprint.FormatValue(true));
            Assert.Equal("false", GHEnvironmentFingerprint.FormatValue(false));
            Assert.Equal("1.5", GHEnvironmentFingerprint.FormatValue(1.5));
            Assert.Equal("-3", GHEnvironmentFingerprint.FormatValue(-3));
            Assert.Equal("Fps60", GHEnvironmentFingerprint.FormatValue("Fps60"));
            Assert.Null(GHEnvironmentFingerprint.FormatValue(null));
        }

        /* A new capture built from the shared key constants and FormatValue, for the same
           values as SampleLegacy, plus keys only a new capture has. */
        private static Dictionary<string, string> NewCaptureOfSameValues()
        {
            Dictionary<string, string> d = new Dictionary<string, string>();
            d[GHEnvironmentFingerprint.MetaFingerprintVersionKey] = GHEnvironmentFingerprint.FingerprintVersion;
            d[GHEnvironmentFingerprint.MetaCapturedUtcKey] = "2026-09-27T10:00:00Z";
            d[GHEnvironmentFingerprint.CodeAppVersionKey] = "4.1.3";
            d[GHEnvironmentFingerprint.CodeGitCommitKey] = "abc123";
            d[GHEnvironmentFingerprint.CodeBuildConfigurationKey] = "Release";
            d[GHEnvironmentFingerprint.ToolchainRuntimeKey] = ".NET 10.0.0";
            d[GHEnvironmentFingerprint.ToolchainFrameworkKey] = "net10.0-windows";
            d[GHEnvironmentFingerprint.ComponentMauiControlsKey] = "10.0.1";
            d[GHEnvironmentFingerprint.ComponentSkiaSharpKey] = "3.119.0";
            d[GHEnvironmentFingerprint.ComponentFmodKey] = "2.03.07";
            d[GHEnvironmentFingerprint.OsPlatformKey] = "Windows";
            d[GHEnvironmentFingerprint.OsVersionKey] = "Windows 10.0.26200";
            d[GHEnvironmentFingerprint.HardwareDeviceModelKey] = "Contoso Laptop";
            d[GHEnvironmentFingerprint.SettingsMapRefreshRateKey] = "Fps60";
            d[GHEnvironmentFingerprint.SettingsGpuBackendKey] = "OpenGL";
            foreach (KeyValuePair<string, object> kv in SampleToggles())
            {
                string value = GHEnvironmentFingerprint.FormatValue(kv.Value);
                if (value != null)
                    d[GHEnvironmentFingerprint.SettingsKey(kv.Key)] = value;
            }
            d["os.build"] = "26200.6584";
            d["hardware.cpu"] = "Test CPU";
            d["component.Microsoft.Extensions.Logging"] = "10.0.0";
            return d;
        }

        [Fact]
        public void FromLegacyFields_KeysAndValuesMatchNewCaptureOfSameValues()
        {
            Dictionary<string, string> legacy = SampleLegacy();
            Dictionary<string, string> capture = NewCaptureOfSameValues();

            foreach (KeyValuePair<string, string> kv in legacy)
            {
                Assert.True(capture.ContainsKey(kv.Key), "new capture lacks legacy key " + kv.Key);
                Assert.Equal(capture[kv.Key], kv.Value);
            }
            List<GHFingerprintChange> changes = GHEnvironmentFingerprint.Diff(legacy, capture);
            foreach (GHFingerprintChange c in changes)
                Assert.True(c.Kind == GHFingerprintChangeKind.Unknown || c.Kind == GHFingerprintChangeKind.NotCompared,
                    c.Key + " is " + c.Kind);
            Assert.Equal("none", GHEnvironmentFingerprint.AttributionLabel(changes));
        }

        [Fact]
        public void MergeMissing_FillsAbsentKeys_KeepsExisting()
        {
            Dictionary<string, string> target = Legacy("os.build", "new", "code.appVersion", "4.1");
            Dictionary<string, string> source = Legacy("os.build", "old", "hardware.cpu", "X");

            GHEnvironmentFingerprint.MergeMissing(target, source);

            Assert.Equal(3, target.Count);
            Assert.Equal("new", target["os.build"]);
            Assert.Equal("X", target["hardware.cpu"]);
        }

        [Fact]
        public void ShortHash_GoldenValue_OrderIndependent_MetaExcluded()
        {
            Dictionary<string, string> a = Legacy("os.build", "26200.6584", "hardware.cpu", "Test CPU");
            Dictionary<string, string> b = Versioned("hardware.cpu", "Test CPU", "os.build", "26200.6584");
            b[GHEnvironmentFingerprint.MetaCapturedUtcKey] = "2026-09-27T10:00:00Z";

            string hash = GHEnvironmentFingerprint.ShortHash(a, null);

            Assert.Equal("099ee2fe66ee", hash);
            Assert.Equal(hash, GHEnvironmentFingerprint.ShortHash(b, null));
        }

        [Fact]
        public void ShortHash_CategoryFilter_HashesOnlyThatCategory()
        {
            Dictionary<string, string> d = Legacy("os.build", "26200.6584", "hardware.cpu", "Test CPU");

            Assert.Equal("e3190fde94c6", GHEnvironmentFingerprint.ShortHash(d, "os"));
            Assert.Equal("e3b0c44298fc", GHEnvironmentFingerprint.ShortHash(d, "driver"));
        }

        [Fact]
        public void ShortHash_ValueChange_ChangesHash()
        {
            string before = GHEnvironmentFingerprint.ShortHash(Legacy("os.build", "1"), null);
            string after = GHEnvironmentFingerprint.ShortHash(Legacy("os.build", "2"), null);

            Assert.NotEqual(before, after);
            Assert.Equal(GHEnvironmentFingerprint.ShortHashLength, before.Length);
            Assert.Matches("^[0-9a-f]{12}$", before);
        }

        [Fact]
        public void AppendReportLines_OneLinePerListedKind()
        {
            List<GHFingerprintChange> changes = new List<GHFingerprintChange>
            {
                Change("os.build", GHFingerprintChangeKind.Changed, "26200.6584", "26200.6725"),
                Change("driver.gpu0.version", GHFingerprintChangeKind.Added, null, "32.0"),
                Change("settings.powerPlan", GHFingerprintChangeKind.Removed, "Balanced", null),
                Change("hardware.cpu", GHFingerprintChangeKind.Unknown, null, "Test CPU"),
                Change("component.A", GHFingerprintChangeKind.NotCompared, null, "1"),
                Change("component.B", GHFingerprintChangeKind.NotCompared, "1", null)
            };
            StringBuilder sb = new StringBuilder();

            GHEnvironmentFingerprint.AppendReportLines(sb, changes, GHPerformanceTextReport.MaxLineWidth);

            Assert.Equal("  os.build: 26200.6584 -> 26200.6725\n"
                + "  driver.gpu0.version: (absent) -> 32.0\n"
                + "  settings.powerPlan: Balanced -> (absent)\n"
                + "  hardware.cpu: (not recorded) -> Test CPU\n"
                + "  (2 components not compared: loaded on one side only)\n", sb.ToString());
        }

        [Fact]
        public void AppendReportLines_NothingListed_IsNone()
        {
            StringBuilder empty = new StringBuilder();
            GHEnvironmentFingerprint.AppendReportLines(empty, new List<GHFingerprintChange>(), 78);
            Assert.Equal("  none\n", empty.ToString());

            StringBuilder onlyNotCompared = new StringBuilder();
            GHEnvironmentFingerprint.AppendReportLines(onlyNotCompared, new List<GHFingerprintChange>
            {
                Change("component.A", GHFingerprintChangeKind.NotCompared, null, "1")
            }, 78);
            Assert.Equal("  none\n  (1 component not compared: loaded on one side only)\n", onlyNotCompared.ToString());
        }

        [Fact]
        public void AppendReportLines_LongValuesAndKeys_FitMaxWidth()
        {
            string longA = new string('a', 100);
            string longB = new string('b', 200);
            List<GHFingerprintChange> changes = new List<GHFingerprintChange>
            {
                Change("toolchain.runtime", GHFingerprintChangeKind.Changed, longA, longB),
                Change("toolchain.sdk", GHFingerprintChangeKind.Changed, "1.0", longB),
                Change("toolchain.compiler", GHFingerprintChangeKind.Changed, longA, "2.0"),
                Change("component." + new string('k', 90), GHFingerprintChangeKind.Changed, "1", "2")
            };
            StringBuilder sb = new StringBuilder();

            GHEnvironmentFingerprint.AppendReportLines(sb, changes, GHPerformanceTextReport.MaxLineWidth);
            string report = sb.ToString();

            AssertNoLineExceedsMaxWidth(report);
            string[] lines = report.Split('\n');
            Assert.Equal(GHPerformanceTextReport.MaxLineWidth, lines[0].Length);
            Assert.Contains("...", lines[0]);
            Assert.Contains(" -> ", lines[0]);
            Assert.StartsWith("  toolchain.sdk: 1.0 -> bbb", lines[1]);
            Assert.EndsWith(" -> 2.0", lines[2]);
        }
    }
}
