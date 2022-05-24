using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
//using FMOD;
//using FMOD.Studio;
using System.Reflection;
using System.IO;
using Xamarin.Forms;
using GnollHackCommon;
using System.Threading.Tasks;
using System.Threading;

using Foundation;
using UIKit;

[assembly: Dependency(typeof(GnollHackClient.iOS.FmodService))]
namespace GnollHackClient.iOS
{
    class FmodService : IFmodService
    {
        public int AddAmbientSound(int ghsound, string eventPath, int bankid, float eventVolume, float soundVolume, out ulong soundSourceId)
        {
            //throw new NotImplementedException();
            soundSourceId = 0;
            return 0;
        }

        public void AddLoadableSoundBank(string fullfilepath)
        {
            //throw new NotImplementedException();
        }

        public int AdjustVolumes(float new_general_volume, float new_general_music_volume, float new_general_ambient_volume, float new_general_dialogue_volume, float new_general_sfx_volume, float new_general_ui_volume)
        {
            //throw new NotImplementedException();
            return 0;
        }

        public void ClearLoadableSoundBanks()
        {
            //throw new NotImplementedException();
        }

        public int DeleteAmbientSound(ulong soundSourceId)
        {
            //throw new NotImplementedException();
            return 0;
        }

        public void InitializeFmod()
        {
            //throw new NotImplementedException();
        }

        public void LoadBanks()
        {
            //throw new NotImplementedException();
        }

        public int PlayEffectAmbient(int ghsound, string eventPath, int bankid, float eventVolume, float soundVolume)
        {
            //throw new NotImplementedException();
            return 0;
        }

        public int PlayEnvironmentAmbient(int ghsound, string eventPath, int bankid, float eventVolume, float soundVolume)
        {
            //throw new NotImplementedException();
            return 0;
        }

        public int PlayImmediateSound(int ghsound, string eventPath, int bankid, float eventVolume, float soundVolume, string[] parameterNames, float[] parameterValues, int arraysize, int sound_type, int play_group, uint dialogue_mid, ulong play_flags)
        {
            //throw new NotImplementedException();
            return 0;
        }

        public int PlayLevelAmbient(int ghsound, string eventPath, int bankid, float eventVolume, float soundVolume)
        {
            //throw new NotImplementedException();
            return 0;
        }

        public int PlayMusic(int ghsound, string eventPath, int bankid, float eventVolume, float soundVolume)
        {
            //throw new NotImplementedException();
            return 0;
        }

        public int PlayOccupationAmbient(int ghsound, string eventPath, int bankid, float eventVolume, float soundVolume)
        {
            //throw new NotImplementedException();
            return 0;
        }

        public void PlayTestSound()
        {
            //throw new NotImplementedException();
        }

        public int SetAmbientSoundVolume(ulong soundSourceId, float soundVolume)
        {
            //throw new NotImplementedException();
            return 0;
        }

        public int SetEffectAmbientVolume(float soundVolume)
        {
            //throw new NotImplementedException();
            return 0;
        }

        public int SetQuieterMode(bool state)
        {
            //throw new NotImplementedException();
            return 0;
        }

        public int StopAllSounds(ulong flags, uint dialogue_mid)
        {
            //throw new NotImplementedException();
            return 0;
        }

        public void StopTestSound()
        {
            //throw new NotImplementedException();
        }
    }
}