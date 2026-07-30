using GnollHackM;
using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackX
{
    public sealed class GHWindowRow
    {
        private readonly GHWindow _window;

        public StringBuilder TextStringBuilder { get; } = new StringBuilder(GHConstants.MapCols);
        
        public List<int> AttributeList { get; } = new List<int>();
        public List<int> ColorList { get; } = new List<int>();

        public GHWindow Window { get { return _window; } }

        public GHWindowRow(GHWindow window)
        {
            _window = window;
        }

        public string Text
        {
            get { return TextStringBuilder.ToString(); }
        }
    }
}
