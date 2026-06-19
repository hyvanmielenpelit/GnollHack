---
name: c99_header_modernization
description: Guide to modernizing GnollHack header infrastructure for C99, covering tradstdc.h simplification, extern.h prototype conversion, removal of compatibility macros (FDECL, NDECL, VDECL, BOOLEAN_P, CHAR_P), and cleanup of dead platform-specific code.
---

# C99 Header Infrastructure Modernization

This skill covers how to modernize the header files that contain the K&R
compatibility macro infrastructure in GnollHack. The headers in `include/`
carry extensive legacy support for pre-ANSI C compilers, dead platforms
(Ultrix, Apollo, Amiga, OS/2, classic Mac, etc.), and K&R-style prototype
workarounds that are no longer needed in a C99 world.

Cross-references: see `c99_migration_guide` for the overall phased migration
strategy, and `c99_function_conversion` for converting K&R function
definitions in `.c` source files.

> **Critical**: Header changes must be coordinated carefully with source file
> conversion. The compatibility macros in `tradstdc.h` must remain functional
> while source files are being converted in the phased approach. Only comment
> out macros after their last usage is removed.

> **User decision**: Dead platform code must be **commented out**, not
> deleted. Use `/* C99-REMOVED: ... */` block comments or `#if 0` guards.

---

## tradstdc.h Modernization

[tradstdc.h](file:///c:/hmp/GnollHack/include/tradstdc.h) is the central
compatibility header. Currently 485 lines, it should shrink to ~80–100 lines
after full modernization.

### What to Comment Out (Not Delete)

| Lines | Content | Reason |
|-------|---------|--------|
| 11–17 | `NOVOID` / `#define void int` | No compiler needs this |
| 25–27 | `NHSTDC` detection logic | Assume C99; always defined |
| 29–39 | Ultrix compatibility | Dead platform |
| 56–57 | `USE_VARARGS` / `USE_OLDARGS` commented defines | Dead approaches |
| 59–67 | Apollo / NHSTDC / ULTRIX_PROTO varargs selection | Dead platforms |
| 134–199 | `USE_VARARGS` and `USE_OLDARGS` VA_DECL branches | Only USE_STDARG needed |
| 232 | `VOID_ARGS` macro | Use `void` directly |
| 234–293 | `genericptr` / `genericptr_t` definitions | Use `void *` directly |
| 247–264 | `#define const` / `#define signed` / `#define volatile` | C99 has these |
| 266–289 | Non-NHSTDC branch of FDECL/NDECL/VDECL | Traditional C dead |
| 285–287 | Traditional C `#define signed` / `const` / `volatile` | C99 has these |
| 295–304 | `HAS_PTR_FMT` macro | All modern compilers support `%p` |
| 306–394 | `WIDENED_PROTOTYPES` / `UNWIDENED_PROTOTYPES` | C99 has proper prototypes |
| 367–394 | `BOOLEAN_P`, `CHAR_P`, `SCHAR_P`, `UCHAR_P`, `XCHAR_P`, `SHORT_P`, `ALIGNTYP_P` | Replace with actual types |
| 396–411 | `OBJ_P`, `MONST_P`, `TRAP_P` macros | Use `struct obj *` directly |
| 413–436 | SGI workaround (already in `#if 0`) | Dead platform |
| 438–442 | MetaWare/AIX `signed` workaround | Dead platforms |

### What to Keep Active

| Content | Reason |
|---------|--------|
| `#ifndef TRADSTDC_H` / `#define TRADSTDC_H` guard | Standard include guard |
| File header comments (lines 1–6) | Attribution |
| `PRINTF_F`, `UNUSED`, `NORETURN` attribute macros (lines 444–482) | Useful compiler-specific functionality |
| `#include <stdarg.h>` | Move to unconditional (outside `NEED_VARARGS`) |
| `__clang__` → `__GNUC__` compat (lines 444–449) | Still useful |
| FDECL/NDECL/VDECL (pass-through form) | Keep during phased migration |
| VA_DECL macros (USE_STDARG branch only) | Keep until all VA_DECL usage removed |

### Phased Approach

**Phase 1** — Comment out dead platform code and non-ANSI branches. Keep
FDECL/NDECL/VDECL active as pass-through macros:

```c
/* Keep these active during phased migration */
#define NDECL(f) f(void)
#define FDECL(f, p) f p
#define VDECL(f, p) f p
```

**Phase 2** — After ALL FDECL/NDECL/VDECL usages are removed from every
header and source file: comment out the macros themselves.

**Phase 3** — After ALL VA_DECL usages are removed from every source file:
comment out the VA_DECL macros.

### Target State After Full Modernization

```c
/* GnollHack 4.0    tradstdc.h */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef TRADSTDC_H
#define TRADSTDC_H

#include <stdarg.h>

/* C99-REMOVED: All K&R compatibility macros, NHSTDC detection,
 * WIDENED_PROTOTYPES, BOOLEAN_P/CHAR_P type aliases,
 * genericptr, VOID_ARGS, USE_VARARGS, USE_OLDARGS,
 * and dead platform workarounds have been commented out.
 * See original source for historical definitions. */

#ifdef __clang__
#ifndef __GNUC__
#define __GNUC__ 4
#endif
#endif

#ifdef __GNUC__
#if defined(ANDROID) || defined(GNH_MOBILE)
#define PRINTF_F(f,v) __attribute__ ((format (__printf__, f, v)))
#elif (__GNUC__ >= 2)
#define PRINTF_F(f, v) __attribute__((format(printf, f, v)))
#endif
#if __GNUC__ >= 3
#define UNUSED __attribute__((unused))
#define NORETURN __attribute__((noreturn))
#if defined(GCC_URWARN)
#define __warn_unused_result__ /*empty*/
#define warn_unused_result /*empty*/
#endif
#endif
#endif

#ifndef PRINTF_F
#define PRINTF_F(f, v)
#endif
#ifndef UNUSED
#define UNUSED
#endif
#ifndef NORETURN
#define NORETURN
#endif

#endif /* TRADSTDC_H */
```

---

## extern.h Conversion

[extern.h](file:///c:/hmp/GnollHack/include/extern.h) contains ~4,640 lines
of function declarations using `FDECL`, `NDECL`, and `VDECL` macros, plus
the `_P` parameter type macros.

### Conversion Rules

```c
/* BEFORE */
E void FDECL(moveloop, (UCHAR_P));
E void NDECL(stop_occupation);
E void VDECL(panic, (const char *, ...)) PRINTF_F(1, 2);
E struct obj *FDECL(mksobj, (int, BOOLEAN_P, BOOLEAN_P, BOOLEAN_P));
E void FDECL(welcome, (BOOLEAN_P));
E int FDECL(still_chewing, (XCHAR_P, XCHAR_P));
E void FDECL(add_class, (char *, CHAR_P));

/* AFTER */
E void moveloop(uchar);
E void stop_occupation(void);
E void panic(const char *, ...) PRINTF_F(1, 2);
E struct obj *mksobj(int, boolean, boolean, boolean);
E void welcome(boolean);
E int still_chewing(xchar, xchar);
E void add_class(char *, char);
```

### Parameter Type Macro Replacements

| Macro | Replacement |
|-------|-------------|
| `BOOLEAN_P` | `boolean` |
| `CHAR_P` | `char` |
| `SCHAR_P` | `schar` |
| `UCHAR_P` | `uchar` |
| `XCHAR_P` | `xchar` |
| `SHORT_P` | `short` |
| `UNSIGNED_SHORT_P` | `unsigned short` |
| `ALIGNTYP_P` | `aligntyp` |
| `OBJ_P` | `struct obj *` |
| `MONST_P` | `struct monst *` |
| `TRAP_P` | `struct trap *` |

### What to Preserve

- `#define E extern` (line 18) and `#undef E` at end — per user decision
- Section comments: `/* ### filename.c ### */`
- `#if !defined(MAKEDEFS_C)` guards (line 29)
- `PRINTF_F`, `UNUSED`, `NORETURN` attributes on declarations
- All `#include` directives at the top
- `#if 0` blocks (already-disabled declarations)
- `#ifdef` / `#ifndef` guards for platform-specific sections

### Conversion Strategy

extern.h should be converted **early** (Phase 2 of the overall migration)
because FDECL/NDECL/VDECL are already pass-through macros on all modern
compilers. The resulting declarations are **binary-identical** before and
after conversion.

Convert in chunks matching the `/* ### filename.c ### */` sections to keep
diffs reviewable. Example workflow:

1. Pick a section, e.g., `/* ### allmain.c ### */`
2. Convert all FDECL/NDECL/VDECL in that section
3. Replace all `_P` type macros
4. Build and verify
5. Commit that section
6. Repeat for the next section

---

## global.h Modernization

[global.h](file:///c:/hmp/GnollHack/include/global.h) (492 lines) defines
fundamental types and game constants.

### What to Keep As-Is

| Item | Line | Reason |
|------|------|--------|
| `typedef schar xchar;` | 73 | Core type alias |
| `typedef xchar boolean;` | 80 | Per user decision — keep unchanged |
| `TRUE` / `FALSE` definitions | 84–87 | Standard boolean constants |
| All game constants (`COLNO`, `ROWNO`, `BUFSZ`, etc.) | various | Core gameplay |
| `struct version_info`, `struct savefile_info` | various | Save file format |
| `Sprintf` / `Strcat` / `Strcpy` / `Strncpy` macros | various | Used throughout |

### What to Comment Out

| Lines | Content | Action |
|-------|---------|--------|
| 75–77 | MinGW-specific `boolean` typedef | Evaluate if still needed; comment out if not |
| 356–364 | `#ifdef NHSTDC` guards around `SFI1_*` macros | Use only the `UL` versions unconditionally |
| various | Dead platform `PORT_ID` blocks (Amiga, Mac Classic, OS/2, TOS, etc.) | Comment out; keep Windows, Unix, macOS, Android, iOS |
| 59–66 | `DUMB` / `STUPID` preprocessor workarounds | Comment out |

### NEARDATA Macro

`NEARDATA` should be removed from `global.h` only **after** it has been
removed from all usage sites in source files. Search for all occurrences
first:

```powershell
rg "NEARDATA" include/ src/ --files-with-matches
```

---

## integer.h Simplification

[integer.h](file:///c:/hmp/GnollHack/include/integer.h) (105 lines) provides
sized integer types with fallbacks for pre-C99 compilers.

### Target: Require `<stdint.h>` Unconditionally

```c
/* BEFORE: 105 lines with fallback */
#ifdef HAS_STDINT_H
#include <stdint.h>
#else
typedef unsigned char uint8_t;
/* ...more fallbacks... */
#endif

/* AFTER: ~30 lines */
#ifndef INTEGER_H
#define INTEGER_H

#include <stdint.h>

typedef uint8_t uint8;
typedef int16_t int16;
typedef uint16_t uint16;
typedef int32_t int32;
typedef uint32_t uint32;
typedef int64_t int64;
typedef uint64_t uint64;

#endif /* INTEGER_H */
```

### What to Comment Out

- VMS / DEC C `HAS_INTTYPES_H` workaround (lines 37–53)
- `__STDC_VERSION__` detection logic (lines 44–51)
- `SKIP_STDINT_WORKAROUND` mechanism (lines 55–63)
- All fallback typedefs in the `STDINT_WORKAROUND` section (lines 65–93)
- Watcom 16-bit workaround (lines 73–83)

Keep the file header comments and the `uint8`–`uint64` convenience typedefs.

---

## hack.h Changes

[hack.h](file:///c:/hmp/GnollHack/include/hack.h) (856 lines) is the master
include file. It defines the `STATIC_*` macros used for overlay support.

### STATIC_DCL / STATIC_OVL / STATIC_VAR / STATIC_PTR

These macros are defined at [lines 710–729](file:///c:/hmp/GnollHack/include/hack.h#L710):

```c
/* Current code */
#if defined(OVERLAY)
#define USE_OVLx
#define STATIC_DCL extern
#define STATIC_OVL
#define STATIC_VAR
#else
#define STATIC_DCL static
#define STATIC_OVL static
#define STATIC_VAR static
#endif

#if defined(USE_TRAMPOLI) || defined(USE_OVLx)
#define STATIC_PTR
#else
#define STATIC_PTR static
#endif
```

**Phase 1** — Simplify by commenting out the `OVERLAY` branch:

```c
/* C99-REMOVED: OVERLAY support branches commented out */
/* Simplified — overlay support removed */
#define STATIC_DCL static
#define STATIC_OVL static
#define STATIC_VAR static
#define STATIC_PTR static
```

**Phase 2** — After all `STATIC_DCL`, `STATIC_OVL`, `STATIC_VAR`, and
`STATIC_PTR` usages are replaced with plain `static` in source files:
comment out the macros entirely.

### Other hack.h Cleanup

| Lines | Content | Action |
|-------|---------|--------|
| 705–708 | `MICRO` `getuid()` / `getlogin()` macros | Evaluate if still needed; comment out if not |
| 735–739 | `CFDECLSPEC` / `WIN_CE` `__cdecl` macro | Comment out — WIN_CE is dead |

---

## config.h and config1.h Changes

Minimal changes needed in these files.

### config.h

[config.h](file:///c:/hmp/GnollHack/include/config.h) (667 lines) handles
platform detection and feature toggles.

- The `schar` and `uchar` typedefs are fine — keep as-is
- Comment out `NOTSTDC` flag if present (this was used to force non-ANSI
  mode on compilers that otherwise set `__STDC__`)
- Comment out ancient platform defines that are no longer relevant

### config1.h

[config1.h](file:///c:/hmp/GnollHack/include/config1.h) handles early
platform detection before `config.h`.

- Comment out dead platform detections (Amiga, Atari TOS, OS/2, etc.)
- Keep Windows (`WIN32`), Unix, macOS, Android, iOS detections

---

## Other Header Files

Many headers in `include/` use `FDECL` and `NDECL` in prototype
declarations. These need the same conversion as `extern.h`.

### winprocs.h

[winprocs.h](file:///c:/hmp/GnollHack/include/winprocs.h) contains `struct
window_procs` with function pointer members using FDECL. Example:

```c
/* BEFORE — struct window_procs members */
    void FDECL((*win_init_nhwindows), (int *, char **));
    void FDECL((*win_display_nhwindow), (winid, BOOLEAN_P));
    void NDECL((*win_player_selection));

/* AFTER */
    void (*win_init_nhwindows)(int *, char **);
    void (*win_display_nhwindow)(winid, boolean);
    void (*win_player_selection)(void);
```

Convert all function pointer members and any FDECL/NDECL declarations
in the struct.

### system.h

[system.h](file:///c:/hmp/GnollHack/include/system.h) contains system
function declarations using FDECL/NDECL. Apply the same conversion rules.

### Other *conf.h Files

Platform-specific configuration headers may contain FDECL/NDECL usage.
Convert using the same rules as extern.h.

---

## Coordination with Source File Conversion

The ordering of header vs. source file changes is critical for maintaining
compilability throughout the migration.

### Required Order

1. **Convert extern.h declarations FIRST** — FDECL/NDECL/VDECL are already
   pass-through macros, so removing the wrappers produces identical
   declarations. This is safe at any time.

2. **Convert other header prototypes** (winprocs.h, system.h, etc.) — same
   reasoning as extern.h.

3. **Convert `.c` files one at a time** — each file should compile
   successfully after conversion. See `c99_function_conversion` for details.

4. **After ALL `.c` files in a directory are converted**, verify a full
   build of the entire project.

5. **Only after ALL source files are converted**: simplify `tradstdc.h` by
   commenting out the now-unused compatibility macros.

> **Critical rule**: During the phased migration, `tradstdc.h` must still
> define all the compatibility macros (FDECL, NDECL, VDECL, BOOLEAN_P,
> CHAR_P, VA_DECL, etc.). Only comment them out after their **last** usage
> is removed from every file in scope.

### Verification Commands

After each batch of header changes, verify the build:

```powershell
# Windows build verification
msbuild win\win32\vs\GnollHack.sln /t:Build /p:Configuration=Debug /p:Platform=x64
```

Search for remaining macro usages to track progress:

```powershell
# Count remaining FDECL/NDECL/VDECL usages
rg "FDECL\(|NDECL\(|VDECL\(" include/ src/ --count-matches

# Count remaining _P type macro usages
rg "BOOLEAN_P|CHAR_P|SCHAR_P|UCHAR_P|XCHAR_P|SHORT_P|ALIGNTYP_P" include/ src/ --count-matches
```

---

## Key Files Reference

| File | Lines | What Changes |
|------|-------|--------------|
| [tradstdc.h](file:///c:/hmp/GnollHack/include/tradstdc.h) | 485 | Major simplification — most content commented out |
| [extern.h](file:///c:/hmp/GnollHack/include/extern.h) | 4,640 | Remove FDECL/NDECL/VDECL wrappers, replace type macros |
| [global.h](file:///c:/hmp/GnollHack/include/global.h) | 492 | Comment out dead platform code, NHSTDC guards |
| [integer.h](file:///c:/hmp/GnollHack/include/integer.h) | 105 | Require stdint.h unconditionally |
| [hack.h](file:///c:/hmp/GnollHack/include/hack.h) | 856 | Simplify STATIC_* macros, remove OVERLAY branches |
| [config.h](file:///c:/hmp/GnollHack/include/config.h) | 667 | Minor — comment out NOTSTDC |
| [config1.h](file:///c:/hmp/GnollHack/include/config1.h) | varies | Comment out dead platform detection |
| [winprocs.h](file:///c:/hmp/GnollHack/include/winprocs.h) | varies | Convert FDECL in function pointer types |
| [system.h](file:///c:/hmp/GnollHack/include/system.h) | varies | Convert FDECL/NDECL declarations |
