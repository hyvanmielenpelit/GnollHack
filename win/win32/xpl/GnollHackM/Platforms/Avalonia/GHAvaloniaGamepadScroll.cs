#if GNH_AVALONIA
using System;
using System.Diagnostics;
using GnollHackX;
using Microsoft.Maui;
using AvControl = Avalonia.Controls.Control;
using MauiView = Microsoft.Maui.Controls.View;

namespace GnollHackM
{
    /* Gamepad focus scrolling on Avalonia: the focused view's platform control asks its ScrollViewer to show it
       (ControlExtensions.BringIntoView), which scrolls minimally. MAUI's ScrollView.ScrollToAsync(element) computes no
       scroll on this backend. UI thread only. */
    internal static class GHAvaloniaGamepadScroll
    {
        /* Sets GHGamepadFocusList.PlatformBringIntoView */
        public static void Register()
        {
            GHGamepadFocusList.PlatformBringIntoView = BringIntoView;
        }

        private static bool BringIntoView(MauiView view)
        {
            if (view == null)
                return false;
            AvControl control;
            try
            {
                IElementHandler handler = view.Handler;
                control = handler?.PlatformView as AvControl;
            }
            catch (InvalidOperationException)
            {
                return false;
            }
            if (control == null)
                return false;
            try
            {
                Avalonia.Controls.ControlExtensions.BringIntoView(control);
                return true;
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex);
                return false;
            }
        }
    }
}
#endif
