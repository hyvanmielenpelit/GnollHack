using System;
using Android.App;
using Android.Content.PM;
using Android.Runtime;
using Android.OS;
using Android.Content.Res;
using Android.Views;
using Xamarin.Forms;
using Xamarin.Essentials;
using System.IO;
using Android.Content;
using Xamarin.Google.Android.Play.Core.AssetPacks;
using Xamarin.Google.Android.Play.Core.AssetPacks.Model;

namespace GnollHackX.Droid
{
    [Activity(Label = "GnollHack", Icon = "@mipmap/icon", Theme = "@style/MainTheme", MainLauncher = true, ConfigurationChanges = ConfigChanges.ScreenSize | ConfigChanges.Orientation | ConfigChanges.UiMode | ConfigChanges.ScreenLayout | ConfigChanges.SmallestScreenSize | ConfigChanges.Keyboard | ConfigChanges.KeyboardHidden | ConfigChanges.Navigation)]
    public class MainActivity : global::Xamarin.Forms.Platform.Android.FormsAppCompatActivity
    {
        public static MainActivity CurrentMainActivity = null;

        public static AssetManager StaticAssets;

        const int REQUEST_USER_CONFIRM_INSTALL_CODE = 101;

        public IAssetPackManager AssetPackManager { get; private set; }
        public AssetPackStateUpdateListenerWrapper AssetPackListener { get; private set; }


        private static readonly object _activityLock = new object();
        private static bool _isHardKeyboardConnected = false;
        public static bool IsHardKeyboardConnected
        {
            get { lock (_activityLock) { return _isHardKeyboardConnected; } }
            set { lock (_activityLock) { _isHardKeyboardConnected = value; } }
        }
        protected override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);
            IsHardKeyboardConnected = Resources?.Configuration?.HardKeyboardHidden == HardKeyboardHidden.No ? true : false;
            Window.AddFlags(WindowManagerFlags.Fullscreen);
            Window.ClearFlags(WindowManagerFlags.ForceNotFullscreen);
            MessagingCenter.Subscribe<Object>(this, "HideOsNavigationBar", (sender) =>
            {
                if (Build.VERSION.SdkInt >= BuildVersionCodes.R)
                {
                    Window.SetDecorFitsSystemWindows(false);
                    Window.InsetsController?.Hide(WindowInsets.Type.NavigationBars());
                }
                else
                {
#pragma warning disable CS0618 // Type or member is obsolete
                    SystemUiFlags systemUiVisibility = (SystemUiFlags)Window.DecorView.SystemUiVisibility;
                    systemUiVisibility |= SystemUiFlags.HideNavigation;
                    systemUiVisibility |= SystemUiFlags.Immersive;
                    Window.DecorView.SystemUiVisibility = (StatusBarVisibility)systemUiVisibility;
#pragma warning restore CS0618 // Type or member is obsolete
                }
            });

            MessagingCenter.Subscribe<Object>(this, "ShowOsNavigationBar", (sender) =>
            {
                if (Build.VERSION.SdkInt >= BuildVersionCodes.R)
                {
                    Window.SetDecorFitsSystemWindows(true);
                    Window.InsetsController?.Show(WindowInsets.Type.NavigationBars());
                }
                else
                {
#pragma warning disable CS0618 // Type or member is obsolete
                    SystemUiFlags systemUiVisibility = (SystemUiFlags)Window.DecorView.SystemUiVisibility;
                    systemUiVisibility &= ~SystemUiFlags.HideNavigation;
                    systemUiVisibility &= ~SystemUiFlags.Immersive;
                    Window.DecorView.SystemUiVisibility = (StatusBarVisibility)systemUiVisibility;
#pragma warning restore CS0618 // Type or member is obsolete
                }
            });

            Platform.Init(this, savedInstanceState);
            global::Xamarin.Forms.Forms.Init(this, savedInstanceState);
            GHApp.InitializeGC();
            FmodService.AndroidInit(this);

            MainActivity.StaticAssets = this.Assets;
            CurrentMainActivity = this;
            //FFImageLoading.Forms.Platform.CachedImageRenderer.Init(enableFastRenderer: true);

            AssetPackManager = AssetPackManagerFactory.GetInstance(this);
            AssetPackListener = new AssetPackStateUpdateListenerWrapper();
            try
            {
                if(AssetPackListener != null)
                    AssetPackListener.StateUpdate += Listener_StateUpdate;
            }
            catch (Exception ex)
            {
                Android.Util.Log.Error("GnollHack Exception", ex.Message);
            }
            LoadApplication(new App());
        }

        private string GetAbsoluteOnDemandPackAssetPath(string assetPack, string relativeAssetPath)
        {
            AssetPackLocation assetPackPath = AssetPackManager?.GetPackLocation(assetPack);
            string assetsFolderPath = assetPackPath?.AssetsPath() ?? null;
            if (assetsFolderPath == null)
            {
                // asset pack is not ready
                return null;
            }

            string assetPath = Path.Combine(assetsFolderPath, relativeAssetPath);
            return assetPath;
        }

        public event EventHandler<AssetPackStatusEventArgs> OnDemandPackStatus;

        private void Listener_StateUpdate(object sender, AssetPackStateUpdateListenerWrapper.AssetPackStateEventArgs e)
        {
            var status = e.State.Status();
            switch (status)
            {
                case AssetPackStatus.Pending:
                    OnDemandPackStatus?.Invoke(null, new AssetPackStatusEventArgs(status));
                    break;
                case AssetPackStatus.Downloading:
                    long downloaded = e.State.BytesDownloaded();
                    long totalSize = e.State.TotalBytesToDownload();
                    OnDemandPackStatus?.Invoke(null, new AssetPackStatusEventArgs(status, downloaded, totalSize));
                    break;

                case AssetPackStatus.Transferring:
                    OnDemandPackStatus?.Invoke(null, new AssetPackStatusEventArgs(status));
                    break;

                case AssetPackStatus.Completed:
                    AssetPackLocation assetPackPath = AssetPackManager?.GetPackLocation(GHConstants.OnDemandPackName);
                    string assetsFolderPath = assetPackPath?.AssetsPath() ?? null;
                    OnDemandPackStatus?.Invoke(null, new AssetPackStatusEventArgs(status, assetsFolderPath));
                    break;

                case AssetPackStatus.Failed:
                    OnDemandPackStatus?.Invoke(null, new AssetPackStatusEventArgs(status));
                    break;

                case AssetPackStatus.Canceled:
                    OnDemandPackStatus?.Invoke(null, new AssetPackStatusEventArgs(status));
                    break;

                case AssetPackStatus.WaitingForWifi:
                    OnDemandPackStatus?.Invoke(null, new AssetPackStatusEventArgs(status));
                    AssetPackManager?.ShowCellularDataConfirmation(this);
                    break;

                case AssetPackStatus.NotInstalled:
                    OnDemandPackStatus?.Invoke(null, new AssetPackStatusEventArgs(status));
                    break;
            }
        }

        protected override void OnDestroy()
        {
            base.OnDestroy();
            Org.Fmod.FMOD.Close();
        }

        protected override void OnResume()
        {
            try
            {
                if (AssetPackListener?.Listener != null)
                    AssetPackManager?.RegisterListener(AssetPackListener.Listener);
            }
            catch (Exception ex)
            {
                Android.Util.Log.Error("GnollHack Exception", ex.Message);
            }
            base.OnResume();
        }

        protected override void OnPause()
        {
            try
            {
                if(AssetPackListener?.Listener != null)
                    AssetPackManager?.UnregisterListener(AssetPackListener.Listener);
            }
            catch (Exception ex)
            {
                Android.Util.Log.Error("GnollHack Exception", ex.Message);
            }
            base.OnPause();
        }

        public override void OnRequestPermissionsResult(int requestCode, string[] permissions, [GeneratedEnum] Android.Content.PM.Permission[] grantResults)
        {
            Platform.OnRequestPermissionsResult(requestCode, permissions, grantResults);

            base.OnRequestPermissionsResult(requestCode, permissions, grantResults);
        }

        [Obsolete("Use GestureNavigation instead")]
        public override async void OnBackPressed()
        {
            if (await GHApp.OnBackButtonPressed())
            {
                base.OnBackPressed();
            }
        }

        public override void OnConfigurationChanged(Configuration newConfig)
        {
            if (newConfig.HardKeyboardHidden == HardKeyboardHidden.No)
            {
                //A hardware keyboard is being connected
                IsHardKeyboardConnected = true;
            }
            else if (newConfig.HardKeyboardHidden == HardKeyboardHidden.Yes)
            {
                //A hardware keyboard is being disconnected
                IsHardKeyboardConnected = false;
            }
            base.OnConfigurationChanged(newConfig);
        }

        public override bool OnKeyUp([GeneratedEnum] Keycode keyCode, KeyEvent e)
        {
            bool wasHandled = PlatformService.HandleOnKeyUp(keyCode, e);
            if (!wasHandled)
                return base.OnKeyUp(keyCode, e);
            else
                return true;
        }

        protected override void OnActivityResult(int requestCode, [GeneratedEnum] Result resultCode, Intent data)
        {
            if (requestCode == REQUEST_USER_CONFIRM_INSTALL_CODE)
            {
                if (resultCode == Result.Canceled)
                    Android.Util.Log.Debug("AssetPack", "User Cancelled.");
                else
                    Android.Util.Log.Debug("AssetPack", "User Accepted.");
            }
            base.OnActivityResult(requestCode, resultCode, data);
        }
    }
}