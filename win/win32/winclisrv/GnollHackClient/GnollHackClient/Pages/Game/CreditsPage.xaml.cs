using GnollHackCommon;
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

namespace GnollHackClient.Pages.Game
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class CreditsPage : ContentPage
    {
        public CreditsPage()
        {
            InitializeComponent();
            On<Xamarin.Forms.PlatformConfiguration.iOS>().SetUseSafeArea(true);
        }

        private async void btnCreditsX_Clicked(object sender, EventArgs e)
        {
            App.PlayButtonClickedSound();
            CreditsTableView.IsEnabled = false;
            string fulltargetpath = Path.Combine(App.GHPath, "xcredits");
            var displFilePage = new DisplayFilePage(fulltargetpath, "Cross-Platform Credits");
            string errormsg = "";
            if (!displFilePage.ReadFile(out errormsg))
            {
                CreditsTableView.IsEnabled = true;
                await DisplayAlert("Error Opening File", "GnollHack cannot open the xcredits file.", "OK");
            }
            else
            {
                await App.Current.MainPage.Navigation.PushModalAsync(displFilePage);
            }
        }

        private async void btnCreditsW_Clicked(object sender, EventArgs e)
        {
            App.PlayButtonClickedSound();
            CreditsTableView.IsEnabled = false;
            string fulltargetpath = Path.Combine(App.GHPath, "credits");
            var displFilePage = new DisplayFilePage(fulltargetpath, "Credits");
            string errormsg = "";
            if (!displFilePage.ReadFile(out errormsg))
            {
                CreditsTableView.IsEnabled = true;
                await DisplayAlert("Error Opening File", "GnollHack cannot open the credits file.", "OK");
            }
            else
            {
                await App.Current.MainPage.Navigation.PushModalAsync(displFilePage);
            }
        }

        private async void btnLicense_Clicked(object sender, EventArgs e)
        {
            App.PlayButtonClickedSound();
            CreditsTableView.IsEnabled = false;
            string fulltargetpath = Path.Combine(App.GHPath, "license");
            var displFilePage = new DisplayFilePage(fulltargetpath, "License");
            string errormsg = "";
            if (!displFilePage.ReadFile(out errormsg))
            {
                CreditsTableView.IsEnabled = true;
                await DisplayAlert("Error Opening File", "GnollHack cannot open the license file.", "OK");
            }
            else
            {
                await App.Current.MainPage.Navigation.PushModalAsync(displFilePage);
            }
        }
        private async void btnGitHub_Clicked(object sender, EventArgs e)
        {
            CreditsTableView.IsEnabled = false;
            App.PlayButtonClickedSound();
            await OpenBrowser(new Uri(GHConstants.GnollHackGitHubPage));
            CreditsTableView.IsEnabled = true;
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
            CreditsTableView.IsEnabled = false;
            App.PlayButtonClickedSound();
            await OpenBrowser(new Uri(GHConstants.GnollHackWebPage));
            CreditsTableView.IsEnabled = true;
        }

        private async void btnWiki_Clicked(object sender, EventArgs e)
        {
            CreditsTableView.IsEnabled = false;
            App.PlayButtonClickedSound();
            await OpenBrowser(new Uri(GHConstants.GnollHackWikiPage));
            CreditsTableView.IsEnabled = true;
        }

        private async void btnSourceCode_Clicked(object sender, EventArgs e)
        {
            CreditsTableView.IsEnabled = false;
            App.PlayButtonClickedSound();
            SecretsFile sourcesf = null;
            foreach (SecretsFile f in App.CurrentSecrets.files)
            {
                if(f.type == "source_zip")
                {
                    sourcesf = f;
                    break;
                }
            }

            if(sourcesf == null)
            {
                await DisplayAlert("Source File Location Not Found", "GnollHack was unable to find instructions to locate the source files archive.", "OK");
                return;
            }

            string ghdir = App.GnollHackService.GetGnollHackPath();
            string targetpath = Path.Combine(ghdir, sourcesf.target_directory, sourcesf.name);

            if(!File.Exists(targetpath))
            {
                await DisplayAlert("Source Files Not Found", "The source files archive was not found.", "OK");
                return;
            }

            byte[] data;
            try
            {
                data = File.ReadAllBytes(targetpath);
            }
            catch(Exception ex)
            {
                await DisplayAlert("Error Occurred", "An error occurred while reading source files: " + ex.Message, "OK");
                return;
            }

            try
            {
                App.PlatformService.SaveFileToDownloads(data, sourcesf.name);
            }
            catch (Exception ex)
            {
                await DisplayAlert("Error Occurred", "An error occurred while saving source files to downloads folder: " + ex.Message, "OK");
                return;
            }
            await DisplayAlert("Source Files in Downloads", "Source files have been successfully saved to the Downloads folder in a zip format. The file is named \'" + sourcesf.name + "\'.", "OK");
            CreditsTableView.IsEnabled = true;
        }

        private async void Button_Clicked(object sender, EventArgs e)
        {
            CloseGrid.IsEnabled = false;
            App.PlayButtonClickedSound();
            await App.Current.MainPage.Navigation.PopModalAsync();
        }

        private bool _backPressed = false;
        private async Task<bool> BackButtonPressed(object sender, EventArgs e)
        {
            if (!_backPressed)
            {
                _backPressed = true;
                await App.Current.MainPage.Navigation.PopModalAsync();
            }
            return false;
        }

        private void ContentPage_Appearing(object sender, EventArgs e)
        {
            App.BackButtonPressed += BackButtonPressed;
            CreditsTableView.IsEnabled = true;
        }
        private void ContentPage_Disappearing(object sender, EventArgs e)
        {
            App.BackButtonPressed -= BackButtonPressed;
        }

        private async void btnCrashReport_Clicked(object sender, EventArgs e)
        {
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
                    return;
                }
            }
        }


        private async void ShareFile(string filename, string title)
        {
            if(!File.Exists(filename))
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
                return;
            }
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
                CloseGrid.Margin = ClientUtils.GetFooterMarginWithBorder(bkgView.BorderStyle, width, height);
            }
        }
    }
}