using System;
using System.Collections.Generic;
using System.Text;
using GnollHackX;

#if GNH_MAUI
#if WINDOWS
using System.Reflection;
using Microsoft.Maui.Platform;
#endif

namespace GnollHackM
#else
using Xamarin.Forms;
using Xamarin.Forms.PlatformConfiguration;
using Xamarin.Forms.PlatformConfiguration.iOSSpecific;

namespace GnollHackX
#endif
{
    public partial class CustomModalPage : ContentPage
    {
        public CustomModalPage() : base()
        {
#if GNH_MAUI
            SafeAreaEdges = SafeAreaEdges.All;
#else
            On<iOS>().SetUseSafeArea(true);
#endif
        }

        protected override void OnAppearing()
        {
            base.OnAppearing();

#if WINDOWS
            if (!GHApp.WindowedMode)
            {
                try
                {
                    // Defer to ensure the modal's MAUI context is fully set up
                    Dispatcher.Dispatch(() =>
                    {
                        FixModalTitleBarGap(this);
                    });
                }
                catch (Exception ex)
                {
                    GHApp.MaybeWriteGHLog(ex.Message);
                }
            }
#endif
        }

        public static void FixModalTitleBarGap(ContentPage modalPage)
        {
#if WINDOWS
            try
            {
                var mauiContext = modalPage.Handler?.MauiContext;
                if (mauiContext is null)
                    return;

                // 1. Get the NavigationRootManager for this modal's scoped context
                var navManager = mauiContext.Services.GetService(
                    typeof(Microsoft.Maui.Platform.NavigationRootManager));

                if (navManager is not null)
                {
                    // 2. Invoke internal void SetTitleBarVisibility(bool isVisible)
                    var method = navManager.GetType().GetMethod(
                        "SetTitleBarVisibility",
                        BindingFlags.Instance | BindingFlags.NonPublic | BindingFlags.Public);

                    if (method is not null)
                    {
                        // Passing false tells it to collapse the title bar, zero the 32px margin,
                        // and clear the unclickable non-client input regions.
                        method.Invoke(navManager, new object[] { false });
                    }
                }
            }
            catch (Exception ex) 
            {
                GHApp.MaybeWriteGHLog(ex.Message);
            }
#endif
        }
    }
}
