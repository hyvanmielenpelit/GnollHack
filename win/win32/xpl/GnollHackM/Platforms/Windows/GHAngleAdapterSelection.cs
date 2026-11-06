#if WINDOWS
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.Runtime.InteropServices;
using GnollHackX;

namespace GnollHackM
{
    // WORKAROUND(WA-SKIASHARP-0012): SKSwapChainPanel cannot choose its Direct3D adapter
    /* The Direct3D 11 adapter of the WinUI 3 GL views. SkiaSharp.Views.WinUI keeps one ANGLE display for the whole
       process and creates it when the first SKSwapChainPanel loads, on the adapter Windows lists first. EnsureSelected
       creates and initializes that display before then, with SkiaSharp's attributes plus the LUID of the adapter
       GHWindowsGpuChoice wants, the rule of Windows Avalonia. ANGLE leaves the LUID out of its display key, so
       SkiaSharp's own eglGetPlatformDisplayEXT call returns the same, initialized display. This happens only when the
       wanted adapter is a hardware adapter other than Windows' first one; otherwise SkiaSharp chooses as it would
       alone, its 9_3 and WARP fallbacks included. The choice holds for the life of the process, and the display is
       never terminated. GNOLLHACK_WIN_GPU sets the mode: auto (the default), dedicated, integrated, or default
       (also avalonia) for no pre-initialization. */
    public static class GHAngleAdapterSelection
    {
        private const string LibEgl = "libEGL.dll";
        private const string WinGpuVariable = "GNOLLHACK_WIN_GPU";
        private const string LogCategory = "gpu";

        private const uint EGL_PLATFORM_ANGLE_ANGLE = 0x3202;
        private const int EGL_PLATFORM_ANGLE_TYPE_ANGLE = 0x3203;
        private const int EGL_PLATFORM_ANGLE_TYPE_D3D11_ANGLE = 0x3208;
        private const int EGL_PLATFORM_ANGLE_D3D_LUID_HIGH_ANGLE = 0x34A0;
        private const int EGL_PLATFORM_ANGLE_D3D_LUID_LOW_ANGLE = 0x34A1;
        private const int EGL_EXPERIMENTAL_PRESENT_PATH_ANGLE = 0x33A4;
        /* SkiaSharp passes 0x33AA under the name EGL_EXPERIMENTAL_PRESENT_PATH_FAST_ANGLE; ANGLE's name is COPY */
        private const int EGL_EXPERIMENTAL_PRESENT_PATH_COPY_ANGLE = 0x33AA;
        private const int EGL_PLATFORM_ANGLE_ENABLE_AUTOMATIC_TRIM_ANGLE = 0x320F;
        private const int EGL_TRUE = 1;
        private const int EGL_FALSE = 0;
        private const int EGL_NONE = 0x3038;
        private const int EGL_DEVICE_EXT = 0x322C;
        private const int EGL_D3D11_DEVICE_ANGLE = 0x33A1;

        private const string OutcomePreinitialized = "pre-initialized";
        private const string OutcomeWindowsDefault = "Windows default";
        private const string OutcomeDefaultSwitch = "default switch";
        private const string OutcomeAlreadyInitialized = "already initialized";
        private const string OutcomeAdapterUnknown = "adapter unknown";
        private const string OutcomeNotUsed = "pre-initialized display not used";
        private const string OutcomeFailedPrefix = "failed: ";

        /* Direct exports of the libEGL.dll SkiaSharp.Views.WinUI loads from the application directory. The two
           query functions write a pointer-sized EGLAttrib. */
        [DllImport(LibEgl)]
        private static extern IntPtr eglGetPlatformDisplayEXT(uint platform, IntPtr nativeDisplay, int[] attribList);
        [DllImport(LibEgl)]
        private static extern int eglInitialize(IntPtr display, out int major, out int minor);
        [DllImport(LibEgl)]
        private static extern int eglGetError();
        [DllImport(LibEgl)]
        private static extern IntPtr eglGetCurrentDisplay();
        [DllImport(LibEgl)]
        private static extern int eglQueryDisplayAttribEXT(IntPtr display, int attribute, out IntPtr value);
        [DllImport(LibEgl)]
        private static extern int eglQueryDeviceAttribEXT(IntPtr device, int attribute, out IntPtr value);

        private static readonly object _lock = new object();
        private static bool _started = false;
        private static string _outcome = null;
        private static bool _hasRenderAdapter = false;
        private static long _renderLuid = 0;
        private static string _renderName = null;
        private static IntPtr _display = IntPtr.Zero;
        private static bool _checkDisplay = false;

        /* Chooses the adapter and pre-initializes ANGLE's display when that changes the adapter; writes one
           "Windows GPU" log line with the outcome. The first call does the work and later calls return at once.
           UI thread, before the first SKSwapChainPanel loads; never throws. */
        public static void EnsureSelected()
        {
            lock (_lock)
            {
                if (_started)
                    return;
                _started = true;
            }

            Stopwatch stopwatch = Stopwatch.StartNew();
            string line;
            try
            {
                line = Select();
            }
            catch (Exception ex)
            {
                string failure = OutcomeFailedPrefix + ex.Message;
                Record(failure, false, 0, null, IntPtr.Zero, false);
                line = "Windows GPU selection " + failure;
            }
            stopwatch.Stop();
            WriteLog(line + ", " + stopwatch.ElapsedMilliseconds.ToString(CultureInfo.InvariantCulture) + " ms");
        }

        /* The contract of GHWindowsGpuSelection.TryGetRenderAdapter: false, with render null and all empty, until
           EnsureSelected has recorded an adapter, which it does not for the default switch, an unknown adapter or a
           failure. After that, all (when non-null) is cleared and filled with a fresh enumeration of every DXGI
           adapter, IsIntegrated from GHApp.DeviceGPUs, and render is a copy of the recorded one; false, with render
           null, when that adapter is gone. Any thread; never throws. */
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
                    if (!_hasRenderAdapter)
                        return false;
                    luid = _renderLuid;
                }

                List<GHRenderAdapter> adapters = new List<GHRenderAdapter>();
                if (!GHDxgiAdapters.Enumerate(adapters, SnapshotDeviceGpus()))
                    return false;
                if (all != null)
                    all.AddRange(adapters);
                GHRenderAdapter found = FindAdapter(adapters, luid);
                if (found == null)
                    return false;
                render = GHDxgiAdapters.Copy(found);
                return true;
            }
            catch
            {
                render = null;
                return false;
            }
        }

        /* "<adapter name>, <outcome>"; the outcome alone when no adapter was recorded; "not selected" until
           EnsureSelected has finished */
        public static string Summary
        {
            get
            {
                lock (_lock)
                {
                    if (_outcome == null)
                        return "not selected";
                    if (!_hasRenderAdapter)
                        return _outcome;
                    return (_renderName ?? "unknown adapter") + ", " + _outcome;
                }
            }
        }

        /* From the first paint of a GL view, whose EGL context is then current: when a display was pre-initialized
           on the wanted adapter, logs whether SkiaSharp draws with that display. A different display means a
           SkiaSharp or ANGLE upgrade broke the pre-initialization and the map is on the default adapter; the recorded
           adapter is then dropped, so TryGetRenderAdapter returns false. Never throws. */
        internal static void VerifyCurrentDisplay()
        {
            try
            {
                IntPtr display;
                string name;
                lock (_lock)
                {
                    if (!_checkDisplay)
                        return;
                    display = _display;
                    name = _renderName;
                }
                if (eglGetCurrentDisplay() == display)
                    WriteLog("Windows GPU check: SkiaSharp uses the pre-initialized display on "
                        + (name ?? "unknown adapter"));
                else
                {
                    Record(OutcomeNotUsed, false, 0, null, IntPtr.Zero, false);
                    WriteLog("Windows GPU check: SkiaSharp uses a different display; "
                        + "the map is on the default adapter");
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex);
            }
        }

        /* Records the outcome and returns the log line without its time */
        private static string Select()
        {
            string switchText = Environment.GetEnvironmentVariable(WinGpuVariable);
            GHWindowsGpuMode mode;
            bool unknownSwitch;
            if (!GHWindowsGpuChoice.ShouldPreinitialize(switchText, out mode, out unknownSwitch))
            {
                Record(OutcomeDefaultSwitch, false, 0, null, IntPtr.Zero, false);
                return "Windows GPU not selected, mode default, " + OutcomeDefaultSwitch;
            }

            GHWindowsGpuPreference preference = GHWindowsGpuChoice.ParseActiveGpu(GHApp.GetActiveGPU());
            long? highPerformanceLuid, minimumPowerLuid;
            GHDxgiAdapters.TryGetPreferenceLuids(out highPerformanceLuid, out minimumPowerLuid);
            long? wantedLuid = GHWindowsGpuChoice.WantedLuid(mode, preference, highPerformanceLuid, minimumPowerLuid);
            List<GHRenderAdapter> adapters = new List<GHRenderAdapter>();
            GHDxgiAdapters.Enumerate(adapters, SnapshotDeviceGpus());
            GHRenderAdapter first = adapters.Count > 0 ? adapters[0] : null;
            GHRenderAdapter wanted = wantedLuid.HasValue ? FindAdapter(adapters, wantedLuid.Value) : null;

            string context = ", mode " + GHWindowsGpuChoice.ModeName(mode)
                + (unknownSwitch ? " (unknown " + WinGpuVariable + " \"" + switchText.Trim() + "\")" : "")
                + ", Windows setting " + GHWindowsGpuChoice.PreferenceName(preference);

            /* An unknown wanted adapter, Windows' first one or a software one: SkiaSharp's own choice */
            if (wanted == null || wanted.IsSoftware || first.Luid == wanted.Luid)
            {
                if (first != null)
                    Record(OutcomeWindowsDefault, true, first.Luid, first.Name, IntPtr.Zero, false);
                else
                    Record(OutcomeWindowsDefault, false, 0, null, IntPtr.Zero, false);
                return "Windows GPU " + AdapterText(first) + context + ", " + OutcomeWindowsDefault;
            }

            int[] attributes = CreateDisplayAttributes(wanted.Luid);
            IntPtr display = eglGetPlatformDisplayEXT(EGL_PLATFORM_ANGLE_ANGLE, IntPtr.Zero, attributes);
            if (display == IntPtr.Zero)
            {
                string failure = OutcomeFailedPrefix + "no display, EGL error " + FormatEglError(eglGetError());
                Record(failure, false, 0, null, IntPtr.Zero, false);
                return "Windows GPU " + AdapterText(wanted) + context + ", " + failure;
            }
            int major, minor;
            if (eglInitialize(display, out major, out minor) == EGL_FALSE)
            {
                /* The display stays uninitialized, and SkiaSharp's call initializes it with its own attributes */
                string failure = OutcomeFailedPrefix + "eglInitialize, EGL error " + FormatEglError(eglGetError());
                Record(failure, false, 0, null, IntPtr.Zero, false);
                return "Windows GPU " + AdapterText(wanted) + context + ", " + failure;
            }

            long usedLuid;
            if (!TryReadDisplayAdapterLuid(display, out usedLuid))
            {
                Record(OutcomeAdapterUnknown, false, 0, null, IntPtr.Zero, false);
                return "Windows GPU " + AdapterText(wanted) + context + ", " + OutcomeAdapterUnknown;
            }
            if (usedLuid == wanted.Luid)
            {
                Record(OutcomePreinitialized, true, wanted.Luid, wanted.Name, display, true);
                return "Windows GPU " + AdapterText(wanted) + context + ", " + OutcomePreinitialized;
            }

            /* A GL view initialized the display first, so ANGLE ignored the LUID */
            GHRenderAdapter used = FindAdapter(adapters, usedLuid);
            Record(OutcomeAlreadyInitialized, true, usedLuid, used != null ? used.Name : null, display, false);
            string usedText = used != null ? AdapterText(used) : "unknown adapter (luid " + FormatLuid(usedLuid) + ")";
            return "Windows GPU " + usedText + context + ", " + OutcomeAlreadyInitialized
                + ", wanted " + AdapterText(wanted);
        }

        /* SkiaSharp.Views.WinUI's GlesContext.defaultDisplayAttributes (GlesInterop/GlesContext.cs, the same in
           SkiaSharp 3.119.4 and 4.154.0) with the LUID pair added. Apart from that pair the list must stay identical to
           SkiaSharp's: ANGLE finds an existing display by some of these attributes, a difference there gives SkiaSharp
           a display of its own on the default adapter, and for the rest the first eglInitialize decides. */
        private static int[] CreateDisplayAttributes(long luid)
        {
            return new int[]
            {
                EGL_PLATFORM_ANGLE_TYPE_ANGLE, EGL_PLATFORM_ANGLE_TYPE_D3D11_ANGLE,
                EGL_PLATFORM_ANGLE_D3D_LUID_HIGH_ANGLE, unchecked((int)(luid >> 32)),
                EGL_PLATFORM_ANGLE_D3D_LUID_LOW_ANGLE, unchecked((int)luid),
                EGL_EXPERIMENTAL_PRESENT_PATH_ANGLE, EGL_EXPERIMENTAL_PRESENT_PATH_COPY_ANGLE,
                EGL_PLATFORM_ANGLE_ENABLE_AUTOMATIC_TRIM_ANGLE, EGL_TRUE,
                EGL_NONE,
            };
        }

        /* The LUID of the adapter under the display's D3D11 device. ANGLE owns that device and returns it without a
           reference, so it is never released. */
        private static bool TryReadDisplayAdapterLuid(IntPtr display, out long luid)
        {
            luid = 0;
            try
            {
                IntPtr device;
                if (eglQueryDisplayAttribEXT(display, EGL_DEVICE_EXT, out device) == EGL_FALSE || device == IntPtr.Zero)
                    return false;
                IntPtr d3d11Device;
                if (eglQueryDeviceAttribEXT(device, EGL_D3D11_DEVICE_ANGLE, out d3d11Device) == EGL_FALSE
                    || d3d11Device == IntPtr.Zero)
                    return false;
                return GHDxgiAdapters.TryGetDeviceAdapterLuid(d3d11Device, out luid);
            }
            catch
            {
                luid = 0;
                return false;
            }
        }

        private static void Record(string outcome, bool hasRenderAdapter, long renderLuid, string renderName,
            IntPtr display, bool checkDisplay)
        {
            lock (_lock)
            {
                _outcome = outcome;
                _hasRenderAdapter = hasRenderAdapter;
                _renderLuid = renderLuid;
                _renderName = renderName;
                _display = display;
                _checkDisplay = checkDisplay;
            }
        }

        private static GHRenderAdapter FindAdapter(List<GHRenderAdapter> adapters, long luid)
        {
            for (int i = 0; i < adapters.Count; i++)
            {
                if (adapters[i] != null && adapters[i].Luid == luid)
                    return adapters[i];
            }
            return null;
        }

        /* "<name> (luid 0x<HighPart>_0x<LowPart>)"; "unknown adapter" for null */
        private static string AdapterText(GHRenderAdapter adapter)
        {
            if (adapter == null)
                return "unknown adapter";
            string name = string.IsNullOrWhiteSpace(adapter.Name) ? "unknown adapter" : adapter.Name;
            return name + " (luid " + FormatLuid(adapter.Luid) + ")";
        }

        /* "0x<HighPart>_0x<LowPart>", as PDH writes it */
        private static string FormatLuid(long luid)
        {
            uint high = unchecked((uint)(luid >> 32));
            uint low = unchecked((uint)luid);
            return "0x" + high.ToString("X8", CultureInfo.InvariantCulture)
                + "_0x" + low.ToString("X8", CultureInfo.InvariantCulture);
        }

        private static string FormatEglError(int error)
        {
            return "0x" + error.ToString("X4", CultureInfo.InvariantCulture);
        }

        private static void WriteLog(string line)
        {
            try
            {
                GHApp.MaybeWriteGHLog(line, addBreadcrumb: true, category: LogCategory);
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex);
            }
        }

        /* GHApp.DeviceGPUs has no lock; it is filled once, from the main menu, and may still be empty */
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
