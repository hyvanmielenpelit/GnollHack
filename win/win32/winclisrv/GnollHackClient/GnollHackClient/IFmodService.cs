using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackClient
{
    public interface IFmodService
    {
        void InitializeFmod();
        void LoadBanks();
        void PlayTestSound();
        void StopTestSound();
    }
}
