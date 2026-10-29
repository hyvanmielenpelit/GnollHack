#if GNH_AVALONIA && LINUX
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.IO;
using System.Text;
using GnollHackX;

namespace GnollHackM
{
    /* Reads Steam Input's virtual gamepad from /dev/input/event* (with GNOLLHACK_GAMEPAD_EVDEV=any, also
       physical Xbox-layout pads), with non-blocking reads on the UI thread. Candidates are pre-filtered
       through sysfs, then opened and classified by GHEvdevDeviceFilter; inotify on /dev/input reports
       hot-plug, and a node udev has not yet given its uaccess ACL is retried on a schedule. Each pad's events
       go through GHEvdevDecoder to the sink with kernel timestamps; a silent snapshot follows open and Resync.
       GNOLLHACK_GAMEPAD_LOG=raw writes the events to standard error. No device is ever grabbed. The device
       log is always on. UI thread only. */
    internal sealed class GHEvdevGamepadBackend : IGHGamepadBackend
    {
        private const string DeviceDirectory = "/dev/input";
        private const string SysClassInput = "/sys/class/input";
        private const int ReadBufferEvents = 64;
        private const int MaxReadsPerDevice = 32;
        private const int MaxRawLines = 5000;
        private const int MaxControllerId = 99;
        private const double RescanIntervalSeconds = 1.0;
        private const double HintDelaySeconds = 10.0;
        private const double RetryWindowSeconds = 60.0;
        private const double RetryIntervalSeconds = 5.0;

        /* EACCES retries after the first denial; then every RetryIntervalSeconds until RetryWindowSeconds,
           then only on IN_ATTRIB */
        private static readonly double[] RetryDelaysSeconds = { 0.25, 0.5, 1.0, 2.0, 4.0 };

        /* event2 before event10 */
        private static readonly Comparison<string> NodePathComparison = (a, b) =>
            a.Length != b.Length ? a.Length.CompareTo(b.Length) : string.CompareOrdinal(a, b);

        private sealed class OpenDevice
        {
            public string Path;
            public string Node;
            public int Fd = -1;
            public int ControllerId;
            public long OpenedTicks;
            public GHEvdevDecoder Decoder;
            public bool MonotonicClock;
            public bool ClockFallbackLogged;
            public int DroppedCount;
            /* The last value of each axis, for the raw dump's rest filter */
            public readonly int[] RawAbs = new int[GHEvdevProtocol.ABS_MAX + 1];
            /* Raw lines appended since the last SYN_REPORT */
            public bool RawFrameHasLines;
        }

        /* The Steam virtual pad does not rest at exactly 0 (Step 0: up to 4.7 % of full scale) */
        private const float InnerDeadzone = 0.15f;

        private sealed class DeniedNode
        {
            public long FirstDeniedTicks;
            public int Attempt;
            public long NextRetryTicks;     /* 0: no timed retry */
            public bool RetriesStopped;
        }

        private IGHGamepadInputSink _sink = null;
        private GHGamepadLaunchEnvironment _env = null;
        private bool _started = false;
        private bool _rawDump = false;
        private int _rawLines = 0;
        private int _inotifyFd = -1;
        private long _startTicks = 0;
        private long _lastRescanTicks = 0;
        private bool _hintsChecked = false;
        private bool _sawSteamVirtual = false;
        private bool _sawValvePhysical = false;
        private int _nextControllerId = 1;

        private readonly List<OpenDevice> _devices = new List<OpenDevice>();
        private readonly Dictionary<string, DeniedNode> _denied = new Dictionary<string, DeniedNode>(StringComparer.Ordinal);
        /* The last skip line logged per node, so a re-probe with the same outcome logs nothing */
        private readonly Dictionary<string, string> _skipped = new Dictionary<string, string>(StringComparer.Ordinal);
        private readonly List<string> _duePaths = new List<string>();
        private readonly StringBuilder _rawBatch = new StringBuilder();

        private readonly byte[] _readBuffer = new byte[ReadBufferEvents * GHEvdevProtocol.InputEventSize];
        private readonly byte[] _inotifyBuffer = new byte[4096];
        private readonly List<GHInotifyRecord> _inotifyRecords = new List<GHInotifyRecord>();
        private readonly byte[] _idBuffer = new byte[GHEvdevProtocol.InputIdSize];
        private readonly byte[] _nameBuffer = new byte[GHEvdevProtocol.NameBytes];
        private readonly byte[] _propBits = new byte[GHEvdevProtocol.PropBitsBytes];
        private readonly byte[] _eventTypeBits = new byte[GHEvdevProtocol.EventTypeBitsBytes];
        private readonly byte[] _keyBits = new byte[GHEvdevProtocol.KeyBitsBytes];
        private readonly byte[] _absBits = new byte[GHEvdevProtocol.AbsBitsBytes];
        private readonly byte[] _absInfoBuffer = new byte[GHEvdevProtocol.AbsInfoSize];
        private readonly byte[] _keyStateBuffer = new byte[GHEvdevProtocol.KeyBitsBytes];
        private readonly int[] _absValues = new int[GHEvdevProtocol.ABS_MAX + 1];

        private GHSteamVirtualGamepadInfo _virtualGamepadInfo = null;
        private DateTime _virtualGamepadInfoWriteTime;
        private bool _virtualGamepadInfoErrorLogged = false;

        public string Name { get { return "evdev"; } }

        /* Device log lines: standard error, which the Deck launcher captures, and the app log */
        public static void WriteDeviceLog(string line)
        {
            try
            {
                Console.Error.WriteLine(line);
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
            GHApp.WriteGHLog(line);
        }

        /* Reads the environment and logs it; false when the backend is not eligible (GHGamepadLaunchEnvironment).
           Succeeds without a pad: Steam may create it later. */
        public bool Start(IGHGamepadInputSink sink, out string error)
        {
            error = null;
            if (_started)
                return true;
            if (sink == null)
            {
                error = "no sink";
                return false;
            }
            if (!BitConverter.IsLittleEndian)
            {
                error = "big-endian machine";
                return false;
            }

            _env = GHGamepadLaunchEnvironment.Create(Environment.GetEnvironmentVariable, ReadTextFile, GHApp.IsSteamDeck);
            string logSetting = Environment.GetEnvironmentVariable("GNOLLHACK_GAMEPAD_LOG");
            _rawDump = logSetting != null && string.Equals(logSetting.Trim(), "raw", StringComparison.OrdinalIgnoreCase);
            string reason;
            bool eligible = _env.IsEvdevEligible(out reason);
            WriteDeviceLog(FormatEnvironmentLine(eligible, reason));
            if (!eligible)
            {
                error = reason;
                return false;
            }

            _sink = sink;
            _startTicks = Stopwatch.GetTimestamp();
            _lastRescanTicks = _startTicks;
            _started = true;
            try
            {
                OpenInotify();
                Rescan(_startTicks);
            }
            catch
            {
                Stop();
                throw;
            }
            return true;
        }

        public void Stop()
        {
            int i;
            for (i = _devices.Count - 1; i >= 0; i--)
            {
                OpenDevice dev = _devices[i];
                CloseDeviceFd(dev);
                WriteDeviceLog("GAMEPAD dev " + dev.Node + " -closed stop cid=" + Format(dev.ControllerId));
                try
                {
                    if (_sink != null)
                        _sink.ControllerRemoved(dev.ControllerId);
                }
                catch (Exception ex)
                {
                    Debug.WriteLine(ex.Message);
                }
            }
            _devices.Clear();
            CloseInotify();
            _denied.Clear();
            _skipped.Clear();
            FlushRaw();
            _started = false;
            _sink = null;
        }

        public void Pump(long nowTicks)
        {
            if (!_started)
                return;

            if (_inotifyFd >= 0)
            {
                DrainInotify(nowTicks);
            }
            else if (Seconds(_lastRescanTicks, nowTicks) >= RescanIntervalSeconds)
            {
                _lastRescanTicks = nowTicks;
                Rescan(nowTicks);
            }

            RetryDeniedNodes(nowTicks);

            int i;
            for (i = _devices.Count - 1; i >= 0; i--)
            {
                if (i < _devices.Count)
                    ReadDevice(_devices[i], nowTicks, true);
            }

            if (!_hintsChecked && Seconds(_startTicks, nowTicks) >= HintDelaySeconds)
                CheckHints();

            FlushRaw();
        }

        /* Drains every device without decoding or logging, then takes a silent snapshot */
        public void Resync(long nowTicks)
        {
            if (!_started)
                return;
            int i;
            for (i = _devices.Count - 1; i >= 0; i--)
            {
                if (i >= _devices.Count)
                    continue;
                OpenDevice dev = _devices[i];
                ReadDevice(dev, nowTicks, false);
                if (dev.Fd >= 0)
                {
                    dev.Decoder.DiscardPending();
                    TakeSnapshot(dev, nowTicks, true);
                }
            }
        }

        private string FormatEnvironmentLine(bool eligible, string reason)
        {
            StringBuilder sb = new StringBuilder(256);
            sb.Append("GAMEPAD env mode=").Append(_env.ModeText);
            sb.Append(" steamLaunch=").Append(_env.SteamLaunch ? "yes(" + _env.SteamLaunchSource + ")" : "no");
            sb.Append(" allowVirtual=").Append(_env.AllowSteamVirtualHint ? "1" : "0");
            sb.Append(" ignore=").Append(_env.Ignore.Describe(4));
            sb.Append(" except=").Append(_env.AllowOnly.Describe(4));
            sb.Append(" info=").Append(_env.VirtualGamepadInfoPath ?? "-");
            sb.Append(" container=").Append(DetectContainer());
            sb.Append(" deck=").Append(_env.IsSteamDeck ? "yes" : "no");
            sb.Append(" eligible=").Append(eligible ? "yes" : "no").Append(" (").Append(reason).Append(')');
            sb.Append(" raw=").Append(_rawDump ? "on" : "off");
            return sb.ToString();
        }

        private static string DetectContainer()
        {
            try
            {
                if (Directory.Exists("/run/pressure-vessel"))
                    return "pressure-vessel";
                if (File.Exists("/.flatpak-info"))
                    return "flatpak";
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
            return "none";
        }

        private static string ReadTextFile(string path)
        {
            try
            {
                return File.Exists(path) ? File.ReadAllText(path) : null;
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
                return null;
            }
        }

        private void OpenInotify()
        {
            int fd = GHEvdevNative.InotifyInit1(GHEvdevProtocol.IN_NONBLOCK | GHEvdevProtocol.IN_CLOEXEC);
            if (fd < 0)
            {
                WriteDeviceLog("GAMEPAD inotify_init1 failed: " + GHEvdevProtocol.GetErrnoName(GHEvdevNative.LastError)
                    + "; rescanning every " + Format(RescanIntervalSeconds) + " s");
                return;
            }
            int wd = GHEvdevNative.InotifyAddWatch(fd, DeviceDirectory, GHEvdevProtocol.DeviceDirectoryWatchMask);
            if (wd < 0)
            {
                int errno = GHEvdevNative.LastError;
                GHEvdevNative.Close(fd);
                WriteDeviceLog("GAMEPAD inotify_add_watch " + DeviceDirectory + " failed: " + GHEvdevProtocol.GetErrnoName(errno)
                    + "; rescanning every " + Format(RescanIntervalSeconds) + " s");
                return;
            }
            _inotifyFd = fd;
        }

        private void CloseInotify()
        {
            int fd = _inotifyFd;
            _inotifyFd = -1;
            if (fd >= 0)
                GHEvdevNative.Close(fd);
        }

        private void DrainInotify(long nowTicks)
        {
            int reads = 0;
            while (_inotifyFd >= 0 && reads < MaxReadsPerDevice)
            {
                reads++;
                nint n = GHEvdevNative.Read(_inotifyFd, _inotifyBuffer, (nuint)_inotifyBuffer.Length);
                if (n < 0)
                {
                    int errno = GHEvdevNative.LastError;
                    if (errno == GHEvdevProtocol.EINTR)
                        continue;
                    if (errno == GHEvdevProtocol.EAGAIN)
                        return;
                    WriteDeviceLog("GAMEPAD inotify read failed: " + GHEvdevProtocol.GetErrnoName(errno)
                        + "; rescanning every " + Format(RescanIntervalSeconds) + " s");
                    CloseInotify();
                    return;
                }
                if (n == 0)
                    return;

                _inotifyRecords.Clear();
                GHEvdevProtocol.ParseInotifyRecords(_inotifyBuffer, (int)n, _inotifyRecords);
                int i;
                for (i = 0; i < _inotifyRecords.Count; i++)
                    HandleInotifyRecord(_inotifyRecords[i], nowTicks);
            }
        }

        private void HandleInotifyRecord(GHInotifyRecord record, long nowTicks)
        {
            uint mask = record.Mask;
            if ((mask & GHEvdevProtocol.IN_Q_OVERFLOW) != 0)
            {
                WriteDeviceLog("GAMEPAD inotify queue overflow; rescanning");
                Rescan(nowTicks);
                return;
            }
            if ((mask & GHEvdevProtocol.IN_IGNORED) != 0)
            {
                WriteDeviceLog("GAMEPAD inotify watch on " + DeviceDirectory + " removed; rescanning every "
                    + Format(RescanIntervalSeconds) + " s");
                CloseInotify();
                return;
            }
            if ((mask & GHEvdevProtocol.IN_ISDIR) != 0 || record.Name == null
                || !record.Name.StartsWith("event", StringComparison.Ordinal))
                return;

            string path = DeviceDirectory + "/" + record.Name;
            OpenDevice dev = FindDevice(path);
            if ((mask & (GHEvdevProtocol.IN_DELETE | GHEvdevProtocol.IN_MOVED_FROM)) != 0)
            {
                _denied.Remove(path);
                _skipped.Remove(path);
                if (dev != null)
                    RemoveDevice(dev, "deleted", nowTicks);
                return;
            }
            if ((mask & GHEvdevProtocol.IN_CREATE) != 0)
            {
                /* The kernel never reuses a node name while the device exists, so an open one is gone */
                if (dev != null)
                    RemoveDevice(dev, "replaced", nowTicks);
                _denied.Remove(path);
                _skipped.Remove(path);
                Probe(path, nowTicks);
                return;
            }
            if ((mask & (GHEvdevProtocol.IN_MOVED_TO | GHEvdevProtocol.IN_ATTRIB)) != 0 && dev == null)
                Probe(path, nowTicks);
        }

        /* Probes every node that is not open and removes open devices whose node is gone */
        private void Rescan(long nowTicks)
        {
            string[] paths;
            try
            {
                paths = Directory.GetFiles(DeviceDirectory, "event*");
            }
            catch (Exception ex)
            {
                WriteDeviceLog("GAMEPAD cannot list " + DeviceDirectory + ": " + ex.Message);
                paths = new string[0];
            }
            Array.Sort(paths, NodePathComparison);

            int i;
            for (i = _devices.Count - 1; i >= 0; i--)
            {
                if (i < _devices.Count && Array.IndexOf(paths, _devices[i].Path) < 0)
                    RemoveDevice(_devices[i], "gone", nowTicks);
            }
            RemoveAbsentKeys(_denied, paths);
            RemoveAbsentKeys(_skipped, paths);

            foreach (string path in paths)
            {
                if (FindDevice(path) == null)
                    Probe(path, nowTicks);
            }
        }

        private static void RemoveAbsentKeys<T>(Dictionary<string, T> dictionary, string[] paths)
        {
            if (dictionary.Count == 0)
                return;
            List<string> absent = null;
            foreach (string key in dictionary.Keys)
            {
                if (Array.IndexOf(paths, key) < 0)
                {
                    if (absent == null)
                        absent = new List<string>();
                    absent.Add(key);
                }
            }
            if (absent != null)
            {
                foreach (string key in absent)
                    dictionary.Remove(key);
            }
        }

        private OpenDevice FindDevice(string path)
        {
            int i;
            for (i = 0; i < _devices.Count; i++)
            {
                if (string.Equals(_devices[i].Path, path, StringComparison.Ordinal))
                    return _devices[i];
            }
            return null;
        }

        private void Probe(string path, long nowTicks)
        {
            string node = Path.GetFileName(path);
            string sysName = ReadSysfs(node, "name");
            if (sysName != null)
                sysName = sysName.Trim();

            /* sysfs pre-filter: a node the filter rejects is never opened */
            ushort sysVendor, sysProduct;
            byte[] sysKeyBits, sysAbsBits, sysPropBits;
            if (TryReadSysfsCapabilities(node, out sysVendor, out sysProduct, out sysKeyBits, out sysAbsBits, out sysPropBits))
            {
                GHEvdevVerdict preVerdict = GHEvdevDeviceFilter.Classify(sysVendor, sysProduct, sysKeyBits, sysAbsBits, sysPropBits, _env);
                NoteSeen(sysVendor, sysProduct, sysKeyBits, sysAbsBits, preVerdict);
                if (!GHEvdevDeviceFilter.IsAccepted(preVerdict))
                {
                    LogSkip(path, node, "skip id=" + Hex4(sysVendor) + ":" + Hex4(sysProduct) + " name=\"" + (sysName ?? "?")
                        + "\" verdict=" + GHEvdevDeviceFilter.GetVerdictName(preVerdict) + " (sysfs)");
                    return;
                }
            }

            int fd = GHEvdevNative.Open(path, GHEvdevProtocol.OpenFlags);
            if (fd < 0)
            {
                int errno = GHEvdevNative.LastError;
                if (errno == GHEvdevProtocol.EACCES || errno == GHEvdevProtocol.EPERM)
                {
                    NoteDenied(path, node, sysName, errno, nowTicks);
                    return;
                }
                _denied.Remove(path);
                if (errno == GHEvdevProtocol.ENOENT || errno == GHEvdevProtocol.ENXIO || errno == GHEvdevProtocol.ENODEV)
                    return;
                LogSkip(path, node, "open=" + GHEvdevProtocol.GetErrnoName(errno) + " sysname=\"" + (sysName ?? "?") + "\" verdict=skip");
                return;
            }
            _denied.Remove(path);

            OpenDevice dev = null;
            GHGamepadControllerInfo info = null;
            try
            {
                dev = ProbeOpenNode(path, node, fd, nowTicks, out info);
            }
            finally
            {
                if (dev == null)
                    GHEvdevNative.Close(fd);
            }
            if (dev != null)
            {
                _devices.Add(dev);
                _sink.ControllerAdded(info);
                TakeSnapshot(dev, nowTicks, true);
            }
        }

        /* EVIOCGKEY and the current axis values into the decoder: silent after open and Resync (no edges for
           held buttons), with edges after SYN_DROPPED */
        private void TakeSnapshot(OpenDevice dev, long nowTicks, bool silent)
        {
            if (dev.Fd < 0)
                return;
            Array.Clear(_keyStateBuffer, 0, _keyStateBuffer.Length);
            if (GHEvdevNative.Ioctl(dev.Fd, GHEvdevProtocol.EVIOCGKEY(_keyStateBuffer.Length), _keyStateBuffer) < 0)
                Array.Clear(_keyStateBuffer, 0, _keyStateBuffer.Length);
            GHEvdevLayout layout = dev.Decoder.Layout;
            int axis;
            for (axis = 0; axis <= GHEvdevProtocol.ABS_MAX; axis++)
            {
                _absValues[axis] = 0;
                if (!layout.HasAxis(axis))
                    continue;
                _absValues[axis] = GHEvdevNative.Ioctl(dev.Fd, GHEvdevProtocol.EVIOCGABS(axis), _absInfoBuffer) >= 0
                    ? GHEvdevProtocol.ParseAbsInfo(_absInfoBuffer, 0).Value
                    : dev.RawAbs[axis];
                dev.RawAbs[axis] = _absValues[axis];
            }
            dev.Decoder.ApplySnapshot(_keyStateBuffer, _absValues, nowTicks, _sink, silent);
        }

        /* The capability queries on an open node, then the verdict; null when rejected. The caller closes the
           fd of a rejected node. */
        private OpenDevice ProbeOpenNode(string path, string node, int fd, long nowTicks, out GHGamepadControllerInfo info)
        {
            info = null;
            if (GHEvdevNative.Ioctl(fd, GHEvdevProtocol.EVIOCGID, _idBuffer) < 0)
            {
                LogProbeFailed(path, node, "EVIOCGID");
                return null;
            }
            GHEvdevInputId id = GHEvdevProtocol.ParseInputId(_idBuffer, 0);

            Array.Clear(_nameBuffer, 0, _nameBuffer.Length);
            int nameLength = GHEvdevNative.Ioctl(fd, GHEvdevProtocol.EVIOCGNAME(_nameBuffer.Length), _nameBuffer);
            if (nameLength < 0)
            {
                LogProbeFailed(path, node, "EVIOCGNAME");
                return null;
            }
            string name = GHEvdevProtocol.ParseName(_nameBuffer, 0, nameLength);

            /* Properties are optional: unknown properties count as none */
            Array.Clear(_propBits, 0, _propBits.Length);
            bool haveProps = GHEvdevNative.Ioctl(fd, GHEvdevProtocol.EVIOCGPROP(_propBits.Length), _propBits) >= 0;

            Array.Clear(_eventTypeBits, 0, _eventTypeBits.Length);
            Array.Clear(_keyBits, 0, _keyBits.Length);
            Array.Clear(_absBits, 0, _absBits.Length);
            if (GHEvdevNative.Ioctl(fd, GHEvdevProtocol.EVIOCGBIT(0, _eventTypeBits.Length), _eventTypeBits) < 0)
            {
                LogProbeFailed(path, node, "EVIOCGBIT(0)");
                return null;
            }
            if (GHEvdevNative.Ioctl(fd, GHEvdevProtocol.EVIOCGBIT(GHEvdevProtocol.EV_KEY, _keyBits.Length), _keyBits) < 0)
            {
                LogProbeFailed(path, node, "EVIOCGBIT(EV_KEY)");
                return null;
            }
            if (GHEvdevNative.Ioctl(fd, GHEvdevProtocol.EVIOCGBIT(GHEvdevProtocol.EV_ABS, _absBits.Length), _absBits) < 0)
            {
                LogProbeFailed(path, node, "EVIOCGBIT(EV_ABS)");
                return null;
            }

            GHEvdevVerdict verdict = GHEvdevDeviceFilter.Classify(id.Vendor, id.Product, _keyBits, _absBits, haveProps ? _propBits : null, _env);
            NoteSeen(id.Vendor, id.Product, _keyBits, _absBits, verdict);
            string idText = Hex4(id.BusType) + ":" + Hex4(id.Vendor) + ":" + Hex4(id.Product) + ":" + Hex4(id.Version);
            string bitsText = " keys=" + Format(GHEvdevProtocol.CountBits(_keyBits)) + " abs=" + Format(GHEvdevProtocol.CountBits(_absBits));
            if (!GHEvdevDeviceFilter.IsAccepted(verdict))
            {
                LogSkip(path, node, "skip id=" + idText + " name=\"" + name + "\"" + bitsText
                    + " verdict=" + GHEvdevDeviceFilter.GetVerdictName(verdict));
                return null;
            }

            int clockId = GHEvdevProtocol.CLOCK_MONOTONIC;
            bool monotonic = GHEvdevNative.Ioctl(fd, GHEvdevProtocol.EVIOCSCLOCKID, ref clockId) == 0;
            string clockText = monotonic ? "monotonic" : "realtime(" + GHEvdevProtocol.GetErrnoName(GHEvdevNative.LastError) + ")";

            StringBuilder axes = new StringBuilder();
            GHEvdevAbsInfo[] absInfos = new GHEvdevAbsInfo[GHEvdevProtocol.ABS_MAX + 1];
            int axis;
            for (axis = 0; axis <= GHEvdevProtocol.ABS_MAX; axis++)
            {
                if (!GHEvdevProtocol.TestBit(_absBits, axis))
                    continue;
                axes.Append(' ').Append(GHEvdevProtocol.GetAxisName(axis)).Append('=');
                if (GHEvdevNative.Ioctl(fd, GHEvdevProtocol.EVIOCGABS(axis), _absInfoBuffer) < 0)
                {
                    axes.Append('?');
                    continue;
                }
                GHEvdevAbsInfo absInfo = GHEvdevProtocol.ParseAbsInfo(_absInfoBuffer, 0);
                absInfos[axis] = absInfo;
                axes.Append(Format(absInfo.Minimum)).Append("..").Append(Format(absInfo.Maximum));
                if (absInfo.Fuzz != 0 || absInfo.Flat != 0)
                    axes.Append("/f").Append(Format(absInfo.Fuzz)).Append('/').Append(Format(absInfo.Flat));
            }

            GHGamepadStyle style;
            string realText = null;
            if (verdict == GHEvdevVerdict.AcceptSteamVirtual)
            {
                int slot = GHSteamVirtualGamepadInfo.GetSlotFromDeviceName(name);
                GHSteamVirtualGamepadInfo virtualInfo = LoadVirtualGamepadInfo();
                GHSteamVirtualGamepadSlot slotInfo = virtualInfo != null ? virtualInfo.GetSlot(slot) : null;
                style = GHSteamVirtualGamepadInfo.ResolveStyle(slotInfo, slot, _env.IsSteamDeck);
                realText = GHSteamVirtualGamepadInfo.Describe(slotInfo, slot);
            }
            else
            {
                style = GHSteamVirtualGamepadInfo.GetStyle(id.Vendor, id.Product);
            }

            int controllerId = AllocateControllerId();
            info = new GHGamepadControllerInfo();
            info.Id = controllerId;
            info.Name = name;
            info.BackendName = Name;
            info.Details = node + " " + Hex4(id.Vendor) + ":" + Hex4(id.Product) + (realText != null ? " " + realText : "");
            info.Style = style;
            GHEvdevLayout layout = GHEvdevLayout.Create(_keyBits, _absBits, absInfos, id.Vendor);
            info.Capabilities = layout.Capabilities;
            info.IsAuxiliary = false;
            info.InnerDeadzone = InnerDeadzone;

            OpenDevice dev = new OpenDevice();
            dev.Path = path;
            dev.Node = node;
            dev.Fd = fd;
            dev.ControllerId = controllerId;
            dev.OpenedTicks = nowTicks;
            dev.Decoder = new GHEvdevDecoder(layout, controllerId);
            dev.MonotonicClock = monotonic;
            for (axis = 0; axis <= GHEvdevProtocol.ABS_MAX; axis++)
                dev.RawAbs[axis] = absInfos[axis].Value;

            _skipped.Remove(path);
            WriteDeviceLog("GAMEPAD dev " + node + " +open id=" + idText + " name=\"" + name + "\"" + bitsText
                + " verdict=" + GHEvdevDeviceFilter.GetVerdictName(verdict)
                + (realText != null ? " " + realText : "")
                + axes.ToString()
                + " clock=" + clockText + " style=" + style.ToString() + " cid=" + Format(controllerId));
            return dev;
        }

        private void LogProbeFailed(string path, string node, string request)
        {
            LogSkip(path, node, "probe-failed " + request + " " + GHEvdevProtocol.GetErrnoName(GHEvdevNative.LastError) + " verdict=skip");
        }

        private void LogSkip(string path, string node, string text)
        {
            string line = "GAMEPAD dev " + node + " " + text;
            string previous;
            if (_skipped.TryGetValue(path, out previous) && string.Equals(previous, line, StringComparison.Ordinal))
                return;
            _skipped[path] = line;
            WriteDeviceLog(line);
        }

        /* For the hints: whether Steam's virtual pad exists, and whether a Valve physical gamepad was rejected */
        private void NoteSeen(ushort vendor, ushort product, byte[] keyBits, byte[] absBits, GHEvdevVerdict verdict)
        {
            if (GHEvdevDeviceFilter.IsSteamVirtualGamepad(vendor, product))
                _sawSteamVirtual = true;
            else if (vendor == GHEvdevDeviceFilter.SteamVendorId && !GHEvdevDeviceFilter.IsAccepted(verdict)
                && GHEvdevDeviceFilter.IsGamepadLike(keyBits, absBits))
                _sawValvePhysical = true;
        }

        private void CheckHints()
        {
            _hintsChecked = true;
            if (_env.AllowSteamVirtual && !_sawSteamVirtual)
                WriteDeviceLog("GAMEPAD hint: no Steam virtual gamepad (GnollHack's Steam Input layout is not a gamepad layout?)");
            if (_devices.Count == 0 && _sawValvePhysical)
                WriteDeviceLog("GAMEPAD hint: only valve-physical device present (Steam Input off?)");
        }

        private void NoteDenied(string path, string node, string sysName, int errno, long nowTicks)
        {
            DeniedNode denied;
            if (!_denied.TryGetValue(path, out denied))
            {
                denied = new DeniedNode();
                denied.FirstDeniedTicks = nowTicks;
                _denied[path] = denied;
                WriteDeviceLog("GAMEPAD dev " + node + " open=" + GHEvdevProtocol.GetErrnoName(errno) + " sysname=\""
                    + (sysName ?? "?") + "\" verdict=skip (retrying)");
            }
            if (denied.RetriesStopped)
                return;

            double offset = denied.Attempt < RetryDelaysSeconds.Length
                ? RetryDelaysSeconds[denied.Attempt]
                : Seconds(denied.FirstDeniedTicks, nowTicks) + RetryIntervalSeconds;
            denied.Attempt++;
            if (offset > RetryWindowSeconds)
            {
                denied.NextRetryTicks = 0;
                denied.RetriesStopped = true;
                WriteDeviceLog("GAMEPAD dev " + node + " open=" + GHEvdevProtocol.GetErrnoName(errno)
                    + " retries stopped; waiting for IN_ATTRIB");
                return;
            }
            denied.NextRetryTicks = denied.FirstDeniedTicks + (long)(offset * Stopwatch.Frequency);
        }

        private void RetryDeniedNodes(long nowTicks)
        {
            if (_denied.Count == 0)
                return;
            _duePaths.Clear();
            foreach (KeyValuePair<string, DeniedNode> pair in _denied)
            {
                if (pair.Value.NextRetryTicks != 0 && nowTicks >= pair.Value.NextRetryTicks)
                    _duePaths.Add(pair.Key);
            }
            int i;
            for (i = 0; i < _duePaths.Count; i++)
            {
                if (FindDevice(_duePaths[i]) == null)
                    Probe(_duePaths[i], nowTicks);
            }
            _duePaths.Clear();
        }

        /* Reads until EAGAIN, a short read or MaxReadsPerDevice reads, decoding the events into the sink when
           decode is set; after a SYN_DROPPED the state is recovered from a snapshot. ENODEV and EIO mean the
           device is gone; any other error is a bug, logged as the device is closed. */
        private void ReadDevice(OpenDevice dev, long nowTicks, bool decode)
        {
            int reads = 0;
            while (dev.Fd >= 0 && reads < MaxReadsPerDevice)
            {
                reads++;
                nint n = GHEvdevNative.Read(dev.Fd, _readBuffer, (nuint)_readBuffer.Length);
                if (n < 0)
                {
                    int errno = GHEvdevNative.LastError;
                    if (errno == GHEvdevProtocol.EINTR)
                        continue;
                    if (errno == GHEvdevProtocol.EAGAIN)
                        break;
                    if (errno != GHEvdevProtocol.ENODEV && errno != GHEvdevProtocol.EIO)
                        WriteDeviceLog("GAMEPAD dev " + dev.Node + " read failed: " + GHEvdevProtocol.GetErrnoName(errno) + "; closing");
                    RemoveDevice(dev, GHEvdevProtocol.GetErrnoName(errno), nowTicks);
                    return;
                }
                if (n == 0)
                    break;

                /* Kernel timestamps are checked against the time of this read, which follows them */
                long readTicks = Stopwatch.GetTimestamp();
                int count = (int)n / GHEvdevProtocol.InputEventSize;
                int i;
                for (i = 0; i < count; i++)
                {
                    GHEvdevInputEvent e = GHEvdevProtocol.ParseInputEvent(_readBuffer, i * GHEvdevProtocol.InputEventSize);
                    if (decode)
                    {
                        if (_rawDump)
                            AppendRaw(dev, e);
                        dev.Decoder.Feed(e, ToEventTicks(dev, e, readTicks), _sink);
                    }
                    if (e.Type == GHEvdevProtocol.EV_ABS && e.Code <= GHEvdevProtocol.ABS_MAX)
                        dev.RawAbs[e.Code] = e.Value;
                }
                if (n < _readBuffer.Length)
                    break;
            }

            if (decode && dev.Fd >= 0 && dev.Decoder.NeedsResync)
            {
                dev.DroppedCount++;
                if (dev.DroppedCount == 1)
                    WriteDeviceLog("GAMEPAD dev " + dev.Node + " SYN_DROPPED; resynchronizing from a snapshot (logged once per device)");
                TakeSnapshot(dev, nowTicks, false);
            }
        }

        private long ToEventTicks(OpenDevice dev, GHEvdevInputEvent e, long readTicks)
        {
            bool fellBack;
            long ticks = GHEvdevClock.ToTicks(e.Seconds, e.Microseconds, dev.MonotonicClock, readTicks, Stopwatch.Frequency, out fellBack);
            if (fellBack && !dev.ClockFallbackLogged)
            {
                dev.ClockFallbackLogged = true;
                WriteDeviceLog("GAMEPAD dev " + dev.Node + " event timestamps fall back to the read time ("
                    + (dev.MonotonicClock ? "outside the last second" : "no monotonic clock") + "; logged once per device)");
            }
            return ticks;
        }

        /* One raw line per event, except stick-axis events that stay within the inner deadzone and SYN_REPORTs
           of frames whose events were all left out */
        private void AppendRaw(OpenDevice dev, GHEvdevInputEvent e)
        {
            if (e.Type == GHEvdevProtocol.EV_ABS && e.Code <= GHEvdevProtocol.ABS_MAX)
            {
                GHEvdevLayout layout = dev.Decoder.Layout;
                if (layout.IsStickAxisAtRest(e.Code, dev.RawAbs[e.Code], InnerDeadzone) && layout.IsStickAxisAtRest(e.Code, e.Value, InnerDeadzone))
                    return;
            }
            bool report = e.Type == GHEvdevProtocol.EV_SYN && e.Code == GHEvdevProtocol.SYN_REPORT;
            if (report)
            {
                bool hadLines = dev.RawFrameHasLines;
                dev.RawFrameHasLines = false;
                if (!hadLines)
                    return;
            }

            if (_rawLines >= MaxRawLines)
            {
                _rawBatch.Append("GAMEPAD raw dump stopped after ").Append(MaxRawLines).Append(" lines\n");
                _rawDump = false;
                return;
            }
            _rawBatch.Append("GAMEPAD raw cid=").Append(dev.ControllerId)
                .Append(" t=").Append(e.Seconds).Append('.').Append(e.Microseconds.ToString("D6", CultureInfo.InvariantCulture))
                .Append(" type=").Append(e.Type)
                .Append(" code=0x").Append(e.Code.ToString("x3", CultureInfo.InvariantCulture))
                .Append(" value=").Append(e.Value)
                .Append('\n');
            _rawLines++;
            if (!report)
                dev.RawFrameHasLines = true;
        }
        private void FlushRaw()
        {
            if (_rawBatch.Length == 0)
                return;
            try
            {
                Console.Error.Write(_rawBatch.ToString());
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
            _rawBatch.Clear();
        }

        private void RemoveDevice(OpenDevice dev, string reason, long nowTicks)
        {
            CloseDeviceFd(dev);
            _devices.Remove(dev);
            WriteDeviceLog("GAMEPAD dev " + dev.Node + " -removed " + reason + " cid=" + Format(dev.ControllerId)
                + " after " + Seconds(dev.OpenedTicks, nowTicks).ToString("F1", CultureInfo.InvariantCulture) + " s");
            _sink.ControllerRemoved(dev.ControllerId);
        }

        /* fd is set to -1 before close, so it is never closed twice */
        private static void CloseDeviceFd(OpenDevice dev)
        {
            int fd = dev.Fd;
            dev.Fd = -1;
            if (fd >= 0)
                GHEvdevNative.Close(fd);
        }

        private int AllocateControllerId()
        {
            int attempt;
            for (attempt = 0; attempt < MaxControllerId; attempt++)
            {
                int id = _nextControllerId;
                _nextControllerId = id >= MaxControllerId ? 1 : id + 1;
                if (!IsControllerIdInUse(id))
                    return id;
            }
            return _nextControllerId;
        }

        private bool IsControllerIdInUse(int id)
        {
            int i;
            for (i = 0; i < _devices.Count; i++)
            {
                if (_devices[i].ControllerId == id)
                    return true;
            }
            return false;
        }

        /* Re-read when the file's modification time changes; null without a readable file */
        private GHSteamVirtualGamepadInfo LoadVirtualGamepadInfo()
        {
            string path = _env.VirtualGamepadInfoPath;
            if (path == null)
                return null;
            try
            {
                DateTime writeTime = File.GetLastWriteTimeUtc(path);
                if (_virtualGamepadInfo == null || writeTime != _virtualGamepadInfoWriteTime)
                {
                    _virtualGamepadInfo = GHSteamVirtualGamepadInfo.Parse(File.ReadAllText(path));
                    _virtualGamepadInfoWriteTime = writeTime;
                }
                return _virtualGamepadInfo;
            }
            catch (Exception ex)
            {
                if (!_virtualGamepadInfoErrorLogged)
                {
                    _virtualGamepadInfoErrorLogged = true;
                    WriteDeviceLog("GAMEPAD cannot read " + path + ": " + ex.Message);
                }
                return null;
            }
        }

        private static string ReadSysfs(string node, string relativePath)
        {
            return ReadTextFile(SysClassInput + "/" + node + "/device/" + relativePath);
        }

        /* id/vendor, id/product and capabilities/key and abs from sysfs; properties when present */
        private static bool TryReadSysfsCapabilities(string node, out ushort vendor, out ushort product,
            out byte[] keyBits, out byte[] absBits, out byte[] propBits)
        {
            product = 0;
            keyBits = null;
            absBits = null;
            propBits = null;
            if (!GHEvdevProtocol.TryParseSysfsHex16(ReadSysfs(node, "id/vendor"), out vendor)
                || !GHEvdevProtocol.TryParseSysfsHex16(ReadSysfs(node, "id/product"), out product))
                return false;
            keyBits = GHEvdevProtocol.ParseSysfsBitmap(ReadSysfs(node, "capabilities/key"), GHEvdevProtocol.KEY_MAX);
            absBits = GHEvdevProtocol.ParseSysfsBitmap(ReadSysfs(node, "capabilities/abs"), GHEvdevProtocol.ABS_MAX);
            if (keyBits == null || absBits == null)
                return false;
            propBits = GHEvdevProtocol.ParseSysfsBitmap(ReadSysfs(node, "properties"), GHEvdevProtocol.INPUT_PROP_MAX);
            return true;
        }

        private static double Seconds(long fromTicks, long toTicks)
        {
            return (toTicks - fromTicks) / (double)Stopwatch.Frequency;
        }

        private static string Hex4(ushort value)
        {
            return value.ToString("x4", CultureInfo.InvariantCulture);
        }

        private static string Format(int value)
        {
            return value.ToString(CultureInfo.InvariantCulture);
        }

        private static string Format(double value)
        {
            return value.ToString("0.##", CultureInfo.InvariantCulture);
        }
    }
}
#endif
