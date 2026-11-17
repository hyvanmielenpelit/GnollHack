using System;

namespace GnollHackX
{
    /* Where an Android gamepad reports its right stick and analog triggers */
    public enum GHAndroidAxisLayout
    {
        Standard,       /* right stick from Z/Rz; triggers max(LTrigger, Brake) and max(RTrigger, Gas) */
        RxRyStick,      /* right stick from Rx/Ry; triggers as Standard */
        LinuxGamepad,   /* right stick from Rx/Ry; left trigger from Z, right trigger from Rz, rescaled to 0..1 */
    }

    /* Android key codes, motion axes and device identities mapped to gamepad terms. No Android references:
       the constants carry Android's values. */
    public static class GHAndroidGamepadMapping
    {
        /* android.view.KeyEvent key codes */
        public const int KeycodeBack = 4;               /* KeyEvent.KEYCODE_BACK */
        public const int KeycodeDpadUp = 19;            /* KeyEvent.KEYCODE_DPAD_UP */
        public const int KeycodeDpadDown = 20;          /* KeyEvent.KEYCODE_DPAD_DOWN */
        public const int KeycodeDpadLeft = 21;          /* KeyEvent.KEYCODE_DPAD_LEFT */
        public const int KeycodeDpadRight = 22;         /* KeyEvent.KEYCODE_DPAD_RIGHT */
        public const int KeycodeMenu = 82;              /* KeyEvent.KEYCODE_MENU */
        public const int KeycodeButtonA = 96;           /* KeyEvent.KEYCODE_BUTTON_A */
        public const int KeycodeButtonB = 97;           /* KeyEvent.KEYCODE_BUTTON_B */
        public const int KeycodeButtonX = 99;           /* KeyEvent.KEYCODE_BUTTON_X */
        public const int KeycodeButtonY = 100;          /* KeyEvent.KEYCODE_BUTTON_Y */
        public const int KeycodeButtonL1 = 102;         /* KeyEvent.KEYCODE_BUTTON_L1 */
        public const int KeycodeButtonR1 = 103;         /* KeyEvent.KEYCODE_BUTTON_R1 */
        public const int KeycodeButtonL2 = 104;         /* KeyEvent.KEYCODE_BUTTON_L2 */
        public const int KeycodeButtonR2 = 105;         /* KeyEvent.KEYCODE_BUTTON_R2 */
        public const int KeycodeButtonThumbL = 106;     /* KeyEvent.KEYCODE_BUTTON_THUMBL */
        public const int KeycodeButtonThumbR = 107;     /* KeyEvent.KEYCODE_BUTTON_THUMBR */
        public const int KeycodeButtonStart = 108;      /* KeyEvent.KEYCODE_BUTTON_START */
        public const int KeycodeButtonSelect = 109;     /* KeyEvent.KEYCODE_BUTTON_SELECT */
        public const int KeycodeButtonMode = 110;       /* KeyEvent.KEYCODE_BUTTON_MODE */

        /* android.view.MotionEvent axes */
        public const int AxisX = 0;                     /* MotionEvent.AXIS_X */
        public const int AxisY = 1;                     /* MotionEvent.AXIS_Y */
        public const int AxisZ = 11;                    /* MotionEvent.AXIS_Z */
        public const int AxisRx = 12;                   /* MotionEvent.AXIS_RX */
        public const int AxisRy = 13;                   /* MotionEvent.AXIS_RY */
        public const int AxisRz = 14;                   /* MotionEvent.AXIS_RZ */
        public const int AxisHatX = 15;                 /* MotionEvent.AXIS_HAT_X */
        public const int AxisHatY = 16;                 /* MotionEvent.AXIS_HAT_Y */
        public const int AxisLTrigger = 17;             /* MotionEvent.AXIS_LTRIGGER */
        public const int AxisRTrigger = 18;             /* MotionEvent.AXIS_RTRIGGER */
        public const int AxisGas = 22;                  /* MotionEvent.AXIS_GAS */
        public const int AxisBrake = 23;                /* MotionEvent.AXIS_BRAKE */

        /* A hat side presses at |v| >= HatPressThreshold and releases below HatReleaseThreshold */
        public const float HatPressThreshold = 0.5f;
        public const float HatReleaseThreshold = 0.3f;

        /* The inner deadzone of the XInput, GameController and evdev backends for physical pads */
        private const float MinimumInnerDeadzone = 0.24f;
        private const float MaximumInnerDeadzone = 0.35f;

        /* KEYCODE_BACK and KEYCODE_MENU are what some controller firmwares send for their two menu buttons.
           KEYCODE_BUTTON_MODE is unmapped: the Guide button stays Android's. */
        public static bool TryMapKey(int keyCode, out GHGamepadButton button)
        {
            switch (keyCode)
            {
            case KeycodeButtonA:
                button = GHGamepadButton.A;
                return true;
            case KeycodeButtonB:
                button = GHGamepadButton.B;
                return true;
            case KeycodeButtonX:
                button = GHGamepadButton.X;
                return true;
            case KeycodeButtonY:
                button = GHGamepadButton.Y;
                return true;
            case KeycodeButtonL1:
                button = GHGamepadButton.LB;
                return true;
            case KeycodeButtonR1:
                button = GHGamepadButton.RB;
                return true;
            case KeycodeButtonL2:
                button = GHGamepadButton.LT;
                return true;
            case KeycodeButtonR2:
                button = GHGamepadButton.RT;
                return true;
            case KeycodeButtonThumbL:
                button = GHGamepadButton.L3;
                return true;
            case KeycodeButtonThumbR:
                button = GHGamepadButton.R3;
                return true;
            case KeycodeButtonStart:
            case KeycodeMenu:
                button = GHGamepadButton.Menu;
                return true;
            case KeycodeButtonSelect:
            case KeycodeBack:
                button = GHGamepadButton.View;
                return true;
            case KeycodeDpadUp:
                button = GHGamepadButton.DPadUp;
                return true;
            case KeycodeDpadDown:
                button = GHGamepadButton.DPadDown;
                return true;
            case KeycodeDpadLeft:
                button = GHGamepadButton.DPadLeft;
                return true;
            case KeycodeDpadRight:
                button = GHGamepadButton.DPadRight;
                return true;
            default:
                button = GHGamepadButton.A;
                return false;
            }
        }

        /* By USB vendor (and product for Backbone), then by name, case-insensitively */
        public static GHGamepadStyle GetStyle(int vendorId, int productId, string name)
        {
            switch (vendorId)
            {
            case 0x045E:
                return GHGamepadStyle.Xbox;
            case 0x054C:
                return GHGamepadStyle.PlayStation;
            case 0x057E:
                return GHGamepadStyle.Nintendo;
            case 0x358A:
                /* Backbone: the PlayStation editions are separate products */
                return productId == 0x0104 || productId == 0x0304 ? GHGamepadStyle.PlayStation : GHGamepadStyle.Xbox;
            }

            if (string.IsNullOrEmpty(name))
                return GHGamepadStyle.Generic;
            if (Contains(name, "xbox"))
                return GHGamepadStyle.Xbox;
            if (Contains(name, "dualsense") || Contains(name, "dualshock") || Contains(name, "playstation"))
                return GHGamepadStyle.PlayStation;
            if (Contains(name, "pro controller") || Contains(name, "joy-con") || Contains(name, "nintendo"))
                return GHGamepadStyle.Nintendo;
            return GHGamepadStyle.Generic;
        }

        /* From the sticks' flat ranges: 1.2 times the larger, at least 0.24 and at most 0.35 */
        public static float GetInnerDeadzone(float leftFlat, float rightFlat)
        {
            float scaled = 1.2f * Math.Max(leftFlat, rightFlat);
            return GHGamepadMath.Clamp(Math.Max(MinimumInnerDeadzone, scaled), MinimumInnerDeadzone, MaximumInnerDeadzone);
        }

        /* hasTriggerAxis: the device has a range for LTrigger, RTrigger, Brake or Gas */
        public static GHAndroidAxisLayout GetAxisLayout(bool hasZ, bool hasRz, bool hasRx, bool hasRy, bool hasTriggerAxis)
        {
            if (hasZ && hasRz && hasRx && hasRy && !hasTriggerAxis)
                return GHAndroidAxisLayout.LinuxGamepad;
            if (hasRx && hasRy && !(hasZ && hasRz))
                return GHAndroidAxisLayout.RxRyStick;
            return GHAndroidAxisLayout.Standard;
        }

        /* A trigger axis to 0..1 over its motion range, whether the range is centered (-1..1) or not; 0 for an
           empty range */
        public static float RescaleTrigger(float value, float min, float max)
        {
            if (max <= min)
                return 0f;
            return GHGamepadMath.Clamp((value - min) / (max - min), 0f, 1f);
        }

        private static bool Contains(string text, string part)
        {
            return text.IndexOf(part, StringComparison.OrdinalIgnoreCase) >= 0;
        }
    }
}
