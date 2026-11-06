#if WINDOWS && GNH_MAUI
using System;
using System.Collections.Generic;
using System.Globalization;
using System.Runtime.InteropServices;
using GnollHackX;

namespace GnollHackM
{
    /* The adapter this process renders on. Begin opens a PDH query on this process's GPU
       engine instances and takes the first sample; End takes the second, sums the 3D engine
       utilization per adapter LUID, and names the busiest adapter through DXGI. Both run on
       a thread-pool thread, never on the UI thread, and never throw: every failure returns
       false. Begin while already begun restarts the interval. A successful End is kept
       as the process's last result, which TryGetLast hands out on any thread; the render
       adapter cannot change within a process. */
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

        private static readonly object _lock = new object();
        private static IntPtr _query = IntPtr.Zero;
        private static IntPtr _counter = IntPtr.Zero;
        private static int _ownPid = -1;
        private static bool _begun = false;

        /* The last successful End's render adapter and adapter list; null and empty until
           an End has succeeded. Their own lock, so a reader never waits for a PDH collect
           or a DXGI enumeration. */
        private static readonly object _lastLock = new object();
        private static GHRenderAdapter _lastRender = null;
        private static List<GHRenderAdapter> _lastAdapters = new List<GHRenderAdapter>();

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
           or it is not among the DXGI adapters. The query is closed in every case. A true
           result also replaces the last result TryGetLast reads, with copies. */
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
                        enumerated = GHDxgiAdapters.Enumerate(adapters, SnapshotDeviceGpus());
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
                            List<GHRenderAdapter> copies = new List<GHRenderAdapter>(adapters.Count);
                            for (int j = 0; j < adapters.Count; j++)
                                copies.Add(GHDxgiAdapters.Copy(adapters[j]));
                            GHRenderAdapter renderCopy = GHDxgiAdapters.Copy(adapters[i]);
                            lock (_lastLock)
                            {
                                _lastRender = renderCopy;
                                _lastAdapters = copies;
                            }
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

        /* The last successful End's result, as copies: render, and all (when non-null)
           cleared and filled with every DXGI adapter of that End. False, with render null
           and all empty, until an End has succeeded. Any thread; never throws. */
        public static bool TryGetLast(out GHRenderAdapter render, List<GHRenderAdapter> all)
        {
            render = null;
            try
            {
                if (all != null)
                    all.Clear();
                lock (_lastLock)
                {
                    if (_lastRender == null)
                        return false;
                    if (all != null)
                    {
                        for (int i = 0; i < _lastAdapters.Count; i++)
                            all.Add(GHDxgiAdapters.Copy(_lastAdapters[i]));
                    }
                    render = GHDxgiAdapters.Copy(_lastRender);
                    return true;
                }
            }
            catch
            {
                render = null;
                return false;
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
            luid = GHDxgiAdapters.MakeLuid(low, unchecked((int)high));
            return true;
        }

        private static bool TryParseHex(ReadOnlySpan<char> text, out uint value)
        {
            value = 0;
            if (text.Length < 3 || text[0] != '0' || (text[1] != 'x' && text[1] != 'X'))
                return false;
            return uint.TryParse(text.Slice(2), NumberStyles.AllowHexSpecifier, CultureInfo.InvariantCulture, out value);
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
