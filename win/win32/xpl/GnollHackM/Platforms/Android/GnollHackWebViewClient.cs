using Android.Net.Http;
using Android.Webkit;
using GnollHackX;
using Microsoft.Maui.Handlers;
using Microsoft.Maui.Platform;

namespace GnollHackM
{
    /// <summary>
    /// Custom WebViewClient that accepts self-signed SSL certificates
    /// for private/local IP addresses. Inherits from MauiWebViewClient
    /// to preserve MAUI's Navigating/Navigated event routing.
    /// Used only for the OverseerPage when connecting to a local
    /// development server.
    /// </summary>
    public class GnollHackWebViewClient : MauiWebViewClient
    {
        public GnollHackWebViewClient(WebViewHandler handler)
            : base(handler)
        {
        }

        public override void OnReceivedSslError(
            Android.Webkit.WebView view, SslErrorHandler handler, SslError error)
        {
            string url = error?.Url;
            if (url != null && UIUtils.IsLocalUrl(url))
            {
                handler.Proceed();
                return;
            }

            base.OnReceivedSslError(view, handler, error);
        }
    }
}
