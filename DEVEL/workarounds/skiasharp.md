# SkiaSharp Workarounds

- Component: SkiaSharp (SkiaSharp, SkiaSharp.Views.Maui, HarfBuzzSharp) and Skia
- Upstream: https://github.com/mono/SkiaSharp, https://github.com/mono/skia
- ID prefix: WA-SKIASHARP

Entries follow the schema in [README.md](README.md). Append new entries at the end of the
file and never renumber or reuse an ID.

<!-- entries -->

## WA-SKIASHARP-0001: SKGLView has no SurfaceView-backed handler on Android

- Status: active
- Added: 2026-10-03 (from git history)
- GnollHack version: 3.22 (422)
- Component: SkiaSharp
- Package: SkiaSharp.Views.Maui.Core
- Defective version: SkiaSharp.Views.Maui.Core 4.152.3
- Platforms: Android
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/Platforms/Android/AndroidSKGLViewSurfaceHandler.cs` (`AndroidSKGLViewSurfaceHandler`); `win/win32/xpl/GnollHackM/Platforms/Android/AndroidSKGLViewSurfaceHandler.cs` (`AndroidSKGLViewSurfaceHandler.SurfaceTouchHandler`)
- Remove when: SkiaSharp.Views.Maui offers an SKGLView handler on SKGLSurfaceView with a usable touch handler, GnollHack adopts that release, and Verify removal passes.
- Related: WA-SKIASHARP-0002

**Symptom.** SkiaSharp.Views.Maui's `SKGLViewHandler` on Android always creates an
`SKGLTextureView`, which HWUI composites into the page's window. The map cannot be drawn
on its own SurfaceFlinger layer, which the "Direct GL Draw" setting needs.

**Cause.** The stock handler is hard-wired to `SKGLTextureView`, and SkiaSharp's Android
`SKTouchHandler` is `internal`, so a custom handler cannot reuse it.

**Workaround.** `AndroidSKGLViewSurfaceHandler` is a full `ViewHandler<ISKGLView,
SKGLSurfaceView>` for `MapSurfaceGLView`, registered in `MauiProgram.CreateMauiApp`. Its
`SurfaceTouchHandler` is adapted from SkiaSharp's internal Android touch handler.

**Verify removal.** Remove the handler and its registration, and use the stock handler's
SurfaceView option. Then:
- Build for Android.
- With "Direct GL Draw" on, play and replay a game, and confirm the map draws, takes
  touch and mouse input, and survives backgrounding and returning to the game.

Checks:
- 2026-10-23 | SkiaSharp.Views.Maui.Core 4.153.1 | still-needed | SkiaSharp main still creates SKGLTextureView, and SKTouchHandler is internal.

## WA-SKIASHARP-0002: SKGLSurfaceView keeps its GRContext across EGL contexts

- Status: active
- Added: 2026-10-04 (from git history)
- GnollHack version: 3.22 (422)
- Component: SkiaSharp
- Package: SkiaSharp.Views, SkiaSharp.Views.Maui.Core
- Defective version: SkiaSharp.Views.Maui.Core 4.153.1
- Platforms: Android
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/Platforms/Android/AndroidSKGLViewSurfaceHandler.cs` (`AndroidSKGLViewSurfaceHandler.OnPaintSurface`); `win/win32/xpl/GnollHackM/Platforms/Android/AndroidSKGLViewSurfaceHandler.cs` (`AndroidSKGLViewSurfaceHandler.MauiSKGLSurfaceView.OnAttachedToWindow`)
- Remove when: SkiaSharp's `SKGLSurfaceViewRenderer` recreates its GRContext for a new EGL context, GnollHack adopts that release, and Verify removal passes.
- Related: WA-SKIASHARP-0001

**Symptom.** After the map's GLSurfaceView is re-attached to a window or loses its EGL
context, the map can stop drawing correctly.

**Cause.** GLSurfaceView gives a re-attached view a new GL thread and EGL context.
`SKGLSurfaceViewRenderer` creates its GRContext once, in `OnDrawFrame`, and its
`OnSurfaceCreated` is empty, so it keeps using the GRContext of the destroyed context.

**Workaround.** None that repairs it. GnollHack detects the condition and logs it to the
app log and Sentry: `OnPaintSurface` compares the current EGL context with the previous
one, and `MauiSKGLSurfaceView.OnAttachedToWindow` reports a re-attach that kept a
GRContext.

**Verify removal.** Remove both checks. Then:
- Build for Android.
- With "Direct GL Draw" on, leave and re-enter the game page and background the app
  repeatedly, and confirm the map keeps drawing and the new SkiaSharp renderer creates
  a fresh GRContext each time (OnGRContextChanged fires).

Checks:
- 2026-10-23 | SkiaSharp.Views.Maui.Core 4.153.1 | still-needed | SKGLSurfaceViewRenderer.OnSurfaceCreated is still empty on SkiaSharp main.

## WA-SKIASHARP-0003: SKGLView has no Metal-backed handler on iOS

- Status: active
- Added: 2026-07-01 (from git history)
- GnollHack version: 2.145 (387)
- Component: SkiaSharp
- Package: SkiaSharp.Views.Maui.Core
- Defective version: SkiaSharp.Views.Maui.Core 4.148.0
- Platforms: iOS
- Upstream issue: https://github.com/mono/SkiaSharp/issues/4891
- Upstream status when added: unreported
- Upstream status: reported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/Platforms/iOS/iOSSKGLViewMetalHandler.cs` (`iOSSKGLViewMetalHandler`); `win/win32/xpl/GnollHackM/Platforms/iOS/iOSSKGLViewMetalHandler.cs` (`iOSSKGLViewMetalHandler.CustomSKTouchHandler`)
- Remove when: SkiaSharp.Views.Maui ships a Metal-backed SKGLView handler (issue 4891), GnollHack adopts that release, and Verify removal passes.
- Related: WA-SKIASHARP-0004, WA-SKIASHARP-0005

**Symptom.** SkiaSharp.Views.Maui's `SKGLViewHandler` on iOS renders only through the
deprecated OpenGL ES `GLKView`. There is no way to draw an `SKGLView` with Metal, which
the "iOS Metal Rendering" setting needs.

**Cause.** The stock iOS handler is a `ViewHandler<ISKGLView, SKGLView>` over OpenGL ES,
marked obsolete in favor of Metal, and SkiaSharp's Apple `SKTouchHandler` is `internal`.

**Workaround.** `iOSSKGLViewMetalHandler` is a full handler for `MetalGLView` over
GnollHack's own MTKView, registered in `MauiProgram.CreateMauiApp`.
`CustomSKTouchHandler` replaces the internal touch handler.

**Verify removal.** Remove the handler and its registration, and use the stock Metal
handler. Then:
- Build for iOS.
- With "iOS Metal Rendering" on, play a game on a device, and confirm the map draws, takes
  touch input, and survives backgrounding and leaving and re-entering the game.

Checks:
- 2026-10-23 | SkiaSharp.Views.Maui.Core 4.153.1 | still-needed | Issue open; SkiaSharp main's iOS SKGLViewHandler still uses GLKView.

## WA-SKIASHARP-0004: SKMetalView leaks per-frame wrappers and its GRContext

- Status: active
- Added: 2026-10-04 (from git history)
- GnollHack version: 3.22 (422)
- Component: SkiaSharp
- Package: SkiaSharp.Views, SkiaSharp.Views.Maui.Core
- Defective version: SkiaSharp.Views.Maui.Core 4.153.1
- Platforms: iOS
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/Platforms/iOS/iOSSKGLViewMetalHandler.cs` (`iOSSKGLViewMetalHandler.GHMetalView`)
- Remove when: SkiaSharp's `SKMetalView` disposes its drawable and texture wrappers each frame and releases its GRContext and command queue, GnollHack adopts that release, and Verify removal passes.
- Related: WA-SKIASHARP-0003

**Symptom.** With `SKMetalView`, frames stall waiting for a free drawable, and the view's
GPU resources outlive it.

**Cause.** `SKMetalView` reads `CurrentDrawable` and its `Texture` without disposing the
managed wrappers, so the layer cannot reuse a drawable that a GC-pending wrapper still
retains. Its per-view GRContext and command queue are never released.

**Workaround.** `GHMetalView` is GnollHack's own MTKView. It disposes every per-frame
wrapper at the end of `Draw`, frees its GRContext in `ReleaseGpuResources` when the
handler disconnects, and disposes its command queue.

**Verify removal.** Base the Metal handler on `SKMetalView` instead of `GHMetalView`.
Then:
- Build for iOS.
- With "iOS Metal Rendering" on, play for several minutes and leave and re-enter the game
  page repeatedly; confirm no frame stalls in the frame-time profiler and that the app's
  memory returns to its baseline after leaving the game.

Checks:
- 2026-10-23 | SkiaSharp.Views.Maui.Core 4.153.1 | still-needed | SKMetalView on SkiaSharp main still leaves CurrentDrawable and its texture undisposed.

## WA-SKIASHARP-0005: Skia uploads whole tile sheets under half the Metal cache limit

- Status: active
- Added: 2026-10-07 (from git history)
- GnollHack version: 3.24 (424)
- Component: SkiaSharp
- Package: SkiaSharp
- Defective version: SkiaSharp 4.153.1
- Platforms: iOS
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackX/GnollHackX/GHConstants.cs` (`GHConstants.MaxMetalGPUCacheSize`); `win/win32/xpl/GnollHackX/GnollHackX/Controls/SwitchableCanvasView.xaml.cs` (`SwitchableCanvasView.SetContextResourceCacheLimit`); `win/win32/xpl/GnollHackX/GnollHackX/GHApp.cs` (`GHApp.ComputeGPUCacheLimits`)
- Remove when: Skia, through a SkiaSharp release GnollHack adopts, uploads only the drawn subsets of a large raster image regardless of the cache limit, or exposes a way to force that, and Verify removal passes.
- Related: WA-SKIASHARP-0003

**Symptom.** With Metal rendering and a large GPU cache, the game slows down and iOS
terminates it for memory.

**Cause.** Skia uploads a raster tile sheet to the GPU whole, instead of the per-sprite
subsets drawn, when its decoded bytes are below half the context's GPU cache limit and it
fits the maximum texture size, which every sheet does on Metal. The sheets are 186-192 MB
each.

**Workaround.** Every Metal GRContext's cache limit is capped at 256 MB, below twice the
smallest sheet's bytes, so Skia keeps uploading subsets. The cap applies to the levels
offered in settings and to every limit set on a context.

**Verify removal.** Raise `MaxMetalGPUCacheSize` above twice the largest sheet's bytes.
Then:
- Build for iOS.
- With "iOS Metal Rendering" on and the largest GPU cache level, play on a device with
  little memory, and confirm the game is not terminated and the frame rate matches the
  256 MB setting.

Checks:
- 2026-10-23 | SkiaSharp 4.153.1 | unverified | No upstream report; Skia's tiling heuristic not re-read.

## WA-SKIASHARP-0006: ClipRect sometimes malfunctions on iOS SKGLView

- Status: active
- Added: 2025-07-29 (from git history)
- GnollHack version: 2.62 (285)
- Component: SkiaSharp
- Package: SkiaSharp, SkiaSharp.Views.Maui.Core
- Defective version: SkiaSharp 3.116.1
- Platforms: iOS
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackX/GnollHackX/Pages/Game/GamePage.xaml.cs` (`GamePage.DrawExtendedStatusBar`)
- Remove when: ClipRect is shown to work on iOS SKGLView with the SkiaSharp release GnollHack uses, and Verify removal passes.

**Symptom.** On iOS with the OpenGL ES `SKGLView`, the clip rectangle applied to the
extended status screen's scrolling text sometimes malfunctions.

**Cause.** Unknown. Seen only on iOS with `SKGLView`.

**Workaround.** The `canvas.ClipRect(cliprect)` call in `DrawExtendedStatusBar` is
commented out. The scrolling text is bounded by the computed rectangle alone.

**Verify removal.** Restore the `ClipRect` call. Then:
- Build for iOS.
- With OpenGL ES rendering, open the extended status screen repeatedly and scroll it,
  and confirm the text is clipped correctly every time.

Checks:
- 2026-10-23 | SkiaSharp 4.153.1 | unverified | Not re-tested; no upstream report found.

## WA-SKIASHARP-0007: SkiaSharp's Windows touch handler is internal and reads every WinRT object

- Status: active
- Added: 2026-10-04 (from git history)
- GnollHack version: 3.22 (422)
- Component: SkiaSharp
- Package: SkiaSharp.Views.Maui.Core
- Defective version: SkiaSharp.Views.Maui.Core 4.153.1
- Platforms: Windows (MAUI)
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/Platforms/Windows/WindowsSKGLViewHandler.cs` (`WindowsSKGLViewHandler`); `win/win32/xpl/GnollHackM/Platforms/Windows/WindowsSKCanvasViewHandler.cs` (`WindowsSKCanvasViewHandler`)
- Remove when: SkiaSharp's Windows touch handler reads only what each event needs (or can be replaced through a public API), GnollHack adopts that release, and Verify removal passes.
- Related: WA-WINUI-0003

**Symptom.** Pointer input on SkiaSharp views, hover moves above all, creates many CsWinRT
wrappers per event, each adding GC memory pressure.

**Cause.** SkiaSharp's `internal` Windows `SKTouchHandler` reads `Pointer`, the
`PointerPoint` and its `Properties` on every event, including every hover move, and the
handlers offer no way to substitute another touch handler.

**Workaround.** `WindowsSKGLViewHandler` and `WindowsSKCanvasViewHandler` derive from
SkiaSharp's handlers and remap `EnableTouchEvents` to GnollHack's own
`WindowsTouchHandler`, which reads only what each event needs (WA-WINUI-0003). Both
handlers are registered in `MauiProgram.CreateMauiApp`.

**Verify removal.** Remove the two handlers and their registrations. Then:
- Build for Windows.
- Move the mouse over the map and menus for a minute with the debug dashboard on, and
  confirm the allocation rate and gen2 collections match the custom handlers; confirm
  clicks, drags and touch still work.

Checks:
- 2026-10-23 | SkiaSharp.Views.Maui.Core 4.153.1 | still-needed | SKTouchHandler on SkiaSharp main is still internal and reads Pointer and Properties per event.

## WA-SKIASHARP-0008: SKGLView reports EGL errors while it is being resized

- Status: active
- Added: 2026-04-18 (from git history)
- GnollHack version: 2.124 (364)
- Component: SkiaSharp
- Package: SkiaSharp.Views.Maui.Core
- Defective version: SkiaSharp.Views.Maui.Core 3.119.1
- Platforms: Windows (MAUI)
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackX/GnollHackX/Pages/Game/GamePage.xaml.cs` (`GamePage.DoResizeCanvasUpdatePause`)
- Remove when: SkiaSharp's GL view draws without EGL errors during a resize in the release GnollHack uses, and Verify removal passes.

**Symptom.** Drawing to the game's canvases while they are being resized produces EGL
errors, especially on Windows with `SKGLView`.

**Cause.** Unknown. The commit names EGL errors during canvas resizing; on Windows
`SKGLView` renders through ANGLE in SkiaSharp's `SKSwapChainPanel`.

**Workaround.** Every size change sets a resizing flag, cleared 100 ms after the last
change and bounded by a deadline, and the canvas update loops skip invalidation while it
is set. The pause applies on every platform.

**Verify removal.** Make `DoResizeCanvasUpdatePause` a no-op. Then:
- Build for Windows.
- Drag-resize the window continuously during a game and a menu, and confirm no EGL
  errors in the debug output and no black or frozen canvases afterwards.

Checks:
- 2026-10-23 | SkiaSharp.Views.Maui.Core 4.153.1 | unverified | Not re-tested; no upstream report found.

## WA-SKIASHARP-0009: libHarfBuzzSharp binds its own calls to the system HarfBuzz

- Status: active
- Added: 2026-10-20 (from git history)
- GnollHack version: 3.28 (428)
- Component: SkiaSharp
- Package: HarfBuzzSharp.NativeAssets.Linux, SkiaSharp.NativeAssets.Linux
- Defective version: SkiaSharp.NativeAssets.Linux 4.153.1 (HarfBuzzSharp.NativeAssets.Linux version not recorded)
- Platforms: Linux
- Upstream issue: https://github.com/mono/SkiaSharp/issues/3038
- Upstream status when added: reported
- Upstream status: fixed-unreleased
- Fixed upstream in: https://github.com/mono/SkiaSharp/pull/5352
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/Platforms/Avalonia/AvaloniaDesktopProgram.cs` (`AvaloniaDesktopProgram.Main`); `win/win32/xpl/GnollHackM/Platforms/Avalonia/AvaloniaDesktopProgram.cs` (`AvaloniaDesktopProgram.PreloadHarfBuzzSharp`)
- Remove when: GnollHack uses a SkiaSharp and HarfBuzzSharp release containing PR 5352, and Verify removal passes.

**Symptom.** On Linux distributions with FreeType 2.14 or later, the game aborts at its
first start, inside HarfBuzz.

**Cause.** libHarfBuzzSharp calls its own exported `hb_*` functions through the PLT.
FreeType 2.14+ loads the system `libharfbuzz.so.0` with `RTLD_GLOBAL` when fontconfig
scans fonts, and a call bound lazily after that resolves to the system HarfBuzz, which
aborts on HarfBuzzSharp's objects.

**Workaround.** `Main` calls `PreloadHarfBuzzSharp` before fontconfig runs. It `dlopen`s
the bundled `libHarfBuzzSharp.so` with `RTLD_NOW`, which binds every call to the bundled
library first; HarfBuzzSharp's own load later gets the same handle.

**Verify removal.** Remove the preload call and method. Then:
- Publish the Linux build.
- Run it on a distribution with FreeType 2.14 or later and the system HarfBuzz installed,
  from a fresh account, and confirm the first start reaches the main menu and text
  renders.

Checks:
- 2026-10-23 | SkiaSharp.NativeAssets.Linux 4.153.1 | still-needed | Fix merged on main 2026-10-17; no release tag contains it (no backport found).

## WA-SKIASHARP-0010: Android touch cancel reaches only one pointer

- Status: active
- Added: 2026-09-10 (from git history)
- GnollHack version: 3.15 (415)
- Component: SkiaSharp
- Package: SkiaSharp.Views.Maui.Core
- Defective version: SkiaSharp.Views.Maui.Core 4.151.2
- Platforms: Android
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackX/GnollHackX/Pages/Game/GamePage.xaml.cs` (`SKTouchAction.Cancelled` case in the main canvas touch handler)
- Remove when: SkiaSharp's Android touch handler raises Cancelled for every pointer of a cancelled gesture, GnollHack adopts that release, and Verify removal passes.

**Symptom.** When Android cancels a multi-touch gesture, only one finger is reported as
cancelled. The others stay in GnollHack's touch dictionary as if still pressed, and the
next gesture is misread, for example as a pinch.

**Cause.** SkiaSharp's Android `SKTouchHandler` reads the pointer from `ActionIndex` for
every action except Move, so `ACTION_CANCEL`, which applies to all pointers, produces a
single `Cancelled` event.

**Workaround.** The first `Cancelled` event resets the whole main canvas touch state
(`ResetMainCanvasTouchState`) instead of removing just that pointer.

**Verify removal.** On a SkiaSharp release that claims the fix:
- Make the `Cancelled` case remove only `e.Id` from the touch dictionary.
- Build for Android, start a two-finger pinch on the map, and pull down the notification
  shade mid-gesture to cancel it.
- A following one-finger tap must act as a tap, not as a pinch.

Checks:
- 2026-10-23 | SkiaSharp.Views.Maui.Core 4.153.1 | still-needed | SKTouchHandler at v4.153.1 still uses ActionIndex for Cancel; one issue search found nothing.
