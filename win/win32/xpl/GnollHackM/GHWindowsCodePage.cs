#if !ANDROID && !IOS
using System;
using System.Globalization;
using System.Runtime.InteropServices;
using Microsoft.Win32;

namespace GnollHackM
{
    /* The process ANSI code page, which the narrow C runtime uses for every path the engine opens. The
       manifests' activeCodePage makes it UTF-8 on Windows 10 1903 and later. */
    public static class GHWindowsCodePage
    {
        public const uint Utf8 = 65001;
        private const uint WC_NO_BEST_FIT_CHARS = 0x00000400;

        [DllImport("kernel32.dll")]
        private static extern uint GetACP();

        [DllImport("kernel32.dll")]
        private static extern int WideCharToMultiByte(uint codePage, uint flags,
            [MarshalAs(UnmanagedType.LPWStr)] string wideText, int wideLength, byte[] multiByteText,
            int multiByteLength, IntPtr defaultChar, out int usedDefaultChar);

        /* Windows only */
        public static uint AnsiCodePage => GetACP();

        private static readonly Lazy<uint> _systemAnsiCodePage = new Lazy<uint>(ReadSystemAnsiCodePage);

        /* The system's ANSI code page, which processes without activeCodePage use; 0 when it cannot be read.
           Read once: it changes only at a restart. */
        public static uint SystemAnsiCodePage => _systemAnsiCodePage.Value;

        private static uint ReadSystemAnsiCodePage()
        {
            if (!OperatingSystem.IsWindows())
                return 0;
            try
            {
                using (RegistryKey key = Registry.LocalMachine.OpenSubKey(@"SYSTEM\CurrentControlSet\Control\Nls\CodePage"))
                {
                    string value = key != null ? key.GetValue("ACP") as string : null;
                    uint codePage;
                    return value != null && uint.TryParse(value, NumberStyles.None, CultureInfo.InvariantCulture, out codePage)
                        ? codePage : 0;
                }
            }
            catch (Exception)
            {
                return 0;
            }
        }

        /* "<process> (S <system>)", or "<process>" when the system code page is unknown */
        public static string FormatCodePages(uint processCodePage, uint systemCodePage)
        {
            string text = processCodePage.ToString(CultureInfo.InvariantCulture);
            if (systemCodePage != 0)
                text += " (S " + systemCodePage.ToString(CultureInfo.InvariantCulture) + ")";
            return text;
        }

        /* Whether text survives conversion to codePage unchanged: no default character, no best-fit
           substitute. UTF-8 represents everything, and takes neither argument. Windows only. */
        public static bool IsRepresentable(string text, uint codePage)
        {
            if (string.IsNullOrEmpty(text) || codePage == Utf8)
                return true;
            byte[] buffer = new byte[text.Length * 4 + 4];
            int usedDefaultChar;
            int written = WideCharToMultiByte(codePage, WC_NO_BEST_FIT_CHARS, text, text.Length, buffer,
                buffer.Length, IntPtr.Zero, out usedDefaultChar);
            return written > 0 && usedDefaultChar == 0;
        }

        /* Whether LPStr marshaling hands the C runtime this exact path. Windows only. */
        public static bool CanMarshalPath(string path)
        {
            return IsRepresentable(path, AnsiCodePage);
        }

        public static bool IsAscii(string text)
        {
            if (text == null)
                return true;
            foreach (char c in text)
            {
                if (c > 0x7F)
                    return false;
            }
            return true;
        }
    }
}
#endif
