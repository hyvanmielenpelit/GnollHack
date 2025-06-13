using SkiaSharp;
#if GNH_MAUI
using GnollHackM;
#endif

namespace GnollHackX
{
    public class GHPetDataItem
    {
        public monst_info Data { get; private set; }

        public GHPetDataItem(monst_info data)
        {
            Data = data;
        }
    }
}
