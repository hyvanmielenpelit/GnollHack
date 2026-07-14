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

        bool edge2edge = GHApp.Edge2Edge; // Microsoft.Maui.Storage.Preferences.Get("Edge2Edge", false);
        if (edge2edge && Window != null)
        {
            // ANDROID 15 / .NET MAUI BUG WORKAROUND:
            // MAUI forcefully applies WindowInsets padding to the root view on Android 15+, ignoring SafeAreaEdges="None".
            // We intercept the insets at the DecorView level to force 0 padding.
            AndroidX.Core.View.ViewCompat.SetOnApplyWindowInsetsListener(Window.DecorView, new BypassInsetsListener());
            Window.DecorView.RequestApplyInsets();
        }
        else
        {
            // Reapply legacy fullscreen flag if edge-to-edge is not requested,
            // restoring the original behavior where the notch area is letterboxed.
            Window?.AddFlags(WindowManagerFlags.Fullscreen);
        }

        if (DefaultShowNavigationBar) 
            ShowOsNavigationBar();
        else
            HideOsNavigationBar();

        CreateSaveGameNotificationChannel();
    }

    protected override void OnDestroy()
    {
        try
        {
            base.OnDestroy();
        }
        catch (Exception ex)
        {
            System.Diagnostics.Debug.WriteLine(ex.Message);
            if (ex is ObjectDisposedException ioe)
            {
                SentrySdk.CaptureMessage($"Caught ObjectDisposedException in MainActivity.OnDestroy: {ioe.Message}", SentryLevel.Warning);
            }
        }
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
        if (activity == null || activity.Window == null)
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
            if (activity.Window.DecorView != null)
            {
                SystemUiFlags systemUiVisibility = (SystemUiFlags)activity.Window.DecorView.SystemUiVisibility;
                systemUiVisibility |= SystemUiFlags.HideNavigation;
                systemUiVisibility |= SystemUiFlags.Immersive;
                activity.Window.DecorView.SystemUiVisibility = (StatusBarVisibility)systemUiVisibility;
            }
#pragma warning restore CS0618 // Type or member is obsolete
        }
    }
    public static void ShowOsNavigationBar()
    {
        Activity activity = CurrentMainActivity;
        if (activity == null || activity.Window == null)
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
            if (activity.Window.DecorView != null)
            {
                SystemUiFlags systemUiVisibility = (SystemUiFlags)activity.Window.DecorView.SystemUiVisibility;
                systemUiVisibility &= ~SystemUiFlags.HideNavigation;
                systemUiVisibility &= ~SystemUiFlags.Immersive;
                activity.Window.DecorView.SystemUiVisibility = (StatusBarVisibility)systemUiVisibility;
            }
#pragma warning restore CS0618 // Type or member is obsolete
        }
    }

    private class BypassInsetsListener : Java.Lang.Object, AndroidX.Core.View.IOnApplyWindowInsetsListener
    {
        public AndroidX.Core.View.WindowInsetsCompat OnApplyWindowInsets(Android.Views.View v, AndroidX.Core.View.WindowInsetsCompat insets)
        {
            // Force padding to 0 to completely ignore safe areas and draw under the notch
            v.SetPadding(0, 0, 0, 0);
            
            // Return consumed insets so MAUI's internal handlers don't add padding back
            return AndroidX.Core.View.WindowInsetsCompat.Consumed ?? insets;
        }
    }
}
