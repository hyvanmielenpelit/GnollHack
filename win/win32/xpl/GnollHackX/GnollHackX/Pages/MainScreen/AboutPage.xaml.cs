using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Compression;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Xamarin.Essentials;
using Xamarin.Forms;
using Xamarin.Forms.PlatformConfiguration.iOSSpecific;
using Xamarin.Forms.Xaml;
using static System.Net.WebRequestMethods;

namespace GnollHackX.Pages.MainScreen
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class AboutPage : ContentPage
    {
        public AboutPage()
        {
            InitializeComponent();
            On<Xamarin.Forms.PlatformConfiguration.iOS>().SetUseSafeArea(true);
        }

        private async void btnCreditsX_Clicked(object sender, EventArgs e)
        {
            AboutGrid.IsEnabled = false;
            App.PlayButtonClickedSound();
            string fulltargetpath = Path.Combine(App.GHPath, "xcredits");
            var displFilePage = new DisplayFilePage(fulltargetpath, "Cross-Platform Credits", 80);
            string errormsg = "";
            if (!displFilePage.ReadFile(out errormsg))
            {
                await DisplayAlert("Error Opening File", "GnollHack cannot open the xcredits file.", "OK");
            }
            else
            {
                await App.Current.MainPage.Navigation.PushModalAsync(displFilePage);
            }
            AboutGrid.IsEnabled = true;
        }

        private async void btnCreditsW_Clicked(object sender, EventArgs e)
        {
            AboutGrid.IsEnabled = false;
            App.PlayButtonClickedSound();
            string fulltargetpath = Path.Combine(App.GHPath, "credits");
            var displFilePage = new DisplayFilePage(fulltargetpath, "Credits", 77);
            string errormsg = "";
            if (!displFilePage.ReadFile(out errormsg))
            {
                await DisplayAlert("Error Opening File", "GnollHack cannot open the credits file.", "OK");
            }
            else
            {
                await App.Current.MainPage.Navigation.PushModalAsync(displFilePage);
            }
            AboutGrid.IsEnabled = true;
        }

        private async void btnLicense_Clicked(object sender, EventArgs e)
        {
            AboutGrid.IsEnabled = false;
            App.PlayButtonClickedSound();
            string fulltargetpath = Path.Combine(App.GHPath, "license");
            var displFilePage = new DisplayFilePage(fulltargetpath, "License", 78);
            string errormsg = "";
            if (!displFilePage.ReadFile(out errormsg))
            {
                await DisplayAlert("Error Opening File", "GnollHack cannot open the license file.", "OK");
            }
            else
            {
                await App.Current.MainPage.Navigation.PushModalAsync(displFilePage);
            }
            AboutGrid.IsEnabled = true;
        }
        private async void btnGitHub_Clicked(object sender, EventArgs e)
        {
            AboutGrid.IsEnabled = false;
            App.PlayButtonClickedSound();
            await OpenBrowser(new Uri(GHConstants.GnollHackGitHubPage));
            AboutGrid.IsEnabled = true;
        }

        public async Task OpenBrowser(Uri uri)
        {
            try
            {
                await Browser.OpenAsync(uri, BrowserLaunchMode.SystemPreferred);
            }
            catch (Exception ex)
            {
                await DisplayAlert("Cannot Open Web Page", "GnollHack cannot open the webpage at " + uri.OriginalString + ". Error: " + ex.Message, "OK");
            }
        }

        private async void btnWebPage_Clicked(object sender, EventArgs e)
        {
            AboutGrid.IsEnabled = false;
            App.PlayButtonClickedSound();
            await OpenBrowser(new Uri(GHConstants.GnollHackWebPage));
            AboutGrid.IsEnabled = true;
        }

        private async void btnWiki_Clicked(object sender, EventArgs e)
        {
            AboutGrid.IsEnabled = false;
            App.PlayButtonClickedSound();
            await OpenBrowser(new Uri(GHConstants.GnollHackWikiPage));
            AboutGrid.IsEnabled = true;
        }

        private async void btnDowngrade_Clicked(object sender, EventArgs e)
        {
            AboutGrid.IsEnabled = false;
            App.PlayButtonClickedSound();
            await OpenBrowser(new Uri(App.IsAndroid ? GHConstants.GnollHackAndroidDowngradePage : App.IsiOS ? GHConstants.GnollHackiOSDowngradePage :  GHConstants.GnollHackGeneralDowngradePage));
            AboutGrid.IsEnabled = true;
        }

        private async void btnSponsor_Clicked(object sender, EventArgs e)
        {
            AboutGrid.IsEnabled = false;
            App.PlayButtonClickedSound();
            await OpenBrowser(new Uri(GHConstants.GnollHackSponsorPage));
            AboutGrid.IsEnabled = true;
        }

        //private async void btnSourceCode_Clicked(object sender, EventArgs e)
        //{
        //    AboutGrid.IsEnabled = false;
        //    App.PlayButtonClickedSound();
        //    SecretsFile sourcesf = null;
        //    foreach (SecretsFile f in App.CurrentSecrets.files)
        //    {
        //        if(f.type == "source_zip")
        //        {
        //            sourcesf = f;
        //            break;
        //        }
        //    }

        //    if(sourcesf == null)
        //    {
        //        await DisplayAlert("Source File Location Not Found", "GnollHack was unable to find instructions to locate the source files archive.", "OK");
        //        return;
        //    }

        //    string ghdir = App.GnollHackService.GetGnollHackPath();
        //    string targetpath = Path.Combine(ghdir, sourcesf.target_directory, sourcesf.name);

        //    if(!File.Exists(targetpath))
        //    {
        //        await DisplayAlert("Source Files Not Found", "The source files archive was not found.", "OK");
        //        return;
        //    }

        //    byte[] data;
        //    try
        //    {
        //        data = File.ReadAllBytes(targetpath);
        //    }
        //    catch(Exception ex)
        //    {
        //        await DisplayAlert("Error Occurred", "An error occurred while reading source files: " + ex.Message, "OK");
        //        return;
        //    }

        //    try
        //    {
        //        App.PlatformService.SaveFileToDownloads(data, sourcesf.name);
        //    }
        //    catch (Exception ex)
        //    {
        //        await DisplayAlert("Error Occurred", "An error occurred while saving source files to downloads folder: " + ex.Message, "OK");
        //        return;
        //    }
        //    await DisplayAlert("Source Files in Downloads", "Source files have been successfully saved to the Downloads folder in a zip format. The file is named \'" + sourcesf.name + "\'.", "OK");
        //    AboutGrid.IsEnabled = true;
        //}

        private async void Button_Clicked(object sender, EventArgs e)
        {
            AboutGrid.IsEnabled = false;
            App.PlayButtonClickedSound();
            await App.Current.MainPage.Navigation.PopModalAsync();
        }

        private bool _backPressed = false;
        private async Task<bool> BackButtonPressed(object sender, EventArgs e)
        {
            if (!_backPressed)
            {
                _backPressed = true;
                AboutGrid.IsEnabled = false;
                await App.Current.MainPage.Navigation.PopModalAsync();
            }
            return false;
        }

        private void ContentPage_Appearing(object sender, EventArgs e)
        {
            App.BackButtonPressed += BackButtonPressed;
            AboutGrid.IsEnabled = true;
        }
        private void ContentPage_Disappearing(object sender, EventArgs e)
        {
            App.BackButtonPressed -= BackButtonPressed;
        }

        private async void btnCrashReport_Clicked(object sender, EventArgs e)
        {
            AboutGrid.IsEnabled = false;
            App.PlayButtonClickedSound();
            bool answer = await DisplayAlert("Send Crash Report?", "This will create a zip archive of the files in your game directory and ask it to be shared further.", "Yes", "No");
            if (answer)
            {
                await CheckAndRequestWritePermission();
                await CheckAndRequestReadPermission();
                string archive_file = "";
                try
                {
                    archive_file = App.CreateGameZipArchive();
                }
                catch(Exception ex)
                {
                    await DisplayAlert("Archive Creation Failure", "GnollHack failed to create a crash report archive: " + ex.Message, "OK");
                    AboutGrid.IsEnabled = true;
                    return;
                }
                try
                {
                    if (archive_file != "")
                        ShareFile(archive_file, "GnollHack Crash Report");

                }
                catch (Exception ex)
                {
                    await DisplayAlert("Share File Failure", "GnollHack failed to share a crash report archive: " + ex.Message, "OK");
                    AboutGrid.IsEnabled = true;
                    return;
                }
            }
            AboutGrid.IsEnabled = true;
        }


        private async void ShareFile(string filename, string title)
        {
            if(!System.IO.File.Exists(filename))
            {
                await DisplayAlert("File Sharing Failure", "GnollHack cannot find file \'" + filename + "\'" , "OK");
                return;
            }
            await Share.RequestAsync(new ShareFileRequest
            {
                Title = title,
                File = new ShareFile(filename)
            });
        }

        public async Task<PermissionStatus> CheckAndRequestWritePermission()
        {
            var status = await Permissions.CheckStatusAsync<Permissions.StorageWrite>();

            if (status == PermissionStatus.Granted)
                return status;

            if (status == PermissionStatus.Denied && DeviceInfo.Platform == DevicePlatform.iOS)
            {
                // Prompt the user to turn on in settings
                // On iOS once a permission has been denied it may not be requested again from the application
                await DisplayAlert("Permission Needed", "GnollHack needs the file write permission to create a zip file. Please turn it on in Settings.", "OK");
                return status;
            }

            if (Permissions.ShouldShowRationale<Permissions.StorageWrite>())
            {
                await DisplayAlert("Permission Needed", "GnollHack needs the file write permission to create a zip file.", "OK");
            }

            status = await Permissions.RequestAsync<Permissions.StorageWrite>();

            return status;
        }

        public async Task<PermissionStatus> CheckAndRequestReadPermission()
        {
            var status = await Permissions.CheckStatusAsync<Permissions.StorageRead>();

            if (status == PermissionStatus.Granted)
                return status;

            if (status == PermissionStatus.Denied && DeviceInfo.Platform == DevicePlatform.iOS)
            {
                // Prompt the user to turn on in settings
                // On iOS once a permission has been denied it may not be requested again from the application
                await DisplayAlert("Permission Needed", "GnollHack needs the file read permission to work with a zip file. Please turn it on in Settings.", "OK");
                return status;
            }

            if (Permissions.ShouldShowRationale<Permissions.StorageRead>())
            {
                await DisplayAlert("Permission Needed", "GnollHack needs the file read permission to work with a zip file.", "OK");
            }

            status = await Permissions.RequestAsync<Permissions.StorageRead>();

            return status;
        }

        private async void btnDumplogs_Clicked(object sender, EventArgs e)
        {
            AboutGrid.IsEnabled = false;
            App.PlayButtonClickedSound();
            await CheckAndRequestWritePermission();
            await CheckAndRequestReadPermission();
            string archive_file = "";
            try
            {
                archive_file = App.CreateDumplogZipArchive();
            }
            catch (Exception ex)
            {
                await DisplayAlert("Archive Creation Failure", "GnollHack failed to create a dumplog archive: " + ex.Message, "OK");
                AboutGrid.IsEnabled = true;
                return;
            }
            try
            {
                if (archive_file != "")
                    ShareFile(archive_file, "GnollHack Dumplogs");
            }
            catch (Exception ex)
            {
                await DisplayAlert("Share File Failure", "GnollHack failed to share a dumplog archive: " + ex.Message, "OK");
                AboutGrid.IsEnabled = true;
                return;
            }
            AboutGrid.IsEnabled = true;
        }

        private async void btnViewPanicLog_Clicked(object sender, EventArgs e)
        {
            AboutGrid.IsEnabled = false;
            App.PlayButtonClickedSound();
            string fulltargetpath = Path.Combine(App.GHPath, "paniclog");
            var displFilePage = new DisplayFilePage(fulltargetpath, "Panic Log");
            string errormsg = "";
            if (!System.IO.File.Exists(fulltargetpath))
            {
                await DisplayAlert("No Panic Log", "Panic Log does not exist.", "OK");
            }
            else if (!displFilePage.ReadFile(out errormsg))
            {
                await DisplayAlert("Error Opening File", "GnollHack cannot open the paniclog file.", "OK");
            }
            else
            {
                await App.Current.MainPage.Navigation.PushModalAsync(displFilePage);
            }
            AboutGrid.IsEnabled = true;
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

                lblHeader.Margin = ClientUtils.GetHeaderMarginWithBorder(bkgView.BorderStyle, width, height);
                CloseButton.Margin = ClientUtils.GetFooterMarginWithBorder(bkgView.BorderStyle, width, height);
            }
        }

        private async void btnVersion_Clicked(object sender, EventArgs e)
        {
            AboutGrid.IsEnabled = false;
            App.PlayButtonClickedSound();
            var verPage = new VersionPage(null);
            await App.Current.MainPage.Navigation.PushModalAsync(verPage);
            AboutGrid.IsEnabled = true;

        }

        private async void btnImportSavedGames_Clicked(object sender, EventArgs e)
        {
            AboutGrid.IsEnabled = false;
            App.PlayButtonClickedSound();
            await CheckAndRequestWritePermission();
            await CheckAndRequestReadPermission();
            try
            {
                FileResult file = await FilePicker.PickAsync();

                if (file != null)
                {
                    using (Stream s = await file.OpenReadAsync())
                    {
                        if (s != null)
                        {
                            string gnhpath = App.GHPath;
                            if (file.FileName.EndsWith("zip", StringComparison.OrdinalIgnoreCase))
                            {
                                string savedirpath = Path.Combine(gnhpath, "save");
                                App.CheckCreateDirectory(savedirpath);

                                string tempdirpath = Path.Combine(gnhpath, "save", "temp");
                                if (Directory.Exists(tempdirpath))
                                    Directory.Delete(tempdirpath, true);
                                App.CheckCreateDirectory(tempdirpath);
    
                                string temp2dirpath = Path.Combine(gnhpath, "save", "zip");
                                if (Directory.Exists(temp2dirpath))
                                    Directory.Delete(temp2dirpath, true);
                                App.CheckCreateDirectory(temp2dirpath);

                                string ziptargetfilename = file.FileName;
                                string fulltargetpath = Path.Combine(tempdirpath, ziptargetfilename);
                                string fulltargetpath2 = Path.Combine(savedirpath, ziptargetfilename);
                                if(System.IO.File.Exists(fulltargetpath2))
                                    System.IO.File.Delete(fulltargetpath2);

                                using (Stream t = System.IO.File.Open(fulltargetpath, FileMode.Create))
                                {
                                    s.CopyTo(t);
                                }
                                using (ZipArchive ziparch = ZipFile.OpenRead(fulltargetpath))
                                {
                                    ziparch.ExtractToDirectory(temp2dirpath);
                                }
                                int nextracted = 0;
                                string[] extractedfiles = Directory.GetFiles(temp2dirpath);
                                if(extractedfiles != null)
                                {
                                    foreach(string filestr in extractedfiles)
                                    {
                                        if(System.IO.File.Exists(filestr))
                                        {
                                            string out_str = "";
                                            if(App.GnollHackService.ValidateSaveFile(filestr, out out_str))
                                            {
                                                FileInfo fileInfo = new FileInfo(filestr);
                                                string finalname = Path.Combine(savedirpath, fileInfo.Name + ".i");
                                                if (System.IO.File.Exists(finalname))
                                                    System.IO.File.Delete(finalname);
                                                System.IO.File.Move(filestr, finalname);
                                                nextracted++;
                                            }
                                            else
                                            {
                                                await DisplayAlert("Invalid Save Game in Zip", "Saved game \'" + filestr + "\' is invalid: " + out_str, "OK");
                                            }
                                        }
                                    }
                                }
                                Directory.Delete(tempdirpath, true);
                                Directory.Delete(temp2dirpath, true);
                                if(extractedfiles.Length == 0)
                                    await DisplayAlert("No Files in Zip", "There are no files in \'" + ziptargetfilename + "\'.", "OK");
                                else if (nextracted > 0)
                                    await DisplayAlert("Games Saved from Zip", "Saved games from \'" + ziptargetfilename + "\' have been saved to the save directory as non-scoring imported saved games.", "OK");
                                else
                                    await DisplayAlert("No Games Saved in Zip", "No saved games from \'" + ziptargetfilename + "\' were saved to the save directory.", "OK");
                            }
                            else
                            {
                                string out_str = "";
                                if (App.GnollHackService.ValidateSaveFile(file.FullPath, out out_str))
                                {
                                    string targetfilename = file.FileName + ".i";
                                    string savedirpath = Path.Combine(gnhpath, "save");
                                    App.CheckCreateDirectory(savedirpath);

                                    string fulltargetpath = Path.Combine(savedirpath, targetfilename);
                                    if (System.IO.File.Exists(fulltargetpath))
                                        System.IO.File.Delete(fulltargetpath);
                                    using (Stream t = System.IO.File.Open(fulltargetpath, FileMode.Create))
                                    {
                                        s.CopyTo(t);
                                    }
                                    await DisplayAlert("Game Saved", "Saved game \'" + file.FileName + "\' has been saved to the save directory as a non-scoring imported saved game.", "OK");
                                }
                                else
                                {
                                    await DisplayAlert("Invalid Saved Game", "Saved game \'" + file.FullPath + "\' is invalid: " + out_str, "OK");
                                }
                            }
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                await DisplayAlert("Error", "An error occurred while trying to import a saved game: " + ex.Message, "OK");
            }
            AboutGrid.IsEnabled = true;
        }

        private async void btnSavedGames_Clicked(object sender, EventArgs e)
        {
            AboutGrid.IsEnabled = false;
            App.PlayButtonClickedSound();
            await CheckAndRequestWritePermission();
            await CheckAndRequestReadPermission();
            string archive_file = "";
            try
            {
                archive_file = App.CreateSavedGamesZipArchive();
            }
            catch (Exception ex)
            {
                await DisplayAlert("Archive Creation Failure", "GnollHack failed to create a saved games archive: " + ex.Message, "OK");
                AboutGrid.IsEnabled = true;
                return;
            }
            try
            {
                if (archive_file != "")
                    ShareFile(archive_file, "GnollHack Saved Games");
            }
            catch (Exception ex)
            {
                await DisplayAlert("Share File Failure", "GnollHack failed to share a saved games archive: " + ex.Message, "OK");
                AboutGrid.IsEnabled = true;
                return;
            }
            AboutGrid.IsEnabled = true;
        }
    }
}