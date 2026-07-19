using System;
using System.Collections.Generic;
using System.Text;
#if GNH_MAUI
using Microsoft.Maui.Controls;

namespace GnollHackM
#else
using Xamarin.Forms;

namespace GnollHackX
#endif
{
    public static class GHColors
    {
        public static Color MsgBoxPageOverlayColor = Color.FromRgba(0.0, 0.0, 0.0, (double)0x66 / 255.0);
        public static Color MsgBoxDarkModeBkgColor = Color.FromRgba(0.15, 0.15, 0.15, 0.85);
        public static Color MsgBoxLightModeBkgColor = Color.FromRgba(0.0, 0.0, 0.0, 0.75);
        public static Color PickerDarkModeBkgColor = Color.FromRgba(1, 1, 1, 0.15);
        public static Color PickerLightModeBkgColor = Color.FromRgba(0.0, 0.0, 0.0, (double)0x22 / 255.0);
        public static Color MsgFilterFrameBkgColor = Color.FromRgba(0.10, 0.10, 0.10, 0.90);
        public static Color MsgFilterEntryBkgColor = Color.FromRgba(1, 1, 1, 0.05);
        public static Color MoreCommandsFilterEntryBkgColor = Color.FromRgba(1, 1, 1, 0.06);
        
        public static Color LessTransparentBlack = Color.FromRgba(0.0, 0.0, 0.0, (double)0x99 / 255.0);
        public static Color SemiTransparentBlack = Color.FromRgba(0.0, 0.0, 0.0, (double)0x66 / 255.0);
        public static Color VeryTransparentBlack = Color.FromRgba(0.0, 0.0, 0.0, (double)0x1F / 255.0);

        public static Color BrighterRed = Color.FromRgba((double)0xFF / 255.0, 32.0 / 255.0, 32.0 / 255.0, 1.0);
        public static Color BrighterGreen = Color.FromRgba(0.0, (double)0xFF / 255.0, 0.0, 1.0);
        public static Color BrighterBlue = Color.FromRgba(112.0 / 255.0, 112.0 / 255.0, (double)0xFF / 255.0, 1.0);
        public static Color BrighterBrown = Color.FromRgba(210.0 / 255.0, 128.0 / 255.0, 32.0 / 255.0, 1.0);
        public static Color BrightBlue = Color.FromRgba(180.0 / 255.0, 200.0 / 255.0, (double)0xFF / 255.0, 1.0);
        public static Color BrightCyan = Color.FromRgba(165.0 / 255.0, 255.0 / 255.0, (double)0xFF / 255.0, 1.0);
        public static Color TitleGoldColor = Color.FromRgba((double)0xD4 / 255.0, (double)0xA0 / 255.0, (double)0x17 / 255.0, 1.0);
        public static Color SemiDarkGreen = Color.FromRgba(0.0, 0.40, 0.0, 1.0);
        public static Color DarkGreen = Color.FromRgba(0.0, 0.25, 0.0, 1.0);
        public static Color DarkerGray = Color.FromRgba(0.2, 0.2, 0.2, 1.0);
        public static Color LighterGray = Color.FromRgba(0.8, 0.8, 0.8, 1.0);
        public static Color LightYellow = Color.FromRgba(0.9, 0.9, 0.5, 1.0);

#if GNH_MAUI
        public static Color White = Colors.White;
        public static Color Black = Colors.Black;
        public static Color DarkGray = Colors.DarkGray;
        public static Color DarkBlue = Colors.DarkBlue;
        public static Color Red = Colors.Red;
        public static Color Green = Colors.Green;
        public static Color Brown = Colors.Brown;
        public static Color Blue = Colors.Blue;
        public static Color Magenta = Colors.Magenta;
        public static Color Cyan = Colors.Cyan;
        public static Color LightGray = Colors.LightGray;
        public static Color Orange = Colors.Orange;
        public static Color LightGreen = Colors.LightGreen;
        public static Color Yellow = Colors.Yellow;
        public static Color LightBlue = Colors.LightBlue;
        public static Color LightPink = Colors.LightPink;
        public static Color LightCyan = Colors.LightCyan;
        public static Color Beige = Colors.Beige;
        public static Color Gray = Colors.Gray;
        public static Color Transparent = Colors.Transparent;

#else
        public static Color White = Color.White;
        public static Color Black = Color.Black;
        public static Color DarkGray = Color.DarkGray;
        public static Color DarkBlue = Color.DarkBlue;
        public static Color Red = Color.Red;
        public static Color Green = Color.Green;
        public static Color Brown = Color.Brown;
        public static Color Blue = Color.Blue;
        public static Color Magenta = Color.Magenta;
        public static Color Cyan = Color.Cyan;
        public static Color LightGray = Color.LightGray;
        public static Color Orange = Color.Orange;
        public static Color LightGreen = Color.LightGreen;
        public static Color Yellow = Color.Yellow;
        public static Color LightBlue = Color.LightBlue;
        public static Color LightPink = Color.LightPink;
        public static Color LightCyan = Color.LightCyan;
        public static Color Beige = Color.Beige;
        public static Color Gray = Color.Gray;
        public static Color Transparent = Color.Transparent;

#endif
    }
}
