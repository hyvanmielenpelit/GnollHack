using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace GnollHackClient.Controls
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
        }

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

        public int LandscapeButtonsInRow { get; set; } = 16;
        public int PortraitButtonsInRow { get; set; } = 8;

        public void SetSideSize(double canvaswidth, double canvasheight)
        {
            double imgsidewidth = 0;
            if (canvaswidth > canvasheight)
                imgsidewidth = Math.Min(75.0, Math.Max(35.0, canvaswidth / Math.Max(1, LandscapeButtonsInRow)));
            else
                imgsidewidth = Math.Min(75.0, Math.Max(35.0, canvaswidth / Math.Max(1, PortraitButtonsInRow)));

            GridWidth = imgsidewidth;
            GridHeight = imgsidewidth;
        }

        private void ViewButton_Clicked(object sender, EventArgs e)
        {
            BtnClicked?.Invoke(this, e);
        }

    }
}