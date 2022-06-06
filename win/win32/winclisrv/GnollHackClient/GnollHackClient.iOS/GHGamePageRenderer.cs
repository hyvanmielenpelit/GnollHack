using Foundation;
using GnollHackClient.iOS;
using GnollHackClient.Pages.Game;
using GnollHackCommon;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UIKit;
using Xamarin.Forms;
using Xamarin.Forms.Platform.iOS;

[assembly: ExportRenderer(typeof(GamePage), typeof(GHGamePageRenderer))]
namespace GnollHackClient.iOS
{
    public class GHGamePageRenderer : PageRenderer
    {
        public override bool CanBecomeFirstResponder
        {
            get { return true; }
        }

        protected override void OnElementChanged(VisualElementChangedEventArgs e)
        {
            base.OnElementChanged(e);
            var selector = new ObjCRuntime.Selector("KeyPressedNormal:");
            UIKeyCommand accelerator1 = UIKeyCommand.Create((NSString)"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890<>!\"#$%&/()=+?,.-;:_*{}[]\'\\ §", 0, selector);
            AddKeyCommand(accelerator1);
            var selector2 = new ObjCRuntime.Selector("KeyPressedCtrl:");
            UIKeyCommand accelerator2 = UIKeyCommand.Create((NSString)"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890<>!\"#$%&/()=+?,.-;:_*{}[]'\\ §", UIKeyModifierFlags.Control, selector2);
            AddKeyCommand(accelerator2);
            var selector3 = new ObjCRuntime.Selector("KeyPressedMeta:");
            UIKeyCommand accelerator3 = UIKeyCommand.Create((NSString)"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890<>!\"#$%&/()=+?,.-;:_*{}[]'\\ §", UIKeyModifierFlags.Alternate, selector3);
            AddKeyCommand(accelerator3);
            var selector4 = new ObjCRuntime.Selector("KeyPressedNumberPad:");
            UIKeyCommand accelerator4 = UIKeyCommand.Create((NSString)"1234567890", UIKeyModifierFlags.NumericPad, selector4);
            AddKeyCommand(accelerator4);
        }

        [Export("KeyPressedNormal:")]
        public void KeyPressedNormal(UIKeyCommand cmd)
        {
            if (cmd == null)
                return;
            string inputValue = cmd.Input;
            if (inputValue == null || inputValue.Length < 1)
                return;

            ((GamePage)Element)?.GenericButton_Clicked(this, new EventArgs(), (int)inputValue[0]);
        }

        [Export("KeyPressedCtrl:")]
        public void KeyPressedCtrl(UIKeyCommand cmd)
        {
            if (cmd == null)
                return;
            string inputValue = cmd.Input;
            if (inputValue == null || inputValue.Length < 1)
                return;

            ((GamePage)Element)?.GenericButton_Clicked(this, new EventArgs(), GHUtils.Ctrl((int)inputValue[0]));
        }

        [Export("KeyPressedMeta:")]
        public void KeyPressedMeta(UIKeyCommand cmd)
        {
            if (cmd == null)
                return;
            string inputValue = cmd.Input;
            if (inputValue == null || inputValue.Length < 1)
                return;

            ((GamePage)Element)?.GenericButton_Clicked(this, new EventArgs(), GHUtils.Meta((int)inputValue[0]));
        }

        [Export("KeyPressedNumberPad:")]
        public void KeyPressedNumberPad(UIKeyCommand cmd)
        {
            if (cmd == null)
                return;
            string inputValue = cmd.Input;
            if (inputValue == null || inputValue.Length < 1)
                return;
            if (inputValue[0] < '0' || inputValue[0] > '9')
                return;

            ((GamePage)Element)?.GenericButton_Clicked(this, new EventArgs(), -1 * (int)(inputValue[0] - '0'));
        }
    }
}