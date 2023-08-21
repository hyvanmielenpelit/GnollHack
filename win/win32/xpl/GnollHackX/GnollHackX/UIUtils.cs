using System;
using System.Collections.Generic;
using System.Text;
using SkiaSharp;
#if GNH_MAUI
using GnollHackM;
using Microsoft.Maui.Controls;
#else
using Xamarin.Forms;
using Xamarin.Forms.PlatformConfiguration.AndroidSpecific;
#endif

namespace GnollHackX
{
    public enum canvas_page_types
    {
        MainGamePage = 0,
        MenuPage,
    }

    public enum menu_page_types
    {
        NormalMenu = 0,
    }

    class UIUtils
    {
        private static SKColor GHDarkGray = new SKColor(96, 96, 96);
        private static SKColor GHVeryDarkGray = new SKColor(64, 64, 64);
        private static SKColor GHTitleGold = new SKColor(0xD4, 0xA0, 0x17);
        private static SKColor GHRed = new SKColor(255, 32, 32);
        private static SKColor GHDarkRed = new SKColor(224, 0, 0);
        private static SKColor GHGreen = new SKColor(0, 255, 0);
        private static SKColor GHDarkGreen = new SKColor(0, 192, 0);
        private static SKColor GHBlue = new SKColor(112, 112, 255);
        private static SKColor GHDarkBlue = new SKColor(64, 64, 255);
        private static SKColor GHBrown = new SKColor(210, 128, 32);
        private static SKColor GHDarkBrown = new SKColor(180, 92, 0);
        private static SKColor GHBrightBlue = new SKColor(180, 200, 255);
        private static SKColor GHCyan = new SKColor(133, 224, 224);
        private static SKColor GHBrightCyan = new SKColor(165, 255, 255);
        private static SKColor GHBrightCyanReverted = new SKColor(190, 255, 255);
        private static SKColor GHDarkYellow = new SKColor(192, 192, 0);

        public static SKColor NHColor2SKColor(int nhclr, int attr)
        {
            return NHColor2SKColorCore(nhclr, attr, false, false);
        }
        public static SKColor NHColor2SKColorCore(int nhclr, int attr, bool revertblackwhite, bool isselected)
        {
            SKColor res;
            if (isselected)
                res = revertblackwhite ? SKColors.DarkGray : SKColors.LightGray;
            else
                res = revertblackwhite ? SKColors.Black : SKColors.White;

            bool usealtcolors = (attr & (int)MenuItemAttributes.AltColors) != 0;
            switch ((nhcolor)nhclr)
            {
                case nhcolor.CLR_BLACK:
                    if(isselected)
                        res = revertblackwhite ? SKColors.Gray : GHVeryDarkGray;
                    else
                        res = revertblackwhite ? SKColors.White : GHDarkGray;
                    break;
                case nhcolor.CLR_RED:
                    if (isselected)
                        res = revertblackwhite ? SKColors.DarkRed : GHDarkRed;
                    else
                        res = revertblackwhite ? SKColors.Red : GHRed;
                    break;
                case nhcolor.CLR_GREEN:
                    if (isselected)
                        res = revertblackwhite ? SKColors.DarkGreen : GHDarkGreen;
                    else
                        res = revertblackwhite ? SKColors.Green : GHGreen;
                    break;
                case nhcolor.CLR_BROWN:
                    if (usealtcolors)
                        res = GHTitleGold;
                    else
                    {
                        if (isselected)
                            res = revertblackwhite ? GHDarkBrown : GHDarkBrown;
                        else
                            res = revertblackwhite ? SKColors.Brown : GHBrown;
                    }
                    break;
                case nhcolor.CLR_BLUE:
                    if (isselected)
                        res = revertblackwhite ? SKColors.DarkBlue : GHDarkBlue;
                    else
                        res = revertblackwhite ? SKColors.Blue : GHBlue;
                    break;
                case nhcolor.CLR_MAGENTA:
                    if (isselected)
                        res = SKColors.DarkMagenta;
                    else
                        res = SKColors.Magenta;
                    break;
                case nhcolor.CLR_CYAN:
                    if (isselected)
                        res = SKColors.DarkCyan;
                    else
                        res = SKColors.Cyan;
                    break;
                case nhcolor.CLR_GRAY:
                    if (isselected)
                        res = revertblackwhite ? GHVeryDarkGray : SKColors.Gray;
                    else
                        res = revertblackwhite ? GHDarkGray : SKColors.LightGray;
                    break;
                case nhcolor.NO_COLOR:
                    break;
                case nhcolor.CLR_ORANGE:
                    if (isselected)
                        res = SKColors.DarkOrange;
                    else
                        res = SKColors.Orange;
                    break;
                case nhcolor.CLR_BRIGHT_GREEN:
                    if (isselected)
                        res = SKColors.Green;
                    else
                        res = SKColors.LightGreen;
                    break;
                case nhcolor.CLR_YELLOW:
                    if (isselected)
                        res = GHDarkYellow;
                    else
                        res = SKColors.Yellow;
                    break;
                case nhcolor.CLR_BRIGHT_BLUE:
                    if (isselected)
                        res = revertblackwhite ? SKColors.Blue : GHBlue;
                    else
                        res = revertblackwhite ? SKColors.LightBlue : GHBrightBlue;
                    break;
                case nhcolor.CLR_BRIGHT_MAGENTA:
                    if (isselected)
                        res = SKColors.Pink;
                    else
                        res = SKColors.LightPink;
                    break;
                case nhcolor.CLR_BRIGHT_CYAN:
                    if (isselected)
                        res = revertblackwhite ? SKColors.Cyan : GHCyan;
                    else
                        res = revertblackwhite ? GHBrightCyanReverted : GHBrightCyan;
                    break;
                case nhcolor.CLR_WHITE:
                    if (isselected)
                        res = revertblackwhite ? SKColors.DarkGray : SKColors.Gray;
                    else
                        res = revertblackwhite ? SKColors.Black : SKColors.White;
                    break;
                case nhcolor.CLR_MAX:
                    break;
                default:
                    break;
            }
            return res;
        }

        public static Color NHColor2XColor(int nhclr, int attr, bool revertblackandwhite, bool istitle)
        {
            Color res = istitle ? GHColors.TitleGoldColor : revertblackandwhite ? GHColors.Black : GHColors.White;
            bool usealtcolors = (attr & (int)MenuItemAttributes.AltColors) != 0;
            switch ((nhcolor)nhclr)
            {
                case nhcolor.CLR_BLACK:
                    res = revertblackandwhite ? GHColors.White : GHColors.DarkGray;
                    break;
                case nhcolor.CLR_RED:
                    res = revertblackandwhite ? GHColors.Red : GHColors.BrighterRed;
                    break;
                case nhcolor.CLR_GREEN:
                    res = revertblackandwhite ? GHColors.Green : GHColors.BrighterGreen;
                    break;
                case nhcolor.CLR_BROWN:
                    if (usealtcolors)
                        res = GHColors.TitleGoldColor;
                    else
                        res = revertblackandwhite ? GHColors.Brown : GHColors.BrighterBrown;
                    break;
                case nhcolor.CLR_BLUE:
                    res = revertblackandwhite ? GHColors.Blue : GHColors.BrighterBlue;
                    break;
                case nhcolor.CLR_MAGENTA:
                    res = GHColors.Magenta;
                    break;
                case nhcolor.CLR_CYAN:
                    res = GHColors.Cyan;
                    break;
                case nhcolor.CLR_GRAY:
                    res = GHColors.LightGray;
                    break;
                case nhcolor.NO_COLOR:
                    break;
                case nhcolor.CLR_ORANGE:
                    res = GHColors.Orange;
                    break;
                case nhcolor.CLR_BRIGHT_GREEN:
                    res = GHColors.LightGreen;
                    break;
                case nhcolor.CLR_YELLOW:
                    res = GHColors.Yellow;
                    break;
                case nhcolor.CLR_BRIGHT_BLUE:
                    res = revertblackandwhite ? GHColors.LightBlue : GHColors.BrightBlue;
                    break;
                case nhcolor.CLR_BRIGHT_MAGENTA:
                    res = GHColors.LightPink;
                    break;
                case nhcolor.CLR_BRIGHT_CYAN:
                    res = revertblackandwhite ? GHColors.LightCyan : GHColors.BrightCyan;
                    break;
                case nhcolor.CLR_WHITE:
                    res = revertblackandwhite ? GHColors.Black : GHColors.White;
                    break;
                case nhcolor.CLR_MAX:
                    break;
                default:
                    break;
            }
            return res;
        }
        public static Color NHColor2GrayedXColor(int color, int attr, bool revertblackandwhite, bool istitle)
        {
            Color basecolor = NHColor2XColor(color, attr, revertblackandwhite, istitle);
            Color bgcolor = revertblackandwhite ? GHColors.White : GHColors.Black;
            Color grayedcolor;
#if GNH_MAUI
            if (color == (int)nhcolor.NO_COLOR || color == (revertblackandwhite ? (int)nhcolor.CLR_BLACK : (int)nhcolor.CLR_WHITE))
                grayedcolor = new Color((basecolor.Red + bgcolor.Red) / 2, (basecolor.Green + bgcolor.Green) / 2, (basecolor.Blue + bgcolor.Blue) / 2, basecolor.Alpha);
            else /* Special colors are brighter */
                grayedcolor = new Color((basecolor.Red * 2 + bgcolor.Red) / 3, (basecolor.Green * 2 + bgcolor.Green) / 3, (basecolor.Blue * 2 + bgcolor.Blue) / 3, basecolor.Alpha);
#else
            if (color == (int)nhcolor.NO_COLOR || color == (revertblackandwhite ? (int)nhcolor.CLR_BLACK : (int)nhcolor.CLR_WHITE))
                grayedcolor = new Color((basecolor.R + bgcolor.R) / 2, (basecolor.G + bgcolor.G) / 2, (basecolor.B + bgcolor.B) / 2, basecolor.A);
            else /* Special colors are brighter */
                grayedcolor = new Color((basecolor.R * 2 + bgcolor.R) / 3, (basecolor.G * 2 + bgcolor.G) / 3, (basecolor.B * 2 + bgcolor.B) / 3, basecolor.A);
#endif
            return grayedcolor;
        }


        public static double MenuViewWidthRequest(ghmenu_styles style)
        {
            double res = 440;
            switch (style)
            {
                case ghmenu_styles.GHMENU_STYLE_GENERAL:
                case ghmenu_styles.GHMENU_STYLE_PREVIOUS_MESSAGES:
                case ghmenu_styles.GHMENU_STYLE_VIEW_FILE:
                    break;
                case ghmenu_styles.GHMENU_STYLE_INVENTORY:
                case ghmenu_styles.GHMENU_STYLE_SKILLS_ALTERNATE:
                case ghmenu_styles.GHMENU_STYLE_SPELLS_ALTERNATE:
                case ghmenu_styles.GHMENU_STYLE_MONSTER_ABILITY:
                    break;
                case ghmenu_styles.GHMENU_STYLE_PERMANENT_INVENTORY:
                    break;
                case ghmenu_styles.GHMENU_STYLE_OTHERS_INVENTORY:
                    break;
                case ghmenu_styles.GHMENU_STYLE_PICK_ITEM_LIST:
                    break;
                case ghmenu_styles.GHMENU_STYLE_PICK_CATEGORY_LIST:
                    break;
                case ghmenu_styles.GHMENU_STYLE_ITEM_COMMAND:
                case ghmenu_styles.GHMENU_STYLE_SKILL_COMMAND:
                case ghmenu_styles.GHMENU_STYLE_SPELL_COMMAND:
                case ghmenu_styles.GHMENU_STYLE_GENERAL_COMMAND:
                    break;
                case ghmenu_styles.GHMENU_STYLE_CHAT:
                    break;
                case ghmenu_styles.GHMENU_STYLE_CHAT_CHOOSE_ITEM:
                    break;
                case ghmenu_styles.GHMENU_STYLE_CHOOSE_SIMPLE:
                    break;
                case ghmenu_styles.GHMENU_STYLE_CHOOSE_COMMAND:
                    break;
                case ghmenu_styles.GHMENU_STYLE_START_GAME_MENU:
                    res = 340;
                    break;
                case ghmenu_styles.GHMENU_STYLE_DELETE_SAVED_GAME:
                case ghmenu_styles.GHMENU_STYLE_CHOOSE_SAVED_GAME:
                    res = 550;
                    break;
                case ghmenu_styles.GHMENU_STYLE_CHOOSE_PLAYER:
                    break;
                case ghmenu_styles.GHMENU_STYLE_CHOOSE_DIFFICULTY:
                    break;
                case ghmenu_styles.GHMENU_STYLE_CHARACTER:
                    break;
                case ghmenu_styles.GHMENU_STYLE_ATTRIBUTES:
                    res = 660;
                    break;
                case ghmenu_styles.GHMENU_STYLE_SKILLS:
                    res = 1024;
                    break;
                case ghmenu_styles.GHMENU_STYLE_SPELLS:
                    res = 1024;
                    break;
                case ghmenu_styles.GHMENU_STYLE_DUNGEON_OVERVIEW:
                    res = 550;
                    break;
                case ghmenu_styles.GHMENU_STYLE_OPTIONS:
                    res = 660;
                    break;
                case ghmenu_styles.GHMENU_STYLE_HELP:
                    res = 880;
                    break;
                case ghmenu_styles.MAX_GHMENU_STYLES:
                    break;
            }
            return res;
        }

        public static string MenuHeaderFontFamily(ghmenu_styles style)
        {
            string res = "Immortal";
            switch (style)
            {
                case ghmenu_styles.GHMENU_STYLE_GENERAL:
                case ghmenu_styles.GHMENU_STYLE_PREVIOUS_MESSAGES:
                case ghmenu_styles.GHMENU_STYLE_VIEW_FILE:
                    break;
                case ghmenu_styles.GHMENU_STYLE_INVENTORY:
                case ghmenu_styles.GHMENU_STYLE_SKILLS_ALTERNATE:
                case ghmenu_styles.GHMENU_STYLE_SPELLS_ALTERNATE:
                    res = "Diablo";
                    break;
                case ghmenu_styles.GHMENU_STYLE_PERMANENT_INVENTORY:
                    break;
                case ghmenu_styles.GHMENU_STYLE_OTHERS_INVENTORY:
                    break;
                case ghmenu_styles.GHMENU_STYLE_PICK_ITEM_LIST:
                    break;
                case ghmenu_styles.GHMENU_STYLE_PICK_CATEGORY_LIST:
                    break;
                case ghmenu_styles.GHMENU_STYLE_ITEM_COMMAND:
                case ghmenu_styles.GHMENU_STYLE_SKILL_COMMAND:
                case ghmenu_styles.GHMENU_STYLE_SPELL_COMMAND:
                case ghmenu_styles.GHMENU_STYLE_GENERAL_COMMAND:
                case ghmenu_styles.GHMENU_STYLE_MONSTER_ABILITY:
                    break;
                case ghmenu_styles.GHMENU_STYLE_CHAT:
                    break;
                case ghmenu_styles.GHMENU_STYLE_CHAT_CHOOSE_ITEM:
                    break;
                case ghmenu_styles.GHMENU_STYLE_CHOOSE_SIMPLE:
                    break;
                case ghmenu_styles.GHMENU_STYLE_CHOOSE_COMMAND:
                    break;
                case ghmenu_styles.GHMENU_STYLE_START_GAME_MENU:
                    res = "ARChristy";
                    break;
                case ghmenu_styles.GHMENU_STYLE_DELETE_SAVED_GAME:
                case ghmenu_styles.GHMENU_STYLE_CHOOSE_SAVED_GAME:
                    break;
                case ghmenu_styles.GHMENU_STYLE_CHOOSE_PLAYER:
                    break;
                case ghmenu_styles.GHMENU_STYLE_CHOOSE_DIFFICULTY:
                    break;
                case ghmenu_styles.GHMENU_STYLE_CHARACTER:
                    break;
                case ghmenu_styles.GHMENU_STYLE_ATTRIBUTES:
                    break;
                case ghmenu_styles.GHMENU_STYLE_SKILLS:
                    break;
                case ghmenu_styles.GHMENU_STYLE_SPELLS:
                    break;
                case ghmenu_styles.GHMENU_STYLE_DUNGEON_OVERVIEW:
                    break;
                case ghmenu_styles.GHMENU_STYLE_OPTIONS:
                    break;
                case ghmenu_styles.GHMENU_STYLE_HELP:
                    break;
                case ghmenu_styles.MAX_GHMENU_STYLES:
                    break;
            }
            return res;
        }

        public static double MenuHeaderFontSize(ghmenu_styles style)
        {
            double res = 22;
            switch (style)
            {
                case ghmenu_styles.GHMENU_STYLE_GENERAL:
                case ghmenu_styles.GHMENU_STYLE_PREVIOUS_MESSAGES:
                case ghmenu_styles.GHMENU_STYLE_VIEW_FILE:
                    break;
                case ghmenu_styles.GHMENU_STYLE_INVENTORY:
                case ghmenu_styles.GHMENU_STYLE_SKILLS_ALTERNATE:
                case ghmenu_styles.GHMENU_STYLE_SPELLS_ALTERNATE:
                    break;
                case ghmenu_styles.GHMENU_STYLE_PERMANENT_INVENTORY:
                    break;
                case ghmenu_styles.GHMENU_STYLE_OTHERS_INVENTORY:
                    break;
                case ghmenu_styles.GHMENU_STYLE_PICK_ITEM_LIST:
                    break;
                case ghmenu_styles.GHMENU_STYLE_PICK_CATEGORY_LIST:
                    break;
                case ghmenu_styles.GHMENU_STYLE_ITEM_COMMAND:
                case ghmenu_styles.GHMENU_STYLE_SKILL_COMMAND:
                case ghmenu_styles.GHMENU_STYLE_SPELL_COMMAND:
                case ghmenu_styles.GHMENU_STYLE_GENERAL_COMMAND:
                case ghmenu_styles.GHMENU_STYLE_MONSTER_ABILITY:
                    break;
                case ghmenu_styles.GHMENU_STYLE_CHAT:
                    break;
                case ghmenu_styles.GHMENU_STYLE_CHAT_CHOOSE_ITEM:
                    break;
                case ghmenu_styles.GHMENU_STYLE_CHOOSE_SIMPLE:
                    break;
                case ghmenu_styles.GHMENU_STYLE_CHOOSE_COMMAND:
                    break;
                case ghmenu_styles.GHMENU_STYLE_START_GAME_MENU:
                    res = 42;
                    break;
                case ghmenu_styles.GHMENU_STYLE_DELETE_SAVED_GAME:
                case ghmenu_styles.GHMENU_STYLE_CHOOSE_SAVED_GAME:
                    break;
                case ghmenu_styles.GHMENU_STYLE_CHOOSE_PLAYER:
                    break;
                case ghmenu_styles.GHMENU_STYLE_CHOOSE_DIFFICULTY:
                    break;
                case ghmenu_styles.GHMENU_STYLE_CHARACTER:
                    break;
                case ghmenu_styles.GHMENU_STYLE_ATTRIBUTES:
                    break;
                case ghmenu_styles.GHMENU_STYLE_SKILLS:
                    break;
                case ghmenu_styles.GHMENU_STYLE_SPELLS:
                    break;
                case ghmenu_styles.GHMENU_STYLE_DUNGEON_OVERVIEW:
                    break;
                case ghmenu_styles.GHMENU_STYLE_OPTIONS:
                    break;
                case ghmenu_styles.GHMENU_STYLE_HELP:
                    break;
                case ghmenu_styles.MAX_GHMENU_STYLES:
                    break;
            }
            return res;
        }

        public static Color MenuHeaderTextColor(ghmenu_styles style)
        {
            Color res = GHColors.Black;
            switch (style)
            {
                case ghmenu_styles.GHMENU_STYLE_START_GAME_MENU:
                    res = GHColors.Beige;
                    break;
                default:
                    break;
            }
            return res;
        }

        public static double MenuHeaderOutlineWidth(ghmenu_styles style)
        {
            double res = 0;
            switch (style)
            {
                case ghmenu_styles.GHMENU_STYLE_START_GAME_MENU:
                    res = 4.0;
                    break;
                default:
                    break;
            }
            return res;
        }

        public static Color MenuHeaderOutlineColor(ghmenu_styles style)
        {
            Color res = GHColors.Black;
            switch (style)
            {
                case ghmenu_styles.GHMENU_STYLE_START_GAME_MENU:
                    break;
                default:
                    break;
            }
            return res;
        }

        public static string MenuSubtitleFontFamily(ghmenu_styles style)
        {
            string res = "Immortal";
            switch (style)
            {
                case ghmenu_styles.GHMENU_STYLE_GENERAL:
                case ghmenu_styles.GHMENU_STYLE_PREVIOUS_MESSAGES:
                case ghmenu_styles.GHMENU_STYLE_VIEW_FILE:
                    break;
                case ghmenu_styles.GHMENU_STYLE_INVENTORY:
                case ghmenu_styles.GHMENU_STYLE_SKILLS_ALTERNATE:
                case ghmenu_styles.GHMENU_STYLE_SPELLS_ALTERNATE:
                    break;
                case ghmenu_styles.GHMENU_STYLE_PERMANENT_INVENTORY:
                    break;
                case ghmenu_styles.GHMENU_STYLE_OTHERS_INVENTORY:
                    break;
                case ghmenu_styles.GHMENU_STYLE_PICK_ITEM_LIST:
                    break;
                case ghmenu_styles.GHMENU_STYLE_PICK_CATEGORY_LIST:
                    break;
                case ghmenu_styles.GHMENU_STYLE_ITEM_COMMAND:
                case ghmenu_styles.GHMENU_STYLE_SKILL_COMMAND:
                case ghmenu_styles.GHMENU_STYLE_SPELL_COMMAND:
                case ghmenu_styles.GHMENU_STYLE_GENERAL_COMMAND:
                case ghmenu_styles.GHMENU_STYLE_MONSTER_ABILITY:
                    break;
                case ghmenu_styles.GHMENU_STYLE_CHAT:
                    break;
                case ghmenu_styles.GHMENU_STYLE_CHAT_CHOOSE_ITEM:
                    break;
                case ghmenu_styles.GHMENU_STYLE_CHOOSE_SIMPLE:
                    break;
                case ghmenu_styles.GHMENU_STYLE_CHOOSE_COMMAND:
                    break;
                case ghmenu_styles.GHMENU_STYLE_START_GAME_MENU:
                case ghmenu_styles.GHMENU_STYLE_DELETE_SAVED_GAME:
                case ghmenu_styles.GHMENU_STYLE_CHOOSE_SAVED_GAME:
                    break;
                case ghmenu_styles.GHMENU_STYLE_CHOOSE_PLAYER:
                    break;
                case ghmenu_styles.GHMENU_STYLE_CHOOSE_DIFFICULTY:
                    break;
                case ghmenu_styles.GHMENU_STYLE_CHARACTER:
                    break;
                case ghmenu_styles.GHMENU_STYLE_ATTRIBUTES:
                    break;
                case ghmenu_styles.GHMENU_STYLE_SKILLS:
                    break;
                case ghmenu_styles.GHMENU_STYLE_SPELLS:
                    break;
                case ghmenu_styles.GHMENU_STYLE_DUNGEON_OVERVIEW:
                    break;
                case ghmenu_styles.GHMENU_STYLE_OPTIONS:
                    break;
                case ghmenu_styles.GHMENU_STYLE_HELP:
                    break;
                case ghmenu_styles.MAX_GHMENU_STYLES:
                    break;
            }
            return res;
        }

        public static double MenuSubtitleFontSize(ghmenu_styles style)
        {
            double res = 15;
            switch (style)
            {
                case ghmenu_styles.GHMENU_STYLE_GENERAL:
                case ghmenu_styles.GHMENU_STYLE_PREVIOUS_MESSAGES:
                case ghmenu_styles.GHMENU_STYLE_VIEW_FILE:
                    break;
                case ghmenu_styles.GHMENU_STYLE_INVENTORY:
                case ghmenu_styles.GHMENU_STYLE_SKILLS_ALTERNATE:
                    break;
                case ghmenu_styles.GHMENU_STYLE_PERMANENT_INVENTORY:
                    break;
                case ghmenu_styles.GHMENU_STYLE_OTHERS_INVENTORY:
                    break;
                case ghmenu_styles.GHMENU_STYLE_PICK_ITEM_LIST:
                    break;
                case ghmenu_styles.GHMENU_STYLE_PICK_CATEGORY_LIST:
                    break;
                case ghmenu_styles.GHMENU_STYLE_ITEM_COMMAND:
                case ghmenu_styles.GHMENU_STYLE_SKILL_COMMAND:
                case ghmenu_styles.GHMENU_STYLE_SPELL_COMMAND:
                case ghmenu_styles.GHMENU_STYLE_GENERAL_COMMAND:
                case ghmenu_styles.GHMENU_STYLE_MONSTER_ABILITY:
                    break;
                case ghmenu_styles.GHMENU_STYLE_CHAT:
                    break;
                case ghmenu_styles.GHMENU_STYLE_CHAT_CHOOSE_ITEM:
                    break;
                case ghmenu_styles.GHMENU_STYLE_CHOOSE_SIMPLE:
                    break;
                case ghmenu_styles.GHMENU_STYLE_CHOOSE_COMMAND:
                    break;
                case ghmenu_styles.GHMENU_STYLE_START_GAME_MENU:
                case ghmenu_styles.GHMENU_STYLE_DELETE_SAVED_GAME:
                case ghmenu_styles.GHMENU_STYLE_CHOOSE_SAVED_GAME:
                    break;
                case ghmenu_styles.GHMENU_STYLE_CHOOSE_PLAYER:
                    break;
                case ghmenu_styles.GHMENU_STYLE_CHOOSE_DIFFICULTY:
                    break;
                case ghmenu_styles.GHMENU_STYLE_CHARACTER:
                    break;
                case ghmenu_styles.GHMENU_STYLE_ATTRIBUTES:
                    break;
                case ghmenu_styles.GHMENU_STYLE_SKILLS:
                    break;
                case ghmenu_styles.GHMENU_STYLE_SPELLS:
                case ghmenu_styles.GHMENU_STYLE_SPELLS_ALTERNATE:
                    res = 13.5;
                    break;
                case ghmenu_styles.GHMENU_STYLE_DUNGEON_OVERVIEW:
                    break;
                case ghmenu_styles.GHMENU_STYLE_OPTIONS:
                    break;
                case ghmenu_styles.GHMENU_STYLE_HELP:
                    break;
                case ghmenu_styles.MAX_GHMENU_STYLES:
                    break;
            }
            return res;
        }

        public static bool MenuSubtitleUsesSpecialSymbols(ghmenu_styles style)
        {
            switch (style)
            {
                case ghmenu_styles.GHMENU_STYLE_SPELLS:
                case ghmenu_styles.GHMENU_STYLE_SPELLS_ALTERNATE:
                    return true;
                default:
                    return false;
            }
        }

        public static char MenuSubtitleWordWrapSeparator(ghmenu_styles style)
        {
            switch (style)
            {
                case ghmenu_styles.GHMENU_STYLE_SPELLS:
                case ghmenu_styles.GHMENU_STYLE_SPELLS_ALTERNATE:
                    return ',';
                default:
                    return ' ';
            }
        }

        public static bool MenuSubtitleDisplayWrapSeparator(ghmenu_styles style)
        {
            switch (style)
            {
                case ghmenu_styles.GHMENU_STYLE_SPELLS:
                case ghmenu_styles.GHMENU_STYLE_SPELLS_ALTERNATE:
                    return false;
                default:
                    return true;
            }
        }

        public static bool StyleClosesMenuUponDestroy(ghmenu_styles style)
        {
            switch (style)
            {
                default:
                case ghmenu_styles.GHMENU_STYLE_GENERAL:
                    return false; /* Closes already upon pressing OK / Cancel before destroy */
                case ghmenu_styles.GHMENU_STYLE_DELETE_SAVED_GAME:
                    return true;
            }
        }

        public static void ProcessAdjustedItems(List<GHPutStrItem> adjusted_list, List<GHPutStrItem> normal_list)
        {
            adjusted_list.Clear();
            GHPutStrItem newpsi = null;

            for (int cnt = 0; cnt < normal_list.Count; cnt++)
            {
                GHPutStrItem psi = normal_list[cnt];
                if (newpsi != null && (psi.Text == "" || psi.StartingSpaces != newpsi.PaddingAmount))
                {
                    adjusted_list.Add(newpsi);
                    newpsi = null;
                }

                if (psi.Text == "")
                {
                    adjusted_list.Add(psi);
                }
                else
                {
                    bool isnewpsi = false;
                    if (newpsi == null)
                    {
                        newpsi = new GHPutStrItem(psi.ReferenceGamePage, psi.Window, "");
                        isnewpsi = true;
                    }

                    if (newpsi.Text != "")
                    {
                        newpsi.Text += " ";
                        newpsi.InstructionList.Add(new GHPutStrInstructions((int)MenuItemAttributes.None, (int)nhcolor.NO_COLOR, 1));
                    }

                    int spacecnt = 0;
                    for (int i = 0; i < psi.Text.Length; i++)
                    {
                        char ch = psi.Text[i];
                        if (ch == ' ')
                            spacecnt++;
                        else
                            break;
                    }
                    newpsi.Text += psi.Text.TrimStart(' ');
                    if (spacecnt > 0)
                    {
                        if (isnewpsi)
                            newpsi.PaddingAmount = spacecnt;

                        if (psi.InstructionList.Count > 0)
                        {
                            if (psi.InstructionList[0].PrintLength > spacecnt)
                                newpsi.InstructionList.Add(new GHPutStrInstructions(psi.InstructionList[0].Attributes, psi.InstructionList[0].Color, psi.InstructionList[0].PrintLength - spacecnt));
                            for (int i = 1; i < psi.InstructionList.Count; i++)
                                newpsi.InstructionList.Add(psi.InstructionList[i]);
                        }
                    }
                    else
                        newpsi.InstructionList.AddRange(psi.InstructionList);

                    if (newpsi != null && (cnt == normal_list.Count - 1))
                    {
                        adjusted_list.Add(newpsi);
                        break;
                    }
                }
            }
        }

        public static Thickness GetHeaderMarginWithBorder(BorderStyles borderstyle, double width, double height)
        {
            return GetHeaderMarginWithBorderWithBottom(borderstyle, width, height, 2.0);
        }

        public static Thickness GetMiddleElementMarginWithBorder(BorderStyles borderstyle, double width, double height)
        {
            switch (borderstyle)
            {
                case BorderStyles.None:
                    break;
                case BorderStyles.Simple:
                    {
                        double bordermarginsmall = GetSmallBorderMargin(width, height);
                        double bordermarginx = bordermarginsmall + (width / GHConstants.BackgroundBorderDivisor - bordermarginsmall) / 2;
                        double bordermarginy = bordermarginsmall + (height / GHConstants.BackgroundBorderDivisor - bordermarginsmall) / 2;
                        double bordermargin = Math.Min(bordermarginx, bordermarginy);
                        return new Thickness(bordermargin, 0, bordermargin, 0);
                    }
                case BorderStyles.Small:
                    {
                        double bordermargin = GetSmallBorderMargin(width, height);
                        return new Thickness(bordermargin, 0, bordermargin, 0);
                    }
                case BorderStyles.Custom:
                    break;
            }
            return new Thickness(2.0, 0, 2.0, 0);
        }

        public static Thickness GetHeaderMarginWithBorderWithBottom(BorderStyles borderstyle, double width, double height, double bottom)
        {
            switch (borderstyle)
            {
                case BorderStyles.None:
                    break;
                case BorderStyles.Simple:
                    {
                        double bordermarginx = (width / GHConstants.BackgroundBorderDivisor);
                        double bordermarginy = (height / GHConstants.BackgroundBorderDivisor);
                        double bordermargin = Math.Min(bordermarginx, bordermarginy);
                        return new Thickness(bordermargin, Math.Min((double)GHApp.SimpleFrameTopHorizontalBitmap.Height, bordermargin / GHConstants.BackgroundTopBorderExtraDivisor), bordermargin, bottom);
                    }
                case BorderStyles.Small:
                    {
                        double bordermargin = GetSmallBorderMargin(width, height);
                        return new Thickness(bordermargin, bordermargin, bordermargin, bottom);
                    }
                case BorderStyles.Custom:
                    break;
            }
            return new Thickness(2.0, 10.0, 2.0, 2.0);
        }

        public static Thickness GetFooterMarginWithBorder(BorderStyles borderstyle, double width, double height)
        {
            return GetFooterMarginWithBorderWithTop(borderstyle, width, height, 2.0);
        }

        public static Thickness GetFooterMarginWithBorderWithTop(BorderStyles borderstyle, double width, double height, double top)
        {
            switch (borderstyle)
            {
                case BorderStyles.None:
                    break;
                case BorderStyles.Simple:
                    {
                        double bordermarginx = (width / GHConstants.BackgroundBorderDivisor);
                        double bordermarginy = (height / GHConstants.BackgroundBorderDivisor);
                        double bordermargin = Math.Min(bordermarginx, bordermarginy);
                        return new Thickness(bordermargin, top, bordermargin, Math.Min((double)GHApp.SimpleFrameTopHorizontalBitmap.Height, bordermargin / GHConstants.BackgroundTopBorderExtraDivisor));
                    }
                case BorderStyles.Small:
                    {
                        double bordermargin = GetSmallBorderMargin(width, height);
                        return new Thickness(bordermargin, top, bordermargin, bordermargin);
                    }
                case BorderStyles.Custom:
                    break;
            }
            return new Thickness(2.0, 2.0, 2.0, 10.0);
        }

        public static double GetSmallBorderMargin(double width, double height)
        {
            double bordermarginx = (width / (GHConstants.BackgroundBorderDivisor * GHConstants.BackgroundTopBorderExtraDivisor));
            double bordermarginy = (height / (GHConstants.BackgroundBorderDivisor * GHConstants.BackgroundTopBorderExtraDivisor));
            return Math.Min(bordermarginx, bordermarginy);
        }

        public static Thickness GetSmallBorderThickness(double width, double height, double scaling_factor)
        {
            double bordermargin = GetSmallBorderMargin(width, height) * scaling_factor;
            return new Thickness(bordermargin, bordermargin, bordermargin, bordermargin);
        }

        public static double GetBorderScale(BorderStyles borderstyle, double width, double height)
        {
            double bordermarginx = (width / GHConstants.BackgroundBorderDivisor);
            double bordermarginy = (height / GHConstants.BackgroundBorderDivisor);
            double bordermargin = Math.Min(bordermarginx, bordermarginy);
            SKBitmap cornerbitmap = borderstyle == BorderStyles.Simple ? GHApp.SimpleFrameTopLeftCornerBitmap : borderstyle == BorderStyles.Small ? GHApp.SimpleFrameSmallTopLeftCornerBitmap : GHApp.SimpleFrameTopLeftCornerBitmap;
            double scale = Math.Max(0.1, Math.Min(1.0, bordermargin / (double)cornerbitmap.Height));
            return scale;
        }
        public static double GetBorderWidth(BorderStyles borderstyle, double width, double height)
        {
            double scale = GetBorderScale(borderstyle, width, height);
            return GHApp.SimpleFrameTopHorizontalBitmap.Height * scale;
        }
        public static double GetBorderHeight(BorderStyles borderstyle, double width, double height)
        {
            double scale = GetBorderScale(borderstyle, width, height);
            return GHApp.SimpleFrameLeftVerticalBitmap.Width * scale;
        }

        public static uint GetMainCanvasAnimationInterval(MapRefreshRateStyle mapRefreshRate)
        {
            if (GHApp.DisplayRefreshRate >= 120.0f && mapRefreshRate >= MapRefreshRateStyle.MapFPS120)
                return 8;
            else if (GHApp.DisplayRefreshRate >= 90.0f && mapRefreshRate >= MapRefreshRateStyle.MapFPS90)
                return 11;
            else if (GHApp.DisplayRefreshRate >= 80.0f && mapRefreshRate >= MapRefreshRateStyle.MapFPS80)
                return 13;
            else if (GHApp.DisplayRefreshRate >= 60.0f && mapRefreshRate >= MapRefreshRateStyle.MapFPS60)
                return 16;
            else if (GHApp.DisplayRefreshRate >= 40.0f && mapRefreshRate >= MapRefreshRateStyle.MapFPS40)
                return 25;
            else if (GHApp.DisplayRefreshRate >= 30.0f && mapRefreshRate >= MapRefreshRateStyle.MapFPS30)
                return 33;
            else if (GHApp.DisplayRefreshRate >= 20.0f && mapRefreshRate >= MapRefreshRateStyle.MapFPS20)
                return 50;
            else
                return 100;

            //return GHConstants.MainCanvasAnimationInterval;
        }
        public static int GetMainCanvasAnimationFrequency(MapRefreshRateStyle mapRefreshRate)
        {
            if (GHApp.DisplayRefreshRate >= 120.0f && mapRefreshRate >= MapRefreshRateStyle.MapFPS120)
                return 120;
            else if (GHApp.DisplayRefreshRate >= 90.0f && mapRefreshRate >= MapRefreshRateStyle.MapFPS90)
                return 90;
            else if (GHApp.DisplayRefreshRate >= 80.0f && mapRefreshRate >= MapRefreshRateStyle.MapFPS80)
                return 80;
            else if (GHApp.DisplayRefreshRate >= 60.0f && mapRefreshRate >= MapRefreshRateStyle.MapFPS60)
                return 60;
            else if (GHApp.DisplayRefreshRate >= 40.0f && mapRefreshRate >= MapRefreshRateStyle.MapFPS40)
                return 40;
            else if (GHApp.DisplayRefreshRate >= 30.0f && mapRefreshRate >= MapRefreshRateStyle.MapFPS30)
                return 30;
            else if (GHApp.DisplayRefreshRate >= 20.0f && mapRefreshRate >= MapRefreshRateStyle.MapFPS20)
                return 20;
            else
                return 40;
        }

        public static MapRefreshRateStyle GetDefaultMapFPS()
        {
            if (GHApp.DisplayRefreshRate >= 60f)
                return MapRefreshRateStyle.MapFPS60;
            else if (GHApp.DisplayRefreshRate >= 40f)
                return MapRefreshRateStyle.MapFPS40;
            else
                return MapRefreshRateStyle.MapFPS30;
        }

        public static uint GetAuxiliaryCanvasAnimationInterval()
        {
            if (GHApp.DisplayRefreshRate >= 120.0f)
                return 8;
            else if (GHApp.DisplayRefreshRate >= 90.0f)
                return 11;
            else
                return 16;
        }
        public static int GetAuxiliaryCanvasAnimationFrequency()
        {
            if (GHApp.DisplayRefreshRate >= 120.0f)
                return 120;
            else if (GHApp.DisplayRefreshRate >= 90.0f)
                return 90;
            else
                return 60;
        }


        public static double GetWindowHideSecs()
        {
            return 0.05 + ((double)GHConstants.PollingInterval) / 1000 * GHConstants.WindowHideIntervals;
        }
    }

    public class TouchEntry
    {
        public SKPoint Location;
        public SKPoint OriginalLocation;
        public DateTime UpdateTime;
        public DateTime PressTime;

        public TouchEntry()
        {

        }
        public TouchEntry(SKPoint loc, DateTime time)
        {
            Location = loc;
            OriginalLocation = loc;
            UpdateTime = time;
            PressTime = time;
        }
    }

    public class TouchSpeedRecord
    {
        public float Distance;
        public float Duration;
        public DateTime TimeStamp;

        public TouchSpeedRecord()
        {

        }
        public TouchSpeedRecord(float distance, float duration, DateTime time)
        {
            Distance = distance;
            Duration = duration;
            TimeStamp = time;
        }
    }
}