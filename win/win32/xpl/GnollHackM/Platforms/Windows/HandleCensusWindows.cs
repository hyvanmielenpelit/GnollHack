#if WINDOWS && GNH_MAUI
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;

namespace GnollHackM
{
    /* One census of this process's handle table. The Section size figures, WindowSections through
       UnreadableSections, are filled only when HasSectionSizes is true. Pagefile-backed sections
       are split into committed ones and SEC_RESERVE reservations; file-backed and image sections
       are in neither. The window classes are empty when the census was taken with windowBytes 0. */
    internal struct HandleCensus
    {
        public int Handles;
        public int SectionHandles;
        public int DxgkSharedResourceHandles;
        public int WindowSections;             /* committed pagefile-backed, 0.9-1.4 x window bytes */
        public long WindowSectionBytes;
        public int LargePagefileSections;      /* committed pagefile-backed, 1 MiB <= size < 256 MiB */
        public long LargePagefileSectionBytes;
        public long PagefileSectionBytes;      /* all committed pagefile-backed sections */
        public int WindowReservedSections;     /* SEC_RESERVE pagefile-backed, 0.9-1.4 x window bytes */
        public int LargeReservedSections;      /* SEC_RESERVE pagefile-backed, 1 MiB <= size < 256 MiB */
        public long LargeReservedSectionBytes;
        public int UnreadableSections;
        public bool HasSectionSizes;
        public double Milliseconds;
        public string TopTypes;                /* "Section=15012, DxgkSharedResource=15000, ..." top 6 */
    }

    /* Counts this process's handles by object type from one ProcessHandleInformation snapshot
       and, on request, sizes its Section objects through NtQuerySection. The snapshot buffer is
       unmanaged, freed after every census, and starts at the size the previous census needed.
       Type names are resolved afresh in every census: a handle closed and its value reused by an
       object of another type between the snapshot and the query names the wrong type, which
       spoils one census instead of every later one. One census runs at a time. Self-contained,
       so it can be compiled outside the app. */
    internal static class HandleCensusWindows
    {
        /* PROCESS_HANDLE_SNAPSHOT_INFORMATION: NumberOfHandles (ULONG_PTR), a reserved ULONG_PTR,
           then the entries. PROCESS_HANDLE_TABLE_ENTRY_INFO: HandleValue, HandleCount and
           PointerCount (pointer-sized), then GrantedAccess, ObjectTypeIndex, HandleAttributes
           and a reserved field (ULONG each). */
        private static readonly int SnapshotHeaderBytes = 2 * IntPtr.Size;
        private static readonly int EntryBytes = 3 * IntPtr.Size + 16;
        private static readonly int EntryGrantedAccessOffset = 3 * IntPtr.Size;
        private static readonly int EntryObjectTypeIndexOffset = 3 * IntPtr.Size + 4;

        /* SECTION_BASIC_INFORMATION: 24 bytes on x64 and arm64, 16 on x86 */
        [StructLayout(LayoutKind.Sequential)]
        private struct SECTION_BASIC_INFORMATION
        {
            public IntPtr BaseAddress;
            public uint AllocationAttributes;
            public long MaximumSize;
        }

        [DllImport("ntdll.dll")]
        private static extern int NtQueryInformationProcess(IntPtr process, int processInformationClass,
            IntPtr processInformation, int processInformationLength, out int returnLength);

        [DllImport("ntdll.dll")]
        private static extern int NtQueryObject(IntPtr handle, int objectInformationClass,
            IntPtr objectInformation, int objectInformationLength, out int returnLength);

        [DllImport("ntdll.dll")]
        private static extern int NtQuerySection(IntPtr sectionHandle, int sectionInformationClass,
            out SECTION_BASIC_INFORMATION sectionInformation, IntPtr sectionInformationLength,
            out IntPtr returnLength);

        [DllImport("kernel32.dll")]
        private static extern IntPtr GetCurrentProcess();

        [DllImport("kernel32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool DuplicateHandle(IntPtr sourceProcess, IntPtr sourceHandle, IntPtr targetProcess,
            out IntPtr targetHandle, uint desiredAccess, [MarshalAs(UnmanagedType.Bool)] bool inheritHandle,
            uint options);

        [DllImport("kernel32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool CloseHandle(IntPtr handle);

        private const int ProcessHandleInformation = 51;
        private const int ObjectTypeInformation = 2;
        private const int SectionBasicInformation = 0;
        private const int STATUS_INFO_LENGTH_MISMATCH = unchecked((int)0xC0000004);
        private const uint SECTION_QUERY = 0x1;
        private const uint SEC_FILE = 0x800000;
        private const uint SEC_IMAGE = 0x1000000;
        private const uint SEC_RESERVE = 0x4000000;
        private const int LockTimeoutMilliseconds = 250;
        private const int InitialSnapshotBytes = 64 * 1024;
        private const int SnapshotSlackBytes = 64 * 1024;
        private const int MaxSnapshotBytes = 64 * 1024 * 1024;
        private const int MaxSnapshotAttempts = 4;
        private const int TypeBufferBytes = 1024;
        private const int MaxTypeNameAttempts = 3;
        private const int TopTypeCount = 6;
        private const long LargeSectionMinBytes = 1L << 20;
        private const long LargeSectionMaxBytes = 256L << 20;
        private const double WindowSectionMinRatio = 0.9;
        private const double WindowSectionMaxRatio = 1.4;
        private const string SectionTypeName = "Section";
        private const string DxgkSharedResourceTypeName = "DxgkSharedResource";

        private static readonly object _lock = new object();
        private static int _snapshotBytes = InitialSnapshotBytes;

        /* Reused across censuses under _lock and cleared at the start of each */
        private static readonly Dictionary<uint, int> _countsByType = new Dictionary<uint, int>();
        private static readonly Dictionary<uint, int> _firstEntryByType = new Dictionary<uint, int>();
        private static readonly Dictionary<uint, string> _namesByType = new Dictionary<uint, string>();
        private static readonly List<KeyValuePair<uint, int>> _sortedCounts = new List<KeyValuePair<uint, int>>();
        private static readonly StringBuilder _topTypes = new StringBuilder(256);
        private static readonly Comparison<KeyValuePair<uint, int>> _byCountDescending = CompareByCountDescending;

        /* Any thread. windowBytes = client width x height x 4, or 0 when unknown (no window classes).
           Returns false when another census holds the lock for 250 ms, or on any failure. Never
           throws. */
        internal static bool TryTake(bool withSectionSizes, long windowBytes, out HandleCensus census)
        {
            census = default(HandleCensus);
            bool locked = false;
            IntPtr snapshot = IntPtr.Zero;
            IntPtr typeBuffer = IntPtr.Zero;
            try
            {
                Monitor.TryEnter(_lock, LockTimeoutMilliseconds, ref locked);
                if (!locked)
                    return false;

                long startTimestamp = Stopwatch.GetTimestamp();
                IntPtr process = GetCurrentProcess();

                /* Handle snapshot; AllocHGlobal throws OutOfMemoryException when commit is exhausted */
                int length = _snapshotBytes;
                int status = STATUS_INFO_LENGTH_MISMATCH;
                for (int attempt = 0; attempt < MaxSnapshotAttempts; attempt++)
                {
                    snapshot = Marshal.AllocHGlobal(length);
                    int needed;
                    status = NtQueryInformationProcess(process, ProcessHandleInformation, snapshot, length, out needed);
                    if (status != STATUS_INFO_LENGTH_MISMATCH)
                        break;
                    Marshal.FreeHGlobal(snapshot);
                    snapshot = IntPtr.Zero;
                    if (length >= MaxSnapshotBytes)
                        return false;
                    long next = (long)Math.Max(needed, length) + SnapshotSlackBytes;
                    length = (int)Math.Min(next, (long)MaxSnapshotBytes);
                }
                if (status != 0 || snapshot == IntPtr.Zero)
                    return false;
                _snapshotBytes = length;

                long handleCount = Marshal.ReadIntPtr(snapshot).ToInt64();
                if (handleCount < 0 || SnapshotHeaderBytes + handleCount * EntryBytes > length)
                    return false;
                int handles = (int)handleCount;

                HandleCensus result = new HandleCensus();
                result.Handles = handles;

                /* Counts per object type, and the first entry of each type */
                _countsByType.Clear();
                _firstEntryByType.Clear();
                _namesByType.Clear();
                for (int i = 0; i < handles; i++)
                {
                    uint typeIndex = EntryObjectTypeIndex(snapshot, i);
                    int count;
                    if (_countsByType.TryGetValue(typeIndex, out count))
                    {
                        _countsByType[typeIndex] = count + 1;
                    }
                    else
                    {
                        _countsByType[typeIndex] = 1;
                        _firstEntryByType[typeIndex] = i;
                    }
                }

                /* Type names */
                typeBuffer = Marshal.AllocHGlobal(TypeBufferBytes);
                foreach (KeyValuePair<uint, int> first in _firstEntryByType)
                {
                    string name = ResolveTypeName(snapshot, handles, first.Key, first.Value, typeBuffer);
                    if (name != null)
                        _namesByType[first.Key] = name;
                }

                bool hasSectionType = false;
                uint sectionTypeIndex = 0;
                foreach (KeyValuePair<uint, string> named in _namesByType)
                {
                    if (string.Equals(named.Value, SectionTypeName, StringComparison.Ordinal))
                    {
                        hasSectionType = true;
                        sectionTypeIndex = named.Key;
                        result.SectionHandles = _countsByType[named.Key];
                    }
                    else if (string.Equals(named.Value, DxgkSharedResourceTypeName, StringComparison.Ordinal))
                    {
                        result.DxgkSharedResourceHandles = _countsByType[named.Key];
                    }
                }

                /* Section sizes */
                if (withSectionSizes)
                {
                    if (hasSectionType)
                        ClassifySections(process, snapshot, handles, sectionTypeIndex, windowBytes, ref result);
                    result.HasSectionSizes = true;
                }

                result.TopTypes = BuildTopTypes();
                result.Milliseconds = (Stopwatch.GetTimestamp() - startTimestamp) * 1000.0 / Stopwatch.Frequency;
                census = result;
                return true;
            }
            catch
            {
                census = default(HandleCensus);
                return false;
            }
            finally
            {
                if (snapshot != IntPtr.Zero)
                    Marshal.FreeHGlobal(snapshot);
                if (typeBuffer != IntPtr.Zero)
                    Marshal.FreeHGlobal(typeBuffer);
                if (locked)
                    Monitor.Exit(_lock);
            }
        }

        /* Sizes every Section handle in the snapshot and adds it to its classes */
        private static void ClassifySections(IntPtr process, IntPtr snapshot, int handles, uint sectionTypeIndex,
            long windowBytes, ref HandleCensus result)
        {
            double windowMin = WindowSectionMinRatio * windowBytes;
            double windowMax = WindowSectionMaxRatio * windowBytes;
            for (int i = 0; i < handles; i++)
            {
                if (EntryObjectTypeIndex(snapshot, i) != sectionTypeIndex)
                    continue;
                IntPtr entry = EntryAddress(snapshot, i);
                IntPtr handle = Marshal.ReadIntPtr(entry);
                uint grantedAccess = (uint)Marshal.ReadInt32(entry, EntryGrantedAccessOffset);
                SECTION_BASIC_INFORMATION info;
                if (!TryQuerySection(process, handle, grantedAccess, out info))
                {
                    result.UnreadableSections++;
                    continue;
                }

                uint attributes = info.AllocationAttributes;
                long size = info.MaximumSize;
                if ((attributes & (SEC_FILE | SEC_IMAGE)) != 0)
                    continue;
                bool windowSized = windowBytes > 0 && size >= windowMin && size <= windowMax;
                bool large = size >= LargeSectionMinBytes && size < LargeSectionMaxBytes;

                /* A reservation commits pages only on demand, so it stays out of the committed figures */
                if ((attributes & SEC_RESERVE) != 0)
                {
                    if (windowSized)
                        result.WindowReservedSections++;
                    if (large)
                    {
                        result.LargeReservedSections++;
                        result.LargeReservedSectionBytes += size;
                    }
                    continue;
                }

                result.PagefileSectionBytes += size;
                if (large)
                {
                    result.LargePagefileSections++;
                    result.LargePagefileSectionBytes += size;
                }
                if (windowSized)
                {
                    result.WindowSections++;
                    result.WindowSectionBytes += size;
                }
            }
        }

        /* A handle without SECTION_QUERY is queried through a duplicate that has it. Only that
           duplicate is ever closed, never a handle from the snapshot. */
        private static bool TryQuerySection(IntPtr process, IntPtr handle, uint grantedAccess,
            out SECTION_BASIC_INFORMATION info)
        {
            info = default(SECTION_BASIC_INFORMATION);
            IntPtr target = handle;
            IntPtr duplicate = IntPtr.Zero;
            if ((grantedAccess & SECTION_QUERY) == 0)
            {
                if (!DuplicateHandle(process, handle, process, out duplicate, SECTION_QUERY, false, 0)
                    || duplicate == IntPtr.Zero)
                    return false;
                target = duplicate;
            }
            try
            {
                IntPtr returnLength;
                return NtQuerySection(target, SectionBasicInformation, out info,
                    (IntPtr)Marshal.SizeOf<SECTION_BASIC_INFORMATION>(), out returnLength) == 0;
            }
            finally
            {
                if (duplicate != IntPtr.Zero)
                    CloseHandle(duplicate);
            }
        }

        /* The type name through ObjectTypeInformation on the first entries of that type, or null
           when none of the first three answers. ObjectNameInformation is never queried: it can
           block forever on a synchronous pipe handle, and type information does not. The result
           begins with a UNICODE_STRING: ushort Length in bytes, ushort MaximumLength, padding to
           pointer alignment, then the Buffer pointer, which points inside typeBuffer. */
        private static string ResolveTypeName(IntPtr snapshot, int handles, uint typeIndex, int firstEntry,
            IntPtr typeBuffer)
        {
            int attempts = 0;
            for (int i = firstEntry; i < handles && attempts < MaxTypeNameAttempts; i++)
            {
                if (EntryObjectTypeIndex(snapshot, i) != typeIndex)
                    continue;
                attempts++;
                IntPtr handle = Marshal.ReadIntPtr(EntryAddress(snapshot, i));
                int returnLength;
                if (NtQueryObject(handle, ObjectTypeInformation, typeBuffer, TypeBufferBytes, out returnLength) != 0)
                    continue;
                int nameBytes = (ushort)Marshal.ReadInt16(typeBuffer);
                IntPtr nameBuffer = Marshal.ReadIntPtr(typeBuffer, IntPtr.Size);
                if (nameBytes == 0 || nameBytes > TypeBufferBytes || nameBuffer == IntPtr.Zero)
                    continue;
                return Marshal.PtrToStringUni(nameBuffer, nameBytes / 2);
            }
            return null;
        }

        /* The six largest counts as "Name=count" joined by ", "; an unresolved type prints as "#index" */
        private static string BuildTopTypes()
        {
            _sortedCounts.Clear();
            foreach (KeyValuePair<uint, int> pair in _countsByType)
                _sortedCounts.Add(pair);
            _sortedCounts.Sort(_byCountDescending);

            _topTypes.Clear();
            int shown = Math.Min(TopTypeCount, _sortedCounts.Count);
            for (int i = 0; i < shown; i++)
            {
                KeyValuePair<uint, int> pair = _sortedCounts[i];
                if (i > 0)
                    _topTypes.Append(", ");
                string name;
                if (_namesByType.TryGetValue(pair.Key, out name))
                    _topTypes.Append(name);
                else
                    _topTypes.Append('#').Append(pair.Key);
                _topTypes.Append('=').Append(pair.Value);
            }
            return _topTypes.ToString();
        }

        /* Larger count first; equal counts by type index */
        private static int CompareByCountDescending(KeyValuePair<uint, int> a, KeyValuePair<uint, int> b)
        {
            int byCount = b.Value.CompareTo(a.Value);
            return byCount != 0 ? byCount : a.Key.CompareTo(b.Key);
        }

        private static IntPtr EntryAddress(IntPtr snapshot, int index)
        {
            return IntPtr.Add(snapshot, SnapshotHeaderBytes + index * EntryBytes);
        }

        private static uint EntryObjectTypeIndex(IntPtr snapshot, int index)
        {
            return (uint)Marshal.ReadInt32(EntryAddress(snapshot, index), EntryObjectTypeIndexOffset);
        }
    }
}
#endif
