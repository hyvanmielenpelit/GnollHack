using System;
using System.IO;
using GnollHackX.Performance;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers GHAtomicFile: the write replaces the target whole, leaves no temp file,
       keeps the previous content in the backup, the stale temp cleanup touches only old
       temp files, and the transient name test. Each test works in its own directory
       under the system temp directory. */
    public class GHAtomicFileTests : IDisposable
    {
        private readonly string _dir;

        public GHAtomicFileTests()
        {
            _dir = Path.Combine(Path.GetTempPath(), "GnollHackAtomicFileTests", Guid.NewGuid().ToString("N"));
            Directory.CreateDirectory(_dir);
        }

        public void Dispose()
        {
            try
            {
                Directory.Delete(_dir, true);
            }
            catch
            {
                /* Left for the system's temp cleanup */
            }
        }

        [Fact]
        public void AtomicWrite_CreatesNew()
        {
            string path = Path.Combine(_dir, "suite.json");

            GHAtomicFile.WriteAllText(path, "{\"a\":1}", null);

            Assert.Equal("{\"a\":1}", File.ReadAllText(path));
        }

        [Fact]
        public void AtomicWrite_ReplacesExisting()
        {
            string path = Path.Combine(_dir, "suite.json");
            File.WriteAllText(path, "old content that is longer than the new one");

            GHAtomicFile.WriteAllText(path, "new", null);

            Assert.Equal("new", File.ReadAllText(path));
        }

        [Fact]
        public void AtomicWrite_WritesUtf8WithoutBom()
        {
            string path = Path.Combine(_dir, "report.txt");

            GHAtomicFile.WriteAllText(path, ((char)0x00E4).ToString(), null);

            Assert.Equal(new byte[] { 0xC3, 0xA4 }, File.ReadAllBytes(path));
        }

        [Fact]
        public void AtomicWrite_LeavesNoTemp()
        {
            string path = Path.Combine(_dir, "baselines.json");
            string backup = path + GHAtomicFile.BackupSuffix;

            GHAtomicFile.WriteAllText(path, "one", backup);
            GHAtomicFile.WriteAllText(path, "two", backup);

            Assert.Empty(Directory.GetFiles(_dir, "*" + GHAtomicFile.TempSuffix));
            Assert.Equal(2, Directory.GetFiles(_dir).Length);
        }

        [Fact]
        public void AtomicWrite_BackupHoldsPrevious()
        {
            string path = Path.Combine(_dir, "baselines.json");
            string backup = path + GHAtomicFile.BackupSuffix;

            GHAtomicFile.WriteAllText(path, "first", backup);
            Assert.False(File.Exists(backup));

            GHAtomicFile.WriteAllText(path, "second", backup);
            Assert.Equal("second", File.ReadAllText(path));
            Assert.Equal("first", File.ReadAllText(backup));

            GHAtomicFile.WriteAllText(path, "third", backup);
            Assert.Equal("third", File.ReadAllText(path));
            Assert.Equal("second", File.ReadAllText(backup));
        }

        [Fact]
        public void AtomicWrite_NoBackupPath_WritesNoBackup()
        {
            string path = Path.Combine(_dir, "baselines.json");
            File.WriteAllText(path, "old");

            GHAtomicFile.WriteAllText(path, "new", null);

            Assert.False(File.Exists(path + GHAtomicFile.BackupSuffix));
        }

        [Fact]
        public void DeleteStaleTemps_OnlyOldTmp()
        {
            DateTime now = DateTime.UtcNow;
            string oldTmp = Path.Combine(_dir, "suite.json.0a1b2c3d" + GHAtomicFile.TempSuffix);
            string newTmp = Path.Combine(_dir, "suite.json.4e5f6a7b" + GHAtomicFile.TempSuffix);
            string oldJson = Path.Combine(_dir, "suite.json");
            string oldTmpLike = Path.Combine(_dir, "notes.tmpx");
            File.WriteAllText(oldTmp, "x");
            File.WriteAllText(newTmp, "x");
            File.WriteAllText(oldJson, "x");
            File.WriteAllText(oldTmpLike, "x");
            File.SetLastWriteTimeUtc(oldTmp, now - TimeSpan.FromHours(2));
            File.SetLastWriteTimeUtc(newTmp, now - TimeSpan.FromMinutes(5));
            File.SetLastWriteTimeUtc(oldJson, now - TimeSpan.FromHours(2));
            File.SetLastWriteTimeUtc(oldTmpLike, now - TimeSpan.FromHours(2));

            int deleted = GHAtomicFile.DeleteStaleTemps(_dir, now, TimeSpan.FromHours(1));

            Assert.Equal(1, deleted);
            Assert.False(File.Exists(oldTmp));
            Assert.True(File.Exists(newTmp));
            Assert.True(File.Exists(oldJson));
            Assert.True(File.Exists(oldTmpLike));
        }

        [Fact]
        public void DeleteStaleTemps_MissingDirectory_ReturnsZero()
        {
            Assert.Equal(0, GHAtomicFile.DeleteStaleTemps(Path.Combine(_dir, "missing"), DateTime.UtcNow,
                TimeSpan.FromHours(1)));
            Assert.Equal(0, GHAtomicFile.DeleteStaleTemps(null, DateTime.UtcNow, TimeSpan.FromHours(1)));
        }

        [Theory]
        [InlineData("suite.json.0a1b2c3d.tmp", true)]
        [InlineData("X.TMP", true)]
        [InlineData("baselines.json.bak", true)]
        [InlineData("baselines.json.corrupt-20260928T101500Z", true)]
        [InlineData("suite.json", false)]
        [InlineData("report.txt", false)]
        [InlineData("run_20260928_101500_000_idle_A.json", false)]
        [InlineData("frametimeline_run_20260928_101500_000_idle_A.csv", false)]
        [InlineData("notes.tmpx", false)]
        [InlineData("backup.json", false)]
        [InlineData("", false)]
        [InlineData(null, false)]
        public void IsTransientName_Cases(string fileName, bool expected)
        {
            Assert.Equal(expected, GHAtomicFile.IsTransientName(fileName));
        }
    }
}
