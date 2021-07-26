using GnollHackCommon;
using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackClient
{
    class ObjectDataItem
    {
        public obj ObjData;
        public List<ObjectDataItem> ContainedObjs;
        private short _tileHeight = 0;
        public short TileHeight {
            get
            {
                return Hallucinated ? (short)0 : _tileHeight;
            }
        }

        public bool Hallucinated { get; set; }

        public ObjectDataItem()
        {

        }
        public ObjectDataItem(obj objData, int tile_height, bool hallucinated)
        {
            ObjData = objData;
            _tileHeight = (short)tile_height;
            Hallucinated = hallucinated;
        }
    }
}
