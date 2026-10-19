using System;
using System.Diagnostics;
using SkiaSharp;
using SkiaSharp.Views.Maui;
using AvPoint = Avalonia.Point;

namespace GnollHackM
{
    /* Raises the no-contact Entered, Moved and Exited touch actions (mouse hover) that ACM's Skia controls do not */
    internal static class GHAvaloniaPointerHover
    {
        /* Hover moves are limited to 120 per second, as on WinUI */
        public static readonly long HoverMoveIntervalTicks = Stopwatch.Frequency / 120;

        public static bool IsInContact(Avalonia.Input.PointerPointProperties props)
        {
            return props.IsLeftButtonPressed || props.IsMiddleButtonPressed || props.IsRightButtonPressed;
        }

        public static void Raise(Avalonia.Controls.Control control, Avalonia.Input.PointerEventArgs e, SKTouchAction action,
            bool ignorePixelScaling, Func<SKTouchEventArgs, bool> touchAction)
        {
            AvPoint p = e.GetPosition(control);
            double scaling = Avalonia.Controls.TopLevel.GetTopLevel(control)?.RenderScaling ?? 1.0;
            SKPoint location = ignorePixelScaling
                ? new SKPoint((float)p.X, (float)p.Y)
                : new SKPoint((float)(p.X * scaling), (float)(p.Y * scaling));
            SKTouchEventArgs args = new SKTouchEventArgs(e.Pointer.Id, action, SKMouseButton.Unknown, SKTouchDeviceType.Mouse, location, false);
            if (touchAction(args) || args.Handled)
                e.Handled = true;
        }
    }
}
