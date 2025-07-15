using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;
#if GNH_MAUI
using GnollHackM;
#endif

namespace GnollHackX
{
    public interface IGnollHackService
    {
        Task InitializeGnollHack();
        Task InitializeSecrets(Secrets secrets);
        Task ResetDefaultsFile();

        void LoadLibrary();
        void UnloadLibrary();
        string GetGnollHackPath();
        void ClearFiles();
        void ClearCoreFiles();
        void ClearTopScores();
        void ClearSavedGames();
        void ClearDumplogs();
        void ClearSnapshots();
        void ClearBones();
        void ClearAllFilesInMainDirectory();
        void InitializeTileData();
        void GetGlyphArrays(out IntPtr gl2ti, out int size1, out IntPtr gltifl, out int gltifl_size);
        void GetTileArrays(out IntPtr ti2an, out int size2, out IntPtr ti2en, out int size3, out IntPtr ti2ad, out int size4, out IntPtr anoff_ptr, out int anoff_size, out IntPtr enoff_ptr, out int enoff_size, out IntPtr reoff_ptr, out int reoff_size);
        int GetTotalTiles();
        void SetGlyphArrays(IntPtr ptr_gl2ti, int size_gl2ti, IntPtr ptr_gltifl, int size_gltifl);
        void SetTile2AnimationArray(IntPtr ptr_ti2an, int size_ti2an);
        int GetUnexploredGlyph();
        int GetNoGlyph();
        void GetOffs(out int a, out int e, out int r, out int gen_tile, out int hit_tile, out int ui_tile, out int spell_tile, out int skill_tile, out int command_tile, out int buff_tile, out int cursor_off);
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
        ulong GetVersionNumber();
        ulong GetVersionCompatibility();
        string GetPropertyName(int prop_index);
        List<string> GetExtendedCommands();
        string DumplogDateString(long startdate);
        void SetPetMID(uint m_id);
        int Chmod(string filename, uint mode);

        int StartGnollHack(GHGame ghGame);

        void SaveAndRestoreSavedGame(int save_style);
        void TallyRealTime();
        bool ValidateSaveFile(string filename, out string out_str);
        int CheckCurrentFileDescriptor();
        void ReportFileDescriptors();
        bool IncreaseFileDescriptorLimitToAtLeast(int min_cur);
        int GetFileDescriptorLimit(bool is_max_limit);
        int GetMaxManuals();
        int GetFirstCatalogue();
        int GetNumCatalogues();
        int GetMaxMajorConsultations();

        bool IsDebug();

        bool GetCharacterClickAction();
        void SetCharacterClickAction(bool newValue);
        bool GetGetPositionArrows();
        void SetGetPositionArrows(bool newValue);
        void SetDiceAsRanges(bool newValue);
        void SetAutoDig(bool newValue);
        void SetIgnoreStopping(bool newValue);
        int GetMouseCommand(bool isMiddle);
        void SetMouseCommand(int newValue, bool isMiddle);

        string GetEventPathForGHSound(int ghsound);
        float GetVolumeForGHSound(int ghsound);
    }
}
