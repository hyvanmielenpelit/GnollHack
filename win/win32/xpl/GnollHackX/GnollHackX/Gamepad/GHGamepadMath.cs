using System;

namespace GnollHackX
{
    /* Stick and trigger math shared by the analog backends and GHGamepadService */
    public static class GHGamepadMath
    {
        /* Bits of a stick's virtual direction mask */
        public const int DirectionUp = 1;
        public const int DirectionDown = 2;
        public const int DirectionLeft = 4;
        public const int DirectionRight = 8;

        /* A raw axis value to -1..1 around the middle of its range */
        public static float NormalizeAxis(int value, int minimum, int maximum)
        {
            if (maximum <= minimum)
                return 0f;
            double center = (minimum + (double)maximum) / 2.0;
            double half = (maximum - (double)minimum) / 2.0;
            return Clamp((float)((value - center) / half), -1f, 1f);
        }

        /* A raw trigger value to 0..1 over its range */
        public static float NormalizeTrigger(int value, int minimum, int maximum)
        {
            if (maximum <= minimum)
                return 0f;
            return Clamp((float)((value - (double)minimum) / (maximum - (double)minimum)), 0f, 1f);
        }

        /* A signed 16-bit axis, as XInput reports it */
        public static float NormalizeInt16(short value)
        {
            return value < 0 ? value / 32768f : value / 32767f;
        }

        /* Radial deadzone with rescale: zero inside inner, continuous at its edge, 1 from outer on, direction
           preserved. Returns the magnitude. */
        public static float ApplyRadialDeadzone(float x, float y, float inner, float outer, out float outX, out float outY)
        {
            double m = Math.Sqrt((double)x * x + (double)y * y);
            if (m <= inner || m <= 0.0)
            {
                outX = 0f;
                outY = 0f;
                return 0f;
            }
            double span = outer - inner;
            double s = span <= 0.0 ? 1.0 : Math.Min(1.0, (m - inner) / span);
            outX = (float)(x / m * s);
            outY = (float)(y / m * s);
            return (float)s;
        }

        /* Zero up to the deadzone, then rescaled to 0..1 */
        public static float ApplyTriggerDeadzone(float value, float deadzone)
        {
            if (value <= deadzone)
                return 0f;
            if (deadzone >= 1f)
                return 1f;
            return Clamp((value - deadzone) / (1f - deadzone), 0f, 1f);
        }

        /* LT/RT from an analog trigger: pressed from TriggerPress, released below TriggerRelease */
        public static bool UpdateTriggerButton(bool wasDown, float value, GHGamepadTiming timing)
        {
            return wasDown ? value >= timing.TriggerRelease : value >= timing.TriggerPress;
        }

        /* One virtual direction: pressed when the component along it is at least DirectionPress and at least
           DirectionShare of the magnitude (a diagonal presses two); released below DirectionRelease */
        public static bool UpdateDirection(bool wasDown, float component, float magnitude, GHGamepadTiming timing)
        {
            if (wasDown)
                return component >= timing.DirectionRelease;
            return component >= timing.DirectionPress && component >= timing.DirectionShare * magnitude;
        }

        /* The four virtual directions of a processed stick vector (+X right, +Y down) */
        public static int UpdateDirectionMask(int previousMask, float x, float y, float magnitude, GHGamepadTiming timing)
        {
            int mask = 0;
            if (UpdateDirection((previousMask & DirectionUp) != 0, -y, magnitude, timing))
                mask |= DirectionUp;
            if (UpdateDirection((previousMask & DirectionDown) != 0, y, magnitude, timing))
                mask |= DirectionDown;
            if (UpdateDirection((previousMask & DirectionLeft) != 0, -x, magnitude, timing))
                mask |= DirectionLeft;
            if (UpdateDirection((previousMask & DirectionRight) != 0, x, magnitude, timing))
                mask |= DirectionRight;
            return mask;
        }

        public static float Clamp(float value, float minimum, float maximum)
        {
            if (value < minimum)
                return minimum;
            if (value > maximum)
                return maximum;
            return value;
        }

        public static double Clamp(double value, double minimum, double maximum)
        {
            if (value < minimum)
                return minimum;
            if (value > maximum)
                return maximum;
            return value;
        }
    }
}
