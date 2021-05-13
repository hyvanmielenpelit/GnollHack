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
using System.Reflection;
using System.IO;
using Xamarin.Forms;
using System.Runtime.InteropServices;

[assembly: Dependency(typeof(GnollHackClient.Droid.GhollHackService))]
namespace GnollHackClient.Droid
{
    class GhollHackService : IGnollHackService
    {
        [DllImport(@"libgnollhackdroid.so")]
        public static extern int DoSomeCalcDroid();
        private void LoadNativeLibrary(string libName)
        {
            Java.Lang.JavaSystem.LoadLibrary(libName);
        }

        public void InitializeGnollHack()
        {
            LoadNativeLibrary("gnollhackdroid");
        }

        public int Test1()
        {
            return DoSomeCalcDroid();
        }
    }
}