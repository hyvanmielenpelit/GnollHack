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
        public int SheetIdx;
        public int MapX;
        public int MapY;
        /* Grouping the deferred lists by sheet must not reorder across layers */
        public int LayerIdx;
        public bool IsAutoDraw;
        public AutoDrawParameterDefinition AutoDrawParameters;

        public GHDrawCommand(SKMatrix matrix, SKRect sourceRect, SKRect destinationRect, SKImage sourceBitmap, SKColor paintColor, SKColorFilter paintColorFilter, int sheetIdx, int mapX, int mapY, int layerIdx)
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
            LayerIdx = layerIdx;
#if !GNH_MAUI
            IsAutoDraw = false;
            AutoDrawParameters = new AutoDrawParameterDefinition();
#endif
        }
        /* sheetIdx is the sheet of the tile being decorated: it is only a grouping key,
           the components themselves select their own sheets as they are drawn. */
        public GHDrawCommand(SKMatrix matrix, SKColor paintColor, SKColorFilter paintColorFilter, int sheetIdx, int mapX, int mapY, AutoDrawParameterDefinition parameters)
        {
            Matrix = matrix;
            SheetIdx = sheetIdx;
            MapX = mapX;
            MapY = mapY;
            LayerIdx = parameters.layer_idx;
            PaintColor = paintColor;
            PaintColorFilter = paintColorFilter;
            AutoDrawParameters = parameters;
            IsAutoDraw = true;

#if !GNH_MAUI
            SourceRect = new SKRect();
            DestinationRect = new SKRect();
            SourceBitmap = null;
#endif
        }
    }
}
