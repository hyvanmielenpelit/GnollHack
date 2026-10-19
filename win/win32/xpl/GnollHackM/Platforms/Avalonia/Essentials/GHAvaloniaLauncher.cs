using System;
using System.Diagnostics;
using System.IO;
using System.Threading.Tasks;
using Microsoft.Maui.ApplicationModel;

using AvDispatcher = Avalonia.Threading.Dispatcher;
using AvLauncherExtensions = Avalonia.Platform.Storage.LauncherExtensions;
using AvTopLevel = Avalonia.Controls.TopLevel;

namespace GnollHackM
{
    /* ILauncher for the Avalonia desktop target, through the main window's Avalonia launcher on the
       Avalonia UI thread. http, https, mailto and file URIs can be opened; files open in their
       default application. OpenAsync throws InvalidOperationException when no window is open, while
       TryOpenAsync returns false on any failure. */
    public sealed class GHAvaloniaLauncher : ILauncher
    {
        public Task<bool> CanOpenAsync(Uri uri)
        {
            if (uri == null)
                throw new ArgumentNullException(nameof(uri));

            return Task.FromResult(IsSupported(uri));
        }

        public Task<bool> OpenAsync(Uri uri)
        {
            if (uri == null)
                throw new ArgumentNullException(nameof(uri));

            return AvDispatcher.UIThread.InvokeAsync(() => LaunchUriOnUIThreadAsync(uri));
        }

        public Task<bool> OpenAsync(OpenFileRequest request)
        {
            if (request == null)
                throw new ArgumentNullException(nameof(request));
            if (request.File == null || string.IsNullOrWhiteSpace(request.File.FullPath))
                throw new ArgumentException("The request does not name a file to open.", nameof(request));

            FileInfo fileInfo = new FileInfo(request.File.FullPath);
            return AvDispatcher.UIThread.InvokeAsync(() => LaunchFileOnUIThreadAsync(fileInfo));
        }

        public async Task<bool> TryOpenAsync(Uri uri)
        {
            if (uri == null)
                throw new ArgumentNullException(nameof(uri));
            if (!IsSupported(uri))
                return false;

            try
            {
                return await OpenAsync(uri);
            }
            catch (Exception ex)
            {
                Debug.WriteLine("GHAvaloniaLauncher: opening " + uri + " failed: " + ex.Message);
                return false;
            }
        }

        private static bool IsSupported(Uri uri)
        {
            if (!uri.IsAbsoluteUri)
                return false;

            string scheme = uri.Scheme;
            return string.Equals(scheme, Uri.UriSchemeHttp, StringComparison.OrdinalIgnoreCase)
                || string.Equals(scheme, Uri.UriSchemeHttps, StringComparison.OrdinalIgnoreCase)
                || string.Equals(scheme, Uri.UriSchemeMailto, StringComparison.OrdinalIgnoreCase)
                || string.Equals(scheme, Uri.UriSchemeFile, StringComparison.OrdinalIgnoreCase);
        }

        private static AvTopLevel GetTopLevel()
        {
            AvTopLevel topLevel = AvTopLevel.GetTopLevel(GHAvaloniaWindowHooks.MainWindow);
            if (topLevel == null)
                throw new InvalidOperationException("No window is open to launch from.");
            return topLevel;
        }

        private static async Task<bool> LaunchUriOnUIThreadAsync(Uri uri)
        {
            AvTopLevel topLevel = GetTopLevel();
            return await topLevel.Launcher.LaunchUriAsync(uri);
        }

        /* Returns false when the file does not exist */
        private static async Task<bool> LaunchFileOnUIThreadAsync(FileInfo fileInfo)
        {
            AvTopLevel topLevel = GetTopLevel();
            return await AvLauncherExtensions.LaunchFileInfoAsync(topLevel.Launcher, fileInfo);
        }
    }
}
