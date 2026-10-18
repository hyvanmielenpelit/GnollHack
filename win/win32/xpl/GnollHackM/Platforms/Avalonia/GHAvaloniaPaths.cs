using System;
using System.IO;

namespace GnollHackM
{
    /* Directories of the Avalonia desktop target */
    public static class GHAvaloniaPaths
    {
        private static readonly object _dataDirectoryLock = new object();
        private static string _dataDirectory = null;

        /* Read-only game assets (gnh, banks, tileset, native libraries): the application directory */
        public static string AssetDirectory => AppContext.BaseDirectory;

        /* Writable game data: ~/.local/share/GnollHack on Linux, %LOCALAPPDATA%\GnollHack-Avalonia
           elsewhere. Created on first use. */
        public static string DataDirectory
        {
            get
            {
                lock (_dataDirectoryLock)
                {
                    if (_dataDirectory == null)
                    {
#if LINUX
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
