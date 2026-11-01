using SkiaSharp;
using System;
using System.Threading;
using GnollHackX;

#if GNH_MAUI
using SkiaSharp.Views.Maui;
using SkiaSharp.Views.Maui.Controls;

namespace GnollHackM
#else
using Xamarin.Forms;
using SkiaSharp.Views.Forms;

namespace GnollHackX.Controls
#endif
{
    /* A canvas over the game UI that always paints in software on the UI thread and has no resources to shut down */
    public class OverlayCanvasView : SKTouchCanvasView, IThreadSafeView
    {
        /* Names the view in Windows RenderSurfaceDiagnostics */
        public CanvasTypes CanvasType { get; set; }

#pragma warning disable 67
        public event EventHandler<GHMouseWheelEventArgs> MouseWheel;
        public event EventHandler<SKTouchEventArgs> MousePointer;
#pragma warning restore 67

        public OverlayCanvasView() : base()
        {
            ThreadSafeWidth = Width;
            ThreadSafeHeight = Height;
            ThreadSafeX = X;
            ThreadSafeY = Y;
            ThreadSafeIsVisible = IsVisible;
            if (Parent == null || !(Parent is IThreadSafeView))
                ThreadSafeParent = null;
            else
                ThreadSafeParent = new WeakReference<IThreadSafeView>((IThreadSafeView)Parent);
            lock (_propertyLock)
            {
                _threadSafeMargin = Margin;
            }
            SizeChanged += OverlayCanvasView_SizeChanged;
            PropertyChanged += OverlayCanvasView_PropertyChanged;
        }

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
        public WeakReference<IThreadSafeView> ThreadSafeParent { get { return Interlocked.CompareExchange(ref _threadSafeParent, null, null); } private set { Interlocked.Exchange(ref _threadSafeParent, value); } }

        private void OverlayCanvasView_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
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

        private void OverlayCanvasView_SizeChanged(object sender, EventArgs e)
        {
            ThreadSafeWidth = Width;
            ThreadSafeHeight = Height;
        }

#if GNH_MAUI && (WINDOWS || GNH_AVALONIA)
        protected override void OnTouch(SKTouchEventArgs e)
        {
            base.OnTouch(e);
            switch (e.ActionType)
            {
            case SKTouchAction.Entered:
            case SKTouchAction.Exited:
            case SKTouchAction.Moved:
            case SKTouchAction.Cancelled:
                EventHandler<SKTouchEventArgs> mousePointer = MousePointer;
                if (mousePointer != null)
                {
                    e.Handled = true;
                    mousePointer.Invoke(this, new SKTouchEventArgs(-1, e.ActionType, e.Location, false));
                }
                break;
#if GNH_AVALONIA
            /* ACM delivers the mouse wheel as a touch action, in WinUI's MouseWheelDelta units */
            case SKTouchAction.WheelChanged:
                if (e.WheelDelta != 0)
                    MouseWheel?.Invoke(this, new GHMouseWheelEventArgs(e.WheelDelta));
                break;
#endif
            default:
                break;
            }
        }
#endif

#if GNH_MAUI && WINDOWS
        protected override void OnHandlerChanged()
        {
            base.OnHandlerChanged();
            SkiaSharp.Views.Windows.SKXamlCanvas view = Handler?.PlatformView as SkiaSharp.Views.Windows.SKXamlCanvas;
            if (view != null)
                view.PointerWheelChanged += View_PointerWheelChanged;
        }

        protected override void OnHandlerChanging(HandlerChangingEventArgs args)
        {
            base.OnHandlerChanging(args);
            if (args.OldHandler != null)
            {
                SkiaSharp.Views.Windows.SKXamlCanvas view = args.OldHandler.PlatformView as SkiaSharp.Views.Windows.SKXamlCanvas;
                if (view != null)
                    view.PointerWheelChanged -= View_PointerWheelChanged;
            }
        }

        private void View_PointerWheelChanged(object sender, Microsoft.UI.Xaml.Input.PointerRoutedEventArgs e)
        {
            if (sender is Microsoft.UI.Xaml.UIElement)
            {
                int delta = e.GetCurrentPoint((Microsoft.UI.Xaml.UIElement)sender).Properties.MouseWheelDelta;
                if (delta != 0)
                {
                    /* WinUI raises pointer events on the UI thread */
                    MouseWheel?.Invoke(sender, new GHMouseWheelEventArgs(delta));
                }
            }
        }
#endif
    }
}
