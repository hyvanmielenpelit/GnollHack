#if WINDOWS && GNH_MAUI
using System;
using System.Diagnostics;
using System.Threading;
using GnollHackX;

namespace GnollHackM
{
    // WORKAROUND(WA-WINUI-0007): WriteableBitmap updates pile up shared surfaces while the display is off
    /* Suspends the main page (carousel and general timer) while the display is off or the session is
       locked, and resumes it when both clear, when GHConstants.PauseMainScreenWhenUnseen is set.
       Suspends the game page's animations while the display is off, when
       GHConstants.PauseGameRenderingWhenUnseen is set; the platform render loop skips the game page by
       itself then (GHApp.CompositionTarget_Rendering). MainPage.Resume starts the carousel only while
       the main page itself is showing, and a window that OnSleep has suspended (minimized) is left for
       HandleResume to resume. A WriteableBitmap-backed canvas, such as the carousel's or the CPU-rendered
       map's, that keeps updating while the display is off piles up window-sized shared surfaces:
       https://github.com/microsoft/microsoft-ui-xaml/issues/12355 */
    internal static class MainScreenPauseWindows
    {
        private static int _started;
        private static int _pausedByEnvironment;
        private static int _gamePausedByEnvironment;

        public static void Start()
        {
            try
            {
                if (!GHConstants.PauseMainScreenWhenUnseen && !GHConstants.PauseGameRenderingWhenUnseen)
                    return;
                if (Interlocked.Exchange(ref _started, 1) != 0)
                    return;
                EnvironmentStateWindows.StateChanged += OnStateChanged;
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
        }

        /* Power callback or session poll thread; the work runs on the UI thread */
        private static void OnStateChanged()
        {
            try
            {
                Microsoft.Maui.ApplicationModel.MainThread.BeginInvokeOnMainThread(ApplyState);
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
        }

        private static void ApplyState()
        {
            try
            {
                if (GHConstants.PauseMainScreenWhenUnseen)
                    ApplyMainScreenState();
                if (GHConstants.PauseGameRenderingWhenUnseen)
                    ApplyGameScreenState();
                if (!EnvironmentStateWindows.IsUnseen)
                    GHApp.TryShowPendingResourceWarning();
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
        }

        private static void ApplyMainScreenState()
        {
            if (EnvironmentStateWindows.IsUnseen)
            {
                if (Interlocked.Exchange(ref _pausedByEnvironment, 1) != 0)
                    return;
                GHApp.CurrentMainPage?.Suspend();
                WriteBreadcrumb("Main screen paused: "
                    + (EnvironmentStateWindows.IsSessionLocked ? "session locked" : "display off"));
            }
            else
            {
                if (Interlocked.Exchange(ref _pausedByEnvironment, 0) == 0)
                    return;
                if (!GHApp.IsSuspended)
                    GHApp.CurrentMainPage?.Resume();
                WriteBreadcrumb("Main screen resumed");
            }
        }

        /* Display off only: the session lock is polled, and pausing on it could leave the map frozen after unlocking */
        private static void ApplyGameScreenState()
        {
            if (EnvironmentStateWindows.IsDisplayOff)
            {
                if (Interlocked.Exchange(ref _gamePausedByEnvironment, 1) != 0)
                    return;
                GHApp.CurrentGamePage?.Suspend();
                WriteBreadcrumb("Game screen paused: display off");
            }
            else
            {
                if (Interlocked.Exchange(ref _gamePausedByEnvironment, 0) == 0)
                    return;
                if (!GHApp.IsSuspended)
                    GHApp.CurrentGamePage?.Resume();
                WriteBreadcrumb("Game screen resumed");
            }
        }

        /* Sentry breadcrumb, and the GnollHack log or the debug output */
        private static void WriteBreadcrumb(string text)
        {
            GHApp.MaybeWriteGHLog(text, true, GHConstants.SentryGnollHackGeneralCategoryName);
        }
    }
}
#endif
