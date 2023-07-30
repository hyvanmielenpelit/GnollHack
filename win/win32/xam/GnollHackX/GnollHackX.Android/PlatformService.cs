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
using GnollHackX;
using Android;
using Java.IO;
using System.IO;
using Xamarin.Google.Android.Play.Core.AssetPacks;
using Android.Content.PM;
using System.Runtime.Remoting.Contexts;
using Xamarin.Google.Android.Play.Core.Review;
using System.Threading.Tasks;
using Xamarin.Google.Android.Play.Core.Review.Model;
using Xamarin.Google.Android.Play.Core.Tasks;
using System.Threading;

[assembly: Dependency(typeof(GnollHackX.Droid.PlatformService))]
namespace GnollHackX.Droid
{
    public class PlatformService : IPlatformService
    {
        public string GetVersionString()
        {
            var context = Android.App.Application.Context;
            if (Build.VERSION.SdkInt >= Android.OS.BuildVersionCodes.Tiramisu)
            {
                return context.PackageManager.GetPackageInfo(context.PackageName, PackageManager.PackageInfoFlags.Of(0L)).VersionName;
            }
            else
            {
#pragma warning disable CS0618 // Type or member is obsolete
                return context.PackageManager.GetPackageInfo(context.PackageName, 0).VersionName;
#pragma warning restore CS0618 // Type or member is obsolete            }
            }
        }

        public ulong GetDeviceMemoryInBytes()
        {
            try
            {
                var activityManager = Android.App.Application.Context.GetSystemService(Activity.ActivityService) as ActivityManager;
                var memoryInfo = new ActivityManager.MemoryInfo();
                activityManager.GetMemoryInfo(memoryInfo);

                long totalRam = memoryInfo.TotalMem;

                return (ulong)totalRam;
            }
            catch
            {
                return 0;
            }
        }

        public ulong GetDeviceFreeDiskSpaceInBytes()
        {
            try
            {
                //Using StatFS
                var path = new StatFs(System.Environment.GetFolderPath(System.Environment.SpecialFolder.LocalApplicationData));
                long blockSize = path.BlockSizeLong;
                long avaliableBlocks = path.AvailableBlocksLong;
                long freeSpace = blockSize * avaliableBlocks;
                return (ulong)freeSpace;
            }
            catch
            {
                return 0;
            }
        }

        public ulong GetDeviceTotalDiskSpaceInBytes()
        {
            try
            {
                //Using StatFS
                var path = new StatFs(System.Environment.GetFolderPath(System.Environment.SpecialFolder.LocalApplicationData));
                long freeSpace = path.TotalBytes;
                return (ulong)freeSpace;
            }
            catch
            {
                return 0;
            }
        }


        public void CloseApplication()
        {
            RevertAnimatorDuration(true);
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

        public float GetAnimatorDurationScaleSetting()
        {
            var resolver = Android.App.Application.Context.ContentResolver;
            var scaleName = Android.Provider.Settings.Global.AnimatorDurationScale;
            float scale = Android.Provider.Settings.Global.GetFloat(resolver, scaleName, 1.0f);
            return scale;
        }

        public float GetTransitionAnimationScaleSetting()
        {
            var resolver = Android.App.Application.Context.ContentResolver;
            var scaleName = Android.Provider.Settings.Global.TransitionAnimationScale;
            float scale = Android.Provider.Settings.Global.GetFloat(resolver, scaleName, 1.0f);
            return scale;
        }

        public float GetWindowAnimationScaleSetting()
        {
            var resolver = Android.App.Application.Context.ContentResolver;
            var scaleName = Android.Provider.Settings.Global.WindowAnimationScale;
            float scale = Android.Provider.Settings.Global.GetFloat(resolver, scaleName, 1.0f);
            return scale;
        }

        public bool IsRemoveAnimationsOn()
        {
            var scale1 = GetAnimatorDurationScaleSetting();
            var scale2 = GetTransitionAnimationScaleSetting();
            var scale3 = GetWindowAnimationScaleSetting();

            return scale1 == 0 && scale2 == 0 && scale3 == 0;
        }

        public float GetCurrentAnimatorDurationScale()
        {
            float scale = -1.0f;
            try
            {
                var classForName = JNIEnv.FindClass("android/animation/ValueAnimator");
                var methodId = JNIEnv.GetStaticMethodID(classForName, "getDurationScale", "()F");

                scale = JNIEnv.CallStaticFloatMethod(classForName, methodId);
            }
            catch //(Exception ex)
            {
                // Log Android Error or try / catch should be moved to the calling function
            }
            return scale;
        }

        private bool _originalSet = false;
        private float _originalAnimationDurationScale = 1.0f;
        public void OverrideAnimatorDuration()
        {
            float scale = GetAnimatorDurationScaleSetting();

            if (scale == 1.0f)
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
                // Log Android Error or try / catch should be moved to the calling function
            }
        }
        public void RevertAnimatorDuration(bool isfinal)
        {
            if (_originalSet && _originalAnimationDurationScale != 1.0f)
            {
                try
                {
                    var classForName = JNIEnv.FindClass("android/animation/ValueAnimator");
                    var methodId = JNIEnv.GetStaticMethodID(classForName, "setDurationScale", "(F)V");

                    float usedValue = !isfinal && _originalAnimationDurationScale == 0.0f ? 0.1f : _originalAnimationDurationScale; //Make sure that the value is not set to zero upon sleep just in case, since that seems to create problems
                    JNIEnv.CallStaticVoidMethod(classForName, methodId, new JValue(usedValue));
                }
                catch //(Exception ex)
                {
                    // Log Android Error or try / catch should be moved to the calling function
                }

                _originalSet = false;
                _originalAnimationDurationScale = 1.0f;
            }
        }

        public void RequestAppReview()
        {
            //IReviewManager manager = ReviewManagerFactory.Create(MainActivity.CurrentMainActivity);
            //if(manager != null)
            //{
            //    try
            //    {
            //        var request = manager.RequestReviewFlow();
            //        Java.Lang.Object result = null;
            //        if (request != null)
            //        {
            //            var task = ToAwaitableTask(request);
            //            if(task != null)
            //                result = await task;
            //        }
            //        if(result != null)
            //        {
            //            ReviewInfo info = (ReviewInfo)result;
            //            var flow = manager.LaunchReviewFlow(MainActivity.CurrentMainActivity, info);
            //            Java.Lang.Object flowresult = null;
            //            var flowtask = ToAwaitableTask(flow);
            //            if(flowtask != null)
            //                flowresult = await flowtask;
            //            if(flowresult != null)
            //            {
            //                string msg = "";
            //                //Continue
            //            }
            //        }
            //    }
            //    catch (Exception ex)
            //    {
            //        string msg = ex.Message;
            //        //Debuglog
            //    }
            //}
        }

        public string GetBaseUrl()
        {
            return "file:///android_asset/";
        }
        public string GetAssetsPath()
        {

            return "file:///android_asset/";
        }

        public string GetAbsoluteOnDemandAssetPath(string assetPack)
        {
            if (MainActivity.CurrentMainActivity == null || MainActivity.CurrentMainActivity.AssetPackManager == null)
                return null;

            var assetPackPath = MainActivity.CurrentMainActivity.AssetPackManager.GetPackLocation(assetPack);
            return assetPackPath?.AssetsPath() ?? null;
        }

        public string GetAbsoluteOnDemandAssetPath(string assetPack, string relativeAssetPath)
        {
            if (MainActivity.CurrentMainActivity == null || MainActivity.CurrentMainActivity.AssetPackManager == null)
                return null;

            string assetsFolderPath = GetAbsoluteOnDemandAssetPath(assetPack);
            if (assetsFolderPath == null)
            {
                // asset pack is not ready
                return null;
            }

            string assetPath = Path.Combine(assetsFolderPath, relativeAssetPath);
            return assetPath;
        }

        public int FetchOnDemandPack(string pack)
        {
            if (MainActivity.CurrentMainActivity == null || MainActivity.CurrentMainActivity.AssetPackManager == null)
                return 2; /* No asset pack manager */

            var location = MainActivity.CurrentMainActivity.AssetPackManager.GetPackLocation(pack);
            if (location == null)
            {
                // TODO Figure out how to use the GetPackStates.
                try
                {
                    var states = MainActivity.CurrentMainActivity.AssetPackManager.GetPackStates(new string[] { pack });
                    // TODO add OnComplete Listeners to the Task returned by Fetch.
                    MainActivity.CurrentMainActivity.AssetPackManager.Fetch(new string[] { pack });
                    return 0;  /* Success */
                }
                catch (Exception ex)
                {
                    string msg = ex.Message;
                    return 1;  /* Exception occurred */
                }
            }
            else
                return -1; /* Already loaded */
        }

        public event EventHandler<AssetPackStatusEventArgs> OnDemandPackStatusNotification;

        public void InitOnDemandPackStatusNotificationEventHandler()
        {
            MainActivity.CurrentMainActivity.OnDemandPackStatus += OnDemandPackStatusNotified;
        }

        private void OnDemandPackStatusNotified(object sender, AssetPackStatusEventArgs e)
        {
            OnDemandPackStatusNotification?.Invoke(this, e);
        }

        //private static Task<Java.Lang.Object> ToAwaitableTask(Xamarin.Google.Android.Play.Core.Tasks.Task task)
        //{
        //    var taskCompletionSource = new TaskCompletionSource<Java.Lang.Object>();
        //    var taskCompleteListener = new TaskCompleteListener(taskCompletionSource);
        //    task.AddOnCompleteListener(taskCompleteListener);
        //    return taskCompletionSource.Task;
        //}
    }


    //public class TaskCompleteListener : Java.Lang.Object, IOnCompleteListener
    //{
    //    private readonly TaskCompletionSource<Java.Lang.Object> _taskCompletionSource;

    //    public TaskCompleteListener(TaskCompletionSource<Java.Lang.Object> tcs) 
    //    {
    //        _taskCompletionSource = tcs;
    //    }

    //    public void OnComplete(Xamarin.Google.Android.Play.Core.Tasks.Task task)
    //    {
    //        var isCompleted = task.IsSuccessful && task.IsComplete;
    //        if (task.IsSuccessful)
    //        {
    //            if(task.IsComplete)
    //                _taskCompletionSource.SetResult(task.GetResult(MainActivity.CurrentMainActivity.Class));
    //            else
    //                _taskCompletionSource.SetCanceled();
    //        }
    //        else
    //        {
    //            _taskCompletionSource.SetException(task.Exception);
    //        }
    //    }
    //}


}