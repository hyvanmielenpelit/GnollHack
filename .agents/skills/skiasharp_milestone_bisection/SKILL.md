---
name: skiasharp_milestone_bisection
description: Bisection workflow to compile and test various native Skia milestones using a local C# source reference build in GnollHack, bypassing version compatibility checks, restoring missing native dependencies, and cherry-picking custom C API functions.
---

# Bisection of Skia Milestones using Local Source Reference Builds

This skill describes the workflow to bisect rendering performance regressions, crashes, or bugs by building different native Skia milestone versions (e.g., m133, m140, m144) and integration-testing them inside GnollHack using a local source-build overlay.

---

## 🏗️ Workflow Overview

To test an intermediate milestone (e.g., m133) using the local `SkiaSharp` source tree, you must coordinate three layers:

```
[Upstream Skia Commit (mXXX)] + [Custom C API Commits]
           │
           ▼ (Compiled by ninja)
[libSkiaSharp.dll (mXXX)] + [SkiaSharp.Views.WinUI.Native.dll]
           │
           ▼ (Referenced & Bypassed Version Check)
[SkiaSharp.dll (Local C#)] + [libEGL.dll / libGLESv2.dll (ANGLE)]
           │
           ▼ (Deployed to output folder)
[GnollHackM (WinUI 3 App)]
```

---

## 🛠️ Step-by-Step Implementation

### Step 1: Check Out the Target Skia Milestone
We must use a branch or merge commit in the `externals/skia` submodule that contains the C API bindings wrapper directory (`src/c/`).
1. Navigate to the submodule:
   ```powershell
   cd c:\repos\SkiaSharp\externals\skia
   ```
2. Checkout the desired milestone commit (see [Milestone Intricacies](#-milestone-specific-intricacies) for specific commits).
3. Synchronize third-party dependencies:
   ```powershell
   python tools/git-sync-deps
   ```

### Step 2: Restore C API Files & Configuration
If using older milestones, the C API directory (`src/c/` and `include/c/`) and build configuration files may need to be restored from a newer milestone reference commit (such as `280ec21ada` from m147):
```powershell
# Restore C API shims
git checkout 280ec21ada -- src/c/
git checkout 280ec21ada -- include/c/
# Restore build configurations
git checkout 280ec21ada -- gn/BUILDCONFIG.gn
git checkout 280ec21ada -- gn/toolchain/BUILD.gn
```

### Step 3: Build the Custom Native DLL
Use the local compiler script to build `libSkiaSharp.dll`:
```powershell
cd c:\repos\SkiaSharp
python scratch/build_skia_native.py
```
This builds both `libSkiaSharp.dll` and `libSkiaSharp.pdb` and puts them in `c:\repos\SkiaSharp\output\native\windows\x64\`.

Also rebuild the WinUI native projections solution:
1. Open and build `native/winui/SkiaSharp.Views.WinUI.Native.sln` in **Release** configuration.
2. Ensure the output is generated at `output/native/winui/any/SkiaSharp.Views.WinUI.Native.dll`.

### Step 4: Bypass Version Compatibility Gate
The local C# bindings expect native milestone `150.0`/`151.0` and will throw `InvalidOperationException` when they detect the native DLL has version `133.0`, `140.0`, or `144.0`.

1. Open `binding/SkiaSharp/SkiaSharpVersion.cs`.
2. Modify `CheckNativeLibraryCompatible` to return `true` immediately to disable checking:
   ```csharp
   internal static bool CheckNativeLibraryCompatible (Version minSupported, Version current, bool throwIfIncompatible = false)
   {
       return true;
   }
   ```

### Step 5: Compile Local C# Assemblies (Important: MauiVersion Sync)
When building the C# projects, you MUST match the `Microsoft.Maui.Controls` package version between `SkiaSharp` and `GnollHackM`.
If they do not match, the build will fail with a `Duplicate Entry` resource error (`PRI175`/`PRI277`) for `TabbedPageStyle.xbf`.
Evaluate the version in `GnollHackM.csproj` (e.g. `10.0.71`) and pass it as a parameter:
```powershell
cd c:\repos\SkiaSharp
# Debug builds
dotnet build binding/SkiaSharp/SkiaSharp.csproj -c Debug /p:TargetFrameworks=net10.0-windows10.0.19041.0 /p:TargetFramework=net10.0-windows10.0.19041.0
dotnet build source/SkiaSharp.Views.Maui/SkiaSharp.Views.Maui.Core/SkiaSharp.Views.Maui.Core.csproj -c Debug /p:TargetFrameworks=net10.0-windows10.0.19041.0 /p:TargetFramework=net10.0-windows10.0.19041.0 /p:MauiVersion=10.0.71
dotnet build source/SkiaSharp.Views.Maui/SkiaSharp.Views.Maui.Controls/SkiaSharp.Views.Maui.Controls.csproj -c Debug /p:TargetFrameworks=net10.0-windows10.0.19041.0 /p:TargetFramework=net10.0-windows10.0.19041.0 /p:MauiVersion=10.0.71
```

### Step 6: Deploy Native Libraries and ANGLE Dependencies
Configure a post-build target in the consumer project (`win/win32/xpl/GnollHackM/GnollHackM.csproj`):
```xml
<Target Name="CopyCustomSkiaSharpNative" AfterTargets="Build" Condition="$(DefineConstants.Contains('SKIASHARP_SOURCE')) AND '$(TargetFramework)'=='net10.0-windows10.0.19041.0'">
    <Copy SourceFiles="C:\repos\SkiaSharp\output\native\windows\x64\libSkiaSharp.dll" DestinationFolder="$(OutputPath)" SkipUnchangedFiles="true" />
    <Copy SourceFiles="C:\repos\SkiaSharp\output\native\winui\any\SkiaSharp.Views.WinUI.Native.dll" DestinationFolder="$(OutputPath)" SkipUnchangedFiles="true" />
    <Copy SourceFiles="$(UserProfile)\.nuget\packages\skiasharp.nativeassets.winui\3.119.1\runtimes\win-x64\native\libEGL.dll" DestinationFolder="$(OutputPath)" SkipUnchangedFiles="true" />
    <Copy SourceFiles="$(UserProfile)\.nuget\packages\skiasharp.nativeassets.winui\3.119.1\runtimes\win-x64\native\libGLESv2.dll" DestinationFolder="$(OutputPath)" SkipUnchangedFiles="true" />
</Target>
```

### Step 7: Clean and Rebuild GnollHackM
Terminate any running instances of the game and background MSBuild worker nodes to prevent locked file errors.
Since resources can be cached, do a recursive clean of `bin` and `obj` across all sibling projects:
```powershell
Stop-Process -Name "GnollHackM" -Force -ErrorAction SilentlyContinue
Stop-Process -Name "MSBuild" -Force -ErrorAction SilentlyContinue

# Recursive Clean
Get-ChildItem -Path c:\hmp\GnollHack\win\win32\xpl\ -Directory -Recurse -Filter "bin" -ErrorAction SilentlyContinue | ForEach-Object { Remove-Item $_.FullName -Recurse -Force -ErrorAction SilentlyContinue }
Get-ChildItem -Path c:\hmp\GnollHack\win\win32\xpl\ -Directory -Recurse -Filter "obj" -ErrorAction SilentlyContinue | ForEach-Object { Remove-Item $_.FullName -Recurse -Force -ErrorAction SilentlyContinue }

# Rebuild with Matched MauiVersion
dotnet build win/win32/xpl/GnollHackM/GnollHackM.csproj -c Debug -f net10.0-windows10.0.19041.0 /p:MauiVersion=10.0.71
```

---

## 🎯 Milestone-Specific Intricacies

When compiling individual milestones, various C++ compiler errors will occur due to upstream API signature changes between the Skia engine and the SkiaSharp C API shims.

### 📍 Milestone 133 (m133)
* **Submodule Commit**: `fb981fd71a`
* **Intricacies**:
  * Requires cherry-picking commit `f5345469d1` to supply the startup-critical API `sk_fontmgr_legacy_create_typeface`.
  * Older shims added in m147 that do not exist or compile in m133 must be commented out or stubbed in the restored `src/c/` C API wrappers.

### 📍 Milestone 140 (m140)
* **Submodule Commit**: `2bd8be83fe91aef59b8979e2a6d71b8ee2ea34ea`
* **Intricacies**:
  * **Shader Gradients**: Revert `sk_shader.cpp` to the m140 gradient-compatible version to match m140's `SkGradientShader::MakeLinear`/`MakeRadial` signatures.
  * **Font breakText**: Modify `SkFont.h` and `SkFont.cpp` to allow `breakText()` to accept the `SkPaint` parameter.
  * **XPS Document**: In `sk_document.cpp`, verify that the parameter signature of `SkXPS::MakeDocument` takes `SkWStream*`.

### 📍 Milestone 144 (m144)
* **Submodule Commit**: `c34bb09814208ea4930883cb6291d83948ce0217`
* **Intricacies**:
  * **Color Types**: Comment out the `R16` color types (`kR16_float_SkColorType`) in `include/c/sk_types.h` and `src/c/sk_enums.cpp` since m144 did not support them.
  * **Codec Options**: Comment out `fMaxDecodeMemory` in `sk_codec_options_t` inside `include/c/sk_types.h` to prevent static size assert failures.
  * **XPS Document**: In `src/c/sk_document.cpp`, change the `SkXPS::MakeDocument` call signature to exclude the XPS metadata struct parameter.
  * **Path Builder**: In `src/c/sk_pathbuilder.cpp`, revert the signature calls for `rMoveTo`/`rArcTo`.
  * **Shader Gradients**: In `src/c/sk_shader.cpp`, use the `SkColor4f::FromColor` helper method instead of the missing `SkColorColor4f` method.
  * **Reference Count**: Add `getRefCount()` definitions to both `SkRefCntBase` and `SkNVRefCnt` in `include/core/SkRefCnt.h`.
  * **Text Blob Bounds**: Modify `allocRunRSXform` in `include/core/SkTextBlob.h` and `src/core/SkTextBlob.cpp` to take optional bounds pointer arguments.
  * **UTF-8 Sentinel Check**: In `src/core/SkFontPriv.h` under `CountTextElements`, add range checking to prevent negative text lengths:
    ```csharp
    if (byteLength < 0) return 0;
    ```

### 📍 Milestone 145 (m145)
* **Submodule Commit**: `f2bc5d570a`
* **Intricacies**:
  * **Build Configurations**: Keep `BUILD.gn`, `gn/opts.gni`, and `gn/shared_sources.gni` at the m145 upstream versions, and only append `SkiaSharp` and `HarfBuzzSharp` targets to the end of `BUILD.gn`. Revert `gn/gpu.gni` to the m145 version as it defines the required `skia_direct3d_sources`.
  * **Custom SkFont shims**: Do NOT restore the entire m147 version of `SkFont.h` and `SkFont.cpp`. Keep the m145 versions and cleanly splice the custom `breakText` method declarations/implementations.
  * **Color Types**: Comment out `R16_FLOAT_SK_COLORTYPE` in `include/c/sk_types.h` and `src/c/sk_enums.cpp`.
  * **Reference Count & Text Blob Bounds**: Apply the same adjustments as m144.

### 📍 Milestone 146 (m146)
* **Submodule Commit**: `34769028da`
* **Intricacies**:
  * **Build Configurations**: Milestone 146 lacks various variables and optimized source files that were introduced in milestone 147 (such as `skia_use_partition_alloc` in `gn/skia.gni` and optimized sources like `ml3`/`ml4` in `gn/opts.gni`/`gn/shared_sources.gni`). To resolve this, keep `BUILD.gn`, `gn/opts.gni`, and `gn/shared_sources.gni` at the m146 upstream versions, and only append `SkiaSharp` and `HarfBuzzSharp` targets to the end of `BUILD.gn`. Revert `gn/gpu.gni` to the m146 version as it defines the required `skia_direct3d_sources`.
  * **Custom SkFont shims**: Do NOT restore the entire m147 version of `SkFont.h` and `SkFont.cpp`, as they reference a missing header `SkStrikeRef.h` (introduced in m147). Instead, keep the m146 versions and cleanly splice the custom `breakText` method declarations/implementations.
  * **Color Types**: Comment out `R16_FLOAT_SK_COLORTYPE` in `include/c/sk_types.h` and `src/c/sk_enums.cpp` since the m146 engine does not define it.
  * **Reference Count & Text Blob Bounds**: Apply the same adjustments as m144.

