using System;
using System.Collections.Generic;
using GnollHackM;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers GHWindowsGpuChoice: the adapter index for each GPU mode and Windows preference, the fallbacks to
       Avalonia's own choice, the wanted LUID, the UserGpuPreferences value and GHApp.GetActiveGPU's text, the
       --win-gpu mode names, the WinUI 3 GNOLLHACK_WIN_GPU switch, the LUID bytes Avalonia passes, and the rule of
       GHApp.InformAboutGPU. */
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
        [InlineData(GHWindowsGpuMode.Auto, GHWindowsGpuPreference.Unset, true)]
        [InlineData(GHWindowsGpuMode.Auto, GHWindowsGpuPreference.Integrated, false)]
        [InlineData(GHWindowsGpuMode.Auto, GHWindowsGpuPreference.Dedicated, true)]
        [InlineData(GHWindowsGpuMode.Dedicated, GHWindowsGpuPreference.Unset, true)]
        [InlineData(GHWindowsGpuMode.Dedicated, GHWindowsGpuPreference.Integrated, true)]
        [InlineData(GHWindowsGpuMode.Dedicated, GHWindowsGpuPreference.Dedicated, true)]
        [InlineData(GHWindowsGpuMode.Integrated, GHWindowsGpuPreference.Unset, false)]
        [InlineData(GHWindowsGpuMode.Integrated, GHWindowsGpuPreference.Integrated, false)]
        [InlineData(GHWindowsGpuMode.Integrated, GHWindowsGpuPreference.Dedicated, false)]
        public void WantedLuid_EachModeAndPreference(GHWindowsGpuMode mode, GHWindowsGpuPreference preference,
            bool wantsHighPerformance)
        {
            long? wanted = GHWindowsGpuChoice.WantedLuid(mode, preference, RtxLuid, IntelLuid);

            Assert.Equal((long?)(wantsHighPerformance ? RtxLuid : IntelLuid), wanted);
        }

        [Theory]
        [InlineData(GHWindowsGpuMode.Auto, GHWindowsGpuPreference.Unset)]
        [InlineData(GHWindowsGpuMode.Auto, GHWindowsGpuPreference.Integrated)]
        [InlineData(GHWindowsGpuMode.Auto, GHWindowsGpuPreference.Dedicated)]
        [InlineData(GHWindowsGpuMode.Dedicated, GHWindowsGpuPreference.Unset)]
        [InlineData(GHWindowsGpuMode.Dedicated, GHWindowsGpuPreference.Integrated)]
        [InlineData(GHWindowsGpuMode.Dedicated, GHWindowsGpuPreference.Dedicated)]
        [InlineData(GHWindowsGpuMode.Integrated, GHWindowsGpuPreference.Unset)]
        [InlineData(GHWindowsGpuMode.Integrated, GHWindowsGpuPreference.Integrated)]
        [InlineData(GHWindowsGpuMode.Integrated, GHWindowsGpuPreference.Dedicated)]
        public void WantedLuid_NullLuids_GiveNull(GHWindowsGpuMode mode, GHWindowsGpuPreference preference)
        {
            Assert.Null(GHWindowsGpuChoice.WantedLuid(mode, preference, null, null));
        }

        [Fact]
        public void WantedLuid_UnknownWantedLuid_DoesNotFallBackToTheOther()
        {
            Assert.Null(GHWindowsGpuChoice.WantedLuid(GHWindowsGpuMode.Auto, GHWindowsGpuPreference.Unset,
                null, IntelLuid));
            Assert.Null(GHWindowsGpuChoice.WantedLuid(GHWindowsGpuMode.Integrated, GHWindowsGpuPreference.Unset,
                RtxLuid, null));
        }

        [Theory]
        [InlineData("Integrated", GHWindowsGpuPreference.Integrated)]
        [InlineData("dedicated ", GHWindowsGpuPreference.Dedicated)]
        [InlineData("Auto", GHWindowsGpuPreference.Unset)]
        [InlineData("Not set", GHWindowsGpuPreference.Unset)]
        [InlineData("", GHWindowsGpuPreference.Unset)]
        [InlineData(null, GHWindowsGpuPreference.Unset)]
        public void ParseActiveGpu_ReadsGetActiveGpuText(string activeGpu, GHWindowsGpuPreference expected)
        {
            Assert.Equal(expected, GHWindowsGpuChoice.ParseActiveGpu(activeGpu));
        }

        [Theory]
        [InlineData(null, GHWindowsGpuMode.Auto, false)]
        [InlineData("", GHWindowsGpuMode.Auto, false)]
        [InlineData("auto", GHWindowsGpuMode.Auto, false)]
        [InlineData("dedicated", GHWindowsGpuMode.Dedicated, false)]
        [InlineData(" Integrated ", GHWindowsGpuMode.Integrated, false)]
        [InlineData("bogus", GHWindowsGpuMode.Auto, true)]
        public void ShouldPreinitialize_TrueWithMode(string switchText, GHWindowsGpuMode expectedMode,
            bool expectedUnknown)
        {
            GHWindowsGpuMode mode;
            bool unknown;

            bool preinitialize = GHWindowsGpuChoice.ShouldPreinitialize(switchText, out mode, out unknown);

            Assert.True(preinitialize);
            Assert.Equal(expectedMode, mode);
            Assert.Equal(expectedUnknown, unknown);
        }

        [Theory]
        [InlineData("default")]
        [InlineData("avalonia")]
        [InlineData("DEFAULT")]
        [InlineData(" default ")]
        public void ShouldPreinitialize_DefaultSwitch_IsFalse(string switchText)
        {
            GHWindowsGpuMode mode;
            bool unknown;

            Assert.False(GHWindowsGpuChoice.ShouldPreinitialize(switchText, out mode, out unknown));
            Assert.False(unknown);
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
