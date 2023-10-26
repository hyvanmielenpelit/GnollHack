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

        public GHDrawCommand() 
        { 

        }
        public GHDrawCommand(SKMatrix matrix, SKRect sourceRect, SKRect destinationRect, SKBitmap sourceBitmap, SKColor paintColor)
        {
            Matrix = matrix;
            SourceRect = sourceRect;
            DestinationRect = destinationRect;
            SourceBitmap = sourceBitmap;
            PaintColor = paintColor;
        }
    }
}
