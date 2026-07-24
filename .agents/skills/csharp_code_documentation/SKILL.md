---
name: csharp_code_documentation
description: Best practices for AI agents documenting the GnollHack C# .NET MAUI codebase. Covers XML doc comments, inline comments, P/Invoke documentation, and Microsoft-standard documentation patterns for the game's frontend, native bridge, and UI code.
---

# C# Code Documentation

## Critical Rules
- **Use standard XML doc comments (`///`)** for all public classes, methods, and properties.
- **Document all `DllImport` P/Invoke methods**. Explicitly state which C function is being called and describe any marshaling behavior.
- **Do NOT document trivial getters/setters** unless they have side effects.

This skill defines how AI agents should apply documentation and comments to GnollHack's C# source code. The documentation style follows Microsoft's own C# and .NET documentation conventions — concise, technical, and structured using standard XML documentation comments.

For supplementary patterns (regions, enums, XAML code-behind, exceptions, constants), see [references/supplementary_patterns.md](file:///c:/hmp/GnollHack/.agents/skills/csharp_code_documentation/references/supplementary_patterns.md).

## Overview

GnollHack's C# codebase is a .NET 10.0 MAUI application that serves as the graphical frontend for the C game engine. It includes:

- **UI pages and controls** (XAML + code-behind)
- **SkiaSharp game rendering** (tile-based map, animations, effects)
- **P/Invoke native bridge** (C# ↔ C interop via delegates and callbacks)
- **Audio integration** (FMOD sound engine)
- **Platform-specific code** (Android, iOS, Windows)

The documentation must help developers understand the architecture, threading model, and native interop boundaries without being overly verbose.

### Current State of Documentation

The existing C# codebase has **very sparse documentation** — XML doc comments are nearly nonexistent (only 5 instances in the entire project), and inline comments are sporadic. The codebase also uses C-style `/* */` inline comments (a NetHack heritage pattern). When adding new documentation:

- Use **`///` XML doc comments** for all public/protected API documentation (Microsoft standard)
- Use **`//` line comments** for new inline documentation (standard C#)
- The existing `/* */` inline comments in the codebase should be **preserved** but new inline comments should use `//`

---

## General Principles

### 1. Follow Microsoft Documentation Conventions

GnollHack C# documentation should look and feel like Microsoft's own .NET documentation:

- Use **XML doc comments** (`///`) for all public and protected members
- Keep descriptions **concise and action-oriented** (start with a verb)
- Use `<see cref="..."/>` for cross-references, not plain text names
- Use `<paramref name="..."/>` when referring to parameters in descriptions
- Document **what**, not **how** — the code shows the implementation

### 2. Be Concise and Technical

Unlike the C documentation skill (which encourages verbosity for legacy code), C# documentation should be **lean and precise**. Developers working in C# are expected to understand modern OOP patterns, async/await, MAUI concepts, and standard .NET idioms without explanation.

```csharp
// BAD: Overly verbose
/// <summary>
/// This method is responsible for handling the event that occurs when
/// the user taps on the screen. It takes the tap coordinates and
/// determines which game tile was tapped, then sends the appropriate
/// command to the game engine through the native bridge.
/// </summary>

// GOOD: Concise and technical
/// <summary>
/// Handles a screen tap by translating touch coordinates to game tile
/// coordinates and dispatching the corresponding command to the native engine.
/// </summary>
```

### 3. Document the "Why" for Non-Obvious Code

Standard C# patterns don't need explanation. Document the parts that are specific to GnollHack's architecture:

- **Threading constraints** — why something must run on a specific thread
- **Native interop details** — why a specific marshalling approach is used
- **Game-engine coupling** — why UI code responds to specific callback patterns
- **Platform differences** — why code branches by platform

---

## XML Documentation Comments

### Classes and Structs

Every public and internal class should have a `<summary>` tag. Include `<remarks>` only when the class has non-obvious architectural significance.

## XML Documentation Format
```csharp
/// <summary>
/// Brief description of the member.
/// </summary>
/// <param name="paramName">Description of the parameter.</param>
/// <returns>Description of the return value.</returns>
/// <remarks>
/// Additional context, edge cases, or threading considerations.
/// </remarks>
```

## P/Invoke Documentation
Document the C engine relationship:
```csharp
/// <summary>
/// Displays a window to the screen. Maps to win_display_nhwindow in C.
/// </summary>
/// <param name="window">The window ID to display.</param>
/// <param name="blocking">If true, blocks execution until user dismisses.</param>
[DllImport(LibraryName, EntryPoint = "gh_display_nhwindow")]
public static extern void DisplayWindow(int window, [MarshalAs(UnmanagedType.I4)] int blocking);
```

## Threading Documentation
Use `<remarks>` to document thread safety requirements.
- Specify if a method MUST run on the `MainThread`.
- Specify if a method MUST run on the `GameThread`.
- Detail any `lock()` or `Monitor.TryEnter` requirements.

---

## Naming Conventions for Interop

GnollHack uses deliberate naming divergences from standard C# conventions for code that crosses the C/C# interop boundary. This ensures identifiers can be found with a single search across both codebases.

### Interop Struct Fields: `snake_case`
Interop structs (marked with `[StructLayout(LayoutKind.Sequential)]`) use `snake_case` field names to match the C side:

```csharp
[StructLayout(LayoutKind.Sequential)]
public struct AnimationDefinition
{
    public IntPtr animation_name;
    public int animation_type;
    public sbyte number_of_tiles;
    public sbyte number_of_frames;
    // ...
}
```

### Interop Enums and Defines: `ALL_CAPS_WITH_UNDERSCORES`
Enums and constants that mirror C `#define` macros or enum values use `ALL_CAPS_WITH_UNDERSCORES` — both the member names and (in many cases) the enum type name itself:

```csharp
// Enum type name is snake_case to match C
public enum layer_types
{
    LAYER_FLOOR = 0,
    LAYER_CARPET,
    LAYER_FLOOR_DOODAD,
    // ...
    MAX_LAYERS
}

public enum MenuFlags : ulong
{
    MENU_FLAGS_NONE =                       0x00000000,
    MENU_FLAGS_IS_HEADING =                 0x00000001,
    MENU_FLAGS_IS_GROUP_HEADING =           0x00000002,
    // ...
}
```

### Pure C# Code: Standard Conventions
All other C# code (classes, properties, methods, local variables) follows standard .NET conventions: PascalCase for public members, camelCase for locals and private fields.

