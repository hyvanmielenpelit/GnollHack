# Linux Workarounds

- Component: Linux distributions, glibc, FreeType, fontconfig, X11, AppImage, Steam runtime
- Upstream: Per library; see each entry
- ID prefix: WA-LINUX

Entries follow the schema in [README.md](README.md). Append workarounds before the
`<!-- known-issues -->` line if there is one, otherwise at the end of the file, and known
issues at the end of the file. Never renumber or reuse an ID.

<!-- entries -->

## WA-LINUX-0001: fontconfig recaches fonts from a moving AppImage mount

- Status: active
- Added: 2026-10-20 (from git history)
- GnollHack version: 3.28 (428)
- Component: fontconfig, AppImage
- Package: fontconfig, AppImage
- Defective version: unknown (the system's fontconfig and the pinned AppImage runtime)
- Platforms: Linux
- Upstream issue: none
- Upstream status when added: not-needed
- Upstream status: not-needed
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/Platforms/Avalonia/GHAvaloniaFontConfig.cs` (`GHAvaloniaFontConfig.CopyToDataDirectory`)
- Remove when: GnollHack no longer ships its own fonts for fontconfig in the AppImage, or fontconfig stops keying its cache by folder path.

**Symptom.** In the AppImage, fontconfig would treat the bundled font folder as a new
folder on every launch and cache it again.

**Cause.** An AppImage is mounted at a new path on every launch, and fontconfig keys its
cache by folder path. Both are by design.

**Workaround.** When started from an AppImage, the bundled fonts (the emoji font for web
pages) are copied to the data directory, when missing or changed, and fontconfig is
pointed at the copy.

**Verify removal.** Make `Install` use the bundled folder for AppImages too. Build the
AppImage, start it twice, and check whether the `fontconfig/cache` folder in the data
directory gains new cache files on each start.

Checks:
- 2026-10-23 | fontconfig (system), AppImage | still-needed | Inherent: the mount path and the cache keying are both by design.

## WA-LINUX-0002: Steam Linux Runtime 4.0 lacks libraries libcef.so needs

- Status: active
- Added: 2026-10-20 (from git history)
- GnollHack version: 3.28 (428)
- Component: Steam runtime
- Package: Steam Linux Runtime 4.0 (steamrt4)
- Defective version: unknown (Steam Linux Runtime 4.0, build not recorded)
- Platforms: Linux
- Upstream issue: https://github.com/ValveSoftware/steam-runtime/issues/861
- Upstream status when added: unreported
- Upstream status: reported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/Platforms/Linux/Packaging/Build-LinuxPackages.ps1` (`$SteamRt4LibsDir`)
- Remove when: Steam Linux Runtime 4.0 ships libatk-bridge-2.0.so.0 and libatspi.so.0, and Verify removal passes.

**Symptom.** Under steamrt4, libcef.so fails to load, so web views cannot use CEF.

**Cause.** steamrt4 does not contain libatk-bridge-2.0.so.0 and libatspi.so.0, which
libcef.so links against. The linked issue reports the same missing library for another
CEF game.

**Workaround.** For `-SteamRuntime steamrt4`, the packaging script adds every file of
`Platforms/Linux/libs-steamrt4/` (the two libraries and their at-spi2-core copyright file)
to the Steam content root.

**Verify removal.** Build the Steam content without the `libs-steamrt4` files, run it
under steamrt4, and open a web view; CEF must initialize.

Checks:
- 2026-10-23 | Steam Linux Runtime 4.0 (steamrt4) | still-needed | Issue open; opened after the workaround landed.

## WA-LINUX-0003: Chromium's sandbox cannot run in Steam's container

- Status: active
- Added: 2026-10-20 (from git history)
- GnollHack version: 3.28 (428)
- Component: Steam runtime
- Package: Steam Linux Runtime 4.0 (steamrt4), cef.runtime.linux-x64
- Defective version: unknown (Steam Linux Runtime pressure-vessel container, build not recorded)
- Platforms: Linux
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/Platforms/Avalonia/Cef/GHCefApp.cs` (`GHCefApp.OnBeforeCommandLineProcessing`); `win/win32/xpl/GnollHackM/Platforms/Avalonia/Cef/GHCefRuntime.cs` (`GHCefRuntime.LoadAndInitialize`)
- Remove when: Chromium's sandbox and zygote work inside the Steam Linux Runtime container, and Verify removal passes under Steam and from the AppImage.

**Symptom.** With Chromium's sandbox and zygote on, CEF does not work inside Steam's
container.

**Cause.** Steam's Linux runtime container does not provide what Chromium's sandbox needs.

**Workaround.** CEF runs with `NoSandbox` and the `no-zygote` switch, and
`chrome-sandbox` is not shipped.

**Verify removal.** Remove `no-zygote` and set `NoSandbox` to false (shipping
`chrome-sandbox` if the setuid sandbox is to be used). Build the Steam content, run it
under the Steam Linux Runtime, and open a web view; CEF must initialize and render.

Checks:
- 2026-10-23 | Steam Linux Runtime 4.0 (steamrt4), cef.runtime.linux-x64 154.0.34 | unverified | No upstream issue found.
