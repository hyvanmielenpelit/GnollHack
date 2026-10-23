#if WINDOWS && GNH_MAUI
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Threading;
using GnollHackX;

namespace GnollHackM
{
    // WORKAROUND(WA-MAUI-0024): OnPlatformMessage costs an allocation for every window message on Windows
    /* Console display state, suspend/resume, session lock and main-window visibility, collected
       without window messages: the display state and suspend/resume through powrprof callbacks
       (DEVICE_NOTIFY_CALLBACK), which run on a system thread; the session lock by polling
       WTSQuerySessionInformation every minute, so a lock is seen up to a minute late and a shorter
       one can be missed; visibility from the window's VisibilityChanged event. Each transition writes
       a breadcrumb; ProcessMemoryWindows reads the current state for its breadcrumb, context and tags. */
    internal static class EnvironmentStateWindows
    {
        private const uint DEVICE_NOTIFY_CALLBACK = 2;
        private const uint PBT_APMSUSPEND = 0x0004;
        private const uint PBT_APMRESUMESUSPEND = 0x0007;
        private const uint PBT_APMRESUMEAUTOMATIC = 0x0012;
        private const uint PBT_POWERSETTINGCHANGE = 0x8013;
        private const int WTSSessionInfoEx = 25;
        private const int WTS_CURRENT_SESSION = -1;
        private const int WTS_SESSIONSTATE_LOCK = 0;
        private const int WTS_SESSIONSTATE_UNLOCK = 1;
        private const int SessionFlagsOffset = 16;  /* WTSINFOEXW: DWORD Level, then the 8-byte aligned WTSINFOEX_LEVEL1_W */
        private const int SessionPollMilliseconds = 60 * 1000;

        /* POWERBROADCAST_SETTING: GUID PowerSetting (16 bytes), DWORD DataLength, UCHAR Data[] */
        private const int PowerSettingDataLengthOffset = 16;
        private const int PowerSettingDataOffset = 20;

        private static readonly Guid ConsoleDisplayStateGuid = new Guid("6fe69556-704a-47a0-8f24-c28d936fda47");

        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        private delegate uint DeviceNotifyCallback(IntPtr context, uint type, IntPtr setting);

        /* DEVICE_NOTIFY_SUBSCRIBE_PARAMETERS */
        [StructLayout(LayoutKind.Sequential)]
        private struct DeviceNotifySubscribeParameters
        {
            public IntPtr Callback;
            public IntPtr Context;
        }

        [DllImport("powrprof.dll")]
        private static extern uint PowerSettingRegisterNotification(ref Guid settingGuid, uint flags, IntPtr recipient, out IntPtr registrationHandle);

        [DllImport("powrprof.dll")]
        private static extern uint PowerSettingUnregisterNotification(IntPtr registrationHandle);

        [DllImport("powrprof.dll")]
        private static extern uint PowerRegisterSuspendResumeNotification(uint flags, IntPtr recipient, out IntPtr registrationHandle);

        [DllImport("powrprof.dll")]
        private static extern uint PowerUnregisterSuspendResumeNotification(IntPtr registrationHandle);

        [DllImport("wtsapi32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool WTSQuerySessionInformationW(IntPtr server, int sessionId, int infoClass, out IntPtr buffer, out uint bytesReturned);

        [DllImport("wtsapi32.dll")]
        private static extern void WTSFreeMemory(IntPtr memory);

        /* Held for the process lifetime so that the callback is never collected while registered */
        private static readonly DeviceNotifyCallback _callback = OnPowerNotification;
        private static IntPtr _subscribeParameters = IntPtr.Zero;
        private static IntPtr _displayNotification = IntPtr.Zero;
        private static IntPtr _suspendNotification = IntPtr.Zero;
        private static Timer _sessionTimer;
        private static int _started;

        private static int _displayState = -1;      /* -1 unknown, 0 off, 1 on, 2 dimmed */
        private static int _sessionLocked;
        private static int _sessionKnown;
        private static int _windowVisible = 1;
        private static long _displayOffCount;
        private static long _sessionLockCount;
        private static long _windowHiddenCount;
        private static long _suspendCount;

        /* Raised on the power callback thread or the session poll thread, never the UI thread, after a
           change of display state or session lock state */
        public static event Action StateChanged;

        public static void Start(Microsoft.UI.Xaml.Window window)
        {
            try
            {
                if (Interlocked.Exchange(ref _started, 1) != 0)
                    return;
                if (window != null)
                    Interlocked.Exchange(ref _windowVisible, window.Visible ? 1 : 0);

                DeviceNotifySubscribeParameters parameters = new DeviceNotifySubscribeParameters();
                parameters.Callback = Marshal.GetFunctionPointerForDelegate(_callback);
                parameters.Context = IntPtr.Zero;
                _subscribeParameters = Marshal.AllocHGlobal(Marshal.SizeOf<DeviceNotifySubscribeParameters>());
                Marshal.StructureToPtr(parameters, _subscribeParameters, false);

                /* The current display state arrives through the callback right after registration */
                Guid guid = ConsoleDisplayStateGuid;
                IntPtr handle;
                uint result = PowerSettingRegisterNotification(ref guid, DEVICE_NOTIFY_CALLBACK, _subscribeParameters, out handle);
                if (result == 0)
                    _displayNotification = handle;
                else
                    WriteBreadcrumb("Environment state: display notification unavailable (error " + result + ")");
                result = PowerRegisterSuspendResumeNotification(DEVICE_NOTIFY_CALLBACK, _subscribeParameters, out handle);
                if (result == 0)
                    _suspendNotification = handle;
                else
                    WriteBreadcrumb("Environment state: suspend notification unavailable (error " + result + ")");

                PollSessionLock();
                _sessionTimer = new Timer(OnSessionTimer, null, SessionPollMilliseconds, SessionPollMilliseconds);
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
        }

        public static void Stop()
        {
            try
            {
                Timer timer = Interlocked.Exchange(ref _sessionTimer, null);
                if (timer != null)
                    timer.Dispose();
                IntPtr handle = Interlocked.Exchange(ref _displayNotification, IntPtr.Zero);
                if (handle != IntPtr.Zero)
                    PowerSettingUnregisterNotification(handle);
                handle = Interlocked.Exchange(ref _suspendNotification, IntPtr.Zero);
                if (handle != IntPtr.Zero)
                    PowerUnregisterSuspendResumeNotification(handle);
                IntPtr parameters = Interlocked.Exchange(ref _subscribeParameters, IntPtr.Zero);
                if (parameters != IntPtr.Zero)
                    Marshal.FreeHGlobal(parameters);
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
        }

        public static void OnVisibilityChanged(bool visible)
        {
            try
            {
                int newValue = visible ? 1 : 0;
                if (Interlocked.Exchange(ref _windowVisible, newValue) == newValue)
                    return;
                if (!visible)
                    Interlocked.Increment(ref _windowHiddenCount);
                WriteBreadcrumb(visible ? "Window visibility: visible" : "Window visibility: hidden");
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
        }

        /* System thread */
        private static uint OnPowerNotification(IntPtr context, uint type, IntPtr setting)
        {
            try
            {
                if (type == PBT_POWERSETTINGCHANGE)
                {
                    if (setting == IntPtr.Zero)
                        return 0;
                    Guid settingGuid = Marshal.PtrToStructure<Guid>(setting);
                    if (settingGuid != ConsoleDisplayStateGuid || Marshal.ReadInt32(setting, PowerSettingDataLengthOffset) < 1)
                        return 0;
                    int value = Marshal.ReadByte(setting, PowerSettingDataOffset);
                    if (value > 2)
                        return 0;
                    if (Interlocked.Exchange(ref _displayState, value) == value)
                        return 0;
                    if (value == 0)
                        Interlocked.Increment(ref _displayOffCount);
                    WriteBreadcrumb("Display state: " + DisplayStateName(value));
                    RaiseStateChanged();
                }
                else if (type == PBT_APMSUSPEND)
                {
                    Interlocked.Increment(ref _suspendCount);
                    WriteBreadcrumb("Power: suspend");
                }
                else if (type == PBT_APMRESUMESUSPEND || type == PBT_APMRESUMEAUTOMATIC)
                {
                    WriteBreadcrumb("Power: resume");
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
            return 0;
        }

        private static void OnSessionTimer(object state)
        {
            try
            {
                PollSessionLock();
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
        }

        private static void PollSessionLock()
        {
            IntPtr buffer;
            uint bytes;
            if (!WTSQuerySessionInformationW(IntPtr.Zero, WTS_CURRENT_SESSION, WTSSessionInfoEx, out buffer, out bytes))
                return;
            int flags;
            try
            {
                if (buffer == IntPtr.Zero || bytes < SessionFlagsOffset + 4)
                    return;
                flags = Marshal.ReadInt32(buffer, SessionFlagsOffset);
            }
            finally
            {
                if (buffer != IntPtr.Zero)
                    WTSFreeMemory(buffer);
            }
            if (flags != WTS_SESSIONSTATE_LOCK && flags != WTS_SESSIONSTATE_UNLOCK)
                return;
            int locked = flags == WTS_SESSIONSTATE_LOCK ? 1 : 0;
            bool firstRead = Interlocked.Exchange(ref _sessionKnown, 1) == 0;
            int previous = Interlocked.Exchange(ref _sessionLocked, locked);
            if (firstRead)
            {
                if (locked != 0)
                {
                    Interlocked.Increment(ref _sessionLockCount);
                    WriteBreadcrumb("Session: locked");
                    RaiseStateChanged();
                }
                return;
            }
            if (previous == locked)
                return;
            if (locked != 0)
                Interlocked.Increment(ref _sessionLockCount);
            WriteBreadcrumb(locked != 0 ? "Session: locked" : "Session: unlocked");
            RaiseStateChanged();
        }

        private static void RaiseStateChanged()
        {
            Action handler = StateChanged;
            if (handler == null)
                return;
            try
            {
                handler();
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
        }

        private static void WriteBreadcrumb(string text)
        {
            GHApp.AddSentryBreadcrumb(text, GHConstants.SentryGnollHackGeneralCategoryName);
            Debug.WriteLine(text);
        }

        private static string DisplayStateName(int value)
        {
            switch (value)
            {
                case 0:
                    return "off";
                case 1:
                    return "on";
                case 2:
                    return "dimmed";
                default:
                    return "unknown";
            }
        }

        public static string DisplayStateText { get { return DisplayStateName(Interlocked.CompareExchange(ref _displayState, 0, 0)); } }
        public static bool IsWindowVisible { get { return Interlocked.CompareExchange(ref _windowVisible, 0, 0) != 0; } }
        public static bool IsSessionLocked { get { return Interlocked.CompareExchange(ref _sessionLocked, 0, 0) != 0; } }
        public static bool IsSessionKnown { get { return Interlocked.CompareExchange(ref _sessionKnown, 0, 0) != 0; } }

        /* Display off, or the session known to be locked; a dimmed display still counts as seen */
        public static bool IsUnseen
        {
            get
            {
                return Interlocked.CompareExchange(ref _displayState, 0, 0) == 0 || (IsSessionKnown && IsSessionLocked);
            }
        }

        public static void AddToContext(Dictionary<string, object> context)
        {
            if (context == null)
                return;
            context["window_visible"] = IsWindowVisible;
            context["display_state"] = DisplayStateText;
            context["session_locked"] = IsSessionLocked;
            context["session_state_known"] = IsSessionKnown;
            context["display_off_count"] = Interlocked.Read(ref _displayOffCount);
            context["session_lock_count"] = Interlocked.Read(ref _sessionLockCount);
            context["window_hidden_count"] = Interlocked.Read(ref _windowHiddenCount);
            context["suspend_count"] = Interlocked.Read(ref _suspendCount);
        }
    }
}
#endif
