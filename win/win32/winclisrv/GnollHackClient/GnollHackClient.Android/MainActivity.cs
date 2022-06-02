using System;

using Android.App;
using Android.Content.PM;
using Android.Runtime;
using Android.OS;
using Android.Content.Res;
using Android.Views;
using Xamarin.Forms;
using Xamarin.Essentials;

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

            MainActivity.StaticAssets = this.Assets;
            CurrentMainActivity = this;
            FFImageLoading.Forms.Platform.CachedImageRenderer.Init(enableFastRenderer: true);
            LoadApplication(new App());
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
    }
}