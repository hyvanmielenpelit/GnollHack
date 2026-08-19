using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Text;
#if GNH_MAUI
using GnollHackX;
namespace GnollHackM
#else
namespace GnollHackX
#endif
{
    public struct ContinuationFile
    {
        public string FileName;
        public long FileSize;

        public ContinuationFile(string fileName, long fileSize)
        {
            FileName = fileName;
            FileSize = fileSize;
        }
    }

    public sealed class GHRecordedGameFile : INotifyPropertyChanged
    {
        int _index;
        string _filePath;
        string _fileName;
        string _extension;
        bool _isFolder;
        long _fileSize;
        int _numberOfFiles;
        DateTime _creationTime;
        DateTime _lastWriteTime;
        List<ContinuationFile> _continuationFiles = new List<ContinuationFile>();
        bool _uploaded;
        bool _downloaded;

        /* Parsed filename parts for columnar display */
        string _characterName = "";
        string _turnString = "";
        string _versionString = "";
        string _formatString = "";

        public event PropertyChangedEventHandler PropertyChanged;

        private void NotifyPropertyChanged([CallerMemberName] string propertyName = "")
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        public GHRecordedGameFile(int index, string filePath, string fileName, string extension, bool isFolder, long fileSize, int numberOfFiles, DateTime creationTime, DateTime lastWriteTime)
        {
            _index = index;
            _filePath = filePath != null ? filePath : "";
            _fileName = fileName != null ? fileName : "";
            _extension = extension != null ? extension : "";
            _isFolder = isFolder;
            _fileSize = fileSize;
            _numberOfFiles = numberOfFiles;
            _creationTime = creationTime;
            _lastWriteTime = lastWriteTime;
            ParseFileNameParts();
        }

        private long CalculateTotalFileSize()
        {
            long totalSize = _fileSize;
            foreach(ContinuationFile continuationFile in _continuationFiles)
            {
                totalSize += continuationFile.FileSize;
            }
            return totalSize;
        }

        private string GetSizeString()
        {
            long totalSize = CalculateTotalFileSize();
            return totalSize < 1024 * 1024 ? string.Format("{0} kB", totalSize / 1024) : string.Format("{0:0.0} MB", (double)totalSize / (1024 * 1024));
        }

        /// <summary>
        /// Parses the replay filename to extract character name, starting turn,
        /// version string, and format. The filename format is:
        /// replay-[CharName-][T&lt;Turn&gt;-]&lt;MMP&gt;-&lt;Build&gt;-&lt;Timestamp&gt;.gnhrec[.gz|.zip]
        /// </summary>
        private void ParseFileNameParts()
        {
            if (_isFolder || string.IsNullOrWhiteSpace(_fileName))
                return;

            /* Determine format from the file extension(s) */
            string nameForParsing = _fileName;
            if (nameForParsing.EndsWith(GHConstants.ReplayGZipFileNameSuffix))
            {
                _formatString = GHConstants.ReplayGZipFileNameSuffix;
                nameForParsing = nameForParsing.Substring(0, nameForParsing.Length - GHConstants.ReplayGZipFileNameSuffix.Length);
            }
            else if (nameForParsing.EndsWith(GHConstants.ReplayZipFileNameSuffix))
            {
                _formatString = GHConstants.ReplayZipFileNameSuffix;
                nameForParsing = nameForParsing.Substring(0, nameForParsing.Length - GHConstants.ReplayZipFileNameSuffix.Length);
            }

            /* Strip the .gnhrec suffix */
            if (nameForParsing.EndsWith(GHConstants.ReplayFileNameSuffix))
                nameForParsing = nameForParsing.Substring(0, nameForParsing.Length - GHConstants.ReplayFileNameSuffix.Length);

            /* Strip the replay- prefix */
            if (!nameForParsing.StartsWith(GHConstants.ReplayFileNamePrefix))
                return;

            string middle = nameForParsing.Substring(GHConstants.ReplayFileNamePrefix.Length);
            if (string.IsNullOrEmpty(middle))
                return;

            /*
             * The middle part has the form: [CharName-][T<Turn>-]<MMP>-<Build>-<Timestamp>
             * Strategy: scan for the pattern -T<digit> which marks the turn boundary.
             * If not found, the character name is absent and we parse version directly.
             */
            int turnMarkerIdx = -1;
            for (int i = 0; i < middle.Length - 2; i++)
            {
                if (middle[i] == '-' && middle[i + 1] == 'T' && i + 2 < middle.Length && middle[i + 2] >= '0' && middle[i + 2] <= '9')
                {
                    turnMarkerIdx = i;
                    break;
                }
            }

            string afterCharName;
            if (turnMarkerIdx > 0)
            {
                /* Character name is everything before the turn marker */
                _characterName = middle.Substring(0, turnMarkerIdx);
                afterCharName = middle.Substring(turnMarkerIdx + 1); /* Skip the leading '-' */
            }
            else if (turnMarkerIdx == 0)
            {
                /* No character name, starts with -T... (the '-' is at position 0) */
                _characterName = "";
                afterCharName = middle.Substring(1); /* Skip the leading '-' */
            }
            else
            {
                /* No turn marker found; character name might still be absent */
                _characterName = "";
                afterCharName = middle;
            }

            /* Parse turn: T<number>-... */
            if (afterCharName.StartsWith("T"))
            {
                int turnEnd = afterCharName.IndexOf('-', 1);
                if (turnEnd > 1)
                {
                    _turnString = afterCharName.Substring(1, turnEnd - 1);
                    afterCharName = afterCharName.Substring(turnEnd + 1);
                }
            }

            /* Parse version: <MMP>-<Build>-<Timestamp>
             * MMP is 3+ concatenated digits, Build is digits, Timestamp is a large number.
             * We split by '-' and try to reconstruct the version from the first two parts. */
            string[] parts = afterCharName.Split('-');
            if (parts.Length >= 3)
            {
                /* parts[0] = MMP (e.g., "430"), parts[1] = Build (e.g., "6"), parts[2..] = timestamp */
                string mmpStr = parts[0];
                string buildStr = parts[1];

                if (mmpStr.Length >= 3)
                {
                    /* Parse MMP digits: Major=first digit(s), Minor=second-to-last, Patch=last
                     * For a 3-digit string "430": M=4, M2=3, P=0
                     * For longer strings, major may be multi-digit */
                    string major = mmpStr.Substring(0, mmpStr.Length - 2);
                    string minor = mmpStr.Substring(mmpStr.Length - 2, 1);
                    string patch = mmpStr.Substring(mmpStr.Length - 1, 1);

                    int buildNum;
                    if (int.TryParse(buildStr, out buildNum) && buildNum > 0)
                        _versionString = major + "." + minor + "." + patch + " Build " + buildStr;
                    else
                        _versionString = major + "." + minor + "." + patch;
                }
            }
            else if (parts.Length >= 1 && parts[0].Length > 10)
            {
                /* Legacy format: version was stored as a raw 64-bit integer */
                ulong legacyVer;
                if (ulong.TryParse(parts[0], out legacyVer) && legacyVer > 0)
                {
                    ulong majorver = (legacyVer >> 24) & 0xFFUL;
                    ulong minorver = (legacyVer >> 16) & 0xFFUL;
                    ulong patchver = (legacyVer >> 8) & 0xFFUL;
                    ulong editver = legacyVer & 0xFFUL;
                    _versionString = majorver + "." + minorver + "." + patchver + (editver > 0 ? " Build " + editver : "");
                }
            }
        }

        public override string ToString()
        {
            return _isFolder ? _fileName : string.Format("{0}. {1} ({2}, {3:d/M/yyyy HH:mm:ss}, {4} file{5})", _index, _fileName, GetSizeString(), _lastWriteTime, NumberOfFiles, NumberOfFiles == 1 ? "" : "s");
        }


        public string FilePath { get { return _filePath; } }
        public string FileName { get { return _fileName; } }
        public string Extension { get { return _extension; } }
        public long FileSize { get { return CalculateTotalFileSize(); } }
        public DateTime CreationTime { get { return _creationTime; } }
        public DateTime LastWriteTime { get { return _lastWriteTime; } }

        public string FormattedIndex
        {
            get { return _isFolder ? (string.IsNullOrWhiteSpace(_filePath) ? "\u21A9" : "") : string.Format("{0}.", _index); }
        }
        public string FormattedName
        {
            get { return _fileName; }
        }
        public string FormattedInformation
        {
            get { return _isFolder ? (string.IsNullOrWhiteSpace(_filePath) ? "" : "Folder") : string.Format("{0}, {1:d/M/yyyy HH:mm:ss}, {2} file{3}", GetSizeString(), _lastWriteTime, NumberOfFiles, NumberOfFiles == 1 ? "" : "s"); }
        }

        /* Parsed column properties for the table layout */
        public string CharacterName { get { return _isFolder ? (string.IsNullOrWhiteSpace(_filePath) ? "(Back)" : _fileName) : _characterName; } }
        public string TurnString { get { return _isFolder ? "" : _turnString; } }
        public string VersionString { get { return _isFolder ? "" : _versionString; } }
        public string FormatString { get { return _isFolder ? "" : _formatString; } }
        public string FormattedDate { get { return _isFolder ? "" : _lastWriteTime.ToString("d MMM yyyy, H:mm:ss"); } }
        public string FormattedSize { get { return _isFolder ? "" : GetSizeString(); } }
        public string FormattedFiles { get { return _isFolder ? "" : NumberOfFiles.ToString(); } }
        public bool IsBackEntry { get { return _isFolder && string.IsNullOrWhiteSpace(_filePath); } }

        public bool IsZip { get { return _extension == "zip"; } }
        public bool IsFolder { get { return _isFolder; } }

        public double FormattedNameFontSize { get { return _isFolder ? 17.0 : 15.0; } }
        public double FormattedInfoFontSize { get { return _isFolder && string.IsNullOrWhiteSpace(_filePath) ? 8.0 : 15.0; } }
        public bool FormattedInfoVisible { get { return !(_isFolder && string.IsNullOrWhiteSpace(_filePath)); } }

        public int NumberOfFiles { get { return _numberOfFiles + _continuationFiles.Count; } }

        public List<ContinuationFile> ContinuationFiles { get { return _continuationFiles; } }

        public bool Uploaded 
        { 
            get { return _uploaded; } 
            set { _uploaded = value; NotifyPropertyChanged(); NotifyPropertyChanged(nameof(TextColor)); } 
        }
        public bool Downloaded 
        { 
            get { return _downloaded; } 
            set { _downloaded = value; NotifyPropertyChanged(); NotifyPropertyChanged(nameof(TextColor)); } 
        }
        public
#if GNH_MAUI
            Microsoft.Maui.Graphics.Color
#else
            Xamarin.Forms.Color 
#endif            
            TextColor { get { return _downloaded && _uploaded ? GHColors.Magenta : _downloaded ? GHColors.Blue : _uploaded ? GHColors.Green : GHApp.DarkMode ? GHColors.White : GHColors.Black; } }

        public int Index { get { return _index; } set { _index = value; } }
    }
}
