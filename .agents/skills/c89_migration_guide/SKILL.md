---
name: c89_migration_guide
description: Master reference for migrating the GnollHack C codebase from K&R C to C89, covering conversion phases, ordering dependencies, rules for what changes and what stays, per-file workflow, verification steps, and coordination between header and source file modernization.
---

# C89 Migration Guide

## Critical Rules
- **Do NOT convert system headers** (e.g., `amiconf.h`, `ntconf.h`). Focus only on core game logic (`src/` and core `include/`).
- **Convert ONE file at a time**. Ensure the file compiles before moving to the next.
- **Do NOT combine C89 conversion with feature changes or bug fixes**. This is a strict refactoring task.
- **Do NOT modify function logic**. Only change function signatures and variable declarations.

## Migration Phases
1. **Source File Conversion (`src/*.c`)**:
   - Convert K&R function definitions to C89 prototypes.
   - Expand grouped parameters (e.g., `int x, y` -> `int x, int y`).
   - Remove `STATIC_DCL`, `STATIC_OVL`, `STATIC_PTR` wrappers (replace with `static`).
   - Convert `VA_DECL` variadic functions (see `c89_function_conversion/references/variadic_conversion.md`).
   - Remove `register` keyword.
2. **Header Modernization (`include/*.h`)**:
   - Done AFTER source files are converted.
   - Remove `FDECL`, `NDECL`, `VDECL` from `extern.h`.
   - Update parameter type macros (`BOOLEAN_P` to `boolean`, etc.).
   - Comment out compatibility macros in `tradstdc.h` as the final step.

## Per-File Workflow
1. Select a file in `src/`.
2. Find all function definitions and convert K&R style to C89 ANSI style.
3. Replace `STATIC_...` macros with `static`.
4. Run the build to verify no syntax errors.
5. Commit the file.

## What Stays (Do NOT change)
- `const` qualifiers.
- `#ifdef` blocks (keep both branches if both use K&R, but typically GnollHack only builds one branch; convert all visible code).
- Variable naming.
- `NORETURN` and `PRINTF_F` attributes.
