using System;

namespace GnollHackX
{
    /* Every radial menu constant: motion, stick selection, slot counts, fixed compass
       directions and geometry factors. Static readonly rather than const, so that a future
       reduce-motion or tuning option can change them at startup. Free of UI types, so that the
       unit tests compile it. */
    public static class GHRadialTuning
    {
        /* Motion, in seconds */
        public static readonly double OpenDuration = 0.140;
        public static readonly double CloseDuration = 0.090;
        public static readonly double ConfirmFlashDuration = 0.070;
        public static readonly double PageChangeDuration = 0.140;
        public static readonly double HighlightEaseDuration = 0.080;
        public static readonly double OpenStartScale = 0.85;
        public static readonly double CloseEndScale = 0.92;
        public static readonly double BackdropMaxOpacity = 0.6;          /* black at 0x99 */
        public static readonly double PageChangeRotationDegrees = 10.0;
        public static readonly double MaxTickStep = 0.25;               /* longer gaps between ticks count as this */
        public static readonly double HighlightSoundMinInterval = 0.045;

        /* Stick selection; magnitudes are 0..1 after the radial deadzone */
        public static readonly double EngageMagnitude = 0.55;
        public static readonly double CenterMagnitude = 0.35;
        public static readonly double ReleaseFreezeRate = 3.0;          /* magnitude per second */
        public static readonly double ArmingDelay = 0.250;
        public static readonly double HysteresisFraction = 0.25;        /* of half a slot */
        public static readonly double HysteresisMinDegrees = 3.0;
        public static readonly double HysteresisMaxDegrees = 6.0;
        public static readonly double StickGhostMinMove = 0.01;         /* smaller stick moves do not repaint */
        public static readonly double StickGhostMinMagnitude = 0.02;

        /* Slot counts */
        public static readonly int ContextSlotCount = 8;
        public static readonly int CrowdedSlotCount = 8;            /* context pages stay at 8 slots and overflow onto further pages */
        public static readonly int MaxSlotCount = 12;
        public static readonly int[] FixedPageSlotCounts = { 6, 8, 12 };
        public static readonly int MorePageSlotCount = 12;
        public static readonly int CommandPageSlotCount = 8;        /* packed commands pages: one item per stick direction */
        public static readonly int MoreColumns = 4;
        public static readonly int MoreRows = 6;
        public static readonly int MoreRowsPerRadialPage = 3;
        public static readonly int SlotMemoryCapacity = 512;

        /* Fixed compass directions in degrees, clockwise from north */
        public static readonly double North = 0.0;
        public static readonly double NorthEast = 45.0;
        public static readonly double East = 90.0;
        public static readonly double SouthEast = 135.0;
        public static readonly double South = 180.0;
        public static readonly double SouthWest = 225.0;
        public static readonly double West = 270.0;
        public static readonly double NorthWest = 315.0;

        /* R3 left-side buttons */
        public static readonly double SkillsAngle = 270.0;
        public static readonly double PolearmAngle = 315.0;
        public static readonly double WieldLastAngle = 225.0;

        /* L3 toggles and the Center on Hero action */
        public static readonly double ExtendedStatusBarAngle = 0.0;
        public static readonly double AutoCenterAngle = 60.0;
        public static readonly double CenterOnHeroAngle = 90.0;
        public static readonly double LookModeAngle = 120.0;
        public static readonly double MoreMessagesAngle = 180.0;
        public static readonly double IgnoreStoppingAngle = 240.0;
        public static readonly double AutoDigAngle = 300.0;

        /* Context items fill these directions first, in this order, then the free slot nearest east */
        public static readonly double[] ContextPreferredAngles = { 90.0, 45.0, 135.0, 0.0, 180.0 };

        /* Geometry. R_o is the outer radius; S is the shorter canvas side; d is canvas pixels per
           device-independent pixel. Factors are of R_o unless named otherwise. */
        public static readonly double OuterRadiusOfShortSide = 0.40;
        public static readonly double OuterRadiusMaxDip = 430.0;
        public static readonly double HubRadius = 0.36;
        public static readonly double HubToRingGap = 0.05;
        public static readonly double SliceGap = 0.012;
        public static readonly double CornerRadius = 0.02;
        public static readonly double IconOfRingWidth = 0.42;
        public static readonly double IconOfArc = 0.55;
        public static readonly double LabelFont = 0.047;
        public static readonly double LabelFontMinDip = 14.0;
        public static readonly double LabelMaxWidthOfArc = 0.92;
        public static readonly double LabelLineHeight = 1.25;           /* of the label font size */
        public static readonly double HubTitleFont = 0.085;
        public static readonly double HubTitleMinOfFont = 0.6;
        public static readonly double HubTitleWidthOfHubDiameter = 0.86;
        public static readonly double HubDescriptionFont = 0.052;
        public static readonly double HubDescriptionWidthOfHubDiameter = 0.8;
        public static readonly int HubDescriptionMaxLines = 3;
        public static readonly double HubBorderDip = 1.5;
        public static readonly double PageTitleFont = 0.07;
        public static readonly double PageTitleOffset = 0.12;           /* title baseline above the ring */
        public static readonly double DotsOffset = 0.045;               /* page dots above the ring */
        public static readonly double DotRadius = 0.0125;
        public static readonly double DotSpacing = 0.06;
        public static readonly double HintFont = 0.05;
        public static readonly double HintOffset = 0.14;                /* hint bar center below the ring */
        public static readonly double ChipOffset = 0.45;                /* L1/R1 chips beyond the ring */
        public static readonly double ChipHitRadius = 0.2;
        public static readonly double HighlightOffset = 0.025;
        public static readonly double HighlightIconScale = 1.12;
        public static readonly double HighlightBorderDip = 2.0;
        public static readonly double GlowBlur = 0.02;
        public static readonly double PointerBandOuter = 1.10;          /* hover counts out to this times R_o */
        public static readonly double StickGhostOfHub = 0.5;
        public static readonly double StickGhostDotOfHub = 0.045;
        public static readonly double WedgeSizeOfHub = 0.11;
        public static readonly double VignetteRadius = 1.35;
        public static readonly double PillFont = 0.036;
        public static readonly double PillInset = 0.085;                /* toggle pill center inside the outer edge */
        public static readonly double ToggleContentInset = 0.03;        /* toggle icon and label move inward by this */
        public static readonly double ArcSegmentOfCorner = 3.0;         /* slice arcs are polylines of segments this many corner radii long */
        public static readonly double RingBoundsOfOuter = 1.2;          /* layer bounds around the ring, glow and highlight offset included */
        public static readonly double ScreenMarginDip = 16.0;
    }
}
