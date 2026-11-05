#if GNH_AVALONIA && MACOS
using System;
using System.Runtime.InteropServices;
using Avalonia;
using Avalonia.Controls;
using Avalonia.Input;
using GnollHackX;

namespace GnollHackM
{
    /* The macOS menu bar: the application menu, named GnollHack, and a Window menu. Avalonia's default application
       items are turned off in AvaloniaDesktopProgram, so that Quit goes through the game's own close flow; the hide
       commands go to NSApplication directly. UI thread. */
    internal static class GHMacApplicationMenu
    {
        public const string ApplicationName = "GnollHack";

        private const string ObjCLibrary = "/usr/lib/libobjc.A.dylib";

        [DllImport(ObjCLibrary)]
        private static extern IntPtr objc_getClass(string name);

        [DllImport(ObjCLibrary)]
        private static extern IntPtr sel_registerName(string name);

        [DllImport(ObjCLibrary, EntryPoint = "objc_msgSend")]
        private static extern IntPtr SendIntPtr(IntPtr receiver, IntPtr selector);

        [DllImport(ObjCLibrary, EntryPoint = "objc_msgSend")]
        private static extern void SendVoidIntPtr(IntPtr receiver, IntPtr selector, IntPtr arg);

        /* After setup, before Avalonia.Native reads the application name for the process name and the menu title */
        public static void Install(AppBuilder builder)
        {
            try
            {
                Avalonia.Application app = builder.Instance;
                if (app == null)
                    return;
                app.Name = ApplicationName;
                NativeMenu.SetMenu(app, CreateApplicationMenu());
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine("GHMacApplicationMenu: " + ex.Message);
            }
        }

        private static NativeMenu CreateApplicationMenu()
        {
            NativeMenu menu = new NativeMenu();

            NativeMenuItem about = new NativeMenuItem("About " + ApplicationName);
            about.Click += OnAboutClicked;
            menu.Add(about);
            menu.Add(new NativeMenuItemSeparator());

            NativeMenuItem hide = new NativeMenuItem("Hide " + ApplicationName);
            hide.Gesture = new KeyGesture(Key.H, KeyModifiers.Meta);
            hide.Click += (sender, e) => SendToApplication("hide:");
            menu.Add(hide);

            NativeMenuItem hideOthers = new NativeMenuItem("Hide Others");
            hideOthers.Gesture = new KeyGesture(Key.H, KeyModifiers.Meta | KeyModifiers.Alt);
            hideOthers.Click += (sender, e) => SendToApplication("hideOtherApplications:");
            menu.Add(hideOthers);

            NativeMenuItem showAll = new NativeMenuItem("Show All");
            showAll.Click += (sender, e) => SendToApplication("unhideAllApplications:");
            menu.Add(showAll);
            menu.Add(new NativeMenuItemSeparator());

            NativeMenuItem quit = new NativeMenuItem("Quit " + ApplicationName);
            quit.Gesture = new KeyGesture(Key.Q, KeyModifiers.Meta);
            quit.Click += (sender, e) => GHAvaloniaWindowHooks.RequestQuit();
            menu.Add(quit);

            return menu;
        }

        /* The window's menu bar items after the application menu */
        public static NativeMenu CreateWindowMenu()
        {
            NativeMenu windowItems = new NativeMenu();

            NativeMenuItem minimize = new NativeMenuItem("Minimize");
            minimize.Gesture = new KeyGesture(Key.M, KeyModifiers.Meta);
            /* Avalonia.Native leaves full screen before it miniaturizes, and the Dock then restores a normal window;
               a full-screen window is hidden instead, and the Dock returns to its full-screen Space */
            minimize.Click += (sender, e) =>
            {
                Avalonia.Controls.Window window = GHAvaloniaWindowHooks.MainWindow;
                if (window == null)
                    return;
                if (window.WindowState == WindowState.FullScreen)
                    SendToApplication("hide:");
                else
                    window.WindowState = WindowState.Minimized;
            };
            windowItems.Add(minimize);

            /* GnollHack has one window: closing it is quitting, as with the window's close button */
            NativeMenuItem close = new NativeMenuItem("Close Window");
            close.Gesture = new KeyGesture(Key.W, KeyModifiers.Meta);
            close.Click += (sender, e) => GHAvaloniaWindowHooks.RequestQuit();
            windowItems.Add(close);

            NativeMenuItem windowMenu = new NativeMenuItem("Window");
            windowMenu.Menu = windowItems;
            NativeMenu bar = new NativeMenu();
            bar.Add(windowMenu);
            return bar;
        }

        private static async void OnAboutClicked(object sender, EventArgs e)
        {
            try
            {
                MainPage mainPage = GHApp.CurrentMainPage;
                if (mainPage != null)
                    await mainPage.TryOpenAboutPageFromMenu();
            }
            catch (Exception ex)
            {
                GHApp.ReportCaughtException(ex);
            }
        }

        /* [NSApp <selector>nil] */
        private static void SendToApplication(string selector)
        {
            try
            {
                IntPtr applicationClass = objc_getClass("NSApplication");
                if (applicationClass == IntPtr.Zero)
                    return;
                IntPtr application = SendIntPtr(applicationClass, sel_registerName("sharedApplication"));
                if (application != IntPtr.Zero)
                    SendVoidIntPtr(application, sel_registerName(selector), IntPtr.Zero);
            }
            catch (Exception ex)
            {
                GHApp.ReportCaughtException(ex);
            }
        }
    }
}
#endif
