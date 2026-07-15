using Foundation;
using UIKit;
using GnollHackX;
using ObjCRuntime;

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

    /* Remove the system Quit menu item so that Cmd+Q is handled by our
     * UIKeyCommand in GHUIApplication instead of immediately killing the app.
     * This is critical on Mac ("Designed for iPad") where Cmd+Q would
     * otherwise terminate without saving the game. */
    public override void BuildMenu(IUIMenuBuilder builder)
    {
        base.BuildMenu(builder);
        if (builder.System == UIMenuSystem.MainSystem)
        {
            builder.RemoveMenu(UIMenuIdentifier.Quit.GetConstant());
        }
    }
}
