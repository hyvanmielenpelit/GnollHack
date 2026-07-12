---
name: skiasharp_milestone_bisection
description: Bisection workflow to compile and test various native Skia milestones using a local C# source reference build in GnollHack, bypassing version compatibility checks, restoring missing native dependencies, and cherry-picking custom C API functions.
---

# Bisection of Skia Milestones using Local Source Reference Builds

This skill describes the workflow to bisect rendering performance regressions, crashes, or bugs by building different native Skia milestone versions (e.g., m133, m140) and integration-testing them inside GnollHack using a local source-build overlay.

---

## 🏗️ Workflow Overview

To test an intermediate milestone (e.g., m133) using the local `SkiaSharp` source tree, you must coordinate three layers:

```
[Upstream Skia Commit (m133)] + [Custom C API Commits]
           │
           ▼ (Compiled by ninja)
[libSkiaSharp.dll (m133)] + [SkiaSharp.Views.WinUI.Native.dll]
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
2. Checkout the desired milestone merge commit (e.g., for chrome/m133, commit `fb981fd71a`):
   ```powershell
   git checkout fb981fd71a
   ```
3. Synchronize third-party dependencies:
   ```powershell
   python tools/git-sync-deps
   ```

---

### Step 2: Cherry-Pick Missing Custom C APIs
Since the local C# codebase is at the head of the branch (expecting the latest C API exports), older milestones will lack custom shims added by the SkiaSharp maintainers. This causes `EntryPointNotFoundException` at startup.

You must cherry-pick these critical commits into your detached HEAD:
- **`sk_fontmgr_legacy_create_typeface`** (Required for `SKTypeface.Default` startup):
  ```powershell
  git cherry-pick f5345469d1
  ```
- **Other common missing shims** (e.g., `sk_paint_compute_fast_bounds`, `sk_colorfilter_new_overdraw`):
  Search the history using `git log fb981fd71a..origin/skiasharp --oneline -- src/c/` and cherry-pick if the C# startup/runtime crashes on them.

---

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

---

### Step 4: Bypass Version Compatibility Gate
The local C# bindings expect native milestone `150.0`/`151.0` and will throw `InvalidOperationException` when they detect the native DLL has version `133.0`.

1. Open `binding/SkiaSharp/SkiaSharpVersion.cs`.
2. Modify `CheckNativeLibraryCompatible` to return `true` immediately to disable checking:
   ```csharp
   internal static bool CheckNativeLibraryCompatible (Version minSupported, Version current, bool throwIfIncompatible = false)
   {
       return true;
   }
   ```
3. In `binding/SkiaSharp/SKCanvas.cs`, if `SetMatrix` obsolete warnings are treated as errors and block the compilation of the consumer project (e.g., GnollHack's `GamePage.xaml.cs`), downgrade the obsolete attribute check from error to warning.

---

### Step 5: Compile Local C# Assemblies
Build both **Debug** and **Release** configurations of the C# projects so they are available to the consumer regardless of configuration:
```powershell
cd c:\repos\SkiaSharp
# Debug builds
dotnet build binding/SkiaSharp/SkiaSharp.csproj -c Debug /p:TargetFrameworks=net10.0-windows10.0.19041.0 /p:TargetFramework=net10.0-windows10.0.19041.0
dotnet build source/SkiaSharp.Views.Maui/SkiaSharp.Views.Maui.Core/SkiaSharp.Views.Maui.Core.csproj -c Debug /p:TargetFrameworks=net10.0-windows10.0.19041.0 /p:TargetFramework=net10.0-windows10.0.19041.0 /p:MauiVersion=10.0.71
dotnet build source/SkiaSharp.Views.Maui/SkiaSharp.Views.Maui.Controls/SkiaSharp.Views.Maui.Controls.csproj -c Debug /p:TargetFrameworks=net10.0-windows10.0.19041.0 /p:TargetFramework=net10.0-windows10.0.19041.0 /p:MauiVersion=10.0.71

# Release builds
dotnet build binding/SkiaSharp/SkiaSharp.csproj -c Release /p:TargetFrameworks=net10.0-windows10.0.19041.0 /p:TargetFramework=net10.0-windows10.0.19041.0
dotnet build source/SkiaSharp.Views.Maui/SkiaSharp.Views.Maui.Core/SkiaSharp.Views.Maui.Core.csproj -c Release /p:TargetFrameworks=net10.0-windows10.0.19041.0 /p:TargetFramework=net10.0-windows10.0.19041.0 /p:MauiVersion=10.0.71
dotnet build source/SkiaSharp.Views.Maui/SkiaSharp.Views.Maui.Controls/SkiaSharp.Views.Maui.Controls.csproj -c Release /p:TargetFrameworks=net10.0-windows10.0.19041.0 /p:TargetFramework=net10.0-windows10.0.19041.0 /p:MauiVersion=10.0.71
```

---

### Step 6: Deploy Native Libraries and ANGLE Dependencies
Since the project references bypass NuGet packages, MSBuild will not copy the native C++ DLLs or ANGLE dependencies. We must configure a post-build target in the consumer project.

1. Open `win/win32/xpl/GnollHackM/GnollHackM.csproj`.
2. Define the `SKIASHARP_SOURCE` ProjectReferences and add the custom post-build copy target:
   ```xml
   <Target Name="CopyCustomSkiaSharpNative" AfterTargets="Build" Condition="$(DefineConstants.Contains('SKIASHARP_SOURCE')) AND '$(TargetFramework)'=='net10.0-windows10.0.19041.0'">
       <!-- Copy local custom compiled native DLLs -->
       <Copy SourceFiles="C:\repos\SkiaSharp\output\native\windows\x64\libSkiaSharp.dll" DestinationFolder="$(OutputPath)" SkipUnchangedFiles="true" />
       <Copy SourceFiles="C:\repos\SkiaSharp\output\native\winui\any\SkiaSharp.Views.WinUI.Native.dll" DestinationFolder="$(OutputPath)" SkipUnchangedFiles="true" />
       <!-- Copy ANGLE OpenGL ES rendering dependencies from user's NuGet cache -->
       <Copy SourceFiles="$(UserProfile)\.nuget\packages\skiasharp.nativeassets.winui\3.119.1\runtimes\win-x64\native\libEGL.dll" DestinationFolder="$(OutputPath)" SkipUnchangedFiles="true" />
       <Copy SourceFiles="$(UserProfile)\.nuget\packages\skiasharp.nativeassets.winui\3.119.1\runtimes\win-x64\native\libGLESv2.dll" DestinationFolder="$(OutputPath)" SkipUnchangedFiles="true" />
   </Target>
   ```

---

### Step 7: Rebuild GnollHackM
Before compiling, terminate any running instances of the game and background MSBuild worker nodes to prevent locked file errors (`LNK1201` / `MSB3026`):
```powershell
Stop-Process -Name "GnollHackM" -Force -ErrorAction SilentlyContinue
Stop-Process -Name "MSBuild" -Force -ErrorAction SilentlyContinue
```

Then rebuild the application in the desired configuration:
```powershell
cd c:\hmp\GnollHack
dotnet build win/win32/xpl/GnollHackM/GnollHackM.csproj -c Release -f net10.0-windows10.0.19041.0 /p:TargetFrameworks=net10.0-windows10.0.19041.0 /p:TargetFramework=net10.0-windows10.0.19041.0 /p:MauiVersion=10.0.71
```

---

## 🔍 Troubleshooting & Diagnostic Checks

### Checking Symbols inside the DLL (PowerShell/Python)
To verify if a specific entry point is actually exported by `libSkiaSharp.dll` (case-sensitive check):
```powershell
python -c "data = open(r'C:\repos\SkiaSharp\output\native\windows\x64\libSkiaSharp.dll', 'rb').read(); print('FOUND' if b'sk_fontmgr_legacy_create_typeface' in data else 'NOT FOUND')"
```

### Unlocking Locked DLLs / PDBs
If MSBuild complains that a file is locked by Visual Studio or another process, find the PID and kill it:
```powershell
Get-Process | Where-Object { $_.Modules.FileName -like "*libSkiaSharp*" } | ForEach-Object { "$($_.Name) (ID: $($_.Id))" }
```
