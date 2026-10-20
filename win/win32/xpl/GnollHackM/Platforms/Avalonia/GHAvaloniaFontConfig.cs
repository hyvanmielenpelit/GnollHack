#if GNH_AVALONIA && LINUX
using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Security;
using System.Text;

namespace GnollHackM
{
    /* The fonts shipped in the app's fonts folder (an emoji font for web pages) added to fontconfig for this process
       and the CEF processes it starts. The configuration includes the system's own first, so system fonts stay. */
    internal static class GHAvaloniaFontConfig
    {
        private const string SystemConfigFile = "/etc/fonts/fonts.conf";

        [DllImport("libc", SetLastError = true)]
        private static extern int setenv(string name, string value, int overwrite);

        /* In Main, before other threads start: setenv is not safe while another thread reads the environment */
        public static void Install()
        {
            try
            {
                string fontDirectory = Path.Combine(AppContext.BaseDirectory, "fonts");
                if (!Directory.Exists(fontDirectory))
                    return;

                string baseConfig = Environment.GetEnvironmentVariable("FONTCONFIG_FILE");
                if (string.IsNullOrEmpty(baseConfig) || !File.Exists(baseConfig))
                    baseConfig = SystemConfigFile;

                if (!string.IsNullOrEmpty(Environment.GetEnvironmentVariable("APPIMAGE")))
                    fontDirectory = CopyToDataDirectory(fontDirectory);

                string configDirectory = Path.Combine(GHAvaloniaPaths.DataDirectory, "fontconfig");
                Directory.CreateDirectory(configDirectory);
                string configFile = Path.Combine(configDirectory, "fonts.conf");

                /* Rewritten on each start: the app folder may move */
                StringBuilder config = new StringBuilder();
                config.Append("<?xml version=\"1.0\"?>\n");
                config.Append("<!DOCTYPE fontconfig SYSTEM \"fonts.dtd\">\n");
                config.Append("<fontconfig>\n");
                config.Append("  <include ignore_missing=\"yes\">").Append(SecurityElement.Escape(baseConfig)).Append("</include>\n");
                config.Append("  <dir>").Append(SecurityElement.Escape(fontDirectory)).Append("</dir>\n");
                config.Append("  <cachedir>").Append(SecurityElement.Escape(Path.Combine(configDirectory, "cache"))).Append("</cachedir>\n");
                config.Append("</fontconfig>\n");
                File.WriteAllText(configFile, config.ToString(), new UTF8Encoding(false));

                /* Natively, so that child processes inherit it; on Unix .NET changes only its own copy */
                if (setenv("FONTCONFIG_FILE", configFile, 1) != 0)
                {
                    Console.Error.WriteLine("GHAvaloniaFontConfig: setenv failed with error " + Marshal.GetLastPInvokeError());
                    return;
                }
                Environment.SetEnvironmentVariable("FONTCONFIG_FILE", configFile);
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine("GHAvaloniaFontConfig: " + ex.Message);
            }
        }

        /* An AppImage is mounted at a new path on every launch, and fontconfig keys its cache by folder path, so the
           fonts are used from a copy in the data directory. Returns the bundled folder if the copy fails. */
        private static string CopyToDataDirectory(string fontDirectory)
        {
            try
            {
                string targetDirectory = Path.Combine(GHAvaloniaPaths.DataDirectory, "fonts");
                Directory.CreateDirectory(targetDirectory);
                foreach (string source in Directory.GetFiles(fontDirectory))
                {
                    FileInfo sourceInfo = new FileInfo(source);
                    FileInfo targetInfo = new FileInfo(Path.Combine(targetDirectory, sourceInfo.Name));
                    if (!targetInfo.Exists || targetInfo.Length != sourceInfo.Length
                        || targetInfo.LastWriteTimeUtc != sourceInfo.LastWriteTimeUtc)
                    {
                        File.Copy(sourceInfo.FullName, targetInfo.FullName, true);
                        File.SetLastWriteTimeUtc(targetInfo.FullName, sourceInfo.LastWriteTimeUtc);
                    }
                }
                return targetDirectory;
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine("GHAvaloniaFontConfig: cannot copy the fonts: " + ex.Message);
                return fontDirectory;
            }
        }
    }
}
#endif
