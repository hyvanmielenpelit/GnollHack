using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Threading.Tasks;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers the file dialog control's logic, places and behaviors. Each test works in its own directory
       under the system temp directory. Symbolic link tests return early where links cannot be created. */
    public class GHFileDialogTests : IDisposable
    {
        private const string Home = "/home/deck";
        private const string UserData = Home + "/.local/share/Steam/userdata";

        private readonly string _dir;

        public GHFileDialogTests()
        {
            _dir = Path.Combine(Path.GetTempPath(), "GnollHackFileDialogTests", Guid.NewGuid().ToString("N"));
            Directory.CreateDirectory(_dir);
        }

        public void Dispose()
        {
            try
            {
                Directory.Delete(_dir, true);
            }
            catch
            {
                /* Left for the system's temp cleanup */
            }
        }

        private string MakeFolder(params string[] segments)
        {
            string path = Path.Combine(new[] { _dir }.Concat(segments).ToArray());
            Directory.CreateDirectory(path);
            return path;
        }

        private static string MakeFile(string folder, string name, string content = "content")
        {
            string path = Path.Combine(folder, name);
            File.WriteAllText(path, content);
            return path;
        }

        private static bool TryLinkFolder(string link, string target)
        {
            try
            {
                Directory.CreateSymbolicLink(link, target);
                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }

        private static bool TryLinkFile(string link, string target)
        {
            try
            {
                File.CreateSymbolicLink(link, target);
                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }

        private static GHFileRow FindRow(GHFileDialogBehavior behavior, string name)
        {
            return behavior.Rows.First(r => r.Name == name);
        }

        private static IList<GHFileRow> ListRows(string path, GHFilePlace place, IList<string> extensions = null)
        {
            int omitted;
            IList<GHFileRow> rows = GHFileDialogLogic.ListFolder(path, place, extensions, 300, out omitted);
            Assert.Equal(0, omitted);
            return rows;
        }

        /* ParseUserDirs */

        [Fact]
        public void ParseUserDirs_ExpandsHomeAndIgnoresComments()
        {
            string text = "# This file is written by xdg-user-dirs-update\r\n"
                + "XDG_DOWNLOAD_DIR=\"$HOME/Downloads\"\r\n"
                + "\n"
                + "XDG_DOCUMENTS_DIR=\"/data/My Documents\"\n"
                + "XDG_PICTURES_DIR=\"$HOME/\"\n"
                + "not a setting\n";

            Dictionary<string, string> dirs = GHFileDialogLogic.ParseUserDirs(text, Home);

            Assert.Equal(3, dirs.Count);
            Assert.Equal(Home + "/Downloads", dirs["XDG_DOWNLOAD_DIR"]);
            Assert.Equal("/data/My Documents", dirs["XDG_DOCUMENTS_DIR"]);
            Assert.Equal(Home, dirs["XDG_PICTURES_DIR"]);
        }

        [Fact]
        public void ParseUserDirs_ToleratesNullAndRelativeValues()
        {
            Assert.Empty(GHFileDialogLogic.ParseUserDirs(null, Home));
            Assert.Empty(GHFileDialogLogic.ParseUserDirs("XDG_DOWNLOAD_DIR=\"Downloads\"", Home));
            Assert.Empty(GHFileDialogLogic.ParseUserDirs("XDG_DOWNLOAD_DIR=\"$HOMEWARD/x\"", Home));
        }

        /* ParseRemovableMedia */

        [Fact]
        public void ParseRemovableMedia_DecodesEscapesAndKeepsOnlyMedia()
        {
            string text = "/dev/nvme0n1p8 /home ext4 rw 0 0\n"
                + "/dev/mmcblk0p1 /run/media/deck/My\\040Card ext4 rw,nosuid 0 0\n"
                + "/dev/sda1 /run/media/mmcblk0p1 ext4 rw 0 0\n"
                + "tmpfs /run/media/ tmpfs rw 0 0\n"
                + "tmpfs /run/mediate/x tmpfs rw 0 0\n";

            IList<string> media = GHFileDialogLogic.ParseRemovableMedia(text);

            Assert.Equal(new[] { "/run/media/deck/My Card", "/run/media/mmcblk0p1" }, media);
        }

        [Fact]
        public void ParseRemovableMedia_ToleratesNull()
        {
            Assert.Empty(GHFileDialogLogic.ParseRemovableMedia(null));
        }

        /* IsInside */

        [Fact]
        public void IsInside_ComparesWholeSegments()
        {
            string root = Path.Combine(_dir, "b");

            Assert.True(GHFileDialogLogic.IsInside(root, root));
            Assert.True(GHFileDialogLogic.IsInside(root + Path.DirectorySeparatorChar, root));
            Assert.True(GHFileDialogLogic.IsInside(root, Path.Combine(root, "c", "d")));
            Assert.False(GHFileDialogLogic.IsInside(root, Path.Combine(_dir, "bc")));
            Assert.False(GHFileDialogLogic.IsInside(root, _dir));
            Assert.False(GHFileDialogLogic.IsInside(root, Path.Combine(root, "..", "bc")));
        }

        [Fact]
        public void IsInside_RespectsPlatformCase()
        {
            string root = Path.Combine(_dir, "Case");
            string other = Path.Combine(_dir, "CASE", "x");

            Assert.Equal(RuntimeInformation.IsOSPlatform(OSPlatform.Windows), GHFileDialogLogic.IsInside(root, other));
        }

        [Fact]
        public void IsInside_NullIsFalse()
        {
            Assert.False(GHFileDialogLogic.IsInside(null, _dir));
            Assert.False(GHFileDialogLogic.IsInside(_dir, null));
        }

        /* ResolvePath */

        [Fact]
        public void ResolvePath_NonLinkIsUnchanged()
        {
            string real = GHFileDialogLogic.ResolvePath(_dir);
            string file = MakeFile(real, "a.txt");

            Assert.Equal(file, GHFileDialogLogic.ResolvePath(file));
        }

        [Fact]
        public void ResolvePath_MissingTargetKeepsMissingTail()
        {
            string real = GHFileDialogLogic.ResolvePath(_dir);
            string missing = Path.Combine(real, "not", "there", "x.txt");

            Assert.Equal(missing, GHFileDialogLogic.ResolvePath(missing));
        }

        [Fact]
        public void ResolvePath_FollowsLinkToOutside()
        {
            string real = GHFileDialogLogic.ResolvePath(_dir);
            string outside = Directory.CreateDirectory(Path.Combine(real, "outside")).FullName;
            string place = Directory.CreateDirectory(Path.Combine(real, "place")).FullName;
            string link = Path.Combine(place, "escape");
            if (!TryLinkFolder(link, outside))
            {
                return;
            }

            Assert.Equal(outside, GHFileDialogLogic.ResolvePath(link));
            Assert.False(GHFileDialogLogic.IsInside(place, GHFileDialogLogic.ResolvePath(link)));
        }

        [Fact]
        public void ResolvePath_FollowsSymlinkedParentAndKeepsMissingTail()
        {
            string real = GHFileDialogLogic.ResolvePath(_dir);
            string target = Directory.CreateDirectory(Path.Combine(real, "target")).FullName;
            MakeFile(target, "a.txt");
            string link = Path.Combine(real, "link");
            if (!TryLinkFolder(link, target))
            {
                return;
            }

            Assert.Equal(Path.Combine(target, "a.txt"), GHFileDialogLogic.ResolvePath(Path.Combine(link, "a.txt")));
            Assert.Equal(Path.Combine(target, "new", "b.txt"), GHFileDialogLogic.ResolvePath(Path.Combine(link, "new", "b.txt")));
        }

        /* SanitizeFileName */

        [Theory]
        [InlineData("a.png", "a.png")]
        [InlineData("dir/sub/a.png", "a.png")]
        [InlineData("dir\\sub\\a.png", "a.png")]
        [InlineData("../../evil.txt", "evil.txt")]
        [InlineData("a:b*c?.png", "a_b_c_.png")]
        [InlineData("a\"b<c>d|e.png", "a_b_c_d_e.png")]
        [InlineData("trailing. ", "trailing")]
        [InlineData("..", "file")]
        [InlineData("", "file")]
        [InlineData(null, "file")]
        [InlineData("dir/", "file")]
        [InlineData("NUL.txt", "_NUL.txt")]
        public void SanitizeFileName_ReturnsValidBareName(string name, string expected)
        {
            Assert.Equal(expected, GHFileDialogLogic.SanitizeFileName(name));
        }

        [Fact]
        public void SanitizeFileName_ReplacesControlCharacters()
        {
            Assert.Equal("a_b", GHFileDialogLogic.SanitizeFileName("a" + (char)7 + "b"));
        }

        /* UniqueFileName */

        [Fact]
        public void UniqueFileName_FreeNameIsUnchanged()
        {
            Assert.Equal("a.png", GHFileDialogLogic.UniqueFileName("a.png", n => false));
        }

        [Fact]
        public void UniqueFileName_CountsFromTwo()
        {
            HashSet<string> taken = new HashSet<string> { "a.png" };
            Assert.Equal("a (2).png", GHFileDialogLogic.UniqueFileName("a.png", taken.Contains));

            taken.Add("a (2).png");
            Assert.Equal("a (3).png", GHFileDialogLogic.UniqueFileName("a.png", taken.Contains));
        }

        [Theory]
        [InlineData("name.tar.gz", "name.tar (2).gz")]
        [InlineData("noextension", "noextension (2)")]
        [InlineData(".hidden", ".hidden (2)")]
        public void UniqueFileName_KeepsLastExtension(string name, string expected)
        {
            Assert.Equal(expected, GHFileDialogLogic.UniqueFileName(name, n => n == name));
        }

        /* NormalizeExtensions */

        [Fact]
        public void NormalizeExtensions_NormalizesAndRemovesDuplicates()
        {
            Assert.Equal(new[] { "png", "jpg" }, GHFileDialogLogic.NormalizeExtensions(new[] { ".png", "*.PNG", "png", " JPG ", "" }));
        }

        [Fact]
        public void NormalizeExtensions_NullAndEmptyGiveNull()
        {
            Assert.Null(GHFileDialogLogic.NormalizeExtensions(null));
            Assert.Null(GHFileDialogLogic.NormalizeExtensions(new string[0]));
            Assert.Null(GHFileDialogLogic.NormalizeExtensions(new[] { "", "  " }));
            Assert.Null(GHFileDialogLogic.NormalizeExtensions(new[] { "*.*" }));
        }

        /* ListFolder */

        [Fact]
        public void ListFolder_FoldersFirstThenNewestFiles()
        {
            string place = MakeFolder("place");
            Directory.CreateDirectory(Path.Combine(place, "zeta"));
            Directory.CreateDirectory(Path.Combine(place, "Alpha"));
            File.SetLastWriteTimeUtc(MakeFile(place, "old.txt"), new DateTime(2020, 1, 1, 0, 0, 0, DateTimeKind.Utc));
            File.SetLastWriteTimeUtc(MakeFile(place, "new.txt"), new DateTime(2024, 1, 1, 0, 0, 0, DateTimeKind.Utc));
            File.SetLastWriteTimeUtc(MakeFile(place, "mid.txt"), new DateTime(2022, 1, 1, 0, 0, 0, DateTimeKind.Utc));

            IList<GHFileRow> rows = ListRows(place, new GHFilePlace("P", place));

            Assert.Equal(new[] { "Alpha", "zeta", "new.txt", "mid.txt", "old.txt" }, rows.Select(r => r.Name));
            Assert.Equal(GHFileRowKind.Folder, rows[0].Kind);
            Assert.Equal(GHFileRowKind.File, rows[2].Kind);
            Assert.Equal(Path.Combine(place, "new.txt"), rows[2].Path);
            Assert.Equal(GHFileDialogLogic.FormatDetail(new DateTime(2024, 1, 1, 0, 0, 0, DateTimeKind.Utc), 7), rows[2].Detail);
            Assert.Equal("P", rows[2].Place.Name);
        }

        [Fact]
        public void ListFolder_FiltersFilesByExtensionButNotFolders()
        {
            string place = MakeFolder("place");
            Directory.CreateDirectory(Path.Combine(place, "folder.txt"));
            MakeFile(place, "a.PNG");
            MakeFile(place, "b.txt");

            IList<GHFileRow> rows = ListRows(place, new GHFilePlace("P", place), GHFileDialogLogic.NormalizeExtensions(new[] { "*.png" }));

            Assert.Equal(new[] { "folder.txt", "a.PNG" }, rows.Select(r => r.Name));
        }

        [Fact]
        public void ListFolder_SkipsDotNames()
        {
            string place = MakeFolder("place");
            Directory.CreateDirectory(Path.Combine(place, ".config"));
            MakeFile(place, ".secret");
            MakeFile(place, "shown.txt");

            Assert.Equal(new[] { "shown.txt" }, ListRows(place, new GHFilePlace("P", place)).Select(r => r.Name));
        }

        [Fact]
        public void ListFolder_SkipsHiddenAttributeOnWindows()
        {
            if (!RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
            {
                return;
            }

            string place = MakeFolder("place");
            string hidden = MakeFile(place, "hidden.txt");
            File.SetAttributes(hidden, FileAttributes.Hidden);
            MakeFile(place, "shown.txt");

            Assert.Equal(new[] { "shown.txt" }, ListRows(place, new GHFilePlace("P", place)).Select(r => r.Name));
        }

        [Fact]
        public void ListFolder_SkipsThumbnailsOnlyWhenPlaceAsks()
        {
            string place = MakeFolder("place");
            Directory.CreateDirectory(Path.Combine(place, "thumbnails"));
            MakeFile(place, "a.jpg");

            Assert.Equal(new[] { "a.jpg" }, ListRows(place, new GHFilePlace("S", place, false, true)).Select(r => r.Name));
            Assert.Equal(new[] { "thumbnails", "a.jpg" }, ListRows(place, new GHFilePlace("P", place)).Select(r => r.Name));
        }

        [Fact]
        public void ListFolder_CountsOmittedPastMaxRows()
        {
            string place = MakeFolder("place");
            Directory.CreateDirectory(Path.Combine(place, "f1"));
            Directory.CreateDirectory(Path.Combine(place, "f2"));
            MakeFile(place, "a.txt");
            MakeFile(place, "b.txt");

            int omitted;
            IList<GHFileRow> rows = GHFileDialogLogic.ListFolder(place, new GHFilePlace("P", place), null, 3, out omitted);

            Assert.Equal(3, rows.Count);
            Assert.Equal(1, omitted);
            Assert.Equal(GHFileRowKind.File, rows[2].Kind);
        }

        [Fact]
        public void ListFolder_MissingFolderIsEmptyWithoutThrowing()
        {
            string place = MakeFolder("place");
            string missing = Path.Combine(place, "missing");

            int omitted;
            IList<GHFileRow> rows = GHFileDialogLogic.ListFolder(missing, new GHFilePlace("P", place), null, 3, out omitted);

            Assert.Empty(rows);
            Assert.Equal(0, omitted);
        }

        [Fact]
        public void ListFolder_FolderOutsideThePlaceIsEmpty()
        {
            string place = MakeFolder("place");
            string other = MakeFolder("other");
            MakeFile(other, "a.txt");

            Assert.Empty(ListRows(other, new GHFilePlace("P", place)));
        }

        [Fact]
        public void ListFolder_SkipsLinksThatEscapeAndKeepsLinksInside()
        {
            string place = MakeFolder("place");
            string inner = MakeFolder("place", "inner");
            string innerFile = MakeFile(inner, "a.txt");
            string outside = MakeFolder("outside");
            string outsideFile = MakeFile(outside, "secret.txt");
            if (!TryLinkFolder(Path.Combine(place, "out"), outside) || !TryLinkFile(Path.Combine(place, "secret.txt"), outsideFile)
                || !TryLinkFolder(Path.Combine(place, "innerlink"), inner) || !TryLinkFile(Path.Combine(place, "filelink.txt"), innerFile))
            {
                return;
            }

            IList<GHFileRow> rows = ListRows(place, new GHFilePlace("P", place));

            Assert.Equal(new[] { "inner", "innerlink", "filelink.txt" }, rows.Select(r => r.Name));
            Assert.Equal(GHFileRowKind.Folder, rows[1].Kind);
            Assert.Equal(GHFileRowKind.File, rows[2].Kind);
        }

        /* FormatDetail and FormatSize */

        [Fact]
        public void FormatDetail_UsesLocalTimeAndSize()
        {
            DateTime utc = new DateTime(2026, 10, 30, 12, 34, 0, DateTimeKind.Utc);
            string expected = utc.ToLocalTime().ToString("yyyy-MM-dd HH:mm", System.Globalization.CultureInfo.InvariantCulture) + " " + (char)0x00B7 + " 1.5 KB";

            Assert.Equal(expected, GHFileDialogLogic.FormatDetail(utc, 1536));
        }

        [Theory]
        [InlineData(-1L, "0 B")]
        [InlineData(0L, "0 B")]
        [InlineData(1023L, "1023 B")]
        [InlineData(1024L, "1.0 KB")]
        [InlineData(1536L, "1.5 KB")]
        [InlineData(1048576L, "1.0 MB")]
        [InlineData(209715200L, "200 MB")]
        [InlineData(3221225472L, "3.0 GB")]
        public void FormatSize_PicksUnit(long bytes, string expected)
        {
            Assert.Equal(expected, GHFileDialogLogic.FormatSize(bytes));
        }

        /* CopyNoClobber and CopyReplacing */

        [Fact]
        public void CopyNoClobber_CreatesDirectoryAndLeavesNoTemporaryFile()
        {
            string source = MakeFile(MakeFolder("source"), "a.bin", "source bytes");
            string directory = Path.Combine(_dir, "new", "folder");

            string final = GHFileDialogLogic.CopyNoClobber(source, directory, "a.bin");

            Assert.Equal(Path.Combine(directory, "a.bin"), final);
            Assert.Equal("source bytes", File.ReadAllText(final));
            Assert.Equal(new[] { final }, Directory.GetFileSystemEntries(directory));
        }

        [Fact]
        public void CopyNoClobber_ExistingTargetsStayByteIdentical()
        {
            string source = MakeFile(MakeFolder("source"), "a.bin", "source bytes");
            string directory = MakeFolder("target");
            byte[] existing = { 1, 2, 3, 0, 255 };
            string first = Path.Combine(directory, "a.bin");
            File.WriteAllBytes(first, existing);

            string second = GHFileDialogLogic.CopyNoClobber(source, directory, "a.bin");
            string third = GHFileDialogLogic.CopyNoClobber(source, directory, "a.bin");

            Assert.Equal(Path.Combine(directory, "a (2).bin"), second);
            Assert.Equal(Path.Combine(directory, "a (3).bin"), third);
            Assert.Equal(existing, File.ReadAllBytes(first));
            Assert.Equal("source bytes", File.ReadAllText(second));
            Assert.Equal("source bytes", File.ReadAllText(source));
            Assert.Equal(3, Directory.GetFileSystemEntries(directory).Length);
        }

        [Fact]
        public void CopyNoClobber_DoesNotWriteThroughLinkAtTarget()
        {
            string source = MakeFile(MakeFolder("source"), "a.bin", "source bytes");
            string directory = MakeFolder("target");
            string victim = MakeFile(MakeFolder("other"), "victim.bin", "victim");
            if (!TryLinkFile(Path.Combine(directory, "a.bin"), victim))
            {
                return;
            }

            string final = GHFileDialogLogic.CopyNoClobber(source, directory, "a.bin");

            Assert.Equal(Path.Combine(directory, "a (2).bin"), final);
            Assert.Equal("victim", File.ReadAllText(victim));
        }

        [Fact]
        public void CopyNoClobber_MissingSourceThrowsAndLeavesNothing()
        {
            string directory = MakeFolder("target");

            Assert.ThrowsAny<IOException>(() => GHFileDialogLogic.CopyNoClobber(Path.Combine(_dir, "missing.bin"), directory, "a.bin"));

            Assert.Empty(Directory.GetFileSystemEntries(directory));
        }

        [Fact]
        public void ReserveUniqueName_NameTakenAtTheCheckMovesToNextName()
        {
            string directory = MakeFolder("target");
            string racer = Path.Combine(directory, "a.bin");
            bool raced = false;

            string reserved = GHFileDialogLogic.ReserveUniqueName(directory, "a.bin", candidate =>
            {
                if (!raced)
                {
                    raced = true;
                    File.WriteAllText(racer, "the other writer");
                }
            });

            Assert.Equal(Path.Combine(directory, "a (2).bin"), reserved);
            Assert.Equal("the other writer", File.ReadAllText(racer));
            Assert.Equal(0, new FileInfo(reserved).Length);
        }

        [Fact]
        public void CheckFreeSpace_ThrowsClearMessageWhenShort()
        {
            GHFileDialogLogic.CheckFreeSpace(100, 100);

            IOException ex = Assert.Throws<IOException>(() => GHFileDialogLogic.CheckFreeSpace(2048, 1024));
            Assert.Contains("free space", ex.Message);
            Assert.Contains("2.0 KB", ex.Message);
        }

        [Fact]
        public void CopyReplacing_ReplacesTargetAndLeavesNoTemporaryFile()
        {
            string source = MakeFile(MakeFolder("source"), "a.bin", "new content");
            string directory = MakeFolder("target");
            string target = MakeFile(directory, "b.bin", "old");

            GHFileDialogLogic.CopyReplacing(source, target);

            Assert.Equal("new content", File.ReadAllText(target));
            Assert.Equal("new content", File.ReadAllText(source));
            Assert.Equal(new[] { target }, Directory.GetFileSystemEntries(directory));
        }

        [Fact]
        public void CopyReplacing_MissingSourceKeepsTarget()
        {
            string directory = MakeFolder("target");
            string target = MakeFile(directory, "b.bin", "old");

            Assert.ThrowsAny<IOException>(() => GHFileDialogLogic.CopyReplacing(Path.Combine(_dir, "missing.bin"), target));

            Assert.Equal("old", File.ReadAllText(target));
            Assert.Equal(new[] { target }, Directory.GetFileSystemEntries(directory));
        }

        /* IsProtectedTarget */

        [Fact]
        public void IsProtectedTarget_InsideFolderOrSameAsSource()
        {
            string data = MakeFolder("data");
            string source = MakeFile(MakeFolder("source"), "a.bin");
            string[] folders = { data, null, "" };

            Assert.True(GHFileDialogLogic.IsProtectedTarget(data, folders, source));
            Assert.True(GHFileDialogLogic.IsProtectedTarget(Path.Combine(data, "sub", "x.bin"), folders, source));
            Assert.True(GHFileDialogLogic.IsProtectedTarget(source, folders, source));
            Assert.False(GHFileDialogLogic.IsProtectedTarget(Path.Combine(_dir, "database", "x.bin"), folders, source));
            Assert.False(GHFileDialogLogic.IsProtectedTarget(Path.Combine(_dir, "free"), null, source));
            Assert.False(GHFileDialogLogic.IsProtectedTarget(Path.Combine(_dir, "free"), folders, null));
        }

        [Fact]
        public void IsProtectedTarget_SeesThroughLinks()
        {
            string data = MakeFolder("data");
            string source = MakeFile(MakeFolder("source"), "a.bin");
            string folderLink = Path.Combine(_dir, "datalink");
            string sourceLink = Path.Combine(_dir, "sourcelink.bin");
            if (!TryLinkFolder(folderLink, data) || !TryLinkFile(sourceLink, source))
            {
                return;
            }

            Assert.True(GHFileDialogLogic.IsProtectedTarget(Path.Combine(folderLink, "new", "x.bin"), new[] { data }, source));
            Assert.True(GHFileDialogLogic.IsProtectedTarget(sourceLink, new[] { data }, source));
        }

        /* GHFreedesktopFilePlaces */

        private sealed class FakeSystem
        {
            public readonly Dictionary<string, string> Environment = new Dictionary<string, string>();
            public readonly Dictionary<string, string> Files = new Dictionary<string, string>();
            public readonly HashSet<string> Directories = new HashSet<string>();
            public readonly Dictionary<string, string[]> Listings = new Dictionary<string, string[]>();

            public GHFreedesktopFilePlaces CreatePlaces()
            {
                return new GHFreedesktopFilePlaces(
                    name => Environment.TryGetValue(name, out string value) ? value : null,
                    Home,
                    path => Files.TryGetValue(path, out string text) ? text : null,
                    path => Directories.Contains(path),
                    path => Listings.TryGetValue(path, out string[] list) ? list : new string[0]);
            }
        }

        private static FakeSystem CreateFullSystem()
        {
            FakeSystem system = new FakeSystem();
            system.Directories.Add(Home + "/Pictures");
            system.Directories.Add(Home + "/Documents");
            system.Directories.Add(Home + "/Downloads");
            system.Directories.Add("/run/media/deck/My Card");
            system.Files["/proc/self/mounts"] = "/dev/mmcblk0p1 /run/media/deck/My\\040Card ext4 rw 0 0\n";
            system.Environment["SteamAppId"] = "2881053371";
            system.Listings[UserData] = new[] { UserData + "/111", UserData + "/222" };
            system.Directories.Add(UserData + "/222/760/remote/2881053371/screenshots");
            return system;
        }

        [Fact]
        public void Freedesktop_OpenPlacesInOrder()
        {
            IList<GHFilePlace> places = CreateFullSystem().CreatePlaces().GetOpenPlaces(false);

            Assert.Equal(new[] { "Pictures", "GnollHack Screenshots", "Documents", "Downloads", "My Card" }, places.Select(p => p.Name));
            Assert.Equal(new[] { Home + "/Pictures", UserData + "/222/760/remote/2881053371/screenshots", Home + "/Documents", Home + "/Downloads", "/run/media/deck/My Card" },
                places.Select(p => p.Path));
            Assert.True(places[1].HideThumbnails);
            Assert.False(places[0].HideThumbnails);
            Assert.All(places, p => Assert.False(p.CreateOnUse));
        }

        [Fact]
        public void Freedesktop_ImportStartsWithExports()
        {
            FakeSystem system = CreateFullSystem();
            system.Directories.Add(Home + "/Documents/GnollHack");

            IList<GHFilePlace> places = system.CreatePlaces().GetOpenPlaces(true);

            Assert.Equal(new[] { "GnollHack Exports", "Pictures", "GnollHack Screenshots", "Documents", "Downloads", "My Card" }, places.Select(p => p.Name));
            Assert.Equal(Home + "/Documents/GnollHack", places[0].Path);
        }

        [Fact]
        public void Freedesktop_MissingFoldersAreLeftOutOfOpen()
        {
            FakeSystem system = new FakeSystem();
            system.Directories.Add(Home + "/Downloads");

            Assert.Equal(new[] { "Downloads" }, system.CreatePlaces().GetOpenPlaces(false).Select(p => p.Name));
            Assert.Equal(new[] { "Downloads" }, system.CreatePlaces().GetOpenPlaces(true).Select(p => p.Name));
        }

        [Fact]
        public void Freedesktop_SaveHasDocumentsAndDownloadsEvenWhenMissing()
        {
            IList<GHFilePlace> places = new FakeSystem().CreatePlaces().GetSavePlaces();

            Assert.Equal(new[] { "Documents/GnollHack", "Downloads/GnollHack" }, places.Select(p => p.Name));
            Assert.Equal(new[] { Home + "/Documents/GnollHack", Home + "/Downloads/GnollHack" }, places.Select(p => p.Path));
            Assert.All(places, p => Assert.True(p.CreateOnUse));
        }

        [Fact]
        public void Freedesktop_SaveIncludesMedia()
        {
            IList<GHFilePlace> places = CreateFullSystem().CreatePlaces().GetSavePlaces();

            Assert.Equal(new[] { "Documents/GnollHack", "Downloads/GnollHack", "My Card/GnollHack" }, places.Select(p => p.Name));
            Assert.Equal("/run/media/deck/My Card/GnollHack", places[2].Path);
        }

        [Fact]
        public void Freedesktop_UserDirsAreHonoredAndDuplicatesRemoved()
        {
            FakeSystem system = new FakeSystem();
            system.Files[Home + "/.config/user-dirs.dirs"] = "XDG_DOCUMENTS_DIR=\"$HOME/Stuff\"\nXDG_DOWNLOAD_DIR=\"$HOME/Stuff\"\nXDG_PICTURES_DIR=\"/data/pics\"\n";
            system.Directories.Add(Home + "/Stuff");
            system.Directories.Add("/data/pics");
            GHFreedesktopFilePlaces places = system.CreatePlaces();

            IList<GHFilePlace> open = places.GetOpenPlaces(false);
            Assert.Equal(new[] { "Pictures", "Documents" }, open.Select(p => p.Name));
            Assert.Equal(new[] { "/data/pics", Home + "/Stuff" }, open.Select(p => p.Path));

            IList<GHFilePlace> save = places.GetSavePlaces();
            Assert.Equal(new[] { "Documents/GnollHack" }, save.Select(p => p.Name));
            Assert.Equal(Home + "/Stuff/GnollHack", save[0].Path);
        }

        [Fact]
        public void Freedesktop_HonorsXdgConfigHome()
        {
            FakeSystem system = new FakeSystem();
            system.Environment["XDG_CONFIG_HOME"] = "/custom/config";
            system.Files["/custom/config/user-dirs.dirs"] = "XDG_DOCUMENTS_DIR=\"/custom/docs\"\n";
            system.Files[Home + "/.config/user-dirs.dirs"] = "XDG_DOCUMENTS_DIR=\"/wrong/docs\"\n";

            Assert.Equal("/custom/docs/GnollHack", system.CreatePlaces().GetSavePlaces()[0].Path);
        }

        [Fact]
        public void Freedesktop_ZeroAppIdUsesHighBitsOfGameId()
        {
            FakeSystem system = CreateFullSystem();
            system.Environment["SteamAppId"] = "0";
            system.Environment["SteamGameId"] = ((2881053371UL << 32) | 0x02000000UL).ToString();

            Assert.Contains(system.CreatePlaces().GetOpenPlaces(false), p => p.Name == "GnollHack Screenshots");
        }

        [Fact]
        public void Freedesktop_NoSteamIdsLeavesScreenshotsOut()
        {
            FakeSystem system = CreateFullSystem();
            system.Environment.Remove("SteamAppId");
            Assert.DoesNotContain(system.CreatePlaces().GetOpenPlaces(false), p => p.Name == "GnollHack Screenshots");

            system.Environment["SteamAppId"] = "0";
            system.Environment["SteamGameId"] = "12345";
            Assert.DoesNotContain(system.CreatePlaces().GetOpenPlaces(false), p => p.Name == "GnollHack Screenshots");
        }

        [Fact]
        public void Freedesktop_FirstMatchingUserWins()
        {
            FakeSystem system = CreateFullSystem();
            system.Directories.Add(UserData + "/111/760/remote/2881053371/screenshots");

            GHFilePlace screenshots = system.CreatePlaces().GetOpenPlaces(false).First(p => p.Name == "GnollHack Screenshots");

            Assert.Equal(UserData + "/111/760/remote/2881053371/screenshots", screenshots.Path);
        }

        /* GHWindowsFilePlaces */

        [Fact]
        public void Windows_SavePlacesAreCreatedOnUseUnderGnollHack()
        {
            IList<GHFilePlace> places = new GHWindowsFilePlaces().GetSavePlaces();

            Assert.All(places, p =>
            {
                Assert.True(p.CreateOnUse);
                Assert.Equal("GnollHack", Path.GetFileName(p.Path));
                Assert.Contains(p.Name, new[] { "Documents/GnollHack", "Downloads/GnollHack" });
            });
        }

        [Fact]
        public void Windows_OpenPlacesExistAndHaveNoScreenshots()
        {
            IList<GHFilePlace> places = new GHWindowsFilePlaces().GetOpenPlaces(true);

            Assert.All(places, p =>
            {
                Assert.True(Directory.Exists(p.Path));
                Assert.Contains(p.Name, new[] { "GnollHack Exports", "Pictures", "Documents", "Downloads" });
            });
        }

        [Fact]
        public void Current_IsNullWithoutAvalonia()
        {
            Assert.Null(GHFilePlaces.Current);
        }

        /* GHFileOpenBehavior */

        private GHFilePlace MakePlaceWithTree(string name)
        {
            string place = MakeFolder(name);
            string sub = MakeFolder(name, "sub");
            MakeFile(place, "a.png");
            MakeFile(place, "b.png");
            MakeFile(sub, "c.png");
            return new GHFilePlace(name, place);
        }

        private static GHFileOpenBehavior CreateOpen(GHFilePlace place, bool multiple = false, int maxFiles = 5, IEnumerable<string> extensions = null)
        {
            return new GHFileOpenBehavior("Title", "Attach", multiple, maxFiles, extensions, new[] { place });
        }

        [Fact]
        public async Task Open_StartsAtPlaces()
        {
            GHFilePlace place = MakePlaceWithTree("Pictures");
            GHFileOpenBehavior behavior = CreateOpen(place);
            int changes = 0;
            behavior.Changed += () => changes++;

            await behavior.LoadAsync();

            Assert.Equal(1, changes);
            Assert.Equal("Title", behavior.Title);
            Assert.False(behavior.CanGoUp);
            Assert.False(behavior.CanConfirm);
            Assert.Null(behavior.ConfirmText);
            Assert.Equal("Choose a place", behavior.Subtitle);
            GHFileRow row = Assert.Single(behavior.Rows);
            Assert.Equal(GHFileRowKind.Place, row.Kind);
            Assert.Equal("Pictures", row.Name);
            Assert.Equal(place.Path, row.Detail);
        }

        [Fact]
        public async Task Open_NoPlacesShowsMessageRow()
        {
            GHFileOpenBehavior behavior = new GHFileOpenBehavior("Title", "Attach", false, 5, null, new GHFilePlace[0]);

            await behavior.LoadAsync();

            Assert.Equal(GHFileRowKind.Message, Assert.Single(behavior.Rows).Kind);
        }

        [Fact]
        public async Task Open_GoUpWalksFoldersThenPlaces()
        {
            GHFileOpenBehavior behavior = CreateOpen(MakePlaceWithTree("Pictures"));
            await behavior.LoadAsync();

            await behavior.ActivateRowAsync(behavior.Rows[0]);
            Assert.True(behavior.CanGoUp);
            Assert.Equal("Pictures", behavior.Subtitle);
            Assert.Equal(GHFileRowKind.Folder, behavior.Rows[0].Kind);

            await behavior.ActivateRowAsync(FindRow(behavior, "sub"));
            Assert.Equal("Pictures " + (char)0x203A + " sub", behavior.Subtitle);
            Assert.Equal("c.png", Assert.Single(behavior.Rows).Name);

            await behavior.GoUpAsync();
            Assert.Equal("Pictures", behavior.Subtitle);
            Assert.Contains(behavior.Rows, r => r.Name == "sub");

            await behavior.GoUpAsync();
            Assert.False(behavior.CanGoUp);
            Assert.Equal("Choose a place", behavior.Subtitle);
            Assert.Equal(GHFileRowKind.Place, behavior.Rows[0].Kind);

            await behavior.GoUpAsync();
            Assert.False(behavior.Result.IsCompleted);
        }

        [Fact]
        public async Task Open_MissingFolderShowsMessage()
        {
            GHFileOpenBehavior behavior = CreateOpen(new GHFilePlace("Gone", Path.Combine(_dir, "gone")));
            await behavior.LoadAsync();

            await behavior.ActivateRowAsync(behavior.Rows[0]);

            GHFileRow row = Assert.Single(behavior.Rows);
            Assert.Equal(GHFileRowKind.Message, row.Kind);
            Assert.Equal("This folder is empty.", row.Name);

            await behavior.ActivateRowAsync(row);
            Assert.False(behavior.Result.IsCompleted);
        }

        [Fact]
        public async Task Open_ExtensionsFilterFiles()
        {
            GHFilePlace place = MakePlaceWithTree("Pictures");
            MakeFile(place.Path, "skip.txt");
            GHFileOpenBehavior behavior = CreateOpen(place, extensions: new[] { ".PNG" });
            await behavior.LoadAsync();

            await behavior.ActivateRowAsync(behavior.Rows[0]);

            Assert.DoesNotContain(behavior.Rows, r => r.Name == "skip.txt");
            Assert.Contains(behavior.Rows, r => r.Name == "a.png");
        }

        [Fact]
        public async Task Open_OmittedRowsSetNotice()
        {
            string place = MakeFolder("many");
            for (int i = 0; i <= GHFileOpenBehavior.MaxRows; i++)
            {
                File.WriteAllBytes(Path.Combine(place, "f" + i + ".txt"), new byte[0]);
            }

            GHFileOpenBehavior behavior = CreateOpen(new GHFilePlace("Many", place));
            await behavior.LoadAsync();

            await behavior.ActivateRowAsync(behavior.Rows[0]);

            Assert.Equal(GHFileOpenBehavior.MaxRows, behavior.Rows.Count);
            Assert.Equal("Showing the newest 300 items.", behavior.Notice);

            await behavior.GoUpAsync();
            Assert.Null(behavior.Notice);
        }

        [Fact]
        public async Task Open_SingleModeCompletesWithFileOnce()
        {
            GHFilePlace place = MakePlaceWithTree("Pictures");
            GHFileOpenBehavior behavior = CreateOpen(place);
            await behavior.LoadAsync();
            await behavior.ActivateRowAsync(behavior.Rows[0]);
            GHFileRow file = FindRow(behavior, "a.png");
            GHFileRow other = FindRow(behavior, "b.png");

            await behavior.ActivateRowAsync(file);
            Assert.True(behavior.Result.IsCompleted);

            behavior.Cancel();
            behavior.HostGone();
            behavior.Confirm();
            await behavior.ActivateRowAsync(other);

            Assert.Equal(new[] { file.Path }, await behavior.Result);
        }

        [Fact]
        public async Task Open_CancelCompletesWithNullOnce()
        {
            GHFileOpenBehavior behavior = CreateOpen(MakePlaceWithTree("Pictures"));
            await behavior.LoadAsync();

            behavior.Cancel();
            await behavior.ActivateRowAsync(behavior.Rows[0]);

            Assert.True(behavior.Result.IsCompleted);
            Assert.Null(await behavior.Result);
            Assert.Equal(GHFileRowKind.Place, behavior.Rows[0].Kind);
        }

        [Fact]
        public void Open_ResultRunsContinuationsAsynchronously()
        {
            GHFileOpenBehavior behavior = CreateOpen(MakePlaceWithTree("Pictures"));

            Assert.NotEqual((TaskCreationOptions)0, behavior.Result.CreationOptions & TaskCreationOptions.RunContinuationsAsynchronously);
        }

        [Fact]
        public async Task Open_MarksSurviveFoldersAndAreReturnedInOrder()
        {
            GHFileOpenBehavior behavior = CreateOpen(MakePlaceWithTree("Pictures"), true);
            Assert.Equal("Attach", behavior.ConfirmText);
            await behavior.LoadAsync();
            await behavior.ActivateRowAsync(behavior.Rows[0]);

            GHFileRow a = FindRow(behavior, "a.png");
            await behavior.ActivateRowAsync(a);
            Assert.True(a.IsMarked);
            Assert.Equal("a.png", a.Name);
            Assert.True(behavior.CanConfirm);
            Assert.False(behavior.Result.IsCompleted);
            Assert.Equal("Pictures (1 selected)", behavior.Subtitle);

            await behavior.ActivateRowAsync(FindRow(behavior, "sub"));
            GHFileRow c = FindRow(behavior, "c.png");
            Assert.False(c.IsMarked);
            await behavior.ActivateRowAsync(c);
            Assert.Equal("Pictures " + (char)0x203A + " sub (2 selected)", behavior.Subtitle);

            await behavior.GoUpAsync();
            Assert.True(FindRow(behavior, "a.png").IsMarked);
            Assert.False(FindRow(behavior, "b.png").IsMarked);
            await behavior.GoUpAsync();
            Assert.Equal("Choose a place (2 selected)", behavior.Subtitle);

            behavior.Confirm();

            Assert.Equal(new[] { a.Path, c.Path }, await behavior.Result);
        }

        [Fact]
        public async Task Open_ToggleRemovesMark()
        {
            GHFileOpenBehavior behavior = CreateOpen(MakePlaceWithTree("Pictures"), true);
            await behavior.LoadAsync();
            await behavior.ActivateRowAsync(behavior.Rows[0]);
            GHFileRow a = FindRow(behavior, "a.png");

            await behavior.ActivateRowAsync(a);
            await behavior.ActivateRowAsync(a);

            Assert.False(a.IsMarked);
            Assert.False(behavior.CanConfirm);
            behavior.Confirm();
            Assert.False(behavior.Result.IsCompleted);
        }

        [Fact]
        public async Task Open_RefusesPastFileLimit()
        {
            GHFileOpenBehavior behavior = CreateOpen(MakePlaceWithTree("Pictures"), true, 1);
            await behavior.LoadAsync();
            await behavior.ActivateRowAsync(behavior.Rows[0]);
            GHFileRow a = FindRow(behavior, "a.png");
            GHFileRow b = FindRow(behavior, "b.png");

            await behavior.ActivateRowAsync(a);
            Assert.Null(behavior.Notice);

            await behavior.ActivateRowAsync(b);
            Assert.False(b.IsMarked);
            Assert.Equal("You can select up to 1 files.", behavior.Notice);

            await behavior.ActivateRowAsync(a);
            Assert.Null(behavior.Notice);
            await behavior.ActivateRowAsync(b);
            Assert.True(b.IsMarked);
        }

        [Fact]
        public async Task Open_ConfirmDropsMissingFiles()
        {
            GHFileOpenBehavior behavior = CreateOpen(MakePlaceWithTree("Pictures"), true);
            await behavior.LoadAsync();
            await behavior.ActivateRowAsync(behavior.Rows[0]);
            GHFileRow a = FindRow(behavior, "a.png");
            GHFileRow b = FindRow(behavior, "b.png");
            await behavior.ActivateRowAsync(a);
            await behavior.ActivateRowAsync(b);
            File.Delete(a.Path);

            behavior.Confirm();

            Assert.Equal(new[] { b.Path }, await behavior.Result);
        }

        [Fact]
        public async Task Open_ConfirmStaysWithNoticeWhenNoneRemain()
        {
            GHFileOpenBehavior behavior = CreateOpen(MakePlaceWithTree("Pictures"), true);
            await behavior.LoadAsync();
            await behavior.ActivateRowAsync(behavior.Rows[0]);
            GHFileRow a = FindRow(behavior, "a.png");
            await behavior.ActivateRowAsync(a);
            File.Delete(a.Path);

            behavior.Confirm();

            Assert.False(behavior.Result.IsCompleted);
            Assert.NotNull(behavior.Notice);
            Assert.False(behavior.CanConfirm);
            Assert.False(a.IsMarked);
        }

        [Fact]
        public async Task Open_ConfirmDropsFileThatBecameAnEscapingLink()
        {
            GHFileOpenBehavior behavior = CreateOpen(MakePlaceWithTree("Pictures"), true);
            string outside = MakeFile(MakeFolder("outside"), "secret.png");
            await behavior.LoadAsync();
            await behavior.ActivateRowAsync(behavior.Rows[0]);
            GHFileRow a = FindRow(behavior, "a.png");
            await behavior.ActivateRowAsync(a);
            File.Delete(a.Path);
            if (!TryLinkFile(a.Path, outside))
            {
                return;
            }

            behavior.Confirm();

            Assert.False(behavior.Result.IsCompleted);
            Assert.False(behavior.CanConfirm);
        }

        [Fact]
        public async Task Open_HostGoneCompletesWithNull()
        {
            GHFileOpenBehavior behavior = CreateOpen(MakePlaceWithTree("Pictures"));
            await behavior.LoadAsync();

            behavior.HostGone();

            Assert.True(behavior.Result.IsCompleted);
            Assert.Null(await behavior.Result);
        }

        [Fact]
        public async Task Open_WritesNothing()
        {
            GHFilePlace place = MakePlaceWithTree("Pictures");
            string[] before = Directory.GetFileSystemEntries(place.Path, "*", SearchOption.AllDirectories);
            GHFileOpenBehavior behavior = CreateOpen(place, true);
            await behavior.LoadAsync();
            await behavior.ActivateRowAsync(behavior.Rows[0]);
            await behavior.ActivateRowAsync(FindRow(behavior, "a.png"));
            behavior.Confirm();
            await behavior.Result;

            Assert.Equal(before, Directory.GetFileSystemEntries(place.Path, "*", SearchOption.AllDirectories));
        }

        /* GHFileSaveBehavior */

        private GHFileSaveBehavior CreateSave(out string source, out string targetFolder, string[] protectedFolders = null)
        {
            source = MakeFile(MakeFolder("source"), "dump.bin", "source bytes");
            targetFolder = Path.Combine(_dir, "Documents", "GnollHack");
            GHFilePlace place = new GHFilePlace("Documents/GnollHack", targetFolder, true);
            return new GHFileSaveBehavior("Save", source, "dump.bin", new[] { place }, protectedFolders);
        }

        [Fact]
        public async Task Save_ShowsOnePlaceRowPerPlace()
        {
            string source = MakeFile(MakeFolder("source"), "dump.bin", new string('x', 2048));
            GHFilePlace first = new GHFilePlace("Documents/GnollHack", Path.Combine(_dir, "d"), true);
            GHFilePlace second = new GHFilePlace("Downloads/GnollHack", Path.Combine(_dir, "e"), true);
            GHFileSaveBehavior behavior = new GHFileSaveBehavior("Save", source, "a?.bin", new[] { first, second }, null);

            await behavior.LoadAsync();

            Assert.Null(behavior.ConfirmText);
            Assert.False(behavior.CanConfirm);
            Assert.False(behavior.CanGoUp);
            Assert.Equal("a_.bin " + (char)0x00B7 + " 2.0 KB", behavior.Subtitle);
            Assert.Equal(2, behavior.Rows.Count);
            Assert.Equal(GHFileRowKind.Place, behavior.Rows[1].Kind);
            Assert.Equal("Downloads/GnollHack", behavior.Rows[1].Name);
            Assert.Equal(second.Path, behavior.Rows[1].Detail);
        }

        [Fact]
        public async Task Save_CopiesAndAcknowledgeCompletesWithFinalPath()
        {
            string source;
            string target;
            GHFileSaveBehavior behavior = CreateSave(out source, out target);
            await behavior.LoadAsync();

            await behavior.ActivateRowAsync(behavior.Rows[0]);

            string final = Path.Combine(target, "dump.bin");
            Assert.False(behavior.IsBusy);
            Assert.Equal("Saved to Documents/GnollHack/dump.bin", behavior.StatusMessage);
            Assert.False(behavior.Result.IsCompleted);
            Assert.Equal("source bytes", File.ReadAllText(final));

            behavior.AcknowledgeStatus();

            Assert.Equal(new[] { final }, await behavior.Result);
        }

        [Fact]
        public async Task Save_SecondCopyGetsNextNameAndExistingStaysByteIdentical()
        {
            string source;
            string target;
            GHFileSaveBehavior behavior = CreateSave(out source, out target);
            Directory.CreateDirectory(target);
            byte[] existing = { 9, 8, 7 };
            string first = Path.Combine(target, "dump.bin");
            File.WriteAllBytes(first, existing);
            await behavior.LoadAsync();

            await behavior.ActivateRowAsync(behavior.Rows[0]);

            Assert.Equal("Saved to Documents/GnollHack/dump (2).bin", behavior.StatusMessage);
            Assert.Equal(existing, File.ReadAllBytes(first));
            Assert.Equal("source bytes", File.ReadAllText(source));
        }

        [Fact]
        public async Task Save_ProtectedPlaceIsRefusedAndStays()
        {
            string source;
            string target;
            GHFileSaveBehavior behavior = CreateSave(out source, out target, new[] { Path.Combine(_dir, "Documents") });
            await behavior.LoadAsync();

            await behavior.ActivateRowAsync(behavior.Rows[0]);

            Assert.Equal("GnollHack does not save copies there.", behavior.StatusMessage);
            Assert.False(Directory.Exists(target));

            behavior.AcknowledgeStatus();

            Assert.Null(behavior.StatusMessage);
            Assert.False(behavior.Result.IsCompleted);
            Assert.Single(behavior.Rows);
        }

        [Fact]
        public async Task Save_FailureIsNotFinal()
        {
            string target = Path.Combine(_dir, "t");
            GHFileSaveBehavior behavior = new GHFileSaveBehavior("Save", Path.Combine(_dir, "missing.bin"), "x.bin",
                new[] { new GHFilePlace("T", target, true) }, null);
            await behavior.LoadAsync();

            await behavior.ActivateRowAsync(behavior.Rows[0]);

            Assert.False(behavior.IsBusy);
            Assert.StartsWith("Could not save the file: ", behavior.StatusMessage);
            Assert.False(behavior.Result.IsCompleted);

            behavior.AcknowledgeStatus();
            Assert.Null(behavior.StatusMessage);
            Assert.False(behavior.Result.IsCompleted);

            behavior.Cancel();
            Assert.Null(await behavior.Result);
        }

        /* Changed is raised with IsBusy set before the copy starts, so these handlers run during the busy state
           with no timing involved. */

        [Fact]
        public async Task Save_ActivateAndCancelDuringCopyAreIgnored()
        {
            string source;
            string target;
            GHFileSaveBehavior behavior = CreateSave(out source, out target);
            await behavior.LoadAsync();
            GHFileRow row = behavior.Rows[0];
            bool sawBusy = false;
            behavior.Changed += () =>
            {
                if (behavior.IsBusy && !sawBusy)
                {
                    sawBusy = true;
                    behavior.Cancel();
                    Assert.False(behavior.Result.IsCompleted);
                    Assert.True(behavior.ActivateRowAsync(row).IsCompleted);
                }
            };

            await behavior.ActivateRowAsync(row);

            Assert.True(sawBusy);
            Assert.False(behavior.Result.IsCompleted);
            Assert.Equal("Saved to Documents/GnollHack/dump.bin", behavior.StatusMessage);
            Assert.Single(Directory.GetFileSystemEntries(target));
        }

        [Fact]
        public async Task Save_ActivateAfterSavedStatusIsIgnored()
        {
            string source;
            string target;
            GHFileSaveBehavior behavior = CreateSave(out source, out target);
            await behavior.LoadAsync();

            await behavior.ActivateRowAsync(behavior.Rows[0]);
            await behavior.ActivateRowAsync(behavior.Rows[0]);

            Assert.Single(Directory.GetFileSystemEntries(target));
        }

        [Fact]
        public async Task Save_HostGoneDuringCopyCompletesAfterCopyWithoutStatus()
        {
            string source;
            string target;
            GHFileSaveBehavior behavior = CreateSave(out source, out target);
            List<string> log = new List<string>();
            behavior.Log = log.Add;
            await behavior.LoadAsync();
            bool sawBusy = false;
            behavior.Changed += () =>
            {
                if (behavior.IsBusy && !sawBusy)
                {
                    sawBusy = true;
                    behavior.HostGone();
                    Assert.False(behavior.Result.IsCompleted);
                }
            };

            await behavior.ActivateRowAsync(behavior.Rows[0]);

            string final = Path.Combine(target, "dump.bin");
            Assert.True(sawBusy);
            Assert.True(behavior.Result.IsCompleted);
            Assert.Equal(new[] { final }, await behavior.Result);
            Assert.Null(behavior.StatusMessage);
            Assert.Contains(log, m => m.Contains(final));
            Assert.Equal("source bytes", File.ReadAllText(final));
        }

        [Fact]
        public async Task Save_HostGoneDuringFailingCopyCompletesWithNull()
        {
            GHFileSaveBehavior behavior = new GHFileSaveBehavior("Save", Path.Combine(_dir, "missing.bin"), "x.bin",
                new[] { new GHFilePlace("T", Path.Combine(_dir, "t"), true) }, null);
            List<string> log = new List<string>();
            behavior.Log = log.Add;
            await behavior.LoadAsync();
            behavior.Changed += () =>
            {
                if (behavior.IsBusy)
                {
                    behavior.HostGone();
                }
            };

            await behavior.ActivateRowAsync(behavior.Rows[0]);

            Assert.True(behavior.Result.IsCompleted);
            Assert.Null(await behavior.Result);
            Assert.Null(behavior.StatusMessage);
            Assert.Single(log);
        }

        [Fact]
        public void Save_HostGoneWhenIdleCompletesNow()
        {
            string source;
            string target;
            GHFileSaveBehavior behavior = CreateSave(out source, out target);

            behavior.HostGone();

            Assert.True(behavior.Result.IsCompleted);
            Assert.Null(behavior.Result.Result);
        }

        [Fact]
        public void Queue_FirstItemStartsSynchronously()
        {
            GHFileDialogQueue queue = new GHFileDialogQueue();
            bool started = false;
            TaskCompletionSource<int> first = new TaskCompletionSource<int>();

            queue.Enqueue(() =>
            {
                started = true;
                return first.Task;
            });

            Assert.True(started);
        }

        [Fact]
        public async Task Queue_SecondItemWaitsForTheFirst()
        {
            GHFileDialogQueue queue = new GHFileDialogQueue();
            bool secondStarted = false;
            TaskCompletionSource<int> first = new TaskCompletionSource<int>();

            Task<int> firstResult = queue.Enqueue(() => first.Task);
            Task<int> secondResult = queue.Enqueue(() =>
            {
                secondStarted = true;
                return Task.FromResult(2);
            });

            await Task.Delay(20);
            Assert.False(secondStarted);

            first.SetResult(1);

            Assert.Equal(1, await firstResult);
            Assert.Equal(2, await secondResult);
            Assert.True(secondStarted);
        }

        [Fact]
        public async Task Queue_RunsThreeItemsInOrder()
        {
            GHFileDialogQueue queue = new GHFileDialogQueue();
            List<int> order = new List<int>();
            TaskCompletionSource<int> first = new TaskCompletionSource<int>();
            TaskCompletionSource<int> second = new TaskCompletionSource<int>();
            TaskCompletionSource<int> third = new TaskCompletionSource<int>();

            Task<int> a = queue.Enqueue(() => { order.Add(1); return first.Task; });
            Task<int> b = queue.Enqueue(() => { order.Add(2); return second.Task; });
            Task<int> c = queue.Enqueue(() => { order.Add(3); return third.Task; });

            third.SetResult(3);
            second.SetResult(2);
            await Task.Delay(20);
            Assert.Equal(new[] { 1 }, order);

            first.SetResult(1);
            await Task.WhenAll(a, b, c);

            Assert.Equal(new[] { 1, 2, 3 }, order);
        }

        [Fact]
        public async Task Queue_FailureReachesOnlyItsOwnCaller()
        {
            GHFileDialogQueue queue = new GHFileDialogQueue();
            TaskCompletionSource<int> first = new TaskCompletionSource<int>();

            Task<int> failing = queue.Enqueue(() => first.Task);
            Task<int> next = queue.Enqueue(() => Task.FromResult(2));

            first.SetException(new InvalidOperationException("failed"));

            await Assert.ThrowsAsync<InvalidOperationException>(() => failing);
            Assert.Equal(2, await next);
        }

        [Fact]
        public async Task Queue_SynchronousThrowIsContained()
        {
            GHFileDialogQueue queue = new GHFileDialogQueue();

            Task<int> failing = queue.Enqueue<int>(() => { throw new InvalidOperationException("failed"); });
            Task<int> next = queue.Enqueue(() => Task.FromResult(2));

            await Assert.ThrowsAsync<InvalidOperationException>(() => failing);
            Assert.Equal(2, await next);
        }

        [Fact]
        public async Task Queue_EnqueueDuringSynchronousPartQueuesAfterThatItem()
        {
            GHFileDialogQueue queue = new GHFileDialogQueue();
            List<int> order = new List<int>();
            TaskCompletionSource<int> first = new TaskCompletionSource<int>();
            Task<int> inner = null;

            Task<int> outer = queue.Enqueue(() =>
            {
                order.Add(1);
                inner = queue.Enqueue(() =>
                {
                    order.Add(2);
                    return Task.FromResult(2);
                });
                return first.Task;
            });

            await Task.Delay(20);
            Assert.Equal(new[] { 1 }, order);

            first.SetResult(1);
            await outer;
            await inner;

            Assert.Equal(new[] { 1, 2 }, order);
        }

        [Fact]
        public async Task Queue_EnqueueAfterAnAwaitQueuesAfterThatItem()
        {
            GHFileDialogQueue queue = new GHFileDialogQueue();
            List<int> order = new List<int>();
            TaskCompletionSource<int> release = new TaskCompletionSource<int>();
            TaskCompletionSource<bool> innerQueued = new TaskCompletionSource<bool>();
            Task<int> inner = null;

            Task<int> outer = queue.Enqueue(async () =>
            {
                order.Add(1);
                await Task.Yield();
                inner = queue.Enqueue(() =>
                {
                    order.Add(2);
                    return Task.FromResult(2);
                });
                innerQueued.SetResult(true);
                return await release.Task;
            });

            await innerQueued.Task;
            await Task.Delay(20);
            Assert.Equal(new[] { 1 }, order);

            release.SetResult(1);
            await outer;
            await inner;

            Assert.Equal(new[] { 1, 2 }, order);
        }
    }
}
