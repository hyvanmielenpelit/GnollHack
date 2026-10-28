using System;
using System.Collections.Generic;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers GHRadialMenuState: the initial page, paging clamps, SwitchSet between Context and More Commands, R53's
       group paging, the highlight re-evaluated or restored on page change, ClosesOnActivate,
       logical close before the visual close, callback order, refused confirms, NeedsFrame at
       rest, toggle state changes, pointer input, the highlight sound throttle, R74's Center on
       Hero in the L3 page, R1's push-up-is-north, and durations that do not depend on the frame
       rate (60 Hz against 90 Hz ticks). */
    public class GHRadialMenuStateTests
    {
        /* 180,000 ticks per second: a 60 Hz frame is 3000 ticks, a 90 Hz frame 2000 */
        private const long Tps = 180000;
        private const long Frame60 = Tps / 60;
        private const long Frame90 = Tps / 90;

        private static long Ms(double ms)
        {
            return (long)Math.Round(ms * Tps / 1000.0);
        }

        private static GHRadialPage FullPage(string title, int n, int group)
        {
            GHRadialPage page = new GHRadialPage(title, n);
            page.GroupIndex = group;
            int k;
            for (k = 0; k < n; k++)
                page.Slots[k] = new GHRadialItem { Id = title + ":" + k, Label = title + k };
            return page;
        }

        private static GHRadialMenuDefinition Def(params GHRadialPage[] pages)
        {
            GHRadialMenuDefinition def = new GHRadialMenuDefinition();
            def.MenuId = "R3";
            def.Pages.AddRange(pages);
            return def;
        }

        private static GHRadialMenuState Opened(GHRadialMenuDefinition def, long now)
        {
            GHRadialMenuState s = new GHRadialMenuState(Tps);
            Assert.True(s.Open(def, now));
            /* Arm the stick by centering it */
            s.UpdateStick(0, 0, 1.0 / 60);
            return s;
        }

        private static void Push(GHRadialMenuState s, double angle, double magnitude)
        {
            double a = angle * Math.PI / 180.0;
            s.UpdateStick(Math.Sin(a) * magnitude, -Math.Cos(a) * magnitude, 1.0);
        }

        private static void TickUntil(GHRadialMenuState s, long from, long to, long step)
        {
            long t;
            for (t = from; t <= to; t += step)
                s.Tick(t);
        }

        [Fact]
        public void Open_StartsOnFirstPageWithoutHighlight()
        {
            GHRadialMenuState s = Opened(Def(FullPage("Context", 8, 0), FullPage("More", 12, 1)), 0);
            Assert.True(s.IsOpen);
            Assert.True(s.IsVisible);
            Assert.True(s.IsMenuOpen("R3"));
            Assert.False(s.IsMenuOpen("L3"));
            Assert.Equal(GHRadialPhase.Opening, s.Phase);
            Assert.Equal(0, s.PageIndex);
            Assert.Equal(-1, s.HighlightedSlot);
            Assert.False(new GHRadialMenuState(Tps).Open(new GHRadialMenuDefinition(), 0));
        }

        [Fact]
        public void Open_StartsOnTheInitialPageWithoutHighlight()
        {
            GHRadialMenuDefinition def = Def(FullPage("Context", 8, 0), FullPage("Commands 1/3", 12, 1),
                FullPage("Commands 2/3", 12, 1), FullPage("Commands 3/3", 12, 1));
            def.InitialPageIndex = 2;
            GHRadialMenuState s = Opened(def, 0);
            Assert.Equal(2, s.PageIndex);
            Assert.Same(def.Pages[2], s.CurrentPage);
            Assert.Equal(-1, s.HighlightedSlot);
            Assert.Equal(-1, s.PreviousPageIndex);
            Assert.Equal(1f, s.PageTransition);

            /* Paging continues from the initial page */
            s.Apply(GHRadialInput.PrevPage, Ms(1));
            Assert.Equal(1, s.PageIndex);
        }

        [Fact]
        public void Open_ClampsTheInitialPage()
        {
            GHRadialMenuDefinition def = Def(FullPage("Context", 8, 0), FullPage("Commands", 12, 1));
            def.InitialPageIndex = 5;
            Assert.Equal(1, Opened(def, 0).PageIndex);
            def.InitialPageIndex = -3;
            Assert.Equal(0, Opened(def, 0).PageIndex);
        }

        [Fact]
        public void SwitchSet_FromTheInitialPageWorksAsBefore()
        {
            GHRadialMenuDefinition def = Def(FullPage("Context", 8, 0), FullPage("Commands 1/3", 12, 1),
                FullPage("Commands 2/3", 12, 1), FullPage("Commands 3/3", 12, 1));
            def.SwitchSetPageIndex = 1;
            def.InitialPageIndex = 1;
            GHRadialMenuState s = Opened(def, 0);
            Assert.Equal(1, s.PageIndex);
            s.Apply(GHRadialInput.SwitchSet, Ms(1));
            Assert.Equal(0, s.PageIndex);
            s.Apply(GHRadialInput.SwitchSet, Ms(2));
            Assert.Equal(1, s.PageIndex);

            /* From a later commands page it also goes back to the context page */
            def.InitialPageIndex = 3;
            s = Opened(def, Ms(10));
            Assert.Equal(3, s.PageIndex);
            s.Apply(GHRadialInput.SwitchSet, Ms(11));
            Assert.Equal(0, s.PageIndex);

            /* From the context page it goes to SwitchSetPageIndex */
            def.InitialPageIndex = 0;
            s = Opened(def, Ms(20));
            s.Apply(GHRadialInput.SwitchSet, Ms(21));
            Assert.Equal(1, s.PageIndex);
        }

        [Fact]
        public void StickPushedUp_HighlightsNorth()
        {
            GHRadialMenuState s = Opened(Def(FullPage("Context", 8, 0)), 0);
            GHRadialApplyResult r = s.UpdateStick(0, -0.9, 1.0 / 60);
            Assert.True(r.Has(GHRadialEffects.HighlightChanged));
            Assert.Equal(0, s.HighlightedSlot);
        }

        [Fact]
        public void StickDrift_FromTheOpeningClickSelectsNothing()
        {
            GHRadialMenuState s = new GHRadialMenuState(Tps);
            s.Open(Def(FullPage("Context", 8, 0)), 0);
            s.UpdateStick(0.9, 0, 1.0 / 60);
            s.UpdateStick(0.9, 0, 1.0 / 60);
            Assert.Equal(-1, s.HighlightedSlot);
        }

        [Fact]
        public void Paging_ClampsWithoutWrapping()
        {
            GHRadialMenuState s = Opened(Def(FullPage("A", 8, 0), FullPage("B", 12, 1), FullPage("C", 12, 2)), 0);
            Assert.Equal(GHRadialEffects.None, s.Apply(GHRadialInput.PrevPage, Ms(1)).Effects);
            Assert.Equal(0, s.PageIndex);
            Assert.True(s.Apply(GHRadialInput.NextPage, Ms(2)).Has(GHRadialEffects.PageChanged));
            s.Apply(GHRadialInput.NextPage, Ms(3));
            Assert.Equal(2, s.PageIndex);
            Assert.Equal(GHRadialEffects.None, s.Apply(GHRadialInput.NextPage, Ms(4)).Effects);
            Assert.Equal(2, s.PageIndex);
            Assert.True(s.PointerWheel(-1, Ms(5)).Has(GHRadialEffects.PageChanged));
            Assert.Equal(1, s.PageIndex);
        }

        [Fact]
        public void SwitchSet_JumpsBetweenContextAndMore()
        {
            GHRadialMenuDefinition def = Def(FullPage("Context", 8, 0), FullPage("Context 2", 8, 0),
                FullPage("Common A", 12, 1), FullPage("Common B", 12, 1));
            def.SwitchSetPageIndex = 2;
            GHRadialMenuState s = Opened(def, 0);
            s.Apply(GHRadialInput.SwitchSet, Ms(1));
            Assert.Equal(2, s.PageIndex);
            s.Apply(GHRadialInput.NextPage, Ms(2));
            s.Apply(GHRadialInput.SwitchSet, Ms(3));
            Assert.Equal(0, s.PageIndex);
            s.Apply(GHRadialInput.NextPage, Ms(4));
            s.Apply(GHRadialInput.SwitchSet, Ms(5));
            Assert.Equal(2, s.PageIndex);

            GHRadialMenuState none = Opened(Def(FullPage("Toggles", 6, 0)), 0);
            Assert.Equal(GHRadialEffects.None, none.Apply(GHRadialInput.SwitchSet, Ms(1)).Effects);
        }

        [Fact]
        public void Groups_PageToTheFirstPageOfTheNeighboringGroup()
        {
            GHRadialMenuState s = Opened(Def(FullPage("Ctx", 8, 0), FullPage("ComA", 12, 1), FullPage("ComB", 12, 1),
                FullPage("AddA", 12, 2), FullPage("AddB", 12, 2), FullPage("WizA", 12, 3), FullPage("WizB", 12, 3)), 0);
            Assert.Equal(GHRadialEffects.None, s.Apply(GHRadialInput.PrevGroup, Ms(1)).Effects);
            Assert.True(s.Apply(GHRadialInput.NextGroup, Ms(2)).Has(GHRadialEffects.PageChanged));
            Assert.Equal(1, s.PageIndex);
            s.Apply(GHRadialInput.NextPage, Ms(3));
            Assert.Equal(2, s.PageIndex);
            s.Apply(GHRadialInput.NextGroup, Ms(4));
            Assert.Equal(3, s.PageIndex);
            s.Apply(GHRadialInput.NextGroup, Ms(5));
            Assert.Equal(5, s.PageIndex);
            Assert.Equal(GHRadialEffects.None, s.Apply(GHRadialInput.NextGroup, Ms(6)).Effects);
            s.Apply(GHRadialInput.NextPage, Ms(7));
            Assert.Equal(6, s.PageIndex);
            s.Apply(GHRadialInput.PrevGroup, Ms(8));
            Assert.Equal(3, s.PageIndex);
            s.Apply(GHRadialInput.NextPage, Ms(9));
            s.Apply(GHRadialInput.PrevGroup, Ms(10));
            Assert.Equal(1, s.PageIndex);
            s.Apply(GHRadialInput.PrevGroup, Ms(11));
            Assert.Equal(0, s.PageIndex);
        }

        [Fact]
        public void PageChange_ReevaluatesWhileStickIsDeflected()
        {
            GHRadialMenuState s = Opened(Def(FullPage("Ctx", 8, 0), FullPage("More", 12, 1)), 0);
            Push(s, 90, 0.9);
            Assert.Equal(2, s.HighlightedSlot);
            s.Apply(GHRadialInput.NextPage, Ms(1));
            Assert.Equal(3, s.HighlightedSlot);
        }

        [Fact]
        public void PageChange_RemembersEachPageHighlightWhenCentered()
        {
            GHRadialMenuState s = Opened(Def(FullPage("Ctx", 8, 0), FullPage("More", 12, 1)), 0);
            Push(s, 90, 0.9);
            s.UpdateStick(0, 0, 1.0);
            Assert.Equal(2, s.HighlightedSlot);
            s.Apply(GHRadialInput.NextPage, Ms(1));
            Assert.Equal(-1, s.HighlightedSlot);
            Assert.Equal(2, s.GetPageHighlight(0));
            s.Apply(GHRadialInput.StepClockwise, Ms(2));
            Assert.Equal(0, s.HighlightedSlot);
            s.Apply(GHRadialInput.PrevPage, Ms(3));
            Assert.Equal(2, s.HighlightedSlot);
            s.Apply(GHRadialInput.NextPage, Ms(4));
            Assert.Equal(0, s.HighlightedSlot);
        }

        [Fact]
        public void Reopening_StartsOnFirstPageWithoutHighlight()
        {
            GHRadialMenuDefinition def = Def(FullPage("Ctx", 8, 0), FullPage("More", 12, 1));
            GHRadialMenuState s = Opened(def, 0);
            s.Apply(GHRadialInput.NextPage, Ms(1));
            s.Apply(GHRadialInput.StepClockwise, Ms(2));
            s.Apply(GHRadialInput.Cancel, Ms(3));
            TickUntil(s, Ms(4), Ms(400), Frame60);
            Assert.False(s.IsVisible);
            s.Open(def, Ms(500));
            Assert.Equal(0, s.PageIndex);
            Assert.Equal(-1, s.HighlightedSlot);
        }

        [Fact]
        public void ConfirmOnToggle_StaysOpen()
        {
            int activations = 0;
            int closes = 0;
            bool on = false;
            GHRadialPage page = new GHRadialPage("Toggles", 6);
            page.Slots[0] = new GHRadialItem { Id = "l3:statusbar", Kind = GHRadialItemKind.Toggle, ClosesOnActivate = false, IsOnGetter = () => on };
            GHRadialMenuDefinition def = Def(page);
            def.OnActivate = item => { activations++; on = !on; };
            def.OnClosed = reason => closes++;
            GHRadialMenuState s = Opened(def, 0);
            s.Apply(GHRadialInput.JumpNorth, Ms(1));
            GHRadialApplyResult r = s.Apply(GHRadialInput.Confirm, Ms(2));
            Assert.True(r.Has(GHRadialEffects.Activated));
            Assert.False(r.Has(GHRadialEffects.Closed));
            Assert.Same(page.Slots[0], r.Item);
            Assert.True(s.IsOpen);
            Assert.Equal(1, activations);
            Assert.Equal(0, closes);
            Assert.True(on);
            Assert.Equal(0, s.FlashSlot);
        }

        [Fact]
        public void ConfirmOnCommand_ClosesLogicallyBeforeVisually()
        {
            List<string> calls = new List<string>();
            GHRadialMenuState s = null;
            GHRadialMenuDefinition def = Def(FullPage("Ctx", 8, 0));
            def.OnActivate = item => calls.Add("activate:" + item.Id + ":" + s.IsOpen);
            def.OnClosed = reason => calls.Add("closed:" + reason);
            s = Opened(def, 0);
            TickUntil(s, 0, Ms(200), Frame60);
            s.Apply(GHRadialInput.StepClockwise, Ms(201));
            GHRadialApplyResult r = s.Apply(GHRadialInput.Confirm, Ms(202));
            Assert.True(r.Has(GHRadialEffects.Activated));
            Assert.True(r.Has(GHRadialEffects.Closed));
            Assert.Equal(GHRadialCloseReason.Confirmed, r.CloseReason);
            Assert.False(s.IsOpen);
            Assert.True(s.IsVisible);
            Assert.Equal(GHRadialPhase.Closing, s.Phase);
            Assert.Equal(new List<string> { "activate:Ctx:0:False", "closed:Confirmed" }, calls);

            /* Input after the logical close does nothing */
            Assert.Equal(GHRadialEffects.None, s.Apply(GHRadialInput.Confirm, Ms(203)).Effects);
            s.Tick(Ms(250));
            Assert.True(s.IsVisible);
            s.Tick(Ms(300));
            Assert.False(s.IsVisible);
            Assert.False(s.NeedsFrame);
        }

        [Fact]
        public void ConfirmOnNothingOrDisabled_IsRefused()
        {
            int activations = 0;
            GHRadialPage page = new GHRadialPage("Ctx", 8);
            page.Slots[2] = new GHRadialItem { Id = "off", IsEnabled = false };
            GHRadialMenuDefinition def = Def(page);
            def.OnActivate = item => activations++;
            GHRadialMenuState s = Opened(def, 0);
            Assert.Equal(GHRadialEffects.Refused, s.Apply(GHRadialInput.Confirm, Ms(1)).Effects);
            s.Apply(GHRadialInput.StepClockwise, Ms(2));
            Assert.Equal(2, s.HighlightedSlot);
            GHRadialApplyResult r = s.Apply(GHRadialInput.Confirm, Ms(3));
            Assert.Equal(GHRadialEffects.Refused, r.Effects);
            Assert.Same(page.Slots[2], r.Item);
            Assert.True(s.IsOpen);
            Assert.Equal(0, activations);
        }

        [Fact]
        public void CancelAndOpener_CloseWithCancelled()
        {
            List<GHRadialCloseReason> reasons = new List<GHRadialCloseReason>();
            GHRadialMenuDefinition def = Def(FullPage("Ctx", 8, 0));
            def.OnClosed = reason => reasons.Add(reason);
            GHRadialMenuState s = Opened(def, 0);
            GHRadialApplyResult r = s.Apply(GHRadialInput.Cancel, Ms(1));
            Assert.True(r.Has(GHRadialEffects.Closed));
            Assert.False(s.IsOpen);
            s.Open(def, Ms(2));
            s.Apply(GHRadialInput.OpenerPressed, Ms(3));
            Assert.False(s.IsOpen);
            Assert.False(s.Close(GHRadialCloseReason.Superseded, Ms(4)));
            Assert.Equal(new List<GHRadialCloseReason> { GHRadialCloseReason.Cancelled, GHRadialCloseReason.Cancelled }, reasons);
        }

        [Fact]
        public void HostClose_ReportsTheReasonAndCanSkipTheAnimation()
        {
            List<GHRadialCloseReason> reasons = new List<GHRadialCloseReason>();
            GHRadialMenuDefinition def = Def(FullPage("Ctx", 8, 0));
            def.OnClosed = reason => reasons.Add(reason);
            GHRadialMenuState s = Opened(def, 0);
            Assert.True(s.Close(GHRadialCloseReason.GameEnded, Ms(1), true));
            Assert.False(s.IsVisible);
            Assert.Equal(new List<GHRadialCloseReason> { GHRadialCloseReason.GameEnded }, reasons);
        }

        [Fact]
        public void NeedsFrame_IsFalseAtRest()
        {
            GHRadialMenuState s = Opened(Def(FullPage("Ctx", 8, 0)), 0);
            s.Tick(Frame60);
            Assert.True(s.NeedsFrame);
            TickUntil(s, 2 * Frame60, Ms(400), Frame60);
            Assert.Equal(GHRadialPhase.Open, s.Phase);
            Assert.False(s.NeedsFrame);
            Assert.False(s.IsAnimating);

            /* A highlight change animates for the ease duration, then rests again */
            Push(s, 90, 0.9);
            s.Tick(Ms(420));
            Assert.True(s.NeedsFrame);
            TickUntil(s, Ms(440), Ms(700), Frame60);
            Assert.False(s.NeedsFrame);
            Assert.Equal(1f, s.GetHighlightWeight(2));
            Assert.Equal(0f, s.GetHighlightWeight(1));
            Assert.Equal(1f, s.WedgeOpacity);
            Assert.Equal(90f, s.WedgeAngle, 3);

            /* A still stick does not repaint */
            Push(s, 90, 0.9);
            s.Tick(Ms(720));
            Assert.False(s.NeedsFrame);
            s.MarkDirty();
            s.Tick(Ms(740));
            Assert.True(s.NeedsFrame);
            s.Tick(Ms(760));
            Assert.False(s.NeedsFrame);
        }

        [Fact]
        public void ToggleStateChange_RequestsAFrame()
        {
            bool on = false;
            GHRadialPage page = new GHRadialPage("Toggles", 6);
            page.Slots[1] = new GHRadialItem { Id = "l3:autocenter", Kind = GHRadialItemKind.Toggle, IsOnGetter = () => on };
            GHRadialMenuState s = Opened(Def(page), 0);
            TickUntil(s, 0, Ms(300), Frame60);
            Assert.False(s.NeedsFrame);
            on = true;
            s.Tick(Ms(320));
            Assert.True(s.NeedsFrame);
            s.Tick(Ms(340));
            Assert.False(s.NeedsFrame);
        }

        [Fact]
        public void Closed_NeedsNoFrame()
        {
            GHRadialMenuState s = new GHRadialMenuState(Tps);
            s.Tick(0);
            Assert.False(s.NeedsFrame);
            Assert.False(s.IsVisible);
            Assert.Equal(GHRadialEffects.None, s.Apply(GHRadialInput.Confirm, 0).Effects);
            Assert.Equal(GHRadialEffects.None, s.UpdateStick(1, 0, 0.1).Effects);
        }

        [Fact]
        public void Durations_DoNotDependOnFrameRate()
        {
            GHRadialMenuDefinition def = Def(FullPage("Ctx", 8, 0));
            GHRadialMenuState a = Opened(def, 0);
            GHRadialMenuState b = Opened(def, 0);
            /* Highlight at a moment both have ticked */
            a.Apply(GHRadialInput.StepClockwise, 0);
            b.Apply(GHRadialInput.StepClockwise, 0);

            long common = Frame60 * 2;     /* 33.3 ms, a multiple of both frame lengths */
            long t60 = 0;
            long t90 = 0;
            long t;
            for (t = common; t <= Ms(300); t += common)
            {
                while (t60 < t)
                {
                    t60 += Frame60;
                    a.Tick(t60);
                }
                while (t90 < t)
                {
                    t90 += Frame90;
                    b.Tick(t90);
                }
                Assert.Equal(a.Phase, b.Phase);
                Assert.Equal(a.Opacity, b.Opacity, 4);
                Assert.Equal(a.RingScale, b.RingScale, 4);
                Assert.Equal(a.BackdropOpacity, b.BackdropOpacity, 4);
                Assert.Equal(a.GetHighlightWeight(0), b.GetHighlightWeight(0), 4);
            }
            Assert.Equal(GHRadialPhase.Open, a.Phase);

            /* The close takes the same time at both rates */
            long closeAt = t60;
            a.Close(GHRadialCloseReason.Cancelled, closeAt);
            b.Close(GHRadialCloseReason.Cancelled, closeAt);
            for (t = closeAt + common; t <= closeAt + Ms(200); t += common)
            {
                while (t60 < t)
                {
                    t60 += Frame60;
                    a.Tick(t60);
                }
                while (t90 < t)
                {
                    t90 += Frame90;
                    b.Tick(t90);
                }
                Assert.Equal(a.IsVisible, b.IsVisible);
                Assert.Equal(a.Opacity, b.Opacity, 4);
            }
            Assert.False(a.IsVisible);
        }

        [Fact]
        public void OpenAnimation_TakesTheOpenDuration()
        {
            GHRadialMenuState s = Opened(Def(FullPage("Ctx", 8, 0)), 0);
            s.Tick(Ms(70));
            Assert.Equal(GHRadialPhase.Opening, s.Phase);
            Assert.InRange(s.Opacity, 0.49f, 0.51f);
            Assert.InRange(s.RingScale, 0.85f, 1f);
            s.Tick(Ms(139));
            Assert.Equal(GHRadialPhase.Opening, s.Phase);
            s.Tick(Ms(141));
            Assert.Equal(GHRadialPhase.Open, s.Phase);
            Assert.Equal(1f, s.Opacity);
            Assert.Equal(1f, s.RingScale);
            Assert.Equal(0.6f, s.BackdropOpacity, 4);
        }

        [Fact]
        public void ReopeningMidFade_ContinuesFromCurrentVisibility()
        {
            GHRadialMenuDefinition def = Def(FullPage("Ctx", 8, 0));
            GHRadialMenuState s = Opened(def, 0);
            TickUntil(s, 0, Ms(300), Frame60);
            s.Close(GHRadialCloseReason.Cancelled, Ms(300));
            s.Tick(Ms(345));
            float faded = s.Opacity;
            Assert.InRange(faded, 0.45f, 0.55f);
            s.Open(def, Ms(345));
            Assert.True(s.IsOpen);
            Assert.Equal(faded, s.Opacity, 2);
        }

        [Fact]
        public void PageTransition_RunsForThePageDuration()
        {
            GHRadialMenuState s = Opened(Def(FullPage("A", 8, 0), FullPage("B", 12, 1)), 0);
            TickUntil(s, 0, Ms(300), Frame60);
            s.Apply(GHRadialInput.NextPage, Ms(300));
            Assert.Equal(0, s.PreviousPageIndex);
            Assert.Equal(1, s.PageDirection);
            s.Tick(Ms(370));
            Assert.InRange(s.PageTransition, 0.49f, 0.51f);
            Assert.True(s.NeedsFrame);
            s.Tick(Ms(441));
            Assert.Equal(1f, s.PageTransition);
            Assert.Equal(-1, s.PreviousPageIndex);
            s.Tick(Ms(460));
            Assert.False(s.NeedsFrame);
        }

        [Fact]
        public void Pointer_HighlightsActivatesAndCloses()
        {
            int activations = 0;
            GHRadialPage page = new GHRadialPage("Ctx", 8);
            page.Slots[2] = new GHRadialItem { Id = "east" };
            GHRadialMenuDefinition def = Def(page, FullPage("More", 12, 1));
            def.OnActivate = item => activations++;
            GHRadialMenuState s = Opened(def, 0);
            GHRadialGeometry g = s.Geometry;
            g.Update(1280, 800, 1);

            Assert.True(s.PointerMove(g.CenterX + g.MidRadius, g.CenterY).Has(GHRadialEffects.HighlightChanged));
            Assert.Equal(2, s.HighlightedSlot);
            Assert.True(s.IsPointerMode);
            Assert.False(s.ShowStickGhost);
            s.PointerMove(g.CenterX, g.CenterY);
            Assert.Equal(-1, s.HighlightedSlot);
            /* Outside the band keeps the highlight */
            s.PointerMove(g.CenterX + g.MidRadius, g.CenterY);
            s.PointerMove(g.CenterX + g.OuterRadius * 1.3f, g.CenterY);
            Assert.Equal(2, s.HighlightedSlot);

            /* An empty slot does nothing; the next-page chip pages */
            Assert.Equal(GHRadialEffects.Refused, s.PointerPress(g.CenterX - g.MidRadius, g.CenterY, Ms(1)).Effects);
            Assert.True(s.IsOpen);
            Assert.True(s.PointerPress(g.CenterX + g.ChipOffsetX, g.CenterY, Ms(2)).Has(GHRadialEffects.PageChanged));
            Assert.Equal(1, s.PageIndex);
            s.Apply(GHRadialInput.PrevPage, Ms(3));

            GHRadialApplyResult r = s.PointerPress(g.CenterX + g.MidRadius, g.CenterY, Ms(4));
            Assert.True(r.Has(GHRadialEffects.Activated));
            Assert.True(r.Has(GHRadialEffects.Closed));
            Assert.Equal(1, activations);

            s.Open(def, Ms(10));
            r = s.PointerPress(g.CenterX, g.CenterY, Ms(11));
            Assert.True(r.Has(GHRadialEffects.Closed));
            Assert.Equal(GHRadialCloseReason.Cancelled, r.CloseReason);
            Assert.Equal(1, activations);
        }

        [Fact]
        public void Stick_LeavesPointerMode()
        {
            GHRadialMenuState s = Opened(Def(FullPage("Ctx", 8, 0)), 0);
            s.Geometry.Update(1280, 800, 1);
            s.PointerMove(s.Geometry.CenterX, s.Geometry.CenterY);
            Assert.True(s.IsPointerMode);
            Push(s, 180, 0.9);
            Assert.False(s.IsPointerMode);
            Assert.True(s.ShowStickGhost);
            Assert.Equal(4, s.HighlightedSlot);
        }

        [Fact]
        public void HighlightSound_IsThrottled()
        {
            GHRadialMenuState s = new GHRadialMenuState(Tps);
            Assert.True(s.TryClaimHighlightSound(0));
            Assert.False(s.TryClaimHighlightSound(Ms(30)));
            Assert.True(s.TryClaimHighlightSound(Ms(50)));
            Assert.False(s.TryClaimHighlightSound(Ms(90)));
        }

        [Fact]
        public void L3Page_CenterOnHeroIsReachedEastAndCloses()
        {
            List<GHRadialItem> items = new List<GHRadialItem>
            {
                new GHRadialItem { Id = "l3:statusbar", Kind = GHRadialItemKind.Toggle, ClosesOnActivate = false, PlaysOwnSound = true, PreferredAngle = GHRadialTuning.ExtendedStatusBarAngle },
                new GHRadialItem { Id = "l3:autocenter", Kind = GHRadialItemKind.Toggle, ClosesOnActivate = false, PlaysOwnSound = true, PreferredAngle = GHRadialTuning.AutoCenterAngle },
                new GHRadialItem { Id = "l3:look", Kind = GHRadialItemKind.Toggle, ClosesOnActivate = true, PlaysOwnSound = true, PreferredAngle = GHRadialTuning.LookModeAngle },
                new GHRadialItem { Id = "l3:messages", Kind = GHRadialItemKind.Toggle, ClosesOnActivate = false, PlaysOwnSound = true, PreferredAngle = GHRadialTuning.MoreMessagesAngle },
                new GHRadialItem { Id = "l3:ignore", Kind = GHRadialItemKind.Toggle, ClosesOnActivate = false, PlaysOwnSound = true, PreferredAngle = GHRadialTuning.IgnoreStoppingAngle },
                new GHRadialItem { Id = "l3:autodig", Kind = GHRadialItemKind.Toggle, ClosesOnActivate = false, PlaysOwnSound = true, PreferredAngle = GHRadialTuning.AutoDigAngle },
                new GHRadialItem { Id = "l3:centeronhero", Kind = GHRadialItemKind.Command, ClosesOnActivate = true, PreferredAngle = GHRadialTuning.CenterOnHeroAngle }
            };
            GHRadialMenuDefinition def = new GHRadialMenuDefinition();
            def.MenuId = "L3";
            def.OwningStick = 0;
            def.ConfirmHint = "Toggle";
            def.Pages.Add(GHRadialSlotAssigner.BuildFixedPage("Toggles", 0, items));
            GHRadialItem activated = null;
            def.OnActivate = item => activated = item;

            GHRadialMenuState s = Opened(def, 0);
            Push(s, 90, 0.9);
            Assert.Equal("l3:centeronhero", s.HighlightedItem.Id);
            GHRadialApplyResult r = s.Apply(GHRadialInput.Confirm, Ms(1));
            Assert.True(r.Has(GHRadialEffects.Closed));
            Assert.Equal("l3:centeronhero", activated.Id);
            Assert.False(s.IsOpen);

            /* A toggle at its Design C direction keeps the menu open */
            s.Open(def, Ms(10));
            s.UpdateStick(0, 0, 1.0 / 60);
            Push(s, 60, 0.9);
            Assert.Equal("l3:autocenter", s.HighlightedItem.Id);
            r = s.Apply(GHRadialInput.Confirm, Ms(11));
            Assert.False(r.Has(GHRadialEffects.Closed));
            Assert.True(r.Item.PlaysOwnSound);
            Assert.True(s.IsOpen);
        }
    }
}
