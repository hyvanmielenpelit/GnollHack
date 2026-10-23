using Foundation;
using Microsoft.Maui.Handlers;
using Microsoft.Maui.Platform;
using UIKit;

namespace GnollHackM.Platforms.iOS
{
    /// <summary>
    /// Temporary diagnostic ScrollViewHandler for the iOS 27 SettingsPage switch investigation.
    /// Its scroll view reports UIKit's content-touch decisions to TouchTrace and returns them unchanged.
    /// </summary>
    public class TracingScrollViewHandler : ScrollViewHandler
    {
        private class TracingScrollView : MauiScrollView
        {
            public override bool TouchesShouldBegin(NSSet touches, UIEvent withEvent, UIView inContentView)
            {
                bool result = base.TouchesShouldBegin(touches, withEvent, inContentView);
                TouchTrace.ScrollCallback("TouchesShouldBegin", inContentView, result);
                return result;
            }

            public override bool TouchesShouldCancelInContentView(UIView view)
            {
                bool result = base.TouchesShouldCancelInContentView(view);
                TouchTrace.ScrollCallback("TouchesShouldCancel", view, result);
                return result;
            }
        }

        protected override UIScrollView CreatePlatformView()
        {
            return new TracingScrollView();
        }
    }
}
