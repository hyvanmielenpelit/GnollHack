using Android.App;
using Android.Content.PM;
using Android.Content.Res;
using Android.OS;
using Android.Runtime;
using Android.Views;
using GnollHackX;

namespace GnollHackM;

[Activity(Theme = "@style/Maui.SplashTheme", MainLauncher = true, ConfigurationChanges = ConfigChanges.ScreenSize | ConfigChanges.Orientation | ConfigChanges.UiMode | ConfigChanges.ScreenLayout | ConfigChanges.SmallestScreenSize | ConfigChanges.Density | ConfigChanges.Keyboard | ConfigChanges.KeyboardHidden | ConfigChanges.Navigation)]
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
        IsHardKeyboardConnected = Resources?.Configuration?.HardKeyboardHidden == HardKeyboardHidden.No ? true : false;
        GHApp.InitializeGC();
        Window.AddFlags(WindowManagerFlags.Fullscreen);
        if (DefaultShowNavigationBar)
            ShowOsNavigationBar();
        else
            HideOsNavigationBar();

        CreateSaveGameNotificationChannel();
    }

    private void CreateSaveGameNotificationChannel()
    {
        if (Build.VERSION.SdkInt >= BuildVersionCodes.O)
        {
            try
            {
                var channel = new NotificationChannel(
                    GHConstants.SAVE_GAME_NOTIFICATION_CHANNEL_ID,
                    "Game Save Progress",
                    NotificationImportance.Default)
                {
                    Description = "Used while saving your game data."
                };

                var manager = (NotificationManager)GetSystemService(NotificationService);
                manager.CreateNotificationChannel(channel);
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog(ex.Message);
            }
        }
    }

    public override bool OnKeyDown([GeneratedEnum] Keycode keyCode, KeyEvent e)
    {
        bool wasHandled = PlatformService.HandleOnKeyDown(keyCode, e);
        if (!wasHandled)
            return base.OnKeyDown(keyCode, e);
        else
            return true;
    }

    //private static readonly object _activityLock = new object();
    private static int _isHardKeyboardConnected = 0;
    public static bool IsHardKeyboardConnected
    {
        get { return Interlocked.CompareExchange(ref _isHardKeyboardConnected, 0, 0) != 0; }
        set { Interlocked.Exchange(ref _isHardKeyboardConnected, value ? 1 : 0); }
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
