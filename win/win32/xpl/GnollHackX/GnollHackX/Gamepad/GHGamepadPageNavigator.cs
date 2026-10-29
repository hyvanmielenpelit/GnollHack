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
       the focus. A focus scrolled wholly out of the layer's ScrollView moves to the top visible entry on Up/Down
       and when the UI is revealed, as an unset focus does on Up/Down, instead of scrolling back to it.
       Entries besides buttons, switches and check boxes: pickers and sliders, which Left/Right adjust; text
       entries, which A focuses with the on-screen keyboard and B leaves; row image buttons; and a CollectionView
       as one entry whose Up/Down move its selection. X runs an entry's secondary action, armed like A. Section
       stops make LB/RB jump between sections; without them LB/RB page ScrollsContent layers. LT/RT go to the first
       and last entry inside the layer's ScrollView (of the whole layer when none is inside).
       The right stick scrolls the shown layer's ScrollView in proportion to its push, through UpdateSticks.
       An overlay gets its initial focus each time it becomes active; the main layer keeps its focus.
       UI thread only. */
    public sealed class GHGamepadPageNavigator
    {
        /* One focus list with its scrolling and back behavior */
        public sealed class Layer
        {
            /* The first entry of a section and the heading scrolled to the top on a jump, if any */
            private sealed class SectionStop
            {
                public int Index;
                public View Heading;
            }

            private readonly GHGamepadFocusList _focus = new GHGamepadFocusList();
            private readonly List<View> _views = new List<View>();
            /* Per entry, its highlight, else the view; its bounds decide whether the entry is scrolled into view */
            private readonly List<View> _highlights = new List<View>();
            private readonly List<View> _initialViews = new List<View>();
            private readonly List<Entry> _textEntries = new List<Entry>();
            private readonly List<SectionStop> _stops = new List<SectionStop>();
            private bool _pendingStop = false;
            private View _pendingStopHeading;
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

            /* Up/Down that cannot move the focus scroll ScrollView by a line, and LB/RB by a page unless the layer
               has section stops */
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
                Add(view, activate, isAvailable, highlight, null);
            }

            /* options: Left/Right, X, leave and enter behavior; null for none */
            public void Add(View view, Action activate, Func<bool> isAvailable, View highlight,
                GHGamepadFocusList.EntryOptions options)
            {
                if (view == null)
                    return;
                if (isAvailable == null)
                    isAvailable = () => IsViewAvailable(view);
                _views.Add(view);
                _highlights.Add(highlight ?? view);
                _focus.Add(view, activate, isAvailable, highlight, options);
                if (_pendingStop)
                {
                    _stops.Add(new SectionStop { Index = _views.Count - 1, Heading = _pendingStopHeading });
                    _pendingStop = false;
                    _pendingStopHeading = null;
                }
            }

            public void AddButton(CustomImageButton button)
            {
                if (button != null)
                    Add(button, button.PerformClick, null);
            }

            public void AddRowButton(RowImageButton button)
            {
                if (button != null)
                    Add(button, button.PerformClick, null);
            }

            /* A flips the switch, which raises Toggled as a tap does; highlight is typically the row with its label */
            public void AddSwitch(Switch toggle, View highlight, Action secondary = null)
            {
                if (toggle != null)
                    Add(toggle, () => toggle.IsToggled = !toggle.IsToggled, null, highlight,
                        SecondaryOptions(secondary));
            }

            /* A flips the check box, which raises CheckedChanged as a tap does */
            public void AddCheckBox(CheckBox checkBox, View highlight, Action secondary = null)
            {
                if (checkBox != null)
                    Add(checkBox, () => checkBox.IsChecked = !checkBox.IsChecked, null, highlight,
                        SecondaryOptions(secondary));
            }

            /* A selects the next item, wrapping; Left/Right the previous or next, stopping at the ends. Items is
               filled from ItemsSource too. The dropdown is never opened. */
            public void AddPicker(Picker picker, View highlight, Action secondary = null)
            {
                if (picker == null)
                    return;
                GHGamepadFocusList.EntryOptions options = new GHGamepadFocusList.EntryOptions();
                options.Secondary = secondary;
                options.Adjust = (dir) =>
                {
                    int count = picker.Items.Count;
                    if (count == 0)
                        return;
                    int index = Math.Max(0, Math.Min(count - 1, picker.SelectedIndex + dir));
                    if (index != picker.SelectedIndex)
                        picker.SelectedIndex = index;
                };
                Add(picker, () =>
                {
                    int count = picker.Items.Count;
                    if (count == 0)
                        return;
                    int index = picker.SelectedIndex + 1;
                    picker.SelectedIndex = index >= 0 && index < count ? index : 0;
                }, null, highlight, options);
            }

            /* Left/Right change the value by stepFraction of the range, clamped; A does nothing */
            public void AddSlider(Slider slider, View highlight, double stepFraction, Action secondary = null)
            {
                if (slider == null)
                    return;
                GHGamepadFocusList.EntryOptions options = new GHGamepadFocusList.EntryOptions();
                options.Secondary = secondary;
                options.Adjust = (dir) =>
                {
                    double min = slider.Minimum;
                    double max = slider.Maximum;
                    if (max <= min)
                        return;
                    double value = GHGamepadMath.Clamp(slider.Value + dir * stepFraction * (max - min), min, max);
                    if (value != slider.Value)
                        slider.Value = value;
                };
                Add(slider, null, null, highlight, options);
            }

            /* A focuses the field and opens the on-screen keyboard; B while the field has the keyboard focus, or
               moving away, unfocuses it and closes the keyboard */
            public void AddEntry(Entry entry, View highlight, GHScreenKeyboardMode mode, Action secondary = null)
            {
                if (entry == null)
                    return;
                GHGamepadFocusList.EntryOptions options = new GHGamepadFocusList.EntryOptions();
                options.Secondary = secondary;
                options.Leave = () => EndTextEdit(entry);
                entry.Completed -= TextEntry_Completed;
                entry.Completed += TextEntry_Completed;
                entry.Unfocused -= TextEntry_Unfocused;
                entry.Unfocused += TextEntry_Unfocused;
                if (!_textEntries.Contains(entry))
                    _textEntries.Add(entry);
                Add(entry, () =>
                {
                    entry.Focus();
                    GHScreenKeyboard.Show(entry, mode);
                }, null, highlight, options);
            }

            /* One entry for the list: Up/Down move SelectedItem and leave past the first or last item, Left/Right
               do nothing, A runs activate on the selected item. The list shows the selection; nothing is tinted. Needs
               SelectionMode Single for the selection to show; available while items is not empty. */
            public void AddCollectionView(CollectionView list, Func<System.Collections.IList> items,
                Action<object> activate)
            {
                if (list == null || items == null)
                    return;
                CollectionCursor cursor = new CollectionCursor(list, items);
                GHGamepadFocusList.EntryOptions options = new GHGamepadFocusList.EntryOptions();
                options.Enter = cursor.Enter;
                options.MoveWithin = cursor.Move;
                options.Adjust = d => { };
                options.NoHighlight = true;
                Add(list, () =>
                {
                    object item = cursor.Current;
                    if (item != null && activate != null)
                        activate(item);
                }, () => IsViewAvailable(list) && cursor.ItemCount > 0, null, options);
            }

            /* Every CustomImageButton under root in visual-tree order; visibility is checked when focusing */
            public void AddButtons(Element root)
            {
                List<CustomImageButton> buttons = new List<CustomImageButton>();
                CollectViews(root, buttons);
                foreach (CustomImageButton button in buttons)
                    AddButton(button);
            }

            /* Every RowImageButton under root in visual-tree order */
            public void AddRowButtons(Element root)
            {
                List<RowImageButton> buttons = new List<RowImageButton>();
                CollectViews(root, buttons);
                foreach (RowImageButton button in buttons)
                    AddRowButton(button);
            }

            /* The next added entry starts a section, for LB/RB */
            public void AddSectionStop()
            {
                AddSectionStop(null);
            }

            /* heading: scrolled to the top of ScrollView when LB/RB jump to the section */
            public void AddSectionStop(View heading)
            {
                _pendingStop = true;
                _pendingStopHeading = heading;
            }

            public bool HasSectionStops { get { return _stops.Count > 0; } }

            /* Removes every entry and section stop, keeping Back, ScrollView, the moves and the initial focus
               rule; the focused view keeps the focus if it is added again */
            public void Clear()
            {
                _focus.Clear();
                _views.Clear();
                _highlights.Clear();
                foreach (Entry entry in _textEntries)
                {
                    entry.Completed -= TextEntry_Completed;
                    entry.Unfocused -= TextEntry_Unfocused;
                }
                _textEntries.Clear();
                _stops.Clear();
                _pendingStop = false;
                _pendingStopHeading = null;
            }

            /* B while a text field of the layer has the keyboard focus: leaves the field; true if one had it */
            internal bool EndFocusedTextEdit()
            {
                foreach (Entry entry in _textEntries)
                {
                    if (entry.IsFocused)
                    {
                        EndTextEdit(entry);
                        return true;
                    }
                }
                return false;
            }

            /* Hides first: Unfocused marks the keyboard closed without hiding it */
            private static void EndTextEdit(Entry entry)
            {
                if (!entry.IsFocused)
                    return;
                GHScreenKeyboard.Hide();
                entry.Unfocus();
            }

            /* Enter closes a single-line keyboard by itself */
            private static void TextEntry_Completed(object sender, EventArgs e)
            {
                GHScreenKeyboard.MarkClosed();
            }

            /* Only marks the keyboard closed, so that a focus change while the keyboard opens cannot close it */
            private static void TextEntry_Unfocused(object sender, FocusEventArgs e)
            {
                GHScreenKeyboard.MarkClosed();
            }

            private static GHGamepadFocusList.EntryOptions SecondaryOptions(Action secondary)
            {
                if (secondary == null)
                    return null;
                GHGamepadFocusList.EntryOptions options = new GHGamepadFocusList.EntryOptions();
                options.Secondary = secondary;
                return options;
            }

            /* LB/RB: dir -1 goes to the start of the current section, or the previous one if already there; +1 to
               the next. Stops at the first and last sections. Entries before the first stop form a section of
               their own. Returns true if the focus moved, with the heading of the section, if any. */
            internal bool JumpSection(int dir, out View heading)
            {
                heading = null;
                int stopCount = _stops.Count;
                if (stopCount == 0)
                    return false;
                int index = _focus.FocusIndex;
                int current = -1;
                int k;
                for (k = 0; k < stopCount; k++)
                {
                    if (_stops[k].Index <= index)
                        current = k;
                    else
                        break;
                }

                int target;
                if (dir > 0)
                {
                    for (k = current + 1; k < stopCount; k++)
                    {
                        target = FirstAvailableInSection(k);
                        if (target >= 0)
                        {
                            heading = _stops[k].Heading;
                            return _focus.SetFocus(target, 1);
                        }
                    }
                    return false;
                }

                int lowest = _stops[0].Index > 0 ? -1 : 0;
                if (current >= lowest)
                {
                    target = FirstAvailableInSection(current);
                    if (target >= 0 && target < index)
                    {
                        heading = current >= 0 ? _stops[current].Heading : null;
                        return _focus.SetFocus(target, -1);
                    }
                }
                for (k = current - 1; k >= lowest; k--)
                {
                    target = FirstAvailableInSection(k);
                    if (target >= 0)
                    {
                        heading = k >= 0 ? _stops[k].Heading : null;
                        return _focus.SetFocus(target, -1);
                    }
                }
                return false;
            }

            /* Section -1 is the entries before the first stop; returns -1 if none of the section is available */
            private int FirstAvailableInSection(int section)
            {
                int start = section < 0 ? 0 : _stops[section].Index;
                int end = section + 1 < _stops.Count ? _stops[section + 1].Index : _views.Count;
                int i;
                for (i = start; i < end; i++)
                {
                    if (_focus.IsAvailableAt(i))
                        return i;
                }
                return -1;
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

            /* The first (dir < 0) or last available entry inside ScrollView, or of the layer when it has none
               inside; -1 if none is available */
            public int FindEdgeIndex(int dir)
            {
                int count = _views.Count;
                int fallback = -1;
                int k;
                for (k = 0; k < count; k++)
                {
                    int i = dir < 0 ? k : count - 1 - k;
                    if (!_focus.IsAvailableAt(i))
                        continue;
                    if (ScrollView == null || IsDescendantOf(_views[i], ScrollView))
                        return i;
                    if (fallback < 0)
                        fallback = i;
                }
                return fallback;
            }

            /* The focus is unset or unavailable, or its row is inside ScrollView but wholly outside the viewport */
            internal bool IsFocusLost()
            {
                int index = _focus.FocusIndex;
                if (!_focus.IsAvailableAt(index))
                    return true;
                double top, bottom, y, height;
                if (!TryGetViewport(out top, out bottom) || !TryGetContentBounds(index, out y, out height))
                    return false;
                return y + height <= top || y >= bottom;
            }

            /* The available entry inside ScrollView whose row starts highest within the viewport, else the first
               one overlapping it; -1 if none */
            internal int FindTopVisibleIndex()
            {
                double top, bottom;
                if (!TryGetViewport(out top, out bottom))
                    return -1;
                int best = -1;
                double bestY = 0.0;
                int overlapping = -1;
                int i;
                for (i = 0; i < _views.Count; i++)
                {
                    double y, height;
                    if (!_focus.IsAvailableAt(i) || !TryGetContentBounds(i, out y, out height))
                        continue;
                    if (y >= top - ViewportTolerance && y < bottom)
                    {
                        if (best < 0 || y < bestY)
                        {
                            best = i;
                            bestY = y;
                        }
                    }
                    else if (overlapping < 0 && y < bottom && y + height > top)
                    {
                        overlapping = i;
                    }
                }
                return best >= 0 ? best : overlapping;
            }

            /* ScrollView's visible range in its content coordinates; false without a laid-out ScrollView */
            private bool TryGetViewport(out double top, out double bottom)
            {
                top = 0.0;
                bottom = 0.0;
                ScrollView scrollView = ScrollView;
                if (scrollView == null || scrollView.Height <= 0.0)
                    return false;
                top = scrollView.ScrollY;
                bottom = top + scrollView.Height;
                return true;
            }

            /* The entry's row top and height in ScrollView's content coordinates; false when the row is not inside
               ScrollView or not laid out */
            private bool TryGetContentBounds(int index, out double y, out double height)
            {
                y = 0.0;
                height = 0.0;
                ScrollView scrollView = ScrollView;
                if (scrollView == null || index < 0 || index >= _highlights.Count)
                    return false;
                View row = _highlights[index];
                height = row.Height;
                if (height <= 0.0)
                    return false;
                Element element = row;
                while (element != null && !ReferenceEquals(element, scrollView))
                {
                    VisualElement visual = element as VisualElement;
                    if (visual != null)
                        y += visual.Y;
                    element = element.Parent;
                }
                return element != null;
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
                if (_focus.MoveWithin(dir))
                    return true;
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
                        return _focus.SetFocus(index - columns, dir);
                    if (!allowWrap)
                        return false;
                    /* The first row wraps to the last entry, or to the same column on the last row */
                    if (count > gridCount)
                        return _focus.SetFocus(count - 1, dir);
                    int target = lastRow * columns + index % columns;
                    return _focus.SetFocus(Math.Min(target, gridCount - 1), dir);
                }
                if (row < lastRow)
                    return _focus.SetFocus(Math.Min(index + columns, gridCount - 1), dir);
                /* The last row continues to the entries after the grid, or wraps to the top of the column */
                if (count > gridCount)
                    return _focus.SetFocus(gridCount, dir);
                if (!allowWrap)
                    return false;
                return _focus.SetFocus(index % columns, dir);
            }
        }

        /* The selection of a CollectionView entry, over items in the list's ItemsSource order; it is set on entry
           only while the gamepad is the active input */
        private sealed class CollectionCursor
        {
            private readonly CollectionView _list;
            private readonly Func<System.Collections.IList> _items;

            public CollectionCursor(CollectionView list, Func<System.Collections.IList> items)
            {
                _list = list;
                _items = items;
            }

            private System.Collections.IList Items
            {
                get
                {
                    try
                    {
                        return _items();
                    }
                    catch (Exception ex)
                    {
                        Debug.WriteLine(ex);
                        return null;
                    }
                }
            }

            public int ItemCount
            {
                get
                {
                    System.Collections.IList items = Items;
                    return items != null ? items.Count : 0;
                }
            }

            /* The selected item if it is in items, else null */
            public object Current
            {
                get
                {
                    object selected = _list.SelectedItem;
                    if (selected == null)
                        return null;
                    System.Collections.IList items = Items;
                    return items != null && items.Contains(selected) ? selected : null;
                }
            }

            /* Keeps the selection; with none, selects the first item, or the last when entered from below */
            public void Enter(int dir)
            {
                if (!GHGamepadService.IsActive)
                    return;
                System.Collections.IList items = Items;
                if (items == null || items.Count == 0)
                    return;
                object current = Current;
                int index = current != null ? items.IndexOf(current) : -1;
                if (index < 0)
                    index = dir < 0 ? items.Count - 1 : 0;
                Select(items, index);
            }

            /* Moves the selection by one; false past the first or last item */
            public bool Move(int dir)
            {
                System.Collections.IList items = Items;
                if (items == null || items.Count == 0)
                    return false;
                object current = Current;
                int index = current != null ? items.IndexOf(current) : -1;
                int next = index < 0 ? (dir < 0 ? items.Count - 1 : 0) : index + dir;
                if (next < 0 || next >= items.Count)
                    return false;
                Select(items, next);
                return true;
            }

            private void Select(System.Collections.IList items, int index)
            {
                object item = items[index];
                if (!Equals(_list.SelectedItem, item))
                    _list.SelectedItem = item;
                try
                {
                    _list.ScrollTo(index, -1, ScrollToPosition.MakeVisible, false);
                }
                catch (Exception ex)
                {
                    Debug.WriteLine(ex);
                }
            }
        }

        private const double LineScrollStep = 48.0;
        /* A row starting this many DIP above the viewport still counts as starting within it */
        private const double ViewportTolerance = 1.0;
        private const double PageScrollFraction = 0.85;
        /* DIP per second at full right-stick push; the push is raised to RightStickScrollExponent first */
        private const double RightStickScrollSpeed = 1200.0;
        private const double RightStickScrollExponent = 1.5;

        private readonly Layer _main = new Layer(null);
        private readonly List<Layer> _overlays = new List<Layer>();
        private Layer _active;
        private bool _attached = false;
        private bool _aArmed = false;
        private int _aPressId = 0;
        private bool _xArmed = false;
        private int _xPressId = 0;
        /* Right-stick scrolling below one DIP not yet applied, for _stickScrollLayer */
        private double _stickScrollRemainder = 0.0;
        private Layer _stickScrollLayer;

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
            _xArmed = false;
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
            _xArmed = false;
        }

        public void RefreshVisuals()
        {
            try
            {
                Layer layer = UpdateActiveLayer();
                /* Becoming the active input does not scroll back to a focus scrolled out of view */
                if (GHGamepadService.IsActive && layer.Focus.FocusIndex >= 0 && FocusTopVisibleIfLost(layer))
                    return;
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
                    _xArmed = false;
                    return true;
                }

                Layer layer = UpdateActiveLayer();
                if (e.RevealsUi)
                {
                    if (e.Button == GHGamepadButton.A)
                        _aArmed = false;
                    else if (e.Button == GHGamepadButton.X)
                        _xArmed = false;
                    if (layer.Focus.FocusIndex >= 0 && FocusTopVisibleIfLost(layer))
                        return true;
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
                        MoveHorizontal(layer, -1, e.Kind);
                        break;
                    case GHGamepadButton.DPadRight:
                    case GHGamepadButton.LeftStickRight:
                        MoveHorizontal(layer, 1, e.Kind);
                        break;
                    case GHGamepadButton.LB:
                    case GHGamepadButton.RB:
                        if (e.Kind == GHGamepadButtonEventKind.Pressed || e.Kind == GHGamepadButtonEventKind.Repeat)
                        {
                            if (layer.HasSectionStops)
                            {
                                JumpSection(layer, e.Button == GHGamepadButton.LB ? -1 : 1);
                            }
                            else if (layer.ScrollsContent && layer.ScrollView != null)
                            {
                                double page = Math.Max(LineScrollStep, layer.ScrollView.Height * PageScrollFraction);
                                ScrollContentBy(layer, e.Button == GHGamepadButton.LB ? -page : page);
                            }
                        }
                        break;
                    case GHGamepadButton.LT:
                    case GHGamepadButton.RT:
                        if (e.Kind == GHGamepadButtonEventKind.Pressed)
                            JumpToEdge(layer, e.Button == GHGamepadButton.LT ? -1 : 1);
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
                    case GHGamepadButton.X:
                        if (e.Kind == GHGamepadButtonEventKind.Pressed)
                        {
                            /* A press that only restores the focus does not run the secondary action */
                            bool hadFocus = EnsureFocus(layer);
                            _xArmed = hadFocus && layer.Focus.HasSecondary;
                            _xPressId = e.PressId;
                            if (!hadFocus)
                            {
                                layer.Focus.RefreshVisuals();
                                ScrollFocusIntoView(layer);
                            }
                        }
                        else if (e.Kind == GHGamepadButtonEventKind.Released)
                        {
                            if (e.IsCancelled && e.PressId == _xPressId)
                                _xArmed = false;
                        }
                        else if (e.Kind == GHGamepadButtonEventKind.ShortPressed)
                        {
                            if (_xArmed && e.PressId == _xPressId)
                            {
                                _xArmed = false;
                                layer.Focus.Secondary();
                            }
                        }
                        break;
                    case GHGamepadButton.B:
                        if (e.Kind == GHGamepadButtonEventKind.Pressed)
                        {
                            if (layer.EndFocusedTextEdit())
                                break;
                            if (layer.Back != null)
                                layer.Back();
                        }
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

        /* Right stick, every gamepad tick: scrolls the shown layer's ScrollView by RightY at up to
           RightStickScrollSpeed, finer for small pushes, in whole DIPs; leaves the focus and the active layer as
           they are */
        public void UpdateSticks(GHGamepadSticks sticks, double dtSeconds)
        {
            try
            {
                if (sticks.IsNeutral || sticks.RightY == 0f || !GHGamepadService.IsActive
                    || GHApp.PushingModalPage || GHApp.IsSystemBrowserOpen || (IsBusy != null && IsBusy()))
                {
                    _stickScrollRemainder = 0.0;
                    return;
                }
                if (dtSeconds <= 0.0)
                    return;

                Layer layer = FindShownLayer();
                if (!ReferenceEquals(layer, _stickScrollLayer))
                {
                    _stickScrollLayer = layer;
                    _stickScrollRemainder = 0.0;
                }
                if (layer.ScrollView == null)
                    return;

                double y = sticks.RightY;
                double speed = RightStickScrollSpeed * Math.Pow(Math.Abs(y), RightStickScrollExponent);
                _stickScrollRemainder += (y < 0.0 ? -speed : speed) * dtSeconds;
                if (Math.Abs(_stickScrollRemainder) < 1.0)
                    return;
                double step = Math.Truncate(_stickScrollRemainder);
                _stickScrollRemainder -= step;
                ScrollContentBy(layer, step);
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex);
                _stickScrollRemainder = 0.0;
            }
        }

        /* Wraps only on a fresh press, so a held direction stops at the end */
        private void MoveFocus(Layer layer, int dir, bool vertical, GHGamepadButtonEventKind kind)
        {
            if (kind != GHGamepadButtonEventKind.Pressed && kind != GHGamepadButtonEventKind.Repeat)
                return;
            bool fresh = kind == GHGamepadButtonEventKind.Pressed;
            if (vertical && FocusTopVisibleIfLost(layer))
                return;
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

        /* Left/Right adjust an entry that has Adjust, held to repeat; otherwise they move where the layer allows */
        private void MoveHorizontal(Layer layer, int dir, GHGamepadButtonEventKind kind)
        {
            if (layer.Focus.CanAdjust)
            {
                if (kind == GHGamepadButtonEventKind.Pressed || kind == GHGamepadButtonEventKind.Repeat)
                    layer.Focus.Adjust(dir);
                return;
            }
            if (layer.HorizontalMoves)
                MoveFocus(layer, dir, false, kind);
        }

        /* L2/R2: the first or last entry of the scrolled list (FindEdgeIndex); entering a list from the far side */
        private void JumpToEdge(Layer layer, int dir)
        {
            if (!EnsureFocus(layer))
            {
                layer.Focus.RefreshVisuals();
                ScrollFocusIntoView(layer);
                return;
            }
            int index = layer.FindEdgeIndex(dir);
            if (index < 0 || index == layer.Focus.FocusIndex)
                return;
            if (layer.Focus.SetFocus(index, -dir))
                ScrollFocusIntoView(layer);
        }

        private void JumpSection(Layer layer, int dir)
        {
            if (!EnsureFocus(layer))
            {
                layer.Focus.RefreshVisuals();
                ScrollFocusIntoView(layer);
                return;
            }
            View heading;
            if (!layer.JumpSection(dir, out heading))
                return;
            ScrollView scrollView = layer.ScrollView;
            if (heading != null && scrollView != null && GHGamepadService.IsActive && heading.IsVisible
                && IsDescendantOf(heading, scrollView))
            {
                /* The row first, then its heading above it, so that both show */
                GHGamepadFocusList.BringIntoView(scrollView, layer.Focus.FocusedHighlight, ScrollToPosition.MakeVisible);
                GHGamepadFocusList.BringIntoView(scrollView, heading, ScrollToPosition.Start);
            }
            else
            {
                ScrollFocusIntoView(layer);
            }
        }

        /* With the focus unset, unavailable or scrolled out of view (Layer.IsFocusLost), focuses the top visible
           entry of the layer's ScrollView; true if it did */
        private static bool FocusTopVisibleIfLost(Layer layer)
        {
            if (!layer.IsFocusLost())
                return false;
            int index = layer.FindTopVisibleIndex();
            if (index < 0 || !layer.Focus.SetFocus(index, 1))
                return false;
            layer.Focus.RefreshVisuals();
            ScrollFocusIntoView(layer);
            return true;
        }

        /* Returns true if the layer already had an available focus; otherwise tries its initial focus */
        private static bool EnsureFocus(Layer layer)
        {
            if (layer.Focus.IsAvailableAt(layer.Focus.FocusIndex))
                return true;
            layer.FocusInitial();
            return false;
        }

        /* The first shown overlay in AddOverlay order, else the main layer; changes nothing */
        private Layer FindShownLayer()
        {
            foreach (Layer overlay in _overlays)
            {
                if (overlay.IsShown())
                    return overlay;
            }
            return _main;
        }

        private Layer UpdateActiveLayer()
        {
            Layer layer = FindShownLayer();
            if (!ReferenceEquals(layer, _active))
            {
                _active.Focus.ClearVisuals();
                _active = layer;
                _aArmed = false;
                _xArmed = false;
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
            GHGamepadFocusList.BringIntoView(scrollView, layer.Focus.FocusedHighlight, ScrollToPosition.MakeVisible);
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

        /* The nearest ScrollView among the view's ancestors, or null */
        public static ScrollView FindScrollViewAncestor(Element view)
        {
            Element current = view != null ? view.Parent : null;
            while (current != null)
            {
                ScrollView scrollView = current as ScrollView;
                if (scrollView != null)
                    return scrollView;
                current = current.Parent;
            }
            return null;
        }

        /* Views of type T under element in visual-tree order, not looking inside a match */
        private static void CollectViews<T>(Element element, List<T> result) where T : View
        {
            if (element == null)
                return;
            T match = element as T;
            if (match != null)
            {
                result.Add(match);
                return;
            }
#if GNH_MAUI
            Microsoft.Maui.IVisualTreeElement tree = element as Microsoft.Maui.IVisualTreeElement;
            if (tree == null)
                return;
            foreach (Microsoft.Maui.IVisualTreeElement child in tree.GetVisualChildren())
                CollectViews(child as Element, result);
#else
            foreach (Element child in ((IElementController)element).LogicalChildren)
                CollectViews(child, result);
#endif
        }
    }
}
