# Pattern Catalog: K&R to C99 Edge Cases

Additional conversion examples supplementing the main
[SKILL.md](file:///c:/hmp/GnollHack/.agents/skills/c99_function_conversion/SKILL.md).
These cover uncommon patterns that require extra care.

For variadic function conversion (VA_DECL patterns), see
[variadic_conversion.md](file:///c:/hmp/GnollHack/.agents/skills/c99_function_conversion/references/variadic_conversion.md).

---

## K&R Parameter Order vs. Type Declaration Order

> **Critical rule**: The C99 parameter order must match the NAME LIST
> in the parentheses, NOT the order of the K&R type declaration lines.

In K&R C, the type declarations after the parameter list can appear
in any order. The C99 version must restore the order from the name
list.

```c
/* BEFORE — cmd.c:301 */
void
set_occupation(fn, txt, attr, color, soundset_id,
               occ_type, sound_type, xtime)
int NDECL((*fn));       /* fn */
const char *txt;        /* txt */
enum object_soundset_types soundset_id;  /* soundset_id (NOT 3rd!) */
enum object_occupation_types occ_type;   /* occ_type */
enum occupation_sound_types sound_type;  /* sound_type */
int xtime, attr, color; /* xtime, attr, color (3 params, 1 line!) */
{

/* AFTER — parameters match the NAME LIST order */
void
set_occupation(int (*fn)(void), const char *txt,
               int attr, int color,
               enum object_soundset_types soundset_id,
               enum object_occupation_types occ_type,
               enum occupation_sound_types sound_type,
               int xtime)
{
```

Note three things:
1. `attr` and `color` appear as the 3rd and 4th names but are declared
   in the last K&R line `int xtime, attr, color;`
2. The K&R line `int xtime, attr, color;` declares 3 params on one
   line — each must get its own `int` type in C99
3. `int NDECL((*fn))` becomes `int (*fn)(void)`

---

## Complex Function Pointer Parameters

**NDECL in K&R parameter — cmd.c:6972:**
```c
/* BEFORE */
char
cmd_from_func(fn)
int NDECL((*fn));
{

/* AFTER */
char
cmd_from_func(int (*fn)(void))
{
```

**NDECL function pointer in STATIC_DCL:**
```c
/* BEFORE */
STATIC_DCL int NDECL((*timed_occ_fn));
STATIC_DCL boolean FDECL(accept_menu_prefix,
                          (int NDECL((*))));
/* AFTER */
static int (*timed_occ_fn)(void);
static boolean accept_menu_prefix(int (*)(void));
```

**Function pointer variable declarations:**
```c
/* BEFORE */
int NDECL((*fn));
void NDECL((*decgraphics_mode_callback)) = 0;
afternmv = (int NDECL((*))) 0;

/* AFTER */
int (*fn)(void);
void (*decgraphics_mode_callback)(void) = 0;
afternmv = (int (*)(void)) 0;
```

**FDECL in struct member function pointers (winprocs.h:25):**
```c
/* BEFORE */
    void FDECL((*win_init_nhwindows), (int *, char **));
    void FDECL((*win_display_nhwindow), (winid, BOOLEAN_P));

/* AFTER */
    void (*win_init_nhwindows)(int *, char **);
    void (*win_display_nhwindow)(winid, boolean);
```

---

## CFDECLSPEC in qsort Callbacks

`CFDECLSPEC` maps to `__cdecl` on WinCE, empty elsewhere. Keep it.
These are always qsort comparison functions.

**Forward declaration:**
```c
/* BEFORE */
STATIC_PTR int FDECL(CFDECLSPEC vanqsort_cmp,
                     (const genericptr, const genericptr));

/* AFTER */
static int CFDECLSPEC vanqsort_cmp(const void *,
                                   const void *);
```

**Definition:**
```c
/* BEFORE — end.c:3077 */
STATIC_PTR int CFDECLSPEC
vanqsort_cmp(vptr1, vptr2)
const genericptr vptr1;
const genericptr vptr2;
{

/* AFTER */
static int CFDECLSPEC
vanqsort_cmp(const void *vptr1, const void *vptr2)
{
```

> Note: `STATIC_PTR` (not `STATIC_OVL`) is used for callbacks passed
> as function pointers to `qsort()`.

---

## Functions with Array Parameters

If a K&R function uses array syntax in parameters, preserve the array
notation — do NOT change to pointer:

```c
/* Illustrative pattern — array syntax is rare in GnollHack */
/* BEFORE */
void
process_data(buf, len)
char buf[];
int len;
{

/* AFTER */
void
process_data(char buf[], int len)
{
```

---

## NORETURN Attribute

Preserve `NORETURN` on both forward declarations and definitions:

```c
/* BEFORE */
STATIC_DCL void FDECL(really_done, (int)) NORETURN;

/* AFTER */
static void really_done(int) NORETURN;
```

```c
/* BEFORE */
void
terminate(status)
int status;
{    /* NORETURN function */

/* AFTER */
void
terminate(int status)
{    /* NORETURN function */
```

---

## PRINTF_F Attribute

Preserve `PRINTF_F` on variadic function declarations:

```c
/* BEFORE */
E void VDECL(impossible, (const char *, ...)) PRINTF_F(1, 2);

/* AFTER */
E void impossible(const char *, ...) PRINTF_F(1, 2);
```

---

## Multi-Variable K&R Pointer Declarations

When a K&R type line declares multiple pointer parameters, each
needs its own fully qualified type in C99:

```c
/* BEFORE — pline.c:157 */
STATIC_OVL void
putmesg_ex2(line, attrs, colors)
const char* line, *attrs, *colors;
{

/* AFTER */
static void
putmesg_ex2(const char *line, const char *attrs,
            const char *colors)
{
```

Note: `const char* line, *attrs, *colors;` declares three
`const char *` parameters on one line. In C99, each needs
`const char *`.

---

## Struct Pointer Multi-Declarations

Similar to the above but with struct pointers:

```c
/* BEFORE */
void
foo(mon1, mon2)
struct monst *mon1, *mon2;
{

/* AFTER */
void
foo(struct monst *mon1, struct monst *mon2)
{
```

---

## Parameters with Const Qualifiers

`const` qualifiers pass through unchanged into C99 parameters:

```c
/* Illustrative pattern */
/* BEFORE */
STATIC_OVL int
example_func(mtmp, name, flags)
struct monst *mtmp;
const char *name;
unsigned long flags;
{

/* AFTER */
static int
example_func(struct monst *mtmp, const char *name,
             unsigned long flags)
{
```

---

## Functions with Very Many Parameters (7+)

Use line wrapping aligned to the opening parenthesis. The
`set_occupation` example from cmd.c:301 (shown above) is a
real-world example with 8 parameters.

General rules for wrapping:
- Break after a comma
- Align continuation lines with the opening parenthesis
- Keep within the 78-character line limit
- When K&R declares `char ch, gch;` it becomes `char ch, char gch`
- When K&R declares `int attr, color;` it becomes `int attr, int color`
