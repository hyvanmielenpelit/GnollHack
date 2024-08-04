using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackX
{
    public class GHUserData
    {
        public Dictionary<string, string> StringDictionary = new Dictionary<string, string>();
        public Dictionary<string, bool> BoolDictionary = new Dictionary<string, bool>();
        public Dictionary<string, int> IntDictionary = new Dictionary<string, int>();
        public Dictionary<string, long> LongDictionary = new Dictionary<string, long>();
        public Dictionary<string, double> DoubleDictionary = new Dictionary<string, double>();
        public Dictionary<string, DateTime> DateTimeDictionary = new Dictionary<string, DateTime>();
        public GHUserData() 
        {

        }

        public void Clear()
        {
            StringDictionary.Clear();
            BoolDictionary.Clear();
            IntDictionary.Clear();
            LongDictionary.Clear();
            DoubleDictionary.Clear();
            DateTimeDictionary.Clear();
        }
    }
}
