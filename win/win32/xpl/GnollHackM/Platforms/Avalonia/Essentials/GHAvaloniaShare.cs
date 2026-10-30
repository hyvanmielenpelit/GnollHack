using System;
using System.Collections.Generic;
using System.IO;
using System.Threading.Tasks;
using GnollHackX;
using Microsoft.Maui.ApplicationModel.DataTransfer;

using AvClipboardExtensions = Avalonia.Input.Platform.ClipboardExtensions;
using AvStorageProviderExtensions = Avalonia.Platform.Storage.StorageProviderExtensions;
using AvDispatcher = Avalonia.Threading.Dispatcher;
using AvFilePickerSaveOptions = Avalonia.Platform.Storage.FilePickerSaveOptions;
using AvFolderPickerOpenOptions = Avalonia.Platform.Storage.FolderPickerOpenOptions;
using AvIClipboard = Avalonia.Input.Platform.IClipboard;
using AvIStorageFile = Avalonia.Platform.Storage.IStorageFile;
using AvIStorageFolder = Avalonia.Platform.Storage.IStorageFolder;
using AvTopLevel = Avalonia.Controls.TopLevel;

namespace GnollHackM
{
    /* IShare for the Avalonia desktop target, where sharing saves a copy: a single file is copied to
       a location chosen in a save dialog, multiple files into a folder chosen in a folder dialog, and
       text, followed by the URI on its own line, is placed on the clipboard. Cancelling a dialog does
       nothing. GHFileDialogHost's control replaces the dialogs when they cannot be used with a
       controller. A copy never lands in GnollHack's own folders or over its source, and a local one
       appears only when complete. Runs on the Avalonia UI thread and throws InvalidOperationException
       when no window is open. */
    public sealed class GHAvaloniaShare : IShare
    {
        private const string DefaultFileTitle = "Save a Copy";
        private const string DefaultFolderTitle = "Choose a Folder for the Copies";

        public Task RequestAsync(ShareTextRequest request)
        {
            if (request == null)
                throw new ArgumentNullException(nameof(request));
            if (string.IsNullOrEmpty(request.Text) && string.IsNullOrEmpty(request.Uri))
                throw new ArgumentException("Both the text and the URI are empty.", nameof(request));

            string text = request.Text;
            if (!string.IsNullOrEmpty(request.Uri))
                text = string.IsNullOrEmpty(text) ? request.Uri : text + Environment.NewLine + request.Uri;

            return AvDispatcher.UIThread.InvokeAsync(() => SetClipboardTextOnUIThreadAsync(text));
        }

        public Task RequestAsync(ShareFileRequest request)
        {
            if (request == null)
                throw new ArgumentNullException(nameof(request));
            if (request.File == null || string.IsNullOrWhiteSpace(request.File.FullPath))
                throw new ArgumentException("The request does not name a file to share.", nameof(request));

            string title = string.IsNullOrWhiteSpace(request.Title) ? DefaultFileTitle : request.Title;
            ShareFile file = request.File;
            return AvDispatcher.UIThread.InvokeAsync(() => SaveFileOnUIThreadAsync(title, file));
        }

        public Task RequestAsync(ShareMultipleFilesRequest request)
        {
            if (request == null)
                throw new ArgumentNullException(nameof(request));
            if (request.Files == null || request.Files.Count == 0)
                throw new ArgumentException("The request does not name any files to share.", nameof(request));

            List<ShareFile> files = new List<ShareFile>();
            foreach (ShareFile file in request.Files)
            {
                if (file == null || string.IsNullOrWhiteSpace(file.FullPath))
                    throw new ArgumentException("The request names a file without a path.", nameof(request));
                files.Add(file);
            }

            string title = string.IsNullOrWhiteSpace(request.Title) ? DefaultFolderTitle : request.Title;
            return AvDispatcher.UIThread.InvokeAsync(() => SaveFilesOnUIThreadAsync(title, files));
        }

        private static AvTopLevel GetTopLevel()
        {
            AvTopLevel topLevel = AvTopLevel.GetTopLevel(GHAvaloniaWindowHooks.MainWindow);
            if (topLevel == null)
                throw new InvalidOperationException("No window is open to share from.");
            return topLevel;
        }

        private static async Task SetClipboardTextOnUIThreadAsync(string text)
        {
            AvTopLevel topLevel = GetTopLevel();
            AvIClipboard clipboard = topLevel.Clipboard;
            if (clipboard == null)
                throw new InvalidOperationException("The clipboard is not available.");

            await AvClipboardExtensions.SetTextAsync(clipboard, text);
        }

        private static async Task SaveFileOnUIThreadAsync(string title, ShareFile file)
        {
            if (GHFileDialogHost.UseCustomDialog)
            {
                await GHFileDialogHost.SaveCopyAsync(title, file.FullPath, file.FileName);
                return;
            }

            AvTopLevel topLevel = GetTopLevel();
            AvFilePickerSaveOptions options = new AvFilePickerSaveOptions();
            options.Title = title;
            options.SuggestedFileName = file.FileName;
            options.ShowOverwritePrompt = true;
            options.SuggestedStartLocation = await GetStartFolderAsync(topLevel);

            using (AvIStorageFile target = await topLevel.StorageProvider.SaveFilePickerAsync(options))
            {
                if (target == null)
                    return;

                string targetPath = AvStorageProviderExtensions.TryGetLocalPath(target);
                if (targetPath == null)
                {
                    await CopyFileAsync(file.FullPath, target);
                }
                else
                {
                    if (GHFileDialogLogic.IsProtectedTarget(targetPath, GHFileDialogHost.ProtectedFolders, file.FullPath))
                        throw new IOException("GnollHack does not save copies over its own files or over the file being shared.");
                    string sourcePath = file.FullPath;
                    await Task.Run(() => GHFileDialogLogic.CopyReplacing(sourcePath, targetPath));
                }
            }
        }

        /* The user's documents folder; null, which leaves the choice to the dialog, when it cannot be had */
        private static async Task<AvIStorageFolder> GetStartFolderAsync(AvTopLevel topLevel)
        {
            try
            {
                string documents = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);
                if (!string.IsNullOrEmpty(documents) && Directory.Exists(documents))
                    return await AvStorageProviderExtensions.TryGetFolderFromPathAsync(topLevel.StorageProvider, documents);
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHAvaloniaShare: no start folder: " + ex.Message);
            }
            return null;
        }

        private static async Task SaveFilesOnUIThreadAsync(string title, List<ShareFile> files)
        {
            if (GHFileDialogHost.UseCustomDialog)
            {
                foreach (ShareFile file in files)
                    await GHFileDialogHost.SaveCopyAsync(title, file.FullPath, file.FileName);
                return;
            }

            AvTopLevel topLevel = GetTopLevel();
            AvFolderPickerOpenOptions options = new AvFolderPickerOpenOptions();
            options.Title = title;
            options.AllowMultiple = false;

            IReadOnlyList<AvIStorageFolder> folders = await topLevel.StorageProvider.OpenFolderPickerAsync(options);
            if (folders == null || folders.Count == 0)
                return;

            try
            {
                AvIStorageFolder folder = folders[0];
                string folderPath = AvStorageProviderExtensions.TryGetLocalPath(folder);
                foreach (ShareFile file in files)
                {
                    if (folderPath != null)
                    {
                        if (GHFileDialogLogic.IsProtectedTarget(Path.Combine(folderPath, file.FileName), GHFileDialogHost.ProtectedFolders, file.FullPath))
                            throw new IOException("GnollHack does not save copies over its own files or over the file being shared.");
                        string sourcePath = file.FullPath;
                        string fileName = file.FileName;
                        await Task.Run(() => GHFileDialogLogic.CopyNoClobber(sourcePath, folderPath, fileName));
                        continue;
                    }

                    using (AvIStorageFile target = await folder.CreateFileAsync(file.FileName))
                    {
                        if (target == null)
                            throw new IOException("Could not create " + file.FileName + " in the chosen folder.");

                        await CopyFileAsync(file.FullPath, target);
                    }
                }
            }
            finally
            {
                foreach (AvIStorageFolder folder in folders)
                    folder?.Dispose();
            }
        }

        /* Replaces the contents of target with the contents of the file at sourcePath */
        private static async Task CopyFileAsync(string sourcePath, AvIStorageFile target)
        {
            using (FileStream source = File.OpenRead(sourcePath))
            using (Stream destination = await target.OpenWriteAsync())
            {
                await source.CopyToAsync(destination);
                if (destination.CanSeek)
                    destination.SetLength(destination.Position);
                await destination.FlushAsync();
            }
        }
    }
}
