import os
import re

filepath = 'c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX/GHConstants.cs'
with open(filepath, 'r', encoding='utf-8') as f:
    content = f.read()

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

for name, flag_name in flags:
    old_prop = f'''        public uint {name} \n        {{\n            get {{ return (bitflags & (ulong)obj_bitfield_types.{flag_name}) != 0 ? 1U : 0U; }} \n            set {{ bitflags = (bitflags & ~(ulong)obj_bitfield_types.{flag_name}) | (value != 0 ? (ulong)obj_bitfield_types.{flag_name} : 0UL); }}\n        }}'''
    new_prop = f'''        public bool {name} \n        {{\n            get {{ return (bitflags & (ulong)obj_bitfield_types.{flag_name}) != 0; }} \n            set {{ bitflags = (bitflags & ~(ulong)obj_bitfield_types.{flag_name}) | (value ? (ulong)obj_bitfield_types.{flag_name} : 0UL); }}\n        }}'''
    content = content.replace(old_prop, new_prop)

with open(filepath, 'w', encoding='utf-8') as f:
    f.write(content)

print("Updated GHConstants.cs getters and setters to bool")
