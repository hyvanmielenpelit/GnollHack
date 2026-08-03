using Foundation;
using Security;
using WebKit;
using GnollHackX;
using Microsoft.Maui.Handlers;
using Microsoft.Maui.Platform;

namespace GnollHackM
{
    /// <summary>
    /// Custom WKNavigationDelegate that accepts self-signed SSL certificates
    /// for private/local IP addresses. Inherits from MauiWebViewNavigationDelegate
    /// to preserve MAUI's Navigating/Navigated event routing.
    /// Used only for the OverseerPage when connecting to a local
    /// development server.
    /// </summary>
    public class GnollHackNavigationDelegate : MauiWebViewNavigationDelegate
    {
        public GnollHackNavigationDelegate(IWebViewHandler handler)
            : base(handler)
        {
        }

        [Export("webView:didReceiveAuthenticationChallenge:completionHandler:")]
        public void DidReceiveAuthenticationChallenge(
            WKWebView webView,
            NSUrlAuthenticationChallenge challenge,
            Action<NSUrlSessionAuthChallengeDisposition, NSUrlCredential> completionHandler)
        {
            if (challenge.ProtectionSpace.AuthenticationMethod
                == NSUrlProtectionSpace.AuthenticationMethodServerTrust)
            {
                string host = challenge.ProtectionSpace.Host;
                if (UIUtils.IsLocalUrl("https://" + host))
                {
                    using (var credential = NSUrlCredential.FromTrust(
                        challenge.ProtectionSpace.ServerSecTrust))
                    {
                        completionHandler(
                            NSUrlSessionAuthChallengeDisposition.UseCredential,
                            credential);
                    }
                    return;
                }
            }

            completionHandler(
                NSUrlSessionAuthChallengeDisposition.PerformDefaultHandling,
                null);
        }
    }
}