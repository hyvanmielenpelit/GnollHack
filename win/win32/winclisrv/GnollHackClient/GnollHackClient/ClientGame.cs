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
        private static ConcurrentQueue<GHRequest> _concurrentRequestQueue = new ConcurrentQueue<GHRequest>();
        private static ConcurrentQueue<GHResponse> _concurrentResponseQueue = new ConcurrentQueue<GHResponse>();
        private string _inputBuffer = "";
        private string _characterName = "";
        private object _characterNameLock = new object();

        public static ConcurrentQueue<GHRequest> RequestQueue { get { return _concurrentRequestQueue; } }
        public static ConcurrentQueue<GHResponse> ResponseQueue { get { return _concurrentResponseQueue; } }
        public string CharacterName { //get; set; }
            get { lock (_characterNameLock) { return _characterName; } } 
            set { lock (_characterNameLock) { _characterName = value; } }
        }

        public ClientGame()
        {

        }

        private void pollResponseQueue()
        {
            GHResponse response;
            if (ClientGame.ResponseQueue.TryDequeue(out response))
            {
                if (response.RequestingClientGame == this)
                {
                    switch (response.RequestType)
                    {
                        case GHRequestType.AskName:
                            CharacterName = response.ResponseStringValue;
                            break;
                        default:
                            break;
                    }
                }
            }
        }

        public void AddInput(int type, float x, float y)
        {
            _inputBuffer = _inputBuffer + " ";    
        }



        public void ClientCallback_InitWindows()
        {
            Debug.WriteLine("ClientCallback_InitWindows");
        }
        public int ClientCallback_PlayerSelection()
        {
            Debug.WriteLine("ClientCallback_PlayerSelection");
            Thread.Sleep(3000);
            return 0;
        }


        public string ClientCallback_AskName()
        {
            Debug.WriteLine("ClientCallback_AskName");
            ClientGame.RequestQueue.Enqueue(new GHRequest(this, GHRequestType.AskName));
            while(string.IsNullOrEmpty(CharacterName))
            {
                Thread.Sleep(25);
                pollResponseQueue();
            }
            return CharacterName;
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
            while(_inputBuffer == "")
            {
                Thread.Sleep(25);
            }
            string value = _inputBuffer.Substring(0, 1);
            _inputBuffer = _inputBuffer.Substring(1, _inputBuffer.Length - 1);
            return value.ToCharArray()[0];
        }
        public int ClientCallback_nh_poskey(ref int value1, ref int value2, ref int value3)
        {
            Debug.WriteLine("ClientCallback_nh_poskey");
            while (_inputBuffer == "")
            {
                Thread.Sleep(25);
            }
            string value = _inputBuffer.Substring(0, 1);
            _inputBuffer = _inputBuffer.Substring(1, _inputBuffer.Length - 1);
            return value.ToCharArray()[0];
        }

        /*
        typedef void (__stdcall* VoidVoidCallback) ();
        typedef void (__stdcall* VoidCharCallback) (char*);
        typedef void (__stdcall* VoidConstCharCallback) (const char*);
        typedef int (__stdcall* IntIntCallback) (int);
        typedef void (__stdcall* VoidIntCallback) (int);
        typedef void (__stdcall* VoidIntIntCallback) (int, int);
        typedef void (__stdcall* VoidIntIntIntCallback) (int, int, int);
        typedef void (__stdcall* VoidIntBooleanCallback) (int, unsigned char);
        typedef void (__stdcall* VoidIntIntConstCharCallback) (int, const char*);
        typedef void (__stdcall* VoidConstCharIntCallback) (const char*, int);
        typedef void (__stdcall* VoidConstCharBooleanCallback) (const char*, unsigned char);
        typedef int (__stdcall* IntVoidCallback) ();
        typedef int (__stdcall* BooleanVoidCallback) ();
         */
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
