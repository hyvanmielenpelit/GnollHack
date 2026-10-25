---
name: build_pipeline
description: Guide on building GnollHack native C libraries, processing assets with makedefs/levcomp/dgncomp/dlb, packaging nhdat, and compiling the C# .NET MAUI frontend for Android, iOS, and Windows, plus the Avalonia port (GnhAvalonia=true) for Linux (AppImage, Steam depot, tar.gz) and Windows.
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
- **Native Sentry binaries.** `gnhsentry.dll` and `libsentry.so` are committed builds checked against `GnhSentryVersion` and the vendored `sentry.h` by `GnhSentryNative.targets`; a Sentry upgrade follows the runbook in `win/win32/xpl/gnhsentrywin/README.md`.

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

Tilesets reach the app via `copytilesetdroid.proj`; sound banks via `update-wsl-in-{debug,release}.bat`. Both run as part of `makedefsdroid`. If the banks/tilesets directories are empty, those steps fail.

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

**`C:\wsl-in\` must exist before the first build** — the asset-pack staging uses plain `copy` with no `mkdir`. Create: `C:\wsl-in\assetpack\Assets\tileset\`, `C:\wsl-in\assetpack2\`, `C:\wsl-in\assetpack-maui\`, `C:\wsl-in\assetpack-maui2\`. (`C:\wsl-out\` is created automatically.)

For full details, see the [GnollHack Wiki](https://github.com/hyvanmielenpelit/GnollHackWiki/tree/main/Development) pages on tilesets, sound banks, and secrets.

## Two-Solution Build Architecture

### Step 1: Native solution (`win/win32/vs/GnollHack.sln`)

Compiles the C engine, runs all data compilation tools, performs XAML transformation. Key property files: `dirs.props` (path variables), `files.props` (source lists, `.des` files, `dlb` contents).

Building this solution: compiles build utilities → runs data pipeline → compiles native libraries → runs `makedefsdroid` (XAML + asset staging) → copies outputs to `GnollHackM/Platforms/`.

> The native solution also contains legacy Xamarin projects, ASCII/GUI game versions, and helper tools. Which compile depends on the solution platform. `MauiAll` and `MauiAndroid` skip legacy/Xamarin targets.

#### Platform Configurations

| Platform | Native libs | Win data | WSL pipeline + `makedefsdroid` | Needs |
|---|---|---|---|---|
| `MauiAndroid` | `gnollhackwin.dll`, `.so` (arm64-v8a + x86_64) | ✓ | ✓ | WSL |
| `MauiAll` | above + `libgnollhackios.a` | ✓ | ✓ | WSL + Mac |
| `Android+Windows` | `gnollhackwin.dll`, `.so` (both ABIs) | ✓ | ✓ | WSL |
| `ARM64` | `.so` (both ABIs) | ✓ | ✓ | WSL |
| `x64` | `gnollhackwin.dll` | ✓ | **✗** | — |
| `iPhone` | `libgnollhackios.a` | ✗ | ✓ | WSL + Mac |

**For MAUI work, prefer `MauiAndroid` or `MauiAll`.** `x64` is the only WSL-free platform but does **not** run `makedefsdroid` — no XAML translation, tileset staging, or sound-bank staging.

The solution defines five further platforms — `Android x64`, `iPhoneSimulator`, `Win32`, `ARM`, and `Any CPU` — which are legacy or niche; avoid them unless you specifically need one. `Any CPU` in particular is not a safe default: it builds the legacy ASCII/GUI targets and the Xamarin projects as well.

```powershell
$msbuild = & "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe" -latest -requires Microsoft.Component.MSBuild -find MSBuild\**\Bin\MSBuild.exe
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
| iOS | `libgnollhackios.a` | `GnollHackX.iOS/Native References/` | Xcode via vcremote on Mac (`gnollhackios.vcxproj`) |

Post-build `.proj` files copy libraries and assets to `GnollHackM/Platforms/`:
- `aftergnollhackdll.proj` — FMOD DLLs + gnh data files → Windows
- `recoverwin.vcxproj` — `util/recover.c` with `GNH_MOBILE;GNH_WIN` (the engine's save layout, unlike `recover.vcxproj`'s tty build), built from `gnollhackwin.vcxproj`'s `AfterBuild` → `Platforms/Windows/gnh/recover.exe`
- `afterdroidutils.proj` — mobile `nhdat` + data from `C:\wsl-out\` → Android/iOS
- `aftergnollhackdroid.proj` — Android `.so` → MAUI Android
- `aftergnollhackios.proj` — iOS `.a` from `C:\mac-out\` → MAUI iOS

### Android prerequisites: WSL with SSH, VS Connection Manager (127.0.0.1:22), `C:\wsl-in\` directories.

### iOS prerequisites: WSL + vcremote on Mac + Pair to Mac + PuTTY/PSCP for downloading `libgnollhackios.a`. See wiki pages for setup.

## The `makedefsdroid` Project

`makedefsdroid` does **three unrelated jobs** (`win/win32/vs/makedefsdroid.vcxproj`, a Linux cross-compilation project):

| # | Job | Runs on | Needs WSL? |
|---|-----|---------|-----------|
| 1 | XAML transformation + tileset staging (`InitialBuild` target) | Windows MSBuild | No |
| 2 | Sound bank + asset-pack staging (`PreBuildEvent` → `update-wsl-in-*.bat`) | Windows batch | No |
| 3 | Linux `makedefs` data pipeline (`RemotePostBuildEvent`) | Ubuntu over SSH | **Yes** |

> **Job 1 runs even when job 3 fails.** `InitialTargets="InitialBuild"` executes before the WSL compile. Don't assume a `makedefsdroid` error means XAML wasn't regenerated — check timestamps.

### XAML Transformation

Auto-generates 31 MAUI XAML files (6 controls, 25 pages) from Xamarin source in `GnollHackX/GnollHackX/`. Key conversions: Xamarin→MAUI namespaces, `WidthRequest  =`→`MaximumWidthRequest=`, `HeightRequest  =`→`MaximumHeightRequest=`, `<Frame>`→`<Border>`, content wrapper stripping. See `maui_frontend` skill for the full conversion list, exempt files, and template for adding new XAML files.

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

## Avalonia Port (Linux and Windows)

`GnollHackM.csproj` with `-p:GnhAvalonia=true` targets `net11.0` on Avalonia.Controls.Maui instead of the platform backends. The runtime identifier defaults to the host's, so a build on Windows is the `win-x64` flavor (development and testing only, not shipped; `Platforms/Windows` libs and `gnh`, data in `%LOCALAPPDATA%\GnollHack-Avalonia`), and `-r linux-x64` is the shipped Linux flavor (`LINUX` and `SENTRY` defined). Both take the `Platforms/Windows` tilesets and Desktop banks. Human-facing walkthroughs: wiki pages `Development/Avalonia Port`, `Development/Build Instructions for Avalonia Linux Version on Windows`, `Development/Build Instructions for Avalonia Windows Version on Windows`.

- **Always run `dotnet` from `GnollHackM/Platforms/Avalonia`.** Its `global.json` pins the SDK and workload set (`rollForward: disable`); from the repository root or `GnollHackM` another SDK is chosen. Install the workload there with `dotnet workload install maui`.
- **No `-t:Compile`** for this flavor (MSB4057); verify with a full build, about 30 s.

```powershell
cd win\win32\xpl\GnollHackM\Platforms\Avalonia
dotnet build ..\..\GnollHackM.csproj -p:GnhAvalonia=true                     # Windows flavor → bin\Debug\net11.0\win-x64\GnollHackM.exe
dotnet publish ..\..\GnollHackM.csproj -p:GnhAvalonia=true -r win-x64 -f net11.0 -c Release --self-contained true      # tester folder
dotnet publish ..\..\GnollHackM.csproj -p:GnhAvalonia=true -r linux-x64 -f net11.0 -c Release --self-contained true -p:GnhSentryUpload=false
```

Publish needs `-f net11.0`. A Release publish uploads Sentry symbols when the machine-wide `SENTRY_AUTH_TOKEN` is set; `-p:GnhSentryUpload=false` suppresses it. `Build-LinuxPackages.ps1` publishes **without** that flag, so for a non-release package run the publish above yourself and then the script with `-SkipPublish`.

### Native prerequisites of the Linux flavor

**One WSL distribution**, any Ubuntu LTS from 20.04 to 26.04, on WSL 2 (26.04's `tar` fails on WSL 1, and WSLg testing needs WSL 2; 20.04 also works on WSL 1). It runs sshd for Visual Studio's Connection Manager (`127.0.0.1:22`), the WSL data pipeline, the Linux builds, packaging and testing. Every GnollHack-built Linux binary (engine, `recover`, `libsentry.so`) compiles and links against the **glibc 2.27 sysroot** in `/opt/gnollhack/sysroot-glibc-2.27`, so the host's glibc never reaches the output; install it once per distribution with `Platforms/Linux/Packaging/Install-LinuxSysroot.ps1` (pinned, hash-checked Ubuntu 18.04 packages; it makes the packages' absolute dev symlinks relative, without which `libdl.so` resolves to the host's empty stub). Its link options are `--sysroot` plus `-L<sysroot>/usr/lib/x86_64-linux-gnu -L<sysroot>/lib/x86_64-linux-gnu` (Ubuntu's GCC searches the host's multiarch directory before the sysroot's), `-B<sysroot>/usr/lib/x86_64-linux-gnu` (the sysroot's start files), and `-Wl,-z,defs` (a function glibc 2.27 lacks would otherwise link unversioned and pass every `GLIBC_` floor check). The `Build-Linux*.ps1` and installer scripts default to the default WSL distribution, print it, and refuse `docker-desktop*`; `-WslDistro` overrides.

**C standard:** Visual Studio's Linux toolset already passes `-std=c11` (`CLanguageStandard` default in `Linux.props`); do not add another. GCC 14+ makes implicit declarations errors, so `levcompdroid` and `dgncompdroid` add `-Wno-error=implicit-function-declaration` for the lex files' `fileno`, as the engine does. The Unix hints (`sys/unix/hints/linux*`) set `-std=gnu17`, since GCC 15 defaults to C23, where `E void srand48();` conflicts with glibc.

1. **WSL data pipeline**: build `GnollHack.sln` once with `MauiAndroid` or `Android+Windows` (not `x64`). Linux ships the Android `nhdat` (`Platforms/Android/gnh/*`), and `afterdroidutils.proj` writes `win/win32/xpl/gnollhacklinux/date_unix.h` (gitignored), which `gnollhacklinux` compiles against. The same build stages the Windows tilesets and banks both flavors use.
2. **`libgnollhacklinux.so`**: `win/win32/vs/gnollhacklinux.vcxproj` (Release|x64) is built by **no** solution platform except `Any CPU`; build it explicitly. It remote-compiles over the Connection Manager's SSH connection against the sysroot (`GnhLinuxSysroot`; a `RemotePreBuildEvent` stops with a clear message when the sysroot's stamp file is missing), and `CopyToMauiLinuxLibs` copies it to `Platforms/Linux/libs/`. From the command line use the **64-bit** MSBuild; the 32-bit one that `vswhere -find` returns first fail-fasts with no output on Linux projects:

```powershell
wsl -d <distro> -u root -e /usr/sbin/service ssh start          # plain 'service' is not on root's -e PATH
$vs = & "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe" -latest -property installationPath
cmd /c "`"$vs\MSBuild\Current\Bin\amd64\MSBuild.exe`" win\win32\vs\gnollhacklinux.vcxproj /t:Build /p:Configuration=Release /p:Platform=x64 /v:m > %TEMP%\gnollhacklinux.log 2>&1"
```

`Could not connect to the remote system` means sshd is not running or the Connection Manager entry is missing.

### Linux packages

The Linux build ships as an **AppImage** on GitHub releases and as **Steam depot content**, plus a tester tar.gz, all from one self-contained `linux-x64` publish. One script builds all three:

```powershell
win/win32/xpl/GnollHackM/Platforms/Linux/Packaging/Build-LinuxPackages.ps1                  # all targets
win/win32/xpl/GnollHackM/Platforms/Linux/Packaging/Build-LinuxPackages.ps1 -Targets AppImage -SkipPublish
```

- **Prerequisite:** `Platforms/Linux/libs/libgnollhacklinux.so`, built by `win/win32/vs/gnollhacklinux.vcxproj` (gitignored). The script stops without it.
- **Source of truth:** the version comes from `ApplicationDisplayVersion` and `ApplicationVersion` in `GnollHackM.csproj`. The pinned `appimagetool` and type-2 runtime, with their SHA-256 values, are constants at the top of the script, and must be present in WSL under `~/.cache/gnollhack-packaging/tools/`. The script never downloads.
- **Where it runs:** the publish runs on Windows, from `Platforms/Avalonia`, whose `global.json` pins the SDK. Staging and packaging run in WSL (`-WslDistro`, default the default WSL distribution), in the WSL home, so Linux permissions survive. That distribution needs `rsync` and `binutils` (`strip`, `readelf`, `objdump`). Outputs land in `GnollHackM/bin/LinuxPackages/<version>/`, which is gitignored; the staged tree stays at `~/.cache/gnollhack-packaging/stage-<version>/`. A full run takes a few minutes and ends with a size and SHA-256 summary.
- **Pinned AppImage tools:** the file names in `$AppImageToolFile` and `$AppImageRuntimeFile` are the GitHub release assets `appimagetool-x86_64.AppImage` (AppImage/appimagetool) and `runtime-x86_64` (AppImage/type2-runtime) of the versions named in the comment above them, saved under the pinned names. Downloading them is the user's call, never the script's.
- **Testing in WSL:** run the staged `stage-<version>/gnollhack/GnollHackM`, or the AppImage with `--appimage-extract-and-run`, in a WSL 2 distribution with `GALLIUM_DRIVER=d3d12` (WSLg GPU). Never run from `/mnt/c`. Game data goes to `~/.local/share/GnollHack`.
- **Steam:** the content targets the Steam Linux Runtime 4.0 (steamrt4) by default, with the two libraries `libcef.so` needs from `Platforms/Linux/libs-steamrt4/` added. `-SteamRuntime sniper` builds for SLR 3.0 without them. The `.vdf` templates in `Packaging/steam/` are filled from `-SteamAppId`, `-SteamDepotId`, `-SteamBranch` and `-BuildDescription`. Upload with `steamcmd` from the WSL copy the summary names, so that the executable bits survive.
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
| `GnollHackM/Platforms/iOS/libs/libgnollhackios.a` | `aftergnollhackios.proj` |
| `GnollHackM/Platforms/{Windows,Android,iOS}/gnh/nhdat` | `afterdlb.proj` / `afterdroidutils.proj` |
| `GnollHackM/Platforms/Windows/gnh/recover.exe` | `recoverwin.vcxproj` (from `gnollhackwin.vcxproj`) |
| `GnollHackM/Platforms/Linux/gnh/recover` (committed) | `Build-LinuxRecover.ps1` (manual) |
| `GnollHackM/Platforms/Linux/libs/libgnollhacklinux.so` | `gnollhacklinux.vcxproj` (manual, built by no platform) |
| `win/win32/xpl/gnollhacklinux/date_unix.h` | `afterdroidutils.proj` (WSL pipeline) |
| `GnollHackM/Platforms/*/tileset/*.ghpng` (3 each) | `copytilesetdroid.proj` |
| `GnollHackM/Platforms/*/banks/*.bank` (6 each) | `update-wsl-in-*.bat` |
| 31 generated `.xaml` in `GnollHackM/` | `makedefsdroid` `InitialBuild` |

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
| `update-wsl-in-*.bat` fails on fresh machine | `C:\wsl-in\` directories don't exist. |
| Link/runtime errors after switching Debug↔Release | Rebuild both solutions in the same config. |
| Data file works on Windows but missing on mobile | Added to `<DlbList>` only — see [Two Data Pipelines](#two-data-pipelines). |
| Avalonia build picks the wrong SDK or fails to restore | `dotnet` not run from `Platforms/Avalonia`, or its pinned SDK/workload missing. |
| `Build-LinuxPackages.ps1`: `Missing ...libgnollhacklinux.so` | Build `gnollhacklinux.vcxproj` — see [Native prerequisites](#native-prerequisites-of-the-linux-flavor). |
| `... needs GLIBC_2.xx, above the ... floor` | A Linux binary was built without the sysroot; run `Install-LinuxSysroot.ps1` in the distribution that built it, then rebuild. |
| `error: the glibc 2.27 sysroot is missing` | Run `Install-LinuxSysroot.ps1` in the distribution the Connection Manager points to. |
| `undefined reference to '...'` linking a Linux binary | `-z defs` caught a function glibc 2.27 lacks (or keeps in `libdl`/`librt`); avoid it or link its library. |

More troubleshooting on the GnollHack Wiki page *Troubleshooting Building GnollHack with .NET MAUI*.

## Related Skills

- **`game_data_files`** — `.des` file syntax, dungeon definitions, text databases
- **`adding_game_content`** — Adding monsters, objects, artifacts (includes `makedefs` flags)
- **`maui_frontend`** — MAUI frontend, XAML pipeline details, SkiaSharp rendering
- **`debugging_gnollhack`** — Wizard mode, panic logs, running the MAUI app locally
- **`client_subagent_guidelines`** — Build dependency chains constraining parallel agent work
