---
name: nethack_c_core
description: Guidance on modifying the GnollHack C core game logic, headers, windowing callbacks, sound/animation definitions, and dungeon data files. Covers the migration from NetHack 3.6.2 K&R style to C99 with ANSI C89 preferences and Allman braces, macro idioms, and key data structures.
---

# NetHack C Core

## Critical Rules
- **Allman Brace Style**: GnollHack uses Allman brace placement — opening brace on its own line for both functions and control statements (`if`, `for`, `while`, `switch`, etc.).
- **Use ANSI Prototypes**: Always use standard ANSI C89/C99 prototypes and definitions.

### Migration state (what "legacy" means here)

GnollHack moved from NetHack 3.6.2's K&R style to **C99 with some ANSI C89
preferences** and Allman braces. Two different things are often conflated:

- **K&R function definitions are fully eliminated.** None remain anywhere in
  `src/`. The `FDECL` / `NDECL` / `VDECL` macros are still *defined* in
  `include/tradstdc.h`, but they have **zero call sites** — they are dead
  scaffolding. Finding them with grep does not mean the idiom is live. Never
  reintroduce them.
- **Legacy *formatting* still lingers** in places: K&R brace placement, older
  wrapping conventions.

All new code is Allman + C99. Convert legacy brace placement in code you are
already touching, but do not undertake unrelated reformatting passes.
- **4-space indent**, no tabs. `switch` `case` labels unindented.
- **Do NOT use standard library directly** for common operations if a NetHack wrapper exists (e.g., use `alloc()` instead of `malloc()`, `pline()` instead of `printf()`).

## Naming Conventions
- **Functions and variables**: `snake_case` (e.g., `min_npc_appearance_depth`, `signed_glyph`, `layer_flags`)
- **Struct fields**: `snake_case` (e.g., `animation_name`, `number_of_tiles`)
- **`#define` macros**: `ALL_CAPS_WITH_UNDERSCORES` (e.g., `NPC_FLAGS_NO_GENERATION`, `MAX_NPC_SUBTYPES`)
- **Enum values**: `ALL_CAPS_WITH_UNDERSCORES` (e.g., `LAYER_FLOOR`, `MAX_LAYERS`)
- **Single-statement bodies**: Both braced and unbraced are acceptable; use whichever is clearer. Do not use the comma operator to combine multiple assignments in a single-statement body.
- **Comments**: `/* */` is the preferred style for documentation and explanatory
  comments. `//` **is permitted** and is used in thousands of places in `src/`:
  for quick inline notes (`x = 5; //was 3`) and, in particular, for commenting
  out **several consecutive lines** of code, where `//` is generally better than
  `/* */` because it nests safely. `#if 0 ... #endif` is also acceptable for
  disabling a block when that reads more clearly (it appears in ~250 places
  across `src/`). **Do not "fix" existing `//` comments to `/* */`.**

## Key Data Structures
- **`struct monst`**: A monster instance. Tracked in the `fmon` linked list.
- **`struct obj`**: An item instance. Tracked in `invent` (player inventory) or `level.objects` (floor).
- **`struct you` (`u`)**: The player character state (`u.ux`, `u.uy`, `u.uhp`).
- **`dlevel_t` (`level`)**: The current dungeon level state.

## Window Procs (`winprocs.h`)
- The C core does not know about MAUI or Windows. It communicates via function pointers in `window_procs`.
- `display_nhwindow`, `putstr`, `getlin`: Examples of callbacks implemented in `libshare` and forwarded to C#.

## Macros & Idioms
- **`pline(format, ...)`**: Print a message to the top message log.
- **`rn2(n)`**: Random integer `0` to `n-1`.
- **`d(n, x)`**: Roll `n` dice with `x` sides.
- **`DEADMONSTER(mtmp)`**: Check if a monster has been killed but not yet removed from the list.
- **`STR18(x)` / `STR19(x)`**: Macros used to manage strength values beyond 18 (defined in `attrib.h`).
  - `STR18(x)` equals `18 + x`. It is used for AD&D-style 18/xx percentile strengths. For example, `STR18(100)` equals `118` and is displayed as `18/**`.
  - `STR19(x)` equals `100 + x`. It is used for linear strength values from 19 to 25. For example, `STR19(25)` equals `125`.
  - Display logic (in `botl.c`) handles values > 118 by subtracting 100 (`st - 100`), which is why `STR19(x)` smoothly displays exactly as `x` (e.g., `119 - 100 = 19`).

## Spelling
- **Archaeologist**: Always prefer the spelling "Archaeologist" (with the 'a' in the middle) instead of "Archeologist" throughout the codebase, documentation, and wiki.
