using SkiaSharp;
using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackX
{
    public class GHDrawCommand
    {
        public SKMatrix Matrix { get; set; }
        public SKRect SourceRect { get; set; }
        public SKRect DestinationRect { get; set; }
        public SKBitmap SourceBitmap { get; set; }
        public SKColor PaintColor { get; set; }
        public bool EndDarkening { get; set; }
        public int MapX { get; set; }
        public int MapY { get; set; }

        public GHDrawCommand()
        {

        }
        public GHDrawCommand(SKMatrix matrix, SKRect sourceRect, SKRect destinationRect, SKBitmap sourceBitmap, SKColor paintColor, int mapX, int mapY)
        {
            Matrix = matrix;
            SourceRect = sourceRect;
            DestinationRect = destinationRect;
            SourceBitmap = sourceBitmap;
            PaintColor = paintColor;
            MapX = mapX;
            MapY = mapY;
        }
        public GHDrawCommand(bool endDarkening)
        {
            EndDarkening = endDarkening;
        }
    }
}
