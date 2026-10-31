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
            // WORKAROUND(WA-MAUI-0029): Shell shows its navigation bar at startup before hiding it for the first page
            SetNavigationBarHidden(true, false);
        }
    }
}
