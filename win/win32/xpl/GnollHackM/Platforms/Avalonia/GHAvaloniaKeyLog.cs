#if GNH_AVALONIA
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.Text;
using Avalonia.Input;
using GnollHackX;

namespace GnollHackM
{
    /* Diagnostic key log, on when GNOLLHACK_KEY_LOG=1 at startup: KeyDown, KeyUp, TextInput and window
       activation, one line each, to standard error and the app log, plus one environment line. rpt is 1 when
       the key is already down by the log's own state; dt is milliseconds since the previous event of the same
       key. Off, every call costs one bool check. UI thread only. */
    internal static class GHAvaloniaKeyLog
    {
        private static readonly bool _enabled = string.Equals(Environment.GetEnvironmentVariable("GNOLLHACK_KEY_LOG"), "1", StringComparison.Ordinal);
        private static readonly Dictionary<int, long> _lastEventTicks = new Dictionary<int, long>();
        private static readonly HashSet<int> _keysDown = new HashSet<int>();
        private static bool _environmentLogged = false;

        public static bool IsEnabled { get { return _enabled; } }

        public static void Attach(Avalonia.Controls.Window window)
        {
            if (!_enabled || window == null)
                return;
            if (!_environmentLogged)
            {
                _environmentLogged = true;
                LogEnvironment();
            }
            window.Activated += OnActivated;
            window.Deactivated += OnDeactivated;
        }

        public static void Detach(Avalonia.Controls.Window window)
        {
            if (!_enabled || window == null)
                return;
            window.Activated -= OnActivated;
            window.Deactivated -= OnDeactivated;
        }

        /* bridge: the grip bridge's decision, or null */
        public static void LogKey(KeyEventArgs e, bool down, string bridge = null)
        {
            if (!_enabled || e == null)
                return;

            long now = Stopwatch.GetTimestamp();
            int keyId = e.PhysicalKey != PhysicalKey.None ? (int)e.PhysicalKey : 0x10000 + (int)e.Key;
            bool repeat = false;
            if (down)
                repeat = !_keysDown.Add(keyId);
            else
                _keysDown.Remove(keyId);
            string dt = "-";
            long last;
            if (_lastEventTicks.TryGetValue(keyId, out last))
                dt = ((now - last) * 1000.0 / Stopwatch.Frequency).ToString("F1", CultureInfo.InvariantCulture);
            _lastEventTicks[keyId] = now;

            StringBuilder sb = new StringBuilder(160);
            sb.Append("KEYLOG t=").Append(Milliseconds(now)).Append(down ? " KeyDown" : " KeyUp");
            sb.Append(" phys=").Append(e.PhysicalKey.ToString());
            sb.Append(" key=").Append(e.Key.ToString());
            sb.Append(" sym=").Append(Quote(e.KeySymbol));
            sb.Append(" mods=").Append(e.KeyModifiers.ToString());
            sb.Append(" dev=").Append(e.KeyDeviceType.ToString());
            sb.Append(" handled=").Append(e.Handled ? "True" : "False");
            sb.Append(" rpt=").Append(repeat ? "1" : "0");
            sb.Append(" dt=").Append(dt);
            sb.Append(" bridge=").Append(bridge ?? "");
            sb.Append(" src=").Append(SourceName(e.Source));
            Write(sb.ToString());
        }

        /* routed: the keystroke's KeyDown was already offered to the game */
        public static void LogText(TextInputEventArgs e, bool routed)
        {
            if (!_enabled || e == null)
                return;
            Write("KEYLOG t=" + Milliseconds(Stopwatch.GetTimestamp()) + " TextInput text=" + Quote(e.Text)
                + " handled=" + (e.Handled ? "True" : "False") + " routed=" + (routed ? "True" : "False")
                + " src=" + SourceName(e.Source));
        }

        /* The element the key event is raised on, i.e. the focused element */
        private static string SourceName(object source)
        {
            if (source == null)
                return "-";
            Avalonia.StyledElement styled = source as Avalonia.StyledElement;
            string name = styled != null ? styled.Name : null;
            return string.IsNullOrEmpty(name) ? source.GetType().Name : source.GetType().Name + "#" + name;
        }

        private static void OnActivated(object sender, EventArgs e)
        {
            Write("KEYLOG t=" + Milliseconds(Stopwatch.GetTimestamp()) + " Activated");
        }

        private static void OnDeactivated(object sender, EventArgs e)
        {
            /* Keys held at deactivation never report KeyUp to this window */
            _keysDown.Clear();
            Write("KEYLOG t=" + Milliseconds(Stopwatch.GetTimestamp()) + " Deactivated");
        }

        private static void LogEnvironment()
        {
            string[] names = { "SteamGameId", "SteamAppId", "SteamClientLaunch", "SteamDeck", "SteamGamepadUI",
                "XDG_SESSION_TYPE", "DISPLAY", "WAYLAND_DISPLAY" };
            StringBuilder sb = new StringBuilder("KEYLOG env");
            foreach (string name in names)
            {
                string value = Environment.GetEnvironmentVariable(name);
                sb.Append(' ').Append(name).Append('=').Append(value ?? "<unset>");
            }
            sb.Append(" IsSteamDeck=").Append(GHApp.IsSteamDeck ? "True" : "False");
            Write(sb.ToString());
        }

        private static string Milliseconds(long ticks)
        {
            return (ticks * 1000.0 / Stopwatch.Frequency).ToString("F1", CultureInfo.InvariantCulture);
        }

        private static string Quote(string text)
        {
            if (text == null)
                return "null";
            StringBuilder sb = new StringBuilder(text.Length + 2);
            sb.Append('"');
            foreach (char c in text)
            {
                if (c < 0x20 || c == 0x7f)
                    sb.Append("\\x").Append(((int)c).ToString("x2", CultureInfo.InvariantCulture));
                else if (c == '"' || c == '\\')
                    sb.Append('\\').Append(c);
                else
                    sb.Append(c);
            }
            sb.Append('"');
            return sb.ToString();
        }

        private static void Write(string line)
        {
            try
            {
                Console.Error.WriteLine(line);
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
            GHApp.WriteGHLog(line);
        }
    }
}
#endif
