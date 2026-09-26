<#
.SYNOPSIS
Captures presentation timing for one process with Intel PresentMon.

.DESCRIPTION
Runs the PresentMon console tool against a process name for a fixed number of seconds
and writes its per-frame CSV. PresentMon observes the OS presentation path, so the
resulting MsBetweenPresents / MsBetweenDisplayChange columns are the ground truth the
plan's external series is built on, independent of anything the app measures itself.

PresentMon 2.x needs to run elevated for its ETW session on most machines; 1.x console
builds generally do too. The script tells you if the tool exited non-zero.

.EXAMPLE
.\Capture-PresentMon.ps1 -ProcessName GnollHackM -Seconds 60 -OutCsv run1.csv
#>
[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)] [string] $ProcessName,
    [Parameter(Mandatory = $true)] [int] $Seconds,
    [Parameter(Mandatory = $true)] [string] $OutCsv,
    [string] $PresentMonPath,
    [int] $DelaySeconds = 0
)

$ErrorActionPreference = 'Stop'
. (Join-Path $PSScriptRoot 'Common.ps1')

$pm = Resolve-PerformancePresentMon -PresentMonPath $PresentMonPath
$exe = $ProcessName
if (-not $exe.ToLower().EndsWith('.exe')) { $exe = $exe + '.exe' }

if (Test-Path -LiteralPath $OutCsv) { Remove-Item -LiteralPath $OutCsv -Force }
$outDir = Split-Path -Parent $OutCsv
if ($outDir -and -not (Test-Path -LiteralPath $outDir)) { New-Item -ItemType Directory -Force -Path $outDir | Out-Null }

# Argument names differ between PresentMon 1.x and 2.x; both accept these forms.
# --qpc_time adds raw QPC timestamps, which are the app's Stopwatch ticks on Windows, so
# each present can be joined to the in-app frame timeline by time.
$pmArgs = @(
    '--process_name', $exe,
    '--output_file', $OutCsv,
    '--timed', $Seconds,
    '--terminate_after_timed',
    '--no_console_stats',
    '--stop_existing_session',
    '--qpc_time'
)
if ($DelaySeconds -gt 0) { $pmArgs += @('--delay', $DelaySeconds) }

Write-PerformanceLog ("PresentMon: {0} for {1} s -> {2}" -f $exe, $Seconds, $OutCsv)
$prev = $ErrorActionPreference
$ErrorActionPreference = 'Continue'
& $pm @pmArgs 2>&1 | ForEach-Object { Write-Verbose $_ }
$code = $LASTEXITCODE
$ErrorActionPreference = $prev

if ($code -ne 0) {
    throw "PresentMon exited with code $code. It usually needs an elevated console. Try running the script as administrator."
}
if (-not (Test-Path -LiteralPath $OutCsv)) {
    throw "PresentMon produced no CSV at $OutCsv. Was $exe running and presenting frames?"
}
$lines = (Get-Content -LiteralPath $OutCsv -Encoding UTF8 | Measure-Object -Line).Lines
Write-PerformanceLog ("PresentMon: {0} rows captured" -f ($lines - 1))
if ($lines -lt 100) { Write-Warning 'Fewer than 100 frames captured; check the process name and that the window was presenting.' }
