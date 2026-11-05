#if GNH_AVALONIA && LINUX
using System.Runtime.InteropServices;

namespace GnollHackM
{
    /* libc calls for the evdev gamepad backend (x86-64). Requests are unsigned long, never int, which would
       sign-extend; the variadic open and ioctl take only integer and pointer arguments, which the SysV ABI
       passes as for a fixed signature. errno comes from Marshal.GetLastPInvokeError. */
    internal static class GHEvdevNative
    {
        private const string LibC = "libc";

        [DllImport(LibC, EntryPoint = "open", SetLastError = true)]
        public static extern int Open([MarshalAs(UnmanagedType.LPStr)] string path, int flags);

        [DllImport(LibC, EntryPoint = "read", SetLastError = true)]
        public static extern nint Read(int fd, byte[] buffer, nuint count);

        [DllImport(LibC, EntryPoint = "close", SetLastError = true)]
        public static extern int Close(int fd);

        [DllImport(LibC, EntryPoint = "ioctl", SetLastError = true)]
        public static extern int Ioctl(int fd, nuint request, byte[] argument);

        [DllImport(LibC, EntryPoint = "ioctl", SetLastError = true)]
        public static extern int Ioctl(int fd, nuint request, ref int argument);

        [DllImport(LibC, EntryPoint = "inotify_init1", SetLastError = true)]
        public static extern int InotifyInit1(int flags);

        [DllImport(LibC, EntryPoint = "inotify_add_watch", SetLastError = true)]
        public static extern int InotifyAddWatch(int fd, [MarshalAs(UnmanagedType.LPStr)] string path, uint mask);

        public static int LastError
        {
            get { return Marshal.GetLastPInvokeError(); }
        }
    }
}
#endif
