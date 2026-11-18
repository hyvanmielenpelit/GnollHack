using System;
using GnollHackM;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers GHWindowsCodePage on Windows: representability in explicit code pages, independent of the machine's
       own code page, the path check and the ASCII test. Each test returns at once elsewhere. */
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
