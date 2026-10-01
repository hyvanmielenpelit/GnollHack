using System;
using System.Collections.Generic;
using System.Globalization;

namespace GnollHackX.Performance
{
    /* The suite store's and the suite page's rules that need no store or app state:
       the device a suite ran on as a key, the per-device baseline key, the sentence
       naming a baseline that differs from a suite's group only in its refresh rate or
       thermal settings, the thermal gate settings and their key segment, and the default
       arm label. Pure functions of strings and dictionaries: no GHApp,
       GHConstants, MAUI or Xamarin types. Must compile under C# 7.3 (the legacy
       netstandard2.0 project). */
    public static class GHPerformanceSuiteLogic
    {
        /* Fingerprint key of the first 12 hex characters of the app assembly's module
           version id; written on MAUI Windows only */
        public const string CodeAssemblyMvidKey = "code.assemblyMvid";

        /* Separates a comparability key from the device key in a baselines.json key */
        public const string DeviceMarker = "|device=";

        /* Setup values of the thermal gate between runs: no worse than the status read at the
           suite start, or Light or better */
        public const string ThermalGateStart = "start";
        public const string ThermalGateLight = "light";

        /* The longest a thermal gate waits when the setup does not say, and in suites recorded
           before the setting existed */
        public const int DefaultThermalWaitSeconds = 300;

        private const string HardwareCpuKey = "hardware.cpu";
        private const string HardwareSocKey = "hardware.soc";

        /* Index of round(measuredRefreshHz) among a comparability key's '|'-separated
           segments: scenario|replaySha256|startTurn|pageMode|mapRefreshRateSetting|
           round(measuredRefreshHz)|m<metricsVersion> */
        private const int RefreshSegment = 5;

        /* Segments of a comparability key before its optional thermal segment, through m<metricsVersion> */
        private const int BaseSegmentCount = 7;

        private const string ThermalSegmentPrefix = "tg=";

        private const int CommitLength = 7;
        private const int MvidLength = 6;

        /* "os.platform|hardware.deviceModel|<hardware.cpu, else hardware.soc>" of the
           fingerprint; a missing platform or model is taken from platform and model (the
           manifest's), and a missing processor is left empty. */
        public static string DeviceKey(IDictionary<string, string> fingerprint, string platform, string model)
        {
            string p = ValueOf(fingerprint, GHEnvironmentFingerprint.OsPlatformKey);
            if (string.IsNullOrEmpty(p))
                p = platform;
            string m = ValueOf(fingerprint, GHEnvironmentFingerprint.HardwareDeviceModelKey);
            if (string.IsNullOrEmpty(m))
                m = model;
            string c = ValueOf(fingerprint, HardwareCpuKey);
            if (string.IsNullOrEmpty(c))
                c = ValueOf(fingerprint, HardwareSocKey);
            return OrEmpty(p) + "|" + OrEmpty(m) + "|" + OrEmpty(c);
        }

        /* The baselines.json key of a comparability key on one device */
        public static string BaselineKey(string comparabilityKey, string deviceKey)
        {
            return OrEmpty(comparabilityKey) + DeviceMarker + OrEmpty(deviceKey);
        }

        /* "a baseline exists for the same replay at <Hz> Hz (this suite: <Hz> Hz)" when
           a key among baselineKeys differs from key only in its refresh rate segment, and
           "a baseline exists for the same replay with thermal gate <settings> (this suite:
           <settings>)" when one differs only in its thermal segment; both joined with "; "
           when both apply, or null when neither does. Either key may carry a device part
           (BaselineKey); when both do, the devices must agree. Several such rates are listed
           in ascending order, several thermal settings in ordinal order. */
        public static string DescribeKeyMismatch(string key, IEnumerable<string> baselineKeys)
        {
            if (string.IsNullOrEmpty(key) || baselineKeys == null)
                return null;
            string device;
            string[] own = SplitKey(key, out device);
            if (own.Length <= RefreshSegment)
                return null;

            List<string> rates = new List<string>();
            foreach (string other in baselineKeys)
            {
                if (string.IsNullOrEmpty(other))
                    continue;
                string otherDevice;
                string[] parts = SplitKey(other, out otherDevice);
                if (parts.Length != own.Length)
                    continue;
                if (device != null && otherDevice != null && !string.Equals(device, otherDevice, StringComparison.Ordinal))
                    continue;
                if (string.Equals(parts[RefreshSegment], own[RefreshSegment], StringComparison.Ordinal))
                    continue;
                bool sameOtherwise = true;
                for (int i = 0; i < parts.Length; i++)
                {
                    if (i != RefreshSegment && !string.Equals(parts[i], own[i], StringComparison.Ordinal))
                    {
                        sameOtherwise = false;
                        break;
                    }
                }
                if (sameOtherwise && !rates.Contains(parts[RefreshSegment]))
                    rates.Add(parts[RefreshSegment]);
            }
            string refreshSentence = null;
            if (rates.Count > 0)
            {
                rates.Sort(CompareRates);
                refreshSentence = "a baseline exists for the same replay at " + string.Join(", ", rates.ToArray())
                    + " Hz (this suite: " + own[RefreshSegment] + " Hz)";
            }

            string thermalSentence = null;
            if (own.Length >= BaseSegmentCount)
            {
                string ownThermal = ThermalPart(own);
                List<string> thermals = new List<string>();
                foreach (string other in baselineKeys)
                {
                    if (string.IsNullOrEmpty(other))
                        continue;
                    string otherDevice;
                    string[] parts = SplitKey(other, out otherDevice);
                    if (parts.Length < BaseSegmentCount)
                        continue;
                    if (device != null && otherDevice != null && !string.Equals(device, otherDevice, StringComparison.Ordinal))
                        continue;
                    bool sameBase = true;
                    for (int i = 0; i < BaseSegmentCount; i++)
                    {
                        if (!string.Equals(parts[i], own[i], StringComparison.Ordinal))
                        {
                            sameBase = false;
                            break;
                        }
                    }
                    if (!sameBase)
                        continue;
                    string otherThermal = ThermalPart(parts);
                    if (string.Equals(otherThermal, ownThermal, StringComparison.Ordinal))
                        continue;
                    string description = DescribeThermalPart(otherThermal);
                    if (!thermals.Contains(description))
                        thermals.Add(description);
                }
                if (thermals.Count > 0)
                {
                    thermals.Sort(string.CompareOrdinal);
                    thermalSentence = "a baseline exists for the same replay with thermal gate " + string.Join(" or ", thermals.ToArray())
                        + " (this suite: " + DescribeThermalPart(ownThermal) + ")";
                }
            }

            if (refreshSentence != null && thermalSentence != null)
                return refreshSentence + "; " + thermalSentence;
            return refreshSentence ?? thermalSentence;
        }

        /* "start" for null, empty or "start", "light" for "light" (trimmed, any case); null otherwise */
        public static string NormalizeThermalGate(string gate)
        {
            if (gate == null)
                return ThermalGateStart;
            string g = gate.Trim();
            if (g.Length == 0 || string.Equals(g, ThermalGateStart, StringComparison.OrdinalIgnoreCase))
                return ThermalGateStart;
            if (string.Equals(g, ThermalGateLight, StringComparison.OrdinalIgnoreCase))
                return ThermalGateLight;
            return null;
        }

        /* "Light or better" for light, otherwise "same as start" */
        public static string ThermalGateDisplayName(string gate)
        {
            return NormalizeThermalGate(gate) == ThermalGateLight ? "Light or better" : "same as start";
        }

        /* The replay header phase of a thermal gate: "cooling Moderate > Light 45/300 s" */
        public static string ThermalGatePhase(string statusName, string limitName, long elapsedSeconds, int waitSeconds)
        {
            return "cooling " + (statusName ?? "?") + " > " + (limitName ?? "?") + " "
                + elapsedSeconds.ToString(CultureInfo.InvariantCulture) + "/"
                + waitSeconds.ToString(CultureInfo.InvariantCulture) + " s";
        }

        /* The comparability key's thermal segment, "|tg=<gate>,<wait seconds>"; empty for the
           start gate with DefaultThermalWaitSeconds, the setting of every suite recorded before
           the thermal settings existed, so that those suites stay comparable */
        public static string ThermalKeySegment(string gate, int waitSeconds)
        {
            string g = NormalizeThermalGate(gate) ?? gate;
            if (g == ThermalGateStart && waitSeconds == DefaultThermalWaitSeconds)
                return "";
            return "|" + ThermalSegmentPrefix + g + "," + waitSeconds.ToString(CultureInfo.InvariantCulture);
        }

        /* The segments after the first BaseSegmentCount, joined with '|'; empty when there are none */
        private static string ThermalPart(string[] parts)
        {
            if (parts.Length <= BaseSegmentCount)
                return "";
            return string.Join("|", parts, BaseSegmentCount, parts.Length - BaseSegmentCount);
        }

        /* "<gate display name>, wait <n> s" of a thermal part; "same as start, wait 300 s" for
           an empty one, and the part itself when it is not "tg=<gate>,<n>" */
        private static string DescribeThermalPart(string part)
        {
            if (string.IsNullOrEmpty(part))
                return ThermalGateDisplayName(ThermalGateStart) + ", wait "
                    + DefaultThermalWaitSeconds.ToString(CultureInfo.InvariantCulture) + " s";
            if (!part.StartsWith(ThermalSegmentPrefix, StringComparison.Ordinal))
                return part;
            string rest = part.Substring(ThermalSegmentPrefix.Length);
            int comma = rest.LastIndexOf(',');
            if (comma < 0)
                return part;
            return ThermalGateDisplayName(rest.Substring(0, comma)) + ", wait " + rest.Substring(comma + 1) + " s";
        }

        /* "<version> <commit, first 7> m<mvid, first 6>", leaving out the commit and the
           id when they are unknown; version alone when both are */
        public static string DefaultArmLabel(string version, string commit, string mvid)
        {
            string c = Shorten(commit, CommitLength);
            string m = Shorten(mvid, MvidLength);
            if (string.IsNullOrEmpty(c) && string.IsNullOrEmpty(m))
                return version ?? "";
            string label = version ?? "";
            if (!string.IsNullOrEmpty(c))
                label += " " + c;
            if (!string.IsNullOrEmpty(m))
                label += " m" + m;
            return label.Trim();
        }

        /* The key's '|'-separated segments before its device part, and that device part
           (null when it has none) */
        private static string[] SplitKey(string key, out string device)
        {
            int marker = key.IndexOf(DeviceMarker, StringComparison.Ordinal);
            if (marker < 0)
            {
                device = null;
                return key.Split('|');
            }
            device = key.Substring(marker + DeviceMarker.Length);
            return key.Substring(0, marker).Split('|');
        }

        /* Numerically where both parse, else ordinally */
        private static int CompareRates(string x, string y)
        {
            double a, b;
            if (double.TryParse(x, NumberStyles.Float, CultureInfo.InvariantCulture, out a)
                && double.TryParse(y, NumberStyles.Float, CultureInfo.InvariantCulture, out b))
                return a.CompareTo(b);
            return string.CompareOrdinal(x, y);
        }

        private static string ValueOf(IDictionary<string, string> fingerprint, string key)
        {
            string value;
            if (fingerprint != null && fingerprint.TryGetValue(key, out value))
                return value;
            return null;
        }

        private static string Shorten(string s, int length)
        {
            if (string.IsNullOrEmpty(s))
                return null;
            s = s.Trim();
            return s.Length <= length ? s : s.Substring(0, length);
        }

        private static string OrEmpty(string s)
        {
            return s ?? "";
        }
    }
}
