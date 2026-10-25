# WinUI Workarounds

- Component: WinUI 3, Windows App SDK, CsWinRT
- Upstream: https://github.com/microsoft/microsoft-ui-xaml, https://github.com/microsoft/WindowsAppSDK, https://github.com/microsoft/CsWinRT
- ID prefix: WA-WINUI

Entries follow the schema in [README.md](README.md). Append workarounds before the
`<!-- known-issues -->` line if there is one, otherwise at the end of the file, and known
issues at the end of the file. Never renumber or reuse an ID.

<!-- entries -->

## WA-WINUI-0001: Rendering capped at 32 FPS by the Sleep(16) frame-pacing fallback

- Status: active
- Added: 2026-09-28 (from git history)
- GnollHack version: 3.21 (421)
- Component: WinUI 3
- Package: Microsoft.WindowsAppSDK.WinUI
- Defective version: Microsoft.WindowsAppSDK.WinUI 1.8.260528001 (Windows App SDK 1.8.260529003 via Microsoft.Maui.Controls 10.0.110)
- Platforms: Windows (MAUI)
- Upstream issue: https://github.com/microsoft/microsoft-ui-xaml/issues/12073
- Upstream status when added: unreported
- Upstream status: reported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackX/GnollHackX/GHApp.cs` (`GHApp.SetProcessPriority`)
- Remove when: GnollHack uses a Windows App SDK release that fixes microsoft-ui-xaml#12073, and Verify removal passes.
- Related: WA-WINUI-0002

**Symptom.** Where `DCompositionWaitForCompositorClock` is unavailable (every Windows 10
system) or fails, `CompositionTarget.Rendering` ticks about every 31 ms, so the game runs
at about 32 FPS on a 60 Hz display.

**Cause.** WinUI's frame scheduler falls back to `Sleep(16)` when the compositor clock
wait cannot be used. At the default 15.6 ms system timer resolution the sleep wakes on the
second timer tick. An earlier report of the symptom, microsoft-ui-xaml#10092, was closed
without a root cause. The bug is hidden whenever something in the process requests a finer
timer resolution, such as the .NET EventPipe sample profiler.

**Workaround.** `SetProcessPriority`, called at startup, calls `timeBeginPeriod(1)` from
`winmm.dll`, which sets 1 ms timer resolution for the process.

**Verify removal.** Remove the `timeBeginPeriod(1)` call. Then:
- Build the Windows Release configuration.
- Run it on Windows 10 at 60 Hz, without a profiler or the debugger's diagnostic tools.
- Confirm that the main menu and the game render at about 60 FPS, not 32.

Checks:
- 2026-10-23 | Microsoft.WindowsAppSDK.WinUI 1.8.260528001 | still-needed | microsoft-ui-xaml#12073 is open.

## WA-WINUI-0002: CompositionTarget.Rendering allocates event-args wrappers every frame

- Status: active
- Added: 2026-09-27 (from git history)
- GnollHack version: 3.21 (421)
- Component: WinUI 3, CsWinRT
- Package: Microsoft.WindowsAppSDK.WinUI, Microsoft.Windows.SDK.NET.Ref
- Defective version: Microsoft.WindowsAppSDK.WinUI 1.8.260528001 (Windows App SDK 1.8.260529003 via Microsoft.Maui.Controls 10.0.110), WinRT.Runtime 2.2.0.48161
- Platforms: Windows (MAUI)
- Upstream issue: https://github.com/microsoft/microsoft-ui-xaml/issues/12112, https://github.com/microsoft/CsWinRT/issues/2594
- Upstream status when added: unreported
- Upstream status: reported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/Platforms/Windows/RenderingSubscriptionWindows.cs` (`RenderingSubscriptionWindows`); `win/win32/xpl/GnollHackX/GnollHackX/GHApp.cs` (`GHApp.InitializePlatformRenderLoop`); `win/win32/xpl/GnollHackX/GnollHackX/GHApp.cs` (`GHApp.ScheduleRawRenderingLivenessCheck`)
- Remove when: GnollHack uses a Windows App SDK release that resolves microsoft-ui-xaml#12112, or a CsWinRT release that resolves CsWinRT#2594, and Verify removal passes.
- Related: WA-WINUI-0001, WA-WINUI-0003, KI-WINUI-0001

**Symptom.** With the managed `CompositionTarget.Rendering +=` subscription, every frame
allocates managed wrappers for the event arguments, and the render loop alone causes
periodic gen2 collections that show as frame hitches.

**Cause.** WinUI creates a new `RenderingEventArgs` and framework peer for every tick
(microsoft-ui-xaml#12112). CsWinRT's `EventHandler<object>` invoker wraps the arguments
whether or not the handler reads them, and each `IObjectReference` adds 1000 bytes of GC
memory pressure until it is finalized (CsWinRT#2594).

**Workaround.** `RenderingSubscriptionWindows` calls `add_Rendering` on the
`CompositionTarget` statics through the raw ABI with a static, hand-built delegate, so no
managed wrapper is created. `RenderingTime` is read through `QueryInterface` only when
needed. A handler failure is returned as `E_FAIL`, because `RPC_E_DISCONNECTED` and
`RPC_E_SERVER_UNAVAILABLE` make XAML drop the handler silently. `InitializePlatformRenderLoop`
falls back to the managed event if the raw subscription fails, and
`ScheduleRawRenderingLivenessCheck` switches to it if no raw callback arrives while the app
is active. The code is compiled under `ENABLE_RAW_RENDERING`, defined in `GnollHackM.csproj`.

**Verify removal.** Remove `ENABLE_RAW_RENDERING` from `DefineConstants` in
`GnollHackM.csproj`, so that the managed subscription is used. Then:
- Build the Windows Release configuration.
- Play or run the performance suite for several minutes with the frame-time profiler on.
- Confirm that the render loop causes no induced gen2 collections and no per-frame
  allocation on the UI thread, as with the raw subscription.

Checks:
- 2026-10-23 | Microsoft.WindowsAppSDK.WinUI 1.8.260528001, Microsoft.Windows.SDK.NET.Ref 10.0.19041.57 (WinRT.Runtime 2.2.0.48161) | still-needed | microsoft-ui-xaml#12112 and CsWinRT#2594 are open.

## WA-WINUI-0003: CsWinRT wrappers on pointer events add GC memory pressure

- Status: active
- Added: 2026-10-04 (from git history)
- GnollHack version: 3.22 (422)
- Component: CsWinRT
- Package: Microsoft.Windows.SDK.NET.Ref
- Defective version: WinRT.Runtime 2.2.0.48161 (Microsoft.Windows.SDK.NET.Ref)
- Platforms: Windows (MAUI)
- Upstream issue: https://github.com/microsoft/CsWinRT/issues/2594
- Upstream status when added: reported
- Upstream status: reported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/Platforms/Windows/WindowsTouchHandler.cs` (`WindowsTouchHandler`)
- Remove when: GnollHack builds with a CsWinRT release, shipped in Microsoft.Windows.SDK.NET.Ref, that no longer charges memory pressure for short-lived wrappers (CsWinRT#2594), and Verify removal passes.
- Related: WA-WINUI-0002, WA-WINUI-0004, WA-SKIASHARP-0007

**Symptom.** SkiaSharp's own Windows touch handler reads several WinRT objects for every
pointer event. At mouse-move rates the wrappers it creates induce gen2 collections.

**Cause.** Every CsWinRT `IObjectReference` adds 1000 bytes of GC memory pressure, which is
removed only when the wrapper is disposed or finalized (CsWinRT#2594). The issue reports
the same constant in CsWinRT 3.0.

**Workaround.** `WindowsTouchHandler`, adapted from SkiaSharp's internal `SKTouchHandler`,
reads one `PointerPoint` per event, `Properties` only when a button can be involved, and
`Pointer` only to capture it and when the capture ends. Hover moves are limited to 120 Hz
before anything is read, and wheel events are not handled. It is plugged into SkiaSharp's
views by the handler subclasses recorded in WA-SKIASHARP-0007.

**Verify removal.** Make `WindowsTouchHandler` read `Pointer`, `PointerPoint` and
`Properties` on every event, as SkiaSharp's own handler does. Then:
- Build the Windows Release configuration.
- Move the mouse continuously over the map and the menus for several minutes with the
  frame-time profiler on.
- Confirm that the induced gen2 collection rate is no higher than with the custom handlers.

Checks:
- 2026-10-23 | Microsoft.Windows.SDK.NET.Ref 10.0.19041.57 (WinRT.Runtime 2.2.0.48161) | still-needed | CsWinRT#2594 is open.

## WA-WINUI-0004: Mouse capture loss raises no PointerCaptureLost or PointerCanceled

- Status: active
- Added: 2026-10-05 (from git history)
- GnollHack version: 3.23 (423)
- Component: WinUI 3
- Package: Microsoft.WindowsAppSDK.WinUI
- Defective version: Microsoft.WindowsAppSDK.WinUI 1.8.260528001 (Windows App SDK 1.8.260529003 via Microsoft.Maui.Controls 10.0.110)
- Platforms: Windows (MAUI)
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackX/GnollHackX/Pages/Game/GamePage.xaml.cs` (`GamePage.ClearCanvasTouchState`)
- Remove when: GnollHack uses a Windows App SDK release that raises `PointerCaptureLost` or `PointerCanceled` when a mouse loses capture, and Verify removal passes.
- Related: WA-WINUI-0003

**Symptom.** When a mouse button is held on a game canvas and the system takes the
capture, for example with Alt+Tab or the Windows key, the game is never told that the
press ended. A pending press, such as hold-to-move or a long tap, stays alive.

**Cause.** WinUI maps a mouse capture loss to `WM_CAPTURECHANGED`, and
`CJupiterControl::HandlePointerMessage` returns early for that message, so XAML raises
neither `PointerCaptureLost` nor `PointerCanceled`. This is in the source of
`winui3/release/1.8-stable` and of the main branch.

**Workaround.** Window deactivation calls `GHApp.OnUnfocus`, which calls
`GamePage.ClearCanvasTouchState`. It clears the touch dictionaries of all four canvases and
also their saved press, so the hold-to-move and long-tap timers stop, together with the
flags that track which button a touch began on. No cancel is synthesized when a pointer
goes from pressed to not pressed. The same reset also runs on suspend and resume and on
iOS resign-active, so only the pending-press part is specific to this defect.

**Verify removal.** With a Windows App SDK release that claims the fix:
- Build the Windows configuration with a temporary log line in
  `WindowsTouchHandler.OnPointerCaptureLost` and `OnPointerCanceled`.
- In a game, hold the left mouse button on the map to move, press Alt+Tab while it is
  held, release it outside the window, and return.
- Confirm that one of the two events is logged and that the hero stops. The reset in
  `ClearCanvasTouchState` can then stay as a general safeguard; remove only its tag and set
  the entry to removed.

Checks:
- 2026-10-23 | Microsoft.WindowsAppSDK.WinUI 1.8.260528001 | still-needed | The early return for WM_CAPTURECHANGED is still in the main branch source; no upstream report.

## WA-WINUI-0005: No public API to set the cursor of an arbitrary UIElement

- Status: active
- Added: 2024-06-23 (from git history)
- GnollHack version: 1.0 (1)
- Component: WinUI 3
- Package: Microsoft.WindowsAppSDK.WinUI
- Defective version: unknown (Windows App SDK via the MAUI workload's Microsoft.Maui.Controls)
- Platforms: Windows (MAUI)
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/Platforms/Windows/CursorUtilities.cs` (`CursorUtilities.ChangeCursor`)
- Remove when: WinUI offers a public way to set the cursor of any `UIElement` in a Windows App SDK release GnollHack uses, and Verify removal passes.

**Symptom.** GnollHack's custom mouse cursors cannot be applied to the platform views of
stock MAUI controls on Windows.

**Cause.** WinUI 3 exposes the cursor only as `UIElement.ProtectedCursor`, which is
`protected`, so it can be set only from a class derived from the element's own type.

**Workaround.** `CursorUtilities.ChangeCursor` sets `ProtectedCursor` through reflection
(`InvokeMember` with `BindingFlags.NonPublic`). The cursor itself is loaded with
`LoadCursorFromFileW` and wrapped through `IInputCursorStaticsInterop.CreateFromHCursor`.

**Verify removal.** Replace the reflection call with the public API. Then:
- Build the Windows configuration.
- Confirm that the custom cursor shows on the main page, the game page and the menus.

Checks:
- 2026-10-23 | Microsoft.WindowsAppSDK.WinUI 1.8.260528001 | still-needed | ProtectedCursor is still declared protected in the main branch source; no upstream report.

## WA-WINUI-0006: TextBox reports line breaks as a lone CR

- Status: active
- Added: 2025-03-25 (from git history)
- GnollHack version: 2.20 (220)
- Component: WinUI 3
- Package: Microsoft.WindowsAppSDK.WinUI
- Defective version: unknown (Windows App SDK via Microsoft.Maui.Controls 9.0.50)
- Platforms: Windows (MAUI)
- Upstream issue: https://github.com/microsoft/microsoft-ui-xaml/issues/1826
- Upstream status when added: reported
- Upstream status: reported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackX/GnollHackX/Pages/MainScreen/EditorPage.xaml.cs` (`EditorPage.OKButton_Clicked`)
- Remove when: GnollHack uses a Windows App SDK release that fixes microsoft-ui-xaml#1826, and Verify removal passes.

**Symptom.** In the options file editor on Windows, the MAUI `Editor` returns every line
break as a carriage return alone. Saving the text unchanged would turn the file's CRLF
line endings into CR only.

**Cause.** The WinUI `TextBox` behind the `Editor` normalizes line breaks to a carriage
return (microsoft-ui-xaml#1826, open since 2020).

**Workaround.** On Windows, `EditorPage.OKButton_Clicked` replaces each carriage return
with `Environment.NewLine` before the file is written.

**Verify removal.** Remove the Windows branch, so that the text is written as the `Editor`
returns it. Then:
- Build the Windows configuration.
- Open the default options file from the main menu, add a line, and save.
- Confirm that every line of the saved file ends in CRLF and that the game reads the
  options.

Checks:
- 2026-10-23 | Microsoft.WindowsAppSDK.WinUI 1.8.260528001 | still-needed | microsoft-ui-xaml#1826 is open.

## WA-WINUI-0007: WriteableBitmap updates pile up shared surfaces while the display is off

- Status: active
- Added: 2026-10-20 (from git history)
- GnollHack version: 3.28 (428)
- Component: WinUI 3
- Package: Microsoft.WindowsAppSDK.WinUI
- Defective version: Microsoft.WindowsAppSDK.WinUI 1.8.260528001 (Windows App SDK 1.8.260529003 via Microsoft.Maui.Controls 10.0.110); the upstream report reproduces it on Windows App SDK 1.8 and 2.5
- Platforms: Windows (MAUI)
- Upstream issue: https://github.com/microsoft/microsoft-ui-xaml/issues/12355
- Upstream status when added: unreported
- Upstream status: reported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/Platforms/Windows/MainScreenPauseWindows.cs` (`MainScreenPauseWindows`); `win/win32/xpl/GnollHackX/GnollHackX/GHConstants.cs` (`GHConstants.PauseMainScreenWhenUnseen`); `win/win32/xpl/GnollHackX/GnollHackX/GHConstants.cs` (`GHConstants.PauseGameRenderingWhenUnseen`); `win/win32/xpl/GnollHackX/GnollHackX/GHApp.cs` (`GHApp.CompositionTarget_Rendering`)
- Remove when: GnollHack uses a Windows App SDK release that fixes microsoft-ui-xaml#12355, and Verify removal passes.
- Related: GNOLLHACK-E9, GNOLLHACK-HC, GNOLLHACK-HW

**Symptom.** While the display is off, the main menu's animated image carousel keeps
updating its canvas, and the process accumulates window-sized shared surfaces, about
100-170 MB of system commit. On one player's desktop the handle count kept growing for
hours on the main menu until the app failed with E_OUTOFMEMORY. A game in progress does the
same when the map is drawn on the CPU-rendered canvas (the GPU setting off): it keeps
painting the map while the display is off. On one player's AMD Radeon 780M laptop the
surfaces never leveled off, about 32 a minute, and the system commit was exhausted in
about two hours.

**Cause.** A `WriteableBitmap` updated while the display is off is uploaded through
DirectComposition surfaces that are retired only when composition resumes. The carousel
draws on SkiaSharp's `SKXamlCanvas`, which is a `WriteableBitmap`. The upstream report
reproduces this with a stock WinUI 3 app, and does not settle whether WinUI,
DirectComposition or the GPU driver owns it.

**Workaround.** `MainScreenPauseWindows` suspends the main page, including the carousel and
its timer, while the display is off or the session is locked, and resumes it when both
clear. `GHConstants.PauseMainScreenWhenUnseen` switches it on. Game page rendering stops
while the display is off: `GHApp.CompositionTarget_Rendering` checks the display state on
every frame and skips the game page, and `MainScreenPauseWindows` suspends the game page's
own animations for the case where the platform render loop is off.
`GHConstants.PauseGameRenderingWhenUnseen` switches it on. The game pause does not apply
to a locked session with the display on, because the lock is polled once a minute.

**Verify removal.** Set `GHConstants.PauseMainScreenWhenUnseen` to `false`. Then:
- Build the Windows Release configuration.
- Leave the app on the main menu with the carousel running, and turn the display off for
  at least ten minutes.
- Confirm that the process handle count and commit stay flat while the display is off.

Then set `GHConstants.PauseGameRenderingWhenUnseen` to `false`, and:
- Build the Windows Release configuration.
- Turn the GPU setting off, start a game, and turn the display off for at least ten
  minutes.
- Confirm that the process handle count and commit stay flat while the display is off.

Checks:
- 2026-10-23 | Microsoft.WindowsAppSDK.WinUI 1.8.260528001 | still-needed | microsoft-ui-xaml#12355 is open.

## WA-WINUI-0008: Swap-chain canvases cannot be transparent or faded

- Status: active
- Added: 2024-08-06 (from git history)
- GnollHack version: 2.5 (205)
- Component: WinUI 3
- Package: Microsoft.WindowsAppSDK.WinUI
- Defective version: unknown
- Platforms: Windows (MAUI)
- Upstream issue: none
- Upstream status when added: unknown
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackX/GnollHackX/Pages/Game/GamePage.xaml.cs` (`GamePage.UseAuxiliaryGLCanvas`); `win/win32/xpl/GnollHackX/GnollHackX/Pages/Game/GamePage.xaml.cs` (`GamePage.FadeWithFadeFrame`)
- Remove when: A WinUI release composites SwapChainPanel content with transparency and element opacity, GnollHack adopts it, and Verify removal passes.

**Symptom.** On Windows, a GL canvas (SkiaSharp's SKGLView, backed by a SwapChainPanel)
paints an opaque background over whatever is behind it, and fading it by changing its
opacity has no visible effect.

**Cause.** WinUI 3 composites SwapChainPanel content without transparency, and the swap
chain does not follow the element's opacity.

**Workaround.** On Windows only the command canvas uses GL; the menu and text canvases,
which are drawn over the map, stay on the CPU-rendered SKCanvasView
(`UseAuxiliaryGLCanvas`). The map is faded with an overlay fade frame instead of its own
opacity (`FadeWithFadeFrame`), which Android's SurfaceView mode also uses, because a
SurfaceView layer ignores view opacity by design.

**Verify removal.** On a WinUI release that claims the fix:
- Let `UseAuxiliaryGLCanvas` set `UseGL` on the menu and text canvases on Windows too, and
  make `FadeWithFadeFrame` return the SurfaceView value on Windows.
- Build for Windows, open a menu and a text window over the map, and start and leave a
  game.
- The map must show through the menu and text windows, and the fades must animate.

Checks:
- 2026-10-23 | Microsoft.WindowsAppSDK.WinUI 1.8.260528001 | unverified | Not re-tested; one issue search found nothing.

<!-- known-issues -->

## KI-WINUI-0001: TrackMemory forces blocking gen2 collections on the UI thread

- Status: open
- Added: 2026-10-23
- GnollHack version: unknown
- Component: WinUI 3
- Package: Microsoft.WindowsAppSDK.WinUI
- Defective version: Microsoft.WindowsAppSDK.WinUI 1.8.260528001
- Platforms: Windows (MAUI)
- Impact: performance
- Sentry: none
- Upstream issue: https://github.com/microsoft/microsoft-ui-xaml/issues/12113
- Upstream status when added: reported
- Upstream status: reported
- Last checked: 2026-10-23
- Resolve when: WinUI stops these collections or lets an app defer or opt out of them, GnollHack uses that release, and Verify fix passes.
- Related: WA-WINUI-0002

**Symptom.** On Windows, the UI thread blocks in an induced gen2 garbage collection
whenever the working set rises more than 50 MiB above WinUI's baseline, including once
shortly after startup when the working set is already above 100 MiB. Each one shows as a
frame hitch.

**Cause.** Every 51st rendered frame, WinUI's `CCoreServices::TrackMemory` reads the working
set, and if it is more than `MAX_ALLOWEDMEMORY_INCREASE` (52,428,800 bytes) above a baseline
that starts at 50 MiB, it triggers a garbage collection on the UI thread. The baseline moves
only when such a collection runs or a sample is lower, so a collection the app makes itself
does not defer it. The issue also names `TriggerCollectionForOrphanedObjects`. Measured in a
blank WinUI app (Windows App SDK 1.8.260529003, Windows 11 build 26200) that grows its
working set in 64 MiB steps: one blocking induced gen2 (reason 1) per step. This is a
different gen2 source from WA-WINUI-0002, where per-frame wrapper allocations cause the
collections.

**Why no workaround.** The collection is requested from WinUI's native code, with no public
setting. Its trigger is working-set growth, which includes memory that GnollHack does not
allocate, such as graphics driver and WinUI allocations, so the app cannot keep below the
threshold.

**Verify fix.** In a blank WinUI app on the new release, and in GnollHack's Windows build
with the frame-time profiler on, grow the working set in 64 MiB steps several times and
confirm that no induced gen2 collection occurs on the UI thread.

Checks:
- 2026-10-23 | Microsoft.WindowsAppSDK.WinUI 1.8.260528001 | still-present | microsoft-ui-xaml#12113 open (feature proposal, needs-triage); behavior measured on Windows App SDK 1.8.260529003.
