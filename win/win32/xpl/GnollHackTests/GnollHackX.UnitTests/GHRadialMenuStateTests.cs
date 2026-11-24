using System;
using System.Collections.Generic;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers GHRadialMenuState: the initial page, paging clamps, SwitchSet between Context and More Commands, R53's
       group paging, the highlight re-evaluated or restored on page change, ClosesOnActivate,
       logical close before the visual close, callback order, refused confirms, NeedsFrame at
       rest, toggle state changes, pointer input, page dots counted within the current set, the
       highlight sound throttle, R74's Center on
       Hero in the L3 page, R1's push-up-is-north, and durations that do not depend on the frame
       rate (60 Hz against 90 Hz ticks). Also the item action, Cancel back to a parent page,
       paging confined to a group, OnCancel, ReplaceDefinition, GoToPageAndSlot, the set switch's
       enable and return page, and the pointer geometry offset. */
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
        public void OpenerReleased_WithHighlight_ActivatesOnceAndCloses()
        {
            int activated = 0;
            List<GHRadialCloseReason> reasons = new List<GHRadialCloseReason>();
            GHRadialMenuDefinition def = Def(FullPage("Dir", 8, 0));
            def.OnActivate = item => activated++;
            def.OnClosed = reason => reasons.Add(reason);
            GHRadialMenuState s = Opened(def, 0);
            Push(s, 90, 1.0);
            GHRadialApplyResult r = s.Apply(GHRadialInput.OpenerReleased, Ms(1));
            Assert.True(r.Has(GHRadialEffects.Activated));
            Assert.True(r.Has(GHRadialEffects.Closed));
            Assert.False(s.IsOpen);
            Assert.Equal(1, activated);
            Assert.Equal(new List<GHRadialCloseReason> { GHRadialCloseReason.Confirmed }, reasons);
        }

        [Fact]
        public void OpenerReleased_WithoutHighlight_Cancels()
        {
            int activated = 0;
            List<GHRadialCloseReason> reasons = new List<GHRadialCloseReason>();
            GHRadialMenuDefinition def = Def(FullPage("Dir", 8, 0));
            def.OnActivate = item => activated++;
            def.OnClosed = reason => reasons.Add(reason);
            GHRadialMenuState s = Opened(def, 0);
            GHRadialApplyResult r = s.Apply(GHRadialInput.OpenerReleased, Ms(1));
            Assert.True(r.Has(GHRadialEffects.Closed));
            Assert.False(r.Has(GHRadialEffects.Activated));
            Assert.False(s.IsOpen);
            Assert.Equal(0, activated);
            Assert.Equal(new List<GHRadialCloseReason> { GHRadialCloseReason.Cancelled }, reasons);
        }

        [Fact]
        public void OpenerReleased_OnDisabledItem_Cancels()
        {
            int activated = 0;
            List<GHRadialCloseReason> reasons = new List<GHRadialCloseReason>();
            GHRadialPage page = FullPage("Dir", 8, 0);
            page.Slots[2].IsEnabled = false;
            GHRadialMenuDefinition def = Def(page);
            def.OnActivate = item => activated++;
            def.OnClosed = reason => reasons.Add(reason);
            GHRadialMenuState s = Opened(def, 0);
            Push(s, 90, 1.0);
            Assert.Equal(2, s.HighlightedSlot);
            s.Apply(GHRadialInput.OpenerReleased, Ms(1));
            Assert.False(s.IsOpen);
            Assert.Equal(0, activated);
            Assert.Equal(new List<GHRadialCloseReason> { GHRadialCloseReason.Cancelled }, reasons);
        }

        [Fact]
        public void OpenerReleased_OnItemThatStaysOpen_Closes()
        {
            int activated = 0;
            List<GHRadialCloseReason> reasons = new List<GHRadialCloseReason>();
            GHRadialPage page = FullPage("Dir", 8, 0);
            page.Slots[2].ClosesOnActivate = false;
            GHRadialMenuDefinition def = Def(page);
            def.OnActivate = item => activated++;
            def.OnClosed = reason => reasons.Add(reason);
            GHRadialMenuState s = Opened(def, 0);
            Push(s, 90, 1.0);
            GHRadialApplyResult r = s.Apply(GHRadialInput.OpenerReleased, Ms(1));
            Assert.True(r.Has(GHRadialEffects.Activated));
            Assert.True(r.Has(GHRadialEffects.Closed));
            Assert.False(s.IsOpen);
            Assert.Equal(1, activated);
            Assert.Equal(new List<GHRadialCloseReason> { GHRadialCloseReason.Confirmed }, reasons);
        }

        [Fact]
        public void OpenerReleased_AfterStickReturnedToCenter_KeepsTheSelection()
        {
            GHRadialItem activated = null;
            GHRadialPage page = FullPage("Dir", 8, 0);
            GHRadialMenuDefinition def = Def(page);
            def.OnActivate = item => activated = item;
            GHRadialMenuState s = Opened(def, 0);
            Push(s, 90, 1.0);
            s.UpdateStick(0, 0, 1.0);
            Assert.Equal(2, s.HighlightedSlot);
            s.Apply(GHRadialInput.OpenerReleased, Ms(1));
            Assert.Same(page.Slots[2], activated);
        }

        [Fact]
        public void StickArmedAtOpen_APushedStickSelectsAtOnce()
        {
            GHRadialMenuDefinition def = Def(FullPage("Dir", 8, 0));
            def.StickArmedAtOpen = true;
            GHRadialMenuState s = new GHRadialMenuState(Tps);
            Assert.True(s.Open(def, 0));
            s.UpdateStick(1.0, 0, 0.0);
            Assert.Equal(2, s.HighlightedSlot);

            GHRadialMenuState other = new GHRadialMenuState(Tps);
            Assert.True(other.Open(Def(FullPage("Ctx", 8, 0)), 0));
            other.UpdateStick(1.0, 0, 0.0);
            Assert.Equal(-1, other.HighlightedSlot);
        }

        [Fact]
        public void SeeThrough_LowersTheBackdrop()
        {
            GHRadialMenuDefinition seeThrough = Def(FullPage("Dir", 8, 0));
            seeThrough.SeeThrough = true;
            GHRadialMenuState s = new GHRadialMenuState(Tps);
            Assert.True(s.Open(seeThrough, 0));
            s.Tick(Ms(1000));
            Assert.Equal((float)GHRadialTuning.SeeThroughBackdropOpacity, s.BackdropOpacity, 3);

            GHRadialMenuState other = new GHRadialMenuState(Tps);
            Assert.True(other.Open(Def(FullPage("Ctx", 8, 0)), 0));
            other.Tick(Ms(1000));
            Assert.Equal((float)GHRadialTuning.BackdropMaxOpacity, other.BackdropOpacity, 3);
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
        public void NeedsFrame_IsTrueOnTheTickThatEndsAnAnimation()
        {
            GHRadialMenuState s = Opened(Def(FullPage("Ctx", 8, 0)), 0);
            s.Tick(Ms(100));
            Assert.True(s.NeedsFrame);
            Assert.True(s.Opacity < 1f);

            /* A late tick ends the opening at once: it must still paint full opacity */
            s.Tick(Ms(200));
            Assert.Equal(GHRadialPhase.Open, s.Phase);
            Assert.Equal(1f, s.Opacity);
            Assert.False(s.IsAnimating);
            Assert.True(s.NeedsFrame);
            s.Tick(Ms(216));
            Assert.False(s.NeedsFrame);

            Push(s, 90, 0.9);
            s.Tick(Ms(220));
            Assert.True(s.NeedsFrame);
            Assert.True(s.GetHighlightWeight(2) < 1f);
            s.Tick(Ms(700));
            Assert.Equal(1f, s.GetHighlightWeight(2));
            Assert.True(s.NeedsFrame);
            s.Tick(Ms(716));
            Assert.False(s.NeedsFrame);
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
        public void SetRange_SplitsAtTheSwitchSetPage()
        {
            GHRadialMenuDefinition def = Def(FullPage("Context", 8, 0), FullPage("Commands 1/3", 8, 1),
                FullPage("Commands 2/3", 8, 1), FullPage("Commands 3/3", 8, 1));
            int first, count;
            def.GetSetRange(2, out first, out count);
            Assert.Equal(0, first);
            Assert.Equal(4, count);

            def.SwitchSetPageIndex = 1;
            def.GetSetRange(0, out first, out count);
            Assert.Equal(0, first);
            Assert.Equal(1, count);
            def.GetSetRange(1, out first, out count);
            Assert.Equal(1, first);
            Assert.Equal(3, count);
            def.GetSetRange(3, out first, out count);
            Assert.Equal(1, first);
            Assert.Equal(3, count);

            /* A switch page past the end is unused */
            def.SwitchSetPageIndex = 4;
            def.GetSetRange(3, out first, out count);
            Assert.Equal(0, first);
            Assert.Equal(4, count);
        }

        [Fact]
        public void PointerDots_PageWithinTheCurrentSet()
        {
            GHRadialMenuDefinition def = Def(FullPage("Context", 8, 0), FullPage("Commands 1/3", 8, 1),
                FullPage("Commands 2/3", 8, 1), FullPage("Commands 3/3", 8, 1));
            def.SwitchSetPageIndex = 1;
            def.InitialPageIndex = 1;
            GHRadialMenuState s = Opened(def, 0);
            GHRadialGeometry g = s.Geometry;
            g.Update(1280, 800, 1);

            /* Three dots for the three commands pages; the last one is page 3 */
            float x, y;
            g.GetDotCenter(2, 3, out x, out y);
            Assert.True(s.PointerPress(x, y, Ms(1)).Has(GHRadialEffects.PageChanged));
            Assert.Equal(3, s.PageIndex);
            g.GetDotCenter(0, 3, out x, out y);
            s.PointerPress(x, y, Ms(2));
            Assert.Equal(1, s.PageIndex);
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
                new GHRadialItem { Id = "l3:look", Kind = GHRadialItemKind.Toggle, ClosesOnActivate = false, PlaysOwnSound = true, PreferredAngle = GHRadialTuning.LookModeAngle },
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

        private sealed class ItemActionCall
        {
            public GHRadialItem Item;
            public int Page;
            public int Slot;
        }

        [Fact]
        public void ItemAction_RunsOnTheHighlightedSlotWithoutClosing()
        {
            List<ItemActionCall> calls = new List<ItemActionCall>();
            int activations = 0;
            GHRadialPage page = FullPage("Spells", 8, 0);
            page.Slots[2].IsEnabled = false;
            page.Slots[5] = null;
            GHRadialMenuDefinition def = Def(page);
            def.OnActivate = item => activations++;
            def.OnItemAction = (item, p, slot) => calls.Add(new ItemActionCall { Item = item, Page = p, Slot = slot });
            GHRadialMenuState s = Opened(def, 0);

            /* A disabled item: Confirm refuses it, ItemAction still runs */
            Push(s, 90, 0.9);
            Assert.Equal(2, s.HighlightedSlot);
            Assert.Equal(GHRadialEffects.Refused, s.Apply(GHRadialInput.Confirm, Ms(1)).Effects);
            GHRadialApplyResult r = s.Apply(GHRadialInput.ItemAction, Ms(2));
            Assert.True(r.Has(GHRadialEffects.Activated));
            Assert.False(r.Has(GHRadialEffects.Closed));
            Assert.Same(page.Slots[2], r.Item);
            Assert.True(s.IsOpen);
            Assert.Single(calls);
            Assert.Same(page.Slots[2], calls[0].Item);
            Assert.Equal(0, calls[0].Page);
            Assert.Equal(2, calls[0].Slot);
            Assert.Equal(0, activations);

            /* An empty slot passes a null item */
            s.UpdateStick(0, 0, 1.0);
            s.GoToPageAndSlot(0, 5, Ms(3));
            Assert.Equal(5, s.HighlightedSlot);
            r = s.Apply(GHRadialInput.ItemAction, Ms(4));
            Assert.Null(r.Item);
            Assert.Equal(2, calls.Count);
            Assert.Null(calls[1].Item);
            Assert.Equal(5, calls[1].Slot);

            /* No highlight passes slot -1 */
            s.GoToPageAndSlot(0, -1, Ms(5));
            s.Apply(GHRadialInput.ItemAction, Ms(6));
            Assert.Equal(3, calls.Count);
            Assert.Null(calls[2].Item);
            Assert.Equal(-1, calls[2].Slot);
            Assert.True(s.IsOpen);
        }

        [Fact]
        public void ItemAction_WithoutCallback_DoesNothing()
        {
            GHRadialMenuState s = Opened(Def(FullPage("Ctx", 8, 0)), 0);
            s.Apply(GHRadialInput.StepClockwise, Ms(1));
            Assert.Equal(GHRadialEffects.None, s.Apply(GHRadialInput.ItemAction, Ms(2)).Effects);
            Assert.True(s.IsOpen);
        }

        /* A school ring on page 0 whose slot 3 opens the school's two pages 1 and 2 */
        private static GHRadialMenuDefinition SchoolDef(Func<GHRadialMenuState> state)
        {
            GHRadialPage ring = FullPage("Known Spells", 12, 0);
            GHRadialPage school1 = FullPage("Healing 1/2", 8, 1);
            GHRadialPage school2 = FullPage("Healing 2/2", 8, 1);
            school1.ParentPageIndex = 0;
            school2.ParentPageIndex = 0;
            ring.Slots[3].ClosesOnActivate = false;
            GHRadialMenuDefinition def = Def(ring, school1, school2);
            def.PageWithinGroup = true;
            def.OnActivate = item =>
            {
                if (item == ring.Slots[3])
                    state().GoToPageAndSlot(1, -1, Ms(2));
            };
            return def;
        }

        [Fact]
        public void Cancel_OnAPageWithAParent_GoesBackToTheSlotThatLedThere()
        {
            List<GHRadialCloseReason> reasons = new List<GHRadialCloseReason>();
            GHRadialMenuState s = null;
            GHRadialMenuDefinition def = SchoolDef(() => s);
            def.OnClosed = reason => reasons.Add(reason);
            s = Opened(def, 0);
            s.GoToPageAndSlot(0, 3, Ms(1));
            GHRadialApplyResult r = s.Apply(GHRadialInput.Confirm, Ms(2));
            Assert.True(r.Has(GHRadialEffects.Activated));
            Assert.Equal(1, s.PageIndex);
            Assert.Equal(-1, s.HighlightedSlot);

            /* Paging inside the school, then back to the ring with the school's slot highlighted */
            s.Apply(GHRadialInput.NextPage, Ms(3));
            Assert.Equal(2, s.PageIndex);
            s.Apply(GHRadialInput.StepClockwise, Ms(4));
            r = s.Apply(GHRadialInput.Cancel, Ms(5));
            Assert.True(r.Has(GHRadialEffects.PageChanged));
            Assert.False(r.Has(GHRadialEffects.Closed));
            Assert.Equal(0, s.PageIndex);
            Assert.Equal(3, s.HighlightedSlot);
            Assert.True(s.IsOpen);
            Assert.Empty(reasons);

            /* The ring has no parent: Cancel closes */
            r = s.Apply(GHRadialInput.Cancel, Ms(6));
            Assert.True(r.Has(GHRadialEffects.Closed));
            Assert.Equal(new List<GHRadialCloseReason> { GHRadialCloseReason.Cancelled }, reasons);
        }

        [Fact]
        public void Cancel_ParentPageHighlightWinsOverADeflectedStick()
        {
            GHRadialMenuState s = null;
            GHRadialMenuDefinition def = SchoolDef(() => s);
            s = Opened(def, 0);
            s.GoToPageAndSlot(0, 3, Ms(1));
            s.Apply(GHRadialInput.Confirm, Ms(2));
            Push(s, 180, 0.9);
            Assert.Equal(4, s.HighlightedSlot);
            s.Apply(GHRadialInput.Cancel, Ms(3));
            Assert.Equal(0, s.PageIndex);
            Assert.Equal(3, s.HighlightedSlot);
        }

        [Fact]
        public void OpenerPressed_ClosesEvenOnAPageWithAParent()
        {
            int cancels = 0;
            GHRadialMenuState s = null;
            GHRadialMenuDefinition def = SchoolDef(() => s);
            def.OnCancel = () => { cancels++; return true; };
            s = Opened(def, 0);
            s.GoToPageAndSlot(1, -1, Ms(1));
            GHRadialApplyResult r = s.Apply(GHRadialInput.OpenerPressed, Ms(2));
            Assert.True(r.Has(GHRadialEffects.Closed));
            Assert.False(s.IsOpen);
            Assert.Equal(0, cancels);
        }

        [Fact]
        public void PageWithinGroup_KeysStayInTheGroup()
        {
            GHRadialMenuDefinition def = Def(FullPage("Hotbar 1/3", 8, 0), FullPage("Hotbar 2/3", 8, 0), FullPage("Hotbar 3/3", 8, 0),
                FullPage("Known 1/2", 8, 1), FullPage("Known 2/2", 8, 1));
            def.PageWithinGroup = true;
            GHRadialMenuState s = Opened(def, 0);
            Assert.Equal(GHRadialEffects.None, s.Apply(GHRadialInput.PrevPage, Ms(1)).Effects);
            s.Apply(GHRadialInput.NextPage, Ms(2));
            s.Apply(GHRadialInput.NextPage, Ms(3));
            Assert.Equal(2, s.PageIndex);
            Assert.Equal(GHRadialEffects.None, s.Apply(GHRadialInput.NextPage, Ms(4)).Effects);
            Assert.Equal(GHRadialEffects.None, s.PointerWheel(1, Ms(5)).Effects);
            Assert.Equal(2, s.PageIndex);

            s.GoToPageAndSlot(3, -1, Ms(6));
            Assert.Equal(3, s.PageIndex);
            Assert.Equal(GHRadialEffects.None, s.Apply(GHRadialInput.PrevPage, Ms(7)).Effects);
            s.Apply(GHRadialInput.NextPage, Ms(8));
            Assert.Equal(4, s.PageIndex);

            /* Without PageWithinGroup the keys cross groups */
            def.PageWithinGroup = false;
            s.Apply(GHRadialInput.PrevPage, Ms(9));
            s.Apply(GHRadialInput.PrevPage, Ms(10));
            Assert.Equal(2, s.PageIndex);
        }

        [Fact]
        public void PageWithinGroup_PointerChipsAndDotsStayInTheGroup()
        {
            GHRadialMenuDefinition def = Def(FullPage("Hotbar 1/3", 8, 0), FullPage("Hotbar 2/3", 8, 0), FullPage("Hotbar 3/3", 8, 0),
                FullPage("Known 1/2", 8, 1), FullPage("Known 2/2", 8, 1));
            def.PageWithinGroup = true;
            def.InitialPageIndex = 3;
            GHRadialMenuState s = Opened(def, 0);
            GHRadialGeometry g = s.Geometry;
            g.Update(1280, 800, 1);

            /* Two dots for the group's two pages */
            float x, y;
            g.GetDotCenter(1, 2, out x, out y);
            Assert.True(s.PointerPress(x, y, Ms(1)).Has(GHRadialEffects.PageChanged));
            Assert.Equal(4, s.PageIndex);
            g.GetDotCenter(0, 2, out x, out y);
            s.PointerPress(x, y, Ms(2));
            Assert.Equal(3, s.PageIndex);

            /* The previous chip on the group's first page does not leave the group */
            GHRadialApplyResult r = s.PointerPress(g.CenterX - g.ChipOffsetX, g.CenterY, Ms(3));
            Assert.False(r.Has(GHRadialEffects.PageChanged));
            Assert.NotEqual(2, s.PageIndex);

            /* The next chip stays inside */
            s = Opened(def, Ms(10));
            s.Geometry.Update(1280, 800, 1);
            Assert.True(s.PointerPress(s.Geometry.CenterX + s.Geometry.ChipOffsetX, s.Geometry.CenterY, Ms(11)).Has(GHRadialEffects.PageChanged));
            Assert.Equal(4, s.PageIndex);
        }

        [Fact]
        public void GroupRange_IsTheRunOfTheSameGroup()
        {
            GHRadialMenuDefinition def = Def(FullPage("A", 8, 0), FullPage("B", 8, 1), FullPage("C", 8, 1), FullPage("D", 8, 2));
            int first, count;
            def.GetGroupRange(2, out first, out count);
            Assert.Equal(1, first);
            Assert.Equal(2, count);
            def.GetGroupRange(3, out first, out count);
            Assert.Equal(3, first);
            Assert.Equal(1, count);
            def.GetPagingRange(2, out first, out count);
            Assert.Equal(0, first);
            Assert.Equal(4, count);
            def.PageWithinGroup = true;
            def.GetPagingRange(2, out first, out count);
            Assert.Equal(1, first);
            Assert.Equal(2, count);
            def.GetDotRange(0, out first, out count);
            Assert.Equal(0, first);
            Assert.Equal(1, count);
        }

        [Fact]
        public void OnCancel_TrueKeepsTheMenuOpen()
        {
            bool consume = true;
            int cancels = 0;
            List<GHRadialCloseReason> reasons = new List<GHRadialCloseReason>();
            GHRadialMenuDefinition def = Def(FullPage("Hotbar", 8, 0));
            def.OnCancel = () => { cancels++; return consume; };
            def.OnClosed = reason => reasons.Add(reason);
            GHRadialMenuState s = Opened(def, 0);
            GHRadialApplyResult r = s.Apply(GHRadialInput.Cancel, Ms(1));
            Assert.Equal(GHRadialEffects.None, r.Effects);
            Assert.True(s.IsOpen);
            Assert.Equal(1, cancels);
            Assert.Empty(reasons);

            consume = false;
            r = s.Apply(GHRadialInput.Cancel, Ms(2));
            Assert.True(r.Has(GHRadialEffects.Closed));
            Assert.False(s.IsOpen);
            Assert.Equal(2, cancels);
            Assert.Equal(new List<GHRadialCloseReason> { GHRadialCloseReason.Cancelled }, reasons);
        }

        [Fact]
        public void OnCancel_ThatReplacesTheDefinition_ReportsAPageChange()
        {
            GHRadialMenuState s = null;
            GHRadialMenuDefinition actions = Def(FullPage("Actions", 8, 0));
            GHRadialMenuDefinition hotbar = Def(FullPage("Hotbar 1/3", 8, 0), FullPage("Hotbar 2/3", 8, 0));
            actions.OnCancel = () =>
            {
                s.ReplaceDefinition(hotbar, 1, 4, Ms(220), false);
                return true;
            };
            s = Opened(actions, 0);
            TickUntil(s, 0, Ms(200), Frame60);
            GHRadialApplyResult r = s.Apply(GHRadialInput.Cancel, Ms(220));
            Assert.True(r.Has(GHRadialEffects.PageChanged));
            Assert.False(r.Has(GHRadialEffects.Closed));
            Assert.Same(hotbar, s.Definition);
            Assert.Equal(1, s.PageIndex);
            Assert.Equal(4, s.HighlightedSlot);
        }

        [Fact]
        public void ReplaceDefinition_SwapsPagesWithoutReopening()
        {
            int closes = 0;
            GHRadialMenuDefinition first = Def(FullPage("Hotbar 1/3", 8, 0), FullPage("Hotbar 2/3", 8, 0), FullPage("Hotbar 3/3", 8, 0));
            first.OnClosed = reason => closes++;
            GHRadialMenuDefinition second = Def(FullPage("Actions", 8, 0));
            GHRadialMenuState s = Opened(first, 0);
            TickUntil(s, 0, Ms(300), Frame60);
            s.Apply(GHRadialInput.NextPage, Ms(300));
            s.Apply(GHRadialInput.NextPage, Ms(301));
            s.Apply(GHRadialInput.StepClockwise, Ms(302));
            TickUntil(s, Ms(310), Ms(600), Frame60);
            GHRadialPage shown = s.CurrentPage;
            Assert.Equal(2, s.PageIndex);

            /* Three pages become one: the page is clamped and the highlight set */
            s.ReplaceDefinition(second, 2, 6, Ms(600), false);
            Assert.Same(second, s.Definition);
            Assert.Equal(1, s.PageCount);
            Assert.Equal(0, s.PageIndex);
            Assert.Equal(6, s.HighlightedSlot);
            Assert.Equal(1f, s.GetHighlightWeight(6));
            Assert.Equal(GHRadialPhase.Open, s.Phase);
            Assert.Equal(1f, s.Opacity);
            Assert.True(s.IsOpen);
            Assert.Equal(0, closes);
            Assert.Equal(-1, s.FlashSlot);

            /* The crossfade starts from the page shown before */
            Assert.True(s.PageTransition < 1f);
            Assert.Same(shown, s.PreviousPage);
            Assert.Equal(0, s.PreviousPageHighlight);
            Assert.Equal(0, s.PageDirection);
            s.Tick(Ms(620));
            Assert.True(s.NeedsFrame);
            TickUntil(s, Ms(640), Ms(900), Frame60);
            Assert.Null(s.PreviousPage);
            Assert.False(s.NeedsFrame);

            /* One page becomes three, the page highlights start over */
            s.ReplaceDefinition(first, 1, -1, Ms(900), false);
            Assert.Equal(3, s.PageCount);
            Assert.Equal(1, s.PageIndex);
            Assert.Equal(-1, s.HighlightedSlot);
            Assert.Equal(-1, s.GetPageHighlight(0));
            Assert.Equal(-1, s.GetPageHighlight(2));
            s.Apply(GHRadialInput.NextPage, Ms(901));
            Assert.Equal(2, s.PageIndex);
            Assert.Equal(-1, s.HighlightedSlot);
        }

        [Fact]
        public void ReplaceDefinition_FlashesTheSlotOnlyWhenAsked()
        {
            GHRadialMenuDefinition first = Def(FullPage("Hotbar", 8, 0));
            GHRadialMenuDefinition second = Def(FullPage("Hotbar", 8, 0), FullPage("Hotbar 2", 8, 0));
            GHRadialMenuState s = Opened(first, 0);
            TickUntil(s, 0, Ms(300), Frame60);
            s.ReplaceDefinition(second, 1, 3, Ms(300), true);
            Assert.Equal(3, s.FlashSlot);
            Assert.True(s.FlashIntensity > 0f);
            TickUntil(s, Ms(310), Ms(500), Frame60);
            Assert.Equal(-1, s.FlashSlot);

            s.ReplaceDefinition(first, 0, 2, Ms(500), false);
            Assert.Equal(-1, s.FlashSlot);

            /* Nothing happens while closed */
            s.Close(GHRadialCloseReason.Cancelled, Ms(510), true);
            s.ReplaceDefinition(second, 1, 1, Ms(520), true);
            Assert.Same(first, s.Definition);
            Assert.False(s.IsVisible);
        }

        [Fact]
        public void ReplaceDefinition_FromOnActivate_StillReportsActivated()
        {
            int closes = 0;
            int activations = 0;
            GHRadialMenuState s = null;
            GHRadialPage hotbar = FullPage("Hotbar", 8, 0);
            hotbar.Slots[2].ClosesOnActivate = false;
            GHRadialMenuDefinition first = Def(hotbar);
            GHRadialMenuDefinition actions = Def(FullPage("Actions", 8, 0));
            first.OnClosed = reason => closes++;
            first.OnActivate = item =>
            {
                activations++;
                s.ReplaceDefinition(actions, 0, 7, Ms(400), false);
            };
            s = Opened(first, 0);
            TickUntil(s, 0, Ms(300), Frame60);
            Push(s, 90, 0.9);
            GHRadialApplyResult r = s.Apply(GHRadialInput.Confirm, Ms(400));
            Assert.Equal(GHRadialEffects.Activated, r.Effects);
            Assert.Same(hotbar.Slots[2], r.Item);
            Assert.Equal(1, activations);
            Assert.Equal(0, closes);
            Assert.True(s.IsOpen);
            Assert.Same(actions, s.Definition);
            Assert.Equal(7, s.HighlightedSlot);
            /* The confirm's flash on the replaced page is cleared */
            Assert.Equal(-1, s.FlashSlot);

            /* From OnItemAction, with the flash kept */
            GHRadialMenuDefinition back = Def(FullPage("Hotbar", 8, 0));
            actions.OnItemAction = (item, page, slot) => s.ReplaceDefinition(back, 0, slot, Ms(500), true);
            r = s.Apply(GHRadialInput.ItemAction, Ms(500));
            Assert.True(r.Has(GHRadialEffects.Activated));
            Assert.Same(back, s.Definition);
            Assert.Equal(7, s.HighlightedSlot);
            Assert.Equal(7, s.FlashSlot);
        }

        [Fact]
        public void SwitchSetEnabled_False_BlocksSwitching()
        {
            GHRadialMenuDefinition def = Def(FullPage("Hotbar", 8, 0), FullPage("Known", 8, 1));
            def.SwitchSetPageIndex = 1;
            def.SwitchSetEnabled = false;
            GHRadialMenuState s = Opened(def, 0);
            Assert.Equal(GHRadialEffects.None, s.Apply(GHRadialInput.SwitchSet, Ms(1)).Effects);
            Assert.Equal(0, s.PageIndex);
            def.SwitchSetEnabled = true;
            Assert.True(s.Apply(GHRadialInput.SwitchSet, Ms(2)).Has(GHRadialEffects.PageChanged));
            Assert.Equal(1, s.PageIndex);
        }

        [Fact]
        public void SwitchSetReturnPageIndex_IsWhereTheSwitchedSetGoesBack()
        {
            GHRadialMenuDefinition def = Def(FullPage("Hotbar 1/3", 8, 0), FullPage("Hotbar 2/3", 8, 0), FullPage("Hotbar 3/3", 8, 0),
                FullPage("Known 1/2", 8, 1), FullPage("Known 2/2", 8, 1));
            def.SwitchSetPageIndex = 3;
            def.SwitchSetReturnPageIndex = 2;
            def.InitialPageIndex = 1;
            GHRadialMenuState s = Opened(def, 0);
            s.Apply(GHRadialInput.SwitchSet, Ms(1));
            Assert.Equal(3, s.PageIndex);
            s.Apply(GHRadialInput.NextPage, Ms(2));
            s.Apply(GHRadialInput.SwitchSet, Ms(3));
            Assert.Equal(2, s.PageIndex);

            /* Unset, it goes back to page 0 as before */
            def.SwitchSetReturnPageIndex = -1;
            s.Apply(GHRadialInput.SwitchSet, Ms(4));
            s.Apply(GHRadialInput.SwitchSet, Ms(5));
            Assert.Equal(0, s.PageIndex);
        }

        [Fact]
        public void GoToPageAndSlot_SetsThePageAndTheHighlight()
        {
            GHRadialMenuState s = Opened(Def(FullPage("A", 8, 0), FullPage("B", 8, 1)), 0);
            GHRadialApplyResult r = s.GoToPageAndSlot(1, 6, Ms(1));
            Assert.True(r.Has(GHRadialEffects.PageChanged));
            Assert.Equal(1, s.PageIndex);
            Assert.Equal(6, s.HighlightedSlot);
            Assert.True(s.PageTransition < 1f);
            Assert.Equal(1f, s.GetHighlightWeight(6));

            r = s.GoToPageAndSlot(1, 2, Ms(2));
            Assert.Equal(GHRadialEffects.HighlightChanged, r.Effects);
            Assert.Equal(2, s.HighlightedSlot);
            s.GoToPageAndSlot(0, -1, Ms(3));
            Assert.Equal(-1, s.HighlightedSlot);
            Assert.Equal(GHRadialEffects.None, s.GoToPageAndSlot(5, 0, Ms(4)).Effects);
            Assert.Equal(0, s.PageIndex);
        }

        [Fact]
        public void GeometryOffset_ShiftsPointerHitTests()
        {
            GHRadialPage page = new GHRadialPage("Ctx", 8);
            page.Slots[2] = new GHRadialItem { Id = "east" };
            page.Slots[4] = new GHRadialItem { Id = "south" };
            GHRadialMenuState s = Opened(Def(page), 0);
            GHRadialGeometry g = s.Geometry;
            g.Update(1280, 700, 1);
            const float offset = 150f;
            s.GeometryOffsetY = offset;
            /* Unshifted, this point would be in the empty southeast slot */
            s.PointerMove(g.CenterX + g.MidRadius, g.CenterY + offset);
            Assert.Equal(2, s.HighlightedSlot);
            s.PointerMove(g.CenterX, g.CenterY + g.MidRadius + offset);
            Assert.Equal(4, s.HighlightedSlot);
            s.PointerMove(g.CenterX, g.CenterY + offset);
            Assert.Equal(-1, s.HighlightedSlot);
        }
    }
}
