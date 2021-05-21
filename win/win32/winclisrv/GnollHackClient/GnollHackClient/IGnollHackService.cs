using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackClient
{
    public interface IGnollHackService
    {
        void InitializeGnollHack();
        int Test1();
        int Test2();
        int TestRunGnollHack();
        int StartGnollHack(ClientGame clientGame);
    }
}
