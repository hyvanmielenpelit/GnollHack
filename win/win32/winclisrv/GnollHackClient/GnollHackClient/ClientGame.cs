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

namespace GnollHackClient
{
    public class ClientGame
    {
        private static ConcurrentDictionary<ClientGame, ConcurrentQueue<GHRequest>> _concurrentRequestDictionary = new ConcurrentDictionary<ClientGame, ConcurrentQueue<GHRequest>>();
        private static ConcurrentDictionary<ClientGame, ConcurrentQueue<GHResponse>> _concurrentResponseDictionary = new ConcurrentDictionary<ClientGame, ConcurrentQueue<GHResponse>>();
        private int[] _inputBuffer = new int[GHConstants.InputBufferLength];
        private int _inputBufferLocation = -1;
        private string _characterName = "";
        private object _characterNameLock = new object();
        private GamePage _gamePage;
        private object _gamePageLock = new object();
        private GHWindow[] _ghWindows = new GHWindow[GHConstants.MaxGHWindows];
        private int _lastWindowHandle = 0;
        public GHWindow[] Windows { get { return _ghWindows; } }

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
            GHWindow ghwin = new GHWindow((GHWinType)wintype, ClientGamePage);
            _ghWindows[handle] = ghwin;
            ghwin.Create();
            return handle;
        }
        public void ClientCallback_DestroyGHWindow(int winHandle)
        {
            GHWindow ghwin = _ghWindows[winHandle];
            if(ghwin != null)
                ghwin.Destroy();
            _ghWindows[winHandle] = null;
        }
        public void ClientCallback_ClearGHWindow(int winHandle)
        {
            if(_ghWindows[winHandle] != null)
                _ghWindows[winHandle].Clear();
        }
        public void ClientCallback_DisplayGHWindow(int winHandle, byte blocking)
        {
            if (_ghWindows[winHandle] != null)
                _ghWindows[winHandle].Display(blocking != 0);
        }

        public int ClientCallback_PlayerSelection()
        {
            Debug.WriteLine("ClientCallback_PlayerSelection");
            return 0;
        }

        public void ClientCallback_Curs(int winHandle, int x, int y)
        {
            if (_ghWindows[winHandle] != null)
                _ghWindows[winHandle].Curs(x, y);
        }
        public void ClientCallback_PrintGlyph(int winHandle, int x, int y, int glyph, int bkglyph, int symbol, int ocolor, uint special)
        {
            if (_ghWindows[winHandle] != null)
                _ghWindows[winHandle].PrintGlyph(x, y, symbol, ocolor, special);
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

        public void ClientCallback_Cliparound(int x, int y)
        {
            _gamePage.ClipX = x;
            _gamePage.ClipY = y;
        }

        public void ClientCallback_RawPrint(string str)
        {
            _gamePage.Message = str;
        }
        public void ClientCallback_RawPrintBold(string str)
        {
            _gamePage.Message = str;
        }
        public void ClientCallback_PutStrEx(int win_id, int attributes, string str, int append)
        {
            _ghWindows[win_id].PutStrEx(attributes, str, append);
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
        public sbyte ClientCallback_CharConstCharPtrConstCharPtrCharDummy(string value1, string value2, sbyte value3)
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
