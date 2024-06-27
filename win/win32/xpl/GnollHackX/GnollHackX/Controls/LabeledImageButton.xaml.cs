using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
#if GNH_MAUI
using GnollHackX;

namespace GnollHackM
#else
using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace GnollHackX.Controls
#endif
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class LabeledImageButton : ContentView
    {
        public static readonly BindableProperty LblTextProperty = BindableProperty.Create(nameof(LblText), typeof(string), typeof(LabeledImageButton), string.Empty);
        public static readonly BindableProperty LblFontSizeProperty = BindableProperty.Create(nameof(LblFontSize), typeof(double), typeof(LabeledImageButton), 9.0);
        public static readonly BindableProperty LblFontFamilyProperty = BindableProperty.Create(nameof(LblFontFamily), typeof(string), typeof(LabeledImageButton), "Lato-Regular");
        public static readonly BindableProperty LblFontColorProperty = BindableProperty.Create(nameof(LblFontColor), typeof(Color), typeof(LabeledImageButton), GHColors.White);
        public static readonly BindableProperty ImgSourcePathProperty = BindableProperty.Create(nameof(ImgSourcePath), typeof(string), typeof(LabeledImageButton), string.Empty);
        public static readonly BindableProperty GridWidthProperty = BindableProperty.Create(nameof(GridWidth), typeof(double), typeof(LabeledImageButton), 50.0);
        public static readonly BindableProperty GridHeightProperty = BindableProperty.Create(nameof(GridHeight), typeof(double), typeof(LabeledImageButton), 60.0);
        public static readonly BindableProperty GridMarginProperty = BindableProperty.Create(nameof(GridMargin), typeof(Thickness), typeof(LabeledImageButton), new Thickness());
        public static readonly BindableProperty ImgWidthProperty = BindableProperty.Create(nameof(ImgWidth), typeof(double), typeof(LabeledImageButton), 50.0);
        public static readonly BindableProperty ImgHeightProperty = BindableProperty.Create(nameof(ImgHeight), typeof(double), typeof(LabeledImageButton), 50.0);
        public static readonly BindableProperty BtnLetterProperty = BindableProperty.Create(nameof(BtnLetter), typeof(char), typeof(LabeledImageButton), (char)0);
        public static readonly BindableProperty BtnMetaProperty = BindableProperty.Create(nameof(BtnMeta), typeof(bool), typeof(LabeledImageButton), false);
        public static readonly BindableProperty BtnCtrlProperty = BindableProperty.Create(nameof(BtnCtrl), typeof(bool), typeof(LabeledImageButton), false);
        public static readonly BindableProperty BtnCommandProperty = BindableProperty.Create(nameof(BtnCommand), typeof(int), typeof(LabeledImageButton), 0);

        public event EventHandler<EventArgs> BtnClicked;
        public char BtnLetter
        {
            get => (char)GetValue(LabeledImageButton.BtnLetterProperty);
            set => SetValue(LabeledImageButton.BtnLetterProperty, value);
        }
        public bool BtnMeta
        {
            get => (bool)GetValue(LabeledImageButton.BtnMetaProperty);
            set => SetValue(LabeledImageButton.BtnMetaProperty, value);
        }
        public bool BtnCtrl
        {
            get => (bool)GetValue(LabeledImageButton.BtnCtrlProperty);
            set => SetValue(LabeledImageButton.BtnCtrlProperty, value);
        }
        public int BtnCommand
        {
            get => (int)GetValue(LabeledImageButton.BtnCommandProperty);
            set => SetValue(LabeledImageButton.BtnCommandProperty, value);
        }

        public string LblText
        {
            get => (string)GetValue(LabeledImageButton.LblTextProperty);
            set => SetValue(LabeledImageButton.LblTextProperty, value);
        }
        public double LblFontSize
        {
            get => (double)GetValue(LabeledImageButton.LblFontSizeProperty);
            set => SetValue(LabeledImageButton.LblFontSizeProperty, value);
        }
        public string LblFontFamily
        {
            get => (string)GetValue(LabeledImageButton.LblFontFamilyProperty);
            set => SetValue(LabeledImageButton.LblFontFamilyProperty, value);
        }
        public Color LblFontColor
        {
            get => (Color)GetValue(LabeledImageButton.LblFontColorProperty);
            set => SetValue(LabeledImageButton.LblFontColorProperty, value);
        }

        public string ImgSourcePath
        {
            get => (string)GetValue(LabeledImageButton.ImgSourcePathProperty);
            set => SetValue(LabeledImageButton.ImgSourcePathProperty, value); 
        }
        public double ImgWidth
        {
            get => (double)GetValue(LabeledImageButton.ImgWidthProperty);
            set => SetValue(LabeledImageButton.ImgWidthProperty, value);
        }
        public double ImgHeight
        {
            get => (double)GetValue(LabeledImageButton.ImgHeightProperty);
            set => SetValue(LabeledImageButton.ImgHeightProperty, value);
        }

        public double GridWidth
        {
            get => (double)GetValue(LabeledImageButton.GridWidthProperty);
            set => SetValue(LabeledImageButton.GridWidthProperty, value);
        }
        public double GridHeight
        {
            get => (double)GetValue(LabeledImageButton.GridHeightProperty);
            set => SetValue(LabeledImageButton.GridHeightProperty, value);
        }
        public Thickness GridMargin
        {
            get => (Thickness)GetValue(LabeledImageButton.GridMarginProperty);
            set => SetValue(LabeledImageButton.GridMarginProperty, value);
        }

        public int GHCommand
        {
            get
            {
                return ViewButton.GHCommand;
            }
        }

        public bool LargerFont { get; set; }
        public int LandscapeButtonsInRow { get; set; } = 14;
        public int PortraitButtonsInRow { get; set; } = 7;

        public void SetSideSize(double canvaswidth, double canvasheight)
        {
            double imgsidewidth = 0;
            if (canvaswidth > canvasheight)
                imgsidewidth = Math.Min(80.0, Math.Max(40.0, canvaswidth / Math.Max(1, LandscapeButtonsInRow)));
            else
                imgsidewidth = Math.Min(80.0, Math.Max(40.0, canvaswidth / Math.Max(1, PortraitButtonsInRow)));

            double imgsideheight = imgsidewidth;
            double fontsize = 8.5 * imgsidewidth / 50.0;
            double fontsize_larger = 9.0 * imgsidewidth / 50.0;
            double gridsidewidth = imgsidewidth;
            double gridsideheight = imgsideheight + fontsize + 2;
            double gridsideheight_larger = imgsideheight + fontsize_larger + 2;

            LblFontSize = LargerFont ? fontsize_larger : fontsize;
            GridWidth = gridsidewidth;
            GridHeight = LargerFont ? gridsideheight_larger : gridsideheight;
            ImgWidth = imgsidewidth;
            ImgHeight = imgsideheight;
            //if(ImgSourcePath != null && ImgSourcePath != "")
            //    ViewImage.Source = ImageSource.FromResource(ImgSourcePath);
        }

        public void SetButtonFocus()
        {
            ViewButton.Focus();
        }

        public LabeledImageButton()
        {
            InitializeComponent();
            ViewButton.Clicked += ViewButton_Clicked;
#if WINDOWS
            ViewButton.HandlerChanged += (s, e) =>
            {
                if (ViewButton.Handler?.PlatformView is Microsoft.UI.Xaml.Controls.Button)
                {
                    var platformView = ViewButton.Handler?.PlatformView as Microsoft.UI.Xaml.Controls.Button;
                    platformView.PointerEntered += PlatformView_PointerEntered;
                    platformView.PointerExited += PlatformView_PointerExited;
                    platformView.Background = new Microsoft.UI.Xaml.Media.SolidColorBrush();
                }
            };
#endif
        }

#if WINDOWS
        private void PlatformView_PointerEntered(object sender, Microsoft.UI.Xaml.Input.PointerRoutedEventArgs e)
        {
            e.Handled = true;
            ViewImage.IsHighlighted = true;
        }

        private void PlatformView_PointerExited(object sender, Microsoft.UI.Xaml.Input.PointerRoutedEventArgs e)
        {
            e.Handled = true;
            ViewImage.IsHighlighted = false;
        }
#endif

        private void ViewButton_Clicked(object sender, EventArgs e)
        {
            BtnClicked?.Invoke(this, e);
        }
    }

}