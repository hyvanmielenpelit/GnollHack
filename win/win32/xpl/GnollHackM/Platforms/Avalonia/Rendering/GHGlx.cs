#if LINUX
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace GnollHackM
{
    /* GLX swap interval and present timing for the drawable Avalonia's render thread has current.
       Called on the render thread only. Every entry point first checks that a GLX context is
       current (non-zero glXGetCurrentDisplay), so it is inert under the software renderer or EGL. */
    internal static class GHGlx
    {
        private const string LibGL = "libGL.so.1";
        private const string LibX11 = "libX11.so.6";

        public const int GLX_SWAP_INTERVAL_EXT = 0x20F1;

        [DllImport(LibGL)]
        private static extern IntPtr glXGetProcAddressARB([MarshalAs(UnmanagedType.LPStr)] string procName);

        [DllImport(LibGL)]
        private static extern IntPtr glXGetCurrentDisplay();

        [DllImport(LibGL)]
        private static extern IntPtr glXGetCurrentDrawable();

        [DllImport(LibGL)]
        private static extern void glXQueryDrawable(IntPtr display, IntPtr drawable, int attribute, out uint value);

        [DllImport(LibGL)]
        private static extern IntPtr glXQueryExtensionsString(IntPtr display, int screen);

        [DllImport(LibX11)]
        private static extern int XDefaultScreen(IntPtr display);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void SwapIntervalEXTProc(IntPtr display, IntPtr drawable, int interval);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate int SwapIntervalMESAProc(uint interval);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate int GetSwapIntervalMESAProc();

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate int GetSyncValuesOMLProc(IntPtr display, IntPtr drawable, out long ust, out long msc, out long sbc);

        /* Set once libGL.so.1 or one of its entry points fails to load */
        private static bool _unavailable = false;

        /* Extension support and entry points, for the display they were read on */
        private static IntPtr _extensionsDisplay = IntPtr.Zero;
        private static bool _hasExtSwapControl = false;
        private static bool _hasMesaSwapControl = false;
        private static bool _hasOmlSyncControl = false;
        private static SwapIntervalEXTProc _swapIntervalEXT = null;
        private static SwapIntervalMESAProc _swapIntervalMESA = null;
        private static GetSwapIntervalMESAProc _getSwapIntervalMESA = null;
        private static GetSyncValuesOMLProc _getSyncValuesOML = null;

        /* The current GLX display and drawable; false when no GLX context is current */
        public static bool TryGetCurrent(out IntPtr display, out IntPtr drawable)
        {
            display = IntPtr.Zero;
            drawable = IntPtr.Zero;
            if (_unavailable)
                return false;

            try
            {
                display = glXGetCurrentDisplay();
                if (display == IntPtr.Zero)
                    return false;
                drawable = glXGetCurrentDrawable();
                return drawable != IntPtr.Zero;
            }
            catch (Exception ex) when (ex is DllNotFoundException || ex is EntryPointNotFoundException)
            {
                _unavailable = true;
                display = IntPtr.Zero;
                drawable = IntPtr.Zero;
                return false;
            }
        }

        /* Sets swap interval 1 on the current drawable and reads it back. NotGlx when no GLX context
           is current, Rejected when no swap-control extension exists or the read-back is not 1. */
        public static GHSwapIntervalState ApplySwapInterval(out IntPtr drawable, out string detail)
        {
            IntPtr display;
            if (!TryGetCurrent(out display, out drawable))
            {
                detail = _unavailable ? "libGL.so.1 not available" : "no current GLX context";
                return GHSwapIntervalState.NotGlx;
            }

            try
            {
                EnsureExtensions(display);

                if (_hasExtSwapControl && _swapIntervalEXT != null)
                {
                    _swapIntervalEXT(display, drawable, 1);
                    uint value;
                    glXQueryDrawable(display, drawable, GLX_SWAP_INTERVAL_EXT, out value);
                    detail = "glXSwapIntervalEXT(1), read back " + value.ToString();
                    return value == 1 ? GHSwapIntervalState.Applied : GHSwapIntervalState.Rejected;
                }

                if (_hasMesaSwapControl && _swapIntervalMESA != null)
                {
                    int result = _swapIntervalMESA(1);
                    int value = _getSwapIntervalMESA != null ? _getSwapIntervalMESA() : (result == 0 ? 1 : 0);
                    detail = "glXSwapIntervalMESA(1) returned " + result.ToString() + ", read back " + value.ToString();
                    return result == 0 && value == 1 ? GHSwapIntervalState.Applied : GHSwapIntervalState.Rejected;
                }

                detail = "neither GLX_EXT_swap_control nor GLX_MESA_swap_control is supported";
                return GHSwapIntervalState.Rejected;
            }
            catch (Exception ex)
            {
                detail = "swap interval failed: " + ex.Message;
                return GHSwapIntervalState.Rejected;
            }
        }

        /* UST, MSC and SBC of the current drawable (GLX_OML_sync_control); false when unavailable */
        public static bool TryGetSyncValues(out long ust, out long msc, out long sbc)
        {
            ust = 0;
            msc = 0;
            sbc = 0;
            IntPtr display, drawable;
            if (!TryGetCurrent(out display, out drawable))
                return false;

            try
            {
                EnsureExtensions(display);
                if (!_hasOmlSyncControl || _getSyncValuesOML == null)
                    return false;

                return _getSyncValuesOML(display, drawable, out ust, out msc, out sbc) != 0;
            }
            catch (Exception)
            {
                return false;
            }
        }

        private static void EnsureExtensions(IntPtr display)
        {
            if (display == _extensionsDisplay)
                return;

            _extensionsDisplay = display;
            HashSet<string> extensions = new HashSet<string>(StringComparer.Ordinal);
            IntPtr extensionString = glXQueryExtensionsString(display, XDefaultScreen(display));
            string text = extensionString != IntPtr.Zero ? Marshal.PtrToStringAnsi(extensionString) : null;
            if (!string.IsNullOrEmpty(text))
            {
                foreach (string name in text.Split(' ', StringSplitOptions.RemoveEmptyEntries))
                    extensions.Add(name);
            }

            _hasExtSwapControl = extensions.Contains("GLX_EXT_swap_control");
            _hasMesaSwapControl = extensions.Contains("GLX_MESA_swap_control");
            _hasOmlSyncControl = extensions.Contains("GLX_OML_sync_control");
            _swapIntervalEXT = _hasExtSwapControl ? GetProc<SwapIntervalEXTProc>("glXSwapIntervalEXT") : null;
            _swapIntervalMESA = _hasMesaSwapControl ? GetProc<SwapIntervalMESAProc>("glXSwapIntervalMESA") : null;
            _getSwapIntervalMESA = _hasMesaSwapControl ? GetProc<GetSwapIntervalMESAProc>("glXGetSwapIntervalMESA") : null;
            _getSyncValuesOML = _hasOmlSyncControl ? GetProc<GetSyncValuesOMLProc>("glXGetSyncValuesOML") : null;
        }

        private static T GetProc<T>(string name) where T : Delegate
        {
            IntPtr proc = glXGetProcAddressARB(name);
            return proc != IntPtr.Zero ? Marshal.GetDelegateForFunctionPointer<T>(proc) : null;
        }
    }
}
#endif
