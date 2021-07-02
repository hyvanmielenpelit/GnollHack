using System;
using System.Collections.Generic;
using System.Drawing;
using System.Text;

namespace GnollHackCommon
{
    public class GHUtils
    {
        public static int Ctrl(int c)
        {
            return (0x1f & (c));
        }
        public static int Meta(int c)
        {
            return (0x80 | (c));
        }
        public static bool isok(int x, int y)
        {
            if (x < 1 || x >= GHConstants.MapCols)
                return false;
            if (y < 0 || y >= GHConstants.MapRows)
                return false;

            return true;
        }
        public static Color NHColor2Color(nhcolor nhcolor)
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
    }
}
