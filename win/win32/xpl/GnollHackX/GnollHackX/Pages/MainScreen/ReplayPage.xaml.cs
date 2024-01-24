using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.IO.Compression;
using System.Linq;
using System.Runtime.InteropServices.ComTypes;
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

            MainListView.ItemsSource = null;
            MainListView.SelectedItem = null;
            ShareButton.IsEnabled = false;
            SelectButton.IsEnabled = false;
            DeleteButton.IsEnabled = false;
            ShareButton.TextColor = GHColors.Gray;
            SelectButton.TextColor = GHColors.Gray;
            DeleteButton.TextColor = GHColors.Gray;

            string dirPath = Path.Combine(GHApp.GHPath, _directory);
            int i = 0;
            long totalBytes = 0L;
            if (Directory.Exists(dirPath))
            {
                string[] files = Directory.GetFiles(dirPath);
                if (files != null)
                {
                    ObservableCollection<GHRecordedGameFile> gHRecordedGameFiles = new ObservableCollection<GHRecordedGameFile>();
                    foreach (string file in files)
                    {
                        if (file != null && File.Exists(file))
                        {
                            FileInfo fi = new FileInfo(file);
                            if (!string.IsNullOrWhiteSpace(fi.Name) && fi.Name.StartsWith(GHConstants.ReplayFileNamePrefix))
                            {
                                i++;
                                long contLen = 0L;
                                int middleLen = fi.Name.Length - GHConstants.ReplayFileNamePrefix.Length - GHConstants.ReplayFileNameSuffix.Length;
                                if(middleLen > 0)
                                {
                                    string middleStr = fi.Name.Substring(GHConstants.ReplayFileNamePrefix.Length, middleLen);
                                    foreach (string contFile in files)
                                    {
                                        if (contFile != null && File.Exists(contFile))
                                        {
                                            FileInfo contFI = new FileInfo(contFile);
                                            if (!string.IsNullOrWhiteSpace(contFI.Name) && contFI.Name.StartsWith(GHConstants.ReplayContinuationFileNamePrefix + middleStr))
                                            {
                                                contLen += contFI.Length;
                                            }
                                        }
                                    }
                                }
                                gHRecordedGameFiles.Add(new GHRecordedGameFile(i, Path.Combine(dirPath, file), fi.Name, fi.Extension, fi.Length + contLen, fi.CreationTime, fi.LastWriteTime));
                                totalBytes += fi.Length + contLen;
                            }
                        }
                    }
                    MainListView.ItemsSource = gHRecordedGameFiles;
                }
            }
            RecordingsLabel.Text = i.ToString() + " recording" + (i == 1 ? "" : "s") + ", " + (totalBytes / 1024) + " kB";
        }

        private async void ShareButton_Clicked(object sender, EventArgs e)
        {
            ShareButton.IsEnabled = false;
            GHApp.PlayButtonClickedSound();

            GHRecordedGameFile recfile = null;
            string filePath = null;
            if (MainListView.SelectedItem != null)
            {
                if (MainListView.SelectedItem is GHRecordedGameFile)
                    recfile = ((GHRecordedGameFile)MainListView.SelectedItem);
                if (recfile != null)
                    filePath = recfile.FilePath;
            }

            if (filePath != null && recfile != null)
            {
                try
                {
                    /* Create zip of al relevant files */
                    string zipFile = Path.Combine(GHApp.GHPath, GHConstants.ReplayDirectory, "Replays" + GHConstants.ReplaySharedZipFileNameSuffix);
                    if(File.Exists(zipFile))
                        File.Delete(zipFile);

                    using (ZipArchive archive = ZipFile.Open(zipFile, ZipArchiveMode.Create))
                    {
                        archive.CreateEntryFromFile(filePath, Path.GetFileName(filePath));

                        FileInfo fi = new FileInfo(filePath);
                        string dir = fi.DirectoryName;
                        string fileName = fi.Name;
                        if (fileName != null && fileName.StartsWith(GHConstants.ReplayFileNamePrefix))
                        {
                            bool isGZip = fileName.Length > GHConstants.ReplayGZipFileNameSuffix.Length && fileName.EndsWith(GHConstants.ReplayGZipFileNameSuffix);
                            bool isNormalZip = fileName.Length > GHConstants.ReplayZipFileNameSuffix.Length && fileName.EndsWith(GHConstants.ReplayZipFileNameSuffix);
                            bool isZip = isGZip || isNormalZip;
                            string usedZipSuffix = isGZip ? GHConstants.ReplayGZipFileNameSuffix : GHConstants.ReplayZipFileNameSuffix;
                            int subLen = fileName.Length - GHConstants.ReplayFileNamePrefix.Length - GHConstants.ReplayFileNameSuffix.Length - (isZip ? usedZipSuffix.Length : 0);
                            if (subLen > 0 && Directory.Exists(dir))
                            {
                                string[] files = Directory.GetFiles(dir);
                                if (files != null)
                                {
                                    foreach (string file in files)
                                    {
                                        if (file != null)
                                        {
                                            string contStart = GHConstants.ReplayContinuationFileNamePrefix + fileName.Substring(GHConstants.ReplayFileNamePrefix.Length, subLen);
                                            FileInfo contFI = new FileInfo(file);
                                            if(contFI != null && contFI.Name != null)
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
                    await Share.RequestAsync(new ShareFileRequest
                    {
                        Title = "Sharing " + recfile.FileName,
                        File = new ShareFile(zipFile)
                    });
                }
                catch (Exception ex)
                {
                    await DisplayAlert("Share File Failure", "GnollHack failed to share " + filePath + ": " + ex.Message, "OK");
                }
            }
            ShareButton.IsEnabled = true;
        }

        private async void SelectButton_Clicked(object sender, EventArgs e)
        {
            SelectButton.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            string filePath = null;
            if (MainListView.SelectedItem != null)
            {
                if(MainListView.SelectedItem is GHRecordedGameFile)
                    filePath = ((GHRecordedGameFile)MainListView.SelectedItem).FilePath;
            }

            if (filePath != null)
            {
                var gamePage = new GamePage(_mainPage);
                GHApp.CurrentGamePage = gamePage;
                await App.Current.MainPage.Navigation.PushModalAsync(gamePage);
                gamePage.StartReplay(filePath);
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
                MainListView.Margin = UIUtils.GetMiddleElementMarginWithBorder(bkgView.BorderStyle, width, height);
                HeaderLabel.Margin = UIUtils.GetHeaderMarginWithBorder(bkgView.BorderStyle, width, height);
                BottomLayout.Margin = UIUtils.GetFooterMarginWithBorder(bkgView.BorderStyle, width, height);
            }
        }

        private void MainListView_ItemSelected(object sender, SelectedItemChangedEventArgs e)
        {
            ShareButton.IsEnabled = true;
            SelectButton.IsEnabled = true;
            DeleteButton.IsEnabled = true;
            ShareButton.TextColor = GHColors.White;
            SelectButton.TextColor = GHColors.White;
            DeleteButton.TextColor = GHColors.White;
        }

        private async void DeleteButton_Clicked(object sender, EventArgs e)
        {
            DeleteButton.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            GHRecordedGameFile recfile = null;

            string filePath = null;

            if (MainListView.SelectedItem != null)
            {
                if (MainListView.SelectedItem is GHRecordedGameFile)
                    recfile = ((GHRecordedGameFile)MainListView.SelectedItem);
                if (recfile != null)
                    filePath = recfile.FilePath;
            }

            if (filePath != null && recfile != null)
            {
                if(await DisplayAlert("Confirm Deletion", "Are you sure to delete the game recording " + recfile.FileName + "?", "Yes", "No"))
                {
                    try
                    {
                        File.Delete(filePath);

                        /* Delete also continuation files */
                        FileInfo fi = new FileInfo(filePath);
                        string dir = fi.DirectoryName;
                        string fileName = fi.Name;
                        if (fileName != null && fileName.StartsWith(GHConstants.ReplayFileNamePrefix))
                        {
                            bool isGZip = fileName.Length > GHConstants.ReplayGZipFileNameSuffix.Length && fileName.EndsWith(GHConstants.ReplayGZipFileNameSuffix);
                            bool isNormalZip = fileName.Length > GHConstants.ReplayZipFileNameSuffix.Length && fileName.EndsWith(GHConstants.ReplayZipFileNameSuffix);
                            bool isZip = isGZip || isNormalZip;
                            string usedZipSuffix = isGZip ? GHConstants.ReplayGZipFileNameSuffix : GHConstants.ReplayZipFileNameSuffix;
                            int subLen = fileName.Length - GHConstants.ReplayFileNamePrefix.Length - GHConstants.ReplayFileNameSuffix.Length - (isZip ? usedZipSuffix.Length : 0);
                            if (subLen > 0 && Directory.Exists(dir))
                            {
                                string[] files = Directory.GetFiles(dir);
                                if (files != null)
                                {
                                    foreach (string file in files)
                                    {
                                        if (file != null)
                                        {
                                            string contStart = GHConstants.ReplayContinuationFileNamePrefix + fileName.Substring(GHConstants.ReplayFileNamePrefix.Length, subLen);
                                            FileInfo contFI = new FileInfo(file);
                                            if (contFI != null && contFI.Name != null)
                                            {
                                                if (contFI.Name.StartsWith(contStart) && (!isZip || file.EndsWith(usedZipSuffix)) && File.Exists(file))
                                                {
                                                    File.Delete(file);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        UpdateRecordings();
                    }
                    catch (Exception ex)
                    {
                        await DisplayAlert("Deletion Failed", "GnollHack failed to delete game recording " + filePath + ": " + ex.Message, "OK");
                    }
                }
            }
            DeleteButton.IsEnabled = true;
        }
    }
}