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

namespace GnollHackClient.Droid
{
    [Activity(Label = "GnollHack", Icon = "@mipmap/icon", Theme = "@style/MainTheme", MainLauncher = true, ConfigurationChanges = ConfigChanges.ScreenSize | ConfigChanges.Orientation | ConfigChanges.UiMode | ConfigChanges.ScreenLayout | ConfigChanges.SmallestScreenSize )]
    public class MainActivity : global::Xamarin.Forms.Platform.Android.FormsAppCompatActivity
    {
        public static MainActivity CurrentMainActivity = null;

        public static AssetManager StaticAssets;

        const int REQUEST_USER_CONFIRM_INSTALL_CODE = 101;

        public IAssetPackManager AssetPackManager { get; private set; }
        public AssetPackStateUpdateListenerWrapper listener { get; private set; }

        protected override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);
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
                    SystemUiFlags systemUiVisibility = (SystemUiFlags)Window.DecorView.SystemUiVisibility;
                    systemUiVisibility |= SystemUiFlags.HideNavigation;
                    systemUiVisibility |= SystemUiFlags.Immersive;
                    Window.DecorView.SystemUiVisibility = (StatusBarVisibility)systemUiVisibility;
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
                    SystemUiFlags systemUiVisibility = (SystemUiFlags)Window.DecorView.SystemUiVisibility;
                    systemUiVisibility &= ~SystemUiFlags.HideNavigation;
                    systemUiVisibility &= ~SystemUiFlags.Immersive;
                    Window.DecorView.SystemUiVisibility = (StatusBarVisibility)systemUiVisibility;
                }
            });

            Platform.Init(this, savedInstanceState);
            global::Xamarin.Forms.Forms.Init(this, savedInstanceState);
            Org.Fmod.FMOD.Init(this);

            MainActivity.StaticAssets = this.Assets;
            CurrentMainActivity = this;
            FFImageLoading.Forms.Platform.CachedImageRenderer.Init(enableFastRenderer: true);

            AssetPackManager = AssetPackManagerFactory.GetInstance(this);
            listener = new AssetPackStateUpdateListenerWrapper();
            listener.StateUpdate += Listener_StateUpdate;
            LoadApplication(new App());
        }

        private string GetAbsoluteOnDemandPackAssetPath(string assetPack, string relativeAssetPath)
        {
            AssetPackLocation assetPackPath = AssetPackManager.GetPackLocation(assetPack);
            string assetsFolderPath = assetPackPath?.AssetsPath() ?? null;
            if (assetsFolderPath == null)
            {
                // asset pack is not ready
                return null;
            }

            string assetPath = Path.Combine(assetsFolderPath, relativeAssetPath);
            return assetPath;
        }

        private void Listener_StateUpdate(object sender, EventArgs e)
        {
            //var status = e.State.Status();
            int i = 0;
            switch (i)
            {
                case AssetPackStatus.Pending:
                    break;
                case AssetPackStatus.Downloading:
                    //long downloaded = e.State.BytesDownloaded();
                    //long totalSize = e.State.TotalBytesToDownload();
                    //double percent = 100.0 * downloaded / totalSize;
                    //Android.Util.Log.Info("DynamicAssetsExample", $"Dowloading {percent}");
                    break;

                case AssetPackStatus.Transferring:
                    // 100% downloaded and assets are being transferred.
                    // Notify user to wait until transfer is complete.
                    break;

                case AssetPackStatus.Completed:
                    // Asset pack is ready to use.
                    //string path = GetAbsoluteOnDemandPackAssetPath("assetsfeature", "Foo.txt");
                    //if (path != null)
                    //{
                    //    Android.Util.Log.Info("DynamicAssetsExample", $"Reading {path}");
                    //    Android.Util.Log.Info("DynamicAssetsExample", File.ReadAllText(path));
                    //}
                    break;

                case AssetPackStatus.Failed:
                    // Request failed. Notify user.
                    break;

                case AssetPackStatus.Canceled:
                    // Request canceled. Notify user.
                    break;

                case AssetPackStatus.WaitingForWifi:
                    // wait for wifi
                    AssetPackManager.ShowCellularDataConfirmation(this);
                    break;

                case AssetPackStatus.NotInstalled:
                    // Asset pack is not downloaded yet.
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
            // regsiter our Listener Wrapper with the SplitInstallManager so we get feedback.
            AssetPackManager.RegisterListener(listener.Listener);
            base.OnResume();
        }

        protected override void OnPause()
        {
            AssetPackManager.UnregisterListener(listener.Listener);
            base.OnPause();
        }

        public override void OnRequestPermissionsResult(int requestCode, string[] permissions, [GeneratedEnum] Android.Content.PM.Permission[] grantResults)
        {
            Platform.OnRequestPermissionsResult(requestCode, permissions, grantResults);

            base.OnRequestPermissionsResult(requestCode, permissions, grantResults);
        }

        public override async void OnBackPressed()
        {
            if (await App.OnBackButtonPressed())
            {
                base.OnBackPressed();
            }
        }

        public override bool OnKeyUp([GeneratedEnum] Keycode keyCode, KeyEvent e)
        {
            //process key press
            return base.OnKeyUp(keyCode, e);
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