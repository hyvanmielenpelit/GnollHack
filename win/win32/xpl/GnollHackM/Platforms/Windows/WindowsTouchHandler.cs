#if WINDOWS
#nullable enable

using System;
using System.Collections.Generic;
using System.Diagnostics;
using Microsoft.UI.Input;
using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Input;
using SkiaSharp;
using SkiaSharp.Views.Maui;

namespace GnollHackM
{
    /* Adapted from SkiaSharp's internal Windows SKTouchHandler (MIT). Every WinRT object read
       here is a CsWinRT wrapper that adds GC memory pressure, so each event reads only what
       it reports: one PointerPoint, Properties only when a button can be involved, and
       Pointer only to capture it and when the capture ends. Hover moves are limited before
       anything is read.
       Wheel events are not handled; SwitchableCanvasView raises MouseWheel itself. */
    internal sealed class WindowsTouchHandler
    {
        private static readonly long s_hoverMoveIntervalTicks = (long)(Stopwatch.Frequency / 120.0);

        private Action<SKTouchEventArgs>? _onTouchAction;
        private Func<double, double, SKPoint>? _scalePixels;
        /* Ids of the pointers in contact over this element; hover moves are limited while it is
           empty. Every event that reports a pointer corrects its entry, so a missed release
           does not leave one behind. */
        private readonly List<uint> _pressedPointers = new List<uint>(4);
        private long _lastHoverTimestamp;

        public WindowsTouchHandler(Action<SKTouchEventArgs> onTouchAction, Func<double, double, SKPoint> scalePixels)
        {
            _onTouchAction = onTouchAction;
            _scalePixels = scalePixels;
        }

        public void SetEnabled(FrameworkElement view, bool enableTouchEvents)
        {
            if (view == null)
                return;

            view.PointerEntered -= OnPointerEntered;
            view.PointerExited -= OnPointerExited;
            view.PointerPressed -= OnPointerPressed;
            view.PointerMoved -= OnPointerMoved;
            view.PointerReleased -= OnPointerReleased;
            view.PointerCanceled -= OnPointerCanceled;
            view.PointerCaptureLost -= OnPointerCaptureLost;
            _pressedPointers.Clear();
            if (enableTouchEvents)
            {
                view.PointerEntered += OnPointerEntered;
                view.PointerExited += OnPointerExited;
                view.PointerPressed += OnPointerPressed;
                view.PointerMoved += OnPointerMoved;
                view.PointerReleased += OnPointerReleased;
                view.PointerCanceled += OnPointerCanceled;
                view.PointerCaptureLost += OnPointerCaptureLost;
            }
        }

        public void Detach(FrameworkElement view)
        {
            SetEnabled(view, false);
            _onTouchAction = null;
            _scalePixels = null;
        }

        private void OnPointerEntered(object sender, PointerRoutedEventArgs args)
        {
            _lastHoverTimestamp = Stopwatch.GetTimestamp();
            args.Handled = CommonHandler(sender, SKTouchAction.Entered, args);
        }

        private void OnPointerExited(object sender, PointerRoutedEventArgs args)
        {
            _lastHoverTimestamp = Stopwatch.GetTimestamp();
            args.Handled = CommonHandler(sender, SKTouchAction.Exited, args);
        }

        private void OnPointerPressed(object sender, PointerRoutedEventArgs args)
        {
            args.Handled = CommonHandler(sender, SKTouchAction.Pressed, args);
            if (args.Handled && sender is FrameworkElement element)
            {
                element.ManipulationMode = ManipulationModes.All;
                element.CapturePointer(args.Pointer);
            }
        }

        private void OnPointerMoved(object sender, PointerRoutedEventArgs args)
        {
            if (_pressedPointers.Count == 0)
            {
                long now = Stopwatch.GetTimestamp();
                if (now - _lastHoverTimestamp < s_hoverMoveIntervalTicks)
                {
                    args.Handled = true;
                    return;
                }
                _lastHoverTimestamp = now;
            }
            args.Handled = CommonHandler(sender, SKTouchAction.Moved, args);
        }

        private void OnPointerReleased(object sender, PointerRoutedEventArgs args)
        {
            args.Handled = CommonHandler(sender, SKTouchAction.Released, args);
            if (sender is FrameworkElement element)
                element.ManipulationMode = ManipulationModes.System;
        }

        private void OnPointerCanceled(object sender, PointerRoutedEventArgs args)
        {
            _lastHoverTimestamp = Stopwatch.GetTimestamp();
            args.Handled = CommonHandler(sender, SKTouchAction.Cancelled, args);
        }

        /* Also raised after an ordinary release, when the id is already gone */
        private void OnPointerCaptureLost(object sender, PointerRoutedEventArgs args)
        {
            _pressedPointers.Remove(args.Pointer.PointerId);
        }

        private bool CommonHandler(object sender, SKTouchAction action, PointerRoutedEventArgs evt)
        {
            var onTouchAction = _onTouchAction;
            var scalePixels = _scalePixels;
            if (onTouchAction == null || scalePixels == null)
                return false;

            PointerPoint point = evt.GetCurrentPoint(sender as UIElement);
            if (point == null)
                return false;

            var position = point.Position;
            uint pointerId = point.PointerId;
            bool inContact = point.IsInContact;
            TrackPointer(action, pointerId, inContact);
            SKMouseButton button = SKMouseButton.Unknown;
            if (inContact || action == SKTouchAction.Pressed || action == SKTouchAction.Released || action == SKTouchAction.Cancelled)
                button = GetMouseButton(point.Properties);

            var e = new SKTouchEventArgs(pointerId, action, button, GetTouchDevice(point.PointerDeviceType),
                scalePixels(position.X, position.Y), inContact, 0);
            onTouchAction(e);
            return e.Handled;
        }

        /* A press, or an enter or move in contact, shows the pointer pressed; every other
           event shows it not pressed */
        private void TrackPointer(SKTouchAction action, uint pointerId, bool inContact)
        {
            bool pressed;
            switch (action)
            {
            case SKTouchAction.Pressed:
                pressed = true;
                break;
            case SKTouchAction.Entered:
            case SKTouchAction.Moved:
                pressed = inContact;
                break;
            default:
                pressed = false;
                break;
            }
            if (!pressed)
                _pressedPointers.Remove(pointerId);
            else if (!_pressedPointers.Contains(pointerId))
                _pressedPointers.Add(pointerId);
        }

        private static SKMouseButton GetMouseButton(PointerPointProperties? properties)
        {
            if (properties == null)
                return SKMouseButton.Unknown;

            SKMouseButton result = SKMouseButton.Unknown;
            if (properties.IsLeftButtonPressed)
                result = SKMouseButton.Left;
            else if (properties.IsMiddleButtonPressed)
                result = SKMouseButton.Middle;
            else if (properties.IsRightButtonPressed)
                result = SKMouseButton.Right;

            switch (properties.PointerUpdateKind)
            {
            case PointerUpdateKind.LeftButtonPressed:
            case PointerUpdateKind.LeftButtonReleased:
                result = SKMouseButton.Left;
                break;
            case PointerUpdateKind.RightButtonPressed:
            case PointerUpdateKind.RightButtonReleased:
                result = SKMouseButton.Right;
                break;
            case PointerUpdateKind.MiddleButtonPressed:
            case PointerUpdateKind.MiddleButtonReleased:
                result = SKMouseButton.Middle;
                break;
            }
            return result;
        }

        private static SKTouchDeviceType GetTouchDevice(PointerDeviceType deviceType)
        {
            switch (deviceType)
            {
            case PointerDeviceType.Pen:
                return SKTouchDeviceType.Pen;
            case PointerDeviceType.Mouse:
                return SKTouchDeviceType.Mouse;
            default:
                return SKTouchDeviceType.Touch;
            }
        }
    }
}
#endif
