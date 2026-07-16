---
name: vtool_fmod_patching
description: How to patch FMOD macOS dylibs for Mac Catalyst using Apple's vtool. Covers why iOS and macOS FMOD libraries don't work on MacCatalyst, how vtool changes the Mach-O platform tag, the complete patching pipeline, install_name_tool usage, re-signing, and verification.
---

# vtool FMOD Patching Guide

This skill documents how to patch FMOD macOS dynamic libraries for use on Mac Catalyst using Apple's `vtool`, and why this workaround is necessary.

## Why Patching is Needed

Mac Catalyst requires libraries with a `maccatalyst` platform tag in the Mach-O `LC_BUILD_VERSION` load command. The linker and `dyld` enforce strict platform matching:

- **iOS `.a` files** have platform tag `ios` → linker rejects them for MacCatalyst (`macabi` target triple)
- **macOS `.dylib` files** have platform tag `macos` → linker/dyld also rejects them

FMOD does **not** officially support Mac Catalyst. They ship:
- iOS SDK: static libraries (`.a`) with `ios` platform
- macOS SDK: dynamic libraries (`.dylib`) with `macos` platform

**Solution**: Apple's `vtool` can rewrite the Mach-O platform tag from `macos` to `maccatalyst` without recompilation. This works because the FMOD macOS dylibs depend only on system frameworks available on Mac Catalyst:
- `AudioToolbox`
- `CoreAudio`
- `AudioUnit`
- `AVFoundation`

> [!WARNING]
> This is an unsupported workaround. FMOD may break compatibility in future versions. If FMOD releases official Mac Catalyst support, switch to the official libraries immediately.

## How "Designed for iPad" Differs

Understanding the distinction is critical for knowing why we can't just use iOS FMOD libraries:

| Approach | Binary Type | FMOD Strategy |
|----------|-------------|---------------|
| **Designed for iPad** | Actual iOS binary, runs unmodified on Apple Silicon | iOS `.a` files work — the entire binary is iOS |
| **Mac Catalyst** | Recompiled for `macabi` target triple | Strict linker checking — needs `maccatalyst` platform tag |

"Designed for iPad" runs via App Store / TestFlight on Apple Silicon Macs, where macOS provides iOS framework shims at `/System/iOSSupport/System/Library/Frameworks/`. The binary is never recompiled.

Mac Catalyst recompiles everything for the `*-apple-ios14.0-macabi` target, which triggers strict Mach-O platform validation in the linker and runtime loader.

## Prerequisites

- **FMOD macOS SDK** — same version as the iOS build (currently **v2.03.14**, `0x00020314`)
- **Xcode command line tools** providing: `xcrun`, `vtool`, `install_name_tool`, `codesign`, `otool`, `lipo`
- Must be run on a **Mac**

> [!IMPORTANT]
> The FMOD SDK version must match **exactly** between the iOS and macOS SDKs. Even minor version mismatches can cause runtime crashes or API incompatibilities.

## Patching Pipeline

### Step 1: Locate FMOD macOS SDK Libraries

After extracting the FMOD macOS SDK, the libraries are at:

```
<fmod_sdk>/api/core/lib/libfmod.dylib
<fmod_sdk>/api/studio/lib/libfmodstudio.dylib
```

### Step 2: Verify Current Platform Tag

```bash
xcrun vtool -show libfmod.dylib
```

Expected output includes: `platform: MACOS`

### Step 3: Patch Platform Tag

```bash
# Patch FMOD core
xcrun vtool \
  -set-build-version maccatalyst 14.0 17.0 \
  -set-build-tool maccatalyst 4 19.0.0 \
  -output libfmod_maccatalyst.dylib \
  libfmod.dylib

# Patch FMOD studio
xcrun vtool \
  -set-build-version maccatalyst 14.0 17.0 \
  -set-build-tool maccatalyst 4 19.0.0 \
  -output libfmodstudio_maccatalyst.dylib \
  libfmodstudio.dylib
```

Parameters:
- `maccatalyst 14.0 17.0` — platform, minimum deployment target, SDK version
- `-set-build-tool maccatalyst 4 19.0.0` — tool type 4 (LD linker), tool version

### Step 4: Fix Install Names

The patched dylibs retain their original install names. Update them to match the new filenames:

```bash
install_name_tool -id @rpath/libfmod_maccatalyst.dylib libfmod_maccatalyst.dylib
install_name_tool -id @rpath/libfmodstudio_maccatalyst.dylib libfmodstudio_maccatalyst.dylib
```

### Step 5: Update Cross-References

`libfmodstudio` links against `libfmod`. Check and update the reference:

```bash
# Check current references
otool -L libfmodstudio_maccatalyst.dylib

# If it still references the old libfmod name, fix it:
install_name_tool \
  -change @rpath/libfmod.dylib @rpath/libfmod_maccatalyst.dylib \
  libfmodstudio_maccatalyst.dylib
```

### Step 6: Re-sign

`vtool` invalidates the existing code signature. Ad-hoc re-sign:

```bash
codesign --force --sign - libfmod_maccatalyst.dylib
codesign --force --sign - libfmodstudio_maccatalyst.dylib
```

### Step 7: Verify

```bash
# Check platform tag
xcrun vtool -show libfmod_maccatalyst.dylib
# Expected: platform: MACCATALYST

# Check install name
otool -D libfmod_maccatalyst.dylib
# Expected: @rpath/libfmod_maccatalyst.dylib

# Check architectures (should be universal)
lipo -info libfmod_maccatalyst.dylib
# Expected: arm64 x86_64

# Check dependencies
otool -L libfmodstudio_maccatalyst.dylib
# Should reference @rpath/libfmod_maccatalyst.dylib
```

## Automation Script

A convenience script is provided at:

```
win/win32/xpl/gnollhackmac/patch_fmod_dylibs.sh
```

Usage:

```bash
./patch_fmod_dylibs.sh <fmod_macos_sdk_dir> <output_dir>
```

The script performs all steps above (verify, patch, rename, fix references, re-sign, verify) and copies the patched dylibs to the output directory.

## Integration with .NET MAUI

After patching, integrate the dylibs into the GnollHack build:

1. **Place** patched dylibs in `Platforms/MacCatalyst/libs/`
2. **Reference** in [GnollHackM.csproj](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackM/GnollHackM.csproj) via `<NativeReference>`:

```xml
<NativeReference Include="Platforms\MacCatalyst\libs\libfmod_maccatalyst.dylib"
                 Condition="$(TargetFramework.Contains('maccatalyst'))">
  <Kind>Dynamic</Kind>
  <Frameworks>AudioToolbox CoreAudio AudioUnit AVFoundation</Frameworks>
</NativeReference>
```

3. **P/Invoke** in C# uses the library name (not `__Internal`):

```csharp
#if FMOD_GNH_MACCATALYST
    const string dll = "libfmod_maccatalyst";
#endif
```

See [fmod.cs:L35-39](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX.FMOD/fmod.cs#L35-L39) and [fmod_studio.cs:L18-22](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX.FMOD/fmod_studio.cs#L18-L22) for the conditional DllImport constants.

## Troubleshooting

| Problem | Cause | Fix |
|---------|-------|-----|
| `vtool` fails with "not enough space" | Mach-O header padding too small | Unlikely with FMOD dylibs; if encountered, use `ld -headerpad_max_install_names` to rebuild |
| `dyld: library not loaded` | `@rpath` not set correctly or NativeReference missing | Check `otool -l <dylib>` for `LC_RPATH`; verify `<NativeReference Kind="Dynamic">` in csproj |
| `code signature invalid` | vtool or install_name_tool invalidated signature | Re-run `codesign --force --sign - <dylib>` |
| Runtime crash in FMOD | Version mismatch or unexpected framework dependency | Verify version match with `strings <dylib> | grep FMOD`; check `otool -L` for dependencies |
| Linker error: "building for Mac Catalyst but linking in object file built for macOS" | Platform tag not patched | Re-run vtool patching; verify with `xcrun vtool -show` |
| App rejected by App Store | Ad-hoc signature not valid for distribution | Sign with proper Apple Developer certificate during archive build |
