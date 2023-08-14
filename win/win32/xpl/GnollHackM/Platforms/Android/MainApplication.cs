using Android.App;
using Android.Content.Res;
using Android.OS;
using Android.Runtime;
using Android.Views;
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
        Java.Lang.JavaSystem.LoadLibrary(PlatformConstants.library);
        int res = LibTest();
        System.Diagnostics.Debug.WriteLine(res);
        MainActivity.StaticAssets = Assets;
        StaticAssets = Assets;
        MainActivity.CurrentMainActivity = Platform.CurrentActivity;
        FmodService.AndroidInit(this);
    }

    protected override MauiApp CreateMauiApp() => MauiProgram.CreateMauiApp();
}
