#if GNH_AVALONIA
using System;
using System.IO;
using GnollHackX;
using Microsoft.Maui.ApplicationModel;
using Microsoft.Maui.Controls;
using Microsoft.Maui.Storage;
using AvOverlayLayer = Avalonia.Controls.Primitives.OverlayLayer;
using AvWindow = Avalonia.Controls.Window;
using AvWindowState = Avalonia.Controls.WindowState;

namespace GnollHackM
{
    /* Avalonia window lifecycle hooks, registered through ACM's AddWindows lifecycle builder */
    public static class GHAvaloniaWindowHooks
    {
        private static readonly TimeSpan FrameReportInterval = TimeSpan.FromSeconds(10);

        /* The app's window from creation until it closes; UI thread */
        public static AvWindow MainWindow { get; private set; }

        /* Called before the window is shown */
        public static void OnWindowCreated(AvWindow window)
        {
            if (window == null)
                return;

            MainWindow = window;

            GHAvaloniaKeyboard.Attach(window);
            GHAvaloniaFocusGuard.Attach(window);
            GHAvaloniaKeyLog.Attach(window);
            GHAvaloniaPointerLog.Attach(window);
            GHAvaloniaGamepad.Attach(window);

            Avalonia.Controls.WindowIcon icon = LoadWindowIcon();
            if (icon != null)
                window.Icon = icon;
#if MACOS
            Avalonia.Controls.NativeMenu.SetMenu(window, GHMacApplicationMenu.CreateWindowMenu());
#endif
#if WINDOWS_AVALONIA
            GHAvaloniaWindowsKeyHook.Start(window);
#endif

            /* Windowed Mode is read at startup; a changed setting applies after a restart, as on WinUI */
            bool fullScreen = GHApp.IsFullScreenForced || !Preferences.Get("WindowedMode", false);
            if (fullScreen)
            {
                window.WindowState = AvWindowState.FullScreen;
            }
            else
            {
                RestoreWindowedBounds(window);
                window.Closing += (sender, e) => SaveWindowedBounds(window);
            }
            window.Closing += OnWindowClosing;

            /* Writes the frame loop report to the log while a game is running */
            Avalonia.Threading.DispatcherTimer reportTimer = new Avalonia.Threading.DispatcherTimer();
            reportTimer.Interval = FrameReportInterval;
            reportTimer.Tick += (sender, e) =>
            {
                string glReport;
                string report = GHAvaloniaFrameLoop.GetReport(out glReport);
                if (GHApp.CurrentGamePage != null)
                {
                    GHApp.MaybeWriteGHLog(report);
                    GHApp.MaybeWriteGHLog(glReport);
                }
            };

            window.Opened += (sender, e) =>
            {
                GHAvaloniaDeviceDisplay.Current?.OnWindowOpened(window);
                if (OperatingSystem.IsWindows())
                    AttachPresentKeepAlive(window);
#if WINDOWS_AVALONIA
                AttachFullScreenRefit(window);
#endif
                GHAvaloniaFrameLoop.Start(window);
                reportTimer.Start();
            };

            window.Closed += (sender, e) =>
            {
                reportTimer.Stop();
                GHAvaloniaFrameLoop.Stop();
                GHAvaloniaFrameLoop.SetPresentKeepAlive(null);
#if WINDOWS_AVALONIA
                GHAvaloniaWindowsKeyHook.Stop();
                DetachFullScreenRefit(window);
#endif
                GHAvaloniaKeyboard.Detach(window);
                GHAvaloniaFocusGuard.Detach(window);
                GHAvaloniaKeyLog.Detach(window);
                GHAvaloniaPointerLog.Detach(window);
                GHAvaloniaGamepad.Detach(window);
#if LINUX
                GHCefRuntime.CloseAllBrowsersIfInitialized();
#endif
                if (ReferenceEquals(MainWindow, window))
                    MainWindow = null;
            };
        }

        /* Windows: a control in the window's overlay layer that the frame loop pulses while Steam's overlay is in the
           process, so that Avalonia keeps presenting frames for the overlay to draw on */
        private static void AttachPresentKeepAlive(AvWindow window)
        {
            try
            {
                AvOverlayLayer layer = AvOverlayLayer.GetOverlayLayer(window);
                if (layer == null)
                {
                    GHApp.MaybeWriteGHLog("GHAvaloniaWindowHooks: no overlay layer for the present keep-alive");
                    return;
                }
                GHAvaloniaPresentKeepAlive keepAlive = new GHAvaloniaPresentKeepAlive();
                layer.Children.Add(keepAlive);
                GHAvaloniaFrameLoop.SetPresentKeepAlive(keepAlive);
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHAvaloniaWindowHooks: cannot attach the present keep-alive: " + ex.Message);
            }
        }

#if WINDOWS_AVALONIA
        private static Avalonia.Controls.Screens _refitScreens = null;
        private static bool _fullScreenRefitQueued = false;
        private static bool _fullScreenRefitPending = false;

        // WORKAROUND(WA-AVALONIA-0027): A full-screen window keeps its size after a display mode change
        /* Checked when the window opens, when the screens change and, if a change came while the window was inactive,
           on its next activation. Setting WindowState activates the window, so an inactive window is not refitted
           until it is active again: after Alt+Tab the game reverts the screen resolution as it loses focus. */
        private static void AttachFullScreenRefit(AvWindow window)
        {
            try
            {
                _refitScreens = window.Screens;
                if (_refitScreens != null)
                    _refitScreens.Changed += OnScreensChanged;
                window.Activated += OnRefitWindowActivated;
                QueueFullScreenRefit();
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHAvaloniaWindowHooks: cannot watch the screens: " + ex.Message);
            }
        }

        private static void DetachFullScreenRefit(AvWindow window)
        {
            if (_refitScreens != null)
                _refitScreens.Changed -= OnScreensChanged;
            _refitScreens = null;
            window.Activated -= OnRefitWindowActivated;
            _fullScreenRefitPending = false;
        }

        private static void OnScreensChanged(object sender, EventArgs e)
        {
            QueueFullScreenRefit();
        }

        private static void OnRefitWindowActivated(object sender, EventArgs e)
        {
            if (_fullScreenRefitPending)
                QueueFullScreenRefit();
        }

        /* Posted, so that the check runs after Avalonia has updated its screen list */
        private static void QueueFullScreenRefit()
        {
            if (_fullScreenRefitQueued)
                return;
            _fullScreenRefitQueued = true;
            Avalonia.Threading.Dispatcher.UIThread.Post(() =>
            {
                _fullScreenRefitQueued = false;
                RefitFullScreen(MainWindow);
            }, Avalonia.Threading.DispatcherPriority.Background);
        }

        /* Avalonia sizes a full-screen window to its screen's bounds only when it enters full screen; leaving and
           re-entering full screen makes it take the screen's current bounds */
        private static void RefitFullScreen(AvWindow window)
        {
            try
            {
                if (window == null || window.WindowState != AvWindowState.FullScreen)
                {
                    _fullScreenRefitPending = false;
                    return;
                }
                Avalonia.Platform.Screen screen = window.Screens?.ScreenFromWindow(window);
                if (screen == null)
                    return;

                Avalonia.PixelRect bounds = screen.Bounds;
                double scaling = window.DesktopScaling;
                int width = (int)Math.Round(window.ClientSize.Width * scaling);
                int height = (int)Math.Round(window.ClientSize.Height * scaling);
                Avalonia.PixelPoint position = window.Position;
                if (Math.Abs(width - bounds.Width) <= 1 && Math.Abs(height - bounds.Height) <= 1
                    && position.X == bounds.X && position.Y == bounds.Y)
                {
                    _fullScreenRefitPending = false;
                    return;
                }
                if (!window.IsActive)
                {
                    _fullScreenRefitPending = true;
                    return;
                }

                _fullScreenRefitPending = false;
                GHApp.MaybeWriteGHLog("GHAvaloniaWindowHooks: full-screen window " + width + "x" + height + " at "
                    + position.X + "," + position.Y + " refitted to its screen " + bounds.Width + "x" + bounds.Height
                    + " at " + bounds.X + "," + bounds.Y);
                window.WindowState = AvWindowState.Normal;
                window.WindowState = AvWindowState.FullScreen;
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHAvaloniaWindowHooks: cannot refit the full-screen window: " + ex.Message);
            }
        }
#endif

        /* A close the user asked for goes through the game's own flow, as on Windows. Programmatic closes pass: they
           are the Exit button's shutdown. So does a close from the operating system, which cannot wait for a prompt;
           Main saves the game once the lifetime ends. A replay has nothing to save. Unlike Windows this does not
           depend on focus: a window manager can close an unfocused window. */
        private static void OnWindowClosing(object sender, Avalonia.Controls.WindowClosingEventArgs e)
        {
            if (e.IsProgrammatic || e.CloseReason != Avalonia.Controls.WindowCloseReason.WindowClosing)
                return;

            if (BeginUserClose(sender as AvWindow))
                e.Cancel = true;
        }

        /* A quit the user asked for from the macOS menu (Cmd+Q, Cmd+W): the same flow as the window's close button.
           Without a game or main page to take it over, as in a replay, the application shuts down. */
        public static void RequestQuit()
        {
            try
            {
                if (BeginUserClose(MainWindow))
                    return;
                (Avalonia.Application.Current?.ApplicationLifetime as Avalonia.Controls.ApplicationLifetimes.IClassicDesktopStyleApplicationLifetime)?.TryShutdown();
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog(ex.Message);
            }
        }

        /* Starts the game's own close flow; true when it took the close over, so the window stays open for now */
        private static bool BeginUserClose(AvWindow window)
        {
            try
            {
                GamePage curGamePage = GHApp.CurrentGamePage;
                if (curGamePage != null)
                {
                    if (curGamePage.PlayingReplay)
                        return false;
                    window?.Activate();
                    MainThread.InvokeOnMainThreadAsync(async () =>
                    {
                        try
                        {
                            Page topPage = GHApp.PageFromTopOfModalNavigationStack();
                            if (topPage is NamePage)
                            {
                                await ((NamePage)topPage).DoPressCancel();
                            }
                            else
                            {
                                await GHApp.PopAllModalPagesAboveGamePageAsync();
                                curGamePage.CloseMoreCommands();
                                curGamePage.PressCharForSaving();
                            }
                        }
                        catch (Exception ex)
                        {
                            GHApp.MaybeWriteGHLog(ex.Message);
                        }
                    });
                    return true;
                }

                MainPage curMainPage = GHApp.CurrentMainPage;
                if (curMainPage != null)
                {
                    window?.Activate();
                    MainThread.InvokeOnMainThreadAsync(async () =>
                    {
                        try
                        {
                            await curMainPage.CheckPendingOrPressOk();
                        }
                        catch (Exception ex)
                        {
                            GHApp.MaybeWriteGHLog(ex.Message);
                        }
                    });
                    return true;
                }
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog(ex.Message);
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
