using SkiaSharp;
#if GNH_MAUI
using GnollHackM;
#else
using Xamarin.Essentials;
using Xamarin.Forms;
using GnollHackX.Pages.Game;
#endif
using Newtonsoft.Json;
using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.IO.Compression;
using System.IO;
using System.Reflection;
using System.Threading.Tasks;
using System.Collections.Concurrent;
using System.Net.Http.Headers;
using System.Net.Http;
using System.Text;
using System.Threading;
using System.Net;
using System.Net.Mail;
using System.Linq;

namespace GnollHackX
{
    public delegate Task<bool> BackButtonHandler(object sender, EventArgs e);
    public struct SendResult
    {
        public bool IsSuccess;
        public bool HasHttpStatusCode;
        public HttpStatusCode StatusCode;
        public string Message;
    }


    public static class GHApp
    {
        public static void Initialize()
        {
            VersionTracking.Track();
            GetDependencyServices();
            PlatformService.InitializePlatform();

            _batteryChargeLevel = Battery.ChargeLevel;
            _batteryChargeLevelTimeStamp = DateTime.Now;
            Battery.BatteryInfoChanged += Battery_BatteryInfoChanged;

            TotalMemory = GHApp.PlatformService.GetDeviceMemoryInBytes();

            Assembly assembly = typeof(App).GetTypeInfo().Assembly;
            InitBaseTypefaces(assembly);
            InitBaseCachedBitmaps(assembly);

            ButtonNormalImageSource = ImageSource.FromResource(AppResourceName + ".Assets.button_normal.png", assembly);
            ButtonSelectedImageSource = ImageSource.FromResource(AppResourceName + ".Assets.button_selected.png", assembly);
            ButtonDisabledImageSource = ImageSource.FromResource(AppResourceName + ".Assets.button_disabled.png", assembly);

            HideAndroidNavigationBar = Preferences.Get("HideAndroidNavigationBar", GHConstants.DefaultHideNavigation);
            HideiOSStatusBar = Preferences.Get("HideiOSStatusBar", GHConstants.DefaultHideStatusBar);
            DeveloperMode = Preferences.Get("DeveloperMode", GHConstants.DefaultDeveloperMode);
            DebugLogMessages = Preferences.Get("DebugLogMessages", GHConstants.DefaultLogMessages);
            FullVersionMode = true; // Preferences.Get("FullVersion", true);
            ClassicMode = Preferences.Get("ClassicMode", false);
            CasualMode = Preferences.Get("CasualMode", false);
            LoadBanks = Preferences.Get("LoadSoundBanks", true);
            GameSaveStatus = Preferences.Get("GameSaveResult", 0);
            InformAboutGameTermination = Preferences.Get("WentToSleepWithGameOn", false);
            Preferences.Set("WentToSleepWithGameOn", false);
            Preferences.Set("GameSaveResult", 0);
            InformAboutCrashReport = !InformAboutGameTermination;
            PostingGameStatus = Preferences.Get("PostingGameStatus", GHConstants.DefaultPosting);
            PostingDiagnosticData = Preferences.Get("PostingDiagnosticData", GHConstants.DefaultPosting);
            PostingXlogEntries = Preferences.Get("PostingXlogEntries", GHConstants.DefaultPosting);
            PostingBonesFiles = Preferences.Get("PostingBonesFiles", GHConstants.DefaultPosting);
            CustomGameStatusLink = Preferences.Get("CustomGameStatusLink", "");
            CustomXlogAccountLink = Preferences.Get("CustomXlogAccountLink", "");
            CustomXlogPostLink = Preferences.Get("CustomXlogPostLink", "");
            UseHTMLDumpLogs = Preferences.Get("UseHTMLDumpLogs", GHConstants.DefaultHTMLDumpLogs);
            UseSingleDumpLog = Preferences.Get("UseSingleDumpLog", GHConstants.DefaultUseSingleDumpLog);
            ReadStreamingBankToMemory = Preferences.Get("ReadStreamingBankToMemory", DefaultStreamingBankToMemory);
            CopyStreamingBankToDisk = Preferences.Get("CopyStreamingBankToDisk", GHConstants.DefaultCopyStreamingBankToDisk);
            ForcePostBones = IsDebug && Preferences.Get("ForcePostBones", false);
            AppSwitchSaveStyle = Preferences.Get("AppSwitchSaveStyle", 0);
            XlogUserName = Preferences.Get("XlogUserName", "");
            XlogPassword = Preferences.Get("XlogPassword", "");
            XlogReleaseAccount = Preferences.Get("XlogReleaseAccount", false);
            AllowBones = Preferences.Get("AllowBones", true);

            BackButtonPressed += EmptyBackButtonPressed;
        }

        private static object _networkAccessLock = new object();
        private static NetworkAccess _networkAccessState = NetworkAccess.None;
        public static bool HasInternetAccess { get { lock (_networkAccessLock) { return _networkAccessState == NetworkAccess.Internet; } } }

        public static void InitializeConnectivity()
        {
            try
            {
                _networkAccessState = Connectivity.NetworkAccess;
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
            Connectivity.ConnectivityChanged += Connectivity_ConnectivityChanged;
        }

        private static void Connectivity_ConnectivityChanged(object sender, ConnectivityChangedEventArgs e)
        {
            lock(_networkAccessLock)
            {
                _networkAccessState = e.NetworkAccess;
            }
        }

        public static ulong TotalMemory { get; set; }
        public static bool DefaultStreamingBankToMemory 
        { 
            get 
            {
                return GHConstants.DefaultReadStreamingBankToMemory || 
                    IsDebug && IsAndroid && TotalMemory >= GHConstants.AndroidBanksToMemoryThreshold;
            }
        }

        public static bool IsDebug
        {
            get
            {
#if DEBUG
                return true;
#else
                return false;
#endif
            }
        }

        public static bool BatterySavingMode { get; set; }

        private static double _batteryChargeLevel = -1;
        private static double _previousBatteryChargeLevel = -1;
        private static double _previousBatteryCheckPointChargeLevel = 2; /* Dummy initial value of 200% */
        private static DateTime _batteryChargeLevelTimeStamp;
        private static DateTime _previousBatteryChargeLevelTimeStamp;
        private static readonly object _batteryLock = new object();

        private static void Battery_BatteryInfoChanged(object sender, BatteryInfoChangedEventArgs e)
        {
            double chargediff;
            double prevcheckpointcharge;
            lock (_batteryLock)
            {
                prevcheckpointcharge = _previousBatteryCheckPointChargeLevel;
                _previousBatteryChargeLevel = _batteryChargeLevel;
                _previousBatteryChargeLevelTimeStamp = _batteryChargeLevelTimeStamp;
                _batteryChargeLevel = e.ChargeLevel;
                _batteryChargeLevelTimeStamp = DateTime.Now;
                chargediff = _batteryChargeLevel - _previousBatteryChargeLevel;
            }

            if (chargediff < 0 && CurrentGHGame != null && e.ChargeLevel >= 0.04 && e.ChargeLevel <= 0.06 && e.ChargeLevel - prevcheckpointcharge < -0.0075)
            {
                lock (_batteryLock)
                {
                    _previousBatteryCheckPointChargeLevel = e.ChargeLevel;
                }
                CurrentGHGame.ActiveGamePage.SaveCheckPoint();
            }
        }

        public static double BatteryChargeLevel {  get { lock (_batteryLock) { return _batteryChargeLevel * 100.0; } } }
        public static double BatteryConsumption
        { 
            get
            { 
                lock (_batteryLock)
                {
                    if(_batteryChargeLevel < 0 || _previousBatteryChargeLevel < 0)
                        return 0;
                    TimeSpan ts = _batteryChargeLevelTimeStamp - _previousBatteryChargeLevelTimeStamp;
                    double ms = ts.TotalMilliseconds;
                    if (ms > 0)
                    {
                        return (_previousBatteryChargeLevel - _batteryChargeLevel) * 100.0 * 1000.0 * 60.0 * 60.0 / ms;
                    }
                    else
                        return 0;
                } 
            } 
        }

#if GNH_MAUI
        public const string AppResourceName = "GnollHackM";
#else
        public const string AppResourceName = "GnollHackX";
#endif
        public static MainPage CurrentMainPage { get; set; }
        public static GamePage CurrentGamePage { get; set; }

        private static readonly object _currentGHGameLock = new object();
        private static GHGame _currentGHGame = null;
        public static GHGame CurrentGHGame { get { lock (_currentGHGameLock) { return _currentGHGame; } } set { lock (_currentGHGameLock) { _currentGHGame = value; } } }
        public static int GameSaveStatus = 0;
        public static bool InformAboutGameTermination = false;
        public static bool InformAboutCrashReport = false;
        public static bool InformAboutIncompatibleSavedGames = false;
        public static bool SavedLongerMessageHistory { get; set; }

        public static bool InformAboutSlowSounds
        {
            get
            {
                return IsAndroid && IsDebug && LoadBanks && !ReadStreamingBankToMemory;
            }
        }

        public static bool IsGPUDefault
        {
            get
            {
#if GNH_MAUI
                return false;
#else
                string manufacturer = DeviceInfo.Manufacturer;
                string model = DeviceInfo.Model;
                bool isGooglePixel6orGreater = false;
                if (manufacturer != null && model != null && model.Length >= 7 &&
                    manufacturer.ToLower() == "google" && model.Substring(0, 6).ToLower() == "pixel ")
                {
                    int pixelver;
                    string endstr = model.Substring(6);
                    int cnt = 0;
                    foreach (char c in endstr)
                    {
                        if (c < '0' || c > '9')
                            break;
                        cnt++;
                    }
                    isGooglePixel6orGreater = cnt > 0 && int.TryParse(endstr.Substring(0, cnt), out pixelver) && pixelver >= 6;
                }
                return isGooglePixel6orGreater ? false : GHConstants.IsGPUDefault;
#endif
            }
        }

        public static void InitFileDescriptors()
        {
#if DEBUG
            int limitbefore = GnollHackService.GetFileDescriptorLimit(false);
#endif
            if (!GnollHackService.IncreaseFileDescriptorLimitToAtLeast(GHConstants.MinimumFileDescriptorLimit))
                Debug.WriteLine("Failed to set file descriptor limits.");
            else
            {
#if DEBUG
                int limitafter = GnollHackService.GetFileDescriptorLimit(false);
                int hardlimit = GnollHackService.GetFileDescriptorLimit(true);
                Debug.WriteLine("File descriptor limit changed from " + limitbefore + " to " + limitafter + " (hard limit: " + hardlimit + ").");
#endif
            }
        }

        private static Secrets _currentSecrets = null;
        public static Secrets CurrentSecrets
        {
            get
            { return _currentSecrets; }
            set
            { _currentSecrets = value; }
        }
        public static void ReadSecrets()
        {
            Assembly assembly = typeof(App).GetTypeInfo().Assembly;
            string json = "";
            using (Stream stream = assembly.GetManifestResourceStream(GHApp.AppResourceName + GHConstants.GHSettingsResourcePath))
            {
                if (stream != null)
                {
                    using (StreamReader sr = new StreamReader(stream))
                    {
                        json = sr.ReadToEnd();
                    }
                }
                else
                {
                    return;
                }
            }

            CurrentSecrets = JsonConvert.DeserializeObject<Secrets>(json);
        }

        private static UserSecrets _currentUserSecrets = null;
        public static UserSecrets CurrentUserSecrets
        {
            get
            { return _currentUserSecrets; }
            set
            { _currentUserSecrets = value; }
        }
        public static void ReadUserSecrets()
        {
            Assembly assembly = typeof(App).GetTypeInfo().Assembly;
            string json = "";
            using (Stream stream = assembly.GetManifestResourceStream(GHApp.AppResourceName + GHConstants.GHSecretsResourcePath))
            {
                if (stream != null)
                {
                    using (StreamReader sr = new StreamReader(stream))
                    {
                        json = sr.ReadToEnd();
                    }
                }
                else
                {
                    return;
                }
            }

            CurrentUserSecrets = JsonConvert.DeserializeObject<UserSecrets>(json);
        }

        private static readonly object _aggregateSessionPlayTimeLock = new object();
        private static long _aggregateSessionPlayTime = 0L;
        public static long AggregateSessionPlayTime { get { lock (_aggregateSessionPlayTimeLock) { return _aggregateSessionPlayTime; } } set { lock (_aggregateSessionPlayTimeLock) { _aggregateSessionPlayTime = value; } } }
        public static void AddAggragateSessionPlayTime(long addition)
        {
            lock (_aggregateSessionPlayTimeLock)
            {
                _aggregateSessionPlayTime = _aggregateSessionPlayTime + addition;
            }
        }

        private static readonly object _cancelSaveGameLock = new object();
        private static bool _cancelSaveGame = false;
        public static bool CancelSaveGame { get { lock (_cancelSaveGameLock) { return _cancelSaveGame; } } set { lock (_cancelSaveGameLock) { _cancelSaveGame = value; } } }

        private static readonly object _savingGameLock = new object();
        private static bool _savingGame = false;
        public static bool SavingGame { get { lock (_savingGameLock) { return _savingGame; } } set { lock (_savingGameLock) { _savingGame = value; } } }

        public static int AppSwitchSaveStyle { get; set; }

        private static readonly object _gameSavedLock = new object();
        private static bool _gameSaved = false;
        public static bool GameSaved { get { lock (_gameSavedLock) { return _gameSaved; } } set { lock (_gameSavedLock) { _gameSaved = value; } } }

        private static readonly object _gameSaveResultLock = new object();
        public static int GameSaveResult { get { lock (_gameSaveResultLock) { return Preferences.Get("GameSaveResult", 0); } } set { lock (_gameSaveResultLock) { Preferences.Set("GameSaveResult", value); } } }

        public static void CollectGarbage()
        {
            GC.Collect();
            GC.WaitForPendingFinalizers();
            GC.Collect();
        }

       public static void OnStart()
        {
            if (PlatformService != null)
                PlatformService.OverrideAnimatorDuration();

            CancelSaveGame = true;
            SleepMuteMode = false;
            if (CurrentGHGame != null && !CurrentGHGame.CasualMode)
            {
                //Detect background app killing OS, check if last exit is through going to sleep, and notify player that the app probably had been terminated by OS but game has been saved
                bool wenttosleep = Preferences.Get("WentToSleepWithGameOn", false);
                Preferences.Set("WentToSleepWithGameOn", false);
                if (wenttosleep && (GameSaved || SavingGame))
                {
                    CurrentGHGame.ActiveGamePage.StopWaitAndResumeSavedGame();
                }
            }
        }

        public static void OnSleep()
        {
            if (PlatformService != null)
                PlatformService.RevertAnimatorDuration(false);

            CancelSaveGame = false;
            SleepMuteMode = true;
            if (CurrentMainPage != null)
                CurrentMainPage.Suspend();
            if (CurrentGamePage != null)
                CurrentGamePage.Suspend();
            if (CurrentGHGame != null)
            {
                //Detect background app killing OS, mark that exit has been through going to sleep, and save the game
                Preferences.Set("WentToSleepWithGameOn", true);
                Preferences.Set("GameSaveResult", 0);
                if (GHApp.BatteryChargeLevel > 3) /* Save only if there is enough battery left to prevent save file corruption when the phone powers off */
                {
                    CurrentGHGame.ActiveGamePage.SaveGameAndWaitForResume();
                }
            }
            CollectGarbage();
        }

        public static void OnResume()
        {
            if (PlatformService != null)
                PlatformService.OverrideAnimatorDuration();

            /* Check current battery level, internet connection, and xlog user name when returning to app */
            try
            {
                Battery_BatteryInfoChanged(null, new BatteryInfoChangedEventArgs(Battery.ChargeLevel, Battery.State, Battery.PowerSource));
                Connectivity_ConnectivityChanged(null, new ConnectivityChangedEventArgs(Connectivity.NetworkAccess, Connectivity.ConnectionProfiles));
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
            TryVerifyXlogUserName();

            CancelSaveGame = true;
            SleepMuteMode = false;
            if (CurrentMainPage != null)
                CurrentMainPage.Resume();
            if (CurrentGamePage != null)
                CurrentGamePage.Resume();
            if (CurrentGHGame != null)
            {
                //Detect background app killing OS, check if last exit is through going to sleep & game has been saved, and load previously saved game
                bool wenttosleep = Preferences.Get("WentToSleepWithGameOn", false);
                Preferences.Set("WentToSleepWithGameOn", false);
                Preferences.Set("GameSaveResult", 0);
                if (wenttosleep && (GameSaved || SavingGame))
                {
                    CurrentGHGame.ActiveGamePage.StopWaitAndResumeSavedGame();
                }
            }
        }

        public static bool DownloadOnDemandPackage
        {
            get
            {
#if DEBUG
                return false;
#else
                return false;
#endif
            }
        }

        public static bool IsMuted { get { return SilentMode || SleepMuteMode || GameMuteMode; } }

        private readonly static object _silentModeLock = new object();
        private static bool _silentMode = false;
        public static bool SilentMode { get { lock (_silentModeLock) { return _silentMode; } } set { UpdateSoundMuteness(GameMuteMode, value, SleepMuteMode); lock (_silentModeLock) { _silentMode = value; } } }    /* Manual mute by user  */

        private readonly static object _sleepMuteModeLock = new object();
        private static bool _sleepMuteMode = false;
        public static bool SleepMuteMode { get { lock (_sleepMuteModeLock) { return _sleepMuteMode; } } set { UpdateSoundMuteness(GameMuteMode, SilentMode, value); lock (_sleepMuteModeLock) { _sleepMuteMode = value; } } }    /* Muteness because switched apps */

        private readonly static object _gameMuteModeLock = new object();
        private static bool _gameMuteMode = false;
        public static bool GameMuteMode { get { lock (_gameMuteModeLock) { return _gameMuteMode; } } set { UpdateSoundMuteness(value, SilentMode, SleepMuteMode); lock (_gameMuteModeLock) { _gameMuteMode = value; } } }    /* Muteness due to game state */

        public static void UpdateSoundMuteness(bool newGameMuted, bool newSilentMode, bool newSleepMuteMode)
        {
            UpdateSoundMutenessCore(newGameMuted, newSilentMode, newSleepMuteMode, GameMuteMode, SleepMuteMode, SilentMode);
        }

        public static void UpdateSoundMutenessCore(bool newGameMuted, bool newSilentMode, bool newSleepMuteMode, bool oldGameMuted, bool oldSilentMode, bool oldSleepMuteMode)
        {
            if (newGameMuted || newSilentMode || newSleepMuteMode)
            {
                if (!oldGameMuted && !oldSilentMode && !oldSleepMuteMode)
                    MuteSounds();
            }
            else
            {
                if (oldGameMuted || oldSilentMode || oldSleepMuteMode)
                    UnmuteSounds();
            }
        }

        public static void MuteSounds()
        {
            try
            {
                if (FmodService != null)
                    FmodService.AdjustVolumes(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
        }

        public static void UnmuteSounds()
        {
            if (FmodService != null)
            {
                try
                {
                    float generalVolume = Preferences.Get("GeneralVolume", GHConstants.DefaultGeneralVolume);
                    float musicVolume = Preferences.Get("MusicVolume", GHConstants.DefaultMusicVolume);
                    float ambientVolume = Preferences.Get("AmbientVolume", GHConstants.DefaultAmbientVolume);
                    float dialogueVolume = Preferences.Get("DialogueVolume", GHConstants.DefaultDialogueVolume);
                    float effectsVolume = Preferences.Get("EffectsVolume", GHConstants.DefaultEffectsVolume);
                    float UIVolume = Preferences.Get("UIVolume", GHConstants.DefaultUIVolume);
                    FmodService.AdjustVolumes(generalVolume, musicVolume, ambientVolume, dialogueVolume, effectsVolume, UIVolume);
                }
                catch (Exception ex)
                {
                    Debug.WriteLine(ex.Message);
                }
            }
        }

        public static void GetDependencyServices()
        {
#if GNH_MAUI
            _mainGnollHackService = new GnollHackService();
#else
            _mainGnollHackService = DependencyService.Get<IGnollHackService>();
#endif
            _mainGnollHackService.LoadLibrary();
            _mainGnollHackService.Test();
#if GNH_MAUI
            _fmodService = new FmodService();
            _platformService = new PlatformService();
#else
            _fmodService = DependencyService.Get<IFmodService>();
            _platformService = DependencyService.Get<IPlatformService>();
#endif
        }

        public static void ResetAcquiredFiles()
        {
            string ghdir = _mainGnollHackService.GetGnollHackPath();
            bool resetfiles = Preferences.Get("ResetExternalFiles", true);
            if (resetfiles)
            {
                foreach (SecretsFile sf in CurrentSecrets.files)
                {
                    if (resetfiles)
                    {
                        string sdir = string.IsNullOrWhiteSpace(sf.target_directory) ? ghdir : Path.Combine(ghdir, sf.target_directory);
                        string sfile = Path.Combine(sdir, sf.name);
                        if (File.Exists(sfile))
                        {
                            FileInfo file = new FileInfo(sfile);
                            file.Delete();
                        }
                        if (Preferences.ContainsKey("Verify_" + sf.id + "_Version"))
                            Preferences.Remove("Verify_" + sf.id + "_Version");
                        if (Preferences.ContainsKey("Verify_" + sf.id + "_LastWriteTime"))
                            Preferences.Remove("Verify_" + sf.id + "_LastWriteTime");
                    }
                }
                Preferences.Set("ResetExternalFiles", false);
            }
        }

        private static bool _hideNavBar;
        public static bool HideAndroidNavigationBar
        {
            get { return _hideNavBar; }
            set
            {
                _hideNavBar = value;
                if (_hideNavBar)
                {
                    MessagingCenter.Send<Object>(new object(), "HideOsNavigationBar");
                }
                else
                {
                    MessagingCenter.Send<Object>(new object(), "ShowOsNavigationBar");
                }
            }
        }

        public static bool HideiOSStatusBar
        {
            get { return PlatformService.GetStatusBarHidden(); }
            set { PlatformService.SetStatusBarHidden(value); }
        }

        public static bool DeveloperMode { get; set; }
        public static bool DebugLogMessages { get; set; }
        public static bool FullVersionMode { get; set; }
        public static bool ClassicMode { get; set; }
        public static bool CasualMode { get; set; }
        public static bool UseHTMLDumpLogs { get; set; }
        public static bool UseSingleDumpLog { get; set; }
        public static bool ReadStreamingBankToMemory { get; set; }
        public static bool CopyStreamingBankToDisk { get; set; }
        public static bool ForceCopyAllBanksToDisk { get; set; }

        public static string GHVersionId { get; set; }
        public static string GHVersionString { get; set; }
        public static ulong GHVersionNumber { get; set; }
        public static ulong GHVersionCompatibility { get; set; }
        public static string GHVersionCompatibilityString { get { return VersionNumberToString(GHVersionCompatibility); } }
        public static ulong GHPreviousVersionNumber { get; set; }
        public static bool GHDebug { get; set; }
        public static string SkiaVersionString { get; set; }
        public static string SkiaSharpVersionString { get; set; }
        public static string FMODVersionString { get; set; }
        public static string GHPath { get; set; }
        public static bool LoadBanks { get; set; }

        public static event BackButtonHandler BackButtonPressed;

        private static IGnollHackService _mainGnollHackService = null;
        public static IGnollHackService GnollHackService { get { return _mainGnollHackService; } }
        private static IFmodService _fmodService = null;
        public static IFmodService FmodService { get { return _fmodService; } }
        private static IPlatformService _platformService = null;
        public static IPlatformService PlatformService { get { return _platformService; } }

        public static readonly float DisplayRefreshRate = Math.Max(30.0f, DeviceDisplay.MainDisplayInfo.RefreshRate);
#if GNH_MAUI
        public static readonly bool IsAndroid = (DeviceInfo.Platform == DevicePlatform.Android);
        public static readonly bool IsiOS = (DeviceInfo.Platform == DevicePlatform.iOS);
#else
        public static readonly bool IsAndroid = (Device.RuntimePlatform == Device.Android);
        public static readonly bool IsiOS = (Device.RuntimePlatform == Device.iOS);
#endif
        public static readonly float DisplayScale = DeviceDisplay.MainDisplayInfo.Density <= 0 ? 1.0f : (float)DeviceDisplay.MainDisplayInfo.Density;
        public static readonly float DisplayWidth = (float)DeviceDisplay.MainDisplayInfo.Width * DisplayScale;
        public static readonly float DisplayHeight = (float)DeviceDisplay.MainDisplayInfo.Height * DisplayScale;

        public static async Task<bool> OnBackButtonPressed()
        {
            var handler = BackButtonPressed;
            if (handler != null)
            {
                var result = await handler.Invoke(App.Current, new EventArgs());
                return result;
            }
            return true;
        }

        public static async Task<bool> EmptyBackButtonPressed(object sender, EventArgs e)
        {
            return await Task.FromResult(false);
        }

        public static string VersionNumberToString(ulong vernum)
        {
            if (vernum == 0UL)
                return "";

            ulong majorver = (vernum >> 24) & 0xFFUL;
            ulong minorver = (vernum >> 16) & 0xFFUL;
            ulong patchver = (vernum >> 8) & 0xFFUL;
            ulong editver = (vernum) & 0xFFUL;
            string verstr = majorver + "." + minorver + "." + patchver + (editver > 0 ? (" (Build " + editver + ")") : "");
            return verstr;
        }

        public static string VersionNumberToFileNameSuffix(ulong vernum)
        {
            if (vernum == 0UL)
                return "";

            ulong majorver = (vernum >> 24) & 0xFFUL;
            ulong minorver = (vernum >> 16) & 0xFFUL;
            ulong patchver = (vernum >> 8) & 0xFFUL;
            ulong editver = (vernum) & 0xFFUL;
            string verstr = majorver + "." + minorver + "." + patchver + "-" + editver;
            return verstr;
        }

        public static SKTypeface DiabloTypeface { get; set; }
        public static SKTypeface ImmortalTypeface { get; set; }
        public static SKTypeface EndorTypeface { get; set; }
        public static SKTypeface XizorTypeface { get; set; }
        public static SKTypeface UnderwoodTypeface { get; set; }
        public static SKTypeface DejaVuSansMonoTypeface { get; set; }
        public static SKTypeface DejaVuSansMonoBoldTypeface { get; set; }
        public static SKTypeface LatoRegular { get; set; }
        public static SKTypeface LatoBold { get; set; }
        public static SKTypeface ARChristyTypeface { get; set; }

        private static Dictionary<string, SKTypeface> TypefaceDictionary = new Dictionary<string, SKTypeface>();
        public static SKTypeface GetTypefaceByName(string fontname)
        {
            SKTypeface res = null;
            if (TypefaceDictionary.TryGetValue(fontname, out res))
            {
                if (res != null)
                    return res;
                else
                    return LatoRegular;
            }
            else
                return LatoRegular;
        }

        public static void InitBaseTypefaces(Assembly assembly)
        {
            using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.diablo_h.ttf"))
            {
                if (stream != null)
                {
                    try
                    {
                        DiabloTypeface = SKTypeface.FromStream(stream);
                        TypefaceDictionary.Add("Diablo", DiabloTypeface);
                    }
                    catch (Exception ex)
                    {
                        Debug.WriteLine(ex.Message);
                    }
                }
            }
            using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.uwch.ttf"))
            {
                if (stream != null)
                {
                    try
                    {
                        UnderwoodTypeface = SKTypeface.FromStream(stream);
                        TypefaceDictionary.Add("Underwood", UnderwoodTypeface);
                    }
                    catch (Exception ex)
                    {
                        Debug.WriteLine(ex.Message);
                    }
                }
            }
            using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.Immortal-Regular.ttf"))
            {
                if (stream != null)
                {
                    try
                    {
                        ImmortalTypeface = SKTypeface.FromStream(stream);
                        TypefaceDictionary.Add("Immortal", ImmortalTypeface);
                    }
                    catch (Exception ex)
                    {
                        Debug.WriteLine(ex.Message);
                    }
                }
            }
            using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.archristy.ttf"))
            {
                if (stream != null)
                {
                    try
                    {
                        ARChristyTypeface = SKTypeface.FromStream(stream);
                        TypefaceDictionary.Add("ARChristy", ARChristyTypeface);
                    }
                    catch (Exception ex)
                    {
                        Debug.WriteLine(ex.Message);
                    }
                }
            }
        }

        public static void InitAdditionalTypefaces(Assembly assembly)
        {
            using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.endr.ttf"))
            {
                if (stream != null)
                {
                    try
                    {
                        EndorTypeface = SKTypeface.FromStream(stream);
                        TypefaceDictionary.Add("Endor", EndorTypeface);
                    }
                    catch (Exception ex)
                    {
                        Debug.WriteLine(ex.Message);
                    }
                }
            }
            using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.shxi.ttf"))
            {
                if (stream != null)
                {
                    try
                    {
                        XizorTypeface = SKTypeface.FromStream(stream);
                        TypefaceDictionary.Add("Xizor", XizorTypeface);
                    }
                    catch (Exception ex)
                    {
                        Debug.WriteLine(ex.Message);
                    }
                }
            }
            using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.DejaVuSansMono.ttf"))
            {
                if (stream != null)
                {
                    try
                    {
                        DejaVuSansMonoTypeface = SKTypeface.FromStream(stream);
                        TypefaceDictionary.Add("DejaVuSansMono", DejaVuSansMonoTypeface);
                    }
                    catch (Exception ex)
                    {
                        Debug.WriteLine(ex.Message);
                    }
                }
            }
            using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.DejaVuSansMono-Bold.ttf"))
            {
                if (stream != null)
                {
                    try
                    {
                        DejaVuSansMonoBoldTypeface = SKTypeface.FromStream(stream);
                        TypefaceDictionary.Add("DejaVuSansMono-Bold", DejaVuSansMonoBoldTypeface);
                    }
                    catch (Exception ex)
                    {
                        Debug.WriteLine(ex.Message);
                    }
                }
            }
            using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.Lato-Regular.ttf"))
            {
                if (stream != null)
                {
                    try
                    {
                        LatoRegular = SKTypeface.FromStream(stream);
                        TypefaceDictionary.Add("Lato-Regular", LatoRegular);
                    }
                    catch (Exception ex)
                    {
                        Debug.WriteLine(ex.Message);
                    }
                }
            }
            using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.Lato-Bold.ttf"))
            {
                if (stream != null)
                {
                    try
                    {
                        LatoBold = SKTypeface.FromStream(stream);
                        TypefaceDictionary.Add("Lato-Bold", LatoBold);
                    }
                    catch (Exception ex)
                    {
                        Debug.WriteLine(ex.Message);
                    }
                }
            }
        }

        //public static void SaveDumplogTypefaces(Assembly assembly)
        //{
        //    string targetdir = Path.Combine(GHPath, GHConstants.DumplogDirectory);
        //    if (!Directory.Exists(targetdir))
        //        return;

        //    string[] fileNames = { "DejaVuSansMono.woff", "DejaVuSansMono-Bold.woff", "DejaVuSansMono-Oblique.woff", "DejaVuSansMono-BoldOblique.woff" };
        //    for(int i = 0; i < 4; i++) 
        //    { 
        //        string filename = fileNames[i];
        //        using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets." + filename))
        //        {
        //            if (stream != null)
        //            {
        //                try
        //                {
        //                    string fulltargetpath = Path.Combine(targetdir, filename);
        //                    if (File.Exists(fulltargetpath))
        //                        File.Delete(fulltargetpath);
        //                    if (!File.Exists(fulltargetpath))
        //                    {
        //                        using (FileStream filestream = new FileStream(fulltargetpath, FileMode.Create, FileAccess.Write, FileShare.None))
        //                        {
        //                            stream.CopyTo(filestream);
        //                        }
        //                    }
        //                }
        //                catch (Exception ex)
        //                {
        //                    Debug.WriteLine(ex.Message);
        //                }
        //            }
        //        }
        //    }
        //}

        public static SKBitmap MenuBackgroundBitmap { get; set; }
        public static SKBitmap OldPaperBackgroundBitmap { get; set; }
        public static SKBitmap LoadingScreenBackgroundBitmap { get; set; }
        public static SKBitmap ButtonNormalBitmap { get; set; }
        public static SKBitmap ButtonSelectedBitmap { get; set; }
        public static SKBitmap ButtonDisabledBitmap { get; set; }

        public static SKBitmap SimpleFrameTopLeftCornerBitmap { get; set; }
        public static SKBitmap SimpleFrameSmallTopLeftCornerBitmap { get; set; }
        public static SKBitmap SimpleFrameTopHorizontalBitmap { get; set; }
        public static SKBitmap SimpleFrameLeftVerticalBitmap { get; set; }

        public static SKBitmap SimpleFrame2TopLeftCornerBitmap { get; set; }
        public static SKBitmap SimpleFrame2SmallTopLeftCornerBitmap { get; set; }
        public static SKBitmap SimpleFrame2TopHorizontalBitmap { get; set; }
        public static SKBitmap SimpleFrame2LeftVerticalBitmap { get; set; }

        public static SKBitmap ScrollBitmap { get; set; }
        public static SKBitmap YouBitmap { get; set; }

        public static ImageSource ButtonNormalImageSource { get; set; }
        public static ImageSource ButtonSelectedImageSource { get; set; }
        public static ImageSource ButtonDisabledImageSource { get; set; }

        public static void InitGameBitmaps(Assembly assembly)
        {
            try
            {
                using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.menubackground.png"))
                {
                    MenuBackgroundBitmap = SKBitmap.Decode(stream);
                    MenuBackgroundBitmap.SetImmutable();
                }
                using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.background-oldpaper.png"))
                {
                    OldPaperBackgroundBitmap = SKBitmap.Decode(stream);
                    OldPaperBackgroundBitmap.SetImmutable();
                }
                using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.background-loading-screen.png"))
                {
                    LoadingScreenBackgroundBitmap = SKBitmap.Decode(stream);
                    LoadingScreenBackgroundBitmap.SetImmutable();
                }
                using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.button_normal.png"))
                {
                    ButtonNormalBitmap = SKBitmap.Decode(stream);
                    ButtonNormalBitmap.SetImmutable();
                }
                using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.button_selected.png"))
                {
                    ButtonSelectedBitmap = SKBitmap.Decode(stream);
                    ButtonSelectedBitmap.SetImmutable();
                }
                using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.button_disabled.png"))
                {
                    ButtonDisabledBitmap = SKBitmap.Decode(stream);
                    ButtonDisabledBitmap.SetImmutable();
                }
                using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.frame-topleft.png"))
                {
                    SimpleFrameTopLeftCornerBitmap = SKBitmap.Decode(stream);
                    SimpleFrameTopLeftCornerBitmap.SetImmutable();
                }
                using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.frame-topleft-small.png"))
                {
                    SimpleFrameSmallTopLeftCornerBitmap = SKBitmap.Decode(stream);
                    SimpleFrameSmallTopLeftCornerBitmap.SetImmutable();
                }
                using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.frame-horizontal.png"))
                {
                    SimpleFrameTopHorizontalBitmap = SKBitmap.Decode(stream);
                    SimpleFrameTopHorizontalBitmap.SetImmutable();
                }
                using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.frame-vertical.png"))
                {
                    SimpleFrameLeftVerticalBitmap = SKBitmap.Decode(stream);
                    SimpleFrameLeftVerticalBitmap.SetImmutable();
                }
                using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.frame2-topleft.png"))
                {
                    SimpleFrame2TopLeftCornerBitmap = SKBitmap.Decode(stream);
                    SimpleFrame2TopLeftCornerBitmap.SetImmutable();
                }
                using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.frame2-topleft-small.png"))
                {
                    SimpleFrame2SmallTopLeftCornerBitmap = SKBitmap.Decode(stream);
                    SimpleFrame2SmallTopLeftCornerBitmap.SetImmutable();
                }
                using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.frame2-horizontal.png"))
                {
                    SimpleFrame2TopHorizontalBitmap = SKBitmap.Decode(stream);
                    SimpleFrame2TopHorizontalBitmap.SetImmutable();
                }
                using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.frame2-vertical.png"))
                {
                    SimpleFrame2LeftVerticalBitmap = SKBitmap.Decode(stream);
                    SimpleFrame2LeftVerticalBitmap.SetImmutable();
                }
                using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.scroll.png"))
                {
                    ScrollBitmap = SKBitmap.Decode(stream);
                    ScrollBitmap.SetImmutable();
                }
                using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.you.png"))
                {
                    YouBitmap = SKBitmap.Decode(stream);
                    YouBitmap.SetImmutable();
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
        }

        private static SKBitmap _successBitmap;
        private static SKBitmap _manaBitmap;
        private static SKBitmap _cooldownBitmap;
        private static SKBitmap _castsBitmap;
        private static SKBitmap _addsBitmap;
        private static SKBitmap _foodBitmap;

        private static SKBitmap _spellAbjurationBitmap;
        private static SKBitmap _spellArcaneBitmap;
        private static SKBitmap _spellCelestialBitmap;
        private static SKBitmap _spellClericalBitmap;
        private static SKBitmap _spellConjurationBitmap;
        private static SKBitmap _spellDivinationBitmap;
        private static SKBitmap _spellEnchantmentBitmap;
        private static SKBitmap _spellHealingBitmap;
        private static SKBitmap _spellMovementBitmap;
        private static SKBitmap _spellNatureBitmap;
        private static SKBitmap _spellNecromancyBitmap;
        private static SKBitmap _spellTransmutationBitmap;

        public static void InitSymbolBitmaps(Assembly assembly)
        {
            try
            {
                /* Replaceable menu symbols */
                using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.symbol-success.png"))
                {
                    _successBitmap = SKBitmap.Decode(stream);
                    _successBitmap.SetImmutable();
                }
                using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.symbol-mana.png"))
                {
                    _manaBitmap = SKBitmap.Decode(stream);
                    _manaBitmap.SetImmutable();
                }
                using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.symbol-cooldown.png"))
                {
                    _cooldownBitmap = SKBitmap.Decode(stream);
                    _cooldownBitmap.SetImmutable();
                }
                using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.symbol-casts.png"))
                {
                    _castsBitmap = SKBitmap.Decode(stream);
                    _castsBitmap.SetImmutable();
                }
                using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.symbol-adds.png"))
                {
                    _addsBitmap = SKBitmap.Decode(stream);
                    _addsBitmap.SetImmutable();
                }
                using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.symbol-food.png"))
                {
                    _foodBitmap = SKBitmap.Decode(stream);
                    _foodBitmap.SetImmutable();
                }

                using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.symbol-spell-abjuration.png"))
                {
                    _spellAbjurationBitmap = SKBitmap.Decode(stream);
                    _spellAbjurationBitmap.SetImmutable();
                }
                using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.symbol-spell-arcane.png"))
                {
                    _spellArcaneBitmap = SKBitmap.Decode(stream);
                    _spellArcaneBitmap.SetImmutable();
                }
                using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.symbol-spell-celestial.png"))
                {
                    _spellCelestialBitmap = SKBitmap.Decode(stream);
                    _spellCelestialBitmap.SetImmutable();
                }
                using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.symbol-spell-clerical.png"))
                {
                    _spellClericalBitmap = SKBitmap.Decode(stream);
                    _spellClericalBitmap.SetImmutable();
                }
                using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.symbol-spell-conjuration.png"))
                {
                    _spellConjurationBitmap = SKBitmap.Decode(stream);
                    _spellConjurationBitmap.SetImmutable();
                }
                using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.symbol-spell-divination.png"))
                {
                    _spellDivinationBitmap = SKBitmap.Decode(stream);
                    _spellDivinationBitmap.SetImmutable();
                }
                using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.symbol-spell-enchantment.png"))
                {
                    _spellEnchantmentBitmap = SKBitmap.Decode(stream);
                    _spellEnchantmentBitmap.SetImmutable();
                }
                using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.symbol-spell-healing.png"))
                {
                    _spellHealingBitmap = SKBitmap.Decode(stream);
                    _spellHealingBitmap.SetImmutable();
                }
                using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.symbol-spell-movement.png"))
                {
                    _spellMovementBitmap = SKBitmap.Decode(stream);
                    _spellMovementBitmap.SetImmutable();
                }
                using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.symbol-spell-nature.png"))
                {
                    _spellNatureBitmap = SKBitmap.Decode(stream);
                    _spellNatureBitmap.SetImmutable();
                }
                using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.symbol-spell-necromancy.png"))
                {
                    _spellNecromancyBitmap = SKBitmap.Decode(stream);
                    _spellNecromancyBitmap.SetImmutable();
                }
                using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.symbol-spell-transmutation.png"))
                {
                    _spellTransmutationBitmap = SKBitmap.Decode(stream);
                    _spellTransmutationBitmap.SetImmutable();
                }

            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
        }



        public static SKBitmap _logoBitmap;
        public static SKBitmap _skillBitmap;
        public static SKBitmap _prevWepBitmap;
        public static SKBitmap _prevUnwieldBitmap;
        public static SKBitmap[] _arrowBitmap = new SKBitmap[9];
        public static SKBitmap _orbBorderBitmap;
        public static SKBitmap _orbFillBitmap;
        public static SKBitmap _orbFillBitmapRed;
        public static SKBitmap _orbFillBitmapBlue;
        public static SKBitmap _orbGlassBitmap;

        public static SKBitmap _batteryFrameBitmap;
        public static SKBitmap _batteryRedFrameBitmap;
        public static SKBitmap _fpsBitmap;

        public static SKBitmap _statusWizardBitmap;
        public static SKBitmap _statusCasualBitmap;
        public static SKBitmap _statusCasualClassicBitmap;
        public static SKBitmap _statusModernBitmap;

        public static SKBitmap _statusDifficultyBitmap;
        public static SKBitmap _statusDifficultyVeryEasyBitmap;
        public static SKBitmap _statusDifficultyEasyBitmap;
        public static SKBitmap _statusDifficultyAverageBitmap;
        public static SKBitmap _statusDifficultyHardBitmap;
        public static SKBitmap _statusDifficultyExpertBitmap;
        public static SKBitmap _statusDifficultyMasterBitmap;
        public static SKBitmap _statusDifficultyGrandMasterBitmap;

        public static SKBitmap _statusXPLevelBitmap;
        public static SKBitmap _statusHDBitmap;
        public static SKBitmap _statusACBitmap;
        public static SKBitmap _statusMCBitmap;
        public static SKBitmap _statusMoveBitmap;
        public static SKBitmap _statusWeaponStyleBitmap;
        public static SKBitmap _statusQuiveredWeaponStyleBitmap;
        public static SKBitmap _statusEmptyHandedBitmap;

        public static SKBitmap _statusGoldBitmap;
        public static SKBitmap _statusTurnsBitmap;

        public static SKBitmap _statusDungeonLevelBitmap;

        public static SKBitmap _searchBitmap;
        public static SKBitmap _waitBitmap;
        public static SKBitmap _damageBitmap;


        public static bool StartGameDataSet = false;
        public static readonly object Glyph2TileLock = new object();
        public static int[] Glyph2Tile { get; set; }
        public static byte[] GlyphTileFlags { get; set; }
        public static short[] Tile2Animation { get; set; }
        public static short[] Tile2Enlargement { get; set; }
        public static short[] Tile2Autodraw { get; set; }
        public static int[] AnimationOffsets { get; set; }
        public static int[] EnlargementOffsets { get; set; }
        public static int[] ReplacementOffsets { get; set; }
        public static int Glyph2TileSize { get; set; }
        public static SKBitmap[] _tileMap = new SKBitmap[GHConstants.MaxTileSheets];
        public static int UsedTileSheets { get; set; }
        public static int TotalTiles { get; set; }
        public static int UnexploredGlyph { get; set; }
        public static int NoGlyph { get; set; }
        public static int AnimationOff { get; set; }
        public static int EnlargementOff { get; set; }
        public static int ReplacementOff { get; set; }
        public static int GeneralTileOff { get; set; }
        public static int HitTileOff { get; set; }
        public static int UITileOff { get; set; }
        public static int SpellTileOff { get; set; }
        public static int SkillTileOff { get; set; }
        public static int CommandTileOff { get; set; }
        public static int BuffTileOff { get; set; }
        public static int CursorOff { get; set; }


        public static int[] _tilesPerRow = new int[GHConstants.MaxTileSheets];
        public static int[] TilesPerRow { get { return _tilesPerRow; } }

        public static List<AnimationDefinition> _animationDefs = null;
        public static List<EnlargementDefinition> _enlargementDefs = null;
        public static List<ReplacementDefinition> _replacementDefs = null;
        public static List<AutoDrawDefinition> _autodraws = null;

        public static List<AnimationDefinition> Animations { get { return _animationDefs; } }
        public static List<EnlargementDefinition> Enlargements { get { return _enlargementDefs; } }
        public static List<ReplacementDefinition> Replacements { get { return _replacementDefs; } }
        public static List<AutoDrawDefinition> Autodraws { get { return _autodraws; } }

        public static readonly object _moreBtnLock = new object();
        public static GHCommandButtonItem[,,] _moreBtnMatrix = new GHCommandButtonItem[GHConstants.MoreButtonPages, GHConstants.MoreButtonsPerRow, GHConstants.MoreButtonsPerColumn];
        public static SKBitmap[,,] _moreBtnBitmaps = new SKBitmap[GHConstants.MoreButtonPages, GHConstants.MoreButtonsPerRow, GHConstants.MoreButtonsPerColumn];
        public static readonly string[] _moreButtonPageTitle = new string[GHConstants.MoreButtonPages] { "Wizard Mode Commands", "Common Commands", "Additional Commands", "Context and More Commands" };

        public static int TileSheetIdx(int ntile)
        {
            return (Math.Min(UsedTileSheets - 1, Math.Max(0, (ntile / GHConstants.NumberOfTilesPerSheet))));
        }

        public static int TileSheetX(int ntile)
        {
            return (((ntile % GHConstants.NumberOfTilesPerSheet) % _tilesPerRow[TileSheetIdx(ntile)]) * GHConstants.TileWidth);
        }
        public static int TileSheetY(int ntile)
        {
            return (((ntile % GHConstants.NumberOfTilesPerSheet) / _tilesPerRow[TileSheetIdx(ntile)]) * GHConstants.TileHeight);
        }

        public static List<SelectableShortcutButton> GetShortcutButtonsToAllocate()
        {
            List<SelectableShortcutButton> barlist = GetSimpleBarButtons();
            List<SelectableShortcutButton> list = new List<SelectableShortcutButton>();
            for (int i = 0; i < 6; i++)
            {
                int defCmd = GHApp.DefaultShortcutButton(0, i, true).GetCommand();
                int listselidx = GHApp.SelectableShortcutButtonIndexInList(defCmd, defCmd);
                if (listselidx >= 0 && listselidx < SelectableShortcutButtons.Count)
                {
                    if(!barlist.Contains(SelectableShortcutButtons[listselidx]))
                        list.Add(SelectableShortcutButtons[listselidx]);
                }
            }
            return list;
        }

        public static List<SelectableShortcutButton> GetSimpleBarButtons()
        {
            List <SelectableShortcutButton> list = new List<SelectableShortcutButton>();
            for (int i = 0; i < 6; i++)
            {
                string keystr = "SimpleUILayoutCommandButton" + (i + 1);
                int defCmd = GHApp.DefaultShortcutButton(0, i, true).GetCommand();
                int savedCmd = Preferences.Get(keystr, defCmd);
                int listselidx = GHApp.SelectableShortcutButtonIndexInList(savedCmd, defCmd);
                if(listselidx >= 0 && listselidx < SelectableShortcutButtons.Count)
                    list.Add(SelectableShortcutButtons[listselidx]);
            }
            return list;
        }

        public static void InitializeMoreCommandButtons(Assembly assembly, bool useSimple)
        {
            lock (_moreBtnLock)
            {
                for (int i = 0; i < GHConstants.MoreButtonPages; i++)
                    for (int j = 0; j < GHConstants.MoreButtonsPerRow; j++)
                        for (int k = 0; k < GHConstants.MoreButtonsPerColumn; k++)
                            _moreBtnMatrix[i, j, k] = null;
                if (useSimple)
                {
                    _moreBtnMatrix[0, 0, 0] = new GHCommandButtonItem("Wish", AppResourceName + ".Assets.UI.wish.png", GHUtils.Ctrl((int)'w'));
                    _moreBtnMatrix[0, 1, 0] = new GHCommandButtonItem("Reveal", AppResourceName + ".Assets.UI.reveal.png", GHUtils.Ctrl((int)'f'));
                    _moreBtnMatrix[0, 2, 0] = new GHCommandButtonItem("Genesis", AppResourceName + ".Assets.UI.genesis.png", GHUtils.Meta((int)'m'));
                    _moreBtnMatrix[0, 3, 0] = new GHCommandButtonItem("Levelport", AppResourceName + ".Assets.UI.levelport.png", GHUtils.Ctrl((int)'v'));

                    _moreBtnMatrix[0, 0, 1] = new GHCommandButtonItem("Identify", AppResourceName + ".Assets.UI.identify.png", GHUtils.Ctrl((int)'i'));
                    _moreBtnMatrix[0, 1, 1] = new GHCommandButtonItem("Teleport", AppResourceName + ".Assets.UI.teleport.png", GHUtils.Ctrl((int)'t'));
                    _moreBtnMatrix[0, 2, 1] = new GHCommandButtonItem("Level Change", AppResourceName + ".Assets.UI.levelchange.png", GHUtils.Meta(0));
                    _moreBtnMatrix[0, 3, 1] = new GHCommandButtonItem("Polymorph Self", AppResourceName + ".Assets.UI.polymorph.png", GHUtils.Meta(1));

                    _moreBtnMatrix[0, 2, 5] = new GHCommandButtonItem("Extended", AppResourceName + ".Assets.UI.extended.png", (int)'#');
                    _moreBtnMatrix[0, 3, 5] = new GHCommandButtonItem("Back to Game", AppResourceName + ".Assets.UI.more.png", -101);

                    _moreBtnMatrix[1, 0, 0] = new GHCommandButtonItem("You", AppResourceName + ".Assets.UI.you.png", (int)'}');
                    _moreBtnMatrix[1, 1, 0] = new GHCommandButtonItem("Skills", AppResourceName + ".Assets.UI.skill.png", (int)'S');
                    _moreBtnMatrix[1, 2, 0] = new GHCommandButtonItem("Overview", AppResourceName + ".Assets.UI.overview.png", GHUtils.Ctrl((int)'o'));
                    _moreBtnMatrix[1, 3, 0] = new GHCommandButtonItem("Goals", AppResourceName + ".Assets.UI.conduct.png", GHUtils.Meta(3));

                    _moreBtnMatrix[1, 0, 1] = new GHCommandButtonItem("Wait", AppResourceName + ".Assets.UI.wait.png", (int)'.');
                    _moreBtnMatrix[1, 1, 1] = new GHCommandButtonItem("Wield", AppResourceName + ".Assets.UI.wield.png", (int)'w');
                    _moreBtnMatrix[1, 2, 1] = new GHCommandButtonItem("Drop Items", AppResourceName + ".Assets.UI.dropmany.png", (int)'%');
                    _moreBtnMatrix[1, 3, 1] = new GHCommandButtonItem("Drop Types", AppResourceName + ".Assets.UI.droptypes.png", (int)'D');

                    _moreBtnMatrix[1, 0, 2] = new GHCommandButtonItem("Throw", AppResourceName + ".Assets.UI.throw.png", (int)'t');
                    _moreBtnMatrix[1, 1, 2] = new GHCommandButtonItem("Fire", AppResourceName + ".Assets.UI.fire.png", (int)'f');
                    _moreBtnMatrix[1, 2, 2] = new GHCommandButtonItem("Chat", AppResourceName + ".Assets.UI.chat.png", (int)'C');
                    _moreBtnMatrix[1, 3, 2] = new GHCommandButtonItem("Zap", AppResourceName + ".Assets.UI.zap.png", (int)'z');

                    _moreBtnMatrix[1, 0, 3] = new GHCommandButtonItem("Eat", AppResourceName + ".Assets.UI.eat.png", (int)'e');
                    _moreBtnMatrix[1, 1, 3] = new GHCommandButtonItem("Drink", AppResourceName + ".Assets.UI.quaff.png", (int)'q');
                    _moreBtnMatrix[1, 2, 3] = new GHCommandButtonItem("Read", AppResourceName + ".Assets.UI.read.png", (int)'r');
                    _moreBtnMatrix[1, 3, 3] = new GHCommandButtonItem("Apply", AppResourceName + ".Assets.UI.apply.png", (int)'a');

                    _moreBtnMatrix[1, 0, 4] = new GHCommandButtonItem("View Spell", AppResourceName + ".Assets.UI.viewspell.png", GHUtils.Meta((int)'z'));
                    _moreBtnMatrix[1, 1, 4] = new GHCommandButtonItem("Mix", AppResourceName + ".Assets.UI.mix.png", (int)'X');
                    _moreBtnMatrix[1, 2, 4] = new GHCommandButtonItem("Yell", AppResourceName + ".Assets.UI.yell.png", GHUtils.Ctrl((int)'y'));
                    _moreBtnMatrix[1, 3, 4] = new GHCommandButtonItem("Pray", AppResourceName + ".Assets.UI.pray.png", GHUtils.Meta((int)'p'));

                    _moreBtnMatrix[1, 0, 5] = new GHCommandButtonItem("Count", AppResourceName + ".Assets.UI.count.png", -105);
                    _moreBtnMatrix[1, 1, 5] = new GHCommandButtonItem("Search 20", AppResourceName + ".Assets.UI.search20.png", -102);
                    _moreBtnMatrix[1, 2, 5] = new GHCommandButtonItem("Rest", AppResourceName + ".Assets.UI.rest.png", -103);
                    _moreBtnMatrix[1, 3, 5] = new GHCommandButtonItem("Back to Game", AppResourceName + ".Assets.UI.more.png", -101);

                    _moreBtnMatrix[2, 0, 0] = new GHCommandButtonItem("Attributes", AppResourceName + ".Assets.UI.attributes.png", GHUtils.Meta(2));
                    _moreBtnMatrix[2, 1, 0] = new GHCommandButtonItem("Discoveries", AppResourceName + ".Assets.UI.discoveries.png", (int)'\\');
                    _moreBtnMatrix[2, 2, 0] = new GHCommandButtonItem("Travel", AppResourceName + ".Assets.UI.travel.png", (int)'_');
                    _moreBtnMatrix[2, 3, 0] = new GHCommandButtonItem("Autostash", AppResourceName + ".Assets.UI.autostash.png", GHUtils.Meta(15));

                    _moreBtnMatrix[2, 0, 1] = new GHCommandButtonItem("Fight", AppResourceName + ".Assets.UI.fight.png", (int)'F');
                    _moreBtnMatrix[2, 1, 1] = new GHCommandButtonItem("2-Weapon", AppResourceName + ".Assets.UI.twoweap.png", GHUtils.Ctrl((int)'x'));
                    _moreBtnMatrix[2, 2, 1] = new GHCommandButtonItem("Examine", AppResourceName + ".Assets.UI.examine.png", GHUtils.Meta((int)'x'));
                    _moreBtnMatrix[2, 3, 1] = new GHCommandButtonItem("Engrave", AppResourceName + ".Assets.UI.engrave.png", (int)'E');

                    _moreBtnMatrix[2, 0, 2] = new GHCommandButtonItem("Ride", AppResourceName + ".Assets.UI.ride.png", GHUtils.Meta((int)'R'));
                    _moreBtnMatrix[2, 1, 2] = new GHCommandButtonItem("Untrap", AppResourceName + ".Assets.UI.untrap.png", GHUtils.Meta((int)'u'));
                    _moreBtnMatrix[2, 2, 2] = new GHCommandButtonItem("Handedness", AppResourceName + ".Assets.UI.handedness.png", GHUtils.Meta((int)'h'));
                    _moreBtnMatrix[2, 3, 2] = new GHCommandButtonItem("Unwield", AppResourceName + ".Assets.UI.unwield.png", GHUtils.Meta(5));

                    _moreBtnMatrix[2, 0, 3] = new GHCommandButtonItem("Light", AppResourceName + ".Assets.UI.light.png", GHUtils.Ctrl((int)'l'));
                    _moreBtnMatrix[2, 1, 3] = new GHCommandButtonItem("Loot", AppResourceName + ".Assets.UI.loot.png", (int)'l');
                    _moreBtnMatrix[2, 2, 3] = new GHCommandButtonItem("Open", AppResourceName + ".Assets.UI.open.png", (int)'o');
                    _moreBtnMatrix[2, 3, 3] = new GHCommandButtonItem("Close", AppResourceName + ".Assets.UI.close.png", (int)'c');

                    _moreBtnMatrix[2, 0, 4] = new GHCommandButtonItem("Teleport", AppResourceName + ".Assets.UI.teleport.png", GHUtils.Ctrl((int)'t'));
                    _moreBtnMatrix[2, 1, 4] = new GHCommandButtonItem("Monster", AppResourceName + ".Assets.UI.monster.png", GHUtils.Meta(4));
                    _moreBtnMatrix[2, 2, 4] = new GHCommandButtonItem("Jump", AppResourceName + ".Assets.UI.jump.png", (int)'j');
                    _moreBtnMatrix[2, 3, 4] = new GHCommandButtonItem("Dig", AppResourceName + ".Assets.UI.dig.png", GHUtils.Ctrl((int)'g'));

                    _moreBtnMatrix[2, 0, 5] = new GHCommandButtonItem("Name", AppResourceName + ".Assets.UI.name.png", (int)'N');
                    _moreBtnMatrix[2, 1, 5] = new GHCommandButtonItem("Look Here", AppResourceName + ".Assets.UI.lookhere.png", (int)':');
                    _moreBtnMatrix[2, 2, 5] = new GHCommandButtonItem("Extended", AppResourceName + ".Assets.UI.extended.png", (int)'#');
                    _moreBtnMatrix[2, 3, 5] = new GHCommandButtonItem("Back to Game", AppResourceName + ".Assets.UI.more.png", -101);

                    List<SelectableShortcutButton> buttonsToAllocate = GetShortcutButtonsToAllocate();
                    List<SelectableShortcutButton> buttonsOnBar = GetSimpleBarButtons();
                    foreach (SelectableShortcutButton button in buttonsToAllocate)
                    {
                        bool foundloc = false;
                        for (int p = 1; p <= 2; p++)
                        {
                            for (int i = 0; i < GHConstants.MoreButtonsPerRow; i++)
                            {
                                for (int j = 0; j < GHConstants.MoreButtonsPerColumn; j++)
                                {
                                    for(int bidx = 0; bidx < buttonsOnBar.Count; bidx++)
                                    {
                                        if (_moreBtnMatrix[p, i, j] != null && _moreBtnMatrix[p, i, j].Command == buttonsOnBar[bidx].GetCommand())
                                        {
                                            foundloc = true;
                                            _moreBtnMatrix[p, i, j].Text = button.Label;
                                            _moreBtnMatrix[p, i, j].ImageSourcePath = button.ImageSourcePath;
                                            _moreBtnMatrix[p, i, j].Command = button.GetCommand();
                                            break;
                                        }
                                    }
                                    if (foundloc)
                                        break;
                                }
                                if (foundloc)
                                    break;
                            }
                            if (foundloc)
                                break;
                        }
                        if (!foundloc)
                            break; /* No spaces found */
                    }
                }
                else
                {
                    _moreBtnMatrix[0, 0, 0] = new GHCommandButtonItem("Wish", AppResourceName + ".Assets.UI.wish.png", GHUtils.Ctrl((int)'w'));
                    _moreBtnMatrix[0, 1, 0] = new GHCommandButtonItem("Reveal", AppResourceName + ".Assets.UI.reveal.png", GHUtils.Ctrl((int)'f'));
                    _moreBtnMatrix[0, 2, 0] = new GHCommandButtonItem("Genesis", AppResourceName + ".Assets.UI.genesis.png", GHUtils.Meta((int)'m'));
                    _moreBtnMatrix[0, 3, 0] = new GHCommandButtonItem("Levelport", AppResourceName + ".Assets.UI.levelport.png", GHUtils.Ctrl((int)'v'));

                    _moreBtnMatrix[0, 0, 1] = new GHCommandButtonItem("Identify", AppResourceName + ".Assets.UI.identify.png", GHUtils.Ctrl((int)'i'));
                    _moreBtnMatrix[0, 1, 1] = new GHCommandButtonItem("Teleport", AppResourceName + ".Assets.UI.teleport.png", GHUtils.Ctrl((int)'t'));
                    _moreBtnMatrix[0, 2, 1] = new GHCommandButtonItem("Level Change", AppResourceName + ".Assets.UI.levelchange.png", GHUtils.Meta(0));
                    _moreBtnMatrix[0, 3, 1] = new GHCommandButtonItem("Polymorph Self", AppResourceName + ".Assets.UI.polymorph.png", GHUtils.Meta(1));

                    _moreBtnMatrix[0, 2, 5] = new GHCommandButtonItem("Extended", AppResourceName + ".Assets.UI.extended.png", (int)'#');
                    _moreBtnMatrix[0, 3, 5] = new GHCommandButtonItem("Back to Game", AppResourceName + ".Assets.UI.more.png", -101);

                    _moreBtnMatrix[1, 0, 0] = new GHCommandButtonItem("You", AppResourceName + ".Assets.UI.you.png", (int)'}');
                    _moreBtnMatrix[1, 1, 0] = new GHCommandButtonItem("Skills", AppResourceName + ".Assets.UI.skill.png", (int)'S');
                    _moreBtnMatrix[1, 2, 0] = new GHCommandButtonItem("Overview", AppResourceName + ".Assets.UI.overview.png", GHUtils.Ctrl((int)'o'));
                    _moreBtnMatrix[1, 3, 0] = new GHCommandButtonItem("Goals", AppResourceName + ".Assets.UI.conduct.png", GHUtils.Meta(3));

                    _moreBtnMatrix[1, 0, 1] = new GHCommandButtonItem("View Spell", AppResourceName + ".Assets.UI.viewspell.png", GHUtils.Meta((int)'z'));
                    _moreBtnMatrix[1, 1, 1] = new GHCommandButtonItem("Mix", AppResourceName + ".Assets.UI.mix.png", (int)'X');
                    _moreBtnMatrix[1, 2, 1] = new GHCommandButtonItem("Travel", AppResourceName + ".Assets.UI.travel.png", (int)'_');
                    _moreBtnMatrix[1, 3, 1] = new GHCommandButtonItem("2-Weapon", AppResourceName + ".Assets.UI.twoweap.png", GHUtils.Ctrl((int)'x'));

                    _moreBtnMatrix[1, 0, 2] = new GHCommandButtonItem("Examine", AppResourceName + ".Assets.UI.examine.png", GHUtils.Meta((int)'x'));
                    _moreBtnMatrix[1, 1, 2] = new GHCommandButtonItem("Engrave", AppResourceName + ".Assets.UI.engrave.png", (int)'E');
                    _moreBtnMatrix[1, 2, 2] = new GHCommandButtonItem("Ride", AppResourceName + ".Assets.UI.ride.png", GHUtils.Meta((int)'R'));
                    _moreBtnMatrix[1, 3, 2] = new GHCommandButtonItem("Wield", AppResourceName + ".Assets.UI.wield.png", (int)'w');

                    _moreBtnMatrix[1, 0, 3] = new GHCommandButtonItem("Eat", AppResourceName + ".Assets.UI.eat.png", (int)'e');
                    _moreBtnMatrix[1, 1, 3] = new GHCommandButtonItem("Drink", AppResourceName + ".Assets.UI.quaff.png", (int)'q');
                    _moreBtnMatrix[1, 2, 3] = new GHCommandButtonItem("Read", AppResourceName + ".Assets.UI.read.png", (int)'r');
                    _moreBtnMatrix[1, 3, 3] = new GHCommandButtonItem("Drop Types", AppResourceName + ".Assets.UI.droptypes.png", (int)'D');

                    _moreBtnMatrix[1, 0, 4] = new GHCommandButtonItem("Untrap", AppResourceName + ".Assets.UI.untrap.png", GHUtils.Meta((int)'u'));
                    _moreBtnMatrix[1, 1, 4] = new GHCommandButtonItem("Handedness", AppResourceName + ".Assets.UI.handedness.png", GHUtils.Meta((int)'h'));
                    _moreBtnMatrix[1, 2, 4] = new GHCommandButtonItem("Yell", AppResourceName + ".Assets.UI.yell.png", GHUtils.Ctrl((int)'y'));
                    _moreBtnMatrix[1, 3, 4] = new GHCommandButtonItem("Pray", AppResourceName + ".Assets.UI.pray.png", GHUtils.Meta((int)'p'));

                    _moreBtnMatrix[1, 0, 5] = new GHCommandButtonItem("Count", AppResourceName + ".Assets.UI.count.png", -105);
                    _moreBtnMatrix[1, 1, 5] = new GHCommandButtonItem("Search 20", AppResourceName + ".Assets.UI.search20.png", -102);
                    _moreBtnMatrix[1, 2, 5] = new GHCommandButtonItem("Rest", AppResourceName + ".Assets.UI.rest.png", -103);
                    _moreBtnMatrix[1, 3, 5] = new GHCommandButtonItem("Back to Game", AppResourceName + ".Assets.UI.more.png", -101);

                    _moreBtnMatrix[2, 0, 0] = new GHCommandButtonItem("Attributes", AppResourceName + ".Assets.UI.attributes.png", GHUtils.Meta(2));
                    _moreBtnMatrix[2, 1, 0] = new GHCommandButtonItem("Discoveries", AppResourceName + ".Assets.UI.discoveries.png", (int)'\\');
                    _moreBtnMatrix[2, 2, 0] = new GHCommandButtonItem("Killed", AppResourceName + ".Assets.UI.killed.png", GHUtils.Meta((int)'k'));
                    _moreBtnMatrix[2, 3, 0] = new GHCommandButtonItem("Genocided", AppResourceName + ".Assets.UI.genocided.png", GHUtils.Meta((int)'g'));

                    _moreBtnMatrix[2, 0, 1] = new GHCommandButtonItem("Wear", AppResourceName + ".Assets.UI.wear.png", (int)'W');
                    _moreBtnMatrix[2, 1, 1] = new GHCommandButtonItem("Put On", AppResourceName + ".Assets.UI.puton.png", (int)'P');
                    _moreBtnMatrix[2, 2, 1] = new GHCommandButtonItem("Quiver", AppResourceName + ".Assets.UI.quiver.png", (int)'Q');
                    _moreBtnMatrix[2, 3, 1] = new GHCommandButtonItem("Chronicle", AppResourceName + ".Assets.UI.chronicle.png", GHUtils.Meta(13));

                    _moreBtnMatrix[2, 0, 2] = new GHCommandButtonItem("Take Off", AppResourceName + ".Assets.UI.takeoff.png", (int)'T');
                    _moreBtnMatrix[2, 1, 2] = new GHCommandButtonItem("Remove", AppResourceName + ".Assets.UI.remove.png", (int)'R');
                    _moreBtnMatrix[2, 2, 2] = new GHCommandButtonItem("Take Off Many", AppResourceName + ".Assets.UI.takeoffmany.png", GHUtils.Meta((int)'t'));
                    _moreBtnMatrix[2, 3, 2] = new GHCommandButtonItem("Unwield", AppResourceName + ".Assets.UI.unwield.png", GHUtils.Meta(5));

                    _moreBtnMatrix[2, 0, 3] = new GHCommandButtonItem("Dig", AppResourceName + ".Assets.UI.dig.png", GHUtils.Ctrl((int)'g'));
                    _moreBtnMatrix[2, 1, 3] = new GHCommandButtonItem("Light", AppResourceName + ".Assets.UI.light.png", GHUtils.Ctrl((int)'l'));
                    _moreBtnMatrix[2, 2, 3] = new GHCommandButtonItem("Jump", AppResourceName + ".Assets.UI.jump.png", (int)'j');
                    _moreBtnMatrix[2, 3, 3] = new GHCommandButtonItem("Fight", AppResourceName + ".Assets.UI.fight.png", (int)'F');

                    _moreBtnMatrix[2, 0, 4] = new GHCommandButtonItem("Tip", AppResourceName + ".Assets.UI.tip.png", GHUtils.Meta((int)'T'));
                    _moreBtnMatrix[2, 1, 4] = new GHCommandButtonItem("Invoke", AppResourceName + ".Assets.UI.invoke.png", GHUtils.Meta((int)'i'));
                    _moreBtnMatrix[2, 2, 4] = new GHCommandButtonItem("Rub", AppResourceName + ".Assets.UI.rub.png", GHUtils.Meta((int)'r'));
                    _moreBtnMatrix[2, 3, 4] = new GHCommandButtonItem("Wipe", AppResourceName + ".Assets.UI.wipe.png", GHUtils.Meta((int)'w'));

                    _moreBtnMatrix[2, 0, 5] = new GHCommandButtonItem("Name", AppResourceName + ".Assets.UI.name.png", (int)'N');
                    _moreBtnMatrix[2, 1, 5] = new GHCommandButtonItem("What Is", AppResourceName + ".Assets.UI.whatis.png", (int)'/');
                    _moreBtnMatrix[2, 2, 5] = new GHCommandButtonItem("Look Far", AppResourceName + ".Assets.UI.lookfar.png", (int)';');
                    _moreBtnMatrix[2, 3, 5] = new GHCommandButtonItem("Back to Game", AppResourceName + ".Assets.UI.more.png", -101);

                    _moreBtnMatrix[3, 0, 0] = new GHCommandButtonItem("Look Here", AppResourceName + ".Assets.UI.lookhere.png", (int)':');
                    _moreBtnMatrix[3, 1, 0] = new GHCommandButtonItem("Pick Up", AppResourceName + ".Assets.UI.pickup.png", (int)',');
                    _moreBtnMatrix[3, 2, 0] = new GHCommandButtonItem("Sit", AppResourceName + ".Assets.UI.sit.png", GHUtils.Ctrl((int)'s'));
                    _moreBtnMatrix[3, 3, 0] = new GHCommandButtonItem("Pay", AppResourceName + ".Assets.UI.pay.png", (int)'p');

                    _moreBtnMatrix[3, 0, 1] = new GHCommandButtonItem("Loot", AppResourceName + ".Assets.UI.loot.png", (int)'l');
                    _moreBtnMatrix[3, 1, 1] = new GHCommandButtonItem("Dip", AppResourceName + ".Assets.UI.dip.png", GHUtils.Meta((int)'d'));
                    _moreBtnMatrix[3, 2, 1] = new GHCommandButtonItem("Offer", AppResourceName + ".Assets.UI.offer.png", GHUtils.Meta((int)'o'));
                    _moreBtnMatrix[3, 3, 1] = new GHCommandButtonItem("Autopickup", AppResourceName + ".Assets.UI.autopickup.png", (int)'@');

                    _moreBtnMatrix[3, 0, 2] = new GHCommandButtonItem("Go Down", AppResourceName + ".Assets.UI.stairs-down.png", (int)'>');
                    _moreBtnMatrix[3, 1, 2] = new GHCommandButtonItem("Go Up", AppResourceName + ".Assets.UI.stairs-up.png", (int)'<');
                    _moreBtnMatrix[3, 2, 2] = new GHCommandButtonItem("Open", AppResourceName + ".Assets.UI.open.png", (int)'o');
                    _moreBtnMatrix[3, 3, 2] = new GHCommandButtonItem("Close", AppResourceName + ".Assets.UI.close.png", (int)'c');

                    _moreBtnMatrix[3, 0, 3] = new GHCommandButtonItem("Break", AppResourceName + ".Assets.UI.break.png", GHUtils.Ctrl((int)'b'));
                    _moreBtnMatrix[3, 1, 3] = new GHCommandButtonItem("Force Lock", AppResourceName + ".Assets.UI.forcelock.png", GHUtils.Meta((int)'f'));
                    _moreBtnMatrix[3, 2, 3] = new GHCommandButtonItem("Teleport", AppResourceName + ".Assets.UI.teleport.png", GHUtils.Ctrl((int)'t'));
                    _moreBtnMatrix[3, 3, 3] = new GHCommandButtonItem("Monster", AppResourceName + ".Assets.UI.monster.png", GHUtils.Meta(4));

                    _moreBtnMatrix[3, 0, 4] = new GHCommandButtonItem("Spells", AppResourceName + ".Assets.UI.spells.png", (int)'+');
                    _moreBtnMatrix[3, 1, 4] = new GHCommandButtonItem("Pick & Stash", AppResourceName + ".Assets.UI.picktobag.png", (int)';');
                    _moreBtnMatrix[3, 2, 4] = new GHCommandButtonItem("Autostash", AppResourceName + ".Assets.UI.autostash.png", GHUtils.Meta(15));

                    _moreBtnMatrix[3, 0, 5] = new GHCommandButtonItem("Help", AppResourceName + ".Assets.UI.help.png", (int)'?');
                    _moreBtnMatrix[3, 1, 5] = new GHCommandButtonItem("Commands", AppResourceName + ".Assets.UI.commands.png", GHUtils.Meta((int)'c'));
                    _moreBtnMatrix[3, 2, 5] = new GHCommandButtonItem("Extended", AppResourceName + ".Assets.UI.extended.png", (int)'#');
                    _moreBtnMatrix[3, 3, 5] = new GHCommandButtonItem("Back to Game", AppResourceName + ".Assets.UI.more.png", -101);
                }

                for (int k = 0; k < GHConstants.MoreButtonPages; k++)
                {
                    for (int i = 0; i < GHConstants.MoreButtonsPerRow; i++)
                    {
                        for (int j = 0; j < GHConstants.MoreButtonsPerColumn; j++)
                        {
                            if (_moreBtnMatrix[k, i, j] != null && !string.IsNullOrEmpty(_moreBtnMatrix[k, i, j].ImageSourcePath))
                            {
                                try
                                {
                                    _moreBtnBitmaps[k, i, j] = GetCachedImageSourceBitmap("resource://" + _moreBtnMatrix[k, i, j].ImageSourcePath, true);
                                    //using (Stream stream = assembly.GetManifestResourceStream(_moreBtnMatrix[k, i, j].ImageSourcePath))
                                    //{
                                    //    if (stream != null)
                                    //    {
                                    //        _moreBtnBitmaps[k, i, j] = SKBitmap.Decode(stream);
                                    //        _moreBtnBitmaps[k, i, j].SetImmutable(); ;
                                    //    }
                                    //}
                                }
                                catch (Exception ex)
                                {
                                    Debug.WriteLine(ex.Message);
                                }
                            }
                        }
                    }
                }
            }
        }

        public static void InitializeArrowButtons(Assembly assembly)
        {
            using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.arrow_left.png"))
            {
                _arrowBitmap[0] = SKBitmap.Decode(stream);
            }
            using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.arrow_up.png"))
            {
                _arrowBitmap[1] = SKBitmap.Decode(stream);
            }
            using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.arrow_right.png"))
            {
                _arrowBitmap[2] = SKBitmap.Decode(stream);
            }
            using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.arrow_down.png"))
            {
                _arrowBitmap[3] = SKBitmap.Decode(stream);
            }
            using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.arrow_upleft.png"))
            {
                _arrowBitmap[4] = SKBitmap.Decode(stream);
            }
            using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.no.png"))
            {
                _arrowBitmap[5] = SKBitmap.Decode(stream);
            }
            using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.arrow_upright.png"))
            {
                _arrowBitmap[6] = SKBitmap.Decode(stream);
            }
            using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.arrow_downright.png"))
            {
                _arrowBitmap[7] = SKBitmap.Decode(stream);
            }
            using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.arrow_downleft.png"))
            {
                _arrowBitmap[8] = SKBitmap.Decode(stream);
            }

            for(int i = 0; i < 9; i++)
                _arrowBitmap[i].SetImmutable();

        }

        public static void InitializeUIBitmaps(Assembly assembly)
        {
            using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.orb_border.png"))
            {
                _orbBorderBitmap = SKBitmap.Decode(stream);
                _orbBorderBitmap.SetImmutable();
            }
            using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.orb_fill.png"))
            {
                _orbFillBitmap = SKBitmap.Decode(stream);
                _orbFillBitmap.SetImmutable();
            }

            using (SKPaint bmpPaint = new SKPaint())
            {
                bmpPaint.Color = SKColors.White.WithAlpha(204);

                var redbitmap = new SKBitmap(_orbFillBitmap.Width, _orbFillBitmap.Height, SKImageInfo.PlatformColorType, SKAlphaType.Premul);
                var redcanvas = new SKCanvas(redbitmap);
                redcanvas.Clear(SKColors.Transparent);
                bmpPaint.ColorFilter = SKColorFilter.CreateColorMatrix(new float[]
                    {
                    -1.0f, 0,     0,    0, 255f,
                    0,     1.0f,  0,    0, 0,
                    0,     0,     1.0f, 0, 0,
                    0,     0,     0,    1, 0
                    });
                redcanvas.DrawBitmap(_orbFillBitmap, 0, 0, bmpPaint);
                _orbFillBitmapRed = redbitmap;
                _orbFillBitmapRed.SetImmutable();

                var bluebitmap = new SKBitmap(_orbFillBitmap.Width, _orbFillBitmap.Height, SKImageInfo.PlatformColorType, SKAlphaType.Premul);
                var bluecanvas = new SKCanvas(bluebitmap);
                bluecanvas.Clear(SKColors.Transparent);
                bmpPaint.ColorFilter = SKColorFilter.CreateColorMatrix(new float[]
                    {
                    1.0f,  0,      0,    0,   0,
                    0,     1.0f,   0,    0,   0,
                    0,     0,     -1.0f, 0,   255f,
                    0,     0,     0,     1,   0
                    });

                bluecanvas.DrawBitmap(_orbFillBitmap, 0, 0, bmpPaint);
                _orbFillBitmapBlue = bluebitmap;
                _orbFillBitmapBlue.SetImmutable();
            }

            using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.orb_glass.png"))
            {
                _orbGlassBitmap = SKBitmap.Decode(stream);
                _orbGlassBitmap.SetImmutable();
            }

            using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.battery-frame.png"))
            {
                _batteryFrameBitmap = SKBitmap.Decode(stream);
                _batteryFrameBitmap.SetImmutable();
            }

            using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.fps.png"))
            {
                _fpsBitmap = SKBitmap.Decode(stream);
                _fpsBitmap.SetImmutable();
            }

            using (SKPaint bmpPaint = new SKPaint())
            {
                bmpPaint.Color = SKColors.White;
                var redbitmap = new SKBitmap(_batteryFrameBitmap.Width, _batteryFrameBitmap.Height, SKImageInfo.PlatformColorType, SKAlphaType.Premul);
                var redcanvas = new SKCanvas(redbitmap);
                redcanvas.Clear(SKColors.Transparent);
                bmpPaint.ColorFilter = SKColorFilter.CreateColorMatrix(new float[]
                    {
                    1.0f,  0,     0,    0, 0,
                    0,     0.0f,  0,    0, 0,
                    0,     0,     0.0f, 0, 0,
                    0,     0,     0,    1, 0
                    });
                redcanvas.DrawBitmap(_batteryFrameBitmap, 0, 0, bmpPaint);
                _batteryRedFrameBitmap = redbitmap;
                _batteryRedFrameBitmap.SetImmutable();
            }

            using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.status-wizard-mode.png"))
            {
                _statusWizardBitmap = SKBitmap.Decode(stream);
                _statusWizardBitmap.SetImmutable();
            }
            using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.status-casual-mode.png"))
            {
                _statusCasualBitmap = SKBitmap.Decode(stream);
                _statusCasualBitmap.SetImmutable();
            }
            using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.status-casual-classic-mode.png"))
            {
                _statusCasualClassicBitmap = SKBitmap.Decode(stream);
                _statusCasualClassicBitmap.SetImmutable();
            }
            using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.status-modern-mode.png"))
            {
                _statusModernBitmap = SKBitmap.Decode(stream);
                _statusModernBitmap.SetImmutable();
            }
            using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.status-difficulty.png"))
            {
                _statusDifficultyBitmap = SKBitmap.Decode(stream);
                _statusDifficultyBitmap.SetImmutable();
            }
            using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.status-difficulty-very-easy.png"))
            {
                _statusDifficultyVeryEasyBitmap = SKBitmap.Decode(stream);
                _statusDifficultyVeryEasyBitmap.SetImmutable();
            }
            using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.status-difficulty-easy.png"))
            {
                _statusDifficultyEasyBitmap = SKBitmap.Decode(stream);
                _statusDifficultyEasyBitmap.SetImmutable();
            }
            using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.status-difficulty-average.png"))
            {
                _statusDifficultyAverageBitmap = SKBitmap.Decode(stream);
                _statusDifficultyAverageBitmap.SetImmutable();
            }
            using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.status-difficulty-hard.png"))
            {
                _statusDifficultyHardBitmap = SKBitmap.Decode(stream);
                _statusDifficultyHardBitmap.SetImmutable();
            }
            using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.status-difficulty-expert.png"))
            {
                _statusDifficultyExpertBitmap = SKBitmap.Decode(stream);
                _statusDifficultyExpertBitmap.SetImmutable();
            }
            using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.status-difficulty-master.png"))
            {
                _statusDifficultyMasterBitmap = SKBitmap.Decode(stream);
                _statusDifficultyMasterBitmap.SetImmutable();
            }
            using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.status-difficulty-grand-master.png"))
            {
                _statusDifficultyGrandMasterBitmap = SKBitmap.Decode(stream);
                _statusDifficultyGrandMasterBitmap.SetImmutable();
            }

            using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.status-xp-level.png"))
            {
                _statusXPLevelBitmap = SKBitmap.Decode(stream);
                _statusXPLevelBitmap.SetImmutable();
            }
            using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.status-hd.png"))
            {
                _statusHDBitmap = SKBitmap.Decode(stream);
                _statusHDBitmap.SetImmutable();
            }
            using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.status-ac.png"))
            {
                _statusACBitmap = SKBitmap.Decode(stream);
                _statusACBitmap.SetImmutable();
            }
            using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.status-mc.png"))
            {
                _statusMCBitmap = SKBitmap.Decode(stream);
                _statusMCBitmap.SetImmutable();
            }
            using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.status-move.png"))
            {
                _statusMoveBitmap = SKBitmap.Decode(stream);
                _statusMoveBitmap.SetImmutable();
            }
            using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.status-weapon-style.png"))
            {
                _statusWeaponStyleBitmap = SKBitmap.Decode(stream);
                _statusWeaponStyleBitmap.SetImmutable();
            }
            using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.status-quivered-weapon-style.png"))
            {
                _statusQuiveredWeaponStyleBitmap = SKBitmap.Decode(stream);
                _statusQuiveredWeaponStyleBitmap.SetImmutable();
            }
            using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.status-empty-handed.png"))
            {
                _statusEmptyHandedBitmap = SKBitmap.Decode(stream);
                _statusEmptyHandedBitmap.SetImmutable();
            }

            using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.status-gold.png"))
            {
                _statusGoldBitmap = SKBitmap.Decode(stream);
                _statusGoldBitmap.SetImmutable();
            }
            using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.status-turns.png"))
            {
                _statusTurnsBitmap = SKBitmap.Decode(stream);
                _statusTurnsBitmap.SetImmutable();
            }
            using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.status-dungeon-level.png"))
            {
                _statusDungeonLevelBitmap = SKBitmap.Decode(stream);
                _statusDungeonLevelBitmap.SetImmutable();
            }

            using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.search.png"))
            {
                _searchBitmap = SKBitmap.Decode(stream);
                _searchBitmap.SetImmutable();
            }
            using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.wait.png"))
            {
                _waitBitmap = SKBitmap.Decode(stream);
                _waitBitmap.SetImmutable();
            }
            using (Stream stream = assembly.GetManifestResourceStream(AppResourceName + ".Assets.UI.symbol-damage.png"))
            {
                _damageBitmap = SKBitmap.Decode(stream);
                _damageBitmap.SetImmutable();
            }
        }

        public static readonly List<SelectableShortcutButton> SelectableShortcutButtons = new List<SelectableShortcutButton>()
        {
            new SelectableShortcutButton("Apply Item", "Apply", 'a', false, false, 0, AppResourceName + ".Assets.UI.apply.png"),
            new SelectableShortcutButton("Cast Spell", "Cast", 'Z', false, false, 0, AppResourceName + ".Assets.UI.cast.png"),
            new SelectableShortcutButton("Chat", "Chat", 'C', false, false, 0, AppResourceName + ".Assets.UI.chat.png"),
            new SelectableShortcutButton("Drink Potion", "Drink", 'q', false, false, 0, AppResourceName + ".Assets.UI.quaff.png"),
            new SelectableShortcutButton("Drop Items", "Drop Items", '%', false, false, 0, AppResourceName + ".Assets.UI.dropmany.png"),
            new SelectableShortcutButton("Drop Item Types", "Drop Types", 'D', false, false, 0, AppResourceName + ".Assets.UI.droptypes.png"),
            new SelectableShortcutButton("Eat", "Eat", 'e', false, false, 0, AppResourceName + ".Assets.UI.eat.png"),
            new SelectableShortcutButton("Engrave", "Engrave", 'E', false, false, 0, AppResourceName + ".Assets.UI.engrave.png"),
            new SelectableShortcutButton("Examine Item", "Examine", 'x', false, true, 0, AppResourceName + ".Assets.UI.examine.png"),
            new SelectableShortcutButton("Fire Weapon", "Fire", 'f', false, false, 0, AppResourceName + ".Assets.UI.fire.png"),
            new SelectableShortcutButton("Force Fight", "Fight", 'F', false, false, 0, AppResourceName + ".Assets.UI.fight.png"),
            new SelectableShortcutButton("Inventory", "Inventory", 'i', false, false, 0, AppResourceName + ".Assets.UI.inventory.png"),
            new SelectableShortcutButton("Kick", "Kick", 'k', false, false, 0, AppResourceName + ".Assets.UI.kick.png"),
            new SelectableShortcutButton("Read", "Read", 'r', false, false, 0, AppResourceName + ".Assets.UI.read.png"),
            new SelectableShortcutButton("Repeat", "Repeat", 'A', true, false, 0, AppResourceName + ".Assets.UI.repeat.png"),
            new SelectableShortcutButton("Rest", "Rest", '\0', false, false, -103, AppResourceName + ".Assets.UI.rest.png"),
            new SelectableShortcutButton("Search", "Search", 's', false, false, 0, AppResourceName + ".Assets.UI.search.png"),
            new SelectableShortcutButton("Search 20 Times", "Search 20", '\0', false, false, -102, AppResourceName + ".Assets.UI.search20.png"),
            new SelectableShortcutButton("Swap Handedness", "Handedness", 'h', false, true, 0, AppResourceName + ".Assets.UI.handedness.png"),
            new SelectableShortcutButton("Swap Weapon", "Swap", 'x', false, false, 0, AppResourceName + ".Assets.UI.swap.png"),
            new SelectableShortcutButton("Throw", "Throw", 't', false, false, 0, AppResourceName + ".Assets.UI.throw.png"),
            new SelectableShortcutButton("Travel", "Travel", '_', false, false, 0, AppResourceName + ".Assets.UI.travel.png"),
            new SelectableShortcutButton("Two-Weapon Fighting", "2-Weapon", 'x', true, false, 0, AppResourceName + ".Assets.UI.twoweap.png"),
            new SelectableShortcutButton("Untrap", "Untrap", 'u', false, true, 0, AppResourceName + ".Assets.UI.untrap.png"),
            new SelectableShortcutButton("Unwield Weapon", "Unwield", '\0', false, false, GHUtils.Meta(5), AppResourceName + ".Assets.UI.unwield.png"),
            new SelectableShortcutButton("Wait", "Wait", '.', false, false, 0, AppResourceName + ".Assets.UI.wait.png"),
            new SelectableShortcutButton("Wield Weapon", "Wield", 'w', false, false, 0, AppResourceName + ".Assets.UI.wield.png"),
            new SelectableShortcutButton("Yell for Companions", "Yell", 'y', false, false, 0, AppResourceName + ".Assets.UI.yell.png"),
            new SelectableShortcutButton("Zap Wand", "Zap", 'z', false, false, 0, AppResourceName + ".Assets.UI.zap.png"),
        };

        private static readonly SelectableShortcutButton _defInventory = new SelectableShortcutButton("Inventory", "Inventory", 'i', false, false, 0, AppResourceName + ".Assets.UI.inventory.png");
        private static readonly SelectableShortcutButton _defSearch = new SelectableShortcutButton("Search", "Search", 's', false, false, 0, AppResourceName + ".Assets.UI.search.png");
        private static readonly SelectableShortcutButton _defSwap = new SelectableShortcutButton("Swap Weapon", "Swap", 'x', false, false, 0, AppResourceName + ".Assets.UI.swap.png");
        private static readonly SelectableShortcutButton _defKick = new SelectableShortcutButton("Kick", "Kick", 'k', false, false, 0, AppResourceName + ".Assets.UI.kick.png");
        private static readonly SelectableShortcutButton _defCast = new SelectableShortcutButton("Cast Spell", "Cast", 'Z', false, false, 0, AppResourceName + ".Assets.UI.cast.png");
        private static readonly SelectableShortcutButton _defRepeat = new SelectableShortcutButton("Repeat", "Repeat", 'A', true, false, 0, AppResourceName + ".Assets.UI.repeat.png");
        public static SelectableShortcutButton DefaultShortcutButton(int row, int column, bool issimple)
        {
            switch(column)
            {
                default:
                case 0:
                    return _defInventory;
                case 1:
                    return _defSearch;
                case 2:
                    return _defSwap;
                case 3:
                    return _defKick;
                case 4:
                    return _defCast;
                case 5:
                    return _defRepeat;
            }
        }

        public static int SelectableShortcutButtonIndexInList(int cmd, int defcmd)
        {
            for(int i = 0; i < SelectableShortcutButtons.Count; i++) 
            {
                SelectableShortcutButton button = SelectableShortcutButtons[i];
                if (button.GetCommand() == cmd)
                    return i;
            }
            for (int i = 0; i < SelectableShortcutButtons.Count; i++)
            {
                SelectableShortcutButton button = SelectableShortcutButtons[i];
                if (button.GetCommand() == defcmd)
                    return i;
            }
            return -1;
        }

#if DEBUG
        public static object ProfilingStopwatchLock = new object();
        private static Stopwatch _profilingStopwatch = new Stopwatch();
        public static Stopwatch ProfilingStopwatch { get { return _profilingStopwatch; } }
#endif

        public static void DebugWriteProfilingStopwatchTimeAndStop(string label)
        {
#if DEBUG
            lock (ProfilingStopwatchLock)
            {
                ProfilingStopwatch.Stop();
                TimeSpan elapsed = ProfilingStopwatch.Elapsed;
                Debug.WriteLine("ProfilingStopwatch: " + label + ": " + elapsed.TotalMilliseconds + " msec");
            }
#endif
        }

        public static void DebugWriteProfilingStopwatchTimeAndStart(string label)
        {
#if DEBUG
            lock (ProfilingStopwatchLock)
            {
                ProfilingStopwatch.Stop();
                TimeSpan elapsed = ProfilingStopwatch.Elapsed;
                Debug.WriteLine("ProfilingStopwatch: " + label + ": " + elapsed.TotalMilliseconds + " msec");
                ProfilingStopwatch.Start();
            }
#endif
        }

        public static void DebugWriteRestart(string label)
        {
#if DEBUG
            lock (ProfilingStopwatchLock)
            {
                Debug.WriteLine("ProfilingStopwatch: " + label + ": " + "Restart");
                ProfilingStopwatch.Restart();
            }
#endif
        }

        public static void DebugWriteProfilingStopwatchTimeAndRestart(string label)
        {
#if DEBUG
            lock (ProfilingStopwatchLock)
            {
                ProfilingStopwatch.Stop();
                TimeSpan elapsed = ProfilingStopwatch.Elapsed;
                Debug.WriteLine("ProfilingStopwatch: " + label + ": " + elapsed.TotalMilliseconds + " msec");
                ProfilingStopwatch.Restart();
            }
#endif
        }

        public static void DebugCheckCurrentFileDescriptor(string str)
        {
#if DEBUG
            if (GnollHackService == null)
                return;

            int fd = GnollHackService.CheckCurrentFileDescriptor();
            Debug.WriteLine("GHApp Debug: File Descriptor is " + fd + ", " + str);
#endif
        }

        public static async Task ListFileDescriptors(Page page)
        {
            /* Send Diagnostic Data via C */
            GnollHackService?.ReportFileDescriptors();
            if(IsAndroid)
            {
                /* Print file descriptors via C# */
                int id = Process.GetCurrentProcess().Id;
                string output = "";
                var proc = new Process
                {
                    StartInfo = new ProcessStartInfo
                    {
                        FileName = "lsof",
                        Arguments = "-a -p " + id.ToString(),
                        UseShellExecute = false,
                        RedirectStandardOutput = true,
                        CreateNoWindow = true
                    }
                };
                proc.Start();
                while (!proc.StandardOutput.EndOfStream)
                {
                    output += proc.StandardOutput.ReadLine() + "\n";
                }
                await page.DisplayAlert("File Descriptors", "GnollHack will now attempt to send critical diagnostic data." + (output != "" ? "The information is as follows:\n\n" + output : ""), "OK");
            }
        }

        public static void PlayButtonClickedSound()
        {
            if (_fmodService != null)
            {
                try
                {
                    string[] parameterNames = new string[1];
                    float[] parameterValues = new float[1];

                    _fmodService.PlayImmediateSound(GHConstants.ButtonClickGHSound, GHConstants.ButtonClickEventPath, GHConstants.ButtonClickBankId, GHConstants.ButtonClickVolume,
                        1.0f, parameterNames, parameterValues, 0, 0, 0, 0, 0UL);
                }
                catch
                {

                }

            }
        }

        private static void Player_PlaybackEnded(object sender, EventArgs e)
        {
        }

        private static Stream GetStreamFromFile(string filename)
        {
            var assembly = typeof(App).GetTypeInfo().Assembly;
            var stream = assembly.GetManifestResourceStream(AppResourceName + "." + filename);

            return stream;
        }

        public static void PlayMenuSelectSound()
        {
            if (_fmodService != null)
            {
                string[] parameterNames = new string[1];
                float[] parameterValues = new float[1];

                _fmodService.PlayImmediateSound(GHConstants.MenuSelectGHSound, GHConstants.MenuSelectEventPath, GHConstants.MenuSelectBankId, GHConstants.MenuSelectVolume,
                    1.0f, parameterNames, parameterValues, 0, 0, 0, 0, 0UL);
            }
        }
        public static string CreateGameZipArchive()
        {
            string ghdir = GnollHackService.GetGnollHackPath();
            string targetpath = Path.Combine(ghdir, "archive");

            CheckCreateDirectory(targetpath);

            string filepath = Path.Combine(targetpath, "crash_report-" + GHApp.VersionNumberToFileNameSuffix(GHApp.GHVersionNumber) + ".zip");
            if (File.Exists(filepath))
                File.Delete(filepath);

            string zipFile = filepath;
            string[] files = Directory.GetFiles(ghdir);

            using (ZipArchive archive = ZipFile.Open(zipFile, ZipArchiveMode.Create))
            {
                foreach (var fPath in files)
                {
                    archive.CreateEntryFromFile(fPath, Path.GetFileName(fPath));
                }
                string[] ghsubdirlist = { GHConstants.SaveDirectory, GHConstants.DumplogDirectory };
                foreach (string ghsubdir in ghsubdirlist)
                {
                    string subdirpath = Path.Combine(ghdir, ghsubdir);
                    string[] subfiles = Directory.GetFiles(subdirpath);
                    foreach (var fPath in subfiles)
                    {
                        archive.CreateEntryFromFile(fPath, Path.Combine(ghsubdir, Path.GetFileName(fPath)));
                    }
                }

            }
            return zipFile;
        }

        public static string CreateDumplogZipArchive()
        {
            string ghdir = GnollHackService.GetGnollHackPath();
            string targetpath = Path.Combine(ghdir, "archive");

            CheckCreateDirectory(targetpath);

            string filepath = Path.Combine(targetpath, "dumplogs.zip");
            if (File.Exists(filepath))
                File.Delete(filepath);

            string zipFile = filepath;
            string[] files = Directory.GetFiles(ghdir);

            using (ZipArchive archive = ZipFile.Open(zipFile, ZipArchiveMode.Create))
            {
                string[] ghsubdirlist = { GHConstants.DumplogDirectory };
                foreach (string ghsubdir in ghsubdirlist)
                {
                    string subdirpath = Path.Combine(ghdir, ghsubdir);
                    string[] subfiles = Directory.GetFiles(subdirpath);
                    foreach (var fPath in subfiles)
                    {
                        archive.CreateEntryFromFile(fPath, Path.GetFileName(fPath));
                    }
                }

            }
            return zipFile;
        }

        public static string CreateSavedGamesZipArchive()
        {
            string ghdir = GnollHackService.GetGnollHackPath();
            string targetpath = Path.Combine(ghdir, "archive");

            CheckCreateDirectory(targetpath);

            ulong vernum = GHVersionNumber;
            ulong majorver = (vernum >> 24) & 0xFFUL;
            ulong minorver = (vernum >> 16) & 0xFFUL;
            ulong patchlvl = (vernum >> 8) & 0xFFUL;
            ulong editlvl = (vernum) & 0xFFUL;
            string versionstring = majorver.ToString() + minorver.ToString() + patchlvl.ToString() + "-" + editlvl;
            string filepath = Path.Combine(targetpath, "savedgames-" + versionstring + ".zip");
            if (File.Exists(filepath))
                File.Delete(filepath);

            string zipFile = filepath;
            string[] files = Directory.GetFiles(ghdir);

            using (ZipArchive archive = ZipFile.Open(zipFile, ZipArchiveMode.Create))
            {
                string[] ghsubdirlist = { GHConstants.SaveDirectory };
                foreach (string ghsubdir in ghsubdirlist)
                {
                    string subdirpath = Path.Combine(ghdir, ghsubdir);
                    string[] subfiles = Directory.GetFiles(subdirpath);
                    foreach (var fPath in subfiles)
                    {
                        archive.CreateEntryFromFile(fPath, Path.GetFileName(fPath));
                    }
                }

            }
            return zipFile;
        }

        public static void CheckCreateDirectory(string targetpath)
        {
            if (!Directory.Exists(targetpath))
                Directory.CreateDirectory(targetpath);

            try
            {
                GnollHackService.Chmod(targetpath, (uint)ChmodPermissions.S_IALL);
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }

            //DirectoryInfo dinfo = new DirectoryInfo(targetpath);
            bool writesuccessful = false;
            string testfilepath = Path.Combine(targetpath, "test.txt");
            try
            {
                if (File.Exists(testfilepath))
                    File.Delete(testfilepath);

                File.WriteAllText(testfilepath, "Hello World!");

                writesuccessful = true;
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }

            try
            {
                if (File.Exists(testfilepath))
                    File.Delete(testfilepath);
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }

            if (!writesuccessful)
            {
                try
                {
                    if (Directory.Exists(targetpath))
                    {
                        foreach (string subfilename in Directory.GetFiles(targetpath))
                        {
                            string subfilepath = Path.Combine(targetpath, subfilename);
                            File.Delete(subfilepath);
                        }
                        Directory.Delete(targetpath);
                    }
                    if (!Directory.Exists(targetpath))
                        Directory.CreateDirectory(targetpath);

                    GnollHackService.Chmod(targetpath, (uint)ChmodPermissions.S_IALL);
                }
                catch (Exception ex)
                {
                    Debug.WriteLine(ex.Message);
                }

            }
        }

        public static void CheckForIncompatibleSavedGames()
        {
            try
            {
                string savepath = Path.Combine(GHPath, GHConstants.SaveDirectory);
                string[] savefiles = Directory.GetFiles(savepath);
                if (savefiles != null && savefiles.Length > 0)
                    InformAboutIncompatibleSavedGames = true;
            }
            catch
            {
                /* Nothing */
            }
        }

        public static SKBitmap GetSpecialSymbol(string str, out SKRect source_rect)
        {
            source_rect = new SKRect();
            if (str == null || !str.StartsWith("&"))
                return null;

            SKBitmap bitmap = null;
            string trimmed_str = str.Trim();
            if (trimmed_str == "&success;")
            {
                bitmap = _successBitmap;
            }
            else if (trimmed_str == "&mana;")
            {
                bitmap = _manaBitmap;
            }
            else if (trimmed_str == "&cool;")
            {
                bitmap = _cooldownBitmap;
            }
            else if (trimmed_str == "&casts;")
            {
                bitmap = _castsBitmap;
            }
            else if (trimmed_str == "&adds;")
            {
                bitmap = _addsBitmap;
            }
            else if (trimmed_str == "&food;")
            {
                bitmap = _foodBitmap;
            }
            else if (trimmed_str == "&spabj;")
            {
                bitmap = _spellAbjurationBitmap;
            }
            else if (trimmed_str == "&sparc;")
            {
                bitmap = _spellArcaneBitmap;
            }
            else if (trimmed_str == "&spcel;")
            {
                bitmap = _spellCelestialBitmap;
            }
            else if (trimmed_str == "&spcle;")
            {
                bitmap = _spellClericalBitmap;
            }
            else if (trimmed_str == "&spcon;")
            {
                bitmap = _spellConjurationBitmap;
            }
            else if (trimmed_str == "&spdiv;")
            {
                bitmap = _spellDivinationBitmap;
            }
            else if (trimmed_str == "&spenc;")
            {
                bitmap = _spellEnchantmentBitmap;
            }
            else if (trimmed_str == "&sphea;")
            {
                bitmap = _spellHealingBitmap;
            }
            else if (trimmed_str == "&spmov;")
            {
                bitmap = _spellMovementBitmap;
            }
            else if (trimmed_str == "&spnat;")
            {
                bitmap = _spellNatureBitmap;
            }
            else if (trimmed_str == "&spnec;")
            {
                bitmap = _spellNecromancyBitmap;
            }
            else if (trimmed_str == "&sptra;")
            {
                bitmap = _spellTransmutationBitmap;
            }
            else if (trimmed_str == "&damage;")
            {
                bitmap = _damageBitmap;
            }
            else if (trimmed_str == "&AC;")
            {
                bitmap = _statusACBitmap;
            }
            else if (trimmed_str == "&MC;")
            {
                bitmap = _statusMCBitmap;
            }

            if (bitmap != null)
            {
                source_rect.Right = bitmap.Width;
                source_rect.Bottom = bitmap.Height;
            }
            return bitmap;
        }

        static readonly object _cachedBitmapsLock = new object();
        static readonly ConcurrentDictionary<string, SKBitmap> _cachedBitmaps = new ConcurrentDictionary<string, SKBitmap>();

        public static void InitBaseCachedBitmaps(Assembly assembly)
        {
            lock (_cachedBitmapsLock)
            {
                try
                {
                    _cachedBitmaps.Clear();
                    string[] cachedBitmaps = new string[]
                    {
                    AppResourceName + ".Assets.UI.missing_icon.png",
                    AppResourceName + ".Assets.FMOD-Logo-192-White.png",
                    AppResourceName + ".Assets.gnollhack-logo-test-2.png",
                    };
                    foreach (string imagePath in cachedBitmaps)
                    {
                        using (Stream stream = assembly.GetManifestResourceStream(imagePath))
                        {
                            SKBitmap newBitmap = SKBitmap.Decode(stream);
                            if (newBitmap != null)
                            {
                                newBitmap.SetImmutable();
                                _cachedBitmaps.TryAdd("resource://" + imagePath, newBitmap);
                            }
                        }
                    }
                }
                catch (Exception ex)
                {
                    Debug.WriteLine(ex.Message);
                }
            }
        }

        public static void InitAdditionalCachedBitmaps(Assembly assembly)
        {
            lock (_cachedBitmapsLock)
            {
                try
                {
                    string[] cachedBitmaps = new string[]
                    {
                    AppResourceName + ".Assets.UI.yes.png",
                    AppResourceName + ".Assets.UI.yestoall.png",
                    AppResourceName + ".Assets.UI.no.png",
                    AppResourceName + ".Assets.UI.cancel.png",
                    AppResourceName + ".Assets.UI.inventory.png",
                    AppResourceName + ".Assets.UI.leftring.png",
                    AppResourceName + ".Assets.UI.rightring.png",
                    AppResourceName + ".Assets.UI.load.png",
                    AppResourceName + ".Assets.UI.name.png",
                    AppResourceName + ".Assets.UI.dropmany.png",
                    AppResourceName + ".Assets.UI.stone-look-off.png",
                    AppResourceName + ".Assets.UI.stone-look-on.png",
                    AppResourceName + ".Assets.UI.stone-autocenter-off.png",
                    AppResourceName + ".Assets.UI.stone-autocenter-on.png",
                    AppResourceName + ".Assets.UI.stone-minimap-off.png",
                    AppResourceName + ".Assets.UI.stone-minimap-on.png",
                    AppResourceName + ".Assets.UI.stone-travel-off.png",
                    AppResourceName + ".Assets.UI.stone-travel-on.png",
                    AppResourceName + ".Assets.UI.stone-altmap-off.png",
                    AppResourceName + ".Assets.UI.stone-altmap-on.png",
                    AppResourceName + ".Assets.UI.stairs-down.png",
                    AppResourceName + ".Assets.UI.stairs-up.png",
                    AppResourceName + ".Assets.UI.chat.png",
                    AppResourceName + ".Assets.UI.pickup.png",
                    AppResourceName + ".Assets.UI.eat.png",
                    AppResourceName + ".Assets.UI.pray.png",
                    AppResourceName + ".Assets.UI.offer.png",
                    AppResourceName + ".Assets.UI.loot.png",
                    AppResourceName + ".Assets.UI.lastitem.png",
                    AppResourceName + ".Assets.tombstone.png",
                    };
                    foreach (string imagePath in cachedBitmaps)
                    {
                        using (Stream stream = assembly.GetManifestResourceStream(imagePath))
                        {
                            SKBitmap newBitmap = SKBitmap.Decode(stream);
                            if (newBitmap != null)
                            {
                                newBitmap.SetImmutable();
                                _cachedBitmaps.TryAdd("resource://" + imagePath, newBitmap);
                            }
                        }
                    }
                }
                catch (Exception ex)
                {
                    Debug.WriteLine(ex.Message);
                }
            }
        }
        public static SKBitmap GetCachedImageSourceBitmap(string sourcePath, bool addToCache)
        {
            if (sourcePath == null || sourcePath == "")
                return null;

            lock (_cachedBitmapsLock)
            {
                try
                {
                    SKBitmap bitmap;
                    if (!_cachedBitmaps.TryGetValue(sourcePath, out bitmap))
                    {
                        string imagePath = sourcePath.Length > 11 && sourcePath.Substring(0, 11) == "resource://" ? sourcePath.Substring(11) : sourcePath;
                        Assembly assembly = typeof(App).GetTypeInfo().Assembly;
                        using (Stream stream = assembly.GetManifestResourceStream(imagePath))
                        {
                            SKBitmap newBitmap = SKBitmap.Decode(stream);
                            if (newBitmap != null)
                            {
                                newBitmap.SetImmutable();
                                if (addToCache)
                                    _cachedBitmaps.TryAdd(sourcePath, newBitmap);

                                return newBitmap;
                            }
                        }
                    }
                    else
                        return bitmap;
                }
                catch (Exception ex)
                {
                    Debug.WriteLine(ex.Message);
                }
                return null;
            }
        }

        private static readonly object _postingGameStatusLock = new object();
        private static bool _postingGameStatus;
        public static bool PostingGameStatus { get { lock (_postingGameStatusLock) { return _postingGameStatus; } } set { lock (_postingGameStatusLock) { _postingGameStatus = value; } } }

        private static readonly object _postingDiagnosticDataLock = new object();
        private static bool _postingDiagnosticData;
        public static bool PostingDiagnosticData { get { lock (_postingDiagnosticDataLock) { return _postingDiagnosticData; } } set { lock (_postingDiagnosticDataLock) { _postingDiagnosticData = value; } } }

        private static readonly object _postingXlogEntriesLock = new object();
        private static bool _postingXlogEntries;
        public static bool PostingXlogEntries { get { lock (_postingXlogEntriesLock) { return _postingXlogEntries; } } set { lock (_postingXlogEntriesLock) { _postingXlogEntries = value; } } }

        private static readonly object _postingBonesFilesLock = new object();
        private static bool _postingBonesFiles;
        public static bool PostingBonesFiles { get { lock (_postingBonesFilesLock) { return _postingBonesFiles; } } set { lock (_postingBonesFilesLock) { _postingBonesFiles = value; } } }

        private static readonly object _allowBonesLock = new object();
        private static bool _allowBones;
        public static bool AllowBones { get { lock (_allowBonesLock) { return _allowBones; } } set { lock (_allowBonesLock) { _allowBones = value; } } }

        public static string CustomGameStatusLink { get; set; }
        public static string CustomXlogAccountLink { get; set; }
        public static string CustomXlogPostLink { get; set; }

        public static string GameStatusPostAddress
        {
            get
            {
                if (CustomGameStatusLink != null && CustomGameStatusLink != "")
                {
                    return CustomGameStatusLink;
                }
                else
                {
#if DEBUG
                    return CurrentUserSecrets.DefaultDiagnosticDataPostAddress;
#else
                return CurrentUserSecrets.DefaultGamePostAddress;
#endif
                }
            }
        }
        public static string DiagnosticDataPostAddress
        {
            get
            {
                return CurrentUserSecrets.DefaultDiagnosticDataPostAddress;
            }
        }

        public static string XlogPostAddress
        {
            get
            {
                string address;
                if (!string.IsNullOrWhiteSpace(CustomXlogPostLink))
                {
                    address = CustomXlogPostLink;
                }
                else
                {
                    address = CurrentUserSecrets.DefaultXlogPostAddress;
                }
#if DEBUG
                if(XlogReleaseAccount)
                    return address;
                else
                    return address?.Replace("https://", "https://test-");
#else
                return address;
#endif
            }
        }

        public static string XlogAccountAddress
        {
            get
            {
                string address;
                if (CustomXlogAccountLink != null && CustomXlogAccountLink != "")
                {
                    address = CustomXlogAccountLink;
                }
                else
                {
                    address = CurrentUserSecrets.DefaultXlogAccountLink;
                }
#if DEBUG
                if (XlogReleaseAccount)
                    return address;
                else
                    return address?.Replace("https://", "https://test-");
#else
                return address;
#endif
            }
        }

        public static string XlogTopScoreAddress
        {
            get
            {
                string address = XlogPostAddress;
                string shortened_address;
                if (address.Length > 8)
                    shortened_address = address.Substring(0, address.Length - 8);
                else
                    shortened_address = "";

                string final_address = shortened_address + GHConstants.XlogTopScorePage;
                return final_address;
            }
        }

        public static string BonesPostAddress
        {
            get
            {
                string address = XlogPostAddress;
                string shortened_address;
                if (address.Length > 8)
                    shortened_address = address.Substring(0, address.Length - 8);
                else
                    shortened_address = "";

                string final_address = shortened_address + GHConstants.BonesPostPage;
                return final_address;
            }
        }

        private static readonly object _forcePostBonesLock = new object();
        private static bool _forcePostBones = false;
        public static bool ForcePostBones { get { lock (_forcePostBonesLock) { return _forcePostBones; } } set { lock (_forcePostBonesLock) { _forcePostBones = value; } } }


        private static readonly object _xlogCreditialLock = new object();
        private static string _xlogUserName = "";
        private static string _xlogPassword = "";

        public static string XlogUserName { get { lock (_xlogCreditialLock) { return _xlogUserName; } } set { lock (_xlogCreditialLock) { _xlogUserName = value; } } }
        public static string XlogPassword { get { lock (_xlogCreditialLock) { return _xlogPassword; } } set { lock (_xlogCreditialLock) { _xlogPassword = value; } } }
        public static string XlogAntiForgeryToken 
        {
            get
            {
                return CurrentUserSecrets.DefaultXlogAntiForgeryToken;
            }
        }

        private static readonly object _xlogReleaseAccountLock = new object();
        private static bool _xlogReleaseAccount;
        public static bool XlogReleaseAccount { get { lock (_xlogReleaseAccountLock) { return _xlogReleaseAccount; } } set { lock (_xlogReleaseAccountLock) { _xlogReleaseAccount = value; } } }

        private static string _verifiedUserName;
        private static string _verifiedPassword;
        private static bool _xlogUserNameVerified;
        private static readonly object _xlogUserNameVerifiedLock = new object();
        public static bool XlogUserNameVerified { get { lock (_xlogUserNameVerifiedLock) { return _xlogUserNameVerified; } } }

        private static readonly object _xlogCredentialsIncorrectLock = new object();
        private static bool _xlogCredentialsIncorrect;
        public static bool XlogCredentialsIncorrect { get { lock (_xlogCredentialsIncorrectLock) { return _xlogCredentialsIncorrect; } } set { lock (_xlogCredentialsIncorrectLock) { _xlogCredentialsIncorrect = value; } } }

        public static void SetXlogUserNameVerified(bool isverified, string username, string password)
        {
            lock(_xlogUserNameVerifiedLock)
            {
                _xlogUserNameVerified = isverified;
                _verifiedUserName = username;
                _verifiedPassword = password;
            }
        }

        public static bool AreCredentialsVerified(string username, string password)
        {
            lock (_xlogUserNameVerifiedLock)
            {
                return _xlogUserNameVerified && _verifiedUserName != null && _verifiedPassword != null && username == _verifiedUserName && password == _verifiedPassword;
            }
        }

        public static async void TryVerifyXlogUserName()
        {
            await TryVerifyXlogUserNameAsync();
        }

        public static async Task TryVerifyXlogUserNameAsync()
        {
            if(!PostingXlogEntries && !PostingBonesFiles)
            {
                SetXlogUserNameVerified(false, null, null);
                return;
            }
            if (XlogUserNameVerified)
                return;

            string username = XlogUserName;
            string password = XlogPassword;
            if (!string.IsNullOrEmpty(username))
            {
                if(_verifiedUserName != null && _verifiedPassword != null && username == _verifiedUserName && password == _verifiedPassword)
                {
                    lock (_xlogUserNameVerifiedLock)
                    {
                        _xlogUserNameVerified = true;
                    }
                }
                else
                {
                    SendResult res = await SendXLogEntry("", 1, 0, new List<GHPostAttachment>(), true);
                    if (res.IsSuccess)
                        Debug.WriteLine("XLog user name successfully verified.");
                    else
                        Debug.WriteLine("XLog user name verification failed.");
                }
            }
        }

        public static bool IsValidHttpsURL(string uriString)
        {
            try
            {
                bool wellformed = Uri.IsWellFormedUriString(uriString, UriKind.Absolute);
                if (wellformed)
                {
                    Uri uri = new Uri(uriString);
                    if (uri.Scheme == Uri.UriSchemeHttps)
                        return true;
                    else
                        return false;
                }
                else
                    return false;
            }
            catch (Exception)
            {
                return false;
            }
        }

        public static bool IsReadToMemoryBank(SecretsFile sf)
        {
            return sf.streaming_asset != 0 && sf.on_demand == 0 && IsAndroid && ReadStreamingBankToMemory;
        }

        public static bool IsSecretsFileSavedToDisk(SecretsFile sf)
        {
            if (sf == null) return false;
            if (ForceCopyAllBanksToDisk) return true;
            if (IsiOS) return false;
            if (IsAndroid)
            {
                if (IsReadToMemoryBank(sf)) return false;
                if (IsSecretsFileAndroidOnDemand(sf)) return false;
                return sf.streaming_asset != 0 && CopyStreamingBankToDisk;
            }
            return true;
        }

        public static bool IsSecretsFileAndroidOnDemand(SecretsFile sf)
        {
            if (sf == null) return false;
            if (IsiOS) return false;
#if DEBUG
            return false;
#else
            return (IsAndroid && (sf.on_demand & 1) != 0) || (IsiOS && (sf.on_demand & 2) != 0);
#endif
        }

        public static int CountSecretsFilesSavedToDirectory(Secrets secrets, SecretsDirectory sd)
        {
            if (secrets == null) return 0;
            if (sd == null) return 0;
            int cnt = 0;
            foreach (SecretsFile sf in CurrentSecrets.files)
            {
                if (sf.target_directory == sd.name && IsSecretsFileSavedToDisk(sf)) cnt++;
            }
            return cnt;
        }

        public static void SetSoundBanksUpForLoading()
        {
            FmodService.ClearLoadableSoundBanks();
            AddLoadableSoundBanks();
            DeleteBanksFromDisk();
        }

        public static void AddLoadableSoundBanks()
        {
            foreach (SecretsFile sf in CurrentSecrets.files)
            {
                if (sf.type == "sound_bank")
                {
                    if (IsSecretsFileAndroidOnDemand(sf)) //In asset pack directory
                    {
                        string rfile = Path.Combine(sf.source_directory, sf.name);
                        string afile = PlatformService.GetAbsoluteOnDemandAssetPath(GHConstants.OnDemandPackName, rfile);
                        FmodService.AddLoadableSoundBank(afile, sf.subtype_id, false, false);
                    }
                    else if (IsSecretsFileSavedToDisk(sf)) //In gnollhack directory's bank subdirectory
                    {
                        string ghdir = GnollHackService.GetGnollHackPath();
                        string sdir = string.IsNullOrWhiteSpace(sf.target_directory) ? ghdir : Path.Combine(ghdir, sf.target_directory); ;
                        string sfile = Path.Combine(sdir, sf.name);
                        FmodService.AddLoadableSoundBank(sfile, sf.subtype_id, false, false);
                    }
                    else //In assets directory
                    {
#if GNH_MAUI
                        string sdir = Path.Combine(PlatformService.GetAssetsPath(), "Platforms", GHApp.IsAndroid ? "Android" : GHApp.IsiOS ? "iOS" : "Unknown", sf.source_directory);
#else
                        string sdir = Path.Combine(PlatformService.GetAssetsPath(), sf.source_directory);
#endif
                        string sfile = Path.Combine(sdir, sf.name);

                        string rfile = Path.Combine(sf.source_directory, sf.name);
                        if (IsReadToMemoryBank(sf))  //Read to memory first and use from there
                            FmodService.AddLoadableSoundBank(rfile, sf.subtype_id, true, true);
                        else
                            FmodService.AddLoadableSoundBank(sfile, sf.subtype_id, true, false);
                    }
                }
            }
        }

        public static void DeleteBanksFromDisk()
        {
            string ghdir = GHPath;

            foreach (SecretsFile sf in CurrentSecrets.files)
            {
                if (!IsSecretsFileSavedToDisk(sf))
                {
                    try
                    {
                        string sdir = string.IsNullOrWhiteSpace(sf.target_directory) ? ghdir : Path.Combine(ghdir, sf.target_directory); ;
                        string sfile = Path.Combine(sdir, sf.name);
                        if (File.Exists(sfile))
                            File.Delete(sfile);
                    }
                    catch (Exception ex)
                    {
                        Debug.WriteLine(ex.Message);
                    }
                }
            }
            foreach (SecretsDirectory sd in CurrentSecrets.directories)
            {
                if (string.IsNullOrWhiteSpace(sd.name))
                    continue;

                if (CountSecretsFilesSavedToDirectory(CurrentSecrets, sd) > 0)
                    continue;

                string sdir = Path.Combine(ghdir, sd.name);

                if (Directory.Exists(sdir))
                {
                    try
                    {
                        Directory.Delete(sdir);
                    }
                    catch (Exception ex)
                    {
                        Debug.WriteLine(ex.Message);
                    }
                }
            }
            //foreach (string dir in target_directories)
            //{
            //    try
            //    {
            //        if (Directory.Exists(dir))
            //        {
            //            Directory.Delete(dir, true);
            //        }
            //    }
            //    catch (Exception ex)
            //    {
            //        Debug.WriteLine(ex.Message);
            //    }
            //}
        }

        public static async Task<bool> ShareFile(Page page, string filename, string title)
        {
            if (!File.Exists(filename))
            {
                await page.DisplayAlert("File Sharing Failure", "GnollHack cannot find file \'" + filename + "\'", "OK");
                return false;
            }
            await Share.RequestAsync(new ShareFileRequest
            {
                Title = title,
                File = new ShareFile(filename)
            });
            return true;
        }

        public static async Task<PermissionStatus> CheckAndRequestWritePermission(Page page)
        {
            var status = await Permissions.CheckStatusAsync<Permissions.StorageWrite>();

            if (status == PermissionStatus.Granted)
                return status;

            if (status == PermissionStatus.Denied && DeviceInfo.Platform == DevicePlatform.iOS)
            {
                // Prompt the user to turn on in settings
                // On iOS once a permission has been denied it may not be requested again from the application
                await page.DisplayAlert("Permission Needed", "GnollHack needs the file write permission to create a zip file. Please turn it on in Settings.", "OK");
                return status;
            }

            if (Permissions.ShouldShowRationale<Permissions.StorageWrite>())
            {
                await page.DisplayAlert("Permission Needed", "GnollHack needs the file write permission to create a zip file.", "OK");
            }

            status = await Permissions.RequestAsync<Permissions.StorageWrite>();

            return status;
        }
        public static async Task<PermissionStatus> CheckAndRequestReadPermission(Page page)
        {
            var status = await Permissions.CheckStatusAsync<Permissions.StorageRead>();

            if (status == PermissionStatus.Granted)
                return status;

            if (status == PermissionStatus.Denied && DeviceInfo.Platform == DevicePlatform.iOS)
            {
                // Prompt the user to turn on in settings
                // On iOS once a permission has been denied it may not be requested again from the application
                await page.DisplayAlert("Permission Needed", "GnollHack needs the file read permission to work with a zip file. Please turn it on in Settings.", "OK");
                return status;
            }

            if (Permissions.ShouldShowRationale<Permissions.StorageRead>())
            {
                await page.DisplayAlert("Permission Needed", "GnollHack needs the file read permission to work with a zip file.", "OK");
            }

            status = await Permissions.RequestAsync<Permissions.StorageRead>();

            return status;
        }

        public static async Task<bool> CreateCrashReport(Page page)
        {
            await CheckAndRequestWritePermission(page);
            await CheckAndRequestReadPermission(page);
            string archive_file = "";
            try
            {
                archive_file = GHApp.CreateGameZipArchive();
            }
            catch (Exception ex)
            {
                await page.DisplayAlert("Archive Creation Failure", "GnollHack failed to create a crash report archive: " + ex.Message, "OK");
                return false;
            }
            try
            {
                if (archive_file != "")
                    return await ShareFile(page, archive_file, "GnollHack Crash Report");
                else
                    return false;
            }
            catch (Exception ex)
            {
                await page.DisplayAlert("Share File Failure", "GnollHack failed to share a crash report archive: " + ex.Message, "OK");
                return false;
            }
        }

        public static async Task<SendResult> SendXLogEntry(string xlogentry_string, int status_type, int status_datatype, List<GHPostAttachment> xlogattachments, bool is_from_queue)
        {
            SendResult res = new SendResult();
            try
            {
                string postaddress = XlogPostAddress;
                Debug.WriteLine("XlogPostAddress: " + postaddress);
                if (postaddress != null && postaddress.Length > 8 && postaddress.Substring(0, 8) == "https://" && Uri.IsWellFormedUriString(postaddress, UriKind.Absolute))
                {
                    using (HttpClient client = new HttpClient { Timeout = TimeSpan.FromDays(1) })
                    {
                        MultipartFormDataContent multicontent = new MultipartFormDataContent("-------------------boundary");

                        string username = XlogUserName;
                        string password = XlogPassword;
                        StringContent content1 = new StringContent(username, Encoding.UTF8, "text/plain");
                        ContentDispositionHeaderValue cdhv1 = new ContentDispositionHeaderValue("form-data");
                        cdhv1.Name = "UserName";
                        content1.Headers.ContentDisposition = cdhv1;
                        multicontent.Add(content1);
                        Debug.WriteLine("UserName: " + username);

                        StringContent content3 = new StringContent(password, Encoding.UTF8, "text/plain");
                        ContentDispositionHeaderValue cdhv3 = new ContentDispositionHeaderValue("form-data");
                        cdhv3.Name = "Password";
                        content3.Headers.ContentDisposition = cdhv3;
                        multicontent.Add(content3);
                        Debug.WriteLine("Password: " + password);

                        StringContent content4 = new StringContent(XlogAntiForgeryToken, Encoding.UTF8, "text/plain");
                        ContentDispositionHeaderValue cdhv4 = new ContentDispositionHeaderValue("form-data");
                        cdhv4.Name = "AntiForgeryToken";
                        content4.Headers.ContentDisposition = cdhv4;
                        multicontent.Add(content4);
                        Debug.WriteLine("AntiForgeryToken: " + XlogAntiForgeryToken);

                        string adjusted_entry_string = "";
                        if(!string.IsNullOrWhiteSpace(xlogentry_string))
                        {
                            adjusted_entry_string = xlogentry_string.Replace("○", "\t").Replace("◙", Environment.NewLine);
                            adjusted_entry_string = adjusted_entry_string.Replace(Environment.NewLine, "") // Should be just on at the end
                                    + "\tplatform=" + DeviceInfo.Platform.ToString()?.ToLower()
                                    + "\tplatformversion=" + DeviceInfo.VersionString?.ToLower()
                                    + "\tport=" + GHConstants.PortName?.ToLower()
                                    + "\tportversion=" + VersionTracking.CurrentVersion?.ToLower()
                                    + "\tportbuild=" + VersionTracking.CurrentBuild?.ToLower()
                                    + Environment.NewLine;
                        }

                        StringContent content2 = new StringContent(adjusted_entry_string, Encoding.UTF8, "text/plain");
                        ContentDispositionHeaderValue cdhv2 = new ContentDispositionHeaderValue("form-data");
                        cdhv2.Name = "XLogEntry";
                        content2.Headers.ContentDisposition = cdhv2;
                        multicontent.Add(content2);
                        Debug.WriteLine("XLogEntry: " + adjusted_entry_string);

                        List<FileStream> filestreams = new List<FileStream>();
                        List<StreamContent> contents = new List<StreamContent>();

                        if (xlogattachments != null)
                        {
                            foreach (GHPostAttachment attachment in xlogattachments)
                            {
                                string fullFilePath = attachment.FullPath;
                                bool fileexists = File.Exists(fullFilePath);
                                if (fileexists)
                                {
                                    FileInfo fileinfo = new FileInfo(fullFilePath);
                                    string filename = fileinfo.Name;
                                    FileStream stream = new FileStream(fullFilePath, FileMode.Open);
                                    StreamContent content5 = new StreamContent(stream);
                                    filestreams.Add(stream);
                                    contents.Add(content5);
                                    ContentDispositionHeaderValue cdhv5 = new ContentDispositionHeaderValue("form-data");
                                    cdhv5.Name = attachment.ContentType == "text/plain" ? "PlainTextDumpLog" : attachment.ContentType == "text/html" ? "HtmlDumpLog" : "GameData";
                                    cdhv5.FileName = filename;
                                    content5.Headers.ContentDisposition = cdhv5;
                                    multicontent.Add(content5);
                                    Debug.WriteLine("XLog entry, file added: " + cdhv5.Name + ", " + fullFilePath);
                                }
                            }
                        }
                        using (var cts = new CancellationTokenSource())
                        {
                            cts.CancelAfter(string.IsNullOrEmpty(xlogentry_string) ? 5000 : xlogattachments == null || xlogattachments.Count == 0 ? 10000 : 120000);
                            string responseContent = "";
                            //string htmlrequest = await multicontent.ReadAsStringAsync();
                            //Debug.WriteLine(htmlrequest);

                            try
                            {
                                using (HttpResponseMessage response = await client.PostAsync(postaddress, multicontent, cts.Token))
                                {
                                    responseContent = await response.Content.ReadAsStringAsync();
                                    Debug.WriteLine("XLog entry response content:");
                                    Debug.WriteLine(responseContent);
                                    res.IsSuccess = response.IsSuccessStatusCode;
                                    res.HasHttpStatusCode = true;
                                    res.StatusCode = response.StatusCode;
                                }
                            }
                            catch (Exception ex)
                            {
                                Debug.WriteLine("Exception occurred while sending XLog entry: " + ex.Message);
                                res.IsSuccess = false;
                                res.Message = ex.Message;
                            }

                            XlogCredentialsIncorrect = false;
                            if (res.IsSuccess)
                            {
                                SetXlogUserNameVerified(true, username, password);
                                WriteGHLog((string.IsNullOrEmpty(xlogentry_string) ? "Server authentication successful" : "XLog entry successfully sent") + (is_from_queue ? " from the post queue" : "") + ". (" + (int)res.StatusCode + ")");
                            }
                            else
                            {
                                Debug.WriteLine("Sending " + (string.IsNullOrEmpty(xlogentry_string) ? "server authentication" : "XLog entry") + " failed. Status Code: " + (int)res.StatusCode);
                                if (XlogUserNameVerified && res.HasHttpStatusCode && (res.StatusCode == HttpStatusCode.Forbidden /* 403 */)) // || res.StatusCode == HttpStatusCode.Locked /* 423 */
                                    SetXlogUserNameVerified(false, null, null);
                                if (res.StatusCode == HttpStatusCode.Forbidden)
                                    XlogCredentialsIncorrect = true;
                            }

                            if (!res.IsSuccess && !is_from_queue && !string.IsNullOrWhiteSpace(xlogentry_string))
                            {
                                WriteGHLog((string.IsNullOrEmpty(xlogentry_string) ? "Server authentication failed." : "Sending XLog entry failed.") + " Writing the send request to disk. Status Code: " + (int)res.StatusCode + ", Message: "+ res.Message);
                                SaveXLogEntryToDisk(status_type, status_datatype, xlogentry_string, xlogattachments);
                            }                            
                        }
                        content1.Dispose();
                        content2.Dispose();
                        content3.Dispose();
                        content4.Dispose();
                        foreach(FileStream fs in filestreams)
                        {
                            fs.Dispose();
                        }
                        filestreams.Clear();
                        foreach (StreamContent cnt in contents)
                        {
                            cnt.Dispose();
                        }
                        contents.Clear();
                        multicontent.Dispose();
                    }
                }
            }
            catch (Exception e)
            {
                Debug.WriteLine(e.Message);
                res.Message = e.Message;
            }
            if (xlogattachments != null)
            {
                if(res.IsSuccess)
                {
                    foreach (var attachment in xlogattachments)
                    {
                        if (attachment.IsTemporary)
                        {
                            try
                            {
                                File.Delete(attachment.FullPath);
                            }
                            catch (Exception e)
                            {
                                Debug.WriteLine(e.Message);
                            }
                        }
                    }
                }
                xlogattachments.Clear();
            }
            return res;
        }

        public static void SaveXLogEntryToDisk(int status_type, int status_datatype, string xlogentry_string, List<GHPostAttachment> xlogattachments)
        {
            string targetpath = Path.Combine(GHApp.GHPath, GHConstants.XlogPostQueueDirectory);
            if (!Directory.Exists(targetpath))
                CheckCreateDirectory(targetpath);
            if (Directory.Exists(targetpath))
            {
                string targetfilename;
                string targetfilepath;
                int id = 0;
                do
                {
                    targetfilename = GHConstants.XlogPostFileNamePrefix + id + GHConstants.XlogPostFileNameSuffix;
                    targetfilepath = Path.Combine(targetpath, targetfilename);
                    id++;
                } while (File.Exists(targetfilepath));

                try
                {
                    using (StreamWriter sw = File.CreateText(targetfilepath))
                    {
                        GHPost fp = new GHPost(1, true, status_type, status_datatype, xlogentry_string, xlogattachments != null ? xlogattachments : new List<GHPostAttachment>(), false);
                        string json = JsonConvert.SerializeObject(fp);
                        Debug.WriteLine(json);
                        sw.Write(json);
                        WriteGHLog((string.IsNullOrEmpty(xlogentry_string) ? "Server authentication request" : "XLog entry send request") + " written to the queue on disk: " + targetfilepath);
                    }
                }
                catch (Exception ex)
                {
                    WriteGHLog("Writing the " + (string.IsNullOrEmpty(xlogentry_string) ? "server authentication request" : "XLog entry send request") + " to the queue on disk using path " + targetfilepath + " failed: " + ex.Message);
                }
            }
        }

        public static async Task<SendResult> SendForumPost(bool is_game_status, string message, int status_type, int status_datatype, List<GHPostAttachment> forumpostattachments, bool forcesend, bool is_from_queue)
        {
            SendResult res = new SendResult();
            try
            {
                string postaddress = is_game_status ? GHApp.GameStatusPostAddress : GHApp.DiagnosticDataPostAddress;
                if (postaddress != null && postaddress.Length > 8 && postaddress.Substring(0, 8) == "https://" && Uri.IsWellFormedUriString(postaddress, UriKind.Absolute))
                {
                    using (HttpClient client = new HttpClient { Timeout = TimeSpan.FromDays(1) })
                    {
                        HttpContent content = null;
                        StringContent content1 = null;
                        List<FileStream> filestreams = new List<FileStream>();
                        List<StreamContent> contents = new List<StreamContent>();
                        if (forumpostattachments != null && forumpostattachments.Count > 0)
                        {
                            DiscordWebHookPostWithAttachment post = new DiscordWebHookPostWithAttachment(message);
                            foreach (GHPostAttachment attachment in forumpostattachments)
                            {
                                string fullFilePath = attachment.FullPath;
                                FileInfo fileinfo = new FileInfo(fullFilePath);
                                string filename = fileinfo.Name;
                                if (File.Exists(fullFilePath))
                                    post.AddAttachment(attachment.Description, filename);
                            }
                            string json = JsonConvert.SerializeObject(post);
                            MultipartFormDataContent multicontent = new MultipartFormDataContent("-------------------boundary");
                            content1 = new StringContent(json, Encoding.UTF8, "application/json");
                            ContentDispositionHeaderValue cdhv = new ContentDispositionHeaderValue("form-data");
                            cdhv.Name = "payload_json";
                            content1.Headers.ContentDisposition = cdhv;
                            multicontent.Add(content1);
                            int aidx = 0;
                            foreach (GHPostAttachment attachment in forumpostattachments)
                            {
                                string fullFilePath = attachment.FullPath;
                                bool fileexists = File.Exists(fullFilePath);
                                if (fileexists)
                                {
                                    FileInfo fileinfo = new FileInfo(fullFilePath);
                                    string filename = fileinfo.Name;
                                    var stream = new FileStream(fullFilePath, FileMode.Open);
                                    StreamContent content2 = new StreamContent(stream);
                                    filestreams.Add(stream);
                                    contents.Add(content2);
                                    ContentDispositionHeaderValue cdhv2 = new ContentDispositionHeaderValue("form-data");
                                    cdhv2.Name = "files[" + aidx + "]";
                                    cdhv2.FileName = filename;
                                    content2.Headers.ContentDisposition = cdhv2;
                                    content2.Headers.ContentType = new MediaTypeHeaderValue(attachment.ContentType);
                                    multicontent.Add(content2);
                                    aidx++;
                                }
                            }
                            content = multicontent;
                        }
                        else
                        {
                            DiscordWebHookPost post = new DiscordWebHookPost(message);
                            string json = JsonConvert.SerializeObject(post);
                            content = new StringContent(json, Encoding.UTF8, "application/json");
                        }

                        using (var cts = new CancellationTokenSource())
                        {
                            cts.CancelAfter(is_game_status || forumpostattachments == null || forumpostattachments.Count == 0 ? 10000 : 120000);
                            string responseContent = "";

                            try
                            {
                                using (HttpResponseMessage response = await client.PostAsync(postaddress, content, cts.Token))
                                {
                                    responseContent = await response.Content.ReadAsStringAsync();
                                    Debug.WriteLine(responseContent);
                                    res.IsSuccess = response.IsSuccessStatusCode;
                                    res.HasHttpStatusCode = true;
                                    res.StatusCode = response.StatusCode;
                                }
                            }
                            catch (Exception ex)
                            {
                                Debug.WriteLine(ex.Message);
                                res.IsSuccess = false;
                                res.Message = ex.Message;
                            }

                            if(res.IsSuccess)
                            {
                                WriteGHLog("Forum post successfully sent" + (is_from_queue ? " from the post queue" : "") + ". (" + (int)res.StatusCode + ")");
                            }

                            if (!res.IsSuccess && !is_from_queue)
                            {
                                WriteGHLog("Forum post send request redirected to the queue on disk. Status Code: " + (int)res.StatusCode + ", Message: " + res.Message);
                                SaveForumPostToDisk(is_game_status, status_type, status_datatype, message, forumpostattachments, forcesend);
                            }
                        }
                        if(content1 != null)
                            content1.Dispose();
                        if (content != null)
                            content.Dispose();
                        foreach (FileStream fs in filestreams)
                        {
                            fs.Dispose();
                        }
                        filestreams.Clear();
                        foreach (StreamContent cnt in contents)
                        {
                            cnt.Dispose();
                        }
                        contents.Clear();
                    }
                }
            }
            catch (Exception e)
            {
                Debug.WriteLine(e.Message);
                res.Message = e.Message;
            }

            if (forumpostattachments != null)
            {
                if(res.IsSuccess)
                {
                    foreach (var attachment in forumpostattachments)
                    {
                        if (attachment.IsTemporary)
                        {
                            try
                            {
                                File.Delete(attachment.FullPath);
                            }
                            catch (Exception e)
                            {
                                Debug.WriteLine(e.Message);
                            }
                        }
                    }
                }
                forumpostattachments.Clear();
            }

            return res;
        }

        public static void SaveForumPostToDisk(bool is_game_status, int status_type, int status_datatype, string message, List<GHPostAttachment> forumpostattachments, bool forcesend)
        {
            string targetpath = Path.Combine(GHApp.GHPath, GHConstants.ForumPostQueueDirectory);
            if (!Directory.Exists(targetpath))
                GHApp.CheckCreateDirectory(targetpath);
            if (Directory.Exists(targetpath))
            {
                string targetfilename;
                string targetfilepath;
                int id = 0;
                do
                {
                    targetfilename = GHConstants.ForumPostFileNamePrefix + id + GHConstants.ForumPostFileNameSuffix;
                    targetfilepath = Path.Combine(targetpath, targetfilename);
                    id++;
                } while (File.Exists(targetfilepath));

                try
                {
                    using (StreamWriter sw = File.CreateText(targetfilepath))
                    {
                        GHPost fp = new GHPost(0, is_game_status, status_type, status_datatype, message, forumpostattachments != null ? forumpostattachments : new List<GHPostAttachment>(), forcesend);
                        string json = JsonConvert.SerializeObject(fp);
                        sw.Write(json);
                        WriteGHLog("Forum post send request written to the queue on disk: " + targetfilepath);
                    }
                }
                catch (Exception ex)
                {
                    WriteGHLog("Writing the forum post send request to the queue on disk using path " + targetfilepath + " failed: " + ex.Message);
                }
            }
        }

        public static async Task<SendResult> SendBonesFile(string bones_filename, int status_type, int status_datatype, bool is_from_queue)
        {
            SendResult res = new SendResult();
            bool didReceiveBonesFile = false;
            bool didWriteBonesFileSuccessfully = false;
            string receivedBonesServerFilePath = null;
            try
            {
                string username = XlogUserName;
                string password = XlogPassword;
                string postaddress = BonesPostAddress;
                Debug.WriteLine("BonesPostAddress: " + postaddress);
                if (postaddress != null && postaddress.Length > 8 && postaddress.Substring(0, 8) == "https://" && Uri.IsWellFormedUriString(postaddress, UriKind.Absolute))
                {
                    using (HttpClient client = new HttpClient { Timeout = TimeSpan.FromDays(1) })
                    {
                        MultipartFormDataContent multicontent = new MultipartFormDataContent("-------------------boundary");

                        StringContent content1 = new StringContent(username, Encoding.UTF8, "text/plain");
                        ContentDispositionHeaderValue cdhv1 = new ContentDispositionHeaderValue("form-data");
                        cdhv1.Name = "UserName";
                        content1.Headers.ContentDisposition = cdhv1;
                        multicontent.Add(content1);
                        Debug.WriteLine("UserName: " + username);

                        StringContent content3 = new StringContent(password, Encoding.UTF8, "text/plain");
                        ContentDispositionHeaderValue cdhv3 = new ContentDispositionHeaderValue("form-data");
                        cdhv3.Name = "Password";
                        content3.Headers.ContentDisposition = cdhv3;
                        multicontent.Add(content3);
                        Debug.WriteLine("Password: " + password);

                        StringContent content4 = new StringContent(XlogAntiForgeryToken, Encoding.UTF8, "text/plain");
                        ContentDispositionHeaderValue cdhv4 = new ContentDispositionHeaderValue("form-data");
                        cdhv4.Name = "AntiForgeryToken";
                        content4.Headers.ContentDisposition = cdhv4;
                        multicontent.Add(content4);
                        Debug.WriteLine("AntiForgeryToken: " + XlogAntiForgeryToken);

                        StringContent content2 = new StringContent("SendBonesFile", Encoding.UTF8, "text/plain");
                        ContentDispositionHeaderValue cdhv2 = new ContentDispositionHeaderValue("form-data");
                        cdhv2.Name = "Command";
                        content2.Headers.ContentDisposition = cdhv2;
                        multicontent.Add(content2);
                        Debug.WriteLine("Command: SendBonesFile");

                        StringContent content5 = new StringContent(status_type.ToString(), Encoding.UTF8, "text/plain");
                        ContentDispositionHeaderValue cdhv5 = new ContentDispositionHeaderValue("form-data");
                        cdhv5.Name = "Data";
                        content5.Headers.ContentDisposition = cdhv5;
                        multicontent.Add(content5);
                        Debug.WriteLine("Data: " + status_type.ToString());

                        StringContent contentE1 = new StringContent(DeviceInfo.Platform.ToString(), Encoding.UTF8, "text/plain");
                        ContentDispositionHeaderValue cdhve1 = new ContentDispositionHeaderValue("form-data");
                        cdhve1.Name = "Platform";
                        contentE1.Headers.ContentDisposition = cdhve1;
                        multicontent.Add(contentE1);
                        Debug.WriteLine("Platform: " + DeviceInfo.Platform.ToString());

                        StringContent contentE2 = new StringContent(DeviceInfo.VersionString, Encoding.UTF8, "text/plain");
                        ContentDispositionHeaderValue cdhve2 = new ContentDispositionHeaderValue("form-data");
                        cdhve2.Name = "PlatformVersion";
                        contentE2.Headers.ContentDisposition = cdhve2;
                        multicontent.Add(contentE2);
                        Debug.WriteLine("PlatformVersion: " + DeviceInfo.VersionString);

                        StringContent contentE3 = new StringContent(GHConstants.PortName, Encoding.UTF8, "text/plain");
                        ContentDispositionHeaderValue cdhve3 = new ContentDispositionHeaderValue("form-data");
                        cdhve3.Name = "Port";
                        contentE3.Headers.ContentDisposition = cdhve3;
                        multicontent.Add(contentE3);
                        Debug.WriteLine("Port: " + GHConstants.PortName);

                        StringContent contentE4 = new StringContent(VersionTracking.CurrentVersion, Encoding.UTF8, "text/plain");
                        ContentDispositionHeaderValue cdhve4 = new ContentDispositionHeaderValue("form-data");
                        cdhve4.Name = "PortVersion";
                        contentE4.Headers.ContentDisposition = cdhve4;
                        multicontent.Add(contentE4);
                        Debug.WriteLine("PortVersion: " + VersionTracking.CurrentVersion);

                        StringContent contentE5 = new StringContent(VersionTracking.CurrentBuild, Encoding.UTF8, "text/plain");
                        ContentDispositionHeaderValue cdhve5 = new ContentDispositionHeaderValue("form-data");
                        cdhve5.Name = "PortBuild";
                        contentE5.Headers.ContentDisposition = cdhve5;
                        multicontent.Add(contentE5);
                        Debug.WriteLine("PortBuild: " + VersionTracking.CurrentBuild);

                        StringContent contentE6 = new StringContent(GHApp.GHVersionNumber.ToString(), Encoding.UTF8, "text/plain");
                        ContentDispositionHeaderValue cdhve6 = new ContentDispositionHeaderValue("form-data");
                        cdhve6.Name = "VersionNumber";
                        contentE6.Headers.ContentDisposition = cdhve6;
                        multicontent.Add(contentE6);
                        Debug.WriteLine("VersionNumber: " + GHApp.GHVersionNumber.ToString());

                        StringContent contentE7 = new StringContent(GHApp.GHVersionCompatibility.ToString(), Encoding.UTF8, "text/plain");
                        ContentDispositionHeaderValue cdhve7 = new ContentDispositionHeaderValue("form-data");
                        cdhve7.Name = "VersionCompatibilityNumber";
                        contentE7.Headers.ContentDisposition = cdhve7;
                        multicontent.Add(contentE7);
                        Debug.WriteLine("VersionCompatibilityNumber: " + GHApp.GHVersionCompatibility.ToString());

                        List<FileStream> filestreams = new List<FileStream>();
                        List<StreamContent> contents = new List<StreamContent>();

                        string full_filepath = Path.Combine(GHApp.GHPath, bones_filename);
                        bool fileexists = File.Exists(full_filepath);
                        if (fileexists)
                        {
                            FileInfo fileinfo = new FileInfo(full_filepath);
                            string filename = fileinfo.Name;
                            FileStream stream = new FileStream(full_filepath, FileMode.Open);
                            StreamContent content6 = new StreamContent(stream);
                            filestreams.Add(stream);
                            contents.Add(content6);
                            ContentDispositionHeaderValue cdhv6 = new ContentDispositionHeaderValue("form-data");
                            cdhv6.Name = "BonesFile";
                            cdhv6.FileName = filename;
                            content6.Headers.ContentDisposition = cdhv6;
                            multicontent.Add(content6);
                            Debug.WriteLine("Bones file added: " + cdhv6.Name + ", " + bones_filename);
                        }
                        using (var cts = new CancellationTokenSource())
                        {
                            cts.CancelAfter(string.IsNullOrEmpty(bones_filename) ? 10000 : 120000);
                            byte[] bytearray = null;
                            try
                            {
                                using (HttpResponseMessage response = await client.PostAsync(postaddress, multicontent, cts.Token))
                                {
                                    if(response.Content.Headers.ContentType.MediaType == "application/octet-stream")
                                        bytearray = await response.Content.ReadAsByteArrayAsync();
                                    res.IsSuccess = response.IsSuccessStatusCode;
                                    res.HasHttpStatusCode = true;
                                    res.StatusCode = response.StatusCode;
                                    if(res.IsSuccess)
                                    {
                                        WriteGHLog("Bones file successfully sent" + (is_from_queue ? " from the post queue" : "") + ". (" + (int)res.StatusCode + "): " + full_filepath);
                                        if (res.StatusCode == HttpStatusCode.OK)
                                        {
                                            // Delete sent file first on OK status code
                                            try
                                            {
                                                File.Delete(full_filepath);
                                                WriteGHLog("Deleted the sent bones file: " + full_filepath);
                                            }
                                            catch (Exception ex)
                                            {
                                                WriteGHLog("Deleting the sent bones file from client failed: " + ex.Message);
                                            }
                                        }
                                        else
                                        {
                                            Debug.WriteLine("Not deleting the sent bones file because status code was " + (int)res.StatusCode + " (" + res.StatusCode.ToString() + ")");
                                        }

                                        //We may or may not have received another bones file in return
                                        if (bytearray != null && bytearray.Length > 0)
                                        {
                                            WriteGHLog("Bones file received from the server. Writing the bones file to disk.");
                                            didReceiveBonesFile = true;
                                            Debug.WriteLine("Response Headers: " + response.Headers.ToString());
                                            if (response.Headers.TryGetValues("X-GH-OriginalFileName", out IEnumerable<string> origFileNames))
                                            {
                                                if (origFileNames != null)
                                                {
                                                    List<string> list = origFileNames.ToList();
                                                    if (list.Count > 0)
                                                    {
                                                        string filename = list[0];
                                                        if (!string.IsNullOrWhiteSpace(filename))
                                                        {
                                                            string savepath = Path.Combine(GHApp.GHPath, filename);
                                                            if (!File.Exists(savepath))
                                                            {
                                                                Debug.WriteLine("Starting writing bones byte array into file: " + savepath);
                                                                try
                                                                {
                                                                    using (FileStream fs = File.OpenWrite(savepath))
                                                                    {
                                                                        await fs.WriteAsync(bytearray, 0, bytearray.Length);
                                                                    }
                                                                    didWriteBonesFileSuccessfully = true;

                                                                    if(response.Headers.TryGetValues("X-GH-BonesFilePath", out IEnumerable<string> bonesfilepathienum))
                                                                    {
                                                                        var bonesfilepathlist = bonesfilepathienum.ToList();
                                                                        if(bonesfilepathlist.Count > 0)
                                                                            receivedBonesServerFilePath = bonesfilepathlist[0];
                                                                    }
                                                                }
                                                                catch (Exception ex)
                                                                {
                                                                    WriteGHLog("Writing the received bones file " + savepath + " to disk failed: " + ex.Message);
                                                                }
                                                            }
                                                            else
                                                                WriteGHLog("Bones file already exists: " + savepath + ". Ignoring the received bones file.");
                                                        }
                                                        else
                                                            WriteGHLog("Bones file name is null or empty.");
                                                    }
                                                    else
                                                        WriteGHLog("Bones original file name list is empty in the server response.");
                                                }
                                                else
                                                    WriteGHLog("Bones original file name list is null in the server response.");
                                            }
                                            else
                                                WriteGHLog("Bones original file name header could not be found in the server response.");
                                        }
                                        else
                                        {
                                            WriteGHLog("No bones file received from the server: Bones byte array was null or empty.");
                                            string str = "";
                                            try
                                            {
                                                str = await response.Content.ReadAsStringAsync();
                                            }
                                            catch (Exception ex)
                                            {
                                                WriteGHLog("Reading bones response content failed: " + ex.Message);
                                            }
                                            WriteGHLog("Bones response content: " + str);
                                        }
                                    }
                                    else
                                    {
                                        Debug.WriteLine("Sending the bones file " + full_filepath + " failed. No bones file received in exchange. (" + (int)res.StatusCode + ")");
                                        string str = "";
                                        try
                                        {
                                            str = await response.Content.ReadAsStringAsync();
                                        }
                                        catch (Exception ex) 
                                        {
                                            Debug.WriteLine("Reading bones response content failed: " + ex.Message);
                                        }
                                        Debug.WriteLine("Bones response content: " + str);
                                    }
                                }
                            }
                            catch (Exception ex)
                            {
                                Debug.WriteLine("Exception occurred while sending bones file: " + ex.Message);
                                res.IsSuccess = false;
                                res.Message = ex.Message;
                            }

                            XlogCredentialsIncorrect = false;
                            if (res.IsSuccess)
                            {
                                SetXlogUserNameVerified(true, username, password);
                                WriteGHLog("Bones file exchange successfully completed. (" + (int)res.StatusCode + ")");
                            }
                            else
                            {
                                Debug.WriteLine("Sending bones file failed. Status Code: " + (int)res.StatusCode + ", Message: " + res.Message);
                                if (XlogUserNameVerified && res.HasHttpStatusCode && (res.StatusCode == HttpStatusCode.Forbidden /* 403 */)) // || res.StatusCode == HttpStatusCode.Locked /* 423 */
                                    SetXlogUserNameVerified(false, null, null);
                                if (res.StatusCode == HttpStatusCode.Forbidden)
                                    XlogCredentialsIncorrect = true;
                            }

                            if (!res.IsSuccess && !is_from_queue && !string.IsNullOrWhiteSpace(bones_filename))
                            {
                                WriteGHLog("Bones file send request redirected to the queue on disk. Status Code: " + (int)res.StatusCode + ", Message: " + res.Message);
                                SaveBonesPostToDisk(status_type, status_datatype, bones_filename);
                            }
                        }
                        content1.Dispose();
                        content2.Dispose();
                        content3.Dispose();
                        content4.Dispose();
                        content5.Dispose();
                        contentE1.Dispose();
                        contentE2.Dispose();
                        contentE3.Dispose();
                        contentE4.Dispose();
                        contentE5.Dispose();
                        contentE6.Dispose();
                        contentE7.Dispose();
                        foreach (FileStream fs in filestreams)
                        {
                            fs.Dispose();
                        }
                        filestreams.Clear();
                        foreach (StreamContent cnt in contents)
                        {
                            cnt.Dispose();
                        }
                        contents.Clear();
                        multicontent.Dispose();
                    }
                }
                if (didReceiveBonesFile)
                {
                    if (didWriteBonesFileSuccessfully && receivedBonesServerFilePath != null)
                    {
                        // Confirm with server that the file was successfully written to disk
                        Debug.WriteLine("Starting confirming received bones file");
                        using (HttpClient client = new HttpClient { Timeout = TimeSpan.FromDays(1) })
                        {
                            MultipartFormDataContent multicontent = new MultipartFormDataContent("-------------------boundary");

                            StringContent content1 = new StringContent(username, Encoding.UTF8, "text/plain");
                            ContentDispositionHeaderValue cdhv1 = new ContentDispositionHeaderValue("form-data");
                            cdhv1.Name = "UserName";
                            content1.Headers.ContentDisposition = cdhv1;
                            multicontent.Add(content1);
                            Debug.WriteLine("UserName: " + username);

                            StringContent content3 = new StringContent(password, Encoding.UTF8, "text/plain");
                            ContentDispositionHeaderValue cdhv3 = new ContentDispositionHeaderValue("form-data");
                            cdhv3.Name = "Password";
                            content3.Headers.ContentDisposition = cdhv3;
                            multicontent.Add(content3);
                            Debug.WriteLine("Password: " + password);

                            StringContent content4 = new StringContent(XlogAntiForgeryToken, Encoding.UTF8, "text/plain");
                            ContentDispositionHeaderValue cdhv4 = new ContentDispositionHeaderValue("form-data");
                            cdhv4.Name = "AntiForgeryToken";
                            content4.Headers.ContentDisposition = cdhv4;
                            multicontent.Add(content4);
                            Debug.WriteLine("AntiForgeryToken: " + XlogAntiForgeryToken);


                            StringContent content2 = new StringContent("ConfirmReceipt", Encoding.UTF8, "text/plain");
                            ContentDispositionHeaderValue cdhv2 = new ContentDispositionHeaderValue("form-data");
                            cdhv2.Name = "Command";
                            content2.Headers.ContentDisposition = cdhv2;
                            multicontent.Add(content2);
                            Debug.WriteLine("Command: ConfirmReceipt");

                            StringContent content5 = new StringContent(receivedBonesServerFilePath, Encoding.UTF8, "text/plain");
                            ContentDispositionHeaderValue cdhv5 = new ContentDispositionHeaderValue("form-data");
                            cdhv5.Name = "Data";
                            content5.Headers.ContentDisposition = cdhv5;
                            multicontent.Add(content5);
                            Debug.WriteLine("Data: " + receivedBonesServerFilePath);

                            StringContent contentE1 = new StringContent(DeviceInfo.Platform.ToString(), Encoding.UTF8, "text/plain");
                            ContentDispositionHeaderValue cdhve1 = new ContentDispositionHeaderValue("form-data");
                            cdhve1.Name = "Platform";
                            contentE1.Headers.ContentDisposition = cdhve1;
                            multicontent.Add(contentE1);
                            Debug.WriteLine("Platform: " + DeviceInfo.Platform.ToString());

                            StringContent contentE2 = new StringContent(DeviceInfo.VersionString, Encoding.UTF8, "text/plain");
                            ContentDispositionHeaderValue cdhve2 = new ContentDispositionHeaderValue("form-data");
                            cdhve2.Name = "PlatformVersion";
                            contentE2.Headers.ContentDisposition = cdhve2;
                            multicontent.Add(contentE2);
                            Debug.WriteLine("PlatformVersion: " + DeviceInfo.VersionString);

                            StringContent contentE3 = new StringContent(GHConstants.PortName, Encoding.UTF8, "text/plain");
                            ContentDispositionHeaderValue cdhve3 = new ContentDispositionHeaderValue("form-data");
                            cdhve3.Name = "Port";
                            contentE3.Headers.ContentDisposition = cdhve3;
                            multicontent.Add(contentE3);
                            Debug.WriteLine("Port: " + GHConstants.PortName);

                            StringContent contentE4 = new StringContent(VersionTracking.CurrentVersion, Encoding.UTF8, "text/plain");
                            ContentDispositionHeaderValue cdhve4 = new ContentDispositionHeaderValue("form-data");
                            cdhve4.Name = "PortVersion";
                            contentE4.Headers.ContentDisposition = cdhve4;
                            multicontent.Add(contentE4);
                            Debug.WriteLine("PortVersion: " + VersionTracking.CurrentVersion);

                            StringContent contentE5 = new StringContent(VersionTracking.CurrentBuild, Encoding.UTF8, "text/plain");
                            ContentDispositionHeaderValue cdhve5 = new ContentDispositionHeaderValue("form-data");
                            cdhve5.Name = "PortBuild";
                            contentE5.Headers.ContentDisposition = cdhve5;
                            multicontent.Add(contentE5);
                            Debug.WriteLine("PortBuild: " + VersionTracking.CurrentBuild);

                            StringContent contentE6 = new StringContent(GHApp.GHVersionNumber.ToString(), Encoding.UTF8, "text/plain");
                            ContentDispositionHeaderValue cdhve6 = new ContentDispositionHeaderValue("form-data");
                            cdhve6.Name = "VersionNumber";
                            contentE6.Headers.ContentDisposition = cdhve6;
                            multicontent.Add(contentE6);
                            Debug.WriteLine("VersionNumber: " + GHApp.GHVersionNumber.ToString());

                            StringContent contentE7 = new StringContent(GHApp.GHVersionCompatibility.ToString(), Encoding.UTF8, "text/plain");
                            ContentDispositionHeaderValue cdhve7 = new ContentDispositionHeaderValue("form-data");
                            cdhve7.Name = "VersionCompatibilityNumber";
                            contentE7.Headers.ContentDisposition = cdhve7;
                            multicontent.Add(contentE7);
                            Debug.WriteLine("VersionCompatibilityNumber: " + GHApp.GHVersionCompatibility.ToString());

                            using (var cts = new CancellationTokenSource())
                            {
                                cts.CancelAfter(10000);
                                try
                                {
                                    using (HttpResponseMessage response = await client.PostAsync(postaddress, multicontent, cts.Token))
                                    {
                                        if(response.IsSuccessStatusCode)
                                        {
                                            WriteGHLog("Bones receipt confirmation of server bones file " + receivedBonesServerFilePath + " sent successfully (" + (int)response.StatusCode + ").");
                                        }
                                        else
                                        {
                                            Debug.WriteLine("Sending bones receipt confirmation failed. Status code: " + (int)response.StatusCode + " (" + response.StatusCode.ToString() + ")");
                                            string str = "";
                                            try
                                            {
                                                str = await response.Content.ReadAsStringAsync();
                                            }
                                            catch (Exception ex)
                                            {
                                                Debug.WriteLine("Reading the response content for the bones receipt confirmation failed: " + ex.Message);
                                            }
                                            Debug.WriteLine("Bones receipt confirmation response content: " + str);
                                        }
                                    }
                                }
                                catch (Exception ex)
                                {
                                    Debug.WriteLine("Exception occurred while confirming received bones file (" + receivedBonesServerFilePath + "): " + ex.Message);
                                }
                            }
                            content1.Dispose();
                            content2.Dispose();
                            content3.Dispose();
                            content4.Dispose();
                            content5.Dispose();
                            contentE1.Dispose();
                            contentE2.Dispose();
                            contentE3.Dispose();
                            contentE4.Dispose();
                            contentE5.Dispose();
                            contentE6.Dispose();
                            contentE7.Dispose();
                            multicontent.Dispose();
                        }
                    }
                }
            }
            catch (Exception e)
            {
                Debug.WriteLine(e.Message);
                res.Message = e.Message;
            }
            return res;
        }

        public static void SaveBonesPostToDisk(int status_type, int status_datatype, string bones_filename)
        {
            string targetpath = Path.Combine(GHApp.GHPath, GHConstants.BonesPostQueueDirectory);
            if (!Directory.Exists(targetpath))
                CheckCreateDirectory(targetpath);
            if (Directory.Exists(targetpath))
            {
                string targetfilename;
                string targetfilepath;
                int id = 0;
                do
                {
                    targetfilename = GHConstants.BonesPostFileNamePrefix + id + GHConstants.BonesPostFileNameSuffix;
                    targetfilepath = Path.Combine(targetpath, targetfilename);
                    id++;
                } while (File.Exists(targetfilepath));

                try
                {
                    using (StreamWriter sw = File.CreateText(targetfilepath))
                    {
                        GHPost fp = new GHPost(2, true, status_type, status_datatype, bones_filename, null, false);
                        string json = JsonConvert.SerializeObject(fp);
                        Debug.WriteLine(json);
                        sw.Write(json);
                    }
                    WriteGHLog("Bones file send request written to the queue on disk: " + targetfilepath);
                }
                catch (Exception ex)
                {
                    WriteGHLog("Writing the bones file send request to the queue on disk using path " + targetfilepath + " failed: " + ex.Message);
                }
            }
        }


        public static string AddDiagnosticInfo(string info_str, int status_type)
        {
            if (info_str == null)
                info_str = "";

            string ver = GHApp.GHVersionString + " / " + VersionTracking.CurrentVersion + " / " + VersionTracking.CurrentBuild;
            string manufacturer = DeviceInfo.Manufacturer;
            if (manufacturer.Length > 0)
                manufacturer = manufacturer.Substring(0, 1).ToUpper() + manufacturer.Substring(1);
            string device_model = manufacturer + " " + DeviceInfo.Model;
            string platform_with_version = DeviceInfo.Platform + " " + DeviceInfo.VersionString;

            ulong TotalMemInBytes = GHApp.PlatformService.GetDeviceMemoryInBytes();
            ulong TotalMemInMB = (TotalMemInBytes / 1024) / 1024;
            ulong FreeDiskSpaceInBytes = GHApp.PlatformService.GetDeviceFreeDiskSpaceInBytes();
            ulong FreeDiskSpaceInGB = ((FreeDiskSpaceInBytes / 1024) / 1024) / 1024;
            ulong TotalDiskSpaceInBytes = GHApp.PlatformService.GetDeviceTotalDiskSpaceInBytes();
            ulong TotalDiskSpaceInGB = ((TotalDiskSpaceInBytes / 1024) / 1024) / 1024;

            string totmem = TotalMemInMB + " MB";
            string diskspace = FreeDiskSpaceInGB + " GB" + " / " + TotalDiskSpaceInGB + " GB";

            string player_name = Preferences.Get("LastUsedPlayerName", "Unknown Player");
            string info = ver + ", " + platform_with_version + ", " + device_model + ", " + totmem + ", " + diskspace;

            switch (status_type)
            {
                case (int)diagnostic_data_types.DIAGNOSTIC_DATA_PANIC:
                    info_str = player_name + " - Panic: " + info_str + " [" + info + "]";
                    break;
                case (int)diagnostic_data_types.DIAGNOSTIC_DATA_IMPOSSIBLE:
                    info_str = player_name + " - Impossible: " + info_str + " [" + info + "]";
                    break;
                case (int)diagnostic_data_types.DIAGNOSTIC_DATA_CRITICAL:
                    info_str = player_name + " - Critical:\n" + info_str + "\n[" + info + "]";
                    break;
                default:
                    info_str = player_name + " - Diagnostics: " + info_str + " [" + info + "]";
                    break;
            }
            return info_str;
        }

        private static readonly object _ghlogLock = new object();
        public static void WriteGHLog(string loggedtext)
        {
            try
            {
                lock (_ghlogLock)
                {
                    Debug.WriteLine(loggedtext);
                    string logdir = Path.Combine(GHPath, GHConstants.AppLogDirectory);
                    string logfullpath = Path.Combine(logdir, GHConstants.AppLogFileName);
                    if (!Directory.Exists(logdir))
                    {
                        CheckCreateDirectory(logdir);
                    }
                    if (Directory.Exists(logdir))
                    {
                        if (File.Exists(logfullpath))
                        {
                            FileInfo fi = new FileInfo(logfullpath);
                            if (fi.Length > GHConstants.MaxGHLogSize)
                            {
                                string[] lines = File.ReadAllLines(logfullpath);
                                File.Delete(logfullpath);
                                List<string> halflines = new List<string>(lines.Length / 2 + 1);
                                for (int i = lines.Length / 2; i < lines.Length; i++)
                                {
                                    halflines.Add(lines[i]);
                                }
                                File.AppendAllLines(logfullpath, halflines);
                            }
                        }
                        var now = DateTime.UtcNow;
                        File.AppendAllText(logfullpath, now.ToString("yyyy-MM-dd HH:mm:ss") + ": "
                            + loggedtext
                            + " [" + VersionTracking.CurrentVersion + "]"
                            + Environment.NewLine);
                    }
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
        }
    }

    class SecretsFileSizeComparer : IComparer<SecretsFile>
    {
        public SecretsFileSizeComparer()
        {

        }

        public int Compare(SecretsFile x, SecretsFile y)
        {
            return y.length - x.length;
        }
    }

}
