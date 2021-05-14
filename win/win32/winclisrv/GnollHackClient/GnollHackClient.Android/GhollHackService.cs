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
        private string _gnollhackfilesdir;

        [DllImport(@"libgnollhackdroid.so")]
        public static extern int DoSomeCalcDroid();
        [DllImport(@"libgnollhackdroid.so")]
        public static extern int DoSomeHackDroid();
        [DllImport(@"libgnollhackdroid.so")]
        public static extern int RunGnollHackTest([MarshalAs(UnmanagedType.LPStr)] string gnhdir);

        private void LoadNativeLibrary(string libName)
        {
            Java.Lang.JavaSystem.LoadLibrary(libName);
        }

        public void InitializeGnollHack()
        {
            LoadNativeLibrary("gnollhackdroid");

            /* Unpack GnollHack files */
            /* Add a check whether to unpack if there are existing files or not */

            string filesdir = Android.App.Application.Context.FilesDir.Path;
            _gnollhackfilesdir = filesdir;

            string content;
            AssetManager assets = MainActivity.StaticAssets;

            string assetsourcedir = "gnh";
            string[] txtfileslist = { "credits" , "license", "logfile", "perm", "record", "recover", "symbols", "sysconf" , "xlogfile" };

            foreach(string txtfile in txtfileslist)
            {
                string fullsourcepath = Path.Combine(assetsourcedir, txtfile);
                using (StreamReader sr = new StreamReader(assets.Open(fullsourcepath)))
                {
                    content = sr.ReadToEnd();
                }
                string fulltargetpath = Path.Combine(filesdir, txtfile);
                if (File.Exists(fulltargetpath))
                    File.Delete(fulltargetpath);

                using (StreamWriter sw = new StreamWriter(fulltargetpath))
                {
                    sw.Write(content);
                }
            }

            string[] binfileslist = { "nhdat" };
            byte[] data;
            int maxsize = 2048 * 1024;
            foreach (string binfile in binfileslist)
            {
                string fullsourcepath = Path.Combine(assetsourcedir, binfile);

                using (BinaryReader br = new BinaryReader(assets.Open(fullsourcepath)))
                {
                    data = br.ReadBytes(maxsize);
                }

                string fulltargetpath = Path.Combine(filesdir, binfile);
                if (File.Exists(fulltargetpath))
                    File.Delete(fulltargetpath);

                using (BinaryWriter sw = new BinaryWriter(File.Open(fulltargetpath, FileMode.Create)))
                {
                    sw.Write(data);
                }
            }
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
            /*
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
            */
            //string cwd4 = System.AppDomain.CurrentDomain.BaseDirectory;

            return RunGnollHackTest(_gnollhackfilesdir);
        }
    }
}