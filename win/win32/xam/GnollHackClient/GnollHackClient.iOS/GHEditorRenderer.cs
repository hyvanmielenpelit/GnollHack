using Foundation;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UIKit;
using Xamarin.Forms.Platform.iOS;
using GnollHackClient;
using Xamarin.Forms;
using GnollHackClient.iOS;
using System.ComponentModel;

[assembly: ExportRenderer(typeof(GHEditor), typeof(GHEditorRenderer))]
namespace GnollHackClient.iOS
{
    public class GHEditorRenderer : EditorRenderer
    {
        private UILabel _placeholder;

        protected override void OnElementChanged(ElementChangedEventArgs<Editor> e)
        {
            base.OnElementChanged(e);

            if (Control != null)
            {
                UpdatePlaceholder();
            }
        }

        protected override void OnElementPropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            base.OnElementPropertyChanged(sender, e);
            if (e.PropertyName == Editor.TextProperty.PropertyName)
            {
                UpdatePlaceholder();
                return;
            }
        }

        private void UpdatePlaceholder()
        {
            if (_placeholder == null)
            {
                var subview = TextView.Subviews?.OfType<UILabel>().FirstOrDefault();

                _placeholder = subview;
            }

            if (string.IsNullOrEmpty(TextView.Text))
            {
                TextView.AddSubview(_placeholder);

                UpdateUIConstraints();
            }
            else
            {
                _placeholder.RemoveFromSuperview();
            }
        }

        private void UpdateUIConstraints()
        {
            var edgeInsets = TextView.TextContainerInset;
            var lineFragmentPadding = TextView.TextContainer.LineFragmentPadding;

            var vConstraints = NSLayoutConstraint.FromVisualFormat(
                "V:|-" + edgeInsets.Top + $"-[{nameof(_placeholder)}]-" + edgeInsets.Bottom + "-|", 0, new NSDictionary(),
                NSDictionary.FromObjectsAndKeys(
                    new NSObject[] { _placeholder }, new NSObject[] { new NSString(nameof(_placeholder)) })
            );

            var hConstraints = NSLayoutConstraint.FromVisualFormat(
                "H:|-" + lineFragmentPadding + $"-[{nameof(_placeholder)}]-" + lineFragmentPadding + "-|",
                0, new NSDictionary(),
                NSDictionary.FromObjectsAndKeys(
                    new NSObject[] { _placeholder }, new NSObject[] { new NSString(nameof(_placeholder)) })
            );

            _placeholder.TranslatesAutoresizingMaskIntoConstraints = false;

            Control.AddConstraints(hConstraints);
            Control.AddConstraints(vConstraints);
        }
    }
}
