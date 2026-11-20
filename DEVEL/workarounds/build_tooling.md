# Build Tooling Workarounds

- Component: MSBuild, the .NET SDK, Visual Studio, NuGet, C compilers and linkers
- Upstream: https://github.com/dotnet/msbuild, https://github.com/dotnet/sdk, https://developercommunity.visualstudio.com
- ID prefix: WA-BUILD

Entries follow the schema in [README.md](README.md). Append workarounds before the
`<!-- known-issues -->` line if there is one, otherwise at the end of the file, and known
issues at the end of the file. Never renumber or reuse an ID.

<!-- entries -->

## WA-BUILD-0001: Xcode project generator hardcodes IPHONEOS_DEPLOYMENT_TARGET 8.1

- Status: removed
- Added: 2026-09-26 (from git history)
- GnollHack version: 3.21 (421)
- Component: Visual Studio
- Package: Visual Studio
- Defective version: unknown (Visual Studio C++ iOS project system, platform toolset XCode_6_1)
- Platforms: iOS
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-11-04
- Code: `win/win32/vs/gnollhackios.vcxproj` (`GHSetXcodeDeploymentTarget`)
- Remove when: Visual Studio's Xcode project generator writes the deployment target from `AdditionalBuildSettings` into the generated project, and Verify removal passes.
- Removed: 2026-11-04, 3.37 (437)

**Symptom.** The Xcode project that Visual Studio generates for `gnollhackios` sets
`IPHONEOS_DEPLOYMENT_TARGET` to 8.1, so the native library is built for iOS 8.1 rather
than the deployment target the project asks for, and Xcode warns about it.

**Cause.** The generator writes `IPHONEOS_DEPLOYMENT_TARGET = 8.1` into the target-level
build settings, which override the value given in `AdditionalBuildSettings`.

**Workaround.** The `GHSetXcodeDeploymentTarget` target runs after `GenerateXCodeProj`. It
rewrites every `IPHONEOS_DEPLOYMENT_TARGET` value in `project.pbxproj` to
`GHiOSDeploymentTarget`, before the project is copied to the Mac, and prints the values the
project then holds.

**Verify removal.** Disable `GHSetXcodeDeploymentTarget`. Then:
- Build `gnollhackios.vcxproj` for Release|ARM64 with the Mac connected.
- Open the generated `project.pbxproj` and confirm that every `IPHONEOS_DEPLOYMENT_TARGET`
  equals the value in `AdditionalBuildSettings`.
- Confirm that the Xcode build log has no deployment target warnings.

Checks:
- 2026-10-23 | Visual Studio 2026 18.11.1 | unverified | Not re-tested; no upstream report.
- 2026-11-04 | Visual Studio 2026 18.11.3 | removed | gnollhackios no longer uses the Visual Studio C++ iOS project type: engine.mk passes the deployment target to clang (-target arm64-apple-ios15.0). Verify removal was not run, because the generated Xcode project it inspects no longer exists.

## WA-BUILD-0002: Visual Studio's Android Archive sets no archive or publish property

- Status: active
- Added: 2026-10-11 (from git history)
- GnollHack version: 3.26 (426)
- Component: Visual Studio
- Package: Visual Studio
- Defective version: unknown (Visual Studio Android Archive)
- Platforms: Android
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/GnollHackM.csproj` (`EnableSentryUploadForPublish`)
- Remove when: Visual Studio's Android Archive passes a property that identifies the build as an archive or publish, and Verify removal passes.

**Symptom.** An MSBuild target cannot tell a Visual Studio Android Archive build from an
ordinary Release build, so the Sentry symbol upload, which runs only for publish and
archive builds, would never run for an Android archive made in Visual Studio.

**Cause.** Visual Studio's Android Archive runs a Release build without any of the
properties that other publish paths set, such as `_IsPublishing` or `ArchiveOnBuild`.

**Workaround.** The `EnableSentryUploadForPublish` target treats every Android Release
build as a publish, except the builds Visual Studio triggers implicitly for F5 and deploy
(`IsImplicitlyTriggeredBuild` is `true`).

**Verify removal.** In a Visual Studio version that sets an archive property, replace the
Android clause of the condition with that property. Then:
- Archive the Android Release build from Visual Studio with `SENTRY_AUTH_TOKEN` set, and
  confirm that the symbol upload runs.
- Deploy the Release build with F5, and confirm that no upload runs.

Checks:
- 2026-10-23 | Visual Studio 2026 18.11.1 | unverified | Not re-tested; no upstream report.

## WA-BUILD-0003: Linux toolset asks Windows project dependencies for GetRecipeFile

- Status: active
- Added: 2026-10-25
- GnollHack version: 3.31 (431)
- Component: Visual Studio
- Package: Visual Studio
- Defective version: Visual Studio 2026 18.11.1 (Linux and embedded development with C++, application type Linux 1.0)
- Platforms: Android, iOS, Linux (Avalonia)
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-25
- Code: `win/win32/vs/makedefsdroid.vcxproj` (`AddSyntheticProjectReferencesForSolutionDependencies`); `win/win32/vs/levcompdroid.vcxproj` (`AddSyntheticProjectReferencesForSolutionDependencies`); `win/win32/vs/dgncompdroid.vcxproj` (`AddSyntheticProjectReferencesForSolutionDependencies`); `win/win32/vs/dlbdroid.vcxproj` (`AddSyntheticProjectReferencesForSolutionDependencies`); `win/win32/vs/gnollhacklinux.vcxproj` (`AddSyntheticProjectReferencesForSolutionDependencies`)
- Remove when: the Linux toolset asks only Linux project references for recipe files, and Verify removal passes.

**Symptom.** A command-line build of `GnollHack.sln` with a platform that includes the
WSL data tools (`Linux`, `MauiAndroid`, `MauiAll`) stops in `makedefsdroid` with
`makedefs.vcxproj : error MSB4057: The target "GetRecipeFile" does not exist in the
project.` The same build from the Visual Studio IDE succeeds.

**Cause.** Outside Visual Studio, `AssignProjectConfiguration` in
`Microsoft.Common.CurrentVersion.targets` turns each solution dependency into a synthetic
project reference. The Linux toolset's `_AssignProjectReferencesPlatformType`
(`Application Type\Linux\1.0\Linux.targets`) marks every project reference as a Linux
project, and `CreateRecipeFile` (`Linux.Common.targets`) then calls `GetRecipeFile` on all
of them, including the Windows `makedefs`, `levcomp` and `dgncomp`, which do not define it.
Visual Studio sets `BuildingInsideVisualStudio`, so no synthetic references are made there.

**Workaround.** The five Linux projects set
`AddSyntheticProjectReferencesForSolutionDependencies` to `false`, as a Visual Studio build
effectively does. The solution still builds each project's dependencies first.

**Verify removal.** Remove the property group from the five projects. Then:
- Start the WSL sshd, and build
  `msbuild win/win32/vs/GnollHack.sln /t:Rebuild /p:Configuration=Debug /p:Platform=Linux`
  with the 64-bit MSBuild.
- Confirm that it succeeds, with no `MSB4057` for `GetRecipeFile`.

Checks:
- 2026-10-25 | Visual Studio 2026 18.11.1 | still-needed | Reproduced with the `Linux` platform; with the property both configurations build the eight projects.

## WA-BUILD-0004: Xcode's GNU make 3.81 has no --output-sync

- Status: active
- Added: 2026-11-05
- GnollHack version: 3.37 (437)
- Component: Xcode
- Package: Xcode
- Defective version: GNU Make 3.81 (Xcode 27.0)
- Platforms: iOS, macOS
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-11-05
- Code: `win/win32/xpl/libapple/engine.mk` (`$(OBJDIR)/%.o`)
- Remove when: the make that Xcode's command-line tools ship supports `--output-sync` (GNU make 4.0 or later), and Verify removal passes.

**Symptom.** With `make -j`, the diagnostics of parallel clang processes interleave character by character in the
build output, and Visual Studio's Error List gets garbled entries.

**Cause.** GNU make added `--output-sync` (`-O`) in 4.0. Apple ships 3.81, the last GPLv2 release, and has not
updated it.

**Workaround.** Each compile in `engine.mk` writes its stderr to `<object>.log`, then prints the file under
`lockf`, so each source's diagnostics appear as one block.

**Verify removal.** On the Mac, check `make --version`. If it is 4.0 or later, replace the object rule's recipe with a
plain `$(CC) $(CFLAGS) -c $< -o $@`, add `-O` to the make command in `Build-AppleEngine.ps1`, build `gnollhackios`
with a source that produces warnings in several files, and confirm that each file's warnings are contiguous.

Checks:
- 2026-11-05 | Xcode 27.0 | still-needed | `make --version` prints GNU Make 3.81; a cold `-j8` iOS build interleaved 264 warnings.

## WA-BUILD-0005: Xcode's GNU make 3.81 compares file times in whole seconds

- Status: active
- Added: 2026-11-05
- GnollHack version: 3.37 (437)
- Component: Xcode
- Package: Xcode
- Defective version: GNU Make 3.81 (Xcode 27.0)
- Platforms: iOS, macOS
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-11-05
- Code: `win/win32/xpl/libapple/sync-tree.sh` (`sleep 1`)
- Remove when: the make that Xcode's command-line tools ship compares sub-second file times, and Verify removal passes.
- Related: WA-BUILD-0004

**Symptom.** A source rewritten within the same second as the object built from its previous content is reported
`up to date`, and the stale object is linked.

**Cause.** Apple's GNU make 3.81 truncates file times to whole seconds, although APFS stores nanoseconds.

**Workaround.** `sync-tree.sh` waits one second before copying changed files into the build tree, so every copied
file is at least a second newer than any object an earlier build compiled.

**Verify removal.** On the Mac, in an empty folder, write a makefile with the rule `x.o: x.c` whose recipe copies
`x.c` to `x.o`, run make, rewrite `x.c` 0.3 seconds later and run make again. If the second run rebuilds `x.o`, remove
the pause from `sync-tree.sh`.

Checks:
- 2026-11-05 | Xcode 27.0 | still-needed | An edit 0.3 s after a build was reported `up to date`; after 1.1 s it rebuilt.

## WA-BUILD-0006: codesign looks up a --keychain identity's private key along the keychain search list

- Status: active
- Added: 2026-11-05
- GnollHack version: 3.37 (437)
- Component: macOS code signing
- Package: macOS
- Defective version: macOS 26.6 (codesign)
- Platforms: macOS
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-11-05
- Code: `win/win32/xpl/GnollHackM/Platforms/MacOS/Packaging/package-mac.sh` (`restore_keychains`)
- Remove when: `codesign --keychain` signs over SSH while a locked login keychain holds the same identity, and Verify removal passes.

**Symptom.** Over SSH, `codesign --keychain <signing keychain>` fails with `errSecInternalComponent` although that
keychain is unlocked and its key's partition list allows `codesign:`.

**Cause.** `codesign` takes the certificate from `--keychain` but looks up the private key along the user's keychain
search list, and stops at the copy in the login keychain, which is locked in an SSH session. macOS 26 has no
per-session (`-d dynamic`) search list.

**Workaround.** `package-mac.sh` puts the signing keychain first in the user search list for the run and restores the
original order on exit.

**Verify removal.** Remove the reordering, keep the identity in both keychains, and run `Build-MacPackage.ps1` with the
Developer ID identity. If signing succeeds, remove the workaround.

Checks:
- 2026-11-05 | macOS 26.6 | still-needed | Signing failed with errSecInternalComponent until the signing keychain was first in the search list.

## WA-BUILD-0007: Visual Studio deploys a stale Android build

- Status: active
- Added: 2026-11-11
- GnollHack version: 3.40 (440)
- Component: Visual Studio
- Package: Visual Studio
- Defective version: Visual Studio 2026 18.12.0
- Platforms: Android
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-11-11
- Code: `win/win32/xpl/GnollHackM/GnollHackM.csproj` (`DisableFastUpToDateCheck`)
- Remove when: Visual Studio's up-to-date check notices edits to GnollHackM's linked sources on Android, and Verify removal passes.

**Symptom.** F5 on the Android target deploys the previous APK: an edited source file is not
compiled. On 2026-11-11 `VersionPage.xaml.cs` was saved at 17:12:46, and the following F5 wrote
no new `GnollHackM.dll` or APK; only the signing step's `android_debug_keystore.flag` changed,
and the phone ran the build of 16:49.

**Cause.** Not proven. Visual Studio's fast up-to-date check judged the project up to date and
skipped MSBuild, so only the deploy ran. Its input list comes from design-time builds, which
had been disturbed that day by a package update, NuGet restores and a cleaned `obj`; the edited
file is linked from `GnollHackX`, outside the project folder.

**Workaround.** The Android target framework sets `DisableFastUpToDateCheck`, so Visual Studio
always runs MSBuild, whose own incremental build detects the change. It costs a few seconds per
F5 when nothing changed.

**Verify removal.** Remove the property. In Visual Studio set Tools > Options > Projects and
Solutions > SDK-Style Projects > Up to Date Checks > Logging Level to Verbose. Build and deploy
the Android target, edit a file under `win/win32/xpl/GnollHackX/GnollHackX`, and press F5 again,
several times over a day of normal work. The Build output must never report the project
up to date after such an edit, and the app must show the change.

Checks:
- 2026-11-11 | Visual Studio 2026 18.12.0 | still-needed | Seen once by the user; the cause was inferred from file timestamps, and the check's own log was not captured. One search of dotnet/project-system found no matching issue.

## WA-BUILD-0008: Visual Studio restore needs one PackageVersion across target frameworks

- Status: active
- Added: 2026-11-17
- GnollHack version: 3.42 (442)
- Component: NuGet in Visual Studio
- Package: Visual Studio, Microsoft.Maui.Controls
- Defective version: Visual Studio 2026 18.12.0, Microsoft.Maui.Controls 11.0.10
- Platforms: Android, iOS, Windows (MAUI)
- Upstream issue: https://github.com/NuGet/Home/issues/6461
- Upstream status when added: reported
- Upstream status: reported
- Last checked: 2026-11-17
- Code: `win/win32/xpl/GnollHackM/GnollHackM.csproj` (`Version`)
- Remove when: Visual Studio's restore accepts a `PackageVersion` that differs between target frameworks, or MAUI sets `Version` on Windows before any restore, and Verify removal passes.

**Symptom.** After an Avalonia restore of `GnollHackM.csproj` (`-p:GnhAvalonia=true`), or with
no restore outputs in `obj`, Visual Studio's restore fails with `NU1105: Unable to read project
information for 'GnollHackM': The property PackageVersion was expected to have a single value
across all target frameworks, but instead had the following values: 1.0.0, 3.42`. It writes a
`project.assets.json` with no targets, and every build then stops with `NETSDK1005`. Visual
Studio does not recover by itself; a command-line `dotnet restore` does.

**Cause.** Visual Studio's restore (`VSNominationUtilities.GetPackageVersion` in NuGet.Client)
requires `PackageVersion`, which defaults to `Version`, to have one value across the project's
target frameworks. Command-line restore has no such check. The Android and iOS workloads set
`Version` to `ApplicationDisplayVersion` in their SDK targets. For Windows, MAUI does the same
in `buildTransitive\WinUI.targets` of the Microsoft.Maui.Core package, which is imported only
through the restore output `obj\GnollHackM.csproj.nuget.g.targets`. When that file is missing,
or was written by a restore of the Avalonia flavor, which shares `obj`, Windows keeps the SDK
default `1.0.0`. The failed restore then writes an empty `nuget.g.targets`, which keeps Windows
at `1.0.0`. dotnet/maui#6626 describes the same failure and recommends this workaround.

**Workaround.** The base property group sets `Version` to `ApplicationDisplayVersion`, the value
the three platforms' targets set, so it is the same on every target framework in every restore
state. The Avalonia property group sets its own `Version` after it.

**Verify removal.** Remove the tagged line. Then:
- Close Visual Studio, and in `win/win32/xpl/GnollHackM` run
  `dotnet restore GnollHackM.csproj -p:GnhAvalonia=true -r linux-x64`.
- Open the solution in Visual Studio and build GnollHackM for Windows, without running a
  command-line restore.
- Confirm that the build succeeds, and that `obj\project.assets.json` lists the three MAUI
  target frameworks and has no `NU1105` in its `logs`.

Checks:
- 2026-11-17 | Visual Studio 2026 18.12.0, Microsoft.Maui.Controls 11.0.10 | still-needed | Reproduced by evaluation: on the outputs of an Avalonia restore, Windows reports PackageVersion 1.0.0, and Android and iOS 3.42; with the property all three report 3.42. NuGet/Home#6461 is open, and NuGet/Home#13304 was closed as not planned.

## WA-BUILD-0009: rsync 3.1.3 hangs sending to a newer rsync with --compress

- Status: active
- Added: 2026-11-20
- GnollHack version: 3.43 (443)
- Component: rsync
- Package: rsync
- Defective version: rsync 3.1.3 (Ubuntu 20.04, sender) to rsync 3.4.4 (SteamOS 3.9.5, receiver)
- Platforms: Linux (Avalonia)
- Upstream issue: https://github.com/RsyncProject/rsync/issues/433
- Upstream status when added: reported
- Upstream status: reported
- Last checked: 2026-11-20
- Code: `win/win32/xpl/GnollHackM/Platforms/Linux/Packaging/Deploy-SteamDeck.ps1` (`rsync`)
- Remove when: the WSL distribution that deploys ships an rsync that no longer hangs with `--compress` against the Deck's, and Verify removal passes. Compression gains nothing on a local network, so keeping it off is also fine.

**Symptom.** `Deploy-SteamDeck.ps1` stops at `Copying ... with rsync` and never finishes. On
2026-11-20 a deploy of a new version ran for 11 minutes without writing a file on the Deck: the
Deck had received about 4 MB, both rsync processes on each end were idle in `poll`, and the TCP
connection had no queued data. A deploy of an unchanged version, which sends only a few
megabytes, finishes, so the hang shows only when a new version is deployed.

**Cause.** Not proven beyond the reproduction. With `--compress`, rsync 3.1.3 sending to rsync
3.4.4 stops after the first few megabytes of file data; without it the same transfer finishes.
The network and ssh are not involved: 200 MB piped through the same ssh from WSL arrived in under
9 seconds, and `-o IPQoS=throughput` did not help. RsyncProject/rsync#433 reports 3.1.3 and 3.2.7
getting stuck against each other with `--compress`.

**Workaround.** The deploy's rsync runs without `--compress`. On a local network compression
only costs CPU: the uncompressed sync of a new version took about 3.5 seconds.

**Verify removal.** Add `--compress` back to the rsync command. Then:
- Build a new version with `Build-LinuxPackages.ps1 -Targets TarGz -NoSentryUpload`, so that
  most files differ from what the Deck has.
- Run `Deploy-SteamDeck.ps1` from the WSL distribution the packaging used.
- Confirm that it finishes within a minute, and that the Deck's `~/gnollhack-dev/gnollhack/`
  matches the staged tree.

Checks:
- 2026-11-20 | rsync 3.1.3, rsync 3.4.4 | still-needed | Reproduced twice with `--compress`, and once more with `-o IPQoS=throughput` added; without `--compress` the same deploy finished in seconds. RsyncProject/rsync#433 is open.