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
        public UInt64 guid;
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

        //private byte[] _bankBuffer1;
        //private byte[] _bankBuffer2;

        Bank[] _bank = new Bank[GHConstants.NumBanks];

        EventInstance? _testEventInstance;

        private float _generalVolume = 1.0f;
        private float _musicVolume = 1.0f;
        private float _ambientVolume = 1.0f;
        private float _dialogueVolume = 1.0f;
        private float _effectsVolume = 1.0f;
        private float _uiVolume = 1.0f;

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

        public string GetBankDir()
        {
            string filesdir = Android.App.Application.Context.FilesDir.Path;
            string bankdir = Path.Combine(filesdir, "bank");
            return bankdir;
        }

        public void LoadBanks()
        {
            RESULT res;
            string bankdir = GetBankDir();
            string bank_path;
            int idx = -1;
            foreach (string bankname in App.BankNameList)
            {
                idx++;
                bank_path = Path.Combine(bankdir, bankname);
                if (!File.Exists(bank_path))
                {
                    /* Error; should already be here */
                    continue;

                    //using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.Master.bank"))
                    //{
                    //    using (var fileStream = File.Create(bank_path))
                    //    {
                    //        stream.CopyTo(fileStream);
                    //    }
                    //    //_bankBuffer1 = new byte[stream.Length];
                    //    //stream.Read(_bankBuffer1, 0, (int)stream.Length);
                    //    //res = _system.loadBankMemory(_bankBuffer1, FMOD.Studio.LOAD_BANK_FLAGS.NORMAL, out _bank1);
                    //}
                }
                res = _system.loadBankFile(bank_path, LOAD_BANK_FLAGS.NORMAL, out _bank[idx]);
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
        public List<GHSoundInstance> levelAmbientInstances = new List<GHSoundInstance>();
        public List<GHSoundInstance> environmentAmbientInstances = new List<GHSoundInstance>();
        public List<GHSoundInstance> ambientList = new List<GHSoundInstance>();


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
            float relevant_volume = sound_type == (int)immediate_sound_types.IMMEDIATE_SOUND_UI ? _uiVolume : sound_type == (int)immediate_sound_types.IMMEDIATE_SOUND_DIALOGUE ? _dialogueVolume : _effectsVolume;

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

                res = eventInstance.setVolume(Math.Max(0.0f, Math.Min(1.0f, eventVolume * soundVolume * _generalVolume * relevant_volume)));
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
                    return (int)RESULT.OK;

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
                res = eventInstance.setVolume(Math.Max(0.0f, Math.Min(1.0f, eventVolume * soundVolume * _generalVolume * _musicVolume)));

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

        public int PlayLevelAmbient(int ghsound, string eventPath, int bankid, float eventVolume, float soundVolume)
        {
            RESULT res;

            if (ghsound <= 0)
            {
                if (levelAmbientInstances.Count > 0 && levelAmbientInstances[0].stopped == false)
                {
                    /* Stop ambient sound */
                    levelAmbientInstances[0].stopped = true;
                    res = levelAmbientInstances[0].instance.stop(FMOD.Studio.STOP_MODE.ALLOWFADEOUT);
                    if (res != RESULT.OK)
                        return (int)res;
                    res = _system.update();
                    return (int)res;
                }

                /* Nothing to do */
                return (int)RESULT.OK;
            }

            /* Check if the ambient is the same as before */
            if (levelAmbientInstances.Count > 0 && levelAmbientInstances[0].ghsound == ghsound && levelAmbientInstances[0].stopped == false)
            {
                /* Check if the volume is different */
                if (levelAmbientInstances[0].normalSoundVolume != soundVolume)
                {
                    res = levelAmbientInstances[0].instance.setVolume(Math.Max(0.0f, Math.Min(1.0f, eventVolume * soundVolume * _generalVolume * _ambientVolume)));
                    levelAmbientInstances[0].normalSoundVolume = soundVolume;
                    res = _system.update();
                    return (int)res;
                }
                return (int)RESULT.OK;
            }

            /* Different ambient; stop the previous one */
            if (levelAmbientInstances.Count > 0)
            {
                if (levelAmbientInstances[0].stopped == false)
                {
                    levelAmbientInstances[0].stopped = true;
                    levelAmbientInstances[0].instance.stop(FMOD.Studio.STOP_MODE.ALLOWFADEOUT);
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

            levelAmbientInstances.Insert(0, ghinstance);
            res = eventInstance.setVolume(Math.Max(0.0f, Math.Min(1.0f, eventVolume * soundVolume * _generalVolume * _ambientVolume)));

            if (levelAmbientInstances.Count >= 2)
            {
                GHSoundInstance ghsi = levelAmbientInstances[levelAmbientInstances.Count - 1];
                if (ghsi.stopped == false)
                {
                    ghsi.stopped = true;
                    ghsi.instance.stop(FMOD.Studio.STOP_MODE.ALLOWFADEOUT);
                }
                levelAmbientInstances.RemoveAt(levelAmbientInstances.Count - 1);
            }
            res = eventInstance.start();

            res = _system.update();
            return (int)res;
        }

        public int PlayEnvironmentAmbient(int ghsound, string eventPath, int bankid, float eventVolume, float soundVolume)
        {
            RESULT res;

            if (ghsound <= 0)
            {
                if (environmentAmbientInstances.Count > 0 && environmentAmbientInstances[0].stopped == false)
                {
                    /* Stop ambient sound */
                    environmentAmbientInstances[0].stopped = true;
                    res = environmentAmbientInstances[0].instance.stop(FMOD.Studio.STOP_MODE.ALLOWFADEOUT);
                    if (res != RESULT.OK)
                        return (int)res;
                    res = _system.update();
                    return (int)res;
                }

                /* Nothing to do */
                return (int)RESULT.OK;
            }

            /* Check if the ambient is the same as before */
            if (environmentAmbientInstances.Count > 0 && environmentAmbientInstances[0].ghsound == ghsound && environmentAmbientInstances[0].stopped == false)
            {
                /* Check if the volume is different */
                if (environmentAmbientInstances[0].normalSoundVolume != soundVolume)
                {
                    res = environmentAmbientInstances[0].instance.setVolume(Math.Max(0.0f, Math.Min(1.0f, eventVolume * soundVolume * _generalVolume * _ambientVolume)));
                    environmentAmbientInstances[0].normalSoundVolume = soundVolume;
                    res = _system.update();
                    return (int)res;
                }
                return (int)RESULT.OK;
            }

            /* Different ambient; stop the previous one */
            if (environmentAmbientInstances.Count > 0)
            {
                if (environmentAmbientInstances[0].stopped == false)
                {
                    environmentAmbientInstances[0].stopped = true;
                    environmentAmbientInstances[0].instance.stop(FMOD.Studio.STOP_MODE.ALLOWFADEOUT);
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

            environmentAmbientInstances.Insert(0, ghinstance);
            res = eventInstance.setVolume(Math.Max(0.0f, Math.Min(1.0f, eventVolume * soundVolume * _generalVolume * _ambientVolume)));

            if (environmentAmbientInstances.Count >= 2)
            {
                GHSoundInstance ghsi = environmentAmbientInstances[environmentAmbientInstances.Count - 1];
                if (ghsi.stopped == false)
                {
                    ghsi.stopped = true;
                    ghsi.instance.stop(FMOD.Studio.STOP_MODE.ALLOWFADEOUT);
                }
                environmentAmbientInstances.RemoveAt(environmentAmbientInstances.Count - 1);
            }
            res = eventInstance.start();

            res = _system.update();
            return (int)res;
        }

        private UInt64 _lastAmbientId = 0;

        private UInt64 GetNewAmbientId()
        {
            _lastAmbientId++;
            if(_lastAmbientId == 0)
                _lastAmbientId++;

            return _lastAmbientId;
        }

        public int AddAmbientSound(int ghsound, string eventPath, int bankid, float eventVolume, float soundVolume, out UInt64 soundSourceId)
        {
            RESULT res = RESULT.OK;

            if (eventPath == null || eventPath == "")
            {
                soundSourceId = 0;
                return 1;
            }

            EventDescription eventDescription;
            res = _system.getEvent(eventPath, out eventDescription);
            EventInstance eventInstance;
            res = eventDescription.createInstance(out eventInstance);

            /* Set volume */
            res = eventInstance.setVolume(Math.Max(0.0f, Math.Min(1.0f, eventVolume * soundVolume * _generalVolume * _ambientVolume)));
            //ambientInstance->setVolume(fmod_volume* event_volume * general_ambient_volume* general_volume);

            /* Create new GHSoundInstance */
            GHSoundInstance ghinstance = new GHSoundInstance();
            ghinstance.instance = eventInstance;
            ghinstance.ghsound = ghsound;
            ghinstance.normalEventVolume = eventVolume;
            ghinstance.normalSoundVolume = soundVolume;
            ghinstance.sound_type = 0;
            ghinstance.dialogue_mid = 0;
            ghinstance.guid = GetNewAmbientId();

            ambientList.Insert(0, ghinstance);

            /* Play sound */
            res = eventInstance.start();
            res = _system.update();

            soundSourceId = ghinstance.guid;

            return (int)res;

        }
        public int DeleteAmbientSound(UInt64 soundSourceId)
        {
            if (soundSourceId == 0)
                return 1;

            RESULT res;
            bool found = false;
            GHSoundInstance ghinstance = null;
            int listidx = -1;
            for (int i = 0; i < ambientList.Count; i++)
            {
                if (ambientList[i].guid == soundSourceId)
                {
                    found = true;
                    listidx = i;
                    ghinstance = ambientList[i];
                    break;
                }
            }

            if (!found || ghinstance == null || listidx == -1)
                return 1;

            if (ghinstance.stopped == false)
            {
                ghinstance.stopped = true;
                ghinstance.instance.stop(FMOD.Studio.STOP_MODE.ALLOWFADEOUT);
            }
            res = _system.update();
            res = ghinstance.instance.release();
            ambientList.RemoveAt(listidx);
            return 0;
        }
        public int SetAmbientSoundVolume(UInt64 soundSourceId, float soundVolume)
        {
            if (soundSourceId == 0)
                return 1;

            if (soundVolume < 0.0f || soundVolume > 1.0f)
                return 1;

            RESULT res;
            GHSoundInstance ghinstance = null;
            for (int i = 0; i < ambientList.Count; i++)
            {
                if (ambientList[i].guid == soundSourceId)
                {
                    ghinstance = ambientList[i];
                    break;
                }
            }

            if (ghinstance == null)
                return 1;

            float old_volume;
            res = ghinstance.instance.getVolume(out old_volume);

            float event_volume = ghinstance.normalEventVolume;
            res = ghinstance.instance.setVolume(event_volume * soundVolume * _generalVolume * _ambientVolume);

            if (old_volume == 0.0f && soundVolume > 0.0f)
                res = ghinstance.instance.start();
            else if (old_volume > 0.0f && soundVolume == 0.0f)
                res = ghinstance.instance.stop(FMOD.Studio.STOP_MODE.ALLOWFADEOUT);

            res = _system.update();
            ghinstance.normalSoundVolume = soundVolume;

            return 0;
        }

        public int AdjustVolumes(float new_general_volume, float new_general_music_volume, float new_general_ambient_volume, float new_general_dialogue_volume, float new_general_sfx_volume, float new_general_ui_volume)
        {
            _generalVolume = new_general_volume;
            _musicVolume = new_general_music_volume;
            _ambientVolume = new_general_ambient_volume;
            _dialogueVolume = new_general_dialogue_volume;
            _effectsVolume = new_general_sfx_volume;
            _uiVolume = new_general_ui_volume;

            RESULT result;
            foreach (GHSoundInstance si in musicInstances)
            {
                if (!si.stopped)
                {
                    result = si.instance.setVolume(Math.Min(1.0f, si.normalSoundVolume * si.normalEventVolume * _musicVolume * _generalVolume));
                }
            }
            foreach (GHSoundInstance si in levelAmbientInstances)
            {
                if (!si.stopped)
                {
                    result = si.instance.setVolume(Math.Min(1.0f, si.normalSoundVolume * si.normalEventVolume * _ambientVolume * _generalVolume));
                }
            }
            foreach (GHSoundInstance si in environmentAmbientInstances)
            {
                if (!si.stopped)
                {
                    result = si.instance.setVolume(Math.Min(1.0f, si.normalSoundVolume * si.normalEventVolume * _ambientVolume * _generalVolume));
                }
            }
            foreach (GHSoundInstance si in environmentAmbientInstances)
            {
                if (!si.stopped)
                {
                    result = si.instance.setVolume(Math.Min(1.0f, si.normalSoundVolume * si.normalEventVolume * _ambientVolume * _generalVolume));
                }
            }
            /* Add occupation ambients */
            /* Add effect ambients */
            foreach (GHSoundInstance si in immediateInstances)
            {
                if (!si.stopped)
                {
                    float relevant_volume = si.sound_type == immediate_sound_types.IMMEDIATE_SOUND_UI ? _uiVolume : si.sound_type == immediate_sound_types.IMMEDIATE_SOUND_DIALOGUE ? _dialogueVolume : _effectsVolume;
                    result = si.instance.setVolume(Math.Min(1.0f, si.normalSoundVolume * si.normalEventVolume * relevant_volume * _generalVolume));
                }
            }
            foreach (GHSoundInstance si in longImmediateInstances)
            {
                if (!si.stopped)
                {
                    float relevant_volume = si.sound_type == immediate_sound_types.IMMEDIATE_SOUND_UI ? _uiVolume : si.sound_type == immediate_sound_types.IMMEDIATE_SOUND_DIALOGUE ? _dialogueVolume : _effectsVolume;
                    result = si.instance.setVolume(Math.Min(1.0f, si.normalSoundVolume * si.normalEventVolume * relevant_volume * _generalVolume));
                }
            }

            foreach (GHSoundInstance si in ambientList)
            {
                if (!si.stopped)
                {
                    result = si.instance.setVolume(Math.Min(1.0f, si.normalSoundVolume * si.normalEventVolume * _ambientVolume * _generalVolume));
                }
            }

            result = _system.update();
            return (int)result;
        }
    }
}