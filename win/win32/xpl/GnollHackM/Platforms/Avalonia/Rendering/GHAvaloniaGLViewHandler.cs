using Microsoft.Maui;
using SkiaSharp.Views.Maui;
using AcmSKGLViewControl = Avalonia.Controls.Maui.SkiaSharp.Views.Controls.SKGLViewControl;
using AcmSKGLViewHandler = Avalonia.Controls.Maui.SkiaSharp.Views.Handlers.SKGLViewHandler;
using AcmViewHandler = Avalonia.Controls.Maui.Handlers.ViewHandler;

namespace GnollHackM
{
    /* ACM's SKGLViewHandler creating a GHAvaloniaGLControl, whose InvalidateSurface invalidates at once
       on the UI thread. The paint, size, context and touch handling are ACM's. */
    public sealed class GHAvaloniaGLViewHandler : AcmSKGLViewHandler
    {
        /* Chained to ACM's view mappers, not to ACM's SKGLViewHandler mappers: those chain to MAUI's own
           ViewHandler mappers, whose entries do nothing on Avalonia, so sizes, margins, visibility and
           the other view properties would never reach the control */
        public static Microsoft.Maui.PropertyMapper<ISKGLView, AcmSKGLViewHandler> GHMapper =
            new Microsoft.Maui.PropertyMapper<ISKGLView, AcmSKGLViewHandler>(AcmViewHandler.ViewMapper)
            {
                [nameof(ISKGLView.EnableTouchEvents)] = AcmSKGLViewHandler.MapEnableTouchEvents,
                [nameof(ISKGLView.IgnorePixelScaling)] = AcmSKGLViewHandler.MapIgnorePixelScaling,
                [nameof(ISKGLView.HasRenderLoop)] = AcmSKGLViewHandler.MapHasRenderLoop,
                [nameof(IView.HorizontalLayoutAlignment)] = MapLayoutAlignment,
                [nameof(IView.VerticalLayoutAlignment)] = MapLayoutAlignment,
            };

        public static Microsoft.Maui.CommandMapper<ISKGLView, AcmSKGLViewHandler> GHCommandMapper =
            new Microsoft.Maui.CommandMapper<ISKGLView, AcmSKGLViewHandler>(AcmViewHandler.ViewCommandMapper)
            {
                [nameof(ISKGLView.InvalidateSurface)] = MapInvalidateSurface,
            };

        public GHAvaloniaGLViewHandler() : base(GHMapper, GHCommandMapper)
        {
        }

        protected override AcmSKGLViewControl CreatePlatformView()
            => new GHAvaloniaGLControl { ForceSoftwareRendering = ForceSoftwareRendering };

        /* The control stays stretched over the frame MAUI arranges it in. MAUI has already aligned that frame, and
           an aligned Avalonia control takes its own desired size, which is zero for a view MAUI sizes itself */
        public static void MapLayoutAlignment(AcmSKGLViewHandler handler, ISKGLView view)
        {
        }

        public static void MapInvalidateSurface(AcmSKGLViewHandler handler, ISKGLView view, object args)
        {
            GHAvaloniaGLControl control = ((IElementHandler)handler).PlatformView as GHAvaloniaGLControl;
            if (control != null && !control.HasRenderLoop)
                control.RequestRender();
        }
    }
}
