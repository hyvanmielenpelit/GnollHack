using FFImageLoading.Forms;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace GnollHackClient.Controls
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class LabeledImageButton : ContentView
    {
        public static readonly BindableProperty LblTextProperty = BindableProperty.Create(nameof(LblText), typeof(string), typeof(LabeledImageButton), string.Empty);
        public static readonly BindableProperty LblFontSizeProperty = BindableProperty.Create(nameof(LblFontSize), typeof(double), typeof(LabeledImageButton), 9.0);
        public static readonly BindableProperty ImgSourcePathProperty = BindableProperty.Create(nameof(ImgSourcePath), typeof(string), typeof(LabeledImageButton), string.Empty);
        public static readonly BindableProperty GridWidthProperty = BindableProperty.Create(nameof(GridWidth), typeof(double), typeof(LabeledImageButton), 80.0);
        public static readonly BindableProperty GridHeightProperty = BindableProperty.Create(nameof(GridHeight), typeof(double), typeof(LabeledImageButton), 97.0);
        public static readonly BindableProperty ImgWidthProperty = BindableProperty.Create(nameof(ImgWidth), typeof(double), typeof(LabeledImageButton), 80.0);
        public static readonly BindableProperty ImgHeightProperty = BindableProperty.Create(nameof(ImgHeight), typeof(double), typeof(LabeledImageButton), 80.0);
        public static readonly BindableProperty BtnLetterProperty = BindableProperty.Create(nameof(BtnLetter), typeof(char), typeof(LabeledImageButton), (char)0);
        public static readonly BindableProperty BtnMetaProperty = BindableProperty.Create(nameof(BtnMeta), typeof(bool), typeof(LabeledImageButton), false);
        public static readonly BindableProperty BtnCtrlProperty = BindableProperty.Create(nameof(BtnCtrl), typeof(bool), typeof(LabeledImageButton), false);

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

        public string ImgSourcePath
        {
            get => (string)GetValue(LabeledImageButton.ImgSourcePathProperty);
            set => SetValue(LabeledImageButton.ImgSourcePathProperty, value); 
        }
        public double ImgWidth
        {
            get => (double)GetValue(LabeledImageButton.ImgHeightProperty);
            set => SetValue(LabeledImageButton.ImgHeightProperty, value);
        }
        public double ImgHeight
        {
            get => (double)GetValue(LabeledImageButton.ImgWidthProperty);
            set => SetValue(LabeledImageButton.ImgWidthProperty, value);
        }

        public double GridWidth
        {
            get => (double)GetValue(LabeledImageButton.GridHeightProperty);
            set => SetValue(LabeledImageButton.GridHeightProperty, value);
        }
        public double GridHeight
        {
            get => (double)GetValue(LabeledImageButton.GridWidthProperty);
            set => SetValue(LabeledImageButton.GridWidthProperty, value);
        }

        public bool LargerFont { get; set; }

        public void SetSideSize(double canvaswidth, double canvasheight)
        {
            double imgsidewidth = Math.Min(Math.Min(80.0, Math.Max(50.0, canvaswidth / 7)), Math.Min(80.0, Math.Max(50.0, canvasheight / 7)));
            double imgsideheight = Math.Min(Math.Min(80.0, Math.Max(50.0, canvaswidth / 7)), Math.Min(80.0, Math.Max(50.0, canvasheight / 7)));
            double fontsize = 9.0 * imgsidewidth / 50.0;
            double fontsize_larger = 9.5 * imgsidewidth / 50.0;
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

        public LabeledImageButton()
        {
            InitializeComponent();
            ViewButton.Clicked += ViewButton_Clicked;
        }

        private void ViewButton_Clicked(object sender, EventArgs e)
        {
            BtnClicked?.Invoke(sender, e);
        }
    }

}