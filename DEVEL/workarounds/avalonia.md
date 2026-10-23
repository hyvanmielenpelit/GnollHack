# Avalonia Workarounds

- Component: Avalonia, Avalonia.Controls.Maui, Avalonia web view adapters
- Upstream: https://github.com/AvaloniaUI/Avalonia
- ID prefix: WA-AVALONIA

Entries follow the schema in [README.md](README.md). Append workarounds before the
`<!-- known-issues -->` line if there is one, otherwise at the end of the file, and known
issues at the end of the file. Never renumber or reuse an ID.

<!-- entries -->

## WA-AVALONIA-0001: ACM applies MAUI margins twice

- Status: active
- Added: 2026-10-18 (from git history)
- GnollHack version: 3.28 (428)
- Component: Avalonia.Controls.Maui
- Package: Avalonia.Controls.Maui
- Defective version: Avalonia.Controls.Maui 11.0.0-rc.2.26287.618
- Platforms: Linux, Windows (Avalonia)
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/Platforms/Avalonia/GHAvaloniaMargin.cs` (`GHAvaloniaMargin.Register`)
- Remove when: An Avalonia.Controls.Maui release no longer copies the MAUI margin to the Avalonia control, and Verify removal passes.

**Symptom.** A view with a horizontal margin is measured too narrow: its text wraps, or
the view vanishes when the margin grows with the window.

**Cause.** MAUI already subtracts the margin from the constraints and arranges the view
inside it. ACM also copies the margin to the Avalonia control, whose `Measure` subtracts
it a second time.

**Workaround.** The `Margin` entry of ACM's view mapper is replaced with one that keeps
the Avalonia container and platform view at a zero margin.

**Verify removal.** Remove the `GHAvaloniaMargin.Register` call from `MauiProgram`. Build
the Avalonia flavor, open pages whose labels have horizontal margins and resize the window;
text must not wrap early or disappear.

Checks:
- 2026-10-23 | Avalonia.Controls.Maui 11.0.0-rc.2.26287.618 | unverified | No upstream issue found.

## WA-AVALONIA-0002: ACM Skia handler mappers chain to MAUI's inert view mappers

- Status: active
- Added: 2026-10-18 (from git history)
- GnollHack version: 3.28 (428)
- Component: Avalonia.Controls.Maui
- Package: Avalonia.Controls.Maui.SkiaSharp.Views, Avalonia.Controls.Maui
- Defective version: Avalonia.Controls.Maui.SkiaSharp.Views 11.0.0-rc.2.26287.618
- Platforms: Linux, Windows (Avalonia)
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/Platforms/Avalonia/Rendering/GHAvaloniaCanvasViewHandler.cs` (`GHAvaloniaCanvasViewHandler.GHMapper`); `win/win32/xpl/GnollHackM/Platforms/Avalonia/Rendering/GHAvaloniaGLViewHandler.cs` (`GHAvaloniaGLViewHandler.GHMapper`); `win/win32/xpl/GnollHackM/Platforms/Avalonia/Cef/GHCefWebViewHandler.cs` (`GHCefWebViewHandler.Mapper`)
- Remove when: ACM's SKCanvasViewHandler and SKGLViewHandler mappers chain to ACM's own view mappers in a release GnollHack uses, and Verify removal passes.
- Related: WA-AVALONIA-0003

**Symptom.** Sizes, margins, visibility and the other view properties never reach a Skia
view's Avalonia control.

**Cause.** ACM's `SKCanvasViewHandler` and `SKGLViewHandler` mappers chain to MAUI's own
`ViewHandler` mappers, whose entries do nothing on Avalonia.

**Workaround.** GnollHack's Skia handlers, and its CEF web view handler, build their
mappers on ACM's `ViewHandler.ViewMapper` and `ViewCommandMapper` and list the Skia
entries they need explicitly.

**Verify removal.** Chain the two Skia handlers' mappers to ACM's own handler mappers.
Build the Avalonia flavor and check that the game map, the menus and CustomLabel views
size, hide and show as on WinUI.

Checks:
- 2026-10-23 | Avalonia.Controls.Maui.SkiaSharp.Views 11.0.0-rc.2.26287.618 | unverified | No upstream issue found.

## WA-AVALONIA-0003: Aligned Skia views collapse to zero size

- Status: active
- Added: 2026-10-18 (from git history)
- GnollHack version: 3.28 (428)
- Component: Avalonia.Controls.Maui
- Package: Avalonia.Controls.Maui.SkiaSharp.Views
- Defective version: Avalonia.Controls.Maui.SkiaSharp.Views 11.0.0-rc.2.26287.618
- Platforms: Linux, Windows (Avalonia)
- Upstream issue: https://github.com/AvaloniaUI/Avalonia.Controls.Maui/issues/201
- Upstream status when added: released
- Upstream status: released
- Fixed upstream in: https://github.com/AvaloniaUI/Avalonia.Controls.Maui/pull/230 (contained in 11.0.0-rc.2.26287.618)
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/Platforms/Avalonia/Rendering/GHAvaloniaCanvasViewHandler.cs` (`GHAvaloniaCanvasViewHandler.MapLayoutAlignment`); `win/win32/xpl/GnollHackM/Platforms/Avalonia/Rendering/GHAvaloniaGLViewHandler.cs` (`GHAvaloniaGLViewHandler.MapLayoutAlignment`)
- Remove when: ACM stops mapping MAUI layout options onto the alignment of Skia controls, or GnollHack's Skia controls stop measuring zero, and Verify removal passes.
- Related: WA-AVALONIA-0002

**Symptom.** A Skia view with a Start, Center or End option has zero width or height and
draws nothing.

**Cause.** ACM maps the MAUI layout options onto the Avalonia control's alignment,
although MAUI has already aligned the frame it arranges the control in. An aligned Avalonia
control takes its own desired size, which is zero for a view that MAUI sizes itself.
PR 230 fixed this inside ACM's `SKCanvasViewControl.MeasureOverride`, which now returns
the available size, while `ViewHandler` still maps the alignment. GnollHack's
`GHAvaloniaCanvasControl` and `GHAvaloniaGLControl` override `MeasureOverride` to return
zero, so the fix does not reach them.

**Workaround.** The horizontal and vertical alignment mapper entries of both Skia handlers
are no-ops, so the control stays stretched over MAUI's frame.

**Verify removal.** Remove the two `MapLayoutAlignment` entries. Build the Avalonia
flavor and check that Skia views with Start, Center or End options keep their size. GnollHack's controls override `MeasureOverride` to return zero, so ACM's fix in PR
230 may not reach them.

Checks:
- 2026-10-23 | Avalonia.Controls.Maui.SkiaSharp.Views 11.0.0-rc.2.26287.618 | unverified | Issue closed as completed by PR 230, which the shipped version contains; not re-tested with GnollHack's MeasureOverride.
- 2026-10-23 | Avalonia.Controls.Maui.SkiaSharp.Views 11.0.0-rc.2.26287.618 | still-needed | ACM's ViewHandler still maps layout alignment; PR 230's fix is in SKCanvasViewControl.MeasureOverride, which GHAvaloniaCanvasControl and GHAvaloniaGLControl override to return zero. Decompiled shipped package, not a run.

## WA-AVALONIA-0004: ACM ViewHandler.GetDesiredSize is not virtual

- Status: active
- Added: 2026-10-18 (from git history)
- GnollHack version: 3.28 (428)
- Component: Avalonia.Controls.Maui
- Package: Avalonia.Controls.Maui
- Defective version: Avalonia.Controls.Maui 11.0.0-rc.2.26287.618
- Platforms: Linux, Windows (Avalonia)
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackX/GnollHackX/CustomLabel.cs` (`AutoSizeSKCanvasViewHandler`)
- Remove when: ACM's `GetDesiredSize` can be overridden in a release GnollHack uses, and Verify removal passes.

**Symptom.** CustomLabel cannot report its own measured size through its handler.

**Cause.** ACM's `ViewHandler.GetDesiredSize` is not virtual, unlike MAUI's.

**Workaround.** The Avalonia `AutoSizeSKCanvasViewHandler` re-implements `IViewHandler`
and hides `GetDesiredSize` with `new`, so MAUI's interface call reaches CustomLabel's
measurement.

**Verify removal.** Replace the `new` method with an override, as in the non-Avalonia
handler. Build the Avalonia flavor and check that auto-sized labels (popups, menu
headers) have the same size as on WinUI.

Checks:
- 2026-10-23 | Avalonia.Controls.Maui 11.0.0-rc.2.26287.618 | unverified | No upstream issue found.

## WA-AVALONIA-0005: ACM does not apply InputTransparent to layouts

- Status: active
- Added: 2026-10-18 (from git history)
- GnollHack version: 3.28 (428)
- Component: Avalonia.Controls.Maui
- Package: Avalonia.Controls.Maui
- Defective version: Avalonia.Controls.Maui 11.0.0-rc.2.26287.618
- Platforms: Linux, Windows (Avalonia)
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/Platforms/Avalonia/GHAvaloniaLayoutInput.cs` (`GHAvaloniaLayoutInput.Register`)
- Remove when: ACM's layout handler honors `InputTransparent` and `CascadeInputTransparent` in a release GnollHack uses, and Verify removal passes.

**Symptom.** An input-transparent layout still catches pointer input, so the controls
under it cannot be clicked.

**Cause.** ACM's layout handler does not map `InputTransparent` to the Avalonia panel. An
Avalonia panel with any background brush, even a transparent one, is hit-testable.

**Workaround.** Appended `InputTransparent` and `Background` mappings turn hit testing
off for a cascading input-transparent layout, and clear the background of a
non-cascading one.

**Verify removal.** Remove the `GHAvaloniaLayoutInput.Register` call. Build the Avalonia
flavor and check that input-transparent layouts let clicks through to the map and buttons
below them.

Checks:
- 2026-10-23 | Avalonia.Controls.Maui 11.0.0-rc.2.26287.618 | unverified | No upstream issue found.

## WA-AVALONIA-0006: Scroll view content keeps its first alignment

- Status: active
- Added: 2026-10-19 (from git history)
- GnollHack version: 3.28 (428)
- Component: Avalonia.Controls.Maui
- Package: Avalonia.Controls.Maui
- Defective version: Avalonia.Controls.Maui 11.0.0-rc.2.26287.618
- Platforms: Linux, Windows (Avalonia)
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/Platforms/Avalonia/GHAvaloniaAlignment.cs` (`GHAvaloniaAlignment.MapScrollContent`)
- Remove when: ACM updates a scroll view content's alignment when its layout options change, in a release GnollHack uses, and Verify removal passes.

**Symptom.** Content of a scroll view whose layout options change after it is shown
(VaultPage) keeps its original alignment.

**Cause.** ACM sets the Avalonia alignment of a scroll view's content from the MAUI
options only when the handler is created, and MAUI does not remap them on change.

**Workaround.** The content's `VerticalOptions` and `HorizontalOptions` changes are
watched, and the matching alignment mapper entry is run again.

**Verify removal.** Remove the `MapScrollContent` mapping. Build the Avalonia flavor, open
VaultPage and change what it shows; the content must be aligned as on WinUI.

Checks:
- 2026-10-23 | Avalonia.Controls.Maui 11.0.0-rc.2.26287.618 | unverified | No upstream issue found.

## WA-AVALONIA-0007: Check box sits above the center of its label

- Status: active
- Added: 2026-10-19 (from git history)
- GnollHack version: 3.28 (428)
- Component: Avalonia.Controls.Maui
- Package: Avalonia.Controls.Maui
- Defective version: Avalonia.Controls.Maui 11.0.0-rc.2.26287.618
- Platforms: Linux, Windows (Avalonia)
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/Platforms/Avalonia/GHAvaloniaAlignment.cs` (`GHAvaloniaAlignment.MapCheckBoxTemplate`)
- Remove when: ACM's check box centers its box vertically in a release GnollHack uses, and Verify removal passes.

**Symptom.** A check box's box sits above the vertical center of the label next to it.

**Cause.** ACM's check box is MAUI's 44-unit minimum height, while the Fluent template
keeps the box at the top of a `CheckBoxMinHeight`-high area.

**Workaround.** When the template is applied, the box's indicator is centered
vertically.

**Verify removal.** Remove the `MapCheckBoxTemplate` mapping. Build the Avalonia flavor,
open SettingsPage and check that each box lines up with its label.

Checks:
- 2026-10-23 | Avalonia.Controls.Maui 11.0.0-rc.2.26287.618 | unverified | No upstream issue found.

## WA-AVALONIA-0008: ACM leaves some control colors to the Fluent theme

- Status: active
- Added: 2026-10-19 (from git history)
- GnollHack version: 3.28 (428)
- Component: Avalonia.Controls.Maui
- Package: Avalonia.Controls.Maui
- Defective version: Avalonia.Controls.Maui 11.0.0-rc.2.26287.618
- Platforms: Linux, Windows (Avalonia)
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/Platforms/Avalonia/GHAvaloniaControlTheming.cs` (`GHAvaloniaControlTheming.MapButtonDisabledTextColor`); `win/win32/xpl/GnollHackM/Platforms/Avalonia/GHAvaloniaControlTheming.cs` (`GHAvaloniaControlTheming.MapCaret`); `win/win32/xpl/GnollHackM/Platforms/Avalonia/GHAvaloniaControlTheming.cs` (`GHAvaloniaControlTheming.MapCheckBoxColor`)
- Remove when: ACM maps these colors as MAUI's WinUI handlers do, in a release GnollHack uses, and Verify removal passes.

**Symptom.** A disabled button's text is the Fluent theme's faint disabled color, the
text caret is black on GnollHack's transparent text boxes, and a check box's box ignores
its color.

**Cause.** ACM sets only the pointer-over and pressed button foregrounds, sets no
`CaretBrush` (Avalonia then draws the inverse of the background), and does not map the
check box color to the Fluent resources the template draws the box from.

**Workaround.** Appended mappings set the `ButtonForegroundDisabled` resource and the
caret brush from the text color, and the check box stroke and fill resources from its
color, as MAUI's WinUI handlers show them.

**Verify removal.** Remove the three mappings in `GHAvaloniaControlTheming.Register`.
Build the Avalonia flavor; disabled buttons, the caret in Entry and Editor fields, and
colored check boxes must look as on WinUI.

Checks:
- 2026-10-23 | Avalonia.Controls.Maui 11.0.0-rc.2.26287.618 | unverified | No upstream issue found.

## WA-AVALONIA-0009: ACM Skia controls raise no hover touch actions

- Status: active
- Added: 2026-10-18 (from git history)
- GnollHack version: 3.28 (428)
- Component: Avalonia.Controls.Maui
- Package: Avalonia.Controls.Maui.SkiaSharp.Views
- Defective version: Avalonia.Controls.Maui.SkiaSharp.Views 11.0.0-rc.2.26287.618
- Platforms: Linux, Windows (Avalonia)
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/Platforms/Avalonia/Rendering/GHAvaloniaPointerHover.cs` (`GHAvaloniaPointerHover`); `win/win32/xpl/GnollHackM/Platforms/Avalonia/Rendering/GHAvaloniaCanvasControl.cs` (`GHAvaloniaCanvasControl.OnPointerEntered`); `win/win32/xpl/GnollHackM/Platforms/Avalonia/Rendering/GHAvaloniaGLControl.cs` (`GHAvaloniaGLControl.OnPointerEntered`)
- Remove when: ACM's Skia controls raise Entered, Moved and Exited for a mouse without contact, in a release GnollHack uses, and Verify removal passes.

**Symptom.** Mouse hover over the map and canvas views does nothing: no hover
highlighting or mouse pointer tracking.

**Cause.** ACM's `SKCanvasViewControl` and `SKGLViewControl` raise touch actions only
while a button is pressed; SkiaSharp's views on WinUI also raise the no-contact Entered,
Moved and Exited actions.

**Workaround.** GnollHack's canvas and GL controls raise those actions for mouse pointers
without contact, with hover moves limited to 120 per second as on WinUI.

**Verify removal.** Remove the pointer overrides from both controls. Build the Avalonia
flavor and check that hovering the mouse over the map still updates the hover display.

Checks:
- 2026-10-23 | Avalonia.Controls.Maui.SkiaSharp.Views 11.0.0-rc.2.26287.618 | unverified | No upstream issue found.

## WA-AVALONIA-0010: ACM canvas view paints inside the render pass

- Status: active
- Added: 2026-10-18 (from git history)
- GnollHack version: 3.28 (428)
- Component: Avalonia.Controls.Maui
- Package: Avalonia.Controls.Maui.SkiaSharp.Views
- Defective version: Avalonia.Controls.Maui.SkiaSharp.Views 11.0.0-rc.2.26287.618
- Platforms: Linux, Windows (Avalonia)
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/Platforms/Avalonia/Rendering/GHAvaloniaCanvasControl.cs` (`GHAvaloniaCanvasControl`); `win/win32/xpl/GnollHackM/Platforms/Avalonia/Rendering/GHAvaloniaCanvasViewHandler.cs` (`GHAvaloniaCanvasViewHandler.CreatePlatformView`)
- Remove when: ACM's canvas view raises `PaintSurface` where a handler may change MAUI properties, as SkiaSharp's views do on other platforms, and Verify removal passes.

**Symptom.** GnollHack's raster views cannot paint safely through ACM's canvas control:
they read and write MAUI properties while painting.

**Cause.** ACM's `SKCanvasViewControl` paints as part of Avalonia's render pass, where
changes to the visual tree are not allowed.

**Workaround.** `GHAvaloniaCanvasControl` paints into its own raster surface on the UI
thread, in one coalesced dispatcher operation below layout priority, and `Render` only
draws the last painted image.

**Verify removal.** Make the canvas handler create ACM's own control. Build the Avalonia
flavor, play a game and open menus, popups and the message window; nothing may throw or
log a paint failure, and every canvas must update.

Checks:
- 2026-10-23 | Avalonia.Controls.Maui.SkiaSharp.Views 11.0.0-rc.2.26287.618 | unverified | No upstream issue found.

## WA-AVALONIA-0011: ACM font registrar misreads UTF-16 font names

- Status: active
- Added: 2026-10-18 (from git history)
- GnollHack version: 3.28 (428)
- Component: Avalonia.Controls.Maui
- Package: Avalonia.Controls.Maui
- Defective version: Avalonia.Controls.Maui 11.0.0-rc.2.26287.618
- Platforms: Linux, Windows (Avalonia)
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/Platforms/Avalonia/GHAvaloniaFontRegistrar.cs` (`GHAvaloniaFontRegistrar`); `win/win32/xpl/GnollHackM/MauiProgram.cs` (`MauiProgram.CreateMauiApp`)
- Remove when: ACM's font registrar decodes Unicode-platform names as UTF-16 in a release GnollHack uses, and Verify removal passes.

**Symptom.** Fonts whose names are stored for the Unicode platform, such as diablo_h.ttf
and archristy.ttf, do not resolve, and text falls back to another font.

**Cause.** ACM's own name table reader decodes UTF-16 names of the Unicode platform as
UTF-8.

**Workaround.** ACM's `IFontRegistrar` is replaced with `GHAvaloniaFontRegistrar`, which
loads each font into a font collection of its own and resolves it to the family name
Avalonia reads from the font.

**Verify removal.** Remove the `IFontRegistrar` replacement from `CreateMauiApp`. Build
the Avalonia flavor and check that text set in diablo_h.ttf and archristy.ttf shows in
those fonts. GnollHack's registrar also adjusts vertical metrics, which must move
elsewhere first.

Checks:
- 2026-10-23 | Avalonia.Controls.Maui 11.0.0-rc.2.26287.618 | unverified | No upstream issue found.

## WA-AVALONIA-0012: ACM converts MauiFont items after the resource step

- Status: active
- Added: 2026-10-18 (from git history)
- GnollHack version: 3.28 (428)
- Component: Avalonia.Controls.Maui
- Package: Avalonia.Controls.Maui
- Defective version: Avalonia.Controls.Maui 11.0.0-rc.2.26287.618
- Platforms: Linux, Windows (Avalonia)
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/GnollHackM.csproj` (`AvaloniaResource` fonts)
- Remove when: ACM's build converts `MauiFont` items before Avalonia's resource step in a release GnollHack uses, and Verify removal passes.

**Symptom.** Fonts given as `MauiFont` items are missing from the Avalonia build.

**Cause.** ACM converts `MauiFont` items to Avalonia resources only after the resource step
has run, so the resource blob never reaches the compiler.

**Workaround.** For the Avalonia flavor the fonts are `AvaloniaResource` items from the
start, at `Assets/Fonts/`, the path the font registrar reads.

**Verify removal.** Replace the `AvaloniaResource` font item with `MauiFont` for the
Avalonia flavor. Build it and check that every game font loads (the registrar logs a
failure per font).

Checks:
- 2026-10-23 | Avalonia.Controls.Maui 11.0.0-rc.2.26287.618 | unverified | No upstream issue found.

## WA-AVALONIA-0013: ACM Essentials leaves most Essentials APIs unimplemented

- Status: active
- Added: 2026-10-18 (from git history)
- GnollHack version: 3.28 (428)
- Component: Avalonia.Controls.Maui
- Package: Avalonia.Controls.Maui.Essentials
- Defective version: Avalonia.Controls.Maui.Essentials 11.0.0-rc.2.26287.618
- Platforms: Linux, Windows (Avalonia)
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/Platforms/Avalonia/Essentials/GHAvaloniaEssentials.cs` (`GHAvaloniaEssentials.InstallEarly`); `win/win32/xpl/GnollHackM/Platforms/Avalonia/Essentials/GHAvaloniaEssentials.cs` (`GHAvaloniaEssentials.InstallAfterBuilder`); `win/win32/xpl/GnollHackM/MauiProgram.cs` (`MauiProgram.CreateMauiApp`); `win/win32/xpl/GnollHackX/GnollHackX/GHApp.cs` (`GHApp.CheckAndRequestWritePermission`); `win/win32/xpl/GnollHackX/GnollHackX/GHApp.cs` (`GHApp.CheckAndRequestReadPermission`); `win/win32/xpl/GnollHackX/GnollHackX/Pages/MainScreen/ResetPage.xaml.cs` (`ResetPage.CheckAndRequestWritePermission`); `win/win32/xpl/GnollHackX/GnollHackX/Pages/MainScreen/ResetPage.xaml.cs` (`ResetPage.CheckAndRequestReadPermission`)
- Remove when: Avalonia.Controls.Maui.Essentials implements the APIs GnollHack uses in a release GnollHack uses, and Verify removal passes for each one.

**Symptom.** DeviceDisplay, DeviceInfo, Battery, Connectivity, SecureStorage, Browser,
Launcher, Share and Permissions throw `NotImplementedInReferenceAssemblyException`, the
portable MAUI Essentials behavior.

**Cause.** ACM Essentials implements only a few APIs (Preferences, FileSystem, FilePicker,
MediaPicker, HapticFeedback, Screenshot, WebAuthenticator). MAUI's facades have only
internal setters for another implementation.

**Workaround.** GnollHack's own implementations are installed into the facades with
`UnsafeAccessor` before GHApp's static initializers run, and Preferences and AppInfo
again after `UseAvaloniaEssentials` and `UseAvaloniaApp` replace them. Storage permission
checks return Granted without calling Permissions.

**Verify removal.** For each API ACM gains, drop GnollHack's installer line for it (or
the `#if GNH_AVALONIA` branch of a permission check). Build the Avalonia flavor, start
it, and use the features that call that API (sharing a file, opening a web link, the reset
page's file commands); nothing may throw.

Checks:
- 2026-10-23 | Avalonia.Controls.Maui.Essentials 11.0.0-rc.2.26287.618 | unverified | No upstream issue found.

## WA-AVALONIA-0014: Avalonia's render timer keeps a stale RandR rate

- Status: active
- Added: 2026-10-18 (from git history)
- GnollHack version: 3.28 (428)
- Component: Avalonia
- Package: Avalonia
- Defective version: Avalonia 12.1.4 (via Avalonia.Controls.Maui 11.0.0-rc.2.26287.618)
- Platforms: Linux
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/Platforms/Avalonia/GHAvaloniaFrameLoop.cs` (`GHAvaloniaFrameLoop.MaybeAdjustRenderTimer`); `win/win32/xpl/GnollHackM/Platforms/Avalonia/GHAvaloniaFrameLoop.cs` (`GHAvaloniaFrameLoop.LookUpRenderTimer`)
- Remove when: Avalonia's render timer follows the display's actual rate, or exposes a public rate setting, in a release GnollHack uses, and Verify removal passes.

**Symptom.** With the GLX swap interval applied, frames arrive below the display's refresh
rate.

**Cause.** Avalonia's `SleepLoopRenderTimer` runs at a rate taken from a stale RandR
value, and its `DesiredFps`, like `AvaloniaLocator` and `IRenderLoop`, is `[PrivateApi]`,
internal in the reference assemblies.

**Workaround.** Once a second while the swap interval is applied, `DesiredFps` is read by
reflection and raised to twice the estimated refresh rate when it is below 1.5 times the
estimate.

**Verify removal.** Make `MaybeAdjustRenderTimer` return at once. Build the Avalonia
flavor for Linux, play on a 120 Hz or faster monitor with debug logging on, and check
that the frame loop report's frame deltas match the monitor's rate.

Checks:
- 2026-10-23 | Avalonia 12.1.4 | unverified | No upstream issue found.

## WA-AVALONIA-0015: Avalonia's Screen reports no refresh rate

- Status: active
- Added: 2026-10-20 (from git history)
- GnollHack version: 3.28 (428)
- Component: Avalonia
- Package: Avalonia
- Defective version: Avalonia 12.1.4 (via Avalonia.Controls.Maui 11.0.0-rc.2.26287.618)
- Platforms: Windows (Avalonia)
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/Platforms/Avalonia/GHAvaloniaMonitorRefreshRate.cs` (`GHAvaloniaMonitorRefreshRate`); `win/win32/xpl/GnollHackM/Platforms/Avalonia/Essentials/GHAvaloniaDeviceDisplay.cs` (`GHAvaloniaDeviceDisplay.UpdateMonitorRefreshRate`)
- Remove when: Avalonia's `Screen` reports the display mode's refresh rate in a release GnollHack uses, and Verify removal passes.

**Symptom.** DeviceDisplay's refresh rate can only be estimated from frame deltas, and
does not settle on the monitor's exact rate.

**Cause.** Avalonia's `Screen` has no refresh rate.

**Workaround.** On Windows the rate of the window's monitor is read with
`MonitorFromWindow`, `GetMonitorInfoW` and `EnumDisplaySettingsW`, and the frame loop's
estimate snaps to it.

**Verify removal.** Replace the user32 reads with Avalonia's rate. Build the Avalonia
flavor for Windows, move the window between monitors of different rates, and check that
DeviceDisplay reports each monitor's rate.

Checks:
- 2026-10-23 | Avalonia 12.1.4 | unverified | No upstream issue found.

## WA-AVALONIA-0016: NativeControlHost ignores render transform changes

- Status: active
- Added: 2026-10-19 (from git history)
- GnollHack version: 3.28 (428)
- Component: Avalonia
- Package: Avalonia
- Defective version: Avalonia 12.1.4 (via Avalonia.Controls.Maui 11.0.0-rc.2.26287.618)
- Platforms: Linux, Windows (Avalonia)
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/Platforms/Avalonia/GHAvaloniaWebView.cs` (`GHAvaloniaWebView.MapPositionTracking`)
- Remove when: `NativeControlHost` moves its native control when an ancestor's render transform changes, in a release GnollHack uses, and Verify removal passes.

**Symptom.** A web page in a modal stays where the modal's slide-in animation was when the
page was placed, instead of moving with the modal.

**Cause.** `NativeControlHost` positions the native window including render transforms,
but a transform change alone does not move it. ACM slides modals in with a render
transform.

**Workaround.** A tracker watches the render transform of the web view and its
ancestors and calls `TryUpdateNativeControlPosition` on its hosts after each change.

**Verify removal.** Remove the `MapPositionTracking` mapping. Build the Avalonia flavor,
use the native web view (WebView2 on Windows, or WebKitGTK on Linux with CEF off), open
DisplayFilePage, and check that the page ends up inside the modal.

Checks:
- 2026-10-23 | Avalonia 12.1.4 | unverified | No upstream issue found.

## WA-AVALONIA-0017: IsEffectivelyVisible has no change event

- Status: active
- Added: 2026-10-20 (from git history)
- GnollHack version: 3.28 (428)
- Component: Avalonia
- Package: Avalonia
- Defective version: Avalonia 12.1.4 (via Avalonia.Controls.Maui 11.0.0-rc.2.26287.618)
- Platforms: Linux
- Upstream issue: https://github.com/AvaloniaUI/Avalonia/issues/15413
- Upstream status when added: reported
- Upstream status: reported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/Platforms/Avalonia/Cef/GHCefWebViewControl.cs` (`GHCefWebViewControl.SubscribeVisibility`)
- Remove when: `IsEffectivelyVisible` raises a change notification in an Avalonia release GnollHack uses, and Verify removal passes.

**Symptom.** Without notification, the CEF browser is not told when its control is hidden
or shown through an ancestor, and CEF does not resume painting it.

**Cause.** `IsEffectivelyVisible` is not an Avalonia property and has no public change
event.

**Workaround.** `IsVisible` is watched on the control and every ancestor while it is
attached, and the browser's hidden state is updated from `IsEffectivelyVisible` after each
change.

**Verify removal.** Replace the ancestor subscriptions with the new event. Build the
Avalonia flavor for Linux with CEF, hide and show a web view through an ancestor (leave
and return to a page that has one), and check that the page paints again.

Checks:
- 2026-10-23 | Avalonia 12.1.4 | still-needed | Issue open.

## WA-AVALONIA-0018: WebKitGTK adapter ignores a cancelled navigation

- Status: active
- Added: 2026-10-20 (from git history)
- GnollHack version: 3.28 (428)
- Component: Avalonia web view adapters
- Package: Avalonia.Controls.WebView
- Defective version: Avalonia.Controls.WebView 12.1.1
- Platforms: Linux
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/Platforms/Avalonia/GHAvaloniaOverseerHost.cs` (`NativeWebViewHost.IgnoresNavigationCancel`); `win/win32/xpl/GnollHackX/GnollHackX/Pages/Game/OverseerPage.xaml.cs` (`OverseerPage.DisplayWebView_Navigated`)
- Remove when: The WebKitGTK adapter honors a cancelled navigation in an Avalonia.Controls.WebView release GnollHack uses, and Verify removal passes.

**Symptom.** An off-site page that the Overseer page cancels and opens in the system
browser loads in the web view anyway.

**Cause.** Avalonia's WebKitGTK adapter does not honor a navigation cancelled in the
Navigating event.

**Workaround.** After a successful navigation to an off-site target, the Overseer page
goes back, or reloads the SPA when there is no history.

**Verify removal.** Make `IgnoresNavigationCancel` return false. Build the Avalonia flavor
for Linux with CEF off (WebKitGTK), open the Overseer and follow an external link; it must
open only in the system browser.

Checks:
- 2026-10-23 | Avalonia.Controls.WebView 12.1.1 | unverified | No upstream issue found.
