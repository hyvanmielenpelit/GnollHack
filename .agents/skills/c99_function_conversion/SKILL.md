---
name: c99_function_conversion
description: Patterns for converting K&R C function definitions to C99 ANSI-style prototypes, removing STATIC_DCL/STATIC_OVL macros, converting VA_DECL variadic functions, removing register keywords, and eliminating parameter type macros (BOOLEAN_P, CHAR_P, etc.).
---

# C99 Function Conversion

## Critical Rules
- **Phase Order**: Convert one `.c` file at a time. Never leave a file half-converted. Verify it compiles.
- **Parameter Order**: In C99, parameter order MUST match the name list in the K&R parenthesis, NOT the order of the K&R type declarations.
- **Do NOT modify function bodies**. Only alter signatures and declarations.
- **Maintain 78-char width**: Break lines after commas and align with the opening parenthesis. Keep return type on its own line.

## Core Conversions
- **`STATIC_DCL`** → `static`
  - Remove `FDECL`/`NDECL` wrappers: `STATIC_DCL int FDECL(foo, (int));` → `static int foo(int);`
- **`STATIC_OVL`** → `static`
- **`STATIC_VAR`** / **`STATIC_PTR`** → `static`
- **`VOID_ARGS`** → `void`
- **`register`** → remove entirely (from parameters and locals)
- **`genericptr` / `genericptr_t`** → `void *`
- **`NEARDATA`** → remove entirely

## K&R to C99 Parameters
- **Expand grouped parameters**: `xchar x, y;` MUST become `xchar x, xchar y`.
- **Type replacements** (`tradstdc.h` macros):
  - `BOOLEAN_P` → `boolean`
  - `CHAR_P` → `char`
  - `SCHAR_P` → `schar`
  - `UCHAR_P` → `uchar`
  - `XCHAR_P` → `xchar`
  - `SHORT_P` → `short`
  - `ALIGNTYP_P` → `aligntyp`
  - `OBJ_P` → `struct obj *`
  - `MONST_P` → `struct monst *`
  - `TRAP_P` → `struct trap *`

## Example
**Before:**
```c
STATIC_OVL boolean
goodpos(x, y, mtmp, gpflags)
int x, y;
struct monst *mtmp;
uint64_t gpflags;
{
```
**After:**
```c
static boolean
goodpos(int x, int y, struct monst *mtmp, uint64_t gpflags)
{
```

## Variadic Functions (`VA_DECL`)
- See `references/variadic_conversion.md` for full details.
- Replace `VA_DECL(type, name)` with `(type name, ...)`
- Add `va_list the_args;`
- Replace `VA_START(name)` with `va_start(the_args, name)`
- Replace `VA_ARGS` with `the_args`
- Replace `VA_END()` with `va_end(the_args);` (Do NOT add closing brace).
- Remove `#ifdef USE_STDARG` blocks and fallback branches.
