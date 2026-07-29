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
        public int CodePoint;
        public string Symbol;
        public SKColor Color;
        public UInt64 Special;
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

        /* Static cache of pre-created strings for ASCII code points (0-127).
         * Covers all standard roguelike map symbols with zero runtime allocations. */
        private static readonly string[] _asciiSymbolCache = InitAsciiSymbolCache();

        private static string[] InitAsciiSymbolCache()
        {
            string[] cache = new string[128];
            for (int i = 0; i < 128; i++)
                cache[i] = Char.ConvertFromUtf32(i);
            return cache;
        }

        public static string CodePointToSymbol(int codePoint)
        {
            if (codePoint > 0 && codePoint < 128)
                return _asciiSymbolCache[codePoint];
            if (codePoint > 0)
                return Char.ConvertFromUtf32(codePoint);
            return null;
        }
    }

}
