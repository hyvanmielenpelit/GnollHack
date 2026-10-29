using System;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers GHWebSpatialNavigation and GHWebSnapshot: script result normalization, snapshot parsing, the Up/Down
       and Left/Right scoring with line tolerance, reading order, wrapped links as focus and target, ties, the top
       visible candidate and the scroll step. Hand-built snapshots use a 1000 x 800 viewport and 20 px lines. */
    public class GHWebSpatialNavigationTests
    {
        private static GHWebRect R(double x, double y, double w, double h)
        {
            return new GHWebRect(x, y, w, h);
        }

        private static GHWebCandidate C(int index, params GHWebRect[] fragments)
        {
            return new GHWebCandidate(index, 'a', fragments);
        }

        private static GHWebSnapshot Snap(int focus, params GHWebCandidate[] candidates)
        {
            GHWebSnapshot s = new GHWebSnapshot();
            s.ViewportWidth = 1000;
            s.ViewportHeight = 800;
            s.FocusPosition = focus;
            s.Candidates.AddRange(candidates);
            return s;
        }

        /* Candidates whose DOM index equals their position */
        private static GHWebSnapshot Snap(int focus, params GHWebRect[] rects)
        {
            GHWebCandidate[] candidates = new GHWebCandidate[rects.Length];
            for (int i = 0; i < rects.Length; i++)
                candidates[i] = C(i, rects[i]);
            return Snap(focus, candidates);
        }

        [Fact]
        public void Normalize_DecodesJsonStringLiterals()
        {
            string aUmlautB = "a" + (char)0x00E4 + "b";
            Assert.Equal(aUmlautB, GHWebSpatialNavigation.NormalizeScriptResult("\"a\\" + "u00e4b\""));
            Assert.Equal(aUmlautB, GHWebSpatialNavigation.NormalizeScriptResult("\"a\\" + "u00E4b\""));
            Assert.Equal("q\"b\\s/\n\r\t\b\f",
                GHWebSpatialNavigation.NormalizeScriptResult("\"q\\\"b\\\\s\\/\\n\\r\\t\\b\\f\""));
            Assert.Equal("S~1", GHWebSpatialNavigation.NormalizeScriptResult("  \"S~1\"\r\n"));
            Assert.Equal("null", GHWebSpatialNavigation.NormalizeScriptResult("\"null\""));
            Assert.Null(GHWebSpatialNavigation.NormalizeScriptResult("\"\""));
        }

        [Fact]
        public void Normalize_InvalidJsonIsReturnedTrimmed()
        {
            Assert.Equal("\"a\\x\"", GHWebSpatialNavigation.NormalizeScriptResult(" \"a\\x\" "));
            Assert.Equal("\"a\\" + "u00\"", GHWebSpatialNavigation.NormalizeScriptResult("\"a\\" + "u00\""));
            Assert.Equal("\"a\\" + "u00g4\"", GHWebSpatialNavigation.NormalizeScriptResult("\"a\\" + "u00g4\""));
            Assert.Equal("\"a\\\"", GHWebSpatialNavigation.NormalizeScriptResult("\"a\\\""));
            Assert.Equal("\"a\"b\"", GHWebSpatialNavigation.NormalizeScriptResult("\"a\"b\""));
        }

        [Fact]
        public void Normalize_PlainStringsAreTrimmed()
        {
            Assert.Equal("S~1~2", GHWebSpatialNavigation.NormalizeScriptResult("S~1~2"));
            Assert.Equal("abc", GHWebSpatialNavigation.NormalizeScriptResult("  abc \r\n"));
            Assert.Equal("\"", GHWebSpatialNavigation.NormalizeScriptResult("\""));
            Assert.Equal("\"abc", GHWebSpatialNavigation.NormalizeScriptResult("\"abc"));
        }

        [Theory]
        [InlineData(null)]
        [InlineData("")]
        [InlineData("   ")]
        [InlineData("null")]
        [InlineData("undefined")]
        [InlineData(" null\n")]
        public void Normalize_NoValueIsNull(string raw)
        {
            Assert.Null(GHWebSpatialNavigation.NormalizeScriptResult(raw));
        }

        [Fact]
        public void Parse_WellFormed()
        {
            GHWebSnapshot s;
            Assert.True(GHWebSnapshot.TryParse(
                "S~7~1280~720~1.25~3~340.5~10~2100~1~hl~0,t,10,20,30,40;3,a,100,200,50,18,-10,220,40,18", out s));
            Assert.Equal(7, s.Seq);
            Assert.Equal(1280.0, s.ViewportWidth);
            Assert.Equal(720.0, s.ViewportHeight);
            Assert.Equal(1.25, s.DevicePixelRatio);
            Assert.Equal(3.0, s.ScrollX);
            Assert.Equal(340.5, s.ScrollY);
            Assert.Equal(10.0, s.MaxScrollX);
            Assert.Equal(2100.0, s.MaxScrollY);
            Assert.Equal(1, s.FocusPosition);
            Assert.True(s.RingHidden);
            Assert.True(s.FocusLost);
            Assert.False(s.Editing);
            Assert.Equal('a', s.FocusKind);

            Assert.Equal(2, s.Candidates.Count);
            Assert.Equal(0, s.Candidates[0].Index);
            Assert.Equal('t', s.Candidates[0].Kind);
            Assert.Single(s.Candidates[0].Fragments);
            GHWebCandidate link = s.Candidates[1];
            Assert.Equal(3, link.Index);
            Assert.Equal(2, link.Fragments.Length);
            Assert.Equal(100.0, link.Fragments[0].Left);
            Assert.Equal(218.0, link.Fragments[0].Bottom);
            Assert.Equal(-10.0, link.Fragments[1].X);
            Assert.Equal(30.0, link.Fragments[1].Right);
            Assert.Equal(220.0, link.Fragments[1].Top);
        }

        [Fact]
        public void Parse_EditingFlagAndNoFlags()
        {
            GHWebSnapshot s;
            Assert.True(GHWebSnapshot.TryParse("S~2~800~600~2~0~0~0~0~0~e~5,m,0,0,300,100", out s));
            Assert.True(s.Editing);
            Assert.False(s.RingHidden);
            Assert.False(s.FocusLost);
            Assert.Equal('m', s.FocusKind);

            Assert.True(GHWebSnapshot.TryParse("S~3~800~600~1~0~0~0~0~-1~~5,b,0,0,30,10", out s));
            Assert.False(s.Editing || s.RingHidden || s.FocusLost);
            Assert.Equal(-1, s.FocusPosition);
            Assert.Equal('\0', s.FocusKind);
        }

        [Fact]
        public void Parse_EmptyItemList()
        {
            GHWebSnapshot s;
            Assert.True(GHWebSnapshot.TryParse("S~1~800~600~1~0~0~0~0~-1~~", out s));
            Assert.Empty(s.Candidates);
            Assert.Equal(-1, s.FocusPosition);
            Assert.Equal('\0', s.FocusKind);
        }

        [Theory]
        [InlineData(null)]
        [InlineData("")]
        [InlineData("X~1~800~600~1~0~0~0~0~-1~~")]
        [InlineData("S~1~800~600~1~0~0~0~0~-1~")]
        [InlineData("S~1~800~600~1~0~0~0~0~-1~~~")]
        [InlineData("S~1~800~600~1~0~0~0~-1~~")]
        [InlineData("S~1~800~600~1,5~0~0~0~0~-1~~")]
        [InlineData("S~x~800~600~1~0~0~0~0~-1~~")]
        [InlineData("S~1~800~600~1~0~0~0~0~0.5~~0,a,0,0,10,10")]
        [InlineData("S~1~NaN~600~1~0~0~0~0~-1~~")]
        [InlineData("S~1~800~600~0~0~0~0~0~-1~~")]
        [InlineData("S~1~-800~600~1~0~0~0~0~-1~~")]
        [InlineData("S~1~800~600~1~0~0~0~0~1~~0,a,0,0,10,10")]
        [InlineData("S~1~800~600~1~0~0~0~0~-2~~")]
        [InlineData("S~1~800~600~1~0~0~0~0~-1~~0,a,0,0,10")]
        [InlineData("S~1~800~600~1~0~0~0~0~-1~~0,a")]
        [InlineData("S~1~800~600~1~0~0~0~0~-1~~0,a,0,0,10,10,1,1,1")]
        [InlineData("S~1~800~600~1~0~0~0~0~-1~~0,a,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1")]
        [InlineData("S~1~800~600~1~0~0~0~0~-1~~0,a,0,0,1x,10")]
        [InlineData("S~1~800~600~1~0~0~0~0~-1~~0,a,0,0,-1,10")]
        [InlineData("S~1~800~600~1~0~0~0~0~-1~~0,q,0,0,10,10")]
        [InlineData("S~1~800~600~1~0~0~0~0~-1~~0,ab,0,0,10,10")]
        [InlineData("S~1~800~600~1~0~0~0~0~-1~~-1,a,0,0,10,10")]
        [InlineData("S~1~800~600~1~0~0~0~0~-1~~0,a,0,0,10,10;")]
        [InlineData("S~1~800~600~1~0~0~0~0~-1~~0,a,0,0,10,10;;1,a,0,20,10,10")]
        public void Parse_RejectsMalformed(string text)
        {
            GHWebSnapshot s;
            Assert.False(GHWebSnapshot.TryParse(text, out s));
            Assert.Null(s);
        }

        [Fact]
        public void Down_PrefersDirectlyBelowOverFarSideNextLine()
        {
            GHWebSnapshot s = Snap(0, R(100, 0, 50, 20), R(500, 20, 50, 20), R(100, 60, 50, 20));
            Assert.Equal(2, GHWebSpatialNavigation.FindTarget(s, GHWebDirection.Down));
        }

        [Fact]
        public void Down_PrefersOverlappingNextLine()
        {
            GHWebSnapshot s = Snap(0, R(100, 0, 50, 20), R(120, 20, 50, 20), R(100, 60, 50, 20));
            Assert.Equal(1, GHWebSpatialNavigation.FindTarget(s, GHWebDirection.Down));
        }

        [Fact]
        public void Up_PrefersDirectlyAboveOverFarSidePreviousLine()
        {
            GHWebSnapshot s = Snap(2, R(100, 40, 50, 20), R(500, 80, 50, 20), R(100, 100, 50, 20));
            Assert.Equal(0, GHWebSpatialNavigation.FindTarget(s, GHWebDirection.Up));
        }

        [Fact]
        public void Up_PrefersOverlappingPreviousLine()
        {
            GHWebSnapshot s = Snap(2, R(100, 40, 50, 20), R(120, 80, 50, 20), R(100, 100, 50, 20));
            Assert.Equal(1, GHWebSpatialNavigation.FindTarget(s, GHWebDirection.Up));
        }

        [Fact]
        public void Down_ToleratesOverlappingLineBoxes()
        {
            GHWebSnapshot s = Snap(0, R(100, 0, 50, 20), R(200, 0, 50, 20), R(100, 16, 50, 20), R(100, 60, 50, 20));
            Assert.Equal(2, GHWebSpatialNavigation.FindTarget(s, GHWebDirection.Down));

            GHWebSnapshot sameLineOnly = Snap(0, R(100, 0, 50, 20), R(200, 0, 50, 20));
            Assert.Equal(-1, GHWebSpatialNavigation.FindTarget(sameLineOnly, GHWebDirection.Down));
        }

        [Fact]
        public void Up_ToleratesOverlappingLineBoxes()
        {
            GHWebSnapshot s = Snap(2, R(100, 40, 50, 20), R(100, 84, 50, 20), R(100, 100, 50, 20));
            Assert.Equal(1, GHWebSpatialNavigation.FindTarget(s, GHWebDirection.Up));
        }

        [Fact]
        public void Right_PicksNearestOnSameLine()
        {
            GHWebSnapshot s = Snap(0, R(100, 0, 50, 20), R(300, 0, 50, 20), R(200, 0, 50, 20));
            Assert.Equal(2, GHWebSpatialNavigation.FindTarget(s, GHWebDirection.Right));
        }

        [Fact]
        public void Right_SmallVerticalOverlapIsNotTheSameLine()
        {
            GHWebSnapshot s = Snap(0, R(100, 0, 50, 20), R(160, 16, 50, 20), R(400, 0, 50, 20));
            Assert.Equal(2, GHWebSpatialNavigation.FindTarget(s, GHWebDirection.Right));
        }

        [Fact]
        public void Right_FallsBackToReadingOrder()
        {
            GHWebSnapshot s = Snap(1,
                C(2, R(100, 40, 50, 20)), C(5, R(400, 0, 50, 20)), C(9, R(100, 80, 50, 20)), C(7, R(0, 40, 50, 20)));
            Assert.Equal(3, GHWebSpatialNavigation.FindTarget(s, GHWebDirection.Right));
        }

        [Fact]
        public void Left_PicksNearestOnSameLine()
        {
            GHWebSnapshot s = Snap(2, R(100, 0, 50, 20), R(300, 0, 50, 20), R(400, 0, 50, 20));
            Assert.Equal(1, GHWebSpatialNavigation.FindTarget(s, GHWebDirection.Left));
        }

        [Fact]
        public void Left_SmallVerticalOverlapIsNotTheSameLine()
        {
            GHWebSnapshot s = Snap(2, R(100, 100, 50, 20), R(340, 84, 50, 20), R(400, 100, 50, 20));
            Assert.Equal(0, GHWebSpatialNavigation.FindTarget(s, GHWebDirection.Left));
        }

        [Fact]
        public void Left_FallsBackToReadingOrder()
        {
            GHWebSnapshot s = Snap(2,
                C(2, R(100, 40, 50, 20)), C(4, R(700, 40, 50, 20)), C(5, R(0, 0, 50, 20)), C(9, R(100, 80, 50, 20)));
            Assert.Equal(1, GHWebSpatialNavigation.FindTarget(s, GHWebDirection.Left));
        }

        [Fact]
        public void WrappedFocus_DownMeasuresFromItsLowerFragment()
        {
            GHWebSnapshot s = Snap(0,
                C(0, R(500, 0, 100, 20), R(0, 20, 60, 20)), C(1, R(500, 60, 100, 20)), C(2, R(0, 60, 60, 20)));
            Assert.Equal(2, GHWebSpatialNavigation.FindTarget(s, GHWebDirection.Down));
        }

        [Fact]
        public void WrappedFocus_RightMeasuresFromItsLastFragment()
        {
            GHWebSnapshot s = Snap(0,
                C(0, R(500, 0, 100, 20), R(0, 20, 60, 20)), C(1, R(700, 0, 50, 20)), C(2, R(100, 20, 50, 20)));
            Assert.Equal(2, GHWebSpatialNavigation.FindTarget(s, GHWebDirection.Right));
        }

        [Fact]
        public void WrappedTarget_IsScoredByItsBestFragment()
        {
            GHWebSnapshot s = Snap(0,
                C(0, R(100, 0, 50, 20)), C(1, R(100, 100, 50, 20)), C(2, R(600, 40, 100, 20), R(0, 60, 120, 20)));
            Assert.Equal(2, GHWebSpatialNavigation.FindTarget(s, GHWebDirection.Down));
        }

        [Fact]
        public void NoEligibleCandidate_GivesMinusOne()
        {
            GHWebSnapshot alone = Snap(0, R(100, 100, 50, 20));
            GHWebSnapshot emptyOthers = Snap(1,
                C(0, R(100, 40, 0, 20)), C(1, R(100, 100, 50, 20)), C(2, R(300, 100, 50, 0)), C(3, R(100, 200, 0, 0)));
            foreach (GHWebDirection dir in new GHWebDirection[] { GHWebDirection.Up, GHWebDirection.Down,
                GHWebDirection.Left, GHWebDirection.Right })
            {
                Assert.Equal(-1, GHWebSpatialNavigation.FindTarget(alone, dir));
                Assert.Equal(-1, GHWebSpatialNavigation.FindTarget(emptyOthers, dir));
            }
        }

        [Fact]
        public void UnsetOrLostFocus_GivesMinusOne()
        {
            GHWebSnapshot unset = Snap(-1, R(100, 0, 50, 20), R(100, 60, 50, 20));
            Assert.False(GHWebSpatialNavigation.HasUsableFocus(unset));
            Assert.Equal(-1, GHWebSpatialNavigation.FindTarget(unset, GHWebDirection.Down));

            GHWebSnapshot lost = Snap(0, R(100, 0, 50, 20), R(100, 60, 50, 20));
            Assert.True(GHWebSpatialNavigation.HasUsableFocus(lost));
            Assert.Equal(1, GHWebSpatialNavigation.FindTarget(lost, GHWebDirection.Down));
            lost.FocusLost = true;
            Assert.False(GHWebSpatialNavigation.HasUsableFocus(lost));
            Assert.Equal(-1, GHWebSpatialNavigation.FindTarget(lost, GHWebDirection.Down));

            GHWebSnapshot outOfRange = Snap(5, R(100, 0, 50, 20), R(100, 60, 50, 20));
            Assert.Equal(-1, GHWebSpatialNavigation.FindTarget(outOfRange, GHWebDirection.Down));

            GHWebSnapshot emptyFocus = Snap(0, R(100, 0, 0, 0), R(100, 60, 50, 20));
            Assert.False(GHWebSpatialNavigation.HasUsableFocus(emptyFocus));
            Assert.Equal(-1, GHWebSpatialNavigation.FindTarget(emptyFocus, GHWebDirection.Down));

            Assert.Equal(-1, GHWebSpatialNavigation.FindTarget(null, GHWebDirection.Down));
        }

        [Fact]
        public void Ties_GoToTheLowerDomIndex()
        {
            GHWebSnapshot s = Snap(0, R(100, 0, 50, 20), R(200, 60, 50, 20), R(0, 60, 50, 20));
            Assert.Equal(1, GHWebSpatialNavigation.FindTarget(s, GHWebDirection.Down));

            GHWebSnapshot byIndex = Snap(0, C(0, R(100, 0, 50, 20)), C(5, R(0, 60, 50, 20)), C(3, R(200, 60, 50, 20)));
            Assert.Equal(2, GHWebSpatialNavigation.FindTarget(byIndex, GHWebDirection.Down));

            GHWebSnapshot up = Snap(2, R(0, 0, 50, 20), R(200, 0, 50, 20), R(100, 60, 50, 20));
            Assert.Equal(0, GHWebSpatialNavigation.FindTarget(up, GHWebDirection.Up));
        }

        [Fact]
        public void TopVisible_SlightlyAboveTheTopCountsAsInside()
        {
            GHWebSnapshot s = Snap(-1, R(0, -30, 50, 40), R(0, 100, 50, 20), R(0, -0.5, 50, 20));
            Assert.Equal(2, GHWebSpatialNavigation.FindTopVisible(s));

            GHWebSnapshot beyond = Snap(-1, R(0, -1.5, 50, 20), R(0, 100, 50, 20));
            Assert.Equal(1, GHWebSpatialNavigation.FindTopVisible(beyond));
        }

        [Fact]
        public void TopVisible_StraddlingOnlyWhenNothingStartsInside()
        {
            GHWebSnapshot s = Snap(-1, R(0, -100, 50, 20), R(0, -30, 50, 40), R(0, -20, 50, 40), R(0, 900, 50, 20));
            Assert.Equal(1, GHWebSpatialNavigation.FindTopVisible(s));
        }

        [Fact]
        public void TopVisible_NoneGivesMinusOne()
        {
            GHWebSnapshot s = Snap(-1, R(0, -100, 50, 20), R(0, 800, 50, 20), R(0, 100, 0, 20));
            Assert.Equal(-1, GHWebSpatialNavigation.FindTopVisible(s));
            Assert.Equal(-1, GHWebSpatialNavigation.FindTopVisible(Snap(-1, new GHWebRect[0])));
            Assert.Equal(-1, GHWebSpatialNavigation.FindTopVisible(null));
        }

        [Fact]
        public void TopVisible_TiesGoToTheLowerDomIndex()
        {
            GHWebSnapshot s = Snap(-1, R(300, 50, 50, 20), R(0, 50, 50, 20), R(0, 80, 50, 20));
            Assert.Equal(0, GHWebSpatialNavigation.FindTopVisible(s));
        }

        [Fact]
        public void ScrollStep_HasAMinimum()
        {
            Assert.Equal(48.0, GHWebSpatialNavigation.ScrollStep(100), 6);
            Assert.Equal(160.0, GHWebSpatialNavigation.ScrollStep(800), 6);
        }
    }
}
