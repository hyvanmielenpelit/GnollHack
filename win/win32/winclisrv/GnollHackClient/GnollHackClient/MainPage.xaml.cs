using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Xamarin.Forms;
using SkiaSharp;
using SkiaSharp.Views.Forms;

using System.Runtime.InteropServices;
using Microsoft.AspNetCore.SignalR.Client;
using Microsoft.AspNetCore.Authentication;
using System.Buffers.Text;
using System.Text.Encodings.Web;
using System.Net.Http;
using System.IO;
using System.Threading;
using System.Diagnostics;
using GnollHackCommon;
using System.Reflection;

[assembly: ExportFont("diablo_h.ttf", Alias = "Diablo")]

namespace GnollHackClient
{
    public partial class MainPage : ContentPage
    {
        class Person
        {
            public Person(string name, DateTime birthday, Color favoriteColor)
            {
                this.Name = name;
                this.Birthday = birthday;
                this.FavoriteColor = favoriteColor;
            }

            public string Name { private set; get; }

            public DateTime Birthday { private set; get; }

            public Color FavoriteColor { private set; get; }
        };

        Boolean _connectionAttempted = false;
        private HubConnection connection;
        private string _connection_status = "";
        private string _message = "";
        private string _message2 = "";
        private string _message3 = "";
        private string _message4 = "";
        private string _message5 = "";
        private int _result = 0;
        private int _result2 = 0;
        private string _accessToken = "MyAccessToken";
        SKBitmap _background_bitmap;
        SKBitmap _logo_bitmap;
        SKTypeface _typeface;
        private IFmodService _fmodService;
        private IGnollHackService _gnollHackService;

        public MainPage()
        {
            InitializeComponent();
            Assembly assembly = GetType().GetTypeInfo().Assembly;

            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.window_background.png"))
            {
                _background_bitmap = SKBitmap.Decode(stream);
            }

            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.gnollhack-logo-test-2.png"))
            {
                _logo_bitmap = SKBitmap.Decode(stream);
            }

            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.diablo_h.ttf"))
            {
                if (stream != null)
                    _typeface = SKTypeface.FromStream(stream);
            }

            //firstButton.ImageSource = ImageSource.FromResource("GnollHackClient.Assets.button_normal.png", assembly);
            myImage.Source = ImageSource.FromResource("GnollHackClient.Assets.button_normal.png", assembly);
            myImage2.Source = ImageSource.FromResource("GnollHackClient.Assets.button_normal.png", assembly);


            /*
                            // Define some data.
                            List<Person> people = new List<Person>
                            {
                                new Person("Abigail", new DateTime(1975, 1, 15), Color.Aqua),
                                new Person("Bob", new DateTime(1976, 2, 20), Color.Black),
                                // ...etc.,...
                                new Person("Yvonne", new DateTime(1987, 1, 10), Color.Purple),
                                new Person("Zachary", new DateTime(1988, 2, 5), Color.Red)
                            };

                            // Create the ListView.
                            // Source of data items.
                            myListView.ItemsSource = people;
                            myListView.ItemTemplate = new DataTemplate(() =>
                                {
                                // Create views with bindings for displaying each property.
                                Label nameLabel = new Label();
                                    nameLabel.SetBinding(Label.TextProperty, "Name");

                                    Label birthdayLabel = new Label();
                                    birthdayLabel.SetBinding(Label.TextProperty,
                                        new Binding("Birthday", BindingMode.OneWay,
                                            null, null, "Born {0:d}"));

                                    BoxView boxView = new BoxView();
                                    boxView.SetBinding(BoxView.ColorProperty, "FavoriteColor");

                                // Return an assembled ViewCell.
                                return new ViewCell
                                    {
                                        View = new StackLayout
                                        {
                                            Padding = new Thickness(0, 5),
                                            Orientation = StackOrientation.Horizontal,
                                            Children =
                                                {
                                                boxView,
                                                new StackLayout
                                                {
                                                    VerticalOptions = LayoutOptions.Center,
                                                    Spacing = 0,
                                                    Children =
                                                    {
                                                        nameLabel,
                                                        birthdayLabel
                                                    }
                                                    }
                                                }
                                        }
                                    };
                                });
                        }
            */

            Device.StartTimer(TimeSpan.FromSeconds(1f / 40), () =>
            {
                canvasView.InvalidateSurface();
                return true;
            });

            _fmodService = DependencyService.Get<IFmodService>();
            _fmodService.InitializeFmod();
            _fmodService.LoadBanks();

            _gnollHackService = DependencyService.Get<IGnollHackService>();
            _gnollHackService.InitializeGnollHack();
            int res = _gnollHackService.Test1();
            _message = "GnollHack: " + res;
            int res2 = _gnollHackService.Test2();
            _message2 = "GnollHack2: " + res2;
        }

        protected void ConnectToServer()
        {
            connection = new HubConnectionBuilder()
            .WithUrl("http://10.0.2.2:57061/gnollhack", options =>
            {
                var cookie = new System.Net.Cookie(".AspNetCore.Cookies", _accessToken, "/", "localhost");
                options.Cookies.Add(cookie);
            })
            .Build();

            if(connection != null)
                _connection_status = "Connection attempted";
            else
                _connection_status = "Connection attempt failed";

            connection.Closed += async (error) =>
            {
                _connection_status = "Connection closed";
                await Task.Delay(new Random().Next(0, 5) * 1000);
                await connection.StartAsync();
            };

            connection.On<string, string>("ReceiveMessage", (user, message) =>
            {
                _message = message;
            });

            connection.On<int>("CalcResult", (result) =>
            {
                _result = result;
            });

            connection.On<string, string>("LoginMessage", (user, message) =>
            {
                _message2 = message;
            });

            connection.On<int>("AddNewGameResult", (result) =>
            {
                _message3 = "New Game Added: " + result;
            });

            connection.On<bool>("GameAliveResult", (result) =>
            {
                _message4 = "Game Alive: " + result.ToString();
            });
            connection.On<int, int>("Client_ExitHack", (hash, status) =>
            {
                _message5 = "ExitHack: Hash: " + hash + ", Status: " + status;
            });
            connection.On<int>("Client_PlayerSelection", (hash) =>
            {
                _message5 = "PlayerSelection: Hash: " + hash;
            });
            connection.On<GHCommandFromServer>("CommandFromServer", (command) =>
            {
                _message5 = "CommandFromServer: " + command.CommandName + ", GUID: " + command.Id.ToString();
            });
            connection.On<Guid, int>("ResponseFromClientResult", (guid, result) =>
            {
                _message5 = "ResponseFromClientResult: " + result + ", GUID: " + guid;
            });
        }

        protected async void LoginToServer()
        {
            try
            {
                var client = new HttpClient();

                var content = new FormUrlEncodedContent(new[]
                {
                    new KeyValuePair<string, string>("Input.UserName", "Tommi"),
                    new KeyValuePair<string, string>("Input.Password", "HMPTommi1!")
                });

                //var result = await client.PostAsync("https://localhost:44333/Identity/Account/LoginRemote", content);
                //var s = "https://localhost:44333/Identity/Account/LoginRemote?UserName=Tommi&Password=HMPTommi1!";
                //var result = await client.GetAsync(s);

                await connection.StartAsync();

                await connection.InvokeAsync("SendMessage",
                    "user", "My message");

                await connection.InvokeAsync("DoCalc");

                await connection.InvokeAsync("AddNewServerGame");
            }
            catch (Exception ex)
            {
                //Error
            }
        }

        private void canvasView_PaintSurface(object sender, SKPaintSurfaceEventArgs e)
        {
            SKImageInfo info = e.Info;
            SKSurface surface = e.Surface;
            SKCanvas canvas = surface.Canvas;

            canvas.Clear(SKColors.DarkGray);

            float x = info.Width;
            float y = info.Height;

            SKRect bkdest = new SKRect(0f, 0f, x, y);
            canvas.DrawBitmap(_background_bitmap, bkdest);

            float logo_width = (float)info.Width;
            float logo_height = (float)_logo_bitmap.Height * logo_width / ((float)_logo_bitmap.Width);
            x = (info.Width - logo_width) / 2;
            y = (float)info.Height * 0.2f;
            SKRect dest = new SKRect(x, y, x + logo_width, y + logo_height);
            canvas.DrawBitmap(_logo_bitmap, dest);

            string str = "GnollHack 4.1.0";

            // Create an SKPaint object to display the text
            SKPaint textPaint = new SKPaint
            {
                Color = SKColors.Black
            };

            if (_typeface != null)
                textPaint.Typeface = _typeface;

            // Adjust TextSize property so text is 90% of screen width
            float textWidth = textPaint.MeasureText(str);
            textPaint.TextSize = 0.65f * info.Width * textPaint.TextSize / textWidth;

            // Find the text bounds
            SKRect textBounds = new SKRect();
            textPaint.MeasureText(str, ref textBounds);

            float xText = info.Width / 2 - textBounds.MidX;
            float yText = y + logo_height + 30; // info.Height / 2 - textBounds.MidY;

            // And draw the text
            canvas.DrawText(str, xText, yText, textPaint);

            str = "Android Version";
            yText = yText + textPaint.TextSize + 5;
            textPaint.MeasureText(str, ref textBounds);
            xText = info.Width / 2 - textBounds.MidX;
            canvas.DrawText(str, xText, yText, textPaint);

            xText = 10;
            yText = 0;
            string additional_info = "";
            if(connection == null && _connectionAttempted)
                additional_info = ", no connection";
            else if (connection == null)
            {
                /* Do nothing */
            }
            else if (connection.State == HubConnectionState.Connected)
                additional_info = ", connected";
            else if (connection.State == HubConnectionState.Connecting)
                additional_info = ", connecting";
            else if (connection.State == HubConnectionState.Disconnected)
                additional_info = ", disconnected";
            else if (connection.State == HubConnectionState.Reconnecting)
                additional_info = ", reconnecting";

            str = _connection_status + additional_info;
            textPaint.TextSize = 36;
            yText = yText + 50;
            canvas.DrawText(str, xText, yText, textPaint);

            str = _message;
            textPaint.TextSize = 36;
            yText = yText + 50;
            canvas.DrawText(str, xText, yText, textPaint);

            str = _message2;
            textPaint.TextSize = 36;
            yText = yText + 50;
            canvas.DrawText(str, xText, yText, textPaint);

            if (_result != 0)
            {
                str = _result.ToString();
                yText = yText + 50;
                canvas.DrawText(str, xText, yText, textPaint);
            }

            if(_result2 != 0)
            {
                str = _result2.ToString();
                yText = yText + 50;
                canvas.DrawText(str, xText, yText, textPaint);
            }

            str = _message3;
            yText = yText + 50;
            canvas.DrawText(str, xText, yText, textPaint);

            str = _message4;
            yText = yText + 50;
            canvas.DrawText(str, xText, yText, textPaint);

            str = _message5;
            yText = yText + 50;
            canvas.DrawText(str, xText, yText, textPaint);

        }

        private void localButton_Clicked(object sender, EventArgs e)
        {
            _gnollHackService.TestRunGnollHack();
        }

        private void serverButton_Clicked(object sender, EventArgs e)
        {
            _fmodService.PlayTestSound();

            _connectionAttempted = true;
            _connection_status = "Not connected";
            _message = "Please wait...";

            if (connection == null)
                ConnectToServer();
            else if(connection.State != HubConnectionState.Connected)
            {
                connection.StopAsync();
                ConnectToServer();
            }
            else
                _connection_status = "Connected";

            if (connection != null)
                LoginToServer();
        }
    }
}
