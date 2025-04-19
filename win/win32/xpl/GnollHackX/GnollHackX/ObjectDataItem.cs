using System;
using System.Collections.Generic;
using System.Text;
#if GNH_MAUI
using GnollHackM;
#endif

namespace GnollHackX
{
    public class ObjectDataItem
    {
        public readonly Obj ObjData;
        public readonly ObjClassData OtypData;
        private readonly List<ObjectDataItem> ContainedObjs;

        public bool HasContainedObjs { get { return ContainedObjs != null && ContainedObjs.Count > 0; } }
        public IEnumerable<ObjectDataItem> ContainedObjsEnumerable { get { return ContainedObjs?.AsEnumerable(); } }

        public ObjectDataItem CloneWithAddedContainedObj(ObjectDataItem addedContainedObj)
        {
            List<ObjectDataItem> containedItems = null;
            if (addedContainedObj != null || ContainedObjs != null)
            {
                containedItems = new List<ObjectDataItem>(16);
                if(ContainedObjs != null)
                    containedItems.AddRange(ContainedObjs);
                if(addedContainedObj != null)
                    containedItems.Add(addedContainedObj);
            }
            ObjectDataItem clone = new ObjectDataItem(ObjData, OtypData, Hallucinated, OutOfAmmo, WrongAmmoType, NotBeingUsed, NotWeapon, FoundThisTurn, IsAmmo, IsThrowingWeapon, PreviousWeaponFound, PreviousUnwield, containedItems);
            return clone;
        }

        public int ContainedObjectCount { get { return ContainedObjs.Count; } }
        public ObjectDataItem GetContainedObjectByIndex(int index) => ContainedObjs[index];

        public ObjectDataItem Clone()
        {
            return CloneWithAddedContainedObj(null);
        }

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

        public readonly bool Hallucinated;
        public readonly bool OutOfAmmo;
        public readonly bool WrongAmmoType;
        public readonly bool NotBeingUsed;
        public readonly bool NotWeapon;
        public readonly bool FoundThisTurn;
        public readonly bool IsAmmo;
        public readonly bool IsThrowingWeapon;
        public readonly bool PreviousWeaponFound;
        public readonly bool PreviousUnwield;

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
        public ObjectDataItem(Obj objData, ObjClassData otypdata, bool hallucinated, bool outofammo, bool wrongammo, bool notbeingused, bool notweapon, bool foundthisturn, bool isammo, bool isthrowingweapon, bool prevwepfound, bool prevunwield, List<ObjectDataItem> containedItems)
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
            ContainedObjs = containedItems;
        }
    }
}
