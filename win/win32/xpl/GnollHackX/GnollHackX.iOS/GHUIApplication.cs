using Foundation;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UIKit;

#if GNH_MAUI
using GnollHackX;

namespace GnollHackM.Platforms.iOS
#else
namespace GnollHackX.iOS
#endif
{
    public class GHUIApplication : UIApplication
    {
        public GHUIApplication() : base()
        {

        }

        public GHUIApplication(IntPtr handle) : base(handle)
        {

        }

        public GHUIApplication(Foundation.NSObjectFlag t) : base(t)
        {

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