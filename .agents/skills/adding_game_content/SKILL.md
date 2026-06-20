---
name: adding_game_content
description: Patterns for adding monsters, objects, and artifacts to GnollHack. Covers MON/WEAPON/ARMOR/A macro syntax, attack tuples, monster and object flags, the auto-generated headers (pm.h, onames.h), soundset and animation assignments, and the makedefs rebuild pipeline.
---

# Adding Game Content

## Critical Rules
- **NEVER hand-edit auto-generated files**: `include/pm.h`, `include/onames.h`, `include/vis_tab.h`, `include/animoff.h`, `include/animtotals.h`.
- **Order matters**: Monsters in `src/monst.c` MUST be grouped by class (`S_ANT`, etc.) and sorted by ascending strength. Objects in `src/objects.c` MUST be grouped by object class.
- **Always rebuild `makedefs`** after changes: Rebuild `pm.h` (`makedefs -p`) and `onames.h` (`makedefs -o`) before compiling the native library.

## Monsters (`src/monst.c`)
- **Location**: `src/monst.c`
- **Macros**:
  - `MON(...)`: Standard monster.
  - `ANIMATED_MON(...)`: Custom tile animation.
  - `ENLARGED_MON(...)`: Oversized tile.
  - `ENLARGED_ANIMATED_MON(...)`: Oversized + animation.

**Parameters format**:
```c
MON(name, title, desc, female_name, common_name,
    symbol, LVL(lvl, move, ac, mc, mr, aln),
    (generation_flags),
    A(attk1, ..., attk8),
    SIZ(weight, nutrition, sound, size, heads, light, material),
    STATS(str, dex, con, int, wis, cha),
    mr1_resistances, mr2_conferred, mc_flags, m1_flags, m2_flags, m3_flags,
    m4_flags, m5_flags, m6_flags, m7_flags, m8_flags,
    difficulty, color, soundset, female_soundset, soundset_subtype)
```
- **ATTK Tuple**: `ATTK(type, damage_type, num_dice, die_size, plus, adjust, min_level, range, flags, action_tile)`
- Use `None` for unused strings, `NO_ATTK` for unused attacks, `NO_SOUNDSET_SUBTYPE` for subtype.

## Objects (`src/objects.c`)
- **Location**: `src/objects.c`
- **Macros**: `WEAPON`, `PROJECTILE`, `BOW`, `ARMOR`, `RING`, `AMULET`, `SCROLL`, `POTION`, `WAND`, `FOOD`, `GEM`, `TOOL`.

**Parameters format (WEAPON)**:
```c
WEAPON(name, desc,
    nmkn, mrg, mgc, bi, prob, multigen, enchtype,
    wt, cost,
    dmgtype, sdice, sdam, sdmgplus,
    ldice, ldam, ldmgplus,
    edmgtype, edice, edam, edmgplus,
    aflags, aflags2, critpct,
    hitbon, mcadj, throwrange,
    acbon, mcbon, manabon, hpbon,
    bonusattrs, attrbonus, splcastpen, multicount,
    power, power2, power3, pflags,
    typ, sub, skill,
    matinit, metal,
    color, height, soundset,
    flags1, flags2, flags3, flags4, flags5, flags6,
    powconfermask, permittedtargets)
```

## Artifacts (`include/artilist.h`)
- **Location**: `include/artilist.h`
- **Macro**: `A(...)`

**Parameters format**:
```c
A(name, desc, hit_desc, base_type, mask_type,
  material, exceptionality, mythic_prefix, mythic_suffix,
  aflags, aflags2, spfx, cspfx, mt,
  tohit_dice, tohit_size, tohit_plus,
  attack_spec, defense, carry, invoke,
  invoke_dur_dice, invoke_dur_size, invoke_dur_plus,
  manacost, repower, alignment, role, race, cost,
  glow_color, obj_color, height, soundset)
```

## Step-by-Step Instructions
1. **Locate appropriate section**: Find the class in `monst.c` or `objects.c` and maintain sorting (e.g. ascending strength).
2. **Add entry**: Use the proper macro.
3. **Run makedefs**: `makedefs -p` for monsters, `makedefs -o` for objects.
4. **Rebuild**: Compile native library, then MAUI app.
