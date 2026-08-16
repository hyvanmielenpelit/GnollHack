using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Compression;
using System.Text.RegularExpressions;
#if GNH_MAUI
using GnollHackX;
namespace GnollHackM
#else
namespace GnollHackX
#endif
{
    public static class ReplayFileHelper
    {
        /* New format: replay-{Player}-T{Turn}-{MajMinPat}-{Edit}-{Timestamp}.gnhrec.gz  (version = e.g. 430-1) */
        /* Old format: replay-{Player}-T{Turn}-{VersionCode}-{Timestamp}.gnhrec.gz       (version = raw ulong) */
        private static readonly Regex _replayFileNameRegex = new Regex(
            @"^(?:replay|rpcont)-(?:(?<PlayerName>[^-]+)-)?(?:T(?<Turn>\d+)-)?(?<Version>\d+(?:-\d+)?)-(?<Timestamp>\d{10,})\.gnhrec(?:\.gz|\.zip)?$",
            RegexOptions.Compiled);

        public static void ParseReplayFileName(GHRecordedGameFile file)
        {
            if (string.IsNullOrEmpty(file.FileName))
                return;

            Match match = _replayFileNameRegex.Match(file.FileName);
            if (match.Success)
            {
                if (match.Groups["PlayerName"].Success)
                {
                    file.PlayerName = match.Groups["PlayerName"].Value;
                }
                
                if (match.Groups["Turn"].Success && int.TryParse(match.Groups["Turn"].Value, out int turn))
                {
                    file.FirstTurn = turn;
                }

                if (match.Groups["Version"].Success)
                {
                    string versionStr = match.Groups["Version"].Value;
                    file.VersionString = FormatVersionString(versionStr);
                }

                if (match.Groups["Timestamp"].Success)
                {
                    if (long.TryParse(match.Groups["Timestamp"].Value, out long timeBinary))
                    {
                        try
                        {
                            file.RecordingTimestamp = DateTime.FromBinary(timeBinary);
                        }
                        catch
                        {
                            // Ignore invalid times
                        }
                    }
                    else if (ulong.TryParse(match.Groups["Timestamp"].Value, out ulong uTimeBinary))
                    {
                        try
                        {
                            file.RecordingTimestamp = DateTime.FromBinary((long)uTimeBinary);
                        }
                        catch
                        {
                            // Ignore invalid times
                        }
                    }
                }
            }
        }

        /* Converts version strings from filename format to human-readable format:
         * New format "430-1" -> "v4.3.0.1" (majorminorpatch-edit)
         * Old format "12345678" -> "v12345678" (raw version code)
         */
        private static string FormatVersionString(string versionStr)
        {
            if (string.IsNullOrEmpty(versionStr))
                return versionStr;

            /* New format: "430-1" means major=4, minor=3, patch=0, edit=1 */
            int dashIdx = versionStr.IndexOf('-');
            if (dashIdx > 0 && dashIdx < versionStr.Length - 1)
            {
                string majMinPat = versionStr.Substring(0, dashIdx);
                string edit = versionStr.Substring(dashIdx + 1);
                if (majMinPat.Length >= 3)
                {
                    /* Each character is a digit: first=major, second=minor, third=patch */
                    return string.Format("v{0}.{1}.{2}.{3}", majMinPat[0], majMinPat[1], majMinPat[2], edit);
                }
                return "v" + versionStr;
            }

            /* Old format: raw version code number */
            return "v" + versionStr;
        }

        public static (bool isGZip, bool isNormalZip, string zipSuffix) GetCompressionInfo(string fileName)
        {
            if (string.IsNullOrEmpty(fileName)) return (false, false, "");
            bool isGZip = fileName.Length > GHConstants.ReplayGZipFileNameSuffix.Length && fileName.EndsWith(GHConstants.ReplayGZipFileNameSuffix);
            bool isNormalZip = fileName.Length > GHConstants.ReplayZipFileNameSuffix.Length && fileName.EndsWith(GHConstants.ReplayZipFileNameSuffix);
            string zipSuffix = isGZip ? GHConstants.ReplayGZipFileNameSuffix : (isNormalZip ? GHConstants.ReplayZipFileNameSuffix : "");
            return (isGZip, isNormalZip, zipSuffix);
        }

        public static string GetMiddlePart(string fileName, string zipSuffix)
        {
            if (string.IsNullOrEmpty(fileName) || !fileName.StartsWith(GHConstants.ReplayFileNamePrefix)) return null;
            
            int middleLen = fileName.Length - GHConstants.ReplayFileNamePrefix.Length - GHConstants.ReplayFileNameSuffix.Length - (zipSuffix?.Length ?? 0);
            if (middleLen > 0)
            {
                return fileName.Substring(GHConstants.ReplayFileNamePrefix.Length, middleLen);
            }
            return null;
        }

        public static List<string> GetContinuationFiles(string middleStr, string[] allFiles, string expectedZipSuffix)
        {
            List<string> contFiles = new List<string>();
            if (string.IsNullOrEmpty(middleStr) || allFiles == null) return contFiles;

            string expectedPrefix = GHConstants.ReplayContinuationFileNamePrefix + middleStr;

            foreach (string contFile in allFiles)
            {
                if (contFile == null || !File.Exists(contFile)) continue;
                
                FileInfo contFI = new FileInfo(contFile);
                if (!string.IsNullOrWhiteSpace(contFI.Name) && contFI.Name.StartsWith(expectedPrefix))
                {
                    contFiles.Add(contFile);
                }
            }
            return contFiles;
        }

        public static void CreateShareArchive(IEnumerable<string> filePaths, string outputPath)
        {
            if (File.Exists(outputPath))
            {
                File.Delete(outputPath);
            }
            using (ZipArchive archive = ZipFile.Open(outputPath, ZipArchiveMode.Create))
            {
                foreach (string filePath in filePaths)
                {
                    if (!string.IsNullOrWhiteSpace(filePath) && File.Exists(filePath))
                    {
                        archive.CreateEntryFromFile(filePath, Path.GetFileName(filePath));
                    }
                }
            }
        }
    }
}
