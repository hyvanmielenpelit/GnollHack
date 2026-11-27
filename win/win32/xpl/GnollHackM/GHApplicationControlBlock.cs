using System;
using System.IO;

namespace GnollHackM
{
    /* Finds a load that Windows refused because of an application control policy: Smart App Control or
       App Control for Business (ERROR_SYSTEM_INTEGRITY_POLICY_VIOLATION), or AppLocker or Software
       Restriction Policies (ERROR_ACCESS_DISABLED_BY_POLICY). */
    public static class GHApplicationControlBlock
    {
        public const int ERROR_ACCESS_DISABLED_BY_POLICY = 1260;
        public const int ERROR_SYSTEM_INTEGRITY_POLICY_VIOLATION = 4551;
        private const int MaxDepth = 16;

        public static bool IsPolicyError(int win32Error)
        {
            return win32Error == ERROR_SYSTEM_INTEGRITY_POLICY_VIOLATION || win32Error == ERROR_ACCESS_DISABLED_BY_POLICY;
        }

        /* A FileLoadException's HResult is HRESULT_FROM_WIN32 of the load error, and FileName is the path or
           the assembly's display name */
        public static bool TryFind(Exception ex, out string fileName, out int win32Error)
        {
            fileName = null;
            win32Error = 0;
            int depth = 0;
            for (Exception e = ex; e != null && depth < MaxDepth; e = e.InnerException, depth++)
            {
                if (!(e is FileLoadException fle))
                    continue;
                int hr = fle.HResult;
                if ((hr & unchecked((int)0xFFFF0000)) != unchecked((int)0x80070000))
                    continue;
                int error = hr & 0xFFFF;
                if (!IsPolicyError(error))
                    continue;
                fileName = GetShortFileName(fle.FileName);
                win32Error = error;
                return true;
            }
            return false;
        }

        public static string GetShortFileName(string fileName)
        {
            if (string.IsNullOrWhiteSpace(fileName))
                return "unknown file";
            string name = fileName.Trim();
            if (name.IndexOf('\\') >= 0 || name.IndexOf('/') >= 0)
                return Path.GetFileName(name);
            int comma = name.IndexOf(',');
            if (comma > 0)
                return name.Substring(0, comma).Trim() + ".dll";
            return name;
        }
    }
}
