<#
.SYNOPSIS
Runs an interleaved two-arm performance batch with external frame capture, thermal
gating, and analysis.

.DESCRIPTION
Phase 0 form: the app is launched by this script, but the scenario itself is performed
by a person (for W1: enter wizard mode, press Ctrl+F to reveal the map, switch to
minimap). The script drives everything around that: the ABBA sequence, a settle
countdown so the scenario can be set up, the warm-up, the capture window, thermal
readings before and after, cool-down with thermal gating, ingestion into run records,
the comparison report, and the history append.

Arms. Each arm is a launch specification:
  Windows  -ArmAExe <path to GnollHackM.exe> (unpackaged build); the process name for
           PresentMon is derived from the file name unless -ArmAProcess is given.
  Android  -ArmAPackage <package id>; the activity is resolved with `cmd package
           resolve-activity`, or pass -ArmAActivity.
The same for B. When A and B are the same binary with different settings, use the
same path for both and change the settings by hand between runs; the script prints
which arm is up before each launch.

Sequence: for -Pairs 2 the order is A B B A  B A A B (ABBA then BAAB), which
balances slow drift within the batch. Every run writes:
  <OutDir>\<nn>_<arm>\env_before.json, env_after.json, capture (CSV or framestats
  directory), run.json
and the batch writes report.md and appends to history.jsonl.

Thermal gating: before each run the device must report the same thermal class as the
first run did (Android status; Windows processor performance at or above 90 percent).
Up to -MaxGateWaitSeconds are spent waiting; after that the run proceeds and the record
says the gate timed out.

In-app records (-InAppRecordDir): with the frame profiler enabled in the app, "Dump
Frame Log" in the developer menu writes a run record of everything the frame timeline
still holds (run_*.json plus its two CSVs) to the app's archive\performance directory and
shares them as framelog.zip. After each capture the script waits for that record to
appear in -InAppRecordDir (on Windows the app's own archive\performance directory; on
Android a directory you copy the shared files into), copies it into the run directory,
and runs the analyzer's smoothness command on it together with that run's PresentMon or
Perfetto capture. The batch then also writes report_smoothness.md, the A/B comparison of
displayed FPS, hitch time ratio and pacing error.

.EXAMPLE
.\Run-PerformanceSuite.ps1 -Platform Windows -ArmAExe C:\b14\GnollHackM.exe -ArmALabel Build14 `
    -ArmBExe C:\head\GnollHackM.exe -ArmBLabel HEAD -Scenario W1 -Pairs 2 -OutDir C:\perf\w1
#>
[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)] [ValidateSet('Windows', 'Android')] [string] $Platform,
    [string] $ArmAExe, [string] $ArmAProcess, [string] $ArmAPackage, [string] $ArmAActivity,
    [string] $ArmBExe, [string] $ArmBProcess, [string] $ArmBPackage, [string] $ArmBActivity,
    [string] $ArmALabel = 'A',
    [string] $ArmBLabel = 'B',
    [string] $ArmABuild = '',
    [string] $ArmBBuild = '',
    [Parameter(Mandatory = $true)] [string] $Scenario,
    [string] $ScenarioKind = 'gameplay',
    [Parameter(Mandatory = $true)] [string] $OutDir,
    [int] $Pairs = 2,
    [int] $SettleSeconds = 45,
    [int] $WarmupSeconds = 10,
    [int] $WindowSeconds = 60,
    [int] $CooldownSeconds = 20,
    [int] $MaxGateWaitSeconds = 300,
    [double] $RefreshHz = 0,
    [double] $TargetFps = 0,
    [string] $Serial,
    [string] $AdbPath,
    [string] $PresentMonPath,
    [switch] $Perfetto,
    [string] $TraceProcessorPath,
    [string] $InAppRecordDir,
    [string] $DeviceId,
    [string] $DeviceModel,
    [string] $DeviceOs,
    [string] $BuildConfiguration = 'Release',
    [string] $HistoryFile,
    [switch] $NoHistory,
    [string] $Notes = ''
)

$ErrorActionPreference = 'Stop'
. (Join-Path $PSScriptRoot 'Common.ps1')
$repoRoot = (Resolve-Path (Join-Path $PSScriptRoot '..\..\..')).Path
if (-not $HistoryFile) { $HistoryFile = Join-Path $repoRoot 'DEVEL\performance\history.jsonl' }

# ---- Validate arms -------------------------------------------------------------------
function New-Arm {
    param([string] $Label, [string] $Exe, [string] $Process, [string] $Package, [string] $Activity, [string] $Build)
    $arm = @{ label = $Label; exe = $Exe; process = $Process; package = $Package; activity = $Activity; build = $Build }
    if ($Platform -eq 'Windows') {
        if (-not $Exe) { throw "Arm $Label needs -Arm${Label}Exe on Windows" }
        if (-not (Test-Path -LiteralPath $Exe)) { throw "Arm $Label executable not found: $Exe" }
        if (-not $Process) { $arm.process = [System.IO.Path]::GetFileNameWithoutExtension($Exe) }
    } else {
        if (-not $Package) { throw "Arm $Label needs a package id on Android" }
    }
    return $arm
}
$arms = @{
    A = (New-Arm -Label $ArmALabel -Exe $ArmAExe -Process $ArmAProcess -Package $ArmAPackage -Activity $ArmAActivity -Build $ArmABuild)
    B = (New-Arm -Label $ArmBLabel -Exe $ArmBExe -Process $ArmBProcess -Package $ArmBPackage -Activity $ArmBActivity -Build $ArmBBuild)
}

$adb = $null
$serialArgs = @()
if ($Platform -eq 'Android') {
    $adb = Resolve-PerformanceAdb -AdbPath $AdbPath
    if ($Serial) { $serialArgs = @('-s', $Serial) }
    if (-not $DeviceModel) { $DeviceModel = ((& $adb @serialArgs shell getprop ro.product.model 2>$null) | Select-Object -First 1) }
    if (-not $DeviceOs) { $DeviceOs = 'Android ' + ((& $adb @serialArgs shell getprop ro.build.version.release 2>$null) | Select-Object -First 1) }
    if ($RefreshHz -le 0) {
        # SurfaceFlinger reports the rate the panel is running at now; dumpsys display lists
        # every supported mode, so its first match may not be the active one
        $sf = (& $adb @serialArgs shell dumpsys SurfaceFlinger 2>$null) -join "`n"
        if ($sf -match 'refresh-rate\s*:\s*([\d.]+)') {
            $RefreshHz = [double]$Matches[1]
        } else {
            $dm = (& $adb @serialArgs shell dumpsys display 2>$null) -join "`n"
            if ($dm -match 'mActiveSfDisplayMode=[^\n]*?(?:refreshRate|fps)=([\d.]+)') { $RefreshHz = [double]$Matches[1] }
            elseif ($dm -match 'fps=([\d.]+)') { $RefreshHz = [double]$Matches[1] }
            else { $RefreshHz = 60 }
        }
        Write-PerformanceLog ("Active refresh rate: {0:0.##} Hz (adaptive panels may change it during a run; the in-app record measures it per frame)" -f $RefreshHz)
    }
} else {
    Resolve-PerformancePresentMon -PresentMonPath $PresentMonPath | Out-Null
    if (-not $DeviceModel) {
        try { $cs = Get-CimInstance Win32_ComputerSystem; $DeviceModel = ($cs.Manufacturer + ' ' + $cs.Model).Trim() } catch { $DeviceModel = $env:COMPUTERNAME }
    }
    if (-not $DeviceOs) {
        try { $os = Get-CimInstance Win32_OperatingSystem; $DeviceOs = ($os.Caption + ' ' + $os.Version).Trim() } catch { $DeviceOs = 'Windows' }
    }
    if ($RefreshHz -le 0) {
        try { $vc = Get-CimInstance Win32_VideoController | Where-Object { $_.CurrentRefreshRate -gt 0 } | Select-Object -First 1; $RefreshHz = [double]$vc.CurrentRefreshRate } catch { $RefreshHz = 60 }
    }
}
if (-not $DeviceId) { $DeviceId = ($DeviceModel + ' / ' + $DeviceOs) }

$analyzer = Resolve-PerformanceAnalyzer -RepoRoot $repoRoot
New-Item -ItemType Directory -Force -Path $OutDir | Out-Null
$git = Get-PerformanceGitFacts -RepoRoot $repoRoot

# ---- Sequence ------------------------------------------------------------------------
$order = @()
for ($p = 0; $p -lt $Pairs; $p++) {
    if (($p % 2) -eq 0) { $order += @('A', 'B', 'B', 'A') } else { $order += @('B', 'A', 'A', 'B') }
}
Write-PerformanceLog ("Batch: {0} on {1}, scenario {2}, sequence {3}" -f $Platform, $DeviceId, $Scenario, ($order -join ' '))
Write-PerformanceLog ("Per run: settle {0} s, warm-up {1} s, window {2} s, cool-down {3} s; vsync {4:0.00} ms" -f $SettleSeconds, $WarmupSeconds, $WindowSeconds, $CooldownSeconds, (1000.0 / $RefreshHz))

# ---- Helpers ---------------------------------------------------------------------------
function Read-Thermal {
    param([string] $Path)
    if ($Platform -eq 'Android') {
        & (Join-Path $PSScriptRoot 'Get-ThermalState.ps1') -Android -Serial $Serial -AdbPath $AdbPath -OutFile $Path | Out-Null
    } else {
        & (Join-Path $PSScriptRoot 'Get-ThermalState.ps1') -SampleSeconds 3 -OutFile $Path | Out-Null
    }
    return (Get-Content -LiteralPath $Path -Raw -Encoding UTF8 | ConvertFrom-Json)
}

function Get-ThermalClass {
    param($Reading)
    if ($Platform -eq 'Android') { return $Reading.status }
    if ($null -eq $Reading.cpuPerformancePct) { return 'Unknown' }
    if ($Reading.cpuPerformancePct -ge 90) { return 'Nominal' } else { return 'Throttled' }
}

function Wait-ForThermalClass {
    param([string] $Target, [string] $ProbePath)
    if ($Target -eq 'Unknown') { return $true }
    $sw = [System.Diagnostics.Stopwatch]::StartNew()
    while ($true) {
        $r = Read-Thermal -Path $ProbePath
        $cls = Get-ThermalClass -Reading $r
        if ($cls -eq $Target -or $cls -eq 'Unknown') { return $true }
        if ($sw.Elapsed.TotalSeconds -ge $MaxGateWaitSeconds) {
            Write-Warning ("Thermal gate timed out after {0} s (class {1}, wanted {2}); proceeding" -f [int]$sw.Elapsed.TotalSeconds, $cls, $Target)
            return $false
        }
        Write-PerformanceLog ("Thermal gate: class {0}, waiting for {1}" -f $cls, $Target)
        Start-Sleep -Seconds 15
    }
}

function Start-Arm {
    param($Arm)
    if ($Platform -eq 'Windows') {
        $proc = Start-Process -FilePath $Arm.exe -WorkingDirectory (Split-Path -Parent $Arm.exe) -PassThru
        return $proc
    }
    $activity = $Arm.activity
    if (-not $activity) {
        $res = (& $adb @serialArgs shell cmd package resolve-activity --brief $Arm.package 2>$null) | Select-Object -Last 1
        if ($res -and $res.Contains('/')) { $activity = $res.Trim() } else { $activity = $Arm.package + '/.MainActivity' }
    }
    & $adb @serialArgs shell am force-stop $Arm.package 2>$null | Out-Null
    & $adb @serialArgs shell am start -W -n $activity 2>$null | Out-Null
    return $null
}

function Stop-Arm {
    param($Arm, $Proc)
    if ($Platform -eq 'Windows') {
        if ($null -ne $Proc -and -not $Proc.HasExited) {
            $Proc.CloseMainWindow() | Out-Null
            Start-Sleep -Seconds 3
            if (-not $Proc.HasExited) { Stop-Process -Id $Proc.Id -Force -Confirm:$false }
        }
        Get-Process -Name $Arm.process -ErrorAction SilentlyContinue | Stop-Process -Force -Confirm:$false -ErrorAction SilentlyContinue
    } else {
        & $adb @serialArgs shell am force-stop $Arm.package 2>$null | Out-Null
    }
}

# Waits for a run record newer than $Since in -InAppRecordDir, copies it and its two CSVs
# into the run directory, and returns the copied JSON path, or $null if none arrives.
function Get-InAppRecord {
    param([string] $RunDir, [DateTime] $Since)
    Write-PerformanceLog ("In the app: developer menu > Dump Frame Log now. Waiting for a run record in {0}" -f $InAppRecordDir)
    $sw = [System.Diagnostics.Stopwatch]::StartNew()
    $found = $null
    while ($null -eq $found -and $sw.Elapsed.TotalSeconds -lt 180) {
        if (Test-Path -LiteralPath $InAppRecordDir) {
            $found = Get-ChildItem -LiteralPath $InAppRecordDir -Filter 'run_*.json' -ErrorAction SilentlyContinue |
                Where-Object { $_.LastWriteTimeUtc -ge $Since } | Sort-Object LastWriteTimeUtc -Descending | Select-Object -First 1
        }
        if ($null -eq $found) { Start-Sleep -Seconds 2 }
    }
    if ($null -eq $found) {
        Write-Warning 'No in-app run record arrived within 180 s; this run has only the external capture.'
        return $null
    }
    $stem = [System.IO.Path]::GetFileNameWithoutExtension($found.Name)
    foreach ($name in @($found.Name, ('frametimeline_' + $stem + '.csv'), ('compositorframes_' + $stem + '.csv'))) {
        $src = Join-Path $InAppRecordDir $name
        if (Test-Path -LiteralPath $src) { Copy-Item -LiteralPath $src -Destination $RunDir -Force }
    }
    Write-PerformanceLog ("In-app record: {0}" -f $found.Name)
    return (Join-Path $RunDir $found.Name)
}

# ---- Runs ------------------------------------------------------------------------------
$batchStart = Read-Thermal -Path (Join-Path $OutDir 'env_batch_start.json')
$targetClass = Get-ThermalClass -Reading $batchStart
$batchPower = $batchStart.isCharging
Write-PerformanceLog ("Batch starting thermal class: {0}; power: {1}" -f $targetClass, $(if ($null -eq $batchPower) { 'unknown' } elseif ($batchPower) { 'charging' } else { 'battery' }))
if ($Platform -eq 'Windows' -and $batchPower -eq $false) { Write-Warning 'Windows reference runs are done plugged in (protocol rule 7). This batch is on battery.' }
if ($Platform -eq 'Android' -and $batchPower -eq $true) { Write-Warning 'Android reference runs are done unplugged (protocol rule 8): charging heats the device. This batch is charging.' }

$runFiles = @{ A = @(); B = @() }
$smoothFiles = @{ A = @(); B = @() }
$n = 0
foreach ($key in $order) {
    $n++
    $arm = $arms[$key]
    $runDir = Join-Path $OutDir ('{0:D2}_{1}' -f $n, $arm.label)
    New-Item -ItemType Directory -Force -Path $runDir | Out-Null
    Write-Host ''
    Write-PerformanceLog ("===== Run {0}/{1}: arm {2} ({3}) =====" -f $n, $order.Count, $key, $arm.label)

    $gateOk = Wait-ForThermalClass -Target $targetClass -ProbePath (Join-Path $runDir 'env_gate.json')
    $before = Read-Thermal -Path (Join-Path $runDir 'env_before.json')
    if ($null -ne $batchPower -and $null -ne $before.isCharging -and $before.isCharging -ne $batchPower) {
        Write-Warning ("Power state changed since the batch started (now {0}). The record will say so; the comparison will flag it." -f $(if ($before.isCharging) { 'charging' } else { 'battery' }))
    }

    $runStartUtc = [DateTime]::UtcNow
    $proc = Start-Arm -Arm $arm
    Wait-PerformanceCountdown -Seconds $SettleSeconds -Message ("Set up scenario {0} in the app now" -f $Scenario)
    Write-PerformanceLog ("Warm-up {0} s then capture {1} s" -f $WarmupSeconds, $WindowSeconds)

    $captureSeconds = $WarmupSeconds + $WindowSeconds
    $capturePath = $null
    if ($Platform -eq 'Windows') {
        $capturePath = Join-Path $runDir 'presentmon.csv'
        & (Join-Path $PSScriptRoot 'Capture-PresentMon.ps1') -ProcessName $arm.process -Seconds $captureSeconds -OutCsv $capturePath -PresentMonPath $PresentMonPath
    } else {
        $capturePath = Join-Path $runDir 'gfxinfo'
        & (Join-Path $PSScriptRoot 'Capture-AndroidFrames.ps1') -Package $arm.package -Seconds $captureSeconds -OutDir $capturePath -Serial $Serial -AdbPath $AdbPath -Perfetto:$Perfetto -TraceProcessorPath $TraceProcessorPath
    }

    $after = Read-Thermal -Path (Join-Path $runDir 'env_after.json')

    $inAppJson = $null
    if ($InAppRecordDir) {
        $inAppJson = Get-InAppRecord -RunDir $runDir -Since $runStartUtc
    }
    Stop-Arm -Arm $arm -Proc $proc

    if ($inAppJson) {
        $smooth = @('smoothness', $inAppJson, '--out', (Join-Path $runDir 'smoothness.md'))
        if ($Platform -eq 'Windows') {
            $smooth += @('--presentmon', $capturePath, '--process', $arm.process)
        } elseif (Test-Path -LiteralPath (Join-Path $capturePath 'perfetto_frames.csv')) {
            $smooth += @('--perfetto', $capturePath)
        }
        try {
            Invoke-PerformanceAnalyzer -Exe $analyzer -Arguments $smooth
        } catch {
            Write-Warning ("Smoothness analysis failed for this run: {0}" -f $_.Exception.Message)
        }
        $smoothFiles[$key] += $inAppJson
    }

    $runJson = Join-Path $runDir 'run.json'
    $ingest = @('ingest', '--out', $runJson,
        '--arm', $arm.label, '--scenario', $Scenario, '--scenario-kind', $ScenarioKind, '--platform', $Platform,
        '--refresh-hz', ([string]$RefreshHz), '--warmup', ([string]$WarmupSeconds), '--window', ([string]$WindowSeconds),
        '--env-before', (Join-Path $runDir 'env_before.json'), '--env-after', (Join-Path $runDir 'env_after.json'),
        '--build-config', $BuildConfiguration, '--device-id', $DeviceId, '--device-model', $DeviceModel, '--device-os', $DeviceOs,
        '--id', ('{0}_{1:D2}_{2}' -f (Get-PerformanceFileStamp), $n, $arm.label))
    if ($TargetFps -gt 0) { $ingest += @('--target-fps', ([string]$TargetFps)) }
    if ($arm.build) { $ingest += @('--tag', $arm.build) } elseif ($git.commit) { $ingest += @('--commit', $git.commit) }
    if ($git.dirty -and -not $arm.build) { $ingest += '--dirty' }
    if ($Platform -eq 'Windows') { $ingest += @('--presentmon', $capturePath, '--process', $arm.process) } else { $ingest += @('--gfxinfo', $capturePath) }
    $noteText = $Notes
    if (-not $gateOk) { $noteText = ($noteText + ' thermal gate timed out before this run').Trim() }
    if ($noteText) { $ingest += @('--notes', $noteText) }
    Invoke-PerformanceAnalyzer -Exe $analyzer -Arguments $ingest
    $runFiles[$key] += $runJson

    if ($n -lt $order.Count) {
        Wait-PerformanceCountdown -Seconds $CooldownSeconds -Message 'Cool-down'
    }
}

# ---- Analysis --------------------------------------------------------------------------
Write-Host ''
$report = Join-Path $OutDir 'report.md'
$cmp = @('compare', '--label-a', $arms.A.label, '--label-b', $arms.B.label, '--series', 'external', '--out', $report, '--a') + $runFiles.A + @('--b') + $runFiles.B
Invoke-PerformanceAnalyzer -Exe $analyzer -Arguments $cmp
Write-PerformanceLog ("Report: {0}" -f $report)

if ($smoothFiles.A.Count -gt 0 -and $smoothFiles.B.Count -gt 0) {
    $smoothReport = Join-Path $OutDir 'report_smoothness.md'
    $cmpSmooth = @('compare', '--label-a', $arms.A.label, '--label-b', $arms.B.label, '--series', 'smoothness', '--out', $smoothReport, '--a') + $smoothFiles.A + @('--b') + $smoothFiles.B
    Invoke-PerformanceAnalyzer -Exe $analyzer -Arguments $cmpSmooth
    Write-PerformanceLog ("Smoothness report: {0}" -f $smoothReport)
}

if (-not $NoHistory) {
    $hist = @('history', '--file', $HistoryFile, '--append') + $runFiles.A + $runFiles.B
    Invoke-PerformanceAnalyzer -Exe $analyzer -Arguments $hist
}
Write-PerformanceLog 'Batch complete'
