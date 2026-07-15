using Foundation;
using ObjCRuntime;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UIKit;
using System.Diagnostics;

#if GNH_MAUI
using GnollHackX;

namespace GnollHackM.Platforms.iOS
#else
namespace GnollHackX.iOS
#endif
{
    public class GHUIApplication : UIApplication
    {
        private UIKeyCommand[] _cachedAllKeyCommands;
        private UIKeyCommand[] _cachedSpecialOnlyKeyCommands;

        public GHUIApplication() : base()
        {

        }

        public GHUIApplication(IntPtr handle) : base(handle)
        {

        }

        public GHUIApplication(Foundation.NSObjectFlag t) : base(t)
        {

        }

        /* UIKeyCommand-based keyboard handling.
         * 
         * On iOS with a physical keyboard, PressesBegan works for all keys.
         * However, when the iOS app runs on Mac via "Designed for iPad",
         * macOS intercepts alphanumeric key presses for its text input system,
         * so UIPress.Key is nil and PressesBegan silently drops them.
         * 
         * UIKeyCommand is Apple's recommended mechanism for keyboard input
         * and works reliably on both iOS and Mac. When a UIKeyCommand matches,
         * its handler is invoked instead of PressesBegan, so there is no
         * double-firing of key events.
         * 
         * When a text input control (UITextField / UITextView) is the first
         * responder, only special key commands (arrows, escape, etc.) are
         * returned so that character keys flow to the text field normally.
         */

        public override UIKeyCommand[] KeyCommands
        {
            get
            {
                if (_cachedAllKeyCommands == null)
                    BuildAllKeyCommands();

                if (IsTextInputActive())
                    return _cachedSpecialOnlyKeyCommands;
                return _cachedAllKeyCommands;
            }
        }

        private bool IsTextInputActive()
        {
            try
            {
                UIWindow keyWindow = this.KeyWindow;
                if (keyWindow == null)
                    return false;
                return HasFirstResponderOfType<UITextField>(keyWindow)
                    || HasFirstResponderOfType<UITextView>(keyWindow);
            }
            catch
            {
                return false;
            }
        }

        private static bool HasFirstResponderOfType<T>(UIView view) where T : UIView
        {
            if (view is T && view.IsFirstResponder)
                return true;
            foreach (UIView subview in view.Subviews)
            {
                if (HasFirstResponderOfType<T>(subview))
                    return true;
            }
            return false;
        }

        private void BuildAllKeyCommands()
        {
            var charCommands = new List<UIKeyCommand>();
            var specialCommands = new List<UIKeyCommand>();

            var charSelector = new Selector("HandleCharKeyInput:");
            var specialSelector = new Selector("HandleSpecialKeyInput:");

            /* Character keys: letters, digits, and symbols */
            string keys = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890<>!\"#$%&/()=+?,.-;:_*{}[]\'\\ §";

            /* Register with no modifier (plain keys) */
            foreach (char c in keys)
            {
                var cmd = UIKeyCommand.Create((NSString)c.ToString(), 0, charSelector);
                MaybeSetPriorityOverSystem(cmd);
                charCommands.Add(cmd);
            }

            /* Register with Control modifier */
            string ctrlKeys = "abcdefghijklmnopqrstuvwxyz";
            foreach (char c in ctrlKeys)
            {
                var cmd = UIKeyCommand.Create((NSString)c.ToString(), UIKeyModifierFlags.Control, charSelector);
                MaybeSetPriorityOverSystem(cmd);
                charCommands.Add(cmd);
            }

            /* Register with Alternate (Option/Alt) modifier for meta keys.
             * Both lowercase and uppercase are registered because Alt+a and Alt+A
             * are different commands in GnollHack. */
            string metaKeys = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
            foreach (char c in metaKeys)
            {
                var cmd = UIKeyCommand.Create((NSString)c.ToString(), UIKeyModifierFlags.Alternate, charSelector);
                MaybeSetPriorityOverSystem(cmd);
                charCommands.Add(cmd);
            }

            /* Special keys: arrows, enter, escape, space, tab */
            var specialKeyInputs = new (NSString input, UIKeyModifierFlags[] modifiers)[]
            {
                ((NSString)UIKeyCommand.UpArrow, new UIKeyModifierFlags[] { 0, UIKeyModifierFlags.Control, UIKeyModifierFlags.Alternate, UIKeyModifierFlags.Shift }),
                ((NSString)UIKeyCommand.DownArrow, new UIKeyModifierFlags[] { 0, UIKeyModifierFlags.Control, UIKeyModifierFlags.Alternate, UIKeyModifierFlags.Shift }),
                ((NSString)UIKeyCommand.LeftArrow, new UIKeyModifierFlags[] { 0, UIKeyModifierFlags.Control, UIKeyModifierFlags.Alternate, UIKeyModifierFlags.Shift }),
                ((NSString)UIKeyCommand.RightArrow, new UIKeyModifierFlags[] { 0, UIKeyModifierFlags.Control, UIKeyModifierFlags.Alternate, UIKeyModifierFlags.Shift }),
                ((NSString)UIKeyCommand.Escape, new UIKeyModifierFlags[] { (UIKeyModifierFlags)0 }),
                ((NSString)"\r", new UIKeyModifierFlags[] { (UIKeyModifierFlags)0 }),      /* Enter */
                ((NSString)"\t", new UIKeyModifierFlags[] { (UIKeyModifierFlags)0 }),      /* Tab */
            };

            foreach (var (input, modifiers) in specialKeyInputs)
            {
                foreach (var mod in modifiers)
                {
                    var cmd = UIKeyCommand.Create(input, mod, specialSelector);
                    MaybeSetPriorityOverSystem(cmd);
                    specialCommands.Add(cmd);
                }
            }

            /* Numpad digits */
            for (char c = '0'; c <= '9'; c++)
            {
                var cmd = UIKeyCommand.Create((NSString)c.ToString(), UIKeyModifierFlags.NumericPad, charSelector);
                MaybeSetPriorityOverSystem(cmd);
                charCommands.Add(cmd);
            }

            /* Cmd+Q: intercept quit to save game first (Mac only).
             * On "Designed for iPad" apps, Cmd+Q immediately kills the process.
             * By registering it as a UIKeyCommand with priority over system behavior,
             * we can trigger the save flow instead. */
            var quitSelector = new Selector("HandleQuitKeyInput:");
            var quitCmd = UIKeyCommand.Create((NSString)"q", UIKeyModifierFlags.Command, quitSelector);
            MaybeSetPriorityOverSystem(quitCmd);
            specialCommands.Add(quitCmd);

            _cachedSpecialOnlyKeyCommands = specialCommands.ToArray();
            _cachedAllKeyCommands = charCommands.Concat(specialCommands).ToArray();
        }

        private static void MaybeSetPriorityOverSystem(UIKeyCommand cmd)
        {
            /* WantsPriorityOverSystemBehavior prevents macOS from intercepting
             * key commands for its own menu shortcuts (e.g., Cmd+H for hide).
             * Available on iOS 15+ / macOS 12+. All Apple Silicon Macs support this. */
            if (OperatingSystem.IsIOSVersionAtLeast(15, 0) || OperatingSystem.IsMacCatalystVersionAtLeast(15, 0))
            {
                cmd.WantsPriorityOverSystemBehavior = true;
            }
        }

        [Export("HandleCharKeyInput:")]
        public void HandleCharKeyInput(UIKeyCommand cmd)
        {
            if (cmd?.Input == null || cmd.Input.Length < 1)
                return;

            bool isCtrl = (cmd.ModifierFlags & UIKeyModifierFlags.Control) != 0;
            bool isMeta = (cmd.ModifierFlags & UIKeyModifierFlags.Alternate) != 0;
            bool isNumPad = (cmd.ModifierFlags & UIKeyModifierFlags.NumericPad) != 0;
            char ch = cmd.Input[0];

            if (isNumPad && ch >= '0' && ch <= '9')
            {
                /* Numpad digits are sent as special keys */
                GHApp.SendSpecialKeyPress(GHSpecialKey.NumberPad0 + (int)(ch - '0'), false, false, false);
            }
            else if (ch == '$' || ch == '¢')
            {
                /* Dollar sign special case (some keyboards produce ¢ for $) */
                GHApp.SendKeyPress((int)'$', false, false);
            }
            else
            {
                GHApp.SendKeyPress((int)ch, isCtrl, isMeta);
            }
        }

        [Export("HandleSpecialKeyInput:")]
        public void HandleSpecialKeyInput(UIKeyCommand cmd)
        {
            if (cmd?.Input == null)
                return;

            bool isCtrl = (cmd.ModifierFlags & UIKeyModifierFlags.Control) != 0;
            bool isMeta = (cmd.ModifierFlags & UIKeyModifierFlags.Alternate) != 0;
            bool isShift = (cmd.ModifierFlags & UIKeyModifierFlags.Shift) != 0;

            string input = cmd.Input;
            GHSpecialKey spkey = GHSpecialKey.None;

            if (input == UIKeyCommand.UpArrow)
                spkey = GHSpecialKey.Up;
            else if (input == UIKeyCommand.DownArrow)
                spkey = GHSpecialKey.Down;
            else if (input == UIKeyCommand.LeftArrow)
                spkey = GHSpecialKey.Left;
            else if (input == UIKeyCommand.RightArrow)
                spkey = GHSpecialKey.Right;
            else if (input == UIKeyCommand.Escape)
                spkey = GHSpecialKey.Escape;
            else if (input.Length > 0 && (int)input[0] == 13)
                spkey = GHSpecialKey.Enter;
            else if (input.Length > 0 && (int)input[0] == 9)
                spkey = GHSpecialKey.Tab;

            if (spkey != GHSpecialKey.None)
                GHApp.SendSpecialKeyPress(spkey, isCtrl, isMeta, isShift);
        }

        /* Cmd+Q handler: mirrors the Windows AppWindow.Closing logic.
         * Saves the game gracefully instead of allowing immediate termination. */
        [Export("HandleQuitKeyInput:")]
        public void HandleQuitKeyInput(UIKeyCommand cmd)
        {
            try
            {
                var curGamePage = GHApp.CurrentGamePage;
                if (curGamePage != null)
                {
                    GHApp.AddSentryBreadcrumb("HandleQuitKeyInput: Saving game before quit", GHConstants.SentryGnollHackGeneralCategoryName);
                    MainThread.InvokeOnMainThreadAsync(async () =>
                    {
                        try
                        {
#if GNH_MAUI
                            Page topPage = GHApp.PageFromTopOfModalNavigationStack();
                            if (topPage is NamePage)
                            {
                                await ((NamePage)topPage).DoPressCancel();
                            }
                            else
                            {
                                await GHApp.PopAllModalPagesAboveGamePageAsync();
                                curGamePage?.CloseMoreCommands();
                                curGamePage?.PressCharForSaving();
                            }
#endif
                        }
                        catch (Exception ex)
                        {
                            Debug.WriteLine(ex.Message);
                        }
                    });
                }
                else
                {
                    var curMainPage = GHApp.CurrentMainPage;
                    if (curMainPage != null)
                    {
                        MainThread.InvokeOnMainThreadAsync(async () =>
                        {
                            try
                            {
#if GNH_MAUI
                                await curMainPage.CheckPendingOrPressOk();
#endif
                            }
                            catch (Exception ex)
                            {
                                Debug.WriteLine(ex.Message);
                            }
                        });
                    }
                    else
                    {
                        /* No game or main page active, allow quit */
                        GHGame curGame = GHApp.CurrentGHGame;
                        curGame?.ResponseQueue.Enqueue(new GHResponse(curGame, GHRequestType.StopAllGameSounds));
                        GHApp.FmodService?.StopAllUISounds();
                        GHApp.PlatformService?.CloseApplication();
                    }
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
        }

        public override void PressesBegan(NSSet<UIPress> presses, UIPressesEvent evt)
        {
            if (presses == null || evt == null)
                return;

            bool wasHandled = false;
            int cnt = 0;

            //GHApp.MaybeWriteGHLog("PressesBegan started (" + presses.Count + ", " + evt.AllPresses.Count + ")");
            foreach (UIPress p in presses) 
            {
                cnt++;
                if (p == null)
                {
                    //GHApp.MaybeWriteGHLog("Press #" + cnt + "/" + presses.Count + " is null");
                    continue;
                }
                if (p?.Key?.CharactersIgnoringModifiers?.Length == 0)
                {
                    //GHApp.MaybeWriteGHLog("Press #" + cnt + "/" + presses.Count + " has no characters");
                    continue;
                }
                string strcharsim = p?.Key?.CharactersIgnoringModifiers;
                if (strcharsim == UIKeyCommand.UpArrow)
                {
                    wasHandled = GHApp.SendSpecialKeyPress(GHSpecialKey.Up, (evt.ModifierFlags & UIKeyModifierFlags.Control) != 0, (evt.ModifierFlags & UIKeyModifierFlags.Alternate) != 0, (evt.ModifierFlags & UIKeyModifierFlags.Shift) != 0);
                }
                else if (strcharsim == UIKeyCommand.DownArrow)
                {
                    wasHandled = GHApp.SendSpecialKeyPress(GHSpecialKey.Down, (evt.ModifierFlags & UIKeyModifierFlags.Control) != 0, (evt.ModifierFlags & UIKeyModifierFlags.Alternate) != 0, (evt.ModifierFlags & UIKeyModifierFlags.Shift) != 0);
                }
                else if (strcharsim == UIKeyCommand.LeftArrow)
                {
                    wasHandled = GHApp.SendSpecialKeyPress(GHSpecialKey.Left, (evt.ModifierFlags & UIKeyModifierFlags.Control) != 0, (evt.ModifierFlags & UIKeyModifierFlags.Alternate) != 0, (evt.ModifierFlags & UIKeyModifierFlags.Shift) != 0);
                }
                else if (strcharsim == UIKeyCommand.RightArrow)
                {
                    wasHandled = GHApp.SendSpecialKeyPress(GHSpecialKey.Right, (evt.ModifierFlags & UIKeyModifierFlags.Control) != 0, (evt.ModifierFlags & UIKeyModifierFlags.Alternate) != 0, (evt.ModifierFlags & UIKeyModifierFlags.Shift) != 0);
                }
                else if (strcharsim == UIKeyCommand.PageUp)
                {
                    wasHandled = GHApp.SendSpecialKeyPress(GHSpecialKey.PageUp, (evt.ModifierFlags & UIKeyModifierFlags.Control) != 0, (evt.ModifierFlags & UIKeyModifierFlags.Alternate) != 0, (evt.ModifierFlags & UIKeyModifierFlags.Shift) != 0);
                }
                else if (strcharsim == UIKeyCommand.PageDown)
                {
                    wasHandled = GHApp.SendSpecialKeyPress(GHSpecialKey.PageDown, (evt.ModifierFlags & UIKeyModifierFlags.Control) != 0, (evt.ModifierFlags & UIKeyModifierFlags.Alternate) != 0, (evt.ModifierFlags & UIKeyModifierFlags.Shift) != 0);
                }
                else if (strcharsim == UIKeyCommand.Home)
                {
                    wasHandled = GHApp.SendSpecialKeyPress(GHSpecialKey.Home, (evt.ModifierFlags & UIKeyModifierFlags.Control) != 0, (evt.ModifierFlags & UIKeyModifierFlags.Alternate) != 0, (evt.ModifierFlags & UIKeyModifierFlags.Shift) != 0);
                }
                else if (strcharsim == UIKeyCommand.End)
                {
                    wasHandled = GHApp.SendSpecialKeyPress(GHSpecialKey.End, (evt.ModifierFlags & UIKeyModifierFlags.Control) != 0, (evt.ModifierFlags & UIKeyModifierFlags.Alternate) != 0, (evt.ModifierFlags & UIKeyModifierFlags.Shift) != 0);
                }
                else if (strcharsim == UIKeyCommand.Escape)
                {
                    //GHApp.SendKeyPress(GHConstants.CancelChar, (evt.ModifierFlags & UIKeyModifierFlags.Control) != 0, (evt.ModifierFlags & UIKeyModifierFlags.Alternate) != 0);
                    wasHandled = GHApp.SendSpecialKeyPress(GHSpecialKey.Escape, (evt.ModifierFlags & UIKeyModifierFlags.Control) != 0, (evt.ModifierFlags & UIKeyModifierFlags.Alternate) != 0, (evt.ModifierFlags & UIKeyModifierFlags.Shift) != 0);
                }
                else
                {
                    //GHApp.MaybeWriteGHLog("Press #" + cnt + "/" + presses.Count);
                    //GHApp.MaybeWriteGHLog("Chars (" + p.Key.Characters.Length + (p.Key.Characters.Length >= 1 ? ", " + (int)p.Key.Characters[0] : "") + (p.Key.Characters.Length >= 2 ? ", " + (int)p.Key.Characters[1] : "") + "): " + p.Key.Characters);
                    //GHApp.MaybeWriteGHLog("CharsIM (" + p.Key.CharactersIgnoringModifiers.Length + (p.Key.CharactersIgnoringModifiers.Length >= 1 ? ", " + (int)p.Key.CharactersIgnoringModifiers[0] : "") + (p.Key.CharactersIgnoringModifiers.Length >= 2 ? ", " + (int)p.Key.CharactersIgnoringModifiers[1] : "") + "): " + p.Key.CharactersIgnoringModifiers);
                    string strchars = p?.Key?.Characters;
                    if(strchars?.Length > 0 && strcharsim?.Length > 0)
                    {
                        if ((int)strchars[0] == 13)
                            wasHandled = GHApp.SendSpecialKeyPress(GHSpecialKey.Enter, (evt.ModifierFlags & UIKeyModifierFlags.Control) != 0, (evt.ModifierFlags & UIKeyModifierFlags.Alternate) != 0, (evt.ModifierFlags & UIKeyModifierFlags.Shift) != 0);
                        else if ((int)strchars[0] == ' ')
                            wasHandled = GHApp.SendSpecialKeyPress(GHSpecialKey.Space, (evt.ModifierFlags & UIKeyModifierFlags.Control) != 0, (evt.ModifierFlags & UIKeyModifierFlags.Alternate) != 0, (evt.ModifierFlags & UIKeyModifierFlags.Shift) != 0);
                        else if ((int)strchars[0] == 9 && (evt.ModifierFlags & UIKeyModifierFlags.Alternate) == 0) /* Tab */
                            wasHandled = GHApp.SendSpecialKeyPress(GHSpecialKey.Tab, (evt.ModifierFlags & UIKeyModifierFlags.Control) != 0, (evt.ModifierFlags & UIKeyModifierFlags.Alternate) != 0, (evt.ModifierFlags & UIKeyModifierFlags.Shift) != 0);
                        else if ((int)strchars[0] == (int)'$' || (int)strcharsim[0] == (int)'$' || (int)strchars[0] == (int)'¢')
                            wasHandled = GHApp.SendKeyPress((int)'$', false, false);
                        else if ((evt.ModifierFlags & UIKeyModifierFlags.Control) != 0 || (evt.ModifierFlags & UIKeyModifierFlags.Alternate) != 0)
                            wasHandled = GHApp.SendKeyPress((int)strcharsim[0], (evt.ModifierFlags & UIKeyModifierFlags.Control) != 0, (evt.ModifierFlags & UIKeyModifierFlags.Alternate) != 0);
                        else
                            wasHandled = GHApp.SendKeyPress((int)strchars[0], (evt.ModifierFlags & UIKeyModifierFlags.Control) != 0, (evt.ModifierFlags & UIKeyModifierFlags.Alternate) != 0);
                    }
                }
            }
            if (!wasHandled)
                base.PressesBegan(presses, evt);
        }
    }
}