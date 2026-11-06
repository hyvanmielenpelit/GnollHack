using System;
using System.Diagnostics;
using System.Globalization;
using System.IO;
using Microsoft.Maui.Devices;
#if MACOS || WINDOWS_AVALONIA
using System.Runtime.InteropServices;
#endif
#if MACOS
using System.Text;
#endif

using AvDispatcher = Avalonia.Threading.Dispatcher;

namespace GnollHackM
{
    /* IBattery for the Avalonia desktop target. On Linux it polls the system battery in
       /sys/class/power_supply, on macOS the internal battery through IOKit's power sources, on Windows
       GetSystemPowerStatus, every 60 s, and raises BatteryInfoChanged on the Avalonia UI thread when the
       level, state or power source changes. Without a battery it reports a full charge on AC with State
       NotPresent, which keeps GHApp's battery-dependent saving enabled. Energy saver follows Windows's
       battery saver, raising EnergySaverStatusChanged on the Avalonia UI thread; elsewhere it is always Off. */
    public sealed class GHAvaloniaBattery : IBattery
    {
        private readonly object _lock = new object();
        private double _chargeLevel = 1.0;
        private BatteryState _state = BatteryState.NotPresent;
        private BatteryPowerSource _powerSource = BatteryPowerSource.AC;
        private EnergySaverStatus _energySaverStatus = EnergySaverStatus.Off;

#if LINUX || MACOS || WINDOWS_AVALONIA
        private const int PollIntervalMilliseconds = 60000;
        private readonly System.Threading.Timer _pollTimer = null;
#endif
#if LINUX
        private const string PowerSupplyDirectory = "/sys/class/power_supply";
#endif

        public GHAvaloniaBattery()
        {
#if LINUX || MACOS || WINDOWS_AVALONIA
            double level;
            BatteryState state;
            BatteryPowerSource source;
            EnergySaverStatus energySaver;
            if (TryReadBattery(out level, out state, out source, out energySaver))
            {
                _chargeLevel = level;
                _state = state;
                _powerSource = source;
                _energySaverStatus = energySaver;
                _pollTimer = new System.Threading.Timer(OnPollTimer, null,
                    PollIntervalMilliseconds, PollIntervalMilliseconds);
            }
#endif
        }

        private EventHandler<BatteryInfoChangedEventArgs> _batteryInfoChanged;

        public event EventHandler<BatteryInfoChangedEventArgs> BatteryInfoChanged
        {
            add
            {
                lock (_lock)
                {
                    _batteryInfoChanged += value;
                }
            }
            remove
            {
                lock (_lock)
                {
                    _batteryInfoChanged -= value;
                }
            }
        }

        private EventHandler<EnergySaverStatusChangedEventArgs> _energySaverStatusChanged;

        /* Raised on Windows only */
        public event EventHandler<EnergySaverStatusChangedEventArgs> EnergySaverStatusChanged
        {
            add
            {
                lock (_lock)
                {
                    _energySaverStatusChanged += value;
                }
            }
            remove
            {
                lock (_lock)
                {
                    _energySaverStatusChanged -= value;
                }
            }
        }

        public double ChargeLevel
        {
            get
            {
                lock (_lock)
                {
                    return _chargeLevel;
                }
            }
        }

        public BatteryState State
        {
            get
            {
                lock (_lock)
                {
                    return _state;
                }
            }
        }

        public BatteryPowerSource PowerSource
        {
            get
            {
                lock (_lock)
                {
                    return _powerSource;
                }
            }
        }

        public EnergySaverStatus EnergySaverStatus
        {
            get
            {
                lock (_lock)
                {
                    return _energySaverStatus;
                }
            }
        }

#if LINUX || MACOS || WINDOWS_AVALONIA
        /* True when a system battery was found at startup and is being polled */
        internal bool IsPolling => _pollTimer != null;

        private void RaiseBatteryInfoChanged(BatteryInfoChangedEventArgs args)
        {
            EventHandler<BatteryInfoChangedEventArgs> handler;
            lock (_lock)
            {
                handler = _batteryInfoChanged;
            }
            handler?.Invoke(null, args);
        }

        private void RaiseEnergySaverStatusChanged(EnergySaverStatusChangedEventArgs args)
        {
            EventHandler<EnergySaverStatusChangedEventArgs> handler;
            lock (_lock)
            {
                handler = _energySaverStatusChanged;
            }
            handler?.Invoke(null, args);
        }

        private void OnPollTimer(object timerState)
        {
            try
            {
                double level;
                BatteryState state;
                BatteryPowerSource source;
                EnergySaverStatus energySaver;
                if (!TryReadBattery(out level, out state, out source, out energySaver))
                {
                    level = 1.0;
                    state = BatteryState.NotPresent;
                    source = BatteryPowerSource.AC;
                    energySaver = EnergySaverStatus.Off;
                }

                bool batteryChanged;
                bool energySaverChanged;
                lock (_lock)
                {
                    batteryChanged = _chargeLevel != level || _state != state || _powerSource != source;
                    energySaverChanged = _energySaverStatus != energySaver;
                    _chargeLevel = level;
                    _state = state;
                    _powerSource = source;
                    _energySaverStatus = energySaver;
                }

                if (batteryChanged)
                {
                    BatteryInfoChangedEventArgs args = new BatteryInfoChangedEventArgs(level, state, source);
                    AvDispatcher.UIThread.Post(() => RaiseBatteryInfoChanged(args));
                }
                if (energySaverChanged)
                {
                    EnergySaverStatusChangedEventArgs args = new EnergySaverStatusChangedEventArgs(energySaver);
                    AvDispatcher.UIThread.Post(() => RaiseEnergySaverStatusChanged(args));
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine("GHAvaloniaBattery: polling failed: " + ex.Message);
            }
        }
#endif

#if LINUX
        /* Reads the first system battery: a power supply named BAT* or of type Battery, excluding
           peripheral batteries (scope Device). False when there is none. Energy saver is always Off. */
        private static bool TryReadBattery(out double level, out BatteryState state, out BatteryPowerSource source,
            out EnergySaverStatus energySaver)
        {
            level = 1.0;
            state = BatteryState.NotPresent;
            source = BatteryPowerSource.AC;
            energySaver = EnergySaverStatus.Off;

            string[] supplies;
            try
            {
                if (!Directory.Exists(PowerSupplyDirectory))
                    return false;
                supplies = Directory.GetDirectories(PowerSupplyDirectory);
            }
            catch (Exception)
            {
                return false;
            }
            Array.Sort(supplies, StringComparer.Ordinal);

            foreach (string supply in supplies)
            {
                string name = Path.GetFileName(supply);
                string type = ReadValue(supply, "type");
                bool isBattery = name.StartsWith("BAT", StringComparison.Ordinal)
                    || string.Equals(type, "Battery", StringComparison.Ordinal);
                if (!isBattery || string.Equals(ReadValue(supply, "scope"), "Device", StringComparison.Ordinal))
                    continue;

                int capacity;
                string capacityText = ReadValue(supply, "capacity");
                if (!int.TryParse(capacityText, NumberStyles.Integer, CultureInfo.InvariantCulture, out capacity))
                    continue;

                level = Math.Clamp(capacity / 100.0, 0.0, 1.0);
                state = MapStatus(ReadValue(supply, "status"));
                if (state == BatteryState.Discharging)
                {
                    source = BatteryPowerSource.Battery;
                }
                else
                {
                    source = ReadExternalPowerSource(supplies);
                    if (source == BatteryPowerSource.Unknown && state != BatteryState.Unknown)
                        source = BatteryPowerSource.AC;
                }
                return true;
            }
            return false;
        }

        private static BatteryState MapStatus(string status)
        {
            switch (status)
            {
            case "Charging":
                return BatteryState.Charging;
            case "Discharging":
                return BatteryState.Discharging;
            case "Full":
                return BatteryState.Full;
            case "Not charging":
                return BatteryState.NotCharging;
            default:
                return BatteryState.Unknown;
            }
        }

        /* AC for an online mains supply, Usb for an online USB supply, otherwise Unknown */
        private static BatteryPowerSource ReadExternalPowerSource(string[] supplies)
        {
            BatteryPowerSource result = BatteryPowerSource.Unknown;
            foreach (string supply in supplies)
            {
                if (!string.Equals(ReadValue(supply, "online"), "1", StringComparison.Ordinal))
                    continue;
                string type = ReadValue(supply, "type");
                if (string.Equals(type, "Mains", StringComparison.Ordinal))
                    return BatteryPowerSource.AC;
                if (type != null && type.StartsWith("USB", StringComparison.Ordinal))
                    result = BatteryPowerSource.Usb;
            }
            return result;
        }

        private static string ReadValue(string supplyDirectory, string attribute)
        {
            try
            {
                string path = Path.Combine(supplyDirectory, attribute);
                if (!File.Exists(path))
                    return null;
                using (StreamReader reader = new StreamReader(path))
                {
                    string line = reader.ReadLine();
                    return line?.Trim();
                }
            }
            catch (Exception)
            {
                return null;
            }
        }
#elif MACOS
        private const string IOKitLibrary = "/System/Library/Frameworks/IOKit.framework/IOKit";
        private const string CoreFoundationLibrary = "/System/Library/Frameworks/CoreFoundation.framework/CoreFoundation";
        private const uint kCFStringEncodingUTF8 = 0x08000100;
        private const int kCFNumberSInt64Type = 4;

        [DllImport(IOKitLibrary)]
        private static extern IntPtr IOPSCopyPowerSourcesInfo();

        [DllImport(IOKitLibrary)]
        private static extern IntPtr IOPSCopyPowerSourcesList(IntPtr blob);

        [DllImport(IOKitLibrary)]
        private static extern IntPtr IOPSGetPowerSourceDescription(IntPtr blob, IntPtr powerSource);

        [DllImport(CoreFoundationLibrary)]
        private static extern long CFArrayGetCount(IntPtr array);

        [DllImport(CoreFoundationLibrary)]
        private static extern IntPtr CFArrayGetValueAtIndex(IntPtr array, long index);

        [DllImport(CoreFoundationLibrary)]
        private static extern IntPtr CFDictionaryGetValue(IntPtr dictionary, IntPtr key);

        [DllImport(CoreFoundationLibrary)]
        private static extern IntPtr CFStringCreateWithCString(IntPtr allocator, [MarshalAs(UnmanagedType.LPUTF8Str)] string text, uint encoding);

        [DllImport(CoreFoundationLibrary)]
        [return: MarshalAs(UnmanagedType.I1)]
        private static extern bool CFStringGetCString(IntPtr text, byte[] buffer, long bufferSize, uint encoding);

        [DllImport(CoreFoundationLibrary)]
        [return: MarshalAs(UnmanagedType.I1)]
        private static extern bool CFNumberGetValue(IntPtr number, int type, out long value);

        [DllImport(CoreFoundationLibrary)]
        [return: MarshalAs(UnmanagedType.I1)]
        private static extern bool CFBooleanGetValue(IntPtr boolean);

        [DllImport(CoreFoundationLibrary)]
        private static extern ulong CFGetTypeID(IntPtr value);

        [DllImport(CoreFoundationLibrary)]
        private static extern ulong CFStringGetTypeID();

        [DllImport(CoreFoundationLibrary)]
        private static extern ulong CFNumberGetTypeID();

        [DllImport(CoreFoundationLibrary)]
        private static extern ulong CFBooleanGetTypeID();

        [DllImport(CoreFoundationLibrary)]
        private static extern void CFRelease(IntPtr value);

        /* The IOPSKeys.h key strings, created once and kept for the life of the process */
        private static readonly object _keyLock = new object();
        private static IntPtr _typeKey = IntPtr.Zero;
        private static IntPtr _isPresentKey = IntPtr.Zero;
        private static IntPtr _currentCapacityKey = IntPtr.Zero;
        private static IntPtr _maxCapacityKey = IntPtr.Zero;
        private static IntPtr _isChargingKey = IntPtr.Zero;
        private static IntPtr _isChargedKey = IntPtr.Zero;
        private static IntPtr _powerSourceStateKey = IntPtr.Zero;

        private static void CreateKeys()
        {
            lock (_keyLock)
            {
                if (_typeKey != IntPtr.Zero)
                    return;
                _isPresentKey = CFStringCreateWithCString(IntPtr.Zero, "Is Present", kCFStringEncodingUTF8);
                _currentCapacityKey = CFStringCreateWithCString(IntPtr.Zero, "Current Capacity", kCFStringEncodingUTF8);
                _maxCapacityKey = CFStringCreateWithCString(IntPtr.Zero, "Max Capacity", kCFStringEncodingUTF8);
                _isChargingKey = CFStringCreateWithCString(IntPtr.Zero, "Is Charging", kCFStringEncodingUTF8);
                _isChargedKey = CFStringCreateWithCString(IntPtr.Zero, "Is Charged", kCFStringEncodingUTF8);
                _powerSourceStateKey = CFStringCreateWithCString(IntPtr.Zero, "Power Source State", kCFStringEncodingUTF8);
                _typeKey = CFStringCreateWithCString(IntPtr.Zero, "Type", kCFStringEncodingUTF8);
            }
        }

        /* Reads the first present internal battery from IOKit's power sources. False when there is none, as on a
           desktop Mac. Energy saver is always Off. */
        private static bool TryReadBattery(out double level, out BatteryState state, out BatteryPowerSource source,
            out EnergySaverStatus energySaver)
        {
            level = 1.0;
            state = BatteryState.NotPresent;
            source = BatteryPowerSource.AC;
            energySaver = EnergySaverStatus.Off;

            try
            {
                CreateKeys();
            }
            catch (Exception)
            {
                return false;
            }

            IntPtr info = IntPtr.Zero;
            IntPtr list = IntPtr.Zero;
            try
            {
                info = IOPSCopyPowerSourcesInfo();
                if (info == IntPtr.Zero)
                    return false;
                list = IOPSCopyPowerSourcesList(info);
                if (list == IntPtr.Zero)
                    return false;

                long count = CFArrayGetCount(list);
                long i;
                for (i = 0; i < count; i++)
                {
                    IntPtr description = IOPSGetPowerSourceDescription(info, CFArrayGetValueAtIndex(list, i));
                    if (description == IntPtr.Zero)
                        continue;
                    if (GetString(description, _typeKey) != "InternalBattery" || GetBoolean(description, _isPresentKey) != true)
                        continue;

                    long current, max;
                    if (!GetNumber(description, _currentCapacityKey, out current) || !GetNumber(description, _maxCapacityKey, out max)
                        || max <= 0)
                        continue;

                    level = Math.Clamp((double)current / max, 0.0, 1.0);
                    string powerState = GetString(description, _powerSourceStateKey);
                    if (GetBoolean(description, _isChargingKey) == true)
                        state = BatteryState.Charging;
                    else if (GetBoolean(description, _isChargedKey) == true)
                        state = BatteryState.Full;
                    else if (powerState == "Battery Power")
                        state = BatteryState.Discharging;
                    else
                        state = BatteryState.NotCharging;

                    if (powerState == "AC Power")
                        source = BatteryPowerSource.AC;
                    else if (powerState == "Battery Power")
                        source = BatteryPowerSource.Battery;
                    else
                        source = BatteryPowerSource.Unknown;
                    return true;
                }
                return false;
            }
            catch (Exception)
            {
                level = 1.0;
                state = BatteryState.NotPresent;
                source = BatteryPowerSource.AC;
                return false;
            }
            finally
            {
                if (list != IntPtr.Zero)
                    CFRelease(list);
                if (info != IntPtr.Zero)
                    CFRelease(info);
            }
        }

        /* The dictionary's string value for the key, or null when it is missing or not a string */
        private static string GetString(IntPtr dictionary, IntPtr key)
        {
            IntPtr value = CFDictionaryGetValue(dictionary, key);
            if (value == IntPtr.Zero || CFGetTypeID(value) != CFStringGetTypeID())
                return null;
            byte[] buffer = new byte[256];
            if (!CFStringGetCString(value, buffer, buffer.Length, kCFStringEncodingUTF8))
                return null;
            int length = Array.IndexOf(buffer, (byte)0);
            return Encoding.UTF8.GetString(buffer, 0, length < 0 ? buffer.Length : length);
        }

        /* The dictionary's Boolean value for the key, or null when it is missing or not a Boolean */
        private static bool? GetBoolean(IntPtr dictionary, IntPtr key)
        {
            IntPtr value = CFDictionaryGetValue(dictionary, key);
            if (value == IntPtr.Zero || CFGetTypeID(value) != CFBooleanGetTypeID())
                return null;
            return CFBooleanGetValue(value);
        }

        /* The dictionary's number value for the key; false when it is missing or not a number */
        private static bool GetNumber(IntPtr dictionary, IntPtr key, out long result)
        {
            result = 0;
            IntPtr value = CFDictionaryGetValue(dictionary, key);
            if (value == IntPtr.Zero || CFGetTypeID(value) != CFNumberGetTypeID())
                return false;
            return CFNumberGetValue(value, kCFNumberSInt64Type, out result);
        }
#elif WINDOWS_AVALONIA
        private const byte AC_LINE_OFFLINE = 0;
        private const byte AC_LINE_ONLINE = 1;
        private const byte BATTERY_FLAG_CHARGING = 8;
        private const byte BATTERY_FLAG_NO_BATTERY = 128;
        private const byte BATTERY_FLAG_UNKNOWN = 255;
        private const byte BATTERY_PERCENTAGE_UNKNOWN = 255;
        private const byte SYSTEM_STATUS_FLAG_BATTERY_SAVER_ON = 1;

        [StructLayout(LayoutKind.Sequential)]
        private struct SYSTEM_POWER_STATUS
        {
            public byte ACLineStatus;
            public byte BatteryFlag;
            public byte BatteryLifePercent;
            public byte SystemStatusFlag;
            public uint BatteryLifeTime;
            public uint BatteryFullLifeTime;
        }

        [DllImport("kernel32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool GetSystemPowerStatus(out SYSTEM_POWER_STATUS status);

        /* Reads the system battery through GetSystemPowerStatus. False when there is none, or its state is unknown.
           A level Windows does not know is reported as full. */
        private static bool TryReadBattery(out double level, out BatteryState state, out BatteryPowerSource source,
            out EnergySaverStatus energySaver)
        {
            level = 1.0;
            state = BatteryState.NotPresent;
            source = BatteryPowerSource.AC;
            energySaver = EnergySaverStatus.Off;

            SYSTEM_POWER_STATUS status;
            try
            {
                if (!GetSystemPowerStatus(out status))
                    return false;
            }
            catch (Exception)
            {
                return false;
            }
            if (status.BatteryFlag == BATTERY_FLAG_UNKNOWN || (status.BatteryFlag & BATTERY_FLAG_NO_BATTERY) != 0)
                return false;

            if (status.BatteryLifePercent != BATTERY_PERCENTAGE_UNKNOWN)
                level = Math.Clamp(status.BatteryLifePercent / 100.0, 0.0, 1.0);

            if (status.ACLineStatus == AC_LINE_ONLINE)
                source = BatteryPowerSource.AC;
            else if (status.ACLineStatus == AC_LINE_OFFLINE)
                source = BatteryPowerSource.Battery;
            else
                source = BatteryPowerSource.Unknown;

            if ((status.BatteryFlag & BATTERY_FLAG_CHARGING) != 0)
                state = BatteryState.Charging;
            else if (source == BatteryPowerSource.Battery)
                state = BatteryState.Discharging;
            else if (source == BatteryPowerSource.AC)
                state = status.BatteryLifePercent == 100 ? BatteryState.Full : BatteryState.NotCharging;
            else
                state = BatteryState.Unknown;

            energySaver = (status.SystemStatusFlag & SYSTEM_STATUS_FLAG_BATTERY_SAVER_ON) != 0
                ? EnergySaverStatus.On : EnergySaverStatus.Off;
            return true;
        }
#endif
    }
}
