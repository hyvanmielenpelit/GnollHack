using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackX
{
    public class GHMouseWheelEventArgs : EventArgs
    {
        int _mouseWheelDelta = 0;
        public GHMouseWheelEventArgs(int mouseWheelDelta) : base()
        {
            _mouseWheelDelta = mouseWheelDelta;
        }
        public int MouseWheelDelta { get { return _mouseWheelDelta; } }
    }
}
