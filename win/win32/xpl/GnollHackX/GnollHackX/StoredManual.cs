using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Text;
using static System.Net.Mime.MediaTypeNames;

namespace GnollHackX
{
    public class StoredManual
    {
        public string Name;
        public int Id;
        public string Text;

        public StoredManual() 
        {
            
        }

        public StoredManual(string name, int id, string text) : this()
        {
            Name = name;
            Id = id;
            Text = text;
        }
    }

}
