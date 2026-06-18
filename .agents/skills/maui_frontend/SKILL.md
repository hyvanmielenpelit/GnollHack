---
name: maui_frontend
description: Guidelines for working on the C# .NET MAUI mobile and desktop frontend client (GnollHackM), SkiaSharp game rendering, XAML page layouts, FMOD audio integration, and the P/Invoke native bridge.
---

# .NET MAUI Frontend Development

GnollHack's graphical client is a .NET 9.0 MAUI application targeting Android, iOS, and Windows Desktop (WinUI 3).

## Project Structure

| Component | Path | Purpose |
|-----------|------|----------|
| **GnollHackM** | [win/win32/xpl/GnollHackM/](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackM) | .NET MAUI application project. Contains XAML pages, platform-specific code, and the `.csproj`. |
| **GnollHackX** (shared code) | [win/win32/xpl/GnollHackX/GnollHackX/](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX) | 70+ shared C# files: game logic, rendering, constants, controls. Compiled into GnollHackM via `<Compile Include>` file-linking (NOT project references). |
| **GnollHackX.Common** | [win/win32/xpl/GnollHackX/GnollHackX.Common/](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX.Common) | Contains ONLY [GnollHackService.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX.Common/GnollHackService.cs) — the P/Invoke bridge to native code. |
| **GnollHackX.FMOD** | [win/win32/xpl/GnollHackX/GnollHackX.FMOD/](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX.FMOD) | FMOD SDK C# wrappers for audio playback. |
| **libshare** (C side) | [win/win32/xpl/libshare/](file:///c:/hmp/GnollHack/win/win32/xpl/libshare) | Native C bridge code (gnhapi.h, callback.h, libproc.c). |

---

## Key Shared C# Classes

| File | Size | Purpose |
|------|------|----------|
| [GHApp.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX/GHApp.cs) | 633KB | **Central application logic.** Static class managing app state, initialization, fonts, resources, platform services, game lifecycle. |
| [GHGame.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX/GHGame.cs) | 233KB | Manages individual game instances. Handles native callback dispatch, game threading, request/response queuing. |
| [GHConstants.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX/GHConstants.cs) | 114KB | All shared constants — critical reference for any feature work. |
| [GHWindow.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX/GHWindow.cs) | 28KB | Window abstraction matching NetHack's window types (NHW_MESSAGE, NHW_MAP, NHW_MENU, NHW_TEXT). |
| [UIUtils.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX/UIUtils.cs) | 86KB | UI utility functions for layout, scaling, text rendering. |
| [IGnollHackService.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX/IGnollHackService.cs) | 4KB | Interface contract for native interop (37 methods: init, tile data, animation, version, game control). |
| [IFmodService.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX/IFmodService.cs) | 3KB | Interface for FMOD audio service. |

---

## Rendering Architecture: SkiaSharp

The entire game dungeon is rendered using **SkiaSharp 3.119.1** via custom canvas views:

* `SwitchableCanvasView` wraps `SKCanvasView` — the primary game rendering surface
* The `PaintSurface` event handler in `GamePage.xaml.cs` performs all tile/sprite drawing
* Touch, mouse wheel, and pointer events are handled directly on the canvas
* Custom controls like `CustomLabel` also use `SKCanvasView` for SkiaSharp-based text rendering
* SkiaSharp is registered in [MauiProgram.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackM/MauiProgram.cs) via `.UseSkiaSharp()`

---

## Native Interop Bridge

### PlatformConstants (per-platform DLL names)

| Platform | DLL Name | Library Name | Linking |
|----------|----------|--------------|----------|
| iOS | `__Internal` | `gnollhackios` | Static linking (`.a` archive) |
| Android | `libgnollhackdroid.so` | `gnollhackdroid` | Dynamic loading via `JavaSystem.LoadLibrary()` |
| Windows | `gnollhackwin.dll` | `gnollhackwin` | Dynamic loading (MauiAsset) |

Defined in [GnollHackService.cs:52-67](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX.Common/GnollHackService.cs#L52).

### RunGnollHack P/Invoke

The main entry point is a single massive P/Invoke function `RunGnollHack()` accepting ~70 callback delegates:

```csharp
[DllImport(PlatformConstants.dll)]
public static extern int RunGnollHack(
    string gnhdir,
    string cmdlineargs,
    // ... run flags, window caps ...
    InitWindowsCallback callback_init_nhwindows,
    PrintGlyphCallback callback_print_glyph,
    PlayImmediateSoundCallback callback_play_immediate_ghsound,
    // ... 60+ more callbacks ...
);
```

Callback categories: window management, input handling, display/glyph rendering, menu system, sound/music playback, animation control, status updates, data transfer.

---

## XAML Pages

### Primary Pages

| Page | Size | Purpose |
|------|------|----------|
| [MainPage.xaml](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackM/MainPage.xaml) | 32KB | Start screen: logo animation, role/mode selection, start/resume buttons. Uses fonts "ARChristy" and "Diablo". |
| [GamePage.xaml](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackM/GamePage.xaml) | 53KB | Active gameplay: SkiaSharp canvas, command button grids, status overlays, message filter. |
| [SettingsPage.xaml](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackM/SettingsPage.xaml) | 141KB | Comprehensive settings: graphics, sound volumes, keybindings, debug flags. |
| [GameMenuPage.xaml](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackM/GameMenuPage.xaml) | 6KB | In-game menu overlay. |

### Other Pages (28 total XAML files)
AboutPage, AchievementsPage, DisplayFilePage, EditorPage, ImportExportPage, LibraryPage, MusicPage, NamePage, OraclePage, OutRipPage, ReplayPage, ResetPage, SnapshotPage, TopScorePage, VaultPage, VersionPage, WikiPage.

### Custom Controls
`SwitchableCanvasView`, `LabeledImageButton`, `SimpleImageButton`, `CustomImageButton`, `RowImageButton`, `MeasurableGrid`, `MeasurableStackLayout`, `CustomLabel`, `GHCachedImage`, `ImageCarousel`.

---

## Threading Model

The C game engine runs on a **background thread**. Native callbacks arrive on that thread.

**Rule:** All UI updates in callback handlers must be marshalled to the main thread:

```csharp
MainThread.BeginInvokeOnMainThread(() =>
{
    // Safe to update XAML controls or invalidate canvas here
});
```

This pattern is used 119+ times across the codebase.

---

## FMOD Audio Integration

* Full FMOD Studio SDK integration for music, ambient sounds, effects, and voiceovers
* C# wrappers in `GnollHackX.FMOD/`: `fmod.cs`, `fmod_studio.cs`, `FMODService.cs`
* `IFmodService` interface for platform-independent audio API
* Android: initialized via `FmodService.AndroidInit()` in `MainApplication` constructor
* Native sound banks stored per-platform under `Platforms/{platform}/banks/`
* ~10 sound-related callbacks in the `RunGnollHack` P/Invoke

---

## Additional Integrations

| Integration | Package | Purpose |
|-------------|---------|----------|
| Crash reporting | Sentry.Maui 6.5.0 | Error tracking on all platforms |
| Cloud storage | Azure.Storage.Blobs 12.25.0 | Cloud save/import/export |
| JSON | Newtonsoft.Json 13.0.4 + System.Text.Json 9.0.16 | Data serialization |
| Play Store | Xamarin.Google.Android.Play.Core 1.10.3.21 | Android in-app updates (Android only) |

---

## Platform Bootstrap

| Platform | Entry Point | Key Setup |
|----------|-------------|------------|
| Android | [MainApplication.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackM/Platforms/Android/MainApplication.cs) | Loads native `.so` via `JavaSystem.LoadLibrary()`, initializes FMOD |
| iOS | [Program.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackM/Platforms/iOS/Program.cs) | Calls `LibTest()` to verify static link, launches UIApplication |
| Windows | [App.xaml.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackM/Platforms/Windows/App.xaml.cs) | WinUI 3 app lifecycle |

All platforms delegate lifecycle to `GHApp.OnStart()`, `GHApp.OnResume()`, `GHApp.OnSleep()` in [App.xaml.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackM/App.xaml.cs).
