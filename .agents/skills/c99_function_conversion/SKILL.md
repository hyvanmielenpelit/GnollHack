---
name: c99_function_conversion
description: Patterns for converting K&R C function definitions to C99 ANSI-style prototypes, removing STATIC_DCL/STATIC_OVL macros, converting VA_DECL variadic functions, removing register keywords, and eliminating parameter type macros (BOOLEAN_P, CHAR_P, etc.).
---

# C99 Function Conversion Patterns

This skill covers the mechanical transformations required to convert
K&R-style C function definitions, declarations, and associated macros
to standard C99/ANSI style in the GnollHack codebase. See
`c99_migration_guide` for phase ordering and overall strategy. See
`c99_header_modernization` for changes to `extern.h`, `tradstdc.h`,
and other infrastructure headers.

The migration approach is **phased**: convert one `.c` file at a time,
verify it compiles, then move on. Never leave a file in a half-converted
state.

For variadic function conversion (VA_DECL patterns), see
[references/variadic_conversion.md](file:///c:/hmp/GnollHack/.agents/skills/c99_function_conversion/references/variadic_conversion.md).
For additional edge-case examples, see
[references/pattern_catalog.md](file:///c:/hmp/GnollHack/.agents/skills/c99_function_conversion/references/pattern_catalog.md).

---

## Regular Function Definitions

Convert K&R parameter declarations (listed after the closing parenthesis
but before the opening brace) into C99 inline parameter declarations.

**Single argument:**
```c
/* BEFORE */
void
moveloop(resuming)
uchar resuming;
{

/* AFTER */
void
moveloop(uchar resuming)
{
```

**Multiple arguments, same type — must expand each:**
```c
/* BEFORE */
anything*
xy_to_any(x, y)
xchar x, y;
{

/* AFTER */
anything*
xy_to_any(xchar x, xchar y)
{
```

> When K&R declares `xchar x, y;` on one line, you **must** expand to
> `xchar x, xchar y` in the C99 parameter list. Each parameter needs
> its own type specifier.

**Multiple arguments, mixed types:**
```c
/* BEFORE */
void
sanity_check_single_mon(mtmp, chk_geno, msg)
struct monst *mtmp;
boolean chk_geno;
const char *msg;
{

/* AFTER */
void
sanity_check_single_mon(struct monst *mtmp, boolean chk_geno,
                        const char *msg)
{
```

**Struct pointer plus flags:**
```c
/* BEFORE */
boolean
goodpos(x, y, mtmp, gpflags)
int x, y;
struct monst *mtmp;
uint64_t gpflags;
{

/* AFTER */
boolean
goodpos(int x, int y, struct monst *mtmp, uint64_t gpflags)
{
```

**No arguments:**
```c
/* BEFORE */
void
stop_occupation()
{

/* AFTER */
void
stop_occupation(void)
{
```

> Always keep the return type on its own line, separate from the
> function name. This is the GnollHack style convention.

---

## STATIC_OVL Function Definitions

Replace `STATIC_OVL` with the keyword `static`. Then convert the
K&R parameters as above.

```c
/* BEFORE */
STATIC_OVL int
use_cream_pie(obj)
struct obj *obj;
{

/* AFTER */
static int
use_cream_pie(struct obj *obj)
{
```

---

## STATIC_DCL Forward Declarations

`STATIC_DCL` wraps a static forward declaration using `FDECL` or
`NDECL`. Replace with plain C99 prototypes:

```c
/* BEFORE */
STATIC_DCL int FDECL(use_cream_pie, (struct obj *));
STATIC_DCL void NDECL(maybe_wail);
STATIC_DCL boolean FDECL(findtravelpath, (int));
STATIC_DCL int FDECL(still_chewing, (XCHAR_P, XCHAR_P));
STATIC_DCL boolean FDECL(its_dead, (int, int, int *));
STATIC_DCL void FDECL(add_class, (char *, CHAR_P));
STATIC_DCL boolean FDECL(is_valid_jump_pos, (int, int, int, BOOLEAN_P));

/* AFTER */
static int use_cream_pie(struct obj *);
static void maybe_wail(void);
static boolean findtravelpath(int);
static int still_chewing(xchar, xchar);
static boolean its_dead(int, int, int *);
static void add_class(char *, char);
static boolean is_valid_jump_pos(int, int, int, boolean);
```

Rules:
1. `STATIC_DCL` → `static`
2. `FDECL(name, (params))` → `name(params)` — unwrap the macro
3. `NDECL(name)` → `name(void)` — unwrap and add `void`
4. Replace all parameter type macros (see table below)

---

## Parameter Type Macro Replacement Table

These macros exist in
[tradstdc.h](file:///c:/hmp/GnollHack/include/tradstdc.h#L367) to
handle K&R argument widening rules. Replace them with the actual types:

| K&R Macro | C99 Type | Notes |
|-----------|----------|-------|
| `BOOLEAN_P` | `boolean` | Keep `boolean` typedef as-is |
| `CHAR_P` | `char` | |
| `SCHAR_P` | `schar` | |
| `UCHAR_P` | `uchar` | |
| `XCHAR_P` | `xchar` | |
| `SHORT_P` | `short` | |
| `UNSIGNED_SHORT_P` | `unsigned short` | Rare |
| `ALIGNTYP_P` | `aligntyp` | |
| `OBJ_P` | `struct obj *` | Only in function pointer decls |
| `MONST_P` | `struct monst *` | Only in function pointer decls |
| `TRAP_P` | `struct trap *` | Only in function pointer decls |

---

## `register` Keyword Removal

Remove `register` from all variable declarations — both K&R
parameters and local variables.

```c
/* BEFORE (K&R parameter + local) */
long *
alloc(lth)
register size_t lth;
{
    register genericptr_t ptr;

/* AFTER */
long *
alloc(size_t lth)
{
    void *ptr;
```

```c
/* BEFORE (local variables) */
    register struct obj *obj;
    register int i;
    register xchar x, y;

/* AFTER */
    struct obj *obj;
    int i;
    xchar x, y;
```

---

## STATIC_PTR, STATIC_VAR, VOID_ARGS, NEARDATA

**STATIC_PTR and STATIC_VAR** — replace with `static`:

```c
/* BEFORE */
STATIC_VAR int cached_value;
STATIC_PTR int FDECL(callback, (genericptr_t, struct obj *));

/* AFTER */
static int cached_value;
static int callback(void *, struct obj *);
```

**VOID_ARGS** — replace with `void`:

```c
/* BEFORE */
int
dodrop(VOID_ARGS)
{

/* AFTER */
int
dodrop(void)
{
```

**NEARDATA** — remove the macro, keep the declaration:

```c
/* BEFORE */
static NEARDATA struct obj *cached_obj;
STATIC_VAR NEARDATA const char drop_types[] = { ... };

/* AFTER */
static struct obj *cached_obj;
static const char drop_types[] = { ... };
```

---

## genericptr and genericptr_t

Replace both with `void *` everywhere they appear:

```c
/* BEFORE */
register genericptr_t ptr;
free((genericptr_t) oldest);
const genericptr vptr1;

/* AFTER */
void *ptr;
free((void *) oldest);
const void *vptr1;
```

The macros are defined in
[tradstdc.h](file:///c:/hmp/GnollHack/include/tradstdc.h#L235):
`#define genericptr void *` and `typedef genericptr genericptr_t;`.

---

## Variadic Functions (VA_DECL) — Summary

VA_DECL conversion is the most complex pattern. Full details and all
VA_DECL variants (VA_DECL through VA_DECL10) are documented in
[references/variadic_conversion.md](file:///c:/hmp/GnollHack/.agents/skills/c99_function_conversion/references/variadic_conversion.md).

Key rules:
1. `VA_DECL(type, name)` → `(type name, ...)`
2. `VA_DECL2(t1, n1, t2, n2)` → `(t1 n1, t2 n2, ...)`
3. Higher variants (`VA_DECL3`–`VA_DECL10`) follow the same pattern
4. `VA_START(x)` → `va_start(the_args, x)`
5. `VA_INIT(x, type)` → remove entirely (no-op under USE_STDARG)
6. `VA_ARGS` → `the_args` (the va_list variable name)
7. `VA_END()` → `va_end(the_args);` — do NOT add a closing brace
8. Remove all `#ifdef USE_STDARG` / `USE_VARARGS` / `USE_OLDARGS` branches
9. Remove `/*VARARGS1*/` and `/*VARARGS2*/` comments
10. Place return type on its own line, function name on next line

Quick example:
```c
/* BEFORE */
void panic
VA_DECL(const char *, str)
{
    VA_START(str);
    VA_INIT(str, char *);
    Vsprintf(buf, str, VA_ARGS);
    VA_END();
}

/* AFTER */
void
panic(const char *str, ...)
{
    va_list the_args;
    va_start(the_args, str);
    Vsprintf(buf, str, the_args);
    va_end(the_args);
}
```

---

## Per-File Conversion Checklist

Follow this order for each `.c` file:

1. Convert all `STATIC_DCL` forward declarations — remove
   `FDECL`/`NDECL` wrappers, replace type macros, replace
   `STATIC_DCL` with `static`
2. Convert all K&R function definitions to C99 parameter style
3. Replace `STATIC_OVL` with `static` in function definitions
4. Replace `STATIC_VAR` with `static` in variable declarations
5. Replace `STATIC_PTR` with `static` in function pointer declarations
6. Remove all `register` keywords (parameters AND locals)
7. Replace `genericptr_t` and `genericptr` with `void *`
8. Replace `NEARDATA` with nothing (delete the token)
9. Replace `VOID_ARGS` with `void`
10. Convert any `VA_DECL` variadic functions to standard C99 stdarg
11. Remove `/*VARARGS*/` and `/*VARARGS1*/` comments
12. Verify the file compiles: `msbuild` or IDE build

---

## Edge Cases and Warnings

> **Function pointers in K&R parameter declarations:** When a K&R
> function takes a function pointer argument, the `NDECL` wrapper
> appears inside the parameter declaration:
> ```c
> /* BEFORE — cmd.c:6972 */
> char
> cmd_from_func(fn)
> int NDECL((*fn));
> {
>
> /* AFTER */
> char
> cmd_from_func(int (*fn)(void))
> {
> ```

> **Multiple K&R params with same-line pointer declaration:**
> `const char *line, *attrs, *colors;` — each parameter needs its
> own fully qualified type:
> ```c
> /* BEFORE — pline.c:157 */
> void
> putmesg_ex2(line, attrs, colors)
> const char* line, *attrs, *colors;
> {
>
> /* AFTER */
> void
> putmesg_ex2(const char *line, const char *attrs,
>             const char *colors)
> {
> ```

> **K&R parameter order matters**: The C99 parameter order must match
> the order of the names in the parenthesized list, NOT the order of
> the K&R type declaration lines. For example:
> ```c
> /* K&R */
> void foo(a, b, c) int c; char b; float a; { }
> /* The name list says: a, b, c */
> /* C99 must be: void foo(float a, char b, int c) */
> ```

> **CFDECLSPEC in qsort callbacks:** Keep `CFDECLSPEC` (it maps to
> `__cdecl` on WinCE, empty elsewhere). Convert everything else:
> ```c
> /* BEFORE — end.c:3077 */
> STATIC_PTR int CFDECLSPEC
> vanqsort_cmp(vptr1, vptr2)
> const genericptr vptr1;
> const genericptr vptr2;
> {
>
> /* AFTER */
> static int CFDECLSPEC
> vanqsort_cmp(const void *vptr1, const void *vptr2)
> {
> ```

> **Custom typedefs:** Treat `Loot`, `coord`, `anything`, `aligntyp`,
> and other project-specific typedefs like any other type — do NOT
> expand them. Only expand the `_P` macros and `genericptr`/
> `genericptr_t`.

> **Preserve return type on separate line:** GnollHack style keeps
> the return type on its own line. Do NOT merge it with the function
> name.

> **Do NOT change function bodies:** Only change the function
> signature and parameter declarations. Leave function bodies
> untouched.

> **Do NOT change extern.h FDECL/NDECL declarations** in the same
> pass. Those are handled separately by the `c99_header_modernization`
> skill.

> **Comments in K&R parameter blocks:** Some functions have inline
> comments after parameters. Preserve them:
> ```c
> /* BEFORE — allmain.c:35 */
> void
> moveloop(resuming)
> uchar resuming; /* 0 = new game, 1 = loaded */
> {
>
> /* AFTER */
> void
> moveloop(uchar resuming) /* 0 = new game, 1 = loaded */
> {
> ```

> **78-character line limit:** If the C99 parameter list exceeds 78
> chars, wrap after a comma and align with the opening parenthesis.

> **NORETURN attribute:** Preserve the `NORETURN` attribute on both
> forward declarations and definitions:
> ```c
> /* BEFORE */
> STATIC_DCL void FDECL(really_done, (int)) NORETURN;
> /* AFTER */
> static void really_done(int) NORETURN;
> ```

---

## Key Files Reference

| File | Relevance |
|------|-----------|
| [tradstdc.h](file:///c:/hmp/GnollHack/include/tradstdc.h) | Defines FDECL, NDECL, VDECL, VA_DECL*, BOOLEAN_P, etc. |
| [hack.h](file:///c:/hmp/GnollHack/include/hack.h#L710) | Defines STATIC_DCL, STATIC_OVL, STATIC_VAR, STATIC_PTR |
| [extern.h](file:///c:/hmp/GnollHack/include/extern.h) | All external function prototypes (~4,640 lines) |
| [global.h](file:///c:/hmp/GnollHack/include/global.h) | boolean typedef, NEARDATA, type definitions |
| [config.h](file:///c:/hmp/GnollHack/include/config.h) | schar, uchar typedefs |
| [pline.c](file:///c:/hmp/GnollHack/src/pline.c) | Most complex VA_DECL usage (VA_DECL through VA_DECL10) |
| [end.c](file:///c:/hmp/GnollHack/src/end.c) | panic() VA_DECL, CFDECLSPEC callback examples |
| [allmain.c](file:///c:/hmp/GnollHack/src/allmain.c) | Typical file with STATIC_DCL + K&R functions |
| [cmd.c](file:///c:/hmp/GnollHack/src/cmd.c) | Function pointer K&R params, NDECL((*fn)) pattern |
| [alloc.c](file:///c:/hmp/GnollHack/src/alloc.c) | register + genericptr_t example |
