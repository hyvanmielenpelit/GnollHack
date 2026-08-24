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

### Required C Style Conventions (C99 & ALLMAN)
AI Agents MUST adhere strictly to the following:
- **Brace placement**: **Strict Allman style** — the opening brace MUST go on its own line for both functions and control statements (`if`, `for`, `while`). Legacy NetHack files may still use K&R brace placement in places; convert to Allman when touching such code.
- **Variables**: May be declared close to where they are first used, not only at the top of a block. However, prefer C89-style `for`-loop declarations (variable declared before the loop) because some compilers may warn on loop-scoped declarations. Loop-scoped declarations are not banned but should be avoided when practical.
  - *Preferred*: `int i;` (before the loop) ... `for (i = 0; i < 5; i++)`
  - *Acceptable but may warn*: `for (int i = 0; i < 5; i++)`
  - *BAD*: declaring variables in a condition: `if (int x = foo())`
- **Function definitions & prototypes**: Use standard ANSI C prototypes (e.g. `void foo(int x)`). Do not use legacy K&R declarations.
- **Indentation**: 4 spaces, NO tabs. 78 characters maximum width.
- **Naming**: `snake_case` for functions/variables. `ALL_CAPS_WITH_UNDERSCORES` for macros/enums.
- **Switch/case**: `case` labels unindented within `switch`
- **Single-statement bodies**: Both braced and unbraced forms are acceptable; use whichever is clearer. Do not use the comma operator to combine multiple assignments in a single-statement body.
- **Comments**: `/* */` is the preferred comment style. `//` may be used for commenting out code or for quick inline notes about old values, but do NOT use `//` for regular documentation or explanatory comments.
- **long long**: Allowed. Used throughout the codebase for 64-bit values.
- **Banned C99 features**: Variable-length arrays (VLAs) are BANNED. 
- **Discouraged C99 features**: Designated initializers, compound literals, variadic macros, `restrict`, `_Pragma`, flexible array members, `<stdbool.h>`, and `inline` should be avoided (see code_style.txt for details).

## C# / .NET Conventions
- **Pure C# naming**: PascalCase for public members, camelCase for locals (standard C# conventions)
- **Interop struct fields**: Use `snake_case` to match the C side (e.g., `animation_name`, `number_of_tiles`, `layer_flags`)
- **Interop enums and defines**: Use `ALL_CAPS_WITH_UNDERSCORES` to match C `#define`s and enum values (e.g., `LAYER_FLOOR`, `MENU_FLAGS_IS_HEADING`). The enum type name itself also uses `snake_case` on the C# side (e.g., `layer_types`) for interop consistency.
- Always marshal UI updates to the main thread via `MainThread.BeginInvokeOnMainThread()`
- P/Invoke strings use `MarshalAs(UnmanagedType.LPStr)` for C interop
- Shared code between GnollHackM and GnollHackX uses `<Compile Include>` file-linking, NOT project references
- **File-linked platform code**: GnollHackM also compiles platform-specific `.cs` files from legacy `GnollHackX.Android/` and `GnollHackX.iOS/` directories via `<Compile Include>` (within platform-conditioned `<ItemGroup>` blocks). These files may import platform-specific NuGet packages (e.g., `Xamarin.Google.Android.Play.Core` for in-app reviews). When evaluating NuGet dependencies in `GnollHackM.csproj`, always check file-linked sources in `GnollHackX.*` directories — not just `GnollHackM/` itself.

## File Indentation Standards

The `.editorconfig` at the repository root enforces these indentation rules. AI agents **MUST** follow them:

| File Type | Indent | Notes |
|-----------|--------|-------|
| `.c`, `.h`, `.cpp`, `.hpp`, `.l`, `.y` | **4 spaces** | C/C++ source, headers, Lex/Yacc |
| `.cs` | **4 spaces** | C# source |
| `.xaml` | **4 spaces** | XAML layouts |
| `.csproj`, `.vcxproj`, `.proj` | **2 spaces** | MSBuild project files |
| `.props`, `.targets` | **2 spaces** | MSBuild property/target sheets |
| `.xml`, `.config`, `.manifest`, `.resx` | **2 spaces** | XML configuration files |
| `.json` | **2 spaces** | JSON configuration |
| `.yaml`, `.yml` | **2 spaces** | YAML (tabs are forbidden by spec) |
| `.sln` | **Tabs** | Visual Studio solution files |

> **⚠️ Common mistake**: MSBuild project files (`.csproj`, `.vcxproj`, `.props`, `.targets`, `.proj`) use **2 spaces** per indent level — never tabs or 4 spaces. This matches the convention used by Visual Studio, `dotnet new`, and official Microsoft repositories.


## BOM (Byte Order Mark) Policy

| File Type | BOM | Reason |
|-----------|-----|--------|
| `.sln` | **WITH BOM** (`utf-8-bom`) | VS solution parser requires it |
| `.vcxproj`, `.vcxproj.filters` | **WITH BOM** (`utf-8-bom`) | VS C++ project serializer convention |
| All other files | **No BOM** (`utf-8`) | Industry standard |

> **⚠️ Important**: AI agents must **never** add a UTF-8 BOM (`EF BB BF`) to files they create or modify, except for `.sln`, `.vcxproj`, and `.vcxproj.filters` files. Most AI tools create BOM-less UTF-8 by default, which is correct. If you encounter a file with an unexpected BOM, remove it. The `.editorconfig` enforces `charset = utf-8` (no BOM) globally with explicit `charset = utf-8-bom` overrides only for `.sln`, `.vcxproj`, and `.vcxproj.filters`.


## Line Ending Policy

Git stores all text files with **LF** line endings in the repository. On checkout, `core.autocrlf = true` converts to the OS-native format (CRLF on Windows, LF on Linux/macOS). A `.gitattributes` file with `* text=auto` enforces this for all contributors regardless of their local Git configuration.

> **⚠️ AI agents frequently produce incorrect line endings.** Pay attention to line endings when creating or modifying files:
>
> - On **Windows**, write files with **CRLF** line endings (the OS default). Git normalizes to LF on commit automatically.
> - On **Linux/macOS**, write files with **LF** line endings (the OS default).
> - **Exception — `.sln`, `.bat`, `.cmd`**: These must **always** be CRLF, even on Linux, because their Windows-native parsers require it.
>
> **Detect before you edit — do not guess from the OS:**
>
> ```bash
> git ls-files --eol <path>   # i/ = index, w/ = working tree (w/crlf or w/lf)
> ```
>
> Without git, use `od -c <path> | head -3` and look for CR bytes.
>
> **Do NOT use `grep`, `head`, or `file` to detect CR.** Git Bash/MSYS and WSL
> open files in text mode and strip CR, so these report LF for a CRLF file —
> silently, with no error. Acting on that produces a file with mixed line
> endings, which is worse than either convention.
>
> When modifying an **existing** file, match whatever `w/` reports, regardless of
> the OS. Use the OS default only when creating a **new** file. Never mix
> conventions within a single file.
>
> **Common mistake**: Creating or modifying a shell script (`.sh`) and mixing line endings within the file. A CRLF shebang (`#!/bin/bash\r`) causes `bad interpreter: No such file or directory` on Linux/WSL. Git normalizes on commit, so the repository always holds LF.
>
> **How `.sh` is handled in this repo**: `.sh` files are deliberately left under
> the default `* text=auto` rule — CRLF in a Windows working tree, LF in a
> Linux/WSL one, LF in the repository. This is correct and should **not** be
> "fixed" with an `eol=lf` attribute. The scripts the build actually executes
> inside WSL from the Windows tree (`createassetpack.sh`) are passed through
> **`dos2unix` first**, in the `makedefsdroid.vcxproj` build command, together
> with the `dat/` text files. **Do not remove those `dos2unix` calls** — they
> look redundant but are what keeps a Windows checkout working under WSL.
>
> **Why AI agents get this wrong**: Git stores all text files with **LF** internally. When AI tools read code from the Git object store (`git show`, `git cat-file`), GitHub API, or `raw.githubusercontent.com`, they receive **LF** line endings — even on Windows. The AI then reproduces those LF endings in its output. On Windows, the correct behavior is to write **CRLF** to the working tree and let Git normalize back to LF on commit. Do not blindly copy line endings from Git/GitHub source data.


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
- **External assets** (tilesets, sound banks, secrets file) must be obtained from separate repositories — see the `build_pipeline` skill for full details
- **For Android and iOS builds**: WSL with SSH service running (`sudo service ssh start`) and configured in VS Connection Manager is required before building `GnollHack.sln`, as `dlbdroid` generates the `nhdat` game data archive inside WSL
- **For iOS builds**: `vcremote` must be configured on the Mac for the C++ library build (`gnollhackios.vcxproj`); `Pair to Mac` is separately needed for the .NET MAUI project (`GnollHackM.csproj`). PSCP download scripts in `win/win32/xpl/gnollhackios/` must be configured with Mac credentials and run interactively once to cache the Mac host key before building in VS.

## Implementation Plans

**Non-trivial tasks require a written implementation plan, approved by the user before any file is modified.**

A plan is **required** when a task meets any of these:
- It touches **more than one file**, or more than one subsystem (C core, native bridge, MAUI frontend, build utilities, game data)
- It crosses a **build regeneration boundary** (`makedefs`, `levcomp`, `dgncomp`, `dlb`, `makedefsdroid`)
- It adds or changes a **C-to-C# interop** struct, enum, or callback
- It is a refactor, a new feature, or anything the user describes as large or non-trivial

A plan is **not** required for single-file fixes, typo and comment corrections, answering questions, or read-only investigation. When in doubt, write one — a rejected plan is cheap, a wrong cross-layer change is not.

### Plan Delivery

- Deliver the plan as a **reviewable document** — a Markdown file saved under the
  repository's gitignored `plans/` directory:
  `plans/YYYY-MM-DD/task_name/implementation_plan_v<N>.md` (where N=1 for the
  first version).
  Use the creation date of the first plan for the topic as the date directory, and
  a short `snake_case` name for the task directory. Create subdirectories as needed.
- **Document versioning**: the first version always gets a `_v1` suffix. Never overwrite
  an existing version — to revise, create a new file with the next version number
  (`_v2`, `_v3`, etc.). This applies to all document types (plans, reviews, analyses,
  reports). `task.md` and `walkthrough.md` are singular (no version suffix).
- Short plans may be presented inline instead.
- **Wait for explicit user approval before editing any file.** Do not begin implementation alongside the plan.
- If the plan turns out to be wrong during implementation, stop and re-confirm rather than silently diverging from what the user approved.

### Subagent Use and Pair Programming

Every implementation plan **MUST** include a **Subagent Use** section. Read the full guidelines in the [subagent_guidelines skill](file:///c:/hmp/GnollHack/.agents/skills/subagent_guidelines/SKILL.md) before creating any plan. Key points:

- **Always document** whether subagents are needed — default to `inherit` model (matching the orchestrator); use `flash` only for trivially mechanical search-and-replace
- **Human task assignments are the rare exception** — only for very extensive cut-and-paste moves where AI would likely fail and waste tokens
- **No two agents may edit the same file concurrently** — plan file assignments to avoid conflicts
- **Respect build dependency chains** — do not parallelize across `makedefs` / `levcomp` / `makedefsdroid` regeneration boundaries
- **Never overwrite uncommitted changes** without explicit user permission — ask the user to commit first if corruption risk exists

## Important Warnings

- **Do NOT hand-edit auto-generated files**: `include/date.h`, `include/onames.h`, `include/pm.h`, `include/vis_tab.h`, `include/animoff.h`, `include/animtotals.h`, `src/vis_tab.c`
- **Do NOT modify `nhdat` directly**: It is a packaged archive rebuilt by `dlb`
- **The `binary/` directory** is NOT the build output directory; build output goes to `bin/$(Configuration)/$(Platform)/`
- **Preserve existing comments and documentation** in all files unless explicitly asked to change them
- **XAML Source of Truth**: All XAML files in `GnollHackM/` are **auto-generated** from `GnollHackX/GnollHackX/` by the `makedefsdroid` build step. **Always edit XAML in the GnollHackX source**, never directly in GnollHackM. After modifying any XAML file, first try to regenerate the MAUI XAML yourself by building the `makedefsdroid` project (locate MSBuild via `vswhere.exe`, then run `& $msbuild win/win32/vs/makedefsdroid.vcxproj /t:Build /p:Configuration=Debug /p:Platform=x64`). If that fails, ask the user to build it. The GnollHackM build will fail to find `x:Name` references from new XAML elements until this regeneration happens.
- **XAML WidthRequest / HeightRequest double-space convention**: In GnollHackX XAML (the Xamarin source), `makedefsdroid` converts `WidthRequest  =` (two spaces before `=`) into `MaximumWidthRequest=` and `HeightRequest  =` into `MaximumHeightRequest=` for GnollHackM. **Two equally critical mistakes** to avoid:
  1. **NEVER write `MaximumWidthRequest=` or `MaximumHeightRequest=`** in GnollHackX XAML — these do not exist in Xamarin.Forms and will not compile. Use `WidthRequest  =` / `HeightRequest  =` (two spaces before `=`) instead.
  2. **NEVER write `WidthRequest=` or `HeightRequest=` (no extra spaces)** when the intent is `MaximumWidthRequest` on the MAUI side — this produces a plain `WidthRequest` in GnollHackM, causing elements to be overly wide or tall. You MUST use the two-space form (`WidthRequest  =` / `HeightRequest  =`) to get the `Maximum` version.
- **Do NOT hardcode version numbers in AI skills**: NuGet package versions, SDK build numbers, game version strings, and platform minimum OS versions change frequently. AI skills (`.agents/skills/`) must reference the source-of-truth files (e.g., `GnollHackM.csproj` for NuGet and target framework versions, `include/date.h` for game version) rather than embedding specific version strings that will drift out of date.
