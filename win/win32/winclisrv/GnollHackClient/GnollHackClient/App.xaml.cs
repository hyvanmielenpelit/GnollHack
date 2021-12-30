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
using Plugin.SimpleAudioPlayer;

[assembly: ExportFont("diablo_h.ttf", Alias = "Diablo")]
[assembly: ExportFont("uwch.ttf", Alias = "Underwood")]
[assembly: ExportFont("imrl.ttf", Alias = "Immortal")]
[assembly: ExportFont("endr.ttf", Alias = "Endor")]
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
            App.LoadServices();
            var mainPage = new MainPage();
            var navPage = new NavigationPage(mainPage);
            MainPage = navPage;
            App.HideAndroidNavigatioBar = Preferences.Get("HideAndroidNavigationBar", false);
            App.DeveloperMode = Preferences.Get("DeveloperMode", GHConstants.DefaultDeveloperMode);
            App.FullVersionMode = true; // Preferences.Get("FullVersion", true);
            App.BeginnerMode = Preferences.Get("BeginnerMode", false);
            App.SponsorButtonVisited = Preferences.Get("SponsorButtonVisited", false);
            App.ShowSpecialEffect = Preferences.Get("ShowSpecialEffect", false);
            App.ReadSecrets();
            Array.Sort<SecretsFile>(App.CurrentSecrets.files, new SecretsFileSizeComparer());
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
        }

        protected override void OnSleep()
        {
        }

        protected override void OnResume()
        {
        }

        public static void LoadServices()
        {
            _mainGnollHackService = DependencyService.Get<IGnollHackService>();
            _mainGnollHackService.LoadLibrary();
            _mainGnollHackService.Test();
            _fmodService = DependencyService.Get<IFmodService>();
            _appCloseService = DependencyService.Get<IAppCloseService>();
        }

        public static void InitializeServices()
        {
            bool resetFiles = Preferences.Get("ResetAtStart", true);
            if (resetFiles)
            {
                _mainGnollHackService.ClearFiles();
                Preferences.Set("ResetAtStart", false);
                Preferences.Set("ResetExternalFiles", true);
            }
            ResetAcquiredFiles();
            _mainGnollHackService.InitializeGnollHack(App.CurrentSecrets);
            _fmodService.InitializeFmod();
            _fmodService.LoadBanks();
        }

        private static void ResetAcquiredFiles()
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
        public static bool HideAndroidNavigatioBar
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

        public static bool IsModernAndroid { get; set; }
        public static bool DeveloperMode { get; set; }
        public static bool FullVersionMode { get; set; }
        public static bool BeginnerMode { get; set; }
        public static bool ServerGameAvailable { get; set; }

        public static string GHVersionId { get; set; }
        public static string GHVersionString { get; set; }
        public static string GHPath { get; set; }
        public static Cookie AuthenticationCookie { get; set; }
        public static Server SelectedServer { get; set; }
        public static string UserName { get; set; }
        public static bool IsServerGame { get; set; }
        public static bool SponsorButtonVisited { get; set; }
        public static bool ShowSpecialEffect { get; set; }


        public static NavigationPage MainNavigationPage { get { return (NavigationPage)Current.MainPage; } }

        public static event BackButtonHandler BackButtonPressed;

        private static IGnollHackService _mainGnollHackService = null;
        public static IGnollHackService GnollHackService { get { return _mainGnollHackService; } }
        private static IFmodService _fmodService = null;
        public static IFmodService FmodService { get { return _fmodService; } }
        private static IAppCloseService _appCloseService = null;
        public static IAppCloseService AppCloseService { get { return _appCloseService; } }

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
                    catch
                    {
                        //E.g., if typeface already exists in the dictionary
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
                    catch
                    {
                        //E.g., if typeface already exists in the dictionary
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
                    catch
                    {
                        //E.g., if typeface already exists in the dictionary
                    }
                }
            }
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.imrl.ttf"))
            {
                if (stream != null)
                {
                    try
                    {
                        App.ImmortalTypeface = SKTypeface.FromStream(stream);
                        App.TypefaceDictionary.Add("Immortal", App.ImmortalTypeface);
                    }
                    catch
                    {
                        //E.g., if typeface already exists in the dictionary
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
                    catch
                    {
                        //E.g., if typeface already exists in the dictionary
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
                    catch
                    {
                        //E.g., if typeface already exists in the dictionary
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
                    catch
                    {
                        //E.g., if typeface already exists in the dictionary
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
                    catch
                    {
                        //E.g., if typeface already exists in the dictionary
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
                    catch
                    {
                        //E.g., if typeface already exists in the dictionary
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
                    catch
                    {
                        //E.g., if typeface already exists in the dictionary
                    }
                }
            }
        }

        public static SKBitmap MenuBackgroundBitmap { get; set; }
        public static SKBitmap DarkMarbleBackgroundBitmap { get; set; }
        public static SKBitmap OldPaperBackgroundBitmap { get; set; }

        public static void InitBitmaps(Assembly assembly)
        {
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.menubackground.png"))
            {
                MenuBackgroundBitmap = SKBitmap.Decode(stream);
            }
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.background-darkmarble.png"))
            {
                DarkMarbleBackgroundBitmap = SKBitmap.Decode(stream);
            }
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.UI.background-oldpaper.png"))
            {
                OldPaperBackgroundBitmap = SKBitmap.Decode(stream);
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
                string[] parameterNames = new string[1];
                float[] parameterValues = new float[1];

                _fmodService.PlayImmediateSound(GHConstants.ButtonClickGHSound, GHConstants.ButtonClickEventPath, GHConstants.ButtonClickBankId, GHConstants.ButtonClickVolume,
                    1.0f, parameterNames, parameterValues, 0, 0, 0, 0);
            }
        }

        private static ISimpleAudioPlayer _soundPlayer = CrossSimpleAudioPlayer.CreateSimpleAudioPlayer();
        public static void PlayButtonClickedSoundB()
        {
            _soundPlayer.Load(GetStreamFromFile("Assets.test4.wav"));
            _soundPlayer.Volume = 0.075;
            _soundPlayer.Play();
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

        private static ISimpleAudioPlayer _musicPlayer = CrossSimpleAudioPlayer.CreateSimpleAudioPlayer();
        public static void PlayIntroMusic()
        {
            if (_musicPlayer.IsPlaying)
                return;

            _musicPlayer.Load(GetStreamFromFile("Assets.testm.mp3"));
            _musicPlayer.Volume = 0.25;
            _musicPlayer.Loop = true;
            _musicPlayer.Play();
        }

        public static void PlayMenuSelectSound()
        {
            if (_fmodService != null)
            {
                string[] parameterNames = new string[1];
                float[] parameterValues = new float[1];

                _fmodService.PlayImmediateSound(GHConstants.MenuSelectGHSound, GHConstants.MenuSelectEventPath, GHConstants.MenuSelectBankId, GHConstants.MenuSelectVolume,
                    1.0f, parameterNames, parameterValues, 0, 0, 0, 0);
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
