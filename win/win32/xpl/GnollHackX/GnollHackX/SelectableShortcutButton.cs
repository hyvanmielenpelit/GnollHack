using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackX
{
    public class SelectableShortcutButton
    {
        public string Name { get; private set; }
        public string Label { get; private set; }
        public char Letter { get; private set; }
        public bool Ctrl { get; private set; }
        public bool Meta { get; private set; }
        public int RawCommand { get; private set; }
        public string ImageSourcePath { get; private set; }

        public SelectableShortcutButton(string name, string label, char letter, bool ctrl, bool meta, int cmd, string imgsourcepath) 
        {
            Name = name;
            Label = label;
            Letter = letter;
            Ctrl = ctrl;
            Meta = meta;
            RawCommand = cmd;
            ImageSourcePath = imgsourcepath;
        }

        public int GetCommand()
        {
            if(RawCommand != 0)
                return RawCommand;
            else if (Ctrl)
                return GHUtils.Ctrl(Letter);
            else if (Meta)
                return GHUtils.Meta(Letter);
            else
                return (int)Letter;
        }
    }
}
