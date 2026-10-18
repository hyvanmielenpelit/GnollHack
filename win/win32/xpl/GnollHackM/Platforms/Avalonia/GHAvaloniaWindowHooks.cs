#if GNH_AVALONIA
using System;
using System.IO;
using GnollHackX;
using Microsoft.Maui.Storage;
using AvWindow = Avalonia.Controls.Window;
using AvWindowState = Avalonia.Controls.WindowState;

namespace GnollHackM
{
    /* Avalonia window lifecycle hooks, registered through ACM's AddWindows lifecycle builder */
    public static class GHAvaloniaWindowHooks
    {
        private static readonly TimeSpan FrameReportInterval = TimeSpan.FromSeconds(10);

        /* Called before the window is shown */
        public static void OnWindowCreated(AvWindow window)
        {
            if (window == null)
                return;

            GHAvaloniaKeyboard.Attach(window);

            Avalonia.Controls.WindowIcon icon = LoadWindowIcon();
            if (icon != null)
                window.Icon = icon;

            /* Windowed Mode is read at startup; a changed setting applies after a restart, as on WinUI */
            bool fullScreen = IsFullScreenRequested() || !Preferences.Get("WindowedMode", false);
            if (fullScreen)
            {
                window.WindowState = AvWindowState.FullScreen;
            }
            else
            {
                RestoreWindowedBounds(window);
                window.Closing += (sender, e) => SaveWindowedBounds(window);
            }

            /* Writes the frame loop report to the log while a game is running */
            Avalonia.Threading.DispatcherTimer reportTimer = new Avalonia.Threading.DispatcherTimer();
            reportTimer.Interval = FrameReportInterval;
            reportTimer.Tick += (sender, e) =>
            {
                string report = GHAvaloniaFrameLoop.GetReport();
                if (GHApp.CurrentGamePage != null)
                    GHApp.MaybeWriteGHLog(report);
            };

            window.Opened += (sender, e) =>
            {
                GHAvaloniaDeviceDisplay.Current?.OnWindowOpened(window);
                GHAvaloniaFrameLoop.Start(window);
                reportTimer.Start();
            };

            window.Closed += (sender, e) =>
            {
                reportTimer.Stop();
                GHAvaloniaFrameLoop.Stop();
                GHAvaloniaKeyboard.Detach(window);
            };
        }

        /* Steam's Gaming Mode sets SteamGamepadUI=1 */
        private static bool IsFullScreenRequested()
        {
            if (Environment.GetEnvironmentVariable("SteamGamepadUI") == "1")
                return true;

            string[] args = Environment.GetCommandLineArgs();
            if (args != null)
            {
                foreach (string arg in args)
                {
                    if (string.Equals(arg, "--fullscreen", StringComparison.OrdinalIgnoreCase))
                        return true;
                }
            }
            return false;
        }

        /* Windowed bounds in this flavor's own preferences: position in physical pixels (Window.Position),
           size in device-independent units (Window.Width and Height) */
        private static void RestoreWindowedBounds(AvWindow window)
        {
            try
            {
                int x = Preferences.Get("WindowedSizeX", 0);
                int y = Preferences.Get("WindowedSizeY", 0);
                int width = Preferences.Get("WindowedSizeWidth", 0);
                int height = Preferences.Get("WindowedSizeHeight", 0);
                bool maximized = Preferences.Get("WindowedSizeIsMaximized", false);

                bool onSomeScreen = false;
                Avalonia.Controls.Screens screens = window.Screens;
                if (width > 0 && height > 0 && screens != null)
                {
                    Avalonia.PixelPoint position = new Avalonia.PixelPoint(x, y);
                    double scaling = screens.ScreenFromPoint(position)?.Scaling ?? 1.0;
                    Avalonia.PixelRect bounds = new Avalonia.PixelRect(x, y, (int)(width * scaling), (int)(height * scaling));
                    foreach (Avalonia.Platform.Screen screen in screens.All)
                    {
                        if (screen.WorkingArea.Intersects(bounds))
                        {
                            onSomeScreen = true;
                            break;
                        }
                    }
                }

                if (onSomeScreen)
                {
                    window.WindowStartupLocation = Avalonia.Controls.WindowStartupLocation.Manual;
                    window.Position = new Avalonia.PixelPoint(x, y);
                    window.Width = width;
                    window.Height = height;
                }
                else
                {
                    window.WindowStartupLocation = Avalonia.Controls.WindowStartupLocation.CenterScreen;
                }

                if (maximized)
                    window.WindowState = AvWindowState.Maximized;
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHAvaloniaWindowHooks: cannot restore the window bounds: " + ex.Message);
            }
        }

        private static void SaveWindowedBounds(AvWindow window)
        {
            try
            {
                AvWindowState state = window.WindowState;
                if (state == AvWindowState.FullScreen)
                    return;

                bool maximized = state == AvWindowState.Maximized;
                Preferences.Set("WindowedSizeIsMaximized", maximized);
                if (!maximized && state != AvWindowState.Minimized)
                {
                    Preferences.Set("WindowedSizeX", window.Position.X);
                    Preferences.Set("WindowedSizeY", window.Position.Y);
                    Preferences.Set("WindowedSizeWidth", (int)Math.Round(window.Width));
                    Preferences.Set("WindowedSizeHeight", (int)Math.Round(window.Height));
                }
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHAvaloniaWindowHooks: cannot save the window bounds: " + ex.Message);
            }
        }

        /* Windows: an icon file with the sizes Windows asks for, since it picks the matching entry but scales a
           single large image with point sampling. Elsewhere the PNG, which the window manager scales. */
        private static Avalonia.Controls.WindowIcon LoadWindowIcon()
        {
            string uri = "avares://" + typeof(GHAvaloniaWindowHooks).Assembly.GetName().Name + "/Assets/AppIcon/appicon.png";
            try
            {
                byte[] png;
                using (Stream stream = Avalonia.Platform.AssetLoader.Open(new Uri(uri)))
                using (MemoryStream memory = new MemoryStream())
                {
                    stream.CopyTo(memory);
                    png = memory.ToArray();
                }

                byte[] iconData = OperatingSystem.IsWindows() ? CreateIconFile(png) : png;
                using (MemoryStream iconStream = new MemoryStream(iconData))
                {
                    return new Avalonia.Controls.WindowIcon(iconStream);
                }
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHAvaloniaWindowHooks: cannot load the window icon: " + ex.Message);
                return null;
            }
        }

        private static readonly int[] IconSizes = { 16, 20, 24, 32, 40, 48, 64, 96, 128, 256 };

        /* An .ico file of PNG entries, each scaled from the source with linear filtering between mipmap levels */
        private static byte[] CreateIconFile(byte[] png)
        {
            byte[][] entries = new byte[IconSizes.Length][];
            using (SkiaSharp.SKImage source = SkiaSharp.SKImage.FromEncodedData(png))
            {
                if (source == null)
                    return png;

                SkiaSharp.SKSamplingOptions sampling = new SkiaSharp.SKSamplingOptions(SkiaSharp.SKFilterMode.Linear, SkiaSharp.SKMipmapMode.Linear);
                int i;
                for (i = 0; i < IconSizes.Length; i++)
                {
                    int size = IconSizes[i];
                    SkiaSharp.SKImageInfo info = new SkiaSharp.SKImageInfo(size, size, SkiaSharp.SKColorType.Bgra8888, SkiaSharp.SKAlphaType.Premul);
                    using (SkiaSharp.SKSurface surface = SkiaSharp.SKSurface.Create(info))
                    {
                        surface.Canvas.Clear(SkiaSharp.SKColors.Transparent);
                        surface.Canvas.DrawImage(source, new SkiaSharp.SKRect(0, 0, size, size), sampling);
                        using (SkiaSharp.SKImage scaled = surface.Snapshot())
                        using (SkiaSharp.SKData data = scaled.Encode(SkiaSharp.SKEncodedImageFormat.Png, 100))
                        {
                            entries[i] = data.ToArray();
                        }
                    }
                }
            }

            using (MemoryStream ico = new MemoryStream())
            using (BinaryWriter writer = new BinaryWriter(ico))
            {
                writer.Write((ushort)0); /* Reserved */
                writer.Write((ushort)1); /* Icon */
                writer.Write((ushort)IconSizes.Length);
                int offset = 6 + 16 * IconSizes.Length;
                int i;
                for (i = 0; i < IconSizes.Length; i++)
                {
                    int size = IconSizes[i];
                    writer.Write((byte)(size >= 256 ? 0 : size)); /* 0 means 256 */
                    writer.Write((byte)(size >= 256 ? 0 : size));
                    writer.Write((byte)0); /* No palette */
                    writer.Write((byte)0);
                    writer.Write((ushort)1); /* Planes */
                    writer.Write((ushort)32); /* Bits per pixel */
                    writer.Write(entries[i].Length);
                    writer.Write(offset);
                    offset += entries[i].Length;
                }
                for (i = 0; i < IconSizes.Length; i++)
                    writer.Write(entries[i]);
                writer.Flush();
                return ico.ToArray();
            }
        }
    }
}
#endif
