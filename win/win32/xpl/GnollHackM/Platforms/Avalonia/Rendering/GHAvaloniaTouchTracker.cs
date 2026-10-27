using System;
using System.Collections.Generic;
using SkiaSharp;
using SkiaSharp.Views.Maui;

namespace GnollHackM
{
    // WORKAROUND(WA-AVALONIA-0019): ACM Skia controls drop touches without Released or Cancelled
    /* The touches an ACM Skia control has raised Pressed for, by pointer. ACM forgets a pointer without
       raising an action when the pointer loses capture or is pressed again; Cancel raises Cancelled
       for it instead. UI thread only. */
    internal sealed class GHAvaloniaTouchTracker
    {
        private struct PressedTouch
        {
            public long Id;
            public SKTouchDeviceType DeviceType;
            public SKPoint Location;
        }

        private readonly Dictionary<Avalonia.Input.IPointer, PressedTouch> _pressed = new Dictionary<Avalonia.Input.IPointer, PressedTouch>();

        public void Track(Avalonia.Input.IPointer pointer, SKTouchEventArgs pressedArgs)
        {
            PressedTouch touch;
            touch.Id = pressedArgs.Id;
            touch.DeviceType = pressedArgs.DeviceType;
            touch.Location = pressedArgs.Location;
            _pressed[pointer] = touch;
        }

        public void Forget(Avalonia.Input.IPointer pointer)
        {
            _pressed.Remove(pointer);
        }

        /* Raises Cancelled, at the press location, for a tracked pointer; a null touchAction only forgets it */
        public void Cancel(Avalonia.Input.IPointer pointer, Func<SKTouchEventArgs, bool> touchAction)
        {
            PressedTouch touch;
            if (!_pressed.Remove(pointer, out touch) || touchAction == null)
                return;

            touchAction(new SKTouchEventArgs(touch.Id, SKTouchAction.Cancelled, SKMouseButton.Unknown, touch.DeviceType, touch.Location, false));
        }
    }
}
