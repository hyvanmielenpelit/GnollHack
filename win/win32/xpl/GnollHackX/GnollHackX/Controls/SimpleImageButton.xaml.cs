using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
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
    public partial class SimpleImageButton : ContentView, IThreadSafeView
    {
        public static readonly BindableProperty ImgSourcePathProperty = BindableProperty.Create(nameof(ImgSourcePath), typeof(string), typeof(SimpleImageButton), string.Empty);
        public static readonly BindableProperty ImgHighFilterQualityProperty = BindableProperty.Create(nameof(ImgHighFilterQuality), typeof(bool), typeof(LabeledImageButton), false);
        public static readonly BindableProperty GridWidthProperty = BindableProperty.Create(nameof(GridWidth), typeof(double), typeof(SimpleImageButton), 45.0);
        public static readonly BindableProperty GridHeightProperty = BindableProperty.Create(nameof(GridHeight), typeof(double), typeof(SimpleImageButton), 45.0);

        public SimpleImageButton()
        {
            InitializeComponent();
            SizeChanged += SimpleImageButton_SizeChanged;
            PropertyChanged += SimpleImageButton_PropertyChanged;
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

        private void SimpleImageButton_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
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
        public bool ImgHighFilterQuality
        {
            get => (bool)GetValue(LabeledImageButton.ImgHighFilterQualityProperty);
            set => SetValue(LabeledImageButton.ImgHighFilterQualityProperty, value);
        }

        public int LandscapeButtonsInRow { get; set; } = 0;
        public int PortraitButtonsInRow { get; set; } = 0;

        public void SetSideSize(double canvasViewWidth, double canvasViewHeight, bool usingDesktopButtons, bool usingSimpleCmdLayout, int stoneButtonRows, float inverseCanvasScale, float customScale)
        {
            //int bigRowNoOfButtons = LandscapeButtonsInRow > 0 ? LandscapeButtonsInRow : UIUtils.LandscapeButtonsInRow(usingDesktopButtons, usingSimpleCmdLayout);
            //bool tooWide = 35.0 * bigRowNoOfButtons + (bigRowNoOfButtons - 1) * 6 > canvaswidth;
            //int noOfButtons = canvaswidth > canvasheight && !tooWide ? bigRowNoOfButtons : PortraitButtonsInRow > 0 ? PortraitButtonsInRow : UIUtils.PortraitButtonsInRow(usingDesktopButtons, usingSimpleCmdLayout);
            //double imgsidewidth = Math.Min(75.0, Math.Max(35.0, (canvaswidth - (noOfButtons - 1) * 6) / Math.Max(1, noOfButtons)));
            double imgsidewidth = UIUtils.CalculateButtonSideWidth(canvasViewWidth, canvasViewHeight, usingDesktopButtons, usingSimpleCmdLayout, stoneButtonRows, inverseCanvasScale, customScale, LandscapeButtonsInRow, PortraitButtonsInRow, true);
            GridWidth = imgsidewidth;
            GridHeight = imgsidewidth;
        }

        private void ViewButton_Clicked(object sender, EventArgs e)
        {
            GHApp.AddSentryBreadcrumb("SimpleImageButton: " + ImgSourcePath, GHConstants.SentryGnollHackButtonClickCategoryName);
            BtnClicked?.Invoke(this, e);
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

        private void SimpleImageButton_SizeChanged(object sender, EventArgs e)
        {
            //lock (_propertyLock)
            //{
            //    _threadSafeWidth = Width;
            //    _threadSafeHeight = Height;
            //}
            ThreadSafeWidth = Width;
            ThreadSafeHeight = Height;
        }

    }
}