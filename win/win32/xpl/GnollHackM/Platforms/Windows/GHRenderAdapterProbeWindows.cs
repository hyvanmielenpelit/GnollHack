#if WINDOWS && GNH_MAUI
using System;
using System.Collections.Generic;
using System.Globalization;
using System.Runtime.InteropServices;
using GnollHackX;

namespace GnollHackM
{
    /* One DXGI adapter. Luid is ((long)HighPart << 32) | LowPart, the value PDH writes as
       "luid_0x<HighPart>_0x<LowPart>" in GPU Engine instance names. */
    public sealed class GHRenderAdapter
    {
        public string Name;
        public long Luid;
        public bool IsSoftware;
        public bool? IsIntegrated;
        public long DedicatedVideoMemoryMB;
    }

    /* The adapter this process renders on. Begin opens a PDH query on this process's GPU
       engine instances and takes the first sample; End takes the second, sums the 3D engine
       utilization per adapter LUID, and names the busiest adapter through DXGI. Both run on
       a thread-pool thread, never on the UI thread, and never throw: every failure returns
       false. Begin while already begun restarts the interval. */
    public static class GHRenderAdapterProbeWindows
    {
        [StructLayout(LayoutKind.Explicit, Size = 16)]
        private struct PDH_FMT_COUNTERVALUE
        {
            [FieldOffset(0)]
            public uint CStatus;
            [FieldOffset(8)]
            public double doubleValue;
        }

        [StructLayout(LayoutKind.Sequential)]
        private struct PDH_FMT_COUNTERVALUE_ITEM_W
        {
            public IntPtr szName;
            public PDH_FMT_COUNTERVALUE FmtValue;
        }

        [DllImport("pdh.dll", CharSet = CharSet.Unicode)]
        private static extern uint PdhOpenQueryW(string szDataSource, IntPtr dwUserData, out IntPtr phQuery);

        [DllImport("pdh.dll", CharSet = CharSet.Unicode)]
        private static extern uint PdhAddEnglishCounterW(IntPtr hQuery, string szFullCounterPath, IntPtr dwUserData, out IntPtr phCounter);

        [DllImport("pdh.dll")]
        private static extern uint PdhCollectQueryData(IntPtr hQuery);

        [DllImport("pdh.dll", CharSet = CharSet.Unicode)]
        private static extern uint PdhGetFormattedCounterArrayW(IntPtr hCounter, uint dwFormat, ref uint lpdwBufferSize, out uint lpdwItemCount, IntPtr ItemBuffer);

        [DllImport("pdh.dll")]
        private static extern uint PdhCloseQuery(IntPtr hQuery);

        private const uint PDH_FMT_DOUBLE = 0x00000200;
        private const uint PDH_FMT_NOCAP100 = 0x00008000;
        private const uint PDH_CSTATUS_VALID_DATA = 0x00000000;
        private const uint PDH_CSTATUS_NEW_DATA = 0x00000001;
        private const uint PDH_MORE_DATA = 0x800007D2;
        private const string AllGpuEngineCounterPath = @"\GPU Engine(*)\Utilization Percentage";

        /* DXGI through raw vtable calls. Slot numbers follow dxgi.h: IUnknown 0-2,
           IDXGIObject 3-6 (SetPrivateData, SetPrivateDataInterface, GetPrivateData,
           GetParent), IDXGIFactory 7-11 (EnumAdapters, MakeWindowAssociation,
           GetWindowAssociation, CreateSwapChain, CreateSoftwareAdapter), IDXGIFactory1 12-13
           (EnumAdapters1, IsCurrent); IDXGIAdapter 7-9 (EnumOutputs, GetDesc,
           CheckInterfaceSupport), IDXGIAdapter1 10 (GetDesc1). */
        [StructLayout(LayoutKind.Sequential)]
        private unsafe struct DXGI_ADAPTER_DESC1
        {
            public fixed ushort Description[128];
            public uint VendorId;
            public uint DeviceId;
            public uint SubSysId;
            public uint Revision;
            public nuint DedicatedVideoMemory;
            public nuint DedicatedSystemMemory;
            public nuint SharedSystemMemory;
            public uint AdapterLuidLowPart;
            public int AdapterLuidHighPart;
            public uint Flags;
        }

        [DllImport("dxgi.dll")]
        private static extern int CreateDXGIFactory1(ref Guid riid, out IntPtr ppFactory);

        private static readonly Guid IID_IDXGIFactory1 = new Guid("770aae78-f26f-4dba-a829-253c83d1b387");
        private const int SlotFactory1EnumAdapters1 = 12;
        private const int SlotAdapter1GetDesc1 = 10;
        private const int DXGI_ERROR_NOT_FOUND = unchecked((int)0x887A0002);
        private const uint DXGI_ADAPTER_FLAG_SOFTWARE = 2;
        private const int AdapterDescriptionLength = 128;
        private const uint MaxAdapters = 64;
        private const long IntegratedMaxDedicatedMB = 512;

        private static readonly object _lock = new object();
        private static IntPtr _query = IntPtr.Zero;
        private static IntPtr _counter = IntPtr.Zero;
        private static int _ownPid = -1;
        private static bool _begun = false;

        public static bool Begin()
        {
            lock (_lock)
            {
                try
                {
                    CloseQuery();
                    int pid = Environment.ProcessId;
                    IntPtr query;
                    if (PdhOpenQueryW(null, IntPtr.Zero, out query) != 0 || query == IntPtr.Zero)
                        return false;
                    _query = query;

                    /* The own-process wildcard; the full wildcard only if that path is refused.
                       End filters by pid either way. */
                    string ownPath = @"\GPU Engine(pid_" + pid.ToString(CultureInfo.InvariantCulture) + @"_*)\Utilization Percentage";
                    IntPtr counter;
                    if (PdhAddEnglishCounterW(query, ownPath, IntPtr.Zero, out counter) != 0 || counter == IntPtr.Zero)
                    {
                        if (PdhAddEnglishCounterW(query, AllGpuEngineCounterPath, IntPtr.Zero, out counter) != 0 || counter == IntPtr.Zero)
                        {
                            CloseQuery();
                            return false;
                        }
                    }
                    _counter = counter;
                    _ownPid = pid;

                    if (PdhCollectQueryData(query) != 0)
                    {
                        CloseQuery();
                        return false;
                    }
                    _begun = true;
                    return true;
                }
                catch
                {
                    CloseQuery();
                    return false;
                }
            }
        }

        /* all is cleared and filled with every DXGI adapter, software ones included, even
           when the render adapter is unknown. render is the adapter with the most 3D engine
           utilization by this process over the interval; false and null when there was none
           or it is not among the DXGI adapters. The query is closed in every case. */
        public static bool End(out GHRenderAdapter render, List<GHRenderAdapter> all)
        {
            render = null;
            lock (_lock)
            {
                try
                {
                    if (all != null)
                        all.Clear();

                    long renderLuid = 0;
                    bool haveLuid = false;
                    try
                    {
                        haveLuid = _begun && TryReadBusiestOwnLuid(out renderLuid);
                    }
                    catch
                    {
                        haveLuid = false;
                    }
                    finally
                    {
                        CloseQuery();
                    }

                    List<GHRenderAdapter> adapters = all ?? new List<GHRenderAdapter>();
                    bool enumerated;
                    try
                    {
                        enumerated = EnumerateAdapters(adapters);
                    }
                    catch
                    {
                        enumerated = false;
                    }

                    if (!haveLuid || !enumerated)
                        return false;
                    for (int i = 0; i < adapters.Count; i++)
                    {
                        if (adapters[i].Luid == renderLuid)
                        {
                            render = adapters[i];
                            return true;
                        }
                    }
                    return false;
                }
                catch
                {
                    render = null;
                    return false;
                }
            }
        }

        private static void CloseQuery()
        {
            IntPtr query = _query;
            _query = IntPtr.Zero;
            _counter = IntPtr.Zero;
            _begun = false;
            if (query != IntPtr.Zero)
            {
                try
                {
                    PdhCloseQuery(query);
                }
                catch
                {
                }
            }
        }

        /* Second collect; sums this process's engtype_3D utilization per LUID. False when
           no LUID has any 3D utilization. */
        private static bool TryReadBusiestOwnLuid(out long luid)
        {
            luid = 0;
            if (_query == IntPtr.Zero || _counter == IntPtr.Zero)
                return false;
            if (PdhCollectQueryData(_query) != 0)
                return false;

            List<KeyValuePair<string, double>> items = new List<KeyValuePair<string, double>>();
            if (!ReadCounterArray(_counter, items))
                return false;

            Dictionary<long, double> byLuid = new Dictionary<long, double>();
            for (int i = 0; i < items.Count; i++)
            {
                int pid;
                string luidText, engType;
                if (!ParseGpuEngineInstance(items[i].Key, out pid, out luidText, out engType))
                    continue;
                if (pid != _ownPid || !string.Equals(engType, "3D", StringComparison.OrdinalIgnoreCase))
                    continue;
                long instanceLuid;
                if (!TryParseLuid(luidText, out instanceLuid))
                    continue;
                double value = items[i].Value;
                if (double.IsNaN(value) || value <= 0)
                    continue;
                double sum;
                byLuid.TryGetValue(instanceLuid, out sum);
                byLuid[instanceLuid] = sum + value;
            }

            double best = 0;
            bool found = false;
            foreach (KeyValuePair<long, double> kv in byLuid)
            {
                if (kv.Value > best)
                {
                    best = kv.Value;
                    luid = kv.Key;
                    found = true;
                }
            }
            return found;
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

        /* Parses "pid_<n>_luid_<a>_<b>_phys_<p>_eng_<e>_engtype_<type>" */
        private static bool ParseGpuEngineInstance(string name, out int pid, out string luid, out string engType)
        {
            pid = 0;
            luid = null;
            engType = null;
            if (!name.StartsWith("pid_", StringComparison.OrdinalIgnoreCase))
                return false;
            int pidEnd = name.IndexOf('_', 4);
            if (pidEnd < 0 || !int.TryParse(name.AsSpan(4, pidEnd - 4), NumberStyles.Integer, CultureInfo.InvariantCulture, out pid))
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

        /* Parses "0x<HighPart>_0x<LowPart>", both hexadecimal */
        private static bool TryParseLuid(string text, out long luid)
        {
            luid = 0;
            if (string.IsNullOrEmpty(text))
                return false;
            int separator = text.IndexOf('_');
            if (separator < 0)
                return false;
            uint high, low;
            if (!TryParseHex(text.AsSpan(0, separator), out high) || !TryParseHex(text.AsSpan(separator + 1), out low))
                return false;
            luid = MakeLuid(low, unchecked((int)high));
            return true;
        }

        private static bool TryParseHex(ReadOnlySpan<char> text, out uint value)
        {
            value = 0;
            if (text.Length < 3 || text[0] != '0' || (text[1] != 'x' && text[1] != 'X'))
                return false;
            return uint.TryParse(text.Slice(2), NumberStyles.AllowHexSpecifier, CultureInfo.InvariantCulture, out value);
        }

        private static long MakeLuid(uint lowPart, int highPart)
        {
            return ((long)highPart << 32) | lowPart;
        }

        /* Appends one GHRenderAdapter per DXGI adapter; false when the factory cannot be
           created or the descriptor layout does not match this process's pointer size. */
        private static unsafe bool EnumerateAdapters(List<GHRenderAdapter> adapters)
        {
            if (sizeof(DXGI_ADAPTER_DESC1) != (IntPtr.Size == 8 ? 312 : 296))
                return false;

            DeviceGPU[] gpus = SnapshotDeviceGpus();
            IntPtr factory = IntPtr.Zero;
            try
            {
                Guid iid = IID_IDXGIFactory1;
                if (CreateDXGIFactory1(ref iid, out factory) < 0 || factory == IntPtr.Zero)
                    return false;
                delegate* unmanaged[Stdcall]<IntPtr, uint, IntPtr*, int> enumAdapters1 =
                    (delegate* unmanaged[Stdcall]<IntPtr, uint, IntPtr*, int>)VtableSlot(factory, SlotFactory1EnumAdapters1);

                for (uint i = 0; i < MaxAdapters; i++)
                {
                    IntPtr adapter = IntPtr.Zero;
                    try
                    {
                        int hr = enumAdapters1(factory, i, &adapter);
                        if (hr == DXGI_ERROR_NOT_FOUND || hr < 0 || adapter == IntPtr.Zero)
                            break;
                        delegate* unmanaged[Stdcall]<IntPtr, DXGI_ADAPTER_DESC1*, int> getDesc1 =
                            (delegate* unmanaged[Stdcall]<IntPtr, DXGI_ADAPTER_DESC1*, int>)VtableSlot(adapter, SlotAdapter1GetDesc1);
                        DXGI_ADAPTER_DESC1 desc = default;
                        if (getDesc1(adapter, &desc) < 0)
                            continue;
                        adapters.Add(MakeAdapter(&desc, gpus));
                    }
                    finally
                    {
                        if (adapter != IntPtr.Zero)
                            Marshal.Release(adapter);
                    }
                }
                return true;
            }
            finally
            {
                if (factory != IntPtr.Zero)
                    Marshal.Release(factory);
            }
        }

        private static unsafe void* VtableSlot(IntPtr comObject, int slot)
        {
            void** vtable = *(void***)comObject;
            return vtable[slot];
        }

        /* IsIntegrated comes from the WMI adapter list (GHApp.DeviceGPUs) when a description
           matches; otherwise a hardware adapter with under 512 MB of dedicated memory counts
           as integrated. */
        private static unsafe GHRenderAdapter MakeAdapter(DXGI_ADAPTER_DESC1* desc, DeviceGPU[] gpus)
        {
            int length = 0;
            while (length < AdapterDescriptionLength && desc->Description[length] != 0)
                length++;
            string name = new string((char*)desc->Description, 0, length).Trim();

            GHRenderAdapter adapter = new GHRenderAdapter();
            adapter.Name = name;
            adapter.Luid = MakeLuid(desc->AdapterLuidLowPart, desc->AdapterLuidHighPart);
            adapter.IsSoftware = (desc->Flags & DXGI_ADAPTER_FLAG_SOFTWARE) != 0;
            adapter.DedicatedVideoMemoryMB = (long)((ulong)desc->DedicatedVideoMemory / (1024UL * 1024UL));

            bool? integrated = null;
            if (name.Length > 0)
            {
                for (int i = 0; i < gpus.Length; i++)
                {
                    DeviceGPU gpu = gpus[i];
                    if (gpu != null && gpu.Description != null && string.Equals(gpu.Description.Trim(), name, StringComparison.OrdinalIgnoreCase))
                    {
                        integrated = gpu.IsIntegratedGraphics;
                        break;
                    }
                }
            }
            if (integrated == null)
                integrated = !adapter.IsSoftware && adapter.DedicatedVideoMemoryMB < IntegratedMaxDedicatedMB;
            adapter.IsIntegrated = integrated;
            return adapter;
        }

        /* GHApp.DeviceGPUs has no lock; it is filled once at startup */
        private static DeviceGPU[] SnapshotDeviceGpus()
        {
            try
            {
                return GHApp.DeviceGPUs.ToArray();
            }
            catch
            {
                return Array.Empty<DeviceGPU>();
            }
        }
    }
}
#endif
