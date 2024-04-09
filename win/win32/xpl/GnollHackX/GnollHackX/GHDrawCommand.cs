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
        public ulong contents_no;
        public ulong contents_id_sum;
        public int item_charges;
        public int item_special_quality;
        public bool item_lit;

        public AutoDrawParameterDefinition(int autodrawInit, ObjectDataItem otmp_roundInit,
            int layer_idxInit, bool tileflag_halfsizeInit, bool tileflag_normalobjmissileInit, bool tileflag_fullsizeditemInit,
            float txInit, float tyInit, float widthInit, float heightInit,
            float scaleInit, float targetscaleInit, float scaled_x_paddingInit, float scaled_y_paddingInit, float scaled_tile_heightInit,
            bool is_inventoryInit, bool drawwallendsInit, ulong contents_noInit, ulong contents_id_sumInit, int item_chargesInit, int item_special_qualityInit, bool item_litInit)
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
            contents_no = contents_noInit;
            contents_id_sum = contents_id_sumInit;
            item_charges = item_chargesInit;
            item_special_quality = item_special_qualityInit;
            item_lit = item_litInit;
        }
    }

    public class GHDrawCommand
    {
        public SKMatrix Matrix { get; set; }
        public SKRect SourceRect { get; set; }
        public SKRect DestinationRect { get; set; }
        public SKImage SourceBitmap { get; set; }
        public SKColor PaintColor { get; set; }
        public bool EndDarkening { get; set; }
        public int MapX { get; set; }
        public int MapY { get; set; }
        public bool IsAutoDraw { get; private set; }
        public AutoDrawParameterDefinition AutoDrawParameters { get; private set; }

        public GHDrawCommand()
        {

        }
        public GHDrawCommand(SKMatrix matrix, SKRect sourceRect, SKRect destinationRect, SKImage sourceBitmap, SKColor paintColor, int mapX, int mapY)
        {
            Matrix = matrix;
            SourceRect = sourceRect;
            DestinationRect = destinationRect;
            SourceBitmap = sourceBitmap;
            PaintColor = paintColor;
            MapX = mapX;
            MapY = mapY;
        }
        public GHDrawCommand(bool endDarkening)
        {
            EndDarkening = endDarkening;
        }
        public GHDrawCommand(SKMatrix matrix, SKColor paintColor, int mapX, int mapY, AutoDrawParameterDefinition parameters)
        {
            Matrix = matrix;
            MapX = mapX;
            MapY = mapY;
            PaintColor = paintColor;
            AutoDrawParameters = parameters;
            IsAutoDraw = true;
        }
    }
}
