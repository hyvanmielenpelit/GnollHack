---
name: maui_frontend
description: Guidelines for working on the C# .NET MAUI mobile and desktop frontend client (GnollHackM), XAML game layouts, and the P/Invoke native bridge.
---

# .NET MAUI Frontend Development

This guide covers the structure, native interop wrapper, page layout, and threading conventions used to develop the modern .NET MAUI client for GnollHack.

## Structure & Architecture

The frontend consists of two main pieces:

1. **Shared Native C# API Wrapper**:
   * Located at: **[win/win32/xpl/GnollHackX/GnollHackX.Common/](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX.Common/)**
   * Core files: [IGnollHackService.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX/IGnollHackService.cs) and [GnollHackService.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX.Common/GnollHackService.cs).
   * Declares raw P/Invokes to the native library (`PlatformConstants.dll`) and registers callback hooks (e.g. key presses, menu outputs, drawing grids).

2. **.NET MAUI Client (GnollHackM)**:
   * Located at: **[win/win32/xpl/GnollHackM/](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackM/)**
   * Target platforms: Android, iOS, Windows Desktop (via WinUI 3).
   * Framework version: .NET 9.0 MAUI.

---

## Native Interop Bridge (`GnollHackService.cs`)

The core game logic runs on the C backend. To coordinate with the .NET application:
* The native DLL (`gnollhack` or similar depending on target OS) is dynamically loaded.
* Standard P/Invokes are used to send player inputs or query configuration.
* Callback function pointers are registered with the native core on startup. These pointers listen to game events, such as rendering glyphs, updating log messages, and requesting selection menus.
* Always check the parameter marshaling. Ensure `MarshalAs(UnmanagedType.LPStr)` is used for strings passed between C and C#.

---

## Key Page Components

* **[MainPage.xaml](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackM/MainPage.xaml)**: The entry point UI where users choose their role, gender, race, alignment, and start/resume a game.
* **[GamePage.xaml](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackM/GamePage.xaml)**: The active gameplay screen containing:
  * Dungeon rendering grid (via custom canvases).
  * Status bars representing character health, energy, experience, and inventory shortcuts.
  * Virtual control overlays (touch pad controls, direction wheels, inventory pane overlays).
  * The log view displaying message outputs (`pline` calls from the core).
* **[SettingsPage.xaml](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackM/SettingsPage.xaml)**: Comprehensive settings containing controls for graphics detail, sound volumes, key-bindings, and debug flags.

---

## Threading and UI Marshalling

* **Native Callback Threads**: The C game engine runs on its own background thread. When the engine executes callbacks (e.g., drawing maps, writing text, playing audio), these callbacks run on background threads.
* **Main UI Thread**: .NET MAUI elements must be updated on the main thread. To prevent crashes, any UI updates within native callback handlers must be marshalled:
  ```csharp
  MainThread.BeginInvokeOnMainThread(() =>
  {
      // Safe to update XAML controls or trigger canvas invalidation here
  });
  ```
* **FMOD Audio Integration**: Platform-specific audio managers coordinate background music and voice lines. Ensure FMOD engine handles are correctly initialized on the correct thread.
