using GnollHackClient.Data;
using GnollHackCommon;
using SkiaSharp;
using System;
using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Reflection;
using System.Threading.Tasks;
using Xamarin.Essentials;
using Xamarin.Forms;
using Xamarin.Forms.Xaml;
using System.Net.Http;
using Newtonsoft.Json;
using System.Diagnostics;
using System.Collections;
using System.IO.Compression;
using System.Globalization;

[assembly: ExportFont("diablo_h.ttf", Alias = "Diablo")]
[assembly: ExportFont("uwch.ttf", Alias = "Underwood")]
[assembly: ExportFont("Immortal-Regular.ttf", Alias = "Immortal")]
[assembly: ExportFont("endr.ttf", Alias = "Endor")]
[assembly: ExportFont("shxi.ttf", Alias = "Xizor")]
[assembly: ExportFont("Lato-Regular.ttf", Alias = "Lato-Regular")]
[assembly: ExportFont("Lato-Bold.ttf", Alias = "Lato-Bold")]
[assembly: ExportFont("DejaVuSansMono.ttf", Alias = "DejaVuSansMono")]
[assembly: ExportFont("DejaVuSansMono-Bold.ttf", Alias = "DejaVuSansMono-Bold")]
[assembly: ExportFont("archristy.ttf", Alias = "ARChristy")]
namespace GnollHackClient
{
    public delegate Task<bool> BackButtonHandler(object sender, EventArgs e);

    public partial class App : Application
    {
        public App()
        {
            InitializeComponent();
            VersionTracking.Track();
            App.GetDependencyServices();

            Assembly assembly = GetType().GetTypeInfo().Assembly;
            App.InitBaseTypefaces(assembly);

            var mainPage = new MainPage();
            var navPage = new NavigationPage(mainPage);
            navPage.BarTextColor = Color.White;
            navPage.BarBackgroundColor = Color.Black;
            MainPage = navPage;

            App.HideAndroidNavigationBar = Preferences.Get("HideAndroidNavigationBar", GHConstants.DefaultHideNavigation);
            App.HideiOSStatusBar = Preferences.Get("HideiOSStatusBar", GHConstants.DefaultHideStatusBar);
            App.DeveloperMode = Preferences.Get("DeveloperMode", GHConstants.DefaultDeveloperMode);
            App.FullVersionMode = true; // Preferences.Get("FullVersion", true);
            App.ClassicMode = Preferences.Get("ClassicMode", false);
            App.CasualMode = Preferences.Get("CasualMode", false);
            App.LoadBanks = Preferences.Get("LoadSoundBanks", true);
            App.InformAboutGameTermination = Preferences.Get("WentToSleepWithGameOn", false);
            Preferences.Set("WentToSleepWithGameOn", false);
            App.UsesCarousel = true; // App.IsiOS || Preferences.Get("UsesCarousel", true);

            App.BackButtonPressed += App.EmptyBackButtonPressed;
        }

        private static readonly object _currentClientGameLock = new object();
        private static ClientGame _currentClientGame = null;
        public static ClientGame CurrentClientGame { get { lock (_currentClientGameLock) { return _currentClientGame; } } set { lock (_currentClientGameLock) { _currentClientGame = value; } } }
        public static bool InformAboutGameTermination = false;


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
            using (Stream stream = assembly.GetManifestResourceStream(GHConstants.GHSettingsResourcePath))
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

        private static readonly object _gameSavedLock = new object();
        private static bool _gameSaved = false;
        public static bool GameSaved { get { lock (_gameSavedLock) { return _gameSaved; } } set { lock (_gameSavedLock) { _gameSaved = value; } } }

        protected override void OnStart()
        {
            if (PlatformService != null)
                PlatformService.OverrideAnimatorDuration();

            App.CancelSaveGame = true;
            App.SleepMuteMode = false;
            if (App.CurrentClientGame != null && !App.CurrentClientGame.CasualMode)
            {
                //Detect background app killing OS, check if last exit is through going to sleep, and notify player that the app probably had been terminated by OS but game has been saved
                bool wenttosleep = Preferences.Get("WentToSleepWithGameOn", false);
                Preferences.Set("WentToSleepWithGameOn", false);
                if (wenttosleep && (App.GameSaved || App.SavingGame))
                {
                    App.CurrentClientGame.GamePage.StopWaitAndResumeSavedGame();
                }
            }
        }

        protected override void OnSleep()
        {
            if (PlatformService != null)
                PlatformService.RevertAnimatorDuration(false);

            App.CancelSaveGame = false;
            App.SleepMuteMode = true;
            if (App.CurrentClientGame != null && !App.CurrentClientGame.CasualMode)
            {
                //Detect background app killing OS, mark that exit has been through going to sleep, and save the game
                Preferences.Set("WentToSleepWithGameOn", true);
                App.CurrentClientGame.GamePage.SaveGameAndWaitForResume();
            }
        }

        protected override void OnResume()
        {
            if (PlatformService != null)
                PlatformService.OverrideAnimatorDuration();

            App.CancelSaveGame = true;
            App.SleepMuteMode = false;
            if (App.CurrentClientGame != null && !App.CurrentClientGame.CasualMode)
            {
                //Detect background app killing OS, check if last exit is through going to sleep & game has been saved, and load previously saved game
                bool wenttosleep = Preferences.Get("WentToSleepWithGameOn", false);
                Preferences.Set("WentToSleepWithGameOn", false);
                if (wenttosleep && (App.GameSaved || App.SavingGame))
                {
                    App.CurrentClientGame.GamePage.StopWaitAndResumeSavedGame();
                }
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
        /* Game can also have mute mode */

        public static void UpdateSoundMuteness(bool newGameMuted, bool newSilentMode, bool newSleepMuteMode)
        {
            UpdateSoundMutenessCore(newGameMuted, newSilentMode, newSleepMuteMode, GameMuteMode, SleepMuteMode, SilentMode);
        }

        public static void UpdateSoundMutenessCore(bool newGameMuted, bool newSilentMode, bool newSleepMuteMode, bool oldGameMuted, bool oldSilentMode, bool oldSleepMuteMode)
        {
            if (newGameMuted || newSilentMode || newSleepMuteMode)
            {
                if(!oldGameMuted && !oldSilentMode && !oldSleepMuteMode)
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
            _mainGnollHackService = DependencyService.Get<IGnollHackService>();
            _mainGnollHackService.LoadLibrary();
            _mainGnollHackService.Test();
            _fmodService = DependencyService.Get<IFmodService>();
            _platformService = DependencyService.Get<IPlatformService>();
        }

        public static void ResetAcquiredFiles()
        {
            string ghdir = _mainGnollHackService.GetGnollHackPath();
            bool resetfiles = Preferences.Get("ResetExternalFiles", true);
            if (resetfiles)
            {
                foreach (SecretsFile sf in App.CurrentSecrets.files)
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
        public static bool FullVersionMode { get; set; }
        public static bool ClassicMode { get; set; }
        public static bool CasualMode { get; set; }
        public static bool ServerGameAvailable { get; set; }

        public static string GHVersionId { get; set; }
        public static string GHVersionString { get; set; }
        public static string SkiaVersionString { get; set; }
        public static string SkiaSharpVersionString { get; set; }
        public static string FMODVersionString { get; set; }
        public static string GHPath { get; set; }
        //public static readonly string LogFile = "console.log";
        //public static TextWriter OldOut;
        //public static TextWriter OldError;
        //public static StreamWriter LogOut;
        //public static FileStream LogStream;
        public static Cookie AuthenticationCookie { get; set; }
        public static Server SelectedServer { get; set; }
        public static string UserName { get; set; }
        public static bool IsServerGame { get; set; }
        public static bool LoadBanks { get; set; }


        public static NavigationPage MainNavigationPage { get { return (NavigationPage)Current.MainPage; } }

        public static event BackButtonHandler BackButtonPressed;

        private static IGnollHackService _mainGnollHackService = null;
        public static IGnollHackService GnollHackService { get { return _mainGnollHackService; } }
        private static IFmodService _fmodService = null;
        public static IFmodService FmodService { get { return _fmodService; } }
        private static IPlatformService _platformService = null;
        public static IPlatformService PlatformService { get { return _platformService; } }

        public static readonly float DisplayRefreshRate = Math.Max(30.0f, DeviceDisplay.MainDisplayInfo.RefreshRate);
        public static readonly bool IsAndroid = (Device.RuntimePlatform == Device.Android);
        public static readonly bool IsiOS = (Device.RuntimePlatform == Device.iOS);
        public static readonly bool IsUWP = (Device.RuntimePlatform == Device.UWP);

        public static readonly float DisplayScale = DeviceDisplay.MainDisplayInfo.Density <= 0 ? 1.0f : (float)DeviceDisplay.MainDisplayInfo.Density;
        public static readonly float DisplayWidth = (float)DeviceDisplay.MainDisplayInfo.Width * DisplayScale;
        public static readonly float DisplayHeight = (float)DeviceDisplay.MainDisplayInfo.Height * DisplayScale;

        public static bool UsesCarousel { get; set; }

        public static async Task<bool> OnBackButtonPressed()
        {
            var handler = BackButtonPressed;
            if(handler != null)
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
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.diablo_h.ttf"))
            {
                if (stream != null)
                {
                    try
                    {
                        App.DiabloTypeface = SKTypeface.FromStream(stream);
                        App.TypefaceDictionary.Add("Diablo", App.DiabloTypeface);
                    }
                    catch (Exception ex)
                    {
                        Debug.WriteLine(ex.Message);
                    }
                }
            }
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.uwch.ttf"))
            {
                if (stream != null)
                {
                    try
                    {
                        App.UnderwoodTypeface = SKTypeface.FromStream(stream);
                        App.TypefaceDictionary.Add("Underwood", App.UnderwoodTypeface);
                    }
                    catch (Exception ex)
                    {
                        Debug.WriteLine(ex.Message);
                    }
                }
            }
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.Immortal-Regular.ttf"))
            {
                if (stream != null)
                {
                    try
                    {
                        App.ImmortalTypeface = SKTypeface.FromStream(stream);
                        App.TypefaceDictionary.Add("Immortal", App.ImmortalTypeface);
                    }
                    catch (Exception ex)
                    {
                        Debug.WriteLine(ex.Message);
                    }
                }
            }
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.archristy.ttf"))
            {
                if (stream != null)
                {
                    try
                    {
                        App.ARChristyTypeface = SKTypeface.FromStream(stream);
                        App.TypefaceDictionary.Add("ARChristy", App.ARChristyTypeface);
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
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.endr.ttf"))
            {
                if (stream != null)
                {
                    try
                    {
                        App.EndorTypeface = SKTypeface.FromStream(stream);
                        App.TypefaceDictionary.Add("Endor", App.EndorTypeface);
                    }
                    catch (Exception ex)
                    {
                        Debug.WriteLine(ex.Message);
                    }
                }
            }
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.shxi.ttf"))
            {
                if (stream != null)
                {
                    try
                    {
                        App.XizorTypeface = SKTypeface.FromStream(stream);
                        App.TypefaceDictionary.Add("Xizor", App.XizorTypeface);
                    }
                    catch (Exception ex)
                    {
                        Debug.WriteLine(ex.Message);
                    }
                }
            }
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.DejaVuSansMono.ttf"))
            {
                if (stream != null)
                {
                    try
                    {
                        App.DejaVuSansMonoTypeface = SKTypeface.FromStream(stream);
                        App.TypefaceDictionary.Add("DejaVuSansMono", App.DejaVuSansMonoTypeface);
                    }
                    catch (Exception ex)
                    {
                        Debug.WriteLine(ex.Message);
                    }
                }
            }
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.DejaVuSansMono-Bold.ttf"))
            {
                if (stream != null)
                {
                    try
                    {
                        App.DejaVuSansMonoBoldTypeface = SKTypeface.FromStream(stream);
                        App.TypefaceDictionary.Add("DejaVuSansMono-Bold", App.DejaVuSansMonoBoldTypeface);
                    }
                    catch (Exception ex)
                    {
                        Debug.WriteLine(ex.Message);
                    }
                }
            }
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.Lato-Regular.ttf"))
            {
                if (stream != null)
                {
                    try
                    {
                        App.LatoRegular = SKTypeface.FromStream(stream);
                        App.TypefaceDictionary.Add("Lato-Regular", App.LatoRegular);
                    }
                    catch (Exception ex)
                    {
                        Debug.WriteLine(ex.Message);
                    }
                }
            }
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.Lato-Bold.ttf"))
            {
                if (stream != null)
                {
                    try
                    {
                        App.LatoBold = SKTypeface.FromStream(stream);
                        App.TypefaceDictionary.Add("Lato-Bold", App.LatoBold);
                    }
                    catch (Exception ex)
                    {
                        Debug.WriteLine(ex.Message);
                    }
                }
            }
        }

        public static SKBitmap MenuBackgroundBitmap { get; set; }
        public static SKBitmap OldPaperBackgroundBitmap { get; set; }

        public static SKBitmap SimpleFrameTopLeftCornerBitmap { get; set; }
        public static SKBitmap SimpleFrameSmallTopLeftCornerBitmap { get; set; }
        public static SKBitmap SimpleFrameTopHorizontalBitmap { get; set; }
        public static SKBitmap SimpleFrameLeftVerticalBitmap { get; set; }

        public static SKBitmap ScrollBitmap { get; set; }
        public static SKBitmap YouBitmap { get; set; }

        public static void InitGameBitmaps(Assembly assembly)
        {
            try
            {
                using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.menubackground.png"))
                {
                    MenuBackgroundBitmap = SKBitmap.Decode(stream);
                }
                using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.background-oldpaper.png"))
                {
                    OldPaperBackgroundBitmap = SKBitmap.Decode(stream);
                }
                using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.frame-topleft.png"))
                {
                    SimpleFrameTopLeftCornerBitmap = SKBitmap.Decode(stream);
                }
                using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.frame-topleft-small.png"))
                {
                    SimpleFrameSmallTopLeftCornerBitmap = SKBitmap.Decode(stream);
                }
                using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.frame-horizontal.png"))
                {
                    SimpleFrameTopHorizontalBitmap = SKBitmap.Decode(stream);
                }
                using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.frame-vertical.png"))
                {
                    SimpleFrameLeftVerticalBitmap = SKBitmap.Decode(stream);
                }
                using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.scroll.png"))
                {
                    ScrollBitmap = SKBitmap.Decode(stream);
                }
                using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.you.png"))
                {
                    YouBitmap = SKBitmap.Decode(stream);
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
                using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.symbol-success.png"))
                {
                    _successBitmap = SKBitmap.Decode(stream);
                }
                using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.symbol-mana.png"))
                {
                    _manaBitmap = SKBitmap.Decode(stream);
                }
                using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.symbol-cooldown.png"))
                {
                    _cooldownBitmap = SKBitmap.Decode(stream);
                }
                using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.symbol-casts.png"))
                {
                    _castsBitmap = SKBitmap.Decode(stream);
                }
                using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.symbol-adds.png"))
                {
                    _addsBitmap = SKBitmap.Decode(stream);
                }
                using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.symbol-food.png"))
                {
                    _foodBitmap = SKBitmap.Decode(stream);
                }

                using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.symbol-spell-abjuration.png"))
                {
                    _spellAbjurationBitmap = SKBitmap.Decode(stream);
                }
                using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.symbol-spell-arcane.png"))
                {
                    _spellArcaneBitmap = SKBitmap.Decode(stream);
                }
                using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.symbol-spell-celestial.png"))
                {
                    _spellCelestialBitmap = SKBitmap.Decode(stream);
                }
                using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.symbol-spell-clerical.png"))
                {
                    _spellClericalBitmap = SKBitmap.Decode(stream);
                }
                using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.symbol-spell-conjuration.png"))
                {
                    _spellConjurationBitmap = SKBitmap.Decode(stream);
                }
                using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.symbol-spell-divination.png"))
                {
                    _spellDivinationBitmap = SKBitmap.Decode(stream);
                }
                using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.symbol-spell-enchantment.png"))
                {
                    _spellEnchantmentBitmap = SKBitmap.Decode(stream);
                }
                using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.symbol-spell-healing.png"))
                {
                    _spellHealingBitmap = SKBitmap.Decode(stream);
                }
                using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.symbol-spell-movement.png"))
                {
                    _spellMovementBitmap = SKBitmap.Decode(stream);
                }
                using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.symbol-spell-nature.png"))
                {
                    _spellNatureBitmap = SKBitmap.Decode(stream);
                }
                using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.symbol-spell-necromancy.png"))
                {
                    _spellNecromancyBitmap = SKBitmap.Decode(stream);
                }
                using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.symbol-spell-transmutation.png"))
                {
                    _spellTransmutationBitmap = SKBitmap.Decode(stream);
                }

            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
        }



        public static SKBitmap _logoBitmap;
        public static SKBitmap _skillBitmap;
        public static SKBitmap[] _arrowBitmap = new SKBitmap[9];
        public static SKBitmap _orbBorderBitmap;
        public static SKBitmap _orbFillBitmap;
        public static SKBitmap _orbFillBitmapRed;
        public static SKBitmap _orbFillBitmapBlue;
        public static SKBitmap _orbGlassBitmap;

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

        public static SKBitmap _statusGoldBitmap;
        public static SKBitmap _statusTurnsBitmap;

        public static SKBitmap _statusDungeonLevelBitmap;

        public static SKBitmap _searchBitmap;
        public static SKBitmap _waitBitmap;


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

        public static GHCommandButtonItem[,,] _moreBtnMatrix = new GHCommandButtonItem[GHConstants.MoreButtonPages, GHConstants.MoreButtonsPerRow, GHConstants.MoreButtonsPerColumn];
        public static SKBitmap[,,] _moreBtnBitmaps = new SKBitmap[GHConstants.MoreButtonPages, GHConstants.MoreButtonsPerRow, GHConstants.MoreButtonsPerColumn];
        public static string[] _moreButtonPageTitle = new string[GHConstants.MoreButtonPages] { "Wizard Mode Commands", "Common Commands", "Additional Commands", "Context and More Commands" };

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

        public static void InitializeMoreCommandButtons(Assembly assembly, bool useSimple)
        {
            for (int i = 0; i < GHConstants.MoreButtonPages; i++)
                for (int j = 0; j < GHConstants.MoreButtonsPerRow; j++)
                    for (int k = 0; k < GHConstants.MoreButtonsPerColumn; k++)
                        _moreBtnMatrix[i, j, k] = null;
            if(useSimple)
            {
                _moreBtnMatrix[0, 0, 0] = new GHCommandButtonItem("Wish", "GnollHackClient.Assets.UI.wish.png", GHUtils.Ctrl((int)'w'));
                _moreBtnMatrix[0, 1, 0] = new GHCommandButtonItem("Reveal", "GnollHackClient.Assets.UI.reveal.png", GHUtils.Ctrl((int)'f'));
                _moreBtnMatrix[0, 2, 0] = new GHCommandButtonItem("Genesis", "GnollHackClient.Assets.UI.genesis.png", GHUtils.Meta((int)'m'));
                _moreBtnMatrix[0, 3, 0] = new GHCommandButtonItem("Levelport", "GnollHackClient.Assets.UI.levelport.png", GHUtils.Ctrl((int)'v'));

                _moreBtnMatrix[0, 0, 1] = new GHCommandButtonItem("Identify", "GnollHackClient.Assets.UI.identify.png", GHUtils.Ctrl((int)'i'));
                _moreBtnMatrix[0, 1, 1] = new GHCommandButtonItem("Teleport", "GnollHackClient.Assets.UI.teleport.png", GHUtils.Ctrl((int)'t'));
                _moreBtnMatrix[0, 2, 1] = new GHCommandButtonItem("Level Change", "GnollHackClient.Assets.UI.levelchange.png", GHUtils.Meta(0));
                _moreBtnMatrix[0, 3, 1] = new GHCommandButtonItem("Polymorph Self", "GnollHackClient.Assets.UI.polymorph.png", GHUtils.Meta(1));

                _moreBtnMatrix[0, 2, 5] = new GHCommandButtonItem("Extended", "GnollHackClient.Assets.UI.extended.png", (int)'#');
                _moreBtnMatrix[0, 3, 5] = new GHCommandButtonItem("Back to Game", "GnollHackClient.Assets.UI.more.png", -1);

                _moreBtnMatrix[1, 0, 0] = new GHCommandButtonItem("You", "GnollHackClient.Assets.UI.you.png", (int)'}');
                _moreBtnMatrix[1, 1, 0] = new GHCommandButtonItem("Skills", "GnollHackClient.Assets.UI.skill.png", (int)'S');
                _moreBtnMatrix[1, 2, 0] = new GHCommandButtonItem("Overview", "GnollHackClient.Assets.UI.overview.png", GHUtils.Ctrl((int)'o'));
                _moreBtnMatrix[1, 3, 0] = new GHCommandButtonItem("Goals", "GnollHackClient.Assets.UI.conduct.png", GHUtils.Meta(3));

                _moreBtnMatrix[1, 0, 1] = new GHCommandButtonItem("Apply", "GnollHackClient.Assets.UI.apply.png", (int)'a');
                _moreBtnMatrix[1, 1, 1] = new GHCommandButtonItem("Wait", "GnollHackClient.Assets.UI.wait.png", (int)'.');
                _moreBtnMatrix[1, 2, 1] = new GHCommandButtonItem("Drop Items", "GnollHackClient.Assets.UI.dropmany.png", (int)'%');
                _moreBtnMatrix[1, 3, 1] = new GHCommandButtonItem("Wield", "GnollHackClient.Assets.UI.wield.png", (int)'w');

                _moreBtnMatrix[1, 0, 2] = new GHCommandButtonItem("Throw", "GnollHackClient.Assets.UI.throw.png", (int)'t');
                _moreBtnMatrix[1, 1, 2] = new GHCommandButtonItem("Fire", "GnollHackClient.Assets.UI.fire.png", (int)'f');
                _moreBtnMatrix[1, 2, 2] = new GHCommandButtonItem("Chat", "GnollHackClient.Assets.UI.chat.png", (int)'C');
                _moreBtnMatrix[1, 3, 2] = new GHCommandButtonItem("Zap", "GnollHackClient.Assets.UI.zap.png", (int)'z');

                _moreBtnMatrix[1, 0, 3] = new GHCommandButtonItem("Eat", "GnollHackClient.Assets.UI.eat.png", (int)'e');
                _moreBtnMatrix[1, 1, 3] = new GHCommandButtonItem("Drink", "GnollHackClient.Assets.UI.quaff.png", (int)'q');
                _moreBtnMatrix[1, 2, 3] = new GHCommandButtonItem("Read", "GnollHackClient.Assets.UI.read.png", (int)'r');
                _moreBtnMatrix[1, 3, 3] = new GHCommandButtonItem("Drop Types", "GnollHackClient.Assets.UI.dropmany.png", (int)'D');

                _moreBtnMatrix[1, 0, 4] = new GHCommandButtonItem("View Spell", "GnollHackClient.Assets.UI.viewspell.png", GHUtils.Meta((int)'z'));
                _moreBtnMatrix[1, 1, 4] = new GHCommandButtonItem("Mix", "GnollHackClient.Assets.UI.mix.png", (int)'X');
                _moreBtnMatrix[1, 2, 4] = new GHCommandButtonItem("Yell", "GnollHackClient.Assets.UI.yell.png", GHUtils.Ctrl((int)'y'));
                _moreBtnMatrix[1, 3, 4] = new GHCommandButtonItem("Pray", "GnollHackClient.Assets.UI.pray.png", GHUtils.Meta((int)'p'));

                _moreBtnMatrix[1, 0, 5] = new GHCommandButtonItem("Count", "GnollHackClient.Assets.UI.count.png", -5);
                _moreBtnMatrix[1, 1, 5] = new GHCommandButtonItem("Search 20", "GnollHackClient.Assets.UI.search20.png", -2);
                _moreBtnMatrix[1, 2, 5] = new GHCommandButtonItem("Search 200", "GnollHackClient.Assets.UI.search200.png", -3);
                _moreBtnMatrix[1, 3, 5] = new GHCommandButtonItem("Back to Game", "GnollHackClient.Assets.UI.more.png", -1);

                _moreBtnMatrix[2, 0, 0] = new GHCommandButtonItem("Attributes", "GnollHackClient.Assets.UI.attributes.png", GHUtils.Meta(2));
                _moreBtnMatrix[2, 1, 0] = new GHCommandButtonItem("Discoveries", "GnollHackClient.Assets.UI.discoveries.png", (int)'\\');
                _moreBtnMatrix[2, 2, 0] = new GHCommandButtonItem("Killed", "GnollHackClient.Assets.UI.killed.png", GHUtils.Meta((int)'k'));
                _moreBtnMatrix[2, 3, 0] = new GHCommandButtonItem("Genocided", "GnollHackClient.Assets.UI.genocided.png", GHUtils.Meta((int)'g'));

                _moreBtnMatrix[2, 0, 1] = new GHCommandButtonItem("Travel", "GnollHackClient.Assets.UI.travel.png", (int)'_');
                _moreBtnMatrix[2, 1, 1] = new GHCommandButtonItem("2-Weapon", "GnollHackClient.Assets.UI.twoweap.png", GHUtils.Ctrl((int)'x'));
                _moreBtnMatrix[2, 2, 1] = new GHCommandButtonItem("Examine", "GnollHackClient.Assets.UI.examine.png", GHUtils.Meta((int)'x'));
                _moreBtnMatrix[2, 3, 1] = new GHCommandButtonItem("Engrave", "GnollHackClient.Assets.UI.engrave.png", (int)'E');
                
                _moreBtnMatrix[2, 0, 2] = new GHCommandButtonItem("Ride", "GnollHackClient.Assets.UI.ride.png", GHUtils.Meta((int)'R'));
                _moreBtnMatrix[2, 1, 2] = new GHCommandButtonItem("Untrap", "GnollHackClient.Assets.UI.untrap.png", GHUtils.Meta((int)'u'));
                _moreBtnMatrix[2, 2, 2] = new GHCommandButtonItem("Handedness", "GnollHackClient.Assets.UI.handedness.png", GHUtils.Meta((int)'h'));
                _moreBtnMatrix[2, 3, 2] = new GHCommandButtonItem("Unwield", "GnollHackClient.Assets.UI.unwield.png", GHUtils.Meta(5));

                _moreBtnMatrix[2, 0, 3] = new GHCommandButtonItem("Light", "GnollHackClient.Assets.UI.light.png", GHUtils.Ctrl((int)'l'));
                _moreBtnMatrix[2, 1, 3] = new GHCommandButtonItem("Loot", "GnollHackClient.Assets.UI.loot.png", (int)'l');
                _moreBtnMatrix[2, 2, 3] = new GHCommandButtonItem("Open", "GnollHackClient.Assets.UI.open.png", (int)'o');
                _moreBtnMatrix[2, 3, 3] = new GHCommandButtonItem("Close", "GnollHackClient.Assets.UI.close.png", (int)'c');

                _moreBtnMatrix[2, 0, 4] = new GHCommandButtonItem("Teleport", "GnollHackClient.Assets.UI.teleport.png", GHUtils.Ctrl((int)'t'));
                _moreBtnMatrix[2, 1, 4] = new GHCommandButtonItem("Monster", "GnollHackClient.Assets.UI.monster.png", GHUtils.Meta(4));
                _moreBtnMatrix[2, 2, 4] = new GHCommandButtonItem("Jump", "GnollHackClient.Assets.UI.jump.png", (int)'j');
                _moreBtnMatrix[2, 3, 4] = new GHCommandButtonItem("Dig", "GnollHackClient.Assets.UI.dig.png", GHUtils.Ctrl((int)'g'));

                _moreBtnMatrix[2, 0, 5] = new GHCommandButtonItem("Name", "GnollHackClient.Assets.UI.name.png", (int)'N');
                _moreBtnMatrix[2, 1, 5] = new GHCommandButtonItem("Look Here", "GnollHackClient.Assets.UI.lookhere.png", (int)':');
                _moreBtnMatrix[2, 2, 5] = new GHCommandButtonItem("Extended", "GnollHackClient.Assets.UI.extended.png", (int)'#');
                _moreBtnMatrix[2, 3, 5] = new GHCommandButtonItem("Back to Game", "GnollHackClient.Assets.UI.more.png", -1);
            }
            else
            {
                _moreBtnMatrix[0, 0, 0] = new GHCommandButtonItem("Wish", "GnollHackClient.Assets.UI.wish.png", GHUtils.Ctrl((int)'w'));
                _moreBtnMatrix[0, 1, 0] = new GHCommandButtonItem("Reveal", "GnollHackClient.Assets.UI.reveal.png", GHUtils.Ctrl((int)'f'));
                _moreBtnMatrix[0, 2, 0] = new GHCommandButtonItem("Genesis", "GnollHackClient.Assets.UI.genesis.png", GHUtils.Meta((int)'m'));
                _moreBtnMatrix[0, 3, 0] = new GHCommandButtonItem("Levelport", "GnollHackClient.Assets.UI.levelport.png", GHUtils.Ctrl((int)'v'));

                _moreBtnMatrix[0, 0, 1] = new GHCommandButtonItem("Identify", "GnollHackClient.Assets.UI.identify.png", GHUtils.Ctrl((int)'i'));
                _moreBtnMatrix[0, 1, 1] = new GHCommandButtonItem("Teleport", "GnollHackClient.Assets.UI.teleport.png", GHUtils.Ctrl((int)'t'));
                _moreBtnMatrix[0, 2, 1] = new GHCommandButtonItem("Level Change", "GnollHackClient.Assets.UI.levelchange.png", GHUtils.Meta(0));
                _moreBtnMatrix[0, 3, 1] = new GHCommandButtonItem("Polymorph Self", "GnollHackClient.Assets.UI.polymorph.png", GHUtils.Meta(1));

                _moreBtnMatrix[0, 2, 5] = new GHCommandButtonItem("Extended", "GnollHackClient.Assets.UI.extended.png", (int)'#');
                _moreBtnMatrix[0, 3, 5] = new GHCommandButtonItem("Back to Game", "GnollHackClient.Assets.UI.more.png", -1);

                _moreBtnMatrix[1, 0, 0] = new GHCommandButtonItem("You", "GnollHackClient.Assets.UI.you.png", (int)'}');
                _moreBtnMatrix[1, 1, 0] = new GHCommandButtonItem("Skills", "GnollHackClient.Assets.UI.skill.png", (int)'S');
                _moreBtnMatrix[1, 2, 0] = new GHCommandButtonItem("Overview", "GnollHackClient.Assets.UI.overview.png", GHUtils.Ctrl((int)'o'));
                _moreBtnMatrix[1, 3, 0] = new GHCommandButtonItem("Goals", "GnollHackClient.Assets.UI.conduct.png", GHUtils.Meta(3));

                _moreBtnMatrix[1, 0, 1] = new GHCommandButtonItem("View Spell", "GnollHackClient.Assets.UI.viewspell.png", GHUtils.Meta((int)'z'));
                _moreBtnMatrix[1, 1, 1] = new GHCommandButtonItem("Mix", "GnollHackClient.Assets.UI.mix.png", (int)'X');
                _moreBtnMatrix[1, 2, 1] = new GHCommandButtonItem("Travel", "GnollHackClient.Assets.UI.travel.png", (int)'_');
                _moreBtnMatrix[1, 3, 1] = new GHCommandButtonItem("2-Weapon", "GnollHackClient.Assets.UI.twoweap.png", GHUtils.Ctrl((int)'x'));

                _moreBtnMatrix[1, 0, 2] = new GHCommandButtonItem("Examine", "GnollHackClient.Assets.UI.examine.png", GHUtils.Meta((int)'x'));
                _moreBtnMatrix[1, 1, 2] = new GHCommandButtonItem("Engrave", "GnollHackClient.Assets.UI.engrave.png", (int)'E');
                _moreBtnMatrix[1, 2, 2] = new GHCommandButtonItem("Ride", "GnollHackClient.Assets.UI.ride.png", GHUtils.Meta((int)'R'));
                _moreBtnMatrix[1, 3, 2] = new GHCommandButtonItem("Wield", "GnollHackClient.Assets.UI.wield.png", (int)'w');

                _moreBtnMatrix[1, 0, 3] = new GHCommandButtonItem("Eat", "GnollHackClient.Assets.UI.eat.png", (int)'e');
                _moreBtnMatrix[1, 1, 3] = new GHCommandButtonItem("Drink", "GnollHackClient.Assets.UI.quaff.png", (int)'q');
                _moreBtnMatrix[1, 2, 3] = new GHCommandButtonItem("Read", "GnollHackClient.Assets.UI.read.png", (int)'r');
                _moreBtnMatrix[1, 3, 3] = new GHCommandButtonItem("Drop Types", "GnollHackClient.Assets.UI.dropmany.png", (int)'D');

                _moreBtnMatrix[1, 0, 4] = new GHCommandButtonItem("Untrap", "GnollHackClient.Assets.UI.untrap.png", GHUtils.Meta((int)'u'));
                _moreBtnMatrix[1, 1, 4] = new GHCommandButtonItem("Handedness", "GnollHackClient.Assets.UI.handedness.png", GHUtils.Meta((int)'h'));
                _moreBtnMatrix[1, 2, 4] = new GHCommandButtonItem("Yell", "GnollHackClient.Assets.UI.yell.png", GHUtils.Ctrl((int)'y'));
                _moreBtnMatrix[1, 3, 4] = new GHCommandButtonItem("Pray", "GnollHackClient.Assets.UI.pray.png", GHUtils.Meta((int)'p'));

                _moreBtnMatrix[1, 0, 5] = new GHCommandButtonItem("Count", "GnollHackClient.Assets.UI.count.png", -5);
                _moreBtnMatrix[1, 1, 5] = new GHCommandButtonItem("Search 20", "GnollHackClient.Assets.UI.search20.png", -2);
                _moreBtnMatrix[1, 2, 5] = new GHCommandButtonItem("Search 200", "GnollHackClient.Assets.UI.search200.png", -3);
                _moreBtnMatrix[1, 3, 5] = new GHCommandButtonItem("Back to Game", "GnollHackClient.Assets.UI.more.png", -1);

                _moreBtnMatrix[2, 0, 0] = new GHCommandButtonItem("Attributes", "GnollHackClient.Assets.UI.attributes.png", GHUtils.Meta(2));
                _moreBtnMatrix[2, 1, 0] = new GHCommandButtonItem("Discoveries", "GnollHackClient.Assets.UI.discoveries.png", (int)'\\');
                _moreBtnMatrix[2, 2, 0] = new GHCommandButtonItem("Killed", "GnollHackClient.Assets.UI.killed.png", GHUtils.Meta((int)'k'));
                _moreBtnMatrix[2, 3, 0] = new GHCommandButtonItem("Genocided", "GnollHackClient.Assets.UI.genocided.png", GHUtils.Meta((int)'g'));

                _moreBtnMatrix[2, 0, 1] = new GHCommandButtonItem("Wear", "GnollHackClient.Assets.UI.wear.png", (int)'W');
                _moreBtnMatrix[2, 1, 1] = new GHCommandButtonItem("Put On", "GnollHackClient.Assets.UI.puton.png", (int)'P');
                _moreBtnMatrix[2, 2, 1] = new GHCommandButtonItem("Quiver", "GnollHackClient.Assets.UI.quiver.png", (int)'Q');
                _moreBtnMatrix[2, 3, 1] = new GHCommandButtonItem("Fight", "GnollHackClient.Assets.UI.fight.png", (int)'F');

                _moreBtnMatrix[2, 0, 2] = new GHCommandButtonItem("Take Off", "GnollHackClient.Assets.UI.takeoff.png", (int)'T');
                _moreBtnMatrix[2, 1, 2] = new GHCommandButtonItem("Remove", "GnollHackClient.Assets.UI.remove.png", (int)'R');
                _moreBtnMatrix[2, 2, 2] = new GHCommandButtonItem("Take Off Many", "GnollHackClient.Assets.UI.takeoffmany.png", GHUtils.Meta((int)'t'));
                _moreBtnMatrix[2, 3, 2] = new GHCommandButtonItem("Unwield", "GnollHackClient.Assets.UI.unwield.png", GHUtils.Meta(5));

                _moreBtnMatrix[2, 0, 3] = new GHCommandButtonItem("Dig", "GnollHackClient.Assets.UI.dig.png", GHUtils.Ctrl((int)'g'));
                _moreBtnMatrix[2, 1, 3] = new GHCommandButtonItem("Light", "GnollHackClient.Assets.UI.light.png", GHUtils.Ctrl((int)'l'));
                _moreBtnMatrix[2, 2, 3] = new GHCommandButtonItem("Jump", "GnollHackClient.Assets.UI.jump.png", (int)'j');
                _moreBtnMatrix[2, 3, 3] = new GHCommandButtonItem("Spells", "GnollHackClient.Assets.UI.spells.png", (int)'+');

                _moreBtnMatrix[2, 0, 4] = new GHCommandButtonItem("Tip", "GnollHackClient.Assets.UI.tip.png", GHUtils.Meta((int)'T'));
                _moreBtnMatrix[2, 1, 4] = new GHCommandButtonItem("Invoke", "GnollHackClient.Assets.UI.invoke.png", GHUtils.Meta((int)'i'));
                _moreBtnMatrix[2, 2, 4] = new GHCommandButtonItem("Rub", "GnollHackClient.Assets.UI.rub.png", GHUtils.Meta((int)'r'));
                _moreBtnMatrix[2, 3, 4] = new GHCommandButtonItem("Wipe", "GnollHackClient.Assets.UI.wipe.png", GHUtils.Meta((int)'w'));

                _moreBtnMatrix[2, 0, 5] = new GHCommandButtonItem("Name", "GnollHackClient.Assets.UI.name.png", (int)'N');
                _moreBtnMatrix[2, 1, 5] = new GHCommandButtonItem("What Is", "GnollHackClient.Assets.UI.whatis.png", (int)'/');
                _moreBtnMatrix[2, 2, 5] = new GHCommandButtonItem("Look Far", "GnollHackClient.Assets.UI.lookfar.png", (int)';');
                _moreBtnMatrix[2, 3, 5] = new GHCommandButtonItem("Back to Game", "GnollHackClient.Assets.UI.more.png", -1);

                _moreBtnMatrix[3, 0, 0] = new GHCommandButtonItem("Look Here", "GnollHackClient.Assets.UI.lookhere.png", (int)':');
                _moreBtnMatrix[3, 1, 0] = new GHCommandButtonItem("Pick Up", "GnollHackClient.Assets.UI.pickup.png", (int)',');
                _moreBtnMatrix[3, 2, 0] = new GHCommandButtonItem("Sit", "GnollHackClient.Assets.UI.sit.png", GHUtils.Ctrl((int)'s'));
                _moreBtnMatrix[3, 3, 0] = new GHCommandButtonItem("Pay", "GnollHackClient.Assets.UI.pay.png", (int)'p');

                _moreBtnMatrix[3, 0, 1] = new GHCommandButtonItem("Loot", "GnollHackClient.Assets.UI.loot.png", (int)'l');
                _moreBtnMatrix[3, 1, 1] = new GHCommandButtonItem("Dip", "GnollHackClient.Assets.UI.dip.png", GHUtils.Meta((int)'d'));
                _moreBtnMatrix[3, 2, 1] = new GHCommandButtonItem("Offer", "GnollHackClient.Assets.UI.offer.png", GHUtils.Meta((int)'o'));
                _moreBtnMatrix[3, 3, 1] = new GHCommandButtonItem("Autopickup", "GnollHackClient.Assets.UI.autopickup.png", (int)'@');

                _moreBtnMatrix[3, 0, 2] = new GHCommandButtonItem("Go Down", "GnollHackClient.Assets.UI.stairs-down.png", (int)'>');
                _moreBtnMatrix[3, 1, 2] = new GHCommandButtonItem("Go Up", "GnollHackClient.Assets.UI.stairs-up.png", (int)'<');
                _moreBtnMatrix[3, 2, 2] = new GHCommandButtonItem("Open", "GnollHackClient.Assets.UI.open.png", (int)'o');
                _moreBtnMatrix[3, 3, 2] = new GHCommandButtonItem("Close", "GnollHackClient.Assets.UI.close.png", (int)'c');

                _moreBtnMatrix[3, 0, 3] = new GHCommandButtonItem("Break", "GnollHackClient.Assets.UI.break.png", GHUtils.Ctrl((int)'b'));
                _moreBtnMatrix[3, 1, 3] = new GHCommandButtonItem("Force Lock", "GnollHackClient.Assets.UI.forcelock.png", GHUtils.Meta((int)'f'));
                _moreBtnMatrix[3, 2, 3] = new GHCommandButtonItem("Teleport", "GnollHackClient.Assets.UI.teleport.png", GHUtils.Ctrl((int)'t'));
                _moreBtnMatrix[3, 3, 3] = new GHCommandButtonItem("Monster", "GnollHackClient.Assets.UI.monster.png", GHUtils.Meta(4));

                _moreBtnMatrix[3, 0, 5] = new GHCommandButtonItem("Help", "GnollHackClient.Assets.UI.help.png", (int)'?'); // "Menu", "GnollHackClient.Assets.Icons.missing_icon.png", -4);
                _moreBtnMatrix[3, 1, 5] = new GHCommandButtonItem("Commands", "GnollHackClient.Assets.UI.commands.png", GHUtils.Meta((int)'c'));
                _moreBtnMatrix[3, 2, 5] = new GHCommandButtonItem("Extended", "GnollHackClient.Assets.UI.extended.png", (int)'#');
                _moreBtnMatrix[3, 3, 5] = new GHCommandButtonItem("Back to Game", "GnollHackClient.Assets.UI.more.png", -1);
            }

            for (int k = 0; k < GHConstants.MoreButtonPages; k++)
            {
                for (int i = 0; i < GHConstants.MoreButtonsPerRow; i++)
                {
                    for (int j = 0; j < GHConstants.MoreButtonsPerColumn; j++)
                    {
                        if (_moreBtnMatrix[k, i, j] != null && !string.IsNullOrEmpty(_moreBtnMatrix[k, i, j].ImageSourcePath))
                        {
                            using (Stream stream = assembly.GetManifestResourceStream(_moreBtnMatrix[k, i, j].ImageSourcePath))
                            {
                                if (stream != null)
                                    _moreBtnBitmaps[k, i, j] = SKBitmap.Decode(stream);
                            }
                        }
                    }
                }
            }
        }

        public static void InitializeArrowButtons(Assembly assembly)
        {
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.arrow_left.png"))
            {
                _arrowBitmap[0] = SKBitmap.Decode(stream);
            }
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.arrow_up.png"))
            {
                _arrowBitmap[1] = SKBitmap.Decode(stream);
            }
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.arrow_right.png"))
            {
                _arrowBitmap[2] = SKBitmap.Decode(stream);
            }
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.arrow_down.png"))
            {
                _arrowBitmap[3] = SKBitmap.Decode(stream);
            }
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.arrow_upleft.png"))
            {
                _arrowBitmap[4] = SKBitmap.Decode(stream);
            }
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.no.png"))
            {
                _arrowBitmap[5] = SKBitmap.Decode(stream);
            }
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.arrow_upright.png"))
            {
                _arrowBitmap[6] = SKBitmap.Decode(stream);
            }
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.arrow_downright.png"))
            {
                _arrowBitmap[7] = SKBitmap.Decode(stream);
            }
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.arrow_downleft.png"))
            {
                _arrowBitmap[8] = SKBitmap.Decode(stream);
            }
        }

        public static void InitializeUIBitmaps(Assembly assembly)
        {
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.orb_border.png"))
            {
                _orbBorderBitmap = SKBitmap.Decode(stream);
            }
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.orb_fill.png"))
            {
                _orbFillBitmap = SKBitmap.Decode(stream);
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
            }

            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.orb_glass.png"))
            {
                _orbGlassBitmap = SKBitmap.Decode(stream);
            }

            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.status-wizard-mode.png"))
            {
                _statusWizardBitmap = SKBitmap.Decode(stream);
            }
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.status-casual-mode.png"))
            {
                _statusCasualBitmap = SKBitmap.Decode(stream);
            }
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.status-casual-classic-mode.png"))
            {
                _statusCasualClassicBitmap = SKBitmap.Decode(stream);
            }
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.status-modern-mode.png"))
            {
                _statusModernBitmap = SKBitmap.Decode(stream);
            }
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.status-difficulty.png"))
            {
                _statusDifficultyBitmap = SKBitmap.Decode(stream);
            }
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.status-difficulty-very-easy.png"))
            {
                _statusDifficultyVeryEasyBitmap = SKBitmap.Decode(stream);
            }
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.status-difficulty-easy.png"))
            {
                _statusDifficultyEasyBitmap = SKBitmap.Decode(stream);
            }
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.status-difficulty-average.png"))
            {
                _statusDifficultyAverageBitmap = SKBitmap.Decode(stream);
            }
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.status-difficulty-hard.png"))
            {
                _statusDifficultyHardBitmap = SKBitmap.Decode(stream);
            }
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.status-difficulty-expert.png"))
            {
                _statusDifficultyExpertBitmap = SKBitmap.Decode(stream);
            }
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.status-difficulty-master.png"))
            {
                _statusDifficultyMasterBitmap = SKBitmap.Decode(stream);
            }
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.status-difficulty-grand-master.png"))
            {
                _statusDifficultyGrandMasterBitmap = SKBitmap.Decode(stream);
            }

            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.status-xp-level.png"))
            {
                _statusXPLevelBitmap = SKBitmap.Decode(stream);
            }
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.status-hd.png"))
            {
                _statusHDBitmap = SKBitmap.Decode(stream);
            }
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.status-ac.png"))
            {
                _statusACBitmap = SKBitmap.Decode(stream);
            }
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.status-mc.png"))
            {
                _statusMCBitmap = SKBitmap.Decode(stream);
            }
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.status-move.png"))
            {
                _statusMoveBitmap = SKBitmap.Decode(stream);
            }
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.status-weapon-style.png"))
            {
                _statusWeaponStyleBitmap = SKBitmap.Decode(stream);
            }

            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.status-gold.png"))
            {
                _statusGoldBitmap = SKBitmap.Decode(stream);
            }
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.status-turns.png"))
            {
                _statusTurnsBitmap = SKBitmap.Decode(stream);
            }
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.status-dungeon-level.png"))
            {
                _statusDungeonLevelBitmap = SKBitmap.Decode(stream);
            }

            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.search.png"))
            {
                _searchBitmap = SKBitmap.Decode(stream);
            }
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.wait.png"))
            {
                _waitBitmap = SKBitmap.Decode(stream);
            }


        }





        public static object ProfilingStopwatchLock = new object();
        private static Stopwatch _profilingStopwatch = new Stopwatch();
        public static Stopwatch ProfilingStopwatch { get { return _profilingStopwatch; } }

        public static void DebugWriteProfilingStopwatchTimeAndStop(string label)
        {
            lock (ProfilingStopwatchLock)
            {
                ProfilingStopwatch.Stop();
                TimeSpan elapsed = ProfilingStopwatch.Elapsed;
                Debug.WriteLine("ProfilingStopwatch: " + label + ": " + elapsed.TotalMilliseconds + " msec");
            }
        }

        public static void DebugWriteProfilingStopwatchTimeAndStart(string label)
        {
            lock (ProfilingStopwatchLock)
            {
                ProfilingStopwatch.Stop();
                TimeSpan elapsed = ProfilingStopwatch.Elapsed;
                Debug.WriteLine("ProfilingStopwatch: " + label + ": " + elapsed.TotalMilliseconds + " msec");
                ProfilingStopwatch.Start();
            }
        }

        public static void DebugWriteRestart(string label)
        {
            lock (ProfilingStopwatchLock)
            {
                Debug.WriteLine("ProfilingStopwatch: " + label + ": " + "Restart");
                ProfilingStopwatch.Restart();
            }
        }

        public static void DebugWriteProfilingStopwatchTimeAndRestart(string label)
        {
            lock (ProfilingStopwatchLock)
            {
                ProfilingStopwatch.Stop();
                TimeSpan elapsed = ProfilingStopwatch.Elapsed;
                Debug.WriteLine("ProfilingStopwatch: " + label + ": " + elapsed.TotalMilliseconds + " msec");
                ProfilingStopwatch.Restart();
            }
        }

        public static void PlayButtonClickedSound()
        {
            if(_fmodService != null)
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
            var stream = assembly.GetManifestResourceStream("GnollHackClient." + filename);

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
            string ghdir = App.GnollHackService.GetGnollHackPath();
            string targetpath = Path.Combine(ghdir, "archive");

            App.CheckCreateDirectory(targetpath);

            string filepath = Path.Combine(targetpath, "crash_report.zip");
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
                string[] ghsubdirlist = { "save", "dumplog" };
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
            string ghdir = App.GnollHackService.GetGnollHackPath();
            string targetpath = Path.Combine(ghdir, "archive");

            App.CheckCreateDirectory(targetpath);

            string filepath = Path.Combine(targetpath, "dumplogs.zip");
            if (File.Exists(filepath))
                File.Delete(filepath);

            string zipFile = filepath;
            string[] files = Directory.GetFiles(ghdir);

            using (ZipArchive archive = ZipFile.Open(zipFile, ZipArchiveMode.Create))
            {
                string[] ghsubdirlist = { "dumplog" };
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
            string ghdir = App.GnollHackService.GetGnollHackPath();
            string targetpath = Path.Combine(ghdir, "archive");

            App.CheckCreateDirectory(targetpath);

            string filepath = Path.Combine(targetpath, "savedgames.zip");
            if (File.Exists(filepath))
                File.Delete(filepath);

            string zipFile = filepath;
            string[] files = Directory.GetFiles(ghdir);

            using (ZipArchive archive = ZipFile.Open(zipFile, ZipArchiveMode.Create))
            {
                string[] ghsubdirlist = { "save" };
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
                        foreach(string subfilename in Directory.GetFiles(targetpath))
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

        //public const string CultureName = "en-US";
        //public static CultureInfo Culture = new CultureInfo(CultureName);

        //public static string GetCurrentTime()
        //{
        //    return DateTime.Now.ToString(Culture);
        //}

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

            if(bitmap != null)
            {
                source_rect.Right = bitmap.Width;
                source_rect.Bottom = bitmap.Height;
            }
            return bitmap;
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
