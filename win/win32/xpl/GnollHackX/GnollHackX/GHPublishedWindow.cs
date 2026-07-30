#if GNH_MAUI
using GnollHackM;
using GnollHackX;
#else
using GnollHackX.Pages.Game;
#endif
using SkiaSharp;
using System;
using System.Collections.Generic;

namespace GnollHackX
{
    public sealed class GHPublishedWindow
    {
        public GHWinType WindowType { get; }
        public ghwindow_styles WindowStyle { get; }
        public int Glyph { get; }
        public bool UseUpperSide { get; }
        public bool UseSpecialSymbols { get; }
        public bool Ascension { get; }
        public int WindowID { get; }
        public ObjectDataItem ObjData { get; }

        public SKTypeface Typeface { get; }
        public SKColor TextColor { get; }
        public float TextSize { get; }
        public float StrokeWidth { get; }
        public bool HasShadow { get; }
        public SKColor BackgroundColor { get; }
        public int CursX { get; }
        public int CursY { get; }
        public bool CenterHorizontally { get; }
        public bool AutoPlacement { get; }
        
        public bool AutoCarriageReturn { get; }
        public GHWindowPrintLocations WindowPrintStyle { get; }

        public List<GHPublishedWindowRow> PutStrs { get; }
        
        public bool Visible { get; set; }
        public int WidthInChars { get; }
        public int HeightInChars { get; }
        public float UnscaledWidth { get; }
        public float UnscaledHeight { get; }

        public GHPadding Padding { get; }
        /* MUTABILITY EXCEPTION: Left and Top are mutable because the UI layout
         * system calculates and sets these coordinates dynamically. */
        public float Left { get; set; }
        public float Top { get; set; }

        public GHMenuInfo MenuInfo { get; }

        /* MUTABILITY EXCEPTION: SelectedMenuItems and WasCancelled represent user
         * input returning from the UI to the game logic. The UI modifies these
         * directly before passing the GHPublishedWindow in a GHResponse back to
         * GHGame. This is safe because each menu interaction creates a fresh
         * published snapshot and this data is handled with Interlocked structures
         * or on the main thread. */
        public List<GHMenuItem> SelectedMenuItems { get; set; }
        public bool WasCancelled { get; set; }

        public WeakReference<GHWindow> PublishedFrom { get; }

        public GHPublishedWindow(
            GHWinType winType, ghwindow_styles winStyle, int glyph, bool useUpperSide, 
            bool useSpecialSymbols, bool ascension, ObjectDataItem objdata, int winId,
            SKTypeface typeface, SKColor textColor, float textSize, float strokeWidth,
            bool hasShadow, SKColor backgroundColor, int cursX, int cursY,
            bool centerHorizontally, bool autoPlacement, bool autoCarriageReturn,
            GHWindowPrintLocations windowPrintStyle, List<GHPublishedWindowRow> putStrs,
            bool visible, int widthInChars, int heightInChars, float unscaledWidth, float unscaledHeight,
            GHPadding padding, float left, float top, GHMenuInfo menuInfo,
            List<GHMenuItem> selectedMenuItems, bool wasCancelled, WeakReference<GHWindow> publishedFrom)
        {
            WindowType = winType;
            WindowStyle = winStyle;
            Glyph = glyph;
            UseUpperSide = useUpperSide;
            UseSpecialSymbols = useSpecialSymbols;
            Ascension = ascension;
            ObjData = objdata;
            WindowID = winId;
            Typeface = typeface;
            TextColor = textColor;
            TextSize = textSize;
            StrokeWidth = strokeWidth;
            HasShadow = hasShadow;
            BackgroundColor = backgroundColor;
            CursX = cursX;
            CursY = cursY;
            CenterHorizontally = centerHorizontally;
            AutoPlacement = autoPlacement;
            AutoCarriageReturn = autoCarriageReturn;
            WindowPrintStyle = windowPrintStyle;
            PutStrs = putStrs;
            Visible = visible;
            WidthInChars = widthInChars;
            HeightInChars = heightInChars;
            UnscaledWidth = unscaledWidth;
            UnscaledHeight = unscaledHeight;
            Padding = padding;
            Left = left;
            Top = top;
            MenuInfo = menuInfo;
            SelectedMenuItems = selectedMenuItems;
            WasCancelled = wasCancelled;
            PublishedFrom = publishedFrom;
        }

        public SKRect GetWindowRect(float textScalingFactor)
        {
            return new SKRect(Left, Top, Left + UnscaledWidth * textScalingFactor, Top + UnscaledHeight * textScalingFactor);
        }

        public double TextWindowMaximumWidth
        {
            get
            {
                switch (WindowStyle)
                {
                    case ghwindow_styles.GHWINDOW_STYLE_ENLIGHTENMENT_SCREEN:
                    case ghwindow_styles.GHWINDOW_STYLE_SEMI_WIDE_LIST:
                    case ghwindow_styles.GHWINDOW_STYLE_MONSTER_DESCRIPTION_SCREEN:
                    case ghwindow_styles.GHWINDOW_STYLE_OBJECT_DESCRIPTION_SCREEN:
                        return 520.0;
                    case ghwindow_styles.GHWINDOW_STYLE_CHARACTER_SCREEN:
                    case ghwindow_styles.GHWINDOW_STYLE_SPELL_DESCRIPTION_SCREEN:
                    case ghwindow_styles.GHWINDOW_STYLE_MEDIUM_WIDE_LIST:
                        return 440.0;
                    case ghwindow_styles.GHWINDOW_STYLE_NARROW_LIST:
                    case ghwindow_styles.GHWINDOW_STYLE_SKILL_DESCRIPTION_SCREEN:
                        return 360.0;
                    case ghwindow_styles.GHWINDOW_STYLE_DISPLAY_FILE:
                    case ghwindow_styles.GHWINDOW_STYLE_DISPLAY_FILE_WITH_INDENTED_TEXT:
                        return 800.0;
                    default:
                        return 600.0;
                }
            }
        }
        
        public bool WrapGlyph
        {
            get
            {
                switch (WindowStyle)
                {
                    case ghwindow_styles.GHWINDOW_STYLE_PAGER_SPEAKER:
                    case ghwindow_styles.GHWINDOW_STYLE_OUTRIP:
                    case ghwindow_styles.GHWINDOW_STYLE_MONSTER_COMMAND_MENU:
                    case ghwindow_styles.GHWINDOW_STYLE_ENLIGHTENMENT_SCREEN:
                        return true;
                    default:
                        return false;
                }
            }
        }


    }
}
