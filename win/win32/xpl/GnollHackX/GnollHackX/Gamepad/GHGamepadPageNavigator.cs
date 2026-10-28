using System;
using System.Collections.Generic;
using System.Diagnostics;
#if GNH_MAUI
using Microsoft.Maui.Controls;
using GnollHackX;
using Switch = Microsoft.Maui.Controls.Switch;
#else
using Xamarin.Forms;
using Switch = Xamarin.Forms.Switch;
#endif

#if GNH_MAUI
namespace GnollHackM
#else
namespace GnollHackX.Controls
#endif
{
    /* Gamepad navigation for a page of buttons and toggles: a main layer and overlay layers (in-page popups and
       panels) that take over while shown, the first shown overlay in AddOverlay order winning. Up/Down (D-pad and
       left stick) move with wrap on a fresh press only, Left/Right too where a layer allows it, A activates on a
       short press that started here, B runs the layer's back action, and the press that reveals the UI only shows
       the focus.
       An overlay gets its initial focus each time it becomes active; the main layer keeps its focus.
       UI thread only. */
    public sealed class GHGamepadPageNavigator
    {
        /* One focus list with its scrolling and back behavior */
        public sealed class Layer
        {
            private readonly GHGamepadFocusList _focus = new GHGamepadFocusList();
            private readonly List<View> _views = new List<View>();
            private readonly List<View> _initialViews = new List<View>();
            private readonly Func<bool> _isShown;

            internal Layer(Func<bool> isShown)
            {
                _isShown = isShown;
            }

            public GHGamepadFocusList Focus { get { return _focus; } }

            /* B Pressed; null does nothing */
            public Func<bool> Back { get; set; }

            /* Scrolled to keep the focused view visible when the view is inside it */
            public ScrollView ScrollView { get; set; }

            /* Left/Right move the focus as well */
            public bool HorizontalMoves { get; set; }

            /* Up/Down that cannot move the focus scroll ScrollView by a line, and LB/RB by a page */
            public bool ScrollsContent { get; set; }

            /* The first GridCount entries form a grid of GridColumns() columns, row by row, for Up/Down */
            public int GridCount { get; set; }
            public Func<int> GridColumns { get; set; }

            public int Count { get { return _views.Count; } }

            /* isAvailable null: the view and its ancestors are visible, enabled and not transparent */
            public void Add(View view, Action activate, Func<bool> isAvailable)
            {
                Add(view, activate, isAvailable, null);
            }

            /* highlight: tinted while focused unless view is an image button; null tints view itself */
            public void Add(View view, Action activate, Func<bool> isAvailable, View highlight)
            {
                if (view == null)
                    return;
                if (isAvailable == null)
                    isAvailable = () => IsViewAvailable(view);
                _views.Add(view);
                _focus.Add(view, activate, isAvailable, highlight);
            }

            public void AddButton(CustomImageButton button)
            {
                if (button != null)
                    Add(button, button.PerformClick, null);
            }

            /* A flips the switch, which raises Toggled as a tap does; highlight is typically the row with its label */
            public void AddSwitch(Switch toggle, View highlight)
            {
                if (toggle != null)
                    Add(toggle, () => toggle.IsToggled = !toggle.IsToggled, null, highlight);
            }

            /* A flips the check box, which raises CheckedChanged as a tap does */
            public void AddCheckBox(CheckBox checkBox, View highlight)
            {
                if (checkBox != null)
                    Add(checkBox, () => checkBox.IsChecked = !checkBox.IsChecked, null, highlight);
            }

            /* Every CustomImageButton under root in visual-tree order; visibility is checked when focusing */
            public void AddButtons(Element root)
            {
                List<CustomImageButton> buttons = new List<CustomImageButton>();
                CollectButtons(root, buttons);
                foreach (CustomImageButton button in buttons)
                    AddButton(button);
            }

            /* The focus on activation: the first available of views, else the first available entry */
            public void SetInitialFocus(params View[] views)
            {
                _initialViews.Clear();
                if (views != null)
                    _initialViews.AddRange(views);
            }

            public bool FocusInitial()
            {
                foreach (View view in _initialViews)
                {
                    int index = _views.IndexOf(view);
                    if (index >= 0 && _focus.IsAvailableAt(index))
                        return _focus.SetFocus(index);
                }
                return _focus.SetFocus(0);
            }

            internal bool IsShown()
            {
                if (_isShown == null)
                    return true;
                try
                {
                    return _isShown();
                }
                catch (Exception ex)
                {
                    Debug.WriteLine(ex);
                    return false;
                }
            }

            internal bool MoveVertical(int dir, bool allowWrap)
            {
                int columns = 1;
                if (GridColumns != null)
                {
                    try
                    {
                        columns = GridColumns();
                    }
                    catch (Exception ex)
                    {
                        Debug.WriteLine(ex);
                    }
                }
                int count = _views.Count;
                int gridCount = Math.Min(GridCount, count);
                int index = _focus.FocusIndex;
                if (columns <= 1 || index < 0 || index >= gridCount)
                    return _focus.Move(dir, allowWrap);

                int row = index / columns;
                int lastRow = (gridCount - 1) / columns;
                if (dir < 0)
                {
                    if (row > 0)
                        return _focus.SetFocus(index - columns);
                    if (!allowWrap)
                        return false;
                    /* The first row wraps to the last entry, or to the same column on the last row */
                    if (count > gridCount)
                        return _focus.SetFocus(count - 1);
                    int target = lastRow * columns + index % columns;
                    return _focus.SetFocus(Math.Min(target, gridCount - 1));
                }
                if (row < lastRow)
                    return _focus.SetFocus(Math.Min(index + columns, gridCount - 1));
                /* The last row continues to the entries after the grid, or wraps to the top of the column */
                if (count > gridCount)
                    return _focus.SetFocus(gridCount);
                if (!allowWrap)
                    return false;
                return _focus.SetFocus(index % columns);
            }
        }

        private const double LineScrollStep = 48.0;
        private const double PageScrollFraction = 0.85;

        private readonly Layer _main = new Layer(null);
        private readonly List<Layer> _overlays = new List<Layer>();
        private Layer _active;
        private bool _attached = false;
        private bool _aArmed = false;
        private int _aPressId = 0;

        public GHGamepadPageNavigator()
        {
            _active = _main;
        }

        public Layer Main { get { return _main; } }

        /* While true, every button is ignored (wait overlays) */
        public Func<bool> IsBusy { get; set; }

        /* Overlays added earlier take precedence when several are shown */
        public Layer AddOverlay(Func<bool> isShown)
        {
            Layer layer = new Layer(isShown);
            _overlays.Add(layer);
            return layer;
        }

        /* From the page's Appearing */
        public void Attach()
        {
            if (!_attached)
            {
                GHGamepadService.StateChanged += GamepadService_StateChanged;
                _attached = true;
            }
            _aArmed = false;
            RefreshVisuals();
        }

        /* From the page's Disappearing */
        public void Detach()
        {
            if (_attached)
            {
                GHGamepadService.StateChanged -= GamepadService_StateChanged;
                _attached = false;
            }
            _aArmed = false;
        }

        public void RefreshVisuals()
        {
            try
            {
                Layer layer = UpdateActiveLayer();
                if (layer.Focus.FocusIndex < 0)
                    layer.FocusInitial();
                layer.Focus.RefreshVisuals();
                ScrollFocusIntoView(layer);
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex);
            }
        }

        private void GamepadService_StateChanged()
        {
            RefreshVisuals();
        }

        /* Always consumes the event */
        public bool HandleButton(GHGamepadButtonEvent e)
        {
            if (GHApp.PushingModalPage || GHApp.IsSystemBrowserOpen) /* Ignore input when opening a page or using a system browser */
                return true;

            try
            {
                if (IsBusy != null && IsBusy())
                {
                    _aArmed = false;
                    return true;
                }

                Layer layer = UpdateActiveLayer();
                if (e.RevealsUi)
                {
                    if (e.Button == GHGamepadButton.A)
                        _aArmed = false;
                    EnsureFocus(layer);
                    layer.Focus.RefreshVisuals();
                    ScrollFocusIntoView(layer);
                    return true;
                }

                switch (e.Button)
                {
                    case GHGamepadButton.DPadUp:
                    case GHGamepadButton.LeftStickUp:
                        MoveFocus(layer, -1, true, e.Kind);
                        break;
                    case GHGamepadButton.DPadDown:
                    case GHGamepadButton.LeftStickDown:
                        MoveFocus(layer, 1, true, e.Kind);
                        break;
                    case GHGamepadButton.DPadLeft:
                    case GHGamepadButton.LeftStickLeft:
                        if (layer.HorizontalMoves)
                            MoveFocus(layer, -1, false, e.Kind);
                        break;
                    case GHGamepadButton.DPadRight:
                    case GHGamepadButton.LeftStickRight:
                        if (layer.HorizontalMoves)
                            MoveFocus(layer, 1, false, e.Kind);
                        break;
                    case GHGamepadButton.LB:
                    case GHGamepadButton.RB:
                        if (layer.ScrollsContent && layer.ScrollView != null
                            && (e.Kind == GHGamepadButtonEventKind.Pressed || e.Kind == GHGamepadButtonEventKind.Repeat))
                        {
                            double page = Math.Max(LineScrollStep, layer.ScrollView.Height * PageScrollFraction);
                            ScrollContentBy(layer, e.Button == GHGamepadButton.LB ? -page : page);
                        }
                        break;
                    case GHGamepadButton.A:
                        if (e.Kind == GHGamepadButtonEventKind.Pressed)
                        {
                            /* A press that only restores the focus does not activate */
                            _aArmed = EnsureFocus(layer);
                            _aPressId = e.PressId;
                            if (!_aArmed)
                            {
                                layer.Focus.RefreshVisuals();
                                ScrollFocusIntoView(layer);
                            }
                        }
                        else if (e.Kind == GHGamepadButtonEventKind.Released)
                        {
                            if (e.IsCancelled && e.PressId == _aPressId)
                                _aArmed = false;
                        }
                        else if (e.Kind == GHGamepadButtonEventKind.ShortPressed)
                        {
                            if (_aArmed && e.PressId == _aPressId)
                            {
                                _aArmed = false;
                                layer.Focus.Activate();
                            }
                        }
                        break;
                    case GHGamepadButton.B:
                        if (e.Kind == GHGamepadButtonEventKind.Pressed && layer.Back != null)
                            layer.Back();
                        break;
                    default:
                        break;
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex);
            }
            return true;
        }

        /* Wraps only on a fresh press, so a held direction stops at the end */
        private void MoveFocus(Layer layer, int dir, bool vertical, GHGamepadButtonEventKind kind)
        {
            if (kind != GHGamepadButtonEventKind.Pressed && kind != GHGamepadButtonEventKind.Repeat)
                return;
            bool fresh = kind == GHGamepadButtonEventKind.Pressed;
            if (!EnsureFocus(layer))
            {
                layer.Focus.RefreshVisuals();
                ScrollFocusIntoView(layer);
                return;
            }
            bool moved = vertical ? layer.MoveVertical(dir, fresh) : layer.Focus.Move(dir, fresh);
            if (moved)
                ScrollFocusIntoView(layer);
            else if (vertical && layer.ScrollsContent)
                ScrollContentBy(layer, dir * LineScrollStep);
        }

        /* Returns true if the layer already had an available focus; otherwise tries its initial focus */
        private static bool EnsureFocus(Layer layer)
        {
            if (layer.Focus.IsAvailableAt(layer.Focus.FocusIndex))
                return true;
            layer.FocusInitial();
            return false;
        }

        private Layer UpdateActiveLayer()
        {
            Layer layer = _main;
            foreach (Layer overlay in _overlays)
            {
                if (overlay.IsShown())
                {
                    layer = overlay;
                    break;
                }
            }
            if (!ReferenceEquals(layer, _active))
            {
                _active.Focus.ClearVisuals();
                _active = layer;
                _aArmed = false;
                if (!ReferenceEquals(layer, _main))
                    layer.FocusInitial();
                layer.Focus.RefreshVisuals();
                ScrollFocusIntoView(layer);
            }
            return layer;
        }

        private static void ScrollFocusIntoView(Layer layer)
        {
            ScrollView scrollView = layer.ScrollView;
            if (scrollView == null || !GHGamepadService.IsActive)
                return;
            View focused = layer.Focus.FocusedView;
            if (focused == null || !focused.IsVisible || !IsDescendantOf(focused, scrollView))
                return;
            try
            {
                _ = scrollView.ScrollToAsync(focused, ScrollToPosition.MakeVisible, false);
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex);
            }
        }

        private static void ScrollContentBy(Layer layer, double delta)
        {
            ScrollView scrollView = layer.ScrollView;
            if (scrollView == null)
                return;
            try
            {
                double maxY = Math.Max(0.0, scrollView.ContentSize.Height - scrollView.Height);
                double y = GHGamepadMath.Clamp(scrollView.ScrollY + delta, 0.0, maxY);
                if (y != scrollView.ScrollY)
                    _ = scrollView.ScrollToAsync(scrollView.ScrollX, y, false);
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex);
            }
        }

        private static bool IsDescendantOf(Element element, Element ancestor)
        {
            Element current = element != null ? element.Parent : null;
            while (current != null)
            {
                if (ReferenceEquals(current, ancestor))
                    return true;
                current = current.Parent;
            }
            return false;
        }

        /* The view and every ancestor up to its page are visible, enabled and not fully transparent */
        public static bool IsViewAvailable(View view)
        {
            Element element = view;
            while (element != null && !(element is Page))
            {
                VisualElement visual = element as VisualElement;
                if (visual != null && (!visual.IsVisible || !visual.IsEnabled || visual.Opacity <= 0.0))
                    return false;
                element = element.Parent;
            }
            return element != null;
        }

        private static void CollectButtons(Element element, List<CustomImageButton> result)
        {
            if (element == null)
                return;
            CustomImageButton button = element as CustomImageButton;
            if (button != null)
            {
                result.Add(button);
                return;
            }
#if GNH_MAUI
            Microsoft.Maui.IVisualTreeElement tree = element as Microsoft.Maui.IVisualTreeElement;
            if (tree == null)
                return;
            foreach (Microsoft.Maui.IVisualTreeElement child in tree.GetVisualChildren())
                CollectButtons(child as Element, result);
#else
            foreach (Element child in ((IElementController)element).LogicalChildren)
                CollectButtons(child, result);
#endif
        }
    }
}
