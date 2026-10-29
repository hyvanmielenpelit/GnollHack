using System;

namespace GnollHackX
{
    /* Picks the highlighted slot of a radial page from a stick, the D-pad or a pointer.
       - The stick engages at EngageMagnitude and counts as centered below CenterMagnitude;
         between the two the previous state holds.
       - The highlighted slot stays until the stick points past its edge by the angular
         hysteresis, and stays when the stick is released.
       - Pointing into an empty slot clears the highlight.
       - A magnitude falling faster than ReleaseFreezeRate changes nothing, so the spring-back of
         a released stick cannot select the opposite slot.
       - After Reset with arming, the stick is ignored until it has been centered once or
         ArmingDelay has passed, so drift from clicking the stick to open selects nothing.
       Stick vectors are +X right, +Y down. Free of UI types, so that the unit tests compile it. */
    public sealed class GHRadialSelector
    {
        private int _slot = -1;
        private bool _engaged;
        private bool _armed = true;
        private double _sinceReset;
        private double _lastMagnitude;
        private double _lastAngle;

        public int HighlightedSlot { get { return _slot; } }
        public bool IsEngaged { get { return _engaged; } }
        public bool IsArmed { get { return _armed; } }
        public double LastMagnitude { get { return _lastMagnitude; } }
        public double LastAngle { get { return _lastAngle; } }

        public void Reset(bool requireArming)
        {
            _slot = -1;
            _engaged = false;
            _armed = !requireArming;
            _sinceReset = 0;
            _lastMagnitude = 0;
            _lastAngle = 0;
        }

        /* Returns true when the highlight changed */
        public bool SetHighlight(int slot)
        {
            if (slot < -1)
                slot = -1;
            if (slot == _slot)
                return false;
            _slot = slot;
            return true;
        }

        /* Returns true when the highlight changed */
        public bool UpdateStick(double x, double y, double dtSeconds, GHRadialItem[] slots)
        {
            if (double.IsNaN(x) || double.IsInfinity(x))
                x = 0;
            if (double.IsNaN(y) || double.IsInfinity(y))
                y = 0;
            if (double.IsNaN(dtSeconds) || dtSeconds < 0)
                dtSeconds = 0;

            double mag = Math.Sqrt(x * x + y * y);
            if (mag > 1.0)
                mag = 1.0;
            double rate = dtSeconds > 0 ? (mag - _lastMagnitude) / dtSeconds : 0;
            _lastMagnitude = mag;
            if (mag > 0)
                _lastAngle = GHRadialGeometry.CompassAngleDegrees(x, y);

            if (!_armed)
            {
                _sinceReset += dtSeconds;
                if (mag < GHRadialTuning.CenterMagnitude || _sinceReset >= GHRadialTuning.ArmingDelay)
                    _armed = true;
                else
                    return false;
            }

            if (_engaged)
            {
                if (mag < GHRadialTuning.CenterMagnitude)
                    _engaged = false;
            }
            else if (mag >= GHRadialTuning.EngageMagnitude)
            {
                _engaged = true;
            }

            if (!_engaged || rate < -GHRadialTuning.ReleaseFreezeRate)
                return false;

            return SelectAtAngle(_lastAngle, slots, true);
        }

        /* Highlights the slot at the angle, or clears the highlight on an empty slot. With
           hysteresis, the current slot stays while the angle is within half a slot plus the
           hysteresis of its center. Returns true when the highlight changed. */
        public bool SelectAtAngle(double compassDegrees, GHRadialItem[] slots, bool useHysteresis)
        {
            int n = slots == null ? 0 : slots.Length;
            if (n == 0)
                return SetHighlight(-1);

            if (useHysteresis && _slot >= 0 && _slot < n && slots[_slot] != null)
            {
                double limit = 180.0 / n + GHRadialGeometry.HysteresisDegrees(n);
                double delta = Math.Abs(GHRadialGeometry.AngleDelta(compassDegrees, GHRadialGeometry.SlotCenterDegrees(_slot, n)));
                if (delta <= limit)
                    return false;
            }

            int candidate = GHRadialGeometry.SlotAt(compassDegrees, n);
            return SetHighlight(slots[candidate] != null ? candidate : -1);
        }

        /* The next filled slot clockwise (direction 1) or counterclockwise (-1), wrapping. With
           no highlight, the first filled slot from north in that direction, north included.
           Returns true when the highlight changed. */
        public bool Step(int direction, GHRadialItem[] slots)
        {
            int n = slots == null ? 0 : slots.Length;
            if (n == 0)
                return false;
            direction = direction < 0 ? -1 : 1;
            int i;
            if (_slot < 0 || _slot >= n)
            {
                for (i = 0; i < n; i++)
                {
                    int k = GHRadialGeometry.Mod(direction * i, n);
                    if (slots[k] != null)
                        return SetHighlight(k);
                }
                return false;
            }
            for (i = 1; i <= n; i++)
            {
                int k = GHRadialGeometry.Mod(_slot + direction * i, n);
                if (slots[k] != null)
                    return SetHighlight(k);
            }
            return false;
        }

        /* The filled slot nearest the compass angle; ties go to the lower slot index. Returns
           true when the highlight changed. */
        public bool JumpToward(double compassDegrees, GHRadialItem[] slots)
        {
            int k = NearestFilledSlot(compassDegrees, slots);
            if (k < 0)
                return false;
            return SetHighlight(k);
        }

        public static int NearestFilledSlot(double compassDegrees, GHRadialItem[] slots)
        {
            int n = slots == null ? 0 : slots.Length;
            int best = -1;
            double bestDelta = double.MaxValue;
            int k;
            for (k = 0; k < n; k++)
            {
                if (slots[k] == null)
                    continue;
                double d = Math.Abs(GHRadialGeometry.AngleDelta(GHRadialGeometry.SlotCenterDegrees(k, n), compassDegrees));
                if (d < bestDelta - 1e-9)
                {
                    bestDelta = d;
                    best = k;
                }
            }
            return best;
        }

        /* A pointer over a slot highlights it if filled and clears the highlight if empty; -1
           (over the hub) clears it. Returns true when the highlight changed. */
        public bool SetFromPointer(int slot, GHRadialItem[] slots)
        {
            int n = slots == null ? 0 : slots.Length;
            if (slot < 0 || slot >= n || slots[slot] == null)
                return SetHighlight(-1);
            return SetHighlight(slot);
        }

        /* After a page change: a deflected stick selects at its angle on the new page, else the
           page's remembered slot returns, or nothing. Returns true when the highlight changed. */
        public bool OnPageChanged(GHRadialItem[] slots, int rememberedSlot)
        {
            if (_armed && _engaged)
                return SelectAtAngle(_lastAngle, slots, false);
            int n = slots == null ? 0 : slots.Length;
            if (rememberedSlot >= 0 && rememberedSlot < n && slots[rememberedSlot] != null)
                return SetHighlight(rememberedSlot);
            return SetHighlight(-1);
        }
    }
}
