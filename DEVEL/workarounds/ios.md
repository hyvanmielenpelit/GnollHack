# iOS Workarounds

- Component: .NET for iOS, iOS, iPadOS, UIKit, WebKit on Apple, Mac running the iPad app
- Upstream: https://github.com/dotnet/macios, https://feedbackassistant.apple.com
- ID prefix: WA-IOS

Entries follow the schema in [README.md](README.md). Append workarounds before the
`<!-- known-issues -->` line if there is one, otherwise at the end of the file, and known
issues at the end of the file. Never renumber or reuse an ID.

<!-- entries -->

## WA-IOS-0001: Liquid Glass flex pan recognizers lose taps on switches

- Status: active
- Added: 2026-10-23 (from git history)
- GnollHack version: 3.30 (430)
- Component: UIKit
- Package: iOS
- Defective version: iOS 27 (minor version not recorded)
- Platforms: iOS
- Upstream issue: none (report drafted, not filed)
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/Platforms/iOS/FlexGestureSuppressor.cs` (`FlexGestureSuppressor`); `win/win32/xpl/GnollHackX/GnollHackX.iOS/GHUIApplication.cs` (`GHUIApplication.SendEvent`); `win/win32/xpl/GnollHackX/GnollHackX/Pages/MainScreen/SettingsPage.xaml.cs` (`SettingsPage.ContentPage_Appearing`); `win/win32/xpl/GnollHackX/GnollHackX/Pages/MainScreen/SettingsPage.xaml.cs` (`SettingsPage.ContentPage_Disappearing`)
- Remove when: An iOS release no longer arbitrates every touch among the window-level flex recognizers of all glass controls, GnollHack's minimum iOS version includes it, and Verify removal passes.

**Symptom.** On the Settings page, which holds many switches and sliders, quick taps on
switches are lost and touch handling falls behind.

**Cause.** Each Liquid Glass `UISwitch` and `UISlider` registers a private
`_UIFlexInteractionPanGestureRecognizer` on its `UIWindow`. With many such controls in one
window, UIKit arbitrates every touch among all of them.

**Workaround.** While the Settings page is shown, `GHUIApplication.SendEvent` calls
`FlexGestureSuppressor.BeforeEvent`, which disables the window's flex recognizers at each
touch-down, before UIKit dispatches it. They are re-enabled when the page disappears.

**Verify removal.** Remove the `SetActive` calls from `SettingsPage` (the hook in
`SendEvent` is then inert). Then:
- Build for iOS with the current iOS SDK and run on a device with the newest iOS.
- Open Settings and tap several switches quickly, repeatedly.
- Confirm that every tap toggles its switch and the page keeps up with the touches.

Checks:
- 2026-10-23 | iOS 27, workload ios 27.0.10722 | unverified | Apple report drafted, not filed.

## WA-IOS-0003: Mac iPad app window loses key status after modal navigation

- Status: active
- Added: 2026-07-15 (from git history)
- GnollHack version: 3.4 (399)
- Component: iPad apps on Mac
- Package: macOS
- Defective version: unknown
- Platforms: Mac (iPad app)
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackX/GnollHackX.iOS/PlatformServiceiOS.cs` (`PlatformService.EnsureWindowFocus`)
- Remove when: Buttons respond after modal navigation on a Mac running the iPad app without the reactivation, and Verify removal passes.

**Symptom.** On a Mac running the iPad app ("Designed for iPad"), the window can lose
active status after a modal page is pushed, and buttons stop responding to clicks.

**Cause.** Not established. The window loses its key status at the UIKit level and the app
its activation at the macOS level; it is not known whether MAUI's modal presentation or
the iPad-app hosting on macOS is at fault.

**Workaround.** After each modal push, `GHApp.PushModalPageAsync` calls
`EnsureWindowFocus`. On a Mac it makes the key window key and visible again and calls
`NSApplication.activateIgnoringOtherApps:` through `objc_msgSend`. It does nothing on
iPhone and iPad.

**Verify removal.** Make `EnsureWindowFocus` return immediately. Then:
- Build for iOS and run the app on an Apple silicon Mac.
- Open and close several modal pages (Settings, the game menu, a game's menus).
- Confirm that every button responds to the first click after each navigation.

Checks:
- 2026-10-23 | macOS (not re-tested), workload ios 27.0.10722, Microsoft.Maui.Controls 10.0.120 | unverified | Seeded from git history; no upstream report found.

## WA-IOS-0004: Incremental builds ignore Info.plist edits

- Status: active
- Added: 2026-10-23 (from git history)
- GnollHack version: 3.30 (430)
- Component: .NET for iOS
- Package: workload ios
- Defective version: workload ios 27.0.10722 (regression from .NET 9, per the issue)
- Platforms: iOS
- Upstream issue: https://github.com/dotnet/macios/issues/26700
- Upstream status when added: fixed-unreleased
- Upstream status: fixed-unreleased
- Fixed upstream in: https://github.com/dotnet/macios/pull/26718, https://github.com/dotnet/macios/pull/26853
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/GnollHackM.csproj` (`AddInfoPlistToAppManifestInputs`)
- Remove when: GnollHack uses an iOS workload whose release contains dotnet/macios#26718, and Verify removal passes.

**Symptom.** After an edit to `Platforms/iOS/Info.plist`, an incremental iOS build keeps
the old manifest, and the app on the device runs with the previous keys.

**Cause.** The SDK's `_CompileAppManifest` target does not list `Info.plist` among its
`Inputs`, so MSBuild skips it when its other inputs are unchanged.

**Workaround.** The `AddInfoPlistToAppManifestInputs` target runs before
`_CompileAppManifest` and adds `$(AppBundleManifest)`, which `_DetectAppManifest` has set
by then, to `_CompileAppManifestInputFile`.

**Verify removal.** Remove the `AddInfoPlistToAppManifestInputs` target. Then:
- Build for iOS.
- Change a value in `Platforms/iOS/Info.plist`, and build again without cleaning.
- Confirm that the `Info.plist` inside the built app bundle has the new value.

Checks:
- 2026-10-23 | workload ios 27.0.10722 | unverified | Fix merged to main (#26718) and release/11.0.1xx (#26853); no release tag contains either, so the workload in use lacks it.

## WA-IOS-0005: Visual Studio iOS Archive leaves dSYMs on the Mac

- Status: active
- Added: 2026-10-15 (from git history)
- GnollHack version: 3.26 (426)
- Component: .NET for iOS
- Package: workload ios
- Defective version: unknown
- Platforms: iOS
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/GnollHackM.csproj` (`CopyDsymsFromMacForSentry`)
- Remove when: The iOS workload copies the dSYMs to Windows for a Visual Studio Archive build, and Verify removal passes.

**Symptom.** A Release iOS Archive from Visual Studio on Windows leaves the dSYMs on the
paired Mac, so `UploadDebugInfoToSentry` finds no native iOS symbols to upload.

**Cause.** The SDK's `CopyDSYMFromMac` target copies the dSYMs to Windows only when
`BuildIpa` or `CopyAppBundle` is true, which a Visual Studio Archive does not set. The Mac
connection closes before `UploadDebugInfoToSentry` runs.

**Workaround.** The `CopyDsymsFromMacForSentry` target runs before `BeforeDisconnect` on
Release iOS builds with Sentry symbol upload enabled. It zips every `*.dSYM` in
`$(DeviceSpecificOutputPath)` on the Mac, copies the zip to Windows, unzips it into the
same output directory, and deletes the zips.

**Verify removal.** Remove the `CopyDsymsFromMacForSentry` target. Then:
- Make a Release iOS Archive from Visual Studio with `SENTRY` defined and
  `SentryUploadSymbols` true.
- Confirm that `GnollHackM.app.dSYM` is in the output directory on Windows and that the
  Sentry upload includes it.

Checks:
- 2026-10-23 | workload ios 27.0.10722 | still-needed | `CopyDSYMFromMac` on dotnet/macios main still requires `BuildIpa` or `CopyAppBundle`.
