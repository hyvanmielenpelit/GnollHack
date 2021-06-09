using System;
using System.Collections.Generic;
using System.Text;
using GnollHackCommon;

namespace GnollHackClient
{
    public class StatusLine
    {
        private StatusString[] _statusStrings = new StatusString[GHConstants.MaxStatusFieldsPerLine];
        private StatusField[] _statusFields = new StatusField[GHConstants.MaxStatusFieldsPerLine];

        public StatusLine()
        {

        }
    }
}
