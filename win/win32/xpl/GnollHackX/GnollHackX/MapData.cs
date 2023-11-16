using System;
using System.Collections.Generic;
using System.Text;
using SkiaSharp;

namespace GnollHackX
{
    public struct MapData
    {
        public int Glyph { get; set; }
        public int BkGlyph { get; set; }
        public string Symbol { get; set; }
        public SKColor Color { get; set; }
        public uint Special { get; set; }
        public LayerInfo Layers;
        public long GlyphPrintAnimationCounterValue { get; set; }
        public long GlyphPrintMainCounterValue { get; set; }
        public long GlyphObjectPrintAnimationCounterValue { get; set; }
        public long GlyphObjectPrintMainCounterValue { get; set; }
        public long GlyphGeneralPrintAnimationCounterValue { get; set; }
        public long GlyphGeneralPrintMainCounterValue { get; set; }
        public bool NeedsUpdate { get; set; }
        public bool MapAnimated { get; set; }
        public bool RedrawTile { get; set; }
        public bool HasEnlargementOrAnimationOrSpecialHeight { get; set; }
        public EngravingInfo Engraving;
    }

}
