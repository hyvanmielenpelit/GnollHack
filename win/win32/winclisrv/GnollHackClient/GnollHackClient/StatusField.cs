using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackClient
{
    public class StatusField
    {
        public int FieldIndex { get; set; } // field index
        public bool Enabled { get; set; } // whether the field is enabled
        public string Name { get; set; } // name of status field
        public string Format { get; set; } // format of field
        public bool SpaceInFront { get; set; } // add a space in front of the field

        public int Percent { get; set; }
        public int Color { get; set; }
        public int Attribute { get; set; }
        public string Text { get; set; }

        public StatusField()
        {

        }
    }
}
