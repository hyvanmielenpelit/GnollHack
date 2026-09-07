using SkiaSharp;
using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackX
{
    public struct AutoDrawParameterDefinition
    {
        public int autodraw;
        public ObjectDataItem otmp_round;
        public int layer_idx; 
        public bool tileflag_halfsize; 
        public bool tileflag_normalobjmissile; 
        public bool tileflag_fullsizeditem;
        public float tx; 
        public float ty; 
        public float width; 
        public float height;
        public float scale; 
        public float targetscale; 
        public float scaled_x_padding; 
        public float scaled_y_padding; 
        public float scaled_tile_height;
        public bool is_inventory; 
        public bool drawwallends;

        public AutoDrawParameterDefinition(int autodrawInit, ObjectDataItem otmp_roundInit,
            int layer_idxInit, bool tileflag_halfsizeInit, bool tileflag_normalobjmissileInit, bool tileflag_fullsizeditemInit,
            float txInit, float tyInit, float widthInit, float heightInit,
            float scaleInit, float targetscaleInit, float scaled_x_paddingInit, float scaled_y_paddingInit, float scaled_tile_heightInit,
            bool is_inventoryInit, bool drawwallendsInit)
        {
            autodraw = autodrawInit;
            otmp_round = otmp_roundInit;
            layer_idx = layer_idxInit;
            tileflag_halfsize = tileflag_halfsizeInit;
            tileflag_fullsizeditem = tileflag_fullsizeditemInit;
            tileflag_normalobjmissile = tileflag_normalobjmissileInit;
            tx = txInit;
            ty = tyInit;
            width = widthInit;
            height = heightInit;
            scale = scaleInit;
            targetscale = targetscaleInit;
            scaled_tile_height = scaled_tile_heightInit;
            scaled_x_padding = scaled_x_paddingInit;
            scaled_y_padding = scaled_y_paddingInit;
            is_inventory = is_inventoryInit;
            drawwallends = drawwallendsInit;
        }
    }

    public struct GHDrawCommand
    {
        public SKMatrix Matrix;
        public SKRect SourceRect;
        public SKRect DestinationRect;
        public SKImage SourceBitmap;
        public SKColor PaintColor;
        public SKColorFilter PaintColorFilter;
        public bool EndDarkening;
        public int SheetIdx;
        public int MapX;
        public int MapY;
        public bool IsAutoDraw { get; private set; }
        public AutoDrawParameterDefinition AutoDrawParameters { get; private set; }

        public GHDrawCommand(SKMatrix matrix, SKRect sourceRect, SKRect destinationRect, SKImage sourceBitmap, SKColor paintColor, SKColorFilter paintColorFilter, int sheetIdx, int mapX, int mapY)
        {
            Matrix = matrix;
            SourceRect = sourceRect;
            DestinationRect = destinationRect;
            SourceBitmap = sourceBitmap;
            PaintColor = paintColor;
            PaintColorFilter = paintColorFilter;
            SheetIdx = sheetIdx;
            MapX = mapX;
            MapY = mapY;
#if !GNH_MAUI
            EndDarkening = false;
            IsAutoDraw = false;
            AutoDrawParameters = new AutoDrawParameterDefinition();
#endif
        }
        public GHDrawCommand(bool endDarkening)
        {
            EndDarkening = endDarkening;

#if !GNH_MAUI
            Matrix = new SKMatrix();
            SourceRect = new SKRect();
            DestinationRect = new SKRect();
            SourceBitmap = null;
            PaintColor = SKColors.Black;
            PaintColorFilter = null;
            SheetIdx = 0;
            MapX = 0;
            MapY = 0;
            IsAutoDraw = false;
            AutoDrawParameters = new AutoDrawParameterDefinition();
#endif
        }
        public GHDrawCommand(SKMatrix matrix, SKColor paintColor, SKColorFilter paintColorFilter, int mapX, int mapY, AutoDrawParameterDefinition parameters)
        {
            Matrix = matrix;
            SheetIdx = 0;
            MapX = mapX;
            MapY = mapY;
            PaintColor = paintColor;
            PaintColorFilter = paintColorFilter;
            AutoDrawParameters = parameters;
            IsAutoDraw = true;

#if !GNH_MAUI
            EndDarkening = false;
            SourceRect = new SKRect();
            DestinationRect = new SKRect();
            SourceBitmap = null;
#endif
        }
    }
}
