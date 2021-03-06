﻿using System;
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
        int PlayImmediateSound(int ghsound, string eventPath, int bankid, float eventVolume, float soundVolume, string[] parameterNames, float[] parameterValues, int arraysize, int sound_type, int play_group, uint dialogue_mid);
        int PlayMusic(int ghsound, string eventPath, int bankid, float eventVolume, float soundVolume);
        int StopAllSounds(ulong flags, uint dialogue_mid);
    }
}
