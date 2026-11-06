using System;
using System.Collections.Generic;

namespace GnollHackM
{
    /* The Windows Avalonia GPU mode of --win-gpu and GNOLLHACK_WIN_GPU, apart from "avalonia", which installs no
       adapter selection callback */
    public enum GHWindowsGpuMode
    {
        Auto,
        Dedicated,
        Integrated,
    }

    /* The per-app Windows graphics preference: GpuPreference=1 is Integrated, 2 Dedicated; everything else,
       0 ("Let Windows decide") and no value included, is Unset */
    public enum GHWindowsGpuPreference
    {
        Unset,
        Integrated,
        Dedicated,
    }

    /* The Windows render adapter rules without DXGI, Avalonia or GHApp: the adapter choice of the Windows Avalonia
       selection callback, its inputs' parsing, and the rule of GHApp.InformAboutGPU */
    public static class GHWindowsGpuChoice
    {
        private const string PreferenceKey = "GpuPreference";
        private const int PreferenceIntegratedValue = 1;
        private const int PreferenceDedicatedValue = 2;

        /* An index into adapterLuids (Avalonia's order); 0 when the wanted adapter is not in the list or its LUID
           is unknown, which is Avalonia's own choice. Integrated mode, and Auto with the Integrated preference,
           want the minimum-power adapter; every other case wants the high-performance one. */
        public static int ChooseIndex(IReadOnlyList<long> adapterLuids, GHWindowsGpuMode mode,
            GHWindowsGpuPreference preference, long? highPerformanceLuid, long? minimumPowerLuid)
        {
            if (adapterLuids == null || adapterLuids.Count == 0)
                return 0;

            long? wanted;
            if (mode == GHWindowsGpuMode.Integrated)
                wanted = minimumPowerLuid;
            else if (mode == GHWindowsGpuMode.Dedicated)
                wanted = highPerformanceLuid;
            else if (preference == GHWindowsGpuPreference.Integrated)
                wanted = minimumPowerLuid;
            else
                wanted = highPerformanceLuid;
            if (!wanted.HasValue)
                return 0;

            for (int i = 0; i < adapterLuids.Count; i++)
            {
                if (adapterLuids[i] == wanted.Value)
                    return i;
            }
            return 0;
        }

        /* The data of the UserGpuPreferences value, such as "GpuPreference=2;" or
           "SwapEffectUpgradeEnable=1;GpuPreference=1;". Read as GHApp.GetActiveGPU reads it: the first entry whose
           key is exactly GpuPreference decides. Null, empty and unparsable data are Unset. */
        public static GHWindowsGpuPreference ParsePreferenceValue(string registryData)
        {
            if (string.IsNullOrEmpty(registryData))
                return GHWindowsGpuPreference.Unset;
            string[] entries = registryData.Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries);
            for (int i = 0; i < entries.Length; i++)
            {
                string[] parts = entries[i].Split(new char[] { '=' }, StringSplitOptions.RemoveEmptyEntries);
                if (parts.Length != 2 || parts[0] != PreferenceKey)
                    continue;
                int value;
                if (!int.TryParse(parts[1], out value))
                    return GHWindowsGpuPreference.Unset;
                if (value == PreferenceIntegratedValue)
                    return GHWindowsGpuPreference.Integrated;
                if (value == PreferenceDedicatedValue)
                    return GHWindowsGpuPreference.Dedicated;
                return GHWindowsGpuPreference.Unset;
            }
            return GHWindowsGpuPreference.Unset;
        }

        /* "Unset", "Integrated", "Dedicated" */
        public static string PreferenceName(GHWindowsGpuPreference preference)
        {
            switch (preference)
            {
            case GHWindowsGpuPreference.Integrated:
                return "Integrated";
            case GHWindowsGpuPreference.Dedicated:
                return "Dedicated";
            default:
                return "Unset";
            }
        }

        /* "auto", "dedicated", "integrated", in any case, trimmed; false for anything else */
        public static bool TryParseMode(string text, out GHWindowsGpuMode mode)
        {
            mode = GHWindowsGpuMode.Auto;
            if (text == null)
                return false;
            string trimmed = text.Trim();
            if (string.Equals(trimmed, "auto", StringComparison.OrdinalIgnoreCase))
            {
                mode = GHWindowsGpuMode.Auto;
                return true;
            }
            if (string.Equals(trimmed, "dedicated", StringComparison.OrdinalIgnoreCase))
            {
                mode = GHWindowsGpuMode.Dedicated;
                return true;
            }
            if (string.Equals(trimmed, "integrated", StringComparison.OrdinalIgnoreCase))
            {
                mode = GHWindowsGpuMode.Integrated;
                return true;
            }
            return false;
        }

        /* "auto", "dedicated", "integrated" */
        public static string ModeName(GHWindowsGpuMode mode)
        {
            switch (mode)
            {
            case GHWindowsGpuMode.Dedicated:
                return "dedicated";
            case GHWindowsGpuMode.Integrated:
                return "integrated";
            default:
                return "auto";
            }
        }

        /* A LUID as Avalonia passes it, BitConverter.GetBytes of ((long)HighPart << 32) | LowPart; 0 for null or
           under 8 bytes */
        public static long LuidFromBytes(byte[] deviceLuid)
        {
            if (deviceLuid == null || deviceLuid.Length < 8)
                return 0;
            return BitConverter.ToInt64(deviceLuid, 0);
        }

        /* GHApp.InformAboutGPU's hardware part: true when the render adapter is integrated and a discrete one
           exists. renderIsIntegrated null (the adapter is unknown) keeps the registry rule on activeGpu, a
           GHApp.GetActiveGPU result: packaged, any preference but "Dedicated"; unpackaged, "Integrated" only. */
        public static bool ShouldInformAboutGpu(bool? renderIsIntegrated, bool discretePresent, bool isPackaged,
            string activeGpu)
        {
            if (renderIsIntegrated.HasValue)
                return renderIsIntegrated.Value && discretePresent;
            return isPackaged ? activeGpu != "Dedicated" : activeGpu == "Integrated";
        }
    }
}
