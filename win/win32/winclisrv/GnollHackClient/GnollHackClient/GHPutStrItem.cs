using GnollHackCommon;
using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackClient
{
    public class GHPutStrItem
    {
        private string _text;
        public string Text { get { return _text; } 
            set 
            { 
                _text = value; 
                if (_instructionList.Count < _text.Length)
                {
                    _instructionList.Add(new GHPutStrInstructions(0, (int)nhcolor.CLR_WHITE, _text.Length - _instructionList.Count));
                }
            }
        }
        private List<GHPutStrInstructions> _instructionList;
        public List<GHPutStrInstructions> InstructionList { get { return _instructionList; } set { _instructionList = value; } }

        public GHPutStrItem(string str)
        {
            _instructionList = new List<GHPutStrInstructions>();
            Text = str;
        }
        public GHPutStrItem(string str, List<GHPutStrInstructions> instrlist)
        {
            InstructionList = instrlist;
            Text = str;
        }
        public GHPutStrItem(string str, List<int> attrlist, List<int> colorlist)
        {
            InstructionList = ConvertListFromArrays(attrlist, colorlist);
            Text = str;
        }
        public List<GHPutStrInstructions> ConvertListFromArrays(List<int> attrlist, List<int> colorlist)
        {
            List <GHPutStrInstructions> res = new List<GHPutStrInstructions>();
            int prevattr = 0, prevclr = 0;
            int curattr = 0, curclr = 0;
            int cnt = 0;

            for (int i = 0; i < Text.Length; i++)
            {
                prevattr = curattr;
                prevclr = curclr;
                cnt++;
                curattr = i < attrlist.Count ? attrlist[i] : 0;
                curclr = i < colorlist.Count ? colorlist[i] : (int)nhcolor.CLR_WHITE;

                if(cnt > 1 && (curattr != prevattr || curclr != prevclr))
                {
                    res.Add(new GHPutStrInstructions(prevattr, prevclr, cnt - 1));
                    cnt = 1;
                }
                else if(i == Text.Length - 1)
                {
                    res.Add(new GHPutStrInstructions(curattr, curclr, cnt));
                }
            }
            return res;
        }

        public void ConvertListToArrays(List<GHPutStrInstructions> instrlist, out List<int> attrlist, out List<int> colorlist)
        {
            attrlist = new List<int>();
            colorlist = new List<int>();
            foreach(GHPutStrInstructions instr in instrlist)
            {
                for(int i = 0; i < instr.PrintLength; i++)
                {
                    attrlist.Add(instr.Attributes);
                    colorlist.Add(instr.Color);
                }
            }
        }
        public void ConvertCurrentListToArrays(out List<int> attrlist, out List<int> colorlist)
        {
            ConvertListToArrays(InstructionList, out attrlist, out colorlist);
        }
        public void ConvertCurrentListFromArrays(List<int> attrlist, List<int> colorlist)
        {
            InstructionList = ConvertListFromArrays(attrlist, colorlist);
        }
    }
}
