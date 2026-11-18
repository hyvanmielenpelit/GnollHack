using System;

namespace GnollHackX
{
    /* The profile class of a GameController extended gamepad */
    public enum GHAppleGamepadProfile
    {
        Extended,
        Xbox,
        DualShock,
        DualSense
    }

    /* Glyph style and stick deadzone of a controller read through Apple's GameController framework */
    public static class GHAppleGamepadMapping
    {
        public const float InnerDeadzone = 0.24f;

        /* The profile class decides first, then the product category (GCController.productCategory), then the
           vendor name */
        public static GHGamepadStyle GetStyle(GHAppleGamepadProfile profile, string productCategory, string vendorName)
        {
            switch (profile)
            {
            case GHAppleGamepadProfile.Xbox:
                return GHGamepadStyle.Xbox;
            case GHAppleGamepadProfile.DualShock:
            case GHAppleGamepadProfile.DualSense:
                return GHGamepadStyle.PlayStation;
            }

            GHGamepadStyle style = GetStyleFromText(productCategory);
            return style != GHGamepadStyle.Generic ? style : GetStyleFromText(vendorName);
        }

        private static GHGamepadStyle GetStyleFromText(string text)
        {
            if (string.IsNullOrEmpty(text))
                return GHGamepadStyle.Generic;
            if (Contains(text, "Xbox"))
                return GHGamepadStyle.Xbox;
            if (Contains(text, "DualShock") || Contains(text, "DualSense") || Contains(text, "PlayStation"))
                return GHGamepadStyle.PlayStation;
            if (Contains(text, "Switch") || Contains(text, "Joy-Con") || Contains(text, "Nintendo"))
                return GHGamepadStyle.Nintendo;
            return GHGamepadStyle.Generic;
        }

        private static bool Contains(string text, string part)
        {
            return text.IndexOf(part, StringComparison.OrdinalIgnoreCase) >= 0;
        }
    }
}
