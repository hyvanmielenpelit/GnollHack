using Android.App;
using Android.Content;
using Android.OS;
using Android.Runtime;
using Android.Views;
using Android.Widget;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using FMOD;
using System.Reflection;
using System.IO;
using Xamarin.Forms;

[assembly: Dependency(typeof(GnollHackClient.Droid.FmodService))]
namespace GnollHackClient.Droid
{
    class FmodService : IFmodService
    {
        private FMOD.Studio.System _system;
        private FMOD.System _coresystem;

        private byte[] _bankBuffer1;
        private byte[] _bankBuffer2;

        FMOD.Studio.Bank _bank1;
        FMOD.Studio.Bank _bank2;

        private void LoadNativeLibrary(string libName)
        {
            Java.Lang.JavaSystem.LoadLibrary(libName);
        }

        public void InitializeFmod()
        {
            LoadNativeLibrary("fmod");
            LoadNativeLibrary("fmodstudio");

            RESULT res;
            res = FMOD.Studio.System.create(out _system);
            res = _system.getCoreSystem(out _coresystem);
            res = _coresystem.setSoftwareFormat(0, SPEAKERMODE.DEFAULT, 0);
            res = _system.initialize(1024, FMOD.Studio.INITFLAGS.NORMAL, INITFLAGS.NORMAL, IntPtr.Zero);
        }

        public void LoadBanks()
        {
            //result = fmod_studio_system->loadBankMemory((const char*)data, (int)len, FMOD_STUDIO_LOAD_MEMORY, FMOD_STUDIO_LOAD_BANK_NORMAL, &bank[i]);
            Assembly assembly = typeof(GnollHackClient.IFmodService).Assembly;
            RESULT res;

            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.Master.bank"))
            {
                _bankBuffer1 = new byte[stream.Length];
                stream.Read(_bankBuffer1, 0, (int)stream.Length);
                res = _system.loadBankMemory(_bankBuffer1, FMOD.Studio.LOAD_BANK_FLAGS.NORMAL, out _bank1);
            }

            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.Master.strings.bank"))
            {
                _bankBuffer2 = new byte[stream.Length];
                stream.Read(_bankBuffer2, 0, (int)stream.Length);
                res = _system.loadBankMemory(_bankBuffer2, FMOD.Studio.LOAD_BANK_FLAGS.NORMAL, out _bank2);
            }
        }

        public void PlayTestSound()
        {
            string eventPath = "event:/Music/Start/Splash";
            FMOD.Studio.EventDescription eventDescription;
            RESULT res = _system.getEvent(eventPath, out eventDescription);
            FMOD.Studio.EventInstance eventInstance;
            res = eventDescription.createInstance(out eventInstance);
            res = eventInstance.setVolume(1.0f);
            res = eventInstance.start();
            res = _system.update();
        }
    }
}