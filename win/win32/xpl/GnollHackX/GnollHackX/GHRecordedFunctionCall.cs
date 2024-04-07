using System;
using System.Collections.Generic;
using System.Runtime.Serialization;
using System.Text;
#if GNH_MAUI
using GnollHackM;
#endif

namespace GnollHackX
{
    public class GHRecordedFunctionCall
    {
        RecordedFunctionID _recordedFunctionID;
        object[] _args;
        DateTime _time;

        public RecordedFunctionID RecordedFunctionID { get { return _recordedFunctionID; } }
        public object[] Args { get { return _args; } }
        public DateTime Time { get { return _time; } }

        public GHRecordedFunctionCall(RecordedFunctionID recordedFunctionID, object[] args, DateTime time) 
        {
            _recordedFunctionID = recordedFunctionID;
            _args = args;
            _time = time;
        }
    }
}
