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

def process_content(content):
    for flag in flags:
        # Match assignments: level.flags.has_shop = expr
        assignment_pattern = r'level\.flags\.' + flag + r'\s*=\s*([^;]+)'
        content = re.sub(assignment_pattern, lambda m: f'set_levflag_{flag}(&level.flags, {m.group(1).strip()})', content)
        
        # Match other reads: level.flags.has_shop
        read_pattern = r'level\.flags\.' + flag + r'\b'
        content = re.sub(read_pattern, f'is_levflag_{flag}(&level.flags)', content)
    return content

changed_files = []
for root, _, files in os.walk('c:/hmp/GnollHack'):
    for file in files:
        if file.endswith('.c') or file.endswith('.h') or file.endswith('.cpp'):
            path = os.path.join(root, file)
            with open(path, 'r', encoding='utf-8', errors='ignore') as f:
                content = f.read()
                
            new_content = process_content(content)
            if new_content != content:
                with open(path, 'w', encoding='utf-8') as f:
                    f.write(new_content)
                changed_files.append(path)

print(f"Changed {len(changed_files)} files.")
for f in changed_files:
    print(" -", os.path.basename(f))
