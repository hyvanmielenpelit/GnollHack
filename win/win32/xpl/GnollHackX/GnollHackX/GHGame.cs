using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Text;
using System.Threading;
#if GNH_MAUI
using GnollHackM;
#else
using GnollHackX.Pages.Game;
using Xamarin.Forms;
using Xamarin.Forms.Xaml;
using Xamarin.Essentials;
#endif
using System.Collections.Concurrent;
using System.Runtime.InteropServices;
using Newtonsoft.Json;
using System.Linq;
using System.Drawing;
using System.Runtime.InteropServices.ComTypes;
using System.IO.Compression;
using static System.Net.Mime.MediaTypeNames;
using System.Collections;
using System.Security.Cryptography;
using SkiaSharp;
using System.Threading.Tasks;
#if SENTRY
using Sentry.Protocol;
#endif

namespace GnollHackX
{
    public sealed class GHGame
    {
        public readonly RunGnollHackFlags StartFlags;
        public bool PlayingReplay { get { return (StartFlags & RunGnollHackFlags.PlayingReplay) != 0; } }
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
        private bool _useLongerMessageHistory = false;
        private bool _useHideMessageHistory = false;
        private int _saveFileTrackingFinished = -1;
        private bool _abortShowMenuPage = false;

        public GamePage ActiveGamePage => GHApp.CurrentGamePage;
        //{
        //    get { return Interlocked.CompareExchange(ref _gamePage, null, null); }
        //    set { Interlocked.Exchange(ref _gamePage, value); }
        //}

        private bool _touchLocSet = false;
        private int _touchLocX;
        private int _touchLocY;
        private int _touchLocMod;

        private int _lastWindowHandle = 0;
        private GHWindow[] _ghWindows = new GHWindow[GHConstants.MaxGHWindows];

        private int _mapWindowId;
        private int _messageWindowId;
        private int _statusWindowId;

        private GHMsgHistoryList _longer_message_history = new GHMsgHistoryList(GHConstants.MaxLongerMessageHistoryLength, GHConstants.MaxLongerMessageHistoryLength / 2);

        public readonly ConcurrentQueue<GHRequest> RequestQueue = new ConcurrentQueue<GHRequest>();
        public readonly ConcurrentQueue<GHResponse> ResponseQueue = new ConcurrentQueue<GHResponse>();

        public string CharacterName 
        {
            get { return Interlocked.CompareExchange(ref _characterName, null, null); }
            set { Interlocked.Exchange(ref _characterName, value); }
        }
        public bool WizardMode { get { return ActiveGamePage?.EnableWizardMode ?? false; } }
        public bool CasualMode { get { return ActiveGamePage?.EnableCasualMode ?? false; } }
        public bool ModernMode { get { return ActiveGamePage?.EnableModernMode ?? false; } }

        public GHGame(RunGnollHackFlags startFlags)
        {
            StartFlags = startFlags;

            lock (_mapDataBufferLock)
            {
                for (int i = 0; i < GHConstants.MapCols; i++)
                {
                    for (int j = 0; j < GHConstants.MapRows; j++)
                    {
                        _mapDataBuffer1[i, j] = _mapDataBuffer2[i, j] = _mapDataMaster[i, j] = new MapData();
                        _mapDataBuffer1[i, j].Glyph = _mapDataBuffer2[i, j].Glyph = _mapDataMaster[i, j].Glyph = GHApp.UnexploredGlyph;
                        _mapDataBuffer1[i, j].BkGlyph = _mapDataBuffer2[i, j].BkGlyph = _mapDataMaster[i, j].BkGlyph = GHApp.NoGlyph;
                        _mapDataBuffer1[i, j].NeedsUpdate = _mapDataBuffer2[i, j].NeedsUpdate = _mapDataMaster[i, j].NeedsUpdate = true;

                        _objectDataMaster[i, j] = new ObjectData();
                        _objectDataBuffer1[i, j] = new ObjectData();
                        _objectDataBuffer2[i, j] = new ObjectData();
                    }
                }
            }
            _mapDataCurrent = _mapDataBuffer1;
            _objectDataCurrent = _objectDataBuffer1;

            GamePage gamePage = GHApp.CurrentGamePage;
            if(gamePage != null)
                _useLongerMessageHistory = gamePage.LongerMessageHistory;
        }

        public void ReactivateGame()
        {
            GamePage gamePage = ActiveGamePage;
            if (gamePage == null)
                return;

            int x, y;
            lock (_mapDataBufferLock)
            {
                x = _ux;
                y = _uy;
            }
            gamePage.SetTargetClip(x, y, true, MainCounterValue);
            for (int winHandle = 0; winHandle <= _lastWindowHandle && winHandle < GHConstants.MaxGHWindows; winHandle++)
            {
                if(_ghWindows[winHandle] != null)
                    gamePage.UpdateGHWindow(winHandle, _ghWindows[winHandle].Clone());
            }
        }

        ~GHGame()
        {

        }

        private bool _gameHasEnded = false;
        private bool _fastForwardGameOver = false;
        private void GameOverHandling()
        {
            if(_gameHasEnded && GHApp.OperatingSystemKillsAppsOnBackground)
                _fastForwardGameOver = true;
        }

        private void PollResponseQueue()
        {
            /* Makes sure that whatever is happening, the game gets saved and then restored upon sleep / restore */
            if(_saveRequested)
            {
                _saveRequested = false;
                _checkPointRequested = false;
                if (!GHApp.CancelSaveGame)
                {
                    GHApp.SavingGame = true;
                    GHApp.GnollHackService?.SaveAndRestoreSavedGame(GHApp.AppSwitchSaveStyle);
                    GameOverHandling();
                }
                GHApp.GameSaved = false;
                GHApp.SavingGame = false;
                SaveDoneConfirmed = true;
            }
            else if (_checkPointRequested)
            {
                _checkPointRequested = false;
                GHApp.GnollHackService?.SaveAndRestoreSavedGame(2); /* Check point and return immediately */
                GameOverHandling();
            }

            if (_timeTallyRequested)
            {
                _timeTallyRequested = false;
                GHApp.GnollHackService?.TallyRealTime();
            }

            GHApp.SaveDiscoveredMusic();
            GHApp.FmodService?.PollTasks();  

            GHResponse response;
            while (ResponseQueue.TryDequeue(out response))
            {
                switch (response.RequestType)
                {
                    case GHRequestType.AskName:
                        CharacterName = response.ResponseStringValue;
                        _characternameSet = true;
                        break;
                    case GHRequestType.GetChar:
                        _inputBufferLocation++;
                        if (_inputBufferLocation >= GHConstants.InputBufferLength)
                            _inputBufferLocation = GHConstants.InputBufferLength - 1;
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
                        if (response.RequestingGHWindow != null)
                        {
                            if (response.SelectedMenuItems != null)
                                response.RequestingGHWindow.SelectedMenuItems = response.SelectedMenuItems;
                            else
                                response.RequestingGHWindow.SelectedMenuItems = new List<GHMenuItem>(); /* Empty selection */

                            response.RequestingGHWindow.WasCancelled = response.ResponseBoolValue;
                        }
                        else
                        {
                            _abortShowMenuPage = true;
                        }
                        break;
                    case GHRequestType.DisplayScreenText:
                        _screenTextSet = true;
                        break;
                    case GHRequestType.SetPetMID:
                        GHApp.GnollHackService?.SetPetMID(response.ResponseUIntValue);
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
                    case GHRequestType.SaveInsuranceCheckPoint:
                        RequestCheckPoint();
                        break;
                    case GHRequestType.StopWaitAndRestoreSavedGame:
                        RequestRestoreSavedGame();
                        break;
                    case GHRequestType.ForceExitThread:
                        ForceExitThread();
                        break;
                    case GHRequestType.TerminateGnollHackAndRestartGameInNewGamePage:
                        ForceTerminateGnollHack(exit_hack_types.EXITHACK_RECOVER_NEW);
                        break;
                    case GHRequestType.TallyRealTime:
                        RequestTallyRealTime();
                        break;
                    case GHRequestType.UseLongerMessageHistory:
                        if (_useLongerMessageHistory != response.ResponseBoolValue)
                        {
                            _useLongerMessageHistory = response.ResponseBoolValue;
                            UpdateMessageHistory();
                        }
                        break;
                    case GHRequestType.UseHideMessageHistory:
                        if (_useHideMessageHistory != response.ResponseBoolValue)
                        {
                            _useHideMessageHistory = response.ResponseBoolValue;
                            UpdateMessageHistory();
                        }
                        break;
                    case GHRequestType.EndReplayFile:
                        EndReplayFile();
                        break;
                    case GHRequestType.SetCharacterClickAction:
                        GHApp.GnollHackService?.SetCharacterClickAction(response.ResponseBoolValue);
                        break;
                    case GHRequestType.SetGetPositionArrows:
                        GHApp.GnollHackService?.SetGetPositionArrows(response.ResponseBoolValue);
                        break;
                    case GHRequestType.SetDiceAsRanges:
                        GHApp.GnollHackService?.SetDiceAsRanges(response.ResponseBoolValue);
                        break;
                    case GHRequestType.SetAutoDig:
                        GHApp.GnollHackService?.SetAutoDig(response.ResponseBoolValue);
                        break;
                    case GHRequestType.SetIgnoreStopping:
                        GHApp.GnollHackService?.SetIgnoreStopping(response.ResponseBoolValue);
                        break;
                    case GHRequestType.SetRightMouseCommand:
                    case GHRequestType.SetMiddleMouseCommand:
                        GHApp.GnollHackService?.SetMouseCommand(response.ResponseIntValue, response.RequestType == GHRequestType.SetMiddleMouseCommand);
                        break;
                    case GHRequestType.SaveFileTrackingLoad:
                    case GHRequestType.SaveFileTrackingSave:
                        _saveFileTrackingFinished = response.ResponseIntValue;
                        break;
                    case GHRequestType.StopAllGameSounds:
                        GHApp.FmodService?.StopAllGameSounds((uint)StopSoundFlags.All, 0U);
                        break;
                    case GHRequestType.SetVolume:
                        GHApp.FmodService?.AdjustGameVolumes(response.GeneralVolume, response.MusicVolume, response.AmbientVolume, 
                            response.DialogueVolume, response.EffectsVolume, response.GameUIVolume);
                        break;
                    default:
                        break;
                }
            }
        }

        public GHMsgHistorySpan GetMessages()
        {
            return _longer_message_history.CurrentSpan;
        }

        public void ClientCallback_InitWindows()
        {
            Debug.WriteLine("ClientCallback_InitWindows");
            int animoff = 0, enloff = 0, reoff = 0, general_tile_off = 0, hit_tile_off = 0, ui_tile_off = 0, spell_tile_off = 0, skill_tile_off = 0, command_tile_off = 0, buff_tile_off = 0, cursor_off = 0;

            if (!PlayingReplay)
            {
                /* In replay, the arrays are not initialized by the game, so should not fetch them, but read them from the replay file. */
                /* Initialize now glyph2tile and other relevant arrays */
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
                GHApp.GnollHackService.GetTileArrays(out ti2an_ptr, out ti2an_size, out ti2en_ptr, out ti2en_size, out ti2ad_ptr, out ti2ad_size,
                    out anoff_ptr, out anoff_size, out enoff_ptr, out enoff_size, out reoff_ptr, out reoff_size);
                lock (GHApp.Glyph2TileLock)
                {
                    if (ti2an_ptr != IntPtr.Zero && ti2an_size > 0)
                    {
                        GHApp.Tile2Animation = new short[ti2an_size];
                        Marshal.Copy(ti2an_ptr, GHApp.Tile2Animation, 0, ti2an_size);
                    }
                    if (ti2en_ptr != IntPtr.Zero && ti2en_size > 0)
                    {
                        GHApp.Tile2Enlargement = new short[ti2en_size];
                        Marshal.Copy(ti2en_ptr, GHApp.Tile2Enlargement, 0, ti2en_size);
                    }
                    if (ti2ad_ptr != IntPtr.Zero && ti2ad_size > 0)
                    {
                        GHApp.Tile2Autodraw = new short[ti2ad_size];
                        Marshal.Copy(ti2ad_ptr, GHApp.Tile2Autodraw, 0, ti2ad_size);
                    }
                    if (anoff_ptr != IntPtr.Zero && anoff_size > 0)
                    {
                        GHApp.AnimationOffsets = new int[anoff_size];
                        Marshal.Copy(anoff_ptr, GHApp.AnimationOffsets, 0, anoff_size);
                    }
                    if (enoff_ptr != IntPtr.Zero && enoff_size > 0)
                    {
                        GHApp.EnlargementOffsets = new int[enoff_size];
                        Marshal.Copy(enoff_ptr, GHApp.EnlargementOffsets, 0, enoff_size);
                    }
                    if (reoff_ptr != IntPtr.Zero && reoff_size > 0)
                    {
                        GHApp.ReplacementOffsets = new int[reoff_size];
                        Marshal.Copy(reoff_ptr, GHApp.ReplacementOffsets, 0, reoff_size);
                    }
                    GHApp.GnollHackService.GetOffs(out animoff, out enloff, out reoff, out general_tile_off, out hit_tile_off, out ui_tile_off, out spell_tile_off, out skill_tile_off, out command_tile_off, out buff_tile_off,
                        out cursor_off);
                    GHApp.AnimationOff = animoff;
                    GHApp.EnlargementOff = enloff;
                    GHApp.ReplacementOff = reoff;
                    GHApp.GeneralTileOff = general_tile_off;
                    GHApp.HitTileOff = hit_tile_off;
                    GHApp.UITileOff = ui_tile_off;
                    GHApp.SpellTileOff = spell_tile_off;
                    GHApp.SkillTileOff = skill_tile_off;
                    GHApp.CommandTileOff = command_tile_off;
                    GHApp.BuffTileOff = buff_tile_off;
                    GHApp.CursorOff = cursor_off;
                }

                int total_tiles_used = GHApp.GnollHackService?.GetTotalTiles() ?? 0;
                int total_sheets_used = Math.Min(GHConstants.MaxTileSheets, (total_tiles_used - 1) / GHConstants.NumberOfTilesPerSheet + 1);

                lock (GHApp.Glyph2TileLock)
                {
                    GHApp.UsedTileSheets = total_sheets_used;
                    GHApp.TotalTiles = total_tiles_used;
                    for (int i = 0; i < total_sheets_used; i++)
                    {
                        GHApp.TilesPerRow[i] = GHApp._tileMap[i].Width / GHConstants.TileWidth;
                    }
                }
            }

            int[] gl2ti = null;
            byte[] gltifl = null;
            short[] ti2an = null;
            short[] ti2en = null;
            short[] ti2ad = null;
            int[] anoffs = null;
            int[] enoffs = null;
            int[] reoffs = null;
            int nosheets = 0;
            int notiles = 0;
            int[] tilesperrow = null;
            lock (GHApp.Glyph2TileLock)
            {
                ti2an = GHApp.Tile2Animation;
                ti2en = GHApp.Tile2Enlargement;
                ti2ad = GHApp.Tile2Autodraw;
                anoffs = GHApp.AnimationOffsets;
                enoffs = GHApp.EnlargementOffsets;
                reoffs = GHApp.ReplacementOffsets;
                nosheets = GHApp.UsedTileSheets;
                notiles = GHApp.TotalTiles;
                tilesperrow = GHApp.TilesPerRow;
            }
            RecordFunctionCall(RecordedFunctionID.InitializeWindows, gl2ti, gltifl, ti2an, ti2en, ti2ad, anoffs, enoffs, reoffs, nosheets, notiles, tilesperrow,
                animoff, enloff, reoff, general_tile_off, hit_tile_off, ui_tile_off, spell_tile_off, skill_tile_off, command_tile_off, buff_tile_off, cursor_off);
            RequestQueue.Enqueue(new GHRequest(this, GHRequestType.HideLoadingScreen));
        }

        public void StopWaitAndResumeSavedGame()
        {
            ResponseQueue.Enqueue(new GHResponse(this, GHRequestType.StopWaitAndRestoreSavedGame));
        }

        public void SaveGameAndWaitForResume()
        {
            ResponseQueue.Enqueue(new GHResponse(this, GHRequestType.SaveGameAndWaitForResume));
        }

        public void SaveCheckPoint()
        {
            ResponseQueue.Enqueue(new GHResponse(this, GHRequestType.SaveInsuranceCheckPoint));
        }

        private static int _saveDoneConfirmed = 0;
        public static bool SaveDoneConfirmed { get { return Interlocked.CompareExchange(ref _saveDoneConfirmed, 0, 0) != 0; } set { Interlocked.Exchange(ref _saveDoneConfirmed, value ? 1 : 0); } }

        /* This is called from the UI thread */
        public async Task SaveGameAndWaitForFinishedConfirmation()
        {
            if (PlayingReplay)
                return;

            SaveDoneConfirmed = false;
            ResponseQueue.Enqueue(new GHResponse(this, GHRequestType.SaveGameAndWaitForResume));
            /* This keeps background task and the app alive */
            for (int i = 0; i < GHConstants.SavePollingTimeoutCount; i++)
            {
                await Task.Delay(GHConstants.PollingInterval);
                if (SaveDoneConfirmed)
                    return;
            }
        }

        public void RequestForceExitThread()
        {
            ResponseQueue.Enqueue(new GHResponse(this, GHRequestType.ForceExitThread));
        }
        public void TerminateGnollHackAndRestartGameInNewGamePage()
        {
            ResponseQueue.Enqueue(new GHResponse(this, GHRequestType.TerminateGnollHackAndRestartGameInNewGamePage));
        }

        public int ClientCallback_CreateGHWindow(int wintype, int style, int glyph, byte dataflags, IntPtr objdata_ptr, IntPtr otypdata_ptr)
        {
            Obj objdata = objdata_ptr == IntPtr.Zero ? new Obj() : (Obj)Marshal.PtrToStructure(objdata_ptr, typeof(Obj));
            ObjClassData otypdata = otypdata_ptr == IntPtr.Zero ? new ObjClassData() : (ObjClassData)Marshal.PtrToStructure(otypdata_ptr, typeof(ObjClassData));

            RecordFunctionCall(RecordedFunctionID.CreateWindow, wintype, style, glyph, dataflags, objdata, otypdata);

            if (_lastWindowHandle >= GHConstants.MaxGHWindows) /* Should not happen, but paranoid */
                _lastWindowHandle = GHConstants.MaxGHWindows - 1;

            while (_lastWindowHandle > 0 && _ghWindows[_lastWindowHandle] == null)
                _lastWindowHandle--;

            if (_ghWindows[_lastWindowHandle] != null)
                _lastWindowHandle++;

            if (_lastWindowHandle >= GHConstants.MaxGHWindows)
                return 0;

            int handle = _lastWindowHandle;
            GHWindow ghwin = new GHWindow((GHWinType)wintype, (ghwindow_styles)style, glyph, 
                (dataflags & 8) != 0, 
                (dataflags & 16) != 0,
                (dataflags & 32) != 0,
                (dataflags & 1) == 0 ? null : new ObjectDataItem(objdata, otypdata, (dataflags & 4) != 0), 
                handle);

            _ghWindows[handle] = ghwin;
            UIUtils.SetCreateGHWindow(ghwin);
            if (wintype == (int)GHWinType.Map)
                _mapWindowId = handle;
            else if (wintype == (int)GHWinType.Message)
                _messageWindowId = handle;
            else if (wintype == (int)GHWinType.Status)
                _statusWindowId = handle;

            RequestQueue.Enqueue(new GHRequest(this, GHRequestType.UpdateGHWindow, handle, ghwin.Clone()));
            return handle;
        }

        public void ClientCallback_DestroyGHWindow(int winHandle)
        {
            RecordFunctionCall(RecordedFunctionID.DestroyWindow, winHandle);

            if (winHandle < 0)
                return;

            //lock (_ghWindowsLock)
            {
                GHWindow ghwin = _ghWindows[winHandle];
                if (ghwin != null)
                {
                    if (ghwin.WindowType == GHWinType.Map)
                        _mapWindowId = 0;
                    else if (ghwin.WindowType == GHWinType.Message)
                        _messageWindowId = 0;
                    else if (ghwin.WindowType == GHWinType.Status)
                        _statusWindowId = 0;
                    DestroyGHWindow(ghwin);
                }
                _ghWindows[winHandle] = null;
            }
        }

        private void DestroyGHWindow(GHWindow ghwin)
        {
            if (ghwin == null)
                return;
            ghwin.Visible = false;
            RequestQueue.Enqueue(new GHRequest(this, GHRequestType.DestroyWindowView, ghwin.WindowID));
            if (ghwin.WindowType == GHWinType.Menu && ghwin.MenuInfo != null && ghwin.MenuInfo.MenuCloseUponDestroy)
            {
                RequestQueue.Enqueue(new GHRequest(this, GHRequestType.HideMenuPage, ghwin.WindowID));
            }
        }

        public void ClientCallback_ClearGHWindow(int winHandle)
        {
            RecordFunctionCall(RecordedFunctionID.ClearWindow, winHandle);

            if (winHandle < 0)
                return;
            ClearGHWindow(_ghWindows[winHandle]);
        }

        private void ClearGHWindow(GHWindow win)
        {
            if (win == null)
                return;

            switch (win.WindowType)
            {
                case GHWinType.Map:
                    ClearMap();
                    break;
            }

            win.PutStrs.Clear();
            win.SetWidthHeight(0, 0, 0, 0);
            win.CursX = 0;
            win.CursY = 0;

            RequestQueue.Enqueue(new GHRequest(this, GHRequestType.UpdateGHWindow, win.WindowID, win.Clone()));
        }

        public void ClearMap()
        {
            lock (_mapDataBufferLock)
            {
                for (int x = 1; x < GHConstants.MapCols; x++)
                {
                    for (int y = 0; y < GHConstants.MapRows; y++)
                    {
                        _mapDataMaster[x, y].Glyph = GHApp.UnexploredGlyph;
                        _mapDataMaster[x, y].BkGlyph = GHApp.NoGlyph;
                        _mapDataMaster[x, y].Symbol = "";
                        _mapDataMaster[x, y].Color = SKColors.Black;// default(MapData);
                        _mapDataMaster[x, y].Special = 0;
                        _mapDataMaster[x, y].NeedsUpdate = true;
                        _mapDataMaster[x, y].GlyphPrintAnimationCounterValue = 0;
                        _mapDataMaster[x, y].GlyphPrintMainCounterValue = 0;
                        _mapDataMaster[x, y].GlyphObjectPrintAnimationCounterValue = 0;
                        _mapDataMaster[x, y].GlyphObjectPrintMainCounterValue = 0;
                        _mapDataMaster[x, y].GlyphGeneralPrintMainCounterValue = 0;

                        _mapDataMaster[x, y].Layers = new LayerInfo();
                        _mapDataMaster[x, y].Layers.layer_glyphs = new int[(int)layer_types.MAX_LAYERS];
                        _mapDataMaster[x, y].Layers.layer_gui_glyphs = new int[(int)layer_types.MAX_LAYERS];
                        _mapDataMaster[x, y].Layers.leash_mon_x = new sbyte[GHConstants.MaxLeashed + 1];
                        _mapDataMaster[x, y].Layers.leash_mon_y = new sbyte[GHConstants.MaxLeashed + 1];

                        _mapDataMaster[x, y].Layers.layer_glyphs[0] = GHApp.UnexploredGlyph;
                        _mapDataMaster[x, y].Layers.layer_gui_glyphs[0] = GHApp.UnexploredGlyph;
                        for (int i = 1; i < (int)layer_types.MAX_LAYERS; i++)
                        {
                            _mapDataMaster[x, y].Layers.layer_glyphs[i] = GHApp.NoGlyph;
                            _mapDataMaster[x, y].Layers.layer_gui_glyphs[i] = GHApp.NoGlyph;
                        }

                        _mapDataMaster[x, y].Layers.glyph = GHApp.UnexploredGlyph;
                        _mapDataMaster[x, y].Layers.bkglyph = GHApp.NoGlyph;
                        ClearAllObjectDataUnlocked(x, y);
                        ClearEngravingDataUnlocked(x, y);

                        _mapDataCurrent[x, y] = _mapDataMaster[x, y];
                    }
                }
                _mapDataCurrentUpdated = true;
            }
        }

        public void CheckUpdateCurrentMapBufferUnlocked()
        {
            if (!_mapDataCurrentUpdated)
            {
                for (int x = 1; x < GHConstants.MapCols; x++)
                {
                    for (int y = 0; y < GHConstants.MapRows; y++)
                    {
                        _mapDataCurrent[x, y] = _mapDataMaster[x, y];
                        if (_objectDataMaster[x, y] != null)
                        {
                            if (_objectDataMaster[x, y].FloorObjectList != null)
                            {
                                if (_objectDataCurrent[x, y].FloorObjectList == null)
                                    _objectDataCurrent[x, y].FloorObjectList = new List<ObjectDataItem>(16);
                                _objectDataCurrent[x, y].FloorObjectList.Clear();
                                _objectDataCurrent[x, y].FloorObjectList.AddRange(_objectDataMaster[x, y].FloorObjectList);
                            }
                            else
                                _objectDataCurrent[x, y].FloorObjectList = null;

                            if (_objectDataMaster[x, y].CoverFloorObjectList != null)
                            {
                                if (_objectDataCurrent[x, y].CoverFloorObjectList == null)
                                    _objectDataCurrent[x, y].CoverFloorObjectList = new List<ObjectDataItem>(4);
                                _objectDataCurrent[x, y].CoverFloorObjectList.Clear();
                                _objectDataCurrent[x, y].CoverFloorObjectList.AddRange(_objectDataMaster[x, y].CoverFloorObjectList);
                            }
                            else
                                _objectDataCurrent[x, y].CoverFloorObjectList = null;

                            if (_objectDataMaster[x, y].MemoryObjectList != null)
                            {
                                if (_objectDataCurrent[x, y].MemoryObjectList == null)
                                    _objectDataCurrent[x, y].MemoryObjectList = new List<ObjectDataItem>(16);
                                _objectDataCurrent[x, y].MemoryObjectList.Clear();
                                _objectDataCurrent[x, y].MemoryObjectList.AddRange(_objectDataMaster[x, y].MemoryObjectList);
                            }
                            else
                                _objectDataCurrent[x, y].MemoryObjectList = null;

                            if (_objectDataMaster[x, y].CoverMemoryObjectList != null)
                            {
                                if (_objectDataCurrent[x, y].CoverMemoryObjectList == null)
                                    _objectDataCurrent[x, y].CoverMemoryObjectList = new List<ObjectDataItem>(4);
                                _objectDataCurrent[x, y].CoverMemoryObjectList.Clear();
                                _objectDataCurrent[x, y].CoverMemoryObjectList.AddRange(_objectDataMaster[x, y].CoverMemoryObjectList);
                            }
                            else
                                _objectDataCurrent[x, y].CoverMemoryObjectList = null;
                        }
                        else
                            _objectDataCurrent[x, y] = null;
                    }
                }
                _mapDataCurrentUpdated = true;
            }
        }

        public void ClientCallback_DisplayGHWindow(int winHandle, byte blocking)
        {
            RecordFunctionCall(RecordedFunctionID.DisplayWindow, winHandle, blocking);

            if (winHandle < 0)
                return;
            bool ismenu = false;
            bool istext = false;
            bool ismap = false;
            //lock (_ghWindowsLock)
            {
                if (_ghWindows[winHandle] != null)
                {
                    if(!PlayingReplay || !GHApp.IsReplaySearching)
                    {
                        DisplayGHWindow(_ghWindows[winHandle]);
                    }
                    else
                    {
                        _ghWindows[winHandle].Visible = true;
                        RequestQueue.Enqueue(new GHRequest(this, GHRequestType.UpdateGHWindowVisibility, winHandle, true));
                    }
                    ismenu = (_ghWindows[winHandle].WindowType == GHWinType.Menu);
                    istext = (_ghWindows[winHandle].WindowType == GHWinType.Text);
                    ismap = (_ghWindows[winHandle].WindowType == GHWinType.Map);
                    if (ismenu || istext)
                        GHApp.AddSentryBreadcrumb("DisplayGHWindow: Displaying " + _ghWindows[winHandle].WindowType.ToString() + " Window", GHConstants.SentryGnollHackCallbackCategoryName);
                }
            }

            if((blocking != 0 && ismap) || ismenu || istext)
            {
                if((PlayingReplay && !GHApp.IsReplaySearching) || _fastForwardGameOver)
                {
                    if(!_fastForwardGameOver)
                        WaitAndCheckPauseReplay(GHConstants.ReplayDisplayWindowDelay);
                    RequestQueue.Enqueue(new GHRequest(this, GHRequestType.HideTextWindow));
                }
                else
                {
                    int res = ClientCallback_nhgetch();
                }
            }
        }

        private void DisplayGHWindow(GHWindow win)
        {
            if (win == null)
                return;

            win.Visible = true;
            RequestQueue.Enqueue(new GHRequest(this, GHRequestType.UpdateGHWindowVisibility, win.WindowID, true));
            if (win.WindowType == GHWinType.Menu || win.WindowType == GHWinType.Text)
            {
                RequestQueue.Enqueue(new GHRequest(this, GHRequestType.DisplayWindowView, win.WindowID));
            }
        }

        public void ClientCallback_ExitWindows(string str)
        {
            Debug.WriteLine("ClientCallback_ExitWindows");
            RecordFunctionCall(RecordedFunctionID.ExitWindows, str);

            ClientCallback_RawPrint(str);

            if (!string.IsNullOrWhiteSpace(str) && !GHApp.IsReplaySearching)
                Thread.Sleep(GHConstants.ExitWindowsWithStringDelay);

            //lock (_ghWindowsLock)
            {
                for(int winHandle = 0; winHandle <= _lastWindowHandle && winHandle < GHConstants.MaxGHWindows; winHandle++)
                {
                    GHWindow ghwin = _ghWindows[winHandle];
                    if (ghwin == null)
                        continue;
                    if (ghwin.WindowType == GHWinType.Map)
                        _mapWindowId = 0;
                    else if (ghwin.WindowType == GHWinType.Message)
                        _messageWindowId = 0;
                    else if (ghwin.WindowType == GHWinType.Status)
                        _statusWindowId = 0;

                    //ghwin.Destroy();
                    DestroyGHWindow(ghwin);
                    _ghWindows[winHandle] = null;
                }
                _lastWindowHandle = -1;
            }
        }

        public int ClientCallback_PlayerSelection()
        {
            RecordFunctionCall(RecordedFunctionID.PlayerSelection);

            Debug.WriteLine("ClientCallback_PlayerSelection");
            return 0;
        }


        private MapData[,] _mapDataMaster = new MapData[GHConstants.MapCols, GHConstants.MapRows];
        private MapData[,] _mapDataCurrent;
        private bool _mapDataCurrentUpdated = true;
        private bool _mapDataCurrentIs2 = false;
        private MapData[,] _mapDataBuffer1 = new MapData[GHConstants.MapCols, GHConstants.MapRows];
        private MapData[,] _mapDataBuffer2 = new MapData[GHConstants.MapCols, GHConstants.MapRows];
        private readonly object _mapDataBufferLock = new object();
        private int _mapCursorX;
        private int _mapCursorY;
        private game_cursor_types _cursorType;
        private bool _force_paint_at_cursor;
        private bool _show_cursor_on_u;

        private int _ux = 0;
        private int _uy = 0;
        private ulong _u_condition_bits = 0;
        private ulong _u_status_bits = 0;
        private ulong[] _u_buff_bits = new ulong[GHConstants.NUM_BUFF_BIT_ULONGS];
        private ObjectData[,] _objectDataMaster = new ObjectData[GHConstants.MapCols, GHConstants.MapRows];
        private ObjectData[,] _objectDataCurrent;
        private ObjectData[,] _objectDataBuffer1 = new ObjectData[GHConstants.MapCols, GHConstants.MapRows];
        private ObjectData[,] _objectDataBuffer2 = new ObjectData[GHConstants.MapCols, GHConstants.MapRows];
        private ObjectDataItem _uChain = null;
        private ObjectDataItem _uBall = null;


        public void ClientCallback_Curs(int winHandle, int x, int y)
        {
            RecordFunctionCall(RecordedFunctionID.Curs, winHandle, x, y);

            if (winHandle < 0 || winHandle >= GHConstants.MaxGHWindows)
                return;

            GHWindow gHWindow = _ghWindows[winHandle];
            if (gHWindow != null)
            {
                gHWindow.CursX = x;
                gHWindow.CursY = y;
                if (gHWindow.WindowType == GHWinType.Map)
                {
                    lock (_mapDataBufferLock)
                    {
                        _mapCursorX = x;
                        _mapCursorY = y;
                    }
                }
            }
        }

        public void ClientCallback_PrintGlyph(int winHandle, int x, int y, int glyph, int bkglyph, int symbol, int ocolor, uint special, IntPtr layers_ptr)
        {
            LayerInfo layers = layers_ptr == IntPtr.Zero ? new LayerInfo() : (LayerInfo)Marshal.PtrToStructure(layers_ptr, typeof(LayerInfo));
            RecordFunctionCall(RecordedFunctionID.PrintGlyph, winHandle, x, y, glyph, bkglyph, symbol, ocolor, special, layers);

            SetMapSymbol(x, y, glyph, bkglyph, symbol, ocolor, special, ref layers);
        }

        public bool GetMapDataBuffer(out MapData[,] mapBuffer, out ObjectData[,] objectBuffer, out ObjectDataItem uBall, out ObjectDataItem uChain, out int ux, out int uy, out ulong u_condition_bits, out ulong u_status_bits, ref ulong[] u_buff_bits,
            out int cursx, out int cursy, out game_cursor_types cursorType, out bool force_paint_at_cursor, out bool show_cursor_on_u)
        {
            bool lockTaken = false;
            //lock(_mapDataBufferLock)
            try
            {
                Monitor.TryEnter(_mapDataBufferLock, ref lockTaken); //TimeSpan.FromTicks(GHConstants.MapDataLockTimeOutTicks), 
                if (lockTaken)
                {
                    if (_mapDataCurrentUpdated)
                    {
                        mapBuffer = _mapDataCurrent;
                        objectBuffer = _objectDataCurrent;
                        uBall = _uBall;
                        uChain = _uChain;
                        ux = _ux;
                        uy = _uy;
                        u_condition_bits = _u_condition_bits;
                        u_status_bits = _u_status_bits;
                        for (int i = 0; i < GHConstants.NUM_BUFF_BIT_ULONGS; i++)
                            u_buff_bits[i] = _u_buff_bits[i];
                        cursx = _mapCursorX;
                        cursy = _mapCursorY;
                        cursorType = _cursorType;
                        force_paint_at_cursor = _force_paint_at_cursor;
                        show_cursor_on_u = _show_cursor_on_u;

                        _mapDataCurrentIs2 = !_mapDataCurrentIs2;
                        _mapDataCurrent = _mapDataCurrentIs2 ? _mapDataBuffer2 : _mapDataBuffer1;
                        _objectDataCurrent = _mapDataCurrentIs2 ? _objectDataBuffer2 : _objectDataBuffer1;
                        _mapDataCurrentUpdated = false;
                        return true;
                    }
                    else
                    {
                        mapBuffer = null;
                        objectBuffer = null;
                        uBall = null;
                        uChain = null;
                        ux = 0;
                        uy = 0;
                        u_condition_bits = 0;
                        u_status_bits = 0;
                        cursx = 0;
                        cursy = 0;
                        cursorType = game_cursor_types.CURSOR_STYLE_GENERIC_CURSOR;
                        force_paint_at_cursor = false;
                        show_cursor_on_u = false;
                        return false;
                    }
                }
                else
                {
                    mapBuffer = null;
                    objectBuffer = null;
                    uBall = null;
                    uChain = null;
                    ux = 0;
                    uy = 0;
                    u_condition_bits = 0;
                    u_status_bits = 0;
                    cursx = 0;
                    cursy = 0;
                    cursorType = game_cursor_types.CURSOR_STYLE_GENERIC_CURSOR;
                    force_paint_at_cursor = false;
                    show_cursor_on_u = false;
                    return false;
                }
            }
            finally
            {
                if (lockTaken) 
                    Monitor.Exit(_mapDataBufferLock);
            }
        }

        private void SetMapSymbol(int x, int y, int glyph, int bkglyph, int c, int color, uint special, ref LayerInfo layers)
        {
            long generalCounter;
            long mainCounter;
            generalCounter = Interlocked.CompareExchange(ref AnimationTimers.general_animation_counter, 0L, 0L);
            mainCounter = MainCounterValue;
            lock (_mapDataBufferLock)
            {
                CheckUpdateCurrentMapBufferUnlocked();
                SetMapSymbolOnTimerUnlocked(x, y, glyph, bkglyph, c, color, special, ref layers, generalCounter, mainCounter);
                ClearAllObjectDataUnlocked(x, y);
                ClearEngravingDataUnlocked(x, y);
            }
        }

        private void SetMapSymbolOnTimerUnlocked(int x, int y, int glyph, int bkglyph, int c, int color, uint special, ref LayerInfo layers, long generalCounter, long mainCounter)
        {
            if (((layers.layer_flags & (ulong)LayerFlags.LFLAGS_UXUY) != 0 && (_mapDataMaster[x, y].Layers.layer_flags & (ulong)LayerFlags.LFLAGS_UXUY) == 0) ||
                (layers.m_id != 0 && layers.m_id != _mapDataMaster[x, y].Layers.m_id))
            {
                /* Update counter value only if the monster just moved here, not, e.g. if it changes action in the same square,
                 * or is printed in the same square again with the same origin coordinates. This way, the movement action is played only once. 
                 */
                _mapDataMaster[x, y].GlyphPrintAnimationCounterValue = generalCounter;
                _mapDataMaster[x, y].GlyphPrintMainCounterValue = mainCounter;
            }
            if ((layers.layer_flags & (ulong)LayerFlags.LFLAGS_UXUY) != 0)
            {
                _ux = x;
                _uy = y;
                _u_condition_bits = layers.condition_bits;
                _u_status_bits = layers.status_bits;
                if (layers.buff_bits != null)
                {
                    for (int i = 0; i < GHConstants.NUM_BUFF_BIT_ULONGS; i++)
                    {
                        _u_buff_bits[i] = layers.buff_bits[i];
                    }
                }
            }
            if (layers.o_id != 0 && layers.o_id != _mapDataMaster[x, y].Layers.o_id)
            {
                /* Update counter value only if the object just moved here, not, e.g. if it changes action in the same square,
                 * or is printed in the same square again with the same origin coordinates. This way, the movement action is played only once. 
                 */
                _mapDataMaster[x, y].GlyphObjectPrintAnimationCounterValue = generalCounter;
                _mapDataMaster[x, y].GlyphObjectPrintMainCounterValue = mainCounter;
            }

            /* General counter that gets always set */
            _mapDataMaster[x, y].GlyphGeneralPrintAnimationCounterValue = generalCounter;
            _mapDataMaster[x, y].GlyphGeneralPrintMainCounterValue = mainCounter;
            _mapDataMaster[x, y].Glyph = glyph;
            _mapDataMaster[x, y].BkGlyph = bkglyph;
            _mapDataMaster[x, y].Symbol = Char.ConvertFromUtf32(c);
            _mapDataMaster[x, y].Color = UIUtils.NHColor2SKColor(color, (special & 0x00002000UL) != 0 ? (int)MenuItemAttributes.AltColors : 0);
            _mapDataMaster[x, y].Special = special;
            _mapDataMaster[x, y].Layers = layers;

            _mapDataMaster[x, y].NeedsUpdate = true;
            _mapDataMaster[x, y].HasEnlargementOrAnimationOrSpecialHeight = false;

            /* copy to current */
            _mapDataCurrent[x, y] = _mapDataMaster[x, y];
        }

        private void ClearAllObjectDataUnlocked(int x, int y)
        {
            if (_objectDataMaster[x, y] != null)
            {
                if (_objectDataMaster[x, y].FloorObjectList != null)
                    _objectDataMaster[x, y].FloorObjectList.Clear();
                if (_objectDataMaster[x, y].CoverFloorObjectList != null)
                    _objectDataMaster[x, y].CoverFloorObjectList.Clear();
                if (_objectDataMaster[x, y].MemoryObjectList != null)
                    _objectDataMaster[x, y].MemoryObjectList.Clear();
                if (_objectDataMaster[x, y].CoverMemoryObjectList != null)
                    _objectDataMaster[x, y].CoverMemoryObjectList.Clear();
            }
            if (_objectDataCurrent[x, y] != null)
            {
                if (_objectDataCurrent[x, y].FloorObjectList != null)
                    _objectDataCurrent[x, y].FloorObjectList.Clear();
                if (_objectDataCurrent[x, y].CoverFloorObjectList != null)
                    _objectDataCurrent[x, y].CoverFloorObjectList.Clear();
                if (_objectDataCurrent[x, y].MemoryObjectList != null)
                    _objectDataCurrent[x, y].MemoryObjectList.Clear();
                if (_objectDataCurrent[x, y].CoverMemoryObjectList != null)
                    _objectDataCurrent[x, y].CoverMemoryObjectList.Clear();
            }
        }

        private void ClearEngravingDataUnlocked(int x, int y)
        {
            if (GHUtils.isok(x, y))
            {
                _mapDataCurrent[x, y].Engraving = _mapDataMaster[x, y].Engraving = new EngravingInfo();
            }
        }

        public int Replay_AskName(string modeName, string modeDescription, string enteredPlayerName)
        {
            RequestQueue.Enqueue(new GHRequest(this, GHRequestType.AskName, modeName, modeDescription, enteredPlayerName));
            WaitAndCheckPauseReplay(GHConstants.ReplayAskNameDelay2);
            RequestQueue.Enqueue(new GHRequest(this, GHRequestType.HideAskNamePage));
            return 0;
        }

        public int ClientCallback_AskName(string modeName, string modeDescription, IntPtr out_string_ptr)
        {
            Debug.WriteLine("ClientCallback_AskName");

            _characternameSet = false;
            CharacterName = "";
            RequestQueue.Enqueue(new GHRequest(this, GHRequestType.AskName, modeName, modeDescription));
            while (!_characternameSet)
            {
                Thread.Sleep(GHConstants.PollingInterval);
                PollResponseQueue();
            }

            if (out_string_ptr != IntPtr.Zero && CharacterName != "")
            {
                byte[] utf8text = Encoding.UTF8.GetBytes(CharacterName);
                Marshal.Copy(utf8text, 0, out_string_ptr, utf8text.Length);
                Marshal.WriteByte(out_string_ptr, utf8text.Length, 0);
                RecordFunctionCall(RecordedFunctionID.AskName, modeName, modeDescription, CharacterName);
                return 1;
            }
            else
            {
                RecordFunctionCall(RecordedFunctionID.AskName, modeName, modeDescription, "");
                return 0;
            }
        }

        public void ClientCallback_get_nh_event()
        {
            Debug.WriteLine("ClientCallback_get_nh_event");
            RecordFunctionCall(RecordedFunctionID.GetEvent);
        }

        public void ClientCallback_ExitHack(int status)
        {
            RecordFunctionCall(RecordedFunctionID.ExitHack, status);
            RecordFunctionCallImmediately(RecordedFunctionID.EndOfFile);
            GHApp.MaybeWriteGHLog("ExitHack: " + status, true, GHConstants.SentryGnollHackCallbackCategoryName);

            switch (status)
            {
                case (int)exit_hack_types.EXITHACK_EXITTHREAD: /* Just forcing the exit of the GnollHack thread before exiting the app; do nothing */
                    break;
                case (int)exit_hack_types.EXITHACK_RECOVER_NEW: /* OS destroyed the activity on Android, or an equivalent situation occurred */
                    RequestQueue.Enqueue(new GHRequest(this, GHRequestType.RestartGameAfterPageDestruction));
                    ActiveGamePage?.DoPolling(); /* A new game page will not have polling timer on */
                    break;
                case (int)exit_hack_types.EXITHACK_RESTART_EXISTING: /* Restart in the same game page (after saving) */
                    RequestQueue.Enqueue(new GHRequest(this, GHRequestType.RestartGame));
                    ActiveGamePage?.DoPolling(); /* A new game page will not have polling timer on */
                    break;
                default:
                case (int)exit_hack_types.EXITHACK_NORMAL: /* Normal case; return to main menu */
                    GHApp.FmodService?.StopAllGameSounds((uint)StopSoundFlags.All, 0);
                    GHApp.FmodService?.ResetGameState();
                    GHApp.SaveDiscoveredMusic();
                    RequestQueue.Enqueue(new GHRequest(this, GHRequestType.ReturnToMainMenu));
                    break;
            }
        }

        public int ClientCallback_nhgetch()
        {
            Debug.WriteLine("ClientCallback_nhgetch");
            WriteFunctionCallsAndCheckEnd();

            if (PlayingReplay)
            {
                if (!GHApp.IsReplaySearching)
                {
                    Thread.Sleep((int)(GHConstants.ReplayStandardDelay / GHApp.ReplaySpeed));
                    GHApp.FmodService?.PollTasks();
                }
                return 0;
            }
            if (_fastForwardGameOver)
            {
                RecordFunctionCall(RecordedFunctionID.GetChar, 0);
                return 0;
            }

            RequestQueue.Enqueue(new GHRequest(this, GHRequestType.GetChar));
            while (_inputBufferLocation < 0)
            {
                Thread.Sleep(GHConstants.PollingInterval);
                PollResponseQueue();
                if (_fastForwardGameOver)
                {
                    RecordFunctionCall(RecordedFunctionID.GetChar, 0);
                    return 0;
                }
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
            RecordFunctionCall(RecordedFunctionID.GetChar, res);
            return res;
        }

        public int ClientCallback_nh_poskey(out int x, out int y, out int mod)
        {
            Debug.WriteLine("ClientCallback_nh_poskey");
            WriteFunctionCallsAndCheckEnd();

            x = 0;
            y = 0;
            mod = 0;

            if (PlayingReplay)
            {
                if (!GHApp.IsReplaySearching)
                {
                    Thread.Sleep((int)(GHConstants.ReplayStandardDelay / GHApp.ReplaySpeed));
                    GHApp.FmodService?.PollTasks();
                }
                return 0;
            }
            if (_fastForwardGameOver)
            {
                RecordFunctionCall(RecordedFunctionID.PosKey, x, y, mod, 0);
                return 0;
            }

            RequestQueue.Enqueue(new GHRequest(this, GHRequestType.PosKey));
            while (_inputBufferLocation < 0)
            {
                if (_touchLocSet)
                {
                    _touchLocSet = false;
                    x = _touchLocX;
                    y = _touchLocY;
                    mod = _touchLocMod;
                    RecordFunctionCall(RecordedFunctionID.PosKey, x, y, mod, 0);
                    return 0;
                }
                Thread.Sleep(GHConstants.PollingInterval);
                PollResponseQueue();
                if (_fastForwardGameOver)
                {
                    RecordFunctionCall(RecordedFunctionID.PosKey, x, y, mod, 0);
                    return 0;
                }
            }
            int res = GHConstants.CancelChar;
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
            RecordFunctionCall(RecordedFunctionID.PosKey, x, y, mod, res);
            return res;
        }

        public int ClientCallback_YnFunction(int style, int attr, int color, int glyph, string title, string question, string responses, string def, string descriptions, string introline, ulong ynflags)
        {
            if (question != null)
                RawPrintEx(question, attr, color, false);

            WriteFunctionCallsAndCheckEnd();

            if (_fastForwardGameOver)
            {
                RecordFunctionCall(RecordedFunctionID.YnFunction, style, attr, color, glyph, title, question, responses, def, descriptions, introline, ynflags, GHConstants.CancelChar);
                return GHConstants.CancelChar;
            }

            if (string.IsNullOrEmpty(responses))
            {
                GHApp.AddSentryBreadcrumb("YnFunction: ShowDirections", GHConstants.SentryGnollHackCallbackCategoryName);
                if ((ynflags & 1UL) != 0)
                    RequestQueue.Enqueue(new GHRequest(this, GHRequestType.ShowDirections)); //TODO: Show keyboard
                else
                    RequestQueue.Enqueue(new GHRequest(this, GHRequestType.ShowDirections));
                int res = ClientCallback_nhgetch(); /* Get direction / letter */
                if ((ynflags & 1UL) != 0)
                    RequestQueue.Enqueue(new GHRequest(this, GHRequestType.HideDirections)); //TODO: Hide keyboard
                else
                    RequestQueue.Enqueue(new GHRequest(this, GHRequestType.HideDirections));
                RecordFunctionCall(RecordedFunctionID.YnFunction, style, attr, color, glyph, title, question, responses, def, descriptions, introline, ynflags, res);
                return res;
            }
            else
            {
                GHApp.AddSentryBreadcrumb("YnFunction: " + (title != null ? title : "no title") + ", " + (question != null ? question : "no question") + ", " + (responses != null ? responses : "no responses") + ", " + (descriptions != null ? descriptions : "no descriptions"), GHConstants.SentryGnollHackCallbackCategoryName);
                RequestQueue.Enqueue(new GHRequest(this, GHRequestType.ShowYnResponses, style, attr, color, glyph, title, question, responses, descriptions, introline, ynflags));

                if (PlayingReplay)
                {
                    WaitAndCheckPauseReplay(GHConstants.ReplayYnDelay);
                }
                else
                {
                    int cnt = 0;
                    while (cnt < 5)
                    {
                        int val = ClientCallback_nhgetch();
                        if (!string.IsNullOrEmpty(def) && (val == 0 || val == GHConstants.CancelChar))
                            val = (int)def[0];
                        string desc = "";
                        if (val < 0)
                        {
                            desc = "Numpad direction " + Math.Abs(val);
                            val = GHConstants.CancelChar;
                        }

                        string res = "n";
                        try
                        {
                            res = Char.ConvertFromUtf32(val);
                        }
                        catch(Exception ex)
                        {
                            Debug.WriteLine(ex.Message);
                        }
                        if (desc == "")
                            desc = res;

                        if (responses.Contains(res))
                        {
                            GHApp.AddSentryBreadcrumb("YnFunction: Result is " + val, GHConstants.SentryGnollHackCallbackCategoryName);
                            RecordFunctionCall(RecordedFunctionID.YnFunction, style, attr, color, glyph, title, question, responses, def, descriptions, introline, ynflags, val);
                            return val;
                        }

                        RequestQueue.Enqueue(new GHRequest(this, GHRequestType.ShowYnResponses, style, attr, color, glyph, title, question, responses, descriptions, introline, ynflags));
                        ClientCallback_RawPrint("'" + desc + "': Invalid input!");
                        cnt++;
                    }
                }
            }
            RequestQueue.Enqueue(new GHRequest(this, GHRequestType.HideYnResponses));
            RecordFunctionCall(RecordedFunctionID.YnFunction, style, attr, color, glyph, title, question, responses, def, descriptions, introline, ynflags, GHConstants.CancelChar);
            return GHConstants.CancelChar;
        }

        public void ClientCallback_Cliparound(int x, int y, byte force)
        {
            RecordFunctionCall(RecordedFunctionID.ClipAround, x, y, force);
            GamePage gamePage = ActiveGamePage;
            if (gamePage == null)
                return;

            if (force == 0 && (gamePage.MapNoClipMode || gamePage.MapLookMode || gamePage.ZoomMiniMode)) //|| (!_gamePage.ZoomAlternateMode && _gamePage.MapNoClipMode) || (_gamePage.ZoomAlternateMode && _gamePage.MapAlternateNoClipMode) 
                return; /* No clip mode ignores cliparound commands */

            //This may be slightly slower to register, but likely more often in UI thread, so should cause fewer lock conflicts
            //RequestQueue.Enqueue(new GHRequest(this, GHRequestType.ClipAround, x, y, force == 1));

            gamePage.SetTargetClip(x, y, force == 1, MainCounterValue);
        }

        public void ClientCallback_RawPrint(string str)
        {
            RecordFunctionCall(RecordedFunctionID.RawPrint, str);
            RawPrintEx(str, (int)MenuItemAttributes.None, (int)NhColor.NO_COLOR, false);
        }

        public void ClientCallback_RawPrintBold(string str)
        {
            RecordFunctionCall(RecordedFunctionID.RawPrintBold, str);
            RawPrintEx(str, (int)MenuItemAttributes.Bold, (int)NhColor.NO_COLOR, false);
        }

        public void UpdateMessageHistory()
        {
            if (_useHideMessageHistory || _longer_message_history.Count == 0)
                RequestQueue.Enqueue(new GHRequest(this, GHRequestType.PrintHistory)); /* Clear history */
            else
                RequestQueue.Enqueue(new GHRequest(this, GHRequestType.PrintHistory, _useLongerMessageHistory ? _longer_message_history.CurrentSpan : _longer_message_history.GetSpanFromEnd(GHConstants.MaxMessageHistoryLength)));
        }

        public void AddNewMessage(GHMsgHistoryItem newmsg, bool isRestoring) //, bool updateMessageHistory, bool isFullRefresh, bool updateOnlyLongerHistory)
        {
            if (newmsg == null)
                return;

            if (!_longer_message_history.TryAdd(newmsg))
            {
                _longer_message_history = new GHMsgHistoryList(_longer_message_history);
                _longer_message_history.TryAdd(newmsg);
            }

            if(!isRestoring)
            {
                UpdateMessageHistory();
                SwitchOffLongerAndHideMessageHistory(); /* Just to make sure that it does not remain on the slow down the game */
            }
        }

        public void SwitchOffLongerAndHideMessageHistory()
        {
            if (_useLongerMessageHistory && _longer_message_history.Count > GHConstants.MaxMessageHistoryLength)
            {
                RequestQueue.Enqueue(new GHRequest(this, GHRequestType.UseLongerMessageHistory, false));
            }
            if (_useHideMessageHistory)
            {
                /* _useHideMessageHistory will be turned off by the response to this request */
                RequestQueue.Enqueue(new GHRequest(this, GHRequestType.UseHideMessageHistory, false));
            }
        }

        public void RawPrintEx(string str, int attr, int color, bool is_restoring)
        {
            GHMsgHistoryItem newmsg = new GHMsgHistoryItem(str, attr, color);
            AddNewMessage(newmsg, is_restoring);
        }

        public void RawPrintEx2(string str, byte[] attrs, byte[] colors, int attr, int color, bool is_restoring)
        {
            GHMsgHistoryItem newmsg = new GHMsgHistoryItem(str, attrs, colors, attr, color);
            AddNewMessage(newmsg, is_restoring);
        }

        public void ClientCallback_PutStrEx(int win_id, string str, int attributes, int color, int append)
        {
            RecordFunctionCall(RecordedFunctionID.PutStrEx, win_id, str, attributes, color, append);
            if (win_id < 0 || win_id >= GHConstants.MaxGHWindows || _ghWindows[win_id] == null || str == null)
                return;

            if (_ghWindows[win_id].WindowPrintStyle == GHWindowPrintLocations.RawPrint)
            {
                RawPrintEx(str, attributes, color, false);
            }
            else
            {
                _ghWindows[win_id].PutStrEx(attributes, str, append, color);
            }
        }

        public void ClientCallback_PutStrEx2(int win_id, string str, IntPtr attributes_ptr, IntPtr colors_ptr, int attr, int color, int append)
        {
            if (win_id < 0 || win_id >= GHConstants.MaxGHWindows || _ghWindows[win_id] == null || str == null)
                return;

            int str_length = str.Length;

            byte[] attributes = new byte[str_length + 1];
            for (int i = 0; i < str_length; i++)
                attributes[i] = (int)MenuItemAttributes.None;
            attributes[str_length] = 0;

            if (attributes_ptr != IntPtr.Zero)
            {
                Marshal.Copy(attributes_ptr, attributes, 0, str_length + 1);
            }

            byte[] colors = new byte[str_length + 1];
            for (int i = 0; i < str_length; i++)
                colors[i] = (int)NhColor.NO_COLOR;
            colors[str_length] = 0;

            if (colors_ptr != IntPtr.Zero)
            {
                Marshal.Copy(colors_ptr, colors, 0, str_length + 1);
            }

            RecordFunctionCall(RecordedFunctionID.PutStrEx2, win_id, str, attributes, colors, attr, color, append);
            if (_ghWindows[win_id].WindowPrintStyle == GHWindowPrintLocations.RawPrint)
            {
                RawPrintEx2(str, attributes, colors, attr, color, false);
            }
            else
            {
                _ghWindows[win_id].PutStrEx2(str, attributes_ptr != IntPtr.Zero ? attributes : null, colors_ptr != IntPtr.Zero ? colors : null, attr, color, append);
            }
        }

        public void ClientCallback_DelayOutput()
        {
            RecordFunctionCall(RecordedFunctionID.DelayOutput);
            if (ClientCallback_UIHasInput() > 0)
                return;
            if (!PlayingReplay || !GHApp.IsReplaySearching)
            {
                Thread.Sleep(GHConstants.DelayOutputDurationInMilliseconds / 2);
                GHApp.FmodService?.PollTasks();
                Thread.Sleep(GHConstants.DelayOutputDurationInMilliseconds / 2);
                GHApp.FmodService?.PollTasks();
            }
        }

        public void ClientCallback_DelayOutputMilliseconds(int milliseconds)
        {
            RecordFunctionCall(RecordedFunctionID.DelayOutputMilliseconds, milliseconds);
            if (ClientCallback_UIHasInput() > 0)
                return;
            if (!PlayingReplay || !GHApp.IsReplaySearching)
            {
                Thread.Sleep(milliseconds);
                GHApp.FmodService?.PollTasks();
            }
        }
        public void ClientCallback_DelayOutputIntervals(int intervals)
        {
            RecordFunctionCall(RecordedFunctionID.DelayOutputIntervals, intervals);
            if (PlayingReplay && GHApp.IsReplaySearching)
                return;

            long start_counter_value = 0L;
            long current_counter_value = 0L;
            //lock (_gamePage.AnimationTimerLock)
            {
                start_counter_value = Interlocked.CompareExchange(ref AnimationTimers.general_animation_counter, 0L, 0L);;
            }

            do
            {
                if (ClientCallback_UIHasInput() > 0)
                    break;

                Thread.Sleep(5);
                //lock (_gamePage.AnimationTimerLock)
                {
                    current_counter_value = Interlocked.CompareExchange(ref AnimationTimers.general_animation_counter, 0L, 0L);;
                }
            } while (current_counter_value < start_counter_value + (long)intervals);
            GHApp.FmodService?.PollTasks();
        }

        public void ClientCallback_PreferenceUpdate(string str)
        {
            RecordFunctionCall(RecordedFunctionID.PreferenceUpdate, str);

            if (str == "statuslines")
            {
                ClearGHWindow(_ghWindows[_statusWindowId]);
            }
        }

        public readonly object StatusFieldLock = new object();
        public readonly GHStatusField[] StatusFields = new GHStatusField[(int)NhStatusFields.MAXBLSTATS];

        public void ClientCallback_StatusInit(int reassessment)
        {
            RecordFunctionCall(RecordedFunctionID.StatusInit, reassessment);

            if (reassessment != 0)
                return;
            lock(StatusFieldLock)
            {
                for (int i = 0; i < (int)NhStatusFields.MAXBLSTATS; i++)
                {
                    StatusFields[i] = new GHStatusField();
                }
            }
        }

        public void ClientCallback_StatusFinish()
        {
            RecordFunctionCall(RecordedFunctionID.StatusFinish);
        }

        public void ClientCallback_StatusEnable(int fieldidx, string nm, string fmt, byte enable)
        {
            RecordFunctionCall(RecordedFunctionID.StatusEnable, fieldidx, nm, fmt, enable);
            if (fieldidx >= 0 && fieldidx < (int)NhStatusFields.MAXBLSTATS)
            {
                lock (StatusFieldLock)
                {
                    StatusFields[fieldidx].Name = nm;
                    StatusFields[fieldidx].Format = fmt;
                    StatusFields[fieldidx].IsEnabled = enable != 0;
                }
            }
        }

        static public string[] cond_names_short = new string[(int)bl_conditions.NUM_BL_CONDITIONS] {
            "Stone", "Slime", "Strngl", "Suffoc", "FoodPois", "TermIll", "Blind",
            "Deaf", "Stun", "Conf", "Hallu", "Lev", "Fly", "Ride", "Slow", "Paral", 
            "Fear", "Sleep", "Cancl", "Silent", "Grab", "Rot", "Lyca", "Legs"
        };

        static public string[] cond_names_long = new string[(int)bl_conditions.NUM_BL_CONDITIONS] {
            "Petrifying", "Slimed", "Being Strangled", "Suffocating", "Food Poisoned", "Terminally Ill", "Blind",
            "Deaf", "Stunned", "Confused", "Hallucinating", "Levitating", "Flying", "Riding", "Slowed", "Paralyzed", 
            "Frightened", "Sleeping", "Cancelled", "Silenced", "Grabbed", "Mummy Rot", "Lycanthropy", "Wounded Legs"
        };

        public void ClientCallback_StatusUpdate(int fieldidx, string text, long condbits, int cng, int percent, int color, IntPtr condcolorptr)
        {
            if(fieldidx != (int)NhStatusFields.BL_CONDITION)
                RecordFunctionCall(RecordedFunctionID.StatusUpdate, fieldidx, text, condbits, cng, percent, color, null);

            long oldbits = 0L;
            if (fieldidx >= 0 && fieldidx < (int)NhStatusFields.MAXBLSTATS)
            {
                lock (StatusFieldLock)
                {
                    oldbits = StatusFields[fieldidx].Bits;

                    StatusFields[fieldidx].Text = text;
                    StatusFields[fieldidx].Bits = condbits;
                    StatusFields[fieldidx].Change = cng;
                    StatusFields[fieldidx].Percent = percent;
                    StatusFields[fieldidx].Color = color;
                }
            }

            switch(fieldidx)
            {
                case (int)NhStatusFields.BL_TIME:
                    if (GHApp.RecordGame && _knownFirstTurn == -1 && !string.IsNullOrWhiteSpace(text) && int.TryParse(text.Trim(), out int turn))
                    {
                        _knownFirstTurn = turn;
                    }
                    break;
                case (int)NhStatusFields.BL_SKILL:
                    {
                        GHRequestType rtype;
                        if (text != null && text == "Skill")
                            rtype = GHRequestType.ShowSkillButton;
                        else
                            rtype = GHRequestType.HideSkillButton;

                        RequestQueue.Enqueue(new GHRequest(this, rtype));
                        break;
                    }
                case (int)NhStatusFields.BL_CAP:
                case (int)NhStatusFields.BL_HUNGER:
                    {
                        if (cng != 0 && text != null && text != "")
                        {
                            DisplayConditionTextData data = new DisplayConditionTextData();
                            data.text = text.Trim();
                            data.style = 0;
                            data.color = (int)(color & 15);
                            data.tflags = 0UL;
                            RequestQueue.Enqueue(new GHRequest(this, GHRequestType.DisplayConditionText, data));
                        }
                        break;
                    }
                case (int)NhStatusFields.BL_CONDITION:
                    {
                        if (cng != 0 && condbits != 0)
                        {
                            int arraysize = (int)bl_conditions.NUM_BL_CONDITIONS; // (int)NhColor.CLR_MAX + 5;
                            short[] condcolors = new short[arraysize];
                            bool condcolorset = false;
                            if (condcolorptr != IntPtr.Zero)
                            {
                                Marshal.Copy(condcolorptr, condcolors, 0, arraysize);
                                condcolorset = true;
                            }
                            else
                            {
                                for (int i = 0; i < arraysize; i++)
                                {
                                    condcolors[i] = 0;
                                }
                            }

                            RecordFunctionCall(RecordedFunctionID.StatusUpdate, fieldidx, text, condbits, cng, percent, color, condcolors);
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
                                        //for(int c = 0; c < (int)NhColor.CLR_MAX; c++)
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
                                    data.text = GHGame.cond_names_long[i];
                                    data.style = 0;
                                    data.attr = 0;
                                    data.color = condcolor;
                                    data.tflags = 0UL;
                                    RequestQueue.Enqueue(new GHRequest(this, GHRequestType.DisplayConditionText, data));

                                    int filtercolor = (int)NhColor.CLR_MAX;
                                    switch (i)
                                    {
                                        case (int)bl_conditions.BL_COND_STONE:
                                            filtercolor = (int)NhColor.CLR_BROWN;
                                            break;
                                        case (int)bl_conditions.BL_COND_SLIME:
                                            filtercolor = (int)NhColor.CLR_BRIGHT_GREEN;
                                            break;
                                        case (int)bl_conditions.BL_COND_STRNGL:
                                            filtercolor = (int)NhColor.CLR_RED;
                                            break;
                                        case (int)bl_conditions.BL_COND_SUFFOC:
                                            filtercolor = (int)NhColor.CLR_RED;
                                            break;
                                        case (int)bl_conditions.BL_COND_FOODPOIS:
                                            filtercolor = (int)NhColor.CLR_MAGENTA;
                                            break;
                                        case (int)bl_conditions.BL_COND_TERMILL:
                                            filtercolor = (int)NhColor.CLR_MAGENTA;
                                            break;
                                    }

                                    if (filtercolor != (int)NhColor.CLR_MAX)
                                    {
                                        DisplayScreenFilterData fdata = new DisplayScreenFilterData();
                                        fdata.style = 0;
                                        fdata.attr = 0;
                                        fdata.color = filtercolor;
                                        fdata.fflags = 0UL;
                                        RequestQueue.Enqueue(new GHRequest(this, GHRequestType.DisplayScreenFilter, fdata));
                                        break;
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
        public int ClientCallback_GetMsgHistory(IntPtr text_ptr, IntPtr attributes_ptr, IntPtr colors_ptr, byte init)
        {
            if (init != 0)
                _msgIndex = 0;

            int res = 0;
            if (_msgIndex < _longer_message_history.Count)
            {
                string text = _longer_message_history[_msgIndex].Text;
                int msgLength = text.Length;
                if (text_ptr != IntPtr.Zero)
                {
                    byte[] utf8text = Encoding.UTF8.GetBytes(text);
                    Marshal.Copy(utf8text, 0, text_ptr, utf8text.Length);
                    Marshal.WriteByte(text_ptr, utf8text.Length, 0);
                }
                if (attributes_ptr != IntPtr.Zero)
                {
                    if(_longer_message_history[_msgIndex].Attributes != null)
                    {
                        Marshal.Copy(_longer_message_history[_msgIndex].Attributes, 0, attributes_ptr, msgLength);
                    }
                    else
                    {
                        for (int i = 0; i < msgLength; i++)
                            Marshal.WriteByte(attributes_ptr, i, (byte)_longer_message_history[_msgIndex].Attribute);
                    }
                    Marshal.WriteByte(attributes_ptr, msgLength, 0);
                }
                if (colors_ptr != IntPtr.Zero)
                {
                    if (_longer_message_history[_msgIndex].Colors != null)
                    {
                        Marshal.Copy(_longer_message_history[_msgIndex].Colors, 0, colors_ptr, msgLength);
                    }
                    else
                    {
                        for(int i = 0; i < msgLength; i++)
                            Marshal.WriteByte(colors_ptr, i, (byte)_longer_message_history[_msgIndex].NHColor);
                    }
                    Marshal.WriteByte(colors_ptr, msgLength, 0);
                }

                _msgIndex++;
                if (_msgIndex < 0)
                    _msgIndex = 0;

                res = 1;
            }

            return res;
        }

        public void ClientCallback_PutMsgHistory(string msg, IntPtr attributes_ptr, IntPtr colors_ptr, byte is_restoring)
        {
            if (msg != null)
            {
                int str_length = msg.Length;
                byte[] attributes = new byte[str_length + 1];
                for (int i = 0; i < str_length; i++)
                    attributes[i] = (int)MenuItemAttributes.None;
                attributes[str_length] = 0;

                if (attributes_ptr != IntPtr.Zero)
                {
                    Marshal.Copy(attributes_ptr, attributes, 0, str_length + 1);
                }

                byte[] colors = new byte[str_length + 1];
                for (int i = 0; i < str_length; i++)
                    colors[i] = (int)NhColor.NO_COLOR;
                colors[str_length] = 0;

                if (colors_ptr != IntPtr.Zero)
                {
                    Marshal.Copy(colors_ptr, colors, 0, str_length + 1);
                }

                RawPrintEx2(msg, attributes, colors, (int)MenuItemAttributes.None, (int)NhColor.NO_COLOR, is_restoring != 0);
            }
            else if (is_restoring != 0)
            {
                UpdateMessageHistory();
            }
        }

        public void ClientCallback_StartMenu(int winid, int style)
        {
            GHApp.DebugWriteProfilingStopwatchTimeAndStart("StartMenu");
            RecordFunctionCall(RecordedFunctionID.StartMenu, winid, style);

            //lock (_ghWindowsLock)
            {
                if (_ghWindows[winid] != null)
                {
                    _ghWindows[winid].MenuInfo = new GHMenuInfo((ghmenu_styles)style);
                }
            }
        }
        public void ClientCallback_AddMenu(int winid, int glyph, long identifier, char accel, char groupaccel, int attr, int color, string text, byte presel)
        {
            ClientCallback_AddExtendedMenu(winid, glyph, identifier, accel, groupaccel, attr, color, text, presel,
                0, 0UL, 0UL, '\0', '\0', 0UL, 0, 0, IntPtr.Zero, IntPtr.Zero, IntPtr.Zero, IntPtr.Zero);
        }
        public void ClientCallback_AddExtendedMenu(int winid, int glyph, long identifier, char accel, char groupaccel, int attr, int color, string text, byte presel, 
            int maxcount, ulong oid, ulong mid, char headingaccel, char special_mark, ulong menuflags, byte dataflags, int style, 
            IntPtr otmpdata_ptr, IntPtr otypdata_ptr, IntPtr attrs_ptr, IntPtr colors_ptr)
        {
            GHApp.DebugWriteProfilingStopwatchTimeAndStart("AddExtendedMenu");
            Obj otmpdata = otmpdata_ptr == IntPtr.Zero ? new Obj() : (Obj)Marshal.PtrToStructure(otmpdata_ptr, typeof(Obj));
            ObjClassData otypdata = otypdata_ptr == IntPtr.Zero ? new ObjClassData() : (ObjClassData)Marshal.PtrToStructure(otypdata_ptr, typeof(ObjClassData));
            byte[] attrs = null;
            byte[] colors = null;
            int attrs_colors_size = text?.Length + 1 ?? 0;
            if (attrs_ptr != IntPtr.Zero && colors_ptr != IntPtr.Zero && attrs_colors_size > 0)
            {
                attrs = new byte[attrs_colors_size];
                colors = new byte[attrs_colors_size];
                Marshal.Copy(attrs_ptr, attrs, 0, attrs_colors_size);
                Marshal.Copy(colors_ptr, colors, 0, attrs_colors_size);
            }

            RecordFunctionCall(RecordedFunctionID.AddExtendedMenu, winid, glyph, identifier, accel, groupaccel, attr, color, text, presel,
                maxcount, oid, mid, headingaccel, special_mark, menuflags, dataflags, style, otmpdata, otypdata, attrs, colors);

            //lock (_ghWindowsLock)
            {
                if (_ghWindows[winid] != null && _ghWindows[winid].MenuInfo != null)
                {
                    GHMenuItem mi = new GHMenuItem(_ghWindows[winid].MenuInfo, GHApp.NoGlyph, ActiveGamePage, identifier,
                        accel == 0 && identifier != 0 ? _ghWindows[winid].MenuInfo.AutoAccelerator : accel, groupaccel, special_mark,
                        attr, color, attrs, colors);

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
                    mi.UseNumItems = (menuflags & (ulong)MenuFlags.MENU_FLAGS_USE_NUM_ITEMS) != 0;
                    mi.NumItems = mi.UseNumItems ? maxcount : 0;
                    mi.IsButtonStyle = (menuflags & (ulong)MenuFlags.MENU_FLAGS_BUTTON_STYLE) != 0;
                    mi.IsAutoClickOk = (menuflags & (ulong)MenuFlags.MENU_FLAGS_AUTO_CLICK_OK) != 0;
                    mi.Selected = (presel != 0);
                    mi.MaxCount = maxcount;
                    mi.Oid = oid;
                    mi.Mid = mid;
                    mi.HeadingGroupAccelerator = headingaccel;
                    mi.Flags = menuflags;
                    _ghWindows[winid].MenuInfo.MenuItems.Add(mi);
                }
            }
        }

        public void ClientCallback_EndMenu(int winid, string prompt, string subtitle)
        {
            GHApp.DebugWriteProfilingStopwatchTimeAndStart("EndMenu");
            RecordFunctionCall(RecordedFunctionID.EndMenu, winid, prompt, subtitle);
            GHApp.AddSentryBreadcrumb("EndMenu: " + (prompt != null ? prompt : "no prompt") + ", " + (subtitle != null ? subtitle : "no subtitle"), GHConstants.SentryGnollHackCallbackCategoryName);

            //lock (_ghWindowsLock)
            {
                if (_ghWindows[winid] != null && _ghWindows[winid].MenuInfo != null)
                {
                    _ghWindows[winid].MenuInfo.Header = prompt;
                    _ghWindows[winid].MenuInfo.Subtitle = subtitle;
                }
            }
        }

        bool DoShowMenu(int winid, int how)
        {
            bool enqueued = false;
            if (_ghWindows[winid] != null && _ghWindows[winid].MenuInfo != null)
            {
                SelectionMode smode = (SelectionMode)how;
                _ghWindows[winid].MenuInfo.SelectionHow = smode;

                /* Clear menu response */
                _ghWindows[winid].SelectedMenuItems = null;
                _ghWindows[winid].WasCancelled = false;

                GHWindow clonedWindow = _ghWindows[winid].Clone();
                RequestQueue.Enqueue(new GHRequest(this, GHRequestType.UpdateGHWindow, winid, clonedWindow));
                RequestQueue.Enqueue(new GHRequest(this, GHRequestType.ShowMenuPage, clonedWindow, clonedWindow.MenuInfo));
                enqueued = true;
            }
            return enqueued;
        }

        public int Replay_SelectMenu(int winid, int how, int cnt)
        {
            bool enqueued = DoShowMenu(winid, how);
            if(enqueued)
            {
                WaitAndCheckPauseReplay(GHConstants.ReplayMenuDelay);
                RequestQueue.Enqueue(new GHRequest(this, GHRequestType.HideMenuPage));
            }
            return cnt;
        }

        public int ClientCallback_SelectMenu(int winid, int how, out IntPtr picklistptr, out int listsize)
        {
            GHApp.DebugWriteProfilingStopwatchTimeAndStart("SelectMenu");
            Debug.WriteLine("ClientCallback_SelectMenu");

            bool enqueued = DoShowMenu(winid, how);
            _abortShowMenuPage = false;

            if (enqueued)
            {
                bool continuepolling = true;
                while (continuepolling)
                {
                    //lock (_ghWindowsLock)
                    {
                        if (_ghWindows[winid] == null)
                            continuepolling = false;
                        else
                            continuepolling = (_ghWindows[winid].SelectedMenuItems == null);
                    }
                    if (!continuepolling || _fastForwardGameOver || _abortShowMenuPage)
                        break;

                    Thread.Sleep(GHConstants.PollingInterval);
                    PollResponseQueue();
                }
            }

            /* Handle result */
            int cnt = 0;
            long[] picklist = null;

            IntPtr arrayptr;

            //lock (_ghWindowsLock)
            {
                if (_abortShowMenuPage || _ghWindows[winid] == null || _ghWindows[winid].SelectedMenuItems == null || _ghWindows[winid].WasCancelled || _fastForwardGameOver)
                    cnt = -1;
                else if (_ghWindows[winid].SelectedMenuItems.Count <= 0)
                    cnt = 0;
                else
                {
                    cnt = _ghWindows[winid].SelectedMenuItems.Count;
                    picklist = new long[cnt * 2];
                    for(int i = 0; i < cnt; i++)
                    {
                        picklist[2 * i] = _ghWindows[winid].SelectedMenuItems[i].Identifier;
                        picklist[2 * i + 1] = (long)_ghWindows[winid].SelectedMenuItems[i].Count;
                    }
                }
                long i64var = 0;
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

            _abortShowMenuPage = false;
            RecordFunctionCall(RecordedFunctionID.SelectMenu, winid, how, picklist, listsize, cnt);
            return cnt;
        }

        private IntPtr _outGoingIntPtr;

        public void ClientCallback_FreeMemory(ref IntPtr ptr)
        {
            //RecordFunctionCall(RecordedFunctionID.FreeMemory);

            IntPtr inComingIntPtr = ptr;
            bool issame = (inComingIntPtr == _outGoingIntPtr);
            if (!issame)
                Debug.WriteLine("Not same");

            Marshal.FreeHGlobal(issame ? ptr : _outGoingIntPtr);
        }

        public void ClientCallback_ReportPlayerName(string used_player_name)
        {
            RecordFunctionCall(RecordedFunctionID.ReportPlayerName, used_player_name);
            _knownPlayerName = used_player_name;
            if (used_player_name != null && used_player_name != "")
            {
                if(GHApp.TournamentMode && !PlayingReplay)
                    GHApp.SaveLastUsedTournamentPlayerName(used_player_name);
                else
                    GHApp.SaveLastUsedPlayerName(used_player_name);
            }
            GHApp.AddSentryBreadcrumb("Used player name reported: " + used_player_name, GHConstants.SentryGnollHackGeneralCategoryName);
        }

        private long _gamePlayTime = 0L;
        public long GamePlayTime { get { return Interlocked.CompareExchange(ref _gamePlayTime, 0, 0); } set { Interlocked.Exchange(ref _gamePlayTime, value); } }

        private long _sessionPlayTime = 0L;
        public long SessionPlayTime { get { return Interlocked.CompareExchange(ref _sessionPlayTime, 0, 0); } set { Interlocked.Exchange(ref _sessionPlayTime, value); } }
        public void AddSessionPlayTime(long addition)
        {
            if (addition < 0) /* Something's wrong */
                return;

            if (Interlocked.Add(ref _sessionPlayTime, addition) < 0)
                Interlocked.Exchange(ref _sessionPlayTime, 0);
        }

        public void ClientCallback_ReportPlayTime(long timePassed, long currentPlayTime)
        {
            RecordFunctionCall(RecordedFunctionID.ReportPlayTime, timePassed, currentPlayTime);
            if (PlayingReplay)
                return;

            long playedalready = GHApp.RealPlayTime;
            long totaltime = playedalready + timePassed;
            GHApp.SaveRealPlayTime(totaltime);
            GamePlayTime = currentPlayTime;
            AddSessionPlayTime(timePassed);
            GHApp.AddAggragateSessionPlayTime(timePassed);
        }

        public void ClientCallback_SendObjectData(int x, int y, IntPtr otmp_ptr, int cmdtype, int where, IntPtr otypdata_ptr, ulong oflags)
        {
            Obj otmp = otmp_ptr == IntPtr.Zero ? new Obj() : (Obj)Marshal.PtrToStructure(otmp_ptr, typeof(Obj));
            ObjClassData otypdata = otypdata_ptr == IntPtr.Zero ? new ObjClassData() : (ObjClassData)Marshal.PtrToStructure(otypdata_ptr, typeof(ObjClassData));

            RecordFunctionCall(RecordedFunctionID.SendObjectData, x, y, otmp, cmdtype, where, otypdata, oflags);
            AddObjectData(x, y, otmp, cmdtype, where, otypdata, oflags);
        }

        public void AddObjectData(int x, int y, Obj otmp, int cmdtype, int where, ObjClassData otypdata, ulong oflags)
        {
            bool is_uwep = (oflags & (ulong)objdata_flags.OBJDATA_FLAGS_UWEP) != 0UL;
            bool is_uwep2 = (oflags & (ulong)objdata_flags.OBJDATA_FLAGS_UWEP2) != 0UL;
            bool is_uquiver = (oflags & (ulong)objdata_flags.OBJDATA_FLAGS_UQUIVER) != 0UL;
            bool is_equipped = is_uwep | is_uwep2 | is_uquiver;
            bool hallucinated = (oflags & (ulong)objdata_flags.OBJDATA_FLAGS_HALLUCINATION) != 0UL;
            bool foundthisturn = (oflags & (ulong)objdata_flags.OBJDATA_FLAGS_FOUND_THIS_TURN) != 0UL;
            bool isuchain = (oflags & (ulong)objdata_flags.OBJDATA_FLAGS_UCHAIN) != 0UL;
            bool isuball = (oflags & (ulong)objdata_flags.OBJDATA_FLAGS_UBALL) != 0UL;

            if (is_equipped)
            {
                AddEquippedObjectData(x, y, otmp, cmdtype, where, otypdata, oflags);
            }
            else
            {
                lock (_mapDataBufferLock)
                {
                    CheckUpdateCurrentMapBufferUnlocked();
                    if (_objectDataMaster[x, y] != null && _objectDataCurrent[x, y] != null)
                    {
                        bool is_memoryobj = (where == (int)obj_where_types.OBJ_MEMORY);
                        bool is_drawn_in_front = (oflags & (ulong)objdata_flags.OBJDATA_FLAGS_DRAWN_IN_FRONT) != 0UL;
                        List<ObjectDataItem> masterobjectList = is_memoryobj ? (is_drawn_in_front ? _objectDataMaster[x, y].CoverMemoryObjectList : _objectDataMaster[x, y].MemoryObjectList) : (is_drawn_in_front ? _objectDataMaster[x, y].CoverFloorObjectList : _objectDataMaster[x, y].FloorObjectList);
                        List<ObjectDataItem> currentobjectList = is_memoryobj ? (is_drawn_in_front ? _objectDataCurrent[x, y].CoverMemoryObjectList : _objectDataCurrent[x, y].MemoryObjectList) : (is_drawn_in_front ? _objectDataCurrent[x, y].CoverFloorObjectList : _objectDataCurrent[x, y].FloorObjectList);
                        ObjectDataItem newItem;
                        switch (cmdtype)
                        {
                            case 1: /* Clear */
                                if (masterobjectList != null)
                                    masterobjectList.Clear();
                                if (currentobjectList != null)
                                    currentobjectList.Clear();
                                break;
                            case 2: /* Add item */
                                if (masterobjectList == null)
                                {
                                    if (is_memoryobj)
                                    {
                                        if (is_drawn_in_front)
                                        {
                                            _objectDataMaster[x, y].CoverMemoryObjectList = new List<ObjectDataItem>(4);
                                        }
                                        else
                                        {
                                            _objectDataMaster[x, y].MemoryObjectList = new List<ObjectDataItem>(16);
                                        }
                                    }
                                    else
                                    {
                                        if (is_drawn_in_front)
                                        {
                                            _objectDataMaster[x, y].CoverFloorObjectList = new List<ObjectDataItem>(4);
                                        }
                                        else
                                        {
                                            _objectDataMaster[x, y].FloorObjectList = new List<ObjectDataItem>(16);
                                        }
                                    }
                                    masterobjectList = is_memoryobj ? (is_drawn_in_front ? _objectDataMaster[x, y].CoverMemoryObjectList : _objectDataMaster[x, y].MemoryObjectList) : (is_drawn_in_front ? _objectDataMaster[x, y].CoverFloorObjectList : _objectDataMaster[x, y].FloorObjectList);
                                }
                                if (currentobjectList == null)
                                {
                                    if (is_memoryobj)
                                    {
                                        if (is_drawn_in_front)
                                        {
                                            _objectDataCurrent[x, y].CoverMemoryObjectList = new List<ObjectDataItem>(4);
                                        }
                                        else
                                        {
                                            _objectDataCurrent[x, y].MemoryObjectList = new List<ObjectDataItem>(16);
                                        }
                                    }
                                    else
                                    {
                                        if (is_drawn_in_front)
                                        {
                                            _objectDataCurrent[x, y].CoverFloorObjectList = new List<ObjectDataItem>(4);
                                        }
                                        else
                                        {
                                            _objectDataCurrent[x, y].FloorObjectList = new List<ObjectDataItem>(16);
                                        }
                                    }
                                    currentobjectList = is_memoryobj ? (is_drawn_in_front ? _objectDataCurrent[x, y].CoverMemoryObjectList : _objectDataCurrent[x, y].MemoryObjectList) : (is_drawn_in_front ? _objectDataCurrent[x, y].CoverFloorObjectList : _objectDataCurrent[x, y].FloorObjectList);
                                }
                                masterobjectList.Add(new ObjectDataItem(otmp, otypdata, hallucinated, foundthisturn));
                                currentobjectList.Add(new ObjectDataItem(otmp, otypdata, hallucinated, foundthisturn));
                                break;
                            case 3: /* Add container item to previous item */
                                if (masterobjectList == null || masterobjectList.Count == 0)
                                    break;
                                //if (masterobjectList[masterobjectList.Count - 1].ContainedObjs == null)
                                //    masterobjectList[masterobjectList.Count - 1].ContainedObjs = new List<ObjectDataItem>(16);
                                ObjectDataItem item = new ObjectDataItem(otmp, otypdata, hallucinated);
                                //masterobjectList[masterobjectList.Count - 1].ContainedObjs.Add(item);
                                masterobjectList[masterobjectList.Count - 1] = masterobjectList[masterobjectList.Count - 1].CloneWithAddedContainedObj(item);

                                if (currentobjectList == null || currentobjectList.Count == 0)
                                    break;
                                //if (currentobjectList[currentobjectList.Count - 1].ContainedObjs == null)
                                //    currentobjectList[currentobjectList.Count - 1].ContainedObjs = new List<ObjectDataItem>(16);
                                currentobjectList[currentobjectList.Count - 1] = currentobjectList[currentobjectList.Count - 1].CloneWithAddedContainedObj(item);
                                break;
                            case 4: /* Clear uchain and uball */
                                _uChain = null;
                                _uBall = null;
                                break;
                            case 5: /* Add uchain or uball */
                                if (!is_memoryobj && (isuchain || isuball))
                                {
                                    newItem = new ObjectDataItem(otmp, otypdata, hallucinated, foundthisturn);
                                    if (isuchain)
                                        _uChain = newItem;
                                    if (isuball)
                                        _uBall = newItem;
                                }
                                break;
                        }
                    }
                }
            }
        }

        public ObjectDataItem[] _weaponStyleObjDataItem = new ObjectDataItem[3];
        public readonly object _weaponStyleObjDataItemLock = new object();

        public void AddEquippedObjectData(int x, int y, Obj otmp, int cmdtype, int where, ObjClassData otypdata, ulong oflags)
        {
            bool is_uwep = (oflags & (ulong)objdata_flags.OBJDATA_FLAGS_UWEP) != 0UL;
            bool is_uwep2 = (oflags & (ulong)objdata_flags.OBJDATA_FLAGS_UWEP2) != 0UL;
            bool is_uquiver = (oflags & (ulong)objdata_flags.OBJDATA_FLAGS_UQUIVER) != 0UL;
            bool hallucinated = (oflags & (ulong)objdata_flags.OBJDATA_FLAGS_HALLUCINATION) != 0UL;
            bool foundthisturn = (oflags & (ulong)objdata_flags.OBJDATA_FLAGS_FOUND_THIS_TURN) != 0UL;

            bool outofammo1 = (oflags & (ulong)objdata_flags.OBJDATA_FLAGS_OUT_OF_AMMO1) != 0UL;
            bool wrongammo1 = (oflags & (ulong)objdata_flags.OBJDATA_FLAGS_WRONG_AMMO_TYPE1) != 0UL;
            bool notbeingused1 = (oflags & (ulong)objdata_flags.OBJDATA_FLAGS_NOT_BEING_USED1) != 0UL;
            bool notweapon1 = (oflags & (ulong)objdata_flags.OBJDATA_FLAGS_NOT_WEAPON1) != 0UL;
            bool outofammo2 = (oflags & (ulong)objdata_flags.OBJDATA_FLAGS_OUT_OF_AMMO2) != 0UL;
            bool wrongammo2 = (oflags & (ulong)objdata_flags.OBJDATA_FLAGS_WRONG_AMMO_TYPE2) != 0UL;
            bool notbeingused2 = (oflags & (ulong)objdata_flags.OBJDATA_FLAGS_NOT_BEING_USED2) != 0UL;
            bool notweapon2 = (oflags & (ulong)objdata_flags.OBJDATA_FLAGS_NOT_WEAPON2) != 0UL;
            bool outofammo = is_uwep ? outofammo1 : is_uwep2 ? outofammo2 : false;
            bool wrongammo = is_uwep ? wrongammo1 : is_uwep2 ? wrongammo2 : false;
            bool notbeingused = is_uwep ? notbeingused1 : is_uwep2 ? notbeingused2 : false;
            bool notweapon = is_uwep ? notweapon1 : is_uwep2 ? notweapon2 : false;
            bool isammo = (oflags & (ulong)objdata_flags.OBJDATA_FLAGS_IS_AMMO) != 0UL;
            bool isthrowingweapon = (oflags & (ulong)objdata_flags.OBJDATA_FLAGS_THROWING_WEAPON) != 0UL;
            bool prevwepfound = (oflags & (ulong)objdata_flags.OBJDATA_FLAGS_PREV_WEP_FOUND) != 0UL;
            bool prevunwield = (oflags & (ulong)objdata_flags.OBJDATA_FLAGS_PREV_UNWIELD) != 0UL;

            int idx = is_uwep ? 0 : is_uwep2 ? 1 : 2;
            lock (_weaponStyleObjDataItemLock)
            {
                switch (cmdtype)
                {
                    case 1: /* Clear */
                        _weaponStyleObjDataItem[idx] = null;
                        break;
                    case 2: /* Add item */
                        _weaponStyleObjDataItem[idx] = new ObjectDataItem(otmp, otypdata, hallucinated, outofammo, wrongammo, notbeingused, notweapon, foundthisturn, isammo, isthrowingweapon, prevwepfound, prevunwield);
                        break;
                    case 3: /* Add container item to previous item */
                        _weaponStyleObjDataItem[idx] = _weaponStyleObjDataItem[idx].CloneWithAddedContainedObj(new ObjectDataItem(otmp, otypdata, hallucinated));
                        break;
                }
            }
        }

        //private List<SavedSendMonsterDataCall> _savedSendMonsterDataCalls = new List<SavedSendMonsterDataCall>();
        public void ClientCallback_SendMonsterData(int cmdtype, int x, int y, IntPtr monster_data_ptr, ulong oflags)
        {
            monst_info monster_data = monster_data_ptr == IntPtr.Zero ? new monst_info() : (monst_info)Marshal.PtrToStructure(monster_data_ptr, typeof(monst_info));
            RecordFunctionCall(RecordedFunctionID.SendMonsterData, cmdtype, x, y, monster_data, oflags);
            //_savedSendMonsterDataCalls.Add(new SavedSendMonsterDataCall(cmdtype, x, y, ref monster_data, oflags));
            switch (cmdtype)
            {
                case 0: /* Add Pet */
                    //_gamePage.AddPetData(monster_data);
                    //This may cause fewer lock conflicts since AddPetData is likely to be in the UI thread, but is a bit slower
                    AddPetData(monster_data);
                    //RequestQueue.Enqueue(new GHRequest(this, GHRequestType.AddPetData, monster_data));
                    break;
            }
        }

        public readonly object _petDataLock = new object();
        public readonly List<GHPetDataItem> _petData = new List<GHPetDataItem>(8);


        public void ClearPetData()
        {
            lock (_petDataLock)
            {
                _petData.Clear();
            }
        }

        public void AddPetData(monst_info monster_data)
        {
            lock (_petDataLock)
            {
                _petData.Add(new GHPetDataItem(monster_data));
            }
        }

        public void ClientCallback_SendEngravingData(int cmdtype, int x, int y, string engraving_text, int etype, ulong eflags, ulong gflags)
        {
            RecordFunctionCall(RecordedFunctionID.SendEngravingData, cmdtype, x, y, engraving_text, etype, eflags, gflags);
            //_savedSendEngravingDataCalls.Add(new SavedSendEngravingDataCall(cmdtype, x, y, engraving_text, etype, eflags, gflags));
            switch (cmdtype)
            {
                case 0: /* Add engraving */
                    //_gamePage.AddEngravingData(x, y, engraving_text, etype, eflags, gflags);
                    AddEngravingData(x, y, engraving_text, etype, eflags, gflags);
                    break;
            }
        }

        public void AddEngravingData(int x, int y, string engraving_text, int etype, ulong eflags, ulong gflags)
        {
            lock (_mapDataBufferLock)
            {
                if (GHUtils.isok(x, y))
                {
                    CheckUpdateCurrentMapBufferUnlocked();
                    _mapDataCurrent[x, y].Engraving = _mapDataMaster[x, y].Engraving = new EngravingInfo(engraving_text, etype, eflags, gflags);
                }
            }
        }


        public readonly object _contextMenuDataLock = new object();
        public readonly List<ContextMenuButton> _contextMenuData = new List<ContextMenuButton>();

        public void ClearContextMenu()
        {
            //ContextLayout.Children.Clear();
            //ContextLayout.IsVisible = false;
            lock (_contextMenuDataLock)
            {
                _contextMenuData.Clear();
            }
        }
        public void AddContextMenu(AddContextMenuData data)
        {
            int cmddefchar = data.cmd_def_char;
            int cmdcurchar = data.cmd_cur_char;
            if (cmddefchar < 0)
                cmddefchar += 256; /* On this operating system, chars are signed chars; fix to positive values */
            if (cmdcurchar < 0)
                cmdcurchar += 256; /* On this operating system, chars are signed chars; fix to positive values */
            string icon_string = "";
            int LastPickedCmd = GHUtils.Meta('<');
            int OfferCmd = GHUtils.Meta('o');
            int PrayCmd = GHUtils.Meta('p');
            int DipCmd = GHUtils.Meta('d');
            int DigCmd = GHUtils.Ctrl('g');
            int SitCmd = GHUtils.Ctrl('s');
            int RideCmd = GHUtils.Meta('R');
            //int PrevWepCmd = GHUtils.Meta(16);
            //int PickNStashCmd = ';';
            //if (cmddefchar == PickNStashCmd && !ShowPut2BagContextCommand)
            //    return; /* Do not add */
            //if (cmddefchar == PrevWepCmd && !ShowPrevWepContextCommand)
            //    return; /* Do not add */

            switch ((char)cmddefchar)
            {
                case 'a':
                    switch (data.style)
                    {
                        case (int)context_menu_styles.CONTEXT_MENU_STYLE_CLOSE_DISPLAY: /* Next interesting / monster */
                        case (int)context_menu_styles.CONTEXT_MENU_STYLE_GETDIR: /* Next interesting / monster */
                        case (int)context_menu_styles.CONTEXT_MENU_STYLE_GETPOS: /* Next interesting / monster */
                            icon_string = GHApp.AppResourceName + ".Assets.UI.next.png";
                            break;
                        case (int)context_menu_styles.CONTEXT_MENU_STYLE_GENERAL: /* Apply */
                        default:
                            icon_string = GHApp.AppResourceName + ".Assets.UI.apply.png";
                            break;
                    }
                    break;
                case 'm':
                    switch (data.style)
                    {
                        case (int)context_menu_styles.CONTEXT_MENU_STYLE_GETPOS: /* Next interesting / monster */
                            icon_string = GHApp.AppResourceName + ".Assets.UI.next.png";
                            break;
                        default:
                            icon_string = GHApp.AppResourceName + ".Assets.UI.next.png";
                            break;
                    }
                    break;
                case 'A':
                case 'M':
                    switch (data.style)
                    {
                        case (int)context_menu_styles.CONTEXT_MENU_STYLE_GETPOS: /* Previous interesting / monster */
                            icon_string = GHApp.AppResourceName + ".Assets.UI.previous.png";
                            break;
                        default:
                            icon_string = GHApp.AppResourceName + ".Assets.UI.previous.png";
                            break;
                    }
                    break;
                case 'e':
                    icon_string = GHApp.AppResourceName + ".Assets.UI.eat.png";
                    break;
                case 'l':
                    icon_string = GHApp.AppResourceName + ".Assets.UI.loot.png";
                    break;
                case 'b':
                    icon_string = GHApp.AppResourceName + ".Assets.UI.lootout.png";
                    break;
                case 'B':
                    icon_string = GHApp.AppResourceName + ".Assets.UI.lootin.png";
                    break;
                case 'p':
                    icon_string = GHApp.AppResourceName + ".Assets.UI.pay.png";
                    break;
                case ',':
                    icon_string = GHApp.AppResourceName + ".Assets.UI.pickup.png";
                    break;
                case '<':
                    switch (data.style)
                    {
                        case (int)context_menu_styles.CONTEXT_MENU_STYLE_GETDIR: /* Upwards */
                            icon_string = GHApp.AppResourceName + ".Assets.UI.target-upwards.png";
                            break;
                        case (int)context_menu_styles.CONTEXT_MENU_STYLE_GETPOS:
                            icon_string = GHApp.AppResourceName + ".Assets.UI.stairs-up.png";
                            break;
                        default:
                        case (int)context_menu_styles.CONTEXT_MENU_STYLE_GENERAL:
                            if (data.target_text != null && data.target_text == "Pit")
                                icon_string = GHApp.AppResourceName + ".Assets.UI.arrow_up.png";
                            else
                                icon_string = GHApp.AppResourceName + ".Assets.UI.stairs-up.png";
                            break;
                    }
                    break;
                case '>':
                    switch (data.style)
                    {
                        case (int)context_menu_styles.CONTEXT_MENU_STYLE_GETDIR: /* Downwards */
                            icon_string = GHApp.AppResourceName + ".Assets.UI.target-downwards.png";
                            break;
                        case (int)context_menu_styles.CONTEXT_MENU_STYLE_GETPOS:
                            icon_string = GHApp.AppResourceName + ".Assets.UI.stairs-down.png";
                            break;
                        default:
                        case (int)context_menu_styles.CONTEXT_MENU_STYLE_GENERAL:
                            if (data.target_text != null && data.target_text == "Pit")
                                icon_string = GHApp.AppResourceName + ".Assets.UI.arrow_down.png";
                            else
                                icon_string = GHApp.AppResourceName + ".Assets.UI.stairs-down.png";
                            break;
                    }
                    break;
                case ':':
                    icon_string = GHApp.AppResourceName + ".Assets.UI.lookhere.png";
                    break;
                case 'q':
                    icon_string = GHApp.AppResourceName + ".Assets.UI.quaff.png";
                    break;
                case 'r':
                    icon_string = GHApp.AppResourceName + ".Assets.UI.read.png";
                    break;
                case '.':
                    switch (data.style)
                    {
                        case (int)context_menu_styles.CONTEXT_MENU_STYLE_GETPOS: /* Pick position in getpos */
                            icon_string = GHApp.AppResourceName + ".Assets.UI.select.png";
                            break;
                        case (int)context_menu_styles.CONTEXT_MENU_STYLE_GETDIR: /* Self in getdir */
                            icon_string = GHApp.AppResourceName + ".Assets.UI.target-self.png";
                            break;
                        default:
                        case (int)context_menu_styles.CONTEXT_MENU_STYLE_GENERAL:
                            icon_string = GHApp.AppResourceName + ".Assets.UI.wait.png";
                            break;
                    }
                    break;
                case (char)GHConstants.CancelChar:
                    switch (data.style)
                    {
                        case (int)context_menu_styles.CONTEXT_MENU_STYLE_CLOSE_DISPLAY:
                            icon_string = GHApp.AppResourceName + ".Assets.UI.exit-to-map.png";
                            break;
                        default:
                            icon_string = GHApp.AppResourceName + ".Assets.UI.no.png";
                            break;
                    }
                    break;
                case 'C':
                    if (data.cmd_text == "Steed")
                        icon_string = GHApp.AppResourceName + ".Assets.UI.chatsteed.png";
                    else
                        icon_string = GHApp.AppResourceName + ".Assets.UI.chat.png";
                    break;
                case ';':
                    icon_string = GHApp.AppResourceName + ".Assets.UI.picktobag.png";
                    break;
                default:
                    if (cmddefchar == LastPickedCmd)
                        icon_string = GHApp.AppResourceName + ".Assets.UI.lastitem.png";
                    else if (cmddefchar == OfferCmd)
                        icon_string = GHApp.AppResourceName + ".Assets.UI.offer.png";
                    else if (cmddefchar == PrayCmd)
                        icon_string = GHApp.AppResourceName + ".Assets.UI.pray.png";
                    else if (cmddefchar == DipCmd)
                        icon_string = GHApp.AppResourceName + ".Assets.UI.dip.png";
                    else if (cmddefchar == DigCmd)
                        icon_string = GHApp.AppResourceName + ".Assets.UI.dig.png";
                    else if (cmddefchar == SitCmd)
                        icon_string = GHApp.AppResourceName + ".Assets.UI.sit.png";
                    else if (cmddefchar == RideCmd)
                        icon_string = GHApp.AppResourceName + ".Assets.UI.ride.png";
                    //else if (cmddefchar == PickNStashCmd)
                    //    icon_string = GHApp.AppResourceName + ".Assets.UI.picktobag.png";
                    //else if (cmddefchar == PrevWepCmd)
                    //    icon_string = GHApp.AppResourceName + ".Assets.UI.wield.png";
                    else
                        icon_string = GHApp.AppResourceName + ".Assets.UI.missing_icon.png";
                    break;
            }

            string sourcePath = "resource://" + icon_string;
            ContextMenuButton cmb = new ContextMenuButton(data.cmd_text, sourcePath, GHApp.GetCachedImageSourceBitmap(sourcePath, true), cmdcurchar);
            //cmb.ImgSourcePath = "resource://" + icon_string;
            //cmb.Bitmap = GHApp.GetCachedImageSourceBitmap(cmb.ImgSourcePath, true);
            //cmb.LblText = data.cmd_text;
            //cmb.BtnCommand = cmdcurchar;
            lock (_contextMenuDataLock)
            {
                _contextMenuData.Add(cmb);
            }
        }

        public int Replay_GetLine(int style, int attr, int color, string query, string placeholder, string linesuffix, string introline, IntPtr out_string_ptr, string enteredLine)
        {
            return GetLine(style, attr, color, query, placeholder, linesuffix, introline, out_string_ptr, enteredLine);
        }

        public int ClientCallback_GetLine(int style, int attr, int color, string query, string placeholder, string linesuffix, string introline, IntPtr out_string_ptr)
        {
            return GetLine(style, attr, color, query, placeholder, linesuffix, introline, out_string_ptr, null);
        }

        private int GetLine(int style, int attr, int color, string query, string placeholder, string linesuffix, string introline, IntPtr out_string_ptr, string enteredLine)
        {
            Debug.WriteLine("ClientCallback_GetLine");
            GHApp.AddSentryBreadcrumb("GetLine: " + (query != null ? query : "no query"), GHConstants.SentryGnollHackCallbackCategoryName);

            if (query == null)
                query = "";

            _getLineString = null;
            RequestQueue.Enqueue(new GHRequest(this, GHRequestType.GetLine, query, placeholder, linesuffix, introline, style, attr, color));

            if (PlayingReplay)
            {
                WaitAndCheckPauseReplay(GHConstants.ReplayGetLineDelay1);
                if (!string.IsNullOrWhiteSpace(enteredLine) && enteredLine != "?" && enteredLine != '\x1B'.ToString()) /* If help or cancel button was pressed */
                {
                    RequestQueue.Enqueue(new GHRequest(this, GHRequestType.EnterGetLineText, enteredLine));
                    WaitAndCheckPauseReplay(GHConstants.ReplayGetLineDelay2);
                }
                RequestQueue.Enqueue(new GHRequest(this, GHRequestType.HideGetLine));
                return 0;
            }
            else
            {
                while (_getLineString == null)
                {
                    Thread.Sleep(GHConstants.PollingInterval);
                    PollResponseQueue();
                }

                RecordFunctionCall(RecordedFunctionID.GetLine, style, attr, color, query, placeholder, linesuffix, introline, _getLineString);

                byte[] utf8text = Encoding.UTF8.GetBytes(_getLineString);
                if (out_string_ptr != IntPtr.Zero)
                {
                    Marshal.Copy(utf8text, 0, out_string_ptr, utf8text.Length);
                    Marshal.WriteByte(out_string_ptr, utf8text.Length, 0);
                    GHApp.AddSentryBreadcrumb("GetLine: Result is 1: " + (_getLineString != null ? _getLineString : "no _getLineString"), GHConstants.SentryGnollHackCallbackCategoryName);
                    return 1;
                }
                else
                {
                    GHApp.AddSentryBreadcrumb("GetLine: Result is 0: out_string_ptr is IntPtr.Zero", GHConstants.SentryGnollHackCallbackCategoryName);
                    return 0;
                }
            }
        }

        public void ClientCallback_ClearContextMenu()
        {
            RecordFunctionCall(RecordedFunctionID.ClearContextMenu);
            ClearContextMenu();
            //RequestQueue.Enqueue(new GHRequest(this, GHRequestType.ClearContextMenu));
        }

        public void ClientCallback_AddContextMenu(int cmd_def_char, int cmd_cur_char, int style, int glyph, string cmd_text, string target_text, int attr, int color)
        {
            RecordFunctionCall(RecordedFunctionID.AddContextMenu, cmd_def_char, cmd_cur_char, style, glyph, cmd_text, target_text, attr, color);

            AddContextMenuData data = new AddContextMenuData();
            data.cmd_def_char = cmd_def_char;
            data.cmd_cur_char = cmd_cur_char;
            data.style = style;
            data.glyph = glyph;
            data.cmd_text = cmd_text;
            data.target_text = target_text;
            data.attr = attr;
            data.color = color;
            //RequestQueue.Enqueue(new GHRequest(this, GHRequestType.AddContextMenu, data));
            AddContextMenu(data);
        }

        public void ClientCallback_UpdateStatusButton(int cmd, int btn, int val, ulong bflags)
        {
            RecordFunctionCall(RecordedFunctionID.UpdateStatusButton, cmd, btn, val, bflags);
        }

        public void IncrementCounters(long counter_increment)
        {
            long res = Interlocked.Increment(ref _mainCounterValue);
            if (res == long.MaxValue)
            {
                Interlocked.Exchange(ref _mainCounterValue, 0L);
                res = 0L;
            }
            //lock (_mainCounterLock)
            //{
            //    _mainCounterValue++;
            //    if (_mainCounterValue < 0)
            //        _mainCounterValue = 0;

            //    maincountervalue = Interlocked.CompareExchange(ref _mainCounterValue, 0L, 0L);
            //}

            if (counter_increment > 0)
            {
                /* Only general counter is interlocked and can be accessed without a lock, achieving most of the benefits of having not to lock */
                /* Moved this outside of the lock to minimize the time spent in lock; InvalidateSurface is called after IncrementCounters, so AnimationTimers should always be fully updated when PaintSurface is called (and there will be no lock taken because of IncrementCounters) */
                res = Interlocked.Add(ref AnimationTimers.general_animation_counter, counter_increment);
                if (res < 0) /* Overflowed */
                {
                    Interlocked.Exchange(ref AnimationTimers.general_animation_counter, 0L);
                    res = 0;
                    lock (AnimationTimerLock)
                    {
                        AnimationTimers.CalculateCounterTimeStampsOnOverflowToZero();
                    }
                }
                //generalcountervalue = Interlocked.CompareExchange(ref AnimationTimers.general_animation_counter, 0L, 0L);;

                //lock (AnimationTimerLock)
                //{
                //    //AnimationTimers.general_animation_counter += counter_increment;
                //    //if (AnimationTimers.general_animation_counter < 0)
                //    //    AnimationTimers.general_animation_counter = 0;

                //    if (AnimationTimers.u_action_animation_counter_on)
                //    {
                //        //Interlocked.Add(ref AnimationTimers.u_action_animation_counter, counter_increment);
                //        //if (AnimationTimers.u_action_animation_counter < 0)
                //        //    Interlocked.Exchange(ref AnimationTimers.u_action_animation_counter, 0L);
                //        if (AnimationTimers.u_action_animation_counter > long.MaxValue - counter_increment)
                //            AnimationTimers.u_action_animation_counter = 0;
                //        else
                //            AnimationTimers.u_action_animation_counter += counter_increment;
                //    }

                //    if (AnimationTimers.m_action_animation_counter_on)
                //    {
                //        //Interlocked.Add(ref AnimationTimers.m_action_animation_counter, counter_increment);
                //        //if (AnimationTimers.m_action_animation_counter < 0)
                //        //    Interlocked.Exchange(ref AnimationTimers.m_action_animation_counter, 0L);
                //        if (AnimationTimers.m_action_animation_counter > long.MaxValue - counter_increment)
                //            AnimationTimers.m_action_animation_counter = 0;
                //        else
                //            AnimationTimers.m_action_animation_counter += counter_increment;
                //    }

                //    if (AnimationTimers.explosion_animation_counter_on)
                //    {
                //        //Interlocked.Add(ref AnimationTimers.explosion_animation_counter, counter_increment);
                //        //if (AnimationTimers.explosion_animation_counter < 0)
                //        //    Interlocked.Exchange(ref AnimationTimers.explosion_animation_counter, 0L);
                //        if (AnimationTimers.explosion_animation_counter > long.MaxValue - counter_increment)
                //            AnimationTimers.explosion_animation_counter = 0;
                //        else
                //            AnimationTimers.explosion_animation_counter += counter_increment;
                //    }

                //    for (i = 0; i < GHConstants.MaxPlayedZapAnimations; i++)
                //    {
                //        if (AnimationTimers.zap_animation_counter_on[i])
                //        {
                //            //Interlocked.Add(ref AnimationTimers.zap_animation_counter[i], counter_increment);
                //            //if (AnimationTimers.zap_animation_counter[i] < 0)
                //            //    Interlocked.Exchange(ref AnimationTimers.zap_animation_counter[i], 0L);
                //            if (AnimationTimers.zap_animation_counter[i] > long.MaxValue - counter_increment)
                //                AnimationTimers.zap_animation_counter[i] = 0;
                //            else
                //                AnimationTimers.zap_animation_counter[i] += counter_increment;
                //        }
                //    }

                //    for (i = 0; i < GHConstants.MaxPlayedSpecialEffects; i++)
                //    {
                //        if (AnimationTimers.special_effect_animation_counter_on[i])
                //        {
                //            //Interlocked.Add(ref AnimationTimers.special_effect_animation_counter[i], counter_increment);
                //            //if (AnimationTimers.special_effect_animation_counter[i] < 0)
                //            //    Interlocked.Exchange(ref AnimationTimers.special_effect_animation_counter[i], 0L);
                //            if (AnimationTimers.special_effect_animation_counter[i] > long.MaxValue - counter_increment)
                //                AnimationTimers.special_effect_animation_counter[i] = 0;
                //            else
                //                AnimationTimers.special_effect_animation_counter[i] += counter_increment;
                //        }
                //    }
                //}
            }
        }


        //private readonly object _mainCounterLock = new object();
        private long _mainCounterValue = 0;
        public long MainCounterValue
        {
            get
            {
                //lock (_mainCounterLock) 
                {
                    return Interlocked.CompareExchange(ref _mainCounterValue, 0L, 0L);
                }
            }
        }

        public readonly object AnimationTimerLock = new object();
        public readonly GHAnimationTimerList AnimationTimers = new GHAnimationTimerList();
        public long GeneralAnimationCounter
        {
            get
            {
                return Interlocked.CompareExchange(ref AnimationTimers.general_animation_counter, 0L, 0L);
            }
        }

        public void ClientCallback_ToggleAnimationTimer(int timertype, int timerid, int state, int x, int y, int layer, ulong tflags)
        {
            RecordFunctionCall(RecordedFunctionID.ToggleAnimationTimer, timertype, timerid, state, x, y, layer, tflags);
            bool ison = (state != 0);
            long general_counter_value = Interlocked.CompareExchange(ref AnimationTimers.general_animation_counter, 0L, 0L);
            lock (AnimationTimerLock)
            {
                switch ((animation_timer_types)timertype)
                {
                    case animation_timer_types.ANIMATION_TIMER_GENERAL:
                        break;
                    case animation_timer_types.ANIMATION_TIMER_YOU:
                        //_AnimationTimers.u_action_animation_counter = 0L;
                        AnimationTimers.u_action_animation_counter_timestamp = general_counter_value;
                        AnimationTimers.u_action_animation_counter_on = ison;
                        break;
                    case animation_timer_types.ANIMATION_TIMER_MONSTER:
                        //_AnimationTimers.m_action_animation_counter = 0L;
                        AnimationTimers.m_action_animation_counter_timestamp = general_counter_value;
                        AnimationTimers.m_action_animation_counter_on = ison;
                        AnimationTimers.m_action_animation_x = (byte)x;
                        AnimationTimers.m_action_animation_y = (byte)y;
                        break;
                    case animation_timer_types.ANIMATION_TIMER_EXPLOSION:
                        //_AnimationTimers.explosion_animation_counter = 0L;
                        AnimationTimers.explosion_animation_counter_timestamp = general_counter_value;
                        AnimationTimers.explosion_animation_counter_on = ison;
                        AnimationTimers.explosion_animation_x = (byte)x;
                        AnimationTimers.explosion_animation_y = (byte)y;
                        break;
                    case animation_timer_types.ANIMATION_TIMER_ZAP:
                        if (timerid < 0 || timerid >= GHConstants.MaxPlayedZapAnimations)
                            break;
                        //_AnimationTimers.zap_animation_counter[timerid] = 0L;
                        AnimationTimers.zap_animation_counter_timestamp[timerid] = general_counter_value;
                        AnimationTimers.zap_animation_counter_on[timerid] = ison;
                        AnimationTimers.zap_animation_x[timerid] = (byte)x;
                        AnimationTimers.zap_animation_y[timerid] = (byte)y;
                        break;
                    case animation_timer_types.ANIMATION_TIMER_SPECIAL_EFFECT:
                        if (timerid < 0 || timerid >= GHConstants.MaxPlayedSpecialEffects)
                            break;
                        //_AnimationTimers.special_effect_animation_counter[timerid] = 0L;
                        AnimationTimers.special_effect_animation_counter_timestamp[timerid] = general_counter_value;
                        AnimationTimers.special_effect_animation_counter_on[timerid] = ison;
                        AnimationTimers.spef_action_animation_x[timerid] = (byte)x;
                        AnimationTimers.spef_action_animation_y[timerid] = (byte)y;
                        AnimationTimers.spef_action_animation_layer[timerid] = (layer_types)layer;
                        break;
                    default:
                        break;
                }
            }
        }

        public void ClientCallback_DisplayFloatingText(int x, int y, string text, int style, int attr, int color, ulong tflags)
        {
            RecordFunctionCall(RecordedFunctionID.DisplayFloatingText, x, y, text, style, attr, color, tflags);

            DisplayFloatingTextData floatingTextData = new DisplayFloatingTextData();
            floatingTextData.x = x;
            floatingTextData.y = y;
            floatingTextData.text = text;
            floatingTextData.style = style;
            floatingTextData.attr = attr;
            floatingTextData.color = color;
            floatingTextData.tflags = tflags;
            RequestQueue.Enqueue(new GHRequest(this, GHRequestType.DisplayFloatingText, floatingTextData));
        }

        public void ClientCallback_DisplayScreenText(string text, string supertext, string subtext, int style, int attr, int color, ulong tflags)
        {
            RecordFunctionCall(RecordedFunctionID.DisplayScreenText, text, supertext, subtext, style, attr, color, tflags);

            _screenTextSet = false;
            if ((tflags & 2UL) != 0)
            {
                DisplayConditionTextData data = new DisplayConditionTextData();
                data.text = text.Trim();
                data.style = style;
                data.attr = attr;
                data.color = color;
                data.tflags = tflags;
                RequestQueue.Enqueue(new GHRequest(this, GHRequestType.DisplayConditionText, data));
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
                RequestQueue.Enqueue(new GHRequest(this, GHRequestType.DisplayScreenText, data));
            }

            if ((tflags & 1UL) != 0)
            {
                /* Blocking call */
                while (!_screenTextSet)
                {
                    Thread.Sleep(GHConstants.PollingInterval);
                    PollResponseQueue();
                }

                int cnt = 0;
                long countervalue;
                GamePage gamePage = ActiveGamePage;
                if (gamePage == null)
                    return;
                do
                {
                    countervalue = MainCounterValue;
                    lock (gamePage._screenTextLock)
                    {
                        if (gamePage._screenText != null)
                        {
                            if (gamePage._screenText.IsFinished(countervalue))
                                break;
                        }
                        else
                            break;
                    }
                    Thread.Sleep(GHConstants.PollingInterval);
                    GHApp.FmodService?.PollTasks();
                    cnt++;
                } while (cnt < 2000);
            }
        }

        public void ClientCallback_DisplayPopupText(string text, string title, int style, int attr, int color, int glyph, ulong tflags)
        {
            RecordFunctionCall(RecordedFunctionID.DisplayPopupText, text, title, style, attr, color, glyph, tflags);

            DisplayScreenTextData data = new DisplayScreenTextData();
            data.text = text;
            data.supertext = title;
            data.subtext = title;
            data.style = style;
            data.attr = attr;
            data.color = color;
            data.glyph = glyph;
            data.tflags = tflags;
            RequestQueue.Enqueue(new GHRequest(this, GHRequestType.DisplayPopupText, data));

            int val = ClientCallback_nhgetch();
            if(PlayingReplay)
                WaitAndCheckPauseReplay(GHConstants.ReplayPopupDelay);
            RequestQueue.Enqueue(new GHRequest(this, GHRequestType.HidePopupText));
        }

        public void ClientCallback_DisplayGUIEffect(int style, int subtype, int x, int y, int x2, int y2, ulong tflags)
        {
            RecordFunctionCall(RecordedFunctionID.DisplayGUIEffect, style, subtype, x, y, x2, y2, tflags);

            DisplayGUIEffectData effectData = new DisplayGUIEffectData();
            effectData.style = style;
            effectData.subtype = subtype;
            effectData.x = x;
            effectData.y = y;
            effectData.x2 = x2;
            effectData.y2 = y2;
            effectData.tflags = tflags;
            RequestQueue.Enqueue(new GHRequest(this, GHRequestType.DisplayGUIEffect, effectData));
        }

        public void ClientCallback_UpdateCursor(int style, int force_paint, int show_on_u)
        {
            RecordFunctionCall(RecordedFunctionID.UpdateCursor, style, force_paint, show_on_u);
            //_gamePage.UpdateCursor(style, force_paint, show_on_u);
            UpdateCursor(style, force_paint, show_on_u);
        }

        public void UpdateCursor(int style, int force_paint, int show_on_u)
        {
            lock (_mapDataBufferLock)
            {
                _cursorType = (game_cursor_types)style;
                _force_paint_at_cursor = (force_paint != 0);
                _show_cursor_on_u = (show_on_u != 0);
            }
        }

        public int ClientCallback_PlayImmediateSound(int ghsound, string eventPath, int bankid, double eventVolume, double soundVolume, string[] parameterNames, float[] parameterValues, int arraysize, int sound_type, int play_group, uint dialogue_mid, uint play_flags)
        {
            RecordFunctionCall(RecordedFunctionID.PlayImmediateSound, ghsound, eventPath, bankid, eventVolume, soundVolume, parameterNames, parameterValues, arraysize, sound_type, play_group, dialogue_mid, play_flags);
            if ((play_flags & (uint)playsound_play_flags.PLAY_FLAGS_SONG) != 0)
                GHApp.AddDiscoveredMusic(ghsound);

            if (GHApp.FmodService != null && !GHApp.IsMuted)
            {
                return GHApp.FmodService.PlayImmediateSound(ghsound, eventPath, bankid, (float)eventVolume, (float)soundVolume, parameterNames, parameterValues, arraysize, sound_type, play_group, dialogue_mid, play_flags);
            }

            return 1;
        }

        public int ClientCallback_PlayMusic(int ghsound, string eventPath, int bankid, double eventVolume, double soundVolume)
        {
            RecordFunctionCall(RecordedFunctionID.PlayMusic, ghsound, eventPath, bankid, eventVolume, soundVolume);
            GHApp.AddDiscoveredMusic(ghsound);

            if (GHApp.FmodService != null)
            {
                return GHApp.FmodService.PlayMusic(ghsound, eventPath, bankid, (float)eventVolume, (float)soundVolume);
            }

            return 1;
        }
        public int ClientCallback_PlayLevelAmbient(int ghsound, string eventPath, int bankid, double eventVolume, double soundVolume)
        {
            RecordFunctionCall(RecordedFunctionID.PlayLevelAmbient, ghsound, eventPath, bankid, eventVolume, soundVolume);

            if (GHApp.FmodService != null)
            {
                return GHApp.FmodService.PlayLevelAmbient(ghsound, eventPath, bankid, (float)eventVolume, (float)soundVolume);
            }

            return 1;
        }

        public int ClientCallback_PlayEnvironmentAmbient(int ghsound, string eventPath, int bankid, double eventVolume, double soundVolume)
        {
            RecordFunctionCall(RecordedFunctionID.PlayEnvironmentAmbient, ghsound, eventPath, bankid, eventVolume, soundVolume);

            if (GHApp.FmodService != null)
            {
                return GHApp.FmodService.PlayEnvironmentAmbient(ghsound, eventPath, bankid, (float)eventVolume, (float)soundVolume);
            }

            return 1;
        }

        public int ClientCallback_PlayOccupationAmbient(int ghsound, string eventPath, int bankid, double eventVolume, double soundVolume)
        {
            RecordFunctionCall(RecordedFunctionID.PlayOccupationAmbient, ghsound, eventPath, bankid, eventVolume, soundVolume);

            if (GHApp.FmodService != null)
            {
                return GHApp.FmodService.PlayOccupationAmbient(ghsound, eventPath, bankid, (float)eventVolume, (float)soundVolume);
            }

            return 1;
        }

        public int ClientCallback_PlayEffectAmbient(int ghsound, string eventPath, int bankid, double eventVolume, double soundVolume)
        {
            RecordFunctionCall(RecordedFunctionID.PlayEffectAmbient, ghsound, eventPath, bankid, eventVolume, soundVolume);

            if (GHApp.FmodService != null)
            {
                return GHApp.FmodService.PlayEffectAmbient(ghsound, eventPath, bankid, (float)eventVolume, (float)soundVolume);
            }

            return 1;
        }

        public int ClientCallback_SetEffectAmbientVolume(double soundVolume)
        {
            RecordFunctionCall(RecordedFunctionID.SetEffectAmbientVolume, soundVolume);

            if (GHApp.FmodService != null)
            {
                return GHApp.FmodService.SetEffectAmbientVolume((float)soundVolume);
            }

            return 1;
        }

        public int ClientCallback_AddAmbientSound(int ghsound, string eventPath, int bankid, double eventVolume, double soundVolume, out UInt64 soundSourceId)
        {
            if (GHApp.FmodService != null)
            {
                int res = GHApp.FmodService.AddAmbientSound(ghsound, eventPath, bankid, (float)eventVolume, (float)soundVolume, out soundSourceId);
                RecordFunctionCall(RecordedFunctionID.AddAmbientSound, ghsound, eventPath, bankid, eventVolume, soundVolume, soundSourceId, res);
                return res;
            }
            soundSourceId = 0;
            RecordFunctionCall(RecordedFunctionID.AddAmbientSound, ghsound, eventPath, bankid, eventVolume, soundVolume, soundSourceId, 1);
            return 1;
        }
        public int ClientCallback_DeleteAmbientSound(UInt64 soundSourceId)
        {
            if (GHApp.FmodService != null)
            {
                int res = GHApp.FmodService.DeleteAmbientSound(soundSourceId);
                RecordFunctionCall(RecordedFunctionID.DeleteAmbientSound, soundSourceId, res);
                return res;
            }

            RecordFunctionCall(RecordedFunctionID.DeleteAmbientSound, soundSourceId, 1);
            return 1;
        }
        public int ClientCallback_SetAmbientSoundVolume(UInt64 soundSourceId, double soundVolume)
        {
            if (GHApp.FmodService != null)
            {
                int res = GHApp.FmodService.SetAmbientSoundVolume(soundSourceId, (float)soundVolume);
                RecordFunctionCall(RecordedFunctionID.SetAmbientSoundVolume, soundSourceId, soundVolume, res);
                return res;
            }

            RecordFunctionCall(RecordedFunctionID.SetAmbientSoundVolume, soundSourceId, soundVolume, 1);
            return 1;
        }

        public int ClientCallback_StopAllSounds(uint flags, uint dialogue_mid)
        {
            if (GHApp.FmodService != null)
            {
                int res = GHApp.FmodService.StopAllGameSounds(flags, dialogue_mid);
                RecordFunctionCall(RecordedFunctionID.StopAllSounds, flags, dialogue_mid, res);
                return res;
            }

            RecordFunctionCall(RecordedFunctionID.StopAllSounds, flags, dialogue_mid, 1);
            return 1;
        }

        public void ClientCallback_IssueGuiCommand(int cmd_id, int cmd_param, int cmd_param2, string cmd_str)
        {
            if(cmd_id != (int)gui_command_types.GUI_CMD_LOAD_GLYPHS)
                RecordFunctionCall(RecordedFunctionID.IssueGuiCommand, cmd_id, cmd_param, cmd_param2, cmd_str);

            string status_str = "";
            switch (cmd_id)
            {
                case (int)gui_command_types.GUI_CMD_LOAD_GLYPHS:
                    if(!PlayingReplay)
                    {
                        /* Reinitialize  glyph2tile after object shuffling */
                        IntPtr gl2ti_ptr;
                        int gl2ti_size;
                        IntPtr gltifl_ptr;
                        int gltifl_size;

                        GHApp.GnollHackService.GetGlyphArrays(out gl2ti_ptr, out gl2ti_size, out gltifl_ptr, out gltifl_size);
                        lock (GHApp.Glyph2TileLock)
                        {
                            if (gl2ti_ptr != IntPtr.Zero && gl2ti_size > 0)
                            {
                                if (GHApp.Glyph2Tile == null || gl2ti_size != GHApp.Glyph2Tile.Length)
                                    GHApp.Glyph2Tile = new int[gl2ti_size];
                                Marshal.Copy(gl2ti_ptr, GHApp.Glyph2Tile, 0, gl2ti_size);
                            }
                            if (gltifl_ptr != IntPtr.Zero && gltifl_size > 0)
                            {
                                if (GHApp.GlyphTileFlags == null || gltifl_size != GHApp.GlyphTileFlags.Length)
                                    GHApp.GlyphTileFlags = new byte[gltifl_size];
                                Marshal.Copy(gltifl_ptr, GHApp.GlyphTileFlags, 0, gltifl_size);
                            }
                            RecordFunctionCall(RecordedFunctionID.IssueGuiCommand, cmd_id, GHApp.Glyph2Tile, GHApp.GlyphTileFlags);
                        }
                    }
                    break;
                case (int)gui_command_types.GUI_CMD_FADE_TO_BLACK:
                    if (PlayingReplay && GHApp.IsReplaySearching)
                        return;
                    RequestQueue.Enqueue(new GHRequest(this, GHRequestType.FadeToBlack, GHConstants.FadeToBlackDuration));
                    Thread.Sleep(GHConstants.FadeToBlackDelay);
                    break;
                case (int)gui_command_types.GUI_CMD_FADE_FROM_BLACK_SLOWLY_NONBLOCKING:
                    if (PlayingReplay && GHApp.IsReplaySearching)
                        return;
                    RequestQueue.Enqueue(new GHRequest(this, GHRequestType.FadeFromBlack, GHConstants.FadeFromBlackDurationAtStart + (int)(GHConstants.FadeFromBlackAtStartExtraDelaySecs * 1000)));
                    break;
                case (int)gui_command_types.GUI_CMD_SET_TO_BLACK:
                    if (PlayingReplay && GHApp.IsReplaySearching)
                        return;
                    RequestQueue.Enqueue(new GHRequest(this, GHRequestType.SetToBlack));
                    break;
                case (int)gui_command_types.GUI_CMD_COLLECT_GARBAGE:
                    if (PlayingReplay && GHApp.IsReplaySearching)
                        return;
                    switch (cmd_param)
                    {
                        case 0:
                            GHApp.IsCriticalClearCachesAndMemoryOk = true;
                            GHApp.IsCompleteClearCachesAndMemoryOk = true;
                            GHApp.CollectGarbage(); /* Collect all, may take a lot of time */
                            break;
                        case 1:
                            //GC.Collect(cmd_param2); /* Collect only from 0 up to the specified generation, 0 = only nursery */
                            GHApp.CollectNursery();
                            break;
                    }
                    break;
                case (int)gui_command_types.GUI_CMD_FADE_FROM_BLACK:
                    if (PlayingReplay && GHApp.IsReplaySearching)
                        return;
                    RequestQueue.Enqueue(new GHRequest(this, GHRequestType.FadeFromBlack, GHConstants.FadeFromBlackDuration));
                    break;
                case (int)gui_command_types.GUI_CMD_FORCE_ASCII:
                    {
                        GamePage gamePage = ActiveGamePage;
                        if (gamePage != null)
                            gamePage.ForceAscii = true;
                    }
                    break;
                case (int)gui_command_types.GUI_CMD_UNFORCE_ASCII:
                    {
                        GamePage gamePage = ActiveGamePage;
                        if (gamePage != null)
                            gamePage.ForceAscii = false;
                    }
                    break;
                case (int)gui_command_types.GUI_CMD_MUTE_SOUNDS:
                    GHApp.GameMuteMode = true;
                    break;
                case (int)gui_command_types.GUI_CMD_UNMUTE_SOUNDS:
                    GHApp.GameMuteMode = false;
                    break;
                case (int)gui_command_types.GUI_CMD_ACTIVATE_QUIETER_MODE:
                    GHApp.FmodService?.SetQuieterMode(true);
                    break;
                case (int)gui_command_types.GUI_CMD_DEACTIVATE_QUIETER_MODE:
                    GHApp.FmodService?.SetQuieterMode(false);
                    break;
                case (int)gui_command_types.GUI_CMD_LOAD_VIDEOS:
                    break;
                case (int)gui_command_types.GUI_CMD_ENABLE_WIZARD_MODE:
                    {
                        GamePage gamePage = ActiveGamePage;
                        if (gamePage != null)
                        {
                            gamePage.EnableWizardMode = true;
                            gamePage.ExtendedCommands = GHApp.GnollHackService.GetExtendedCommands();
                            GHApp.AddSentryBreadcrumb("Wizard Mode Enabled", GHConstants.SentryGnollHackGeneralCategoryName);
                        }
                    }
                    break;
                case (int)gui_command_types.GUI_CMD_DISABLE_WIZARD_MODE:
                    {
                        GamePage gamePage = ActiveGamePage;
                        if (gamePage != null && gamePage.EnableWizardMode)
                        {
                            gamePage.EnableWizardMode = false;
                            gamePage.ExtendedCommands = GHApp.GnollHackService.GetExtendedCommands();
                            GHApp.AddSentryBreadcrumb("Wizard Mode Disabled", GHConstants.SentryGnollHackGeneralCategoryName);
                        }
                    }
                    break;
                case (int)gui_command_types.GUI_CMD_ENABLE_CASUAL_MODE:
                    {
                        GamePage gamePage = ActiveGamePage;
                        if (gamePage != null)
                        {
                            gamePage.EnableCasualMode = true;
                            gamePage.ExtendedCommands = GHApp.GnollHackService.GetExtendedCommands();
                            GHApp.AddSentryBreadcrumb("Casual Mode Enabled", GHConstants.SentryGnollHackGeneralCategoryName);
                        }
                    }
                    break;
                case (int)gui_command_types.GUI_CMD_DISABLE_CASUAL_MODE:
                    {
                        GamePage gamePage = ActiveGamePage;
                        if (gamePage != null && gamePage.EnableCasualMode)
                        {
                            gamePage.EnableCasualMode = false;
                            gamePage.ExtendedCommands = GHApp.GnollHackService.GetExtendedCommands();
                            GHApp.AddSentryBreadcrumb("Casual Mode Disabled", GHConstants.SentryGnollHackGeneralCategoryName);
                        }
                    }
                    break;
                case (int)gui_command_types.GUI_CMD_ENABLE_TOURNAMENT_MODE:
                    GHApp.TournamentMode = true;
                    GHApp.AddSentryBreadcrumb("Tournament Mode Enabled", GHConstants.SentryGnollHackGeneralCategoryName);
                    break;
                case (int)gui_command_types.GUI_CMD_DISABLE_TOURNAMENT_MODE:
                    GHApp.TournamentMode = false;
                    GHApp.AddSentryBreadcrumb("Tournament Mode Disabled", GHConstants.SentryGnollHackGeneralCategoryName);
                    break;
                case (int)gui_command_types.GUI_CMD_CLEAR_PET_DATA:
                    ClearPetData();
                    //RequestQueue.Enqueue(new GHRequest(this, GHRequestType.ClearPetData));
                    break;
                case (int)gui_command_types.GUI_CMD_SAVE_AND_DISABLE_TRAVEL_MODE:
                    RequestQueue.Enqueue(new GHRequest(this, GHRequestType.SaveAndDisableTravelMode));
                    break;
                case (int)gui_command_types.GUI_CMD_RESTORE_TRAVEL_MODE:
                    RequestQueue.Enqueue(new GHRequest(this, GHRequestType.RestoreTravelMode));
                    break;
                case (int)gui_command_types.GUI_CMD_SAVE_AND_DISABLE_TRAVEL_MODE_ON_LEVEL:
                    RequestQueue.Enqueue(new GHRequest(this, GHRequestType.SaveAndDisableTravelModeOnLevel));
                    break;
                case (int)gui_command_types.GUI_CMD_RESTORE_TRAVEL_MODE_ON_LEVEL:
                    RequestQueue.Enqueue(new GHRequest(this, GHRequestType.RestoreTravelModeOnLevel));
                    break;
                case (int)gui_command_types.GUI_CMD_CLEAR_CONDITION_TEXTS:
                    ActiveGamePage?.ClearConditionTexts();
                    break;
                case (int)gui_command_types.GUI_CMD_CLEAR_FLOATING_TEXTS:
                    ActiveGamePage?.ClearFloatingTexts();
                    break;
                case (int)gui_command_types.GUI_CMD_CLEAR_GUI_EFFECTS:
                    ActiveGamePage?.ClearGuiEffects();
                    break;
                case (int)gui_command_types.GUI_CMD_CLEAR_MESSAGE_HISTORY:
                    _longer_message_history = new GHMsgHistoryList(_longer_message_history.Capacity, _longer_message_history.Excess);
                    RequestQueue.Enqueue(new GHRequest(this, GHRequestType.PrintHistory));
                    break;
                case (int)gui_command_types.GUI_CMD_LOAD_INTRO_SOUND_BANK:
                    if(GHApp.LoadBanks)
                        GHApp.FmodService.LoadIntroSoundBank();
                    break;
                case (int)gui_command_types.GUI_CMD_UNLOAD_INTRO_SOUND_BANK:
                    if (GHApp.LoadBanks)
                        GHApp.FmodService.UnloadIntroSoundBank();
                    break;
                case (int)gui_command_types.GUI_CMD_WAIT_FOR_RESUME:
                    if (PlayingReplay)
                        break;
                    SaveDoneConfirmed = true;
                    GHApp.GameSaved = true;
                    GHApp.GameSaveResult = cmd_param;
                    GHApp.SavingGame = false;
                    if(cmd_param != 0)
                    {
                        RequestQueue.Enqueue(new GHRequest(this, GHRequestType.CloseAllDialogs));
                    }
                    while (!_restoreRequested)
                    {
                        Thread.Sleep(GHConstants.PollingInterval);
                        _saveRequested = false; //Should be the case. but just in case there is some sort of a mixup going on so that we do not save and restore again in PollResponseQueue
                        PollResponseQueue();
                    }
                    _restoreRequested = false;
                    break;
                case (int)gui_command_types.GUI_CMD_POST_DIAGNOSTIC_DATA:
                    if (PlayingReplay)
                        break;

                    if (cmd_str != null)
                        status_str = cmd_str;
#if SENTRY
                    switch(cmd_param)
                    {
                        case (int)diagnostic_data_types.DIAGNOSTIC_DATA_CRITICAL:
                        default:
                            string data_title = cmd_param == (int)diagnostic_data_types.DIAGNOSTIC_DATA_CRITICAL ? "C - " : "D - ";
                            SentrySdk.CaptureMessage(data_title + status_str);
                            break;
                        case (int)diagnostic_data_types.DIAGNOSTIC_DATA_IMPOSSIBLE:
                        case (int)diagnostic_data_types.DIAGNOSTIC_DATA_PANIC:
                            string[] strs = status_str.Split('|');
                            if (strs == null || strs.Length == 0)
                                break;
                            var sentryEvent = new SentryEvent();
                            sentryEvent.SentryExceptions = new[]
                            {
                                new SentryException
                                {
                                    Type = cmd_param == (int)diagnostic_data_types.DIAGNOSTIC_DATA_IMPOSSIBLE ? "Impossible" : "Panic",
                                    Value = strs[0],
                                    Mechanism = new Mechanism
                                    {
                                        Type = "generic",
                                        Description = "IssueGuiCommand: GUI_CMD_POST_DIAGNOSTIC_DATA"
                                    }
                                }
                            };
                            if (strs.Length > 1)
                                sentryEvent.SetExtra("Debug Buffers", strs[1]);
                            SentrySdk.CaptureEvent(sentryEvent);
                            break;
                    }
#else
                    if (GHApp.PostingDiagnosticData)
                    {
                        RequestQueue.Enqueue(new GHRequest(this,
                            GHRequestType.PostDiagnosticData,
                            cmd_param, cmd_param2, status_str));
                    }
#endif
                    break;
                case (int)gui_command_types.GUI_CMD_POST_GAME_STATUS:
                    if (PlayingReplay)
                        break;

                    if (cmd_param == (int)game_status_types.GAME_STATUS_POST_IF_CLOUD_REPLAY_ON
                        && (!GHApp.RecordGame || !GHApp.AutoUploadReplays))
                        break;

                    if (cmd_str != null)
                        status_str = cmd_str;

                    if (GHApp.PostingGameStatus)
                    {
                        if (cmd_param == (int)game_status_types.GAME_STATUS_RESULT_ATTACHMENT)
                        {
                            GHApp.MaybeWriteGHLog("Forum Post: Attaching the file " + cmd_str + " to the next post.");
                        }
                        else
                        {
                            GHApp.MaybeWriteGHLog("Forum Post: Posting game progress to the server: " + status_str);
                        }
                        RequestQueue.Enqueue(new GHRequest(this,
                            GHRequestType.PostGameStatus,
                            cmd_param, cmd_param2, status_str));
                    }
                    else
                    {
                        GHApp.MaybeWriteGHLog("Forum Post: Received a request to " + (cmd_param == (int)game_status_types.GAME_STATUS_RESULT_ATTACHMENT ? "attach a file to the next forum post" : "post game progress to the server") + ", but Post Game Progress setting is off.");
                    }
                    break;
                case (int)gui_command_types.GUI_CMD_POST_XLOG_ENTRY:
                    if (PlayingReplay)
                        break;

                    if (cmd_str != null)
                        status_str = cmd_str;
                    if (GHApp.PostingXlogEntries)
                    {
                        if(cmd_param == (int)game_status_types.GAME_STATUS_RESULT_ATTACHMENT)
                            GHApp.MaybeWriteGHLog("XLog Post: Attaching the file " + cmd_str + " to the next post.");
                        else
                            GHApp.MaybeWriteGHLog("XLog Post: Posting the top score entry to the server.");
                        RequestQueue.Enqueue(new GHRequest(this, GHRequestType.PostXlogEntry, cmd_param, cmd_param2, status_str));
                    }
                    else
                    {
                        GHApp.MaybeWriteGHLog("Received a request to " + (cmd_param == (int)game_status_types.GAME_STATUS_RESULT_ATTACHMENT ? "attach a file to the next top score post" : "post the top score entry to the server") + ", but Post Top Scores setting is off.");
                    }
                    break;
                case (int)gui_command_types.GUI_CMD_POST_BONES_FILE:
                    if (PlayingReplay)
                        break;

                    if (cmd_str != null)
                        status_str = cmd_str;

                    if (GHApp.AllowBones && GHApp.PostingBonesFiles)
                    {
                        Random rnd = new Random();
                        if (GHApp.ForcePostBones || rnd.NextDouble() < GHConstants.BonesPostBaseChance)
                        {
                            GHApp.MaybeWriteGHLog("Bones Post: Chose to post the bones file (" + status_str + ") to the server for user " + GHApp.XlogUserName + ".");
                            RequestQueue.Enqueue(new GHRequest(this, GHRequestType.PostBonesFile, cmd_param, cmd_param2, status_str));
                        }
                        else
                        {
                            GHApp.MaybeWriteGHLog("Bones Retention: Chose not to post the bones file (" + status_str + ") to the server for user " + GHApp.XlogUserName + ". Bones file retained locally.");
                        }
                    }
                    else
                    {
                        GHApp.MaybeWriteGHLog("Received a request to post the bones file " + status_str + " to the server, but "
                            + (!GHApp.AllowBones && !GHApp.PostingBonesFiles ? "Allow Ghost Levels and Share Bones Files settings are both off" : GHApp.AllowBones ? "Allow Ghost Levels setting is off" : "Share Bones Files setting is off")
                            + (!GHApp.AllowBones ? ". Allow Ghost Levels was probably on when the game was started" : "")
                            + ". Retaining the bones file locally.");
                    }
                    break;
                case (int)gui_command_types.GUI_CMD_LIBRARY_MANUAL:
                case (int)gui_command_types.GUI_CMD_ORACLE_MAJOR_CONSULTATION:
                    if (PlayingReplay)
                        break;

                    if (cmd_param >= 0 && cmd_param < GHConstants.MaxGHWindows)
                    {
                        string windowText = "";
                        //lock (_ghWindowsLock)
                        {
                            if (_ghWindows[cmd_param] != null)
                            {
                                foreach(GHPutStrItem psi in _ghWindows[cmd_param].PutStrs)
                                {
                                    if(windowText != "")
                                        windowText += Environment.NewLine;
                                    windowText += psi.Text;
                                }
                            }
                        }
                        if(windowText != "" && cmd_str != null && cmd_str != "")
                        {
                            string datadir = Path.Combine(GHApp.GHPath, GHConstants.UserDataDirectory);
                            if(!Directory.Exists(datadir))
                                GHApp.CheckCreateDirectory(datadir);
                            if(Directory.Exists(datadir))
                            {
                                string filename = (cmd_id == (int)gui_command_types.GUI_CMD_LIBRARY_MANUAL ? GHConstants.ManualFilePrefix : GHConstants.OracleMajorConsultationFilePrefix) + cmd_param2.ToString() + ".json";
                                string filepath = Path.Combine(datadir, filename);
                                if (File.Exists(filepath))
                                {
                                    try
                                    {
                                        File.Delete(filepath); /* Delete and update with the newest version; the existing one can be outdated */
                                    }
                                    catch (Exception ex)
                                    {
                                        Debug.WriteLine(ex);
                                    }
                                }
                                if (!File.Exists(filepath))
                                {
                                    try
                                    {
                                        StoredManual manual = new StoredManual(cmd_str, cmd_param2, windowText);
                                        string json = JsonConvert.SerializeObject(manual, Formatting.Indented);
                                        using (FileStream fs = File.Create(filepath))
                                        {
                                            using (StreamWriter sw = new StreamWriter(fs))
                                            {
                                                sw.Write(json);
                                            }
                                        }
                                    }
                                    catch (Exception ex)
                                    {
                                        Debug.WriteLine(ex);
                                    }
                                }
                            }
                        }
                    }
                    break;
                case (int)gui_command_types.GUI_CMD_DEBUGLOG:
                    if(!PlayingReplay && !string.IsNullOrWhiteSpace(cmd_str))
                    {
                        if (cmd_param == (int)debug_log_types.DEBUGLOG_DEBUG_ONLY && !GHApp.IsDebug)
                            break;
                        string logged_str = cmd_str + (cmd_param2 != 0 ? " [" + cmd_param2 + "]" : "");
                        GHApp.MaybeWriteGHLog(logged_str);
                        if (cmd_param == (int)debug_log_types.DEBUGLOG_PRIORITY)
                        {
#if SENTRY
                            SentrySdk.CaptureMessage("Log: " + logged_str);
#endif
                        }
                        else if (cmd_param == (int)debug_log_types.DEBUGLOG_PANIC || cmd_param == (int)debug_log_types.DEBUGLOG_IMPOSSIBLE)
                        {
#if SENTRY
                            string[] strs = logged_str.Split('|');
                            if (strs == null || strs.Length == 0)
                                break;
                            var sentryEvent = new SentryEvent();
                            sentryEvent.SentryExceptions = new[]
                            {
                                new SentryException
                                {
                                    Type = cmd_param == (int)debug_log_types.DEBUGLOG_IMPOSSIBLE ? "Impossible" : "Panic",
                                    Value = strs[0],
                                    Mechanism = new Mechanism
                                    {
                                        Type = "generic",
                                        Description = "IssueGuiCommand: " + (cmd_param == (int)debug_log_types.DEBUGLOG_IMPOSSIBLE ? "DEBUGLOG_IMPOSSIBLE" : "DEBUGLOG_PANIC")
                                    }
                                }
                            };
                            if (strs.Length > 1)
                                sentryEvent.SetExtra("Debug Buffers", strs[1]);
                            SentrySdk.CaptureEvent(sentryEvent);
#endif
                        }
                    }
                    //if (GHGame.RequestDictionary.TryGetValue(this, out queue))
                    //{
                    //    RequestQueue.Enqueue(new GHRequest(this, GHRequestType.DebugLog, cmd_param, cmd_param2, cmd_str));
                    //}
                    break;
                case (int)gui_command_types.GUI_CMD_GAME_ENDED:
                    _gameHasEnded = true;
                    RequestQueue.Enqueue(new GHRequest(this, GHRequestType.GameEnded));
                    if (PlayingReplay)
                        break;
                    GHApp.TryVerifyXlogUserName(); /* In case not verified yet; in advance of possibly posting files to the server and forums */
                    break;
                case (int)gui_command_types.GUI_CMD_TOGGLE_MENU_POSITION_SAVING:
                    RequestQueue.Enqueue(new GHRequest(this, GHRequestType.ToggleMenuPositionSaving, cmd_param, cmd_param2));
                    break;
                case (int)gui_command_types.GUI_CMD_TOGGLE_CHARACTER_CLICK_ACTION:
                    GHApp.MirroredCharacterClickAction = cmd_param != 0;
                    break;
                case (int)gui_command_types.GUI_CMD_TOGGLE_DICE_AS_RANGES:
                    GHApp.MirroredDiceAsRanges = cmd_param != 0;
                    break;
                case (int)gui_command_types.GUI_CMD_TOGGLE_AUTODIG:
                    GHApp.MirroredAutoDig = cmd_param != 0;
                    ActiveGamePage?.ToggleMapAutoDigOnMainThread(cmd_param != 0); //Do not set the game value of autodig; this is GUI notification that it has been changed
                    break;
                case (int)gui_command_types.GUI_CMD_TOGGLE_IGNORE_STOPPING:
                    GHApp.MirroredIgnoreStopping = cmd_param != 0;
                    ActiveGamePage?.ToggleMapIgnoreModeOnMainThread(cmd_param != 0); //Do not set the game value of autodig; this is GUI notification that it has been changed
                    break;
                case (int)gui_command_types.GUI_CMD_TOGGLE_GETPOS_ARROWS:
                    GHApp.GetPositionArrows = cmd_param != 0;
                    break;
                case (int)gui_command_types.GUI_CMD_DELETE_TRACKING_FILE:
                    try
                    {
                        if (!string.IsNullOrEmpty(status_str))
                        {
                            string trackingFile = status_str + GHConstants.SaveFileTrackingSuffix;
                            if (File.Exists(trackingFile))
                                File.Delete(trackingFile);
                        }
                    }
                    catch (Exception ex)
                    {
                        GHApp.MaybeWriteGHLog("Delete tracking file: " + ex.Message);
                    }
                    break;
                case (int)gui_command_types.GUI_CMD_START_FLUSH:
                    break;
                case (int)gui_command_types.GUI_CMD_FINISH_FLUSH:
                    //_gamePage.ProcessPrintGlyphCallList(_savedPrintGlyphCalls, _savedSendObjectDataCalls, _savedSendMonsterDataCalls, _savedSendEngravingDataCalls);
                    //_savedPrintGlyphCalls.Clear();
                    //_savedSendObjectDataCalls.Clear();
                    //_savedSendMonsterDataCalls.Clear();
                    //_savedSendEngravingDataCalls.Clear();
                    break;
                case (int)gui_command_types.GUI_CMD_REPORT_MOUSE_COMMAND:
                    if (cmd_param2 != 0)
                        GHApp.MirroredMiddleMouseCommand = cmd_param;
                    else
                        GHApp.MirroredRightMouseCommand = cmd_param;
                    break;
                case (int)gui_command_types.GUI_CMD_TOGGLE_QUICK_ZAP_WAND:
                    SetQuickZapWand(cmd_param, cmd_param2, cmd_str);
                    GHApp.AddSentryBreadcrumb("Notify GUI finished", GHConstants.SentryGnollHackGeneralCategoryName);
                    break;
                case (int)gui_command_types.GUI_CMD_TOGGLE_QUICK_CAST_SPELL:
                    SetQuickCastSpell(cmd_param, cmd_param2, cmd_str);
                    break;
                case (int)gui_command_types.GUI_CMD_ZOOM_NORMAL:
                    if (!PlayingReplay)
                        RequestQueue.Enqueue(new GHRequest(this, GHRequestType.ZoomNormal));
                    break;
                case (int)gui_command_types.GUI_CMD_ZOOM_IN:
                    if (!PlayingReplay)
                        RequestQueue.Enqueue(new GHRequest(this, GHRequestType.ZoomIn));
                    break;
                case (int)gui_command_types.GUI_CMD_ZOOM_OUT:
                    if (!PlayingReplay)
                        RequestQueue.Enqueue(new GHRequest(this, GHRequestType.ZoomOut));
                    break;
                case (int)gui_command_types.GUI_CMD_ZOOM_MINI:
                    if (!PlayingReplay)
                        RequestQueue.Enqueue(new GHRequest(this, GHRequestType.ZoomMini));
                    break;
                case (int)gui_command_types.GUI_CMD_ZOOM_HALF:
                    if (!PlayingReplay)
                        RequestQueue.Enqueue(new GHRequest(this, GHRequestType.ZoomHalf));
                    break;
                case (int)gui_command_types.GUI_CMD_ZOOM_TO_SCALE:
                    break;
                case (int)gui_command_types.GUI_CMD_SAVE_ZOOM:
                    break;
                case (int)gui_command_types.GUI_CMD_RESTORE_ZOOM:
                    break;
                case (int)gui_command_types.GUI_CMD_KEYBOARD_FOCUS:
                    if(!PlayingReplay)
                        RequestQueue.Enqueue(new GHRequest(this, GHRequestType.KeyboardFocus));
                    break;
                case (int)gui_command_types.GUI_CMD_EXIT_APP_ON_MAIN_SCREEN:
                    GHApp.DoAppExitOnReturn = true;
                    break;
                case (int)gui_command_types.GUI_CMD_BREADCRUMB:
                    GHApp.AddSentryBreadcrumb(cmd_str, GHConstants.SentryGnollHackLibraryCategoryName);
                    break;
                case (int)gui_command_types.GUI_CMD_BREADCRUMB2:
                    GHApp.AddSentryBreadcrumb(cmd_str + ": " + cmd_param, GHConstants.SentryGnollHackLibraryCategoryName);
                    break;
                case (int)gui_command_types.GUI_CMD_BREADCRUMB3:
                    GHApp.AddSentryBreadcrumb(cmd_str + ": " + cmd_param + ", " + cmd_param2, GHConstants.SentryGnollHackLibraryCategoryName);
                    break;
                default:
                    break;
            }
        }

        public void ClientCallback_OutRip(int winid, string plname, int points, string killer, string time)
        {
            RecordFunctionCall(RecordedFunctionID.OutRip, winid, plname, points, killer, time);

            if (_fastForwardGameOver)
                return;

            if (_ghWindows[winid] != null)
            {
                GHWindow clonedWindow = _ghWindows[winid].Clone();
                RequestQueue.Enqueue(new GHRequest(this, GHRequestType.UpdateGHWindow, winid, clonedWindow));
                RequestQueue.Enqueue(new GHRequest(this, GHRequestType.ShowOutRipPage, clonedWindow, new GHOutRipInfo(plname, points, killer, time)));
            }

            int res = ClientCallback_nhgetch();
            if(PlayingReplay || _fastForwardGameOver)
            {
                /* Only like this for replay, as normal hiding code is a bit more robust */
                if (!_fastForwardGameOver && !GHApp.StopReplay && !GHApp.IsReplaySearching) /* No pause, since outrip page hides the controls */
                {
                    Thread.Sleep((int)(GHConstants.ReplayOutripDelay / GHApp.ReplaySpeed));
                    GHApp.FmodService?.PollTasks();
                }

                //lock (_ghWindowsLock)
                {
                    if (_ghWindows[winid] != null)
                    {
                        RequestQueue.Enqueue(new GHRequest(this, GHRequestType.HideOutRipPage));
                    }
                }
            }
        }

        public int ClientCallback_UIHasInput()
        {
            if (PlayingReplay)
                return 0;

            RecordFunctionCall(RecordedFunctionID.UIHasInput, ResponseQueue.Count);
            return ResponseQueue.Count;
        }

        public byte ClientCallback_AdjustGeneralVolumes(double value1, double value2, double value3, double value4, double value5, double value6)
        {
            /* Unused; handled on the GUI side completely */
            return 0;
        }

        public int ClientCallback_OpenSpecialView(int viewtype, string text, string title, int attr, int color, long time_stamp)
        {
            RecordFunctionCall(RecordedFunctionID.OpenSpecialView, viewtype, text, title, attr, color); //add time_stamp here later
            switch (viewtype)
            {
                case (int)special_view_types.SPECIAL_VIEW_CHAT_MESSAGE:
                    break;
                case (int)special_view_types.SPECIAL_VIEW_SHOW_SPECIAL_EFFECT:
                    /* Do nothing currently */
                    break;
                case (int)special_view_types.SPECIAL_VIEW_GUI_TIPS:
                    {
                        if (PlayingReplay)
                        {
                            Debug.WriteLine("View GUI Tips in Replay");
                            break;
                        }

                        if (GHApp.GUITipsShown)
                            break;

                        _guiTipsFinished = false;
                        RequestQueue.Enqueue(new GHRequest(this, GHRequestType.ShowGUITips));
                        while (!_guiTipsFinished)
                        {
                            Thread.Sleep(GHConstants.PollingInterval);
                            PollResponseQueue();
                        }
                        break;
                    }
                case (int)special_view_types.SPECIAL_VIEW_CRASH_DETECTED:
                    {
                        if (PlayingReplay)
                        {
                            Debug.WriteLine("Crash Detected in Replay");
                            break;
                        }
                        _crashReportFinished = false;
                        RequestQueue.Enqueue(new GHRequest(this, GHRequestType.CrashReport));
                        while (!_crashReportFinished)
                        {
                            Thread.Sleep(GHConstants.PollingInterval);
                            PollResponseQueue();
                        }
                        break;
                    }
                case (int)special_view_types.SPECIAL_VIEW_PANIC:
                    {
                        /* Already logged by IssueGuiCommand: Panic */

//                        if(!PlayingReplay)
//                        {
//#if SENTRY
//                            //SentrySdk.CaptureMessage("Panic: " + text);
//                            var sentryEvent = new SentryEvent();
//                            sentryEvent.SentryExceptions = new[]
//                            {
//                                new SentryException
//                                {
//                                    Type = "Panic",
//                                    Value = text,
//                                    Mechanism = new Mechanism
//                                    {
//                                        Type = "generic",
//                                        Description = "OpenSpecialView: SPECIAL_VIEW_PANIC"
//                                    }
//                                }
//                            };
//                            SentrySdk.CaptureEvent(sentryEvent);
//#endif
//                        }

                        _panicFinished = false;
                        RequestQueue.Enqueue(new GHRequest(this, GHRequestType.Panic, text));
                        while (!_panicFinished)
                        {
                            Thread.Sleep(GHConstants.PollingInterval);
                            PollResponseQueue();
                        }
                        break;
                    }
                case (int)special_view_types.SPECIAL_VIEW_DEBUGLOG:
                    if (!string.IsNullOrWhiteSpace(text))
                        GHApp.MaybeWriteGHLog(text + (attr != 0 ? " [" + attr + "]" : ""));
                    break;
                case (int)special_view_types.SPECIAL_VIEW_MESSAGE:
                    {
                        if (PlayingReplay)
                        {
                            Debug.WriteLine("Message in Replay: " + text);
                            break;
                        }
                        _messageFinished = false;
                        RequestQueue.Enqueue(new GHRequest(this, GHRequestType.Message, text));
                        while (!_messageFinished)
                        {
                            Thread.Sleep(GHConstants.PollingInterval);
                            PollResponseQueue();
                        }
                        break;
                    }
                case (int)special_view_types.SPECIAL_VIEW_HELP_DIR:
                    break;
                case (int)special_view_types.SPECIAL_VIEW_GUI_YN_CONFIRMATION_DEFAULT_Y:
                case (int)special_view_types.SPECIAL_VIEW_GUI_YN_CONFIRMATION_DEFAULT_N:
                    {
                        if (PlayingReplay)
                        {
                            Debug.WriteLine("YN Confirmation in Replay: " + title + ": " + text);
                            break;
                        }
                        char def = viewtype == (int)special_view_types.SPECIAL_VIEW_GUI_YN_CONFIRMATION_DEFAULT_Y ? 'y' : 'n'; ;
                        if (_fastForwardGameOver)
                            return def;
                        _ynConfirmationFinished = false;
                        RequestQueue.Enqueue(new GHRequest(this, GHRequestType.YnConfirmation, title, text, "Yes", "No"));
                        while (!_ynConfirmationFinished)
                        {
                            Thread.Sleep(GHConstants.PollingInterval);
                            PollResponseQueue();
                            if (_fastForwardGameOver)
                                return def;
                        }
                        return _ynConfirmationResult ? 'y' : 'n';
                    }
                case (int)special_view_types.SPECIAL_VIEW_SAVE_FILE_TRACKING_LOAD:
                case (int)special_view_types.SPECIAL_VIEW_SAVE_FILE_TRACKING_SAVE:
                    if(!PlayingReplay)
                    {
                        string filename = text;
                        try
                        {
                            if (time_stamp > 0)
                            {

                                //Calculate
                                if (!string.IsNullOrEmpty(filename) && File.Exists(filename))
                                {
                                    FileInfo fi = new FileInfo(filename);
                                    long length = fi.Length;
                                    string shaHashBase64 = "";
                                    using (var shaAlgorithm = SHA256.Create())
                                    {
                                        using (FileStream fs = File.OpenRead(filename))
                                        {
                                            byte[] shaHashbytes = shaAlgorithm.ComputeHash(fs);
                                            shaHashBase64 = Convert.ToBase64String(shaHashbytes);
                                        }
                                    }
                                    if (length > 0 && !string.IsNullOrEmpty(shaHashBase64))
                                    {
                                        //Start contacting server and saving the code to disk
                                        //Wait until contacting is done, or fails due to timeout etc.
                                        _saveFileTrackingFinished = -1;
                                        RequestQueue.Enqueue(new GHRequest(this, viewtype == (int)special_view_types.SPECIAL_VIEW_SAVE_FILE_TRACKING_LOAD ? GHRequestType.SaveFileTrackingLoad : GHRequestType.SaveFileTrackingSave, time_stamp, filename, length, shaHashBase64));
                                        while (_saveFileTrackingFinished < 0)
                                        {
                                            Thread.Sleep(GHConstants.PollingInterval);
                                            PollResponseQueue();
                                        }
                                        return _saveFileTrackingFinished;
                                    }
                                    else
                                    {
                                        return 4;
                                    }
                                }
                                else
                                {
                                    return 3;
                                }
                            }
                            else
                            {
                                return 2;
                            }
                        }
                        catch (Exception ex)
                        {
                            Debug.WriteLine(ex.Message);
                            return 1;
                        }
                    }
                    break;
                default:
                    break;
            }
            return 0;
        }

        public readonly object _quickLock = new object();
        public int _quickWandGlyph = 0;
        public int _quickWandExceptionality = 0;
        public string _quickWandName = "";
        public int _quickSpellGlyph = 0;
        public int _quickSpellOtyp = 0;
        public string _quickSpellName = "";
        public void SetQuickZapWand(int glyph, int exceptionality, string name)
        {
            lock (_quickLock)
            {
                _quickWandGlyph = glyph;
                _quickWandExceptionality = exceptionality;
                _quickWandName = name;
            }
        }

        public void SetQuickCastSpell(int glyph, int otyp, string name)
        {
            lock (_quickLock)
            {
                _quickSpellGlyph = glyph;
                _quickSpellOtyp = otyp;
                _quickSpellName = name;
            }
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
        bool _checkPointRequested = false;

        private void RequestSaveGame()
        {
            if (PlayingReplay)
                return;
            _saveRequested = true;
        }

        private void RequestRestoreSavedGame()
        {
            if (PlayingReplay)
                return;
            _restoreRequested = true;
        }

        private void ForceExitThread()
        {
            if (PlayingReplay)
                return;
            /* The fact that this function is only called from GnhThread should ensure the thread is alive */
            GHApp.GnollHackService?.ExitGnhThread(exit_hack_types.EXITHACK_EXITTHREAD);
        }

        private void ForceTerminateGnollHack(exit_hack_types used_exit_hack_code)
        {
            if (PlayingReplay)
                return;
            /* The fact that this function is only called from GnhThread should ensure the thread is alive */
            GHApp.GnollHackService?.TerminateGnollHack(used_exit_hack_code);
        }

        private void RequestCheckPoint()
        {
            if (PlayingReplay)
                return;
            _checkPointRequested = true;
        }

        bool _timeTallyRequested = false;
        private void RequestTallyRealTime()
        {
            if (PlayingReplay)
                return;
            _timeTallyRequested = true;
        }
        
        private void EndReplayFile()
        {
            RecordFunctionCallImmediately(RecordedFunctionID.EndOfFile);
            _replayTimeStamp = DateTime.Now;
            _replayContinuation = 0;
        }

        private void ContinueToNextReplayFile()
        {
            string plName = GHApp.LastUsedPlayerName;
            DateTime saved_timestamp = _replayTimeStamp;
            int next_replayContinuation = _replayContinuation + 1;
            RecordFunctionCallImmediately(RecordedFunctionID.ContinueToNextFile, GHApp.GHVersionNumber, _replayTimeStamp.ToBinary(), plName, next_replayContinuation);
            _replayTimeStamp = saved_timestamp; /* Use existing time stamp, and new continuation number */
            _replayContinuation = next_replayContinuation;
        }

        private List<GHRecordedFunctionCall> _recordedFunctionCalls = new List<GHRecordedFunctionCall>();
        private void RecordFunctionCall(RecordedFunctionID functionID, params object[] args)
        {
            if (!GHApp.RecordGame || PlayingReplay)
                return;

            _recordedFunctionCalls.Add(new GHRecordedFunctionCall(functionID, args, DateTime.Now));
        }

        private void RecordFunctionCallImmediately(RecordedFunctionID functionID, params object[] args)
        {
            if (!GHApp.RecordGame || PlayingReplay)
                return;

            RecordFunctionCall(functionID, args);
            long len = WriteFunctionCallsToDisk();
            if(functionID != RecordedFunctionID.EndOfFile && functionID != RecordedFunctionID.ContinueToNextFile)
                CheckEndReplay(len);
        }

        private void CheckEndReplay(long currentLengthInBytes)
        {
            if (!GHApp.RecordGame || PlayingReplay || currentLengthInBytes <= 0)
                return;

            if (GHApp.PlatformService.GetDeviceFreeDiskSpaceInBytes() < GHConstants.CritiallyLowFreeDiskSpaceThresholdInBytes)
            {
                EndReplayFile();
                GHApp.RecordGame = false; /* Preferences relating to RecordGame will be set off on the game page in another thread just in case */
                RequestQueue.Enqueue(new GHRequest(this, GHRequestType.InformRecordingWentOff));
            }
            else if (currentLengthInBytes >= GHConstants.MaxSingleReplayFileSizeInBytes)
            {
                ContinueToNextReplayFile();
            }
        }

        private void WriteFunctionCallsAndCheckEnd()
        {
            CheckEndReplay(WriteFunctionCallsToDisk());            
        }

        private DateTime _replayTimeStamp = DateTime.Now;
        int _replayContinuation = 0;
        private long _headerSize = 0L;
        private long _noOfCommmands = 0L;
        private long[] _commandSize = new long[(int)RecordedFunctionID.NumberOfFunctionCalls];
        private string _knownPlayerName = null;
        private int _knownFirstTurn = -1;

        private long WriteFunctionCallsToDisk()
        {
            if (!GHApp.RecordGame || PlayingReplay)
                return 0L;

            long res = 0L;
            if (_recordedFunctionCalls.Count > 0)
            {
                try
                {
                    string dir = Path.Combine(GHApp.GHPath, GHConstants.ReplayDirectory);
                    if (!Directory.Exists(dir))
                        GHApp.CheckCreateDirectory(dir);
                    if (Directory.Exists(dir))
                    {
                        string configString = ""; /* Used to be player name, but in replays we do not in fact know it in advance before the replay starts */
                        long timeBinary = _replayTimeStamp.ToBinary();
                        string filepath = Path.Combine(dir, GHApp.GetReplayFileName(GHApp.GHVersionNumber, timeBinary, _replayContinuation, null, -1, true));
                        bool fileDidExist = File.Exists(filepath);
                        bool eofFound = false;
                        long appendSize = 0L;
                        long existingSize = 0L;
                        using (FileStream fileStream = new FileStream(filepath, fileDidExist ? FileMode.Append : FileMode.Create, FileAccess.Write, FileShare.None))
                        {
                            if (fileStream != null)
                            {
                                existingSize = fileStream.Length;
                                using (BinaryWriter writer = new BinaryWriter(fileStream))
                                {
                                    if(!fileDidExist)
                                    {
                                        /* Write header */
                                        writer.Write(GHApp.GHVersionNumber);
                                        writer.Write(GHApp.GHVersionCompatibility);
                                        writer.Write(timeBinary);
                                        writer.Write(configString);
                                        writer.Write(WizardMode);
                                        writer.Write(ModernMode);
                                        writer.Write(CasualMode);
                                        writer.Write(0); /* Recording type: 0 = full; 1 = ascii only */
                                        writer.Write((int)GHApp.PlatformId);
                                        ulong versionFlags = 0L;
                                        versionFlags |= (ulong)ReplayVersionFlags.HasOffsetData; /* This was missing from replays before 4.2.0 Build 55 */
                                        writer.Write(versionFlags);
                                        ulong flags2 = 0L;
                                        writer.Write(flags2); /* flags for future use */
                                        _headerSize += 3 * 8L + (long)configString.Length + 1L + 3 * 1L + 2 * 4L + 2 * 8L;
                                        appendSize += _headerSize;
                                    }
                                    foreach (GHRecordedFunctionCall rfc in _recordedFunctionCalls)
                                    {
                                        if (rfc == null)
                                            continue;

                                        _noOfCommmands++;

                                        long saveSize = 0L;
                                        if (rfc.RecordedFunctionID == RecordedFunctionID.EndOfFile || rfc.RecordedFunctionID == RecordedFunctionID.ContinueToNextFile)
                                            eofFound = true;
                                        writer.Write((byte)rfc.RecordedFunctionID);
                                        saveSize += 1L;
                                        if (GHApp.IsTimeStampedFunctionCall((byte)rfc.RecordedFunctionID))
                                        {
                                            writer.Write(rfc.Time.ToBinary());
                                            saveSize += 8L;
                                        }

                                        int noOfArgs = rfc.Args == null ? 0 : rfc.Args.Length;
                                        //writer.Write((byte)noOfArgs);
                                        for (int i = 0; i < noOfArgs; i++)
                                        {
                                            object o = rfc.Args[i];
                                            if (o == null)
                                            {
                                                writer.Write(0);
                                                saveSize += 4L;
                                            }
                                            else if (o is bool)
                                            {
                                                writer.Write((bool)o);
                                                saveSize += 1L;
                                            }
                                            else if (o is byte)
                                            {
                                                writer.Write((byte)o);
                                                saveSize += 1L;
                                            }
                                            else if (o is sbyte)
                                            {
                                                writer.Write((sbyte)o);
                                                saveSize += 1L;
                                            }
                                            else if (o is char)
                                            {
                                                writer.Write((char)o);
                                                saveSize += 2L;
                                            }
                                            else if (o is short)
                                            {
                                                writer.Write((short)o);
                                                saveSize += 2L;
                                            }
                                            else if (o is ushort)
                                            {
                                                writer.Write((ushort)o);
                                                saveSize += 2L;
                                            }
                                            else if (o is int)
                                            {
                                                writer.Write((int)o);
                                                saveSize += 4L;
                                            }
                                            else if (o is uint)
                                            {
                                                writer.Write((uint)o);
                                                saveSize += 4L;
                                            }
                                            else if (o is long)
                                            {
                                                writer.Write((long)o);
                                                saveSize += 8L;
                                            }
                                            else if (o is ulong)
                                            {
                                                writer.Write((ulong)o);
                                                saveSize += 8L;
                                            }
                                            else if (o is float)
                                            {
                                                writer.Write((float)o);
                                                saveSize += 4L;
                                            }
                                            else if (o is double)
                                            {
                                                writer.Write((double)o);
                                                saveSize += 8L;
                                            }
                                            else if (o is string)
                                            {
                                                int len = ((string)o).Length + 1;
                                                writer.Write(len);
                                                writer.Write((string)o);
                                                saveSize += 4L + (long)len;
                                            }
                                            else if (o is byte[])
                                            {
                                                int len = ((byte[])o).Length;
                                                writer.Write(len);
                                                writer.Write((byte[])o);
                                                saveSize += 4L + (long)len;
                                            }
                                            else if (o is short[])
                                            {
                                                short[] arr = (short[])o;
                                                writer.Write(arr.Length);
                                                for (int j = 0; j < arr.Length; j++)
                                                    writer.Write(arr[j]);
                                                saveSize += 4L + (long)arr.Length * 2L;
                                            }
                                            else if (o is int[])
                                            {
                                                int[] arr = (int[])o;
                                                writer.Write(arr.Length);
                                                for (int j = 0; j < arr.Length; j++)
                                                    writer.Write(arr[j]);
                                                saveSize += 4L + (long)arr.Length * 4L;
                                            }
                                            else if (o is uint[])
                                            {
                                                uint[] arr = (uint[])o;
                                                writer.Write(arr.Length);
                                                for (int j = 0; j < arr.Length; j++)
                                                    writer.Write(arr[j]);
                                                saveSize += 4L + (long)arr.Length * 4L;
                                            }
                                            else if (o is long[])
                                            {
                                                long[] arr = (long[])o;
                                                writer.Write(arr.Length);
                                                for (int j = 0; j < arr.Length; j++)
                                                    writer.Write(arr[j]);
                                                saveSize += 4L + (long)arr.Length * 8L;
                                            }
                                            else if (o is ulong[])
                                            {
                                                ulong[] arr = (ulong[])o;
                                                writer.Write(arr.Length);
                                                for (int j = 0; j < arr.Length; j++)
                                                    writer.Write(arr[j]);
                                                saveSize += 4L + (long)arr.Length * 8L;
                                            }
                                            else if (o is float[])
                                            {
                                                float[] arr = (float[])o;
                                                writer.Write(arr.Length);
                                                for (int j = 0; j < arr.Length; j++)
                                                    writer.Write(arr[j]);
                                                saveSize += 4L + (long)arr.Length * 4L;
                                            }
                                            else if (o is string[])
                                            {
                                                string[] arr = (string[])o;
                                                writer.Write(arr.Length);
                                                saveSize += 4L;
                                                for (int j = 0; j < arr.Length; j++)
                                                {
                                                    if(arr[j] == null)
                                                    {
                                                        writer.Write(0);
                                                        saveSize += 4L;
                                                    }
                                                    else
                                                    {
                                                        writer.Write(arr[j].Length + 1);
                                                        writer.Write(arr[j]);
                                                        saveSize += 4L + (long)arr[j].Length + 1L;
                                                    }
                                                }
                                            }
                                            else if (o.GetType().IsValueType) // Is struct
                                            {
                                                byte[] arr = null;
                                                IntPtr ptr = IntPtr.Zero;
                                                int size = 0;
                                                try
                                                {
                                                    size = Marshal.SizeOf(o);
                                                    arr = new byte[size];
                                                    ptr = Marshal.AllocHGlobal(size);
                                                    Marshal.StructureToPtr(o, ptr, true);
                                                    Marshal.Copy(ptr, arr, 0, size);
                                                }
                                                catch (Exception e)
                                                {
                                                    Debug.WriteLine(e.Message);
                                                }
                                                finally
                                                {
                                                    Marshal.FreeHGlobal(ptr);
                                                }
                                                writer.Write(size);
                                                writer.Write(arr);
                                                saveSize += 4L + (long)size;
                                            }
                                        }
                                        _commandSize[(int)rfc.RecordedFunctionID] += saveSize;
                                        appendSize += saveSize;
                                    }
                                }
                                _recordedFunctionCalls.Clear();
                                res = existingSize + appendSize;
                            }
                        } 
                        if(eofFound && File.Exists(filepath))
                        {
                            string newFilepath = Path.Combine(dir, GHApp.GetReplayFileName(GHApp.GHVersionNumber, timeBinary, _replayContinuation, _knownPlayerName, _knownFirstTurn, true));
                            string zipFile = newFilepath + (GHApp.UseGZipForReplays ? GHConstants.ReplayGZipFileNameSuffix : GHConstants.ReplayZipFileNameSuffix);
                            if(File.Exists(zipFile))
                                File.Delete(zipFile);
                            if (newFilepath != filepath)
                            {
                                if (File.Exists(newFilepath))
                                    File.Delete(newFilepath);
                                File.Move(filepath, newFilepath);
                            }

                            if (GHApp.UseGZipForReplays)
                            {
                                using (FileStream originalFileStream = File.Open(newFilepath, FileMode.Open))
                                {
                                    using (FileStream compressedFileStream = File.Create(zipFile))
                                    {
                                        using (var compressor = new GZipStream(compressedFileStream, CompressionMode.Compress))
                                        {
                                            originalFileStream.CopyTo(compressor);
                                        }
                                    }
                                }
                            }
                            else
                            {
                                using (ZipArchive archive = ZipFile.Open(zipFile, ZipArchiveMode.Create))
                                {
                                    archive.CreateEntryFromFile(newFilepath, Path.GetFileName(newFilepath));
                                }
                            }

                            if (File.Exists(newFilepath) && File.Exists(zipFile))
                                File.Delete(newFilepath);

                            GHApp.MaybeWriteGHLog("Replay Finalized: Existing: " + existingSize + " bytes; Appended: " + appendSize + " bytes; Total: " + (existingSize + appendSize).ToString());

                            long totalCommands = 0L;
                            for(RecordedFunctionID i = RecordedFunctionID.InitializeWindows; i < RecordedFunctionID.NumberOfFunctionCalls; i++)
                            {
                                totalCommands += _commandSize[(int)i];
                            }
                            GHApp.MaybeWriteGHLog("Header: " + _headerSize + " bytes; Commands (" + _noOfCommmands +"): " + totalCommands + " bytes; Total: " + (_headerSize + totalCommands));
                            for (RecordedFunctionID i = RecordedFunctionID.InitializeWindows; i < RecordedFunctionID.NumberOfFunctionCalls; i++)
                            {
                                GHApp.MaybeWriteGHLog("- " + i.ToString() + ": " + _commandSize[(int)i] + string.Format(" bytes ({0:P2})", ((double)_commandSize[(int)i]) / ((double)totalCommands)));
                                _commandSize[(int)i] = 0L;
                            }
                            _noOfCommmands = 0L;
                            _headerSize = 0L;
                            _replayTimeStamp = DateTime.Now; /* Will be overridden if continuing to next record file, but this is here just to make it point away from the finished file just in case */
                            _replayContinuation = 0;
                            res = -1; /* Indicating the file has been finalized and zipped */
                            if (GHApp.AutoUploadReplays)
                            {
                                RequestQueue.Enqueue(new GHRequest(this, GHRequestType.PostReplayFile, 0, 0, zipFile));
                            }
                        }
                    }
                }
                catch (Exception ex)
                {
                    Debug.WriteLine(ex.Message);
                }
            }
            return res;
        }

        private void WaitAndCheckPauseReplay(int baseDelay)
        {
            if (!PlayingReplay || GHApp.IsReplaySearching)
                return;

            if(!GHApp.StopReplay)
            {
                Thread.Sleep((int)(baseDelay / GHApp.ReplaySpeed));
                GHApp.FmodService?.PollTasks();
                do
                {
                    if (GHApp.StopReplay)
                        break;
                    else if (GHApp.PauseReplay && !GHApp.IsReplaySearching)
                    {
                        Thread.Sleep(GHConstants.PollingInterval);
                        GHApp.FmodService?.PollTasks();
                    }
                }
                while (GHApp.PauseReplay && !GHApp.IsReplaySearching);
            }
        }
    }

    public struct SavedPrintGlyphCall
    {
        public GHWindow ReferenceGHWindow;
        public int X;
        public int Y;
        public int Glyph;
        public int Bkglyph;
        public int Symbol;
        public int Ocolor;
        public uint Special;
        public LayerInfo Layers;

        public SavedPrintGlyphCall(GHWindow gHWindow, int x, int y, int glyph, int bkglyph, int symbol, int ocolor, uint special, ref LayerInfo layers)
        {
            ReferenceGHWindow = gHWindow;
            X = x;
            Y = y;
            Glyph = glyph;
            Bkglyph = bkglyph;
            Symbol = symbol;
            Ocolor = ocolor; 
            Special = special; 
            Layers = layers;
        }
    }

    public struct SavedSendObjectDataCall
    {
        public int x;
        public int y;
        public Obj otmp;
        public int cmdtype;
        public int where;
        public ObjClassData otypdata;
        public ulong oflags;

        public SavedSendObjectDataCall(int x, int y, ref Obj otmp, int cmdtype, int where, ref ObjClassData otypdata, ulong oflags)
        {
            this.x = x;
            this.y = y;
            this.cmdtype = cmdtype;
            this.where = where;
            this.otmp = otmp;
            this.otypdata = otypdata;
            this.oflags = oflags;
        }
    }

    public struct SavedSendMonsterDataCall
    {
        public int cmdtype;
        public int x;
        public int y;
        public monst_info monster_data;
        public ulong oflags;

        public SavedSendMonsterDataCall(int cmdtype, int x, int y, ref monst_info monster_data, ulong oflags)
        {
            this.cmdtype = cmdtype;
            this.x = x;
            this.y = y;
            this.monster_data = monster_data;
            this.oflags = oflags;
        }
    }

    public struct SavedSendEngravingDataCall
    {
        public int cmdtype;
        public int x;
        public int y;
        public string engraving_text;
        public int etype;
        public ulong eflags;
        public ulong gflags;

        public SavedSendEngravingDataCall(int cmdtype, int x, int y, string engraving_text, int etype, ulong eflags, ulong gflags)
        {
            this.cmdtype = cmdtype;
            this.x = x;
            this.y = y;
            this.engraving_text = engraving_text;
            this.etype = etype;
            this.eflags = eflags;
            this.gflags = gflags;
        }
    }

    public class ContextMenuButton
    {
        public readonly string LblText;
        public readonly string ImgSourcePath;
        public readonly SKImage Bitmap;
        public readonly int BtnCommand;
        public readonly string ShortcutText;
        
        //public SKRect Rect;
        public ContextMenuButton(string lblText, string imgSourcePath, SKImage bitmap, int btnCommand)
        {
            LblText = lblText;
            ImgSourcePath = imgSourcePath;
            Bitmap = bitmap;
            BtnCommand = btnCommand;
            ShortcutText = GHUtils.ConstructShortcutText(btnCommand);
        }
    }
}
