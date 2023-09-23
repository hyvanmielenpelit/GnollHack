using System;
using System.Collections.Generic;
using System.Text;
#if GNH_MAUI
using Microsoft.Maui.Controls;
#else
using Xamarin.Forms;
#endif

namespace GnollHackX
{
    public static class GHColors
    {
#if GNH_MAUI
        public static Color White = Colors.White;
        public static Color Black = Colors.Black;
        public static Color DarkGray = Colors.DarkGray;
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
        public static Color LessTransparentBlack = new Color(0, 0, 0, (float)0x99 / 255);
        public static Color SemiTransparentBlack = new Color(0, 0, 0, (float)0x66 / 255);
        public static Color VeryTransparentBlack = new Color(0, 0, 0, (float)0x1F / 255);

        public static Color BrighterRed = new Color((float)0xFF / 255, 32.0f / 255, 32.0f / 255);
        public static Color BrighterGreen = new Color(0, (float)0xFF / 255, 0);
        public static Color BrighterBlue = new Color(112.0f / 255, 112.0f / 255, (float)0xFF / 255);
        public static Color BrighterBrown = new Color(210.0f / 255, 128.0f / 255, 32.0f / 255);
        public static Color BrightBlue = new Color(180.0f / 255, 200 / 255, (float)0xFF / 255);
        public static Color BrightCyan = new Color(165.0f / 255, 255 / 255, (float)0xFF / 255);
        public static Color TitleGoldColor = new Color((float)0xD4 / 255, (float)0xA0 / 255, (float)0x17 / 255);
        public static Color DarkGreen = new Color(0, 0.25f, 0);
#else
        public static Color White = Color.White;
        public static Color Black = Color.Black;
        public static Color DarkGray = Color.DarkGray;
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
        public static Color LessTransparentBlack = new Color(0, 0, 0, (double)0x99 / 255.0);
        public static Color SemiTransparentBlack = new Color(0, 0, 0, (double)0x66 / 255.0);
        public static Color VeryTransparentBlack = new Color(0, 0, 0, (double)0x1F / 255.0);

        public static Color BrighterRed = new Color((double)0xFF / 255.0, 32.0 / 255.0, 32.0 / 255.0);
        public static Color BrighterGreen = new Color(0, (double)0xFF / 255, 0);
        public static Color BrighterBlue = new Color(112.0 / 255.0, 112.0 / 255.0, (double)0xFF / 255.0);
        public static Color BrighterBrown = new Color(210.0 / 255.0, 128.0 / 255.0, 32.0 / 255.0);
        public static Color BrightBlue = new Color(180.0 / 255.0, 200.0 / 255.0, (double)0xFF / 255.0);
        public static Color BrightCyan = new Color(165.0 / 255.0, 255.0 / 255.0, (double)0xFF / 255.0);
        public static Color TitleGoldColor = new Color((double)0xD4 / 255, (double)0xA0 / 255, (double)0x17 / 255);
        public static Color DarkGreen = new Color(0, 0.25, 0);
#endif
    }
}
