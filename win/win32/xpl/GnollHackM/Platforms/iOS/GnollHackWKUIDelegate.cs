using System;
using Foundation;
using PhotosUI;
using UIKit;
using WebKit;
using Microsoft.Maui.Handlers;
using Microsoft.Maui.Platform;

namespace GnollHackM
{
    /// <summary>
    /// Custom WKUIDelegate that overrides RunOpenPanel to present a
    /// PHPickerViewController (Photo Library only, no camera) when the
    /// WKWebView's &lt;input type="file"&gt; is tapped. This avoids requiring
    /// NSCameraUsageDescription in Info.plist and also works around
    /// an iOS NSItemProvider crash (_NSIPCloneURLToTemporaryFolder)
    /// that can occur when presenting pickers from deeply nested modal
    /// pages. Inherits from MauiWebViewUIDelegate to preserve MAUI's
    /// JS alert/confirm/prompt handling.
    /// </summary>
    public class GnollHackWKUIDelegate : MauiWebViewUIDelegate
    {
        public GnollHackWKUIDelegate(IWebViewHandler handler)
            : base(handler)
        {
        }

        [Export("webView:runOpenPanelWithParameters:initiatedByFrame:completionHandler:")]
        public override void RunOpenPanel(
            WKWebView webView,
            WKOpenPanelParameters parameters,
            WKFrameInfo frame,
            Action<NSUrl[]> completionHandler)
        {
            bool allowsMultiple = false;
            if (OperatingSystem.IsIOSVersionAtLeast(18, 4))
            {
#pragma warning disable CA1416
                allowsMultiple = parameters.AllowsMultipleSelection;
#pragma warning restore CA1416
            }

            /* Determine whether the accept list includes images.
             * WKOpenPanelParameters does not expose the accept attribute
             * directly, so we always offer the photo library for image
             * types (which is the Overseer chat's use case). */

            var config = new PHPickerConfiguration();
            config.SelectionLimit = allowsMultiple ? 0 : 1; /* 0 = unlimited */
            config.Filter = PHPickerFilter.ImagesFilter;

            var picker = new PHPickerViewController(config);
            var pickerDelegate = new OpenPanelPickerDelegate(completionHandler);
            picker.Delegate = pickerDelegate;

            /* Find the topmost presented view controller to present from,
             * which avoids issues with deep modal stacks. */
            var rootVc = UIApplication.SharedApplication?.GetKeyWindow()?.RootViewController;
            if (rootVc == null)
            {
                completionHandler(null);
                return;
            }

            var topVc = GetTopViewController(rootVc);
            topVc.PresentViewController(picker, true, null);
        }

        private static UIViewController GetTopViewController(UIViewController vc)
        {
            if (vc is UINavigationController navController && navController.VisibleViewController != null)
                return GetTopViewController(navController.VisibleViewController);

            if (vc is UITabBarController tabController && tabController.SelectedViewController != null)
                return GetTopViewController(tabController.SelectedViewController);

            if (vc.PresentedViewController != null)
                return GetTopViewController(vc.PresentedViewController);

            return vc;
        }

        /// <summary>
        /// Delegate that handles the PHPickerViewController result,
        /// loads each selected image to a temp file, and passes the
        /// file URLs back to WKWebView via the completion handler.
        /// Stored as a strong reference on the picker to prevent
        /// premature garbage collection.
        /// </summary>
        private class OpenPanelPickerDelegate : PHPickerViewControllerDelegate
        {
            private readonly Action<NSUrl[]> _completionHandler;

            public OpenPanelPickerDelegate(Action<NSUrl[]> completionHandler)
            {
                _completionHandler = completionHandler;
            }

            public override void DidFinishPicking(
                PHPickerViewController picker,
                PHPickerResult[] results)
            {
                picker.DismissViewController(true, null);

                if (results == null || results.Length == 0)
                {
                    _completionHandler(null);
                    return;
                }

                /* Load each image to a temp file and collect URLs.
                 * We write to NSTemporaryDirectory ourselves to avoid
                 * the _NSIPCloneURLToTemporaryFolder assertion crash. */
                var remaining = results.Length;
                var urls = new NSUrl[results.Length];
                var failed = false;

                for (int i = 0; i < results.Length; i++)
                {
                    int index = i;
                    var provider = results[i].ItemProvider;

                    if (provider.HasItemConformingTo(UniformTypeIdentifiers.UTTypes.Image.Identifier))
                    {
                        provider.LoadDataRepresentation(
                            UniformTypeIdentifiers.UTTypes.Image,
                            (data, error) =>
                            {
                                if (data != null && error == null)
                                {
                                    try
                                    {
                                        string ext = GetExtension(provider);
                                        string tempPath = System.IO.Path.Combine(
                                            System.IO.Path.GetTempPath(),
                                            $"overseer_pick_{Guid.NewGuid():N}{ext}");
                                        var tempUrl = NSUrl.FromFilename(tempPath);
                                        data.Save(tempUrl, true);
                                        urls[index] = tempUrl;
                                    }
                                    catch
                                    {
                                        failed = true;
                                    }
                                }
                                else
                                {
                                    failed = true;
                                }

                                if (System.Threading.Interlocked.Decrement(ref remaining) == 0)
                                {
                                    InvokeOnMainThread(() =>
                                    {
                                        if (failed && urls.All(u => u == null))
                                            _completionHandler(null);
                                        else
                                            _completionHandler(
                                                urls.Where(u => u != null).ToArray());
                                    });
                                }
                            });
                    }
                    else
                    {
                        if (System.Threading.Interlocked.Decrement(ref remaining) == 0)
                        {
                            InvokeOnMainThread(() => _completionHandler(null));
                        }
                    }
                }
            }

            private static string GetExtension(NSItemProvider provider)
            {
                /* Try to determine a reasonable file extension from the
                 * registered type identifiers. */
                var identifiers = provider.RegisteredTypeIdentifiers;
                foreach (var uti in identifiers)
                {
                    if (uti.Contains("jpeg") || uti.Contains("jpg"))
                        return ".jpg";
                    if (uti.Contains("png"))
                        return ".png";
                    if (uti.Contains("webp"))
                        return ".webp";
                    if (uti.Contains("heic") || uti.Contains("heif"))
                        return ".heic";
                }
                return ".jpg"; /* safe fallback */
            }
        }
    }
}
