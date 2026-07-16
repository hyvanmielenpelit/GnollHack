using GnollHackM.Platforms.MacCatalyst;
using ObjCRuntime;
using System.Runtime.InteropServices;
using UIKit;

namespace GnollHackM;

public class Program
{
    [DllImport(PlatformConstants.dll)]
    public static extern int LibTest();

    static void Main(string[] args)
    {
        int res = LibTest();
        System.Diagnostics.Debug.WriteLine(res);

        UIApplication.Main(args, typeof(GHUIApplication), typeof(AppDelegate));
    }
}
