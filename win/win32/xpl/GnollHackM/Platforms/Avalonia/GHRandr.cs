#if LINUX
using System;
using System.Runtime.InteropServices;

namespace GnollHackM
{
    // WORKAROUND(WA-AVALONIA-0015): Avalonia's Screen reports no refresh rate
    /* X11 RandR lookup of the CRTC that shows a point of the root window, and the refresh rate of its
       current mode. It uses its own display connection, opened once and kept for the process, never
       Avalonia's. Inert once libX11.so.6 or libXrandr.so.2 fails to load or no X display opens.
       UI thread only. */
    internal static unsafe class GHRandr
    {
        private const string LibX11 = "libX11.so.6";
        private const string LibXrandr = "libXrandr.so.2";

        private const ulong RR_Interlace = 0x10;
        private const ulong RR_DoubleScan = 0x20;

        /* Layouts of Xrandr.h for 64-bit Linux, as in Avalonia.X11's X11Structs.cs; libXrandr fills them */
#pragma warning disable CS0649
        internal unsafe struct XRRScreenResources
        {
            public long timestamp;
            public long configTimestamp;
            public int ncrtc;
            public IntPtr* crtcs;
            public int noutput;
            public IntPtr* outputs;
            public int nmode;
            public XRRModeInfo* modes;
        }

        internal unsafe struct XRRModeInfo
        {
            public IntPtr id;
            public uint width;
            public uint height;
            public ulong dotClock;
            public uint hSyncStart;
            public uint hSyncEnd;
            public uint hTotal;
            public uint hSkew;
            public uint vSyncStart;
            public uint vSyncEnd;
            public uint vTotal;
            public byte* name;
            public uint nameLength;
            public ulong modeFlags;     /* RR_Interlace 0x10, RR_DoubleScan 0x20 */
        }

        internal unsafe struct XRRCrtcInfo
        {
            public ulong timestamp;
            public int x, y;
            public uint width, height;
            public IntPtr mode;
            public ushort rotation;
            public int noutput;
            public IntPtr* outputs;
            public ushort rotations;
            public int npossible;
            public IntPtr* possible;
        }
#pragma warning restore CS0649

        [DllImport(LibX11)]
        private static extern IntPtr XOpenDisplay(IntPtr display);

        [DllImport(LibX11)]
        private static extern IntPtr XDefaultRootWindow(IntPtr display);

        [DllImport(LibXrandr)]
        private static extern XRRScreenResources* XRRGetScreenResourcesCurrent(IntPtr dpy, IntPtr window);

        [DllImport(LibXrandr)]
        private static extern void XRRFreeScreenResources(XRRScreenResources* resources);

        [DllImport(LibXrandr)]
        private static extern XRRCrtcInfo* XRRGetCrtcInfo(IntPtr dpy, XRRScreenResources* resources, IntPtr crtc);

        [DllImport(LibXrandr)]
        private static extern void XRRFreeCrtcInfo(XRRCrtcInfo* crtcInfo);

        /* Set once a library or entry point fails to load, or XOpenDisplay fails */
        private static bool _unavailable = false;

        /* The connection and its root window; kept for the process */
        private static IntPtr _display = IntPtr.Zero;
        private static IntPtr _root = IntPtr.Zero;

        /* The first CRTC with a mode whose rectangle contains the point; IntPtr.Zero when none */
        public static IntPtr FindCrtc(int x, int y)
        {
            if (!EnsureDisplay())
                return IntPtr.Zero;

            XRRScreenResources* resources = null;
            try
            {
                resources = XRRGetScreenResourcesCurrent(_display, _root);
                if (resources == null)
                    return IntPtr.Zero;

                int i;
                for (i = 0; i < resources->ncrtc; i++)
                {
                    IntPtr crtc = resources->crtcs[i];
                    XRRCrtcInfo* info = XRRGetCrtcInfo(_display, resources, crtc);
                    if (info == null)
                        continue;
                    try
                    {
                        if (info->mode != IntPtr.Zero
                            && x >= info->x && x < info->x + (long)info->width
                            && y >= info->y && y < info->y + (long)info->height)
                            return crtc;
                    }
                    finally
                    {
                        XRRFreeCrtcInfo(info);
                    }
                }
                return IntPtr.Zero;
            }
            catch (Exception ex) when (ex is DllNotFoundException || ex is EntryPointNotFoundException)
            {
                SetUnavailable(ex.Message);
                return IntPtr.Zero;
            }
            finally
            {
                if (resources != null)
                    XRRFreeScreenResources(resources);
            }
        }

        /* In whole Hz, from the CRTC's current mode; 0 when unknown */
        public static int GetRefreshRate(IntPtr crtc)
        {
            if (crtc == IntPtr.Zero || !EnsureDisplay())
                return 0;

            XRRScreenResources* resources = null;
            try
            {
                resources = XRRGetScreenResourcesCurrent(_display, _root);
                if (resources == null)
                    return 0;

                /* A CRTC that no longer exists would raise an X error */
                bool found = false;
                int i;
                for (i = 0; i < resources->ncrtc; i++)
                {
                    if (resources->crtcs[i] == crtc)
                    {
                        found = true;
                        break;
                    }
                }
                if (!found)
                    return 0;

                IntPtr mode = IntPtr.Zero;
                XRRCrtcInfo* info = XRRGetCrtcInfo(_display, resources, crtc);
                if (info == null)
                    return 0;
                try
                {
                    mode = info->mode;
                }
                finally
                {
                    XRRFreeCrtcInfo(info);
                }
                if (mode == IntPtr.Zero)
                    return 0;

                for (i = 0; i < resources->nmode; i++)
                {
                    XRRModeInfo* modeInfo = &resources->modes[i];
                    if (modeInfo->id == mode)
                    {
                        return GHRefreshRateEstimator.ModeRefreshRate(modeInfo->dotClock, modeInfo->hTotal, modeInfo->vTotal,
                            (modeInfo->modeFlags & RR_Interlace) != 0, (modeInfo->modeFlags & RR_DoubleScan) != 0);
                    }
                }
                return 0;
            }
            catch (Exception ex) when (ex is DllNotFoundException || ex is EntryPointNotFoundException)
            {
                SetUnavailable(ex.Message);
                return 0;
            }
            finally
            {
                if (resources != null)
                    XRRFreeScreenResources(resources);
            }
        }

        private static bool EnsureDisplay()
        {
            if (_unavailable)
                return false;
            if (_display != IntPtr.Zero)
                return true;

            try
            {
                IntPtr display = XOpenDisplay(IntPtr.Zero);
                if (display == IntPtr.Zero)
                {
                    SetUnavailable("XOpenDisplay failed");
                    return false;
                }
                _root = XDefaultRootWindow(display);
                _display = display;
                return true;
            }
            catch (Exception ex) when (ex is DllNotFoundException || ex is EntryPointNotFoundException)
            {
                SetUnavailable(ex.Message);
                return false;
            }
        }

        private static void SetUnavailable(string reason)
        {
            if (_unavailable)
                return;
            _unavailable = true;
            GnollHackX.GHApp.MaybeWriteGHLog("Avalonia display: X11 RandR is not available: " + reason);
        }
    }
}
#endif
