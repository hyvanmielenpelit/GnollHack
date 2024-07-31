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
    public partial class SimpleImageButton : ContentView
    {
        public static readonly BindableProperty ImgSourcePathProperty = BindableProperty.Create(nameof(ImgSourcePath), typeof(string), typeof(SimpleImageButton), string.Empty);
        public static readonly BindableProperty GridWidthProperty = BindableProperty.Create(nameof(GridWidth), typeof(double), typeof(SimpleImageButton), 45.0);
        public static readonly BindableProperty GridHeightProperty = BindableProperty.Create(nameof(GridHeight), typeof(double), typeof(SimpleImageButton), 45.0);

        public SimpleImageButton()
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
        private void PlatformView_PointerEntered(object sender, Microsoft.UI.Xaml.Input.PointerRoutedEventArgs e)
        {
            e.Handled = true;
            MainThread.BeginInvokeOnMainThread(() =>
            {
                ViewImage.IsHighlighted = true;
            });
        }

        private void PlatformView_PointerExited(object sender, Microsoft.UI.Xaml.Input.PointerRoutedEventArgs e)
        {
            e.Handled = true;
            MainThread.BeginInvokeOnMainThread(() =>
            {
                ViewImage.IsHighlighted = false;
            });
        }
#endif

        public event EventHandler<EventArgs> BtnClicked;

        public string ImgSourcePath
        {
            get => (string)GetValue(SimpleImageButton.ImgSourcePathProperty);
            set => SetValue(SimpleImageButton.ImgSourcePathProperty, value);
        }
        public double GridWidth
        {
            get => (double)GetValue(SimpleImageButton.GridWidthProperty);
            set => SetValue(SimpleImageButton.GridWidthProperty, value);
        }
        public double GridHeight
        {
            get => (double)GetValue(SimpleImageButton.GridHeightProperty);
            set => SetValue(SimpleImageButton.GridHeightProperty, value);
        }

        public int LandscapeButtonsInRow { get; set; } = 0;
        public int PortraitButtonsInRow { get; set; } = 0;

        public void SetSideSize(double canvaswidth, double canvasheight, bool usingDesktopButtons, bool usingSimpleCmdLayout)
        {
            int bigRowNoOfButtons = LandscapeButtonsInRow > 0 ? LandscapeButtonsInRow : UIUtils.LandscapeButtonsInRow(usingDesktopButtons, usingSimpleCmdLayout);
            bool tooWide = 35.0 * bigRowNoOfButtons + (bigRowNoOfButtons - 1) * 6 > canvaswidth;
            int noOfButtons = canvaswidth > canvasheight && !tooWide ? bigRowNoOfButtons : PortraitButtonsInRow > 0 ? PortraitButtonsInRow : UIUtils.PortraitButtonsInRow(usingDesktopButtons, usingSimpleCmdLayout);
            double imgsidewidth = Math.Min(75.0, Math.Max(35.0, (canvaswidth - (noOfButtons - 1) * 6) / Math.Max(1, noOfButtons)));
            GridWidth = imgsidewidth;
            GridHeight = imgsidewidth;
        }

        private void ViewButton_Clicked(object sender, EventArgs e)
        {
            BtnClicked?.Invoke(this, e);
        }

    }
}