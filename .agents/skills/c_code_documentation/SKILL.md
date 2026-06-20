---
name: c_code_documentation
description: Best practices for AI agents documenting the GnollHack C codebase. Covers file headers, function documentation, inline comments, macro explanations, and strategies for making legacy NetHack-derived C code understandable to modern developers.
---

# C Code Documentation

## Critical Rules
- **Never delete existing documentation**. Append to or clarify existing comments; do not replace them.
- **Use `/* */` for all C comments**. Do NOT use `//` in the C core.
- **Document game mechanics**, not syntax. Explain *why* the code does what it does in the context of the game.

## Documentation Standards

### File Headers
Add a top-level block comment explaining the file's purpose within the game engine.
```c
/*
 * file.c — Description of the subsystem.
 * Explains how this file connects to other parts of the engine.
 */
```

### Function Documentation
Place a block comment above the function definition (not the prototype).
```c
/*
 * function_name()
 *
 * Describe what the function does, what the parameters mean,
 * and what it returns. Mention any side effects (e.g., "might
 * destroy the object" or "can kill the player").
 */
```

### Inline Comments
Use inline comments for complex bitwise operations, obscure macros, or deeply nested logic.
```c
if (u.umonnum == PM_LICH) {
    /* Liches are immune to cold damage */
    ...
}
```

### Macros
Document complex macros with a block comment explaining the pattern and usage.

## Common Idioms to Document
- **`rn2(n)`**: Returns a random integer `0` to `n-1`.
- **`d(n, s)`**: Rolls `n` dice with `s` sides.
- **`Obj_gone_p(otmp)`**: Checks if an object has been freed.
- Iterating over `fmon` (monster list): Always document that `nmon` must be saved before processing in case the monster dies.
