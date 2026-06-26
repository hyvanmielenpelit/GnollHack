import re

with open('c:/hmp/GnollHack/include/rm.h', 'r') as f:
    content = f.read()

# Restore struct rm bitflags
content = content.replace(
'''    uint32_t bitflags;
    uint32_t bitflags2;
#define RM_BITFLAG_NONE''',
'''    uint64_t bitflags;
#define RM_BITFLAG_NONE''')

with open('c:/hmp/GnollHack/include/rm.h', 'w') as f:
    f.write(content)
