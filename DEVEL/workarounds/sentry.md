# Sentry Workarounds

- Component: Sentry SDKs (sentry-dotnet, Sentry.Maui, sentry-native, sentry-cocoa)
- Upstream: https://github.com/getsentry/sentry-dotnet, https://github.com/getsentry/sentry-native
- ID prefix: WA-SENTRY

Entries follow the schema in [README.md](README.md). Append workarounds before the
`<!-- known-issues -->` line if there is one, otherwise at the end of the file, and known
issues at the end of the file. Never renumber or reuse an ID.

<!-- entries -->

## WA-SENTRY-0001: inproc's unhandled-exception filter does not chain to the filter it displaced

- Status: active
- Added: 2026-10-23 (from git history)
- GnollHack version: 3.30 (430)
- Component: sentry-native
- Package: sentry-native
- Defective version: sentry-native 0.17.2
- Platforms: Windows (MAUI)
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/gnhsentrywin/gnhsentry.c` (`gnh_unhandled_filter`); `win/win32/xpl/gnhsentrywin/gnhsentry.c` (`gnh_crash_install`); `win/win32/xpl/GnollHackM/GHSentryNative.cs` (`GHSentryNative.InitOnce`)
- Remove when: GnollHack uses a sentry-native release whose inproc backend calls the previous unhandled-exception filter, and Verify removal passes.
- Related: WA-SENTRY-0002

**Symptom.** Once sentry-native's inproc backend is initialized, an unhandled managed
exception no longer reaches CoreCLR's unhandled-exception filter, so
`AppDomain.UnhandledException` is not raised and the "Unhandled exception." message and the
".NET Runtime" event-log entry are missing.

**Cause.** `sentry_init` installs inproc's filter with `SetUnhandledExceptionFilter`, and
that filter never calls the filter it displaced.

**Workaround.** `gnh_crash_prepare` records the filter current before `sentry_init`.
`gnh_crash_install` puts `gnh_unhandled_filter` in front of sentry-native's filter; it hands
hardware faults to sentry-native and then passes every exception to the recorded filter.
`gnh_crash_uninstall` gives the slot back before `sentry_close`. When the install fails,
`GHSentryNative.InitOnce` closes sentry-native and runs without native crash capture.

**Verify removal.** With a sentry-native release that chains, make `gnh_crash_install`
skip the filter swap and keep only the vectored handler. Then:
- Build gnhsentry.dll and a Windows Release build.
- Raise an unhandled managed exception on the main thread, and confirm that
  `AppDomain.UnhandledException` runs, Sentry .NET reports the exception, and the Windows
  event log has the ".NET Runtime" entry.
- In wizard mode, run `#crashtest` with a segmentation fault, and confirm that exactly one
  native crash report is uploaded at the next start.

Checks:
- 2026-10-23 | sentry-native 0.17.2 | still-needed | No upstream issue found; the pinned release does not chain.

## WA-SENTRY-0002: Native faults under managed frames never reach inproc's crash handler

- Status: active
- Added: 2026-10-23 (from git history)
- GnollHack version: 3.30 (430)
- Component: sentry-native
- Package: sentry-native
- Defective version: sentry-native 0.17.2
- Platforms: Windows (MAUI)
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/gnhsentrywin/gnhsentry.c` (`gnh_vectored_handler`)
- Remove when: sentry-native captures hardware faults in native modules called from managed code in a CoreCLR process without help, and Verify removal passes.
- Related: WA-SENTRY-0001

**Symptom.** An access violation or illegal instruction in a native library called from
managed code, such as gnollhackwin.dll, SkiaSharp, FMOD or ANGLE, ends the process
without a sentry-native crash report.

**Cause.** Under managed frames such a fault ends in CoreCLR's fail-fast and never reaches
the unhandled-exception filter, which is the only hook inproc's backend installs on
Windows. Whether the missing capability belongs to sentry-native (a vectored-handler mode)
or to the .NET runtime is not established.

**Workaround.** `gnh_vectored_handler`, a first vectored exception handler installed by
`gnh_crash_install`, passes crash codes raised inside an allowlisted native module to
`sentry_handle_exception`, and then lets the exception continue unchanged, so CoreCLR's
fail-fast and WER still run.

**Verify removal.** Disable the `AddVectoredExceptionHandler` call in `gnh_crash_install`.
Then:
- Build gnhsentry.dll and a Windows Release build.
- In wizard mode, run `#crashtest` with a segmentation fault from a game started from the
  main menu.
- Confirm that a native crash report with the faulting gnollhackwin.dll frame is uploaded
  at the next start.

Checks:
- 2026-10-23 | sentry-native 0.17.2 | still-needed | No upstream issue found; inproc installs only the unhandled-exception filter.

## WA-SENTRY-0003: Sentry .NET captures no native crashes in JIT builds on Windows and Linux

- Status: active
- Added: 2026-10-22 (from git history)
- GnollHack version: 3.29 (429)
- Component: sentry-dotnet
- Package: Sentry, Sentry.Maui
- Defective version: Sentry 6.12.1
- Platforms: Windows (MAUI), Linux
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/GHSentryNative.cs` (`GHSentryNative`); `win/win32/xpl/GnollHackM/GHSentryNativeScopeObserver.cs` (`GHSentryNativeScopeObserver`); `win/win32/xpl/GnollHackM/GnollHackM.csproj` (`SentryNative`)
- Remove when: Sentry .NET starts its native SDK, with scope sync and crash upload, in JIT builds on Windows and Linux, GnollHack uses that release, and Verify removal passes.
- Related: WA-SENTRY-0004

**Symptom.** A crash in native code, such as the C game core, is not reported by Sentry .NET
on the Windows and Linux builds.

**Cause.** Sentry .NET starts its own native SDK only in trimmed or Native AOT builds.
GnollHack ships JIT builds on these platforms.

**Workaround.** `GHSentryNative` initializes sentry-native itself before `SentrySdk.Init`:
gnhsentry.dll on Windows, libsentry.so on Linux, inproc backend, with a transport that
stores crash envelopes in an outbox that is uploaded at the next start. It feeds
`CrashedLastRun` to Sentry .NET, and `GHSentryNativeScopeObserver` mirrors the managed
scope into the native one. `SentryNative` is `false` for these builds, so Sentry .NET never
starts a second sentry-native beside it.

**Verify removal.** With a Sentry .NET release that supports native crashes in JIT builds:
- Remove `GHSentryNative.Init` and the scope observer from `WindowsSentryNative` and
  `GHAvaloniaSentry`, and enable the SDK's own native support (remove `SentryNative=false`
  from `GnollHackM.csproj`).
- Build Windows and Linux Release builds.
- In wizard mode, run `#crashtest` with a segmentation fault on each, and confirm that the
  next start uploads a native crash event carrying the managed scope's tags, extras and
  breadcrumbs, and that the release-health session ends as crashed.

Checks:
- 2026-10-23 | Sentry 6.12.1, Sentry.Maui 6.12.1 | still-needed | No upstream issue found; the comment states the SDK's native support is limited to trimmed or Native AOT builds.

## WA-SENTRY-0004: SentrySdk.CaptureEnvelope can drop native-only event fields

- Status: active
- Added: 2026-10-22 (from git history)
- GnollHack version: 3.29 (429)
- Component: sentry-dotnet
- Package: Sentry, Sentry.Maui
- Defective version: Sentry 6.12.1
- Platforms: Windows (MAUI), Linux
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/GHSentryNative.cs` (`GHSentryNative.UploadOutboxAsync`)
- Remove when: Sentry .NET forwards an envelope's bytes unchanged, or WA-SENTRY-0003 is removed, and Verify removal passes.
- Related: WA-SENTRY-0003

**Symptom.** A sentry-native crash envelope sent through `SentrySdk.CaptureEnvelope` can
lose native-only fields, such as registers, signal metadata and `debug_meta` details.

**Cause.** `CaptureEnvelope` re-parses the event into Sentry .NET's model, which does not
keep every field sentry-native writes.

**Workaround.** `UploadOutboxAsync` posts the stored envelope bytes unchanged to the DSN's
envelope endpoint with its own `HttpClient`, oldest first, deleting files that are
accepted or rejected with a 4xx other than 429, and keeping the rest for the next start.

**Verify removal.** Replace the HTTP post with `SentrySdk.CaptureEnvelope`. Then:
- Build a Windows Release build.
- In wizard mode, run `#crashtest` with a segmentation fault, and restart.
- Compare the uploaded event with one sent by the current code: registers, the exception
  mechanism and signal metadata, and the module list must all be present.

Checks:
- 2026-10-23 | Sentry 6.12.1, Sentry.Maui 6.12.1 | unverified | No upstream issue found; not re-tested.

## WA-SENTRY-0005: inproc's SA_ONSTACK handlers overflow the alternate stack for CoreCLR's chained handlers

- Status: active
- Added: 2026-10-22 (from git history)
- GnollHack version: 3.29 (429)
- Component: sentry-native
- Package: sentry-native
- Defective version: sentry-native 0.17.2
- Platforms: Linux
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/Platforms/Avalonia/GHAvaloniaSentryNative.cs` (`GHAvaloniaSentryNative.ClearAlternateStackOnManagedFaultSignals`)
- Remove when: GnollHack uses a sentry-native release whose CHAIN_AT_START strategy runs the chained handler on the interrupted thread's stack for these signals, and Verify removal passes.

**Symptom.** With sentry-native initialized, a caught `DivideByZeroException` ends the
process.

**Cause.** inproc installs its signal handlers with `SA_ONSTACK`, so CoreCLR's handler,
chained first under `SENTRY_HANDLER_STRATEGY_CHAIN_AT_START`, runs on the small alternate
signal stack. CoreCLR dispatches managed exceptions from its SIGILL, SIGTRAP, SIGBUS and
SIGFPE handlers on the current stack, which then overflows; only its SIGSEGV handler
leaves the alternate stack first.

**Workaround.** After `sentry_init`, `ClearAlternateStackOnManagedFaultSignals` clears
`SA_ONSTACK` from the installed handlers for SIGILL, SIGTRAP, SIGBUS and SIGFPE. SIGSEGV
keeps it for native stack overflows. If the adjustment throws, `GHSentryNative.InitOnce`
closes sentry-native again.

**Verify removal.** Remove the call from `GHAvaloniaSentryNative.AfterInit`. Then:
- Build the Linux Avalonia Release build.
- Run code that throws and catches a `DivideByZeroException` from an integer division,
  and confirm that the game continues.
- In wizard mode, run `#crashtest` with a segmentation fault, and confirm that a native
  crash report is uploaded at the next start.

Checks:
- 2026-10-23 | sentry-native 0.17.2 | still-needed | No upstream issue found; the pinned release still sets SA_ONSTACK.

## WA-SENTRY-0006: UploadDebugInfoToSentry uploads ELF symbols only for Android

- Status: active
- Added: 2026-10-22 (from git history)
- GnollHack version: 3.29 (429)
- Component: sentry-dotnet
- Package: Sentry
- Defective version: Sentry 6.12.1
- Platforms: Linux
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/GnollHackM.csproj` (`UploadGnollHackLinuxSoToSentry`)
- Remove when: Sentry's MSBuild integration uploads ELF debug files for Linux JIT builds, GnollHack uses that release, and Verify removal passes.

**Symptom.** A Linux Release build with `SentryUploadSymbols` uploads no native symbols, so
native frames in libgnollhacklinux.so stay unsymbolicated.

**Cause.** In a standard managed build, `Sentry.targets` adds the `elf` symbol type only
when the target platform is Android.

**Workaround.** `UploadGnollHackLinuxSoToSentry` runs after `UploadDebugInfoToSentry` and
uploads the repository copy of libgnollhacklinux.so, which keeps its DWARF, with
`sentry-cli debug-files upload -t elf`.

**Verify removal.** Delete the target. Then:
- Build the Linux Avalonia Release build with `SENTRY_AUTH_TOKEN` set and
  `GnhSentryUpload=true`.
- Confirm in the build log that `UploadDebugInfoToSentry` uploads libgnollhacklinux.so,
  and in Sentry that its debug file is present for the build.

Checks:
- 2026-10-23 | Sentry 6.12.1 | still-needed | No upstream issue found; Sentry.targets at 6.12.1 still limits elf to Android.

## WA-SENTRY-0007: Sentry.Maui invokes the UseSentry options callback twice

- Status: active
- Added: 2026-10-23 (from git history)
- GnollHack version: 3.30 (430)
- Component: Sentry.Maui
- Package: Sentry.Maui
- Defective version: Sentry.Maui 6.12.1
- Platforms: Windows (MAUI)
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/MauiProgram.cs` (`MauiProgram.CreateMauiApp`)
- Remove when: Sentry.Maui invokes the options callback once, GnollHack uses that release, and Verify removal passes.

**Symptom.** Code with side effects in the `UseSentry` options callback runs twice, so a
one-time initialization placed there, such as sentry-native's, would run twice.

**Cause.** `UseSentry` registers the callback with `services.Configure` and also invokes
it directly on a temporary `SentryMauiOptions` to collect integration event binders.

**Workaround.** `WindowsSentryNative.Init` runs once in `CreateMauiApp`, before the
builder, and the Windows `UseSentry` callback only reads its results.

**Verify removal.** With a fixed Sentry.Maui, move `WindowsSentryNative.Init` into the
Windows `UseSentry` callback. Then:
- Build a Windows Debug build.
- Confirm with a breakpoint or log line that the callback runs once per start, and that
  the native crash capture initializes once.

Checks:
- 2026-10-23 | Sentry.Maui 6.12.1 | still-needed | No upstream issue found; SentryMauiAppBuilderExtensions.UseSentry at 6.12.1 still invokes the callback twice.

## WA-SENTRY-0008: The Android and iOS scope observers ignore a null extra

- Status: active
- Added: 2026-09-04 (from git history)
- GnollHack version: 3.15 (415)
- Component: Sentry.Maui
- Package: Sentry.Maui
- Defective version: Sentry.Maui 6.9.0
- Platforms: Android, iOS
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackX/GnollHackX/GHApp.cs` (`GHApp.ClearSentryCrashContextScope`)
- Remove when: The Android and Cocoa scope observers remove the native extra when the value is null, GnollHack uses that release, and Verify removal passes.

**Symptom.** Setting a scope extra to null leaves the previous value in the native SDK's
scope, so a later native crash carries a stale crash-context snapshot.

**Cause.** `AndroidScopeObserver.SetExtra` and `CocoaScopeObserver.SetExtra` log and return
early when the value is null, without removing the native extra.

**Workaround.** `ClearSentryCrashContextScope` never sets null; it overwrites the debug
buffers, game state and crash-context stamp extras with a sentinel string.

**Verify removal.** Change `ClearSentryCrashContextScope` to set the three extras to null.
Then:
- Build an Android Debug build.
- Start a game so that a crash-context snapshot is written, and return to the main menu.
- Crash the process natively from the main menu, for example with
  `adb shell run-as <package> kill -SEGV <pid>`, and restart.
- Confirm that the native crash event carries no "Debug Buffers" or game state extra from
  the finished game. Repeat on iOS with a debugger-induced native crash.

Checks:
- 2026-10-23 | Sentry.Maui 6.12.1 | still-needed | No upstream issue found; both observers at 6.12.1 still return early on null.
