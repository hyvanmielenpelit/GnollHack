using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GnollHackM.Platforms.Windows
{
    public partial class CustomButtonHandler : Microsoft.Maui.Handlers.ButtonHandler
    {
        protected override void ConnectHandler(Microsoft.UI.Xaml.Controls.Button platformView)
        {
            base.ConnectHandler(platformView);
            platformView.IsTabStop = false;
        }
    }
}
