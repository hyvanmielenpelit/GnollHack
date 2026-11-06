#if GNH_AVALONIA && WINDOWS_AVALONIA
using System;
using System.Collections.Generic;
using System.Globalization;
using Avalonia.Platform;
using GnollHackX;
using Microsoft.Win32;

namespace GnollHackM
{
    /* The Direct3D 11 adapter of Windows Avalonia. CreateCallback makes the GraphicsAdapterSelectionCallback of
       Win32PlatformOptions, which Avalonia calls when it creates its ANGLE display: during platform setup, and again
       on the render thread after a device loss. The callback chooses with GHWindowsGpuChoice from the Windows per-app
       preference and EnumAdapterByGpuPreference, records the choice, and writes one "GnollHack: Windows GPU" line to
       standard error on every run. It never touches GHApp, whose static initializers need MAUI Essentials that are not
       installed when it first runs, never throws, and always returns an index within the list. */
    public static class GHWindowsGpuSelection
    {
        private const string UserGpuPreferencesKey = @"Software\Microsoft\DirectX\UserGpuPreferences";

        private static readonly object _lock = new object();
        private static bool _hasChoice = false;
        private static long _chosenLuid = 0;
        private static string _chosenName = null;
        private static GHWindowsGpuMode _chosenMode = GHWindowsGpuMode.Auto;

        public static Func<IReadOnlyList<PlatformGraphicsDeviceAdapterDescription>, int> CreateCallback(GHWindowsGpuMode mode)
        {
            return descriptions => ChooseAdapter(descriptions, mode);
        }

        /* The adapter the callback last chose, "<name>, mode <mode>"; "not selected" until the callback has chosen */
        public static string Summary
        {
            get
            {
                lock (_lock)
                {
                    if (!_hasChoice)
                        return "not selected";
                    return _chosenName + ", mode " + GHWindowsGpuChoice.ModeName(_chosenMode);
                }
            }
        }

        /* The contract of GHRenderAdapterProbeWindows.TryGetLast: false, with render null and all empty, until the
           callback has chosen. After that, all (when non-null) is cleared and filled with a fresh enumeration of every
           DXGI adapter, IsIntegrated from GHApp.DeviceGPUs, and render is a copy of the one whose LUID the callback
           recorded; false, with render null, when that adapter is gone. Any thread; never throws. */
        public static bool TryGetRenderAdapter(out GHRenderAdapter render, List<GHRenderAdapter> all)
        {
            render = null;
            try
            {
                if (all != null)
                    all.Clear();
                long luid;
                lock (_lock)
                {
                    if (!_hasChoice)
                        return false;
                    luid = _chosenLuid;
                }

                List<GHRenderAdapter> adapters = new List<GHRenderAdapter>();
                if (!GHDxgiAdapters.Enumerate(adapters, SnapshotDeviceGpus()))
                    return false;
                if (all != null)
                    all.AddRange(adapters);
                for (int i = 0; i < adapters.Count; i++)
                {
                    if (adapters[i].Luid == luid)
                    {
                        render = GHDxgiAdapters.Copy(adapters[i]);
                        return true;
                    }
                }
                return false;
            }
            catch
            {
                render = null;
                return false;
            }
        }

        private static int ChooseAdapter(IReadOnlyList<PlatformGraphicsDeviceAdapterDescription> descriptions, GHWindowsGpuMode mode)
        {
            try
            {
                int count = descriptions != null ? descriptions.Count : 0;
                if (count == 0)
                {
                    WriteLog("GnollHack: Windows GPU selection failed: no adapters; using Avalonia's choice");
                    return 0;
                }

                List<long> luids = new List<long>(count);
                for (int i = 0; i < count; i++)
                {
                    PlatformGraphicsDeviceAdapterDescription description = descriptions[i];
                    luids.Add(GHWindowsGpuChoice.LuidFromBytes(description != null ? description.DeviceLuid : null));
                }

                GHWindowsGpuPreference preference = ReadPreference();
                long? highPerformanceLuid, minimumPowerLuid;
                GHDxgiAdapters.TryGetPreferenceLuids(out highPerformanceLuid, out minimumPowerLuid);
                int index = GHWindowsGpuChoice.ChooseIndex(luids, mode, preference, highPerformanceLuid, minimumPowerLuid);
                if (index < 0 || index >= count)
                    index = 0;

                /* Avalonia's descriptions are lower-cased; DXGI's own names are not */
                List<GHRenderAdapter> adapters = new List<GHRenderAdapter>();
                GHDxgiAdapters.Enumerate(adapters, null);
                long chosenLuid = luids[index];
                string chosenName = AdapterName(adapters, chosenLuid);
                if (chosenName == null)
                {
                    PlatformGraphicsDeviceAdapterDescription description = descriptions[index];
                    chosenName = description != null && !string.IsNullOrWhiteSpace(description.Description)
                        ? description.Description.Trim() : "unknown adapter";
                }

                lock (_lock)
                {
                    _hasChoice = true;
                    _chosenLuid = chosenLuid;
                    _chosenName = chosenName;
                    _chosenMode = mode;
                }

                WriteLog("GnollHack: Windows GPU " + chosenName + " (luid " + FormatLuid(chosenLuid) + "), mode "
                    + GHWindowsGpuChoice.ModeName(mode) + ", Windows preference " + GHWindowsGpuChoice.PreferenceName(preference)
                    + ", high performance " + PreferenceAdapterName(adapters, highPerformanceLuid)
                    + ", minimum power " + PreferenceAdapterName(adapters, minimumPowerLuid));
                return index;
            }
            catch (Exception ex)
            {
                WriteLog("GnollHack: Windows GPU selection failed: " + ex.Message + "; using Avalonia's choice");
                return 0;
            }
        }

        /* The per-app preference of HKCU\Software\Microsoft\DirectX\UserGpuPreferences, whose value names are
           executable paths; registry value names are case-insensitive */
        private static GHWindowsGpuPreference ReadPreference()
        {
            if (!OperatingSystem.IsWindows())
                return GHWindowsGpuPreference.Unset;
            string path = Environment.ProcessPath;
            if (string.IsNullOrEmpty(path))
                return GHWindowsGpuPreference.Unset;
            using (RegistryKey key = Registry.CurrentUser.OpenSubKey(UserGpuPreferencesKey))
            {
                if (key == null)
                    return GHWindowsGpuPreference.Unset;
                return GHWindowsGpuChoice.ParsePreferenceValue(key.GetValue(path) as string);
            }
        }

        private static string AdapterName(List<GHRenderAdapter> adapters, long luid)
        {
            for (int i = 0; i < adapters.Count; i++)
            {
                if (adapters[i].Luid == luid && !string.IsNullOrWhiteSpace(adapters[i].Name))
                    return adapters[i].Name;
            }
            return null;
        }

        private static string PreferenceAdapterName(List<GHRenderAdapter> adapters, long? luid)
        {
            if (!luid.HasValue)
                return "none";
            string name = AdapterName(adapters, luid.Value);
            return name ?? "luid " + FormatLuid(luid.Value);
        }

        /* "0x<HighPart>_0x<LowPart>", as PDH writes it */
        private static string FormatLuid(long luid)
        {
            uint high = unchecked((uint)(luid >> 32));
            uint low = unchecked((uint)luid);
            return "0x" + high.ToString("X8", CultureInfo.InvariantCulture) + "_0x" + low.ToString("X8", CultureInfo.InvariantCulture);
        }

        private static void WriteLog(string line)
        {
            try
            {
                Console.Error.WriteLine(line);
            }
            catch
            {
            }
        }

        /* GHApp.DeviceGPUs has no lock; it is filled once at startup */
        private static DeviceGPU[] SnapshotDeviceGpus()
        {
            try
            {
                return GHApp.DeviceGPUs.ToArray();
            }
            catch
            {
                return Array.Empty<DeviceGPU>();
            }
        }
    }
}
#endif
