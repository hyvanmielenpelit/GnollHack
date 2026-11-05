using System;
using System.IO;

namespace GnollHackM
{
    /* Directories of the Avalonia desktop target */
    public static class GHAvaloniaPaths
    {
        private static readonly object _dataDirectoryLock = new object();
        private static string _dataDirectory = null;
        private static readonly Lazy<string> _assetDirectory = new Lazy<string>(FindAssetDirectory);

        /* Read-only game assets (gnh, banks, tileset): the application directory, or on macOS a bundle's
           Contents/Resources. Native libraries stay beside the executable, in AppContext.BaseDirectory. */
        public static string AssetDirectory => _assetDirectory.Value;

        private static string FindAssetDirectory()
        {
            string baseDirectory = AppContext.BaseDirectory;
            if (OperatingSystem.IsMacOS())
            {
                string trimmed = baseDirectory.TrimEnd('/');
                if (trimmed.EndsWith("/Contents/MacOS", StringComparison.Ordinal))
                {
                    string resources = Path.Combine(Path.GetDirectoryName(trimmed), "Resources");
                    if (Directory.Exists(resources))
                        return resources + "/";
                }
            }
            return baseDirectory;
        }

        /* Writable game data: ~/.local/share/GnollHack on Linux, ~/Library/Application Support/GnollHack on macOS,
           %LOCALAPPDATA%\GnollHack-Avalonia on Windows. Created on first use. */
        public static string DataDirectory
        {
            get
            {
                lock (_dataDirectoryLock)
                {
                    if (_dataDirectory == null)
                    {
#if LINUX || MACOS
                        string folderName = "GnollHack";
#else
                        string folderName = "GnollHack-Avalonia";
#endif
                        /* Create: by default GetFolderPath returns an empty string for a missing folder, such as
                           ~/.local/share on a fresh Linux account, which would make the path relative */
                        string path = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData, Environment.SpecialFolderOption.Create), folderName);
                        Directory.CreateDirectory(path);
                        _dataDirectory = path;
                    }
                    return _dataDirectory;
                }
            }
        }
    }
}
