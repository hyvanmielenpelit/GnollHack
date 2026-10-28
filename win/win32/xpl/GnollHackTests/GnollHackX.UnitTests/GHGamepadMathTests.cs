using System;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers GHGamepadMath: normalization, the radial deadzone with rescale, the trigger deadzone and
       hysteresis, and the virtual stick directions, with the timing defaults */
    public class GHGamepadMathTests
    {
        private static readonly GHGamepadTiming Timing = GHGamepadTiming.Default;

        [Fact]
        public void Deadzone_InsideIsZero()
        {
            float x, y;
            Assert.Equal(0f, GHGamepadMath.ApplyRadialDeadzone(0.1f, 0.1f, 0.15f, 0.95f, out x, out y));
            Assert.Equal(0f, x);
            Assert.Equal(0f, y);
        }

        [Fact]
        public void Deadzone_ContinuousAtTheInnerEdge()
        {
            float x, y;
            float m = GHGamepadMath.ApplyRadialDeadzone(0.1501f, 0f, 0.15f, 0.95f, out x, out y);
            Assert.True(m > 0f && m < 0.001f, m.ToString());
            Assert.True(x > 0f && x < 0.001f);
        }

        [Fact]
        public void Deadzone_OuterClampAndDirectionPreserved()
        {
            float x, y;
            Assert.Equal(1f, GHGamepadMath.ApplyRadialDeadzone(0f, -1f, 0.15f, 0.95f, out x, out y), 5);
            Assert.Equal(-1f, y, 5);
            Assert.Equal(0f, x, 5);

            float m = GHGamepadMath.ApplyRadialDeadzone(0.3f, 0.4f, 0.15f, 0.95f, out x, out y);
            Assert.Equal((0.5f - 0.15f) / 0.8f, m, 4);
            Assert.Equal(0.75, x / (double)y, 4);
            Assert.Equal(1.0, Math.Sqrt(x * x + y * y) / m, 4);
        }

        [Fact]
        public void NormalizeInt16_IsAsymmetric()
        {
            Assert.Equal(-1f, GHGamepadMath.NormalizeInt16(short.MinValue), 6);
            Assert.Equal(1f, GHGamepadMath.NormalizeInt16(short.MaxValue), 6);
            Assert.Equal(0f, GHGamepadMath.NormalizeInt16(0));
        }

        [Fact]
        public void NormalizeAxis_UsesTheRange()
        {
            Assert.Equal(-1f, GHGamepadMath.NormalizeAxis(-32767, -32767, 32767), 6);
            Assert.Equal(1f, GHGamepadMath.NormalizeAxis(32767, -32767, 32767), 6);
            Assert.Equal(-1f, GHGamepadMath.NormalizeAxis(-40000, -32767, 32767), 6);
            Assert.Equal(0f, GHGamepadMath.NormalizeAxis(127, 0, 254), 6);
            Assert.Equal(0f, GHGamepadMath.NormalizeAxis(5, 3, 3));
        }

        [Fact]
        public void NormalizeTrigger_UsesTheRange()
        {
            Assert.Equal(0f, GHGamepadMath.NormalizeTrigger(0, 0, 255));
            Assert.Equal(1f, GHGamepadMath.NormalizeTrigger(255, 0, 255));
            Assert.Equal(1f, GHGamepadMath.NormalizeTrigger(300, 0, 255));
        }

        [Fact]
        public void TriggerDeadzone_Rescales()
        {
            Assert.Equal(0f, GHGamepadMath.ApplyTriggerDeadzone(0.05f, 0.05f));
            Assert.Equal(1f, GHGamepadMath.ApplyTriggerDeadzone(1f, 0.05f), 6);
            Assert.Equal(0.5f, GHGamepadMath.ApplyTriggerDeadzone(0.525f, 0.05f), 5);
        }

        [Fact]
        public void TriggerHysteresis()
        {
            Assert.False(GHGamepadMath.UpdateTriggerButton(false, 0.54f, Timing));
            Assert.True(GHGamepadMath.UpdateTriggerButton(false, 0.55f, Timing));
            Assert.True(GHGamepadMath.UpdateTriggerButton(true, 0.36f, Timing));
            Assert.False(GHGamepadMath.UpdateTriggerButton(true, 0.34f, Timing));
        }

        [Fact]
        public void Directions_CardinalsWithHysteresis()
        {
            int mask = GHGamepadMath.UpdateDirectionMask(0, 0f, -0.59f, 0.59f, Timing);
            Assert.Equal(0, mask);
            mask = GHGamepadMath.UpdateDirectionMask(0, 0f, -0.6f, 0.6f, Timing);
            Assert.Equal(GHGamepadMath.DirectionUp, mask);
            mask = GHGamepadMath.UpdateDirectionMask(mask, 0f, -0.45f, 0.45f, Timing);
            Assert.Equal(GHGamepadMath.DirectionUp, mask);
            mask = GHGamepadMath.UpdateDirectionMask(mask, 0f, -0.39f, 0.39f, Timing);
            Assert.Equal(0, mask);
            mask = GHGamepadMath.UpdateDirectionMask(0, 1f, 0f, 1f, Timing);
            Assert.Equal(GHGamepadMath.DirectionRight, mask);
            mask = GHGamepadMath.UpdateDirectionMask(0, -1f, 0f, 1f, Timing);
            Assert.Equal(GHGamepadMath.DirectionLeft, mask);
            mask = GHGamepadMath.UpdateDirectionMask(0, 0f, 1f, 1f, Timing);
            Assert.Equal(GHGamepadMath.DirectionDown, mask);
        }

        [Fact]
        public void Directions_DiagonalPressesTwo()
        {
            float c = (float)Math.Sqrt(0.5);
            int mask = GHGamepadMath.UpdateDirectionMask(0, c, -c, 1f, Timing);
            Assert.Equal(GHGamepadMath.DirectionUp | GHGamepadMath.DirectionRight, mask);
        }

        [Fact]
        public void Directions_ShallowAngleOutsideSixtyDegreesPressesOne()
        {
            /* 70 degrees from up: the up component is below half the magnitude */
            double a = 70.0 * Math.PI / 180.0;
            int mask = GHGamepadMath.UpdateDirectionMask(0, (float)Math.Sin(a), -(float)Math.Cos(a), 1f, Timing);
            Assert.Equal(GHGamepadMath.DirectionRight, mask);
        }
    }
}
