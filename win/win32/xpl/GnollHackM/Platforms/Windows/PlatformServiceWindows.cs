#if WINDOWS && GNH_MAUI
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Runtime.InteropServices;
using GnollHackX;
using GnollHackX.Performance;
using System.Runtime.Intrinsics.Arm;
using Windows.Services.Store;
using Windows.System;
using System.IO;
using System.Diagnostics;

namespace GnollHackM
{
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Auto)]
    internal class MEMORYSTATUSEX
    {
        public uint dwLength;
        public uint dwMemoryLoad;
        public ulong ullTotalPhys;
        public ulong ullAvailPhys;
        public ulong ullTotalPageFile;
        public ulong ullAvailPageFile;
        public ulong ullTotalVirtual;
        public ulong ullAvailVirtual;
        public ulong ullAvailExtendedVirtual;

        public MEMORYSTATUSEX()
        {
            dwLength = Convert.ToUInt32(Marshal.SizeOf(typeof(MEMORYSTATUSEX)));
        }
    }

    public class PlatformService : IPlatformService
    {
        [DllImport("kernel32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        static extern bool GlobalMemoryStatusEx([In, Out] MEMORYSTATUSEX lpBuffer);

        private MemoryStatus GetMemoryStatus()
        {
            MemoryStatus res = new MemoryStatus();
            MEMORYSTATUSEX memStatusEx = new MEMORYSTATUSEX();
            if (GlobalMemoryStatusEx(memStatusEx))
            {
                res.TotalPhysical = memStatusEx.ullTotalPhys;
                res.AvailablePhysical = memStatusEx.ullAvailPhys;
                res.TotalPageFile = memStatusEx.ullTotalPageFile;
                res.AvailablePageFile = memStatusEx.ullAvailPageFile;
                res.TotalVirtual = memStatusEx.ullTotalVirtual;
                res.AvailableVirtual = memStatusEx.ullAvailVirtual;
                res.AvailableExtendedVirtual = memStatusEx.ullAvailExtendedVirtual;
            }

            return res;
        }


        public string GetVersionString()
        {

            return DeviceInfo.Current.VersionString;
        }

        public bool IsRunningOnDesktop()
        {
            return true;
        }

        public ulong GetUsedMemoryInBytes()
        {
            try
            {
                var process = Process.GetCurrentProcess();
                return (ulong)(process?.PrivateMemorySize64 ?? 0);
                //MemoryStatus memoryStatus = GetMemoryStatus();
                //if (memoryStatus.TotalPhysical >= memoryStatus.AvailablePhysical)
                //    return memoryStatus.TotalPhysical - memoryStatus.AvailablePhysical;
                //else
                //    return 0;
            }
            catch
            {
                return 0;
            }
        }

        public ulong GetDeviceMemoryInBytes()
        {
            try
            {
                MemoryStatus memoryStatus = GetMemoryStatus();
                return memoryStatus.TotalPhysical;
            }
            catch
            {
                return 0;
            }
        }

        public ulong GetDeviceFreeDiskSpaceInBytes()
        {
            try
            {
                string curDir = Directory.GetCurrentDirectory();
                DirectoryInfo di = new DirectoryInfo(curDir);
                DriveInfo drive = new DriveInfo(di.Root.FullName);
                ulong freeSize = (ulong)drive.AvailableFreeSpace;
                return freeSize;
            }
            catch
            {
                return 0;
            }
        }

        public ulong GetDeviceTotalDiskSpaceInBytes()
        {
            try
            {
                string curDir = Directory.GetCurrentDirectory();
                DirectoryInfo di = new DirectoryInfo(curDir);
                DriveInfo drive = new DriveInfo(di.Root.FullName);
                ulong totalSize = (ulong)drive.TotalSize;
                return totalSize;
            }
            catch
            {
                return 0;
            }
        }

        public float GetPlatformScreenScale()
        {
            try
            {
                return 1.0f;
            }
            catch
            {
                return 1.0f;
            }
        }

        public void CloseApplication()
        {
            MainThread.BeginInvokeOnMainThread(() =>
            {
                GHApp.AddSentryBreadcrumb("CloseApplication", GHConstants.SentryGnollHackGeneralCategoryName);
                RevertAnimatorDuration(true);
                GHApp.SaveWindowPosition();
                if (GHApp.WindowsApp != null)
                {
                    GHApp.WindowsApp?.Exit();
                    GHApp.WindowsApp = null;
                }
                Application.Current?.Quit();
                //Environment.Exit(0);
            });
        }

        public Task<Stream> GetPlatformAssetsStreamAsync(string directory, string fileName)
        {
            string relativePath = string.IsNullOrEmpty(directory) ? fileName : Path.Combine(directory, fileName);
            return FileSystem.Current.OpenAppPackageFileAsync(relativePath);
        }

        public void SetStatusBarHidden(bool ishidden)
        {

        }

        public bool GetStatusBarHidden()
        {
            return true;
        }

        public float GetAnimatorDurationScaleSetting()
        {
            return 1.0f;
        }
        public float GetCurrentAnimatorDurationScale()
        {
            return 1.0f;
        }

        public float GetTransitionAnimationScaleSetting()
        {
            return 1.0f;
        }

        public float GetWindowAnimationScaleSetting()
        {
            return 1.0f;
        }

        public bool IsRemoveAnimationsOn()
        {
            var scale1 = GetAnimatorDurationScaleSetting();
            var scale2 = GetTransitionAnimationScaleSetting();
            var scale3 = GetWindowAnimationScaleSetting();

            return scale1 == 0 && scale2 == 0 && scale3 == 0;
        }
        public void OverrideAnimatorDuration()
        {

        }
        public void RevertAnimatorDuration(bool isfinal)
        {

        }

        public async Task RequestAppReview(ContentPage page)
        {
            if (GHApp.IsPackaged && !GHApp.IsSteam && !GHApp.IsNoStore) /* Microsoft Store */
            {
                try
                {
                    await PromptUserToRateApp(page);
                }
                catch (Exception ex)
                {
                    System.Diagnostics.Debug.WriteLine(ex.Message);
                }
                await System.Threading.Tasks.Task.Delay(50);
            }
        }


        private StoreContext _storeContext;

        public void InitializeStoreReview()
        {
            try
            {
                _storeContext = StoreContext.GetDefault();
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine(ex.Message);
            }
        }

        private async Task PromptUserToRateApp(ContentPage page)
        {
            if (_storeContext == null)
                InitializeStoreReview();

            if (_storeContext == null || GHApp.WindowsXamlWindow == null)
                return;

            try
            {
                var hwnd = WinRT.Interop.WindowNative.GetWindowHandle(GHApp.WindowsXamlWindow);
                WinRT.Interop.InitializeWithWindow.Initialize(_storeContext, hwnd);

                StoreRateAndReviewResult result = await _storeContext.RequestRateAndReviewAppAsync();
                if (result != null)
                {
                    switch (result.Status)
                    {
                        case StoreRateAndReviewStatus.Succeeded:
                            // Was this an updated review or a new review, if Updated is false it means it was a users first time reviewing
                            if (result.WasUpdated)
                            {
                                // This was an updated review thank user
                            }
                            else
                            {
                                // This was a new review, thank user for reviewing and give some free in app tokens
                            }
                            // Keep track that we prompted user and don’t do it again for a while
                            break;

                        case StoreRateAndReviewStatus.CanceledByUser:
                            // Keep track that we prompted user and don’t prompt again for a while
                            break;

                        case StoreRateAndReviewStatus.NetworkError:
                            // User is probably not connected, so we’ll try again, but keep track so we don’t try too often
                            break;

                        // Something else went wrong
                        case StoreRateAndReviewStatus.Error:
                        default:
                            if (result.ExtendedError?.Message != null)
                                System.Diagnostics.Debug.WriteLine(result.ExtendedError.Message);
                            if (result.ExtendedJsonData != null)
                                System.Diagnostics.Debug.WriteLine(result.ExtendedJsonData);
                            break;
                    }
                }
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine(ex.Message);
            }
        }

        private string GetAssemblyDirectory()
        {
            try
            {
                string assemblyFullPath = System.Reflection.Assembly.GetExecutingAssembly().Location;
                string assemblyDirectory = Path.GetDirectoryName(assemblyFullPath);
                return assemblyDirectory;
            }
            catch (Exception)
            {
                return ".";
            }
        }

        public string GetBaseUrl()
        {
            return Path.Combine(GetAssemblyDirectory(), "Assets");
        }
        public string GetAssetsPath()
        {
            return GetAssemblyDirectory();
        }

        public string GetCanonicalPath(string fileName)
        {
            try
            {
                return Path.Combine(GetAssemblyDirectory(), fileName);
            }
            catch (Exception)
            {
                return fileName;
            }
        }

        public string GetAbsoluteOnDemandAssetPath(string assetPack)
        {
            return null;
        }

        public string GetAbsoluteOnDemandAssetPath(string assetPack, string relativeAssetPath)
        {
            return null;
        }

        public int FetchOnDemandPack(string pack)
        {
            return -2; /* No need to load */
        }

        public event EventHandler<AssetPackStatusEventArgs> OnDemandPackStatusNotification;

        private void OnDemandPackStatusNotified(object sender, AssetPackStatusEventArgs e)
        {
            OnDemandPackStatusNotification?.Invoke(this, e);
        }

        public void InitializePlatform()
        {

        }

        public void EnsureWindowFocus()
        {

        }

        public void HideKeyboard()
        {

        }

        public void SetAdjustResize(bool adjustResize)
        {
        }

        public void HideOsNavigationBar()
        {

        }
        public void ShowOsNavigationBar()
        {

        }
        public void CollectGarbage()
        {

        }
        public bool GetKeyboardConnected()
        {
            return true;
        }

        /* CPU performance is read through PDH (pdh.dll), the same source as the
           "% Processor Performance" performance counter: actual CPU frequency as a
           percentage of nominal. A rate counter needs two samples, so the query is
           collected at most once per 500 ms and the value derived from the previous
           pair is returned; the very first reading is NaN. The counter set can be
           missing or disabled, in which case every reading is NaN. */
        [StructLayout(LayoutKind.Explicit, Size = 16)]
        private struct PDH_FMT_COUNTERVALUE
        {
            [FieldOffset(0)]
            public uint CStatus;
            [FieldOffset(8)]
            public double doubleValue;
        }

        [DllImport("pdh.dll", CharSet = CharSet.Unicode)]
        private static extern uint PdhOpenQueryW(string szDataSource, IntPtr dwUserData, out IntPtr phQuery);

        [DllImport("pdh.dll", CharSet = CharSet.Unicode)]
        private static extern uint PdhAddEnglishCounterW(IntPtr hQuery, string szFullCounterPath, IntPtr dwUserData, out IntPtr phCounter);

        [DllImport("pdh.dll")]
        private static extern uint PdhCollectQueryData(IntPtr hQuery);

        [DllImport("pdh.dll")]
        private static extern uint PdhGetFormattedCounterValue(IntPtr hCounter, uint dwFormat, IntPtr lpdwType, out PDH_FMT_COUNTERVALUE pValue);

        private const uint PDH_FMT_DOUBLE = 0x00000200;
        private const uint PDH_CSTATUS_VALID_DATA = 0x00000000;
        private const uint PDH_CSTATUS_NEW_DATA = 0x00000001;
        private const long CpuPerfMinSampleIntervalTicks = 500 * TimeSpan.TicksPerMillisecond;
        private const string CpuPerfCounterPath = @"\Processor Information(_Total)\% Processor Performance";

        private static readonly object _cpuPerfLock = new object();
        private static IntPtr _cpuPerfQuery = IntPtr.Zero;
        private static IntPtr _cpuPerfCounter = IntPtr.Zero;
        private static bool _cpuPerfInitTried = false;
        private static long _cpuPerfLastSampleTicks = 0;
        private static float _cpuPerfLastValue = float.NaN;

        private static float ReadCpuPerformancePct()
        {
            lock (_cpuPerfLock)
            {
                try
                {
                    if (!_cpuPerfInitTried)
                    {
                        _cpuPerfInitTried = true;
                        IntPtr query;
                        if (PdhOpenQueryW(null, IntPtr.Zero, out query) == 0 && query != IntPtr.Zero)
                        {
                            IntPtr counter;
                            if (PdhAddEnglishCounterW(query, CpuPerfCounterPath, IntPtr.Zero, out counter) == 0 && counter != IntPtr.Zero)
                            {
                                _cpuPerfQuery = query;
                                _cpuPerfCounter = counter;
                            }
                        }
                    }

                    if (_cpuPerfQuery == IntPtr.Zero || _cpuPerfCounter == IntPtr.Zero)
                        return float.NaN;

                    long now = DateTime.UtcNow.Ticks;
                    if (_cpuPerfLastSampleTicks != 0 && now - _cpuPerfLastSampleTicks < CpuPerfMinSampleIntervalTicks)
                        return _cpuPerfLastValue;

                    if (PdhCollectQueryData(_cpuPerfQuery) != 0)
                        return _cpuPerfLastValue;

                    bool hadPrevious = _cpuPerfLastSampleTicks != 0;
                    _cpuPerfLastSampleTicks = now;
                    if (!hadPrevious)
                        return float.NaN; /* Only one sample; a rate needs two */

                    PDH_FMT_COUNTERVALUE value;
                    uint res = PdhGetFormattedCounterValue(_cpuPerfCounter, PDH_FMT_DOUBLE, IntPtr.Zero, out value);
                    if (res == 0 && (value.CStatus == PDH_CSTATUS_VALID_DATA || value.CStatus == PDH_CSTATUS_NEW_DATA))
                        _cpuPerfLastValue = (float)value.doubleValue;
                    return _cpuPerfLastValue;
                }
                catch
                {
                    return float.NaN;
                }
            }
        }

        public GHThermalReading GetThermalReading()
        {
            GHThermalReading r = GHThermalProbe.Unknown;
            try
            {
                r.CpuPerformancePct = ReadCpuPerformancePct();
            }
            catch
            {
                r.CpuPerformancePct = float.NaN;
            }

            string batteryDetail = "battery=unknown";
            try
            {
                /* On external power: a full battery on AC and a desktop without a battery
                   both count, which BatteryStatus.Charging alone would miss */
                global::Windows.System.Power.PowerSupplyStatus supply = global::Windows.System.Power.PowerManager.PowerSupplyStatus;
                global::Windows.System.Power.BatteryStatus batteryStatus = global::Windows.System.Power.PowerManager.BatteryStatus;
                r.IsCharging = supply != global::Windows.System.Power.PowerSupplyStatus.NotPresent;
                r.PowerStateKnown = true;
                batteryDetail = "battery=" + batteryStatus.ToString() + " supply=" + supply.ToString();
            }
            catch
            {
                r.IsCharging = false;
            }

            try
            {
                r.IsLowPower = global::Windows.System.Power.PowerManager.EnergySaverStatus == global::Windows.System.Power.EnergySaverStatus.On;
            }
            catch
            {
                r.IsLowPower = false;
            }

            try
            {
                r.Detail = "cpuperf=" + (float.IsNaN(r.CpuPerformancePct) ? "n/a" : r.CpuPerformancePct.ToString("0.#", System.Globalization.CultureInfo.InvariantCulture))
                    + " " + batteryDetail
                    + " energysaver=" + (r.IsLowPower ? "on" : "off");
            }
            catch
            {
                r.Detail = null;
            }
            r.TimestampTicks = DateTime.UtcNow.Ticks;
            return r;
        }

        public bool SetSustainedPerformanceMode(bool enabled)
        {
            return false;
        }

        /* Background load. The system query is separate from the % Processor Performance
           query above and holds three whole-machine counters; own CPU comes from
           GetProcessTimes and memory from GlobalMemoryStatusEx. Rates are over the time
           since the previous call, so the first call returns NaN rates. Each counter that
           cannot be added stays NaN on its own. Allocation-free after the first call. */
        [StructLayout(LayoutKind.Sequential)]
        private struct PDH_FMT_COUNTERVALUE_ITEM_W
        {
            public IntPtr szName;
            public PDH_FMT_COUNTERVALUE FmtValue;
        }

        [DllImport("pdh.dll", CharSet = CharSet.Unicode)]
        private static extern uint PdhGetFormattedCounterArrayW(IntPtr hCounter, uint dwFormat, ref uint lpdwBufferSize, out uint lpdwItemCount, IntPtr ItemBuffer);

        [DllImport("pdh.dll")]
        private static extern uint PdhCloseQuery(IntPtr hQuery);

        [DllImport("kernel32.dll", EntryPoint = "GetCurrentProcess")]
        private static extern IntPtr GetCurrentProcessHandle();

        [DllImport("kernel32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool GetProcessTimes(IntPtr hProcess, out long lpCreationTime, out long lpExitTime, out long lpKernelTime, out long lpUserTime);

        [DllImport("powrprof.dll")]
        private static extern uint PowerGetActiveScheme(IntPtr UserRootPowerKey, out IntPtr ActivePolicyGuid);

        [DllImport("powrprof.dll")]
        private static extern uint PowerGetEffectiveOverlayScheme(out Guid EffectiveOverlayGuid);

        [DllImport("kernel32.dll")]
        private static extern IntPtr LocalFree(IntPtr hMem);

        private const uint PDH_FMT_NOCAP100 = 0x00008000;
        private const uint PDH_MORE_DATA = 0x800007D2;
        private const string SystemCpuCounterPath = @"\Processor(_Total)\% Processor Time";
        private const string DiskIdleCounterPath = @"\PhysicalDisk(_Total)\% Idle Time";
        private const string PagesInputCounterPath = @"\Memory\Pages Input/sec";
        private const string ProcessCpuCounterPath = @"\Process(*)\% Processor Time";
        private const string GpuEngineCounterPath = @"\GPU Engine(*)\Utilization Percentage";
        private const int MaxProcessCpuRows = 8;
        private const int MaxProcessGpuRows = 3;

        private static readonly object _sysLoadLock = new object();
        private static bool _sysLoadInitTried = false;
        private static IntPtr _sysLoadQuery = IntPtr.Zero;
        private static IntPtr _sysCpuCounter = IntPtr.Zero;
        private static IntPtr _sysDiskIdleCounter = IntPtr.Zero;
        private static IntPtr _sysPagesInputCounter = IntPtr.Zero;
        private static bool _sysLoadHasPrevious = false;
        private static long _ownCpuLastTicks = -1;          /* kernel + user, 100 ns */
        private static long _ownCpuLastTimestamp = 0;       /* Stopwatch timestamp */
        private static readonly MEMORYSTATUSEX _sysLoadMemStatus = new MEMORYSTATUSEX();

        private static IntPtr AddCounterOrZero(IntPtr query, string path)
        {
            IntPtr counter;
            if (PdhAddEnglishCounterW(query, path, IntPtr.Zero, out counter) == 0)
                return counter;
            return IntPtr.Zero;
        }

        private static float ReadCounterOrNaN(IntPtr counter)
        {
            if (counter == IntPtr.Zero)
                return float.NaN;
            PDH_FMT_COUNTERVALUE value;
            uint res = PdhGetFormattedCounterValue(counter, PDH_FMT_DOUBLE | PDH_FMT_NOCAP100, IntPtr.Zero, out value);
            if (res == 0 && (value.CStatus == PDH_CSTATUS_VALID_DATA || value.CStatus == PDH_CSTATUS_NEW_DATA))
                return (float)value.doubleValue;
            return float.NaN;
        }

        private static void InitSystemLoadQuery()
        {
            _sysLoadInitTried = true;
            IntPtr query;
            if (PdhOpenQueryW(null, IntPtr.Zero, out query) != 0 || query == IntPtr.Zero)
                return;
            _sysCpuCounter = AddCounterOrZero(query, SystemCpuCounterPath);
            _sysDiskIdleCounter = AddCounterOrZero(query, DiskIdleCounterPath);
            _sysPagesInputCounter = AddCounterOrZero(query, PagesInputCounterPath);
            if (_sysCpuCounter == IntPtr.Zero && _sysDiskIdleCounter == IntPtr.Zero && _sysPagesInputCounter == IntPtr.Zero)
            {
                PdhCloseQuery(query);
                return;
            }
            _sysLoadQuery = query;
        }

        public bool TryGetSystemLoadSample(ref GHSystemLoadSample sample)
        {
            lock (_sysLoadLock)
            {
                try
                {
                    bool any = false;
                    long nowTimestamp = Stopwatch.GetTimestamp();

                    try
                    {
                        if (!_sysLoadInitTried)
                            InitSystemLoadQuery();
                        if (_sysLoadQuery != IntPtr.Zero && PdhCollectQueryData(_sysLoadQuery) == 0)
                        {
                            if (_sysLoadHasPrevious)
                            {
                                float cpu = ReadCounterOrNaN(_sysCpuCounter);
                                if (!float.IsNaN(cpu))
                                    sample.SystemCpuPct = Math.Clamp(cpu, 0f, 100f);
                                float idle = ReadCounterOrNaN(_sysDiskIdleCounter);
                                if (!float.IsNaN(idle))
                                    sample.DiskBusyPct = Math.Clamp(100f - idle, 0f, 100f);
                                float faults = ReadCounterOrNaN(_sysPagesInputCounter);
                                if (!float.IsNaN(faults))
                                    sample.HardFaultsPerSec = Math.Max(0f, faults);
                            }
                            _sysLoadHasPrevious = true;
                            any = true;
                        }
                    }
                    catch
                    {
                    }

                    try
                    {
                        long creation, exit, kernel, user;
                        if (GetProcessTimes(GetCurrentProcessHandle(), out creation, out exit, out kernel, out user))
                        {
                            long cpuTicks = kernel + user;
                            if (_ownCpuLastTicks >= 0 && nowTimestamp > _ownCpuLastTimestamp)
                            {
                                double wallTicks = (nowTimestamp - _ownCpuLastTimestamp) * (double)TimeSpan.TicksPerSecond / Stopwatch.Frequency;
                                double capacity = wallTicks * Environment.ProcessorCount;
                                if (capacity > 0)
                                    sample.OwnCpuPct = (float)Math.Clamp((cpuTicks - _ownCpuLastTicks) * 100.0 / capacity, 0.0, 100.0);
                            }
                            _ownCpuLastTicks = cpuTicks;
                            _ownCpuLastTimestamp = nowTimestamp;
                            any = true;
                        }
                    }
                    catch
                    {
                    }

                    try
                    {
                        if (GlobalMemoryStatusEx(_sysLoadMemStatus))
                        {
                            sample.AvailableMemoryPct = 100f - _sysLoadMemStatus.dwMemoryLoad;
                            sample.AvailableMemoryMB = (long)(_sysLoadMemStatus.ullAvailPhys / (1024UL * 1024UL));
                            any = true;
                        }
                    }
                    catch
                    {
                    }

                    sample.TimestampTicks = DateTime.UtcNow.Ticks;
                    return any;
                }
                catch
                {
                    return false;
                }
            }
        }

        /* Per-process interval. A third query with the wildcard process CPU and GPU engine
           counters; PDH averages each instance between the begin and the end collect.
           rows is cleared first. Process CPU is of total logical capacity, this process
           excluded by name; duplicate "name#N" instances are summed. GPU is the 3D engine
           utilization of other processes, summed per adapter (luid); otherGpuPct is the
           busiest adapter. */
        private static readonly object _procLoadLock = new object();
        private static bool _procLoadInitTried = false;
        private static IntPtr _procLoadQuery = IntPtr.Zero;
        private static IntPtr _procCpuCounter = IntPtr.Zero;
        private static IntPtr _procGpuCounter = IntPtr.Zero;
        private static bool _procLoadBegun = false;
        private static string _ownProcessName = null;
        private static int _ownProcessId = -1;

        private static void InitProcessLoadQuery()
        {
            _procLoadInitTried = true;
            try
            {
                using (Process own = Process.GetCurrentProcess())
                {
                    _ownProcessName = own.ProcessName;
                    _ownProcessId = own.Id;
                }
            }
            catch
            {
            }
            IntPtr query;
            if (PdhOpenQueryW(null, IntPtr.Zero, out query) != 0 || query == IntPtr.Zero)
                return;
            _procCpuCounter = AddCounterOrZero(query, ProcessCpuCounterPath);
            _procGpuCounter = AddCounterOrZero(query, GpuEngineCounterPath);
            if (_procCpuCounter == IntPtr.Zero && _procGpuCounter == IntPtr.Zero)
            {
                PdhCloseQuery(query);
                return;
            }
            _procLoadQuery = query;
        }

        /* Reads a wildcard counter as (instance name, value) pairs; false when the counter
           is missing or the read fails. Instances with invalid data are skipped. */
        private static bool ReadCounterArray(IntPtr counter, List<KeyValuePair<string, double>> items)
        {
            items.Clear();
            if (counter == IntPtr.Zero)
                return false;
            uint format = PDH_FMT_DOUBLE | PDH_FMT_NOCAP100;
            uint bufferSize = 0;
            uint itemCount;
            uint res = PdhGetFormattedCounterArrayW(counter, format, ref bufferSize, out itemCount, IntPtr.Zero);
            if (res != PDH_MORE_DATA || bufferSize == 0)
                return res == 0;
            IntPtr buffer = Marshal.AllocHGlobal((int)bufferSize);
            try
            {
                res = PdhGetFormattedCounterArrayW(counter, format, ref bufferSize, out itemCount, buffer);
                if (res != 0)
                    return false;
                int itemSize = Marshal.SizeOf<PDH_FMT_COUNTERVALUE_ITEM_W>();
                int valueOffset = (int)Marshal.OffsetOf<PDH_FMT_COUNTERVALUE_ITEM_W>(nameof(PDH_FMT_COUNTERVALUE_ITEM_W.FmtValue));
                for (int i = 0; i < itemCount; i++)
                {
                    IntPtr item = IntPtr.Add(buffer, i * itemSize);
                    uint status = (uint)Marshal.ReadInt32(item, valueOffset);
                    if (status != PDH_CSTATUS_VALID_DATA && status != PDH_CSTATUS_NEW_DATA)
                        continue;
                    string name = Marshal.PtrToStringUni(Marshal.ReadIntPtr(item));
                    if (string.IsNullOrEmpty(name))
                        continue;
                    double value = BitConverter.Int64BitsToDouble(Marshal.ReadInt64(item, valueOffset + 8));
                    items.Add(new KeyValuePair<string, double>(name, value));
                }
                return true;
            }
            finally
            {
                Marshal.FreeHGlobal(buffer);
            }
        }

        /* The instance name without a trailing "#N" suffix */
        private static string BaseInstanceName(string name)
        {
            int hash = name.LastIndexOf('#');
            if (hash <= 0 || hash == name.Length - 1)
                return name;
            for (int i = hash + 1; i < name.Length; i++)
            {
                if (name[i] < '0' || name[i] > '9')
                    return name;
            }
            return name.Substring(0, hash);
        }

        /* Parses "pid_<n>_luid_<a>_<b>_phys_<p>_eng_<e>_engtype_<type>" */
        private static bool ParseGpuEngineInstance(string name, out int pid, out string luid, out string engType)
        {
            pid = 0;
            luid = null;
            engType = null;
            if (!name.StartsWith("pid_", StringComparison.OrdinalIgnoreCase))
                return false;
            int pidEnd = name.IndexOf('_', 4);
            if (pidEnd < 0 || !int.TryParse(name.AsSpan(4, pidEnd - 4), System.Globalization.NumberStyles.Integer, System.Globalization.CultureInfo.InvariantCulture, out pid))
                return false;
            int luidStart = name.IndexOf("_luid_", pidEnd, StringComparison.OrdinalIgnoreCase);
            int physStart = luidStart < 0 ? -1 : name.IndexOf("_phys_", luidStart, StringComparison.OrdinalIgnoreCase);
            if (luidStart < 0 || physStart < 0)
                return false;
            luid = name.Substring(luidStart + 6, physStart - luidStart - 6);
            int engTypeStart = name.IndexOf("_engtype_", physStart, StringComparison.OrdinalIgnoreCase);
            if (engTypeStart < 0)
                return false;
            engType = name.Substring(engTypeStart + 9);
            return true;
        }

        public bool TryCollectProcessInterval(bool begin, List<GHProcessLoad> rows, out float otherGpuPct)
        {
            otherGpuPct = float.NaN;
            lock (_procLoadLock)
            {
                try
                {
                    if (rows != null)
                        rows.Clear();
                    if (!_procLoadInitTried)
                        InitProcessLoadQuery();
                    if (_procLoadQuery == IntPtr.Zero)
                        return false;
                    if (PdhCollectQueryData(_procLoadQuery) != 0)
                    {
                        _procLoadBegun = false;
                        return false;
                    }
                    if (begin)
                    {
                        _procLoadBegun = true;
                        return true;
                    }
                    if (!_procLoadBegun)
                        return false;
                    _procLoadBegun = false;

                    List<KeyValuePair<string, double>> items = new List<KeyValuePair<string, double>>();
                    int processorCount = Math.Max(1, Environment.ProcessorCount);
                    Dictionary<string, float> cpuByName = new Dictionary<string, float>(StringComparer.OrdinalIgnoreCase);
                    bool cpuOk = ReadCounterArray(_procCpuCounter, items);
                    if (cpuOk)
                    {
                        for (int i = 0; i < items.Count; i++)
                        {
                            string name = BaseInstanceName(items[i].Key);
                            if (string.Equals(name, "_Total", StringComparison.OrdinalIgnoreCase)
                                || string.Equals(name, "Idle", StringComparison.OrdinalIgnoreCase)
                                || (_ownProcessName != null && string.Equals(name, _ownProcessName, StringComparison.OrdinalIgnoreCase)))
                                continue;
                            float cpu = (float)(items[i].Value / processorCount);
                            float sum;
                            cpuByName.TryGetValue(name, out sum);
                            cpuByName[name] = sum + cpu;
                        }
                    }

                    Dictionary<string, float> gpuByLuid = new Dictionary<string, float>(StringComparer.OrdinalIgnoreCase);
                    Dictionary<int, float> gpuByPid = new Dictionary<int, float>();
                    bool gpuOk = ReadCounterArray(_procGpuCounter, items);
                    if (gpuOk)
                    {
                        for (int i = 0; i < items.Count; i++)
                        {
                            int pid;
                            string luid, engType;
                            if (!ParseGpuEngineInstance(items[i].Key, out pid, out luid, out engType))
                                continue;
                            if (pid == _ownProcessId || !string.Equals(engType, "3D", StringComparison.OrdinalIgnoreCase))
                                continue;
                            float value = (float)items[i].Value;
                            float sum;
                            gpuByLuid.TryGetValue(luid, out sum);
                            gpuByLuid[luid] = sum + value;
                            gpuByPid.TryGetValue(pid, out sum);
                            gpuByPid[pid] = sum + value;
                        }
                        float max = 0f;
                        foreach (KeyValuePair<string, float> kv in gpuByLuid)
                            max = Math.Max(max, kv.Value);
                        otherGpuPct = max;
                    }

                    if (!cpuOk && !gpuOk)
                        return false;
                    if (rows == null)
                        return true;

                    List<KeyValuePair<string, float>> cpuRows = new List<KeyValuePair<string, float>>(cpuByName);
                    cpuRows.Sort((x, y) => y.Value.CompareTo(x.Value));
                    float defaultGpu = gpuOk ? 0f : float.NaN;
                    for (int i = 0; i < cpuRows.Count && i < MaxProcessCpuRows; i++)
                        rows.Add(new GHProcessLoad(cpuRows[i].Key, cpuRows[i].Value, defaultGpu));

                    if (gpuOk)
                    {
                        List<KeyValuePair<int, float>> gpuPids = new List<KeyValuePair<int, float>>(gpuByPid);
                        gpuPids.Sort((x, y) => y.Value.CompareTo(x.Value));
                        int named = 0;
                        for (int i = 0; i < gpuPids.Count && named < MaxProcessGpuRows; i++)
                        {
                            if (gpuPids[i].Value <= 0f)
                                break;
                            string name = null;
                            try
                            {
                                using (Process p = Process.GetProcessById(gpuPids[i].Key))
                                {
                                    name = p.ProcessName;
                                }
                            }
                            catch
                            {
                                name = null;
                            }
                            if (string.IsNullOrEmpty(name))
                                continue;
                            named++;
                            int existing = -1;
                            for (int j = 0; j < rows.Count; j++)
                            {
                                if (string.Equals(rows[j].Name, name, StringComparison.OrdinalIgnoreCase))
                                {
                                    existing = j;
                                    break;
                                }
                            }
                            if (existing >= 0)
                            {
                                GHProcessLoad row = rows[existing];
                                row.GpuPct = (float.IsNaN(row.GpuPct) ? 0f : row.GpuPct) + gpuPids[i].Value;
                                rows[existing] = row;
                            }
                            else
                            {
                                float cpu;
                                if (!cpuByName.TryGetValue(name, out cpu))
                                    cpu = cpuOk ? 0f : float.NaN;
                                rows.Add(new GHProcessLoad(name, cpu, gpuPids[i].Value));
                            }
                        }
                    }
                    return true;
                }
                catch
                {
                    _procLoadBegun = false;
                    return false;
                }
            }
        }

        /* Environment fingerprint. Registry and WMI values are cached until refresh;
           pending reboot and the power scheme are read on every call. */
        private static readonly object _fingerprintLock = new object();
        private static Dictionary<string, string> _cachedStaticFingerprint = null;
        private static System.Threading.Tasks.Task<Dictionary<string, string>> _wmiTask = null;
        private static Dictionary<string, string> _cachedWmiFingerprint = null;
        private const int WmiTimeoutMs = 5000;

        private static readonly Guid PowerSchemeBalanced = new Guid("381b4222-f694-41f0-9685-ff5bb260df2e");
        private static readonly Guid PowerSchemeHighPerformance = new Guid("8c5e7fda-e8bf-4a96-9a85-a6e23a8c635c");
        private static readonly Guid PowerSchemePowerSaver = new Guid("a1841308-3541-4fab-bc81-f71556f20b4a");
        private static readonly Guid PowerSchemeUltimatePerformance = new Guid("e9a42b02-d5df-448d-aa00-03f14749eb61");
        private static readonly Guid PowerOverlayBestEfficiency = new Guid("961cc777-2547-4f9d-8174-7d86181b8a7a");
        private static readonly Guid PowerOverlayBetterPerformance = new Guid("3af9b8d9-7c97-431d-ad78-34a8bfea439f");
        private static readonly Guid PowerOverlayBestPerformance = new Guid("ded574b5-45a0-4f42-8737-46345c09c238");

        private static void PutFingerprint(Dictionary<string, string> fingerprint, string key, string value)
        {
            if (!string.IsNullOrWhiteSpace(value))
                fingerprint[key] = value.Trim();
        }

        private static global::Microsoft.Win32.RegistryKey OpenLocalMachine()
        {
            return global::Microsoft.Win32.RegistryKey.OpenBaseKey(global::Microsoft.Win32.RegistryHive.LocalMachine,
                Environment.Is64BitOperatingSystem ? global::Microsoft.Win32.RegistryView.Registry64 : global::Microsoft.Win32.RegistryView.Default);
        }

        private static bool LocalMachineKeyExists(string path)
        {
            try
            {
                using (global::Microsoft.Win32.RegistryKey root = OpenLocalMachine())
                using (global::Microsoft.Win32.RegistryKey key = root.OpenSubKey(path))
                {
                    return key != null;
                }
            }
            catch
            {
                return false;
            }
        }

        private static Dictionary<string, string> ReadStaticFingerprint()
        {
            Dictionary<string, string> d = new Dictionary<string, string>();
            try
            {
                using (global::Microsoft.Win32.RegistryKey root = OpenLocalMachine())
                {
                    try
                    {
                        using (global::Microsoft.Win32.RegistryKey key = root.OpenSubKey(@"SOFTWARE\Microsoft\Windows NT\CurrentVersion"))
                        {
                            if (key != null)
                            {
                                string build = key.GetValue("CurrentBuild") as string;
                                object ubr = key.GetValue("UBR");
                                if (!string.IsNullOrEmpty(build))
                                    PutFingerprint(d, "os.build", ubr != null ? build + "." + GHEnvironmentFingerprint.FormatValue(ubr) : build);
                                PutFingerprint(d, "os.displayVersion", key.GetValue("DisplayVersion") as string);
                                PutFingerprint(d, "os.edition", key.GetValue("EditionID") as string);
                            }
                        }
                    }
                    catch
                    {
                    }

                    try
                    {
                        using (global::Microsoft.Win32.RegistryKey key = root.OpenSubKey(@"HARDWARE\DESCRIPTION\System\CentralProcessor\0"))
                        {
                            if (key != null)
                                PutFingerprint(d, "hardware.cpu", key.GetValue("ProcessorNameString") as string);
                        }
                    }
                    catch
                    {
                    }
                }
            }
            catch
            {
            }

            PutFingerprint(d, "hardware.logicalProcessors", GHEnvironmentFingerprint.FormatValue(Environment.ProcessorCount));
            try
            {
                MEMORYSTATUSEX memStatus = new MEMORYSTATUSEX();
                if (GlobalMemoryStatusEx(memStatus) && memStatus.ullTotalPhys > 0)
                    PutFingerprint(d, "hardware.memoryGB", (memStatus.ullTotalPhys / (1024.0 * 1024.0 * 1024.0)).ToString("F1", System.Globalization.CultureInfo.InvariantCulture));
            }
            catch
            {
            }

            try
            {
                PutFingerprint(d, "component.windowsAppSdk", global::Microsoft.Windows.ApplicationModel.WindowsAppRuntime.ReleaseInfo.AsString);
            }
            catch
            {
            }
            try
            {
                System.Reflection.AssemblyFileVersionAttribute winUiVersion = (System.Reflection.AssemblyFileVersionAttribute)Attribute.GetCustomAttribute(
                    typeof(global::Microsoft.UI.Xaml.Application).Assembly, typeof(System.Reflection.AssemblyFileVersionAttribute));
                if (winUiVersion != null)
                    PutFingerprint(d, "component.winui", winUiVersion.Version);
            }
            catch
            {
            }
            return d;
        }

        /* "yyyy-MM-dd" from a CIM datetime ("yyyyMMddHHmmss.ffffff+zzz"); the raw value
           when it does not have that shape */
        private static string FormatCimDate(string cim)
        {
            if (string.IsNullOrEmpty(cim) || cim.Length < 8)
                return cim;
            for (int i = 0; i < 8; i++)
            {
                if (cim[i] < '0' || cim[i] > '9')
                    return cim;
            }
            return cim.Substring(0, 4) + "-" + cim.Substring(4, 2) + "-" + cim.Substring(6, 2);
        }

        private static Dictionary<string, string> ReadWmiFingerprint()
        {
            Dictionary<string, string> d = new Dictionary<string, string>();
            try
            {
                using (global::System.Management.ManagementObjectSearcher searcher = new global::System.Management.ManagementObjectSearcher(
                    "SELECT Name, DriverVersion, DriverDate FROM Win32_VideoController"))
                using (global::System.Management.ManagementObjectCollection results = searcher.Get())
                {
                    int n = 0;
                    foreach (global::System.Management.ManagementBaseObject mo in results)
                    {
                        using (mo)
                        {
                            string prefix = "gpu" + n.ToString(System.Globalization.CultureInfo.InvariantCulture);
                            PutFingerprint(d, "hardware." + prefix, mo["Name"] as string);
                            PutFingerprint(d, "driver." + prefix + ".version", mo["DriverVersion"] as string);
                            PutFingerprint(d, "driver." + prefix + ".date", FormatCimDate(mo["DriverDate"] as string));
                        }
                        n++;
                    }
                }
            }
            catch
            {
            }
            return d;
        }

        /* Caller holds _fingerprintLock */
        private static void HarvestWmiFingerprint()
        {
            if (_wmiTask != null && _wmiTask.Status == System.Threading.Tasks.TaskStatus.RanToCompletion && _wmiTask.Result != null)
                _cachedWmiFingerprint = _wmiTask.Result;
        }

        private static string PowerSchemeName(Guid scheme)
        {
            if (scheme == PowerSchemeBalanced)
                return "Balanced";
            if (scheme == PowerSchemeHighPerformance)
                return "High performance";
            if (scheme == PowerSchemePowerSaver)
                return "Power saver";
            if (scheme == PowerSchemeUltimatePerformance)
                return "Ultimate Performance";
            return scheme.ToString("D");
        }

        private static string PowerOverlayName(Guid overlay)
        {
            if (overlay == Guid.Empty)
                return "Balanced";
            if (overlay == PowerOverlayBestEfficiency)
                return "Best power efficiency";
            if (overlay == PowerOverlayBetterPerformance)
                return "Better performance";
            if (overlay == PowerOverlayBestPerformance)
                return "Best performance";
            return overlay.ToString("D");
        }

        private static void AddPowerFingerprint(Dictionary<string, string> fingerprint)
        {
            try
            {
                IntPtr schemePtr;
                if (PowerGetActiveScheme(IntPtr.Zero, out schemePtr) == 0 && schemePtr != IntPtr.Zero)
                {
                    try
                    {
                        Guid scheme = Marshal.PtrToStructure<Guid>(schemePtr);
                        PutFingerprint(fingerprint, "settings.powerPlan", PowerSchemeName(scheme));
                    }
                    finally
                    {
                        LocalFree(schemePtr);
                    }
                }
            }
            catch
            {
            }

            try
            {
                Guid overlay;
                if (PowerGetEffectiveOverlayScheme(out overlay) == 0)
                    PutFingerprint(fingerprint, "settings.powerMode", PowerOverlayName(overlay));
            }
            catch (EntryPointNotFoundException)
            {
                /* Older Windows builds have no power mode overlay */
            }
            catch
            {
            }
        }

        public void AddEnvironmentFingerprint(Dictionary<string, string> fingerprint, bool refresh)
        {
            if (fingerprint == null)
                return;
            try
            {
                System.Threading.Tasks.Task<Dictionary<string, string>> wmiTask = null;
                bool waitForWmi = false;
                Dictionary<string, string> staticPart;
                lock (_fingerprintLock)
                {
                    if (refresh || _cachedStaticFingerprint == null)
                        _cachedStaticFingerprint = ReadStaticFingerprint();
                    staticPart = _cachedStaticFingerprint;

                    HarvestWmiFingerprint();
                    if (_wmiTask == null || (refresh && _wmiTask.IsCompleted))
                    {
                        _wmiTask = System.Threading.Tasks.Task.Run(() => ReadWmiFingerprint());
                        waitForWmi = true;
                    }
                    else if (refresh)
                    {
                        waitForWmi = true;
                    }
                    wmiTask = _wmiTask;
                }

                /* A call that starts or refreshes the query waits for it; others take the
                   last finished result without blocking */
                if (waitForWmi)
                {
                    try
                    {
                        wmiTask.Wait(WmiTimeoutMs);
                    }
                    catch
                    {
                    }
                }

                lock (_fingerprintLock)
                {
                    HarvestWmiFingerprint();
                    foreach (KeyValuePair<string, string> kv in staticPart)
                        fingerprint[kv.Key] = kv.Value;
                    if (_cachedWmiFingerprint != null)
                    {
                        foreach (KeyValuePair<string, string> kv in _cachedWmiFingerprint)
                            fingerprint[kv.Key] = kv.Value;
                    }
                }

                fingerprint["os.pendingReboot"] = GHEnvironmentFingerprint.FormatValue(
                    LocalMachineKeyExists(@"SOFTWARE\Microsoft\Windows\CurrentVersion\WindowsUpdate\Auto Update\RebootRequired")
                    || LocalMachineKeyExists(@"SOFTWARE\Microsoft\Windows\CurrentVersion\Component Based Servicing\RebootPending"));
                AddPowerFingerprint(fingerprint);
                AddGpuFingerprint(fingerprint);
            }
            catch
            {
            }
        }

        /* Read on every call: settings.gpuPreference, the app's Windows graphics preference
           (GHApp.GetActiveGPU), and hardware.renderAdapter, the adapter the render adapter
           probe last found this process rendering on; absent before the process's first
           successful probe */
        private static void AddGpuFingerprint(Dictionary<string, string> fingerprint)
        {
            try
            {
                PutFingerprint(fingerprint, "settings.gpuPreference", GHApp.GetActiveGPU());
            }
            catch
            {
            }

            try
            {
                GHRenderAdapter render;
                if (GHRenderAdapterProbeWindows.TryGetLast(out render, null))
                    PutFingerprint(fingerprint, "hardware.renderAdapter", GHRenderAdapterProbeWindows.FingerprintValue(render));
            }
            catch
            {
            }
        }

    }

    public class MemoryStatus
    {
        /// <summary>
        /// The amount of actual physical memory, in bytes.
        /// </summary>
        public ulong TotalPhysical { get; set; }

        /// <summary>
        /// The amount of physical memory currently available, in bytes. 
        /// This is the amount of physical memory that can be immediately reused without having to write its contents to disk first. 
        /// It is the sum of the size of the standby, free, and zero lists.
        /// </summary>
        public ulong AvailablePhysical { get; set; }

        /// <summary>
        /// The current committed memory limit for the system or the current process, whichever is smaller, in bytes.
        /// </summary>
        public ulong TotalPageFile { get; set; }

        /// <summary>
        /// The maximum amount of memory the current process can commit, in bytes. 
        /// This value is equal to or smaller than the system-wide available commit value.
        /// </summary>
        public ulong AvailablePageFile { get; set; }

        /// <summary>
        /// The size of the user-mode portion of the virtual address space of the calling process, in bytes. 
        /// This value depends on the type of process, the type of processor, and the configuration of the operating system.
        /// </summary>
        public ulong TotalVirtual { get; set; }

        /// <summary>
        /// The amount of unreserved and uncommitted memory currently in the user-mode portion of the virtual address space of the calling process, in bytes.
        /// </summary>
        public ulong AvailableVirtual { get; set; }

        /// <summary>
        /// Reserved. This value is always 0.
        /// </summary>
        public ulong AvailableExtendedVirtual { get; set; }

        /// <summary>
        /// Write all property values to a string
        /// </summary>
        /// <returns>Each property on a new line</returns>
        public override string ToString()
        {
            return
                "TotalPhysical: " + TotalPhysical + Environment.NewLine +
                "AvailablePhysical: " + AvailablePhysical + Environment.NewLine +
                "TotalPageFile: " + TotalPageFile + Environment.NewLine +
                "AvailablePageFile: " + AvailablePageFile + Environment.NewLine +
                "TotalVirtual: " + TotalVirtual + Environment.NewLine +
                "AvailableVirtual: " + AvailableVirtual + Environment.NewLine +
                "AvailableExtendedVirtual: " + AvailableExtendedVirtual + Environment.NewLine;
        }
    }
}
#endif