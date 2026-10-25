# Build Tooling Workarounds

- Component: MSBuild, the .NET SDK, Visual Studio, NuGet, C compilers and linkers
- Upstream: https://github.com/dotnet/msbuild, https://github.com/dotnet/sdk, https://developercommunity.visualstudio.com
- ID prefix: WA-BUILD

Entries follow the schema in [README.md](README.md). Append workarounds before the
`<!-- known-issues -->` line if there is one, otherwise at the end of the file, and known
issues at the end of the file. Never renumber or reuse an ID.

<!-- entries -->

## WA-BUILD-0001: Xcode project generator hardcodes IPHONEOS_DEPLOYMENT_TARGET 8.1

- Status: active
- Added: 2026-09-26 (from git history)
- GnollHack version: 3.21 (421)
- Component: Visual Studio
- Package: Visual Studio
- Defective version: unknown (Visual Studio C++ iOS project system, platform toolset XCode_6_1)
- Platforms: iOS
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/vs/gnollhackios.vcxproj` (`GHSetXcodeDeploymentTarget`)
- Remove when: Visual Studio's Xcode project generator writes the deployment target from `AdditionalBuildSettings` into the generated project, and Verify removal passes.

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