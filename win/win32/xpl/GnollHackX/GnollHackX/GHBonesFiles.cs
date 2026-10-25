using System;
using System.IO;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace GnollHackX
{
    /// <summary>
    /// Bones file names as the game core makes them, and how a received bones file is written.
    /// Pure functions of plain inputs: no GHApp, MAUI or Xamarin types. Must compile as C# 7.3.
    /// </summary>
    public static class GHBonesFiles
    {
        private const int MaxLogTextLength = 64;

        // The name set_bonesfile_name (src/files.c) produces:
        //   bon              literal
        //   [0-9]?           bones pool, only when sysopt.bones_pools > 1
        //   -[0-6]           context.game_difficulty - MIN_DIFFICULTY_LEVEL (include/general.h: -4 to 2)
        //   [A-Z]            dungeons[].boneid (dat/dungeon.def)
        //   0|[A-Z][a-z]{2}  "0", or urole.filecode in the quest (src/role.c)
        //   \.[A-Z]          special level boneid (dat/dungeon.def), or
        //   \.[1-9][0-9]?    level number
        private static readonly Regex _bonesFileNameRegex =
            new Regex(@"\Abon[0-9]?-[0-6][A-Z](0|[A-Z][a-z]{2})\.([A-Z]|[1-9][0-9]?)\z", RegexOptions.CultureInvariant);

        /// <summary>
        /// True when <paramref name="fileName"/> is a bare bones file name of the form set_bonesfile_name
        /// (src/files.c) produces, such as "bon-2D0.15", "bon3-1QBar.2" or "bon-0M0.T".
        /// </summary>
        public static bool IsValidBonesFileName(string fileName)
        {
            if (string.IsNullOrEmpty(fileName))
                return false;
            return _bonesFileNameRegex.IsMatch(fileName);
        }

        /// <summary>
        /// The full path of <paramref name="fileName"/> inside <paramref name="directory"/>, or null when the name is not
        /// a valid bones file name or the path would resolve outside the directory.
        /// </summary>
        public static string GetSavePathInside(string directory, string fileName)
        {
            if (!IsValidBonesFileName(fileName) || string.IsNullOrEmpty(directory))
                return null;

            try
            {
                string root = Path.GetFullPath(directory).TrimEnd(Path.DirectorySeparatorChar, Path.AltDirectorySeparatorChar)
                    + Path.DirectorySeparatorChar;
                string full = Path.GetFullPath(Path.Combine(directory, fileName));
                if (full.StartsWith(root, StringComparison.Ordinal) && full.Length > root.Length)
                    return full;
                return null;
            }
            catch (ArgumentException)
            {
                return null;
            }
            catch (NotSupportedException)
            {
                return null;
            }
            catch (PathTooLongException)
            {
                return null;
            }
            catch (System.Security.SecurityException)
            {
                return null;
            }
        }

        /// <summary>
        /// Writes <paramref name="bytes"/> to <paramref name="path"/>, which must not exist. The bytes go to a temporary
        /// file beside it first, so <paramref name="path"/> never holds a partial file.
        /// </summary>
        /// <exception cref="IOException"><paramref name="path"/> already exists, or the write failed.</exception>
        public static async Task WriteNewFileAsync(string path, byte[] bytes)
        {
            if (path == null)
                throw new ArgumentNullException(nameof(path));
            if (bytes == null)
                throw new ArgumentNullException(nameof(bytes));

            // Starts with the bones name, so GHApp.ClearBones also removes one left behind by a killed process
            string temp = path + "." + Guid.NewGuid().ToString("N") + ".tmp";
            try
            {
                using (FileStream fs = new FileStream(temp, FileMode.CreateNew, FileAccess.Write))
                {
                    await fs.WriteAsync(bytes, 0, bytes.Length);
                }
                // Does not overwrite: throws IOException when path exists
                File.Move(temp, path);
            }
            catch
            {
                try
                {
                    if (File.Exists(temp))
                        File.Delete(temp);
                }
                catch
                {
                    // A temporary file that cannot be deleted is never loaded by the core
                }
                throw;
            }
        }

        /// <summary>
        /// <paramref name="text"/> made safe for one log line: control characters become '?', and text longer than
        /// 64 characters is cut and ends in "...". Null becomes "(null)".
        /// </summary>
        public static string FormatForLog(string text)
        {
            if (text == null)
                return "(null)";

            int length = Math.Min(text.Length, MaxLogTextLength);
            StringBuilder sb = new StringBuilder(length + 3);
            for (int i = 0; i < length; i++)
            {
                char c = text[i];
                sb.Append(c < (char)0x20 || c == (char)0x7F ? '?' : c);
            }
            if (text.Length > MaxLogTextLength)
                sb.Append("...");
            return sb.ToString();
        }
    }
}
