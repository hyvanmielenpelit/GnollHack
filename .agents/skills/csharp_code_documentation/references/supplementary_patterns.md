# Supplementary C# Documentation Patterns

This reference supplements the main [SKILL.md](file:///c:/hmp/GnollHack/.agents/skills/csharp_code_documentation/SKILL.md) with additional documentation patterns for region markers, enums, XAML code-behind, exceptions, and constants.

---

## Region Markers

Use `#region` blocks to organize large files into logical sections. This is particularly important for GnollHack's largest files like [GamePage.xaml.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX/Pages/Game/GamePage.xaml.cs) (24,000+ lines) and [GHApp.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX/GHApp.cs) (12,000+ lines).

```csharp
#region Native Callbacks
// Callback delegates and methods invoked by the C game engine
...
#endregion

#region Map Rendering
// SkiaSharp drawing logic for the game map canvas
...
#endregion

#region Player Input
// Touch, keyboard, and gesture input handling
...
#endregion

#region Status Display
// HP bar, status conditions, inventory summary
...
#endregion
```

**Rules for regions:**
- Use regions only in files with 200+ lines
- Never nest regions
- Never put a single method in its own region
- Group related methods and fields together logically
- Add a brief `//` comment after the region name describing the contents

**Note**: The existing codebase does not currently use `#region` markers (they are only found in third-party FMOD wrapper files). When adding regions to existing files, use them to organize major sections without changing code structure.

---

## Enum Documentation

Document enums with a summary and per-member descriptions for non-obvious values:

```csharp
/// <summary>
/// Specifies the type of window managed by the native game engine.
/// Window types correspond to the C engine's <c>NHW_*</c> constants.
/// </summary>
public enum GHWindowType
{
    /// <summary>The scrolling message log at the top of the screen.</summary>
    Message = 1,

    /// <summary>The player's status display (HP, AC, conditions).</summary>
    Status = 2,

    /// <summary>The dungeon map view.</summary>
    Map = 3,

    /// <summary>A modal menu for item selection or choice prompts.</summary>
    Menu = 4,

    /// <summary>A text display window for multi-line information.</summary>
    Text = 5
}
```

For enums with many members that follow a clear pattern, document only the enum itself and any exceptions:

```csharp
/// <summary>
/// Sound effect identifiers matching the C engine's <c>GHSOUND_*</c>
/// constants. Values must stay in sync with <c>include/general.h</c>.
/// </summary>
public enum GHSoundEffect
{
    None = 0,
    HitMelee = 1,
    HitRanged = 2,
    // ... values follow the C header ordering
}
```

For flag enums, document the `[Flags]` attribute and combination semantics:

```csharp
/// <summary>
/// Bitwise flags controlling native engine run behavior.
/// Combine with <c>|</c> operator.
/// </summary>
[Flags]
public enum RunFlags : uint
{
    /// <summary>No special behavior.</summary>
    None = 0,

    /// <summary>Enable wizard (debug) mode.</summary>
    WizardMode = 1 << 0,

    /// <summary>Restore from a saved game file.</summary>
    RestoreGame = 1 << 1,
}
```

---

## XAML Code-Behind Documentation

For XAML code-behind files, document the page/control purpose at the class level. Individual event handlers typically don't need XML docs unless they contain complex logic.

```csharp
/// <summary>
/// Main game page hosting the SkiaSharp map canvas, status bars,
/// message log, and on-screen input controls. Manages the lifecycle
/// of a single game session.
/// </summary>
public partial class GamePage : ContentPage
{
    /// <summary>
    /// Handles the map canvas tap gesture by converting screen
    /// coordinates to tile coordinates and sending a move/interact
    /// command.
    /// </summary>
    private void OnMapTapped(object sender, TappedEventArgs e)
    {
        ...
    }

    // Simple UI event handlers don't need XML docs
    private void OnSettingsClicked(object sender, EventArgs e)
    {
        Navigation.PushAsync(new SettingsPage());
    }
}
```

### When to Document Event Handlers

Document event handlers when they:
- Contain **threading logic** (dispatching between game thread and UI thread)
- Involve **complex coordinate transformations** (screen → tile mapping)
- Trigger **native engine commands** (sending input through the bridge)
- Have **platform-specific behavior** (different logic per platform)

Skip documentation for handlers that:
- Simply navigate to another page
- Toggle a boolean property
- Show/hide a UI element

---

## Exception Documentation

Document exceptions using `<exception>` tags for public methods that throw:

```csharp
/// <summary>
/// Loads a tileset image from the specified asset path.
/// </summary>
/// <param name="assetPath">
/// Relative path to the tileset PNG within app assets.
/// </param>
/// <returns>
/// A decoded <see cref="SKBitmap"/> containing the tileset atlas.
/// </returns>
/// <exception cref="FileNotFoundException">
/// Thrown if <paramref name="assetPath"/> does not exist in
/// the app bundle.
/// </exception>
/// <exception cref="InvalidDataException">
/// Thrown if the file exists but is not a valid PNG image.
/// </exception>
public SKBitmap LoadTileset(string assetPath)
{
    ...
}
```

### Exception Documentation Guidelines

- Only document exceptions that the method **explicitly throws** or that callers should **reasonably handle**.
- Don't document `ArgumentNullException` for every parameter — this is implied by non-nullable reference types in modern C#.
- Document `InvalidOperationException` when the method has state preconditions.
- Document domain-specific exceptions (e.g., game not initialized, native engine error).

---

## Constants and Configuration

Document constant groups and configuration values with class-level and member-level summaries:

```csharp
/// <summary>
/// Default rendering constants for the game map canvas.
/// </summary>
internal static class RenderDefaults
{
    /// <summary>Tile width in pixels at 1x zoom.</summary>
    public const int TileWidth = 64;

    /// <summary>Tile height in pixels at 1x zoom.</summary>
    public const int TileHeight = 96;

    /// <summary>Maximum zoom level (4x magnification).</summary>
    public const float MaxZoom = 4.0f;

    /// <summary>
    /// Minimum zoom level (fits the full map width on screen).
    /// </summary>
    public const float MinZoom = 0.25f;
}
```

### Constants That Mirror C Engine Values

When C# constants must stay in sync with C header definitions, document the relationship:

```csharp
/// <summary>
/// Maximum number of game windows that can be open simultaneously.
/// Must match <c>MAX_GHWINDOWS</c> in the C engine's
/// <c>include/general.h</c>.
/// </summary>
public const int MaxGHWindows = 32;
```

---

## Async Method Documentation

For async methods, document what the method awaits and whether the caller should await the result:

```csharp
/// <summary>
/// Saves the current game state to the specified file path.
/// </summary>
/// <param name="filePath">
/// Absolute path for the save file. Parent directory must exist.
/// </param>
/// <returns>
/// A task that completes when the save file has been written
/// and verified.
/// </returns>
/// <remarks>
/// This method performs file I/O on a background thread. The
/// native game engine is paused during the save operation.
/// </remarks>
public async Task SaveGameAsync(string filePath)
{
    ...
}
```

---

## Nullable Reference Type Documentation

When a parameter or return value is nullable, document what `null` means:

```csharp
/// <summary>
/// Finds the game window with the specified handle.
/// </summary>
/// <param name="handle">The native window handle to search for.</param>
/// <returns>
/// The matching <see cref="GHWindow"/>, or <c>null</c> if no
/// window with this handle exists.
/// </returns>
public GHWindow? FindWindow(int handle)
{
    ...
}
```
