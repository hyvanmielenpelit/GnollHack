# Build Tooling Workarounds

- Component: MSBuild, the .NET SDK, Visual Studio, NuGet, C compilers and linkers
- Upstream: https://github.com/dotnet/msbuild, https://github.com/dotnet/sdk, https://developercommunity.visualstudio.com
- ID prefix: WA-BUILD

Entries follow the schema in [README.md](README.md). Append new entries at the end of the
file and never renumber or reuse an ID.

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
