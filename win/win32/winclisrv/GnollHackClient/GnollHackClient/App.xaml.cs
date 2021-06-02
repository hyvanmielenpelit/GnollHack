using GnollHackClient.Data;
using SkiaSharp;
using System;
using System.IO;
using System.Net;
using System.Reflection;
using System.Threading.Tasks;
using Xamarin.Forms;
using Xamarin.Forms.Xaml;

[assembly: ExportFont("diablo_h.ttf", Alias = "Diablo")]
[assembly: ExportFont("uwch.ttf", Alias = "Underwood")]
[assembly: ExportFont("imrl.ttf", Alias = "Immortal")]
[assembly: ExportFont("endr.ttf", Alias = "Endor")]
[assembly: ExportFont("Lato-Regular.ttf", Alias = "Lato-Regular")]
[assembly: ExportFont("Lato-Bold.ttf", Alias = "Lato-Bold")]
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

            Assembly assembly = GetType().GetTypeInfo().Assembly;
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.diablo_h.ttf"))
            {
                if (stream != null)
                {
                    DiabloTypeface = SKTypeface.FromStream(stream);
                }                    
            }
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.uwch.ttf"))
            {
                if (stream != null)
                {
                    UnderwoodTypeface = SKTypeface.FromStream(stream);
                }
            }

            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.endr.ttf"))
            {
                if (stream != null)
                {
                    EndorTypeface = SKTypeface.FromStream(stream);
                }
            }
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.imrl.ttf"))
            {
                if (stream != null)
                {
                    ImmortalTypeface = SKTypeface.FromStream(stream);
                }
            }
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.shxi.ttf"))
            {
                if (stream != null)
                {
                    XizorTypeface = SKTypeface.FromStream(stream);
                }
            }
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.DejaVuSansMono.ttf"))
            {
                if (stream != null)
                {
                    DejaVuSansMonoTypeface = SKTypeface.FromStream(stream);
                }
            }
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.Lato-Regular.ttf"))
            {
                if (stream != null)
                {
                    LatoRegular = SKTypeface.FromStream(stream);
                }
            }
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.Lato-Bold.ttf"))
            {
                if (stream != null)
                {
                    LatoBold = SKTypeface.FromStream(stream);
                }
            }

            _fmodService = DependencyService.Get<IFmodService>();
            _fmodService.InitializeFmod();
            _fmodService.LoadBanks();

            _appCloseService = DependencyService.Get<IAppCloseService>();
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

        public static Cookie AuthenticationCookie { get; set; }
        public static Server SelectedServer { get; set; }
        public static string UserName { get; set; }
        public static bool IsServerGame { get; set; }

        public static NavigationPage MainNavigationPage { get { return (NavigationPage)Current.MainPage; } }

        public static event BackButtonHandler BackButtonPressed;

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
        public static SKTypeface LatoRegular { get; set; }
        public static SKTypeface LatoBold { get; set; }

    }
}
