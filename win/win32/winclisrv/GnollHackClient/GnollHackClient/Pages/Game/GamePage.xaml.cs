using GnollHackCommon;
using Microsoft.AspNetCore.SignalR.Client;
using SkiaSharp;
using SkiaSharp.Views.Forms;
using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Globalization;
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
        //private string Message { get { lock (MessageLock) { return TopLineLabel.Text; } } set { lock (MessageLock) { TopLineLabel.Text = value; } } }
        //private object MessageLock = new object();
        //private string _message1 = "";

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
        public ClientGame ClientGame { get { return _clientGame; } }
        private MapData[,] _mapData = new MapData[GHConstants.MapCols, GHConstants.MapRows];
        private bool _cursorIsOn;
        private bool _showDirections = false;

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

            Device.StartTimer(TimeSpan.FromSeconds(1f / 2), () =>
            {
                if (ClientGame != null)
                    _cursorIsOn = !_cursorIsOn;
                else
                    _cursorIsOn = false;

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
            if(_clientGame != null)
            {
                GHRequest req;
                ConcurrentQueue<GHRequest> queue;
                if (ClientGame.RequestDictionary.TryGetValue(_clientGame, out queue))
                {
                    if (queue.TryDequeue(out req))
                    {
                        switch (req.RequestType)
                        {
                            case GHRequestType.PrintHistory:
                                PrintHistory(req.MessageHistory);
                                break;
                            case GHRequestType.PrintTopLine:
                                PrintTopLine(req.RequestString, req.RequestStringAttributes);
                                break;
                            case GHRequestType.ShowYnResponses:
                                ShowYnResponses(req.RequestString, req.Responses);
                                break;
                            case GHRequestType.HideYnResponses:
                                HideYnResponses();
                                break;
                            case GHRequestType.ShowDirections:
                                ShowDirections();
                                break;
                            case GHRequestType.HideDirections:
                                HideDirections();
                                break;
                            case GHRequestType.GetChar:
                                GetChar();
                                break;
                            case GHRequestType.AskName:
                                AskName();
                                break;
                            case GHRequestType.GetLine:
                                GetLine(req.RequestString);
                                break;
                            case GHRequestType.ReturnToMainMenu:
                                ClearMap();
                                _clientGame = null;
                                ReturnToMainMenu();
                                break;
                            case GHRequestType.ShowMenuPage:
                                ShowMenuPage(req.RequestMenuInfo != null ? req.RequestMenuInfo : new GHMenuInfo(), req.RequestingGHWindow);
                                break;
                        }
                    }
                }
            }
        }

        private void PrintTopLine(string str, uint attributes)
        {
            /*
            TopLineLabel.Text = str;
            switch(attributes)
            {
                case 0:
                    TopLineLabel.FontAttributes = FontAttributes.None;
                    break;
                case 1:
                    TopLineLabel.FontAttributes = FontAttributes.Bold;
                    break;
            }
            */
        }

        private string CurrentYnResponses;
        private void ShowYnResponses(string question, string responses)
        {
            CurrentYnResponses = responses;
            QuestionLabel.Text = question;
            if (responses.Length == 0)
                return;
            else if(responses.Length == 1)
            {
                SecondButton.Text = responses;
                ZeroButton.IsVisible = false;
                FirstButton.IsVisible = false;
                SecondButton.IsVisible = true;
                ThirdButton.IsVisible = false;
                FourthButton.IsVisible = false;
            }
            else if(responses.Length == 2)
            {
                ZeroButton.IsVisible = false;
                FirstButton.IsVisible = true;
                SecondButton.IsVisible = true;
                ThirdButton.IsVisible = false;
                FourthButton.IsVisible = false;
                FirstButton.Text = responses.Substring(0, 1);
                SecondButton.Text = responses.Substring(1, 1);
            }
            else if (responses.Length == 3)
            {
                ZeroButton.IsVisible = false;
                FirstButton.IsVisible = true;
                SecondButton.IsVisible = true;
                ThirdButton.IsVisible = true;
                FourthButton.IsVisible = false;
                FirstButton.Text = responses.Substring(0, 1);
                SecondButton.Text = responses.Substring(1, 1);
                ThirdButton.Text = responses.Substring(2, 1);
            }
            else
            {
                Button[] btnList = { ZeroButton, FirstButton, SecondButton, ThirdButton, FourthButton };


                for (int i = 0; i < 4; i++)
                {
                    if (i < responses.Length)
                    {
                        btnList[i].Text = responses.Substring(i, 1);
                        btnList[i].IsVisible = true;
                    }
                    else
                    {
                        btnList[i].Text = "?";
                        btnList[i].IsVisible = false;
                    }
                }
            }

            YnGrid.IsVisible = true;
        }

        private void HideYnResponses()
        {
            YnGrid.IsVisible = false;
        }
        private void ShowDirections()
        {
            _showDirections = true;
        }
        private void HideDirections()
        {
            _showDirections = false;
        }

        private void PrintHistory(List<GHMsgHistoryItem> msgHistory)
        {
            MessageHistoryView.ItemsSource = msgHistory;
            if(msgHistory.Count > 0)
                MessageHistoryView.ScrollTo(msgHistory.Count - 1, -1, ScrollToPosition.End, false);
        }

        private async void AskName()
        {
            var namePage = new NamePage(this);
            await App.Current.MainPage.Navigation.PushModalAsync(namePage);
        }
        private async void GetLine(string query)
        {
            var getLinePage = new GetLinePage(this, query);
            await App.Current.MainPage.Navigation.PushModalAsync(getLinePage);
        }

        private void GetChar()
        {
            // Set focus to GameViewPage
        }
        private async void ReturnToMainMenu()
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
        }
        private async void ShowMenuPage(GHMenuInfo menuinfo, GHWindow ghwindow)
        {
            var menuPage = new GHMenuPage(this, ghwindow);
            menuPage.SelectionHow = menuinfo.SelectionHow;
            menuPage.Header = menuinfo.Header;
            if (menuinfo != null)
                menuPage.MenuItems.AddRange(menuinfo.MenuItems);
            menuPage.Process();
            await App.Current.MainPage.Navigation.PushModalAsync(menuPage);
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


            /* Map */
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
                        if ((_mapData[mapx, mapy].Special & (uint)MapSpecial.Pet) != 0)
                        {
                            textPaint.Style = SKPaintStyle.Fill;
                            SKRect winRect = new SKRect(tx, ty + textPaint.FontMetrics.Descent, tx + width, ty + textPaint.FontMetrics.Descent + height);
                            canvas.DrawRect(winRect, textPaint);
                            textPaint.Color = SKColors.Black;
                        }
                        tx = (offsetX + width * (float)mapx);
                        ty = (offsetY + height * (float)mapy);
                        canvas.DrawText(str, tx, ty, textPaint);

                        if ((_mapData[mapx, mapy].Special & (uint)MapSpecial.Peaceful) != 0)
                        {
                            canvas.DrawText("_", tx, ty, textPaint);
                        }
                    }
                }
            }


            if (_clientGame != null)
            {
                /* Window strings */
                lock (_clientGame.WindowsLock)
                {
                    /* Cursor */
                    if (_clientGame.MapWindowId > 0 && _clientGame.Windows[_clientGame.MapWindowId] != null && _clientGame.Windows[_clientGame.MapWindowId].WindowType == GHWinType.Map && _cursorIsOn)
                    {
                        int cx = _clientGame.Windows[_clientGame.MapWindowId].CursX, cy = _clientGame.Windows[_clientGame.MapWindowId].CursY;
                        str = "_";
                        textPaint.Color = SKColors.White;
                        tx = (offsetX + width * (float)cx);
                        ty = (offsetY + height * (float)cy);
                        canvas.DrawText(str, tx, ty, textPaint);
                    }

                    for (int i = 0; _clientGame.Windows[i] != null && i < GHConstants.MaxGHWindows; i++)
                    {
                        if (_clientGame.Windows[i].Visible)
                        {
                            textPaint.Typeface = _clientGame.Windows[i].Typeface;
                            textPaint.TextSize = _clientGame.Windows[i].TextSize;
                            textPaint.Color = _clientGame.Windows[i].TextColor;
                            width = textPaint.FontMetrics.AverageCharacterWidth;
                            height = textPaint.FontMetrics.Descent - textPaint.FontMetrics.Ascent;

                            SKRect winRect = new SKRect(_clientGame.Windows[i].Left, _clientGame.Windows[i].Top,
                                _clientGame.Windows[i].Right,
                                _clientGame.Windows[i].Bottom);

                            if(_clientGame.Windows[i].CenterHorizontally && winRect.Right - winRect.Left < canvaswidth)
                            {
                                float newleft = (canvaswidth - (winRect.Right - winRect.Left)) / 2;
                                float addition = newleft - winRect.Left;
                                winRect.Left += addition;
                                winRect.Right += addition;
                            }

                            SKPaint winPaint = new SKPaint
                            {
                                Color = _clientGame.Windows[i].BackgroundColor,
                                Style = SKPaintStyle.Fill
                            };

                            canvas.DrawRect(winRect, winPaint);

                            for (int j = 0; j < GHConstants.MaxPutStrHeight; j++)
                            {
                                if (_clientGame.Windows[i].PutStrs[j] == null || _clientGame.Windows[i].PutStrs[j] == "")
                                    continue;

                                str = _clientGame.Windows[i].PutStrs[j];
                                textPaint.Color = SKColors.White;
                                tx = winRect.Left + _clientGame.Windows[i].Padding.Left;
                                ty = winRect.Top + _clientGame.Windows[i].Padding.Top - textPaint.FontMetrics.Ascent + j * height;
                                canvas.DrawText(str, tx, ty, textPaint);
                            }
                        }
                    }
                }

            }

            if(_showDirections)
            {
                textPaint.Color = new SKColor(255, 255, 255, 128);
                textPaint.Typeface = App.DejaVuSansMonoTypeface;
                textPaint.TextSize = 400;
                for (int i = 0; i < 8;i++)
                {
                    switch(i)
                    {
                        case 0:
                            str = "\u2190";
                            tx = canvasView.CanvasSize.Width * 0.15f - textPaint.FontMetrics.AverageCharacterWidth / 2;
                            ty = canvasView.CanvasSize.Height / 2 + textPaint.FontMetrics.Descent;
                            break;
                        case 1:
                            str = "\u2191";
                            tx = canvasView.CanvasSize.Width / 2 - textPaint.FontMetrics.AverageCharacterWidth / 2;
                            ty = canvasView.CanvasSize.Height * 0.15f + textPaint.FontMetrics.Descent;
                            break;
                        case 2:
                            str = "\u2192";
                            tx = canvasView.CanvasSize.Width * 0.85f - textPaint.FontMetrics.AverageCharacterWidth / 2;
                            ty = canvasView.CanvasSize.Height / 2 + textPaint.FontMetrics.Descent;
                            break;
                        case 3:
                            str = "\u2193";
                            tx = canvasView.CanvasSize.Width / 2 - textPaint.FontMetrics.AverageCharacterWidth / 2;
                            ty = canvasView.CanvasSize.Height * 0.85f + textPaint.FontMetrics.Descent;
                            break;
                        case 4:
                            str = "\u2196";
                            tx = canvasView.CanvasSize.Width * 0.15f - textPaint.FontMetrics.AverageCharacterWidth / 2;
                            ty = canvasView.CanvasSize.Height * 0.15f + textPaint.FontMetrics.Descent;
                            break;
                        case 5:
                            str = "\u2197";
                            tx = canvasView.CanvasSize.Width * 0.85f - textPaint.FontMetrics.AverageCharacterWidth / 2;
                            ty = canvasView.CanvasSize.Height * 0.15f + textPaint.FontMetrics.Descent;
                            break;
                        case 6:
                            str = "\u2198";
                            tx = canvasView.CanvasSize.Width * 0.85f - textPaint.FontMetrics.AverageCharacterWidth / 2;
                            ty = canvasView.CanvasSize.Height * 0.85f + textPaint.FontMetrics.Descent;
                            break;
                        case 7:
                            str = "\u2199";
                            tx = canvasView.CanvasSize.Width * 0.15f - textPaint.FontMetrics.AverageCharacterWidth / 2;
                            ty = canvasView.CanvasSize.Height * 0.85f + textPaint.FontMetrics.Descent;
                            break;
                    }
                    canvas.DrawText(str, tx, ty, textPaint);

                }
            }


            /* RawPrint */
            /*
            lock(MessageLock)
            {
                str = Message;
            }

            textPaint.Typeface = App.DiabloTypeface;
            textPaint.TextSize = 36;
            textPaint.Color = SKColors.White;
            yText = 50;
            canvas.DrawText(str, xText, yText, textPaint);
            */


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
                    if (e.Location.Y < canvasView.CanvasSize.Height * 0.3 && e.Location.X < canvasView.CanvasSize.Width * 0.3)
                        resp = -7;
                    else if (e.Location.Y < canvasView.CanvasSize.Height * 0.3 && e.Location.X > canvasView.CanvasSize.Width * 0.7)
                        resp = -9;
                    else if (e.Location.Y > canvasView.CanvasSize.Height * 0.7 && e.Location.X < canvasView.CanvasSize.Width * 0.3)
                        resp = -1;
                    else if (e.Location.Y > canvasView.CanvasSize.Height * 0.7 && e.Location.X > canvasView.CanvasSize.Width * 0.7)
                        resp = -3;
                    else if (e.Location.Y < canvasView.CanvasSize.Height * 0.3)
                        resp = -8; //ch = "k";
                    else if (e.Location.Y > canvasView.CanvasSize.Height * 0.7)
                        resp = -2; // ch = "j";
                    else if (e.Location.X < canvasView.CanvasSize.Width * 0.3)
                        resp = -4; // ch = "h";
                    else if (e.Location.X > canvasView.CanvasSize.Width * 0.7)
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

        private SKColor GHDarkGray = new SKColor(96, 96, 96);

        public SKColor NHColor2SKColor(nhcolor nhcolor)
        {
            SKColor res = SKColors.Gray;
            switch (nhcolor)
            {
                case nhcolor.CLR_BLACK:
                    res = GHDarkGray;
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
                    res = SKColors.LightGray;
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
            _mapData[x, y].Special = special;
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
                    _mapData[x, y].Special = 0;
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

        public void GenericButton_Clicked(object sender, EventArgs e, int resp)
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

        private void InventoryButton_Clicked(object sender, EventArgs e)
        {
            GenericButton_Clicked(sender, e, 'i');
        }
        private void LookHereButton_Clicked(object sender, EventArgs e)
        {
            GenericButton_Clicked(sender, e, ':');
        }
        private void WaitButton_Clicked(object sender, EventArgs e)
        {
            GenericButton_Clicked(sender, e, '.');
        }
        private void FireButton_Clicked(object sender, EventArgs e)
        {
            GenericButton_Clicked(sender, e, 'f');
        }

        private async void MenuButton_Clicked(object sender, EventArgs e)
        {
            var cmdPage = new CommandPage(this);
            await App.Current.MainPage.Navigation.PushModalAsync(cmdPage);

        }

        private void ZeroButton_Clicked(object sender, EventArgs e)
        {
            if (CurrentYnResponses.Length >= 4)
                GenericButton_Clicked(sender, e, (int)CurrentYnResponses.Substring(0, 1).ToCharArray()[0]);
        }
        private void FirstButton_Clicked(object sender, EventArgs e)
        {
            if (CurrentYnResponses.Length >= 4)
                GenericButton_Clicked(sender, e, (int)CurrentYnResponses.Substring(1, 1).ToCharArray()[0]);
            else if (CurrentYnResponses.Length >= 2)
                GenericButton_Clicked(sender, e, (int)CurrentYnResponses.Substring(0, 1).ToCharArray()[0]);
        }

        private void SecondButton_Clicked(object sender, EventArgs e)
        {
            if (CurrentYnResponses.Length == 1)
                GenericButton_Clicked(sender, e, (int)CurrentYnResponses.Substring(0, 1).ToCharArray()[0]);
            else if (CurrentYnResponses.Length >= 4)
                GenericButton_Clicked(sender, e, (int)CurrentYnResponses.Substring(2, 1).ToCharArray()[0]);
            else if (CurrentYnResponses.Length >= 2)
                GenericButton_Clicked(sender, e, (int)CurrentYnResponses.Substring(1, 1).ToCharArray()[0]);
        }

        private void ThirdButton_Clicked(object sender, EventArgs e)
        {
            if (CurrentYnResponses.Length == 3)
                GenericButton_Clicked(sender, e, (int)CurrentYnResponses.Substring(2, 1).ToCharArray()[0]);
            else if (CurrentYnResponses.Length >= 4)
                GenericButton_Clicked(sender, e, (int)CurrentYnResponses.Substring(3, 1).ToCharArray()[0]);
        }

        private void FourthButton_Clicked(object sender, EventArgs e)
        {
            if (CurrentYnResponses.Length >= 5)
                GenericButton_Clicked(sender, e, (int)CurrentYnResponses.Substring(4, 1).ToCharArray()[0]);
        }

        private void RepeatButton_Clicked(object sender, EventArgs e)
        {
            GenericButton_Clicked(sender, e, GHUtils.Ctrl('A'));
        }

        private void CastButton_Clicked(object sender, EventArgs e)
        {
            GenericButton_Clicked(sender, e, 'Z');
        }

    }

    public class ColorConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {

            if ((bool)value == true)
            {
                return Color.White;
            }

            return Color.DarkGray;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if ((bool)value == true)
            {
                return Color.White;
            }

            return Color.DarkGray;
        }
    }

}