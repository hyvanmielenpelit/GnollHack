# Variadic Function Conversion (VA_DECL)

Detailed guide for converting GnollHack's variadic functions from
VA_DECL macros to standard C99 `<stdarg.h>`. This is the most complex
pattern in the K&R to C99 migration.

The VA_DECL macros are defined in
[tradstdc.h](file:///c:/hmp/GnollHack/include/tradstdc.h#L89).

---

## How VA_DECL Works Internally

The `VA_DECL` macro hides:
- The parameter list `(type name, ...)`
- An opening brace `{`
- A `va_list` declaration

`VA_START` calls `va_start`, `VA_INIT` is a no-op under USE_STDARG,
`VA_ARGS` is the `va_list` variable name, and `VA_END` calls
`va_end` and includes a hidden closing brace `}`.

The three VA_DECL macro branches (USE_STDARG, USE_VARARGS,
USE_OLDARGS) exist only because the original NetHack supported
pre-ANSI compilers. GnollHack always uses USE_STDARG. After
conversion, all branches are removed and only direct C99 stdarg
calls remain.

---

## Conversion Rules

| Old Macro | C99 Replacement | Notes |
|-----------|-----------------|-------|
| `VA_DECL(T, name)` | `(T name, ...)` | 1 fixed parameter |
| `VA_DECL2(T1, n1, T2, n2)` | `(T1 n1, T2 n2, ...)` | 2 fixed parameters |
| `VA_DECL3` through `VA_DECL10` | Same pattern with N params | See examples below |
| `VA_START(x)` | `va_start(the_args, x)` | Use last fixed param |
| `VA_INIT(x, type)` | *(remove entirely)* | No-op in USE_STDARG |
| `VA_ARGS` | `the_args` | The `va_list` variable |
| `VA_END()` | `va_end(the_args);` | Do NOT add `}` — VA_END contained one |
| `/*VARARGS1*/` | *(remove)* | Lint hint, no longer needed |

Additionally:
- Add `va_list the_args;` as a local variable declaration
- Remove all `#if defined(USE_STDARG)` / `USE_VARARGS` / `USE_OLDARGS`
  preprocessor branches — keep only the ANSI version
- Place return type on its own line, function name on next line

---

## Simple VA_DECL (1 fixed arg)

Used by `pline`, `panic`, `You`, `pline_The`, `verbalize`, etc.

```c
/* BEFORE — end.c:732 */
/*VARARGS1*/
void panic
VA_DECL(const char *, str)
{
    VA_START(str);
    VA_INIT(str, char *);
    /* ... */
    Vsprintf(buf, str, VA_ARGS);
    VA_END();
    really_done(PANICKED);
}

/* AFTER */
void
panic(const char *str, ...)
{
    va_list the_args;
    va_start(the_args, str);
    /* ... */
    Vsprintf(buf, str, the_args);
    va_end(the_args);
    really_done(PANICKED);
}
```

---

## VA_DECL2 (2 fixed args)

Used by `custompline`.

```c
/* BEFORE */
/*VARARGS2*/
void custompline
VA_DECL2(unsigned, pflags, const char *, line)
{
    VA_START(line);
    VA_INIT(line, const char *);
    pline_flags = pflags;
    vpline(line, VA_ARGS);
    pline_flags = 0;
    VA_END();
    return;
}

/* AFTER */
void
custompline(unsigned pflags, const char *line, ...)
{
    va_list the_args;
    va_start(the_args, line);
    pline_flags = pflags;
    vpline(line, the_args);
    pline_flags = 0;
    va_end(the_args);
    return;
}
```

---

## VA_DECL3 (3 fixed args)

Used by `pline_ex`, `You_ex`, `Your_ex`, etc.

```c
/* BEFORE */
void pline_ex
VA_DECL3(int, attr, int, color, const char*, line)

/* AFTER */
void
pline_ex(int attr, int color, const char *line, ...)
```

---

## VA_DECL4 (4 fixed args)

Used by `custompline_ex`.

```c
/* BEFORE */
void custompline_ex
VA_DECL4(int, attr, int, color, unsigned, pflags,
         const char*, line)

/* AFTER */
void
custompline_ex(int attr, int color, unsigned pflags,
               const char *line, ...)
```

---

## VA_DECL5 (5 fixed args)

Used by `pline_multi_ex`.

```c
/* BEFORE */
void pline_multi_ex
VA_DECL5(int, attr, int, color, const int*, multiattrs,
         const int*, multicolors, const char*, line)

/* AFTER */
void
pline_multi_ex(int attr, int color, const int *multiattrs,
               const int *multicolors, const char *line, ...)
```

---

## VA_DECL6 (6 fixed args)

Used by `pline_multi_ex_flags`.

```c
/* BEFORE */
void pline_multi_ex_flags
VA_DECL6(int, attr, int, color, const int*, multiattrs,
         const int*, multicolors, unsigned, pflags,
         const char*, line)

/* AFTER */
void
pline_multi_ex_flags(int attr, int color,
                     const int *multiattrs,
                     const int *multicolors,
                     unsigned pflags,
                     const char *line, ...)
```

---

## VA_DECL7 (7 fixed args)

Used by `pline_multi_ex_popup`.

```c
/* BEFORE */
void pline_multi_ex_popup
VA_DECL7(int, attr, int, color, const int*, multiattrs,
         const int*, multicolors, const char*, title,
         int, dopopup, const char*, line)

/* AFTER */
void
pline_multi_ex_popup(int attr, int color,
                     const int *multiattrs,
                     const int *multicolors,
                     const char *title, int dopopup,
                     const char *line, ...)
```

---

## VA_DECL10 (10 fixed args)

Used by `custompline_ex_prefix`.

```c
/* BEFORE */
void custompline_ex_prefix
VA_DECL10(int, prefix_attr, int, prefix_color,
          const char*, prefix_line,
          int, separator_attr, int, separator_color,
          const char*, separator_line,
          int, attr, int, color,
          unsigned, pflags, const char*, line)

/* AFTER */
void
custompline_ex_prefix(int prefix_attr, int prefix_color,
                      const char *prefix_line,
                      int separator_attr,
                      int separator_color,
                      const char *separator_line,
                      int attr, int color,
                      unsigned pflags,
                      const char *line, ...)
```

> Note: `VA_DECL8` and `VA_DECL9` are not defined in tradstdc.h and
> are not used anywhere in the codebase.

---

## The pline/vpline Dual-Definition Pattern

This is the most complex pattern in the entire codebase. Found in
[pline.c](file:///c:/hmp/GnollHack/src/pline.c#L181), the `pline()`
function has **three interleaved implementations** under preprocessor
guards. Many other functions in pline.c follow the same structure
(e.g., `pline_ex`, `pline_multi_ex`, etc.).

### How It Works (Before)

```c
#if defined(USE_STDARG) || defined(USE_VARARGS)
/* Branch 1+2: declare vpline, have pline() as a thin wrapper */
STATIC_DCL void FDECL(vpline, (const char *, va_list));

/*VARARGS1*/
void
pline
VA_DECL(const char *, line)
{
    VA_START(line);
    VA_INIT(line, char *);
    vpline(line, VA_ARGS);
    VA_END();
}

# ifdef USE_STDARG
/* Branch 1: ANSI C prototype for vpline */
STATIC_OVL void
vpline(const char *line, va_list the_args)
# else
/* Branch 2: K&R prototype for vpline */
STATIC_OVL void
vpline(line, the_args)
const char *line;
va_list the_args;
# endif

#else /* USE_OLDARGS */
/* Branch 3: No vpline at all; pline IS the implementation */
# define vpline pline

/*VARARGS1*/
void
pline
VA_DECL(const char *, line)
#endif
{       /* start of vpline() or nested block */
    /* ... function body (~300 lines) ... */

#if !(defined(USE_STDARG) || defined(USE_VARARGS))
    VA_END();
#endif
}
```

### How It Should Look (After)

```c
static void vpline(const char *line, va_list the_args);

void
pline(const char *line, ...)
{
    va_list the_args;
    va_start(the_args, line);
    vpline(line, the_args);
    va_end(the_args);
}

static void
vpline(const char *line, va_list the_args)
{
    /* ... function body (unchanged) ... */
}
```

### Key Insights

1. The function body belongs to `vpline()` after conversion
2. The `pline()` wrapper simply sets up the `va_list` and forwards
3. Remove ALL three `#if` branches — only the clean ANSI version remains
4. The `#define vpline pline` in the USE_OLDARGS branch disappears
5. The trailing `VA_END()` inside `#if !USE_STDARG` disappears
6. Every pline variant (pline_ex, pline_multi_ex, etc.) follows
   this same structure and gets the same treatment

---

## Forward Declarations of Variadic Functions

In extern.h or at file scope, variadic function declarations use
`VDECL`:

```c
/* BEFORE (extern.h) */
E void VDECL(pline, (const char *, ...)) PRINTF_F(1, 2);
E void VDECL(panic, (const char *, ...)) PRINTF_F(1, 2);

/* AFTER (extern.h) */
E void pline(const char *, ...) PRINTF_F(1, 2);
E void panic(const char *, ...) PRINTF_F(1, 2);
```

> Note: extern.h conversion is handled by the
> `c99_header_modernization` skill. This reference file focuses on
> the `.c` file implementations.
