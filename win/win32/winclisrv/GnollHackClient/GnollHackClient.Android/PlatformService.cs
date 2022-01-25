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
using Android;
using Java.IO;

[assembly: Dependency(typeof(GnollHackClient.Droid.PlatformService))]
namespace GnollHackClient.Droid
{
    public class PlatformService : IPlatformService
    {
        public void CloseApplication()
        {
            MainActivity.CurrentMainActivity.Finish();
        }
        public bool IsModernAndroid()
        {
            return Build.VERSION.SdkInt >= BuildVersionCodes.R;
        }

        public void SaveFileToDownloads(byte[] data, string name)
        {
            if (MainActivity.CurrentMainActivity?.CheckSelfPermission(Manifest.Permission.WriteExternalStorage) != Android.Content.PM.Permission.Granted)
            {
                MainActivity.CurrentMainActivity?.RequestPermissions(new string[] { Manifest.Permission.WriteExternalStorage }, 0);
            }

            if (MainActivity.CurrentMainActivity?.CheckSelfPermission(Manifest.Permission.WriteExternalStorage) == Android.Content.PM.Permission.Granted)
            {
                string path = Android.OS.Environment.GetExternalStoragePublicDirectory(Android.OS.Environment.DirectoryDownloads).AbsolutePath;
                string filePath = System.IO.Path.Combine(path, name);

                if(System.IO.File.Exists(filePath))
                {
                    System.IO.FileInfo file = new System.IO.FileInfo(filePath);
                    file.Delete();
                }
                FileOutputStream fileOutputStream = new FileOutputStream(new Java.IO.File(filePath));
                fileOutputStream.Write(data);
                fileOutputStream.Close();
            }
        }
    }
}