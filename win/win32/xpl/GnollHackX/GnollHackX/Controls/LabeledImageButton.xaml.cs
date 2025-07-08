using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading;
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
    public partial class LabeledImageButton : ContentView, IThreadSafeView
    {
        public static readonly BindableProperty LblTextProperty = BindableProperty.Create(nameof(LblText), typeof(string), typeof(LabeledImageButton), string.Empty);
        public static readonly BindableProperty LblFontSizeProperty = BindableProperty.Create(nameof(LblFontSize), typeof(double), typeof(LabeledImageButton), 9.0);
        public static readonly BindableProperty LblFontFamilyProperty = BindableProperty.Create(nameof(LblFontFamily), typeof(string), typeof(LabeledImageButton), "LatoRegular");
        public static readonly BindableProperty LblFontColorProperty = BindableProperty.Create(nameof(LblFontColor), typeof(Color), typeof(LabeledImageButton), GHColors.White);
        public static readonly BindableProperty ImgSourcePathProperty = BindableProperty.Create(nameof(ImgSourcePath), typeof(string), typeof(LabeledImageButton), string.Empty);
        public static readonly BindableProperty ImgHighFilterQualityProperty = BindableProperty.Create(nameof(ImgHighFilterQuality), typeof(bool), typeof(LabeledImageButton), false);
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
        public bool ImgHighFilterQuality
        {
            get => (bool)GetValue(LabeledImageButton.ImgHighFilterQualityProperty);
            set => SetValue(LabeledImageButton.ImgHighFilterQualityProperty, value);
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
        public int LandscapeButtonsInRow { get; set; } = 0;
        public int PortraitButtonsInRow { get; set; } = 0;

        public void SetSideSize(double canvasViewWidth, double canvasViewHeight, bool usingDesktopButtons, bool usingSimpleCmdLayout, float inverseCanvasScale, float customScale)
        {
            double imgsidewidth = UIUtils.CalculateButtonSideWidth(canvasViewWidth, canvasViewHeight, usingDesktopButtons, usingSimpleCmdLayout, inverseCanvasScale, customScale, LandscapeButtonsInRow, PortraitButtonsInRow, false);
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

        private readonly object _propertyLock = new object();
        private double _threadSafeWidth = 0;
        private double _threadSafeHeight = 0;
        private double _threadSafeX = 0;
        private double _threadSafeY = 0;
        private int _threadSafeIsVisible = 1;
        private Thickness _threadSafeMargin = new Thickness();
        WeakReference<IThreadSafeView> _threadSafeParent = null;

        public double ThreadSafeWidth { get { return Interlocked.CompareExchange(ref _threadSafeWidth, 0.0, 0.0); } private set { Interlocked.Exchange(ref _threadSafeWidth, value); } }
        public double ThreadSafeHeight { get { return Interlocked.CompareExchange(ref _threadSafeHeight, 0.0, 0.0); } private set { Interlocked.Exchange(ref _threadSafeHeight, value); } }
        public double ThreadSafeX { get { return Interlocked.CompareExchange(ref _threadSafeX, 0.0, 0.0); } private set { Interlocked.Exchange(ref _threadSafeX, value); } }
        public double ThreadSafeY { get { return Interlocked.CompareExchange(ref _threadSafeY, 0.0, 0.0); } private set { Interlocked.Exchange(ref _threadSafeY, value); } }
        public bool ThreadSafeIsVisible { get { return Interlocked.CompareExchange(ref _threadSafeIsVisible, 0, 0) != 0; } private set { Interlocked.Exchange(ref _threadSafeIsVisible, value ? 1 : 0); } }
        public Thickness ThreadSafeMargin { get { lock (_propertyLock) { return _threadSafeMargin; } } private set { lock (_propertyLock) { _threadSafeMargin = value; } } }
        public WeakReference<IThreadSafeView> ThreadSafeParent { get { lock (_propertyLock) { return _threadSafeParent; } } private set { lock (_propertyLock) { _threadSafeParent = value; } } }

        private void LabeledImageButton_SizeChanged(object sender, EventArgs e)
        {
            //lock (_propertyLock)
            //{
            //    _threadSafeWidth = Width;
            //    _threadSafeHeight = Height;
            //}
            ThreadSafeWidth = Width;
            ThreadSafeHeight = Height;
        }

        public void SetButtonFocus()
        {
            ViewButton.Focus();
        }

        public LabeledImageButton()
        {
            InitializeComponent();
            SizeChanged += LabeledImageButton_SizeChanged;
            PropertyChanged += LabeledImageButton_PropertyChanged;
            lock (_propertyLock)
            {
                _threadSafeWidth = Width;
                _threadSafeHeight = Height;
                _threadSafeX = X;
                _threadSafeY = Y;
                _threadSafeIsVisible = IsVisible ? 1 : 0;
                _threadSafeMargin = Margin;
                if (Parent == null || !(Parent is IThreadSafeView))
                    _threadSafeParent = null;
                else
                    _threadSafeParent = new WeakReference<IThreadSafeView>((IThreadSafeView)Parent);
            }
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

        public void InvalidateSurface()
        {
            ViewImage.InvalidateSurface();
        }

        private void LabeledImageButton_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (e.PropertyName == nameof(IsVisible))
            {
                ThreadSafeIsVisible = IsVisible;
            }
            else if (e.PropertyName == nameof(Width))
            {
                ThreadSafeWidth = Width;
            }
            else if (e.PropertyName == nameof(Height))
            {
                ThreadSafeHeight = Height;
            }
            else if (e.PropertyName == nameof(X))
            {
                ThreadSafeX = X;
            }
            else if (e.PropertyName == nameof(Y))
            {
                ThreadSafeY = Y;
            }
            else if (e.PropertyName == nameof(Margin))
            {
                ThreadSafeMargin = Margin;
            }
            else if (e.PropertyName == nameof(Parent))
            {
                if (Parent == null || !(Parent is IThreadSafeView))
                    ThreadSafeParent = null;
                else
                    ThreadSafeParent = new WeakReference<IThreadSafeView>((IThreadSafeView)Parent);
            }
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

        private void ViewButton_Clicked(object sender, EventArgs e)
        {
            BtnClicked?.Invoke(this, e);
        }
    }

}