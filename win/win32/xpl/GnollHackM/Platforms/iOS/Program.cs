using ObjCRuntime;
using System.Runtime.InteropServices;
using UIKit;

namespace GnollHackM;

public class Program
{
    [DllImport(PlatformConstants.dll)]
    public static extern int LibTest();

    // This is the main entry point of the application.
    static void Main(string[] args)
	{
        // if you want to use a different Application Delegate class from "AppDelegate"
        // you can specify it here.
        int res = LibTest();
        System.Diagnostics.Debug.WriteLine(res);

        UIApplication.Main(args, null, typeof(AppDelegate));
	}
}
