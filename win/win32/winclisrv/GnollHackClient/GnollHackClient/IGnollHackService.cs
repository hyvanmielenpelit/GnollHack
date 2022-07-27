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
        void InitializeGnollHack(Secrets secrets);
        string GetGnollHackPath();
        void ClearFiles();
        void ClearCoreFiles();
        void ClearTopScores();
        void ClearSavedGames();
        void ClearDumplogs();
        void ClearAllFilesInMainDirectory();
        void ResetDefaultsFile();
        void GetGlyphArrays(out IntPtr gl2ti, out int size1, out IntPtr gltifl, out int gltifl_size);
        void GetTileArrays(out IntPtr ti2an, out int size2, out IntPtr ti2en, out int size3, out IntPtr ti2ad, out int size4, out IntPtr anoff_ptr, out int anoff_size, out IntPtr enoff_ptr, out int enoff_size, out IntPtr reoff_ptr, out int reoff_size);
        int GetTotalTiles();
        int GetUnexploredGlyph();
        int GetNoGlyph();
        void GetOffs(out int a, out int e, out int r, out int gen_tile, out int hit_tile, out int ui_tile, out int spell_tile, out int skill_tile, out int buff_tile, out int cursor_off);
        List<AnimationDefinition> GetAnimationArray();
        List<EnlargementDefinition> GetEnlargementArray();
        List<ReplacementDefinition> GetReplacementArray();
        List<AutoDrawDefinition> GetAutoDrawArray();
        int GetTileAnimationIndexFromGlyph(int glyph);
        bool GlyphIsExplosion(int glyph);
        bool GlyphIsZap(int glyph);
        bool GlyphIsAnyDying(int glyph);
        int GetAnimatedTile(int ntile, int tile_animation_idx, int play_type, long interval_counter,
            out int frame_idx_ptr, out int main_tile_idx_ptr, out sbyte mapAnimated, ref int autodraw_ptr);

        int ZapGlyphToCornerGlyph(int adjglyph, ulong adjflags, int source_dir);
        int Test();

        string GetVersionString();
        string GetVersionId();
        string GetPropertyName(int prop_index);
        List<string> GetExtendedCommands();
        string DumplogDateString(long startdate);
        void SwitchDemoVersion(bool active);
        void SetPetMID(uint m_id);
        int Chmod(string filename, uint mode);

        int StartGnollHack(ClientGame clientGame);

        void SaveAndRestoreSavedGame();
    }
}
