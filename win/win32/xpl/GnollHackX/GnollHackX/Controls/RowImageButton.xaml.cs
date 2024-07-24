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

        public static readonly BindableProperty SubLblTextProperty = BindableProperty.Create(nameof(SubLblText), typeof(string), typeof(RowImageButton), string.Empty);
        public static readonly BindableProperty SubLblTextColorProperty = BindableProperty.Create(nameof(SubLblTextColor), typeof(Color), typeof(RowImageButton), GHColors.Black, propertyChanged: OnSubTextColorChanged);
        public static readonly BindableProperty SubLblFontSizeProperty = BindableProperty.Create(nameof(SubLblFontSize), typeof(double), typeof(RowImageButton), 13.0);
        public static readonly BindableProperty IsSubLblVisibleProperty = BindableProperty.Create(nameof(IsSubLblVisible), typeof(bool), typeof(RowImageButton), false);

        public static readonly BindableProperty ImgSourcePathProperty = BindableProperty.Create(nameof(ImgSourcePath), typeof(string), typeof(RowImageButton), string.Empty);
        public static readonly BindableProperty GridWidthProperty = BindableProperty.Create(nameof(GridWidth), typeof(double), typeof(RowImageButton), 320.0);
        public static readonly BindableProperty GridHeightProperty = BindableProperty.Create(nameof(GridHeight), typeof(double), typeof(RowImageButton), 80.0);
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

        public string SubLblText
        {
            get => (string)GetValue(RowImageButton.SubLblTextProperty);
            set => SetValue(RowImageButton.SubLblTextProperty, value);
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
        public double SubLblFontSize
        {
            get => (double)GetValue(RowImageButton.SubLblFontSizeProperty);
            set => SetValue(RowImageButton.SubLblFontSizeProperty, value);
        }
        public bool IsSubLblVisible
        {
            get => (bool)GetValue(RowImageButton.IsSubLblVisibleProperty);
            set => SetValue(RowImageButton.IsSubLblVisibleProperty, value);
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

        public RowImageButton()
        {
            InitializeComponent();
            ViewButton.Clicked += ViewButton_Clicked;
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
            });
        }
#else
        private bool _isHoveringEnabled = false;
        private bool _isHovering = false;
#endif

        private void ViewButton_Clicked(object sender, EventArgs e)
        {
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
    }
}