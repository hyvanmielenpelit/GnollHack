import os
import re

filepath = 'c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX/GHConstants.cs'
with open(filepath, 'r', encoding='utf-8') as f:
    content = f.read()

# 1. Replace the enum obj_bitfield_types

enum_replacement = '''    public enum obj_bitfield_types : ulong
    {
        OBJFLAG_CURSED                   = 0x00000001UL,
        OBJFLAG_BLESSED                  = 0x00000002UL,
        OBJFLAG_UNPAID                   = 0x00000004UL,
        OBJFLAG_NO_CHARGE                = 0x00000008UL,
        OBJFLAG_KNOWN                    = 0x00000010UL,
        OBJFLAG_DKNOWN                   = 0x00000020UL,
        OBJFLAG_BKNOWN                   = 0x00000040UL,
        OBJFLAG_RKNOWN                   = 0x00000080UL,
        OBJFLAG_OERODEPROOF              = 0x00000100UL,
        OBJFLAG_OLOCKED                  = 0x00000200UL,
        OBJFLAG_OBROKEN                  = 0x00000400UL,
        OBJFLAG_OTRAPPED                 = 0x00000800UL,
        OBJFLAG_LAMPLIT                  = 0x00001000UL,
        OBJFLAG_MAKINGSOUND              = 0x00002000UL,
        OBJFLAG_GLOBBY                   = 0x00004000UL,
        OBJFLAG_GREASED                  = 0x00008000UL,
        OBJFLAG_NOMERGE                  = 0x00010000UL,
        OBJFLAG_WAS_THROWN               = 0x00020000UL,
        OBJFLAG_HAS_SPECIAL_TILESET      = 0x00040000UL,
        OBJFLAG_IN_USE                   = 0x00080000UL,
        OBJFLAG_BYPASS                   = 0x00100000UL,
        OBJFLAG_CKNOWN                   = 0x00200000UL,
        OBJFLAG_LKNOWN                   = 0x00400000UL,
        OBJFLAG_TKNOWN                   = 0x00800000UL,
        OBJFLAG_NKNOWN                   = 0x01000000UL,
        OBJFLAG_AKNOWN                   = 0x02000000UL,
        OBJFLAG_MKNOWN                   = 0x04000000UL,
        OBJFLAG_ROTKNOWN                 = 0x08000000UL
    }'''

enum_pattern = re.compile(r'    public enum obj_bitfield_types : ulong\n    \{.*?\}', re.DOTALL)
content = enum_pattern.sub(enum_replacement, content)


# 2. Rewrite the properties in struct Obj
flags = [
    ('cursed', 'OBJFLAG_CURSED'),
    ('blessed', 'OBJFLAG_BLESSED'),
    ('unpaid', 'OBJFLAG_UNPAID'),
    ('no_charge', 'OBJFLAG_NO_CHARGE'),
    ('known', 'OBJFLAG_KNOWN'),
    ('dknown', 'OBJFLAG_DKNOWN'),
    ('bknown', 'OBJFLAG_BKNOWN'),
    ('rknown', 'OBJFLAG_RKNOWN'),
    ('oerodeproof', 'OBJFLAG_OERODEPROOF'),
    ('olocked', 'OBJFLAG_OLOCKED'),
    ('obroken', 'OBJFLAG_OBROKEN'),
    ('otrapped', 'OBJFLAG_OTRAPPED'),
    ('lamplit', 'OBJFLAG_LAMPLIT'),
    ('makingsound', 'OBJFLAG_MAKINGSOUND'),
    ('globby', 'OBJFLAG_GLOBBY'),
    ('greased', 'OBJFLAG_GREASED'),
    ('nomerge', 'OBJFLAG_NOMERGE'),
    ('was_thrown', 'OBJFLAG_WAS_THROWN'),
    ('has_special_tileset', 'OBJFLAG_HAS_SPECIAL_TILESET'),
    ('in_use', 'OBJFLAG_IN_USE'),
    ('bypass', 'OBJFLAG_BYPASS'),
    ('cknown', 'OBJFLAG_CKNOWN'),
    ('lknown', 'OBJFLAG_LKNOWN'),
    ('tknown', 'OBJFLAG_TKNOWN'),
    ('nknown', 'OBJFLAG_NKNOWN'),
    ('aknown', 'OBJFLAG_AKNOWN'),
    ('mknown', 'OBJFLAG_MKNOWN'),
    ('rotknown', 'OBJFLAG_ROTKNOWN'),
]

properties = []
for name, flag_name in flags:
    prop = f'''        public uint {name} 
        {{
            get {{ return (bitflags & (ulong)obj_bitfield_types.{flag_name}) != 0 ? 1U : 0U; }} 
            set {{ bitflags = (bitflags & ~(ulong)obj_bitfield_types.{flag_name}) | (value != 0 ? (ulong)obj_bitfield_types.{flag_name} : 0UL); }}
        }}'''
    properties.append(prop)

props_str = '\n'.join(properties)

# We replace the old properties section. The old section is between `public byte oeroded2;` and `public uint firing_m_id;`
prop_pattern = re.compile(r'(        public byte oeroded2;\s+)(.*?)(        public uint firing_m_id;)', re.DOTALL)
content = prop_pattern.sub(r'\1' + props_str + r'\n\n\3', content)

with open(filepath, 'w', encoding='utf-8') as f:
    f.write(content)

print("Updated GHConstants.cs with OBJFLAGS_")
