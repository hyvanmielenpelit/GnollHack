using SkiaSharp;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;
using GnollHackX;


#if GNH_MAUI
using SkiaSharp.Views.Maui;
using SkiaSharp.Views.Maui.Controls;

#if WINDOWS
using Microsoft.UI.Input;
#endif

namespace GnollHackM
#else
using Xamarin.Forms;
using Xamarin.Forms.Xaml;
using GnollHackX.Pages.Game;
using SkiaSharp.Views.Forms;
using Xamarin.Essentials;
using System.Threading;

namespace GnollHackX.Controls
#endif
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class SwitchableCanvasView : ContentView, IThreadSafeView
    {
        //private object _glLock = new object();
        private int _useGL = 0;
        private readonly SKGLView _internalGLView = null;

        public bool UseGL 
        {   get
            {
                //lock (_glLock) { return _useGL; }
                return Interlocked.CompareExchange(ref _useGL, 0, 0) != 0;
            }
            set
            {
                Interlocked.Exchange(ref _useGL, value ? 1 : 0);
                //lock(_glLock)
                //{
                //    _useGL = value;
                //}
                if(HasGL)
                {
                    internalCanvasView.IsVisible = !value;
                    _internalGLView.IsVisible = value;
                }
                else
                    internalCanvasView.IsVisible = true;
            }
        }

        public bool HasGL {  get { return _internalGLView != null; } }

        public SwitchableCanvasView()
        {
            bool gpuAvailable = GHApp.IsGPUAvailable;
            if (gpuAvailable)
            {
                _internalGLView = new SKGLView()
                {
                    IsVisible = false,
                    HorizontalOptions = LayoutOptions.Fill,
                    VerticalOptions = LayoutOptions.Fill,
                };
                _internalGLView.PaintSurface += internalGLView_PaintSurface;
                _internalGLView.Touch += internalGLView_Touch;
                //_internalGLView.PropertyChanged += internalGLView_PropertyChanged;
#if WINDOWS
                _internalGLView.HandlerChanged += (s, e) =>
                {
                    SkiaSharp.Views.Windows.SKSwapChainPanel glView = _internalGLView?.Handler?.PlatformView as SkiaSharp.Views.Windows.SKSwapChainPanel;
                    if (glView != null)
                    {
                        glView.PointerWheelChanged += View_PointerWheelChanged;
                        glView.PointerEntered += View_PointerEntered;
                        glView.PointerExited += View_PointerExited;
                        glView.PointerMoved += View_PointerMoved;
                        glView.PointerCanceled += View_PointerCanceled;
                    }
                };
                _internalGLView.HandlerChanging += (s, e) =>
                {
                    if(e.OldHandler != null && e.NewHandler == null)
                    {
                        SkiaSharp.Views.Windows.SKSwapChainPanel glView = _internalGLView?.Handler?.PlatformView as SkiaSharp.Views.Windows.SKSwapChainPanel;
                        if (glView != null)
                        {
                            glView.PointerWheelChanged -= View_PointerWheelChanged;
                            glView.PointerEntered -= View_PointerEntered;
                            glView.PointerExited -= View_PointerExited;
                            glView.PointerMoved -= View_PointerMoved;
                            glView.PointerCanceled -= View_PointerCanceled;
                        }
                    }
                };
#endif
            }
            InitializeComponent();
            if(gpuAvailable)
            {
                RootGrid.Children.Add(_internalGLView);
            }
            ThreadSafeWidth = Width;
            ThreadSafeHeight = Height;
            ThreadSafeX = X;
            ThreadSafeY = Y;
            ThreadSafeIsVisible = IsVisible;
            lock (_propertyLock)
            {
                _threadSafeMargin = Margin;
                if (Parent == null || !(Parent is IThreadSafeView))
                    _threadSafeParent = null;
                else
                    _threadSafeParent = new WeakReference<IThreadSafeView>((IThreadSafeView)Parent);
                //_threadSafeInternalCanvasSize = internalCanvasView.CanvasSize;
                //if (HasGL)
                //    _threadSafeInternalGLCanvasSize = _internalGLView.CanvasSize;
            }
            SizeChanged += SwitchableCanvasView_SizeChanged;
            PropertyChanged += SwitchableCanvasView_PropertyChanged;
#if GNH_MAUI
            HandlerChanged += SwitchableCanvasView_HandlerChanged;
#endif
        }

#if GNH_MAUI
        private void SwitchableCanvasView_HandlerChanged(object sender, EventArgs e)
        {
            if (Handler != null)
            {
#if ANDROID
                if (Handler?.PlatformView is Android.Views.View nativeView)
                {
                    var density = nativeView.Context.Resources.DisplayMetrics.Density;
                    nativeView.SetCameraDistance(8000f * density);
                }
#endif
            }
        }
#endif

        private int _shutDown = 0;
        public bool IsShutDown { get { return Interlocked.CompareExchange(ref _shutDown, 0, 0) != 0; } }
        public void ShutDown()
        {
            if (Interlocked.CompareExchange(ref _shutDown, 1, 0) == 0) // Change the value to 1 if it is 0
            {
                // Handle the GRContext on the main thread
                MainThread.BeginInvokeOnMainThread(() => 
                {
                    SizeChanged -= SwitchableCanvasView_SizeChanged;
                    PropertyChanged -= SwitchableCanvasView_PropertyChanged;
#if GNH_MAUI
                    HandlerChanged -= SwitchableCanvasView_HandlerChanged;
#endif
                    internalCanvasView.PaintSurface -= internalCanvasView_PaintSurface;
                    internalCanvasView.Touch -= internalCanvasView_Touch;
                    if (_internalGLView != null)
                    {
                        _internalGLView.PaintSurface -= internalGLView_PaintSurface;
                        _internalGLView.Touch -= internalGLView_Touch;
#if GNH_MAUI
                        _internalGLView.Handler?.DisconnectHandler();
#endif
                    }
                });
            }
        }

        private void SwitchableCanvasView_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
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

        //private void internalCanvasView_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        //{
        //    if (!HasGL || !UseGL)
        //    {
        //        if (e.PropertyName == nameof(internalCanvasView.CanvasSize))
        //        {
        //            lock (_propertyLock)
        //            {
        //                _threadSafeInternalCanvasSize = internalCanvasView.CanvasSize;
        //            }
        //        }
        //    }
        //}

        //private void internalGLView_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        //{
        //    if (HasGL)
        //    {
        //        if (e.PropertyName == nameof(_internalGLView.CanvasSize))
        //        {
        //            lock (_propertyLock)
        //            {
        //                _threadSafeInternalGLCanvasSize = _internalGLView.CanvasSize;
        //            }
        //        }
        //    }
        //}

        private double _threadSafeWidth = 0;
        private double _threadSafeHeight = 0;
        private double _threadSafeX = 0;
        private double _threadSafeY = 0;
        private int _threadSafeIsVisible = 1;

        private readonly object _propertyLock = new object();
        private Thickness _threadSafeMargin = new Thickness();
        WeakReference<IThreadSafeView> _threadSafeParent = null;

        public double ThreadSafeWidth { get { return Interlocked.CompareExchange(ref _threadSafeWidth, 0.0, 0.0); } private set { Interlocked.Exchange(ref _threadSafeWidth, value); } }
        public double ThreadSafeHeight { get { return Interlocked.CompareExchange(ref _threadSafeHeight, 0.0, 0.0); } private set { Interlocked.Exchange(ref _threadSafeHeight, value); } }
        public double ThreadSafeX { get { return Interlocked.CompareExchange(ref _threadSafeX, 0.0, 0.0); } private set { Interlocked.Exchange(ref _threadSafeX, value); } }
        public double ThreadSafeY { get { return Interlocked.CompareExchange(ref _threadSafeY, 0.0, 0.0); } private set { Interlocked.Exchange(ref _threadSafeY, value); } }
        public bool ThreadSafeIsVisible { get { return Interlocked.CompareExchange(ref _threadSafeIsVisible, 0, 0) != 0; } private set { Interlocked.Exchange(ref _threadSafeIsVisible, value ? 1 : 0); } }
        public Thickness ThreadSafeMargin { get { lock (_propertyLock) { return _threadSafeMargin; } } private set { lock (_propertyLock) { _threadSafeMargin = value; } } }
        public WeakReference<IThreadSafeView> ThreadSafeParent { get { lock (_propertyLock) { return _threadSafeParent; } } private set { lock (_propertyLock) { _threadSafeParent = value; } } }
        
        //public SKSize ThreadSafeCanvasSize { 
        //    get 
        //    { 
        //        bool usingGL = UseGL && HasGL; 
        //        lock (_propertyLock) 
        //        { 
        //            return usingGL ? _threadSafeInternalGLCanvasSize : _threadSafeInternalCanvasSize; 
        //        } 
        //    }  
        //}

        private void SwitchableCanvasView_SizeChanged(object sender, EventArgs e)
        {
            ThreadSafeWidth = Width;
            ThreadSafeHeight = Height;
        }

        public SKSize CanvasSize { get { return UseGL && HasGL ? _internalGLView.CanvasSize : internalCanvasView.CanvasSize; } }
        public bool IgnorePixelScaling
        {
            get { return UseGL && HasGL ? false : internalCanvasView.IgnorePixelScaling; }
            set
            {
                internalCanvasView.IgnorePixelScaling = value;
            }
        }
        public bool EnableTouchEvents 
        { 
            get { return UseGL && HasGL ? _internalGLView.EnableTouchEvents : internalCanvasView.EnableTouchEvents; } 
            set 
            {
                if (HasGL)
                    _internalGLView.EnableTouchEvents = value;
                internalCanvasView.EnableTouchEvents = value;
            }
        }
        public event EventHandler<SKPaintSurfaceEventArgs> PaintSurface;
        public event EventHandler<SKTouchEventArgs> Touch;
#pragma warning disable 67
        public event EventHandler<GHMouseWheelEventArgs> MouseWheel;
        public event EventHandler<SKTouchEventArgs> MousePointer;
#pragma warning restore 67

        public void InvalidateSurface()
        {
            if (IsShutDown)
                return;

            if (UseGL && HasGL)
            {
#if false
                if(GHApp.WindowsXamlWindow != null)
                {
                    GHApp.WindowsXamlWindow.DispatcherQueue?.TryEnqueue(Microsoft.UI.Dispatching.DispatcherQueuePriority.High, () => 
                    {
#endif
                        try
                        {
                            _internalGLView.InvalidateSurface();
                        }
                        catch (Exception ex) 
                        {
                            Debug.WriteLine(ex);    
                        }
#if false
                    });
                }
#endif
            }
            else
            {
                try
                {
                    internalCanvasView.InvalidateSurface();
                }
                catch (Exception ex)
                {
                    Debug.WriteLine(ex);
                }
            }
        }

        //protected override SizeRequest OnMeasure(double widthConstraint, double heightConstraint)
        //{
        //    return new SizeRequest();
        //}

        private bool _firstCanvasDraw = true;

        private void internalCanvasView_PaintSurface(object sender, SKPaintSurfaceEventArgs e)
        {
            if (UseGL && HasGL)
                return; /* Insurance in the case both canvases mistakenly are updated */

            if (IsShutDown)
                return;

            if(_firstCanvasDraw)
            {
                _firstCanvasDraw = false;
                SKCanvas canvas = e?.Surface?.Canvas;
                if (canvas != null)
                    canvas.Clear(CanvasType == CanvasTypes.CommandCanvas || CanvasType == CanvasTypes.MainCanvas ? SKColors.Black : SKColors.Transparent);
            }
            PaintSurface?.Invoke(sender, e);
        }

        private bool _canvasTouchThreadChecked = false;
        private void internalCanvasView_Touch(object sender, SKTouchEventArgs e)
        {
            bool isCanvasOnMainThread = MainThread.IsMainThread;
            if (!_canvasTouchThreadChecked && !isCanvasOnMainThread)
            {
                _canvasTouchThreadChecked = true;
                GHApp.MaybeWriteGHLog("internalCanvasView_Touch not on main thread!");
            }
            if (IsShutDown)
                return;
            MainThread.BeginInvokeOnMainThread(() =>
            {
                Touch?.Invoke(sender, e);
            });
        }

        private bool _firstDraw = true;

        private void internalGLView_PaintSurface(object sender, SKPaintGLSurfaceEventArgs e)
        {
            if (!UseGL || !HasGL)
                return; /* Insurance in the case both canvases mistakenly are updated */

            if (IsShutDown)
                return;

            if (_firstDraw)
            {
                _firstDraw = false;
                if (CanvasType == CanvasTypes.MainCanvas)
                    GHApp.DefaultGPUCacheSize = ResourceCacheLimit;

                SKCanvas canvas = e?.Surface?.Canvas;
                if (canvas != null)
                    canvas.Clear(CanvasType == CanvasTypes.CommandCanvas || CanvasType == CanvasTypes.MainCanvas ? SKColors.Black : SKColors.Transparent);

                if (e.BackendRenderTarget != null)
                {
                    if(CanvasType == CanvasTypes.MainCanvas)
                        GHApp.GPUBackend = e.BackendRenderTarget.Backend.ToString();

                    Debug.WriteLine("Using is Skia GPU Rendering: GRContext Backend is " + e.BackendRenderTarget.Backend.ToString());
                }
                else
                    Debug.WriteLine("Using Skia GPU Rendering: BackendRenderTarget is null");

                /* Set to requested PrimaryCache limits */
                long defaultSize = GHApp.DefaultGPUCacheSize;
                if (CanvasType == CanvasTypes.MainCanvas)
                {
                    long limit = GHApp.PrimaryGPUCacheLimit;
                    Debug.WriteLine("PrimaryGPUCacheLimit is " + limit);
                    try
                    {
                        if (limit > 0)
                            _internalGLView.GRContext.SetResourceCacheLimit(limit);
                        else if (limit == -2 && GHApp.RecommendedPrimaryGPUCacheSize > 0)
                            _internalGLView.GRContext.SetResourceCacheLimit(GHApp.RecommendedPrimaryGPUCacheSize);
                        else if (limit == -3 && defaultSize > 0)
                            _internalGLView.GRContext.SetResourceCacheLimit(defaultSize);
                    }
                    catch (Exception ex)
                    {
                        Debug.WriteLine(ex.Message);
                    }

                    long newLimit = ResourceCacheLimit;
                    Debug.WriteLine("ResourceCacheSize is now " + newLimit);
                    GHApp.CurrentGPUCacheSize = newLimit;
                    GHApp.CurrentGPUCacheUsage = ResourceCacheUsage;
                }
                else
                {
                    long limit = GHApp.SecondaryGPUCacheLimit;
                    Debug.WriteLine("SecondaryGPUCacheLimit is " + limit);
                    try
                    {
                        if (limit > 0)
                            _internalGLView.GRContext.SetResourceCacheLimit(limit);
                        else if (limit == -2 && GHApp.RecommendedSecondaryGPUCacheSize > 0)
                            _internalGLView.GRContext.SetResourceCacheLimit(GHApp.RecommendedSecondaryGPUCacheSize);
                        else if (limit == -3 && defaultSize > 0)
                            _internalGLView.GRContext.SetResourceCacheLimit(defaultSize);
                        Debug.WriteLine("ResourceCacheSize is now " + ResourceCacheLimit);
                    }
                    catch (Exception ex)
                    {
                        Debug.WriteLine(ex.Message);
                    }
                }
            }

            /* Note: this case most likely will never happen, but is here still as a backup */
            long delayedResourceCacheLimit = DelayedResourceCacheLimit;
            if (delayedResourceCacheLimit != -1 && _internalGLView?.GRContext != null)
            {
                Debug.WriteLine("CanvasType is " + CanvasType.ToString());
                Debug.WriteLine("ResourceCacheSize is " + ResourceCacheLimit);
                try
                {
                    if (delayedResourceCacheLimit > 0)
                    {
                        Debug.WriteLine("_delayedResourceCacheLimit is " + delayedResourceCacheLimit);
                        _internalGLView.GRContext.SetResourceCacheLimit(delayedResourceCacheLimit);
                        Debug.WriteLine("ResourceCacheSize is now " + ResourceCacheLimit);
                    }
                    else if (delayedResourceCacheLimit == -2) /* Recommended */
                    {
                        long defaultSize = CanvasType == CanvasTypes.MainCanvas ? GHApp.RecommendedPrimaryGPUCacheSize : GHApp.RecommendedSecondaryGPUCacheSize;
                        Debug.WriteLine("_delayedResourceCacheLimit is " + delayedResourceCacheLimit);
                        Debug.WriteLine("RecommendedGPUCacheSize is " + defaultSize);
                        if (defaultSize > 0)
                        {
                            _internalGLView.GRContext.SetResourceCacheLimit(defaultSize);
                        }
                        Debug.WriteLine("ResourceCacheSize is now " + ResourceCacheLimit);
                    }
                    else if (delayedResourceCacheLimit == -3) /* Skia Default */
                    {
                        long defaultSize = GHApp.DefaultGPUCacheSize;
                        Debug.WriteLine("_delayedResourceCacheLimit is " + delayedResourceCacheLimit);
                        Debug.WriteLine("DefaultGPUCacheSize is " + defaultSize);
                        if (defaultSize > 0)
                        {
                            _internalGLView.GRContext.SetResourceCacheLimit(defaultSize);
                        }
                        Debug.WriteLine("ResourceCacheSize is now " + ResourceCacheLimit);
                    }
                }
                catch (Exception ex)
                {
                    Debug.WriteLine(ex.Message);
                }
                DelayedResourceCacheLimit = -1;
                if (CanvasType == CanvasTypes.MainCanvas)
                    GHApp.CurrentGPUCacheSize = ResourceCacheLimit;
            }

#if GNH_MAUI
            SKPaintSurfaceEventArgs convargs = new SKPaintSurfaceEventArgs(e.Surface, e.Info);
#else
            SKImageInfo info = new SKImageInfo();
            info.ColorType = e.ColorType;
            info.Width = e.BackendRenderTarget?.Width ?? 0;
            info.Height = e.BackendRenderTarget?.Height ?? 0;
            SKPaintSurfaceEventArgs convargs = new SKPaintSurfaceEventArgs(e.Surface, info);
#endif
            PaintSurface?.Invoke(sender, convargs);
        }

        private bool _glTouchThreadChecked = false;

        private void internalGLView_Touch(object sender, SKTouchEventArgs e)
        {
            bool isCanvasOnMainThread = MainThread.IsMainThread;
            if (!_glTouchThreadChecked && !isCanvasOnMainThread)
            {
                _glTouchThreadChecked = true;
                GHApp.MaybeWriteGHLog("internalGLView_Touch not on main thread!");
            }
            if (IsShutDown)
                return;
            MainThread.BeginInvokeOnMainThread(() =>
            {
                Touch?.Invoke(sender, e);
            });
        }

        public GamePage _gamePage;
        public Grid _parentGrid;

        private GHWindow _gHWindow;
        private int _menuStyle;
        private ObservableCollection<GHMenuItem> _menuItems = null;
        private List<GHPutStrItem> _putStrItems = null;
        private int _selectionHow;
        private int _selectionIndex = -1;
        private long _delayedResourceCacheLimit = -1L;
        private int _revertBlackAndWhite;
        private int _useTextOutline;
        private int _hideMenuLetters;
        private int _menuButtonStyle;
        private int _clickOKOnSelection;
        private int _menuGlyphAtBottom;
        private int _allowLongTap = 1;
        private int _specialClickOnLongTap;
        private int _allowHighlight;

        public CanvasTypes CanvasType { get; init; }

        public GHWindow GHWindow { get { return Interlocked.CompareExchange(ref _gHWindow, null, null); } set { Interlocked.Exchange(ref _gHWindow, value); } }
        public ghmenu_styles MenuStyle { get { return (ghmenu_styles)Interlocked.CompareExchange(ref _menuStyle, 0, 0); } set { Interlocked.Exchange(ref _menuStyle, (int)value); } }
        public ObservableCollection<GHMenuItem> MenuItems { get { return Interlocked.CompareExchange(ref _menuItems, null, null); } set { Interlocked.Exchange(ref _menuItems, value); } }
        public List<GHPutStrItem> PutStrItems { get { return Interlocked.CompareExchange(ref _putStrItems, null, null); } set { Interlocked.Exchange(ref _putStrItems, value); } } //{ get { return _GHPutStrItems; } set { _GHPutStrItems = value; } }

        public SelectionMode SelectionHow { get { return (SelectionMode)Interlocked.CompareExchange(ref _selectionHow, 0, 0); } set { Interlocked.Exchange(ref _selectionHow, (int)value); } }
        public int SelectionIndex { get { return Interlocked.CompareExchange(ref _selectionIndex, 0, 0); } set { Interlocked.Exchange(ref _selectionIndex, value); } }
        public long DelayedResourceCacheLimit { get { return Interlocked.CompareExchange(ref _delayedResourceCacheLimit, 0, 0); } set { Interlocked.Exchange(ref _delayedResourceCacheLimit, value); } }
        public bool RevertBlackAndWhite { get { return Interlocked.CompareExchange(ref _revertBlackAndWhite, 0, 0) != 0; } set { Interlocked.Exchange(ref _revertBlackAndWhite, value ? 1 : 0); } }
        public bool UseTextOutline { get { return Interlocked.CompareExchange(ref _useTextOutline, 0, 0) != 0; } set { Interlocked.Exchange(ref _useTextOutline, value ? 1 : 0); } }
        public bool HideMenuLetters { get { return Interlocked.CompareExchange(ref _hideMenuLetters, 0, 0) != 0; } set { Interlocked.Exchange(ref _hideMenuLetters, value ? 1 : 0); } }
        public bool MenuButtonStyle { get { return Interlocked.CompareExchange(ref _menuButtonStyle, 0, 0) != 0; } set { Interlocked.Exchange(ref _menuButtonStyle, value ? 1 : 0); } }
        public bool ClickOKOnSelection { get { return Interlocked.CompareExchange(ref _clickOKOnSelection, 0, 0) != 0; } set { Interlocked.Exchange(ref _clickOKOnSelection, value ? 1 : 0); } }
        public bool MenuGlyphAtBottom { get { return Interlocked.CompareExchange(ref _menuGlyphAtBottom, 0, 0) != 0; } set { Interlocked.Exchange(ref _menuGlyphAtBottom, value ? 1 : 0); } }
        public bool AllowLongTap { get { return Interlocked.CompareExchange(ref _allowLongTap, 0, 0) != 0; } set { Interlocked.Exchange(ref _allowLongTap, value ? 1 : 0); } }
        public bool SpecialClickOnLongTap { get { return Interlocked.CompareExchange(ref _specialClickOnLongTap, 0, 0) != 0; } set { Interlocked.Exchange(ref _specialClickOnLongTap, value ? 1 : 0); } }
        public bool AllowHighlight { get { return Interlocked.CompareExchange(ref _allowHighlight, 0, 0) != 0; } set { Interlocked.Exchange(ref _allowHighlight, value ? 1 : 0); } }

        public long ResourceCacheLimit
        {
            get 
            {
                try
                {
                    return _internalGLView?.GRContext != null ? _internalGLView.GRContext.GetResourceCacheLimit() : -1;
                }
                catch (Exception ex)
                {
                    Debug.WriteLine(ex.Message);
                }
                return -1;
            }
            set
            {
                if(_internalGLView?.GRContext != null)
                {
                    try
                    {
                        switch (value)
                        {
                            case -3:
                                {
                                    long defaultSize = GHApp.DefaultGPUCacheSize;
                                    if (defaultSize > 0)
                                        _internalGLView.GRContext.SetResourceCacheLimit(defaultSize);
                                    break;
                                }
                            case -2:
                                {
                                    long defaultSize = CanvasType == CanvasTypes.MainCanvas ? GHApp.RecommendedPrimaryGPUCacheSize : GHApp.RecommendedSecondaryGPUCacheSize;
                                    if (defaultSize > 0)
                                        _internalGLView.GRContext.SetResourceCacheLimit(defaultSize);
                                    break;
                                }
                            default:
                                if (value > 0)
                                    _internalGLView.GRContext.SetResourceCacheLimit(value);
                                break;
                        }
                    }
                    catch (Exception ex)
                    {
                        Debug.WriteLine(ex.Message);
                    }
                }
                else
                {
                    DelayedResourceCacheLimit = value;
                }
            }
        }

        public CacheUsageInfo ResourceCacheUsage
        {
            get
            {
                CacheUsageInfo res = new CacheUsageInfo(-1, -1);
                if (_internalGLView?.GRContext != null)
                {
                    try
                    {
                        _internalGLView.GRContext.GetResourceCacheUsage(out res.MaxResources, out res.MaxResourceBytes);
                    }
                    catch (Exception ex)
                    {
                        Debug.WriteLine(ex.Message);
                    }
                }
                return res;
            }
        }

        public BindableProperty GeneralAnimationCounterProperty =
            BindableProperty.Create(nameof(GeneralAnimationCounter), typeof(long), typeof(SwitchableCanvasView), 0L);

        //private readonly object _generalAnimationCounterLock = new object();
        public long GeneralAnimationCounter
        {
            get { return (long)GetValue(GeneralAnimationCounterProperty); }
            set { SetValue(GeneralAnimationCounterProperty, value); }
        }

        //private long _tickCounter = 0L;

        protected override void OnPropertyChanged(string propertyName = null)
        {
            base.OnPropertyChanged(propertyName);

            if (_gamePage != null && propertyName == nameof(GeneralAnimationCounter))
            {
                if (!IsVisible || (_parentGrid != null && !_parentGrid.IsVisible))
                    return;

                _gamePage.RenderCanvasByCanvasType(CanvasType);
            }
        }

#if GNH_MAUI
        protected override void OnHandlerChanged()
        {
            base.OnHandlerChanged();
#if WINDOWS
            SkiaSharp.Views.Windows.SKXamlCanvas view = internalCanvasView.Handler?.PlatformView as SkiaSharp.Views.Windows.SKXamlCanvas;
            if(view != null)
            {
                view.PointerWheelChanged += View_PointerWheelChanged;
                view.PointerEntered += View_PointerEntered;
                view.PointerExited += View_PointerExited;
                view.PointerMoved += View_PointerMoved;
                view.PointerCanceled += View_PointerCanceled;
            }
#endif
        }

        protected override void OnHandlerChanging(HandlerChangingEventArgs args)
        {
            base.OnHandlerChanging(args);
#if WINDOWS
            if(args.OldHandler != null && args.NewHandler == null)
            {
                SkiaSharp.Views.Windows.SKXamlCanvas view = internalCanvasView.Handler?.PlatformView as SkiaSharp.Views.Windows.SKXamlCanvas;
                if (view != null)
                {
                    view.PointerWheelChanged -= View_PointerWheelChanged;
                    view.PointerEntered -= View_PointerEntered;
                    view.PointerExited -= View_PointerExited;
                    view.PointerMoved -= View_PointerMoved;
                    view.PointerCanceled -= View_PointerCanceled;
                }
            }
#endif
        }


#if WINDOWS
        private void View_PointerWheelChanged(object sender, Microsoft.UI.Xaml.Input.PointerRoutedEventArgs e)
        {
            if(sender is Microsoft.UI.Xaml.UIElement)
            {
                var delta = e.GetCurrentPoint((Microsoft.UI.Xaml.UIElement)sender).Properties.MouseWheelDelta;
                if (delta != 0)
                {
                    MainThread.BeginInvokeOnMainThread(() =>
                    {
                        MouseWheel?.Invoke(sender, new GHMouseWheelEventArgs(delta));
                    });
                }
            }
        }

        private void View_PointerExited(object sender, Microsoft.UI.Xaml.Input.PointerRoutedEventArgs e)
        {
            PointerEvent(sender, e, SKTouchAction.Exited);
        }

        private void View_PointerEntered(object sender, Microsoft.UI.Xaml.Input.PointerRoutedEventArgs e)
        {
            PointerEvent(sender, e, SKTouchAction.Entered);
        }

        private void View_PointerMoved(object sender, Microsoft.UI.Xaml.Input.PointerRoutedEventArgs e)
        {
            PointerEvent(sender, e, SKTouchAction.Moved);
        }

        private void View_PointerCanceled(object sender, Microsoft.UI.Xaml.Input.PointerRoutedEventArgs e)
        {
            PointerEvent(sender, e, SKTouchAction.Cancelled);
        }

        private void PointerEvent(object sender, Microsoft.UI.Xaml.Input.PointerRoutedEventArgs e, SKTouchAction action)
        {
            Microsoft.UI.Xaml.UIElement element = sender as Microsoft.UI.Xaml.UIElement;
            if (element != null)
            {
                PointerPoint point = e.GetCurrentPoint(element);
                float canvasWidth = CanvasSize.Width;
                float scale = canvasWidth / Math.Max(1.0f, (float)ThreadSafeWidth);
                SKPoint pointerPosition = point == null ? new SKPoint() : new SKPoint((float)point.Position.X * scale, (float)point.Position.Y * scale);
                SKTouchEventArgs args = new SKTouchEventArgs(-1, action, pointerPosition, false);
                if(MousePointer != null)
                {
                    e.Handled = true;
                    MainThread.BeginInvokeOnMainThread(() =>
                    {
                        MousePointer?.Invoke(sender, args);
                    });
                }
            }
        }
#endif
#endif

    }

}