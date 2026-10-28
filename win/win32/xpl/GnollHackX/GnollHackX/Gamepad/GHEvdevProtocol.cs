using System;
using System.Collections.Generic;
using System.Globalization;
using System.Text;

namespace GnollHackX
{
    /* struct input_event: kernel timestamp, then type, code and value */
    public struct GHEvdevInputEvent
    {
        public long Seconds;
        public long Microseconds;
        public ushort Type;
        public ushort Code;
        public int Value;
    }

    /* struct input_absinfo */
    public struct GHEvdevAbsInfo
    {
        public int Value;
        public int Minimum;
        public int Maximum;
        public int Fuzz;
        public int Flat;
        public int Resolution;
    }

    /* struct input_id */
    public struct GHEvdevInputId
    {
        public ushort BusType;
        public ushort Vendor;
        public ushort Product;
        public ushort Version;
    }

    /* struct inotify_event, with the name up to its first NUL */
    public struct GHInotifyRecord
    {
        public int WatchDescriptor;
        public uint Mask;
        public uint Cookie;
        public string Name;     /* empty when the record carries no name */
    }

    /* Linux evdev and inotify definitions for x86-64 (include/uapi/linux/input.h, input-event-codes.h,
       inotify.h), and parsing of the kernel's structures from byte buffers in the machine's byte order */
    public static class GHEvdevProtocol
    {
        /* Event types */
        public const ushort EV_SYN = 0x00;
        public const ushort EV_KEY = 0x01;
        public const ushort EV_REL = 0x02;
        public const ushort EV_ABS = 0x03;
        public const ushort EV_MSC = 0x04;

        public const ushort SYN_REPORT = 0;
        public const ushort SYN_DROPPED = 3;

        /* Keys */
        public const int BTN_LEFT = 0x110;
        public const int BTN_SOUTH = 0x130;
        public const int BTN_EAST = 0x131;
        public const int BTN_C = 0x132;
        public const int BTN_NORTH = 0x133;     /* = BTN_X */
        public const int BTN_X = 0x133;
        public const int BTN_WEST = 0x134;      /* = BTN_Y */
        public const int BTN_Y = 0x134;
        public const int BTN_Z = 0x135;
        public const int BTN_TL = 0x136;
        public const int BTN_TR = 0x137;
        public const int BTN_TL2 = 0x138;
        public const int BTN_TR2 = 0x139;
        public const int BTN_SELECT = 0x13a;
        public const int BTN_START = 0x13b;
        public const int BTN_MODE = 0x13c;
        public const int BTN_THUMBL = 0x13d;
        public const int BTN_THUMBR = 0x13e;
        public const int BTN_TOUCH = 0x14a;
        public const int BTN_DPAD_UP = 0x220;
        public const int BTN_DPAD_DOWN = 0x221;
        public const int BTN_DPAD_LEFT = 0x222;
        public const int BTN_DPAD_RIGHT = 0x223;
        public const int KEY_MAX = 0x2ff;

        /* Absolute axes */
        public const int ABS_X = 0x00;
        public const int ABS_Y = 0x01;
        public const int ABS_Z = 0x02;
        public const int ABS_RX = 0x03;
        public const int ABS_RY = 0x04;
        public const int ABS_RZ = 0x05;
        public const int ABS_HAT0X = 0x10;
        public const int ABS_HAT0Y = 0x11;
        public const int ABS_HAT2X = 0x14;
        public const int ABS_HAT2Y = 0x15;
        public const int ABS_MAX = 0x3f;

        /* Device properties */
        public const int INPUT_PROP_ACCELEROMETER = 0x06;
        public const int INPUT_PROP_MAX = 0x1f;

        /* Buffer sizes for the capability queries: one bit per code up to the maximum */
        public const int EventTypeBitsBytes = 4;
        public const int KeyBitsBytes = (KEY_MAX + 1) / 8;     /* 96 */
        public const int AbsBitsBytes = (ABS_MAX + 1) / 8;     /* 8 */
        public const int PropBitsBytes = (INPUT_PROP_MAX + 1) / 8;
        public const int NameBytes = 256;

        /* Structure sizes */
        public const int InputEventSize = 24;
        public const int AbsInfoSize = 24;
        public const int InputIdSize = 8;
        public const int InotifyHeaderSize = 16;

        /* errno */
        public const int EPERM = 1;
        public const int ENOENT = 2;
        public const int EINTR = 4;
        public const int EIO = 5;
        public const int ENXIO = 6;
        public const int EAGAIN = 11;
        public const int EACCES = 13;
        public const int ENODEV = 19;
        public const int EINVAL = 22;

        /* open and inotify_init1 flags */
        public const int O_RDONLY = 0x0;
        public const int O_NONBLOCK = 0x800;
        public const int O_CLOEXEC = 0x80000;
        public const int OpenFlags = O_RDONLY | O_NONBLOCK | O_CLOEXEC;   /* 0x80800 */
        public const int IN_NONBLOCK = O_NONBLOCK;
        public const int IN_CLOEXEC = O_CLOEXEC;

        /* inotify event masks */
        public const uint IN_ATTRIB = 0x00000004;
        public const uint IN_MOVED_FROM = 0x00000040;
        public const uint IN_MOVED_TO = 0x00000080;
        public const uint IN_CREATE = 0x00000100;
        public const uint IN_DELETE = 0x00000200;
        public const uint IN_Q_OVERFLOW = 0x00004000;
        public const uint IN_IGNORED = 0x00008000;
        public const uint IN_ISDIR = 0x40000000;
        public const uint DeviceDirectoryWatchMask = IN_CREATE | IN_DELETE | IN_ATTRIB | IN_MOVED_TO | IN_MOVED_FROM;  /* 0x3C4 */

        public const int CLOCK_MONOTONIC = 1;

        /* _IOC(dir, type, nr, size) */
        public const uint IOC_WRITE = 1;
        public const uint IOC_READ = 2;

        public static uint Ioc(uint dir, uint type, uint nr, uint size)
        {
            return (dir << 30) | (size << 16) | (type << 8) | nr;
        }

        private const uint EvdevIocType = 0x45;     /* 'E' */

        public static readonly uint EVIOCGID = Ioc(IOC_READ, EvdevIocType, 0x02, InputIdSize);
        public static readonly uint EVIOCSCLOCKID = Ioc(IOC_WRITE, EvdevIocType, 0xa0, 4);
        /* Never used: a grab would take the shared Steam virtual pad from every other app */
        public static readonly uint EVIOCGRAB = Ioc(IOC_WRITE, EvdevIocType, 0x90, 4);

        public static uint EVIOCGNAME(int length)
        {
            return Ioc(IOC_READ, EvdevIocType, 0x06, (uint)length);
        }

        public static uint EVIOCGPROP(int length)
        {
            return Ioc(IOC_READ, EvdevIocType, 0x09, (uint)length);
        }

        public static uint EVIOCGKEY(int length)
        {
            return Ioc(IOC_READ, EvdevIocType, 0x18, (uint)length);
        }

        /* eventType 0 queries the supported event types */
        public static uint EVIOCGBIT(int eventType, int length)
        {
            return Ioc(IOC_READ, EvdevIocType, (uint)(0x20 + eventType), (uint)length);
        }

        public static uint EVIOCGABS(int axis)
        {
            return Ioc(IOC_READ, EvdevIocType, (uint)(0x40 + axis), AbsInfoSize);
        }

        public static GHEvdevInputEvent ParseInputEvent(byte[] buffer, int offset)
        {
            GHEvdevInputEvent e;
            e.Seconds = BitConverter.ToInt64(buffer, offset);
            e.Microseconds = BitConverter.ToInt64(buffer, offset + 8);
            e.Type = BitConverter.ToUInt16(buffer, offset + 16);
            e.Code = BitConverter.ToUInt16(buffer, offset + 18);
            e.Value = BitConverter.ToInt32(buffer, offset + 20);
            return e;
        }

        public static void WriteInputEvent(GHEvdevInputEvent e, byte[] buffer, int offset)
        {
            Array.Copy(BitConverter.GetBytes(e.Seconds), 0, buffer, offset, 8);
            Array.Copy(BitConverter.GetBytes(e.Microseconds), 0, buffer, offset + 8, 8);
            Array.Copy(BitConverter.GetBytes(e.Type), 0, buffer, offset + 16, 2);
            Array.Copy(BitConverter.GetBytes(e.Code), 0, buffer, offset + 18, 2);
            Array.Copy(BitConverter.GetBytes(e.Value), 0, buffer, offset + 20, 4);
        }

        public static GHEvdevAbsInfo ParseAbsInfo(byte[] buffer, int offset)
        {
            GHEvdevAbsInfo a;
            a.Value = BitConverter.ToInt32(buffer, offset);
            a.Minimum = BitConverter.ToInt32(buffer, offset + 4);
            a.Maximum = BitConverter.ToInt32(buffer, offset + 8);
            a.Fuzz = BitConverter.ToInt32(buffer, offset + 12);
            a.Flat = BitConverter.ToInt32(buffer, offset + 16);
            a.Resolution = BitConverter.ToInt32(buffer, offset + 20);
            return a;
        }

        public static GHEvdevInputId ParseInputId(byte[] buffer, int offset)
        {
            GHEvdevInputId id;
            id.BusType = BitConverter.ToUInt16(buffer, offset);
            id.Vendor = BitConverter.ToUInt16(buffer, offset + 2);
            id.Product = BitConverter.ToUInt16(buffer, offset + 4);
            id.Version = BitConverter.ToUInt16(buffer, offset + 6);
            return id;
        }

        /* UTF-8 up to the first NUL or length bytes, whichever comes first */
        public static string ParseName(byte[] buffer, int offset, int length)
        {
            if (buffer == null || length <= 0)
                return "";
            int end = Math.Min(buffer.Length, offset + length);
            int i;
            for (i = offset; i < end; i++)
            {
                if (buffer[i] == 0)
                    break;
            }
            return Encoding.UTF8.GetString(buffer, offset, i - offset);
        }

        /* A capability or state bitmask as the kernel fills it: bit n is bit n % 8 of byte n / 8 */
        public static bool TestBit(byte[] bits, int bit)
        {
            if (bits == null || bit < 0)
                return false;
            int index = bit >> 3;
            if (index >= bits.Length)
                return false;
            return (bits[index] & (1 << (bit & 7))) != 0;
        }

        public static int CountBits(byte[] bits)
        {
            if (bits == null)
                return 0;
            int count = 0;
            int i;
            for (i = 0; i < bits.Length; i++)
            {
                int b = bits[i];
                while (b != 0)
                {
                    count += b & 1;
                    b >>= 1;
                }
            }
            return count;
        }

        /* Appends the complete records in buffer[0, count) and returns the bytes consumed. A record is
           the 16-byte header followed by len bytes of NUL-padded name. */
        public static int ParseInotifyRecords(byte[] buffer, int count, List<GHInotifyRecord> records)
        {
            int offset = 0;
            while (count - offset >= InotifyHeaderSize)
            {
                int nameLength = (int)BitConverter.ToUInt32(buffer, offset + 12);
                if (nameLength < 0 || count - offset - InotifyHeaderSize < nameLength)
                    break;
                GHInotifyRecord record;
                record.WatchDescriptor = BitConverter.ToInt32(buffer, offset);
                record.Mask = BitConverter.ToUInt32(buffer, offset + 4);
                record.Cookie = BitConverter.ToUInt32(buffer, offset + 8);
                record.Name = ParseName(buffer, offset + InotifyHeaderSize, nameLength);
                records.Add(record);
                offset += InotifyHeaderSize + nameLength;
            }
            return offset;
        }

        /* A sysfs capability file such as /sys/class/input/event10/device/capabilities/key: 64-bit words in
           hex, most significant first, separated by spaces, with leading zero words omitted. Returns the
           bitmask in the layout of TestBit, sized for maxBit, or null when the text is not such a list. */
        public static byte[] ParseSysfsBitmap(string text, int maxBit)
        {
            if (text == null || maxBit < 0)
                return null;
            string[] words = text.Split(new char[] { ' ', '\t', '\r', '\n' }, StringSplitOptions.RemoveEmptyEntries);
            if (words.Length == 0)
                return null;

            byte[] bits = new byte[maxBit / 8 + 1];
            int i;
            for (i = 0; i < words.Length; i++)
            {
                ulong word;
                if (words[i].Length > 16
                    || !ulong.TryParse(words[i], NumberStyles.AllowHexSpecifier, CultureInfo.InvariantCulture, out word))
                    return null;

                int baseBit = 64 * (words.Length - 1 - i);
                int b;
                for (b = 0; b < 64; b++)
                {
                    if (((word >> b) & 1UL) == 0)
                        continue;
                    int bit = baseBit + b;
                    if (bit <= maxBit)
                        bits[bit >> 3] |= (byte)(1 << (bit & 7));
                }
            }
            return bits;
        }

        /* A sysfs id file such as id/vendor: four hex digits */
        public static bool TryParseSysfsHex16(string text, out ushort value)
        {
            value = 0;
            if (text == null)
                return false;
            return ushort.TryParse(text.Trim(), NumberStyles.AllowHexSpecifier, CultureInfo.InvariantCulture, out value);
        }

        /* Log names for the common axes; others as hex */
        public static string GetAxisName(int axis)
        {
            switch (axis)
            {
            case ABS_X:
                return "X";
            case ABS_Y:
                return "Y";
            case ABS_Z:
                return "Z";
            case ABS_RX:
                return "RX";
            case ABS_RY:
                return "RY";
            case ABS_RZ:
                return "RZ";
            case ABS_HAT0X:
                return "HAT0X";
            case ABS_HAT0Y:
                return "HAT0Y";
            case ABS_HAT2X:
                return "HAT2X";
            case ABS_HAT2Y:
                return "HAT2Y";
            default:
                return "ABS0x" + axis.ToString("x2", CultureInfo.InvariantCulture);
            }
        }

        /* Name of an errno value, for logs */
        public static string GetErrnoName(int errno)
        {
            switch (errno)
            {
            case EPERM:
                return "EPERM";
            case ENOENT:
                return "ENOENT";
            case EINTR:
                return "EINTR";
            case EIO:
                return "EIO";
            case ENXIO:
                return "ENXIO";
            case EAGAIN:
                return "EAGAIN";
            case EACCES:
                return "EACCES";
            case ENODEV:
                return "ENODEV";
            case EINVAL:
                return "EINVAL";
            default:
                return "errno " + errno.ToString(CultureInfo.InvariantCulture);
            }
        }
    }
}
