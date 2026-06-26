import os
import re

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

# 1. macwin.h (convert Bitfield(name, 1) to boolean name;)
macwin_path = 'c:/hmp/GnollHack/include/macwin.h'
with open(macwin_path, 'r', encoding='utf-8') as f:
    macwin_content = f.read()

macwin_content = re.sub(r'Bitfield\((\w+),\s*1\);', r'boolean \1;', macwin_content)
with open(macwin_path, 'w', encoding='utf-8') as f:
    f.write(macwin_content)
print("Updated macwin.h")

# 2. pager.c (convert Bitfield(name, 1) to boolean name;)
pager_path = 'c:/hmp/GnollHack/src/pager.c'
with open(pager_path, 'r', encoding='utf-8') as f:
    pager_content = f.read()
pager_content = re.sub(r'Bitfield\((\w+),\s*1\);', r'boolean \1;', pager_content)
with open(pager_path, 'w', encoding='utf-8') as f:
    f.write(pager_content)
print("Updated pager.c")

# 3. rm.h (convert struct levelflags)
rm_path = 'c:/hmp/GnollHack/include/rm.h'
with open(rm_path, 'r', encoding='utf-8') as f:
    rm_content = f.read()

# Extract all bitfields from struct levelflags
# The struct starts with: struct levelflags {
# and ends with: struct levelflags flags; } dlevel_t;
struct_pattern = re.compile(r'(struct levelflags \{.*?)(    Bitfield\(has_tileset, 1\);.*?)(    unsigned reserved;)', re.DOTALL)
m = struct_pattern.search(rm_content)
if not m:
    print("Could not find struct levelflags in rm.h")
    exit(1)

bitfields_block = m.group(2)
flags = re.findall(r'Bitfield\((\w+),\s*1\);', bitfields_block)

new_block = "    uint64_t bitflags;\n"

rm_content = rm_content[:m.start(2)] + new_block + rm_content[m.end(2):]

# Add macros at the end of rm.h
macros = "\n/* levelflags bitflags */\n"
for i, flag in enumerate(flags):
    macro_name = "LEVFLAG_" + flag.upper()
    macros += f"#define {macro_name:<40} (0x{1<<i:016x}UL)\n"

macros += "\n"
for flag in flags:
    macro_name = "LEVFLAG_" + flag.upper()
    macros += f"#define is_levflag_{flag}(flags) (((flags)->bitflags & {macro_name}) != 0)\n"
    macros += f"#define set_levflag_{flag}(flags, v) ((flags)->bitflags = ((flags)->bitflags & ~{macro_name}) | ((v) ? {macro_name} : 0))\n"
    macros += f"#define toggle_levflag_{flag}(flags) ((flags)->bitflags ^= {macro_name})\n"

rm_content = rm_content + macros

with open(rm_path, 'w', encoding='utf-8') as f:
    f.write(rm_content)

print("Updated rm.h")
