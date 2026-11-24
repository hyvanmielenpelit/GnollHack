using System;

namespace GnollHackX
{
    public enum GHRadialHitZone
    {
        Hub,
        Ring,
        Outside
    }

    /* Radial menu layout in canvas pixels, plus the angle and slot arithmetic shared by the
       selector, the state and the renderer. Angles are compass degrees, clockwise from north;
       vectors and canvas coordinates are +X right, +Y down. Values are recomputed only when the
       canvas size or scale changes. Free of UI types, so that the unit tests compile it. */
    public sealed class GHRadialGeometry
    {
        public float Width { get; private set; }
        public float Height { get; private set; }
        public float DipScale { get; private set; }
        public float CenterX { get; private set; }
        public float CenterY { get; private set; }
        public float OuterRadius { get; private set; }
        public float HubRadius { get; private set; }
        public float InnerRadius { get; private set; }
        public float MidRadius { get; private set; }
        public float SliceGap { get; private set; }
        public float CornerRadius { get; private set; }
        public float LabelFontSize { get; private set; }
        public float LabelMinFontSize { get; private set; }
        public float HubTitleFontSize { get; private set; }
        public float HubDescriptionFontSize { get; private set; }
        public float PageTitleFontSize { get; private set; }
        public float HintFontSize { get; private set; }
        public float HintScale { get; private set; }
        /* The hint bar and the L1/R1 chips: HintFontSize times HintScale */
        public float HintBarFontSize { get; private set; }
        public float PillFontSize { get; private set; }
        public float PageTitleBaselineY { get; private set; }
        public float DotsCenterY { get; private set; }
        public float DotRadius { get; private set; }
        public float DotSpacing { get; private set; }
        public float HintCenterY { get; private set; }
        public float ChipOffsetX { get; private set; }
        public float ChipHitRadius { get; private set; }
        public float HighlightOffset { get; private set; }
        public float GlowBlurRadius { get; private set; }

        /* Advances whenever any value above changes, so that caches keyed on it are rebuilt */
        public int Version { get; private set; }

        public bool IsValid { get { return OuterRadius > 0; } }

        /* Returns true when the layout changed */
        public bool Update(float width, float height, float dipScale)
        {
            return Update(width, height, dipScale, 1f);
        }

        /* hintScale is the Hint Size setting, 1 for 100%. Returns true when the layout changed. */
        public bool Update(float width, float height, float dipScale, float hintScale)
        {
            if (!(dipScale > 0))
                dipScale = 1;
            if (!(hintScale > 0))
                hintScale = 1;
            if (!(width > 0) || !(height > 0))
            {
                width = 0;
                height = 0;
            }
            if (width == Width && height == Height && dipScale == DipScale && hintScale == HintScale && Version != 0)
                return false;

            Width = width;
            Height = height;
            DipScale = dipScale;
            HintScale = hintScale;

            float shortSide = Math.Min(width, height);
            float ro = (float)Math.Min(GHRadialTuning.OuterRadiusOfShortSide * shortSide, GHRadialTuning.OuterRadiusMaxDip * dipScale);
            if (!(ro > 0))
                ro = 0;

            CenterX = width / 2;
            CenterY = height / 2;
            OuterRadius = ro;
            HubRadius = (float)(GHRadialTuning.HubRadius * ro);
            InnerRadius = HubRadius + (float)(GHRadialTuning.HubToRingGap * ro);
            MidRadius = (InnerRadius + ro) / 2;
            SliceGap = (float)(GHRadialTuning.SliceGap * ro);
            CornerRadius = (float)(GHRadialTuning.CornerRadius * ro);
            LabelMinFontSize = (float)(GHRadialTuning.LabelFontMinDip * dipScale);
            LabelFontSize = Math.Max((float)(GHRadialTuning.LabelFont * ro), LabelMinFontSize);
            HubTitleFontSize = (float)(GHRadialTuning.HubTitleFont * ro);
            HubDescriptionFontSize = (float)(GHRadialTuning.HubDescriptionFont * ro);
            PageTitleFontSize = (float)(GHRadialTuning.PageTitleFont * ro);
            HintFontSize = (float)(GHRadialTuning.HintFont * ro);
            HintBarFontSize = HintFontSize * hintScale;
            PillFontSize = (float)(GHRadialTuning.PillFont * ro);
            PageTitleBaselineY = CenterY - ro - (float)(GHRadialTuning.PageTitleOffset * ro);
            DotsCenterY = CenterY - ro - (float)(GHRadialTuning.DotsOffset * ro);
            DotRadius = (float)(GHRadialTuning.DotRadius * ro);
            DotSpacing = (float)(GHRadialTuning.DotSpacing * ro);
            HintCenterY = CenterY + ro + (float)(GHRadialTuning.HintOffset * ro);
            ChipOffsetX = ro + (float)(GHRadialTuning.ChipOffset * ro);
            ChipHitRadius = (float)(GHRadialTuning.ChipHitRadius * ro);
            HighlightOffset = (float)(GHRadialTuning.HighlightOffset * ro);
            GlowBlurRadius = (float)(GHRadialTuning.GlowBlur * ro);
            Version++;
            return true;
        }

        /* Arc length of one slot at the given radius */
        public static float GetArcLength(float radius, int slotCount)
        {
            if (slotCount <= 0)
                return 0;
            return (float)(2.0 * Math.PI * radius / slotCount);
        }

        /* min(0.42 * ring width, 0.55 * the slot's arc at the mid radius) */
        public float GetIconSize(int slotCount)
        {
            float byRing = (float)(GHRadialTuning.IconOfRingWidth * (OuterRadius - InnerRadius));
            float byArc = (float)(GHRadialTuning.IconOfArc * GetArcLength(MidRadius, slotCount));
            return Math.Max(0, Math.Min(byRing, byArc));
        }

        public float GetLabelMaxWidth(int slotCount)
        {
            return (float)(GHRadialTuning.LabelMaxWidthOfArc * GetArcLength(MidRadius, slotCount));
        }

        /* The label width for a horizontal block of blockHeight centered at centerRadius along the slot's
           center direction; see GetLabelMaxWidthAt */
        public float GetLabelMaxWidth(int slotCount, int slot, float centerRadius, float blockHeight)
        {
            double a = SlotCenterDegrees(slot, slotCount) * Math.PI / 180.0;
            return GetLabelMaxWidthAt(slotCount, slot, (float)(Math.Sin(a) * centerRadius), -(float)(Math.Cos(a) * centerRadius), blockHeight);
        }

        /* LabelMaxWidthOfSlice of the widest horizontal block of blockHeight, centered at (offsetX, offsetY) from
           the ring's center, that lies inside the slot's slice: between its two straight edges, SliceGap / 2
           inside the slot's boundary radii, outside the inner arc and inside the outer arc. Wider blocks contain
           narrower ones, so the width is found by bisection. A block that does not fit even at zero width is
           measured on its center line; one whose center is outside the slice gets the arc-based width. */
        public float GetLabelMaxWidthAt(int slotCount, int slot, float offsetX, float offsetY, float blockHeight)
        {
            if (!IsValid || slotCount < 3)
                return GetLabelMaxWidth(slotCount);
            double half = 180.0 / slotCount;
            double c = SlotCenterDegrees(Mod(slot, slotCount), slotCount);
            double a0 = (c - half) * Math.PI / 180.0;
            double a1 = (c + half) * Math.PI / 180.0;
            /* The edges' inward normals: toward the slot's center angle */
            double n0x = Math.Cos(a0);
            double n0y = Math.Sin(a0);
            double n1x = -Math.Cos(a1);
            double n1y = -Math.Sin(a1);
            double halfHeight = Math.Max(0.0, blockHeight / 2.0);
            if (!LabelBlockFits(offsetX, offsetY, 0.0, halfHeight, n0x, n0y, n1x, n1y))
            {
                if (halfHeight > 0.0 && LabelBlockFits(offsetX, offsetY, 0.0, 0.0, n0x, n0y, n1x, n1y))
                    halfHeight = 0.0;
                else
                    return GetLabelMaxWidth(slotCount);
            }
            double lo = 0.0;
            double hi = 2.0 * OuterRadius;
            int i;
            for (i = 0; i < 32; i++)
            {
                double mid = (lo + hi) / 2.0;
                if (LabelBlockFits(offsetX, offsetY, mid / 2.0, halfHeight, n0x, n0y, n1x, n1y))
                    lo = mid;
                else
                    hi = mid;
            }
            return (float)(GHRadialTuning.LabelMaxWidthOfSlice * lo);
        }

        /* The block's rectangle against the two edge half-planes, the outer circle and the inner circle */
        private bool LabelBlockFits(double cx, double cy, double halfWidth, double halfHeight, double n0x, double n0y, double n1x, double n1y)
        {
            double x0 = cx - halfWidth;
            double x1 = cx + halfWidth;
            double y0 = cy - halfHeight;
            double y1 = cy + halfHeight;
            double edge = SliceGap / 2.0;
            /* A linear function's minimum over the rectangle is at a corner */
            if (n0x * (n0x >= 0 ? x0 : x1) + n0y * (n0y >= 0 ? y0 : y1) < edge)
                return false;
            if (n1x * (n1x >= 0 ? x0 : x1) + n1y * (n1y >= 0 ? y0 : y1) < edge)
                return false;
            double fx = Math.Max(Math.Abs(x0), Math.Abs(x1));
            double fy = Math.Max(Math.Abs(y0), Math.Abs(y1));
            if (fx * fx + fy * fy > (double)OuterRadius * OuterRadius)
                return false;
            double nx = ClampDouble(0.0, x0, x1);
            double ny = ClampDouble(0.0, y0, y1);
            return nx * nx + ny * ny >= (double)InnerRadius * InnerRadius;
        }

        /* The canvas point at a compass angle and a radius from the center */
        public void GetPoint(double compassDegrees, float radius, out float x, out float y)
        {
            double a = compassDegrees * Math.PI / 180.0;
            x = CenterX + (float)(Math.Sin(a) * radius);
            y = CenterY - (float)(Math.Cos(a) * radius);
        }

        /* Classifies a canvas point: inside the ring's inner edge is the hub, the ring band
           reaches PointerBandOuter times the outer radius, beyond that is outside */
        public GHRadialHitZone HitTest(float x, float y, out float radius, out double compassDegrees)
        {
            float dx = x - CenterX;
            float dy = y - CenterY;
            radius = (float)Math.Sqrt(dx * dx + dy * dy);
            compassDegrees = CompassAngleDegrees(dx, dy);
            if (!IsValid)
                return GHRadialHitZone.Outside;
            if (radius < InnerRadius)
                return GHRadialHitZone.Hub;
            if (radius <= GHRadialTuning.PointerBandOuter * OuterRadius)
                return GHRadialHitZone.Ring;
            return GHRadialHitZone.Outside;
        }

        public void GetDotCenter(int index, int count, out float x, out float y)
        {
            x = CenterX + (index - (count - 1) / 2.0f) * DotSpacing;
            y = DotsCenterY;
        }

        /* The page dot under a canvas point, or -1. The hit area is half the spacing wide. */
        public int HitTestDot(float x, float y, int count)
        {
            if (count <= 1 || !IsValid)
                return -1;
            float half = DotSpacing / 2;
            if (Math.Abs(y - DotsCenterY) > half)
                return -1;
            int i;
            for (i = 0; i < count; i++)
            {
                float dx, dy;
                GetDotCenter(i, count, out dx, out dy);
                if (Math.Abs(x - dx) <= half)
                    return i;
            }
            return -1;
        }

        /* -1 for the previous-page chip, +1 for the next-page chip, 0 for neither */
        public int HitTestChip(float x, float y)
        {
            if (!IsValid)
                return 0;
            float dy = y - CenterY;
            float dxPrev = x - (CenterX - ChipOffsetX);
            float dxNext = x - (CenterX + ChipOffsetX);
            float r2 = ChipHitRadius * ChipHitRadius;
            if (dxPrev * dxPrev + dy * dy <= r2)
                return -1;
            if (dxNext * dxNext + dy * dy <= r2)
                return 1;
            return 0;
        }

        /* [0, 360) */
        public static double NormalizeDegrees(double degrees)
        {
            if (double.IsNaN(degrees) || double.IsInfinity(degrees))
                return 0;
            double d = degrees % 360.0;
            if (d < 0)
                d += 360.0;
            if (d >= 360.0)
                d -= 360.0;
            return d;
        }

        /* The signed shortest turn from b to a, in (-180, 180] */
        public static double AngleDelta(double a, double b)
        {
            double d = NormalizeDegrees(a - b);
            if (d > 180.0)
                d -= 360.0;
            return d;
        }

        /* Compass angle of a vector with +Y down: up is 0, right is 90, down is 180, left is 270 */
        public static double CompassAngleDegrees(double x, double y)
        {
            if (x == 0 && y == 0)
                return 0;
            return NormalizeDegrees(Math.Atan2(x, -y) * 180.0 / Math.PI);
        }

        /* Slot k of N is centered at k * 360 / N; slot 0 is north */
        public static double SlotCenterDegrees(int slot, int slotCount)
        {
            if (slotCount <= 0)
                return 0;
            return NormalizeDegrees(slot * 360.0 / slotCount);
        }

        /* The slot whose sector contains the angle; a boundary belongs to the clockwise slot */
        public static int SlotAt(double compassDegrees, int slotCount)
        {
            if (slotCount <= 0)
                return -1;
            double step = 360.0 / slotCount;
            int k = (int)Math.Floor((NormalizeDegrees(compassDegrees) + step / 2) / step);
            return Mod(k, slotCount);
        }

        /* The slot nearest the angle. An angle exactly between two slot centers goes to the one
           nearer biasDegrees. */
        public static int NearestSlot(double compassDegrees, int slotCount, double biasDegrees)
        {
            if (slotCount <= 0)
                return -1;
            double step = 360.0 / slotCount;
            double pos = NormalizeDegrees(compassDegrees) / step;
            int lo = (int)Math.Floor(pos);
            int hi = lo + 1;
            double dlo = pos - lo;
            double dhi = hi - pos;
            int res;
            if (Math.Abs(dlo - dhi) < 1e-9)
            {
                double blo = Math.Abs(AngleDelta(lo * step, biasDegrees));
                double bhi = Math.Abs(AngleDelta(hi * step, biasDegrees));
                res = bhi < blo ? hi : lo;
            }
            else
            {
                res = dhi < dlo ? hi : lo;
            }
            return Mod(res, slotCount);
        }

        /* Angular hysteresis: clamp(0.25 * 180 / N, 3, 6) degrees */
        public static double HysteresisDegrees(int slotCount)
        {
            if (slotCount <= 0)
                return 0;
            double h = GHRadialTuning.HysteresisFraction * (180.0 / slotCount);
            return ClampDouble(h, GHRadialTuning.HysteresisMinDegrees, GHRadialTuning.HysteresisMaxDegrees);
        }

        /* Half the angle a gap of the given width subtends at a radius, so that both edges of the
           gap stay parallel instead of converging on the center */
        public static double InsetDegrees(float gap, float radius)
        {
            if (!(radius > 0) || !(gap > 0))
                return 0;
            double s = Math.Min(1.0, (gap / 2.0) / radius);
            return Math.Asin(s) * 180.0 / Math.PI;
        }

        public static int Mod(int a, int n)
        {
            int r = a % n;
            return r < 0 ? r + n : r;
        }

        public static double ClampDouble(double v, double min, double max)
        {
            if (v < min)
                return min;
            if (v > max)
                return max;
            return v;
        }

        public static double Clamp01(double v)
        {
            if (double.IsNaN(v))
                return 0;
            return ClampDouble(v, 0.0, 1.0);
        }
    }
}
