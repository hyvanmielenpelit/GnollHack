---
name: c_code_documentation
description: Best practices for AI agents documenting the GnollHack C codebase. Covers file headers, function documentation, inline comments, macro explanations, and strategies for making legacy NetHack-derived C code understandable to modern developers.
---

# C Code Documentation Guide for GnollHack

This skill defines how AI agents should apply documentation and comments to GnollHack's C source code. The goal is to make the codebase highly understandable for modern developers who may have little familiarity with NetHack heritage, K&R C conventions, or the game's complex internal systems.

For advanced documentation patterns (macros, structs, enums, complex control flow, NetHack idioms), see [references/advanced_patterns.md](file:///c:/hmp/GnollHack/.agents/skills/c_code_documentation/references/advanced_patterns.md).

## Why Documentation Matters Here

GnollHack's C core is derived from NetHack 3.6.2 — a codebase with roots in the 1980s. It uses conventions that are unfamiliar to modern C developers:

- **K&R-style function definitions** with separate parameter type declarations
- **Single-letter global variables** (`u` for the player, `m` for monsters)
- **Dense macro systems** that generate data tables and encode game logic
- **Implicit conventions** passed down through decades of NetHack development
- **Bitfield flags** accessed through macros with no obvious meaning
- **Complex pointer chains** linking monsters, objects, and map tiles

Without documentation, a modern developer reading `if (u.uswallow && u.ustuck == mtmp)` has no idea this checks whether the player has been swallowed by a specific monster. Documentation bridges this knowledge gap.

**Reference model**: [display.c](file:///c:/hmp/GnollHack/src/display.c) contains an excellent ~120-line block comment (lines 8-125) documenting the display system architecture. This level of documentation should be the aspiration for all major subsystem files.

---

## General Principles

### 1. Explain "Why" and "What", Not Just "How"

The code already shows *how* something is done. Documentation should explain *what* the code accomplishes in game terms and *why* it works the way it does.

```c
/* BAD: Restates the code */
/* Set i to 0 */
i = 0;

/* GOOD: Explains the game-level purpose */
/* Reset the player's stun counter so they can act normally */
make_stunned(0L, TRUE);
```

### 2. Use Game-Domain Language

Translate C implementation details into game concepts that a developer (or player) would recognize.

```c
/* BAD: Pure implementation description */
/* Check bits 0-7 of the monst flags field */

/* GOOD: Game-domain description */
/* Check if the monster has fire resistance (intrinsic property flag) */
```

### 3. Be Concise but Complete

Comments should be technically precise and brief, but can be more verbose than typical modern C projects when explaining legacy patterns, game mechanics, or non-obvious control flow. A few extra lines of explanation for a complex function are far better than leaving a modern developer confused.

### 4. Use Only `/* */` Block Comments

GnollHack follows the traditional C comment style. **Never use `//` line comments.** This is consistent with the NetHack heritage and the project's coding conventions defined in [DEVEL/code_style.txt](file:///c:/hmp/GnollHack/DEVEL/code_style.txt).

```c
/* Correct: C89-style block comment */
int x = 5;

// WRONG: C++ style line comment — do not use
int y = 10;
```

**Note**: The existing codebase does contain `//` comments, but these are almost exclusively used for commented-out code, not for documentation. New documentation must use `/* */`.

### 5. Respect the 78-Character Line Width

All comments must respect the project's 78-character line width limit. Break long comments across multiple lines:

```c
/*
 * When the player polymorphs into a form that cannot wear armor,
 * all worn armor is dropped. This iterates through each equipment
 * slot and calls dropx() for any occupied slot.
 */
```

### 6. Preserve Existing Comments

Do not remove or modify existing comments unless they are factually incorrect or the code they describe has changed. Many comments contain historical context or attribution (e.g., `/* Created by GAN 01/28/87 */`) that should be preserved.

---

## File-Level Documentation

Every `.c` and `.h` file should have a file-level comment block immediately after the copyright header. This block orients a new developer by explaining the file's purpose, scope, and key concepts.

### Format

```c
/* GnollHack File Change Notice: ... */
/* GnollHack 4.0    filename.c    ... */
/* Copyright (c) ... */
/* GnollHack may be freely redistributed. See license for details. */

/*
 * filename.c — Brief one-line description of the file's purpose
 *
 * This file implements [system/subsystem description]. It handles
 * [list of major responsibilities].
 *
 * Key concepts:
 *   - [Concept 1]: Brief explanation
 *   - [Concept 2]: Brief explanation
 *
 * Related files:
 *   - otherfile.c: [relationship]
 *   - header.h: [what it defines for this file]
 *
 * Global state accessed:
 *   - u (struct you): [which player fields and why]
 *   - level (dlevel_t): [which level data and why]
 */
```

### Example

```c
/*
 * eat.c — Food consumption, choking, and nutrition mechanics
 *
 * This file implements all logic related to eating: the 'e' command
 * handler, food preparation (e.g., tinning corpses), nutrition
 * calculation, corpse effects (poisoning, intrinsic gain), choking
 * on food, and cannibalism penalties.
 *
 * Key concepts:
 *   - Nutrition: Each food item has a nutrition value that reduces
 *     the player's hunger counter (u.uhunger). At 0 the player
 *     begins starving.
 *   - Corpse effects: Eating monster corpses can grant intrinsic
 *     properties (e.g., fire resistance from a red dragon corpse)
 *     or cause negative effects (e.g., poisoning, sliming).
 *   - Eating interruption: Eating takes multiple turns. The player
 *     can be interrupted by monster attacks mid-meal.
 *
 * Related files:
 *   - src/objnam.c: Food item naming and identification
 *   - include/obj.h: struct obj fields for food (oeaten, orotten)
 */
```

### When to Add File Headers

- **Always** when creating a new file.
- **When significantly modifying** an existing file that lacks a descriptive header.
- **Do NOT** remove or replace existing copyright notices — add the description block after them.
- **Note**: Most existing files only have a terse one-liner or no description at all (e.g., `/* Contains code for 'd', 'D' (drop), '>', '<' (up, down) */`). Expanding these when working on a file is encouraged.

---

## Function Documentation

Every function should have a documentation block immediately before its definition. This is especially critical in GnollHack because K&R-style definitions separate the parameter types from the parameter names, making the signature harder to read at a glance.

### Format

```c
/*
 * function_name -- Brief one-line description of what the function does
 *
 * Detailed explanation of the function's behavior, covering:
 * - What it does in game terms (player-visible effect)
 * - What it does technically (data structures modified)
 * - Important side effects (messages printed, sounds played)
 * - When/where it is typically called from
 *
 * Parameters:
 *   param1 -- Description of the parameter and valid values
 *   param2 -- Description of the parameter and valid values
 *
 * Returns:
 *   Description of the return value and its meaning.
 *   For boolean-like returns: what TRUE/FALSE (1/0) mean.
 *
 * Side effects:
 *   - List any global state modifications
 *   - List any messages printed to the player
 *   - List any map/display updates triggered
 */
```

### Full Example

```c
/*
 * attack_checks -- Validate whether the player can melee-attack
 *                  a monster
 *
 * Performs pre-attack validation. Checks whether the target is a
 * shopkeeper (triggers shop interaction instead), a peaceful
 * monster (prompts confirmation), or the player's pet (asks
 * "Really attack Fido?"). Also handles the cockatrice corpse
 * wielding special case (instant petrification of target).
 *
 * Parameters:
 *   mtmp -- The target monster. Must not be NULL. The monster's
 *           position (mtmp->mx, mtmp->my) is used for distance
 *           and line-of-sight checks.
 *
 * Returns:
 *   TRUE (1) if the attack should be aborted (player declined,
 *            or a special interaction replaced the attack).
 *   FALSE (0) if the attack should proceed normally.
 *
 * Side effects:
 *   - May print messages ("Really attack the peaceful kitten?")
 *   - May call shk_move() for shopkeeper behavior
 *   - May petrify the target (cockatrice corpse case)
 */
int
attack_checks(mtmp)
struct monst *mtmp;
{
    ...
}
```

### Short Functions

For trivial helper functions (< 5 lines, obvious purpose), a one-line comment is sufficient:

```c
/* Return TRUE if the object is a weapon that can be poisoned */
boolean
is_poisonable(otmp)
struct obj *otmp;
{
    ...
}
```

### Static Functions

Static (file-local) functions still need documentation but can use a shorter format:

```c
/*
 * place_lregion -- Position a level region (staircase, portal)
 *
 * Finds a valid coordinate within the given bounds for placing
 * a level region. Used during level generation.
 */
static void
place_lregion(x1, y1, x2, y2, rgn)
...
```

### Important: Do not use Doxygen/Javadoc-style tags

Do not use `@param`, `@return`, `\brief`, or similar markup. The project uses plain C block comments with descriptive text labels (`Parameters:`, `Returns:`, `Side effects:`).

---

## Inline Comments

Inline comments explain specific lines or small blocks of code. They are essential in GnollHack because much of the logic relies on implicit knowledge about game mechanics.

### When to Add Inline Comments

1. **Non-obvious conditionals** — Explain what a complex boolean expression tests in game terms
2. **Magic numbers** — Explain why a specific numeric value is used
3. **Global state access** — Explain what fields like `u.ulevel` or `mtmp->mhp` represent when context is not obvious
4. **Bitwise operations** — Explain what flags are being tested or set
5. **Fall-through cases** — Mark intentional `switch` fall-throughs with `/* FALLTHROUGH */`
6. **Workarounds** — Explain why code exists to handle an edge case
7. **Conditional compilation** — Explain `#ifdef` blocks that are not self-evident

### Examples

```c
/* Explain non-obvious conditionals */
if (u.uswallow) {
    /* Player has been engulfed by a monster — attacks target
     * the engulfing monster's interior, not the map tile */
    return attack_interior(u.ustuck);
}

/* Explain magic numbers and macros */
if (rn2(20) < ACURR(A_CHA)) {
    /* 1-in-20 base chance, improved by Charisma stat.
     * rn2(n) returns random int in [0, n-1].
     * ACURR(A_CHA) returns effective Charisma (3-25). */
    ...
}

/* Mark intentional fall-through */
switch (obj->otyp) {
case LONG_SWORD:
case SHORT_SWORD:
case BROADSWORD:
    /* FALLTHROUGH — all swords share the same wielding logic */
    use_sword(obj);
    break;
...
}

/* Explain a cryptic macro */
if (MON_AT(x, y)) {
    /* MON_AT(x,y): TRUE if a monster occupies tile (x,y).
     * Expands to a check on level.monsters[x][y]. */
    mtmp = m_at(x, y);
    ...
}

/* Explain conditional compilation */
#ifdef INSURANCE
    /* INSURANCE: enables periodic checkpoint saves so the game
     * can be recovered after a crash. Defined in config.h. */
    save_currentstate();
#endif
```

### Inline Comment Placement

- Place comments on the line **above** the code they describe, not at the end of the line (to stay within 78 characters).
- Group related comment + code as a visual unit with a blank line before the comment if it starts a new logical block.

```c
/* GOOD: Comment above */
/* Calculate damage reduction from armor class */
damage -= AC_VALUE(u.uac);

/* AVOID: Comment at end (easily exceeds 78 chars) */
damage -= AC_VALUE(u.uac); /* Calculate damage reduction */
```

### Section Separator Comments

Use separator comments to divide long functions into logical phases. The existing codebase uses this pattern effectively in [allmain.c](file:///c:/hmp/GnollHack/src/allmain.c):

```c
/********************************/
/* once-per-turn things go here */
/********************************/

/****************************************/
/* once-per-player-input things go here */
/****************************************/
```

---

## What NOT to Document

1. **Do not add comments to auto-generated files.** Files like `pm.h`, `onames.h`, `vis_tab.h`, `vis_tab.c`, `animoff.h`, and `animtotals.h` are generated by build utilities (`makedefs`). Comments will be overwritten.

2. **Do not restate obvious code.**
   ```c
   /* BAD: Adds no information */
   i++;  /* increment i */

   /* GOOD: Only comment when there's context to add */
   i++;  /* advance to the next tile in the column */
   ```

3. **Do not document dead code.** If code is commented out, either remove it or add a brief note explaining why it is retained (e.g., `/* Disabled pending rebalancing */`).

4. **Do not duplicate content from other skills.** The `adding_game_content` skill documents MON/WEAPON/ARMOR macro parameters. Do not re-document these in inline comments — instead, add comments that explain the *game design intent* of specific parameter choices.

---

## Documentation Checklist for AI Agents

When modifying a C file, apply the following checklist:

- [ ] **File header**: Does the file have a descriptive comment block after the copyright? If not, add one.
- [ ] **New functions**: Every new function has a doc block with purpose, parameters, return value, and side effects.
- [ ] **Modified functions**: If significantly modifying a function, add or update its documentation block.
- [ ] **Complex conditionals**: Are non-obvious boolean expressions explained in game terms?
- [ ] **Magic numbers**: Are numeric constants explained or replaced with named constants?
- [ ] **Game mechanics**: Would a developer unfamiliar with roguelikes understand what this code does?
- [ ] **Cross-system calls**: Are calls to other subsystems (display, sound, map) briefly annotated?
- [ ] **Global state**: Are accesses to `u`, `level`, `flags`, and other globals contextualized?
- [ ] **Comment style**: All comments use `/* */` format and respect 78-character line width.
- [ ] **No auto-generated files**: Documentation was not added to build-generated header files.
- [ ] **Existing comments preserved**: No existing comments were removed unless factually incorrect.

---

## Tone and Voice

- **Technical and precise** — use correct game terminology (e.g., "intrinsic resistance", "polymorph form", "dungeon branch")
- **Explanatory for newcomers** — assume the reader knows C but not NetHack internals
- **Concise but not terse** — a well-written two-line comment is better than a cryptic abbreviation
- **Neutral and factual** — avoid humor, personal opinions, or narrative style in comments

---

## Advanced Patterns

For detailed guidance on documenting macros, structs, enums, complex control flow, cross-system interactions, and common NetHack idioms, see [references/advanced_patterns.md](file:///c:/hmp/GnollHack/.agents/skills/c_code_documentation/references/advanced_patterns.md).
