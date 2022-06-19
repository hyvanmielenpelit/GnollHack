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
            App.InitTypefaces(assembly);
            App.InitBaseBitmaps(assembly);

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
            App.SponsorButtonVisited = Preferences.Get("SponsorButtonVisited", false);
            App.ShowSpecialEffect = Preferences.Get("ShowSpecialEffect", false);
            App.LoadBanks = Preferences.Get("LoadSoundBanks", true);

            App.ReadSecrets();
            Array.Sort<SecretsFile>(App.CurrentSecrets.files, new SecretsFileSizeComparer());
            App.BackButtonPressed += App.EmptyBackButtonPressed;
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

        protected override void OnStart()
        {
            if(PlatformService != null)
                PlatformService.OverrideAnimationDuration();
        }

        protected override void OnSleep()
        {
            if (PlatformService != null)
                PlatformService.RevertAnimationDuration();
        }

        protected override void OnResume()
        {
            if (PlatformService != null)
                PlatformService.OverrideAnimationDuration();
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
        public static bool SponsorButtonVisited { get; set; }
        public static bool ShowSpecialEffect { get; set; }
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

        public static void InitTypefaces(Assembly assembly)
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

        public static void InitBaseBitmaps(Assembly assembly)
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

        public static SKBitmap GetSpecialSymbol(string str)
        {
            string trimmed_str = str.Trim();
            if (trimmed_str == "&success;")
            {
                return _successBitmap;
            }
            else if (trimmed_str == "&mana;")
            {
                return _manaBitmap;
            }
            else if (trimmed_str == "&cool;")
            {
                return _cooldownBitmap;
            }
            else if (trimmed_str == "&casts;")
            {
                return _castsBitmap;
            }
            else if (trimmed_str == "&adds;")
            {
                return _addsBitmap;
            }
            else if (trimmed_str == "&food;")
            {
                return _foodBitmap;
            }
            else if (trimmed_str == "&spabj;")
            {
                return _spellAbjurationBitmap;
            }
            else if (trimmed_str == "&sparc;")
            {
                return _spellArcaneBitmap;
            }
            else if (trimmed_str == "&spcel;")
            {
                return _spellCelestialBitmap;
            }
            else if (trimmed_str == "&spcle;")
            {
                return _spellClericalBitmap;
            }
            else if (trimmed_str == "&spcon;")
            {
                return _spellConjurationBitmap;
            }
            else if (trimmed_str == "&spdiv;")
            {
                return _spellDivinationBitmap;
            }
            else if (trimmed_str == "&spenc;")
            {
                return _spellEnchantmentBitmap;
            }
            else if (trimmed_str == "&sphea;")
            {
                return _spellHealingBitmap;
            }
            else if (trimmed_str == "&spmov;")
            {
                return _spellMovementBitmap;
            }
            else if (trimmed_str == "&spnat;")
            {
                return _spellNatureBitmap;
            }
            else if (trimmed_str == "&spnec;")
            {
                return _spellNecromancyBitmap;
            }
            else if (trimmed_str == "&sptra;")
            {
                return _spellTransmutationBitmap;
            }
            return null;
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
