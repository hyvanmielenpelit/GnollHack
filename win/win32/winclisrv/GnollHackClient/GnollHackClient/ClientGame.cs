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

namespace GnollHackClient
{
    public class ClientGame
    {
        private static ConcurrentDictionary<ClientGame, ConcurrentQueue<GHRequest>> _concurrentRequestDictionary = new ConcurrentDictionary<ClientGame, ConcurrentQueue<GHRequest>>();
        private static ConcurrentDictionary<ClientGame, ConcurrentQueue<GHResponse>> _concurrentResponseDictionary = new ConcurrentDictionary<ClientGame, ConcurrentQueue<GHResponse>>();
        private int[] _inputBuffer = new int[GHConstants.InputBufferLength];
        private int _inputBufferLocation = -1;
        private string _getLineString = null;
        private string _characterName = "";
        private object _characterNameLock = new object();
        private GamePage _gamePage;
        private object _gamePageLock = new object();
        private int _lastWindowHandle = 0;
        private GHWindow[] _ghWindows = new GHWindow[GHConstants.MaxGHWindows];
        private object _ghWindowsLock = new object();
        public GHWindow[] Windows { get { return _ghWindows; } }
        public object WindowsLock { get { return _ghWindowsLock; } }
        public int MapWindowId { get; set; }
        private List<GHMsgHistoryItem> _message_history = new List<GHMsgHistoryItem>();

        public static ConcurrentDictionary<ClientGame, ConcurrentQueue<GHRequest>> RequestDictionary { get { return _concurrentRequestDictionary; } }
        public static ConcurrentDictionary<ClientGame, ConcurrentQueue<GHResponse>> ResponseDictionary { get { return _concurrentResponseDictionary; } }
        public string CharacterName {
            get { lock (_characterNameLock) { return _characterName; } } 
            set { lock (_characterNameLock) { _characterName = value; } }
        }

        public GamePage ClientGamePage
        {
            get { lock (_gamePageLock) { return _gamePage; } }
        }

        public ClientGame(GamePage gamePage)
        {
            ClientGame.RequestDictionary.TryAdd(this, new ConcurrentQueue<GHRequest>());
            ClientGame.ResponseDictionary.TryAdd(this, new ConcurrentQueue<GHResponse>());
            lock (_gamePageLock)
            {
                _gamePage = gamePage;
            }
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
            ConcurrentQueue<GHResponse> queue;
            GHResponse response;
            if(ClientGame.ResponseDictionary.TryGetValue(this, out queue))
            {
                if (queue.TryDequeue(out response))
                {
                    switch (response.RequestType)
                    {
                        case GHRequestType.AskName:
                            CharacterName = response.ResponseStringValue;
                            break;
                        case GHRequestType.GetChar:
                            _inputBufferLocation++;
                            if(_inputBufferLocation >= GHConstants.InputBufferLength)
                                _inputBufferLocation = GHConstants.InputBufferLength -1;
                            _inputBuffer[_inputBufferLocation] = response.ResponseIntValue;
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
                            }
                            else
                            {
                                //Throw an error or stop waiting
                            }
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
        }
        public int ClientCallback_CreateGHWindow(int wintype)
        {
            if (_lastWindowHandle >= GHConstants.MaxGHWindows) /* Should not happen, but paranoid */
                _lastWindowHandle = GHConstants.MaxGHWindows - 1;

            while (_lastWindowHandle > 0 && _ghWindows[_lastWindowHandle] == null)
                _lastWindowHandle--;

            if (_ghWindows[_lastWindowHandle] != null)
                _lastWindowHandle++;

            if (_lastWindowHandle >= GHConstants.MaxGHWindows)
                return 0;

            int handle = _lastWindowHandle;
            GHWindow ghwin = new GHWindow((GHWinType)wintype, ClientGamePage, handle);
            lock(_ghWindowsLock)
            {
                _ghWindows[handle] = ghwin;
                ghwin.Create();
                if (wintype == (int)GHWinType.Map)
                    MapWindowId = handle;
            }
            return handle;
        }
        public void ClientCallback_DestroyGHWindow(int winHandle)
        {
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
            lock (_ghWindowsLock)
            {
                if (_ghWindows[winHandle] != null)
                    _ghWindows[winHandle].Clear();
            }
        }
        public void ClientCallback_DisplayGHWindow(int winHandle, byte blocking)
        {
            bool ismenu = false;
            lock (_ghWindowsLock)
            {
                if (_ghWindows[winHandle] != null)
                {
                    _ghWindows[winHandle].Display(blocking != 0);
                    ismenu = (_ghWindows[winHandle].WindowType == GHWinType.Menu);
                }
            }

            if(blocking != 0 || ismenu)
            {
                int res = ClientCallback_nhgetch();
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
        public void ClientCallback_PrintGlyph(int winHandle, int x, int y, int glyph, int bkglyph, int symbol, int ocolor, uint special)
        {
            lock(_ghWindowsLock)
            {
                if (_ghWindows[winHandle] != null)
                    _ghWindows[winHandle].PrintGlyph(x, y, symbol, ocolor, special);
            }
        }

        public string ClientCallback_AskName()
        {
            Debug.WriteLine("ClientCallback_AskName");
            ConcurrentQueue<GHRequest> queue;
            if (ClientGame.RequestDictionary.TryGetValue(this, out queue))
            {
                queue.Enqueue(new GHRequest(this, GHRequestType.AskName));
                while (string.IsNullOrEmpty(CharacterName))
                {
                    Thread.Sleep(25);
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
            App.FmodService.StopTestSound();
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
                    res = _inputBuffer[_inputBufferLocation];
                    _inputBuffer[_inputBufferLocation] = 0;
                    _inputBufferLocation--;
                }
                return res;
            }
            else
                return 0;

        }
        public int ClientCallback_nh_poskey(ref int value1, ref int value2, ref int value3)
        {
            Debug.WriteLine("ClientCallback_nh_poskey");

            return ClientCallback_nhgetch();
        }
        public int ClientCallback_yn_question(string question, string responses, string def)
        {
            if(question != null)
                ClientCallback_RawPrint(question);

            ConcurrentQueue<GHRequest> queue;
            if (responses == null || responses == "")
            {
                if (ClientGame.RequestDictionary.TryGetValue(this, out queue))
                {
                    queue.Enqueue(new GHRequest(this, GHRequestType.ShowDirections));
                }
                int res = ClientCallback_nhgetch(); /* Get direction */
                if (ClientGame.RequestDictionary.TryGetValue(this, out queue))
                {
                    queue.Enqueue(new GHRequest(this, GHRequestType.HideDirections));
                }
                return res;
            }
            else
            {
                if (ClientGame.RequestDictionary.TryGetValue(this, out queue))
                {
                    queue.Enqueue(new GHRequest(this, GHRequestType.ShowYnResponses, question, responses));
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


        public void ClientCallback_Cliparound(int x, int y)
        {
            _gamePage.ClipX = x;
            _gamePage.ClipY = y;
        }

        public void ClientCallback_RawPrint(string str)
        {
            if (_message_history.Count > 0)
                _message_history[_message_history.Count - 1].IsLast = false;
            _message_history.Add(new GHMsgHistoryItem(str));
            if (_message_history.Count > GHConstants.MaxMessageHistoryLength)
                _message_history.RemoveAt(0);

            List<GHMsgHistoryItem> sendlist = new List<GHMsgHistoryItem>();
            sendlist.AddRange(_message_history);
            if(sendlist.Count > 0)
                sendlist[sendlist.Count - 1].IsLast = true;
            ConcurrentQueue<GHRequest> queue;
            if (ClientGame.RequestDictionary.TryGetValue(this, out queue))
            {
                queue.Enqueue(new GHRequest(this, GHRequestType.PrintHistory, sendlist));
            }
        }
        public void ClientCallback_RawPrintBold(string str)
        {
            if (_message_history.Count > 0)
                _message_history[_message_history.Count - 1].IsLast = false;
            _message_history.Add(new GHMsgHistoryItem(str, 1));
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
        public void ClientCallback_PutStrEx(int win_id, int attributes, string str, int append)
        {
            _ghWindows[win_id].PutStrEx(attributes, str, append);
        }
        public void ClientCallback_DelayOutput()
        {
            Thread.Sleep(2 * GHConstants.DefaultAnimationInterval);
        }
        public void ClientCallback_DelayOutputMilliseconds(int milliseconds)
        {
            Thread.Sleep(milliseconds);
        }
        public void ClientCallback_DelayOutputIntervals(int intervals)
        {
            Thread.Sleep(intervals * GHConstants.DefaultAnimationInterval); /* To be fixed */
        }
        public void ClientCallback_PreferenceUpdate(string str)
        {

        }
        public void ClientCallback_StatusInit()
        {

        }
        public void ClientCallback_StatusFinish()
        {

        }
        public void ClientCallback_StatusEnable(int value1, string value2, string value3, byte value4)
        {

        }
        public void ClientCallback_StatusUpdate(int value1, ref int value2, int value3, int value4, int value5, ref UInt32 value6)
        {

        }

        public void ClientCallback_StartMenu(int winid)
        {
            lock(_ghWindowsLock)
            {
                if (_ghWindows[winid] != null)
                {
                    _ghWindows[winid].MenuInfo = new GHMenuInfo();
                }
            }
        }
        public void ClientCallback_AddMenu(int winid, int glyph, Int64 identifier, char accel, char groupaccel, int attributes, string text, byte presel)
        {
            lock (_ghWindowsLock)
            {
                if (_ghWindows[winid] != null && _ghWindows[winid].MenuInfo != null)
                {
                    GHMenuItem mi = new GHMenuItem();
                    mi.Identifier = identifier;
                    mi.Accelerator = accel;
                    mi.GroupAccelerator = groupaccel;
                    mi.Attributes = attributes;
                    mi.Glyph = glyph;
                    mi.Text = text;
                    mi.Selected = (presel != 0);
                    mi.Count = mi.Selected ? -1 : 0;
                    _ghWindows[winid].MenuInfo.MenuItems.Add(mi);
                }
            }
        }

        public void ClientCallback_EndMenu(int winid, string prompt)
        {
            lock (_ghWindowsLock)
            {
                if (_ghWindows[winid] != null && _ghWindows[winid].MenuInfo != null)
                {
                    _ghWindows[winid].MenuInfo.Header = prompt;
                }
            }
        }
        public int ClientCallback_SelectMenu(int winid, int how, out IntPtr picklistptr, out int listsize)
        {
            Debug.WriteLine("ClientCallback_SelectMenu");
            ConcurrentQueue<GHRequest> queue;

            lock (_ghWindowsLock)
            {
                if (_ghWindows[winid] != null && _ghWindows[winid].MenuInfo != null)
                {
                    SelectionMode smode = (SelectionMode)how;
                    _ghWindows[winid].MenuInfo.SelectionHow = smode;

                    _ghWindows[winid].SelectedMenuItems = null; /* Clear menu response */
                    if (ClientGame.RequestDictionary.TryGetValue(this, out queue))
                    {
                        queue.Enqueue(new GHRequest(this, GHRequestType.ShowMenuPage, _ghWindows[winid], _ghWindows[winid].MenuInfo));
                    }
                }
            }

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

                Thread.Sleep(25);
                pollResponseQueue();
            }

            /* Handle result */
            int cnt = 0;
            Int64[] picklist = null;

            IntPtr arrayptr;

            lock (_ghWindowsLock)
            {
                if (_ghWindows[winid] == null || _ghWindows[winid].SelectedMenuItems == null)
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

        public string ClientCallback_getlin(string query)
        {
            Debug.WriteLine("ClientCallback_getlin");

            ConcurrentQueue<GHRequest> queue;
            if (ClientGame.RequestDictionary.TryGetValue(this, out queue))
            {
                _getLineString = null;
                queue.Enqueue(new GHRequest(this, GHRequestType.GetLine, query));
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
        public void ClientCallback_VoidIntIntPtrIntIntIntUlongPtrDummy(int value1, ref int value2, int value3, int value4, int value5, ref UInt32 value6)
        {
            return;
        }
        public void ClientCallback_VoidUlongDummy(UInt32 value1)
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
        public byte ClientCallback_BooleanDoubleDoubleDoubleDoubleDoubleDummy(double value1, double value2, double value3, double value4, double value5)
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
            return "message here";
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
    }
}
