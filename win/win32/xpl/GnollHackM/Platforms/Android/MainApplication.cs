using Android.App;
using Android.Content.Res;
using Android.OS;
using Android.Runtime;
using Android.Views;
using GnollHackX;
using System.Runtime.InteropServices;
using static AndroidX.Navigation.Navigator;

namespace GnollHackM;

[Application]
public class MainApplication : MauiApplication
{
    [DllImport(PlatformConstants.dll)]
    public static extern int LibTest();

    public static AssetManager StaticAssets;

    public MainApplication(IntPtr handle, JniHandleOwnership ownership)
    : base(handle, ownership)
	{
        try
        {
            GHStartupTimeline.Current.SetProcessAgeNow(SystemClock.ElapsedRealtime() - Android.OS.Process.StartElapsedRealtime);
        }
        catch (Exception ex)
        {
            System.Diagnostics.Debug.WriteLine("MainApplication process age: " + ex.Message);
        }
        Java.Lang.JavaSystem.LoadLibrary(PlatformConstants.library);
        int res = LibTest();
        System.Diagnostics.Debug.WriteLine(res);
        MainActivity.StaticAssets = Assets;
        StaticAssets = Assets;
        FmodService.AndroidInit(this);

        // WORKAROUND(WA-MAUI-0016): Framework teardown raises ObjectDisposedException on Android
        /* Runs before Sentry's raiser handler, which captures the exception either way;
           a handled framework-teardown ObjectDisposedException becomes a warning in BeforeSend */
        AndroidEnvironment.UnhandledExceptionRaiser += (s, e) =>
        {
            if (FrameworkObjectDisposedFilter.FindFrameworkTeardownDisposal(e.Exception) != null)
            {
                e.Handled = true;
            }
        };
    }

    /* MauiApplication.OnCreate creates the MAUI app and then the App, so a stall in either is covered by the sampler */
    public override void OnCreate()
    {
        GHStartupTimeline.Current.Mark(GHStartupMilestone.AppCreate);
#if SENTRY
        // WORKAROUND(WA-SENTRY-0012): Sentry reports no Android process deaths other than native crashes and ANRs
        if (OperatingSystem.IsAndroidVersionAtLeast(30))
            AndroidMemorySampler.Start();
#endif
        base.OnCreate();
    }

    protected override MauiApp CreateMauiApp() => MauiProgram.CreateMauiApp();
}
