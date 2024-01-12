using System;
using System.Collections.Generic;
using System.Runtime.Serialization;
using System.Text;

namespace GnollHackX
{
    public class GHRecordedFunctionCall
    {
        RecordedFunctionID _recordedFunctionID;
        object[] _args;
        DateTime _time;
        public GHRecordedFunctionCall(RecordedFunctionID recordedFunctionID, object[] args, DateTime time) 
        {
            _recordedFunctionID = recordedFunctionID;
            _args = args;
            _time = time;
        }
    }
}
