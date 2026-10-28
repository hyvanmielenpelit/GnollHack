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
    /* Gamepad focus over an ordered set of buttons; UI thread only */
    public sealed class GHGamepadFocusList
    {
        private sealed class Entry
        {
            public View View;
            public Action Activate;
            public Func<bool> IsAvailable;
        }

        private readonly List<Entry> _entries = new List<Entry>();
        private int _index = -1;

        /* Scrolled to keep the focused view visible, if set */
        public ScrollView ScrollView { get; set; }

        public int Count { get { return _entries.Count; } }
        public int FocusIndex { get { return _index; } }
        public View FocusedView { get { return _index >= 0 && _index < _entries.Count ? _entries[_index].View : null; } }

        public void Add(View view, Action activate, Func<bool> isAvailable)
        {
            if (view == null)
                return;
            _entries.Add(new Entry { View = view, Activate = activate, IsAvailable = isAvailable });
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
                    _index = candidate;
                    RefreshVisuals();
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
                        _index = candidate;
                        RefreshVisuals();
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
                    _index = candidate;
                    RefreshVisuals();
                    return true;
                }
            }
            return false;
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

        /* Shows the focus only while the gamepad is the active input */
        public void RefreshVisuals()
        {
            bool active = GHGamepadService.IsActive;
            int i;
            for (i = 0; i < _entries.Count; i++)
                SetViewFocused(_entries[i].View, active && i == _index);

            if (active && ScrollView != null)
            {
                View focused = FocusedView;
                if (focused != null && focused.IsVisible)
                {
                    try
                    {
                        _ = ScrollView.ScrollToAsync(focused, ScrollToPosition.MakeVisible, false);
                    }
                    catch (Exception ex)
                    {
                        Debug.WriteLine(ex);
                    }
                }
            }
        }

        public void ClearVisuals()
        {
            int i;
            for (i = 0; i < _entries.Count; i++)
                SetViewFocused(_entries[i].View, false);
        }

        private static void SetViewFocused(View view, bool focused)
        {
            CustomImageButton cib = view as CustomImageButton;
            if (cib != null)
            {
                cib.GamepadFocused = focused;
                return;
            }
            LabeledImageButton lib = view as LabeledImageButton;
            if (lib != null)
                lib.GamepadFocused = focused;
        }
    }
}
