using SkiaSharp;

namespace GnollHackX
{
    public class GHPetDataItem
    {
        public monst_info Data { get; set; }
        public SKRect Rect { get; set; }

        public GHPetDataItem(monst_info data)
        {
            Data = data;
        }
    }
}
