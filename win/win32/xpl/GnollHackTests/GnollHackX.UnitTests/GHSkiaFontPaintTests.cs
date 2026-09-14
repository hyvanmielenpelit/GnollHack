using GnollHackM;
using SkiaSharp;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Exercises the MAUI-only text blob cache. The paints are built with small budgets
       through the constructor so each bound trips within a handful of draws. */
    public class GHSkiaFontPaintTests : IDisposable
    {
        private readonly SKSurface _surface;
        private readonly SKCanvas _canvas;

        public GHSkiaFontPaintTests()
        {
            _surface = SKSurface.Create(new SKImageInfo(128, 64));
            _canvas = _surface.Canvas;
        }

        public void Dispose()
        {
            _surface.Dispose();
        }

        private static GHSkiaFontPaint NewCachingPaint(int maxBlobs, int maxTextLength, int maxTotalChars)
        {
            GHSkiaFontPaint paint = new GHSkiaFontPaint(maxBlobs, maxTextLength, maxTotalChars);
            paint.Typeface = SKTypeface.Default;
            paint.TextSize = 12f;
            paint.SyncBlobCache(true);
            return paint;
        }

        private void Draw(GHSkiaFontPaint paint, string text)
        {
            paint.DrawTextOnCanvas(_canvas, text, 4f, 20f);
        }

        [Fact]
        public void Defaults_ComeFromGHConstants()
        {
            using (GHSkiaFontPaint paint = new GHSkiaFontPaint())
            {
                Assert.False(paint.CacheTextBlobs);
                Assert.Equal(0, paint.BlobCacheCount);
                Assert.NotNull(paint.Paint);
                Assert.Equal(12f, paint.TextSize);
            }
        }

        [Fact]
        public void SameText_MissesThenHits()
        {
            using (GHSkiaFontPaint paint = NewCachingPaint(GHConstants.MaxTextBlobCacheSize, GHConstants.MaxCachedTextLength, GHConstants.MaxCachedTotalChars))
            {
                Assert.True(paint.CacheTextBlobs);
                Draw(paint, "Hello");
                Assert.Equal(1L, paint.BlobCacheMisses);
                Assert.Equal(0L, paint.BlobCacheHits);
                Assert.Equal(1, paint.BlobCacheCount);
                Assert.Equal(5, paint.BlobCacheChars);

                Draw(paint, "Hello");
                Assert.Equal(1L, paint.BlobCacheMisses);
                Assert.Equal(1L, paint.BlobCacheHits);
                Assert.Equal(1, paint.BlobCacheCount);
                Assert.Equal(0L, paint.BlobCacheFlushes);

                /* The span overloads share the cache with the string overloads */
                paint.DrawTextOnCanvas(_canvas, "Hello".AsSpan(), 4f, 40f);
                paint.DrawTextOnCanvas(_canvas, "Hello".AsSpan(), 4f, 40f, SKTextAlign.Center);
                paint.DrawTextOnCanvas(_canvas, "Hello", 4f, 40f, SKTextAlign.Right);
                Assert.Equal(1L, paint.BlobCacheMisses);
                Assert.Equal(4L, paint.BlobCacheHits);
            }
        }

        [Fact]
        public void CacheDisabled_NeverFiles()
        {
            using (GHSkiaFontPaint paint = new GHSkiaFontPaint(4, 16, 64))
            {
                paint.Typeface = SKTypeface.Default;
                Draw(paint, "Hello");
                Draw(paint, "Hello");
                Assert.Equal(0L, paint.BlobCacheMisses);
                Assert.Equal(0L, paint.BlobCacheHits);
                Assert.Equal(0, paint.BlobCacheCount);
            }
        }

        [Fact]
        public void EntryBudget_FlushesEverything()
        {
            using (GHSkiaFontPaint paint = NewCachingPaint(4, 16, 4096))
            {
                Draw(paint, "a");
                Draw(paint, "b");
                Draw(paint, "c");
                Draw(paint, "d");
                Assert.Equal(4, paint.BlobCacheCount);
                Assert.Equal(0L, paint.BlobCacheFlushes);

                Draw(paint, "e");
                Assert.Equal(1L, paint.BlobCacheFlushes);
                Assert.Equal(1, paint.BlobCacheCount);
                Assert.Equal(1, paint.BlobCacheChars);

                /* The flushed entries miss again */
                Draw(paint, "a");
                Assert.Equal(6L, paint.BlobCacheMisses);
                Assert.Equal(2, paint.BlobCacheCount);
            }
        }

        [Fact]
        public void CharacterBudget_FlushesEverything()
        {
            using (GHSkiaFontPaint paint = NewCachingPaint(100, 16, 20))
            {
                Draw(paint, "abcdefghij");
                Draw(paint, "klmnopqrs");
                Assert.Equal(19, paint.BlobCacheChars);
                Assert.Equal(2, paint.BlobCacheCount);
                Assert.Equal(0L, paint.BlobCacheFlushes);

                Draw(paint, "tuv");
                Assert.Equal(1L, paint.BlobCacheFlushes);
                Assert.Equal(1, paint.BlobCacheCount);
                Assert.Equal(3, paint.BlobCacheChars);
            }
        }

        [Fact]
        public void TextLongerThanLimit_IsNotCached()
        {
            using (GHSkiaFontPaint paint = NewCachingPaint(100, 4, 4096))
            {
                Draw(paint, "abcde");
                Draw(paint, "abcde");
                Assert.Equal(0L, paint.BlobCacheMisses);
                Assert.Equal(0L, paint.BlobCacheHits);
                Assert.Equal(0, paint.BlobCacheCount);

                Draw(paint, "abcd");
                Assert.Equal(1L, paint.BlobCacheMisses);
                Assert.Equal(1, paint.BlobCacheCount);
            }
        }

        [Fact]
        public void EmptyText_IsNotCached()
        {
            using (GHSkiaFontPaint paint = NewCachingPaint(100, 16, 4096))
            {
                paint.DrawTextOnCanvas(_canvas, ReadOnlySpan<char>.Empty, 4f, 20f);
                paint.DrawTextOnCanvas(_canvas, ReadOnlySpan<char>.Empty, 4f, 20f, SKTextAlign.Left);
                Assert.Equal(0L, paint.BlobCacheMisses);
                Assert.Equal(0, paint.BlobCacheCount);
            }
        }

        [Fact]
        public void FontBucketBound_FlushesOnTooManySizes()
        {
            using (GHSkiaFontPaint paint = NewCachingPaint(1000, 16, 65536))
            {
                int i;
                for (i = 0; i < GHConstants.MaxTextBlobFontBuckets; i++)
                {
                    paint.TextSize = 10f + i;
                    Draw(paint, "x");
                }
                Assert.Equal(GHConstants.MaxTextBlobFontBuckets, paint.BlobCacheCount);
                Assert.Equal(0L, paint.BlobCacheFlushes);

                /* Revisiting an existing bucket does not mint one */
                paint.TextSize = 10f;
                Draw(paint, "x");
                Assert.Equal(0L, paint.BlobCacheFlushes);
                Assert.Equal(1L, paint.BlobCacheHits);

                paint.TextSize = 10f + GHConstants.MaxTextBlobFontBuckets;
                Draw(paint, "x");
                Assert.Equal(1L, paint.BlobCacheFlushes);
                Assert.Equal(1, paint.BlobCacheCount);
            }
        }

        [Fact]
        public void TypefaceChange_MintsSeparateBucket()
        {
            using (GHSkiaFontPaint paint = NewCachingPaint(1000, 16, 65536))
            {
                SKTypeface original = paint.Typeface;
                Draw(paint, "x");
                paint.Typeface = null;
                Draw(paint, "x");
                Assert.Equal(2L, paint.BlobCacheMisses);
                Assert.Equal(2, paint.BlobCacheCount);

                /* The same instance keys the same bucket: identity, not value, is compared */
                paint.Typeface = original;
                Draw(paint, "x");
                Assert.Equal(1L, paint.BlobCacheHits);
            }
        }

        [Fact]
        public void BypassBlobCache_SkipsLookupAndFiling()
        {
            using (GHSkiaFontPaint paint = NewCachingPaint(100, 16, 4096))
            {
                Draw(paint, "x");
                paint.BypassBlobCache = true;
                Draw(paint, "x");
                Draw(paint, "y");
                Assert.Equal(1L, paint.BlobCacheMisses);
                Assert.Equal(0L, paint.BlobCacheHits);
                Assert.Equal(1, paint.BlobCacheCount);

                /* Reset clears the bypass */
                paint.Reset();
                Assert.False(paint.BypassBlobCache);
            }
        }

        [Fact]
        public void ClearBlobCache_EmptiesButKeepsCounters()
        {
            using (GHSkiaFontPaint paint = NewCachingPaint(100, 16, 4096))
            {
                Draw(paint, "x");
                Draw(paint, "y");
                paint.ClearBlobCache();
                Assert.Equal(0, paint.BlobCacheCount);
                Assert.Equal(0, paint.BlobCacheChars);
                Assert.Equal(2L, paint.BlobCacheMisses);

                /* Idempotent, and usable again afterwards */
                paint.ClearBlobCache();
                Draw(paint, "x");
                Assert.Equal(3L, paint.BlobCacheMisses);
                Assert.Equal(1, paint.BlobCacheCount);
            }
        }

        [Fact]
        public void SyncBlobCache_AppliesRequestedClearAndSettingChange()
        {
            using (GHSkiaFontPaint paint = NewCachingPaint(100, 16, 4096))
            {
                Draw(paint, "x");
                paint.RequestBlobCacheClear();
                Assert.Equal(1, paint.BlobCacheCount);
                paint.SyncBlobCache(true);
                Assert.Equal(0, paint.BlobCacheCount);

                Draw(paint, "x");
                paint.SyncBlobCache(false);
                Assert.False(paint.CacheTextBlobs);
                Assert.Equal(0, paint.BlobCacheCount);

                Draw(paint, "x");
                Assert.Equal(0, paint.BlobCacheCount);
            }
        }

        [Fact]
        public void Dispose_LeavesCacheEmpty()
        {
            GHSkiaFontPaint paint = NewCachingPaint(100, 16, 4096);
            Draw(paint, "x");
            Draw(paint, "y");
            Assert.Equal(2, paint.BlobCacheCount);

            paint.Dispose();
            Assert.Equal(0, paint.BlobCacheCount);
            Assert.Equal(0, paint.BlobCacheChars);

            /* A second Dispose is a no-op */
            paint.Dispose();
        }

        [Fact]
        public void MeasureText_MatchesCachedWidth()
        {
            using (GHSkiaFontPaint paint = NewCachingPaint(100, 16, 4096))
            {
                float width = paint.MeasureText("Hello");
                Assert.True(width > 0f);
                Assert.Equal(width, paint.MeasureText("Hello".AsSpan()));
                SKRect bounds = SKRect.Empty;
                paint.MeasureText("Hello", ref bounds);
                Assert.True(bounds.Width > 0f);
            }
        }
    }
}
