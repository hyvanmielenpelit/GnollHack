using GnollHackCommon;
using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackClient
{
    public interface IGnollHackService
    {
        void LoadLibrary();
        void UnloadLibrary();
        void InitializeGnollHack();
        void ClearFiles();
        void GetTileArrays(out IntPtr gl2ti, out int size1, out IntPtr gltifl, out int gltifl_size, out IntPtr ti2an, out int size2, out IntPtr ti2en, out int size3, out IntPtr ti2ad, out int size4, out IntPtr anoff_ptr, out int anoff_size, out IntPtr enoff_ptr, out int enoff_size, out IntPtr reoff_ptr, out int reoff_size);
        int GetTotalTiles();
        int GetUnexploredGlyph();
        int GetNoGlyph();
        void GetOffs(out int a, out int e, out int r, out int gen_tile, out int hit_tile, out int ui_tile, out int buff_tile);
        List<AnimationDefinition> GetAnimationArray();
        List<EnlargementDefinition> GetEnlargementArray();
        List<ReplacementDefinition> GetReplacementArray();
        List<AutoDrawDefinition> GetAutoDrawArray();
        int GetTileAnimationIndexFromGlyph(int glyph);
        bool GlyphIsExplosion(int glyph);
        bool GlyphIsZap(int glyph);
        bool GlyphIsAnyDying(int glyph);
        int GetAnimatedTile(int ntile, int tile_animation_idx, int play_type, long interval_counter,
            out int frame_idx_ptr, out int main_tile_idx_ptr, out sbyte mapAnimated, out int autodraw_ptr);

        int Test1();
        int Test2();
        int StartGnollHack(ClientGame clientGame);
    }
}
