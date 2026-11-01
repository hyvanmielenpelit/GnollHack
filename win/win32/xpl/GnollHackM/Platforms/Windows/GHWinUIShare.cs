using System;
using System.Threading.Tasks;
using GnollHackX;
using Microsoft.Maui.ApplicationModel;
using Microsoft.Maui.ApplicationModel.DataTransfer;

namespace GnollHackM
{
    /* IShare for the WinUI 3 build: a file request saves a copy through GHFileDialogHost's control when Windows' share UI
       cannot be used with a controller, else it goes to the implementation it wraps, as text always does. */
    public sealed class GHWinUIShare : IShare
    {
        private const string DefaultTitle = "Save a Copy";

        private readonly IShare _systemShare;

        public GHWinUIShare(IShare systemShare)
        {
            _systemShare = systemShare;
        }

        public Task RequestAsync(ShareTextRequest request)
        {
            return _systemShare.RequestAsync(request);
        }

        public Task RequestAsync(ShareFileRequest request)
        {
            ArgumentNullException.ThrowIfNull(request);
            ArgumentNullException.ThrowIfNull(request.File);
            return MainThread.InvokeOnMainThreadAsync(async () =>
            {
                if (GHFileDialogHost.UseCustomDialog)
                    await GHFileDialogHost.SaveCopyAsync(string.IsNullOrEmpty(request.Title) ? DefaultTitle : request.Title, request.File.FullPath, request.File.FileName);
                else
                    await _systemShare.RequestAsync(request);
            });
        }

        public Task RequestAsync(ShareMultipleFilesRequest request)
        {
            ArgumentNullException.ThrowIfNull(request);
            ArgumentNullException.ThrowIfNull(request.Files);
            return MainThread.InvokeOnMainThreadAsync(async () =>
            {
                if (GHFileDialogHost.UseCustomDialog)
                {
                    foreach (ShareFile file in request.Files)
                        await GHFileDialogHost.SaveCopyAsync(string.IsNullOrEmpty(request.Title) ? DefaultTitle : request.Title, file.FullPath, file.FileName);
                }
                else
                {
                    await _systemShare.RequestAsync(request);
                }
            });
        }
    }
}
