using System;
using System.Collections.Generic;
using SkiaSharp;

#if GNH_MAUI
namespace GnollHackM
#else
namespace GnollHackX.Controls
#endif
{
    /* Gamepad focus visuals: a color filter for a focused button bitmap, a glow following an image's alpha
       (cached per image and size), and a glow with a border for a rounded list row. Sizes are in DIP, scaled by
       the caller's DIP-to-pixel factor. */
    public static class GHGamepadFocusGlow
    {
        /* How far an image glow reaches beyond the image, in DIP */
        public const float ImageGlowExtent = 10.0f;
        private const float ImageGlowInnerSigma = 1.5f;
        private const float ImageGlowOuterSigma = 4.0f;
        private const int ImageGlowCacheSize = 8;
        private const float RowGlowSigma = 3.0f;
        private const float RowBorderWidth = 1.5f;

        private static readonly SKColor ImageGlowInnerColor = new SKColor(0xFF, 0xE0, 0x70, 0xF0);
        private static readonly SKColor ImageGlowOuterColor = new SKColor(0xFF, 0xA8, 0x00, 0xFF);

        /* Brightens and warms a focused button bitmap; the last column is normalized */
        public static readonly SKColorFilter FocusedImageColorFilter = SKColorFilter.CreateColorMatrix(new float[]
        {
            1.75f, 0.00f, 0.00f, 0.00f, 0.08f,
            0.00f, 1.75f, 0.00f, 0.00f, 0.04f,
            0.00f, 0.00f, 1.30f, 0.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 1.00f, 0.00f
        });

        private sealed class CachedGlow
        {
            public SKImage Source;
            public int Width;
            public int Height;
            public float Scale;
            public SKImage Glow;
        }

        /* Most recently used first */
        private static readonly List<CachedGlow> _glowCache = new List<CachedGlow>();
        private static readonly object _cacheLock = new object();
        private static float _rowGlowMaskSigma = 0.0f;
        private static SKMaskFilter _rowGlowMask;

        /* Draws the glow of image drawn at dest, to go behind it; reaches ImageGlowExtent beyond dest */
        public static void DrawImageGlow(SKCanvas canvas, SKImage image, SKRect dest, float dpScale)
        {
            if (canvas == null || image == null || dest.Width < 1 || dest.Height < 1)
                return;
            SKImage glow = GetImageGlow(image, (int)Math.Ceiling(dest.Width), (int)Math.Ceiling(dest.Height), dpScale);
            if (glow == null)
                return;
            float pad = (float)Math.Ceiling(ImageGlowExtent * dpScale);
#if GNH_MAUI
            canvas.DrawImage(glow, dest.Left - pad, dest.Top - pad, new SKSamplingOptions(SKFilterMode.Nearest), null);
#else
            canvas.DrawImage(glow, dest.Left - pad, dest.Top - pad);
#endif
        }

        /* The least recently used entry is disposed past ImageGlowCacheSize */
        private static SKImage GetImageGlow(SKImage image, int width, int height, float dpScale)
        {
            lock (_cacheLock)
            {
                int i;
                for (i = 0; i < _glowCache.Count; i++)
                {
                    CachedGlow entry = _glowCache[i];
                    if (ReferenceEquals(entry.Source, image) && entry.Width == width && entry.Height == height
                        && entry.Scale == dpScale)
                    {
                        if (i > 0)
                        {
                            _glowCache.RemoveAt(i);
                            _glowCache.Insert(0, entry);
                        }
                        return entry.Glow;
                    }
                }

                SKImage glow = RenderImageGlow(image, width, height, dpScale);
                if (glow == null)
                    return null;
                _glowCache.Insert(0, new CachedGlow { Source = image, Width = width, Height = height, Scale = dpScale, Glow = glow });
                while (_glowCache.Count > ImageGlowCacheSize)
                {
                    _glowCache[_glowCache.Count - 1].Glow.Dispose();
                    _glowCache.RemoveAt(_glowCache.Count - 1);
                }
                return glow;
            }
        }

        /* A tight bright glow merged with a wide warm one, from the image's alpha */
        private static SKImage RenderImageGlow(SKImage image, int width, int height, float dpScale)
        {
            int pad = (int)Math.Ceiling(ImageGlowExtent * dpScale);
            SKImageInfo info = new SKImageInfo(width + 2 * pad, height + 2 * pad, SKColorType.Rgba8888, SKAlphaType.Premul);
            using (SKSurface surface = SKSurface.Create(info))
            {
                if (surface == null)
                    return null;
                SKCanvas canvas = surface.Canvas;
                canvas.Clear(SKColors.Transparent);
                float outerSigma = ImageGlowOuterSigma * dpScale;
                float innerSigma = ImageGlowInnerSigma * dpScale;
                using (SKImageFilter outer = SKImageFilter.CreateDropShadowOnly(0, 0, outerSigma, outerSigma, ImageGlowOuterColor))
                using (SKImageFilter inner = SKImageFilter.CreateDropShadowOnly(0, 0, innerSigma, innerSigma, ImageGlowInnerColor))
                using (SKImageFilter merged = SKImageFilter.CreateMerge(new SKImageFilter[] { outer, inner }))
                using (SKPaint paint = new SKPaint())
                {
                    paint.ImageFilter = merged;
                    SKRect source = new SKRect(0, 0, image.Width, image.Height);
                    SKRect target = new SKRect(pad, pad, pad + width, pad + height);
#if GNH_MAUI
                    canvas.DrawImage(image, source, target, new SKSamplingOptions(SKFilterMode.Linear), paint);
#else
                    paint.FilterQuality = SKFilterQuality.High;
                    canvas.DrawImage(image, source, target, paint);
#endif
                }
                return surface.Snapshot();
            }
        }

        /* A soft glow outside rect and a crisp border just inside it; paint is a scratch paint whose MaskFilter is
           cleared afterwards */
        public static void DrawRowGlow(SKCanvas canvas, SKRect rect, float radius, float dpScale, SKColor glowColor,
            SKColor borderColor, SKPaint paint)
        {
            if (canvas == null || paint == null)
                return;
            paint.IsAntialias = true;
            paint.Shader = null;
            if (glowColor.Alpha > 0)
            {
                using (SKRoundRect rrect = new SKRoundRect(rect, radius, radius))
                {
                    paint.Style = SKPaintStyle.Fill;
                    paint.Color = glowColor;
                    paint.MaskFilter = GetRowGlowMask(RowGlowSigma * dpScale);
                    canvas.Save();
                    // WORKAROUND(WA-SKIASHARP-0011): SKBlurStyle.Outer paints inside a rounded rectangle
                    canvas.ClipRoundRect(rrect, SKClipOperation.Difference, true);
                    canvas.DrawRoundRect(rrect, paint);
                    canvas.Restore();
                    paint.MaskFilter = null;
                }
            }
            float stroke = RowBorderWidth * dpScale;
            SKRect borderRect = rect;
            borderRect.Inflate(-stroke / 2, -stroke / 2);
            paint.Style = SKPaintStyle.Stroke;
            paint.StrokeWidth = stroke;
            paint.Color = borderColor;
            canvas.DrawRoundRect(borderRect, radius, radius, paint);
        }

        /* A normal blur, recreated when sigma changes; the previous one is unused once paints release it */
        private static SKMaskFilter GetRowGlowMask(float sigma)
        {
            lock (_cacheLock)
            {
                if (_rowGlowMask == null || _rowGlowMaskSigma != sigma)
                {
                    if (_rowGlowMask != null)
                        _rowGlowMask.Dispose();
                    _rowGlowMask = SKMaskFilter.CreateBlur(SKBlurStyle.Normal, sigma);
                    _rowGlowMaskSigma = sigma;
                }
                return _rowGlowMask;
            }
        }
    }
}
