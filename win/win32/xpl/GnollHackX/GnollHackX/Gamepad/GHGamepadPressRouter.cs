using System;

namespace GnollHackX
{
    public enum GHGamepadRouteAction
    {
        Drop,
        Deliver,            /* to the current target */
        DeliverCancelled    /* a cancelled Released to the target that captured the press */
    }

    /* Press capture: a press belongs to the target (top page, and whether its popup was open) that received
       its Pressed. While that target is still current, the press's later events go to it. When the target
       changed, its Released goes to the capturing target as cancelled, and ShortPressed, LongPressed and
       Repeat are dropped, so a release never activates something that appeared during the press. A null
       target receives nothing. */
    public sealed class GHGamepadPressRouter
    {
        private struct Capture
        {
            public bool Active;
            public int PressId;
            public object Target;
            public bool PopupOpen;
            public bool Released;
            public bool ReleaseCancelled;
            public bool TargetChanged;
        }

        private readonly Capture[] _captures = new Capture[(int)GHGamepadButton.Count];

        /* capturedTarget and capturedPopupOpen are set for DeliverCancelled */
        public GHGamepadRouteAction Route(GHGamepadButtonEvent e, object target, bool popupOpen,
            out object capturedTarget, out bool capturedPopupOpen)
        {
            capturedTarget = null;
            capturedPopupOpen = false;
            if (e.Button < 0 || e.Button >= GHGamepadButton.Count)
                return GHGamepadRouteAction.Drop;
            int i = (int)e.Button;

            if (e.Kind == GHGamepadButtonEventKind.Pressed)
            {
                if (target == null)
                {
                    _captures[i] = new Capture();
                    return GHGamepadRouteAction.Drop;
                }
                Capture c = new Capture();
                c.Active = true;
                c.PressId = e.PressId;
                c.Target = target;
                c.PopupOpen = popupOpen;
                _captures[i] = c;
                return GHGamepadRouteAction.Deliver;
            }

            if (!_captures[i].Active || _captures[i].PressId != e.PressId)
                return GHGamepadRouteAction.Drop;

            bool sameTarget = !_captures[i].TargetChanged && IsSameTarget(_captures[i], target, popupOpen);
            switch (e.Kind)
            {
            case GHGamepadButtonEventKind.Released:
                if (_captures[i].Released)
                    return GHGamepadRouteAction.Drop;
                _captures[i].Released = true;
                if (sameTarget && !e.IsCancelled)
                    return GHGamepadRouteAction.Deliver;
                _captures[i].ReleaseCancelled = true;
                capturedTarget = _captures[i].Target;
                capturedPopupOpen = _captures[i].PopupOpen;
                return GHGamepadRouteAction.DeliverCancelled;
            case GHGamepadButtonEventKind.ShortPressed:
                bool deliver = sameTarget && _captures[i].Released && !_captures[i].ReleaseCancelled;
                _captures[i] = new Capture();
                return deliver ? GHGamepadRouteAction.Deliver : GHGamepadRouteAction.Drop;
            default:
                if (_captures[i].Released || !sameTarget)
                    return GHGamepadRouteAction.Drop;
                return GHGamepadRouteAction.Deliver;
            }
        }

        /* Called once per Poll with the current target, so that a target that changes and changes back during a
           press still cancels it */
        public void ObserveTarget(object target, bool popupOpen)
        {
            int i;
            for (i = 0; i < _captures.Length; i++)
            {
                if (_captures[i].Active && !_captures[i].Released && !IsSameTarget(_captures[i], target, popupOpen))
                    _captures[i].TargetChanged = true;
            }
        }

        private static bool IsSameTarget(Capture capture, object target, bool popupOpen)
        {
            return target != null && ReferenceEquals(capture.Target, target) && capture.PopupOpen == popupOpen;
        }

        /* Forgets every capture */
        public void Reset()
        {
            Array.Clear(_captures, 0, _captures.Length);
        }
    }
}
