---
name: skiasharp_from_source
description: Build the entire SkiaSharp (native + managed C# assemblies) from the local source repository and reference it from GnollHack via ProjectReference instead of NuGet PackageReference. Use when you need to modify SkiaSharp C# code (e.g., add profiling, fix threading, change rendering behavior) or need full control over both native and managed layers for debugging.
---

# Building SkiaSharp from Source for GnollHack

## Purpose

Reference SkiaSharp directly from source (`c:\repos\SkiaSharp`) instead of NuGet packages. This gives full control over both the native Skia engine and the C# wrapper layer, enabling:
- Adding profiling/instrumentation inside SkiaSharp itself (e.g., `SKXamlCanvas.DoInvalidate`)
- Testing C#-level fixes (e.g., modifying Dispose locking, changing surface creation)
- Debugging with full SkiaSharp symbols and source stepping
- Combining a custom Skia engine with modified C# wrappers

## Critical Rules

- **Build the native DLL first** — C# code cannot compile without `libSkiaSharp.dll` in `output/native/`.
- **Do NOT run `externals-download`** if you've modified anything under `externals/skia/` — it will overwrite your changes with pre-built CI binaries.
- **Keep platform conditionals** — only override SkiaSharp for the Windows target; keep NuGet PackageReference for Android/iOS to avoid cross-compilation headaches.
- **Do NOT commit GnollHack csproj changes** that reference local SkiaSharp paths to the main branch.

## Prerequisites

Same as `skiasharp_custom_native` skill, plus:
- .NET SDK matching SkiaSharp's requirements (check `global.json` in the SkiaSharp repo)
- Sufficient disk space (~2GB for managed build artifacts)

## Architecture Overview

GnollHack currently references SkiaSharp via three NuGet packages:

```
GnollHackM.csproj
  ├── SkiaSharp (4.150.0)                    → C# bindings + P/Invoke
  ├── SkiaSharp.Views.Maui.Controls (4.150.0) → SKCanvasView MAUI control
  └── SkiaSharp.Views.Maui.Core (4.150.0)    → Platform handlers (SKXamlCanvas on WinUI)
```

To build from source, we replace these with ProjectReferences to the corresponding `.csproj` files in the SkiaSharp repo.

## Step 1: Build the Native DLL

Follow the `skiasharp_custom_native` skill Steps 1–7 to build `libSkiaSharp.dll`. The native output at `c:\repos\SkiaSharp\output\native\windows\x64\` is required before the C# projects will build.

If you only want to test C# changes (not Skia engine changes), you can download pre-built natives instead:

```powershell
cd c:\repos\SkiaSharp
dotnet cake --target=externals-download
```

## Step 2: Build SkiaSharp C# Projects

```powershell
cd c:\repos\SkiaSharp

# Build the core binding (produces SkiaSharp.dll)
dotnet build binding/SkiaSharp/SkiaSharp.csproj -c Debug

# Build the MAUI views
dotnet build source/SkiaSharp.Views.Maui/SkiaSharp.Views.Maui.Core/SkiaSharp.Views.Maui.Core.csproj -c Debug
dotnet build source/SkiaSharp.Views.Maui/SkiaSharp.Views.Maui.Controls/SkiaSharp.Views.Maui.Controls.csproj -c Debug
```

## Step 3: Modify GnollHack's Project References

Edit `win/win32/xpl/GnollHackM/GnollHackM.csproj`:

```xml
<!-- Add a new conditional property for source-reference builds -->
<PropertyGroup Condition="'$(Configuration)'=='Debug' AND '$(UseSkiaSharpSource)'=='true'">
    <DefineConstants>$(DefineConstants);SKIASHARP_FROM_SOURCE</DefineConstants>
</PropertyGroup>

<!-- Existing NuGet references (keep for non-source builds and other platforms) -->
<ItemGroup Condition="!$(DefineConstants.Contains('SKIASHARP_3119')) AND '$(UseSkiaSharpSource)'!='true'">
    <PackageReference Include="SkiaSharp" Version="4.150.0" />
    <PackageReference Include="SkiaSharp.Views.Maui.Controls" Version="4.150.0" />
    <PackageReference Include="SkiaSharp.Views.Maui.Core" Version="4.150.0" />
</ItemGroup>

<!-- Source references (Windows only, Debug only) -->
<ItemGroup Condition="'$(UseSkiaSharpSource)'=='true' AND '$(TargetFramework)'=='net10.0-windows10.0.19041.0'">
    <ProjectReference Include="c:\repos\SkiaSharp\binding\SkiaSharp\SkiaSharp.csproj" />
    <ProjectReference Include="c:\repos\SkiaSharp\source\SkiaSharp.Views.Maui\SkiaSharp.Views.Maui.Core\SkiaSharp.Views.Maui.Core.csproj" />
    <ProjectReference Include="c:\repos\SkiaSharp\source\SkiaSharp.Views.Maui\SkiaSharp.Views.Maui.Controls\SkiaSharp.Views.Maui.Controls.csproj" />
</ItemGroup>

<!-- For non-Windows targets, always use NuGet even in source mode -->
<ItemGroup Condition="'$(UseSkiaSharpSource)'=='true' AND '$(TargetFramework)'!='net10.0-windows10.0.19041.0'">
    <PackageReference Include="SkiaSharp" Version="4.150.0" />
    <PackageReference Include="SkiaSharp.Views.Maui.Controls" Version="4.150.0" />
    <PackageReference Include="SkiaSharp.Views.Maui.Core" Version="4.150.0" />
</ItemGroup>
```

## Step 4: Build GnollHack with Source References

```powershell
cd c:\hmp\GnollHack

# Build with the source flag
dotnet build win/win32/xpl/GnollHackM/GnollHackM.csproj -c Debug -f net10.0-windows10.0.19041.0 /p:UseSkiaSharpSource=true
```

## Step 5: Ensure Native DLL Is Deployed

When using ProjectReference, the native DLL is NOT automatically copied from NuGet. You need to ensure `libSkiaSharp.dll` is in the output:

```powershell
# Copy the native DLL to the build output
$outputDir = Get-ChildItem -Path "win\win32\xpl\GnollHackM\bin\Debug\net10.0-windows10.0.19041.0" -Recurse -Filter "GnollHackM.dll" | Select-Object -First 1 -ExpandProperty DirectoryName
$nativeDll = "c:\repos\SkiaSharp\output\native\windows\x64\libSkiaSharp.dll"
Copy-Item $nativeDll $outputDir -Force
echo "Copied libSkiaSharp.dll to $outputDir"
```

For a more robust solution, add a post-build target to GnollHackM.csproj:

```xml
<Target Name="CopyCustomSkiaSharpNative" AfterTargets="Build" 
        Condition="'$(UseSkiaSharpSource)'=='true' AND '$(TargetFramework)'=='net10.0-windows10.0.19041.0'">
    <Copy SourceFiles="c:\repos\SkiaSharp\output\native\windows\x64\libSkiaSharp.dll" 
          DestinationFolder="$(OutputPath)" SkipUnchangedFiles="true" />
</Target>
```

## Making SkiaSharp C# Changes

With source references, you can modify SkiaSharp C# code directly. Common scenarios:

### Add Timing to SKXamlCanvas (WinUI Renderer)

```csharp
// File: c:\repos\SkiaSharp\source\SkiaSharp.Views\SkiaSharp.Views.WinUI\SKXamlCanvas.cs
// In the DoInvalidate() method, wrap the rendering in a Stopwatch:

private void DoInvalidate()
{
    var sw = System.Diagnostics.Stopwatch.StartNew();
    // ... existing rendering code ...
    sw.Stop();
    if (sw.ElapsedMilliseconds >= 5)
        System.Diagnostics.Debug.WriteLine($"[SKXamlCanvas] DoInvalidate={sw.ElapsedMilliseconds}ms size={bitmap?.PixelWidth}x{bitmap?.PixelHeight}");
}
```

### Remove Dispose Lock (Test PR #4080 Impact)

```csharp
// File: c:\repos\SkiaSharp\binding\SkiaSharp\SKObject.cs
// In DisposeNative() or DisposeManaged(), comment out the write lock to test impact:

// _rwLock.EnterWriteLock();  // DISABLED FOR TESTING
try {
    // ... dispose logic ...
} finally {
    // _rwLock.ExitWriteLock();  // DISABLED FOR TESTING
}
```

### Add Surface Creation Timing

```csharp
// File: c:\repos\SkiaSharp\binding\SkiaSharp\SKSurface.cs
// In the static Create methods, add timing:

public static SKSurface Create(SKImageInfo info, IntPtr pixels, int rowBytes)
{
    var sw = System.Diagnostics.Stopwatch.StartNew();
    // ... existing code ...
    sw.Stop();
    if (sw.ElapsedMilliseconds >= 1)
        System.Diagnostics.Debug.WriteLine($"[SKSurface] Create={sw.ElapsedMilliseconds}ms {info.Width}x{info.Height}");
    return result;
}
```

## Key Source Files

| Component | File | Purpose |
|-----------|------|---------|
| WinUI Canvas | `source/SkiaSharp.Views/SkiaSharp.Views.WinUI/SKXamlCanvas.cs` | Surface creation + bitmap render loop |
| MAUI Handler | `source/SkiaSharp.Views.Maui/SkiaSharp.Views.Maui.Core/Handlers/SKCanvasView/SKCanvasViewHandler.Windows.cs` | Maps SKCanvasView → SKXamlCanvas |
| SKSurface | `binding/SkiaSharp/SKSurface.cs` | Managed wrapper for native surface |
| SKObject | `binding/SkiaSharp/SKObject.cs` | Base class with Dispose/locking logic |
| Version Check | `binding/SkiaSharp/SkiaSharpVersion.cs` | Native library compatibility gate |
| P/Invoke | `binding/SkiaSharp/SkiaApi.generated.cs` | All native function declarations (DO NOT EDIT) |

## Restoration

To go back to NuGet packages:

```powershell
# Build without the source flag (defaults to NuGet)
dotnet build win/win32/xpl/GnollHackM/GnollHackM.csproj -c Debug -f net10.0-windows10.0.19041.0
```

Or simply remove `/p:UseSkiaSharpSource=true` from the build command. The csproj conditionals ensure NuGet packages are used when the flag is absent.

## Troubleshooting

### TFM Mismatch Errors

If SkiaSharp projects target different TFMs than GnollHack, you may see build errors. Check `global.json` and `.csproj` files for compatible framework versions.

### Missing HarfBuzzSharp

SkiaSharp depends on `HarfBuzzSharp`. If you get errors about missing HarfBuzzSharp, either:
- Keep the HarfBuzzSharp NuGet PackageReference in GnollHack
- Or add a ProjectReference to `binding/HarfBuzzSharp/HarfBuzzSharp.csproj`

### Circular or Duplicate Assembly References

If you get `CS0433` (type exists in both assemblies), ensure you're not mixing NuGet and ProjectReference for the same package. The conditional ItemGroups above should prevent this, but verify with:

```powershell
dotnet build --no-restore -v diag 2>&1 | Select-String "SkiaSharp" | Select-Object -First 20
```
