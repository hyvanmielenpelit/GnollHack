using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.Maui.Handlers;
using Microsoft.Maui.Platform;
using ContentView = Microsoft.Maui.Platform.ContentView;

namespace GnollHackM.Platforms.iOS
{
    public class NotAnimatedBorderHandler : BorderHandler
    {
        private class BorderContentView : ContentView
        {
            public override void LayoutSubviews()
            {
                // This is the only workaround I found to avoid the animation when the border size is updated.
                // https://github.com/dotnet/maui/issues/15363
                // https://github.com/dotnet/maui/issues/18204

                if (Layer.Sublayers?.FirstOrDefault(layer => layer is MauiCALayer) is { AnimationKeys: not null } caLayer)
                {
                    caLayer.RemoveAnimation("bounds");
                    caLayer.RemoveAnimation("position");
                }

                base.LayoutSubviews();
            }
        }

        protected override ContentView CreatePlatformView()
        {
            _ = VirtualView ?? throw new InvalidOperationException($"{nameof(VirtualView)} must be set to create a {nameof(ContentView)}");
            _ = MauiContext ?? throw new InvalidOperationException($"{nameof(MauiContext)} cannot be null");

            return new BorderContentView
            {
                CrossPlatformLayout = VirtualView
            };
        }
    }
}
