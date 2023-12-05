#if !GNH_MAUI
using System.Reflection;
using Xamarin.Forms;

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
namespace GnollHackX
{
    public partial class App : Application
    {
        public App()
        {
            InitializeComponent();
            GHApp.Initialize();
            var mainPage = new MainPage();
            var navPage = new NavigationPage(mainPage);
            navPage.BarTextColor = GHColors.White;
            navPage.BarBackgroundColor = GHColors.Black;
            MainPage = navPage;
        }

        protected override void OnStart()
        {
            base.OnStart();
            GHApp.OnStart();
        }

        protected override void OnSleep()
        {
            base.OnSleep();
            GHApp.OnSleep();
        }

        protected override void OnResume()
        {
            base.OnResume();
            GHApp.OnResume();
        }
    }
}
#endif
