using AVFoundation;
using Foundation;
using GnollHackX;
using GnollHackX.iOS;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UIKit;
using Xamarin.Forms;
using Xamarin.Forms.Platform.iOS;

[assembly: ExportRenderer(typeof(MainPage), typeof(GHMainPageRenderer))]
namespace GnollHackX.iOS
{
    public class GHMainPageRenderer : PageRenderer
    {

        protected override void OnElementChanged(VisualElementChangedEventArgs e)
        {
            base.OnElementChanged(e);

            //var asset = AVAsset.FromUrl(NSUrl.FromFilename("Resources/mainmenuportrait.mp4"));
            //var playerItem = new AVPlayerItem(asset);
            //var player = new AVPlayer(playerItem);
            //var playerLayer = AVPlayerLayer.FromPlayer(player);
            //View.Layer.AddSublayer(playerLayer);
            //player.Play();

        }
    }
}