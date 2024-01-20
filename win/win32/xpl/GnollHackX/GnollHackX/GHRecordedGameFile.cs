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
        DateTime _creationTime;
        DateTime _lastwriteTime;

        public GHRecordedGameFile(int index, string filePath, string fileName, string extension, long fileSize, DateTime creationTime, DateTime lastWriteTime) 
        {
            _index = index;
            _filePath = filePath != null ? filePath : "";
            _fileName = fileName != null ? fileName : "";
            _extension = extension != null ? extension : "";
            _fileSize = fileSize;
            _creationTime = creationTime;
            _lastwriteTime = lastWriteTime;
        }
        public override string ToString()
        {
            return string.Format("{0}. {1} ({2} kB, {3:d/M/yyyy HH:mm:ss})", _index, _fileName, _fileSize / 1024, _lastwriteTime);
        }

        public string FilePath {  get { return _filePath; } }
        public string FileName { get { return _fileName; } }
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
            get { return string.Format("{0} kB, {1:d/M/yyyy HH:mm:ss}",  _fileSize / 1024, _lastwriteTime); }
        }

        public bool IsZip {  get { return _extension == "zip"; } } 
    }
}
