using System;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers GHGamepadControllerSelector: the last active controller wins, removal, and auxiliary controllers
       (the back-grip key bridge), which never switch and add HasGrips */
    public class GHGamepadControllerSelectorTests
    {
        private static GHGamepadControllerInfo Pad(int id, GHGamepadStyle style)
        {
            GHGamepadControllerInfo info = new GHGamepadControllerInfo();
            info.Id = id;
            info.Name = "pad " + id;
            info.Style = style;
            GHGamepadCapabilities caps = new GHGamepadCapabilities();
            caps.HasAnalogSticks = true;
            caps.HasAnalogTriggers = true;
            info.Capabilities = caps;
            return info;
        }

        private static GHGamepadControllerInfo Bridge()
        {
            GHGamepadControllerInfo info = new GHGamepadControllerInfo();
            info.Id = 1000;
            info.IsAuxiliary = true;
            GHGamepadCapabilities caps = new GHGamepadCapabilities();
            caps.HasGrips = true;
            info.Capabilities = caps;
            return info;
        }

        [Fact]
        public void FirstPad_BecomesActive()
        {
            GHGamepadControllerSelector selector = new GHGamepadControllerSelector();
            Assert.Equal(-1, selector.ActiveId);
            Assert.Equal(GHGamepadStyle.Generic, selector.Style);

            selector.Add(Pad(1, GHGamepadStyle.SteamDeck));
            selector.Add(Pad(2, GHGamepadStyle.PlayStation));

            Assert.Equal(1, selector.ActiveId);
            Assert.Equal(GHGamepadStyle.SteamDeck, selector.Style);
        }

        [Fact]
        public void LastActive_Wins()
        {
            GHGamepadControllerSelector selector = new GHGamepadControllerSelector();
            selector.Add(Pad(1, GHGamepadStyle.SteamDeck));
            selector.Add(Pad(2, GHGamepadStyle.PlayStation));

            Assert.True(selector.NoteActivity(2));
            Assert.Equal(2, selector.ActiveId);
            Assert.Equal(GHGamepadStyle.PlayStation, selector.Style);
            Assert.False(selector.NoteActivity(2));
            Assert.True(selector.Delivers(2));
            Assert.False(selector.Delivers(1));
            Assert.True(selector.NoteActivity(1));
            Assert.Equal(1, selector.ActiveId);
        }

        [Fact]
        public void RemovingTheActivePad_LeavesNoneUntilInputOrAdd()
        {
            GHGamepadControllerSelector selector = new GHGamepadControllerSelector();
            selector.Add(Pad(1, GHGamepadStyle.SteamDeck));
            selector.Add(Pad(2, GHGamepadStyle.Xbox));

            Assert.False(selector.Remove(2));
            Assert.True(selector.Remove(1));
            Assert.Equal(-1, selector.ActiveId);

            /* Steam recreates its virtual pad: the new one becomes active when added */
            selector.Add(Pad(3, GHGamepadStyle.SteamDeck));
            Assert.Equal(3, selector.ActiveId);
            Assert.False(selector.Remove(99));
        }

        [Fact]
        public void ActivityOnAPadWhenNoneIsActive_IsNotASwitch()
        {
            GHGamepadControllerSelector selector = new GHGamepadControllerSelector();
            selector.Add(Pad(1, GHGamepadStyle.SteamDeck));
            selector.Add(Pad(2, GHGamepadStyle.Xbox));
            selector.Remove(1);

            Assert.False(selector.NoteActivity(2));
            Assert.Equal(2, selector.ActiveId);
        }

        [Fact]
        public void Auxiliary_NeverSwitchesAndDeliversToTheActivePad()
        {
            GHGamepadControllerSelector selector = new GHGamepadControllerSelector();
            selector.Add(Bridge());
            Assert.Equal(-1, selector.ActiveId);
            Assert.True(selector.Delivers(1000));
            Assert.False(selector.NoteActivity(1000));

            selector.Add(Pad(1, GHGamepadStyle.SteamDeck));
            Assert.Equal(1, selector.ActiveId);
            Assert.False(selector.NoteActivity(1000));
            Assert.Equal(1, selector.ActiveId);
            Assert.True(selector.Delivers(1000));
            Assert.True(selector.IsAuxiliary(1000));
            Assert.Equal(GHGamepadStyle.SteamDeck, selector.Style);
        }

        [Fact]
        public void Capabilities_GripsFromTheAuxiliary()
        {
            GHGamepadControllerSelector selector = new GHGamepadControllerSelector();
            selector.Add(Pad(1, GHGamepadStyle.SteamDeck));
            Assert.False(selector.Capabilities.HasGrips);
            Assert.True(selector.Capabilities.HasAnalogSticks);

            selector.Add(Bridge());
            Assert.True(selector.Capabilities.HasGrips);
            Assert.True(selector.Capabilities.HasAnalogTriggers);

            selector.Remove(1);
            Assert.True(selector.Capabilities.HasGrips);
            Assert.False(selector.Capabilities.HasAnalogSticks);
        }

        [Fact]
        public void UnknownController_IsIgnored()
        {
            GHGamepadControllerSelector selector = new GHGamepadControllerSelector();
            selector.Add(Pad(1, GHGamepadStyle.SteamDeck));
            Assert.False(selector.NoteActivity(7));
            Assert.False(selector.Delivers(7));
            Assert.Equal(1, selector.ActiveId);
        }
    }
}
