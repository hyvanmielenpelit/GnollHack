---
name: csharp_code_documentation
description: Best practices for AI agents documenting the GnollHack C# .NET MAUI codebase. Covers XML doc comments, inline comments, P/Invoke documentation, and Microsoft-standard documentation patterns for the game's frontend, native bridge, and UI code.
---

# C# Code Documentation Guide for GnollHack

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

```csharp
/// <summary>
/// Manages the SkiaSharp canvas rendering for the game map, including
/// tile drawing, animation playback, and layer compositing.
/// </summary>
/// <remarks>
/// Rendering occurs on a dedicated background thread on Android.
/// All map data access is synchronized through double-buffering
/// to avoid blocking the game engine's main loop.
/// </remarks>
public class GameMapRenderer
{
    ...
}
```

For classes implementing interfaces or inheriting from a base, note the relationship:

```csharp
/// <summary>
/// Game page hosting the SkiaSharp canvas and handling player input.
/// Implements <see cref="IThreadSafeView"/> for cross-thread UI
/// updates from the native game engine.
/// </summary>
public partial class GamePage : ContentPage, IThreadSafeView
{
    ...
}
```

### Methods

Public and protected methods require `<summary>`, `<param>`, and `<returns>` tags. Private methods should have XML doc comments if their purpose is not immediately obvious from the name and context.

```csharp
/// <summary>
/// Converts screen pixel coordinates to game map tile coordinates,
/// accounting for scroll offset and zoom level.
/// </summary>
/// <param name="screenX">
/// Horizontal pixel position relative to the canvas origin.
/// </param>
/// <param name="screenY">
/// Vertical pixel position relative to the canvas origin.
/// </param>
/// <returns>
/// The map tile coordinates, or <c>null</c> if the screen
/// position is outside the visible map area.
/// </returns>
public GHPoint? ScreenToTile(float screenX, float screenY)
{
    ...
}
```

For `void` methods, omit `<returns>`:

```csharp
/// <summary>
/// Refreshes the entire game map display, forcing a full redraw
/// of all visible tiles on the next rendering frame.
/// </summary>
public void InvalidateMap()
{
    ...
}
```

### Properties

Document properties with a `<summary>` that describes what the property represents, not the get/set mechanics:

```csharp
/// <summary>
/// The player character's current hit points, as reported by
/// the native engine.
/// </summary>
public int HitPoints { get; private set; }

/// <summary>
/// Whether the game engine is currently waiting for player input.
/// When <c>true</c>, UI input controls are active.
/// </summary>
public bool AwaitingInput { get; private set; }
```

### Events and Delegates

```csharp
/// <summary>
/// Raised when the native engine reports a change to the player's
/// status (HP, power, hunger, conditions, etc.).
/// </summary>
public event EventHandler<StatusUpdateEventArgs>? StatusUpdated;
```

### Constructors

Document constructors that perform significant initialization:

```csharp
/// <summary>
/// Initializes a new game session with the specified configuration
/// and registers native engine callbacks.
/// </summary>
/// <param name="config">
/// Game configuration including tileset and sound preferences.
/// </param>
/// <exception cref="InvalidOperationException">
/// Thrown if a game session is already active.
/// </exception>
public GameSession(GameConfig config)
{
    ...
}
```

---

## P/Invoke and Native Bridge Documentation

The P/Invoke boundary is one of the most critical areas to document because it connects two very different codebases. Key files:

- [GnollHackService.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX.Common/GnollHackService.cs) — ~360 lines of `[DllImport]` declarations with zero documentation currently
- [GHDelegates.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX/GHDelegates.cs) — callback delegate definitions
- [IGnollHackService.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX/IGnollHackService.cs) — 90 interface methods, currently undocumented

### Delegate Declarations

Document each callback delegate with its purpose and threading context:

```csharp
/// <summary>
/// Callback invoked by the native engine to display a glyph at
/// a map position.
/// </summary>
/// <param name="winid">Window identifier for the map window.</param>
/// <param name="x">Horizontal map coordinate (0-based).</param>
/// <param name="y">Vertical map coordinate (0-based).</param>
/// <param name="glyphInfo">
/// Pointer to a <c>LayerInfo</c> struct containing the full tile
/// layer data. Must be marshalled before the pointer goes out
/// of scope.
/// </param>
/// <remarks>
/// Called on the native game thread. UI updates triggered by this
/// callback must be marshalled to the main thread via
/// <see cref="MainThread.BeginInvokeOnMainThread"/>.
/// </remarks>
[UnmanagedFunctionPointer(CallingConvention.StdCall)]
public delegate void PrintGlyphCallback(
    int winid, int x, int y, IntPtr glyphInfo);
```

### DllImport Methods

```csharp
/// <summary>
/// Starts the native GnollHack game engine with the specified
/// callbacks and flags. Blocks until the game session ends.
/// </summary>
/// <param name="callbacks">
/// All callback function pointers the engine will invoke for
/// UI operations (display, input, sound, etc.).
/// </param>
/// <param name="runFlags">
/// Bitmask controlling engine behavior.
/// </param>
/// <returns>0 on success; non-zero error code on failure.</returns>
/// <remarks>
/// Must be called from a dedicated background thread — never
/// from the UI thread, as it blocks until the game ends.
/// </remarks>
[DllImport("gnollhacklib", CallingConvention = CallingConvention.StdCall)]
private static extern int RunGnollHack(
    ref CallbackStruct callbacks, uint runFlags);
```

### Marshalling Comments

Add inline comments explaining non-obvious marshalling decisions:

```csharp
// MarshalAs LPStr because the C engine uses char* (single-byte).
// .NET strings are UTF-16 internally; this attribute converts.
[MarshalAs(UnmanagedType.LPStr)]
public string? MessageText;
```

---

## Threading Documentation

GnollHack's multi-threaded architecture is a major source of complexity. See the `maui_threading` skill for full architectural details. Documentation should clearly communicate thread affinity and synchronization.

### Thread-Affinity Annotations

Use `<remarks>` to specify which thread a method must run on:

```csharp
/// <summary>
/// Updates the status bar with current player statistics.
/// </summary>
/// <remarks>
/// Must be called on the main (UI) thread. If called from the
/// native game thread, use
/// <see cref="MainThread.BeginInvokeOnMainThread"/> to dispatch.
/// </remarks>
public void UpdateStatusBar(StatusData data)
{
    ...
}
```

### Custom Thread-Safety Attributes

GnollHack defines custom attributes in [GHConstants.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX/GHConstants.cs) for annotating thread affinity. **Use these attributes** on methods where thread context matters — they serve as both documentation and a machine-readable contract:

```csharp
[ThreadSafe]        // Safe to call from any thread
[UIThreadOnly]      // Must only be called on the main (UI) thread
[GameThreadOnly]    // Must only be called on the native game thread
[RenderThreadOnly]  // Must only be called on the SkiaSharp render thread
```

Apply attributes alongside `<remarks>` XML docs. The attribute provides a scannable annotation; the `<remarks>` provides context on *why* the constraint exists:

```csharp
/// <summary>
/// Writes updated map tile data into the back buffer.
/// </summary>
/// <remarks>
/// Called from the native game thread during print_glyph callbacks.
/// The back buffer is swapped between frames, so writes here do
/// not require the render lock.
/// </remarks>
[GameThreadOnly]
public void UpdateTileData(int x, int y, LayerInfo layers)
{
    ...
}
```

### Lock Documentation

Document what each lock protects and the expected lock ordering:

```csharp
/// <summary>
/// Guards access to the <see cref="_mapData"/> buffer.
/// Acquired by the rendering thread during draw and by the game
/// thread during map updates. Keep hold time minimal to avoid
/// frame drops.
/// </summary>
private readonly object _mapDataLock = new();
```

---

## Inline Comments

Use inline comments (`//`) sparingly and only for non-obvious logic. C# code should be largely self-documenting through descriptive names.

### When to Use Inline Comments

1. **Platform-specific workarounds**

```csharp
// Android's GPU thread requires explicit GL context binding before
// SkiaSharp surface creation. iOS handles this automatically.
if (DeviceInfo.Platform == DevicePlatform.Android)
{
    EnsureGLContext();
}
```

2. **Game-engine-specific constants**

```csharp
// Glyph value 0 is NO_GLYPH in the C engine — empty tile
if (glyph == 0)
    return;
```

3. **Performance-critical sections**

```csharp
// Using Span<byte> to avoid heap allocation during per-frame
// tile decoding
ReadOnlySpan<byte> tileData = ...;
```

4. **Non-obvious LINQ or expression logic**

```csharp
// Filter to only identified inventory items, grouped by object
// class for display ordering
var groupedItems = inventory
    .Where(item => item.IsIdentified)
    .GroupBy(item => item.ObjectClass)
    .OrderBy(group => group.Key);
```

### When NOT to Use Inline Comments

```csharp
// BAD: Restates the code
int count = items.Count; // get the count of items

// BAD: Explains standard C# patterns
await Task.Run(() => ProcessAsync()); // run asynchronously

// BAD: Documents obvious property access
string name = player.Name; // get the player's name
```

---

## What NOT to Document

1. **Standard C# patterns** — Don't explain `using`, `async`/`await`, properties, LINQ, or other well-known language features.

2. **Trivial properties with self-documenting names**:
   ```csharp
   // Skip XML docs for these — the names are sufficient
   public string PlayerName { get; set; }
   public bool IsGameRunning { get; }
   ```

3. **Auto-generated code** — Don't add documentation to designer-generated files, `.g.cs` files, or build output.

4. **Obvious event handlers** — Simple one-line handlers that navigate or toggle UI state don't need docs.

5. **Private fields with clear names in small classes**:
   ```csharp
   // In a small, focused class, these are self-evident
   private readonly SKCanvas _canvas;
   private readonly List<GHTile> _visibleTiles;
   ```

6. **Content covered by other skills** — Threading patterns are documented in the `maui_threading` skill. Rendering architecture is in `glyph_tile_display`. Don't duplicate this content in code comments.

---

## Documentation Checklist for AI Agents

When modifying a C# file, apply the following checklist:

- [ ] **Class/struct**: Has a `<summary>` XML doc comment describing its purpose.
- [ ] **Public/protected methods**: Have `<summary>`, `<param>`, `<returns>`, and `<exception>` tags as appropriate.
- [ ] **P/Invoke declarations**: Delegates and `DllImport` methods are fully documented with marshalling notes and threading context.
- [ ] **Thread-sensitive code**: Thread affinity is annotated with custom attributes (`[GameThreadOnly]`, etc.) and `<remarks>`.
- [ ] **Constants**: Groups of related constants have summary documentation.
- [ ] **Enums**: Have `<summary>` on the type; non-obvious members have individual docs.
- [ ] **Platform-specific code**: Platform branches have inline comments explaining why.
- [ ] **No over-documentation**: Standard patterns, trivial members, and obvious code are not cluttered with unnecessary comments.
- [ ] **Cross-references**: Related types and methods use `<see cref="..."/>` for navigability.
- [ ] **Existing comments preserved**: No existing comments were removed unless factually incorrect.
- [ ] **Comment style consistency**: New docs use `///` for XML and `//` for inline (not `/* */`).

---

## Supplementary Patterns

For additional guidance on region markers, enum documentation, XAML code-behind patterns, exception documentation, and constant/configuration documentation, see [references/supplementary_patterns.md](file:///c:/hmp/GnollHack/.agents/skills/csharp_code_documentation/references/supplementary_patterns.md).
