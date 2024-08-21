using Android.App;
using Android.Content.PM;
using Android.Content.Res;
using Android.OS;
using Android.Views;
using GnollHackX;
//using Xamarin.Google.Android.Play.Core.AssetPacks;
//using Xamarin.Google.Android.Play.Core.AssetPacks.Model;


namespace GnollHackM;

[Activity(Theme = "@style/Maui.SplashTheme", MainLauncher = true, ConfigurationChanges = ConfigChanges.ScreenSize | ConfigChanges.Orientation | ConfigChanges.UiMode | ConfigChanges.ScreenLayout | ConfigChanges.SmallestScreenSize | ConfigChanges.Density)]
public class MainActivity : MauiAppCompatActivity
{
    public static Activity CurrentMainActivity = null;
    public static bool DefaultShowNavigationBar = false;
    public static AssetManager StaticAssets;
    protected override void OnCreate(Bundle savedInstanceState)
    {
        base.OnCreate(savedInstanceState);
        CurrentMainActivity = this;
        Platform.Init(this, savedInstanceState);
        GHApp.InitializeGC();
        Window.AddFlags(WindowManagerFlags.Fullscreen);
        if (DefaultShowNavigationBar)
            ShowOsNavigationBar();
        else
            HideOsNavigationBar();
    }

    public static void HideOsNavigationBar()
    {
        Activity activity = CurrentMainActivity;
        if (activity == null)
            return;
        if (Build.VERSION.SdkInt >= BuildVersionCodes.R)
        {
#pragma warning disable CA1416 // Supported on: 'android' 30.0 and later
            activity.Window.SetDecorFitsSystemWindows(false);
            activity.Window.InsetsController?.Hide(WindowInsets.Type.SystemBars());
            if (activity.Window.InsetsController != null)
                activity.Window.InsetsController.SystemBarsBehavior = (int)WindowInsetsControllerBehavior.ShowTransientBarsBySwipe;
#pragma warning restore CA1416 // Supported on: 'android' 30.0 and later
        }
        else
        {
#pragma warning disable CS0618 // Type or member is obsolete
            SystemUiFlags systemUiVisibility = (SystemUiFlags)activity.Window.DecorView.SystemUiVisibility;
            systemUiVisibility |= SystemUiFlags.HideNavigation;
            systemUiVisibility |= SystemUiFlags.Immersive;
            activity.Window.DecorView.SystemUiVisibility = (StatusBarVisibility)systemUiVisibility;
#pragma warning restore CS0618 // Type or member is obsolete
        }
    }
    public static void ShowOsNavigationBar()
    {
        Activity activity = CurrentMainActivity;
        if (activity == null)
            return;
        if (Build.VERSION.SdkInt >= BuildVersionCodes.R)
        {
#pragma warning disable CA1416 // Supported on: 'android' 30.0 and later
            activity.Window.SetDecorFitsSystemWindows(true);
            activity.Window.InsetsController?.Show(WindowInsets.Type.SystemBars());
#pragma warning restore CA1416 // Supported on: 'android' 30.0 and later
        }
        else
        {
#pragma warning disable CS0618 // Type or member is obsolete
            SystemUiFlags systemUiVisibility = (SystemUiFlags)activity.Window.DecorView.SystemUiVisibility;
            systemUiVisibility &= ~SystemUiFlags.HideNavigation;
            systemUiVisibility &= ~SystemUiFlags.Immersive;
            activity.Window.DecorView.SystemUiVisibility = (StatusBarVisibility)systemUiVisibility;
#pragma warning restore CS0618 // Type or member is obsolete
        }
    }
}
