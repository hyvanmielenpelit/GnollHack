---
name: c99_header_modernization
description: Guide to modernizing GnollHack header infrastructure for C99, covering tradstdc.h simplification, extern.h prototype conversion, removal of compatibility macros (FDECL, NDECL, VDECL, BOOLEAN_P, CHAR_P), and cleanup of dead platform-specific code.
---

# C99 Header Modernization

## Critical Rules
- **Do NOT delete dead platform code**. Use block comments `/* C99-REMOVED: ... */` to comment out dead code (e.g., Amiga, OS/2, Ultrix).
- **Phased Approach**: Leave `FDECL`/`NDECL`/`VDECL` and parameter type macros in `tradstdc.h` as pass-throughs until ALL `.c` files are fully converted to C99. Only comment them out in `tradstdc.h` at the very end.

## `extern.h` Conversion
- Remove all `FDECL`, `NDECL`, and `VDECL` wrappers.
- Replace `_P` macros (`BOOLEAN_P` → `boolean`, `CHAR_P` → `char`, etc.).
- **Example:**
  `E struct obj *FDECL(mksobj, (int, BOOLEAN_P));`
  → `E struct obj *mksobj(int, boolean);`
- Convert section by section (matching the `/* ### filename.c ### */` comments).

## `tradstdc.h` Modernization
- **Phase 1**: Comment out dead platforms (Ultrix, Apollo, SGI) and pre-ANSI fallbacks (`NOVOID`, `NHSTDC`).
- **Phase 2** (After source files are converted): Comment out `FDECL`, `NDECL`, `VDECL`, `BOOLEAN_P`, `genericptr`, `VOID_ARGS`, `VA_DECL*` definitions.
- **Keep**: `#include <stdarg.h>`, `PRINTF_F`, `UNUSED`, `NORETURN`.

## `hack.h` & `global.h` & `integer.h`
- **`hack.h`**: Remove `OVERLAY` branches. `STATIC_DCL`, `STATIC_OVL`, `STATIC_VAR`, `STATIC_PTR` should just be `#define STATIC_... static`. Eventually comment them out entirely.
- **`global.h`**: Keep `schar`, `xchar`, `boolean`, `TRUE`/`FALSE`. Comment out `NEARDATA` ONLY AFTER removing it from all source files.
- **`integer.h`**: Make `#include <stdint.h>` unconditional. Remove Watcom and VMS fallbacks. Keep `uint8`, `int16`, etc., typedefs.
