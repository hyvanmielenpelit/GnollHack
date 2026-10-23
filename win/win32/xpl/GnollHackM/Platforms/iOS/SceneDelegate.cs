using Foundation;
using UIKit;

namespace GnollHackM;

[Register("SceneDelegate")]
public class SceneDelegate : MauiUISceneDelegate
{
    public override void WillConnect(UIScene scene, UISceneSession session, UISceneConnectionOptions connectionOptions)
    {
        base.WillConnect(scene, session, connectionOptions);
        if (Window != null)
        {
            Window.BackgroundColor = UIColor.FromRGB(0x00, 0x00, 0x00); // Black behind the page, as on the former app-delegate window
        }
    }
}
