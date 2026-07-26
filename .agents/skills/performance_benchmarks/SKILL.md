---
name: performance_benchmarks
description: How to add new performance benchmark entries to DEVEL/performance.txt. Covers the required file format, where to find version numbers, the benchmark test procedure, and formatting rules.
---

# Performance Benchmarks

## Overview

The file `DEVEL/performance.txt` records FPS benchmarks measured in **minimap mode** using **GPU rendering** on the **first dungeon level** with the **whole map revealed** in **wizard mode**.

Each dated entry contains shared version information followed by per-platform results.

## File Location

- **Performance record**: `DEVEL/performance.txt`

## Entry Structure

Each new benchmark entry is appended at the **end** of the file, separated from the previous entry by **two blank lines**. The structure is:

```
Date: YYYY-MM-DD
================
.NET: <runtime version> (SDK <sdk version>)
.NET MAUI: <non-ios version> (iOS: <ios version>)
SkiaSharp: <skiasharp version>

Android
-------
  Workload:       <android workload manifest version>
  Test Device:    <device name>
  Device OS:      <android version>
  FPS:            <fps>

iOS
---
  Workload:       <ios workload manifest version>
  Test Device:    <device name>
  Device OS:      <ipados/ios version>
  FPS:            <fps>

Windows
-------
  Workload:       <maui-windows workload manifest version>
  Test Device:    <device name>
  Device OS:      <windows version and build>
  FPS:            <fps>
```

## Where to Find Version Numbers

| Field | Source |
|-------|--------|
| .NET runtime version | The version band of `Microsoft.Extensions.Logging.Debug` in `win/win32/xpl/GnollHackM/GnollHackM.csproj` |
| .NET SDK version | `win/win32/xpl/GnollHackM/global.json` → `sdk.version` |
| .NET MAUI (non-iOS) | `Microsoft.Maui.Controls` version with `!$(TargetFramework.Contains('-ios'))` condition in `GnollHackM.csproj` |
| .NET MAUI (iOS) | `Microsoft.Maui.Controls` version with `$(TargetFramework.Contains('-ios'))` condition in `GnollHackM.csproj` |
| Workload (per platform) | Run `dotnet workload list --sdk-version <sdk version>` → use the manifest version (first part before `/`) for `android`, `ios`, and `maui-windows` respectively |
| SkiaSharp | Default (non-`SKIASHARP_3119`) `SkiaSharp` package version in `GnollHackM.csproj` |

## Formatting Rules

- **Line width**: Maximum 78 characters per line.
- **Line endings**: Windows CRLF (`\r\n`). After writing the file, verify and convert if needed.
- **Indentation**: Two spaces before field names in platform sections. Field values aligned with spaces to column 21 (after `  Test Device:    `).
- **Platform heading underlines**: Use `-` characters matching the length of the platform name (e.g., `Android` → `-------`, `iOS` → `---`, `Windows` → `-------`).
- **Date heading underline**: Use `=` characters matching the length of the `Date: YYYY-MM-DD` line (16 `=` characters).
- **Separation**: Two blank lines between the last platform section of one entry and the `Date:` line of the next entry.
- **Do NOT modify existing entries** unless the user explicitly asks to correct one.

## Required Information from the User

Before adding an entry, ensure the user provides:

1. **Date** of the benchmark (or use today's date if the user confirms)
2. **FPS values** for each platform tested
3. **Test device names** for each platform
4. **Device OS versions** for each platform (e.g., Android 16, iPadOS 26.5.2, Windows 11 Home 10.0.26200)

Version numbers (.NET, MAUI, SkiaSharp) should be read from the project files unless the user provides overrides.

## Benchmark Test Procedure (for Reference)

The FPS measurement is taken under these conditions:
1. Launch GnollHack in **wizard mode**
2. Navigate to or start on **dungeon level 1**
3. Use **Ctrl+F** (wizard map command) to **reveal the whole map**
4. Switch to **minimap mode**
5. Ensure **GPU rendering** is enabled
6. Record the displayed **FPS**
