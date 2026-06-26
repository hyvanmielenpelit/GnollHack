import os
import re

flags = [
    'has_tileset', 'has_shop', 'has_vault', 'has_zoo', 'has_court', 'has_morgue',
    'has_beehive', 'has_barracks', 'has_armory', 'has_temple', 'has_smithy',
    'has_npc_room', 'has_library', 'has_dragonlair', 'has_garden', 'has_desertedshop',
    'has_swamp', 'noteleport', 'hardfloor', 'nommap', 'hero_memory', 'shortsighted',
    'graveyard', 'sokoban_rules', 'is_maze_lev', 'is_cavernous_lev', 'arboreal',
    'swampy', 'desert', 'wizard_bones', 'corrmaze', 'mapping_does_not_reveal_special',
    'no_special_level_naming_checks'
]

flag_pattern = re.compile(r'([\w\.>\-]+)\.(' + '|'.join(flags) + r')\b')
arrow_pattern = re.compile(r'([\w\.>\-]+)->(' + '|'.join(flags) + r')\b')

found = set()

for root, _, files in os.walk('c:/hmp/GnollHack/src'):
    for file in files:
        if file.endswith('.c') or file.endswith('.cpp'):
            path = os.path.join(root, file)
            with open(path, 'r', encoding='utf-8', errors='ignore') as f:
                content = f.read()
                
                for m in flag_pattern.finditer(content):
                    found.add(f"{m.group(1)}.{m.group(2)}")
                for m in arrow_pattern.finditer(content):
                    found.add(f"{m.group(1)}->{m.group(2)}")

for f in sorted(list(found)):
    print(f)
