using System;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers GHWebGamepadProfile: the default profile holds the wiki page's labels and calls, the Overseer profile
       the Overseer page's, and each factory call returns a new instance. */
    public class GHWebGamepadProfileTests
    {
        [Fact]
        public void CreateDefault_HasWikiPageValues()
        {
            GHWebGamepadProfile p = GHWebGamepadProfile.CreateDefault();
            Assert.Equal("Close", p.CloseLabel);
            Assert.Null(p.BackFallback);
            Assert.Equal("Close", p.BackFallbackLabel);
            Assert.Null(p.ComposeKey);
            Assert.Null(p.ComposeLabel);
            Assert.Null(p.ComposeFallback);
            Assert.Equal("top()", p.L3Call);
            Assert.Null(p.L3Label);
            Assert.Equal("Section", p.SectionLabel);
            Assert.Equal("Search", p.SearchLabel);
            Assert.Equal("Area", p.ZoneLabel);
            Assert.Equal("Actions", p.RowActionsLabel);
            Assert.Equal("Open", p.ZoneShortcutYLabel);
        }

        [Fact]
        public void CreateOverseer_HasOverseerValues()
        {
            GHWebGamepadProfile p = GHWebGamepadProfile.CreateOverseer();
            Assert.Equal("Leave", p.CloseLabel);
            Assert.Null(p.BackFallback);
            Assert.Equal("Leave", p.BackFallbackLabel);
            Assert.Equal("compose", p.ComposeKey);
            Assert.Equal("Write", p.ComposeLabel);
            Assert.Equal("textarea", p.ComposeFallback);
            Assert.Equal("bottom('m')", p.L3Call);
            Assert.Equal("Newest", p.L3Label);
            Assert.Equal("Message", p.SectionLabel);
            Assert.Equal("Search", p.SearchLabel);
            Assert.Equal("Area", p.ZoneLabel);
            Assert.Equal("Actions", p.RowActionsLabel);
            Assert.Equal("Model", p.ZoneShortcutYLabel);
        }

        [Fact]
        public void Factories_ReturnNewInstances()
        {
            GHWebGamepadProfile d1 = GHWebGamepadProfile.CreateDefault();
            GHWebGamepadProfile d2 = GHWebGamepadProfile.CreateDefault();
            Assert.NotSame(d1, d2);
            GHWebGamepadProfile o1 = GHWebGamepadProfile.CreateOverseer();
            GHWebGamepadProfile o2 = GHWebGamepadProfile.CreateOverseer();
            Assert.NotSame(o1, o2);

            d1.CloseLabel = "Changed";
            Assert.Equal("Close", d2.CloseLabel);
            Assert.Equal("Close", GHWebGamepadProfile.CreateDefault().CloseLabel);
        }
    }
}
