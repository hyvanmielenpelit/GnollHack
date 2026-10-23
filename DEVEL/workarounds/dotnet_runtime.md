# .NET Runtime Workarounds

- Component: .NET runtime (CoreCLR, Mono, GC, BCL, AOT compilers)
- Upstream: https://github.com/dotnet/runtime
- ID prefix: WA-RUNTIME

Entries follow the schema in [README.md](README.md). Append new entries at the end of the
file and never renumber or reuse an ID.

<!-- entries -->

## WA-RUNTIME-0001: Mono LLVM AOT breaks System.Net.Sockets on Android

- Status: active
- Added: 2025-05-14 (from git history)
- GnollHack version: 2.38 (256)
- Component: .NET runtime
- Package: Microsoft.NETCore.App.Runtime.AOT.Cross.android-arm64
- Defective version: .NET 9 Mono AOT compiler (SDK 9.0.300)
- Platforms: Android
- Upstream issue: https://github.com/dotnet/runtime/issues/104599
- Upstream status when added: reported
- Upstream status: wontfix
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/CompileMonoAoTLibraries2.targets` (`CompileMonoAotLibraries`); `win/win32/xpl/GnollHackM/GnollHackM.csproj` (`Import CompileMonoAoTLibraries2.targets`)
- Remove when: GnollHack's Android build no longer AOT-compiles with Mono and LLVM (for example after moving Android to CoreCLR), or a Mono release compiles System.Net.Sockets.dll correctly with LLVM, and Verify removal passes.

**Symptom.** In an Android build with `EnableLLVM`, networking through
`System.Net.Sockets` fails at run time. The commits that introduced the workaround were
getting the Azure storage client to work with LLVM.

**Cause.** Mono's LLVM AOT compilation of `System.Net.Sockets.dll` produces code that fails
at run time. Upstream closed the issue as not planned, since a workaround exists and the
configuration (Mono with LLVM) is not the default.

**Workaround.** When LLVM is enabled on Android, `CompileMonoAoTLibraries2.targets` runs
after `_AndroidAotCompilation` and AOT-compiles the linked `System.Net.Sockets.dll` for
android-arm64 again with `UseLLVM="false"`, then copies the result over the LLVM output.
This is the "AOT build without LLVM" suggested in the upstream issue.

**Verify removal.** Remove the `Import` of `CompileMonoAoTLibraries2.targets`. Then:
- Build and install an Android Release build with LLVM on an arm64 device.
- Use every network feature, including the Azure blob uploads and downloads and the
  server posts, and confirm each succeeds.

Checks:
- 2026-10-23 | .NET SDK 10.0.402 (runtime 10.0.13), workload android 36.1.118 | still-needed | Issue closed as not planned; no fix exists, and Android on .NET 10 still uses Mono with LLVM.

## WA-RUNTIME-0002: GC.GetTotalMemory walks the whole major heap on Mono

- Status: active
- Added: 2026-10-01 (from git history)
- GnollHack version: 3.21 (421)
- Component: .NET runtime
- Package: Mono runtime
- Defective version: .NET 10 Mono (SDK 10.0.401)
- Platforms: Android, iOS
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackX/GnollHackX/FrameTimeProfiler.cs` (`FrameTimeProfiler.HeapSampleIntervalMs`)
- Remove when: GnollHack's Android and iOS builds no longer run on Mono, or Mono reports the used heap size without iterating every object, and Verify removal passes.

**Symptom.** Reading the managed heap size on the UI thread takes milliseconds per call on
Android and iOS, which shows up as frame time in the profiler that measures it.

**Cause.** On Mono, `GC.GetTotalMemory(false)` reaches SGen's `major_get_used_size`, which
iterates every object in the major heap to sum their sizes.

**Workaround.** While the frame-time profiler is enabled, a timer samples the heap size
once a second on a pool thread. `BeginFrame` reads the sample and calls
`GC.GetTotalMemory` directly only after a collection.

**Verify removal.** Call `GC.GetTotalMemory(false)` in `BeginFrame` on every frame
instead of the sample. Then:
- Build for Android.
- Enable the frame-time profiler during a game, and confirm the UI thread's CPU time per
  frame does not grow compared with the sampled version.

Checks:
- 2026-10-23 | .NET SDK 10.0.402 (runtime 10.0.13), workload android 36.1.118, workload ios 27.0.10722 | still-needed | major_get_used_size still iterates all objects in dotnet/runtime.

## WA-RUNTIME-0003: DllNotFoundException does not carry the Win32 load error

- Status: active
- Added: 2026-10-22 (from git history)
- GnollHack version: 3.29 (429)
- Component: .NET runtime
- Package: CoreCLR
- Defective version: .NET 10 CoreCLR (SDK 10.0.402)
- Platforms: Windows (MAUI)
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/Platforms/Windows/NativeLibraryPreflightWindows.cs` (`NativeLibraryPreflightWindows`)
- Remove when: the runtime exposes the Win32 error of a failed native library load on the exception, GnollHack adopts that release, and Verify removal passes.

**Symptom.** When Windows refuses to load the game library, for example under Smart App
Control, the app cannot tell why: the exception does not say which Win32 error occurred.

**Cause.** CoreCLR's `LoadLibErrorTracker` puts the HRESULT only into the
`DllNotFoundException` message text, not into `HResult`, and for `ERROR_BAD_EXE_FORMAT`
throws a `BadImageFormatException` without it.

**Workaround.** `ProbeLoadError` loads the file again with `LoadLibraryExW` from the
application folder, with the loader's error box suppressed, to recover the Win32 error.
The failure is reported to Sentry with the error and the Smart App Control state, and a
failure of the game library shows a message box explaining the cause.

**Verify removal.** Read the Win32 error from the exception instead of calling
`ProbeLoadError`. Then:
- Build for Windows.
- Rename or corrupt `gnollhackwin.dll` in the output folder, start the game, and confirm
  the reported error code matches the one `ProbeLoadError` gave (126 for a missing file).

Checks:
- 2026-10-23 | .NET SDK 10.0.402 (runtime 10.0.13) | still-needed | LoadLibErrorTracker in dotnet/runtime still passes the HRESULT only as message text.
