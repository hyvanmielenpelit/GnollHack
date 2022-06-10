using Android.App;
using Android.Content;
using Android.OS;
using Android.Runtime;
using Android.Views;
using Android.Widget;
using GnollHackClient.Droid;
using GnollHackClient.Pages.Game;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Xamarin.Forms;
using Xamarin.Forms.Platform.Android;

[assembly: ExportRenderer(typeof(GamePage), typeof(GHGamePageRenderer))]
namespace GnollHackClient.Droid
{
    public class GHGamePageRenderer : PageRenderer
    {
        public GHGamePageRenderer(Context context) : base(context)
        {

        }

        protected override void OnElementChanged(ElementChangedEventArgs<Page> e)
        {
            base.OnElementChanged(e);

            if (e == null || e.OldElement != null || Element == null)
            {
                return;
            }
        }
    }
}