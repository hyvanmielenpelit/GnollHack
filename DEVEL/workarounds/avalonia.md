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
- Last checked: 2026-10-27
- Code: `win/win32/xpl/GnollHackM/Platforms/Avalonia/GHAvaloniaControlTheming.cs` (`GHAvaloniaControlTheming.MapButtonDisabledTextColor`); `win/win32/xpl/GnollHackM/Platforms/Avalonia/GHAvaloniaControlTheming.cs` (`GHAvaloniaControlTheming.MapButtonDisabledBackground`); `win/win32/xpl/GnollHackM/Platforms/Avalonia/GHAvaloniaControlTheming.cs` (`GHAvaloniaControlTheming.MapCaret`); `win/win32/xpl/GnollHackM/Platforms/Avalonia/GHAvaloniaControlTheming.cs` (`GHAvaloniaControlTheming.MapCheckBoxColor`)
- Remove when: ACM maps these colors as MAUI's WinUI handlers do, in a release GnollHack uses, and Verify removal passes.

**Symptom.** A disabled button's text is the Fluent theme's faint disabled color, a
disabled transparent button, such as the one inside every `CustomImageButton`, is
covered by a translucent gray (dark theme) or black (light theme) layer, the text caret
is black on GnollHack's transparent text boxes, and a check box's box ignores its color.
The gray layer is most visible on the Steam Deck, whose dark theme Avalonia follows, for
example over the main menu's upper buttons when Exit disables them.

**Cause.** ACM sets only the pointer-over and pressed button foregrounds and
backgrounds, so the Fluent `:disabled` style draws `ButtonBackgroundDisabled`
(`SystemBaseLowColor`, 20 % white in the dark theme and 20 % black in the light theme)
instead of the button's background. ACM also sets no `CaretBrush` (Avalonia then draws
the inverse of the background), and does not map the check box color to the Fluent
resources the template draws the box from.

**Workaround.** Appended mappings set the `ButtonForegroundDisabled` resource and the
caret brush from the text color, the `ButtonBackgroundDisabled` resource from the
button's background, and the check box stroke and fill resources from its color, as
MAUI's WinUI handlers show them.

**Verify removal.** Remove the four mappings in `GHAvaloniaControlTheming.Register`.
Build the Avalonia flavor and run it in the dark theme (the Steam Deck, or Windows in
dark mode); disabled buttons, including the main menu's upper buttons after pressing
Exit, the caret in Entry and Editor fields, and colored check boxes must look as on
WinUI.

Checks:
- 2026-10-23 | Avalonia.Controls.Maui 11.0.0-rc.2.26287.618 | unverified | No upstream issue found.
- 2026-10-27 | Avalonia.Controls.Maui 11.0.0-rc.2.26287.618 | still-needed | Disabled button background seen on the Steam Deck; ACM's ButtonExtensions sets no ButtonBackgroundDisabled. Background mapping added.

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
- Last checked: 2026-10-27
- Code: `win/win32/xpl/GnollHackM/Platforms/Avalonia/Rendering/GHAvaloniaCanvasControl.cs` (`GHAvaloniaCanvasControl`); `win/win32/xpl/GnollHackM/Platforms/Avalonia/Rendering/GHAvaloniaCanvasViewHandler.cs` (`GHAvaloniaCanvasViewHandler.CreatePlatformView`)
- Remove when: ACM's canvas view raises `PaintSurface` where a handler may change MAUI properties, as SkiaSharp's views do on other platforms, and Verify removal passes.
- Related: WA-AVALONIA-0021

**Symptom.** GnollHack's raster views cannot paint safely through ACM's canvas control:
they read and write MAUI properties while painting.

**Cause.** ACM's `SKCanvasViewControl` paints as part of Avalonia's render pass, where
changes to the visual tree are not allowed.

**Workaround.** `GHAvaloniaCanvasControl` paints into its own raster surface on the UI
thread, in one coalesced dispatcher operation below layout priority, and `Render` only
draws the last painted image. A request made inside the platform frame callback paints at
once instead, which is also outside the render pass (WA-AVALONIA-0021).

**Verify removal.** Make the canvas handler create ACM's own control. Build the Avalonia
flavor, play a game and open menus, popups and the message window; nothing may throw or
log a paint failure, and every canvas must update.

Checks:
- 2026-10-23 | Avalonia.Controls.Maui.SkiaSharp.Views 11.0.0-rc.2.26287.618 | unverified | No upstream issue found.
- 2026-10-27 | Avalonia.Controls.Maui.SkiaSharp.Views 11.0.0-rc.2.26287.618 | unverified | Requests inside the platform frame callback paint at once, for WA-AVALONIA-0021.

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
again after `UseAvaloniaEssentials` and `UseAvaloniaApp` replace them. ACM's FilePicker is
wrapped by `GHAvaloniaFilePicker` at that point too, for `WA-STEAM-0002`. Storage permission
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
- Platforms: Linux, Windows (Avalonia)
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-27
- Code: `win/win32/xpl/GnollHackM/Platforms/Avalonia/GHAvaloniaMonitorRefreshRate.cs` (`GHAvaloniaMonitorRefreshRate`); `win/win32/xpl/GnollHackM/Platforms/Avalonia/Essentials/GHAvaloniaDeviceDisplay.cs` (`GHAvaloniaDeviceDisplay.UpdateMonitorRefreshRate`); `win/win32/xpl/GnollHackM/Platforms/Avalonia/GHRandr.cs` (`GHRandr`)
- Remove when: Avalonia's `Screen` reports the display mode's refresh rate in a release GnollHack uses, and Verify removal passes.

**Symptom.** DeviceDisplay's refresh rate can only be estimated from frame deltas, and
does not settle on the monitor's exact rate. On Linux the estimate also follows any
irregular frame timing, since it has nothing to snap to.

**Cause.** Avalonia's `Screen` has no refresh rate. Avalonia's X11 backend reads the RandR
rates internally, for its render timer, but does not expose them.

**Workaround.** On Windows the rate of the window's monitor is read with
`MonitorFromWindow`, `GetMonitorInfoW` and `EnumDisplaySettingsW`; on Linux, `GHRandr`
reads the mode of the X11 RandR CRTC under the window's screen on its own display
connection. The frame loop's estimate snaps to that rate.

**Verify removal.** Replace the user32 and RandR reads with Avalonia's rate. Build the
Avalonia flavor for Windows, move the window between monitors of different rates, and check
that DeviceDisplay reports each monitor's rate. Build the Linux flavor, start it with debug
log messages on, and check that the log shows the monitor rate read from RandR.

Checks:
- 2026-10-23 | Avalonia 12.1.4 | unverified | No upstream issue found.
- 2026-10-27 | Avalonia 12.1.4 | unverified | Extended to Linux through X11 RandR.

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

## WA-AVALONIA-0019: ACM Skia controls drop touches without Released or Cancelled

- Status: active
- Added: 2026-10-27
- GnollHack version: 3.32 (432)
- Component: Avalonia.Controls.Maui
- Package: Avalonia.Controls.Maui.SkiaSharp.Views
- Defective version: Avalonia.Controls.Maui.SkiaSharp.Views 11.0.0-rc.2.26287.618
- Platforms: Linux, Windows (Avalonia)
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-27
- Code: `win/win32/xpl/GnollHackM/Platforms/Avalonia/Rendering/GHAvaloniaTouchTracker.cs` (`GHAvaloniaTouchTracker`); `win/win32/xpl/GnollHackM/Platforms/Avalonia/Rendering/GHAvaloniaCanvasControl.cs` (`GHAvaloniaCanvasControl.OnPointerPressed`); `win/win32/xpl/GnollHackM/Platforms/Avalonia/Rendering/GHAvaloniaGLControl.cs` (`GHAvaloniaGLControl.OnPointerPressed`)
- Remove when: ACM's Skia controls raise Released or Cancelled for every touch they raised Pressed for, in a release GnollHack uses, and Verify removal passes.
- Related: WA-AVALONIA-0009, WA-WINUI-0004, WA-SKIASHARP-0010

**Symptom.** After some multi-finger touches, on Steam Deck for example, a finger stays
"stuck" on the main canvas: nothing can be clicked any more, and every drag pans or
zooms the map.

**Cause.** ACM's `SKCanvasViewControl` and `SKGLViewControl` keep a private pointer-to-id
map. `OnPointerCaptureLost` removes a pointer from it without raising an action, and
`OnPointerPressed` on a pointer that is already pressed replaces its id without releasing
the old one. Avalonia raises capture loss before or instead of a release in several
cases: a touch cancel, a touch end that X11 turns into a cancel while a transient child
window is active, a disabled window, a gesture recognizer taking the pointer, and the
control leaving the visual tree with a finger down. A second mouse button pressed while
the first is held re-presses the same pointer. GamePage's touch dictionary then keeps the
orphaned id forever.

**Workaround.** GnollHack's canvas and GL controls wrap `TouchAction` during
`OnPointerPressed` to record the id ACM raises `Pressed` for, and raise `Cancelled` for it
when the pointer loses capture without a release or is pressed again.

**Verify removal.** Delete `GHAvaloniaTouchTracker.cs` and the three pointer overrides and
the `_touchTracker` field in both controls. Build the Avalonia flavor. In a headless
Avalonia harness, or on a device, check that every `Pressed` is followed by `Released` or
`Cancelled` for the same id in each of these cases:
- the second of two fingers loses capture
- the control is removed from its window while a finger is down
- a mouse's left button is held while the right is clicked

Checks:
- 2026-10-27 | Avalonia.Controls.Maui.SkiaSharp.Views 11.0.0-rc.2.26287.618 | still-needed | Reproduced in Avalonia.Headless 12.1.4: stock controls leave the touch open in all three cases. No upstream issue found.

## WA-AVALONIA-0020: ACM modal overlays do not trap keyboard focus

- Status: active
- Added: 2026-10-27
- GnollHack version: 3.32 (432)
- Component: Avalonia.Controls.Maui
- Package: Avalonia.Controls.Maui
- Defective version: Avalonia.Controls.Maui 11.0.0-rc.2.26287.618
- Platforms: Linux, Windows (Avalonia)
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-27
- Code: `win/win32/xpl/GnollHackM/Platforms/Avalonia/GHAvaloniaFocusGuard.cs` (`GHAvaloniaFocusGuard.ClearFocusOutsideTopPage`)
- Remove when: ACM's modal presentation keeps keyboard focus inside the top-most modal page (Tab, arrows, and a focus held by the page below) in a release GnollHack uses, and Verify removal passes. The guard stays for GnollHack's own in-page overlays; only the tag and this entry go.

**Symptom.** With a modal page open, Tab and the D-Pad move the focus ring onto the
page below, drawn over the modal, and a button of the page below that still holds focus
clicks on Space.

**Cause.** `ModalOverlayHelper.Present` adds a scrim and a wrapper to the window's root
grid and leaves the page below attached, visible and enabled, and Avalonia's tab and
directional navigation never consider what is painted above a candidate.

**Workaround.** `GHAvaloniaFocusGuard` redirects a keyboard focus change away from any
control whose center a pointer could not reach, clears a focus outside the top-most page
when a modal page is pushed or popped, and clears a covered focus on the next key,
dropping the key when the game declined it.

**Verify removal.** Make `ClearFocusOutsideTopPage` return without doing anything and
make `IsCovered` return false. Build the Avalonia flavor, open Settings from the main
page with the mouse, press Tab repeatedly and then Space: the ring must stay inside
Settings and nothing on the main page may react.

Checks:
- 2026-10-27 | Avalonia.Controls.Maui 11.0.0-rc.2.26287.618 | unverified | No upstream issue found.

## WA-AVALONIA-0021: An animation frame that commits nothing waits for a 16 ms timer

- Status: active
- Added: 2026-10-27
- GnollHack version: 3.32 (432)
- Component: Avalonia
- Package: Avalonia
- Defective version: Avalonia 12.1.4 (via Avalonia.Controls.Maui 11.0.0-rc.2.26287.618)
- Platforms: Linux, Windows (Avalonia)
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-27
- Code: `win/win32/xpl/GnollHackX/GnollHackX/GHApp.cs` (`GHApp.CompositionTarget_Rendering`); `win/win32/xpl/GnollHackM/Platforms/Avalonia/Rendering/GHAvaloniaGLControl.cs` (`GHAvaloniaGLControl.RequestPresent`); `win/win32/xpl/GnollHackM/Platforms/Avalonia/Rendering/GHAvaloniaCanvasControl.cs` (`GHAvaloniaCanvasControl.RequestPaint`); `win/win32/xpl/GnollHackM/Platforms/Avalonia/Rendering/GHAvaloniaCanvasControl.cs` (`GHAvaloniaCanvasControl.RequestPresent`); `win/win32/xpl/GnollHackM/Platforms/Avalonia/Rendering/GHAvaloniaCanvasControl.cs` (`GHAvaloniaCanvasControl.FlushDeferredPaints`); `win/win32/xpl/GnollHackX/GnollHackX/Pages/Game/GamePage.Radial.cs` (`GamePage.InvalidateRadialCanvas`)
- Remove when: An animation frame that commits nothing is followed by one at the display's next refresh in a release GnollHack uses, and Verify removal passes.
- Related: WA-AVALONIA-0010

**Symptom.** At a Map FPS below the refresh rate the map judders: each skipped tick lasts
about 16 ms instead of one refresh, so frames reach the screen on a free-running clock. Raster
canvases (menus, text windows, the command page) miss the display's timing even when they
render every tick.

**Cause.** `MediaContext.RenderCore` (Avalonia `src/Avalonia.Base/Media/MediaContext.cs`)
starts `_animationsTimer`, a dispatcher timer fixed at 16 ms, when a pending
`RequestAnimationFrame` exists and the frame committed nothing. Only a committed frame makes
the next animation frame follow the compositor's render loop. A raster canvas's posted paint
(WA-AVALONIA-0010) runs below render priority, after its frame's render pass.

**Workaround.** Every tick of the platform frame loop commits a frame. On a tick the pacing
logic skips, `GamePage.PresentCanvasByCanvasType` makes the active canvas show its last image
again (`RequestPresent`; the GL control redraws its cached surface without painting), and a
raster canvas asked to paint inside the frame callback paints at once, before the render pass.

**Verify removal.** Make `GamePage.PresentCanvasByCanvasType` return false at once and remove
the in-callback branch of `GHAvaloniaCanvasControl.RequestPaint`. Build and deploy the Linux
flavor, set Map FPS 45 on a 90 Hz display, and run the in-game performance test. The
workaround is still needed if the callback intervals after skipped ticks cluster at 15 to
17 ms in the frame timeline CSV.

Checks:
- 2026-10-27 | Avalonia 12.1.4 | still-needed | Measured on a Steam Deck OLED in Desktop Mode, before the workaround: 15.75 ms median after skipped ticks. No upstream issue found.

## WA-AVALONIA-0022: FileResult.OpenReadAsync throws for ACM's file picker results

- Status: active
- Added: 2026-10-30
- GnollHack version: 3.32 (432)
- Component: Avalonia.Controls.Maui
- Package: Avalonia.Controls.Maui.Essentials
- Defective version: Avalonia.Controls.Maui.Essentials 11.0.0-rc.2.26287.618
- Platforms: Linux, Windows (Avalonia)
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-30
- Code: `win/win32/xpl/GnollHackX/GnollHackX/GHApp.cs` (`GHApp.OpenPickedFileAsync`)
- Remove when: `FileResult.OpenReadAsync`, called through a `FileResult` reference, works on the Avalonia flavor in a release GnollHack uses, and Verify removal passes.

**Symptom.** Import Saved Games and Import Replays fail with an error as soon as a file is
chosen.

**Cause.** `AvaloniaFileResult` only shadows `OpenReadAsync`, as
`FileBase.PlatformOpenReadAsync` is internal. A call through a `FileResult` reference, which
is what `FilePicker.PickAsync` returns, runs the base method, and on portable Essentials that
throws (`FileSystem.netstandard.cs`). The class's documentation says so. Read in source; the
failure itself was not run before the workaround.

**Workaround.** `GHApp.OpenPickedFileAsync` opens `FileResult.FullPath` directly on the
Avalonia flavor, where picked files have local paths.

**Verify removal.** Make `GHApp.OpenPickedFileAsync` call `file.OpenReadAsync()` everywhere.
Build the Avalonia flavor, and import a saved game with the system's dialog. The import must
succeed.

Checks:
- 2026-10-30 | Avalonia.Controls.Maui.Essentials 11.0.0-rc.2.26287.618 | still-needed | Read in the decompiled package and MAUI's source; no upstream issue found.
