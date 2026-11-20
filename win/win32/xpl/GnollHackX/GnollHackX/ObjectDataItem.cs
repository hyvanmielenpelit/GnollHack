using System;
using System.Collections.Generic;
using System.Text;
#if GNH_MAUI
using GnollHackM;
#endif

namespace GnollHackX
{
    public sealed class ObjectDataItem
    {
        public readonly Obj ObjData;
        public readonly ObjClassData OtypData;
        private readonly List<ObjectDataItem> ContainedObjs;

        public bool HasContainedObjs { get { return ContainedObjs != null && ContainedObjs.Count > 0; } }
        public IEnumerable<ObjectDataItem> ContainedObjsEnumerable { get { return ContainedObjs; } }

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
        public bool IsWorn
        {
            get
            {
                return ObjData.owornmask != 0;
            }
        }
        public long WornBits
        {
            get
            {
                return ObjData.owornmask;
            }
        }
        public bool IsArmor
        {
            get
            {
                return ObjData.oclass == (sbyte)obj_class_types.ARMOR_CLASS;
            }
        }
        public sbyte ArmorType
        {
            get
            {
                return ObjData.oclass == (sbyte)obj_class_types.ARMOR_CLASS ? OtypData.oc_subtyp : (sbyte)-1;
            }
        }
        public sbyte ClassType
        {
            get
            {
                return ObjData.oclass;
            }
        }

        public bool IsShield
        {
            get
            {
                return ObjData.oclass == (sbyte)obj_class_types.ARMOR_CLASS && OtypData.oc_subtyp == (sbyte)obj_armor_types.ARM_SHIELD;
            }
        }
        public bool IsWieldedWeapon
        {
            get
            {
                return (OtypData.ocdata_flags & (byte)ocdata_flag_types.OCDATA_WIELDED_WEAPON) != 0;
            }
        }
        public bool IsQuiverable
        {
            get
            {
                return (OtypData.ocdata_flags & (byte)ocdata_flag_types.OCDATA_QUIVERABLE) != 0;
            }
        }
        public bool IsBlindfold
        {
            get
            {
                return (OtypData.ocdata_flags & (byte)ocdata_flag_types.OCDATA_BLINDFOLD) != 0;
            }
        }
        public bool IsBimanual
        {
            get
            {
                return (OtypData.ocdata_flags & (byte)ocdata_flag_types.OCDATA_BIMANUAL) != 0;
            }
        }
        public bool IsAppliablePolearm
        {
            get
            {
                return (OtypData.ocdata_flags & (byte)ocdata_flag_types.OCDATA_APPLIABLE_POLEARM) != 0;
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
        public readonly bool TwinkleEligible;
        public readonly bool TwinkleAlways;
        public readonly bool TwinkleKnown;

        /* By reference, so that the large Obj and ObjClassData structs are not copied; fields only, since a
           property read through an in parameter of a non-readonly struct copies the struct */
        private static void GetTwinkleFlags(in Obj objData, in ObjClassData otypdata, bool hallucinated, out bool eligible, out bool always, out bool known)
        {
            bool dknown = (objData.bitflags & (ulong)obj_bitfield_types.OBJFLAG_DKNOWN) != 0;
            /* Gem-class items have their own rule, which keeps metal ores out */
            bool special = GHItemTwinkle.IsSpecialQuality(objData.elemental_enchantment, objData.mythic_prefix, objData.mythic_suffix, objData.exceptionality)
                || (objData.oclass != (sbyte)obj_class_types.GEM_CLASS && GHItemTwinkle.IsAlwaysTwinkleMaterial(objData.material));
            eligible = GHItemTwinkle.IsEligible(objData.oclass, objData.material, hallucinated,
                dknown, otypdata.is_uchain != 0 || otypdata.is_uball != 0, special);
            always = GHItemTwinkle.IsAlwaysTwinkle(objData.oclass, objData.material, special);
            known = dknown && GHItemTwinkle.DknownIsMeaningful(objData.oclass);
        }

        public ObjectDataItem()
        {

        }
        public ObjectDataItem(in Obj objData, in ObjClassData otypdata, bool hallucinated)
        {
            ObjData = objData;
            OtypData = otypdata;
            Hallucinated = hallucinated;
            GetTwinkleFlags(in objData, in otypdata, hallucinated, out TwinkleEligible, out TwinkleAlways, out TwinkleKnown);
        }
        public ObjectDataItem(in Obj objData, in ObjClassData otypdata, bool hallucinated, bool foundthisturn)
        {
            ObjData = objData;
            OtypData = otypdata;
            Hallucinated = hallucinated;
            GetTwinkleFlags(in objData, in otypdata, hallucinated, out TwinkleEligible, out TwinkleAlways, out TwinkleKnown);
            FoundThisTurn = foundthisturn;
        }
        public ObjectDataItem(in Obj objData, in ObjClassData otypdata, bool hallucinated, bool outofammo, bool wrongammo, bool notbeingused,  bool notweapon, bool foundthisturn, bool isammo, bool isthrowingweapon, bool prevwepfound, bool prevunwield)
        {
            ObjData = objData;
            OtypData = otypdata;
            Hallucinated = hallucinated;
            GetTwinkleFlags(in objData, in otypdata, hallucinated, out TwinkleEligible, out TwinkleAlways, out TwinkleKnown);
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
        public ObjectDataItem(in Obj objData, in ObjClassData otypdata, bool hallucinated, bool outofammo, bool wrongammo, bool notbeingused, bool notweapon, bool foundthisturn, bool isammo, bool isthrowingweapon, bool prevwepfound, bool prevunwield, List<ObjectDataItem> containedItems)
        {
            ObjData = objData;
            OtypData = otypdata;
            Hallucinated = hallucinated;
            GetTwinkleFlags(in objData, in otypdata, hallucinated, out TwinkleEligible, out TwinkleAlways, out TwinkleKnown);
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
