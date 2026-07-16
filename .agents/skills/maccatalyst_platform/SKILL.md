---
name: maccatalyst_platform
description: Guide to the Mac Catalyst platform target in GnollHack. Covers the relationship between iOS and MacCatalyst in .NET MAUI, csproj configuration, native library setup (static C engine + vtool-patched FMOD dylibs), platform files, conditional compilation, desktop vs mobile asset selection, and debugging on Mac.
---

# Mac Catalyst Platform Guide

This skill covers everything needed to work with GnollHack's Mac Catalyst target — how it relates to iOS, how native libraries are configured, FMOD integration differences, and platform-specific code paths.

## How MacCatalyst Relates to iOS

Mac Catalyst is UIKit-based: it runs iPad UIKit code natively on macOS, not through an iOS compatibility shim like "Designed for iPad." The app is recompiled for the `macabi` target triple, producing a true macOS binary that uses UIKit instead of AppKit.

In .NET MAUI, **both iOS and MacCatalyst define `__IOS__`**. To distinguish between them at compile time and runtime:

- **Compile time**: Use `#if MACCATALYST` (MacCatalyst only) or `#if IOS` (iOS only, NOT defined for MacCatalyst)
- **Runtime**: Use `DeviceInfo.Platform == DevicePlatform.MacCatalyst`
- **GnollHack helper**: [`GHApp.IsMacCatalyst`](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX/GHApp.cs#L2915) provides a cached check

MacCatalyst is treated as a **desktop** platform in GnollHack:

```csharp
IsDesktop = IsWindows || IsMacCatalyst
```

Platform-specific code from `GnollHackX.iOS/` (e.g., `PlatformServiceiOS.cs`, `GHUIApplication.cs`) is file-linked into **both** iOS and MacCatalyst builds via shared `<Compile Include>` ItemGroups in the csproj. There are no separate MacCatalyst platform service files — the iOS files serve both targets.

## csproj Configuration

All configuration lives in [GnollHackM.csproj](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackM/GnollHackM.csproj).

### Target Framework

- TFM: `net10.0-maccatalyst` (line 7)
- `SupportedOSPlatformVersion`: 13.1 (line 32)

### PropertyGroup

The MacCatalyst-specific `<PropertyGroup>` contains:

- `RuntimeIdentifiers`: `maccatalyst-arm64;maccatalyst-x64`
- `DefineConstants`: adds `FMOD_GNH_MACCATALYST`
- `ProvisioningType`: `Manual`

### Code Signing

Separate `Debug` and `Release` PropertyGroups conditioned on `$(TargetFramework.Contains('maccatalyst'))` define:

- `CodesignKey` — uses the same Apple Development certificate as iOS
- `CodesignProvision` — requires a **separate** Mac Catalyst provisioning profile (different from iOS)
- `CodesignEntitlements` — points to `Platforms/MacCatalyst/Entitlements.plist`

### Asset Conditions

`MauiIcon` and `MauiFont` elements already have MacCatalyst conditions (lines 135, 145).

## Native Libraries

Native libraries live in `Platforms/MacCatalyst/libs/`.

### C Engine

- **File**: `libgnollhackmac.a`
- **Format**: Universal static library (arm64 + x86_64)
- **Target**: Compiled for `*-apple-ios14.0-macabi` target triple
- **P/Invoke**: Uses `__Internal` (same as iOS — statically linked)
- **NativeReference**: `Kind=Static`, `ForceLoad=True`, `IsCxx=True`

### FMOD Libraries

- **Files**: `libfmod_maccatalyst.dylib`, `libfmodstudio_maccatalyst.dylib`
- **Origin**: macOS FMOD dylibs patched with `vtool` to change Mach-O platform tag from `macos` to `maccatalyst` (see the `vtool_fmod_patching` skill)
- **P/Invoke**: Dynamic linking via `[DllImport("libfmod_maccatalyst")]` / `[DllImport("libfmodstudio_maccatalyst")]`
- **NativeReference**: `Kind=Dynamic`, `Frameworks=AudioToolbox CoreAudio AudioUnit AVFoundation`

### Comparison: iOS vs MacCatalyst Linking

| Component | iOS | MacCatalyst |
|-----------|-----|-------------|
| C engine | `__Internal` (static) | `__Internal` (static) |
| FMOD core | `__Internal` (static `.a`) | `libfmod_maccatalyst` (dynamic `.dylib`) |
| FMOD studio | `__Internal` (static `.a`) | `libfmodstudio_maccatalyst` (dynamic `.dylib`) |

## FMOD Integration Differences

FMOD library name resolution is controlled by compile-time constants:

- **iOS** defines `FMOD_GNH_IOS` → `dll = "__Internal"` (static linking)
- **MacCatalyst** defines `FMOD_GNH_MACCATALYST` → `dll = "libfmod_maccatalyst"` / `dll = "libfmodstudio_maccatalyst"` (dynamic linking)

These constants are set in:
- [fmod.cs:L35-39](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX.FMOD/fmod.cs#L35-L39)
- [fmod_studio.cs:L18-22](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX.FMOD/fmod_studio.cs#L18-L22)

> [!IMPORTANT]
> FMOD version must match exactly across all platforms: **v2.03.14** (`0x00020314`). A mismatch will cause runtime crashes or API errors.

`FmodService.cs` uses `#if __IOS__` which covers MacCatalyst too — no MacCatalyst-specific changes are needed in that file.

## Sound Banks: Desktop vs Mobile

MacCatalyst uses **desktop banks** (smaller files, higher compression, Vorbis codec), while iOS uses **mobile banks** (larger files, different codec):

| Platform | Bank Source Path | Codec |
|----------|-----------------|-------|
| iOS | `Platforms\iOS\banks\` | Mobile (FADPCM) |
| MacCatalyst | `Platforms\Windows\banks\` | Desktop (Vorbis) |

Bank file names are identical across platforms — game code loads them by the same logical names.

In the csproj:
- iOS banks are included as `BundleResource` from the iOS path
- MacCatalyst banks are included as `BundleResource` from the Windows path

## Platform Files

Files in `Platforms/MacCatalyst/`:

| File | Purpose |
|------|---------|
| `AppDelegate.cs` | Mirrors iOS: GC initialization, black background |
| `Program.cs` | Mirrors iOS: `LibTest()` P/Invoke call, `GHUIApplication` startup |
| `Info.plist` | UIDeviceFamily `[2, 6]`, arm64 arch, all orientations, MinimumOSVersion 14.0 |
| `Entitlements.plist` | App Sandbox enabled + network client capability |

## Keyboard Input

[GHUIApplication.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX.iOS/GHUIApplication.cs) (file-linked from `GnollHackX.iOS/`) handles keyboard input for both iOS and MacCatalyst:

- Uses `OperatingSystem.IsMacCatalystVersionAtLeast(15, 0)` for `WantsPriorityOverSystemBehavior` (prevents system key bindings from intercepting game input)
- Handles `Cmd+Q` for Mac quit behavior
- Physical keyboard support is shared between both platforms

## Key Conditional Compilation Symbols

| Symbol | Defined For | Usage |
|--------|-------------|-------|
| `__IOS__` | iOS **and** MacCatalyst | Shared UIKit code |
| `MACCATALYST` | MacCatalyst only | Mac-specific behavior |
| `IOS` | iOS only (NOT MacCatalyst) | iOS-only code paths |
| `FMOD_GNH_IOS` | iOS | FMOD static linking (`__Internal`) |
| `FMOD_GNH_MACCATALYST` | MacCatalyst | FMOD dynamic linking (named dylibs) |
| `GNH_MAUI` | All MAUI platforms | General MAUI code |

> [!WARNING]
> `__IOS__` is defined for MacCatalyst. Never use `#if __IOS__` to guard iOS-only code — use `#if IOS` instead. Similarly, `#if !IOS` does NOT exclude MacCatalyst; it includes it.

## Build Commands

```bash
# Debug build for MacCatalyst
dotnet build win/win32/xpl/GnollHackM/GnollHackM.csproj -c Debug -f net10.0-maccatalyst

# Release build
dotnet build win/win32/xpl/GnollHackM/GnollHackM.csproj -c Release -f net10.0-maccatalyst

# Publish
dotnet publish win/win32/xpl/GnollHackM/GnollHackM.csproj -c Release -f net10.0-maccatalyst
```

> [!NOTE]
> MacCatalyst builds must be run on a Mac with Xcode installed. The `dotnet` CLI will invoke the Apple build toolchain for code signing and bundling.

## Critical Files Reference

| File | Purpose |
|------|---------|
| [GnollHackM.csproj](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackM/GnollHackM.csproj) | All TFM, NativeReference, signing, and bank configuration |
| [GHApp.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX/GHApp.cs) | `IsMacCatalyst`, `IsDesktop` properties |
| [fmod.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX.FMOD/fmod.cs) | FMOD core library name selection |
| [fmod_studio.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX.FMOD/fmod_studio.cs) | FMOD studio library name selection |
| [FmodService.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX.FMOD/FmodService.cs) | FMOD initialization (shared iOS/MacCatalyst) |
| [GHUIApplication.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX.iOS/GHUIApplication.cs) | Keyboard handling (file-linked, both iOS & MacCatalyst) |
| [PlatformServiceiOS.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX.iOS/PlatformServiceiOS.cs) | Platform service (file-linked, both iOS & MacCatalyst) |
| `Platforms/MacCatalyst/AppDelegate.cs` | MacCatalyst app delegate |
| `Platforms/MacCatalyst/Program.cs` | MacCatalyst entry point |
| `Platforms/MacCatalyst/Info.plist` | MacCatalyst app manifest |
| `Platforms/MacCatalyst/Entitlements.plist` | Sandbox and capabilities |
| `Platforms/MacCatalyst/libs/` | Native libraries directory |
