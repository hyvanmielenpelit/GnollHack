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
    /// </summary>
    public class OverseerJsBridge : Java.Lang.Object
    {
        private readonly OverseerPage _page;

        public OverseerJsBridge(OverseerPage page)
        {
            _page = page;
        }

        [JavascriptInterface]
        [Export("onWebMessage")]
        public void OnWebMessage(string json)
        {
            _page.HandleWebMessageFromBridge(json);
        }
    }
}
