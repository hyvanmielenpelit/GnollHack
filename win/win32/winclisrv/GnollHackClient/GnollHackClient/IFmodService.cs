using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackClient
{
    public interface IFmodService
    {
        void InitializeFmod();
        string GetBankDir();
        void LoadBanks();
        void PlayTestSound();
        void StopTestSound();
        int PlayImmediateSound(int ghsound, string eventPath, int bankid, float eventVolume, float soundVolume, string[] parameterNames, float[] parameterValues, int arraysize, int sound_type, int play_group, uint dialogue_mid);
        int PlayMusic(int ghsound, string eventPath, int bankid, float eventVolume, float soundVolume);
        int PlayLevelAmbient(int ghsound, string eventPath, int bankid, float eventVolume, float soundVolume);
        int PlayEnvironmentAmbient(int ghsound, string eventPath, int bankid, float eventVolume, float soundVolume);
        int AddAmbientSound(int ghsound, string eventPath, int bankid, float eventVolume, float soundVolume, out UInt64 soundSourceId);
        int DeleteAmbientSound(UInt64 soundSourceId);
        int SetAmbientSoundVolume(UInt64 soundSourceId, float soundVolume);
        int StopAllSounds(ulong flags, uint dialogue_mid);
        int AdjustVolumes(float new_general_volume, float new_general_music_volume, float new_general_ambient_volume, float new_general_dialogue_volume, float new_general_sfx_volume, float new_general_ui_volume);

    }
}
