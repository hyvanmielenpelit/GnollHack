using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;

namespace GnollHackX
{
    public sealed class GHFilePlace
    {
        public string Name;
        public string Path;
        public bool CreateOnUse;
        public bool HideThumbnails;

        public GHFilePlace(string name, string path, bool createOnUse = false, bool hideThumbnails = false)
        {
            Name = name;
            Path = path;
            CreateOnUse = createOnUse;
            HideThumbnails = hideThumbnails;
        }
    }

    /* The named folders the file dialog control offers. Current is null where the control is never used. */
    public abstract class GHFilePlaces
    {
        protected const string PicturesName = "Pictures";
        protected const string ScreenshotsName = "GnollHack Screenshots";
        protected const string DocumentsName = "Documents";
        protected const string DownloadsName = "Downloads";
        protected const string ExportsName = "GnollHack Exports";
        protected const string SaveFolderName = "GnollHack";

        private static readonly Lazy<GHFilePlaces> _current = new Lazy<GHFilePlaces>(CreateCurrent);

        public static GHFilePlaces Current
        {
            get { return _current.Value; }
        }

        private static GHFilePlaces CreateCurrent()
        {
#if GNH_AVALONIA && LINUX
            return new GHFreedesktopFilePlaces();
#elif GNH_AVALONIA
            return new GHWindowsFilePlaces();
#else
            return null;
#endif
        }

        public abstract IList<GHFilePlace> GetOpenPlaces(bool isImport);
        public abstract IList<GHFilePlace> GetSavePlaces();

        protected static void AddUnique(List<GHFilePlace> places, GHFilePlace place, StringComparison comparison)
        {
            foreach (GHFilePlace existing in places)
            {
                if (string.Equals(existing.Path, place.Path, comparison))
                {
                    return;
                }
            }

            places.Add(place);
        }
    }

    /* Paths are built with '/' rather than Path.Combine, so that the class behaves the same under test on Windows */
    public sealed class GHFreedesktopFilePlaces : GHFilePlaces
    {
        private const string UserDirsFileName = "user-dirs.dirs";
        private const string MountsFile = "/proc/self/mounts";
        private const string SteamUserDataFolder = "/.local/share/Steam/userdata";

        private readonly Func<string, string> _getEnvironment;
        private readonly string _home;
        private readonly Func<string, string> _readFile;
        private readonly Func<string, bool> _directoryExists;
        private readonly Func<string, IEnumerable<string>> _listDirectories;

        public GHFreedesktopFilePlaces()
            : this(Environment.GetEnvironmentVariable, Environment.GetFolderPath(Environment.SpecialFolder.UserProfile),
                  ReadFileOrNull, Directory.Exists, ListDirectoriesOrEmpty)
        {
        }

        /* readFile returns null for a missing file. listDirectories returns full paths. */
        public GHFreedesktopFilePlaces(Func<string, string> getEnvironment, string home, Func<string, string> readFile,
            Func<string, bool> directoryExists, Func<string, IEnumerable<string>> listDirectories)
        {
            _getEnvironment = getEnvironment;
            _home = (home ?? "").TrimEnd('/');
            _readFile = readFile;
            _directoryExists = directoryExists;
            _listDirectories = listDirectories;
        }

        public override IList<GHFilePlace> GetOpenPlaces(bool isImport)
        {
            Dictionary<string, string> userDirs = ReadUserDirs();
            string documents = GetUserDir(userDirs, "XDG_DOCUMENTS_DIR", DocumentsName);
            List<GHFilePlace> places = new List<GHFilePlace>();
            if (isImport)
            {
                AddIfExists(places, new GHFilePlace(ExportsName, documents + "/" + SaveFolderName));
            }

            AddIfExists(places, new GHFilePlace(PicturesName, GetUserDir(userDirs, "XDG_PICTURES_DIR", PicturesName)));
            string screenshots = FindScreenshotsFolder();
            if (screenshots != null)
            {
                AddIfExists(places, new GHFilePlace(ScreenshotsName, screenshots, false, true));
            }

            AddIfExists(places, new GHFilePlace(DocumentsName, documents));
            AddIfExists(places, new GHFilePlace(DownloadsName, GetUserDir(userDirs, "XDG_DOWNLOAD_DIR", DownloadsName)));
            foreach (string medium in GHFileDialogLogic.ParseRemovableMedia(_readFile(MountsFile)))
            {
                AddIfExists(places, new GHFilePlace(GetLastSegment(medium), medium));
            }

            return places;
        }

        public override IList<GHFilePlace> GetSavePlaces()
        {
            Dictionary<string, string> userDirs = ReadUserDirs();
            List<GHFilePlace> places = new List<GHFilePlace>();
            AddSavePlace(places, DocumentsName, GetUserDir(userDirs, "XDG_DOCUMENTS_DIR", DocumentsName));
            AddSavePlace(places, DownloadsName, GetUserDir(userDirs, "XDG_DOWNLOAD_DIR", DownloadsName));
            foreach (string medium in GHFileDialogLogic.ParseRemovableMedia(_readFile(MountsFile)))
            {
                if (_directoryExists(medium))
                {
                    AddSavePlace(places, GetLastSegment(medium), medium);
                }
            }

            return places;
        }

        private static void AddSavePlace(List<GHFilePlace> places, string parentName, string parentPath)
        {
            AddUnique(places, new GHFilePlace(parentName + "/" + SaveFolderName, parentPath + "/" + SaveFolderName, true),
                StringComparison.Ordinal);
        }

        private void AddIfExists(List<GHFilePlace> places, GHFilePlace place)
        {
            if (_directoryExists(place.Path))
            {
                AddUnique(places, place, StringComparison.Ordinal);
            }
        }

        private Dictionary<string, string> ReadUserDirs()
        {
            string configHome = _getEnvironment("XDG_CONFIG_HOME");
            if (string.IsNullOrEmpty(configHome) || configHome[0] != '/')
            {
                configHome = _home + "/.config";
            }

            return GHFileDialogLogic.ParseUserDirs(_readFile(configHome.TrimEnd('/') + "/" + UserDirsFileName), _home);
        }

        private string GetUserDir(Dictionary<string, string> userDirs, string key, string defaultFolder)
        {
            string value;
            return userDirs.TryGetValue(key, out value) ? value : _home + "/" + defaultFolder;
        }

        private string FindScreenshotsFolder()
        {
            string id = GetSteamShortcutId();
            if (id == null)
            {
                return null;
            }

            IEnumerable<string> users = _listDirectories(_home + SteamUserDataFolder);
            if (users == null)
            {
                return null;
            }

            foreach (string user in users)
            {
                string candidate = user.TrimEnd('/') + "/760/remote/" + id + "/screenshots";
                if (_directoryExists(candidate))
                {
                    return candidate;
                }
            }

            return null;
        }

        /* SteamAppId when nonzero, else the high 32 bits of SteamGameId. Null with neither. */
        private string GetSteamShortcutId()
        {
            ulong value;
            if (TryParseId(_getEnvironment("SteamAppId"), out value) && value != 0)
            {
                return value.ToString(CultureInfo.InvariantCulture);
            }

            if (TryParseId(_getEnvironment("SteamGameId"), out value) && (value >> 32) != 0)
            {
                return (value >> 32).ToString(CultureInfo.InvariantCulture);
            }

            return null;
        }

        private static bool TryParseId(string text, out ulong value)
        {
            return ulong.TryParse(text, NumberStyles.None, CultureInfo.InvariantCulture, out value);
        }

        private static string GetLastSegment(string path)
        {
            return path.Substring(path.LastIndexOf('/') + 1);
        }

        private static string ReadFileOrNull(string path)
        {
            try
            {
                return File.ReadAllText(path);
            }
            catch (IOException)
            {
                return null;
            }
            catch (UnauthorizedAccessException)
            {
                return null;
            }
        }

        private static IEnumerable<string> ListDirectoriesOrEmpty(string path)
        {
            try
            {
                return new List<string>(Directory.EnumerateDirectories(path));
            }
            catch (IOException)
            {
                return new List<string>();
            }
            catch (UnauthorizedAccessException)
            {
                return new List<string>();
            }
        }
    }

    public sealed class GHWindowsFilePlaces : GHFilePlaces
    {
        public override IList<GHFilePlace> GetOpenPlaces(bool isImport)
        {
            string documents = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);
            List<GHFilePlace> places = new List<GHFilePlace>();
            if (isImport && documents.Length > 0)
            {
                AddIfExists(places, new GHFilePlace(ExportsName, Path.Combine(documents, SaveFolderName)));
            }

            AddIfExists(places, new GHFilePlace(PicturesName, Environment.GetFolderPath(Environment.SpecialFolder.MyPictures)));
            AddIfExists(places, new GHFilePlace(DocumentsName, documents));
            AddIfExists(places, new GHFilePlace(DownloadsName, GetDownloadsFolder()));
            return places;
        }

        public override IList<GHFilePlace> GetSavePlaces()
        {
            List<GHFilePlace> places = new List<GHFilePlace>();
            AddSavePlace(places, DocumentsName, Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments));
            AddSavePlace(places, DownloadsName, GetDownloadsFolder());
            return places;
        }

        private static void AddSavePlace(List<GHFilePlace> places, string parentName, string parentPath)
        {
            if (!string.IsNullOrEmpty(parentPath))
            {
                AddUnique(places, new GHFilePlace(parentName + "/" + SaveFolderName, Path.Combine(parentPath, SaveFolderName), true),
                    StringComparison.OrdinalIgnoreCase);
            }
        }

        private static void AddIfExists(List<GHFilePlace> places, GHFilePlace place)
        {
            if (!string.IsNullOrEmpty(place.Path) && Directory.Exists(place.Path))
            {
                AddUnique(places, place, StringComparison.OrdinalIgnoreCase);
            }
        }

        private static string GetDownloadsFolder()
        {
            string profile = Environment.GetEnvironmentVariable("USERPROFILE");
            if (string.IsNullOrEmpty(profile))
            {
                profile = Environment.GetFolderPath(Environment.SpecialFolder.UserProfile);
            }

            return string.IsNullOrEmpty(profile) ? "" : Path.Combine(profile, DownloadsName);
        }
    }
}
