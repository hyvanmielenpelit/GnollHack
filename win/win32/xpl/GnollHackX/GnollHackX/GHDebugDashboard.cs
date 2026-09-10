using SkiaSharp;
using System;
using System.Collections.Generic;
using System.Threading;
#if GNH_MAUI
using GnollHackM;
#endif

namespace GnollHackX
{
    /// <summary>
    /// One whole-frame snapshot of everything the on-screen debug
    /// dashboard displays. Two producers on two different threads fill
    /// disjoint field groups of a staging copy; a consumer reads the
    /// struct back as a unit, so no reader ever sees a half-written row.
    /// </summary>
    public struct GHDebugDashboardData
    {
        /* Frame timing, copied from FrameTimeStatistics */
        public float FPS;
        public float InterFrameAvgMs;
        public float InterFrameStdDevMs;
        public float InterFrameP95Ms;
        public float InterFrameP99Ms;
        public float InterFrameMaxMs;
        public float DroppedFramePct;
        public float LockFailPct;
        public int SampleCount;
        public bool ProfilerEnabled;

        /* Memory */
        public float AllocationRateMBPerSec;
        public long HeapSizeBytes;
        public long Gen0SizeBytes;
        public long Gen1SizeBytes;
        public long Gen2SizeBytes;
        public long LohSizeBytes;

        /* GC */
        public int RuntimeGcFrameCount;
        public int GcGen0Count;
        public int GcGen1Count;
        public int GcGen2Count;
        public float RuntimeGcAvgMs;
        public float RuntimeGcP99Ms;
        public float RuntimeGcWorstMs;
        public int GcFrameCount;
        public int PauseFrameCount;
        public float GcAvgMs;
        public float GcWorstMs;

        /* Draw and caches, published from the paint thread */
        public int DrawCommandCount;
        public int SheetSwitchCount;
        public int SavedRectCount;
        public int SavedAutoDrawBitmapCount;
        public int CompositeFilterFallbackCount;
        public int MapBlobCount;
        public int MapBlobChars;
        public int MenuBlobCount;
        public int MenuBlobChars;
        public long MapBlobHits;
        public long MapBlobMisses;
        public long MapBlobFlushes;
        public long MenuBlobHits;
        public long MenuBlobMisses;
        public long MenuBlobFlushes;
        public int DashBlobCount;
        public long DashBlobHits;
        public long DashBlobMisses;
        public long DashBlobFlushes;
    }

    /// <summary>
    /// Collapsible on-screen performance panel. One instance per game
    /// page; the counters themselves are published into shared static
    /// staging because the two producers -- the main thread's frame
    /// bookkeeping and the paint thread's draw bookkeeping -- do not
    /// otherwise meet.
    ///
    /// The split between TryRefresh and Draw is the whole point of the
    /// class: TryRefresh is the only place a string is built or a row
    /// list is touched, and it runs only when something actually
    /// changed, while Draw allocates nothing at all.
    /// </summary>
    public sealed class GHDebugDashboard
    {
        /* Guards _staging against its two producer threads and the
           paint thread that copies it out */
        private static readonly object _publishLock = new object();
        private static GHDebugDashboardData _staging;
        private static long _version;

        private const string Ellipsis = "...";
        private const string EmptyValue = "";

        /* Monospace advance probe. One character is enough because the
           dashboard font is fixed-pitch, so a row's width is its
           character count times this advance. */
        private const string AdvanceProbe = "0";
        private const byte ChevronAlpha = 200;

        private enum RowKind
        {
            Value,
            SectionHeading,
            PanelHeader,
            LogLine
        }

        private struct DashboardRow
        {
            public string Label;
            public string Value;
            public SKColor ValueColor;
            public RowKind Kind;
        }

        /* Rebuilt in place by TryRefresh and never reallocated */
        private readonly List<DashboardRow> _rows = new List<DashboardRow>(48);
        /* Built centred on the origin and positioned by a canvas translation,
           so the geometry is rebuilt only when the font scale changes */
        private SKPath _chevronDownPath;
        private SKPath _chevronRightPath;
        private float _chevronPathSize = -1f;

        private GHDebugDashboardData _data;

        /* Requested collapse state, written by the touch handler on the
           main thread and read by TryRefresh on the paint thread */
        private volatile bool _panelCollapsed;
        private volatile bool _logCollapsed;

        /* What _rows was last built from */
        private long _builtVersion = -1;
        private bool _builtPanelCollapsed;
        private bool _builtLogCollapsed;
        private bool _builtLogSectionVisible;
        private int _builtLogCount = -1;
        private string _builtLogTail;
        private int _builtMaxLogLines = -1;

        /* Widest row content, in characters */
        private int _maxLabelChars;
        private int _maxValueChars;
        private int _maxLogChars;
        private int _logTimeChars;

        /* Row index of each heading that fit-shrinking may cut at, or -1 */
        private int _gcHeadingRow = -1;
        private int _drawHeadingRow = -1;
        private int _logHeadingRow = -1;

        private float _charAdvance;
        private float _charAdvanceSize = -1f;

        private SKRect _panelToggleRect;
        private SKRect _logToggleRect;
        private SKRect _lastDrawnRect;

        public SKRect PanelToggleRect { get { return _panelToggleRect; } }
        public SKRect LogToggleRect { get { return _logToggleRect; } }
        public SKRect LastDrawnRect { get { return _lastDrawnRect; } }

        /// <summary>
        /// Publishes the frame timing, memory and GC groups. Called from
        /// the main thread once per statistics refresh.
        /// </summary>
        public static void PublishFrameStats(in FrameTimeStatistics stats, bool profilerEnabled)
        {
            lock (_publishLock)
            {
                _staging.FPS = stats.FPS;
                _staging.InterFrameAvgMs = stats.InterFrameAvgMs;
                _staging.InterFrameStdDevMs = stats.InterFrameStdDevMs;
                _staging.InterFrameP95Ms = stats.InterFrameP95Ms;
                _staging.InterFrameP99Ms = stats.InterFrameP99Ms;
                _staging.InterFrameMaxMs = stats.InterFrameMaxMs;
                _staging.DroppedFramePct = stats.DroppedFramePct;
                _staging.LockFailPct = stats.LockFailPct;
                _staging.SampleCount = stats.SampleCount;
                _staging.ProfilerEnabled = profilerEnabled;

                _staging.AllocationRateMBPerSec = stats.AllocationRateMBPerSec;
                _staging.HeapSizeBytes = stats.HeapSizeBytes;
                _staging.Gen0SizeBytes = stats.Gen0SizeBytes;
                _staging.Gen1SizeBytes = stats.Gen1SizeBytes;
                _staging.Gen2SizeBytes = stats.Gen2SizeBytes;
                _staging.LohSizeBytes = stats.LohSizeBytes;

                _staging.RuntimeGcFrameCount = stats.RuntimeGcFrameCount;
                _staging.RuntimeGcAvgMs = stats.RuntimeGcAvgMs;
                _staging.RuntimeGcP99Ms = stats.RuntimeGcP99Ms;
                _staging.RuntimeGcWorstMs = stats.RuntimeGcWorstMs;
                _staging.GcGen0Count = stats.GcGen0Count;
                _staging.GcGen1Count = stats.GcGen1Count;
                _staging.GcGen2Count = stats.GcGen2Count;
                _staging.GcFrameCount = stats.GcFrameCount;
                _staging.GcAvgMs = stats.GcAvgMs;
                _staging.GcWorstMs = stats.GcWorstMs;
                _staging.PauseFrameCount = stats.PauseFrameCount;
            }

            Interlocked.Increment(ref _version);
        }

        /// <summary>
        /// Publishes the draw and cache group. Called from the paint
        /// thread, which owns the two font paints and is therefore the
        /// only thread allowed to read their cache counters.
        /// </summary>
        public static void PublishDrawStats(int drawCommandCount, int sheetSwitchCount,
            int savedRectCount, int savedAutoDrawBitmapCount, int compositeFilterFallbackCount,
            GHSkiaFontPaint mapTextPaint, GHSkiaFontPaint menuTextPaint,
            GHSkiaFontPaint dashTextPaint)
        {
            lock (_publishLock)
            {
                _staging.DrawCommandCount = drawCommandCount;
                _staging.SheetSwitchCount = sheetSwitchCount;
                _staging.SavedRectCount = savedRectCount;
                _staging.SavedAutoDrawBitmapCount = savedAutoDrawBitmapCount;
                _staging.CompositeFilterFallbackCount = compositeFilterFallbackCount;

                if (mapTextPaint != null)
                {
                    _staging.MapBlobCount = mapTextPaint.BlobCacheCount;
                    _staging.MapBlobChars = mapTextPaint.BlobCacheChars;
                    _staging.MapBlobHits = mapTextPaint.BlobCacheHits;
                    _staging.MapBlobMisses = mapTextPaint.BlobCacheMisses;
                    _staging.MapBlobFlushes = mapTextPaint.BlobCacheFlushes;
                }
                else
                {
                    _staging.MapBlobCount = 0;
                    _staging.MapBlobChars = 0;
                    _staging.MapBlobHits = 0;
                    _staging.MapBlobMisses = 0;
                    _staging.MapBlobFlushes = 0;
                }

                if (menuTextPaint != null)
                {
                    _staging.MenuBlobCount = menuTextPaint.BlobCacheCount;
                    _staging.MenuBlobChars = menuTextPaint.BlobCacheChars;
                    _staging.MenuBlobHits = menuTextPaint.BlobCacheHits;
                    _staging.MenuBlobMisses = menuTextPaint.BlobCacheMisses;
                    _staging.MenuBlobFlushes = menuTextPaint.BlobCacheFlushes;
                }
                else
                {
                    _staging.MenuBlobCount = 0;
                    _staging.MenuBlobChars = 0;
                    _staging.MenuBlobHits = 0;
                    _staging.MenuBlobMisses = 0;
                    _staging.MenuBlobFlushes = 0;
                }

                if (dashTextPaint != null)
                {
                    _staging.DashBlobCount = dashTextPaint.BlobCacheCount;
                    _staging.DashBlobHits = dashTextPaint.BlobCacheHits;
                    _staging.DashBlobMisses = dashTextPaint.BlobCacheMisses;
                    _staging.DashBlobFlushes = dashTextPaint.BlobCacheFlushes;
                }
                else
                {
                    _staging.DashBlobCount = 0;
                    _staging.DashBlobHits = 0;
                    _staging.DashBlobMisses = 0;
                    _staging.DashBlobFlushes = 0;
                }
            }

            Interlocked.Increment(ref _version);
        }

        /// <summary>
        /// Sets which sections are folded away. Called from the touch
        /// handler on the main thread; the next TryRefresh picks it up.
        /// </summary>
        public void SetCollapsed(bool panelCollapsed, bool logCollapsed)
        {
            _panelCollapsed = panelCollapsed;
            _logCollapsed = logCollapsed;
        }

        /// <summary>
        /// Rebuilds the row list if anything it depends on has changed,
        /// and returns whether it did. Paint thread only. This is the
        /// only member that allocates.
        /// </summary>
        public bool TryRefresh(bool logSectionVisible, IReadOnlyList<GHScreenLogEntry> logLines, int maxLogLines)
        {
            long version = Interlocked.Read(ref _version);
            bool panelCollapsed = _panelCollapsed;
            bool logCollapsed = _logCollapsed;

            int logCount = logLines == null ? 0 : logLines.Count;
            /* The log buffer is append-only between swaps and a swap
               changes Count, so the count plus the identity of the last
               entry detects every possible change exactly. */
            string logTail = logCount > 0 ? logLines[logCount - 1].Text : null;

            if (_builtVersion == version
                && _builtPanelCollapsed == panelCollapsed
                && _builtLogCollapsed == logCollapsed
                && _builtLogSectionVisible == logSectionVisible
                && _builtLogCount == logCount
                && _builtMaxLogLines == maxLogLines
                && ReferenceEquals(_builtLogTail, logTail))
            {
                return false;
            }

            _builtVersion = version;
            _builtPanelCollapsed = panelCollapsed;
            _builtLogCollapsed = logCollapsed;
            _builtLogSectionVisible = logSectionVisible;
            _builtLogCount = logCount;
            _builtMaxLogLines = maxLogLines;
            _builtLogTail = logTail;

            lock (_publishLock)
            {
                _data = _staging;
            }

            BuildRows(panelCollapsed, logCollapsed, logSectionVisible, logLines, logCount, maxLogLines);
            return true;
        }

        private void BuildRows(bool panelCollapsed, bool logCollapsed, bool logSectionVisible,
            IReadOnlyList<GHScreenLogEntry> logLines, int logCount, int maxLogLines)
        {
            _rows.Clear();
            _maxLabelChars = 0;
            _maxValueChars = 0;
            _maxLogChars = 0;
            _logTimeChars = 0;
            _gcHeadingRow = -1;
            _drawHeadingRow = -1;
            _logHeadingRow = -1;

            AddRow("DEBUG", EmptyValue, SKColors.White, RowKind.PanelHeader);
            if (panelCollapsed)
                return;

            GHDebugDashboardData d = _data;

            if (!d.ProfilerEnabled)
            {
                AddRow("profiler off", EmptyValue, SKColors.White, RowKind.Value);
            }
            else if (d.SampleCount == 0)
            {
                AddRow("no data", EmptyValue, SKColors.White, RowKind.Value);
            }
            else
            {
                AddRow("FRAME", FormattableString.Invariant($"{d.FPS:0} fps"),
                    SKColors.White, RowKind.SectionHeading);

                AddRow("avg", FormattableString.Invariant(
                        $"{d.InterFrameAvgMs:0.0} ms   sd {d.InterFrameStdDevMs:0.0}"),
                    SKColors.White, RowKind.Value);

                SKColor p99Color = SKColors.White;
                if (d.InterFrameP99Ms > GHConstants.DebugDashboardP99AlarmMs)
                    p99Color = SKColors.Red;
                else if (d.InterFrameP99Ms > GHConstants.DebugDashboardP99WarnMs)
                    p99Color = SKColors.Orange;

                AddRow("p95", FormattableString.Invariant(
                        $"{d.InterFrameP95Ms:0.0}  p99 {d.InterFrameP99Ms:0.0}  max {d.InterFrameMaxMs:0.0}"),
                    p99Color, RowKind.Value);

                SKColor dropColor = SKColors.White;
                if (d.DroppedFramePct > GHConstants.DebugDashboardDroppedFrameAlarmPct)
                    dropColor = SKColors.Red;
                else if (d.DroppedFramePct > GHConstants.DebugDashboardDroppedFrameWarnPct
                    || d.LockFailPct > GHConstants.DebugDashboardLockFailWarnPct)
                    dropColor = SKColors.Orange;

                AddRow("drop", FormattableString.Invariant(
                        $"{d.DroppedFramePct:0.0} %  lock {d.LockFailPct:0.0} %  n {d.SampleCount}"),
                    dropColor, RowKind.Value);

                AddRow("MEMORY", EmptyValue, SKColors.White, RowKind.SectionHeading);

                float heapMB = d.HeapSizeBytes / (1024f * 1024f);
                AddRow("alloc", FormattableString.Invariant(
                        $"{d.AllocationRateMBPerSec:0.0} MB/s   heap {heapMB:0} MB"),
                    SKColors.White, RowKind.Value);

                AddRow("gen", BuildGenerationSizes(d), SKColors.White, RowKind.Value);

                bool hasRuntimeGc = d.RuntimeGcFrameCount > 0
                    || d.GcGen0Count > 0 || d.GcGen1Count > 0 || d.GcGen2Count > 0;
                bool hasForcedGc = d.GcFrameCount > 0 || d.PauseFrameCount > 0;
                if (hasRuntimeGc || hasForcedGc)
                {
                    _gcHeadingRow = _rows.Count;
                    AddRow("GC", EmptyValue, SKColors.White, RowKind.SectionHeading);

                    if (d.RuntimeGcFrameCount > 0)
                    {
                        AddRow("rt", FormattableString.Invariant(
                                $"{d.RuntimeGcFrameCount}x avg {d.RuntimeGcAvgMs:0.0} p99 {d.RuntimeGcP99Ms:0.0} max {d.RuntimeGcWorstMs:0.0}"),
                            SKColors.White, RowKind.Value);
                    }

                    if (d.GcGen0Count > 0 || d.GcGen1Count > 0 || d.GcGen2Count > 0)
                    {
                        AddRow("gen", FormattableString.Invariant(
                                $"{d.GcGen0Count} / {d.GcGen1Count} / {d.GcGen2Count}"),
                            SKColors.White, RowKind.Value);
                    }

                    if (hasForcedGc)
                    {
                        AddRow("forc", FormattableString.Invariant(
                                $"{d.GcFrameCount}x avg {d.GcAvgMs:0.0} max {d.GcWorstMs:0.0}  pause {d.PauseFrameCount}"),
                            SKColors.White, RowKind.Value);
                    }
                }
            }

            _drawHeadingRow = _rows.Count;
            AddRow("DRAW", EmptyValue, SKColors.White, RowKind.SectionHeading);

            AddRow("cmds", FormattableString.Invariant(
                    $"{d.DrawCommandCount}   sheets {d.SheetSwitchCount}"),
                SKColors.White, RowKind.Value);

            AddRow("map", BuildBlobSummary(d.MapBlobCount, d.MapBlobChars, d.MapBlobHits, d.MapBlobMisses, d.MapBlobFlushes),
                SKColors.White, RowKind.Value);

            AddRow("menu", BuildBlobSummary(d.MenuBlobCount, d.MenuBlobChars, d.MenuBlobHits, d.MenuBlobMisses, d.MenuBlobFlushes),
                SKColors.White, RowKind.Value);

            /* The panel's own cache. A flush here is expected and harmless; one on the
               map row is not, which is why the two are separate paints. */
            AddRow("dash", BuildDashSummary(d.DashBlobCount, d.DashBlobHits, d.DashBlobMisses, d.DashBlobFlushes),
                SKColors.White, RowKind.Value);

            AddRow("rect", FormattableString.Invariant(
                    $"{d.SavedRectCount}  auto {d.SavedAutoDrawBitmapCount}  filt {d.CompositeFilterFallbackCount}"),
                SKColors.White, RowKind.Value);

            if (logSectionVisible)
            {
                _logHeadingRow = _rows.Count;
                AddRow("LOG", EmptyValue, SKColors.White, RowKind.SectionHeading);

                if (!logCollapsed && logLines != null && maxLogLines > 0)
                {
                    int start = Math.Max(0, logCount - maxLogLines);
                    for (int i = start; i < logCount; i++)
                    {
                        GHScreenLogEntry entry = logLines[i];
                        AddRow(FormattableString.Invariant($"{entry.Time:HH:mm:ss.fff}"), entry.Text,
                            SKColors.LightSalmon, RowKind.LogLine);
                    }
                }
            }

            /* Room for the elision marker a dropped section's heading shows */
            if (_gcHeadingRow >= 0 || _drawHeadingRow >= 0 || _logHeadingRow >= 0)
                _maxValueChars = Math.Max(_maxValueChars, Ellipsis.Length);
        }

        /// <summary>
        /// Per-generation heap sizes in megabytes, or "?" when the
        /// runtime does not report them.
        /// </summary>
        private static string BuildGenerationSizes(in GHDebugDashboardData d)
        {
            if (d.Gen0SizeBytes <= 0 && d.Gen1SizeBytes <= 0 && d.Gen2SizeBytes <= 0)
                return "?";

            float g0 = d.Gen0SizeBytes / (1024f * 1024f);
            float g1 = d.Gen1SizeBytes / (1024f * 1024f);
            float g2 = d.Gen2SizeBytes / (1024f * 1024f);
            if (d.LohSizeBytes > 0)
            {
                float loh = d.LohSizeBytes / (1024f * 1024f);
                return FormattableString.Invariant($"{g0:0} / {g1:0} / {g2:0} + {loh:0} MB");
            }
            return FormattableString.Invariant($"{g0:0} / {g1:0} / {g2:0} MB");
        }

        /* Characters are reported in rounded kibicharacters and the hit rate as a whole
           percentage: both are bounded, so the same string recurs and stays cached,
           where a raw running total would be a fresh entry every refresh. */
        private static string BuildBlobSummary(int count, int chars, long hits, long misses, long flushes)
        {
            int kc = (chars + 512) / 1024;
            long total = hits + misses;
            if (total <= 0)
                return FormattableString.Invariant($"{count} blob / {kc}kc   - % hit   {flushes} flush");

            float hitPct = (float)hits / total * 100f;
            return FormattableString.Invariant($"{count} blob / {kc}kc   {hitPct:0} % hit   {flushes} flush");
        }

        private static string BuildDashSummary(int count, long hits, long misses, long flushes)
        {
            long total = hits + misses;
            if (total <= 0)
                return FormattableString.Invariant($"{count} blob   - % hit   {flushes} flush");

            float hitPct = (float)hits / total * 100f;
            return FormattableString.Invariant($"{count} blob   {hitPct:0} % hit   {flushes} flush");
        }

        private void AddRow(string label, string value, SKColor valueColor, RowKind kind)
        {
            DashboardRow row = new DashboardRow();
            row.Label = label ?? EmptyValue;
            row.Value = value ?? EmptyValue;
            row.ValueColor = valueColor;
            row.Kind = kind;
            _rows.Add(row);

            if (kind == RowKind.LogLine)
            {
                /* Log lines span the whole panel rather than the label
                   column, so they must not push the value column right.
                   The time is fixed width, so one gap separates the two. */
                if (row.Label.Length > _logTimeChars)
                    _logTimeChars = row.Label.Length;
                int combined = row.Label.Length + 1 + row.Value.Length;
                if (combined > _maxLogChars)
                    _maxLogChars = combined;
            }
            else
            {
                if (row.Label.Length > _maxLabelChars)
                    _maxLabelChars = row.Label.Length;
                if (row.Value.Length > _maxValueChars)
                    _maxValueChars = row.Value.Length;
            }
        }

        /// <summary>
        /// Draws the panel inside the box (left, top)-(maxRight,
        /// maxBottom) and records the toggle hit rectangles. Paint
        /// thread only, and allocation-free: every string it draws was
        /// built by TryRefresh.
        /// </summary>
        public void Draw(SKCanvas canvas, GHSkiaFontPaint textPaint, float left, float top,
            float maxRight, float maxBottom, float scale)
        {
            _panelToggleRect = SKRect.Empty;
            _logToggleRect = SKRect.Empty;
            _lastDrawnRect = SKRect.Empty;

            if (canvas == null || textPaint == null || _rows.Count == 0)
                return;

            SKTypeface oldTypeface = textPaint.Typeface;
            float oldTextSize = textPaint.TextSize;
            SKColor oldColor = textPaint.Color;
            SKPaintStyle oldStyle = textPaint.Style;
            float oldStrokeWidth = textPaint.StrokeWidth;

            float fontSize = GHConstants.DebugDashboardBaseFontSize * scale;
            textPaint.Typeface = GHApp.DejaVuSansMonoTypeface;
            textPaint.TextSize = fontSize;

            try
            {
                float lineHeight = textPaint.FontSpacing;
                float pad = GHConstants.DebugDashboardPadding * scale;
                float minSpan = 2f * lineHeight;
                if (lineHeight <= 0f || maxRight - left < minSpan || maxBottom - top < minSpan)
                    return;

                if (_charAdvanceSize != fontSize)
                {
                    _charAdvance = textPaint.MeasureText(AdvanceProbe);
                    _charAdvanceSize = fontSize;
                }
                if (_charAdvance <= 0f)
                    return;

                int rowsThatFit = (int)Math.Floor((maxBottom - top - 2f * pad) / lineHeight);
                if (rowsThatFit < 1)
                    return;

                /* Fit-shrinking cuts the row list at a section heading,
                   bottom-up, and marks the surviving heading as elided.
                   A suffix cut keeps Draw free of a row rebuild. */
                int rowsToDraw = _rows.Count;
                int elidedRow = -1;
                if (rowsToDraw > rowsThatFit && _logHeadingRow >= 0)
                {
                    rowsToDraw = _logHeadingRow + 1;
                    elidedRow = _logHeadingRow;
                }
                if (rowsToDraw > rowsThatFit && _drawHeadingRow >= 0)
                {
                    rowsToDraw = _drawHeadingRow + 1;
                    elidedRow = _drawHeadingRow;
                }
                if (rowsToDraw > rowsThatFit && _gcHeadingRow >= 0)
                {
                    rowsToDraw = _gcHeadingRow + 1;
                    elidedRow = _gcHeadingRow;
                }
                if (rowsToDraw > rowsThatFit)
                {
                    rowsToDraw = rowsThatFit;
                    elidedRow = -1;
                }

                float chevronSize = GHConstants.DebugDashboardChevronSize * scale;
                float labelWidth = _maxLabelChars * _charAdvance;
                float valueWidth = _maxValueChars * _charAdvance;
                float logWidth = _maxLogChars * _charAdvance;
                float contentWidth = Math.Max(labelWidth + pad + valueWidth, logWidth);
                float panelWidth = pad + contentWidth + pad + chevronSize + pad;
                float panelHeight = 2f * pad + rowsToDraw * lineHeight;

                SKRect rect = new SKRect(left, top,
                    Math.Min(left + panelWidth, maxRight),
                    Math.Min(top + panelHeight, maxBottom));

                float radius = GHConstants.DebugDashboardCornerRadius * scale;
                textPaint.Style = SKPaintStyle.Fill;
                textPaint.Color = SKColors.Black.WithAlpha(GHConstants.DebugDashboardBackgroundAlpha);
                canvas.DrawRoundRect(rect, radius, radius, textPaint.Paint);
                textPaint.Style = SKPaintStyle.Stroke;
                textPaint.StrokeWidth = 1f;
                textPaint.Color = SKColors.White.WithAlpha(GHConstants.DebugDashboardBorderAlpha);
                canvas.DrawRoundRect(rect, radius, radius, textPaint.Paint);

                float labelX = rect.Left + pad;
                float valueX = labelX + labelWidth + pad;

                canvas.Save();
                try
                {
                    /* An over-long row is clipped to the panel rather
                       than bleeding across the map */
                    canvas.ClipRect(rect);

                    float baseline = rect.Top + pad - textPaint.FontMetrics.Ascent;
                    for (int i = 0; i < rowsToDraw; i++)
                    {
                        DashboardRow row = _rows[i];
                        if (row.Kind == RowKind.LogLine)
                        {
                            textPaint.Typeface = GHApp.DejaVuSansMonoTypeface;
                            textPaint.Style = SKPaintStyle.Fill;
                            /* Monospace, so the message column is an offset rather
                               than a measurement */
                            textPaint.Color = SKColors.Gray;
                            textPaint.DrawTextOnCanvas(canvas, row.Label, labelX, baseline);
                            textPaint.Color = SKColors.LightSalmon;
                            textPaint.DrawTextOnCanvas(canvas, row.Value,
                                labelX + (_logTimeChars + 1) * _charAdvance, baseline);
                        }
                        else
                        {
                            bool heading = row.Kind == RowKind.SectionHeading
                                || row.Kind == RowKind.PanelHeader;
                            textPaint.Typeface = heading
                                ? GHApp.DejaVuSansMonoBoldTypeface
                                : GHApp.DejaVuSansMonoTypeface;

                            string value = i == elidedRow ? Ellipsis : row.Value;

                            textPaint.Style = SKPaintStyle.Fill;
                            textPaint.Color = heading ? SKColors.Gold : SKColors.LightGray;
                            textPaint.DrawTextOnCanvas(canvas, row.Label, labelX, baseline);
                            if (value.Length > 0)
                            {
                                textPaint.Color = row.ValueColor;
                                textPaint.DrawTextOnCanvas(canvas, value, valueX, baseline);
                            }
                        }
                        baseline += lineHeight;
                    }
                }
                finally
                {
                    canvas.Restore();
                }

                float minTouch = GHConstants.DebugDashboardMinTouchRows * lineHeight;
                float chevronCenterX = rect.Right - pad - chevronSize * 0.5f;

                textPaint.Style = SKPaintStyle.Fill;
                textPaint.StrokeWidth = 0f;
                textPaint.Color = SKColors.White.WithAlpha(ChevronAlpha);

                float panelChevronY = rect.Top + pad + 0.5f * lineHeight;
                DrawChevron(canvas, textPaint, chevronCenterX, panelChevronY, chevronSize, !_builtPanelCollapsed);
                _panelToggleRect = InflateToTouchSize(chevronCenterX, panelChevronY,
                    chevronSize, minTouch, rect.Left, rect.Right);

                if (!_builtPanelCollapsed && _logHeadingRow >= 0
                    && _logHeadingRow < rowsToDraw && elidedRow != _logHeadingRow)
                {
                    float logChevronY = rect.Top + pad + (_logHeadingRow + 0.5f) * lineHeight;
                    DrawChevron(canvas, textPaint, chevronCenterX, logChevronY, chevronSize, !_builtLogCollapsed);
                    _logToggleRect = InflateToTouchSize(chevronCenterX, logChevronY,
                        chevronSize, minTouch, rect.Left, rect.Right);
                }

                _lastDrawnRect = rect;
            }
            finally
            {
                textPaint.Typeface = oldTypeface;
                textPaint.TextSize = oldTextSize;
                textPaint.Color = oldColor;
                textPaint.Style = oldStyle;
                textPaint.StrokeWidth = oldStrokeWidth;
            }
        }

        /// <summary>
        /// Draws a filled triangle pointing down when the section it
        /// controls is expanded, right when it is collapsed. A path
        /// rather than a glyph, so it cannot go missing with the font.
        /// </summary>
        private void DrawChevron(SKCanvas canvas, GHSkiaFontPaint textPaint,
            float centerX, float centerY, float size, bool pointDown)
        {
            EnsureChevronPaths(size);
            SKPath path = pointDown ? _chevronDownPath : _chevronRightPath;
            if (path == null)
                return;

            canvas.Save();
            try
            {
                canvas.Translate(centerX, centerY);
                canvas.DrawPath(path, textPaint.Paint);
            }
            finally
            {
                canvas.Restore();
            }
        }

        private void EnsureChevronPaths(float size)
        {
            if (_chevronPathSize == size && _chevronDownPath != null && _chevronRightPath != null)
                return;

            if (_chevronDownPath != null)
                _chevronDownPath.Dispose();
            if (_chevronRightPath != null)
                _chevronRightPath.Dispose();

            _chevronDownPath = BuildChevronPath(size, true);
            _chevronRightPath = BuildChevronPath(size, false);
            _chevronPathSize = size;
        }

        private static SKPath BuildChevronPath(float size, bool pointDown)
        {
            float half = size * 0.5f;
#if GNH_MAUI && !SKIASHARP_3119
            using (SKPathBuilder builder = new SKPathBuilder())
            {
                if (pointDown)
                {
                    builder.MoveTo(-half, -half);
                    builder.LineTo(half, -half);
                    builder.LineTo(0f, half);
                }
                else
                {
                    builder.MoveTo(-half, -half);
                    builder.LineTo(half, 0f);
                    builder.LineTo(-half, half);
                }
                builder.Close();
                return builder.Snapshot();
            }
#else
            SKPath path = new SKPath();
            if (pointDown)
            {
                path.MoveTo(-half, -half);
                path.LineTo(half, -half);
                path.LineTo(0f, half);
            }
            else
            {
                path.MoveTo(-half, -half);
                path.LineTo(half, 0f);
                path.LineTo(-half, half);
            }
            path.Close();
            return path;
#endif
        }

        /// <summary>
        /// Grows the chevron's drawn rectangle around its own centre to
        /// a finger-sized square, then slides it back inside the panel
        /// horizontally so a toggle never reaches past the edge.
        /// </summary>
        private static SKRect InflateToTouchSize(float centerX, float centerY, float size,
            float minSize, float clampLeft, float clampRight)
        {
            float halfSide = Math.Max(size, minSize) * 0.5f;
            SKRect rect = new SKRect(centerX - halfSide, centerY - halfSide,
                centerX + halfSide, centerY + halfSide);
            if (rect.Right > clampRight)
                rect.Offset(clampRight - rect.Right, 0f);
            if (rect.Left < clampLeft)
                rect.Offset(clampLeft - rect.Left, 0f);
            return rect;
        }
    }
}
