#if GNH_AVALONIA && LINUX
using System;
using System.Diagnostics;
using System.IO;
using System.Text;
using GnollHackX;

namespace GnollHackM
{
    /* The AppImage's entry in the user's application menu: a desktop file and an icon under the user's data folder,
       pointing at the AppImage file. They have names of their own, so entries made by hand or by AppImage managers
       are left alone. */
    internal static class GHAvaloniaDesktopEntry
    {
        public const string AskedPreferenceKey = "AppImageMenuEntryAsked";

        private const string EntryFileName = "gnollhack-appimage.desktop";
        private const string IconName = "gnollhack-appimage";
        private const string MarkerLine = "X-GnollHack-AppImage=true";
        private const string TryExecPrefix = "TryExec=";

        /* The AppImage file's path, which the AppImage runtime sets */
        private static string AppImagePath
        {
            get { return Environment.GetEnvironmentVariable("APPIMAGE"); }
        }

        private static string DataHome
        {
            get
            {
                string dataHome = Environment.GetEnvironmentVariable("XDG_DATA_HOME");
                if (string.IsNullOrEmpty(dataHome) || !Path.IsPathRooted(dataHome))
                    dataHome = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.UserProfile), ".local", "share");
                return dataHome;
            }
        }

        private static string ApplicationsDirectory
        {
            get { return Path.Combine(DataHome, "applications"); }
        }

        private static string EntryPath
        {
            get { return Path.Combine(ApplicationsDirectory, EntryFileName); }
        }

        private static string IconDirectory
        {
            get { return Path.Combine(DataHome, "icons", "hicolor"); }
        }

        private static string IconPath
        {
            get { return Path.Combine(IconDirectory, "512x512", "apps", IconName + ".png"); }
        }

        /* Started from an AppImage file that still exists */
        public static bool IsAvailable
        {
            get
            {
                try
                {
                    string path = AppImagePath;
                    return !string.IsNullOrEmpty(path) && File.Exists(path);
                }
                catch (Exception)
                {
                    return false;
                }
            }
        }

        /* Our desktop file exists */
        public static bool IsInstalled
        {
            get
            {
                try
                {
                    return File.Exists(EntryPath) && File.ReadAllText(EntryPath).Contains(MarkerLine);
                }
                catch (Exception)
                {
                    return false;
                }
            }
        }

        /* Another desktop file, such as one by Gear Lever or AppImageLauncher, already points at this AppImage */
        public static bool IsManagedElsewhere
        {
            get
            {
                try
                {
                    string path = AppImagePath;
                    if (string.IsNullOrEmpty(path) || !Directory.Exists(ApplicationsDirectory))
                        return false;
                    foreach (string file in Directory.GetFiles(ApplicationsDirectory, "*.desktop"))
                    {
                        if (string.Equals(Path.GetFileName(file), EntryFileName, StringComparison.Ordinal))
                            continue;
                        if (File.ReadAllText(file).Contains(path))
                            return true;
                    }
                }
                catch (Exception ex)
                {
                    GHApp.MaybeWriteGHLog("GHAvaloniaDesktopEntry: cannot read the application entries: " + ex.Message);
                }
                return false;
            }
        }

        /* Whether to ask about adding the entry: an AppImage start, no entry yet, not managed elsewhere, not asked */
        public static bool ShouldOffer()
        {
            return IsAvailable && !IsInstalled && !IsManagedElsewhere
                && !Microsoft.Maui.Storage.Preferences.Get(AskedPreferenceKey, false);
        }

        /* Writes the icon and the desktop file for the current AppImage path */
        public static bool Install()
        {
            try
            {
                string path = AppImagePath;
                if (string.IsNullOrEmpty(path) || path.IndexOf('\n') >= 0 || path.IndexOf('\r') >= 0)
                {
                    GHApp.MaybeWriteGHLog("GHAvaloniaDesktopEntry: no usable AppImage path");
                    return false;
                }

                string sourceIcon = Path.Combine(AppContext.BaseDirectory, "gnollhack.png");
                Directory.CreateDirectory(Path.GetDirectoryName(IconPath));
                File.Copy(sourceIcon, IconPath, true);

                StringBuilder entry = new StringBuilder();
                entry.Append("[Desktop Entry]\n");
                entry.Append("Type=Application\n");
                entry.Append("Name=GnollHack\n");
                entry.Append("Comment=Roguelike adventure game\n");
                entry.Append("Exec=\"").Append(EscapeQuotedExecArgument(path)).Append("\"\n");
                entry.Append(TryExecPrefix).Append(EscapeString(path)).Append('\n');
                entry.Append("Icon=").Append(IconName).Append('\n');
                entry.Append("StartupWMClass=GnollHack\n");
                entry.Append("Terminal=false\n");
                entry.Append("Categories=Game;RolePlaying;\n");
                entry.Append(MarkerLine).Append('\n');

                Directory.CreateDirectory(ApplicationsDirectory);
                File.WriteAllText(EntryPath, entry.ToString(), new UTF8Encoding(false));
                GHApp.MaybeWriteGHLog("GHAvaloniaDesktopEntry: menu entry written for " + path);
                RefreshMenus();
                return true;
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHAvaloniaDesktopEntry: cannot write the menu entry: " + ex.Message);
                return false;
            }
        }

        /* Deletes our desktop file and icon only */
        public static bool Remove()
        {
            try
            {
                if (File.Exists(EntryPath))
                    File.Delete(EntryPath);
                if (File.Exists(IconPath))
                    File.Delete(IconPath);
                GHApp.MaybeWriteGHLog("GHAvaloniaDesktopEntry: menu entry removed");
                RefreshMenus();
                return true;
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHAvaloniaDesktopEntry: cannot remove the menu entry: " + ex.Message);
                return false;
            }
        }

        /* Rewrites the entry when the AppImage has moved since it was written */
        public static void RefreshIfMoved()
        {
            try
            {
                if (!IsAvailable || !IsInstalled)
                    return;
                string expected = TryExecPrefix + EscapeString(AppImagePath);
                foreach (string line in File.ReadAllLines(EntryPath))
                {
                    if (line.StartsWith(TryExecPrefix, StringComparison.Ordinal))
                    {
                        if (string.Equals(line, expected, StringComparison.Ordinal))
                            return;
                        break;
                    }
                }
                Install();
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHAvaloniaDesktopEntry: cannot check the menu entry: " + ex.Message);
            }
        }

        /* A desktop-entry string value: a backslash is written as two */
        private static string EscapeString(string value)
        {
            return value.Replace("\\", "\\\\");
        }

        /* An Exec argument inside double quotes. The quoting rule puts a backslash before ", `, $ and \, and the string
           rule, applied first, doubles every backslash; % is written as %% */
        private static string EscapeQuotedExecArgument(string value)
        {
            StringBuilder escaped = new StringBuilder();
            foreach (char c in value)
            {
                if (c == '"' || c == '`' || c == '$')
                    escaped.Append("\\\\").Append(c);
                else if (c == '\\')
                    escaped.Append("\\\\\\\\");
                else if (c == '%')
                    escaped.Append("%%");
                else
                    escaped.Append(c);
            }
            return escaped.ToString();
        }

        /* Best effort: rebuilds the desktop database if the tool exists, and touches the icon theme folder */
        private static void RefreshMenus()
        {
            try
            {
                ProcessStartInfo startInfo = new ProcessStartInfo("update-desktop-database", "\"" + ApplicationsDirectory + "\"");
                startInfo.UseShellExecute = false;
                startInfo.CreateNoWindow = true;
                using (Process.Start(startInfo))
                {
                }
            }
            catch (Exception)
            {
                /* The tool is optional */
            }
            try
            {
                if (Directory.Exists(IconDirectory))
                    Directory.SetLastWriteTimeUtc(IconDirectory, DateTime.UtcNow);
            }
            catch (Exception)
            {
                /* Touching the folder is optional */
            }
        }
    }
}
#endif
