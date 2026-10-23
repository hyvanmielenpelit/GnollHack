# Android Workarounds

- Component: .NET for Android, the Android OS, AndroidX and Google Play libraries
- Upstream: https://github.com/dotnet/android, https://issuetracker.google.com
- ID prefix: WA-ANDROID

Entries follow the schema in [README.md](README.md). Append workarounds before the
`<!-- known-issues -->` line if there is one, otherwise at the end of the file, and known
issues at the end of the file. Never renumber or reuse an ID.

<!-- entries -->

## WA-ANDROID-0001: Choreographer frameTimeNanos can be one refresh stale

- Status: active
- Added: 2026-10-02 (from git history)
- GnollHack version: 3.21 (421)
- Component: Android OS
- Package: Android
- Defective version: unknown
- Platforms: Android
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackX/GnollHackX/Performance/GHSmoothnessMetrics.cs` (`GHSmoothnessMetrics.CorrectStaleVsync`)
- Remove when: Choreographer's frameTimeNanos matches the vsync HWUI records for the same frame on the Android versions GnollHack is measured on, and Verify removal passes.

**Symptom.** The performance suite's smoothness analysis misattributes frames: for whole
runs at a time, the vsync time of each frame callback is one refresh earlier than the
vsync HWUI reports for the same `doFrame`, so frame lateness and dropped frames come out
wrong.

**Cause.** Android's Choreographer sometimes hands the frame callback a `frameTimeNanos`
one refresh older than the intended vsync that FrameMetrics records for the same frame.
The documentation does not describe this.

**Workaround.** The analysis matches each tick to the FrameMetrics report of the same
`doFrame` (the latest intended vsync at or before the callback start). Where the two differ
by one refresh, within 1 ms, it uses HWUI's vsync; ticks without a matching report take
their neighbors' state. The raw CSV keeps the original value, and
`smoothness.vsyncCorrectedShare` reports the share of ticks corrected
(`DEVEL/performance/README.md`, "Stale vsync correction").

**Verify removal.** Without changing the code:
- Run the Android performance suite with FrameMetrics coverage on current Android versions.
- Confirm that `smoothness.vsyncCorrectedShare` is 0 in every run.
- Then remove `CorrectStaleVsync` and its call, rebuild, rerun, and confirm the smoothness
  figures are unchanged.

Checks:
- 2026-10-23 | Android (device version not re-tested), workload android 36.1.118 | unverified | Seeded from git history; no upstream report exists.
