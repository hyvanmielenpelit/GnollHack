using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Data.SqlTypes;
using System.Diagnostics;
using System.IO;
using System.IO.Compression;
using System.Linq;
using System.Runtime.InteropServices.ComTypes;
using System.Runtime.Serialization.Json;
using System.Text;
using System.Threading.Tasks;

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
        string _directory = null;

        public bool IsMultiSelect { get { return ReplayCollectionView.SelectionMode == SelectionMode.Multiple; } }

		public ReplayPage(string dir, MainPage mainPage)
		{
			InitializeComponent();
#if GNH_MAUI
            On<iOS>().SetUseSafeArea(true);
#else
            On<Xamarin.Forms.PlatformConfiguration.iOS>().SetUseSafeArea(true);
#endif

            _directory = dir;
            _mainPage = mainPage;
            ShareButton.IsEnabled = false;
            SelectButton.IsEnabled = false;
            
            UpdateRecordings();
        }

        private void UpdateRecordings()
        {
            if (string.IsNullOrWhiteSpace(_directory))
                return;

            ReplayCollectionView.SelectedItem = null;
            ReplayCollectionView.ItemsSource = null;
            ShareButton.IsEnabled = false;
            SelectButton.IsEnabled = false;
            ShareButton.TextColor = GHColors.Gray;
            SelectButton.TextColor = GHColors.Gray;

            string dirPath = Path.Combine(GHApp.GHPath, _directory);
            int i = 0;
            long totalBytes = 0L;
            if (Directory.Exists(dirPath))
            {
                string[] files = Directory.GetFiles(dirPath);
                if (files != null)
                {
                    Array.Sort(files, new FileDateComparer());
                    ObservableCollection<GHRecordedGameFile> gHRecordedGameFiles = new ObservableCollection<GHRecordedGameFile>();
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
                                                gHRecordedGameFiles.Add(new GHRecordedGameFile(i, Path.Combine(dirPath, zipFile), zipFI.Name, zipFI.Extension, zipFI.Length + 0, 1, zipFI.CreationTime, zipFI.LastWriteTime));
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
                                        gHRecordedGameFiles.Add(new GHRecordedGameFile(i, Path.Combine(dirPath, file), fi.Name, fi.Extension, fi.Length + contLen, 1 + noOfContFiles, fi.CreationTime, fi.LastWriteTime));
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
                    ReplayCollectionView.ItemsSource = gHRecordedGameFiles;
                }
            }
            RecordingsLabel.Text = i.ToString() + " replay" + (i == 1 ? "" : "s") + ", " + (totalBytes < 1024 * 1024 ? string.Format("{0} kB", totalBytes / 1024) : string.Format("{0:0.0} MB" , (double)totalBytes / (1024 * 1024)));
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
        }

        private async void SelectButton_Clicked(object sender, EventArgs e)
        {
            if(IsMultiSelect)
            {
                DeleteButton_Clicked(sender, e);
                return;
            }

            SelectButton.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            string filePath = null;
            if (ReplayCollectionView.SelectedItem != null)
            {
                if(ReplayCollectionView.SelectedItem is GHRecordedGameFile)
                    filePath = ((GHRecordedGameFile)ReplayCollectionView.SelectedItem).FilePath;
            }

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
            if(ReplayCollectionView.SelectionMode == SelectionMode.Single)
            {
                if (ReplayCollectionView.SelectedItem != null)
                {
                    ShareButton.IsEnabled = true;
                    SelectButton.IsEnabled = true;
                    ShareButton.TextColor = GHColors.White;
                    SelectButton.TextColor = GHColors.BrighterGreen;
                }
                else
                {
                    ShareButton.IsEnabled = false;
                    SelectButton.IsEnabled = false;
                    ShareButton.TextColor = GHColors.Gray;
                    SelectButton.TextColor = GHColors.Gray;
                }
            }
            else if (ReplayCollectionView.SelectionMode == SelectionMode.Multiple)
            {
                if (ReplayCollectionView.SelectedItems.Count > 0)
                {
                    ShareButton.IsEnabled = true;
                    SelectButton.IsEnabled = true;
                    ShareButton.TextColor = GHColors.White;
                    SelectButton.TextColor = GHColors.Red;
                }
                else
                {
                    ShareButton.IsEnabled = false;
                    SelectButton.IsEnabled = false;
                    ShareButton.TextColor = GHColors.Gray;
                    SelectButton.TextColor = GHColors.Gray;
                }
            }
        }

        private async void DeleteButton_Clicked(object sender, EventArgs e)
        {
            SelectButton.IsEnabled = false;
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

            SelectButton.IsEnabled = true;
        }

        private void MultiButton_Clicked(object sender, EventArgs e)
        {
            if(IsMultiSelect)
            {
                ReplayCollectionView.SelectedItems.Clear();
                ReplayCollectionView.SelectionMode = SelectionMode.Single;
                MultiButton.Text = "Multiple";
                SelectButton.Text = "Play";
                SelectButton.TextColor = GHColors.BrighterGreen;
                bkgView.BorderStyle = BorderStyles.Simple;
            }
            else
            {
                ReplayCollectionView.SelectedItem = null;
                ReplayCollectionView.SelectionMode = SelectionMode.Multiple;
                MultiButton.Text = "Single";
                SelectButton.Text = "Delete";
                SelectButton.TextColor = GHColors.Red;
                bkgView.BorderStyle = BorderStyles.SimpleAlternative;
            }
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
}