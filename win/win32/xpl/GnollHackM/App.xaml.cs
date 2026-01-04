using GnollHackX;
using Microsoft.Maui.Controls.PlatformConfiguration;
using Microsoft.Maui.Platform;
using Newtonsoft.Json.Linq;

#if WINDOWS
using WindowsUI = Windows.UI;
using XamlUI = Microsoft.UI.Xaml.Media;
#endif

namespace GnollHackM;

public partial class App : Application
{
	public App()
	{
		InitializeComponent();

#if WINDOWS
        Microsoft.Maui.Handlers.PickerHandler.Mapper.AppendToMapping(nameof(IPicker.Title), (handler, view) =>
        {
            if (handler.PlatformView is not null && view is Picker pick && !String.IsNullOrWhiteSpace(pick.Title))
            {
                handler.PlatformView.HeaderTemplate = new Microsoft.UI.Xaml.DataTemplate();
                handler.PlatformView.PlaceholderText = pick.Title;
                pick.Title = null;

                pick.TitleColor.ToRgba(out byte r, out byte g, out byte b, out byte a);
                handler.PlatformView.PlaceholderForeground = new XamlUI.SolidColorBrush(WindowsUI.Color.FromArgb(a, r, g, b));
            }
        });
        Microsoft.Maui.Handlers.SwitchHandler.Mapper.AppendToMapping("NoLabel", (handler, View) =>
        {
            handler.PlatformView.OnContent = null;
            handler.PlatformView.OffContent = null;
            handler.PlatformView.MinWidth = 0;
            handler.PlatformView.MinHeight = 0;
        });
        Microsoft.Maui.Handlers.ButtonHandler.Mapper.AppendToMapping("NoFocusBorder", (handler, View) =>
        {
            handler.PlatformView.BorderThickness = new Microsoft.UI.Xaml.Thickness(0);
        });
#endif
#if ANDROID
        Microsoft.Maui.Handlers.ButtonHandler.Mapper.AppendToMapping("NoRippleEffect", (handler, view) =>
        {
            if (handler.PlatformView.Background is Android.Graphics.Drawables.RippleDrawable ripple )
            {
                ripple.SetColor(Android.Content.Res.ColorStateList.ValueOf(Android.Graphics.Color.Transparent));
            };
        });
#endif
#if ANDROID || WINDOWS
        // If UIDesignRequiresCompatibility Info.plist item gets deprecated, you can change the name of the global Switch style to PlatformSwitchStyle, and add it only for right platforms here
        if (Application.Current.Resources.TryGetValue("PlatformSwitchStyle", out var styleObj) && styleObj is Style baseStyle)
        {
            var switchStyle = new Style(typeof(Switch));
            foreach (var setter in baseStyle.Setters)
            {
                switchStyle.Setters.Add(setter);
            }
            Application.Current.Resources.Add(switchStyle);
        }
#endif
        GHApp.Initialize();
        //MainPage = new AppShell();
    }

    protected override Window CreateWindow(IActivationState activationState)
    {
        Window window = new Window(new AppShell());
        window.Activated += WindowActivated;
        window.Deactivated += WindowDeactivated;
        window.Destroying += WindowDestroying;
        window.Stopped += Window_Stopped;
        window.Resumed += Window_Resumed;
        window.Created += Window_Created;
        return window;  
    }

    private static void Window_Created(object sender, EventArgs e)
    {
        GHApp.MaybeWriteGHLog("MAUI Window has been created.", true, "Window Information");
        GHApp.OnStart();
    }

    private static void Window_Resumed(object sender, EventArgs e)
    {
        GHApp.MaybeWriteGHLog("MAUI Window has been resumed.", true, "Window Information");
        GHApp.OnResume();
    }

    private static void Window_Stopped(object sender, EventArgs e)
    {
        GHApp.MaybeWriteGHLog("MAUI Window has been stopped / entered background.", true, "Window Information");
        GHApp.OnSleep();
    }

    private static void WindowActivated(object sender, EventArgs e)
    {
        GHApp.MaybeWriteGHLog("MAUI Window has been activated / focused.", true, "Window Information");
        GHApp.OnFocus();
    }
    private static void WindowDeactivated(object sender, EventArgs e)
    {
        GHApp.MaybeWriteGHLog("MAUI Window has been deactivated / unfocused.", true, "Window Information");
        GHApp.OnUnfocus();
    }
    private static void WindowDestroying(object sender, EventArgs e)
    {
        GHApp.MaybeWriteGHLog("MAUI Window's native window is being destroyed.", true, "Window Information");
        //Window window = sender as Window;
        //if (window != null)
        //{
        //    window.Activated -= WindowActivated;
        //    window.Deactivated -= WindowDeactivated;
        //    window.Destroying -= WindowDestroying;
        //    window.Stopped -= Window_Stopped;
        //    window.Resumed -= Window_Resumed;
        //    window.Created -= Window_Created;
        //}
    }

    //protected override void OnStart()
    //{
    //    base.OnStart();
    //    GHApp.OnStart();
    //}

    //protected override void OnSleep()
    //{
    //    base.OnSleep();
    //    GHApp.OnSleep();
    //}

    //protected override void OnResume()
    //{
    //    base.OnResume();
    //    GHApp.OnResume();
    //}

    protected override void CleanUp()
    {
        GHApp.RevertScreenResolution();
        base.CleanUp();
    }


}
