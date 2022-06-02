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
        public string GetVersionString()
        {
            var context = Android.App.Application.Context;
            return context.PackageManager.GetPackageInfo(context.PackageName, 0).VersionName;
        }

        public void CloseApplication()
        {
            RevertAnimationDuration();
            MainActivity.CurrentMainActivity.Finish();
        }

        public void SetStatusBarHidden(bool ishidden)
        {

        }

        public bool GetStatusBarHidden()
        {
            return true;
        }

        //public void SaveFileToDownloads(byte[] data, string name)
        //{
        //    if (MainActivity.CurrentMainActivity?.CheckSelfPermission(Manifest.Permission.WriteExternalStorage) != Android.Content.PM.Permission.Granted)
        //    {
        //        MainActivity.CurrentMainActivity?.RequestPermissions(new string[] { Manifest.Permission.WriteExternalStorage }, 0);
        //    }

        //    if (MainActivity.CurrentMainActivity?.CheckSelfPermission(Manifest.Permission.WriteExternalStorage) == Android.Content.PM.Permission.Granted)
        //    {
        //        string path = Android.OS.Environment.GetExternalStoragePublicDirectory(Android.OS.Environment.DirectoryDownloads).AbsolutePath;
        //        string filePath = System.IO.Path.Combine(path, name);

        //        if (System.IO.File.Exists(filePath))
        //        {
        //            System.IO.FileInfo file = new System.IO.FileInfo(filePath);
        //            file.Delete();
        //        }
        //        FileOutputStream fileOutputStream = new FileOutputStream(new Java.IO.File(filePath));
        //        fileOutputStream.Write(data);
        //        fileOutputStream.Close();
        //    }
        //}

        private bool _originalSet = false;
        private float _originalAnimationDurationScale = 1.0f;
        public void OverrideAnimationDuration()
        {
            var resolver = Android.App.Application.Context.ContentResolver;
            var scaleName = Android.Provider.Settings.Global.AnimatorDurationScale;
            float scale = Android.Provider.Settings.Global.GetFloat(resolver, scaleName, 0);

            if (scale > 0)
                return;

            if (!_originalSet)
            {
                _originalAnimationDurationScale = scale;
                _originalSet = true;
            }

            try
            {
                var classForName = JNIEnv.FindClass("android/animation/ValueAnimator");
                var methodId = JNIEnv.GetStaticMethodID(classForName, "setDurationScale", "(F)V");

                JNIEnv.CallStaticVoidMethod(classForName, methodId, new JValue(1f));
            }
            catch //(Exception ex)
            {
                // Log Error
            }
        }
        public void RevertAnimationDuration()
        {
            if (_originalSet)
            {
                try
                {
                    var classForName = JNIEnv.FindClass("android/animation/ValueAnimator");
                    var methodId = JNIEnv.GetStaticMethodID(classForName, "setDurationScale", "(F)V");

                    JNIEnv.CallStaticVoidMethod(classForName, methodId, new JValue(_originalAnimationDurationScale));
                }
                catch //(Exception ex)
                {
                    // Log Error
                }

                _originalSet = false;
                _originalAnimationDurationScale = 1.0f;
            }
        }
    }
}