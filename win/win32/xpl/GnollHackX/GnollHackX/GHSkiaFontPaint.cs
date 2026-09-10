using SkiaSharp;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading;

#if GNH_MAUI
namespace GnollHackM
#else
namespace GnollHackX
#endif
{
    public sealed class GHSkiaFontPaint : IDisposable
    {
        SKPaint _paint = new SKPaint();
#if GNH_MAUI
        SKFont _font = new SKFont();

        /* One dictionary per font state the blobs were shaped with. BySpan matches a
           ReadOnlySpan<char> against the string keys without allocating one, which is what
           lets the span draw overloads hit the cache at all. */
        private sealed class FontBucket
        {
            public Dictionary<string, SKTextBlob> Blobs;
            public Dictionary<string, SKTextBlob>.AlternateLookup<ReadOnlySpan<char>> BySpan;
        }

        /* A blob is fixed at creation by the typeface and size alone: it keeps drawing
           correctly after the font changes, and under any paint. Nothing else on _font is
           mutated after the constructor sets Edging, so nothing else belongs here. */
        private readonly struct FontKey
        {
            public readonly SKTypeface Typeface;
            public readonly float Size;

            public FontKey(SKTypeface typeface, float size)
            {
                Typeface = typeface;
                Size = size;
            }
        }

        /* SKObject defines no value equality, and a disposed typeface's native handle can
           be reused by the next one, so identity is the only sound comparison. */
        private sealed class FontKeyComparer : IEqualityComparer<FontKey>
        {
            public static readonly FontKeyComparer Instance = new FontKeyComparer();

            public bool Equals(FontKey a, FontKey b)
            {
                /* float.Equals, not ==, so that two NaN sizes match and cannot strand an
                   unreachable bucket that still counts against the bound. */
                return ReferenceEquals(a.Typeface, b.Typeface) && a.Size.Equals(b.Size);
            }

            public int GetHashCode(FontKey k)
            {
                int h = k.Typeface == null ? 0 : RuntimeHelpers.GetHashCode(k.Typeface);
                return (h * 397) ^ k.Size.GetHashCode();
            }
        }

        private readonly int _maxCachedBlobs;
        private readonly int _maxCachedTextLength;
        private readonly int _maxCachedTotalChars;

        private Dictionary<FontKey, FontBucket> _blobBuckets;
        /* Null whenever _font may have been mutated: the Typeface and TextSize setters and
           Reset all invalidate it. A stale value files blobs under the previous font and
           renders wrong glyphs, so any future write to _font must invalidate it too. */
        private FontBucket _currentBucket;
        /* The font state shadowed, rather than read back from _font, so that the key does
           not depend on SKFont getter behaviour and every invalidation point is explicit. */
        private SKTypeface _keyTypeface;
        private float _keySize = 12;
        private int _blobCount;
        private int _cachedChars;
        private bool _cacheTextBlobs;
        private int _blobCacheClearRequested;
        private bool _disposed;
        private long _blobCacheHits;
        private long _blobCacheMisses;
        private long _blobCacheFlushes;
#endif

        public GHSkiaFontPaint(
            int maxCachedBlobs = GHConstants.MaxTextBlobCacheSize,
            int maxCachedTextLength = GHConstants.MaxCachedTextLength,
            int maxCachedTotalChars = GHConstants.MaxCachedTotalChars)
        {
#if GNH_MAUI
            _maxCachedBlobs = Math.Max(1, maxCachedBlobs);
            _maxCachedTextLength = Math.Max(1, maxCachedTextLength);
            _maxCachedTotalChars = Math.Max(1, maxCachedTotalChars);
            _font.Edging = SKFontEdging.Antialias;
#else
            _paint.IsAntialias = true;
#endif
        }

        public void Reset()
        {
            _paint.Color = new SKColor(0, 0, 0, 255);
            _paint.Style = SKPaintStyle.Fill;
            _paint.StrokeWidth = 0;
            _paint.IsAntialias = false;
            _paint.BlendMode = SKBlendMode.SrcOver;
            _paint.ColorFilter = null;
            _paint.Shader = null;
            _paint.MaskFilter = null;
            _paint.PathEffect = null;
#if GNH_MAUI
            _font.Typeface = null;
            _font.Size = 12;
            /* Written straight to _font above, bypassing the setters, so the cache key is
               invalidated by hand */
            _keyTypeface = null;
            _keySize = 12;
            _currentBucket = null;
#else
            _paint.Typeface = null;
            _paint.TextSize = 12;
#endif
        }

        public void Dispose()
        {
#if GNH_MAUI
            if (_disposed)
                return;
            _disposed = true;
            /* Before the paint and font: nothing else may reach the blobs afterwards */
            ClearBlobCache();
#endif
            _paint.Dispose();
#if GNH_MAUI
            _font.Dispose();
#endif
        }

        public SKPaint Paint { get { return _paint; } }

        public SKTypeface Typeface
        {
            get
            {
#if GNH_MAUI
                return _font.Typeface;
#else
                return _paint.Typeface;
#endif
            }
            set
            {
#if GNH_MAUI
                if (!ReferenceEquals(_keyTypeface, value))
                {
                    _keyTypeface = value;
                    InvalidateCurrentBucket();
                }
                _font.Typeface = value;
#else
                _paint.Typeface = value;
#endif
            }
        }

        public float TextSize
        {
            get
            {
#if GNH_MAUI
                return _font.Size;
#else
                return _paint.TextSize;
#endif
            }
            set
            {
#if GNH_MAUI
                if (_keySize != value)
                {
                    _keySize = value;
                    InvalidateCurrentBucket();
                }
                _font.Size = value;
#else
                _paint.TextSize = value;
#endif
            }
        }

        public SKPaintStyle Style
        {
            get
            {
                return _paint.Style;
            }
            set
            {
                _paint.Style = value;
            }
        }

        public float StrokeWidth
        {
            get
            {
                return _paint.StrokeWidth;
            }
            set
            {
                _paint.StrokeWidth = value;
            }
        }

        public SKPathEffect PathEffect
        {
            get
            {
                return _paint.PathEffect;
            }
            set
            {
                _paint.PathEffect = value;
            }
        }

        public SKMaskFilter MaskFilter
        {
            get
            {
                return _paint.MaskFilter;
            }
            set
            {
                _paint.MaskFilter = value;
            }
        }

        public SKColor Color
        {
            get
            {
                return _paint.Color;
            }
            set
            {
                _paint.Color = value;
            }
        }

        public SKFontMetrics FontMetrics
        {
            get
            {
#if GNH_MAUI
                return _font.Metrics;
#else
                return _paint.FontMetrics;
#endif
            }
        }

        public float FontSpacing
        {
            get
            {
#if GNH_MAUI
                return _font.Spacing;
#else
                return _paint.FontSpacing;
#endif
            }
        }

#if GNH_MAUI
        private void InvalidateCurrentBucket()
        {
            _currentBucket = null;
        }

        private FontBucket GetCurrentBucket()
        {
            if (_currentBucket != null)
                return _currentBucket;

            if (_blobBuckets == null)
                _blobBuckets = new Dictionary<FontKey, FontBucket>(FontKeyComparer.Instance);

            FontKey key = new FontKey(_keyTypeface, _keySize);
            FontBucket bucket;
            if (!_blobBuckets.TryGetValue(key, out bucket))
            {
                /* A size derived from a changing canvas scale mints a bucket per frame, so
                   this bound is load-bearing, not defensive. */
                if (_blobBuckets.Count >= GHConstants.MaxTextBlobFontBuckets)
                {
                    ClearBlobCache();
                    _blobCacheFlushes++;
                }

                bucket = new FontBucket();
                bucket.Blobs = new Dictionary<string, SKTextBlob>(StringComparer.Ordinal);
                bucket.BySpan = bucket.Blobs.GetAlternateLookup<ReadOnlySpan<char>>();
                _blobBuckets.Add(key, bucket);
            }

            _currentBucket = bucket;
            return bucket;
        }

        /* Text that will not be cached: the caller keeps its own `using` blob instead. The
           length cap stops one long string from consuming the character budget. */
        private bool CanCacheText(ReadOnlySpan<char> text)
        {
            return _cacheTextBlobs
                && !_disposed
                && !text.IsEmpty
                && text.Length <= _maxCachedTextLength;
        }

        /* Returns a blob owned by the cache; the caller must not dispose it. Null when the
           text shapes to no glyphs, in which case nothing is inserted and there is nothing
           to own. */
        private SKTextBlob GetOrCreateBlob(ReadOnlySpan<char> text)
        {
            FontBucket bucket = GetCurrentBucket();

            SKTextBlob cached;
            if (bucket.BySpan.TryGetValue(text, out cached))
            {
                _blobCacheHits++;
                return cached;
            }

            SKTextBlob blob = SKTextBlob.Create(text, _font);
            _blobCacheMisses++;
            if (blob == null)
                return null;

            /* Either bound may trip first: the entry count guards wrapper and dictionary
               overhead, the character count guards native glyph data. */
            if (_blobCount >= _maxCachedBlobs
                || _cachedChars + text.Length > _maxCachedTotalChars)
            {
                /* Everything goes, rather than one entry: the working set is a whole
                   frame's text, so evicting a single blob would be refilled within the same
                   frame and the cache would thrash at the bound. ClearBlobCache resets
                   _currentBucket, so the bucket is re-resolved before the blob is filed --
                   the local above now refers to an orphan. */
                ClearBlobCache();
                _blobCacheFlushes++;
                bucket = GetCurrentBucket();
            }

            /* Indexer, not Add: the TryGetValue above missed, so the key cannot already be
               present, and the indexer cannot throw and strand the blob unreferenced. */
            bucket.Blobs[text.ToString()] = blob;
            _blobCount++;
            _cachedChars += text.Length;
            return blob;
        }

        /* Disposes every cached blob. Idempotent, and safe when the cache was never
           populated. Must run on the thread that owns this instance; other threads use
           RequestBlobCacheClear. */
        public void ClearBlobCache()
        {
            try
            {
                if (_blobBuckets != null)
                {
                    foreach (FontBucket bucket in _blobBuckets.Values)
                    {
                        if (bucket == null || bucket.Blobs == null)
                            continue;
                        foreach (SKTextBlob blob in bucket.Blobs.Values)
                        {
                            if (blob != null)
                                blob.Dispose();
                        }
                        bucket.Blobs.Clear();
                    }
                }
            }
            finally
            {
                /* Reset even if a Dispose threw: a half-disposed cache that is still
                   reachable is the one state that could hand a disposed blob to DrawText. */
                if (_blobBuckets != null)
                    _blobBuckets.Clear();
                _currentBucket = null;
                _blobCount = 0;
                _cachedChars = 0;
            }
        }

        /* Callable from any thread. The clear itself happens on the owning thread, at the
           top of that canvas's next paint. */
        public void RequestBlobCacheClear()
        {
            Interlocked.Exchange(ref _blobCacheClearRequested, 1);
        }

        /* Call once at the top of the paint handler that owns this instance, before any
           text is drawn. Applies a setting change in either direction and any clear another
           thread has requested. */
        public void SyncBlobCache(bool enabled)
        {
            bool clear = Interlocked.Exchange(ref _blobCacheClearRequested, 0) != 0;

            if (_cacheTextBlobs != enabled)
            {
                _cacheTextBlobs = enabled;
                /* Both directions clear: switching off releases the native memory rather
                   than leaving it dormant, and switching on does not resume from entries
                   shaped before whatever prompted the change. */
                clear = true;
            }

            if (clear)
                ClearBlobCache();
        }

        public bool CacheTextBlobs { get { return _cacheTextBlobs; } }
        public int BlobCacheCount { get { return _blobCount; } }
        public int BlobCacheChars { get { return _cachedChars; } }
        public long BlobCacheHits { get { return _blobCacheHits; } }
        public long BlobCacheMisses { get { return _blobCacheMisses; } }
        public long BlobCacheFlushes { get { return _blobCacheFlushes; } }

        /* The blob belongs to the cache and is never disposed here. A shaped blob carries
           its own glyph positions, so alignment is applied by shifting the origin -- the
           same way the uncached span overload has always done it. */
        private void DrawCachedText(SKCanvas canvas, ReadOnlySpan<char> text, float x, float y, SKTextAlign textAlign)
        {
            SKTextBlob blob = GetOrCreateBlob(text);
            if (blob == null)
                return;

            if (textAlign != SKTextAlign.Left)
            {
                var width = _font.MeasureText(text);
                if (textAlign == SKTextAlign.Center)
                    width *= 0.5f;
                x -= width;
            }

            canvas.DrawText(blob, x, y, _paint);
        }
#else
        /* The blob cache is a MAUI-only feature: the Xamarin build has no SKFont and no
           Dictionary alternate lookup. These no-ops let the shared GamePage call the same
           API unconditionally rather than guarding every call site. */
        public void ClearBlobCache() { }
        public void RequestBlobCacheClear() { }
        public void SyncBlobCache(bool enabled) { }
        public bool CacheTextBlobs { get { return false; } }
        public int BlobCacheCount { get { return 0; } }
        public int BlobCacheChars { get { return 0; } }
        public long BlobCacheHits { get { return 0; } }
        public long BlobCacheMisses { get { return 0; } }
        public long BlobCacheFlushes { get { return 0; } }
#endif

        public void DrawTextOnCanvas(SKCanvas canvas, string text, float x, float y, SKTextAlign textAlign)
        {
#if GNH_MAUI
            if (text != null && CanCacheText(text.AsSpan()))
            {
                DrawCachedText(canvas, text.AsSpan(), x, y, textAlign);
                return;
            }
            canvas.DrawText(text, x, y, textAlign, _font, _paint);
#else
            SKTextAlign oldAlign = _paint.TextAlign;
            _paint.TextAlign = textAlign;
            canvas.DrawText(text, x, y, _paint);
            _paint.TextAlign = oldAlign;
#endif
        }

        public void DrawTextOnCanvas(SKCanvas canvas, string text, SKPoint p, SKTextAlign textAlign)
        {
            DrawTextOnCanvas(canvas, text, p.X, p.Y, textAlign);
        }

        public void DrawTextOnCanvas(SKCanvas canvas, ReadOnlySpan<char> text, float x, float y, SKTextAlign textAlign)
        {
#if GNH_MAUI
            if (CanCacheText(text))
            {
                DrawCachedText(canvas, text, x, y, textAlign);
                return;
            }

            using (SKTextBlob textBlob = SKTextBlob.Create(text, _font))
            {
                if (textBlob == null)
                    return;

                if (textAlign != SKTextAlign.Left)
                {
                    var width = _font.MeasureText(text);
                    if (textAlign == SKTextAlign.Center)
                        width *= 0.5f;
                    x -= width;
                }

                canvas.DrawText(textBlob, x, y, _paint);
            }
#else
            SKTextAlign oldAlign = _paint.TextAlign;
            _paint.TextAlign = textAlign;
            using (SKTextBlob sKTextBlob = SKTextBlob.Create(text, _paint.ToFont()))
            {
                canvas.DrawText(sKTextBlob, x, y, _paint);
            }
            _paint.TextAlign = oldAlign;
#endif
        }

        public void DrawTextOnCanvas(SKCanvas canvas, ReadOnlySpan<char> text, SKPoint p, SKTextAlign textAlign)
        {
            DrawTextOnCanvas(canvas, text, p.X, p.Y, textAlign);
        }

        public void DrawTextOnCanvas(SKCanvas canvas, string text, float x, float y)
        {
#if GNH_MAUI
            if (text != null && CanCacheText(text.AsSpan()))
            {
                DrawCachedText(canvas, text.AsSpan(), x, y, SKTextAlign.Left);
                return;
            }
            canvas.DrawText(text, x, y, SKTextAlign.Left, _font, _paint);
#else
            canvas.DrawText(text, x, y, _paint);
#endif
        }

        public void DrawTextOnCanvas(SKCanvas canvas, string text, SKPoint p)
        {
            DrawTextOnCanvas(canvas, text, p.X, p.Y);
        }

#if GNH_MAUI
        public void DrawTextOnCanvas(SKCanvas canvas, ReadOnlySpan<char> text, float x, float y)
        {
            if (text == ReadOnlySpan<char>.Empty)
                return;

            if (CanCacheText(text))
            {
                DrawCachedText(canvas, text, x, y, SKTextAlign.Left);
                return;
            }

            using (SKTextBlob textBlob = SKTextBlob.Create(text, _font))
            {
                if (textBlob == null)
                    return;
                canvas.DrawText(textBlob, x, y, _paint);
            }
        }

        public void DrawTextOnCanvas(SKCanvas canvas, ReadOnlySpan<char> text, SKPoint p)
        {
            DrawTextOnCanvas(canvas, text, p.X, p.Y);
        }
#endif

        public float MeasureText(string text)
        {
#if GNH_MAUI
            return _font.MeasureText(text, _paint);
#else
            return _paint.MeasureText(text);
#endif
        }

        public float MeasureText(string text, ref SKRect bounds)
        {
#if GNH_MAUI
            return _font.MeasureText(text, out bounds, _paint);
#else
            return _paint.MeasureText(text, ref bounds);
#endif
        }


        public float MeasureText(ReadOnlySpan<char> text)
        {
#if GNH_MAUI
            return _font.MeasureText(text, _paint);
#else
            return _paint.MeasureText(text);
#endif
        }

        public float MeasureText(ReadOnlySpan<char> text, ref SKRect bounds)
        {
#if GNH_MAUI
            return _font.MeasureText(text, out bounds, _paint);
#else
            return _paint.MeasureText(text, ref bounds);
#endif
        }
    }
}
