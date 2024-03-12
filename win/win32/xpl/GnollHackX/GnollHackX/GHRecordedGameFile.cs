using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackX
{
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
        DateTime _lastwriteTime;
        List<string> _continuationFiles = new List<string>();

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
            _lastwriteTime = lastWriteTime;
        }

        private string GetSizeString()
        {
            return _fileSize < 1024 * 1024 ? string.Format("{0} kB", _fileSize / 1024) : string.Format("{0:0.0} MB", (double)_fileSize / (1024 * 1024));
        }

        public override string ToString()
        {
            return _isFolder ? _fileName : string.Format("{0}. {1} ({2}, {3:d/M/yyyy HH:mm:ss}, {4} file{5})", _index, _fileName, GetSizeString(), _lastwriteTime, NumberOfFiles, NumberOfFiles == 1 ? "" : "s");
        }

        public string FilePath { get { return _filePath; } }
        public string FileName { get { return _fileName; } }
        public string Extension { get { return _extension; } }
        public long FileSize { get { return _fileSize; } }
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
            get { return _isFolder ? (string.IsNullOrWhiteSpace(_filePath) ? "" : "Folder") : string.Format("{0}, {1:d/M/yyyy HH:mm:ss}, {2} file{3}", GetSizeString(), _lastwriteTime, NumberOfFiles, NumberOfFiles == 1 ? "" : "s"); }
        }

        public bool IsZip { get { return _extension == "zip"; } }
        public bool IsFolder { get { return _isFolder; } }

        public double FormattedNameFontSize { get { return _isFolder ? 16.0 : 15.0; } }
        public int NumberOfFiles { get { return _numberOfFiles + _continuationFiles.Count; } }

        public List<string> ContinuationFiles { get { return _continuationFiles; } }
    }
}
