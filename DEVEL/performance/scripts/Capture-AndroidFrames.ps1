<#
.SYNOPSIS
Captures per-frame timing for an Android package with dumpsys gfxinfo, and optionally a
Perfetto trace.

.DESCRIPTION
`dumpsys gfxinfo <package> framestats` returns only the most recent frames (about 120),
so this script resets the stats, then polls framestats once per -PollSeconds for the
whole window, writing one file per poll into -OutDir (framestats_NNNN.txt). The analyzer
merges the polls and de-duplicates frames by IntendedVsync. At the end the cumulative
summary (`dumpsys gfxinfo <package>` with its jank percentage and percentile lines) is
saved as summary.txt for cross-checking.

With -Perfetto, a system trace covering the same window is recorded with the config in
DEVEL\performance\perfetto\frametimeline.pbtxt (gfx, SurfaceFlinger frame timeline,
scheduling, CPU frequency, thermal, and the app's own GH.* trace sections) and pulled to
-OutDir\trace.pftrace. Open it at https://ui.perfetto.dev for hitch attribution to
threads, frequency and thermal zones.

When trace_processor_shell is available (-TraceProcessorPath, the TRACE_PROCESSOR
environment variable, or on PATH), the trace is also exported with the queries in
DEVEL\performance\perfetto\ to perfetto_frames.csv (SurfaceFlinger's per-frame present
times and jank types) and perfetto_app_slices.csv (the app's sections with their FrameId),
which the analyzer joins to the in-app frame timeline. Frame timeline data needs Android 12
or later.

.EXAMPLE
.\Capture-AndroidFrames.ps1 -Package com.hyvanmielenpelit.gnollhack -Seconds 60 -OutDir run1 -Perfetto
#>
[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)] [string] $Package,
    [Parameter(Mandatory = $true)] [int] $Seconds,
    [Parameter(Mandatory = $true)] [string] $OutDir,
    [string] $Serial,
    [string] $AdbPath,
    [double] $PollSeconds = 1.0,
    [switch] $Perfetto,
    [string] $TraceProcessorPath
)

$ErrorActionPreference = 'Stop'
. (Join-Path $PSScriptRoot 'Common.ps1')

$adb = Resolve-PerformanceAdb -AdbPath $AdbPath
$serialArgs = @()
if ($Serial) { $serialArgs = @('-s', $Serial) }
New-Item -ItemType Directory -Force -Path $OutDir | Out-Null

function Invoke-Adb {
    param([string[]] $Arguments)
    $out = & $adb @serialArgs @Arguments 2>$null
    if ($LASTEXITCODE -ne 0) { throw "adb $($Arguments -join ' ') failed with exit code $LASTEXITCODE" }
    return $out
}

$state = Invoke-Adb -Arguments @('get-state')
if (($state | Select-Object -First 1).Trim() -ne 'device') { throw "No device in 'device' state (got '$state')." }

$perfettoDir = Join-Path $PSScriptRoot '..\perfetto'
$perfettoJob = $null
if ($Perfetto) {
    $template = [System.IO.File]::ReadAllText((Join-Path $perfettoDir 'frametimeline.pbtxt'))
    $config = $template.Replace('{PACKAGE}', $Package).Replace('{DURATION_MS}', [string](($Seconds + 2) * 1000))
    $cfgLocal = Join-Path $OutDir 'perfetto.cfg'
    [System.IO.File]::WriteAllText($cfgLocal, $config.Replace("`r`n", "`n"), (Get-PerformanceUtf8NoBom))
    Invoke-Adb -Arguments @('push', $cfgLocal, '/data/local/tmp/gnh_perfetto.cfg') | Out-Null
    Write-PerformanceLog 'Perfetto: starting trace'
    $perfettoJob = Start-Job -ScriptBlock {
        param($adb, $serialArgs)
        & $adb @serialArgs shell 'cat /data/local/tmp/gnh_perfetto.cfg | perfetto --txt -c - -o /data/misc/perfetto-traces/gnh.pftrace' 2>&1
    } -ArgumentList $adb, $serialArgs
    Start-Sleep -Seconds 1
}

Write-PerformanceLog ("gfxinfo: reset and poll {0} every {1} s for {2} s -> {3}" -f $Package, $PollSeconds, $Seconds, $OutDir)
Invoke-Adb -Arguments @('shell', 'dumpsys', 'gfxinfo', $Package, 'reset') | Out-Null

$sw = [System.Diagnostics.Stopwatch]::StartNew()
$poll = 0
$next = 0.0
while ($sw.Elapsed.TotalSeconds -lt $Seconds) {
    $now = $sw.Elapsed.TotalSeconds
    if ($now -ge $next) {
        $poll++
        $file = Join-Path $OutDir ('framestats_{0:D4}.txt' -f $poll)
        $text = & $adb @serialArgs shell dumpsys gfxinfo $Package framestats 2>$null
        [System.IO.File]::WriteAllText($file, (($text -join "`n") + "`n"), (Get-PerformanceUtf8NoBom))
        $next = $now + $PollSeconds
    }
    Start-Sleep -Milliseconds 100
}
$sw.Stop()

$summary = & $adb @serialArgs shell dumpsys gfxinfo $Package 2>$null
[System.IO.File]::WriteAllText((Join-Path $OutDir 'summary.txt'), (($summary -join "`n") + "`n"), (Get-PerformanceUtf8NoBom))
$jank = ($summary | Where-Object { $_ -match 'Janky frames' } | Select-Object -First 1)
Write-PerformanceLog ("gfxinfo: {0} polls; {1}" -f $poll, $jank)

if ($null -ne $perfettoJob) {
    Write-PerformanceLog 'Perfetto: waiting for the trace to finish'
    Wait-Job -Job $perfettoJob -Timeout 30 | Out-Null
    Receive-Job -Job $perfettoJob | ForEach-Object { Write-Verbose $_ }
    Remove-Job -Job $perfettoJob -Force
    $tracePath = Join-Path $OutDir 'trace.pftrace'
    & $adb @serialArgs pull /data/misc/perfetto-traces/gnh.pftrace $tracePath 2>$null | Out-Null
    if ($LASTEXITCODE -eq 0 -and (Test-Path -LiteralPath $tracePath)) {
        Write-PerformanceLog ("Perfetto: trace pulled to {0}" -f $tracePath)
        $tp = Resolve-PerformanceTraceProcessor -TraceProcessorPath $TraceProcessorPath
        if ($tp) {
            foreach ($query in @('export_frames', 'export_app_slices')) {
                $sqlText = [System.IO.File]::ReadAllText((Join-Path $perfettoDir ($query + '.sql'))).Replace('{PACKAGE}', $Package)
                $sqlLocal = Join-Path $OutDir ($query + '.sql')
                [System.IO.File]::WriteAllText($sqlLocal, $sqlText, (Get-PerformanceUtf8NoBom))
                $csvName = $query.Replace('export_', 'perfetto_') + '.csv'
                $csvPath = Join-Path $OutDir $csvName
                $prev = $ErrorActionPreference
                $ErrorActionPreference = 'Continue'
                $rows = & $tp -q $sqlLocal $tracePath 2>$null
                $code = $LASTEXITCODE
                $ErrorActionPreference = $prev
                if ($code -eq 0) {
                    [System.IO.File]::WriteAllText($csvPath, (($rows -join "`n") + "`n"), (Get-PerformanceUtf8NoBom))
                    Write-PerformanceLog ("Perfetto: {0} rows -> {1}" -f (@($rows).Count - 1), $csvName)
                } else {
                    Write-Warning ("trace_processor_shell failed on {0} (exit code {1})." -f $query, $code)
                }
            }
        } else {
            Write-PerformanceLog 'Perfetto: trace_processor_shell not found; skipping the CSV export'
        }
    } else {
        Write-Warning 'Perfetto trace could not be pulled. Check that the device allows perfetto (Android 9+ with the traced service enabled).'
    }
}
