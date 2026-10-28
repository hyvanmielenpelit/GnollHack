using System;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers GHRadialSelector: R1's convention (push up selects north with +Y down), the engage
       and center thresholds with magnitude hysteresis, switching only past half a slot plus the
       angular hysteresis, the highlight staying on release, the release freeze, arming by
       recentering or after 250 ms, empty slots clearing, D-pad stepping and jumps, pointer
       highlighting and page changes. */
    public class GHRadialSelectorTests
    {
        /* Slow enough that no magnitude change trips the release freeze */
        private const double SlowDt = 1.0;
        private const double FrameDt = 1.0 / 60.0;

        private static GHRadialItem[] Slots(int n, params int[] filled)
        {
            GHRadialItem[] slots = new GHRadialItem[n];
            int i;
            if (filled.Length == 0)
            {
                for (i = 0; i < n; i++)
                    slots[i] = new GHRadialItem { Id = "s" + i };
            }
            else
            {
                for (i = 0; i < filled.Length; i++)
                    slots[filled[i]] = new GHRadialItem { Id = "s" + filled[i] };
            }
            return slots;
        }

        private static bool Push(GHRadialSelector s, double angle, double magnitude, double dt, GHRadialItem[] slots)
        {
            double a = angle * Math.PI / 180.0;
            return s.UpdateStick(Math.Sin(a) * magnitude, -Math.Cos(a) * magnitude, dt, slots);
        }

        private static GHRadialSelector Armed()
        {
            GHRadialSelector s = new GHRadialSelector();
            s.Reset(false);
            return s;
        }

        [Theory]
        [InlineData(0.0, -1.0, 0)]
        [InlineData(1.0, 0.0, 2)]
        [InlineData(0.0, 1.0, 4)]
        [InlineData(-1.0, 0.0, 6)]
        [InlineData(-0.7, -0.7, 7)]
        public void StickDirections_FollowYDownConvention(double x, double y, int expected)
        {
            GHRadialSelector s = Armed();
            Assert.True(s.UpdateStick(x, y, FrameDt, Slots(8)));
            Assert.Equal(expected, s.HighlightedSlot);
        }

        [Fact]
        public void Engages_AtThresholdOnly()
        {
            GHRadialItem[] slots = Slots(8);
            GHRadialSelector s = Armed();
            Assert.False(Push(s, 90, 0.5, SlowDt, slots));
            Assert.Equal(-1, s.HighlightedSlot);
            Assert.False(s.IsEngaged);
            Assert.True(Push(s, 90, 0.6, SlowDt, slots));
            Assert.Equal(2, s.HighlightedSlot);
            Assert.True(s.IsEngaged);
        }

        [Fact]
        public void MagnitudeHysteresis_HoldsBetweenThresholds()
        {
            GHRadialItem[] slots = Slots(8);
            GHRadialSelector s = Armed();
            Push(s, 90, 0.9, SlowDt, slots);
            /* Engaged, so 0.4 still selects */
            Assert.True(Push(s, 180, 0.4, SlowDt, slots));
            Assert.Equal(4, s.HighlightedSlot);
            /* Below 0.35 is centered; then 0.5 is not enough to engage again */
            Assert.False(Push(s, 180, 0.3, SlowDt, slots));
            Assert.False(s.IsEngaged);
            Assert.False(Push(s, 270, 0.5, SlowDt, slots));
            Assert.Equal(4, s.HighlightedSlot);
            Assert.True(Push(s, 270, 0.56, SlowDt, slots));
            Assert.Equal(6, s.HighlightedSlot);
        }

        [Fact]
        public void AngularHysteresis_SwitchesOnlyPastTheLimit()
        {
            GHRadialItem[] slots = Slots(8);
            GHRadialSelector s = Armed();
            Push(s, 90, 0.9, SlowDt, slots);
            Assert.Equal(2, s.HighlightedSlot);
            /* Limit for N = 8: 22.5 + 5.625 = 28.125 degrees from the slot center */
            Assert.False(Push(s, 90 + 25.0, 0.9, SlowDt, slots));
            Assert.False(Push(s, 90 + 28.0, 0.9, SlowDt, slots));
            Assert.Equal(2, s.HighlightedSlot);
            Assert.True(Push(s, 90 + 28.3, 0.9, SlowDt, slots));
            Assert.Equal(3, s.HighlightedSlot);
            /* And back: the new slot holds until 135 - 28.125 */
            Assert.False(Push(s, 107.0, 0.9, SlowDt, slots));
            Assert.Equal(3, s.HighlightedSlot);
            Assert.True(Push(s, 106.5, 0.9, SlowDt, slots));
            Assert.Equal(2, s.HighlightedSlot);
        }

        [Fact]
        public void AngularHysteresis_IsNarrowerAtTwelveSlots()
        {
            GHRadialItem[] slots = Slots(12);
            GHRadialSelector s = Armed();
            Push(s, 90, 0.9, SlowDt, slots);
            Assert.Equal(3, s.HighlightedSlot);
            /* 15 + 3.75 */
            Assert.False(Push(s, 90 + 18.5, 0.9, SlowDt, slots));
            Assert.True(Push(s, 90 + 19.0, 0.9, SlowDt, slots));
            Assert.Equal(4, s.HighlightedSlot);
        }

        [Fact]
        public void Highlight_StaysWhenStickIsReleased()
        {
            GHRadialItem[] slots = Slots(8);
            GHRadialSelector s = Armed();
            Push(s, 90, 0.9, SlowDt, slots);
            Assert.False(s.UpdateStick(0, 0, SlowDt, slots));
            Assert.Equal(2, s.HighlightedSlot);
            Assert.False(s.IsEngaged);
        }

        [Fact]
        public void ReleaseFreeze_IgnoresFastFallingMagnitude()
        {
            GHRadialItem[] slots = Slots(8);
            GHRadialSelector s = Armed();
            Push(s, 90, 1.0, SlowDt, slots);
            /* Spring-back overshoot: 1.0 to 0.6 in one frame is -24 per second */
            Assert.False(Push(s, 270, 0.6, FrameDt, slots));
            Assert.Equal(2, s.HighlightedSlot);

            /* The same move made slowly selects */
            GHRadialSelector slow = Armed();
            Push(slow, 90, 1.0, SlowDt, slots);
            Assert.True(Push(slow, 270, 0.6, SlowDt, slots));
            Assert.Equal(6, slow.HighlightedSlot);
        }

        [Fact]
        public void ReleaseFreeze_AllowsSteadyRotation()
        {
            GHRadialItem[] slots = Slots(8);
            GHRadialSelector s = Armed();
            double angle;
            for (angle = 90; angle <= 180; angle += 3)
                Push(s, angle, 0.95, FrameDt, slots);
            Assert.Equal(4, s.HighlightedSlot);
        }

        [Fact]
        public void Arming_WaitsForTheDelayWhileDeflected()
        {
            GHRadialItem[] slots = Slots(8);
            GHRadialSelector s = new GHRadialSelector();
            s.Reset(true);
            Assert.False(s.IsArmed);
            Assert.False(Push(s, 90, 0.9, 0.1, slots));
            Assert.False(Push(s, 90, 0.9, 0.1, slots));
            Assert.Equal(-1, s.HighlightedSlot);
            Assert.True(Push(s, 90, 0.9, 0.1, slots));
            Assert.True(s.IsArmed);
            Assert.Equal(2, s.HighlightedSlot);
        }

        [Fact]
        public void Arming_HappensOnRecentering()
        {
            GHRadialItem[] slots = Slots(8);
            GHRadialSelector s = new GHRadialSelector();
            s.Reset(true);
            Assert.False(Push(s, 90, 0.9, FrameDt, slots));
            Assert.False(s.UpdateStick(0.1, 0, FrameDt, slots));
            Assert.True(s.IsArmed);
            Assert.True(Push(s, 270, 0.9, 0.2, slots));
            Assert.Equal(6, s.HighlightedSlot);
        }

        [Fact]
        public void EmptySlot_ClearsTheHighlight()
        {
            GHRadialItem[] slots = Slots(8, 2);
            GHRadialSelector s = Armed();
            Push(s, 90, 0.9, SlowDt, slots);
            Assert.Equal(2, s.HighlightedSlot);
            Assert.True(Push(s, 270, 0.9, SlowDt, slots));
            Assert.Equal(-1, s.HighlightedSlot);
            Assert.True(Push(s, 90, 0.9, SlowDt, slots));
            Assert.Equal(2, s.HighlightedSlot);
        }

        [Fact]
        public void Step_SkipsEmptiesAndWraps()
        {
            GHRadialItem[] slots = Slots(8, 1, 4, 6);
            GHRadialSelector s = Armed();
            Assert.True(s.Step(1, slots));
            Assert.Equal(1, s.HighlightedSlot);
            s.Step(1, slots);
            Assert.Equal(4, s.HighlightedSlot);
            s.Step(1, slots);
            Assert.Equal(6, s.HighlightedSlot);
            s.Step(1, slots);
            Assert.Equal(1, s.HighlightedSlot);
            s.Step(-1, slots);
            Assert.Equal(6, s.HighlightedSlot);

            GHRadialSelector ccw = Armed();
            Assert.True(ccw.Step(-1, slots));
            Assert.Equal(6, ccw.HighlightedSlot);

            Assert.False(Armed().Step(1, new GHRadialItem[8]));
        }

        [Fact]
        public void Step_WithOneItemDoesNotChange()
        {
            GHRadialItem[] slots = Slots(8, 3);
            GHRadialSelector s = Armed();
            Assert.True(s.Step(1, slots));
            Assert.False(s.Step(1, slots));
            Assert.Equal(3, s.HighlightedSlot);
        }

        [Fact]
        public void Jumps_PickTheFilledSlotNearestNorthOrSouth()
        {
            GHRadialItem[] diagonals = Slots(8, 1, 3, 5, 7);
            GHRadialSelector s = Armed();
            Assert.True(s.JumpToward(0, diagonals));
            Assert.Equal(1, s.HighlightedSlot);
            Assert.True(s.JumpToward(180, diagonals));
            Assert.Equal(3, s.HighlightedSlot);

            GHRadialItem[] cardinals = Slots(8, 0, 4);
            Assert.True(s.JumpToward(0, cardinals));
            Assert.Equal(0, s.HighlightedSlot);
            Assert.False(s.JumpToward(0, cardinals));
            Assert.True(s.JumpToward(180, cardinals));
            Assert.Equal(4, s.HighlightedSlot);

            Assert.False(Armed().JumpToward(0, new GHRadialItem[8]));
        }

        [Fact]
        public void Pointer_HighlightsFilledAndClearsOnEmptyOrHub()
        {
            GHRadialItem[] slots = Slots(8, 2, 5);
            GHRadialSelector s = Armed();
            Assert.True(s.SetFromPointer(5, slots));
            Assert.Equal(5, s.HighlightedSlot);
            Assert.True(s.SetFromPointer(4, slots));
            Assert.Equal(-1, s.HighlightedSlot);
            s.SetFromPointer(2, slots);
            Assert.True(s.SetFromPointer(-1, slots));
            Assert.Equal(-1, s.HighlightedSlot);
        }

        [Fact]
        public void PageChange_ReselectsAtTheStickAngleWhileDeflected()
        {
            GHRadialSelector s = Armed();
            Push(s, 90, 0.9, SlowDt, Slots(8));
            Assert.Equal(2, s.HighlightedSlot);
            Assert.True(s.OnPageChanged(Slots(12), -1));
            Assert.Equal(3, s.HighlightedSlot);
            /* An empty slot at the stick angle clears */
            Assert.True(s.OnPageChanged(Slots(12, 0), 0));
            Assert.Equal(-1, s.HighlightedSlot);
        }

        [Fact]
        public void PageChange_RestoresTheRememberedSlotWhenCentered()
        {
            GHRadialSelector s = Armed();
            Push(s, 90, 0.9, SlowDt, Slots(8));
            s.UpdateStick(0, 0, SlowDt, Slots(8));
            s.OnPageChanged(Slots(12), 7);
            Assert.Equal(7, s.HighlightedSlot);
            s.OnPageChanged(Slots(12), -1);
            Assert.Equal(-1, s.HighlightedSlot);
            /* A remembered slot that is now empty is not restored */
            s.OnPageChanged(Slots(12, 1), 7);
            Assert.Equal(-1, s.HighlightedSlot);
        }

        [Fact]
        public void Reset_ClearsTheHighlight()
        {
            GHRadialSelector s = Armed();
            Push(s, 90, 0.9, SlowDt, Slots(8));
            s.Reset(true);
            Assert.Equal(-1, s.HighlightedSlot);
            Assert.False(s.IsEngaged);
            Assert.False(s.IsArmed);
        }
    }
}
