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

        // Parsed metadata fields
        public string PlayerName { get; set; }
        public int? FirstTurn { get; set; }
        public string VersionString { get; set; }
        public DateTime? RecordingTimestamp { get; set; }

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

        public override string ToString()
        {
            return _isFolder ? _fileName : string.Format("{0}. {1} ({2}, {3:d/M/yyyy HH:mm:ss}, {4} file{5})", _index, FormattedName, GetSizeString(), _lastWriteTime, NumberOfFiles, NumberOfFiles == 1 ? "" : "s");
        }

        public string FilePath { get { return _filePath; } }
        public string FileName { get { return _fileName; } }
        public string Extension { get { return _extension; } }
        public long FileSize { get { return CalculateTotalFileSize(); } }
        public DateTime CreationTime { get { return _creationTime; } }
        public DateTime LastWriteTime { get { return _lastWriteTime; } }

        public int Index
        {
            get { return _index; }
            set
            {
                if (_index != value)
                {
                    _index = value;
                    NotifyPropertyChanged(nameof(FormattedIndex));
                }
            }
        }

        public string FormattedIndex
        {
            get { return _isFolder ? "" : string.Format("{0}.", _index); }
        }
        
        public string FormattedName
        {
            get 
            {
                if (_isFolder) return _fileName;
                if (!string.IsNullOrEmpty(PlayerName))
                {
                    if (FirstTurn.HasValue)
                        return string.Format("{0} (Turn {1})", PlayerName, FirstTurn.Value);
                    return PlayerName;
                }
                else if (VersionString != null)
                {
                    // For crash replays or old format missing player name
                    return "(Unknown Player)";
                }
                return _fileName; // Fallback
            }
        }
        
        public string FormattedInformation
        {
            get 
            {
                if (_isFolder) 
                    return string.IsNullOrWhiteSpace(_filePath) ? "" : "Folder";
                
                string versionInfo = string.IsNullOrEmpty(VersionString) ? "" : string.Format("{0} \u00B7 ", VersionString);
                string timestamp = RecordingTimestamp.HasValue ? string.Format("{0:d/M/yyyy HH:mm:ss}", RecordingTimestamp.Value) : string.Format("{0:d/M/yyyy HH:mm:ss}", _lastWriteTime);
                
                return string.Format("{0}{1} \u00B7 {2} file{3} \u00B7 {4}", 
                    versionInfo, GetSizeString(), NumberOfFiles, NumberOfFiles == 1 ? "" : "s", timestamp);
            }
        }

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
    }
}
