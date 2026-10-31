#if GNH_AVALONIA
using System;
using System.Diagnostics;
using System.Globalization;
using Avalonia.Input;
using Avalonia.Interactivity;
using GnollHackX;

namespace GnollHackM
{
    /* Gamepad glue for the Avalonia build. At the first frame at least a second after the window is attached
       it registers the backends with GHGamepadService: evdev on Linux, XInput on Windows, and the back-grip key
       bridge when GNOLLHACK_GAMEPAD_KEYS allows it. OnFrame runs first in every frame callback and polls the
       service with immediate canvas paints deferred until after the poll. Window activation goes to the
       service, and pointer movement or presses clear its gamepad-active state. A frame that throws is logged at
       most once per 10 s, and 30 such frames in a row stop the backends for the session. UI thread only. */
    internal static class GHAvaloniaGamepad
    {
        private const double StartDelaySeconds = 1.0;
        private const double ErrorLogIntervalSeconds = 10.0;
        private const int MaxConsecutiveFailures = 30;
        private const double PointerMoveThreshold = 4.0;

        private static Avalonia.Controls.Window _window = null;
        private static long _attachTicks = 0;
        private static bool _started = false;
        private static bool _stoppedForSession = false;
        private static int _consecutiveFailures = 0;
        private static long _lastErrorLogTicks = 0;
        private static bool _errorLogged = false;
        private static GHGamepadKeyBridge _keyBridge = null;
        private static Avalonia.Point _pointerBase;
        private static bool _hasPointerBase = false;

        /* The grip bridge's decision for the last key passed to TryHandleKey, for the key log */
        public static string LastKeyDecision { get; private set; }

        public static void Attach(Avalonia.Controls.Window window)
        {
            if (window == null)
                return;
            _window = window;
            _attachTicks = Stopwatch.GetTimestamp();
            window.Activated += OnActivated;
            window.Deactivated += OnDeactivated;
            window.AddHandler(InputElement.PointerMovedEvent, OnPointerMoved, RoutingStrategies.Tunnel, true);
            window.AddHandler(InputElement.PointerPressedEvent, OnPointerPressed, RoutingStrategies.Tunnel, true);
        }

        public static void Detach(Avalonia.Controls.Window window)
        {
            if (window == null || !ReferenceEquals(_window, window))
                return;
            window.Activated -= OnActivated;
            window.Deactivated -= OnDeactivated;
            window.RemoveHandler(InputElement.PointerMovedEvent, OnPointerMoved);
            window.RemoveHandler(InputElement.PointerPressedEvent, OnPointerPressed);
            StopBackends();
            _window = null;
        }

        public static void OnFrame(long nowTicks)
        {
            if (_window == null)
                return;
            if (_stoppedForSession)
            {
                /* Without backends Poll only runs the game page's frame tick */
                try
                {
                    GHGamepadService.Poll(nowTicks);
                }
                catch (Exception ex)
                {
                    Debug.WriteLine(ex.Message);
                }
                return;
            }

            try
            {
                if (!_started && nowTicks - _attachTicks >= (long)(StartDelaySeconds * Stopwatch.Frequency))
                {
                    _started = true;
                    StartBackends();
                }

                GHAvaloniaCanvasControl.DeferImmediatePaint = true;
                try
                {
                    GHGamepadService.Poll(nowTicks);
                }
                finally
                {
                    GHAvaloniaCanvasControl.DeferImmediatePaint = false;
                    GHAvaloniaCanvasControl.FlushDeferredPaints();
                }
                _consecutiveFailures = 0;
            }
            catch (Exception ex)
            {
                _consecutiveFailures++;
                if (!_errorLogged || nowTicks - _lastErrorLogTicks >= (long)(ErrorLogIntervalSeconds * Stopwatch.Frequency))
                {
                    _errorLogged = true;
                    _lastErrorLogTicks = nowTicks;
                    WriteLog("GAMEPAD frame failed: " + ex.GetType().Name + ": " + ex.Message);
                }
                if (_consecutiveFailures >= MaxConsecutiveFailures)
                {
                    _stoppedForSession = true;
                    StopBackends();
                    WriteLog("GAMEPAD stopped for this session after " + MaxConsecutiveFailures.ToString(CultureInfo.InvariantCulture)
                        + " consecutive failed frames");
                }
            }
        }

        // WORKAROUND(WA-STEAM-0001): Steam Input's virtual gamepad has no back grips
        /* A back-grip key goes to the key bridge and is marked handled, which also suppresses its TextInput.
           False without the bridge, for keys with modifiers (Steam sends none), while Use Controller is off
           (the keys pass through) and for keys the bridge does not own. */
        public static bool TryHandleKey(KeyEventArgs e, bool down)
        {
            LastKeyDecision = null;
            if (e == null || _keyBridge == null)
                return false;
            if (e.KeyModifiers != KeyModifiers.None || e.PhysicalKey == PhysicalKey.None || !_keyBridge.Owns((int)e.PhysicalKey))
            {
                LastKeyDecision = "none";
                return false;
            }
            if (!GHGamepadService.IsEnabled)
            {
                LastKeyDecision = "passthrough";
                return false;
            }
            GHGamepadKeyBridgeResult result = _keyBridge.OnKey((int)e.PhysicalKey, down, Stopwatch.GetTimestamp());
            LastKeyDecision = result.ToString();
            e.Handled = true;
            return true;
        }

        /* A key TryHandleKey would take */
        public static bool IsBridgeKey(KeyEventArgs e)
        {
            return e != null && _keyBridge != null && GHGamepadService.IsEnabled && e.KeyModifiers == KeyModifiers.None
                && e.PhysicalKey != PhysicalKey.None && _keyBridge.Owns((int)e.PhysicalKey);
        }

        private static void StartBackends()
        {
#if LINUX
            if (OperatingSystem.IsLinux())
            {
                string error;
                if (!GHGamepadService.RegisterBackend(new GHEvdevGamepadBackend(), out error))
                    WriteLog("GAMEPAD evdev backend not started: " + error);
            }
#else
            if (OperatingSystem.IsWindows())
            {
                string error;
                if (!GHGamepadService.RegisterBackend(new GHXInputGamepadBackend(), out error))
                    WriteLog("GAMEPAD XInput backend not started: " + error);
            }
#endif
            StartKeyBridge();
        }

        /* GNOLLHACK_GAMEPAD_KEYS: on, off, or auto (default), which means a Steam launch on a Steam Deck */
        private static void StartKeyBridge()
        {
            string setting = Environment.GetEnvironmentVariable("GNOLLHACK_GAMEPAD_KEYS");
            setting = string.IsNullOrWhiteSpace(setting) ? "auto" : setting.Trim().ToLowerInvariant();
            bool eligible;
            if (setting == "on")
            {
                eligible = true;
            }
            else if (setting == "off")
            {
                eligible = false;
            }
            else
            {
                GHGamepadLaunchEnvironment env = GHGamepadLaunchEnvironment.Create(Environment.GetEnvironmentVariable, null, GHApp.IsSteamDeck);
                eligible = env.IsSteamDeck && env.SteamLaunch;
            }
            if (!eligible)
            {
                WriteLog("GAMEPAD keys bridge off (GNOLLHACK_GAMEPAD_KEYS=" + setting + ")");
                return;
            }

            int count = GHGamepadKeyMap.Primary.Length;
            int[] codes = new int[count];
            GHGamepadButton[] buttons = new GHGamepadButton[count];
            int i;
            for (i = 0; i < count; i++)
            {
                PhysicalKey key;
                if (!Enum.TryParse(GHGamepadKeyMap.Primary[i].KeyName, out key))
                {
                    WriteLog("GAMEPAD keys bridge off: unknown key " + GHGamepadKeyMap.Primary[i].KeyName);
                    return;
                }
                codes[i] = (int)key;
                buttons[i] = GHGamepadKeyMap.Primary[i].Button;
            }

            GHGamepadKeyBridge bridge = new GHGamepadKeyBridge(codes, buttons);
            string error;
            if (!GHGamepadService.RegisterBackend(bridge, out error))
            {
                WriteLog("GAMEPAD keys bridge not started: " + error);
                return;
            }
            _keyBridge = bridge;
            WriteLog("GAMEPAD keys bridge on (GNOLLHACK_GAMEPAD_KEYS=" + setting + "): L4 "
                + GHGamepadKeyMap.Primary[0].KeyName + ", L5 " + GHGamepadKeyMap.Primary[1].KeyName
                + ", R4 " + GHGamepadKeyMap.Primary[2].KeyName + ", R5 " + GHGamepadKeyMap.Primary[3].KeyName);
        }

        private static void StopBackends()
        {
            _keyBridge = null;
            try
            {
                GHGamepadService.StopAllBackends();
            }
            catch (Exception ex)
            {
                WriteLog("GAMEPAD stopping the backends failed: " + ex.Message);
            }
        }

        private static void OnActivated(object sender, EventArgs e)
        {
            GHGamepadService.SetWindowActive(true);
        }

        private static void OnDeactivated(object sender, EventArgs e)
        {
            GHGamepadService.SetWindowActive(false);
        }

        /* While the gamepad is active, a move of more than PointerMoveThreshold from the last position the
           pointer had in mouse use switches back to the mouse */
        private static void OnPointerMoved(object sender, Avalonia.Input.PointerEventArgs e)
        {
            if (_window == null)
                return;
            Avalonia.Point position = e.GetPosition(_window);
            if (!GHGamepadService.IsActive || !_hasPointerBase)
            {
                _pointerBase = position;
                _hasPointerBase = true;
                return;
            }
            double dx = position.X - _pointerBase.X;
            double dy = position.Y - _pointerBase.Y;
            if (dx * dx + dy * dy > PointerMoveThreshold * PointerMoveThreshold)
            {
                _pointerBase = position;
                GHGamepadService.NoteNonGamepadInput();
            }
        }

        private static void OnPointerPressed(object sender, PointerPressedEventArgs e)
        {
            if (GHGamepadService.IsActive)
                GHGamepadService.NoteNonGamepadInput();
        }

        private static void WriteLog(string line)
        {
            try
            {
                Console.Error.WriteLine(line);
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
            GHApp.MaybeWriteGHLog(line);
        }
    }
}
#endif
