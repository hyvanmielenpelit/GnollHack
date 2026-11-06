#if WINDOWS || WINDOWS_AVALONIA
using System;
using System.Collections.Generic;
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

    /* The machine's DXGI adapters, for the WinUI 3 render adapter probe and the adapter selections of WinUI 3 and
       Windows Avalonia. Nothing here touches GHApp: the WMI adapter list is a parameter. No public method throws;
       every failure returns false. Any thread. */
    public static class GHDxgiAdapters
    {
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

        /* IDXGIFactory6 (dxgi1_6.h, Windows 10 1803 and later) continues IDXGIFactory1 with
           IDXGIFactory2 14-24, IDXGIFactory3 25, IDXGIFactory4 26-27 and IDXGIFactory5 28;
           slot 29 is EnumAdapterByGpuPreference. Slot 0 is IUnknown's QueryInterface. */
        private static readonly Guid IID_IDXGIFactory6 = new Guid("c1b6694f-ff09-44a9-b03c-77900a0a1d17");
        private static readonly Guid IID_IDXGIAdapter1 = new Guid("29038f61-3839-4626-91fd-086879011a05");
        private const int SlotUnknownQueryInterface = 0;
        private const int SlotFactory6EnumAdapterByGpuPreference = 29;
        private const int DXGI_GPU_PREFERENCE_MINIMUM_POWER = 1;
        private const int DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE = 2;

        /* IDXGIDevice continues IDXGIObject with GetAdapter at slot 7 */
        private static readonly Guid IID_IDXGIDevice = new Guid("54ec77fa-1377-44e6-8c32-88fd5f44c84c");
        private const int SlotDeviceGetAdapter = 7;

        /* Appends one GHRenderAdapter per DXGI adapter, in EnumAdapters1 order; false when the
           factory cannot be created or the descriptor layout does not match this process's
           pointer size. gpus is the WMI adapter list that decides IsIntegrated; null is none. */
        public static bool Enumerate(List<GHRenderAdapter> adapters, DeviceGPU[] gpus)
        {
            if (adapters == null)
                return false;
            try
            {
                return EnumerateAdapters(adapters, gpus);
            }
            catch
            {
                return false;
            }
        }

        /* The LUIDs of the adapters IDXGIFactory6::EnumAdapterByGpuPreference lists first for
           DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE and for DXGI_GPU_PREFERENCE_MINIMUM_POWER. False,
           with both null, when IDXGIFactory6 is missing or a call fails. */
        public static bool TryGetPreferenceLuids(out long? highPerformanceLuid, out long? minimumPowerLuid)
        {
            highPerformanceLuid = null;
            minimumPowerLuid = null;
            try
            {
                long high, low;
                if (!ReadPreferenceLuids(out high, out low))
                    return false;
                highPerformanceLuid = high;
                minimumPowerLuid = low;
                return true;
            }
            catch
            {
                highPerformanceLuid = null;
                minimumPowerLuid = null;
                return false;
            }
        }

        /* adapter is the first adapter EnumAdapters1 lists, the one D3D11CreateDevice uses for a
           null adapter: what SkiaSharp's ANGLE and Avalonia without an adapter selection callback
           render on. discretePresent is HasDiscreteAdapter of all adapters. False, with adapter
           null and discretePresent false, when there is no adapter. */
        public static bool TryGetDefaultAdapter(DeviceGPU[] gpus, out GHRenderAdapter adapter, out bool discretePresent)
        {
            adapter = null;
            discretePresent = false;
            try
            {
                List<GHRenderAdapter> adapters = new List<GHRenderAdapter>();
                if (!Enumerate(adapters, gpus) || adapters.Count == 0)
                    return false;
                adapter = adapters[0];
                discretePresent = HasDiscreteAdapter(adapters);
                return true;
            }
            catch
            {
                adapter = null;
                discretePresent = false;
                return false;
            }
        }

        /* The LUID of the adapter a Direct3D 11 device was created on, through IDXGIDevice::GetAdapter. The device
           reference is borrowed: it is never released here. False, with luid 0, when a query fails. */
        public static bool TryGetDeviceAdapterLuid(IntPtr d3d11Device, out long luid)
        {
            luid = 0;
            if (d3d11Device == IntPtr.Zero)
                return false;
            try
            {
                return ReadDeviceAdapterLuid(d3d11Device, out luid);
            }
            catch
            {
                luid = 0;
                return false;
            }
        }

        /* True when a hardware adapter in the list has IsIntegrated false */
        public static bool HasDiscreteAdapter(List<GHRenderAdapter> adapters)
        {
            if (adapters == null)
                return false;
            for (int i = 0; i < adapters.Count; i++)
            {
                GHRenderAdapter a = adapters[i];
                if (a != null && !a.IsSoftware && a.IsIntegrated == false)
                    return true;
            }
            return false;
        }

        /* The fingerprint's hardware.renderAdapter value: "<Name> (software)", "<Name>
           (integrated)", "<Name> (discrete)", or "<Name>" when IsIntegrated is unknown;
           software wins over integrated. Null for a null adapter or an empty name. */
        public static string FingerprintValue(GHRenderAdapter a)
        {
            if (a == null || string.IsNullOrWhiteSpace(a.Name))
                return null;
            string name = a.Name.Trim();
            if (a.IsSoftware)
                return name + " (software)";
            if (a.IsIntegrated == true)
                return name + " (integrated)";
            if (a.IsIntegrated == false)
                return name + " (discrete)";
            return name;
        }

        public static GHRenderAdapter Copy(GHRenderAdapter a)
        {
            GHRenderAdapter copy = new GHRenderAdapter();
            copy.Name = a.Name;
            copy.Luid = a.Luid;
            copy.IsSoftware = a.IsSoftware;
            copy.IsIntegrated = a.IsIntegrated;
            copy.DedicatedVideoMemoryMB = a.DedicatedVideoMemoryMB;
            return copy;
        }

        public static long MakeLuid(uint lowPart, int highPart)
        {
            return ((long)highPart << 32) | lowPart;
        }

        /* Appends one GHRenderAdapter per DXGI adapter; false when the factory cannot be
           created or the descriptor layout does not match this process's pointer size. */
        private static unsafe bool EnumerateAdapters(List<GHRenderAdapter> adapters, DeviceGPU[] gpus)
        {
            if (sizeof(DXGI_ADAPTER_DESC1) != (IntPtr.Size == 8 ? 312 : 296))
                return false;

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

        /* Queries IDXGIFactory6 from a new IDXGIFactory1 and reads the first adapter of each
           preference; releases the factories in every case */
        private static unsafe bool ReadPreferenceLuids(out long highPerformanceLuid, out long minimumPowerLuid)
        {
            highPerformanceLuid = 0;
            minimumPowerLuid = 0;
            if (sizeof(DXGI_ADAPTER_DESC1) != (IntPtr.Size == 8 ? 312 : 296))
                return false;

            IntPtr factory = IntPtr.Zero;
            IntPtr factory6 = IntPtr.Zero;
            try
            {
                Guid iid = IID_IDXGIFactory1;
                if (CreateDXGIFactory1(ref iid, out factory) < 0 || factory == IntPtr.Zero)
                    return false;
                delegate* unmanaged[Stdcall]<IntPtr, Guid*, IntPtr*, int> queryInterface =
                    (delegate* unmanaged[Stdcall]<IntPtr, Guid*, IntPtr*, int>)VtableSlot(factory, SlotUnknownQueryInterface);
                Guid iid6 = IID_IDXGIFactory6;
                if (queryInterface(factory, &iid6, &factory6) < 0 || factory6 == IntPtr.Zero)
                    return false;
                return ReadPreferenceLuid(factory6, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, out highPerformanceLuid)
                    && ReadPreferenceLuid(factory6, DXGI_GPU_PREFERENCE_MINIMUM_POWER, out minimumPowerLuid);
            }
            finally
            {
                if (factory6 != IntPtr.Zero)
                    Marshal.Release(factory6);
                if (factory != IntPtr.Zero)
                    Marshal.Release(factory);
            }
        }

        /* EnumAdapterByGpuPreference(0, preference, IID_IDXGIAdapter1) and the adapter's LUID
           from GetDesc1; releases the adapter */
        private static unsafe bool ReadPreferenceLuid(IntPtr factory6, int preference, out long luid)
        {
            luid = 0;
            IntPtr adapter = IntPtr.Zero;
            try
            {
                delegate* unmanaged[Stdcall]<IntPtr, uint, int, Guid*, IntPtr*, int> enumAdapterByGpuPreference =
                    (delegate* unmanaged[Stdcall]<IntPtr, uint, int, Guid*, IntPtr*, int>)VtableSlot(factory6, SlotFactory6EnumAdapterByGpuPreference);
                Guid iid = IID_IDXGIAdapter1;
                if (enumAdapterByGpuPreference(factory6, 0, preference, &iid, &adapter) < 0 || adapter == IntPtr.Zero)
                    return false;
                delegate* unmanaged[Stdcall]<IntPtr, DXGI_ADAPTER_DESC1*, int> getDesc1 =
                    (delegate* unmanaged[Stdcall]<IntPtr, DXGI_ADAPTER_DESC1*, int>)VtableSlot(adapter, SlotAdapter1GetDesc1);
                DXGI_ADAPTER_DESC1 desc = default;
                if (getDesc1(adapter, &desc) < 0)
                    return false;
                luid = MakeLuid(desc.AdapterLuidLowPart, desc.AdapterLuidHighPart);
                return true;
            }
            finally
            {
                if (adapter != IntPtr.Zero)
                    Marshal.Release(adapter);
            }
        }

        /* IDXGIDevice from the device, its adapter, IDXGIAdapter1 from the adapter and the LUID from GetDesc1;
           releases the three interfaces it obtains and never the device */
        private static unsafe bool ReadDeviceAdapterLuid(IntPtr d3d11Device, out long luid)
        {
            luid = 0;
            if (sizeof(DXGI_ADAPTER_DESC1) != (IntPtr.Size == 8 ? 312 : 296))
                return false;

            IntPtr dxgiDevice = IntPtr.Zero;
            IntPtr adapter = IntPtr.Zero;
            IntPtr adapter1 = IntPtr.Zero;
            try
            {
                delegate* unmanaged[Stdcall]<IntPtr, Guid*, IntPtr*, int> deviceQueryInterface =
                    (delegate* unmanaged[Stdcall]<IntPtr, Guid*, IntPtr*, int>)VtableSlot(d3d11Device, SlotUnknownQueryInterface);
                Guid iidDevice = IID_IDXGIDevice;
                if (deviceQueryInterface(d3d11Device, &iidDevice, &dxgiDevice) < 0 || dxgiDevice == IntPtr.Zero)
                    return false;
                delegate* unmanaged[Stdcall]<IntPtr, IntPtr*, int> getAdapter =
                    (delegate* unmanaged[Stdcall]<IntPtr, IntPtr*, int>)VtableSlot(dxgiDevice, SlotDeviceGetAdapter);
                if (getAdapter(dxgiDevice, &adapter) < 0 || adapter == IntPtr.Zero)
                    return false;
                delegate* unmanaged[Stdcall]<IntPtr, Guid*, IntPtr*, int> adapterQueryInterface =
                    (delegate* unmanaged[Stdcall]<IntPtr, Guid*, IntPtr*, int>)VtableSlot(adapter, SlotUnknownQueryInterface);
                Guid iidAdapter1 = IID_IDXGIAdapter1;
                if (adapterQueryInterface(adapter, &iidAdapter1, &adapter1) < 0 || adapter1 == IntPtr.Zero)
                    return false;
                delegate* unmanaged[Stdcall]<IntPtr, DXGI_ADAPTER_DESC1*, int> getDesc1 =
                    (delegate* unmanaged[Stdcall]<IntPtr, DXGI_ADAPTER_DESC1*, int>)VtableSlot(adapter1, SlotAdapter1GetDesc1);
                DXGI_ADAPTER_DESC1 desc = default;
                if (getDesc1(adapter1, &desc) < 0)
                    return false;
                luid = MakeLuid(desc.AdapterLuidLowPart, desc.AdapterLuidHighPart);
                return true;
            }
            finally
            {
                if (adapter1 != IntPtr.Zero)
                    Marshal.Release(adapter1);
                if (adapter != IntPtr.Zero)
                    Marshal.Release(adapter);
                if (dxgiDevice != IntPtr.Zero)
                    Marshal.Release(dxgiDevice);
            }
        }

        private static unsafe void* VtableSlot(IntPtr comObject, int slot)
        {
            void** vtable = *(void***)comObject;
            return vtable[slot];
        }

        /* IsIntegrated comes from the WMI adapter list (GHApp.DeviceGPUs) when a description
           matches; otherwise a hardware adapter with under 512 MB of dedicated memory counts
           as integrated. A null list is an empty one. */
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
            if (name.Length > 0 && gpus != null)
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
    }
}
#endif
