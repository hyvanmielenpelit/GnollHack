---
name: nethack_c_core
description: Guidance on modifying the GnollHack C core game logic, headers, windowing callbacks, sound/animation definitions, and dungeon data files. Covers K&R coding style, macro idioms, and key data structures.
---

# NetHack C Core Development

GnollHack is a roguelike game built upon NetHack 3.6. The core game logic is written in C using traditional NetHack coding conventions.

## Directory Layout

| Directory | Purpose |
|---|---|
| [src/](file:///c:/hmp/GnollHack/src) | Core gameplay C source files (~120 files). Combat, movement, inventory, dungeon generation, windowing interfaces. |
| [include/](file:///c:/hmp/GnollHack/include) | Header files (~114 files). Struct definitions, prototypes, macros, platform config. |
| [dat/](file:///c:/hmp/GnollHack/dat) | Text-based data files: level descriptions (.des), rumors, quest text, dungeon definitions. |
| [util/](file:///c:/hmp/GnollHack/util) | Build-time utility programs: `makedefs`, `lev_comp`, `dgn_comp` for data compilation. |
| [win/win32/xpl/libshare/](file:///c:/hmp/GnollHack/win/win32/xpl/libshare) | C-side native API bridge for the .NET MAUI frontend (gnhapi.h, callback.h, libproc.c). |

---

## Key Header Files

| Header | Size | Purpose |
|--------|------|----------|
| [hack.h](file:///c:/hmp/GnollHack/include/hack.h) | 34KB | **Master include file.** Includes 25+ other headers. All gameplay `.c` files include this. |
| [decl.h](file:///c:/hmp/GnollHack/include/decl.h) | 25KB | Global variable declarations. Declares `u` (player), dungeon topology, game counters. |
| [extern.h](file:///c:/hmp/GnollHack/include/extern.h) | 211KB | All public function prototypes. New public functions in `src/` must be declared here. |
| [winprocs.h](file:///c:/hmp/GnollHack/include/winprocs.h) | 37KB | `struct window_procs` — the windowing callback interface (~120 function pointer fields). |
| [general.h](file:///c:/hmp/GnollHack/include/general.h) | 51KB | GnollHack-specific enums and constants: materials, zap types, explosion types, GUI commands, animation types. |
| [rm.h](file:///c:/hmp/GnollHack/include/rm.h) | 58KB | Map data model: `struct rm` (per-tile data), `dlevel_t` (full level), location types, room definitions. |
| [obj.h](file:///c:/hmp/GnollHack/include/obj.h) | 76KB | `struct obj` — the core item/object data structure with 64-bit flags and enchantment fields. |
| [monst.h](file:///c:/hmp/GnollHack/include/monst.h) | 16KB | `struct monst` — monster instance structure. |
| [permonst.h](file:///c:/hmp/GnollHack/include/permonst.h) | 5KB | `struct permonst` — per-monster-type static data (attacks, resistances, speed, weight). |
| [you.h](file:///c:/hmp/GnollHack/include/you.h) | 27KB | `struct you` — the player character structure. Also defines roles (13), races (5 incl. Gnoll), genders. |
| [flag.h](file:///c:/hmp/GnollHack/include/flag.h) | 41KB | `struct flag` — all persistent game options and runtime state flags. |
| [prop.h](file:///c:/hmp/GnollHack/include/prop.h) | 13KB | 194 intrinsic property types (resistances, senses, transportation, conditions). |
| [display.h](file:///c:/hmp/GnollHack/include/display.h) | 63KB | Rendering macros, monster sensing logic, warning system, newsym flags. |
| [objclass.h](file:///c:/hmp/GnollHack/include/objclass.h) | 66KB | `struct objclass` — per-object-type static definitions (weight, price, material, damage). |
| [soundset.h](file:///c:/hmp/GnollHack/include/soundset.h) | 154KB | Sound mapping and soundset trigger definitions (GnollHack-specific). |
| [animation.h](file:///c:/hmp/GnollHack/include/animation.h) | 125KB | Sprite animation definitions and animation data tables (GnollHack-specific). |

---

## Critical Global Variables

| Variable | Type | Declared In | Purpose |
|----------|------|-------------|----------|
| `u` | `struct you` | [decl.h:454](file:///c:/hmp/GnollHack/include/decl.h#L454) | The player character. Access fields like `u.ux`, `u.uy` (coordinates), `u.ulevel`, `u.uhunger`. |
| `level` | `dlevel_t` | [rm.h:1498](file:///c:/hmp/GnollHack/include/rm.h#L1498) | Current dungeon level. Contains `level.locations[x][y]`, `level.objects[x][y]`, `level.monsters[x][y]`. |
| `levl` | macro | [rm.h:1503](file:///c:/hmp/GnollHack/include/rm.h#L1503) | Shorthand for `level.locations`. Use `levl[x][y].typ` to check terrain type. |
| `flags` | `struct flag` | [decl.h](file:///c:/hmp/GnollHack/include/decl.h) | Game options and state flags. `flags.debug` aliased as `wizard`. |

### Map Dimensions
* `COLNO` = 80, `ROWNO` = 21 (defined in [global.h:375-376](file:///c:/hmp/GnollHack/include/global.h#L375))
* Valid coordinates: `1 <= x < COLNO`, `0 <= y < ROWNO`
* Always bounds-check before accessing `level.locations[x][y]`

---

## C Coding Style (from [DEVEL/code_style.txt](file:///c:/hmp/GnollHack/DEVEL/code_style.txt))

GnollHack uses **K&R-style** C function definitions:

```c
void
foo(i, c)
int i;
char c;
{
    /* function body */
}
```

**Key rules:**
* 4-space indentation, **no tabs**, 78-character line width
* Return type, declarator, and opening brace each on their own line
* K&R-style argument declarations (not ANSI prototype-style definitions)
* Control statement braces on the same line: `if (cond) {`
* `case` labels unindented within `switch`
* Variables never declared in `for` init or conditions

---

## Function Declaration Macros

NetHack uses macros from [tradstdc.h](file:///c:/hmp/GnollHack/include/tradstdc.h) for function prototypes in headers:

| Macro | Usage | Example |
|-------|-------|----------|
| `FDECL(name, (args))` | Functions with arguments | `E void FDECL(pline, (const char *, ...));` |
| `NDECL(name)` | Functions with no arguments | `E int NDECL(doredraw);` |
| `VDECL(name, (args))` | Variadic functions | `E void VDECL(pline, (const char *, ...)) PRINTF_F(1, 2);` |

* `E` is defined as `extern` in [decl.h](file:///c:/hmp/GnollHack/include/decl.h)
* All new public functions must have their prototype added to [extern.h](file:///c:/hmp/GnollHack/include/extern.h)

---

## Messaging Functions

| Function | Purpose |
|----------|----------|
| `pline(fmt, ...)` | Standard player message (like printf to the message log). |
| `pline_ex(sound, anim, fmt, ...)` | GnollHack extended: message with sound effect and animation trigger. |
| `pline_The(fmt, ...)` | Auto-capitalizes with "The". |
| `You(fmt, ...)` | Prefixes message with "You". |
| `Your(fmt, ...)` | Prefixes message with "Your". |
| `panic(fmt, ...)` | Fatal error: prints message, attempts crash save, terminates. |
| `impossible(fmt, ...)` | Non-fatal logic error: logs the issue but continues. |
| `raw_print(str)` | Direct console output, used before windowing is initialized. |

---

## Memory Management

* **`alloc(size)`**: Wraps `malloc()`. Calls `panic()` on allocation failure. Returns `long *`. Always use this instead of raw `malloc`.
* **`free(ptr)`**: Standard C `free()`. Used to release memory allocated by `alloc`.

---

## Key Source Files

| File | Size | Purpose |
|------|------|----------|
| [monst.c](file:///c:/hmp/GnollHack/src/monst.c) | 521KB | Static monster type table — all monster definitions. |
| [objects.c](file:///c:/hmp/GnollHack/src/objects.c) | 430KB | Static object/item table — all item definitions. |
| [soundset.c](file:///c:/hmp/GnollHack/src/soundset.c) | 1.1MB | Soundset data tables (auto-generated, very large). |
| [animdef.c](file:///c:/hmp/GnollHack/src/animdef.c) | 619KB | Animation definition tables. |
| [cmd.c](file:///c:/hmp/GnollHack/src/cmd.c) | 419KB | Command processing and key binding. |
| [allmain.c](file:///c:/hmp/GnollHack/src/allmain.c) | 66KB | Main game loop (`moveloop()`). |
| [windows.c](file:///c:/hmp/GnollHack/src/windows.c) | 125KB | Window proc registration and dispatch. |
| [display.c](file:///c:/hmp/GnollHack/src/display.c) | 191KB | Map rendering and glyph display logic. |

---

## Native API Bridge (C side)

The C core communicates with the .NET MAUI frontend through a callback-based API:

* [gnhapi.h](file:///c:/hmp/GnollHack/win/win32/xpl/libshare/gnhapi.h): Declares `RunGnollHack()` — the main entry point accepting ~100 callback function pointers and run flags.
* [callback.h](file:///c:/hmp/GnollHack/win/win32/xpl/libshare/callback.h): Defines 60+ callback typedefs (e.g., `PrintGlyphCallback`, `PlayImmediateSoundCallback`, `StatusUpdateCallback`).
* [libproc.c](file:///c:/hmp/GnollHack/win/win32/xpl/libshare/libproc.c): Implements the library windowing procs that bridge `window_procs` to the callback function pointers.
* Calling convention: `__stdcall` on Windows, default on other platforms.
* `DLLEXPORT` marks exported functions (`__declspec(dllexport)` on Windows).
