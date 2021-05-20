using GnollHackClient.Data;
using System;
using System.Net;
using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace GnollHackClient
{
    public partial class App : Application
    {
        public App()
        {
            InitializeComponent();

            var mainPage = new MainPage();
            var navPage = new NavigationPage(mainPage);
            
            MainPage = navPage;
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
        public static NavigationPage MainNavigationPage { get { return (NavigationPage)Current.MainPage; } }

    }
}
