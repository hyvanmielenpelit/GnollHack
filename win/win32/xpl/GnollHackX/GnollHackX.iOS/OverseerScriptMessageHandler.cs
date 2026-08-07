using Foundation;
using WebKit;

#if GNH_MAUI
namespace GnollHackM
#else
namespace GnollHackX.iOS
#endif
{
    /// <summary>
    /// iOS WKScriptMessage handler for the Overseer v2 web message bridge.
    /// Registered as "gnollhackBridge", allowing Angular to call
    /// <c>window.webkit.messageHandlers.gnollhackBridge.postMessage(jsonString)</c>.
    /// Uses a WeakReference to avoid a retain cycle through
    /// WKUserContentController back to the OverseerPage.
    /// </summary>
    public class OverseerScriptMessageHandler : NSObject, IWKScriptMessageHandler
    {
        private readonly WeakReference<OverseerPage> _pageRef;

        public OverseerScriptMessageHandler(OverseerPage page)
        {
            _pageRef = new WeakReference<OverseerPage>(page);
        }

        public void DidReceiveScriptMessage(WKUserContentController controller,
                                             WKScriptMessage message)
        {
            string json = message.Body?.ToString();
            if (!string.IsNullOrEmpty(json)
                && _pageRef.TryGetTarget(out var page))
            {
                page.HandleWebMessageFromBridge(json);
            }
        }
    }
}
