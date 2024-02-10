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
        long _fileSize;
        int _numberOfFiles;
        DateTime _creationTime;
        DateTime _lastwriteTime;

        public GHRecordedGameFile(int index, string filePath, string fileName, string extension, long fileSize, int numberOfFiles, DateTime creationTime, DateTime lastWriteTime) 
        {
            _index = index;
            _filePath = filePath != null ? filePath : "";
            _fileName = fileName != null ? fileName : "";
            _extension = extension != null ? extension : "";
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
            return string.Format("{0}. {1} ({2}, {3:d/M/yyyy HH:mm:ss}, {4} file{5})", _index, _fileName, GetSizeString(), _lastwriteTime, _numberOfFiles, _numberOfFiles == 1 ? "" : "s");
        }

        public string FilePath {  get { return _filePath; } }
        public string FileName { get { return _fileName; } }
        public long FileSize { get { return _fileSize; } }
        public string FormattedIndex
        {
            get { return string.Format("{0}.", _index); }
        }
        public string FormattedName
        {
            get { return _fileName; }
        }
        public string FormattedInformation
        {
            get { return string.Format("{0}, {1:d/M/yyyy HH:mm:ss}, {2} file{3}", GetSizeString(), _lastwriteTime, _numberOfFiles, _numberOfFiles == 1 ? "" : "s"); }
        }

        public bool IsZip {  get { return _extension == "zip"; } } 
    }
}
