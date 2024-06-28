using SkiaSharp;
#if GNH_MAUI
using Microsoft.Maui.Controls;
using SkiaSharp.Views.Maui;
using SkiaSharp.Views.Maui.Controls;
using GnollHackX;
#else
using SkiaSharp.Views.Forms;
using Xamarin.Forms;
using Xamarin.Forms.Xaml;
#endif
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

#if GNH_MAUI
namespace GnollHackM
#else
namespace GnollHackX.Controls
#endif
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class CustomImageButton : ContentView
    {
        public CustomImageButton()
        {
            InitializeComponent();
            customCanvasView.InvalidateSurface();
            TextColor = TextColor;
            customButton.Pressed += (object sender, EventArgs args) =>
            {
                _isPressed = true;
                if(UseVaryingTextColors)
                    customButton.TextColor = !IsEnabled ? DisabledTextColor : _isPressed ? SelectedTextColor : NormalTextColor;
                customCanvasView.InvalidateSurface();
            };
            customButton.Released += (object sender, EventArgs args) =>
            {
                _isPressed = false;
                if (UseVaryingTextColors)
                    customButton.TextColor = !IsEnabled ? DisabledTextColor : _isPressed ? SelectedTextColor : NormalTextColor;
                customCanvasView.InvalidateSurface();
            };
            customButton.Unfocused += (object sender, FocusEventArgs args) =>
            {
                _isPressed = false;
                if (UseVaryingTextColors)
                    customButton.TextColor = !IsEnabled ? DisabledTextColor : _isPressed ? SelectedTextColor : NormalTextColor;
                customCanvasView.InvalidateSurface();
            };
            customButton.SizeChanged += (object sender, EventArgs args) =>
            {
                customCanvasView.InvalidateSurface();
            };
#if WINDOWS
            customCanvasView.HandlerChanged += (object sender, EventArgs args) =>
            {
                if(customCanvasView.Handler?.PlatformView is SkiaSharp.Views.Windows.SKXamlCanvas)
                {
                    var canvas = customCanvasView.Handler?.PlatformView as SkiaSharp.Views.Windows.SKXamlCanvas;
                    canvas.Background = new Microsoft.UI.Xaml.Media.SolidColorBrush();
                    canvas.PointerEntered += Canvas_PointerEntered;
                    canvas.PointerExited += Canvas_PointerExited;
                }
            };
            customButton.HandlerChanged += (object sender, EventArgs args) =>
            {
                if (customButton.Handler?.PlatformView is Microsoft.UI.Xaml.Controls.Button)
                {
                    var button = customButton.Handler?.PlatformView as Microsoft.UI.Xaml.Controls.Button;
                    button.Background = new Microsoft.UI.Xaml.Media.SolidColorBrush();
                    button.PointerEntered += Canvas_PointerEntered;
                    button.PointerExited += Canvas_PointerExited;
                }
            };
#endif
        }


#if WINDOWS
        private bool _isHoveringEnabled = true;
        private bool _isHovering = false;
        private void Canvas_PointerEntered(object sender, Microsoft.UI.Xaml.Input.PointerRoutedEventArgs e)
        {
            _isHovering = true;
            customCanvasView.InvalidateSurface();
            TextColor = TextColor;
        }
        private void Canvas_PointerExited(object sender, Microsoft.UI.Xaml.Input.PointerRoutedEventArgs e)
        {
            _isHovering = false;
            customCanvasView.InvalidateSurface();
            TextColor = TextColor;
        }
#else
        private bool _isHoveringEnabled = false;
        private bool _isHovering = false;
#endif
        private bool _isPressed = false;

        public event EventHandler Clicked;

        public static readonly BindableProperty ButtonRelativeWidthProperty = BindableProperty.Create(nameof(ButtonRelativeWidthProperty), typeof(double), typeof(CustomImageButton), 10.0 / 12.0);
        public static readonly BindableProperty NormalTextColorProperty = BindableProperty.Create(nameof(NormalTextColorProperty), typeof(Color), typeof(CustomImageButton), GHColors.White);
        public static readonly BindableProperty SelectedTextColorProperty = BindableProperty.Create(nameof(SelectedTextColorProperty), typeof(Color), typeof(CustomImageButton), GHColors.White);
        public static readonly BindableProperty DisabledTextColorProperty = BindableProperty.Create(nameof(DisabledTextColorProperty), typeof(Color), typeof(CustomImageButton), GHColors.Gray);
        public static readonly BindableProperty TextColorProperty = BindableProperty.Create(nameof(TextColorProperty), typeof(Color), typeof(CustomImageButton), GHColors.White);
        public static readonly BindableProperty UseVaryingTextColorsProperty = BindableProperty.Create(nameof(UseVaryingTextColorsProperty), typeof(bool), typeof(CustomImageButton), false);
        public static readonly BindableProperty UseVaryingBackgroundImagesProperty = BindableProperty.Create(nameof(UseVaryingBackgroundImages), typeof(bool), typeof(CustomImageButton), true);

        public double ButtonRelativeWidth
        {
            get => (double)GetValue(CustomImageButton.ButtonRelativeWidthProperty);
            set => SetValue(CustomImageButton.ButtonRelativeWidthProperty, value);
        }
        public Color TextColor
        {
            get { return (Color)GetValue(TextColorProperty); }
            set 
            { 
                SetValue(TextColorProperty, value);
                customButton.TextColor = _isHoveringEnabled && (!_isHovering || !IsEnabled) ? NonHoveringColorAdjustment(value) : value;
            }
        }
        public Color NormalTextColor
        {
            get { return (Color)GetValue(NormalTextColorProperty); }
            set { SetValue(NormalTextColorProperty, value); }
        }
        public Color SelectedTextColor
        {
            get { return (Color)GetValue(SelectedTextColorProperty); }
            set { SetValue(SelectedTextColorProperty, value); }
        }
        public Color DisabledTextColor
        {
            get { return (Color)GetValue(DisabledTextColorProperty); }
            set { SetValue(DisabledTextColorProperty, value); }
        }
        public string Text
        {
            get { return (string)customButton.GetValue(Button.TextProperty); }
            set { customButton.SetValue(Button.TextProperty, value); }
        }
        public string FontFamily
        {
            get => (string)customButton.GetValue(Button.FontFamilyProperty);
            set { customButton.SetValue(Button.FontFamilyProperty, value); }
        }
        public double FontSize
        {
            get => (double)customButton.GetValue(Button.FontSizeProperty);
            set { customButton.SetValue(Button.FontSizeProperty, value); }
        }
        public new double WidthRequest
        {
            get { return (double)GetValue(CustomImageButton.WidthRequestProperty); }
            set { SetValue(CustomImageButton.WidthRequestProperty, value); customGrid.WidthRequest = value; customCanvasView.WidthRequest = value; customButton.WidthRequest = value * ButtonRelativeWidth; customCanvasView.InvalidateSurface(); }
        }
        public new double HeightRequest
        {
            get { return (double)GetValue(CustomImageButton.HeightRequestProperty); }
            set { SetValue(CustomImageButton.HeightRequestProperty, value); customGrid.HeightRequest = value; customCanvasView.HeightRequest = value; customButton.HeightRequest = value; customCanvasView.InvalidateSurface(); }
        }

        public new bool IsEnabled
        {
            get { return (bool)GetValue(CustomImageButton.IsEnabledProperty); }
            set { SetValue(CustomImageButton.IsEnabledProperty, value); customGrid.IsEnabled = value; if (UseVaryingTextColors) { TextColor = !value ? DisabledTextColor : _isPressed ? SelectedTextColor : NormalTextColor; } customCanvasView.InvalidateSurface(); }
        }
        public bool UseVaryingTextColors
        {
            get { return (bool)GetValue(CustomImageButton.UseVaryingTextColorsProperty); }
            set { SetValue(CustomImageButton.UseVaryingTextColorsProperty, value); TextColor = value ? (!IsEnabled ? DisabledTextColor : _isPressed ? SelectedTextColor : NormalTextColor) : NormalTextColor; }
        }
        public bool UseVaryingBackgroundImages
        {
            get { return (bool)GetValue(CustomImageButton.UseVaryingBackgroundImagesProperty); }
            set { SetValue(CustomImageButton.UseVaryingBackgroundImagesProperty, value); customCanvasView.InvalidateSurface(); }
        }


        private void CustomButton_Clicked(object sender, EventArgs e)
        {
            Clicked?.Invoke(this, e);
        }

        private void CustomCanvasView_PaintSurface(object sender, SKPaintSurfaceEventArgs e)
        {
            SKImageInfo info = e.Info;
            SKSurface surface = e.Surface;
            SKCanvas canvas = surface.Canvas;
            float canvaswidth = customCanvasView.CanvasSize.Width;
            float canvasheight = customCanvasView.CanvasSize.Height;
#if WINDOWS
            SKImage targetBitmap = !UseVaryingBackgroundImages ? GHApp.ButtonNormalBitmap : _isPressed && IsEnabled ? GHApp.ButtonSelectedBitmap : _isHovering && IsEnabled ? GHApp.ButtonNormalBitmap : GHApp.ButtonDisabledBitmap;
#else
            SKImage targetBitmap = !UseVaryingBackgroundImages ? GHApp.ButtonNormalBitmap : !IsEnabled ? GHApp.ButtonDisabledBitmap : _isPressed ? GHApp.ButtonSelectedBitmap : GHApp.ButtonNormalBitmap;
#endif
            if (targetBitmap == null)
                return;
            canvas.Clear();
            SKRect sourcerect = new SKRect(0, 0, targetBitmap.Width, targetBitmap.Height);
            SKRect targetrect = new SKRect(0, 0, canvaswidth, canvasheight);
            canvas.DrawImage(targetBitmap, sourcerect, targetrect);
        }

        private Color NonHoveringColorAdjustment(Color rawColor)
        {
#if GNH_MAUI
            return new Color(rawColor.Red * GHConstants.NonHoveringColorMultiplier, rawColor.Green * GHConstants.NonHoveringColorMultiplier, rawColor.Blue * GHConstants.NonHoveringColorMultiplier, rawColor.Alpha);
#else
            return new Color(rawColor.R * GHConstants.NonHoveringColorMultiplier, rawColor.G * GHConstants.NonHoveringColorMultiplier, rawColor.B * GHConstants.NonHoveringColorMultiplier, rawColor.A);
#endif
        }

        //public void Redraw()
        //{
        //    customCanvasView.InvalidateSurface();
        //}

    }
}