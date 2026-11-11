using System.IO;
using GnollHackX.Performance;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers GHReadyToRunProbe: a ReadyToRun framework assembly, an IL-only assembly, and the
       inputs that must give null rather than throw. */
    public class GHReadyToRunProbeTests
    {
        [Fact]
        public void SharedFrameworkCoreLibIsFull()
        {
            Assert.Equal("Full", GHReadyToRunProbe.ModeOfFile(typeof(object).Assembly.Location));
        }

        [Fact]
        public void TestAssemblyIsNone()
        {
            Assert.Equal("None", GHReadyToRunProbe.ModeOfFile(typeof(GHReadyToRunProbeTests).Assembly.Location));
        }

        [Theory]
        [InlineData(null)]
        [InlineData("")]
        public void EmptyPathIsNull(string path)
        {
            Assert.Null(GHReadyToRunProbe.ModeOfFile(path));
        }

        [Fact]
        public void MissingFileIsNull()
        {
            string path = Path.Combine(Path.GetTempPath(), "GHReadyToRunProbeTests-missing-" + System.Guid.NewGuid().ToString("N") + ".dll");
            Assert.Null(GHReadyToRunProbe.ModeOfFile(path));
        }

        [Fact]
        public void NonPeFileIsNull()
        {
            string path = Path.GetTempFileName();
            try
            {
                File.WriteAllText(path, "not a portable executable");
                Assert.Null(GHReadyToRunProbe.ModeOfFile(path));
            }
            finally
            {
                File.Delete(path);
            }
        }
    }
}
