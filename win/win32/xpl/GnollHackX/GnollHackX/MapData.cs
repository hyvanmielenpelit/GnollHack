using System;
using System.Collections.Generic;
using System.Text;
using SkiaSharp;
#if GNH_MAUI
using GnollHackM;
#endif

namespace GnollHackX
{
    public struct MapData
    {
        public int Glyph;
        public int BkGlyph;
        public string Symbol;
        public SKColor Color;
        public uint Special;
        public LayerInfo Layers;
        public long GlyphPrintAnimationCounterValue;
        public long GlyphPrintMainCounterValue;
        public long GlyphObjectPrintAnimationCounterValue;
        public long GlyphObjectPrintMainCounterValue;
        public long GlyphGeneralPrintAnimationCounterValue;
        public long GlyphGeneralPrintMainCounterValue;
        public bool NeedsUpdate;
        public bool MapAnimated;
        public bool RedrawTile;
        public bool HasEnlargementOrAnimationOrSpecialHeight;
        public EngravingInfo Engraving;
    }

}
