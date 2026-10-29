#if GNH_AVALONIA
using System;
using System.ComponentModel;
using System.Diagnostics;
using System.Globalization;
using System.Threading.Tasks;
using GnollHackX;
using Microsoft.Maui;
using AvControl = Avalonia.Controls.Control;
using AvTopLevel = Avalonia.Controls.TopLevel;
using MauiView = Microsoft.Maui.Controls.View;

namespace GnollHackM
{
    /* Steam's on-screen keyboard for the Avalonia build, opened with a steam://open/keyboard link placed away from
       the field and closed with steam://close/keyboard; the typed text arrives as ordinary key events. Available on
       Linux on a Steam Deck in Game Mode. GNOLLHACK_STEAM_KEYBOARD: auto (default), on (any Linux session) or off.
       GNOLLHACK_STEAM_KEYBOARD_LAUNCH: xdg-open (default), steam, or launcher (the window's Avalonia launcher).
       Programs start without LD_PRELOAD, which carries Steam's overlay library, and are never waited for. Both
       variables are read once. UI thread only. */
    internal sealed class GHAvaloniaSteamKeyboard : IGHScreenKeyboardProvider
    {
        private const string OpenUrl = "steam://open/keyboard";
        private const string CloseUrl = "steam://close/keyboard";
        private const string LaunchXdgOpen = "xdg-open";
        private const string LaunchSteam = "steam";
        private const string LaunchLauncher = "launcher";

        private static readonly Lazy<bool> _isAvailable = new Lazy<bool>(DecideAvailability);
        private static readonly Lazy<string> _launchMode = new Lazy<string>(ReadLaunchMode);

        /* Sets GHScreenKeyboard.Provider */
        public static void Register()
        {
            GHScreenKeyboard.Provider = new GHAvaloniaSteamKeyboard();
        }

        public bool IsAvailable
        {
            get { return _isAvailable.Value; }
        }

        /* True when the launch was started; the position is left out when the field's rectangle is unknown */
        public bool Show(MauiView field, GHScreenKeyboardMode mode)
        {
            int x, y, width, height;
            bool hasRect = TryGetFieldPixelRect(field, out x, out y, out width, out height);
            return LaunchOpen(hasRect, x, y, width, height, mode);
        }

        /* As Show; the host's whole rectangle is used when the area lies outside the host */
        public bool ShowForArea(MauiView host, double x, double y, double width, double height,
            GHScreenKeyboardMode mode)
        {
            int pixelX, pixelY, pixelWidth, pixelHeight;
            bool hasRect = TryGetFieldPixelRect(host, x, y, width, height,
                    out pixelX, out pixelY, out pixelWidth, out pixelHeight)
                || TryGetFieldPixelRect(host, out pixelX, out pixelY, out pixelWidth, out pixelHeight);
            return LaunchOpen(hasRect, pixelX, pixelY, pixelWidth, pixelHeight, mode);
        }

        public void Hide()
        {
            Launch(CloseUrl);
        }

        /* The open link, with the position only when hasRect */
        private static bool LaunchOpen(bool hasRect, int x, int y, int width, int height, GHScreenKeyboardMode mode)
        {
            string url;
            if (hasRect)
            {
                url = string.Format(CultureInfo.InvariantCulture, "{0}?XPosition={1}&YPosition={2}&Width={3}&Height={4}&Mode={5}",
                    OpenUrl, x, y, width, height, (int)mode);
            }
            else
            {
                url = string.Format(CultureInfo.InvariantCulture, "{0}?Mode={1}", OpenUrl, (int)mode);
            }
            return Launch(url);
        }

        /* The field's bounds in the top-level window, in physical pixels */
        private static bool TryGetFieldPixelRect(MauiView field, out int x, out int y, out int width, out int height)
        {
            return TryGetFieldPixelRect(field, 0, 0, double.PositiveInfinity, double.PositiveInfinity,
                out x, out y, out width, out height);
        }

        /* The area areaX, areaY, areaWidth, areaHeight of the field, in DIPs from its top-left and clamped to its
           bounds, in the top-level window, in physical pixels; false when the clamped area is empty */
        private static bool TryGetFieldPixelRect(MauiView field, double areaX, double areaY, double areaWidth,
            double areaHeight, out int x, out int y, out int width, out int height)
        {
            x = 0;
            y = 0;
            width = 0;
            height = 0;
            if (field == null)
                return false;

            AvControl control;
            try
            {
                IElementHandler handler = field.Handler;
                control = handler?.PlatformView as AvControl;
            }
            catch (InvalidOperationException)
            {
                return false;
            }
            if (control == null)
                return false;

            AvTopLevel topLevel = AvTopLevel.GetTopLevel(control);
            if (topLevel == null)
                return false;
            Avalonia.Point? origin = Avalonia.VisualExtensions.TranslatePoint(control, new Avalonia.Point(0, 0), topLevel);
            if (!origin.HasValue)
                return false;
            Avalonia.Size size = control.Bounds.Size;
            if (size.Width <= 0 || size.Height <= 0)
                return false;

            double right = areaX + areaWidth;
            double bottom = areaY + areaHeight;
            if (double.IsNaN(areaX) || double.IsNaN(areaY) || double.IsNaN(right) || double.IsNaN(bottom))
                return false;
            double left = Math.Clamp(areaX, 0, size.Width);
            double top = Math.Clamp(areaY, 0, size.Height);
            right = Math.Clamp(right, 0, size.Width);
            bottom = Math.Clamp(bottom, 0, size.Height);
            if (right <= left || bottom <= top)
                return false;

            double scale = topLevel.RenderScaling;
            if (double.IsNaN(scale) || scale <= 0)
                scale = 1.0;
            x = (int)Math.Round((origin.Value.X + left) * scale);
            y = (int)Math.Round((origin.Value.Y + top) * scale);
            width = (int)Math.Round((right - left) * scale);
            height = (int)Math.Round((bottom - top) * scale);
            return width > 0 && height > 0;
        }

        private static bool Launch(string url)
        {
            string mode = _launchMode.Value;
            WriteLog("SCREENKEYBOARD launch " + url + " (" + mode + ")");
            if (mode == LaunchLauncher)
                return StartLauncher(url);
            return StartProcess(mode == LaunchSteam ? "steam" : "xdg-open", url);
        }

        private static bool StartProcess(string fileName, string url)
        {
            try
            {
                ProcessStartInfo startInfo = new ProcessStartInfo
                {
                    FileName = fileName,
                    CreateNoWindow = true,
                    UseShellExecute = false,
                };
                startInfo.ArgumentList.Add(url);
                startInfo.Environment.Remove("LD_PRELOAD");
                using (Process process = Process.Start(startInfo))
                {
                    return process != null;
                }
            }
            catch (Win32Exception ex)
            {
                WriteLog("SCREENKEYBOARD cannot start " + fileName + ": " + ex.Message);
                return false;
            }
            catch (InvalidOperationException ex)
            {
                WriteLog("SCREENKEYBOARD cannot start " + fileName + ": " + ex.Message);
                return false;
            }
        }

        private static bool StartLauncher(string url)
        {
            AvTopLevel topLevel = AvTopLevel.GetTopLevel(GHAvaloniaWindowHooks.MainWindow);
            if (topLevel == null)
            {
                WriteLog("SCREENKEYBOARD no window to launch from");
                return false;
            }
            _ = LaunchWithLauncherAsync(topLevel, new Uri(url));
            return true;
        }

        private static async Task LaunchWithLauncherAsync(AvTopLevel topLevel, Uri uri)
        {
            try
            {
                if (!await topLevel.Launcher.LaunchUriAsync(uri))
                    WriteLog("SCREENKEYBOARD the launcher did not open " + uri);
            }
            catch (Exception ex)
            {
                WriteLog("SCREENKEYBOARD the launcher failed: " + ex.GetType().Name + ": " + ex.Message);
            }
        }

        /* Linux only; auto means a Steam Deck in Game Mode. Desktop Mode is left out: the keyboard opens behind
           full-screen windows there. */
        private static bool DecideAvailability()
        {
            string setting = ReadSetting("GNOLLHACK_STEAM_KEYBOARD", "auto");
            string desktop = Environment.GetEnvironmentVariable("XDG_CURRENT_DESKTOP");
            string gamepadUI = Environment.GetEnvironmentVariable("SteamGamepadUI");
            bool available;
            if (!OperatingSystem.IsLinux())
                available = false;
            else if (setting == "on")
                available = true;
            else if (setting == "off")
                available = false;
            else
                available = GHApp.IsSteamDeck && IsGameMode(desktop, gamepadUI);
            WriteLog("SCREENKEYBOARD " + (available ? "available" : "not available") + " (GNOLLHACK_STEAM_KEYBOARD=" + setting
                + ", XDG_CURRENT_DESKTOP=" + (desktop ?? "<unset>") + ", SteamGamepadUI=" + (gamepadUI ?? "<unset>")
                + ", deck=" + (OperatingSystem.IsLinux() && GHApp.IsSteamDeck ? "yes" : "no") + ")");
            return available;
        }

        /* Steam's Game Mode runs games under gamescope and sets SteamGamepadUI */
        private static bool IsGameMode(string desktop, string gamepadUI)
        {
            if (desktop != null && string.Equals(desktop.Trim(), "gamescope", StringComparison.OrdinalIgnoreCase))
                return true;
            return !string.IsNullOrWhiteSpace(gamepadUI) && gamepadUI.Trim() != "0";
        }

        /* xdg-open unless the variable names another known mode */
        private static string ReadLaunchMode()
        {
            string setting = ReadSetting("GNOLLHACK_STEAM_KEYBOARD_LAUNCH", LaunchXdgOpen);
            if (setting == LaunchSteam || setting == LaunchLauncher)
                return setting;
            return LaunchXdgOpen;
        }

        private static string ReadSetting(string name, string defaultValue)
        {
            string value = Environment.GetEnvironmentVariable(name);
            return string.IsNullOrWhiteSpace(value) ? defaultValue : value.Trim().ToLowerInvariant();
        }

        private static void WriteLog(string line)
        {
            try
            {
                Console.Error.WriteLine(line);
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
            GHApp.WriteGHLog(line);
        }
    }
}
#endif
