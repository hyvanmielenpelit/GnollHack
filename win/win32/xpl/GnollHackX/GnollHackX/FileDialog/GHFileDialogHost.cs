using System;
using System.Collections.Generic;
using System.Threading.Tasks;

#if GNH_MAUI
using GnollHackM;
using Microsoft.Maui.Controls;
#else
using GnollHackX.Controls;
using Xamarin.Forms;
#endif

namespace GnollHackX
{
    /* Decides between the system's file dialogs and FileDialogView, and shows the view inside the page that takes input.
       No modal page is involved, because a page beneath a modal gets Disappearing and Appearing, and OverseerPage
       answers those by leaving and reloading the chat. UI thread only. */
    public static class GHFileDialogHost
    {
        private static readonly GHFileDialogQueue _queue = new GHFileDialogQueue();
        private static FileDialogView _activeView;
        private static string _environmentChoice;
        private static bool _environmentRead = false;
        private static bool _isGameMode = false;
        private static bool _testSwitch = false;
        private static bool _testFired = false;

        /* The open view, which takes the gamepad's input; null when none is open */
        public static IGHGamepadHandlingPage ActiveView { get { return _activeView; } }

        public static bool IsOpen { get { return _activeView != null; } }

        /* Raised when a view opens or closes */
        public static event Action ActiveViewChanged;

        // WORKAROUND(WA-STEAM-0002): SteamOS Gaming Mode has no FileChooser portal, and GTK dialogs have no controller input
        /* The system's dialog cannot be used with a controller: Gaming Mode, or the last input came from a controller.
           Touch or the mouse reset the latter, so outside Gaming Mode they get the system's dialog.
           GNOLLHACK_FILE_DIALOG=custom or system decides for testing. Always false on a platform without file places. */
        public static bool UseCustomDialog
        {
            get
            {
                if (GHFilePlaces.Current == null)
                    return false;
                ReadEnvironment();
                bool res;
                if (_environmentChoice == "custom")
                    res = true;
                else if (_environmentChoice == "system")
                    res = false;
                else
                    res = _isGameMode || GHGamepadService.IsActive;
                GHApp.MaybeWriteGHLog("GHFileDialogHost: using the " + (res ? "custom" : "system") + " file dialog");
                return res;
            }
        }

        private static void ReadEnvironment()
        {
            if (_environmentRead)
                return;
            _environmentRead = true;
            try
            {
                string choice = Environment.GetEnvironmentVariable("GNOLLHACK_FILE_DIALOG");
                _environmentChoice = choice != null ? choice.Trim().ToLowerInvariant() : null;
                _testSwitch = Environment.GetEnvironmentVariable("GNOLLHACK_FILE_DIALOG_TEST") == "1";
#if GNH_AVALONIA
                _isGameMode = GHAvaloniaSteamKeyboard.IsGameMode(Environment.GetEnvironmentVariable("XDG_CURRENT_DESKTOP"), Environment.GetEnvironmentVariable("SteamGamepadUI"));
#endif
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHFileDialogHost: reading the environment failed: " + ex.Message);
            }
        }

        /* The chosen paths; null when cancelled. Never throws. */
        public static Task<IList<string>> PickFilesAsync(string title, string confirmText, bool multiple, int maxFiles, IEnumerable<string> extensions, bool isImport)
        {
            return ShowAsync(new GHFileOpenBehavior(title, confirmText, multiple, maxFiles, extensions, GHFilePlaces.Current.GetOpenPlaces(isImport)));
        }

        /* Saves a copy of sourcePath in a place the user picks, with no overwriting; returns once the dialog has closed */
        public static Task SaveCopyAsync(string title, string sourcePath, string suggestedName)
        {
            return ShowAsync(new GHFileSaveBehavior(title, sourcePath, suggestedName, GHFilePlaces.Current.GetSavePlaces(), ProtectedFolders));
        }

        /* GnollHack's data and install folders, which a copy must not land in */
        public static IList<string> ProtectedFolders
        {
            get
            {
                List<string> folders = new List<string>();
                folders.Add(GHApp.GHPath);
#if GNH_AVALONIA
                folders.Add(GHAvaloniaPaths.AssetDirectory);
#endif
                return folders;
            }
        }

        /* Escape from a keyboard, as B */
        public static void SendBack()
        {
            FileDialogView view = _activeView;
            if (view != null)
                view.GoBack();
        }

        private static Task<IList<string>> ShowAsync(GHFileDialogBehavior behavior)
        {
            return _queue.Enqueue(() => ShowNowAsync(behavior));
        }

        /* On whichever page is on top when the dialog's turn comes. GetInputTargetPage is not used, because during a game's
           exit it can return the game page after it has left the modal stack, and a view there would take input unseen. */
        private static async Task<IList<string>> ShowNowAsync(GHFileDialogBehavior behavior)
        {
            Page page = GHApp.PageFromTopOfModalNavigationStack() ?? GHApp.CurrentMainPage;
            ContentPage contentPage = page as ContentPage;
            Grid root = contentPage != null ? contentPage.Content as Grid : null;
            if (root == null)
            {
                GHApp.MaybeWriteGHLog("GHFileDialogHost: no page with a root grid to show a file dialog on");
                return null;
            }

            FileDialogView view = null;
            EventHandler disappearing = (s, e) => behavior.HostGone();
            try
            {
                behavior.Log = (message) => GHApp.MaybeWriteGHLog(message);
                view = new FileDialogView(behavior);
                Grid.SetRowSpan(view, Math.Max(1, root.RowDefinitions.Count));
                Grid.SetColumnSpan(view, Math.Max(1, root.ColumnDefinitions.Count));
                root.Children.Insert(GetInsertIndex(root), view);
                _activeView = view;
                page.Disappearing += disappearing;
                RaiseActiveViewChanged();
                view.Open();
                _ = RunTestSwitchAsync(page, view, behavior as GHFileSaveBehavior);
                await behavior.LoadAsync();
                return await behavior.Result;
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHFileDialogHost: the file dialog failed: " + ex.Message);
                return null;
            }
            finally
            {
                /* First, so that a failure below cannot leave input going to a dead view */
                _activeView = null;
                try
                {
                    page.Disappearing -= disappearing;
                    if (view != null)
                    {
                        view.Close();
                        root.Children.Remove(view);
#if GNH_MAUI
                        GHApp.DisconnectIViewHandlers(view);
#endif
                    }
                }
                catch (Exception ex)
                {
                    GHApp.MaybeWriteGHLog("GHFileDialogHost: closing the file dialog failed: " + ex.Message);
                }
                RaiseActiveViewChanged();
            }
        }

        /* Below the page's message popup, so that a popup stays above the dialog; else on top */
        private static int GetInsertIndex(Grid root)
        {
            int i;
            for (i = 0; i < root.Children.Count; i++)
            {
                if (root.Children[i] is MessagePopupView)
                    return i;
            }
            return root.Children.Count;
        }

        /* GNOLLHACK_FILE_DIALOG_TEST=1: once per process, a popup and a second dialog arrive during a save dialog. A dialog
           closed within the wait consumes nothing. */
        private static async Task RunTestSwitchAsync(Page page, FileDialogView view, GHFileSaveBehavior behavior)
        {
            ReadEnvironment();
            if (!_testSwitch || _testFired || behavior == null)
                return;
            try
            {
                await Task.Delay(3000);
                if (_testFired || !ReferenceEquals(_activeView, view))
                    return;
                _testFired = true;
                Task popup = Task.CompletedTask;
                if (page is IMessagePopupPage)
                    popup = GHApp.DisplayMessageBox(page, "File Dialog Test", "This popup should be above the file dialog.", "OK");
                else
                    GHApp.MaybeWriteGHLog("GHFileDialogHost: no popup test on " + page.GetType().Name);
                Task queued = SaveCopyAsync("Queued Test", behavior.SourcePath, behavior.SuggestedName);
                await Task.WhenAll(popup, queued);
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHFileDialogHost: the file dialog test failed: " + ex.Message);
            }
        }

        private static void RaiseActiveViewChanged()
        {
            try
            {
                ActiveViewChanged?.Invoke();
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHFileDialogHost: ActiveViewChanged failed: " + ex.Message);
            }
        }
    }
}
