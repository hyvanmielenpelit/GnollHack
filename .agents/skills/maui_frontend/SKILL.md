---
name: maui_frontend
description: Guidelines for working on the C# .NET MAUI mobile and desktop frontend client (GnollHackM), SkiaSharp game rendering, XAML page layouts, FMOD audio integration, and the P/Invoke native bridge.
---

# MAUI Frontend

GnollHack's graphical client is a .NET 10.0 MAUI application targeting Android, iOS, and Windows Desktop (WinUI 3).

## Critical Rules
- **UI Thread Safety**: All UI updates must be marshaled to the main thread via `MainThread.BeginInvokeOnMainThread()`.
- **Do NOT block the UI Thread**: Long-running native calls or file I/O must run asynchronously.
- **Cross-Platform XAML**: Use `OnPlatform` in XAML to handle differences between iOS/Android/Windows.
- **XAML `WidthRequest` / `HeightRequest` → `MaximumWidthRequest` / `MaximumHeightRequest` Convention**:

  > [!CAUTION]
  > **This is one of the most common XAML errors.** Getting this wrong causes either compilation failures or layout bugs that are hard to track down. Read this carefully.

  In GnollHackX XAML (the Xamarin source files), the `makedefsdroid` build tool converts attributes with **exactly two spaces before `=`** into their `Maximum` counterparts for GnollHackM (MAUI). Specifically:
  - `WidthRequest  =` (two spaces) → `MaximumWidthRequest=` in GnollHackM
  - `HeightRequest  =` (two spaces) → `MaximumHeightRequest=` in GnollHackM

  **There are two equally critical mistakes to avoid:**

  | ❌ WRONG (in GnollHackX XAML) | Why It's Wrong | ✅ CORRECT (in GnollHackX XAML) |
  |------|------|------|
  | `MaximumWidthRequest="400"` | Does NOT exist in Xamarin.Forms — **will not compile** | `WidthRequest  ="400"` (two spaces before `=`) |
  | `MaximumHeightRequest="300"` | Does NOT exist in Xamarin.Forms — **will not compile** | `HeightRequest  ="300"` (two spaces before `=`) |
  | `WidthRequest="400"` (when you want Maximum) | Produces plain `WidthRequest` in GnollHackM — **elements become overly wide** | `WidthRequest  ="400"` (two spaces before `=`) |
  | `HeightRequest="300"` (when you want Maximum) | Produces plain `HeightRequest` in GnollHackM — **elements become overly tall** | `HeightRequest  ="300"` (two spaces before `=`) |

  **When to use each form:**
  - Use `WidthRequest="..."` (no extra spaces, standard `=`) when you genuinely want a **fixed `WidthRequest`** on both Xamarin and MAUI sides.
  - Use `WidthRequest  ="..."` (two spaces before `=`) when you want `MaximumWidthRequest` on the MAUI side. This is the **only** way to achieve `MaximumWidthRequest` in GnollHackM.

## XAML Pipeline: GnollHackX → makedefsdroid → GnollHackM

> **⚠️ CRITICAL**: XAML files in `GnollHackM/` are **auto-generated** — do NOT edit them directly.
> 
> *Note: Although auto-generated, these `.xaml` files are currently checked into the repository for ease of use. This is expected behavior for the time being.*

The **source of truth** for all XAML files is in the legacy Xamarin project at `win/win32/xpl/GnollHackX/GnollHackX/`. A build tool called `makedefsdroid` transforms these into MAUI-compatible XAML and copies them to `GnollHackM/`. This means:

1. **Always edit XAML in `GnollHackX/GnollHackX/`** (e.g., `Pages/MainScreen/SettingsPage.xaml`)
2. **Code-behind (`.xaml.cs`) files** are shared via `<Compile Include>` file-linking and can be edited directly — they are the same physical file for both projects
3. **After modifying any XAML**, the MAUI XAML in GnollHackM must be regenerated. The agent should attempt this itself by building the `makedefsdroid` project (see below). If `x:Name` elements were added or removed, the code-behind `.g.cs` files for GnollHackM also need regeneration.
4. **Until regeneration happens**, GnollHackM builds will fail with `CS0103: The name 'ElementName' does not exist in the current context` for any newly added `x:Name` references

### What to do after modifying XAML

The XAML conversion is performed by MSBuild targets in the `makedefsdroid` project (`win/win32/vs/makedefsdroid.vcxproj`). **First, try to regenerate the MAUI XAML yourself** by building this project:

```powershell
# Locate MSBuild via vswhere (msbuild is not on the default PowerShell PATH)
$msbuild = & "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe" -latest -requires Microsoft.Component.MSBuild -find MSBuild\**\Bin\MSBuild.exe
& $msbuild win/win32/vs/makedefsdroid.vcxproj /t:Build /p:Configuration=Debug /p:Platform=x64
```

This runs the `InitialBuild` target which transforms all GnollHackX XAML files into MAUI-compatible XAML in `GnollHackM/`.

**Only if the build fails** (e.g., missing Linux/WSL build tools, MSBuild not found), fall back to asking the user:

> "I've modified the XAML in GnollHackX but was unable to regenerate the MAUI XAML automatically. Could you please build the `makedefsdroid` project (or the GnollHackX solution) so that the MAUI XAML for GnollHackM is regenerated?"

### What makedefsdroid converts

- Xamarin.Forms namespaces → .NET MAUI namespaces
- `WidthRequest  =` (two spaces) → `MaximumWidthRequest=` (**never write `MaximumWidthRequest` directly in GnollHackX!**)
- `HeightRequest  =` (two spaces) → `MaximumHeightRequest=` (**never write `MaximumHeightRequest` directly in GnollHackX!**)
- `WidthRequest=` (no extra spaces) is **NOT converted** — it stays as `WidthRequest=` in GnollHackM. Only use this form when you actually want a plain `WidthRequest`, not a `MaximumWidthRequest`.
- Other Xamarin-to-MAUI compatibility transforms

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
| JSON | Newtonsoft.Json 13.0.4 + System.Text.Json 10.0.0 | Data serialization |
| Play Store reviews | Xamarin.Google.Android.Play.Core 1.10.3.21 | Google Play in-app review flow (Android only). Used by file-linked [PlatformServiceAndroid.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX.Android/PlatformServiceAndroid.cs) via `IReviewManager` / `ReviewManagerFactory`. |

---

## Platform Bootstrap

| Platform | Entry Point | Key Setup |
|----------|-------------|------------|
| Android | [MainApplication.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackM/Platforms/Android/MainApplication.cs) | Loads native `.so` via `JavaSystem.LoadLibrary()`, initializes FMOD |
| iOS | [Program.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackM/Platforms/iOS/Program.cs) | Calls `LibTest()` to verify static link, launches UIApplication |
| Windows | [App.xaml.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackM/Platforms/Windows/App.xaml.cs) | WinUI 3 app lifecycle |

All platforms delegate lifecycle to `GHApp.OnStart()`, `GHApp.OnResume()`, `GHApp.OnSleep()` in [App.xaml.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackM/App.xaml.cs).

## SkiaSharp Rendering
- Map and status UI are rendered using SkiaSharp (`SKCanvasView`).
- **`InvalidateSurface()`**: Call this to trigger a redraw of the canvas. Do not call it excessively.

## P/Invoke Bridge (`GnollHackService.cs`)
- All calls to the C engine pass through `GnollHackService`.
- **Delegates**: Used to pass C# callbacks to C (e.g., `gh_set_display_callback()`).
- Keep `[MarshalAs(UnmanagedType.LPStr)]` for strings crossing the bridge to convert C# Unicode to C-style UTF-8 strings.
