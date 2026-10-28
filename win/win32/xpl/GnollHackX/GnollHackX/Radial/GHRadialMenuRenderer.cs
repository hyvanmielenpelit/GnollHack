using SkiaSharp;
using System;
using System.Collections.Generic;
#if GNH_MAUI
using GnollHackM;
#endif

namespace GnollHackX
{
    /* Draws a GHRadialMenuState: backdrop, vignette, slices, hub, page title and dots, the L1/R1
       chips and the hint bar.
       - Slice paths, the vignette shader, the blur mask filter and the path effects are cached
         per layout; item, page and hint text layouts are cached per definition and layout. A
         paint at rest therefore allocates nothing; the first paint after opening fills the
         caches.
       - Free of GHApp: the host supplies the typefaces, the icon lookup, the text blob cache
         setting and the keyboard shortcut setting.
       - Opacity and scale animate through a layer and a canvas transform only while animating;
         text origins snap to whole pixels at rest.
       Single-threaded: paint on the thread that owns the canvas. */
    public sealed class GHRadialMenuRenderer : IDisposable
    {
        /* Host-supplied */
        public Func<string, SKImage> IconLookup;    /* cached images; the renderer never disposes them */
        public SKTypeface TitleTypeface;            /* Immortal */
        public SKTypeface LabelTypeface;            /* Lato Regular */
        public SKTypeface BoldTypeface;             /* Lato Bold */
        public SKTypeface DescriptionTypeface;      /* Underwood */
        public bool UseTextBlobCaching;
        public bool ShowKeyboardShortcuts;

        public static readonly string CurrentlyText = "Currently: ";
        public static readonly string OnText = "On";
        public static readonly string OffText = "Off";
        public static readonly string PillOnText = "ON";
        public static readonly string PillOffText = "OFF";

        private static readonly SKColor Gold = new SKColor(0xD4, 0xA0, 0x17);
        private static readonly SKColor SliceFill = new SKColor(38, 38, 38, 0xD9);
        private static readonly SKColor HighlightFill = new SKColor(58, 46, 10, 0xE6);
        private static readonly SKColor FlashFill = new SKColor(128, 102, 26, 0xF0);
        private static readonly SKColor SliceBorder = new SKColor(255, 255, 255, 0x40);
        private static readonly SKColor EmptyOutline = new SKColor(255, 255, 255, 0x20);
        private static readonly SKColor DisabledBorder = new SKColor(128, 128, 128, 0xFF);
        private static readonly SKColor HubFill = new SKColor(0, 0, 0, 0xBF);
        private static readonly SKColor LighterGray = new SKColor(204, 204, 204);
        private static readonly SKColor TextGray = new SKColor(128, 128, 128);
        private static readonly SKColor OnGreen = SKColors.LightGreen;
        private static readonly SKColor PillDarkText = new SKColor(20, 20, 20);
        private static readonly SKColor StickGhostColor = new SKColor(255, 255, 255, 0x50);
        private static readonly SKColor HintDisc = new SKColor(255, 255, 255, 0xE6);
        private static readonly SKColor HintGlyphText = new SKColor(24, 24, 24);
        private static readonly SKColor VignetteInner = new SKColor(0, 0, 0, 0x40);
        private static readonly SKColor VignetteOuter = new SKColor(0, 0, 0, 0);
        private static readonly string Ellipsis = ((char)0x2026).ToString();
        private static readonly char[] WordSeparators = { ' ' };
        private const int MaxCachedSlotCount = 32;

#if GNH_MAUI
        private static readonly SKSamplingOptions IconSampling = new SKSamplingOptions(SKFilterMode.Linear, SKMipmapMode.Linear);
#endif

        private sealed class ItemLayout
        {
            public int SlotCount;
            public string Label;
            public float LabelSize;
            public string HubTitle;
            public float HubTitleSize;
            public readonly string[] DescriptionLines = new string[GHRadialTuning.HubDescriptionMaxLines];
            public int DescriptionLineCount;
        }

        private sealed class HintBar
        {
            public readonly string[] Glyphs = new string[4];
            public readonly string[] Labels = new string[4];
            public readonly float[] GlyphWidths = new float[4];
            public readonly float[] LabelWidths = new float[4];
            public int Count;
            public float TotalWidth;
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

        private SKPathEffect _cornerEffect;
        private SKPathEffect _dashEffect;
        private SKPathEffect _emptyEffect;
        private SKMaskFilter _glowFilter;
        private SKShader _vignette;
        private SKPath _wedgePath;
        private readonly SKPath[][] _slicePaths = new SKPath[MaxCachedSlotCount + 1][];
        private SKRect _ringBounds;
        private int _resourceVersion = -1;

        private readonly Dictionary<GHRadialItem, ItemLayout> _itemLayouts = new Dictionary<GHRadialItem, ItemLayout>();
        private GHRadialMenuDefinition _layoutDefinition;
        private int _layoutVersion = -1;
        private SKTypeface _layoutTitleTypeface;
        private SKTypeface _layoutLabelTypeface;
        private SKTypeface _layoutBoldTypeface;
        private SKTypeface _layoutDescriptionTypeface;
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
        private float _prevGlyphWidth;
        private float _nextGlyphWidth;
        private float _hubSecondaryGlyphWidth;
        private float _hubSecondaryLabelWidth;
        private readonly HintBar _hintsMain = new HintBar();
        private readonly HintBar _hintsSwitched = new HintBar();
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
            _textPaint.IsAntialias = true;
            _textPaint.Style = SKPaintStyle.Fill;
        }

        /* Text drawn by other threads' requests is cleared at the top of the next paint */
        public void RequestTextCacheClear()
        {
            _textPaint.RequestBlobCacheClear();
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
            GHRadialGeometry g = state.Geometry;
            g.Update(info.Width, info.Height, dipScale);
            if (!g.IsValid)
                return;
            EnsureResources(g);
            GHRadialMenuDefinition def = state.Definition;
            EnsureLayouts(def, g);

            float opacity = state.Opacity;
            bool still = !state.IsAnimating;

            _plainFill.Color = new SKColor(0, 0, 0, ToByte(state.BackdropOpacity));
            canvas.DrawRect(0, 0, g.Width, g.Height, _plainFill);
            _vignettePaint.Color = new SKColor(255, 255, 255, ToByte(opacity));
            canvas.DrawCircle(g.CenterX, g.CenterY, (float)(GHRadialTuning.VignetteRadius * g.OuterRadius), _vignettePaint);

            DrawHeader(canvas, state, def, g, opacity, still);
            DrawChips(canvas, state, def, g, opacity, still);
            HintBar hints = def.IsSwitchedSetPage(state.PageIndex) ? _hintsSwitched : _hintsMain;
            DrawHintBar(canvas, hints, g, g.HintCenterY, opacity, still);

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

            GHRadialPage prevPage = def.GetPage(state.PreviousPageIndex);
            if (prevPage != null)
            {
                float t = GHRadialMenuState.SmoothStep(state.PageTransition);
                float rot = (float)GHRadialTuning.PageChangeRotationDegrees * state.PageDirection;
                DrawRing(canvas, state, g, prevPage, false, 1f - t, -rot * t, state.GetPageHighlight(state.PreviousPageIndex), false);
                DrawRing(canvas, state, g, state.CurrentPage, true, t, rot * (1f - t), state.HighlightedSlot, false);
            }
            else
            {
                DrawRing(canvas, state, g, state.CurrentPage, true, 1f, 0f, state.HighlightedSlot, still && scale == 1f);
            }
            DrawHub(canvas, state, def, g, still && scale == 1f);
            canvas.Restore();
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
                    DrawSlice(canvas, state, g, paths[k], item, k, n, w, isCurrent, snap);
                }
            }
            canvas.Restore();
        }

        private void DrawSlice(SKCanvas canvas, GHRadialMenuState state, GHRadialGeometry g, SKPath path, GHRadialItem item,
            int slot, int n, float weight, bool isCurrent, bool snap)
        {
            float e = GHRadialMenuState.SmoothStep(weight);
            double rad = GHRadialGeometry.SlotCenterDegrees(slot, n) * Math.PI / 180.0;
            float dirX = (float)Math.Sin(rad);
            float dirY = -(float)Math.Cos(rad);
            float offset = g.HighlightOffset * e;
            bool enabled = item.IsEnabled;

            canvas.Save();
            canvas.Translate(g.CenterX + dirX * offset, g.CenterY + dirY * offset);
            if (e > 0f && enabled)
            {
                _glowPaint.Color = Gold.WithAlpha(ToByte(e * 0x60 / 255f));
                canvas.DrawPath(path, _glowPaint);
            }
            SKColor fill = LerpColor(SliceFill, HighlightFill, enabled ? e : e * 0.5f);
            float flash = isCurrent && state.FlashSlot == slot ? state.FlashIntensity : 0f;
            if (flash > 0f)
                fill = LerpColor(fill, FlashFill, flash);
            _sliceFill.Color = fill;
            canvas.DrawPath(path, _sliceFill);
            _sliceStroke.Color = LerpColor(SliceBorder, enabled ? Gold : DisabledBorder, e);
            _sliceStroke.StrokeWidth = g.DipScale * (1f + ((float)GHRadialTuning.HighlightBorderDip - 1f) * e);
            canvas.DrawPath(path, _sliceStroke);
            canvas.Restore();

            DrawSlotContents(canvas, g, item, n, dirX, dirY, offset, e, enabled, snap);
        }

        private void DrawSlotContents(SKCanvas canvas, GHRadialGeometry g, GHRadialItem item, int n, float dirX, float dirY,
            float offset, float e, bool enabled, bool snap)
        {
            bool isToggle = item.IsToggle;
            bool isOn = isToggle && item.IsOn;
            ItemLayout layout = GetItemLayout(item, n, g);

            float radius = g.MidRadius + offset - (isToggle ? (float)(GHRadialTuning.ToggleContentInset * g.OuterRadius) : 0f);
            float px = g.CenterX + dirX * radius;
            float py = g.CenterY + dirY * radius;
            float iconSize = g.GetIconSize(n) * (1f + ((float)GHRadialTuning.HighlightIconScale - 1f) * e);

            _textPaint.Typeface = LabelTypeface;
            _textPaint.TextSize = layout.LabelSize;
            float labelAscent = -_textPaint.FontMetrics.Ascent;
            float labelHeight = layout.LabelSize * (float)GHRadialTuning.LabelLineHeight;

            Func<string, SKImage> lookup = IconLookup;
            SKImage image = lookup == null ? null : lookup(item.GetIconPath(isOn));
            float top;
            if (image != null)
            {
                top = py - (iconSize + labelHeight) / 2;
                SKRect dest = new SKRect(px - iconSize / 2, top, px + iconSize / 2, top + iconSize);
                _iconPaint.ColorFilter = !enabled ? UIUtils.GrayedColorFilter : (e > 0.5f ? UIUtils.HighlightColorFilter : null);
                _iconPaint.Color = SKColors.White;
                canvas.DrawImage(image, dest
#if GNH_MAUI
                    , IconSampling
#endif
                    , _iconPaint);
                top += iconSize;
            }
            else
            {
                top = py - labelHeight / 2;
            }

            if (!string.IsNullOrEmpty(layout.Label))
            {
                _textPaint.Typeface = LabelTypeface;
                _textPaint.TextSize = layout.LabelSize;
                _textPaint.Color = enabled ? SKColors.White : SKColors.White.WithAlpha(0x80);
                float tx = px;
                float ty = top + labelAscent;
                if (snap)
                {
                    tx = Snap(tx);
                    ty = Snap(ty);
                }
                _textPaint.DrawTextOnCanvas(canvas, layout.Label, tx, ty, SKTextAlign.Center);
            }

            if (isToggle)
                DrawPill(canvas, g, dirX, dirY, offset, isOn, snap);
        }

        private void DrawPill(SKCanvas canvas, GHRadialGeometry g, float dirX, float dirY, float offset, bool isOn, bool snap)
        {
            float r = g.OuterRadius + offset - (float)(GHRadialTuning.PillInset * g.OuterRadius);
            float px = g.CenterX + dirX * r;
            float py = g.CenterY + dirY * r;
            float fs = g.PillFontSize;
            float h = fs * 1.5f;
            float w = (isOn ? _pillOnWidth : _pillOffWidth) + fs * 1.2f;
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
            _textPaint.DrawTextOnCanvas(canvas, isOn ? PillOnText : PillOffText, tx, ty, SKTextAlign.Center);
        }

        private void DrawHub(SKCanvas canvas, GHRadialMenuState state, GHRadialMenuDefinition def, GHRadialGeometry g, bool snap)
        {
            float cx = g.CenterX;
            float cy = g.CenterY;
            _plainFill.Color = HubFill;
            canvas.DrawCircle(cx, cy, g.HubRadius, _plainFill);
            _plainStroke.Color = Gold;
            _plainStroke.StrokeWidth = (float)(GHRadialTuning.HubBorderDip * g.DipScale);
            canvas.DrawCircle(cx, cy, g.HubRadius, _plainStroke);

            if (state.WedgeOpacity > 0f)
            {
                canvas.Save();
                canvas.RotateDegrees(state.WedgeAngle, cx, cy);
                canvas.Translate(cx, cy - g.HubRadius);
                _plainFill.Color = Gold.WithAlpha(ToByte(state.WedgeOpacity));
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
            if (item != null)
                DrawHubItem(canvas, def, g, item, GetItemLayout(item, page.SlotCount, g), snap);
            else
                DrawHubIdle(canvas, state, def, g, page, snap);
        }

        private void DrawHubItem(SKCanvas canvas, GHRadialMenuDefinition def, GHRadialGeometry g, GHRadialItem item, ItemLayout layout, bool snap)
        {
            bool showStatus = item.IsToggle || !item.IsEnabled;
            bool showShortcut = ShowKeyboardShortcuts && !string.IsNullOrEmpty(item.ShortcutText);
            float descSize = g.HubDescriptionFontSize;

            float titleAscent;
            float descAscent = 0f;
            float statusAscent = 0f;
            float descH = 0f;
            float statusH = 0f;
            float titleH = LineHeight(TitleTypeface, layout.HubTitleSize, out titleAscent);
            if (layout.DescriptionLineCount > 0)
                descH = LineHeight(DescriptionTypeface, descSize, out descAscent);
            if (showStatus || showShortcut)
                statusH = LineHeight(LabelTypeface, _statusSize, out statusAscent);

            float total = titleH + descH * layout.DescriptionLineCount + (showStatus ? statusH : 0f) + (showShortcut ? statusH : 0f);
            float y = g.CenterY - total / 2;
            float cx = g.CenterX;

            _textPaint.Typeface = TitleTypeface;
            _textPaint.TextSize = layout.HubTitleSize;
            _textPaint.Color = SKColors.White;
            DrawCentered(canvas, layout.HubTitle, cx, y + titleAscent, snap);
            y += titleH;

            int i;
            if (layout.DescriptionLineCount > 0)
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
            bool showSecondary = empty && def.SwitchSetPageIndex > 0 && !def.IsSwitchedSetPage(pageIndex) && !string.IsNullOrEmpty(def.SecondaryHint);

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
                float total = _hubSecondaryGlyphWidth + gap + _hubSecondaryLabelWidth;
                float x = cx - total / 2;
                float centerY = y + hintH / 2;
                DrawGlyph(canvas, def.SecondaryGlyph, x + _hubSecondaryGlyphWidth / 2, centerY, g.HintFontSize, _hubSecondaryGlyphWidth, 1f, snap);
                _textPaint.Typeface = LabelTypeface;
                _textPaint.TextSize = g.HintFontSize;
                _textPaint.Color = LighterGray;
                SKFontMetrics m = _textPaint.FontMetrics;
                float tx = x + _hubSecondaryGlyphWidth + gap;
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
                _textPaint.Color = SKColors.White.WithAlpha(a);
                DrawCentered(canvas, page.Title, g.CenterX, g.PageTitleBaselineY, snap);
            }

            /* One dot per page of the current page's set, so that a set's first page lights the
               first dot */
            int first, count;
            def.GetSetRange(pageIndex, out first, out count);
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

        private void DrawChips(SKCanvas canvas, GHRadialMenuState state, GHRadialMenuDefinition def, GHRadialGeometry g, float opacity, bool snap)
        {
            if (!_chipsVisible || def.PageCount <= 1)
                return;
            int pageIndex = state.PageIndex;
            if (pageIndex > 0)
                DrawChip(canvas, g, g.CenterX - g.ChipOffsetX, def.PrevPageGlyph, _prevGlyphWidth, _chipTitles[pageIndex - 1], opacity, snap);
            if (pageIndex < def.PageCount - 1)
                DrawChip(canvas, g, g.CenterX + g.ChipOffsetX, def.NextPageGlyph, _nextGlyphWidth, _chipTitles[pageIndex + 1], opacity, snap);
        }

        private void DrawChip(SKCanvas canvas, GHRadialGeometry g, float x, string glyph, float glyphWidth, string title, float opacity, bool snap)
        {
            float fs = g.HintFontSize;
            float glyphCenterY = g.CenterY - fs * 0.6f;
            DrawGlyph(canvas, glyph, x, glyphCenterY, fs, glyphWidth, opacity, snap);
            if (!string.IsNullOrEmpty(title))
            {
                _textPaint.Typeface = LabelTypeface;
                _textPaint.TextSize = fs;
                _textPaint.Color = LighterGray.WithAlpha(ToByte(opacity));
                float baseline = glyphCenterY + fs * 0.72f + fs * 0.4f - _textPaint.FontMetrics.Ascent;
                DrawCentered(canvas, title, x, baseline, snap);
            }
        }

        private void DrawHintBar(SKCanvas canvas, HintBar bar, GHRadialGeometry g, float centerY, float opacity, bool snap)
        {
            if (bar.Count == 0)
                return;
            float fs = g.HintFontSize;
            float smallGap = fs * 0.4f;
            float largeGap = fs * 1.4f;
            float x = g.CenterX - bar.TotalWidth / 2;
            _textPaint.Typeface = LabelTypeface;
            _textPaint.TextSize = fs;
            SKFontMetrics m = _textPaint.FontMetrics;
            float labelBaseline = centerY - (m.Ascent + m.Descent) / 2;
            int i;
            for (i = 0; i < bar.Count; i++)
            {
                DrawGlyph(canvas, bar.Glyphs[i], x + bar.GlyphWidths[i] / 2, centerY, fs, bar.GlyphWidths[i], opacity, snap);
                x += bar.GlyphWidths[i] + smallGap;
                _textPaint.Typeface = LabelTypeface;
                _textPaint.TextSize = fs;
                _textPaint.Color = SKColors.White.WithAlpha(ToByte(opacity));
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

        /* A controller button: a disc for one character, a pill for longer names such as L1 */
        private void DrawGlyph(SKCanvas canvas, string glyph, float centerX, float centerY, float fontSize, float width, float opacity, bool snap)
        {
            if (string.IsNullOrEmpty(glyph))
                return;
            float h = fontSize * 1.44f;
            _plainFill.Color = HintDisc.WithAlpha(ToByte(opacity * HintDisc.Alpha / 255f));
            if (glyph.Length == 1)
                canvas.DrawCircle(centerX, centerY, h / 2, _plainFill);
            else
                canvas.DrawRoundRect(new SKRect(centerX - width / 2, centerY - h / 2, centerX + width / 2, centerY + h / 2), h / 2, h / 2, _plainFill);
            _textPaint.Typeface = BoldTypeface;
            _textPaint.TextSize = fontSize * 0.8f;
            _textPaint.Color = HintGlyphText.WithAlpha(ToByte(opacity));
            SKFontMetrics m = _textPaint.FontMetrics;
            float tx = centerX;
            float ty = centerY - (m.Ascent + m.Descent) / 2;
            if (snap)
            {
                tx = Snap(tx);
                ty = Snap(ty);
            }
            _textPaint.DrawTextOnCanvas(canvas, glyph, tx, ty, SKTextAlign.Center);
        }

        /* Measures with the bold glyph font; call only while caches are built */
        private float GetGlyphWidth(string glyph, float fontSize)
        {
            if (string.IsNullOrEmpty(glyph))
                return 0f;
            float h = fontSize * 1.44f;
            if (glyph.Length == 1)
                return h;
            _textPaint.Typeface = BoldTypeface;
            _textPaint.TextSize = fontSize * 0.8f;
            return Math.Max(h, _textPaint.MeasureText(glyph) + fontSize * 0.9f);
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

        private ItemLayout GetItemLayout(GHRadialItem item, int slotCount, GHRadialGeometry g)
        {
            ItemLayout layout;
            if (_itemLayouts.TryGetValue(item, out layout) && layout.SlotCount == slotCount)
                return layout;

            layout = new ItemLayout();
            layout.SlotCount = slotCount;
            float labelMax = g.GetLabelMaxWidth(slotCount);
            layout.LabelSize = FitSize(item.Label, LabelTypeface, g.LabelFontSize, Math.Min(g.LabelMinFontSize, g.LabelFontSize), labelMax);
            layout.Label = Ellipsize(item.Label, LabelTypeface, layout.LabelSize, labelMax);

            float hubWidth = (float)(GHRadialTuning.HubTitleWidthOfHubDiameter * 2 * g.HubRadius);
            float titleMin = (float)(GHRadialTuning.HubTitleMinOfFont * g.HubTitleFontSize);
            layout.HubTitleSize = FitSize(item.Label, TitleTypeface, g.HubTitleFontSize, titleMin, hubWidth);
            layout.HubTitle = Ellipsize(item.Label, TitleTypeface, layout.HubTitleSize, hubWidth);

            /* A description that only repeats the label gets no lines; DrawHubItem centers the lines that remain */
            float descWidth = (float)(GHRadialTuning.HubDescriptionWidthOfHubDiameter * 2 * g.HubRadius);
            layout.DescriptionLineCount = IsDescriptionRedundant(item.Description, item.Label) ? 0
                : WrapText(item.Description, DescriptionTypeface, g.HubDescriptionFontSize, descWidth, layout.DescriptionLines);
            _itemLayouts[item] = layout;
            return layout;
        }

        /* Empty, or the label again once both are trimmed */
        private static bool IsDescriptionRedundant(string description, string label)
        {
            if (string.IsNullOrWhiteSpace(description))
                return true;
            return label != null && string.Equals(description.Trim(), label.Trim(), StringComparison.Ordinal);
        }

        private void EnsureLayouts(GHRadialMenuDefinition def, GHRadialGeometry g)
        {
            if (ReferenceEquals(def, _layoutDefinition) && g.Version == _layoutVersion
                && ReferenceEquals(TitleTypeface, _layoutTitleTypeface) && ReferenceEquals(LabelTypeface, _layoutLabelTypeface)
                && ReferenceEquals(BoldTypeface, _layoutBoldTypeface) && ReferenceEquals(DescriptionTypeface, _layoutDescriptionTypeface))
                return;

            _itemLayouts.Clear();
            _layoutDefinition = def;
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
            _chipsVisible = chipHalf > g.HintFontSize * 2;
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
                _chipTitles[i] = _chipsVisible ? Ellipsize(title, LabelTypeface, g.HintFontSize, chipMax) : null;
            }
            _stickTextSize = FitSize(def.NoHighlightStickText, LabelTypeface, _idleTextBaseSize, _idleTextBaseSize * 0.7f, hubWidth);
            _pointerTextSize = FitSize(def.NoHighlightPointerText, LabelTypeface, _idleTextBaseSize, _idleTextBaseSize * 0.7f, hubWidth);

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

            _prevGlyphWidth = GetGlyphWidth(def.PrevPageGlyph, g.HintFontSize);
            _nextGlyphWidth = GetGlyphWidth(def.NextPageGlyph, g.HintFontSize);
            _hubSecondaryGlyphWidth = GetGlyphWidth(def.SecondaryGlyph, g.HintFontSize);
            _hubSecondaryLabelWidth = MeasureLabel(def.SecondaryHint, g.HintFontSize);

            BuildHintBar(_hintsMain, def, g, false);
            BuildHintBar(_hintsSwitched, def, g, true);
        }

        private void BuildHintBar(HintBar bar, GHRadialMenuDefinition def, GHRadialGeometry g, bool switched)
        {
            bar.Count = 0;
            AddHint(bar, def.ConfirmGlyph, def.ConfirmHint, g);
            AddHint(bar, def.CancelGlyph, def.CancelHint, g);
            if (def.SwitchSetPageIndex > 0)
                AddHint(bar, def.SecondaryGlyph, switched ? def.SecondaryHintOnSwitchedSet : def.SecondaryHint, g);
            if (def.HasGroups)
                AddHint(bar, def.GroupGlyph, def.GroupHint, g);

            float fs = g.HintFontSize;
            float total = 0f;
            int i;
            for (i = 0; i < bar.Count; i++)
            {
                total += bar.GlyphWidths[i] + fs * 0.4f + bar.LabelWidths[i];
                if (i > 0)
                    total += fs * 1.4f;
            }
            bar.TotalWidth = total;
        }

        private void AddHint(HintBar bar, string glyph, string label, GHRadialGeometry g)
        {
            if (string.IsNullOrEmpty(label) || string.IsNullOrEmpty(glyph) || bar.Count >= bar.Glyphs.Length)
                return;
            bar.Glyphs[bar.Count] = glyph;
            bar.Labels[bar.Count] = label;
            bar.GlyphWidths[bar.Count] = GetGlyphWidth(glyph, g.HintFontSize);
            bar.LabelWidths[bar.Count] = MeasureLabel(label, g.HintFontSize);
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

        /* Greedy word wrap into lines; the last line takes an ellipsis when text remains */
        private int WrapText(string text, SKTypeface typeface, float size, float maxWidth, string[] lines)
        {
            if (string.IsNullOrEmpty(text) || lines.Length == 0)
                return 0;
            string[] words = text.Split(WordSeparators, StringSplitOptions.RemoveEmptyEntries);
            _textPaint.Typeface = typeface;
            _textPaint.TextSize = size;
            int count = 0;
            int w = 0;
            while (w < words.Length && count < lines.Length)
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
                if (count == lines.Length - 1 && w < words.Length)
                    line = Ellipsize(line + " " + string.Join(" ", words, w, words.Length - w), typeface, size, maxWidth);
                else if (_textPaint.MeasureText(line) > maxWidth)
                    line = Ellipsize(line, typeface, size, maxWidth);
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
            _vignettePaint.Shader = null;
            DisposeObject(_emptyEffect);
            DisposeObject(_dashEffect);
            DisposeObject(_cornerEffect);
            DisposeObject(_glowFilter);
            DisposeObject(_vignette);
            DisposeObject(_wedgePath);
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
            _itemLayouts.Clear();
        }
    }
}
