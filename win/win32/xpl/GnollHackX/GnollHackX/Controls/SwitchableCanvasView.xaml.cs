using SkiaSharp;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

#if GNH_MAUI
using SkiaSharp.Views.Maui;
using SkiaSharp.Views.Maui.Controls;
using GnollHackX;
namespace GnollHackM
#else
using Xamarin.Forms;
using Xamarin.Forms.Xaml;
using GnollHackX.Pages.Game;
using SkiaSharp.Views.Forms;

namespace GnollHackX.Controls
#endif
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class SwitchableCanvasView : ContentView
    {
        private object glLock = new object();
        private bool _useGL = false;
        public bool UseGL 
        {   get { lock (glLock) { return _useGL; } }
            set
            {
                lock(glLock)
                {
                    _useGL = value;
                }
                internalCanvasView.IsVisible = !value;
                internalGLView.IsVisible = value;
            }
        }

        public SwitchableCanvasView()
        {
            InitializeComponent();
        }

        public SKSize CanvasSize { get { return UseGL ? internalGLView.CanvasSize : internalCanvasView.CanvasSize; } }
        public bool IgnorePixelScaling
        {
            get { return UseGL ? false : internalCanvasView.IgnorePixelScaling; }
            set
            {
                internalCanvasView.IgnorePixelScaling = value;
            }
        }
        public bool EnableTouchEvents 
        { 
            get { return UseGL ? internalGLView.EnableTouchEvents : internalCanvasView.EnableTouchEvents; } 
            set 
            {
                internalGLView.EnableTouchEvents = value;
                internalCanvasView.EnableTouchEvents = value;
            }
        }
        public event EventHandler<SKPaintSurfaceEventArgs> PaintSurface;
        public event EventHandler<SKTouchEventArgs> Touch;
        public void InvalidateSurface()
        {
            if(UseGL)
                internalGLView.InvalidateSurface();
            else
                internalCanvasView.InvalidateSurface();
        }
        protected override SizeRequest OnMeasure(double widthConstraint, double heightConstraint)
        {
            return new SizeRequest();
        }

        private void internalCanvasView_PaintSurface(object sender, SKPaintSurfaceEventArgs e)
        {
            PaintSurface?.Invoke(sender, e);
        }

        private void internalCanvasView_Touch(object sender, SKTouchEventArgs e)
        {
            Touch?.Invoke(sender, e);
        }

#if GNH_MAUI
        private void internalGLView_PaintSurface(object sender, SKPaintSurfaceEventArgs e)
        {
            PaintSurface?.Invoke(sender, e);
        }
#else
        private void internalGLView_PaintSurface(object sender, SKPaintGLSurfaceEventArgs e)
        {
            SKImageInfo info = new SKImageInfo();
            info.ColorType = e.ColorType;
            SKPaintSurfaceEventArgs convargs = new SKPaintSurfaceEventArgs(e.Surface, info);
            PaintSurface?.Invoke(sender, convargs);
        }
#endif

        private void internalGLView_Touch(object sender, SKTouchEventArgs e)
        {
            Touch?.Invoke(sender, e);
        }




        public GamePage _gamePage;
        public Grid _parentGrid;

        public GHWindow GHWindow { get; set; }
        public ghmenu_styles MenuStyle { get; set; }
        public readonly object MenuItemLock = new object();

        private ObservableCollection<GHMenuItem> _GHMenuItems = null;
        public ObservableCollection<GHMenuItem> MenuItems { get { return _GHMenuItems; } set { _GHMenuItems = value; } }

        public readonly object TextItemLock = new object();
        private List<GHPutStrItem> _GHPutStrItems = null;
        public List<GHPutStrItem> PutStrItems { get { return _GHPutStrItems; } set { _GHPutStrItems = value; } }

        public SelectionMode SelectionHow { get; set; }
        public int SelectionIndex { get; set; }

        public bool RevertBlackAndWhite { get; set; }
        public bool UseTextOutline { get; set; }
        public bool HideMenuLetters { get; set; }
        public bool MenuButtonStyle { get; set; }
        public bool ClickOKOnSelection { get; set; }
        public bool MenuGlyphAtBottom { get; set; }

        public CanvasTypes CanvasType { get; set; }


        public BindableProperty GeneralAnimationCounterProperty =
            BindableProperty.Create(nameof(GeneralAnimationCounter), typeof(long), typeof(SwitchableCanvasView), 0L);

        public long GeneralAnimationCounter
        {
            get { return (long)GetValue(GeneralAnimationCounterProperty); }
            set { SetValue(GeneralAnimationCounterProperty, value); }
        }

        private long _tickCounter = 0L;

        protected override void OnPropertyChanged(string propertyName = null)
        {
            base.OnPropertyChanged(propertyName);
            if (!IsVisible || (_parentGrid != null && !_parentGrid.IsVisible))
                return;
            if (_gamePage != null)
            {
                _tickCounter++;
                _tickCounter = _tickCounter % GHConstants.MaxRefreshRate;
                int auxRefreshRate = UIUtils.GetAuxiliaryCanvasAnimationFrequency();
                int mainfps = UIUtils.GetMainCanvasAnimationFrequency(_gamePage.MapRefreshRate);
                int divisor = Math.Max(1, (int)Math.Round((double)auxRefreshRate / (double)mainfps, 0));
                switch (CanvasType)
                {
                    case CanvasTypes.MainCanvas:
                        {
                            _gamePage.UpdateMainCanvas();
                            break;
                        }
                    case CanvasTypes.CommandCanvas:
                        {
                            _gamePage.UpdateCommandCanvas();
                            break;
                        }
                    case CanvasTypes.MenuCanvas:
                        {
                            if (_tickCounter % divisor == 0)
                                _gamePage.IncrementCounters();

                            _gamePage.UpdateMenuCanvas();
                            break;
                        }
                    case CanvasTypes.TextCanvas:
                        {
                            if (_tickCounter % divisor == 0)
                                _gamePage.IncrementCounters();

                            _gamePage.UpdateTextCanvas();
                            break;
                        }
                    default:
                        break;
                }
            }
        }
    }
}