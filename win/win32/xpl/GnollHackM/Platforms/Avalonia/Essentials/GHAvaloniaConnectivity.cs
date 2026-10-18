using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Net.NetworkInformation;
using Microsoft.Maui.Networking;

using AvDispatcher = Avalonia.Threading.Dispatcher;

namespace GnollHackM
{
    /* IConnectivity for the Avalonia desktop target, from System.Net.NetworkInformation. Access is
       Internet when any network is available and None otherwise; the profiles are the kinds of the
       up, non-loopback interfaces. ConnectivityChanged is raised on the Avalonia UI thread, only
       when the access or the profiles differ from the last values seen, and only while subscribed. */
    public sealed class GHAvaloniaConnectivity : IConnectivity
    {
        private readonly object _lock = new object();
        private EventHandler<ConnectivityChangedEventArgs> _connectivityChanged;
        private bool _listening = false;
        private Microsoft.Maui.Networking.NetworkAccess _currentAccess =
            Microsoft.Maui.Networking.NetworkAccess.Unknown;
        private List<ConnectionProfile> _currentProfiles = new List<ConnectionProfile>();

        public IEnumerable<ConnectionProfile> ConnectionProfiles => ReadProfiles();

        public Microsoft.Maui.Networking.NetworkAccess NetworkAccess => ReadAccess();

        public event EventHandler<ConnectivityChangedEventArgs> ConnectivityChanged
        {
            add
            {
                lock (_lock)
                {
                    if (_connectivityChanged == null)
                        StartListening();
                    _connectivityChanged += value;
                }
            }
            remove
            {
                lock (_lock)
                {
                    _connectivityChanged -= value;
                    if (_connectivityChanged == null)
                        StopListening();
                }
            }
        }

        private static Microsoft.Maui.Networking.NetworkAccess ReadAccess()
        {
            try
            {
                return NetworkInterface.GetIsNetworkAvailable()
                    ? Microsoft.Maui.Networking.NetworkAccess.Internet
                    : Microsoft.Maui.Networking.NetworkAccess.None;
            }
            catch (Exception)
            {
                return Microsoft.Maui.Networking.NetworkAccess.Unknown;
            }
        }

        private static List<ConnectionProfile> ReadProfiles()
        {
            List<ConnectionProfile> profiles = new List<ConnectionProfile>();
            try
            {
                foreach (NetworkInterface networkInterface in NetworkInterface.GetAllNetworkInterfaces())
                {
                    if (networkInterface.OperationalStatus != OperationalStatus.Up)
                        continue;

                    ConnectionProfile profile;
                    switch (networkInterface.NetworkInterfaceType)
                    {
                    case NetworkInterfaceType.Loopback:
                        continue;
                    case NetworkInterfaceType.Wireless80211:
                        profile = ConnectionProfile.WiFi;
                        break;
                    case NetworkInterfaceType.Ethernet:
                    case NetworkInterfaceType.Ethernet3Megabit:
                    case NetworkInterfaceType.FastEthernetT:
                    case NetworkInterfaceType.FastEthernetFx:
                    case NetworkInterfaceType.GigabitEthernet:
                        profile = ConnectionProfile.Ethernet;
                        break;
                    default:
                        profile = ConnectionProfile.Unknown;
                        break;
                    }

                    if (!profiles.Contains(profile))
                        profiles.Add(profile);
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine("GHAvaloniaConnectivity: listing network interfaces failed: " + ex.Message);
            }
            return profiles;
        }

        /* Called with _lock held */
        private void StartListening()
        {
            if (_listening)
                return;

            _currentAccess = ReadAccess();
            _currentProfiles = ReadProfiles();
            try
            {
                NetworkChange.NetworkAvailabilityChanged += OnNetworkAvailabilityChanged;
                NetworkChange.NetworkAddressChanged += OnNetworkAddressChanged;
                _listening = true;
            }
            catch (Exception ex)
            {
                Debug.WriteLine("GHAvaloniaConnectivity: subscribing to network changes failed: " + ex.Message);
                StopListening();
            }
        }

        /* Called with _lock held */
        private void StopListening()
        {
            try
            {
                NetworkChange.NetworkAvailabilityChanged -= OnNetworkAvailabilityChanged;
                NetworkChange.NetworkAddressChanged -= OnNetworkAddressChanged;
            }
            catch (Exception ex)
            {
                Debug.WriteLine("GHAvaloniaConnectivity: unsubscribing from network changes failed: " + ex.Message);
            }
            _listening = false;
        }

        private void OnNetworkAvailabilityChanged(object sender, NetworkAvailabilityEventArgs e)
        {
            CheckForChange();
        }

        private void OnNetworkAddressChanged(object sender, EventArgs e)
        {
            CheckForChange();
        }

        private void CheckForChange()
        {
            Microsoft.Maui.Networking.NetworkAccess access = ReadAccess();
            List<ConnectionProfile> profiles = ReadProfiles();

            lock (_lock)
            {
                if (!_listening)
                    return;
                if (access == _currentAccess && profiles.SequenceEqual(_currentProfiles))
                    return;
                _currentAccess = access;
                _currentProfiles = profiles;
            }

            ConnectivityChangedEventArgs args = new ConnectivityChangedEventArgs(access, profiles.ToArray());
            AvDispatcher.UIThread.Post(() => RaiseConnectivityChanged(args));
        }

        private void RaiseConnectivityChanged(ConnectivityChangedEventArgs args)
        {
            EventHandler<ConnectivityChangedEventArgs> handler;
            lock (_lock)
            {
                handler = _connectivityChanged;
            }
            handler?.Invoke(null, args);
        }
    }
}
