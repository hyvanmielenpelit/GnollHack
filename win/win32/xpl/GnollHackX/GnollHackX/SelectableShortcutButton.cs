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
        public char MappedLetter => (char)GHUtils.UnMetaCtrl(MappedGHCommand);
        public bool MappedCtrl => GHUtils.IsCtrl(MappedGHCommand);
        public bool MappedMeta => GHUtils.IsMeta(MappedGHCommand);
        public int MappedRawCommand => GHApp.MapCommand(RawCommand);
        public string ImageSourcePath { get; private set; }

        public readonly int GHCommand;
        public int MappedGHCommand => GHApp.MapCommand(GHCommand);

        public SelectableShortcutButton(string name, string label, char letter, bool ctrl, bool meta, int cmd, string imgsourcepath) 
        {
            Name = name;
            Label = label;
            Letter = letter;
            Ctrl = ctrl;
            Meta = meta;
            RawCommand = cmd;
            ImageSourcePath = imgsourcepath;
            GHCommand = CalculateGHCommand();
        }

        private int CalculateGHCommand()
        {
            if (RawCommand != 0)
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
