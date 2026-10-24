# gnhsentry.dll and libsentry.so

GnollHack's Windows and Linux builds capture native crashes with sentry-native's inproc backend, from two
binaries committed to the repository:

| Binary | Built by | Contents |
|--------|----------|----------|
| `GnollHackM\Platforms\Windows\libs\gnhsentry.dll` | `Build-GnhSentry.ps1` in this folder | `sentry-native.lib` from the Sentry NuGet package, linked statically, plus GnollHack's crash shim `gnhsentry.c` |
| `GnollHackM\Platforms\Linux\libs\libsentry.so` | `GnollHackM\Platforms\Linux\Packaging\Build-LinuxSentryNative.ps1` | sentry-native built in WSL as a shared library from a pinned checkout |

## Why they exist

- **WA-SENTRY-0003.** Sentry .NET starts its own sentry-native only in trimmed or Native AOT builds, and GnollHack
  ships JIT builds on Windows and Linux. `GHSentryNative` therefore initializes sentry-native itself, and
  `SentryNative` is `false` in `GnollHackM.csproj` so that Sentry .NET never starts a second one.
- On Windows the Sentry package carries sentry-native as a static library, `sentry-native\win-x64\sentry-native.lib`,
  which P/Invoke cannot load. gnhsentry.dll links it into a DLL that exports the functions in `gnhsentry.def`, and
  adds two pieces in front of it:
  - **WA-SENTRY-0001.** An unhandled-exception filter that chains to the filter current before `sentry_init`, so
    that an unhandled managed exception still reaches CoreCLR.
  - **WA-SENTRY-0002.** A vectored exception handler that captures faults in GnollHack's native modules under
    managed frames, which never reach inproc's own filter.

The register entries in `DEVEL\workarounds\sentry.md` have the details.

Both binaries are committed because building them needs tools that an ordinary GnollHackM build does not: MSVC and
the restored Sentry package for gnhsentry.dll, and a WSL distribution with the build tools and the pinned
sentry-native checkout for libsentry.so. They change only when Sentry is upgraded or the shim changes, and a manifest
next to each records what it was built from.

## include\sentry.h

`include\sentry.h` is sentry-native's own public header, unchanged except for CRLF line endings. It is MIT-licensed;
its license is the one already shipped in `GnollHackM\Platforms\Windows\licenses\sentry-native\LICENSE`.
`gnhsentry.c` includes it with `SENTRY_BUILD_STATIC` defined. Never edit it by hand: replace it with
`Compare-SentryNativeApi.ps1 -Update`, as in the runbook below.

## Sources of truth

- **`GnhSentryVersion`** in `GnollHackM\GnollHackM.csproj`: the version of the Sentry and Sentry.Maui packages. Both
  `PackageReference`s use `Version="$(GnhSentryVersion)"`; `Build-GnhSentry.ps1` refuses any other form.
- **`include\sentry.h`**: the sentry-native API that `gnhsentry.c`, `GHSentryNative.cs` and
  `GHAvaloniaSentryNative.cs` were verified against, and, through its `SENTRY_SDK_VERSION_MAJOR`, `_MINOR` and
  `_PATCH` macros, the sentry-native version both scripts build. `Build-GnhSentry.ps1` stops before compiling when
  the package's `sentry-native.lib` is another version. `Build-LinuxSentryNative.ps1` requires the tag of that
  version to resolve to its pinned `$SentryNativeCommit`, and the checkout's `include/sentry.h` to equal this one,
  ignoring line endings.

## The manifests and the build-time guard

Each script writes a manifest next to its binary, only after the binary has passed the script's checks. Both are
generated; never edit them.

- **`GnollHackM\Platforms\Windows\libs\gnhsentry.build.props`**: the package version and the sentry-native version
  the DLL was built from; the SHA-256 of the linked `sentry-native.lib`, of the DLL, of its PDB and of the compiler
  and linker arguments; the normalized hash of `gnhsentry.c`, `gnhsentry.def` and `include\sentry.h`; the PDB's
  Sentry debug ID; and the MSVC and Windows SDK versions.
- **`GnollHackM\Platforms\Linux\libs\libsentry.build.props`**: the sentry-native version, the pinned commit, the
  normalized hash of `include\sentry.h`, and the SHA-256 and GNU build ID of libsentry.so.

The normalized hash reads each file as UTF-8 and turns CRLF into LF before hashing, so it does not depend on the
checkout's line endings.

`GnollHackM\GnhSentryNative.targets` imports both manifests and, in the `GnhCheckSentryNativeBinaries` target before
every Windows and Linux build of GnollHackM, compares them with the current inputs. It does not run for Android or
iOS.

| Code | Problem |
|------|---------|
| `GNHSN001` | `GnhSentryVersion` is not the package version gnhsentry.dll was built from |
| `GNHSN002` | The restored `sentry-native.lib` is not the one gnhsentry.dll linked |
| `GNHSN003` | `gnhsentry.dll` is not the DLL the script built |
| `GNHSN004` | `gnhsentry.c`, `gnhsentry.def` or `include\sentry.h` changed since the DLL was built |
| `GNHSN005` | The Windows manifest or the restored `sentry-native.lib` is missing |
| `GNHSN006` | A Sentry `PackageReference`, or the restored Sentry package folder, is not at `GnhSentryVersion` |
| `GNHSN011` | `GnhSentryVersion` is not the package version in the Windows manifest, which the Linux build follows |
| `GNHSN012` | libsentry.so's sentry-native version is not gnhsentry.dll's |
| `GNHSN013` | `libsentry.so` is not the library the script built |
| `GNHSN014` | `include\sentry.h` changed since libsentry.so was built |
| `GNHSN015` | The Linux or the Windows manifest is missing |
| `GNHSN016` | As `GNHSN006`, for the Linux build |

The problems are warnings, except in Release builds that publish, archive, package or upload symbols
(`_IsPublishing`, `ArchiveOnBuild`, `PublishAppxPackage`, `GenerateAppxPackageOnBuild` or `SentryUploadSymbols`),
where they are errors. Two switches override that:

- `GnhSentryNativeStrict=true` makes them errors in any build.
- `GnhAllowStaleSentryNative=true` makes them warnings in any build, and every message then says so.

`UploadGnhSentryPdbToSentry` uploads `win\win32\vs\symbols\Release\x64\gnhsentry\gnhsentry.pdb` with the other
Windows symbols, and only when its hash equals the manifest's, so the symbols come from the machine that built the
committed DLL.

## Skipping a rebuild on purpose

A `-p:GnhAllowStaleSentryNative=true` on the command line is for a local build. A release that ships binaries the
guard reports as stale is recorded, not passed on a command line: commit
`<GnhAllowStaleSentryNative>true</GnhAllowStaleSentryNative>` in a `GnollHackM.csproj` property group with a dated
comment giving the reason, and remove it at the next rebuild.

The expected case is a Linux-only release from a machine without MSVC after a Sentry upgrade: `GNHSN011` blocks the
Linux publish until gnhsentry.dll is rebuilt, by design.

## Upgrade runbook

1. Read the Sentry .NET release notes (Breaking Changes, "update Native SDK" lines) and the sentry-native changelog
   for every version in between; look for inproc, Windows, and the used symbols.
2. Change `GnhSentryVersion`; restore. The build reports `GNHSN001`/`GNHSN002`.
3. Run `Build-GnhSentry.ps1`; it stops and names the lib's sentry-native version `<X>`.
4. Find the commit of tag `<X>` (for example
   `git ls-remote https://github.com/getsentry/sentry-native refs/tags/<X>`), set `$SentryNativeCommit` in
   `Build-LinuxSentryNative.ps1`, and re-clone the WSL checkout at `<X>` from PowerShell (the clone command the
   Linux script prints uses the vendored header's version, which is still the old one at this point):

   ```powershell
   wsl.exe -d Ubuntu-20.04 -e bash -c 'rm -rf "$HOME/.cache/gnollhack-sentry-native/sentry-native" && git clone --depth 1 --branch <X> https://github.com/getsentry/sentry-native.git "$HOME/.cache/gnollhack-sentry-native/sentry-native"'
   ```

5. Compare the used API:

   ```powershell
   .\Compare-SentryNativeApi.ps1 -NewHeader \\wsl.localhost\Ubuntu-20.04\root\.cache\gnollhack-sentry-native\sentry-native\include\sentry.h
   ```

   Exit 0: rerun with `-Update`. Exit 1: update `gnhsentry.c`, `GHSentryNative.cs` and `GHAvaloniaSentryNative.cs`
   to the new declarations, then rerun with `-Update -AcceptChanges`. Exit 2: the script's symbol list or parser
   needs fixing first.
6. Run `Build-GnhSentry.ps1` and `Build-LinuxSentryNative.ps1`.
7. Re-check every register entry whose `Package` names `Sentry`, `Sentry.Maui` or `sentry-native`, found by
   searching the `Package` fields, per `client_workarounds`. The lint's drift warnings cover only entries whose
   upstream fix is merged or released, so they are not the list. Then run
   `DEVEL\workarounds\Test-WorkaroundRegister.ps1`.
8. Smoke tests: wizard-mode `#crashtest` segmentation fault on Windows and Linux, restart, one native report
   uploaded; an unhandled managed exception still raises `AppDomain.UnhandledException` (WA-SENTRY-0001's Verify
   removal steps).
9. Commit the version change, header, both binaries and both manifests together.
