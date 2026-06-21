# Variadic Function Conversion (VA_DECL)

## Conversion Rules
- **`VA_DECL(T, name)`** → `(T name, ...)`
- **`VA_DECL2(T1, n1, T2, n2)`** → `(T1 n1, T2 n2, ...)`
- **`VA_START(x)`** → `va_start(the_args, x)`
- **`VA_INIT(x, type)`** → *(remove entirely)*
- **`VA_ARGS`** → `the_args`
- **`VA_END()`** → `va_end(the_args);` (Do NOT add closing brace).
- **`/*VARARGS1*/`** → *(remove)*

## Required Additions
- Add `va_list the_args;` as a local variable.

## Branches to Remove
- Remove `#if defined(USE_STDARG)`, `USE_VARARGS`, `USE_OLDARGS`. Keep only the ANSI version.

## The Dual-Definition Pattern (e.g., `pline`)
- Complex functions often have a `v` version (e.g., `vpline`).
- After conversion, the `pline` wrapper just calls `vpline` with `the_args`.
- Remove all fallback `#define vpline pline` logic.
