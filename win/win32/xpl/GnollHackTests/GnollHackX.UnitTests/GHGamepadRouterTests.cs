using System;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers GHGamepadPressRouter: a press belongs to the (page, popup open) target that got its Pressed; a
       target change cancels the release and drops ShortPressed, LongPressed and Repeat */
    public class GHGamepadRouterTests
    {
        private static readonly object GamePage = new object();
        private static readonly object MenuPage = new object();

        private static GHGamepadButtonEvent Ev(GHGamepadButton button, GHGamepadButtonEventKind kind, int pressId, bool cancelled = false)
        {
            GHGamepadButtonEvent e = new GHGamepadButtonEvent();
            e.Button = button;
            e.Kind = kind;
            e.PressId = pressId;
            e.IsCancelled = cancelled;
            return e;
        }

        private static GHGamepadRouteAction Route(GHGamepadPressRouter router, GHGamepadButtonEvent e, object target, bool popup)
        {
            object captured;
            bool capturedPopup;
            return router.Route(e, target, popup, out captured, out capturedPopup);
        }

        [Fact]
        public void SameTarget_DeliversTheWholePress()
        {
            GHGamepadPressRouter router = new GHGamepadPressRouter();
            Assert.Equal(GHGamepadRouteAction.Deliver, Route(router, Ev(GHGamepadButton.A, GHGamepadButtonEventKind.Pressed, 1), GamePage, false));
            Assert.Equal(GHGamepadRouteAction.Deliver, Route(router, Ev(GHGamepadButton.A, GHGamepadButtonEventKind.Released, 1), GamePage, false));
            Assert.Equal(GHGamepadRouteAction.Deliver, Route(router, Ev(GHGamepadButton.A, GHGamepadButtonEventKind.ShortPressed, 1), GamePage, false));
        }

        [Fact]
        public void PageChange_CancelsTheReleaseToTheCapturingPage()
        {
            GHGamepadPressRouter router = new GHGamepadPressRouter();
            Route(router, Ev(GHGamepadButton.Menu, GHGamepadButtonEventKind.Pressed, 7), GamePage, false);

            object captured;
            bool capturedPopup;
            GHGamepadRouteAction action = router.Route(Ev(GHGamepadButton.Menu, GHGamepadButtonEventKind.Released, 7), MenuPage, false,
                out captured, out capturedPopup);

            Assert.Equal(GHGamepadRouteAction.DeliverCancelled, action);
            Assert.Same(GamePage, captured);
            Assert.False(capturedPopup);
            Assert.Equal(GHGamepadRouteAction.Drop, Route(router, Ev(GHGamepadButton.Menu, GHGamepadButtonEventKind.ShortPressed, 7), MenuPage, false));
        }

        [Fact]
        public void PageChange_DropsRepeatAndLongPressed()
        {
            GHGamepadPressRouter router = new GHGamepadPressRouter();
            Route(router, Ev(GHGamepadButton.DPadDown, GHGamepadButtonEventKind.Pressed, 3), GamePage, false);
            Assert.Equal(GHGamepadRouteAction.Deliver, Route(router, Ev(GHGamepadButton.DPadDown, GHGamepadButtonEventKind.Repeat, 3), GamePage, false));
            Assert.Equal(GHGamepadRouteAction.Drop, Route(router, Ev(GHGamepadButton.DPadDown, GHGamepadButtonEventKind.Repeat, 3), MenuPage, false));
            Assert.Equal(GHGamepadRouteAction.Drop, Route(router, Ev(GHGamepadButton.DPadDown, GHGamepadButtonEventKind.LongPressed, 3), MenuPage, false));
        }

        [Fact]
        public void PopupOpening_IsATargetChange()
        {
            GHGamepadPressRouter router = new GHGamepadPressRouter();
            Route(router, Ev(GHGamepadButton.A, GHGamepadButtonEventKind.Pressed, 4), GamePage, false);

            object captured;
            bool capturedPopup;
            Assert.Equal(GHGamepadRouteAction.DeliverCancelled,
                router.Route(Ev(GHGamepadButton.A, GHGamepadButtonEventKind.Released, 4), GamePage, true, out captured, out capturedPopup));
            Assert.Same(GamePage, captured);
            Assert.False(capturedPopup);
            Assert.Equal(GHGamepadRouteAction.Drop, Route(router, Ev(GHGamepadButton.A, GHGamepadButtonEventKind.ShortPressed, 4), GamePage, true));
        }

        [Fact]
        public void TargetThatChangesAndChangesBack_StillCancels()
        {
            GHGamepadPressRouter router = new GHGamepadPressRouter();
            Route(router, Ev(GHGamepadButton.B, GHGamepadButtonEventKind.Pressed, 5), GamePage, false);
            router.ObserveTarget(GamePage, true);
            router.ObserveTarget(GamePage, false);

            Assert.Equal(GHGamepadRouteAction.DeliverCancelled, Route(router, Ev(GHGamepadButton.B, GHGamepadButtonEventKind.Released, 5), GamePage, false));
            Assert.Equal(GHGamepadRouteAction.Drop, Route(router, Ev(GHGamepadButton.B, GHGamepadButtonEventKind.ShortPressed, 5), GamePage, false));
        }

        [Fact]
        public void TrackerCancelledRelease_GoesToTheCapturingTarget()
        {
            GHGamepadPressRouter router = new GHGamepadPressRouter();
            Route(router, Ev(GHGamepadButton.X, GHGamepadButtonEventKind.Pressed, 6), GamePage, false);
            object captured;
            bool capturedPopup;
            Assert.Equal(GHGamepadRouteAction.DeliverCancelled,
                router.Route(Ev(GHGamepadButton.X, GHGamepadButtonEventKind.Released, 6, true), GamePage, false, out captured, out capturedPopup));
            Assert.Same(GamePage, captured);
        }

        [Fact]
        public void NoTarget_DropsThePressAndItsEvents()
        {
            GHGamepadPressRouter router = new GHGamepadPressRouter();
            Assert.Equal(GHGamepadRouteAction.Drop, Route(router, Ev(GHGamepadButton.A, GHGamepadButtonEventKind.Pressed, 8), null, false));
            Assert.Equal(GHGamepadRouteAction.Drop, Route(router, Ev(GHGamepadButton.A, GHGamepadButtonEventKind.Released, 8), GamePage, false));
            Assert.Equal(GHGamepadRouteAction.Drop, Route(router, Ev(GHGamepadButton.A, GHGamepadButtonEventKind.ShortPressed, 8), GamePage, false));
        }

        [Fact]
        public void EventsOfAnotherPress_AreDropped()
        {
            GHGamepadPressRouter router = new GHGamepadPressRouter();
            Route(router, Ev(GHGamepadButton.A, GHGamepadButtonEventKind.Pressed, 9), GamePage, false);
            Assert.Equal(GHGamepadRouteAction.Drop, Route(router, Ev(GHGamepadButton.A, GHGamepadButtonEventKind.Released, 8), GamePage, false));
        }

        [Fact]
        public void LongPressThenRelease_SameTarget()
        {
            GHGamepadPressRouter router = new GHGamepadPressRouter();
            Route(router, Ev(GHGamepadButton.View, GHGamepadButtonEventKind.Pressed, 10), GamePage, false);
            Assert.Equal(GHGamepadRouteAction.Deliver, Route(router, Ev(GHGamepadButton.View, GHGamepadButtonEventKind.LongPressed, 10), GamePage, false));
            Assert.Equal(GHGamepadRouteAction.Deliver, Route(router, Ev(GHGamepadButton.View, GHGamepadButtonEventKind.Released, 10), GamePage, false));
        }
    }
}
