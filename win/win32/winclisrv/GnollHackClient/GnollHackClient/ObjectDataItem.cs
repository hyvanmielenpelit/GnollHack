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
        public short TileHeight;

        public ObjectDataItem()
        {

        }
        public ObjectDataItem(obj objData, int tile_height)
        {
            ObjData = objData;
            TileHeight = (short)tile_height;
        }
    }
}
