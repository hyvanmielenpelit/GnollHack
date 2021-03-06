﻿using Android.App;
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
using FMOD.Studio;
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
        public immediate_sound_types sound_type;
        public uint dialogue_mid;
        public bool queued;
        public bool stopped;

        public GHSoundInstance()
        {

        }
    };


    class FmodService : IFmodService
    {
        public static FmodService _latestService = null;
        private static FMOD.Studio.System _system;
        private static FMOD.System _coresystem;

        private byte[] _bankBuffer1;
        private byte[] _bankBuffer2;

        FMOD.Studio.Bank _bank1;
        FMOD.Studio.Bank _bank2;

        FMOD.Studio.EventInstance? _testEventInstance;

        public FmodService()
        {
            _latestService = this;
        }

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
            res = _system.initialize(1024, FMOD.Studio.INITFLAGS.NORMAL, FMOD.INITFLAGS.NORMAL, IntPtr.Zero);
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
            EventDescription eventDescription;
            RESULT res = _system.getEvent(eventPath, out eventDescription);
            EventInstance testEventInstance;
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


        //public object eventInstanceLock = new object();
        public List<GHSoundInstance> musicInstances = new List<GHSoundInstance>();
        public List<GHSoundInstance> immediateInstances = new List<GHSoundInstance>();
        public List<GHSoundInstance> longImmediateInstances = new List<GHSoundInstance>();


        public static RESULT GNHEventCallback(EVENT_CALLBACK_TYPE type, EventInstance _event, IntPtr parameters)
        {
            RESULT result;
            FmodService service = _latestService;

            if (service == null)
                return RESULT.ERR_UNSUPPORTED;

            //lock(service.eventInstanceLock)
            //{
                if (type == EVENT_CALLBACK_TYPE.STOPPED || type == EVENT_CALLBACK_TYPE.START_FAILED)
                {
                    for (int i = 0; i < GHConstants.MaxNormalImmediateSoundInstances; i++)
                    {
                        if (i >= service.immediateInstances.Count)
                            break;

                        if (service.immediateInstances[i].instance.handle == _event.handle)
                        {
                            service.immediateInstances[i].stopped = true;
                            for (int j = i - 1; j >= 0; j--)
                            {
                                if (service.immediateInstances[j].queued)
                                {
                                    service.immediateInstances[j].queued = false;
                                    result = service.immediateInstances[j].instance.start();
                                    result = _system.update();
                                    return RESULT.OK;
                                }
                            }
                        }
                    }
                    for (int i = 0; i < GHConstants.MaxLongImmediateSoundInstances; i++)
                    {
                        if (i >= service.longImmediateInstances.Count)
                            break;

                        if (service.longImmediateInstances[i].instance.handle == _event.handle)
                        {
                            service.longImmediateInstances[i].stopped = true;
                            for (int j = i - 1; j >= 0; j--)
                            {
                                if (service.longImmediateInstances[j].queued)
                                {
                                    service.longImmediateInstances[j].queued = false;
                                    result = service.longImmediateInstances[j].instance.start();
                                    result = _system.update();
                                    return RESULT.OK;
                                }
                            }
                        }
                    }
                }
                return RESULT.OK;
            //}
        }


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
            ghinstance.sound_type = (immediate_sound_types)sound_type;
            ghinstance.dialogue_mid = dialogue_mid;
            ghinstance.queued = false;
            ghinstance.stopped = false;

            bool queue_sound = false;

            //lock (eventInstanceLock)
            //{
                if (play_group == (int)sound_play_groups.SOUND_PLAY_GROUP_LONG)
                {
                    queue_sound = (longImmediateInstances.Count > 0 && sound_type == (int)immediate_sound_types.IMMEDIATE_SOUND_DIALOGUE && longImmediateInstances[0].sound_type == immediate_sound_types.IMMEDIATE_SOUND_DIALOGUE && !longImmediateInstances[0].stopped);
                    longImmediateInstances.Insert(0, ghinstance);

                    if (sound_type == (int)immediate_sound_types.IMMEDIATE_SOUND_DIALOGUE)
                        res = longImmediateInstances[0].instance.setCallback(GNHEventCallback, EVENT_CALLBACK_TYPE.STOPPED | EVENT_CALLBACK_TYPE.START_FAILED);

                    /* Fallback if queued for too long */
                    if (longImmediateInstances.Count >= GHConstants.MaxLongImmediateSoundInstances && longImmediateInstances[GHConstants.MaxLongImmediateSoundInstances - 1].queued && !longImmediateInstances[GHConstants.MaxLongImmediateSoundInstances - 1].stopped)
                    {
                        longImmediateInstances[GHConstants.MaxLongImmediateSoundInstances - 1].queued = false;
                        res = longImmediateInstances[GHConstants.MaxLongImmediateSoundInstances - 1].instance.start();
                    }
                }
                else
                {
                    queue_sound = (immediateInstances.Count > 0 && sound_type == (int)immediate_sound_types.IMMEDIATE_SOUND_DIALOGUE && immediateInstances[0].sound_type == immediate_sound_types.IMMEDIATE_SOUND_DIALOGUE && !immediateInstances[0].stopped);

                    immediateInstances.Insert(0, ghinstance);

                    if (sound_type == (int)immediate_sound_types.IMMEDIATE_SOUND_DIALOGUE)
                        res = immediateInstances[0].instance.setCallback(GNHEventCallback, EVENT_CALLBACK_TYPE.STOPPED | EVENT_CALLBACK_TYPE.START_FAILED);

                    /* Fallback if queued for too long */
                    if (immediateInstances.Count >= GHConstants.MaxNormalImmediateSoundInstances && immediateInstances[GHConstants.MaxNormalImmediateSoundInstances - 1].queued && !immediateInstances[GHConstants.MaxNormalImmediateSoundInstances - 1].stopped)
                    {
                        immediateInstances[GHConstants.MaxNormalImmediateSoundInstances - 1].queued = false;
                        res = immediateInstances[GHConstants.MaxNormalImmediateSoundInstances - 1].instance.start(); ;
                    }
                }

                ghinstance.queued = queue_sound;

                res = eventInstance.setVolume(Math.Max(0.0f, Math.Min(1.0f, eventVolume * soundVolume)));
                for (int i = 0; i < arraysize; i++)
                {
                    if (i < parameterNames.Length && i < parameterValues.Length)
                    {
                        string str = parameterNames[i];
                        if (str != null && str != "")
                            eventInstance.setParameterByName(str, parameterValues[i]);
                        else
                            break;
                    }
                }

                if (play_group == (int)sound_play_groups.SOUND_PLAY_GROUP_LONG)
                {
                    if (longImmediateInstances.Count > GHConstants.MaxLongImmediateSoundInstances)
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
                    if (immediateInstances.Count > GHConstants.MaxNormalImmediateSoundInstances)
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
            //}

            if (!queue_sound)
            {
                res = eventInstance.start();
            }

            res = _system.update();
            return (int)res;
        }

        public int StopAllSounds(ulong flags, uint dialogue_mid)
        {
            RESULT res = 0;
            if (flags == 0)
                flags = (ulong)StopSoundFlags.All;

            //lock (eventInstanceLock)
            //{
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
            //}
            res = _system.update();
            return (int)res;
        }


        public int PlayMusic(int ghsound, string eventPath, int bankid, float eventVolume, float soundVolume)
        {
            RESULT res;
            //lock (eventInstanceLock)
            //{

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

                EventDescription eventDescription;
                res = _system.getEvent(eventPath, out eventDescription);
                EventInstance eventInstance;
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
            //}
            res = _system.update();
            return (int)res;
        }


    }
}