#if WINDOWS || GNH_AVALONIA
using System;
using System.IO;
using GnollHackX;

namespace GnollHackM
{
    /* The controller's page script for in-app browsers, read from the manifest resource on first use */
    internal static class GHWebPageScript
    {
        private const string ResourceName = "GnollHackM.GamepadWeb.js";

        private static string _script;
        private static bool _loaded;

        /* The script; null when it cannot be read */
        public static string Load()
        {
            if (_loaded)
                return _script;
            _loaded = true;
            try
            {
                System.Reflection.Assembly assembly = typeof(GHWebPageScript).Assembly;
                using (Stream stream = assembly.GetManifestResourceStream(ResourceName))
                {
                    if (stream == null)
                    {
                        GHApp.MaybeWriteGHLog("GHWebPageScript: page script resource " + ResourceName + " not found");
                        return null;
                    }
                    using (StreamReader reader = new StreamReader(stream))
                    {
                        _script = reader.ReadToEnd();
                    }
                }
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHWebPageScript: cannot read the page script: " + ex.Message);
                _script = null;
            }
            return _script;
        }
    }
}
#endif
