using GnollHackCommon;
using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackClient
{
    public class ObjectDataItem
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
        public bool Poisoned
        {
            get
            {
                return OtypData.poisoned != 0;
            }
        }
        public byte Eroded
        {
            get
            {
                return OtypData.eroded;
            }
        }
        public byte Eroded2
        {
            get
            {
                return OtypData.eroded2;
            }
        }
        public byte ElementalEnchantment
        {
            get
            {
                return ObjData.elemental_enchantment;
            }
        }
        public byte MythicPrefix
        {
            get
            {
                return ObjData.mythic_prefix;
            }
        }
        public byte MythicSuffix
        {
            get
            {
                return ObjData.mythic_suffix;
            }
        }
        public byte Exceptionality
        {
            get
            {
                return ObjData.exceptionality;
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
