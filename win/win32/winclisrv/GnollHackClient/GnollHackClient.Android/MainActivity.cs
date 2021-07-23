using System;

using Android.App;
using Android.Content.PM;
using Android.Runtime;
using Android.OS;
using Android.Content.Res;
using Android.Views;
using Xamarin.Forms;

namespace GnollHackClient.Droid
{
    [Activity(Label = "GnollHack", Icon = "@mipmap/icon", Theme = "@style/MainTheme", MainLauncher = true, ConfigurationChanges = ConfigChanges.ScreenSize | ConfigChanges.Orientation | ConfigChanges.UiMode | ConfigChanges.ScreenLayout | ConfigChanges.SmallestScreenSize )]
    public class MainActivity : global::Xamarin.Forms.Platform.Android.FormsAppCompatActivity
    {
        public static MainActivity CurrentMainActivity = null;

        public static AssetManager StaticAssets;

        protected override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);
            this.Window.AddFlags(WindowManagerFlags.Fullscreen);
            Rg.Plugins.Popup.Popup.Init(this);
            MessagingCenter.Subscribe<Object>(this, "HideOsNavigationBar", (sender) =>
            {
                int uiOptions = (int)Window.DecorView.SystemUiVisibility;
                uiOptions |= (int)SystemUiFlags.HideNavigation;
                Window.DecorView.SystemUiVisibility = (StatusBarVisibility)uiOptions;
            });

            MessagingCenter.Subscribe<Object>(this, "ShowOsNavigationBar", (sender) =>
            {
                int uiOptions = (int)Window.DecorView.SystemUiVisibility;
                uiOptions &= ~((int)SystemUiFlags.HideNavigation);
                Window.DecorView.SystemUiVisibility = (StatusBarVisibility)uiOptions;
            });

            Xamarin.Essentials.Platform.Init(this, savedInstanceState);
            global::Xamarin.Forms.Forms.Init(this, savedInstanceState);

            MainActivity.StaticAssets = this.Assets;
            CurrentMainActivity = this;
            FFImageLoading.Forms.Platform.CachedImageRenderer.Init(enableFastRenderer: true);
            LoadApplication(new App());
        }

        public override void OnRequestPermissionsResult(int requestCode, string[] permissions, [GeneratedEnum] Android.Content.PM.Permission[] grantResults)
        {
            Xamarin.Essentials.Platform.OnRequestPermissionsResult(requestCode, permissions, grantResults);

            base.OnRequestPermissionsResult(requestCode, permissions, grantResults);
        }

        public override async void OnBackPressed()
        {
            if(await App.OnBackButtonPressed())
            {
                Rg.Plugins.Popup.Popup.SendBackPressed(base.OnBackPressed);
            }
        }
    }
}