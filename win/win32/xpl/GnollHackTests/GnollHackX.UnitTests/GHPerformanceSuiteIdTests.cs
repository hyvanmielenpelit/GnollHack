using System.IO;
using GnollHackX.Performance;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers GHPerformanceSuiteId: the character and length rule, the manifest-to-folder
       match, and directory resolution, which must never leave the suites directory. */
    public class GHPerformanceSuiteIdTests
    {
        private const string GeneratedId = "20260927_101500_idle_Pixel-6a";

        private static string Root()
        {
            return Path.Combine(Path.GetTempPath(), "GnollHackSuiteIdTests", "suites");
        }

        [Theory]
        [InlineData(GeneratedId)]
        [InlineData(GeneratedId + "_2")]
        [InlineData("a")]
        [InlineData("Z9_-")]
        public void IsValid_AcceptsIdsOfTheAllowedCharacters(string id)
        {
            Assert.True(GHPerformanceSuiteId.IsValid(id));
        }

        [Fact]
        public void IsValid_AcceptsMaxLength()
        {
            Assert.True(GHPerformanceSuiteId.IsValid(new string('a', GHPerformanceSuiteId.MaxLength)));
        }

        [Theory]
        [InlineData(null)]
        [InlineData("")]
        [InlineData(".")]
        [InlineData("..")]
        [InlineData("../..")]
        [InlineData("a/b")]
        [InlineData("a\\b")]
        [InlineData("/abs")]
        [InlineData("C:\\x")]
        [InlineData("C:x")]
        [InlineData("a b")]
        [InlineData("a.b")]
        [InlineData("a:b")]
        [InlineData("\u00e4")]
        [InlineData("abc\n")]
        public void IsValid_RejectsUnsafeIds(string id)
        {
            Assert.False(GHPerformanceSuiteId.IsValid(id));
        }

        [Fact]
        public void IsValid_RejectsOverMaxLength()
        {
            Assert.False(GHPerformanceSuiteId.IsValid(new string('a', GHPerformanceSuiteId.MaxLength + 1)));
        }

        [Fact]
        public void MatchesFolder_TrueWhenEqualAndValid()
        {
            Assert.True(GHPerformanceSuiteId.MatchesFolder(GeneratedId, GeneratedId));
        }

        [Theory]
        [InlineData("20260927_101500_IDLE_Pixel-6a", GeneratedId)]
        [InlineData(null, GeneratedId)]
        [InlineData("../..", GeneratedId)]
        [InlineData("..", "..")]
        [InlineData("", "")]
        [InlineData(null, null)]
        public void MatchesFolder_FalseOtherwise(string manifestId, string folder)
        {
            Assert.False(GHPerformanceSuiteId.MatchesFolder(manifestId, folder));
        }

        [Fact]
        public void ResolveDirectory_CombinesAValidId()
        {
            string root = Root();
            Assert.Equal(Path.Combine(root, GeneratedId), GHPerformanceSuiteId.ResolveDirectory(root, GeneratedId));
        }

        [Fact]
        public void ResolveDirectory_AcceptsRootWithTrailingSeparator()
        {
            string root = Root() + Path.DirectorySeparatorChar;
            string resolved = GHPerformanceSuiteId.ResolveDirectory(root, GeneratedId);
            Assert.NotNull(resolved);
            Assert.Equal(Path.GetFullPath(Path.Combine(Root(), GeneratedId)), Path.GetFullPath(resolved));
        }

        [Theory]
        [InlineData(null)]
        [InlineData("")]
        [InlineData(".")]
        [InlineData("..")]
        [InlineData("../..")]
        [InlineData("a/b")]
        [InlineData("a\\b")]
        [InlineData("/abs")]
        [InlineData("C:\\x")]
        [InlineData("C:x")]
        public void ResolveDirectory_NullForUnsafeIds(string id)
        {
            Assert.Null(GHPerformanceSuiteId.ResolveDirectory(Root(), id));
        }

        [Theory]
        [InlineData(null)]
        [InlineData("")]
        public void ResolveDirectory_NullForMissingRoot(string root)
        {
            Assert.Null(GHPerformanceSuiteId.ResolveDirectory(root, GeneratedId));
        }
    }
}
