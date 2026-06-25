import os
import re

filepath = 'c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX/GHConstants.cs'
with open(filepath, 'r', encoding='utf-8') as f:
    content = f.read()

# We will replace from `public uint oeaten;` down to `public IntPtr oextra;`
pattern = re.compile(r'(        public uint oeaten;        /\* nutrition left in food, if partly eaten \*/\n)(.*?)(        public IntPtr oextra; /\* pointer to oextra struct \*/)', re.DOTALL)

replacement = r'''\1
        public ulong bitflags;
        public byte oeroded;
        public byte oeroded2;
        
        public uint cursed 
        {
            get { return (uint)(bitflags & 0x00000001UL); } 
            set { bitflags = (bitflags & ~0x00000001UL) | ((ulong)value & 0x00000001UL); }
        }
        public uint blessed
        {
            get { return (uint)((bitflags >> 1) & 0x00000001UL); }
            set { bitflags = (bitflags & ~(0x00000001UL << 1)) | (((ulong)value & 0x00000001UL) << 1); }
        }
        public uint unpaid
        {
            get { return (uint)((bitflags >> 2) & 0x00000001UL); }
            set { bitflags = (bitflags & ~(0x00000001UL << 2)) | (((ulong)value & 0x00000001UL) << 2); }
        }
        public uint no_charge
        {
            get { return (uint)((bitflags >> 3) & 0x00000001UL); }
            set { bitflags = (bitflags & ~(0x00000001UL << 3)) | (((ulong)value & 0x00000001UL) << 3); }
        }
        public uint known
        {
            get { return (uint)((bitflags >> 4) & 0x00000001UL); }
            set { bitflags = (bitflags & ~(0x00000001UL << 4)) | (((ulong)value & 0x00000001UL) << 4); }
        }
        public uint dknown
        {
            get { return (uint)((bitflags >> 5) & 0x00000001UL); }
            set { bitflags = (bitflags & ~(0x00000001UL << 5)) | (((ulong)value & 0x00000001UL) << 5); }
        }
        public uint bknown
        {
            get { return (uint)((bitflags >> 6) & 0x00000001UL); }
            set { bitflags = (bitflags & ~(0x00000001UL << 6)) | (((ulong)value & 0x00000001UL) << 6); }
        }
        public uint rknown
        {
            get { return (uint)((bitflags >> 7) & 0x00000001UL); }
            set { bitflags = (bitflags & ~(0x00000001UL << 7)) | (((ulong)value & 0x00000001UL) << 7); }
        }
        public uint oerodeproof
        {
            get { return (uint)((bitflags >> 8) & 0x00000001UL); }
            set { bitflags = (bitflags & ~(0x00000001UL << 8)) | (((ulong)value & 0x00000001UL) << 8); }
        }
        public uint olocked
        {
            get { return (uint)((bitflags >> 9) & 0x00000001UL); }
            set { bitflags = (bitflags & ~(0x00000001UL << 9)) | (((ulong)value & 0x00000001UL) << 9); }
        }
        public uint obroken
        {
            get { return (uint)((bitflags >> 10) & 0x00000001UL); }
            set { bitflags = (bitflags & ~(0x00000001UL << 10)) | (((ulong)value & 0x00000001UL) << 10); }
        }
        public uint otrapped
        {
            get { return (uint)((bitflags >> 11) & 0x00000001UL); }
            set { bitflags = (bitflags & ~(0x00000001UL << 11)) | (((ulong)value & 0x00000001UL) << 11); }
        }
        public uint lamplit
        {
            get { return (uint)((bitflags >> 12) & 0x00000001UL); }
            set { bitflags = (bitflags & ~(0x00000001UL << 12)) | (((ulong)value & 0x00000001UL) << 12); }
        }
        public uint makingsound
        {
            get { return (uint)((bitflags >> 13) & 0x00000001UL); }
            set { bitflags = (bitflags & ~(0x00000001UL << 13)) | (((ulong)value & 0x00000001UL) << 13); }
        }
        public uint globby
        {
            get { return (uint)((bitflags >> 14) & 0x00000001UL); }
            set { bitflags = (bitflags & ~(0x00000001UL << 14)) | (((ulong)value & 0x00000001UL) << 14); }
        }
        public uint greased
        {
            get { return (uint)((bitflags >> 15) & 0x00000001UL); }
            set { bitflags = (bitflags & ~(0x00000001UL << 15)) | (((ulong)value & 0x00000001UL) << 15); }
        }
        public uint nomerge
        {
            get { return (uint)((bitflags >> 16) & 0x00000001UL); }
            set { bitflags = (bitflags & ~(0x00000001UL << 16)) | (((ulong)value & 0x00000001UL) << 16); }
        }
        public uint was_thrown
        {
            get { return (uint)((bitflags >> 17) & 0x00000001UL); }
            set { bitflags = (bitflags & ~(0x00000001UL << 17)) | (((ulong)value & 0x00000001UL) << 17); }
        }
        public uint has_special_tileset
        {
            get { return (uint)((bitflags >> 18) & 0x00000001UL); }
            set { bitflags = (bitflags & ~(0x00000001UL << 18)) | (((ulong)value & 0x00000001UL) << 18); }
        }
        public uint in_use
        {
            get { return (uint)((bitflags >> 19) & 0x00000001UL); }
            set { bitflags = (bitflags & ~(0x00000001UL << 19)) | (((ulong)value & 0x00000001UL) << 19); }
        }
        public uint bypass
        {
            get { return (uint)((bitflags >> 20) & 0x00000001UL); }
            set { bitflags = (bitflags & ~(0x00000001UL << 20)) | (((ulong)value & 0x00000001UL) << 20); }
        }
        public uint cknown
        {
            get { return (uint)((bitflags >> 21) & 0x00000001UL); }
            set { bitflags = (bitflags & ~(0x00000001UL << 21)) | (((ulong)value & 0x00000001UL) << 21); }
        }
        public uint lknown
        {
            get { return (uint)((bitflags >> 22) & 0x00000001UL); }
            set { bitflags = (bitflags & ~(0x00000001UL << 22)) | (((ulong)value & 0x00000001UL) << 22); }
        }
        public uint tknown
        {
            get { return (uint)((bitflags >> 23) & 0x00000001UL); }
            set { bitflags = (bitflags & ~(0x00000001UL << 23)) | (((ulong)value & 0x00000001UL) << 23); }
        }
        public uint nknown
        {
            get { return (uint)((bitflags >> 24) & 0x00000001UL); }
            set { bitflags = (bitflags & ~(0x00000001UL << 24)) | (((ulong)value & 0x00000001UL) << 24); }
        }
        public uint aknown
        {
            get { return (uint)((bitflags >> 25) & 0x00000001UL); }
            set { bitflags = (bitflags & ~(0x00000001UL << 25)) | (((ulong)value & 0x00000001UL) << 25); }
        }
        public uint mknown
        {
            get { return (uint)((bitflags >> 26) & 0x00000001UL); }
            set { bitflags = (bitflags & ~(0x00000001UL << 26)) | (((ulong)value & 0x00000001UL) << 26); }
        }
        public uint rotknown
        {
            get { return (uint)((bitflags >> 27) & 0x00000001UL); }
            set { bitflags = (bitflags & ~(0x00000001UL << 27)) | (((ulong)value & 0x00000001UL) << 27); }
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

\3'''

new_content = pattern.sub(replacement, content)

with open(filepath, 'w', encoding='utf-8') as f:
    f.write(new_content)

print("Updated GHConstants.cs")
