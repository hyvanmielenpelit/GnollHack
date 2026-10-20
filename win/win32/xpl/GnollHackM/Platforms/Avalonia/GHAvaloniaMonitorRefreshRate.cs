#if GNH_AVALONIA
using System;
using System.Runtime.InteropServices;
using System.Threading;
using GnollHackX;

namespace GnollHackM
{
    /* The refresh rate of a monitor's current display mode, read with user32 since Avalonia's Screen does
       not report one. Windows only; elsewhere every call returns 0. UI thread. */
    public static class GHAvaloniaMonitorRefreshRate
    {
        private const uint MONITOR_DEFAULTTONEAREST = 2;
        private const int ENUM_CURRENT_SETTINGS = -1;

        private static int _failureLogged = 0;

        [StructLayout(LayoutKind.Sequential)]
        private struct RECT
        {
            public int left;
            public int top;
            public int right;
            public int bottom;
        }

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
        private struct MONITORINFOEXW
        {
            public int cbSize;
            public RECT rcMonitor;
            public RECT rcWork;
            public uint dwFlags;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 32)]
            public string szDevice;
        }

        /* The layout of DisplaySettingsHelper.DEVMODE, with UTF-16 strings */
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
        private struct DEVMODEW
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 32)]
            public string dmDeviceName;

            public ushort dmSpecVersion;
            public ushort dmDriverVersion;
            public ushort dmSize;
            public ushort dmDriverExtra;
            public uint dmFields;

            public int dmPositionX;
            public int dmPositionY;
            public uint dmDisplayOrientation;
            public uint dmDisplayFixedOutput;

            public short dmColor;
            public short dmDuplex;
            public short dmYResolution;
            public short dmTTOption;
            public short dmCollate;

            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 32)]
            public string dmFormName;

            public ushort dmLogPixels;
            public uint dmBitsPerPel;
            public uint dmPelsWidth;
            public uint dmPelsHeight;
            public uint dmDisplayFlags;
            public uint dmDisplayFrequency;
            public uint dmICMMethod;
            public uint dmICMIntent;
            public uint dmMediaType;
            public uint dmDitherType;
            public uint dmReserved1;
            public uint dmReserved2;
            public uint dmPanningWidth;
            public uint dmPanningHeight;
        }

        [DllImport("user32.dll", ExactSpelling = true)]
        private static extern IntPtr MonitorFromWindow(IntPtr hwnd, uint dwFlags);

        [DllImport("user32.dll", CharSet = CharSet.Unicode, ExactSpelling = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool GetMonitorInfoW(IntPtr hMonitor, ref MONITORINFOEXW lpmi);

        [DllImport("user32.dll", CharSet = CharSet.Unicode, ExactSpelling = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool EnumDisplaySettingsW(string lpszDeviceName, int iModeNum, ref DEVMODEW lpDevMode);

        /* The monitor nearest to the window; IntPtr.Zero when unknown */
        public static IntPtr GetMonitor(Avalonia.Controls.TopLevel topLevel)
        {
            if (topLevel == null || !OperatingSystem.IsWindows())
                return IntPtr.Zero;

            try
            {
                IntPtr hwnd = topLevel.TryGetPlatformHandle()?.Handle ?? IntPtr.Zero;
                if (hwnd == IntPtr.Zero)
                    return IntPtr.Zero;
                return MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
            }
            catch (Exception ex)
            {
                LogFailureOnce(ex);
                return IntPtr.Zero;
            }
        }

        /* In Hz; 0 when unknown, or when the mode leaves the rate to the hardware's default */
        public static int GetRefreshRate(IntPtr monitor)
        {
            if (monitor == IntPtr.Zero || !OperatingSystem.IsWindows())
                return 0;

            try
            {
                MONITORINFOEXW info = new MONITORINFOEXW();
                info.cbSize = Marshal.SizeOf<MONITORINFOEXW>();
                if (!GetMonitorInfoW(monitor, ref info))
                    return 0;

                DEVMODEW mode = new DEVMODEW();
                mode.dmSize = (ushort)Marshal.SizeOf<DEVMODEW>();
                if (!EnumDisplaySettingsW(info.szDevice, ENUM_CURRENT_SETTINGS, ref mode))
                    return 0;

                /* 0 and 1 stand for the hardware's default rate */
                return mode.dmDisplayFrequency > 1 ? (int)mode.dmDisplayFrequency : 0;
            }
            catch (Exception ex)
            {
                LogFailureOnce(ex);
                return 0;
            }
        }

        private static void LogFailureOnce(Exception ex)
        {
            if (Interlocked.Exchange(ref _failureLogged, 1) == 0)
                GHApp.MaybeWriteGHLog("Avalonia display: reading the monitor refresh rate failed: " + ex.Message);
        }
    }
}
#endif
