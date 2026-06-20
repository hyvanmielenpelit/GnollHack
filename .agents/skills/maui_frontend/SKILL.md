---
name: maui_frontend
description: Guidelines for working on the C# .NET MAUI mobile and desktop frontend client (GnollHackM), SkiaSharp game rendering, XAML page layouts, FMOD audio integration, and the P/Invoke native bridge.
---

# MAUI Frontend

## Critical Rules
- **UI Thread Safety**: All UI updates must be marshaled to the main thread via `MainThread.BeginInvokeOnMainThread()`.
- **Do NOT block the UI Thread**: Long-running native calls or file I/O must run asynchronously.
- **Cross-Platform XAML**: Use `OnPlatform` in XAML to handle differences between iOS/Android/Windows.

## Project Structure
- **`GnollHackM`**: The MAUI application project (Entry point).
- **`GnollHackX`**: Shared code project containing Views, Pages, and Services.
- **`libshare`**: The native C bridge that interfaces with the GnollHack C core.

## SkiaSharp Rendering
- Map and status UI are rendered using SkiaSharp (`SKCanvasView`).
- **`InvalidateSurface()`**: Call this to trigger a redraw of the canvas. Do not call it excessively.

## P/Invoke Bridge (`GnollHackService.cs`)
- All calls to the C engine pass through `GnollHackService`.
- **Delegates**: Used to pass C# callbacks to C (e.g., `gh_set_display_callback()`).
- Keep `[MarshalAs(UnmanagedType.LPStr)]` for strings crossing the bridge to convert C# Unicode to C-style UTF-8 strings.
