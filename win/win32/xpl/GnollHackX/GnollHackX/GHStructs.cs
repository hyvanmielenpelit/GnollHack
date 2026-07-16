using System;
using System.Collections.Generic;
using System.Globalization;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;
#if GNH_MAUI
using Microsoft.Maui.Controls;
namespace GnollHackM
#else
using Xamarin.Forms;
namespace GnollHackX
#endif
{
    public struct AnimationDefinition
    {
        public IntPtr animation_name;
        public int animation_type;
        public sbyte number_of_tiles;
        public sbyte number_of_frames;
        public sbyte number_of_tile_animations;
        public sbyte intervals_between_frames;
        public int play_type;
        public int main_tile_use_style;
        public int main_tile_autodraw;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = GHConstants.MaxFramesPerAnimation)]
        public sbyte[] frame2tile;
        public sbyte sound_play_frame;
        public sbyte action_execution_frame;
        public short tile_enlargement;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = GHConstants.MaxFramesPerAnimation)]
        public int[] frame_autodraw;
    }

    public struct EnlargementDefinition
    {
        public IntPtr enlargement_name;
        public sbyte number_of_animation_tiles;
        public sbyte number_of_animation_frames;
        public sbyte number_of_enlargement_tiles;
        public sbyte width_in_tiles;
        public sbyte height_in_tiles;
        public sbyte main_tile_x_coordinate; /* Always 0 or 1 on the last row */
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = GHConstants.NumPositionsInEnlargement)]
        public sbyte[] position2tile;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = GHConstants.NumPositionsInEnlargement)]
        public byte[] position_flags;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = GHConstants.NumPositionsInEnlargement)]
        public int[] position_autodraw;
    }

    public struct ReplacementDefinition
    {
        public IntPtr replacement_name;
        public sbyte number_of_tiles;
        public ulong replacement_events;
        public int replacement_action; /* hard-coded - defines which tile to use and when */
        public int general_autodraw; /* For zero-tile replacements */
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = GHConstants.MaxTilesPerReplacement)]
        public IntPtr[] tile_names; /* Not needed */
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = GHConstants.MaxTilesPerReplacement)]
        public short[] tile_animation;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = GHConstants.MaxTilesPerReplacement)]
        public short[] tile_enlargement;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = GHConstants.MaxTilesPerReplacement)]
        public int[] tile_autodraw;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = GHConstants.MaxTilesPerReplacement)]
        public ulong[] tile_flags;
    }

    public struct AutoDrawDefinition
    {
        public IntPtr autodraw_name;
        public int draw_type;
        public byte flags;
        public int source_glyph;
        public int source_glyph2;
        public int source_glyph3;
        public int source_glyph4;
        public ulong parameter1;
        public ulong parameter2;
        public ulong parameter3;
    }

    public struct LayerInfo
    {
        public int glyph; /* For ascii compatibility */
        public int bkglyph; /* For ascii compatibility */
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = (int)layer_types.MAX_LAYERS)]
        public int[] layer_glyphs;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = (int)layer_types.MAX_LAYERS)]
        public int[] layer_gui_glyphs;
        public ulong layer_flags;
        public uint m_id;  /* check that the monster found at the square is the one that is supposed to be drawn by comparing their m_ids */
        public uint o_id;  /* this is the o_id of the possibly moving boulder */
        public IntPtr memory_objchn;     /* Pointer to the location's first memory object*/
        public short damage_displayed;
        public short hit_tile;
        public sbyte special_feature_doodad_layer_height;
        public sbyte special_monster_layer_height;
        public sbyte monster_origin_x;
        public sbyte monster_origin_y;
        public int monster_hp;
        public int monster_maxhp;
        public int rider_glyph;
        public int rider_gui_glyph;
        public ulong status_bits;
        public ulong condition_bits;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = GHConstants.NUM_BUFF_BIT_ULONGS)]
        public ulong[] buff_bits;
        public sbyte wsegdir;
        public sbyte reverse_prev_wsegdir;
        public ulong monster_flags;
        public short object_height;
        public byte missile_poisoned;
        public byte missile_material;
        public short missile_special_quality;
        public byte missile_elemental_enchantment;
        public byte missile_exceptionality;
        public byte missile_mythic_prefix;
        public byte missile_mythic_suffix;
        public byte missile_eroded;
        public byte missile_eroded2;
        public ulong missile_flags;
        public short missile_height;
        public sbyte missile_origin_x;
        public sbyte missile_origin_y;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = GHConstants.MaxLeashed + 1)]
        public sbyte[] leash_mon_x; /* the last coordinate is the other end of the leash, i.e., u.ux at the time */
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = GHConstants.MaxLeashed + 1)]
        public sbyte[] leash_mon_y; /* the last coordinate is the other end of the leash, i.e., u.uy at the time */
    }

    public struct Obj
    {
        public IntPtr nobj;
        public IntPtr v;
        public IntPtr cobj; /* contents list for containers */
        public uint o_id;
        public uint owt;
        public short otyp; /* object class number */
        public sbyte ox, oy;
        public sbyte ox0, oy0;
        public long quan;         /* number of items */
        public long age;          /* creation date */
        public long owornmask;
        public ulong item_flags;  /* general purpose object flags, like speflags */
        public ulong speflags;    /* anything else that might be going on with an item, not affected by cancellation */
        public short enchantment; /* Always set to zero by cancellation */
        public short charges; /* number of charges for wand or charged tool ( >= -1 ), always set to -1/0 by cancellation */
        public short special_quality; /* item-specific special quality, e.g., the amount of wetness of a towel, number of candles attached to candelabrum, not affected by cancellation */
        public short oartifact; /* artifact array index */
        public sbyte oclass;    /* object class */
        public sbyte invlet;    /* designation in inventory */
        public byte mythic_prefix; /* magical quality for a weapon or armor giving additional powers */
        public byte mythic_suffix;  /* magical quality for a weapon or armor giving additional powers */
        public byte exceptionality; /* exceptional, elite, etc. weapon, multiplies base damage */
        public byte elemental_enchantment; /* cold, fire, lightning, or deathly */
        public byte material; /*  specific material of this object */
        public sbyte recharged; /* number of times it's been recharged */
        public sbyte where;        /* where the object thinks it is */
        public sbyte timed; /* # of fuses (timers) attached to this Obj */
        public int corpsenm;         /* type of corpse is mons[corpsenm] */
        public int usecount;           /* overloaded for various things that tally */
        public uint oeaten;        /* nutrition left in food, if partly eaten */
        public ulong bitflags;
        public byte oeroded;
        public byte oeroded2;
        public bool cursed
        {
            get => (bitflags & (ulong)obj_bitfield_types.OBJFLAG_CURSED) != 0;
            set => bitflags = (bitflags & ~(ulong)obj_bitfield_types.OBJFLAG_CURSED) | (value ? (ulong)obj_bitfield_types.OBJFLAG_CURSED : 0UL);
        }
        public bool blessed
        {
            get => (bitflags & (ulong)obj_bitfield_types.OBJFLAG_BLESSED) != 0;
            set => bitflags = (bitflags & ~(ulong)obj_bitfield_types.OBJFLAG_BLESSED) | (value ? (ulong)obj_bitfield_types.OBJFLAG_BLESSED : 0UL);
        }
        public bool unpaid
        {
            get => (bitflags & (ulong)obj_bitfield_types.OBJFLAG_UNPAID) != 0;
            set => bitflags = (bitflags & ~(ulong)obj_bitfield_types.OBJFLAG_UNPAID) | (value ? (ulong)obj_bitfield_types.OBJFLAG_UNPAID : 0UL);
        }
        public bool no_charge
        {
            get => (bitflags & (ulong)obj_bitfield_types.OBJFLAG_NO_CHARGE) != 0;
            set => bitflags = (bitflags & ~(ulong)obj_bitfield_types.OBJFLAG_NO_CHARGE) | (value ? (ulong)obj_bitfield_types.OBJFLAG_NO_CHARGE : 0UL);
        }
        public bool known
        {
            get => (bitflags & (ulong)obj_bitfield_types.OBJFLAG_KNOWN) != 0;
            set => bitflags = (bitflags & ~(ulong)obj_bitfield_types.OBJFLAG_KNOWN) | (value ? (ulong)obj_bitfield_types.OBJFLAG_KNOWN : 0UL);
        }
        public bool dknown
        {
            get => (bitflags & (ulong)obj_bitfield_types.OBJFLAG_DKNOWN) != 0;
            set => bitflags = (bitflags & ~(ulong)obj_bitfield_types.OBJFLAG_DKNOWN) | (value ? (ulong)obj_bitfield_types.OBJFLAG_DKNOWN : 0UL);
        }
        public bool bknown
        {
            get => (bitflags & (ulong)obj_bitfield_types.OBJFLAG_BKNOWN) != 0;
            set => bitflags = (bitflags & ~(ulong)obj_bitfield_types.OBJFLAG_BKNOWN) | (value ? (ulong)obj_bitfield_types.OBJFLAG_BKNOWN : 0UL);
        }
        public bool rknown
        {
            get => (bitflags & (ulong)obj_bitfield_types.OBJFLAG_RKNOWN) != 0;
            set => bitflags = (bitflags & ~(ulong)obj_bitfield_types.OBJFLAG_RKNOWN) | (value ? (ulong)obj_bitfield_types.OBJFLAG_RKNOWN : 0UL);
        }
        public bool oerodeproof
        {
            get => (bitflags & (ulong)obj_bitfield_types.OBJFLAG_OERODEPROOF) != 0;
            set => bitflags = (bitflags & ~(ulong)obj_bitfield_types.OBJFLAG_OERODEPROOF) | (value ? (ulong)obj_bitfield_types.OBJFLAG_OERODEPROOF : 0UL);
        }
        public bool olocked
        {
            get => (bitflags & (ulong)obj_bitfield_types.OBJFLAG_OLOCKED) != 0;
            set => bitflags = (bitflags & ~(ulong)obj_bitfield_types.OBJFLAG_OLOCKED) | (value ? (ulong)obj_bitfield_types.OBJFLAG_OLOCKED : 0UL);
        }
        public bool obroken
        {
            get => (bitflags & (ulong)obj_bitfield_types.OBJFLAG_OBROKEN) != 0;
            set => bitflags = (bitflags & ~(ulong)obj_bitfield_types.OBJFLAG_OBROKEN) | (value ? (ulong)obj_bitfield_types.OBJFLAG_OBROKEN : 0UL);
        }
        public bool otrapped
        {
            get => (bitflags & (ulong)obj_bitfield_types.OBJFLAG_OTRAPPED) != 0;
            set => bitflags = (bitflags & ~(ulong)obj_bitfield_types.OBJFLAG_OTRAPPED) | (value ? (ulong)obj_bitfield_types.OBJFLAG_OTRAPPED : 0UL);
        }
        public bool lamplit
        {
            get => (bitflags & (ulong)obj_bitfield_types.OBJFLAG_LAMPLIT) != 0;
            set => bitflags = (bitflags & ~(ulong)obj_bitfield_types.OBJFLAG_LAMPLIT) | (value ? (ulong)obj_bitfield_types.OBJFLAG_LAMPLIT : 0UL);
        }
        public bool makingsound
        {
            get => (bitflags & (ulong)obj_bitfield_types.OBJFLAG_MAKINGSOUND) != 0;
            set => bitflags = (bitflags & ~(ulong)obj_bitfield_types.OBJFLAG_MAKINGSOUND) | (value ? (ulong)obj_bitfield_types.OBJFLAG_MAKINGSOUND : 0UL);
        }
        public bool globby
        {
            get => (bitflags & (ulong)obj_bitfield_types.OBJFLAG_GLOBBY) != 0;
            set => bitflags = (bitflags & ~(ulong)obj_bitfield_types.OBJFLAG_GLOBBY) | (value ? (ulong)obj_bitfield_types.OBJFLAG_GLOBBY : 0UL);
        }
        public bool greased
        {
            get => (bitflags & (ulong)obj_bitfield_types.OBJFLAG_GREASED) != 0;
            set => bitflags = (bitflags & ~(ulong)obj_bitfield_types.OBJFLAG_GREASED) | (value ? (ulong)obj_bitfield_types.OBJFLAG_GREASED : 0UL);
        }
        public bool nomerge
        {
            get => (bitflags & (ulong)obj_bitfield_types.OBJFLAG_NOMERGE) != 0;
            set => bitflags = (bitflags & ~(ulong)obj_bitfield_types.OBJFLAG_NOMERGE) | (value ? (ulong)obj_bitfield_types.OBJFLAG_NOMERGE : 0UL);
        }
        public bool was_thrown
        {
            get => (bitflags & (ulong)obj_bitfield_types.OBJFLAG_WAS_THROWN) != 0;
            set => bitflags = (bitflags & ~(ulong)obj_bitfield_types.OBJFLAG_WAS_THROWN) | (value ? (ulong)obj_bitfield_types.OBJFLAG_WAS_THROWN : 0UL);
        }
        public bool has_special_tileset
        {
            get => (bitflags & (ulong)obj_bitfield_types.OBJFLAG_HAS_SPECIAL_TILESET) != 0;
            set => bitflags = (bitflags & ~(ulong)obj_bitfield_types.OBJFLAG_HAS_SPECIAL_TILESET) | (value ? (ulong)obj_bitfield_types.OBJFLAG_HAS_SPECIAL_TILESET : 0UL);
        }
        public bool in_use
        {
            get => (bitflags & (ulong)obj_bitfield_types.OBJFLAG_IN_USE) != 0;
            set => bitflags = (bitflags & ~(ulong)obj_bitfield_types.OBJFLAG_IN_USE) | (value ? (ulong)obj_bitfield_types.OBJFLAG_IN_USE : 0UL);
        }
        public bool bypass
        {
            get => (bitflags & (ulong)obj_bitfield_types.OBJFLAG_BYPASS) != 0;
            set => bitflags = (bitflags & ~(ulong)obj_bitfield_types.OBJFLAG_BYPASS) | (value ? (ulong)obj_bitfield_types.OBJFLAG_BYPASS : 0UL);
        }
        public bool cknown
        {
            get => (bitflags & (ulong)obj_bitfield_types.OBJFLAG_CKNOWN) != 0;
            set => bitflags = (bitflags & ~(ulong)obj_bitfield_types.OBJFLAG_CKNOWN) | (value ? (ulong)obj_bitfield_types.OBJFLAG_CKNOWN : 0UL);
        }
        public bool lknown
        {
            get => (bitflags & (ulong)obj_bitfield_types.OBJFLAG_LKNOWN) != 0;
            set => bitflags = (bitflags & ~(ulong)obj_bitfield_types.OBJFLAG_LKNOWN) | (value ? (ulong)obj_bitfield_types.OBJFLAG_LKNOWN : 0UL);
        }
        public bool tknown
        {
            get => (bitflags & (ulong)obj_bitfield_types.OBJFLAG_TKNOWN) != 0;
            set => bitflags = (bitflags & ~(ulong)obj_bitfield_types.OBJFLAG_TKNOWN) | (value ? (ulong)obj_bitfield_types.OBJFLAG_TKNOWN : 0UL);
        }
        public bool nknown
        {
            get => (bitflags & (ulong)obj_bitfield_types.OBJFLAG_NKNOWN) != 0;
            set => bitflags = (bitflags & ~(ulong)obj_bitfield_types.OBJFLAG_NKNOWN) | (value ? (ulong)obj_bitfield_types.OBJFLAG_NKNOWN : 0UL);
        }
        public bool aknown
        {
            get => (bitflags & (ulong)obj_bitfield_types.OBJFLAG_AKNOWN) != 0;
            set => bitflags = (bitflags & ~(ulong)obj_bitfield_types.OBJFLAG_AKNOWN) | (value ? (ulong)obj_bitfield_types.OBJFLAG_AKNOWN : 0UL);
        }
        public bool mknown
        {
            get => (bitflags & (ulong)obj_bitfield_types.OBJFLAG_MKNOWN) != 0;
            set => bitflags = (bitflags & ~(ulong)obj_bitfield_types.OBJFLAG_MKNOWN) | (value ? (ulong)obj_bitfield_types.OBJFLAG_MKNOWN : 0UL);
        }
        public bool rotknown
        {
            get => (bitflags & (ulong)obj_bitfield_types.OBJFLAG_ROTKNOWN) != 0;
            set => bitflags = (bitflags & ~(ulong)obj_bitfield_types.OBJFLAG_ROTKNOWN) | (value ? (ulong)obj_bitfield_types.OBJFLAG_ROTKNOWN : 0UL);
        }
        public uint firing_m_id;  /* m_id for monster that fired this object (for picking up back) */
        public uint o_id_memory;  /* This is a memory object of this o_id */
        public uint m_id_memory;  /* This is a memory object of this mimic m_id */
        public short cooldownleft;       /* item cooldown left before it can be used again*/
        public short repowerleft;       /* artifact cooldown left before its invoke ability can be used again*/
        public short detectioncount;    /* monsters detected for WARN_ORC and other similar properties */
        public short invokeleft;       /* counter for an artifact's item-specific invoke ability */
        public byte invokeon;      /* the object's / artifact's invoked ability is on */
        public byte special_tileset;
        public int glyph;
        public int gui_glyph;
        public IntPtr oextra; /* pointer to oextra struct */
    }

    public struct ObjClassData
    {
        public int tile_height;
        public short special_quality;
        public short max_charges;
        public byte semitransparent;
        public byte lamplit;
        public byte poisoned;
        public byte eroded;
        public byte eroded2;
        public byte corrodeable;
        public byte rottable;
        public byte flammable;
        public byte rustprone;
        public byte poisonable;
        public byte is_uchain;
        public byte is_uball;
        public sbyte obj_loc_x;
        public sbyte obj_loc_y;
        public sbyte oc_subtyp;
        public byte ocdata_flags;
    }

    public class EquipmentSlot
    {
        public readonly string SlotName;
        public readonly int PictureIndex;
        public readonly int AltPictureIndex;
        public readonly int AltPictureStyle;
        public readonly obj_worn_flags WornFlag;
        public readonly obj_class_types ObjClassType;
        public readonly obj_armor_types ArmorType;
        public EquipmentSlot(string slotName, int pictureIndex, int altPictureIndex, int altPictureStyle, obj_worn_flags wornFlag, obj_class_types objClassType, obj_armor_types armorType)
        {
            SlotName = slotName;
            PictureIndex = pictureIndex;
            AltPictureIndex = altPictureIndex;
            AltPictureStyle = altPictureStyle;
            WornFlag = wornFlag;
            ObjClassType = objClassType;
            ArmorType = armorType;
        }
    }

    public struct monst_info
    {
        public int glyph;
        public int gui_glyph;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = GHConstants.UTF8BUFSZ)]
        public string name;
        public uint m_id;
        public int mhp;
        public int mhpmax;
        public ulong status_bits;
        public ulong condition_bits;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = GHConstants.NUM_BUFF_BIT_ULONGS)]
        public ulong[] buff_bits;
        public ulong monster_flags;
    }

    public struct GHSelectedItem
    {
        public IntPtr Identifier;
        public int Count;
    }

    public struct EngravingInfo
    {
        public readonly bool HasEngraving;
        public readonly string Text;
        public readonly int EngrType;
        public readonly ulong EngrFlags;
        public readonly ulong GeneralFlags;
        public readonly string[] RowSplit;
        public EngravingInfo(string text, int etype, ulong eflags, ulong gflags)
        {
            HasEngraving = true;
            Text = text;
            EngrType = etype;
            EngrFlags = eflags;
            GeneralFlags = gflags;
            string[] TextSplit;
            if(text != null)
            TextSplit = text.Split();
            else
            TextSplit = null;
            if (TextSplit != null)
            {
                int defmaxrowlen = 5;
                int maxrows = 6;
                int currows = (text.Length - 1) / defmaxrowlen + 1;
                int usedmaxrowlen = defmaxrowlen;
                if (currows > maxrows)
                usedmaxrowlen = (int)Math.Ceiling((double)defmaxrowlen * (double)currows / (double)maxrows);
                List<string> list = new List<string>();
                string builtString = "";
                for(int i = 0; i < TextSplit.Length; i++)
                {
                    string str = TextSplit[i].Trim();
                    if (string.IsNullOrWhiteSpace(str))
                    {
                        if(i == TextSplit.Length - 1)
                        {
                            list.Add(str);
                            builtString = "";
                        }
                        continue;
                    }
                    if (builtString.Length >= (usedmaxrowlen * 4) / 5 && builtString.Length + str.Length >= (usedmaxrowlen * 9) / 5)
                    {
                        list.Add(builtString);
                        if (str.Length >= usedmaxrowlen || i == TextSplit.Length - 1)
                        {
                            list.Add(str);
                            builtString = "";
                        }
                        else
                        {
                            builtString = str;
                        }
                    }
                    else
                    {
                        if(builtString != "")
                        builtString += " " + str;
                        else
                        builtString = str;
                        if (str.Length >= usedmaxrowlen || i == TextSplit.Length - 1)
                        {
                            list.Add(builtString);
                            builtString = "";
                        }
                    }
                }
                RowSplit = list.ToArray();
            }
            else
            RowSplit = null;
        }
    }

    public class InverseBooleanConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return value is bool b ? !b : value;
        }
        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return value is bool b ? !b : value;
        }
    }
}
