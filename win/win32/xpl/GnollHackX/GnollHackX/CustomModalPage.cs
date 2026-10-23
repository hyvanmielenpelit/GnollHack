using System;
using System.Collections.Generic;
using System.Text;

#if GNH_MAUI
using GnollHackX;

namespace GnollHackM
#else
using Xamarin.Forms;
using Xamarin.Forms.PlatformConfiguration;
using Xamarin.Forms.PlatformConfiguration.iOSSpecific;

namespace GnollHackX
#endif
{
    public partial class CustomModalPage : CustomContentPage
    {
        public CustomModalPage() : base()
        {

        }
    }
}
