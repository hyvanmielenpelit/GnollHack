#if WINDOWS && GNH_MAUI && SENTRY
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace GnollHackM
{
    /* One hardware adapter's video memory as seen by this process. Budget and CurrentUsage are
       per process; usage is non-zero only on adapters where this process has devices, and it can
       exceed the budget. On a discrete adapter the local group is its VRAM and the non-local group
       system RAM; on an integrated (UMA) adapter all memory is in the local group and the
       non-local budget is 0. */
    internal struct VideoMemorySample
    {
        public string Name;                 /* adapter description, trimmed */
        public long Luid;                   /* ((long)HighPart << 32) | LowPart, as in GHRenderAdapter */
        public ulong LocalBudgetBytes;
        public ulong LocalUsageBytes;
        public ulong NonLocalBudgetBytes;
        public ulong NonLocalUsageBytes;
    }

    /* Per-process video memory through IDXGIAdapter3::QueryVideoMemoryInfo. The DXGI factory and
       one IDXGIAdapter3 per hardware adapter are cached for the process lifetime. The cache is
       kept while IDXGIFactory1::IsCurrent reports the adapter list unchanged and no query has
       failed; otherwise the next sample rebuilds it. Once built, a sample allocates nothing here:
       DXGI writes into stack structs and names are the cached strings. */
    internal static class VideoMemoryWindows
    {
        /* DXGI through raw vtable calls. Slot numbers follow dxgi.h, dxgi1_2.h and dxgi1_4.h:
           IUnknown 0-2, IDXGIObject 3-6 (SetPrivateData, SetPrivateDataInterface,
           GetPrivateData, GetParent), IDXGIFactory 7-11 (EnumAdapters, MakeWindowAssociation,
           GetWindowAssociation, CreateSwapChain, CreateSoftwareAdapter), IDXGIFactory1 12-13
           (EnumAdapters1, IsCurrent); IDXGIAdapter 7-9 (EnumOutputs, GetDesc,
           CheckInterfaceSupport), IDXGIAdapter1 10 (GetDesc1), IDXGIAdapter2 11 (GetDesc2),
           IDXGIAdapter3 12-17 (RegisterHardwareContentProtectionTeardownStatusEvent,
           UnregisterHardwareContentProtectionTeardownStatus, QueryVideoMemoryInfo,
           SetVideoMemoryReservation, RegisterVideoMemoryBudgetChangeNotificationEvent,
           UnregisterVideoMemoryBudgetChangeNotification). */
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

        [StructLayout(LayoutKind.Sequential)]
        private struct DXGI_QUERY_VIDEO_MEMORY_INFO
        {
            public ulong Budget;
            public ulong CurrentUsage;
            public ulong AvailableForReservation;
            public ulong CurrentReservation;
        }

        [DllImport("dxgi.dll")]
        private static extern int CreateDXGIFactory1(ref Guid riid, out IntPtr ppFactory);

        private static readonly Guid IID_IDXGIFactory1 = new Guid("770aae78-f26f-4dba-a829-253c83d1b387");
        private static readonly Guid IID_IDXGIAdapter3 = new Guid("645967A4-1392-4310-A798-8053CE3E93FD");
        private const int SlotFactory1EnumAdapters1 = 12;
        private const int SlotFactory1IsCurrent = 13;
        private const int SlotAdapter1GetDesc1 = 10;
        private const int SlotAdapter3QueryVideoMemoryInfo = 14;
        private const uint DXGI_MEMORY_SEGMENT_GROUP_LOCAL = 0;
        private const uint DXGI_MEMORY_SEGMENT_GROUP_NON_LOCAL = 1;
        private const int DXGI_ERROR_NOT_FOUND = unchecked((int)0x887A0002);
        private const uint DXGI_ADAPTER_FLAG_SOFTWARE = 2;
        private const int AdapterDescriptionLength = 128;
        private const uint MaxAdapters = 64;

        private sealed class CachedAdapter
        {
            public IntPtr Adapter3;
            public string Name;
            public long Luid;
        }

        private static readonly object _lock = new object();
        private static IntPtr _factory = IntPtr.Zero;
        private static readonly List<CachedAdapter> _adapters = new List<CachedAdapter>();
        private static bool _stale = false;

        /* Clears samples and adds one entry per hardware adapter that answered; false when
           DXGI is unavailable or no adapter answered. Any thread; never throws. */
        public static unsafe bool TrySample(List<VideoMemorySample> samples)
        {
            if (samples == null)
                return false;
            try
            {
                samples.Clear();
                lock (_lock)
                {
                    if (!EnsureAdaptersLocked())
                        return false;
                    for (int i = 0; i < _adapters.Count; i++)
                    {
                        CachedAdapter adapter = _adapters[i];
                        delegate* unmanaged[Stdcall]<IntPtr, uint, uint, DXGI_QUERY_VIDEO_MEMORY_INFO*, int> query =
                            (delegate* unmanaged[Stdcall]<IntPtr, uint, uint, DXGI_QUERY_VIDEO_MEMORY_INFO*, int>)
                            VtableSlot(adapter.Adapter3, SlotAdapter3QueryVideoMemoryInfo);
                        DXGI_QUERY_VIDEO_MEMORY_INFO local = default;
                        DXGI_QUERY_VIDEO_MEMORY_INFO nonLocal = default;
                        if (query(adapter.Adapter3, 0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &local) < 0
                            || query(adapter.Adapter3, 0, DXGI_MEMORY_SEGMENT_GROUP_NON_LOCAL, &nonLocal) < 0)
                        {
                            _stale = true;
                            continue;
                        }
                        VideoMemorySample sample = new VideoMemorySample();
                        sample.Name = adapter.Name;
                        sample.Luid = adapter.Luid;
                        sample.LocalBudgetBytes = local.Budget;
                        sample.LocalUsageBytes = local.CurrentUsage;
                        sample.NonLocalBudgetBytes = nonLocal.Budget;
                        sample.NonLocalUsageBytes = nonLocal.CurrentUsage;
                        samples.Add(sample);
                    }
                }
                return samples.Count > 0;
            }
            catch
            {
                return false;
            }
        }

        /* True when the cache is usable, possibly with no adapters. A failed rebuild leaves the
           cache empty and returns false. The struct layouts are checked before any COM call. */
        private static unsafe bool EnsureAdaptersLocked()
        {
            if (sizeof(DXGI_ADAPTER_DESC1) != (IntPtr.Size == 8 ? 312 : 296)
                || sizeof(DXGI_QUERY_VIDEO_MEMORY_INFO) != 32)
                return false;

            if (_factory != IntPtr.Zero && !_stale)
            {
                delegate* unmanaged[Stdcall]<IntPtr, int> isCurrent =
                    (delegate* unmanaged[Stdcall]<IntPtr, int>)VtableSlot(_factory, SlotFactory1IsCurrent);
                if (isCurrent(_factory) != 0)
                    return true;
            }

            ReleaseCacheLocked();
            try
            {
                Guid iid = IID_IDXGIFactory1;
                IntPtr factory;
                int hr = CreateDXGIFactory1(ref iid, out factory);
                _factory = factory;
                if (hr < 0 || factory == IntPtr.Zero)
                {
                    ReleaseCacheLocked();
                    return false;
                }

                delegate* unmanaged[Stdcall]<IntPtr, uint, IntPtr*, int> enumAdapters1 =
                    (delegate* unmanaged[Stdcall]<IntPtr, uint, IntPtr*, int>)
                    VtableSlot(factory, SlotFactory1EnumAdapters1);
                for (uint i = 0; i < MaxAdapters; i++)
                {
                    IntPtr adapter = IntPtr.Zero;
                    try
                    {
                        hr = enumAdapters1(factory, i, &adapter);
                        if (hr == DXGI_ERROR_NOT_FOUND || hr < 0 || adapter == IntPtr.Zero)
                            break;
                        delegate* unmanaged[Stdcall]<IntPtr, DXGI_ADAPTER_DESC1*, int> getDesc1 =
                            (delegate* unmanaged[Stdcall]<IntPtr, DXGI_ADAPTER_DESC1*, int>)
                            VtableSlot(adapter, SlotAdapter1GetDesc1);
                        DXGI_ADAPTER_DESC1 desc = default;
                        if (getDesc1(adapter, &desc) < 0)
                            continue;
                        if ((desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) != 0)
                            continue;
                        string name = DescriptionName(&desc);
                        long luid = ((long)desc.AdapterLuidHighPart << 32) | desc.AdapterLuidLowPart;

                        /* Pre-WDDM 2.0 drivers have no IDXGIAdapter3 */
                        IntPtr adapter3;
                        if (Marshal.QueryInterface(adapter, in IID_IDXGIAdapter3, out adapter3) < 0
                            || adapter3 == IntPtr.Zero)
                            continue;
                        bool stored = false;
                        try
                        {
                            CachedAdapter cached = new CachedAdapter();
                            cached.Adapter3 = adapter3;
                            cached.Name = name;
                            cached.Luid = luid;
                            _adapters.Add(cached);
                            stored = true;
                        }
                        finally
                        {
                            if (!stored)
                                Marshal.Release(adapter3);
                        }
                    }
                    finally
                    {
                        if (adapter != IntPtr.Zero)
                            Marshal.Release(adapter);
                    }
                }
                _stale = false;
                return true;
            }
            catch
            {
                ReleaseCacheLocked();
                return false;
            }
        }

        /* Releases every cached IDXGIAdapter3 and the factory, and empties the cache */
        private static void ReleaseCacheLocked()
        {
            for (int i = 0; i < _adapters.Count; i++)
            {
                IntPtr adapter3 = _adapters[i].Adapter3;
                if (adapter3 != IntPtr.Zero)
                {
                    try
                    {
                        Marshal.Release(adapter3);
                    }
                    catch
                    {
                    }
                }
            }
            _adapters.Clear();

            IntPtr factory = _factory;
            _factory = IntPtr.Zero;
            if (factory != IntPtr.Zero)
            {
                try
                {
                    Marshal.Release(factory);
                }
                catch
                {
                }
            }
        }

        /* The description up to the first NUL, trimmed */
        private static unsafe string DescriptionName(DXGI_ADAPTER_DESC1* desc)
        {
            int length = 0;
            while (length < AdapterDescriptionLength && desc->Description[length] != 0)
                length++;
            return new string((char*)desc->Description, 0, length).Trim();
        }

        private static unsafe void* VtableSlot(IntPtr comObject, int slot)
        {
            void** vtable = *(void***)comObject;
            return vtable[slot];
        }
    }
}
#endif
