using System;
using System.Globalization;
using System.Threading;
using SkiaSharp;

namespace GnollHackX.Performance
{
    /* The in-game performance test's countdown, drawn on the map canvas right after
       GHFrameMarker: a small rounded pill at top center, below the frame marker strip,
       reading "Performance test starts in N" until the settle end, "Measuring... N s -
       please wait" until the window end, and "Preparing the report - please wait" after
       it until Stop.

       Start and Stop run on the UI thread; Draw runs on the paint thread (the GL thread
       on Android), so the state is kept with Interlocked. Draw is allocation-free once
       the text blobs exist: every text is precomputed, and its blob is shaped for the
       current text size on the first draw at that size. The blobs, font and paints are
       touched only by Draw.

       Must compile under C# 7.3 (the legacy netstandard2.0 project). */
    public static class GHDiagnosticCountdown
    {
        public const int MaxSettleSeconds = 10;
        public const int MaxWindowSeconds = 60;

        private const float TextSizeDip = 14f;
        private const float PaddingXDip = 10f;
        private const float PaddingYDip = 4f;
        private const byte BackgroundAlpha = 160;

        private const int PhaseSettle = 0;
        private const int PhaseWindow = 1;
        private const int PhaseFinishing = 2;
        private const int PhaseCount = 3;

        private const string FinishingText = "Preparing the report - please wait";

        private static long _settleEndUtcTicks = 0;
        private static long _windowEndUtcTicks = 0;
        private static int _active = 0;

        private static readonly string[] _settleTexts = BuildTexts("Performance test starts in ", "", MaxSettleSeconds);
        private static readonly string[] _windowTexts = BuildTexts("Measuring... ", " s - please wait", MaxWindowSeconds);

        /* Paint thread only */
        private static readonly SKPaint _backgroundPaint = new SKPaint
        {
            Color = SKColors.Black.WithAlpha(BackgroundAlpha),
            Style = SKPaintStyle.Fill,
            IsAntialias = true
        };
        private static readonly SKPaint _textPaint = new SKPaint
        {
            Color = SKColors.White,
            Style = SKPaintStyle.Fill,
            IsAntialias = true
        };
        private static readonly SKFont _font = CreateFont();
#if !GNH_MAUI
        private static readonly SKPaint _measurePaint = new SKPaint { IsAntialias = true };
#endif
        private static readonly SKTextBlob[] _settleBlobs = new SKTextBlob[MaxSettleSeconds + 1];
        private static readonly SKTextBlob[] _windowBlobs = new SKTextBlob[MaxWindowSeconds + 1];
        private static SKTextBlob _finishingBlob = null;
        private static readonly float[] _settleWidths = new float[MaxSettleSeconds + 1];
        private static readonly float[] _windowWidths = new float[MaxWindowSeconds + 1];
        private static float _finishingWidth = 0f;
        private static readonly float[] _phaseMaxWidths = new float[PhaseCount];
        private static float _shapedSize = -1f;
        private static float _ascent = 0f;     /* negative, above the baseline */
        private static float _descent = 0f;

        public static bool IsActive
        {
            get { return Interlocked.CompareExchange(ref _active, 0, 0) != 0; }
        }

        /* Shows the countdown: the settle phase until settleEndUtcTicks, then the window
           phase until windowEndUtcTicks (both DateTime.UtcNow.Ticks). Calling it again
           while active moves the ends. */
        public static void Start(long settleEndUtcTicks, long windowEndUtcTicks)
        {
            Interlocked.Exchange(ref _settleEndUtcTicks, settleEndUtcTicks);
            Interlocked.Exchange(ref _windowEndUtcTicks, windowEndUtcTicks);
            Interlocked.Exchange(ref _active, 1);
        }

        public static void Stop()
        {
            Interlocked.Exchange(ref _active, 0);
        }

        public static void Draw(SKCanvas canvas, float width, float height, float density)
        {
            if (Interlocked.CompareExchange(ref _active, 0, 0) == 0)
                return;
            if (canvas == null || width <= 0 || height <= 0)
                return;
            try
            {
                float d = density > 0 ? density : 1f;
                float size = TextSizeDip * d;
                if (size != _shapedSize)
                    Shape(size);

                long now = DateTime.UtcNow.Ticks;
                long settleEnd = Interlocked.Read(ref _settleEndUtcTicks);
                long windowEnd = Interlocked.Read(ref _windowEndUtcTicks);

                SKTextBlob blob;
                float textWidth;
                int phase;
                if (now < settleEnd)
                {
                    int i = SecondsLeft(settleEnd - now, MaxSettleSeconds);
                    blob = _settleBlobs[i];
                    textWidth = _settleWidths[i];
                    phase = PhaseSettle;
                }
                else if (now < windowEnd)
                {
                    int i = SecondsLeft(windowEnd - now, MaxWindowSeconds);
                    blob = _windowBlobs[i];
                    textWidth = _windowWidths[i];
                    phase = PhaseWindow;
                }
                else
                {
                    blob = _finishingBlob;
                    textWidth = _finishingWidth;
                    phase = PhaseFinishing;
                }
                if (blob == null)
                    return;

                /* The pill keeps the phase's widest size, so it does not jitter as the
                   digits change; the text is centered in it */
                float padX = PaddingXDip * d;
                float padY = PaddingYDip * d;
                float pillWidth = _phaseMaxWidths[phase] + 2f * padX;
                float pillHeight = (_descent - _ascent) + 2f * padY;
                float cell = Math.Max(8f, 10f * d);   /* GHFrameMarker's cell size */
                float top = 3.5f * cell;
                float left = (width - pillWidth) / 2f;
                SKRect rect = new SKRect(left, top, left + pillWidth, top + pillHeight);
                float radius = pillHeight / 2f;
                canvas.DrawRoundRect(rect, radius, radius, _backgroundPaint);
                canvas.DrawText(blob, (width - textWidth) / 2f, top + padY - _ascent, _textPaint);
            }
            catch
            {
                /* The countdown must never break the map paint */
            }
        }

        /* Whole seconds left, rounded up and clamped to 1..max */
        private static int SecondsLeft(long ticksLeft, int max)
        {
            long seconds = (ticksLeft + TimeSpan.TicksPerSecond - 1) / TimeSpan.TicksPerSecond;
            if (seconds < 1)
                seconds = 1;
            if (seconds > max)
                seconds = max;
            return (int)seconds;
        }

        private static string[] BuildTexts(string prefix, string suffix, int max)
        {
            string[] texts = new string[max + 1];
            for (int i = 0; i <= max; i++)
                texts[i] = prefix + i.ToString(CultureInfo.InvariantCulture) + suffix;
            return texts;
        }

        private static SKFont CreateFont()
        {
            SKFont font = new SKFont();
            font.Edging = SKFontEdging.Antialias;
            return font;
        }

        /* Shapes every text at size with the default typeface and measures it; the
           previous blobs are disposed */
        private static void Shape(float size)
        {
            DisposeBlobs();
            _font.Size = size;
#if !GNH_MAUI
            _measurePaint.TextSize = size;
#endif
            SKFontMetrics metrics = _font.Metrics;
            _ascent = metrics.Ascent;
            _descent = metrics.Descent;

            _phaseMaxWidths[PhaseSettle] = ShapeAll(_settleTexts, _settleBlobs, _settleWidths);
            _phaseMaxWidths[PhaseWindow] = ShapeAll(_windowTexts, _windowBlobs, _windowWidths);
            _finishingBlob = SKTextBlob.Create(FinishingText, _font, SKPoint.Empty);
            _finishingWidth = Measure(FinishingText);
            _phaseMaxWidths[PhaseFinishing] = _finishingWidth;
            _shapedSize = size;
        }

        /* Returns the widest text's width */
        private static float ShapeAll(string[] texts, SKTextBlob[] blobs, float[] widths)
        {
            float max = 0f;
            for (int i = 0; i < texts.Length; i++)
            {
                blobs[i] = SKTextBlob.Create(texts[i], _font, SKPoint.Empty);
                widths[i] = Measure(texts[i]);
                if (widths[i] > max)
                    max = widths[i];
            }
            return max;
        }

        private static float Measure(string text)
        {
#if GNH_MAUI
            return _font.MeasureText(text, _textPaint);
#else
            return _measurePaint.MeasureText(text);
#endif
        }

        private static void DisposeBlobs()
        {
            DisposeAll(_settleBlobs);
            DisposeAll(_windowBlobs);
            if (_finishingBlob != null)
            {
                _finishingBlob.Dispose();
                _finishingBlob = null;
            }
            _shapedSize = -1f;
        }

        private static void DisposeAll(SKTextBlob[] blobs)
        {
            for (int i = 0; i < blobs.Length; i++)
            {
                if (blobs[i] != null)
                {
                    blobs[i].Dispose();
                    blobs[i] = null;
                }
            }
        }
    }
}
