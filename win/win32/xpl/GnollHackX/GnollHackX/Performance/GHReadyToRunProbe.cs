using System;
using System.IO;
#if GNH_MAUI
using System.Reflection.PortableExecutable;
#endif

namespace GnollHackX.Performance
{
    /* Whether an assembly file carries ReadyToRun native code. Must compile under C# 7.3. */
    public static class GHReadyToRunProbe
    {
        /* "Full" when the file's CLI header has a managed native header (a ReadyToRun image),
           "None" when it is IL only, and null when the path is empty, the file is missing or
           not a managed PE file, or on the legacy netstandard2.0 build. A partial ReadyToRun
           image also reads "Full", so build metadata takes precedence where it exists.
           Never throws. */
        public static string ModeOfFile(string path)
        {
#if GNH_MAUI
            if (string.IsNullOrEmpty(path))
                return null;
            try
            {
                if (!File.Exists(path))
                    return null;
                using (FileStream stream = new FileStream(path, FileMode.Open, FileAccess.Read, FileShare.ReadWrite | FileShare.Delete))
                using (PEReader reader = new PEReader(stream))
                {
                    CorHeader corHeader = reader.PEHeaders.CorHeader;
                    if (corHeader == null)
                        return null;
                    return corHeader.ManagedNativeHeaderDirectory.Size > 0 ? "Full" : "None";
                }
            }
            catch
            {
                return null;
            }
#else
            return null;
#endif
        }
    }
}
