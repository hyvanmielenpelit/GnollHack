using System;
using System.Collections.Generic;
using System.Text;
using SkiaSharp;
using Xamarin.Forms;
using GnollHackCommon;

namespace GnollHackClient
{
    public enum canvas_page_types
    {
        MainGamePage = 0,
    }


    class ClientUtils
    {
        private static SKColor GHDarkGray = new SKColor(96, 96, 96);

        public static SKColor NHColor2SKColor(nhcolor nhcolor)
        {
            SKColor res = SKColors.White;
            switch (nhcolor)
            {
                case nhcolor.CLR_BLACK:
                    res = GHDarkGray;
                    break;
                case nhcolor.CLR_RED:
                    res = SKColors.Red;
                    break;
                case nhcolor.CLR_GREEN:
                    res = SKColors.Green;
                    break;
                case nhcolor.CLR_BROWN:
                    res = SKColors.Brown;
                    break;
                case nhcolor.CLR_BLUE:
                    res = SKColors.Blue;
                    break;
                case nhcolor.CLR_MAGENTA:
                    res = SKColors.Magenta;
                    break;
                case nhcolor.CLR_CYAN:
                    res = SKColors.Cyan;
                    break;
                case nhcolor.CLR_GRAY:
                    res = SKColors.LightGray;
                    break;
                case nhcolor.NO_COLOR:
                    break;
                case nhcolor.CLR_ORANGE:
                    res = SKColors.Orange;
                    break;
                case nhcolor.CLR_BRIGHT_GREEN:
                    res = SKColors.LightGreen;
                    break;
                case nhcolor.CLR_YELLOW:
                    res = SKColors.Yellow;
                    break;
                case nhcolor.CLR_BRIGHT_BLUE:
                    res = SKColors.LightBlue;
                    break;
                case nhcolor.CLR_BRIGHT_MAGENTA:
                    res = SKColors.LightPink;
                    break;
                case nhcolor.CLR_BRIGHT_CYAN:
                    res = SKColors.LightCyan;
                    break;
                case nhcolor.CLR_WHITE:
                    res = SKColors.White;
                    break;
                case nhcolor.CLR_MAX:
                    break;
                default:
                    break;
            }

            return res;
        }

        public static Color NHColor2XColor(nhcolor nhcolor)
        {
            Color res = Color.White;
            switch (nhcolor)
            {
                case nhcolor.CLR_BLACK:
                    res = Color.DarkGray;
                    break;
                case nhcolor.CLR_RED:
                    res = Color.Red;
                    break;
                case nhcolor.CLR_GREEN:
                    res = Color.Green;
                    break;
                case nhcolor.CLR_BROWN:
                    res = Color.Brown;
                    break;
                case nhcolor.CLR_BLUE:
                    res = Color.Blue;
                    break;
                case nhcolor.CLR_MAGENTA:
                    res = Color.Magenta;
                    break;
                case nhcolor.CLR_CYAN:
                    res = Color.Cyan;
                    break;
                case nhcolor.CLR_GRAY:
                    res = Color.LightGray;
                    break;
                case nhcolor.NO_COLOR:
                    break;
                case nhcolor.CLR_ORANGE:
                    res = Color.Orange;
                    break;
                case nhcolor.CLR_BRIGHT_GREEN:
                    res = Color.LightGreen;
                    break;
                case nhcolor.CLR_YELLOW:
                    res = Color.Yellow;
                    break;
                case nhcolor.CLR_BRIGHT_BLUE:
                    res = Color.LightBlue;
                    break;
                case nhcolor.CLR_BRIGHT_MAGENTA:
                    res = Color.LightPink;
                    break;
                case nhcolor.CLR_BRIGHT_CYAN:
                    res = Color.LightCyan;
                    break;
                case nhcolor.CLR_WHITE:
                    res = Color.White;
                    break;
                case nhcolor.CLR_MAX:
                    break;
                default:
                    break;
            }

            return res;
        }
        public static Color NHColor2GrayedXColor(nhcolor color)
        {
            Color basecolor = NHColor2XColor(color);
            Color bgcolor = Color.Black;
            Color grayedcolor;
            if (color == nhcolor.NO_COLOR || color == nhcolor.CLR_WHITE)
                grayedcolor = new Color((basecolor.R + bgcolor.R) / 2, (basecolor.G + bgcolor.G) / 2, (basecolor.B + bgcolor.B) / 2, basecolor.A);
            else /* Special colors are brighter */
                grayedcolor = new Color((basecolor.R * 2 + bgcolor.R) / 3, (basecolor.G * 2 + bgcolor.G) / 3, (basecolor.B * 2 + bgcolor.B) / 3, basecolor.A);

            return grayedcolor;
        }
    }
}
