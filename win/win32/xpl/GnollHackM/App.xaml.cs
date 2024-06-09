using GnollHackX;
using Microsoft.Maui.Controls.PlatformConfiguration;
using Newtonsoft.Json.Linq;

#if WINDOWS10_0_19041_0_OR_GREATER
using WindowsUI = Windows.UI;
using XamlUI = Microsoft.UI.Xaml.Media;
#endif

namespace GnollHackM;

public partial class App : Application
{
	public App()
	{
		InitializeComponent();

#if WINDOWS10_0_19041_0_OR_GREATER
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
#endif
        GHApp.Initialize();
		MainPage = new AppShell();
    }

    protected override void OnStart()
    {
        base.OnStart();
        GHApp.OnStart();
    }

    protected override void OnSleep()
    {
        base.OnSleep();
        GHApp.OnSleep();
    }

    protected override void OnResume()
    {
        base.OnResume();
        GHApp.OnResume();
    }
}
