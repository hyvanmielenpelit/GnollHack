using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Data.SqlTypes;
using System.Diagnostics;
using System.IO;
using System.IO.Compression;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices.ComTypes;
using System.Runtime.Serialization.Json;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Azure;
using Azure.Identity;
using Azure.Storage.Blobs;
using Azure.Storage.Blobs.Models;
using Azure.Storage.Blobs.Specialized;

#if GNH_MAUI
using GnollHackX;
using Microsoft.Maui.Controls.PlatformConfiguration;
using Microsoft.Maui.Controls.PlatformConfiguration.iOSSpecific;

namespace GnollHackM
#else
using GnollHackX.Pages.Game;
using Xamarin.Essentials;
using Xamarin.Forms;
using Xamarin.Forms.PlatformConfiguration.iOSSpecific;
using Xamarin.Forms.Xaml;

namespace GnollHackX.Pages.MainScreen
#endif
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class ReplayPage : ContentPage
    {
        MainPage _mainPage = null;
        string _subDirectoryLocal = null;
        string _subDirectoryDownload = null;
        string _subDirectoryServer = null;

        public bool IsMultiSelect { get { return ReplayCollectionView.SelectionMode == SelectionMode.Multiple; } }
        public bool IsLocal { get { return FolderPicker.SelectedIndex == 0; } }
        public bool IsCloud { get { return FolderPicker.SelectedIndex == 1; } }
        public bool IsDownload { get { return FolderPicker.SelectedIndex == 2; } }

        public ReplayPage(MainPage mainPage)
		{
			InitializeComponent();
#if GNH_MAUI
            On<iOS>().SetUseSafeArea(true);
#else
            On<Xamarin.Forms.PlatformConfiguration.iOS>().SetUseSafeArea(true);
#endif

            _mainPage = mainPage;
            MoreButton.IsEnabled = false;
            SelectButton.IsEnabled = false;
            FolderPicker.SelectedIndex = 0;

            UpdateRecordings();
        }

        private void UpdateRecordings()
        {
            UpdateLocalOrServerRecordings(IsCloud);
        }

        private void UpdateLocalOrServerRecordings(bool isServer)
        {
            if (isServer)
            {
                UpdateServerRecordings();
            }
            else
            {
                UpdateLocalRecordings();
            }
        }


        private void UpdateLocalRecordings()
        {
            ReplayCollectionView.SelectedItem = null;
            ReplayCollectionView.SelectedItems.Clear();
            ReplayCollectionView.ItemsSource = null;
            MoreButton.IsEnabled = false;
            SelectButton.IsEnabled = false;
            MoreButton.TextColor = GHColors.Gray;
            SelectButton.TextColor = GHColors.Gray;

            string dirPath = Path.Combine(GHApp.GHPath, IsDownload ? GHConstants.ReplayDownloadFromCloudDirectory : GHConstants.ReplayDirectory);
            if(IsDownload && !string.IsNullOrWhiteSpace(_subDirectoryDownload))
            {
                dirPath = Path.Combine(dirPath, _subDirectoryDownload);
            }
            else if (IsLocal && !string.IsNullOrWhiteSpace(_subDirectoryLocal))
            {
                dirPath = Path.Combine(dirPath, _subDirectoryLocal);
            }
            int i = 0;
            long totalBytes = 0L;
            ObservableCollection<GHRecordedGameFile> gHRecordedGameFiles = new ObservableCollection<GHRecordedGameFile>();
            if (Directory.Exists(dirPath))
            {
                if (!string.IsNullOrWhiteSpace(IsDownload ? _subDirectoryDownload : _subDirectoryLocal))
                {
                    gHRecordedGameFiles.Add(new GHRecordedGameFile(0, null, "(Back)", "", true, 0, 1, DateTime.Now, DateTime.Now));
                }

                string[] directories = Directory.GetDirectories(dirPath);
                if(directories != null && directories.Length > 0)
                {
                    Array.Sort(directories);
                    foreach(string dir in directories)
                    {
                        DirectoryInfo di = new DirectoryInfo(dir);
                        gHRecordedGameFiles.Add(new GHRecordedGameFile(i, dir, di.Name, di.Extension, true, 0, 1, di.CreationTime, di.LastWriteTime));
                    }
                }

                string[] files = Directory.GetFiles(dirPath);
                if (files != null && files.Length > 0)
                {
                    Array.Sort(files, new FileDateComparer());
                    foreach (string file in files)
                    {
                        if (file != null && File.Exists(file))
                        {
                            FileInfo fi = new FileInfo(file);
                            if (fi != null && !string.IsNullOrWhiteSpace(fi.Name))
                            {
                                bool isGZip = fi.Name.Length > GHConstants.ReplayGZipFileNameSuffix.Length && fi.Name.EndsWith(GHConstants.ReplayGZipFileNameSuffix);
                                bool isNormalZip = fi.Name.Length > GHConstants.ReplayZipFileNameSuffix.Length && fi.Name.EndsWith(GHConstants.ReplayZipFileNameSuffix);
                                bool isZip = isGZip || isNormalZip;
                                if (fi.Name.StartsWith(GHConstants.ReplayFileNamePrefix))
                                {
                                    long contLen = 0L;
                                    int noOfContFiles = 0;
                                    if (!isZip)
                                    {
                                        string zipFile = file + (GHApp.UseGZipForReplays ? GHConstants.ReplayGZipFileNameSuffix : GHConstants.ReplayZipFileNameSuffix);
                                        if (File.Exists(zipFile)) /* Ensure that a corresponding zip exists before deletion; then we know the unzipped file is a result of a crash during replay */
                                        {
                                            try
                                            {
                                                GHApp.DeleteReplay(file);
                                            }
                                            catch (Exception ex)
                                            {
                                                Debug.WriteLine(ex);
                                            }
                                        }
                                        else
                                        {
                                            /* Zip it */
                                            if (GHApp.UseGZipForReplays)
                                            {
                                                using (FileStream originalFileStream = File.Open(file, FileMode.Open))
                                                {
                                                    using (FileStream compressedFileStream = File.Create(zipFile))
                                                    {
                                                        using (var compressor = new GZipStream(compressedFileStream, CompressionMode.Compress))
                                                        {
                                                            originalFileStream.CopyTo(compressor);
                                                        }
                                                    }
                                                }
                                            }
                                            else
                                            {
                                                using (ZipArchive archive = ZipFile.Open(zipFile, ZipArchiveMode.Create))
                                                {
                                                    archive.CreateEntryFromFile(file, Path.GetFileName(file));
                                                }
                                            }

                                            if (File.Exists(file) && File.Exists(zipFile))
                                                File.Delete(file);

                                            if(File.Exists(zipFile))
                                            {
                                                i++;
                                                FileInfo zipFI = new FileInfo(zipFile);
                                                gHRecordedGameFiles.Add(new GHRecordedGameFile(i, Path.Combine(dirPath, zipFile), zipFI.Name, zipFI.Extension, false, zipFI.Length + 0, 1, zipFI.CreationTime, zipFI.LastWriteTime));
                                                totalBytes += zipFI.Length + 0;
                                            }
                                        }
                                    }
                                    else
                                    {
                                        i++;
                                        string usedZipSuffix = isGZip ? GHConstants.ReplayGZipFileNameSuffix : GHConstants.ReplayZipFileNameSuffix;
                                        int middleLen = fi.Name.Length - GHConstants.ReplayFileNamePrefix.Length - GHConstants.ReplayFileNameSuffix.Length - (isZip ? usedZipSuffix.Length : 0);
                                        if (middleLen > 0)
                                        {
                                            string middleStr = fi.Name.Substring(GHConstants.ReplayFileNamePrefix.Length, middleLen);
                                            foreach (string contFile in files)
                                            {
                                                if (contFile != null && File.Exists(contFile))
                                                {
                                                    FileInfo contFI = new FileInfo(contFile);
                                                    if (!string.IsNullOrWhiteSpace(contFI.Name) && contFI.Name.StartsWith(GHConstants.ReplayContinuationFileNamePrefix + middleStr))
                                                    {
                                                        if (contFile.EndsWith(usedZipSuffix))
                                                        {
                                                            noOfContFiles++;
                                                            contLen += contFI.Length;
                                                        }
                                                        else
                                                        {
                                                            if (File.Exists(contFile + usedZipSuffix)) /* Ensure that a corresponding zip exists before deletion; then we know the unzipped file is a result of a crash during replay */
                                                            {
                                                                try
                                                                {
                                                                    File.Delete(contFile);
                                                                }
                                                                catch (Exception ex)
                                                                {
                                                                    Debug.WriteLine(ex);
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                        gHRecordedGameFiles.Add(new GHRecordedGameFile(i, Path.Combine(dirPath, file), fi.Name, fi.Extension, false, fi.Length + contLen, 1 + noOfContFiles, fi.CreationTime, fi.LastWriteTime));
                                        totalBytes += fi.Length + contLen;
                                    }
                                }
                                else if (fi.Name.StartsWith(GHConstants.ReplayContinuationFileNamePrefix))
                                {
                                    if(!isZip)
                                    {
                                        /* Check that is not dangling unzipped file from crash during game being recorded */
                                        int extraRemoved = 0;
                                        for(int j = fi.Name.Length - GHConstants.ReplayFileNameSuffix.Length - 1; j >= 0; j--)
                                        {
                                            char c = fi.Name[j];
                                            if (c >= '0' && c <= '9')
                                            {
                                                if(j == 0)
                                                {
                                                    extraRemoved = 0; /* Error */
                                                    break;
                                                }
                                                else
                                                    extraRemoved++;
                                            }
                                            else if (c == GHConstants.ReplayFileContinuationNumberDivisor[0])
                                            {
                                                extraRemoved++;
                                                break;
                                            }
                                            else
                                            {
                                                extraRemoved = 0; /* Error */
                                                break;
                                            }
                                        }
                                        int noOfDigits = 0;
                                        for (int j = fi.Name.Length - extraRemoved - GHConstants.ReplayFileNameSuffix.Length - 1; j >= 0; j--)
                                        {
                                            char c = fi.Name[j];
                                            if (c >= '0' && c <= '9')
                                            {
                                                if (j == 0)
                                                {
                                                    noOfDigits = 0; /* Error */
                                                    break;
                                                }
                                                else
                                                    noOfDigits++;
                                            }
                                            else if (c == GHConstants.ReplayFileNameMiddleDivisor[0])
                                            {
                                                /* Done successfully */
                                                break;
                                            }
                                            else
                                            {
                                                noOfDigits = 0; /* Error */
                                                break;
                                            }
                                        }
                                        bool mainFileFound = false;
                                        string mainFileName = null;
                                        int middleStart = fi.Name.Length - noOfDigits - extraRemoved - GHConstants.ReplayFileNameSuffix.Length;
                                        int middleLen = noOfDigits;
                                        if (middleStart > 0 && middleLen > 0)
                                        {
                                            string middleStr = fi.Name.Substring(middleStart, middleLen);
                                            string searchString = GHConstants.ReplayFileNameMiddleDivisor + middleStr + GHConstants.ReplayFileNameSuffix + (GHApp.UseGZipForReplays ? GHConstants.ReplayGZipFileNameSuffix : GHConstants.ReplayZipFileNameSuffix);
                                            foreach (string mainFile in files)
                                            {
                                                if (mainFile != null && File.Exists(mainFile) && mainFile.EndsWith(searchString))
                                                {
                                                    FileInfo mainFI = new FileInfo(mainFile);
                                                    if (mainFI != null && mainFI.Name != null && mainFI.Name.StartsWith(GHConstants.ReplayFileNamePrefix))
                                                    {
                                                        mainFileFound = true;
                                                        mainFileName = mainFile;
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                        if (mainFileFound && mainFileName != null)
                                        {
                                            FileInfo mainFI = new FileInfo(mainFileName);
                                            int mainMiddleLen = mainFI.Name.Length - GHConstants.ReplayFileNamePrefix.Length - GHConstants.ReplayFileNameSuffix.Length - (GHApp.UseGZipForReplays ? GHConstants.ReplayGZipFileNameSuffix.Length : GHConstants.ReplayZipFileNameSuffix.Length);
                                            int numberStart = fi.Name.Length - extraRemoved - GHConstants.ReplayFileNameSuffix.Length;
                                            if (mainMiddleLen > 0 && numberStart > 0)
                                            {
                                                try
                                                {
                                                    string mainMiddleStr = mainFI.Name.Substring(GHConstants.ReplayFileNamePrefix.Length, mainMiddleLen);
                                                    string numberStr = fi.Name.Substring(numberStart, extraRemoved + GHConstants.ReplayFileNameSuffix.Length);
                                                    string newFileName = GHConstants.ReplayContinuationFileNamePrefix + mainMiddleStr + numberStr;
                                                    string newFile = Path.Combine(dirPath, newFileName);
                                                    string zipFile = newFile + (GHApp.UseGZipForReplays ? GHConstants.ReplayGZipFileNameSuffix : GHConstants.ReplayZipFileNameSuffix);

                                                    if(newFile != file)
                                                    {
                                                        if (File.Exists(newFile))
                                                            File.Delete(newFile);

                                                        File.Move(file, newFile);
                                                    }

                                                    /* Zip it */
                                                    if (GHApp.UseGZipForReplays)
                                                    {
                                                        using (FileStream originalFileStream = File.Open(newFile, FileMode.Open))
                                                        {
                                                            using (FileStream compressedFileStream = File.Create(zipFile))
                                                            {
                                                                using (var compressor = new GZipStream(compressedFileStream, CompressionMode.Compress))
                                                                {
                                                                    originalFileStream.CopyTo(compressor);
                                                                }
                                                            }
                                                        }
                                                    }
                                                    else
                                                    {
                                                        using (ZipArchive archive = ZipFile.Open(zipFile, ZipArchiveMode.Create))
                                                        {
                                                            archive.CreateEntryFromFile(newFile, Path.GetFileName(newFile));
                                                        }
                                                    }

                                                    if (File.Exists(newFile) && File.Exists(zipFile))
                                                        File.Delete(newFile);
                                                }
                                                catch (Exception ex)
                                                {
                                                    Debug.WriteLine(ex);
                                                }
                                            }
                                        }
                                        else
                                        {
                                            /* Delete it */
                                            try
                                            {
                                                File.Delete(file);
                                            }
                                            catch (Exception ex)
                                            {
                                                Debug.WriteLine(ex);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            ReplayCollectionView.ItemsSource = gHRecordedGameFiles;
            UpdateRecordingsLabel();
        }

        private void UpdateRecordingsLabel()
        {
            if(IsMultiSelect)
            {
                int i = 0;
                long totalBytes = 0;
                if (ReplayCollectionView.SelectedItems != null)
                {
                    //i = ReplayCollectionView.SelectedItems.Count;
                    foreach (object obj in ReplayCollectionView.SelectedItems)
                    {
                        if(obj is GHRecordedGameFile)
                        {
                            GHRecordedGameFile file = (GHRecordedGameFile)obj;
                            if(!file.IsFolder)
                            {
                                i++;
                                totalBytes += file.FileSize;
                            }
                        }
                    }
                }
                RecordingsLabel.Text = i.ToString() + " replay" + (i == 1 ? "" : "s") + " selected, " + (totalBytes < 1024 * 1024 ? string.Format("{0} kB", totalBytes / 1024) : string.Format("{0:0.0} MB", (double)totalBytes / (1024 * 1024)));
            }
            else
            {
                int i = 0;
                long totalBytes = 0;
                if(ReplayCollectionView.ItemsSource != null)
                {
                    if(ReplayCollectionView.ItemsSource is ObservableCollection<GHRecordedGameFile>)
                    {
                        ObservableCollection<GHRecordedGameFile> fileCollection = (ObservableCollection<GHRecordedGameFile>)ReplayCollectionView.ItemsSource;
                        //i = fileCollection.Count;
                        foreach(GHRecordedGameFile file in fileCollection)
                        {
                            if(!file.IsFolder)
                            {
                                i++;
                                totalBytes += file.FileSize;
                            }
                        }
                    }
                }
                RecordingsLabel.Text = i.ToString() + " replay" + (i == 1 ? "" : "s") + ", " + (totalBytes < 1024 * 1024 ? string.Format("{0} kB", totalBytes / 1024) : string.Format("{0:0.0} MB", (double)totalBytes / (1024 * 1024)));
            }
        }

        private async void ShareButton_Clicked(object sender, EventArgs e)
        {
            ShareButton.IsEnabled = false;
            GHApp.PlayButtonClickedSound();

            if(IsMultiSelect && ReplayCollectionView.SelectedItems != null && ReplayCollectionView.SelectedItems.Count != 1)
            {
                int noFiles = ReplayCollectionView.SelectedItems.Count;
                if (noFiles > 0)
                {
                    try
                    {
                        /* Create zip of all relevant files */
                        string targetpath = Path.Combine(GHApp.GHPath, GHConstants.ArchiveDirectory);
                        if (!Directory.Exists(targetpath))
                            GHApp.CheckCreateDirectory(targetpath);

                        string zipFile = Path.Combine(targetpath, GHConstants.ReplaySharedZipFileNamePrefix + "archive-of-" + noFiles + "-replays" + GHConstants.ReplaySharedZipFileNameSuffix);
                        if (File.Exists(zipFile))
                            File.Delete(zipFile);

                        using (ZipArchive archive = ZipFile.Open(zipFile, ZipArchiveMode.Create))
                        {
                            foreach (object selItem in ReplayCollectionView.SelectedItems)
                            {
                                GHRecordedGameFile recfile = null;
                                string filePath = null;
                                if (selItem != null)
                                {
                                    if (selItem is GHRecordedGameFile)
                                        recfile = ((GHRecordedGameFile)selItem);
                                    if (recfile != null)
                                        filePath = recfile.FilePath;
                                }

                                if (!string.IsNullOrWhiteSpace(filePath) && recfile != null && File.Exists(filePath))
                                {
                                    FileInfo fi = new FileInfo(filePath);
                                    string fileName = fi.Name;
                                    string dir = fi.DirectoryName;
                                    if (!string.IsNullOrWhiteSpace(fileName) && fileName.StartsWith(GHConstants.ReplayFileNamePrefix) && Directory.Exists(dir))
                                    {
                                        archive.CreateEntryFromFile(filePath, Path.GetFileName(filePath));

                                        bool isGZip = fileName.Length > GHConstants.ReplayGZipFileNameSuffix.Length && fileName.EndsWith(GHConstants.ReplayGZipFileNameSuffix);
                                        bool isNormalZip = fileName.Length > GHConstants.ReplayZipFileNameSuffix.Length && fileName.EndsWith(GHConstants.ReplayZipFileNameSuffix);
                                        bool isZip = isGZip || isNormalZip;
                                        string usedZipSuffix = isGZip ? GHConstants.ReplayGZipFileNameSuffix : GHConstants.ReplayZipFileNameSuffix;
                                        int subLen = fileName.Length - GHConstants.ReplayFileNamePrefix.Length - GHConstants.ReplayFileNameSuffix.Length - (isZip ? usedZipSuffix.Length : 0);
                                        if (subLen > 0)
                                        {
                                            string subString = fileName.Substring(GHConstants.ReplayFileNamePrefix.Length, subLen);
                                            string[] files = Directory.GetFiles(dir);
                                            if (files != null)
                                            {
                                                foreach (string file in files)
                                                {
                                                    if (!string.IsNullOrWhiteSpace(file))
                                                    {
                                                        string contStart = GHConstants.ReplayContinuationFileNamePrefix + subString;
                                                        FileInfo contFI = new FileInfo(file);
                                                        if (contFI != null && !string.IsNullOrWhiteSpace(contFI.Name))
                                                        {
                                                            if (contFI.Name.StartsWith(contStart) && (!isZip || file.EndsWith(usedZipSuffix)) && File.Exists(file))
                                                            {
                                                                archive.CreateEntryFromFile(file, Path.GetFileName(file));
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        await Share.RequestAsync(new ShareFileRequest
                        {
                            Title = "Sharing " + noFiles + " Replay" + (noFiles != 1 ? "s" : ""),
                            File = new ShareFile(zipFile)
                        });
                    }
                    catch (Exception ex)
                    {
                        await DisplayAlert("Share File Failure", "GnollHack failed to share " + noFiles + " replay" + (noFiles != 1 ? "s" : "") + ": " + ex.Message, "OK");
                    }
                }
            }
            else
            {
                GHRecordedGameFile recfile = null;
                string filePath = null;
                if(IsMultiSelect)
                {
                    if (ReplayCollectionView.SelectedItems != null && ReplayCollectionView.SelectedItems.Count == 1)
                    {
                        if (ReplayCollectionView.SelectedItems[0] is GHRecordedGameFile)
                            recfile = ((GHRecordedGameFile)ReplayCollectionView.SelectedItems[0]);
                        if (recfile != null)
                            filePath = recfile.FilePath;
                    }
                }
                else
                {
                    if (ReplayCollectionView.SelectedItem != null)
                    {
                        if (ReplayCollectionView.SelectedItem is GHRecordedGameFile)
                            recfile = ((GHRecordedGameFile)ReplayCollectionView.SelectedItem);
                        if (recfile != null)
                            filePath = recfile.FilePath;
                    }
                }

                if (!string.IsNullOrWhiteSpace(filePath) && recfile != null)
                {
                    try
                    {
                        /* Create zip of all relevant files */
                        string targetpath = Path.Combine(GHApp.GHPath, GHConstants.ArchiveDirectory);
                        if (!Directory.Exists(targetpath))
                            GHApp.CheckCreateDirectory(targetpath);

                        FileInfo fi = new FileInfo(filePath);
                        string fileName = fi.Name;
                        string dir = fi.DirectoryName;
                        if (!string.IsNullOrWhiteSpace(fileName) && fileName.StartsWith(GHConstants.ReplayFileNamePrefix) && Directory.Exists(dir))
                        {
                            bool isGZip = fileName.Length > GHConstants.ReplayGZipFileNameSuffix.Length && fileName.EndsWith(GHConstants.ReplayGZipFileNameSuffix);
                            bool isNormalZip = fileName.Length > GHConstants.ReplayZipFileNameSuffix.Length && fileName.EndsWith(GHConstants.ReplayZipFileNameSuffix);
                            bool isZip = isGZip || isNormalZip;
                            string usedZipSuffix = isGZip ? GHConstants.ReplayGZipFileNameSuffix : GHConstants.ReplayZipFileNameSuffix;
                            int subLen = fileName.Length - GHConstants.ReplayFileNamePrefix.Length - GHConstants.ReplayFileNameSuffix.Length - (isZip ? usedZipSuffix.Length : 0);
                            if (subLen > 0)
                            {
                                string subString = fileName.Substring(GHConstants.ReplayFileNamePrefix.Length, subLen);
                                string zipFile = Path.Combine(targetpath, GHConstants.ReplaySharedZipFileNamePrefix + subString + GHConstants.ReplaySharedZipFileNameSuffix);
                                if (File.Exists(zipFile))
                                    File.Delete(zipFile);

                                using (ZipArchive archive = ZipFile.Open(zipFile, ZipArchiveMode.Create))
                                {
                                    archive.CreateEntryFromFile(filePath, Path.GetFileName(filePath));
                                    string[] files = Directory.GetFiles(dir);
                                    if (files != null)
                                    {
                                        foreach (string file in files)
                                        {
                                            if (!string.IsNullOrWhiteSpace(file))
                                            {
                                                string contStart = GHConstants.ReplayContinuationFileNamePrefix + subString;
                                                FileInfo contFI = new FileInfo(file);
                                                if (contFI != null && !string.IsNullOrWhiteSpace(contFI.Name))
                                                {
                                                    if (contFI.Name.StartsWith(contStart) && (!isZip || file.EndsWith(usedZipSuffix)) && File.Exists(file))
                                                    {
                                                        archive.CreateEntryFromFile(file, Path.GetFileName(file));
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                                await Share.RequestAsync(new ShareFileRequest
                                {
                                    Title = "Sharing " + recfile.FileName,
                                    File = new ShareFile(zipFile)
                                });
                            }
                        }
                    }
                    catch (Exception ex)
                    {
                        await DisplayAlert("Share File Failure", "GnollHack failed to share " + filePath + ": " + ex.Message, "OK");
                    }
                }
            }

            ShareButton.IsEnabled = true;
            PopupCancelButton_Clicked(sender, e);
        }

        private async void SelectButton_Clicked(object sender, EventArgs e)
        {
            GHRecordedGameFile rgf = null;
            if (!IsMultiSelect && ReplayCollectionView.SelectedItem != null)
            {
                if (ReplayCollectionView.SelectedItem is GHRecordedGameFile)
                    rgf = (GHRecordedGameFile)ReplayCollectionView.SelectedItem;
            }

            if (IsMultiSelect && ReplayCollectionView.SelectedItems.Count == 1)
            {
                if (ReplayCollectionView.SelectedItems[0] is GHRecordedGameFile)
                    rgf = (GHRecordedGameFile)ReplayCollectionView.SelectedItems[0];
            }

            if (rgf != null && rgf.IsFolder)
            {
                if(IsCloud)
                    _subDirectoryServer = rgf.FilePath;
                else if(IsDownload)
                    _subDirectoryDownload = string.IsNullOrWhiteSpace(rgf.FilePath) ? null : rgf.FileName + rgf.Extension;
                else if (IsLocal)
                    _subDirectoryLocal = string.IsNullOrWhiteSpace(rgf.FilePath) ? null : rgf.FileName + rgf.Extension;
                UpdateRecordings();
                return;
            }

            if (IsCloud)
            {
                /* Download */
                DownloadButton_Clicked(sender, e);
                return;
            }

            /* Play or Open */
            SelectButton.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            string filePath = null;
            if (rgf != null)
                filePath = rgf.FilePath;

            if (filePath != null)
            {
                string outstr;
                if(GHApp.ValidateReplayFile(filePath, out outstr))
                {
                    var gamePage = new GamePage(_mainPage);
                    GHApp.CurrentGamePage = gamePage;
                    await App.Current.MainPage.Navigation.PushModalAsync(gamePage);
                    gamePage.StartReplay(filePath, -1);
                }
                else
                {
                    FileInfo fi = new FileInfo(filePath);
                    bool ans = await DisplayAlert("Replay Invalid", "Replay " + fi.Name + " is invalid: " + outstr + ". Delete it?", "Yes", "No");
                    if(ans)
                    {
                        try
                        {
                            GHApp.DeleteReplay(filePath);
                            UpdateRecordings();
                        }
                        catch (Exception ex)
                        {
                            await DisplayAlert("Deletion Failed", "GnollHack failed to delete game recording " + filePath + ": " + ex.Message, "OK");
                        }
                    }
                }
            }
            SelectButton.IsEnabled = true;
        }

        private async void CloseButton_Clicked(object sender, EventArgs e)
        {
            CloseButton.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            await App.Current.MainPage.Navigation.PopModalAsync();
        }

        private double _currentPageWidth = 0;
        private double _currentPageHeight = 0;
        protected override void OnSizeAllocated(double width, double height)
        {
            base.OnSizeAllocated(width, height);
            if (width != _currentPageWidth || height != _currentPageHeight)
            {
                _currentPageWidth = width;
                _currentPageHeight = height;
                ReplayCollectionView.Margin = UIUtils.GetMiddleElementMarginWithBorder(bkgView.BorderStyle, width, height);
                HeaderLabel.Margin = UIUtils.GetHeaderMarginWithBorder(bkgView.BorderStyle, width, height);
                BottomLayout.Margin = UIUtils.GetFooterMarginWithBorder(bkgView.BorderStyle, width, height);

                if(width > height && width - BottomLayout.Margin.Left - BottomLayout.Margin.Right >= 500)
                {
                    ButtonMainStack.Orientation = StackOrientation.Horizontal;
                }
                else
                {
                    ButtonMainStack.Orientation = StackOrientation.Vertical;
                }
            }
        }

        private void MainListView_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            UpdateButtons();
        }

        private void UpdateButtons()
        {
            if(IsCloud)
            {
                MoreButton.IsEnabled = false;
                MoreButton.TextColor = GHColors.Gray;
                if (ReplayCollectionView.SelectionMode == SelectionMode.Single)
                {
                    if (ReplayCollectionView.SelectedItem is GHRecordedGameFile)
                    {
                        if (ReplayCollectionView.SelectedItem != null)
                        {
                            SelectButton.IsEnabled = true;
                            GHRecordedGameFile rgf = (GHRecordedGameFile)ReplayCollectionView.SelectedItem;
                            if (rgf.IsFolder)
                            {
                                SelectButton.TextColor = GHColors.LightBlue;
                                SelectButton.Text = "Open";
                                SelectButton.FontSize = 15;
                            }
                            else
                            {
                                SelectButton.TextColor = GHColors.Yellow;
                                SelectButton.Text = "Download";
                                SelectButton.FontSize = 13;
                            }
                        }
                        else
                        {
                            SelectButton.IsEnabled = false;
                            SelectButton.TextColor = GHColors.Gray;
                            SelectButton.Text = "Download";
                            SelectButton.FontSize = 13;
                        }
                    }
                    else
                    {
                        SelectButton.IsEnabled = false;
                        SelectButton.TextColor = GHColors.Gray;
                        SelectButton.Text = "Download";
                        SelectButton.FontSize = 13;
                    }
                }
                else if (ReplayCollectionView.SelectionMode == SelectionMode.Multiple)
                {
                    if (ReplayCollectionView.SelectedItems.Count == 1 && ReplayCollectionView.SelectedItems[0] != null && ReplayCollectionView.SelectedItems[0] is GHRecordedGameFile)
                    {
                        SelectButton.IsEnabled = true;
                        GHRecordedGameFile rgf = (GHRecordedGameFile)ReplayCollectionView.SelectedItems[0];
                        if (rgf.IsFolder)
                        {
                            SelectButton.TextColor = GHColors.LightBlue;
                            SelectButton.Text = "Open";
                            SelectButton.FontSize = 15;
                        }
                        else
                        {
                            SelectButton.TextColor = GHColors.Yellow;
                            SelectButton.Text = "Download";
                            SelectButton.FontSize = 13;
                        }
                    }
                    else
                    {

                        SelectButton.Text = "Download";
                        SelectButton.FontSize = 13;
                        if (ReplayCollectionView.SelectedItems.Count > 0)
                        {
                            SelectButton.IsEnabled = true;
                            SelectButton.TextColor = GHColors.Yellow;
                        }
                        else
                        {
                            SelectButton.IsEnabled = false;
                            SelectButton.TextColor = GHColors.Gray;
                        }
                    }
                    UpdateRecordingsLabel();
                }
            }
            else
            {
                SelectButton.FontSize = 15;
                if (ReplayCollectionView.SelectionMode == SelectionMode.Single)
                {
                    if (ReplayCollectionView.SelectedItem != null)
                    {
                        MoreButton.IsEnabled = true;
                        SelectButton.IsEnabled = true;
                        MoreButton.TextColor = GHColors.White;
                        if (ReplayCollectionView.SelectedItem is GHRecordedGameFile)
                        {
                            GHRecordedGameFile rgf = (GHRecordedGameFile)ReplayCollectionView.SelectedItem;
                            if (rgf.IsFolder)
                            {
                                SelectButton.TextColor = GHColors.LightBlue;
                                SelectButton.Text = "Open";
                            }
                            else
                            {
                                SelectButton.TextColor = GHColors.BrighterGreen;
                                SelectButton.Text = "Play";
                            }
                        }
                        else
                        {
                            SelectButton.TextColor = GHColors.White;
                        }
                    }
                    else
                    {
                        SelectButton.Text = "Play";
                        MoreButton.IsEnabled = false;
                        SelectButton.IsEnabled = false;
                        MoreButton.TextColor = GHColors.Gray;
                        SelectButton.TextColor = GHColors.Gray;
                    }
                }
                else if (ReplayCollectionView.SelectionMode == SelectionMode.Multiple)
                {
                    if(ReplayCollectionView.SelectedItems.Count == 1 && ReplayCollectionView.SelectedItems[0] != null && ReplayCollectionView.SelectedItems[0] is GHRecordedGameFile)
                    {
                        MoreButton.IsEnabled = true;
                        SelectButton.IsEnabled = true;
                        MoreButton.TextColor = GHColors.White;
                        GHRecordedGameFile rgf = (GHRecordedGameFile)ReplayCollectionView.SelectedItems[0];
                        if (rgf.IsFolder)
                        {
                            SelectButton.TextColor = GHColors.LightBlue;
                            SelectButton.Text = "Open";
                        }
                        else
                        {
                            SelectButton.TextColor = GHColors.BrighterGreen;
                            SelectButton.Text = "Play";
                        }
                    }
                    else
                    {
                        SelectButton.Text = "Play";
                        if (ReplayCollectionView.SelectedItems.Count > 0)
                        {
                            MoreButton.IsEnabled = true;
                            SelectButton.IsEnabled = false;
                            MoreButton.TextColor = GHColors.White;
                            SelectButton.TextColor = GHColors.Gray;
                        }
                        else
                        {
                            MoreButton.IsEnabled = false;
                            SelectButton.IsEnabled = false;
                            MoreButton.TextColor = GHColors.Gray;
                            SelectButton.TextColor = GHColors.Gray;
                        }
                    }
                    UpdateRecordingsLabel();
                }
            }
        }

        private async void DeleteButton_Clicked(object sender, EventArgs e)
        {
            DeleteButton.IsEnabled = false;
            GHApp.PlayButtonClickedSound();

            if(IsMultiSelect)
            {
                if (ReplayCollectionView.SelectedItems != null && ReplayCollectionView.SelectedItems.Count > 0)
                {
                    int noErrors = 0;
                    int noFiles = ReplayCollectionView.SelectedItems.Count;
                    string qText;
                    if (noFiles == 1 && ReplayCollectionView.SelectedItems[0] is GHRecordedGameFile)
                        qText = "Are you sure to delete the replay " + ((GHRecordedGameFile)ReplayCollectionView.SelectedItems[0]).FileName + "?";
                    else
                        qText = "Are you sure to delete the " + noFiles + " selected replay" + (noFiles != 1 ? "s" : "") + "?";
                    if (await DisplayAlert("Confirm Deletion", qText, "Yes", "No"))
                    {
                        foreach (object selItem in ReplayCollectionView.SelectedItems)
                        {
                            GHRecordedGameFile recfile = null;
                            string filePath = null;
                            if (selItem != null)
                            {
                                if (selItem is GHRecordedGameFile)
                                    recfile = ((GHRecordedGameFile)selItem);
                                if (recfile != null)
                                    filePath = recfile.FilePath;
                            }
                            if (filePath != null && recfile != null)
                            {
                                try
                                {
                                    GHApp.DeleteReplay(filePath);
                                }
                                catch (Exception ex)
                                {
                                    GHApp.MaybeWriteGHLog("An error occurred while trying to delete " + recfile.FileName + ": " + ex.Message);
                                    noErrors++;
                                }
                            }
                        }
                        if (noErrors > 0)
                        {
                            await DisplayAlert("Deletion Failed",
                                noErrors + " error" + (noErrors != 1 ? "s" : "") + " occurred when trying to delete " + noFiles + " replay" + (noFiles != 1 ? "s" : "") + "."
                                + (GHApp.DebugLogMessages ? " See App Log for details." : ""),
                                "OK");
                        }
                        UpdateRecordings();
                    }
                }
            }
            else
            {
                GHRecordedGameFile recfile = null;
                string filePath = null;

                if (ReplayCollectionView.SelectedItem != null)
                {
                    if (ReplayCollectionView.SelectedItem is GHRecordedGameFile)
                        recfile = ((GHRecordedGameFile)ReplayCollectionView.SelectedItem);
                    if (recfile != null)
                        filePath = recfile.FilePath;
                }

                if (filePath != null && recfile != null)
                {
                    if (await DisplayAlert("Confirm Deletion", "Are you sure to delete the game recording " + recfile.FileName + "?", "Yes", "No"))
                    {
                        try
                        {
                            GHApp.DeleteReplay(filePath);
                            UpdateRecordings();
                        }
                        catch (Exception ex)
                        {
                            await DisplayAlert("Deletion Failed", "GnollHack failed to delete game recording " + filePath + ": " + ex.Message, "OK");
                        }
                    }
                }
            }

            DeleteButton.IsEnabled = true;
            PopupCancelButton_Clicked(sender, e);
        }

        private void MultiButton_Clicked(object sender, EventArgs e)
        {
            ReplayCollectionView.SelectedItem = null;
            ReplayCollectionView.SelectedItems.Clear();
            if (IsMultiSelect)
            {
                ReplayCollectionView.SelectionMode = SelectionMode.Single;
                MultiButton.Text = "Multiple";
                bkgView.BorderStyle = BorderStyles.Simple;
            }
            else
            {
                ReplayCollectionView.SelectionMode = SelectionMode.Multiple;
                MultiButton.Text = "Single";
                bkgView.BorderStyle = BorderStyles.SimpleAlternative;
            }
            UpdateRecordingsLabel();
        }

        private async void UpdateServerRecordings()
        {
            BlobServiceClient client = GHApp.GetBlobServiceClient();
            if (client == null)
                return;
            BlobContainerClient containerClient = client.GetBlobContainerClient(GHConstants.AzureBlobStorageReplayContainerName);
            if (containerClient == null)
                return;

            RecordingsLabel.Text = "(Loading...)";
            ReplayCollectionView.SelectedItem = null;
            ReplayCollectionView.SelectedItems.Clear();
            ReplayCollectionView.ItemsSource = null;
            MoreButton.IsEnabled = false;
            SelectButton.IsEnabled = false;
            MoreButton.TextColor = GHColors.Gray;
            SelectButton.TextColor = GHColors.Gray;

            List<GHRecordedGameFile> gHRecordedGameFiles = new List<GHRecordedGameFile>();
            List<ContinuationFile> contFiles = new List<ContinuationFile>();            
            int i = 0;

            try
            {
                if(!string.IsNullOrWhiteSpace(_subDirectoryServer))
                {
                    gHRecordedGameFiles.Add(new GHRecordedGameFile(0, null, "(Back)", "", true, 0, 1, DateTime.Now, DateTime.Now));
                }

                // Call the listing operation and return pages of the specified size.
                var resultSegment = containerClient.GetBlobsByHierarchyAsync(prefix: _subDirectoryServer, delimiter: GHConstants.AzureBlobStorageDelimiter)
                    .AsPages(default, null);

                var enumer = resultSegment.GetAsyncEnumerator();

                try
                {
                    // Enumerate the blobs returned for each page.
                    while (await enumer.MoveNextAsync())
                    {
                        Page<BlobHierarchyItem> blobPage = enumer.Current;
                        // A hierarchical listing may return both virtual directories and blobs.
                        foreach (BlobHierarchyItem blobhierarchyItem in blobPage.Values)
                        {
                            if (blobhierarchyItem.IsPrefix)
                            {
                                // Write out the prefix of the virtual directory.
                                Debug.WriteLine(i + ": Virtual directory prefix: " + blobhierarchyItem.Prefix);
                                gHRecordedGameFiles.Add(new GHRecordedGameFile(i, blobhierarchyItem.Prefix, blobhierarchyItem.Prefix, "", true, 0, 1, DateTime.Now, DateTime.Now));
                            }
                            else if(blobhierarchyItem.Blob != null)
                            {
                                Debug.WriteLine(i + ": Item name without prefix: " + blobhierarchyItem.Blob.Name);
                                if(!string.IsNullOrWhiteSpace(blobhierarchyItem.Blob.Name))
                                {
                                    string nameWithoutPrefix;
                                    if(string.IsNullOrWhiteSpace(_subDirectoryServer))
                                    {
                                        nameWithoutPrefix = blobhierarchyItem.Blob.Name;
                                    }
                                    else
                                    {
                                        if (blobhierarchyItem.Blob.Name.Length > _subDirectoryServer.Length && blobhierarchyItem.Blob.Name.StartsWith(_subDirectoryServer))
                                            nameWithoutPrefix = blobhierarchyItem.Blob.Name.Substring(_subDirectoryServer.Length);
                                        else
                                            nameWithoutPrefix = blobhierarchyItem.Blob.Name;
                                    }

                                    if(nameWithoutPrefix.StartsWith(GHConstants.ReplayFileNamePrefix))
                                    {
                                        i++;
                                        gHRecordedGameFiles.Add(new GHRecordedGameFile(i, blobhierarchyItem.Blob?.Name, blobhierarchyItem.Blob?.Name, "", false,
                                            blobhierarchyItem.Blob?.Properties.ContentLength ?? 0, 1,
                                              (blobhierarchyItem.Blob?.Properties.CreatedOn ?? DateTimeOffset.Now).DateTime,
                                              (blobhierarchyItem.Blob?.Properties.LastModified ?? DateTimeOffset.Now).DateTime));
                                    }
                                    else if (nameWithoutPrefix.StartsWith(GHConstants.ReplayContinuationFileNamePrefix))
                                    {
                                        if(!string.IsNullOrWhiteSpace(blobhierarchyItem.Blob.Name))
                                            contFiles.Add(new ContinuationFile(blobhierarchyItem.Blob.Name, blobhierarchyItem.Blob?.Properties.ContentLength ?? 0));
                                    }
                                }
                            }
                            else
                            {
                                Debug.WriteLine(i + ": Blob is null");
                                //i++;
                                //gHRecordedGameFiles.Add(new GHRecordedGameFile(i, "?", "?", "", false, 0, 1, DateTime.Now, DateTime.Now));

                            }
                        }
                    }

                    string prefix = !string.IsNullOrWhiteSpace(_subDirectoryServer) ? _subDirectoryServer : "";
                    foreach (ContinuationFile contFile in contFiles)
                    {
                        string contFileName = contFile.FileName;
                        if (string.IsNullOrWhiteSpace(contFileName))
                            continue;

                        int extraRemoved = 0;
                        for (int j = contFileName.Length - GHConstants.ReplayFileNameSuffix.Length - GHConstants.ReplayGZipFileNameSuffix.Length - 1; j >= 0; j--)
                        {
                            char c = contFileName[j];
                            if (c >= '0' && c <= '9')
                            {
                                if (j == 0)
                                {
                                    extraRemoved = 0; /* Error */
                                    break;
                                }
                                else
                                    extraRemoved++;
                            }
                            else if (c == GHConstants.ReplayFileContinuationNumberDivisor[0])
                            {
                                extraRemoved++;
                                break;
                            }
                            else
                            {
                                extraRemoved = 0; /* Error */
                                break;
                            }
                        }
                        if (contFileName.Length > prefix.Length + GHConstants.ReplayFileNamePrefix.Length + GHConstants.ReplayFileNameSuffix.Length + GHConstants.ReplayGZipFileNameSuffix.Length + extraRemoved)
                        {
                            string middlePart = contFileName.Substring(
                                prefix.Length + GHConstants.ReplayFileNamePrefix.Length,
                                contFileName.Length - prefix.Length - GHConstants.ReplayFileNamePrefix.Length - GHConstants.ReplayFileNameSuffix.Length - GHConstants.ReplayGZipFileNameSuffix.Length - extraRemoved);

                            foreach (GHRecordedGameFile rgf in gHRecordedGameFiles)
                            {
                                if(rgf != null && rgf.FilePath != null)
                                {
                                    if (rgf.FilePath.StartsWith(prefix + GHConstants.ReplayFileNamePrefix + middlePart))
                                    {
                                        rgf.ContinuationFiles.Add(new ContinuationFile(contFileName, contFile.FileSize));
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
                catch (Exception e)
                {
                    Debug.WriteLine(e.Message);
                }
                finally
                {
                    if (enumer != null)
                        await enumer.DisposeAsync();
                }
            }
            catch (Exception e)
            {
                Debug.WriteLine(e.Message);
            }

            gHRecordedGameFiles.Sort(new RecordedGameFileComparer());
            ObservableCollection<GHRecordedGameFile> oc = new ObservableCollection<GHRecordedGameFile>();
            int idx = 0;
            foreach(GHRecordedGameFile rgf in gHRecordedGameFiles)
            {
                if(!rgf.IsFolder)
                {
                    string nameWithoutPrefix;
                    string nameext = rgf.FileName + rgf.Extension;
                    if (string.IsNullOrWhiteSpace(_subDirectoryServer) || nameext.Length <= _subDirectoryServer.Length || !nameext.StartsWith(_subDirectoryServer))
                        nameWithoutPrefix = rgf.FileName + rgf.Extension;
                    else
                        nameWithoutPrefix = nameext.Substring(_subDirectoryServer.Length);
                    string basePath = Path.Combine(GHApp.GHPath, GHConstants.ReplayDownloadFromCloudDirectory);
                    string filePath;
                    if (!string.IsNullOrWhiteSpace(_subDirectoryServer) && _subDirectoryServer.Length > 0)
                        filePath = Path.Combine(basePath, _subDirectoryServer[_subDirectoryServer.Length - 1] == GHConstants.AzureBlobStorageDelimiter[0] ? _subDirectoryServer.Substring(0, _subDirectoryServer.Length - 1) : _subDirectoryServer, nameWithoutPrefix);
                    else
                        filePath = Path.Combine(basePath, nameWithoutPrefix);

                    if (File.Exists(filePath))
                        rgf.Downloaded = true;

                    string username = GHApp.XlogUserName;
                    if(!string.IsNullOrWhiteSpace(_subDirectoryServer) && !string.IsNullOrWhiteSpace(username) && GHApp.XlogUserNameVerified)
                    {
                        string usedDirName;
                        if (_subDirectoryServer.Length > 1 && _subDirectoryServer[_subDirectoryServer.Length - 1] == GHConstants.AzureBlobStorageDelimiter[0])
                            usedDirName = _subDirectoryServer.Substring(0, _subDirectoryServer.Length - 1);
                        else
                            usedDirName = _subDirectoryServer;

                        if(usedDirName == username)
                        {
                            string basePathLocal = Path.Combine(GHApp.GHPath, GHConstants.ReplayDirectory);
                            string filePathLocal = Path.Combine(basePathLocal, nameWithoutPrefix);
                            if (File.Exists(filePathLocal))
                                rgf.Uploaded = true;
                        }
                    }

                    /* Sort indices in the right order */
                    idx++;
                    rgf.Index = idx;
                }
                oc.Add(rgf);
            }

            ReplayCollectionView.ItemsSource = oc;
            UpdateRecordingsLabel();
        }

        private object _uploadDownloadLock = new object();
        private bool _uploadDownloadCancelled = false;
        public bool UploadDownloadCancelled { get { lock (_uploadDownloadLock) { return _uploadDownloadCancelled; } } set { lock (_uploadDownloadLock) { _uploadDownloadCancelled = value; } } }
        private CancellationTokenSource _uploadDownloadCts = null;

        private async void UploadButton_Clicked(object sender, EventArgs e)
        {
            UploadButton.IsEnabled = false;
            GHApp.PlayButtonClickedSound();

            UploadDownloadTitleLabel.Text = "Uploading...";
            UploadDownloadFileLabel.Text = "Initializing...";
            UploadDownloadStatusLabel.Text = "Please wait...";
            UploadDownloadGrid.IsVisible = true;
            UploadDownloadCancelled = false;
            if(_uploadDownloadCts == null)
                _uploadDownloadCts = new CancellationTokenSource();

            BlobServiceClient blobServiceClient = GHApp.GetBlobServiceClient();
            BlobContainerClient blobContainerClient = blobServiceClient.GetBlobContainerClient(GHConstants.AzureBlobStorageReplayContainerName);
            string prefix = GHApp.XlogUserNameVerified ? GHApp.XlogUserName : GHConstants.AzureBlobStorageGeneralDirectoryName;

            if (IsMultiSelect && ReplayCollectionView.SelectedItems != null && ReplayCollectionView.SelectedItems.Count != 1)
            {
                int noFiles = ReplayCollectionView.SelectedItems.Count;
                if (noFiles > 0)
                {
                    try
                    {
                        int fileIdx = 0;
                        int noValidFiles = 0;
                        foreach (object selItem in ReplayCollectionView.SelectedItems)
                        {

                            GHRecordedGameFile recfile = null;
                            string filePath = null;
                            if (selItem != null)
                            {
                                if (selItem is GHRecordedGameFile)
                                    recfile = ((GHRecordedGameFile)selItem);
                                if (recfile != null)
                                    filePath = recfile.FilePath;
                            }

                            if (recfile != null && recfile.IsFolder)
                                continue;

                            if (!string.IsNullOrWhiteSpace(filePath) && recfile != null && File.Exists(filePath))
                            {
                                FileInfo fi = new FileInfo(filePath);
                                string fileName = fi.Name;
                                string dir = fi.DirectoryName;
                                if (!string.IsNullOrWhiteSpace(fileName) && fileName.StartsWith(GHConstants.ReplayFileNamePrefix) && Directory.Exists(dir))
                                {
                                    noValidFiles++;
                                }
                            }
                        }

                        foreach (object selItem in ReplayCollectionView.SelectedItems)
                        {
                            if (UploadDownloadCancelled)
                                break;

                            GHRecordedGameFile recfile = null;
                            string filePath = null;
                            if (selItem != null)
                            {
                                if (selItem is GHRecordedGameFile)
                                    recfile = ((GHRecordedGameFile)selItem);
                                if (recfile != null)
                                    filePath = recfile.FilePath;
                            }

                            if (recfile != null && recfile.IsFolder)
                                continue;

                            if (!string.IsNullOrWhiteSpace(filePath) && recfile != null && File.Exists(filePath))
                            {
                                FileInfo fi = new FileInfo(filePath);
                                string fileName = fi.Name;
                                string dir = fi.DirectoryName;
                                if (!string.IsNullOrWhiteSpace(fileName) && fileName.StartsWith(GHConstants.ReplayFileNamePrefix) && Directory.Exists(dir))
                                {
                                    fileIdx++;
                                    UploadDownloadFileLabel.Text = fileName;
                                    UploadDownloadStatusLabel.Text = "Main replay file " + fileIdx + " of " + noValidFiles;
                                    await GHApp.UploadFromFileAsync(blobContainerClient, prefix, filePath, _uploadDownloadCts.Token);
                                    recfile.Uploaded = true;
                                    if (UploadDownloadCancelled)
                                        break;

                                    bool isGZip = fileName.Length > GHConstants.ReplayGZipFileNameSuffix.Length && fileName.EndsWith(GHConstants.ReplayGZipFileNameSuffix);
                                    bool isNormalZip = fileName.Length > GHConstants.ReplayZipFileNameSuffix.Length && fileName.EndsWith(GHConstants.ReplayZipFileNameSuffix);
                                    bool isZip = isGZip || isNormalZip;
                                    string usedZipSuffix = isGZip ? GHConstants.ReplayGZipFileNameSuffix : GHConstants.ReplayZipFileNameSuffix;
                                    int subLen = fileName.Length - GHConstants.ReplayFileNamePrefix.Length - GHConstants.ReplayFileNameSuffix.Length - (isZip ? usedZipSuffix.Length : 0);
                                    if (subLen > 0)
                                    {
                                        string subString = fileName.Substring(GHConstants.ReplayFileNamePrefix.Length, subLen);
                                        string contStart = GHConstants.ReplayContinuationFileNamePrefix + subString;
                                        string[] files = Directory.GetFiles(dir);
                                        if (files != null)
                                        {
                                            int subFileIdx = 0;
                                            int noSubFiles = 0;
                                            foreach (string file in files)
                                            {
                                                if (!string.IsNullOrWhiteSpace(file))
                                                {
                                                    FileInfo contFI = new FileInfo(file);
                                                    if (contFI != null && !string.IsNullOrWhiteSpace(contFI.Name))
                                                    {
                                                        if (contFI.Name.StartsWith(contStart) && (!isZip || file.EndsWith(usedZipSuffix)) && File.Exists(file))
                                                        {
                                                            noSubFiles++;
                                                        }
                                                    }
                                                }
                                            }
                                            foreach (string file in files)
                                            {
                                                if (UploadDownloadCancelled)
                                                    break;

                                                if (!string.IsNullOrWhiteSpace(file))
                                                {
                                                    FileInfo contFI = new FileInfo(file);
                                                    if (contFI != null && !string.IsNullOrWhiteSpace(contFI.Name))
                                                    {
                                                        if (contFI.Name.StartsWith(contStart) && (!isZip || file.EndsWith(usedZipSuffix)) && File.Exists(file))
                                                        {
                                                            UploadDownloadFileLabel.Text = contFI.Name;
                                                            UploadDownloadStatusLabel.Text = "Continuation " + subFileIdx + " of " + noSubFiles + " for file " + fileIdx + " of " + noValidFiles;
                                                            await GHApp.UploadFromFileAsync(blobContainerClient, prefix, file, _uploadDownloadCts.Token);
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    catch (Exception ex)
                    {
                        await DisplayAlert("Upload File Failure", "GnollHack failed to upload " + noFiles + " replay" + (noFiles != 1 ? "s" : "") + ": " + ex.Message, "OK");
                    }
                }
            }
            else
            {
                GHRecordedGameFile recfile = null;
                string filePath = null;
                if (IsMultiSelect)
                {
                    if (ReplayCollectionView.SelectedItems != null && ReplayCollectionView.SelectedItems.Count == 1)
                    {
                        if (ReplayCollectionView.SelectedItems[0] is GHRecordedGameFile)
                            recfile = ((GHRecordedGameFile)ReplayCollectionView.SelectedItems[0]);
                        if (recfile != null)
                            filePath = recfile.FilePath;
                    }
                }
                else
                {
                    if (ReplayCollectionView.SelectedItem != null)
                    {
                        if (ReplayCollectionView.SelectedItem is GHRecordedGameFile)
                            recfile = ((GHRecordedGameFile)ReplayCollectionView.SelectedItem);
                        if (recfile != null)
                            filePath = recfile.FilePath;
                    }
                }

                if (!string.IsNullOrWhiteSpace(filePath) && recfile != null)
                {
                    try
                    {
                        FileInfo fi = new FileInfo(filePath);
                        string fileName = fi.Name;
                        string dir = fi.DirectoryName;
                        if (!string.IsNullOrWhiteSpace(fileName) && fileName.StartsWith(GHConstants.ReplayFileNamePrefix) && Directory.Exists(dir))
                        {
                            bool isGZip = fileName.Length > GHConstants.ReplayGZipFileNameSuffix.Length && fileName.EndsWith(GHConstants.ReplayGZipFileNameSuffix);
                            bool isNormalZip = fileName.Length > GHConstants.ReplayZipFileNameSuffix.Length && fileName.EndsWith(GHConstants.ReplayZipFileNameSuffix);
                            bool isZip = isGZip || isNormalZip;
                            string usedZipSuffix = isGZip ? GHConstants.ReplayGZipFileNameSuffix : GHConstants.ReplayZipFileNameSuffix;
                            int subLen = fileName.Length - GHConstants.ReplayFileNamePrefix.Length - GHConstants.ReplayFileNameSuffix.Length - (isZip ? usedZipSuffix.Length : 0);
                            if (subLen > 0)
                            {
                                UploadDownloadFileLabel.Text = fileName;
                                UploadDownloadStatusLabel.Text = "Main replay file";
                                await GHApp.UploadFromFileAsync(blobContainerClient, prefix, filePath, _uploadDownloadCts.Token);

                                string[] files = Directory.GetFiles(dir);
                                if (files != null)
                                {
                                    string subString = fileName.Substring(GHConstants.ReplayFileNamePrefix.Length, subLen);
                                    string contStart = GHConstants.ReplayContinuationFileNamePrefix + subString;
                                    int subFileIdx = 0;
                                    int noSubFiles = 0;
                                    foreach (string file in files)
                                    {
                                        if (!string.IsNullOrWhiteSpace(file))
                                        {
                                            FileInfo contFI = new FileInfo(file);
                                            if (contFI != null && !string.IsNullOrWhiteSpace(contFI.Name))
                                            {
                                                if (contFI.Name.StartsWith(contStart) && (!isZip || file.EndsWith(usedZipSuffix)) && File.Exists(file))
                                                {
                                                    noSubFiles++;
                                                }
                                            }
                                        }
                                    }
                                    foreach (string file in files)
                                    {
                                        if (UploadDownloadCancelled)
                                            break;

                                        if (!string.IsNullOrWhiteSpace(file))
                                        {
                                            FileInfo contFI = new FileInfo(file);
                                            if (contFI != null && !string.IsNullOrWhiteSpace(contFI.Name))
                                            {
                                                if (contFI.Name.StartsWith(contStart) && (!isZip || file.EndsWith(usedZipSuffix)) && File.Exists(file))
                                                {
                                                    subFileIdx++;
                                                    UploadDownloadFileLabel.Text = contFI.Name;
                                                    UploadDownloadStatusLabel.Text = "Continuation " + subFileIdx + " of " + noSubFiles;
                                                    await GHApp.UploadFromFileAsync(blobContainerClient, prefix, file, _uploadDownloadCts.Token);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    catch (Exception ex)
                    {
                        await DisplayAlert("Upload File Failure", "GnollHack failed to upload " + filePath + ": " + ex.Message, "OK");
                    }
                }
            }

            if (_uploadDownloadCts != null)
            {
                _uploadDownloadCts.Dispose();
                _uploadDownloadCts = null;
            }
            UploadDownloadFileLabel.Text = "Done.";
            UploadDownloadStatusLabel.Text = "";
            UploadDownloadGrid.IsVisible = false;
            UploadButton.IsEnabled = true;
            PopupCancelButton_Clicked(sender, e);
        }

        private async void DownloadButton_Clicked(object sender, EventArgs e)
        {
            SelectButton.IsEnabled = false;
            GHApp.PlayButtonClickedSound();

            UploadDownloadTitleLabel.Text = "Downloading...";
            UploadDownloadFileLabel.Text = "Initializing...";
            UploadDownloadStatusLabel.Text = "Please wait...";
            UploadDownloadGrid.IsVisible = true;
            UploadDownloadCancelled = false;
            if (_uploadDownloadCts == null)
                _uploadDownloadCts = new CancellationTokenSource();

            BlobServiceClient blobServiceClient = GHApp.GetBlobServiceClient();
            BlobContainerClient blobContainerClient = blobServiceClient.GetBlobContainerClient(GHConstants.AzureBlobStorageReplayContainerName);
            string prefix = _subDirectoryServer;

            if (IsMultiSelect && ReplayCollectionView.SelectedItems != null && ReplayCollectionView.SelectedItems.Count != 1)
            {
                int noFiles = ReplayCollectionView.SelectedItems.Count;
                int fileIdx = 0;
                if (noFiles > 0)
                {
                    try
                    {
                        int noValidFiles = 0;
                        foreach (object selItem in ReplayCollectionView.SelectedItems)
                        {
                            GHRecordedGameFile recfile = null;
                            string filePath = null;
                            if (selItem != null)
                            {
                                if (selItem is GHRecordedGameFile)
                                    recfile = ((GHRecordedGameFile)selItem);
                                if (recfile != null)
                                    filePath = recfile.FilePath;
                            }

                            if (recfile != null && recfile.IsFolder)
                                continue;

                            if (!string.IsNullOrWhiteSpace(filePath) && recfile != null)
                            {
                                string fileName = string.IsNullOrWhiteSpace(prefix) || prefix.Length >= filePath.Length ? filePath : filePath.Substring(prefix.Length);
                                if (!string.IsNullOrWhiteSpace(fileName) && fileName.StartsWith(GHConstants.ReplayFileNamePrefix))
                                {
                                    noValidFiles++;
                                }
                            }
                        }

                        foreach (object selItem in ReplayCollectionView.SelectedItems)
                        {
                            if (UploadDownloadCancelled)
                                break;

                            GHRecordedGameFile recfile = null;
                            string filePath = null;
                            if (selItem != null)
                            {
                                if (selItem is GHRecordedGameFile)
                                    recfile = ((GHRecordedGameFile)selItem);
                                if (recfile != null)
                                    filePath = recfile.FilePath;
                            }

                            if (recfile != null && recfile.IsFolder)
                                continue;

                            if (!string.IsNullOrWhiteSpace(filePath) && recfile != null)
                            {
                                string fileName = string.IsNullOrWhiteSpace(prefix) || prefix.Length >= filePath.Length ? filePath : filePath.Substring(prefix.Length);
                                if (!string.IsNullOrWhiteSpace(fileName) && fileName.StartsWith(GHConstants.ReplayFileNamePrefix))
                                {
                                    fileIdx++;
                                    UploadDownloadFileLabel.Text = fileName;
                                    UploadDownloadStatusLabel.Text = "Main replay file " + fileIdx + " of " + noValidFiles;
                                    await GHApp.DownloadFileAsync(blobContainerClient, prefix, filePath, recfile.FileSize, _uploadDownloadCts.Token);
                                    recfile.Downloaded = true;
                                    if (UploadDownloadCancelled)
                                        break;

                                    List<ContinuationFile> files = recfile.ContinuationFiles;
                                    if (files != null)
                                    {
                                        int subFileIdx = 0;
                                        int noOfSubFiles = files.Count;
                                        foreach (ContinuationFile cfile in files)
                                        {
                                            if (UploadDownloadCancelled)
                                                break;

                                            string filename = cfile.FileName;
                                            if (!string.IsNullOrWhiteSpace(filename))
                                            {
                                                subFileIdx++;
                                                string contFileName = string.IsNullOrWhiteSpace(prefix) || prefix.Length >= filename.Length ? filename : filename.Substring(prefix.Length);
                                                UploadDownloadFileLabel.Text = contFileName;
                                                UploadDownloadStatusLabel.Text = "Continuation " + subFileIdx + " of " + noOfSubFiles + " for file " + fileIdx + " of " + noValidFiles;
                                                await GHApp.DownloadFileAsync(blobContainerClient, prefix, filename, cfile.FileSize, _uploadDownloadCts.Token);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    catch (Exception ex)
                    {
                        await DisplayAlert("Download File Failure", "GnollHack failed to download " + noFiles + " replay" + (noFiles != 1 ? "s" : "") + ": " + ex.Message, "OK");
                    }
                }
            }
            else
            {
                GHRecordedGameFile recfile = null;
                string filePath = null;
                if (IsMultiSelect)
                {
                    if (ReplayCollectionView.SelectedItems != null && ReplayCollectionView.SelectedItems.Count == 1)
                    {
                        if (ReplayCollectionView.SelectedItems[0] is GHRecordedGameFile)
                            recfile = ((GHRecordedGameFile)ReplayCollectionView.SelectedItems[0]);
                        if (recfile != null)
                            filePath = recfile.FilePath;
                    }
                }
                else
                {
                    if (ReplayCollectionView.SelectedItem != null)
                    {
                        if (ReplayCollectionView.SelectedItem is GHRecordedGameFile)
                            recfile = ((GHRecordedGameFile)ReplayCollectionView.SelectedItem);
                        if (recfile != null)
                            filePath = recfile.FilePath;
                    }
                }

                if (!string.IsNullOrWhiteSpace(filePath) && recfile != null)
                {
                    try
                    {
                        string fileName = string.IsNullOrWhiteSpace(prefix) || prefix.Length >= filePath.Length ? filePath : filePath.Substring(prefix.Length);
                        if (!string.IsNullOrWhiteSpace(fileName) && fileName.StartsWith(GHConstants.ReplayFileNamePrefix))
                        {
                            UploadDownloadFileLabel.Text = fileName;
                            UploadDownloadStatusLabel.Text = "Main replay file";
                            await GHApp.DownloadFileAsync(blobContainerClient, prefix, filePath, recfile.FileSize, _uploadDownloadCts.Token);
                            List<ContinuationFile> files = recfile.ContinuationFiles;
                            if (files != null)
                            {
                                int subFileIdx = 0;
                                int noOfSubFiles = files.Count;
                                foreach (ContinuationFile cfile in files)
                                {
                                    if (UploadDownloadCancelled)
                                        break;

                                    string filename = cfile.FileName;
                                    if (!string.IsNullOrWhiteSpace(filename))
                                    {
                                        subFileIdx++;
                                        string contFileName = string.IsNullOrWhiteSpace(prefix) || prefix.Length >= filename.Length ? filename : filename.Substring(prefix.Length);
                                        UploadDownloadFileLabel.Text = contFileName;
                                        UploadDownloadStatusLabel.Text = "Continuation " + subFileIdx + " of " + noOfSubFiles;
                                        await GHApp.DownloadFileAsync(blobContainerClient, prefix, filename, cfile.FileSize, _uploadDownloadCts.Token);
                                    }
                                }
                            }
                        }
                    }
                    catch (Exception ex)
                    {
                        await DisplayAlert("Download File Failure", "GnollHack failed to download " + filePath + ": " + ex.Message, "OK");
                    }
                }
            }

            if (_uploadDownloadCts != null)
            {
                _uploadDownloadCts.Dispose();
                _uploadDownloadCts = null;
            }
            UploadDownloadFileLabel.Text = "Done.";
            UploadDownloadStatusLabel.Text = "";
            UploadDownloadGrid.IsVisible = false;
            UploadButton.IsEnabled = true;
            PopupCancelButton_Clicked(sender, e);
            UpdateRecordings();
        }

        private async void ServerButton_Clicked(object sender, EventArgs e)
        {
            BlobServiceClient client = GHApp.GetBlobServiceClient();
            if (client == null)
                return;
            BlobContainerClient containerClient = client.GetBlobContainerClient(GHConstants.AzureBlobStorageReplayContainerName);
            if (containerClient == null)
                return;
            await GHApp.ListBlobsHierarchicalListing(containerClient, null, null);
        }

        private void PopupCancelButton_Clicked(object sender, EventArgs e)
        {
            PopupGrid.IsVisible = false;
            MoreButton.IsEnabled = true;
        }

        private void MoreButton_Clicked(object sender, EventArgs e)
        {
            MoreButton.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            PopupGrid.IsVisible = true;
        }

        private void UploadDownloadCancelButton_Clicked(object sender, EventArgs e)
        {
            _uploadDownloadCts.Cancel();
            UploadDownloadCancelled = true;
            UploadDownloadGrid.IsVisible = false;
        }

        private void ServerSwitch_Toggled(object sender, ToggledEventArgs e)
        {
            UpdateLocalOrServerRecordings(e.Value);
            UpdateButtons();
        }

        private void FolderPicker_SelectedIndexChanged(object sender, EventArgs e)
        {
            UpdateLocalOrServerRecordings(IsCloud);
            UpdateButtons();
        }
    }

    public class FileDateComparer : IComparer<string>
    {
        public int Compare(string s1, string s2)
        {
            try
            {
                if (!File.Exists(s1))
                    return -1;
                if (!File.Exists(s2))
                    return 1;
                FileInfo fileInfo1 = new FileInfo(s1);
                FileInfo fileInfo2 = new FileInfo(s2);
                return -1 * fileInfo1.CreationTime.CompareTo(fileInfo2.CreationTime);
            }
            catch
            { 
                return -1; 
            }
        }
    }
    public class RecordedGameFileComparer : IComparer<GHRecordedGameFile>
    {
        public int Compare(GHRecordedGameFile s1, GHRecordedGameFile s2)
        {
            try
            {
                if(s1 == null && s2 == null)
                    return 0;
                if (s1 == null)
                    return -1;
                if (s2 == null)
                    return 1;

                string comp1str = (string.IsNullOrWhiteSpace(s1.FileName) ? "" : s1.FileName) + (string.IsNullOrWhiteSpace(s1.Extension) ? "" : s1.Extension);
                string comp2str = (string.IsNullOrWhiteSpace(s2.FileName) ? "" : s2.FileName) + (string.IsNullOrWhiteSpace(s2.Extension) ? "" : s2.Extension);
                if (s1.IsFolder && s2.IsFolder)
                {
                    if(s1.FilePath == null && s2.FilePath == null)
                        return 0;
                    if (s1.FilePath == null)
                        return -1;
                    if (s2.FilePath == null)
                        return 1;
                    return string.Compare(comp1str, comp2str);
                }
                if (s1.IsFolder)
                    return -1;
                if (s2.IsFolder)
                    return 1;
                if (s1.FilePath == null && s2.FilePath == null)
                    return 0;
                if (s1.FilePath == null)
                    return -1;
                if (s2.FilePath == null)
                    return 1;
                int res = -1 * DateTime.Compare(s1.LastWriteTime, s2.LastWriteTime);
                if (res != 0)
                    return res;
                else
                    return string.Compare(comp1str, comp2str);
            }
            catch
            {
                return -1;
            }
        }
    }
}