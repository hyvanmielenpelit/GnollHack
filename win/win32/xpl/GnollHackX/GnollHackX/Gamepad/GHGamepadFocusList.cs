using System;
using System.Collections.Generic;
using System.Diagnostics;
#if GNH_MAUI
using Microsoft.Maui.Controls;
using GnollHackX;
#else
using Xamarin.Forms;
#endif

#if GNH_MAUI
namespace GnollHackM
#else
namespace GnollHackX.Controls
#endif
{
    /* Gamepad focus over an ordered set of buttons and other views, each with optional EntryOptions for
       adjusting (Left/Right), a secondary action (X), moving inside the entry, and leave and enter hooks.
       UI thread only. */
    public sealed class GHGamepadFocusList
    {
        /* Optional per-entry behavior besides activation */
        public sealed class EntryOptions
        {
            /* Left/Right instead of moving the focus; -1 or +1 */
            public Action<int> Adjust;

            /* X */
            public Action Secondary;

            /* The focus moved from this entry to another */
            public Action Leave;

            /* The focus moved to this entry: +1 moving forward, -1 moving backward, 0 otherwise */
            public Action<int> Enter;

            /* Up/Down inside the entry (-1 or +1); false lets the focus move on */
            public Func<int, bool> MoveWithin;

            /* The view shows its own focus; nothing is tinted */
            public bool NoHighlight;
        }

        private sealed class Entry
        {
            public View View;
            public Action Activate;
            public Func<bool> IsAvailable;
            public EntryOptions Options;

            /* Tinted while a view that is not an image button has the focus */
            public View Highlight;
            public bool HighlightApplied;
            public bool OriginalBackgroundSet;
            public object OriginalBackground;
        }

        private readonly List<Entry> _entries = new List<Entry>();
        private int _index = -1;

        /* After Clear, the view that had the focus; it gets the focus back if it is added again */
        private View _restoreView;

        /* Scrolled to keep the focused view visible, if set */
        public ScrollView ScrollView { get; set; }

        /* The platform's own scroll-into-view, which scrolls the enclosing scroll view minimally; true when it
           handled the view. Set on Avalonia, where ScrollView.ScrollToAsync(element) computes no scroll. */
        public static Func<View, bool> PlatformBringIntoView;

        public int Count { get { return _entries.Count; } }
        public int FocusIndex { get { return _index; } }
        public View FocusedView { get { return _index >= 0 && _index < _entries.Count ? _entries[_index].View : null; } }

        /* The focused entry's highlight view (its row), else the focused view */
        public View FocusedHighlight
        {
            get
            {
                if (_index < 0 || _index >= _entries.Count)
                    return null;
                return _entries[_index].Highlight ?? _entries[_index].View;
            }
        }

        /* Scrolls scrollView so that view shows, through PlatformBringIntoView when it handles the view */
        public static void BringIntoView(ScrollView scrollView, View view, ScrollToPosition position)
        {
            if (scrollView == null || view == null)
                return;
            try
            {
                Func<View, bool> platform = PlatformBringIntoView;
                if (platform != null && platform(view))
                    return;
                _ = scrollView.ScrollToAsync(view, position, false);
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex);
            }
        }

        public void Add(View view, Action activate, Func<bool> isAvailable)
        {
            Add(view, activate, isAvailable, null);
        }

        /* highlight: tinted while focused unless view is an image button; null tints view itself */
        public void Add(View view, Action activate, Func<bool> isAvailable, View highlight)
        {
            Add(view, activate, isAvailable, highlight, null);
        }

        /* options: null for none */
        public void Add(View view, Action activate, Func<bool> isAvailable, View highlight, EntryOptions options)
        {
            if (view == null)
                return;
            Entry entry = new Entry
            {
                View = view,
                Activate = activate,
                IsAvailable = isAvailable,
                Options = options,
                Highlight = highlight ?? view
            };
            _entries.Add(entry);
            if (_restoreView != null && ReferenceEquals(view, _restoreView))
            {
                _restoreView = null;
                _index = _entries.Count - 1;
                SetEntryFocused(entry, GHGamepadService.IsActive);
            }
        }

        /* Removes every entry, restoring their looks; the focused view keeps the focus if it is added again */
        public void Clear()
        {
            View focused = FocusedView;
            ClearVisuals();
            _entries.Clear();
            _index = -1;
            _restoreView = focused;
        }

        public bool IsAvailableAt(int index)
        {
            if (index < 0 || index >= _entries.Count)
                return false;
            Entry entry = _entries[index];
            try
            {
                return entry.IsAvailable == null || entry.IsAvailable();
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex);
                return false;
            }
        }

        /* Focuses index, or the next available entry after it (wrapping); returns false if none is available */
        public bool SetFocus(int index)
        {
            return SetFocus(index, 0);
        }

        /* dir: the direction of the move that leads here, passed to the entry's Enter */
        public bool SetFocus(int index, int dir)
        {
            int count = _entries.Count;
            if (count == 0)
                return false;
            if (index < 0 || index >= count)
                index = 0;
            int i;
            for (i = 0; i < count; i++)
            {
                int candidate = (index + i) % count;
                if (IsAvailableAt(candidate))
                {
                    ChangeFocus(candidate, dir);
                    return true;
                }
            }
            return false;
        }

        public bool SetFocus(View view)
        {
            int i;
            for (i = 0; i < _entries.Count; i++)
            {
                if (_entries[i].View == view)
                    return SetFocus(i);
            }
            return false;
        }

        /* Moves to the next available entry in direction dir (+1 or -1); returns true if the focus moved */
        public bool Move(int dir, bool allowWrap)
        {
            int count = _entries.Count;
            if (count == 0 || dir == 0)
                return false;
            dir = dir > 0 ? 1 : -1;
            if (_index < 0 || _index >= count || !IsAvailableAt(_index))
            {
                /* No valid focus: start from the end the movement enters from */
                int start = _index >= 0 && _index < count ? _index : (dir > 0 ? 0 : count - 1);
                int k;
                for (k = 0; k < count; k++)
                {
                    int candidate = ((start + dir * k) % count + count) % count;
                    if (IsAvailableAt(candidate))
                    {
                        ChangeFocus(candidate, dir);
                        return true;
                    }
                }
                return false;
            }

            int steps;
            for (steps = 1; steps < count; steps++)
            {
                int candidate = _index + dir * steps;
                if (candidate < 0 || candidate >= count)
                {
                    if (!allowWrap)
                        return false;
                    candidate = (candidate % count + count) % count;
                }
                if (IsAvailableAt(candidate))
                {
                    ChangeFocus(candidate, dir);
                    return true;
                }
            }
            return false;
        }

        /* Runs Leave of the entry losing the focus and Enter of the one getting it */
        private void ChangeFocus(int index, int dir)
        {
            _restoreView = null;
            int previous = _index;
            _index = index;
            if (previous != index)
            {
                EntryOptions leaving = previous >= 0 && previous < _entries.Count ? _entries[previous].Options : null;
                if (leaving != null && leaving.Leave != null)
                {
                    try
                    {
                        leaving.Leave();
                    }
                    catch (Exception ex)
                    {
                        Debug.WriteLine(ex);
                    }
                }
                EntryOptions entering = _entries[index].Options;
                if (entering != null && entering.Enter != null)
                {
                    try
                    {
                        entering.Enter(dir);
                    }
                    catch (Exception ex)
                    {
                        Debug.WriteLine(ex);
                    }
                }
            }
            RefreshVisuals();
        }

        /* The focused entry's options if it is available, else null */
        private EntryOptions FocusedOptions
        {
            get { return IsAvailableAt(_index) ? _entries[_index].Options : null; }
        }

        /* Runs the focused entry's action if it is available; returns true if it ran */
        public bool Activate()
        {
            if (!IsAvailableAt(_index))
                return false;
            Action activate = _entries[_index].Activate;
            if (activate == null)
                return false;
            activate();
            return true;
        }

        public bool CanAdjust
        {
            get
            {
                EntryOptions options = FocusedOptions;
                return options != null && options.Adjust != null;
            }
        }

        /* Runs the focused entry's Adjust; returns true if it has one */
        public bool Adjust(int dir)
        {
            EntryOptions options = FocusedOptions;
            if (options == null || options.Adjust == null)
                return false;
            options.Adjust(dir > 0 ? 1 : -1);
            return true;
        }

        public bool HasSecondary
        {
            get
            {
                EntryOptions options = FocusedOptions;
                return options != null && options.Secondary != null;
            }
        }

        /* Runs the focused entry's Secondary; returns true if it has one */
        public bool Secondary()
        {
            EntryOptions options = FocusedOptions;
            if (options == null || options.Secondary == null)
                return false;
            options.Secondary();
            return true;
        }

        /* Moves inside the focused entry; returns true if it consumed the move */
        public bool MoveWithin(int dir)
        {
            EntryOptions options = FocusedOptions;
            if (options == null || options.MoveWithin == null)
                return false;
            return options.MoveWithin(dir > 0 ? 1 : -1);
        }

        /* Shows the focus only while the gamepad is the active input */
        public void RefreshVisuals()
        {
            bool active = GHGamepadService.IsActive;
            int i;
            for (i = 0; i < _entries.Count; i++)
                SetEntryFocused(_entries[i], active && i == _index);

            if (active && ScrollView != null)
            {
                View focused = FocusedView;
                if (focused != null && focused.IsVisible)
                    BringIntoView(ScrollView, FocusedHighlight, ScrollToPosition.MakeVisible);
            }
        }

        public void ClearVisuals()
        {
            int i;
            for (i = 0; i < _entries.Count; i++)
                SetEntryFocused(_entries[i], false);
        }

        private static void SetEntryFocused(Entry entry, bool focused)
        {
            if (entry.Options != null && entry.Options.NoHighlight)
                return;
            CustomImageButton cib = entry.View as CustomImageButton;
            if (cib != null)
            {
                cib.GamepadFocused = focused;
                return;
            }
            LabeledImageButton lib = entry.View as LabeledImageButton;
            if (lib != null)
            {
                lib.GamepadFocused = focused;
                return;
            }
            RowImageButton rib = entry.View as RowImageButton;
            if (rib != null)
            {
                rib.GamepadFocused = focused;
                return;
            }
            SetHighlight(entry, focused);
        }

        /* Tints the highlight view's background, restoring the original value or its unset state afterwards */
        private static void SetHighlight(Entry entry, bool focused)
        {
            View highlight = entry.Highlight;
            if (highlight == null || entry.HighlightApplied == focused)
                return;
            try
            {
                if (focused)
                {
                    entry.OriginalBackgroundSet = highlight.IsSet(VisualElement.BackgroundColorProperty);
                    entry.OriginalBackground = entry.OriginalBackgroundSet
                        ? highlight.GetValue(VisualElement.BackgroundColorProperty) : null;
                    highlight.BackgroundColor = UIUtils.GamepadFocusBackgroundColor;
                }
                else if (entry.OriginalBackgroundSet)
                {
                    highlight.SetValue(VisualElement.BackgroundColorProperty, entry.OriginalBackground);
                }
                else
                {
                    highlight.ClearValue(VisualElement.BackgroundColorProperty);
                }
                entry.HighlightApplied = focused;
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex);
            }
        }
    }
}
