# Supplementary C# Documentation Patterns

## 1. Region Markers
- Use `#region` blocks in files with 200+ lines.
- Never nest regions.
- Group related methods logically.

## 2. Enums
- Document the enum and non-obvious members.
- For `[Flags]`, document combination semantics.

## 3. XAML Code-Behind
- Document the page/control purpose at the class level.
- **Document event handlers ONLY if they contain**:
  - Threading logic (marshaling).
  - Complex coordinate mapping.
  - Native engine commands.
- Do NOT document simple navigation or UI toggle handlers.

## 4. Exceptions
- Document exceptions explicitly thrown using `<exception>` tags.
- Document `InvalidOperationException` for state preconditions.

## 5. Async Methods
- Document what the method awaits and if it runs on a background thread.
