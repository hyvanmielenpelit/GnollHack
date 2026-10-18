using Microsoft.Maui;
using SkiaSharp.Views.Maui;
using AcmSKCanvasViewControl = Avalonia.Controls.Maui.SkiaSharp.Views.Controls.SKCanvasViewControl;
using AcmSKCanvasViewHandler = Avalonia.Controls.Maui.SkiaSharp.Views.Handlers.SKCanvasViewHandler;
using AcmViewHandler = Avalonia.Controls.Maui.Handlers.ViewHandler;

namespace GnollHackM
{
    /* ACM's SKCanvasViewHandler creating a GHAvaloniaCanvasControl, which paints on the UI thread.
       InvalidateSurface and IgnorePixelScaling request a coalesced paint; the rest is ACM's. */
    public class GHAvaloniaCanvasViewHandler : AcmSKCanvasViewHandler
    {
        /* Chained to ACM's view mappers, not to ACM's SKCanvasViewHandler mappers: those chain to MAUI's own
           ViewHandler mappers, whose entries do nothing on Avalonia, so sizes, margins, visibility and
           the other view properties would never reach the control */
        public static Microsoft.Maui.PropertyMapper<ISKCanvasView, AcmSKCanvasViewHandler> GHMapper =
            new Microsoft.Maui.PropertyMapper<ISKCanvasView, AcmSKCanvasViewHandler>(AcmViewHandler.ViewMapper)
            {
                [nameof(ISKCanvasView.EnableTouchEvents)] = AcmSKCanvasViewHandler.MapEnableTouchEvents,
                [nameof(ISKCanvasView.IgnorePixelScaling)] = MapIgnorePixelScalingAndPaint,
                [nameof(IView.HorizontalLayoutAlignment)] = MapLayoutAlignment,
                [nameof(IView.VerticalLayoutAlignment)] = MapLayoutAlignment,
            };

        public static Microsoft.Maui.CommandMapper<ISKCanvasView, AcmSKCanvasViewHandler> GHCommandMapper =
            new Microsoft.Maui.CommandMapper<ISKCanvasView, AcmSKCanvasViewHandler>(AcmViewHandler.ViewCommandMapper)
            {
                [nameof(ISKCanvasView.InvalidateSurface)] = MapInvalidateSurface,
            };

        public GHAvaloniaCanvasViewHandler() : base(GHMapper, GHCommandMapper)
        {
        }

        protected override AcmSKCanvasViewControl CreatePlatformView() => new GHAvaloniaCanvasControl();

        /* An entry in a derived mapper replaces the chained one, so ACM's mapping is called explicitly */
        public static void MapIgnorePixelScalingAndPaint(AcmSKCanvasViewHandler handler, ISKCanvasView view)
        {
            AcmSKCanvasViewHandler.MapIgnorePixelScaling(handler, view);
            GHAvaloniaCanvasControl control = ((IElementHandler)handler).PlatformView as GHAvaloniaCanvasControl;
            control?.RequestPaint();
        }

        /* The control stays stretched over the frame MAUI arranges it in. MAUI has already aligned that frame, and
           an aligned Avalonia control takes its own desired size, which is zero for a view MAUI sizes itself */
        public static void MapLayoutAlignment(AcmSKCanvasViewHandler handler, ISKCanvasView view)
        {
        }

        public static void MapInvalidateSurface(AcmSKCanvasViewHandler handler, ISKCanvasView view, object args)
        {
            GHAvaloniaCanvasControl control = ((IElementHandler)handler).PlatformView as GHAvaloniaCanvasControl;
            control?.RequestPaint();
        }
    }
}
