using System;
using System.Collections.Generic;
using System.Globalization;

namespace GnollHackX.Performance
{
    /* The suite store's and the suite page's rules that need no store or app state:
       the device a suite ran on as a key, the per-device baseline key, the sentence
       naming a baseline that differs from a suite's group only in its refresh rate, and
       the default arm label. Pure functions of strings and dictionaries: no GHApp,
       GHConstants, MAUI or Xamarin types. Must compile under C# 7.3 (the legacy
       netstandard2.0 project). */
    public static class GHPerformanceSuiteLogic
    {
        /* Fingerprint key of the first 12 hex characters of the app assembly's module
           version id; written on MAUI Windows only */
        public const string CodeAssemblyMvidKey = "code.assemblyMvid";

        /* Separates a comparability key from the device key in a baselines.json key */
        public const string DeviceMarker = "|device=";

        private const string HardwareCpuKey = "hardware.cpu";
        private const string HardwareSocKey = "hardware.soc";

        /* Index of round(measuredRefreshHz) among a comparability key's '|'-separated
           segments: scenario|replaySha256|startTurn|pageMode|mapRefreshRateSetting|
           round(measuredRefreshHz)|m<metricsVersion> */
        private const int RefreshSegment = 5;

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
           a key among baselineKeys differs from key only in its refresh rate segment, or
           null. Either key may carry a device part (BaselineKey); when both do, the
           devices must agree. Several such rates are listed in ascending order. */
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
            if (rates.Count == 0)
                return null;
            rates.Sort(CompareRates);
            return "a baseline exists for the same replay at " + string.Join(", ", rates.ToArray())
                + " Hz (this suite: " + own[RefreshSegment] + " Hz)";
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
