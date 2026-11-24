using SkiaSharp;
using System;
using System.Collections.Generic;
#if GNH_MAUI
using GnollHackM;
#endif

namespace GnollHackX
{
    /* A special symbol's image and the part of it to draw, or null when the name is not a symbol */
    public delegate SKImage GHRadialSymbolLookup(string symbol, out SKRect sourceRect);

    /* Draws a GHRadialMenuState: backdrop, vignette, slices, hub, page title and dots, the L1/R1
       chips and the hint bar, and for definitions that ask for them the set tabs, slot badges, the
       description band and the carried item's ghost.
       - The set tabs and the description band take their room from the ring: the geometry is
         laid out on the canvas less those rows and drawn moved down by the tab row, which
         state.GeometryOffsetY passes on to the pointer hit tests.
       - Slice paths, the vignette shader, the blur mask filter and the path effects are cached
         per layout; item, page and hint text layouts are cached per definition and layout. A
         paint at rest therefore allocates nothing; the first paint after opening fills the
         caches.
       - Controller buttons are drawn as badges by an owned GHGamepadHintRenderer, which takes
         this renderer's typefaces and text blob cache setting.
       - The definition's Accent picks the gold or violet border, highlight, glow and flash colors.
       - A SeeThrough definition gets nearly clear slices, a lighter hub, and no slice glow or vignette, to keep the
         map readable; the slice outlines carry the highlight.
       - Free of GHApp: the host supplies the typefaces, the icon lookup, the text blob cache
         setting and the keyboard shortcut setting.
       - Opacity and scale animate through a layer and a canvas transform only while animating;
         text origins snap to whole pixels at rest.
       Single-threaded: paint on the thread that owns the canvas. */
    public sealed class GHRadialMenuRenderer : IDisposable
    {
        /* Host-supplied */
        public Func<string, SKImage> IconLookup;    /* cached images; the renderer never disposes them */
        public Func<SKCanvas, GHRadialItem, SKRect, bool, bool, bool> CustomIconDrawer; /* (canvas, item, dest, enabled, highlighted): true when it drew the icon; tried before IconLookup */
        public GHRadialSymbolLookup SymbolLookup;   /* hub run symbols; the host sets GHApp.GetSpecialSymbol */
        public Func<int, SKColor> NhColorLookup;    /* an item's or run's NhColor; the host sets c => UIUtils.NHColor2SKColor(c, 0) */
        public SKTypeface TitleTypeface;            /* Immortal */
        public SKTypeface LabelTypeface;            /* Lato Regular */
        public SKTypeface BoldTypeface;             /* Lato Bold */
        public SKTypeface DescriptionTypeface;      /* Underwood */
        public bool UseTextBlobCaching;
        public bool ShowKeyboardShortcuts;
        public float HintScale = 1f;                /* the Hint Size setting, for the hint bar and the L1/R1 chips */

        public static readonly string CurrentlyText = "Currently: ";
        public static readonly string OnText = "On";
        public static readonly string OffText = "Off";
        public static readonly string PillOnText = "ON";
        public static readonly string PillOffText = "OFF";
        public static readonly string PlaceholderText = "Empty";   /* a placeholder's label when it has none */
        public static readonly string NewBadgeText = "NEW";

        private static readonly SKColor Gold = new SKColor(0xD4, 0xA0, 0x17);
        private static readonly SKColor GoldHighlightFill = new SKColor(58, 46, 10, 0xE6);
        private static readonly SKColor GoldFlashFill = new SKColor(128, 102, 26, 0xF0);
        private static readonly SKColor Violet = new SKColor(0xA8, 0x7B, 0xFF);
        private static readonly SKColor VioletHighlightFill = new SKColor(44, 30, 74, 0xE6);
        private static readonly SKColor VioletFlashFill = new SKColor(84, 58, 140, 0xF0);
        private static readonly SKColor VioletTitle = new SKColor(0xD8, 0xC8, 0xFF);
        private static readonly SKColor SeeThroughSliceFill = new SKColor(38, 38, 38, 0x14);    /* 8 % opaque */
        private static readonly SKColor SeeThroughHubFill = new SKColor(0, 0, 0, 0x55);
        private const byte SeeThroughHighlightAlpha = 0x29;                                     /* 16 % opaque */
        private const byte SeeThroughFlashAlpha = 0x4D;                                         /* 30 % opaque */
        private static readonly SKColor SliceFill = new SKColor(38, 38, 38, 0xD9);
        private static readonly SKColor SliceBorder = new SKColor(255, 255, 255, 0x40);
        private static readonly SKColor EmptyOutline = new SKColor(255, 255, 255, 0x20);
        private static readonly SKColor DisabledBorder = new SKColor(128, 128, 128, 0xFF);
        private static readonly SKColor HubFill = new SKColor(0, 0, 0, 0xBF);
        private static readonly SKColor LighterGray = new SKColor(204, 204, 204);
        private static readonly SKColor TextGray = new SKColor(128, 128, 128);
        private static readonly SKColor OnGreen = SKColors.LightGreen;
        private static readonly SKColor PillDarkText = new SKColor(20, 20, 20);
        private static readonly SKColor StickGhostColor = new SKColor(255, 255, 255, 0x50);
        private static readonly SKColor VignetteInner = new SKColor(0, 0, 0, 0x40);
        private static readonly SKColor VignetteOuter = new SKColor(0, 0, 0, 0);
        private static readonly SKColor DisabledLabel = SKColors.White.WithAlpha(0x80);
        private static readonly SKColor StarOutline = new SKColor(70, 48, 0);
        private static readonly SKColor CountBadgeFill = new SKColor(24, 24, 24, 0xE6);
        private static readonly string Ellipsis = ((char)0x2026).ToString();
        private static readonly char[] WordSeparators = { ' ' };
        private const int MaxCachedSlotCount = 32;
        private const int MaxHints = 8;
        private const float DimmedHintOpacity = 0.45f;     /* the confirm hint over a disabled item */
        private const float BadgeTextOfHeight = 0.62f;     /* badge pill text, of the badge height */
        private const float BadgeAngleOfHalfSlot = 0.55f;  /* badges sit clockwise of the slot's center line */
        private const float HubFixedIconOfHub = 0.7f;      /* the fixed hub item's icon, of the hub radius */
        private const float HubFixedTitleRaiseOfIcon = 0.06f; /* how far the fixed hub item's label moves up into the icon's margin */
        private const float HubFixedTitleWidthOfHubDiameter = 0.68f; /* the label sits low in the hub, where the circle is narrower */
        private const float FooterTitleMinOfFont = 0.7f;   /* the band's title row shrinks to fit down to this */
        private const int MaxWrappedLines = 3;             /* the most lines a slice label or a hub title wraps into */
        private const float HubRunGapOfFont = 0.5f;        /* before a run with a symbol */
        private const float HubSymbolGapOfFont = 0.2f;     /* between a symbol and its text */
        private const float TabFontOfHeight = 0.5f;
        private const float TabIconOfHeight = 0.62f;
        private const float TabPaddingOfHeight = 0.45f;
        private const float TabBadgeOfHeight = 0.7f;
        private const float TabSpacingOfHeight = 0.35f;    /* between a tab and the X badge */

#if GNH_MAUI
        private static readonly SKSamplingOptions IconSampling = new SKSamplingOptions(SKFilterMode.Linear, SKMipmapMode.Linear);
#endif

        private sealed class ItemLayout
        {
            public int SlotCount;
            public float HubTitleSize;
            public readonly string[] HubTitleLines = new string[MaxWrappedLines];
            public int HubTitleLineCount;

            /* The slice label, fitted to the slot it was last drawn in */
            public bool LabelReady;
            public int LabelSlot = -1;
            public float LabelSize;
            public float LabelMaxWidth;             /* one line's width limit */
            public readonly string[] LabelLines = new string[MaxWrappedLines];
            public int LabelLineCount;
            public readonly string[] DescriptionLines = new string[GHRadialTuning.HubDescriptionMaxLines];
            public int DescriptionLineCount;
            public float BadgeTextWidth;            /* a Count badge's text */

            /* The hub's run lines and the description band, filled when the item is first highlighted */
            public bool HubLinesReady;
            public HubLineLayout TopLine;
            public HubLineLayout[] HubLines;
            public float HubLinesGap;               /* above the first of HubLines */
            public bool FooterReady;
            public HubLineLayout FooterTitle;
            public string[] FooterLines;
            public int FooterLineCount;
            public float FooterSize;
        }

        /* One hub line of runs at its fitted size */
        private sealed class HubLineLayout
        {
            public GHRadialHubRun[] Runs;
            public SKTypeface Typeface;
            public float Size;
            public float[] SymbolWidths;
            public float[] TextWidths;
            public float TotalWidth;
        }

        private sealed class HintBar
        {
            public readonly GHGamepadHint[] Hints = new GHGamepadHint[MaxHints];    /* the badges; the label is in Labels */
            public readonly string[] Labels = new string[MaxHints];
            public readonly float[] BadgeWidths = new float[MaxHints];
            public readonly float[] LabelWidths = new float[MaxHints];
            public int Count;
            public float TotalWidth;
            public float FontSize;      /* the labels' size; the badges are BadgeHeightOfFont times it */
            public int ConfirmIndex = -1;   /* the confirm hint's entry, or -1 */

            /* What the bar was measured for */
            public string ItemHint;
            public string CancelLabel;
            public bool ShowsSecondary;
        }

        private readonly GHSkiaFontPaint _textPaint = new GHSkiaFontPaint();
        private readonly SKPaint _plainFill = new SKPaint();
        private readonly SKPaint _plainStroke = new SKPaint();
        private readonly SKPaint _sliceFill = new SKPaint();
        private readonly SKPaint _sliceStroke = new SKPaint();
        private readonly SKPaint _emptyStroke = new SKPaint();
        private readonly SKPaint _glowPaint = new SKPaint();
        private readonly SKPaint _vignettePaint = new SKPaint();
        private readonly SKPaint _layerPaint = new SKPaint();
        private readonly SKPaint _iconPaint = new SKPaint();
        private readonly SKPaint _placeholderStroke = new SKPaint();
        private readonly SKPaint _markStroke = new SKPaint();
        private readonly SKPaint _contentLayerPaint = new SKPaint();

        private SKPathEffect _cornerEffect;
        private SKPathEffect _dashEffect;
        private SKPathEffect _emptyEffect;
        private SKMaskFilter _glowFilter;
        private SKShader _vignette;
        private SKPath _wedgePath;
        private SKPath _starPath;
        private readonly SKPath[][] _slicePaths = new SKPath[MaxCachedSlotCount + 1][];
        private SKRect _ringBounds;
        private int _resourceVersion = -1;

        private readonly Dictionary<GHRadialItem, ItemLayout> _itemLayouts = new Dictionary<GHRadialItem, ItemLayout>();
        private readonly float[] _lineWidths = new float[MaxWrappedLines];     /* FitLines' width per line count */
        private GHRadialMenuDefinition _layoutDefinition;
        private int _layoutVersion = -1;
        private SKTypeface _layoutTitleTypeface;
        private SKTypeface _layoutLabelTypeface;
        private SKTypeface _layoutBoldTypeface;
        private SKTypeface _layoutDescriptionTypeface;
        private GHGamepadStyle _layoutStyle;
        private float[] _pageTitleSizes = new float[0];
        private float[] _hubIdleTitleSizes = new float[0];
        private float[] _hubCounterSizes = new float[0];
        private float[] _emptyTextSizes = new float[0];
        private string[] _chipTitles = new string[0];
        private bool _chipsVisible;
        private float _stickTextSize;
        private float _pointerTextSize;
        private float _idleTextBaseSize;
        private float _pillOnWidth;
        private float _pillOffWidth;
        private float _currentlyWidth;
        private float _onWidth;
        private float _offWidth;
        private float _statusSize;
        private float _hubSecondaryBadgeWidth;
        private float _hubSecondaryLabelWidth;
        private bool _hasPrompt;                        /* the definition has a PromptVerb: the hub is DrawHubPrompt's */
        private string _promptVerb;
        private float _promptVerbSize;
        private float _promptTopSize;                   /* one size for every direction label and the idle texts */
        private readonly string[] _promptObjectLines = new string[GHRadialTuning.HubPromptObjectMaxLines];
        private int _promptObjectLineCount;
        private readonly HintBar _hintsMain = new HintBar();
        private readonly HintBar _hintsSwitched = new HintBar();
        private GHGamepadHintRenderer _badgeRenderer;   /* created by the first paint */
        private float _newBadgeWidth;

        /* The set tabs, measured per definition */
        private bool _hasTabs;
        private float _tabHeight;
        private float _tabFontSize;
        private float _tabIconSize;
        private float _tabRowCenterY;
        private float _tabBadgeHeight;
        private float _tabBadgeWidth;
        private float _tabsTotalWidth;
        private readonly string[] _tabLabels = new string[2];
        private readonly float[] _tabLabelWidths = new float[2];
        private readonly float[] _tabWidths = new float[2];

        /* The rows reserved above and below the geometry, decided per definition and typeface */
        private GHRadialMenuDefinition _reserveDefinition;
        private bool _reserveTabs;
        private bool _reserveFooter;
        private int _reserveFooterLines;
        private SKTypeface _spacingTypeface;
        private float _descriptionSpacingRatio = 1.2f;  /* the description typeface's line spacing, of its size */
        private float _reserveTop;
        private float _reserveBottom;

        /* The definition's accent colors, resolved at the top of each paint */
        private SKColor _accent = Gold;
        private SKColor _accentHighlightFill = GoldHighlightFill;
        private SKColor _accentFlashFill = GoldFlashFill;
        private SKColor _accentTitle = SKColors.White;
        private SKColor _sliceBaseFill = SliceFill;
        private SKColor _hubBaseFill = HubFill;
        private bool _sliceGlow = true;
        private bool _disposed;

        public GHRadialMenuRenderer()
        {
            _plainFill.IsAntialias = true;
            _plainFill.Style = SKPaintStyle.Fill;
            _plainStroke.IsAntialias = true;
            _plainStroke.Style = SKPaintStyle.Stroke;
            _sliceFill.IsAntialias = true;
            _sliceFill.Style = SKPaintStyle.Fill;
            _sliceStroke.IsAntialias = true;
            _sliceStroke.Style = SKPaintStyle.Stroke;
            _emptyStroke.IsAntialias = true;
            _emptyStroke.Style = SKPaintStyle.Stroke;
            _glowPaint.IsAntialias = true;
            _glowPaint.Style = SKPaintStyle.Fill;
            _vignettePaint.IsAntialias = true;
            _vignettePaint.Style = SKPaintStyle.Fill;
            _layerPaint.IsAntialias = false;
            _iconPaint.IsAntialias = true;
            _placeholderStroke.IsAntialias = true;
            _placeholderStroke.Style = SKPaintStyle.Stroke;
            _markStroke.IsAntialias = true;
            _markStroke.Style = SKPaintStyle.Stroke;
            _markStroke.StrokeCap = SKStrokeCap.Round;
            _contentLayerPaint.IsAntialias = false;
            _textPaint.IsAntialias = true;
            _textPaint.Style = SKPaintStyle.Fill;
        }

        /* Text drawn by other threads' requests is cleared at the top of the next paint */
        public void RequestTextCacheClear()
        {
            _textPaint.RequestBlobCacheClear();
            GHGamepadHintRenderer badges = _badgeRenderer;
            if (badges != null)
                badges.RequestTextCacheClear();
        }

        /* Draws the state on a canvas of info's size; dipScale is canvas pixels per
           device-independent pixel. Also updates state.Geometry for pointer hit tests. */
        public void Paint(SKCanvas canvas, SKImageInfo info, GHRadialMenuState state, float dipScale)
        {
            if (canvas == null || _disposed)
                return;
            canvas.Clear(SKColors.Transparent);
            if (state == null || !state.IsVisible || state.Definition == null || state.CurrentPage == null)
                return;

            _textPaint.SyncBlobCache(UseTextBlobCaching);
            BeginBadgePaint();
            GHRadialMenuDefinition def = state.Definition;
            UpdateReserve(def, info.Width, info.Height, dipScale);
            float reserveTop = _reserveTop;
            float reserveBottom = _reserveBottom;
            GHRadialGeometry g = state.Geometry;
            g.Update(info.Width, info.Height - reserveTop - reserveBottom, dipScale, HintScale);
            state.GeometryOffsetY = reserveTop;
            if (!g.IsValid)
                return;
            EnsureResources(g);
            ResolveAccent(def.Accent);
            _sliceBaseFill = def.SeeThrough ? SeeThroughSliceFill : SliceFill;
            _hubBaseFill = def.SeeThrough ? SeeThroughHubFill : HubFill;
            _sliceGlow = !def.SeeThrough;
            if (def.SeeThrough)
            {
                _accentHighlightFill = _accentHighlightFill.WithAlpha(SeeThroughHighlightAlpha);
                _accentFlashFill = _accentFlashFill.WithAlpha(SeeThroughFlashAlpha);
            }
            EnsureLayouts(def, g);

            float opacity = state.Opacity;
            bool still = !state.IsAnimating;

            _plainFill.Color = new SKColor(0, 0, 0, ToByte(state.BackdropOpacity));
            canvas.DrawRect(0, 0, g.Width, g.Height + reserveTop + reserveBottom, _plainFill);
            /* From here on in the geometry's coordinates */
            bool shifted = reserveTop != 0f;
            if (shifted)
            {
                canvas.Save();
                canvas.Translate(0, reserveTop);
            }
            if (!def.SeeThrough)
            {
                _vignettePaint.Color = new SKColor(255, 255, 255, ToByte(opacity));
                canvas.DrawCircle(g.CenterX, g.CenterY, (float)(GHRadialTuning.VignetteRadius * g.OuterRadius), _vignettePaint);
            }

            DrawHeader(canvas, state, def, g, opacity, still);
            DrawChips(canvas, state, def, g, opacity, still);
            HintBar hints = GetHintBar(state, def, g);
            float hintCenterY = g.HintCenterY + reserveBottom;
            if (hints.FontSize > g.HintFontSize)
            {
                /* A larger bar stays above the screen margin */
                float margin = (float)(GHRadialTuning.ScreenMarginDip * g.DipScale);
                hintCenterY = Math.Min(hintCenterY, g.Height + reserveBottom - margin - BadgeHeightAt(hints.FontSize) / 2);
            }
            GHRadialItem highlighted = state.HighlightedItem;
            bool dimConfirm = def.OnItemAction != null && highlighted != null && !highlighted.IsEnabled;
            DrawHintBar(canvas, hints, def.Style, g, hintCenterY, opacity, still, dimConfirm);
            if (_reserveFooter)
                DrawFooter(canvas, state, def, g, g.CenterY + g.OuterRadius + g.HighlightOffset,
                    hintCenterY - BadgeHeightAt(hints.FontSize) / 2, opacity, still);

            if (opacity < 0.997f)
            {
                _layerPaint.Color = new SKColor(255, 255, 255, ToByte(opacity));
                canvas.SaveLayer(_ringBounds, _layerPaint);
            }
            else
            {
                canvas.Save();
            }
            float scale = state.RingScale;
            if (scale != 1f)
                canvas.Scale(scale, scale, g.CenterX, g.CenterY);

            /* The page object, which after ReplaceDefinition belongs to the replaced definition */
            GHRadialPage prevPage = state.PreviousPage;
            if (prevPage != null)
            {
                float t = GHRadialMenuState.SmoothStep(state.PageTransition);
                float rot = (float)GHRadialTuning.PageChangeRotationDegrees * state.PageDirection;
                DrawRing(canvas, state, g, prevPage, false, 1f - t, -rot * t, state.PreviousPageHighlight, false);
                DrawRing(canvas, state, g, state.CurrentPage, true, t, rot * (1f - t), state.HighlightedSlot, false);
            }
            else
            {
                DrawRing(canvas, state, g, state.CurrentPage, true, 1f, 0f, state.HighlightedSlot, still && scale == 1f);
            }
            DrawHub(canvas, state, def, g, still && scale == 1f);
            canvas.Restore();
            if (shifted)
                canvas.Restore();
        }

        /* Decides the tab row above and the description band below the geometry. Both scale with the outer
           radius, which is itself 0.4 of the shorter side, so the radius is solved for the canvas less the rows. */
        private void UpdateReserve(GHRadialMenuDefinition def, float width, float height, float dipScale)
        {
            if (!ReferenceEquals(def, _reserveDefinition))
            {
                _reserveDefinition = def;
                _reserveTabs = def.HasSetTabs;
                _reserveFooter = def.HasFooters || (def.FooterMaxLines > 0 && HasFooterTitles(def));
                /* One row more for the band's title row when any item has one */
                _reserveFooterLines = Math.Max(0, def.FooterMaxLines) + (HasFooterTitles(def) ? 1 : 0);
            }
            _reserveTop = 0f;
            _reserveBottom = 0f;
            if (!_reserveTabs && !_reserveFooter)
                return;

            if (_reserveFooter && !ReferenceEquals(DescriptionTypeface, _spacingTypeface))
            {
                _spacingTypeface = DescriptionTypeface;
                _textPaint.Typeface = DescriptionTypeface;
                _textPaint.TextSize = 100f;
                float ratio = _textPaint.FontSpacing / 100f;
                _descriptionSpacingRatio = ratio > 0f ? ratio : 1.2f;
            }

            double kTop = _reserveTabs
                ? (1.0 + GHRadialTuning.SetTabGapOfHeight) * GHRadialTuning.SetTabHeightOfTitleFont * GHRadialTuning.PageTitleFont : 0.0;
            double kBottom = _reserveFooter
                ? (_reserveFooterLines + GHRadialTuning.FooterGapOfLine) * _descriptionSpacingRatio
                  * GHRadialTuning.FooterFontOfDescriptionFont * GHRadialTuning.HubDescriptionFont : 0.0;
            double k = kTop + kBottom;
            if (!(width > 0) || !(height > 0) || !(k > 0))
                return;
            double ds = dipScale > 0 ? dipScale : 1.0;
            double fixedRadius = Math.Min(GHRadialTuning.OuterRadiusOfShortSide * width, GHRadialTuning.OuterRadiusMaxDip * ds);
            double heightRadius = GHRadialTuning.OuterRadiusOfShortSide * height / (1.0 + GHRadialTuning.OuterRadiusOfShortSide * k);
            double ro = Math.Max(0.0, Math.Min(fixedRadius, heightRadius));
            _reserveTop = (float)Math.Round(kTop * ro);
            _reserveBottom = (float)(kBottom * ro);
        }

        /* True when the fixed hub item or any slot item has a FooterTitle */
        private static bool HasFooterTitles(GHRadialMenuDefinition def)
        {
            if (def.FixedHubItem != null && def.FixedHubItem.FooterTitle != null)
                return true;
            int i;
            for (i = 0; i < def.PageCount; i++)
            {
                GHRadialPage page = def.Pages[i];
                if (page == null)
                    continue;
                int k;
                for (k = 0; k < page.SlotCount; k++)
                {
                    GHRadialItem item = page.Slots[k];
                    if (item != null && item.FooterTitle != null)
                        return true;
                }
            }
            return false;
        }

        private void DrawRing(SKCanvas canvas, GHRadialMenuState state, GHRadialGeometry g, GHRadialPage page, bool isCurrent,
            float alpha, float rotation, int highlightSlot, bool snap)
        {
            if (alpha <= 0.003f)
                return;
            int n = Math.Min(page.SlotCount, MaxCachedSlotCount);
            SKPath[] paths = GetSlicePaths(n, g);

            if (alpha < 0.997f)
            {
                _layerPaint.Color = new SKColor(255, 255, 255, ToByte(alpha));
                canvas.SaveLayer(_ringBounds, _layerPaint);
            }
            else
            {
                canvas.Save();
            }
            if (rotation != 0f)
                canvas.RotateDegrees(rotation, g.CenterX, g.CenterY);

            int k;
            for (k = 0; k < n; k++)
            {
                if (page.Slots[k] != null)
                    continue;
                canvas.Save();
                canvas.Translate(g.CenterX, g.CenterY);
                canvas.DrawPath(paths[k], _emptyStroke);
                canvas.Restore();
            }
            /* Highlighted slices last, so that their glow and offset draw over their neighbors */
            GHRadialItem carried = isCurrent && state.Definition != null ? state.Definition.CarriedItem : null;
            int pass;
            for (pass = 0; pass < 2; pass++)
            {
                for (k = 0; k < n; k++)
                {
                    GHRadialItem item = page.Slots[k];
                    if (item == null)
                        continue;
                    float w = isCurrent ? state.GetHighlightWeight(k) : (k == highlightSlot ? 1f : 0f);
                    if ((pass == 0) != (w <= 0f))
                        continue;
                    /* The slot the carried item would swap into dims under its ghost */
                    float contentAlpha = carried != null && k == highlightSlot && item.ShowsSwapMark ? (float)GHRadialTuning.SwapOccupantOpacity : 1f;
                    DrawSlice(canvas, state, g, paths[k], item, k, n, w, isCurrent, snap, contentAlpha);
                }
            }
            if (carried != null)
                DrawCarriedGhost(canvas, state, g, page, carried, n, highlightSlot);
            canvas.Restore();
        }

        /* The carried item's icon at CarriedGhostOpacity over the highlighted slot's icon, and a swap mark when the
           occupant shows one */
        private void DrawCarriedGhost(SKCanvas canvas, GHRadialMenuState state, GHRadialGeometry g, GHRadialPage page, GHRadialItem carried,
            int n, int slot)
        {
            if (slot < 0 || slot >= n)
                return;
            float e = GHRadialMenuState.SmoothStep(state.GetHighlightWeight(slot));
            double rad = GHRadialGeometry.SlotCenterDegrees(slot, n) * Math.PI / 180.0;
            float dirX = (float)Math.Sin(rad);
            float dirY = -(float)Math.Cos(rad);
            float radius = g.MidRadius + g.HighlightOffset * e;
            float px = g.CenterX + dirX * radius;
            float py = g.CenterY + dirY * radius;
            float iconSize = g.GetIconSize(n) * (1f + ((float)GHRadialTuning.HighlightIconScale - 1f) * e);
            /* Over the occupant's icon, which its label's lines push up */
            GHRadialItem occupant = page.GetItem(slot);
            float labelHeight = g.LabelFontSize * (float)GHRadialTuning.LabelLineHeight;
            if (occupant != null)
                labelHeight = LabelBlockHeight(GetItemLayout(occupant, n, slot, g));
            float top = py - (iconSize + labelHeight) / 2;
            SKRect dest = new SKRect(px - iconSize / 2, top, px + iconSize / 2, top + iconSize);

            _contentLayerPaint.Color = SKColors.White.WithAlpha(ToByte((float)GHRadialTuning.CarriedGhostOpacity));
            int layer = canvas.SaveLayer(new SKRect(dest.Left - 2, dest.Top - 2, dest.Right + 2, dest.Bottom + 2), _contentLayerPaint);
            DrawIcon(canvas, carried, dest, true, false, false);
            canvas.RestoreToCount(layer);

            if (occupant != null && occupant.ShowsSwapMark)
                DrawSwapMark(canvas, g, dest);
        }

        /* Two opposed arrows on a dark disc at the icon's lower right corner */
        private void DrawSwapMark(SKCanvas canvas, GHRadialGeometry g, SKRect icon)
        {
            float s = icon.Width * 0.42f;
            float cx = icon.Right - s * 0.35f;
            float cy = icon.Bottom - s * 0.35f;
            _plainFill.Color = HubFill;
            canvas.DrawCircle(cx, cy, s * 0.62f, _plainFill);
            _markStroke.Color = _accent;
            _markStroke.StrokeWidth = Math.Max(1.5f * g.DipScale, s * 0.1f);
            float half = s * 0.38f;
            float rise = s * 0.17f;
            float head = s * 0.17f;
            float y = cy - rise;
            canvas.DrawLine(cx - half, y, cx + half, y, _markStroke);
            canvas.DrawLine(cx + half, y, cx + half - head, y - head, _markStroke);
            canvas.DrawLine(cx + half, y, cx + half - head, y + head, _markStroke);
            y = cy + rise;
            canvas.DrawLine(cx + half, y, cx - half, y, _markStroke);
            canvas.DrawLine(cx - half, y, cx - half + head, y - head, _markStroke);
            canvas.DrawLine(cx - half, y, cx - half + head, y + head, _markStroke);
        }

        private void DrawSlice(SKCanvas canvas, GHRadialMenuState state, GHRadialGeometry g, SKPath path, GHRadialItem item,
            int slot, int n, float weight, bool isCurrent, bool snap, float contentAlpha)
        {
            float e = GHRadialMenuState.SmoothStep(weight);
            double rad = GHRadialGeometry.SlotCenterDegrees(slot, n) * Math.PI / 180.0;
            float dirX = (float)Math.Sin(rad);
            float dirY = -(float)Math.Cos(rad);
            float offset = g.HighlightOffset * e;
            bool enabled = item.IsEnabled;

            canvas.Save();
            canvas.Translate(g.CenterX + dirX * offset, g.CenterY + dirY * offset);
            /* The blurred glow fills the whole slice, so a see-through slice goes without it */
            if (e > 0f && enabled && _sliceGlow)
            {
                _glowPaint.Color = _accent.WithAlpha(ToByte(e * 0x60 / 255f));
                canvas.DrawPath(path, _glowPaint);
            }
            SKColor fill = LerpColor(_sliceBaseFill, _accentHighlightFill, enabled ? e : e * 0.5f);
            float flash = isCurrent && state.FlashSlot == slot ? state.FlashIntensity : 0f;
            if (flash > 0f)
                fill = LerpColor(fill, _accentFlashFill, flash);
            SKColor border = LerpColor(SliceBorder, enabled ? _accent : DisabledBorder, e);
            /* A tinted slice leans toward its color, less while highlighted so that the highlight shows through */
            if (item.SliceTintNhColor >= 0 && NhColorLookup != null)
            {
                SKColor tint = GetNhColor(item.SliceTintNhColor, fill);
                float keep = 1f - (1f - (float)GHRadialTuning.SliceTintHighlightKeep) * e;
                fill = TintColor(fill, tint, (float)GHRadialTuning.SliceTintOfFill * keep);
                border = TintColor(border, tint, (float)GHRadialTuning.SliceTintOfBorder * keep);
            }
            _sliceFill.Color = fill;
            canvas.DrawPath(path, _sliceFill);
            /* A placeholder's outline is dashed */
            SKPaint stroke = item.IsPlaceholder ? _placeholderStroke : _sliceStroke;
            stroke.Color = border;
            stroke.StrokeWidth = g.DipScale * (1f + ((float)GHRadialTuning.HighlightBorderDip - 1f) * e);
            canvas.DrawPath(path, stroke);
            canvas.Restore();

            DrawSlotContents(canvas, g, item, slot, n, dirX, dirY, offset, e, enabled, snap, contentAlpha);
        }

        private void DrawSlotContents(SKCanvas canvas, GHRadialGeometry g, GHRadialItem item, int slot, int n, float dirX, float dirY,
            float offset, float e, bool enabled, bool snap, float contentAlpha)
        {
            bool isToggle = item.IsToggle;
            bool isOn = isToggle && item.IsOn;
            ItemLayout layout = GetItemLayout(item, n, slot, g);

            float radius = g.MidRadius + offset - (isToggle ? (float)(GHRadialTuning.ToggleContentInset * g.OuterRadius) : 0f);
            float px = g.CenterX + dirX * radius;
            float py = g.CenterY + dirY * radius;
            float iconSize = g.GetIconSize(n) * (1f + ((float)GHRadialTuning.HighlightIconScale - 1f) * e);

            _textPaint.Typeface = LabelTypeface;
            _textPaint.TextSize = layout.LabelSize;
            float labelAscent = -_textPaint.FontMetrics.Ascent;
            float labelLineHeight = layout.LabelSize * (float)GHRadialTuning.LabelLineHeight;
            float labelHeight = LabelBlockHeight(layout);

            float top = py - (iconSize + labelHeight) / 2;
            SKRect dest = new SKRect(px - iconSize / 2, top, px + iconSize / 2, top + iconSize);

            /* A dimmed slot draws its icon and label through a layer, which also dims a host-drawn icon */
            int layer = -1;
            if (contentAlpha < 0.997f)
            {
                float halfW = Math.Max(iconSize, Math.Max(g.GetLabelMaxWidth(n), layout.LabelMaxWidth)) * 0.6f;
                float halfH = (iconSize + labelHeight) * 0.6f;
                _contentLayerPaint.Color = SKColors.White.WithAlpha(ToByte(contentAlpha));
                layer = canvas.SaveLayer(new SKRect(px - halfW, py - halfH, px + halfW, py + halfH), _contentLayerPaint);
            }

            bool drawn;
            if (item.IsPlaceholder)
            {
                DrawPlus(canvas, g, dest, e);
                drawn = true;
            }
            else
            {
                /* IconGrayed draws the icon as a disabled item's */
                drawn = DrawIcon(canvas, item, dest, enabled && !item.IconGrayed, e > 0.5f, isOn);
            }
            if (drawn)
                top += iconSize;
            else
                top = py - labelHeight / 2;

            int i;
            for (i = 0; i < layout.LabelLineCount; i++)
            {
                string line = layout.LabelLines[i];
                if (string.IsNullOrEmpty(line))
                    continue;
                _textPaint.Typeface = LabelTypeface;
                _textPaint.TextSize = layout.LabelSize;
                _textPaint.Color = item.IsPlaceholder ? DisabledLabel : GetItemColor(item, enabled ? SKColors.White : DisabledLabel);
                float tx = px;
                float ty = top + labelAscent + i * labelLineHeight;
                if (snap)
                {
                    tx = Snap(tx);
                    ty = Snap(ty);
                }
                _textPaint.DrawTextOnCanvas(canvas, line, tx, ty, SKTextAlign.Center);
            }

            if (layer >= 0)
                canvas.RestoreToCount(layer);

            if (isToggle)
                DrawPill(canvas, g, dirX, dirY, offset, isOn, snap);
            if (item.Badge != GHRadialBadge.None)
                DrawSlotBadge(canvas, g, item, layout, slot, n, dirX, dirY, offset, enabled, snap);
        }

        /* The host's icon, else the icon image; false when there is neither */
        private bool DrawIcon(SKCanvas canvas, GHRadialItem item, SKRect dest, bool enabled, bool highlighted, bool isOn)
        {
            Func<SKCanvas, GHRadialItem, SKRect, bool, bool, bool> drawer = CustomIconDrawer;
            if (drawer != null && drawer(canvas, item, dest, enabled, highlighted))
                return true;
            Func<string, SKImage> lookup = IconLookup;
            SKImage image = lookup == null ? null : lookup(item.GetIconPath(isOn));
            if (image == null)
                return false;
            _iconPaint.ColorFilter = !enabled ? UIUtils.GrayedColorFilter : (highlighted ? UIUtils.HighlightColorFilter : null);
            _iconPaint.Color = SKColors.White;
            canvas.DrawImage(image, dest
#if GNH_MAUI
                , IconSampling
#endif
                , _iconPaint);
            return true;
        }

        /* A placeholder's plus, two strokes in the disabled-label gray, lighter while highlighted */
        private void DrawPlus(SKCanvas canvas, GHRadialGeometry g, SKRect dest, float e)
        {
            float cx = dest.MidX;
            float cy = dest.MidY;
            float half = dest.Width * 0.25f;
            _markStroke.Color = LerpColor(DisabledLabel, LighterGray, e);
            _markStroke.StrokeWidth = Math.Max(1.5f * g.DipScale, dest.Width * 0.06f);
            canvas.DrawLine(cx - half, cy, cx + half, cy, _markStroke);
            canvas.DrawLine(cx, cy - half, cx, cy + half, _markStroke);
        }

        /* A Star, New or Count badge at the slice's outer edge, clockwise of its center line */
        private void DrawSlotBadge(SKCanvas canvas, GHRadialGeometry g, GHRadialItem item, ItemLayout layout, int slot, int n,
            float dirX, float dirY, float offset, bool enabled, bool snap)
        {
            float h = SlotBadgeHeight(g);
            float r = g.OuterRadius - (float)(GHRadialTuning.PillInset * g.OuterRadius);
            double a = (GHRadialGeometry.SlotCenterDegrees(slot, n) + BadgeAngleOfHalfSlot * 180.0 / Math.Max(1, n)) * Math.PI / 180.0;
            float bx = g.CenterX + dirX * offset + (float)Math.Sin(a) * r;
            float by = g.CenterY + dirY * offset - (float)Math.Cos(a) * r;
            byte alpha = enabled ? (byte)0xFF : (byte)0x80;
            switch (item.Badge)
            {
            case GHRadialBadge.Star:
                if (_starPath == null)
                    break;
                canvas.Save();
                canvas.Translate(bx, by);
                _plainFill.Color = Gold.WithAlpha(alpha);
                canvas.DrawPath(_starPath, _plainFill);
                _plainStroke.Color = StarOutline.WithAlpha(alpha);
                _plainStroke.StrokeWidth = g.DipScale;
                canvas.DrawPath(_starPath, _plainStroke);
                canvas.Restore();
                break;
            case GHRadialBadge.New:
                DrawBadgePill(canvas, g, bx, by, h, NewBadgeText, _newBadgeWidth, OnGreen.WithAlpha(alpha), PillDarkText.WithAlpha(alpha), false, snap);
                break;
            case GHRadialBadge.Count:
                if (!string.IsNullOrEmpty(item.BadgeText))
                    DrawBadgePill(canvas, g, bx, by, h, item.BadgeText, layout.BadgeTextWidth, CountBadgeFill.WithAlpha((byte)(CountBadgeFill.Alpha * alpha / 255)),
                        SKColors.White.WithAlpha(alpha), true, snap);
                break;
            default:
                break;
            }
        }

        private void DrawBadgePill(SKCanvas canvas, GHRadialGeometry g, float cx, float cy, float h, string text, float textWidth,
            SKColor fill, SKColor textColor, bool outline, bool snap)
        {
            float w = Math.Max(h, textWidth + h * 0.7f);
            SKRect rect = new SKRect(cx - w / 2, cy - h / 2, cx + w / 2, cy + h / 2);
            _plainFill.Color = fill;
            canvas.DrawRoundRect(rect, h / 2, h / 2, _plainFill);
            if (outline)
            {
                _plainStroke.Color = LighterGray.WithAlpha(textColor.Alpha);
                _plainStroke.StrokeWidth = g.DipScale;
                canvas.DrawRoundRect(rect, h / 2, h / 2, _plainStroke);
            }
            _textPaint.Typeface = BoldTypeface;
            _textPaint.TextSize = h * BadgeTextOfHeight;
            _textPaint.Color = textColor;
            SKFontMetrics m = _textPaint.FontMetrics;
            float ty = cy - (m.Ascent + m.Descent) / 2;
            float tx = cx;
            if (snap)
            {
                tx = Snap(tx);
                ty = Snap(ty);
            }
            _textPaint.DrawTextOnCanvas(canvas, text, tx, ty, SKTextAlign.Center);
        }

        private static float SlotBadgeHeight(GHRadialGeometry g)
        {
            return (float)(GHRadialTuning.BadgeSizeOfLabelFont * g.LabelFontSize);
        }

        /* The label's lines; at least one line, as the icon's placement leaves room for a label */
        private static float LabelBlockHeight(ItemLayout layout)
        {
            return layout.LabelSize * (float)GHRadialTuning.LabelLineHeight * Math.Max(1, layout.LabelLineCount);
        }

        /* The item's NhColor through NhColorLookup, or the default */
        private SKColor GetItemColor(GHRadialItem item, SKColor defaultColor)
        {
            return GetNhColor(item == null ? -1 : item.NhColor, defaultColor);
        }

        private SKColor GetNhColor(int nhColor, SKColor defaultColor)
        {
            Func<int, SKColor> lookup = NhColorLookup;
            return nhColor >= 0 && lookup != null ? lookup(nhColor) : defaultColor;
        }

        /* ON (green) or OFF (outline) */
        private void DrawPill(SKCanvas canvas, GHRadialGeometry g, float dirX, float dirY, float offset, bool isOn, bool snap)
        {
            float r = g.OuterRadius + offset - (float)(GHRadialTuning.PillInset * g.OuterRadius);
            float px = g.CenterX + dirX * r;
            float py = g.CenterY + dirY * r;
            float fs = g.PillFontSize;
            float h = fs * 1.5f;
            float textWidth = !isOn ? _pillOffWidth : _pillOnWidth;
            string text = !isOn ? PillOffText : PillOnText;
            float w = textWidth + fs * 1.2f;
            SKRect rect = new SKRect(px - w / 2, py - h / 2, px + w / 2, py + h / 2);
            if (isOn)
            {
                _plainFill.Color = OnGreen;
                canvas.DrawRoundRect(rect, h / 2, h / 2, _plainFill);
            }
            else
            {
                _plainStroke.Color = TextGray;
                _plainStroke.StrokeWidth = g.DipScale;
                canvas.DrawRoundRect(rect, h / 2, h / 2, _plainStroke);
            }
            _textPaint.Typeface = BoldTypeface;
            _textPaint.TextSize = fs;
            _textPaint.Color = isOn ? PillDarkText : TextGray;
            SKFontMetrics m = _textPaint.FontMetrics;
            float ty = py - (m.Ascent + m.Descent) / 2;
            float tx = px;
            if (snap)
            {
                tx = Snap(tx);
                ty = Snap(ty);
            }
            _textPaint.DrawTextOnCanvas(canvas, text, tx, ty, SKTextAlign.Center);
        }

        private void DrawHub(SKCanvas canvas, GHRadialMenuState state, GHRadialMenuDefinition def, GHRadialGeometry g, bool snap)
        {
            float cx = g.CenterX;
            float cy = g.CenterY;
            _plainFill.Color = _hubBaseFill;
            canvas.DrawCircle(cx, cy, g.HubRadius, _plainFill);
            _plainStroke.Color = _accent;
            _plainStroke.StrokeWidth = (float)(GHRadialTuning.HubBorderDip * g.DipScale);
            canvas.DrawCircle(cx, cy, g.HubRadius, _plainStroke);

            if (state.WedgeOpacity > 0f)
            {
                canvas.Save();
                canvas.RotateDegrees(state.WedgeAngle, cx, cy);
                canvas.Translate(cx, cy - g.HubRadius);
                _plainFill.Color = _accent.WithAlpha(ToByte(state.WedgeOpacity));
                canvas.DrawPath(_wedgePath, _plainFill);
                canvas.Restore();
            }

            if (state.ShowStickGhost)
            {
                float reach = (float)(GHRadialTuning.StickGhostOfHub * g.HubRadius);
                _plainFill.Color = StickGhostColor;
                canvas.DrawCircle(cx + state.StickX * reach, cy + state.StickY * reach,
                    (float)(GHRadialTuning.StickGhostDotOfHub * g.HubRadius), _plainFill);
            }

            GHRadialPage page = state.CurrentPage;
            GHRadialItem item = state.HighlightedItem;
            if (_hasPrompt)
                DrawHubPrompt(canvas, state, def, g, snap);
            else if (def.FixedHubItem != null)
                DrawHubFixedItem(canvas, g, def.FixedHubItem, GetItemLayout(def.FixedHubItem, page.SlotCount, -1, g), snap);
            else if (item != null)
                DrawHubItem(canvas, def, g, item, GetItemLayout(item, page.SlotCount, -1, g), snap);
            else
                DrawHubIdle(canvas, state, def, g, page, snap);
        }

        /* A direction prompt's hub: the verb and the object centered as a block, and above them the highlighted
           direction or the idle text, which does not move the block */
        private void DrawHubPrompt(SKCanvas canvas, GHRadialMenuState state, GHRadialMenuDefinition def, GHRadialGeometry g, bool snap)
        {
            GHRadialItem item = state.HighlightedItem;
            string top = item != null ? item.Label : state.IsPointerMode ? def.NoHighlightPointerText : def.NoHighlightStickText;
            float cx = g.CenterX;
            float verbBaseline, objectTop, topCenterY;
            GetPromptPositions(g, out verbBaseline, out objectTop, out topCenterY);

            _textPaint.Typeface = LabelTypeface;
            _textPaint.TextSize = _promptTopSize;
            _textPaint.Color = _accentTitle;
            SKFontMetrics m = _textPaint.FontMetrics;
            DrawCentered(canvas, top, cx, topCenterY - (m.Ascent + m.Descent) / 2, snap);

            _textPaint.Typeface = TitleTypeface;
            _textPaint.TextSize = _promptVerbSize;
            _textPaint.Color = SKColors.White;
            DrawCentered(canvas, _promptVerb, cx, verbBaseline, snap);

            if (_promptObjectLineCount > 0)
            {
                int i;
                float objectAscent;
                float objectH = LineHeight(DescriptionTypeface, g.HubDescriptionFontSize, out objectAscent);
                float y = objectTop;
                _textPaint.Color = LighterGray;
                for (i = 0; i < _promptObjectLineCount; i++)
                {
                    DrawCentered(canvas, _promptObjectLines[i], cx, y + objectAscent, snap);
                    y += objectH;
                }
            }
        }

        /* The verb's baseline and the top of the object lines, the two centered as a block, and the center line of the
           top text, halfway between the top of the hub and the top of the verb's capitals */
        private void GetPromptPositions(GHRadialGeometry g, out float verbBaseline, out float objectTop, out float topCenterY)
        {
            float verbAscent;
            float verbH = LineHeight(TitleTypeface, _promptVerbSize, out verbAscent);
            float capHeight = _textPaint.FontMetrics.CapHeight;
            if (!(capHeight > 0f))
                capHeight = verbAscent;
            float objectH = _promptObjectLineCount > 0 ? LineHeight(DescriptionTypeface, g.HubDescriptionFontSize, out _) : 0f;
            float y = g.CenterY - (verbH + objectH * _promptObjectLineCount) / 2;
            verbBaseline = y + verbAscent;
            objectTop = y + verbH;
            topCenterY = (g.CenterY - g.HubRadius + verbBaseline - capHeight) / 2;
        }

        /* The fixed hub item's icon with its label under it, centered as a block */
        private void DrawHubFixedItem(SKCanvas canvas, GHRadialGeometry g, GHRadialItem item, ItemLayout layout, bool snap)
        {
            float iconSize = HubFixedIconOfHub * g.HubRadius;
            float titleAscent;
            float titleLineH = LineHeight(TitleTypeface, layout.HubTitleSize, out titleAscent);
            float titleH = titleLineH * Math.Max(1, layout.HubTitleLineCount);
            /* The title line's leading already separates it from the icon, so the label sits slightly up into it */
            float gap = -iconSize * HubFixedTitleRaiseOfIcon;
            float top = g.CenterY - (iconSize + gap + titleH) / 2;
            SKRect dest = new SKRect(g.CenterX - iconSize / 2, top, g.CenterX + iconSize / 2, top + iconSize);
            bool drawn = DrawIcon(canvas, item, dest, true, false, false);
            float y = drawn ? top + iconSize + gap : g.CenterY - titleH / 2;
            _textPaint.Typeface = TitleTypeface;
            _textPaint.TextSize = layout.HubTitleSize;
            _textPaint.Color = GetItemColor(item, SKColors.White);
            DrawHubTitleLines(canvas, layout, g.CenterX, y + titleAscent, titleLineH, snap);
        }

        /* The title's lines from the first baseline down, in the paint's current typeface, size and color */
        private void DrawHubTitleLines(SKCanvas canvas, ItemLayout layout, float cx, float firstBaseline, float lineH, bool snap)
        {
            int i;
            for (i = 0; i < layout.HubTitleLineCount; i++)
                DrawCentered(canvas, layout.HubTitleLines[i], cx, firstBaseline + i * lineH, snap);
        }

        /* The title, then the HubLines or the description lines, then the status and the shortcut, centered as a block;
           a HubTopLine sits halfway between the top of the hub and the top of the title's capitals */
        private void DrawHubItem(SKCanvas canvas, GHRadialMenuDefinition def, GHRadialGeometry g, GHRadialItem item, ItemLayout layout, bool snap)
        {
            bool showStatus = item.IsEnabled ? item.IsToggle : item.ShowsUnavailableText;
            bool showShortcut = ShowKeyboardShortcuts && !string.IsNullOrEmpty(item.ShortcutText);
            float descSize = g.HubDescriptionFontSize;
            if (item.HubTopLine != null || item.HubLines != null)
                EnsureHubLines(item, layout, g, (float)def.HubLineMinScale, (float)def.HubLinesGapOfFont);
            bool useHubLines = item.HubLines != null && layout.HubLines != null;

            float titleAscent;
            float descAscent = 0f;
            float statusAscent = 0f;
            float descH = 0f;
            float statusH = 0f;
            float titleLineH = LineHeight(TitleTypeface, layout.HubTitleSize, out titleAscent);
            float titleH = titleLineH * Math.Max(1, layout.HubTitleLineCount);
            if (!useHubLines && layout.DescriptionLineCount > 0)
                descH = LineHeight(DescriptionTypeface, descSize, out descAscent);
            if (showStatus || showShortcut)
                statusH = LineHeight(LabelTypeface, _statusSize, out statusAscent);

            float linesH = useHubLines ? HubLinesHeight(layout) : descH * layout.DescriptionLineCount;
            float total = titleH + linesH + (showStatus ? statusH : 0f) + (showShortcut ? statusH : 0f);
            float y = g.CenterY - total / 2;
            float cx = g.CenterX;

            if (layout.TopLine != null && item.HubTopLine != null)
                DrawHubLine(canvas, layout.TopLine, cx, HubTopLineBaseline(g, layout, y + titleAscent), LighterGray, snap);

            _textPaint.Typeface = TitleTypeface;
            _textPaint.TextSize = layout.HubTitleSize;
            _textPaint.Color = GetItemColor(item, SKColors.White);
            DrawHubTitleLines(canvas, layout, cx, y + titleAscent, titleLineH, snap);
            y += titleH;

            int i;
            if (useHubLines)
            {
                y += layout.HubLinesGap;
                for (i = 0; i < layout.HubLines.Length; i++)
                {
                    HubLineLayout line = layout.HubLines[i];
                    float lineAscent;
                    float lineH = LineHeight(line.Typeface, line.Size, out lineAscent);
                    DrawHubLine(canvas, line, cx, y + lineAscent, LighterGray, snap);
                    y += lineH;
                }
            }
            else if (layout.DescriptionLineCount > 0)
            {
                _textPaint.Typeface = DescriptionTypeface;
                _textPaint.TextSize = descSize;
                _textPaint.Color = LighterGray;
                for (i = 0; i < layout.DescriptionLineCount; i++)
                {
                    DrawCentered(canvas, layout.DescriptionLines[i], cx, y + descAscent, snap);
                    y += descH;
                }
            }

            if (showStatus)
            {
                float baseline = y + statusAscent;
                if (!item.IsEnabled)
                {
                    _textPaint.Typeface = LabelTypeface;
                    _textPaint.TextSize = _statusSize;
                    _textPaint.Color = TextGray;
                    DrawCentered(canvas, def.UnavailableText, cx, baseline, snap);
                }
                else
                {
                    bool isOn = item.IsOn;
                    float valueWidth = isOn ? _onWidth : _offWidth;
                    float x = cx - (_currentlyWidth + valueWidth) / 2;
                    if (snap)
                    {
                        x = Snap(x);
                        baseline = Snap(baseline);
                    }
                    _textPaint.Typeface = LabelTypeface;
                    _textPaint.TextSize = _statusSize;
                    _textPaint.Color = LighterGray;
                    _textPaint.DrawTextOnCanvas(canvas, CurrentlyText, x, baseline, SKTextAlign.Left);
                    _textPaint.Typeface = BoldTypeface;
                    _textPaint.Color = isOn ? OnGreen : TextGray;
                    _textPaint.DrawTextOnCanvas(canvas, isOn ? OnText : OffText, x + _currentlyWidth, baseline, SKTextAlign.Left);
                }
                y += statusH;
            }

            if (showShortcut)
            {
                _textPaint.Typeface = LabelTypeface;
                _textPaint.TextSize = _statusSize;
                _textPaint.Color = TextGray;
                DrawCentered(canvas, item.ShortcutText, cx, y + statusAscent, snap);
            }
        }

        /* The HubLines and the gap above them */
        private float HubLinesHeight(ItemLayout layout)
        {
            float h = layout.HubLinesGap;
            int i;
            for (i = 0; i < layout.HubLines.Length; i++)
            {
                HubLineLayout line = layout.HubLines[i];
                h += LineHeight(line.Typeface, line.Size, out _);
            }
            return h;
        }

        /* The top line's baseline: its center halfway between the top of the hub and the top of the title's capitals */
        private float HubTopLineBaseline(GHRadialGeometry g, ItemLayout layout, float titleBaseline)
        {
            _textPaint.Typeface = TitleTypeface;
            _textPaint.TextSize = layout.HubTitleSize;
            SKFontMetrics tm = _textPaint.FontMetrics;
            float capHeight = tm.CapHeight > 0f ? tm.CapHeight : -tm.Ascent;
            float centerY = (g.CenterY - g.HubRadius + titleBaseline - capHeight) / 2;
            HubLineLayout top = layout.TopLine;
            _textPaint.Typeface = top.Typeface;
            _textPaint.TextSize = top.Size;
            SKFontMetrics m = _textPaint.FontMetrics;
            return centerY - (m.Ascent + m.Descent) / 2;
        }

        /* The hub's block height as DrawHubItem lays it out, for the top line's width */
        private float MeasureHubItemBlock(GHRadialGeometry g, GHRadialItem item, ItemLayout layout, out float titleAscent)
        {
            bool showStatus = item.IsEnabled ? item.IsToggle : item.ShowsUnavailableText;
            bool showShortcut = ShowKeyboardShortcuts && !string.IsNullOrEmpty(item.ShortcutText);
            float titleH = LineHeight(TitleTypeface, layout.HubTitleSize, out titleAscent) * Math.Max(1, layout.HubTitleLineCount);
            float linesH;
            if (item.HubLines != null && layout.HubLines != null)
                linesH = HubLinesHeight(layout);
            else
                linesH = layout.DescriptionLineCount > 0 ? LineHeight(DescriptionTypeface, g.HubDescriptionFontSize, out _) * layout.DescriptionLineCount : 0f;
            float statusH = showStatus || showShortcut ? LineHeight(LabelTypeface, _statusSize, out _) : 0f;
            return titleH + linesH + (showStatus ? statusH : 0f) + (showShortcut ? statusH : 0f);
        }

        /* Measures the item's HubLines across the description width and its HubTopLine across the hub's chord at
           its height, each shrinking to fit down to minScale of the size; the HubLines get gapOfFont of the hub line
           font above them */
        private void EnsureHubLines(GHRadialItem item, ItemLayout layout, GHRadialGeometry g, float minScale, float gapOfFont)
        {
            if (layout.HubLinesReady)
                return;
            layout.HubLinesReady = true;
            float descWidth = (float)(GHRadialTuning.HubDescriptionWidthOfHubDiameter * 2 * g.HubRadius);
            List<GHRadialHubRun[]> lines = item.HubLines;
            layout.HubLinesGap = 0f;
            if (lines != null)
            {
                layout.HubLines = new HubLineLayout[lines.Count];
                int i;
                for (i = 0; i < lines.Count; i++)
                    layout.HubLines[i] = MeasureHubLine(lines[i], DescriptionTypeface, g.HubDescriptionFontSize, descWidth, minScale);
                if (lines.Count > 0 && gapOfFont > 0f)
                    layout.HubLinesGap = gapOfFont * g.HubDescriptionFontSize;
            }
            if (item.HubTopLine != null)
            {
                /* Measured at the base size first, for its baseline */
                layout.TopLine = MeasureHubLine(item.HubTopLine, LabelTypeface, g.HubDescriptionFontSize, float.MaxValue, minScale);
                float titleAscent;
                float total = MeasureHubItemBlock(g, item, layout, out titleAscent);
                float baseline = HubTopLineBaseline(g, layout, g.CenterY - total / 2 + titleAscent);
                float rise = g.CenterY - baseline;
                float chord = 2 * (float)Math.Sqrt(Math.Max(0.0, (double)g.HubRadius * g.HubRadius - (double)rise * rise));
                float hubWidth = (float)(GHRadialTuning.HubTitleWidthOfHubDiameter * 2 * g.HubRadius);
                float topWidth = Math.Max(1f, Math.Min(hubWidth, (float)(GHRadialTuning.HubTitleWidthOfHubDiameter * chord)));
                layout.TopLine = MeasureHubLine(item.HubTopLine, LabelTypeface, g.HubDescriptionFontSize, topWidth, minScale);
            }
        }

        /* A line of runs at size, or smaller to fit maxWidth, down to minScale of size */
        private HubLineLayout MeasureHubLine(GHRadialHubRun[] runs, SKTypeface typeface, float size, float maxWidth, float minScale)
        {
            if (!(minScale > 0f) || minScale > 1f)
                minScale = 1f;
            HubLineLayout line = new HubLineLayout();
            int n = runs == null ? 0 : runs.Length;
            line.Runs = runs;
            line.Typeface = typeface;
            line.Size = size;
            line.SymbolWidths = new float[n];
            line.TextWidths = new float[n];
            MeasureHubRuns(line);
            if (line.TotalWidth > maxWidth && line.TotalWidth > 0f)
            {
                line.Size = Math.Max(size * minScale, size * maxWidth / line.TotalWidth);
                MeasureHubRuns(line);
            }
            return line;
        }

        /* A run's symbol is HubSymbolSizeOfLabelFont of the size tall; a run with a symbol after the first is set
           off by HubRunGapOfFont, and its text follows the symbol after HubSymbolGapOfFont */
        private void MeasureHubRuns(HubLineLayout line)
        {
            int n = line.SymbolWidths.Length;
            float size = line.Size;
            float symbolH = (float)(GHRadialTuning.HubSymbolSizeOfLabelFont * size);
            GHRadialSymbolLookup lookup = SymbolLookup;
            float total = 0f;
            int i;
            for (i = 0; i < n; i++)
            {
                GHRadialHubRun run = line.Runs[i];
                float sw = 0f;
                float tw = 0f;
                if (run != null)
                {
                    if (!string.IsNullOrEmpty(run.Symbol) && lookup != null)
                    {
                        SKRect src;
                        SKImage image = lookup(run.Symbol, out src);
                        if (image != null)
                        {
                            float srcW = src.Width > 0f ? src.Width : image.Width;
                            float srcH = src.Height > 0f ? src.Height : image.Height;
                            sw = srcH > 0f ? symbolH * srcW / srcH : 0f;
                        }
                    }
                    if (!string.IsNullOrEmpty(run.Text))
                    {
                        _textPaint.Typeface = line.Typeface;
                        _textPaint.TextSize = size;
                        tw = _textPaint.MeasureText(run.Text);
                    }
                }
                line.SymbolWidths[i] = sw;
                line.TextWidths[i] = tw;
                if (sw > 0f)
                {
                    if (i > 0)
                        total += size * HubRunGapOfFont;
                    total += sw + (tw > 0f ? size * HubSymbolGapOfFont : 0f);
                }
                total += tw;
            }
            line.TotalWidth = total;
        }

        /* A line of runs centered on cx; a run's symbol is centered on the line's cap height */
        private void DrawHubLine(SKCanvas canvas, HubLineLayout line, float cx, float baseline, SKColor defaultColor, bool snap)
        {
            DrawHubLine(canvas, line, cx, baseline, defaultColor, 1f, snap);
        }

        /* The same at an opacity, for a line drawn outside the ring's layer */
        private void DrawHubLine(SKCanvas canvas, HubLineLayout line, float cx, float baseline, SKColor defaultColor, float opacity, bool snap)
        {
            bool faded = opacity < 1f;
            byte alpha = ToByte(opacity);
            int n = line.SymbolWidths.Length;
            float size = line.Size;
            _textPaint.Typeface = line.Typeface;
            _textPaint.TextSize = size;
            SKFontMetrics m = _textPaint.FontMetrics;
            float capHeight = m.CapHeight > 0f ? m.CapHeight : -m.Ascent * 0.7f;
            float symbolH = (float)(GHRadialTuning.HubSymbolSizeOfLabelFont * size);
            float x = cx - line.TotalWidth / 2;
            if (snap)
                baseline = Snap(baseline);
            float symbolMid = baseline - capHeight / 2;
            GHRadialSymbolLookup lookup = SymbolLookup;
            int i;
            for (i = 0; i < n; i++)
            {
                GHRadialHubRun run = line.Runs[i];
                if (run == null)
                    continue;
                float sw = line.SymbolWidths[i];
                float tw = line.TextWidths[i];
                if (sw > 0f)
                {
                    if (i > 0)
                        x += size * HubRunGapOfFont;
                    SKRect src = SKRect.Empty;
                    SKImage image = lookup == null ? null : lookup(run.Symbol, out src);
                    if (image != null)
                    {
                        if (!(src.Width > 0f) || !(src.Height > 0f))
                            src = new SKRect(0, 0, image.Width, image.Height);
                        SKRect dest = new SKRect(x, symbolMid - symbolH / 2, x + sw, symbolMid + symbolH / 2);
                        _iconPaint.ColorFilter = null;
                        _iconPaint.Color = faded ? SKColors.White.WithAlpha(alpha) : SKColors.White;
                        canvas.DrawImage(image, src, dest
#if GNH_MAUI
                            , IconSampling
#endif
                            , _iconPaint);
                    }
                    x += sw + (tw > 0f ? size * HubSymbolGapOfFont : 0f);
                }
                if (tw > 0f)
                {
                    _textPaint.Typeface = line.Typeface;
                    _textPaint.TextSize = size;
                    SKColor color = GetNhColor(run.NhColor, defaultColor);
                    _textPaint.Color = faded ? color.WithAlpha((byte)(color.Alpha * alpha / 255)) : color;
                    _textPaint.DrawTextOnCanvas(canvas, run.Text, snap ? Snap(x) : x, baseline, SKTextAlign.Left);
                    x += tw;
                }
            }
        }

        /* The highlighted item's FooterTitle and FooterText, else the fixed hub item's: the title row, then the text
           wrapped, centered as a block in the band */
        private void DrawFooter(SKCanvas canvas, GHRadialMenuState state, GHRadialMenuDefinition def, GHRadialGeometry g,
            float bandTop, float bandBottom, float opacity, bool snap)
        {
            GHRadialItem item = state.HighlightedItem;
            if (item == null || (item.FooterText == null && item.FooterTitle == null))
                item = def.FixedHubItem;
            if (item == null || (string.IsNullOrEmpty(item.FooterText) && item.FooterTitle == null))
                return;
            ItemLayout layout = GetItemLayout(item, state.CurrentPage.SlotCount, -1, g);
            EnsureFooter(item, layout, g, def.FooterMaxLines);
            HubLineLayout title = layout.FooterTitle;
            if (layout.FooterLineCount == 0 && title == null)
                return;
            float titleAscent = 0f;
            float titleH = title != null ? LineHeight(title.Typeface, title.Size, out titleAscent) : 0f;
            float ascent = 0f;
            float lineH = layout.FooterLineCount > 0 ? LineHeight(DescriptionTypeface, layout.FooterSize, out ascent) : 0f;
            float y = (bandTop + bandBottom) / 2 - (titleH + lineH * layout.FooterLineCount) / 2;
            if (title != null)
            {
                DrawHubLine(canvas, title, g.CenterX, y + titleAscent, SKColors.White, opacity, snap);
                y += titleH;
            }
            _textPaint.Typeface = DescriptionTypeface;
            _textPaint.TextSize = layout.FooterSize;
            _textPaint.Color = LighterGray.WithAlpha(ToByte(opacity));
            int i;
            for (i = 0; i < layout.FooterLineCount; i++)
            {
                DrawCentered(canvas, layout.FooterLines[i], g.CenterX, y + ascent, snap);
                y += lineH;
            }
        }

        /* Measures FooterTitle at the band's size, shrinking to fit down to FooterTitleMinOfFont, and wraps
           FooterText into maxLines lines; text that does not fit takes the one smaller size, and then an ellipsis on
           its last line */
        private void EnsureFooter(GHRadialItem item, ItemLayout layout, GHRadialGeometry g, int maxLines)
        {
            if (maxLines < 0)
                maxLines = 0;
            if (layout.FooterReady && layout.FooterLines != null && layout.FooterLines.Length == maxLines)
                return;
            layout.FooterReady = true;
            if (layout.FooterLines == null || layout.FooterLines.Length != maxLines)
                layout.FooterLines = new string[maxLines];
            layout.FooterLineCount = 0;
            layout.FooterTitle = null;

            float margin = (float)(GHRadialTuning.ScreenMarginDip * g.DipScale);
            float width = Math.Max(1f, Math.Min(g.Width - 2 * margin, (float)(GHRadialTuning.FooterWidthOfOuterRadius * g.OuterRadius)));
            float size = (float)(GHRadialTuning.FooterFontOfDescriptionFont * g.HubDescriptionFontSize);
            if (item.FooterTitle != null && item.FooterTitle.Length > 0)
                layout.FooterTitle = MeasureHubLine(item.FooterTitle, DescriptionTypeface, size, width, FooterTitleMinOfFont);
            if (maxLines == 0 || string.IsNullOrWhiteSpace(item.FooterText))
                return;

            bool truncated;
            int count = WrapText(item.FooterText, DescriptionTypeface, size, width, layout.FooterLines, maxLines, out truncated);
            if (truncated)
            {
                size *= (float)GHRadialTuning.FooterStepDownOfFont;
                count = WrapText(item.FooterText, DescriptionTypeface, size, width, layout.FooterLines, maxLines, out truncated);
            }
            layout.FooterSize = size;
            layout.FooterLineCount = count;
        }

        private void DrawHubIdle(SKCanvas canvas, GHRadialMenuState state, GHRadialMenuDefinition def, GHRadialGeometry g, GHRadialPage page, bool snap)
        {
            int pageIndex = state.PageIndex;
            bool empty = page.FilledCount == 0;
            string line2;
            float size2;
            if (empty)
            {
                line2 = page.EmptyText;
                size2 = pageIndex < _emptyTextSizes.Length ? _emptyTextSizes[pageIndex] : _idleTextBaseSize;
            }
            else if (state.IsPointerMode)
            {
                line2 = def.NoHighlightPointerText;
                size2 = _pointerTextSize;
            }
            else
            {
                line2 = def.NoHighlightStickText;
                size2 = _stickTextSize;
            }
            bool showSecondary = empty && def.SwitchSetEnabled && def.SwitchSetPageIndex > 0 && !def.IsSwitchedSetPage(pageIndex)
                && !string.IsNullOrEmpty(def.SecondaryHint);

            /* The counter ("3/7") on its own line above the page name, when the page has one */
            bool showCounter = page.HasCounter;
            float titleSize = pageIndex < _hubIdleTitleSizes.Length ? _hubIdleTitleSizes[pageIndex] : g.HubTitleFontSize;
            float counterSize = pageIndex < _hubCounterSizes.Length ? _hubCounterSizes[pageIndex] : (float)(GHRadialTuning.HubCounterOfTitleFont * g.HubTitleFontSize);
            float titleAscent;
            float counterAscent = 0f;
            float lineAscent = 0f;
            float counterH = 0f;
            float lineH = 0f;
            float hintH = 0f;
            float titleH = LineHeight(TitleTypeface, titleSize, out titleAscent);
            if (showCounter)
                counterH = LineHeight(TitleTypeface, counterSize, out counterAscent);
            if (!string.IsNullOrEmpty(line2))
                lineH = LineHeight(LabelTypeface, size2, out lineAscent);
            if (showSecondary)
                hintH = LineHeight(LabelTypeface, g.HintFontSize, out _) * 1.3f;
            float y = g.CenterY - (counterH + titleH + lineH + hintH) / 2;
            float cx = g.CenterX;

            if (showCounter)
            {
                _textPaint.Typeface = TitleTypeface;
                _textPaint.TextSize = counterSize;
                _textPaint.Color = LighterGray;
                DrawCentered(canvas, page.Counter, cx, y + counterAscent, snap);
                y += counterH;
            }

            _textPaint.Typeface = TitleTypeface;
            _textPaint.TextSize = titleSize;
            _textPaint.Color = SKColors.White;
            DrawCentered(canvas, page.DisplayName, cx, y + titleAscent, snap);
            y += titleH;

            if (lineH > 0f)
            {
                _textPaint.Typeface = LabelTypeface;
                _textPaint.TextSize = size2;
                _textPaint.Color = LighterGray;
                DrawCentered(canvas, line2, cx, y + lineAscent, snap);
                y += lineH;
            }

            if (showSecondary)
            {
                float gap = g.HintFontSize * 0.4f;
                float total = _hubSecondaryBadgeWidth + gap + _hubSecondaryLabelWidth;
                float x = cx - total / 2;
                float centerY = y + hintH / 2;
                _badgeRenderer.DrawBadge(canvas, def.SecondaryButton, def.Style, x + _hubSecondaryBadgeWidth / 2, centerY,
                    BadgeHeight(g), 1f, false);
                _textPaint.Typeface = LabelTypeface;
                _textPaint.TextSize = g.HintFontSize;
                _textPaint.Color = LighterGray;
                SKFontMetrics m = _textPaint.FontMetrics;
                float tx = x + _hubSecondaryBadgeWidth + gap;
                float ty = centerY - (m.Ascent + m.Descent) / 2;
                if (snap)
                {
                    tx = Snap(tx);
                    ty = Snap(ty);
                }
                _textPaint.DrawTextOnCanvas(canvas, def.SecondaryHint, tx, ty, SKTextAlign.Left);
            }
        }

        private void DrawHeader(SKCanvas canvas, GHRadialMenuState state, GHRadialMenuDefinition def, GHRadialGeometry g, float opacity, bool snap)
        {
            GHRadialPage page = state.CurrentPage;
            int pageIndex = state.PageIndex;
            byte a = ToByte(opacity);
            if (!string.IsNullOrEmpty(page.Title))
            {
                _textPaint.Typeface = TitleTypeface;
                _textPaint.TextSize = pageIndex < _pageTitleSizes.Length ? _pageTitleSizes[pageIndex] : g.PageTitleFontSize;
                _textPaint.Color = _accentTitle.WithAlpha(a);
                DrawCentered(canvas, page.Title, g.CenterX, g.PageTitleBaselineY, snap);
            }
            if (_hasTabs)
                DrawSetTabs(canvas, def, g, def.IsSwitchedSetPage(pageIndex) ? 1 : 0, opacity, snap);

            /* One dot per page of the current page's set, or of its group's run with PageWithinGroup, so
               that a set's first page lights the first dot */
            int first, count;
            def.GetDotRange(pageIndex, out first, out count);
            if (count <= 1)
                return;
            int i;
            for (i = 0; i < count; i++)
            {
                float x, y;
                g.GetDotCenter(i, count, out x, out y);
                bool current = first + i == pageIndex;
                _plainFill.Color = (current ? SKColors.LightGreen : SKColors.White).WithAlpha(a);
                canvas.DrawCircle(x, y, current ? g.DotRadius * 2 : g.DotRadius, _plainFill);
            }
        }

        /* The two set tabs, each an icon and a label in a rounded outline, with the switch badge between them. The
           active one is at full opacity with the accent outline; with switching disabled both are dimmed and the
           badge is hidden. */
        private void DrawSetTabs(SKCanvas canvas, GHRadialMenuDefinition def, GHRadialGeometry g, int active, float opacity, bool snap)
        {
            float h = _tabHeight;
            float cy = _tabRowCenterY;
            float spacing = h * TabSpacingOfHeight;
            float pad = h * TabPaddingOfHeight;
            float x = g.CenterX - _tabsTotalWidth / 2;
            bool enabled = def.SwitchSetEnabled;
            int i;
            for (i = 0; i < 2; i++)
            {
                float alpha = opacity * (enabled && i == active ? 1f : (float)GHRadialTuning.SetTabInactiveOpacity);
                byte a = ToByte(alpha);
                SKRect rect = new SKRect(x, cy - h / 2, x + _tabWidths[i], cy + h / 2);
                _plainFill.Color = HubFill.WithAlpha((byte)(HubFill.Alpha * alpha));
                canvas.DrawRoundRect(rect, h / 2, h / 2, _plainFill);
                _plainStroke.Color = (enabled && i == active ? _accent : LighterGray).WithAlpha(a);
                _plainStroke.StrokeWidth = (float)(GHRadialTuning.HubBorderDip * g.DipScale);
                canvas.DrawRoundRect(rect, h / 2, h / 2, _plainStroke);

                float cx = x + pad;
                string iconPath = def.SetTabIcons != null && i < def.SetTabIcons.Length ? def.SetTabIcons[i] : null;
                Func<string, SKImage> lookup = IconLookup;
                SKImage image = lookup == null || string.IsNullOrEmpty(iconPath) ? null : lookup(iconPath);
                if (image != null)
                {
                    SKRect dest = new SKRect(cx, cy - _tabIconSize / 2, cx + _tabIconSize, cy + _tabIconSize / 2);
                    _iconPaint.ColorFilter = null;
                    _iconPaint.Color = SKColors.White.WithAlpha(a);
                    canvas.DrawImage(image, dest
#if GNH_MAUI
                        , IconSampling
#endif
                        , _iconPaint);
                    cx += _tabIconSize + h * 0.2f;
                }
                if (!string.IsNullOrEmpty(_tabLabels[i]))
                {
                    _textPaint.Typeface = LabelTypeface;
                    _textPaint.TextSize = _tabFontSize;
                    _textPaint.Color = SKColors.White.WithAlpha(a);
                    SKFontMetrics m = _textPaint.FontMetrics;
                    float tx = cx;
                    float ty = cy - (m.Ascent + m.Descent) / 2;
                    if (snap)
                    {
                        tx = Snap(tx);
                        ty = Snap(ty);
                    }
                    _textPaint.DrawTextOnCanvas(canvas, _tabLabels[i], tx, ty, SKTextAlign.Left);
                }
                x += _tabWidths[i];
                if (i == 0)
                {
                    if (enabled)
                        _badgeRenderer.DrawBadge(canvas, def.SecondaryButton, def.Style, x + spacing + _tabBadgeWidth / 2, cy, _tabBadgeHeight, opacity, false);
                    x += 2 * spacing + _tabBadgeWidth;
                }
            }
        }

        /* The prev and next chips within the definition's paging range */
        private void DrawChips(SKCanvas canvas, GHRadialMenuState state, GHRadialMenuDefinition def, GHRadialGeometry g, float opacity, bool snap)
        {
            int pageIndex = state.PageIndex;
            int first, count;
            def.GetPagingRange(pageIndex, out first, out count);
            if (!_chipsVisible || count <= 1)
                return;
            if (pageIndex > first)
                DrawChip(canvas, g, g.CenterX - g.ChipOffsetX, def.PrevPageButton, def.Style, _chipTitles[pageIndex - 1], opacity, snap);
            if (pageIndex < first + count - 1)
                DrawChip(canvas, g, g.CenterX + g.ChipOffsetX, def.NextPageButton, def.Style, _chipTitles[pageIndex + 1], opacity, snap);
        }

        /* The button's badge with the neighboring page's title below it */
        private void DrawChip(SKCanvas canvas, GHRadialGeometry g, float x, GHGamepadButton button, GHGamepadStyle style, string title, float opacity, bool snap)
        {
            float fs = g.HintBarFontSize;
            float badgeHeight = BadgeHeightAt(fs);
            float badgeCenterY = g.CenterY - fs * 0.6f;
            _badgeRenderer.DrawBadge(canvas, button, style, x, badgeCenterY, badgeHeight, opacity, false);
            if (!string.IsNullOrEmpty(title))
            {
                _textPaint.Typeface = LabelTypeface;
                _textPaint.TextSize = fs;
                _textPaint.Color = LighterGray.WithAlpha(ToByte(opacity));
                float baseline = badgeCenterY + badgeHeight / 2 + fs * 0.4f - _textPaint.FontMetrics.Ascent;
                DrawCentered(canvas, title, x, baseline, snap);
            }
        }

        /* dimConfirm draws the confirm hint at DimmedHintOpacity */
        private void DrawHintBar(SKCanvas canvas, HintBar bar, GHGamepadStyle style, GHRadialGeometry g, float centerY, float opacity, bool snap,
            bool dimConfirm)
        {
            if (bar.Count == 0)
                return;
            float fs = bar.FontSize;
            float smallGap = fs * 0.4f;
            float largeGap = fs * 1.4f;
            float badgeHeight = BadgeHeightAt(fs);
            float x = g.CenterX - bar.TotalWidth / 2;
            _textPaint.Typeface = LabelTypeface;
            _textPaint.TextSize = fs;
            SKFontMetrics m = _textPaint.FontMetrics;
            float labelBaseline = centerY - (m.Ascent + m.Descent) / 2;
            int i;
            for (i = 0; i < bar.Count; i++)
            {
                float hintOpacity = dimConfirm && i == bar.ConfirmIndex ? opacity * DimmedHintOpacity : opacity;
                _badgeRenderer.DrawHintBadges(canvas, bar.Hints[i], style, x, centerY, badgeHeight, hintOpacity);
                x += bar.BadgeWidths[i] + smallGap;
                _textPaint.Typeface = LabelTypeface;
                _textPaint.TextSize = fs;
                _textPaint.Color = SKColors.White.WithAlpha(ToByte(hintOpacity));
                float tx = x;
                float ty = labelBaseline;
                if (snap)
                {
                    tx = Snap(tx);
                    ty = Snap(ty);
                }
                _textPaint.DrawTextOnCanvas(canvas, bar.Labels[i], tx, ty, SKTextAlign.Left);
                x += bar.LabelWidths[i] + largeGap;
            }
        }

        /* Controller badges are as tall as a hint strip's badges at the hint font size */
        private static float BadgeHeight(GHRadialGeometry g)
        {
            return BadgeHeightAt(g.HintFontSize);
        }

        private static float BadgeHeightAt(float fontSize)
        {
            return fontSize * GHGamepadHintRenderer.BadgeHeightOfFont;
        }

        /* Creates the badge renderer on first use and passes it this renderer's typefaces and text
           blob cache setting; once per paint, before any badge */
        private void BeginBadgePaint()
        {
            GHGamepadHintRenderer badges = _badgeRenderer;
            if (badges == null)
            {
                badges = new GHGamepadHintRenderer();
                _badgeRenderer = badges;
            }
            badges.LabelTypeface = LabelTypeface;
            badges.BoldTypeface = BoldTypeface;
            badges.UseTextBlobCaching = UseTextBlobCaching;
            badges.BeginPaint();
        }

        private void ResolveAccent(GHRadialAccent accent)
        {
            switch (accent)
            {
            case GHRadialAccent.Violet:
                _accent = Violet;
                _accentHighlightFill = VioletHighlightFill;
                _accentFlashFill = VioletFlashFill;
                _accentTitle = VioletTitle;
                break;
            default:
                _accent = Gold;
                _accentHighlightFill = GoldHighlightFill;
                _accentFlashFill = GoldFlashFill;
                _accentTitle = SKColors.White;
                break;
            }
        }

        private void DrawCentered(SKCanvas canvas, string text, float x, float baseline, bool snap)
        {
            if (string.IsNullOrEmpty(text))
                return;
            if (snap)
            {
                x = Snap(x);
                baseline = Snap(baseline);
            }
            _textPaint.DrawTextOnCanvas(canvas, text, x, baseline, SKTextAlign.Center);
        }

        private float LineHeight(SKTypeface typeface, float size, out float ascent)
        {
            _textPaint.Typeface = typeface;
            _textPaint.TextSize = size;
            ascent = -_textPaint.FontMetrics.Ascent;
            return _textPaint.FontSpacing;
        }

        /* The item's layout for pages of slotCount; slot is where its label is drawn, or -1 for a caller that does not
           draw the label. The label is fitted again only when the slot differs. */
        private ItemLayout GetItemLayout(GHRadialItem item, int slotCount, int slot, GHRadialGeometry g)
        {
            ItemLayout layout;
            if (!_itemLayouts.TryGetValue(item, out layout) || layout.SlotCount != slotCount)
            {
                layout = CreateItemLayout(item, slotCount, g);
                _itemLayouts[item] = layout;
            }
            if (slot >= 0 && (!layout.LabelReady || layout.LabelSlot != slot))
                FitItemLabel(item, layout, slotCount, slot, g);
            return layout;
        }

        /* A placeholder without a label reads "Empty" */
        private static string GetSliceLabel(GHRadialItem item)
        {
            return item.IsPlaceholder && string.IsNullOrEmpty(item.Label) ? PlaceholderText : item.Label;
        }

        /* The label in at most the definition's LabelMaxLines lines, each within the slice's width at the label's
           place for the whole block's height: it wraps first, then shrinks to the label's minimum size, then ends
           with an ellipsis */
        private void FitItemLabel(GHRadialItem item, ItemLayout layout, int slotCount, int slot, GHRadialGeometry g)
        {
            layout.LabelReady = true;
            layout.LabelSlot = slot;
            string label = GetSliceLabel(item);
            float baseSize = g.LabelFontSize;
            float minSize = Math.Min(g.LabelMinFontSize, g.LabelFontSize);
            int maxLines = ClampLines(_layoutDefinition == null ? 1 : _layoutDefinition.LabelMaxLines);
            float lineH = baseSize * (float)GHRadialTuning.LabelLineHeight;
            int n;
            for (n = 1; n <= maxLines; n++)
                _lineWidths[n - 1] = GetSliceLabelMaxWidth(item, slotCount, slot, g, n * lineH);
            float labelMax = _lineWidths[0];
            layout.LabelMaxWidth = labelMax;
            if (maxLines == 1)
            {
                layout.LabelSize = FitSize(label, LabelTypeface, baseSize, minSize, labelMax);
                layout.LabelLines[0] = Ellipsize(label, LabelTypeface, layout.LabelSize, labelMax);
                layout.LabelLineCount = string.IsNullOrEmpty(layout.LabelLines[0]) ? 0 : 1;
                return;
            }
            float size;
            layout.LabelLineCount = FitLines(label, LabelTypeface, baseSize, minSize, _lineWidths, maxLines, true, layout.LabelLines, out size);
            layout.LabelSize = size;
        }

        /* The slice's width for a label block of blockHeight under the slot's icon, as DrawSlotContents places it
           while the slot is not highlighted */
        private static float GetSliceLabelMaxWidth(GHRadialItem item, int slotCount, int slot, GHRadialGeometry g, float blockHeight)
        {
            if (slot < 0 || slotCount <= 0)
                return g.GetLabelMaxWidth(slotCount);
            double rad = GHRadialGeometry.SlotCenterDegrees(slot, slotCount) * Math.PI / 180.0;
            float radius = g.MidRadius - (item.IsToggle ? (float)(GHRadialTuning.ToggleContentInset * g.OuterRadius) : 0f);
            float x = (float)Math.Sin(rad) * radius;
            float y = -(float)Math.Cos(rad) * radius + g.GetIconSize(slotCount) / 2;
            return g.GetLabelMaxWidthAt(slotCount, slot, x, y, blockHeight);
        }

        private static int ClampLines(int lines)
        {
            return Math.Max(1, Math.Min(MaxWrappedLines, lines));
        }

        private ItemLayout CreateItemLayout(GHRadialItem item, int slotCount, GHRadialGeometry g)
        {
            ItemLayout layout = new ItemLayout();
            layout.SlotCount = slotCount;
            string label = GetSliceLabel(item);

            /* The hub title shrinks to its minimum on one line before it takes more lines; the fixed hub item's title, under
               its icon, gets a narrower width */
            bool isFixedHubItem = _layoutDefinition != null && ReferenceEquals(item, _layoutDefinition.FixedHubItem);
            float hubWidth = (float)((isFixedHubItem ? HubFixedTitleWidthOfHubDiameter : GHRadialTuning.HubTitleWidthOfHubDiameter)
                * 2 * g.HubRadius);
            float titleMin = (float)(GHRadialTuning.HubTitleMinOfFont * g.HubTitleFontSize);
            string hubTitle = item.HubTitle ?? label;
            int titleLines = ClampLines(_layoutDefinition == null ? 1 : _layoutDefinition.HubTitleMaxLines);
            if (titleLines == 1)
            {
                layout.HubTitleSize = FitSize(hubTitle, TitleTypeface, g.HubTitleFontSize, titleMin, hubWidth);
                layout.HubTitleLines[0] = Ellipsize(hubTitle, TitleTypeface, layout.HubTitleSize, hubWidth);
                layout.HubTitleLineCount = 1;
            }
            else
            {
                int n;
                for (n = 0; n < titleLines; n++)
                    _lineWidths[n] = hubWidth;
                float titleSize;
                layout.HubTitleLineCount = Math.Max(1, FitLines(hubTitle, TitleTypeface, g.HubTitleFontSize, titleMin, _lineWidths, titleLines, false,
                    layout.HubTitleLines, out titleSize));
                layout.HubTitleSize = titleSize;
            }

            if (item.Badge == GHRadialBadge.Count && !string.IsNullOrEmpty(item.BadgeText))
            {
                _textPaint.Typeface = BoldTypeface;
                _textPaint.TextSize = SlotBadgeHeight(g) * BadgeTextOfHeight;
                layout.BadgeTextWidth = _textPaint.MeasureText(item.BadgeText);
            }

            /* A description that only repeats the label gets no lines; DrawHubItem centers the lines that remain */
            float descWidth = (float)(GHRadialTuning.HubDescriptionWidthOfHubDiameter * 2 * g.HubRadius);
            layout.DescriptionLineCount = IsDescriptionRedundant(item.Description, item.Label) ? 0
                : WrapText(item.Description, DescriptionTypeface, g.HubDescriptionFontSize, descWidth, layout.DescriptionLines);
            return layout;
        }

        /* Empty, or the label again once both are trimmed */
        private static bool IsDescriptionRedundant(string description, string label)
        {
            if (string.IsNullOrWhiteSpace(description))
                return true;
            return label != null && string.Equals(description.Trim(), label.Trim(), StringComparison.Ordinal);
        }

        /* Keyed on the definition, its badge style, the geometry and the typefaces; the definition's
           buttons are taken as fixed once it is shown. The accent changes no layout. */
        private void EnsureLayouts(GHRadialMenuDefinition def, GHRadialGeometry g)
        {
            if (ReferenceEquals(def, _layoutDefinition) && def.Style == _layoutStyle && g.Version == _layoutVersion
                && ReferenceEquals(TitleTypeface, _layoutTitleTypeface) && ReferenceEquals(LabelTypeface, _layoutLabelTypeface)
                && ReferenceEquals(BoldTypeface, _layoutBoldTypeface) && ReferenceEquals(DescriptionTypeface, _layoutDescriptionTypeface))
                return;

            _itemLayouts.Clear();
            _layoutDefinition = def;
            _layoutStyle = def.Style;
            _layoutVersion = g.Version;
            _layoutTitleTypeface = TitleTypeface;
            _layoutLabelTypeface = LabelTypeface;
            _layoutBoldTypeface = BoldTypeface;
            _layoutDescriptionTypeface = DescriptionTypeface;

            float margin = (float)(GHRadialTuning.ScreenMarginDip * g.DipScale);
            float hubWidth = (float)(GHRadialTuning.HubTitleWidthOfHubDiameter * 2 * g.HubRadius);
            float titleMin = (float)(GHRadialTuning.HubTitleMinOfFont * g.HubTitleFontSize);
            float pageTitleMax = Math.Max(1f, g.Width - 2 * margin);

            /* L1/R1 chips: half-width up to the screen edge and up to the ring, whichever is nearer */
            float chipX = g.CenterX - g.ChipOffsetX;
            float chipHalf = Math.Min(chipX - margin, g.CenterX - g.OuterRadius - g.HighlightOffset - margin / 2 - chipX);
            _chipsVisible = chipHalf > g.HintBarFontSize * 2;
            float chipMax = Math.Max(1f, 2 * chipHalf);

            int count = def.PageCount;
            _pageTitleSizes = new float[count];
            _hubIdleTitleSizes = new float[count];
            _hubCounterSizes = new float[count];
            _emptyTextSizes = new float[count];
            _chipTitles = new string[count];
            _idleTextBaseSize = g.HubDescriptionFontSize;
            float counterBase = (float)(GHRadialTuning.HubCounterOfTitleFont * g.HubTitleFontSize);
            int i;
            for (i = 0; i < count; i++)
            {
                GHRadialPage page = def.Pages[i];
                string title = page == null ? null : page.Title;
                _pageTitleSizes[i] = FitSize(title, TitleTypeface, g.PageTitleFontSize, g.PageTitleFontSize * 0.6f, pageTitleMax);
                /* The hub shows the name, with the counter on a line of its own above it */
                _hubIdleTitleSizes[i] = FitSize(page == null ? null : page.DisplayName, TitleTypeface, g.HubTitleFontSize, titleMin, hubWidth);
                _hubCounterSizes[i] = FitSize(page == null ? null : page.Counter, TitleTypeface, counterBase, counterBase * 0.6f, hubWidth);
                _emptyTextSizes[i] = FitSize(page == null ? null : page.EmptyText, LabelTypeface, _idleTextBaseSize, _idleTextBaseSize * 0.7f, hubWidth);
                _chipTitles[i] = _chipsVisible ? Ellipsize(title, LabelTypeface, g.HintBarFontSize, chipMax) : null;
            }
            _stickTextSize = FitSize(def.NoHighlightStickText, LabelTypeface, _idleTextBaseSize, _idleTextBaseSize * 0.7f, hubWidth);
            _pointerTextSize = FitSize(def.NoHighlightPointerText, LabelTypeface, _idleTextBaseSize, _idleTextBaseSize * 0.7f, hubWidth);
            EnsurePromptLayout(def, g, hubWidth, titleMin);

            _textPaint.Typeface = BoldTypeface;
            _textPaint.TextSize = g.PillFontSize;
            _pillOnWidth = _textPaint.MeasureText(PillOnText);
            _pillOffWidth = _textPaint.MeasureText(PillOffText);

            _statusSize = g.HubDescriptionFontSize;
            _textPaint.Typeface = LabelTypeface;
            _textPaint.TextSize = _statusSize;
            _currentlyWidth = _textPaint.MeasureText(CurrentlyText);
            _textPaint.Typeface = BoldTypeface;
            _onWidth = _textPaint.MeasureText(OnText);
            _offWidth = _textPaint.MeasureText(OffText);

            _hubSecondaryBadgeWidth = _badgeRenderer.MeasureBadge(def.SecondaryButton, def.Style, BadgeHeight(g));
            _hubSecondaryLabelWidth = MeasureLabel(def.SecondaryHint, g.HintFontSize);

            _textPaint.Typeface = BoldTypeface;
            _textPaint.TextSize = SlotBadgeHeight(g) * BadgeTextOfHeight;
            _newBadgeWidth = _textPaint.MeasureText(NewBadgeText);
            EnsureTabLayout(def, g, margin);

            BuildHintBar(_hintsMain, def, g, false, null, def.CancelHint);
            BuildHintBar(_hintsSwitched, def, g, true, null, def.CancelHint);
        }

        /* The set tab row: its height scales with the page title font, and it sits SetTabGapOfHeight above the page
           title's capitals; each label is shortened to its half of the screen */
        private void EnsureTabLayout(GHRadialMenuDefinition def, GHRadialGeometry g, float margin)
        {
            _hasTabs = def.HasSetTabs;
            if (!_hasTabs)
                return;
            float h = (float)(GHRadialTuning.SetTabHeightOfTitleFont * g.PageTitleFontSize);
            _tabHeight = h;
            _tabFontSize = h * TabFontOfHeight;
            _tabIconSize = h * TabIconOfHeight;
            _tabBadgeHeight = h * TabBadgeOfHeight;
            _tabBadgeWidth = _badgeRenderer.MeasureBadge(def.SecondaryButton, def.Style, _tabBadgeHeight);
            _textPaint.Typeface = TitleTypeface;
            _textPaint.TextSize = g.PageTitleFontSize;
            float titleTop = g.PageTitleBaselineY + _textPaint.FontMetrics.Ascent;
            _tabRowCenterY = titleTop - (float)(GHRadialTuning.SetTabGapOfHeight * h) - h / 2;

            float spacing = h * TabSpacingOfHeight;
            float pad = h * TabPaddingOfHeight;
            float available = Math.Max(1f, g.Width - 2 * margin);
            float half = (available - _tabBadgeWidth - 2 * spacing) / 2;
            float total = _tabBadgeWidth + 2 * spacing;
            int i;
            for (i = 0; i < 2; i++)
            {
                bool hasIcon = def.SetTabIcons != null && i < def.SetTabIcons.Length && !string.IsNullOrEmpty(def.SetTabIcons[i]);
                float iconW = hasIcon ? _tabIconSize + h * 0.2f : 0f;
                float labelMax = Math.Max(1f, half - 2 * pad - iconW);
                _tabLabels[i] = Ellipsize(def.SetTabs[i], LabelTypeface, _tabFontSize, labelMax);
                _tabLabelWidths[i] = MeasureLabel(_tabLabels[i], _tabFontSize);
                _tabWidths[i] = 2 * pad + iconW + _tabLabelWidths[i];
                total += _tabWidths[i];
            }
            _tabsTotalWidth = total;
        }

        /* The bar for the current page's set, measured again when the highlighted item's action hint, the cancel
           label or the switch hint's visibility differs from what it was measured for */
        private HintBar GetHintBar(GHRadialMenuState state, GHRadialMenuDefinition def, GHRadialGeometry g)
        {
            bool switched = def.IsSwitchedSetPage(state.PageIndex);
            HintBar bar = switched ? _hintsSwitched : _hintsMain;
            GHRadialItem item = state.HighlightedItem;
            string itemHint = item == null ? null : item.ItemActionHint;
            GHRadialPage page = state.CurrentPage;
            bool hasParent = page != null && page.ParentPageIndex >= 0 && page.ParentPageIndex < def.PageCount && page.ParentPageIndex != state.PageIndex;
            string cancelLabel = def.CancelConsumed || hasParent ? def.BackHint : def.CancelHint;
            if (!string.Equals(bar.ItemHint, itemHint, StringComparison.Ordinal) || !string.Equals(bar.CancelLabel, cancelLabel, StringComparison.Ordinal)
                || bar.ShowsSecondary != def.SwitchSetEnabled)
                BuildHintBar(bar, def, g, switched, itemHint, cancelLabel);
            return bar;
        }

        /* The direction prompt's hub texts; the top line takes the smallest size any of its texts needs to fit the hub's
           chord at its height */
        private void EnsurePromptLayout(GHRadialMenuDefinition def, GHRadialGeometry g, float hubWidth, float titleMin)
        {
            _hasPrompt = !string.IsNullOrWhiteSpace(def.PromptVerb);
            _promptObjectLineCount = 0;
            if (!_hasPrompt)
            {
                _promptVerb = null;
                return;
            }

            _promptVerbSize = FitSize(def.PromptVerb, TitleTypeface, g.HubTitleFontSize, titleMin, hubWidth);
            _promptVerb = Ellipsize(def.PromptVerb, TitleTypeface, _promptVerbSize, hubWidth);

            float descWidth = (float)(GHRadialTuning.HubDescriptionWidthOfHubDiameter * 2 * g.HubRadius);
            if (!string.IsNullOrWhiteSpace(def.PromptObject))
                _promptObjectLineCount = WrapText(def.PromptObject, DescriptionTypeface, g.HubDescriptionFontSize, descWidth, _promptObjectLines);

            float verbBaseline, objectTop, topCenterY;
            GetPromptPositions(g, out verbBaseline, out objectTop, out topCenterY);
            float rise = g.CenterY - topCenterY;
            float chord = 2 * (float)Math.Sqrt(Math.Max(0.0, (double)g.HubRadius * g.HubRadius - (double)rise * rise));
            float topWidth = Math.Max(1f, Math.Min(hubWidth, (float)(GHRadialTuning.HubTitleWidthOfHubDiameter * chord)));

            float topBase = g.HubDescriptionFontSize;
            float topMin = topBase * 0.7f;
            _promptTopSize = Math.Min(FitSize(def.NoHighlightStickText, LabelTypeface, topBase, topMin, topWidth),
                FitSize(def.NoHighlightPointerText, LabelTypeface, topBase, topMin, topWidth));
            GHRadialPage page = def.GetPage(0);
            if (page != null)
            {
                int i;
                for (i = 0; i < page.SlotCount; i++)
                {
                    GHRadialItem item = page.GetItem(i);
                    if (item != null)
                        _promptTopSize = Math.Min(_promptTopSize, FitSize(item.Label, LabelTypeface, topBase, topMin, topWidth));
                }
            }
        }

        /* At the Hint Size, shrinking back toward the 100% size when the bar is wider than the screen */
        private void BuildHintBar(HintBar bar, GHRadialMenuDefinition def, GHRadialGeometry g, bool switched, string itemHint, string cancelLabel)
        {
            bar.ItemHint = itemHint;
            bar.CancelLabel = cancelLabel;
            bar.ShowsSecondary = def.SwitchSetEnabled;
            MeasureHintBar(bar, def, switched, g.HintBarFontSize);
            float available = g.Width - 2 * (float)(GHRadialTuning.ScreenMarginDip * g.DipScale);
            if (bar.TotalWidth > available && bar.TotalWidth > 0f && g.HintBarFontSize > g.HintFontSize)
                MeasureHintBar(bar, def, switched, Math.Max(g.HintFontSize, g.HintBarFontSize * available / bar.TotalWidth));
        }

        /* Confirm, cancel, the item action, the extra hints, the set switch and the group pair, each when it has a label */
        private void MeasureHintBar(HintBar bar, GHRadialMenuDefinition def, bool switched, float fs)
        {
            bar.Count = 0;
            bar.FontSize = fs;
            AddHint(bar, new GHGamepadHint(def.ConfirmButton, null, false), def.ConfirmHint, def.Style);
            bar.ConfirmIndex = bar.Count > 0 ? 0 : -1;
            AddHint(bar, new GHGamepadHint(def.CancelButton, null, false), bar.CancelLabel, def.Style);
            AddHint(bar, new GHGamepadHint(def.ItemActionButton, null, false), bar.ItemHint, def.Style);
            if (def.ExtraHints != null)
            {
                int e;
                for (e = 0; e < def.ExtraHints.Length; e++)
                    AddHint(bar, new GHGamepadHint(def.ExtraHints[e].Button, null, false), def.ExtraHints[e].Label, def.Style);
            }
            if (def.SwitchSetPageIndex > 0 && bar.ShowsSecondary)
                AddHint(bar, new GHGamepadHint(def.SecondaryButton, null, false), switched ? def.SecondaryHintOnSwitchedSet : def.SecondaryHint, def.Style);
            if (def.HasGroups)
                AddHint(bar, GHGamepadHint.Pair(def.GroupButton, def.GroupButton2, null), def.GroupHint, def.Style);

            float total = 0f;
            int i;
            for (i = 0; i < bar.Count; i++)
            {
                total += bar.BadgeWidths[i] + fs * 0.4f + bar.LabelWidths[i];
                if (i > 0)
                    total += fs * 1.4f;
            }
            bar.TotalWidth = total;
        }

        /* The hint's badges, measured at the bar's font size; its label is drawn by this renderer */
        private void AddHint(HintBar bar, GHGamepadHint hint, string label, GHGamepadStyle style)
        {
            if (string.IsNullOrEmpty(label) || bar.Count >= bar.Hints.Length)
                return;
            bar.Hints[bar.Count] = hint;
            bar.Labels[bar.Count] = label;
            bar.BadgeWidths[bar.Count] = _badgeRenderer.MeasureHintBadges(hint, style, BadgeHeightAt(bar.FontSize));
            bar.LabelWidths[bar.Count] = MeasureLabel(label, bar.FontSize);
            bar.Count++;
        }

        private float MeasureLabel(string text, float size)
        {
            if (string.IsNullOrEmpty(text))
                return 0f;
            _textPaint.Typeface = LabelTypeface;
            _textPaint.TextSize = size;
            return _textPaint.MeasureText(text);
        }

        /* The size at which the text fits maxWidth, not below minSize */
        private float FitSize(string text, SKTypeface typeface, float size, float minSize, float maxWidth)
        {
            if (string.IsNullOrEmpty(text))
                return size;
            _textPaint.Typeface = typeface;
            _textPaint.TextSize = size;
            float w = _textPaint.MeasureText(text);
            if (!(w > maxWidth))
                return size;
            return Math.Max(minSize, size * maxWidth / w);
        }

        /* The text, shortened with an ellipsis until it fits maxWidth */
        private string Ellipsize(string text, SKTypeface typeface, float size, float maxWidth)
        {
            if (string.IsNullOrEmpty(text))
                return text;
            _textPaint.Typeface = typeface;
            _textPaint.TextSize = size;
            if (_textPaint.MeasureText(text) <= maxWidth)
                return text;
            int len;
            for (len = text.Length - 1; len > 0; len--)
            {
                string candidate = text.Substring(0, len).TrimEnd() + Ellipsis;
                if (_textPaint.MeasureText(candidate) <= maxWidth)
                    return candidate;
            }
            return Ellipsis;
        }

        /* Fits text into at most maxLines lines, widths[n - 1] wide when it takes n lines. wrapFirst takes more lines
           at baseSize before it shrinks, choosing the line count that shrinks least; otherwise each line count in turn
           shrinks down to minSize before the next is tried. Text that fits no line count at minSize is wrapped into
           maxLines at minSize with an ellipsis on its last line. Returns the line count; size is the size used. */
        private int FitLines(string text, SKTypeface typeface, float baseSize, float minSize, float[] widths, int maxLines, bool wrapFirst,
            string[] lines, out float size)
        {
            size = baseSize;
            maxLines = Math.Max(1, Math.Min(maxLines, Math.Min(widths.Length, lines.Length)));
            if (string.IsNullOrWhiteSpace(text))
            {
                lines[0] = text;
                return 0;
            }
            if (minSize > baseSize)
                minSize = baseSize;
            int n, count;
            if (wrapFirst)
            {
                for (n = 1; n <= maxLines; n++)
                {
                    count = TryWrapExact(text, typeface, baseSize, widths[n - 1], lines, n);
                    if (count > 0)
                        return count;
                }
                float best = 0f;
                int bestLines = 0;
                for (n = 1; n <= maxLines; n++)
                {
                    float s = LargestFittingSize(text, typeface, baseSize, minSize, widths[n - 1], lines, n);
                    if (s > best)
                    {
                        best = s;
                        bestLines = n;
                    }
                }
                if (bestLines > 0)
                {
                    count = TryWrapExact(text, typeface, best, widths[bestLines - 1], lines, bestLines);
                    if (count > 0)
                    {
                        size = best;
                        return count;
                    }
                }
            }
            else
            {
                for (n = 1; n <= maxLines; n++)
                {
                    float s = LargestFittingSize(text, typeface, baseSize, minSize, widths[n - 1], lines, n);
                    if (s > 0f)
                    {
                        count = TryWrapExact(text, typeface, s, widths[n - 1], lines, n);
                        if (count > 0)
                        {
                            size = s;
                            return count;
                        }
                    }
                }
            }

            size = minSize;
            bool truncated;
            count = WrapText(text, typeface, minSize, widths[maxLines - 1], lines, maxLines, out truncated);
            if (count > 0 && count < maxLines)
                count = WrapText(text, typeface, minSize, widths[count - 1], lines, count, out truncated);
            return count;
        }

        /* The largest size from minSize to baseSize at which text wraps into maxLines lines of maxWidth, or 0 when it
           does not even at minSize */
        private float LargestFittingSize(string text, SKTypeface typeface, float baseSize, float minSize, float maxWidth, string[] lines, int maxLines)
        {
            if (TryWrapExact(text, typeface, baseSize, maxWidth, lines, maxLines) > 0)
                return baseSize;
            if (TryWrapExact(text, typeface, minSize, maxWidth, lines, maxLines) <= 0)
                return 0f;
            float lo = minSize;
            float hi = baseSize;
            int i;
            for (i = 0; i < 10; i++)
            {
                float mid = (lo + hi) / 2;
                if (TryWrapExact(text, typeface, mid, maxWidth, lines, maxLines) > 0)
                    lo = mid;
                else
                    hi = mid;
            }
            return lo;
        }

        /* Greedy word wrap into at most maxLines lines with no word shortened; 0 when the text does not fit */
        private int TryWrapExact(string text, SKTypeface typeface, float size, float maxWidth, string[] lines, int maxLines)
        {
            maxLines = Math.Min(maxLines, lines.Length);
            if (string.IsNullOrEmpty(text) || maxLines <= 0)
                return 0;
            string[] words = text.Split(WordSeparators, StringSplitOptions.RemoveEmptyEntries);
            if (words.Length == 0)
                return 0;
            _textPaint.Typeface = typeface;
            _textPaint.TextSize = size;
            int count = 0;
            int w = 0;
            while (w < words.Length)
            {
                if (count >= maxLines)
                    return 0;
                string line = words[w++];
                if (_textPaint.MeasureText(line) > maxWidth)
                    return 0;
                while (w < words.Length)
                {
                    string candidate = line + " " + words[w];
                    if (_textPaint.MeasureText(candidate) > maxWidth)
                        break;
                    line = candidate;
                    w++;
                }
                lines[count++] = line;
            }
            return count;
        }

        /* Greedy word wrap into lines; the last line takes an ellipsis when text remains */
        private int WrapText(string text, SKTypeface typeface, float size, float maxWidth, string[] lines)
        {
            bool truncated;
            return WrapText(text, typeface, size, maxWidth, lines, lines.Length, out truncated);
        }

        /* Greedy word wrap into at most maxLines of lines; the last line takes an ellipsis when text remains, and
           truncated tells whether any line was shortened */
        private int WrapText(string text, SKTypeface typeface, float size, float maxWidth, string[] lines, int maxLines, out bool truncated)
        {
            truncated = false;
            maxLines = Math.Min(maxLines, lines.Length);
            if (string.IsNullOrEmpty(text) || maxLines <= 0)
                return 0;
            string[] words = text.Split(WordSeparators, StringSplitOptions.RemoveEmptyEntries);
            _textPaint.Typeface = typeface;
            _textPaint.TextSize = size;
            int count = 0;
            int w = 0;
            while (w < words.Length && count < maxLines)
            {
                string line = words[w++];
                while (w < words.Length)
                {
                    string candidate = line + " " + words[w];
                    if (_textPaint.MeasureText(candidate) > maxWidth)
                        break;
                    line = candidate;
                    w++;
                }
                if (count == maxLines - 1 && w < words.Length)
                {
                    line = Ellipsize(line + " " + string.Join(" ", words, w, words.Length - w), typeface, size, maxWidth);
                    truncated = true;
                }
                else if (_textPaint.MeasureText(line) > maxWidth)
                {
                    line = Ellipsize(line, typeface, size, maxWidth);
                    truncated = true;
                }
                _textPaint.Typeface = typeface;
                _textPaint.TextSize = size;
                lines[count++] = line;
            }
            return count;
        }

        private void EnsureResources(GHRadialGeometry g)
        {
            if (_resourceVersion == g.Version)
                return;
            _resourceVersion = g.Version;

            SKPathEffect oldCorner = _cornerEffect;
            SKPathEffect oldDash = _dashEffect;
            SKPathEffect oldEmpty = _emptyEffect;
            SKMaskFilter oldGlow = _glowFilter;
            SKShader oldVignette = _vignette;

            _cornerEffect = SKPathEffect.CreateCorner(Math.Max(0.5f, g.CornerRadius));
            float dash = Math.Max(2f, 0.02f * g.OuterRadius);
            _dashEffect = SKPathEffect.CreateDash(new float[] { dash, dash }, 0);
            _emptyEffect = SKPathEffect.CreateCompose(_dashEffect, _cornerEffect);
            _glowFilter = SKMaskFilter.CreateBlur(SKBlurStyle.Normal, Math.Max(0.5f, g.GlowBlurRadius));
            _vignette = SKShader.CreateRadialGradient(new SKPoint(g.CenterX, g.CenterY), (float)(GHRadialTuning.VignetteRadius * g.OuterRadius),
                new SKColor[] { VignetteInner, VignetteOuter }, null, SKShaderTileMode.Clamp);

            _sliceFill.PathEffect = _cornerEffect;
            _sliceStroke.PathEffect = _cornerEffect;
            _glowPaint.PathEffect = _cornerEffect;
            _glowPaint.MaskFilter = _glowFilter;
            _emptyStroke.PathEffect = _emptyEffect;
            _placeholderStroke.PathEffect = _emptyEffect;
            _emptyStroke.Color = EmptyOutline;
            _emptyStroke.StrokeWidth = g.DipScale;
            _vignettePaint.Shader = _vignette;

            DisposeObject(oldEmpty);
            DisposeObject(oldDash);
            DisposeObject(oldCorner);
            DisposeObject(oldGlow);
            DisposeObject(oldVignette);

            float b = (float)(GHRadialTuning.RingBoundsOfOuter * g.OuterRadius);
            _ringBounds = new SKRect(g.CenterX - b, g.CenterY - b, g.CenterX + b, g.CenterY + b);

            DisposeSlicePaths();
            DisposeObject(_wedgePath);
            float s = (float)(GHRadialTuning.WedgeSizeOfHub * g.HubRadius);
            SKPoint[] wedge = { new SKPoint(-s, s * 0.25f), new SKPoint(s, s * 0.25f), new SKPoint(0, -s * 0.9f) };
            _wedgePath = CreatePolygonPath(wedge, wedge.Length);

            /* A five-point star around the origin, as tall as a slot badge */
            DisposeObject(_starPath);
            float outer = SlotBadgeHeight(g) * 0.55f;
            float inner = outer * 0.45f;
            SKPoint[] star = new SKPoint[10];
            int i;
            for (i = 0; i < star.Length; i++)
                star[i] = PolarPoint(i * 36.0, i % 2 == 0 ? outer : inner);
            _starPath = CreatePolygonPath(star, star.Length);
        }

        private SKPath[] GetSlicePaths(int n, GHRadialGeometry g)
        {
            SKPath[] paths = _slicePaths[n];
            if (paths != null)
                return paths;
            paths = new SKPath[n];
            float segmentLength = (float)Math.Max(1.0, GHRadialTuning.ArcSegmentOfCorner * g.CornerRadius);
            double half = 180.0 / n;
            double outerInset = GHRadialGeometry.InsetDegrees(g.SliceGap, g.OuterRadius);
            double innerInset = GHRadialGeometry.InsetDegrees(g.SliceGap, g.InnerRadius);
            int k;
            for (k = 0; k < n; k++)
            {
                double c = k * 360.0 / n;
                double o0 = c - half + outerInset;
                double o1 = c + half - outerInset;
                double i0 = c - half + innerInset;
                double i1 = c + half - innerInset;
                int outerSegments = ArcSegments(o1 - o0, g.OuterRadius, segmentLength);
                int innerSegments = ArcSegments(i1 - i0, g.InnerRadius, segmentLength);
                SKPoint[] pts = new SKPoint[outerSegments + innerSegments + 2];
                int count = 0;
                int j;
                for (j = 0; j <= outerSegments; j++)
                    pts[count++] = PolarPoint(o0 + (o1 - o0) * j / outerSegments, g.OuterRadius);
                for (j = 0; j <= innerSegments; j++)
                    pts[count++] = PolarPoint(i1 - (i1 - i0) * j / innerSegments, g.InnerRadius);
                paths[k] = CreatePolygonPath(pts, count);
            }
            _slicePaths[n] = paths;
            return paths;
        }

        private static int ArcSegments(double sweepDegrees, float radius, float segmentLength)
        {
            double length = Math.Abs(sweepDegrees) * Math.PI / 180.0 * radius;
            return Math.Max(2, (int)Math.Ceiling(length / segmentLength));
        }

        /* A point relative to the ring center */
        private static SKPoint PolarPoint(double compassDegrees, float radius)
        {
            double a = compassDegrees * Math.PI / 180.0;
            return new SKPoint((float)(Math.Sin(a) * radius), (float)(-Math.Cos(a) * radius));
        }

        private static SKPath CreatePolygonPath(SKPoint[] pts, int count)
        {
#if GNH_MAUI && !SKIASHARP_3119
            using (SKPathBuilder builder = new SKPathBuilder())
            {
                builder.MoveTo(pts[0].X, pts[0].Y);
                int i;
                for (i = 1; i < count; i++)
                    builder.LineTo(pts[i].X, pts[i].Y);
                builder.Close();
                return builder.Snapshot();
            }
#else
            SKPath path = new SKPath();
            path.MoveTo(pts[0].X, pts[0].Y);
            int i;
            for (i = 1; i < count; i++)
                path.LineTo(pts[i].X, pts[i].Y);
            path.Close();
            return path;
#endif
        }

        private void DisposeSlicePaths()
        {
            int n;
            for (n = 0; n < _slicePaths.Length; n++)
            {
                SKPath[] paths = _slicePaths[n];
                if (paths == null)
                    continue;
                int k;
                for (k = 0; k < paths.Length; k++)
                    DisposeObject(paths[k]);
                _slicePaths[n] = null;
            }
        }

        private static void DisposeObject(IDisposable obj)
        {
            if (obj != null)
                obj.Dispose();
        }

        private static SKColor LerpColor(SKColor a, SKColor b, float t)
        {
            if (t <= 0f)
                return a;
            if (t >= 1f)
                return b;
            return new SKColor(
                (byte)(a.Red + (b.Red - a.Red) * t + 0.5f),
                (byte)(a.Green + (b.Green - a.Green) * t + 0.5f),
                (byte)(a.Blue + (b.Blue - a.Blue) * t + 0.5f),
                (byte)(a.Alpha + (b.Alpha - a.Alpha) * t + 0.5f));
        }

        /* a's color moved toward tint's by t, keeping a's alpha */
        private static SKColor TintColor(SKColor a, SKColor tint, float t)
        {
            if (t <= 0f)
                return a;
            if (t > 1f)
                t = 1f;
            return new SKColor(
                (byte)(a.Red + (tint.Red - a.Red) * t + 0.5f),
                (byte)(a.Green + (tint.Green - a.Green) * t + 0.5f),
                (byte)(a.Blue + (tint.Blue - a.Blue) * t + 0.5f),
                a.Alpha);
        }

        private static byte ToByte(float unit)
        {
            if (!(unit > 0f))
                return 0;
            if (unit >= 1f)
                return 255;
            return (byte)(unit * 255f + 0.5f);
        }

        private static float Snap(float v)
        {
            return (float)Math.Round(v);
        }

        public void Dispose()
        {
            if (_disposed)
                return;
            _disposed = true;
            _sliceFill.PathEffect = null;
            _sliceStroke.PathEffect = null;
            _glowPaint.PathEffect = null;
            _glowPaint.MaskFilter = null;
            _emptyStroke.PathEffect = null;
            _placeholderStroke.PathEffect = null;
            _vignettePaint.Shader = null;
            DisposeObject(_emptyEffect);
            DisposeObject(_dashEffect);
            DisposeObject(_cornerEffect);
            DisposeObject(_glowFilter);
            DisposeObject(_vignette);
            DisposeObject(_wedgePath);
            DisposeObject(_starPath);
            _starPath = null;
            DisposeSlicePaths();
            _textPaint.Dispose();
            _plainFill.Dispose();
            _plainStroke.Dispose();
            _sliceFill.Dispose();
            _sliceStroke.Dispose();
            _emptyStroke.Dispose();
            _glowPaint.Dispose();
            _vignettePaint.Dispose();
            _layerPaint.Dispose();
            _iconPaint.Dispose();
            _placeholderStroke.Dispose();
            _markStroke.Dispose();
            _contentLayerPaint.Dispose();
            DisposeObject(_badgeRenderer);
            _badgeRenderer = null;
            _itemLayouts.Clear();
        }
    }
}
