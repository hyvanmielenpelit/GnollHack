---
name: build_pipeline
description: Guide on building GnollHack native C libraries, processing assets with makedefs/levcomp/dgncomp/dlb, packaging nhdat, and compiling the C# .NET MAUI frontend for Android, iOS, and Windows, plus the Avalonia port (GnhAvalonia=true) for Linux (AppImage, Steam depot, tar.gz), macOS (GnollHack.app with Developer ID signing, notarization, DMG and Steam depot, built on the Mac over SSH) and Windows. Also covers the Apple engine libraries built over SSH and deploying the Linux build to a Steam Deck.
---

# Build Pipeline

End-to-end build process for GnollHack, from compiling the C core engine to packaging the .NET MAUI frontend.

## Critical Rules
- **Do NOT manually edit generated files** — they are rebuilt by `makedefs`. See the auto-generated files list in `AGENTS.md`.
- Always rebuild data files using `makedefs`, `levcomp`, and `dgncomp` after editing `dat/` files.
- AI skills must **not hardcode version numbers**. Reference source files (`GnollHackM.csproj`, `include/date.h`) instead.
- **A data file added to the Windows pipeline is not automatically in the mobile one.** See [Two Data Pipelines](#two-data-pipelines).
- **After any build, verify the expected artifacts exist.** MSBuild reports success even when a project was silently skipped. See [Verifying the Build](#verifying-the-build).
- **Upgrading a dependency re-checks its workarounds and known issues.** Bumping a package in `GnollHackM.csproj`, an SDK in a `global.json`, a workload, FMOD, or sentry-native re-checks the `DEVEL/workarounds/` entries, workarounds and known issues alike, whose `Package` names it. For NuGet packages and sentry-native the lint's version-drift warning lists the upstream-fixed entries that matter; for the rest, search the `Package` fields. An upgrade that contains a known issue's fix is the moment to start its **Verify fix**. See `client_workarounds`.
- **Native Sentry binaries.** `gnhsentry.dll`, `libsentry.so` and `libgnhsentry.dylib` are committed builds checked against `GnhSentryVersion` and the vendored `sentry.h` by `GnhSentryNative.targets`; a Sentry upgrade follows the runbook in `win/win32/xpl/gnhsentrywin/README.md`.
- **Gamepad work needs a device.** Shared controller code needs the Linux package on a Steam Deck; a change specific to another platform needs that platform's build, and a WinUI 3 build verifies WinUI-specific controller changes only. Android and iOS builds verify nothing about controllers. See `client_gamepad_testing`.

## Quick Path (clean clone → running Windows app)

1. Install prerequisites (see `AGENTS.md` → Build Prerequisites and the GnollHack Wiki). For Android/iOS, also set up WSL with `build-essential gdb rsync zip openssh-server dos2unix bison flex`.
2. Install external assets — tilesets, FMOD sound banks, `ghsecrets.sjson` (see [External Assets](#external-assets)).
3. Start WSL SSH if building Android/iOS or needing XAML translation: `sudo service ssh start`.
4. Open `win/win32/vs/GnollHack.sln` (the **native solution**), pick a platform from the [Platform Configurations](#platform-configurations) table.
5. Choose Debug or Release and rebuild. **Match configuration between both solutions** — mismatching causes confusing link errors.
6. Open `win/win32/xpl/GnollHackM/GnollHackM.sln` (the **MAUI solution**), same config, build and run.
7. Verify artifacts from [Verifying the Build](#verifying-the-build).

Repeat steps 4–5 for C core / `dat/` / XAML changes; steps 6–7 for C# changes.

## External Assets

These files are **not** in the GnollHack repository. Without them the app has no graphics or sound. **Install before the first native build.**

| Asset | Source | Install To |
|-------|--------|-----------|
| 3 tileset PNGs (`gnollhack_64x96_transparent_32bits*.png`) | [GnollHackTileSet](https://github.com/hyvanmielenpelit/GnollHackTileSet) releases or build from source with [TileSetCompiler](https://github.com/hyvanmielenpelit/TileSetCompiler) | `win/win32/tileset/` |
| 12 FMOD `.bank` files (6 Desktop + 6 Mobile) | [GnollHackSoundSet](https://github.com/hyvanmielenpelit/GnollHackSoundSet) releases or build with FMOD Studio | `win/win32/bank/` (must contain `Desktop/` and `Mobile/` subdirs) |
| `ghsecrets.sjson` (webhook URLs, Azure keys) | Create from template with empty values | `win/win32/xpl/GnollHackM/Assets/` and `win/win32/xpl/GnollHackX/GnollHackX/Assets/` |

Tilesets reach the app via `copytilesetdroid.proj`; sound banks via `update-wsl-in-{debug,release}.bat`. Both run as part of `makedefsdroid`. If the banks/tilesets directories are empty, those steps fail. The Avalonia port does not use the staged copies: it reads `win/win32/tileset/*.png` and `win/win32/bank/Desktop/*.bank` directly.

`ghsecrets.sjson` template (all seven keys, empty values are fine for a local build):

```json
{
   "EncodedDefaultGamePostAddress": "",
   "EncodedDefaultDiagnosticDataPostAddress": "",
   "EncodedDefaultXlogRegisterationAddress": "",
   "EncodedDefaultXlogPostAddress": "",
   "EncodedDefaultXlogAntiForgeryToken": "",
   "EncodedDefaultAzureBlobStorageConnectionString": "",
   "EncodedDefaultSentryDSN": ""
}
```

`C:\wsl-in\` and `C:\wsl-out\` are created by the build itself (`makedefsdroid`'s `WriteLinesToFile`, the `update-wsl-in-*.bat` scripts, `afterdroidutils.proj`); nothing needs creating by hand. The `assetpack-maui*` folders are referenced only by a commented-out `RemotePostBuildEvent`.

For full details, see the [GnollHack Wiki](https://github.com/hyvanmielenpelit/GnollHackWiki/tree/main/Development) pages on tilesets, sound banks, and secrets.

## Two-Solution Build Architecture

### Step 1: Native solution (`win/win32/vs/GnollHack.sln`)

Compiles the C engine, runs all data compilation tools, performs XAML transformation. Key property files: `dirs.props` (path variables), `files.props` (source lists, `.des` files, `dlb` contents).

Building this solution: compiles build utilities → runs data pipeline → compiles native libraries → runs `makedefsdroid` (XAML + asset staging) → copies outputs to `GnollHackM/Platforms/`.

> The native solution also contains legacy Xamarin projects, ASCII/GUI game versions, and helper tools. Which compile depends on the solution platform. `MauiAll` and `MauiAndroid` skip legacy/Xamarin targets.

#### Platform Configurations

| Platform | Native libs | Win data | WSL pipeline + `makedefsdroid` | Needs |
|---|---|---|---|---|
| `Any CPU` | all: `gnollhackwin.dll`, `.so` (both ABIs), `libgnollhackios.a`, `libgnollhackmac.dylib`, `libgnollhacklinux.so`; also the legacy ASCII/GUI and Xamarin projects | ✓ | ✓ | WSL + Mac; Visual Studio 2022 |
| `MauiAndroid` | `gnollhackwin.dll`, `.so` (arm64-v8a + x86_64) | ✓ | ✓ | WSL |
| `MauiAll` | above + `libgnollhackios.a` (Debug only; Release does not build it) | ✓ | ✓ | WSL; Mac for Debug |
| `Android+Windows` | `gnollhackwin.dll`, `.so` (both ABIs) | ✓ | ✓ | WSL |
| `ARM64` | `.so` (both ABIs) | ✓ | ✓ | WSL |
| `x64` | `gnollhackwin.dll` | ✓ | **✗** | — |
| `Linux` | `libgnollhacklinux.so` | ✗ | ✓ | WSL |
| `macOS` | `libgnollhackmac.dylib` | ✗ | ✓ | WSL + Mac |
| `iPhone` | `libgnollhackios.a` | ✗ | ✓ | WSL + Mac |

**To build every library version, use `Any CPU`.** It is the platform normally used for a full build, and for checking that everything still builds. It builds the Apple libraries on the Mac, so a fully successful build needs the Mac reachable; without one, the iOS and macOS projects fail and the other platforms below build the rest. It also builds the legacy Xamarin projects, so it needs Visual Studio 2022, or Visual Studio 2026 with those projects unloaded.

**For MAUI work without a full build, prefer `MauiAndroid` or `MauiAll`.** `x64` is the only WSL-free platform but does **not** run `makedefsdroid` — no XAML translation, tileset staging, or sound-bank staging.

`Linux` builds `gnollhacklinux` and its dependencies — `makedefs`, `levcomp`, `dgncomp` and the four `*droid` data tools — and nothing else, for the Avalonia Linux flavor. It needs no Android C++ workload.

`macOS` builds `gnollhackmac` and the same data tools as `Linux`, and nothing else. It needs WSL for the data pipeline and the Mac for the library; see [Apple engine libraries over SSH](#apple-engine-libraries-over-ssh).

`Any CPU`, `Android+Windows` and `ARM64` also build the legacy Xamarin projects `GnollHackX.Android` and `GnollHackX.Android.Bindings`. Visual Studio 2026 has no Xamarin, so they need Visual Studio 2022, or Visual Studio 2026 with those projects unloaded. Unloading is IDE state that a command-line build ignores, and `vswhere -latest` returns VS 2026's MSBuild whenever VS 2026 is installed. `MauiAndroid` builds everything else `Android+Windows` does.

The solution defines four further platforms — `Android x64`, `iPhoneSimulator`, `Win32`, and `ARM` — which are legacy or niche; avoid them unless you specifically need one. `iPhoneSimulator` also builds `gnollhackios` and needs the Mac; `ARM` and `Win32` build neither Apple library.

Use the 64-bit MSBuild, as Visual Studio does. `-find MSBuild\**\Bin\MSBuild.exe` returns the 32-bit host, which fail-fasts with no output on Visual Studio's Linux projects (`gnollhacklinux` and the `*droid` data tools). Those projects set `AddSyntheticProjectReferencesForSolutionDependencies` to `false` (`WA-BUILD-0003`): otherwise a command-line solution build stops with `MSB4057` for `GetRecipeFile` on `makedefs.vcxproj`.

```powershell
$msbuild = & "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe" -latest -requires Microsoft.Component.MSBuild -find MSBuild\**\Bin\amd64\MSBuild.exe
& $msbuild win/win32/vs/GnollHack.sln /t:Rebuild /p:Configuration=Debug /p:Platform=MauiAndroid
```

### Step 2: MAUI solution (`win/win32/xpl/GnollHackM/GnollHackM.sln`)

Compiles the .NET MAUI app, bundling native libraries and assets from Step 1. Check `GnollHackM.csproj` for target frameworks and NuGet versions.

## Two Data Pipelines

Game data is compiled **twice** through independent toolchains with **separate file lists**.

| | Windows pipeline | Mobile pipeline (WSL) |
|---|---|---|
| Tools | `makedefs.exe`, `levcomp.exe`, `dgncomp.exe`, `dlb.exe` | `makedefsdroid`, `levcompdroid`, `dgncompdroid`, `dlbdroid` |
| Runs on | Windows, via `.proj` MSBuild targets | Ubuntu under WSL, over VS SSH |
| Archive file list | `<DlbList>` item group in `files.props` | **Hardcoded in `dlbdroid.vcxproj` `RemotePostBuildEvent`** |
| Output `nhdat` → | `GnollHackM/Platforms/Windows/gnh/` | `GnollHackM/Platforms/{Android,iOS}/gnh/` |

> ⚠️ **When you add a data file that must ship in `nhdat`, update both lists.**

## Data Pipeline Steps (Windows)

### 1. `makedefs.exe` (via `aftermakedefs.proj`)

Run each flag from the working directory shown — the tool resolves its inputs
and outputs relative to it.

| Flag | Working Dir | Output | Description |
|------|-------------|--------|-------------|
| `-v` | `util/` | `include/date.h` | Version and build date header |
| `-o` | `util/` | `include/onames.h` | Object name defines (from `objects.c`) |
| `-p` | `util/` | `include/pm.h` | Monster defines (from `monst.c`) |
| `-z` | `util/` | `include/vis_tab.h`, `src/vis_tab.c` | Visibility tables |
| `-d` | `dat/` | `dat/data` | Processed `data.base` |
| `-r` | `dat/` | `dat/rumors` | Merged `rumors.tru` + `rumors.fal` |
| `-s` | `dat/` | `dat/engrave`, `dat/epitaph`, `dat/bogusmon` | Text databases |
| `-q` | `dat/` | `dat/quest.dat` | Quest text |
| `-h` | `dat/` | `dat/oracles` | Oracle messages |
| `-e` | `dat/` | `dat/dungeon.pdf` | Dungeon definition (input for `dgncomp`) |
| `-a` | `dat/` | `include/animoff.h`, `include/animtotals.h` | Animation offset headers |

The mobile pipeline runs the same flags through `makedefsdroid.out` inside WSL.

#### `dos2unix` in WSL build commands (MANDATORY — do not remove)

The `makedefsdroid.vcxproj` commands begin with `dos2unix` calls over `dat/` text files, `libshare/defaults.gnh`, `sysconf`, `AndroidManifest.xml`, and `createassetpack.sh`. These convert CRLF (Windows checkout) to LF (Linux requirement) at build time. **Never delete these calls** — the build works on machines where files happen to be LF, and breaks on fresh Windows checkouts. When adding a new text file the WSL side reads, add a matching `dos2unix` call.

### 2. `levcomp.exe` (via `afterlevcomp.proj`)

Compiles `.des` files in `dat/` into binary `.lev` files. Input: `<Desfiles>` item group in `files.props`.
Run: `levcomp.exe <list of .des files>` from `dat/`.

### 3. `dgncomp.exe` (via `afterdgncomp.proj`)

Compiles `dat/dungeon.pdf` (from `makedefs -e`) → `dat/dungeon`.
Run: `dgncomp.exe dungeon.pdf` from `dat/`.

### 4. `dlb.exe` (via `afterdlb.proj`)

Packages data into `nhdat` archive. Input: the `<DlbList>` item group in `files.props` — **not** `dat/dlb.lst`, which is *generated* from `<DlbList>` on every build and is gitignored.
Run: `dlb.exe cIf dat/dlb.lst bin/$(Configuration)/$(Platform)/nhdat` from `dat/`.

## Native Library Compilation

| Platform | Library | Output Path | Build Method |
|----------|---------|-------------|--------------|
| Windows | `gnollhackwin.dll` | `GnollHackM/Platforms/Windows/libs/` | MSVC v143 (`gnollhackwin.vcxproj`, `OutDir` → MAUI directly) |
| Android | `libgnollhackdroid.so` | `GnollHackX.Android/libs/$(TargetArchAbi)/` | Clang via WSL SSH (`gnollhackdroid` = arm64-v8a, `gnollhackdroid2` = x86_64) |
| iOS | `libgnollhackios.a` (arm64) | `GnollHackM/Platforms/iOS/libs/` and `GnollHackX.iOS/Native References/` | clang over SSH on the Mac (`gnollhackios.vcxproj`, `Build-AppleEngine.ps1`) |
| macOS | `libgnollhackmac.dylib` (arm64 + x86_64) | `GnollHackM/Platforms/MacOS/libs/` | clang over SSH on the Mac (`gnollhackmac.vcxproj`, `Build-AppleEngine.ps1`) |

Post-build `.proj` files copy libraries and assets to `GnollHackM/Platforms/`:
- `aftergnollhackdll.proj` — FMOD DLLs + gnh data files → Windows
- `recoverwin.vcxproj` — `util/recover.c` with `GNH_MOBILE;GNH_WIN` (the engine's save layout, unlike `recover.vcxproj`'s tty build), built from `gnollhackwin.vcxproj`'s `AfterBuild` → `Platforms/Windows/gnh/recover.exe`
- `afterdroidutils.proj` — mobile `nhdat` + data from `C:\wsl-out\` → Android/iOS
- `aftergnollhackdroid.proj` — Android `.so` → MAUI Android
- `aftergnollhackios.proj` — `libgnollhackios.a` from `gnollhackios.vcxproj`'s output (property `GnhiOSLib`) → `Platforms/iOS/libs/` and `GnollHackX.iOS/Native References/`
- `CopyToMauiMacLibs` (target in `gnollhackmac.vcxproj`) — `libgnollhackmac.dylib` → `Platforms/MacOS/libs/` (gitignored)

### Android prerequisites: WSL with SSH, VS Connection Manager (127.0.0.1:22).

### iOS prerequisites: WSL + key-based SSH to the Mac (wiki page *Set Up SSH Access to the Mac for Native Libraries*) + Xcode on the Mac + Pair to Mac for the .NET MAUI app.

### Apple engine libraries over SSH

`gnollhackios.vcxproj` and `gnollhackmac.vcxproj` are Makefile-type projects with `x64` configurations. Both import `win/win32/vs/gnollhackapple.props`, which holds the shared engine source list and the build, rebuild and clean command lines, and run `win/win32/xpl/libapple/Build-AppleEngine.ps1`. The script packs the listed sources and the headers into a tar file, copies it to the Mac with Windows' OpenSSH `scp`, unpacks it beside `~/gnollhack-build/<ios|macos>-<Configuration>-<computer>-<hash>/tree` and copies into the tree only the files whose content changed (`sync-tree.sh`), runs `make -f engine.mk` there (`win/win32/xpl/libapple/engine.mk`) with Xcode's clang through `xcrun`, and copies the library back to the project's output folder. The objects stay on the Mac between runs. Each checkout (the hash is of its path) and each computer has its own folder, so worktrees never share objects.

- **Source list:** `gnollhackapple.props` lists the same engine sources as `gnollhacklinux.vcxproj`. Adding an engine source means adding it to both.
- **Mac host:** an OpenSSH host alias with key-based login: the script's `-SshHost`, else the environment variable `GNOLLHACK_MAC_SSH_HOST`, else `gnollhack-mac`. From MSBuild: `/p:GnhAppleExtraArgs="-SshHost <alias>"`. Every connection uses `BatchMode`, so a missing key or an unknown host key fails at once instead of prompting. The setup is on the wiki page *Set Up SSH Access to the Mac for Native Libraries*.
- **Mac address:** the Mac's IP address or host name is entered only as the alias's `HostName` in `%USERPROFILE%\.ssh\config`; nothing in the repository, the projects or the environment holds it. When the Mac moves to another network, editing that line is the whole update. The alias sets `HostKeyAlias` to its own name, so the host key is trusted under the alias, not the address; an entry without it fails with `GNHAB006` after an address change, because the new address is an unknown host under `BatchMode`.
- **Recompiling:** make recompiles a source when it, any header, `engine.mk` or the compiler flags and SDK
  (`obj/flags.txt`) changed. Change detection is by content, not by Windows file times. Every WSL data pipeline run
  rewrites `date_unix.h`, and every object depends on every header, so a solution build usually recompiles
  everything. `Rebuild` (`-Clean`) always does. `Clean` (`-CleanOnly`) deletes the local library and this checkout's
  folder on the Mac.
- **Output:** each source prints `Compiling <file>`, and its diagnostics follow as one block (`WA-BUILD-0004`), with
  paths rewritten to this checkout so that Visual Studio's Error List opens them.
- **Deployment targets:** `GHiOSDeploymentTarget` in `gnollhackios.vcxproj` and `GHMacDeploymentTarget` in `gnollhackmac.vcxproj` are the source of truth.
- **Errors:** the script prints canonical MSBuild errors: `GNHAB001` `ssh`, `scp` or `tar` missing (Windows' OpenSSH Client), `GNHAB002` the platform's `date_unix.h` missing, `GNHAB003` the source list missing or naming a file outside the repository, `GNHAB004` an invalid host alias or remote folder, `GNHAB005` packing failed, `GNHAB006` the Mac unreachable, `GNHAB007` the SDK unavailable (Xcode not selected or its license not accepted), `GNHAB008` preparing or cleaning the remote folder failed, `GNHAB009` copying or unpacking the sources failed, `GNHAB010` the build on the Mac failed (the compiler output is above it), `GNHAB011` copying the library or its `.dSYM` back failed, `GNHAB013` the macOS engine and its `.dSYM` have different arm64 UUIDs; warning `GNHAB012` means a clean could not reach the Mac.
- **Engine debug information (macOS):** make writes `libgnollhackmac.dylib.dSYM` with `dsymutil` whenever it is missing or older than the dylib, and the script copies it back beside the dylib. It is gitignored, and the Release publish uploads it to Sentry. This adds the `dsymutil` run and the compressed copy to every macOS engine build.
- **sentry-native for macOS:** `Platforms/MacOS/Packaging/Build-MacSentryNative.ps1` links the committed `libgnhsentry.dylib` on the Mac over the same SSH alias, with its own `GNHMS0nn` error codes. `win/win32/xpl/gnhsentrywin/README.md` says when to run it: after a Sentry upgrade or any edit to `gnhsentry.def` or `gnhsentry.exports`.
- **No `vcremote`, no PSCP:** the C++ build needs neither `vcremote` on the Mac nor download scripts. Pair to Mac is still needed for the .NET MAUI iOS app (`GnollHackM.csproj`).

## The `makedefsdroid` Project

`makedefsdroid` does **three unrelated jobs** (`win/win32/vs/makedefsdroid.vcxproj`, a Linux cross-compilation project):

| # | Job | Runs on | Needs WSL? |
|---|-----|---------|-----------|
| 1 | XAML transformation + tileset staging (`InitialBuild` target) | Windows MSBuild | No |
| 2 | Sound bank + asset-pack staging (`PreBuildEvent` → `update-wsl-in-*.bat`) | Windows batch | No |
| 3 | Linux `makedefs` data pipeline (`RemotePostBuildEvent`) | Ubuntu over SSH | **Yes** |

> **Job 1 runs even when job 3 fails.** `InitialTargets="InitialBuild"` executes before the WSL compile. Don't assume a `makedefsdroid` error means XAML wasn't regenerated — check timestamps.

### XAML Transformation

Auto-generates 32 MAUI XAML files (7 controls, 25 pages) from Xamarin source in `GnollHackX/GnollHackX/`. Key conversions: Xamarin→MAUI namespaces, `WidthRequest  =`→`MaximumWidthRequest=`, `HeightRequest  =`→`MaximumHeightRequest=`, `<Frame>`→`<Border>`, content wrapper stripping. See `maui_frontend` skill for the full conversion list, exempt files, and template for adding new XAML files.

App-level XAML (`App.xaml`, `AppShell.xaml`, `Resources/Styles/`) is **not** generated — edit directly in `GnollHackM`.

> ⚠️ **The transform is per-file hardcoded, not a wildcard.** New XAML files produce no MAUI output until a block is added to `makedefsdroid.vcxproj`. Replacement chains vary between files.

```powershell
& $msbuild win/win32/vs/makedefsdroid.vcxproj /t:Build /p:Configuration=Debug /p:Platform=x64
```

## Build Commands (MAUI Frontend)

```powershell
dotnet build win/win32/xpl/GnollHackM/GnollHackM.csproj -c Debug -f net10.0-windows10.0.19041.0  # Windows
dotnet build win/win32/xpl/GnollHackM/GnollHackM.csproj -c Debug -f net10.0-android               # Android
dotnet build win/win32/xpl/GnollHackM/GnollHackM.csproj -c Debug -f net10.0-ios                   # iOS
```

> Check `GnollHackM.csproj` for current target framework monikers — they change over time.

## Avalonia Port (Linux, Windows and macOS)

`GnollHackM.csproj` with `-p:GnhAvalonia=true` targets `net11.0` on Avalonia.Controls.Maui instead of the platform backends. The runtime identifier defaults to the host's, so a build on Windows is the `win-x64` flavor (development and testing only, not shipped; `Platforms/Windows` libs and `gnh`, data in `%LOCALAPPDATA%\GnollHack-Avalonia`), `-r linux-x64` is the shipped Linux flavor (`LINUX` and `SENTRY` defined), and `-r osx-arm64` is the macOS flavor (`MACOS` and `SENTRY` defined; packaged as a signed, notarized `GnollHack.app`, not released yet; see [macOS flavor](#macos-flavor)). All take the tilesets from `win/win32/tileset` and the Desktop banks from `win/win32/bank/Desktop` directly, so neither needs staged assets. Human-facing walkthroughs: wiki pages `Development/Avalonia Port`, `Development/Build Instructions for Avalonia Linux Version on Windows`, `Development/Build Instructions for Avalonia Windows Version on Windows`, `Development/Build Instructions for Avalonia macOS Version on Windows`.

- **Always run `dotnet` from `GnollHackM/Platforms/Avalonia`.** Its `global.json` pins the SDK and workload set; from the repository root or `GnollHackM` another SDK is chosen. First install the SDK named by `sdk.version` in that file (it may be a preview or release candidate), then run `dotnet workload install maui` in that folder, so that the pinned `workloadVersion` is installed.
- **No `-t:Compile`** for this flavor (MSB4057); verify with a full build, about 30 s.
- **Where Avalonia C# goes.** `Platforms/Avalonia/` holds what every runtime identifier compiles. C# that can only work on one OS goes in that OS's folder, `Platforms/Linux/` or `Platforms/MacOS/`, beside its native libraries and packaging, in the subfolder it would have under `Platforms/Avalonia/` (`Cef/`, `Rendering/`). `GnollHackM.csproj` compiles each OS folder only for its runtime identifier (`MauiPlatformSpecificFolder` items; MAUI drops every other `.cs` under `Platforms/`). A file shared by several OSes stays in `Platforms/Avalonia/` with its OS branches under `#if LINUX` / `#if MACOS`, and files in an OS folder keep their file-level `#if` for projects that link them directly.

```powershell
cd win\win32\xpl\GnollHackM\Platforms\Avalonia
dotnet build ..\..\GnollHackM.csproj -p:GnhAvalonia=true                     # Windows flavor → bin\Debug\net11.0\win-x64\GnollHackM.exe
dotnet publish ..\..\GnollHackM.csproj -p:GnhAvalonia=true -r win-x64 -f net11.0 -c Release --self-contained true      # tester folder
dotnet publish ..\..\GnollHackM.csproj -p:GnhAvalonia=true -r linux-x64 -f net11.0 -c Release --self-contained true -p:GnhSentryUpload=false
```

Publish needs `-f net11.0`. A Release publish uploads Sentry symbols when the machine-wide `SENTRY_AUTH_TOKEN` is set; `-p:GnhSentryUpload=false` suppresses it. `Build-LinuxPackages.ps1 -NoSentryUpload` passes it.

### Windows flavor

1. **Native solution**: build `GnollHack.sln` with `x64` (no WSL): `& $msbuild win/win32/vs/GnollHack.sln /t:Rebuild /p:Configuration=Debug /p:Platform=x64`. It writes `Platforms/Windows/libs/gnollhackwin.dll` and `Platforms/Windows/gnh/nhdat`. The Avalonia build takes whichever `gnollhackwin.dll` was built last, so build the native solution in Release before a Release publish. Editing GnollHackX XAML still needs `MauiAndroid` (WSL) to regenerate the MAUI XAML.
2. **Build** from `Platforms/Avalonia`: `dotnet build ..\..\GnollHackM.csproj -p:GnhAvalonia=true`.
3. **Run** `GnollHackM/bin/Debug/net11.0/win-x64/GnollHackM.exe`. To debug, attach Visual Studio to it (Managed .NET Core); starting `GnollHackM.sln` runs the WinUI 3 app instead.

`GnhCheckAvaloniaNativeInputs` in `GnollHackM.csproj` stops either flavor's build with `GNHNI001` (engine or `nhdat`), `GNHNI002` (banks) or `GNHNI003` (tilesets), naming each missing file. `-p:GnhAllowMissingNativeInputs=true` makes them warnings, for a compile check on a machine without them.

### macOS flavor

`-r osx-arm64` (Apple Silicon) and `-r osx-x64` (Intel) build the macOS flavor, with the `MACOS` and `SENTRY` defines. It is not shipped yet. Step 4 packages it as a signed `GnollHack.app`, DMG and Steam depot content; steps 2-3 are the unbundled development loop. Releases are arm64 only. Controllers work through GameController (`DEVEL/gamepad_controls.md`). C# files used only by this flavor go in `Platforms/MacOS/` (see "Where Avalonia C# goes" above).

1. **Native solution**: build `GnollHack.sln` with the `macOS` platform in Release (WSL and the Mac; see [Apple engine libraries over SSH](#apple-engine-libraries-over-ssh)). `CopyToMauiMacLibs` puts `libgnollhackmac.dylib` in `Platforms/MacOS/libs/`, beside FMOD's committed `libfmod.dylib` and `libfmodstudio.dylib`; the game data is the Android-format `Platforms/Android/gnh`, as for Linux.
2. **Publish** from `Platforms/Avalonia`: `dotnet publish ..\..\GnollHackM.csproj -p:GnhAvalonia=true -r osx-arm64 -f net11.0 -c Release --self-contained true -p:GnhSentryUpload=false`. The publish runs on Windows; the .NET SDK signs the apphost ad hoc.
3. **Run** on the Mac from Terminal, unbundled: copy the `publish` folder over (a tar file made on Windows loses the executable bits, so `chmod +x GnollHackM` there), ad-hoc sign `libgnollhackmac.dylib` (`codesign --force --sign -`: the linker signs only its arm64 slice), and start `./GnollHackM`. Data goes to `~/Library/Application Support/GnollHack`. The agents' copy is `~/gnollhack-dev/osx-arm64.tar` plus a `VERSION` file (the GnollHack commit and `git status --short`), unpacked by `~/gnollhack-dev/deploy-mac-build.sh`, a script that lives only on the Mac: it replaces `~/gnollhack-dev/osx-arm64` with the tar's content, moves `VERSION` into it, sets the executable bits of `GnollHackM` and `createdump`, ad-hoc signs `GnollHackM` and every `*.dylib` that `codesign -v` rejects, and prints `VERSION`. Agents never start the game on the Mac: a launch over SSH opens windows and alerts on the screen of whoever is at it. `~/gnollhack-dev/GnollHackDev.app`, a wrapper whose `Contents/MacOS/launch` execs this copy, lets Steam add it as a non-Steam game.
4. **Package**: `Platforms/MacOS/Packaging/Build-MacPackage.ps1 -Targets App,Dmg,Steam`, run on Windows.
   - It publishes for osx-arm64 as a single file (`PublishSingleFile`, with native libraries and content left out of the bundle), stages the publish with LF copies of the packaging files, and runs `package-mac.sh` on the Mac over SSH (`-SshHost`, else `GNOLLHACK_MAC_SSH_HOST`, else `gnollhack-mac`, as in `Build-AppleEngine.ps1`, but the alias may not contain `@`).
   - On the Mac, it assembles `GnollHack.app`: Mach-O files in `Contents/MacOS`, `gnh`, `banks` and `tileset` in `Contents/Resources`, where `GHAvaloniaPaths.AssetDirectory` finds them in a bundle. It makes the `.icns` from `Resources/AppIcon/macOS/appicon-macos.png`, a 1024 x 1024 master that `Platforms/MacOS/Packaging/New-MacAppIcon.ps1` composes from the iOS icon's two layers (`Resources/AppIcon/iOS/appicon.png` and `appiconios.png`) in Apple's macOS 11+ icon layout; run it again and commit the output when either layer changes. It writes `Info.plist` from `Info.plist.template` (bundle id `com.soundmindgames.GnollHackMac`, distinct from the iOS app's).
   - It signs every Mach-O and then the bundle with the hardened runtime and `GnollHack.entitlements` (the .NET defaults; no sandbox, which Steam does not support).
   - With `-NotaryProfile`, it notarizes and staples with `notarytool`; then it makes the DMG and the Steam content with `app_build_mac.vdf` and `depot_build_mac.vdf`.
   - `-SigningIdentity` defaults to `-` (ad hoc, which runs only on the Mac that made it). A release passes `-SigningIdentity "Developer ID Application: Hyvan mielen pelit ry (CUG8RS847T)" -NotaryProfile gnollhack-notary`, as verified on 2026-11-05: both notarizations were accepted, and the stapled app and DMG pass `spctl` as "Notarized Developer ID".
   - **Signing keychain:**
     - An SSH session cannot use the login keychain, which stays locked there: `codesign` fails with `errSecInternalComponent`, and `notarytool` with `keychainLocked`. So the Developer ID identity, the Apple intermediate certificate and the `gnollhack-notary` profile (an App Store Connect Team API key) live in `~/Library/Keychains/gnollhack-signing.keychain-db`.
     - Its password is in `~/.gnollhack/signing-keychain-password` (mode 600). `package-mac.sh` unlocks it and passes `--keychain` to `codesign` and `notarytool`. `-Keychain` and `-KeychainPasswordFile` override the defaults; `-Keychain ''` passes no keychain. Over SSH that fails, because the locked login keychain is used, so it serves only `package-mac.sh` run by hand in Terminal on the Mac.
     - `codesign` takes the certificate from `--keychain` but looks up the private key along the user's search list. Because the login keychain holds a second copy of the identity, the script puts the signing keychain first for the run and restores the original order on exit. The per-session (`-d dynamic`) search list does not exist on macOS 26.
     - The one-time setup, step by step, is on the wiki page `Development/Build Instructions for Avalonia macOS Version on Windows`.
     - Pitfalls seen in the setup: Terminal may not read `~/Desktop` ("Operation not permitted"); `set-key-partition-list` must run after the key is imported.
     - The search-list reordering is WA-BUILD-0006 in `DEVEL/workarounds/build_tooling.md`.
   - `-Targets Steam` needs `-SteamAppId` and `-SteamDepotId` (`GNHMP004`); the app is `3558190` and its macOS depot `3558193`. `-SteamBranch` names a beta branch or stays empty, because SteamPipe cannot set the default branch live. The content comes back to `<results>\steam\` with its `.vdf` files, as a tar without Apple metadata, every file checked against its SHA-256 on the Mac (`GNHMP012`), and stays on the Mac too. A person uploads it from either; a depot uploaded from Windows installs with every file executable, which the code signature does not cover. The Steamworks launch option is `GnollHack.app` with the arguments `-store steam`; Steam then starts `Contents/MacOS/GnollHackM -store steam` directly (verified 2026-11-05).
   - Build releases from a clean tree: `BUILD-INFO` marks a dirty one `-dirty`. A release also needs the team's real `ghsecrets.sjson`; the template's empty Sentry DSN turns Sentry off.
   - **Agents and the Mac:** an agent may run `Build-MacPackage.ps1` ad hoc, and with the Developer ID identity when the user asks for a release, because the script unlocks the signing keychain without showing its password. An agent never reads `~/.gnollhack/signing-keychain-password`, never runs `security` commands that create, export or change keychains, never runs `notarytool store-credentials`, never handles the `.p12`, the `.p8` or any password, never uploads to Steam, and never starts the game on the Mac.
   - Depending on `-Targets`, the zip, the DMG, `GnollHack-<version>-macos-arm64.SHA256SUMS`, `BUILD-INFO` and `steam\` come back to `GnollHackM\bin\MacPackages\<version>-<build>\`; the script never uploads. `-DevCopy` also puts the app at `~/gnollhack-dev/GnollHack.app`.
   - Errors are `GNHMP0nn` (Windows) and `GNHMP1nn` (Mac).

- **Rendering**: Metal, then OpenGL, then software, unless the Metal Rendering setting is off (OpenGL, then software; read at startup, so a change applies after a restart). `GNOLLHACK_MAC_RENDERING=metal`, `=opengl` or `=software` overrides the setting for one run. The GPU control prints `GnollHack: Avalonia GL: GPU context backend <Metal|OpenGL>` on standard error when its context changes; no such line means the map is not on the GPU.
- **GPU budget**: Skia's resource cache is 2 GiB on macOS and 1.5 GiB elsewhere (`SkiaOptions` in `AvaloniaDesktopProgram`). On Metal some levels need about 1.3 GB, against about 0.7 GB on OpenGL, and a full cache re-uploads textures every frame (about 66 ms per paint). Skia keeps unused textures until the cache is full, so a long session can approach the limit. The app log's `Avalonia GL: Skia GPU cache` line shows the use every 10 s. GnollHack's GPU cache setting does not apply to the Avalonia build.
- **Logs**: started outside a terminal, standard output and error go to `~/Library/Logs/GnollHack/GnollHack.log` (`GHMacStderrLog`); the previous run's is `GnollHack.previous.log`.
- **Menu bar**: `GHMacApplicationMenu` names the application GnollHack (Avalonia's default is "Avalonia Application") and replaces Avalonia's default application menu items, so that Quit (Cmd+Q) and Close Window (Cmd+W) go through the game's own close flow; a quit from the Dock or at logout saves without asking.
- **Option key**: by default Option is the meta key, and the letter comes from the key's unmodified character in the current layout (`GHMacKeyboardLayout`), not from Avalonia's US-position key. With the macOS setting **Allow Option Keys** on, Option types any printable ASCII character the layout puts there, such as `@` on Option+2 in Finnish (`GHAvaloniaKeyText.ClassifyMacOption`).
- **Sentry**: `GHAvaloniaSentry` reports exceptions, breadcrumbs and release health, with the tag `macos.package` (`bundle`, `steam` or `unbundled`). Native crashes are captured by sentry-native in `libgnhsentry.dylib` and uploaded at the next start. A Release publish with symbol upload also uploads the engine's `libgnollhackmac.dylib.dSYM` and fails when it is missing or does not match the dylib. `#crashtest` needs a Debug engine. `GNOLLHACK_SENTRY_TEST=1` sends one test message at startup, on Linux too. Events need a DSN: the real `ghsecrets.sjson`, or `SENTRY_DSN` for one run. The release is Sentry .NET's default, `GnollHackM@<version>.<build>+<commit>`, as seen in GNOLLHACK-J3. A build packaged with `-SkipPublish` after a `-NoSentryUpload` publish has no symbols in Sentry; upload its `stage\publish\GnollHackM.pdb` with the Sentry package's `sentry-cli debug-files upload -t portablepdb` (org and project from `SENTRY_ORG` and `SENTRY_PROJECT`).
- **FMOD**: `libfmodstudio.dylib` refers to its core as `@rpath/libfmod.dylib` and has no run paths, so `AvaloniaDesktopProgram.ResolveMacLibrary` loads `libfmod.dylib` from the application directory first.
- **Web view**: Avalonia's `NativeWebView` with the WKWebView adapter, part of the system.

### Native prerequisites of the Linux flavor

**One WSL distribution**, any Ubuntu LTS from 20.04 to 26.04, on WSL 2 (26.04's `tar` fails on WSL 1, and WSLg testing needs WSL 2; 20.04 also works on WSL 1). It runs sshd for Visual Studio's Connection Manager (`127.0.0.1:22`), the WSL data pipeline, the Linux builds, packaging and testing. Every GnollHack-built Linux binary (engine, `recover`, `libsentry.so`) compiles and links against the **glibc 2.27 sysroot** in `/opt/gnollhack/sysroot-glibc-2.27`, so the host's glibc never reaches the output; install it once per distribution with `Platforms/Linux/Packaging/Install-LinuxSysroot.ps1` (pinned, hash-checked Ubuntu 18.04 packages; it makes the packages' absolute dev symlinks relative, without which `libdl.so` resolves to the host's empty stub). Its link options are `--sysroot` plus `-L<sysroot>/usr/lib/x86_64-linux-gnu -L<sysroot>/lib/x86_64-linux-gnu` (Ubuntu's GCC searches the host's multiarch directory before the sysroot's), `-B<sysroot>/usr/lib/x86_64-linux-gnu` (the sysroot's start files), and `-Wl,-z,defs` (a function glibc 2.27 lacks would otherwise link unversioned and pass every `GLIBC_` floor check). The `Build-Linux*.ps1` and installer scripts default to the default WSL distribution, print it, and refuse `docker-desktop*`; `-WslDistro` overrides.

**C standard:** Visual Studio's Linux toolset already passes `-std=c11` (`CLanguageStandard` default in `Linux.props`); do not add another. GCC 14+ makes implicit declarations errors, so `levcompdroid` and `dgncompdroid` add `-Wno-error=implicit-function-declaration` for the lex files' `fileno`, as the engine does. The Unix hints (`sys/unix/hints/linux*`) set `-std=gnu17`, since GCC 15 defaults to C23, where `E void srand48();` conflicts with glibc.

**Native build:** build `GnollHack.sln` with the `Linux` platform in **Release**. It runs the WSL data pipeline, which gives Linux the Android-format `nhdat` (`Platforms/Android/gnh/*`) and writes `win/win32/xpl/gnollhacklinux/date_unix.h` (gitignored), and then `gnollhacklinux.vcxproj`. That remote-compiles the engine over the Connection Manager's SSH connection against the sysroot (`GnhLinuxSysroot`; a `RemotePreBuildEvent` stops with a clear message when the sysroot's stamp file is missing), and `CopyToMauiLinuxLibs` copies it to `Platforms/Linux/libs/`. **Build Release before packaging**: `CopyToMauiLinuxLibs` runs after every build of either configuration, and `Build-LinuxPackages.ps1` ships whatever library is there, so a later Debug build replaces the one a package would ship. That is how every platform's engine works: either configuration can be tested, and a Release build is made before shipping. From the command line use the 64-bit `$msbuild` from [Platform Configurations](#platform-configurations):

```powershell
wsl -d <distro> -u root -e /usr/sbin/service ssh start          # plain 'service' is not on root's -e PATH
& $msbuild win/win32/vs/GnollHack.sln /t:Rebuild /p:Configuration=Release /p:Platform=Linux /v:m
```

`Could not connect to the remote system` means sshd is not running or the Connection Manager entry is missing.

### Linux packages

The Linux build ships as an **AppImage** on GitHub releases and as **Steam depot content**, plus a tester tar.gz, all from one self-contained `linux-x64` publish. One script builds all three:

```powershell
win/win32/xpl/GnollHackM/Platforms/Linux/Packaging/Build-LinuxPackages.ps1                  # all targets
win/win32/xpl/GnollHackM/Platforms/Linux/Packaging/Build-LinuxPackages.ps1 -Targets AppImage -SkipPublish
```

- **Prerequisite:** `Platforms/Linux/libs/libgnollhacklinux.so` (gitignored), built by the `Linux` solution platform. The script stops without it.
- **Sentry:** with `SENTRY_AUTH_TOKEN` set, the script's Release publish uploads the release's debug symbols; `-NoSentryUpload` publishes with `-p:GnhSentryUpload=false` (it has no effect with `-SkipPublish`).
- **Source of truth:** the version comes from `ApplicationDisplayVersion` and `ApplicationVersion` in `GnollHackM.csproj`. The pinned `appimagetool` and type-2 runtime, with their SHA-256 values, are constants at the top of the script, and must be present in WSL under `~/.cache/gnollhack-packaging/tools/`. The script never downloads.
- **Where it runs:** the publish runs on Windows, from `Platforms/Avalonia`, whose `global.json` pins the SDK. Staging and packaging run in WSL (`-WslDistro`, default the default WSL distribution), in the WSL home, so Linux permissions survive. That distribution needs `rsync` and `binutils` (`strip`, `readelf`, `objdump`). Outputs land in `GnollHackM/bin/LinuxPackages/<version>/`, which is gitignored; the staged tree stays at `~/.cache/gnollhack-packaging/stage-<version>/`. A full run takes a few minutes and ends with a size and SHA-256 summary.
- **Pinned AppImage tools:** the file names in `$AppImageToolFile` and `$AppImageRuntimeFile` are the GitHub release assets `appimagetool-x86_64.AppImage` (AppImage/appimagetool) and `runtime-x86_64` (AppImage/type2-runtime) of the versions named in the comment above them, saved under the pinned names. Downloading them is the user's call, never the script's.
- **Testing in WSL:** run the staged `stage-<version>/gnollhack/GnollHackM`, or the AppImage with `--appimage-extract-and-run`, in a WSL 2 distribution with `GALLIUM_DRIVER=d3d12` (WSLg GPU). Never run from `/mnt/c`. Game data goes to `~/.local/share/GnollHack`.
- **Testing on a Steam Deck:** `Deploy-SteamDeck.ps1` copies the staged `stage-<version>/gnollhack/` from WSL to `~/gnollhack-dev/` on the Deck over SSH (rsync, or tar when the Deck has no rsync) and installs `run-gnollhack.sh`, the target of a Non-Steam Game shortcut, which logs each run to `~/gnollhack-dev/logs/` and sources an optional `dev.env`. It works over Wi-Fi and wired Ethernet alike: `-DeckHost` is resolved on Windows, because WSL cannot resolve `.local` names, and the host key is checked under `-HostKeyAlias` (default `steamdeck`), so every route shares one trusted key. It never builds, never handles a password, and refuses an unknown host key; its first run prints the one-time `ssh-copy-id` command. Run any `Build-LinuxPackages.ps1` target first. When to do this, the full sequence, and what counts as tested are in `client_gamepad_testing`.
- **Steam:** the content targets the Steam Linux Runtime 4.0 (steamrt4) by default, with the two libraries `libcef.so` needs from `Platforms/Linux/libs-steamrt4/` added. `-SteamRuntime sniper` builds for SLR 3.0 without them. The `.vdf` templates in `Packaging/steam/` are filled from `-SteamAppId`, `-SteamDepotId`, `-SteamBranch` and `-BuildDescription`. Upload with `steamcmd` or ContentBuilder from the Windows copy in `bin\LinuxPackages\`, or from the WSL copy the summary names: a depot uploaded from Windows installs with every file executable.
- **Steam Input configuration:** the Steam target also copies the `.vdf` files of `Packaging/steam/input/` into the content root as `steam_input/`: the hand-written Action Manifest `gnollhack_input_manifest.vdf` and the layout `controller_neptune.vdf`, which is exported from the Steam client, never hand-written; the script warns when the layout is missing. The gamepad backends are gated by `GNOLLHACK_GAMEPAD_EVDEV` (`auto`/`off`/`any`) and `GNOLLHACK_GAMEPAD_KEYS` (`auto`/`on`/`off`), with diagnostics from `GNOLLHACK_GAMEPAD_LOG=raw` and `GNOLLHACK_KEY_LOG=1`; on a Deck, set them in the deploy folder's `dev.env`. The controls, the layout, its publishing in Steamworks and the gates are in `DEVEL/gamepad_controls.md`.
- **Engine debug information:** the staged `libgnollhacklinux.so` ships stripped with `strip --strip-debug` (about 10 MB instead of 34 MB), keeping its build ID and symbol table; the script stops if the build ID changes. The repository copy keeps its DWARF for the Sentry symbol upload. `-KeepDebugInfo` ships it unstripped.
- **glibc and libstdc++ floor:** every ELF file in the staged publish must need no newer glibc than `$MaxGlibcVersion` in `Build-LinuxPackages.ps1`, and no newer system libstdc++ than `$MaxGlibcxxVersion` and `$MaxCxxabiVersion` (libstdc++ is not bundled on Linux, unlike Android's `libc++_shared.so`); the script stops otherwise, naming each file. GnollHack's own binaries meet the glibc floor through the sysroot (above); `Build-LinuxRecover.ps1` and `Build-LinuxSentryNative.ps1` check theirs against the same floor in their own `$MaxGlibcVersion`.
- **Save recovery utility:** `Build-LinuxRecover.ps1` builds `util/recover.c` in WSL with the release hints `sys/unix/hints/linux` (never `linux-asan-debug`, which is for local debugging), strips and checks it, and writes `Platforms/Linux/gnh/recover`, which is committed like `libsentry.so` and shipped as `gnh/recover` with mode 755. Rerun it after a change to `util/recover.c` or the save file headers it reads.
- **Licences:** `Platforms/Linux/licenses/cef/` holds CEF's `LICENSE.txt` and Chromium's `CREDITS.html`. `CREDITS.html` is the credits page built into CEF's `resources.pak`, extracted locally (brotli). **Refresh both with every CEF update.**
- **Design and decisions:** plans repository, `hyvanmielenpelit/GnollHack/2026-10-20/linux_distribution/implementation_plan_v2.md`.

## Verifying the Build

A green MSBuild result is not proof artifacts exist. Check these after building the native solution:

| Artifact | Produced by |
|---|---|
| `GnollHackM/Platforms/Windows/libs/gnollhackwin.dll` | `gnollhackwin.vcxproj` |
| `GnollHackM/Platforms/Android/libs/{arm64-v8a,x86_64}/libgnollhackdroid.so` | `aftergnollhackdroid.proj` |
| `GnollHackM/Platforms/iOS/libs/libgnollhackios.a` | `gnollhackios.vcxproj` on the Mac, copied by `aftergnollhackios.proj` (`iPhone`, `iPhoneSimulator`, `Debug\|MauiAll`, `Any CPU`) |
| `GnollHackM/Platforms/MacOS/libs/libgnollhackmac.dylib` | `gnollhackmac.vcxproj` on the Mac, copied by `CopyToMauiMacLibs` (`macOS`, `Any CPU`; the last configuration built) |
| `GnollHackM/Platforms/{Windows,Android,iOS}/gnh/nhdat` | `afterdlb.proj` / `afterdroidutils.proj` |
| `GnollHackM/Platforms/Windows/gnh/recover.exe` | `recoverwin.vcxproj` (from `gnollhackwin.vcxproj`) |
| `GnollHackM/Platforms/Linux/gnh/recover` (committed) | `Build-LinuxRecover.ps1` (manual) |
| `GnollHackM/Platforms/Linux/libs/libgnollhacklinux.so` | `gnollhacklinux.vcxproj` (`Linux` platform; the last configuration built) |
| `win/win32/xpl/gnollhacklinux/date_unix.h` | `afterdroidutils.proj` (WSL pipeline) |
| `win/win32/xpl/gnollhackmac/date_unix.h` | `afterdroidutils.proj` (WSL pipeline) |
| `GnollHackM/Platforms/*/tileset/*.ghpng` (3 each; WinUI 3, Android, iOS — the Avalonia port reads `win/win32/tileset`) | `copytilesetdroid.proj` |
| `GnollHackM/Platforms/*/banks/*.bank` (6 each; WinUI 3, Android, iOS — the Avalonia port reads `win/win32/bank/Desktop`) | `update-wsl-in-*.bat` |
| 32 generated `.xaml` in `GnollHackM/` | `makedefsdroid` `InitialBuild` |

## Troubleshooting

| Symptom | Likely cause |
|---|---|
| Build succeeds but app uses stale native library | Solution platform didn't build that library — check [Platform Configurations](#platform-configurations). |
| XAML changes don't appear in GnollHackM | `makedefsdroid` didn't run (e.g. `x64` platform), or file has no block in `makedefsdroid.vcxproj`. |
| `GnollHackM` can't resolve new `x:Name` | Rebuild `makedefsdroid`, then rebuild `GnollHackM`. |
| No sound | Sound banks not staged. Rebuild `makedefsdroid`; confirm `win/win32/bank/` has `Desktop/` and `Mobile/`. |
| No tile graphics | Tilesets missing from `win/win32/tileset/`, or `copytilesetdroid.proj` didn't run. |
| `bad interpreter` in WSL step | Missing `dos2unix` call or package. |
| WSL connection refused | Run `sudo service ssh start`; check VS Connection Manager. |
| `MSB4057: The target "GetRecipeFile" does not exist` on `makedefs.vcxproj`, command line only | A Linux project lost `AddSyntheticProjectReferencesForSolutionDependencies=false` (`WA-BUILD-0003`). |
| Link/runtime errors after switching Debug↔Release | Rebuild both solutions in the same config. |
| Data file works on Windows but missing on mobile | Added to `<DlbList>` only — see [Two Data Pipelines](#two-data-pipelines). |
| Avalonia build picks the wrong SDK or fails to restore | `dotnet` not run from `Platforms/Avalonia`, or its pinned SDK/workload missing. |
| `Build-LinuxPackages.ps1`: `Missing ...libgnollhacklinux.so` | Build `GnollHack.sln` with the `Linux` platform in Release — see [Native prerequisites](#native-prerequisites-of-the-linux-flavor). |
| Avalonia build: `GNHNI001`, `GNHNI002` or `GNHNI003` | A native input is missing; build or install what the message names. |
| `... needs GLIBC_2.xx, above the ... floor` | A Linux binary was built without the sysroot; run `Install-LinuxSysroot.ps1` in the distribution that built it, then rebuild. |
| `error: the glibc 2.27 sysroot is missing` | Run `Install-LinuxSysroot.ps1` in the distribution the Connection Manager points to. |
| `undefined reference to '...'` linking a Linux binary | `-z defs` caught a function glibc 2.27 lacks (or keeps in `libdl`/`librt`); avoid it or link its library. |
| `error GNHAB006: Cannot connect to the Mac` | The Mac is asleep or unreachable, its address changed (update `HostName` in `%USERPROFILE%\.ssh\config`), Remote Login is off, or `ssh gnollhack-mac` (or the alias in use) does not log in without a password. See [Apple engine libraries over SSH](#apple-engine-libraries-over-ssh). |
| `error GNHAB007: The ... SDK is not available on the Mac` | Xcode is missing, not selected, or its license is not accepted: on the Mac, `sudo xcode-select -s /Applications/Xcode.app/Contents/Developer` and `sudo xcodebuild -license accept`. |
| `error GNHAB002: ...date_unix.h is missing` | The WSL data pipeline has not run: build `GnollHack.sln` with the `macOS` or `iPhone` platform, which builds the `*droid` tools first. |
| `Build-MacPackage.ps1 ... error GNHMP002` | `libgnollhackmac.dylib`, `nhdat` or the macOS icon master missing: build `GnollHack.sln` with the `macOS` platform in Release, or run `New-MacAppIcon.ps1`. |
| `GNHMP008` | The Mac is unreachable, as `GNHAB006`. |
| `GNHMP010` after a `GNHMP1nn` | `package-mac.sh` failed on the Mac; the `GNHMP1nn` line names the step. |
| `GNHMP012` | A file of the Steam content differs after the copy to Windows; the file is named. Run the build again. |
| `errSecInternalComponent` / `GNHMP106`, `keychainLocked` / `GNHMP108`, `GNHMP114`, `GNHMP115` | Signing keychain problems; the person's fixes are in the macOS wiki page's Troubleshooting. Agents report them and never repair keychains. |
| `GNHMP109` | Apple rejected the notarization; its log is printed above and names the file. |

More troubleshooting on the GnollHack Wiki page *Troubleshooting Building GnollHack with .NET MAUI*.

## Related Skills

- **`game_data_files`** — `.des` file syntax, dungeon definitions, text databases
- **`adding_game_content`** — Adding monsters, objects, artifacts (includes `makedefs` flags)
- **`maui_frontend`** — MAUI frontend, XAML pipeline details, SkiaSharp rendering
- **`debugging_gnollhack`** — Wizard mode, panic logs, running the MAUI app locally
- **`client_subagent_guidelines`** — Build dependency chains constraining parallel agent work
- **`client_gamepad_testing`** — When a Steam Deck deployment is required, and the test handover
