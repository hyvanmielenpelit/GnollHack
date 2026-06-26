import re
import sys

def patch_rm_h():
    with open('c:/hmp/GnollHack/include/rm.h', 'r') as f:
        content = f.read()

    # Change uint64_t bitflags; to uint32_t bitflags; uint32_t bitflags2;
    content = content.replace('uint64_t bitflags;', 'uint32_t bitflags;\n    uint32_t bitflags2;')

    # Replace the LEVFLAG definition
    content = content.replace(
        '#define LEVFLAG_NO_SPECIAL_LEVEL_NAMING_CHECKS   (0x0000000100000000UL)',
        '#define LEVFLAG2_NO_SPECIAL_LEVEL_NAMING_CHECKS  (0x00000001UL)'
    )

    # Replace the getters and setters
    old_get = '#define is_levflag_no_special_level_naming_checks(flags) (((flags)->bitflags & LEVFLAG_NO_SPECIAL_LEVEL_NAMING_CHECKS) != 0)'
    new_get = '#define is_levflag_no_special_level_naming_checks(flags) (((flags)->bitflags2 & LEVFLAG2_NO_SPECIAL_LEVEL_NAMING_CHECKS) != 0)'
    
    old_set = '#define set_levflag_no_special_level_naming_checks(flags, v) ((flags)->bitflags = ((flags)->bitflags & ~LEVFLAG_NO_SPECIAL_LEVEL_NAMING_CHECKS) | ((v) ? LEVFLAG_NO_SPECIAL_LEVEL_NAMING_CHECKS : 0))'
    new_set = '#define set_levflag_no_special_level_naming_checks(flags, v) ((flags)->bitflags2 = ((flags)->bitflags2 & ~LEVFLAG2_NO_SPECIAL_LEVEL_NAMING_CHECKS) | ((v) ? LEVFLAG2_NO_SPECIAL_LEVEL_NAMING_CHECKS : 0))'
    
    old_tog = '#define toggle_levflag_no_special_level_naming_checks(flags) ((flags)->bitflags ^= LEVFLAG_NO_SPECIAL_LEVEL_NAMING_CHECKS)'
    new_tog = '#define toggle_levflag_no_special_level_naming_checks(flags) ((flags)->bitflags2 ^= LEVFLAG2_NO_SPECIAL_LEVEL_NAMING_CHECKS)'

    content = content.replace(old_get, new_get)
    content = content.replace(old_set, new_set)
    content = content.replace(old_tog, new_tog)
    
    # Strip leading zeros from all UL definitions to make it clean
    for i in range(32):
        hex_val = 1 << i
        old_hex = f"(0x{hex_val:016x}UL)"
        new_hex = f"(0x{hex_val:08x}UL)"
        content = content.replace(old_hex, new_hex)

    with open('c:/hmp/GnollHack/include/rm.h', 'w') as f:
        f.write(content)
    print("rm.h patched")

patch_rm_h()
