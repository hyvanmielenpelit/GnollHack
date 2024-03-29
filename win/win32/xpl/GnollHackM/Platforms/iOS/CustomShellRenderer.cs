using Microsoft.Maui.Controls.Handlers.Compatibility;
using Microsoft.Maui.Controls.Platform.Compatibility;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GnollHackM.Platforms.iOS
{
    public class CustomShellRenderer : ShellRenderer
    {
        public CustomShellRenderer()
        {

        }
        protected override IShellSectionRenderer CreateShellSectionRenderer(ShellSection shellSection)
        {
            return new CustomSectionRenderer(this);
        }
    }

    public class CustomSectionRenderer : ShellSectionRenderer
    {
        public CustomSectionRenderer(IShellContext context) : base(context)
        {

        }
        public override void ViewDidLoad()
        {
            base.ViewDidLoad();
            InteractivePopGestureRecognizer.Enabled = false;
        }
    }
}
