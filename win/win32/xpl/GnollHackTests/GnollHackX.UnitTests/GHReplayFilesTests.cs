using System;
using System.Collections.Generic;
using System.IO;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers GHReplayFiles: replay file name parsing, the next-session file lookup, and the
       glyph table record scan and peek. */
    public class GHReplayFilesTests
    {
        private const string T1Name = "replay-Janne-T1-430-21-9862637582834625281.gnhrec.gz";
        private const string T313Name = "replay-Janne-T313-430-21-9862637584386188026.gnhrec.gz";
        private const string T786Name = "replay-Janne-T786-430-21-9862637611196089353.gnhrec.gz";
        private const string T1ContName = "rpcont-Janne-T1-430-21-9862637582834625281_1.gnhrec.gz";
        private const ulong T313TimeStamp = 9862637584386188026UL;

        private static readonly string[] RealNames = { T1Name, T313Name, T786Name, T1ContName };

        /* ---- Name parsing ---- */

        [Theory]
        [InlineData(T1Name, false, "Janne", 1, 9862637582834625281UL, 0, ".gz")]
        [InlineData(T313Name, false, "Janne", 313, 9862637584386188026UL, 0, ".gz")]
        [InlineData(T786Name, false, "Janne", 786, 9862637611196089353UL, 0, ".gz")]
        [InlineData(T1ContName, true, "Janne", 1, 9862637582834625281UL, 1, ".gz")]
        [InlineData("replay-430-21-9862637582834625281.gnhrec", false, null, -1, 9862637582834625281UL, 0, "")]
        [InlineData("replay-Mary-Ann-T5-430-21-123.gnhrec.zip", false, "Mary-Ann", 5, 123UL, 0, ".zip")]
        [InlineData("replay-Mary-Ann-430-21-123.gnhrec", false, "Mary-Ann", -1, 123UL, 0, "")]
        [InlineData("replay-T7-430-21-123.gnhrec", false, null, 7, 123UL, 0, "")]
        [InlineData("rpcont-Janne-T1-430-21-123_12.gnhrec", true, "Janne", 1, 123UL, 12, "")]
        [InlineData("rpcont-430-21-123_3.gnhrec.zip", true, null, -1, 123UL, 3, ".zip")]
        public void Parse_ValidNames(string fileName, bool isContinuation, string playerName, int firstTurn,
            ulong timeStamp, int continuation, string compression)
        {
            ReplayFileNameInfo info;
            Assert.True(GHReplayFiles.TryParseReplayFileName(fileName, out info));
            Assert.Equal(isContinuation, info.IsContinuation);
            Assert.Equal(playerName, info.PlayerName);
            Assert.Equal(firstTurn, info.FirstTurn);
            Assert.Equal("430-21", info.VersionSuffix);
            Assert.Equal(timeStamp, info.TimeStamp);
            Assert.Equal(continuation, info.Continuation);
            Assert.Equal(compression, info.CompressionSuffix);
        }

        [Theory]
        [InlineData(@"C:\Users\x\replay\" + T313Name)]
        [InlineData("/data/user/0/replay/" + T313Name)]
        public void Parse_UsesOnlyTheFileNamePartOfAPath(string path)
        {
            ReplayFileNameInfo info;
            Assert.True(GHReplayFiles.TryParseReplayFileName(path, out info));
            Assert.Equal("Janne", info.PlayerName);
            Assert.Equal(313, info.FirstTurn);
            Assert.Equal(T313TimeStamp, info.TimeStamp);
        }

        [Theory]
        [InlineData(null)]
        [InlineData("")]
        [InlineData("foo.txt")]
        [InlineData(".gnhrec")]
        [InlineData("replay-.gnhrec")]
        [InlineData("Janne-T1-430-21-123.gnhrec")]
        [InlineData("save-Janne-T1-430-21-123.gnhrec")]
        [InlineData("replay-Janne-T1-430-21-12x3.gnhrec")]
        [InlineData("replay-Janne-T1-430-21-.gnhrec")]
        [InlineData("replay-Janne-T1-123.gnhrec")]
        [InlineData("replay-123.gnhrec")]
        [InlineData("replay-Janne-T1-4a0-21-123.gnhrec")]
        [InlineData("replay-Janne-T1-430-21-99999999999999999999999.gnhrec")]
        [InlineData("replay-Janne-T1-430-21-123_1.gnhrec")]
        [InlineData("rpcont-Janne-T1-430-21-123.gnhrec")]
        [InlineData("rpcont-Janne-T1-430-21-123_.gnhrec")]
        [InlineData("rpcont-Janne-T1-430-21-123_x.gnhrec")]
        [InlineData("replay-Janne-T1-430-21-123.gnhrec.tar")]
        [InlineData("replay-430--123.gnhrec")]
        [InlineData("replay-1234567-9223372036854775807.gnhrec")]
        [InlineData("replay-1234567--9223372036854775807.gnhrec")]
        public void Parse_InvalidNames_ReturnFalse(string fileName)
        {
            ReplayFileNameInfo info;
            Assert.False(GHReplayFiles.TryParseReplayFileName(fileName, out info));
            Assert.Null(info);
        }

        /* ---- Next session lookup ---- */

        private static DateTime T313Time
        {
            get { return DateTime.FromBinary(unchecked((long)T313TimeStamp)); }
        }

        private static long DefaultExit
        {
            get { return (T313Time - TimeSpan.FromMilliseconds(340)).ToBinary(); }
        }

        private static string MakeName(string prefix, string player, int turn, DateTime time, string compression)
        {
            return prefix + (player != null ? player + "-" : "") + (turn >= 0 ? "T" + turn + "-" : "")
                + "430-21-" + ((ulong)time.ToBinary()).ToString() + ".gnhrec" + compression;
        }

        [Fact]
        public void Next_RealNames_FindsT313()
        {
            Assert.Equal(T313Name, GHReplayFiles.FindNextSessionFile(RealNames, "Janne", "430-21", ".gz", DefaultExit, 313));
        }

        [Fact]
        public void Next_ReturnsTheNameAsGiven()
        {
            string[] paths = { @"C:\r\" + T1Name, @"C:\r\" + T313Name, @"C:\r\" + T786Name };
            Assert.Equal(@"C:\r\" + T313Name, GHReplayFiles.FindNextSessionFile(paths, "Janne", "430-21", ".gz", DefaultExit, 313));
        }

        [Theory]
        [InlineData(312)]
        [InlineData(314)]
        public void Next_AcceptsATurnOffByOne(int lastTurn)
        {
            Assert.Equal(T313Name, GHReplayFiles.FindNextSessionFile(RealNames, "Janne", "430-21", ".gz", DefaultExit, lastTurn));
        }

        [Fact]
        public void Next_OtherPlayer_ReturnsNull()
        {
            Assert.Null(GHReplayFiles.FindNextSessionFile(RealNames, "Other", "430-21", ".gz", DefaultExit, 313));
        }

        [Fact]
        public void Next_OtherVersion_ReturnsNull()
        {
            Assert.Null(GHReplayFiles.FindNextSessionFile(RealNames, "Janne", "430-20", ".gz", DefaultExit, 313));
        }

        [Theory]
        [InlineData(".zip")]
        [InlineData("")]
        [InlineData(null)]
        public void Next_OtherCompression_ReturnsNull(string compression)
        {
            Assert.Null(GHReplayFiles.FindNextSessionFile(RealNames, "Janne", "430-21", compression, DefaultExit, 313));
        }

        [Fact]
        public void Next_OtherTurn_ReturnsNull()
        {
            Assert.Null(GHReplayFiles.FindNextSessionFile(RealNames, "Janne", "430-21", ".gz", DefaultExit, 400));
        }

        [Fact]
        public void Next_ExitTwoHoursEarlier_ReturnsNull()
        {
            long exit = (T313Time - TimeSpan.FromHours(2)).ToBinary();
            Assert.Null(GHReplayFiles.FindNextSessionFile(RealNames, "Janne", "430-21", ".gz", exit, 313));
        }

        [Fact]
        public void Next_ExitAfterTheFile_ReturnsNull()
        {
            long exit = (T313Time + TimeSpan.FromSeconds(1)).ToBinary();
            Assert.Null(GHReplayFiles.FindNextSessionFile(RealNames, "Janne", "430-21", ".gz", exit, 313));
        }

        [Fact]
        public void Next_ExitEqualToTheFile_ReturnsNull()
        {
            Assert.Null(GHReplayFiles.FindNextSessionFile(RealNames, "Janne", "430-21", ".gz", T313Time.ToBinary(), 313));
        }

        [Fact]
        public void Next_WindowEndIsInclusive()
        {
            DateTime exit = DateTime.Now;
            string atEnd = MakeName("replay-", "Janne", 10, exit.AddSeconds(GHReplayFiles.NextSessionWindowSeconds), ".gz");
            string pastEnd = MakeName("replay-", "Janne", 10, exit.AddSeconds(GHReplayFiles.NextSessionWindowSeconds + 1), ".gz");
            Assert.Equal(atEnd, GHReplayFiles.FindNextSessionFile(new[] { atEnd }, "Janne", "430-21", ".gz", exit.ToBinary(), 10));
            Assert.Null(GHReplayFiles.FindNextSessionFile(new[] { pastEnd }, "Janne", "430-21", ".gz", exit.ToBinary(), 10));
        }

        [Fact]
        public void Next_TwoCandidates_ReturnsTheEarlier()
        {
            DateTime exit = T313Time - TimeSpan.FromMilliseconds(340);
            string later = MakeName("replay-", "Janne", 314, T313Time.AddSeconds(10), ".gz");
            string[] names = { later, T786Name, T313Name, T1Name };
            Assert.Equal(T313Name, GHReplayFiles.FindNextSessionFile(names, "Janne", "430-21", ".gz", exit.ToBinary(), 313));
        }

        [Fact]
        public void Next_ContinuationFiles_AreNeverReturned()
        {
            string cont = MakeName("rpcont-", "Janne", 313, T313Time, ".gz").Replace(".gnhrec", "_1.gnhrec");
            ReplayFileNameInfo info;
            Assert.True(GHReplayFiles.TryParseReplayFileName(cont, out info));
            Assert.True(info.IsContinuation);
            Assert.Null(GHReplayFiles.FindNextSessionFile(new[] { cont, T1ContName }, "Janne", "430-21", ".gz", DefaultExit, 313));
        }

        [Fact]
        public void Next_NullPlayer_DoesNotComparePlayers()
        {
            string other = MakeName("replay-", "Other", 313, T313Time, ".gz");
            string none = MakeName("replay-", null, 313, T313Time.AddSeconds(1), ".gz");
            Assert.Equal(other, GHReplayFiles.FindNextSessionFile(new[] { other }, null, "430-21", ".gz", DefaultExit, 313));
            Assert.Equal(none, GHReplayFiles.FindNextSessionFile(new[] { none }, null, "430-21", ".gz", DefaultExit, 313));
            Assert.Null(GHReplayFiles.FindNextSessionFile(new[] { none }, "Janne", "430-21", ".gz", DefaultExit, 313));
        }

        [Fact]
        public void Next_NameWithoutTurn_IsNotReturned()
        {
            string noTurn = MakeName("replay-", "Janne", -1, T313Time, ".gz");
            Assert.Null(GHReplayFiles.FindNextSessionFile(new[] { noTurn }, "Janne", "430-21", ".gz", DefaultExit, 313));
        }

        [Fact]
        public void Next_NullOrGarbageInput_ReturnsNull()
        {
            Assert.Null(GHReplayFiles.FindNextSessionFile(null, "Janne", "430-21", ".gz", DefaultExit, 313));
            Assert.Null(GHReplayFiles.FindNextSessionFile(new[] { null, "", "foo.txt" }, "Janne", "430-21", ".gz", DefaultExit, 313));
        }

        /* ---- Glyph table record ---- */

        private static void WriteRecord(BinaryWriter writer, int[] values, byte[] flags)
        {
            writer.Write((byte)70);
            writer.Write(2);
            writer.Write(values.Length);
            foreach (int v in values)
                writer.Write(v);
            writer.Write(flags.Length);
            writer.Write(flags);
        }

        private static int[] MakeValues(int n, int offset)
        {
            int[] values = new int[n];
            int i;
            for (i = 0; i < n; i++)
                values[i] = (i + offset) % 24220;
            return values;
        }

        private static byte[] MakeFlags(int n)
        {
            byte[] flags = new byte[n];
            int i;
            for (i = 0; i < n; i++)
                flags[i] = (byte)(i % 7);
            return flags;
        }

        private static void WriteJunk(BinaryWriter writer, int count)
        {
            int i;
            for (i = 0; i < count; i++)
                writer.Write((byte)((i * 37 + 11) % 251));
        }

        private static MemoryStream BuildStreamWithDecoys(out long validRecordPosition)
        {
            MemoryStream ms = new MemoryStream();
            BinaryWriter writer = new BinaryWriter(ms);
            WriteJunk(writer, 1000);

            /* A signature with an implausible glyph count */
            writer.Write((byte)70);
            writer.Write(2);
            writer.Write(5);
            WriteJunk(writer, 100);

            /* A plausible count with an out-of-range value */
            int[] bad = MakeValues(10000, 0);
            bad[5000] = 2000000;
            WriteRecord(writer, bad, MakeFlags(10000));
            WriteJunk(writer, 100);

            /* A flag count that differs from the glyph count */
            writer.Write((byte)70);
            writer.Write(2);
            writer.Write(10000);
            foreach (int v in MakeValues(10000, 0))
                writer.Write(v);
            writer.Write(9999);
            writer.Write(MakeFlags(9999));
            WriteJunk(writer, 100);

            validRecordPosition = ms.Position;
            WriteRecord(writer, MakeValues(12000, 0), MakeFlags(12000));
            WriteJunk(writer, 500);
            writer.Flush();
            return ms;
        }

        [Fact]
        public void Glyph_SkipsDecoysAndFindsTheValidRecord()
        {
            long validPosition;
            using (MemoryStream ms = BuildStreamWithDecoys(out validPosition))
            {
                int[] glyph2Tile;
                byte[] flags;
                Assert.True(GHReplayFiles.TryFindGlyphTableRecord(ms, 0, out glyph2Tile, out flags));
                Assert.Equal(MakeValues(12000, 0), glyph2Tile);
                Assert.Equal(MakeFlags(12000), flags);
            }
        }

        [Fact]
        public void Glyph_FromPosition_FindsTheLaterRecord()
        {
            using (MemoryStream ms = new MemoryStream())
            {
                BinaryWriter writer = new BinaryWriter(ms);
                WriteRecord(writer, MakeValues(11000, 0), MakeFlags(11000));
                long second = ms.Position;
                WriteRecord(writer, MakeValues(11000, 100), MakeFlags(11000));
                writer.Flush();

                int[] glyph2Tile;
                byte[] flags;
                Assert.True(GHReplayFiles.TryFindGlyphTableRecord(ms, 0, out glyph2Tile, out flags));
                Assert.Equal(MakeValues(11000, 0), glyph2Tile);
                Assert.True(GHReplayFiles.TryFindGlyphTableRecord(ms, 1, out glyph2Tile, out flags));
                Assert.Equal(MakeValues(11000, 100), glyph2Tile);
                Assert.True(GHReplayFiles.TryFindGlyphTableRecord(ms, second, out glyph2Tile, out flags));
                Assert.Equal(MakeValues(11000, 100), glyph2Tile);
            }
        }

        [Fact]
        public void Glyph_NoValidRecord_ReturnsFalse()
        {
            using (MemoryStream ms = new MemoryStream())
            {
                BinaryWriter writer = new BinaryWriter(ms);
                WriteJunk(writer, 200000);
                writer.Write((byte)70);
                writer.Write(2);
                writer.Write(5);
                writer.Flush();

                int[] glyph2Tile;
                byte[] flags;
                Assert.False(GHReplayFiles.TryFindGlyphTableRecord(ms, 0, out glyph2Tile, out flags));
                Assert.Null(glyph2Tile);
                Assert.Null(flags);
            }
        }

        [Theory]
        [InlineData(65530)]
        [InlineData(65531)]
        [InlineData(65532)]
        [InlineData(65533)]
        [InlineData(65534)]
        [InlineData(65535)]
        [InlineData(65536)]
        [InlineData(131067)]
        public void Glyph_RecordAcrossChunkBoundary_IsFound(int offset)
        {
            using (MemoryStream ms = new MemoryStream())
            {
                BinaryWriter writer = new BinaryWriter(ms);
                writer.Write(new byte[offset]);
                WriteRecord(writer, MakeValues(12000, 0), MakeFlags(12000));
                writer.Flush();

                int[] glyph2Tile;
                byte[] flags;
                Assert.True(GHReplayFiles.TryFindGlyphTableRecord(ms, 0, out glyph2Tile, out flags));
                Assert.Equal(MakeValues(12000, 0), glyph2Tile);
                Assert.Equal(MakeFlags(12000), flags);
                Assert.True(GHReplayFiles.IsGlyphTableRecordAt(ms, offset));
            }
        }

        [Theory]
        [InlineData(1)]
        [InlineData(100)]
        [InlineData(12000 + 4)]
        [InlineData(12000 + 4 + 48000)]
        public void Glyph_TruncatedRecord_ReturnsFalse(int bytesCut)
        {
            using (MemoryStream full = new MemoryStream())
            {
                BinaryWriter writer = new BinaryWriter(full);
                WriteJunk(writer, 300);
                WriteRecord(writer, MakeValues(12000, 0), MakeFlags(12000));
                writer.Flush();

                byte[] bytes = full.ToArray();
                using (MemoryStream ms = new MemoryStream(bytes, 0, bytes.Length - bytesCut))
                {
                    int[] glyph2Tile;
                    byte[] flags;
                    Assert.False(GHReplayFiles.TryFindGlyphTableRecord(ms, 0, out glyph2Tile, out flags));
                    Assert.Null(glyph2Tile);
                    Assert.Null(flags);
                }
            }
        }

        [Fact]
        public void Glyph_NullOrEmptyStream_ReturnsFalse()
        {
            int[] glyph2Tile;
            byte[] flags;
            Assert.False(GHReplayFiles.TryFindGlyphTableRecord(null, 0, out glyph2Tile, out flags));
            using (MemoryStream ms = new MemoryStream())
            {
                Assert.False(GHReplayFiles.TryFindGlyphTableRecord(ms, 0, out glyph2Tile, out flags));
                Assert.False(GHReplayFiles.TryFindGlyphTableRecord(ms, 100, out glyph2Tile, out flags));
            }
        }

        [Fact]
        public void IsGlyphTableRecordAt_PeeksAndRestoresThePosition()
        {
            long validPosition;
            using (MemoryStream ms = BuildStreamWithDecoys(out validPosition))
            {
                ms.Position = 3;
                Assert.True(GHReplayFiles.IsGlyphTableRecordAt(ms, validPosition));
                Assert.Equal(3, ms.Position);
                Assert.False(GHReplayFiles.IsGlyphTableRecordAt(ms, validPosition + 1));
                Assert.Equal(3, ms.Position);
                Assert.False(GHReplayFiles.IsGlyphTableRecordAt(ms, 0));
                Assert.False(GHReplayFiles.IsGlyphTableRecordAt(ms, ms.Length - 2));
                Assert.False(GHReplayFiles.IsGlyphTableRecordAt(ms, ms.Length + 10));
                Assert.Equal(3, ms.Position);
                Assert.False(GHReplayFiles.IsGlyphTableRecordAt(null, 0));
            }
        }
    }
}
