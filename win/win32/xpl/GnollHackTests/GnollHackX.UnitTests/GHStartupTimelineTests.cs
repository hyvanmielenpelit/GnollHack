using System;
using System.Collections.Generic;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers GHStartupTimeline with an injected clock: first marks, the last milestone and mask,
       ages against the anchor, the timing text, names and the reached event. */
    public class GHStartupTimelineTests
    {
        private long _clock = 10000;

        private GHStartupTimeline NewTimeline()
        {
            GHStartupTimeline timeline = new GHStartupTimeline(() => _clock);
            timeline.SetProcessAgeNow(500);
            return timeline;
        }

        [Fact]
        public void Mark_FirstMarkWins()
        {
            GHStartupTimeline timeline = NewTimeline();
            _clock += 100;
            timeline.Mark(GHStartupMilestone.AppInit);
            _clock += 900;
            timeline.Mark(GHStartupMilestone.AppInit);

            Assert.Equal(600, timeline.MilestoneMilliseconds(GHStartupMilestone.AppInit));
            Assert.True(timeline.HasReached(GHStartupMilestone.AppInit));
            Assert.False(timeline.HasReached(GHStartupMilestone.AppReady));
            Assert.Equal(-1, timeline.MilestoneMilliseconds(GHStartupMilestone.AppReady));
        }

        [Fact]
        public void Mark_TracksLastMilestoneAndMask()
        {
            GHStartupTimeline timeline = NewTimeline();
            timeline.Mark(GHStartupMilestone.AppCreate);
            _clock += 250;
            timeline.Mark(GHStartupMilestone.MainPage);
            _clock += 50;
            timeline.Mark(GHStartupMilestone.Activity);

            long age, lastMilliseconds;
            GHStartupMilestone last;
            int mask;
            timeline.GetState(out age, out last, out lastMilliseconds, out mask);

            Assert.Equal(GHStartupMilestone.Activity, last);
            Assert.Equal(800, lastMilliseconds);
            Assert.Equal(800, age);
            Assert.Equal((1 << 1) | (1 << 6) | (1 << 8), mask);
            Assert.Equal(mask, timeline.ReachedMask);
            Assert.Equal(GHStartupMilestone.Activity, timeline.LastMilestone);
        }

        [Fact]
        public void ProcessAge_FollowsTheAnchorAndOnlyTheFirstAnchorCounts()
        {
            GHStartupTimeline timeline = NewTimeline();
            timeline.SetProcessAgeNow(99999);
            _clock += 1234;

            Assert.Equal(1734, timeline.ProcessAgeMilliseconds);
        }

        [Fact]
        public void ProcessAge_BeforeAnchor_IsMinusOne()
        {
            GHStartupTimeline timeline = new GHStartupTimeline(() => _clock);

            Assert.Equal(-1, timeline.ProcessAgeMilliseconds);
            long age, lastMilliseconds;
            GHStartupMilestone last;
            int mask;
            timeline.GetState(out age, out last, out lastMilliseconds, out mask);
            Assert.Equal(-1, age);
            Assert.Equal(GHStartupMilestone.None, last);
            Assert.Equal(-1, lastMilliseconds);
            Assert.Equal(0, mask);
        }

        [Fact]
        public void Mark_WithoutAnchor_AnchorsItself()
        {
            GHStartupTimeline timeline = new GHStartupTimeline(() => _clock);
            timeline.Mark(GHStartupMilestone.AppInit);

            Assert.True(timeline.ProcessAgeMilliseconds >= 0);
            Assert.True(timeline.HasReached(GHStartupMilestone.AppInit));
        }

        [Theory]
        [InlineData(GHStartupMilestone.None)]
        [InlineData((GHStartupMilestone)16)]
        [InlineData((GHStartupMilestone)(-3))]
        public void Mark_OutOfRange_IsIgnored(GHStartupMilestone milestone)
        {
            GHStartupTimeline timeline = NewTimeline();
            timeline.Mark(milestone);

            Assert.Equal(0, timeline.ReachedMask);
            Assert.Equal(GHStartupMilestone.None, timeline.LastMilestone);
        }

        [Fact]
        public void FormatTimings_OrdersByTimeNotByCode()
        {
            GHStartupTimeline timeline = NewTimeline();
            timeline.Mark(GHStartupMilestone.AppCreate);
            _clock += 1500;
            timeline.Mark(GHStartupMilestone.MainPage);
            timeline.Mark(GHStartupMilestone.Activity);
            _clock += 1234;
            timeline.Mark(GHStartupMilestone.Focused);

            Assert.Equal("app-create 0.50 s, activity 2.00 s, main-page 2.00 s, focused 3.23 s", timeline.FormatTimings());
        }

        [Fact]
        public void FormatTimings_NothingReached_IsEmpty()
        {
            Assert.Equal("", NewTimeline().FormatTimings());
        }

        [Fact]
        public void Name_CoversEveryCode()
        {
            string[] expected =
            {
                "none", "app-create", "app-init", "secure-store", "secure-store-done", "app-ready",
                "activity", "focused", "main-page", "startup-tasks", "main-menu",
                "runtime", "native-loaded", "maui-app", "base-assets", "first-draw"
            };
            int code;
            for (code = 0; code <= GHStartupTimeline.MaxMilestone; code++)
                Assert.Equal(expected[code], GHStartupTimeline.Name((GHStartupMilestone)code));
            Assert.Equal("unknown", GHStartupTimeline.Name((GHStartupMilestone)16));
            Assert.Equal("unknown", GHStartupTimeline.Name((GHStartupMilestone)(-1)));
        }

        /* Sentry's default server-side scrubbing discards any value containing one of these words */
        [Fact]
        public void Names_AvoidSentryScrubbedWords()
        {
            string[] scrubbedWords =
            {
                "password", "secret", "passwd", "api_key", "apikey", "auth", "credentials", "mysql_pwd",
                "privatekey", "private_key", "token", "bearer"
            };
            GHStartupTimeline timeline = NewTimeline();
            int code;
            for (code = 1; code <= GHStartupTimeline.MaxMilestone; code++)
                timeline.Mark((GHStartupMilestone)code);

            string timings = timeline.FormatTimings().ToLowerInvariant();
            Assert.Contains("first-draw", timings);
            foreach (string word in scrubbedWords)
                Assert.DoesNotContain(word, timings);
        }

        [Fact]
        public void MilestoneReached_RaisedOncePerMilestone_AndAThrowingHandlerIsContained()
        {
            GHStartupTimeline timeline = NewTimeline();
            List<GHStartupMilestone> seen = new List<GHStartupMilestone>();
            timeline.MilestoneReached += m => { throw new InvalidOperationException("test"); };
            timeline.MilestoneReached += m => seen.Add(m);

            timeline.Mark(GHStartupMilestone.AppInit);
            timeline.Mark(GHStartupMilestone.AppInit);
            timeline.Mark(GHStartupMilestone.AppReady);

            Assert.Equal(new[] { GHStartupMilestone.AppInit, GHStartupMilestone.AppReady }, seen);
            Assert.True(timeline.HasReached(GHStartupMilestone.AppReady));
        }
    }
}
