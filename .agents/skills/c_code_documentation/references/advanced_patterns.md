# Advanced C Documentation Patterns

## 1. Macros and Constants
- Use block comments (`/* ... */`) to explain groups of `#define` macros.
- Explain the *game design intent* behind data macros (e.g., why a monster has specific stats), not the macro syntax itself.

## 2. Structs and Enums
- Document structs in headers with a block comment explaining the struct's role and key fields.
- Document enums with their purpose and the meaning of individual values.

## 3. Complex Control Flow
- Use structural inline comments to divide long functions into logical phases.
- Document cross-system interactions (e.g., triggering display + sound + animation).

## 4. Conditional Compilation
- Document non-obvious `#ifdef` blocks (e.g., `#ifdef INSURANCE`, `#ifdef STEED`).
- Brief inline comments are sufficient for platform toggles (`#ifdef WIN32`).
