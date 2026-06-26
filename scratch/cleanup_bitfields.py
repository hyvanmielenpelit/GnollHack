import re
import os

def update_file(path, replacements):
    with open(path, 'r', encoding='utf-8') as f:
        content = f.read()
    
    for old, new in replacements:
        if isinstance(old, re.Pattern):
            content = old.sub(new, content)
        else:
            content = content.replace(old, new)
            
    with open(path, 'w', encoding='utf-8') as f:
        f.write(content)

# 1. Update config.h
config_replacements = [
    (re.compile(r'/\* If you want to use bitfields in structs \(to save space in save files \n \* containing bitfields\), you can define BITFIELDS\.  Otherwise, the game will\n \* allocate a whole byte for each field\.\n \*/\n#define BITFIELDS /\* Good bitfield handling \*/ /\* Implementation of bitfields may vary across compilers and platforms, and may not be a reliable choice for GnollHack \*/\n\n'), '')
]
update_file('c:/hmp/GnollHack/include/config.h', config_replacements)
print("Updated config.h")

# 2. Update global.h
global_replacements = [
    (re.compile(r'#ifdef BITFIELDS\n#undef BITFIELDS\n#endif\n'), ''),
    (re.compile(r'#ifdef BITFIELDS\n#define Bitfield\(x, n\) unsigned x : n\n#else\n#define Bitfield\(x, n\) uchar x\n#endif\n'), '')
]
update_file('c:/hmp/GnollHack/include/global.h', global_replacements)
print("Updated global.h")

# 3. Update patchlevel.h
# #define EDITLEVEL 0
with open('c:/hmp/GnollHack/include/patchlevel.h', 'r', encoding='utf-8') as f:
    patchlevel_content = f.read()

patchlevel_content = re.sub(r'(#define EDITLEVEL\s+)(\d+)', lambda m: f"{m.group(1)}{int(m.group(2)) + 1}", patchlevel_content)

with open('c:/hmp/GnollHack/include/patchlevel.h', 'w', encoding='utf-8') as f:
    f.write(patchlevel_content)
print("Updated patchlevel.h")
