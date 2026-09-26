# GnollHack Performance Measurement

This directory holds the performance test protocol, the scripts that run it, the
machine-readable record of measured runs, and the reports. The in-app tooling (the frame
timeline in `win/win32/xpl/GnollHackX/GnollHackX/Performance/`, `FrameTimeProfiler`, the
debug dashboard) and the offline analyzer in
`win/win32/xpl/GnollHackTests/GnollHack.PerformanceAnalyzer` implement it.

`DEVEL/performance.txt`, next to this directory, is a different thing: the manual,
human-readable benchmark log kept by the `performance_benchmarks` skill.

The point of all of it: a claim such as "the map judders on a 90 Hz phone" or "Build 15
is jerkier than Build 14" must come from what actually reached the screen, with the stage
that caused each hitch named, measured on a device whose thermal state is known, and
compared with a stated effect size and confidence interval.

## Why frame times are not enough

The render loop's own timing is taken where frames are *produced*. What the player sees is
decided later, where frames are *consumed*:

| Stage | What happens |
|-------|--------------|
| Display vsync | The panel refreshes, at whatever rate it is running at right now |
| Frame callback | Choreographer / CADisplayLink / CompositionTarget.Rendering runs on the UI thread |
| Pacing decision | The render loop renders or skips this refresh (divisor and modulo pattern) |
| Content advance | The animation counters advance one step per rendered frame |
| Invalidate to paint | The paint runs on the UI thread, or on a GL thread on Android |
| Paint | Lock, draw, flush |
| Render thread / GPU | HWUI, the GL thread's swap, ANGLE Present |
| Compositor / display | SurfaceFlinger, DWM or Core Animation puts the buffer on a vsync |

A perfectly regular callback stream can still show on screen as uneven motion: a skipped
refresh, a merged or dropped paint, a paint that missed its vsync, or a panel that changed
its refresh rate. Because animation advances per rendered frame rather than per unit of
time, any unevenness in *when* frames appear is seen as unevenness in *motion*.

## What is recorded

With **Settings > Frame Time Profiler** on (developer mode), the app keeps a ring of the
last 8640 display callbacks (60 s at 144 Hz), one record per callback:

- the platform's vsync time and the measured refresh period (median of recent vsync deltas);
- the pacing decision (rendered, catch-up, skipped by divisor, skipped by modulo, auxiliary
  canvas, suspended, ...), the target map rate and the refresh rate the pacing logic assumed;
- the content counters requested by the tick and the counters and map data generation the
  paint actually drew;
- the invalidation outcome, and the paint outcome (painted, coalesced into a later paint,
  overlay visible, reentrant, ...), with the paint's thread, start, lock, draw end and flush
  end;
- GC counts.

Every stage refers to the frame by its `FrameId`, so a paint on another thread, or one merged
into a later one, is attributed to the tick that requested it. The platform side adds:

| Platform | In-app | Trace markers | External capture |
|----------|--------|---------------|------------------|
| Android | FrameMetrics (HWUI frames: vsync, sync start, completion, GPU time from API 31) | `GH.Tick`, `GH.Paint`, `GH.Flush` sections and `GH.FrameId` / `GH.PaintFrameId` counters in Perfetto | Perfetto with SurfaceFlinger's frame timeline (Android 12+): actual present time and jank type per frame; `gfxinfo framestats` |
| Windows | DWM composition timing (vblank, refresh and composition counters, in QPC) | `GnollHack-Rendering` EventSource (ETW, WPA) | PresentMon with `--qpc_time`: display time per present |
| iOS | CADisplayLink timestamp and target timestamp | `os_signpost` Tick, Paint and Flush intervals (the shim in `gnollhackios.c`, compiled into the app when `GNH_IOS_SIGNPOSTS` is defined) | Instruments, Animation Hitches template |

A **frame marker** is drawn on the map while the timeline records: a Gray-coded `FrameId mod
256` strip in the top-left corner and a block crossing the top edge in two seconds of content
time. Film the screen in slow motion to see directly which frame was on screen and whether
the block moves evenly.

The debug dashboard's **SCREEN** section shows the displayed rate, the measured, assumed and
target rates (orange when the assumed rate is off by more than 5 %), the hitch time ratio,
the pacing error, the coalesced paints and the last cadence change. Cadence changes are also
written to the screen log (`CADENCE 60->30 fps`, `REFRESH 8.3->16.7 ms`) and emitted as trace
markers.

**Developer menu > Dump Frame Log** writes a run record of everything the ring holds
(`run_*.json` plus `frametimeline_*.csv` and `compositorframes_*.csv`) to the archive's
`performance` directory and shares it with the legacy `framelog.csv` as `framelog.zip`. The
app clears the archive directory when it starts, so keep the shared zip or copy the files
before restarting; `Run-PerformanceSuite.ps1` copies each record before it stops the app.

## Metrics

A frame is *displayed* at the first vsync after it was ready; when two frames are ready for
the same vsync only the later one is shown, and the earlier one counts as dropped. Ready is
the flush end or, on Android, the completion of the HWUI frame that carried it (for a
GL-thread paint, the first HWUI frame whose sync began after the flush). A measured display
time from PresentMon or Perfetto replaces the estimate; every record says which it is.

Let `R` be the measured refresh period, `T` the target content period (`1 / map FPS`), and,
for each displayed frame `j`, `g_j` the time since the previous displayed frame and `dc_j`
the main-counter advance since then.

| Metric | Definition |
|--------|------------|
| Displayed FPS | Displayed frames per second, pauses (menus, overlays, suspension, resizes) excluded |
| Hitch time ratio | Sum of `g_j - T` over frames with `g_j > T + R/2`, per second. Apple's bands: under 5 ms/s good, 5 to 10 warning, over 10 critical |
| Pacing error | `e_j = g_j - dc_j * T`: zero for perfectly even motion at any frame rate. RMS and P99 of its magnitude |
| Judder | Share of frames with `abs(e_j) > R/4`. A target rate that does not divide the refresh rate makes judder unavoidable: 40 FPS on 60 Hz alternates one and two refreshes, an error of exactly `R/2` on every frame |
| Dropped, coalesced, not run | Painted but never shown; invalidated but merged into a later paint; rendered but the paint returned early |
| Repeated refreshes | Refreshes a frame stayed on screen beyond its intended hold, per second |
| Latency | Display time minus the vsync of the tick that produced the frame, P50 and P99 |
| Assumed refresh mismatch | The pacing logic divided a refresh rate more than 5 % away from the measured one |
| On-screen pacing | The classic percentile set (P50 to Max, 1 % low, jank, hitch ratio, stutter index) over the displayed-frame gaps, judged against `T` rather than `R` |

The render loop's callback intervals, paint and lock durations, GC and allocation data are
still collected by `FrameTimeProfiler` and shown on the dashboard's FRAME section; they are
diagnostic series, not the measure of smoothness.

## Hitch attribution

Every hitch and every judder frame is charged to the **first** stage, in pipeline order,
that exceeded its budget:

| Order | Cause | Test |
|-------|-------|------|
| 1 | `DisplayMode` | The measured refresh period moved by more than 5 % across the gap, or the pacing logic assumes a rate more than 5 % off the measured one |
| 2 | `PaintCpu` / `Gpu` | A late or missed callback while the UI thread was still painting the previous map frame |
| 2 | `UiThreadRequests` | A late or missed callback after the UI thread spent more than `R/2` handling game requests (floating texts, messages, windows, ...) |
| 2 | `UiThreadLateGc` / `UiThreadLate` | A missed callback, or a callback more than `R/2` after its vsync, with or without a collection in the gap |
| 3 | `PacingPolicy` | A modulo skip or catch-up render in the gap, or a refresh-to-target ratio the divisor pattern cannot pace evenly (within the pattern's longest hold) |
| 4 | `PaintNotRun` | A rendered tick in the gap produced no paint (coalesced, early return, no invalidation) |
| 5 | `DispatchLate` | A paint started more than `R/2` after its invalidation |
| 6 | `GameLock` | Map data lock wait over `T/4` |
| 7 | `PaintCpu` | Draw over `3T/4` |
| 8 | `Gpu` | Flush over `T/2`, or a compositor frame's GPU time over `R` |
| 9 | `Compositor` | Measured on screen later than the vsync it was ready for, a painted frame in the gap never shown, or the compositor ran long |
| 10 | `Unattributed` | Nothing identified. Its share of hitch time is reported as a measure of the instrument itself |

### Content events

Each record also carries the content that appeared since the previous tick and the time the
UI thread spent handling the game's requests before it. Requests are sorted into
`FloatingText`, `ScreenText`, `ConditionText`, `GuiEffect`, `ScreenFilter`, `Message`,
`ViewChange`, `Window` and `OtherRequest`; `MapUpdate` marks a paint that drew new map data.
The report's **Content events** table compares, per kind, the hitch rate of the gaps in which
that content appeared with the hitch rate of gaps in which nothing did, with a one-sided
Fisher exact p-value. A rate far above the quiet one, with a small p, ties the hitches to that
content; the worst-hitch tables then show, per tick, the request time and the events, next to
the paint's draw and flush times that say *how* the content cost the frame. Kinds often arrive
together, so read the rate of one kind with the others in view.

Offline, the analyzer segments the run with PELT change-point detection over 250 ms buckets of
displayed FPS and pacing error, and lists the events near each boundary (refresh change,
target change, canvas pause, GC).

## Protocol

1. **Release builds only.** Debug builds carry Sentry tracing and debug logging, no
   trimming, and different JIT behaviour. The analyzer refuses to append a non-Release run
   to `history.jsonl`.
2. **Warm-up** of 10 seconds is discarded. **Window** of 60 seconds for scripted
   scenarios, 120 seconds for gameplay.
3. **Repetitions**: at least 5 runs per configuration for a decision, 3 for a smoke
   check. The report prints the minimum detectable effect at the observed spread; raise
   the count when it is too coarse for the question.
4. **Interleaving**: ABBA then BAAB, never AAAA BBBB, so that drift within the batch
   affects both arms equally. `Run-PerformanceSuite.ps1 -Pairs 2` produces exactly that.
5. **Cool-down** of 20 seconds between runs, extended until the device is back in the
   thermal class it started the batch in (up to 5 minutes).
6. **One variable per comparison.** A configuration is the full toggle vector; an A/B
   run varies exactly one entry.
7. **Windows reference runs** are done plugged in, on the High performance power plan,
   with the laptop on a hard surface, starting from a cold machine. Close other
   applications; the thermal probe lists the top CPU consumers so a busy background
   process is visible in the record.
8. **Android reference runs** are done on a charged, unplugged device (charging heats
   it) with the screen at a fixed brightness, no other apps recently used.
9. **Power state is a controlled factor.** Whether the device is plugged in changes the
   CPU governor, the power plan, and the thermal budget on every platform, so it is
   recorded before and after each run, held constant for a whole batch, and never varied
   between the arms of a comparison.
10. **Same cadence per comparison.** Arms must share the refresh period and the target map
    rate; `compare` refuses otherwise (`--allow-mixed-cadence` overrides with a warning).
    Record whether the Windows display has variable refresh rate enabled.

## Statistics

- Percentiles are nearest-rank on the raw values.
- **Run level decides.** Each run contributes one value per metric. The difference of arm
  medians gets a bootstrap 95 percent confidence interval by resampling runs; a
  difference counts only when that interval excludes zero **and** exceeds the
  pre-registered threshold. For the smoothness series (`--series smoothness`): hitch time
  ratio worse by 2 ms/s, pacing error RMS worse by 1 ms, displayed FPS worse by 5 %. For
  the external series: P99 worse by 20 % or by one target period, hitch ratio worse by
  2 ms/s, 1 % low FPS worse by 10 %. The Mann-Whitney p-value is reported, not decided on:
  at five runs per arm the smallest attainable p is 0.0079.
- **Interval level describes.** All intervals of an arm pooled: Mann-Whitney U and Cliff's
  delta, the Hodges-Lehmann shift, and bootstrap intervals on the median and P99
  differences. Pooled intervals are not independent, so this level never decides.
- **No outlier removal.** A hitch is the phenomenon. Runs are excluded only for a recorded
  reason: thermal throttling, too few frames, capture failure.
- **Minimum detectable effect** is printed so that "no difference" reads as "no difference
  larger than X".

## Test matrix

Every mobile device here runs at 60 Hz, where the default map rate divides evenly, so the
uneven-cadence case is induced with the map FPS setting:

| Device | Configuration | Expected |
|--------|---------------|----------|
| Any 60 Hz device | Map 60 (default) | Instrument baseline: pacing error and hitch ratio near zero |
| Any 60 Hz device | Map 40 | 16.7 / 33.3 ms alternation, pacing error 8.3 ms RMS, cause `PacingPolicy` |
| Windows, 144 Hz | Map 60 | Four 13.9 ms frames then 27.8 ms, cause `PacingPolicy` |
| Windows, 120 Hz | Map 80 | 8.3 / 16.7 ms alternation, cause `PacingPolicy` |
| Windows | Switch 144 to 120 Hz during a run | A `REFRESH` change and `DisplayMode` hitches |
| Windows | GPU vs CPU canvas | Dispatch latency (`DispatchLate`), present alternation |
| Google Pixel 6a | GPU (default) vs CPU canvas | Coalesced paints, latency, Perfetto jank types |
| iPhone 11 | Default | Signposts beside Instruments' Animation Hitches; menu scrolling over the map (display link in the default run-loop mode only) |
| iPad (8th generation) | Map 40 | Filmed at 240 fps with the iPhone 11: the frame marker advances one then two refreshes |

Not covered by this hardware: adaptive and LTPO Android panels, ProMotion, and 90 / 165 Hz
panels. The unit tests cover their cadences with synthetic timelines.

## Scenarios

| Id | Workload |
|----|----------|
| W-idle | Map on screen, hero idle, no input: tile animations only. The trigger for adaptive-refresh drops |
| W1 | Minimap mode, whole level revealed (wizard mode, Ctrl+F) |
| W-move | A movement-heavy replay segment |
| W-pan | Map panning and target clipping |
| W-effects | Spell and screen-filter effects |
| W-menus | Inventory open and close every 2 s (canvas switches; excluded as pauses) |
| replay | A reference `.gnhrec` played back at speed 1.0: identical content in both arms |

## Running a batch

Windows, Build 14 versus HEAD, with in-app records joined to PresentMon (elevated console):

```powershell
DEVEL\performance\scripts\Run-PerformanceSuite.ps1 -Platform Windows `
    -ArmAExe C:\builds\b14\GnollHackM.exe -ArmALabel Build14 -ArmABuild 4.3.0-14 `
    -ArmBExe C:\builds\head\GnollHackM.exe -ArmBLabel HEAD `
    -Scenario W1 -Pairs 2 -OutDir C:\performance\w1 `
    -InAppRecordDir <GnollHack path>\archive\performance
```

Android, with Perfetto and its CSV export (`trace_processor_shell` on `PATH`, in
`TRACE_PROCESSOR`, or `-TraceProcessorPath`); copy each shared `framelog.zip`'s files into
the `-InAppRecordDir` directory when the script asks for them:

```powershell
DEVEL\performance\scripts\Run-PerformanceSuite.ps1 -Platform Android `
    -ArmAPackage com.hyvanmielenpelit.gnollhack -ArmALabel GPU `
    -ArmBPackage com.hyvanmielenpelit.gnollhack -ArmBLabel CPU `
    -Scenario W-idle -Pairs 2 -Perfetto -OutDir C:\performance\idle -InAppRecordDir C:\performance\inbox
```

Each run directory gets `smoothness.md`; the batch gets `report.md` (external series) and,
with in-app records, `report_smoothness.md`. When both arms are the same package, change the
setting or install the other build when the script announces the arm.

## Analyzer

Build once with `dotnet build win\win32\xpl\GnollHackTests\GnollHackTests.sln -c Release`.

```powershell
$a = 'win\win32\xpl\GnollHackTests\GnollHack.PerformanceAnalyzer\bin\Release\net10.0\GnollHack.PerformanceAnalyzer.exe'
& $a smoothness run_20260926_101500_manual_dump.json --presentmon presentmon.csv --process GnollHackM --out smoothness.md
& $a smoothness run_20260926_101500_manual_dump.json --perfetto C:\performance\idle\01_GPU\gfxinfo --out smoothness.md
& $a compare --a runsA --b runsB --label-a GPU --label-b CPU --series smoothness --out report_smoothness.md
& $a ingest --presentmon run.csv --process GnollHackM --refresh-hz 144 --target-fps 72 --out run.json
& $a history --file DEVEL\performance\history.jsonl --append runsA runsB
```

The `smoothness` report gives the headline metrics (recomputed and as the app reported
them), the join statistics for measured display times, the hitch-cause table, the ten worst
hitches each with its full stage timeline, and the change-point segments.

## iOS signposts

The shim is in `win/win32/xpl/gnollhackios/gnollhackios.c`. After rebuilding
`libgnollhackios.a` on the Mac and copying it to `win/win32/xpl/GnollHackM/Platforms/iOS/libs/`,
add `GNH_IOS_SIGNPOSTS` to the iOS `DefineConstants` in `GnollHackM.csproj`. Without the
rebuilt library the symbol must stay undefined: .NET for iOS links `__Internal` calls
directly, so the app would not link. In Instruments, record with the Animation Hitches and
Points of Interest instruments; the intervals appear under the `fi.hyvanmielenpelit.gnollhack`
subsystem.

## Contents

| Path | What |
|------|------|
| `scripts/Run-PerformanceSuite.ps1` | Interleaved two-arm batch driver: launches the app, captures presented frames, collects in-app records, gates on thermal state, analyzes, compares, appends to history |
| `scripts/Capture-PresentMon.ps1` | Windows: presentation timing for one process with PresentMon, QPC timestamps included |
| `scripts/Capture-AndroidFrames.ps1` | Android: `gfxinfo framestats` polling, optional Perfetto trace and CSV export |
| `scripts/Get-ThermalState.ps1` | Thermal and power facts for the Windows host or an Android device, as JSON |
| `scripts/Common.ps1` | Shared helpers (tool resolution, JSON writing, git facts) |
| `perfetto/frametimeline.pbtxt` | Perfetto trace config |
| `perfetto/export_frames.sql`, `perfetto/export_app_slices.sql` | `trace_processor` queries behind the Perfetto CSVs |
| `schema/run-record.schema.json` | The in-app run record format (schema v2) |
| `history.jsonl` | Append-only record of Release runs, created on first append |
