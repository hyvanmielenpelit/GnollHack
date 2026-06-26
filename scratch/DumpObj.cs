using System;
using System.Runtime.InteropServices;

namespace DumpObj {

    [StructLayout(LayoutKind.Sequential)]
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

    class Program {
        static void Main() {
            Console.WriteLine("Size of Obj: " + Marshal.SizeOf(typeof(Obj)));
            Console.WriteLine("Offset quan: " + Marshal.OffsetOf(typeof(Obj), "quan"));
            Console.WriteLine("Offset corpsenm: " + Marshal.OffsetOf(typeof(Obj), "corpsenm"));
            Console.WriteLine("Offset bitflags: " + Marshal.OffsetOf(typeof(Obj), "bitflags"));
            Console.WriteLine("Offset firing_m_id: " + Marshal.OffsetOf(typeof(Obj), "firing_m_id"));
            Console.WriteLine("Offset oextra: " + Marshal.OffsetOf(typeof(Obj), "oextra"));
        }
    }
}
