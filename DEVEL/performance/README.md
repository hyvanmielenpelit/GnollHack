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
last 32768 display callbacks (136 s at 240 Hz, about 10 MB while the profiler is on), one
record per callback:

- the platform's vsync time; the refresh period, which is the panel's own where the
  platform reports it (Windows: DWM) and otherwise the median of recent vsync deltas; and the
  callback period, the median interval between display callbacks;
- the pacing decision (rendered, catch-up, skipped by divisor, skipped by modulo, auxiliary
  canvas, suspended, ...), the target map rate and the refresh rate the pacing logic assumed;
- the content counters requested by the tick and the counters and map data generation the
  paint actually drew;
- the invalidation outcome, and the paint outcome (painted, coalesced into a later paint,
  overlay visible, reentrant, ...), with the paint's thread, start, lock, draw end and flush
  end;
- GC counts, and the process's total GC pause time (`GC.GetTotalPauseDuration`; the Mono
  runtime on Android and iOS may not report it, and a record then says so).

A measurement window (a suite run or a window command) may be at most as long as the ring
holds at the current refresh rate: 90 % of its capacity divided by the refresh rate, counted
as at least 60 Hz (`GHFrameTimeline.MaxWindowSeconds`), which is 491 s at 60 Hz and 122 s at
240 Hz. A suite refuses a longer window, and a window command is shortened to the limit. A
saved record's optional `timeline` block says how many ticks the window requested and how
many the ring still retained; a truncated window is excluded with the reason
`window longer than the frame timeline holds (kept N of M ticks)`.

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
the block moves evenly. The [in-game Test Performance](#in-game-test-performance) hides it
while it measures, so that the report reflects the game as a player sees it.

The debug dashboard's **SCREEN** section shows the displayed rate, the measured, assumed and
target rates (orange when the assumed rate is off by more than 5 %), the hitch time ratio,
the pacing error, the coalesced paints and the last cadence change. Cadence changes are also
written to the screen log and emitted as trace markers:

| Line | Meaning |
|------|---------|
| `CADENCE 60->30 fps` | The displayed frame rate of the last second differs from that of the three seconds before by more than 10 % |
| `REFRESH 8.3->16.7 ms` | The panel's refresh period changed for a full second: a display mode switch, variable refresh rate, or an adaptive panel |
| `CALLBACKS 6.9->13.9 ms (panel 6.9 ms)` | The display callbacks left the panel's rate (or returned to it) for a full second while the panel did not change: the UI framework called the render loop less often than the panel refreshed. Only where the platform reports the panel period separately (Windows); elsewhere the callbacks are the vsync measurement |

`compositorframes_*.csv` in a dump has DWM's own `RefreshPeriodMs` per frame on Windows. With
variable refresh rate enabled, DWM reports the panel's maximum rate while the panel may run
slower, so callbacks read as below the panel's rate; record whether VRR is on (protocol
rule 10).

**Developer menu > Dump Frame Log** writes a run record of everything the ring holds
(`run_*.json` plus `frametimeline_*.csv` and `compositorframes_*.csv`) to the archive's
`performance` directory and shares it with the legacy `framelog.csv`, and with `screenlog.txt`
when the screen log holds entries, as `framelog.zip`. The
app clears the archive directory when it starts, so keep the shared zip or copy the files
before restarting. Its "before" thermal reading is the one taken when the profiler was
switched on, and the record says when that was.

**Window commands** bracket exactly the protocol's window instead. While the profiler is on
the app accepts one line, `begin <scenario> <arm> <delaySeconds> <windowSeconds>` (or `end`,
`cancel`): it opens a window after the delay, saves that window's frames after the window,
and then writes `window.done`, naming the saved JSON, next to the record. On Windows the
line is written to `window.cmd` in the archive's `performance` directory, which the app
polls every 500 ms. On Android it is a broadcast,
`adb shell am broadcast -a <package>.PERFORMANCE_WINDOW -p <package> --es command '<line>'`,
and the record goes to `/sdcard/Android/data/<package>/files/performance/`, where
`adb pull` can reach it. `Run-PerformanceSuite.ps1` sends the command at the start of each
capture, so the in-app record covers the same window as PresentMon or Perfetto.

A command window never replaces another measurement's window. A `begin` command is refused
while a Performance Suite or the in-game performance test is running, or while a window that
a command did not open is open, and the window itself is refused if one of those starts
before it opens; the app then writes `window.refused` where `window.done` would go, naming
the scenario and the reason. A `begin` accepted while an earlier command's window is
scheduled or open discards that window unsaved. `end` and `cancel` act only on a window a
command opened. Likewise, a suite or the in-game test does not start
while a measurement window is open or a command's window is scheduled; a suite whose run
window is refused saves that run excluded with the reason `measurement window refused`.

### Stutter tools

For a stutter felt during play or a replay, while the profiler records. The Developer button
of the game menu is shown in developer mode; its frame tools, Test Performance included, only
while the Frame Time Profiler setting is on.

| Tool | What it does |
|------|--------------|
| **F8** (Windows) | Marks the current frame, in play and in replays. With screen logging on, the screen log gets `MARK frame N` (or `MARK failed for frame N`) |
| **Game menu > Developer > Analyze Hitches** | A plain-text report of the last 30 s before the game menu was opened, shown in the viewer and written to `archive/recent_hitches.txt`: displayed FPS, hitch ratio and GC of the span; **Marked moments**, each mark with the hitches displayed from 5 s before to 0.5 s after it (the ten largest when there are more); the ten worst hitches, each with its cause, the draw and flush times of the tick that ended it, the request time and collections in the gap, and its content events; and the hitch causes of the span |
| **Game menu > Developer > Test Performance** | A 30 s measurement of the map as it is now, with a diagnosis naming the most likely cause and where it lies; see [In-game Test Performance](#in-game-test-performance) |

The report ends at the frame on screen when the game menu was opened, so on a touch device,
open the game menu right after a stutter and use Analyze Hitches.

A mark sets the `UserMark` flag on the frame's record and enters a ring of the last 32 marks.
Every saved run record (Dump Frame Log, a window command, a suite run) lists the marks in its
range as `marks`: frame id, UTC time, and milliseconds from the first saved tick. The frame
timeline CSV's `# OriginUtc` header line gives the wall-clock time of the tick its millisecond
columns count from, so a row can be matched with a screen recording or the screen log.
Offline, the `smoothness` report has a **Marked moments** section: for each mark (from
`marks`, or from the `UserMark` flags when the record has none), the hitches displayed from
5 s before to 0.5 s after it and the stage timeline of the nearest one.

## Metrics

A frame is *displayed* at the first vsync after it was ready; when two frames are ready for
the same vsync only the later one is shown, and the earlier one counts as dropped. Ready is
the flush end or, on Android, the completion of the HWUI frame that carried it (for a
GL-thread paint, the first HWUI frame whose sync began after the flush). In the app the
display time is always this estimate, and the in-app reports say so; their `Compositor`
cause and dropped frames are inferences. Measured display times come only from the offline
analyzer, which joins PresentMon or Perfetto display times to a record; every record says
which it is.

Let `R` be the measured refresh period, `T` the target content period (`1 / map FPS`), and,
for each displayed frame `j`, `g_j` the time since the previous displayed frame and `dc_j`
the main-counter advance since then.

| Metric | Definition |
|--------|------------|
| Displayed FPS | Displayed frames per second, pauses (menus, overlays, suspension, resizes) excluded |
| Hitch time ratio | Sum of `g_j - T` over frames with `g_j > H + R/2`, per second, where `H` is the longest on-time hold: `T` rounded up to whole refreshes (`max(T, ceil(T/R - 0.05) * R)`), since a frame can only change at a vsync. At 40 FPS on 60 Hz the two-refresh hold is on time and only three refreshes are a hitch; at a divisor rate `H = T`. Apple's bands: under 5 ms/s good, 5 to 10 warning, over 10 critical |
| Pacing error | `e_j = g_j - dc_j * T`: zero for perfectly even motion at any frame rate. RMS and P99 of its magnitude |
| Judder | Share of frames with `abs(e_j) > R/4`. A target rate that does not divide the refresh rate makes judder unavoidable: 40 FPS on 60 Hz alternates one and two refreshes, an error of exactly `R/2` on every frame |
| Dropped, coalesced, not run | Painted but never shown; invalidated but merged into a later paint; rendered but the paint returned early |
| Repeated refreshes | Refreshes a frame stayed on screen beyond its intended hold, per second |
| Latency | Display time minus the vsync of the tick that produced the frame, P50 and P99 |
| Assumed refresh mismatch | The pacing logic divided a refresh rate more than 5 % away from the measured one |
| On-screen pacing | The classic percentile set (P50 to Max, 1 % low, jank, hitch ratio, stutter index) over the displayed-frame gaps, judged against `T` rather than `R` |

**Pauses and long stalls.** A gap that contains a pause tick or a lifecycle break (the app
suspended or resumed: `GHFrameTimeline.NoteLifecycleBreak`, called from `GHApp.OnSleep` and
`GHApp.HandleResume`) is a pause. A gap in which the display callbacks stopped for 1 s or
more is a long stall. Inside a measurement window (a suite run, the in-game performance test,
a window command) a long stall counts as a hitch, and the report says
`Stalls over 1 s: N (X ms), counted as hitches`; in Dump Frame Log and Analyze Hitches it is
excluded from hitch time. A long gap in which the callbacks kept coming, such as a GL-thread
stall, is always a hitch. The record's `smoothness` object carries `longStallCount`,
`longStallMs` and `longStallsExcluded`.

**Metrics version.** `smoothness.metricsVersion` (4, `GHSmoothnessMetrics.MetricsVersion`)
names the version of these definitions; a record without it is version 1. Results of
different versions do not compare.

**FrameMetrics coverage.** `smoothness.compositorCoverage` is the share of GL-thread paints
carried by a FrameMetrics report, from 0 to 1, and `smoothness.syncOffsetP50Ms` the median
FrameMetrics sync offset; the suite manifest's run summaries carry both. Coverage applies
only on Android and is `null` elsewhere. On Android, FrameMetrics follow the window of the
top modal page, so a coverage near 0 across a suite means the listener is on the wrong
window and the `Compositor` attribution has nothing to go on. The recent hitches report, the
suite medians and the analyzer's hitch causes print it when it applies.

**Stale vsync correction.** On Android, Choreographer sometimes hands the frame callback a
`frameTimeNanos` one refresh older than the vsync HWUI records for the same frame, for whole
runs at a time. The analysis matches each tick to the FrameMetrics report of the same
`doFrame` (the latest intended vsync at or before the callback start) and, where the two
differ by one refresh (within 1 ms), uses HWUI's vsync; unmatched ticks take their neighbors'
state. The raw CSV keeps the original value. It needs FrameMetrics coverage;
`smoothness.vsyncCorrectedShare` is the share of ticks corrected, `null` elsewhere.

**Collections.** `smoothness.gcCount` is the number of generation 0 collections in the
window, and `smoothness.forcedGcCount`, beside it, the number of display callbacks that
followed a collection the app forced (an overlay hide, a level change, start-up, the suite's
page open, memory pressure). Such a callback carries `GHFrameFlags.ForcedCollection` (32) in
the frame timeline CSV's `Flags` column; several forced collections between two callbacks
count once. Both count from the window's second tick, since the first tick's collections
happened before the window.

The render loop's callback intervals, paint and lock durations, GC and allocation data are
still collected by `FrameTimeProfiler` and shown on the dashboard's FRAME section; they are
diagnostic series, not the measure of smoothness.

## Hitch attribution

Every hitch and every judder frame is charged to the **first** stage, in pipeline order,
that exceeded its budget:

| Order | Cause | Test |
|-------|-------|------|
| 1 | `DisplayMode` | The measured refresh period moved by more than 5 % across the gap or within the few ticks after it (the measurement is a running median, which lags a real change), or the pacing logic assumes a rate more than 5 % off the measured one |
| 2 | `PaintCpu` / `Gpu` | A late or missed callback while the UI thread was still painting the previous map frame, a buffer swap inside the callback included: `Gpu` when the flush or swap took longer than the draw |
| 2 | `UiThreadRequests` | A late or missed callback on a tick whose request handling (floating texts, messages, windows, ...) took more than `R/2`; with pause data it takes precedence over a collection in the same gap, and without it (Mono, older captures) the collection does, since its pause shows up as request time too |
| 2 | `UiThreadLateGc` | A missed callback, or a callback more than `R/2` after its vsync, and collections in the gap paused the process for at least `R/2` in total: long enough to explain the lateness. Without pause data (Mono, older captures), any collection in the gap counts, and the report says so |
| 2 | `FrameworkCadence` | A late or missed callback, not explained by the above, while the callback period ran at 1.5 refreshes or more around the gap: the UI framework delivered callbacks below the panel's rate for a while (Windows) |
| 2 | `UiThreadLate` | A late or missed callback that nothing above explains |
| 3 | `PacingPolicy` | A modulo skip, or a catch-up render that bypassed a skip pattern, in the gap (a catch-up render at or below the target rate renders exactly as a regular tick and is not a cause), or a refresh-to-target ratio the divisor pattern cannot pace evenly; either only when the gap is within the pattern's longest hold (two divisor steps) |
| 4 | `PaintNotRun` | A rendered tick in the gap produced no paint (coalesced, early return, no invalidation) |
| 5 | `DispatchLate` | A paint started more than `R/2` after its invalidation |
| 6 | `GameLock` | Map data lock wait over `T/4` |
| 7 | `PaintCpu` | Draw over `3T/4` |
| 8 | `Gpu` | Flush over `T/2`, a swap wait inside the callback over `T/2`, or a compositor frame's GPU time over `R` |
| 9 | `Compositor` | Measured on screen later than the vsync it was ready for, a painted frame in the gap never shown, or the compositor ran long |
| 10 | `Unattributed` | Nothing identified. Its share of hitch time is reported as a measure of the instrument itself |

Steps 5 to 8 are evaluated per frame painted in the gap, oldest first, and the first stage
over budget is the cause, since a frame that ran late can miss its vsync and be overwritten
by the next one. The compositor frame's GPU time over `R` is checked after them and before
the `Compositor` rules.

A cause's count is its hitches only: `hitchCount` per cause in the record, and in the
reports. Records from before metrics version 2 count judder frames too, and reports of them
say so.

On Windows, DWM gives the latest vblank rather than each callback's own vsync, so a
callback's lateness there is a phase within one refresh. It is not used to blame the UI
thread: only a missed callback counts as late, and `callbackLatenessP99Ms` is the vblank
phase.

FrameMetrics' `dropCountSinceLastInvocation` counts FrameMetrics reports the listener
missed, not frames the display dropped. It is data lost by the instrument and never a cause:
Analyze Hitches gives it as `FrameMetrics reports lost: N`, and the record as
`smoothness.compositorReportsLost`.

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
   scenarios, 120 seconds for gameplay; `Run-PerformanceSuite.ps1` picks it from
   `-ScenarioKind` unless `-WindowSeconds` is given.
3. **Repetitions**: at least 5 runs per configuration for a firm decision; 4 give a
   provisional one. Below 4 used runs in either arm `compare` makes no decision and prints
   the minimum detectable effect only. The report prints the minimum detectable effect at
   the observed spread; raise the count when it is too coarse for the question.
4. **Interleaving**: ABBA then BAAB, never AAAA BBBB, so that drift within the batch
   affects both arms equally. `Run-PerformanceSuite.ps1 -Pairs 2` produces exactly that.
5. **Cool-down** of 20 seconds between runs, extended until the device is back in the
   thermal class it started the batch in (up to 5 minutes). On Windows, which has no
   thermal status, the class is the processor performance counter compared with the batch
   start: more than 10 points lower is throttled. After the thermal gate a **quiet gate**
   waits until the last 5 seconds average other CPU under 10 % and disk busy under 50 %,
   for at most 120 s; a run started after a timeout carries the note
   `quiet gate timed out (other CPU N %)`.
6. **One variable per comparison.** A configuration is the full toggle vector; an A/B
   run varies exactly one entry.
7. **Windows reference runs** are done plugged in, on the High performance power plan,
   with the laptop on a hard surface, starting from a cold machine. Close other
   applications. A load sampler runs through every measurement window; a run it judges
   `busy` is excluded, and one it judges `elevated` is kept and annotated (see
   [Background load and environment drift](#background-load-and-environment-drift)).
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

- Percentiles are nearest-rank on the raw values. So are medians: every median shown in the
  reports and the suite list, like the one the decision uses, is the lower of the two
  middle values for an even count, never their average.
- **Run level decides.** Each run contributes one value per metric. The difference of arm
  medians gets a bootstrap 95 percent confidence interval by resampling runs; a
  difference counts only when the interval excludes zero **and** the point estimate is
  beyond the pre-registered threshold. Each arm is resampled from a sorted copy, so the
  order of the runs does not change the interval. For the smoothness series
  (`--series smoothness`): hitch time ratio worse by 2 ms/s, pacing error RMS worse by 1 ms,
  displayed FPS worse by 5 %. For the external series: P99 worse by 20 % or by one target
  period, hitch ratio worse by 2 ms/s, 1 % low FPS worse by 10 %. The Mann-Whitney p-value
  is reported, not decided on: at five runs per arm the smallest attainable p is 0.0079.
- **Run counts.** A verdict needs at least 4 used runs in each arm
  (`GHPerformanceComparison.MinRunsForVerdict`); with fewer there is no verdict. Below 5
  (`ProvisionalBelowRuns`) the verdict is marked provisional, and the minimum detectable
  effect is the more honest read.
- **Interval level describes.** All intervals of an arm pooled: Mann-Whitney U and Cliff's
  delta, the Hodges-Lehmann shift, and bootstrap intervals on the median and P99
  differences. Pooled intervals are not independent, so this level never decides.
- **No outlier removal.** A hitch is the phenomenon. Runs are excluded only for a recorded
  reason, the first that applies: a reason preset by the suite (`warm-up run`,
  `cold first run (warm-up run off)`, an abort), thermal throttling, a power state that
  changed during the run, a `busy` background verdict (reason `background load: ...`), or
  fewer than 100 frames (external series) or on-screen intervals (in-app records). A run
  whose capture failed produces no record; the batch script logs it as skipped and
  continues.
- **Elevated background load annotates, it does not exclude.** A run with an `elevated`
  verdict stays in the decision. A comparison with such runs adds a sensitivity line: the
  run-level decisions recomputed without them, and whether any of them changes. Read a
  decision that changes as fragile, and rerun under quiet conditions.
- **Windows throttling** is judged from the processor performance counter, which reads
  well under 100 on an idle machine under the Balanced plan: a run counts as throttled only
  when the reading taken after the run is under 90 % and more than 10 points below the one
  taken before it. The reason names the power plan.
- **Minimum detectable effect** is printed so that "no difference" reads as "no difference
  larger than X".

## Background load and environment drift

Two things move the numbers without any code change: other processes competing for the
machine during a run, and an environment that differs between runs (a Windows update, a GPU
driver, a NuGet package, a .NET SDK or runtime, an Android security patch, a setting). Both
measuring paths sample the machine's load during every window and record an environment
fingerprint, so that a run measured under load is flagged or excluded, and a difference is
attributed to what actually changed. The rules are shared code: `GHBackgroundLoad` and
`GHEnvironmentFingerprint` in `win/win32/xpl/GnollHackX/GnollHackX/Performance/`, compiled
into the app and the analyzer alike.

### Sampling

**In the app**, `GHSystemLoadSampler` reads the platform once a second on its own thread
into a ring of the last 900 samples. A suite holds it from start to end, so the quiet gate
and each window's 10 s pre-window have samples; a commanded window starts it when the window
opens, so its record has no pre-window; Dump Frame Log adds a block, without processes, only
while a suite or window holds the sampler. On Windows the per-process table comes from one
PDH collect just before the window and one after it, which PDH turns into each process's
average over the interval: the top 8 processes by CPU, plus up to 3 by GPU. The sampler is on
by default; the `PerformanceBackgroundSampler` preference, which has no setting in the UI,
switches it off, and the fingerprint records it as `settings.backgroundSampler`.

**`Run-PerformanceSuite.ps1`** samples each capture, warm-up included. On Windows a hidden
`typeperf` writes the whole-machine counters once a second to `load_system.csv`, and a
background job runs one `Get-Counter` sample as long as the capture over `\Process(*)` and
`\GPU Engine(*)`, written to `load_processes.csv` in `typeperf`'s CSV shape. (`typeperf`
cannot do this itself: it reads `-si` in mm:ss form as 1 s, and `-sc 2` averages the
interval after the window.) On Android an `adb shell` loop appends `/proc/stat`, the app's
`/proc/<pid>/stat` and `/proc/meminfo` to `load_android.txt` once a second, and falls back to
`top -b` when `/proc/stat` is not readable. `ingest --env-during-*` turns the files into an
`external` background block whose window starts at the first sample plus the warm-up.

| Signal | Windows, in-app | Android, in-app | iOS, in-app | Windows, script | Android, script |
|--------|-----------------|-----------------|-------------|-----------------|-----------------|
| Whole-machine CPU | `\Processor(_Total)\% Processor Time` | not readable by an app | not readable by an app | Same counter | `/proc/stat`, or `top` |
| Own CPU | Process times | `Process.ElapsedCpuTime` | Process CPU time | `\Process(<name>)\% Processor Time` | `/proc/<pid>/stat`; none from `top` |
| Disk busy | 100 - `\PhysicalDisk(_Total)\% Idle Time` | no | no | Same counter | no |
| Available memory | `GlobalMemoryStatusEx` | `ActivityManager.MemoryInfo` | The process's own headroom (`os_proc_available_memory`), in MB only | `\Memory\Available MBytes`; the percentage needs `hardware.memoryGB` | `MemAvailable` of `MemTotal` |
| Hard page reads | `\Memory\Pages Input/sec` | no | no | Same counter | no |
| Low-memory flag | no | `MemoryInfo.LowMemory` | no | no | no |
| Memory-pressure events | no | The app's memory warnings | The app's memory warnings | no | no |
| Per-process CPU | PDH, window average | no | no | `Get-Counter`, capture average | no |
| Other processes' 3D GPU | PDH, busiest adapter | no | no | Busiest adapter **including the app**: recorded with a note, never judged | no |

"Other CPU" is whole-machine CPU minus this process's own, as a percentage of total logical
capacity, never below 0. Without a whole-machine reading there is no other CPU: on Android
and iOS the in-app verdict is therefore `unknown` unless a memory rule fires, and on iOS,
whose memory reading is the app's own headroom and no percentage, only a memory warning can
fire one.

### Verdict

`GHBackgroundLoad.Classify` judges the window's summary, and on Windows its per-process
table, against pre-registered thresholds. Percentiles are nearest-rank over the 1 Hz
samples.

| Signal | Elevated (annotate) | Busy (exclude) |
|--------|---------------------|----------------|
| Other CPU, P90 over the window | >= 10 % | >= 25 % |
| Other CPU spikes | | >= 50 % in >= 10 % of the samples |
| Disk busy, P90 | >= 50 % | never: the app's own disk activity cannot be separated |
| Available physical memory, minimum | < 10 % of total | < 5 % of total |
| Hard page reads, P90 | >= 200 /s | >= 1000 /s |
| Other processes' 3D GPU, window average | >= 10 % | >= 30 % |
| A known activity (below), its processes' summed CPU | >= 2 % | covered by the CPU rules |
| Android `LowMemory` in any sample | | yes |
| Memory-pressure events during the window | >= 1 | |

- **`unknown`** when fewer than half the expected samples are present, or when there is no
  other-CPU signal and no memory rule (low memory, available memory, hard page reads,
  memory pressure) fires.
- Otherwise **`busy`** when any busy rule fires, else **`elevated`** when any elevated rule
  fires, else **`quiet`**.
- The **reason** of an `elevated` or `busy` run names that level's facts and up to two
  suspect processes, in at most 100 characters, for example
  `background load: other CPU P90 31 %; wsl-vm (vmmemWSL 22 %), build-tools (devenv 5 %)`.
  A busy run's reason is its exclusion reason.
- An ingested record can carry both an `in-app` and an `external` block. Its verdict is the
  worst one that is not `unknown`.

**Known activities** are matched on the process name, case-insensitively and exactly, after
an instance suffix `#N` and a trailing `.exe` are stripped; any other process is `other`:

| Category | Processes |
|----------|-----------|
| `windows-update` | `TiWorker`, `TrustedInstaller`, `MoUsoCoreWorker`, `usocoreworker`, `wuauclt`, `WaaSMedicAgent`, `SIHClient`, `msiexec` |
| `antivirus` | `MsMpEng`, `MpCmdRun`, `NisSrv`, `MpDefenderCoreService` |
| `indexer` | `SearchIndexer`, `SearchProtocolHost`, `SearchFilterHost` |
| `wsl-vm` | `vmmem`, `vmmemWSL`, `VmmemWSA`, `vmwp` |
| `build-tools` | `devenv`, `MSBuild`, `VBCSCompiler`, `cl`, `link`, `clang`, `lld-link`, `dotnet`, `ServiceHub.Host.dotnet.x64`, `ServiceHub.RoslynCodeAnalysisService` |
| `sync` | `OneDrive`, `Dropbox`, `GoogleDriveFS` |
| `telemetry` | `CompatTelRunner`, `DiagTrack` |
| `measurement` | `PresentMon`, `typeperf`, `powershell`, `pwsh`, `adb`: listed, never counted as an activity, never named as a suspect |
| `compositor` | `dwm`: listed, never counted as an activity, never named as a suspect, and its GPU use is subtracted from the other processes' GPU share before the GPU rules apply, since the compositor's load follows the frames the app itself presents |

### Quiet gate

Before each run, both paths wait until the last 5 seconds average other CPU under 10 % and
disk busy under 50 % (disk is ignored where it is not reported), polling once a second. On
timeout the run goes ahead and carries the note `quiet gate timed out (other CPU N %)`:
`suite.notes` in the in-app record, `notes` in the ingested one.

- **In the app** the gate runs before the first run and after the thermal gate before each
  later one, waits at most 30 s, and needs at least 3 samples with a CPU reading. Once it
  has timed out, or the user has skipped it, the later gates of the same suite do not
  wait: a machine that stayed busy once is taken to stay busy, as a development machine
  usually does, and each later run that starts while the system is not quiet carries the
  note `quiet gate skipped: the system stayed busy (other CPU N %)`. A skip gives the note
  `quiet gate skipped by the user (other CPU N %)`. The runs' background verdicts still
  flag or exclude a busy run. Without a whole-machine CPU reading (Android, iOS) or with
  the sampler off there is no gate.
- **`Run-PerformanceSuite.ps1`** gates after the thermal gate and before it launches the
  arm, sampling 5 s windows back to back for up to `-MaxQuietWaitSeconds` (default 120).
  When no CPU reading is possible the gate counts as quiet.

### Environment fingerprint

A fingerprint is a flat map of string keys `<category>.<name>` to string values: booleans
are `true`/`false`, numbers use the invariant culture. It is captured into every in-app run
record (`environment.fingerprint`), at the start and end of every suite (`suite.json`:
`environment.fingerprint` and `fingerprintAtEnd`), and by `Get-EnvironmentFingerprint.ps1`
at the start and end of every batch.

| Category | Keys | Notes |
|----------|------|-------|
| `meta` | `meta.fingerprintVersion` (`1`), `meta.capturedUtc` | Never diffed |
| `code` | `code.appVersion`, `code.gitCommit`, `code.buildConfiguration`, `code.portVersion`, `code.portBuild`, `code.renderSubscription`; Windows: `code.assemblyMvid` (the first 12 hex characters of the app assembly's module version id, which changes whenever the compiled code does) | In-app |
| `toolchain` | `toolchain.runtime`, `toolchain.framework`, `toolchain.compiler`, `toolchain.sdk` (the build's SDK, assembly metadata `GHBuildSdkVersion`), `toolchain.packaging`; script: `toolchain.dotnetSdk` (the host's `dotnet --version` in `GnollHackTests`) | |
| `component` | `component.<assembly name>` for every loaded assembly except those starting with `System`, `mscorlib`, `netstandard`, `Microsoft.CSharp`, `Microsoft.VisualBasic`, `Microsoft.Win32` or `GnollHack`; `component.native.skia`, `component.native.fmod`; Windows: `component.windowsAppSdk`, `component.winui` | In-app; e.g. `component.SkiaSharp`, `component.Microsoft.Maui.Controls` |
| `os` | `os.platform`, `os.version`, `os.build` (Windows `26200.6584`), `os.displayVersion`, `os.edition`, `os.pendingReboot`; Android: `os.securityPatch`, `os.fingerprint`; script, Windows: `os.latestHotfix` | |
| `driver` | `driver.gpu<N>.version`, `driver.gpu<N>.date` (Windows, `<N>` 0-based in WMI order); script, Android: `driver.gles` | |
| `hardware` | `hardware.deviceModel`, `hardware.cpu`, `hardware.logicalProcessors`, `hardware.memoryGB`, `hardware.gpu<N>`; Android: `hardware.soc`; Windows, in-app: `hardware.renderAdapter` | |
| `settings` | `settings.<toggle>` for every entry of the record's `environment.configuration`; `settings.mapRefreshRate`, `settings.gpuBackend`, `settings.gpuCacheSize`, `settings.mainCanvasUsesGpu`, `settings.refreshHz`, `settings.backgroundSampler`; Windows: `settings.powerPlan`, `settings.powerMode`; Windows, in-app: `settings.gpuPreference` | |

`hardware.gpu<N>` lists the installed adapters; two Windows keys record which one the app
uses:

- `settings.gpuPreference`: the Windows graphics preference for the app's executable, from
  `HKCU\Software\Microsoft\DirectX\UserGpuPreferences`: `Auto`, `Integrated`, `Dedicated`,
  `Not set` or `Not found`.
- `hardware.renderAdapter`: the adapter the app renders on, `<name> (discrete)`,
  `<name> (integrated)` or `<name> (software)`, measured by the render adapter probe during
  the process's first measurement window. It is absent before that window ends, and when
  the app renders on the CPU (`settings.mainCanvasUsesGpu` records that case). The adapter
  cannot change within a process, so the value stays current.

`code.renderSubscription` names what drives the render loop: `raw` or `managed` on Windows,
for the raw-ABI and the CsWinRT subscription to `CompositionTarget.Rendering`, `none` before
the loop starts, and `platform` elsewhere. It is `raw` only in a build that defines
`ENABLE_RAW_RENDERING`, which no project defines by default. Such a build still falls back
from `raw` to `managed` at run time when the raw subscription cannot be made or delivers no
frames, so the value can differ between two runs of one build.

A suite's starting fingerprint is taken when its first window ends, warm-up or run 1; the
one written when the suite is created is provisional until then. By that point the game
page has painted, which sets `settings.gpuBackend` and `settings.gpuCacheSize`, and the
first window has measured `hardware.renderAdapter`. A change between the suite's creation
and the end of its first window, which only opens the page and warms up, is therefore not
reported as a change during the suite.

Offline fingerprints from `Get-EnvironmentFingerprint.ps1` carry neither key: the script
does not know the app's executable path, which the preference lookup needs, and cannot
probe the app's rendering. This is safe, because fingerprints from different sources are
compared on the keys both sides recorded only (see below).

A record or manifest written before fingerprints existed gets one mapped from its per-field
environment (`appVersion` to `code.appVersion`, `skiaSharpVersion` to
`component.SkiaSharp`, `deviceModel` to `hardware.deviceModel`, each toggle to
`settings.<key>`, and so on), without `meta.fingerprintVersion`. New captures write the same
keys for those values, so old and new records diff meaningfully. `ingest` takes the in-app
record's fingerprint first, then fills the keys it lacks from `--fingerprint` files, then
from the record's own fields, and marks the result `meta.source`: `in-app` when an in-app
record took part, else `script`. `Get-EnvironmentFingerprint.ps1` writes `os.version`,
`hardware.cpu`, `hardware.memoryGB` and `hardware.deviceModel` in the app's formats, but the
two sources still record different key sets, so when `compare` or `drift` meets fingerprints
from different sources it compares only the keys both sides recorded and says so.

**Diff.** Every key but `meta.*` is compared, and each difference is one of:

| Kind | When | Counts as a change |
|------|------|--------------------|
| Changed | Present on both sides with different values | yes |
| Added, Removed | Present on one side only, and both sides carry `meta.fingerprintVersion` | yes |
| NotCompared | A `component.*` key present on one side only: assemblies load lazily, so an assembly that was not loaded yet at the capture is not a change. Reports give only their count | no |
| Unknown | Present on one side only, and at least one side is a legacy record, which never recorded the key; reported as `(not recorded)` | no |

**Attribution** is drawn from the changes, with categories listed in the order code,
toolchain, component, os, driver, hardware, settings:

| Label | Meaning |
|-------|---------|
| `none` | Nothing counted differs |
| `code` | Only `code.*` keys differ: the comparison measures the code change |
| `environment: <categories>` | No `code.*` key differs: the difference belongs to the environment |
| `confounded: <categories>` | Code and something else both differ: the effect cannot be assigned to either |

More than one changed `settings.*` key adds the warning `more than one setting differs`
(protocol rule 6). When several runs or suites are pooled into an arm, a key whose value
differs within the arm reads `mixed`, and a key missing from any of them is left out.

### What the reports show

- **In-app suite report** (`report.txt`): an **Environment** section, one line per category
  with a 12-character hash of its keys and their count, followed by the changed keys when
  the environment changed during the suite (`environmentChanged` in `suite.json`: a change
  outside `meta` and `code`); a **Bg** column in the run table, the verdict's letter (`q`
  quiet, `e` elevated, `B` busy) and the other-CPU P90, or `?`; a `note:` line under a run
  with a note; a **Background load** section with the verdict counts and the suspects the
  elevated and busy runs name most often; and **Changes since the previous comparable
  suite**, the most recent earlier suite with the same comparability key on the same device,
  its median hitch ratio then and now, and the fingerprint changes since then. That last
  section is descriptive only; the baseline comparison decides.
- **In-app comparison** (Compare with Baseline, `comparison.txt`): **Environment
  differences (A -> B)** between the arms' pooled fingerprints, the **Attribution** line, a
  warning when it is confounded or more than one setting differs, and, when any used run is
  elevated, the sensitivity line (`Without the N elevated runs: decision unchanged`, or the
  metrics whose verdict changes). A warning names the `code.appVersion`, `code.gitCommit`
  or `code.assemblyMvid` that differs when one arm pools suites of more than one build. The
  list of suites marks a suite `bg` when a run was excluded for background load, and
  `env changed` when its environment changed during it.
- **`compare`**: a `bg` column in both run tables (`e 12`: the verdict letter and the
  other-CPU P90), an **Environment differences** section with the common fingerprint of each
  arm's used runs and the attribution, and a **Sensitivity** line under the run-level
  decision. With `--include-excluded`, busy runs are left out of the sensitivity
  recomputation too.
- **`history --list`** shows each run's batch and background verdict.
- **`smoothness`**: in-app background samples add `background` events (other CPU rising by
  15 points or more between samples, disk busy of 80 % or more) near the change-point
  boundaries.

### Reading a drift report

`drift` looks for steps over time in `history.jsonl` and attributes each one:

```powershell
& $a drift --file DEVEL\performance\history.jsonl --scenario W1 --platform Windows --out drift.md
```

- **Lines.** Runs are grouped into lines of like-for-like measurements: platform, device
  model, scenario, scenario kind, series, refresh and target periods (to 0.1 ms), and the
  hash of the `settings` category. A changed setting therefore starts a new line rather
  than showing up as a step; this includes `settings.gpuPreference`, so switching the GPU
  starts a new line. `--series` picks one series; without it every run joins one
  line per series it carries (external, smoothness).
- **Batches.** Within a line, runs are grouped by their `batch` (the leaf name of the
  batch's `-OutDir`, or the suite id of an in-app record), or by date, arm and commit for
  older records. A batch with at least 4 used runs, the minimum a verdict needs
  (`MinRunsForVerdict`), is compared with the previous batch and, when that shows no
  shift, with the pooled previous up to 3 batches, which catches slow drift; the pool
  never reaches back across a detected shift. A shift is a metric whose
  bootstrap interval excludes zero and whose difference exceeds `compare`'s pre-registered
  threshold (see [Statistics](#statistics)).
- **The table**, one per line: each batch's used and total runs, the metric medians,
  **Delta** (the shifted metrics, signed, with their verdict), **bg** (the counts of `q`,
  `e`, `B` and `?` runs and the mean other-CPU P90) and **Attribution**.
- **Shifts** lists each step with the medians before and after, the interval, the
  attribution and the changed keys. The attribution is the fingerprint label of the two
  sides' common fingerprints; `none` becomes `background` when at least half of the later
  batch's runs were elevated or busy, and `unexplained -- rerun under quiet conditions`
  otherwise. A different number of throttled runs or a different power state is appended as
  context.

For example, a hitch-ratio step labeled `environment: driver`, listing
`driver.gpu0.version` with its old and new values, says that the GPU driver changed between
the batches and the code did not: rerun the old build on the new driver before blaming or
crediting a commit. `confounded: code, component` means a commit and a package upgrade
arrived together, and only a run that separates them can say which one moved the numbers.

### Known limitations

- **Short-lived processes** (single `cl.exe` compilations, for example) are missing from the
  per-process tables, which list processes alive at both ends of the interval. Their CPU
  still counts in the 1 Hz whole-machine other CPU.
- **Measurement tools count as other CPU** in the script's samples: PresentMon, `typeperf`,
  the `Get-Counter` job's PowerShell and `adb` run beside the app. The `measurement`
  category keeps them from being blamed, but not out of the other-CPU figures.
- **Capacity dilution.** Other CPU is a share of all logical processors, so on a machine
  with many threads one busy core reads as a few percent, below the elevated threshold,
  while it can still compete with the UI or render thread.
- **Android `/proc/stat`** is readable from `adb shell` only where the vendor's SELinux
  policy allows it. The `top` fallback gives whole-machine CPU but not the app's own, so the
  external other CPU, and with it the verdict, stays `unknown`.
- **iOS** reports neither whole-machine CPU nor a system memory percentage, so its verdict
  is `unknown` unless a memory warning arrives during the window.
- **The script's per-process averages** cover the whole capture, warm-up included, and
  start only once the background job is up, so they overhang the capture's end by about
  the job's start-up time.

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
panels. The unit tests cover 60 on 90 Hz, 80 on 120 Hz, 60 on 165 Hz and a 120 to 60 Hz drop
whose measured period lags the change, with synthetic timelines; variable-period (LTPO,
ProMotion) timelines are not covered.

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

## In-game Test Performance

**Game menu > Developer > Test Performance** measures the live game as it is on screen and
writes a plain-text diagnosis naming the most likely cause of poor smoothness and where it
lies. `GHPerformanceDiagnosticRunner` runs the test and `GHPerformanceDiagnosis` judges it.

After a confirmation the menu closes and a countdown is drawn at the top of the map. The
test settles for 3 s, so that the menu's pause and collection fall outside the window, waits
at most 5 s for the background sampler's per-process begin collect, records the scene, and
measures the map for one 30 s window. The debug dashboard and the frame marker are hidden
during the settle and the window. Do not touch the game until the report opens. The
window is saved as a run record in `<GHPath>/performance/diagnostics/<stamp>/`, and the
report is written to `diagnostics/perftest_<stamp>.txt` (`<stamp>` is the local time as
`yyyyMMdd_HHmmss`) and opened in the viewer. The newest 30 reports are kept: after every
test, aborted or not, older reports and every `<stamp>` folder without a kept report are
deleted, except the folder of the test that just finished, which is kept even when its
report could not be written. Reset > Delete Performance Data deletes them all.

The game menu offers the test only while the Frame Time Profiler setting is on. Should the
profiler still be off when the test starts, the test turns it on after the confirmation,
before the settle, and turns it off again when it ends, whether it completed, was cancelled
or failed. It refuses to start when the platform render loop is off, the game has ended, or
a test, a suite, a measurement window or a window command is already running; it checks
again after the confirmation. It stops when the app goes to the background, a page opens
over the game page, or the game ends, and, once the window is open, when a menu or window
opens over the map. An aborted window is discarded unsaved and
no report is written. The final message, a cancellation or an error, appears on whichever
page is on top once the test has ended, so a second test can start at once.

**Health** is judged against the effective target, the lower of the map FPS target and the
measured refresh rate:

| Class | Rule |
|-------|------|
| Healthy | Displayed FPS at least 95 % of the effective target and hitch ratio under 5 ms/s |
| Poor | Displayed FPS under 75 % of the effective target, or hitch ratio over 10 ms/s |
| Degraded | Otherwise |
| Inconclusive | The first that applies: the platform render loop is off; the measurement is excluded (the power state changed, or the window could not be saved); a Debug build or an attached debugger; the map was covered or paused for more than 20 % of the window; fewer than 100 on-screen intervals; no displayed frame rate, target rate or hitch ratio |

The run record's throttling and busy-background exclusions do not make the test
inconclusive; they become findings. A cause's share of hitch time counts only when the
summed cause time is at least 5 ms/s, so the few hitches of a healthy run name no cause.

| Area | Findings (`likely`, `suspect` or `info`) |
|------|----------|
| Heat | `THERMAL_THROTTLED` likely when the thermal status shows throttling (above Moderate, or a rise of two classes), suspect at Moderate; otherwise `THERMAL_RISING` suspect at a thermal headroom of 0.85 or more, info for a one-class rise. `CPU_CLOCK_CAPPED` suspect when the processor performance counter fell by more than 10 points to under 90 % (or reads under 90 % with no earlier reading), otherwise info under 60 % |
| Power | `LOW_POWER_MODE` likely; `POWER_EFFICIENCY_MODE` (Best power efficiency or the Power saver plan) suspect; `ON_BATTERY` info, suspect with power saving on |
| Background | `BACKGROUND_BUSY` likely, `BACKGROUND_ELEVATED` suspect (see [Verdict](#verdict)); a known activity, `BG_<CATEGORY>`, from 2 % CPU: info unless the verdict is elevated or busy or the activity uses at least 10 % CPU; `BG_OTHER_PROCESS` suspect from 10 % CPU; `OTHER_GPU_LOAD`, compositor excluded, suspect from 10 %, likely from 30 % |
| Memory | `LOW_MEMORY` suspect at the elevated levels, likely at the busy ones or a low-memory report |
| GPU | `CPU_RENDERING` likely when degraded, poor or 30 % `PaintCpu`, else info; `GPU_CONTEXT_MISSING`, `SOFTWARE_ADAPTER`, `WRONG_GPU` likely; `WRONG_GPU_POSSIBLE` suspect; `GPU_BOUND` from 30 % `Gpu` |
| Display | `COMPOSITOR` from 30 % `Compositor` (below); `REFRESH_BELOW_MAX` (the refresh at most 90 % of the display's maximum while it caps the rate) and `PACING_MISMATCH` suspect |
| Settings | `MAP_FPS_CAP` info (target under 90 % of the refresh rate); `SETTINGS_NONDEFAULT` suspect (a performance feature off against its default) |
| Game | `GC_PRESSURE` from 20 % `UiThreadLateGc`, or suspect without a score from 50 MB/s allocation over the window; `UI_THREAD_BUSY` from 30 % `UiThreadLate` + `UiThreadRequests`; `PAINT_HEAVY` from 30 % `PaintCpu` with the GPU on |
| Instrument | `DEBUG_OVERHEAD` info, suspect with a debugger attached; `PLAYER_INPUT` (below); `SAMPLER_OFF` info |

The stage findings, `GPU_BOUND`, `GC_PRESSURE` by share, `UI_THREAD_BUSY` and `PAINT_HEAVY`,
are suspect below a 50 % share and likely from it, and carry the share as their score.
`COMPOSITOR` is suspect only on measured display times, which the app never has (see
[Metrics](#metrics)); on the vsync estimate it is info and marked inferred. `PLAYER_INPUT`
is suspect with at least 3 hitches of which the gaps with content events hold at least
half, and info whenever content changed.

**Ranking.** Findings are sorted by severity first. Within a severity come, in order: heat
and power findings and any likely finding without a score, which are causes outside the
pipeline and explain any share; the findings scored by their share of hitch time, highest
first; then the rest. When the health is Degraded or Poor, the primary cause is the first
suspect-or-stronger finding outside the Instrument area, so `PLAYER_INPUT` never is. The
**Location** line gives its side: `external` (heat, power, background, memory),
`configuration` (GPU, display, settings) or `inside the game`. Without a primary cause, a
run with at least 30 % of its hitch time in `Compositor` concludes `unclear`, since that
share is inferred, and any other `inside the game`, by elimination. A healthy run has the
location `none`, an inconclusive one `unclear`. The **Also:** line under the primary cause
names the other side: for a scored primary, the first suspect-or-stronger heat, power,
background or memory finding; for any other primary, the first scored finding.

**The report**, format version 2, gives in order: a Summary in plain words (whether the
game ran smoothly, the most likely cause and what to try, or why the test could not judge),
the Result line (health, displayed and target FPS, hitch ratio), Location, the most likely cause, the other suspect and likely
findings, a Checks row per area (`ok`, `warn`, `BAD` or `n/a`), Measurements, Hitch causes,
Notes (the info findings), Environment (the fingerprint without `meta` and most `component`
keys), Facts, and Frame detail, the Analyze Hitches report of the 30 s ending at the
window's last frame. The **Scene** line gives the level, the zoom mode (`normal`,
`alternate` or `minimap`) and the map font size just before the window; the **Content** line
gives the hitch rates of the quiet gaps and of the gaps with content events (see
[Content events](#content-events)). **Facts** is one `key=value` line per fact, `n/a` when
unknown: `formatVersion` first, `metricsVersion` (see [Metrics](#metrics)), `frameMarker`
(`off` when the marker was hidden during the window, as this test does), the `scene.*` keys, and last the health, location, primary and finding
codes. Reports of different metrics versions do not compare.

## In-app Performance Suite

The suite measures a build on the device itself, with no PC: a series of measurement windows
over one recorded replay, each started from the same turn, saved as ordinary run records and
compared on the device. **About > Performance Suite** opens it; the button is shown only with
developer mode on and **Settings > Frame Time Profiler** on. The runner switches the profiler
and the debug dashboard on for the suite, whatever the settings say, and afterward returns the
profiler to its setting and ends the dashboard override.

The view is fixed for every run: the map is at the device's default zoom (the default map font
size), with alternative zoom off, auto-center on, no overlays and the dashboard expanded. The
zoom and auto-center stones, pinch, the mouse wheel and drag panning are disabled, and the
player's saved zoom is left untouched. The C core's own zoom save and restore around
detection still apply.

### Setup

The page remembers the last setup.

| Field | Default | Meaning |
|-------|---------|---------|
| Replay | none | A main replay file in `<GHPath>/replay`, newest first; continuation files are not listed |
| Start Turn | 1 | The turn every run starts from, at least 1 |
| Scenario | Idle | **Idle**: the replay pauses at the start turn, leaving only tile animations (W-idle). **Minimap**: the replay pauses and the map switches to minimap zoom at the default minimap size and position (W1). **Playback**: the replay keeps playing (replay) |
| Measured runs | 6 | 4, 5, 6 or 8, not counting the warm-up run |
| Warm-up run | On | Run 0, a full run that is saved but excluded as `warm-up run`. With it off, run 1 is excluded instead as `cold first run (warm-up run off)`, and the page asks before starting when that leaves fewer used runs than a verdict needs |
| Page mode | Shared page | **Shared page**: one game page for every run. **Fresh page per run**: a new game page for each run |
| Arm label | Windows: `<version> <commit7> m<id6>`; elsewhere `<version> <commit7>` | Names the arm; suites with the same label pool into one arm. `<commit7>` is the first 7 characters of the commit, and `<id6>` the first 6 of the app assembly's module version id (`code.assemblyMvid`), so on Windows any code change gives a new default. An edited label is kept until the default changes |
| Warm-up, window and cool-down seconds | 10, 60, 20 | Per run, as in the Protocol |
| Thermal gate | Light or better | What each run after the first waits for where the platform reports a thermal status: **Light or better**, a fixed limit, the same for every run of every suite; **Same as start**, no worse than the suite-start reading, the only behavior before this setting existed |
| Thermal wait seconds | 300 | The longest each thermal wait lasts, the one before the first run included; 0 turns the thermal waits off |

The estimated duration is (measured runs + warm-up run) x (15 s for the seek + warm-up +
window + cool-down), the cool-downs being the one before the first run and those between
runs; thermal and quiet gate waits are not included. With the defaults it is about 12
minutes.

### What a suite does

1. Before the first run, the warm-up run included, and before any game page opens, the suite
   waits one cool-down and then, where the platform reports a thermal status, until the
   status is Light or better, checking every 15 s for at most the thermal wait seconds. The
   suite's starting thermal reading, which the later Same as start gates compare with, is
   taken after that wait.
   The quiet gate follows (step 5). While no game page is open, a popup on the suite page
   shows what the suite is waiting for, with a progress bar: the cool-down's seconds left,
   the thermal status against its limit, or the other CPU against the quiet threshold.
   **Skip** ends the current cool-down, thermal or quiet wait; **Cancel** ends the suite as
   an abort with the reason `cancelled by the user`, keeping its finished runs. The Run
   Suite button is grayed out while a suite runs.
2. The first game page plays the replay from its beginning, exactly as the Replay page starts
   it, when the start turn is 1 or less; otherwise it seeks to start turn - 1 and plays the
   last turn at normal speed.
3. The runner waits, for at most 120 s, until the start turn is reached and no replayed menu,
   text window, prompt or popup covers the map, and applies the scenario. The replay header
   shows the progress as `Run N of M: <phase>` (`Warm-up run: <phase>` for the warm-up run),
   the phase being `warming up 10 s`, `measuring 60 s`, `cooling down 20 s` or, during a
   thermal gate, `cooling Moderate > Light 45/300 s`, updated every second. The replay's real
   time is hidden while the suite shows its progress.
4. Warm-up, then the measurement window, saved with the suite's context. The debug dashboard
   and the frame marker are hidden from the start of each warm-up until the window is saved.
5. Cool-down, then a thermal gate: the next run waits until the thermal class is Light or
   better, or no worse than at the suite start, as the Thermal gate setting says, checking every
   15 s, and goes ahead after the thermal wait seconds regardless, carrying the note
   `thermal gate timed out after N s (<status> vs <limit>)`. On the game page there is no Skip;
   Quit ends the suite. Windows
   reports no thermal class, so there is no gate there: the processor performance counter
   mostly follows turbo boost, which drops whenever the replay pauses. A run measured while
   throttled is still excluded by the per-run rule. Then the quiet gate, which also runs
   before the first run: the next run waits, for at most 30 s, until the last 5 seconds
   average other CPU under 10 % and disk busy under 50 %, and the replay header shows
   `waiting for a quiet system (other CPU N %)`. After a timeout the run carries the note
   `quiet gate timed out (other CPU N %)`, and the later quiet gates of the suite do not
   wait (see [Quiet gate](#quiet-gate)). There is no quiet gate on Android and iOS, which
   give an app no whole-machine CPU reading.
6. The next run starts. With a shared page the replay, paused during the cool-down, seeks back
   to start turn - 1, which restarts it in place and collects garbage. With a fresh page the
   game page closes before the cool-down, which then runs on the suite page, and after a
   collection a new game page starts as in step 2.

The suite aborts when the replay ends, the user stops it, the app goes to the background
(minimizing counts on Windows), or the start turn is not reached; between fresh pages, also
when a game or another page is opened. Finished runs are kept, a window open at that moment is
saved excluded with the abort reason, and the suite's status becomes `aborted` with the
reason. Measured runs are otherwise excluded by the rules under [Statistics](#statistics).

### Recording a benchmark replay

- The start turn must leave at least warm-up plus window seconds of replayed content after
  it; early turns seek faster.
- **Minimap**: record in wizard mode and reveal the level with Ctrl+F before the start turn,
  so that the minimap draws the whole level.
- **Playback**: start at a stretch with fighting and spell or screen-filter effects.

### Results, comparison and baselines

Each suite is a folder `<GHPath>/performance/suites/<suiteId>/`, where `<suiteId>` is
`yyyyMMdd_HHmmss_<scenario>_<device model>` in UTC. It holds `suite.json`
(`schema/suite-manifest.schema.json`), each run's `run_*.json`, `frametimeline_*.csv` and
`compositorframes_*.csv`, and `report.txt`. `<GHPath>/performance/baselines.json` records
one baseline arm label per comparability key and device. Unlike `archive`, the
`performance` directory is not cleared when the app starts. Reset > Delete Performance Data
deletes it entirely, together with the archive's performance files and, on Android, the
external `performance` export directory.

The results list shows each suite's date, scenario, label, used runs, median hitch ratio and
size, tagged `baseline`, `aborted`, `imported`, `bg` (a run was excluded for background
load), `env changed` (the environment fingerprint changed during the suite), `invalid id`
(the `suiteId` in its `suite.json` differs from its folder name) or `unreadable` (its
`suite.json` cannot be read); an `invalid id` or `unreadable` suite can only be deleted.
Tapping a suite selects it and tapping it again deselects it. The buttons sit above the list,
which does not scroll by itself: the page does. A used run is a measured run with a summary
and no exclusion reason. A suite's id
is its folder name, and a folder whose name is not letters, digits, `_` and `-` is not
listed.

| Button | Action |
|--------|--------|
| View Report | `report.txt`: setup, replay size, SHA-256 and start turn, environment and its fingerprint by category, the run table with each run's background verdict, the background load, the medians of the used runs, the changes since the previous comparable suite, hitch causes and content events |
| Set as Baseline | Records the suite's arm label as the baseline for its comparability key on this device |
| Compare with Baseline | Compares the suite's arm with the baseline arm of its key |
| Share, Import Results | See [Share and Import](#share-and-import) |
| Delete | Deletes the selected suites' folders; `baselines.json` is left as it is |

The **comparability key** is the scenario, the replay's SHA-256, the start turn, the page
mode, the map FPS setting, the measured refresh rate rounded to whole hertz and the metrics
version, as `|m<metricsVersion>` at its end, fixed when the suite finishes. A suite measured
with an earlier metrics version therefore matches no baseline, previous comparable suite or
suite measured later; after the version changes, baselines must be measured again. The key
then ends with `|tg=<thermal gate>,<thermal wait seconds>`, e.g. `|tg=light,300`, unless the
suite used the `start` gate with 300 s: every suite recorded before the thermal settings
existed ran that way, so those suites stay comparable with each other and with new ones set
alike, while suites with other thermal settings pool and compare only among themselves. The
suite's measured refresh rate and target FPS, set when it finishes, are the medians over
its used runs only.

Set as Baseline records the arm label in `baselines.json` under the comparability key and
this device: `os.platform`, `hardware.deviceModel`, and `hardware.cpu` or, without it,
`hardware.soc`. A baseline set by an older build, which recorded no device, applies to any
device that has no baseline of its own for the key.

A comparison pools, per arm, every suite with the same key and the same arm label that was
measured on the selected suite's device: arm A the baseline label, arm B the selected
suite's. Suites with another key never enter it. The device matches when every
`hardware.*` key and `os.platform` present in both fingerprints agree, or, for suites
without a fingerprint, when platform and device model agree. Compare with Baseline refuses
with one of these messages:

| Message | When |
|---------|------|
| `This suite could not be read.` | Its `suite.json` cannot be read, or has no setup |
| `This suite has not finished running.` | The suite is still running |
| `This suite was interrupted and cannot be compared.` | The app stopped during the suite, leaving it marked running |
| `This suite has no comparability key yet.` | The suite has not been finished |
| `baselines.json cannot be read, so no baseline is known. Set a baseline to start a new one.` | Neither `baselines.json` nor its backup can be read |
| `No baseline for this group on this device; a baseline exists for the same replay at <Hz> Hz (this suite: <Hz> Hz).` | No baseline for the key on this device, but one for a key that differs only in the refresh rate |
| `No baseline for this group on this device; a baseline exists for the same replay with thermal gate <gate>, wait <N> s (this suite: <gate>, wait <N> s).` | No baseline for the key on this device, but one for a key that differs only in the thermal settings; joined after the refresh-rate sentence with `; ` when both apply |
| `No baseline is set for this suite's comparability group on this device.` | No baseline for the key on this device |
| `This suite belongs to the baseline arm.` | The suite's own label is the baseline label |
| `Baseline measured on a different device (<device> vs <device>).` | The baseline arm has no used runs on this device but has suites on another |
| `One of the two arms has no used runs.` | Either arm has no used runs |
| `The comparison could not be computed.` | The comparison failed |

The decision is the one `compare --series smoothness` makes offline, from the same code
(`GHPerformanceComparison`), as described under [Statistics](#statistics). With fewer than 4
used runs in either arm there is no verdict; below 5 the verdict is marked provisional. The
report lists both arms' versions, a warning when one arm pools suites from different builds
(a different `code.appVersion`, `code.gitCommit` or `code.assemblyMvid`), their
environment differences and the attribution, the suites in start order, the verdict per
metric, the sensitivity line when a used run was elevated, and each arm's hitch causes (see
[What the reports show](#what-the-reports-show)).

The suites are listed with their UTC start, arm and used runs, followed by an `Order:` line
such as `Order: A B B A`. The report warns when every suite of one arm ran before every
suite of the other, since drift is then confounded with the change, and when an arm has a
single suite.

For the Playback scenario the report also warns when the arms covered different amounts of
the replay in their windows, since a slower build plays less in the same time. Coverage is
measured by the input records played in the window, else by the turns in the window, else
by the turn reached, whichever every used run of both arms has; it differs when the arms'
medians differ by more than 1 and by more than 10 % of the larger one, or their ranges are
more than 1 apart (a window's ends can each shift a count by one). The run record's `suite`
object carries `turnAtWindowStart` and `inputRecordsInWindow` for this.

The store's files are written as follows:

- `suite.json`, `report.txt`, `baselines.json` and each run's JSON are written whole: to a
  temp file beside the target, then moved over it in one step, so a crash never leaves a
  partly written file. A temp file older than an hour, left by a killed app, is deleted the
  first time the suite list is read.
- Each write of `baselines.json` first copies the previous file to `baselines.json.bak`, but
  only when that file could be read. When `baselines.json` cannot be read and the backup
  can, the backup is used. When neither can be read, Set as Baseline refuses and offers to
  start a new file; the unreadable one is kept as
  `baselines.json.corrupt-<yyyyMMddTHHmmssZ>`. Compare with Baseline says when
  `baselines.json` cannot be read.
- The store's lock is per process. Two app instances sharing one store (possible on
  Windows) can each change `baselines.json`, and the later write wins; the atomic writes
  prevent a torn file, not a lost update.

### Two builds on one device

- **Windows**: unpackaged builds share one store. Run the suites in the order A B B A: the
  old build, the new build, the new build again, and the old build again, with the same
  label for both suites of a build and at least 4 runs each. Set an old-build suite as the
  baseline and compare a new-build one's. The default arm labels differ, since they carry
  the version, the commit and the assembly id. The comparison report lists the suites in
  start order with an `Order:` line, and warns when every suite of one arm ran before every
  suite of the other, or when an arm has a single suite.
- **Android and iOS**: a reinstall may wipe the app's data, and the store with it. Share the
  suites before installing another build and import them afterwards. Interleave in blocks:
  a suite of 4 runs with the old build, install the new build, two suites of 4 runs,
  reinstall the old build, a suite of 4 runs. The suites of each build carry the same label
  and pool into one 8-run arm, so drift over the session affects both arms alike, and the
  report's order is A B B A.

### Share and Import

**Share** zips the selected suites into
`archive/GnollHack_Performance_<device model>_<timestamp>.zip`, each suite's folder with its
`report.txt` rewritten and, when the suite compares with its baseline, a `comparison.txt`,
and hands the zip to the system share sheet. **Import Results** picks a zip and checks every
top-level folder before extracting anything: a `suite.json` with a `manifestVersion` and a
`suiteId` equal to the folder name, of letters, digits, `_` and `-` only, and every run file
it names present with schema version 2. Valid folders move into the store,
tagged `imported`; a suite whose id is already present is skipped, never overwritten; the
others are reported with the reason. Share leaves out temp, backup and set-aside corrupt
files (names ending in `.tmp` or `.bak`, or containing `.corrupt-`), and Import does not
extract such entries.

On a PC, unzip the archive and run the analyzer on the suite folders (`$a` as in
[Analyzer](#analyzer)). See the status note under [Running a batch](#running-a-batch) before
relying on the result.

```powershell
& $a smoothness <suite>\run_<...>.json --out smoothness.md
& $a compare --a <old suite 1> --a <old suite 2> --b <new suite> --label-a Old --label-b New --series smoothness --out report_smoothness.md
```

`smoothness` takes one run record at a time. `compare` reads every `*.json` of a folder and
ignores `suite.json`, which is not a run record. The warm-up run and a window cut short by an
abort stay out through the `excludedReason` of the record's `suite` object, and the other runs
go through the analyzer's own exclusion rules (`--include-excluded` brings excluded runs in).
`compare` does not check the comparability key; it refuses runs whose refresh or target
period differ (protocol rule 10).

## Running a batch

**Status.** The batch scripts and the analyzer's `ingest`, `compare`, `history` and `drift`
commands have not been validated end to end on real captures. Known defects:

- `drift` pools the two arms of one batch into a single line.
- The join with PresentMon or Perfetto display times improves the `smoothness` report only.
  `compare`, `history` and `drift` decide on the app's estimated display times.
- The batch script builds the analyzer only when its executable is missing, so a stale
  analyzer is used without a warning.
- Without in-app records and without `-TargetFps`, the target rate is taken to be the
  refresh rate.
- On Android, frames painted on the GL thread are not joined to Perfetto.
- The analyzer does not know a run's metrics version, and compares runs of different
  versions without a warning.

Use the in-app Performance Suite for decisions until these are fixed.

Windows, Build 14 versus HEAD, with in-app records joined to PresentMon (elevated console;
PresentMon 2.x). The profiler must be on in both builds so that they accept the window
command. Without in-app records, pass `-BuildConfiguration Release`: the script no longer
assumes it, and `history` refuses a run whose build configuration is unknown.

```powershell
DEVEL\performance\scripts\Run-PerformanceSuite.ps1 -Platform Windows `
    -ArmAExe C:\builds\b14\GnollHackM.exe -ArmALabel Build14 -ArmABuild 4.3.0-14 `
    -ArmBExe C:\builds\head\GnollHackM.exe -ArmBLabel HEAD `
    -Scenario W1 -Pairs 2 -OutDir C:\performance\w1 `
    -InAppRecordDir <GnollHack path>\archive\performance
```

Android, with Perfetto and its CSV export (Android 12 or later; `trace_processor_shell` from
`-TraceProcessorPath`, the `TRACE_PROCESSOR` environment variable, or `PATH`, in that
order). `-InApp` sends the window command and pulls the record; when a device refuses
`adb pull` from `Android/data`, the script falls back to asking for Dump Frame Log and
waiting for the shared files in `-InAppRecordDir`:

```powershell
DEVEL\performance\scripts\Run-PerformanceSuite.ps1 -Platform Android `
    -ArmAPackage <package> -ArmALabel GPU `
    -ArmBPackage <package> -ArmBLabel CPU `
    -Scenario W-idle -Pairs 2 -Perfetto -OutDir C:\performance\idle -InApp -InAppRecordDir C:\performance\inbox
```

`<package>` is the application id of the installed build; read it from the device with
`adb shell pm list packages` (`adb shell pm list packages gnollhack` narrows the list).

Each run directory gets `smoothness.md`; the batch gets `report.md` (external series) and,
with in-app records, `report_smoothness.md`. When both arms are the same package, change the
setting or install the other build when the script announces the arm.

Background load and environment files:

| File | Where | What |
|------|-------|------|
| `load_system.csv`, `load_system_counters.txt` | Run, Windows | `typeperf` at 1 s over the capture: whole-machine CPU, the arm's process CPU, disk idle, available memory, hard page reads; the second file is its counter list |
| `load_processes.csv` | Run, Windows | Per-process CPU and GPU engine averages over the capture, in `typeperf`'s CSV shape |
| `load_android.txt`, `load_android_stderr.txt` | Run, Android | The 1 s `/proc/stat`, `/proc/<pid>/stat` and `/proc/meminfo` blocks, or the `top` fallback |
| `env_gate.json`, `env_before.json`, `env_after.json` | Run | `Get-ThermalState.ps1`, which also gives a one-shot load reading: disk busy, available memory, hard page reads, top processes, known activities and pending reboot on Windows, `dumpsys cpuinfo` on Android |
| `fingerprint_batch_start.json`, `fingerprint_batch_end.json` | Batch | `Get-EnvironmentFingerprint.ps1` before the first run and after the last; the script warns about every key outside `meta.*` that differs between the two |

The script passes each run's load files, the start fingerprint and the batch id (the leaf
name of `-OutDir`) to `ingest`. Before each run, after the thermal gate, it waits for a quiet
machine for up to `-MaxQuietWaitSeconds` (default 120); see
[Quiet gate](#quiet-gate).

### Mono GC log (Android)

Android runs the Mono runtime, whose SGen collector logs every collection when asked to.
`Capture-AndroidGcLog.ps1` has three modes:

```powershell
DEVEL\performance\scripts\Capture-AndroidGcLog.ps1 -Enable
DEVEL\performance\scripts\Capture-AndroidGcLog.ps1 -Seconds 60 -OutDir C:\performance\gc1
DEVEL\performance\scripts\Capture-AndroidGcLog.ps1 -Disable
```

`-Enable` sets `debug.mono.log` to `gc` and `debug.mono.env` to
`MONO_LOG_LEVEL=debug|MONO_LOG_MASK=gc`; `-Disable` clears both. Mono reads them when the
app starts, so after either, force-stop GnollHack (`adb shell am force-stop <package>`, which
the script prints) and start it again. The capture mode clears the device log, records
`adb logcat --pid=<pid> -v threadtime` of the running process for `-Seconds` into
`gc_log.txt`, and writes one row per collection line to `gc_events.csv`. `-Package`,
`-Serial` and `-AdbPath` work as in `Capture-AndroidFrames.ps1`.

| Line | Fields |
|------|--------|
| `GC_MINOR`, `GC_MAJOR` (and suffixed forms such as `GC_MAJOR_SWEEP`) | The reason in parentheses; `time`, the collection's pause; `stw`, the stop-the-world time, from suspending the managed threads to resuming them; `promoted`, the bytes copied out of the nursery into the major heap; the major heap and large object space sizes and their use |
| `GC_TAR_BRIDGE`, `GC_BRIDGE`, `GC_OLD_BRIDGE` | The GC bridge phase, which works out which Java peers of managed objects can be collected; `BridgeMs` sums its `<step> <n>ms` fields |

A field a line does not carry is left empty. When the log has no collection lines, the
script says so: either no collection happened in the window, the app was not restarted, or
the build ignores the properties, which a Release build may. Then use a diagnostic build:
add the two lines

```text
MONO_LOG_LEVEL=debug
MONO_LOG_MASK=gc
```

to `win\win32\xpl\GnollHackM\Platforms\Android\EnvironmentMono.txt` locally, build and
deploy, and capture without `-Enable`. Never commit that change.

## Analyzer

Build and test from the `GnollHackTests` directory, so that its `global.json` selects the SDK
(`dotnet` looks for `global.json` from the current directory, not the solution's):

```powershell
cd win\win32\xpl\GnollHackTests
dotnet build GnollHackTests.sln -c Release
dotnet test GnollHackTests.sln -c Release
```

```powershell
$a = 'win\win32\xpl\GnollHackTests\GnollHack.PerformanceAnalyzer\bin\Release\net10.0\GnollHack.PerformanceAnalyzer.exe'
& $a smoothness run_20260926_101500_123_manual_dump.json --presentmon presentmon.csv --process GnollHackM --out smoothness.md
& $a smoothness run_20260926_101500_123_manual_dump.json --perfetto C:\performance\idle\01_GPU\gfxinfo --out smoothness.md
& $a compare --a runsA --b runsB --label-a GPU --label-b CPU --series smoothness --out report_smoothness.md
& $a ingest --presentmon run.csv --process GnollHackM --refresh-hz 144 --target-fps 72 --out run.json
& $a history --file DEVEL\performance\history.jsonl --append runsA runsB
& $a history --file DEVEL\performance\history.jsonl --list --scenario W1
& $a drift --file DEVEL\performance\history.jsonl --scenario W1 --out drift.md
```

The `smoothness` report gives the headline metrics (recomputed and as the app reported
them), the join statistics for measured display times, the hitch-cause table, the ten worst
hitches each with its full stage timeline, and the change-point segments.

`ingest` also takes `--batch <id>`, `--fingerprint <json>` (repeatable; the files of
`Get-EnvironmentFingerprint.ps1`), and the per-run load files as `--env-during-system
<load_system.csv>`, `--env-during-processes <load_processes.csv>` or `--env-during-android
<load_android.txt>`, which become the record's `external` background block. With
`--run-json` it also takes the in-app record's fingerprint, batch (its suite id), notes and
background block, and it excludes in the same order as the app. A history line keeps the
background blocks without their per-second samples. `drift` finds steps over time in the
history and attributes them; see [Reading a drift report](#reading-a-drift-report).

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
| `scripts/Run-PerformanceSuite.ps1` | Interleaved two-arm batch driver: launches the app, captures presented frames and the background load, collects in-app records, gates on thermal state and a quiet machine, fingerprints the environment at batch start and end, analyzes, compares, appends to history |
| `scripts/Capture-PresentMon.ps1` | Windows: presentation timing for one process with PresentMon 2.x, QPC timestamps included |
| `scripts/Capture-AndroidFrames.ps1` | Android: `gfxinfo framestats` polling, optional Perfetto trace and CSV export. The package defaults to the Android manifest's, `com.soundmindentertainment.gnollhack` (not the csproj `ApplicationId`), and the script stops when that process is not running |
| `scripts/Capture-AndroidGcLog.ps1` | Android: turns the Mono GC log on (`-Enable`) or off (`-Disable`), or captures it from the running process for `-Seconds` into `gc_log.txt` and tabulates the collections (pause, stop-the-world time, promoted bytes, heap sizes, GC bridge time) in `gc_events.csv` |
| `scripts/Get-ThermalState.ps1` | Thermal and power facts for the Windows host or an Android device, as JSON, with a one-shot background load reading (top processes, known activities, disk, memory, pending reboot) |
| `scripts/Get-EnvironmentFingerprint.ps1` | The environment fingerprint of the Windows host or an Android device, as flat JSON in the shared key scheme |
| `scripts/Common.ps1` | Shared helpers (tool resolution, the Android package and its running process, native calls that write to stderr, JSON writing, git facts, the background load sampler and the quiet check) |
| `perfetto/frametimeline.pbtxt` | Perfetto trace config |
| `perfetto/export_frames.sql`, `perfetto/export_app_slices.sql` | `trace_processor` queries behind the Perfetto CSVs |
| `schema/run-record.schema.json` | The in-app run record format (schema v2), with the optional `suite` object of a suite run, the `marks` array, the `background` block and `environment.fingerprint` |
| `schema/suite-manifest.schema.json` | The `suite.json` manifest of an in-app Performance Suite (manifest version 1), with the fingerprints at start and end and each run's background verdict |
| `history.jsonl` | Append-only record of Release runs, created on first append |
