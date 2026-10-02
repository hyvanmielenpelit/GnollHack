using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;

namespace GnollHackX
{
    /* The parts of a replay file name:
       <prefix>[<Player>-][T<firstTurn>-]<versionSuffix>-<timestamp>[_<N>].gnhrec[.gz|.zip] */
    public sealed class ReplayFileNameInfo
    {
        public bool IsContinuation { get; }
        /* Null when the name has no player part */
        public string PlayerName { get; }
        /* -1 when the name has no turn part */
        public int FirstTurn { get; }
        /* For example "430-21" */
        public string VersionSuffix { get; }
        /* (ulong)DateTime.ToBinary() of the game's replay time stamp */
        public ulong TimeStamp { get; }
        /* 0 for a main file */
        public int Continuation { get; }
        /* "", ".gz" or ".zip" */
        public string CompressionSuffix { get; }

        public ReplayFileNameInfo(bool isContinuation, string playerName, int firstTurn, string versionSuffix,
            ulong timeStamp, int continuation, string compressionSuffix)
        {
            IsContinuation = isContinuation;
            PlayerName = playerName;
            FirstTurn = firstTurn;
            VersionSuffix = versionSuffix;
            TimeStamp = timeStamp;
            Continuation = continuation;
            CompressionSuffix = compressionSuffix;
        }
    }

    /* Replay file name parsing, next-session lookup, and glyph table record scanning.
       Pure functions of plain inputs: no GHApp, MAUI or Xamarin types. Must compile
       under C# 7.3 (the legacy netstandard2.0 project). */
    public static class GHReplayFiles
    {
        /* These mirror GHConstants, which the unit test project does not compile */
        private const string ReplayFileNamePrefix = "replay-";
        private const string ReplayContinuationFileNamePrefix = "rpcont-";
        private const char ReplayFileNameMiddleDivisor = '-';
        private const char ReplayFileContinuationNumberDivisor = '_';
        private const string ReplayFileNameSuffix = ".gnhrec";
        private const string ReplayZipFileNameSuffix = ".zip";
        private const string ReplayGZipFileNameSuffix = ".gz";

        /* These mirror RecordedFunctionID.IssueGuiCommand and gui_command_types.GUI_CMD_LOAD_GLYPHS */
        private const byte IssueGuiCommandRecordId = 70;
        private const int LoadGlyphsCommandId = 2;

        private const int MinGlyphCount = 10000;
        private const int MaxGlyphCount = 1000000;
        private const int MaxTileValue = 1000000;
        private const int SignatureLength = 5;
        private const int ScanChunkSize = 64 * 1024;

        /* A next session file must be time-stamped at most this long after the previous session's exit */
        public const int NextSessionWindowSeconds = 3600;

        public static bool TryParseReplayFileName(string fileName, out ReplayFileNameInfo info)
        {
            info = null;
            try
            {
                if (string.IsNullOrEmpty(fileName))
                    return false;

                string name = fileName;
                int slash = Math.Max(name.LastIndexOf('/'), name.LastIndexOf('\\'));
                if (slash >= 0)
                    name = name.Substring(slash + 1);

                string compression = "";
                if (name.EndsWith(ReplayGZipFileNameSuffix, StringComparison.Ordinal))
                    compression = ReplayGZipFileNameSuffix;
                else if (name.EndsWith(ReplayZipFileNameSuffix, StringComparison.Ordinal))
                    compression = ReplayZipFileNameSuffix;
                name = name.Substring(0, name.Length - compression.Length);

                if (!name.EndsWith(ReplayFileNameSuffix, StringComparison.Ordinal))
                    return false;
                name = name.Substring(0, name.Length - ReplayFileNameSuffix.Length);

                bool isContinuation;
                if (name.StartsWith(ReplayFileNamePrefix, StringComparison.Ordinal))
                {
                    isContinuation = false;
                    name = name.Substring(ReplayFileNamePrefix.Length);
                }
                else if (name.StartsWith(ReplayContinuationFileNamePrefix, StringComparison.Ordinal))
                {
                    isContinuation = true;
                    name = name.Substring(ReplayContinuationFileNamePrefix.Length);
                }
                else
                {
                    return false;
                }

                int continuation = 0;
                if (isContinuation)
                {
                    int divisor = name.LastIndexOf(ReplayFileContinuationNumberDivisor);
                    if (divisor < 0)
                        return false;
                    string contStr = name.Substring(divisor + 1);
                    if (!IsAllDigits(contStr)
                        || !int.TryParse(contStr, NumberStyles.None, CultureInfo.InvariantCulture, out continuation))
                        return false;
                    name = name.Substring(0, divisor);
                }

                string[] parts = name.Split(ReplayFileNameMiddleDivisor);
                int count = parts.Length;
                if (count < 3)
                    return false;

                string timeStampStr = parts[count - 1];
                ulong timeStamp;
                if (!IsAllDigits(timeStampStr)
                    || !ulong.TryParse(timeStampStr, NumberStyles.None, CultureInfo.InvariantCulture, out timeStamp))
                    return false;

                string versionMajor = parts[count - 3];
                string versionEdit = parts[count - 2];
                if (!IsAllDigits(versionMajor) || !IsAllDigits(versionEdit))
                    return false;
                string versionSuffix = versionMajor + ReplayFileNameMiddleDivisor + versionEdit;

                int remaining = count - 3;
                int firstTurn = -1;
                if (remaining > 0)
                {
                    string turnStr = parts[remaining - 1];
                    int turn;
                    if (turnStr.Length > 1 && turnStr[0] == 'T' && IsAllDigits(turnStr.Substring(1))
                        && int.TryParse(turnStr.Substring(1), NumberStyles.None, CultureInfo.InvariantCulture, out turn))
                    {
                        firstTurn = turn;
                        remaining--;
                    }
                }

                string playerName = remaining > 0 ? string.Join(ReplayFileNameMiddleDivisor.ToString(), parts, 0, remaining) : null;
                if (playerName != null && playerName.Length == 0)
                    playerName = null;

                info = new ReplayFileNameInfo(isContinuation, playerName, firstTurn, versionSuffix, timeStamp, continuation, compression);
                return true;
            }
            catch (Exception)
            {
                info = null;
                return false;
            }
        }

        /* The main replay file that continues a session which exited at exitTimeBinary on lastTurn:
           same compression, version and (when playerName is non-null) player, a first turn within
           one of lastTurn, and a time stamp in (exit, exit + NextSessionWindowSeconds]. Returns the
           earliest such name as given, or null. */
        public static string FindNextSessionFile(IEnumerable<string> fileNames, string playerName, string versionSuffix,
            string compressionSuffix, long exitTimeBinary, int lastTurn)
        {
            if (fileNames == null)
                return null;

            long exitTicks;
            try
            {
                exitTicks = ToComparableTicks(DateTime.FromBinary(exitTimeBinary));
            }
            catch (ArgumentException)
            {
                return null;
            }
            long windowEndTicks = exitTicks + TimeSpan.TicksPerSecond * NextSessionWindowSeconds;

            string best = null;
            long bestTicks = long.MaxValue;
            foreach (string fileName in fileNames)
            {
                ReplayFileNameInfo info;
                if (!TryParseReplayFileName(fileName, out info) || info.IsContinuation)
                    continue;
                if (!string.Equals(info.CompressionSuffix, compressionSuffix ?? "", StringComparison.Ordinal))
                    continue;
                if (!string.Equals(info.VersionSuffix, versionSuffix, StringComparison.Ordinal))
                    continue;
                if (playerName != null && !string.Equals(info.PlayerName, playerName, StringComparison.Ordinal))
                    continue;
                if (info.FirstTurn < 0 || Math.Abs((long)info.FirstTurn - lastTurn) > 1)
                    continue;

                long ticks;
                try
                {
                    ticks = ToComparableTicks(DateTime.FromBinary(unchecked((long)info.TimeStamp)));
                }
                catch (ArgumentException)
                {
                    continue;
                }
                if (ticks <= exitTicks || ticks > windowEndTicks)
                    continue;
                if (best == null || ticks < bestTicks)
                {
                    best = fileName;
                    bestTicks = ticks;
                }
            }
            return best;
        }

        /* Scans stream from fromPosition for the first validated GUI_CMD_LOAD_GLYPHS record and
           returns its tables. The stream must be readable and seekable; its position afterwards
           is unspecified. */
        public static bool TryFindGlyphTableRecord(Stream stream, long fromPosition, out int[] glyph2Tile, out byte[] glyphTileFlags)
        {
            glyph2Tile = null;
            glyphTileFlags = null;
            try
            {
                if (stream == null || !stream.CanRead || !stream.CanSeek || fromPosition < 0)
                    return false;

                byte[] buffer = new byte[ScanChunkSize];
                long chunkStart = fromPosition;
                while (true)
                {
                    stream.Position = chunkStart;
                    int bytesRead = ReadFully(stream, buffer, 0, buffer.Length);
                    if (bytesRead < SignatureLength)
                        return false;

                    int lastCandidate = bytesRead - SignatureLength;
                    int i;
                    for (i = 0; i <= lastCandidate; i++)
                    {
                        if (buffer[i] == IssueGuiCommandRecordId
                            && ReadInt32(buffer, i + 1) == LoadGlyphsCommandId
                            && TryReadGlyphTables(stream, chunkStart + i + SignatureLength, out glyph2Tile, out glyphTileFlags))
                            return true;
                    }

                    /* The last SignatureLength - 1 positions are rescanned with the next chunk */
                    chunkStart += lastCandidate + 1;
                }
            }
            catch (Exception)
            {
                glyph2Tile = null;
                glyphTileFlags = null;
                return false;
            }
        }

        /* True when the bytes at position start a GUI_CMD_LOAD_GLYPHS record; the stream position is restored */
        public static bool IsGlyphTableRecordAt(Stream stream, long position)
        {
            if (stream == null || !stream.CanRead || !stream.CanSeek || position < 0)
                return false;

            long savedPosition = stream.Position;
            try
            {
                byte[] buffer = new byte[SignatureLength];
                stream.Position = position;
                if (ReadFully(stream, buffer, 0, SignatureLength) < SignatureLength)
                    return false;
                return buffer[0] == IssueGuiCommandRecordId && ReadInt32(buffer, 1) == LoadGlyphsCommandId;
            }
            catch (Exception)
            {
                return false;
            }
            finally
            {
                try
                {
                    stream.Position = savedPosition;
                }
                catch (Exception)
                {
                    /* Nothing more can be done for a stream that refuses to seek back */
                }
            }
        }

        /* Reads and validates the glyph tables following a LOAD_GLYPHS signature at position */
        private static bool TryReadGlyphTables(Stream stream, long position, out int[] glyph2Tile, out byte[] glyphTileFlags)
        {
            glyph2Tile = null;
            glyphTileFlags = null;

            byte[] intBuffer = new byte[4];
            stream.Position = position;
            if (ReadFully(stream, intBuffer, 0, 4) < 4)
                return false;
            int n = ReadInt32(intBuffer, 0);
            if (n < MinGlyphCount || n > MaxGlyphCount)
                return false;

            /* Glyph values, flag count and flags must all fit in the rest of the stream */
            long needed = 4L * n + 4 + n;
            if (stream.Length - stream.Position < needed)
                return false;

            byte[] valueBytes = new byte[4 * n];
            if (ReadFully(stream, valueBytes, 0, valueBytes.Length) < valueBytes.Length)
                return false;
            int[] values = new int[n];
            int j;
            for (j = 0; j < n; j++)
            {
                int value = ReadInt32(valueBytes, 4 * j);
                if (value < 0 || value >= MaxTileValue)
                    return false;
                values[j] = value;
            }

            if (ReadFully(stream, intBuffer, 0, 4) < 4)
                return false;
            int m = ReadInt32(intBuffer, 0);
            if (m != n)
                return false;

            byte[] flags = new byte[m];
            if (ReadFully(stream, flags, 0, m) < m)
                return false;

            glyph2Tile = values;
            glyphTileFlags = flags;
            return true;
        }

        /* Little-endian, as BinaryWriter writes it */
        private static int ReadInt32(byte[] buffer, int offset)
        {
            return buffer[offset] | (buffer[offset + 1] << 8) | (buffer[offset + 2] << 16) | (buffer[offset + 3] << 24);
        }

        /* Reads until count bytes or the end of the stream; returns the number of bytes read */
        private static int ReadFully(Stream stream, byte[] buffer, int offset, int count)
        {
            int total = 0;
            while (total < count)
            {
                int read = stream.Read(buffer, offset + total, count - total);
                if (read <= 0)
                    break;
                total += read;
            }
            return total;
        }

        private static bool IsAllDigits(string s)
        {
            if (string.IsNullOrEmpty(s))
                return false;
            int i;
            for (i = 0; i < s.Length; i++)
            {
                if (s[i] < '0' || s[i] > '9')
                    return false;
            }
            return true;
        }

        /* Local times compare as UTC, so the two DateTimes being compared agree regardless of
           the time zone; Utc and Unspecified ticks are taken as they are */
        private static long ToComparableTicks(DateTime time)
        {
            return time.Kind == DateTimeKind.Local ? time.ToUniversalTime().Ticks : time.Ticks;
        }
    }
}
