using System;
using GnollHackM;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers GHWindowsCodePage: representability in explicit code pages, independent of the machine's own code
       page, the path check, the ASCII test, the system code page read and the code page text. The tests that call
       Windows return at once elsewhere. */
    public class GHWindowsCodePageTests
    {
        private const string DataPath = "C:\\Users\\x\\AppData\\Local\\Hyv\u00e4n mielen pelit ry\\Data";
        private const string Japanese = "\u65e5\u672c";

        [Theory]
        [InlineData(DataPath, 932u, false)]
        [InlineData(DataPath, 1251u, false)]
        [InlineData(DataPath, 1252u, true)]
        [InlineData(DataPath, 65001u, true)]
        [InlineData(Japanese, 932u, true)]
        [InlineData(Japanese, 1252u, false)]
        [InlineData("abc", 932u, true)]
        public void IsRepresentable_ExplicitCodePage(string text, uint codePage, bool expected)
        {
            if (!OperatingSystem.IsWindows())
                return;

            Assert.Equal(expected, GHWindowsCodePage.IsRepresentable(text, codePage));
        }

        [Fact]
        public void IsRepresentable_NullOrEmpty_IsTrue()
        {
            if (!OperatingSystem.IsWindows())
                return;

            Assert.True(GHWindowsCodePage.IsRepresentable(null, 932u));
            Assert.True(GHWindowsCodePage.IsRepresentable("", 932u));
        }

        [Fact]
        public void CanMarshalPath_AsciiPath_IsTrue()
        {
            if (!OperatingSystem.IsWindows())
                return;

            Assert.True(GHWindowsCodePage.CanMarshalPath("C:\\plain\\path"));
        }

        [Theory]
        [InlineData(65001u, 932u, "65001 (S 932)")]
        [InlineData(65001u, 65001u, "65001 (S 65001)")]
        [InlineData(1252u, 1252u, "1252 (S 1252)")]
        [InlineData(65001u, 0u, "65001")]
        public void FormatCodePages_ShowsSystemCodePageWhenKnown(uint processCodePage, uint systemCodePage, string expected)
        {
            Assert.Equal(expected, GHWindowsCodePage.FormatCodePages(processCodePage, systemCodePage));
        }

        [Fact]
        public void SystemAnsiCodePage_OnWindows_IsKnown()
        {
            if (!OperatingSystem.IsWindows())
                return;

            Assert.True(GHWindowsCodePage.SystemAnsiCodePage > 0);
        }

        [Theory]
        [InlineData("abc", true)]
        [InlineData("Hyv\u00e4n", false)]
        [InlineData(null, true)]
        public void IsAscii_DetectsNonAscii(string text, bool expected)
        {
            if (!OperatingSystem.IsWindows())
                return;

            Assert.Equal(expected, GHWindowsCodePage.IsAscii(text));
        }
    }
}
