using Foundation;
using WebKit;

#if GNH_MAUI
namespace GnollHackM
#else
namespace GnollHackX.iOS
#endif
{
    /// <summary>
    /// iOS WKScriptMessage handler for the Overseer v2 client tool bridge.
    /// Registered as "gnollhackBridge", allowing Angular to call
    /// <c>window.webkit.messageHandlers.gnollhackBridge.postMessage(jsonString)</c>.
    /// </summary>
    public class OverseerScriptMessageHandler : NSObject, IWKScriptMessageHandler
    {
        private readonly OverseerPage _page;

        public OverseerScriptMessageHandler(OverseerPage page)
        {
            _page = page;
        }

        public void DidReceiveScriptMessage(WKUserContentController controller,
                                             WKScriptMessage message)
        {
            string json = message.Body?.ToString();
            if (!string.IsNullOrEmpty(json))
            {
                _page.HandleToolRequestFromBridge(json);
            }
        }
    }
}
