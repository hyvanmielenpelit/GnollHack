using Foundation;
using UIKit;
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

    public override bool FinishedLaunching(UIApplication app, NSDictionary options)
    {
        var result = base.FinishedLaunching(app, options);
        if (Window != null)
        {
            Window.BackgroundColor = UIColor.FromRGB(0x00, 0x00, 0x00); // Set the background color to black
        }
        return result;
    }
}
