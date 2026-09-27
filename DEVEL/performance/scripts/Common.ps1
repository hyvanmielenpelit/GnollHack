# Shared helpers for the GnollHack performance scripts. Dot-source from the others:
#   . (Join-Path $PSScriptRoot 'Common.ps1')
# Windows PowerShell 5.1 compatible: no &&, ||, ternary, or null-coalescing.

Set-StrictMode -Version 2.0

function Get-PerformanceUtf8NoBom {
    return New-Object System.Text.UTF8Encoding($false)
}

function Write-PerformanceJson {
    param(
        [Parameter(Mandatory = $true)] $Object,
        [Parameter(Mandatory = $true)] [string] $Path
    )
    $json = ConvertTo-Json -InputObject $Object -Depth 20
    [System.IO.File]::WriteAllText($Path, $json + "`r`n", (Get-PerformanceUtf8NoBom))
}

function Write-PerformanceLog {
    param([string] $Message)
    $stamp = Get-Date -Format 'HH:mm:ss'
    Write-Host ("[{0}] {1}" -f $stamp, $Message)
}

function Get-PerformanceUtcStamp {
    return (Get-Date).ToUniversalTime().ToString('o')
}

function Get-PerformanceFileStamp {
    return (Get-Date).ToString('yyyyMMdd_HHmmss')
}

# Runs a native executable without tripping the Stop-preference stderr crash: a native
# command that writes to stderr and exits 0 throws NativeCommandError when the caller's
# $ErrorActionPreference is 'Stop' and the call redirects stderr (2>&1 or 2>$null). This
# sets 'Continue' for the duration of the call only, splits the merged stream back into
# stdout lines and stderr lines, and restores the caller's preference even if the call
# throws for an unrelated reason (e.g. the executable does not exist).
function Invoke-PerformanceNative {
    param(
        [Parameter(Mandatory = $true)] [string] $Exe,
        [Parameter(Mandatory = $true)] [string[]] $Arguments
    )
    $previousPreference = $ErrorActionPreference
    $ErrorActionPreference = 'Continue'
    $stdout = New-Object System.Collections.ArrayList
    $stderr = New-Object System.Collections.ArrayList
    try {
        $raw = & $Exe @Arguments 2>&1
        foreach ($item in @($raw)) {
            if ($item -is [System.Management.Automation.ErrorRecord]) {
                [void]$stderr.Add($item.ToString())
            } else {
                [void]$stdout.Add([string]$item)
            }
        }
        $code = $LASTEXITCODE
    } finally {
        $ErrorActionPreference = $previousPreference
    }
    return @{
        Output   = [string[]]@($stdout.ToArray())
        Stderr   = [string[]]@($stderr.ToArray())
        ExitCode = [int]$code
    }
}

# Resolves adb: explicit path, then PATH, then the known SDK locations. Throws with an
# install hint when none exists, so a missing tool never degrades silently.
function Resolve-PerformanceAdb {
    param([string] $AdbPath)
    $candidates = @()
    if ($AdbPath) { $candidates += $AdbPath }
    $onPath = Get-Command adb -ErrorAction SilentlyContinue
    if ($null -ne $onPath) { $candidates += $onPath.Source }
    $candidates += 'C:\Program Files (x86)\Android\android-sdk\platform-tools\adb.exe'
    if ($env:ANDROID_HOME) { $candidates += (Join-Path $env:ANDROID_HOME 'platform-tools\adb.exe') }
    if ($env:ANDROID_SDK_ROOT) { $candidates += (Join-Path $env:ANDROID_SDK_ROOT 'platform-tools\adb.exe') }
    if ($env:LOCALAPPDATA) { $candidates += (Join-Path $env:LOCALAPPDATA 'Android\Sdk\platform-tools\adb.exe') }
    foreach ($c in $candidates) {
        if ($c -and (Test-Path -LiteralPath $c)) { return $c }
    }
    throw 'adb not found. Install Android SDK platform-tools, or pass -AdbPath <path to adb.exe>.'
}

# Resolves PresentMon: explicit path, PRESENTMON_PATH, then PATH.
function Resolve-PerformancePresentMon {
    param([string] $PresentMonPath)
    $candidates = @()
    if ($PresentMonPath) { $candidates += $PresentMonPath }
    if ($env:PRESENTMON_PATH) { $candidates += $env:PRESENTMON_PATH }
    foreach ($name in @('PresentMon', 'PresentMon-2.3.1-x64', 'PresentMon-x64', 'presentmon')) {
        $cmd = Get-Command $name -ErrorAction SilentlyContinue
        if ($null -ne $cmd) { $candidates += $cmd.Source }
    }
    foreach ($c in $candidates) {
        if ($c -and (Test-Path -LiteralPath $c)) { return $c }
    }
    throw 'PresentMon not found. Download the console build from https://github.com/GameTechDev/PresentMon/releases, then pass -PresentMonPath <exe> or set PRESENTMON_PATH.'
}

# Returns the path of Perfetto's trace_processor_shell, or $null when it is not installed;
# the Perfetto CSV export is optional. Download: https://get.perfetto.dev/trace_processor
function Resolve-PerformanceTraceProcessor {
    param([string] $TraceProcessorPath)
    $candidates = @()
    if ($TraceProcessorPath) { $candidates += $TraceProcessorPath }
    if ($env:TRACE_PROCESSOR) { $candidates += $env:TRACE_PROCESSOR }
    foreach ($name in @('trace_processor_shell', 'trace_processor_shell.exe', 'trace_processor')) {
        $cmd = Get-Command $name -ErrorAction SilentlyContinue
        if ($null -ne $cmd) { $candidates += $cmd.Source }
    }
    foreach ($c in $candidates) {
        if ($c -and (Test-Path -LiteralPath $c)) { return $c }
    }
    return $null
}

# Resolves the analyzer executable, building it once if needed. Returns a hashtable with
# Exe (path or $null) and Project (csproj path) so callers can fall back to dotnet run.
function Resolve-PerformanceAnalyzer {
    param([string] $RepoRoot)
    $proj = Join-Path $RepoRoot 'win\win32\xpl\GnollHackTests\GnollHack.PerformanceAnalyzer\GnollHack.PerformanceAnalyzer.csproj'
    if (-not (Test-Path -LiteralPath $proj)) { throw "Analyzer project not found at $proj" }
    $exe = Join-Path $RepoRoot 'win\win32\xpl\GnollHackTests\GnollHack.PerformanceAnalyzer\bin\Release\net10.0\GnollHack.PerformanceAnalyzer.exe'
    if (-not (Test-Path -LiteralPath $exe)) {
        Write-PerformanceLog 'Building GnollHack.PerformanceAnalyzer (Release)'
        & dotnet build $proj -c Release --nologo -v q | Out-Null
        if ($LASTEXITCODE -ne 0) { throw "Analyzer build failed with exit code $LASTEXITCODE" }
    }
    if (-not (Test-Path -LiteralPath $exe)) { throw "Analyzer executable not found after build: $exe" }
    return $exe
}

function Invoke-PerformanceAnalyzer {
    param(
        [Parameter(Mandatory = $true)] [string] $Exe,
        [Parameter(Mandatory = $true)] [string[]] $Arguments
    )
    & $Exe @Arguments
    if ($LASTEXITCODE -ne 0) { throw "Analyzer failed (exit $LASTEXITCODE): $($Arguments -join ' ')" }
}

# Git facts for the record. Never throws: a missing git, or any single failing call, is
# reported as unknown rather than skipping the rest of the facts.
function Get-PerformanceGitFacts {
    param([string] $RepoRoot)
    $facts = @{ commit = $null; dirty = $false; tag = $null; branch = $null }
    $git = Get-Command git -ErrorAction SilentlyContinue
    if ($null -eq $git) { return $facts }

    try {
        $r = Invoke-PerformanceNative -Exe $git.Source -Arguments @('-C', $RepoRoot, 'rev-parse', 'HEAD')
        if ($r.ExitCode -eq 0 -and $r.Output.Count -gt 0) { $facts.commit = $r.Output[0] }
    } catch { }

    try {
        $r = Invoke-PerformanceNative -Exe $git.Source -Arguments @('-C', $RepoRoot, 'rev-parse', '--abbrev-ref', 'HEAD')
        if ($r.ExitCode -eq 0 -and $r.Output.Count -gt 0) { $facts.branch = $r.Output[0] }
    } catch { }

    try {
        $r = Invoke-PerformanceNative -Exe $git.Source -Arguments @('-C', $RepoRoot, 'describe', '--tags', '--exact-match')
        if ($r.ExitCode -eq 0 -and $r.Output.Count -gt 0) { $facts.tag = $r.Output[0] }
    } catch { }

    try {
        $r = Invoke-PerformanceNative -Exe $git.Source -Arguments @('-C', $RepoRoot, 'status', '--porcelain')
        if ($r.ExitCode -eq 0 -and $r.Output.Count -gt 0) { $facts.dirty = $true }
    } catch { }

    return $facts
}

# True on an elevated (Run as administrator) console; PresentMon's ETW session and some
# thermal signals need one.
function Test-PerformanceElevated {
    try {
        $id = [Security.Principal.WindowsIdentity]::GetCurrent()
        return (New-Object Security.Principal.WindowsPrincipal($id)).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)
    } catch {
        return $false
    }
}

# Counts down on the console without Read-Host, so the script never blocks on stdin.
function Wait-PerformanceCountdown {
    param([int] $Seconds, [string] $Message)
    if ($Seconds -le 0) { return }
    Write-PerformanceLog ("{0} ({1} s)" -f $Message, $Seconds)
    $remaining = $Seconds
    while ($remaining -gt 0) {
        $step = [Math]::Min(5, $remaining)
        Start-Sleep -Seconds $step
        $remaining -= $step
        if ($remaining -gt 0 -and ($remaining % 10) -eq 0) { Write-Host ("    {0} s" -f $remaining) }
    }
}

# True when Windows has a reboot pending (Windows Update or component servicing), $null
# when the registry cannot be read.
function Test-PerformancePendingReboot {
    try {
        $keys = @(
            'HKLM:\SOFTWARE\Microsoft\Windows\CurrentVersion\WindowsUpdate\Auto Update\RebootRequired',
            'HKLM:\SOFTWARE\Microsoft\Windows\CurrentVersion\Component Based Servicing\RebootPending'
        )
        foreach ($k in $keys) {
            if (Test-Path -LiteralPath $k) { return $true }
        }
        return $false
    } catch {
        return $null
    }
}

function ConvertTo-PerformanceQuotedArgument {
    param([string] $Value)
    return ('"' + $Value + '"')
}

function Get-PerformanceTypeperfPath {
    $candidate = Join-Path $env:SystemRoot 'System32\typeperf.exe'
    if (Test-Path -LiteralPath $candidate) { return $candidate }
    return 'typeperf'
}

# Starts the background load sampler for one capture and returns a handle for
# Stop-PerformanceLoadSampler. Never throws: a sampler that cannot start is reported with
# a warning and the handle carries the error.
#   Windows  load_system.csv    a hidden typeperf process reading load_system_counters.txt,
#                               1 s interval, Seconds + 5 samples (the first data row is
#                               blank for rate counters):
#                               \Processor(_Total)\% Processor Time,
#                               \Process(<ProcessName>)\% Processor Time,
#                               \PhysicalDisk(_Total)\% Idle Time, \Memory\Available MBytes,
#                               \Memory\Pages Input/sec
#            load_processes.csv a background job running Get-Counter on
#                               \Process(*)\% Processor Time and
#                               \GPU Engine(*)\Utilization Percentage with one sample of
#                               Seconds, written in typeperf's CSV shape: a header row
#                               ("(PDH-CSV 4.0)", then each counter path) and one data row
#                               (timestamp, then the per-process averages over the interval)
#   Android  one adb shell loop writing load_android.txt, one block per second:
#              T <device epoch ms>
#              cpu  ... (first line of /proc/stat)
#              P <contents of /proc/<pid>/stat>
#              M MemAvailable: ...
#              L MemTotal: ...
#            When /proc/stat is not readable, the file starts with "FALLBACK top" followed
#            by the raw output of top -b -d 1 -n <Seconds + 5>.
function Start-PerformanceLoadSampler {
    param(
        [Parameter(Mandatory = $true)] [string] $Dir,
        [Parameter(Mandatory = $true)] [int] $Seconds,
        [switch] $Android,
        [string] $Serial,
        [string] $AdbPath,
        [string] $ProcessName,
        [string] $PackageName
    )
    $handle = [pscustomobject]@{
        Processes = New-Object System.Collections.ArrayList
        Jobs = New-Object System.Collections.ArrayList
        Files = New-Object System.Collections.ArrayList
        Error = $null
    }
    $sampleCount = [Math]::Max(1, $Seconds) + 5

    if ($Android) {
        try {
            $adb = Resolve-PerformanceAdb -AdbPath $AdbPath
            $serialArgs = @()
            if ($Serial) { $serialArgs = @('-s', $Serial) }
            $appPid = ''
            if ($PackageName) {
                $pidCall = Invoke-PerformanceNative -Exe $adb -Arguments (@($serialArgs) + @('shell', 'pidof', $PackageName))
                if ($pidCall.ExitCode -eq 0 -and $pidCall.Output.Count -gt 0) {
                    $tokens = @(($pidCall.Output[0]).Trim() -split '\s+')
                    if ($tokens.Count -gt 0 -and $tokens[0] -match '^\d+$') { $appPid = $tokens[0] }
                }
            }
            # One line for sh -c on the device, free of double quotes and backslashes so it
            # survives the Windows command line as a single quoted argument. The timestamp
            # falls back to whole seconds when date has no %N.
            $remote = 'N=' + $sampleCount + '; P=' + $appPid + '; ' +
                'if head -n 1 /proc/stat >/dev/null 2>&1; then ' +
                'i=0; while [ $i -lt $N ]; do ' +
                't=$(date +%s%3N 2>/dev/null); ' +
                'case x$t in x|x*[!0-9]*) t=$(date +%s)000;; esac; ' +
                'if [ ${#t} -lt 13 ]; then t=$(date +%s)000; fi; ' +
                'echo T $t; head -n 1 /proc/stat; ' +
                'printf ''P ''; cat /proc/$P/stat 2>/dev/null || echo; ' +
                'printf ''M ''; grep MemAvailable /proc/meminfo || echo; ' +
                'printf ''L ''; grep MemTotal /proc/meminfo || echo; ' +
                'i=$((i+1)); sleep 1; done; ' +
                'else echo FALLBACK top; top -b -d 1 -n $N; fi'
            $outPath = Join-Path $Dir 'load_android.txt'
            $errPath = Join-Path $Dir 'load_android_stderr.txt'
            $argList = @()
            foreach ($a in $serialArgs) { $argList += (ConvertTo-PerformanceQuotedArgument $a) }
            $argList += 'shell'
            $argList += (ConvertTo-PerformanceQuotedArgument $remote)
            $p = Start-Process -FilePath $adb -ArgumentList $argList -WindowStyle Hidden -PassThru `
                -RedirectStandardOutput $outPath -RedirectStandardError $errPath
            [void]$handle.Processes.Add($p)
            [void]$handle.Files.Add($outPath)
        } catch {
            $handle.Error = $_.Exception.Message
            Write-Warning ("Background load sampler did not start on the device: {0}" -f $handle.Error)
        }
        return $handle
    }

    $typeperf = Get-PerformanceTypeperfPath
    $utf8 = Get-PerformanceUtf8NoBom
    try {
        $systemCounters = @('\Processor(_Total)\% Processor Time')
        if ($ProcessName) { $systemCounters += ('\Process(' + $ProcessName + ')\% Processor Time') }
        $systemCounters += @('\PhysicalDisk(_Total)\% Idle Time', '\Memory\Available MBytes', '\Memory\Pages Input/sec')
        $systemCf = Join-Path $Dir 'load_system_counters.txt'
        [System.IO.File]::WriteAllText($systemCf, (($systemCounters -join "`r`n") + "`r`n"), $utf8)
        $systemOut = Join-Path $Dir 'load_system.csv'
        $argList = @('-cf', (ConvertTo-PerformanceQuotedArgument $systemCf), '-si', '1', '-sc', ([string]$sampleCount),
            '-f', 'CSV', '-y', '-o', (ConvertTo-PerformanceQuotedArgument $systemOut))
        $p = Start-Process -FilePath $typeperf -ArgumentList $argList -WindowStyle Hidden -PassThru
        [void]$handle.Processes.Add($p)
        [void]$handle.Files.Add($systemOut)
    } catch {
        $handle.Error = $_.Exception.Message
        Write-Warning ("System load sampler did not start: {0}" -f $handle.Error)
    }

    try {
        $processOut = Join-Path $Dir 'load_processes.csv'
        # Get-Counter with one sample of -SampleInterval Seconds returns values averaged over
        # that interval. The file is written under a temporary name and renamed, so a job
        # stopped early leaves no partial load_processes.csv.
        $job = Start-Job -ArgumentList ([Math]::Max(1, $Seconds)), $processOut -ScriptBlock {
            param([int] $Interval, [string] $OutPath)
            $ErrorActionPreference = 'Stop'
            $counters = @('\Process(*)\% Processor Time', '\GPU Engine(*)\Utilization Percentage')
            $data = @(Get-Counter -Counter $counters -SampleInterval $Interval -MaxSamples 1 -ErrorAction SilentlyContinue)
            if ($data.Count -eq 0) { return }
            $samples = @($data[0].CounterSamples)
            if ($samples.Count -eq 0) { return }
            $invariant = [System.Globalization.CultureInfo]::InvariantCulture
            $header = New-Object System.Collections.Generic.List[string]
            $row = New-Object System.Collections.Generic.List[string]
            [void]$header.Add('"(PDH-CSV 4.0)"')
            [void]$row.Add('"' + $data[0].Timestamp.ToString('MM/dd/yyyy HH:mm:ss.fff', $invariant) + '"')
            foreach ($s in $samples) {
                [void]$header.Add('"' + $s.Path.Replace('"', '""') + '"')
                [void]$row.Add('"' + ([double]$s.CookedValue).ToString('F6', $invariant) + '"')
            }
            $text = ($header -join ',') + "`r`n" + ($row -join ',') + "`r`n"
            $tmp = $OutPath + '.partial'
            [System.IO.File]::WriteAllText($tmp, $text, (New-Object System.Text.UTF8Encoding($false)))
            if (Test-Path -LiteralPath $OutPath) { Remove-Item -LiteralPath $OutPath -Force -Confirm:$false }
            [System.IO.File]::Move($tmp, $OutPath)
        }
        [void]$handle.Jobs.Add($job)
        [void]$handle.Files.Add($processOut)
    } catch {
        $message = $_.Exception.Message
        if ($null -eq $handle.Error) { $handle.Error = $message } else { $handle.Error = $handle.Error + '; ' + $message }
        Write-Warning ("Process load sampler did not start: {0}" -f $message)
    }
    return $handle
}

# Stops the sampler processes and jobs: waits up to 10 s in total for them to finish on
# their own, then kills the processes and stops and removes the jobs. Safe to call more
# than once and with a $null handle; never throws.
function Stop-PerformanceLoadSampler {
    param($Handle)
    try {
        if ($null -eq $Handle) { return }
        $sw = [System.Diagnostics.Stopwatch]::StartNew()
        if ($null -ne $Handle.PSObject.Properties['Processes']) {
            foreach ($p in @($Handle.Processes)) {
                if ($null -eq $p) { continue }
                try {
                    if (-not $p.HasExited) {
                        $remainingMs = [int][Math]::Max(0, 10000 - $sw.ElapsedMilliseconds)
                        [void]$p.WaitForExit($remainingMs)
                    }
                    if (-not $p.HasExited) {
                        Stop-Process -Id $p.Id -Force -Confirm:$false -ErrorAction Stop
                    }
                } catch { }
            }
        }
        if ($null -ne $Handle.PSObject.Properties['Jobs']) {
            foreach ($j in @($Handle.Jobs)) {
                if ($null -eq $j) { continue }
                try {
                    $remainingSeconds = [int][Math]::Ceiling([Math]::Max(0, 10000 - $sw.ElapsedMilliseconds) / 1000.0)
                    if ($remainingSeconds -gt 0) { [void](Wait-Job -Job $j -Timeout $remainingSeconds -ErrorAction Stop) }
                } catch { }
                try { Stop-Job -Job $j -Confirm:$false -ErrorAction Stop } catch { }
                try { Remove-Job -Job $j -Force -Confirm:$false -ErrorAction Stop } catch { }
            }
            $Handle.Jobs.Clear()
        }
    } catch { }
}

# Samples whole-machine load for -Seconds (1 s samples, averaged) and returns
# { OtherCpuPct, DiskBusyPct, Quiet }. Quiet means other CPU < 10 % and disk busy < 50 %;
# disk is ignored when its counter is unavailable, and Quiet is $true when no CPU reading
# was possible, so a missing counter never holds the gate.
#   Windows  other CPU = \Processor(_Total)\% Processor Time minus
#            \Process(<ProcessName>)\% Processor Time / logical processors when that
#            process is running, clamped at 0; disk busy = 100 - \PhysicalDisk(_Total)\% Idle Time.
#   Android  /proc/stat delta over -Seconds, minus the package's /proc/<pid>/stat delta
#            when it is running; DiskBusyPct stays $null.
function Test-PerformanceQuiet {
    param(
        [int] $Seconds = 5,
        [string] $ProcessName,
        [switch] $Android,
        [string] $Serial,
        [string] $AdbPath,
        [string] $PackageName
    )
    $otherCpu = $null
    $diskBusy = $null
    $samples = [Math]::Max(1, $Seconds)

    if ($Android) {
        try {
            $adb = Resolve-PerformanceAdb -AdbPath $AdbPath
            $serialArgs = @()
            if ($Serial) { $serialArgs = @('-s', $Serial) }
            $pidPart = 'P='
            if ($PackageName) { $pidPart = 'set -- $(pidof ' + $PackageName + '); P=$1' }
            $remote = $pidPart + '; head -n 1 /proc/stat; printf ''P ''; cat /proc/$P/stat 2>/dev/null || echo; ' +
                'sleep ' + $samples + '; head -n 1 /proc/stat; printf ''P ''; cat /proc/$P/stat 2>/dev/null || echo'
            $call = Invoke-PerformanceNative -Exe $adb -Arguments (@($serialArgs) + @('shell', $remote))
            $cpuLines = @($call.Output | Where-Object { $_ -match '^cpu\s' })
            $procLines = @($call.Output | Where-Object { $_ -match '^P ' })
            if ($cpuLines.Count -ge 2) {
                $first = @(($cpuLines[0].Trim() -split '\s+') | Select-Object -Skip 1 | ForEach-Object { [double]$_ })
                $second = @(($cpuLines[1].Trim() -split '\s+') | Select-Object -Skip 1 | ForEach-Object { [double]$_ })
                $total = 0.0
                $idle = 0.0
                $count = [Math]::Min($first.Count, $second.Count)
                for ($i = 0; $i -lt $count; $i++) {
                    $d = $second[$i] - $first[$i]
                    # Fields 9 and 10 (guest, guest_nice) are already counted in user and nice
                    if ($i -lt 8) { $total += $d }
                    if ($i -eq 3 -or $i -eq 4) { $idle += $d }
                }
                if ($total -gt 0) {
                    $busyPct = 100.0 * ($total - $idle) / $total
                    $ownPct = 0.0
                    if ($procLines.Count -ge 2) {
                        $ownTicks = @()
                        foreach ($line in @($procLines[0], $procLines[1])) {
                            $close = $line.LastIndexOf(')')
                            if ($close -lt 0) { continue }
                            $fields = @($line.Substring($close + 1).Trim() -split '\s+')
                            # utime and stime are fields 14 and 15 of /proc/<pid>/stat
                            if ($fields.Count -ge 13) { $ownTicks += ([double]$fields[11] + [double]$fields[12]) }
                        }
                        if ($ownTicks.Count -eq 2 -and $ownTicks[1] -ge $ownTicks[0]) {
                            $ownPct = 100.0 * ($ownTicks[1] - $ownTicks[0]) / $total
                        }
                    }
                    $otherCpu = [Math]::Max(0.0, $busyPct - $ownPct)
                }
            }
        } catch { }
    } else {
        $cpuCount = [Environment]::ProcessorCount
        if ($cpuCount -lt 1) { $cpuCount = 1 }
        $ownRunning = $false
        if ($ProcessName) {
            $ownRunning = ($null -ne (Get-Process -Name $ProcessName -ErrorAction SilentlyContinue | Select-Object -First 1))
        }
        $total = '\Processor(_Total)\% Processor Time'
        $disk = '\PhysicalDisk(_Total)\% Idle Time'
        $own = $null
        if ($ownRunning) { $own = '\Process(' + $ProcessName + ')\% Processor Time' }
        # Narrower counter sets are tried when one path is unavailable, because a single
        # invalid path fails the whole Get-Counter call.
        $attempts = @()
        if ($null -ne $own) { $attempts += ,@($total, $own, $disk); $attempts += ,@($total, $own) }
        $attempts += ,@($total, $disk)
        $attempts += ,@($total)
        foreach ($set in $attempts) {
            try {
                $data = Get-Counter -Counter $set -SampleInterval 1 -MaxSamples $samples -ErrorAction Stop
            } catch {
                continue
            }
            $sys = @(); $ownValues = @(); $idleValues = @()
            foreach ($s in @($data)) {
                foreach ($c in $s.CounterSamples) {
                    if ($c.Path -like '*\processor(_total)\% processor time') { $sys += $c.CookedValue }
                    elseif ($c.Path -like '*\process(*)\% processor time') { $ownValues += $c.CookedValue }
                    elseif ($c.Path -like '*\physicaldisk(_total)\% idle time') { $idleValues += $c.CookedValue }
                }
            }
            if ($sys.Count -eq 0) { continue }
            $sysAvg = ($sys | Measure-Object -Average).Average
            $ownAvg = 0.0
            if ($ownValues.Count -gt 0) { $ownAvg = (($ownValues | Measure-Object -Average).Average) / $cpuCount }
            $otherCpu = [Math]::Max(0.0, $sysAvg - $ownAvg)
            if ($idleValues.Count -gt 0) {
                $idleAvg = ($idleValues | Measure-Object -Average).Average
                $diskBusy = [Math]::Min(100.0, [Math]::Max(0.0, 100.0 - $idleAvg))
            }
            break
        }
    }

    $quiet = $true
    if ($null -ne $otherCpu) {
        $quiet = ($otherCpu -lt 10.0)
        if ($null -ne $diskBusy -and $diskBusy -ge 50.0) { $quiet = $false }
        $otherCpu = [Math]::Round($otherCpu, 1)
    }
    if ($null -ne $diskBusy) { $diskBusy = [Math]::Round($diskBusy, 1) }
    return [pscustomobject]@{
        OtherCpuPct = $otherCpu
        DiskBusyPct = $diskBusy
        Quiet = $quiet
    }
}
