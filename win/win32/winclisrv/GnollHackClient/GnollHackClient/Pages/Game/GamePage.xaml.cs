using GnollHackCommon;
using Microsoft.AspNetCore.SignalR.Client;
using SkiaSharp;
using SkiaSharp.Views.Forms;
using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace GnollHackClient.Pages.Game
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class GamePage : ContentPage
    {
        Boolean _connectionAttempted = false;
        private HubConnection _connection;
        private string _connection_status = "";
        public string Message { get { return _message1; } set { _message1 = value; } }
        private string _message1 = "";

        private string _message = "";
        private string _message2 = "";
        private string _message3 = "";
        private string _message4 = "";
        private string _message5 = "";
        private int _result = 0;
        private int _result2 = 0;
        private IFmodService _fmodService;
        private IGnollHackService _gnollHackService;
        private bool _isFirstAppearance = true;
        private Thread _gnhthread;
        private ClientGame _clientGame;
        private MapData[,] _mapData = new MapData[GHConstants.MapCols, GHConstants.MapRows];

        public int ClipX { get; set; }
        public int ClipY { get; set; }

        public IFmodService FModService { get { return _fmodService; } }

        public GamePage()
        {
            InitializeComponent();
            Device.StartTimer(TimeSpan.FromSeconds(1f / 10), () =>
            {
                canvasView.InvalidateSurface();
                pollRequestQueue();
                return true;
            });

            if (App.IsServerGame)
            {

            }

            for(int i = 0; i < GHConstants.MapCols; i++)
            {
                for(int j = 0; j < GHConstants.MapRows; j++)
                {
                    _mapData[i, j] = new MapData();
                }
            }

            _fmodService = DependencyService.Get<IFmodService>();
            _fmodService.InitializeFmod();
            _fmodService.LoadBanks();

            _gnollHackService = DependencyService.Get<IGnollHackService>();
            _gnollHackService.InitializeGnollHack();
        }

        private async void ContentPage_Appearing(object sender, EventArgs e)
        {
            App.BackButtonPressed += BackButtonPressed;

            if(_isFirstAppearance)
            {
                _isFirstAppearance = false;

                int res = 0; // _gnollHackService.Test1();

                //Authenticated
                _fmodService.PlayTestSound();

                if (App.IsServerGame)
                {
                    _connectionAttempted = true;
                    _connection_status = "Not connected";
                    _message = "Please wait...";

                    if (_connection == null)
                    {
                        ConnectToServer();
                    }
                    else if (_connection.State != HubConnectionState.Connected)
                    {
                        await _connection.StopAsync();
                        ConnectToServer();
                    }
                    else
                    {
                        _connection_status = "Connected";
                    }

                    if (_connection != null)
                    {
                        LoginToServer();
                    }
                }
                else
                {
                    Thread t = new Thread(new ThreadStart(GNHThreadProc));
                    _gnhthread = t;
                    _gnhthread.Start();
                }
            }
        }

        protected void GNHThreadProc()
        {
            _clientGame = new ClientGame(this);
            _gnollHackService.StartGnollHack(_clientGame);
        }

        private void pollRequestQueue()
        {
            GHRequest req;
            ConcurrentQueue<GHRequest> queue;
            if(ClientGame.RequestDictionary.TryGetValue(_clientGame, out queue))
            {
                if (queue.TryDequeue(out req))
                {
                    switch (req.RequestType)
                    {
                        case GHRequestType.GetChar:
                            GetChar();
                            break;
                        case GHRequestType.AskName:
                            AskName();
                            break;
                    }
                }
            }
        }

        private async void AskName()
        {
            var namePage = new NamePage(_clientGame, this);
            await App.Current.MainPage.Navigation.PushModalAsync(namePage);
        }

        private void GetChar()
        {
            // Set focus to GameViewPage
        }
        private async Task<bool> BackButtonPressed(object sender, EventArgs e)
        {
            var menu = new GameMenuPage(this);
            await App.Current.MainPage.Navigation.PushModalAsync(menu);

            return false;
        }

        private void ContentPage_Disappearing(object sender, EventArgs e)
        {
            App.BackButtonPressed -= BackButtonPressed;
        }

        private void canvasView_PaintSurface(object sender, SKPaintSurfaceEventArgs e)
        {
            SKImageInfo info = e.Info;
            SKSurface surface = e.Surface;
            SKCanvas canvas = surface.Canvas;

            string str = "";

            canvas.Clear(SKColors.Black);

            float x = info.Width;
            float y = info.Height;

            SKPaint textPaint = new SKPaint
            {
                Color = SKColors.White
            };
            textPaint.Typeface = App.DiabloTypeface;

            // Adjust TextSize property so text is 90% of screen width
            float textWidth = textPaint.MeasureText(str);
            textPaint.TextSize = 0.65f * info.Width * textPaint.TextSize / textWidth;

            // Find the text bounds
            SKRect textBounds = new SKRect();
            textPaint.MeasureText(str, ref textBounds);

            var xText = 10;
            var yText = 0;
            string additional_info = "";
            if (_connection == null && _connectionAttempted)
                additional_info = ", no _connection";
            else if (_connection == null)
            {
                /* Do nothing */
            }
            else if (_connection.State == HubConnectionState.Connected)
                additional_info = ", connected";
            else if (_connection.State == HubConnectionState.Connecting)
                additional_info = ", connecting";
            else if (_connection.State == HubConnectionState.Disconnected)
                additional_info = ", disconnected";
            else if (_connection.State == HubConnectionState.Reconnecting)
                additional_info = ", reconnecting";

            str = _connection_status + additional_info;
            textPaint.TextSize = 36;
            yText = yText + 50;
            canvas.DrawText(str, xText, yText, textPaint);

            str = Message;
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

            if (_result2 != 0)
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

            textPaint.Typeface = App.DejaVuSansMonoTypeface;
            textPaint.TextSize = 48;
            float canvaswidth = canvasView.CanvasSize.Width;
            float canvasheight = canvasView.CanvasSize.Height;
            float width = textPaint.FontMetrics.AverageCharacterWidth;
            float height = textPaint.FontMetrics.Descent - textPaint.FontMetrics.Ascent;
            float mapwidth = width * (GHConstants.MapCols -1);
            float mapheight = height * (GHConstants.MapRows);

            int startX = 1;
            int endX = GHConstants.MapCols - 1;
            int startY = 0;
            int endY = GHConstants.MapRows - 1;

            float offsetX = (canvaswidth - mapwidth) / 2;
            float offsetY = (canvasheight - mapheight) / 2;

            if(ClipX > 0 && (mapwidth > canvaswidth || mapheight > canvasheight))
            {
                offsetX -= (ClipX - (GHConstants.MapCols - 1) / 2) * width;
                offsetY -= (ClipY - GHConstants.MapRows / 2) * height;
            }

            float tx = 0, ty = 0;
            for (int mapx = startX; mapx <= endX; mapx++)
            {
                for(int mapy = startY; mapy <= endY; mapy++)
                {
                    if(_mapData[mapx, mapy] != null && _mapData[mapx, mapy].Symbol != null && _mapData[mapx, mapy].Symbol != "")
                    {
                        str = _mapData[mapx, mapy].Symbol;
                        textPaint.Color = _mapData[mapx, mapy].Color;
                        tx = (offsetX + width * (float)mapx);
                        ty = (offsetY + height * (float)mapy);
                        canvas.DrawText(str, tx, ty, textPaint);
                    }
                }
            }

            textPaint.Typeface = App.DiabloTypeface;
            textPaint.TextSize = 36;
            textPaint.Color = SKColors.White;

            for (int i = 0; _clientGame.Windows[i] != null && i < GHConstants.MaxGHWindows; i++)
            {
                for(int j = 0; j < GHConstants.MaxPutStrHeight; j++)
                {
                    if (_clientGame.Windows[i].PutStrs[j] == null || _clientGame.Windows[i].PutStrs[j] == "")
                        continue;

                    str = _clientGame.Windows[i].PutStrs[j];
                    textPaint.Color = SKColors.White;
                    tx = _clientGame.Windows[i].WinX;
                    ty = _clientGame.Windows[i].WinY + j * height;
                    canvas.DrawText(str, tx, ty, textPaint);
                }
            }
        }

        protected void ConnectToServer()
        {
            if (App.AuthenticationCookie == null)
            {
                throw new Exception("AuthenticationCookie is null");
            }

            if (App.SelectedServer == null)
            {
                throw new Exception("SelectedServer is null");
            }

            _connection = new HubConnectionBuilder().WithUrl(App.SelectedServer.Url + "gnollhack", options =>
            {
                options.Cookies.Add(App.AuthenticationCookie);
            }).Build();

            if (_connection != null)
            {
                _connection_status = "Connection attempted";
            }
            else
            {
                _connection_status = "Connection attempt failed";
            }

            _connection.Closed += async (error) =>
            {
                _connection_status = "Connection closed";
                await Task.Delay(new Random().Next(0, 5) * 1000);
                await _connection.StartAsync();
            };

            _connection.On<string, string>("ReceiveMessage", (user, message) =>
            {
                _message = message;
            });

            _connection.On<int>("CalcResult", (result) =>
            {
                _result = result;
            });

            _connection.On<string, string>("LoginMessage", (user, message) =>
            {
                _message2 = message;
            });

            _connection.On<int>("AddNewGameResult", (result) =>
            {
                _message3 = "New Game Added: " + result;
                _clientGame = new ClientGame(this);
            });

            _connection.On<bool>("GameAliveResult", (result) =>
            {
                _message4 = "Game Alive: " + result.ToString();
            });
            _connection.On<int, int>("Client_ExitHack", (hash, status) =>
            {
                _message5 = "ExitHack: Hash: " + hash + ", Status: " + status;
            });
            _connection.On<int>("Client_PlayerSelection", (hash) =>
            {
                _message5 = "PlayerSelection: Hash: " + hash;
            });
            _connection.On<GHCommandFromServer>("CommandFromServer", (command) =>
            {
                _message5 = "CommandFromServer: " + command.CommandName + ", GUID: " + command.Id.ToString();
            });
            _connection.On<Guid, int>("ResponseFromClientResult", (guid, result) =>
            {
                _message5 = "ResponseFromClientResult: " + result + ", GUID: " + guid;
            });
        }

        protected async void LoginToServer()
        {
            try
            {
                await _connection.StartAsync();

                await _connection.InvokeAsync("SendMessage",
                    "user", "My message");

                await _connection.InvokeAsync("DoCalc");

                await _connection.InvokeAsync("AddNewServerGame");
            }
            catch (Exception ex)
            {
                //Error
            }
        }

        private void canvasView_Touch(object sender, SKTouchEventArgs e)
        {
            if (_clientGame != null)
            {
                if(e.ActionType == SKTouchAction.Pressed)
                {
                    int resp = 0;
                    //string ch = " ";
                    if (e.Location.Y < canvasView.CanvasSize.Height * 0.2 && e.Location.X < canvasView.CanvasSize.Width * 0.2)
                        resp = -7;
                    else if (e.Location.Y < canvasView.CanvasSize.Height * 0.2 && e.Location.X > canvasView.CanvasSize.Width * 0.8)
                        resp = -9;
                    else if (e.Location.Y > canvasView.CanvasSize.Height * 0.8 && e.Location.X < canvasView.CanvasSize.Width * 0.2)
                        resp = -1;
                    else if (e.Location.Y > canvasView.CanvasSize.Height * 0.8 && e.Location.X > canvasView.CanvasSize.Width * 0.8)
                        resp = -3;
                    else if (e.Location.Y < canvasView.CanvasSize.Height * 0.2)
                        resp = -8; //ch = "k";
                    else if (e.Location.Y > canvasView.CanvasSize.Height * 0.8)
                        resp = -2; // ch = "j";
                    else if (e.Location.X < canvasView.CanvasSize.Width * 0.2)
                        resp = -4; // ch = "h";
                    else if (e.Location.X > canvasView.CanvasSize.Width * 0.8)
                        resp = -6; // ch = "l";
                    else
                        resp = 32;

                    ConcurrentQueue<GHResponse> queue;
                    if (ClientGame.ResponseDictionary.TryGetValue(_clientGame, out queue))
                    {
                        queue.Enqueue(new GHResponse(_clientGame, GHRequestType.GetChar, resp));
                        e.Handled = true;
                    }
                }
            }
        }

        public SKColor NHColor2SKColor(nhcolor nhcolor)
        {
            SKColor res = SKColors.Gray;
            switch (nhcolor)
            {
                case nhcolor.CLR_BLACK:
                    res = SKColors.DarkGray;
                    break;
                case nhcolor.CLR_RED:
                    res = SKColors.Red;
                    break;
                case nhcolor.CLR_GREEN:
                    res = SKColors.Green;
                    break;
                case nhcolor.CLR_BROWN:
                    res = SKColors.Brown;
                    break;
                case nhcolor.CLR_BLUE:
                    res = SKColors.Blue;
                    break;
                case nhcolor.CLR_MAGENTA:
                    res = SKColors.Magenta;
                    break;
                case nhcolor.CLR_CYAN:
                    res = SKColors.Cyan;
                    break;
                case nhcolor.CLR_GRAY:
                    res = SKColors.Gray;
                    break;
                case nhcolor.NO_COLOR:
                    break;
                case nhcolor.CLR_ORANGE:
                    res = SKColors.Orange;
                    break;
                case nhcolor.CLR_BRIGHT_GREEN:
                    res = SKColors.LightGreen;
                    break;
                case nhcolor.CLR_YELLOW:
                    res = SKColors.Yellow;
                    break;
                case nhcolor.CLR_BRIGHT_BLUE:
                    res = SKColors.LightBlue;
                    break;
                case nhcolor.CLR_BRIGHT_MAGENTA:
                    res = SKColors.LightPink;
                    break;
                case nhcolor.CLR_BRIGHT_CYAN:
                    res = SKColors.LightCyan;
                    break;
                case nhcolor.CLR_WHITE:
                    res = SKColors.White;
                    break;
                case nhcolor.CLR_MAX:
                    break;
                default:
                    break;
            }

            return res;
        }
        public void SetMapSymbol(int x, int y, int c, int color, uint special)
        {
            _mapData[x, y].Symbol = Char.ConvertFromUtf32(c);
            _mapData[x, y].Color = NHColor2SKColor((nhcolor)color);
        }
        public void ClearMap()
        {
            for(int x = 1; x < GHConstants.MapCols; x++)
            {
                for(int y = 0; y < GHConstants.MapRows; y++)
                {
                    _mapData[x, y].Glyph = 0;
                    _mapData[x, y].BkGlyph = 0;
                    _mapData[x, y].Symbol = "";
                    _mapData[x, y].Color = SKColors.Black;// default(MapData);
                }
            }
        }


        private void PickupButton_Clicked(object sender, EventArgs e)
        {
            GenericButton_Clicked(sender, e, ',');
        }
        private void SearchButton_Clicked(object sender, EventArgs e)
        {
            GenericButton_Clicked(sender, e, 's');
        }
        private void KickButton_Clicked(object sender, EventArgs e)
        {
            GenericButton_Clicked(sender, e, 'k');
        }
        private void UpButton_Clicked(object sender, EventArgs e)
        {
            GenericButton_Clicked(sender, e, '<');
        }
        private void DownButton_Clicked(object sender, EventArgs e)
        {
            GenericButton_Clicked(sender, e, '>');
        }

        private void GenericButton_Clicked(object sender, EventArgs e, int resp)
        {
            if (_clientGame != null)
            {
                ConcurrentQueue<GHResponse> queue;
                if (ClientGame.ResponseDictionary.TryGetValue(_clientGame, out queue))
                {
                    queue.Enqueue(new GHResponse(_clientGame, GHRequestType.GetChar, resp));
                }
            }
        }
    }
}