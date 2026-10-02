<#
.SYNOPSIS
Turns the Mono runtime's GC log on or off on an Android device, or captures it from the
running GnollHack process and tabulates the collections.

.DESCRIPTION
-Enable sets the system properties debug.mono.log to gc and debug.mono.env to
MONO_LOG_LEVEL=debug|MONO_LOG_MASK=gc; -Disable clears both. Mono reads them when the app
starts, so GnollHack must be force-stopped and started again after either, before a
capture.

Without -Enable or -Disable, the script clears the device log (adb logcat -c), records
`adb logcat --pid=<pid> -v threadtime` of the running process for -Seconds into
-OutDir\gc_log.txt (adb's own errors go to gc_log_stderr.txt), and writes one row per
SGen collection line (GC_MINOR, GC_MAJOR, GC_BRIDGE, GC_TAR_BRIDGE, GC_OLD_BRIDGE and
their suffixed forms such as GC_MAJOR_SWEEP) to -OutDir\gc_events.csv, with the columns
LogTime, Tag, Reason, TimeMs, StwMs, PromotedKB, MajorSizeKB, MajorInUseKB, LosSizeKB,
BridgeMs and Raw. A field the line does not carry is left empty; BridgeMs is the sum of
every "<name> <n>ms" field of a bridge line.

A Release build may ignore the properties; DEVEL\performance\README.md, "Mono GC log
(Android)", describes the diagnostic-build fallback.

-Package defaults to the package attribute of the Android manifest
(win\win32\xpl\GnollHackM\Platforms\Android\AndroidManifest.xml). A capture stops if
that package has no running process on the device.

.EXAMPLE
.\Capture-AndroidGcLog.ps1 -Enable

.EXAMPLE
.\Capture-AndroidGcLog.ps1 -Seconds 60 -OutDir gc1

.EXAMPLE
.\Capture-AndroidGcLog.ps1 -Disable

.EXAMPLE
.\Capture-AndroidGcLog.ps1 -Package com.soundmindentertainment.gnollhack -Serial R5CT1234 -Seconds 30 -OutDir gc2
#>
[CmdletBinding(DefaultParameterSetName = 'Capture')]
param(
    [string] $Package,
    [string] $Serial,
    [string] $AdbPath,
    [Parameter(Mandatory = $true, ParameterSetName = 'Enable')] [switch] $Enable,
    [Parameter(Mandatory = $true, ParameterSetName = 'Disable')] [switch] $Disable,
    [Parameter(Mandatory = $true, ParameterSetName = 'Capture')] [int] $Seconds,
    [Parameter(Mandatory = $true, ParameterSetName = 'Capture')] [string] $OutDir
)

$ErrorActionPreference = 'Stop'
. (Join-Path $PSScriptRoot 'Common.ps1')

if ($PSCmdlet.ParameterSetName -eq 'Capture' -and $Seconds -lt 1) { throw '-Seconds must be at least 1.' }

# Parses logcat lines into one object per SGen collection line. A line in threadtime
# format ("MM-dd HH:mm:ss.fff pid tid level tag: message") gives LogTime and its message;
# any other line is taken whole as the message, with an empty LogTime. Numeric fields are
# doubles (milliseconds) or longs (KB), $null when the line does not carry them.
function ConvertFrom-MonoGcLog {
    param([string[]] $Lines)
    $invariant = [System.Globalization.CultureInfo]::InvariantCulture
    $threadtime = New-Object System.Text.RegularExpressions.Regex('^\s*(\d\d-\d\d)\s+(\d\d:\d\d:\d\d\.\d+)\s+\d+\s+\d+\s+[VDIWEFS]\s+.*?:\s(.*)$')
    $tagPattern = New-Object System.Text.RegularExpressions.Regex('\b(GC_(?:MINOR|MAJOR|TAR_BRIDGE|OLD_BRIDGE|BRIDGE)[A-Z_]*)')
    $reasonPattern = New-Object System.Text.RegularExpressions.Regex('\(([^)]*)\)')
    $timePattern = New-Object System.Text.RegularExpressions.Regex('\btime\s+(\d+(?:\.\d+)?)\s*ms')
    $stwPattern = New-Object System.Text.RegularExpressions.Regex('\bstw\s+(\d+(?:\.\d+)?)\s*ms')
    $promotedPattern = New-Object System.Text.RegularExpressions.Regex('\bpromoted\s+(\d+)K')
    $majorPattern = New-Object System.Text.RegularExpressions.Regex('major size:\s*(\d+)K(?:\s*,?\s*in use:\s*(\d+)K)?')
    $losPattern = New-Object System.Text.RegularExpressions.Regex('los size:\s*(\d+)K')
    $bridgeFieldPattern = New-Object System.Text.RegularExpressions.Regex('([A-Za-z][A-Za-z0-9_-]*)\s+(\d+(?:\.\d+)?)\s*ms\b')

    $rows = New-Object System.Collections.ArrayList
    foreach ($line in @($Lines)) {
        if ($null -eq $line) { continue }
        $logTime = ''
        $message = $line.TrimEnd()
        $tt = $threadtime.Match($line)
        if ($tt.Success) {
            $logTime = $tt.Groups[1].Value + ' ' + $tt.Groups[2].Value
            $message = $tt.Groups[3].Value.TrimEnd()
        }
        $tagMatch = $tagPattern.Match($message)
        if (-not $tagMatch.Success) { continue }
        $tag = $tagMatch.Groups[1].Value.TrimEnd('_')
        $rest = $message.Substring($tagMatch.Index + $tagMatch.Length)

        $reason = ''
        $m = $reasonPattern.Match($rest)
        if ($m.Success) { $reason = $m.Groups[1].Value.Trim() }

        $timeMs = $null
        $m = $timePattern.Match($rest)
        if ($m.Success) { $timeMs = [double]::Parse($m.Groups[1].Value, $invariant) }

        $stwMs = $null
        $m = $stwPattern.Match($rest)
        if ($m.Success) { $stwMs = [double]::Parse($m.Groups[1].Value, $invariant) }

        $promotedKB = $null
        $m = $promotedPattern.Match($rest)
        if ($m.Success) { $promotedKB = [long]::Parse($m.Groups[1].Value, $invariant) }

        $majorSizeKB = $null
        $majorInUseKB = $null
        $m = $majorPattern.Match($rest)
        if ($m.Success) {
            $majorSizeKB = [long]::Parse($m.Groups[1].Value, $invariant)
            if ($m.Groups[2].Success) { $majorInUseKB = [long]::Parse($m.Groups[2].Value, $invariant) }
        }

        $losSizeKB = $null
        $m = $losPattern.Match($rest)
        if ($m.Success) { $losSizeKB = [long]::Parse($m.Groups[1].Value, $invariant) }

        $bridgeMs = $null
        if ($tag.Contains('BRIDGE')) {
            $sum = 0.0
            $found = $false
            foreach ($f in $bridgeFieldPattern.Matches($rest)) {
                $sum += [double]::Parse($f.Groups[2].Value, $invariant)
                $found = $true
            }
            if ($found) { $bridgeMs = $sum }
        }

        [void]$rows.Add([pscustomobject]@{
            LogTime = $logTime
            Tag = $tag
            Reason = $reason
            TimeMs = $timeMs
            StwMs = $stwMs
            PromotedKB = $promotedKB
            MajorSizeKB = $majorSizeKB
            MajorInUseKB = $majorInUseKB
            LosSizeKB = $losSizeKB
            BridgeMs = $bridgeMs
            Raw = $message
        })
    }
    return $rows.ToArray()
}

# Writes the parsed rows as CSV, UTF-8 without a BOM, CRLF, every field quoted. Numbers
# use the invariant culture, so the file reads the same under any regional setting.
function Write-MonoGcEventCsv {
    param(
        [object[]] $Rows,
        [Parameter(Mandatory = $true)] [string] $Path
    )
    $invariant = [System.Globalization.CultureInfo]::InvariantCulture
    $columns = @('LogTime', 'Tag', 'Reason', 'TimeMs', 'StwMs', 'PromotedKB', 'MajorSizeKB', 'MajorInUseKB', 'LosSizeKB', 'BridgeMs', 'Raw')
    $sb = New-Object System.Text.StringBuilder
    [void]$sb.Append((($columns | ForEach-Object { '"' + $_ + '"' }) -join ',') + "`r`n")
    foreach ($row in @($Rows)) {
        if ($null -eq $row) { continue }
        $fields = New-Object System.Collections.Generic.List[string]
        foreach ($c in $columns) {
            $value = $row.$c
            $text = ''
            if ($value -is [double]) {
                $text = $value.ToString('0.###', $invariant)
            } elseif ($null -ne $value) {
                $text = [string]$value
            }
            [void]$fields.Add('"' + $text.Replace('"', '""') + '"')
        }
        [void]$sb.Append(($fields -join ',') + "`r`n")
    }
    [System.IO.File]::WriteAllText($Path, $sb.ToString(), (Get-PerformanceUtf8NoBom))
}

$adb = Resolve-PerformanceAdb -AdbPath $AdbPath
$serialArgs = @()
if ($Serial) { $serialArgs = @('-s', $Serial) }

function Invoke-Adb {
    param([string[]] $Arguments)
    $result = Invoke-PerformanceNative -Exe $adb -Arguments (@($serialArgs) + $Arguments)
    if ($result.ExitCode -ne 0) { throw "adb $($Arguments -join ' ') failed with exit code $($result.ExitCode)" }
    return $result.Output
}

# adb get-state prints nothing (rather than a non-zero exit) when no device is attached,
# and under Set-StrictMode indexing into that empty result would itself throw; check the
# count first so the "no device" case is reported clearly instead of as a strict-mode error.
$state = @(Invoke-Adb -Arguments @('get-state'))
if ($state.Count -eq 0) {
    throw 'No device connected (adb get-state returned no output).'
}
if ($state[0].Trim() -ne 'device') {
    throw "No device in 'device' state (got '$($state[0])')."
}

if ($PSCmdlet.ParameterSetName -eq 'Enable' -or $PSCmdlet.ParameterSetName -eq 'Disable') {
    $Package = (Resolve-PerformanceAndroidPackage -Package $Package -SkipRunningCheck).Package
    # adb joins its arguments into one device shell command line, so the values are
    # single-quoted for the device shell: '|' would otherwise start a pipe there.
    if ($PSCmdlet.ParameterSetName -eq 'Enable') {
        Invoke-Adb -Arguments @('shell', 'setprop debug.mono.log gc') | Out-Null
        Invoke-Adb -Arguments @('shell', "setprop debug.mono.env 'MONO_LOG_LEVEL=debug|MONO_LOG_MASK=gc'") | Out-Null
    } else {
        Invoke-Adb -Arguments @('shell', "setprop debug.mono.log ''") | Out-Null
        Invoke-Adb -Arguments @('shell', "setprop debug.mono.env ''") | Out-Null
    }
    $logValue = (@(Invoke-Adb -Arguments @('shell', 'getprop', 'debug.mono.log')) -join ' ').Trim()
    $envValue = (@(Invoke-Adb -Arguments @('shell', 'getprop', 'debug.mono.env')) -join ' ').Trim()
    Write-PerformanceLog ("debug.mono.log = '{0}'" -f $logValue)
    Write-PerformanceLog ("debug.mono.env = '{0}'" -f $envValue)
    Write-PerformanceLog 'Mono reads these properties at start-up. Force-stop GnollHack and start it again before capturing:'
    Write-Host ("    adb {0}shell am force-stop {1}" -f (($serialArgs | ForEach-Object { $_ + ' ' }) -join ''), $Package)
    return
}

$resolved = Resolve-PerformanceAndroidPackage -Package $Package -Adb $adb -SerialArgs $serialArgs
$Package = $resolved.Package
$appPid = $resolved.ProcessId
if ($null -eq $appPid) {
    throw "pidof $Package did not return a process id."
}
New-Item -ItemType Directory -Force -Path $OutDir | Out-Null
$logPath = Join-Path $OutDir 'gc_log.txt'
$errPath = Join-Path $OutDir 'gc_log_stderr.txt'
$csvPath = Join-Path $OutDir 'gc_events.csv'

$clear = Invoke-PerformanceNative -Exe $adb -Arguments (@($serialArgs) + @('logcat', '-c'))
if ($clear.ExitCode -ne 0) {
    Write-Warning ("adb logcat -c failed with exit code {0}; the log may begin with older lines of the same process." -f $clear.ExitCode)
}

# logcat streams until stopped. It runs as a process of its own, redirected to files, so
# it can be killed directly: a background job's native child outlives Stop-Job on Windows.
Write-PerformanceLog ("logcat: process {0} of {1} for {2} s -> {3}" -f $appPid, $Package, $Seconds, $logPath)
$argList = @()
foreach ($a in $serialArgs) { $argList += (ConvertTo-PerformanceQuotedArgument $a) }
$argList += @('logcat', ('--pid=' + $appPid), '-v', 'threadtime')
$logcat = $null
try {
    $logcat = Start-Process -FilePath $adb -ArgumentList $argList -WindowStyle Hidden -PassThru `
        -RedirectStandardOutput $logPath -RedirectStandardError $errPath
    if ($logcat.WaitForExit($Seconds * 1000)) {
        Write-Warning ("logcat exited early with exit code {0}; see {1}." -f $logcat.ExitCode, $errPath)
    }
} finally {
    if ($null -ne $logcat) {
        try {
            if (-not $logcat.HasExited) {
                Stop-Process -Id $logcat.Id -Force -Confirm:$false -ErrorAction Stop
                [void]$logcat.WaitForExit(5000)
            }
        } catch { }
    }
}

$lines = @()
if (Test-Path -LiteralPath $logPath) {
    $lines = [System.IO.File]::ReadAllLines((Resolve-Path -LiteralPath $logPath).ProviderPath, (Get-PerformanceUtf8NoBom))
}
$rows = @(ConvertFrom-MonoGcLog -Lines $lines)
Write-MonoGcEventCsv -Rows $rows -Path $csvPath
Write-PerformanceLog ("logcat: {0} lines, {1} collection lines -> {2}" -f $lines.Count, $rows.Count, $csvPath)

if ($rows.Count -eq 0) {
    Write-Warning ("No GC lines in the log. Either this build does not honor debug.mono.log and debug.mono.env " +
        "(a Release build may ignore them; see the diagnostic-build fallback in DEVEL\performance\README.md), " +
        "the app was not restarted after -Enable, or no collection happened during the window.")
} else {
    foreach ($group in @($rows | Group-Object -Property Tag | Sort-Object -Property Name)) {
        Write-PerformanceLog ("    {0}: {1}" -f $group.Name, $group.Count)
    }
}
