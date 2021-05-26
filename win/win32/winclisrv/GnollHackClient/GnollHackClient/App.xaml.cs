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

            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.consola.ttf"))
            {
                if (stream != null)
                {
                    ConsolasTypeface = SKTypeface.FromStream(stream);
                }
            }
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.DejaVuSansMono.ttf"))
            {
                if (stream != null)
                {
                    DejaVuSansMonoTypeface = SKTypeface.FromStream(stream);
                }
            }
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
        public static SKTypeface ConsolasTypeface { get; set; }
        public static SKTypeface UnderwoodTypeface { get; set; }
        public static SKTypeface DejaVuSansMonoTypeface { get; set; }
        
    }
}
