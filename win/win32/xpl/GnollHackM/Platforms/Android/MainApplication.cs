using Android.App;
using Android.Runtime;
using System.Runtime.InteropServices;
using static AndroidX.Navigation.Navigator;

namespace GnollHackM;

public class PlatformConstants
{
#if __IOS__
        public const string dll     = "__Internal";
        public const string library = "gnollhackios";
#elif __ANDROID__
    public const string dll = @"libgnollhackdroid.so";
    public const string library = "gnollhackdroid";
#else
        public const string dll     = @"libgnollhackunknown.so";
        public const string library = "gnollhackunknown";
#endif
}


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
	}

	protected override MauiApp CreateMauiApp() => MauiProgram.CreateMauiApp();
}
