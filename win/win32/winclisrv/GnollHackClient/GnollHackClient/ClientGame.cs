using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Text;
using GnollHackCommon;
using System.Threading;
using GnollHackClient.Pages.Game;
using Xamarin.Forms;
using Xamarin.Forms.Xaml;
using System.Threading.Tasks;
using System.Collections.Concurrent;
using System.Runtime.InteropServices;
using Xamarin.Essentials;

namespace GnollHackClient
{
    public class ClientGame
    {
        private static ConcurrentDictionary<ClientGame, ConcurrentQueue<GHRequest>> _concurrentRequestDictionary = new ConcurrentDictionary<ClientGame, ConcurrentQueue<GHRequest>>();
        private static ConcurrentDictionary<ClientGame, ConcurrentQueue<GHResponse>> _concurrentResponseDictionary = new ConcurrentDictionary<ClientGame, ConcurrentQueue<GHResponse>>();
        private int[] _inputBuffer = new int[GHConstants.InputBufferLength];
        private int _inputBufferLocation = -1;
        private string _getLineString = null;
        private bool _screenTextSet = false;
        private bool _crashReportFinished = false;
        private bool _guiTipsFinished = false;
        private bool _panicFinished = false;
        private bool _messageFinished = false;
        private bool _ynConfirmationFinished = false;
        private bool _ynConfirmationResult = false;
        private bool _characternameSet = false;
        private string _characterName = "";
        private object _characterNameLock = new object();
        private readonly GamePage _gamePage;
        public GamePage GamePage { get { return _gamePage; } }

        private bool _touchLocSet = false;
        private int _touchLocX;
        private int _touchLocY;
        private int _touchLocMod;

        private int _lastWindowHandle = 0;
        private GHWindow[] _ghWindows = new GHWindow[GHConstants.MaxGHWindows];
        private object _ghWindowsLock = new object();
        public GHWindow[] Windows { get { return _ghWindows; } }
        public object WindowsLock { get { return _ghWindowsLock; } }
        public int MapWindowId { get; set; }
        public int MessageWindowId { get; set; }
        public int StatusWindowId { get; set; }
        private List<GHMsgHistoryItem> _message_history = new List<GHMsgHistoryItem>();

        public static ConcurrentDictionary<ClientGame, ConcurrentQueue<GHRequest>> RequestDictionary { get { return _concurrentRequestDictionary; } }
        public static ConcurrentDictionary<ClientGame, ConcurrentQueue<GHResponse>> ResponseDictionary { get { return _concurrentResponseDictionary; } }
        public string CharacterName {
            get { lock (_characterNameLock) { return _characterName; } } 
            set { lock (_characterNameLock) { _characterName = value; } }
        }
        public bool WizardMode { get { return _gamePage.EnableWizardMode; } }
        public bool CasualMode { get { return _gamePage.EnableCasualMode; } }
        public bool ModernMode { get { return _gamePage.EnableModernMode; } }

        public ClientGame(GamePage gamePage)
        {
            ClientGame.RequestDictionary.TryAdd(this, new ConcurrentQueue<GHRequest>());
            ClientGame.ResponseDictionary.TryAdd(this, new ConcurrentQueue<GHResponse>());
            _gamePage = gamePage;
        }

        ~ClientGame()
        {
            ConcurrentQueue<GHRequest> requestqueue;
            ConcurrentQueue<GHResponse> responsequeue;
            ClientGame.RequestDictionary.TryRemove(this, out requestqueue);
            ClientGame.ResponseDictionary.TryRemove(this, out responsequeue);
        }

        private void pollResponseQueue()
        {
            /* Makes sure that whatever is happening, the game gets saved and then restored upon sleep / restore */
            if(_saveRequested)
            {
                _saveRequested = false;
                if(!App.CancelSaveGame)
                {
                    App.SavingGame = true;
                    App.GnollHackService.SaveAndRestoreSavedGame();
                }
                App.GameSaved = false;
                App.SavingGame = false;
            }
            if (_timeTallyRequested)
            {
                _timeTallyRequested = false;
                App.GnollHackService.TallyRealTime();
            }

            ConcurrentQueue<GHResponse> queue;
            GHResponse response;
            if(ClientGame.ResponseDictionary.TryGetValue(this, out queue))
            {
                while (queue.TryDequeue(out response))
                {
                    switch (response.RequestType)
                    {
                        case GHRequestType.AskName:
                            CharacterName = response.ResponseStringValue;
                            _characternameSet = true;
                            break;
                        case GHRequestType.GetChar:
                            _inputBufferLocation++;
                            if(_inputBufferLocation >= GHConstants.InputBufferLength)
                                _inputBufferLocation = GHConstants.InputBufferLength -1;
                            _inputBuffer[_inputBufferLocation] = response.ResponseIntValue;
                            break;
                        case GHRequestType.Location:
                            _touchLocX = response.X;
                            _touchLocY = response.Y;
                            _touchLocMod = response.Mod;
                            _touchLocSet = true;
                            break;
                        case GHRequestType.GetLine:
                            _getLineString = response.ResponseStringValue;
                            break;
                        case GHRequestType.ShowMenuPage:
                            if(response.RequestingGHWindow != null)
                            {
                                if (response.SelectedMenuItems != null)
                                    response.RequestingGHWindow.SelectedMenuItems = response.SelectedMenuItems;
                                else
                                    response.RequestingGHWindow.SelectedMenuItems = new List<GHMenuItem>(); /* Empty selection */

                                response.RequestingGHWindow.WasCancelled = response.ResponseBoolValue;
                            }
                            else
                            {
                                //Throw an error or stop waiting
                            }
                            break;
                        case GHRequestType.DisplayScreenText:
                            _screenTextSet = true;
                            break;
                        case GHRequestType.SetPetMID:
                            if(_gamePage != null)
                                _gamePage.GnollHackService.SetPetMID(response.ResponseUIntValue);
                            break;
                        case GHRequestType.ShowGUITips:
                            _guiTipsFinished = true;
                            break;
                        case GHRequestType.CrashReport:
                            _crashReportFinished = true;
                            break;
                        case GHRequestType.Panic:
                            _panicFinished = true;
                            break;
                        case GHRequestType.Message:
                            _messageFinished = true;
                            break;
                        case GHRequestType.YnConfirmation:
                            _ynConfirmationResult = response.ResponseBoolValue;
                            _ynConfirmationFinished = true;
                            break;
                        case GHRequestType.SaveGameAndWaitForResume:
                            RequestSaveGame();
                            break;
                        case GHRequestType.StopWaitAndRestoreSavedGame:
                            RequestRestoreSavedGame();
                            break;
                        case GHRequestType.TallyRealTime:
                            RequestTallyRealTime();
                            break;
                        default:
                            break;
                    }
                }
            }
        }


        public void ClientCallback_InitWindows()
        {
            Debug.WriteLine("ClientCallback_InitWindows");

            /* Initialize now glyph2tile and other relevant arrays */
            IntPtr gl2ti_ptr;
            int gl2ti_size;
            IntPtr gltifl_ptr;
            int gltifl_size;
            IntPtr ti2an_ptr;
            int ti2an_size;
            IntPtr ti2en_ptr;
            int ti2en_size;
            IntPtr ti2ad_ptr;
            int ti2ad_size;
            IntPtr anoff_ptr;
            int anoff_size;
            IntPtr enoff_ptr;
            int enoff_size;
            IntPtr reoff_ptr;
            int reoff_size;
            _gamePage.GnollHackService.GetGlyphArrays(out gl2ti_ptr, out gl2ti_size, out gltifl_ptr, out gltifl_size);
            _gamePage.GnollHackService.GetTileArrays(out ti2an_ptr, out ti2an_size, out ti2en_ptr, out ti2en_size, out ti2ad_ptr, out ti2ad_size,
                out anoff_ptr, out anoff_size, out enoff_ptr, out enoff_size, out reoff_ptr, out reoff_size);
            lock (App.Glyph2TileLock)
            {
                if (gl2ti_ptr != null && gl2ti_size > 0)
                {
                    App.Glyph2Tile = new int[gl2ti_size];
                    Marshal.Copy(gl2ti_ptr, App.Glyph2Tile, 0, gl2ti_size);
                }
                if (gltifl_ptr != null && gltifl_size > 0)
                {
                    App.GlyphTileFlags = new byte[gltifl_size];
                    Marshal.Copy(gltifl_ptr, App.GlyphTileFlags, 0, gltifl_size);
                }
                if (ti2an_ptr != null && ti2an_size > 0)
                {
                    App.Tile2Animation = new short[ti2an_size];
                    Marshal.Copy(ti2an_ptr, App.Tile2Animation, 0, ti2an_size);
                }
                if (ti2en_ptr != null && ti2en_size > 0)
                {
                    App.Tile2Enlargement = new short[ti2en_size];
                    Marshal.Copy(ti2en_ptr, App.Tile2Enlargement, 0, ti2en_size);
                }
                if (ti2ad_ptr != null && ti2ad_size > 0)
                {
                    App.Tile2Autodraw = new short[ti2ad_size];
                    Marshal.Copy(ti2ad_ptr, App.Tile2Autodraw, 0, ti2ad_size);
                }
                if (anoff_ptr != null && anoff_size > 0)
                {
                    App.AnimationOffsets = new int[anoff_size];
                    Marshal.Copy(anoff_ptr, App.AnimationOffsets, 0, anoff_size);
                }
                if (enoff_ptr != null && enoff_size > 0)
                {
                    App.EnlargementOffsets = new int[enoff_size];
                    Marshal.Copy(enoff_ptr, App.EnlargementOffsets, 0, enoff_size);
                }
                if (reoff_ptr != null && reoff_size > 0)
                {
                    App.ReplacementOffsets = new int[reoff_size];
                    Marshal.Copy(reoff_ptr, App.ReplacementOffsets, 0, reoff_size);
                }
            }

            int total_tiles_used = _gamePage.GnollHackService.GetTotalTiles();
            int total_sheets_used = Math.Min(GHConstants.MaxTileSheets, (total_tiles_used - 1) / GHConstants.NumberOfTilesPerSheet + 1);

            lock (App.Glyph2TileLock)
            {
                App.UsedTileSheets = total_sheets_used;
                App.TotalTiles = total_tiles_used;
                for (int i = 0; i < total_sheets_used; i++)
                {
                    App.TilesPerRow[i] = _gamePage.TileMap[i].Width / GHConstants.TileWidth;
                }
            }

            ConcurrentQueue<GHRequest> queue;
            if (ClientGame.RequestDictionary.TryGetValue(this, out queue))
            {
                queue.Enqueue(new GHRequest(this, GHRequestType.HideLoadingScreen));
            }
        }
        public int ClientCallback_CreateGHWindow(int wintype, int style, int glyph, byte dataflags, IntPtr objdata_ptr, IntPtr otypdata_ptr)
        {
            obj objdata = objdata_ptr == IntPtr.Zero ? new obj() : (obj)Marshal.PtrToStructure(objdata_ptr, typeof(obj));
            objclassdata otypdata = otypdata_ptr == IntPtr.Zero ? new objclassdata() : (objclassdata)Marshal.PtrToStructure(otypdata_ptr, typeof(objclassdata));

            if (_lastWindowHandle >= GHConstants.MaxGHWindows) /* Should not happen, but paranoid */
                _lastWindowHandle = GHConstants.MaxGHWindows - 1;

            while (_lastWindowHandle > 0 && _ghWindows[_lastWindowHandle] == null)
                _lastWindowHandle--;

            if (_ghWindows[_lastWindowHandle] != null)
                _lastWindowHandle++;

            if (_lastWindowHandle >= GHConstants.MaxGHWindows)
                return 0;

            int handle = _lastWindowHandle;
            GHWindow ghwin = new GHWindow((GHWinType)wintype, (ghwindow_styles)style, glyph, (dataflags & 8) != 0, (dataflags & 16) != 0,
                (dataflags & 1) == 0 ? null : new ObjectDataItem(objdata, otypdata, (dataflags & 4) != 0), _gamePage, handle);

            lock(_ghWindowsLock)
            {
                _ghWindows[handle] = ghwin;
                ghwin.Create();
                if (wintype == (int)GHWinType.Map)
                    MapWindowId = handle;
                else if (wintype == (int)GHWinType.Message)
                    MessageWindowId = handle;
                else if (wintype == (int)GHWinType.Status)
                    StatusWindowId = handle;
            }
            return handle;
        }
        public void ClientCallback_DestroyGHWindow(int winHandle)
        {
            if (winHandle < 0)
                return;
            lock (_ghWindowsLock)
            {
                GHWindow ghwin = _ghWindows[winHandle];
                if (ghwin != null)
                    ghwin.Destroy();
                if (ghwin.WindowType == GHWinType.Map)
                    MapWindowId = 0;
                _ghWindows[winHandle] = null;
            }
        }
        public void ClientCallback_ClearGHWindow(int winHandle)
        {
            if (winHandle < 0)
                return;
            lock (_ghWindowsLock)
            {
                if (_ghWindows[winHandle] != null)
                    _ghWindows[winHandle].Clear();
            }
        }
        public void ClientCallback_DisplayGHWindow(int winHandle, byte blocking)
        {
            if (winHandle < 0)
                return;
            bool ismenu = false;
            bool istext = false;
            bool ismap = false;
            lock (_ghWindowsLock)
            {
                if (_ghWindows[winHandle] != null)
                {
                    _ghWindows[winHandle].Display(blocking != 0);
                    ismenu = (_ghWindows[winHandle].WindowType == GHWinType.Menu);
                    istext = (_ghWindows[winHandle].WindowType == GHWinType.Text);
                    ismap = (_ghWindows[winHandle].WindowType == GHWinType.Map);
                }
            }

            if((blocking != 0 && ismap) || ismenu || istext)
            {
                int res = ClientCallback_nhgetch();
            }
        }
        public void ClientCallback_ExitWindows(string str)
        {
            Debug.WriteLine("ClientCallback_ExitWindows");
            ClientCallback_RawPrint(str);

            if (!string.IsNullOrWhiteSpace(str))
                Thread.Sleep(1100);

            lock (_ghWindowsLock)
            {
                for(int winHandle = 0; winHandle <= _lastWindowHandle; winHandle++)
                {
                    GHWindow ghwin = _ghWindows[winHandle];
                    if (ghwin == null)
                        continue;
                    if (ghwin.WindowType == GHWinType.Map)
                        MapWindowId = 0;

                    ghwin.Destroy();
                    _ghWindows[winHandle] = null;
                }
                _lastWindowHandle = -1;
            }

        }

        public int ClientCallback_PlayerSelection()
        {
            Debug.WriteLine("ClientCallback_PlayerSelection");
            return 0;
        }

        public void ClientCallback_Curs(int winHandle, int x, int y)
        {
            lock (_ghWindowsLock)
            {
                if (_ghWindows[winHandle] != null)
                    _ghWindows[winHandle].Curs(x, y);
            }
        }

        public void ClientCallback_PrintGlyph(int winHandle, int x, int y, int glyph, int bkglyph, int symbol, int ocolor, uint special, IntPtr layers_ptr)
        {
            lock (_ghWindowsLock)
            {
                if (_ghWindows[winHandle] != null)
                {
                    LayerInfo layers = layers_ptr == IntPtr.Zero ? new LayerInfo() : (LayerInfo)Marshal.PtrToStructure(layers_ptr, typeof(LayerInfo));
                    _ghWindows[winHandle].PrintGlyph(x, y, glyph, bkglyph, symbol, ocolor, special, layers);
                }
            }

            _gamePage.ClearAllObjectData(x, y);

        }

        public string ClientCallback_AskName(string modeName, string modeDescription)
        {
            Debug.WriteLine("ClientCallback_AskName");
            ConcurrentQueue<GHRequest> queue;
            _characternameSet = false;
            CharacterName = "";
            if (ClientGame.RequestDictionary.TryGetValue(this, out queue))
            {
                queue.Enqueue(new GHRequest(this, GHRequestType.AskName, modeName, modeDescription));
                while (!_characternameSet)
                {
                    Thread.Sleep(GHConstants.PollingInterval);
                    pollResponseQueue();
                }
                return CharacterName;
            }
            else
                return "AskNameFailed";
        }

        public void ClientCallback_get_nh_event()
        {
            Debug.WriteLine("ClientCallback_get_nh_event");
        }

        public void ClientCallback_ExitHack(int status)
        {
            Debug.WriteLine("ClientCallback_ExitHack");
            //App.FmodService.StopTestSound();
            App.FmodService.StopAllSounds((uint)GnollHackCommon.StopSoundFlags.All, 0);
            ConcurrentQueue<GHRequest> queue;
            if (ClientGame.RequestDictionary.TryGetValue(this, out queue))
            {
                queue.Enqueue(new GHRequest(this, GHRequestType.ReturnToMainMenu));
            }
        }

        public int ClientCallback_nhgetch()
        {
            Debug.WriteLine("ClientCallback_nhgetch");

            ConcurrentQueue<GHRequest> queue;
            if (ClientGame.RequestDictionary.TryGetValue(this, out queue))
            {
                queue.Enqueue(new GHRequest(this, GHRequestType.GetChar));
                while (_inputBufferLocation < 0)
                {
                    Thread.Sleep(GHConstants.PollingInterval);
                    pollResponseQueue();
                }
                int res = 0;
                if (_inputBufferLocation >= 0)
                {
                    res = _inputBuffer[0];
                    for(int i = 1; i <= _inputBufferLocation; i++)
                    {
                        _inputBuffer[i - 1] = _inputBuffer[i];
                    }
                    _inputBuffer[_inputBufferLocation] = 0;
                    _inputBufferLocation--;
                }
                return res;
            }
            else
                return 0;

        }
        public int ClientCallback_nh_poskey(out int x, out int y, out int mod)
        {
            Debug.WriteLine("ClientCallback_nh_poskey");

            x = 0;
            y = 0;
            mod = 0;

            ConcurrentQueue<GHRequest> queue;
            if (ClientGame.RequestDictionary.TryGetValue(this, out queue))
            {
                queue.Enqueue(new GHRequest(this, GHRequestType.PosKey));
                while (_inputBufferLocation < 0)
                {
                    if(_touchLocSet)
                    {
                        _touchLocSet = false;
                        x = _touchLocX;
                        y = _touchLocY;
                        mod = _touchLocMod;
                        return 0;
                    }
                    Thread.Sleep(GHConstants.PollingInterval);
                    pollResponseQueue();
                }
                int res = 0;
                if (_inputBufferLocation >= 0)
                {
                    res = _inputBuffer[0];
                    for (int i = 1; i <= _inputBufferLocation; i++)
                    {
                        _inputBuffer[i - 1] = _inputBuffer[i];
                    }
                    _inputBuffer[_inputBufferLocation] = 0;
                    _inputBufferLocation--;
                }
                return res;
            }
            else
                return 0;

        }
        public int ClientCallback_YnFunction(int style, int attr, int color, int glyph, string title, string question, string responses, string def, string descriptions, string introline, ulong ynflags)
        {
            if(question != null)
                RawPrintEx(question, attr, color);

            ConcurrentQueue<GHRequest> queue;
            if (responses == null || responses == "")
            {
                if (ClientGame.RequestDictionary.TryGetValue(this, out queue))
                {
                    if((ynflags & 1UL) != 0)
                        queue.Enqueue(new GHRequest(this, GHRequestType.ShowDirections)); //TODO: Show keyboard
                    else
                        queue.Enqueue(new GHRequest(this, GHRequestType.ShowDirections));
                }
                int res = ClientCallback_nhgetch(); /* Get direction / letter */
                if (ClientGame.RequestDictionary.TryGetValue(this, out queue))
                {
                    if ((ynflags & 1UL) != 0)
                        queue.Enqueue(new GHRequest(this, GHRequestType.HideDirections)); //TODO: Hide keyboard
                    else
                        queue.Enqueue(new GHRequest(this, GHRequestType.HideDirections));
                }
                return res;
            }
            else
            {
                if (ClientGame.RequestDictionary.TryGetValue(this, out queue))
                {
                    queue.Enqueue(new GHRequest(this, GHRequestType.ShowYnResponses, style, attr, color, glyph, title, question, responses, descriptions, introline, ynflags));
                }

                int cnt = 0;
                while(cnt < 5)
                {
                    int val = ClientCallback_nhgetch();
                    string desc = "";
                    if (val < 0)
                    {
                        desc = "Numpad direction " + Math.Abs(val);
                        val = 27;
                    }

                    string res = Char.ConvertFromUtf32(val);
                    if (desc == "")
                        desc = res;

                    if (responses.Contains(res))
                    {
                        if (ClientGame.RequestDictionary.TryGetValue(this, out queue))
                        {
                            queue.Enqueue(new GHRequest(this, GHRequestType.HideYnResponses));
                        }
                        return val;
                    }

                    ClientCallback_RawPrint("'" + desc + "': Invalid input!");
                    cnt++;
                }
            }
            if (ClientGame.RequestDictionary.TryGetValue(this, out queue))
            {
                queue.Enqueue(new GHRequest(this, GHRequestType.HideYnResponses));
            }
            return 27;
        }


        public void ClientCallback_Cliparound(int x, int y, byte force)
        {
            if (force == 0 && (_gamePage.MapNoClipMode || _gamePage.MapLookMode || _gamePage.ZoomMiniMode)) //|| (!_gamePage.ZoomAlternateMode && _gamePage.MapNoClipMode) || (_gamePage.ZoomAlternateMode && _gamePage.MapAlternateNoClipMode) 
                return; /* No clip mode ignores cliparound commands */

            _gamePage.SetTargetClip(x, y, force == 1);

            //lock (_gamePage.ClipLock)
            //{
            //    _gamePage.ClipX = x;
            //    _gamePage.ClipY = y;
            //}
        }

        public void ClientCallback_RawPrint(string str)
        {
            RawPrintEx(str, 0, (int)nhcolor.NO_COLOR);
        }
        public void ClientCallback_RawPrintBold(string str)
        {
            RawPrintEx(str, 1, (int)nhcolor.NO_COLOR);
        }
        public void RawPrintEx(string str, int attr, int color)
        {
            if (_message_history.Count > 0)
                _message_history[_message_history.Count - 1].IsLast = false;
            _message_history.Add(new GHMsgHistoryItem(str, attr, color));
            if (_message_history.Count > GHConstants.MaxMessageHistoryLength)
                _message_history.RemoveAt(0);

            List<GHMsgHistoryItem> sendlist = new List<GHMsgHistoryItem>();
            sendlist.AddRange(_message_history);
            if (sendlist.Count > 0)
                sendlist[sendlist.Count - 1].IsLast = true;
            ConcurrentQueue<GHRequest> queue;
            if (ClientGame.RequestDictionary.TryGetValue(this, out queue))
            {
                queue.Enqueue(new GHRequest(this, GHRequestType.PrintHistory, sendlist));
            }
        }
        public void ClientCallback_PutStrEx(int win_id, int attributes, string str, int append, int color)
        {
            if (win_id < 0)
                return;

            if (_ghWindows[win_id].WindowPrintStyle == GHWindowPrintLocations.RawPrint)
            {
                RawPrintEx(str, attributes, color);
            }
            else
            {
                _ghWindows[win_id].PutStrEx(attributes, str, append, color);
            }
        }
        public void ClientCallback_DelayOutput()
        {
            if (ClientCallback_UIHasInput() > 0)
                return;
            Thread.Sleep(GHConstants.DelayOutputDurationInMilliseconds);
        }
        public void ClientCallback_DelayOutputMilliseconds(int milliseconds)
        {
            if (ClientCallback_UIHasInput() > 0)
                return;
            Thread.Sleep(milliseconds);
        }
        public void ClientCallback_DelayOutputIntervals(int intervals)
        {
            long start_counter_value = 0L;
            long current_counter_value = 0L;
            lock (_gamePage.AnimationTimerLock)
            {
                start_counter_value = _gamePage.AnimationTimers.general_animation_counter;
            }

            do
            {
                if (ClientCallback_UIHasInput() > 0)
                    break;

                Thread.Sleep(5);
                lock (_gamePage.AnimationTimerLock)
                {
                    current_counter_value = _gamePage.AnimationTimers.general_animation_counter;
                }
            } while (current_counter_value < start_counter_value + (long)intervals);
        }
        public void ClientCallback_PreferenceUpdate(string str)
        {
            if(str == "statuslines")
            {
                _ghWindows[StatusWindowId].Clear();
            }
        }
        public void ClientCallback_StatusInit()
        {
            for(int i = 0; i < (int)statusfields.MAXBLSTATS; i++)
            {
                _gamePage.StatusFields[i] = new GHStatusField();
            }
        }
        public void ClientCallback_StatusFinish()
        {

        }

        public void ClientCallback_StatusEnable(int fieldidx, string nm, string fmt, byte enable)
        {
            if (fieldidx >= 0 && fieldidx < (int)statusfields.MAXBLSTATS)
            {
                lock (_gamePage.StatusFieldLock)
                {
                    _gamePage.StatusFields[fieldidx].Name = nm;
                    _gamePage.StatusFields[fieldidx].Format = fmt;
                    _gamePage.StatusFields[fieldidx].IsEnabled = enable != 0;
                }
            }
        }

        static public string[] cond_names_short = new string[(int)bl_conditions.NUM_BL_CONDITIONS] {
            "Stone", "Slime", "Strngl", "Suffoc", "FoodPois", "TermIll", "Blind",
            "Deaf", "Stun", "Conf", "Hallu", "Lev", "Fly", "Ride", "Slow", "Paral", 
            "Fear", "Sleep", "Cancl", "Silent", "Grab", "Rot", "Lyca"
        };

        static public string[] cond_names_long = new string[(int)bl_conditions.NUM_BL_CONDITIONS] {
            "Petrifying", "Slimed", "Being Strangled", "Suffocating", "Food Poisoned", "Terminally Ill", "Blind",
            "Deaf", "Stunned", "Confused", "Hallucinating", "Levitating", "Flying", "Riding", "Slowed", "Paralyzed", 
            "Frightened", "Sleeping", "Cancelled", "Silenced", "Grabbed", "Mummy Rot", "Lycanthropy"
        };

        public void ClientCallback_StatusUpdate(int fieldidx, string text, long condbits, int cng, int percent, int color, IntPtr condcolorptr)
        {
            long oldbits = 0L;
            if (fieldidx >= 0 && fieldidx < (int)statusfields.MAXBLSTATS)
            {
                lock (_gamePage.StatusFieldLock)
                {
                    oldbits = _gamePage.StatusFields[fieldidx].Bits;

                    _gamePage.StatusFields[fieldidx].Text = text;
                    _gamePage.StatusFields[fieldidx].Bits = condbits;
                    _gamePage.StatusFields[fieldidx].Change = cng;
                    _gamePage.StatusFields[fieldidx].Percent = percent;
                    _gamePage.StatusFields[fieldidx].Color = color;
                }
            }

            switch(fieldidx)
            {
                case (int)statusfields.BL_SKILL:
                    {
                        GHRequestType rtype;
                        if (text != null && text == "Skill")
                            rtype = GHRequestType.ShowSkillButton;
                        else
                            rtype = GHRequestType.HideSkillButton;

                        ConcurrentQueue<GHRequest> queue;
                        if (ClientGame.RequestDictionary.TryGetValue(this, out queue))
                        {
                            queue.Enqueue(new GHRequest(this, rtype));
                        }
                        break;
                    }
                case (int)statusfields.BL_CAP:
                case (int)statusfields.BL_HUNGER:
                    {
                        if(cng != 0 && text != null && text != "")
                        {
                            ConcurrentQueue<GHRequest> queue;
                            if (ClientGame.RequestDictionary.TryGetValue(this, out queue))
                            {
                                DisplayConditionTextData data = new DisplayConditionTextData();
                                data.text = text.Trim();
                                data.style = 0;
                                data.color = (int)(color & 15);
                                data.tflags = 0UL;
                                queue.Enqueue(new GHRequest(this, GHRequestType.DisplayConditionText, data));
                            }
                        }
                        break;
                    }
                case (int)statusfields.BL_CONDITION:
                    {
                        if (cng != 0 && condbits != 0)
                        {
                            ConcurrentQueue<GHRequest> queue;
                            if (ClientGame.RequestDictionary.TryGetValue(this, out queue))
                            {
                                int arraysize = (int)bl_conditions.NUM_BL_CONDITIONS; // (int)nhcolor.CLR_MAX + 5;
                                short[] condcolors = new short[arraysize];
                                bool condcolorset = false;
                                if (condcolorptr != null)
                                {
                                    Marshal.Copy(condcolorptr, condcolors, 0, arraysize);
                                    condcolorset = true;
                                }

                                for (int i = 0; i < (int)bl_conditions.NUM_BL_CONDITIONS; i++)
                                {
                                    long bit = 1L << i;
                                    bool has_bit = (condbits & bit) != 0;
                                    bool had_bit = (oldbits & bit) != 0;

                                    if (has_bit && !had_bit)
                                    {
                                        int condcolor = (color & 15);
                                        if (condcolorset)
                                        {
                                            condcolor = ((int)condcolors[i] & 15);
                                            //for(int c = 0; c < (int)nhcolor.CLR_MAX; c++)
                                            //{
                                            //    long cbit = 1L << c;
                                            //    bool has_cbit = (condcolormasks[c] & cbit) != 0;
                                            //    if (has_cbit)
                                            //    {
                                            //        condcolor = c;
                                            //        break;
                                            //    }
                                            //}
                                        }

                                        DisplayConditionTextData data = new DisplayConditionTextData();
                                        data.text = ClientGame.cond_names_long[i];
                                        data.style = 0;
                                        data.attr = 0;
                                        data.color = condcolor;
                                        data.tflags = 0UL;
                                        queue.Enqueue(new GHRequest(this, GHRequestType.DisplayConditionText, data));

                                        int filtercolor = (int)nhcolor.CLR_MAX;
                                        switch(i)
                                        {
                                            case (int)bl_conditions.BL_COND_STONE:
                                                filtercolor = (int)nhcolor.CLR_BROWN;
                                                break;
                                            case (int)bl_conditions.BL_COND_SLIME:
                                                filtercolor = (int)nhcolor.CLR_BRIGHT_GREEN;
                                                break;
                                            case (int)bl_conditions.BL_COND_STRNGL:
                                                filtercolor = (int)nhcolor.CLR_RED;
                                                break;
                                            case (int)bl_conditions.BL_COND_SUFFOC:
                                                filtercolor = (int)nhcolor.CLR_RED;
                                                break;
                                            case (int)bl_conditions.BL_COND_FOODPOIS:
                                                filtercolor = (int)nhcolor.CLR_MAGENTA;
                                                break;
                                            case (int)bl_conditions.BL_COND_TERMILL:
                                                filtercolor = (int)nhcolor.CLR_MAGENTA;
                                                break;
                                        }

                                        if(filtercolor != (int)nhcolor.CLR_MAX)
                                        {
                                            DisplayScreenFilterData fdata = new DisplayScreenFilterData();
                                            fdata.style = 0;
                                            fdata.attr = 0;
                                            fdata.color = filtercolor;
                                            fdata.fflags = 0UL;
                                            queue.Enqueue(new GHRequest(this, GHRequestType.DisplayScreenFilter, fdata));
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                        break;
                    }
                default:
                    break;
            }
        }

        private int _msgIndex = 0;
        public string ClientCallback_GetMsgHistory(IntPtr attr, IntPtr color, byte init)
        {
            if (init != 0)
                _msgIndex = 0;

            if (attr != IntPtr.Zero)
                Marshal.WriteInt32(attr, 0);
            if (color != IntPtr.Zero)
                Marshal.WriteInt32(color, (int)nhcolor.NO_COLOR);

            string res = null;
            if (_msgIndex < _message_history.Count)
            {
                res = _message_history[_msgIndex].Text;
                if (attr != IntPtr.Zero)
                    Marshal.WriteInt32(attr, _message_history[_msgIndex].Attributes);
                if (color != IntPtr.Zero)
                    Marshal.WriteInt32(color, _message_history[_msgIndex].NHColor);
                _msgIndex++;
                if (_msgIndex < 0)
                    _msgIndex = 0;
            }

            return res;
        }

        public void ClientCallback_PutMsgHistory(string msg, int attr, int color, byte is_restoring)
        {
            if(msg != null)
                RawPrintEx(msg, attr, color);
        }

        public void ClientCallback_StartMenu(int winid, int style)
        {
            App.DebugWriteProfilingStopwatchTimeAndStart("StartMenu");
            lock (_ghWindowsLock)
            {
                if (_ghWindows[winid] != null)
                {
                    _ghWindows[winid].MenuInfo = new GHMenuInfo((ghmenu_styles)style);
                }
            }
        }
        public void ClientCallback_AddMenu(int winid, int glyph, Int64 identifier, char accel, char groupaccel, int attributes, string text, byte presel, int color)
        {
            ClientCallback_AddExtendedMenu(winid, glyph, identifier, accel, groupaccel, attributes, text, presel, color, 
                0, 0, 0, '\0', '\0', 0, 0, 0, IntPtr.Zero, IntPtr.Zero);
        }
        public void ClientCallback_AddExtendedMenu(int winid, int glyph, Int64 identifier, char accel, char groupaccel, int attributes, string text, byte presel, int color, 
            int maxcount, UInt64 oid, UInt64 mid, char headingaccel, char special_mark, ulong menuflags, byte dataflags, int style, IntPtr otmpdata_ptr, IntPtr otypdata_ptr)
        {
            App.DebugWriteProfilingStopwatchTimeAndStart("AddExtendedMenu");
            obj otmpdata = otmpdata_ptr == IntPtr.Zero ? new obj() : (obj)Marshal.PtrToStructure(otmpdata_ptr, typeof(obj));
            objclassdata otypdata = otypdata_ptr == IntPtr.Zero ? new objclassdata() : (objclassdata)Marshal.PtrToStructure(otypdata_ptr, typeof(objclassdata));

            lock (_ghWindowsLock)
            {
                if (_ghWindows[winid] != null && _ghWindows[winid].MenuInfo != null)
                {
                    GHMenuItem mi = new GHMenuItem(_ghWindows[winid].MenuInfo, App.NoGlyph, _gamePage);
                    mi.Identifier = identifier;
                    if (accel == 0 && identifier != 0)
                        mi.Accelerator = _ghWindows[winid].MenuInfo.AutoAccelerator;
                    else
                        mi.Accelerator = accel;
                    mi.GroupAccelerator = groupaccel;
                    mi.SpecialMark = special_mark;
                    mi.Attributes = attributes;
                    mi.Glyph = glyph;
                    mi.UseUpperSide = (menuflags & (ulong)MenuFlags.MENU_FLAGS_ACTIVE) != 0;
                    mi.UseColorForSuffixes = (menuflags & (ulong)MenuFlags.MENU_FLAGS_USE_COLOR_FOR_SUFFIXES) != 0;
                    mi.UseSpecialSymbols = (menuflags & (ulong)MenuFlags.MENU_FLAGS_USE_SPECIAL_SYMBOLS) != 0;
                    if ((dataflags & (byte)MenuDataFlags.MENU_DATAFLAGS_HAS_OBJECT_DATA) != 0)
                    {
                        ObjectDataItem odi = new ObjectDataItem(otmpdata, otypdata, (dataflags & (byte)MenuDataFlags.MENU_DATAFLAGS_HALLUCINATED) != 0);
                        mi.ObjData = odi;
                    }
                    mi.Text = text;
                    mi.Count = (presel != 0) ? -1 : 0;
                    mi.Selected = (presel != 0);
                    mi.MaxCount = maxcount;
//                    mi.SelectedSliderValue = maxcount + 1;
//                    mi.SelectedPickerIndex = maxcount + 1;
                    mi.NHColor = color;
                    mi.Oid = oid;
                    mi.Mid = mid;
                    mi.HeadingGroupAccelerator = headingaccel;
                    mi.MenuFlags = menuflags;
                    _ghWindows[winid].MenuInfo.MenuItems.Add(mi);
                }
            }
        }

        public void ClientCallback_EndMenu(int winid, string prompt, string subtitle)
        {
            App.DebugWriteProfilingStopwatchTimeAndStart("EndMenu");
            lock (_ghWindowsLock)
            {
                if (_ghWindows[winid] != null && _ghWindows[winid].MenuInfo != null)
                {
                    _ghWindows[winid].MenuInfo.Header = prompt;
                    _ghWindows[winid].MenuInfo.Subtitle = subtitle;
                }
            }
        }
        public int ClientCallback_SelectMenu(int winid, int how, out IntPtr picklistptr, out int listsize)
        {
            App.DebugWriteProfilingStopwatchTimeAndStart("SelectMenu");
            Debug.WriteLine("ClientCallback_SelectMenu");
            ConcurrentQueue<GHRequest> queue;
            bool enqueued = false;

            lock (_ghWindowsLock)
            {
                if (_ghWindows[winid] != null && _ghWindows[winid].MenuInfo != null)
                {
                    SelectionMode smode = (SelectionMode)how;
                    _ghWindows[winid].MenuInfo.SelectionHow = smode;

                    /* Clear menu response */
                    _ghWindows[winid].SelectedMenuItems = null;
                    _ghWindows[winid].WasCancelled = false;

                    if (ClientGame.RequestDictionary.TryGetValue(this, out queue))
                    {
                        queue.Enqueue(new GHRequest(this, GHRequestType.ShowMenuPage, _ghWindows[winid], _ghWindows[winid].MenuInfo));
                        enqueued = true;
                    }
                }
            }

            //lock (_gamePage.RefreshScreenLock)
            //{
            //    _gamePage.RefreshScreen = false;
            //}

            if(enqueued)
            {
                bool continuepolling = true;
                while (continuepolling)
                {
                    lock (_ghWindowsLock)
                    {
                        if (_ghWindows[winid] == null)
                            continuepolling = false;
                        else
                            continuepolling = (_ghWindows[winid].SelectedMenuItems == null);
                    }
                    if (!continuepolling)
                        break;

                    Thread.Sleep(GHConstants.PollingInterval);
                    pollResponseQueue();
                }
            }

            //lock (_gamePage.RefreshScreenLock)
            //{
            //    _gamePage.RefreshScreen = true;
            //}

            /* Handle result */
            int cnt = 0;
            Int64[] picklist = null;

            IntPtr arrayptr;

            lock (_ghWindowsLock)
            {
                if (_ghWindows[winid] == null || _ghWindows[winid].SelectedMenuItems == null || _ghWindows[winid].WasCancelled)
                    cnt = -1;
                else if (_ghWindows[winid].SelectedMenuItems.Count <= 0)
                    cnt = 0;
                else
                {
                    cnt = _ghWindows[winid].SelectedMenuItems.Count;
                    picklist = new Int64[cnt * 2];
                    for(int i = 0; i < cnt; i++)
                    {
                        picklist[2 * i] = _ghWindows[winid].SelectedMenuItems[i].Identifier;
                        picklist[2 * i + 1] = (Int64)_ghWindows[winid].SelectedMenuItems[i].Count;
                    }
                }
                Int64 i64var = 0;
                int size = (picklist == null ? 0 : picklist.Length);
                if (cnt < 1)
                    arrayptr = Marshal.AllocHGlobal(Marshal.SizeOf(i64var) * 1); /* One int */
                else
                {
                    arrayptr = Marshal.AllocHGlobal(Marshal.SizeOf(i64var) * size);
                    Marshal.Copy(picklist, 0, arrayptr, size);
                }
            }

            picklistptr = arrayptr;
            _outGoingIntPtr = arrayptr;
            listsize = cnt * 2;

            //lock (_gamePage.RefreshScreenLock)
            //{
            //    _gamePage.RefreshScreen = true;
            //}
            return cnt;
        }

        private IntPtr _outGoingIntPtr;

        public void ClientCallback_FreeMemory(ref IntPtr ptr)
        {
            IntPtr inComingIntPtr = ptr;
            Boolean issame = (inComingIntPtr == _outGoingIntPtr);
            if (!issame)
                Debug.WriteLine("Not same");

            Marshal.FreeHGlobal(issame ? ptr : _outGoingIntPtr);
        }

        public void ClientCallback_ReportPlayerName(string used_player_name)
        {
            if(used_player_name != null && used_player_name != "")
                Preferences.Set("LastUsedPlayerName", used_player_name);
        }

        private readonly object _gamePlayTimeLock = new object();
        private long _gamePlayTime = 0;
        public long GamePlayTime { get { lock (_gamePlayTimeLock) { return _gamePlayTime; } } set { lock (_gamePlayTimeLock) { _gamePlayTime = value; } } }

        private readonly object _sessionPlayTimeLock = new object();
        private long _sessionPlayTime = 0L;
        public long SessionPlayTime { get { lock (_sessionPlayTimeLock) { return _sessionPlayTime; } } set { lock (_sessionPlayTimeLock) { _sessionPlayTime = value; } } }
        public void AddSessionPlayTime(long addition)
        {
            lock (_sessionPlayTimeLock)
            {
                _sessionPlayTime = _sessionPlayTime + addition;
            }
        }

        public void ClientCallback_ReportPlayTime(long timePassed, long currentPlayTime)
        {
            long playedalready = Preferences.Get("RealPlayTime", 0L);
            long totaltime = playedalready + timePassed;
            Preferences.Set("RealPlayTime", totaltime);
            GamePlayTime = currentPlayTime;
            AddSessionPlayTime(timePassed);
            App.AddAggragateSessionPlayTime(timePassed);
        }

        public void ClientCallback_SendObjectData(int x, int y, IntPtr otmp_ptr, int cmdtype, int where, IntPtr otypdata_ptr, ulong oflags)
        {
            obj otmp = otmp_ptr == IntPtr.Zero ? new obj() : (obj)Marshal.PtrToStructure(otmp_ptr, typeof(obj));
            objclassdata otypdata = otypdata_ptr == IntPtr.Zero ? new objclassdata() : (objclassdata)Marshal.PtrToStructure(otypdata_ptr, typeof(objclassdata));
            _gamePage.AddObjectData(x, y, otmp, cmdtype, where, otypdata, oflags);
        }

        public void ClientCallback_SendMonsterData(int cmdtype, int x, int y, IntPtr monster_data_ptr, ulong oflags)
        {
            monst_info monster_data = monster_data_ptr == IntPtr.Zero ? new monst_info() : (monst_info)Marshal.PtrToStructure(monster_data_ptr, typeof(monst_info));
            switch (cmdtype)
            {
                case 0: /* Add Pet */
                    _gamePage.AddPetData(monster_data);
                    break;
            }
        }

        public string ClientCallback_GetLine(int style, int attr, int color, string query, string placeholder, string linesuffix, string introline)
        {
            Debug.WriteLine("ClientCallback_GetLine");
            if (query == null)
                query = "";

            ConcurrentQueue<GHRequest> queue;
            if (ClientGame.RequestDictionary.TryGetValue(this, out queue))
            {
                _getLineString = null;
                queue.Enqueue(new GHRequest(this, GHRequestType.GetLine, query, placeholder, linesuffix, introline, style, attr, color));
                while (_getLineString == null)
                {
                    Thread.Sleep(GHConstants.PollingInterval);
                    pollResponseQueue();
                }

                return _getLineString;
            }
            else
            {
                return "";
            }
        }

        public void ClientCallback_ClearContextMenu()
        {
            ConcurrentQueue<GHRequest> queue;
            if (ClientGame.RequestDictionary.TryGetValue(this, out queue))
            {
                queue.Enqueue(new GHRequest(this, GHRequestType.ClearContextMenu));
            }
        }

        public void ClientCallback_AddContextMenu(int cmd_def_char, int cmd_cur_char, int style, int glyph, string cmd_text, string target_text, int attr, int color)
        {
            ConcurrentQueue<GHRequest> queue;
            if (ClientGame.RequestDictionary.TryGetValue(this, out queue))
            {
                AddContextMenuData data = new AddContextMenuData();
                data.cmd_def_char = cmd_def_char;
                data.cmd_cur_char = cmd_cur_char;
                data.style = style;
                data.glyph = glyph;
                data.cmd_text = cmd_text;
                data.target_text = target_text;
                data.attr = attr;
                data.color = color;
                queue.Enqueue(new GHRequest(this, GHRequestType.AddContextMenu, data));
            }

        }

        public void ClientCallback_UpdateStatusButton(int cmd, int btn, int val, ulong bflags)
        {

        }

        public void ClientCallback_ToggleAnimationTimer(int timertype, int timerid, int state, int x, int y, int layer, ulong tflags)
        {
            lock (_gamePage.AnimationTimerLock)
            {
                bool ison = (state != 0);
                switch ((animation_timer_types)timertype)
                {
                    case animation_timer_types.ANIMATION_TIMER_GENERAL:
                        break;
                    case animation_timer_types.ANIMATION_TIMER_YOU:
                        _gamePage.AnimationTimers.u_action_animation_counter = 0L;
                        _gamePage.AnimationTimers.u_action_animation_counter_on = ison;
                        break;
                    case animation_timer_types.ANIMATION_TIMER_MONSTER:
                        _gamePage.AnimationTimers.m_action_animation_counter = 0L;
                        _gamePage.AnimationTimers.m_action_animation_counter_on = ison;
                        _gamePage.AnimationTimers.m_action_animation_x = (byte)x;
                        _gamePage.AnimationTimers.m_action_animation_y = (byte)y;
                        break;
                    case animation_timer_types.ANIMATION_TIMER_EXPLOSION:
                        _gamePage.AnimationTimers.explosion_animation_counter = 0L;
                        _gamePage.AnimationTimers.explosion_animation_counter_on = ison;
                        _gamePage.AnimationTimers.explosion_animation_x = (byte)x;
                        _gamePage.AnimationTimers.explosion_animation_y = (byte)y;
                        break;
                    case animation_timer_types.ANIMATION_TIMER_ZAP:
                        _gamePage.AnimationTimers.zap_animation_counter[timerid] = 0L;
                        _gamePage.AnimationTimers.zap_animation_counter_on[timerid] = ison;
                        _gamePage.AnimationTimers.zap_animation_x[timerid] = (byte)x;
                        _gamePage.AnimationTimers.zap_animation_y[timerid] = (byte)y;
                        break;
                    case animation_timer_types.ANIMATION_TIMER_SPECIAL_EFFECT:
                        _gamePage.AnimationTimers.special_effect_animation_counter[timerid] = 0L;
                        _gamePage.AnimationTimers.special_effect_animation_counter_on[timerid] = ison;
                        _gamePage.AnimationTimers.spef_action_animation_x[timerid] = (byte)x;
                        _gamePage.AnimationTimers.spef_action_animation_y[timerid] = (byte)y;
                        _gamePage.AnimationTimers.spef_action_animation_layer[timerid] = (layer_types)layer;
                        break;
                    default:
                        break;
                }
            }
        }

        public void ClientCallback_DisplayFloatingText(int x, int y, string text, int style, int attr, int color, ulong tflags)
        {
            ConcurrentQueue<GHRequest> queue;
            if (ClientGame.RequestDictionary.TryGetValue(this, out queue))
            {
                DisplayFloatingTextData floatingTextData = new DisplayFloatingTextData();
                floatingTextData.x = x;
                floatingTextData.y = y;
                floatingTextData.text = text;
                floatingTextData.style = style;
                floatingTextData.attr = attr;
                floatingTextData.color = color;
                floatingTextData.tflags = tflags;
                queue.Enqueue(new GHRequest(this, GHRequestType.DisplayFloatingText, floatingTextData));
            }
        }

        public void ClientCallback_DisplayScreenText(string text, string supertext, string subtext, int style, int attr, int color, ulong tflags)
        {
            _screenTextSet = false;
            ConcurrentQueue<GHRequest> queue;
            if (ClientGame.RequestDictionary.TryGetValue(this, out queue))
            {
                if((tflags & 2UL) != 0)
                {
                    DisplayConditionTextData data = new DisplayConditionTextData();
                    data.text = text.Trim();
                    data.style = style;
                    data.attr = attr;
                    data.color = color;
                    data.tflags = tflags;
                    queue.Enqueue(new GHRequest(this, GHRequestType.DisplayConditionText, data));
                }
                else
                {
                    DisplayScreenTextData data = new DisplayScreenTextData();
                    data.text = text;
                    data.supertext = supertext;
                    data.subtext = subtext;
                    data.style = style;
                    data.attr = attr;
                    data.color = color;
                    data.tflags = tflags;
                    queue.Enqueue(new GHRequest(this, GHRequestType.DisplayScreenText, data));
                }

                if ((tflags & 1UL) != 0)
                {
                    /* Blocking call */
                    while (!_screenTextSet)
                    {
                        Thread.Sleep(GHConstants.PollingInterval);
                        pollResponseQueue();
                    }

                    int cnt = 0;
                    long countervalue;
                    do
                    {
                        countervalue = _gamePage.MainCounterValue;
                        lock (_gamePage._screenTextLock)
                        {
                            if (_gamePage._screenText != null)
                            {
                                if (_gamePage._screenText.IsFinished(countervalue))
                                    break;
                            }
                            else
                                break;
                        }
                        Thread.Sleep(GHConstants.PollingInterval);
                        cnt++;
                    } while (cnt < 2000);
                }
            }
        }

        public void ClientCallback_DisplayPopupText(string text, string title, int style, int attr, int color, int glyph, ulong tflags)
        {
            ConcurrentQueue<GHRequest> queue;
            if (ClientGame.RequestDictionary.TryGetValue(this, out queue))
            {
                DisplayScreenTextData data = new DisplayScreenTextData();
                data.text = text;
                data.supertext = title;
                data.subtext = title;
                data.style = style;
                data.attr = attr;
                data.color = color;
                data.glyph = glyph;
                data.tflags = tflags;
                queue.Enqueue(new GHRequest(this, GHRequestType.DisplayPopupText, data));
            }

            int val = ClientCallback_nhgetch();
            if (ClientGame.RequestDictionary.TryGetValue(this, out queue))
            {
                queue.Enqueue(new GHRequest(this, GHRequestType.HidePopupText));
            }
        }

        public void ClientCallback_DisplayGUIEffect(int x, int y, int style, ulong tflags)
        {
            ConcurrentQueue<GHRequest> queue;
            if (ClientGame.RequestDictionary.TryGetValue(this, out queue))
            {
                DisplayGUIEffectData effectData = new DisplayGUIEffectData();
                effectData.x = x;
                effectData.y = y;
                effectData.style = style;
                effectData.tflags = tflags;
                queue.Enqueue(new GHRequest(this, GHRequestType.DisplayGUIEffect, effectData));
            }
        }

        public void ClientCallback_UpdateCursor(int style, int force_paint, int show_on_u)
        {
            _gamePage.UpdateCursor(style, force_paint, show_on_u);
        }

        public int ClientCallback_PlayImmediateSound(int ghsound, string eventPath, int bankid, double eventVolume, double soundVolume, string[] parameterNames, float[] parameterValues, int arraysize, int sound_type, int play_group, uint dialogue_mid, ulong play_flags)
        {
            if(App.FmodService != null && !App.IsMuted)
            {
                return App.FmodService.PlayImmediateSound(ghsound, eventPath, bankid, (float)eventVolume, (float)soundVolume, parameterNames, parameterValues, arraysize, sound_type, play_group, dialogue_mid, play_flags);
            }

            return 1;
        }

        public int ClientCallback_PlayMusic(int ghsound, string eventPath, int bankid, double eventVolume, double soundVolume)
        {
            if (App.FmodService != null)
            {
                return App.FmodService.PlayMusic(ghsound, eventPath, bankid, (float)eventVolume, (float)soundVolume);
            }

            return 1;
        }
        public int ClientCallback_PlayLevelAmbient(int ghsound, string eventPath, int bankid, double eventVolume, double soundVolume)
        {
            if (App.FmodService != null)
            {
                return App.FmodService.PlayLevelAmbient(ghsound, eventPath, bankid, (float)eventVolume, (float)soundVolume);
            }

            return 1;
        }

        public int ClientCallback_PlayEnvironmentAmbient(int ghsound, string eventPath, int bankid, double eventVolume, double soundVolume)
        {
            if (App.FmodService != null)
            {
                return App.FmodService.PlayEnvironmentAmbient(ghsound, eventPath, bankid, (float)eventVolume, (float)soundVolume);
            }

            return 1;
        }

        public int ClientCallback_PlayOccupationAmbient(int ghsound, string eventPath, int bankid, double eventVolume, double soundVolume)
        {
            if (App.FmodService != null)
            {
                return App.FmodService.PlayOccupationAmbient(ghsound, eventPath, bankid, (float)eventVolume, (float)soundVolume);
            }

            return 1;
        }

        public int ClientCallback_PlayEffectAmbient(int ghsound, string eventPath, int bankid, double eventVolume, double soundVolume)
        {
            if (App.FmodService != null)
            {
                return App.FmodService.PlayEffectAmbient(ghsound, eventPath, bankid, (float)eventVolume, (float)soundVolume);
            }

            return 1;
        }

        public int ClientCallback_SetEffectAmbientVolume(double soundVolume)
        {
            if (App.FmodService != null)
            {
                return App.FmodService.SetEffectAmbientVolume((float)soundVolume);
            }

            return 1;
        }

        public int ClientCallback_AddAmbientSound(int ghsound, string eventPath, int bankid, double eventVolume, double soundVolume, out UInt64 soundSourceId)
        {
            if (App.FmodService != null)
            {
                return App.FmodService.AddAmbientSound(ghsound, eventPath, bankid, (float)eventVolume, (float)soundVolume, out soundSourceId);
            }
            soundSourceId = 0;
            return 1;
        }
        public int ClientCallback_DeleteAmbientSound(UInt64 soundSourceId)
        {
            if (App.FmodService != null)
            {
                return App.FmodService.DeleteAmbientSound(soundSourceId);
            }

            return 1;
        }
        public int ClientCallback_SetAmbientSoundVolume(UInt64 soundSourceId, double soundVolume)
        {
            if (App.FmodService != null)
            {
                return App.FmodService.SetAmbientSoundVolume(soundSourceId, (float)soundVolume);
            }

            return 1;
        }

        public int ClientCallback_StopAllSounds(uint flags, uint dialogue_mid)
        {
            if (App.FmodService != null)
            {
                return App.FmodService.StopAllSounds(flags, dialogue_mid);
            }

            return 1;
        }

        public void ClientCallback_IssueGuiCommand(int cmdtype)
        {
            ConcurrentQueue<GHRequest> queue;
            switch (cmdtype)
            {
                case (int)gui_command_types.GUI_CMD_LOAD_GLYPHS:
                    /* Reinitialize  glyph2tile after object shuffling */
                    IntPtr gl2ti_ptr;
                    int gl2ti_size;
                    IntPtr gltifl_ptr;
                    int gltifl_size;

                    _gamePage.GnollHackService.GetGlyphArrays(out gl2ti_ptr, out gl2ti_size, out gltifl_ptr, out gltifl_size);
                    lock (App.Glyph2TileLock)
                    {
                        if (gl2ti_ptr != null && gl2ti_size > 0)
                        {
                            if(App.Glyph2Tile == null || gl2ti_size != App.Glyph2Tile.Length)
                                App.Glyph2Tile = new int[gl2ti_size];
                            Marshal.Copy(gl2ti_ptr, App.Glyph2Tile, 0, gl2ti_size);
                        }
                        if (gltifl_ptr != null && gltifl_size > 0)
                        {
                            if (App.GlyphTileFlags == null || gltifl_size != App.GlyphTileFlags.Length)
                                App.GlyphTileFlags = new byte[gltifl_size];
                            Marshal.Copy(gltifl_ptr, App.GlyphTileFlags, 0, gltifl_size);
                        }
                    }
                    break;
                case (int)gui_command_types.GUI_CMD_FADE_TO_BLACK:
                    if (ClientGame.RequestDictionary.TryGetValue(this, out queue))
                    {
                        queue.Enqueue(new GHRequest(this, GHRequestType.FadeToBlack, 200));
                        Thread.Sleep(325);
                    }
                    break;
                case (int)gui_command_types.GUI_CMD_COLLECT_GARBAGE:
                    GC.Collect();
                    GC.WaitForPendingFinalizers();
                    GC.Collect();
                    break;
                case (int)gui_command_types.GUI_CMD_FADE_FROM_BLACK:
                    if (ClientGame.RequestDictionary.TryGetValue(this, out queue))
                    {
                        queue.Enqueue(new GHRequest(this, GHRequestType.FadeFromBlack, 200));
                    }
                    break;
                case (int)gui_command_types.GUI_CMD_FORCE_ASCII:
                    _gamePage.ForceAscii = true;
                    break;
                case (int)gui_command_types.GUI_CMD_UNFORCE_ASCII:
                    _gamePage.ForceAscii = false;
                    break;
                case (int)gui_command_types.GUI_CMD_MUTE_SOUNDS:
                    App.GameMuteMode = true;
                    break;
                case (int)gui_command_types.GUI_CMD_UNMUTE_SOUNDS:
                    App.GameMuteMode = false;
                    break;
                case (int)gui_command_types.GUI_CMD_ACTIVATE_QUIETER_MODE:
                    App.FmodService.SetQuieterMode(true);
                    break;
                case (int)gui_command_types.GUI_CMD_DEACTIVATE_QUIETER_MODE:
                    App.FmodService.SetQuieterMode(false);
                    break;
                case (int)gui_command_types.GUI_CMD_LOAD_VIDEOS:
                    break;
                case (int)gui_command_types.GUI_CMD_ENABLE_WIZARD_MODE:
                    _gamePage.EnableWizardMode = true;
                    _gamePage.ExtendedCommands = _gamePage.GnollHackService.GetExtendedCommands();
                    break;
                case (int)gui_command_types.GUI_CMD_ENABLE_CASUAL_MODE:
                    _gamePage.EnableCasualMode = true;
                    _gamePage.ExtendedCommands = _gamePage.GnollHackService.GetExtendedCommands();
                    break;
                case (int)gui_command_types.GUI_CMD_CLEAR_PET_DATA:
                    _gamePage.ClearPetData();
                    break;
                case (int)gui_command_types.GUI_CMD_SAVE_AND_DISABLE_TRAVEL_MODE:
                    if (ClientGame.RequestDictionary.TryGetValue(this, out queue))
                    {
                        queue.Enqueue(new GHRequest(this, GHRequestType.SaveAndDisableTravelMode));
                    }
                    break;
                case (int)gui_command_types.GUI_CMD_RESTORE_TRAVEL_MODE:
                    if (ClientGame.RequestDictionary.TryGetValue(this, out queue))
                    {
                        queue.Enqueue(new GHRequest(this, GHRequestType.RestoreTravelMode));
                    }
                    break;
                case (int)gui_command_types.GUI_CMD_SAVE_AND_DISABLE_TRAVEL_MODE_ON_LEVEL:
                    if (ClientGame.RequestDictionary.TryGetValue(this, out queue))
                    {
                        queue.Enqueue(new GHRequest(this, GHRequestType.SaveAndDisableTravelModeOnLevel));
                    }
                    break;
                case (int)gui_command_types.GUI_CMD_RESTORE_TRAVEL_MODE_ON_LEVEL:
                    if (ClientGame.RequestDictionary.TryGetValue(this, out queue))
                    {
                        queue.Enqueue(new GHRequest(this, GHRequestType.RestoreTravelModeOnLevel));
                    }
                    break;
                case (int)gui_command_types.GUI_CMD_CLEAR_CONDITION_TEXTS:
                    _gamePage.ClearConditionTexts();
                    break;
                case (int)gui_command_types.GUI_CMD_CLEAR_FLOATING_TEXTS:
                    _gamePage.ClearFloatingTexts();
                    break;
                case (int)gui_command_types.GUI_CMD_CLEAR_GUI_EFFECTS:
                    _gamePage.ClearGuiEffects();
                    break;
                case (int)gui_command_types.GUI_CMD_CLEAR_MESSAGE_HISTORY:
                    _message_history.Clear();
                    if (ClientGame.RequestDictionary.TryGetValue(this, out queue))
                    {
                        queue.Enqueue(new GHRequest(this, GHRequestType.PrintHistory, new List<GHMsgHistoryItem>()));
                    }
                    break;
                case (int)gui_command_types.GUI_CMD_LOAD_INTRO_SOUND_BANK:
                    if(App.LoadBanks)
                        App.FmodService.LoadIntroSoundBank();
                    break;
                case (int)gui_command_types.GUI_CMD_UNLOAD_INTRO_SOUND_BANK:
                    if (App.LoadBanks)
                        App.FmodService.UnloadIntroSoundBank();
                    break;
                case (int)gui_command_types.GUI_CMD_WAIT_FOR_RESUME:
                    App.GameSaved = true;
                    App.SavingGame = false;
                    while (!_restoreRequested)
                    {
                        Thread.Sleep(GHConstants.PollingInterval);
                        _saveRequested = false; //Should be the case. but just in case there is some sort of a mixup going on so that we do not save and restore again in pollResponseQueue
                        pollResponseQueue();
                    }
                    _restoreRequested = false;
                    break;
                default:
                    break;
            }
        }

        public void ClientCallback_OutRip(int winid, string plname, int points, string killer, string time)
        {
            ConcurrentQueue<GHRequest> queue;
            lock (_ghWindowsLock)
            {
                if (_ghWindows[winid] != null)
                {
                    if (ClientGame.RequestDictionary.TryGetValue(this, out queue))
                    {
                        queue.Enqueue(new GHRequest(this, GHRequestType.ShowOutRipPage, _ghWindows[winid], new GHOutRipInfo(plname, points, killer, time)));
                    }
                }
            }

            int res = ClientCallback_nhgetch();
        }

        public int ClientCallback_UIHasInput()
        {
            ConcurrentQueue<GHResponse> queue;
            if (ClientGame.ResponseDictionary.TryGetValue(this, out queue))
            {
                return queue.Count;
            }
            return 0;
        }

        public byte ClientCallback_AdjustGeneralVolumes(double value1, double value2, double value3, double value4, double value5, double value6)
        {
            /* Unused; handled on the GUI side completely */
            return 0;
        }

        public int ClientCallback_OpenSpecialView(int viewtype, string text, string title, int attr, int color)
        {
            switch (viewtype)
            {
                case (int)special_view_types.SPECIAL_VIEW_CHAT_MESSAGE:
                    break;
                case (int)special_view_types.SPECIAL_VIEW_SHOW_SPECIAL_EFFECT:
                    if (!App.SponsorButtonVisited)
                    {
                        App.ShowSpecialEffect = true;
                        Preferences.Set("ShowSpecialEffect", true);
                    }
                    break;
                case (int)special_view_types.SPECIAL_VIEW_GUI_TIPS:
                    {
                        if (Preferences.Get("GUITipsShown", false))
                            break;

                        ConcurrentQueue<GHRequest> queue;
                        if (ClientGame.RequestDictionary.TryGetValue(this, out queue))
                        {
                            _guiTipsFinished = false;
                            queue.Enqueue(new GHRequest(this, GHRequestType.ShowGUITips));
                            while (!_guiTipsFinished)
                            {
                                Thread.Sleep(GHConstants.PollingInterval);
                                pollResponseQueue();
                            }
                        }
                        break;
                    }
                case (int)special_view_types.SPECIAL_VIEW_CRASH_DETECTED:
                    {
                        ConcurrentQueue<GHRequest> queue;
                        if (ClientGame.RequestDictionary.TryGetValue(this, out queue))
                        {
                            _crashReportFinished = false;
                            queue.Enqueue(new GHRequest(this, GHRequestType.CrashReport));
                            while (!_crashReportFinished)
                            {
                                Thread.Sleep(GHConstants.PollingInterval);
                                pollResponseQueue();
                            }
                        }
                        break;
                    }
                case (int)special_view_types.SPECIAL_VIEW_PANIC:
                    {
                        ConcurrentQueue<GHRequest> queue;
                        if (ClientGame.RequestDictionary.TryGetValue(this, out queue))
                        {
                            _panicFinished = false;
                            queue.Enqueue(new GHRequest(this, GHRequestType.Panic, text));
                            while (!_panicFinished)
                            {
                                Thread.Sleep(GHConstants.PollingInterval);
                                pollResponseQueue();
                            }
                        }
                        break;
                    }
                case (int)special_view_types.SPECIAL_VIEW_DEBUGLOG:
                    if (text != null)
                        Debug.WriteLine(text);
                    break;
                case (int)special_view_types.SPECIAL_VIEW_MESSAGE:
                    {
                        ConcurrentQueue<GHRequest> queue;
                        if (ClientGame.RequestDictionary.TryGetValue(this, out queue))
                        {
                            _messageFinished = false;
                            queue.Enqueue(new GHRequest(this, GHRequestType.Message, text));
                            while (!_messageFinished)
                            {
                                Thread.Sleep(GHConstants.PollingInterval);
                                pollResponseQueue();
                            }
                        }

                        break;
                    }
                case (int)special_view_types.SPECIAL_VIEW_HELP_DIR:
                    break;
                case (int)special_view_types.SPECIAL_VIEW_GUI_YN_CONFIRMATION:
                    {
                        ConcurrentQueue<GHRequest> queue;
                        if (ClientGame.RequestDictionary.TryGetValue(this, out queue))
                        {
                            _ynConfirmationFinished = false;
                            queue.Enqueue(new GHRequest(this, GHRequestType.YnConfirmation, title, text, "Yes", "No"));
                            while (!_ynConfirmationFinished)
                            {
                                Thread.Sleep(GHConstants.PollingInterval);
                                pollResponseQueue();
                            }
                            return _ynConfirmationResult ? 1 : 0;
                        }
                        break;
                    }
                default:
                    break;
            }
            return 0;
        }


        /* Dummies */
        public void ClientCallback_VoidVoidDummy()
        {

        }
        public void ClientCallback_VoidCharDummy(string value)
        {

        }
        public void ClientCallback_VoidConstCharDummy(string value)
        {

        }
        public int ClientCallback_IntIntDummy(int value1)
        {
            return 0;
        }
        public void ClientCallback_VoidIntDummy(int value1)
        {

        }
        public void ClientCallback_VoidIntIntDummy(int value1, int value2)
        {

        }
        public void ClientCallback_VoidIntIntIntDummy(int value1, int value2, int value3)
        {

        }
        public void ClientCallback_VoidIntBooleanDummy(int value1, byte value2)
        {

        }
        public void ClientCallback_VoidIntCharDummy(int value1, string value2)
        {

        }
        public void ClientCallback_VoidIntIntConstCharDummy(int value1, int value2, string value3)
        {

        }
        public void ClientCallback_VoidIntIntConstCharIntDummy(int value1, int value2, string value3, int value4)
        {

        }
        public void ClientCallback_VoidConstCharIntDummy(string value1, int value2)
        {

        }
        public void ClientCallback_VoidConstCharBooleanDummy(string value1, byte value2)
        {

        }
        public int ClientCallback_IntVoidDummy()
        {
            return 0;
        }
        public byte ClientCallback_BooleanVoidDummy()
        {
            return 0;
        }

        public string ClientCallback_GetCwd()
        {
            return Directory.GetCurrentDirectory();
        }
        public int ClientCallback_MessageBox(string text, string title, uint type)
        {
            return 1;
        }
        public int ClientCallback_IntIntIntVoidPtrDummy(int value1, int value2, IntPtr value3)
        {
            return 0;
        }
        public int ClientCallback_IntIntPtrIntPtrIntPtrDummy(ref int value1, ref int value2, ref int value3)
        {
            return 0;
        }
        public void ClientCallback_VoidIntIntPtrIntIntIntUlongPtrDummy(int value1, ref int value2, int value3, int value4, int value5, ref ulong value6)
        {
            return;
        }
        public void ClientCallback_VoidUlongDummy(ulong value1)
        {

        }
        public byte ClientCallback_BooleanIntDoubleDummy(int value1, double value2)
        {
            return 0;
        }
        public byte ClientCallback_BooleanDoubleDummy(double value1)
        {
            return 0;
        }
        public byte ClientCallback_BooleanIntDoubleVoidPtrDummy(int value1, double value2, IntPtr value3)
        {
            return 0;
        }
        public byte ClientCallback_BooleanDoubleVoidPtrDummy(double value1, IntPtr value2)
        {
            return 0;
        }
        public byte ClientCallback_BooleanVoidPtrDummy(IntPtr value1)
        {
            return 0;
        }
        public int ClientCallback_IntConstCharPtrConstCharPtrCharPtrDummy(string value1, string value2, string value3)
        {
            return 0;
        }
        public void ClientCallback_VoidConstCharPtrCharPtrDummy(string value1, string value2)
        {
            return;
        }
        public string ClientCallback_CharPtrBooleanDummy(byte value1)
        {
            return null;
        }
        public string ClientCallback_CharVoidDummy()
        {
            return "";
        }
        public void ClientCallback_VoidIntIntIntIntIntDummy(int value1, int value2, int value3, int value4, int value5)
        {
            return;
        }
        public void ClientCallback_VoidIntConstCharPtrConstCharPtrBooleanDummy(int value1, string value2, string value3, byte value4)
        {
            return;
        }


        bool _saveRequested = false;
        bool _restoreRequested = false;
        /* Other functions */
        private void RequestSaveGame()
        {
            _saveRequested = true;
        }

        private void RequestRestoreSavedGame()
        {
            _restoreRequested = true;
        }

        bool _timeTallyRequested = false;
        private void RequestTallyRealTime()
        {
            _timeTallyRequested = true;
        }
    }
}
