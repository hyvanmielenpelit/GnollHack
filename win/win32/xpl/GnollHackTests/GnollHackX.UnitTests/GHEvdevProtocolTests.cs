using System;
using System.Collections.Generic;
using System.Text;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers GHEvdevProtocol: the ioctl request literals, the kernel structure layouts, bitmask tests,
       inotify records and sysfs capability bitmaps. The fixtures are the Steam virtual gamepad as measured
       on a Steam Deck: "Microsoft X-Box 360 pad 0", 0003:28de:11ff:0001, 11 keys and 8 axes. */
    public class GHEvdevProtocolTests
    {
        private static readonly int[] VirtualPadKeys =
        {
            0x130, 0x131, 0x133, 0x134, 0x136, 0x137, 0x13a, 0x13b, 0x13c, 0x13d, 0x13e
        };

        private static readonly int[] VirtualPadAxes =
        {
            GHEvdevProtocol.ABS_X, GHEvdevProtocol.ABS_Y, GHEvdevProtocol.ABS_Z, GHEvdevProtocol.ABS_RX,
            GHEvdevProtocol.ABS_RY, GHEvdevProtocol.ABS_RZ, GHEvdevProtocol.ABS_HAT0X, GHEvdevProtocol.ABS_HAT0Y
        };

        [Fact]
        public void IoctlRequests_MatchTheKernelEncodings()
        {
            Assert.Equal(0x80084502u, GHEvdevProtocol.EVIOCGID);
            Assert.Equal(0x81004506u, GHEvdevProtocol.EVIOCGNAME(256));
            Assert.Equal(0x80044509u, GHEvdevProtocol.EVIOCGPROP(4));
            Assert.Equal(0x80604518u, GHEvdevProtocol.EVIOCGKEY(96));
            Assert.Equal(0x80044520u, GHEvdevProtocol.EVIOCGBIT(0, 4));
            Assert.Equal(0x80604521u, GHEvdevProtocol.EVIOCGBIT(GHEvdevProtocol.EV_KEY, 96));
            Assert.Equal(0x80084523u, GHEvdevProtocol.EVIOCGBIT(GHEvdevProtocol.EV_ABS, 8));
            Assert.Equal(0x400445a0u, GHEvdevProtocol.EVIOCSCLOCKID);
            Assert.Equal(0x40044590u, GHEvdevProtocol.EVIOCGRAB);
        }

        [Theory]
        [InlineData(GHEvdevProtocol.ABS_X, 0x80184540u)]
        [InlineData(GHEvdevProtocol.ABS_Y, 0x80184541u)]
        [InlineData(GHEvdevProtocol.ABS_RZ, 0x80184545u)]
        [InlineData(GHEvdevProtocol.ABS_HAT0X, 0x80184550u)]
        [InlineData(GHEvdevProtocol.ABS_HAT0Y, 0x80184551u)]
        [InlineData(GHEvdevProtocol.ABS_MAX, 0x8018457fu)]
        public void EVIOCGABS_IsTheBaseRequestPlusTheAxis(int axis, uint expected)
        {
            Assert.Equal(expected, GHEvdevProtocol.EVIOCGABS(axis));
        }

        [Fact]
        public void BufferSizesAndFlags_MatchTheKernel()
        {
            Assert.Equal(96, GHEvdevProtocol.KeyBitsBytes);
            Assert.Equal(8, GHEvdevProtocol.AbsBitsBytes);
            Assert.Equal(4, GHEvdevProtocol.PropBitsBytes);
            Assert.Equal(4, GHEvdevProtocol.EventTypeBitsBytes);
            Assert.Equal(0x80800, GHEvdevProtocol.OpenFlags);
            Assert.Equal(0x3C4u, GHEvdevProtocol.DeviceDirectoryWatchMask);
        }

        [Fact]
        public void InputEvent_RoundTrips()
        {
            GHEvdevInputEvent e = new GHEvdevInputEvent();
            e.Seconds = 41234;
            e.Microseconds = 567890;
            e.Type = GHEvdevProtocol.EV_ABS;
            e.Code = GHEvdevProtocol.ABS_Y;
            e.Value = -32768;

            byte[] buffer = new byte[2 * GHEvdevProtocol.InputEventSize];
            GHEvdevProtocol.WriteInputEvent(e, buffer, GHEvdevProtocol.InputEventSize);
            GHEvdevInputEvent parsed = GHEvdevProtocol.ParseInputEvent(buffer, GHEvdevProtocol.InputEventSize);

            Assert.Equal(e.Seconds, parsed.Seconds);
            Assert.Equal(e.Microseconds, parsed.Microseconds);
            Assert.Equal(e.Type, parsed.Type);
            Assert.Equal(e.Code, parsed.Code);
            Assert.Equal(e.Value, parsed.Value);
        }

        [Fact]
        public void InputEvent_FieldsAreAtTheKernelOffsets()
        {
            Assert.True(BitConverter.IsLittleEndian);
            byte[] buffer = new byte[GHEvdevProtocol.InputEventSize];
            buffer[0] = 0x01;                   /* tv_sec = 1 */
            buffer[8] = 0x02;                   /* tv_usec = 2 */
            buffer[16] = 0x01;                  /* type = EV_KEY */
            buffer[18] = 0x30;                  /* code = 0x130 */
            buffer[19] = 0x01;
            buffer[20] = 0xFF;                  /* value = -1 */
            buffer[21] = 0xFF;
            buffer[22] = 0xFF;
            buffer[23] = 0xFF;

            GHEvdevInputEvent e = GHEvdevProtocol.ParseInputEvent(buffer, 0);

            Assert.Equal(1L, e.Seconds);
            Assert.Equal(2L, e.Microseconds);
            Assert.Equal(GHEvdevProtocol.EV_KEY, e.Type);
            Assert.Equal(GHEvdevProtocol.BTN_SOUTH, e.Code);
            Assert.Equal(-1, e.Value);
        }

        [Fact]
        public void AbsInfo_ParsesTheSixFields()
        {
            /* The virtual pad's stick axis: -32767..32767, fuzz 16, flat 128 */
            byte[] buffer = new byte[GHEvdevProtocol.AbsInfoSize];
            int[] fields = { 1530, -32767, 32767, 16, 128, 0 };
            int i;
            for (i = 0; i < fields.Length; i++)
                Array.Copy(BitConverter.GetBytes(fields[i]), 0, buffer, 4 * i, 4);

            GHEvdevAbsInfo a = GHEvdevProtocol.ParseAbsInfo(buffer, 0);

            Assert.Equal(1530, a.Value);
            Assert.Equal(-32767, a.Minimum);
            Assert.Equal(32767, a.Maximum);
            Assert.Equal(16, a.Fuzz);
            Assert.Equal(128, a.Flat);
            Assert.Equal(0, a.Resolution);
        }

        [Fact]
        public void InputId_ParsesBusVendorProductVersion()
        {
            byte[] buffer = { 0x03, 0x00, 0xde, 0x28, 0xff, 0x11, 0x01, 0x00 };

            GHEvdevInputId id = GHEvdevProtocol.ParseInputId(buffer, 0);

            Assert.Equal(0x0003, id.BusType);
            Assert.Equal(0x28de, id.Vendor);
            Assert.Equal(0x11ff, id.Product);
            Assert.Equal(0x0001, id.Version);
        }

        [Fact]
        public void Name_StopsAtTheFirstNul()
        {
            byte[] buffer = new byte[GHEvdevProtocol.NameBytes];
            byte[] name = Encoding.ASCII.GetBytes("Microsoft X-Box 360 pad 0");
            Array.Copy(name, buffer, name.Length);

            /* EVIOCGNAME returns the bytes copied, including the NUL */
            Assert.Equal("Microsoft X-Box 360 pad 0", GHEvdevProtocol.ParseName(buffer, 0, name.Length + 1));
            Assert.Equal("Microsoft X-Box 360 pad 0", GHEvdevProtocol.ParseName(buffer, 0, buffer.Length));
            Assert.Equal("Microsoft", GHEvdevProtocol.ParseName(buffer, 0, 9));
            Assert.Equal("", GHEvdevProtocol.ParseName(buffer, 0, 0));
        }

        [Fact]
        public void Name_DecodesUtf8()
        {
            string expected = "Manette " + (char)0x00E9 + "tendue";
            byte[] utf8 = Encoding.UTF8.GetBytes(expected);
            byte[] buffer = new byte[utf8.Length + 8];
            Array.Copy(utf8, buffer, utf8.Length);

            Assert.Equal(expected, GHEvdevProtocol.ParseName(buffer, 0, buffer.Length));
        }

        [Fact]
        public void TestBit_ReadsTheKernelBitLayout()
        {
            byte[] keyBits = Bits(GHEvdevProtocol.KeyBitsBytes, VirtualPadKeys);

            foreach (int code in VirtualPadKeys)
                Assert.True(GHEvdevProtocol.TestBit(keyBits, code), "0x" + code.ToString("x"));
            Assert.False(GHEvdevProtocol.TestBit(keyBits, GHEvdevProtocol.BTN_C));
            Assert.False(GHEvdevProtocol.TestBit(keyBits, GHEvdevProtocol.BTN_TL2));
            Assert.False(GHEvdevProtocol.TestBit(keyBits, GHEvdevProtocol.BTN_TOUCH));
            Assert.False(GHEvdevProtocol.TestBit(keyBits, -1));
            Assert.False(GHEvdevProtocol.TestBit(keyBits, 8 * GHEvdevProtocol.KeyBitsBytes));
            Assert.False(GHEvdevProtocol.TestBit(null, 0));
            Assert.Equal(11, GHEvdevProtocol.CountBits(keyBits));
        }

        [Fact]
        public void InotifyRecords_ParseNamesWithPaddingAndAnEmptyOverflow()
        {
            List<byte> bytes = new List<byte>();
            AddInotifyRecord(bytes, 1, GHEvdevProtocol.IN_CREATE, 0, "event10", 16);
            AddInotifyRecord(bytes, 1, GHEvdevProtocol.IN_ATTRIB, 0, "event10", 16);
            AddInotifyRecord(bytes, 1, GHEvdevProtocol.IN_MOVED_FROM, 77, "js0", 4);
            AddInotifyRecord(bytes, -1, GHEvdevProtocol.IN_Q_OVERFLOW, 0, null, 0);
            int complete = bytes.Count;
            AddInotifyRecord(bytes, 1, GHEvdevProtocol.IN_DELETE, 0, "event11", 16);
            byte[] buffer = bytes.ToArray();
            int truncated = complete + GHEvdevProtocol.InotifyHeaderSize + 8;

            List<GHInotifyRecord> records = new List<GHInotifyRecord>();
            int consumed = GHEvdevProtocol.ParseInotifyRecords(buffer, truncated, records);

            Assert.Equal(complete, consumed);
            Assert.Equal(4, records.Count);
            Assert.Equal(1, records[0].WatchDescriptor);
            Assert.Equal(GHEvdevProtocol.IN_CREATE, records[0].Mask);
            Assert.Equal("event10", records[0].Name);
            Assert.Equal(GHEvdevProtocol.IN_ATTRIB, records[1].Mask);
            Assert.Equal("event10", records[1].Name);
            Assert.Equal(77u, records[2].Cookie);
            Assert.Equal("js0", records[2].Name);
            Assert.Equal(-1, records[3].WatchDescriptor);
            Assert.Equal(GHEvdevProtocol.IN_Q_OVERFLOW, records[3].Mask);
            Assert.Equal("", records[3].Name);

            records.Clear();
            Assert.Equal(buffer.Length, GHEvdevProtocol.ParseInotifyRecords(buffer, buffer.Length, records));
            Assert.Equal(5, records.Count);
            Assert.Equal("event11", records[4].Name);
        }

        [Fact]
        public void SysfsBitmap_VirtualPadKeys()
        {
            /* Bits 0x130-0x13e are bits 48-62 of the fifth 64-bit word; four zero words follow it */
            byte[] bits = GHEvdevProtocol.ParseSysfsBitmap("7cdb000000000000 0 0 0 0\n", GHEvdevProtocol.KEY_MAX);

            Assert.NotNull(bits);
            Assert.Equal(GHEvdevProtocol.KeyBitsBytes, bits.Length);
            Assert.Equal(Bits(GHEvdevProtocol.KeyBitsBytes, VirtualPadKeys), bits);
            Assert.True(GHEvdevProtocol.TestBit(bits, GHEvdevProtocol.BTN_SOUTH));
            Assert.False(GHEvdevProtocol.TestBit(bits, GHEvdevProtocol.BTN_TL2));
        }

        [Fact]
        public void SysfsBitmap_VirtualPadAxes()
        {
            byte[] bits = GHEvdevProtocol.ParseSysfsBitmap("3003f", GHEvdevProtocol.ABS_MAX);

            Assert.Equal(Bits(GHEvdevProtocol.AbsBitsBytes, VirtualPadAxes), bits);
        }

        [Fact]
        public void SysfsBitmap_KeyboardHasNoGamepadButtons()
        {
            byte[] bits = GHEvdevProtocol.ParseSysfsBitmap("402000000 3803078f800d001 feffffdfffefffff fffffffffffffffe",
                GHEvdevProtocol.KEY_MAX);

            Assert.NotNull(bits);
            Assert.True(GHEvdevProtocol.TestBit(bits, 1));         /* KEY_ESC */
            Assert.False(GHEvdevProtocol.TestBit(bits, 0));        /* KEY_RESERVED */
            Assert.False(GHEvdevProtocol.TestBit(bits, GHEvdevProtocol.BTN_SOUTH));
        }

        [Fact]
        public void SysfsBitmap_RejectsJunkAndAcceptsZero()
        {
            Assert.Null(GHEvdevProtocol.ParseSysfsBitmap(null, GHEvdevProtocol.KEY_MAX));
            Assert.Null(GHEvdevProtocol.ParseSysfsBitmap("", GHEvdevProtocol.KEY_MAX));
            Assert.Null(GHEvdevProtocol.ParseSysfsBitmap("  \n", GHEvdevProtocol.KEY_MAX));
            Assert.Null(GHEvdevProtocol.ParseSysfsBitmap("7cdb 0 zz", GHEvdevProtocol.KEY_MAX));
            Assert.Null(GHEvdevProtocol.ParseSysfsBitmap("10000000000000000", GHEvdevProtocol.KEY_MAX));

            byte[] zero = GHEvdevProtocol.ParseSysfsBitmap("0\n", GHEvdevProtocol.KEY_MAX);
            Assert.NotNull(zero);
            Assert.Equal(0, GHEvdevProtocol.CountBits(zero));
        }

        [Fact]
        public void SysfsBitmap_IgnoresBitsAboveTheMaximum()
        {
            byte[] bits = GHEvdevProtocol.ParseSysfsBitmap("1 0", GHEvdevProtocol.ABS_MAX);

            Assert.Equal(GHEvdevProtocol.AbsBitsBytes, bits.Length);
            Assert.Equal(0, GHEvdevProtocol.CountBits(bits));
        }

        [Theory]
        [InlineData("28de\n", true, 0x28de)]
        [InlineData("11FF", true, 0x11ff)]
        [InlineData("0001", true, 0x0001)]
        [InlineData("", false, 0)]
        [InlineData("xyz", false, 0)]
        [InlineData(null, false, 0)]
        public void SysfsHex16_ParsesIdFiles(string text, bool ok, int expected)
        {
            ushort value;
            Assert.Equal(ok, GHEvdevProtocol.TryParseSysfsHex16(text, out value));
            Assert.Equal(expected, value);
        }

        internal static byte[] Bits(int size, params int[] codes)
        {
            byte[] bits = new byte[size];
            foreach (int code in codes)
                bits[code >> 3] |= (byte)(1 << (code & 7));
            return bits;
        }

        private static void AddInotifyRecord(List<byte> bytes, int wd, uint mask, uint cookie, string name, int paddedLength)
        {
            bytes.AddRange(BitConverter.GetBytes(wd));
            bytes.AddRange(BitConverter.GetBytes(mask));
            bytes.AddRange(BitConverter.GetBytes(cookie));
            bytes.AddRange(BitConverter.GetBytes((uint)paddedLength));
            byte[] padded = new byte[paddedLength];
            if (name != null)
            {
                byte[] nameBytes = Encoding.ASCII.GetBytes(name);
                Array.Copy(nameBytes, padded, nameBytes.Length);
            }
            bytes.AddRange(padded);
        }
    }
}
