using Android.Webkit;
using Java.Interop;

#if GNH_MAUI
namespace GnollHackM
#else
namespace GnollHackX.Droid
#endif
{
    /// <summary>
    /// Android JavaScript interface for the Overseer v2 web message bridge.
    /// Registered on the WebView as "GnollHackBridge", allowing Angular to call
    /// <c>window.GnollHackBridge.onWebMessage(jsonString)</c>.
    /// Uses a WeakReference to avoid preventing garbage collection of the
    /// OverseerPage after the page is dismissed.
    /// </summary>
    public class OverseerJsBridge : Java.Lang.Object
    {
        private readonly WeakReference<OverseerPage> _pageRef;

        public OverseerJsBridge(OverseerPage page)
        {
            _pageRef = new WeakReference<OverseerPage>(page);
        }

        [JavascriptInterface]
        [Export("onWebMessage")]
        public void OnWebMessage(string json)
        {
            if (_pageRef.TryGetTarget(out var page))
            {
                page.HandleWebMessageFromBridge(json);
            }
        }
    }
}
