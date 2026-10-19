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
       none of them can be started. */
    public sealed class GHAvaloniaBrowser : IBrowser
    {
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
