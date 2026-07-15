using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.Maui.Handlers;
using UIKit;

namespace GnollHackM.Platforms.iOS
{
    /// <summary>
    /// Custom ButtonHandler for iOS that suppresses the implicit UIButton title
    /// crossfade animation introduced in iOS 15 with UIButtonConfiguration.
    /// Without this, button text changes (or the initial text appearance when a
    /// parent view becomes visible) cause a visible fade-in effect.
    /// Similar workaround to NotAnimatedBorderHandler for Border layout animations.
    /// </summary>
    public class NotAnimatedButtonHandler : ButtonHandler
    {
        private class NotAnimatedUIButton : UIButton
        {
            public NotAnimatedUIButton() : base(UIButtonType.System)
            {
            }

            public override void SetTitle(string title, UIControlState forState)
            {
                // Wrap title changes in PerformWithoutAnimation to prevent
                // the implicit crossfade transition on the title label.
                UIView.PerformWithoutAnimation(() =>
                {
                    base.SetTitle(title, forState);
                    LayoutIfNeeded();
                });
            }

            public override void LayoutSubviews()
            {
                // Also suppress any implicit Core Animation transitions during layout,
                // which can occur when the button's parent view becomes visible.
                UIView.PerformWithoutAnimation(() =>
                {
                    base.LayoutSubviews();
                });
            }
        }

        protected override UIButton CreatePlatformView()
        {
            return new NotAnimatedUIButton();
        }
    }
}
