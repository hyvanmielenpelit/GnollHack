using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using CoreGraphics;
using Foundation;
using GnollHackX;
using Microsoft.Maui.Platform;
using UIKit;

namespace GnollHackM.Platforms.iOS
{
    /// <summary>
    /// Temporary diagnostic for the iOS 27 SettingsPage switch investigation. Traces touch sequences that
    /// begin inside a UISwitch or a MAUI scroll view: the hit view, the gesture recognizers that receive the
    /// touch and their state changes, scroll view cancellation decisions and the switch's control events.
    /// Writes one block per sequence to the GnollHack log, at most MaxSequences per launch. UI thread only.
    /// </summary>
    public static class TouchTrace
    {
        private const int MaxSequences = 60;
        private static readonly double[] _moveThresholds = { 1.0, 3.0, 6.0, 10.0, 20.0 };

        private static int _sequenceCount;
        private static bool _active;
        private static double _startTimestamp;
        private static CGPoint _startLocation;
        private static double _maxDistance;
        private static int _nextThresholdIndex;
        private static UISwitch _switch;
        private static readonly StringBuilder _sb = new StringBuilder();
        private static readonly List<UIGestureRecognizer> _recognizers = new List<UIGestureRecognizer>();
        private static readonly List<UIGestureRecognizerState> _recognizerStates = new List<UIGestureRecognizerState>();
        private static readonly HashSet<IntPtr> _targetedRecognizers = new HashSet<IntPtr>();
        private static readonly HashSet<IntPtr> _targetedSwitches = new HashSet<IntPtr>();
        private static readonly HashSet<string> _dumpedPages = new HashSet<string>();

        public static void BeforeEvent(UIEvent evt)
        {
            try
            {
                if (evt == null || evt.Type != UIEventType.Touches)
                    return;
                if (!_active && _sequenceCount >= MaxSequences)
                    return;

                UITouch[] touches = GetTouches(evt);
                foreach (UITouch touch in touches)
                {
                    switch (touch.Phase)
                    {
                        case UITouchPhase.Began:
                            if (_active)
                                Log(touch.Timestamp, "another touch began on " + Describe(touch.View));
                            else
                                StartSequence(touch);
                            break;
                        case UITouchPhase.Moved:
                            if (_active)
                                LogMovement(touch);
                            break;
                        case UITouchPhase.Ended:
                        case UITouchPhase.Cancelled:
                            if (_active)
                            {
                                LogMovement(touch);
                                Log(touch.Timestamp, "touch " + touch.Phase + ", duration "
                                    + ElapsedMs(touch.Timestamp) + " ms, max distance "
                                    + _maxDistance.ToString("0.0", CultureInfo.InvariantCulture) + " pt");
                            }
                            break;
                        default:
                            break;
                    }
                }
            }
            catch (Exception ex)
            {
                Fail(ex);
            }
        }

        public static void AfterEvent(UIEvent evt)
        {
            try
            {
                if (!_active || evt == null || evt.Type != UIEventType.Touches)
                    return;

                LogStateChanges(NSProcessInfo.ProcessInfo.SystemUptime);

                UITouch[] touches = GetTouches(evt);
                bool allDone = touches.Length > 0;
                foreach (UITouch touch in touches)
                {
                    if (touch.Phase != UITouchPhase.Ended && touch.Phase != UITouchPhase.Cancelled)
                    {
                        allDone = false;
                        break;
                    }
                }

                if (allDone)
                    EndSequence();
            }
            catch (Exception ex)
            {
                Fail(ex);
            }
        }

        public static void ScrollCallback(string name, UIView view, bool result)
        {
            try
            {
                if (_active)
                    Log(NSProcessInfo.ProcessInfo.SystemUptime, "scroll " + name + "(" + Describe(view) + ") -> " + result);
            }
            catch (Exception ex)
            {
                Fail(ex);
            }
        }

        private static void StartSequence(UITouch touch)
        {
            UIView hitView = touch.View;
            UISwitch uiSwitch = FindAncestor<UISwitch>(hitView);
            MauiScrollView scrollView = FindAncestor<MauiScrollView>(hitView);
            if (uiSwitch == null && scrollView == null)
                return;

            _active = true;
            _sequenceCount++;
            _sb.Clear();
            _recognizers.Clear();
            _recognizerStates.Clear();
            _startTimestamp = touch.Timestamp;
            _startLocation = touch.LocationInView(null);
            _maxDistance = 0;
            _nextThresholdIndex = 0;
            _switch = uiSwitch;

            string pageName = GetPageName();
            _sb.Append("TouchTrace #").Append(_sequenceCount)
                .Append(" (iOS ").Append(UIDevice.CurrentDevice.SystemVersion)
                .Append(", page ").Append(pageName).Append(')').AppendLine();
            Log(touch.Timestamp, "touch Began at " + FormatPoint(_startLocation) + " on " + Describe(hitView));
            _sb.Append("  hit chain: ").Append(DescribeHitChain(hitView)).AppendLine();
            if (uiSwitch != null)
                _sb.Append("  switch ").Append(Describe(uiSwitch)).Append(" On=").Append(uiSwitch.On).AppendLine();

            UIGestureRecognizer[] recognizers = touch.GestureRecognizers ?? Array.Empty<UIGestureRecognizer>();
            _sb.Append("  recognizers receiving the touch: ").Append(recognizers.Length).AppendLine();
            foreach (UIGestureRecognizer recognizer in recognizers)
            {
                if (recognizer == null)
                    continue;
                _recognizers.Add(recognizer);
                _recognizerStates.Add(recognizer.State);
                _sb.Append("    ").Append(DescribeRecognizer(recognizer)).AppendLine();
                AddRecognizerTarget(recognizer);
            }

            if (uiSwitch != null)
            {
                AddSwitchTargets(uiSwitch);
                if (_dumpedPages.Add(pageName))
                    AppendViewTreeDump(uiSwitch);
            }
        }

        private static void EndSequence()
        {
            _sb.Append("  final recognizer states:").AppendLine();
            for (int i = 0; i < _recognizers.Count; i++)
            {
                UIGestureRecognizer recognizer = _recognizers[i];
                _sb.Append("    ").Append(recognizer.Class.Name).Append(" ").Append(recognizer.State).AppendLine();
            }

            int sequence = _sequenceCount;
            UISwitch uiSwitch = _switch;
            if (uiSwitch != null)
                _sb.Append("  switch On=").Append(uiSwitch.On).Append(" at end").AppendLine();

            string text = _sb.ToString();
            _active = false;
            _sb.Clear();
            _recognizers.Clear();
            _recognizerStates.Clear();
            _switch = null;

            GHApp.WriteGHLog(text);

            if (uiSwitch != null)
            {
                NSTimer.CreateScheduledTimer(0.5, false, (timer) =>
                {
                    try
                    {
                        GHApp.WriteGHLog("TouchTrace #" + sequence + " +500 ms after end: switch On=" + uiSwitch.On);
                    }
                    catch (Exception ex)
                    {
                        GHApp.MaybeWriteGHLog("TouchTrace: " + ex.Message);
                    }
                });
            }
        }

        private static void AddRecognizerTarget(UIGestureRecognizer recognizer)
        {
            IntPtr handle = recognizer.Handle;
            if (!_targetedRecognizers.Add(handle))
                return;

            WeakReference<UIGestureRecognizer> weakRecognizer = new WeakReference<UIGestureRecognizer>(recognizer);
            recognizer.AddTarget(() =>
            {
                try
                {
                    if (!_active || !weakRecognizer.TryGetTarget(out UIGestureRecognizer target))
                        return;
                    int index = _recognizers.IndexOf(target);
                    if (index >= 0)
                    {
                        if (_recognizerStates[index] == target.State && target.State == UIGestureRecognizerState.Changed)
                            return;
                        _recognizerStates[index] = target.State;
                    }
                    Log(NSProcessInfo.ProcessInfo.SystemUptime, "action " + target.Class.Name + " -> " + target.State);
                }
                catch (Exception ex)
                {
                    Fail(ex);
                }
            });
        }

        private static void AddSwitchTargets(UISwitch uiSwitch)
        {
            if (!_targetedSwitches.Add(uiSwitch.Handle))
                return;

            AddSwitchTarget(uiSwitch, UIControlEvent.TouchDown, "TouchDown");
            AddSwitchTarget(uiSwitch, UIControlEvent.TouchUpInside, "TouchUpInside");
            AddSwitchTarget(uiSwitch, UIControlEvent.TouchUpOutside, "TouchUpOutside");
            AddSwitchTarget(uiSwitch, UIControlEvent.TouchCancel, "TouchCancel");
            AddSwitchTarget(uiSwitch, UIControlEvent.ValueChanged, "ValueChanged");
            AddSwitchTarget(uiSwitch, UIControlEvent.PrimaryActionTriggered, "PrimaryActionTriggered");
        }

        private static void AddSwitchTarget(UISwitch uiSwitch, UIControlEvent controlEvent, string eventName)
        {
            uiSwitch.AddTarget((sender, e) =>
            {
                try
                {
                    UISwitch source = sender as UISwitch;
                    string line = "switch " + eventName + " On=" + (source?.On.ToString() ?? "?");
                    if (_active)
                        Log(NSProcessInfo.ProcessInfo.SystemUptime, line);
                    else
                        GHApp.WriteGHLog("TouchTrace (outside a sequence): " + line + " on " + Describe(source));
                }
                catch (Exception ex)
                {
                    Fail(ex);
                }
            }, controlEvent);
        }

        private static void LogStateChanges(double timestamp)
        {
            for (int i = 0; i < _recognizers.Count; i++)
            {
                UIGestureRecognizerState state = _recognizers[i].State;
                if (state != _recognizerStates[i])
                {
                    _recognizerStates[i] = state;
                    Log(timestamp, "state " + _recognizers[i].Class.Name + " -> " + state);
                }
            }
        }

        private static void LogMovement(UITouch touch)
        {
            CGPoint location = touch.LocationInView(null);
            double dx = location.X - _startLocation.X;
            double dy = location.Y - _startLocation.Y;
            double distance = Math.Sqrt(dx * dx + dy * dy);
            if (distance > _maxDistance)
                _maxDistance = distance;

            while (_nextThresholdIndex < _moveThresholds.Length && distance >= _moveThresholds[_nextThresholdIndex])
            {
                Log(touch.Timestamp, "moved " + distance.ToString("0.0", CultureInfo.InvariantCulture) + " pt");
                _nextThresholdIndex++;
            }
        }

        private static void AppendViewTreeDump(UISwitch uiSwitch)
        {
            _sb.Append("  view tree of the switch:").AppendLine();
            AppendSubtree(uiSwitch, 2);
            _sb.Append("  ancestors of the switch:").AppendLine();
            int depth = 2;
            for (UIView view = uiSwitch.Superview; view != null; view = view.Superview)
            {
                AppendViewLine(view, depth);
            }
        }

        private static void AppendSubtree(UIView view, int depth)
        {
            AppendViewLine(view, depth);
            UIView[] subviews = view.Subviews ?? Array.Empty<UIView>();
            foreach (UIView subview in subviews)
            {
                AppendSubtree(subview, depth + 1);
            }
        }

        private static void AppendViewLine(UIView view, int depth)
        {
            string indent = new string(' ', depth * 2);
            _sb.Append(indent).Append(Describe(view))
                .Append(" frame=").Append(FormatRect(view.Frame))
                .Append(" interaction=").Append(view.UserInteractionEnabled);
            IUIInteraction[] interactions = view.Interactions;
            if (interactions != null && interactions.Length > 0)
            {
                _sb.Append(" interactions=[");
                for (int i = 0; i < interactions.Length; i++)
                {
                    if (i > 0)
                        _sb.Append(", ");
                    _sb.Append(interactions[i] is NSObject interactionObject ? interactionObject.Class.Name : interactions[i]?.GetType().Name);
                }
                _sb.Append(']');
            }
            _sb.AppendLine();

            UIGestureRecognizer[] recognizers = view.GestureRecognizers;
            if (recognizers != null)
            {
                foreach (UIGestureRecognizer recognizer in recognizers)
                {
                    _sb.Append(indent).Append("  * ").Append(DescribeRecognizer(recognizer)).AppendLine();
                }
            }
        }

        private static string DescribeRecognizer(UIGestureRecognizer recognizer)
        {
            StringBuilder sb = new StringBuilder();
            sb.Append(recognizer.Class.Name)
                .Append('@').Append(Describe(recognizer.View))
                .Append(' ').Append(recognizer.State)
                .Append(" enabled=").Append(recognizer.Enabled)
                .Append(" cancels=").Append(recognizer.CancelsTouchesInView)
                .Append(" delaysBegan=").Append(recognizer.DelaysTouchesBegan)
                .Append(" delaysEnded=").Append(recognizer.DelaysTouchesEnded);
            if (recognizer is UITapGestureRecognizer tap)
                sb.Append(" taps=").Append(tap.NumberOfTapsRequired);
            if (recognizer is UILongPressGestureRecognizer longPress)
                sb.Append(" minPress=").Append(longPress.MinimumPressDuration.ToString("0.###", CultureInfo.InvariantCulture))
                    .Append(" allowMove=").Append(longPress.AllowableMovement.ToString("0.#", CultureInfo.InvariantCulture));
            NSObject recognizerDelegate = recognizer.WeakDelegate;
            sb.Append(" delegate=").Append(recognizerDelegate != null ? recognizerDelegate.Class.Name : "none");
            return sb.ToString();
        }

        private static string DescribeHitChain(UIView hitView)
        {
            StringBuilder sb = new StringBuilder();
            for (UIView view = hitView; view != null; view = view.Superview)
            {
                if (sb.Length > 0)
                    sb.Append(" < ");
                sb.Append(Describe(view));
                if (view is MauiScrollView)
                    break;
            }
            return sb.ToString();
        }

        private static string Describe(UIView view)
        {
            if (view == null)
                return "null";
            long handle = ((IntPtr)view.Handle).ToInt64();
            return view.Class.Name + "#" + (handle & 0xFFFFFF).ToString("x6", CultureInfo.InvariantCulture);
        }

        private static T FindAncestor<T>(UIView view) where T : UIView
        {
            for (UIView current = view; current != null; current = current.Superview)
            {
                if (current is T match)
                    return match;
            }
            return null;
        }

        private static UITouch[] GetTouches(UIEvent evt)
        {
            NSSet allTouches = evt.AllTouches;
            if (allTouches == null)
                return Array.Empty<UITouch>();
            return allTouches.ToArray<UITouch>();
        }

        private static string GetPageName()
        {
            try
            {
                Microsoft.Maui.Controls.Page topPage = GHApp.PageFromTopOfModalNavigationStack();
                return topPage != null ? topPage.GetType().Name : "MainPage";
            }
            catch (Exception)
            {
                return "?";
            }
        }

        private static void Log(double timestamp, string text)
        {
            _sb.Append("  +").Append(ElapsedMs(timestamp)).Append(" ms ").Append(text).AppendLine();
        }

        private static int ElapsedMs(double timestamp)
        {
            return (int)Math.Round((timestamp - _startTimestamp) * 1000.0);
        }

        private static string FormatPoint(CGPoint point)
        {
            return "(" + point.X.ToString("0.#", CultureInfo.InvariantCulture) + ", " + point.Y.ToString("0.#", CultureInfo.InvariantCulture) + ")";
        }

        private static string FormatRect(CGRect rect)
        {
            return "(" + rect.X.ToString("0.#", CultureInfo.InvariantCulture) + ", " + rect.Y.ToString("0.#", CultureInfo.InvariantCulture)
                + ", " + rect.Width.ToString("0.#", CultureInfo.InvariantCulture) + "x" + rect.Height.ToString("0.#", CultureInfo.InvariantCulture) + ")";
        }

        private static void Fail(Exception ex)
        {
            _active = false;
            _sb.Clear();
            _recognizers.Clear();
            _recognizerStates.Clear();
            _switch = null;
            GHApp.MaybeWriteGHLog("TouchTrace: " + ex.GetType().Name + ": " + ex.Message);
        }
    }
}
