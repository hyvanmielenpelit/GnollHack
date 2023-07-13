using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackCommon
{
    public class AssetPackStatusEventArgs : EventArgs
    {
        public readonly int Status;
        public readonly long DownloadedBytes;
        public readonly long TotalBytes;
        public double PercentDownloaded
        {
            get { return TotalBytes > 0 ? (double)DownloadedBytes / TotalBytes : 0; }
        }
        public readonly string CompletedAssetPackPath = null;

        public AssetPackStatusEventArgs()
        {

        }
        public AssetPackStatusEventArgs(int status)
        {
            Status = status;
        }
        public AssetPackStatusEventArgs(int status, long downloaded, long totalsize)
        {
            Status = status;
            DownloadedBytes = downloaded;
            TotalBytes = totalsize;
        }
        public AssetPackStatusEventArgs(int status, string completedpath)
        {
            Status = status;
            CompletedAssetPackPath = completedpath;
        }
    }

    public class GHAssetPackStatus
    {
        public const int Canceled = 6;
        public const int Completed = 4;
        public const int Downloading = 2;
        public const int Failed = 5;
        public const int NotInstalled = 8;
        public const int Pending = 1;
        public const int Transferring = 3;
        public const int Unknown = 0;
        public const int WaitingForWifi = 7;
    };
}
