using System;

namespace GnollHackX
{
    /* Text labels for the buttons in each controller style. Generic uses the Xbox labels. */
    public static class GHGamepadGlyphs
    {
        public static string GetLabel(GHGamepadButton button, GHGamepadStyle style)
        {
            switch (button)
            {
            case GHGamepadButton.A:
                return style == GHGamepadStyle.PlayStation ? "Cross" : style == GHGamepadStyle.Nintendo ? "B" : "A";
            case GHGamepadButton.B:
                return style == GHGamepadStyle.PlayStation ? "Circle" : style == GHGamepadStyle.Nintendo ? "A" : "B";
            case GHGamepadButton.X:
                return style == GHGamepadStyle.PlayStation ? "Square" : style == GHGamepadStyle.Nintendo ? "Y" : "X";
            case GHGamepadButton.Y:
                return style == GHGamepadStyle.PlayStation ? "Triangle" : style == GHGamepadStyle.Nintendo ? "X" : "Y";
            case GHGamepadButton.LB:
                return IsXboxLike(style) ? "LB" : style == GHGamepadStyle.Nintendo ? "L" : "L1";
            case GHGamepadButton.RB:
                return IsXboxLike(style) ? "RB" : style == GHGamepadStyle.Nintendo ? "R" : "R1";
            case GHGamepadButton.LT:
                return IsXboxLike(style) ? "LT" : style == GHGamepadStyle.Nintendo ? "ZL" : "L2";
            case GHGamepadButton.RT:
                return IsXboxLike(style) ? "RT" : style == GHGamepadStyle.Nintendo ? "ZR" : "R2";
            case GHGamepadButton.View:
                return style == GHGamepadStyle.PlayStation ? "Create" : style == GHGamepadStyle.Nintendo ? "Minus" : "View";
            case GHGamepadButton.Menu:
                return style == GHGamepadStyle.PlayStation ? "Options" : style == GHGamepadStyle.Nintendo ? "Plus" : "Menu";
            case GHGamepadButton.L3:
                return IsXboxLike(style) ? "LS" : "L3";
            case GHGamepadButton.R3:
                return IsXboxLike(style) ? "RS" : "R3";
            case GHGamepadButton.DPadUp:
                return "D-pad Up";
            case GHGamepadButton.DPadDown:
                return "D-pad Down";
            case GHGamepadButton.DPadLeft:
                return "D-pad Left";
            case GHGamepadButton.DPadRight:
                return "D-pad Right";
            case GHGamepadButton.L4:
                return IsXboxLike(style) ? "P3" : "L4";
            case GHGamepadButton.L5:
                return IsXboxLike(style) ? "P4" : "L5";
            case GHGamepadButton.R4:
                return IsXboxLike(style) ? "P1" : "R4";
            case GHGamepadButton.R5:
                return IsXboxLike(style) ? "P2" : "R5";
            case GHGamepadButton.LeftStickUp:
                return GetStickLabel(0, style) + " Up";
            case GHGamepadButton.LeftStickDown:
                return GetStickLabel(0, style) + " Down";
            case GHGamepadButton.LeftStickLeft:
                return GetStickLabel(0, style) + " Left";
            case GHGamepadButton.LeftStickRight:
                return GetStickLabel(0, style) + " Right";
            case GHGamepadButton.RightStickUp:
                return GetStickLabel(1, style) + " Up";
            case GHGamepadButton.RightStickDown:
                return GetStickLabel(1, style) + " Down";
            case GHGamepadButton.RightStickLeft:
                return GetStickLabel(1, style) + " Left";
            case GHGamepadButton.RightStickRight:
                return GetStickLabel(1, style) + " Right";
            default:
                return button.ToString();
            }
        }

        /* 0 left, 1 right */
        public static string GetStickLabel(int stick, GHGamepadStyle style)
        {
            return stick == 0 ? "L-stick" : "R-stick";
        }

        private static bool IsXboxLike(GHGamepadStyle style)
        {
            return style == GHGamepadStyle.Xbox || style == GHGamepadStyle.Generic;
        }
    }
}
