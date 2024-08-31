using Foundation;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UIKit;

namespace GnollHackX.iOS
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

        public override void PressesEnded(NSSet<UIPress> presses, UIPressesEvent evt)
        {
            bool wasHandled = false;
            int cnt = 0;
            GHApp.MaybeWriteGHLog("PressesEnded started (" + presses.Count + ", " + evt.AllPresses.Count + ")");
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
                    GHApp.MaybeWriteGHLog("Press #" + cnt + "/" + presses.Count + " has no characters");
                    continue;
                }
                string strcharsim = p.Key.CharactersIgnoringModifiers;
                if (strcharsim == UIKeyCommand.UpArrow)
                {
                    GHApp.SendSpecialKeyPress(GHSpecialKey.Up, (evt.ModifierFlags & UIKeyModifierFlags.Control) != 0, (evt.ModifierFlags & UIKeyModifierFlags.Alternate) != 0, (evt.ModifierFlags & UIKeyModifierFlags.Shift) != 0);
                    wasHandled = true;
                }
                else if (strcharsim == UIKeyCommand.DownArrow)
                {
                    GHApp.SendSpecialKeyPress(GHSpecialKey.Down, (evt.ModifierFlags & UIKeyModifierFlags.Control) != 0, (evt.ModifierFlags & UIKeyModifierFlags.Alternate) != 0, (evt.ModifierFlags & UIKeyModifierFlags.Shift) != 0);
                    wasHandled = true;
                }
                else if (strcharsim == UIKeyCommand.LeftArrow)
                {
                    GHApp.SendSpecialKeyPress(GHSpecialKey.Left, (evt.ModifierFlags & UIKeyModifierFlags.Control) != 0, (evt.ModifierFlags & UIKeyModifierFlags.Alternate) != 0, (evt.ModifierFlags & UIKeyModifierFlags.Shift) != 0);
                    wasHandled = true;
                }
                else if (strcharsim == UIKeyCommand.RightArrow)
                {
                    GHApp.SendSpecialKeyPress(GHSpecialKey.Right, (evt.ModifierFlags & UIKeyModifierFlags.Control) != 0, (evt.ModifierFlags & UIKeyModifierFlags.Alternate) != 0, (evt.ModifierFlags & UIKeyModifierFlags.Shift) != 0);
                    wasHandled = true;
                }
                else if (strcharsim == UIKeyCommand.Escape)
                {
                    //GHApp.SendKeyPress(GHConstants.CancelChar, (evt.ModifierFlags & UIKeyModifierFlags.Control) != 0, (evt.ModifierFlags & UIKeyModifierFlags.Alternate) != 0);
                    GHApp.SendSpecialKeyPress(GHSpecialKey.Escape, (evt.ModifierFlags & UIKeyModifierFlags.Control) != 0, (evt.ModifierFlags & UIKeyModifierFlags.Alternate) != 0, (evt.ModifierFlags & UIKeyModifierFlags.Shift) != 0);
                    wasHandled = true;
                }
                else
                {
                    GHApp.MaybeWriteGHLog("Press #" + cnt + "/" + presses.Count);
                    GHApp.MaybeWriteGHLog("Chars (" + p.Key.Characters.Length + (p.Key.Characters.Length >= 1 ? ", " + (int)p.Key.Characters[0] : "") + (p.Key.Characters.Length >= 2 ? ", " + (int)p.Key.Characters[1] : "") + "): " + p.Key.Characters);
                    GHApp.MaybeWriteGHLog("CharsIM (" + p.Key.CharactersIgnoringModifiers.Length + (p.Key.CharactersIgnoringModifiers.Length >= 1 ? ", " + (int)p.Key.CharactersIgnoringModifiers[0] : "") + (p.Key.CharactersIgnoringModifiers.Length >= 2 ? ", " + (int)p.Key.CharactersIgnoringModifiers[1] : "") + "): " + p.Key.CharactersIgnoringModifiers);
                    string strchars = p.Key.Characters;
                    if(strchars?.Length > 0 && strcharsim?.Length > 0)
                    {
                        if ((int)strchars[0] == 13)
                            GHApp.SendSpecialKeyPress(GHSpecialKey.Enter, (evt.ModifierFlags & UIKeyModifierFlags.Control) != 0, (evt.ModifierFlags & UIKeyModifierFlags.Alternate) != 0, (evt.ModifierFlags & UIKeyModifierFlags.Shift) != 0);
                        else if ((int)strchars[0] == (int)'$' || (int)strcharsim[0] == (int)'$' || (int)strchars[0] == (int)'¢')
                            GHApp.SendKeyPress((int)'$', false, false);
                        else if ((evt.ModifierFlags & UIKeyModifierFlags.Control) != 0 || (evt.ModifierFlags & UIKeyModifierFlags.Alternate) != 0)
                            GHApp.SendKeyPress((int)strcharsim[0], (evt.ModifierFlags & UIKeyModifierFlags.Control) != 0, (evt.ModifierFlags & UIKeyModifierFlags.Alternate) != 0);
                        else
                            GHApp.SendKeyPress((int)strchars[0], (evt.ModifierFlags & UIKeyModifierFlags.Control) != 0, (evt.ModifierFlags & UIKeyModifierFlags.Alternate) != 0);
                    }
                    wasHandled = true;
                }
            }
            if (!wasHandled)
                base.PressesEnded(presses, evt);
        }
    }
}