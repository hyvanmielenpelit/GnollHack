---
name: skiasharp_dll_swap
description: Quick diagnostic test for SkiaSharp performance regressions by swapping the native libSkiaSharp.dll from a different version and bypassing the version compatibility check via reflection. Use when you need to quickly test whether a different Skia engine version resolves a rendering performance issue on Windows without rebuilding anything from source.
---

# SkiaSharp DLL Swap (Quick Diagnostic)

## Purpose

Swap the native `libSkiaSharp.dll` from one SkiaSharp version into a build using a different version, to isolate whether a performance or correctness issue is in the **Skia native engine** vs the **C# wrapper layer**. This is a diagnostic test, NOT a production solution.

## Critical Rules

- **This is for diagnostic testing ONLY** — never ship a mismatched DLL.
- When possible, **use matching C# and native versions** (see Recommended Workflow) to avoid both the version check issue AND `EntryPointNotFoundException`.
- The reflection version check bypass **MUST** execute before ANY SkiaSharp type is accessed.
- **Do NOT commit** the reflection bypass or test defines to the main branch.

## Recommended Workflow: Matching C# + Custom Native

The cleanest approach combines matching C# NuGet packages with custom-built native DLLs:

| Test Goal | C# Package (GnollHack NuGet) | Native DLL | Version Check | EPNFE Risk |
|-----------|------------------------------|------------|---------------|------------|
| m119 engine (known good baseline) | v3.119.4 via `SKIASHARP_3119` define | Build from m119 skia commit (see `skiasharp_custom_native` skill) | ✅ Pass naturally (119 ∈ [119,120)) | ✅ None |
| m147 engine (known bad) | v4.147-preview.1.1 (add to csproj) | Build from m147 skia commit | ✅ Pass naturally (147 ∈ [147,148)) | ✅ None |
| Intermediate milestone (e.g., m132) | v3.119.4 via `SKIASHARP_3119` | Build from m132 skia commit | ❌ Fails — use reflection bypass | Low — m132 C API is superset of m119 |

GnollHack already has a `SKIASHARP_3119` conditional toggle in `GnollHackM.csproj` to switch between SkiaSharp versions. Use this to match the C# bindings to the native DLL era.

For building the native DLL from a specific milestone, see the `skiasharp_custom_native` skill.

## Prerequisites

- GnollHack builds and runs on Windows with the current SkiaSharp version.
- The target SkiaSharp NuGet package is in the local NuGet cache (`%USERPROFILE%\.nuget\packages\skiasharp.nativeassets.win32\`).

## Reflection Version Check Bypass

SkiaSharp validates that the native DLL milestone matches the C# assembly's expected range. A mismatched DLL triggers `InvalidOperationException` → `TypeInitializationException` that permanently poisons the `SkiaApi` type.

The bypass sets the `nativeMinimum` field to `Version(0, 0)` via reflection, which causes `CheckNativeLibraryCompatible` to return `true` immediately without inspecting the native DLL.

### Where to Add

Add the bypass at the **very start** of `MauiProgram.CreateMauiApp()`, before `.UseMauiApp<App>()` or any other code that might reference SkiaSharp types:

```csharp
// File: win/win32/xpl/GnollHackM/MauiProgram.cs (or equivalent entry point)

public static MauiApp CreateMauiApp()
{
#if WINDOWS && SKIASHARP_DLL_SWAP_TEST
    // DIAGNOSTIC ONLY: Bypass SkiaSharp native version check for DLL swap testing.
    // This must run before ANY SkiaSharp type is accessed.
    var versionField = typeof(SkiaSharp.SkiaSharpVersion).GetField(
        "nativeMinimum",
        System.Reflection.BindingFlags.NonPublic | System.Reflection.BindingFlags.Static);
    versionField?.SetValue(null, new Version(0, 0));
    System.Diagnostics.Debug.WriteLine("[DLL-SWAP] SkiaSharp version check bypassed");
#endif

    var builder = MauiApp.CreateBuilder();
    // ... rest of the method
}
```

### How It Works

```
SkiaSharpVersion.CheckNativeLibraryCompatible(true)
  → NativeMinimum → nativeMinimum ??= new Version(Milestone, Increment)
  → But we already set nativeMinimum = Version(0, 0)
  → minSupported (0.0) <= Zero (0.0) → return true immediately
  → Version check passes without calling native code
```

### When You Need It

- **NOT needed** when C# package and native DLL are from the same version (e.g., v3.119.4 C# + m119 native)
- **Needed** when testing an intermediate milestone with a mismatched C# package (e.g., v3.119.4 C# + m132 native)

## Steps for Quick NuGet DLL Swap (No Source Build)

If you just want to swap a DLL from an existing NuGet package (no source build):

### Step 1: Build with bypass enabled

```powershell
dotnet build win/win32/xpl/GnollHackM/GnollHackM.csproj -c Debug -f net10.0-windows10.0.19041.0 /p:DefineConstants="SKIASHARP_DLL_SWAP_TEST"
```

### Step 2: Swap the Native DLL

```powershell
$outputDir = Get-ChildItem -Path "win\win32\xpl\GnollHackM\bin\Debug\net10.0-windows10.0.19041.0" -Recurse -Filter "libSkiaSharp.dll" | Select-Object -First 1 -ExpandProperty DirectoryName

$sourceVersion = "3.119.1"  # The known-good version
$sourceDll = "$env:USERPROFILE\.nuget\packages\skiasharp.nativeassets.win32\$sourceVersion\runtimes\win-x64\native\libSkiaSharp.dll"
Copy-Item $sourceDll "$outputDir\libSkiaSharp.dll" -Force

Write-Host "Swapped libSkiaSharp.dll from $sourceVersion into $outputDir"
```

### Step 3: Run and Compare

1. Run the app and open the affected pages (SnapshotPage, SaveTransferPage).
2. Check Debug Output for `[DLL-SWAP]` confirmation message.
3. Compare rendering speed to the original (non-swapped) build.

## Interpreting Results

| Observation | Conclusion |
|-------------|-----------|
| Performance returns to normal | **Skia engine regression confirmed** — proceed with milestone bisection (use `skiasharp_custom_native` skill) |
| Performance unchanged | Issue is in the C# layer or app code, not the native engine |
| `EntryPointNotFoundException` crash | GnollHack uses a new API not present in the old DLL — use matching C# package version instead |

## Available DLL Versions in NuGet Cache

```powershell
Get-ChildItem "$env:USERPROFILE\.nuget\packages\skiasharp.nativeassets.win32" -Directory | Select-Object Name
```

## Cleanup

After testing, revert the changes:
1. Remove the `SKIASHARP_DLL_SWAP_TEST` define.
2. Remove the reflection bypass code (or leave it guarded by the ifdef).
3. Rebuild normally — NuGet restore will replace the swapped DLL.
