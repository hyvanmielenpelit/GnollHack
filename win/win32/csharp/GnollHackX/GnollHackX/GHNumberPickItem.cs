using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackX
{
    public class GHNumberPickItem
    {
        public int Number { get; set; }
        public string Name { get; set; }
        public GHNumberPickItem()
        {

        }
        public GHNumberPickItem(int number)
        {
            Number = number;
            Name = number.ToString();
        }
        public GHNumberPickItem(int number, string name)
        {
            Number = number;
            Name = name;
        }
    }
}
