using Android.App;
using Android.Content.PM;
using Android.Content.Res;
using Android.OS;
//using Xamarin.Google.Android.Play.Core.AssetPacks;
//using Xamarin.Google.Android.Play.Core.AssetPacks.Model;


namespace GnollHackM;

[Activity(Theme = "@style/Maui.SplashTheme", MainLauncher = true, ConfigurationChanges = ConfigChanges.ScreenSize | ConfigChanges.Orientation | ConfigChanges.UiMode | ConfigChanges.ScreenLayout | ConfigChanges.SmallestScreenSize | ConfigChanges.Density)]
public class MainActivity : MauiAppCompatActivity
{
    public static MainActivity CurrentMainActivity = null;

    public static AssetManager StaticAssets;

    const int REQUEST_USER_CONFIRM_INSTALL_CODE = 101;

    //public IAssetPackManager AssetPackManager { get; private set; }
    //public AssetPackStateUpdateListenerWrapper AssetPackListener { get; private set; }
}
