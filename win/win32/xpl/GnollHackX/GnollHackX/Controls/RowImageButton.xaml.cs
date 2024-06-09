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
        public static readonly BindableProperty LblTextProperty = BindableProperty.Create(nameof(LblText), typeof(string), typeof(RowImageButton), string.Empty);
        public static readonly BindableProperty LblFontSizeProperty = BindableProperty.Create(nameof(LblFontSize), typeof(double), typeof(RowImageButton), 14.0);
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
        public double LblFontSize
        {
            get => (double)GetValue(RowImageButton.LblFontSizeProperty);
            set => SetValue(RowImageButton.LblFontSizeProperty, value);
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
        }

        private void ViewButton_Clicked(object sender, EventArgs e)
        {
            BtnClicked?.Invoke(this, e);
        }
    }
}