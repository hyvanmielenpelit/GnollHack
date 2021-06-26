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
        void GetTileArrays(out IntPtr gl2ti, out int size);
        int GetTotalTiles();
        int GetUnexploredGlyph();
        int GetNoGlyph();
        int Test1();
        int Test2();
        int TestRunGnollHack();
        int StartGnollHack(ClientGame clientGame);
    }
}
