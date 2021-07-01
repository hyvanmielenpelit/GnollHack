using Android.App;
using Android.Content;
using Android.OS;
using Android.Runtime;
using Android.Views;
using Android.Widget;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using FMOD;
using System.Reflection;
using System.IO;
using Xamarin.Forms;
using GnollHackCommon;

[assembly: Dependency(typeof(GnollHackClient.Droid.FmodService))]
namespace GnollHackClient.Droid
{
    public class GHSoundInstance
    {
        public FMOD.Studio.EventInstance instance;
        public int ghsound;
        public float normalEventVolume;
        public float normalSoundVolume;
        public int sound_type;
        public uint dialogue_mid;
        public bool stopped;
        public bool queued;

        public GHSoundInstance()
        {

        }
    };


    class FmodService : IFmodService
    {
        private FMOD.Studio.System _system;
        private FMOD.System _coresystem;

        private byte[] _bankBuffer1;
        private byte[] _bankBuffer2;

        FMOD.Studio.Bank _bank1;
        FMOD.Studio.Bank _bank2;

        FMOD.Studio.EventInstance? _testEventInstance;

        private void LoadNativeLibrary(string libName)
        {
            Java.Lang.JavaSystem.LoadLibrary(libName);
        }

        public void InitializeFmod()
        {
            LoadNativeLibrary("fmod");
            LoadNativeLibrary("fmodstudio");

            RESULT res;
            res = FMOD.Studio.System.create(out _system);
            res = _system.getCoreSystem(out _coresystem);
            res = _coresystem.setSoftwareFormat(0, SPEAKERMODE.DEFAULT, 0);
            res = _system.initialize(1024, FMOD.Studio.INITFLAGS.NORMAL, INITFLAGS.NORMAL, IntPtr.Zero);
        }

        public void LoadBanks()
        {
            //result = fmod_studio_system->loadBankMemory((const char*)data, (int)len, FMOD_STUDIO_LOAD_MEMORY, FMOD_STUDIO_LOAD_BANK_NORMAL, &bank[i]);
            Assembly assembly = typeof(GnollHackClient.IFmodService).Assembly;
            RESULT res;

            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.Master.bank"))
            {
                _bankBuffer1 = new byte[stream.Length];
                stream.Read(_bankBuffer1, 0, (int)stream.Length);
                res = _system.loadBankMemory(_bankBuffer1, FMOD.Studio.LOAD_BANK_FLAGS.NORMAL, out _bank1);
            }

            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.Master.strings.bank"))
            {
                _bankBuffer2 = new byte[stream.Length];
                stream.Read(_bankBuffer2, 0, (int)stream.Length);
                res = _system.loadBankMemory(_bankBuffer2, FMOD.Studio.LOAD_BANK_FLAGS.NORMAL, out _bank2);
            }
        }

        public void PlayTestSound()
        {
            string eventPath = "event:/Music/Start/Splash";
            FMOD.Studio.EventDescription eventDescription;
            RESULT res = _system.getEvent(eventPath, out eventDescription);
            FMOD.Studio.EventInstance testEventInstance;
            res = eventDescription.createInstance(out testEventInstance);
            _testEventInstance = testEventInstance;
            res = _testEventInstance.Value.setVolume(1.0f);
            res = _testEventInstance.Value.start();
            res = _system.update();
        }

        public void StopTestSound()
        {
            if(_testEventInstance.HasValue)
            {
                var res = _testEventInstance.Value.stop(FMOD.Studio.STOP_MODE.ALLOWFADEOUT);
                res = _system.update();
            }
        }

        List<GHSoundInstance> musicInstances = new List<GHSoundInstance>();
        List<GHSoundInstance> immediateInstances = new List<GHSoundInstance>();
        List<GHSoundInstance> longImmediateInstances = new List<GHSoundInstance>();

        public int PlayImmediateSound(int ghsound, string eventPath, int bankid, float eventVolume, float soundVolume, string[] parameterNames, float[] parameterValues, int arraysize, int sound_type, int play_group, uint dialogue_mid)
        {
            FMOD.Studio.EventDescription eventDescription;
            RESULT res = _system.getEvent(eventPath, out eventDescription);
            FMOD.Studio.EventInstance eventInstance;
            res = eventDescription.createInstance(out eventInstance);

            GHSoundInstance ghinstance = new GHSoundInstance();
            ghinstance.instance = eventInstance;
            ghinstance.ghsound = ghsound;
            ghinstance.normalEventVolume = eventVolume;
            ghinstance.normalSoundVolume = soundVolume;
            ghinstance.sound_type = sound_type;
            ghinstance.dialogue_mid = dialogue_mid;

            if(sound_type == 2)
                longImmediateInstances.Insert(0, ghinstance);
            else
                immediateInstances.Insert(0, ghinstance);

            res = eventInstance.setVolume(Math.Max(0.0f, Math.Min(1.0f, eventVolume * soundVolume)));
            for(int i = 0; i < arraysize; i++)
            {
                if(i < parameterNames.Length && i < parameterValues.Length)
                {
                    string str = parameterNames[i];
                    if (str != null && str != "")
                        eventInstance.setParameterByName(str, parameterValues[i]);
                    else
                        break;
                }
            }

            if (sound_type == 2)
            {
                if(longImmediateInstances.Count >= 64)
                {
                    GHSoundInstance ghsi = longImmediateInstances[longImmediateInstances.Count - 1];
                    if (ghsi.stopped == false)
                    {
                        ghsi.stopped = true;
                        ghsi.instance.stop(FMOD.Studio.STOP_MODE.ALLOWFADEOUT);
                    }
                    ghsi.instance.release();
                    longImmediateInstances.RemoveAt(longImmediateInstances.Count - 1);
                }
            }
            else
            {
                if (immediateInstances.Count >= 64)
                {
                    GHSoundInstance ghsi = immediateInstances[immediateInstances.Count - 1];
                    if (ghsi.stopped == false)
                    {
                        ghsi.stopped = true;
                        ghsi.instance.stop(FMOD.Studio.STOP_MODE.ALLOWFADEOUT);
                    }
                    ghsi.instance.release();
                    immediateInstances.RemoveAt(immediateInstances.Count - 1);
                }
            }

            res = eventInstance.start();
            res = _system.update();

            return (int)res;
        }

        public int StopAllSounds(ulong flags, uint dialogue_mid)
        {
            RESULT res = 0;
            if (flags == 0)
                flags = (ulong)StopSoundFlags.All;

            if ((flags & (ulong)StopSoundFlags.ImmediateNormal) != 0)
            {
                foreach (GHSoundInstance ghsi in immediateInstances)
                {
                    if (ghsi.stopped == false)
                    {
                        ghsi.stopped = true;
                        ghsi.instance.stop(FMOD.Studio.STOP_MODE.ALLOWFADEOUT);
                    }
                    ghsi.instance.release();
                }
                immediateInstances.Clear();
            }

            if ((flags & (ulong)StopSoundFlags.ImmediateLong) != 0)
            {
                foreach (GHSoundInstance ghsi in longImmediateInstances)
                {
                    if (ghsi.stopped == false)
                    {
                        ghsi.stopped = true;
                        ghsi.instance.stop(FMOD.Studio.STOP_MODE.ALLOWFADEOUT);
                    }
                    ghsi.instance.release();
                }
                longImmediateInstances.Clear();
            }

            if ((flags & (ulong)StopSoundFlags.Music) != 0)
            {
                foreach (GHSoundInstance ghsi in musicInstances)
                {
                    if (ghsi.stopped == false)
                    {
                        ghsi.stopped = true;
                        ghsi.instance.stop(FMOD.Studio.STOP_MODE.ALLOWFADEOUT);
                    }
                    ghsi.instance.release();
                }
                musicInstances.Clear();
            }
            res = _system.update();
            return (int)res;
        }


        public int PlayMusic(int ghsound, string eventPath, int bankid, float eventVolume, float soundVolume)
        {
            if (musicInstances.Count > 0 && musicInstances[0].ghsound == ghsound && musicInstances[0].stopped == false)
                return 0;

            if (musicInstances.Count > 0)
            {
                if (musicInstances[0].stopped == false)
                {
                    musicInstances[0].stopped = true;
                    musicInstances[0].instance.stop(FMOD.Studio.STOP_MODE.ALLOWFADEOUT);
                }
            }

            FMOD.Studio.EventDescription eventDescription;
            RESULT res = _system.getEvent(eventPath, out eventDescription);
            FMOD.Studio.EventInstance eventInstance;
            res = eventDescription.createInstance(out eventInstance);

            GHSoundInstance ghinstance = new GHSoundInstance();
            ghinstance.instance = eventInstance;
            ghinstance.ghsound = ghsound;
            ghinstance.normalEventVolume = eventVolume;
            ghinstance.normalSoundVolume = soundVolume;
            ghinstance.sound_type = 0;
            ghinstance.dialogue_mid = 0;

            musicInstances.Insert(0, ghinstance);
            res = eventInstance.setVolume(Math.Max(0.0f, Math.Min(1.0f, eventVolume * soundVolume)));
 
            if (musicInstances.Count >= 2)
            {
                GHSoundInstance ghsi = musicInstances[musicInstances.Count - 1];
                if (ghsi.stopped == false)
                {
                    ghsi.stopped = true;
                    ghsi.instance.stop(FMOD.Studio.STOP_MODE.ALLOWFADEOUT);
                }
                musicInstances.RemoveAt(musicInstances.Count - 1);
            }

            res = eventInstance.start();
            res = _system.update();

            return (int)res;
        }


    }
}