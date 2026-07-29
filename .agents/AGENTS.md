# GnollHack Project Rules

These rules apply to all AI-assisted development on the GnollHack codebase.

## Project Overview

GnollHack is a turn-based roguelike game derived from NetHack 3.6.2. It consists of:
- A **C core** game engine (~120 source files in `src/`, ~114 headers in `include/`)
- A **.NET 10.0 MAUI** graphical frontend (C#/XAML targeting Android, iOS, Windows)
- A **native bridge** connecting C and C# via P/Invoke callbacks (`win/win32/xpl/libshare/`)
- **Build-time utilities** (`makedefs`, `levcomp`, `dgncomp`, `dlb`) that compile game data

## C Code Style

Refer to [DEVEL/code_style.txt](file:///c:/hmp/GnollHack/DEVEL/code_style.txt) for the complete guide.

### Required C Style Conventions (STRICT C89 & ALLMAN)
AI Agents MUST override their default modern C habits and adhere strictly to the following:
- **Brace placement**: **Strict Allman style** — the opening brace MUST go on its own line for both functions and control statements (`if`, `for`, `while`). Legacy NetHack files may still use K&R brace placement in places; convert to Allman when touching such code.
- **Variables**: Strict C89 rules. **NEVER** declare variables inside `for` loops or conditions. All variables must be declared at the top of the block.
  - *BAD*: `for (int i = 0; i < 5; i++) {`
  - *GOOD*: `int i;` (at block start) ... `for (i = 0; i < 5; i++)`
- **Function definitions & prototypes**: Use standard ANSI prototypes (e.g. `void foo(int x)`). Do not use legacy K&R declarations. (Note: Despite using modern prototypes, you must still strictly adhere to C89 rules for variable declarations).
- **Indentation**: 4 spaces, NO tabs. 78 characters maximum width.
- **Naming**: `snake_case` for functions/variables. `ALL_CAPS_WITH_UNDERSCORES` for macros/enums.
- **Switch/case**: `case` labels unindented within `switch`
- **Single-statement bodies**: Both braced and unbraced forms are acceptable; use whichever is clearer. Do not use the comma operator to combine multiple assignments in a single-statement body.
- **Comments**: Use `/* */` for all C comments. Do NOT use `//` in the C core.

## C# / .NET Conventions
- **Pure C# naming**: PascalCase for public members, camelCase for locals (standard C# conventions)
- **Interop struct fields**: Use `snake_case` to match the C side (e.g., `animation_name`, `number_of_tiles`, `layer_flags`)
- **Interop enums and defines**: Use `ALL_CAPS_WITH_UNDERSCORES` to match C `#define`s and enum values (e.g., `LAYER_FLOOR`, `MENU_FLAGS_IS_HEADING`). The enum type name itself also uses `snake_case` on the C# side (e.g., `layer_types`) for interop consistency.
- Always marshal UI updates to the main thread via `MainThread.BeginInvokeOnMainThread()`
- P/Invoke strings use `MarshalAs(UnmanagedType.LPStr)` for C interop
- Shared code between GnollHackM and GnollHackX uses `<Compile Include>` file-linking, NOT project references
- **File-linked platform code**: GnollHackM also compiles platform-specific `.cs` files from legacy `GnollHackX.Android/` and `GnollHackX.iOS/` directories via `<Compile Include>` (within platform-conditioned `<ItemGroup>` blocks). These files may import platform-specific NuGet packages (e.g., `Xamarin.Google.Android.Play.Core` for in-app reviews). When evaluating NuGet dependencies in `GnollHackM.csproj`, always check file-linked sources in `GnollHackX.*` directories — not just `GnollHackM/` itself.

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
| Android platform code (file-linked) | `win/win32/xpl/GnollHackX/GnollHackX.Android/` |
| iOS platform code (file-linked) | `win/win32/xpl/GnollHackX/GnollHackX.iOS/` |
| FMOD audio wrappers (file-linked) | `win/win32/xpl/GnollHackX/GnollHackX.FMOD/` |
| Native bridge (C) | `win/win32/xpl/libshare/` |
| Native bridge (C#) | `win/win32/xpl/GnollHackX/GnollHackX.Common/GnollHackService.cs` |
| VS solution | `win/win32/vs/GnollHack.sln` |
| Dev documentation | `DEVEL/` |

## Build Prerequisites

- Visual Studio 2026 with C++ Desktop (v143 build tools) and .NET MAUI workloads
- .NET 10.0 SDK
- Windows 10 SDK
- For Android: Android SDK with NDK
- For iOS: Mac build host with Xcode

## Important Warnings

- **Do NOT hand-edit auto-generated files**: `include/date.h`, `include/onames.h`, `include/pm.h`, `include/vis_tab.h`, `include/animoff.h`, `include/animtotals.h`, `src/vis_tab.c`
- **Do NOT modify `nhdat` directly**: It is a packaged archive rebuilt by `dlb`
- **The `binary/` directory** is NOT the build output directory; build output goes to `bin/$(Configuration)/$(Platform)/`
- **Preserve existing comments and documentation** in all files unless explicitly asked to change them
- **XAML Source of Truth**: All XAML files in `GnollHackM/` are **auto-generated** from `GnollHackX/GnollHackX/` by the `makedefsdroid` build step. **Always edit XAML in the GnollHackX source**, never directly in GnollHackM. After modifying any XAML file, ask the user to build the GnollHackX solution so `makedefsdroid` regenerates the GnollHackM XAML and the code-behind `.g.cs` files. The GnollHackM build will fail to find `x:Name` references from new XAML elements until this regeneration happens.
