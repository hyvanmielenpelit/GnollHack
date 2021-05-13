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
        [DllImport(@"libgnollhackdroid.so")]
        public static extern int DoSomeHackDroid();
        [DllImport(@"libgnollhackdroid.so")]
        public static extern int RunGnollHackTest([MarshalAs(UnmanagedType.LPStr)] string cwd, [MarshalAs(UnmanagedType.LPStr)] string cwd2, [MarshalAs(UnmanagedType.LPStr)] string cwd3);

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
        public int Test2()
        {
            return DoSomeHackDroid();
        }
        public int TestRunGnollHack()
        {
            string cwd = Directory.GetCurrentDirectory();
            string cwd2 = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string cwd3 = Android.OS.Environment.DataDirectory.Path;
            string cwd4 = Android.App.Application.Context.DataDir.Path;
            string cwd5 = Android.App.Application.Context.ApplicationInfo.DataDir;
            string cwd6 = Android.App.Application.Context.ApplicationInfo.NativeLibraryDir;
            string cwd7 = Android.App.Application.Context.ApplicationInfo.PublicSourceDir;
            string cwd8 = Android.App.Application.Context.ApplicationInfo.SourceDir;
            string cwd9 = Android.App.Application.Context.ApplicationInfo.DeviceProtectedDataDir;
            string cwd10 = Android.App.Application.Context.FilesDir.Path;
            string cwd11 = Android.App.Application.Context.ObbDir.Path;
            //string cwd4 = System.AppDomain.CurrentDomain.BaseDirectory;
            return RunGnollHackTest(cwd4, cwd10, cwd3);
        }
    }
}