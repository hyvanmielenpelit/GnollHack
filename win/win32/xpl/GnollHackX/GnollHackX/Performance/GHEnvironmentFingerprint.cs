using System;
using System.Collections.Generic;
using System.Globalization;
using System.Security.Cryptography;
using System.Text;

namespace GnollHackX.Performance
{
    /* The environment fingerprint of a run: a flat string dictionary keyed
       "<category>.<name>", its diff against another fingerprint, the attribution label
       drawn from that diff, and the mapping from the older per-field environment
       records. Shared by the in-app harness and the offline analyzer, so that both
       attribute a difference the same way.

       The file must compile under C# 7.3 (the legacy netstandard2.0 project) and must not
       depend on LINQ, JSON, GHApp, GHConstants or any MAUI type. */

    public enum GHFingerprintChangeKind
    {
        Changed = 0,        /* present on both sides with different values */
        Added = 1,          /* only in the second fingerprint; both sides versioned */
        Removed = 2,        /* only in the first fingerprint; both sides versioned */
        NotCompared = 3,    /* a component loaded on one side only */
        Unknown = 4         /* one-sided, and at least one side is a legacy record */
    }

    /* One differing key. Before is the first fingerprint's value and After the second's;
       the absent side is null. */
    public struct GHFingerprintChange
    {
        public string Key;
        public string Category;
        public GHFingerprintChangeKind Kind;
        public string Before;
        public string After;
    }

    public static class GHEnvironmentFingerprint
    {
        public const string FingerprintVersion = "1";

        public const string CategoryMeta = "meta";
        public const string CategoryCode = "code";
        public const string CategoryToolchain = "toolchain";
        public const string CategoryComponent = "component";
        public const string CategoryOs = "os";
        public const string CategoryDriver = "driver";
        public const string CategoryHardware = "hardware";
        public const string CategorySettings = "settings";

        /* Attribution order. CategoryMeta is never diffed and is not listed; categories
           outside this list sort after it, ordinally. */
        public static readonly string[] CategoryOrder =
        {
            CategoryCode, CategoryToolchain, CategoryComponent, CategoryOs, CategoryDriver, CategoryHardware, CategorySettings
        };

        public const string MetaFingerprintVersionKey = "meta.fingerprintVersion";
        public const string MetaCapturedUtcKey = "meta.capturedUtc";

        /* Keys the legacy mapping emits. A new capture emits the same keys for the same
           values, so that old and new records diff meaningfully. */
        public const string CodeAppVersionKey = "code.appVersion";
        public const string CodeGitCommitKey = "code.gitCommit";
        public const string CodeBuildConfigurationKey = "code.buildConfiguration";
        public const string ToolchainRuntimeKey = "toolchain.runtime";
        public const string ToolchainFrameworkKey = "toolchain.framework";
        public const string ComponentMauiControlsKey = "component.Microsoft.Maui.Controls";
        public const string ComponentSkiaSharpKey = "component.SkiaSharp";
        public const string ComponentFmodKey = "component.native.fmod";
        public const string OsPlatformKey = "os.platform";
        public const string OsVersionKey = "os.version";
        public const string HardwareDeviceModelKey = "hardware.deviceModel";
        public const string SettingsMapRefreshRateKey = "settings.mapRefreshRate";
        public const string SettingsGpuBackendKey = "settings.gpuBackend";
        public const string SettingsPrefix = "settings.";

        public const string AttributionNone = "none";
        public const string AttributionCode = "code";
        public const string AttributionEnvironmentPrefix = "environment: ";
        public const string AttributionConfoundedPrefix = "confounded: ";
        public const string SettingsViolationText = "more than one setting differs";

        /* The value CommonValues gives a key whose inputs disagree. */
        public const string MixedValue = "mixed";

        public const int ShortHashLength = 12;

        private const string Ellipsis = "...";
        private const string AbsentText = "(absent)";
        private const string NotRecordedText = "(not recorded)";

        /* The part of a key before its first '.', or the whole key when it has none. */
        public static string CategoryOf(string key)
        {
            if (string.IsNullOrEmpty(key))
                return "";
            int dot = key.IndexOf('.');
            return dot < 0 ? key : key.Substring(0, dot);
        }

        /* Position in CategoryOrder; categories outside it rank after the last entry. */
        public static int CategoryRank(string category)
        {
            for (int i = 0; i < CategoryOrder.Length; i++)
            {
                if (CategoryOrder[i] == category)
                    return i;
            }
            return CategoryOrder.Length;
        }

        public static string SettingsKey(string name)
        {
            return SettingsPrefix + name;
        }

        /* A fingerprint value for a setting or measurement: booleans "true"/"false",
           numbers and other formattable values in invariant culture, null for null. */
        public static string FormatValue(object value)
        {
            if (value == null)
                return null;
            if (value is bool)
                return (bool)value ? "true" : "false";
            IFormattable formattable = value as IFormattable;
            if (formattable != null)
                return formattable.ToString(null, CultureInfo.InvariantCulture);
            return value.ToString();
        }

        /* The keys that differ between a and b, sorted by category order, then ordinally
           by key. Values compare ordinally. meta.* is never diffed. A one-sided
           component.* key is NotCompared, since assemblies load lazily; any other
           one-sided key is Added or Removed when both sides carry
           MetaFingerprintVersionKey, and Unknown otherwise. A null fingerprint is empty. */
        public static List<GHFingerprintChange> Diff(IDictionary<string, string> a, IDictionary<string, string> b)
        {
            List<GHFingerprintChange> result = new List<GHFingerprintChange>();
            if (a == null)
                a = new Dictionary<string, string>();
            if (b == null)
                b = new Dictionary<string, string>();
            bool bothVersioned = a.ContainsKey(MetaFingerprintVersionKey) && b.ContainsKey(MetaFingerprintVersionKey);

            foreach (KeyValuePair<string, string> kv in a)
            {
                string category = CategoryOf(kv.Key);
                if (category == CategoryMeta)
                    continue;
                string after;
                if (b.TryGetValue(kv.Key, out after))
                {
                    if (!string.Equals(kv.Value, after, StringComparison.Ordinal))
                        result.Add(MakeChange(kv.Key, category, GHFingerprintChangeKind.Changed, kv.Value, after));
                }
                else
                {
                    result.Add(MakeChange(kv.Key, category, OneSidedKind(category, bothVersioned, false), kv.Value, null));
                }
            }
            foreach (KeyValuePair<string, string> kv in b)
            {
                string category = CategoryOf(kv.Key);
                if (category == CategoryMeta || a.ContainsKey(kv.Key))
                    continue;
                result.Add(MakeChange(kv.Key, category, OneSidedKind(category, bothVersioned, true), null, kv.Value));
            }

            result.Sort(CompareChanges);
            return result;
        }

        private static GHFingerprintChangeKind OneSidedKind(string category, bool bothVersioned, bool onlyInB)
        {
            if (category == CategoryComponent)
                return GHFingerprintChangeKind.NotCompared;
            if (!bothVersioned)
                return GHFingerprintChangeKind.Unknown;
            return onlyInB ? GHFingerprintChangeKind.Added : GHFingerprintChangeKind.Removed;
        }

        private static GHFingerprintChange MakeChange(string key, string category, GHFingerprintChangeKind kind,
                                                      string before, string after)
        {
            GHFingerprintChange c = new GHFingerprintChange();
            c.Key = key;
            c.Category = category;
            c.Kind = kind;
            c.Before = before;
            c.After = after;
            return c;
        }

        private static int CompareCategories(string x, string y)
        {
            int c = CategoryRank(x).CompareTo(CategoryRank(y));
            if (c != 0)
                return c;
            return string.CompareOrdinal(x, y);
        }

        private static int CompareChanges(GHFingerprintChange x, GHFingerprintChange y)
        {
            int c = CompareCategories(x.Category, y.Category);
            if (c != 0)
                return c;
            return string.CompareOrdinal(x.Key, y.Key);
        }

        private static bool Counts(GHFingerprintChangeKind kind)
        {
            return kind == GHFingerprintChangeKind.Changed
                || kind == GHFingerprintChangeKind.Added
                || kind == GHFingerprintChangeKind.Removed;
        }

        /* The attribution label over the Changed, Added and Removed entries: "none",
           "code", "environment: <categories>" when no code key differs, or
           "confounded: <categories>" when code and anything else differ. Categories are
           comma-separated in category order. */
        public static string AttributionLabel(IList<GHFingerprintChange> changes)
        {
            List<string> categories = new List<string>();
            if (changes != null)
            {
                for (int i = 0; i < changes.Count; i++)
                {
                    if (!Counts(changes[i].Kind))
                        continue;
                    string category = changes[i].Category ?? CategoryOf(changes[i].Key);
                    if (!categories.Contains(category))
                        categories.Add(category);
                }
            }
            if (categories.Count == 0)
                return AttributionNone;
            categories.Sort(CompareCategories);
            bool hasCode = categories.Contains(CategoryCode);
            if (hasCode && categories.Count == 1)
                return AttributionCode;
            return (hasCode ? AttributionConfoundedPrefix : AttributionEnvironmentPrefix) + string.Join(", ", categories.ToArray());
        }

        /* True when more than one settings.* key is Changed: the comparison then varies
           more than one variable (SettingsViolationText). */
        public static bool SettingsViolation(IList<GHFingerprintChange> changes)
        {
            if (changes == null)
                return false;
            int changed = 0;
            for (int i = 0; i < changes.Count; i++)
            {
                if (changes[i].Kind == GHFingerprintChangeKind.Changed && changes[i].Category == CategorySettings)
                    changed++;
            }
            return changed > 1;
        }

        /* The pooled fingerprint of several runs or suites: each key present in every
           input, with its value when all inputs agree and MixedValue when they do not.
           Keys missing from any input are dropped; null inputs are skipped. */
        public static Dictionary<string, string> CommonValues(IList<IDictionary<string, string>> fingerprints)
        {
            Dictionary<string, string> result = new Dictionary<string, string>();
            if (fingerprints == null)
                return result;
            IDictionary<string, string> first = null;
            for (int i = 0; i < fingerprints.Count && first == null; i++)
                first = fingerprints[i];
            if (first == null)
                return result;

            foreach (KeyValuePair<string, string> kv in first)
            {
                bool inAll = true;
                bool agree = true;
                for (int i = 0; i < fingerprints.Count && inAll; i++)
                {
                    IDictionary<string, string> f = fingerprints[i];
                    if (f == null)
                        continue;
                    string value;
                    if (!f.TryGetValue(kv.Key, out value))
                        inAll = false;
                    else if (!string.Equals(value, kv.Value, StringComparison.Ordinal))
                        agree = false;
                }
                if (inAll)
                    result[kv.Key] = agree ? kv.Value : MixedValue;
            }
            return result;
        }

        /* A fingerprint from the per-field environment of a record written before
           fingerprints existed. Null and empty values are skipped, toggle values are
           formatted with FormatValue, and MetaFingerprintVersionKey is never added. */
        public static Dictionary<string, string> FromLegacyFields(string appVersion, string gitCommit, string buildConfiguration,
                                                                  string runtimeVersion, string frameworkVersion,
                                                                  string uiFrameworkVersion, string skiaSharpVersion,
                                                                  string fmodVersion, string platform, string deviceOs,
                                                                  string deviceModel, string mapRefreshRateSetting,
                                                                  string gpuBackend, IDictionary<string, object> toggles)
        {
            Dictionary<string, string> d = new Dictionary<string, string>();
            AddIfPresent(d, CodeAppVersionKey, appVersion);
            AddIfPresent(d, CodeGitCommitKey, gitCommit);
            AddIfPresent(d, CodeBuildConfigurationKey, buildConfiguration);
            AddIfPresent(d, ToolchainRuntimeKey, runtimeVersion);
            AddIfPresent(d, ToolchainFrameworkKey, frameworkVersion);
            AddIfPresent(d, ComponentMauiControlsKey, uiFrameworkVersion);
            AddIfPresent(d, ComponentSkiaSharpKey, skiaSharpVersion);
            AddIfPresent(d, ComponentFmodKey, fmodVersion);
            AddIfPresent(d, OsPlatformKey, platform);
            AddIfPresent(d, OsVersionKey, deviceOs);
            AddIfPresent(d, HardwareDeviceModelKey, deviceModel);
            AddIfPresent(d, SettingsMapRefreshRateKey, mapRefreshRateSetting);
            AddIfPresent(d, SettingsGpuBackendKey, gpuBackend);
            if (toggles != null)
            {
                foreach (KeyValuePair<string, object> kv in toggles)
                {
                    if (string.IsNullOrEmpty(kv.Key))
                        continue;
                    AddIfPresent(d, SettingsKey(kv.Key), FormatValue(kv.Value));
                }
            }
            return d;
        }

        private static void AddIfPresent(Dictionary<string, string> d, string key, string value)
        {
            if (!string.IsNullOrEmpty(value))
                d[key] = value;
        }

        /* Copies into target every key of source that target lacks. */
        public static void MergeMissing(IDictionary<string, string> target, IDictionary<string, string> source)
        {
            if (target == null || source == null)
                return;
            foreach (KeyValuePair<string, string> kv in source)
            {
                if (!target.ContainsKey(kv.Key))
                    target[kv.Key] = kv.Value;
            }
        }

        /* The first ShortHashLength lowercase hex characters of the SHA-256 of the
           ordinally sorted "key=value\n" lines, UTF-8. A null categoryFilter takes every
           key outside meta.*; otherwise only keys of that category. A null value hashes
           as empty. */
        public static string ShortHash(IDictionary<string, string> dict, string categoryFilter)
        {
            List<string> keys = new List<string>();
            if (dict != null)
            {
                foreach (KeyValuePair<string, string> kv in dict)
                {
                    string category = CategoryOf(kv.Key);
                    if (categoryFilter == null ? category != CategoryMeta : category == categoryFilter)
                        keys.Add(kv.Key);
                }
            }
            keys.Sort(string.CompareOrdinal);

            StringBuilder sb = new StringBuilder();
            for (int i = 0; i < keys.Count; i++)
            {
                sb.Append(keys[i]);
                sb.Append('=');
                sb.Append(dict[keys[i]]);
                sb.Append('\n');
            }

            byte[] hash;
            using (SHA256 sha = SHA256.Create())
            {
                hash = sha.ComputeHash(Encoding.UTF8.GetBytes(sb.ToString()));
            }
            StringBuilder hex = new StringBuilder(ShortHashLength);
            for (int i = 0; i < hash.Length && hex.Length < ShortHashLength; i++)
                hex.Append(hash[i].ToString("x2", CultureInfo.InvariantCulture));
            return hex.ToString(0, ShortHashLength);
        }

        /* Appends one "\n"-terminated line per Changed, Added, Removed or Unknown entry,
           "  key: before -> after", with values shortened in the middle so that no line
           exceeds maxWidth (maxWidth <= 0 means no limit); "  none" when no entry is
           listed; then a count line when there are NotCompared entries. */
        public static void AppendReportLines(StringBuilder sb, IList<GHFingerprintChange> changes, int maxWidth)
        {
            if (sb == null)
                return;
            int listed = 0;
            int notCompared = 0;
            if (changes != null)
            {
                for (int i = 0; i < changes.Count; i++)
                {
                    GHFingerprintChange c = changes[i];
                    if (c.Kind == GHFingerprintChangeKind.NotCompared)
                    {
                        notCompared++;
                        continue;
                    }
                    string missing = c.Kind == GHFingerprintChangeKind.Unknown ? NotRecordedText : AbsentText;
                    string before = c.Before ?? missing;
                    string after = c.After ?? missing;
                    AppendLine(sb, FormatChangeLine(c.Key, before, after, maxWidth), maxWidth);
                    listed++;
                }
            }
            if (listed == 0)
                AppendLine(sb, "  " + AttributionNone, maxWidth);
            if (notCompared > 0)
            {
                string noun = notCompared == 1 ? " component" : " components";
                AppendLine(sb, "  (" + notCompared.ToString(CultureInfo.InvariantCulture) + noun
                    + " not compared: loaded on one side only)", maxWidth);
            }
        }

        private static string FormatChangeLine(string key, string before, string after, int maxWidth)
        {
            string prefix = "  " + key + ": ";
            const string arrow = " -> ";
            if (maxWidth <= 0)
                return prefix + before + arrow + after;
            int room = maxWidth - prefix.Length - arrow.Length;
            if (before.Length + after.Length > room)
            {
                int half = room / 2;
                if (before.Length <= half)
                {
                    after = ShortenMiddle(after, room - before.Length);
                }
                else if (after.Length <= room - half)
                {
                    before = ShortenMiddle(before, room - after.Length);
                }
                else
                {
                    before = ShortenMiddle(before, half);
                    after = ShortenMiddle(after, room - half);
                }
            }
            return prefix + before + arrow + after;
        }

        /* s cut to at most maxLen characters, keeping its head and tail around an
           ellipsis. */
        private static string ShortenMiddle(string s, int maxLen)
        {
            if (s.Length <= maxLen)
                return s;
            if (maxLen <= Ellipsis.Length)
                return maxLen <= 0 ? "" : s.Substring(0, maxLen);
            int keep = maxLen - Ellipsis.Length;
            int head = (keep + 1) / 2;
            int tail = keep - head;
            return s.Substring(0, head) + Ellipsis + s.Substring(s.Length - tail);
        }

        private static void AppendLine(StringBuilder sb, string text, int maxWidth)
        {
            if (maxWidth > 0 && text.Length > maxWidth)
                text = maxWidth <= Ellipsis.Length ? text.Substring(0, maxWidth)
                    : text.Substring(0, maxWidth - Ellipsis.Length) + Ellipsis;
            sb.Append(text);
            sb.Append("\n");
        }
    }
}
