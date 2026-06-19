---
name: c99_migration_guide
description: Master reference for migrating the GnollHack C codebase from K&R C to C99, covering conversion phases, ordering dependencies, rules for what changes and what stays, per-file workflow, verification steps, and coordination between header and source file modernization.
---

# K&R to C99 Migration Guide

This skill is the master reference for converting the GnollHack C codebase from pre-C99 "K&R" conventions (inherited from NetHack 3.6.2) to modern C99. Read this skill first, then consult `c99_function_conversion` for detailed transformation patterns and `c99_header_modernization` for header infrastructure changes.

---

## Migration Scope

The conversion touches virtually every C file in the project. Here is a summary of all patterns that must change:

| Category | K&R Pattern | C99 Replacement | Approximate Count |
|----------|-------------|-----------------|-------------------|
| Function definitions | `void foo(x) int x; { }` | `void foo(int x) { }` | ~4,000–6,000 functions |
| FDECL macro | `E int FDECL(func, (int));` | `E int func(int);` | ~1,500+ in extern.h |
| NDECL macro | `E void NDECL(func);` | `E void func(void);` | ~500+ in extern.h |
| VDECL macro | `E void VDECL(func, (const char *, ...));` | `E void func(const char *, ...);` | ~20 in extern.h |
| STATIC_DCL | `STATIC_DCL int FDECL(foo, (int));` | `static int foo(int);` | Hundreds per-file |
| STATIC_OVL | `STATIC_OVL int foo(x) int x; { }` | `static int foo(int x) { }` | Hundreds per-file |
| STATIC_VAR | `STATIC_VAR int cached;` | `static int cached;` | Dozens |
| STATIC_PTR | `STATIC_PTR int callback(...)` | `static int callback(...)` | Dozens |
| BOOLEAN_P, CHAR_P, etc. | Type-widening macros in prototypes | Direct types: `boolean`, `char`, etc. | Thousands |
| VA_DECL macros | `void pline VA_DECL(const char *, line)` | `void pline(const char *line, ...)` | ~45 variadic functions |
| `register` keyword | `register struct obj *obj;` | `struct obj *obj;` | Thousands of occurrences |
| `genericptr`/`genericptr_t` | `typedef void *genericptr_t;` | `void *` directly | Dozens |
| `NEARDATA` | `NEARDATA struct obj *obj;` | `struct obj *obj;` | Hundreds |
| `VOID_ARGS` | `int func(VOID_ARGS)` | `int func(void)` | ~440 occurrences |
| OBJ_P / MONST_P / TRAP_P | Struct pointer macros | Direct `struct obj *`, etc. | ~50 |
| Pre-ANSI `#ifdef` guards | `#ifdef NHSTDC`, `#ifdef NOVOID`, etc. | Commented out | tradstdc.h, global.h |

---

## What Does NOT Change

These aspects of the code remain unchanged during the migration:

| Aspect | Rule |
|--------|------|
| **Game logic** | All algorithms, data structures, and behavior stay identical |
| **`boolean` type** | Keep `typedef xchar boolean;` as-is (user decision) |
| **`E` macro** | Keep `#define E extern` in extern.h (user decision) |
| **Indentation** | 4 spaces, no tabs |
| **Line width** | 78 characters maximum |
| **Return type placement** | Return type on its own line above the function name |
| **Brace style** | Opening brace on same line for control statements; own line for functions |
| **Comment style** | Keep `/* */` comments; do not convert to `//` |
| **`stdint.h` types** | Already used via integer.h — no changes needed to existing `uint64_t` etc. |
| **Switch/case formatting** | Case labels unindented within switch |
| **Existing comments** | Preserve all existing comments and documentation |
| **Dead platform code** | Comment out, do NOT delete (user decision) |
| **Function bodies** | Only signatures change, never logic |

---

## Conversion Phases

The migration must follow a strict phase order to maintain compilability at each step.

### Phase 1: Preparation

Before any code changes:

1. Update [code_style.txt](file:///c:/hmp/GnollHack/DEVEL/code_style.txt) to document the new C99 function style
2. Update [AGENTS.md](file:///c:/hmp/GnollHack/.agents/AGENTS.md) rules to reflect C99 conventions
3. Update the `nethack_c_core` skill to teach C99 style instead of K&R
4. Update the `c_code_documentation` skill if it references K&R patterns

### Phase 2: Header Infrastructure

Modernize headers in this order:

1. **Simplify [tradstdc.h](file:///c:/hmp/GnollHack/include/tradstdc.h)**: Comment out dead platform code and non-ANSI branches. Keep FDECL/NDECL/VDECL and VA_DECL macros active as pass-through during migration.
2. **Simplify [integer.h](file:///c:/hmp/GnollHack/include/integer.h)**: Require `<stdint.h>` unconditionally, comment out fallback typedefs.
3. **Simplify [hack.h](file:///c:/hmp/GnollHack/include/hack.h#L710-L729)**: Comment out OVERLAY branches, keep STATIC_DCL/STATIC_OVL as `static`.
4. **Convert [extern.h](file:///c:/hmp/GnollHack/include/extern.h)**: Remove FDECL/NDECL/VDECL wrappers, replace parameter type macros.
5. **Convert other headers**: winprocs.h, system.h, and any headers with FDECL usage.
6. **Clean up [global.h](file:///c:/hmp/GnollHack/include/global.h)**: Comment out dead platform PORT_ID blocks.

> After Phase 2, verify a full build succeeds before proceeding.

### Phase 3: Source File Conversion

Convert source files one at a time, in this directory order:

1. **`src/`** — Main game engine (~120 files). Start with small files to build confidence.
2. **`win/tty/`** — TTY display frontend
3. **`win/curses/`** — Curses display frontend
4. **`win/win32/xpl/libshare/`** — Native bridge (C side of P/Invoke)

For each file, follow the per-file checklist in the `c99_function_conversion` skill.

> After each file, verify it compiles. After each directory, verify a full build.

### Phase 4: Cleanup

After all source files are converted:

1. Comment out FDECL/NDECL/VDECL macro definitions in tradstdc.h (no longer used)
2. Comment out VA_DECL macro definitions in tradstdc.h (no longer used)
3. Comment out STATIC_DCL/STATIC_OVL/STATIC_VAR/STATIC_PTR in hack.h (no longer used)
4. Comment out BOOLEAN_P/CHAR_P/etc. definitions in tradstdc.h (no longer used)
5. Comment out OBJ_P/MONST_P/TRAP_P definitions in tradstdc.h
6. Verify no K&R patterns remain via grep searches
7. Full build verification on all platforms
8. Update `nethack_c_core` and `c_code_documentation` skills

---

## Directory Scope

The initial migration covers these directories:

| Directory | Contents | File Count |
|-----------|----------|------------|
| [src/](file:///c:/hmp/GnollHack/src) | Main game engine | ~120 .c files |
| [include/](file:///c:/hmp/GnollHack/include) | All C headers | ~114 .h files |
| [win/tty/](file:///c:/hmp/GnollHack/win/tty) | TTY display frontend | ~5 .c files |
| [win/curses/](file:///c:/hmp/GnollHack/win/curses) | Curses display frontend | ~10 .c files |
| [win/win32/xpl/libshare/](file:///c:/hmp/GnollHack/win/win32/xpl/libshare) | Native bridge (C) | ~5 .c files |

Directories NOT in scope for initial migration:

| Directory | Reason |
|-----------|--------|
| `util/` | Build utilities — convert later |
| `sys/` | Platform-specific code — convert later |
| `win/win32/` (other than libshare) | Windows-specific code — convert later |

---

## Per-File Conversion Workflow

For each `.c` file being converted, follow this exact sequence:

1. **Read the file** to understand its structure
2. **Convert forward declarations** at the top of the file:
   - `STATIC_DCL ... FDECL(name, (params));` → `static ... name(params);`
   - `STATIC_DCL ... NDECL(name);` → `static ... name(void);`
   - Replace all parameter type macros (BOOLEAN_P → boolean, etc.)
3. **Convert each function definition** from K&R to C99:
   - Move parameters into the parentheses with their types
   - Replace `STATIC_OVL` with `static`
   - Expand multi-variable K&R declarations (`int x, y;` → `int x, int y`)
   - Remove `register` from parameters
4. **Convert variadic functions** (if any):
   - Replace VA_DECL with standard C99 `(type name, ...)`
   - Replace VA_START/VA_INIT/VA_END with standard stdarg calls
   - Remove `#ifdef USE_STDARG`/`USE_VARARGS`/`USE_OLDARGS` branches
5. **Remove `register`** from all local variable declarations
6. **Replace `genericptr_t`/`genericptr`** with `void *`
7. **Remove `NEARDATA`** from variable declarations
8. **Replace `VOID_ARGS`** with `void`
9. **Replace `STATIC_VAR`** with `static`
10. **Replace `STATIC_PTR`** with `static`
11. **Verify the file compiles**

> See the `c99_function_conversion` skill for detailed before/after examples of each transformation.

---

## Verification Commands

### After Each File

Build the project targeting the current platform to check for compilation errors.

### After Each Directory

Full build of all targets:

```powershell
# Windows x64 build
msbuild win\win32\vs\GnollHack.sln /p:Configuration=Debug /p:Platform=x64

# Android build (if set up)
msbuild win\win32\vs\GnollHack.sln /p:Configuration=Debug /p:Platform="ARM64"
```

### Final Verification Greps

After all conversions, verify no K&R patterns remain:

```powershell
# Should return zero results in converted directories:
rg "FDECL\(" src/ include/
rg "NDECL\(" src/ include/
rg "VDECL\(" src/ include/
rg "STATIC_DCL" src/
rg "STATIC_OVL" src/
rg "STATIC_VAR" src/
rg "STATIC_PTR" src/
rg "BOOLEAN_P" src/ include/
rg "CHAR_P[^A-Z]" src/ include/
rg "XCHAR_P" src/ include/
rg "\bregister\b" src/
rg "VOID_ARGS" src/ include/
rg "NEARDATA" src/ include/
rg "genericptr_t" src/
rg "VA_DECL" src/
```

---

## Commenting Out Dead Code Convention

When commenting out dead platform code (per user decision), use this pattern:

```c
/* [C99-REMOVED] Pre-ANSI compatibility code commented out during C99 migration.
 * Original code preserved for reference.
 *
 * #ifdef NOVOID
 * #define void int
 * #endif
 *
 * [END C99-REMOVED] */
```

This makes it easy to:
- Find all commented-out code with `grep "C99-REMOVED"`
- Understand why it was removed
- Restore it if ever needed

---

## Handling Conflicts Between Phases

During the phased migration, some files will be converted while others are still K&R. This works because:

1. **FDECL/NDECL/VDECL are pass-through macros** on all modern compilers. `FDECL(func, (int))` already expands to `func(int)`. So converting extern.h declarations from `E int FDECL(func, (int));` to `E int func(int);` produces identical preprocessor output.

2. **STATIC_DCL/STATIC_OVL already expand to `static`** in non-OVERLAY builds (which is always the case for GnollHack). So `STATIC_OVL void foo(x) int x;` and `static void foo(int x)` can coexist.

3. **Parameter type macros** (BOOLEAN_P = boolean or int) are already defined to the actual types on Win32 (UNWIDENED_PROTOTYPES). Replacing them with the actual types is safe.

4. **The macros remain defined** throughout the migration until Phase 4 cleanup. Un-converted files still use them without error.

---

## Special Considerations

### P/Invoke Boundary

Functions in [libshare/](file:///c:/hmp/GnollHack/win/win32/xpl/libshare) are called from C# via P/Invoke. When converting these:
- The function signatures must not change at the ABI level
- Parameter types must remain the same size and signedness
- `boolean` (signed char) must stay as signed char, not change to `_Bool`
- Verify the C# `[DllImport]` declarations still match

### makedefs-Generated Headers

Do NOT hand-edit these auto-generated files:
- `include/date.h`
- `include/onames.h`
- `include/pm.h`
- `include/vis_tab.h`
- `include/animoff.h`
- `include/animtotals.h`
- `src/vis_tab.c`

These are generated by `makedefs` and will be regenerated during build.

### The `E` Macro

`#define E extern` is kept as-is per user decision. All external declarations in extern.h continue to use `E` instead of `extern`.

---

## Key Files Reference

| File | Role in Migration |
|------|--------------------|
| [tradstdc.h](file:///c:/hmp/GnollHack/include/tradstdc.h) | Central compatibility layer — major simplification |
| [extern.h](file:///c:/hmp/GnollHack/include/extern.h) | ~4,640 lines of function prototypes to convert |
| [global.h](file:///c:/hmp/GnollHack/include/global.h) | Type definitions, boolean typedef, dead platform code |
| [integer.h](file:///c:/hmp/GnollHack/include/integer.h) | Sized integer types — simplify to require stdint.h |
| [hack.h](file:///c:/hmp/GnollHack/include/hack.h) | STATIC_* macros, master include chain |
| [config.h](file:///c:/hmp/GnollHack/include/config.h) | schar/uchar typedefs, build config |
| [config1.h](file:///c:/hmp/GnollHack/include/config1.h) | Platform auto-detection |
| [code_style.txt](file:///c:/hmp/GnollHack/DEVEL/code_style.txt) | Must be updated for C99 style |
| [AGENTS.md](file:///c:/hmp/GnollHack/.agents/AGENTS.md) | Must be updated to document C99 conventions |
| [pline.c](file:///c:/hmp/GnollHack/src/pline.c) | Most complex VA_DECL patterns |
| [end.c](file:///c:/hmp/GnollHack/src/end.c) | panic() variadic function |
| [allmain.c](file:///c:/hmp/GnollHack/src/allmain.c) | Typical file structure example |
