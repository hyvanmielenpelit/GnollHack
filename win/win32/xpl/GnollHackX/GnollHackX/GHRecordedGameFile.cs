using System;
using System.Collections.Generic;
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

    public sealed class GHRecordedGameFile
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
            return _isFolder ? _fileName : string.Format("{0}. {1} ({2}, {3:d/M/yyyy HH:mm:ss}, {4} file{5})", _index, _fileName, GetSizeString(), _lastWriteTime, NumberOfFiles, NumberOfFiles == 1 ? "" : "s");
        }

        public string FilePath => _filePath;
        public string FileName => _fileName;
        public string Extension => _extension;
        public long FileSize => CalculateTotalFileSize();
        public DateTime CreationTime => _creationTime;
        public DateTime LastWriteTime => _lastWriteTime;

        public string FormattedIndex => _isFolder ? "" : string.Format("{0}.", _index);
        public string FormattedName => _fileName;
        public string FormattedInformation => _isFolder ? (string.IsNullOrWhiteSpace(_filePath) ? "" : "Folder") : string.Format("{0}, {1:d/M/yyyy HH:mm:ss}, {2} file{3}", GetSizeString(), _lastWriteTime, NumberOfFiles, NumberOfFiles == 1 ? "" : "s");

        public bool IsZip => _extension == "zip";
        public bool IsFolder => _isFolder;

        public double FormattedNameFontSize => _isFolder ? 17.0 : 15.0;
        public double FormattedInfoFontSize => _isFolder && string.IsNullOrWhiteSpace(_filePath) ? 8.0 : 15.0;
        public bool FormattedInfoVisible => !(_isFolder && string.IsNullOrWhiteSpace(_filePath));

        public int NumberOfFiles => _numberOfFiles + _continuationFiles.Count;

        public List<ContinuationFile> ContinuationFiles => _continuationFiles;

        public bool Uploaded { get => _uploaded; set => _uploaded = value; }
        public bool Downloaded { get => _downloaded; set => _downloaded = value; }
        public
#if GNH_MAUI
            Microsoft.Maui.Graphics.Color
#else
            Xamarin.Forms.Color 
#endif            
            TextColor => _downloaded && _uploaded ? GHColors.Magenta : _downloaded ? GHColors.Blue : _uploaded ? GHColors.Green : GHApp.DarkMode ? GHColors.White : GHColors.Black;

        public int Index { get => _index; set => _index = value; }
    }
}
