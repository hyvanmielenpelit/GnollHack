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
last 20736 display callbacks (144 s at 144 Hz, about 6 MB while the profiler is on), one
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

### Stutter tools

For a stutter felt during play or a replay, while the profiler records. The Developer button
of the game menu is shown in developer mode; its frame tools only while the profiler is on.

| Tool | What it does |
|------|--------------|
| **Game menu > Developer > Mark Stutter** | Marks the frame that was on screen when the game menu was opened; reports a failure when the frame ring no longer holds it |
| **F8** (Windows) | Marks the current frame, in play and in replays. With screen logging on, the screen log gets `MARK frame N` (or `MARK failed for frame N`) |
| **Game menu > Developer > Analyze Recent** | A plain-text report of the last 30 s before the game menu was opened, shown in the viewer and written to `archive/recent_hitches.txt`: displayed FPS, hitch ratio and GC of the span; **Marked moments**, each mark with the hitches displayed within 3 s either side of it (the ten largest when there are more); the ten worst hitches, each with its cause, the draw and flush times of the tick that ended it, the request time and collections in the gap, and its content events; and the hitch causes of the span |

A mark sets the `UserMark` flag on the frame's record and enters a ring of the last 32 marks.
Every saved run record (Dump Frame Log, a window command, a suite run) lists the marks in its
range as `marks`: frame id, UTC time, and milliseconds from the first saved tick. The frame
timeline CSV's `# OriginUtc` header line gives the wall-clock time of the tick its millisecond
columns count from, so a row can be matched with a screen recording or the screen log.
Offline, the `smoothness` report has a **Marked moments** section: for each mark (from
`marks`, or from the `UserMark` flags when the record has none), the hitches displayed within
3 s of it and the stage timeline of the nearest one.

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
| Hitch time ratio | Sum of `g_j - T` over frames with `g_j > H + R/2`, per second, where `H` is the longest on-time hold: `T` rounded up to whole refreshes (`max(T, ceil(T/R - 0.05) * R)`), since a frame can only change at a vsync. At 40 FPS on 60 Hz the two-refresh hold is on time and only three refreshes are a hitch; at a divisor rate `H = T`. Apple's bands: under 5 ms/s good, 5 to 10 warning, over 10 critical |
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
| 1 | `DisplayMode` | The measured refresh period moved by more than 5 % across the gap or within the few ticks after it (the measurement is a running median, which lags a real change), or the pacing logic assumes a rate more than 5 % off the measured one |
| 2 | `PaintCpu` / `Gpu` | A late or missed callback while the UI thread was still painting the previous map frame |
| 2 | `UiThreadRequests` | A late or missed callback on a tick whose request handling (floating texts, messages, windows, ...) took more than `R/2`; it takes precedence over a collection in the same gap |
| 2 | `UiThreadLateGc` | A missed callback, or a callback more than `R/2` after its vsync, and collections in the gap paused the process for at least `R/2` in total: long enough to explain the lateness. Without pause data (Mono, older captures), any collection in the gap counts, and the report says so |
| 2 | `FrameworkCadence` | A late or missed callback, not explained by the above, while the callback period ran at 1.5 refreshes or more around the gap: the UI framework delivered callbacks below the panel's rate for a while (Windows) |
| 2 | `UiThreadLate` | A late or missed callback that nothing above explains |
| 3 | `PacingPolicy` | A modulo skip or catch-up render in the gap, or a refresh-to-target ratio the divisor pattern cannot pace evenly; either only when the gap is within the pattern's longest hold (two divisor steps) |
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
   scenarios, 120 seconds for gameplay; `Run-PerformanceSuite.ps1` picks it from
   `-ScenarioKind` unless `-WindowSeconds` is given.
3. **Repetitions**: at least 5 runs per configuration for a decision, 3 for a smoke
   check. Below 3 runs in either arm `compare` makes no decision and prints the minimum
   detectable effect only. The report prints the minimum detectable effect at the observed
   spread; raise the count when it is too coarse for the question.
4. **Interleaving**: ABBA then BAAB, never AAAA BBBB, so that drift within the batch
   affects both arms equally. `Run-PerformanceSuite.ps1 -Pairs 2` produces exactly that.
5. **Cool-down** of 20 seconds between runs, extended until the device is back in the
   thermal class it started the batch in (up to 5 minutes). On Windows, which has no
   thermal status, the class is the processor performance counter compared with the batch
   start: more than 10 points lower is throttled.
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
  reason: thermal throttling, a power state that changed during the run, or fewer than 100
  frames (external series) or on-screen intervals (in-app records). A run whose capture
  failed produces no record; the batch script logs it as skipped and continues.
- **Windows throttling** is judged from the processor performance counter, which reads
  well under 100 on an idle machine under the Balanced plan: a run counts as throttled only
  when the reading taken after the run is under 90 % and at least 10 points below the one
  taken before it. The reason names the power plan.
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

## In-app Performance Suite

The suite measures a build on the device itself, with no PC: a series of measurement windows
over one recorded replay, each started from the same turn, saved as ordinary run records and
compared on the device. **About > Performance Suite** opens it; the button is shown only with
developer mode on and **Settings > Frame Time Profiler** on. The runner switches the profiler
on for the suite and restores it afterwards.

### Setup

The page remembers the last setup.

| Field | Default | Meaning |
|-------|---------|---------|
| Replay | none | A main replay file in `<GHPath>/replay`, newest first; continuation files are not listed |
| Start Turn | 1 | The turn every run starts from, at least 1 |
| Scenario | Idle | **Idle**: the replay pauses at the start turn, leaving only tile animations (W-idle). **Minimap**: the replay pauses and the map switches to minimap zoom (W1). **Playback**: the replay keeps playing (replay) |
| Measured runs | 6 | 3, 5, 6 or 8, not counting the warm-up run |
| Warm-up run | On | Run 0, a full run that is saved but excluded as `warm-up run` |
| Page mode | Shared page | **Shared page**: one game page for every run. **Fresh page per run**: a new game page for each run |
| Arm label | `<app version> <short commit>` | Names the arm; suites with the same label pool into one arm |
| Warm-up, window and cool-down seconds | 10, 60, 20 | Per run, as in the Protocol |

The estimated duration is (measured runs + warm-up run) x (15 s for the seek + warm-up +
window + cool-down): about 12 minutes with the defaults.

### What a suite does

1. The first game page plays the replay from its beginning, exactly as the Replay page starts
   it, when the start turn is 1 or less; otherwise it seeks to start turn - 1 and plays the
   last turn at normal speed.
2. The runner waits, for at most 120 s, until the start turn is reached and no replayed menu,
   text window, prompt or popup covers the map, and applies the scenario. The replay header
   shows the progress: `Performance suite: run N of M`, `warm-up run` or `cooling down`.
3. Warm-up, then the measurement window, saved with the suite's context.
4. Cool-down, then a thermal gate: the next run waits until the thermal class is no worse than
   at the suite start, checking every 15 s, and goes ahead after 300 s regardless. On Windows
   the class comes from the processor performance counter: more than 10 points below the
   suite start is throttled.
5. The next run starts. With a shared page the replay, paused during the cool-down, seeks back
   to start turn - 1, which restarts it in place and collects garbage. With a fresh page the
   game page closes before the cool-down, which then runs on the suite page, and after a
   collection a new game page starts as in step 1.

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
`compositorframes_*.csv`, and `report.txt`. `<GHPath>/performance/baselines.json` records one
baseline arm label per comparability key. Unlike `archive`, the `performance` directory is not
cleared when the app starts.

The results list shows each suite's date, scenario, label, used runs, median hitch ratio and
size, tagged `baseline`, `aborted` or `imported`. A used run is a measured run with a summary
and no exclusion reason.

| Button | Action |
|--------|--------|
| View Report | `report.txt`: setup, replay size, SHA-256 and start turn, environment, the run table, the medians of the used runs, hitch causes and content events |
| Set as Baseline | Records the suite's arm label as the baseline for its comparability key |
| Compare with Baseline | Compares the suite's arm with the baseline arm of its key |
| Share, Import Results | See [Share and Import](#share-and-import) |
| Delete | Deletes the selected suites' folders; `baselines.json` is left as it is |

The **comparability key** is the scenario, the replay's SHA-256, the start turn, the page
mode, the map FPS setting and the measured refresh rate rounded to whole hertz, fixed when the
suite finishes. A comparison pools, per arm, every suite with the same key and the same arm
label: arm A the baseline label, arm B the selected suite's. Suites with another key never
enter it. It refuses when the suite is still running, no baseline is set for its key, the
suite's own label is the baseline label, or either arm has no used runs.

The decision is the one `compare --series smoothness` makes offline, from the same code
(`GHPerformanceComparison`), as described under [Statistics](#statistics). With fewer than 3
used runs in either arm there is no verdict; below 5 the verdict is marked provisional. The
report lists both arms' versions and what differs between them, the verdict per metric, and
each arm's hitch causes.

### Two builds on one device

- **Windows**: unpackaged builds share one store. Run a suite in each build, set the older
  build's suite as the baseline, and compare the newer one's. The default arm labels differ,
  since they carry the version and commit.
- **Android and iOS**: a reinstall may wipe the app's data, and the store with it. Share the
  suites before installing another build and import them afterwards. Interleave in blocks:
  3 runs with the old build, install the new build, 6 runs, reinstall the old build, 3 runs.
  Both old-build suites carry the same label and pool into one 6-run arm, so drift over the
  session affects both arms alike.

### Share and Import

**Share** zips the selected suites into
`archive/GnollHack_Performance_<device model>_<timestamp>.zip`, each suite's folder with its
`report.txt` rewritten and, when the suite compares with its baseline, a `comparison.txt`,
and hands the zip to the system share sheet. **Import Results** picks a zip and checks every
top-level folder before extracting anything: a `suite.json` with a `manifestVersion`, and
every run file it names present with schema version 2. Valid folders move into the store,
tagged `imported`; a suite whose id is already present is skipped, never overwritten; the
others are reported with the reason.

On a PC, unzip the archive and run the analyzer on the suite folders (`$a` as in
[Analyzer](#analyzer)):

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
| `scripts/Capture-PresentMon.ps1` | Windows: presentation timing for one process with PresentMon 2.x, QPC timestamps included |
| `scripts/Capture-AndroidFrames.ps1` | Android: `gfxinfo framestats` polling, optional Perfetto trace and CSV export |
| `scripts/Get-ThermalState.ps1` | Thermal and power facts for the Windows host or an Android device, as JSON |
| `scripts/Common.ps1` | Shared helpers (tool resolution, native calls that write to stderr, JSON writing, git facts) |
| `perfetto/frametimeline.pbtxt` | Perfetto trace config |
| `perfetto/export_frames.sql`, `perfetto/export_app_slices.sql` | `trace_processor` queries behind the Perfetto CSVs |
| `schema/run-record.schema.json` | The in-app run record format (schema v2), with the optional `suite` object of a suite run and the `marks` array |
| `schema/suite-manifest.schema.json` | The `suite.json` manifest of an in-app Performance Suite (manifest version 1) |
| `history.jsonl` | Append-only record of Release runs, created on first append |
