#if GNH_AVALONIA
using System;
using System.Collections.Generic;
using Avalonia.Input;
using Avalonia.Interactivity;
using Avalonia.VisualTree;
using AvVisual = Avalonia.Visual;
using AvWindow = Avalonia.Controls.Window;

namespace GnollHackM
{
    /* Keeps keyboard focus on controls a pointer could reach. Avalonia's Tab and directional navigation accept any
       focusable, enabled and visible control, also one painted over by a modal page or an opaque overlay, and the
       focus adorner is drawn above everything. A Tab or directional focus change to a covered control is redirected
       to the next candidate that is not covered, or cancelled when there is none. A modal push or pop clears a focus
       outside the top-most page. A key pressed while a covered control holds focus clears the focus; when the game
       declined the key and was offered it on KeyDown, the key is dropped as well. Covered means that a hit test at the
       center of the control's clipped bounds lands in a sibling branch. UI thread only.
       WORKAROUND(WA-AVALONIA-0020): ACM's modal presentation leaves the page below focusable. */
    internal static class GHAvaloniaFocusGuard
    {
        /* Bound on the redirect walk; the visited set stops it earlier when the walk wraps around */
        private const int MaxRedirectSteps = 64;

        private static AvWindow _window;

        /* Direction of the last Tab or arrow KeyDown, which a Directional focus change follows */
        private static NavigationDirection? _lastDirection;

        public static void Attach(AvWindow window)
        {
            if (window == null)
                return;
            _window = window;
            window.AddHandler(InputElement.GettingFocusEvent, OnGettingFocus, RoutingStrategies.Bubble, true);
            window.AddHandler(InputElement.KeyDownEvent, OnKeyDown, RoutingStrategies.Tunnel, true);
        }

        public static void Detach(AvWindow window)
        {
            if (window == null)
                return;
            window.RemoveHandler(InputElement.GettingFocusEvent, OnGettingFocus);
            window.RemoveHandler(InputElement.KeyDownEvent, OnKeyDown);
            if (ReferenceEquals(_window, window))
                _window = null;
        }

        public static void OnModalPushed(object sender, Microsoft.Maui.Controls.ModalPushedEventArgs e)
        {
            ClearFocusOutsideTopPage(sender as Microsoft.Maui.Controls.Window, e?.Modal, null);
        }

        public static void OnModalPopped(object sender, Microsoft.Maui.Controls.ModalPoppedEventArgs e)
        {
            ClearFocusOutsideTopPage(sender as Microsoft.Maui.Controls.Window, null, e?.Modal);
        }

        /* After the layout that follows a modal push or pop, a focus outside the top-most page's platform view is
           cleared. The page tree decides rather than a hit test: hit testing reads the renderer's last drawn frame,
           which does not yet contain the overlay the push added. */
        private static void ClearFocusOutsideTopPage(Microsoft.Maui.Controls.Window mauiWindow, Microsoft.Maui.Controls.Page pushed, Microsoft.Maui.Controls.Page popped)
        {
            if (mauiWindow == null || _window == null)
                return;
            Avalonia.Threading.Dispatcher.UIThread.Post(() =>
            {
                IFocusManager fm = _window?.FocusManager;
                if (fm == null)
                    return;
                AvVisual focused = fm.GetFocusedElement() as AvVisual;
                if (focused == null)
                    return;
                AvVisual top = GetTopPageVisual(mauiWindow, pushed, popped);
                if (top != null && !top.IsVisualAncestorOf(focused))
                    fm.Focus(null);
            }, Avalonia.Threading.DispatcherPriority.Loaded);
        }

        /* The platform view of the top-most modal page other than a popped one, or of the window's page */
        private static AvVisual GetTopPageVisual(Microsoft.Maui.Controls.Window mauiWindow, Microsoft.Maui.Controls.Page pushed, Microsoft.Maui.Controls.Page popped)
        {
            Microsoft.Maui.Controls.Page top = null;
            IReadOnlyList<Microsoft.Maui.Controls.Page> stack = mauiWindow.Navigation?.ModalStack;
            if (stack != null)
            {
                int i;
                for (i = stack.Count - 1; i >= 0 && top == null; i--)
                {
                    if (!ReferenceEquals(stack[i], popped))
                        top = stack[i];
                }
            }
            if (top == null)
                top = pushed ?? mauiWindow.Page;
            return top?.Handler?.PlatformView as AvVisual;
        }

        /* Runs after GHAvaloniaKeyboard's tunnel handler, so e.Handled and KeyDownRouted say what the game did */
        private static void OnKeyDown(object sender, KeyEventArgs e)
        {
            NavigationDirection? direction = GetNavigationDirection(e);
            _lastDirection = direction;

            AvWindow window = _window;
            IFocusManager fm = window?.FocusManager;
            if (fm == null)
                return;
            AvVisual focused = fm.GetFocusedElement() as AvVisual;
            if (focused == null || !IsCovered(window, focused))
                return;

            /* The player cannot see the focused control, so it gives focus up. A declined Tab or arrow then navigates
               from nothing and OnGettingFocus steers it. A key the game was offered on KeyDown is complete on KeyDown
               and is dropped, so a covered Button never clicks on Space or Enter; a character key is left to continue,
               since a handled KeyDown suppresses the TextInput the game reads the character from. */
            fm.Focus(null);
            if (!e.Handled && direction == null && GHAvaloniaKeyboard.KeyDownRouted)
                e.Handled = true;
        }

        private static NavigationDirection? GetNavigationDirection(KeyEventArgs e)
        {
            switch (e.Key)
            {
            case Key.Tab:
                return (e.KeyModifiers & KeyModifiers.Shift) != 0 ? NavigationDirection.Previous : NavigationDirection.Next;
            case Key.Left:
                return NavigationDirection.Left;
            case Key.Right:
                return NavigationDirection.Right;
            case Key.Up:
                return NavigationDirection.Up;
            case Key.Down:
                return NavigationDirection.Down;
            default:
                return null;
            }
        }

        /* Pointer and programmatic focus changes are never vetoed */
        private static void OnGettingFocus(object sender, FocusChangingEventArgs e)
        {
            if (e.NavigationMethod != NavigationMethod.Tab && e.NavigationMethod != NavigationMethod.Directional)
                return;

            AvWindow window = _window;
            AvVisual candidate = e.NewFocusedElement as AvVisual;
            if (window == null || candidate == null || !IsCovered(window, candidate))
                return;

            NavigationDirection direction;
            bool arrow = false;
            if (e.NavigationMethod == NavigationMethod.Tab)
            {
                direction = (e.KeyModifiers & KeyModifiers.Shift) != 0 ? NavigationDirection.Previous : NavigationDirection.Next;
            }
            else
            {
                NavigationDirection? last = _lastDirection;
                arrow = last == NavigationDirection.Left || last == NavigationDirection.Right
                    || last == NavigationDirection.Up || last == NavigationDirection.Down;
                direction = arrow ? last.Value : NavigationDirection.Next;
            }

            IFocusManager fm = Avalonia.Controls.TopLevel.GetTopLevel(candidate)?.FocusManager ?? window.FocusManager;
            if (fm == null)
                return;

            IInputElement next = FindUncoveredElement(fm, window, e.NewFocusedElement, direction);
            if (next == null && arrow)
                next = FindUncoveredElement(fm, window, e.NewFocusedElement, NavigationDirection.Next);

            if (next != null)
                e.TrySetNewFocusedElement(next);
            else
                e.TryCancel();
        }

        /* The focus manager's own walk from start in the given direction, until a candidate is not covered. Next and
           Previous follow tab order and wrap at the window root, which the visited set turns into a stop; the arrow
           directions are the geometric search, which no device type restricts here. */
        private static IInputElement FindUncoveredElement(IFocusManager fm, AvWindow window, IInputElement start, NavigationDirection direction)
        {
            HashSet<IInputElement> visited = new HashSet<IInputElement>();
            visited.Add(start);
            IInputElement current = start;
            int i;
            for (i = 0; i < MaxRedirectSteps; i++)
            {
                IInputElement next = fm.FindNextElement(direction, new FindNextElementOptions { FocusedElement = current });
                if (next == null || !visited.Add(next))
                    return null;
                AvVisual nextVisual = next as AvVisual;
                if (nextVisual != null && !IsCovered(window, nextVisual))
                    return next;
                current = next;
            }
            return null;
        }

        /* True when a hit test at the center of the control's clipped bounds lands in a sibling branch, which is what a
           pointer there would reach instead of the control. A control clipped away, scrolled out of a ScrollViewer for
           instance, is not covered: Avalonia scrolls it into view on focus. */
        private static bool IsCovered(AvWindow window, AvVisual visual)
        {
            if (window == null || visual == null || !visual.IsAttachedToVisualTree())
                return false;

            TransformedBounds? bounds = visual.GetTransformedBounds();
            if (bounds == null)
                return false;
            Avalonia.Rect clip = bounds.Value.Clip;
            if (clip.Width <= 0 || clip.Height <= 0)
                return false;

            AvVisual hit = window.InputHitTest(clip.Center) as AvVisual;
            if (hit == null || ReferenceEquals(hit, visual))
                return false;
            if (visual.IsVisualAncestorOf(hit) || hit.IsVisualAncestorOf(visual))
                return false;
            return true;
        }
    }
}
#endif
