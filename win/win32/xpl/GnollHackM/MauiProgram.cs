using Microsoft.Extensions.Logging;
using Microsoft.Maui.LifecycleEvents;
using SkiaSharp.Views.Maui.Controls.Hosting;
using System.Runtime.Intrinsics.Arm;
using GnollHackX;

#if IOS
using GnollHackM.Platforms.iOS;
#endif

#if WINDOWS
using GnollHackM.Platforms.Windows;
using Microsoft.Maui.Platform;
#endif

#if SENTRY
using Sentry.Maui;
#if WINDOWS
using Sentry.Profiling;
using Microsoft.UI;
using Microsoft.UI.Windowing;
#endif
#endif

namespace GnollHackM;

public static class MauiProgram
{
	public static MauiApp CreateMauiApp()
	{
		var builder = MauiApp.CreateBuilder();
		builder
			.UseMauiApp<App>()
            .UseSkiaSharp()
            .ConfigureMauiHandlers((handlers) => {
                handlers.AddHandler(typeof(CustomLabel), typeof(AutoSizeSKCanvasViewHandler));
#if IOS
                handlers.AddHandler(typeof(Shell), typeof(CustomShellRenderer));
                handlers.AddHandler<Border, NotAnimatedBorderHandler>();
#endif
            })

#if SENTRY && !WINDOWS
            .UseSentry(options => {
                  // The DSN is the only required setting.
                  options.Dsn = "https://c45d9f5d2540eae9538cb9aa78eb25cd@o4507617242906624.ingest.de.sentry.io/4507617248608336";

                  // Use debug mode if you want to see what the SDK is doing.
                  // Debug messages are written to stdout with Console.Writeline,
                  // and are viewable in your IDE's debug console or with 'adb logcat', etc.
                  // This option is not recommended when deploying your application.
                  options.Debug = true;

                  // Set TracesSampleRate to 1.0 to capture 100% of transactions for performance monitoring.
                  // We recommend adjusting this value in production.
                  options.TracesSampleRate = 1.0;

                  // Sample rate for profiling, applied on top of othe TracesSampleRate,
                  // e.g. 0.2 means we want to profile 20 % of the captured transactions.
                  // We recommend adjusting this value in production.
                  options.ProfilesSampleRate = 1.0;

                  // Other Sentry options can be set here.
            })
#endif
#if SENTRY && WINDOWS
            // Add this section anywhere on the builder:
            .UseSentry(options =>
            {
                // The DSN is the only required setting.
                options.Dsn = "https://c45d9f5d2540eae9538cb9aa78eb25cd@o4507617242906624.ingest.de.sentry.io/4507617248608336";

                // Use debug mode if you want to see what the SDK is doing.
                // Debug messages are written to stdout with Console.Writeline,
                // and are viewable in your IDE's debug console or with 'adb logcat', etc.
                // This option is not recommended when deploying your application.
                options.Debug = true;

                // Set TracesSampleRate to 1.0 to capture 100% of transactions for performance monitoring.
                // We recommend adjusting this value in production.
                options.TracesSampleRate = 1.0;

                // Sample rate for profiling, applied on top of othe TracesSampleRate,
                // e.g. 0.2 means we want to profile 20 % of the captured transactions.
                // We recommend adjusting this value in production.
                options.ProfilesSampleRate = 1.0;

                // Requires NuGet package: Sentry.Profiling
                // Note: By default, the profiler is initialized asynchronously. This can
                // be tuned by passing a desired initialization timeout to the constructor.
                options.AddIntegration(new ProfilingIntegration(
                    // During startup, wait up to 500ms to profile the app startup code.
                    // This could make launching the app a bit slower so comment it out if you
                    // prefer profiling to start asynchronously
                    TimeSpan.FromMilliseconds(500)
                ));
            })
#endif

#if WINDOWS
            .ConfigureLifecycleEvents(events =>
            {
                // Make sure to add "using Microsoft.Maui.LifecycleEvents;" in the top of the file 
                events.AddWindows(windowsLifecycleBuilder =>
                {
                    windowsLifecycleBuilder.OnPlatformMessage((window, e) => 
                    {
                        switch(e.MessageId)
                        {
                            case 0x0112: /* WM_SYSCOMMAND */
                                if (e.WParam == 0xF020)
                                {
                                    System.Diagnostics.Debug.WriteLine("Minimizing!");
                                }
                                else if (e.WParam == 0xF120)
                                {
                                    System.Diagnostics.Debug.WriteLine("Restoring!");
                                }
                                break;
                            case 0x0005: /* WM_SIZE */
                                if (e.WParam == 0)
                                {
                                    System.Diagnostics.Debug.WriteLine("SIZE_RESTORED!");
                                }
                                else if (e.WParam == 1)
                                {
                                    System.Diagnostics.Debug.WriteLine("SIZE_MINIMIZED!");
                                }
                                break;
                        }
                    });
                    windowsLifecycleBuilder.OnWindowCreated(window =>
                    {
                        GHApp.WindowsXamlWindow = window;
                        window.Closed += (s, e) =>
                        {
                            if (GHApp.WindowsApp != null)
                                GHApp.WindowsApp.Exit();
                            else
                                Environment.Exit(0);
                        };
                        var handle = WinRT.Interop.WindowNative.GetWindowHandle(window);
                        var id = Microsoft.UI.Win32Interop.GetWindowIdFromWindow(handle);
                        var appWindow = Microsoft.UI.Windowing.AppWindow.GetFromWindowId(id);
                        appWindow.Closing += (s, e) =>
                        {
                            if(GHApp.CurrentGamePage != null)
                            {
                                e.Cancel = true;
                                GHApp.CurrentGamePage?.GenericButton_Clicked(s, new EventArgs(), GHUtils.Meta('q'));
                            }
                            else
                            {
                                GHApp.SaveWindowPosition();
                                GHApp.FmodService?.StopAllSounds((uint)StopSoundFlags.All, 0U);
                            }
                        };
                        bool maximizeWindow = !Preferences.Get("WindowedMode", false);
                        if(maximizeWindow)
                        {
                            window.ExtendsContentIntoTitleBar = false;
                            switch (appWindow.Presenter)
                            {
                                case Microsoft.UI.Windowing.OverlappedPresenter overlappedPresenter:
                                    overlappedPresenter.SetBorderAndTitleBar(false, false);
                                    overlappedPresenter.Maximize();
                                    break;
                            }
                        }
                        else
                        {
                            int sizeX = Preferences.Get("WindowedSizeX", 0);
                            int sizeY = Preferences.Get("WindowedSizeY", 0);
                            int sizeWidth = Preferences.Get("WindowedSizeWidth", 0);
                            int sizeHeight = Preferences.Get("WindowedSizeHeight", 0);

                            Microsoft.UI.Windowing.DisplayArea displayArea = Microsoft.UI.Windowing.DisplayArea.GetFromWindowId(id, Microsoft.UI.Windowing.DisplayAreaFallback.Nearest);
                            if (sizeWidth > 0 && sizeHeight > 0)
                            {
                                if (displayArea != null && displayArea.WorkArea.Width > 0 && displayArea.WorkArea.Height > 0)
                                {
                                    if (sizeX + sizeWidth > displayArea.WorkArea.X + displayArea.WorkArea.Width * 1.1 + 32)
                                        sizeX = sizeWidth = 0;
                                    else if (sizeX < displayArea.WorkArea.X - 16)
                                        sizeX = sizeWidth = 0;

                                    if (sizeY + sizeHeight > displayArea.WorkArea.Y + displayArea.WorkArea.Height * 1.1 + 32)
                                        sizeY = sizeHeight = 0;
                                    else if (sizeY < displayArea.WorkArea.Y - 16)
                                        sizeY = sizeHeight = 0;
                                }
                                appWindow.MoveAndResize(new Windows.Graphics.RectInt32(sizeX, sizeY, sizeWidth, sizeHeight));
                            }
                            else if (displayArea != null && displayArea.WorkArea.Width > 0 && displayArea.WorkArea.Height > 0)
                            {
                                var CenteredPosition = appWindow.Position;
                                CenteredPosition.X = ((displayArea.WorkArea.Width - appWindow.Size.Width) / 2);
                                CenteredPosition.Y = ((displayArea.WorkArea.Height - appWindow.Size.Height) / 2);
                                appWindow.Move(CenteredPosition);
                            }

                            appWindow.Destroying += (sender, args) =>
                            {
                                if(sender != null && GHApp.WindowedMode)
                                {
                                    Preferences.Set("WindowedSizeX", sender.Position.X);
                                    Preferences.Set("WindowedSizeY", sender.Position.Y);
                                    Preferences.Set("WindowedSizeWidth", sender.Size.Width);
                                    Preferences.Set("WindowedSizeHeight", sender.Size.Height);
                                }
                            };
                        }
                    });
                });
            })
#endif            
            .ConfigureFonts(fonts =>
            {
                fonts.AddFont("OpenSans-Regular.ttf", "OpenSansRegular");
				fonts.AddFont("OpenSans-Semibold.ttf", "OpenSansSemibold");
                fonts.AddFont("archristy.ttf", "ARChristy");
                fonts.AddFont("DejaVuSansMono.ttf", "DejaVuSansMono");
                fonts.AddFont("DejaVuSansMono-Bold.ttf", "DejaVuSansMonoBold");
                fonts.AddFont("DejaVuSansMono-BoldOblique.ttf", "DejaVuSansMonoBoldOblique");
                fonts.AddFont("DejaVuSansMono-Oblique.ttf", "DejaVuSansMonoOblique");
                fonts.AddFont("diablo_h.ttf", "Diablo");
                fonts.AddFont("endr.ttf", "Endor");
                fonts.AddFont("Immortal-Regular.ttf", "Immortal");
                fonts.AddFont("Lato-Regular.ttf", "LatoRegular");
                fonts.AddFont("Lato-Bold.ttf", "LatoBold");
                fonts.AddFont("shxi.ttf", "Xizor");
                fonts.AddFont("uwch.ttf", "Underwood");
            });

#if DEBUG
		builder.Logging.AddDebug();
#endif
        return builder.Build();
	}
}
