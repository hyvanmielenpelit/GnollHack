using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Security.Cryptography;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Azure;
using Azure.Storage.Blobs;
using Azure.Storage.Blobs.Models;
using Newtonsoft.Json;

#if GNH_MAUI
using GnollHackX;
using Microsoft.Maui.Controls.PlatformConfiguration;
using Microsoft.Maui.Controls.PlatformConfiguration.iOSSpecific;

namespace GnollHackM
#else
using GnollHackX.Pages.Game;
using Xamarin.Essentials;
using Xamarin.Forms;
using Xamarin.Forms.PlatformConfiguration;
using Xamarin.Forms.PlatformConfiguration.iOSSpecific;
using Xamarin.Forms.Xaml;

namespace GnollHackX.Pages.MainScreen
#endif
{
    public class GHSaveTransferFile
    {
        public string Name { get; set; }
        public string SaveFileName { get; set; }
        public string Description { get; set; }
        public string CharacterDescription { get; set; }
        public string LocationDescription { get; set; }
        public string ModeDescription { get; set; }
        public bool IsCloud { get; set; }
        public bool IsTracked { get; set; }
        public bool IsValid { get; set; } = true;
        public string FileName { get; set; }
        public string ExtraInfo { get; set; }
        public string CloudDirectory { get; set; } // e.g. playername-GUID
        public Color TextColor => GHApp.DarkMode ? GHColors.White : GHColors.Black;
    }

    public class SaveManifest
    {
        public int ManifestVersion { get; set; }
        public long CreationDateUtc { get; set; }  // Unix timestamp (seconds since UTC epoch)
        public string UserName { get; set; }
        public string PlayerName { get; set; }
        public ulong SaveVersion { get; set; }
        public ulong SaveVersionCompatibility { get; set; }
        public ulong LibVersion { get; set; }
        public ulong LibVersionCompatibility { get; set; }
        public string AppVersion { get; set; }
        public int AppBuildNumber { get; set; }
        public bool HasBackupSaveFile { get; set; }
        public bool HasTrackingFile { get; set; }
        public long SaveTimeStamp { get; set; }
        /// <summary>
        /// Reason the backup (.bup) file was omitted from this transfer.
        /// 0x0000 = not omitted (included normally).
        /// 0x0001 = file was not found in the save directory.
        /// 0x0002 = backup content is identical to the save file (redundant).
        /// Bits may be combined.
        /// </summary>
        public int BackupOmitReason { get; set; }
        /// <summary>
        /// Reason the tracking token file was omitted from this transfer.
        /// 0x0000 = not omitted (token included normally).
        /// 0x0001 = token file was not found in the save/staging directory.
        /// Bits may be combined.
        /// </summary>
        public int TokenOmitReason { get; set; }
        public string CharacterDescription { get; set; }
        public string LocationDescription { get; set; }
        public string ModeDescription { get; set; }
        public FileDetail SaveFile { get; set; }
        public FileDetail BackupFile { get; set; }
        public FileDetail TrackingToken { get; set; }
    }

    public class FileDetail
    {
        public string FileName { get; set; }
        public long FileLength { get; set; }
        public string Sha256 { get; set; }
    }

    public class SaveLock
    {
        public string Guid { get; set; }
        public int Direction { get; set; } // 0 = upload, 1 = download
        public long CreatedUtcUnix { get; set; }  // Unix timestamp (seconds since UTC epoch)
    }

    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class SaveTransferPage : CustomModalPage, ICloseablePage
    {
        private ObservableCollection<GHSaveTransferFile> _saves = new ObservableCollection<GHSaveTransferFile>();
        private CancellationTokenSource _cts = null;
        private bool _isProcessing = false;
        private bool _appeared = false;

        public SaveTransferPage()
        {
            InitializeComponent();
            UIUtils.SetPageThemeOnHandler(this, GHApp.DarkMode);
            UIUtils.SetViewCursorOnHandler(RootGrid, GameCursorType.Normal);
            if (GHApp.DarkMode)
            {
                HeaderLabel.TextColor = GHColors.White;
                StatusLabel.TextColor = GHColors.White;
            }
            SavesCollectionView.ItemsSource = _saves;
            
            TransferModePicker.SelectedIndexChanged -= TransferModePicker_SelectedIndexChanged;
            TransferModePicker.SelectedIndex = 0; // Default to Upload
            TransferModePicker.SelectedIndexChanged += TransferModePicker_SelectedIndexChanged;
        }

        private async void ContentPage_Appearing(object sender, EventArgs e)
        {
            GHApp.BackButtonPressed += BackButtonPressed;
            _appeared = true;

            StatusLabel.Text = "Scanning...";

            await Task.Delay(250);
            await RefreshListAsync();
        }

        private void ContentPage_Disappearing(object sender, EventArgs e)
        {
            GHApp.BackButtonPressed -= BackButtonPressed;
            CancelCurrentProcess();
        }

        private async void TransferModePicker_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (!_appeared) return;
            await RefreshListAsync();
        }

        private void SavesCollectionView_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            UpdateButtons();
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
                SavesCollectionView.Margin = UIUtils.GetMiddleElementMarginWithBorder(bkgView.BorderStyle, width, height);
                EmptyLabel.Margin = SavesCollectionView.Margin;
                HeaderLabel.Margin = UIUtils.GetHeaderMarginWithBorder(bkgView.BorderStyle, width, height);
                BottomLayout.Margin = UIUtils.GetFooterMarginWithBorder(bkgView.BorderStyle, width, height);

                if (width > height && width - BottomLayout.Margin.Left - BottomLayout.Margin.Right >= 500)
                {
                    ButtonMainStack.Orientation = StackOrientation.Horizontal;
                }
                else
                {
                    ButtonMainStack.Orientation = StackOrientation.Vertical;
                }
            }
        }

        private void UpdateButtons()
        {
            var selected = SavesCollectionView.SelectedItem as GHSaveTransferFile;
            if (selected == null || _isProcessing)
            {
                ActionButton.IsEnabled = false;
                ActionButton.Text = "Transfer";
                ActionButton.TextColor = GHColors.Gray;
                return;
            }

            ActionButton.IsEnabled = true;
            if (!selected.IsValid)
            {
                ActionButton.Text = "Delete";
                ActionButton.TextColor = GHColors.Red;
            }
            else
            {
                if (selected.IsCloud)
                {
                    ActionButton.Text = "Download";
                }
                else
                {
                    ActionButton.Text = "Upload";
                }
                ActionButton.TextColor = GHColors.White;
            }
        }

        private bool _backPressed = false;
        private async Task ClosePageAsync(bool playClickedSound)
        {
            _backPressed = true;
            ButtonMainStack.IsEnabled = false;
            if (playClickedSound)
                GHApp.PlayButtonClickedSound();
            await GHApp.PopModalPageAsync();
        }

        private async Task<bool> BackButtonPressed(object sender, EventArgs e)
        {
            if (!_backPressed)
            {
                await ClosePageAsync(false);
            }
            return false;
        }

        public void ClosePage()
        {
            try
            {
                MainThread.BeginInvokeOnMainThread(async () =>
                {
                    try
                    {
                        if (PopupGrid.IsVisible)
                        {
                            if (PopupCancelButton.IsEnabled)
                            {
                                PopupCancelButton_Clicked(PopupCancelButton, EventArgs.Empty);
                            }
                        }
                        else
                        {
                            if (CloseButton.IsEnabled)
                                await ClosePageAsync(true);
                        }
                    }
                    catch (Exception ex)
                    {
                        System.Diagnostics.Debug.WriteLine(ex);
                    }
                });
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine(ex);
            }
        }


        private async Task RefreshListAsync()
        {
            if (_isProcessing) return;

            _saves.Clear();
            StatusLabel.Text = "Scanning...";
            EmptyLabel.IsVisible = false;
            SavesCollectionView.IsEnabled = false;

            try
            {
                if (TransferModePicker.SelectedIndex == 0)
                {
                    // Upload mode (Local files)
                    string saveDir = Path.Combine(GHApp.GHPath, GHConstants.SaveDirectory);
                    if (Directory.Exists(saveDir))
                    {
                        string[] files = Directory.GetFiles(saveDir);
                        foreach (string file in files)
                        {
                            FileInfo fi = new FileInfo(file);
                            if (string.IsNullOrEmpty(fi.Extension))
                            {
                                // Eligible normal save file (not imported .i, error .e, backup .bup, or tracking .ghsft)
                                string baseName = fi.Name;
                                string charName = baseName.StartsWith("1") ? baseName.Substring(1) : baseName;
                                if (files.Any(f => Path.GetFileName(f) == baseName + ".i") ||
                                    files.Any(f => Path.GetFileName(f) == baseName + ".e"))
                                {
                                    continue; // Skip imported or error saves
                                }

                                bool validated = GHApp.GnollHackService.ValidateSaveFile(file, out string resStr);
                                ulong version = 0, compat = 0;
                                uint saveFlags = 0;
                                long timeStamp = 0;
                                bool hasInfo = GHApp.GnollHackService.GetSaveFileInfo(file, out version, out compat, out saveFlags, out timeStamp);
                                bool isTracked = hasInfo && ((GHSaveFlags)saveFlags & GHSaveFlags.FileTrackValid) != 0 && ((GHSaveFlags)saveFlags & GHSaveFlags.NonTrackingMask) == 0;

                                string verStr = GHApp.VersionNumberToString(version).Replace(" (", " ").Replace(")", "");
                                string compatStr = GHApp.VersionNumberToString(compat).Replace(" (", " ").Replace(")", "");

                                string charDesc = "", locDesc = "", modeDesc = "";
                                GHApp.GnollHackService.GetSaveFileDescription(file, out charDesc, out locDesc, out modeDesc);

                                _saves.Add(new GHSaveTransferFile
                                {
                                    Name = charName,
                                    SaveFileName = baseName,
                                    Description = $"Version: {verStr} (Compat: {compatStr}) - " + (validated ? "Valid" : "Invalid/Incompatible"),
                                    CharacterDescription = charDesc,
                                    LocationDescription = locDesc,
                                    ModeDescription = modeDesc,
                                    IsCloud = false,
                                    IsTracked = isTracked,
                                    IsValid = validated,
                                    FileName = file,
                                    ExtraInfo = isTracked ? "Tracked" : "Local Only"
                                });
                            }
                        }
                    }
                }
                else
                {
                    // Download mode (Cloud files)
                    if (!GHApp.HasInternetAccess)
                    {
                        StatusLabel.Text = "No internet access";
                        return;
                    }

                    BlobServiceClient blobServiceClient = GHApp.GetBlobServiceClient();
                    if (blobServiceClient != null)
                    {
                        string containerName = GHApp.GetAzureBlobStorageSaveTransferContainerName();
                        BlobContainerClient containerClient = blobServiceClient.GetBlobContainerClient(containerName);

                        if (await containerClient.ExistsAsync())
                        {
                            string prefix = GHApp.XlogUserName + "/";
                            var resultSegment = containerClient.GetBlobsAsync(prefix: prefix);
                            
                            List<string> manifestNames = new List<string>();
                            List<(string path, string folder)> unknownManifests = new List<(string, string)>();
                            var enumerator = resultSegment.GetAsyncEnumerator();
                            try
                            {
                                while (await enumerator.MoveNextAsync())
                                {
                                    var blobItem = enumerator.Current;
                                    if (blobItem.Name.EndsWith(".json"))
                                    {
                                        manifestNames.Add(blobItem.Name);
                                    }
                                }
                            }
                            finally
                            {
                                await enumerator.DisposeAsync();
                            }

                            foreach (var manifestPath in manifestNames)
                            {
                                BlobClient manifestClient = containerClient.GetBlobClient(manifestPath);
                                using (MemoryStream ms = new MemoryStream())
                                {
                                    await manifestClient.DownloadToAsync(ms);
                                    string manifestText = Encoding.UTF8.GetString(ms.ToArray());
                                    var manifest = JsonConvert.DeserializeObject<SaveManifest>(manifestText);
                                    if (manifest == null)
                                        continue;

                                    // Check manifest version; version 0 means old/unrecognized format
                                    if (manifest.ManifestVersion == 0)
                                    {
                                        string[] pathParts = manifestPath.Split('/');
                                        string folder = pathParts.Length > 1 ? pathParts[pathParts.Length - 2] : "";
                                        unknownManifests.Add((manifestPath, folder));
                                        continue;
                                    }

                                    string[] parts = manifestPath.Split('/');
                                    string directory = parts.Length > 1 ? parts[parts.Length - 2] : "";

                                    string baseName = manifest.SaveFile.FileName;
                                    string charName = baseName.StartsWith("1") ? baseName.Substring(1) : baseName;
                                    bool cloudValid = manifest.SaveVersion == GHApp.GHVersionNumber ? true :
                                        (GHApp.GHVersionNumber > manifest.SaveVersion ? GHApp.GHVersionCompatibility <= manifest.SaveVersion :
                                        manifest.SaveVersionCompatibility <= GHApp.GHVersionNumber);

                                    string verStr = GHApp.VersionNumberToString(manifest.SaveVersion).Replace(" (", " ").Replace(")", "");
                                    string compatStr = GHApp.VersionNumberToString(manifest.SaveVersionCompatibility).Replace(" (", " ").Replace(")", "");

                                    _saves.Add(new GHSaveTransferFile
                                    {
                                        Name = charName,
                                        SaveFileName = baseName,
                                        Description = $"Cloud Save - Version: {verStr} (Compat: {compatStr}) - " + (cloudValid ? "Valid" : "Invalid/Incompatible"),
                                        CharacterDescription = manifest.CharacterDescription,
                                        LocationDescription = manifest.LocationDescription,
                                        ModeDescription = manifest.ModeDescription,
                                        IsCloud = true,
                                        IsTracked = manifest.HasTrackingFile,
                                        IsValid = cloudValid,
                                        FileName = manifestPath,
                                        ExtraInfo = manifest.HasTrackingFile ? "Tracked" : "Local Only",
                                        CloudDirectory = directory
                                    });
                                }
                            }

                            // Prompt user about any unrecognized manifests
                            foreach (var (blobPath, folder) in unknownManifests)
                            {
                                bool deleteIt = await GHApp.DisplayMessageBox(this,
                                    "Unrecognized Cloud Save",
                                    $"A cloud save file with an unrecognized format was found (path: {blobPath}). " +
                                    "This may be from an older version of the app that is no longer compatible. " +
                                    "Do you want to delete it from cloud storage?",
                                    "Delete", "Keep");
                                if (deleteIt)
                                {
                                    try
                                    {
                                        if (!string.IsNullOrEmpty(folder))
                                        {
                                            string deletePrefix = $"{GHApp.XlogUserName}/{folder}/";
                                            var deleteEnum = containerClient.GetBlobsAsync(prefix: deletePrefix).GetAsyncEnumerator();
                                            try
                                            {
                                                while (await deleteEnum.MoveNextAsync())
                                                    await containerClient.GetBlobClient(deleteEnum.Current.Name).DeleteIfExistsAsync();
                                            }
                                            finally
                                            {
                                                await deleteEnum.DisposeAsync();
                                            }
                                        }
                                        else
                                        {
                                            // Fallback: delete just the manifest blob
                                            await containerClient.GetBlobClient(blobPath).DeleteIfExistsAsync();
                                        }
                                    }
                                    catch (Exception delEx)
                                    {
                                        await GHApp.DisplayMessageBox(this, "Deletion Failed",
                                            "Could not delete the unrecognized cloud save: " + delEx.Message, "OK");
                                    }
                                }
                            }
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
                StatusLabel.Text = "Error loading list";
                EmptyLabel.IsVisible = _saves.Count == 0;
                SavesCollectionView.IsEnabled = true;
                UpdateButtons();
                return;
            }

            StatusLabel.Text = TransferModePicker.SelectedIndex == 0 ? "Eligible Local Saves" : "Saves Available in Cloud";
            EmptyLabel.IsVisible = _saves.Count == 0;
            SavesCollectionView.IsEnabled = true;
            UpdateButtons();
        }

        private async void ActionButton_Clicked(object sender, EventArgs e)
        {
            var selected = SavesCollectionView.SelectedItem as GHSaveTransferFile;
            if (selected == null || _isProcessing) return;

            if (!selected.IsValid)
            {
                string displayCharName = selected.Name;
                bool confirm = await ShowMessagePopupAsync("Confirm Deletion", $"Are you sure to delete the invalid save game \"{displayCharName}\" from save file transfer?", "Yes", "No", redTitle: true);
                if (confirm)
                {
                    await DeleteInvalidSaveAsync(selected);
                }
                return;
            }

            ActionButton.IsEnabled = false;
            if (selected.IsCloud)
            {
                await RunDownloadProcessAsync(selected);
            }
            else
            {
                await RunUploadProcessAsync(selected);
            }
            ActionButton.IsEnabled = true;
        }

        private async Task DeleteInvalidSaveAsync(GHSaveTransferFile file)
        {
            _isProcessing = true;
            UpdateButtons();
            
            PopupGrid.IsVisible = true;
            PopupProgressBar.Progress = 0.0;
            PopupStatusLabel.Text = "Deleting invalid save...";
            PopupCancelButton.IsEnabled = false;

            try
            {
                if (file.IsCloud)
                {
                    BlobServiceClient blobServiceClient = GHApp.GetBlobServiceClient();
                    if (blobServiceClient != null)
                    {
                        string containerName = GHApp.GetAzureBlobStorageSaveTransferContainerName();
                        BlobContainerClient containerClient = blobServiceClient.GetBlobContainerClient(containerName);

                        string cloudFolder = file.CloudDirectory;
                        if (!string.IsNullOrEmpty(cloudFolder))
                        {
                            string prefix = $"{GHApp.XlogUserName}/{cloudFolder}/";
                            var blobSegment = containerClient.GetBlobsAsync(prefix: prefix);
                            var enumerator = blobSegment.GetAsyncEnumerator();
                            try
                            {
                                while (await enumerator.MoveNextAsync())
                                {
                                    var item = enumerator.Current;
                                    await containerClient.GetBlobClient(item.Name).DeleteIfExistsAsync();
                                }
                            }
                            finally
                            {
                                await enumerator.DisposeAsync();
                            }
                        }
                    }
                }
                else
                {
                    string localSaveFile = file.FileName;
                    if (File.Exists(localSaveFile)) File.Delete(localSaveFile);
                    if (File.Exists(localSaveFile + ".bup")) File.Delete(localSaveFile + ".bup");
                    if (File.Exists(localSaveFile + GHConstants.SaveFileTrackingSuffix)) File.Delete(localSaveFile + GHConstants.SaveFileTrackingSuffix);
                }
                PopupProgressBar.Progress = 1.0;
                PopupStatusLabel.Text = "Deleted successfully!";
                await Task.Delay(1000);
            }
            catch (Exception ex)
            {
                await GHApp.DisplayMessageBox(this, "Deletion Failed", "Error during deletion: " + ex.Message, "OK");
            }
            finally
            {
                PopupGrid.IsVisible = false;
                PopupCancelButton.IsEnabled = true;
                _isProcessing = false;
                UpdateButtons();
                await RefreshListAsync();
            }
        }

        private async void CloseButton_Clicked(object sender, EventArgs e)
        {
            CancelCurrentProcess();
            await ClosePageAsync(true);
        }

        private void PopupCancelButton_Clicked(object sender, EventArgs e)
        {
            CancelCurrentProcess();
        }

        private void CancelCurrentProcess()
        {
            if (_cts != null)
            {
                _cts.Cancel();
                // Do not Dispose here — the upload/download task may still be
                // referencing the CancellationToken.  Disposal happens in the
                // finally block of RunUploadProcessAsync / RunDownloadProcessAsync.
            }
        }

        private async Task<bool> CheckServerUpAsync()
        {
            try
            {
                string host = "gnollhack.com";
                if (!string.IsNullOrEmpty(GHApp.XlogPostAddress))
                {
                    var uri = new Uri(GHApp.XlogPostAddress);
                    host = uri.Host;
                }
                using (var tcp = new System.Net.Sockets.TcpClient())
                {
                    var connectTask = tcp.ConnectAsync(host, 443);
                    var delayTask = Task.Delay(3000); // 3 sec timeout
                    var completedTask = await Task.WhenAny(connectTask, delayTask);
                    if (completedTask == connectTask)
                    {
                        return tcp.Connected;
                    }
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Server reachability check failed: " + ex.Message);
            }
            return false;
        }

        private async Task RunUploadProcessAsync(GHSaveTransferFile fileInfo)
        {
            _isProcessing = true;
            UpdateButtons();
            PopupGrid.IsVisible = true;
            PopupProgressBar.Progress = 0.0;
            PopupStatusLabel.Text = "Preparing upload...";
            _cts = new CancellationTokenSource();
            CancellationToken token = _cts.Token;

            string playerName = fileInfo.SaveFileName;
            string guid = Guid.NewGuid().ToString();
            string cloudFolder = $"{playerName}-{guid}";
            string localSaveDir = Path.Combine(GHApp.GHPath, GHConstants.SaveDirectory);
            string localSaveFile = Path.Combine(localSaveDir, playerName);
            string localBackupFile = localSaveFile + ".bup";

            string tempDir = Path.Combine(GHApp.GHPath, GHConstants.TransferTempDirectory);
            string uploadDir = Path.Combine(GHApp.GHPath, GHConstants.TransferUploadDirectory);
            // Each upload session stores its files in a GUID-based subdirectory to avoid conflicts
            string uploadSessionDir = Path.Combine(uploadDir, "files-" + guid);
            
            bool createdToken = false;
            bool isTracked = false;
            long saveTimeStamp = 0;
            BlobContainerClient containerClient = null;

            try
            {
                // Check Internet & Server Reachability
                token.ThrowIfCancellationRequested();
                if (!GHApp.HasInternetAccess)
                {
                    throw new Exception("Internet connection was lost.");
                }

                PopupStatusLabel.Text = "Checking server reachability...";
                bool serverUp = await CheckServerUpAsync();
                if (!serverUp)
                {
                    bool proceed = await ShowMessagePopupAsync("Server Unreachable", "The GnollHack tracking server is unreachable. If your save is tracked, upload will fail to register the tracking token. Do you want to proceed anyway?", "Yes", "No", redTitle: true);
                    if (!proceed)
                    {
                        throw new OperationCanceledException("Server unreachable.");
                    }
                }

                // 2. Target check & Lock Expiry checks
                PopupStatusLabel.Text = "Checking existing files on Azure...";
                BlobServiceClient blobServiceClient = GHApp.GetBlobServiceClient();
                if (blobServiceClient == null) throw new Exception("Azure client not available.");
                string containerName = GHApp.GetAzureBlobStorageSaveTransferContainerName();
                containerClient = blobServiceClient.GetBlobContainerClient(containerName);
                await GHApp.CheckCreateSaveTransferContainer(containerName);

                // Look for active locks or manifests for this player
                string prefix = $"{GHApp.XlogUserName}/";
                var blobSegment = containerClient.GetBlobsAsync(prefix: prefix, cancellationToken: token);
                var enumerator = blobSegment.GetAsyncEnumerator();
                try
                {
                    while (await enumerator.MoveNextAsync())
                    {
                        var item = enumerator.Current;
                        if (item.Name.Contains($"/{playerName}-") && item.Name.EndsWith(".lock"))
                        {
                            BlobClient lockClient = containerClient.GetBlobClient(item.Name);
                            using (MemoryStream ms = new MemoryStream())
                            {
                                await lockClient.DownloadToAsync(ms, token);
                                string lockJson = Encoding.UTF8.GetString(ms.ToArray());
                                var existingLock = JsonConvert.DeserializeObject<SaveLock>(lockJson);
                                if (existingLock != null)
                                {
                                    if (DateTimeOffset.UtcNow.ToUnixTimeSeconds() - existingLock.CreatedUtcUnix < 3600)
                                    {
                                        // Lock is active
                                        bool overrideLock = await ShowMessagePopupAsync("Active Lock Found", "An active Save Transfer session is currently locked on another device for this save name. Force override?", "Yes", "No", redTitle: true);
                                        if (!overrideLock)
                                        {
                                            throw new Exception("Transfer aborted: Another device has locked this save file.");
                                        }
                                    }
                                    // Lock is expired, delete it
                                    await lockClient.DeleteIfExistsAsync(cancellationToken: token);
                                }
                            }
                        }
                        else if (item.Name.Contains($"/{playerName}-") && item.Name.EndsWith(".json"))
                        {
                            bool overwrite = await ShowMessagePopupAsync("Save Already in Cloud", "A save file for this character already exists in the cloud. Overwrite?", "Yes", "No", redTitle: true);
                            if (!overwrite)
                            {
                                throw new Exception("Transfer aborted: Overwrite declined.");
                            }
                        }
                    }
                }
                finally
                {
                    await enumerator.DisposeAsync();
                }

                // 3a. Cleanup before start — find a usable temp directory
                PopupStatusLabel.Text = "Cleaning temp folders...";
                tempDir = await FindUsableTempDirectoryAsync(tempDir);

                // Create the per-session upload subdirectory (files-{guid}/)
                // Guard: if the directory somehow already exists (extremely unlikely), try to delete it;
                // if that fails, generate a fresh GUID so neither the local dir nor the cloud folder collide.
                if (!Directory.Exists(uploadDir)) GHApp.CheckCreateDirectory(uploadDir);
                if (Directory.Exists(uploadSessionDir))
                {
                    try
                    {
                        Directory.Delete(uploadSessionDir, true);
                    }
                    catch
                    {
                        guid = Guid.NewGuid().ToString();
                        cloudFolder = $"{playerName}-{guid}";
                        uploadSessionDir = Path.Combine(uploadDir, "files-" + guid);
                        Debug.WriteLine("RunUploadProcessAsync: collision on session dir, regenerated GUID: " + guid);
                    }
                }
                GHApp.CheckCreateDirectory(uploadSessionDir);

                // 3b. Verify validity & Read flags
                PopupStatusLabel.Text = "Validating local save file...";
                bool validated = GHApp.GnollHackService.ValidateSaveFile(localSaveFile, out string validateMsg);
                if (!validated)
                {
                    throw new Exception("Local save file failed validation check: " + validateMsg);
                }

                ulong saveVer = 0, saveCompat = 0;
                uint saveFlags = 0;
                if (!GHApp.GnollHackService.GetSaveFileInfo(localSaveFile, out saveVer, out saveCompat, out saveFlags, out saveTimeStamp))
                {
                    throw new Exception("Could not read save file header statistics.");
                }
                isTracked = ((GHSaveFlags)saveFlags & GHSaveFlags.FileTrackValid) != 0 && ((GHSaveFlags)saveFlags & GHSaveFlags.NonTrackingMask) == 0;

                string charDesc = "", locDesc = "", modeDesc = "";
                GHApp.GnollHackService.GetSaveFileDescription(localSaveFile, out charDesc, out locDesc, out modeDesc);

                // 4. Create lock file locally
                PopupStatusLabel.Text = "Creating upload lock...";
                var uploadLock = new SaveLock
                {
                    Guid = guid,
                    Direction = 0,
                    CreatedUtcUnix = DateTimeOffset.UtcNow.ToUnixTimeSeconds()
                };
                string localLockPath = Path.Combine(tempDir, playerName + ".lock");
                if (File.Exists(localLockPath))
                {
                    try { File.SetAttributes(localLockPath, FileAttributes.Normal); } catch { /* best effort */ }
                    File.Delete(localLockPath);
                }
                File.WriteAllText(localLockPath, JsonConvert.SerializeObject(uploadLock));

                // 5. Upload lock file to Azure
                string cloudLockPath = $"{GHApp.XlogUserName}/{cloudFolder}/{playerName}.lock";
                BlobClient uploadLockClient = containerClient.GetBlobClient(cloudLockPath);
                await uploadLockClient.UploadAsync(localLockPath, token);

                // 6. Download and double-check GUID matches (in-memory — no disk write needed)
                PopupStatusLabel.Text = "Verifying upload lock...";
                SaveLock downloadedLock;
                using (var verifyMs = new MemoryStream())
                {
                    await uploadLockClient.DownloadToAsync(verifyMs, token);
                    downloadedLock = JsonConvert.DeserializeObject<SaveLock>(Encoding.UTF8.GetString(verifyMs.ToArray()));
                }
                if (downloadedLock == null || downloadedLock.Guid != guid)
                {
                    throw new Exception("Upload lock verify failed. A concurrent session may have overwritten the lock.");
                }

                PopupProgressBar.Progress = 0.2;

                // 7. Save tracking file (with GnollHack server)
                if (isTracked)
                {
                    PopupStatusLabel.Text = "Handling tracking token...";
                    string localTokenPath = localSaveFile + GHConstants.SaveFileTrackingSuffix;
                    string uploadTokenPath = Path.Combine(uploadSessionDir, playerName + GHConstants.SaveFileTrackingSuffix);

                    bool platformUsesTracking = GHApp.IsSaveFileTrackingNeeded;

                    if (platformUsesTracking)
                    {
                        if (File.Exists(localTokenPath))
                        {
                            File.Copy(localTokenPath, uploadTokenPath, true);
                        }
                        else
                        {
                            throw new Exception("Tracking token is missing on a tracked platform.");
                        }
                    }
                    else
                    {
                        // On Android/iOS: Register new token with server
                        long timeStamp = saveTimeStamp;
                        long saveLength = new FileInfo(localSaveFile).Length;
                        string saveSha = "";
                        using (var sha = SHA256.Create())
                        {
                            using (var fs = File.OpenRead(localSaveFile))
                            {
                                saveSha = Convert.ToBase64String(sha.ComputeHash(fs));
                            }
                        }

                        SendResult sResult = await GHApp.SendSaveFileTrackingSaveRequest(this, timeStamp, Path.Combine(uploadSessionDir, playerName), saveLength, saveSha);
                        if (!sResult.IsSuccess)
                        {
                            throw new Exception("Failed to register tracking token on server: " + sResult.Message);
                        }
                        createdToken = true;
                    }
                }

                // 8. Compute hashes
                PopupStatusLabel.Text = "Computing hashes...";
                FileInfo fiSave = new FileInfo(localSaveFile);
                string hashSave = "";
                using (var sha = SHA256.Create())
                {
                    using (var fs = File.OpenRead(localSaveFile))
                    {
                        hashSave = Convert.ToBase64String(sha.ComputeHash(fs));
                    }
                }

                bool backupExists = File.Exists(localBackupFile);
                bool backupIsDifferent = false;
                int backupOmitReason = 0x0000;
                string hashBackup = "";
                long backupLen = 0;
                if (backupExists)
                {
                    FileInfo fiBup = new FileInfo(localBackupFile);
                    backupLen = fiBup.Length;
                    using (var sha = SHA256.Create())
                    {
                        using (var fs = File.OpenRead(localBackupFile))
                        {
                            hashBackup = Convert.ToBase64String(sha.ComputeHash(fs));
                        }
                    }
                    backupIsDifferent = (backupLen != fiSave.Length) || (hashBackup != hashSave);
                    if (!backupIsDifferent)
                        backupOmitReason |= 0x0002; // same content as save file
                }
                else
                {
                    backupOmitReason |= 0x0001; // file not found in save directory
                }

                string hashToken = "";
                long tokenLen = 0;
                int tokenOmitReason = 0x0000;

                // Record all reasons why a tracking token may be absent.
                // Multiple flags can be set simultaneously (e.g. wizard + non-scoring).
                // Bit definitions:
                //   0x0001 — token file not found in local staging directory (isTracked but file missing)
                //   0x0002 — (case 5) tracking not supported by this platform/build
                //   0x0004 — (case 6) tracking supported but not needed on this platform (Android/iOS)
                //   0x0008 — (case 7) tracking supported+needed but turned off by user
                //   0x0010 — (case 8) tracking supported+needed+on but save tracking has failed (FileTrackValid not set)
                //   0x0020 — (case 4) non-scoring game (imported/modified save)
                //   0x0040 — (case 1) wizard / debug mode
                //   0x0080 — (case 2) explore mode
                //   0x0100 — (case 3) casual mode
                var ghFlags = (GHSaveFlags)saveFlags;
                bool trackingSupportedOnPlatform = true; /* Yes for this app */
                bool trackingNeededOnPlatform = GHApp.IsSaveFileTrackingNeeded;
                bool trackingOn = GHApp.SaveFileTracking;

                if (!trackingSupportedOnPlatform)
                    tokenOmitReason |= 0x0002;
                else if (!trackingNeededOnPlatform)
                    tokenOmitReason |= 0x0004; // supported but not needed (Android/iOS)
                else if (!trackingOn)
                    tokenOmitReason |= 0x0008; // turned off by user
                else if ((ghFlags & GHSaveFlags.FileTrackValid) == 0)
                    tokenOmitReason |= 0x0010; // tracking failed

                if ((ghFlags & GHSaveFlags.NonScoring) != 0)
                    tokenOmitReason |= 0x0020; // non-scoring game
                if ((ghFlags & GHSaveFlags.DebugMode) != 0)
                    tokenOmitReason |= 0x0040; // wizard / debug mode
                if ((ghFlags & GHSaveFlags.ExploreMode) != 0)
                    tokenOmitReason |= 0x0080; // explore mode
                if ((ghFlags & GHSaveFlags.CasualMode) != 0)
                    tokenOmitReason |= 0x0100; // casual mode

                string uploadTokenFile = Path.Combine(uploadSessionDir, playerName + GHConstants.SaveFileTrackingSuffix);
                if (isTracked && File.Exists(uploadTokenFile))
                {
                    FileInfo fiTok = new FileInfo(uploadTokenFile);
                    tokenLen = fiTok.Length;
                    using (var sha = SHA256.Create())
                    {
                        using (var fs = File.OpenRead(uploadTokenFile))
                        {
                            hashToken = Convert.ToBase64String(sha.ComputeHash(fs));
                        }
                    }
                }
                else if (isTracked)
                {
                    tokenOmitReason |= 0x0001; // token file not found in staging directory
                }

                // 9. Create manifest file
                PopupStatusLabel.Text = "Creating JSON manifest...";
                var manifest = new SaveManifest
                {
                    ManifestVersion = 1,
                    CreationDateUtc = DateTimeOffset.UtcNow.ToUnixTimeSeconds(),
                    UserName = GHApp.XlogUserName,
                    PlayerName = playerName,
                    SaveVersion = saveVer,
                    SaveVersionCompatibility = saveCompat,
                    LibVersion = GHApp.GHVersionNumber,
                    LibVersionCompatibility = GHApp.GHVersionCompatibility,
                    AppVersion = GHApp.GHVersionString,
                    AppBuildNumber = 0, // build number is int, default 0 if not available
                    HasBackupSaveFile = backupIsDifferent,
                    HasTrackingFile = isTracked,
                    SaveTimeStamp = saveTimeStamp,
                    BackupOmitReason = backupOmitReason,
                    TokenOmitReason = tokenOmitReason,
                    CharacterDescription = charDesc,
                    LocationDescription = locDesc,
                    ModeDescription = modeDesc,
                    SaveFile = new FileDetail { FileName = playerName, FileLength = fiSave.Length, Sha256 = hashSave },
                    BackupFile = backupIsDifferent ? new FileDetail { FileName = playerName + ".bup", FileLength = backupLen, Sha256 = hashBackup } : null,
                    TrackingToken = isTracked ? new FileDetail { FileName = playerName + GHConstants.SaveFileTrackingSuffix, FileLength = tokenLen, Sha256 = hashToken } : null
                };

                string manifestLocalPath = Path.Combine(uploadSessionDir, playerName + ".json");
                File.WriteAllText(manifestLocalPath, JsonConvert.SerializeObject(manifest, Formatting.Indented));

                // 10. Copy save + backup to uploadSessionDir and upload them
                PopupStatusLabel.Text = "Uploading files to cloud...";
                File.Copy(localSaveFile, Path.Combine(uploadSessionDir, playerName), true);
                if (backupExists)
                {
                    File.Copy(localBackupFile, Path.Combine(uploadSessionDir, playerName + ".bup"), true);
                }

                PopupProgressBar.Progress = 0.5;

                // Upload manifest
                BlobClient manifestBlob = containerClient.GetBlobClient($"{GHApp.XlogUserName}/{cloudFolder}/{playerName}.json");
                await manifestBlob.UploadAsync(manifestLocalPath, token);

                // Upload save file
                BlobClient saveBlob = containerClient.GetBlobClient($"{GHApp.XlogUserName}/{cloudFolder}/{playerName}");
                await saveBlob.UploadAsync(Path.Combine(uploadSessionDir, playerName), token);

                // Upload backup if different
                if (backupIsDifferent)
                {
                    BlobClient bupBlob = containerClient.GetBlobClient($"{GHApp.XlogUserName}/{cloudFolder}/{playerName}.bup");
                    await bupBlob.UploadAsync(Path.Combine(uploadSessionDir, playerName + ".bup"), token);
                }

                // Upload tracking token if exists
                if (isTracked && File.Exists(uploadTokenFile))
                {
                    BlobClient tokenBlob = containerClient.GetBlobClient($"{GHApp.XlogUserName}/{cloudFolder}/{playerName}{GHConstants.SaveFileTrackingSuffix}");
                    await tokenBlob.UploadAsync(uploadTokenFile, token);
                }

                // 11. Verify uploads with Azure Blob Properties
                PopupStatusLabel.Text = "Verifying cloud file integrity...";
                var saveProps = await saveBlob.GetPropertiesAsync(cancellationToken: token);
                if (saveProps.Value.ContentLength != fiSave.Length) throw new Exception("Cloud save file size mismatch.");

                if (backupIsDifferent)
                {
                    var bupProps = await containerClient.GetBlobClient($"{GHApp.XlogUserName}/{cloudFolder}/{playerName}.bup").GetPropertiesAsync(cancellationToken: token);
                    if (bupProps.Value.ContentLength != backupLen) throw new Exception("Cloud backup save file size mismatch.");
                }

                if (isTracked && File.Exists(uploadTokenFile))
                {
                    var tokProps = await containerClient.GetBlobClient($"{GHApp.XlogUserName}/{cloudFolder}/{playerName}{GHConstants.SaveFileTrackingSuffix}").GetPropertiesAsync(cancellationToken: token);
                    if (tokProps.Value.ContentLength != tokenLen) throw new Exception("Cloud tracking token size mismatch.");
                }

                // 12. Double-check download to transfer_temp
                PopupStatusLabel.Text = "Downloading files for verification...";
                string tempManifest = Path.Combine(tempDir, playerName + ".json");
                string tempSave = Path.Combine(tempDir, playerName);
                string tempBup = Path.Combine(tempDir, playerName + ".bup");
                string tempTok = Path.Combine(tempDir, playerName + GHConstants.SaveFileTrackingSuffix);

                await manifestBlob.DownloadToAsync(tempManifest, token);
                await saveBlob.DownloadToAsync(tempSave, token);

                if (backupIsDifferent)
                {
                    await containerClient.GetBlobClient($"{GHApp.XlogUserName}/{cloudFolder}/{playerName}.bup").DownloadToAsync(tempBup, token);
                }
                if (isTracked && File.Exists(uploadTokenFile))
                {
                    await containerClient.GetBlobClient($"{GHApp.XlogUserName}/{cloudFolder}/{playerName}{GHConstants.SaveFileTrackingSuffix}").DownloadToAsync(tempTok, token);
                }

                // 13. Verify download
                PopupStatusLabel.Text = "Double checking hashes and validate check...";
                string dlSaveHash = "";
                using (var sha = SHA256.Create())
                {
                    using (var fs = File.OpenRead(tempSave))
                    {
                        dlSaveHash = Convert.ToBase64String(sha.ComputeHash(fs));
                    }
                }
                if (dlSaveHash != hashSave || new FileInfo(tempSave).Length != fiSave.Length)
                {
                    throw new Exception("Downloaded save file does not match locally uploaded file.");
                }

                bool dlValidated = GHApp.GnollHackService.ValidateSaveFile(tempSave, out string dlValidateMsg);
                if (!dlValidated)
                {
                    throw new Exception("Downloaded save file failed validation check: " + dlValidateMsg);
                }

                if (File.ReadAllText(tempManifest) != File.ReadAllText(manifestLocalPath))
                {
                    throw new Exception("Downloaded manifest does not match locally created manifest.");
                }

                PopupProgressBar.Progress = 0.8;

                // 14. Backup & Cleanup
                PopupStatusLabel.Text = "Completing transfer operations...";
                await uploadLockClient.DeleteIfExistsAsync(cancellationToken: token);
                if (Directory.Exists(tempDir)) Directory.Delete(tempDir, true);

                // Move original files from save/ to transfer_upload/
                File.Delete(localSaveFile);
                if (File.Exists(localBackupFile)) File.Delete(localBackupFile);
                if (File.Exists(localSaveFile + GHConstants.SaveFileTrackingSuffix)) File.Delete(localSaveFile + GHConstants.SaveFileTrackingSuffix);

                // 15. (Old backups are now stored as files-{old-guid}/ subdirs and cleaned by date at startup)

                PopupProgressBar.Progress = 1.0;
                PopupStatusLabel.Text = "Transfer successful.";

                await Task.Delay(1000);
                PopupGrid.IsVisible = false;

                await ShowMessagePopupAsync("Success", "Save file uploaded successfully to cloud.", "OK");
            }
            catch (OperationCanceledException)
            {
                PopupGrid.IsVisible = false;
                await CleanUpUploadFailAsync(playerName, uploadSessionDir, isTracked && createdToken, containerClient, cloudFolder, saveTimeStamp);
                await ShowMessagePopupAsync("Cancelled", "Save transfer was cancelled.", "OK");
            }
            catch (Exception ex)
            {
                PopupGrid.IsVisible = false;
                await CleanUpUploadFailAsync(playerName, uploadSessionDir, isTracked && createdToken, containerClient, cloudFolder, saveTimeStamp);
                await GHApp.DisplayMessageBox(this, "Upload Failed", "Error during save file upload: " + ex.Message, "OK");
            }
            finally
            {
                if (_cts != null)
                {
                    _cts.Dispose();
                    _cts = null;
                }
                _isProcessing = false;
                UpdateButtons();
                await RefreshListAsync();
            }
        }

        private async Task CleanUpUploadFailAsync(string playerName, string uploadDir, bool consumeToken, BlobContainerClient containerClient, string cloudFolder, long timeStamp)
        {
            try
            {
                if (consumeToken)
                {
                    string tokenPath = Path.Combine(uploadDir, playerName);
                    if (File.Exists(tokenPath + GHConstants.SaveFileTrackingSuffix))
                    {
                        long len = 0;
                        string saveFile = Path.Combine(Path.Combine(GHApp.GHPath, GHConstants.SaveDirectory), playerName);
                        if (File.Exists(saveFile)) len = new FileInfo(saveFile).Length;
                        await GHApp.SendSaveFileTrackingLoadRequest(this, timeStamp, tokenPath, len, "");
                    }
                }

                // Delete remote lock and folder files
                string prefix = $"{GHApp.XlogUserName}/{cloudFolder}/";
                var blobSegment = containerClient.GetBlobsAsync(prefix: prefix);
                var enumerator = blobSegment.GetAsyncEnumerator();
                try
                {
                    while (await enumerator.MoveNextAsync())
                    {
                        var item = enumerator.Current;
                        await containerClient.GetBlobClient(item.Name).DeleteIfExistsAsync();
                    }
                }
                finally
                {
                    await enumerator.DisposeAsync();
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine("CleanUpUploadFailAsync error: " + ex.Message);
            }
        }

        private async Task RunDownloadProcessAsync(GHSaveTransferFile cloudFile)
        {
            _isProcessing = true;
            UpdateButtons();
            PopupGrid.IsVisible = true;
            PopupProgressBar.Progress = 0.0;
            PopupStatusLabel.Text = "Preparing download...";
            _cts = new CancellationTokenSource();
            CancellationToken token = _cts.Token;

            string playerName = cloudFile.SaveFileName;
            string cloudFolder = cloudFile.CloudDirectory;
            string guid = cloudFolder.Substring(cloudFolder.LastIndexOf('-') + 1);

            string saveDir = Path.Combine(GHApp.GHPath, GHConstants.SaveDirectory);
            string localSaveFile = Path.Combine(saveDir, playerName);
            string localBackupFile = localSaveFile + ".bup";

            string tempDir = Path.Combine(GHApp.GHPath, GHConstants.TransferTempDirectory);
            string downloadDir = Path.Combine(GHApp.GHPath, GHConstants.TransferDownloadDirectory);
            // Each download session logs its manifest in a GUID-based subdirectory to avoid conflicts
            string downloadSessionDir = Path.Combine(downloadDir, "files-" + guid);

            BlobContainerClient containerClient = null;
            BlobClient cloudLockClient = null;

            try
            {
                token.ThrowIfCancellationRequested();

                // 2. Check local conflicts
                if (File.Exists(localSaveFile))
                {
                    bool overwrite = await ShowMessagePopupAsync("Local Conflict", $"A local save file named '{playerName}' already exists. Overwrite it?", "Yes", "No", redTitle: true);
                    if (!overwrite)
                    {
                        throw new OperationCanceledException("Download cancelled due to local file conflict.");
                    }
                }

                // Check Internet & Server Reachability
                if (!GHApp.HasInternetAccess)
                {
                    throw new Exception("Internet connection was lost.");
                }

                PopupStatusLabel.Text = "Checking server reachability...";
                bool serverUp = await CheckServerUpAsync();
                if (!serverUp && cloudFile.IsTracked)
                {
                    bool proceed = await ShowMessagePopupAsync("Server Unreachable", "The GnollHack tracking server is unreachable. Since the save file in cloud is tracked, download will fail to verify the tracking token. Do you want to proceed anyway?", "Yes", "No", redTitle: true);
                    if (!proceed)
                    {
                        throw new OperationCanceledException("Server unreachable.");
                    }
                }

                // 3. Check for cloud lock files
                PopupStatusLabel.Text = "Checking cloud locks...";
                BlobServiceClient blobServiceClient = GHApp.GetBlobServiceClient();
                if (blobServiceClient == null) throw new Exception("Azure client not available.");
                string containerName = GHApp.GetAzureBlobStorageSaveTransferContainerName();
                containerClient = blobServiceClient.GetBlobContainerClient(containerName);

                string lockBlobPath = $"{GHApp.XlogUserName}/{cloudFolder}/{playerName}.lock";
                cloudLockClient = containerClient.GetBlobClient(lockBlobPath);

                if (await cloudLockClient.ExistsAsync(token))
                {
                    using (MemoryStream ms = new MemoryStream())
                    {
                        await cloudLockClient.DownloadToAsync(ms, token);
                        string lockJson = Encoding.UTF8.GetString(ms.ToArray());
                        var existingLock = JsonConvert.DeserializeObject<SaveLock>(lockJson);
                        if (existingLock != null)
                        {
                            if (DateTimeOffset.UtcNow.ToUnixTimeSeconds() - existingLock.CreatedUtcUnix < 3600)
                            {
                                throw new Exception("Download locked by another process.");
                            }
                            else
                            {
                                await cloudLockClient.DeleteIfExistsAsync(cancellationToken: token);
                            }
                        }
                    }
                }

                // 4. Clean transfer_temp — find a usable temp directory
                PopupStatusLabel.Text = "Cleaning local temp folder...";
                tempDir = await FindUsableTempDirectoryAsync(tempDir);

                // Create the per-session download subdirectory (files-{guid}/)
                // Guard: if the local session dir already exists, try to delete it first;
                // if that fails, use a fresh local GUID for the directory name only
                // (cloudFolder and guid are kept unchanged since they identify the cloud blob location).
                if (!Directory.Exists(downloadDir)) GHApp.CheckCreateDirectory(downloadDir);
                if (Directory.Exists(downloadSessionDir))
                {
                    try
                    {
                        Directory.Delete(downloadSessionDir, true);
                    }
                    catch
                    {
                        string localFallbackGuid = Guid.NewGuid().ToString();
                        downloadSessionDir = Path.Combine(downloadDir, "files-" + localFallbackGuid);
                        Debug.WriteLine("RunDownloadProcessAsync: collision on session dir, using fallback dir: " + downloadSessionDir);
                    }
                }
                GHApp.CheckCreateDirectory(downloadSessionDir);

                // 5. Create lock file in the session subdirectory
                PopupStatusLabel.Text = "Creating download lock...";
                var downloadLock = new SaveLock
                {
                    Guid = guid,
                    Direction = 1,
                    CreatedUtcUnix = DateTimeOffset.UtcNow.ToUnixTimeSeconds()
                };
                string localLockPath = Path.Combine(downloadSessionDir, playerName + ".lock");
                File.WriteAllText(localLockPath, JsonConvert.SerializeObject(downloadLock));

                // 6. Upload lock file to Azure
                await cloudLockClient.UploadAsync(localLockPath, token);

                // 7. Download lock file and check GUID (in-memory — no disk write needed)
                PopupStatusLabel.Text = "Verifying download lock...";
                SaveLock downloadedLock;
                using (var verifyMs = new MemoryStream())
                {
                    await cloudLockClient.DownloadToAsync(verifyMs, token);
                    downloadedLock = JsonConvert.DeserializeObject<SaveLock>(Encoding.UTF8.GetString(verifyMs.ToArray()));
                }
                if (downloadedLock == null || downloadedLock.Guid != guid)
                {
                    throw new Exception("Download lock verification failed. Race condition detected.");
                }

                PopupProgressBar.Progress = 0.2;

                // 8. Check source files exist at Azure
                PopupStatusLabel.Text = "Verifying cloud file existence...";
                BlobClient manifestBlob = containerClient.GetBlobClient($"{GHApp.XlogUserName}/{cloudFolder}/{playerName}.json");
                if (!await manifestBlob.ExistsAsync(token)) throw new Exception("Save manifest does not exist in the cloud.");

                // 9. Download and verify manifest
                PopupStatusLabel.Text = "Downloading manifest...";
                string tempManifestPath = Path.Combine(tempDir, playerName + ".json");
                await manifestBlob.DownloadToAsync(tempManifestPath, token);
                var manifest = JsonConvert.DeserializeObject<SaveManifest>(File.ReadAllText(tempManifestPath));
                if (manifest == null) throw new Exception("Could not parse cloud manifest.");

                // 10. Check version compatibility
                PopupStatusLabel.Text = "Verifying version compatibility...";
                bool compatible = manifest.SaveVersion == GHApp.GHVersionNumber ? true :
                    (GHApp.GHVersionNumber > manifest.SaveVersion ? GHApp.GHVersionCompatibility <= manifest.SaveVersion :
                    manifest.SaveVersionCompatibility <= GHApp.GHVersionNumber);
                if (!compatible)
                {
                    throw new Exception($"Save file version ({manifest.SaveVersion}) is not compatible with this build of GnollHack.");
                }

                PopupProgressBar.Progress = 0.4;

                // 11. Download files
                PopupStatusLabel.Text = "Downloading save files...";
                string tempSavePath = Path.Combine(tempDir, playerName);
                BlobClient saveBlob = containerClient.GetBlobClient($"{GHApp.XlogUserName}/{cloudFolder}/{playerName}");
                await saveBlob.DownloadToAsync(tempSavePath, token);

                if (manifest.HasBackupSaveFile)
                {
                    PopupStatusLabel.Text = "Downloading backup save...";
                    string tempBupPath = Path.Combine(tempDir, playerName + ".bup");
                    BlobClient bupBlob = containerClient.GetBlobClient($"{GHApp.XlogUserName}/{cloudFolder}/{playerName}.bup");
                    await bupBlob.DownloadToAsync(tempBupPath, token);
                }

                if (manifest.HasTrackingFile)
                {
                    PopupStatusLabel.Text = "Downloading tracking token...";
                    string tempTokPath = Path.Combine(tempDir, playerName + GHConstants.SaveFileTrackingSuffix);
                    BlobClient tokBlob = containerClient.GetBlobClient($"{GHApp.XlogUserName}/{cloudFolder}/{playerName}{GHConstants.SaveFileTrackingSuffix}");
                    await tokBlob.DownloadToAsync(tempTokPath, token);
                }

                PopupProgressBar.Progress = 0.6;

                // 12. Check for non-corruption
                PopupStatusLabel.Text = "Checking file integrity...";
                FileInfo dlSaveFi = new FileInfo(tempSavePath);
                if (dlSaveFi.Length != manifest.SaveFile.FileLength) throw new Exception("Save file size mismatch.");
                string dlSaveSha = "";
                using (var sha = SHA256.Create())
                {
                    using (var fs = File.OpenRead(tempSavePath))
                    {
                        dlSaveSha = Convert.ToBase64String(sha.ComputeHash(fs));
                    }
                }
                if (dlSaveSha != manifest.SaveFile.Sha256) throw new Exception("Save file checksum mismatch.");

                bool dlValidated = GHApp.GnollHackService.ValidateSaveFile(tempSavePath, out string dlValidateMsg);
                if (!dlValidated)
                {
                    throw new Exception("Downloaded save validation failed: " + dlValidateMsg);
                }

                ulong dlVer = 0, dlCompat = 0;
                uint dlFlags = 0;
                long dlTimeStamp = 0;
                if (!GHApp.GnollHackService.GetSaveFileInfo(tempSavePath, out dlVer, out dlCompat, out dlFlags, out dlTimeStamp))
                {
                    throw new Exception("Could not read downloaded save statistics.");
                }
                bool dlTracked = ((GHSaveFlags)dlFlags & GHSaveFlags.FileTrackValid) != 0 && ((GHSaveFlags)dlFlags & GHSaveFlags.NonTrackingMask) == 0;
                if (dlTracked != manifest.HasTrackingFile)
                {
                    throw new Exception("Downloaded save tracked status mismatch.");
                }

                // 13 & 14. Save file tracking token handling
                if (manifest.HasTrackingFile)
                {
                    PopupStatusLabel.Text = "Registering/consuming tracking token...";
                    string tempTokPath = Path.Combine(tempDir, playerName + GHConstants.SaveFileTrackingSuffix);
                    if (!File.Exists(tempTokPath)) throw new Exception("Downloaded tracking token file is missing.");

                    bool platformUsesTracking = GHApp.IsSaveFileTrackingNeeded;

                    if (platformUsesTracking)
                    {
                        // Copy token to save folder
                        File.Copy(tempTokPath, Path.Combine(saveDir, playerName + GHConstants.SaveFileTrackingSuffix), true);
                    }
                    else
                    {
                        // Consume tracking token on GnollHack Server
                        // Prefer the timestamp stored in the manifest (set by the uploader from the native C library);
                        // fall back to what we read from the downloaded file via LibGetSaveFileInfo.
                        long timeStamp = manifest.SaveTimeStamp != 0 ? manifest.SaveTimeStamp : dlTimeStamp;
                        if (timeStamp == 0)
                            throw new Exception("Cannot consume tracking token: save file timestamp is 0.");
                        SendResult loadResult = await GHApp.SendSaveFileTrackingLoadRequest(this, timeStamp, Path.Combine(tempDir, playerName), dlSaveFi.Length, dlSaveSha);
                        if (!loadResult.IsSuccess)
                        {
                            throw new Exception("Server tracking token consumption failed: " + loadResult.Message);
                        }
                        File.Delete(tempTokPath);
                    }
                }

                // 15. Copy to save folder
                PopupStatusLabel.Text = "Copying files to local save folder...";
                if (!Directory.Exists(saveDir)) GHApp.CheckCreateDirectory(saveDir);
                File.Copy(tempSavePath, localSaveFile, true);
                if (manifest.HasBackupSaveFile)
                {
                    File.Copy(Path.Combine(tempDir, playerName + ".bup"), localBackupFile, true);
                }
                else
                {
                    // Copy save as backup .bup
                    File.Copy(tempSavePath, localBackupFile, true);
                }

                PopupProgressBar.Progress = 0.8;

                // 16. Delete the files from Azure Blob Storage
                PopupStatusLabel.Text = "Cleaning files in cloud...";
                await manifestBlob.DeleteIfExistsAsync(cancellationToken: token);
                await saveBlob.DeleteIfExistsAsync(cancellationToken: token);
                if (manifest.HasBackupSaveFile)
                {
                    await containerClient.GetBlobClient($"{GHApp.XlogUserName}/{cloudFolder}/{playerName}.bup").DeleteIfExistsAsync(cancellationToken: token);
                }
                if (manifest.HasTrackingFile)
                {
                    await containerClient.GetBlobClient($"{GHApp.XlogUserName}/{cloudFolder}/{playerName}{GHConstants.SaveFileTrackingSuffix}").DeleteIfExistsAsync(cancellationToken: token);
                }

                // 17. Verify deletion and delete lock
                PopupStatusLabel.Text = "Confirming deletion and removing lock...";
                bool clean = true;
                string prefix = $"{GHApp.XlogUserName}/{cloudFolder}/";
                var remBlobs = containerClient.GetBlobsAsync(prefix: prefix, cancellationToken: token);
                var enumerator = remBlobs.GetAsyncEnumerator();
                try
                {
                    while (await enumerator.MoveNextAsync())
                    {
                        var remBlob = enumerator.Current;
                        if (!remBlob.Name.EndsWith(".lock"))
                        {
                            clean = false;
                            break;
                        }
                    }
                }
                finally
                {
                    await enumerator.DisposeAsync();
                }
                if (!clean) throw new Exception("Cloud files cleanup failed: some files remained.");

                await cloudLockClient.DeleteIfExistsAsync(cancellationToken: token);

                // 18. Move manifest to the per-session download subdirectory as a log
                string finalManifestLog = Path.Combine(downloadSessionDir, playerName + ".json");
                File.Move(tempManifestPath, finalManifestLog);

                // 19. Final cleanup check
                if (File.Exists(tempSavePath)) File.Delete(tempSavePath);
                if (File.Exists(Path.Combine(tempDir, playerName + ".bup"))) File.Delete(Path.Combine(tempDir, playerName + ".bup"));

                if (Directory.GetFiles(tempDir).Length > 0)
                {
                    Debug.WriteLine("Warning: tempDir not fully cleared.");
                }

                PopupProgressBar.Progress = 1.0;
                PopupStatusLabel.Text = "Transfer successful.";

                await Task.Delay(1000);
                PopupGrid.IsVisible = false;

                await ShowMessagePopupAsync("Success", "Save file was downloaded successfully.", "OK");
            }
            catch (OperationCanceledException)
            {
                PopupGrid.IsVisible = false;
                await CleanUpDownloadFailAsync(containerClient, cloudLockClient, tempDir, downloadSessionDir);
                await ShowMessagePopupAsync("Cancelled", "Save download was cancelled.", "OK");
            }
            catch (Exception ex)
            {
                PopupGrid.IsVisible = false;
                await CleanUpDownloadFailAsync(containerClient, cloudLockClient, tempDir, downloadSessionDir);
                await GHApp.DisplayMessageBox(this, "Download Failed", "Error during save file download: " + ex.Message, "OK");
            }
            finally
            {
                if (_cts != null)
                {
                    _cts.Dispose();
                    _cts = null;
                }
                _isProcessing = false;
                UpdateButtons();
                await RefreshListAsync();
            }
        }

        /// <summary>
        /// Attempts to find and prepare a clean temp directory. Tries the base path first, then
        /// transfer_temp2, transfer_temp3, … up to transfer_temp10, returning the first directory
        /// that can be emptied successfully. The returned directory is guaranteed to exist and be empty.
        /// </summary>
        private async Task<string> FindUsableTempDirectoryAsync(string baseDir)
        {
            string baseName = baseDir; // e.g. .../transfer_temp
            for (int suffix = 0; suffix <= 10; suffix++)
            {
                string candidate = suffix == 0 ? baseName : baseName + suffix.ToString();
                try
                {
                    if (Directory.Exists(candidate))
                    {
                        // Try recursive delete first
                        try
                        {
                            Directory.Delete(candidate, true);
                        }
                        catch
                        {
                            await Task.Delay(300);
                            try
                            {
                                Directory.Delete(candidate, true);
                            }
                            catch
                            {
                                // Last resort: delete files one by one, clearing attributes first
                                foreach (string f in Directory.GetFiles(candidate))
                                {
                                    try
                                    {
                                        File.SetAttributes(f, FileAttributes.Normal);
                                        File.Delete(f);
                                    }
                                    catch { /* best effort */ }
                                }
                            }
                        }
                    }

                    // (Re-)create the directory
                    GHApp.CheckCreateDirectory(candidate);

                    // Accept this directory only if it is now empty
                    if (Directory.GetFiles(candidate).Length == 0)
                    {
                        if (suffix > 0)
                            Debug.WriteLine($"FindUsableTempDirectory: using fallback '{candidate}' (base was locked).");
                        return candidate;
                    }
                }
                catch (Exception ex)
                {
                    Debug.WriteLine($"FindUsableTempDirectory: candidate '{candidate}' failed: {ex.Message}");
                }
            }

            // Should never reach here, but guarantee a result
            Debug.WriteLine("FindUsableTempDirectory: all candidates failed, falling back to base dir.");
            GHApp.CheckCreateDirectory(baseName);
            return baseName;
        }

        private async Task CleanUpDownloadFailAsync(BlobContainerClient containerClient, BlobClient cloudLockClient, string tempDir, string downloadSessionDir = null)
        {
            try
            {
                // Delete Azure lock
                if (cloudLockClient != null)
                    await cloudLockClient.DeleteIfExistsAsync();

                // Clean temp directory
                if (Directory.Exists(tempDir)) Directory.Delete(tempDir, true);

                // Clean the per-session download subdirectory if one was created
                if (downloadSessionDir != null && Directory.Exists(downloadSessionDir))
                    Directory.Delete(downloadSessionDir, true);
            }
            catch (Exception ex)
            {
                Debug.WriteLine("CleanUpDownloadFailAsync error: " + ex.Message);
            }
        }

        private TaskCompletionSource<bool> _messagePopupTcs;

        private Task<bool> ShowMessagePopupAsync(string title, string message, string okButtonText, string cancelButtonText = null, bool redTitle = false)
        {
            _messagePopupTcs = new TaskCompletionSource<bool>();

            MessagePopupTitleLabel.Text = title;
            MessagePopupTitleLabel.TextColor = redTitle ? GHColors.Red : GHColors.TitleGoldColor;
            MessagePopupLabel.Text = message;

            if (string.IsNullOrEmpty(cancelButtonText))
            {
                MessagePopupOkButton.Text = okButtonText;
                MessagePopupCancelButton.IsVisible = false;
                MessagePopupOkButton.HorizontalOptions = LayoutOptions.Center;
            }
            else
            {
                MessagePopupOkButton.Text = okButtonText;
                MessagePopupCancelButton.Text = cancelButtonText;
                MessagePopupCancelButton.IsVisible = true;
                MessagePopupOkButton.HorizontalOptions = LayoutOptions.End;
                MessagePopupCancelButton.HorizontalOptions = LayoutOptions.Start;
            }

            MessagePopupGrid.IsVisible = true;
            return _messagePopupTcs.Task;
        }

        private void MessagePopupOkButton_Clicked(object sender, EventArgs e)
        {
            MessagePopupGrid.IsVisible = false;
            _messagePopupTcs?.TrySetResult(true);
        }

        private void MessagePopupCancelButton_Clicked(object sender, EventArgs e)
        {
            MessagePopupGrid.IsVisible = false;
            _messagePopupTcs?.TrySetResult(false);
        }
    }
}
