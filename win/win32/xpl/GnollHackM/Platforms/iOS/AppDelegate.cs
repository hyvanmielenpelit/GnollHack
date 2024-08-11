using Foundation;
using GnollHackX;

namespace GnollHackM;

[Register("AppDelegate")]
public class AppDelegate : MauiUIApplicationDelegate
{
	public AppDelegate() : base()
	{
        GHApp.InitializeGC();
    }
    protected override MauiApp CreateMauiApp() => MauiProgram.CreateMauiApp();
}
