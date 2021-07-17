using System;
using System.Collections.Generic;
using System.Text;
using SkiaSharp;
using GnollHackCommon;

namespace GnollHackClient
{
    public struct MapData
    {
        public int Glyph { get; set; }
        public int BkGlyph { get; set; }
        public string Symbol { get; set; }
        public SKColor Color { get; set; }
        public uint Special { get; set; }
        public LayerInfo Layers;
        public long GlyphPrintCounterValue { get; set; }
    }

}
