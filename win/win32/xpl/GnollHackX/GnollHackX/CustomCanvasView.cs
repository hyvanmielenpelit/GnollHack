using SkiaSharp;
using System;
using System.Collections.Generic;
using System.Text;

#if GNH_MAUI
using SkiaSharp.Views.Maui.Controls;
namespace GnollHackM
#else
using SkiaSharp.Views.Forms;

namespace GnollHackX
#endif
{
    public class CustomCanvasView : SKCanvasView
    {
        public CustomCanvasView() : base()
        {
        }
    }
}
