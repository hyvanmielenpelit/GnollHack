---
name: nethack_c_core
description: Guidance on modifying the GnollHack C core game logic, headers, windowing callbacks, sound/animation definitions, and dungeon data files. Covers K&R coding style, macro idioms, and key data structures.
---

# NetHack C Core

## Critical Rules
- **Match Surrounding Style**: GnollHack is migrating to C99. Match the file's current style (K&R or C99). Use 4-space indent, opening brace on same line for control structures, `switch` case labels unindented.
- **Do NOT use standard library directly** for common operations if a NetHack wrapper exists (e.g., use `alloc()` instead of `malloc()`, `pline()` instead of `printf()`).

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

## Spelling
- **Archaeologist**: Always prefer the spelling "Archaeologist" (with the 'a' in the middle) instead of "Archeologist" throughout the codebase, documentation, and wiki.
