using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using FMOD;
using FMOD.Studio;
using System.Reflection;
using System.IO;
using System.Threading;
using System.Threading.Tasks;
using System.Collections.Concurrent;

#if GNH_MAUI
using GnollHackX;
using GnollHackM;
using Microsoft.Maui.Graphics;
#else
using Xamarin.Forms;
using GnollHackX;
#endif

#if __IOS__
using Foundation;
using AVFoundation;

#if GNH_MAUI
namespace GnollHackM
#else
[assembly: Dependency(typeof(GnollHackX.iOS.FmodService))]
namespace GnollHackX.iOS
#endif
#elif __ANDROID__
using Android.Content.Res;
using Java.Util.Zip;
using Android.App;
using Android.Content;
using Android.OS;
using Android.Runtime;
using Android.Views;
using Android.Widget;

#if GNH_MAUI
namespace GnollHackM
#else
[assembly: Dependency(typeof(GnollHackX.Droid.FmodService))]
namespace GnollHackX.Droid
#endif
#else
#if GNH_MAUI
namespace GnollHackM
#else
[assembly: Dependency(typeof(GnollHackX.Unknown.FmodService))]
namespace GnollHackX.Unknown
#endif
#endif
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

    public class LoadedBank
    {
        public readonly Bank Bank;
        public readonly int SubType;

        public LoadedBank(Bank bank, int subType)
        {
            Bank = bank;
            SubType = subType;
        }
    }

    public class LoadableBank
    {
        public readonly string FullPathName;
        public readonly int SubType;
        public readonly bool IsResource;
        public readonly bool ReadToMemory;
        public readonly byte[] ByteBuffer;
        public LoadableBank(string path, int subType, bool isResource, bool readToMemory, byte[] byteBuffer)
        {
            FullPathName = path;
            SubType = subType;
            IsResource = isResource;
            ReadToMemory = readToMemory;
            ByteBuffer = byteBuffer;
        }
    }


    class FmodService : IFmodService
    {
        public static FmodService _latestService = null;
        private static FMOD.Studio.System _system = new FMOD.Studio.System();
        private static FMOD.System _coresystem = new FMOD.System();

        //private byte[] _bankBuffer1;
        //private byte[] _bankBuffer2;

        private readonly List<LoadedBank> _banks = new List<LoadedBank>();

        EventInstance? _testEventInstance;

        private float _generalVolume = 1.0f;
        private float _musicVolume = 1.0f;
        private float GeneralVolume /* Called from both UI and game thread */
        {
            get { return Interlocked.CompareExchange(ref _generalVolume, 0.0f, 0.0f); }
            set { Interlocked.Exchange(ref _generalVolume, value); }
        }
        private float MusicVolume /* Called from both UI and game thread */
        {
            get { return Interlocked.CompareExchange(ref _musicVolume, 0.0f, 0.0f); }
            set { Interlocked.Exchange(ref _musicVolume, value); }
        }

        private float _ambientVolume = 1.0f; /* Called from game thread */
        private float _dialogueVolume = 1.0f; /* Called from game thread */
        private float _effectsVolume = 1.0f; /* Called from game thread */

        private float _uiVolume = 1.0f; /* Called from UI thread */
        private float _gameUiVolume = 1.0f; /* Called from game thread */

        private bool _quieterMode = false;
        private const float _quietModeMultiplier = 0.35f;

        public FmodService()
        {
            _latestService = this;
        }

#if __ANDROID__
        private static void LoadNativeLibrary(string libName)
        {
            Java.Lang.JavaSystem.LoadLibrary(libName);
        }

        public static void AndroidInit(Context p0)
        {
            LoadNativeLibrary("fmod");
            LoadNativeLibrary("fmodstudio");
            Org.Fmod.FMOD.Init(p0);
        }
#endif

        private bool _initialized = false;

        public void InitializeFmod()
        {
            RESULT res;

            //uint ver;
            //res = Factory.System_Create(out _coresystem);
            //if (res != RESULT.OK)
            //    return;

            //res = _coresystem.getVersion(out ver);
            //if (res != RESULT.OK)
            //    return;

            res = FMOD.Studio.System.create(out _system);
            if (res != RESULT.OK)
                return;

            res = _system.getCoreSystem(out _coresystem);
            if (res != RESULT.OK)
                return;

            res = _coresystem.setSoftwareFormat(0, SPEAKERMODE.DEFAULT, 0);
            if (res != RESULT.OK)
                return;

            _coresystem.getSoftwareFormat(out int systemSampleRate, out _, out _);
            _coresystem.getDSPBufferSize(out uint systemDSPBufferLength, out _);
            SetAudioSessionSettings(systemSampleRate, systemDSPBufferLength);
            res = _system.initialize(GHConstants.MaxChannels, FMOD.Studio.INITFLAGS.NORMAL, FMOD.INITFLAGS.NORMAL, IntPtr.Zero);
            if (res != RESULT.OK)
                return;

            _initialized = true;
            GHApp.MaybeWriteGHLog("FMOD initialized successfully.");
        }

        private void SetAudioSessionSettings(double rate, double blockSize)
        {
#if __IOS__ || (GNH_MAUI && IOS)
            AVAudioSession si = AVAudioSession.SharedInstance();
            if (si != null)
            {
                GHApp.MaybeWriteGHLog("SetAudioSessionSettings: rate is " + rate + ", blockSize is " + blockSize);
                try
                {
                    NSError audioSessionError;
                    double bufferDuration = blockSize / rate;
                    si.SetPreferredIOBufferDuration(bufferDuration, out audioSessionError);
                    si.SetPreferredSampleRate(rate, out audioSessionError);
                    si.SetCategory(AVAudioSessionCategory.Ambient);
                    si.SetActive(true);
                }
                catch (Exception ex)
                {
                    GHApp.MaybeWriteGHLog("Exception occurred with AVAudioSession: " + ex.Message);
                }
            }
#endif
        }


        private bool FMODup()
        {
            return _initialized && GHApp.LoadBanks;
        }

        public void UnloadBanks(sound_bank_loading_type loadingType)
        {
            try
            {
                int subType = (int)loadingType;
                RESULT res;
                for (int i = _banks.Count - 1; i >= 0; i--)
                {
                    if (_banks[i].SubType == subType)
                    {
                        res = _banks[i].Bank.unload();
                        _banks[i].Bank.clearHandle();
                        _banks.RemoveAt(i);
                    }
                }
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("Unload Banks: "+ ex.Message);
            }

            //RESULT res;
            ////res = _system.unloadAll();
            //foreach(var loadedBank in _banks)
            //{
            //    res = loadedBank.Bank.unload();
            //    loadedBank.Bank.clearHandle();
            //}
            //_banks.Clear();
        }

        public void LoadBanks(sound_bank_loading_type loadingType)
        {
            try
            {
                int subType = (int)loadingType;
                RESULT res;
                foreach (LoadableBank loadableBank in _loadableSoundBanks)
                {
                    if (loadableBank.SubType == subType)
                    {
                        if (loadableBank.ReadToMemory)
                        {
                            if (loadableBank.ByteBuffer != null)
                            {
                                Bank tmpbank = new Bank();
                                res = _system.loadBankMemory(loadableBank.ByteBuffer, LOAD_BANK_FLAGS.NORMAL, out tmpbank);
                                if (res == RESULT.OK)
                                    _banks.Add(new LoadedBank(tmpbank, loadableBank.SubType));
                                else
                                    GHApp.MaybeWriteGHLog("LoadBanks, ReadToMemory: Result: " + ((int)res).ToString() + "(" + res.ToString() + ") , bank_path: " + loadableBank.FullPathName);
                            }
                        }
                        else
                        {
                            string bank_path = loadableBank.FullPathName;
                            if (loadableBank.IsResource || File.Exists(bank_path))
                            {
                                Bank tmpbank = new Bank();
                                res = _system.loadBankFile(bank_path, LOAD_BANK_FLAGS.NORMAL, out tmpbank);
                                if (res == RESULT.OK)
                                    _banks.Add(new LoadedBank(tmpbank, loadableBank.SubType));
                                else
                                    GHApp.MaybeWriteGHLog("LoadBanks, Non-ReadToMemory: Result: " + ((int)res).ToString() + "(" + res.ToString() + ") , bank_path: " + bank_path);
                            }
                        }
                    }
                }
            }
            catch (Exception ex) 
            {
                GHApp.MaybeWriteGHLog("LoadBanks: " + ex.Message);
            }
        }

        private readonly List<LoadableBank> _loadableSoundBanks = new List<LoadableBank>();

        public void ClearLoadableSoundBanks()
        {
            _loadableSoundBanks.Clear();
        }


        public async Task AddLoadableSoundBank(string fullfilepath, int subType, bool isResource, bool readToMemory)
        {
            byte[] data = null;
            if (readToMemory)
            {
                try
                {
                    if (isResource)
                    {
#if __IOS__
                        using(FileStream fs = File.OpenRead(fullfilepath))
#elif __ANDROID__
                        AssetManager assets = MainActivity.StaticAssets;
                        using (Stream fs = assets.Open(fullfilepath))
#elif WINDOWS
                        using Stream fs = await FileSystem.Current.OpenAppPackageFileAsync(fullfilepath);
#else
                        using (FileStream fs = File.OpenRead(fullfilepath))
#endif
                        {
                            using (var ms = new MemoryStream())
                            {
                                try
                                {
                                    await fs.CopyToAsync(ms);
                                    data = ms.ToArray();
                                }
                                catch (Exception ex)
                                {
                                    GHApp.MaybeWriteGHLog("AddLoadableSoundBank, MemoryStream: Path: " + fullfilepath + ", Exception: " + ex.Message);  ;
                                }
                            }
                        }
                    }
                    else
                    {
                        if (File.Exists(fullfilepath))
                        {
                            using (FileStream fs = File.OpenRead(fullfilepath))
                            {
                                using (BinaryReader br = new BinaryReader(fs))
                                {
                                    long length = fs.Length;
                                    data = br.ReadBytes((int)length);
                                }
                            }
                        }
                    }
                }
                catch (Exception ex)
                {
                    GHApp.MaybeWriteGHLog("AddLoadableSoundBank: Path: " + fullfilepath + ", Exception: " + ex.Message); ;
                }
            }
            _loadableSoundBanks.Add(new LoadableBank(fullfilepath, subType, isResource, readToMemory, data));
        }

        public void LoadIntroSoundBank()
        {
            LoadBanks(sound_bank_loading_type.Intro);
        }

        public void UnloadIntroSoundBank()
        {
            UnloadBanks(sound_bank_loading_type.Intro);
        }

        public void PlayTestSound()
        {
            string eventPath = "event:/Music/Main Screen/Splash";
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
            if (_testEventInstance.HasValue)
            {
                var res = _testEventInstance.Value.stop(FMOD.Studio.STOP_MODE.ALLOWFADEOUT);
                res = _system.update();
            }
        }

        //public readonly object _eventInstanceLock = new object();
        /* Modified from the game thread */
        public List<GHSoundInstance> musicInstances = new List<GHSoundInstance>();
        public List<GHSoundInstance> immediateInstances = new List<GHSoundInstance>();
        public List<GHSoundInstance> longImmediateInstances = new List<GHSoundInstance>();
        public List<GHSoundInstance> levelAmbientInstances = new List<GHSoundInstance>();
        public List<GHSoundInstance> environmentAmbientInstances = new List<GHSoundInstance>();
        public List<GHSoundInstance> occupationAmbientInstances = new List<GHSoundInstance>();
        public List<GHSoundInstance> effectAmbientInstances = new List<GHSoundInstance>();
        public List<GHSoundInstance> ambientList = new List<GHSoundInstance>();

        /* Modified from the UI thread */
        public List<GHSoundInstance> uiMusicInstances = new List<GHSoundInstance>();
        public List<GHSoundInstance> uiInstances = new List<GHSoundInstance>();


        /* Called from the UI thread */
        public const int nooflists = 9;
        public void ReleaseAllSoundInstances()
        {
            List<GHSoundInstance>[] listoflists = new List<GHSoundInstance>[nooflists]
            {
                musicInstances, immediateInstances, longImmediateInstances, uiInstances, levelAmbientInstances,
                environmentAmbientInstances, occupationAmbientInstances, effectAmbientInstances, ambientList
            };

            //lock (_eventInstanceLock)
            {
                foreach (List<GHSoundInstance> list in listoflists)
                {
                    foreach (GHSoundInstance instance in list)
                    {
                        if (!instance.stopped)
                            instance.instance.stop(STOP_MODE.IMMEDIATE);
                        instance.instance.release();
                    }
                    list.Clear();
                }
            }
            RESULT res = _system.update();
        }

        /* Returns to UI thread */
        public static RESULT GNHUIEventCallback(EVENT_CALLBACK_TYPE type, IntPtr _event, IntPtr parameters)
        {
            FmodService service = _latestService;

            if (service == null)
                return RESULT.ERR_UNSUPPORTED;

            EventInstance instance = new FMOD.Studio.EventInstance(_event);

            if (type == EVENT_CALLBACK_TYPE.STOPPED || type == EVENT_CALLBACK_TYPE.START_FAILED)
            {
                for (int i = 0; i < GHConstants.MaxUISoundInstances; i++)
                {
                    if (i >= service.uiInstances.Count)
                        break;

                    if (service.uiInstances[i].instance.handle == instance.handle)
                    {
                        service.uiInstances[i].stopped = true;
                        return RESULT.OK;
                    }
                }
            }
            return RESULT.OK;
        }

        /* Returns to game thread */
        public static RESULT GNHImmediateEventCallback(EVENT_CALLBACK_TYPE type, IntPtr _event, IntPtr parameters)
        {
            FmodService service = _latestService;

            if (service == null)
                return RESULT.ERR_UNSUPPORTED;

            EventInstance instance = new FMOD.Studio.EventInstance(_event);

            if (type == EVENT_CALLBACK_TYPE.STOPPED || type == EVENT_CALLBACK_TYPE.START_FAILED)
            {
                //lock (service._eventInstanceLock)
                {
                    for (int i = 0; i < GHConstants.MaxNormalImmediateSoundInstances; i++)
                    {
                        if (i >= service.immediateInstances.Count)
                            break;

                        if (service.immediateInstances[i].instance.handle == instance.handle)
                        {
                            service.immediateInstances[i].stopped = true;
                            return RESULT.OK;
                        }
                    }
                    for (int i = 0; i < GHConstants.MaxLongImmediateSoundInstances; i++)
                    {
                        if (i >= service.longImmediateInstances.Count)
                            break;

                        if (service.longImmediateInstances[i].instance.handle == instance.handle)
                        {
                            service.longImmediateInstances[i].stopped = true;
                            return RESULT.OK;
                        }
                    }
                }
            }
            return RESULT.OK;
        }

        /* Returns to game thread */
        public static RESULT GNHDialogueEventCallback(EVENT_CALLBACK_TYPE type, IntPtr _event, IntPtr parameters)
        {
            RESULT result;
            FmodService service = _latestService;
            EventInstance instance = new FMOD.Studio.EventInstance(_event);
            if (service == null)
                return RESULT.ERR_UNSUPPORTED;

            if (type == EVENT_CALLBACK_TYPE.STOPPED || type == EVENT_CALLBACK_TYPE.START_FAILED)
            {
                //lock (service._eventInstanceLock)
                {
                    for (int i = 0; i < GHConstants.MaxNormalImmediateSoundInstances; i++)
                    {
                        if (i >= service.immediateInstances.Count)
                            break;

                        if (service.immediateInstances[i].instance.handle == instance.handle)
                        {
                            service.immediateInstances[i].stopped = true;
                            if (service.immediateInstances[i].sound_type == immediate_sound_types.IMMEDIATE_SOUND_DIALOGUE)
                            {
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
                                service.SetQuieterMode(false);
                                break;
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                    for (int i = 0; i < GHConstants.MaxLongImmediateSoundInstances; i++)
                    {
                        if (i >= service.longImmediateInstances.Count)
                            break;

                        if (service.longImmediateInstances[i].instance.handle == instance.handle)
                        {
                            service.longImmediateInstances[i].stopped = true;
                            if (service.longImmediateInstances[i].sound_type == immediate_sound_types.IMMEDIATE_SOUND_DIALOGUE)
                            {
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
                                service.SetQuieterMode(false);
                                break;
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                }
            }
            return RESULT.OK;
        }

        /* Called from UI thread, no need for locking */
        public int PlayUISound(int ghsound, string eventPath, int bankid, float eventVolume, float soundVolume)
        {
            if (!FMODup())
                return 1;

            EventDescription eventDescription;
            RESULT res = _system.getEvent(eventPath, out eventDescription);
            if (res != RESULT.OK)
                return (int)res;
            EventInstance eventInstance;
            res = eventDescription.createInstance(out eventInstance);
            if (res != RESULT.OK)
                return (int)res;

            res = eventInstance.setCallback(GNHUIEventCallback, EVENT_CALLBACK_TYPE.STOPPED | EVENT_CALLBACK_TYPE.START_FAILED);
            if (res != RESULT.OK)
                return (int)res;
            res = eventInstance.setVolume(Math.Max(0.0f, Math.Min(1.0f, eventVolume * soundVolume * GeneralVolume * _uiVolume)));
            if (res != RESULT.OK)
                return (int)res;

            GHSoundInstance ghinstance = new GHSoundInstance();
            ghinstance.instance = eventInstance;
            ghinstance.ghsound = ghsound;
            ghinstance.normalEventVolume = eventVolume;
            ghinstance.normalSoundVolume = soundVolume;
            ghinstance.sound_type = immediate_sound_types.IMMEDIATE_SOUND_UI;
            ghinstance.dialogue_mid = 0;
            ghinstance.queued = false;
            ghinstance.stopped = false;
            uiInstances.Insert(0, ghinstance);

            if (uiInstances.Count > GHConstants.MaxUISoundInstances)
            {
                GHSoundInstance ghsi = uiInstances[uiInstances.Count - 1];
                if (ghsi != null)
                {
                    if (ghsi.stopped == false)
                    {
                        ghsi.stopped = true;
                        ghsi.instance.stop(FMOD.Studio.STOP_MODE.ALLOWFADEOUT);
                    }
                    ghsi.instance.release();
                }
                uiInstances.RemoveAt(uiInstances.Count - 1);
            }

            res = eventInstance.start();
            if (res != RESULT.OK)
                return (int)res;
            res = _system.update();
            if (res != RESULT.OK)
                return (int)res;

            return 0;
        }

        /* Called from game thread */
        public int PlayImmediateSound(int ghsound, string eventPath, int bankid, float eventVolume, float soundVolume, string[] parameterNames, float[] parameterValues, int arraysize, int sound_type, int play_group, uint dialogue_mid, uint play_flags)
        {
            if (!FMODup())
                return 1;

            RESULT res;
            EventInstance eventInstance;
            bool queue_sound = false;
            //lock (_eventInstanceLock)
            {
                /* Decline to play if no play play_flag is set and the sound is playing */
                if ((play_flags & (uint)sound_play_flags.PLAY_FLAGS_NO_PLAY_IF_ALREADY_PLAYING_OR_QUEUED) != 0)
                {
                    List<GHSoundInstance> soundlist = play_group == (uint)sound_play_groups.SOUND_PLAY_GROUP_LONG ? longImmediateInstances : immediateInstances;
                    bool include_playing = (play_flags & (uint)sound_play_flags.PLAY_FLAGS_NO_PLAY_IF_ALREADY_PLAYING) != 0;
                    bool include_queued = (play_flags & (uint)sound_play_flags.PLAY_FLAGS_NO_PLAY_IF_ALREADY_QUEUED) != 0;
                    foreach (GHSoundInstance ghsi in soundlist)
                    {
                        if (!ghsi.stopped && ghsi.normalSoundVolume > 0.0f && ghsi.ghsound == ghsound
                            && ghsi.dialogue_mid == dialogue_mid && (int)ghsi.sound_type == sound_type
                            && ((include_queued && ghsi.queued) || (include_playing && !ghsi.queued)))
                        {
                            return 0;
                        }
                    }
                }

                EventDescription eventDescription;
                res = _system.getEvent(eventPath, out eventDescription);
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

                float relevant_volume = sound_type == (int)immediate_sound_types.IMMEDIATE_SOUND_UI ? _gameUiVolume : sound_type == (int)immediate_sound_types.IMMEDIATE_SOUND_DIALOGUE ? _dialogueVolume : _effectsVolume;

                if (play_group == (int)sound_play_groups.SOUND_PLAY_GROUP_LONG)
                {
                    queue_sound = false;
                    for (int j = 0; j < longImmediateInstances.Count; j++)
                    {
                        if (sound_type == (int)immediate_sound_types.IMMEDIATE_SOUND_DIALOGUE && longImmediateInstances[j].sound_type == immediate_sound_types.IMMEDIATE_SOUND_DIALOGUE && !longImmediateInstances[j].stopped)
                        {
                            queue_sound = true;
                            break;
                        }
                    }
                    longImmediateInstances.Insert(0, ghinstance);

                    if (sound_type == (int)immediate_sound_types.IMMEDIATE_SOUND_DIALOGUE)
                        res = longImmediateInstances[0].instance.setCallback(GNHDialogueEventCallback, EVENT_CALLBACK_TYPE.STOPPED | EVENT_CALLBACK_TYPE.START_FAILED);
                    else
                        res = longImmediateInstances[0].instance.setCallback(GNHImmediateEventCallback, EVENT_CALLBACK_TYPE.STOPPED | EVENT_CALLBACK_TYPE.START_FAILED);

                    /* Fallback if queued for too long */
                    if (longImmediateInstances.Count >= GHConstants.MaxLongImmediateSoundInstances && longImmediateInstances[GHConstants.MaxLongImmediateSoundInstances - 1] != null && longImmediateInstances[GHConstants.MaxLongImmediateSoundInstances - 1].queued && !longImmediateInstances[GHConstants.MaxLongImmediateSoundInstances - 1].stopped)
                    {
                        longImmediateInstances[GHConstants.MaxLongImmediateSoundInstances - 1].queued = false;
                        res = longImmediateInstances[GHConstants.MaxLongImmediateSoundInstances - 1].instance.start();
                    }
                }
                else
                {
                    queue_sound = false;
                    for (int j = 0; j < immediateInstances.Count; j++)
                    {
                        if (sound_type == (int)immediate_sound_types.IMMEDIATE_SOUND_DIALOGUE && immediateInstances[j].sound_type == immediate_sound_types.IMMEDIATE_SOUND_DIALOGUE && !immediateInstances[j].stopped)
                        {
                            queue_sound = true;
                            break;
                        }
                    }

                    immediateInstances.Insert(0, ghinstance);

                    if (sound_type == (int)immediate_sound_types.IMMEDIATE_SOUND_DIALOGUE)
                        res = immediateInstances[0].instance.setCallback(GNHDialogueEventCallback, EVENT_CALLBACK_TYPE.STOPPED | EVENT_CALLBACK_TYPE.START_FAILED);
                    else
                        res = immediateInstances[0].instance.setCallback(GNHImmediateEventCallback, EVENT_CALLBACK_TYPE.STOPPED | EVENT_CALLBACK_TYPE.START_FAILED);

                    /* Fallback if queued for too long */
                    if (immediateInstances.Count >= GHConstants.MaxNormalImmediateSoundInstances && immediateInstances[GHConstants.MaxNormalImmediateSoundInstances - 1] != null && immediateInstances[GHConstants.MaxNormalImmediateSoundInstances - 1].queued && !immediateInstances[GHConstants.MaxNormalImmediateSoundInstances - 1].stopped)
                    {
                        immediateInstances[GHConstants.MaxNormalImmediateSoundInstances - 1].queued = false;
                        res = immediateInstances[GHConstants.MaxNormalImmediateSoundInstances - 1].instance.start(); ;
                    }
                }

                ghinstance.queued = queue_sound;

                res = eventInstance.setVolume(Math.Max(0.0f, Math.Min(1.0f, eventVolume * soundVolume * GeneralVolume * relevant_volume)));
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
                        if (ghsi != null)
                        {
                            if (ghsi.stopped == false)
                            {
                                ghsi.stopped = true;
                                ghsi.instance.stop(FMOD.Studio.STOP_MODE.ALLOWFADEOUT);
                            }
                            ghsi.instance.release();
                        }
                        longImmediateInstances.RemoveAt(longImmediateInstances.Count - 1);
                    }
                }
                else
                {
                    if (immediateInstances.Count > GHConstants.MaxNormalImmediateSoundInstances)
                    {
                        GHSoundInstance ghsi = immediateInstances[immediateInstances.Count - 1];
                        if (ghsi != null)
                        {
                            if (ghsi.stopped == false)
                            {
                                ghsi.stopped = true;
                                ghsi.instance.stop(FMOD.Studio.STOP_MODE.ALLOWFADEOUT);
                            }
                            ghsi.instance.release();
                        }
                        immediateInstances.RemoveAt(immediateInstances.Count - 1);
                    }
                }

            }

            if (!queue_sound)
            {
                if (sound_type == (int)immediate_sound_types.IMMEDIATE_SOUND_DIALOGUE)
                    SetQuieterMode(true);
                res = eventInstance.start();
            }

            res = _system.update();
            return (int)res;
        }

        public void StopAllUISounds()
        {
            if (!FMODup())
                return;

            foreach (GHSoundInstance ghsi in uiInstances)
            {
                if (ghsi.stopped == false)
                {
                    ghsi.stopped = true;
                    ghsi.instance.stop(FMOD.Studio.STOP_MODE.ALLOWFADEOUT);
                }
                ghsi.instance.release();
            }
            uiInstances.Clear();

            foreach (GHSoundInstance ghsi in uiMusicInstances)
            {
                if (ghsi.stopped == false)
                {
                    ghsi.stopped = true;
                    ghsi.instance.stop(FMOD.Studio.STOP_MODE.ALLOWFADEOUT);
                }
                ghsi.instance.release();
            }
            uiMusicInstances.Clear();
        }

        public int StopAllGameSounds(ulong flags, uint dialogue_mid)
        {
            if (!FMODup())
                return 1;

            RESULT res = 0;
            if (flags == 0)
                flags = (ulong)StopSoundFlags.All;

            bool only_mid = (flags & (ulong)StopSoundFlags.OnlyDialogueMid) != 0;
            //lock (_eventInstanceLock)
            {
                if ((flags & (ulong)StopSoundFlags.ImmediateNormal) != 0)
                {
                    foreach (GHSoundInstance ghsi in immediateInstances)
                    {
                        if (!only_mid || (only_mid && dialogue_mid == ghsi.dialogue_mid))
                        {
                            if (ghsi.stopped == false)
                            {
                                ghsi.stopped = true;
                                ghsi.instance.stop(FMOD.Studio.STOP_MODE.ALLOWFADEOUT);
                            }
                            ghsi.instance.release();
                        }
                    }
                    /* Callback removes the sound instance */
                    //for (int i = immediateInstances.Count - 1; i >= 0; i--)
                    //{
                    //    if (immediateInstances[i].stopped)
                    //        immediateInstances.RemoveAt(i);
                    //}
                    //immediateInstances.Clear();
                }

                if ((flags & (ulong)StopSoundFlags.ImmediateLong) != 0)
                {
                    foreach (GHSoundInstance ghsi in longImmediateInstances)
                    {
                        if (!only_mid || (only_mid && dialogue_mid == ghsi.dialogue_mid))
                        {
                            if (ghsi.stopped == false)
                            {
                                ghsi.stopped = true;
                                ghsi.instance.stop(FMOD.Studio.STOP_MODE.ALLOWFADEOUT);
                            }
                            ghsi.instance.release();
                        }
                    }
                    /* Callback removes the sound instance */
                    //for (int i = longImmediateInstances.Count - 1; i >= 0; i--)
                    //{
                    //    if (longImmediateInstances[i].stopped)
                    //        longImmediateInstances.RemoveAt(i);
                    //}
                    //longImmediateInstances.Clear();
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

                if ((flags & (ulong)StopSoundFlags.LevelAmbient) != 0)
                {
                    foreach (GHSoundInstance ghsi in levelAmbientInstances)
                    {
                        if (ghsi.stopped == false)
                        {
                            ghsi.stopped = true;
                            ghsi.instance.stop(FMOD.Studio.STOP_MODE.ALLOWFADEOUT);
                        }
                        ghsi.instance.release();
                    }
                    levelAmbientInstances.Clear();
                }

                if ((flags & (ulong)StopSoundFlags.EnvironmentAmbient) != 0)
                {
                    foreach (GHSoundInstance ghsi in environmentAmbientInstances)
                    {
                        if (ghsi.stopped == false)
                        {
                            ghsi.stopped = true;
                            ghsi.instance.stop(FMOD.Studio.STOP_MODE.ALLOWFADEOUT);
                        }
                        ghsi.instance.release();
                    }
                    environmentAmbientInstances.Clear();
                }

                if ((flags & (ulong)StopSoundFlags.OccupationAmbient) != 0)
                {
                    foreach (GHSoundInstance ghsi in occupationAmbientInstances)
                    {
                        if (ghsi.stopped == false)
                        {
                            ghsi.stopped = true;
                            ghsi.instance.stop(FMOD.Studio.STOP_MODE.ALLOWFADEOUT);
                        }
                        ghsi.instance.release();
                    }
                    occupationAmbientInstances.Clear();
                }

                if ((flags & (ulong)StopSoundFlags.EffectAmbient) != 0)
                {
                    foreach (GHSoundInstance ghsi in effectAmbientInstances)
                    {
                        if (ghsi.stopped == false)
                        {
                            ghsi.stopped = true;
                            ghsi.instance.stop(FMOD.Studio.STOP_MODE.ALLOWFADEOUT);
                        }
                        ghsi.instance.release();
                    }
                    effectAmbientInstances.Clear();
                }

                if ((flags & (ulong)StopSoundFlags.SoundSources) != 0)
                {
                    foreach (GHSoundInstance ghsi in ambientList)
                    {
                        if (!only_mid || (only_mid && dialogue_mid == ghsi.dialogue_mid))
                        {
                            if (!ghsi.stopped && ghsi.normalSoundVolume > 0.0f)
                            {
                                SetAmbientSoundVolume(ghsi.guid, 0.0f);
                            }
                        }
                    }
                }
            }

            res = _system.update();
            return (int)res;
        }

        public int PlayUIMusic(int ghsound, string eventPath, int bankid, float eventVolume, float soundVolume)
        {
            return PlayMusicCore(uiMusicInstances, ghsound, eventPath, bankid, eventVolume, soundVolume);
        }

        public int PlayMusic(int ghsound, string eventPath, int bankid, float eventVolume, float soundVolume)
        {
            return PlayMusicCore(musicInstances, ghsound, eventPath, bankid, eventVolume, soundVolume);
        }

        public int PlayMusicCore(List<GHSoundInstance> musicList, int ghsound, string eventPath, int bankid, float eventVolume, float soundVolume)
        {
            if (!FMODup())
                return 1;

            RESULT res;
            EventInstance eventInstance;
            //lock (_eventInstanceLock)
            {
                if (musicList.Count > 0 && musicList[0].ghsound == ghsound && musicList[0].stopped == false)
                    return (int)RESULT.OK;

                if (musicList.Count > 0)
                {
                    if (musicList[0].stopped == false)
                    {
                        musicList[0].stopped = true;
                        musicList[0].instance.stop(FMOD.Studio.STOP_MODE.ALLOWFADEOUT);
                    }
                }

                EventDescription eventDescription;
                res = _system.getEvent(eventPath, out eventDescription);
                res = eventDescription.createInstance(out eventInstance);

                GHSoundInstance ghinstance = new GHSoundInstance();
                ghinstance.instance = eventInstance;
                ghinstance.ghsound = ghsound;
                ghinstance.normalEventVolume = eventVolume;
                ghinstance.normalSoundVolume = soundVolume;
                ghinstance.sound_type = 0;
                ghinstance.dialogue_mid = 0;

                musicList.Insert(0, ghinstance);
                res = eventInstance.setVolume(Math.Max(0.0f, Math.Min(1.0f, eventVolume * soundVolume * GeneralVolume * MusicVolume * ModeVolume)));

                if (musicList.Count >= 2)
                {
                    GHSoundInstance ghsi = musicList[musicList.Count - 1];
                    if (ghsi.stopped == false)
                    {
                        ghsi.stopped = true;
                        ghsi.instance.stop(FMOD.Studio.STOP_MODE.ALLOWFADEOUT);
                    }
                    musicList.RemoveAt(musicList.Count - 1);
                }
            }
            res = eventInstance.start();
            //}
            res = _system.update();
            return (int)res;
        }

        public int PlayLevelAmbient(int ghsound, string eventPath, int bankid, float eventVolume, float soundVolume)
        {
            if (!FMODup())
                return 1;

            RESULT res;
            EventInstance eventInstance;
            //lock (_eventInstanceLock)
            {
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
                        res = levelAmbientInstances[0].instance.setVolume(Math.Max(0.0f, Math.Min(1.0f, eventVolume * soundVolume * GeneralVolume * _ambientVolume * ModeVolume)));
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
                res = eventDescription.createInstance(out eventInstance);

                GHSoundInstance ghinstance = new GHSoundInstance();
                ghinstance.instance = eventInstance;
                ghinstance.ghsound = ghsound;
                ghinstance.normalEventVolume = eventVolume;
                ghinstance.normalSoundVolume = soundVolume;
                ghinstance.sound_type = 0;
                ghinstance.dialogue_mid = 0;

                levelAmbientInstances.Insert(0, ghinstance);
                res = eventInstance.setVolume(Math.Max(0.0f, Math.Min(1.0f, eventVolume * soundVolume * _ambientVolume * GeneralVolume * ModeVolume)));

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
            }

            res = eventInstance.start();
            res = _system.update();
            return (int)res;
        }

        public int PlayEnvironmentAmbient(int ghsound, string eventPath, int bankid, float eventVolume, float soundVolume)
        {
            if (!FMODup())
                return 1;

            RESULT res;
            EventInstance eventInstance;
            //lock (_eventInstanceLock)
            {
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
                        res = environmentAmbientInstances[0].instance.setVolume(Math.Max(0.0f, Math.Min(1.0f, eventVolume * soundVolume * _ambientVolume * GeneralVolume * ModeVolume)));
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
                res = eventDescription.createInstance(out eventInstance);

                GHSoundInstance ghinstance = new GHSoundInstance();
                ghinstance.instance = eventInstance;
                ghinstance.ghsound = ghsound;
                ghinstance.normalEventVolume = eventVolume;
                ghinstance.normalSoundVolume = soundVolume;
                ghinstance.sound_type = 0;
                ghinstance.dialogue_mid = 0;

                environmentAmbientInstances.Insert(0, ghinstance);
                res = eventInstance.setVolume(Math.Max(0.0f, Math.Min(1.0f, eventVolume * soundVolume * _ambientVolume * GeneralVolume * ModeVolume)));

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
            }

            res = eventInstance.start();
            res = _system.update();
            return (int)res;
        }

        public int PlayOccupationAmbient(int ghsound, string eventPath, int bankid, float eventVolume, float soundVolume)
        {
            if (!FMODup())
                return 1;

            RESULT res;
            EventInstance eventInstance;
            //lock (_eventInstanceLock)
            {
                if (ghsound <= 0)
                {
                    if (occupationAmbientInstances.Count > 0 && occupationAmbientInstances[0].stopped == false)
                    {
                        /* Stop ambient sound */
                        occupationAmbientInstances[0].stopped = true;
                        res = occupationAmbientInstances[0].instance.stop(FMOD.Studio.STOP_MODE.ALLOWFADEOUT);
                        if (res != RESULT.OK)
                            return (int)res;
                        res = _system.update();
                        return (int)res;
                    }

                    /* Nothing to do */
                    return (int)RESULT.OK;
                }

                /* Check if the ambient is the same as before */
                if (occupationAmbientInstances.Count > 0 && occupationAmbientInstances[0].ghsound == ghsound && occupationAmbientInstances[0].stopped == false)
                {
                    /* Check if the volume is different */
                    if (occupationAmbientInstances[0].normalSoundVolume != soundVolume)
                    {
                        res = occupationAmbientInstances[0].instance.setVolume(Math.Max(0.0f, Math.Min(1.0f, eventVolume * soundVolume * _ambientVolume * GeneralVolume * ModeVolume)));
                        occupationAmbientInstances[0].normalSoundVolume = soundVolume;
                        res = _system.update();
                        return (int)res;
                    }
                    return (int)RESULT.OK;
                }

                /* Different ambient; stop the previous one */
                if (occupationAmbientInstances.Count > 0)
                {
                    if (occupationAmbientInstances[0].stopped == false)
                    {
                        occupationAmbientInstances[0].stopped = true;
                        occupationAmbientInstances[0].instance.stop(FMOD.Studio.STOP_MODE.ALLOWFADEOUT);
                    }
                }

                EventDescription eventDescription;
                res = _system.getEvent(eventPath, out eventDescription);
                res = eventDescription.createInstance(out eventInstance);

                GHSoundInstance ghinstance = new GHSoundInstance();
                ghinstance.instance = eventInstance;
                ghinstance.ghsound = ghsound;
                ghinstance.normalEventVolume = eventVolume;
                ghinstance.normalSoundVolume = soundVolume;
                ghinstance.sound_type = 0;
                ghinstance.dialogue_mid = 0;

                occupationAmbientInstances.Insert(0, ghinstance);
                res = eventInstance.setVolume(Math.Max(0.0f, Math.Min(1.0f, eventVolume * soundVolume * _ambientVolume * GeneralVolume * ModeVolume)));

                if (occupationAmbientInstances.Count >= 2)
                {
                    GHSoundInstance ghsi = occupationAmbientInstances[occupationAmbientInstances.Count - 1];
                    if (ghsi.stopped == false)
                    {
                        ghsi.stopped = true;
                        ghsi.instance.stop(FMOD.Studio.STOP_MODE.ALLOWFADEOUT);
                    }
                    occupationAmbientInstances.RemoveAt(occupationAmbientInstances.Count - 1);
                }
            }

            res = eventInstance.start();
            res = _system.update();
            return (int)res;
        }

        public int PlayEffectAmbient(int ghsound, string eventPath, int bankid, float eventVolume, float soundVolume)
        {
            if (!FMODup())
                return 1;

            RESULT res;
            EventInstance eventInstance;
            //lock (_eventInstanceLock)
            {
                if (ghsound <= 0)
                {
                    if (effectAmbientInstances.Count > 0 && effectAmbientInstances[0].stopped == false)
                    {
                        /* Stop ambient sound */
                        effectAmbientInstances[0].stopped = true;
                        res = effectAmbientInstances[0].instance.stop(FMOD.Studio.STOP_MODE.ALLOWFADEOUT);
                        if (res != RESULT.OK)
                            return (int)res;
                        res = _system.update();
                        return (int)res;
                    }

                    /* Nothing to do */
                    return (int)RESULT.OK;
                }

                /* Check if the ambient is the same as before */
                if (effectAmbientInstances.Count > 0 && effectAmbientInstances[0].ghsound == ghsound && effectAmbientInstances[0].stopped == false)
                {
                    /* Check if the volume is different */
                    if (effectAmbientInstances[0].normalSoundVolume != soundVolume)
                    {
                        res = effectAmbientInstances[0].instance.setVolume(Math.Max(0.0f, Math.Min(1.0f, eventVolume * soundVolume * _ambientVolume * GeneralVolume * ModeVolume)));
                        effectAmbientInstances[0].normalSoundVolume = soundVolume;
                        res = _system.update();
                        return (int)res;
                    }
                    return (int)RESULT.OK;
                }

                /* Different ambient; stop the previous one */
                if (effectAmbientInstances.Count > 0)
                {
                    if (effectAmbientInstances[0].stopped == false)
                    {
                        effectAmbientInstances[0].stopped = true;
                        effectAmbientInstances[0].instance.stop(FMOD.Studio.STOP_MODE.ALLOWFADEOUT);
                    }
                }

                EventDescription eventDescription;
                res = _system.getEvent(eventPath, out eventDescription);
                res = eventDescription.createInstance(out eventInstance);

                GHSoundInstance ghinstance = new GHSoundInstance();
                ghinstance.instance = eventInstance;
                ghinstance.ghsound = ghsound;
                ghinstance.normalEventVolume = eventVolume;
                ghinstance.normalSoundVolume = soundVolume;
                ghinstance.sound_type = 0;
                ghinstance.dialogue_mid = 0;

                effectAmbientInstances.Insert(0, ghinstance);
                res = eventInstance.setVolume(Math.Max(0.0f, Math.Min(1.0f, eventVolume * soundVolume * _ambientVolume * GeneralVolume * ModeVolume)));

                if (effectAmbientInstances.Count >= 2)
                {
                    GHSoundInstance ghsi = effectAmbientInstances[effectAmbientInstances.Count - 1];
                    if (ghsi.stopped == false)
                    {
                        ghsi.stopped = true;
                        ghsi.instance.stop(FMOD.Studio.STOP_MODE.ALLOWFADEOUT);
                    }
                    effectAmbientInstances.RemoveAt(effectAmbientInstances.Count - 1);
                }
            }

            res = eventInstance.start();
            res = _system.update();
            return (int)res;
        }


        public int SetEffectAmbientVolume(float soundVolume)
        {
            if (!FMODup())
                return 1;

            /* Check if the ambient is the same as before */
            //lock (_eventInstanceLock)
            {
                if (effectAmbientInstances.Count > 0 && effectAmbientInstances[0].stopped == false)
                {
                    RESULT res;
                    /* Check if the volume is different */
                    if (effectAmbientInstances[0].normalSoundVolume != soundVolume)
                    {
                        float eventVolume = effectAmbientInstances[0].normalEventVolume;
                        res = effectAmbientInstances[0].instance.setVolume(Math.Max(0.0f, Math.Min(1.0f, eventVolume * soundVolume * _ambientVolume * GeneralVolume * ModeVolume)));
                        effectAmbientInstances[0].normalSoundVolume = soundVolume;
                        res = _system.update();
                        return (int)res;
                    }
                    return (int)RESULT.OK;
                }
            }

            return (int)RESULT.OK;
        }

        private UInt64 _lastAmbientId = 0;

        private UInt64 GetNewAmbientId()
        {
            _lastAmbientId++;
            if (_lastAmbientId == 0)
                _lastAmbientId++;

            return _lastAmbientId;
        }

        public int AddAmbientSound(int ghsound, string eventPath, int bankid, float eventVolume, float soundVolume, out UInt64 soundSourceId)
        {
            if (!FMODup())
            {
                soundSourceId = 0;
                return 1;
            }

            if (eventPath == null || eventPath == "")
            {
                soundSourceId = 0;
                return 1;
            }

            RESULT res = RESULT.OK;
            EventInstance eventInstance;
            GHSoundInstance ghinstance;
            //lock (_eventInstanceLock)
            {
                EventDescription eventDescription;
                res = _system.getEvent(eventPath, out eventDescription);
                res = eventDescription.createInstance(out eventInstance);

                /* Set volume */
                res = eventInstance.setVolume(Math.Max(0.0f, Math.Min(1.0f, eventVolume * soundVolume * _ambientVolume * GeneralVolume * ModeVolume)));
                //ambientInstance->setVolume(fmod_volume* event_volume * general_ambient_volume* general_volume);

                /* Create new GHSoundInstance */
                ghinstance = new GHSoundInstance();
                ghinstance.instance = eventInstance;
                ghinstance.ghsound = ghsound;
                ghinstance.normalEventVolume = eventVolume;
                ghinstance.normalSoundVolume = soundVolume;
                ghinstance.sound_type = 0;
                ghinstance.dialogue_mid = 0;
                ghinstance.guid = GetNewAmbientId();

                ambientList.Insert(0, ghinstance);
            }

            /* Play sound */
            res = eventInstance.start();
            res = _system.update();

            soundSourceId = ghinstance.guid;

            return (int)res;

        }
        public int DeleteAmbientSound(UInt64 soundSourceId)
        {
            if (!FMODup())
                return 1;

            if (soundSourceId == 0)
                return 1;

            RESULT res;
            bool found = false;
            GHSoundInstance ghinstance = null;
            int listidx = -1;
            //lock (_eventInstanceLock)
            {
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
                ambientList.RemoveAt(listidx);
                if (ghinstance.stopped == false)
                {
                    ghinstance.stopped = true;
                    ghinstance.instance.stop(FMOD.Studio.STOP_MODE.ALLOWFADEOUT);
                }
            }

            res = _system.update();
            res = ghinstance.instance.release();
            return 0;
        }
        public int SetAmbientSoundVolume(UInt64 soundSourceId, float soundVolume)
        {
            if (!FMODup())
                return 1;

            if (soundSourceId == 0)
                return 1;

            if (soundVolume < 0.0f || soundVolume > 1.0f)
                return 1;

            RESULT res;
            GHSoundInstance ghinstance = null;
            float event_volume;
            //lock (_eventInstanceLock)
            {
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
                event_volume = ghinstance.normalEventVolume;
                ghinstance.normalSoundVolume = soundVolume;
            }

            float old_volume;
            res = ghinstance.instance.getVolume(out old_volume);
            res = ghinstance.instance.setVolume(event_volume * soundVolume  * _ambientVolume * GeneralVolume * ModeVolume);

            if (old_volume == 0.0f && soundVolume > 0.0f)
                res = ghinstance.instance.start();
            else if (old_volume > 0.0f && soundVolume == 0.0f)
                res = ghinstance.instance.stop(FMOD.Studio.STOP_MODE.ALLOWFADEOUT);

            res = _system.update();

            return 0;
        }

        public int AdjustGameVolumes(float new_general_volume, float new_general_music_volume, float new_general_ambient_volume, float new_general_dialogue_volume, float new_general_sfx_volume, float new_general_game_ui_volume)
        {
            if (!FMODup())
                return 1;

            /* No need to set again, since AdjustUIVolumes has been called before, setting GeneralVolume and MusicVolume */
            //GeneralVolume = new_general_volume;
            //MusicVolume = new_general_music_volume;

            _ambientVolume = new_general_ambient_volume;
            _dialogueVolume = new_general_dialogue_volume;
            _effectsVolume = new_general_sfx_volume;
            _gameUiVolume = new_general_game_ui_volume;

            RESULT result;
            //lock (_eventInstanceLock)
            {
                result = SetGameMusicAndAmbientVolumesWithoutUpdate(new_general_music_volume, new_general_ambient_volume, new_general_volume);
                result = AdjustImmediateVolumeType(immediateInstances, new_general_volume);
                result = AdjustImmediateVolumeType(longImmediateInstances, new_general_volume);
            }
            result = _system.update();
            return (int)result;
        }

        public int AdjustUIVolumes(float new_general_volume, float new_general_music_volume, float new_general_ui_volume)
        {
            if (!FMODup())
                return 1;

            GeneralVolume = new_general_volume;
            MusicVolume = new_general_music_volume;

            _uiVolume = new_general_ui_volume;

            RESULT result;
            result = AdjustVolumeType(uiMusicInstances, new_general_music_volume, ModeVolume, new_general_volume);
            foreach (GHSoundInstance si in uiInstances)
            {
                if (!si.stopped)
                {
                    result = si.instance.setVolume(Math.Min(1.0f, si.normalSoundVolume * si.normalEventVolume * _uiVolume * new_general_volume));
                }
            }
            result = _system.update();
            return (int)result;
        }

        public void ToggleMuteSounds(bool mute)
        {
            if (!FMODup())
                return;
            _coresystem.getMasterChannelGroup(out var _masterChannelGroup);
            _masterChannelGroup.setMute(mute);
        }

        private RESULT AdjustVolumeType(List<GHSoundInstance> soundList, float typeVolume, float modeVolume, float generalVolume)
        {
            RESULT result = RESULT.OK;
            foreach (GHSoundInstance si in soundList)
            {
                if (!si.stopped)
                {
                    result = si.instance.setVolume(Math.Min(1.0f, si.normalSoundVolume * si.normalEventVolume * typeVolume * generalVolume * modeVolume));
                }
            }
            return result;
        }

        private RESULT AdjustImmediateVolumeType(List<GHSoundInstance> soundList, float generalVolume)
        {
            RESULT result = RESULT.OK;
            foreach (GHSoundInstance si in soundList)
            {
                if (!si.stopped)
                {
                    float relevant_volume = si.sound_type == immediate_sound_types.IMMEDIATE_SOUND_UI ? _gameUiVolume : si.sound_type == immediate_sound_types.IMMEDIATE_SOUND_DIALOGUE ? _dialogueVolume : _effectsVolume;
                    result = si.instance.setVolume(Math.Min(1.0f, si.normalSoundVolume * si.normalEventVolume * relevant_volume * generalVolume));
                }
            }
            return result;
        }

        private RESULT SetGameMusicAndAmbientVolumesWithoutUpdate(float generalVolume, float musicVolume, float ambientVolume)
        {
            RESULT result;
            float modeVolume = ModeVolume;
            result = AdjustVolumeType(musicInstances, musicVolume, modeVolume, generalVolume);
            result = AdjustVolumeType(levelAmbientInstances, ambientVolume, modeVolume, generalVolume);
            result = AdjustVolumeType(environmentAmbientInstances, ambientVolume, modeVolume, generalVolume);
            result = AdjustVolumeType(occupationAmbientInstances, ambientVolume, modeVolume, generalVolume);
            result = AdjustVolumeType(effectAmbientInstances, ambientVolume, modeVolume, generalVolume);
            result = AdjustVolumeType(ambientList, ambientVolume, modeVolume, generalVolume);
            return result;
        }

        private int AdjustMusicAndAmbientVolumes()
        {
            RESULT result;
            //lock (_eventInstanceLock)
            {
                result = SetGameMusicAndAmbientVolumesWithoutUpdate(GeneralVolume, MusicVolume, _ambientVolume);
            }
            result = _system.update();
            return (int)result;
        }

        //private int ReadjustAllVolumes()
        //{
        //    return AdjustVolumes(_generalVolume, _musicVolume, _ambientVolume, _dialogueVolume, _effectsVolume, _uiVolume);
        //}

        private const int _maxModeFadeCounter = 10;
        private float ModeVolume
        {
            get { return (_quieterMode ? _quietModeMultiplier : 1.0f) + (_quieterMode ? 1.0f : -1.0f) * (1.0f - _quietModeMultiplier) * (float)(_maxModeFadeCounter - ModeFadeCounter) / _maxModeFadeCounter; }
        }

        //private readonly object _modeFadeLock = new object();
        private int _modeFadeCounter = _maxModeFadeCounter;
        private int ModeFadeCounter { get { return Interlocked.CompareExchange(ref _modeFadeCounter, 0, 0); } set { Interlocked.Exchange(ref _modeFadeCounter, value); } }

        private int _quieterModeRequestNumber = 0;

        public int SetQuieterMode(bool state)
        {
            bool prevState = _quieterModeRequestNumber > 0;
            _quieterModeRequestNumber = _quieterModeRequestNumber + (state ? 1 : -1);
            bool newState = _quieterModeRequestNumber > 0;

            //if (_quieterMode == state)
            if (prevState == newState)
                return (int)RESULT.OK;

            _quieterMode = newState; // state;
            ModeFadeCounter = 0;
            _tasks.Enqueue(new GHSoundTask(GHSoundTaskType.IncreaseModeFadeCounter));

            //ModeFadeCounter = _maxModeFadeCounter;
            //AdjustMusicAndAmbientVolumes();
            //ModeFadeCounter = 0;
            //Task.Run(() => 
            //{
            //    for (int i = 0; i < _maxModeFadeCounter; i++)
            //    {
            //        //lock (_modeFadeLock)
            //        {
            //            if (ModeFadeCounter >= _maxModeFadeCounter)
            //            {
            //                ModeFadeCounter = _maxModeFadeCounter;
            //                break;
            //            }
            //        }
            //        Interlocked.Increment(ref _modeFadeCounter);
            //        AdjustMusicAndAmbientVolumes();
            //        System.Threading.Thread.Sleep(25);
            //    }
            //});

            return (int)RESULT.OK; // AdjustMusicAndAmbientVolumes();
        }

        enum GHSoundTaskType
        {
            IncreaseModeFadeCounter,
        }

        private class GHSoundTask
        {
            public GHSoundTaskType TaskType;
            public GHSoundTask(GHSoundTaskType taskType)
            {
                TaskType = taskType;
            }
        }
        private ConcurrentQueue<GHSoundTask> _tasks = new ConcurrentQueue<GHSoundTask>();
        public void PollTasks()
        {
            if (!FMODup())
                return;

            List<GHSoundTask> addedTasks = new List<GHSoundTask>();
            while (_tasks.TryDequeue(out GHSoundTask ghst))
            {
                switch(ghst.TaskType)
                {
                    case GHSoundTaskType.IncreaseModeFadeCounter:
                        {
                            int modeFadeCounter = ModeFadeCounter;
                            if (modeFadeCounter < _maxModeFadeCounter)
                            {
                                modeFadeCounter = Interlocked.Increment(ref _modeFadeCounter);
                                AdjustMusicAndAmbientVolumes();
                                if (modeFadeCounter < _maxModeFadeCounter)
                                {
                                    addedTasks.Add(new GHSoundTask(GHSoundTaskType.IncreaseModeFadeCounter));
                                }
                            }
                        }
                        break;
                    default:
                        break;
                }
            }

            if (addedTasks.Count > 0)
            {
                foreach (GHSoundTask ghst in addedTasks)
                {
                    _tasks.Enqueue(ghst);
                }
            }
        }

        public uint GetVersionCode()
        {
            if (!FMODup())
                return 0;

            uint ver = 0;
            RESULT res = _coresystem.getVersion(out ver);
            if (res != RESULT.OK)
                return 0;

            return ver;
        }
        public string GetVersionString()
        {
            uint ver = GetVersionCode();
            if (ver == 0)
                return "";

            uint majorversion = (ver >> 16) & 0xFFFFU;
            uint minorversion = (ver >> 8) & 0xFFU;
            uint patchversion = ver & 0xFFU;

            string str = majorversion.ToString("X") + "." + minorversion.ToString("X") + "." + patchversion.ToString("X");
            return str;
        }
    }
}