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
        void GetTileArrays(out IntPtr gl2ti, out int size1, out IntPtr ti2an, out int size2, out IntPtr ti2en, out int size3, out IntPtr ti2re, out int size4);
        int GetTotalTiles();
        int GetUnexploredGlyph();
        int GetNoGlyph();
        List<AnimationDefinition> GetAnimationArray();
        List<EnlargementDefinition> GetEnlargementArray();
        List<ReplacementDefinition> GetReplacementArray();
        List<AutoDrawDefinition> GetAutoDrawArray();
        int Test1();
        int Test2();
        int StartGnollHack(ClientGame clientGame);
    }
}
