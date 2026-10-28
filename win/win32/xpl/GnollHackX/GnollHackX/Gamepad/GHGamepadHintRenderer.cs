using SkiaSharp;
using System;
using System.Collections.Generic;
#if GNH_MAUI
using GnollHackM;
#endif
#if GNH_MAUI && !SKIASHARP_3119
using PathSink = SkiaSharp.SKPathBuilder;
#else
using PathSink = SkiaSharp.SKPath;
#endif

namespace GnollHackX
{
    /* Draws controller button badges and rows of hints (badges followed by labels).
       - A badge is a vector shape sized by a height in pixels. The height is rounded to whole
         pixels and the badge's left and top edges snap to whole pixels, so straight edges are
         crisp. Shape paths are cached per shape and height (two heights per shape) and badge text
         sizes per text and size, so a paint at rest allocates nothing.
       - A hint with Hold gets an outlined up arrow (the hold mark) after its badges.
       - LayoutRow measures a row once and splits it into two lines: before the first entry, other
         than the first, that has LineBreakBefore, else into two balanced lines when it does not
         fit. DrawRow draws that layout.
       - Free of GHApp: the host supplies the typefaces and the text blob cache setting.
       Single-threaded: one instance per painting canvas, used on the thread that owns it. */
    public sealed class GHGamepadHintRenderer : IDisposable
    {
        /* Host-supplied */
        public SKTypeface LabelTypeface;    /* Lato Regular */
        public SKTypeface BoldTypeface;     /* Lato Bold */
        public bool UseTextBlobCaching;

        public static readonly SKColor BadgeFill = new SKColor(255, 255, 255, 0xE6);
        public static readonly SKColor BadgeInk = new SKColor(24, 24, 24);
        public static readonly SKColor HighlightFill = new SKColor(0xD4, 0xA0, 0x17);
        public static readonly SKColor LabelColor = SKColors.White;
        public static readonly SKColor BackingColor = new SKColor(0, 0, 0, 0x99);

        /* Row metrics as multiples of the label font size */
        public const float BadgeHeightOfFont = 1.44f;
        public const float PairGapOfFont = 0.25f;
        public const float LabelGapOfFont = 0.4f;
        public const float EntryGapOfFont = 1.4f;
        public const float LineHeightOfFont = 1.9f;
        public const float BackingPadOfFont = 0.6f;
        public const float MinFontOfRequested = 0.7f;

        /* Hold mark metrics as multiples of the badge height */
        public const float HoldMarkHeightOfBadge = 0.8f;
        public const float HoldMarkStrokeOfBadge = 0.08f;
        public const float HoldMarkMinStroke = 1.25f;

        private const float CurveKappa = 0.5523f;
        private const int PathWays = 2;
        private const int MaxPolygonVertices = 12;
        private const int FitCacheSize = 24;

        private enum PathSlot
        {
            ShiftArrow,             /* the hold mark */
            Bumper,
            Trigger,
            StickClick,
            GripLeft,
            GripRight,
            DPad,
            PlayStationTriangle,    /* relative to the badge center */
            StickArrow,             /* relative to the badge center, pointing up */
            Count
        }

        private readonly GHSkiaFontPaint _fontPaint = new GHSkiaFontPaint();
        private readonly SKPaint _fill = new SKPaint();
        private readonly SKPaint _stroke = new SKPaint();

        private readonly SKPath[] _paths = new SKPath[(int)PathSlot.Count * PathWays];
        private readonly int[] _pathHeights = new int[(int)PathSlot.Count * PathWays];
        private readonly long[] _pathStamps = new long[(int)PathSlot.Count * PathWays];
        private long _pathClock;

        /* Polygon scratch for path building */
        private readonly float[] _vx = new float[MaxPolygonVertices];
        private readonly float[] _vy = new float[MaxPolygonVertices];
        private readonly float[] _vrIn = new float[MaxPolygonVertices];
        private readonly float[] _vrOut = new float[MaxPolygonVertices];
        private int _vertexCount;

        /* Fitted badge text sizes, replaced round-robin */
        private readonly string[] _fitTexts = new string[FitCacheSize];
        private readonly SKTypeface[] _fitTypefaces = new SKTypeface[FitCacheSize];
        private readonly float[] _fitBaseSizes = new float[FitCacheSize];
        private readonly float[] _fitMaxWidths = new float[FitCacheSize];
        private readonly float[] _fitSizes = new float[FitCacheSize];
        private int _fitNext;

        /* Row layout, reused across LayoutRow calls */
        private readonly List<float> _badge1Widths = new List<float>();
        private readonly List<float> _badge2Widths = new List<float>();     /* 0 without a second badge */
        private readonly List<float> _holdWidths = new List<float>();       /* the hold mark with its gap; 0 without Hold */
        private readonly List<float> _entryWidths = new List<float>();
        private readonly List<float> _entryOffsets = new List<float>();     /* from the start of the entry's line */
        private int _entryCount;
        private int _secondLineStart;       /* the first entry on the second line; _entryCount with one line */
        private float _firstLineWidth;
        private float _secondLineWidth;
        private bool _disposed;

        public GHGamepadHintRenderer()
        {
            _fill.IsAntialias = true;
            _fill.Style = SKPaintStyle.Fill;
            _stroke.IsAntialias = true;
            _stroke.Style = SKPaintStyle.Stroke;
            _stroke.StrokeCap = SKStrokeCap.Round;
            _stroke.StrokeJoin = SKStrokeJoin.Round;
            _fontPaint.IsAntialias = true;
            _fontPaint.Style = SKPaintStyle.Fill;
        }

        /* The last LayoutRow: line count, widest line, total line height and effective font size */
        public int LineCount { get; private set; }
        public float RowWidth { get; private set; }
        public float RowHeight { get; private set; }
        public float FontSize { get; private set; }
        public float LineHeight { get { return FontSize * LineHeightOfFont; } }

        /* How far DrawRow's backing extends beyond the row box on each side */
        public float BackingPadX { get { return FontSize * BackingPadOfFont; } }
        public float BackingPadY { get { return Math.Max(0f, FontSize * (BackingPadOfFont - (LineHeightOfFont - BadgeHeightOfFont) * 0.5f)); } }

        /* Call once at the top of a paint that draws badges without DrawRow, before any badge;
           DrawRow calls it itself */
        public void BeginPaint()
        {
            _fontPaint.SyncBlobCache(UseTextBlobCaching);
        }

        /* Callable from any thread; the clear happens at the next BeginPaint */
        public void RequestTextCacheClear()
        {
            _fontPaint.RequestBlobCacheClear();
        }

        /* Badges */

        public float MeasureBadge(GHGamepadButton button, GHGamepadStyle style, float height)
        {
            if (!(height > 0f))
                return 0f;
            return BadgeWidth(GHGamepadGlyphs.GetBadgeKind(button, style), QuantizeHeight(height));
        }

        public void DrawBadge(SKCanvas canvas, GHGamepadButton button, GHGamepadStyle style, float cx, float cy, float height, float opacity, bool highlighted)
        {
            GHGamepadBadgeKind kind = GHGamepadGlyphs.GetBadgeKind(button, style);
            DrawBadgeCore(canvas, kind, button, GHGamepadGlyphs.GetBadgeText(button, style), GetDirection(button),
                button == GHGamepadButton.R4 || button == GHGamepadButton.R5, cx, cy, height, opacity, highlighted);
        }

        public float MeasureSymbolBadge(GHGamepadHintSymbol symbol, float height)
        {
            if (!(height > 0f))
                return 0f;
            return BadgeWidth(GHGamepadGlyphs.GetSymbolBadgeKind(symbol), QuantizeHeight(height));
        }

        public void DrawSymbolBadge(SKCanvas canvas, GHGamepadHintSymbol symbol, float cx, float cy, float height, float opacity, bool highlighted)
        {
            DrawBadgeCore(canvas, GHGamepadGlyphs.GetSymbolBadgeKind(symbol), GHGamepadButton.A, GHGamepadGlyphs.GetSymbolBadgeText(symbol), -1,
                false, cx, cy, height, opacity, highlighted);
        }

        /* The badges of one hint: one badge, a pair with a small gap, or the symbol badge; with Hold,
           followed by the hold mark after the same gap */
        public float MeasureHintBadges(GHGamepadHint hint, GHGamepadStyle style, float height)
        {
            float b1, b2;
            MeasureHintBadgeWidths(hint, style, height, out b1, out b2);
            float pairGap = height * PairGapOfFont / BadgeHeightOfFont;
            float width = b2 > 0f ? b1 + pairGap + b2 : b1;
            if (hint.Hold)
                width += pairGap + MeasureHoldMark(height);
            return width;
        }

        /* Draws the badges of one hint, and the hold mark with Hold, from left, centered on centerY;
           returns their width */
        public float DrawHintBadges(SKCanvas canvas, GHGamepadHint hint, GHGamepadStyle style, float left, float centerY, float height, float opacity)
        {
            float b1, b2;
            MeasureHintBadgeWidths(hint, style, height, out b1, out b2);
            float pairGap = height * PairGapOfFont / BadgeHeightOfFont;
            float width = b2 > 0f ? b1 + pairGap + b2 : b1;
            DrawEntryBadges(canvas, hint, style, left, centerY, height, b1, b2, pairGap, opacity);
            if (hint.Hold)
            {
                DrawHoldMark(canvas, left + width + pairGap, centerY, height, opacity, hint.Highlight);
                width += pairGap + MeasureHoldMark(height);
            }
            return width;
        }

        /* The hold mark that follows the badges of a hint with Hold: a Shift-style up arrow, outlined.
           height is the badge height; the width includes the stroke. */
        public float MeasureHoldMark(float height)
        {
            if (!(height > 0f))
                return 0f;
            return ShiftArrowWidth(QuantizeHeight(height * HoldMarkHeightOfBadge)) + HoldMarkStrokePad(height) * 2;
        }

        /* Draws the hold mark from left, centered on centerY, in the label color, or in the highlight
           color when highlighted */
        public void DrawHoldMark(SKCanvas canvas, float left, float centerY, float height, float opacity, bool highlighted)
        {
            if (canvas == null || _disposed || !(height > 0f) || !(opacity > 0f))
                return;
            int h = QuantizeHeight(height * HoldMarkHeightOfBadge);
            _stroke.Color = Fade(highlighted ? HighlightFill : LabelColor, opacity);
            _stroke.StrokeWidth = HoldMarkStroke(height);
            DrawPathAt(canvas, GetPath(PathSlot.ShiftArrow, h), Snap(left) + HoldMarkStrokePad(height), Snap(centerY - h * 0.5f), _stroke);
        }

        /* Hint rows */

        /* Measures the row at fontSize. When an entry other than the first has LineBreakBefore, the
           first such entry starts the second line. Otherwise, when the row is wider than maxWidth
           (0 or less: unlimited), splits it into two lines, the longer as short as possible and the
           first the longer on a tie. Then shrinks the font, not below MinFontOfRequested of fontSize,
           until the lines fit. */
        public void LayoutRow(IList<GHGamepadHint> hints, GHGamepadStyle style, float fontSize, float maxWidth)
        {
            _entryCount = 0;
            _secondLineStart = 0;
            _firstLineWidth = 0f;
            _secondLineWidth = 0f;
            _entryOffsets.Clear();
            LineCount = 0;
            RowWidth = 0f;
            RowHeight = 0f;
            FontSize = fontSize;
            if (_disposed || hints == null || hints.Count == 0 || !(fontSize > 0f))
                return;

            bool limited = maxWidth > 0f;
            float limit = limited ? maxWidth : float.MaxValue;
            float minSize = fontSize * MinFontOfRequested;
            float size = fontSize;
            float widest = 0f;
            int forcedBreak = 0;
            int i;
            for (i = 1; i < hints.Count; i++)
            {
                if (hints[i].LineBreakBefore)
                {
                    forcedBreak = i;
                    break;
                }
            }

            int attempt;
            for (attempt = 0; ; attempt++)
            {
                MeasureEntries(hints, style, size);
                SplitLines(size, limit, forcedBreak);
                widest = Math.Max(_firstLineWidth, _secondLineWidth);
                if (!limited || widest <= maxWidth || size <= minSize || attempt >= 7)
                    break;
                float next = Math.Min(size * maxWidth / widest, size * 0.98f);
                size = Math.Max(minSize, next);
            }

            FontSize = size;
            LineCount = _secondLineStart < _entryCount ? 2 : 1;
            RowWidth = widest;
            RowHeight = LineCount * size * LineHeightOfFont;

            float gap = size * EntryGapOfFont;
            float x = 0f;
            for (i = 0; i < _entryCount; i++)
            {
                if (i == _secondLineStart)
                    x = 0f;
                _entryOffsets.Add(x);
                x += _entryWidths[i] + gap;
            }
        }

        /* Draws the last LayoutRow; hints and style must be the ones passed to it. Each line is
           centered on centerX, the last line's box ends at bottomY, and lines stack upward. The
           backing extends BackingPadX and BackingPadY beyond the row box. */
        public void DrawRow(SKCanvas canvas, IList<GHGamepadHint> hints, GHGamepadStyle style, float centerX, float bottomY, float opacity, bool drawBacking)
        {
            if (canvas == null || _disposed || hints == null || LineCount == 0 || !(opacity > 0f))
                return;
            BeginPaint();
            int n = Math.Min(hints.Count, _entryCount);
            float size = FontSize;
            float badgeHeight = size * BadgeHeightOfFont;
            float pairGap = size * PairGapOfFont;
            float labelGap = size * LabelGapOfFont;

            if (drawBacking)
            {
                float padX = BackingPadX;
                float padY = BackingPadY;
                SKRect rect = new SKRect(Snap(centerX - RowWidth * 0.5f - padX), Snap(bottomY - RowHeight - padY),
                    Snap(centerX + RowWidth * 0.5f + padX), Snap(bottomY + padY));
                float radius = Math.Min(rect.Height * 0.5f, size * 0.8f);
                _fill.Color = Fade(BackingColor, opacity);
                canvas.DrawRoundRect(rect, radius, radius, _fill);
            }

            /* Badges and hold marks first, then labels, so that the font changes once */
            int i;
            float x, cy;
            for (i = 0; i < n; i++)
            {
                GetEntryOrigin(i, centerX, bottomY, out x, out cy);
                GHGamepadHint hint = hints[i];
                float b1 = _badge1Widths[i];
                float b2 = _badge2Widths[i];
                DrawEntryBadges(canvas, hint, style, x, cy, badgeHeight, b1, b2, pairGap, opacity);
                if (_holdWidths[i] > 0f)
                    DrawHoldMark(canvas, x + (b2 > 0f ? b1 + pairGap + b2 : b1) + pairGap, cy, badgeHeight, opacity, hint.Highlight);
            }

            _fontPaint.Typeface = LabelTypeface;
            _fontPaint.TextSize = size;
            _fontPaint.Color = Fade(LabelColor, opacity);
            SKFontMetrics m = _fontPaint.FontMetrics;
            float baselineOffset = -(m.Ascent + m.Descent) * 0.5f;
            for (i = 0; i < n; i++)
            {
                GHGamepadHint hint = hints[i];
                if (string.IsNullOrEmpty(hint.Label))
                    continue;
                GetEntryOrigin(i, centerX, bottomY, out x, out cy);
                float b2 = _badge2Widths[i];
                float badges = (b2 > 0f ? _badge1Widths[i] + pairGap + b2 : _badge1Widths[i]) + _holdWidths[i];
                float tx = Snap(x + badges + labelGap);
                float ty = Snap(cy + baselineOffset);
                _fontPaint.DrawTextOnCanvas(canvas, hint.Label, tx, ty, SKTextAlign.Left);
            }
        }

        private void GetEntryOrigin(int index, float centerX, float bottomY, out float left, out float centerY)
        {
            int line = index < _secondLineStart ? 0 : 1;
            float lineWidth = line == 0 ? _firstLineWidth : _secondLineWidth;
            float lineHeight = LineHeight;
            centerY = bottomY - (LineCount - 1 - line) * lineHeight - lineHeight * 0.5f;
            left = Snap(centerX - lineWidth * 0.5f) + _entryOffsets[index];
        }

        private void MeasureEntries(IList<GHGamepadHint> hints, GHGamepadStyle style, float size)
        {
            _badge1Widths.Clear();
            _badge2Widths.Clear();
            _holdWidths.Clear();
            _entryWidths.Clear();
            float badgeHeight = size * BadgeHeightOfFont;
            float pairGap = size * PairGapOfFont;
            float labelGap = size * LabelGapOfFont;
            _fontPaint.Typeface = LabelTypeface;
            _fontPaint.TextSize = size;
            float holdWidth = pairGap + MeasureHoldMark(badgeHeight);
            int count = hints.Count;
            int i;
            for (i = 0; i < count; i++)
            {
                GHGamepadHint hint = hints[i];
                float b1, b2;
                MeasureHintBadgeWidths(hint, style, badgeHeight, out b1, out b2);
                float label = string.IsNullOrEmpty(hint.Label) ? 0f : _fontPaint.MeasureText(hint.Label);
                float hold = hint.Hold ? holdWidth : 0f;
                float width = b1;
                if (b2 > 0f)
                    width += pairGap + b2;
                width += hold;
                if (label > 0f)
                    width += labelGap + label;
                _badge1Widths.Add(b1);
                _badge2Widths.Add(b2);
                _holdWidths.Add(hold);
                _entryWidths.Add(width);
            }
            _entryCount = count;
        }

        /* forcedBreak: the entry that starts the second line, or 0 to split only when the row does
           not fit */
        private void SplitLines(float size, float limit, int forcedBreak)
        {
            int n = _entryCount;
            float gap = size * EntryGapOfFont;
            float total = 0f;
            int i;
            for (i = 0; i < n; i++)
                total += _entryWidths[i];
            if (n > 1)
                total += gap * (n - 1);
            if (forcedBreak > 0 && forcedBreak < n)
            {
                float first = 0f;
                for (i = 0; i < forcedBreak; i++)
                    first += _entryWidths[i];
                first += gap * (forcedBreak - 1);
                _secondLineStart = forcedBreak;
                _firstLineWidth = first;
                _secondLineWidth = total - first - gap;
                return;
            }
            if (n <= 1 || total <= limit)
            {
                _secondLineStart = n;
                _firstLineWidth = total;
                _secondLineWidth = 0f;
                return;
            }

            int best = 1;
            float bestLonger = float.MaxValue;
            bool bestFirstLonger = false;
            float bestFirst = 0f;
            float bestSecond = 0f;
            float prefix = 0f;
            int k;
            for (k = 1; k < n; k++)
            {
                prefix += _entryWidths[k - 1];
                if (k > 1)
                    prefix += gap;
                float first = prefix;
                float second = total - prefix - gap;
                float longer = Math.Max(first, second);
                bool firstLonger = first >= second;
                if (longer < bestLonger - 0.5f || (longer <= bestLonger + 0.5f && firstLonger && !bestFirstLonger))
                {
                    best = k;
                    bestLonger = longer;
                    bestFirstLonger = firstLonger;
                    bestFirst = first;
                    bestSecond = second;
                }
            }
            _secondLineStart = best;
            _firstLineWidth = bestFirst;
            _secondLineWidth = bestSecond;
        }

        private void MeasureHintBadgeWidths(GHGamepadHint hint, GHGamepadStyle style, float height, out float b1, out float b2)
        {
            if (hint.Symbol != GHGamepadHintSymbol.None)
            {
                b1 = MeasureSymbolBadge(hint.Symbol, height);
                b2 = 0f;
            }
            else
            {
                b1 = MeasureBadge(hint.Button, style, height);
                b2 = hint.HasButton2 ? MeasureBadge(hint.Button2, style, height) : 0f;
            }
        }

        private void DrawEntryBadges(SKCanvas canvas, GHGamepadHint hint, GHGamepadStyle style, float left, float centerY, float height,
            float b1, float b2, float pairGap, float opacity)
        {
            if (hint.Symbol != GHGamepadHintSymbol.None)
            {
                DrawSymbolBadge(canvas, hint.Symbol, left + b1 * 0.5f, centerY, height, opacity, hint.Highlight);
                return;
            }
            DrawBadge(canvas, hint.Button, style, left + b1 * 0.5f, centerY, height, opacity, hint.Highlight);
            if (b2 > 0f)
                DrawBadge(canvas, hint.Button2, style, left + b1 + pairGap + b2 * 0.5f, centerY, height, opacity, hint.Highlight);
        }

        /* Badge drawing */

        /* direction: 0 up, 1 right, 2 down, 3 left, -1 none */
        private void DrawBadgeCore(SKCanvas canvas, GHGamepadBadgeKind kind, GHGamepadButton button, string text, int direction, bool rightGrip,
            float cx, float cy, float height, float opacity, bool highlighted)
        {
            if (canvas == null || _disposed || !(height > 0f) || !(opacity > 0f))
                return;
            int h = QuantizeHeight(height);
            int w = BadgeWidth(kind, h);
            float left = Snap(cx - w * 0.5f);
            float top = Snap(cy - h * 0.5f);
            float mx = left + w * 0.5f;
            float my = top + h * 0.5f;
            SKColor ink = Fade(BadgeInk, opacity);
            _fill.Color = Fade(highlighted ? HighlightFill : BadgeFill, opacity);

            switch (kind)
            {
            case GHGamepadBadgeKind.PlayStationSymbol:
                canvas.DrawCircle(mx, my, h * 0.5f, _fill);
                DrawPlayStationSymbol(canvas, button, mx, my, h, ink);
                break;
            case GHGamepadBadgeKind.Bumper:
                DrawPathAt(canvas, GetPath(PathSlot.Bumper, h), left, top, _fill);
                DrawBadgeText(canvas, text, mx, top + (BumperBulge(h) + h) * 0.5f, h * 0.5f, w * 0.75f, ink);
                break;
            case GHGamepadBadgeKind.Trigger:
                DrawPathAt(canvas, GetPath(PathSlot.Trigger, h), left, top, _fill);
                DrawBadgeText(canvas, text, mx, top + h * 0.58f, h * 0.5f, w * 0.8f, ink);
                break;
            case GHGamepadBadgeKind.StickClick:
                DrawPathAt(canvas, GetPath(PathSlot.StickClick, h), left, top, _fill);
                DrawBadgeText(canvas, text, mx, top + h * 0.5f, h * 0.44f, w * 0.7f, ink);
                break;
            case GHGamepadBadgeKind.Grip:
            {
                float shift = GripSlant(h) * 0.25f;
                DrawPathAt(canvas, GetPath(rightGrip ? PathSlot.GripRight : PathSlot.GripLeft, h), left, top, _fill);
                DrawBadgeText(canvas, text, rightGrip ? mx - shift : mx + shift, my, h * 0.48f, w * 0.7f, ink);
                break;
            }
            case GHGamepadBadgeKind.Menu:
                DrawRoundedSquare(canvas, left, top, h);
                DrawMenuLines(canvas, left, top, h, ink);
                break;
            case GHGamepadBadgeKind.View:
                DrawRoundedSquare(canvas, left, top, h);
                DrawViewSquares(canvas, left, top, h, ink);
                break;
            case GHGamepadBadgeKind.DPad:
                DrawPathAt(canvas, GetPath(PathSlot.DPad, h), left, top, _fill);
                if (direction >= 0)
                    DrawDPadArm(canvas, left, top, h, direction, ink);
                break;
            case GHGamepadBadgeKind.Stick:
                canvas.DrawCircle(mx, my, h * 0.5f, _fill);
                _stroke.Color = ink;
                _stroke.StrokeWidth = Math.Max(1f, 0.075f * h);
                canvas.DrawCircle(mx, my, 0.35f * h, _stroke);
                if (direction >= 0)
                {
                    _fill.Color = ink;
                    canvas.Save();
                    canvas.Translate(mx, my);
                    if (direction > 0)
                        canvas.RotateDegrees(90f * direction);
                    canvas.DrawPath(GetPath(PathSlot.StickArrow, h), _fill);
                    canvas.Restore();
                }
                DrawBadgeText(canvas, text, mx, my, h * 0.4f, h * 0.46f, ink);
                break;
            default:
                canvas.DrawCircle(mx, my, h * 0.5f, _fill);
                DrawBadgeText(canvas, text, mx, my, h * 0.62f, h * 0.8f, ink);
                break;
            }
        }

        /* Positional: A south = cross, B east = circle, X west = square, Y north = triangle */
        private void DrawPlayStationSymbol(SKCanvas canvas, GHGamepadButton button, float mx, float my, int h, SKColor ink)
        {
            _stroke.Color = ink;
            _stroke.StrokeWidth = Math.Max(1.25f, 0.085f * h);
            switch (button)
            {
            case GHGamepadButton.A:
            {
                float e = 0.19f * h;
                canvas.DrawLine(mx - e, my - e, mx + e, my + e, _stroke);
                canvas.DrawLine(mx - e, my + e, mx + e, my - e, _stroke);
                break;
            }
            case GHGamepadButton.B:
                canvas.DrawCircle(mx, my, 0.21f * h, _stroke);
                break;
            case GHGamepadButton.X:
            {
                float e = 0.18f * h;
                canvas.DrawRect(new SKRect(mx - e, my - e, mx + e, my + e), _stroke);
                break;
            }
            default:
                DrawPathAt(canvas, GetPath(PathSlot.PlayStationTriangle, h), mx, my, _stroke);
                break;
            }
        }

        private void DrawRoundedSquare(SKCanvas canvas, float left, float top, int h)
        {
            float r = 0.24f * h;
            canvas.DrawRoundRect(new SKRect(left, top, left + h, top + h), r, r, _fill);
        }

        private void DrawMenuLines(SKCanvas canvas, float left, float top, int h, SKColor ink)
        {
            int lineWidth = CenteredSpan(h, 0.52f, 3);
            int thickness = Math.Max(1, RoundInt(0.1f * h));
            int step = Math.Max(thickness + 1, RoundInt(0.2f * h));
            int y0 = (h - 2 * step - thickness) / 2;
            float x = left + (h - lineWidth) / 2;
            _fill.Color = ink;
            int k;
            for (k = 0; k < 3; k++)
                canvas.DrawRect(x, top + y0 + k * step, lineWidth, thickness, _fill);
        }

        /* Two square outlines, the front one down-right of the back one and hiding the part of it
           that it covers; drawn as non-overlapping rectangles so partial opacity stays even */
        private void DrawViewSquares(SKCanvas canvas, float left, float top, int h, SKColor ink)
        {
            int s = Math.Max(3, RoundInt(0.42f * h));
            int t = Math.Max(1, RoundInt(0.085f * h));
            int o = Math.Max(t + 1, RoundInt(0.16f * h));
            if (o > s - t - 1)
                o = Math.Max(1, s - t - 1);
            float a = left + (h - s - o) / 2;
            float b = top + (h - s - o) / 2;
            float fx = a + o;
            float fy = b + o;
            _fill.Color = ink;

            /* Back square: top, left, then the parts of the right and bottom edges left of and
               above the front square */
            canvas.DrawRect(a, b, s, t, _fill);
            if (s - 2 * t > 0)
                canvas.DrawRect(a, b + t, t, s - 2 * t, _fill);
            int rightVisible = Math.Min(o, s - t) - t;
            if (rightVisible > 0)
                canvas.DrawRect(a + s - t, b + t, t, rightVisible, _fill);
            canvas.DrawRect(a, b + s - t, Math.Min(o, s), t, _fill);

            /* Front square */
            canvas.DrawRect(fx, fy, s, t, _fill);
            canvas.DrawRect(fx, fy + s - t, s, t, _fill);
            if (s - 2 * t > 0)
            {
                canvas.DrawRect(fx, fy + t, t, s - 2 * t, _fill);
                canvas.DrawRect(fx + s - t, fy + t, t, s - 2 * t, _fill);
            }
        }

        /* The arm of one direction, inset within the plus, in ink */
        private void DrawDPadArm(SKCanvas canvas, float left, float top, int h, int direction, SKColor ink)
        {
            int a = CenteredSpan(h, 0.38f, 2);
            int p = (h - a) / 2;
            int q = p + a;
            int i = Math.Max(1, RoundInt(0.09f * h));
            int across = a - 2 * i;
            int along = p - i;
            if (across <= 0 || along <= 0)
                return;
            _fill.Color = ink;
            switch (direction)
            {
            case 0:
                canvas.DrawRect(left + p + i, top + i, across, along, _fill);
                break;
            case 1:
                canvas.DrawRect(left + q, top + p + i, along, across, _fill);
                break;
            case 2:
                canvas.DrawRect(left + p + i, top + q, across, along, _fill);
                break;
            default:
                canvas.DrawRect(left + i, top + p + i, along, across, _fill);
                break;
            }
        }

        /* Bold text centered on (cx, cy) by its cap height, shrunk to fit maxWidth */
        private void DrawBadgeText(SKCanvas canvas, string text, float cx, float cy, float baseSize, float maxWidth, SKColor ink)
        {
            if (string.IsNullOrEmpty(text))
                return;
            float size = GetFittedSize(text, baseSize, maxWidth);
            _fontPaint.Typeface = BoldTypeface;
            _fontPaint.TextSize = size;
            _fontPaint.Color = ink;
            SKFontMetrics m = _fontPaint.FontMetrics;
            float capHeight = m.CapHeight > 0f ? m.CapHeight : -m.Ascent * 0.7f;
            _fontPaint.DrawTextOnCanvas(canvas, text, Snap(cx), Snap(cy + capHeight * 0.5f), SKTextAlign.Center);
        }

        private float GetFittedSize(string text, float baseSize, float maxWidth)
        {
            SKTypeface typeface = BoldTypeface;
            int i;
            for (i = 0; i < FitCacheSize; i++)
            {
                string cached = _fitTexts[i];
                if (cached != null && _fitBaseSizes[i] == baseSize && _fitMaxWidths[i] == maxWidth
                    && ReferenceEquals(_fitTypefaces[i], typeface) && string.Equals(cached, text, StringComparison.Ordinal))
                    return _fitSizes[i];
            }
            _fontPaint.Typeface = typeface;
            _fontPaint.TextSize = baseSize;
            float width = _fontPaint.MeasureText(text);
            float size = width > maxWidth && width > 0f ? Math.Max(baseSize * 0.5f, baseSize * maxWidth / width) : baseSize;
            i = _fitNext;
            _fitNext = (_fitNext + 1) % FitCacheSize;
            _fitTexts[i] = text;
            _fitTypefaces[i] = typeface;
            _fitBaseSizes[i] = baseSize;
            _fitMaxWidths[i] = maxWidth;
            _fitSizes[i] = size;
            return size;
        }

        private static void DrawPathAt(SKCanvas canvas, SKPath path, float x, float y, SKPaint paint)
        {
            canvas.Save();
            canvas.Translate(x, y);
            canvas.DrawPath(path, paint);
            canvas.Restore();
        }

        /* Geometry; all in whole pixels of the quantized height h */

        private static int BadgeWidth(GHGamepadBadgeKind kind, int h)
        {
            switch (kind)
            {
            case GHGamepadBadgeKind.Bumper:
                return BumperWidth(h);
            case GHGamepadBadgeKind.Trigger:
            case GHGamepadBadgeKind.Grip:
                return RoundInt(1.35f * h);
            case GHGamepadBadgeKind.StickClick:
                return RoundInt(1.3f * h);
            default:
                return h;
            }
        }

        private static int BumperWidth(int h)
        {
            return Math.Max(4, RoundInt(1.5f * h));
        }

        /* How far the middle of the bumper's top edge rises above its ends */
        private static float BumperBulge(int h)
        {
            return 0.12f * h;
        }

        private static int ShiftArrowWidth(int h)
        {
            return Math.Max(3, RoundInt(0.95f * h));
        }

        private static float HoldMarkStroke(float badgeHeight)
        {
            return Math.Max(HoldMarkMinStroke, HoldMarkStrokeOfBadge * badgeHeight);
        }

        /* The space on each side of the hold mark's path for its stroke: half the stroke rounded up
           to whole pixels */
        private static float HoldMarkStrokePad(float badgeHeight)
        {
            return (float)Math.Ceiling(HoldMarkStroke(badgeHeight)) * 0.5f;
        }

        private static void GetShiftArrowGeometry(int h, out int headHeight, out int stemLeft, out int stemWidth)
        {
            int w = ShiftArrowWidth(h);
            headHeight = Math.Max(2, RoundInt(0.46f * h));
            stemWidth = CenteredSpan(w, 0.56f, 2);
            stemLeft = (w - stemWidth) / 2;
        }

        private static int GripSlant(int h)
        {
            return Math.Max(1, RoundInt(0.28f * h));
        }

        /* A span of about fraction of total with the same parity, so that it centers on whole pixels */
        private static int CenteredSpan(int total, float fraction, int min)
        {
            int span = Math.Max(min, RoundInt(total * fraction));
            if (((total - span) & 1) != 0)
                span++;
            return Math.Min(span, total);
        }

        /* Path cache */

        private SKPath GetPath(PathSlot slot, int h)
        {
            int first = (int)slot * PathWays;
            int victim = first;
            int i;
            for (i = first; i < first + PathWays; i++)
            {
                if (_paths[i] != null && _pathHeights[i] == h)
                {
                    _pathStamps[i] = ++_pathClock;
                    return _paths[i];
                }
                if (_paths[i] == null || (_paths[victim] != null && _pathStamps[i] < _pathStamps[victim]))
                    victim = i;
            }
            SKPath old = _paths[victim];
            _paths[victim] = BuildPath(slot, h);
            _pathHeights[victim] = h;
            _pathStamps[victim] = ++_pathClock;
            DisposeObject(old);
            return _paths[victim];
        }

        private SKPath BuildPath(PathSlot slot, int h)
        {
#if GNH_MAUI && !SKIASHARP_3119
            using (SKPathBuilder sink = new SKPathBuilder())
            {
                AddShape(sink, slot, h);
                return sink.Snapshot();
            }
#else
            SKPath sink = new SKPath();
            AddShape(sink, slot, h);
            return sink;
#endif
        }

        private void AddShape(PathSink sink, PathSlot slot, int h)
        {
            _vertexCount = 0;
            switch (slot)
            {
            case PathSlot.ShiftArrow:
            {
                /* A Shift-key up arrow: a full-width head over a narrower stem */
                int w = ShiftArrowWidth(h);
                int headHeight, stemLeft, stemWidth;
                GetShiftArrowGeometry(h, out headHeight, out stemLeft, out stemWidth);
                int stemRight = stemLeft + stemWidth;
                float apex = 0.1f * h;
                float barb = 0.05f * h;
                float foot = 0.06f * h;
                AddVertex(w * 0.5f, 0f, apex, apex);
                AddVertex(w, headHeight, barb, barb);
                AddVertex(stemRight, headHeight, 0f, 0f);
                AddVertex(stemRight, h, foot, foot);
                AddVertex(stemLeft, h, foot, foot);
                AddVertex(stemLeft, headHeight, 0f, 0f);
                AddVertex(0f, headHeight, barb, barb);
                break;
            }
            case PathSlot.Bumper:
                AddBumper(sink, h);
                return;
            case PathSlot.Trigger:
            {
                /* Seen from the front: strongly rounded top corners, slightly rounded bottom ones */
                int w = RoundInt(1.35f * h);
                float bottom = 0.12f * h;
                AddVertex(0f, h, bottom, bottom);
                AddVertex(0f, 0f, 0.5f * h, 0.42f * w);
                AddVertex(w, 0f, 0.42f * w, 0.5f * h);
                AddVertex(w, h, bottom, bottom);
                break;
            }
            case PathSlot.StickClick:
            {
                /* A stick cap: a shaft tab on top, a body with rounded upper corners and a blunt
                   downward point */
                int w = RoundInt(1.3f * h);
                int tabWidth = CenteredSpan(w, 0.3f, 2);
                int tabLeft = (w - tabWidth) / 2;
                int tabRight = tabLeft + tabWidth;
                int bodyTop = Math.Max(2, RoundInt(0.15f * h));
                int sideBottom = RoundInt(0.62f * h);
                int tipHalf = Math.Max(1, RoundInt(0.07f * w));
                float small = Math.Max(0.5f, 0.05f * h);
                float shoulder = 0.22f * h;
                AddVertex(tabLeft, 0f, small, small);
                AddVertex(tabRight, 0f, small, small);
                AddVertex(tabRight, bodyTop, 0f, 0f);
                AddVertex(w, bodyTop, shoulder, shoulder);
                AddVertex(w, sideBottom, small, small);
                AddVertex(w * 0.5f + tipHalf, h, small, small);
                AddVertex(w * 0.5f - tipHalf, h, small, small);
                AddVertex(0f, sideBottom, small, small);
                AddVertex(0f, bodyTop, shoulder, shoulder);
                AddVertex(tabLeft, bodyTop, 0f, 0f);
                break;
            }
            case PathSlot.GripLeft:
            case PathSlot.GripRight:
            {
                /* A paddle: a rounded rectangle whose outer side slants */
                int w = RoundInt(1.35f * h);
                int s = GripSlant(h);
                float r = 0.2f * h;
                if (slot == PathSlot.GripLeft)
                {
                    AddVertex(s, 0f, r, r);
                    AddVertex(w, 0f, r, r);
                }
                else
                {
                    AddVertex(0f, 0f, r, r);
                    AddVertex(w - s, 0f, r, r);
                }
                AddVertex(w, h, r, r);
                AddVertex(0f, h, r, r);
                break;
            }
            case PathSlot.DPad:
            {
                int a = CenteredSpan(h, 0.38f, 2);
                int p = (h - a) / 2;
                int q = p + a;
                float r = Math.Max(0.5f, 0.08f * h);
                AddVertex(p, 0f, r, r);
                AddVertex(q, 0f, r, r);
                AddVertex(q, p, 0f, 0f);
                AddVertex(h, p, r, r);
                AddVertex(h, q, r, r);
                AddVertex(q, q, 0f, 0f);
                AddVertex(q, h, r, r);
                AddVertex(p, h, r, r);
                AddVertex(p, q, 0f, 0f);
                AddVertex(0f, q, r, r);
                AddVertex(0f, p, r, r);
                AddVertex(p, p, 0f, 0f);
                break;
            }
            case PathSlot.PlayStationTriangle:
                AddVertex(0f, -0.25f * h, 0f, 0f);
                AddVertex(0.24f * h, 0.16f * h, 0f, 0f);
                AddVertex(-0.24f * h, 0.16f * h, 0f, 0f);
                break;
            default:
                AddVertex(0f, -0.47f * h, 0f, 0f);
                AddVertex(0.15f * h, -0.26f * h, 0f, 0f);
                AddVertex(-0.15f * h, -0.26f * h, 0f, 0f);
                break;
            }
            AddRoundedPolygon(sink);
        }

        /* A shoulder bumper seen from the front: straight sides, a top edge that bulges upward in a
           gentle arc and meets the sides in large rounded corners, and a straight bottom edge with
           small rounded corners */
        private static void AddBumper(PathSink sink, int h)
        {
            float w = BumperWidth(h);
            float ends = BumperBulge(h);                /* y of the top arc's ends; its middle is at 0 */
            float top = 0.3f * h;                       /* the top corners' rounding along the sides */
            float bottom = Math.Max(0.5f, 0.1f * h);
            float arcLeft = Math.Min(top, w * 0.25f);   /* x of the top arc's left end */
            float arcRight = w - arcLeft;

            /* The arc is a quadratic curve through (arcLeft, ends), (w / 2, 0) and (arcRight, ends),
               so its control point is at (w / 2, -ends). The corners round toward where the sides
               meet the arc's end tangents. */
            float slope = 2f * ends / (w * 0.5f - arcLeft);
            float cornerY = ends + arcLeft * slope;
            float sideTop = Math.Min(h - bottom - 0.5f, Math.Max(cornerY + 0.5f, ends + top));

            sink.MoveTo(0f, sideTop);
            AddCorner(sink, 0f, sideTop, 0f, cornerY, arcLeft, ends);
            sink.CubicTo(arcLeft + (w * 0.5f - arcLeft) * (2f / 3f), ends + (-ends - ends) * (2f / 3f),
                arcRight + (w * 0.5f - arcRight) * (2f / 3f), ends + (-ends - ends) * (2f / 3f), arcRight, ends);
            AddCorner(sink, arcRight, ends, w, cornerY, w, sideTop);
            sink.LineTo(w, h - bottom);
            AddCorner(sink, w, h - bottom, w, h, w - bottom, h);
            sink.LineTo(bottom, h);
            AddCorner(sink, bottom, h, 0f, h, 0f, h - bottom);
            sink.Close();
        }

        /* A rounded corner from (ax, ay) to (bx, by) whose sharp vertex would be at (vx, vy) */
        private static void AddCorner(PathSink sink, float ax, float ay, float vx, float vy, float bx, float by)
        {
            sink.CubicTo(ax + (vx - ax) * CurveKappa, ay + (vy - ay) * CurveKappa,
                bx + (vx - bx) * CurveKappa, by + (vy - by) * CurveKappa, bx, by);
        }

        /* rIn and rOut: how far along the incoming and outgoing edges the corner's rounding starts
           and ends; 0 for a sharp corner */
        private void AddVertex(float x, float y, float rIn, float rOut)
        {
            if (_vertexCount >= MaxPolygonVertices)
                return;
            _vx[_vertexCount] = x;
            _vy[_vertexCount] = y;
            _vrIn[_vertexCount] = rIn;
            _vrOut[_vertexCount] = rOut;
            _vertexCount++;
        }

        /* The scratch polygon as one closed contour, each corner rounded with a cubic curve and
           each rounding limited to half of its edges */
        private void AddRoundedPolygon(PathSink sink)
        {
            int n = _vertexCount;
            if (n < 3)
                return;
            int i;
            for (i = 0; i < n; i++)
            {
                int prev = (i + n - 1) % n;
                int next = (i + 1) % n;
                float vx = _vx[i];
                float vy = _vy[i];
                float inX = vx - _vx[prev];
                float inY = vy - _vy[prev];
                float outX = _vx[next] - vx;
                float outY = _vy[next] - vy;
                float inLength = (float)Math.Sqrt(inX * inX + inY * inY);
                float outLength = (float)Math.Sqrt(outX * outX + outY * outY);
                float rIn = inLength > 0f ? Math.Min(_vrIn[i], inLength * 0.5f) : 0f;
                float rOut = outLength > 0f ? Math.Min(_vrOut[i], outLength * 0.5f) : 0f;
                float ax = rIn > 0f ? vx - inX / inLength * rIn : vx;
                float ay = rIn > 0f ? vy - inY / inLength * rIn : vy;
                float bx = rOut > 0f ? vx + outX / outLength * rOut : vx;
                float by = rOut > 0f ? vy + outY / outLength * rOut : vy;
                if (i == 0)
                    sink.MoveTo(ax, ay);
                else
                    sink.LineTo(ax, ay);
                if (rIn > 0f || rOut > 0f)
                {
                    sink.CubicTo(ax + (vx - ax) * CurveKappa, ay + (vy - ay) * CurveKappa,
                        bx + (vx - bx) * CurveKappa, by + (vy - by) * CurveKappa, bx, by);
                }
            }
            sink.Close();
        }

        /* Helpers */

        /* 0 up, 1 right, 2 down, 3 left, -1 for buttons that are not a direction */
        private static int GetDirection(GHGamepadButton button)
        {
            switch (button)
            {
            case GHGamepadButton.DPadUp:
            case GHGamepadButton.LeftStickUp:
            case GHGamepadButton.RightStickUp:
                return 0;
            case GHGamepadButton.DPadRight:
            case GHGamepadButton.LeftStickRight:
            case GHGamepadButton.RightStickRight:
                return 1;
            case GHGamepadButton.DPadDown:
            case GHGamepadButton.LeftStickDown:
            case GHGamepadButton.RightStickDown:
                return 2;
            case GHGamepadButton.DPadLeft:
            case GHGamepadButton.LeftStickLeft:
            case GHGamepadButton.RightStickLeft:
                return 3;
            default:
                return -1;
            }
        }

        private static int QuantizeHeight(float height)
        {
            if (!(height > 4f))
                return 4;
            if (height > 4096f)
                return 4096;
            return RoundInt(height);
        }

        private static int RoundInt(float v)
        {
            return (int)Math.Round(v);
        }

        private static SKColor Fade(SKColor color, float opacity)
        {
            if (opacity >= 1f)
                return color;
            return color.WithAlpha(ToByte(opacity * color.Alpha / 255f));
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

        private static void DisposeObject(IDisposable obj)
        {
            if (obj != null)
                obj.Dispose();
        }

        public void Dispose()
        {
            if (_disposed)
                return;
            _disposed = true;
            int i;
            for (i = 0; i < _paths.Length; i++)
            {
                DisposeObject(_paths[i]);
                _paths[i] = null;
            }
            for (i = 0; i < FitCacheSize; i++)
            {
                _fitTexts[i] = null;
                _fitTypefaces[i] = null;
            }
            _fontPaint.Dispose();
            _fill.Dispose();
            _stroke.Dispose();
        }
    }
}
