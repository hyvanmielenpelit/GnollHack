using System;
using System.IO;
using SkiaSharp;

namespace GnollHackM
{
    /* Converts a Windows .cur file into an Avalonia cursor */
    internal static class GHAvaloniaCursors
    {
        private const int IconDirSize = 6;
        private const int IconDirEntrySize = 16;
        private const int BitmapInfoHeaderSize = 40;

        /* Returns null if the cursor cannot be read */
        public static Avalonia.Input.Cursor FromCurStream(Stream stream)
        {
            if (stream == null)
            {
                GnollHackX.GHApp.MaybeWriteGHLog("Avalonia cursor: resource stream not found");
                return null;
            }

            try
            {
                byte[] data;
                using (MemoryStream ms = new MemoryStream())
                {
                    stream.CopyTo(ms);
                    data = ms.ToArray();
                }

                /* ICONDIR: reserved, type (2 = cursor), count; then the first ICONDIRENTRY */
                if (data.Length < IconDirSize + IconDirEntrySize || ReadUInt16(data, 2) != 2 || ReadUInt16(data, 4) < 1)
                {
                    GnollHackX.GHApp.MaybeWriteGHLog("Avalonia cursor: not a cursor file");
                    return null;
                }

                int width = data[IconDirSize] == 0 ? 256 : data[IconDirSize];
                int height = data[IconDirSize + 1] == 0 ? 256 : data[IconDirSize + 1];
                int hotX = ReadUInt16(data, IconDirSize + 4);
                int hotY = ReadUInt16(data, IconDirSize + 6);
                long imageSize = ReadUInt32(data, IconDirSize + 8);
                long imageOffset = ReadUInt32(data, IconDirSize + 12);
                if (imageSize <= 0 || imageOffset < IconDirSize + IconDirEntrySize || imageOffset + imageSize > data.Length)
                {
                    GnollHackX.GHApp.MaybeWriteGHLog("Avalonia cursor: invalid image entry");
                    return null;
                }

                using (SKBitmap bitmap = DecodeWithSkia(data, width, height) ?? DecodeDib(data, (int)imageOffset, (int)imageSize))
                {
                    if (bitmap == null)
                    {
                        GnollHackX.GHApp.MaybeWriteGHLog("Avalonia cursor: image could not be decoded");
                        return null;
                    }

                    hotX = Math.Max(0, Math.Min(hotX, bitmap.Width - 1));
                    hotY = Math.Max(0, Math.Min(hotY, bitmap.Height - 1));

                    using (SKImage image = SKImage.FromBitmap(bitmap))
                    using (SKData png = image?.Encode(SKEncodedImageFormat.Png, 100))
                    {
                        if (png == null)
                        {
                            GnollHackX.GHApp.MaybeWriteGHLog("Avalonia cursor: PNG encoding failed");
                            return null;
                        }

                        using (MemoryStream pngStream = new MemoryStream(png.ToArray()))
                        {
                            /* Not disposed, as the platform cursor may still refer to the bitmap */
                            Avalonia.Media.Imaging.Bitmap avaloniaBitmap = new Avalonia.Media.Imaging.Bitmap(pngStream);
                            return new Avalonia.Input.Cursor(avaloniaBitmap, new Avalonia.PixelPoint(hotX, hotY));
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                GnollHackX.GHApp.MaybeWriteGHLog("Avalonia cursor: " + ex.Message);
                return null;
            }
        }

        /* Decodes the image as an .ico (type field 1) with Skia; returns null unless the result has the expected size and real alpha */
        private static SKBitmap DecodeWithSkia(byte[] data, int width, int height)
        {
            SKBitmap bitmap = null;
            try
            {
                byte[] icoData = (byte[])data.Clone();
                icoData[2] = 1;
                icoData[3] = 0;
                bitmap = SKBitmap.Decode(icoData);
                if (bitmap != null && bitmap.Width == width && bitmap.Height == height && HasTransparentAndVisiblePixels(bitmap))
                    return bitmap;
            }
            catch (Exception ex)
            {
                GnollHackX.GHApp.MaybeWriteGHLog("Avalonia cursor: Skia decoding failed: " + ex.Message);
            }

            bitmap?.Dispose();
            return null;
        }

        private static bool HasTransparentAndVisiblePixels(SKBitmap bitmap)
        {
            bool hasTransparent = false;
            bool hasVisible = false;
            int x, y;
            for (y = 0; y < bitmap.Height; y++)
            {
                for (x = 0; x < bitmap.Width; x++)
                {
                    if (bitmap.GetPixel(x, y).Alpha == 0)
                        hasTransparent = true;
                    else
                        hasVisible = true;

                    if (hasTransparent && hasVisible)
                        return true;
                }
            }
            return false;
        }

        /* Decodes a 32-bit BI_RGB DIB: BGRA rows (bottom-up when the height is positive), height doubled for the AND mask;
           the AND mask supplies the alpha only when every alpha byte is zero */
        private static SKBitmap DecodeDib(byte[] data, int offset, int size)
        {
            if (size < BitmapInfoHeaderSize)
                return null;

            int headerSize = (int)ReadUInt32(data, offset);
            int dibWidth = ReadInt32(data, offset + 4);
            int dibHeight = ReadInt32(data, offset + 8);
            int bitCount = ReadUInt16(data, offset + 14);
            uint compression = ReadUInt32(data, offset + 16);
            if (headerSize < BitmapInfoHeaderSize || headerSize > size || bitCount != 32 || compression != 0 || dibWidth <= 0 || dibHeight == 0)
                return null;

            bool bottomUp = dibHeight > 0;
            int w = dibWidth;
            int h = Math.Abs(dibHeight) / 2;
            if (h <= 0)
                return null;

            int end = offset + size;
            int xorOffset = offset + headerSize;
            int xorStride = w * 4;
            long xorEnd = (long)xorOffset + (long)xorStride * h;
            if (xorEnd > end)
                return null;

            int maskOffset = (int)xorEnd;
            int maskStride = ((w + 31) / 32) * 4;
            bool hasMask = (long)maskOffset + (long)maskStride * h <= end;

            bool allAlphaZero = true;
            int i;
            for (i = xorOffset + 3; i < xorEnd; i += 4)
            {
                if (data[i] != 0)
                {
                    allAlphaZero = false;
                    break;
                }
            }

            SKBitmap bitmap = new SKBitmap(w, h, SKColorType.Bgra8888, SKAlphaType.Unpremul);
            int x, y;
            for (y = 0; y < h; y++)
            {
                int srcRow = bottomUp ? h - 1 - y : y;
                int rowOffset = xorOffset + srcRow * xorStride;
                int maskRowOffset = maskOffset + srcRow * maskStride;
                for (x = 0; x < w; x++)
                {
                    int p = rowOffset + x * 4;
                    byte b = data[p];
                    byte g = data[p + 1];
                    byte r = data[p + 2];
                    byte a = data[p + 3];
                    if (allAlphaZero)
                    {
                        bool masked = hasMask && (data[maskRowOffset + (x >> 3)] & (0x80 >> (x & 7))) != 0;
                        a = masked ? (byte)0 : (byte)255;
                    }
                    bitmap.SetPixel(x, y, new SKColor(r, g, b, a));
                }
            }
            return bitmap;
        }

        private static ushort ReadUInt16(byte[] data, int offset)
        {
            return (ushort)(data[offset] | (data[offset + 1] << 8));
        }

        private static uint ReadUInt32(byte[] data, int offset)
        {
            return (uint)(data[offset] | (data[offset + 1] << 8) | (data[offset + 2] << 16) | (data[offset + 3] << 24));
        }

        private static int ReadInt32(byte[] data, int offset)
        {
            return data[offset] | (data[offset + 1] << 8) | (data[offset + 2] << 16) | (data[offset + 3] << 24);
        }
    }
}
