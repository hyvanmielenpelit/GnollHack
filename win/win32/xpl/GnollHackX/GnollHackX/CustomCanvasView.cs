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

    /* The gamepad hint strips; a type of its own so that Windows RenderSurfaceDiagnostics names their surfaces apart */
    public class GamepadHintCanvasView : SKCanvasView
    {
    }
}
