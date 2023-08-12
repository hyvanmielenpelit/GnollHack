using Android.App;
using Android.Runtime;
using System.Runtime.InteropServices;
using static AndroidX.Navigation.Navigator;

namespace GnollHackM;

[Application]
public class MainApplication : MauiApplication
{
    [DllImport(PlatformConstants.dll)]
    public static extern int LibTest();

    public MainApplication(IntPtr handle, JniHandleOwnership ownership)
    : base(handle, ownership)
	{
        Java.Lang.JavaSystem.LoadLibrary(PlatformConstants.library);
        int res = LibTest();
        System.Diagnostics.Debug.WriteLine(res);
        MainActivity.StaticAssets = this.Assets;
    }

    protected override MauiApp CreateMauiApp() => MauiProgram.CreateMauiApp();
}
