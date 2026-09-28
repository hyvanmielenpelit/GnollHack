using System;
using System.IO;

namespace GnollHackX.Performance
{
    /* The rules a performance suite id obeys: it is the suite's folder name under the
       store's suites directory, and consists of 1 to MaxLength characters of
       [A-Za-z0-9_-], so it can never be empty, rooted, "." or "..", or contain a path
       separator. Pure: no GHApp, MAUI or file access. Must compile under C# 7.3 (the
       legacy netstandard2.0 project). */
    public static class GHPerformanceSuiteId
    {
        public const int MaxLength = 128;

        public static bool IsValid(string suiteId)
        {
            if (string.IsNullOrEmpty(suiteId) || suiteId.Length > MaxLength)
                return false;
            for (int i = 0; i < suiteId.Length; i++)
            {
                char c = suiteId[i];
                bool ok = (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_' || c == '-';
                if (!ok)
                    return false;
            }
            return true;
        }

        /* True when folderName is a valid id and the manifest's suiteId equals it exactly */
        public static bool MatchesFolder(string manifestSuiteId, string folderName)
        {
            return IsValid(folderName) && string.Equals(manifestSuiteId, folderName, StringComparison.Ordinal);
        }

        /* root joined with suiteId; null when root is empty, suiteId is not valid, or the
           resulting full path is not a direct child of root's full path */
        public static string ResolveDirectory(string root, string suiteId)
        {
            if (string.IsNullOrEmpty(root) || !IsValid(suiteId))
                return null;
            try
            {
                string combined = Path.Combine(root, suiteId);
                string rootFull = TrimSeparators(Path.GetFullPath(root));
                string parentFull = Path.GetDirectoryName(Path.GetFullPath(combined));
                if (parentFull == null || !string.Equals(TrimSeparators(parentFull), rootFull, StringComparison.Ordinal))
                    return null;
                return combined;
            }
            catch
            {
                return null;
            }
        }

        private static string TrimSeparators(string path)
        {
            return path.TrimEnd(Path.DirectorySeparatorChar, Path.AltDirectorySeparatorChar);
        }
    }
}
