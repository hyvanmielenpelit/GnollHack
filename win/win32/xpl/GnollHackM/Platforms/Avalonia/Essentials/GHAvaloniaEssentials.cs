using System;
using System.Runtime.CompilerServices;
using Microsoft.Maui.ApplicationModel;
using Microsoft.Maui.ApplicationModel.DataTransfer;
using Microsoft.Maui.Devices;
using Microsoft.Maui.Networking;
using Microsoft.Maui.Storage;

namespace GnollHackM
{
    /* Installs GnollHack's Essentials implementations into the MAUI static facades for the Avalonia
       target. The facades' setters are internal, so they are reached with UnsafeAccessor. */
    public static class GHAvaloniaEssentials
    {
        private static readonly object _installLock = new object();
        private static GHAvaloniaDeviceDisplay _deviceDisplay = null;
        private static GHAvaloniaDeviceInfo _deviceInfo = null;
        private static GHAvaloniaAppInfo _appInfo = null;
        private static GHAvaloniaPreferences _preferences = null;
        private static GHAvaloniaBattery _battery = null;
        private static GHAvaloniaConnectivity _connectivity = null;
        private static GHAvaloniaSecureStorage _secureStorage = null;
        private static GHAvaloniaBrowser _browser = null;
        private static GHAvaloniaLauncher _launcher = null;
        private static GHAvaloniaShare _share = null;
        private static GHAvaloniaFilePicker _filePicker = null;

        // WORKAROUND(WA-AVALONIA-0013): ACM Essentials leaves most Essentials APIs unimplemented
        /* Installs all ten implementations. Runs before GHApp's static initializers and before
           the MAUI app builder exists; the instances are created once and reused. */
        public static void InstallEarly()
        {
            lock (_installLock)
            {
                EnsureCreated();
                SetDeviceDisplay(null, _deviceDisplay);
                SetDeviceInfo(null, _deviceInfo);
                SetAppInfo(null, _appInfo);
                SetPreferences(null, _preferences);
                SetBattery(null, _battery);
                SetConnectivity(null, _connectivity);
                SetSecureStorage(null, _secureStorage);
                SetBrowser(null, _browser);
                SetLauncher(null, _launcher);
                SetShare(null, _share);
                GHAvaloniaDeviceDisplay.Current = _deviceDisplay;
            }
        }

        // WORKAROUND(WA-AVALONIA-0013): ACM Essentials leaves most Essentials APIs unimplemented
        /* Re-installs Preferences and AppInfo, which UseAvaloniaEssentials() and UseAvaloniaApp()
           replace when called. The other eight are not touched by ACM. Wraps the FilePicker that
           UseAvaloniaEssentials() installed, once. */
        public static void InstallAfterBuilder()
        {
            lock (_installLock)
            {
                if (_preferences == null || _appInfo == null)
                {
                    InstallEarly();
                }
                else
                {
                    SetPreferences(null, _preferences);
                    SetAppInfo(null, _appInfo);
                }
                if (_filePicker == null)
                {
                    _filePicker = new GHAvaloniaFilePicker(FilePicker.Default);
                    SetFilePicker(null, _filePicker);
                }
            }
        }

        private static void EnsureCreated()
        {
            if (_deviceDisplay == null)
                _deviceDisplay = new GHAvaloniaDeviceDisplay();
            if (_deviceInfo == null)
                _deviceInfo = new GHAvaloniaDeviceInfo();
            if (_appInfo == null)
                _appInfo = new GHAvaloniaAppInfo();
            if (_preferences == null)
                _preferences = new GHAvaloniaPreferences();
            if (_battery == null)
                _battery = new GHAvaloniaBattery();
            if (_connectivity == null)
                _connectivity = new GHAvaloniaConnectivity();
            if (_secureStorage == null)
                _secureStorage = new GHAvaloniaSecureStorage();
            if (_browser == null)
                _browser = new GHAvaloniaBrowser();
            if (_launcher == null)
                _launcher = new GHAvaloniaLauncher();
            if (_share == null)
                _share = new GHAvaloniaShare();
        }

        [UnsafeAccessor(UnsafeAccessorKind.StaticMethod, Name = "SetCurrent")]
        private static extern void SetDeviceDisplay(
            [UnsafeAccessorType("Microsoft.Maui.Devices.DeviceDisplay, Microsoft.Maui.Essentials")] object facade,
            IDeviceDisplay implementation);

        [UnsafeAccessor(UnsafeAccessorKind.StaticMethod, Name = "SetCurrent")]
        private static extern void SetDeviceInfo(
            [UnsafeAccessorType("Microsoft.Maui.Devices.DeviceInfo, Microsoft.Maui.Essentials")] object facade,
            IDeviceInfo implementation);

        [UnsafeAccessor(UnsafeAccessorKind.StaticMethod, Name = "SetCurrent")]
        private static extern void SetAppInfo(
            [UnsafeAccessorType("Microsoft.Maui.ApplicationModel.AppInfo, Microsoft.Maui.Essentials")] object facade,
            IAppInfo implementation);

        [UnsafeAccessor(UnsafeAccessorKind.StaticMethod, Name = "SetDefault")]
        private static extern void SetPreferences(
            [UnsafeAccessorType("Microsoft.Maui.Storage.Preferences, Microsoft.Maui.Essentials")] object facade,
            IPreferences implementation);

        [UnsafeAccessor(UnsafeAccessorKind.StaticMethod, Name = "SetDefault")]
        private static extern void SetBattery(
            [UnsafeAccessorType("Microsoft.Maui.Devices.Battery, Microsoft.Maui.Essentials")] object facade,
            IBattery implementation);

        [UnsafeAccessor(UnsafeAccessorKind.StaticMethod, Name = "SetCurrent")]
        private static extern void SetConnectivity(
            [UnsafeAccessorType("Microsoft.Maui.Networking.Connectivity, Microsoft.Maui.Essentials")] object facade,
            IConnectivity implementation);

        [UnsafeAccessor(UnsafeAccessorKind.StaticMethod, Name = "SetDefault")]
        private static extern void SetSecureStorage(
            [UnsafeAccessorType("Microsoft.Maui.Storage.SecureStorage, Microsoft.Maui.Essentials")] object facade,
            ISecureStorage implementation);

        [UnsafeAccessor(UnsafeAccessorKind.StaticMethod, Name = "SetDefault")]
        private static extern void SetBrowser(
            [UnsafeAccessorType("Microsoft.Maui.ApplicationModel.Browser, Microsoft.Maui.Essentials")] object facade,
            IBrowser implementation);

        [UnsafeAccessor(UnsafeAccessorKind.StaticMethod, Name = "SetDefault")]
        private static extern void SetLauncher(
            [UnsafeAccessorType("Microsoft.Maui.ApplicationModel.Launcher, Microsoft.Maui.Essentials")] object facade,
            ILauncher implementation);

        [UnsafeAccessor(UnsafeAccessorKind.StaticMethod, Name = "SetDefault")]
        private static extern void SetShare(
            [UnsafeAccessorType("Microsoft.Maui.ApplicationModel.DataTransfer.Share, Microsoft.Maui.Essentials")]
            object facade,
            IShare implementation);

        [UnsafeAccessor(UnsafeAccessorKind.StaticMethod, Name = "SetDefault")]
        private static extern void SetFilePicker(
            [UnsafeAccessorType("Microsoft.Maui.Storage.FilePicker, Microsoft.Maui.Essentials")] object facade,
            IFilePicker implementation);
    }
}
