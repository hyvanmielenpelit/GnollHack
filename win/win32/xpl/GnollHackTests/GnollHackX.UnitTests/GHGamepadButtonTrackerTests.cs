using System;
using System.Collections.Generic;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers GHGamepadButtonTracker with 1000 ticks per second: taps, long presses (also retroactive),
       repeats, cancellation, suppression and the R55 silent start of derived buttons */
    public class GHGamepadButtonTrackerTests
    {
        private const long Tps = 1000;

        private static GHGamepadButtonTracker NewTracker()
        {
            return new GHGamepadButtonTracker(GHGamepadTiming.Default, Tps);
        }

        private static string[] Kinds(List<GHGamepadButtonEvent> events)
        {
            string[] kinds = new string[events.Count];
            int i;
            for (i = 0; i < events.Count; i++)
                kinds[i] = events[i].Button + " " + events[i].Kind + (events[i].IsCancelled ? " cancelled" : "");
            return kinds;
        }

        [Fact]
        public void Tap_PressedReleasedShortPressed()
        {
            GHGamepadButtonTracker tracker = NewTracker();
            List<GHGamepadButtonEvent> events = new List<GHGamepadButtonEvent>();
            tracker.Down(GHGamepadButton.A, 1000, events);
            tracker.Update(1100, events);
            tracker.Up(GHGamepadButton.A, 1150, events);

            Assert.Equal(new[] { "A Pressed", "A Released", "A ShortPressed" }, Kinds(events));
            Assert.Equal(events[0].PressId, events[1].PressId);
            Assert.Equal(events[0].PressId, events[2].PressId);
            Assert.Equal(0.15, events[1].HeldSeconds, 6);
        }

        [Fact]
        public void Hold_OneLongPressedAndNoShortPressed()
        {
            GHGamepadButtonTracker tracker = NewTracker();
            List<GHGamepadButtonEvent> events = new List<GHGamepadButtonEvent>();
            tracker.Down(GHGamepadButton.View, 0, events);
            tracker.Update(499, events);
            tracker.Update(500, events);
            tracker.Update(900, events);
            tracker.Up(GHGamepadButton.View, 1000, events);

            Assert.Equal(new[] { "View Pressed", "View LongPressed", "View Released" }, Kinds(events));
            Assert.Equal(500, events[1].Timestamp);
        }

        [Fact]
        public void LateUp_EmitsLongPressedBeforeReleased()
        {
            GHGamepadButtonTracker tracker = NewTracker();
            List<GHGamepadButtonEvent> events = new List<GHGamepadButtonEvent>();
            tracker.Down(GHGamepadButton.B, 0, events);
            tracker.Up(GHGamepadButton.B, 800, events);

            Assert.Equal(new[] { "B Pressed", "B LongPressed", "B Released" }, Kinds(events));
            Assert.Equal(500, events[1].Timestamp);
        }

        [Fact]
        public void Repeat_DPadOverOneSecond()
        {
            GHGamepadButtonTracker tracker = NewTracker();
            List<GHGamepadButtonEvent> events = new List<GHGamepadButtonEvent>();
            tracker.Down(GHGamepadButton.DPadDown, 0, events);
            long t;
            for (t = 10; t <= 1000; t += 10)
                tracker.Update(t, events);

            List<GHGamepadButtonEvent> repeats = events.FindAll(e => e.Kind == GHGamepadButtonEventKind.Repeat);
            /* 0.40 s, then every 0.09 s: 0.40, 0.49, 0.58, 0.67, 0.76, 0.85, 0.94 */
            Assert.Equal(7, repeats.Count);
            Assert.Equal(1, repeats[0].RepeatCount);
            Assert.Equal(400, repeats[0].Timestamp);
            Assert.Equal(7, repeats[6].RepeatCount);
            Assert.Single(events.FindAll(e => e.Kind == GHGamepadButtonEventKind.LongPressed));
        }

        [Fact]
        public void Repeat_OnlyForRepeatableButtons()
        {
            Assert.True(GHGamepadButtonTracker.IsRepeatable(GHGamepadButton.DPadLeft));
            Assert.True(GHGamepadButtonTracker.IsRepeatable(GHGamepadButton.LB));
            Assert.True(GHGamepadButtonTracker.IsRepeatable(GHGamepadButton.RB));
            Assert.True(GHGamepadButtonTracker.IsRepeatable(GHGamepadButton.LeftStickUp));
            Assert.True(GHGamepadButtonTracker.IsRepeatable(GHGamepadButton.RightStickRight));
            Assert.False(GHGamepadButtonTracker.IsRepeatable(GHGamepadButton.A));
            Assert.False(GHGamepadButtonTracker.IsRepeatable(GHGamepadButton.LT));
            Assert.False(GHGamepadButtonTracker.IsRepeatable(GHGamepadButton.L4));

            GHGamepadButtonTracker tracker = NewTracker();
            List<GHGamepadButtonEvent> events = new List<GHGamepadButtonEvent>();
            tracker.Down(GHGamepadButton.A, 0, events);
            tracker.Update(2000, events);
            Assert.DoesNotContain(events, e => e.Kind == GHGamepadButtonEventKind.Repeat);
        }

        [Fact]
        public void Repeat_CatchUpIsOnePerUpdate()
        {
            GHGamepadButtonTracker tracker = NewTracker();
            List<GHGamepadButtonEvent> events = new List<GHGamepadButtonEvent>();
            tracker.Down(GHGamepadButton.RB, 0, events);
            tracker.Update(5000, events);
            Assert.Single(events.FindAll(e => e.Kind == GHGamepadButtonEventKind.Repeat));
            events.Clear();
            tracker.Update(5050, events);
            Assert.Empty(events);
            tracker.Update(5090, events);
            Assert.Single(events);
        }

        [Fact]
        public void CancelAll_CancelledReleasedAndSilentRelease()
        {
            GHGamepadButtonTracker tracker = NewTracker();
            List<GHGamepadButtonEvent> events = new List<GHGamepadButtonEvent>();
            tracker.Down(GHGamepadButton.A, 0, events);
            tracker.Down(GHGamepadButton.LeftStickUp, 0, events);
            events.Clear();

            tracker.CancelAll(100, events);
            Assert.Equal(new[] { "A Released cancelled", "LeftStickUp Released cancelled" }, Kinds(events));
            events.Clear();

            tracker.Update(2000, events);
            tracker.Up(GHGamepadButton.A, 2100, events);
            Assert.Empty(events);
            Assert.False(tracker.IsDown(GHGamepadButton.A));

            tracker.Down(GHGamepadButton.A, 2200, events);
            Assert.Equal(new[] { "A Pressed" }, Kinds(events));
        }

        [Fact]
        public void SecondDownOfASuppressedButton_StartsANewPress()
        {
            GHGamepadButtonTracker tracker = NewTracker();
            List<GHGamepadButtonEvent> events = new List<GHGamepadButtonEvent>();
            tracker.Down(GHGamepadButton.L4, 0, events);
            tracker.CancelAll(10, events);
            events.Clear();

            /* The release was lost while gated; the next press arrives as a down edge */
            tracker.Down(GHGamepadButton.L4, 500, events);
            tracker.Up(GHGamepadButton.L4, 600, events);
            Assert.Equal(new[] { "L4 Pressed", "L4 Released", "L4 ShortPressed" }, Kinds(events));
        }

        [Fact]
        public void DuplicateDown_IsIgnored()
        {
            GHGamepadButtonTracker tracker = NewTracker();
            List<GHGamepadButtonEvent> events = new List<GHGamepadButtonEvent>();
            tracker.Down(GHGamepadButton.X, 0, events);
            tracker.Down(GHGamepadButton.X, 10, events);
            tracker.Up(GHGamepadButton.Y, 20, events);
            Assert.Equal(new[] { "X Pressed" }, Kinds(events));
        }

        [Fact]
        public void SuppressHeld_DerivedButtonPastThresholdStartsSilent()
        {
            GHGamepadButtonTracker tracker = NewTracker();
            List<GHGamepadButtonEvent> events = new List<GHGamepadButtonEvent>();
            tracker.SuppressHeld(GHGamepadButton.RT, 0);
            tracker.Update(3000, events);
            tracker.Up(GHGamepadButton.RT, 3100, events);
            Assert.Empty(events);

            tracker.Down(GHGamepadButton.RT, 3200, events);
            Assert.Equal(new[] { "RT Pressed" }, Kinds(events));
        }

        [Fact]
        public void PressIds_IncreasePerPress()
        {
            GHGamepadButtonTracker tracker = NewTracker();
            List<GHGamepadButtonEvent> events = new List<GHGamepadButtonEvent>();
            tracker.Down(GHGamepadButton.A, 0, events);
            tracker.Up(GHGamepadButton.A, 10, events);
            tracker.Down(GHGamepadButton.A, 20, events);
            Assert.NotEqual(events[0].PressId, events[3].PressId);
            Assert.False(events[3].RevealsUi);
        }
    }
}
