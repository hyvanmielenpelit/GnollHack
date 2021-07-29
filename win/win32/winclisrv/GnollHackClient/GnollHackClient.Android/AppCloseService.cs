using Android.App;
using Android.Content;
using Android.OS;
using Android.Runtime;
using Android.Views;
using Android.Widget;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Xamarin.Forms;
using System.Runtime.InteropServices;
using GnollHackClient;

[assembly: Dependency(typeof(GnollHackClient.Droid.AppCloseService))]
namespace GnollHackClient.Droid
{
    public class AppCloseService : IAppCloseService
    {
        public void CloseApplication()
        {
            MainActivity.CurrentMainActivity.Finish();
        }
        public bool IsModernAndroid()
        {
            return Build.VERSION.SdkInt >= BuildVersionCodes.R;
        }

    }
}