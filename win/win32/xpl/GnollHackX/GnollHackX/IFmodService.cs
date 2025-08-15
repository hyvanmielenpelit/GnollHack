using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;
#if GNH_MAUI
namespace GnollHackM
#else
namespace GnollHackX
#endif
{
    public interface IFmodService
    {
        void InitializeFmod();
        void ShutdownFmod();
        void ReleaseAllGameSoundInstances(bool doUpdate = true);
        void ReleaseAllUISoundInstances(bool doUpdate = true);
        void ClearLoadableSoundBanks();
        Task AddLoadableSoundBank(string fullFilePath, int subType, bool isResource, bool readToMemory);
        void LoadBanks(sound_bank_loading_type loadingType);
        void UnloadBanks(sound_bank_loading_type loadingType);
        void LoadIntroSoundBank();
        void UnloadIntroSoundBank();
        void PlayTestSound();
        void StopTestSound();
        int PlayUISound(int ghsound, string eventPath, int bankid, float eventVolume, float soundVolume);
        int PlayImmediateSound(int ghsound, string eventPath, int bankid, float eventVolume, float soundVolume, string[] parameterNames, float[] parameterValues, int arraysize, int sound_type, int play_group, uint dialogue_mid, uint play_flags);
        int PlayMusic(int ghsound, string eventPath, int bankid, float eventVolume, float soundVolume);
        int PlayUIMusic(int ghsound, string eventPath, int bankid, float eventVolume, float soundVolume);
        int PlayLevelAmbient(int ghsound, string eventPath, int bankid, float eventVolume, float soundVolume);
        int PlayEnvironmentAmbient(int ghsound, string eventPath, int bankid, float eventVolume, float soundVolume);
        int PlayOccupationAmbient(int ghsound, string eventPath, int bankid, float eventVolume, float soundVolume);
        int PlayEffectAmbient(int ghsound, string eventPath, int bankid, float eventVolume, float soundVolume);
        int SetEffectAmbientVolume(float soundVolume);
        int AddAmbientSound(int ghsound, string eventPath, int bankid, float eventVolume, float soundVolume, out UInt64 soundSourceId);
        int DeleteAmbientSound(UInt64 soundSourceId);
        int SetAmbientSoundVolume(UInt64 soundSourceId, float soundVolume);
        int StopAllGameSounds(ulong flags, uint dialogue_mid);
        void StopAllUISounds();
        int AdjustGameVolumes(float new_general_volume, float new_general_music_volume, float new_general_ambient_volume, float new_general_dialogue_volume, float new_general_sfx_volume, float new_general_game_ui_volume);
        int AdjustUIVolumes(float new_general_volume, float new_general_music_volume, float new_general_ui_volume);
        void ToggleMuteSounds(bool mute);
        int SetQuieterMode(bool state);
        void ResetGameState();
        void PollTasks();
        uint GetVersionCode();
        string GetVersionString();
    }
}
