# .NET MAUI Workarounds

- Component: .NET MAUI (Microsoft.Maui.Controls, Microsoft.Maui.Core, Microsoft.Maui.Essentials)
- Upstream: https://github.com/dotnet/maui
- ID prefix: WA-MAUI

Entries follow the schema in [README.md](README.md). Append workarounds before the
`<!-- known-issues -->` line if there is one, otherwise at the end of the file, and known
issues at the end of the file. Never renumber or reuse an ID.

<!-- entries -->

## WA-MAUI-0001: Border animates its size and position changes on iOS

- Status: active
- Added: 2024-08-16 (from git history)
- GnollHack version: 2.6 (206)
- Component: .NET MAUI
- Package: Microsoft.Maui.Controls
- Defective version: unknown
- Platforms: iOS
- Upstream issue: https://github.com/dotnet/maui/issues/15363, https://github.com/dotnet/maui/issues/18204
- Upstream status when added: released
- Upstream status: released
- Fixed upstream in: 8.0.70, 9.0.0-preview.7 (https://github.com/dotnet/maui/pull/23156, for #18204)
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/Platforms/iOS/NotAnimatedBorderHandler.cs` (`NotAnimatedBorderHandler.BorderContentView.LayoutSubviews`); `win/win32/xpl/GnollHackM/MauiProgram.cs` (`CreateMauiApp`, `AddHandler<Border, NotAnimatedBorderHandler>`)
- Remove when: Verify removal passes on the Microsoft.Maui.Controls version GnollHack uses.
- Related: WA-MAUI-0002

**Symptom.** When a Border changes size, its stroke and background visibly animate to the
new bounds and position instead of snapping, so the border lags behind its content.

**Cause.** MAUI's iOS border layer (`MauiCALayer`) receives implicit Core Animation
`bounds` and `position` animations on resize. Both cited issues were closed as completed
before this workaround landed (#18204 by PR 23156, in 8.0.70 and 9.0.0-preview.7; #15363
without a linked PR), so the workaround was added on a .NET 9 preview workload that may
already have contained those fixes. Whether it is still needed is unknown.

**Workaround.** A `BorderHandler` subclass creates a `ContentView` whose `LayoutSubviews`
removes the `bounds` and `position` animations from the `MauiCALayer` sublayer before
laying out. It is registered for every `Border` on iOS.

**Verify removal.** Remove the `AddHandler<Border, NotAnimatedBorderHandler>`
registration. Then:
- Build for iOS and run on a device.
- Open pages and popups whose Border elements resize or appear (menus, message popups,
  settings info popups), and confirm no Border animates its size or position.

Checks:
- 2026-10-23 | Microsoft.Maui.Controls 10.0.120 | unverified | Both issues closed as completed; fix for #18204 released in 8.0.70. Possibly obsolete.

## WA-MAUI-0002: Button title crossfades when its text changes on iOS

- Status: active
- Added: 2026-07-15 (from git history)
- GnollHack version: 3.3 (396)
- Component: .NET MAUI
- Package: Microsoft.Maui.Controls
- Defective version: Microsoft.Maui.Controls 10.0.51
- Platforms: iOS
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/Platforms/iOS/NotAnimatedButtonHandler.cs` (`NotAnimatedButtonHandler.NotAnimatedUIButton`); `win/win32/xpl/GnollHackM/MauiProgram.cs` (`CreateMauiApp`, `AddHandler<Button, NotAnimatedButtonHandler>`)
- Remove when: MAUI's iOS ButtonHandler sets the title without an implicit transition, and Verify removal passes.
- Related: WA-MAUI-0001

**Symptom.** Button text fades in when it changes, or when the button's parent becomes
visible, instead of appearing at once.

**Cause.** MAUI's iOS ButtonHandler drives `UIButton` through `UIButtonConfiguration`,
and since iOS 15 a configuration-based button crossfades its title label on title changes
and during layout.

**Workaround.** A `ButtonHandler` subclass creates a `UIButton` whose `SetTitle` and
`LayoutSubviews` run inside `UIView.PerformWithoutAnimation`. It is registered for every
`Button` on iOS.

**Verify removal.** Remove the `AddHandler<Button, NotAnimatedButtonHandler>`
registration. Then:
- Build for iOS and run on a device.
- Show pages and popups containing buttons, and change button texts (for example the
  game's command buttons), and confirm the text appears without a fade.

Checks:
- 2026-10-23 | Microsoft.Maui.Controls 10.0.120 | still-needed | No upstream issue; ButtonHandler.iOS.cs unchanged on main since 10.0.120.

## WA-MAUI-0003: IsOn mapping recolors and re-animates the Liquid Glass UISwitch mid-toggle

- Status: active
- Added: 2026-10-23 (from git history)
- GnollHack version: 3.30 (430)
- Component: .NET MAUI
- Package: Microsoft.Maui.Controls
- Defective version: Microsoft.Maui.Controls 10.0.120
- Platforms: iOS
- Upstream issue: https://github.com/dotnet/maui/issues/34560
- Upstream status when added: reported
- Upstream status: reported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/App.xaml.cs` (`App` constructor, SwitchHandler `IsOn` and `TrackColor` mappings)
- Remove when: A Microsoft.Maui.Controls release fixes dotnet/maui#34560, and Verify removal passes.

**Symptom.** On iOS 26 and later, toggling a Switch glitches: the Liquid Glass switch is
recolored and its toggle animation restarts while UIKit is still animating it.

**Cause.** MAUI's default `IsOn` mapping also forces a `TrackColor` update, which writes
`OnTintColor` and a private track subview, and re-issues an animated `SetState` in the
middle of UIKit's own toggle animation.

**Workaround.** `SwitchHandler.Mapper.ModifyMapping` replaces the `IsOn` mapping with one
that calls `SetState` only when the native state differs, and the `TrackColor` mapping
with one that clears `OnTintColor` when no track color is set and calls MAUI's mapping
only when one is. On iOS the implicit `PlatformSwitchStyle` is not applied, so switches
keep the native colors.

**Verify removal.** Remove both `ModifyMapping` calls. Then:
- Build for iOS and run on an iOS 26 or later device.
- Toggle several switches on the Settings page, and confirm each animates once, keeps the
  native colors, and ends in the tapped state.

Checks:
- 2026-10-23 | Microsoft.Maui.Controls 10.0.120 | still-needed | Issue open.

## WA-MAUI-0004: ControlsModalWrapper does not forward status bar visibility to its page

- Status: active
- Added: 2026-10-23 (from git history)
- GnollHack version: 3.30 (430)
- Component: .NET MAUI
- Package: Microsoft.Maui.Controls
- Defective version: Microsoft.Maui.Controls 10.0.120
- Platforms: iOS
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/Platforms/iOS/StatusBarVisibility.cs` (`StatusBarVisibility.AddModalWrapperForwarding`); `win/win32/xpl/GnollHackM/Platforms/iOS/StatusBarVisibility.cs` (`StatusBarVisibility.ChildViewControllerForStatusBarHidden`)
- Remove when: ControlsModalWrapper overrides `ChildViewControllerForStatusBarHidden` in a Microsoft.Maui.Controls release GnollHack uses, and Verify removal passes.

**Symptom.** With view-controller-based status bar appearance, a modal page's
`PrefersStatusBarHidden` setting has no effect: the status bar shows or hides according
to the wrapper, not the page.

**Cause.** MAUI presents modal pages inside the internal `ControlsModalWrapper`, which
overrides `ChildViewControllerForStatusBarStyle` but not
`ChildViewControllerForStatusBarHidden`, so UIKit asks the wrapper rather than the page.
GnollHack depends on view-controller-based appearance because `UIApplication.StatusBarHidden`
has no effect on iOS 27; that part is an iOS limitation recorded in the iOS register.

**Workaround.** At registration, the wrapper class is looked up by name and
`childViewControllerForStatusBarHidden` is added to it with `class_addMethod`, returning
the wrapper's last child view controller. `class_addMethod` changes nothing if the class
already implements the selector.

**Verify removal.** Remove the `AddModalWrapperForwarding` call from
`StatusBarVisibility.Register`. Then:
- Build for iOS and run on an iOS 27 device.
- Open a modal page (for example Settings), toggle the status bar setting, and confirm
  the status bar hides and shows on the modal page and after closing it.

Checks:
- 2026-10-23 | Microsoft.Maui.Controls 10.0.120 | still-needed | No upstream issue; ControlsModalWrapper.cs on main still lacks the override.

## WA-MAUI-0005: Done accessory leaves an empty band above the keyboard on iOS 27

- Status: active
- Added: 2026-10-23 (from git history)
- GnollHack version: 3.30 (430)
- Component: .NET MAUI
- Package: Microsoft.Maui.Controls
- Defective version: Microsoft.Maui.Controls 10.0.120
- Platforms: iOS
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/Platforms/iOS/KeyboardDoneOverlay.cs` (`KeyboardDoneOverlay`); `win/win32/xpl/GnollHackM/MauiProgram.cs` (`CreateMauiApp`, `KeyboardDoneOverlay.Register` call)
- Remove when: MAUI's iOS Done accessory no longer adds empty height to the keyboard frame on iOS 27, and Verify removal passes.
- Related: WA-MAUI-0006, WA-MAUI-0007

**Symptom.** On iOS 27, a page padded above the keyboard while an Entry or Picker is being
edited shows an empty band between its content and the keyboard.

**Cause.** On iOS 26 and later, MAUI's `MauiDoneAccessoryView` is a 44-point transparent
strip holding a floating glass close button. As an input accessory view, iOS counts the
whole strip as part of the keyboard.

**Workaround.** On iOS 27 and later, Entry and Picker mappings remove MAUI's input
accessory view, and a glass close button of the same look is added to the app window,
floating just above the keyboard; it follows keyboard notifications and ends editing when
tapped (an Entry then gets no Completed event). Because MAUI's Picker applies the wheel's
row only from its Done action, `ApplyPickerWheel` applies it whenever editing ends.
`ButtonFrame` lets the scroll view keep the focused input clear of the button.

**Verify removal.** Remove the `KeyboardDoneOverlay.Register` call from `CreateMauiApp`
(on iOS 26 also see WA-MAUI-0006). Then:
- Build for iOS and run on an iOS 27 device.
- Edit an Entry and a Picker on pages padded above the keyboard, and confirm no empty band
  shows above the keyboard and the MAUI Done control is usable and applies the picker row.

Checks:
- 2026-10-23 | Microsoft.Maui.Controls 10.0.120 | still-needed | No upstream issue; MauiDoneAccessoryView.cs unchanged on main since 10.0.120.

## WA-MAUI-0006: Glass Done button does not show in the iOS 26 compatibility design

- Status: active
- Added: 2026-10-23 (from git history)
- GnollHack version: 3.30 (430)
- Component: .NET MAUI
- Package: Microsoft.Maui.Controls
- Defective version: Microsoft.Maui.Controls 10.0.120
- Platforms: iOS
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/Platforms/iOS/KeyboardDoneOverlay.cs` (`KeyboardDoneOverlay.Register`, `UseLegacyDoneAccessory` switch)
- Remove when: MAUI chooses the legacy Done toolbar when the app runs with UIDesignRequiresCompatibility, and Verify removal passes.
- Related: WA-MAUI-0005

**Symptom.** On iOS 26, where GnollHack runs with `UIDesignRequiresCompatibility`, Entry
and Picker show no Done control above the keyboard.

**Cause.** `MauiDoneAccessoryView` uses its floating glass close button whenever the OS
version is 26 or later, without checking whether the app runs in the compatibility
design, where that button does not show. The legacy Done toolbar does show.

**Workaround.** Below iOS 27, `Register` sets MAUI's
`Microsoft.Maui.Platform.iOS.UseLegacyDoneAccessory` AppContext switch to true, so MAUI
uses its legacy Done toolbar.

**Verify removal.** Remove the `AppContext.SetSwitch` call. Then:
- Build for iOS and run on an iOS 26 device.
- Edit an Entry and a Picker, and confirm a Done control shows and works.

Checks:
- 2026-10-23 | Microsoft.Maui.Controls 10.0.120 | still-needed | No upstream issue; MauiDoneAccessoryView.cs unchanged on main since 10.0.120.

## WA-MAUI-0007: Focused input is not kept visible when the page shrinks above the keyboard

- Status: active
- Added: 2026-10-23 (from git history)
- GnollHack version: 3.30 (430)
- Component: .NET MAUI
- Package: Microsoft.Maui.Controls
- Defective version: Microsoft.Maui.Controls 10.0.120
- Platforms: iOS
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/Platforms/iOS/FocusKeepingScrollViewHandler.cs` (`FocusKeepingScrollViewHandler.FocusKeepingScrollView`); `win/win32/xpl/GnollHackM/MauiProgram.cs` (`CreateMauiApp`, `AddHandler<ScrollView, FocusKeepingScrollViewHandler>`)
- Remove when: MAUI keeps the focused input visible when a `SafeAreaEdges.All` page shrinks above the keyboard, and Verify removal passes.
- Related: WA-MAUI-0005

**Symptom.** On a `SafeAreaEdges.All` page, focusing an Entry or Picker low in a
ScrollView lets the keyboard or the picker wheel cover it.

**Cause.** MAUI shrinks such pages above the keyboard but does not scroll the ScrollView,
and UIKit scrolls a field into view only when it gains focus, before the shrink.
`MauiScrollView.ScrollRectToVisible` also does nothing while
`KeyboardAutoManagerScroll.IsKeyboardAutoScrollHandling` is set.

**Workaround.** A `ScrollViewHandler` subclass creates a `MauiScrollView` that, whenever
its height shrinks, finds the first responder and sets the content offset directly so the
input is visible, also above the floating close button of WA-MAUI-0005. Outside
edge-to-edge mode its `ScrollRectToVisible` uses the same offset logic. It is registered
for every `ScrollView` on iOS.

**Verify removal.** Remove the `AddHandler<ScrollView, FocusKeepingScrollViewHandler>`
registration. Then:
- Build for iOS and run on a device, with edge-to-edge off.
- On Settings, focus an Entry and a Picker near the bottom of the page, and confirm each
  stays visible above the keyboard or wheel.

Checks:
- 2026-10-23 | Microsoft.Maui.Controls 10.0.120 | still-needed | No upstream issue; MauiScrollView.cs unchanged on main since 10.0.120.

## WA-MAUI-0008: SafeAreaEdges.None does not make iOS pages edge-to-edge

- Status: active
- Added: 2026-07-14 (from git history)
- GnollHack version: 3.3 (396)
- Component: .NET MAUI
- Package: Microsoft.Maui.Controls
- Defective version: Microsoft.Maui.Controls 10.0.51
- Platforms: iOS
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackX/GnollHackX/CustomContentPage.cs` (`CustomContentPage.OnAppearing`); `win/win32/xpl/GnollHackX/GnollHackX/CustomContentPage.cs` (`CustomContentPage.OnSizeAllocated`); `win/win32/xpl/GnollHackX/GnollHackX/CustomContentPage.cs` (`CustomContentPage.SetEdge2Edge`); `win/win32/xpl/GnollHackX/GnollHackX/CustomContentPage.cs` (`CustomContentPage.ApplyiOSEdge2Edge`)
- Remove when: Setting `SafeAreaEdges` to `None` alone extends main and modal pages under the notch and home indicator in a Microsoft.Maui.Controls release GnollHack uses, and Verify removal passes.

**Symptom.** With edge-to-edge on, pages still stop at the notch and home indicator on
iOS, even though `SafeAreaEdges` is `None`.

**Cause.** The `SafeAreaEdges` setting does not propagate to the iOS view controller
hierarchy, including the separate hierarchies of modal presentations.

**Workaround.** The MAUI property is still set. In addition, on iOS, negative
`AdditionalSafeAreaInsets` equal to the key window's safe area are applied to the root
view controller and its children, every presented view controller chain, and the page's
own view controller and its parents. They are re-applied on appearing and on size
changes, since rotation changes the insets.

**Verify removal.** Make `ApplyiOSEdge2Edge` return at once. Then:
- Build for iOS and run on a device with a notch or Dynamic Island.
- Turn edge-to-edge on, open the main page, a game, and a modal page, rotate the device,
  and confirm content extends under the notch and home indicator each time.

Checks:
- 2026-10-23 | Microsoft.Maui.Controls 10.0.120 | unverified | No upstream issue found.

## WA-MAUI-0009: Span tap regions are offset from the rendered text on iOS

- Status: active
- Added: 2026-08-02 (from git history)
- GnollHack version: 3.7 (405)
- Component: .NET MAUI
- Package: Microsoft.Maui.Controls
- Defective version: Microsoft.Maui.Controls 10.0.51
- Platforms: iOS
- Upstream issue: https://github.com/dotnet/maui/issues/37013
- Upstream status when added: unreported
- Upstream status: released
- Fixed upstream in: 10.0.90 (per the reporter; fixing PR not identified)
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackX/GnollHackX/Controls/MessagePopupView.xaml.cs` (`MessagePopupView.ScheduleSpanPositionRecalculation`); `win/win32/xpl/GnollHackX/GnollHackX/Controls/MessagePopupView.xaml.cs` (`MessagePopupView.OnLabelSizeChangedForSpanRecalc`); `win/win32/xpl/GnollHackX/GnollHackX/Controls/MessagePopupView.xaml.cs` (`MessagePopupView.RecalculateSpanPositionsFromNative`)
- Remove when: Verify removal passes on the Microsoft.Maui.Controls version GnollHack uses.

**Symptom.** Tapping a link span in a message popup misses: the tap regions are offset
from the visible text, increasingly toward the bottom of longer texts.

**Cause.** MAUI computes span regions in `Label.ArrangeOverride`, before the `UILabel`
bounds are final, and with a TextKit 1 layout that differs from the TextKit 2 layout
`UILabel` renders with. The issue was filed after this workaround and closed by the
reporter after 10.0.90 was found to work, so the workaround is probably obsolete.

**Workaround.** After each formatted message, the next `SizeChanged` of the label defers
to the next run loop iteration and recomputes each span's `ISpatialElement.Region` from
the native `UILabel`'s attributed text and bounds, falling back to the MAUI size.

**Verify removal.** Make `ScheduleSpanPositionRecalculation` do nothing. Then:
- Build for iOS and run on a device.
- Open message popups with several link spans, including one near the end of a long
  text, and confirm every link opens when tapped.

Checks:
- 2026-10-23 | Microsoft.Maui.Controls 10.0.120 | unverified | Issue closed as completed by the reporter, fixed in 10.0.90. Probably obsolete.

## WA-MAUI-0010: FormattedText spans do not inherit the Label's font on iOS

- Status: active
- Added: 2026-07-11 (from git history)
- GnollHack version: 3.1 (393)
- Component: .NET MAUI
- Package: Microsoft.Maui.Controls
- Defective version: Microsoft.Maui.Controls 10.0.51
- Platforms: iOS
- Upstream issue: https://github.com/dotnet/maui/issues/36520
- Upstream status when added: unreported
- Upstream status: released
- Fixed upstream in: 10.0.60 (per the reporter; fixing PR not identified)
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackX/GnollHackX/Pages/MainScreen/SettingsPage.xaml.cs` (`SettingsPage.SetupSettingLabels`); `win/win32/xpl/GnollHackX/GnollHackX/UIUtils.cs` (`UIUtils.SetMarkdownText`)
- Remove when: Verify removal passes on the Microsoft.Maui.Controls version GnollHack uses.

**Symptom.** In setting description popups, the plain text runs between formatted runs
appear in a different font than the label's own font on iOS.

**Cause.** Spans without their own `FontFamily` do not inherit the parent Label's font
family on iOS and fall back to the system font. The issue was filed after this workaround
and closed by the reporter as already fixed for iOS in 10.0.60, so the workaround is
probably obsolete.

**Workaround.** `SetMarkdownText` takes a base font family and sets it on every plain
span. The settings page passes the font family of the tapped setting label.

**Verify removal.** In `SetMarkdownText`, create the plain spans without `FontFamily`.
Then:
- Build for iOS and run on a device.
- Tap several setting labels on the Settings page, and confirm the popup's plain text uses
  the tapped label's font.

Checks:
- 2026-10-23 | Microsoft.Maui.Controls 10.0.120 | unverified | Issue closed as completed by the reporter, fixed in 10.0.60. Probably obsolete.

## WA-MAUI-0011: WebView Navigated event is not raised on iOS

- Status: active
- Added: 2024-05-26 (from git history)
- GnollHack version: 1.0 (1)
- Component: .NET MAUI
- Package: Microsoft.Maui.Controls
- Defective version: unknown
- Platforms: iOS
- Upstream issue: none
- Upstream status when added: unknown
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackX/GnollHackX/Pages/Game/GameMenuPage.xaml.cs` (`GameMenuPage.OpenWikiPage`)
- Remove when: WikiPage's navigation buttons work with the MAUI WebView on iOS, and Verify removal passes.

**Symptom.** In the in-app wiki page, the back and forward navigation buttons stay
disabled on iOS.

**Cause.** The WebView's `Navigated` event does not fire on iOS, and the wiki page enables
its navigation buttons from that event. This was observed with the .NET 9 preview
workload; whether MAUI 10 still behaves so is unknown (`OverseerPage` relies on MAUI's
iOS navigation delegate routing these events).

**Workaround.** On iOS the wiki opens with `GHApp.OpenBrowser` instead of `WikiPage`.

**Verify removal.** In `OpenWikiPage`, drop `GHApp.IsiOS` from the browser condition.
Then:
- Build for iOS and run on a device.
- Open the wiki from the game menu, follow two links, and confirm the back and forward
  buttons enable and work.

Checks:
- 2026-10-23 | Microsoft.Maui.Controls 10.0.120 | unverified | No upstream issue found. Possibly obsolete.

## WA-MAUI-0012: WindowInsets padding is applied despite SafeAreaEdges None on Android 15

- Status: active
- Added: 2026-07-14 (from git history)
- GnollHack version: 3.3 (396)
- Component: .NET MAUI
- Package: Microsoft.Maui.Controls
- Defective version: Microsoft.Maui.Controls 10.0.71
- Platforms: Android
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/Platforms/Android/MainActivity.cs` (`MainActivity.OnCreate`); `win/win32/xpl/GnollHackM/Platforms/Android/MainActivity.cs` (`MainActivity.BypassInsetsListener`); `win/win32/xpl/GnollHackM/MauiProgram.cs` (`CreateMauiApp`, modal DialogFragment `BypassInsetsListener` registration); `win/win32/xpl/GnollHackM/MauiProgram.cs` (`BypassInsetsListener`)
- Remove when: With `SafeAreaEdges="None"` alone, main and modal pages extend under the cutout and system bars on Android 15 and later in a Microsoft.Maui.Controls release GnollHack uses, and Verify removal passes.
- Related: WA-MAUI-0008, WA-MAUI-0013

**Symptom.** With edge-to-edge on, pages on Android 15 and later stay padded away from the
display cutout and system bars.

**Cause.** MAUI applies the WindowInsets as padding on the root view even when the page's
`SafeAreaEdges` is `None`.

**Workaround.** When edge-to-edge is on, an `IOnApplyWindowInsetsListener` on the activity's
`DecorView`, and on the `DecorView` of each modal DialogFragment window, sets the padding
to zero and returns consumed insets so MAUI does not add the padding back. The listener
class exists twice, as a private class in `MainActivity` and as a public class in
`MauiProgram.cs`.

**Verify removal.** Remove the three `SetOnApplyWindowInsetsListener` calls. Then:
- Build for Android and run on an Android 15 or later device with a display cutout.
- Turn edge-to-edge on, restart, open the main page, a game and a modal page (Settings),
  and confirm content extends under the cutout and system bars each time.

Checks:
- 2026-10-23 | Microsoft.Maui.Controls 10.0.120 | unverified | No upstream issue found.

## WA-MAUI-0013: Modal pages on Android lose the activity's immersive mode and key handling

- Status: active
- Added: 2024-10-10 (from git history)
- GnollHack version: 2.14 (214)
- Component: .NET MAUI
- Package: Microsoft.Maui.Controls
- Defective version: unknown (.NET 9 workload, `$(MauiVersion)`)
- Platforms: Android
- Upstream issue: none
- Upstream status when added: unknown
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/MauiProgram.cs` (`CreateMauiApp`, Android `OnCreate` registering `MyFragmentLifecycleCallbacks`); `win/win32/xpl/GnollHackM/MauiProgram.cs` (`MyFragmentLifecycleCallbacks`); `win/win32/xpl/GnollHackM/MauiProgram.cs` (`AndroidDialogKeyPress`)
- Remove when: MAUI's modal DialogFragment windows follow the activity's system bar visibility and pass key events to the activity in a Microsoft.Maui.Controls release GnollHack uses, and Verify removal passes.
- Related: WA-MAUI-0012

**Symptom.** While a modal page is shown, the status and navigation bars reappear, and
hardware keyboard keys do not reach the game's key handling.

**Cause.** Since .NET 9, MAUI shows modal pages in a DialogFragment. The dialog has its own
Window, which does not inherit the activity's immersive system bar settings, and key
events go to the dialog instead of the activity.

**Workaround.** A fragment lifecycle callback, registered on each activity's fragment
manager, runs on every start and stop of a DialogFragment. While the fragment is added, it
hides the system bars of the dialog's window (WindowInsetsController on API 30 and later,
the legacy immersive flags below) and subscribes `AndroidDialogKeyPress`, which passes key
presses to `PlatformService.HandleOnKeyDown`; it unsubscribes when the fragment is removed.

**Verify removal.** Remove the `RegisterFragmentLifecycleCallbacks` call. Then:
- Build for Android and run on a device with a hardware keyboard, with the navigation bar
  hidden.
- Open a modal page (Settings, the game menu), and confirm the system bars stay hidden and
  keyboard keys (Esc, letters) work on the modal page.

Checks:
- 2026-10-23 | Microsoft.Maui.Controls 10.0.120 | unverified | No upstream issue found.

## WA-MAUI-0014: Fill-aligned ScrollView measures at the full constraint on Android

- Status: active
- Added: 2026-10-02 (from git history)
- GnollHack version: 3.21 (421)
- Component: .NET MAUI
- Package: Microsoft.Maui.Controls
- Defective version: Microsoft.Maui.Controls 10.0.110
- Platforms: Android
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackX/GnollHackX/ConsistentScrollView.cs` (`ConsistentScrollViewHandler`); `win/win32/xpl/GnollHackM/MauiProgram.cs` (`CreateMauiApp`, `AddHandler(typeof(ConsistentScrollView), typeof(ConsistentScrollViewHandler))`)
- Remove when: MAUI's Android ScrollViewHandler measures a Fill-aligned ScrollView to its content like iOS and Windows in a Microsoft.Maui.Controls release GnollHack uses, and Verify removal passes.

**Symptom.** A ScrollView with Fill alignment takes the whole available height on Android
even when its content is short, while on iOS and Windows it sizes to its content.

**Cause.** The stock Android ScrollViewHandler measures a Fill-aligned ScrollView at the
full constraint instead of measuring its content.

**Workaround.** `ConsistentScrollView` gets its own Android handler. `GetDesiredSize`
measures the platform view with a copy of MAUI's internal
`ContextExtensions.CreateMeasureSpec`, so it sizes to content up to the constraint, and
`PlatformArrange` re-measures at the final frame so `FillViewport` still stretches short
content.

**Verify removal.** Remove the `AddHandler` registration for `ConsistentScrollView`. Then:
- Build for Android and run on a device.
- Open the game menu, whose list is a `ConsistentScrollView`, in portrait and landscape,
  and confirm the menu sizes to its items as on Windows.

Checks:
- 2026-10-23 | Microsoft.Maui.Controls 10.0.120 | unverified | No upstream issue found.

## WA-MAUI-0015: ScrollView misbehaves with enabled entries when the Settings page opens on Android

- Status: active
- Added: 2024-11-13 (from git history)
- GnollHack version: 2.14 (214)
- Component: .NET MAUI
- Package: Microsoft.Maui.Controls
- Defective version: unknown (.NET 9 GA workload, `$(MauiVersion)`)
- Platforms: Android
- Upstream issue: none
- Upstream status when added: unknown
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackX/GnollHackX/Pages/MainScreen/SettingsPage.xaml.cs` (`SettingsPage` constructor, `PostXlogUserNameEntry` and `PostXlogPasswordEntry` disabled by a timer)
- Remove when: The Settings page opens correctly on Android with both entries enabled from the start, and Verify removal passes.

**Symptom.** The source names only "ScrollView bug in .NET MAUI 9.0 GA". The code keeps the
server account name and password entries disabled for the first 0.5 seconds after the
Settings page is created, which points at the ScrollView reacting to these entries (for
example scrolling to them) while the page opens.

**Cause.** Not recorded beyond the comment. The workaround dates from the .NET 9 GA
workload and may be obsolete on MAUI 10.

**Workaround.** On MAUI Android, the constructor disables both entries and re-enables them
from a one-shot 0.5-second dispatcher timer.

**Verify removal.** Remove the `#if GNH_MAUI && ANDROID` block that disables the entries.
Then:
- Build for Android and run on a device.
- Open Settings several times from the main menu and from the game menu, and confirm the
  page opens at the top without jumping, and both entries can be edited.

Checks:
- 2026-10-23 | Microsoft.Maui.Controls 10.0.120 | unverified | No upstream issue found. Possibly obsolete.

## WA-MAUI-0016: Framework teardown raises ObjectDisposedException on Android

- Status: active
- Added: 2026-06-26 (from git history)
- GnollHack version: 2.143 (385)
- Component: .NET MAUI
- Package: Microsoft.Maui.Controls
- Defective version: Microsoft.Maui.Controls 10.0.51
- Platforms: Android
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/Platforms/Android/FrameworkObjectDisposedFilter.cs` (`FrameworkObjectDisposedFilter`); `win/win32/xpl/GnollHackM/Platforms/Android/MainApplication.cs` (`MainApplication` constructor, `UnhandledExceptionRaiser` handler); `win/win32/xpl/GnollHackM/Platforms/Android/MainActivity.cs` (`MainActivity.OnDestroy`); `win/win32/xpl/GnollHackM/MauiProgram.cs` (`CreateMauiApp`, Android `SetBeforeSend` filter)
- Remove when: No "Handled framework ObjectDisposedException" warnings reach Sentry from a release built with a Microsoft.Maui.Controls version that fixes the teardown, and Verify removal passes.

**Symptom.** Since .NET 10, an ObjectDisposedException is thrown during Android activity
and view teardown from MAUI code, and reaches the app as an unhandled exception or from
`base.OnDestroy`, crashing the app or reporting an error to Sentry.

**Cause.** MAUI accesses Java peers that have already been disposed while the activity or
its views are being torn down. The nearest frame outside the Java interop plumbing, the
Android bindings and the base class library belongs to `Microsoft.Maui`.

**Workaround.** `FrameworkObjectDisposedFilter` classifies an ObjectDisposedException,
unwrapping transparent wrappers, as framework teardown by that nearest frame. Such an
exception is marked handled in `AndroidEnvironment.UnhandledExceptionRaiser`, caught
around `base.OnDestroy`, and turned into a Sentry warning in `BeforeSend`. Every other
ObjectDisposedException is still reported as an error.

**Verify removal.** Keep the filter and check Sentry first: while releases keep sending
"Handled framework ObjectDisposedException" warnings, the workaround is needed. Then remove
the four tagged sites, and:
- Build for Android Release and run on a device.
- Close the app from the main menu and from a game, background and resume it repeatedly,
  and rotate on several pages, and confirm no crash and no ObjectDisposedException error
  reaches Sentry.

Checks:
- 2026-10-23 | Microsoft.Maui.Controls 10.0.120 | unverified | No upstream issue found.

## WA-MAUI-0017: Finishing the activity leaves MAUI in an unstable state on Android

- Status: active
- Added: 2026-01-23 (from git history)
- GnollHack version: 2.96 (332)
- Component: .NET MAUI
- Package: Microsoft.Maui.Controls
- Defective version: Microsoft.Maui.Controls 9.0.120
- Platforms: Android
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackX/GnollHackX.Android/PlatformServiceAndroid.cs` (`PlatformServiceAndroid.CloseApplication`)
- Remove when: Closing the app with `FinishAffinity` and `Finish`, or with `Application.Quit`, leaves no unstable state on the next launch in a Microsoft.Maui.Controls release GnollHack uses, and Verify removal passes.

**Symptom.** After the app is closed by finishing its activities, the process keeps
running and MAUI is left in an unstable state.

**Cause.** Not established. The source says only that `FinishAffinity` "seems to leave MAUI
in an unstable state"; finishing the activities does not end the process, so MAUI's
process-wide application state survives into the next launch.

**Workaround.** On MAUI, `CloseApplication` kills its own process with
`Process.KillProcess`, then calls `JavaSystem.Exit(0)` in case that failed, and calls
`Application.Quit` only if both failed. The Xamarin build still uses `FinishAffinity` and
`Finish`.

**Verify removal.** Replace the kill and exit calls with
`Platform.CurrentActivity.FinishAffinity()` and `Finish()`. Then:
- Build for Android and run on a device.
- Exit from the main menu, relaunch from the launcher and from recent apps, start and
  save a game, and confirm the app starts cleanly each time with no errors in Sentry or
  the app log.

Checks:
- 2026-10-23 | Microsoft.Maui.Controls 10.0.120 | unverified | No upstream issue found.

## WA-MAUI-0018: Modal page has a 32-pixel title bar gap in full-screen mode on Windows

- Status: active
- Added: 2026-06-20 (from git history)
- GnollHack version: 2.140 (381)
- Component: .NET MAUI
- Package: Microsoft.Maui.Controls
- Defective version: Microsoft.Maui.Controls 10.0.71
- Platforms: Windows (MAUI)
- Upstream issue: https://github.com/dotnet/maui/issues/36040
- Upstream status when added: reported
- Upstream status: released
- Fixed upstream in: 10.0.110 (https://github.com/dotnet/maui/pull/36209)
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackX/GnollHackX/CustomModalPage.cs` (`CustomModalPage.OnAppearing`); `win/win32/xpl/GnollHackX/GnollHackX/CustomModalPage.cs` (`CustomModalPage.FixModalTitleBarGap`)
- Remove when: Verify removal passes on the Microsoft.Maui.Controls version GnollHack uses.

**Symptom.** In full-screen mode, a modal page starts 32 pixels below the top of the
window, and the strip above it is an unclickable non-client area.

**Cause.** When pushing a modal, MAUI's Windows `ModalNavigationManager` re-enabled the
title bar reservation on the modal's navigation root even though the window used the
full-screen presenter. Fixed by PR 36209, released in 10.0.110, which GnollHack's
Microsoft.Maui.Controls 10.0.120 contains, so the workaround is probably obsolete.

**Workaround.** Modal pages derive from `CustomModalPage`. Outside windowed mode,
`OnAppearing` dispatches `FixModalTitleBarGap`, which gets the modal's scoped
`NavigationRootManager` and calls its internal `SetTitleBarVisibility(false)` by
reflection, collapsing the title bar and its margin.

**Verify removal.** Make `FixModalTitleBarGap` return at once. Then:
- Build for Windows and run in full-screen mode (windowed mode off).
- Open several modal pages (Settings, About, the game menu), and confirm each fills the
  window from the top and its top strip is clickable.

Checks:
- 2026-10-23 | Microsoft.Maui.Controls 10.0.120 | unverified | Fix released in 10.0.110. Probably obsolete.

## WA-MAUI-0019: Re-entrant window activation throws "already activated" or "already deactivated"

- Status: active
- Added: 2026-05-20 (from git history)
- GnollHack version: 2.128 (369)
- Component: .NET MAUI
- Package: Microsoft.Maui.Controls
- Defective version: Microsoft.Maui.Controls 9.0.120
- Platforms: iOS, Windows (MAUI)
- Upstream issue: https://github.com/dotnet/maui/issues/31315
- Upstream status when added: reported
- Upstream status: reported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/Platforms/Windows/App.xaml.cs` (`App` constructor, `UnhandledException` handler); `win/win32/xpl/GnollHackM/MauiProgram.cs` (`CreateMauiApp`, iOS `SetBeforeSend` activation filter); `win/win32/xpl/GnollHackM/MauiProgram.cs` (`CreateMauiApp`, Windows `SetBeforeSend` activation filter); `win/win32/xpl/GnollHackM/MauiProgram.cs` (`KeyboardHook.ShouldHandleKeys`); `win/win32/xpl/GnollHackX/GnollHackX/GHApp.cs` (`GHApp.CompleteWindowsResume`)
- Remove when: dotnet/maui#31315 is fixed in a Microsoft.Maui.Controls release GnollHack uses, and Verify removal passes.
- Related: GNOLLHACK-GK, GNOLLHACK-FA, GNOLLHACK-FB, GNOLLHACK-FD, GNOLLHACK-FP

**Symptom.** Switching apps occasionally throws an InvalidOperationException "Window was
already activated" or "already deactivated" from MAUI's window lifecycle, which crashes
the app on Windows and skips GnollHack's activation handlers, leaving its focus state
stale.

**Cause.** MAUI's window state machine throws when an activation or deactivation arrives
while it believes the window is already in that state. On Windows, a WM_ACTIVATE that
arrives re-entrantly, while a synchronous call inside the resume handler dispatches window
messages, desyncs MAUI's two activation flags.

**Workaround.** The exception is marked handled in WinUI's `UnhandledException` on
Windows, and `BeforeSend` turns it into a Sentry warning on iOS and Windows, with the
activation state attached on Windows. On Windows, the resume work that dispatches window
messages (battery, connectivity and account checks, restoring the presenter) runs from
the dispatcher after MAUI's activation sequence in `CompleteWindowsResume`, and the
keyboard hook checks the foreground window directly instead of trusting the
lifecycle-driven `WindowFocused` flag.

**Verify removal.** Check Sentry first: while "Handled InvalidOperationException ... already
activated" or "already deactivated" warnings keep arriving, the workaround is needed. Then
remove the `UnhandledException` handler and both `BeforeSend` filters, and:
- Build for Windows and iOS and run on each.
- Switch apps rapidly many times, including with Alt+Tab during the resume, and confirm no
  exception is thrown and keys reach the game after each return.

Checks:
- 2026-10-23 | Microsoft.Maui.Controls 10.0.120 | still-needed | Issue open.

## WA-MAUI-0020: WebViewHandler dereferences a null CoreWebView2 when WebView2 fails to initialize

- Status: active
- Added: 2026-09-14 (from git history)
- GnollHack version: 3.16 (416)
- Component: .NET MAUI
- Package: Microsoft.Maui.Controls
- Defective version: Microsoft.Maui.Controls 10.0.101
- Platforms: Windows (MAUI)
- Upstream issue: https://github.com/dotnet/maui/issues/38527
- Upstream status when added: reported
- Upstream status: released
- Fixed upstream in: 10.0.120 (https://github.com/dotnet/maui/pull/38569)
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackX/GnollHackX/Pages/MainScreen/DisplayFilePage.xaml.cs` (`DisplayFilePage.ReadFile`, `_pendingHtmlSource`); `win/win32/xpl/GnollHackX/GnollHackX/Pages/MainScreen/DisplayFilePage.xaml.cs` (`DisplayFilePage.DisplayWebView_Loaded`)
- Remove when: Verify removal passes on the Microsoft.Maui.Controls version GnollHack uses.

**Symptom.** On a computer where WebView2 cannot initialize, opening an HTML file (such as
a dumplog) crashes the app with a NullReferenceException.

**Cause.** MAUI's Windows `WebViewHandler` handled `CoreWebView2Initialized` by subscribing
to `sender.CoreWebView2` events without checking the exception or a null core. Fixed by
PR 38569, released in 10.0.120, which GnollHack uses, so the workaround is probably
obsolete.

**Workaround.** On MAUI Windows, `ReadFile` holds the HTML source back. When the WebView is
loaded, `DisplayWebView_Loaded` awaits `EnsureCoreWebView2Async` and assigns the source
only if that succeeded; otherwise it shows an error text instead of the WebView.

**Verify removal.** Assign the HTML source directly in `ReadFile` and remove the `Loaded`
handler. Then:
- Build for Windows.
- Run with WebView2 initialization failing (for example `WEBVIEW2_BROWSER_EXECUTABLE_FOLDER`
  set to an empty folder), open a dumplog, and confirm the app does not crash.

Checks:
- 2026-10-23 | Microsoft.Maui.Controls 10.0.120 | unverified | Fix released in 10.0.120, which GnollHack uses. Probably obsolete.

## WA-MAUI-0021: MauiFont fonts are unreliable on Android and Windows

- Status: active
- Added: 2024-07-07 (from git history)
- GnollHack version: 2.0 (200)
- Component: .NET MAUI
- Package: Microsoft.Maui.Controls
- Defective version: unknown (.NET 9 preview workload, `$(MauiVersion)`)
- Platforms: Android, Windows (MAUI)
- Upstream issue: none
- Upstream status when added: unknown
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/GnollHackM.csproj` (`MauiAsset` items for `Resources\Fonts` on Android and Windows)
- Remove when: Custom fonts declared as `MauiFont` load on Android and Windows in a Microsoft.Maui.Controls release GnollHack uses, and Verify removal passes.
- Related: WA-MAUI-0022

**Symptom.** Custom fonts declared as `MauiFont` items were missing on Android, and later
on Windows.

**Cause.** Not recorded beyond "MauiFont has been unreliable on Android and Windows". The
Android change was called a temporary fix (2024-07-07), and Windows followed on
2024-07-17.

**Workaround.** On Android and Windows the font files are copied as `MauiAsset` items with
their file names as logical names, and the fonts registered in `ConfigureFonts` load
from there.
iOS and Mac Catalyst still use `MauiFont`.

**Verify removal.** Replace the two `MauiAsset` font items with `MauiFont` items. Then:
- Build for Android and Windows, from a clean `obj` directory, and run each.
- Open the main menu, a game and the Settings page, and confirm every custom font
  (Diablo, Immortal, Underwood, DejaVu Sans Mono, Lato) renders.

Checks:
- 2026-10-23 | Microsoft.Maui.Controls 10.0.120 | unverified | No upstream issue found.

## WA-MAUI-0022: First use of a custom font sometimes fails on Windows

- Status: active
- Added: 2026-03-25 (from git history)
- GnollHack version: 2.117 (355)
- Component: .NET MAUI
- Package: Microsoft.Maui.Controls
- Defective version: Microsoft.Maui.Controls 9.0.120
- Platforms: Windows (MAUI)
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackX/GnollHackX/GHApp.cs` (`GHApp.InitBaseFontsViaGrid`); `win/win32/xpl/GnollHackX/GnollHackX/MainPage.xaml.cs` (`MainPage.StartUpTasks`, `InitBaseFontsViaGrid` call)
- Remove when: Custom fonts load on first use on Windows without COM re-entrancy problems in a Microsoft.Maui.Controls release GnollHack uses, and Verify removal passes.
- Related: WA-MAUI-0021

**Symptom.** On Windows, a custom font used for the first time later in the session
sometimes fails to load, and the source ties this to COM re-entrancy issues.

**Cause.** Not established beyond the comments: MAUI's Windows font loading, done lazily on
first use of a font family, "seems to sometimes fail" and is involved in COM re-entrancy
problems.

**Workaround.** During start-up on Windows, `InitBaseFontsViaGrid` adds an invisible Label
for each base font family to the main page's root grid, waits for its handler (at most 84
ms) and removes it, so each font is loaded early, one at a time.

**Verify removal.** Remove the `InitBaseFontsViaGrid` call from `StartUpTasks`. Then:
- Build for Windows and run several cold starts.
- Open the main menu, start a game and open menus and popups that use Diablo, Immortal,
  Underwood, Lato and DejaVu Sans Mono, and confirm every font renders and no COM or
  re-entrancy exception appears in the app log or Sentry.

Checks:
- 2026-10-23 | Microsoft.Maui.Controls 10.0.120 | unverified | No upstream issue found.

## WA-MAUI-0023: Replacing FormattedText leaves old span highlights on Windows

- Status: active
- Added: 2026-07-11 (from git history)
- GnollHack version: 3.1 (393)
- Component: .NET MAUI
- Package: Microsoft.Maui.Controls
- Defective version: Microsoft.Maui.Controls 10.0.71
- Platforms: Windows (MAUI)
- Upstream issue: https://github.com/dotnet/maui/issues/36519
- Upstream status when added: unreported
- Upstream status: released
- Fixed upstream in: 10.0.110 (https://github.com/dotnet/maui/pull/36608)
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackX/GnollHackX/UIUtils.cs` (`UIUtils.SetMarkdownText`, `label.Text = ""` before the new FormattedText)
- Remove when: Verify removal passes on the Microsoft.Maui.Controls version GnollHack uses.

**Symptom.** On Windows, setting a Label's `FormattedText` again leaves the previous spans'
background and text colors in place, overlapping the new text.

**Cause.** MAUI's Windows `FormattedStringExtensions.UpdateInlines` cleared the TextBlock's
`Inlines` but not its `TextHighlighters`, which only the plain-text path cleared. The
issue was filed a few hours after this workaround, and the fix (PR 36608) is released in
10.0.110, which GnollHack's 10.0.120 contains, so the workaround is probably obsolete.

**Workaround.** `SetMarkdownText` sets `label.Text` to an empty string, which clears the
highlighters, before assigning the new FormattedText.

**Verify removal.** Remove the `label.Text = ""` line before the FormattedString is built.
Then:
- Build for Windows.
- On the Settings page, tap several setting labels with formatted descriptions in a row,
  and confirm each popup shows only its own formatting.

Checks:
- 2026-10-23 | Microsoft.Maui.Controls 10.0.120 | unverified | Fix released in 10.0.110. Probably obsolete.

## WA-MAUI-0024: OnPlatformMessage costs an allocation for every window message on Windows

- Status: active
- Added: 2026-10-20 (from git history)
- GnollHack version: 3.28 (428)
- Component: .NET MAUI
- Package: Microsoft.Maui.Controls
- Defective version: Microsoft.Maui.Controls 10.0.110
- Platforms: Windows (MAUI)
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/Platforms/Windows/EnvironmentStateWindows.cs` (`EnvironmentStateWindows`)
- Remove when: A registered `WindowsLifecycle.OnPlatformMessage` handler no longer allocates per window message in a Microsoft.Maui.Controls release GnollHack uses, and Verify removal passes.

**Symptom.** Registering MAUI's Windows `OnPlatformMessage` lifecycle event, the only MAUI
way to see the window's messages, slowed the app down severely, as the team observed.

**Cause.** `MauiWinUIWindow` raises `OnPlatformMessage` from its window procedure for every
message, through a lifecycle-event lookup and a capturing lambda, and allocates a
`WindowsPlatformMessageEventArgs` per message for each registered handler.

**Workaround.** GnollHack registers no `OnPlatformMessage` handler. `EnvironmentStateWindows`
collects the console display state and suspend and resume through powrprof
`DEVICE_NOTIFY_CALLBACK` callbacks, the session lock by polling
`WTSQuerySessionInformation` once a minute (so a lock is seen up to a minute late and a
short one can be missed), and window visibility from the window's `VisibilityChanged`
event, instead of from `WM_POWERBROADCAST` and `WM_WTSSESSION_CHANGE`.

**Verify removal.** In a test build, register an `OnPlatformMessage` handler that only
reads the message ID. Then:
- Build for Windows Release and run with the frame time profiler on.
- Move the mouse continuously over the map during play and in menus, and confirm the
  frame time, allocation rate and GC counts match a build without the handler.

Checks:
- 2026-10-23 | Microsoft.Maui.Controls 10.0.120 | unverified | No upstream issue found; MauiWinUIWindow on main still raises the event per message.

## WA-MAUI-0025: Entry cannot be placed directly in a Frame in some MAUI versions

- Status: active
- Added: 2024-06-30 (from git history)
- GnollHack version: 2.0 (201)
- Component: .NET MAUI
- Package: Microsoft.Maui.Controls
- Defective version: unknown (.NET 9 preview workload, `$(MauiVersion)`)
- Platforms: all
- Upstream issue: none
- Upstream status when added: unknown
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackX/GnollHackX/Pages/Game/GamePage.xaml` (`MessageFilterFrame`, `StackLayout` around `MessageFilterEntry`)
- Remove when: An Entry placed directly as a Frame's content works on every platform in a Microsoft.Maui.Controls release GnollHack uses, and Verify removal passes.

**Symptom.** Per the source, "some versions of MAUI do not support placing Entrys in
Frames"; the failure mode is not recorded.

**Cause.** Unknown. `Frame` is obsolete in MAUI 9 and later in favor of `Border`.

**Workaround.** The message filter Entry is wrapped in a StackLayout inside its Frame.

**Verify removal.** Make `MessageFilterEntry` the Frame's direct content in the
`GnollHackX` XAML, and run `makedefsdroid` to regenerate the MAUI XAML. Then:
- Build for Android, iOS and Windows.
- In a game, open the message history filter, type a search, and confirm the Entry shows,
  takes focus and filters the messages on each platform.

Checks:
- 2026-10-23 | Microsoft.Maui.Controls 10.0.120 | unverified | No upstream issue found.

## WA-MAUI-0026: Design-time builds corrupt the Resizetizer cache during Publish

- Status: active
- Added: 2026-08-22 (from git history)
- GnollHack version: 3.11 (411)
- Component: .NET MAUI
- Package: Microsoft.Maui.Controls, Microsoft.Maui.Resizetizer
- Defective version: Microsoft.Maui.Controls 10.0.100 (10.0.51 for iOS)
- Platforms: all
- Upstream issue: https://github.com/dotnet/maui/issues/37748
- Upstream status when added: unreported
- Upstream status: reported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/GnollHackM.csproj` (`PropertyGroup` with `Condition="'$(DesignTimeBuild)' == 'true'"`)
- Remove when: dotnet/maui#37748 is fixed in a Microsoft.Maui.Controls release GnollHack uses, and Verify removal passes.

**Symptom.** Builds and publishes from Visual Studio fail at random in the image, splash
screen and font processing steps, or package stale generated resources.

**Cause.** The Resizetizer targets have no guard against `$(DesignTimeBuild)`, so the
background design-time builds Visual Studio runs for IntelliSense execute the whole
Resizetizer pipeline in the same `obj` directory, concurrently with real builds, and
overwrite its inputs, stamps and generated files. The issue was filed the same day, after
the workaround.

**Workaround.** In design-time builds, `DisableResizetizer` is set and
`EnableMauiImageProcessing` and `EnableMauiSplashScreenProcessing` are false.

**Verify removal.** Remove the design-time `PropertyGroup`. Then:
- In Visual Studio, publish GnollHackM for Windows (MSIX) and archive for Android several
  times in a row while editing XAML and C# files so design-time builds run.
- Confirm every publish succeeds with the correct icons and splash screen.

Checks:
- 2026-10-23 | Microsoft.Maui.Controls 10.0.120 | still-needed | Issue open; the Resizetizer targets on main still have no DesignTimeBuild guard.

## WA-MAUI-0027: EvaluateJavaScriptAsync wraps scripts in eval, which a Content Security Policy refuses

- Status: active
- Added: 2026-10-20 (from git history)
- GnollHack version: 3.28 (428)
- Component: .NET MAUI
- Package: Microsoft.Maui.Controls
- Defective version: Microsoft.Maui.Controls 11.0.0-rc.2.26505.5
- Platforms: Linux, Windows (Avalonia)
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/Platforms/Avalonia/GHAvaloniaOverseerHost.cs` (`NativeWebViewHost.InvokeScriptAsync`); `win/win32/xpl/GnollHackX/GnollHackX/Pages/Game/OverseerPage.xaml.cs` (`OverseerPage.SendToolResponse`); `win/win32/xpl/GnollHackX/GnollHackX/Pages/Game/OverseerPage.xaml.cs` (`OverseerPage.SendPickedFilesToJs`)
- Remove when: `WebView.EvaluateJavaScriptAsync` runs scripts without `eval` in a Microsoft.Maui.Controls release GnollHack's Avalonia flavor uses, and Verify removal passes.

**Symptom.** On the Avalonia flavor, tool responses and picked files sent to the Overseer
page never arrive: WebKitGTK refuses the script under the page's Content Security Policy,
which does not allow `unsafe-eval`.

**Cause.** On every platform except Android, MAUI's `WebView.EvaluateJavaScriptAsync`
escapes the script and wraps it in `eval(...)` (with `JSON.stringify` except on WinUI)
before handing it to the platform, so a page whose CSP forbids eval refuses it. The
wrapper is unchanged on MAUI's main branch.

**Workaround.** Under Avalonia, `OverseerPage` sends its scripts through
`NativeWebViewHost.InvokeScriptAsync`, which calls Avalonia's `NativeWebView.InvokeScript`
directly, without MAUI's wrapper. The MAUI flavors still use `EvaluateJavaScriptAsync`.

**Verify removal.** In `SendToolResponse` and `SendPickedFilesToJs`, use
`DisplayWebView.EvaluateJavaScriptAsync` under Avalonia too. Then:
- Build the Avalonia flavor and run it on Linux.
- Open the Overseer, let it call a tool and attach a file, and confirm the page receives
  both.

Checks:
- 2026-10-23 | Microsoft.Maui.Controls 11.0.0-rc.2.26505.5 | still-needed | No upstream issue found; WebView.cs on main still wraps scripts in eval.

## WA-MAUI-0028: FormattedText lines get uneven heights when spans mix fonts

- Status: active
- Added: 2026-07-12 (from git history)
- GnollHack version: 3.2 (395)
- Component: .NET MAUI
- Package: Microsoft.Maui.Controls
- Defective version: Microsoft.Maui.Controls 10.0.71 (iOS: 10.0.51)
- Platforms: Android, iOS
- Upstream issue: https://github.com/dotnet/maui/issues/36534
- Upstream status when added: unknown
- Upstream status: reported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/App.xaml.cs` (`uniformLineHeightHandler`)
- Remove when: MAUI ships uniform line heights for mixed-font FormattedText (dotnet/maui#36534), GnollHack adopts that release, and Verify removal passes.

**Symptom.** A Label whose FormattedText mixes fonts, such as body text with inline
monospace code, shows visibly uneven spacing: lines that contain the taller font are
taller than the others.

**Cause.** CoreText on iOS and StaticLayout on Android size each line from the tallest
font on it, and MAUI offers no way to set a uniform absolute line height for
FormattedText. MAUI's own `MapLineHeight` also applies a per-line relative multiplier
that overrides an absolute height set afterwards. Upstream tracks this as an open
proposal.

**Workaround.** `uniformLineHeightHandler` post-processes the platform text after MAUI
sets it: on iOS it sets equal minimum and maximum line heights on every paragraph style,
and on Android it disables font padding and applies an absolute line height span. It is
appended to both the `FormattedText` and the `LineHeight` mappings so that MAUI's
`MapLineHeight` cannot undo it.

**Verify removal.** On a MAUI release that implements the proposal:
- Remove the two `AppendToMapping` registrations of `uniformLineHeightHandler`.
- Build for Android and iOS, and open a text window whose lines mix the body font with
  the monospace font.
- Every line must have the same height.

Checks:
- 2026-10-23 | Microsoft.Maui.Controls 10.0.120 | still-needed | dotnet/maui#36534 is open (proposal/open).
