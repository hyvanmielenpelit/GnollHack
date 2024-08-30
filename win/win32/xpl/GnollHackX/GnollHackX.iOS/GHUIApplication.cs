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
            foreach (UIPress p in presses) 
            {
                if(p.Key.CharactersIgnoringModifiers == UIKeyCommand.UpArrow)
                {
                    GHApp.SendKeyPress(-8, (evt.ModifierFlags & UIKeyModifierFlags.Control) != 0, (evt.ModifierFlags & UIKeyModifierFlags.Alternate) != 0);
                    wasHandled = true;
                }
                else if (p.Key.CharactersIgnoringModifiers == UIKeyCommand.DownArrow)
                {
                    GHApp.SendKeyPress(-2, (evt.ModifierFlags & UIKeyModifierFlags.Control) != 0, (evt.ModifierFlags & UIKeyModifierFlags.Alternate) != 0);
                    wasHandled = true;
                }
                else if (p.Key.CharactersIgnoringModifiers == UIKeyCommand.LeftArrow)
                {
                    GHApp.SendKeyPress(-4, (evt.ModifierFlags & UIKeyModifierFlags.Control) != 0, (evt.ModifierFlags & UIKeyModifierFlags.Alternate) != 0);
                    wasHandled = true;
                }
                else if (p.Key.CharactersIgnoringModifiers == UIKeyCommand.RightArrow)
                {
                    GHApp.SendKeyPress(-6, (evt.ModifierFlags & UIKeyModifierFlags.Control) != 0, (evt.ModifierFlags & UIKeyModifierFlags.Alternate) != 0);
                    wasHandled = true;
                }
                else if (p.Key.CharactersIgnoringModifiers == UIKeyCommand.Escape)
                {
                    GHApp.SendKeyPress(GHConstants.CancelChar, (evt.ModifierFlags & UIKeyModifierFlags.Control) != 0, (evt.ModifierFlags & UIKeyModifierFlags.Alternate) != 0);
                    wasHandled = true;
                }
                else
                {
                    string str = p.Key.CharactersIgnoringModifiers;
                    GHApp.SendKeyPress(str?.Length > 0 ? (int)str[0] : 0, (evt.ModifierFlags & UIKeyModifierFlags.Control) != 0, (evt.ModifierFlags & UIKeyModifierFlags.Alternate) != 0);
                    wasHandled = true;
                }
            }
            if (!wasHandled)
                base.PressesEnded(presses, evt);
        }
    }
}