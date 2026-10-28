using System;
using System.Diagnostics;
#if GNH_MAUI
using Microsoft.Maui.Controls;
#else
using Xamarin.Forms;
#endif

namespace GnollHackX
{
    /* Same values as Steamworks' EFloatingGamepadTextInputMode */
    public enum GHScreenKeyboardMode
    {
        SingleLine = 0,
        MultipleLines = 1,
        Email = 2,
        Numeric = 3,
    }

    /* A platform's on-screen keyboard that the app opens itself */
    public interface IGHScreenKeyboardProvider
    {
        /* The platform and session allow opening it now (e.g. Steam Deck in Game Mode) */
        bool IsAvailable { get; }

        /* Opens the keyboard placed away from field; false when it could not be opened */
        bool Show(View field, GHScreenKeyboardMode mode);

        void Hide();
    }

    /* Opens the on-screen keyboard for a text field when the controller starts the edit. Does nothing
       unless a platform has set Provider and the gamepad is the active input. Hide closes only a keyboard
       that Show opened. UI thread only. */
    public static class GHScreenKeyboard
    {
        public static IGHScreenKeyboardProvider Provider;

        private static bool _isShown;

        public static bool IsShown { get { return _isShown; } }

        public static bool IsAvailable
        {
            get
            {
                IGHScreenKeyboardProvider provider = Provider;
                return provider != null && GHGamepadService.IsActive && provider.IsAvailable;
            }
        }

        public static bool Show(View field, GHScreenKeyboardMode mode)
        {
            if (field == null || !IsAvailable)
                return false;
            try
            {
                _isShown = Provider.Show(field, mode);
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex);
                _isShown = false;
            }
            return _isShown;
        }

        public static void Hide()
        {
            if (!_isShown)
                return;
            _isShown = false;
            try
            {
                IGHScreenKeyboardProvider provider = Provider;
                if (provider != null)
                    provider.Hide();
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex);
            }
        }

        /* The keyboard was closed by the player (Enter, its own close); no hide request is needed */
        public static void MarkClosed()
        {
            _isShown = false;
        }
    }
}
