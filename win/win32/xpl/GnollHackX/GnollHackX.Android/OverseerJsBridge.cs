using Android.Webkit;
using Java.Interop;

#if GNH_MAUI
namespace GnollHackM
#else
namespace GnollHackX.Droid
#endif
{
    /// <summary>
    /// Android JavaScript interface for the Overseer v2 client tool bridge.
    /// Registered on the WebView as "GnollHackBridge", allowing Angular to call
    /// <c>window.GnollHackBridge.onToolRequest(jsonString)</c>.
    /// </summary>
    public class OverseerJsBridge : Java.Lang.Object
    {
        private readonly OverseerPage _page;

        public OverseerJsBridge(OverseerPage page)
        {
            _page = page;
        }

        [JavascriptInterface]
        [Export("onToolRequest")]
        public void OnToolRequest(string json)
        {
            _page.HandleToolRequestFromBridge(json);
        }
    }
}
