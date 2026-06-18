---
name: nethack_c_core
description: Guidance on modifying the GnollHack C core game logic, headers, and windowing callbacks, including animations, sound definitions, and dungeon data files.
---

# NetHack C Core Development

This guide outlines the structure, design patterns, and protocols for developing and debugging the C core of GnollHack.

## Codebase Layout

The core of the game is written in C (C99/C11 compatible) and is organized as follows:
* **[src/](file:///c:/hmp/GnollHack/src)**: Source code files containing the core gameplay loop, systems (combat, movement, inventory), and window/sound driver interfaces.
* **[include/](file:///c:/hmp/GnollHack/include)**: Header files containing structure definitions, function prototypes, macro helpers, and platform configuration defines.
* **[dat/](file:///c:/hmp/GnollHack/dat)**: Text-based dungeon layout, rumors, quest files, and compiler specifications.
* **[util/](file:///c:/hmp/GnollHack/util)**: Code and scripts for utilities compiled during build-time (e.g. `makedefs`, `lev_comp`, `dgn_comp`) to pre-compile game data.

---

## Important Header Files

Always include or refer to these primary headers when adding new features or tracing execution paths:

* **[hack.h](file:///c:/hmp/GnollHack/include/hack.h)**: The main header file for gameplay source files. Includes most other core headers. Always review it first for fundamental types.
* **[decl.h](file:///c:/hmp/GnollHack/include/decl.h)**: Declares all global game state variables, such as the current dungeon level configuration, the player character properties (`you`), and active monsters/objects.
* **[extern.h](file:///c:/hmp/GnollHack/include/extern.h)**: Lists all public function declarations across the entire C core. If you create a new public helper in `src/`, its prototype must be declared here.
* **[winprocs.h](file:///c:/hmp/GnollHack/include/winprocs.h)**: Contains the definition of the `window_procs` struct which handles drawing layout callbacks.

---

## Key Core Entities & Customizations

GnollHack heavily extends the original NetHack 3.4.3/3.6 backend. Pay special attention to these customized subsystems:

### 1. Monsters and Objects
* **Monsters**: Look up definitions in [monst.h](file:///c:/hmp/GnollHack/include/monst.h) and [permonst.h](file:///c:/hmp/GnollHack/include/permonst.h). The static monster type table is defined in [monst.c](file:///c:/hmp/GnollHack/src/monst.c).
* **Objects**: Defined in [objclass.h](file:///c:/hmp/GnollHack/include/objclass.h). The static objects table containing item statistics (weight, price, color, damage) is defined in [objects.c](file:///c:/hmp/GnollHack/src/objects.c).

### 2. Audio & Soundsets (GnollHack Specific)
* Sound mapping and soundset triggers are defined in [soundset.h](file:///c:/hmp/GnollHack/include/soundset.h).
* Implementation of voiceovers, background music, and localized sound triggers are in [sounds.c](file:///c:/hmp/GnollHack/src/sounds.c) and the auto-generated/large table [soundset.c](file:///c:/hmp/GnollHack/src/soundset.c).

### 3. Graphic Animations & Tiles (GnollHack Specific)
* Sprite animations are managed in [animation.h](file:///c:/hmp/GnollHack/include/animation.h) and implemented in [animation.c](file:///c:/hmp/GnollHack/src/animation.c) / [animdef.c](file:///c:/hmp/GnollHack/src/animdef.c).
* Custom tile metadata mapping maps raw NetHack glyphs to high-resolution tile sprites in [tiledata.h](file:///c:/hmp/GnollHack/include/tiledata.h) and [tiledata.c](file:///c:/hmp/GnollHack/src/tiledata.c).

---

## NetHack C Programming Idioms

When writing or modifying core C code, you must conform to standard NetHack programming patterns:

### Memory Management
* Never use raw standard `malloc` or `free`. Use NetHack's wrappers:
  * `alloc(size)` returns a pointer and automatically handles out-of-memory errors by calling `panic`.
  * `free(ptr)` releases memory allocated by `alloc`.

### Console & UI Messaging
* **`pline(format, ...)`**: The standard way to output inline game messages to the player's message log (comparable to `printf`).
* **`panic(format, ...)`**: Call this when a critical system error occurs (e.g., failed resource loading, corrupt save files). It prints the error message, attempts to save the game in a crash file, and terminates execution.

### Dungeon Grid Coordinates
* Map grid positions are stored in two-dimensional coordinates. In NetHack, the grid size is `COLNO` (columns, typically 80) by `ROWNO` (rows, typically 22).
* Top-left is `(1, 0)`. Coordinate `0` on the x-axis is reserved for boundary markers.
* Always check coordinate bounds before querying dungeon structures like `level.locations[x][y]`.
