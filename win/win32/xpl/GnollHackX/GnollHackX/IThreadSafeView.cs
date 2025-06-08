using SkiaSharp;
using System;
using System.Collections.Generic;
using System.Text;

#if GNH_MAUI
namespace GnollHackM
#else
using Xamarin.Forms;

namespace GnollHackX
#endif
{
    public interface IThreadSafeView
    {
        double ThreadSafeWidth { get; }
        double ThreadSafeHeight { get; }
        double ThreadSafeX { get; }
        double ThreadSafeY { get; }
        bool ThreadSafeIsVisible { get; }
        Thickness ThreadSafeMargin { get; }
        WeakReference<IThreadSafeView> ThreadSafeParent { get; }
    }
}
