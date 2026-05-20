using Microsoft.UI.Xaml;
using GnollHackX;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace GnollHackM.WinUI;

/// <summary>
/// Provides application-specific behavior to supplement the default Application class.
/// </summary>
public partial class App : MauiWinUIApplication
{
	/// <summary>
	/// Initializes the singleton application object.  This is the first line of authored code
	/// executed, and as such is the logical equivalent of main() or WinMain().
	/// </summary>
	public App()
	{
        this.InitializeComponent();
		this.RequestedTheme = ApplicationTheme.Dark;
		GHApp.WindowsApp = this;
        GHApp.InitializeGC();
        Microsoft.UI.Xaml.Application.Current.UnhandledException += (s, e) =>
        {
            GHApp.MaybeWriteGHLog(e.Exception.Message);

            if (e.Exception is InvalidOperationException ioe && ioe.Message != null &&
                ((ioe.Message.Contains("already deactivated")) 
                || (ioe.Message.Contains("already activated")))
                )
            {
                e.Handled = true;
            }
        };
    }

    protected override MauiApp CreateMauiApp() => MauiProgram.CreateMauiApp();
}

