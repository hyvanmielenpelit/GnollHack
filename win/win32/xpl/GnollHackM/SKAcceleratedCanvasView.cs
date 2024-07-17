using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SkiaSharp;
using SkiaSharp.Views.Maui;

namespace GnollHackM
{
    public class SKAcceleratedCanvasView : View, ISKGLView
    {
        public static readonly BindableProperty IgnorePixelScalingProperty =
            BindableProperty.Create(nameof(IgnorePixelScaling), typeof(bool), typeof(SKAcceleratedCanvasView), false);

        public static readonly BindableProperty HasRenderLoopProperty =
            BindableProperty.Create(nameof(HasRenderLoop), typeof(bool), typeof(SKAcceleratedCanvasView), false);

        public static readonly BindableProperty EnableTouchEventsProperty =
            BindableProperty.Create(nameof(EnableTouchEvents), typeof(bool), typeof(SKAcceleratedCanvasView), false);

        private SKSizeI lastCanvasSize;
        private GRContext? lastGRContext;

        public bool IgnorePixelScaling
        {
            get => (bool)GetValue(IgnorePixelScalingProperty);
            set => SetValue(IgnorePixelScalingProperty, value);
        }

        public bool HasRenderLoop
        {
            get => (bool)GetValue(HasRenderLoopProperty);
            set => SetValue(HasRenderLoopProperty, value);
        }

        public bool EnableTouchEvents
        {
            get => (bool)GetValue(EnableTouchEventsProperty);
            set => SetValue(EnableTouchEventsProperty, value);
        }

        public event EventHandler<SKPaintGLSurfaceEventArgs>? PaintSurface;

        public event EventHandler<SKTouchEventArgs>? Touch;

        public SKSize CanvasSize => lastCanvasSize;

        public GRContext? GRContext => lastGRContext;

        public void InvalidateSurface()
        {
            Handler?.Invoke(nameof(ISKGLView.InvalidateSurface));
        }

        protected virtual void OnPaintSurface(SKPaintGLSurfaceEventArgs e)
        {
            PaintSurface?.Invoke(this, e);
        }

        protected virtual void OnTouch(SKTouchEventArgs e)
        {
            Touch?.Invoke(this, e);
        }

        void ISKGLView.OnCanvasSizeChanged(SKSizeI size) =>
            lastCanvasSize = size;

        void ISKGLView.OnGRContextChanged(GRContext? context) =>
            lastGRContext = context;

        void ISKGLView.OnPaintSurface(SKPaintGLSurfaceEventArgs e) =>
            OnPaintSurface(e);

        void ISKGLView.OnTouch(SKTouchEventArgs e) =>
            OnTouch(e);
    }
}
