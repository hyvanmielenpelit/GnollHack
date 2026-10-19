using System;
using System.ComponentModel;
using System.Diagnostics;
using System.Threading.Tasks;
using Microsoft.Maui.ApplicationModel;

using AvDispatcher = Avalonia.Threading.Dispatcher;
using AvTopLevel = Avalonia.Controls.TopLevel;

namespace GnollHackM
{
    /* IBrowser for the Avalonia desktop target. Pages open in the system browser through the main
       window's Avalonia launcher, on the Avalonia UI thread; the launch options are not used. Throws
       InvalidOperationException when no window is open. On Linux, where the launcher runs xdg-open,
       gio open and the BROWSER variable are tried next, and InvalidOperationException is thrown when
       none of them can be started. On Linux with the Steam overlay in the process, a web page opens
       in Steam's own browser through a steam://openurl link instead. */
    public sealed class GHAvaloniaBrowser : IBrowser
    {
        private const string SteamOverlayLibrary = "gameoverlayrenderer.so";

        private static readonly Lazy<bool> _hasSteamOverlay = new Lazy<bool>(DetectSteamOverlay);

        /* Whether Steam has injected its overlay into this process; Linux only */
        public static bool HasSteamOverlay
        {
            get { return _hasSteamOverlay.Value; }
        }

        public Task<bool> OpenAsync(Uri uri, BrowserLaunchOptions options)
        {
            if (uri == null)
                throw new ArgumentNullException(nameof(uri));

            return AvDispatcher.UIThread.InvokeAsync(() => LaunchOnUIThreadAsync(uri));
        }

        private static async Task<bool> LaunchOnUIThreadAsync(Uri uri)
        {
            AvTopLevel topLevel = AvTopLevel.GetTopLevel(GHAvaloniaWindowHooks.MainWindow);
            if (topLevel == null)
                throw new InvalidOperationException("No window is open to launch the browser from.");

            if (OperatingSystem.IsLinux() && HasSteamOverlay && uri.IsAbsoluteUri && (uri.Scheme == Uri.UriSchemeHttp || uri.Scheme == Uri.UriSchemeHttps))
            {
                Uri steamUri = new Uri("steam://openurl/" + uri.AbsoluteUri);
                if (await topLevel.Launcher.LaunchUriAsync(steamUri))
                    return true;
                GnollHackX.GHApp.MaybeWriteGHLog("GHAvaloniaBrowser: cannot open " + steamUri + "; trying the system browser");
            }

            if (await topLevel.Launcher.LaunchUriAsync(uri))
                return true;

            if (!OperatingSystem.IsLinux() || !uri.IsAbsoluteUri)
                return false;

            if (TryStart("gio", "open", uri.AbsoluteUri))
                return true;

            string browser = Environment.GetEnvironmentVariable("BROWSER");
            if (!string.IsNullOrWhiteSpace(browser))
            {
                foreach (string command in browser.Split(':', StringSplitOptions.RemoveEmptyEntries | StringSplitOptions.TrimEntries))
                {
                    if (TryStart(command, null, uri.AbsoluteUri))
                        return true;
                }
            }

            throw new InvalidOperationException("No program to open web pages was found. Install xdg-utils, or set the BROWSER environment variable.");
        }

        /* Steam's Linux overlay is a library Steam preloads into the game; it shows in LD_PRELOAD or, once loaded,
           in the process's memory map */
        private static bool DetectSteamOverlay()
        {
            if (!OperatingSystem.IsLinux())
                return false;

            try
            {
                string preload = Environment.GetEnvironmentVariable("LD_PRELOAD");
                bool found = preload != null && preload.Contains(SteamOverlayLibrary, StringComparison.Ordinal);
                if (!found)
                {
                    foreach (string line in System.IO.File.ReadLines("/proc/self/maps"))
                    {
                        if (line.Contains(SteamOverlayLibrary, StringComparison.Ordinal))
                        {
                            found = true;
                            break;
                        }
                    }
                }
                GnollHackX.GHApp.MaybeWriteGHLog("GHAvaloniaBrowser: Steam overlay " + (found ? "detected" : "not detected"));
                return found;
            }
            catch (Exception ex)
            {
                GnollHackX.GHApp.MaybeWriteGHLog("GHAvaloniaBrowser: cannot check for the Steam overlay: " + ex.Message);
                return false;
            }
        }

        /* Whether the program started; it is not waited for */
        private static bool TryStart(string fileName, string verb, string url)
        {
            try
            {
                ProcessStartInfo startInfo = new ProcessStartInfo
                {
                    FileName = fileName,
                    CreateNoWindow = true,
                    UseShellExecute = false,
                };
                if (verb != null)
                    startInfo.ArgumentList.Add(verb);
                startInfo.ArgumentList.Add(url);
                using (Process.Start(startInfo))
                {
                    return true;
                }
            }
            catch (Win32Exception)
            {
                return false;
            }
            catch (InvalidOperationException)
            {
                return false;
            }
        }
    }
}
