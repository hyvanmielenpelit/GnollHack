#if GNH_AVALONIA
using System;
using System.Diagnostics;
using System.Globalization;
using System.Text;
using Avalonia.Input;
using Avalonia.Interactivity;
using Avalonia.VisualTree;
using GnollHackX;

namespace GnollHackM
{
    /* Diagnostic pointer log, on when GNOLLHACK_POINTER_LOG=1 at startup: every press and release twice, as the
       window's tunnel handler first sees it and as it leaves the bubble phase, every tap, and at most one line a
       second counting pointer moves, to standard error and the app log. A tunnel line has the elements hit at the
       point, innermost first, and the element that held the pointer capture before the event. Controls that forward
       input elsewhere add their own lines through Log. Off, every call costs one bool check. UI thread only, except
       Log and Num, which keep no state. */
    internal static class GHAvaloniaPointerLog
    {
        private const int HitChainLength = 6;

        private static readonly bool _enabled = string.Equals(Environment.GetEnvironmentVariable("GNOLLHACK_POINTER_LOG"), "1", StringComparison.Ordinal);
        private static Avalonia.Controls.Window _window = null;
        private static int _moveCount = 0;
        private static long _moveWindowStart = 0;
        private static string _lastMove = "-";

        public static bool IsEnabled { get { return _enabled; } }

        public static void Attach(Avalonia.Controls.Window window)
        {
            if (!_enabled || window == null)
                return;
            _window = window;
            window.AddHandler(InputElement.PointerPressedEvent, OnPressedTunnel, RoutingStrategies.Tunnel, true);
            window.AddHandler(InputElement.PointerReleasedEvent, OnReleasedTunnel, RoutingStrategies.Tunnel, true);
            window.AddHandler(InputElement.PointerPressedEvent, OnPressedBubble, RoutingStrategies.Bubble, true);
            window.AddHandler(InputElement.PointerReleasedEvent, OnReleasedBubble, RoutingStrategies.Bubble, true);
            window.AddHandler(InputElement.PointerMovedEvent, OnMoved, RoutingStrategies.Tunnel, true);
            window.AddHandler(InputElement.TappedEvent, OnTapped, RoutingStrategies.Bubble, true);
            Log("on");
        }

        public static void Detach(Avalonia.Controls.Window window)
        {
            if (!_enabled || window == null)
                return;
            window.RemoveHandler(InputElement.PointerPressedEvent, OnPressedTunnel);
            window.RemoveHandler(InputElement.PointerReleasedEvent, OnReleasedTunnel);
            window.RemoveHandler(InputElement.PointerPressedEvent, OnPressedBubble);
            window.RemoveHandler(InputElement.PointerReleasedEvent, OnReleasedBubble);
            window.RemoveHandler(InputElement.PointerMovedEvent, OnMoved);
            window.RemoveHandler(InputElement.TappedEvent, OnTapped);
            if (ReferenceEquals(_window, window))
                _window = null;
        }

        /* One line with the log's prefix and time */
        public static void Log(string text)
        {
            if (!_enabled)
                return;
            Write("POINTERLOG t=" + Milliseconds(Stopwatch.GetTimestamp()) + " " + text);
        }

        public static string Num(double value)
        {
            return value.ToString("F1", CultureInfo.InvariantCulture);
        }

        private static void OnPressedTunnel(object sender, PointerPressedEventArgs e)
        {
            LogPointer("Pressed tunnel", e, "clicks=" + e.ClickCount.ToString(CultureInfo.InvariantCulture), true);
        }

        private static void OnReleasedTunnel(object sender, PointerReleasedEventArgs e)
        {
            LogPointer("Released tunnel", e, "button=" + e.InitialPressMouseButton.ToString(), true);
        }

        private static void OnPressedBubble(object sender, PointerPressedEventArgs e)
        {
            LogPointer("Pressed bubble", e, null, false);
        }

        private static void OnReleasedBubble(object sender, PointerReleasedEventArgs e)
        {
            LogPointer("Released bubble", e, null, false);
        }

        private static void LogPointer(string what, Avalonia.Input.PointerEventArgs e, string extra, bool withHits)
        {
            try
            {
                if (_window == null || e == null)
                    return;
                PointerPoint point = e.GetCurrentPoint(_window);
                StringBuilder sb = new StringBuilder(240);
                sb.Append(what);
                sb.Append(" type=").Append(e.Pointer.Type.ToString());
                sb.Append(" id=").Append(e.Pointer.Id.ToString(CultureInfo.InvariantCulture));
                sb.Append(" kind=").Append(point.Properties.PointerUpdateKind.ToString());
                sb.Append(" pos=").Append(Num(point.Position.X)).Append(',').Append(Num(point.Position.Y));
                if (extra != null)
                    sb.Append(' ').Append(extra);
                sb.Append(" handled=").Append(e.Handled ? "True" : "False");
                sb.Append(" src=").Append(Describe(e.Source));
                sb.Append(" captured=").Append(Describe(e.Pointer.Captured));
                if (withHits)
                    sb.Append(" hit=").Append(HitChain(point.Position));
                Log(sb.ToString());
            }
            catch (Exception ex)
            {
                Log(what + " failed: " + ex.Message);
            }
        }

        private static void OnMoved(object sender, Avalonia.Input.PointerEventArgs e)
        {
            try
            {
                if (_window == null || e == null)
                    return;
                long now = Stopwatch.GetTimestamp();
                Avalonia.Point position = e.GetPosition(_window);
                _moveCount++;
                _lastMove = Num(position.X) + "," + Num(position.Y) + " type=" + e.Pointer.Type.ToString();
                if (_moveWindowStart == 0)
                {
                    _moveWindowStart = now;
                }
                else if (now - _moveWindowStart >= Stopwatch.Frequency)
                {
                    Log("Moves n=" + _moveCount.ToString(CultureInfo.InvariantCulture) + " last=" + _lastMove
                        + " captured=" + Describe(e.Pointer.Captured));
                    _moveCount = 0;
                    _moveWindowStart = now;
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
        }

        private static void OnTapped(object sender, Avalonia.Input.TappedEventArgs e)
        {
            if (e == null)
                return;
            Log("Tapped handled=" + (e.Handled ? "True" : "False") + " src=" + Describe(e.Source));
        }

        /* The element hit at position and its visual ancestors, innermost first */
        private static string HitChain(Avalonia.Point position)
        {
            IInputElement hit = _window.InputHitTest(position);
            if (hit == null)
                return "none";
            StringBuilder sb = new StringBuilder(200);
            Avalonia.Visual visual = hit as Avalonia.Visual;
            int i;
            for (i = 0; visual != null && i < HitChainLength; i++)
            {
                if (i > 0)
                    sb.Append('>');
                sb.Append(Describe(visual));
                visual = visual.GetVisualParent();
            }
            return sb.ToString();
        }

        /* Type#Name[hit,enabled,visible] */
        private static string Describe(object element)
        {
            if (element == null)
                return "-";
            StringBuilder sb = new StringBuilder(64);
            sb.Append(element.GetType().Name);
            Avalonia.StyledElement styled = element as Avalonia.StyledElement;
            if (styled != null && !string.IsNullOrEmpty(styled.Name))
                sb.Append('#').Append(styled.Name);
            InputElement input = element as InputElement;
            if (input != null)
            {
                sb.Append("[hit=").Append(input.IsHitTestVisible ? '1' : '0');
                sb.Append(",en=").Append(input.IsEffectivelyEnabled ? '1' : '0');
                sb.Append(",vis=").Append(input.IsEffectivelyVisible ? '1' : '0').Append(']');
            }
            return sb.ToString();
        }

        private static string Milliseconds(long ticks)
        {
            return (ticks * 1000.0 / Stopwatch.Frequency).ToString("F1", CultureInfo.InvariantCulture);
        }

        private static void Write(string line)
        {
            try
            {
                Console.Error.WriteLine(line);
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
            GHApp.WriteGHLog(line);
        }
    }
}
#endif
