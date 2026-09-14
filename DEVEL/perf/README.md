# GnollHack Performance Measurement

This directory holds the performance test protocol, the scripts that run it, the
machine-readable record of every measured run, and the reports. The in-app tooling
(`FrameTimeProfiler`, the debug dashboard, the developer-menu harness) and the offline
analyzer in `win/win32/xpl/GnollHackTests/GnollHack.PerfAnalyzer` implement it.

The point of all of it: a claim such as "tile batching is slower on Android" or
"Build 15 is jerkier than Build 14" must come from a controlled comparison with a stated
effect size and confidence interval, taken on a device whose thermal state is known,
and it must be recorded so that the next framework update can be judged against it.

## Contents

| Path | What |
|------|------|
| `scripts/Run-PerfSuite.ps1` | Interleaved two-arm batch driver: launches the app, captures presented frames, gates on thermal state, ingests, compares, appends to history |
| `scripts/Capture-PresentMon.ps1` | Windows: captures presentation timing for one process with Intel PresentMon |
| `scripts/Capture-AndroidFrames.ps1` | Android: polls `dumpsys gfxinfo framestats`, saves the summary, optionally records a Perfetto trace |
| `scripts/Get-ThermalState.ps1` | Thermal and power facts for the Windows host or an Android device, as JSON |
| `scripts/Common.ps1` | Shared helpers (tool resolution, JSON writing, git facts) |
| `schema/run-record.schema.json` | The run record format |
| `history.jsonl` | Append-only record of every Release run, one JSON object per line, without raw intervals |
| `reports/` | Comparison reports (`compare` output) worth keeping |
| `../performance.txt` | Human-readable summary, older format; see the `performance_benchmarks` skill |

## Metrics

All metrics come from a series of frame intervals over a fixed measurement window after
a discarded warm-up. Two series exist per run and are reported side by side:

- **External**: presented-frame intervals from the operating system (PresentMon on
  Windows, `gfxinfo framestats` on Android). This is the ground truth and is
  independent of any code in the app.
- **Internal**: `FrameTimeProfiler` intervals between rendered frames, with the CPU phase
  breakdown (update, lock wait, paint, flush), GC and allocation data, UI-thread latency
  and game-thread turn time that only the app can see.

| Metric | Definition |
|--------|------------|
| `frameDurationP50/P90/P95/P99/Max` | Nearest-rank percentiles of the interval, ms |
| `fpsMean` | Frames divided by window length |
| `fps1PctLow`, `fps01PctLow` | 1000 divided by the mean of the slowest 1 percent (0.1 percent) of intervals |
| `frameOverrunP50/P95/P99` | Interval minus the vsync period |
| `jankPct` | Percent of intervals over 1.5 x vsync |
| `hitchRatio` | Over intervals exceeding 2 x vsync, sum of (interval minus vsync) divided by window length: ms of hitch per second (Apple's hitch time ratio) |
| `hitch2xPerMin`, `hitch4xPerMin` | Intervals over 2 x and 4 x vsync, per minute |
| `stutterIndex` | Standard deviation divided by mean of the intervals |

The vsync period is the display refresh period actually in effect and is recorded per run.

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
   affects both arms equally. `Run-PerfSuite.ps1 -Pairs 2` produces exactly that.
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
   recorded before and after each run (`thermal.powerState`: `charging`, `battery`,
   `changed`, `unknown`), held constant for a whole batch, and never varied between the
   arms of a comparison. A run whose state changed mid-window is excluded; a comparison
   whose runs mix states is flagged in the report. The batch driver warns when Windows
   starts on battery or Android starts charging, and when the state changes between runs.

## Statistics

- Percentiles are nearest-rank on the raw intervals.
- **Run level decides.** Each run contributes one value per metric. The difference of arm
  medians gets a bootstrap 95 percent confidence interval by resampling runs; a
  difference counts only when that interval excludes zero **and** exceeds the
  pre-registered threshold: `frameDurationP99` worse by 20 percent or by one vsync
  period, `hitchRatio` worse by 2 ms/s, `fps1PctLow` worse by 10 percent. The
  Mann-Whitney p-value is exact for tie-free samples up to 400 pairs (R's
  `wilcox.test`, SciPy `method="exact"`), asymptotic with tie and continuity
  corrections otherwise; it is reported, not decided on, because at five runs per arm
  the smallest attainable p is 0.0079.
- **Interval level describes.** All intervals of an arm pooled: Mann-Whitney U and
  Cliff's delta (bands: negligible below 0.147, small below 0.33, medium below 0.474,
  large above), the Hodges-Lehmann shift in ms, and bootstrap intervals on the median
  and P99 differences. Pooled intervals from different runs are not independent, so this
  level never decides.
- **No outlier removal.** A hitch is the phenomenon. Runs are excluded only for a
  recorded reason: thermal throttling, too few frames, capture failure.
- **Minimum detectable effect** is printed so that "no difference" reads as "no
  difference larger than X".

## Thermal control

Every run records thermal facts before and after the window:

| Platform | Signals |
|----------|---------|
| Windows | `% Processor Performance` counter (throttling shows as sustained values under 100 at load; the gate floor is 90), processor frequency, GPU temperature and clocks via `nvidia-smi` when present, CPU package temperature when running elevated, power plan, charging, refresh rate, top processes |
| Android | `dumpsys thermalservice` status (Nominal, Light, Moderate, Severe, Critical), thermal zones, battery temperature, charging, low-power mode, CPU frequencies |

A run is flagged **throttled** and excluded by default when the status rises above
Moderate, when the Windows processor performance counter is under 90 percent, or when
the status rose two classes across the run. The signal that decided is stored in the
record (`thermal.gateSignal`).

## Running a batch

Windows, Build 14 versus HEAD, scenario W1 by hand (wizard mode, Ctrl+F to reveal the map,
minimap mode), two ABBA pairs:

```powershell
DEVEL\perf\scripts\Run-PerfSuite.ps1 -Platform Windows `
    -ArmAExe C:\builds\b14\GnollHackM.exe -ArmALabel Build14 -ArmABuild 4.3.0-14 `
    -ArmBExe C:\builds\head\GnollHackM.exe -ArmBLabel HEAD `
    -Scenario W1 -Pairs 2 -OutDir C:\perf\2026-09-14_w1_b14_head
```

Android, same comparison, with a Perfetto trace per run:

```powershell
DEVEL\perf\scripts\Run-PerfSuite.ps1 -Platform Android `
    -ArmAPackage com.hyvanmielenpelit.gnollhack -ArmALabel Build14 -ArmABuild 4.3.0-14 `
    -ArmBPackage com.hyvanmielenpelit.gnollhack -ArmBLabel HEAD `
    -Scenario W1 -Pairs 2 -Perfetto -OutDir C:\perf\2026-09-14_w1_android
```

When both arms are the same package (as above), install the other build between runs
when the script announces the arm; the settle countdown is for that and for setting up
the scenario.

PresentMon needs an elevated console. `adb` is resolved from `PATH`, then
`C:\Program Files (x86)\Android\android-sdk\platform-tools`, then `ANDROID_HOME`; pass
`-AdbPath` otherwise.

## Analyzer commands

```powershell
$a = 'win\win32\xpl\GnollHackTests\GnollHack.PerfAnalyzer\bin\Release\net10.0\GnollHack.PerfAnalyzer.exe'
& $a ingest --presentmon run.csv --process GnollHackM --refresh-hz 120 --warmup 10 --window 60 --arm HEAD --scenario W1 --platform Windows --build-config Release --env-before before.json --env-after after.json --out run.json
& $a compare --a runsA --b runsB --label-a Build14 --label-b HEAD --out report.md
& $a history --file DEVEL\perf\history.jsonl --append runsA runsB
& $a history --file DEVEL\perf\history.jsonl --list --platform Android
```

Build it once with `dotnet build win\win32\xpl\GnollHackTests\GnollHackTests.sln -c Release`.

## Reference devices

| Platform | Device |
|----------|--------|
| Android | Google Pixel 6a |
| iOS | iPad (8th generation, 2020) |
| Windows | MSI Pulse GL66 |

A different device is a different device id in the record, never a continuation.

## Scenarios

| Id | Workload |
|----|----------|
| W0 | Static map, hero idle, no input |
| W1 | Minimap mode, whole level revealed (wizard mode, Ctrl+F) |
| W2 | Zoom cycle: normal, mini, half, normal every 5 s |
| W3 | Message flood and message-history scrolling |
| W4 | Menu churn: inventory open and close every 2 s |
| W5 | Travel between the up and down stairs, repeated |
| W6 | Level change every 10 s |
| replay | A reference `.gnhrec` played back at speed 1.0; no game thread, no C core, so it isolates the render path and is never compared against a gameplay run |

W0 to W6 are driven by the in-app harness (Developer menu, Performance Tests) from Phase 2
of the plan onward; before that, W1 is performed by hand.
