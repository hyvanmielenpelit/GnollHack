using GnollHackClient.Data;
using GnollHackClient.Pages.Game;
using GnollHackClient.Pages.Login;
using GnollHackCommon.Authentication;
using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Net.Http.Headers;
using System.Text;
using System.Threading.Tasks;

using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace GnollHackClient
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class LoginPage : ContentPage
    {
        private List<Server> _servers = new List<Server>()
        {
            new Server("Local Host", "http://10.0.2.2:57061/")
        };
        private Color _errorColor = Color.Red;
        private Color _infoColor = Color.Black;
        private Color _successColor = Color.Green;
        private bool _canClickButton = true;
        private bool _canClickLogin = true;
        private MainPage _mainPage;

        public IList<Server> Servers { get { return _servers; } }
        public Server SelectedServer { get; set; }

        public string UserName { get; set; }
        public string Password { get; set; }
        
        public LoginPage(MainPage mainPage)
        {
            BindingContext = this;
            _mainPage = mainPage;
            InitializeComponent();
        }

        private async void btnLogin_Clicked(object sender, EventArgs e)
        {
            if (_canClickLogin == false)
            {
                return;
            }

            _canClickLogin = false;

            App.AuthenticationCookie = await Authenticate();
            if(App.AuthenticationCookie != null)
            {
                App.SelectedServer = SelectedServer;
                App.UserName = eUserName.Text;
                App.IsServerGame = true;

                var gamePage = new GamePage(_mainPage);
                await App.Current.MainPage.Navigation.PushModalAsync(gamePage);
            }

            _canClickLogin = true;
        }

        private async Task<Cookie> Authenticate()
        {
            if(SelectedServer == null)
            {
                lblStatus.TextColor = _errorColor;
                lblStatus.Text = "Please select a server.";
                return null;
            }

            if(string.IsNullOrWhiteSpace(UserName))
            {
                lblStatus.TextColor = _errorColor;
                lblStatus.Text = "Please give your user name.";
                return null;
            }

            if (string.IsNullOrWhiteSpace(Password))
            {
                lblStatus.TextColor = _errorColor;
                lblStatus.Text = "Please give your password.";
                return null;
            }

            lblStatus.Text = "";

            CookieContainer cookies = new CookieContainer();
            HttpClientHandler handler = new HttpClientHandler();
            handler.CookieContainer = cookies;
            Uri url = new Uri(SelectedServer.Url + "api/login");

            using (var client = new HttpClient(handler))
            {
                client.BaseAddress = url;
                client.DefaultRequestHeaders.Accept.Add(new MediaTypeWithQualityHeaderValue("application/json"));

                LoginCredentials loginCredentials = new LoginCredentials()
                {
                    UserName = this.UserName,
                    Password = this.Password
                };
                
                using (StringWriter sw = new StringWriter())
                {
                    JsonSerializer js = new JsonSerializer();
                    js.Serialize(sw, loginCredentials);
                    System.Net.Http.StringContent content = new StringContent(sw.ToString(), Encoding.UTF8, "application/json");
                    lblStatus.TextColor = _infoColor;
                    lblStatus.Text = "Connecting to the server " + SelectedServer.Name + ". Please wait...";
                    var response = await client.PostAsync("", content);
                    if (response.IsSuccessStatusCode && response.Headers.Contains("Set-Cookie"))
                    {
                        lblStatus.TextColor = _successColor;
                        lblStatus.Text = "Login successful.";
                        try
                        {
                            var responseCookies = cookies.GetCookies(url).Cast<Cookie>();
                            var cookie = responseCookies.First(c => c.Name == ".AspNetCore.Identity.Application");
                            return cookie;
                        }
                        catch(Exception ex)
                        {
                            lblStatus.TextColor = _errorColor;
                            lblStatus.Text = "Server error: Server did not return authentication cookie.";
                            return null;
                        }
                    }
                    else
                    {
                        lblStatus.TextColor = _errorColor;
                        lblStatus.Text = "Invalid user name or password.";
                        return null;
                    }
                }
            }
        }

        private void btnForgotPassword_Clicked(object sender, EventArgs e)
        {
            
            string urlEnd = "Identity/Account/ForgotPassword";
            OpenWebView(urlEnd, "Recover Password on {0}");
        }

        private void btnRegister_Clicked(object sender, EventArgs e)
        {
            string urlEnd = "Identity/Account/Register";
            OpenWebView(urlEnd, "Sign up to {0}");
        }

        private async void OpenWebView(string urlEnd, string title)
        {
            if (_canClickButton == false)
            {
                return;
            }

            if (SelectedServer == null)
            {
                lblStatus.TextColor = _errorColor;
                lblStatus.Text = "Please select a server.";
                return;
            }

            _canClickButton = false;
            
            string url = SelectedServer.Url + urlEnd;
            var webViewPage = new WebViewPage(url);
            webViewPage.Title = string.Format(title, SelectedServer.Name);
            var navPage = new NavigationPage(webViewPage);

            await App.Current.MainPage.Navigation.PushAsync(navPage);
        }

        private void ContentPage_Appearing(object sender, EventArgs e)
        {
            _canClickButton = true;
        }
    }
}