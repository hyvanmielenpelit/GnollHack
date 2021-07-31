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
using Xamarin.Essentials;
using System.Runtime.InteropServices;
using System.Reflection;
using System.IO;
using FFImageLoading.Forms;
using System.Diagnostics;

namespace GnollHackClient.Pages.Game
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class GamePage : ContentPage
    {
        private SKColor _cursorDefaultGreen = new SKColor(0, 255, 0);
        private SKRect canvasButtonRect = new SKRect(0, 0, 0, 0);

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
        private IGnollHackService _gnollHackService;
        public IGnollHackService GnollHackService { get { return _gnollHackService;} }
        private bool _isFirstAppearance = true;
        private Thread _gnhthread;
        private ClientGame _clientGame;
        public ClientGame ClientGame { get { return _clientGame; } }
        private MapData[,] _mapData = new MapData[GHConstants.MapCols, GHConstants.MapRows];
        private object _mapDataLock = new object();
        private int _mapCursorX;
        private int _mapCursorY;

        public object RefreshScreenLock = new object();
        private bool _refreshScreen = true;
        public bool RefreshScreen
        {
            get { return _refreshScreen; }
            set { _refreshScreen = value; }
        }

        private game_cursor_types _cursorType;
        private bool _force_paint_at_cursor;
        private bool _show_cursor_on_u;

        private ObjectData[,] _objectData = new ObjectData[GHConstants.MapCols, GHConstants.MapRows];
        private object _objectDataLock = new object();

        private bool _useMapBitmap = false;

        private int _shownMessageRows = GHConstants.DefaultMessageRows;
        public int NumDisplayedMessages { get { return _shownMessageRows; } set { _shownMessageRows = value; } }
        public TTYCursorStyle CursorStyle { get; set; }
        public GHGraphicsStyle GraphicsStyle { get; set; }
        public bool ShowFPS { get; set; }
        private double _fps;
        private long _previousGeneralCounterValue;
        private object _fpslock = new object();
        private Stopwatch _stopWatch = new Stopwatch();

        public bool HitPointBars { get; set; }

        private bool _cursorIsOn;
        private bool _showDirections = false;
        private bool _showNumberPad = false;
        private bool showNumberPad { get { return _showNumberPad; } set { _showNumberPad = value; } }
        private bool _showWaitIcon = false;
        public bool ShowWaitIcon { get { return _showWaitIcon; } set { _showWaitIcon = value; } }


        private MainPage _mainPage;
        private SKBitmap _logoBitmap;

        public object TargetClipLock = new object();
        public float _originMapOffsetWithNewClipX;
        public float _originMapOffsetWithNewClipY;
        public bool _targetClipOn;
        public long _targetClipStartCounterValue;
        public long _targetClipPanTime;

        private int _clipX;
        private int _clipY;
        public object ClipLock = new object();
        public int ClipX { get { return _clipX; } set { _clipX = value; lock (MapOffsetLock) { _mapOffsetX = 0; } } }
        public int ClipY { get { return _clipY; } set { _clipY = value; lock (MapOffsetLock) { _mapOffsetY = 0; } } }
        public GHMapMode MapMode { get; set; }
        public bool MapNoClipMode { get; set; }
        public bool MapLookMode { get; set; }
        public bool MapTravelMode { get; set; }
        public bool ZoomMiniMode { get; set; }
        public bool ZoomAlternateMode { get; set; }


        private float _mapFontSize = GHConstants.MapFontDefaultSize;
        private float _mapFontAlternateSize = GHConstants.MapFontDefaultAlternateSize;
        private object _mapFontSizeLock = new object();
        public float MapFontSize { get { lock (_mapFontSizeLock) { return _mapFontSize; } } set { lock (_mapFontSizeLock) { _mapFontSize = value; } } }
        public float MapFontAlternateSize { get { lock (_mapFontSizeLock) { return _mapFontAlternateSize; } } set { lock (_mapFontSizeLock) { _mapFontAlternateSize = value; } } }

        private float _tileWidth;
        private float _tileHeight;
        private float _mapWidth;
        private float _mapHeight;
        private float _mapFontAscent;
        public object AnimationTimerLock = new object();
        public GHAnimationTimerList AnimationTimers = new GHAnimationTimerList();

        public object Glyph2TileLock = new object();
        public int[] Glyph2Tile { get; set; }
        public byte[] GlyphTileFlags { get; set; }
        public short[] Tile2Animation { get; set; }
        public short[] Tile2Enlargement { get; set; }
        public short[] Tile2Autodraw { get; set; }
        public int[] AnimationOffsets { get; set; }
        public int[] EnlargementOffsets { get; set; }
        public int[] ReplacementOffsets { get; set; }
        public int Glyph2TileSize { get; set; }
        private SKBitmap[] _tileMap = new SKBitmap[GHConstants.MaxTileSheets];
        public SKBitmap[] TileMap { get { return _tileMap; } }
        public int UsedTileSheets { get; set; }
        public int TotalTiles { get; set; }
        public int UnexploredGlyph { get; set; }
        public int NoGlyph { get; set; }
        public int AnimationOff { get; set; }
        public int EnlargementOff { get; set; }
        public int ReplacementOff { get; set; }
        public int GeneralTileOff { get; set; }
        public int HitTileOff { get; set; }
        public int UITileOff { get; set; }
        public int BuffTileOff { get; set; }
        public int CursorOff { get; set; }


        private int[] _tilesPerRow = new int[GHConstants.MaxTileSheets];
        public int[] TilesPerRow { get { return _tilesPerRow; } }

        private List<AnimationDefinition> _animationDefs = null;
        private List<EnlargementDefinition> _enlargementDefs = null;
        private List<ReplacementDefinition> _replacementDefs = null;
        private List<AutoDrawDefinition> _autodraws = null;

        public List<AnimationDefinition> Animations { get { return _animationDefs; } }
        public List<EnlargementDefinition> Enlargements { get { return _enlargementDefs; } }
        public List<ReplacementDefinition> Replacements { get { return _replacementDefs; } }
        public List<AutoDrawDefinition> Autodraws { get { return _autodraws; } }

        public object _floatingTextLock = new object();
        public List<GHFloatingText> _floatingTexts = new List<GHFloatingText>();
        public object _screenTextLock = new object();
        public GHScreenText _screenText = null;

        public bool EnableWizardMode { get; set; }

        private List<AddContextMenuData> _contextMenuData = new List<AddContextMenuData>();

        private object _mapBitmapLock = new object();
        private SKBitmap _mapBitmap = null;

        public GamePage(MainPage mainPage)
        {
            InitializeComponent();
            _mainPage = mainPage;

            CursorStyle = (TTYCursorStyle)Preferences.Get("CursorStyle", 1);
            GraphicsStyle = (GHGraphicsStyle)Preferences.Get("GraphicsStyle", 1);
            ShowFPS = Preferences.Get("ShowFPS", false);
            HitPointBars = Preferences.Get("HitPointBars", false);
            NumDisplayedMessages = Preferences.Get("NumDisplayedMessages", GHConstants.DefaultMessageRows);
            MapFontSize = Preferences.Get("MapFontSize", GHConstants.MapFontDefaultSize);
            MapFontAlternateSize = Preferences.Get("MapFontAlternateSize", GHConstants.MapFontDefaultAlternateSize);

            ToggleModeButton_Clicked(null, null);
            ZoomMiniMode = true;
            ZoomAlternateMode = true;
            ToggleZoomMiniButton_Clicked(null, null);
            ToggleZoomAlternateButton_Clicked(null, null);
        }

        public async void StartGame()
        {
            //Authenticated
            //App.FmodService.PlayTestSound();

            Assembly assembly = GetType().GetTypeInfo().Assembly;
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.gnollhack_64x96_transparent_32bits.png"))
            {
                _tileMap[0] = SKBitmap.Decode(stream);
            }

            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.gnollhack_64x96_transparent_32bits-2.png"))
            {
                _tileMap[1] = SKBitmap.Decode(stream);
            }
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.gnollhack-logo-test-2.png"))
            {
                _logoBitmap = SKBitmap.Decode(stream);
            }
            InventoryImg.Source = ImageSource.FromResource("GnollHackClient.Assets.Icons.inventory.png");
            SearchImg.Source = ImageSource.FromResource("GnollHackClient.Assets.Icons.search.png");
            WaitImg.Source = ImageSource.FromResource("GnollHackClient.Assets.Icons.wait.png");
            DropManyImg.Source = ImageSource.FromResource("GnollHackClient.Assets.Icons.dropmany.png");
            SkillImg.Source = ImageSource.FromResource("GnollHackClient.Assets.Icons.skill.png");

            _gnollHackService = DependencyService.Get<IGnollHackService>();
            _gnollHackService.InitializeGnollHack(); /* In case the game data was factory-reset prior to start, otherwise InitializeGame would suffice */
            UnexploredGlyph = _gnollHackService.GetUnexploredGlyph();
            NoGlyph = _gnollHackService.GetNoGlyph();

            int animoff, enloff, reoff, general_tile_off, hit_tile_off, ui_tile_off, buff_tile_off, cursor_off;
            _gnollHackService.GetOffs(out animoff, out enloff, out reoff, out general_tile_off, out hit_tile_off, out ui_tile_off, out buff_tile_off,
                out cursor_off);
            AnimationOff = animoff;
            EnlargementOff = enloff;
            ReplacementOff = reoff;
            GeneralTileOff = general_tile_off;
            HitTileOff = hit_tile_off;
            UITileOff = ui_tile_off;
            BuffTileOff = buff_tile_off;
            CursorOff = cursor_off;

            _animationDefs = _gnollHackService.GetAnimationArray();
            _enlargementDefs = _gnollHackService.GetEnlargementArray();
            _replacementDefs = _gnollHackService.GetReplacementArray();
            _autodraws = _gnollHackService.GetAutoDrawArray();

            for (int i = 0; i < GHConstants.MapCols; i++)
            {
                for (int j = 0; j < GHConstants.MapRows; j++)
                {
                    _mapData[i, j] = new MapData();
                    _mapData[i, j].Glyph = UnexploredGlyph;
                    _mapData[i, j].BkGlyph = NoGlyph;
                    _mapData[i, j].NeedsUpdate = true;

                    _objectData[i, j] = new ObjectData();
                }
            }

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

            _stopWatch.Start();
            lock (AnimationTimerLock)
            {
                _previousGeneralCounterValue = AnimationTimers.general_animation_counter;
            }

            //lock(_mapBitmapLock)
            //{
            //    _mapBitmap = new SKBitmap(GHConstants.MapCols * GHConstants.TileWidth, GHConstants.MapRows * GHConstants.TileHeight, SKImageInfo.PlatformColorType, SKAlphaType.Premul);
            //}

            canvasView._gamePage = this;
            uint timeToAnimate = 2000;
            Xamarin.Forms.Animation animation = new Animation(v => canvasView.GeneralAnimationCounter = (long)v, 1, 80);
            animation.Commit(canvasView, "GeneralAnimationCounter", length: timeToAnimate, rate: 25, repeat: () => true);

            Device.StartTimer(TimeSpan.FromSeconds(1f / 40), () =>
            {
                if (_useMapBitmap)
                    UpdateMap();
                //canvasView.InvalidateSurface();
                pollRequestQueue();

                /* Increment counters */
                //lock (AnimationTimerLock)
                //{
                //    AnimationTimers.general_animation_counter++;
                //    if (AnimationTimers.general_animation_counter < 0)
                //        AnimationTimers.general_animation_counter = 0;

                //    if (AnimationTimers.u_action_animation_counter_on)
                //    {
                //        AnimationTimers.u_action_animation_counter++;
                //        if (AnimationTimers.u_action_animation_counter < 0)
                //            AnimationTimers.u_action_animation_counter = 0;
                //    }

                //    if (AnimationTimers.m_action_animation_counter_on)
                //    {
                //        AnimationTimers.m_action_animation_counter++;
                //        if (AnimationTimers.m_action_animation_counter < 0)
                //            AnimationTimers.m_action_animation_counter = 0;
                //    }

                //    if (AnimationTimers.explosion_animation_counter_on)
                //    {
                //        AnimationTimers.explosion_animation_counter++;
                //        if (AnimationTimers.explosion_animation_counter < 0)
                //            AnimationTimers.explosion_animation_counter = 0;
                //    }

                //    int i;
                //    for (i = 0; i < GHConstants.MaxPlayedZapAnimations; i++)
                //    {
                //        if (AnimationTimers.zap_animation_counter_on[i])
                //        {
                //            AnimationTimers.zap_animation_counter[i]++;
                //            if (AnimationTimers.zap_animation_counter[i] < 0)
                //                AnimationTimers.zap_animation_counter[i] = 0;
                //        }
                //    }

                //    for (i = 0; i < GHConstants.MaxPlayedSpecialEffects; i++)
                //    {
                //        if (AnimationTimers.special_effect_animation_counter_on[i])
                //        {
                //            AnimationTimers.special_effect_animation_counter[i]++;
                //            if (AnimationTimers.special_effect_animation_counter[i] < 0)
                //                AnimationTimers.special_effect_animation_counter[i] = 0;
                //        }
                //    }

                //    lock (_floatingTextLock)
                //    {
                //        for (i = _floatingTexts.Count - 1; i >= 0; i--)
                //        {
                //            if (_floatingTexts[i].IsFinished(AnimationTimers.general_animation_counter))
                //                _floatingTexts.RemoveAt(i);
                //        }
                //    }

                //    lock (_screenTextLock)
                //    {
                //        if (_screenText != null && _screenText.IsFinished(AnimationTimers.general_animation_counter))
                //            _screenText = null;
                //    }

                //    lock (TargetClipLock)
                //    {
                //        if (AnimationTimers.general_animation_counter < _targetClipStartCounterValue
                //            || AnimationTimers.general_animation_counter > _targetClipStartCounterValue + _targetClipPanTime)
                //            _targetClipOn = false;

                //        if (_targetClipOn)
                //        {
                //            lock (MapOffsetLock)
                //            {
                //                _mapOffsetX = _originMapOffsetWithNewClipX * Math.Max(0.0f, 1.0f - (float)(AnimationTimers.general_animation_counter - _targetClipStartCounterValue) / (float)_targetClipPanTime);
                //                _mapOffsetY = _originMapOffsetWithNewClipY * Math.Max(0.0f, 1.0f - (float)(AnimationTimers.general_animation_counter - _targetClipStartCounterValue) / (float)_targetClipPanTime);
                //            }
                //        }
                //    }
                //}

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


        }

        public void HideLoadingScreen()
        {
            LoadingLabel.IsVisible = false;
            MainGrid.IsVisible = true;
        }

        public void ClearContextMenu()
        {
            ContextGrid.Children.Clear();
            _contextMenuData.Clear();
            ContextGrid.IsVisible = false;
        }
        public void AddContextMenu(AddContextMenuData data)
        {
            _contextMenuData.Add(data);
            string icon_string = "";
            int LastPicked = GHUtils.Meta('<');
            if (data.cmd_def_char < 0)
                LastPicked -= 256;

            switch ((char)data.cmd_def_char)
            {
                case 'e':
                    icon_string = "GnollHackClient.Assets.Icons.eat.png";
                    break;
                case 'l':
                    icon_string = "GnollHackClient.Assets.Icons.loot.png";
                    break;
                case 'p':
                    icon_string = "GnollHackClient.Assets.Icons.pay.png";
                    break;
                case ',':
                    icon_string = "GnollHackClient.Assets.Icons.pickup.png";
                    break;
                default:
                    if (data.cmd_def_char == LastPicked)
                        icon_string = "GnollHackClient.Assets.Icons.inventory.png";
                    break;
            }

            double sidesize = Math.Min(Math.Min(80.0, Math.Max(50.0, _currentPageWidth / 7)), Math.Min(80.0, Math.Max(50.0, _currentPageHeight / 7)));
            double fontsize = 9.5 * sidesize / 50.0;
            if (icon_string != "")
            {
                CachedImage img = new CachedImage();
                img.Source = ImageSource.FromResource(icon_string);
                img.BackgroundColor = Color.Transparent;
                img.HeightRequest = sidesize;
                img.WidthRequest = sidesize;
                img.IsVisible = true;
                img.InputTransparent = true;
                img.VerticalOptions = LayoutOptions.Start;
                img.HorizontalOptions = LayoutOptions.Center;

                Label lbl = new Label();
                lbl.Text = data.cmd_text;
                lbl.TextColor = Color.White;
                lbl.FontFamily = "LatoRegular";
                lbl.VerticalOptions = LayoutOptions.End;
                lbl.VerticalTextAlignment = TextAlignment.End;
                lbl.HorizontalOptions = LayoutOptions.Center;
                lbl.HorizontalTextAlignment = TextAlignment.Center;
                lbl.FontSize = fontsize;
                lbl.BackgroundColor = Color.Transparent;
                lbl.WidthRequest = sidesize;
                lbl.IsVisible = true;
                lbl.InputTransparent = true;

                Button contextButton = new Button();
                contextButton.Text = "";
                contextButton.HeightRequest = sidesize;
                contextButton.WidthRequest = sidesize;
                contextButton.IsVisible = true;
                contextButton.BackgroundColor = Color.Transparent;
                contextButton.HorizontalOptions = LayoutOptions.CenterAndExpand;
                contextButton.VerticalOptions = LayoutOptions.CenterAndExpand;
                contextButton.Clicked += ContextButton_Clicked;

                Grid addgrid = new Grid();
                addgrid.HeightRequest = sidesize + fontsize + 2;
                addgrid.WidthRequest = sidesize;
                addgrid.IsVisible = true;
                addgrid.Children.Add(img);
                addgrid.Children.Add(lbl);
                addgrid.Children.Add(contextButton);

                //int row = ContextGrid.RowDefinitions.Count - 1 - ContextGrid.Children.Count;
                //if (row < 0)
                //    row = 0;
                //Grid.SetRow(contextButton, row);
                ContextGrid.IsVisible = true;
                ContextGrid.Children.Add(addgrid);
            }
            else
            {
                Button contextButton = new Button();
                string str;
                if (data.cmd_def_char >= 32 && data.cmd_def_char < 128)
                    str = ((char)data.cmd_def_char).ToString();
                else if (data.cmd_def_char >= 128)
                    str = "M" + ((char)(data.cmd_def_char - 128)).ToString();
                else if (data.cmd_def_char < 0)
                    str = "M" + ((char)(data.cmd_def_char + 128)).ToString();
                else
                    str = "C" + ((char)(data.cmd_def_char + 64)).ToString();

                contextButton.Text = str;
                contextButton.TextColor = Color.White;
                contextButton.FontFamily = "Immortal";
                contextButton.CornerRadius = 10;
                contextButton.FontSize = 18;
                contextButton.BackgroundColor = Color.DarkBlue;
                contextButton.HeightRequest = sidesize;
                contextButton.WidthRequest = Math.Max(75, sidesize);
                contextButton.IsVisible = true;

                contextButton.Clicked += ContextButton_Clicked;
                //int row = ContextGrid.RowDefinitions.Count - 1 - ContextGrid.Children.Count;
                //if (row < 0)
                //    row = 0;
                //Grid.SetRow(contextButton, row);
                ContextGrid.IsVisible = true;
                ContextGrid.Children.Add(contextButton);
            }
        }

        public void DisplayFloatingText(DisplayFloatingTextData data)
        {
            lock(_floatingTextLock)
            {
                lock (AnimationTimerLock)
                {
                    _floatingTexts.Add(new GHFloatingText(data, AnimationTimers.general_animation_counter));
                }
            }
        }

        public void DisplayScreenText(DisplayScreenTextData data)
        {
            lock (_screenTextLock)
            {
                lock (AnimationTimerLock)
                {
                    _screenText = new GHScreenText(data, AnimationTimers.general_animation_counter);
                }
            }
        }

        private void ContextButton_Clicked(object sender, EventArgs e)
        {
            int idx = 0;
            idx = ContextGrid.Children.IndexOf((Xamarin.Forms.View)sender);
            if(idx < 0)
                idx = ContextGrid.Children.IndexOf((Xamarin.Forms.View)((Xamarin.Forms.View)sender).Parent);

            if (idx >= 0 && idx < _contextMenuData.Count)
            {
                int resp = _contextMenuData[idx].cmd_cur_char;
                GenericButton_Clicked(sender, e, resp);
            }
        }

        public int TileSheetIdx(int ntile)
        {
            return (Math.Min(UsedTileSheets - 1, Math.Max(0, (ntile / GHConstants.NumberOfTilesPerSheet))));
        }

        public int TileSheetX(int ntile)
        {
            return (((ntile % GHConstants.NumberOfTilesPerSheet) % _tilesPerRow[TileSheetIdx(ntile)]) * GHConstants.TileWidth);
        }
        public int TileSheetY(int ntile)
        {
            return (((ntile % GHConstants.NumberOfTilesPerSheet) / _tilesPerRow[TileSheetIdx(ntile)]) * GHConstants.TileHeight);
        }

        private /*async*/ void ContentPage_Appearing(object sender, EventArgs e)
        {
            App.BackButtonPressed += BackButtonPressed;
            lock(RefreshScreenLock)
            {
                RefreshScreen = true;
            }

            if(_isFirstAppearance)
            {
                _isFirstAppearance = false;

                //int res = 0; // _gnollHackService.Test1();
            }
        }

        protected void GNHThreadProc()
        {
            _clientGame = new ClientGame(this, EnableWizardMode);
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
                    while (queue.TryDequeue(out req))
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
                                ShowMenuPage(req.RequestMenuInfo != null ? req.RequestMenuInfo : new GHMenuInfo(ghmenu_styles.GHMENU_STYLE_GENERAL), req.RequestingGHWindow);
                                break;
                            case GHRequestType.ShowOutRipPage:
                                ShowOutRipPage(req.RequestOutRipInfo != null ? req.RequestOutRipInfo : new GHOutRipInfo("", 0, "", ""), req.RequestingGHWindow);
                                break;
                            case GHRequestType.CreateWindowView:
                                CreateWindowView(req.RequestInt);
                                break;
                            case GHRequestType.DestroyWindowView:
                                DestroyWindowView(req.RequestInt);
                                break;
                            case GHRequestType.ClearWindowView:
                                ClearWindowView(req.RequestInt);
                                break;
                            case GHRequestType.DisplayWindowView:
                                DisplayWindowView(req.RequestInt, req.RequestPutStrItems);
                                break;
                            case GHRequestType.HideLoadingScreen:
                                HideLoadingScreen();
                                break;
                            case GHRequestType.ClearContextMenu:
                                ClearContextMenu();
                                break;
                            case GHRequestType.AddContextMenu:
                                AddContextMenu(req.ContextMenuData);
                                break;
                            case GHRequestType.DisplayFloatingText:
                                DisplayFloatingText(req.FloatingTextData);
                                break;
                            case GHRequestType.DisplayScreenText:
                                DisplayScreenText(req.ScreenTextData);
                                break;
                            case GHRequestType.ShowSkillButton:
                                SkillGrid.IsVisible = true;
                                break;
                            case GHRequestType.HideSkillButton:
                                SkillGrid.IsVisible = false;
                                break;
                        }
                    }
                }
            }
        }

        private void CreateWindowView(int winid)
        {

        }

        private void DestroyWindowView(int winid)
        {

        }

        private void ClearWindowView(int winid)
        {

        }

        private void DisplayWindowView(int winid, List<GHPutStrItem> strs)
        {
            GHWindow window;
            lock(_clientGame.WindowsLock)
            {
                window = _clientGame.Windows[winid];
            }
            ShowWindowPage(window, strs);
        }
        private async void ShowWindowPage(GHWindow window, List<GHPutStrItem> strs)
        {
            var cpage = new GHTextPage(this, window, strs);
            await App.Current.MainPage.Navigation.PushModalAsync(cpage);
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
            showNumberPad = false;
        }
        private void HideDirections()
        {
            _showDirections = false;
            showNumberPad = false;
        }
        public void ShowNumberPad()
        {
            if(!_showDirections)
                showNumberPad = true;
        }

        private object msgHistoryLock = new object();
        private List<GHMsgHistoryItem> _msgHistory = null;
        private void PrintHistory(List<GHMsgHistoryItem> msgHistory)
        {
            lock(msgHistoryLock)
            {
                _msgHistory = msgHistory;
            }
            /*
            messagehistoryview.itemssource = msghistory;
            if (msghistory.count > 0)
                messagehistoryview.scrollto(msghistory.count - 1, -1, scrolltoposition.end, false);
            */
        }

        private async void AskName()
        {
            var namePage = new NamePage(this);
            await App.Current.MainPage.Navigation.PushModalAsync(namePage);
        }
        private void GetLine(string query)
        {
            GetLineCaption.Text = query;
            GetLineEntryText.Text = "";
            GetLineGrid.IsVisible = true;
            //var getLinePage = new GetLinePage(this, query);
            //await App.Current.MainPage.Navigation.PushModalAsync(getLinePage);
        }

        private void GetLineOkButton_Clicked(object sender, EventArgs e)
        {
            string res = GetLineEntryText.Text;
            if (string.IsNullOrWhiteSpace(GetLineEntryText.Text))
            {
                res = "";
            }

            ConcurrentQueue<GHResponse> queue;
            if (ClientGame.ResponseDictionary.TryGetValue(_clientGame, out queue))
            {
                queue.Enqueue(new GHResponse(_clientGame, GHRequestType.GetLine, res));
            }

            GetLineGrid.IsVisible = false;
            GetLineEntryText.Text = "";
            GetLineCaption.Text = "";
        }

        private void GetLineCancelButton_Clicked(object sender, EventArgs e)
        {
            ConcurrentQueue<GHResponse> queue;
            if (ClientGame.ResponseDictionary.TryGetValue(_clientGame, out queue))
            {
                queue.Enqueue(new GHResponse(_clientGame, GHRequestType.GetLine, '\x1B'.ToString()));
            }

            GetLineGrid.IsVisible = false;
            GetLineEntryText.Text = "";
            GetLineCaption.Text = "";
        }

        private void GetChar()
        {
            // Set focus to GameViewPage
        }
        private async void ReturnToMainMenu()
        {
            if(!App.IsServerGame)
                _mainPage.HideLocalGameButton();
    
            await App.Current.MainPage.Navigation.PopModalAsync();
            if (App.IsServerGame)
            {
                await App.Current.MainPage.Navigation.PopAsync(); //Login
            }
        }
        private async void ShowMenuPage(GHMenuInfo menuinfo, GHWindow ghwindow)
        {
            ShowWaitIcon = true;
            var menuPage = new GHMenuPage(this, ghwindow, menuinfo.Style);
            menuPage.SelectionHow = menuinfo.SelectionHow;
            if (menuinfo.Header == null)
                menuPage.HeaderVisible = false;
            else
                menuPage.Header = menuinfo.Header;

            if (menuinfo.Subtitle == null)
                menuPage.SubtitleVisible = false;
            else
                menuPage.Subtitle = menuinfo.Subtitle;

            if (menuinfo != null)
            {
                foreach(GHMenuItem mi in menuinfo.MenuItems)
                    menuPage.MenuItems.Add(mi);
            }
            menuPage.Process();
            await App.Current.MainPage.Navigation.PushModalAsync(menuPage, false);
        }
        private async void ShowOutRipPage(GHOutRipInfo outripinfo, GHWindow ghwindow)
        {
            var outRipPage = new OutRipPage(this, ghwindow, outripinfo);
            await App.Current.MainPage.Navigation.PushModalAsync(outRipPage);
        }

        private async Task<bool> BackButtonPressed(object sender, EventArgs e)
        {
            var menu = new GameMenuPage(this);
            await App.Current.MainPage.Navigation.PushModalAsync(menu);

            return false;
        }
        public async void ShowGameMenu(object sender, EventArgs e)
        {
            var menu = new GameMenuPage(this);
            await App.Current.MainPage.Navigation.PushModalAsync(menu);
        }

        private void ContentPage_Disappearing(object sender, EventArgs e)
        {
            App.BackButtonPressed -= BackButtonPressed;
            lock (RefreshScreenLock)
            {
                RefreshScreen = false;
            }

            Preferences.Set("MapFontSize", Math.Max(GHConstants.MinimumMapFontSize, MapFontSize));
            Preferences.Set("MapFontAlternateSize", Math.Max(GHConstants.MinimumMapFontSize, MapFontAlternateSize));
        }


        private List<string> wrappedLines = new List<string>();
        private string longLine;
        private string wordWithSpace;

        private void UpdateMap()
        {
            //int startX = 1, endX = GHConstants.MapCols - 1;
            //int startY = 0, endY = GHConstants.MapRows - 1;
            //string str;
            //float tx, ty;

            //using (SKPaint textPaint = new SKPaint())
            //{
            //    textPaint.FilterQuality = SKFilterQuality.None;

            //    /* Map */
            //    textPaint.Typeface = App.DejaVuSansMonoTypeface;
            //    textPaint.TextSize = 48;
            //    float width = (float)GHConstants.TileWidth;
            //    float height = (float)GHConstants.TileHeight;
            //    float stdMapFontAscent = textPaint.FontMetrics.Ascent;

            //    lock (_mapBitmapLock)
            //    {
            //        if (_mapBitmap == null)
            //            return;
            //        using (SKCanvas canvas = new SKCanvas(_mapBitmap))
            //        {
            //            lock (Glyph2TileLock)
            //            {
            //                lock (_mapDataLock)
            //                {
            //                    if (GraphicsStyle == GHGraphicsStyle.ASCII)
            //                    {
            //                        for (int mapx = startX; mapx <= endX; mapx++)
            //                        {
            //                            for (int mapy = startY; mapy <= endY; mapy++)
            //                            {
            //                                if (_mapData[mapx, mapy].Symbol != null && _mapData[mapx, mapy].Symbol != "")
            //                                {
            //                                    str = _mapData[mapx, mapy].Symbol;
            //                                    textPaint.Color = _mapData[mapx, mapy].Color;
            //                                    tx = (width * (float)mapx);
            //                                    ty = (height * (float)mapy);
            //                                    if (CursorStyle == TTYCursorStyle.GreenBlock && _mapCursorX == mapx && _mapCursorY == mapy)
            //                                    {
            //                                        textPaint.Style = SKPaintStyle.Fill;
            //                                        textPaint.Color = _cursorDefaultGreen;
            //                                        SKRect winRect = new SKRect(tx, ty, tx + width, ty + height);
            //                                        canvas.DrawRect(winRect, textPaint);
            //                                        textPaint.Color = SKColors.Black;
            //                                    }
            //                                    else if ((_mapData[mapx, mapy].Special & ((uint)MapSpecial.Pet | (uint)MapSpecial.ObjectPile)) != 0)
            //                                    {
            //                                        textPaint.Style = SKPaintStyle.Fill;
            //                                        SKRect winRect = new SKRect(tx, ty, tx + width, ty + height);
            //                                        canvas.DrawRect(winRect, textPaint);
            //                                        textPaint.Color = SKColors.Black;
            //                                    }

            //                                    canvas.DrawText(str, tx, ty - stdMapFontAscent, textPaint);

            //                                    if ((_mapData[mapx, mapy].Special & (uint)MapSpecial.Peaceful) != 0)
            //                                    {
            //                                        canvas.DrawText("_", tx, ty - stdMapFontAscent, textPaint);
            //                                    }
            //                                }
            //                            }
            //                        }
            //                    }
            //                    else
            //                    {
            //                        if (Glyph2Tile != null && _tilesPerRow[0] > 0 && UsedTileSheets > 0)
            //                        {
            //                            /* Update */
            //                            for (int mapx = startX; mapx <= endX; mapx++)
            //                            {
            //                                for (int mapy = startY; mapy <= endY; mapy++)
            //                                {
            //                                    if (_mapData[mapx, mapy].NeedsUpdate || _mapData[mapx, mapy].MapAnimated)
            //                                    {
            //                                        for (int i = -2; i <= 2; i++)
            //                                        {
            //                                            for (int j = -3; j <= 1; j++)
            //                                            {
            //                                                if (GHUtils.isok(mapx + i, mapy + j))
            //                                                {
            //                                                    _mapData[mapx + i, mapy + j].RedrawTile = true;
            //                                                }
            //                                            }
            //                                        }
            //                                    }
            //                                }
            //                            }

            //                            using (SKPaint paint = new SKPaint())
            //                            {
            //                                paint.FilterQuality = SKFilterQuality.None;

            //                                float pit_border = (float)GHConstants.PIT_BOTTOM_BORDER * height / (float)GHConstants.TileHeight;
            //                                for (int layer_idx = 0; layer_idx <= (int)layer_types.MAX_LAYERS; layer_idx++)
            //                                {
            //                                    for (int mapy = startY; mapy <= endY; mapy++)
            //                                    {
            //                                        for (int mapx = startX; mapx <= endX; mapx++)
            //                                        {
            //                                            if (_mapData[mapx, mapy].RedrawTile == false)
            //                                                continue;
            //                                            else if (layer_idx == 0)
            //                                            {
            //                                                paint.Color = SKColors.Transparent;
            //                                                paint.Style = SKPaintStyle.Fill;
            //                                                canvas.DrawRect((float)mapx * width, (float)mapy * height, (float)(mapx + 1) * width, (float)(mapy + 1) * height, paint);
            //                                            }

            //                                            short monster_height = _mapData[mapx, mapy].Layers.special_monster_layer_height;
            //                                            float scaled_y_height_change = 0;
            //                                            sbyte monster_origin_x = _mapData[mapx, mapy].Layers.monster_origin_x;
            //                                            sbyte monster_origin_y = _mapData[mapx, mapy].Layers.monster_origin_y;
            //                                            long currentcountervalue = 0;
            //                                            lock (AnimationTimerLock)
            //                                            {
            //                                                currentcountervalue = AnimationTimers.general_animation_counter;
            //                                            }
            //                                            long glyphprintcountervalue = _mapData[mapx, mapy].GlyphPrintCounterValue;
            //                                            float base_move_offset_x = 0, base_move_offset_y = 0;
            //                                            int movediffx = (int)monster_origin_x - mapx;
            //                                            int movediffy = (int)monster_origin_y - mapy;
            //                                            long counterdiff = currentcountervalue - glyphprintcountervalue;
            //                                            if (GHUtils.isok(monster_origin_x, monster_origin_y)
            //                                                && (movediffx != 0 || movediffy != 0)
            //                                                && counterdiff >= 0 && counterdiff < GHConstants.MoveIntervals)
            //                                            {
            //                                                base_move_offset_x = width * (float)movediffx * (float)(GHConstants.MoveIntervals - counterdiff) / (float)GHConstants.MoveIntervals;
            //                                                base_move_offset_y = height * (float)movediffy * (float)(GHConstants.MoveIntervals - counterdiff) / (float)GHConstants.MoveIntervals;
            //                                            }

            //                                            if (layer_idx == (int)layer_types.MAX_LAYERS)
            //                                            {
            //                                                if (monster_height > 0)
            //                                                    scaled_y_height_change = (float)-monster_height * height / (float)GHConstants.TileHeight;

            //                                                if ((_mapData[mapx, mapy].Layers.layer_flags & (ulong)(LayerFlags.LFLAGS_M_YOU | LayerFlags.LFLAGS_UXUY | LayerFlags.LFLAGS_M_CANSPOTMON)) != 0
            //                                                    && (_mapData[mapx, mapy].Layers.layer_flags & (ulong)(LayerFlags.LFLAGS_M_WORM_TAIL)) == 0)
            //                                                {
            //                                                    tx = (base_move_offset_x + width * (float)mapx);
            //                                                    ty = (base_move_offset_y + scaled_y_height_change + height * (float)mapy);

            //                                                    /* Draw condition and status marks */
            //                                                    float x_scaling_factor = width / (float)(GHConstants.TileWidth);
            //                                                    float y_scaling_factor = height / (float)(GHConstants.TileHeight);
            //                                                    int max_fitted_rows = (GHConstants.TileHeight - 4) / (GHConstants.StatusMarkHeight + 2);
            //                                                    int status_count = 0;

            //                                                    ulong status_bits = _mapData[mapx, mapy].Layers.status_bits;
            //                                                    if (status_bits != 0)
            //                                                    {
            //                                                        int[] statusmarkorder = { (int)game_ui_status_mark_types.STATUS_MARK_TOWNGUARD_PEACEFUL, (int)game_ui_status_mark_types.STATUS_MARK_TOWNGUARD_HOSTILE, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
            //                                                        int tiles_per_row = GHConstants.TileWidth / GHConstants.StatusMarkWidth;
            //                                                        int mglyph = (int)game_ui_tile_types.STATUS_MARKS + UITileOff;
            //                                                        int mtile = Glyph2Tile[mglyph];
            //                                                        int sheet_idx = TileSheetIdx(mtile);
            //                                                        int tile_x = TileSheetX(mtile);
            //                                                        int tile_y = TileSheetY(mtile);
            //                                                        foreach (int status_mark in statusmarkorder)
            //                                                        {
            //                                                            if (status_count >= max_fitted_rows)
            //                                                                break;

            //                                                            ulong statusbit = 1UL << status_mark;
            //                                                            if ((status_bits & statusbit) != 0)
            //                                                            {
            //                                                                int within_tile_x = status_mark % tiles_per_row;
            //                                                                int within_tile_y = status_mark / tiles_per_row;
            //                                                                int c_x = tile_x + within_tile_x * GHConstants.StatusMarkWidth;
            //                                                                int c_y = tile_y + within_tile_y * GHConstants.StatusMarkHeight;

            //                                                                SKRect source_rt = new SKRect();
            //                                                                source_rt.Left = c_x;
            //                                                                source_rt.Right = c_x + GHConstants.StatusMarkWidth;
            //                                                                source_rt.Top = c_y;
            //                                                                source_rt.Bottom = c_y + GHConstants.StatusMarkHeight;

            //                                                                /* Define draw location in target */
            //                                                                int unscaled_left = GHConstants.TileWidth - 2 - GHConstants.StatusMarkWidth;
            //                                                                int unscaled_right = unscaled_left + GHConstants.StatusMarkWidth;
            //                                                                int unscaled_top = 2 + (2 + GHConstants.StatusMarkWidth) * status_count;
            //                                                                int unscaled_bottom = unscaled_top + GHConstants.StatusMarkHeight;

            //                                                                SKRect target_rt = new SKRect();
            //                                                                target_rt.Left = tx + (int)(x_scaling_factor * (double)unscaled_left);
            //                                                                target_rt.Right = tx + (int)(x_scaling_factor * (double)unscaled_right);
            //                                                                target_rt.Top = ty + (int)(y_scaling_factor * (double)unscaled_top);
            //                                                                target_rt.Bottom = ty + (int)(y_scaling_factor * (double)unscaled_bottom);

            //                                                                canvas.DrawBitmap(TileMap[sheet_idx], source_rt, target_rt);

            //                                                                status_count++;
            //                                                            }
            //                                                        }
            //                                                    }

            //                                                    ulong condition_bits = _mapData[mapx, mapy].Layers.condition_bits;
            //                                                    if (condition_bits != 0)
            //                                                    {
            //                                                        int tiles_per_row = GHConstants.TileWidth / GHConstants.StatusMarkWidth;
            //                                                        int mglyph = (int)game_ui_tile_types.CONDITION_MARKS + UITileOff;
            //                                                        int mtile = Glyph2Tile[mglyph];
            //                                                        int sheet_idx = TileSheetIdx(mtile);
            //                                                        int tile_x = TileSheetX(mtile);
            //                                                        int tile_y = TileSheetY(mtile);
            //                                                        for (int condition_mark = 0; condition_mark < (int)bl_conditions.NUM_BL_CONDITIONS; condition_mark++)
            //                                                        {
            //                                                            ulong conditionbit = 1UL << condition_mark;
            //                                                            if ((condition_bits & conditionbit) != 0)
            //                                                            {
            //                                                                int within_tile_x = condition_mark % tiles_per_row;
            //                                                                int within_tile_y = condition_mark / tiles_per_row;
            //                                                                int c_x = tile_x + within_tile_x * GHConstants.StatusMarkWidth;
            //                                                                int c_y = tile_y + within_tile_y * GHConstants.StatusMarkHeight;

            //                                                                SKRect source_rt = new SKRect();
            //                                                                source_rt.Left = c_x;
            //                                                                source_rt.Right = c_x + GHConstants.StatusMarkWidth;
            //                                                                source_rt.Top = c_y;
            //                                                                source_rt.Bottom = c_y + GHConstants.StatusMarkHeight;

            //                                                                /* Define draw location in target */
            //                                                                int unscaled_left = GHConstants.TileWidth - 2 - GHConstants.StatusMarkWidth;
            //                                                                int unscaled_right = unscaled_left + GHConstants.StatusMarkWidth;
            //                                                                int unscaled_top = 2 + (2 + GHConstants.StatusMarkWidth) * status_count;
            //                                                                int unscaled_bottom = unscaled_top + GHConstants.StatusMarkHeight;

            //                                                                SKRect target_rt = new SKRect();
            //                                                                target_rt.Left = tx + (int)(x_scaling_factor * (double)unscaled_left);
            //                                                                target_rt.Right = tx + (int)(x_scaling_factor * (double)unscaled_right);
            //                                                                target_rt.Top = ty + (int)(y_scaling_factor * (double)unscaled_top);
            //                                                                target_rt.Bottom = ty + (int)(y_scaling_factor * (double)unscaled_bottom);

            //                                                                if (status_count >= max_fitted_rows)
            //                                                                    break;

            //                                                                canvas.DrawBitmap(TileMap[sheet_idx], source_rt, target_rt);

            //                                                                status_count++;
            //                                                            }
            //                                                        }
            //                                                    }

            //                                                    for (int buff_ulong = 0; buff_ulong < GHConstants.NUM_BUFF_BIT_ULONGS; buff_ulong++)
            //                                                    {
            //                                                        if (status_count >= max_fitted_rows)
            //                                                            break;

            //                                                        ulong buff_bits = _mapData[mapx, mapy].Layers.buff_bits[buff_ulong];
            //                                                        int tiles_per_row = GHConstants.TileWidth / GHConstants.StatusMarkWidth;
            //                                                        if (buff_bits != 0)
            //                                                        {
            //                                                            for (int buff_idx = 0; buff_idx < 32; buff_idx++)
            //                                                            {
            //                                                                if (status_count >= max_fitted_rows)
            //                                                                    break;

            //                                                                ulong buffbit = 1UL << buff_idx;
            //                                                                if ((buff_bits & buffbit) != 0)
            //                                                                {
            //                                                                    int propidx = buff_ulong * 32 + buff_idx;
            //                                                                    if (propidx > GHConstants.LAST_PROP)
            //                                                                        break;
            //                                                                    int mglyph = (propidx - 1) / GHConstants.BUFFS_PER_TILE + BuffTileOff;
            //                                                                    int mtile = Glyph2Tile[mglyph];
            //                                                                    int sheet_idx = TileSheetIdx(mtile);
            //                                                                    int tile_x = TileSheetX(mtile);
            //                                                                    int tile_y = TileSheetY(mtile);

            //                                                                    int buff_mark = (propidx - 1) % GHConstants.BUFFS_PER_TILE;
            //                                                                    int within_tile_x = buff_mark % tiles_per_row;
            //                                                                    int within_tile_y = buff_mark / tiles_per_row;
            //                                                                    int c_x = tile_x + within_tile_x * GHConstants.StatusMarkWidth;
            //                                                                    int c_y = tile_y + within_tile_y * GHConstants.StatusMarkHeight;

            //                                                                    SKRect source_rt = new SKRect();
            //                                                                    source_rt.Left = c_x;
            //                                                                    source_rt.Right = c_x + GHConstants.StatusMarkWidth;
            //                                                                    source_rt.Top = c_y;
            //                                                                    source_rt.Bottom = c_y + GHConstants.StatusMarkHeight;

            //                                                                    /* Define draw location in target */
            //                                                                    int unscaled_left = GHConstants.TileWidth - 2 - GHConstants.StatusMarkWidth;
            //                                                                    int unscaled_right = unscaled_left + GHConstants.StatusMarkWidth;
            //                                                                    int unscaled_top = 2 + (2 + GHConstants.StatusMarkWidth) * status_count;
            //                                                                    int unscaled_bottom = unscaled_top + GHConstants.StatusMarkHeight;

            //                                                                    SKRect target_rt = new SKRect();
            //                                                                    target_rt.Left = tx + (int)(x_scaling_factor * (double)unscaled_left);
            //                                                                    target_rt.Right = tx + (int)(x_scaling_factor * (double)unscaled_right);
            //                                                                    target_rt.Top = ty + (int)(y_scaling_factor * (double)unscaled_top);
            //                                                                    target_rt.Bottom = ty + (int)(y_scaling_factor * (double)unscaled_bottom);

            //                                                                    canvas.DrawBitmap(TileMap[sheet_idx], source_rt, target_rt);

            //                                                                    status_count++;
            //                                                                }
            //                                                            }
            //                                                        }
            //                                                    }

            //                                                }

            //                                                /* Draw death and hit markers */
            //                                                if ((_mapData[mapx, mapy].Layers.layer_flags & (ulong)LayerFlags.LFLAGS_M_KILLED) != 0)
            //                                                {
            //                                                    int mglyph = (int)general_tile_types.GENERAL_TILE_DEATH + GeneralTileOff;
            //                                                    int mtile = Glyph2Tile[mglyph];
            //                                                    int sheet_idx = TileSheetIdx(mtile);
            //                                                    int tile_x = TileSheetX(mtile);
            //                                                    int tile_y = TileSheetY(mtile);

            //                                                    tx = (base_move_offset_x + width * (float)mapx);
            //                                                    ty = (base_move_offset_y + scaled_y_height_change + height * (float)mapy);
            //                                                    SKRect targetrect = new SKRect(tx, ty, tx + width, ty + height);

            //                                                    SKRect sourcerect = new SKRect(tile_x, tile_y, tile_x + GHConstants.TileWidth, tile_y + GHConstants.TileHeight);
            //                                                    canvas.DrawBitmap(TileMap[sheet_idx], sourcerect, targetrect);
            //                                                }
            //                                                else if ((_mapData[mapx, mapy].Layers.layer_flags & (ulong)LayerFlags.LFLAGS_M_BEING_HIT) != 0)
            //                                                {
            //                                                    short hit_text_num = _mapData[mapx, mapy].Layers.hit_tile;
            //                                                    int mglyph = Math.Max(0, Math.Min((int)hit_tile_types.MAX_HIT_TILES - 1, (int)hit_text_num)) + HitTileOff;
            //                                                    int mtile = Glyph2Tile[mglyph];
            //                                                    int sheet_idx = TileSheetIdx(mtile);
            //                                                    int tile_x = TileSheetX(mtile);
            //                                                    int tile_y = TileSheetY(mtile);

            //                                                    tx = (base_move_offset_x + width * (float)mapx);
            //                                                    ty = (base_move_offset_y + scaled_y_height_change + height * (float)mapy);
            //                                                    SKRect targetrect = new SKRect(tx, ty, tx + width, ty + height);

            //                                                    SKRect sourcerect = new SKRect(tile_x, tile_y, tile_x + GHConstants.TileWidth, tile_y + GHConstants.TileHeight);
            //                                                    canvas.DrawBitmap(TileMap[sheet_idx], sourcerect, targetrect);
            //                                                }

            //                                            }
            //                                            else
            //                                            {
            //                                                int sub_layer_cnt = 1;
            //                                                lock (_objectDataLock)
            //                                                {
            //                                                    if (layer_idx == (int)layer_types.LAYER_OBJECT)
            //                                                    {
            //                                                        if ((_mapData[mapx, mapy].Layers.layer_flags & (ulong)LayerFlags.LFLAGS_SHOWING_MEMORY) != 0)
            //                                                            sub_layer_cnt = _objectData[mapx, mapy].MemoryObjectList == null ? 0 : Math.Min(GHConstants.MaxObjectsDrawn, _objectData[mapx, mapy].MemoryObjectList.Count);
            //                                                        else if ((_mapData[mapx, mapy].Layers.layer_flags & (ulong)LayerFlags.LFLAGS_CAN_SEE) != 0)
            //                                                            sub_layer_cnt = _objectData[mapx, mapy].FloorObjectList == null ? 0 : Math.Min(GHConstants.MaxObjectsDrawn, _objectData[mapx, mapy].FloorObjectList.Count);
            //                                                        else
            //                                                            sub_layer_cnt = 1; /* As a backup, show layer glyph (probably often NoGlyph) */
            //                                                    }
            //                                                    else if (layer_idx == (int)layer_types.LAYER_COVER_OBJECT)
            //                                                    {
            //                                                        if ((_mapData[mapx, mapy].Layers.layer_flags & (ulong)LayerFlags.LFLAGS_SHOWING_MEMORY) != 0)
            //                                                            sub_layer_cnt = _objectData[mapx, mapy].CoverMemoryObjectList == null ? 0 : Math.Min(GHConstants.MaxObjectsDrawn, _objectData[mapx, mapy].CoverMemoryObjectList.Count);
            //                                                        else if ((_mapData[mapx, mapy].Layers.layer_flags & (ulong)LayerFlags.LFLAGS_CAN_SEE) != 0)
            //                                                            sub_layer_cnt = _objectData[mapx, mapy].CoverFloorObjectList == null ? 0 : Math.Min(GHConstants.MaxObjectsDrawn, _objectData[mapx, mapy].CoverFloorObjectList.Count);
            //                                                        else
            //                                                            sub_layer_cnt = 1; /* As a backup, show layer glyph (probably often NoGlyph) */
            //                                                    }
            //                                                    for (int sub_layer_idx = sub_layer_cnt - 1; sub_layer_idx >= 0; sub_layer_idx--)
            //                                                    {

            //                                                        int signed_glyph = NoGlyph;
            //                                                        short obj_height = _mapData[mapx, mapy].Layers.object_height;
            //                                                        if (layer_idx == (int)layer_types.LAYER_OBJECT)
            //                                                        {
            //                                                            if ((_mapData[mapx, mapy].Layers.layer_flags & (ulong)LayerFlags.LFLAGS_SHOWING_MEMORY) != 0)
            //                                                            {
            //                                                                signed_glyph = _objectData[mapx, mapy].MemoryObjectList[sub_layer_idx].ObjData.gui_glyph;
            //                                                                obj_height = _objectData[mapx, mapy].MemoryObjectList[sub_layer_idx].TileHeight;
            //                                                            }
            //                                                            else if ((_mapData[mapx, mapy].Layers.layer_flags & (ulong)LayerFlags.LFLAGS_CAN_SEE) != 0)
            //                                                            {
            //                                                                signed_glyph = _objectData[mapx, mapy].FloorObjectList[sub_layer_idx].ObjData.gui_glyph;
            //                                                                obj_height = _objectData[mapx, mapy].FloorObjectList[sub_layer_idx].TileHeight;
            //                                                            }
            //                                                            else
            //                                                            {
            //                                                                signed_glyph = _mapData[mapx, mapy].Layers.layer_glyphs == null ? NoGlyph : _mapData[mapx, mapy].Layers.layer_glyphs[layer_idx];
            //                                                            }
            //                                                        }
            //                                                        else if (layer_idx == (int)layer_types.LAYER_COVER_OBJECT)
            //                                                        {
            //                                                            if ((_mapData[mapx, mapy].Layers.layer_flags & (ulong)LayerFlags.LFLAGS_SHOWING_MEMORY) != 0)
            //                                                            {
            //                                                                signed_glyph = _objectData[mapx, mapy].CoverMemoryObjectList[sub_layer_idx].ObjData.gui_glyph;
            //                                                                obj_height = _objectData[mapx, mapy].CoverMemoryObjectList[sub_layer_idx].TileHeight;
            //                                                            }
            //                                                            else if ((_mapData[mapx, mapy].Layers.layer_flags & (ulong)LayerFlags.LFLAGS_CAN_SEE) != 0)
            //                                                            {
            //                                                                signed_glyph = _objectData[mapx, mapy].CoverFloorObjectList[sub_layer_idx].ObjData.gui_glyph;
            //                                                                obj_height = _objectData[mapx, mapy].CoverFloorObjectList[sub_layer_idx].TileHeight;
            //                                                            }
            //                                                            else
            //                                                            {
            //                                                                signed_glyph = _mapData[mapx, mapy].Layers.layer_glyphs == null ? NoGlyph : _mapData[mapx, mapy].Layers.layer_glyphs[layer_idx];
            //                                                            }
            //                                                        }
            //                                                        else
            //                                                        {
            //                                                            signed_glyph = _mapData[mapx, mapy].Layers.layer_glyphs == null ? (layer_idx == 0 ? UnexploredGlyph : NoGlyph) : _mapData[mapx, mapy].Layers.layer_glyphs[layer_idx];
            //                                                        }

            //                                                        if (signed_glyph == NoGlyph)
            //                                                            continue;

            //                                                        int glyph = Math.Abs(signed_glyph);
            //                                                        if (glyph == 0 || glyph >= Glyph2Tile.Length)
            //                                                            continue;

            //                                                        short missile_height = _mapData[mapx, mapy].Layers.missile_height;
            //                                                        bool obj_in_pit = (_mapData[mapx, mapy].Layers.layer_flags & (ulong)LayerFlags.LFLAGS_O_IN_PIT) != 0;

            //                                                        /* Base flips */
            //                                                        bool hflip = (signed_glyph < 0);
            //                                                        bool manual_hflip = false;
            //                                                        bool manual_vflip = false;

            //                                                        /* Tile flips */
            //                                                        bool tileflag_hflip = (GlyphTileFlags[glyph] & (byte)glyph_tile_flags.GLYPH_TILE_FLAG_FLIP_HORIZONTALLY) != 0;
            //                                                        bool tileflag_vflip = (GlyphTileFlags[glyph] & (byte)glyph_tile_flags.GLYPH_TILE_FLAG_FLIP_VERTICALLY) != 0;
            //                                                        bool tileflag_halfsize = (GlyphTileFlags[glyph] & (byte)glyph_tile_flags.GLYPH_TILE_FLAG_HALF_SIZED_TILE) != 0;
            //                                                        bool tileflag_floortile = (GlyphTileFlags[glyph] & (byte)glyph_tile_flags.GLYPH_TILE_FLAG_HAS_FLOOR_TILE) != 0;
            //                                                        bool tileflag_normalobjmissile = (GlyphTileFlags[glyph] & (byte)glyph_tile_flags.GLYPH_TILE_FLAG_NORMAL_ITEM_AS_MISSILE) != 0;
            //                                                        bool tileflag_fullsizeditem = (GlyphTileFlags[glyph] & (byte)glyph_tile_flags.GLYPH_TILE_FLAG_FULL_SIZED_ITEM) != 0;

            //                                                        if ((!tileflag_halfsize || monster_height > 0) && (layer_idx == (int)layer_types.LAYER_MONSTER || layer_idx == (int)layer_types.LAYER_MONSTER_EFFECT))
            //                                                            scaled_y_height_change = (float)-monster_height * height / (float)GHConstants.TileHeight;
            //                                                        else if (tileflag_halfsize && (layer_idx == (int)layer_types.LAYER_OBJECT || layer_idx == (int)layer_types.LAYER_COVER_OBJECT))
            //                                                            scaled_y_height_change = (float)(-(sub_layer_cnt - 1 - sub_layer_idx) * GHConstants.OBJECT_PILE_HEIGHT_DIFFERENCE - GHConstants.OBJECT_PILE_START_HEIGHT) * height / (float)GHConstants.TileHeight;

            //                                                        int ntile = Glyph2Tile[glyph];
            //                                                        /* Replace tile here */
            //                                                        int animation = Tile2Animation[ntile];
            //                                                        int autodraw = Tile2Autodraw[ntile];
            //                                                        int anim_frame_idx = 0, main_tile_idx = 0;
            //                                                        sbyte mapAnimated = 0;
            //                                                        int tile_animation_idx = _gnollHackService.GetTileAnimationIndexFromGlyph(glyph);
            //                                                        bool is_dropping_piercer = false;

            //                                                        /* Determine animation tile here */
            //                                                        lock (AnimationTimerLock)
            //                                                        {
            //                                                            if (AnimationTimers.u_action_animation_counter_on && layer_idx == (int)layer_types.LAYER_MONSTER && ((_mapData[mapx, mapy].Layers.layer_flags & (ulong)LayerFlags.LFLAGS_UXUY) != 0))
            //                                                                ntile = _gnollHackService.GetAnimatedTile(ntile, tile_animation_idx, (int)animation_play_types.ANIMATION_PLAY_TYPE_PLAYED_SEPARATELY, AnimationTimers.u_action_animation_counter, out anim_frame_idx, out main_tile_idx, out mapAnimated, out autodraw);
            //                                                            else if (AnimationTimers.m_action_animation_counter_on && ((!is_dropping_piercer && layer_idx == (int)layer_types.LAYER_MONSTER) || (is_dropping_piercer && layer_idx == (int)layer_types.LAYER_MISSILE)) && AnimationTimers.m_action_animation_x == mapx && AnimationTimers.m_action_animation_y == mapy)
            //                                                                ntile = _gnollHackService.GetAnimatedTile(ntile, tile_animation_idx, (int)animation_play_types.ANIMATION_PLAY_TYPE_PLAYED_SEPARATELY, AnimationTimers.m_action_animation_counter, out anim_frame_idx, out main_tile_idx, out mapAnimated, out autodraw);
            //                                                            else if (_gnollHackService.GlyphIsExplosion(glyph))
            //                                                                ntile = _gnollHackService.GetAnimatedTile(ntile, tile_animation_idx, (int)animation_play_types.ANIMATION_PLAY_TYPE_PLAYED_SEPARATELY, AnimationTimers.explosion_animation_counter, out anim_frame_idx, out main_tile_idx, out mapAnimated, out autodraw);
            //                                                            else if (_gnollHackService.GlyphIsZap(glyph))
            //                                                            {
            //                                                                for (int zap_anim_idx = 0; zap_anim_idx < GHConstants.MaxPlayedZapAnimations; zap_anim_idx++)
            //                                                                {
            //                                                                    if (AnimationTimers.zap_animation_counter_on[zap_anim_idx]
            //                                                                        && mapx == AnimationTimers.zap_animation_x[zap_anim_idx]
            //                                                                        && mapy == AnimationTimers.zap_animation_y[zap_anim_idx])
            //                                                                    {
            //                                                                        ntile = _gnollHackService.GetAnimatedTile(ntile, tile_animation_idx, (int)animation_play_types.ANIMATION_PLAY_TYPE_PLAYED_SEPARATELY, AnimationTimers.zap_animation_counter[zap_anim_idx], out anim_frame_idx, out main_tile_idx, out mapAnimated, out autodraw);
            //                                                                        break;
            //                                                                    }
            //                                                                }
            //                                                            }
            //                                                            else
            //                                                            {
            //                                                                /* Check for special effect animations */
            //                                                                bool spef_found = false;
            //                                                                for (int spef_idx = 0; spef_idx < GHConstants.MaxPlayedSpecialEffects; spef_idx++)
            //                                                                {
            //                                                                    if (AnimationTimers.special_effect_animation_counter_on[spef_idx]
            //                                                                        && layer_idx == (int)AnimationTimers.spef_action_animation_layer[spef_idx]
            //                                                                        && mapx == AnimationTimers.spef_action_animation_x[spef_idx]
            //                                                                        && mapy == AnimationTimers.spef_action_animation_y[spef_idx])
            //                                                                    {
            //                                                                        ntile = _gnollHackService.GetAnimatedTile(ntile, tile_animation_idx, (int)animation_play_types.ANIMATION_PLAY_TYPE_PLAYED_SEPARATELY, AnimationTimers.special_effect_animation_counter[spef_idx], out anim_frame_idx, out main_tile_idx, out mapAnimated, out autodraw);
            //                                                                        spef_found = true;
            //                                                                        break;
            //                                                                    }
            //                                                                }

            //                                                                /* Otherwise, normal animation check */
            //                                                                if (!spef_found)
            //                                                                    ntile = _gnollHackService.GetAnimatedTile(ntile, tile_animation_idx, (int)animation_play_types.ANIMATION_PLAY_TYPE_ALWAYS, AnimationTimers.general_animation_counter, out anim_frame_idx, out main_tile_idx, out mapAnimated, out autodraw);
            //                                                            }
            //                                                        }

            //                                                        if (mapAnimated != 0)
            //                                                            _mapData[mapx, mapy].MapAnimated = true;

            //                                                        int enlargement = Tile2Enlargement[ntile];
            //                                                        for (int enl_idx = -1; enl_idx < 5; enl_idx++)
            //                                                        {
            //                                                            if (enlargement == 0 && enl_idx >= 0)
            //                                                                break;

            //                                                            bool vflip_glyph = false;
            //                                                            bool hflip_glyph = false;
            //                                                            if ((hflip != tileflag_hflip) != manual_hflip) /* XOR */
            //                                                                hflip_glyph = true;
            //                                                            else
            //                                                                hflip_glyph = false;

            //                                                            if (tileflag_vflip != manual_vflip) /* XOR */
            //                                                                vflip_glyph = true;
            //                                                            else
            //                                                                vflip_glyph = false;


            //                                                            int enlarg_idx = enl_idx;
            //                                                            int position_index = -1;
            //                                                            int orig_position_index = -1;
            //                                                            if (enlargement > 0)
            //                                                            {
            //                                                                orig_position_index = -1;
            //                                                                /* Set position_index */
            //                                                                if (enlarg_idx == -1)
            //                                                                {
            //                                                                    if (vflip_glyph)
            //                                                                        position_index = 1;
            //                                                                    else
            //                                                                        position_index = -1;
            //                                                                }
            //                                                                else if (enlarg_idx == 0)
            //                                                                {
            //                                                                    orig_position_index = 4;
            //                                                                    if (vflip_glyph)
            //                                                                        position_index = hflip_glyph ? 0 : 2;
            //                                                                    else
            //                                                                        position_index = hflip_glyph ? 3 : 4;
            //                                                                }
            //                                                                else if (enlarg_idx == 1)
            //                                                                {
            //                                                                    orig_position_index = 3;
            //                                                                    if (vflip_glyph)
            //                                                                        position_index = hflip_glyph ? 2 : 0;
            //                                                                    else
            //                                                                        position_index = hflip_glyph ? 4 : 3;
            //                                                                }
            //                                                                else if (enlarg_idx == 2)
            //                                                                {
            //                                                                    orig_position_index = 2;
            //                                                                    if (vflip_glyph)
            //                                                                        position_index = hflip_glyph ? 3 : 4;
            //                                                                    else
            //                                                                        position_index = hflip_glyph ? 0 : 2;
            //                                                                }
            //                                                                else if (enlarg_idx == 3)
            //                                                                {
            //                                                                    orig_position_index = 1;
            //                                                                    if (vflip_glyph)
            //                                                                        position_index = -1;
            //                                                                    else
            //                                                                        position_index = 1;
            //                                                                }
            //                                                                else if (enlarg_idx == 4)
            //                                                                {
            //                                                                    orig_position_index = 0;
            //                                                                    if (vflip_glyph)
            //                                                                        position_index = hflip_glyph ? 4 : 3;
            //                                                                    else
            //                                                                        position_index = hflip_glyph ? 2 : 0;
            //                                                                }

            //                                                            }

            //                                                            if (enlargement > 0 && orig_position_index >= 0)
            //                                                            {
            //                                                                int enl_tile_idx = _enlargementDefs[enlargement].position2tile[orig_position_index];
            //                                                                if (enl_tile_idx >= 0)
            //                                                                {
            //                                                                    int addedindex = 0;
            //                                                                    if (_enlargementDefs[enlargement].number_of_animation_frames > 0)
            //                                                                    {
            //                                                                        if (main_tile_idx == -1
            //                                                                            && anim_frame_idx >= 0
            //                                                                            && anim_frame_idx < _enlargementDefs[enlargement].number_of_animation_frames
            //                                                                            )
            //                                                                        {
            //                                                                            addedindex = anim_frame_idx * _enlargementDefs[enlargement].number_of_enlargement_tiles;
            //                                                                        }
            //                                                                        else if (main_tile_idx == 0
            //                                                                            && anim_frame_idx > 0
            //                                                                            && anim_frame_idx <= _enlargementDefs[enlargement].number_of_animation_frames)
            //                                                                        {
            //                                                                            addedindex = (anim_frame_idx - 1) * _enlargementDefs[enlargement].number_of_enlargement_tiles;
            //                                                                        }
            //                                                                        else if (main_tile_idx == _enlargementDefs[enlargement].number_of_animation_frames
            //                                                                            && anim_frame_idx >= 0
            //                                                                            && anim_frame_idx < _enlargementDefs[enlargement].number_of_animation_frames
            //                                                                            )
            //                                                                        {
            //                                                                            addedindex = anim_frame_idx * _enlargementDefs[enlargement].number_of_enlargement_tiles;
            //                                                                        }
            //                                                                    }
            //                                                                    int enl_glyph = enl_tile_idx + addedindex + EnlargementOffsets[enlargement] + EnlargementOff;
            //                                                                    ntile = Glyph2Tile[enl_glyph]; /* replace */
            //                                                                }
            //                                                                else
            //                                                                    continue;
            //                                                            }

            //                                                            int dx = 0, dy = 0;
            //                                                            int darken_dx = 0, darken_dy = 0;
            //                                                            switch (position_index)
            //                                                            {
            //                                                                case 0:
            //                                                                    dx = -1;
            //                                                                    dy = -1;
            //                                                                    break;
            //                                                                case 1:
            //                                                                    dx = 0;
            //                                                                    dy = -1;
            //                                                                    break;
            //                                                                case 2:
            //                                                                    dx = 1;
            //                                                                    dy = -1;
            //                                                                    break;
            //                                                                case 3:
            //                                                                    dx = -1;
            //                                                                    dy = 0;
            //                                                                    break;
            //                                                                case 4:
            //                                                                    dx = 1;
            //                                                                    dy = 0;
            //                                                                    break;
            //                                                            }

            //                                                            int draw_map_x = mapx + dx;
            //                                                            int draw_map_y = mapy + dy;
            //                                                            if (!GHUtils.isok(draw_map_x, draw_map_y))
            //                                                                continue;

            //                                                            darken_dx = dx;
            //                                                            darken_dy = 0;
            //                                                            //int darken_x = mapx + darken_dx;
            //                                                            //int darken_y = mapy + darken_dy;
            //                                                            //bool darken = ((_mapData[darken_x, darken_y].Layers.layer_flags & (ulong)LayerFlags.LFLAGS_CAN_SEE) == 0);
            //                                                            //if (_mapData[mapx, mapy].Layers.layer_glyphs != null
            //                                                            //    && (_mapData[mapx, mapy].Layers.layer_glyphs[(int)layer_types.LAYER_FLOOR] == UnexploredGlyph
            //                                                            //        || _mapData[mapx, mapy].Layers.layer_glyphs[(int)layer_types.LAYER_FLOOR] == NoGlyph)
            //                                                            //   )
            //                                                            //    darken = false;


            //                                                            int sheet_idx = TileSheetIdx(ntile);
            //                                                            int tile_x = TileSheetX(ntile);
            //                                                            int tile_y = TileSheetY(ntile);

            //                                                            SKRect sourcerect;
            //                                                            float scaled_tile_width = width;
            //                                                            float scaled_tile_height = tileflag_halfsize || (tileflag_normalobjmissile && !tileflag_fullsizeditem) ? height / 2 : height;
            //                                                            float scaled_x_padding = 0;
            //                                                            float scaled_y_padding = 0;
            //                                                            int source_y_added = 0;
            //                                                            int source_height_deducted = 0;
            //                                                            int source_height = tileflag_halfsize ? GHConstants.TileHeight / 2 : GHConstants.TileHeight;

            //                                                            if (tileflag_halfsize)
            //                                                            {
            //                                                                float scale = 1.0f;
            //                                                                if ((layer_idx == (int)layer_types.LAYER_OBJECT || layer_idx == (int)layer_types.LAYER_COVER_OBJECT))
            //                                                                {
            //                                                                    if (obj_in_pit)
            //                                                                        scale *= GHConstants.OBJECT_PIT_SCALING_FACTOR;
            //                                                                }

            //                                                                if (monster_height < 0 && (layer_idx == (int)layer_types.LAYER_MONSTER || layer_idx == (int)layer_types.LAYER_MONSTER_EFFECT))
            //                                                                    scale *= Math.Min(1.0f, Math.Max(0.1f, 1.0f - (1.0f - (float)GHConstants.OBJECT_PIT_SCALING_FACTOR) * (float)monster_height / (float)GHConstants.SPECIAL_HEIGHT_IN_PIT));

            //                                                                if (tileflag_floortile)
            //                                                                {
            //                                                                    if ((layer_idx == (int)layer_types.LAYER_OBJECT || layer_idx == (int)layer_types.LAYER_OBJECT)
            //                                                                        && obj_height > 0 && obj_height < 48)
            //                                                                    {
            //                                                                        source_y_added = (GHConstants.TileHeight / 2 - obj_height) / 2;
            //                                                                        source_height_deducted = GHConstants.TileHeight / 2 - obj_height;
            //                                                                        source_height = GHConstants.TileHeight / 2 - source_height_deducted;
            //                                                                        scaled_tile_width = scale * width;
            //                                                                        scaled_x_padding = (width - scaled_tile_width) / 2;
            //                                                                        scaled_tile_height = scale * (float)source_height * height / (float)GHConstants.TileHeight;
            //                                                                        scaled_y_padding = Math.Max(0, scale * (float)source_height_deducted * height / (float)GHConstants.TileHeight - pit_border);
            //                                                                    }
            //                                                                    sourcerect = new SKRect(tile_x, tile_y + source_y_added, tile_x + GHConstants.TileWidth, tile_y + source_y_added + source_height);
            //                                                                }
            //                                                                else
            //                                                                {
            //                                                                    if ((layer_idx == (int)layer_types.LAYER_OBJECT || layer_idx == (int)layer_types.LAYER_COVER_OBJECT))
            //                                                                    {
            //                                                                        if (obj_height > 0 && obj_height < 48)
            //                                                                            scale *= ((float)obj_height) / 48.0f;
            //                                                                    }
            //                                                                    scaled_tile_width = scale * width;
            //                                                                    scaled_tile_height = scale * height / 2;
            //                                                                    scaled_x_padding = (width - scaled_tile_width) / 2;
            //                                                                    scaled_y_padding = Math.Max(0, height / 2 - scaled_tile_height - pit_border);

            //                                                                    sourcerect = new SKRect(tile_x, tile_y + GHConstants.TileHeight / 2, tile_x + GHConstants.TileWidth, tile_y + GHConstants.TileHeight);
            //                                                                }
            //                                                            }
            //                                                            else
            //                                                            {
            //                                                                if (tileflag_normalobjmissile && !tileflag_fullsizeditem)
            //                                                                {
            //                                                                    if (tileflag_floortile)
            //                                                                    {
            //                                                                        sourcerect = new SKRect(tile_x, tile_y, tile_x + GHConstants.TileWidth, tile_y + GHConstants.TileHeight / 2);
            //                                                                    }
            //                                                                    else
            //                                                                    {
            //                                                                        float scale = 1.0f;
            //                                                                        if (missile_height > 0 && missile_height < 48)
            //                                                                        {
            //                                                                            scale = ((float)missile_height) / 48.0f;
            //                                                                        }
            //                                                                        scaled_tile_width = scale * width;
            //                                                                        scaled_tile_height = scale * height / 2;
            //                                                                        scaled_x_padding = (width - scaled_tile_width) / 2;
            //                                                                        scaled_y_padding = (height / 2 - scaled_tile_height) / 2;

            //                                                                        sourcerect = new SKRect(tile_x, tile_y + GHConstants.TileHeight / 2, tile_x + GHConstants.TileWidth, tile_y + GHConstants.TileHeight);
            //                                                                    }
            //                                                                }
            //                                                                else
            //                                                                {
            //                                                                    if (monster_height < 0 && dy == 0)
            //                                                                    {
            //                                                                        sourcerect = new SKRect(tile_x, tile_y, tile_x + GHConstants.TileWidth, tile_y + GHConstants.TileHeight + monster_height);
            //                                                                        source_height_deducted = -monster_height;
            //                                                                        source_height = GHConstants.TileHeight - source_height_deducted;
            //                                                                        scaled_tile_height = (float)source_height * height / (float)GHConstants.TileHeight;
            //                                                                    }
            //                                                                    else
            //                                                                    {
            //                                                                        sourcerect = new SKRect(tile_x, tile_y, tile_x + GHConstants.TileWidth, tile_y + GHConstants.TileHeight);
            //                                                                    }
            //                                                                }
            //                                                            }

            //                                                            float move_offset_x = 0, move_offset_y = 0;
            //                                                            float opaqueness = 1.0f;
            //                                                            if ((layer_idx == (int)layer_types.LAYER_MONSTER || layer_idx == (int)layer_types.LAYER_MONSTER_EFFECT))
            //                                                            {
            //                                                                move_offset_x = base_move_offset_x;
            //                                                                move_offset_y = base_move_offset_y;
            //                                                                if ((_mapData[mapx, mapy].Layers.monster_flags & (ulong)(LayerMonsterFlags.LMFLAGS_INVISIBLE_TRANSPARENT | LayerMonsterFlags.LMFLAGS_SEMI_TRANSPARENT | LayerMonsterFlags.LMFLAGS_RADIAL_TRANSPARENCY)) != 0)
            //                                                                    opaqueness = 0.5f;
            //                                                            }
            //                                                            else if (layer_idx == (int)layer_types.LAYER_COVER_TRAP)
            //                                                            {
            //                                                                opaqueness = 0.5f;
            //                                                            }

            //                                                            tx = (move_offset_x + width * (float)draw_map_x);
            //                                                            ty = (move_offset_y + scaled_y_height_change + height * (float)draw_map_y);

            //                                                            using (new SKAutoCanvasRestore(canvas, true))
            //                                                            {
            //                                                                canvas.Translate(tx + (hflip_glyph ? width : 0), ty + (vflip_glyph ? height : 0));
            //                                                                canvas.Scale(hflip_glyph ? -1 : 1, vflip_glyph ? -1 : 1, 0, 0);
            //                                                                SKRect targetrect;
            //                                                                if (tileflag_halfsize)
            //                                                                {
            //                                                                    targetrect = new SKRect(scaled_x_padding, height / 2 + scaled_y_padding, scaled_x_padding + scaled_tile_width, height / 2 + scaled_y_padding + scaled_tile_height);
            //                                                                }
            //                                                                else
            //                                                                {
            //                                                                    if (tileflag_normalobjmissile && !tileflag_fullsizeditem)
            //                                                                        targetrect = new SKRect(scaled_x_padding, height / 4 + scaled_y_padding, scaled_x_padding + scaled_tile_width, height / 4 + scaled_y_padding + scaled_tile_height);
            //                                                                    else
            //                                                                        targetrect = new SKRect(scaled_x_padding, scaled_y_padding, scaled_x_padding + scaled_tile_width, scaled_y_padding + scaled_tile_height);
            //                                                                }
            //                                                                paint.Color = paint.Color.WithAlpha((byte)(0xFF * opaqueness));
            //                                                                canvas.DrawBitmap(TileMap[sheet_idx], sourcerect, targetrect, paint);
            //                                                            }

            //                                                            /* AUTODRAW */
            //                                                            if (_autodraws != null && _autodraws[autodraw].draw_type == (int)autodraw_drawing_types.AUTODRAW_DRAW_LONG_WORM)
            //                                                            {
            //                                                                /* Long worm here */

            //                                                                int source_glyph_seg_end = _autodraws[autodraw].source_glyph;
            //                                                                int source_glyph_seg_dir_out = _autodraws[autodraw].source_glyph2;
            //                                                                int source_glyph_seg_dir_in = _autodraws[autodraw].source_glyph2 + 4;
            //                                                                int source_glyph_seg_layer = _autodraws[autodraw].source_glyph3;
            //                                                                int drawing_tail = _autodraws[autodraw].flags;
            //                                                                float scale = height / (float)GHConstants.TileHeight;
            //                                                                int wdir_out = _mapData[mapx, mapy].Layers.wsegdir;
            //                                                                int wdir_in = _mapData[mapx, mapy].Layers.reverse_prev_wsegdir;
            //                                                                bool is_head = (_mapData[mapx, mapy].Layers.monster_flags & (ulong)LayerMonsterFlags.LMFLAGS_WORM_HEAD) != 0;
            //                                                                bool is_tailend = (_mapData[mapx, mapy].Layers.monster_flags & (ulong)LayerMonsterFlags.LMFLAGS_WORM_TAILEND) != 0;
            //                                                                for (int wlayer = 0; wlayer < 5; wlayer++)
            //                                                                {
            //                                                                    int source_glyph = NoGlyph;
            //                                                                    bool hflip_seg = false;
            //                                                                    bool vflip_seg = false;
            //                                                                    switch (wlayer)
            //                                                                    {
            //                                                                        case 0:
            //                                                                        case 2:
            //                                                                        case 4:
            //                                                                            if (is_head || is_tailend)
            //                                                                                continue;
            //                                                                            source_glyph = source_glyph_seg_layer + wlayer / 2;
            //                                                                            break;
            //                                                                        case 1:
            //                                                                            source_glyph = is_tailend ? NoGlyph : is_head ? source_glyph_seg_end : source_glyph_seg_dir_in;
            //                                                                            break;
            //                                                                        case 3:
            //                                                                            source_glyph = is_tailend ? source_glyph_seg_end : is_head ? NoGlyph : source_glyph_seg_dir_out;
            //                                                                            break;
            //                                                                        default:
            //                                                                            break;
            //                                                                    }

            //                                                                    if (source_glyph != NoGlyph)
            //                                                                    {
            //                                                                        int wdir = (wlayer == 1 ? wdir_in : wlayer == 3 ? wdir_out : 0);
            //                                                                        switch (wdir)
            //                                                                        {
            //                                                                            case 1:
            //                                                                                source_glyph += 2;
            //                                                                                hflip_seg = false;
            //                                                                                vflip_seg = false;
            //                                                                                break;
            //                                                                            case 2:
            //                                                                                source_glyph += 0;
            //                                                                                hflip_seg = false;
            //                                                                                vflip_seg = false;
            //                                                                                break;
            //                                                                            case 3:
            //                                                                                source_glyph += 3;
            //                                                                                hflip_seg = false;
            //                                                                                vflip_seg = true;
            //                                                                                break;
            //                                                                            case 4:
            //                                                                                source_glyph += 1;
            //                                                                                hflip_seg = true;
            //                                                                                vflip_seg = false;
            //                                                                                break;
            //                                                                            case 5:
            //                                                                                source_glyph += 3;
            //                                                                                hflip_seg = false;
            //                                                                                vflip_seg = false;
            //                                                                                break;
            //                                                                            case 6:
            //                                                                                source_glyph += 0;
            //                                                                                hflip_seg = false;
            //                                                                                vflip_seg = true;
            //                                                                                break;
            //                                                                            case 7:
            //                                                                                source_glyph += 2;
            //                                                                                hflip_seg = false;
            //                                                                                vflip_seg = true;
            //                                                                                break;
            //                                                                            case 8:
            //                                                                                source_glyph += 1;
            //                                                                                hflip_seg = false;
            //                                                                                vflip_seg = false;
            //                                                                                break;
            //                                                                            default:
            //                                                                                break;
            //                                                                        }

            //                                                                        int atile = Glyph2Tile[source_glyph];
            //                                                                        int a_sheet_idx = TileSheetIdx(atile);
            //                                                                        int at_x = TileSheetX(atile);
            //                                                                        int at_y = TileSheetY(atile);

            //                                                                        int worm_source_x = at_x;
            //                                                                        int worm_source_y = at_y;
            //                                                                        int worm_source_width = GHConstants.TileWidth;
            //                                                                        int worm_source_height = GHConstants.TileHeight;
            //                                                                        sourcerect = new SKRect(worm_source_x, worm_source_y, worm_source_x + worm_source_width, worm_source_y + worm_source_height);

            //                                                                        float target_x = tx;
            //                                                                        float target_y = ty;
            //                                                                        float target_width = ((float)worm_source_width * scale);
            //                                                                        float target_height = ((float)worm_source_height * scale);
            //                                                                        SKRect targetrect;
            //                                                                        targetrect = new SKRect(0, 0, target_width, target_height);

            //                                                                        using (new SKAutoCanvasRestore(canvas, true))
            //                                                                        {
            //                                                                            canvas.Translate(target_x + (hflip_seg ? width : 0), target_y + (vflip_seg ? height : 0));
            //                                                                            canvas.Scale(hflip_seg ? -1 : 1, vflip_seg ? -1 : 1, 0, 0);
            //                                                                            paint.Color = paint.Color.WithAlpha((byte)(0xFF * opaqueness));
            //                                                                            canvas.DrawBitmap(TileMap[sheet_idx], sourcerect, targetrect, paint);
            //                                                                        }
            //                                                                    }
            //                                                                }
            //                                                            }

            //                                                        }
            //                                                    }
            //                                                }
            //                                            }
            //                                        }
            //                                    }

            //                                    /* Darkening at the end of layers */
            //                                    if (layer_idx == (int)layer_types.LAYER_OBJECT)
            //                                    {
            //                                        for (int mapx = startX; mapx <= endX; mapx++)
            //                                        {
            //                                            for (int mapy = startY; mapy <= endY; mapy++)
            //                                            {
            //                                                if(_mapData[mapx, mapy].RedrawTile)
            //                                                {
            //                                                    bool darken = ((_mapData[mapx, mapy].Layers.layer_flags & (ulong)LayerFlags.LFLAGS_CAN_SEE) == 0);

            //                                                    if (_mapData[mapx, mapy].Layers.layer_glyphs != null
            //                                                        && (_mapData[mapx, mapy].Layers.layer_glyphs[(int)layer_types.LAYER_FLOOR] == UnexploredGlyph
            //                                                            || _mapData[mapx, mapy].Layers.layer_glyphs[(int)layer_types.LAYER_FLOOR] == NoGlyph)
            //                                                       )
            //                                                        darken = false;

            //                                                    // Draw rectangle with blend mode in bottom half
            //                                                    if (darken)
            //                                                    {
            //                                                        bool uloc = ((_mapData[mapx, mapy].Layers.layer_flags & (ulong)LayerFlags.LFLAGS_UXUY) != 0);
            //                                                        bool unlit = ((_mapData[mapx, mapy].Layers.layer_flags & (ulong)LayerFlags.LFLAGS_APPEARS_UNLIT) != 0);
            //                                                        // Get values from XAML controls
            //                                                        SKBlendMode blendMode = SKBlendMode.Modulate;
            //                                                        int val = ((uloc ? 85 : unlit ? 35 : 65) * 255) / 100;
            //                                                        SKColor color = new SKColor((byte)val, (byte)val, (byte)val);

            //                                                        paint.Color = color;
            //                                                        SKBlendMode old_bm = paint.BlendMode;
            //                                                        paint.BlendMode = blendMode;
            //                                                        tx = (width * (float)mapx);
            //                                                        ty = (height * (float)mapy);
            //                                                        SKRect targetrect = new SKRect(tx, ty, tx + width, ty + height);
            //                                                        canvas.DrawRect(targetrect, paint);
            //                                                        paint.BlendMode = old_bm;
            //                                                    }
            //                                                }
            //                                            }
            //                                        }
            //                                    }
            //                                }
            //                            }
            //                            for (int mapx = startX; mapx <= endX; mapx++)
            //                            {
            //                                for (int mapy = startY; mapy <= endY; mapy++)
            //                                {
            //                                    _mapData[mapx, mapy].RedrawTile = false;
            //                                    if(_mapData[mapx, mapy].Layers.layer_glyphs != null)
            //                                    {
            //                                        if (_mapData[mapx, mapy].Layers.layer_glyphs[(int)layer_types.LAYER_MONSTER] == NoGlyph)
            //                                            _mapData[mapx, mapy].NeedsUpdate = false;
            //                                    }
            //                                }
            //                            }
            //                        }
            //                    }
            //                }

            //                /* Cursor */
            //                if (GraphicsStyle == GHGraphicsStyle.ASCII && CursorStyle == TTYCursorStyle.BlinkingUnderline && _cursorIsOn && _mapCursorX >= 1 && _mapCursorY >= 0)
            //                {
            //                    int cx = _mapCursorX, cy = _mapCursorY;
            //                    str = "_";
            //                    textPaint.Color = SKColors.White;
            //                    tx = (width * (float)cx);
            //                    ty = (height * (float)cy);
            //                    canvas.DrawText(str, tx, ty, textPaint);
            //                }
            //            }
            //        }
            //    }
            //}
        }

        private SKMaskFilter _blur = SKMaskFilter.CreateBlur(SKBlurStyle.Normal, 3.4f);

        private void canvasView_PaintSurface(object sender, SKPaintSurfaceEventArgs e)
        {
            SKImageInfo info = e.Info;
            SKSurface surface = e.Surface;
            SKCanvas canvas = surface.Canvas;

            string str = "";

            canvas.Clear(SKColors.Black);

            float x = info.Width;
            float y = info.Height;

            using (SKPaint textPaint = new SKPaint())
            {
                textPaint.Color = SKColors.White;
                textPaint.Typeface = App.DiabloTypeface;

                // Adjust TextSize property so text is 90% of screen width
                float textWidth = textPaint.MeasureText(str);
                textPaint.TextSize = 0.65f * info.Width * textPaint.TextSize / textWidth;

                // Find the text bounds
                SKRect textBounds = new SKRect();
                textPaint.MeasureText(str, ref textBounds);

                float xText = 10;
                float yText = 0;
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
                float canvaswidth = canvasView.CanvasSize.Width;
                float canvasheight = canvasView.CanvasSize.Height;
                float UsedFontSize = ZoomAlternateMode ? MapFontAlternateSize : MapFontSize;
                textPaint.Typeface = App.DejaVuSansMonoTypeface;
                textPaint.TextSize = UsedFontSize;
                if (ZoomMiniMode)
                {
                    float tmpwidth = textPaint.FontMetrics.AverageCharacterWidth;
                    float tmpheight = textPaint.FontMetrics.Descent - textPaint.FontMetrics.Ascent;
                    if(GraphicsStyle == GHGraphicsStyle.Tiles)
                    {
                        tmpwidth = GHConstants.TileWidth * UsedFontSize / 48;
                        tmpheight = GHConstants.TileHeight * UsedFontSize / 48;
                    }
                    float tmpmapwidth = tmpwidth * (GHConstants.MapCols - 1);
                    float tmpmapheight = tmpheight * GHConstants.MapRows;
                    float xscale = tmpmapwidth > 0 ? canvaswidth / tmpmapwidth : 0;
                    float yscale = tmpmapheight > 0 ? canvasheight / tmpmapheight : 0;
                    float cscale = Math.Min(xscale, yscale);
                    UsedFontSize = Math.Max(2.0f, UsedFontSize * cscale);
                    textPaint.TextSize = UsedFontSize;
                }
                float width = textPaint.FontMetrics.AverageCharacterWidth;
                float height = textPaint.FontMetrics.Descent - textPaint.FontMetrics.Ascent;

                if (GraphicsStyle == GHGraphicsStyle.Tiles)
                {
                    width = GHConstants.TileWidth * UsedFontSize / 48;
                    height = GHConstants.TileHeight * UsedFontSize / 48;
                }

                float mapwidth = width * (GHConstants.MapCols - 1);
                float mapheight = height * (GHConstants.MapRows);

                _tileWidth = width;
                _tileHeight = height;
                _mapWidth = mapwidth;
                _mapHeight = mapheight;
                _mapFontAscent = textPaint.FontMetrics.Ascent;
                float targetscale = height / (float)GHConstants.TileHeight;

                int startX = 1;
                int endX = GHConstants.MapCols - 1;
                int startY = 0;
                int endY = GHConstants.MapRows - 1;

                float offsetX = (canvaswidth - mapwidth) / 2;
                float offsetY = (canvasheight - mapheight) / 2;

                if(ZoomMiniMode)
                {
                    offsetX -= _mapOffsetX;
                    offsetY -= _mapOffsetY;
                }
                else 
                {
                    lock (ClipLock)
                    {
                        if (ClipX > 0 && (mapwidth > canvaswidth || mapheight > canvasheight))
                        {
                            offsetX -= (ClipX - (GHConstants.MapCols - 1) / 2) * width;
                            offsetY -= (ClipY - GHConstants.MapRows / 2) * height;
                        }
                    }
                }

                float tx = 0, ty = 0;
                float startx = 0, starty = 0;
                if (_clientGame != null)
                {
                    lock (_clientGame.WindowsLock)
                    {
                        if (_clientGame.Windows[_clientGame.MapWindowId] != null)
                        {
                            startx = _clientGame.Windows[_clientGame.MapWindowId].Left;
                            starty = _clientGame.Windows[_clientGame.MapWindowId].Top;
                        }
                    }
                }


                if(_useMapBitmap)
                {
                    lock (_mapBitmapLock)
                    {
                        if (_mapBitmap != null)
                        {
                            float sourcewidth = (float)(GHConstants.MapCols * GHConstants.TileWidth);
                            float sourceheight = (float)(GHConstants.MapRows * GHConstants.TileHeight);
                            SKRect sourcerect = new SKRect(0, 0, sourcewidth, sourceheight);
                            tx = offsetX + _mapOffsetX;
                            ty = offsetY + _mapOffsetY + _mapFontAscent;
                            SKRect targetrect = new SKRect(tx, ty, tx + sourcewidth * width / (float)GHConstants.TileWidth, ty + sourceheight * height / GHConstants.TileHeight);
                            canvas.DrawBitmap(_mapBitmap, sourcerect, targetrect);
                        }
                    }
                }
                else
                {
                    lock (Glyph2TileLock)
                    {
                        lock (_mapDataLock)
                        {
                            if (GraphicsStyle == GHGraphicsStyle.ASCII)
                            {
                                for (int mapx = startX; mapx <= endX; mapx++)
                                {
                                    for (int mapy = startY; mapy <= endY; mapy++)
                                    {
                                        if (_mapData[mapx, mapy].Symbol != null && _mapData[mapx, mapy].Symbol != "")
                                        {
                                            str = _mapData[mapx, mapy].Symbol;
                                            textPaint.Color = _mapData[mapx, mapy].Color;
                                            tx = (offsetX + _mapOffsetX + width * (float)mapx);
                                            ty = (offsetY + _mapOffsetY + height * (float)mapy);
                                            if (CursorStyle == TTYCursorStyle.GreenBlock && _mapCursorX == mapx && _mapCursorY == mapy)
                                            {
                                                textPaint.Style = SKPaintStyle.Fill;
                                                textPaint.Color = _cursorDefaultGreen;
                                                SKRect winRect = new SKRect(tx, ty + textPaint.FontMetrics.Ascent, tx + width, ty + textPaint.FontMetrics.Ascent + height);
                                                canvas.DrawRect(winRect, textPaint);
                                                textPaint.Color = SKColors.Black;
                                            }
                                            else if ((_mapData[mapx, mapy].Special & (uint)MapSpecial.Pet) != 0)
                                            {
                                                textPaint.Style = SKPaintStyle.Fill;
                                                SKRect winRect = new SKRect(tx, ty + textPaint.FontMetrics.Ascent, tx + width, ty + textPaint.FontMetrics.Ascent + height);
                                                canvas.DrawRect(winRect, textPaint);
                                                textPaint.Color = SKColors.Black;
                                            }

                                            canvas.DrawText(str, tx, ty, textPaint);

                                            if ((_mapData[mapx, mapy].Special & (uint)MapSpecial.Peaceful) != 0)
                                            {
                                                canvas.DrawText("_", tx, ty, textPaint);
                                            }
                                        }
                                    }
                                }
                            }
                            else
                            {
                                if (Glyph2Tile != null && _tilesPerRow[0] > 0 && UsedTileSheets > 0)
                                {
                                    using (SKPaint paint = new SKPaint())
                                    {
                                        paint.FilterQuality = SKFilterQuality.None;

                                        bool[,] draw_shadow = new bool[GHConstants.MapCols, GHConstants.MapRows];
                                        float pit_border = (float)GHConstants.PIT_BOTTOM_BORDER * height / (float)GHConstants.TileHeight;
                                        long currentcountervalue = 0;
                                        lock (AnimationTimerLock)
                                        {
                                            currentcountervalue = AnimationTimers.general_animation_counter;
                                        }

                                        float altStartX = -(offsetX + _mapOffsetX) / width - 1;
                                        float altEndX = (canvaswidth - (offsetX + _mapOffsetX)) / width;
                                        float altStartY = -(offsetY + _mapOffsetY) / height - 1;
                                        float altEndY = (canvasheight - (offsetY + _mapOffsetY)) / height;
                                        altStartX -= 3;
                                        altEndX += 3;
                                        altStartY -= 1;
                                        altEndY += 3;
                                        startX = Math.Max(startX, (int)(Math.Sign(altStartX) * Math.Floor(Math.Abs(altStartX))));
                                        endX = Math.Min(endX, (int)Math.Ceiling(altEndX));
                                        startY = Math.Max(startY, (int)(Math.Sign(altStartY) * Math.Floor(Math.Abs(altStartY))));
                                        endY = Math.Min(endY, (int)Math.Ceiling(altEndY));

                                        for (int layer_idx = 0; layer_idx <= (int)layer_types.MAX_LAYERS + 1; layer_idx++)
                                        {
                                            for (int mapy = startY; mapy <= endY; mapy++)
                                            {
                                                for (int mapx = startX; mapx <= endX; mapx++)
                                                {
                                                    if (_mapData[mapx, mapy].Layers.layer_glyphs == null)
                                                        continue;

                                                    if (layer_idx == (int)layer_types.MAX_LAYERS 
                                                        && (!draw_shadow[mapx, mapy] || _mapData[mapx, mapy].Layers.layer_glyphs[(int)layer_types.LAYER_MONSTER] == NoGlyph)
                                                        )
                                                        continue;

                                                    bool loc_is_you = (_mapData[mapx, mapy].Layers.layer_flags & (ulong)LayerFlags.LFLAGS_UXUY) != 0;
                                                    bool canspotself = (_mapData[mapx, mapy].Layers.layer_flags & (ulong)LayerMonsterFlags.LMFLAGS_CAN_SPOT_SELF) != 0;
                                                    short monster_height = _mapData[mapx, mapy].Layers.special_monster_layer_height;
                                                    float scaled_y_height_change = 0;
                                                    sbyte monster_origin_x = _mapData[mapx, mapy].Layers.monster_origin_x;
                                                    sbyte monster_origin_y = _mapData[mapx, mapy].Layers.monster_origin_y;
                                                    long glyphprintcountervalue = _mapData[mapx, mapy].GlyphPrintCounterValue;
                                                    float base_move_offset_x = 0, base_move_offset_y = 0;
                                                    int movediffx = (int)monster_origin_x - mapx;
                                                    int movediffy = (int)monster_origin_y - mapy;
                                                    long counterdiff = currentcountervalue - glyphprintcountervalue;
                                                    //if (GHUtils.isok(monster_origin_x, monster_origin_y) && layer_idx == (int)layer_types.LAYER_MONSTER)
                                                    //    mapx = mapx;

                                                    if (GHUtils.isok(monster_origin_x, monster_origin_y)
                                                        && (movediffx != 0 || movediffy != 0)
                                                        && counterdiff >= 0 && counterdiff < GHConstants.MoveIntervals)
                                                    {
                                                        base_move_offset_x = width * (float)movediffx * (float)(GHConstants.MoveIntervals - counterdiff) / (float)GHConstants.MoveIntervals;
                                                        base_move_offset_y = height * (float)movediffy * (float)(GHConstants.MoveIntervals - counterdiff) / (float)GHConstants.MoveIntervals;
                                                    }

                                                    if (layer_idx == (int)layer_types.MAX_LAYERS + 1)
                                                    {
                                                        if (monster_height > 0)
                                                            scaled_y_height_change = (float)-monster_height * height / (float)GHConstants.TileHeight;

                                                        /* Cursor */
                                                        bool cannotseeself = (loc_is_you && !canspotself);
                                                        if (loc_is_you 
                                                            && (cannotseeself || _show_cursor_on_u) 
                                                            && (mapx == _mapCursorX && mapy == _mapCursorY)
                                                            )
                                                        {
                                                            int cidx = (cannotseeself && _cursorType == game_cursor_types.CURSOR_STYLE_GENERIC_CURSOR ? 
                                                                (int)game_cursor_types.CURSOR_STYLE_INVISIBLE :
                                                                (int)_cursorType);
                                                            int cglyph = cidx + CursorOff;
                                                            int ctile = Glyph2Tile[cglyph];
                                                            int animation = Tile2Animation[ctile];
                                                            int autodraw = Tile2Autodraw[ctile];
                                                            int anim_frame_idx = 0, main_tile_idx = 0;
                                                            sbyte mapAnimated = 0;
                                                            int tile_animation_idx = _gnollHackService.GetTileAnimationIndexFromGlyph(cglyph);
                                                            ctile = _gnollHackService.GetAnimatedTile(ctile, tile_animation_idx, (int)animation_play_types.ANIMATION_PLAY_TYPE_ALWAYS, AnimationTimers.general_animation_counter, out anim_frame_idx, out main_tile_idx, out mapAnimated, out autodraw);
                                                            int sheet_idx = TileSheetIdx(ctile);
                                                            int tile_x = TileSheetX(ctile);
                                                            int tile_y = TileSheetY(ctile);

                                                            tx = (offsetX + _mapOffsetX + (loc_is_you ? base_move_offset_x : 0) + width * (float)mapx);
                                                            ty = (offsetY + _mapOffsetY + (loc_is_you ? base_move_offset_y : 0) + scaled_y_height_change + _mapFontAscent + height * (float)mapy);
                                                            SKRect targetrect = new SKRect(tx, ty, tx + width, ty + height);
                                                            SKRect sourcerect = new SKRect(tile_x, tile_y, tile_x + GHConstants.TileWidth, tile_y + GHConstants.TileHeight);
                                                            canvas.DrawBitmap(TileMap[sheet_idx], sourcerect, targetrect);

                                                        }

                                                        /* General tx, ty for all others, except cursors */
                                                        tx = (offsetX + _mapOffsetX + base_move_offset_x + width * (float)mapx);
                                                        ty = (offsetY + _mapOffsetY + base_move_offset_y + scaled_y_height_change + _mapFontAscent + height * (float)mapy);

                                                        if (HitPointBars)
                                                        {
                                                            /* Draw hit point bars */
                                                            if ((_mapData[mapx, mapy].Layers.layer_flags & (ulong)(LayerFlags.LFLAGS_M_YOU | LayerFlags.LFLAGS_UXUY | LayerFlags.LFLAGS_M_CANSPOTMON)) != 0
                                                            && (_mapData[mapx, mapy].Layers.layer_flags & (ulong)(LayerFlags.LFLAGS_M_WORM_TAIL)) == 0
                                                            && _mapData[mapx, mapy].Layers.monster_maxhp > 0)
                                                            {
                                                                int hp = _mapData[mapx, mapy].Layers.monster_hp;
                                                                int hpmax = _mapData[mapx, mapy].Layers.monster_maxhp;
                                                                float fraction = (hpmax == 0 ? 0 : Math.Max(0, Math.Min(1, (float)hp / (float)hpmax)));
                                                                float r_mult = fraction <= 0.25f ? fraction * 2.0f + 0.5f : fraction <= 0.5f ? 1.0f : (1.0f - fraction) * 2.0f;
                                                                float g_mult = fraction <= 0.25f ? 0 : fraction <= 0.5f ? (fraction - 0.25f) * 4.0f : 1.0f;
                                                                SKColor clr = new SKColor((byte)(255.0f * r_mult), (byte)(255.0f * g_mult), 0);
                                                                SKRect smaller_rect = new SKRect();
                                                                SKRect even_smaller_rect = new SKRect();
                                                                smaller_rect.Bottom = ty + height;
                                                                smaller_rect.Top = ty + height - Math.Max(1, (height) / 12);
                                                                smaller_rect.Left = tx;
                                                                smaller_rect.Right = tx + width;
                                                                even_smaller_rect.Bottom = smaller_rect.Bottom - 1 * targetscale;
                                                                even_smaller_rect.Top = smaller_rect.Top + 1 * targetscale;
                                                                even_smaller_rect.Left = smaller_rect.Left + 1 * targetscale;
                                                                even_smaller_rect.Right = even_smaller_rect.Left + (fraction * (smaller_rect.Right - 1 * targetscale - even_smaller_rect.Left));

                                                                paint.Style = SKPaintStyle.Fill;
                                                                paint.Color = SKColors.Black;
                                                                canvas.DrawRect(smaller_rect, paint);
                                                                paint.Color = clr;
                                                                canvas.DrawRect(even_smaller_rect, paint);
                                                            }
                                                        }

                                                        if ((_mapData[mapx, mapy].Layers.layer_flags & (ulong)(LayerFlags.LFLAGS_M_YOU | LayerFlags.LFLAGS_UXUY | LayerFlags.LFLAGS_M_CANSPOTMON)) != 0
                                                            && (_mapData[mapx, mapy].Layers.layer_flags & (ulong)(LayerFlags.LFLAGS_M_WORM_TAIL)) == 0)
                                                        {
                                                            /* Draw condition and status marks */
                                                            float x_scaling_factor = width / (float)(GHConstants.TileWidth);
                                                            float y_scaling_factor = height / (float)(GHConstants.TileHeight);
                                                            int max_fitted_rows = (GHConstants.TileHeight - 4) / (GHConstants.StatusMarkHeight + 2);
                                                            int status_count = 0;

                                                            ulong status_bits = _mapData[mapx, mapy].Layers.status_bits;
                                                            if (status_bits != 0)
                                                            {
                                                                int[] statusmarkorder = { (int)game_ui_status_mark_types.STATUS_MARK_TOWNGUARD_PEACEFUL, (int)game_ui_status_mark_types.STATUS_MARK_TOWNGUARD_HOSTILE, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
                                                                int tiles_per_row = GHConstants.TileWidth / GHConstants.StatusMarkWidth;
                                                                int mglyph = (int)game_ui_tile_types.STATUS_MARKS + UITileOff;
                                                                int mtile = Glyph2Tile[mglyph];
                                                                int sheet_idx = TileSheetIdx(mtile);
                                                                int tile_x = TileSheetX(mtile);
                                                                int tile_y = TileSheetY(mtile);
                                                                foreach (int status_mark in statusmarkorder)
                                                                {
                                                                    if (status_count >= max_fitted_rows)
                                                                        break;

                                                                    ulong statusbit = 1UL << status_mark;
                                                                    if ((status_bits & statusbit) != 0)
                                                                    {
                                                                        int within_tile_x = status_mark % tiles_per_row;
                                                                        int within_tile_y = status_mark / tiles_per_row;
                                                                        int c_x = tile_x + within_tile_x * GHConstants.StatusMarkWidth;
                                                                        int c_y = tile_y + within_tile_y * GHConstants.StatusMarkHeight;

                                                                        SKRect source_rt = new SKRect();
                                                                        source_rt.Left = c_x;
                                                                        source_rt.Right = c_x + GHConstants.StatusMarkWidth;
                                                                        source_rt.Top = c_y;
                                                                        source_rt.Bottom = c_y + GHConstants.StatusMarkHeight;

                                                                        /* Define draw location in target */
                                                                        int unscaled_left = GHConstants.TileWidth - 2 - GHConstants.StatusMarkWidth;
                                                                        int unscaled_right = unscaled_left + GHConstants.StatusMarkWidth;
                                                                        int unscaled_top = 2 + (2 + GHConstants.StatusMarkWidth) * status_count;
                                                                        int unscaled_bottom = unscaled_top + GHConstants.StatusMarkHeight;

                                                                        SKRect target_rt = new SKRect();
                                                                        target_rt.Left = tx + (int)(x_scaling_factor * (double)unscaled_left);
                                                                        target_rt.Right = tx + (int)(x_scaling_factor * (double)unscaled_right);
                                                                        target_rt.Top = ty + (int)(y_scaling_factor * (double)unscaled_top);
                                                                        target_rt.Bottom = ty + (int)(y_scaling_factor * (double)unscaled_bottom);

                                                                        canvas.DrawBitmap(TileMap[sheet_idx], source_rt, target_rt);

                                                                        status_count++;
                                                                    }
                                                                }
                                                            }

                                                            ulong condition_bits = _mapData[mapx, mapy].Layers.condition_bits;
                                                            if (condition_bits != 0)
                                                            {
                                                                int tiles_per_row = GHConstants.TileWidth / GHConstants.StatusMarkWidth;
                                                                int mglyph = (int)game_ui_tile_types.CONDITION_MARKS + UITileOff;
                                                                int mtile = Glyph2Tile[mglyph];
                                                                int sheet_idx = TileSheetIdx(mtile);
                                                                int tile_x = TileSheetX(mtile);
                                                                int tile_y = TileSheetY(mtile);
                                                                for (int condition_mark = 0; condition_mark < (int)bl_conditions.NUM_BL_CONDITIONS; condition_mark++)
                                                                {
                                                                    if (status_count >= max_fitted_rows)
                                                                        break;

                                                                    ulong conditionbit = 1UL << condition_mark;
                                                                    if ((condition_bits & conditionbit) != 0)
                                                                    {
                                                                        int within_tile_x = condition_mark % tiles_per_row;
                                                                        int within_tile_y = condition_mark / tiles_per_row;
                                                                        int c_x = tile_x + within_tile_x * GHConstants.StatusMarkWidth;
                                                                        int c_y = tile_y + within_tile_y * GHConstants.StatusMarkHeight;

                                                                        SKRect source_rt = new SKRect();
                                                                        source_rt.Left = c_x;
                                                                        source_rt.Right = c_x + GHConstants.StatusMarkWidth;
                                                                        source_rt.Top = c_y;
                                                                        source_rt.Bottom = c_y + GHConstants.StatusMarkHeight;

                                                                        /* Define draw location in target */
                                                                        int unscaled_left = GHConstants.TileWidth - 2 - GHConstants.StatusMarkWidth;
                                                                        int unscaled_right = unscaled_left + GHConstants.StatusMarkWidth;
                                                                        int unscaled_top = 2 + (2 + GHConstants.StatusMarkWidth) * status_count;
                                                                        int unscaled_bottom = unscaled_top + GHConstants.StatusMarkHeight;

                                                                        SKRect target_rt = new SKRect();
                                                                        target_rt.Left = tx + (int)(x_scaling_factor * (double)unscaled_left);
                                                                        target_rt.Right = tx + (int)(x_scaling_factor * (double)unscaled_right);
                                                                        target_rt.Top = ty + (int)(y_scaling_factor * (double)unscaled_top);
                                                                        target_rt.Bottom = ty + (int)(y_scaling_factor * (double)unscaled_bottom);

                                                                        canvas.DrawBitmap(TileMap[sheet_idx], source_rt, target_rt);

                                                                        status_count++;
                                                                    }
                                                                }
                                                            }

                                                            for (int buff_ulong = 0; buff_ulong < GHConstants.NUM_BUFF_BIT_ULONGS; buff_ulong++)
                                                            {
                                                                if (status_count >= max_fitted_rows)
                                                                    break;

                                                                ulong buff_bits = _mapData[mapx, mapy].Layers.buff_bits[buff_ulong];
                                                                int tiles_per_row = GHConstants.TileWidth / GHConstants.StatusMarkWidth;
                                                                if (buff_bits != 0)
                                                                {
                                                                    for (int buff_idx = 0; buff_idx < 32; buff_idx++)
                                                                    {
                                                                        if (status_count >= max_fitted_rows)
                                                                            break;

                                                                        ulong buffbit = 1UL << buff_idx;
                                                                        if ((buff_bits & buffbit) != 0)
                                                                        {
                                                                            int propidx = buff_ulong * 32 + buff_idx;
                                                                            if (propidx > GHConstants.LAST_PROP)
                                                                                break;
                                                                            int mglyph = (propidx - 1) / GHConstants.BUFFS_PER_TILE + BuffTileOff;
                                                                            int mtile = Glyph2Tile[mglyph];
                                                                            int sheet_idx = TileSheetIdx(mtile);
                                                                            int tile_x = TileSheetX(mtile);
                                                                            int tile_y = TileSheetY(mtile);

                                                                            int buff_mark = (propidx - 1) % GHConstants.BUFFS_PER_TILE;
                                                                            int within_tile_x = buff_mark % tiles_per_row;
                                                                            int within_tile_y = buff_mark / tiles_per_row;
                                                                            int c_x = tile_x + within_tile_x * GHConstants.StatusMarkWidth;
                                                                            int c_y = tile_y + within_tile_y * GHConstants.StatusMarkHeight;

                                                                            SKRect source_rt = new SKRect();
                                                                            source_rt.Left = c_x;
                                                                            source_rt.Right = c_x + GHConstants.StatusMarkWidth;
                                                                            source_rt.Top = c_y;
                                                                            source_rt.Bottom = c_y + GHConstants.StatusMarkHeight;

                                                                            /* Define draw location in target */
                                                                            int unscaled_left = GHConstants.TileWidth - 2 - GHConstants.StatusMarkWidth;
                                                                            int unscaled_right = unscaled_left + GHConstants.StatusMarkWidth;
                                                                            int unscaled_top = 2 + (2 + GHConstants.StatusMarkWidth) * status_count;
                                                                            int unscaled_bottom = unscaled_top + GHConstants.StatusMarkHeight;

                                                                            SKRect target_rt = new SKRect();
                                                                            target_rt.Left = tx + (int)(x_scaling_factor * (double)unscaled_left);
                                                                            target_rt.Right = tx + (int)(x_scaling_factor * (double)unscaled_right);
                                                                            target_rt.Top = ty + (int)(y_scaling_factor * (double)unscaled_top);
                                                                            target_rt.Bottom = ty + (int)(y_scaling_factor * (double)unscaled_bottom);

                                                                            canvas.DrawBitmap(TileMap[sheet_idx], source_rt, target_rt);

                                                                            status_count++;
                                                                        }
                                                                    }
                                                                }
                                                            }

                                                        }

                                                        /* Draw death and hit markers */
                                                        if ((_mapData[mapx, mapy].Layers.layer_flags & (ulong)LayerFlags.LFLAGS_M_KILLED) != 0)
                                                        {
                                                            int mglyph = (int)general_tile_types.GENERAL_TILE_DEATH + GeneralTileOff;
                                                            int mtile = Glyph2Tile[mglyph];
                                                            int sheet_idx = TileSheetIdx(mtile);
                                                            int tile_x = TileSheetX(mtile);
                                                            int tile_y = TileSheetY(mtile);

                                                            SKRect targetrect = new SKRect(tx, ty, tx + width, ty + height);
                                                            SKRect sourcerect = new SKRect(tile_x, tile_y, tile_x + GHConstants.TileWidth, tile_y + GHConstants.TileHeight);
                                                            canvas.DrawBitmap(TileMap[sheet_idx], sourcerect, targetrect);
                                                        }
                                                        else if ((_mapData[mapx, mapy].Layers.layer_flags & (ulong)LayerFlags.LFLAGS_M_BEING_HIT) != 0)
                                                        {
                                                            short hit_text_num = _mapData[mapx, mapy].Layers.hit_tile;
                                                            int mglyph = Math.Max(0, Math.Min((int)hit_tile_types.MAX_HIT_TILES - 1, (int)hit_text_num)) + HitTileOff;
                                                            int mtile = Glyph2Tile[mglyph];
                                                            int sheet_idx = TileSheetIdx(mtile);
                                                            int tile_x = TileSheetX(mtile);
                                                            int tile_y = TileSheetY(mtile);

                                                            SKRect targetrect = new SKRect(tx, ty, tx + width, ty + height);
                                                            SKRect sourcerect = new SKRect(tile_x, tile_y, tile_x + GHConstants.TileWidth, tile_y + GHConstants.TileHeight);
                                                            canvas.DrawBitmap(TileMap[sheet_idx], sourcerect, targetrect);
                                                        }

                                                    }
                                                    else
                                                    {
                                                        int sub_layer_cnt = 1;
                                                        lock (_objectDataLock)
                                                        {
                                                            if (layer_idx == (int)layer_types.LAYER_OBJECT)
                                                            {
                                                                if ((_mapData[mapx, mapy].Layers.layer_flags & (ulong)LayerFlags.LFLAGS_SHOWING_MEMORY) != 0)
                                                                    sub_layer_cnt = _objectData[mapx, mapy].MemoryObjectList == null ? 0 : Math.Min(GHConstants.MaxObjectsDrawn, _objectData[mapx, mapy].MemoryObjectList.Count);
                                                                else if ((_mapData[mapx, mapy].Layers.layer_flags & (ulong)LayerFlags.LFLAGS_CAN_SEE) != 0)
                                                                    sub_layer_cnt = _objectData[mapx, mapy].FloorObjectList == null ? 0 : Math.Min(GHConstants.MaxObjectsDrawn, _objectData[mapx, mapy].FloorObjectList.Count);
                                                                else
                                                                    sub_layer_cnt = 1; /* As a backup, show layer glyph (probably often NoGlyph) */
                                                            }
                                                            else if (layer_idx == (int)layer_types.LAYER_COVER_OBJECT)
                                                            {
                                                                if ((_mapData[mapx, mapy].Layers.layer_flags & (ulong)LayerFlags.LFLAGS_SHOWING_MEMORY) != 0)
                                                                    sub_layer_cnt = _objectData[mapx, mapy].CoverMemoryObjectList == null ? 0 : Math.Min(GHConstants.MaxObjectsDrawn, _objectData[mapx, mapy].CoverMemoryObjectList.Count);
                                                                else if ((_mapData[mapx, mapy].Layers.layer_flags & (ulong)LayerFlags.LFLAGS_CAN_SEE) != 0)
                                                                    sub_layer_cnt = _objectData[mapx, mapy].CoverFloorObjectList == null ? 0 : Math.Min(GHConstants.MaxObjectsDrawn, _objectData[mapx, mapy].CoverFloorObjectList.Count);
                                                                else
                                                                    sub_layer_cnt = 1; /* As a backup, show layer glyph (probably often NoGlyph) */
                                                            }
                                                            for (int sub_layer_idx = sub_layer_cnt - 1; sub_layer_idx >= 0; sub_layer_idx--)
                                                            {

                                                                int signed_glyph = NoGlyph;
                                                                short obj_height = _mapData[mapx, mapy].Layers.object_height;
                                                                ObjectDataItem otmp_round = null;

                                                                int source_main_dir_num = 1;
                                                                switch(layer_idx)
                                                                {
                                                                    case (int)layer_types.LAYER_MONSTER:
                                                                        source_main_dir_num = GHConstants.NUM_WORM_SOURCE_DIRS + 1;
                                                                        break;
                                                                    case (int)layer_types.LAYER_CHAIN:
                                                                        source_main_dir_num = GHConstants.NUM_CHAIN_SOURCE_DIRS + 1;
                                                                        break;
                                                                    case (int)layer_types.LAYER_ZAP:
                                                                        source_main_dir_num = GHConstants.NUM_ZAP_SOURCE_DIRS + 1;
                                                                        break;
                                                                }

                                                                for (int source_dir_main_idx = 0; source_dir_main_idx < source_main_dir_num; source_dir_main_idx++)
                                                                {
                                                                    int source_dir_idx = source_dir_main_idx;
                                                                    switch (layer_idx)
                                                                    {
                                                                        case (int)layer_types.LAYER_CHAIN:
                                                                        case (int)layer_types.LAYER_MONSTER:
                                                                            source_dir_idx = source_dir_main_idx * 2;
                                                                            break;
                                                                    }

                                                                    bool manual_hflip = false;
                                                                    bool manual_vflip = false;
                                                                    int adj_x = mapx;
                                                                    int adj_y = mapy;
                                                                    if (source_dir_idx > 0)
                                                                    {
                                                                        switch ((source_dir_idx - 1) % GHConstants.NUM_ZAP_SOURCE_BASE_DIRS + 1)
                                                                        {
                                                                            case 1:
                                                                                adj_x = mapx + 1;
                                                                                adj_y = mapy + 1;
                                                                                break;
                                                                            case 2:
                                                                                adj_x = mapx;
                                                                                adj_y = mapy + 1;
                                                                                break;
                                                                            case 3:
                                                                                adj_x = mapx - 1;
                                                                                adj_y = mapy + 1;
                                                                                break;
                                                                            case 4:
                                                                                adj_x = mapx - 1;
                                                                                adj_y = mapy;
                                                                                break;
                                                                            case 5:
                                                                                adj_x = mapx - 1;
                                                                                adj_y = mapy - 1;
                                                                                break;
                                                                            case 6:
                                                                                adj_x = mapx;
                                                                                adj_y = mapy - 1;
                                                                                break;
                                                                            case 7:
                                                                                adj_x = mapx + 1;
                                                                                adj_y = mapy - 1;
                                                                                break;
                                                                            case 8:
                                                                                adj_x = mapx + 1;
                                                                                adj_y = mapy;
                                                                                break;
                                                                            default:
                                                                                break;

                                                                        }

                                                                        if (layer_idx == (int)layer_types.LAYER_ZAP)
                                                                        {
                                                                            int adjacent_zap_glyph = _mapData[mapx, mapy].Layers.layer_glyphs[(int)layer_types.LAYER_ZAP];
                                                                            ulong adjacent_layer_flags = (ulong)_mapData[mapx, mapy].Layers.layer_flags;

                                                                            if (adjacent_zap_glyph == NoGlyph) // || !glyph_is_zap(adjacent_zap_glyph))
                                                                                signed_glyph = NoGlyph;
                                                                            else
                                                                                signed_glyph = _gnollHackService.ZapGlyphToCornerGlyph(adjacent_zap_glyph, adjacent_layer_flags, source_dir_idx);
                                                                        }
                                                                        else if (layer_idx == (int)layer_types.LAYER_MONSTER)
                                                                        {
                                                                            /* Worm */
                                                                            uint worm_id_stored = _mapData[mapx, mapy].Layers.m_id;
                                                                            if (worm_id_stored == 0)
                                                                                continue;

                                                                            bool is_long_worm_with_tail = (_mapData[mapx, mapy].Layers.monster_flags & (ulong)LayerMonsterFlags.LMFLAGS_LONG_WORM_WITH_TAIL) != 0;
                                                                            bool is_long_worm_tail = (_mapData[mapx, mapy].Layers.monster_flags & (ulong)LayerMonsterFlags.LMFLAGS_LONG_WORM_TAIL) != 0;
                                                                            bool is_adj_worm_tail = (_mapData[mapx, mapy].Layers.layer_flags & (ulong)LayerFlags.LFLAGS_M_WORM_TAIL) != 0;
                                                                            bool is_adj_worm_seen = (_mapData[mapx, mapy].Layers.layer_flags & (ulong)LayerFlags.LFLAGS_M_WORM_SEEN) != 0;
                                                                            bool worm = !is_adj_worm_tail ? false : is_adj_worm_seen ? (worm_id_stored > 0 ? true : false) : true;
                                                                            signed_glyph = NoGlyph;

                                                                            if (worm && (_mapData[mapx, mapy].Layers.layer_flags & (ulong)LayerFlags.LFLAGS_M_WORM_SEEN) != 0
                                                                                && ((
                                                                                _mapData[mapx, mapy].Layers.layer_flags & (ulong)LayerFlags.LFLAGS_CAN_SEE) != 0
                                                                                || is_adj_worm_seen || (_mapData[mapx, mapy].Layers.layer_flags & (ulong)LayerFlags.LFLAGS_M_WORM_SEEN) != 0))
                                                                            {
                                                                                if (is_long_worm_with_tail && !is_adj_worm_tail)
                                                                                {
                                                                                    signed_glyph = NoGlyph;
                                                                                }
                                                                                else if (is_long_worm_tail || (is_long_worm_with_tail && is_adj_worm_tail))
                                                                                {
                                                                                    int signed_main_glyph = _mapData[mapx, mapy].Layers.layer_glyphs[layer_idx];
                                                                                    int main_glyph = Math.Abs(signed_main_glyph);
                                                                                    int tile_animation_index = _gnollHackService.GetTileAnimationIndexFromGlyph(main_glyph);
                                                                                    int main_tile = Glyph2Tile[main_glyph];
                                                                                    int wormautodraw = Tile2Autodraw[main_tile];
                                                                                    int base_source_glyph = NoGlyph;
                                                                                    if (wormautodraw > 0)
                                                                                    {
                                                                                        base_source_glyph = _autodraws[wormautodraw].source_glyph4;
                                                                                    }

                                                                                    int wdir = _mapData[mapx, mapy].Layers.wsegdir;
                                                                                    int tilenum = -1;
                                                                                    if (wdir % 2 == 1)
                                                                                    {
                                                                                        switch (source_dir_idx)
                                                                                        {
                                                                                            case 2:
                                                                                                if (wdir == 7)
                                                                                                {
                                                                                                    //tilenum = GENERAL_TILE_WORM_IS_UP_GOING_DOWN_LEFT;
                                                                                                    tilenum = 1; //GENERAL_TILE_WORM_IS_DOWN_GOING_UP_LEFT;
                                                                                                    manual_vflip = true;
                                                                                                }
                                                                                                else if (wdir == 5)
                                                                                                {
                                                                                                    //tilenum = GENERAL_TILE_WORM_IS_UP_GOING_DOWN_RIGHT;
                                                                                                    tilenum = 3; // GENERAL_TILE_WORM_IS_UP_GOING_DOWN_RIGHT;
                                                                                                    manual_hflip = false;
                                                                                                    manual_vflip = false;
                                                                                                }
                                                                                                break;
                                                                                            case 4:
                                                                                                if (wdir == 1)
                                                                                                {
                                                                                                    //tilenum = GENERAL_TILE_WORM_IS_RIGHT_GOING_UP_LEFT;
                                                                                                    tilenum = 0;  //GENERAL_TILE_WORM_IS_RIGHT_GOING_UP_LEFT;
                                                                                                    manual_hflip = false;
                                                                                                    manual_vflip = false;
                                                                                                }
                                                                                                else if (wdir == 7)
                                                                                                {
                                                                                                    //tilenum = GENERAL_TILE_WORM_IS_RIGHT_GOING_DOWN_LEFT;
                                                                                                    tilenum = 0; // GENERAL_TILE_WORM_IS_RIGHT_GOING_UP_LEFT;
                                                                                                    manual_hflip = false;
                                                                                                    manual_vflip = true;
                                                                                                }
                                                                                                break;
                                                                                            case 6:
                                                                                                if (wdir == 1)
                                                                                                {
                                                                                                    //tilenum = GENERAL_TILE_WORM_IS_DOWN_GOING_UP_LEFT;
                                                                                                    tilenum = 1; // GENERAL_TILE_WORM_IS_DOWN_GOING_UP_LEFT;
                                                                                                    manual_hflip = false;
                                                                                                    manual_vflip = false;
                                                                                                }
                                                                                                else if (wdir == 3)
                                                                                                {
                                                                                                    //tilenum = GENERAL_TILE_WORM_IS_DOWN_GOING_UP_RIGHT;
                                                                                                    tilenum = 3; // GENERAL_TILE_WORM_IS_UP_GOING_DOWN_RIGHT;
                                                                                                    manual_hflip = false;
                                                                                                    manual_vflip = true;
                                                                                                }
                                                                                                break;
                                                                                            case 8:
                                                                                                if (wdir == 3)
                                                                                                {
                                                                                                    //tilenum = GENERAL_TILE_WORM_IS_LEFT_GOING_UP_RIGHT;
                                                                                                    tilenum = 2; // GENERAL_TILE_WORM_IS_LEFT_GOING_DOWN_RIGHT;
                                                                                                    manual_hflip = false;
                                                                                                    manual_vflip = true;
                                                                                                }
                                                                                                else if (wdir == 5)
                                                                                                {
                                                                                                    //tilenum = GENERAL_TILE_WORM_IS_LEFT_GOING_DOWN_RIGHT;
                                                                                                    tilenum = 2; // GENERAL_TILE_WORM_IS_LEFT_GOING_DOWN_RIGHT;
                                                                                                    manual_hflip = false;
                                                                                                    manual_vflip = false;
                                                                                                }
                                                                                                break;
                                                                                            default:
                                                                                                break;
                                                                                        }
                                                                                        if (tilenum > -1)
                                                                                            signed_glyph = tilenum + base_source_glyph;
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                                        else if (layer_idx == (int)layer_types.LAYER_CHAIN)
                                                                        {
                                                                            /* Chain */
                                                                            if ((_mapData[mapx, mapy].Layers.layer_flags & (ulong)LayerFlags.LFLAGS_O_CHAIN) != 0)
                                                                            {
                                                                                signed_glyph = (source_dir_idx / 2 - 1) + (int)general_tile_types.GENERAL_TILE_CHAIN_IS_UP + GeneralTileOff;
                                                                            }
                                                                            else
                                                                                signed_glyph = NoGlyph;
                                                                        }
                                                                    }
                                                                    else if (layer_idx == (int)layer_types.LAYER_OBJECT)
                                                                    {
                                                                        if ((_mapData[mapx, mapy].Layers.layer_flags & (ulong)LayerFlags.LFLAGS_SHOWING_MEMORY) != 0)
                                                                        {
                                                                            otmp_round = _objectData[mapx, mapy].MemoryObjectList[sub_layer_idx];
                                                                            signed_glyph = _objectData[mapx, mapy].MemoryObjectList[sub_layer_idx].ObjData.gui_glyph;
                                                                            obj_height = _objectData[mapx, mapy].MemoryObjectList[sub_layer_idx].TileHeight;
                                                                        }
                                                                        else if ((_mapData[mapx, mapy].Layers.layer_flags & (ulong)LayerFlags.LFLAGS_CAN_SEE) != 0)
                                                                        {
                                                                            otmp_round = _objectData[mapx, mapy].FloorObjectList[sub_layer_idx];
                                                                            signed_glyph = _objectData[mapx, mapy].FloorObjectList[sub_layer_idx].ObjData.gui_glyph;
                                                                            obj_height = _objectData[mapx, mapy].FloorObjectList[sub_layer_idx].TileHeight;
                                                                        }
                                                                        else
                                                                        {
                                                                            signed_glyph = _mapData[mapx, mapy].Layers.layer_glyphs == null ? NoGlyph : _mapData[mapx, mapy].Layers.layer_glyphs[layer_idx];
                                                                        }
                                                                    }
                                                                    else if (layer_idx == (int)layer_types.LAYER_COVER_OBJECT)
                                                                    {
                                                                        if ((_mapData[mapx, mapy].Layers.layer_flags & (ulong)LayerFlags.LFLAGS_SHOWING_MEMORY) != 0)
                                                                        {
                                                                            otmp_round = _objectData[mapx, mapy].CoverMemoryObjectList[sub_layer_idx];
                                                                            signed_glyph = _objectData[mapx, mapy].CoverMemoryObjectList[sub_layer_idx].ObjData.gui_glyph;
                                                                            obj_height = _objectData[mapx, mapy].CoverMemoryObjectList[sub_layer_idx].TileHeight;
                                                                        }
                                                                        else if ((_mapData[mapx, mapy].Layers.layer_flags & (ulong)LayerFlags.LFLAGS_CAN_SEE) != 0)
                                                                        {
                                                                            otmp_round = _objectData[mapx, mapy].CoverFloorObjectList[sub_layer_idx];
                                                                            signed_glyph = _objectData[mapx, mapy].CoverFloorObjectList[sub_layer_idx].ObjData.gui_glyph;
                                                                            obj_height = _objectData[mapx, mapy].CoverFloorObjectList[sub_layer_idx].TileHeight;
                                                                        }
                                                                        else
                                                                        {
                                                                            signed_glyph = _mapData[mapx, mapy].Layers.layer_glyphs == null ? NoGlyph : _mapData[mapx, mapy].Layers.layer_glyphs[layer_idx];
                                                                        }
                                                                    }
                                                                    else
                                                                    {
                                                                        int used_layer_idx = layer_idx;
                                                                        if (layer_idx == (int)layer_types.MAX_LAYERS)
                                                                            used_layer_idx = (int)layer_types.LAYER_MONSTER;
                                                                        signed_glyph = _mapData[mapx, mapy].Layers.layer_glyphs == null ? NoGlyph : _mapData[mapx, mapy].Layers.layer_glyphs[used_layer_idx];
                                                                    }

                                                                    if (signed_glyph == NoGlyph)
                                                                        continue;

                                                                    int glyph = Math.Abs(signed_glyph);
                                                                    if (glyph == 0 || glyph >= Glyph2Tile.Length)
                                                                        continue;

                                                                    short missile_height = _mapData[mapx, mapy].Layers.missile_height;
                                                                    bool obj_in_pit = (_mapData[mapx, mapy].Layers.layer_flags & (ulong)LayerFlags.LFLAGS_O_IN_PIT) != 0;

                                                                    /* Base flips */
                                                                    bool hflip = (signed_glyph < 0);

                                                                    /* Tile flips */
                                                                    bool tileflag_hflip = (GlyphTileFlags[glyph] & (byte)glyph_tile_flags.GLYPH_TILE_FLAG_FLIP_HORIZONTALLY) != 0;
                                                                    bool tileflag_vflip = (GlyphTileFlags[glyph] & (byte)glyph_tile_flags.GLYPH_TILE_FLAG_FLIP_VERTICALLY) != 0;
                                                                    bool tileflag_halfsize = (GlyphTileFlags[glyph] & (byte)glyph_tile_flags.GLYPH_TILE_FLAG_HALF_SIZED_TILE) != 0;
                                                                    bool tileflag_floortile = (GlyphTileFlags[glyph] & (byte)glyph_tile_flags.GLYPH_TILE_FLAG_HAS_FLOOR_TILE) != 0;
                                                                    bool tileflag_normalobjmissile = (GlyphTileFlags[glyph] & (byte)glyph_tile_flags.GLYPH_TILE_FLAG_NORMAL_ITEM_AS_MISSILE) != 0;
                                                                    bool tileflag_fullsizeditem = (GlyphTileFlags[glyph] & (byte)glyph_tile_flags.GLYPH_TILE_FLAG_FULL_SIZED_ITEM) != 0;

                                                                    if ((!tileflag_halfsize || monster_height > 0) && (layer_idx == (int)layer_types.LAYER_MONSTER || layer_idx == (int)layer_types.LAYER_MONSTER_EFFECT || layer_idx == (int)layer_types.MAX_LAYERS))
                                                                        scaled_y_height_change = (float)-monster_height * height / (float)GHConstants.TileHeight;
                                                                    else if (tileflag_halfsize && (layer_idx == (int)layer_types.LAYER_OBJECT || layer_idx == (int)layer_types.LAYER_COVER_OBJECT))
                                                                        scaled_y_height_change = (float)(-(sub_layer_cnt - 1 - sub_layer_idx) * GHConstants.OBJECT_PILE_HEIGHT_DIFFERENCE - GHConstants.OBJECT_PILE_START_HEIGHT) * height / (float)GHConstants.TileHeight;

                                                                    int ntile = Glyph2Tile[glyph];
                                                                    /* Replace tile here */
                                                                    int animation = Tile2Animation[ntile];
                                                                    int autodraw = Tile2Autodraw[ntile];
                                                                    int anim_frame_idx = 0, main_tile_idx = 0;
                                                                    sbyte mapAnimated = 0;
                                                                    int tile_animation_idx = _gnollHackService.GetTileAnimationIndexFromGlyph(glyph);
                                                                    bool is_dropping_piercer = false;

                                                                    /* Determine animation tile here */
                                                                    lock (AnimationTimerLock)
                                                                    {
                                                                        if (AnimationTimers.u_action_animation_counter_on && (layer_idx == (int)layer_types.LAYER_MONSTER || layer_idx == (int)layer_types.MAX_LAYERS) && ((_mapData[mapx, mapy].Layers.layer_flags & (ulong)LayerFlags.LFLAGS_UXUY) != 0))
                                                                            ntile = _gnollHackService.GetAnimatedTile(ntile, tile_animation_idx, (int)animation_play_types.ANIMATION_PLAY_TYPE_PLAYED_SEPARATELY, AnimationTimers.u_action_animation_counter, out anim_frame_idx, out main_tile_idx, out mapAnimated, out autodraw);
                                                                        else if (AnimationTimers.m_action_animation_counter_on && ((!is_dropping_piercer && (layer_idx == (int)layer_types.LAYER_MONSTER || layer_idx == (int)layer_types.MAX_LAYERS)) || (is_dropping_piercer && layer_idx == (int)layer_types.LAYER_MISSILE)) && AnimationTimers.m_action_animation_x == mapx && AnimationTimers.m_action_animation_y == mapy)
                                                                            ntile = _gnollHackService.GetAnimatedTile(ntile, tile_animation_idx, (int)animation_play_types.ANIMATION_PLAY_TYPE_PLAYED_SEPARATELY, AnimationTimers.m_action_animation_counter, out anim_frame_idx, out main_tile_idx, out mapAnimated, out autodraw);
                                                                        else if (_gnollHackService.GlyphIsExplosion(glyph))
                                                                            ntile = _gnollHackService.GetAnimatedTile(ntile, tile_animation_idx, (int)animation_play_types.ANIMATION_PLAY_TYPE_PLAYED_SEPARATELY, AnimationTimers.explosion_animation_counter, out anim_frame_idx, out main_tile_idx, out mapAnimated, out autodraw);
                                                                        else if (_gnollHackService.GlyphIsZap(glyph))
                                                                        {
                                                                            for (int zap_anim_idx = 0; zap_anim_idx < GHConstants.MaxPlayedZapAnimations; zap_anim_idx++)
                                                                            {
                                                                                if (AnimationTimers.zap_animation_counter_on[zap_anim_idx]
                                                                                    && mapx == AnimationTimers.zap_animation_x[zap_anim_idx]
                                                                                    && mapy == AnimationTimers.zap_animation_y[zap_anim_idx])
                                                                                {
                                                                                    ntile = _gnollHackService.GetAnimatedTile(ntile, tile_animation_idx, (int)animation_play_types.ANIMATION_PLAY_TYPE_PLAYED_SEPARATELY, AnimationTimers.zap_animation_counter[zap_anim_idx], out anim_frame_idx, out main_tile_idx, out mapAnimated, out autodraw);
                                                                                    break;
                                                                                }
                                                                            }
                                                                        }
                                                                        else
                                                                        {
                                                                            /* Check for special effect animations */
                                                                            bool spef_found = false;
                                                                            for (int spef_idx = 0; spef_idx < GHConstants.MaxPlayedSpecialEffects; spef_idx++)
                                                                            {
                                                                                if (AnimationTimers.special_effect_animation_counter_on[spef_idx]
                                                                                    && layer_idx == (int)AnimationTimers.spef_action_animation_layer[spef_idx]
                                                                                    && mapx == AnimationTimers.spef_action_animation_x[spef_idx]
                                                                                    && mapy == AnimationTimers.spef_action_animation_y[spef_idx])
                                                                                {
                                                                                    ntile = _gnollHackService.GetAnimatedTile(ntile, tile_animation_idx, (int)animation_play_types.ANIMATION_PLAY_TYPE_PLAYED_SEPARATELY, AnimationTimers.special_effect_animation_counter[spef_idx], out anim_frame_idx, out main_tile_idx, out mapAnimated, out autodraw);
                                                                                    spef_found = true;
                                                                                    break;
                                                                                }
                                                                            }

                                                                            /* Otherwise, normal animation check */
                                                                            if (!spef_found)
                                                                                ntile = _gnollHackService.GetAnimatedTile(ntile, tile_animation_idx, (int)animation_play_types.ANIMATION_PLAY_TYPE_ALWAYS, AnimationTimers.general_animation_counter, out anim_frame_idx, out main_tile_idx, out mapAnimated, out autodraw);
                                                                        }
                                                                    }

                                                                    int enlargement = Tile2Enlargement[ntile];
                                                                    for (int enl_idx = -1; enl_idx < 5; enl_idx++)
                                                                    {
                                                                        if (enlargement == 0 && enl_idx >= 0)
                                                                            break;

                                                                        bool vflip_glyph = false;
                                                                        bool hflip_glyph = false;
                                                                        if ((hflip != tileflag_hflip) != manual_hflip) /* XOR */
                                                                            hflip_glyph = true;
                                                                        else
                                                                            hflip_glyph = false;

                                                                        if (tileflag_vflip != manual_vflip) /* XOR */
                                                                            vflip_glyph = true;
                                                                        else
                                                                            vflip_glyph = false;


                                                                        int enlarg_idx = enl_idx;
                                                                        int position_index = -1;
                                                                        int orig_position_index = -1;
                                                                        if (enlargement > 0)
                                                                        {
                                                                            orig_position_index = -1;
                                                                            /* Set position_index */
                                                                            if (enlarg_idx == -1)
                                                                            {
                                                                                if (vflip_glyph)
                                                                                    position_index = 1;
                                                                                else
                                                                                    position_index = -1;
                                                                            }
                                                                            else if (enlarg_idx == 0)
                                                                            {
                                                                                orig_position_index = 4;
                                                                                if (vflip_glyph)
                                                                                    position_index = hflip_glyph ? 0 : 2;
                                                                                else
                                                                                    position_index = hflip_glyph ? 3 : 4;
                                                                            }
                                                                            else if (enlarg_idx == 1)
                                                                            {
                                                                                orig_position_index = 3;
                                                                                if (vflip_glyph)
                                                                                    position_index = hflip_glyph ? 2 : 0;
                                                                                else
                                                                                    position_index = hflip_glyph ? 4 : 3;
                                                                            }
                                                                            else if (enlarg_idx == 2)
                                                                            {
                                                                                orig_position_index = 2;
                                                                                if (vflip_glyph)
                                                                                    position_index = hflip_glyph ? 3 : 4;
                                                                                else
                                                                                    position_index = hflip_glyph ? 0 : 2;
                                                                            }
                                                                            else if (enlarg_idx == 3)
                                                                            {
                                                                                orig_position_index = 1;
                                                                                if (vflip_glyph)
                                                                                    position_index = -1;
                                                                                else
                                                                                    position_index = 1;
                                                                            }
                                                                            else if (enlarg_idx == 4)
                                                                            {
                                                                                orig_position_index = 0;
                                                                                if (vflip_glyph)
                                                                                    position_index = hflip_glyph ? 4 : 3;
                                                                                else
                                                                                    position_index = hflip_glyph ? 2 : 0;
                                                                            }

                                                                        }

                                                                        if (enlargement > 0 && orig_position_index >= 0)
                                                                        {
                                                                            int enl_tile_idx = _enlargementDefs[enlargement].position2tile[orig_position_index];
                                                                            if (enl_tile_idx >= 0)
                                                                            {
                                                                                int addedindex = 0;
                                                                                if (_enlargementDefs[enlargement].number_of_animation_frames > 0)
                                                                                {
                                                                                    if (main_tile_idx == -1
                                                                                        && anim_frame_idx >= 0
                                                                                        && anim_frame_idx < _enlargementDefs[enlargement].number_of_animation_frames
                                                                                        )
                                                                                    {
                                                                                        addedindex = anim_frame_idx * _enlargementDefs[enlargement].number_of_enlargement_tiles;
                                                                                    }
                                                                                    else if (main_tile_idx == 0
                                                                                        && anim_frame_idx > 0
                                                                                        && anim_frame_idx <= _enlargementDefs[enlargement].number_of_animation_frames)
                                                                                    {
                                                                                        addedindex = (anim_frame_idx - 1) * _enlargementDefs[enlargement].number_of_enlargement_tiles;
                                                                                    }
                                                                                    else if (main_tile_idx == _enlargementDefs[enlargement].number_of_animation_frames
                                                                                        && anim_frame_idx >= 0
                                                                                        && anim_frame_idx < _enlargementDefs[enlargement].number_of_animation_frames
                                                                                        )
                                                                                    {
                                                                                        addedindex = anim_frame_idx * _enlargementDefs[enlargement].number_of_enlargement_tiles;
                                                                                    }
                                                                                }
                                                                                int enl_glyph = enl_tile_idx + addedindex + EnlargementOffsets[enlargement] + EnlargementOff;
                                                                                ntile = Glyph2Tile[enl_glyph]; /* replace */
                                                                                autodraw = Tile2Autodraw[ntile];
                                                                            }
                                                                            else
                                                                                continue;
                                                                        }

                                                                        int dx = 0, dy = 0;
                                                                        int darken_dx = 0, darken_dy = 0;
                                                                        switch (position_index)
                                                                        {
                                                                            case 0:
                                                                                dx = -1;
                                                                                dy = -1;
                                                                                break;
                                                                            case 1:
                                                                                dx = 0;
                                                                                dy = -1;
                                                                                break;
                                                                            case 2:
                                                                                dx = 1;
                                                                                dy = -1;
                                                                                break;
                                                                            case 3:
                                                                                dx = -1;
                                                                                dy = 0;
                                                                                break;
                                                                            case 4:
                                                                                dx = 1;
                                                                                dy = 0;
                                                                                break;
                                                                        }

                                                                        int draw_map_x = mapx + dx + (adj_x - mapx);
                                                                        int draw_map_y = mapy + dy + (adj_y - mapy);
                                                                        if (!GHUtils.isok(draw_map_x, draw_map_y))
                                                                            continue;

                                                                        darken_dx = dx;
                                                                        darken_dy = 0;
                                                                        //int darken_x = mapx + darken_dx;
                                                                        //int darken_y = mapy + darken_dy;
                                                                        //bool darken = ((_mapData[darken_x, darken_y].Layers.layer_flags & (ulong)LayerFlags.LFLAGS_CAN_SEE) == 0);
                                                                        //if (_mapData[mapx, mapy].Layers.layer_glyphs != null
                                                                        //    && (_mapData[mapx, mapy].Layers.layer_glyphs[(int)layer_types.LAYER_FLOOR] == UnexploredGlyph
                                                                        //        || _mapData[mapx, mapy].Layers.layer_glyphs[(int)layer_types.LAYER_FLOOR] == NoGlyph)
                                                                        //   )
                                                                        //    darken = false;

                                                                        if (dx != 0 || dy != 0)
                                                                            draw_shadow[draw_map_x, draw_map_y] = true;

                                                                        int sheet_idx = TileSheetIdx(ntile);
                                                                        int tile_x = TileSheetX(ntile);
                                                                        int tile_y = TileSheetY(ntile);

                                                                        SKRect sourcerect;
                                                                        float scaled_tile_width = width;
                                                                        float scaled_tile_height = tileflag_halfsize || (tileflag_normalobjmissile && !tileflag_fullsizeditem) ? height / 2 : height;
                                                                        float scaled_x_padding = 0;
                                                                        float scaled_y_padding = 0;
                                                                        int source_y_added = 0;
                                                                        int source_height_deducted = 0;
                                                                        int source_height = tileflag_halfsize ? GHConstants.TileHeight / 2 : GHConstants.TileHeight;

                                                                        float scale = 1.0f;
                                                                        if (tileflag_halfsize)
                                                                        {
                                                                            if ((layer_idx == (int)layer_types.LAYER_OBJECT || layer_idx == (int)layer_types.LAYER_COVER_OBJECT))
                                                                            {
                                                                                if (obj_in_pit)
                                                                                    scale *= GHConstants.OBJECT_PIT_SCALING_FACTOR;
                                                                            }

                                                                            if (monster_height < 0 && (layer_idx == (int)layer_types.LAYER_MONSTER || layer_idx == (int)layer_types.LAYER_MONSTER_EFFECT) || layer_idx == (int)layer_types.MAX_LAYERS)
                                                                                scale *= Math.Min(1.0f, Math.Max(0.1f, 1.0f - (1.0f - (float)GHConstants.OBJECT_PIT_SCALING_FACTOR) * (float)monster_height / (float)GHConstants.SPECIAL_HEIGHT_IN_PIT));

                                                                            if (tileflag_floortile)
                                                                            {
                                                                                if ((layer_idx == (int)layer_types.LAYER_OBJECT || layer_idx == (int)layer_types.LAYER_OBJECT)
                                                                                    && obj_height > 0 && obj_height < 48)
                                                                                {
                                                                                    source_y_added = (GHConstants.TileHeight / 2 - obj_height) / 2;
                                                                                    source_height_deducted = GHConstants.TileHeight / 2 - obj_height;
                                                                                    source_height = GHConstants.TileHeight / 2 - source_height_deducted;
                                                                                    scaled_tile_width = scale * width;
                                                                                    scaled_x_padding = (width - scaled_tile_width) / 2;
                                                                                    scaled_tile_height = scale * (float)source_height * height / (float)GHConstants.TileHeight;
                                                                                    scaled_y_padding = Math.Max(0, scale * (float)source_height_deducted * height / (float)GHConstants.TileHeight - pit_border);
                                                                                }
                                                                                sourcerect = new SKRect(tile_x, tile_y + source_y_added, tile_x + GHConstants.TileWidth, tile_y + source_y_added + source_height);
                                                                            }
                                                                            else
                                                                            {
                                                                                if ((layer_idx == (int)layer_types.LAYER_OBJECT || layer_idx == (int)layer_types.LAYER_COVER_OBJECT))
                                                                                {
                                                                                    if (obj_height > 0 && obj_height < 48)
                                                                                        scale *= ((float)obj_height) / 48.0f;
                                                                                }
                                                                                scaled_tile_width = scale * width;
                                                                                scaled_tile_height = scale * height / 2;
                                                                                scaled_x_padding = (width - scaled_tile_width) / 2;
                                                                                scaled_y_padding = Math.Max(0, height / 2 - scaled_tile_height - pit_border);

                                                                                sourcerect = new SKRect(tile_x, tile_y + GHConstants.TileHeight / 2, tile_x + GHConstants.TileWidth, tile_y + GHConstants.TileHeight);
                                                                            }
                                                                        }
                                                                        else
                                                                        {
                                                                            if (tileflag_normalobjmissile && !tileflag_fullsizeditem)
                                                                            {
                                                                                if (tileflag_floortile)
                                                                                {
                                                                                    sourcerect = new SKRect(tile_x, tile_y, tile_x + GHConstants.TileWidth, tile_y + GHConstants.TileHeight / 2);
                                                                                }
                                                                                else
                                                                                {
                                                                                    if (missile_height > 0 && missile_height < 48)
                                                                                    {
                                                                                        scale = ((float)missile_height) / 48.0f;
                                                                                    }
                                                                                    scaled_tile_width = scale * width;
                                                                                    scaled_tile_height = scale * height / 2;
                                                                                    scaled_x_padding = (width - scaled_tile_width) / 2;
                                                                                    scaled_y_padding = (height / 2 - scaled_tile_height) / 2;

                                                                                    sourcerect = new SKRect(tile_x, tile_y + GHConstants.TileHeight / 2, tile_x + GHConstants.TileWidth, tile_y + GHConstants.TileHeight);
                                                                                }
                                                                            }
                                                                            else
                                                                            {
                                                                                if (monster_height < 0 && dy == 0)
                                                                                {
                                                                                    sourcerect = new SKRect(tile_x, tile_y, tile_x + GHConstants.TileWidth, tile_y + GHConstants.TileHeight + monster_height);
                                                                                    source_height_deducted = -monster_height;
                                                                                    source_height = GHConstants.TileHeight - source_height_deducted;
                                                                                    scaled_tile_height = (float)source_height * height / (float)GHConstants.TileHeight;
                                                                                }
                                                                                else
                                                                                {
                                                                                    sourcerect = new SKRect(tile_x, tile_y, tile_x + GHConstants.TileWidth, tile_y + GHConstants.TileHeight);
                                                                                }
                                                                            }
                                                                        }

                                                                        float move_offset_x = 0, move_offset_y = 0;
                                                                        float opaqueness = 1.0f;
                                                                        if ((layer_idx == (int)layer_types.LAYER_MONSTER || layer_idx == (int)layer_types.LAYER_MONSTER_EFFECT || layer_idx == (int)layer_types.MAX_LAYERS))
                                                                        {
                                                                            move_offset_x = base_move_offset_x;
                                                                            move_offset_y = base_move_offset_y;
                                                                            if (layer_idx == (int)layer_types.MAX_LAYERS 
                                                                                || ((_mapData[mapx, mapy].Layers.monster_flags & (ulong)(LayerMonsterFlags.LMFLAGS_INVISIBLE_TRANSPARENT | LayerMonsterFlags.LMFLAGS_SEMI_TRANSPARENT | LayerMonsterFlags.LMFLAGS_RADIAL_TRANSPARENCY)) != 0)
                                                                                )
                                                                                opaqueness = 0.5f;
                                                                        }
                                                                        else if (layer_idx == (int)layer_types.LAYER_COVER_TRAP)
                                                                        {
                                                                            opaqueness = 0.5f;
                                                                        }

                                                                        tx = (offsetX + _mapOffsetX + move_offset_x + width * (float)draw_map_x);
                                                                        ty = (offsetY + _mapOffsetY + move_offset_y + scaled_y_height_change + _mapFontAscent + height * (float)draw_map_y);

                                                                        using (new SKAutoCanvasRestore(canvas, true))
                                                                        {
                                                                            canvas.Translate(tx + (hflip_glyph ? width : 0), ty + (vflip_glyph ? height : 0));
                                                                            canvas.Scale(hflip_glyph ? -1 : 1, vflip_glyph ? -1 : 1, 0, 0);
                                                                            SKRect targetrect;
                                                                            if (tileflag_halfsize)
                                                                            {
                                                                                targetrect = new SKRect(scaled_x_padding, height / 2 + scaled_y_padding, scaled_x_padding + scaled_tile_width, height / 2 + scaled_y_padding + scaled_tile_height);
                                                                            }
                                                                            else
                                                                            {
                                                                                if (tileflag_normalobjmissile && !tileflag_fullsizeditem)
                                                                                    targetrect = new SKRect(scaled_x_padding, height / 4 + scaled_y_padding, scaled_x_padding + scaled_tile_width, height / 4 + scaled_y_padding + scaled_tile_height);
                                                                                else
                                                                                    targetrect = new SKRect(scaled_x_padding, scaled_y_padding, scaled_x_padding + scaled_tile_width, scaled_y_padding + scaled_tile_height);
                                                                            }
                                                                            paint.Color = paint.Color.WithAlpha((byte)(0xFF * opaqueness));
                                                                            canvas.DrawBitmap(TileMap[sheet_idx], sourcerect, targetrect, paint);
                                                                        }

                                                                        /************/
                                                                        /* AUTODRAW */
                                                                        /************/
                                                                        if (_autodraws != null)
                                                                        {
                                                                            if (_autodraws[autodraw].draw_type == (int)autodraw_drawing_types.AUTODRAW_DRAW_LONG_WORM)
                                                                            {
                                                                                /* Long worm here */

                                                                                int source_glyph_seg_end = _autodraws[autodraw].source_glyph;
                                                                                int source_glyph_seg_dir_out = _autodraws[autodraw].source_glyph2;
                                                                                int source_glyph_seg_dir_in = _autodraws[autodraw].source_glyph2 + 4;
                                                                                int source_glyph_seg_layer = _autodraws[autodraw].source_glyph3;
                                                                                int drawing_tail = _autodraws[autodraw].flags;
                                                                                int wdir_out = _mapData[mapx, mapy].Layers.wsegdir;
                                                                                int wdir_in = _mapData[mapx, mapy].Layers.reverse_prev_wsegdir;
                                                                                bool is_head = (_mapData[mapx, mapy].Layers.monster_flags & (ulong)LayerMonsterFlags.LMFLAGS_WORM_HEAD) != 0;
                                                                                bool is_tailend = (_mapData[mapx, mapy].Layers.monster_flags & (ulong)LayerMonsterFlags.LMFLAGS_WORM_TAILEND) != 0;
                                                                                for (int wlayer = 0; wlayer < 5; wlayer++)
                                                                                {
                                                                                    int source_glyph = NoGlyph;
                                                                                    bool hflip_seg = false;
                                                                                    bool vflip_seg = false;
                                                                                    switch (wlayer)
                                                                                    {
                                                                                        case 0:
                                                                                        case 2:
                                                                                        case 4:
                                                                                            if (is_head || is_tailend)
                                                                                                continue;
                                                                                            source_glyph = source_glyph_seg_layer + wlayer / 2;
                                                                                            break;
                                                                                        case 1:
                                                                                            source_glyph = is_tailend ? NoGlyph : is_head ? source_glyph_seg_end : source_glyph_seg_dir_in;
                                                                                            break;
                                                                                        case 3:
                                                                                            source_glyph = is_tailend ? source_glyph_seg_end : is_head ? NoGlyph : source_glyph_seg_dir_out;
                                                                                            break;
                                                                                        default:
                                                                                            break;
                                                                                    }

                                                                                    if (source_glyph != NoGlyph)
                                                                                    {
                                                                                        int wdir = (wlayer == 1 ? wdir_in : wlayer == 3 ? wdir_out : 0);
                                                                                        switch (wdir)
                                                                                        {
                                                                                            case 1:
                                                                                                source_glyph += 2;
                                                                                                hflip_seg = false;
                                                                                                vflip_seg = false;
                                                                                                break;
                                                                                            case 2:
                                                                                                source_glyph += 0;
                                                                                                hflip_seg = false;
                                                                                                vflip_seg = false;
                                                                                                break;
                                                                                            case 3:
                                                                                                source_glyph += 3;
                                                                                                hflip_seg = false;
                                                                                                vflip_seg = true;
                                                                                                break;
                                                                                            case 4:
                                                                                                source_glyph += 1;
                                                                                                hflip_seg = true;
                                                                                                vflip_seg = false;
                                                                                                break;
                                                                                            case 5:
                                                                                                source_glyph += 3;
                                                                                                hflip_seg = false;
                                                                                                vflip_seg = false;
                                                                                                break;
                                                                                            case 6:
                                                                                                source_glyph += 0;
                                                                                                hflip_seg = false;
                                                                                                vflip_seg = true;
                                                                                                break;
                                                                                            case 7:
                                                                                                source_glyph += 2;
                                                                                                hflip_seg = false;
                                                                                                vflip_seg = true;
                                                                                                break;
                                                                                            case 8:
                                                                                                source_glyph += 1;
                                                                                                hflip_seg = false;
                                                                                                vflip_seg = false;
                                                                                                break;
                                                                                            default:
                                                                                                break;
                                                                                        }

                                                                                        int atile = Glyph2Tile[source_glyph];
                                                                                        int a_sheet_idx = TileSheetIdx(atile);
                                                                                        int at_x = TileSheetX(atile);
                                                                                        int at_y = TileSheetY(atile);

                                                                                        int worm_source_x = at_x;
                                                                                        int worm_source_y = at_y;
                                                                                        int worm_source_width = GHConstants.TileWidth;
                                                                                        int worm_source_height = GHConstants.TileHeight;
                                                                                        sourcerect = new SKRect(worm_source_x, worm_source_y, worm_source_x + worm_source_width, worm_source_y + worm_source_height);

                                                                                        float target_x = tx;
                                                                                        float target_y = ty;
                                                                                        float target_width = ((float)worm_source_width * targetscale);
                                                                                        float target_height = ((float)worm_source_height * targetscale);
                                                                                        SKRect targetrect;
                                                                                        targetrect = new SKRect(0, 0, target_width, target_height);

                                                                                        using (new SKAutoCanvasRestore(canvas, true))
                                                                                        {
                                                                                            canvas.Translate(target_x + (hflip_seg ? width : 0), target_y + (vflip_seg ? height : 0));
                                                                                            canvas.Scale(hflip_seg ? -1 : 1, vflip_seg ? -1 : 1, 0, 0);
                                                                                            paint.Color = paint.Color.WithAlpha((byte)(0xFF * opaqueness));
                                                                                            canvas.DrawBitmap(TileMap[sheet_idx], sourcerect, targetrect, paint);
                                                                                        }
                                                                                    }
                                                                                }
                                                                            }
                                                                            else if (_autodraws[autodraw].draw_type == (int)autodraw_drawing_types.AUTODRAW_DRAW_BOOKSHELF_CONTENTS && otmp_round != null && otmp_round.ContainedObjs != null)
                                                                            {
                                                                                int num_shelves = 4;
                                                                                int y_to_first_shelf = 49;
                                                                                int shelf_start = 8;
                                                                                int shelf_width = 50;
                                                                                int shelf_height = 10;
                                                                                int shelf_border_height = 2;
                                                                                int shelf_item_width = 5;
                                                                                int src_book_x = 0;
                                                                                int src_book_y = 0;
                                                                                int src_scroll_x = 5;
                                                                                int src_scroll_y = 0;
                                                                                int cnt = 0;
                                                                                int items_per_row = shelf_width / shelf_item_width;
                                
                                                                                foreach(ObjectDataItem contained_obj in otmp_round.ContainedObjs)
                                                                                {
                                                                                    int src_x = 0, src_y = 0;
                                                                                    float dest_x = 0, dest_y = 0;
                                                                                    if (contained_obj.ObjData.oclass == (int)obj_class_types.SPBOOK_CLASS)
                                                                                    {
                                                                                        src_x = src_book_x;
                                                                                        src_y = src_book_y;
                                                                                    }
                                                                                    else if (contained_obj.ObjData.oclass == (int)obj_class_types.SCROLL_CLASS)
                                                                                    {
                                                                                        src_x = src_scroll_x;
                                                                                        src_y = src_scroll_y;
                                                                                    }
                                                                                    else
                                                                                        continue;

                                                                                    for (int item_idx = 0; item_idx < contained_obj.ObjData.quan; item_idx++)
                                                                                    {
                                                                                        int item_row = cnt / items_per_row;
                                                                                        int item_xpos = cnt % items_per_row;

                                                                                        if (item_row >= num_shelves)
                                                                                            break;

                                                                                        dest_y = (y_to_first_shelf + item_row * (shelf_height + shelf_border_height)) * scale * targetscale;
                                                                                        dest_x = (shelf_start + item_xpos * shelf_item_width) * scale * targetscale;

                                                                                        int source_glyph = _autodraws[autodraw].source_glyph;
                                                                                        int atile = Glyph2Tile[source_glyph];
                                                                                        int a_sheet_idx = TileSheetIdx(atile);
                                                                                        int at_x = TileSheetX(atile);
                                                                                        int at_y = TileSheetY(atile);

                                                                                        SKRect source_rt = new SKRect();
                                                                                        source_rt.Left = at_x + src_x;
                                                                                        source_rt.Right = source_rt.Left + shelf_item_width;
                                                                                        source_rt.Top = at_y + src_y;
                                                                                        source_rt.Bottom = source_rt.Top + shelf_height;

                                                                                        float target_x = tx + dest_x;
                                                                                        float target_y = ty + dest_y;
                                                                                        float target_width = targetscale * scale * source_rt.Width;
                                                                                        float target_height = targetscale * scale * source_rt.Height;
                                                                                        SKRect target_rt;
                                                                                        target_rt = new SKRect(0, 0, target_width, target_height);

                                                                                        using (new SKAutoCanvasRestore(canvas, true))
                                                                                        {
                                                                                            canvas.Translate(target_x, target_y);
                                                                                            canvas.Scale(1, 1, 0, 0);
                                                                                            paint.Color = paint.Color.WithAlpha((byte)(0xFF * opaqueness));
                                                                                            canvas.DrawBitmap(TileMap[a_sheet_idx], source_rt, target_rt, paint);
                                                                                        }

                                                                                        cnt++;
                                                                                    }
                                                                                }
                                                                            }
                                                                            else if (_autodraws[autodraw].draw_type == (int)autodraw_drawing_types.AUTODRAW_DRAW_WEAPON_RACK_CONTENTS && otmp_round != null && otmp_round.ContainedObjs != null)
                                                                            {
                                                                                int y_to_rack_top = 31;
                                                                                int rack_start = 0; /* Assume weapons are drawn reasonably well in the center */
                                                                                int rack_width = 48;
                                                                                int rack_height = GHConstants.TileHeight - y_to_rack_top;
                                                                                int rack_item_spacing = 6;

                                                                                int cnt = 0;

                                                                                foreach (ObjectDataItem contained_obj in otmp_round.ContainedObjs)
                                                                                {
                                                                                    int source_glyph = Math.Abs(contained_obj.ObjData.gui_glyph);
                                                                                    if (source_glyph <= 0 || source_glyph == NoGlyph)
                                                                                        continue;
                                                                                    bool has_floor_tile = (GlyphTileFlags[source_glyph] & (byte)glyph_tile_flags.GLYPH_TILE_FLAG_HAS_FLOOR_TILE) != 0; // artidx > 0 ? has_artifact_floor_tile(artidx) : has_obj_floor_tile(contained_obj);
                                                                                    bool fullsizeditem = (GlyphTileFlags[source_glyph] & (byte)glyph_tile_flags.GLYPH_TILE_FLAG_FULL_SIZED_ITEM) != 0;
                                                                                    int cobj_height = contained_obj.OtypData.tile_height; // artidx ? artilist[artidx].tile_floor_height : OBJ_TILE_HEIGHT(contained_obj->otyp);
                                                                                    int artidx = contained_obj.ObjData.oartifact;
                                                                                    float dest_x = 0, dest_y = 0;
                                                                                    int src_x = 0, src_y = fullsizeditem || has_floor_tile ? 0 : GHConstants.TileHeight / 2;
                                                                                    int item_width = has_floor_tile ? GHConstants.TileHeight / 2 : cobj_height > 0 ? cobj_height : GHConstants.TileHeight / 2;
                                                                                    int item_height = has_floor_tile ? GHConstants.TileWidth : (item_width * GHConstants.TileWidth) / (GHConstants.TileHeight / 2);
                                                                                    int padding = (GHConstants.TileHeight / 2 - item_width) / 2;
                                                                                    int vertical_padding = (GHConstants.TileWidth - item_height) / 2;
                                                                                    if (contained_obj.ObjData.oclass != (int)obj_class_types.WEAPON_CLASS)
                                                                                        continue;

                                                                                    int item_xpos = cnt / 2 * rack_item_spacing;
                                                                                    if (item_xpos >= rack_width / 2)
                                                                                        break;

                                                                                    dest_y = (y_to_rack_top + vertical_padding) * scale * targetscale;
                                                                                    dest_x = (cnt % 2 == 0 ? rack_start + item_xpos + padding : GHConstants.TileWidth - item_width - rack_start - item_xpos - padding) * scale * targetscale;

                                                                                    int atile = Glyph2Tile[source_glyph];
                                                                                    int a_sheet_idx = TileSheetIdx(atile);
                                                                                    int at_x = TileSheetX(atile);
                                                                                    int at_y = TileSheetY(atile);

                                                                                    SKRect source_rt = new SKRect();
                                                                                    source_rt.Left = at_x + src_x;
                                                                                    source_rt.Right = source_rt.Left + GHConstants.TileWidth;
                                                                                    source_rt.Top = at_y + src_y;
                                                                                    source_rt.Bottom = source_rt.Top + (fullsizeditem ? GHConstants.TileHeight : GHConstants.TileHeight / 2);

                                                                                    float original_width = source_rt.Right - source_rt.Left;
                                                                                    float original_height = source_rt.Bottom - source_rt.Top;
                                                                                    float rotated_width = original_height;
                                                                                    float rotated_height = original_width;

                                                                                    float target_x = tx + dest_x;
                                                                                    float target_y = ty + dest_y;
                                                                                    float target_width = targetscale * scale * original_width; //(float)item_width;
                                                                                    float target_height = targetscale * scale * original_height; //((float)item_width * rotated_height) / rotated_width;
                                                                                    SKRect target_rt;
                                                                                    target_rt = new SKRect(0, 0, target_width, target_height);

                                                                                    using (new SKAutoCanvasRestore(canvas, true))
                                                                                    {
                                                                                        canvas.Translate(target_x, target_y);
                                                                                        canvas.Scale(1, 1, 0, 0);
                                                                                        canvas.RotateDegrees(-90);
                                                                                        canvas.Translate(-target_width, 0);
                                                                                        paint.Color = paint.Color.WithAlpha((byte)(0xFF * opaqueness));
                                                                                        canvas.DrawBitmap(TileMap[a_sheet_idx], source_rt, target_rt, paint);
                                                                                    }

                                                                                    cnt++;
                                                                                }
                                                                            }
                                                                            else if (_autodraws[autodraw].draw_type == (int)autodraw_drawing_types.AUTODRAW_DRAW_CANDELABRUM_CANDLES && otmp_round != null)
                                                                            {
                                                                                float y_start = scaled_y_padding;
                                                                                float x_start = scaled_x_padding;
                                                                                int x_padding = 13;
                                                                                int item_width = 6;
                                                                                int item_height = 13;
                                                                                int src_unlit_x = 0;
                                                                                int src_unlit_y = 10;
                                                                                int src_lit_x = 6 * (1 + (int)_autodraws[autodraw].flags);
                                                                                int src_lit_y = 10;
                                                                                int cnt = 0;

                                                                                for (int cidx = 0; cidx < Math.Min((short)7, otmp_round.ObjData.special_quality); cidx++)
                                                                                {
                                                                                    int src_x = 0, src_y = 0;
                                                                                    float dest_x = 0, dest_y = 0;
                                                                                    if (otmp_round.LampLit)
                                                                                    {
                                                                                        src_x = src_lit_x;
                                                                                        src_y = src_lit_y;
                                                                                    }
                                                                                    else
                                                                                    {
                                                                                        src_x = src_unlit_x;
                                                                                        src_y = src_unlit_y;
                                                                                    }

                                                                                    int item_xpos = cnt;

                                                                                    dest_y = y_start;
                                                                                    dest_x = x_start + ((float)(x_padding + item_xpos * item_width) * scale * targetscale);

                                                                                    int source_glyph = _autodraws[autodraw].source_glyph;
                                                                                    int atile = Glyph2Tile[source_glyph];
                                                                                    int a_sheet_idx = TileSheetIdx(atile);
                                                                                    int at_x = TileSheetX(atile);
                                                                                    int at_y = TileSheetY(atile);

                                                                                    SKRect source_rt = new SKRect();
                                                                                    source_rt.Left = at_x + src_x;
                                                                                    source_rt.Right = source_rt.Left + item_width;
                                                                                    source_rt.Top = at_y + src_y;
                                                                                    source_rt.Bottom = source_rt.Top + item_height;

                                                                                    float target_x = tx + dest_x;
                                                                                    float target_y = ty + dest_y;
                                                                                    float target_width = targetscale * scale * source_rt.Width;
                                                                                    float target_height = targetscale * scale * source_rt.Height;
                                                                                    SKRect target_rt;
                                                                                    target_rt = new SKRect(0, 0, target_width, target_height);

                                                                                    using (new SKAutoCanvasRestore(canvas, true))
                                                                                    {
                                                                                        canvas.Translate(target_x, target_y);
                                                                                        canvas.Scale(1, 1, 0, 0);
                                                                                        paint.Color = paint.Color.WithAlpha((byte)(0xFF * opaqueness));
                                                                                        canvas.DrawBitmap(TileMap[a_sheet_idx], source_rt, target_rt, paint);
                                                                                    }

                                                                                    cnt++;
                                                                                }
                                                                            }
                                                                            else if (_autodraws[autodraw].draw_type == (int)autodraw_drawing_types.AUTODRAW_DRAW_LARGE_FIVE_BRANCHED_CANDELABRUM_CANDLES && otmp_round != null)
                                                                            {
                                                                                float y_start = scaled_y_padding;
                                                                                float x_start = scaled_x_padding;
                                                                                int item_width = 9;
                                                                                int item_height = 31;
                                                                                int src_unlit_x = 0;
                                                                                int src_unlit_y = 0;
                                                                                int src_lit_x = 9 * (1 + (int)_autodraws[autodraw].flags);
                                                                                int src_lit_y = 0;
                                                                                int cnt = 0;

                                                                                for (int cidx = 0; cidx < Math.Min((short)otmp_round.OtypData.special_quality, otmp_round.ObjData.special_quality); cidx++)
                                                                                {
                                                                                    int src_x = 0, src_y = 0;
                                                                                    float dest_x = 0, dest_y = 0;
                                                                                    if (otmp_round.LampLit)
                                                                                    {
                                                                                        src_x = src_lit_x;
                                                                                        src_y = src_lit_y;
                                                                                    }
                                                                                    else
                                                                                    {
                                                                                        src_x = src_unlit_x;
                                                                                        src_y = src_unlit_y;
                                                                                    }

                                                                                    switch (cidx)
                                                                                    {
                                                                                        case 0:
                                                                                            dest_x = x_start + ((float)(29) * scale * targetscale);
                                                                                            dest_y = y_start + ((float)(0) * scale * targetscale);
                                                                                            break;
                                                                                        case 1:
                                                                                            dest_x = x_start + ((float)(18) * scale * targetscale);
                                                                                            dest_y = y_start + ((float)(4) * scale * targetscale);
                                                                                            break;
                                                                                        case 2:
                                                                                            dest_x = x_start + ((float)(40) * scale * targetscale);
                                                                                            dest_y = y_start + ((float)(3) * scale * targetscale);
                                                                                            break;
                                                                                        case 3:
                                                                                            dest_x = x_start + ((float)(8) * scale * targetscale);
                                                                                            dest_y = y_start + ((float)(14) * scale * targetscale);
                                                                                            break;
                                                                                        case 4:
                                                                                            dest_x = x_start + ((float)(50) * scale * targetscale);
                                                                                            dest_y = y_start + ((float)(15) * scale * targetscale);
                                                                                            break;
                                                                                        default:
                                                                                            break;
                                                                                    }

                                                                                    int source_glyph = _autodraws[autodraw].source_glyph;
                                                                                    int atile = Glyph2Tile[source_glyph];
                                                                                    int a_sheet_idx = TileSheetIdx(atile);
                                                                                    int at_x = TileSheetX(atile);
                                                                                    int at_y = TileSheetY(atile);

                                                                                    SKRect source_rt = new SKRect();
                                                                                    source_rt.Left = at_x + src_x;
                                                                                    source_rt.Right = source_rt.Left + item_width;
                                                                                    source_rt.Top = at_y + src_y;
                                                                                    source_rt.Bottom = source_rt.Top + item_height;

                                                                                    float target_x = tx + dest_x;
                                                                                    float target_y = ty + dest_y;
                                                                                    float target_width = targetscale * scale * source_rt.Width;
                                                                                    float target_height = targetscale * scale * source_rt.Height;
                                                                                    SKRect target_rt;
                                                                                    target_rt = new SKRect(0, 0, target_width, target_height);

                                                                                    using (new SKAutoCanvasRestore(canvas, true))
                                                                                    {
                                                                                        canvas.Translate(target_x, target_y);
                                                                                        canvas.Scale(1, 1, 0, 0);
                                                                                        paint.Color = paint.Color.WithAlpha((byte)(0xFF * opaqueness));
                                                                                        canvas.DrawBitmap(TileMap[a_sheet_idx], source_rt, target_rt, paint);
                                                                                    }
                                                                                    cnt++;
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }

                                            /* Darkening at the end of layers */
                                            if (layer_idx == (int)layer_types.LAYER_OBJECT)
                                            {
                                                for (int mapx = startX; mapx <= endX; mapx++)
                                                {
                                                    for (int mapy = startY; mapy <= endY; mapy++)
                                                    {
                                                        bool darken = ((_mapData[mapx, mapy].Layers.layer_flags & (ulong)LayerFlags.LFLAGS_CAN_SEE) == 0);

                                                        if (_mapData[mapx, mapy].Layers.layer_glyphs != null
                                                            && (_mapData[mapx, mapy].Layers.layer_glyphs[(int)layer_types.LAYER_FLOOR] == UnexploredGlyph
                                                                || _mapData[mapx, mapy].Layers.layer_glyphs[(int)layer_types.LAYER_FLOOR] == NoGlyph)
                                                            )
                                                            darken = false;

                                                        // Draw rectangle with blend mode in bottom half
                                                        if (darken)
                                                        {
                                                            bool uloc = ((_mapData[mapx, mapy].Layers.layer_flags & (ulong)LayerFlags.LFLAGS_UXUY) != 0);
                                                            bool unlit = ((_mapData[mapx, mapy].Layers.layer_flags & (ulong)LayerFlags.LFLAGS_APPEARS_UNLIT) != 0);
                                                            // Get values from XAML controls
                                                            SKBlendMode blendMode = SKBlendMode.Modulate;
                                                            int val = ((uloc ? 85 : unlit ? 35 : 65) * 255) / 100;
                                                            SKColor color = new SKColor((byte)val, (byte)val, (byte)val);

                                                            paint.Color = color;
                                                            SKBlendMode old_bm = paint.BlendMode;
                                                            paint.BlendMode = blendMode;
                                                            tx = (offsetX + _mapOffsetX + width * (float)mapx);
                                                            ty = (offsetY + _mapOffsetY + _mapFontAscent + height * (float)mapy);
                                                            SKRect targetrect = new SKRect(tx, ty, tx + width, ty + height);
                                                            canvas.DrawRect(targetrect, paint);
                                                            paint.BlendMode = old_bm;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }

                        /* Cursor */
                        if (GraphicsStyle == GHGraphicsStyle.ASCII && CursorStyle == TTYCursorStyle.BlinkingUnderline && _cursorIsOn && _mapCursorX >= 1 && _mapCursorY >= 0)
                        {
                            int cx = _mapCursorX, cy = _mapCursorY;
                            str = "_";
                            textPaint.Color = SKColors.White;
                            tx = (offsetX + _mapOffsetX + width * (float)cx);
                            ty = (offsetY + _mapOffsetY + height * (float)cy);
                            canvas.DrawText(str, tx, ty, textPaint);
                        }
                    }
                }

                /* Floating Texts */
                if (GraphicsStyle != GHGraphicsStyle.ASCII)
                {
                    lock (_floatingTextLock)
                    {
                        foreach (GHFloatingText ft in _floatingTexts)
                        {
                            SKPoint p;
                            float relativestrokewidth = 0.0f;
                            SKColor strokecolor = SKColors.White;
                            lock (AnimationTimerLock)
                            {
                                p = ft.GetPosition(AnimationTimers.general_animation_counter);
                                textPaint.Color = ft.GetColor(AnimationTimers.general_animation_counter);
                                textPaint.Typeface = ft.GetTypeface(AnimationTimers.general_animation_counter);
                                textPaint.TextSize = UsedFontSize * ft.GetRelativeTextSize(AnimationTimers.general_animation_counter);
                                relativestrokewidth = ft.GetRelativeOutlineWidth(AnimationTimers.general_animation_counter);
                                strokecolor = ft.GetOutlineColor(AnimationTimers.general_animation_counter);
                                str = ft.GetText(AnimationTimers.general_animation_counter);
                            }
                            textPaint.MeasureText(str, ref textBounds);
                            tx = (offsetX + _mapOffsetX + width * p.X - textBounds.Width / 2);
                            ty = (offsetY + _mapOffsetY + height * p.Y - textBounds.Height / 2);
                            canvas.DrawText(str, tx, ty, textPaint);
                            if (relativestrokewidth > 0)
                            {
                                textPaint.Style = SKPaintStyle.Stroke;
                                textPaint.StrokeWidth = textPaint.TextSize * relativestrokewidth;
                                textPaint.Color = strokecolor;
                                canvas.DrawText(str, tx, ty, textPaint);
                                textPaint.Style = SKPaintStyle.Fill;
                            }
                        }
                    }
                    lock (_screenTextLock)
                    {
                        if (_screenText != null)
                        {
                            float targetwidth = 0, yoffsetpct = 0, relativestrokewidth = 0, relativesubstrokewidth = 0;
                            SKColor strokecolor = SKColors.White, substrokecolor = SKColors.White;
                            float maxfontsize = 9999.0f;
                            lock (AnimationTimerLock)
                            {
                                double canvasheightscale = this.Height / canvasView.Height; 
                                textPaint.Color = _screenText.GetTextColor(AnimationTimers.general_animation_counter);
                                textPaint.Typeface = _screenText.GetTextTypeface(AnimationTimers.general_animation_counter);
                                targetwidth = Math.Min(canvaswidth, canvasheight * (float)canvasheightscale) * _screenText.GetMainTextSizeRelativeToScreenWidth(AnimationTimers.general_animation_counter);
                                maxfontsize = _screenText.GetMainTextMaxFontSize(AnimationTimers.general_animation_counter);
                                yoffsetpct = _screenText.GetYOffsetPctOfScreen(AnimationTimers.general_animation_counter);
                                relativestrokewidth = _screenText.GetRelativeTextOutlineWidth(AnimationTimers.general_animation_counter);
                                strokecolor = _screenText.GetTextOutlineColor(AnimationTimers.general_animation_counter);
                                str = _screenText.GetText(AnimationTimers.general_animation_counter);
                            }
                            textPaint.TextSize = UsedFontSize;
                            textPaint.MeasureText(str, ref textBounds);
                            if (textBounds.Width > 0)
                            {
                                float relativesize = targetwidth / textBounds.Width;
                                //if (relativesize > maxfontsize)
                                //    relativesize = maxfontsize;
                                textPaint.TextSize = UsedFontSize * relativesize;
                            }

                            textPaint.MeasureText(str, ref textBounds);
                            float maintextascent = textPaint.FontMetrics.Ascent;
                            float maintextdescent = textPaint.FontMetrics.Descent;

                            tx = (canvaswidth / 2 - textBounds.Width / 2);
                            ty = (canvasheight / 2 - textBounds.Height / 2 - (maintextascent + maintextdescent) / 2) + yoffsetpct * canvasheight;

                            /* Shadow first */
                            {
                                SKColor oldcolor = textPaint.Color;
                                SKMaskFilter oldfilter = textPaint.MaskFilter;
                                textPaint.Color = SKColors.Black.WithAlpha(oldcolor.Alpha);
                                textPaint.MaskFilter = _blur;
                                float offset = textPaint.TextSize / 15;
                                canvas.DrawText(str, tx + offset, ty + offset, textPaint);
                                textPaint.Color = oldcolor;
                                textPaint.MaskFilter = oldfilter;
                            }

                            canvas.DrawText(str, tx, ty, textPaint);

                            if (relativestrokewidth > 0)
                            {
                                textPaint.Style = SKPaintStyle.Stroke;
                                textPaint.StrokeWidth = textPaint.TextSize * relativestrokewidth;
                                textPaint.Color = strokecolor;
                                canvas.DrawText(str, tx, ty, textPaint);
                                textPaint.Style = SKPaintStyle.Fill;
                            }


                            float maintextsize = textPaint.TextSize;
                            float maintextspacing = textPaint.FontSpacing;
                            float maintexty = ty;

                            if (_screenText.HasSubText)
                            {
                                lock (AnimationTimerLock)
                                {
                                    textPaint.Color = _screenText.GetSubTextColor(AnimationTimers.general_animation_counter);
                                    textPaint.Typeface = _screenText.GetSubTextTypeface(AnimationTimers.general_animation_counter);
                                    textPaint.TextSize = maintextsize * _screenText.GetSubTextSizeRelativeToMainText(AnimationTimers.general_animation_counter);
                                    relativesubstrokewidth = _screenText.GetRelativeSubTextOutlineWidth(AnimationTimers.general_animation_counter);
                                    substrokecolor = _screenText.GetSubTextOutlineColor(AnimationTimers.general_animation_counter);
                                    str = _screenText.GetSubText(AnimationTimers.general_animation_counter);
                                }
                                textPaint.MeasureText(str, ref textBounds);
                                tx = (canvaswidth / 2 - textBounds.Width / 2);
                                if (_screenText.IsSubTextAbove)
                                    ty = maintexty + maintextascent - textPaint.FontMetrics.Descent;
                                else
                                    ty = maintexty + maintextdescent - textPaint.FontMetrics.Ascent;

                                /* Shadow first */
                                {
                                    SKColor oldcolor = textPaint.Color;
                                    SKMaskFilter oldfilter = textPaint.MaskFilter;
                                    textPaint.Color = SKColors.Black.WithAlpha(oldcolor.Alpha);
                                    textPaint.MaskFilter = _blur;
                                    float offset = textPaint.TextSize / 15;
                                    canvas.DrawText(str, tx + offset, ty + offset, textPaint);
                                    textPaint.Color = oldcolor;
                                    textPaint.MaskFilter = oldfilter;
                                }

                                canvas.DrawText(str, tx, ty, textPaint);
                                if (relativesubstrokewidth > 0)
                                {
                                    textPaint.Style = SKPaintStyle.Stroke;
                                    textPaint.StrokeWidth = textPaint.TextSize * relativesubstrokewidth;
                                    textPaint.Color = substrokecolor;
                                    canvas.DrawText(str, tx, ty, textPaint);
                                    textPaint.Style = SKPaintStyle.Fill;
                                }
                            }
                        }
                    }
                }


                /* Window strings */
                canvasButtonRect.Top = 0; /* Maybe overrwritten below */
                canvasButtonRect.Bottom = canvasheight; /* Maybe overrwritten below */
                if (_clientGame != null)
                {
                    lock (_clientGame.WindowsLock)
                    {
                        for (int i = 0; _clientGame.Windows[i] != null && i < GHConstants.MaxGHWindows; i++)
                        {
                            if (_clientGame.Windows[i].Visible && (
                                _clientGame.Windows[i].WindowPrintStyle == GHWindowPrintLocations.PrintToMap
                                || _clientGame.Windows[i].WindowPrintStyle == GHWindowPrintLocations.RawPrint))
                            {
                                textPaint.Typeface = _clientGame.Windows[i].Typeface;
                                textPaint.TextSize = _clientGame.Windows[i].TextSize;
                                textPaint.Color = _clientGame.Windows[i].TextColor;
                                width = textPaint.FontMetrics.AverageCharacterWidth;
                                height = textPaint.FontSpacing; // textPaint.FontMetrics.Descent - textPaint.FontMetrics.Ascent;

                                if (_clientGame.Windows[i].AutoPlacement)
                                {
                                    if (_clientGame.Windows[i].WindowType == GHWinType.Message)
                                    {
                                        float newleft = 0;
                                        float newtop = canvasheight - height * _shownMessageRows - 30;
                                        _clientGame.Windows[i].Left = newleft;
                                        _clientGame.Windows[i].Top = newtop;
                                    }
                                    else if (_clientGame.Windows[i].WindowType == GHWinType.Here)
                                    {
                                        float newleft = 0;
                                        float messagetop = _clientGame.Windows[_clientGame.MessageWindowId].Top;
                                        float newtop = messagetop - _clientGame.Windows[i].Height - 30;
                                        _clientGame.Windows[i].Left = newleft;
                                        _clientGame.Windows[i].Top = newtop;
                                    }
                                }

                                SKRect winRect = new SKRect(_clientGame.Windows[i].Left, _clientGame.Windows[i].Top,
                                    _clientGame.Windows[i].Right,
                                    _clientGame.Windows[i].Bottom);

                                if (_clientGame.Windows[i].CenterHorizontally && winRect.Right - winRect.Left < canvaswidth)
                                {
                                    float newleft = (canvaswidth - (winRect.Right - winRect.Left)) / 2;
                                    float addition = newleft - winRect.Left;
                                    winRect.Left += addition;
                                    winRect.Right += addition;
                                }

                                using (SKPaint winPaint = new SKPaint())
                                {
                                    winPaint.FilterQuality = SKFilterQuality.None;

                                    winPaint.Color = _clientGame.Windows[i].BackgroundColor;
                                    winPaint.Style = SKPaintStyle.Fill;

                                    if (winPaint.Color != SKColors.Transparent)
                                        canvas.DrawRect(winRect, winPaint);

                                    if (i == _clientGame.StatusWindowId)
                                        canvasButtonRect.Top = winRect.Bottom;
                                    else if (i == _clientGame.MessageWindowId)
                                        canvasButtonRect.Bottom = winRect.Top;
                                }

                                if (_clientGame.Windows[i].WindowType != GHWinType.Message)
                                {
                                    lock (_clientGame.Windows[i].PutStrsLock)
                                    {
                                        int j = 0;
                                        foreach (GHPutStrItem putstritem in _clientGame.Windows[i].PutStrs)
                                        {
                                            int pos = 0;
                                            float xpos = 0;
                                            float totwidth = 0;
                                            foreach (GHPutStrInstructions instr in putstritem.InstructionList)
                                            {
                                                if (putstritem.Text == null)
                                                    str = "";
                                                else if (pos + instr.PrintLength <= putstritem.Text.Length)
                                                    str = putstritem.Text.Substring(pos, instr.PrintLength);
                                                else if (putstritem.Text.Length - pos > 0)
                                                    str = putstritem.Text.Substring(pos, putstritem.Text.Length - pos);
                                                else
                                                    str = "";
                                                pos += str.Length;
                                                textPaint.Color = ClientUtils.NHColor2SKColor(instr.Color < (int)nhcolor.CLR_MAX ? (nhcolor)instr.Color : nhcolor.CLR_WHITE);
                                                totwidth = textPaint.MeasureText(str, ref textBounds);

                                                /* attributes */
                                                tx = xpos + winRect.Left + _clientGame.Windows[i].Padding.Left;
                                                ty = winRect.Top + _clientGame.Windows[i].Padding.Top - textPaint.FontMetrics.Ascent + j * height;
                                                canvas.DrawText(str, tx, ty, textPaint);
                                                textPaint.Style = SKPaintStyle.Stroke;
                                                textPaint.StrokeWidth = _clientGame.Windows[i].StrokeWidth;
                                                textPaint.Color = SKColors.Black;
                                                canvas.DrawText(str, tx, ty, textPaint);
                                                textPaint.Style = SKPaintStyle.Fill;
                                                xpos += totwidth;
                                            }
                                            j++;
                                        }
                                    }
                                }

                                if (_clientGame.Windows[i].WindowType == GHWinType.Message)
                                {
                                    lock (msgHistoryLock)
                                    {
                                        if (_msgHistory != null)
                                        {
                                            int j = _shownMessageRows - 1, idx;
                                            float lineLengthLimit = 0.85f * canvaswidth;

                                            for (idx = _msgHistory.Count - 1; idx >= 0 && j >= 0; idx--)
                                            {
                                                GHMsgHistoryItem msgHistoryItem = _msgHistory[idx];
                                                longLine = msgHistoryItem.Text;
                                                SKColor printColor = ClientUtils.NHColor2SKColor(msgHistoryItem.NHColor < nhcolor.CLR_MAX ? msgHistoryItem.NHColor : nhcolor.CLR_WHITE);

                                                textPaint.Style = SKPaintStyle.Fill;
                                                textPaint.StrokeWidth = 0;
                                                textPaint.Color = printColor;
                                                /* attributes */

                                                wrappedLines.Clear();
                                                float lineLength = 0.0f;
                                                string line = "";
                                                foreach (string word in longLine.Split(' '))
                                                {
                                                    wordWithSpace = word + " ";
                                                    float wordWithSpaceLength = textPaint.MeasureText(wordWithSpace);
                                                    if (lineLength + wordWithSpaceLength > lineLengthLimit)
                                                    {
                                                        wrappedLines.Add(line);
                                                        line = "" + wordWithSpace;
                                                        lineLength = wordWithSpaceLength;
                                                    }
                                                    else
                                                    {
                                                        line += wordWithSpace;
                                                        lineLength += wordWithSpaceLength;
                                                    }
                                                }
                                                wrappedLines.Add(line);

                                                int lineidx;
                                                for (lineidx = 0; lineidx < wrappedLines.Count; lineidx++)
                                                {
                                                    string wrappedLine = wrappedLines[lineidx];
                                                    int window_row_idx = j + lineidx - wrappedLines.Count + 1;
                                                    if (window_row_idx < 0)
                                                        continue;
                                                    tx = winRect.Left + _clientGame.Windows[i].Padding.Left;
                                                    ty = winRect.Top + _clientGame.Windows[i].Padding.Top - textPaint.FontMetrics.Ascent + window_row_idx * height;
                                                    canvas.DrawText(wrappedLine, tx, ty, textPaint);
                                                    textPaint.Style = SKPaintStyle.Stroke;
                                                    textPaint.StrokeWidth = _clientGame.Windows[i].StrokeWidth;
                                                    textPaint.Color = SKColors.Black;
                                                    canvas.DrawText(wrappedLine, tx, ty, textPaint);
                                                    textPaint.Style = SKPaintStyle.Fill;
                                                    textPaint.StrokeWidth = 0;
                                                    textPaint.Color = printColor;
                                                }
                                                j -= wrappedLines.Count;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                }

                canvasButtonRect.Right = canvaswidth * (float)(0.8);
                canvasButtonRect.Left = canvaswidth * (float)(0.2);

                if (_showDirections || showNumberPad)
                {
                    textPaint.Color = new SKColor(255, 255, 255, 128);
                    textPaint.Typeface = App.DejaVuSansMonoTypeface;
                    textPaint.TextSize = showNumberPad ? 250 : 400;
                    for (int i = 0; i <= 9; i++)
                    {
                        switch (i)
                        {
                            case 0:
                                if (_showDirections)
                                    str = "\u2190";
                                else
                                    str = "4";
                                tx = canvasButtonRect.Left + canvasButtonRect.Width * 0.15f - textPaint.FontMetrics.AverageCharacterWidth / 2;
                                ty = canvasButtonRect.Top + canvasButtonRect.Height / 2 + textPaint.FontMetrics.Descent;
                                break;
                            case 1:
                                if (_showDirections)
                                    str = "\u2191";
                                else
                                    str = "8";
                                tx = canvasButtonRect.Left + canvasButtonRect.Width / 2 - textPaint.FontMetrics.AverageCharacterWidth / 2;
                                ty = canvasButtonRect.Top + canvasButtonRect.Height * 0.15f + textPaint.FontMetrics.Descent;
                                break;
                            case 2:
                                if (_showDirections)
                                    str = "\u2192";
                                else
                                    str = "6";
                                tx = canvasButtonRect.Left + canvasButtonRect.Width * 0.85f - textPaint.FontMetrics.AverageCharacterWidth / 2;
                                ty = canvasButtonRect.Top + canvasButtonRect.Height / 2 + textPaint.FontMetrics.Descent;
                                break;
                            case 3:
                                if (_showDirections)
                                    str = "\u2193";
                                else
                                    str = "2";
                                tx = canvasButtonRect.Left + canvasButtonRect.Width / 2 - textPaint.FontMetrics.AverageCharacterWidth / 2;
                                ty = canvasButtonRect.Top + canvasButtonRect.Height * 0.85f + textPaint.FontMetrics.Descent;
                                break;
                            case 4:
                                if (_showDirections)
                                    str = "\u2196";
                                else
                                    str = "7";
                                tx = canvasButtonRect.Left + canvasButtonRect.Width * 0.15f - textPaint.FontMetrics.AverageCharacterWidth / 2;
                                ty = canvasButtonRect.Top + canvasButtonRect.Height * 0.15f + textPaint.FontMetrics.Descent;
                                break;
                            case 5:
                                if (_showDirections)
                                    str = "";
                                else
                                    str = "5";
                                tx = canvasButtonRect.Left + canvasButtonRect.Width / 2 - textPaint.FontMetrics.AverageCharacterWidth / 2;
                                ty = canvasButtonRect.Top + canvasButtonRect.Height / 2 + textPaint.FontMetrics.Descent;
                                break;
                            case 6:
                                if (_showDirections)
                                    str = "\u2197";
                                else
                                    str = "9";
                                tx = canvasButtonRect.Left + canvasButtonRect.Width * 0.85f - textPaint.FontMetrics.AverageCharacterWidth / 2;
                                ty = canvasButtonRect.Top + canvasButtonRect.Height * 0.15f + textPaint.FontMetrics.Descent;
                                break;
                            case 7:
                                if (_showDirections)
                                    str = "\u2198";
                                else
                                    str = "3";
                                tx = canvasButtonRect.Left + canvasButtonRect.Width * 0.85f - textPaint.FontMetrics.AverageCharacterWidth / 2;
                                ty = canvasButtonRect.Top + canvasButtonRect.Height * 0.85f + textPaint.FontMetrics.Descent;
                                break;
                            case 8:
                                if (_showDirections)
                                    str = "\u2199";
                                else
                                    str = "1";
                                tx = canvasButtonRect.Left + canvasButtonRect.Width * 0.15f - textPaint.FontMetrics.AverageCharacterWidth / 2;
                                ty = canvasButtonRect.Top + canvasButtonRect.Height * 0.85f + textPaint.FontMetrics.Descent;
                                break;
                            case 9:
                                if (_showDirections)
                                    continue;
                                else
                                    str = "0";
                                tx = 0 + canvasButtonRect.Left / 2 - textPaint.FontMetrics.AverageCharacterWidth / 2;
                                ty = canvasButtonRect.Top + canvasButtonRect.Height * 0.15f + textPaint.FontMetrics.Descent;
                                textPaint.TextSize = Math.Max(10.0f, textPaint.TextSize * Math.Min(1.0f, canvasButtonRect.Left / (canvasButtonRect.Width * 0.3f)));
                                break;
                        }
                        canvas.DrawText(str, tx, ty, textPaint);

                    }
                }

                if (ShowWaitIcon)
                {
                    SKRect targetrect;
                    float[] sizearray = { 10.0f, 10.1f, 10.2f, 10.3f, 10.4f, 10.3f, 10.2f, 10.1f, 10.0f, 9.9f };
                    long sizeidx = 0;
                    lock (AnimationTimerLock)
                    {
                        sizeidx = AnimationTimers.general_animation_counter % 10;
                    }
                    float size = sizearray[sizeidx];
                    targetrect = new SKRect(canvaswidth / 2 - canvaswidth / size, canvasheight / 2 - canvaswidth / size, canvaswidth / 2 + canvaswidth / size, canvasheight / 2 + canvaswidth / size);
                    canvas.DrawBitmap(_logoBitmap, targetrect);
                }

                if(ShowFPS)
                {
                    if (!_stopWatch.IsRunning)
                    {
                        _stopWatch.Restart();
                    }
                    else 
                    {
                        _stopWatch.Stop();
                        TimeSpan ts = _stopWatch.Elapsed;
                        long _currentGeneralCounterValue = 0L;
                        lock (AnimationTimerLock)
                        {
                            _currentGeneralCounterValue = AnimationTimers.general_animation_counter;
                        }
                        long diff = _currentGeneralCounterValue - _previousGeneralCounterValue;
                        _previousGeneralCounterValue = _currentGeneralCounterValue;
                        lock (_fpslock)
                        {
                            _fps = ts.TotalMilliseconds == 0.0 ? 0.0 : 1000.0 / ts.TotalMilliseconds;
                            str = "FPS: " + string.Format("{0:0.0}", _fps) + ", D:" + diff;
                        }
                        textPaint.Typeface = App.LatoBold;
                        textPaint.TextSize = 30;
                        textPaint.Color = SKColors.Yellow;
                        textWidth = textPaint.MeasureText(str, ref textBounds);
                        yText = -textPaint.FontMetrics.Ascent + 5;
                        xText = canvaswidth - textWidth - 5;
                        canvas.DrawText(str, xText, yText, textPaint);

                        _stopWatch.Restart();
                    }
                }
                else
                {
                    if(_stopWatch.IsRunning)
                        _stopWatch.Stop();
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

            canvas.Flush();

        }


        public double CurrentPageWidth { get { return _currentPageWidth;  } }
        public double CurrentPageHeight { get { return _currentPageHeight; } }

        private double _currentPageWidth = 0;
        private double _currentPageHeight = 0;

        protected override void OnSizeAllocated(double width, double height)
        {
            base.OnSizeAllocated(width, height);
            if(width != _currentPageWidth || height != _currentPageHeight)
            {
                _currentPageWidth = width;
                _currentPageHeight = height;

                double sidesize = Math.Min(Math.Min(80.0, Math.Max(45.0, width / 8)), Math.Min(80.0, Math.Max(45.0, height / 8)));
                GameMenuButton.WidthRequest = ESCButton.HeightRequest = sidesize;
                ESCButton.WidthRequest = ESCButton.HeightRequest = sidesize;
                LookModeButton.WidthRequest = LookModeButton.HeightRequest = sidesize;
                ToggleModeButton.WidthRequest = ToggleModeButton.HeightRequest = sidesize;
                ToggleZoomMiniButton.WidthRequest = ToggleZoomMiniButton.HeightRequest = sidesize;
                ToggleZoomAlternateButton.WidthRequest = ToggleZoomAlternateButton.HeightRequest = sidesize;

                sidesize = Math.Min(Math.Min(80.0, Math.Max(60.0, width / 6)), Math.Min(80.0, Math.Max(60.0, height / 6)));
                ZeroButton.WidthRequest = ZeroButton.HeightRequest = sidesize;
                FirstButton.WidthRequest = FirstButton.HeightRequest = sidesize;
                SecondButton.WidthRequest = SecondButton.HeightRequest = sidesize;
                ThirdButton.WidthRequest = ThirdButton.HeightRequest = sidesize;
                FourthButton.WidthRequest = FourthButton.HeightRequest = sidesize;

                double imgsidewidth = Math.Min(Math.Min(80.0, Math.Max(50.0, width / 7)), Math.Min(80.0, Math.Max(50.0, height / 7)));
                double imgsideheight = Math.Min(Math.Min(80.0, Math.Max(50.0, width / 7)), Math.Min(80.0, Math.Max(50.0, height / 7)));
                double fontsize = 9.0 * imgsidewidth / 50.0;
                double fontsize_larger = 9.5 * imgsidewidth / 50.0;
                double gridsidewidth = imgsidewidth;
                double gridsideheight = imgsideheight + fontsize + 2;
                double gridsideheight_larger = imgsideheight + fontsize_larger + 2;

                AbilitiesGrid.WidthRequest = SkillGrid.WidthRequest = gridsidewidth;
                AbilitiesGrid.HeightRequest = SkillGrid.HeightRequest = gridsideheight_larger;
                AbilitiesImg.WidthRequest = SkillImg.WidthRequest = imgsidewidth;
                AbilitiesImg.HeightRequest = SkillImg.HeightRequest = imgsideheight;
                /* AbilitiesLbl */
                InventoryGrid.WidthRequest = SearchGrid.WidthRequest = WaitGrid.WidthRequest = DropManyGrid.WidthRequest = gridsidewidth;
                InventoryGrid.HeightRequest = SearchGrid.HeightRequest = WaitGrid.HeightRequest = DropManyGrid.HeightRequest = gridsideheight;
                InventoryImg.WidthRequest = SearchImg.WidthRequest = WaitImg.WidthRequest = DropManyImg.WidthRequest = imgsidewidth;
                InventoryImg.HeightRequest = SearchImg.HeightRequest = WaitImg.HeightRequest = DropManyImg.HeightRequest = imgsideheight;
                SkillLbl.FontSize = fontsize_larger;
                InventoryLbl.FontSize = SearchLbl.FontSize = WaitLbl.FontSize = DropManyLbl.FontSize = fontsize;
                UpperCmdGrid.HeightRequest = gridsideheight;
                LowerCmdGrid.HeightRequest = gridsideheight;
                ChatButton.HeightRequest = RepeatButton.HeightRequest = MenuButton.HeightRequest = KickButton.HeightRequest = FireButton.HeightRequest =
                    ThrowQuiveredButton.HeightRequest = ApplyWieldedButton.HeightRequest = CastButton.HeightRequest = ZapButton.HeightRequest =
                    SwapWeaponButton.HeightRequest = gridsideheight;

                if (width > height)
                {
                    /* Landscape */
                    ButtonRowStack.Orientation = StackOrientation.Horizontal;
                    ModeLayout.Orientation = StackOrientation.Vertical;
                    ModeSubLayout1.Orientation = StackOrientation.Horizontal;
                    ModeSubLayout2.Orientation = StackOrientation.Horizontal;
                    ZoomLayout.Orientation = StackOrientation.Horizontal;
                    UpperCmdLayout.Orientation = StackOrientation.Vertical;
                }
                else
                {
                    /* Portrait */
                    ButtonRowStack.Orientation = StackOrientation.Vertical;
                    ModeLayout.Orientation = StackOrientation.Vertical;
                    ModeSubLayout1.Orientation = StackOrientation.Vertical;
                    ModeSubLayout2.Orientation = StackOrientation.Vertical;
                    ZoomLayout.Orientation = StackOrientation.Horizontal;
                    UpperCmdLayout.Orientation = StackOrientation.Horizontal;
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
                _clientGame = new ClientGame(this, EnableWizardMode);
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

        private Dictionary<long, TouchEntry> TouchDictionary = new Dictionary<long, TouchEntry>();
        public object MapOffsetLock = new object();
        public float _mapOffsetX = 0;
        public float _mapOffsetY = 0;
        private bool _touchMoved = false;
        private object _savedSender = null;
        private SKTouchEventArgs _savedEventArgs = null;

        private void canvasView_Touch(object sender, SKTouchEventArgs e)
        {
            if (_clientGame != null)
            {
                lock(TargetClipLock)
                {
                    _targetClipOn = false;
                }

                switch (e?.ActionType)
                {
                    case SKTouchAction.Entered:
                        break;
                    case SKTouchAction.Pressed:
                        _savedSender = null;
                        _savedEventArgs = null;

                        if (TouchDictionary.ContainsKey(e.Id))
                            TouchDictionary[e.Id] = new TouchEntry(e.Location, DateTime.Now);
                        else
                            TouchDictionary.Add(e.Id, new TouchEntry(e.Location, DateTime.Now));
                        
                        if (TouchDictionary.Count > 1)
                            _touchMoved = true;
                        else if (MapMode == GHMapMode.Normal)
                        {
                            _savedSender = sender;
                            _savedEventArgs = e;
                            Device.StartTimer(TimeSpan.FromSeconds(GHConstants.MoveByHoldingDownThreshold), () =>
                            {
                                if (_savedSender == null || _savedEventArgs == null)
                                    return false;

                                IssueNHCommandViaTouch(_savedSender, _savedEventArgs);
                                return true; /* Continue until cancelled */
                            });
                        }

                        e.Handled = true;
                        break;
                    case SKTouchAction.Moved:
                        {
                            TouchEntry entry;
                            bool res = TouchDictionary.TryGetValue(e.Id, out entry);
                            if (res)
                            {
                                SKPoint anchor = entry.Location;

                                float diffX = e.Location.X - anchor.X;
                                float diffY = e.Location.Y - anchor.Y;
                                float dist = (float)Math.Sqrt((Math.Pow(diffX, 2) + Math.Pow(diffY, 2)));

                                if (TouchDictionary.Count == 1)
                                {
                                    if (MapMode == GHMapMode.Normal)
                                    {
                                        /* Move the save location */
                                        _savedSender = sender;
                                        _savedEventArgs = e;
                                    }
                                    else if(!ZoomMiniMode && (dist > 25 ||
                                        (DateTime.Now.Ticks - entry.PressTime.Ticks) / TimeSpan.TicksPerMillisecond > GHConstants.MoveOrPressTimeThreshold
                                           ))
                                    {
                                        /* Just one finger => Move the map */
                                        if (diffX != 0 || diffY != 0)
                                        {
                                            lock (MapOffsetLock)
                                            {
                                                _mapOffsetX += diffX;
                                                _mapOffsetY += diffY;
                                                if (_mapWidth > 0 && Math.Abs(_mapOffsetX) > 10 * _mapWidth)
                                                {
                                                    _mapOffsetX = 10 * _mapWidth * Math.Sign(_mapOffsetX);
                                                }
                                                if (_mapHeight > 0 && Math.Abs(_mapOffsetY) > 10 * _mapHeight)
                                                {
                                                    _mapOffsetY = 10 * _mapHeight * Math.Sign(_mapOffsetY);
                                                }
                                            }
                                            TouchDictionary[e.Id].Location = e.Location;
                                            _touchMoved = true;
                                        }
                                    }

                                }
                                else if (TouchDictionary.Count == 2)
                                {
                                    _savedSender = null;
                                    _savedEventArgs = null;

                                    SKPoint prevloc = TouchDictionary[e.Id].Location;
                                    SKPoint curloc = e.Location;
                                    SKPoint otherloc;

                                    Dictionary<long, TouchEntry>.KeyCollection keys = TouchDictionary.Keys;
                                    long other_key = 0;
                                    foreach (long key in keys)
                                    {
                                        if (key != e.Id)
                                        {
                                            other_key = key;
                                            break;
                                        }
                                    }

                                    if (other_key != 0 && !ZoomMiniMode)
                                    {
                                        otherloc = TouchDictionary[other_key].Location;
                                        float prevdist = (float)Math.Sqrt((Math.Pow((double)otherloc.X - (double)prevloc.X, 2) + Math.Pow((double)otherloc.Y - (double)prevloc.Y, 2)));
                                        float curdist = (float)Math.Sqrt((Math.Pow((double)otherloc.X - (double)curloc.X, 2) + Math.Pow((double)otherloc.Y - (double)curloc.Y, 2)));
                                        if (prevdist > 0 && curdist > 0)
                                        {
                                            float ratio = curdist / prevdist;
                                            float curfontsize = ZoomAlternateMode ? MapFontAlternateSize : MapFontSize;
                                            float newfontsize = curfontsize * ratio;
                                            if (newfontsize > 500)
                                                newfontsize = 500;
                                            if (newfontsize < 4)
                                                newfontsize = 4;

                                            if(ZoomAlternateMode)
                                                MapFontAlternateSize = newfontsize;
                                            else
                                                MapFontSize = newfontsize;
                                        }
                                    }

                                    TouchDictionary[e.Id].Location = e.Location;
                                    _touchMoved = true;
                                }
                            }
                            e.Handled = true;
                        }
                        break;
                    case SKTouchAction.Released:
                        {
                            _savedSender = null;
                            _savedEventArgs = null;

                            TouchEntry entry;
                            bool res = TouchDictionary.TryGetValue(e.Id, out entry);
                            long elapsedms = (DateTime.Now.Ticks - entry.PressTime.Ticks) / TimeSpan.TicksPerMillisecond;

                            if (elapsedms <= GHConstants.MoveOrPressTimeThreshold && !_touchMoved)
                            {
                                IssueNHCommandViaTouch(sender, e);
                            }
                            if (TouchDictionary.ContainsKey(e.Id))
                                TouchDictionary.Remove(e.Id);
                            if (TouchDictionary.Count == 0)
                                _touchMoved = false;
                            e.Handled = true;
                        }
                        break;
                    case SKTouchAction.Cancelled:
                        break;
                    case SKTouchAction.Exited:
                        break;
                    case SKTouchAction.WheelChanged:
                        break;
                    default:
                        break;
                }
            }
        }

        public void IssueNHCommandViaTouch(object sender, SKTouchEventArgs e)
        {
            if (!_showDirections && !_showNumberPad)
            {
                int x = 0, y = 0, mod = 0;
                float canvaswidth = canvasView.CanvasSize.Width;
                float canvasheight = canvasView.CanvasSize.Height;
                float offsetX = (canvaswidth - _mapWidth) / 2;
                float offsetY = (canvasheight - _mapHeight) / 2;

                if (ZoomMiniMode)
                {
                    offsetX -= _mapOffsetX;
                    offsetY -= _mapOffsetY;
                }
                else
                {
                    lock (ClipLock)
                    {
                        if (ClipX > 0 && (_mapWidth > canvaswidth || _mapHeight > canvasheight))
                        {
                            offsetX -= (ClipX - (GHConstants.MapCols - 1) / 2) * _tileWidth;
                            offsetY -= (ClipY - GHConstants.MapRows / 2) * _tileHeight;
                        }
                    }
                }
                offsetX += _mapOffsetX;
                offsetY += _mapOffsetY + _mapFontAscent;

                if (_tileWidth > 0)
                    x = (int)((e.Location.X - offsetX) / _tileWidth);
                if (_tileHeight > 0)
                    y = (int)((e.Location.Y - offsetY) / _tileHeight);

                if (x > 0 && x < GHConstants.MapCols && y >= 0 && y < GHConstants.MapRows)
                {
                    if (MapMode == GHMapMode.Look)
                        mod = (int)NhGetPosMods.Click2;
                    else if (MapMode == GHMapMode.Travel)
                        mod = (int)NhGetPosMods.Click1;
                    else
                        mod = (int)NhGetPosMods.Click3;

                    ConcurrentQueue<GHResponse> queue;
                    if (ClientGame.ResponseDictionary.TryGetValue(_clientGame, out queue))
                    {
                        queue.Enqueue(new GHResponse(_clientGame, GHRequestType.Location, x, y, mod));
                    }
                }
            }
            else
            {
                if (e.Location.X >= canvasButtonRect.Left && e.Location.X <= canvasButtonRect.Right && e.Location.Y >= canvasButtonRect.Top && e.Location.Y <= canvasButtonRect.Bottom)
                {
                    int resp = 0;
                    SKPoint RectLoc = new SKPoint(e.Location.X - canvasButtonRect.Left, e.Location.Y - canvasButtonRect.Top);

                    if (RectLoc.Y < canvasButtonRect.Height * 0.3 && RectLoc.X < canvasButtonRect.Width * 0.3)
                        resp = -7;
                    else if (RectLoc.Y < canvasButtonRect.Height * 0.3 && RectLoc.X > canvasButtonRect.Width * 0.7)
                        resp = -9;
                    else if (RectLoc.Y > canvasButtonRect.Height * 0.7 && RectLoc.X < canvasButtonRect.Width * 0.3)
                        resp = -1;
                    else if (RectLoc.Y > canvasButtonRect.Height * 0.7 && RectLoc.X > canvasButtonRect.Width * 0.7)
                        resp = -3;
                    else if (RectLoc.Y < canvasButtonRect.Height * 0.3)
                        resp = -8; //ch = "k";
                    else if (RectLoc.Y > canvasButtonRect.Height * 0.7)
                        resp = -2; // ch = "j";
                    else if (RectLoc.X < canvasButtonRect.Width * 0.3)
                        resp = -4; // ch = "h";
                    else if (RectLoc.X > canvasButtonRect.Width * 0.7)
                        resp = -6; // ch = "l";
                    else
                        resp = showNumberPad ? -5 : 46; /* '.', or self */

                    if (showNumberPad)
                        resp -= 10;

                    ConcurrentQueue<GHResponse> queue;
                    if (ClientGame.ResponseDictionary.TryGetValue(_clientGame, out queue))
                    {
                        queue.Enqueue(new GHResponse(_clientGame, GHRequestType.GetChar, resp));
                    }
                }
                else if (showNumberPad && e.Location.X < canvasButtonRect.Left && e.Location.Y >= canvasButtonRect.Top && e.Location.Y <= canvasButtonRect.Top + canvasButtonRect.Height / 3)
                {
                    int resp = -10;
                    ConcurrentQueue<GHResponse> queue;
                    if (ClientGame.ResponseDictionary.TryGetValue(_clientGame, out queue))
                    {
                        queue.Enqueue(new GHResponse(_clientGame, GHRequestType.GetChar, resp));
                    }
                }
            }
        }


        public void SetMapSymbol(int x, int y, int glyph, int bkglyph, int c, int color, uint special, LayerInfo layers)
        {
            lock (_mapDataLock)
            {
                if (((layers.layer_flags & (ulong)LayerFlags.LFLAGS_UXUY) != 0 && (_mapData[x, y].Layers.layer_flags & (ulong)LayerFlags.LFLAGS_UXUY) == 0) ||
                    (layers.m_id != 0 && layers.m_id != _mapData[x, y].Layers.m_id))
                {
                    /* Update counter value only if the monster just moved here, not, e.g. if it changes action in the same square,
                     * or is printed in the same square again with the same origin coordinates. This way, the movement action is played only once. 
                     */
                    lock (AnimationTimerLock)
                    {
                        _mapData[x, y].GlyphPrintCounterValue = AnimationTimers.general_animation_counter;
                    }
                }
                _mapData[x, y].Glyph = glyph;
                _mapData[x, y].BkGlyph = bkglyph;
                _mapData[x, y].Symbol = Char.ConvertFromUtf32(c);
                _mapData[x, y].Color = ClientUtils.NHColor2SKColor((nhcolor)color);
                _mapData[x, y].Special = special;
                _mapData[x, y].Layers = layers;

                _mapData[x, y].NeedsUpdate = true;
            }
        }
        public void SetMapCursor(int x, int y)
        {
            lock (_mapDataLock)
            {
                _mapCursorX = x;
                _mapCursorY = y;
            }
        }
        public void UpdateCursor(int style, int force_paint, int show_on_u)
        {
            lock (_mapDataLock)
            {
                _cursorType = (game_cursor_types)style;
                _force_paint_at_cursor = (force_paint != 0);
                _show_cursor_on_u = (show_on_u != 0);
            }
        }

        public void ClearMap()
        {
            lock (_mapDataLock)
            {
                for (int x = 1; x < GHConstants.MapCols; x++)
                {
                    for (int y = 0; y < GHConstants.MapRows; y++)
                    {
                        _mapData[x, y].Glyph = UnexploredGlyph;
                        _mapData[x, y].BkGlyph = NoGlyph;
                        _mapData[x, y].Symbol = "";
                        _mapData[x, y].Color = SKColors.Black;// default(MapData);
                        _mapData[x, y].Special = 0;
                        _mapData[x, y].NeedsUpdate = true;

                        _mapData[x, y].Layers = new LayerInfo();
                        _mapData[x, y].Layers.layer_glyphs = new int[(int)layer_types.MAX_LAYERS];
                        _mapData[x, y].Layers.leash_mon_x = new sbyte[GHConstants.MaxLeashed + 1];
                        _mapData[x, y].Layers.leash_mon_y = new sbyte[GHConstants.MaxLeashed + 1];

                        _mapData[x, y].Layers.layer_glyphs[0] = UnexploredGlyph;
                        for (int i = 1; i < (int)layer_types.MAX_LAYERS; i++)
                            _mapData[x, y].Layers.layer_glyphs[i] = NoGlyph;

                        _mapData[x, y].Layers.glyph = UnexploredGlyph;
                        _mapData[x, y].Layers.bkglyph = NoGlyph;
                    }
                }
            }
        }


        public void SetTargetClip(int x, int y, bool force)
        {
            lock (TargetClipLock)
            {
                if (force)
                {
                    _targetClipOn = false;
                    _originMapOffsetWithNewClipX = 0;
                    _originMapOffsetWithNewClipY = 0;
                }
                else
                {
                    _targetClipOn = true;
                    _originMapOffsetWithNewClipX = _mapOffsetX + (float)(x - ClipX) * _tileWidth;
                    _originMapOffsetWithNewClipY = _mapOffsetY + (float)(y - ClipY) * _tileHeight;
                    _targetClipStartCounterValue = AnimationTimers.general_animation_counter;
                    _targetClipPanTime = GHConstants.DefaultPanTime;
                }
            }

            lock (ClipLock)
            {
                _clipX = x;
                _clipY = y;
            }
            lock (MapOffsetLock)
            {
                _mapOffsetX = _originMapOffsetWithNewClipX;
                _mapOffsetY = _originMapOffsetWithNewClipY;
            }
        }

        public void ClearAllObjectData(int x, int y)
        {
            lock (_objectDataLock)
            {
                if (_objectData[x, y] != null)
                {
                    if (_objectData[x, y].FloorObjectList != null)
                        _objectData[x, y].FloorObjectList.Clear();
                    if (_objectData[x, y].CoverFloorObjectList != null)
                        _objectData[x, y].CoverFloorObjectList.Clear();
                    if (_objectData[x, y].MemoryObjectList != null)
                        _objectData[x, y].MemoryObjectList.Clear();
                    if (_objectData[x, y].CoverMemoryObjectList != null)
                        _objectData[x, y].CoverMemoryObjectList.Clear();
                }
            }
        }

        public void AddObjectData(int x, int y, obj otmp, int cmdtype, int where, objclassdata otypdata, ulong oflags)
        {
            lock(_objectDataLock)
            {
                if(_objectData[x, y] != null)
                {
                    bool is_memoryobj = (where == (int)obj_where_types.OBJ_HEROMEMORY);
                    bool is_drawn_in_front = (oflags & (ulong)objdata_flags.OBJDATA_FLAGS_DRAWN_IN_FRONT) != 0UL;
                    bool hallucinated = (oflags & (ulong)objdata_flags.OBJDATA_FLAGS_HALLUCINATION) != 0UL;
                    List<ObjectDataItem> ObjectList = is_memoryobj ? (is_drawn_in_front ? _objectData[x, y].CoverMemoryObjectList : _objectData[x, y].MemoryObjectList) : (is_drawn_in_front ? _objectData[x, y].CoverFloorObjectList : _objectData[x, y].FloorObjectList);

                    switch(cmdtype)
                    {
                        case 1: /* Clear */
                            if (ObjectList != null)
                                ObjectList.Clear();
                            break;
                        case 2: /* Add item */
                            if (ObjectList == null)
                            {
                                if (is_memoryobj)
                                {
                                    if (is_drawn_in_front)
                                        _objectData[x, y].CoverMemoryObjectList = new List<ObjectDataItem>();
                                    else
                                        _objectData[x, y].MemoryObjectList = new List<ObjectDataItem>();
                                }
                                else 
                                {
                                    if (is_drawn_in_front)
                                        _objectData[x, y].CoverFloorObjectList = new List<ObjectDataItem>();
                                    else
                                        _objectData[x, y].FloorObjectList = new List<ObjectDataItem>();
                                }

                                ObjectList = is_memoryobj ? (is_drawn_in_front ? _objectData[x, y].CoverMemoryObjectList : _objectData[x, y].MemoryObjectList) : (is_drawn_in_front ? _objectData[x, y].CoverFloorObjectList : _objectData[x, y].FloorObjectList);
                            }
                            ObjectList.Add(new ObjectDataItem(otmp, otypdata, hallucinated));
                            break;
                        case 3: /* Add container item to previous item */
                            if (ObjectList == null || ObjectList.Count == 0)
                                break;
                            if (ObjectList[ObjectList.Count - 1].ContainedObjs == null)
                                ObjectList[ObjectList.Count - 1].ContainedObjs = new List<ObjectDataItem>();
                            ObjectList[ObjectList.Count - 1].ContainedObjs.Add(new ObjectDataItem(otmp, otypdata, hallucinated));
                            break;
                    }                   
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
            if (!((resp >= '0' && resp <= '9') || (resp <= -1 && resp >= -19)))
                showNumberPad = false;

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

        private void LootButton_Clicked(object sender, EventArgs e)
        {
            GenericButton_Clicked(sender, e, 'l');
        }

        private void EatButton_Clicked(object sender, EventArgs e)
        {
            GenericButton_Clicked(sender, e, 'e');
        }

        private void CountButton_Clicked(object sender, EventArgs e)
        {
            GenericButton_Clicked(sender, e, 'n');
        }

        private void RunButton_Clicked(object sender, EventArgs e)
        {
            GenericButton_Clicked(sender, e, 'G');
        }

        private void ESCButton_Clicked(object sender, EventArgs e)
        {
            GenericButton_Clicked(sender, e, 27);
        }

        private void ToggleModeButton_Clicked(object sender, EventArgs e)
        {
            MapTravelMode = !MapTravelMode;
            if(MapTravelMode)
            {
                ToggleModeButton.BackgroundColor = Color.Green;
                MapMode = GHMapMode.Travel;
                MapLookMode = false;
                LookModeButton.BackgroundColor = Color.DarkBlue;
            }
            else
            {
                ToggleModeButton.BackgroundColor = Color.DarkBlue;
                MapMode = GHMapMode.Normal;
            }
        }
        private void LookModeButton_Clicked(object sender, EventArgs e)
        {
            MapLookMode = !MapLookMode;
            if(MapLookMode)
            {
                LookModeButton.BackgroundColor = Color.Green;
                MapMode = GHMapMode.Look;
                MapTravelMode = false;
                ToggleModeButton.BackgroundColor = Color.DarkBlue;
            }
            else 
            {
                LookModeButton.BackgroundColor = Color.DarkBlue;
                MapMode = GHMapMode.Normal;
            }
        }
        private void NoClipButton_Clicked(object sender, EventArgs e)
        {
            MapNoClipMode = !MapNoClipMode;
            if (MapNoClipMode)
            {
                //NoClipButton.BackgroundColor = Color.Green;
            }
            else
            {
                //NoClipButton.BackgroundColor = Color.DarkBlue;
            }

        }

        private void ChatButton_Clicked(object sender, EventArgs e)
        {
            GenericButton_Clicked(sender, e, 'C');
        }

        private void ThrowQuiveredButton_Clicked(object sender, EventArgs e)
        {
            GenericButton_Clicked(sender, e, 't');
        }

        private void ApplyWieldedButton_Clicked(object sender, EventArgs e)
        {
            GenericButton_Clicked(sender, e, 'a');
        }

        private void ZapButton_Clicked(object sender, EventArgs e)
        {
            GenericButton_Clicked(sender, e, 'z');
        }

        private void SwapWeaponButton_Clicked(object sender, EventArgs e)
        {
            GenericButton_Clicked(sender, e, 'x');
        }

        private void LootContextButton_Clicked(object sender, EventArgs e)
        {
            GenericButton_Clicked(sender, e, 'l');
        }

        private void EatContextButton_Clicked(object sender, EventArgs e)
        {
            GenericButton_Clicked(sender, e, 'e');
        }

        private void DropAllContextButton_Clicked(object sender, EventArgs e)
        {
            GenericButton_Clicked(sender, e, 'D');
        }

        private void SacrificeContextButton_Clicked(object sender, EventArgs e)
        {
            GenericButton_Clicked(sender, e, GHUtils.Meta('o'));
        }

        private void SkillButton_Clicked(object sender, EventArgs e)
        {
            GenericButton_Clicked(sender, e, 'S');
        }

        private void AbilitiesButton_Clicked(object sender, EventArgs e)
        {
            GenericButton_Clicked(sender, e, 'A');
        }

        private void DropManyButton_Clicked(object sender, EventArgs e)
        {
            GenericButton_Clicked(sender, e, '%');
        }

        private void ToggleZoomMiniButton_Clicked(object sender, EventArgs e)
        {
            ZoomMiniMode = !ZoomMiniMode;
            if (ZoomMiniMode)
            {
                ToggleZoomMiniButton.BackgroundColor = Color.Green;
            }
            else
            {
                ToggleZoomMiniButton.BackgroundColor = Color.DarkBlue;
            }
        }

        private void ToggleZoomAlternateButton_Clicked(object sender, EventArgs e)
        {
            ZoomAlternateMode = !ZoomAlternateMode;
            if (ZoomAlternateMode)
            {
                ToggleZoomAlternateButton.BackgroundColor = Color.Green;
            }
            else
            {
                ToggleZoomAlternateButton.BackgroundColor = Color.DarkBlue;
            }

        }

        private void GameMenuButton_Clicked(object sender, EventArgs e)
        {
            ShowGameMenu(sender, e);
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

    public class TouchEntry 
    {
        public SKPoint Location;
        public DateTime PressTime;

        public TouchEntry()
        {

        }
        public TouchEntry(SKPoint loc, DateTime time)
        {
            Location = loc;
            PressTime = time;
        }
    }

}