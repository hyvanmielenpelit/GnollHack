using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackX
{
    public class ObjectDataItem
    {
        public Obj ObjData;
        public ObjClassData OtypData;
        public List<ObjectDataItem> ContainedObjs;

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
        public bool OutOfAmmo { get; set; }
        public bool WrongAmmoType { get; set; }
        public bool NotBeingUsed { get; set; }
        public bool NotWeapon { get; set; }
        public bool FoundThisTurn { get; set; }
        public bool IsAmmo { get; set; }
        public bool IsThrowingWeapon { get; set; }
        public bool PreviousWeaponFound { get; set; }
        public bool PreviousUnwield { get; set; }

        public ObjectDataItem()
        {

        }
        public ObjectDataItem(Obj objData, ObjClassData otypdata, bool hallucinated)
        {
            ObjData = objData;
            OtypData = otypdata;
            Hallucinated = hallucinated;
        }
        public ObjectDataItem(Obj objData, ObjClassData otypdata, bool hallucinated, bool foundthisturn)
        {
            ObjData = objData;
            OtypData = otypdata;
            Hallucinated = hallucinated;
            FoundThisTurn = foundthisturn;
        }
        public ObjectDataItem(Obj objData, ObjClassData otypdata, bool hallucinated, bool outofammo, bool wrongammo, bool notbeingused,  bool notweapon, bool foundthisturn, bool isammo, bool isthrowingweapon, bool prevwepfound, bool prevunwield)
        {
            ObjData = objData;
            OtypData = otypdata;
            Hallucinated = hallucinated;
            OutOfAmmo = outofammo;
            WrongAmmoType = wrongammo;
            NotBeingUsed = notbeingused;
            NotWeapon = notweapon;
            FoundThisTurn = foundthisturn;
            IsAmmo = isammo;
            IsThrowingWeapon = isthrowingweapon;
            PreviousWeaponFound = prevwepfound;
            PreviousUnwield = prevunwield;
        }
    }
}
