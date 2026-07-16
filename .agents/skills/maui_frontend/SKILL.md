---
name: maui_frontend
description: Guidelines for working on the C# .NET MAUI mobile and desktop frontend client (GnollHackM), SkiaSharp game rendering, XAML page layouts, FMOD audio integration, and the P/Invoke native bridge.
---

# MAUI Frontend

GnollHack's graphical client is a .NET 10.0 MAUI application targeting Android, iOS, MacCatalyst, and Windows Desktop (WinUI 3).

## Critical Rules
- **UI Thread Safety**: All UI updates must be marshaled to the main thread via `MainThread.BeginInvokeOnMainThread()`.
- **Do NOT block the UI Thread**: Long-running native calls or file I/O must run asynchronously.
- **Cross-Platform XAML**: Use `OnPlatform` in XAML to handle differences between iOS/Android/Windows.
- **XAML WidthRequest / HeightRequest Quirk**: If a GnollHackX XAML file has exactly two spaces between `WidthRequest` or `HeightRequest` and `=` (e.g., `WidthRequest  =` or `HeightRequest  =`), the `makedefsdroid` build process automatically converts them into `MaximumWidthRequest` and `MaximumHeightRequest` respectively for GnollHackM. You MUST preserve these exact extra spaces if you are modifying or copying these blocks to retain this functionality.

## Project Structure
- **`GnollHackM`**: The MAUI application project (Entry point).
- **`GnollHackX`**: Shared code project containing Views, Pages, and Services.
- **`libshare`**: The native C bridge that interfaces with the GnollHack C core.

| Component | Path | Purpose |
|-----------|------|----------|
| **GnollHackM** | [win/win32/xpl/GnollHackM/](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackM) | .NET MAUI application project. Contains XAML pages, platform-specific code, and the `.csproj`. |
| **GnollHackX** (shared code) | [win/win32/xpl/GnollHackX/GnollHackX/](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX) | 70+ shared C# files: game logic, rendering, constants, controls. Compiled into GnollHackM via `<Compile Include>` file-linking (NOT project references). |
| **GnollHackX.Android** (platform) | [win/win32/xpl/GnollHackX/GnollHackX.Android/](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX.Android) | Android platform service. [PlatformServiceAndroid.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX.Android/PlatformServiceAndroid.cs) is file-linked into GnollHackM (line 737 of `.csproj`, inside the `net10.0-android` ItemGroup). Uses `Xamarin.Google.Android.Play.Core` for Google Play Store in-app reviews (`IReviewManager`, `ReviewManagerFactory`, `ReviewInfo`). |
| **GnollHackX.iOS** (platform) | [win/win32/xpl/GnollHackX/GnollHackX.iOS/](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX.iOS) | iOS platform service. [PlatformServiceiOS.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX.iOS/PlatformServiceiOS.cs) and [GHUIApplication.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX.iOS/GHUIApplication.cs) are file-linked into GnollHackM (inside the `net10.0-ios` ItemGroup). |
| **GnollHackX.Common** | [win/win32/xpl/GnollHackX/GnollHackX.Common/](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX.Common) | Contains ONLY [GnollHackService.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX.Common/GnollHackService.cs) — the P/Invoke bridge to native code. File-linked into GnollHackM. |
| **GnollHackX.FMOD** | [win/win32/xpl/GnollHackX/GnollHackX.FMOD/](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX.FMOD) | FMOD SDK C# wrappers for audio playback. File-linked into GnollHackM. |
| **libshare** (C side) | [win/win32/xpl/libshare/](file:///c:/hmp/GnollHack/win/win32/xpl/libshare) | Native C bridge code (gnhapi.h, callback.h, libproc.c). |

> **Important:** Because GnollHackM uses `<Compile Include>` file-linking (not project references), NuGet packages required by file-linked code must be declared in `GnollHackM.csproj` — not in a separate project. When evaluating whether a NuGet package is used, always search `GnollHackX.*` source directories, not just `GnollHackM/`.

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
| MacCatalyst | `__Internal` | `gnollhackmac` | Static linking (`.a` archive) |
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
* MacCatalyst uses **desktop banks** (from `Platforms/Windows/banks/`), not mobile banks
* FMOD on MacCatalyst uses dynamic linking (`libfmod_maccatalyst.dylib`) instead of static (`__Internal`) — see `vtool_fmod_patching` skill
* ~10 sound-related callbacks in the `RunGnollHack` P/Invoke

---

## Additional Integrations

| Integration | Package | Purpose |
|-------------|---------|----------|
| Crash reporting | Sentry.Maui 6.5.0 | Error tracking on all platforms |
| Cloud storage | Azure.Storage.Blobs 12.25.0 | Cloud save/import/export |
| JSON | Newtonsoft.Json 13.0.4 + System.Text.Json 10.0.0 | Data serialization |
| Play Store reviews | Xamarin.Google.Android.Play.Core 1.10.3.21 | Google Play in-app review flow (Android only). Used by file-linked [PlatformServiceAndroid.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX.Android/PlatformServiceAndroid.cs) via `IReviewManager` / `ReviewManagerFactory`. |

---

## Platform Bootstrap

| Platform | Entry Point | Key Setup |
|----------|-------------|------------|
| Android | [MainApplication.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackM/Platforms/Android/MainApplication.cs) | Loads native `.so` via `JavaSystem.LoadLibrary()`, initializes FMOD |
| iOS | [Program.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackM/Platforms/iOS/Program.cs) | Calls `LibTest()` to verify static link, launches UIApplication |
| MacCatalyst | [Program.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackM/Platforms/MacCatalyst/Program.cs) | Calls `LibTest()` to verify static link, launches UIApplication with GHUIApplication |
| Windows | [App.xaml.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackM/Platforms/Windows/App.xaml.cs) | WinUI 3 app lifecycle |

All platforms delegate lifecycle to `GHApp.OnStart()`, `GHApp.OnResume()`, `GHApp.OnSleep()` in [App.xaml.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackM/App.xaml.cs).

## SkiaSharp Rendering
- Map and status UI are rendered using SkiaSharp (`SKCanvasView`).
- **`InvalidateSurface()`**: Call this to trigger a redraw of the canvas. Do not call it excessively.

## P/Invoke Bridge (`GnollHackService.cs`)
- All calls to the C engine pass through `GnollHackService`.
- **Delegates**: Used to pass C# callbacks to C (e.g., `gh_set_display_callback()`).
- Keep `[MarshalAs(UnmanagedType.LPStr)]` for strings crossing the bridge to convert C# Unicode to C-style UTF-8 strings.
