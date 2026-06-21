# Pattern Catalog: K&R to C89 Edge Cases

## 1. Parameter Order vs. Type Declaration Order
- **Rule**: C89 parameter order MUST match the NAME LIST in parentheses, NOT the type declaration lines.
- **Example**: `set_occupation(fn, txt, attr, color)` with `int attr, color;` must become `set_occupation(int (*fn)(void), const char *txt, int attr, int color)`.

## 2. Function Pointer Parameters
- `int NDECL((*fn))` → `int (*fn)(void)`
- `STATIC_DCL int NDECL((*timed_occ_fn));` → `static int (*timed_occ_fn)(void);`

## 3. CFDECLSPEC (qsort Callbacks)
- **Rule**: Keep `CFDECLSPEC`. Keep `STATIC_PTR`. Use `const void *`.
- **Example**: `STATIC_PTR int CFDECLSPEC vanqsort_cmp(const void *vptr1, const void *vptr2)`

## 4. Attributes (NORETURN, PRINTF_F)
- **Rule**: Preserve these attributes.
- **Example**: `static void really_done(int) NORETURN;`

## 5. Multi-Variable K&R Pointers
- **Rule**: Each parameter gets its own type.
- **Example**: `const char* line, *attrs, *colors;` → `(const char *line, const char *attrs, const char *colors)`

## 6. Array Parameters
- **Rule**: Keep array syntax.
- **Example**: `char buf[]` → `char buf[]`
