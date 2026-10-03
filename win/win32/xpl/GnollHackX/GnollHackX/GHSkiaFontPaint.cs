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

        /* The width is measured with the same font state the blob was shaped with, so it
           is keyed by the bucket exactly as the blob is. Alignment needs it on every
           draw, and measuring shapes the text again. */
        /* A class, so that the last-used stamp written on a hit persists in the dictionary:
           the span-keyed alternate lookup returns values by copy. */
        private sealed class BlobEntry
        {
            public readonly SKTextBlob Blob;
            public readonly float Width;
            public long LastUsedFrame;

            public BlobEntry(SKTextBlob blob, float width, long frame)
            {
                Blob = blob;
                Width = width;
                LastUsedFrame = frame;
            }
        }

        /* One dictionary per font state the blobs were shaped with. BySpan matches a
           ReadOnlySpan<char> against the string keys without allocating one, which is what
           lets the span draw overloads hit the cache at all. */
        private sealed class FontBucket
        {
            public Dictionary<string, BlobEntry> Blobs;
            public Dictionary<string, BlobEntry>.AlternateLookup<ReadOnlySpan<char>> BySpan;
            public long LastUsedFrame;
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
        private bool _bypassBlobCache;
        private int _blobCacheClearRequested;
        private bool _disposed;
        private long _blobCacheHits;
        private long _blobCacheMisses;
        private long _blobCacheFlushes;
        private long _blobCacheEvictions;
        /* Advanced once per paint by SyncBlobCache; entries and buckets carry the frame they
           were last drawn in */
        private long _frame;
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
            _bypassBlobCache = false;
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

        /* Applies to geometry drawn through Paint. Text edges are governed by the font's
           Edging on MAUI and by this flag on Xamarin. */
        public bool IsAntialias
        {
            get
            {
                return _paint.IsAntialias;
            }
            set
            {
                _paint.IsAntialias = value;
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
                   this bound is load-bearing, not defensive. Everything goes only when every
                   bucket was drawn in this paint. */
                if (_blobBuckets.Count >= GHConstants.MaxTextBlobFontBuckets && !EvictLeastRecentlyUsedBucket())
                {
                    ClearBlobCache();
                    _blobCacheFlushes++;
                }

                bucket = new FontBucket();
                bucket.Blobs = new Dictionary<string, BlobEntry>(StringComparer.Ordinal);
                bucket.BySpan = bucket.Blobs.GetAlternateLookup<ReadOnlySpan<char>>();
                _blobBuckets.Add(key, bucket);
            }

            bucket.LastUsedFrame = _frame;
            _currentBucket = bucket;
            return bucket;
        }

        /* Removes the bucket drawn longest ago, provided it was not drawn in this paint.
           _currentBucket is null while GetCurrentBucket resolves, and every bucket resolved in
           this paint carries the current frame, so the victim is never in use. */
        private bool EvictLeastRecentlyUsedBucket()
        {
            FontKey victimKey = default(FontKey);
            FontBucket victim = null;
            foreach (KeyValuePair<FontKey, FontBucket> kv in _blobBuckets)
            {
                if (kv.Value.LastUsedFrame < _frame && (victim == null || kv.Value.LastUsedFrame < victim.LastUsedFrame))
                {
                    victimKey = kv.Key;
                    victim = kv.Value;
                }
            }

            if (victim == null)
                return false;

            /* Counted out first, so a throwing Dispose cannot leave the counts too high */
            foreach (string text in victim.Blobs.Keys)
            {
                _blobCount--;
                _cachedChars -= text.Length;
            }

            try
            {
                foreach (BlobEntry entry in victim.Blobs.Values)
                {
                    if (entry != null && entry.Blob != null)
                        entry.Blob.Dispose();
                }
            }
            finally
            {
                victim.Blobs.Clear();
                _blobBuckets.Remove(victimKey);
                _blobCacheEvictions++;
            }
            return true;
        }

        /* Disposes entries not drawn for TextBlobCacheAgingFrames paints, then, if that leaves
           either bound above its low-water mark, every entry not drawn in this paint. Buckets
           left empty go too, unless drawn in this paint, so the current bucket survives. */
        private void SweepStaleEntries(int incomingChars)
        {
            if (_blobBuckets == null)
                return;

            int removed = 0;
            try
            {
                removed += RemoveEntriesUnusedSince(_frame - GHConstants.TextBlobCacheAgingFrames);

                long lowWaterCount = (long)_maxCachedBlobs * GHConstants.TextBlobCacheSweepLowWaterPercent / 100;
                long lowWaterChars = (long)_maxCachedTotalChars * GHConstants.TextBlobCacheSweepLowWaterPercent / 100;
                if (_blobCount > lowWaterCount || _cachedChars + incomingChars > lowWaterChars)
                    removed += RemoveEntriesUnusedSince(_frame);
            }
            finally
            {
                /* Removal while enumerating is supported for Dictionary since .NET Core 3.0 */
                foreach (KeyValuePair<FontKey, FontBucket> kv in _blobBuckets)
                {
                    if (kv.Value.Blobs.Count == 0 && kv.Value.LastUsedFrame != _frame)
                        _blobBuckets.Remove(kv.Key);
                }
                if (removed > 0)
                    _blobCacheEvictions++;
            }
        }

        /* Disposes every entry last drawn before the given frame and returns how many */
        private int RemoveEntriesUnusedSince(long frame)
        {
            int removed = 0;
            foreach (FontBucket bucket in _blobBuckets.Values)
            {
                foreach (KeyValuePair<string, BlobEntry> kv in bucket.Blobs)
                {
                    if (kv.Value != null && kv.Value.LastUsedFrame >= frame)
                        continue;

                    /* Removed and counted before the dispose, so a throwing Dispose cannot
                       leave the counts too high or the blob reachable */
                    bucket.Blobs.Remove(kv.Key);
                    _blobCount--;
                    _cachedChars -= kv.Key.Length;
                    removed++;
                    if (kv.Value != null && kv.Value.Blob != null)
                        kv.Value.Blob.Dispose();
                }
            }
            return removed;
        }

        /* Text that will not be cached: the caller keeps its own `using` blob instead. The
           length cap stops one long string from consuming the character budget. */
        private bool CanCacheText(ReadOnlySpan<char> text)
        {
            return _cacheTextBlobs
                && !_bypassBlobCache
                && !_disposed
                && !text.IsEmpty
                && text.Length <= _maxCachedTextLength;
        }

        /* Returns an entry owned by the cache; the caller must not dispose its blob. False
           when the text shapes to no glyphs, in which case nothing is inserted and there
           is nothing to own. */
        private bool TryGetOrCreateEntry(ReadOnlySpan<char> text, out BlobEntry entry)
        {
            FontBucket bucket = GetCurrentBucket();

            if (bucket.BySpan.TryGetValue(text, out entry) && entry != null)
            {
                entry.LastUsedFrame = _frame;
                _blobCacheHits++;
                return true;
            }

            SKTextBlob blob = SKTextBlob.Create(text, _font);
            _blobCacheMisses++;
            if (blob == null)
                return false;

            /* Either bound may trip first: the entry count guards wrapper and dictionary
               overhead, the character count guards native glyph data. */
            if (_blobCount >= _maxCachedBlobs
                || _cachedChars + text.Length > _maxCachedTotalChars)
            {
                /* Stale entries go first, then everything not drawn in this paint; the sweep
                   never removes the current bucket, so the local above stays valid. Evicting
                   single entries from this paint's own text would be refilled within the same
                   frame and thrash, so if that text alone exceeds the bound, everything goes.
                   ClearBlobCache resets _currentBucket, so the bucket is then re-resolved
                   before the blob is filed -- the local above would refer to an orphan. */
                SweepStaleEntries(text.Length);
                if (_blobCount >= _maxCachedBlobs
                    || _cachedChars + text.Length > _maxCachedTotalChars)
                {
                    ClearBlobCache();
                    _blobCacheFlushes++;
                    bucket = GetCurrentBucket();
                }
            }

            /* Measured without the paint, matching the uncached span overload: passing it
               would fold stroke width into the advance and shift aligned text. */
            entry = new BlobEntry(blob, _font.MeasureText(text), _frame);

            /* Indexer, not Add: the TryGetValue above missed, so the key cannot already be
               present, and the indexer cannot throw and strand the blob unreferenced. */
            bucket.Blobs[text.ToString()] = entry;
            _blobCount++;
            _cachedChars += text.Length;
            return true;
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
                        foreach (BlobEntry entry in bucket.Blobs.Values)
                        {
                            if (entry != null && entry.Blob != null)
                                entry.Blob.Dispose();
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
           text is drawn. Starts a new frame for eviction, and applies a setting change in
           either direction and any clear another thread has requested. */
        public void SyncBlobCache(bool enabled)
        {
            /* Every bucket drawn in this paint is then resolved, and stamped, at least once,
               even when the font state is unchanged from the previous paint */
            _frame++;
            _currentBucket = null;

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

        /* Text drawn while this is set is neither looked up nor filed. For text a frame
           draws once -- a scrolling history, where a row is exposed for a few frames and
           never returns -- a lookup can only miss, while filing it costs a key allocation
           now and a full-cache disposal when the bound trips. Reset clears it, so a paint
           that returns early cannot leave it set. */
        public bool BypassBlobCache
        {
            get { return _bypassBlobCache; }
            set { _bypassBlobCache = value; }
        }

        public int BlobCacheCount { get { return _blobCount; } }
        public int BlobCacheBuckets { get { Dictionary<FontKey, FontBucket> buckets = _blobBuckets; return buckets == null ? 0 : buckets.Count; } }
        public int BlobCacheChars { get { return _cachedChars; } }
        public long BlobCacheHits { get { return _blobCacheHits; } }
        public long BlobCacheMisses { get { return _blobCacheMisses; } }
        public long BlobCacheFlushes { get { return _blobCacheFlushes; } }
        public long BlobCacheEvictions { get { return _blobCacheEvictions; } }

        /* The entry belongs to the cache and its blob is never disposed here. A shaped blob
           carries its own glyph positions, so alignment is applied by shifting the origin
           -- the same way the uncached span overload has always done it. */
        private void DrawCachedText(SKCanvas canvas, ReadOnlySpan<char> text, float x, float y, SKTextAlign textAlign)
        {
            BlobEntry entry;
            if (!TryGetOrCreateEntry(text, out entry))
                return;

            if (textAlign != SKTextAlign.Left)
            {
                float width = entry.Width;
                if (textAlign == SKTextAlign.Center)
                    width *= 0.5f;
                x -= width;
            }

            canvas.DrawText(entry.Blob, x, y, _paint);
        }
#else
        /* The blob cache is a MAUI-only feature: the Xamarin build has no SKFont and no
           Dictionary alternate lookup. These no-ops let the shared GamePage call the same
           API unconditionally rather than guarding every call site. */
        public void ClearBlobCache() { }
        public void RequestBlobCacheClear() { }
        public void SyncBlobCache(bool enabled) { }
        public bool CacheTextBlobs { get { return false; } }
        public bool BypassBlobCache { get { return false; } set { } }
        public int BlobCacheCount { get { return 0; } }
        public int BlobCacheBuckets { get { return 0; } }
        public int BlobCacheChars { get { return 0; } }
        public long BlobCacheHits { get { return 0; } }
        public long BlobCacheMisses { get { return 0; } }
        public long BlobCacheFlushes { get { return 0; } }
        public long BlobCacheEvictions { get { return 0; } }
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
