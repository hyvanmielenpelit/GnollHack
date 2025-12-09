using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GnollHackX;

#if GNH_MAUI
namespace GnollHackM
#else
using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace GnollHackX.Controls
#endif
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
	public partial class RowImageButton : ContentView
	{
        public static readonly BindableProperty LblTextProperty = BindableProperty.Create(nameof(LblText), typeof(string), typeof(RowImageButton), string.Empty);
        public static readonly BindableProperty LblTextColorProperty = BindableProperty.Create(nameof(LblTextColor), typeof(Color), typeof(RowImageButton), GHColors.Black, propertyChanged: OnTextColorChanged);
        public static readonly BindableProperty LblFontSizeProperty = BindableProperty.Create(nameof(LblFontSize), typeof(double), typeof(RowImageButton), 14.0);
        public static readonly BindableProperty LblFontFamilyProperty = BindableProperty.Create(nameof(LblFontFamily), typeof(string), typeof(RowImageButton), "Immortal");

        public static readonly BindableProperty SubLblTextProperty = BindableProperty.Create(nameof(SubLblText), typeof(string), typeof(RowImageButton), string.Empty);
        public static readonly BindableProperty SubLblTextColorProperty = BindableProperty.Create(nameof(SubLblTextColor), typeof(Color), typeof(RowImageButton), GHColors.Black, propertyChanged: OnSubTextColorChanged);
        public static readonly BindableProperty SubLblFontSizeProperty = BindableProperty.Create(nameof(SubLblFontSize), typeof(double), typeof(RowImageButton), 13.0);
        public static readonly BindableProperty SubLblFontFamilyProperty = BindableProperty.Create(nameof(SubLblFontFamily), typeof(string), typeof(RowImageButton), "Immortal");
        public static readonly BindableProperty IsSubLblVisibleProperty = BindableProperty.Create(nameof(IsSubLblVisible), typeof(bool), typeof(RowImageButton), false);

        public static readonly BindableProperty SubLbl2TextProperty = BindableProperty.Create(nameof(SubLbl2Text), typeof(string), typeof(RowImageButton), string.Empty);
        public static readonly BindableProperty SubLbl3TextProperty = BindableProperty.Create(nameof(SubLbl3Text), typeof(string), typeof(RowImageButton), string.Empty);
        public static readonly BindableProperty SubLbl4TextProperty = BindableProperty.Create(nameof(SubLbl4Text), typeof(string), typeof(RowImageButton), string.Empty);
        public static readonly BindableProperty SubLbl2TextColorProperty = BindableProperty.Create(nameof(SubLbl2TextColor), typeof(Color), typeof(RowImageButton), GHColors.Black, propertyChanged: OnSub2TextColorChanged);
        public static readonly BindableProperty SubLbl3TextColorProperty = BindableProperty.Create(nameof(SubLbl3TextColor), typeof(Color), typeof(RowImageButton), GHColors.Black, propertyChanged: OnSub3TextColorChanged);
        public static readonly BindableProperty SubLbl4TextColorProperty = BindableProperty.Create(nameof(SubLbl4TextColor), typeof(Color), typeof(RowImageButton), GHColors.Black, propertyChanged: OnSub4TextColorChanged);
        public static readonly BindableProperty SubLbl2FontSizeProperty = BindableProperty.Create(nameof(SubLbl2FontSize), typeof(double), typeof(RowImageButton), 13.0);
        public static readonly BindableProperty SubLbl3FontSizeProperty = BindableProperty.Create(nameof(SubLbl3FontSize), typeof(double), typeof(RowImageButton), 13.0);
        public static readonly BindableProperty SubLbl4FontSizeProperty = BindableProperty.Create(nameof(SubLbl4FontSize), typeof(double), typeof(RowImageButton), 13.0);
        public static readonly BindableProperty SubLbl2FontFamilyProperty = BindableProperty.Create(nameof(SubLbl2FontFamily), typeof(string), typeof(RowImageButton), "Immortal");
        public static readonly BindableProperty SubLbl3FontFamilyProperty = BindableProperty.Create(nameof(SubLbl3FontFamily), typeof(string), typeof(RowImageButton), "Immortal");
        public static readonly BindableProperty SubLbl4FontFamilyProperty = BindableProperty.Create(nameof(SubLbl4FontFamily), typeof(string), typeof(RowImageButton), "Immortal");
        public static readonly BindableProperty IsSubLbl2VisibleProperty = BindableProperty.Create(nameof(IsSubLbl2Visible), typeof(bool), typeof(RowImageButton), false);
        public static readonly BindableProperty IsSubLbl3VisibleProperty = BindableProperty.Create(nameof(IsSubLbl3Visible), typeof(bool), typeof(RowImageButton), false);
        public static readonly BindableProperty IsSubLbl4VisibleProperty = BindableProperty.Create(nameof(IsSubLbl4Visible), typeof(bool), typeof(RowImageButton), false);

        public static readonly BindableProperty ImgGlyphImageSourceProperty = BindableProperty.Create(nameof(ImgGlyphImageSource), typeof(GlyphImageSource), typeof(RowImageButton), null);
        public static readonly BindableProperty ImgSourcePathProperty = BindableProperty.Create(nameof(ImgSourcePath), typeof(string), typeof(RowImageButton), string.Empty);
        public static readonly BindableProperty ImgHighFilterQualityProperty = BindableProperty.Create(nameof(ImgHighFilterQuality), typeof(bool), typeof(LabeledImageButton), false);
        public static readonly BindableProperty GridWidthProperty = BindableProperty.Create(nameof(GridWidth), typeof(double), typeof(RowImageButton), 320.0);
        public static readonly BindableProperty GridHeightProperty = BindableProperty.Create(nameof(GridHeight), typeof(double), typeof(RowImageButton), -1.0);
        public static readonly BindableProperty GridMarginProperty = BindableProperty.Create(nameof(GridMargin), typeof(Thickness), typeof(RowImageButton), new Thickness());
        public static readonly BindableProperty ImgWidthProperty = BindableProperty.Create(nameof(ImgWidth), typeof(double), typeof(RowImageButton), 80.0);
        public static readonly BindableProperty ImgHeightProperty = BindableProperty.Create(nameof(ImgHeight), typeof(double), typeof(RowImageButton), 80.0);
        public static readonly BindableProperty BtnCommandProperty = BindableProperty.Create(nameof(BtnCommand), typeof(int), typeof(RowImageButton), 0);

        public event EventHandler<EventArgs> BtnClicked;
        public int BtnCommand
        {
            get => (int)GetValue(RowImageButton.BtnCommandProperty);
            set => SetValue(RowImageButton.BtnCommandProperty, value);
        }

        public string LblText
        {
            get => (string)GetValue(RowImageButton.LblTextProperty);
            set => SetValue(RowImageButton.LblTextProperty, value);
        }
        public Color LblTextColor
        {
            get => (Color)GetValue(RowImageButton.LblTextColorProperty);
            set
            {
                SetValue(LblTextColorProperty, value);
                ViewLabel.TextColor = _isHoveringEnabled && (!_isHovering || !IsEnabled) ? UIUtils.NonHoveringColorAdjustment(value, GHApp.DarkMode) : UIUtils.HoveringColorAdjustment(value, GHApp.DarkMode);
            }
        }
        public double LblFontSize
        {
            get => (double)GetValue(RowImageButton.LblFontSizeProperty);
            set => SetValue(RowImageButton.LblFontSizeProperty, value);
        }
        public string LblFontFamily
        {
            get => (string)GetValue(RowImageButton.LblFontFamilyProperty);
            set => SetValue(RowImageButton.LblFontFamilyProperty, value);
        }

        public string SubLblText
        {
            get => (string)GetValue(RowImageButton.SubLblTextProperty);
            set => SetValue(RowImageButton.SubLblTextProperty, value);
        }

        public string SubLbl2Text
        {
            get => (string)GetValue(RowImageButton.SubLbl2TextProperty);
            set => SetValue(RowImageButton.SubLbl2TextProperty, value);
        }

        public string SubLbl3Text
        {
            get => (string)GetValue(RowImageButton.SubLbl3TextProperty);
            set => SetValue(RowImageButton.SubLbl3TextProperty, value);
        }

        public string SubLbl4Text
        {
            get => (string)GetValue(RowImageButton.SubLbl4TextProperty);
            set => SetValue(RowImageButton.SubLbl4TextProperty, value);
        }

        public Color SubLblTextColor
        {
            get => (Color)GetValue(RowImageButton.SubLblTextColorProperty);
            set
            {
                SetValue(SubLblTextColorProperty, value);
                SubViewLabel.TextColor = _isHoveringEnabled && (!_isHovering || !IsEnabled) ? UIUtils.NonHoveringColorAdjustment(value, GHApp.DarkMode) : UIUtils.HoveringColorAdjustment(value, GHApp.DarkMode);
            }
        }
        public Color SubLbl2TextColor
        {
            get => (Color)GetValue(RowImageButton.SubLbl2TextColorProperty);
            set
            {
                SetValue(SubLbl2TextColorProperty, value);
                SubViewLabel2.TextColor = _isHoveringEnabled && (!_isHovering || !IsEnabled) ? UIUtils.NonHoveringColorAdjustment(value, GHApp.DarkMode) : UIUtils.HoveringColorAdjustment(value, GHApp.DarkMode);
            }
        }
        public Color SubLbl3TextColor
        {
            get => (Color)GetValue(RowImageButton.SubLbl3TextColorProperty);
            set
            {
                SetValue(SubLbl3TextColorProperty, value);
                SubViewLabel3.TextColor = _isHoveringEnabled && (!_isHovering || !IsEnabled) ? UIUtils.NonHoveringColorAdjustment(value, GHApp.DarkMode) : UIUtils.HoveringColorAdjustment(value, GHApp.DarkMode);
            }
        }
        public Color SubLbl4TextColor
        {
            get => (Color)GetValue(RowImageButton.SubLbl4TextColorProperty);
            set
            {
                SetValue(SubLbl4TextColorProperty, value);
                SubViewLabel4.TextColor = _isHoveringEnabled && (!_isHovering || !IsEnabled) ? UIUtils.NonHoveringColorAdjustment(value, GHApp.DarkMode) : UIUtils.HoveringColorAdjustment(value, GHApp.DarkMode);
            }
        }
        public double SubLblFontSize
        {
            get => (double)GetValue(RowImageButton.SubLblFontSizeProperty);
            set => SetValue(RowImageButton.SubLblFontSizeProperty, value);
        }
        public double SubLbl2FontSize
        {
            get => (double)GetValue(RowImageButton.SubLbl2FontSizeProperty);
            set => SetValue(RowImageButton.SubLbl2FontSizeProperty, value);
        }
        public double SubLbl3FontSize
        {
            get => (double)GetValue(RowImageButton.SubLbl3FontSizeProperty);
            set => SetValue(RowImageButton.SubLbl3FontSizeProperty, value);
        }
        public double SubLbl4FontSize
        {
            get => (double)GetValue(RowImageButton.SubLbl4FontSizeProperty);
            set => SetValue(RowImageButton.SubLbl4FontSizeProperty, value);
        }
        public string SubLblFontFamily
        {
            get => (string)GetValue(RowImageButton.SubLblFontFamilyProperty);
            set => SetValue(RowImageButton.SubLblFontFamilyProperty, value);
        }
        public string SubLbl2FontFamily
        {
            get => (string)GetValue(RowImageButton.SubLbl2FontFamilyProperty);
            set => SetValue(RowImageButton.SubLbl2FontFamilyProperty, value);
        }
        public string SubLbl3FontFamily
        {
            get => (string)GetValue(RowImageButton.SubLbl3FontFamilyProperty);
            set => SetValue(RowImageButton.SubLbl3FontFamilyProperty, value);
        }
        public string SubLbl4FontFamily
        {
            get => (string)GetValue(RowImageButton.SubLbl4FontFamilyProperty);
            set => SetValue(RowImageButton.SubLbl4FontFamilyProperty, value);
        }
        public bool IsSubLblVisible
        {
            get => (bool)GetValue(RowImageButton.IsSubLblVisibleProperty);
            set => SetValue(RowImageButton.IsSubLblVisibleProperty, value);
        }
        public bool IsSubLbl2Visible
        {
            get => (bool)GetValue(RowImageButton.IsSubLbl2VisibleProperty);
            set => SetValue(RowImageButton.IsSubLbl2VisibleProperty, value);
        }
        public bool IsSubLbl3Visible
        {
            get => (bool)GetValue(RowImageButton.IsSubLbl3VisibleProperty);
            set => SetValue(RowImageButton.IsSubLbl3VisibleProperty, value);
        }
        public bool IsSubLbl4Visible
        {
            get => (bool)GetValue(RowImageButton.IsSubLbl4VisibleProperty);
            set => SetValue(RowImageButton.IsSubLbl4VisibleProperty, value);
        }

        public GlyphImageSource ImgGlyphImageSource
        {
            get => (GlyphImageSource)GetValue(RowImageButton.ImgGlyphImageSourceProperty);
            set => SetValue(RowImageButton.ImgGlyphImageSourceProperty, value);
        }
        public string ImgSourcePath
        {
            get => (string)GetValue(RowImageButton.ImgSourcePathProperty);
            set => SetValue(RowImageButton.ImgSourcePathProperty, value);
        }
        public double ImgWidth
        {
            get => (double)GetValue(RowImageButton.ImgWidthProperty);
            set => SetValue(RowImageButton.ImgWidthProperty, value);
        }
        public double ImgHeight
        {
            get => (double)GetValue(RowImageButton.ImgHeightProperty);
            set => SetValue(RowImageButton.ImgHeightProperty, value);
        }

        public double GridWidth
        {
            get => (double)GetValue(RowImageButton.GridWidthProperty);
            set => SetValue(RowImageButton.GridWidthProperty, value);
        }
        public double GridHeight
        {
            get => (double)GetValue(RowImageButton.GridHeightProperty);
            set => SetValue(RowImageButton.GridHeightProperty, value);
        }
        public Thickness GridMargin
        {
            get => (Thickness)GetValue(RowImageButton.GridMarginProperty);
            set => SetValue(RowImageButton.GridMarginProperty, value);
        }
        public bool ImgHighFilterQuality
        {
            get => (bool)GetValue(LabeledImageButton.ImgHighFilterQualityProperty);
            set => SetValue(LabeledImageButton.ImgHighFilterQualityProperty, value);
        }

        public RowImageButton()
        {
            InitializeComponent();
#if WINDOWS
            ViewButton.HandlerChanged += (s, e) =>
            {
                if (ViewButton.Handler?.PlatformView is Microsoft.UI.Xaml.Controls.Button)
                {
                    var platformView = ViewButton.Handler?.PlatformView as Microsoft.UI.Xaml.Controls.Button;
                    if (platformView != null)
                    {
                        platformView.PointerEntered += PlatformView_PointerEntered;
                        platformView.PointerExited += PlatformView_PointerExited;
                        platformView.PointerCanceled += PlatformView_PointerExited;
                        platformView.Background = new Microsoft.UI.Xaml.Media.SolidColorBrush();
                    }
                }
            };
            ViewButton.HandlerChanging += (s, e) =>
            {
                if (e.OldHandler != null && e.NewHandler == null)
                {
                    if (e.OldHandler?.PlatformView is Microsoft.UI.Xaml.Controls.Button)
                    {
                        var platformView = e.OldHandler?.PlatformView as Microsoft.UI.Xaml.Controls.Button;
                        if (platformView != null)
                        {
                            platformView.PointerEntered -= PlatformView_PointerEntered;
                            platformView.PointerExited -= PlatformView_PointerExited;
                            platformView.PointerCanceled -= PlatformView_PointerExited;
                        }
                    }
                }
            };
#endif
        }

#if WINDOWS
        private bool _isHoveringEnabled = true;
        private bool _isHovering = false;
        private void PlatformView_PointerEntered(object sender, Microsoft.UI.Xaml.Input.PointerRoutedEventArgs e)
        {
            e.Handled = true;
            MainThread.BeginInvokeOnMainThread(() =>
            {
                ViewImage.IsHighlighted = true;
                _isHovering = true;
                LblTextColor = LblTextColor;
                SubLblTextColor = SubLblTextColor;
                SubLbl2TextColor = SubLbl2TextColor;
                SubLbl3TextColor = SubLbl3TextColor;
                SubLbl4TextColor = SubLbl4TextColor;
            });
        }

        private void PlatformView_PointerExited(object sender, Microsoft.UI.Xaml.Input.PointerRoutedEventArgs e)
        {
            e.Handled = true;
            MainThread.BeginInvokeOnMainThread(() =>
            {
                ViewImage.IsHighlighted = false;
                _isHovering = false;
                LblTextColor = LblTextColor;
                SubLblTextColor = SubLblTextColor;
                SubLbl2TextColor = SubLbl2TextColor;
                SubLbl3TextColor = SubLbl3TextColor;
                SubLbl4TextColor = SubLbl4TextColor;
            });
        }
#else
        private bool _isHoveringEnabled = false;
        private bool _isHovering = false;
#endif

        private void ViewButton_Clicked(object sender, EventArgs e)
        {
            GHApp.AddSentryBreadcrumb(LblText, "RowImageButton Clicked");
            BtnClicked?.Invoke(this, e);
        }

        private static void OnTextColorChanged(BindableObject bindable, object oldValue, object newValue)
        {
            RowImageButton cib = bindable as RowImageButton;
            if (cib != null && newValue is Color)
            {
                Color newColor = (Color)newValue;
                cib.ViewLabel.TextColor = cib._isHoveringEnabled && (!cib._isHovering || !cib.IsEnabled) ? UIUtils.NonHoveringColorAdjustment(newColor, GHApp.DarkMode) : UIUtils.HoveringColorAdjustment(newColor, GHApp.DarkMode);
            }
        }

        private static void OnSubTextColorChanged(BindableObject bindable, object oldValue, object newValue)
        {
            RowImageButton cib = bindable as RowImageButton;
            if (cib != null && newValue is Color)
            {
                Color newColor = (Color)newValue;
                cib.SubViewLabel.TextColor = cib._isHoveringEnabled && (!cib._isHovering || !cib.IsEnabled) ? UIUtils.NonHoveringColorAdjustment(newColor, GHApp.DarkMode) : UIUtils.HoveringColorAdjustment(newColor, GHApp.DarkMode);
            }
        }
        private static void OnSub2TextColorChanged(BindableObject bindable, object oldValue, object newValue)
        {
            RowImageButton cib = bindable as RowImageButton;
            if (cib != null && newValue is Color)
            {
                Color newColor = (Color)newValue;
                cib.SubViewLabel2.TextColor = cib._isHoveringEnabled && (!cib._isHovering || !cib.IsEnabled) ? UIUtils.NonHoveringColorAdjustment(newColor, GHApp.DarkMode) : UIUtils.HoveringColorAdjustment(newColor, GHApp.DarkMode);
            }
        }
        private static void OnSub3TextColorChanged(BindableObject bindable, object oldValue, object newValue)
        {
            RowImageButton cib = bindable as RowImageButton;
            if (cib != null && newValue is Color)
            {
                Color newColor = (Color)newValue;
                cib.SubViewLabel3.TextColor = cib._isHoveringEnabled && (!cib._isHovering || !cib.IsEnabled) ? UIUtils.NonHoveringColorAdjustment(newColor, GHApp.DarkMode) : UIUtils.HoveringColorAdjustment(newColor, GHApp.DarkMode);
            }
        }
        private static void OnSub4TextColorChanged(BindableObject bindable, object oldValue, object newValue)
        {
            RowImageButton cib = bindable as RowImageButton;
            if (cib != null && newValue is Color)
            {
                Color newColor = (Color)newValue;
                cib.SubViewLabel4.TextColor = cib._isHoveringEnabled && (!cib._isHovering || !cib.IsEnabled) ? UIUtils.NonHoveringColorAdjustment(newColor, GHApp.DarkMode) : UIUtils.HoveringColorAdjustment(newColor, GHApp.DarkMode);
            }
        }
    }
}