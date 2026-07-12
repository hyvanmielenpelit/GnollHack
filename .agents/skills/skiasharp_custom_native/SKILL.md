---
name: skiasharp_custom_native
description: Build a custom libSkiaSharp.dll from the SkiaSharp repository with a specific Skia engine milestone for Windows. Use for bisecting which Skia milestone introduced a rendering regression, or for testing patches to the Skia raster pipeline. Builds only the native DLL — does not rebuild the C# managed layer.
---

# Building a Custom SkiaSharp Native DLL for Windows

## Purpose

Build `libSkiaSharp.dll` from the SkiaSharp source repository (`c:\repos\SkiaSharp`) at a specific Skia engine milestone. The DLL reports its **real** milestone version, and you pair it with a matching C# SkiaSharp NuGet package in GnollHack. This enables clean bisection of which Skia milestone introduced a rendering regression.

## Critical Rules

- **Do NOT fake the milestone number** in `SkMilestone.h` — let the DLL report its real version. Pair with matching C# bindings instead (see Recommended Workflow below).
- **Do NOT run `externals-download`** after changing the Skia submodule — that would overwrite your custom build with pre-built binaries from CI.
- **Do NOT modify the SkiaSharp C# code** — this skill builds native only. For C# modifications, see the `skiasharp_from_source` skill.
- Build requires ~5GB disk space for Skia build intermediates.

## Recommended Workflow: Matching C# + Real Milestone

Use matching C# NuGet packages to avoid both version check failures AND `EntryPointNotFoundException`:

| Test Goal | C# Package | Native DLL Source | Version Check | EPNFE Risk |
|-----------|-----------|-------------------|---------------|------------|
| m119 (baseline) | v3.119.4 via `SKIASHARP_3119` | Build from v3.119.4's skia SHA | ✅ Natural match | ✅ None |
| m147 (known bad) | v4.147-preview.1.1 | Build from v4.147-preview.1.1's skia SHA | ✅ Natural match | ✅ None |
| Intermediate (m132, m140, etc.) | v3.119.4 via `SKIASHARP_3119` | Build from that milestone's skia SHA | ❌ Use reflection bypass (see `skiasharp_dll_swap` skill) | Low |

**Key insight**: GnollHack already has a `SKIASHARP_3119` conditional toggle in `GnollHackM.csproj` to switch between SkiaSharp C# versions. The C# wrapper code is **identical** between v3.119.4 and v4.147-preview.1.1 — only the Skia engine milestone differs.

For intermediate milestones, the C API is a superset of m119 (functions are added between milestones, not removed), so v3.119.4 C# bindings can safely call into m132/m140 DLLs. The only issue is the version check, which is bypassed via reflection (see `skiasharp_dll_swap` skill).

## Prerequisites

All must be present on the Windows machine:

| Tool | Required | How to Check |
|------|----------|--------------|
| Visual Studio 2022+ with C++ Desktop workload | Yes | `& "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe" -latest -property installationPath` |
| LLVM/Clang-CL | Recommended | `Test-Path "C:\Program Files\LLVM\bin\clang-cl.exe"` |
| Python 3.x | Yes | `python --version` |
| .NET SDK with Cake tool | Yes | `dotnet cake --version` |
| depot_tools | Yes (auto-cloned by Cake) | `Test-Path "c:\repos\SkiaSharp\externals\depot_tools"` |

## Step 1: Identify the Skia Submodule SHA for Each Version

Use git to find the skia submodule commit that each SkiaSharp version uses:

```powershell
cd c:\repos\SkiaSharp

# Get the skia submodule SHA for specific SkiaSharp tags
$versions = @("v3.119.4", "v4.147.0-preview.1.1", "v4.148.0", "v4.150.0")
foreach ($v in $versions) {
    $sha = git ls-tree $v externals/skia 2>$null | ForEach-Object { ($_ -split '\s+')[2] }
    if ($sha) { echo "${v}: $sha" } else { echo "${v}: TAG NOT FOUND" }
}
```

For intermediate milestones not tied to a SkiaSharp release, use the mono/skia fork branches:

```powershell
cd c:\repos\SkiaSharp\externals\skia
git fetch origin
git branch -r | Select-String "skiasharp"
# Look for branches like: origin/skiasharp-m132, origin/skiasharp-m140, etc.
```

## Step 2: Record Current State

```powershell
cd c:\repos\SkiaSharp\externals\skia

# Save current commit so you can restore later
$originalSha = git rev-parse HEAD
echo "Original Skia SHA: $originalSha"
echo $originalSha > "$env:TEMP\skia-original-sha.txt"
```

## Step 3: Check Out Target Milestone

```powershell
cd c:\repos\SkiaSharp\externals\skia

# For a known SkiaSharp version (e.g., v3.119.4's skia commit)
git checkout <sha-from-step-1>

# For a mono/skia fork milestone branch
git checkout origin/skiasharp-m132
```

## Step 4: Sync Dependencies

```powershell
cd c:\repos\SkiaSharp\externals\skia

# Download third-party dependencies (freetype, libjpeg, libpng, etc.)
# Use Python directly since the cake target validates milestone numbers
python tools/git-sync-deps
```

> **Note**: If using `dotnet cake --target=git-sync-deps` instead, it validates that `SK_MILESTONE` in `SkMilestone.h` matches `VERSIONS.txt`. This will fail for non-current milestones. Either use the Python command directly, or temporarily update `VERSIONS.txt` to match (revert afterward).

## Step 5: Build the Native DLL

```powershell
cd c:\repos\SkiaSharp

# Set VS_INSTALL if not auto-detected
$env:VS_INSTALL = "C:\Program Files\Microsoft Visual Studio\18\Community"

# Build for Windows x64 only
dotnet cake native/windows/build.cake --target=libSkiaSharp --arch=x64
```

The output DLL will be at:
```
c:\repos\SkiaSharp\output\native\windows\x64\libSkiaSharp.dll
```

### Build Time

| Scenario | Time |
|----------|------|
| First build (cold) | 10-20 minutes |
| Rebuild after changing submodule commit | 5-10 minutes |
| Incremental (minor changes) | 1-3 minutes |

### Build Configuration

The default builds a Release DLL. For Debug (slower code but better debugging symbols):

```powershell
dotnet cake native/windows/build.cake --target=libSkiaSharp --arch=x64 --configuration=Debug
```

## Step 6: Deploy the Custom DLL into GnollHack

### Option A: Direct copy to build output

```powershell
$customDll = "c:\repos\SkiaSharp\output\native\windows\x64\libSkiaSharp.dll"

# Find where GnollHack puts the DLL
$outputDir = Get-ChildItem -Path "c:\hmp\GnollHack\win\win32\xpl\GnollHackM\bin" -Recurse -Filter "libSkiaSharp.dll" | Select-Object -First 1 -ExpandProperty DirectoryName

Copy-Item $customDll "$outputDir\libSkiaSharp.dll" -Force
echo "Deployed custom DLL to $outputDir"
```

> **Note**: This must be repeated after each GnollHack rebuild (NuGet restore overwrites the DLL).

### Option B: Override the NuGet cache (survives rebuilds)

```powershell
$skiasharpVersion = "3.119.4"  # Match whatever C# package GnollHack is using
$cacheDir = "$env:USERPROFILE\.nuget\packages\skiasharp.nativeassets.win32\$skiasharpVersion\runtimes\win-x64\native"

# Back up the original
Copy-Item "$cacheDir\libSkiaSharp.dll" "$cacheDir\libSkiaSharp.dll.bak" -Force
# Replace with custom build
Copy-Item $customDll "$cacheDir\libSkiaSharp.dll" -Force
echo "Replaced NuGet cache DLL (backup at .dll.bak)"
```

## Step 7: Build and Test GnollHack

```powershell
cd c:\hmp\GnollHack

# For m119 native with v3.119.4 C# (natural version match, no reflection needed)
dotnet build win/win32/xpl/GnollHackM/GnollHackM.csproj -c Debug -f net10.0-windows10.0.19041.0 /p:DefineConstants="SKIASHARP_3119"

# For intermediate milestones (e.g., m132 native with v3.119.4 C#, needs reflection bypass)
dotnet build win/win32/xpl/GnollHackM/GnollHackM.csproj -c Debug -f net10.0-windows10.0.19041.0 /p:DefineConstants="SKIASHARP_3119;SKIASHARP_DLL_SWAP_TEST"
```

Run and test the affected pages (SnapshotPage, SaveTransferPage). Check the Skia milestone version displayed in GnollHack to confirm the correct engine is loaded.

## Restoration

```powershell
cd c:\repos\SkiaSharp\externals\skia

# Restore the original Skia commit
$originalSha = Get-Content "$env:TEMP\skia-original-sha.txt"
git checkout $originalSha

# If you modified the NuGet cache, restore it
$cacheDir = "$env:USERPROFILE\.nuget\packages\skiasharp.nativeassets.win32\3.119.4\runtimes\win-x64\native"
if (Test-Path "$cacheDir\libSkiaSharp.dll.bak") {
    Move-Item "$cacheDir\libSkiaSharp.dll.bak" "$cacheDir\libSkiaSharp.dll" -Force
}
```

## Troubleshooting

### C API Compilation Errors

When building from an older milestone, the C API shim in `src/c/` and `include/c/` is from that era. It should compile cleanly against its own engine. If you see errors, the submodule checkout may have been incomplete:

```powershell
cd c:\repos\SkiaSharp\externals\skia
git status  # Should show clean checkout
git submodule update --init --recursive  # Ensure submodules-of-submodules are correct
```

### `git-sync-deps` or Python Errors

- Ensure Python 3 is on PATH.
- Ensure network access to `chromium.googlesource.com`.
- Try running the sync directly: `python tools/git-sync-deps` from the `externals/skia` directory.

### Build Fails with Missing Tools

| Error | Fix |
|-------|-----|
| gn not found | `python externals/skia/bin/fetch-gn` |
| ninja not found | Handled by `depot_tools/ninja.py` |
| MSVC not found | Set `$env:VS_INSTALL = "C:\Program Files\Microsoft Visual Studio\18\Community"` |
| Spectre libs not found | Install "MSVC vNNN Spectre-mitigated Libs" component in VS Installer |

### Bisection Strategy

For efficient bisection across milestones:

1. Start with m119 (known good) and m147 (known bad).
2. Test the midpoint (m133).
3. Based on result, narrow to m119–m133 or m133–m147.
4. Continue halving until you find the exact milestone.

Keep a log:

| Milestone | Skia SHA | C# Package | Result | Notes |
|-----------|----------|-----------|--------|-------|
| m119 | 7dbfc07... | v3.119.4 | ✅ Fast | Baseline |
| m147 | (from v4.147-preview.1.1) | v4.147-preview.1.1 | ❌ Slow | Known bad |
| m133 | (from fork branch) | v3.119.4 + bypass | ? | Test next |
