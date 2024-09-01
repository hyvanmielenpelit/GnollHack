using Android.App;
using Android.Content;
using Android.OS;
using Android.Runtime;
using Android.Views;
using Android.Widget;
using GnollHackX.Droid;
using GnollHackX.Pages.Game;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Xamarin.Forms;
using Xamarin.Forms.Platform.Android;

[assembly: ExportRenderer(typeof(Xamarin.Forms.Button), typeof(GHButtonRenderer))]
namespace GnollHackX.Droid
{
    public class GHButtonRenderer : ButtonRenderer
    {
        public GHButtonRenderer(Context context) : base(context)
        {
        }
        protected override void OnElementChanged(ElementChangedEventArgs<Xamarin.Forms.Button> e)
        {
            base.OnElementChanged(e);
            if (e.NewElement != null)
            {
                Android.Widget.Button b = Control as Android.Widget.Button;
                if (b != null)
                {
                    b.Focusable = false;
                }
            }
        }
    }
}