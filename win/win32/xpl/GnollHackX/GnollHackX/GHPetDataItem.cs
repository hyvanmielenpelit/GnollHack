﻿using SkiaSharp;
#if GNH_MAUI
using GnollHackM;
#endif

namespace GnollHackX
{
    public class GHPetDataItem
    {
        public readonly monst_info Data;

        public GHPetDataItem(monst_info data)
        {
            Data = data;
        }
    }
}
