using Microsoft.Extensions.Logging;
using Microsoft.Maui.LifecycleEvents;
using SkiaSharp.Views.Maui.Controls.Hosting;
using System.Runtime.Intrinsics.Arm;
using GnollHackX;
using SkiaSharp;

#if ANDROID
using AndroidX.Activity;
using Android.App;
using AndroidX.Fragment.App;
using Microsoft.Maui.Platform;
using Android.OS;
using Android.Views;
#endif

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
using Microsoft.UI.Windowing;
using System.Runtime.InteropServices;
using System.Diagnostics;
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
#if ANDROID
                handlers.AddHandler(typeof(Button), typeof(NoFocusButtonHandler));
#endif
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
                                {
                                    switch (e.WParam)
                                    {
                                        case 0xF100:
                                            System.Diagnostics.Debug.WriteLine("MenuKey Pressed!");
                                            return;
                                        case 0xF020:
                                            System.Diagnostics.Debug.WriteLine("Minimizing!");
                                            break;
                                        case 0xF120:
                                            System.Diagnostics.Debug.WriteLine("Restoring!");
                                            GHApp.DoKeyboardFocus();
                                            break;
                                    }

                                }
                                break;
                            case 0x0005: /* WM_SIZE */
                                {
                                    switch (e.WParam)
                                    {
                                        case 0:
                                            System.Diagnostics.Debug.WriteLine("SIZE_RESTORED!");
                                            GHApp.DoKeyboardFocus();
                                            break;
                                        case 1:
                                            System.Diagnostics.Debug.WriteLine("SIZE_MINIMIZED!");
                                            break;
                                        case 2:
                                            System.Diagnostics.Debug.WriteLine("SIZE_MAXIMIZED!");
                                            GHApp.DoKeyboardFocus();
                                            break;
                                    }
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
                            {
                                GHApp.WindowsApp.Exit();
                                GHApp.WindowsApp = null;
                            }
                            else
                                Environment.Exit(0);
                        };
                        var handle = WinRT.Interop.WindowNative.GetWindowHandle(window);
                        var id = Microsoft.UI.Win32Interop.GetWindowIdFromWindow(handle);
                        //var appWindow = Microsoft.UI.Windowing.AppWindow.GetFromWindowId(id);
                        KeyboardHook.Start();

                        var appWindow = window.AppWindow;
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

                        bool fullScreen = !Preferences.Get("WindowedMode", false);
                        if (fullScreen)
                        {
                            window.AppWindow.SetPresenter(AppWindowPresenterKind.FullScreen);
                            //window.ExtendsContentIntoTitleBar = false;
                            //switch (appWindow.Presenter)
                            //{
                            //    case Microsoft.UI.Windowing.OverlappedPresenter overlappedPresenter:
                            //        overlappedPresenter.SetBorderAndTitleBar(false, false);
                            //        overlappedPresenter.Maximize();
                            //        break;
                            //}
                        }
                        else
                        {
                            bool onSomeMonitor = false;
                            int sizeX = Preferences.Get("WindowedSizeX", 0);
                            int sizeY = Preferences.Get("WindowedSizeY", 0);
                            int sizeWidth = Preferences.Get("WindowedSizeWidth", 0);
                            int sizeHeight = Preferences.Get("WindowedSizeHeight", 0);
                            float sizeDisplayDensity = Preferences.Get("WindowedSizeDisplayDensity", 1.0f);
                            var monitors = GHMonitor.All;
                            if (monitors?.Count >= 1)
                            {
                                SKRect r1 = new SKRect(sizeX, sizeY, sizeX + sizeWidth, sizeY + sizeHeight);
                                foreach (var monitor in monitors)
                                {
                                    SKRect r2 = new SKRect(monitor.WorkingArea.X, monitor.WorkingArea.Y, monitor.WorkingArea.X + monitor.WorkingArea.Width, monitor.WorkingArea.Y + monitor.WorkingArea.Height);
                                    if (r1.IntersectsWith(r2))
                                    {
                                        onSomeMonitor = true;
                                        break;
                                    }
                                }
                            }

                            Microsoft.UI.Windowing.DisplayArea displayArea = Microsoft.UI.Windowing.DisplayArea.GetFromWindowId(id, Microsoft.UI.Windowing.DisplayAreaFallback.Nearest);
                            if (sizeWidth > 0 && sizeHeight > 0 && onSomeMonitor)
                            {
                                float scale = GHApp.DisplayDensity / (sizeDisplayDensity <= 0 ? 1.0f : sizeDisplayDensity);
                                appWindow.MoveAndResize(new Windows.Graphics.RectInt32(sizeX, sizeY, (int)(sizeWidth * scale), (int)(sizeHeight * scale)));
                            }
                            else if (displayArea != null && displayArea.WorkArea.Width > 0 && displayArea.WorkArea.Height > 0)
                            {
                                var CenteredPosition = appWindow.Position;
                                CenteredPosition.X = ((displayArea.WorkArea.Width - appWindow.Size.Width) / 2);
                                CenteredPosition.Y = ((displayArea.WorkArea.Height - appWindow.Size.Height) / 2);
                                appWindow.Move(CenteredPosition);
                            }

                            bool sizeMaximized = Preferences.Get("WindowedSizeIsMaximized", false);
                            if(sizeMaximized)
                            {
                                switch (appWindow.Presenter)
                                {
                                    case Microsoft.UI.Windowing.OverlappedPresenter overlappedPresenter:
                                        overlappedPresenter.Maximize();
                                        break;
                                }
                            }

                            appWindow.Destroying += (sender, args) =>
                            {
                                KeyboardHook.Stop();
                                if (sender != null && GHApp.WindowedMode)
                                {
                                    bool isMaximized = false;
                                    var presenter = sender.Presenter as OverlappedPresenter;
                                    if (presenter != null)
                                    {
                                        if (presenter.State == OverlappedPresenterState.Maximized)
                                        {
                                            isMaximized = true;
                                        }
                                    }
                                    Preferences.Set("WindowedSizeDisplayDensity", GHApp.DisplayDensity);
                                    Preferences.Set("WindowedSizeIsMaximized", isMaximized);
                                    if (!isMaximized)
                                    {
                                        Preferences.Set("WindowedSizeX", sender.Position.X);
                                        Preferences.Set("WindowedSizeY", sender.Position.Y);
                                        Preferences.Set("WindowedSizeWidth", sender.Size.Width);
                                        Preferences.Set("WindowedSizeHeight", sender.Size.Height);
                                    }
                                }
                            };
                        }
                    });
                });
            })
#endif
            .ConfigureLifecycleEvents(static lifecycleBuilder =>
            {
            #if ANDROID
                lifecycleBuilder.AddAndroid(androidLifecycleBuilder =>
                {
                    androidLifecycleBuilder.OnCreate((activity, savedInstance) =>
                    {
                        if (activity is ComponentActivity componentActivity)
                        {
                            componentActivity.GetFragmentManager()?.RegisterFragmentLifecycleCallbacks(new MyFragmentLifecycleCallbacks((fragmentManager, fragment) =>
                            {
                                // Modals in MAUI in NET9 use DialogFragment
                                if (fragment is AndroidX.Fragment.App.DialogFragment dialogFragment)
                                {
                                    var window = dialogFragment.Dialog?.Window;
                                    if(window != null)
                                    {
                                        if (Build.VERSION.SdkInt >= BuildVersionCodes.R)
                                        {
#pragma warning disable CA1416 // Supported on: 'android' 30.0 and later
                                            window!.SetDecorFitsSystemWindows(false);
                                            window!.InsetsController?.Hide(WindowInsets.Type.SystemBars());
                                            if (window!.InsetsController != null)
                                                window!.InsetsController.SystemBarsBehavior = (int)WindowInsetsControllerBehavior.ShowTransientBarsBySwipe;
#pragma warning restore CA1416 // Supported on: 'android' 30.0 and later
                                        }
                                        else
                                        {
#pragma warning disable CS0618 // Type or member is obsolete
                                            SystemUiFlags systemUiVisibility = (SystemUiFlags)window!.DecorView.SystemUiVisibility;
                                            systemUiVisibility |= SystemUiFlags.HideNavigation;
                                            systemUiVisibility |= SystemUiFlags.Immersive;
                                            window!.DecorView.SystemUiVisibility = (StatusBarVisibility)systemUiVisibility;
#pragma warning restore CS0618 // Type or member is obsolete
                                        }
                                    }
                                }
                            }), false);
                        }
                    });
                });
            #endif
            })
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

#if ANDROID
public class MyFragmentLifecycleCallbacks(Action<AndroidX.Fragment.App.FragmentManager, AndroidX.Fragment.App.Fragment> onFragmentStarted) : AndroidX.Fragment.App.FragmentManager.FragmentLifecycleCallbacks
{
    public override void OnFragmentStarted(AndroidX.Fragment.App.FragmentManager fm, AndroidX.Fragment.App.Fragment f)
    {
        onFragmentStarted?.Invoke(fm, f);
        base.OnFragmentStarted(fm, f);
    }
}
#endif

#if WINDOWS
public class KeyboardHook
{
    private const int WH_KEYBOARD_LL = 13;

    private const int WM_KEYDOWN = 0x0100; // Key messages
    private const int WM_KEYUP = 0x0101; // Key messages
    private const int WM_SYSKEYDOWN = 0x0104; // ALT + Key messages
    private const int WM_SYSKEYUP = 0x0105; // ALT + Key messages    
    private const int VK_SHIFT = 0x10;
    private const int VK_CONTROL = 0x11;

    private static LowLevelKeyboardProc _proc = HookCallback;
    private static IntPtr _hookID = IntPtr.Zero;

    public static void Start()
    {
        _hookID = SetHook(_proc);
    }

    public static void Stop()
    {
        UnhookWindowsHookEx(_hookID);
    }

    private static IntPtr SetHook(LowLevelKeyboardProc proc)
    {
        using (Process curProcess = Process.GetCurrentProcess())
        using (ProcessModule curModule = curProcess.MainModule)
        {
            return SetWindowsHookEx(WH_KEYBOARD_LL, proc,
                GetModuleHandle(curModule.ModuleName), 0);
        }
    }

    private delegate IntPtr LowLevelKeyboardProc(int nCode, IntPtr wParam, IntPtr lParam);

    private static IntPtr HookCallback(int nCode, IntPtr wParam, IntPtr lParam)
    {
        Debug.WriteLine("HookCallback: " + nCode + ", " + wParam + ", " + lParam);
        if (nCode >= 0)
        {
            if (wParam == (IntPtr)WM_KEYUP)
            {
                Debug.WriteLine("HookCallback: WM_KEYUP");
                int vkCode = Marshal.ReadInt32(lParam);
                if (vkCode == 0x10 || vkCode == 0xA0 || vkCode == 0xA1)
                {
                    GHApp.ShiftDown = false;
                    Debug.WriteLine("HookCallback: Shift Up");
                }
                else if (vkCode == 0x11 || vkCode == 0xA2 || vkCode == 0xA3)
                {
                    GHApp.CtrlDown = false;
                    Debug.WriteLine("HookCallback: Control Up");
                }
            }
            else if (wParam == (IntPtr)WM_KEYDOWN)
            {
                Debug.WriteLine("HookCallback: WM_KEYDOWN");
                int vkCode = Marshal.ReadInt32(lParam);
                if (vkCode == 0x10 || vkCode == 0xA0 || vkCode == 0xA1)
                {
                    GHApp.ShiftDown = true;
                    Debug.WriteLine("HookCallback: Shift Down");
                }
                else if (vkCode == 0x11 || vkCode == 0xA2 || vkCode == 0xA3)
                {
                    GHApp.CtrlDown = true;
                    Debug.WriteLine("HookCallback: Control Down");
                }
            }
            else if (wParam == (IntPtr)WM_SYSKEYUP)
            {
                Debug.WriteLine("HookCallback: WM_SYSKEYUP");

                bool isShiftDown = GHApp.ShiftDown;
                bool isCtrlDown = GHApp.CtrlDown;

                GHApp.AltDown = false;
                int vkCode = Marshal.ReadInt32(lParam);
                if (vkCode == 0x10 || vkCode == 0xA0 || vkCode == 0xA1)
                {
                    GHApp.ShiftDown = false;
                    Debug.WriteLine("HookCallback: Syskey Shift Up");
                    return 1;
                }
                else if (vkCode == 0x11 || vkCode == 0xA2 || vkCode == 0xA3)
                {
                    GHApp.CtrlDown = false;
                    Debug.WriteLine("HookCallback: Syskey Control Up");
                    return 1;
                }
                else if (isCtrlDown) /* AltGr, also includes $ on Finnish keyboard */
                {
                    Debug.WriteLine("HookCallback: Syskey with Ctrl (AltGr)");
                    return CallNextHookEx(_hookID, nCode, wParam, lParam);
                }

                GHSpecialKey spkey = GHSpecialKey.None;
                if (vkCode >= 0x41 && vkCode <= 0x5A)
                    spkey = GHSpecialKey.A + vkCode - 0x41;
                else if (vkCode >= 0x30 && vkCode <= 0x39)
                    spkey = GHSpecialKey.Number0 + vkCode - 0x30;
                else if (vkCode >= 0x60 && vkCode <= 0x69)
                    spkey = GHSpecialKey.NumberPad0 + vkCode - 0x60;
                else
                {
                    switch (vkCode)
                    {
                        case 0xBB:
                            spkey = GHSpecialKey.Plus;
                            break;
                        case 0xBC:
                            spkey = GHSpecialKey.Comma;
                            break;
                        case 0xBD:
                            spkey = GHSpecialKey.Minus;
                            break;
                        case 0xBE:
                            spkey = GHSpecialKey.Period;
                            break;
                    }
                }
                GHApp.SendSpecialKeyPress(spkey, isCtrlDown, true, isShiftDown);
                return 1;
            }
            else if (wParam == (IntPtr)WM_SYSKEYDOWN)
            {
                int vkCode = Marshal.ReadInt32(lParam);
                Debug.WriteLine("HookCallback: WM_SYSKEYDOWN, vkCode=" + vkCode);
                if (vkCode == 0x10 || vkCode == 0xA0 || vkCode == 0xA1)
                {
                    GHApp.ShiftDown = true;
                    Debug.WriteLine("HookCallback: Syskey Shift Down");
                }
                else if (vkCode == 0x11 || vkCode == 0xA2 || vkCode == 0xA3)
                {
                    GHApp.CtrlDown = true;
                    Debug.WriteLine("HookCallback: Syskey Control Down");
                }
                else if (vkCode == 0x12 || vkCode == 0xA4 || vkCode == 0xA5)
                {
                    GHApp.AltDown = true;
                    Debug.WriteLine("HookCallback: Syskey Alt Down");
                }
                else if (GHApp.CtrlDown)
                {
                    Debug.WriteLine("HookCallback: Syskey Down with Ctrl (AltGr)");
                }
                else
                    return 1;
            }
        }
        return CallNextHookEx(_hookID, nCode, wParam, lParam);
    }

    [DllImport("user32.dll", SetLastError = true)]
    private static extern IntPtr SetWindowsHookEx(int idHook, LowLevelKeyboardProc lpfn, IntPtr hMod, uint dwThreadId);

    [DllImport("user32.dll", SetLastError = true)]
    private static extern bool UnhookWindowsHookEx(IntPtr hhk);

    [DllImport("user32.dll", SetLastError = true)]
    private static extern IntPtr CallNextHookEx(IntPtr hhk, int nCode, IntPtr wParam, IntPtr lParam);

    [DllImport("kernel32.dll", SetLastError = true)]
    private static extern IntPtr GetModuleHandle(string lpModuleName);

    [DllImport("user32.dll")]
    private static extern short GetKeyState(int nVirtKey);

    [DllImport("user32.dll", SetLastError = true)]
    private static extern bool GetKeyboardState(byte[] lpKeyState);

}
#endif

