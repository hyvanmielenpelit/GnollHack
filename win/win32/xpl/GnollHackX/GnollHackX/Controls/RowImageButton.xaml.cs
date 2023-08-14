using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

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
        public static readonly BindableProperty LblTextProperty = BindableProperty.Create(nameof(LblText), typeof(string), typeof(LabeledImageButton), string.Empty);
        public static readonly BindableProperty LblFontSizeProperty = BindableProperty.Create(nameof(LblFontSize), typeof(double), typeof(LabeledImageButton), 14.0);
        public static readonly BindableProperty ImgSourcePathProperty = BindableProperty.Create(nameof(ImgSourcePath), typeof(string), typeof(LabeledImageButton), string.Empty);
        public static readonly BindableProperty GridWidthProperty = BindableProperty.Create(nameof(GridWidth), typeof(double), typeof(LabeledImageButton), 320.0);
        public static readonly BindableProperty GridHeightProperty = BindableProperty.Create(nameof(GridHeight), typeof(double), typeof(LabeledImageButton), 80.0);
        public static readonly BindableProperty GridMarginProperty = BindableProperty.Create(nameof(GridMargin), typeof(Thickness), typeof(LabeledImageButton), new Thickness());
        public static readonly BindableProperty ImgWidthProperty = BindableProperty.Create(nameof(ImgWidth), typeof(double), typeof(LabeledImageButton), 80.0);
        public static readonly BindableProperty ImgHeightProperty = BindableProperty.Create(nameof(ImgHeight), typeof(double), typeof(LabeledImageButton), 80.0);
        public static readonly BindableProperty BtnCommandProperty = BindableProperty.Create(nameof(BtnCommand), typeof(int), typeof(LabeledImageButton), 0);

        public event EventHandler<EventArgs> BtnClicked;
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

        public RowImageButton()
        {
            InitializeComponent();
            ViewButton.Clicked += ViewButton_Clicked;
        }

        private void ViewButton_Clicked(object sender, EventArgs e)
        {
            BtnClicked?.Invoke(this, e);
        }
    }
}