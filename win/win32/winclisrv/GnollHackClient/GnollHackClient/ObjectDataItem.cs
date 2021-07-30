using GnollHackCommon;
using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackClient
{
    class ObjectDataItem
    {
        public obj ObjData;
        public objclassdata OtypData;
        public List<ObjectDataItem> ContainedObjs;
        private short _tileHeight = 0;
        public short TileHeight {
            get
            {
                return Hallucinated ? (short)0 : (short)OtypData.tile_height;
            }
        }
        public bool LampLit
        {
            get
            {
                return OtypData.lamplit != 0;
            }
        }
        public int OtypSpecialQuality
        {
            get
            {
                return OtypData.special_quality;
            }
        }

        public bool Hallucinated { get; set; }

        public ObjectDataItem()
        {

        }
        public ObjectDataItem(obj objData, objclassdata otypdata, bool hallucinated)
        {
            ObjData = objData;
            OtypData = otypdata;
            Hallucinated = hallucinated;
        }
    }
}
