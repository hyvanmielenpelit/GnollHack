using Microsoft.UI.Input;
using Microsoft.UI.Xaml;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
#nullable enable

namespace GnollHackM.Platforms.Windows
{
    public static class CursorUtilities
    {
        public static void ChangeCursor(this UIElement element, InputCursor? cursor)
        {
            ArgumentNullException.ThrowIfNull(element);
            typeof(UIElement).InvokeMember("ProtectedCursor", BindingFlags.NonPublic | BindingFlags.Instance | BindingFlags.SetProperty, null, element, new[] { cursor });
        }

        public static InputCursor? LoadCursor(string filePath)
        {
            ArgumentNullException.ThrowIfNull(filePath);
            var hcursor = LoadCursorFromFileW(filePath);
            if (hcursor == 0)
                throw new Win32Exception(Marshal.GetLastWin32Error());

            return CreateCursorFromHCURSOR(hcursor);
        }

        public static InputCursor? CreateCursorFromHCURSOR(nint hcursor)
        {
            if (hcursor == 0)
                return null;

            const string classId = "Microsoft.UI.Input.InputCursor";
            _ = WindowsCreateString(classId, classId.Length, out var hs);
            _ = RoGetActivationFactory(hs, typeof(IActivationFactory).GUID, out var fac);
            _ = WindowsDeleteString(hs);
            if (fac is not IInputCursorStaticsInterop interop)
                return null;

            interop.CreateFromHCursor(hcursor, out var cursorAbi);
            if (cursorAbi == 0)
                return null;

            return WinRT.MarshalInspectable<InputCursor>.FromAbi(cursorAbi);
        }

        [ComImport, Guid("ac6f5065-90c4-46ce-beb7-05e138e54117"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
        private interface IInputCursorStaticsInterop
        {
            // IInspectable unused methods
            void GetIids();
            void GetRuntimeClassName();
            void GetTrustLevel();

            [PreserveSig]
            int CreateFromHCursor(nint hcursor, out nint inputCursor);
        }

        [ComImport, Guid("00000035-0000-0000-c000-000000000046"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
        private interface IActivationFactory
        {
            // IInspectable unused methods
            void GetIids();
            void GetRuntimeClassName();
            void GetTrustLevel();

            [PreserveSig]
            int ActivateInstance(out nint instance);
        }

        [DllImport("api-ms-win-core-winrt-l1-1-0.dll")]
        private static extern int RoGetActivationFactory(nint runtimeClassId, [MarshalAs(UnmanagedType.LPStruct)] Guid iid, out IActivationFactory factory);

        [DllImport("user32", CharSet = CharSet.Unicode, SetLastError = true)]
        private static extern nint LoadCursorFromFileW(string name);

        [DllImport("user32", CharSet = CharSet.Unicode)]
        private static extern bool DestroyCursor(nint hcursor);

        [DllImport("api-ms-win-core-winrt-string-l1-1-0", CharSet = CharSet.Unicode)]
        private static extern int WindowsCreateString(string? sourceString, int length, out nint @string);

        [DllImport("api-ms-win-core-winrt-string-l1-1-0", CharSet = CharSet.Unicode)]
        private static extern int WindowsDeleteString(nint @string);
    }
}


