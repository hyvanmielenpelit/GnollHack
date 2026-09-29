using System;
using System.IO;
using System.Text;
using System.Threading;

namespace GnollHackX.Performance
{
    /* Whole-file writes that a reader never sees half done: the text goes to a temp file
       beside the target (<target>.<8 hex>.tmp), is flushed to disk, and then replaces the
       target in one move. An optional backup path receives the previous target first, by
       the same copy-and-move. The move is retried MoveRetries times, MoveRetryDelayMs
       apart, on IOException or UnauthorizedAccessException (a reader or a virus scanner
       holding the target); after the last failure the temp file is deleted and the
       exception rethrown.

       DeleteStaleTemps removes temp files a killed process left behind, and
       IsTransientName tells the names of temp, backup and set-aside corrupt files apart
       from real data, so they are not shared or imported.

       Pure: no GHApp, MAUI or Xamarin types. Must compile under C# 7.3 (the legacy
       netstandard2.0 project). */
    public static class GHAtomicFile
    {
        public const string TempSuffix = ".tmp";
        public const string BackupSuffix = ".bak";
        public const string CorruptMarker = ".corrupt-";

        private const int MoveRetries = 3;
        private const int MoveRetryDelayMs = 50;

        /* Writes text as UTF-8 without a BOM to path, replacing it in one step. When
           backupPath is not null and path exists, the previous path is first copied to
           backupPath, which is replaced in one step too. Throws on failure, leaving no
           temp file behind. */
        public static void WriteAllText(string path, string text, string backupPath)
        {
            string tmp = TempPathFor(path);
            try
            {
                byte[] bytes = new UTF8Encoding(false).GetBytes(text ?? "");
                using (FileStream fs = new FileStream(tmp, FileMode.CreateNew, FileAccess.Write, FileShare.None, 4096,
                    FileOptions.WriteThrough))
                {
                    fs.Write(bytes, 0, bytes.Length);
                    fs.Flush(true);
                }
                if (backupPath != null && File.Exists(path))
                    CopyAtomic(path, backupPath);
                MoveWithRetry(tmp, path);
            }
            catch
            {
                TryDelete(tmp);
                throw;
            }
        }

        /* Deletes the files in directory whose name ends in TempSuffix and whose last
           write is more than maxAge before nowUtc. Returns the number deleted; never
           throws. */
        public static int DeleteStaleTemps(string directory, DateTime nowUtc, TimeSpan maxAge)
        {
            int deleted = 0;
            try
            {
                if (string.IsNullOrEmpty(directory) || !Directory.Exists(directory))
                    return 0;
                string[] files = Directory.GetFiles(directory);
                for (int i = 0; i < files.Length; i++)
                {
                    try
                    {
                        if (!files[i].EndsWith(TempSuffix, StringComparison.OrdinalIgnoreCase))
                            continue;
                        if (nowUtc - File.GetLastWriteTimeUtc(files[i]) <= maxAge)
                            continue;
                        File.Delete(files[i]);
                        deleted++;
                    }
                    catch
                    {
                        /* Held open or already gone; the next cleanup tries again */
                    }
                }
            }
            catch
            {
                /* The directory vanished or cannot be listed */
            }
            return deleted;
        }

        /* True for a temp file (TempSuffix), a backup (BackupSuffix) or a set-aside
           corrupt file (containing CorruptMarker) */
        public static bool IsTransientName(string fileName)
        {
            if (string.IsNullOrEmpty(fileName))
                return false;
            return fileName.EndsWith(TempSuffix, StringComparison.OrdinalIgnoreCase)
                || fileName.EndsWith(BackupSuffix, StringComparison.OrdinalIgnoreCase)
                || fileName.IndexOf(CorruptMarker, StringComparison.OrdinalIgnoreCase) >= 0;
        }

        /* A new temp file name in path's directory */
        private static string TempPathFor(string path)
        {
            return path + "." + Guid.NewGuid().ToString("N").Substring(0, 8) + TempSuffix;
        }

        /* Copies source to a temp file beside destination, then moves it over destination */
        private static void CopyAtomic(string source, string destination)
        {
            string tmp = TempPathFor(destination);
            try
            {
                File.Copy(source, tmp, false);
                MoveWithRetry(tmp, destination);
            }
            catch
            {
                TryDelete(tmp);
                throw;
            }
        }

        private static void MoveWithRetry(string source, string destination)
        {
            int attempt = 0;
            while (true)
            {
                try
                {
                    MoveOverwrite(source, destination);
                    return;
                }
                catch (Exception ex)
                {
                    if (!(ex is IOException || ex is UnauthorizedAccessException) || attempt >= MoveRetries)
                        throw;
                }
                attempt++;
                Thread.Sleep(MoveRetryDelayMs);
            }
        }

        /* Replaces destination in one step, so a reader never sees a partial file */
        private static void MoveOverwrite(string source, string destination)
        {
#if GNH_MAUI
            File.Move(source, destination, true);
#else
            if (File.Exists(destination))
                File.Replace(source, destination, null, true);
            else
                File.Move(source, destination);
#endif
        }

        private static void TryDelete(string path)
        {
            try
            {
                if (File.Exists(path))
                    File.Delete(path);
            }
            catch
            {
                /* A stale temp is removed by DeleteStaleTemps later */
            }
        }
    }
}
