using System;
using System.IO;
using System.Threading.Tasks;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers GHBonesFiles: the bones file name grammar, path containment, the non-overwriting
       write, and log formatting of a received name. */
    public class GHBonesFilesTests
    {
        [Theory]
        [InlineData("bon-2D0.15")]
        [InlineData("bon-0D0.2")]
        [InlineData("bon-6G0.15")]
        [InlineData("bon3-1QBar.2")]
        [InlineData("bon-1QArc.L")]
        [InlineData("bon9-4QVal.3")]
        [InlineData("bon-0M0.T")]
        [InlineData("bon-2D0.O")]
        [InlineData("bon-3K0.K")]
        [InlineData("bon9-6QArc.26")]
        public void IsValidBonesFileName_AcceptsCoreNames(string name)
        {
            Assert.True(GHBonesFiles.IsValidBonesFileName(name));
        }

        [Theory]
        [InlineData(null)]
        [InlineData("")]
        [InlineData("bon")]
        [InlineData("bon-2D0")]
        [InlineData("bon-2D0.")]
        [InlineData("bon-7D0.1")]
        [InlineData("bon10-2D0.1")]
        [InlineData("bon-2D0.0")]
        [InlineData("bon-2D0.07")]
        [InlineData("bon-2D0.123")]
        [InlineData("bon-2d0.1")]
        [InlineData("bon-2QBAR.1")]
        [InlineData("bon-2D0.t")]
        [InlineData("BON-2D0.1")]
        [InlineData(@"..\bon-2D0.1")]
        [InlineData("../bon-2D0.1")]
        [InlineData(@"sub\bon-2D0.1")]
        [InlineData("sub/bon-2D0.1")]
        [InlineData(@"C:\bon-2D0.1")]
        [InlineData("/bon-2D0.1")]
        [InlineData("bon-2D0.1\n")]
        [InlineData(" bon-2D0.1")]
        [InlineData("bon-2D0.1.tmp")]
        [InlineData("record")]
        [InlineData("sysconf")]
        [InlineData("logfile")]
        [InlineData("bonnie.gsv")]
        public void IsValidBonesFileName_RejectsOtherNames(string name)
        {
            Assert.False(GHBonesFiles.IsValidBonesFileName(name));
        }

        [Fact]
        public void IsValidBonesFileName_RejectsLongName()
        {
            Assert.False(GHBonesFiles.IsValidBonesFileName("bon-2D0.1" + new string('a', 10000)));
        }

        [Fact]
        public void GetSavePathInside_ValidName_ReturnsFullPathInDirectory()
        {
            string dir = Path.GetTempPath();
            Assert.Equal(Path.GetFullPath(Path.Combine(dir, "bon-2D0.15")), GHBonesFiles.GetSavePathInside(dir, "bon-2D0.15"));
        }

        [Fact]
        public void GetSavePathInside_InvalidName_ReturnsNull()
        {
            string dir = Path.GetTempPath();
            Assert.Null(GHBonesFiles.GetSavePathInside(dir, @"..\bon-2D0.15"));
            Assert.Null(GHBonesFiles.GetSavePathInside(dir, "sysconf"));
        }

        [Fact]
        public void GetSavePathInside_TrailingSeparator_GivesSameResult()
        {
            string withSeparator = Path.GetTempPath();
            string withoutSeparator = withSeparator.TrimEnd(Path.DirectorySeparatorChar, Path.AltDirectorySeparatorChar);
            Assert.NotEqual(withSeparator, withoutSeparator);
            Assert.Equal(GHBonesFiles.GetSavePathInside(withSeparator, "bon-2D0.15"),
                GHBonesFiles.GetSavePathInside(withoutSeparator, "bon-2D0.15"));
        }

        [Fact]
        public void GetSavePathInside_NullOrEmptyDirectory_ReturnsNull()
        {
            Assert.Null(GHBonesFiles.GetSavePathInside(null, "bon-2D0.15"));
            Assert.Null(GHBonesFiles.GetSavePathInside("", "bon-2D0.15"));
        }

        [Fact]
        public async Task WriteNewFileAsync_NewPath_WritesBytesAndLeavesNoOtherFile()
        {
            string dir = CreateTempDirectory();
            try
            {
                string path = Path.Combine(dir, "bon-2D0.15");
                await GHBonesFiles.WriteNewFileAsync(path, new byte[] { 1, 2, 3 });
                Assert.Equal(new byte[] { 1, 2, 3 }, File.ReadAllBytes(path));
                Assert.Single(Directory.GetFiles(dir));
            }
            finally
            {
                Directory.Delete(dir, true);
            }
        }

        [Fact]
        public async Task WriteNewFileAsync_ExistingPath_ThrowsAndKeepsFile()
        {
            string dir = CreateTempDirectory();
            try
            {
                string path = Path.Combine(dir, "bon-2D0.15");
                File.WriteAllBytes(path, new byte[] { 9 });
                await Assert.ThrowsAnyAsync<IOException>(() => GHBonesFiles.WriteNewFileAsync(path, new byte[] { 1, 2, 3 }));
                Assert.Equal(new byte[] { 9 }, File.ReadAllBytes(path));
                Assert.Empty(Directory.GetFiles(dir, "*.tmp"));
            }
            finally
            {
                Directory.Delete(dir, true);
            }
        }

        [Fact]
        public async Task WriteNewFileAsync_NullArguments_Throw()
        {
            await Assert.ThrowsAsync<ArgumentNullException>(() => GHBonesFiles.WriteNewFileAsync(null, new byte[1]));
            await Assert.ThrowsAsync<ArgumentNullException>(() => GHBonesFiles.WriteNewFileAsync("bon-2D0.15", null));
        }

        [Fact]
        public void FormatForLog_Null_GivesPlaceholder()
        {
            Assert.Equal("(null)", GHBonesFiles.FormatForLog(null));
        }

        [Fact]
        public void FormatForLog_ControlCharacters_BecomeQuestionMarks()
        {
            string text = new string(new[] { 'a', (char)13, (char)10, 'b', (char)9, 'c', (char)0x7F });
            Assert.Equal("a??b?c?", GHBonesFiles.FormatForLog(text));
        }

        [Fact]
        public void FormatForLog_LongText_IsCut()
        {
            string text64 = new string('x', 64);
            Assert.Equal(text64, GHBonesFiles.FormatForLog(text64));
            Assert.Equal(text64 + "...", GHBonesFiles.FormatForLog(text64 + "y"));
        }

        private static string CreateTempDirectory()
        {
            string dir = Path.Combine(Path.GetTempPath(), "GHBonesFilesTests-" + Guid.NewGuid().ToString("N"));
            Directory.CreateDirectory(dir);
            return dir;
        }
    }
}
