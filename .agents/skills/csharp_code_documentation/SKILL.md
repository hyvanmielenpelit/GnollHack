---
name: csharp_code_documentation
description: Best practices for AI agents documenting the GnollHack C# .NET MAUI codebase. Covers XML doc comments, inline comments, P/Invoke documentation, and Microsoft-standard documentation patterns for the game's frontend, native bridge, and UI code.
---

# C# Code Documentation

## Critical Rules
- **Use standard XML doc comments (`///`)** for all public classes, methods, and properties.
- **Document all `DllImport` P/Invoke methods**. Explicitly state which C function is being called and describe any marshaling behavior.
- **Do NOT document trivial getters/setters** unless they have side effects.

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
