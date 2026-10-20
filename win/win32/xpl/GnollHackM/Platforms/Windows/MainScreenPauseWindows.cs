#if WINDOWS && GNH_MAUI
using System;
using System.Diagnostics;
using System.Threading;
using GnollHackX;

namespace GnollHackM
{
    /* Suspends the main page (carousel and general timer) while the display is off or the session is
       locked, and resumes it when both clear, when GHConstants.PauseMainScreenWhenUnseen is set.
       MainPage.Resume starts the carousel only while the main page itself is showing, and a window
       that OnSleep has suspended (minimized) is left for HandleResume to resume. A WriteableBitmap-backed
       canvas such as the carousel's that keeps updating while the display is off piles up window-sized
       shared surfaces: https://github.com/microsoft/microsoft-ui-xaml/issues/12355 */
    internal static class MainScreenPauseWindows
    {
        private static int _started;
        private static int _pausedByEnvironment;

        public static void Start()
        {
            try
            {
                if (!GHConstants.PauseMainScreenWhenUnseen)
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
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
        }

        private static void WriteBreadcrumb(string text)
        {
            GHApp.AddSentryBreadcrumb(text, GHConstants.SentryGnollHackGeneralCategoryName);
            Debug.WriteLine(text);
        }
    }
}
#endif
