using System;
using System.IO;
using System.Text;
using Microsoft.Maui.Devices;

namespace GnollHackM
{
    /* IDeviceInfo for the Avalonia desktop target. The platform is "Linux" on Linux builds and
       "WindowsAvalonia" otherwise, so that GHApp.IsWindows stays false for the Avalonia Win32 build.
       On Linux, model and manufacturer come from DMI (a Steam Deck reports Jupiter or Galileo, and
       Valve) and the version from /etc/os-release. */
    public sealed class GHAvaloniaDeviceInfo : IDeviceInfo
    {
        private readonly string _model;
        private readonly string _manufacturer;
        private readonly string _name;
        private readonly string _versionString;
        private readonly Version _version;
        private readonly DevicePlatform _platform;

        public GHAvaloniaDeviceInfo()
        {
            _name = GetMachineName();
#if LINUX
            _platform = DevicePlatform.Create("Linux");
            _model = ReadFirstLine("/sys/class/dmi/id/product_name") ?? _name;
            _manufacturer = ReadFirstLine("/sys/class/dmi/id/sys_vendor") ?? "Unknown";
            _versionString = ReadOsReleaseVersionId() ?? Environment.OSVersion.Version.ToString();
#else
            _platform = DevicePlatform.Create("WindowsAvalonia");
            _model = _name;
            _manufacturer = "Unknown";
            _versionString = Environment.OSVersion.Version.ToString();
#endif
            _version = ParseVersion(_versionString);
        }

        public string Model => _model;

        public string Manufacturer => _manufacturer;

        public string Name => _name;

        public string VersionString => _versionString;

        public Version Version => _version;

        public DevicePlatform Platform => _platform;

        public DeviceIdiom Idiom => DeviceIdiom.Desktop;

        public DeviceType DeviceType => DeviceType.Physical;

        private static string GetMachineName()
        {
            try
            {
                string name = Environment.MachineName;
                return string.IsNullOrWhiteSpace(name) ? "Unknown" : name;
            }
            catch (InvalidOperationException)
            {
                return "Unknown";
            }
        }

        /* Trimmed first line of a text file, or null when it is missing, unreadable or empty */
        private static string ReadFirstLine(string path)
        {
            try
            {
                if (!File.Exists(path))
                    return null;
                using (StreamReader reader = new StreamReader(path))
                {
                    string line = reader.ReadLine();
                    if (line == null)
                        return null;
                    line = line.Trim();
                    return line.Length == 0 ? null : line;
                }
            }
            catch (Exception)
            {
                return null;
            }
        }

        /* VERSION_ID from os-release, without quotes, or null when absent (rolling releases have none) */
        private static string ReadOsReleaseVersionId()
        {
            string[] candidates = { "/etc/os-release", "/usr/lib/os-release" };
            foreach (string path in candidates)
            {
                try
                {
                    if (!File.Exists(path))
                        continue;
                    foreach (string rawLine in File.ReadAllLines(path))
                    {
                        string line = rawLine.Trim();
                        if (!line.StartsWith("VERSION_ID=", StringComparison.Ordinal))
                            continue;
                        string value = line.Substring("VERSION_ID=".Length).Trim().Trim('"', '\'').Trim();
                        return value.Length == 0 ? null : value;
                    }
                    return null;
                }
                catch (Exception)
                {
                    continue;
                }
            }
            return null;
        }

        /* The leading digits-and-dots part of a version string; a lone major number gets ".0".
           Unparsable strings give 0.0. */
        private static Version ParseVersion(string versionString)
        {
            if (string.IsNullOrWhiteSpace(versionString))
                return new Version(0, 0);

            StringBuilder sb = new StringBuilder();
            foreach (char c in versionString.Trim())
            {
                if ((c >= '0' && c <= '9') || c == '.')
                    sb.Append(c);
                else
                    break;
            }

            string numeric = sb.ToString().Trim('.');
            if (numeric.Length == 0)
                return new Version(0, 0);
            if (numeric.IndexOf('.') < 0)
                numeric += ".0";

            Version parsed;
            if (Version.TryParse(numeric, out parsed))
                return parsed;
            return new Version(0, 0);
        }
    }
}
