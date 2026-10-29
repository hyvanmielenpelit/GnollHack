using System;
using System.Collections.Generic;

namespace GnollHackX
{
    /* Turns button edges into Pressed, Released, ShortPressed, LongPressed and Repeat events. LongPressed
       fires once, LongPressSeconds after the press, from Update or retroactively before a late Released.
       Repeat fires for the D-pad, LB/RB and the virtual stick directions only, at most once per Update.
       A suppressed button is held but silent until it is released. Timestamps are ticks of ticksPerSecond. */
    public sealed class GHGamepadButtonTracker
    {
        private struct ButtonState
        {
            public bool Down;
            public bool Suppressed;
            public bool LongFired;
            public long DownTicks;
            public long NextRepeatTicks;
            public int PressId;
            public int RepeatCount;
        }

        private readonly ButtonState[] _states = new ButtonState[(int)GHGamepadButton.Count];
        private readonly GHGamepadTiming _timing;
        private readonly long _ticksPerSecond;
        private int _nextPressId = 1;

        public GHGamepadButtonTracker(GHGamepadTiming timing, long ticksPerSecond)
        {
            _timing = timing ?? GHGamepadTiming.Default;
            _ticksPerSecond = ticksPerSecond > 0 ? ticksPerSecond : 1;
        }

        public static bool IsRepeatable(GHGamepadButton button)
        {
            switch (button)
            {
            case GHGamepadButton.DPadUp:
            case GHGamepadButton.DPadDown:
            case GHGamepadButton.DPadLeft:
            case GHGamepadButton.DPadRight:
            case GHGamepadButton.LB:
            case GHGamepadButton.RB:
                return true;
            default:
                return button >= GHGamepadButton.LeftStickUp && button <= GHGamepadButton.RightStickRight;
            }
        }

        /* Held, whether or not suppressed */
        public bool IsDown(GHGamepadButton button)
        {
            return IsValid(button) && _states[(int)button].Down;
        }

        public bool IsSuppressed(GHGamepadButton button)
        {
            return IsValid(button) && _states[(int)button].Down && _states[(int)button].Suppressed;
        }

        /* A down edge. A second down edge for a suppressed button means its release was missed while silent,
           so it starts a new press; for a button already pressed it is ignored. */
        public void Down(GHGamepadButton button, long timestamp, List<GHGamepadButtonEvent> output)
        {
            if (!IsValid(button))
                return;
            int i = (int)button;
            if (_states[i].Down && !_states[i].Suppressed)
                return;

            _states[i].Down = true;
            _states[i].Suppressed = false;
            _states[i].LongFired = false;
            _states[i].DownTicks = timestamp;
            _states[i].NextRepeatTicks = timestamp + SecondsToTicks(_timing.RepeatDelaySeconds);
            _states[i].RepeatCount = 0;
            _states[i].PressId = _nextPressId++;
            if (_nextPressId == int.MaxValue)
                _nextPressId = 1;
            output.Add(MakeEvent(button, GHGamepadButtonEventKind.Pressed, timestamp, 0.0, 0, false));
        }

        /* An up edge: a LongPressed that is due but has not fired comes first, then Released, then ShortPressed
           when no LongPressed fired */
        public void Up(GHGamepadButton button, long timestamp, List<GHGamepadButtonEvent> output)
        {
            if (!IsValid(button))
                return;
            int i = (int)button;
            if (!_states[i].Down)
                return;

            _states[i].Down = false;
            if (_states[i].Suppressed)
            {
                _states[i].Suppressed = false;
                return;
            }

            long longTicks = _states[i].DownTicks + SecondsToTicks(_timing.LongPressSeconds);
            if (!_states[i].LongFired && timestamp >= longTicks)
            {
                _states[i].LongFired = true;
                output.Add(MakeEvent(button, GHGamepadButtonEventKind.LongPressed, longTicks, _timing.LongPressSeconds, 0, false));
            }
            output.Add(MakeEvent(button, GHGamepadButtonEventKind.Released, timestamp, HeldSeconds(i, timestamp), 0, false));
            if (!_states[i].LongFired)
                output.Add(MakeEvent(button, GHGamepadButtonEventKind.ShortPressed, timestamp, HeldSeconds(i, timestamp), 0, false));
        }

        /* Marks a button held and suppressed without any event: a derived button already past its threshold
           when a controller is added or resynchronized */
        public void SuppressHeld(GHGamepadButton button, long timestamp)
        {
            if (!IsValid(button))
                return;
            int i = (int)button;
            if (_states[i].Down)
                return;
            _states[i].Down = true;
            _states[i].Suppressed = true;
            _states[i].DownTicks = timestamp;
        }

        /* LongPressed and Repeat events that are due, in button order */
        public void Update(long now, List<GHGamepadButtonEvent> output)
        {
            long longTicks = SecondsToTicks(_timing.LongPressSeconds);
            long repeatTicks = Math.Max(1L, SecondsToTicks(_timing.RepeatIntervalSeconds));
            int i;
            for (i = 0; i < _states.Length; i++)
            {
                if (!_states[i].Down || _states[i].Suppressed)
                    continue;
                GHGamepadButton button = (GHGamepadButton)i;

                if (!_states[i].LongFired && now >= _states[i].DownTicks + longTicks)
                {
                    _states[i].LongFired = true;
                    output.Add(MakeEvent(button, GHGamepadButtonEventKind.LongPressed, _states[i].DownTicks + longTicks,
                        _timing.LongPressSeconds, 0, false));
                }

                if (IsRepeatable(button) && now >= _states[i].NextRepeatTicks)
                {
                    _states[i].RepeatCount++;
                    output.Add(MakeEvent(button, GHGamepadButtonEventKind.Repeat, now, HeldSeconds(i, now), _states[i].RepeatCount, false));
                    _states[i].NextRepeatTicks += repeatTicks;
                    if (_states[i].NextRepeatTicks <= now)
                        _states[i].NextRepeatTicks = now + repeatTicks;
                }
            }
        }

        /* Every press in progress gets a cancelled Released and no ShortPressed; everything held stays silent
           until released */
        public void CancelAll(long now, List<GHGamepadButtonEvent> output)
        {
            int i;
            for (i = 0; i < _states.Length; i++)
            {
                if (!_states[i].Down)
                    continue;
                if (!_states[i].Suppressed)
                {
                    output.Add(MakeEvent((GHGamepadButton)i, GHGamepadButtonEventKind.Released, now, HeldSeconds(i, now), 0, true));
                    _states[i].Suppressed = true;
                }
            }
        }

        /* Forgets every held button without events */
        public void Reset()
        {
            Array.Clear(_states, 0, _states.Length);
        }

        private GHGamepadButtonEvent MakeEvent(GHGamepadButton button, GHGamepadButtonEventKind kind, long timestamp,
            double heldSeconds, int repeatCount, bool cancelled)
        {
            GHGamepadButtonEvent e = new GHGamepadButtonEvent();
            e.Button = button;
            e.Kind = kind;
            e.PressId = _states[(int)button].PressId;
            e.Timestamp = timestamp;
            e.HeldSeconds = heldSeconds;
            e.RepeatCount = repeatCount;
            e.IsCancelled = cancelled;
            e.RevealsUi = false;
            return e;
        }

        private double HeldSeconds(int index, long timestamp)
        {
            long held = timestamp - _states[index].DownTicks;
            return held <= 0 ? 0.0 : held / (double)_ticksPerSecond;
        }

        private long SecondsToTicks(double seconds)
        {
            return (long)(seconds * _ticksPerSecond);
        }

        private static bool IsValid(GHGamepadButton button)
        {
            return button >= 0 && button < GHGamepadButton.Count;
        }
    }
}
