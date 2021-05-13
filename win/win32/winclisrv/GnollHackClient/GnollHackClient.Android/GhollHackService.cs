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
using Android.Content.Res;

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

            string content;
            AssetManager assets = MainActivity.StaticAssets;
            using(StreamReader sr = new StreamReader(assets.Open("GNHAssets/credits")))
            {
                content = sr.ReadToEnd();
            }
            if (File.Exists(Path.Combine(cwd10, "credits")))
                File.Delete(Path.Combine(cwd10, "credits"));

            using (StreamWriter sw = new StreamWriter(Path.Combine(cwd10, "credits")))
            {
                sw.Write(content);
            }

            using (StreamReader sr = new StreamReader(assets.Open("GNHAssets/record")))
            {
                content = sr.ReadToEnd();
            }
            if (File.Exists(Path.Combine(cwd10, "record")))
                File.Delete(Path.Combine(cwd10, "record"));

            using (StreamWriter sw = new StreamWriter(Path.Combine(cwd10, "record")))
            {
                sw.Write(content);
            }

            using (StreamReader sr = new StreamReader(assets.Open("GNHAssets/sysconf")))
            {
                content = sr.ReadToEnd();
            }
            if (File.Exists(Path.Combine(cwd10, "sysconf")))
                File.Delete(Path.Combine(cwd10, "sysconf"));

            using (StreamWriter sw = new StreamWriter(Path.Combine(cwd10, "sysconf")))
            {
                sw.Write(content);
            }

            using (StreamReader sr = new StreamReader(assets.Open("GNHAssets/symbols")))
            {
                content = sr.ReadToEnd();
            }
            if (File.Exists(Path.Combine(cwd10, "symbols")))
                File.Delete(Path.Combine(cwd10, "symbols"));

            using (StreamWriter sw = new StreamWriter(Path.Combine(cwd10, "symbols")))
            {
                sw.Write(content);
            }

            byte[] data;
            int cnt = 2048 * 1024;
            using (BinaryReader br = new BinaryReader(assets.Open("GNHAssets/nhdat")))
            {
                data = br.ReadBytes(cnt);
            }
            if (File.Exists(Path.Combine(cwd10, "nhdat")))
                File.Delete(Path.Combine(cwd10, "nhdat"));
            using (BinaryWriter sw = new BinaryWriter(File.Open(Path.Combine(cwd10, "nhdat"), FileMode.Create)))
            {
                sw.Write(data);
            }

            return RunGnollHackTest(cwd4, cwd10, cwd3);
        }
    }
}