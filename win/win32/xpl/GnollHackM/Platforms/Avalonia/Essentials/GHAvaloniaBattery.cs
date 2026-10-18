using System;
using System.Diagnostics;
using System.Globalization;
using System.IO;
using Microsoft.Maui.Devices;

using AvDispatcher = Avalonia.Threading.Dispatcher;

namespace GnollHackM
{
    /* IBattery for the Avalonia desktop target. On Linux it polls the system battery in
       /sys/class/power_supply every 60 s and raises BatteryInfoChanged on the Avalonia UI thread when
       the level, state or power source changes. Without a battery, and on Windows, it reports a full
       charge on AC with State NotPresent, which keeps GHApp's battery-dependent saving enabled.
       Energy saver is always Off. */
    public sealed class GHAvaloniaBattery : IBattery
    {
        private readonly object _lock = new object();
        private double _chargeLevel = 1.0;
        private BatteryState _state = BatteryState.NotPresent;
        private BatteryPowerSource _powerSource = BatteryPowerSource.AC;

#if LINUX
        private const string PowerSupplyDirectory = "/sys/class/power_supply";
        private const int PollIntervalMilliseconds = 60000;
        private readonly System.Threading.Timer _pollTimer = null;
#endif

        public GHAvaloniaBattery()
        {
#if LINUX
            double level;
            BatteryState state;
            BatteryPowerSource source;
            if (TryReadBattery(out level, out state, out source))
            {
                _chargeLevel = level;
                _state = state;
                _powerSource = source;
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

        /* Never raised */
        public event EventHandler<EnergySaverStatusChangedEventArgs> EnergySaverStatusChanged
        {
            add
            {
            }
            remove
            {
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

        public EnergySaverStatus EnergySaverStatus => EnergySaverStatus.Off;

#if LINUX
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

        private void OnPollTimer(object timerState)
        {
            try
            {
                double level;
                BatteryState state;
                BatteryPowerSource source;
                if (!TryReadBattery(out level, out state, out source))
                {
                    level = 1.0;
                    state = BatteryState.NotPresent;
                    source = BatteryPowerSource.AC;
                }

                lock (_lock)
                {
                    if (_chargeLevel == level && _state == state && _powerSource == source)
                        return;
                    _chargeLevel = level;
                    _state = state;
                    _powerSource = source;
                }

                BatteryInfoChangedEventArgs args = new BatteryInfoChangedEventArgs(level, state, source);
                AvDispatcher.UIThread.Post(() => RaiseBatteryInfoChanged(args));
            }
            catch (Exception ex)
            {
                Debug.WriteLine("GHAvaloniaBattery: polling failed: " + ex.Message);
            }
        }

        /* Reads the first system battery: a power supply named BAT* or of type Battery, excluding
           peripheral batteries (scope Device). False when there is none. */
        private static bool TryReadBattery(out double level, out BatteryState state, out BatteryPowerSource source)
        {
            level = 1.0;
            state = BatteryState.NotPresent;
            source = BatteryPowerSource.AC;

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
#endif
    }
}
