# GnollHack Project Rules

These rules apply to all AI-assisted development on the GnollHack codebase.

## Project Overview

GnollHack is a turn-based roguelike game derived from NetHack 3.6.2. It consists of:
- A **C core** game engine (~120 source files in `src/`, ~114 headers in `include/`)
- A **.NET 9.0 MAUI** graphical frontend (C#/XAML targeting Android, iOS, Windows)
- A **native bridge** connecting C and C# via P/Invoke callbacks (`win/win32/xpl/libshare/`)
- **Build-time utilities** (`makedefs`, `levcomp`, `dgncomp`, `dlb`) that compile game data

## C Code Style

Refer to [DEVEL/code_style.txt](file:///c:/hmp/GnollHack/DEVEL/code_style.txt) for the complete guide.

### Required Conventions
- **Indentation**: 4 spaces, NO tabs
- **Line width**: 78 characters maximum
- **Function definitions**: K&R-style argument declarations:
  ```c
  void
  foo(i, c)
  int i;
  char c;
  {
      /* body */
  }
  ```
- **Control statements**: Opening brace on same line: `if (cond) {`
- **Switch/case**: `case` labels unindented within `switch`
- **Variables**: Never declare in `for` init or conditions
- **Function prototypes**: Use `FDECL(name, (args))`, `NDECL(name)`, or `VDECL(name, (args))` macros in header files. Add new prototypes to `include/extern.h`.

## C# / .NET Conventions
- Use standard C# naming: PascalCase for public members, camelCase for locals
- Always marshal UI updates to the main thread via `MainThread.BeginInvokeOnMainThread()`
- P/Invoke strings use `MarshalAs(UnmanagedType.LPStr)` for C interop
- Shared code between GnollHackM and GnollHackX uses `<Compile Include>` file-linking, NOT project references

## Key Terminology

| Term | Meaning |
|------|----------|
| Glyph | An integer index representing a visual symbol (monster, object, terrain, effect) |
| Tile | A specific graphic image from a tileset sheet, mapped from a glyph |
| Soundset | A collection of sound effect assignments for a monster, object, or event |
| Layer | A rendering layer in the tile display system (background, floor, feature, monster, object, etc.) |
| Window Procs | The callback interface (`struct window_procs`) connecting the C core to a display frontend |
| nhdat | The bundled game data archive file created by the `dlb` utility |
| .des file | A human-readable level description file compiled by `levcomp` into binary `.lev` files |
| makedefs | Build utility that generates header files and data databases from source text |

## File Organization

| Area | Location |
|------|----------|
| C source | `src/` |
| C headers | `include/` |
| Game data sources | `dat/` |
| Build utilities | `util/` |
| MAUI app project | `win/win32/xpl/GnollHackM/` |
| Shared C# code | `win/win32/xpl/GnollHackX/GnollHackX/` |
| Native bridge (C) | `win/win32/xpl/libshare/` |
| Native bridge (C#) | `win/win32/xpl/GnollHackX/GnollHackX.Common/GnollHackService.cs` |
| VS solution | `win/win32/vs/GnollHack.sln` |
| Dev documentation | `DEVEL/` |

## Build Prerequisites

- Visual Studio 2022 with C++ Desktop and .NET MAUI workloads
- .NET 9.0 SDK
- Windows 10 SDK
- For Android: Android SDK with NDK
- For iOS: Mac build host with Xcode

## Important Warnings

- **Do NOT hand-edit auto-generated files**: `include/date.h`, `include/onames.h`, `include/pm.h`, `include/vis_tab.h`, `include/animoff.h`, `include/animtotals.h`, `src/vis_tab.c`
- **Do NOT modify `nhdat` directly**: It is a packaged archive rebuilt by `dlb`
- **The `binary/` directory** is NOT the build output directory; build output goes to `bin/$(Configuration)/$(Platform)/`
- **Preserve existing comments and documentation** in all files unless explicitly asked to change them
