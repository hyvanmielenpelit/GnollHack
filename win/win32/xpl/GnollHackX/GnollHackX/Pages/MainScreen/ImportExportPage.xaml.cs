using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.Compression;
using System.IO;


#if GNH_MAUI
using GnollHackX;
using Microsoft.Maui.Controls.PlatformConfiguration;
using Microsoft.Maui.Controls.PlatformConfiguration.iOSSpecific;

namespace GnollHackM
#else
using Xamarin.Essentials;
using Xamarin.Forms;
using Xamarin.Forms.PlatformConfiguration.iOSSpecific;
using Xamarin.Forms.Xaml;
using GnollHackX.Pages.Game;
using static Xamarin.Essentials.AppleSignInAuthenticator;

namespace GnollHackX.Pages.MainScreen
#endif
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
	public partial class ImportExportPage : ContentPage
	{
		public ImportExportPage ()
		{
			InitializeComponent ();
#if GNH_MAUI
            On<iOS>().SetUseSafeArea(true);
#else
            On<Xamarin.Forms.PlatformConfiguration.iOS>().SetUseSafeArea(true);
#endif
        }

        private async void btnExportSavedGames_Clicked(object sender, EventArgs e)
        {
            ImportExportGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            await GHApp.CheckAndRequestWritePermission(this);
            await GHApp.CheckAndRequestReadPermission(this);
            string archive_file = "";
            try
            {
                archive_file = GHApp.CreateSavedGamesZipArchive();
            }
            catch (Exception ex)
            {
                await DisplayAlert("Archive Creation Failure", "GnollHack failed to create a saved games archive: " + ex.Message, "OK");
                ImportExportGrid.IsEnabled = true;
                return;
            }
            try
            {
                if (archive_file != "")
                    await GHApp.ShareFile(this, archive_file, "GnollHack Saved Games");
            }
            catch (Exception ex)
            {
                await DisplayAlert("Share File Failure", "GnollHack failed to share a saved games archive: " + ex.Message, "OK");
                ImportExportGrid.IsEnabled = true;
                return;
            }
            ImportExportGrid.IsEnabled = true;
        }

        private async void btnImportSavedGames_Clicked(object sender, EventArgs e)
        {
            ImportExportGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            await GHApp.CheckAndRequestWritePermission(this);
            await GHApp.CheckAndRequestReadPermission(this);
            try
            {
                FileResult file = await FilePicker.PickAsync();

                if (file != null)
                {
                    using (Stream s = await file.OpenReadAsync())
                    {
                        if (s != null)
                        {
                            string gnhpath = GHApp.GHPath;
                            if (file.FileName.EndsWith(GHConstants.SavedGameSharedZipFileNameSuffix, StringComparison.OrdinalIgnoreCase))
                            {
                                string savedirpath = Path.Combine(gnhpath, GHConstants.SaveDirectory);
                                GHApp.CheckCreateDirectory(savedirpath);

                                string tempdirpath = Path.Combine(gnhpath, GHConstants.SaveDirectory, GHConstants.UnzipTempSubDirectory);
                                if (Directory.Exists(tempdirpath))
                                    Directory.Delete(tempdirpath, true);
                                GHApp.CheckCreateDirectory(tempdirpath);

                                string temp2dirpath = Path.Combine(gnhpath, GHConstants.SaveDirectory, GHConstants.UnzipZipSubDirectory);
                                if (Directory.Exists(temp2dirpath))
                                    Directory.Delete(temp2dirpath, true);
                                GHApp.CheckCreateDirectory(temp2dirpath);

                                string ziptargetfilename = file.FileName;
                                string fulltargetpath = Path.Combine(tempdirpath, ziptargetfilename);
                                string fulltargetpath2 = Path.Combine(savedirpath, ziptargetfilename);
                                if (System.IO.File.Exists(fulltargetpath2))
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
                                if (extractedfiles != null)
                                {
                                    foreach (string filestr in extractedfiles)
                                    {
                                        if (System.IO.File.Exists(filestr))
                                        {
                                            string out_str = "";
                                            if (GHApp.GnollHackService.ValidateSaveFile(filestr, out out_str))
                                            {
                                                FileInfo fileInfo = new FileInfo(filestr);
                                                string finalname = Path.Combine(savedirpath, fileInfo.Name + ".i");
                                                if (System.IO.File.Exists(finalname))
                                                    System.IO.File.Delete(finalname);
                                                System.IO.File.Move(filestr, finalname);
                                                nextracted++;
                                                if (!string.IsNullOrWhiteSpace(out_str) && GHApp.DebugLogMessages)
                                                    await DisplayAlert("ValidateSaveFile Message", out_str, "OK");
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
                                if (extractedfiles.Length == 0)
                                    await DisplayAlert("No Files in Zip", "There are no files in \'" + ziptargetfilename + "\'.", "OK");
                                else if (nextracted > 0)
                                    await DisplayAlert("Games Saved from Zip", "Saved games from \'" + ziptargetfilename + "\' have been saved to the save directory as non-scoring imported saved games.", "OK");
                                else
                                    await DisplayAlert("No Games Saved in Zip", "No saved games from \'" + ziptargetfilename + "\' were saved to the save directory.", "OK");
                            }
                            else
                            {
                                string out_str = "";
                                if (GHApp.GnollHackService.ValidateSaveFile(file.FullPath, out out_str))
                                {
                                    string targetfilename = file.FileName + ".i";
                                    string savedirpath = Path.Combine(gnhpath, GHConstants.SaveDirectory);
                                    GHApp.CheckCreateDirectory(savedirpath);

                                    string fulltargetpath = Path.Combine(savedirpath, targetfilename);
                                    if (System.IO.File.Exists(fulltargetpath))
                                        System.IO.File.Delete(fulltargetpath);
                                    using (Stream t = System.IO.File.Open(fulltargetpath, FileMode.Create))
                                    {
                                        s.CopyTo(t);
                                    }
                                    await DisplayAlert("Game Saved", "Saved game \'" + file.FileName + "\' has been saved to the save directory as a non-scoring imported saved game.", "OK");
                                    if (!string.IsNullOrWhiteSpace(out_str) && GHApp.DebugLogMessages)
                                        await DisplayAlert("ValidateSaveFile Message", out_str, "OK");
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
            ImportExportGrid.IsEnabled = true;
        }

        private async void btnExportReplays_Clicked(object sender, EventArgs e)
        {
            ImportExportGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            await GHApp.CheckAndRequestWritePermission(this);
            await GHApp.CheckAndRequestReadPermission(this);
            string archive_file = "";
            try
            {
                archive_file = GHApp.CreateReplayZipArchive();
            }
            catch (Exception ex)
            {
                await DisplayAlert("Archive Creation Failure", "GnollHack failed to create a replay archive: " + ex.Message, "OK");
                ImportExportGrid.IsEnabled = true;
                return;
            }
            try
            {
                if (archive_file != "")
                    await GHApp.ShareFile(this, archive_file, "GnollHack Replays");
            }
            catch (Exception ex)
            {
                await DisplayAlert("Share File Failure", "GnollHack failed to share a replay archive: " + ex.Message, "OK");
                ImportExportGrid.IsEnabled = true;
                return;
            }
            ImportExportGrid.IsEnabled = true;

        }

        private async void btnImportReplays_Clicked(object sender, EventArgs e)
        {
            ImportExportGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            await GHApp.CheckAndRequestWritePermission(this);
            await GHApp.CheckAndRequestReadPermission(this);
            try
            {
                FileResult file = await FilePicker.PickAsync();

                if (file != null)
                {
                    using (Stream s = await file.OpenReadAsync())
                    {
                        if (s != null)
                        {
                            string gnhpath = GHApp.GHPath;
                            if (file.FileName.EndsWith(GHConstants.ReplaySharedZipFileNameSuffix, StringComparison.OrdinalIgnoreCase))
                            {
                                string savedirpath = Path.Combine(gnhpath, GHConstants.ReplayDirectory);
                                GHApp.CheckCreateDirectory(savedirpath);

                                string tempdirpath = Path.Combine(gnhpath, GHConstants.ReplayDirectory, GHConstants.UnzipTempSubDirectory);
                                if (Directory.Exists(tempdirpath))
                                    Directory.Delete(tempdirpath, true);
                                GHApp.CheckCreateDirectory(tempdirpath);

                                string temp2dirpath = Path.Combine(gnhpath, GHConstants.ReplayDirectory, GHConstants.UnzipZipSubDirectory);
                                if (Directory.Exists(temp2dirpath))
                                    Directory.Delete(temp2dirpath, true);
                                GHApp.CheckCreateDirectory(temp2dirpath);

                                string ziptargetfilename = file.FileName;
                                string fulltargetpath = Path.Combine(tempdirpath, ziptargetfilename);
                                string fulltargetpath2 = Path.Combine(savedirpath, ziptargetfilename);
                                if (System.IO.File.Exists(fulltargetpath2))
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
                                int ntotalfiles = 0;
                                int ninvalid = 0;
                                string[] extractedfiles = Directory.GetFiles(temp2dirpath);
                                if (extractedfiles != null)
                                {
                                    foreach (string filestr in extractedfiles)
                                    {
                                        if (System.IO.File.Exists(filestr))
                                        {
                                            FileInfo fileInfo = new FileInfo(filestr);
                                            if (fileInfo != null && fileInfo.Name != null && fileInfo.Name.StartsWith(GHConstants.ReplayFileNamePrefix))
                                            {
                                                if (GHApp.ValidateReplayFile(filestr, out string out_str))
                                                {
                                                    string finalname = Path.Combine(savedirpath, fileInfo.Name);
                                                    if (System.IO.File.Exists(finalname))
                                                        System.IO.File.Delete(finalname);
                                                    System.IO.File.Move(filestr, finalname);
                                                    nextracted++;
                                                    ntotalfiles++;

                                                    bool isGZip = fileInfo.Name.Length > GHConstants.ReplayGZipFileNameSuffix.Length && fileInfo.Name.EndsWith(GHConstants.ReplayGZipFileNameSuffix);
                                                    bool isNormalZip = fileInfo.Name.Length > GHConstants.ReplayZipFileNameSuffix.Length && fileInfo.Name.EndsWith(GHConstants.ReplayZipFileNameSuffix);
                                                    bool isZip = isGZip || isNormalZip;
                                                    string usedZipSuffix = isGZip ? GHConstants.ReplayGZipFileNameSuffix : GHConstants.ReplayZipFileNameSuffix;
                                                    int len = fileInfo.Name.Length - GHConstants.ReplayFileNamePrefix.Length - GHConstants.ReplayFileNameSuffix.Length - (isZip ? usedZipSuffix.Length : 0);
                                                    if (len > 0)
                                                    {
                                                        string middleStr = fileInfo.Name.Substring(GHConstants.ReplayFileNamePrefix.Length, len);
                                                        foreach (string contstr in extractedfiles)
                                                        {
                                                            if (System.IO.File.Exists(contstr))
                                                            {
                                                                FileInfo contFileInfo = new FileInfo(contstr);
                                                                if (contFileInfo != null && contFileInfo.Name != null && contFileInfo.Name.StartsWith(GHConstants.ReplayContinuationFileNamePrefix))
                                                                {
                                                                    int contLen = contFileInfo.Name.Length - GHConstants.ReplayContinuationFileNamePrefix.Length - GHConstants.ReplayFileNameSuffix.Length - (isZip ? usedZipSuffix.Length : 0);
                                                                    if (contLen >= len && contFileInfo.Name.Substring(GHConstants.ReplayContinuationFileNamePrefix.Length, len) == middleStr)
                                                                    {
                                                                        string contfinalname = Path.Combine(savedirpath, contFileInfo.Name);
                                                                        if (System.IO.File.Exists(contfinalname))
                                                                            System.IO.File.Delete(contfinalname);
                                                                        System.IO.File.Move(contstr, contfinalname);
                                                                        ntotalfiles++;
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                                else
                                                {
                                                    GHApp.MaybeWriteGHLog(out_str);
                                                    ninvalid++;
                                                }
                                            }
                                        }
                                    }
                                }
                                Directory.Delete(tempdirpath, true);
                                Directory.Delete(temp2dirpath, true);
                                if (extractedfiles.Length == 0)
                                    await DisplayAlert("No Files in Zip", "There are no files in \'" + ziptargetfilename + "\'.", "OK");
                                else if (nextracted > 0)
                                    await DisplayAlert("Replays Saved from Zip", nextracted + " replay " + (nextracted == 1 ? "" : "s") + " (" + ntotalfiles + " file" + (ntotalfiles == 1 ? "" : "s") + ") from \'" + ziptargetfilename + "\' have been saved to the replay directory." + (ninvalid > 0 ? " (" + ninvalid + " were invalid.)" : ""), "OK");
                                else
                                    await DisplayAlert("No Replays Saved from Zip", "No replays from \'" + ziptargetfilename + "\' were saved to the replay directory." + (ninvalid > 0 ? " (" + ninvalid + " were invalid.)" : ""), "OK");
                            }
                            else
                            {
                                await DisplayAlert("Shared Zip File Not Recognized", file.FileName + " is not recognized as a shared replay zip file. Its file name must end with " + GHConstants.ReplaySharedZipFileNameSuffix + ".", "OK");
                            }
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                await DisplayAlert("Error", "An error occurred while trying to import a replay: " + ex.Message, "OK");
            }
            ImportExportGrid.IsEnabled = true;
        }

        private async void btnExportDumplogs_Clicked(object sender, EventArgs e)
        {
            ImportExportGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            await GHApp.CheckAndRequestWritePermission(this);
            await GHApp.CheckAndRequestReadPermission(this);
            string archive_file = "";
            try
            {
                archive_file = GHApp.CreateDumplogZipArchive();
            }
            catch (Exception ex)
            {
                await DisplayAlert("Archive Creation Failure", "GnollHack failed to create a dumplog archive: " + ex.Message, "OK");
                ImportExportGrid.IsEnabled = true;
                return;
            }
            try
            {
                if (archive_file != "")
                    await GHApp.ShareFile(this, archive_file, "GnollHack Dumplogs");
            }
            catch (Exception ex)
            {
                await DisplayAlert("Share File Failure", "GnollHack failed to share a dumplog archive: " + ex.Message, "OK");
                ImportExportGrid.IsEnabled = true;
                return;
            }
            ImportExportGrid.IsEnabled = true;
        }

        private async void CloseButton_Clicked(object sender, EventArgs e)
        {
            ImportExportGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            await App.Current.MainPage.Navigation.PopModalAsync();
        }

        private bool _backPressed = false;
        private async Task<bool> BackButtonPressed(object sender, EventArgs e)
        {
            if (!_backPressed)
            {
                _backPressed = true;
                ImportExportGrid.IsEnabled = false;
                await App.Current.MainPage.Navigation.PopModalAsync();
            }
            return false;
        }

        private void ContentPage_Appearing(object sender, EventArgs e)
        {
            GHApp.BackButtonPressed += BackButtonPressed;
            ImportExportGrid.IsEnabled = true;
        }
        private void ContentPage_Disappearing(object sender, EventArgs e)
        {
            GHApp.BackButtonPressed -= BackButtonPressed;
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

                lblHeader.Margin = UIUtils.GetHeaderMarginWithBorder(bkgView.BorderStyle, width, height);
                CloseButton.Margin = UIUtils.GetFooterMarginWithBorder(bkgView.BorderStyle, width, height);
            }
        }

        private async void btnDeleteSavedGames_Clicked(object sender, EventArgs e)
        {
            ImportExportGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            bool answer = await DisplayAlert("Delete Saved Games?", "Are you sure to delete all saved games?", "Yes", "No");
            if (answer)
            {
                GHApp.GnollHackService.ClearSavedGames();
                btnDeleteSavedGames.Text = "Done";
                btnDeleteSavedGames.TextColor = GHColors.Red;
            }
            ImportExportGrid.IsEnabled = true;
        }

        private async void btnDeleteReplays_Clicked(object sender, EventArgs e)
        {
            ImportExportGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            string directory1 = Path.Combine(GHApp.GHPath, GHConstants.ReplayDirectory);
            int nofiles1 = 0;
            int nofiles_main = 0;
            int nofiles_cont = 0;
            //int nofiles_shared = 0;
            int nofiles_other = 0;
            if (Directory.Exists(directory1))
            {
                string[] files1 = Directory.GetFiles(directory1);
                if (files1 != null)
                {
                    nofiles1 = files1.Length;
                    foreach(string file in files1)
                    {
                        if(!string.IsNullOrWhiteSpace(file))
                        {
                            FileInfo fileInfo = new FileInfo(file);
                            if(fileInfo != null && !string.IsNullOrWhiteSpace(fileInfo.Name))
                            {
                                if (fileInfo.Name.StartsWith(GHConstants.ReplayFileNamePrefix))
                                    nofiles_main++;
                                else if (fileInfo.Name.StartsWith(GHConstants.ReplayContinuationFileNamePrefix))
                                    nofiles_cont++;
                                //else if (fileInfo.Name.StartsWith(GHConstants.ReplaySharedZipFileNamePrefix))
                                //    nofiles_shared++;
                                else
                                    nofiles_other++;
                            }
                        }
                    }
                }
            }
            bool answer = await DisplayAlert("Delete All Replays?", "Are you sure to delete all files (" 
                + nofiles1 + ": " + nofiles_main + " main, " + nofiles_cont + " continuation, "/* + nofiles_shared + " shared, "*/ + nofiles_other + " other) in the " 
                + GHConstants.ReplayDirectory + " directory?", "Yes", "No");
            if (answer)
            {
                try
                {
                    string datadir = Path.Combine(GHApp.GHPath, GHConstants.ReplayDirectory);
                    if (Directory.Exists(datadir))
                        Directory.Delete(datadir, true);

                    btnDeleteReplays.Text = "Done";
                    btnDeleteReplays.TextColor = GHColors.Red;
                }
                catch (Exception ex)
                {
                    btnDeleteReplays.Text = "Failed";
                    btnDeleteReplays.TextColor = GHColors.Red;
                    await DisplayAlert("Deletion Failed", "GnollHack failed to delete the files in " + GHConstants.ReplayDirectory + ": " + ex.Message, "OK");
                }
            }
            ImportExportGrid.IsEnabled = true;
        }

        private async void btnDeleteDumplogs_Clicked(object sender, EventArgs e)
        {
            ImportExportGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            bool answer = await DisplayAlert("Delete Dumplogs?", "Are you sure to delete all dumplogs?", "Yes", "No");
            if (answer)
            {
                GHApp.GnollHackService.ClearDumplogs();
                btnDeleteDumplogs.Text = "Done";
                btnDeleteDumplogs.TextColor = GHColors.Red;
            }
            ImportExportGrid.IsEnabled = true;
        }
    }
}