using GnollHackClient.Data;
using SkiaSharp;
using System;
using System.IO;
using System.Net;
using System.Reflection;
using System.Threading.Tasks;
using Xamarin.Essentials;
using Xamarin.Forms;
using Xamarin.Forms.Xaml;

[assembly: ExportFont("diablo_h.ttf", Alias = "Diablo")]
[assembly: ExportFont("uwch.ttf", Alias = "Underwood")]
[assembly: ExportFont("imrl.ttf", Alias = "Immortal")]
[assembly: ExportFont("endr.ttf", Alias = "Endor")]
[assembly: ExportFont("Lato-Regular.ttf", Alias = "Lato-Regular")]
[assembly: ExportFont("Lato-Bold.ttf", Alias = "Lato-Bold")]
[assembly: ExportFont("DejaVuSansMono.ttf", Alias = "DejaVuSansMono")]
[assembly: ExportFont("DejaVuSansMono-Bold.ttf", Alias = "DejaVuSansMono-Bold")]
namespace GnollHackClient
{
    public delegate Task<bool> BackButtonHandler(object sender, EventArgs e);

    public partial class App : Application
    {
        public App()
        {
            InitializeComponent();
            var mainPage = new MainPage();
            var navPage = new NavigationPage(mainPage);
            MainPage = navPage;

            HideAndroidNavigatioBar = Preferences.Get("HideAndroidNavigationBar", false);
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
            _mainGnollHackService.InitializeGnollHack();
            _fmodService = DependencyService.Get<IFmodService>();
            _fmodService.InitializeFmod();
            _fmodService.LoadBanks();
            _appCloseService = DependencyService.Get<IAppCloseService>();
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


        public static string GHVersionId { get; set; }
        public static string GHVersionString { get; set; }
        public static string GHPath { get; set; }
        public static Cookie AuthenticationCookie { get; set; }
        public static Server SelectedServer { get; set; }
        public static string UserName { get; set; }
        public static bool IsServerGame { get; set; }

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

    }
}
