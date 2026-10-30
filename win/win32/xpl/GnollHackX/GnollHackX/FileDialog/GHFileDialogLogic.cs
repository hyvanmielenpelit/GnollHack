using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Runtime.InteropServices;
using System.Security;
using System.Text;
using System.Threading.Tasks;

namespace GnollHackX
{
    public enum GHFileRowKind
    {
        Place,
        Folder,
        File,
        Message
    }

    public sealed class GHFileRow
    {
        public GHFileRowKind Kind;
        public string Name;
        public string Detail;
        public string Path;
        public GHFilePlace Place;
        public bool IsMarked;
    }

    /* Runs file dialogs one at a time, in the order requested. A request made during a running item's synchronous part
       queues after that item. */
    public sealed class GHFileDialogQueue
    {
        private Task _last = Task.CompletedTask;

        public Task<T> Enqueue<T>(Func<Task<T>> show)
        {
            TaskCompletionSource<bool> done = new TaskCompletionSource<bool>(TaskCreationOptions.RunContinuationsAsynchronously);
            Task previous = _last;
            _last = done.Task;
            return RunAfterAsync(previous, show, done);
        }

        private static async Task<T> RunAfterAsync<T>(Task previous, Func<Task<T>> show, TaskCompletionSource<bool> done)
        {
            try
            {
                await previous;
                return await show();
            }
            finally
            {
                done.TrySetResult(true);
            }
        }
    }

    /* Path, listing and copy helpers of the file dialog control. No GHApp, MAUI or Xamarin types.
       Must compile under C# 7.3 (the legacy netstandard2.0 project). */
    public static class GHFileDialogLogic
    {
        private const string RemovableMediaPrefix = "/run/media/";
        private const string ThumbnailsFolderName = "thumbnails";
        private const string DefaultFileName = "file";
        private const string InvalidFileNameCharacters = "<>:\"/\\|?*";
        private const int TemporaryNameStemLength = 40;
        private const int MaxUniqueFileNames = 100000;
        private const int MaxReserveAttempts = 1000;
        private const int MaxLinkDepth = 40;

        private static readonly bool IsWindows = RuntimeInformation.IsOSPlatform(OSPlatform.Windows);
        private static readonly char[] AnySeparator = { '/', '\\' };
        private static readonly char[] SystemSeparators = { Path.DirectorySeparatorChar, Path.AltDirectorySeparatorChar };
        private static readonly string[] SizeUnits = { "KB", "MB", "GB", "TB" };
        private static readonly HashSet<string> ReservedFileNames = new HashSet<string>(StringComparer.Ordinal)
        {
            "CON", "PRN", "AUX", "NUL",
            "COM1", "COM2", "COM3", "COM4", "COM5", "COM6", "COM7", "COM8", "COM9",
            "LPT1", "LPT2", "LPT3", "LPT4", "LPT5", "LPT6", "LPT7", "LPT8", "LPT9"
        };

        /* Reads user-dirs.dirs: KEY="$HOME/dir" or KEY="/absolute/dir". Other values are not valid in that file. */
        public static Dictionary<string, string> ParseUserDirs(string text, string home)
        {
            Dictionary<string, string> result = new Dictionary<string, string>(StringComparer.Ordinal);
            if (string.IsNullOrEmpty(text))
            {
                return result;
            }

            foreach (string rawLine in text.Split('\n'))
            {
                string line = rawLine.Trim();
                if (line.Length == 0 || line[0] == '#')
                {
                    continue;
                }

                int equals = line.IndexOf('=');
                if (equals <= 0)
                {
                    continue;
                }

                string key = line.Substring(0, equals).Trim();
                string value = DecodeUserDirValue(line.Substring(equals + 1).Trim(), home);
                if (key.Length > 0 && value != null)
                {
                    result[key] = value;
                }
            }

            return result;
        }

        private static string DecodeUserDirValue(string raw, string home)
        {
            int position = 0;
            bool isQuoted = raw.Length > 0 && raw[0] == '"';
            if (isQuoted)
            {
                position = 1;
            }

            StringBuilder builder = new StringBuilder();
            foreach (string token in new[] { "$HOME", "${HOME}" })
            {
                if (string.CompareOrdinal(raw, position, token, 0, token.Length) == 0)
                {
                    int next = position + token.Length;
                    if (next == raw.Length || raw[next] == '/' || raw[next] == '"')
                    {
                        if (string.IsNullOrEmpty(home))
                        {
                            return null;
                        }

                        builder.Append(home.TrimEnd('/'));
                        position = next;
                        break;
                    }
                }
            }

            for (; position < raw.Length; position++)
            {
                char c = raw[position];
                if (c == '"' && isQuoted)
                {
                    break;
                }

                if (c == '\\' && position + 1 < raw.Length)
                {
                    position++;
                    c = raw[position];
                }

                builder.Append(c);
            }

            string value = builder.ToString();
            if (value.Length == 0 || value[0] != '/')
            {
                return null;
            }

            return TrimTrailingSlashes(value);
        }

        private static string TrimTrailingSlashes(string path)
        {
            string trimmed = path.TrimEnd('/');
            return trimmed.Length == 0 ? "/" : trimmed;
        }

        /* Mount points under /run/media/ from /proc/self/mounts text, in file order without duplicates */
        public static IList<string> ParseRemovableMedia(string mountsText)
        {
            List<string> result = new List<string>();
            if (string.IsNullOrEmpty(mountsText))
            {
                return result;
            }

            foreach (string line in mountsText.Split('\n'))
            {
                string[] fields = line.Split(new[] { ' ', '\t' }, StringSplitOptions.RemoveEmptyEntries);
                if (fields.Length < 2)
                {
                    continue;
                }

                string mountPoint = TrimTrailingSlashes(DecodeMountField(fields[1]));
                if (mountPoint.Length > RemovableMediaPrefix.Length
                    && mountPoint.StartsWith(RemovableMediaPrefix, StringComparison.Ordinal)
                    && !result.Contains(mountPoint))
                {
                    result.Add(mountPoint);
                }
            }

            return result;
        }

        /* Decodes the three-digit octal escapes of a mounts field, such as \040 for a space */
        private static string DecodeMountField(string field)
        {
            StringBuilder builder = new StringBuilder(field.Length);
            for (int i = 0; i < field.Length; i++)
            {
                if (field[i] == '\\' && i + 3 < field.Length && IsOctalDigit(field[i + 1])
                    && IsOctalDigit(field[i + 2]) && IsOctalDigit(field[i + 3]))
                {
                    builder.Append((char)(((field[i + 1] - '0') * 64) + ((field[i + 2] - '0') * 8) + (field[i + 3] - '0')));
                    i += 3;
                }
                else
                {
                    builder.Append(field[i]);
                }
            }

            return builder.ToString();
        }

        private static bool IsOctalDigit(char c)
        {
            return c >= '0' && c <= '7';
        }

        /* Compares whole segments: /a/bc is not inside /a/b, and the root is inside itself */
        public static bool IsInside(string root, string path)
        {
            if (string.IsNullOrEmpty(root) || string.IsNullOrEmpty(path))
            {
                return false;
            }

            string fullRoot;
            string fullPath;
            try
            {
                fullRoot = Path.GetFullPath(root).TrimEnd(SystemSeparators);
                fullPath = Path.GetFullPath(path).TrimEnd(SystemSeparators);
            }
            catch (Exception ex) when (IsFileSystemError(ex))
            {
                return false;
            }

            StringComparison comparison = IsWindows ? StringComparison.OrdinalIgnoreCase : StringComparison.Ordinal;
            if (string.Equals(fullRoot, fullPath, comparison))
            {
                return true;
            }

            return fullPath.StartsWith(fullRoot + Path.DirectorySeparatorChar, comparison);
        }

        /* The full path with each existing symbolic link resolved, and any missing tail appended unchanged */
        public static string ResolvePath(string path)
        {
            if (string.IsNullOrEmpty(path))
            {
                return path;
            }

            string fullPath = Path.GetFullPath(path);
#if GNH_MAUI
            return ResolveFullPath(fullPath, 0);
#else
            return fullPath;
#endif
        }

#if GNH_MAUI
        private static string ResolveFullPath(string fullPath, int depth)
        {
            string root = Path.GetPathRoot(fullPath);
            string[] segments = fullPath.Substring(root.Length).Split(SystemSeparators, StringSplitOptions.RemoveEmptyEntries);
            string current = root;
            for (int i = 0; i < segments.Length; i++)
            {
                string next = Path.Combine(current, segments[i]);
                FileSystemInfo target;
                try
                {
                    target = File.ResolveLinkTarget(next, true);
                }
                catch (IOException)
                {
                    /* Missing, or a link loop */
                    current = next;
                    for (int j = i + 1; j < segments.Length; j++)
                    {
                        current = Path.Combine(current, segments[j]);
                    }

                    return current;
                }
                catch (UnauthorizedAccessException)
                {
                    target = null;
                }

                if (target == null)
                {
                    current = next;
                }
                else
                {
                    /* A link's target can have a symbolic link among its own parents */
                    string resolved = Path.GetFullPath(target.FullName);
                    current = depth < MaxLinkDepth ? ResolveFullPath(resolved, depth + 1) : resolved;
                }
            }

            return current;
        }
#endif

        /* A bare file name that is valid on Linux and on FAT */
        public static string SanitizeFileName(string name)
        {
            if (string.IsNullOrEmpty(name))
            {
                return DefaultFileName;
            }

            string bareName = name.Substring(name.LastIndexOfAny(AnySeparator) + 1);
            StringBuilder builder = new StringBuilder(bareName.Length);
            foreach (char c in bareName)
            {
                bool isInvalid = c < 32 || c == 127 || InvalidFileNameCharacters.IndexOf(c) >= 0;
                builder.Append(isInvalid ? '_' : c);
            }

            string cleaned = builder.ToString().Trim().TrimEnd('.', ' ');
            if (cleaned.Length == 0)
            {
                return DefaultFileName;
            }

            int firstDot = cleaned.IndexOf('.');
            string stem = firstDot >= 0 ? cleaned.Substring(0, firstDot) : cleaned;
            if (ReservedFileNames.Contains(stem.ToUpperInvariant()))
            {
                cleaned = "_" + cleaned;
            }

            return cleaned;
        }

        /* The name itself when free, otherwise "stem (2).ext", "stem (3).ext" and so on */
        public static string UniqueFileName(string name, Func<string, bool> exists)
        {
            if (exists == null || !exists(name))
            {
                return name;
            }

            string stem = Path.GetFileNameWithoutExtension(name);
            string extension = Path.GetExtension(name);
            if (stem.Length == 0)
            {
                stem = name;
                extension = "";
            }

            for (int number = 2; number < MaxUniqueFileNames; number++)
            {
                string candidate = stem + " (" + number.ToString(CultureInfo.InvariantCulture) + ")" + extension;
                if (!exists(candidate))
                {
                    return candidate;
                }
            }

            throw new IOException("Could not find a free file name for " + name + ".");
        }

        /* Bare lowercase extensions. Null stands for all files, as does a wildcard such as "*.*". */
        public static IList<string> NormalizeExtensions(IEnumerable<string> extensions)
        {
            if (extensions == null)
            {
                return null;
            }

            List<string> result = new List<string>();
            foreach (string extension in extensions)
            {
                if (string.IsNullOrWhiteSpace(extension))
                {
                    continue;
                }

                string normalized = extension.Trim().TrimStart('*', '.').ToLowerInvariant();
                if (normalized.Length == 0)
                {
                    return null;
                }

                if (!result.Contains(normalized))
                {
                    result.Add(normalized);
                }
            }

            return result.Count == 0 ? null : result;
        }

        /* Folders first by name, then files newest first. Extensions are in NormalizeExtensions form. */
        public static IList<GHFileRow> ListFolder(string path, GHFilePlace place, IList<string> extensions, int maxRows, out int omitted)
        {
            omitted = 0;
            List<GHFileRow> folders = new List<GHFileRow>();
            List<KeyValuePair<DateTime, GHFileRow>> files = new List<KeyValuePair<DateTime, GHFileRow>>();
            if (string.IsNullOrEmpty(path) || place == null)
            {
                return folders;
            }

            try
            {
                string root = ResolvePath(place.Path);
                if (!IsInside(root, ResolvePath(path)))
                {
                    return folders;
                }

                foreach (FileSystemInfo info in new DirectoryInfo(path).EnumerateFileSystemInfos())
                {
                    try
                    {
                        AddListedEntry(info, root, place, extensions, folders, files);
                    }
                    catch (Exception ex) when (IsFileSystemError(ex))
                    {
                        /* The entry went away or cannot be read */
                    }
                }
            }
            catch (Exception ex) when (IsFileSystemError(ex))
            {
                return new List<GHFileRow>();
            }

            folders.Sort(CompareRowNames);
            files.Sort((a, b) =>
            {
                int byTime = b.Key.CompareTo(a.Key);
                return byTime != 0 ? byTime : CompareRowNames(a.Value, b.Value);
            });

            int limit = Math.Max(maxRows, 0);
            omitted = Math.Max(folders.Count + files.Count - limit, 0);
            List<GHFileRow> rows = new List<GHFileRow>(Math.Min(folders.Count + files.Count, limit));
            foreach (GHFileRow folder in folders)
            {
                if (rows.Count < limit)
                {
                    rows.Add(folder);
                }
            }

            foreach (KeyValuePair<DateTime, GHFileRow> file in files)
            {
                if (rows.Count < limit)
                {
                    rows.Add(file.Value);
                }
            }

            return rows;
        }

        private static void AddListedEntry(FileSystemInfo info, string root, GHFilePlace place, IList<string> extensions,
            List<GHFileRow> folders, List<KeyValuePair<DateTime, GHFileRow>> files)
        {
            string name = info.Name;
            FileAttributes attributes = info.Attributes;
            if (name.StartsWith(".", StringComparison.Ordinal) || (int)attributes == -1
                || (attributes & FileAttributes.Hidden) != 0)
            {
                return;
            }

            if (place.HideThumbnails && string.Equals(name, ThumbnailsFolderName, StringComparison.OrdinalIgnoreCase))
            {
                return;
            }

            bool isDirectory = info is DirectoryInfo;
            FileInfo file = info as FileInfo;
            if ((attributes & FileAttributes.ReparsePoint) != 0)
            {
                string resolved = ResolvePath(info.FullName);
                if (!IsInside(root, resolved))
                {
                    return;
                }

                isDirectory = Directory.Exists(resolved);
                if (!isDirectory)
                {
                    if (!File.Exists(resolved))
                    {
                        return;
                    }

                    file = new FileInfo(resolved);
                }
            }

            if (isDirectory)
            {
                folders.Add(new GHFileRow { Kind = GHFileRowKind.Folder, Name = name, Path = info.FullName, Place = place });
            }
            else if (file != null && MatchesExtension(name, extensions))
            {
                DateTime lastWriteUtc = file.LastWriteTimeUtc;
                GHFileRow row = new GHFileRow
                {
                    Kind = GHFileRowKind.File,
                    Name = name,
                    Detail = FormatDetail(lastWriteUtc, file.Length),
                    Path = info.FullName,
                    Place = place
                };
                files.Add(new KeyValuePair<DateTime, GHFileRow>(lastWriteUtc, row));
            }
        }

        private static bool MatchesExtension(string name, IList<string> extensions)
        {
            if (extensions == null)
            {
                return true;
            }

            foreach (string extension in extensions)
            {
                if (name.EndsWith("." + extension, StringComparison.OrdinalIgnoreCase))
                {
                    return true;
                }
            }

            return false;
        }

        private static int CompareRowNames(GHFileRow a, GHFileRow b)
        {
            int result = string.Compare(a.Name, b.Name, StringComparison.OrdinalIgnoreCase);
            return result != 0 ? result : string.CompareOrdinal(a.Name, b.Name);
        }

        public static string FormatDetail(DateTime lastWriteUtc, long size)
        {
            DateTime local = DateTime.SpecifyKind(lastWriteUtc, DateTimeKind.Utc).ToLocalTime();
            return local.ToString("yyyy-MM-dd HH:mm", CultureInfo.InvariantCulture) + " · " + FormatSize(size);
        }

        public static string FormatSize(long bytes)
        {
            if (bytes < 1024)
            {
                return Math.Max(bytes, 0L).ToString(CultureInfo.InvariantCulture) + " B";
            }

            double value = bytes;
            int unit = -1;
            while (value >= 1024 && unit < SizeUnits.Length - 1)
            {
                value /= 1024;
                unit++;
            }

            return value.ToString(value < 100 ? "0.0" : "0", CultureInfo.InvariantCulture) + " " + SizeUnits[unit];
        }

        /* Copies to a new file in the directory, creating the directory, and never replaces an existing file.
           Returns the final path. */
        public static string CopyNoClobber(string source, string directory, string name)
        {
#if GNH_MAUI
            Directory.CreateDirectory(directory);
            EnsureFreeSpace(source, directory);
            string temporary = Path.Combine(directory, GetTemporaryName(name));
            string reserved = null;
            try
            {
                WriteTemporaryCopy(source, temporary);
                reserved = ReserveUniqueName(directory, name, null);
                /* Moves over this copy's own empty reservation: a no-overwrite move on Unix checks, then renames */
                File.Move(temporary, reserved, true);
                return reserved;
            }
            catch
            {
                TryDelete(temporary);
                TryDelete(reserved);
                throw;
            }
#else
            throw new PlatformNotSupportedException();
#endif
        }

        /* Writes a temporary file beside the target, then moves it over the target */
        public static void CopyReplacing(string source, string target)
        {
#if GNH_MAUI
            string fullTarget = Path.GetFullPath(target);
            string directory = Path.GetDirectoryName(fullTarget);
            EnsureFreeSpace(source, directory);
            string temporary = Path.Combine(directory, GetTemporaryName(Path.GetFileName(fullTarget)));
            try
            {
                WriteTemporaryCopy(source, temporary);
                File.Move(temporary, fullTarget, true);
            }
            catch
            {
                TryDelete(temporary);
                throw;
            }
#else
            throw new PlatformNotSupportedException();
#endif
        }

#if GNH_MAUI
        /* Creates the first free name as an empty file. onChecked runs between the check and the creation. */
        internal static string ReserveUniqueName(string directory, string name, Action<string> onChecked)
        {
            for (int attempt = 0; attempt < MaxReserveAttempts; attempt++)
            {
                string candidate = Path.Combine(directory, UniqueFileName(name, n => PathExists(Path.Combine(directory, n))));
                onChecked?.Invoke(candidate);
                try
                {
                    using (new FileStream(candidate, FileMode.CreateNew, FileAccess.Write, FileShare.None))
                    {
                    }

                    return candidate;
                }
                catch (IOException) when (PathExists(candidate))
                {
                    /* Another writer took the name after the check */
                }
            }

            throw new IOException("Could not reserve a file name for " + name + ".");
        }

        internal static void CheckFreeSpace(long required, long available)
        {
            if (required > available)
            {
                throw new IOException("There is not enough free space: the file needs " + FormatSize(required)
                    + " and " + FormatSize(available) + " is available.");
            }
        }

        private static void EnsureFreeSpace(string source, string directory)
        {
            long required = new FileInfo(source).Length;
            long available;
            try
            {
                available = new DriveInfo(directory).AvailableFreeSpace;
            }
            catch (Exception ex) when (IsFileSystemError(ex))
            {
                return;
            }

            CheckFreeSpace(required, available);
        }

        private static void WriteTemporaryCopy(string source, string temporary)
        {
            using (FileStream input = new FileStream(source, FileMode.Open, FileAccess.Read, FileShare.Read | FileShare.Delete))
            using (FileStream output = new FileStream(temporary, FileMode.CreateNew, FileAccess.Write, FileShare.None))
            {
                input.CopyTo(output);
                output.Flush(true);
            }
        }

        private static string GetTemporaryName(string name)
        {
            string stem = name ?? "";
            if (stem.Length > TemporaryNameStemLength)
            {
                stem = stem.Substring(0, TemporaryNameStemLength);
                if (char.IsHighSurrogate(stem[stem.Length - 1]))
                {
                    stem = stem.Substring(0, stem.Length - 1);
                }
            }

            return "." + stem + "." + Guid.NewGuid().ToString("N") + ".part";
        }

        private static bool PathExists(string path)
        {
            return File.Exists(path) || Directory.Exists(path);
        }

        private static void TryDelete(string path)
        {
            if (path == null)
            {
                return;
            }

            try
            {
                File.Delete(path);
            }
            catch (Exception ex) when (IsFileSystemError(ex))
            {
                /* Left behind; a dot name is not listed */
            }
        }
#endif

        /* True when the target resolves inside a protected folder or to the source. An invalid path counts as protected. */
        public static bool IsProtectedTarget(string target, IEnumerable<string> protectedFolders, string source)
        {
            try
            {
                string resolvedTarget = ResolvePath(target);
                if (!string.IsNullOrEmpty(source))
                {
                    string resolvedSource = ResolvePath(source);
                    if (IsInside(resolvedTarget, resolvedSource) && IsInside(resolvedSource, resolvedTarget))
                    {
                        return true;
                    }
                }

                if (protectedFolders != null)
                {
                    foreach (string folder in protectedFolders)
                    {
                        if (!string.IsNullOrEmpty(folder) && IsInside(ResolvePath(folder), resolvedTarget))
                        {
                            return true;
                        }
                    }
                }

                return false;
            }
            catch (Exception ex) when (IsFileSystemError(ex))
            {
                return true;
            }
        }

        private static bool IsFileSystemError(Exception ex)
        {
            return ex is IOException || ex is UnauthorizedAccessException || ex is ArgumentException
                || ex is NotSupportedException || ex is SecurityException;
        }
    }
}
