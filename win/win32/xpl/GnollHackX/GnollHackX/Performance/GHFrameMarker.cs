using System;
using SkiaSharp;

namespace GnollHackX.Performance
{
    /* A developer overlay that makes the displayed frame identifiable on film. Drawn last on
       the map canvas while the frame timeline records:
         - a strip of eight cells showing FrameId mod 256 in Gray code (one cell changes per
           frame), between two always-white reference cells, in the top-left corner;
         - a block crossing the top edge in two seconds of content time, advanced by the main
           counter, so uneven motion on screen shows up directly as uneven block steps.
       A slow-motion video of the screen then yields, per video frame, which FrameId was on
       screen and where the block was. Allocation-free. */
    public static class GHFrameMarker
    {
        public const int Bits = 8;

        private static readonly SKPaint _white = new SKPaint { Color = SKColors.White, Style = SKPaintStyle.Fill, IsAntialias = false };
        private static readonly SKPaint _black = new SKPaint { Color = SKColors.Black, Style = SKPaintStyle.Fill, IsAntialias = false };

        public static void Draw(SKCanvas canvas, float width, float height, long frameId, long mainCounter,
                                int targetFps, float density)
        {
            if (canvas == null || width <= 0 || height <= 0)
                return;

            float cell = Math.Max(8f, 10f * density);

            /* Motion block along the top edge */
            int fps = targetFps > 0 ? targetFps : 60;
            long cycle = 2L * fps;
            long phase = mainCounter % cycle;
            if (phase < 0)
                phase += cycle;
            float travel = Math.Max(0f, width - cell);
            float bx = (float)phase / cycle * travel;
            canvas.DrawRect(0, 0, width, cell, _black);
            canvas.DrawRect(bx, 0, cell, cell, _white);

            /* Gray-coded frame id below it, on a black backing */
            float x0 = cell;
            float y0 = cell * 1.5f;
            float margin = cell / 4;
            canvas.DrawRect(x0 - margin, y0 - margin, (Bits + 2) * cell + 2 * margin, cell + 2 * margin, _black);

            uint value = (uint)(frameId & 0xFF);
            uint gray = value ^ (value >> 1);
            canvas.DrawRect(x0, y0, cell, cell, _white);
            for (int b = 0; b < Bits; b++)
            {
                bool on = ((gray >> (Bits - 1 - b)) & 1u) != 0;
                canvas.DrawRect(x0 + (b + 1) * cell, y0, cell, cell, on ? _white : _black);
            }
            canvas.DrawRect(x0 + (Bits + 1) * cell, y0, cell, cell, _white);
        }
    }
}
