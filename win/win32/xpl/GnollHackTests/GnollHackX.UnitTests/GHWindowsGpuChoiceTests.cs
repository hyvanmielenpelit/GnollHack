using System;
using System.Collections.Generic;
using GnollHackM;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers GHWindowsGpuChoice: the adapter index for each GPU mode and Windows preference, the fallbacks to
       Avalonia's own choice, the UserGpuPreferences value, the --win-gpu mode names, the LUID bytes Avalonia
       passes, and the rule of GHApp.InformAboutGPU. */
    public class GHWindowsGpuChoiceTests
    {
        private const long IntelLuid = 0x0000D1A3L;
        private const long RtxLuid = 0x0000E2B4L;
        private const long WarpLuid = 0x0000F3C5L;

        private static List<long> Luids(params long[] luids)
        {
            return new List<long>(luids);
        }

        [Theory]
        [InlineData(0)]
        [InlineData(1)]
        [InlineData(2)]
        public void Auto_Unset_PicksHighPerformanceAtAnyIndex(int rtxIndex)
        {
            List<long> luids = Luids(IntelLuid, WarpLuid);
            luids.Insert(rtxIndex, RtxLuid);

            int index = GHWindowsGpuChoice.ChooseIndex(luids, GHWindowsGpuMode.Auto, GHWindowsGpuPreference.Unset, RtxLuid, IntelLuid);

            Assert.Equal(rtxIndex, index);
        }

        [Fact]
        public void Auto_IntegratedPreference_PicksMinimumPower()
        {
            List<long> luids = Luids(RtxLuid, IntelLuid, WarpLuid);

            int index = GHWindowsGpuChoice.ChooseIndex(luids, GHWindowsGpuMode.Auto, GHWindowsGpuPreference.Integrated, RtxLuid, IntelLuid);

            Assert.Equal(1, index);
        }

        [Fact]
        public void Auto_DedicatedPreference_PicksHighPerformance()
        {
            List<long> luids = Luids(IntelLuid, RtxLuid, WarpLuid);

            int index = GHWindowsGpuChoice.ChooseIndex(luids, GHWindowsGpuMode.Auto, GHWindowsGpuPreference.Dedicated, RtxLuid, IntelLuid);

            Assert.Equal(1, index);
        }

        [Theory]
        [InlineData(GHWindowsGpuPreference.Unset)]
        [InlineData(GHWindowsGpuPreference.Integrated)]
        [InlineData(GHWindowsGpuPreference.Dedicated)]
        public void DedicatedMode_IgnoresPreference(GHWindowsGpuPreference preference)
        {
            List<long> luids = Luids(IntelLuid, RtxLuid, WarpLuid);

            int index = GHWindowsGpuChoice.ChooseIndex(luids, GHWindowsGpuMode.Dedicated, preference, RtxLuid, IntelLuid);

            Assert.Equal(1, index);
        }

        [Theory]
        [InlineData(GHWindowsGpuPreference.Unset)]
        [InlineData(GHWindowsGpuPreference.Integrated)]
        [InlineData(GHWindowsGpuPreference.Dedicated)]
        public void IntegratedMode_IgnoresPreference(GHWindowsGpuPreference preference)
        {
            List<long> luids = Luids(RtxLuid, WarpLuid, IntelLuid);

            int index = GHWindowsGpuChoice.ChooseIndex(luids, GHWindowsGpuMode.Integrated, preference, RtxLuid, IntelLuid);

            Assert.Equal(2, index);
        }

        [Fact]
        public void WantedLuidNotInList_GivesZero()
        {
            List<long> luids = Luids(IntelLuid, WarpLuid);

            int index = GHWindowsGpuChoice.ChooseIndex(luids, GHWindowsGpuMode.Auto, GHWindowsGpuPreference.Unset, RtxLuid, IntelLuid);

            Assert.Equal(0, index);
        }

        [Theory]
        [InlineData(GHWindowsGpuMode.Auto)]
        [InlineData(GHWindowsGpuMode.Dedicated)]
        [InlineData(GHWindowsGpuMode.Integrated)]
        public void NullLuids_GiveZero(GHWindowsGpuMode mode)
        {
            List<long> luids = Luids(WarpLuid, IntelLuid, RtxLuid);

            int index = GHWindowsGpuChoice.ChooseIndex(luids, mode, GHWindowsGpuPreference.Unset, null, null);

            Assert.Equal(0, index);
        }

        [Fact]
        public void EmptyOrNullList_GivesZero()
        {
            Assert.Equal(0, GHWindowsGpuChoice.ChooseIndex(Luids(), GHWindowsGpuMode.Auto, GHWindowsGpuPreference.Unset, RtxLuid, IntelLuid));
            Assert.Equal(0, GHWindowsGpuChoice.ChooseIndex(null, GHWindowsGpuMode.Dedicated, GHWindowsGpuPreference.Unset, RtxLuid, IntelLuid));
        }

        [Theory]
        [InlineData("GpuPreference=2;", GHWindowsGpuPreference.Dedicated)]
        [InlineData("GpuPreference=1;", GHWindowsGpuPreference.Integrated)]
        [InlineData("GpuPreference=0;", GHWindowsGpuPreference.Unset)]
        [InlineData("SwapEffectUpgradeEnable=1;GpuPreference=2;", GHWindowsGpuPreference.Dedicated)]
        [InlineData("SwapEffectUpgradeEnable=1;GpuPreference=1;", GHWindowsGpuPreference.Integrated)]
        [InlineData("GpuPreference=x;", GHWindowsGpuPreference.Unset)]
        [InlineData("SwapEffectUpgradeEnable=1;", GHWindowsGpuPreference.Unset)]
        [InlineData("", GHWindowsGpuPreference.Unset)]
        [InlineData(null, GHWindowsGpuPreference.Unset)]
        public void ParsePreferenceValue_ReadsGpuPreference(string data, GHWindowsGpuPreference expected)
        {
            Assert.Equal(expected, GHWindowsGpuChoice.ParsePreferenceValue(data));
        }

        [Theory]
        [InlineData(GHWindowsGpuPreference.Unset, "Unset")]
        [InlineData(GHWindowsGpuPreference.Integrated, "Integrated")]
        [InlineData(GHWindowsGpuPreference.Dedicated, "Dedicated")]
        public void PreferenceName_NamesEachPreference(GHWindowsGpuPreference preference, string expected)
        {
            Assert.Equal(expected, GHWindowsGpuChoice.PreferenceName(preference));
        }

        [Theory]
        [InlineData("auto", GHWindowsGpuMode.Auto)]
        [InlineData("dedicated", GHWindowsGpuMode.Dedicated)]
        [InlineData("integrated", GHWindowsGpuMode.Integrated)]
        [InlineData("  DediCated ", GHWindowsGpuMode.Dedicated)]
        [InlineData(" INTEGRATED", GHWindowsGpuMode.Integrated)]
        public void TryParseMode_KnownNames(string text, GHWindowsGpuMode expected)
        {
            GHWindowsGpuMode mode;

            bool parsed = GHWindowsGpuChoice.TryParseMode(text, out mode);

            Assert.True(parsed);
            Assert.Equal(expected, mode);
            Assert.Equal(expected, ParseModeName(GHWindowsGpuChoice.ModeName(mode)));
        }

        [Theory]
        [InlineData("bogus")]
        [InlineData("avalonia")]
        [InlineData("")]
        [InlineData(null)]
        public void TryParseMode_UnknownWord_IsFalse(string text)
        {
            GHWindowsGpuMode mode;

            Assert.False(GHWindowsGpuChoice.TryParseMode(text, out mode));
        }

        private static GHWindowsGpuMode ParseModeName(string name)
        {
            GHWindowsGpuMode mode;
            Assert.True(GHWindowsGpuChoice.TryParseMode(name, out mode));
            return mode;
        }

        [Theory]
        [InlineData(0L)]
        [InlineData(0x0000D1A3L)]
        [InlineData(0x00000001_0000D1A3L)]
        [InlineData(unchecked((long)0xFFFFFFFF_0000D1A3UL))]
        [InlineData(long.MinValue)]
        public void LuidFromBytes_RoundTrips(long luid)
        {
            Assert.Equal(luid, GHWindowsGpuChoice.LuidFromBytes(BitConverter.GetBytes(luid)));
        }

        [Fact]
        public void LuidFromBytes_NullOrShort_GivesZero()
        {
            Assert.Equal(0L, GHWindowsGpuChoice.LuidFromBytes(null));
            Assert.Equal(0L, GHWindowsGpuChoice.LuidFromBytes(new byte[] { 0xA3, 0xD1, 0x00, 0x00 }));
        }

        [Theory]
        [InlineData(true, true, false, "", true)]
        [InlineData(true, false, false, "Integrated", false)]
        [InlineData(false, true, true, "Auto", false)]
        [InlineData(false, true, false, "Integrated", false)]
        public void ShouldInformAboutGpu_KnownAdapter(bool renderIsIntegrated, bool discretePresent, bool isPackaged,
            string activeGpu, bool expected)
        {
            Assert.Equal(expected, GHWindowsGpuChoice.ShouldInformAboutGpu(renderIsIntegrated, discretePresent, isPackaged, activeGpu));
        }

        [Theory]
        [InlineData(true, "Auto", true)]
        [InlineData(true, "Dedicated", false)]
        [InlineData(false, "", false)]
        [InlineData(false, "Integrated", true)]
        public void ShouldInformAboutGpu_UnknownAdapter_KeepsRegistryRule(bool isPackaged, string activeGpu, bool expected)
        {
            Assert.Equal(expected, GHWindowsGpuChoice.ShouldInformAboutGpu(null, true, isPackaged, activeGpu));
        }
    }
}
