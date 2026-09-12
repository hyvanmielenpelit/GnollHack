using System;
using SkiaSharp;

#if GNH_MAUI
namespace GnollHackM
#else
namespace GnollHackX
#endif
{
    /* Accumulates sprites per tile sheet for SKCanvas.DrawAtlas. Only usable for
       tiles that draw as an unrotated, uniformly scaled, unsplit blit whose only
       colour adjustment is a grey Modulate: the caller owns that guarantee. */
    public class GHTileBatch
    {
        private class SheetBucket
        {
            public SKRect[] Sprites;
            public SKRotationScaleMatrix[] Xforms;
            public SKColor[] Colors;
            public int Count;
        }

        private readonly SheetBucket[] _buckets;
        private readonly int _initialCapacity;

        /* DrawAtlas reads array.Length, so a flush needs arrays whose length is the sprite
           count. Their lengths are quantised to this granularity and the arrays retained,
           so a count that differs from frame to frame reuses one rather than allocating a
           new one; the tail past the real count is zeroed, which leaves those quads
           degenerate. One set serves every sheet, as a flush completes before the next
           one starts. */
        private const int FlushGranularity = 256;
        private SKRect[][] _flushSprites;
        private SKRotationScaleMatrix[][] _flushXforms;
        private SKColor[][] _flushColors;
        private float _atlasScale;
        private float _atlasSrcHInset;
        private float _atlasSrcVInset;
        private SKRect _atlasCullRect;

        public GHTileBatch(int initialCapacity)
        {
            _initialCapacity = Math.Max(16, initialCapacity);
            _buckets = new SheetBucket[GHConstants.MaxTileSheets];

            int slots = FlushSlot(_initialCapacity) + 1;
            _flushSprites = new SKRect[slots][];
            _flushXforms = new SKRotationScaleMatrix[slots][];
            _flushColors = new SKColor[slots][];
        }

        /* Slot n holds the arrays of length n * FlushGranularity; slot 0 is never used,
           as an empty bucket does not flush */
        private static int FlushSlot(int count)
        {
            return (count + FlushGranularity - 1) / FlushGranularity;
        }

        public void SetFrameGeometry(float scale, float srcHInset, float srcVInset, in SKRect cullRect)
        {
            _atlasScale = scale;
            _atlasSrcHInset = srcHInset;
            _atlasSrcVInset = srcVInset;
            _atlasCullRect = cullRect;
        }

        public void Add(int sheetIdx, int tile_x, int tile_y, float tx, float ty,
                        byte greyVal, byte alpha)
        {
            SKRect sourceRect = new SKRect(
                tile_x + _atlasSrcHInset,
                tile_y + _atlasSrcVInset,
                tile_x + (float)GHConstants.TileWidth - _atlasSrcHInset,
                tile_y + (float)GHConstants.TileHeight - _atlasSrcVInset);

            Add(sheetIdx, in sourceRect, tx, ty, _atlasScale, greyVal, alpha);
        }

        public void Add(int sheetIdx, in SKRect sourceRect, float tx, float ty,
                        float scale, byte greyVal, byte alpha)
        {
            if (sheetIdx < 0 || sheetIdx >= _buckets.Length)
                return;

            SheetBucket b = _buckets[sheetIdx];
            if (b == null)
            {
                b = new SheetBucket();
                b.Sprites = new SKRect[_initialCapacity];
                b.Xforms = new SKRotationScaleMatrix[_initialCapacity];
                b.Colors = new SKColor[_initialCapacity];
                _buckets[sheetIdx] = b;
            }
            else if (b.Count >= b.Sprites.Length)
            {
                int newCap = b.Sprites.Length * 2;
                Array.Resize(ref b.Sprites, newCap);
                Array.Resize(ref b.Xforms, newCap);
                Array.Resize(ref b.Colors, newCap);
            }

            b.Sprites[b.Count] = sourceRect;
            b.Xforms[b.Count] = new SKRotationScaleMatrix(scale, 0f, tx, ty);
            b.Colors[b.Count] = new SKColor(greyVal, greyVal, greyVal, alpha);
            b.Count++;
        }

        public int Count(int sheetIdx)
        {
            if (sheetIdx < 0 || sheetIdx >= _buckets.Length)
                return 0;
            SheetBucket b = _buckets[sheetIdx];
            return b == null ? 0 : b.Count;
        }

        public int TotalCount
        {
            get
            {
                int total = 0;
                for (int i = 0; i < _buckets.Length; i++)
                    total += _buckets[i] == null ? 0 : _buckets[i].Count;
                return total;
            }
        }

        /* Returns the sprite count drawn, or 0 if the bucket was empty. The caller
           restores any paint state it needs preserved. */
        public int Flush(SKCanvas canvas, int sheetIdx, SKImage atlas, SKPaint paint
#if GNH_MAUI
                         , SKSamplingOptions sampling
#endif
                         )
        {
            if (sheetIdx < 0 || sheetIdx >= _buckets.Length)
                return 0;

            SheetBucket b = _buckets[sheetIdx];
            if (b == null || b.Count == 0)
                return 0;

            int count = b.Count;
            b.Count = 0;

            if (atlas == null)
                return 0;

            int slot = FlushSlot(count);
            if (slot >= _flushSprites.Length)
            {
                /* Only reachable if Add has grown a bucket past the initial capacity */
                int slots = slot + 1;
                Array.Resize(ref _flushSprites, slots);
                Array.Resize(ref _flushXforms, slots);
                Array.Resize(ref _flushColors, slots);
            }

            int padded = slot * FlushGranularity;
            if (_flushSprites[slot] == null)
            {
                _flushSprites[slot] = new SKRect[padded];
                _flushXforms[slot] = new SKRotationScaleMatrix[padded];
                _flushColors[slot] = new SKColor[padded];
            }

            SKRect[] sprites = _flushSprites[slot];
            SKRotationScaleMatrix[] xforms = _flushXforms[slot];
            SKColor[] colors = _flushColors[slot];

            Array.Copy(b.Sprites, sprites, count);
            Array.Copy(b.Xforms, xforms, count);
            Array.Copy(b.Colors, colors, count);

            /* The tail can still hold sprites left by a larger earlier flush of the same
               slot, which would draw a second time. A zeroed transform has no scale, so
               its four vertices coincide and the quad covers nothing. */
            int tail = padded - count;
            if (tail > 0)
            {
                Array.Clear(sprites, count, tail);
                Array.Clear(xforms, count, tail);
                Array.Clear(colors, count, tail);
            }

            canvas.DrawAtlas(atlas, sprites, xforms, colors,
                SKBlendMode.Modulate,
#if GNH_MAUI
                sampling,
#endif
                _atlasCullRect, paint);

            return count;
        }

        public void Reset()
        {
            for (int i = 0; i < _buckets.Length; i++)
            {
                if (_buckets[i] != null)
                    _buckets[i].Count = 0;
            }
        }
    }
}
