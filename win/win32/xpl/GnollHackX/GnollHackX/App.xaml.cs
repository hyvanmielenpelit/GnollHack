#if !GNH_MAUI
using Xamarin.Forms;
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
