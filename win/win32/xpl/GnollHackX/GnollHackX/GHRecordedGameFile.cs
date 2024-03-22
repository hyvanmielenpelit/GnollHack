using System;
using System.Collections.Generic;
using System.Drawing;
using System.Text;

namespace GnollHackX
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

    public class GHRecordedGameFile
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

        private string GetSizeString()
        {
            return _fileSize < 1024 * 1024 ? string.Format("{0} kB", _fileSize / 1024) : string.Format("{0:0.0} MB", (double)_fileSize / (1024 * 1024));
        }

        public override string ToString()
        {
            return _isFolder ? _fileName : string.Format("{0}. {1} ({2}, {3:d/M/yyyy HH:mm:ss}, {4} file{5})", _index, _fileName, GetSizeString(), _lastWriteTime, NumberOfFiles, NumberOfFiles == 1 ? "" : "s");
        }


        public string FilePath { get { return _filePath; } }
        public string FileName { get { return _fileName; } }
        public string Extension { get { return _extension; } }
        public long FileSize { get { return _fileSize; } }
        public DateTime CreationTime { get { return _creationTime; } }
        public DateTime LastWriteTime { get { return _lastWriteTime; } }

        public string FormattedIndex
        {
            get { return _isFolder ? "" : string.Format("{0}.", _index); }
        }
        public string FormattedName
        {
            get { return _fileName; }
        }
        public string FormattedInformation
        {
            get { return _isFolder ? (string.IsNullOrWhiteSpace(_filePath) ? "" : "Folder") : string.Format("{0}, {1:d/M/yyyy HH:mm:ss}, {2} file{3}", GetSizeString(), _lastWriteTime, NumberOfFiles, NumberOfFiles == 1 ? "" : "s"); }
        }

        public bool IsZip { get { return _extension == "zip"; } }
        public bool IsFolder { get { return _isFolder; } }

        public double FormattedNameFontSize { get { return _isFolder ? 17.0 : 15.0; } }
        public double FormattedInfoFontSize { get { return _isFolder && string.IsNullOrWhiteSpace(_filePath) ? 8.0 : 15.0; } }
        public bool FormattedInfoVisible { get { return !(_isFolder && string.IsNullOrWhiteSpace(_filePath)); } }

        public int NumberOfFiles { get { return _numberOfFiles + _continuationFiles.Count; } }

        public List<ContinuationFile> ContinuationFiles { get { return _continuationFiles; } }

        public bool Uploaded { get { return _uploaded; } set { _uploaded = value; } }
        public bool Downloaded { get { return _downloaded; } set { _downloaded = value; } }
        public
#if GNH_MAUI
            Microsoft.Maui.Graphics.Color
#else
            Color 
#endif            
            TextColor { get { return _downloaded && _uploaded ? GHColors.Magenta : _downloaded ? GHColors.Blue : _uploaded ? GHColors.Green : GHColors.Black; } }

        public int Index { get { return _index; } set { _index = value; } }
    }
}
