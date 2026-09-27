<#
.SYNOPSIS
Reads the thermal and power state of the Windows host, or of an Android device, as JSON.

.DESCRIPTION
Emits one JSON object with the fields the analyzer's EnvJson reader expects:
status, headroomFraction, batteryTempC, cpuPackageTempC, gpuTempC, cpuPerformancePct,
cpuFrequencyMHz, isCharging, isLowPower, powerPlan, timestampUtc, plus source-specific
extras. A field stays null when its sensor is missing or its query failed; nothing is
guessed.

Windows extras: cpuUtilizationPct, refreshHz, gpuClockMHz, gpuThrottleReasons, elevated,
topProcesses, and the background-load fields diskBusyPct, availableMemoryMB,
availableMemoryPct, hardFaultsPerSec, pendingReboot and activities. Android extras:
statusCode, thermalZones, loadAverage and topProcesses.

Null on Windows: status (stays "Unknown"; there is no thermal status API for user code,
so the analyzer's gate uses cpuPerformancePct instead), headroomFraction, batteryTempC,
and isLowPower (no equivalent signal is read here). cpuPackageTempC, gpuTempC /
gpuClockMHz / gpuThrottleReasons, cpuPerformancePct / cpuFrequencyMHz /
cpuUtilizationPct, powerPlan and refreshHz are each null only when their own query fails
or the sensor is absent (a thermal zone, an NVIDIA GPU). diskBusyPct,
availableMemoryMB, hardFaultsPerSec are null when their counter is unavailable
(counterError lists the failing paths); availableMemoryPct also when the total memory
query fails; pendingReboot when the registry cannot be read. topProcesses and
activities are empty lists when the per-process counter is unavailable (topProcesses then
falls back to lifetime cpuSeconds).

Null on Android: cpuPerformancePct and headroomFraction (not measured on this platform).
cpuPackageTempC and gpuTempC are null unless a matching thermal zone is reported;
batteryTempC, isLowPower and cpuFrequencyMHz are null when their dumpsys or sysfs read
fails; status stays "Unknown" unless dumpsys thermalservice reports a code. loadAverage
is null and topProcesses empty when dumpsys cpuinfo fails (cpuinfoError says why).

Windows signals, in order of value:
  cpuPerformancePct  \Processor Information(_Total)\% Processor Performance sampled over
                     -SampleSeconds. Sustained values under 100 at load indicate frequency
                     throttling; the analyzer's gate uses 90 as the floor. Needs no
                     elevation.
  cpuPackageTempC    MSAcpi_ThermalZoneTemperature (root\wmi), usually needs elevation and
                     is absent on many laptops; null otherwise.
  gpuTempC, gpuClockMHz, gpuThrottleReasons  from nvidia-smi when present.
  powerPlan, isCharging, refreshHz.
  Background load, from the same single Get-Counter sample as cpuPerformancePct:
    diskBusyPct        100 - \PhysicalDisk(_Total)\% Idle Time, averaged.
    availableMemoryMB  \Memory\Available MBytes, minimum over the sample;
                       availableMemoryPct is its share of Win32_OperatingSystem
                       TotalVisibleMemorySize.
    hardFaultsPerSec   \Memory\Pages Input/sec, averaged.
    topProcesses       top 5 of \Process(*)\% Processor Time as { name, cpuPct }, cpuPct
                       normalized by the logical processor count.
    activities         known background activities as { category, processes, cpuPct },
                       cpuPct normalized the same way; categories "other" and
                       "measurement" and those under 0.1 % are left out.
  pendingReboot      true when the Windows Update RebootRequired or the Component Based
                     Servicing RebootPending registry key exists.

Android (-Android, optional -Serial): dumpsys thermalservice (status 0..6 mapped to
Nominal..Critical), dumpsys battery (temperature, charging), dumpsys cpuinfo (loadAverage
as [1 min, 5 min, 15 min]; topProcesses as the first five { name, cpuPct } entries, over
the tracker's last update period).

.EXAMPLE
.\Get-ThermalState.ps1 -OutFile before.json
.\Get-ThermalState.ps1 -Android -OutFile before.json
#>
[CmdletBinding()]
param(
    [switch] $Android,
    [string] $Serial,
    [string] $AdbPath,
    [int] $SampleSeconds = 3,
    [string] $OutFile
)

$ErrorActionPreference = 'Stop'
. (Join-Path $PSScriptRoot 'Common.ps1')

# Known background activity by process name (case-insensitive exact match, instance
# suffix #N stripped). Mirrors the table in GHBackgroundLoad.cs
# (win\win32\xpl\GnollHackX\GnollHackX\Performance\GHBackgroundLoad.cs) and the plan's
# data contract; keep the two identical. Unknown names are "other".
$KnownProcessCategories = [ordered]@{
    'windows-update' = @('TiWorker', 'TrustedInstaller', 'MoUsoCoreWorker', 'usocoreworker', 'wuauclt', 'WaaSMedicAgent', 'SIHClient', 'msiexec')
    'antivirus' = @('MsMpEng', 'MpCmdRun', 'NisSrv', 'MpDefenderCoreService')
    'indexer' = @('SearchIndexer', 'SearchProtocolHost', 'SearchFilterHost')
    'wsl-vm' = @('vmmem', 'vmmemWSL', 'VmmemWSA', 'vmwp')
    'build-tools' = @('devenv', 'MSBuild', 'VBCSCompiler', 'cl', 'link', 'clang', 'lld-link', 'dotnet', 'ServiceHub.Host.dotnet.x64', 'ServiceHub.RoslynCodeAnalysisService')
    'sync' = @('OneDrive', 'Dropbox', 'GoogleDriveFS')
    'telemetry' = @('CompatTelRunner', 'DiagTrack')
    'measurement' = @('PresentMon', 'typeperf', 'powershell', 'pwsh', 'adb')
}

# Groups per-process CPU (already normalized by the logical processor count) into known
# activities. Categories "other" and "measurement" are left out, as are categories whose
# processes sum to under 0.1 %. Returns entries { category, processes, cpuPct }, busiest first.
function Get-KnownActivities {
    param([hashtable] $CpuByName)
    $lookup = @{}
    foreach ($category in $KnownProcessCategories.Keys) {
        foreach ($name in $KnownProcessCategories[$category]) { $lookup[$name] = @{ category = $category; name = $name } }
    }
    $groups = @{}
    foreach ($entry in $CpuByName.GetEnumerator()) {
        $key = ([string]$entry.Key) -replace '#\d+$', ''
        if (-not $lookup.ContainsKey($key)) { continue }
        $known = $lookup[$key]
        if ($known.category -eq 'measurement') { continue }
        if ([double]$entry.Value -le 0) { continue }
        if (-not $groups.ContainsKey($known.category)) { $groups[$known.category] = @{ cpu = 0.0; members = @{} } }
        $groups[$known.category].cpu += [double]$entry.Value
        if ($groups[$known.category].members.ContainsKey($known.name)) {
            $groups[$known.category].members[$known.name] += [double]$entry.Value
        } else {
            $groups[$known.category].members[$known.name] = [double]$entry.Value
        }
    }
    $list = @()
    foreach ($g in $groups.GetEnumerator()) {
        $cpu = [Math]::Round($g.Value.cpu, 1)
        if ($cpu -lt 0.1) { continue }
        $names = @($g.Value.members.GetEnumerator() | Sort-Object Value -Descending | ForEach-Object { $_.Key })
        $list += [pscustomobject]@{ category = $g.Key; processes = $names; cpuPct = $cpu }
    }
    return @($list | Sort-Object cpuPct -Descending | ForEach-Object {
        [ordered]@{ category = $_.category; processes = @($_.processes); cpuPct = $_.cpuPct }
    })
}

function Get-WindowsThermal {
    param([int] $SampleSeconds)
    $r = [ordered]@{
        source = 'windows'
        timestampUtc = Get-PerformanceUtcStamp
        status = 'Unknown'
        headroomFraction = $null
        batteryTempC = $null
        cpuPackageTempC = $null
        gpuTempC = $null
        cpuPerformancePct = $null
        cpuFrequencyMHz = $null
        cpuUtilizationPct = $null
        isCharging = $null
        isLowPower = $null
        powerPlan = $null
        refreshHz = $null
        gpuClockMHz = $null
        gpuThrottleReasons = $null
        elevated = $false
        topProcesses = @()
        diskBusyPct = $null
        availableMemoryMB = $null
        availableMemoryPct = $null
        hardFaultsPerSec = $null
        pendingReboot = $null
        activities = @()
    }

    $r.elevated = Test-PerformanceElevated
    $r.pendingReboot = Test-PerformancePendingReboot

    # One Get-Counter call over the sample period covers the processor, per-process, disk
    # and memory counters. The performance counter is the throttling signal that needs no
    # elevation: it is the actual frequency as a percentage of the nominal one, so a
    # machine held at base clock by heat reads well under 100 while under load. An
    # unavailable path is skipped with a non-terminating error (listed in counterError);
    # if the call yields nothing, the processor counters are retried on their own.
    $processTotals = $null
    try {
        $samples = [Math]::Max(2, $SampleSeconds)
        $processorCounters = @(
            '\Processor Information(_Total)\% Processor Performance',
            '\Processor Information(_Total)\Processor Frequency',
            '\Processor(_Total)\% Processor Time'
        )
        $counters = $processorCounters + @(
            '\Process(*)\% Processor Time',
            '\PhysicalDisk(_Total)\% Idle Time',
            '\Memory\Available MBytes',
            '\Memory\Pages Input/sec'
        )
        $counterErrors = @()
        $data = @()
        try {
            $data = @(Get-Counter -Counter $counters -SampleInterval 1 -MaxSamples $samples -ErrorAction SilentlyContinue -ErrorVariable counterErrors)
        } catch {
            $counterErrors = @($counterErrors) + @($_)
        }
        if ($data.Count -eq 0) {
            $data = @(Get-Counter -Counter $processorCounters -SampleInterval 1 -MaxSamples $samples -ErrorAction Stop)
        }
        if (@($counterErrors).Count -gt 0) {
            $r['counterError'] = ((@($counterErrors) | ForEach-Object { $_.ToString() } | Select-Object -Unique) -join '; ')
        }
        $performance = @(); $freq = @(); $util = @(); $idle = @(); $availMB = @(); $pagesIn = @()
        $processTotals = @{}
        foreach ($set in $data) {
            foreach ($s in $set.CounterSamples) {
                $path = $s.Path
                if ($path -like '*\processor information(_total)\% processor performance') { $performance += $s.CookedValue }
                elseif ($path -like '*\processor information(_total)\processor frequency') { $freq += $s.CookedValue }
                elseif ($path -like '*\processor(_total)\% processor time') { $util += $s.CookedValue }
                elseif ($path -like '*\process(*)\% processor time') {
                    if ($s.InstanceName -eq '_total' -or $s.InstanceName -eq 'idle') { continue }
                    if (-not $processTotals.ContainsKey($s.InstanceName)) { $processTotals[$s.InstanceName] = New-Object System.Collections.Generic.List[double] }
                    [void]$processTotals[$s.InstanceName].Add($s.CookedValue)
                }
                elseif ($path -like '*\physicaldisk(_total)\% idle time') { $idle += $s.CookedValue }
                elseif ($path -like '*\memory\available mbytes') { $availMB += $s.CookedValue }
                elseif ($path -like '*\memory\pages input/sec') { $pagesIn += $s.CookedValue }
            }
        }
        if ($performance.Count -gt 0) { $r.cpuPerformancePct = [Math]::Round(($performance | Measure-Object -Average).Average, 1) }
        if ($freq.Count -gt 0) { $r.cpuFrequencyMHz = [Math]::Round(($freq | Measure-Object -Average).Average, 0) }
        if ($util.Count -gt 0) { $r.cpuUtilizationPct = [Math]::Round(($util | Measure-Object -Average).Average, 1) }
        if ($idle.Count -gt 0) {
            $busy = 100.0 - ($idle | Measure-Object -Average).Average
            $r.diskBusyPct = [Math]::Round([Math]::Min(100.0, [Math]::Max(0.0, $busy)), 1)
        }
        if ($availMB.Count -gt 0) {
            $r.availableMemoryMB = [Math]::Round(($availMB | Measure-Object -Minimum).Minimum, 0)
            try {
                $os = Get-CimInstance -ClassName Win32_OperatingSystem -ErrorAction Stop
                $totalMB = [double]$os.TotalVisibleMemorySize / 1024.0
                if ($totalMB -gt 0) { $r.availableMemoryPct = [Math]::Round(100.0 * $r.availableMemoryMB / $totalMB, 1) }
            } catch { }
        }
        if ($pagesIn.Count -gt 0) { $r.hardFaultsPerSec = [Math]::Round(($pagesIn | Measure-Object -Average).Average, 1) }
        if ($processTotals.Count -eq 0) { $processTotals = $null }
    } catch {
        $r['counterError'] = $_.Exception.Message
    }

    # Package temperature: only some firmware exposes it, and only to an elevated caller.
    try {
        $tz = Get-CimInstance -Namespace 'root\wmi' -ClassName MSAcpi_ThermalZoneTemperature -ErrorAction Stop
        $temps = @()
        foreach ($z in $tz) { if ($z.CurrentTemperature -gt 0) { $temps += (($z.CurrentTemperature / 10.0) - 273.15) } }
        if ($temps.Count -gt 0) { $r.cpuPackageTempC = [Math]::Round(($temps | Measure-Object -Maximum).Maximum, 1) }
    } catch {
        $r['thermalZoneError'] = $_.Exception.Message
    }

    # GPU via nvidia-smi when the machine has an NVIDIA adapter.
    $smi = Get-Command nvidia-smi -ErrorAction SilentlyContinue
    if ($null -eq $smi -and (Test-Path 'C:\Windows\System32\nvidia-smi.exe')) { $smi = @{ Source = 'C:\Windows\System32\nvidia-smi.exe' } }
    if ($null -ne $smi) {
        try {
            $call = Invoke-PerformanceNative -Exe $smi.Source -Arguments @('--query-gpu=temperature.gpu,clocks.sm,clocks.max.sm,clocks_throttle_reasons.active,utilization.gpu', '--format=csv,noheader,nounits')
            if ($call.ExitCode -eq 0 -and $call.Output.Count -gt 0) {
                $parts = ($call.Output | Select-Object -First 1) -split ','
                if ($parts.Count -ge 4) {
                    $r.gpuTempC = [double]($parts[0].Trim())
                    $r.gpuClockMHz = [double]($parts[1].Trim())
                    $r['gpuMaxClockMHz'] = [double]($parts[2].Trim())
                    $r.gpuThrottleReasons = $parts[3].Trim()
                    if ($parts.Count -ge 5) { $r['gpuUtilizationPct'] = [double]($parts[4].Trim()) }
                }
            }
        } catch { $r['nvidiaSmiError'] = $_.Exception.Message }
    }

    try {
        $call = Invoke-PerformanceNative -Exe 'powercfg' -Arguments @('/getactivescheme')
        if ($call.ExitCode -eq 0 -and $call.Output.Count -gt 0) {
            $scheme = $call.Output[0]
            if ($scheme -match '\((.+)\)\s*$') { $r.powerPlan = $Matches[1] }
        }
    } catch { }

    # isCharging means "on external power". BatteryStatus (root\wmi) exposes PowerOnline
    # directly when present; otherwise Win32_Battery.BatteryStatus values 2 (AC), 3, 6, 7,
    # 8, 9 (charging in its various states) and 11 (partially charged) all mean external
    # power is present, and every other value means the battery alone is supplying power.
    # No battery instance at all means a desktop, which is always on AC.
    try {
        $onlineStatus = $null
        $wmiBattery = $null
        try { $wmiBattery = @(Get-CimInstance -Namespace 'root\wmi' -ClassName BatteryStatus -ErrorAction Stop) } catch { $wmiBattery = $null }
        if ($null -ne $wmiBattery -and $wmiBattery.Count -gt 0) {
            $onlineStatus = $false
            foreach ($b in $wmiBattery) { if ($b.PowerOnline) { $onlineStatus = $true } }
        }
        if ($null -ne $onlineStatus) {
            $r.isCharging = $onlineStatus
        } else {
            $bat = Get-CimInstance -ClassName Win32_Battery -ErrorAction Stop | Select-Object -First 1
            if ($null -ne $bat) {
                $r.isCharging = (@(2, 3, 6, 7, 8, 9, 11) -contains $bat.BatteryStatus)
                $r['batteryPct'] = $bat.EstimatedChargeRemaining
            } else {
                $r.isCharging = $true
            }
        }
    } catch { }

    try {
        $vc = Get-CimInstance -ClassName Win32_VideoController -ErrorAction Stop | Where-Object { $_.CurrentRefreshRate -gt 0 } | Select-Object -First 1
        if ($null -ne $vc) { $r.refreshHz = $vc.CurrentRefreshRate; $r['gpuName'] = $vc.Name }
    } catch { }

    # Top CPU consumers by current activity, not lifetime total: % Processor Time per
    # process from the combined sample above, normalized by the logical processor count so
    # a single busy thread does not read over 100. Falls back to the lifetime CPU-seconds
    # sort when the counter is unavailable.
    try {
        if ($null -eq $processTotals) { throw 'Process counter unavailable' }
        $cpuCount = [Environment]::ProcessorCount
        if ($cpuCount -lt 1) { $cpuCount = 1 }
        # Instances of one executable (code, code#1, ...) are summed under its name.
        # Sort-Object needs objects, not hashtables, to sort by a key.
        $byName = @{}
        foreach ($entry in $processTotals.GetEnumerator()) {
            $name = $entry.Key -replace '#\d+$', ''
            $avg = (($entry.Value | Measure-Object -Average).Average) / $cpuCount
            if ($byName.ContainsKey($name)) { $byName[$name] += $avg } else { $byName[$name] = $avg }
        }
        $top = $byName.GetEnumerator() | ForEach-Object {
            [pscustomobject]@{ name = $_.Key; cpuPct = [Math]::Round($_.Value, 1) }
        } | Sort-Object cpuPct -Descending | Select-Object -First 5
        $r.topProcesses = @($top | ForEach-Object { @{ name = $_.name; cpuPct = $_.cpuPct } })
        $r.activities = @(Get-KnownActivities -CpuByName $byName)
    } catch {
        try {
            $procs = Get-Process | Where-Object { $_.CPU -gt 0 } | Sort-Object CPU -Descending | Select-Object -First 5
            $r.topProcesses = @($procs | ForEach-Object { @{ name = $_.ProcessName; cpuSeconds = [Math]::Round($_.CPU, 1) } })
        } catch { }
    }

    # Windows has no thermal status API for user code; the status stays Unknown and the
    # analyzer gates on cpuPerformancePct instead.
    return $r
}

function Get-AndroidThermal {
    param([string] $Serial, [string] $AdbPath)
    $adb = Resolve-PerformanceAdb -AdbPath $AdbPath
    $serialArgs = @()
    if ($Serial) { $serialArgs = @('-s', $Serial) }
    $r = [ordered]@{
        source = 'android'
        timestampUtc = Get-PerformanceUtcStamp
        status = 'Unknown'
        statusCode = $null
        headroomFraction = $null
        batteryTempC = $null
        cpuPackageTempC = $null
        gpuTempC = $null
        cpuPerformancePct = $null
        cpuFrequencyMHz = $null
        isCharging = $null
        isLowPower = $null
        powerPlan = $null
        thermalZones = @()
        loadAverage = $null
        topProcesses = @()
    }
    $statusNames = @('Nominal', 'Light', 'Moderate', 'Severe', 'Critical', 'Emergency', 'Shutdown')

    $call = Invoke-PerformanceNative -Exe $adb -Arguments (@($serialArgs) + @('shell', 'dumpsys', 'thermalservice'))
    if ($call.ExitCode -eq 0 -and $call.Output.Count -gt 0) {
        $ts = $call.Output
        $joined = ($ts -join "`n")
        if ($joined -match 'Thermal Status:\s*(\d+)') {
            $code = [int]$Matches[1]
            $r.statusCode = $code
            if ($code -ge 0 -and $code -lt $statusNames.Count) {
                $name = $statusNames[$code]
                # The analyzer's classes stop at Critical
                if ($code -ge 4) { $name = 'Critical' }
                $r.status = $name
            }
        }
        $zones = @()
        foreach ($line in $ts) {
            if ($line -match 'Temperature\{mValue=([\d.\-]+),\s*mType=(\d+),\s*mName=([^,]+),\s*mStatus=(\d+)') {
                $zones += @{ name = $Matches[3].Trim(); value = [double]$Matches[1]; type = [int]$Matches[2]; status = [int]$Matches[4] }
            }
        }
        $r.thermalZones = $zones
        $cpuZones = @($zones | Where-Object { $_.type -eq 0 })
        if ($cpuZones.Count -gt 0) { $r.cpuPackageTempC = ($cpuZones | Measure-Object -Property value -Maximum).Maximum }
        $gpuZones = @($zones | Where-Object { $_.type -eq 1 })
        if ($gpuZones.Count -gt 0) { $r.gpuTempC = ($gpuZones | Measure-Object -Property value -Maximum).Maximum }
    } else {
        $r['thermalserviceError'] = 'dumpsys thermalservice failed or returned nothing'
    }

    $call = Invoke-PerformanceNative -Exe $adb -Arguments (@($serialArgs) + @('shell', 'dumpsys', 'battery'))
    if ($call.ExitCode -eq 0 -and $call.Output.Count -gt 0) {
        $joined = ($call.Output -join "`n")
        if ($joined -match 'temperature:\s*(\d+)') { $r.batteryTempC = ([int]$Matches[1]) / 10.0 }
        if ($joined -match 'AC powered:\s*(true|false)') { $ac = $Matches[1] -eq 'true' } else { $ac = $false }
        if ($joined -match 'USB powered:\s*(true|false)') { $usb = $Matches[1] -eq 'true' } else { $usb = $false }
        if ($joined -match 'Wireless powered:\s*(true|false)') { $wl = $Matches[1] -eq 'true' } else { $wl = $false }
        $r.isCharging = ($ac -or $usb -or $wl)
        if ($joined -match 'level:\s*(\d+)') { $r['batteryPct'] = [int]$Matches[1] }
    }

    $call = Invoke-PerformanceNative -Exe $adb -Arguments (@($serialArgs) + @('shell', 'settings', 'get', 'global', 'low_power'))
    if ($call.ExitCode -eq 0 -and $call.Output.Count -gt 0) { $r.isLowPower = (($call.Output[0]).Trim() -eq '1') }

    # Current CPU frequency of the big cores, when the sysfs node is readable
    $call = Invoke-PerformanceNative -Exe $adb -Arguments (@($serialArgs) + @('shell', 'cat /sys/devices/system/cpu/cpu*/cpufreq/scaling_cur_freq 2>/dev/null'))
    if ($call.ExitCode -eq 0 -and $call.Output.Count -gt 0) {
        $vals = @($call.Output | ForEach-Object { $_.Trim() } | Where-Object { $_ -match '^\d+$' } | ForEach-Object { [double]$_ / 1000.0 })
        if ($vals.Count -gt 0) { $r.cpuFrequencyMHz = ($vals | Measure-Object -Maximum).Maximum; $r['cpuFrequenciesMHz'] = $vals }
    }

    # dumpsys cpuinfo: the "Load: 1m / 5m / 15m" line, and the first five process lines
    # ("12% 1234/name: ..."), whose percentages are already shares of total CPU capacity.
    # They cover the tracker's last update period, not this probe's moment.
    try {
        $call = Invoke-PerformanceNative -Exe $adb -Arguments (@($serialArgs) + @('shell', 'dumpsys', 'cpuinfo'))
        if ($call.ExitCode -eq 0 -and $call.Output.Count -gt 0) {
            $invariant = [System.Globalization.CultureInfo]::InvariantCulture
            $top = @()
            foreach ($line in $call.Output) {
                if ($null -eq $r.loadAverage -and $line -match '^\s*Load:\s*([\d.]+)\s*/\s*([\d.]+)\s*/\s*([\d.]+)') {
                    $r.loadAverage = @([double]::Parse($Matches[1], $invariant), [double]::Parse($Matches[2], $invariant), [double]::Parse($Matches[3], $invariant))
                } elseif ($top.Count -lt 5 -and $line -match '^\s*([\d.]+)%\s+\d+/([^:]+):') {
                    $top += @{ name = $Matches[2].Trim(); cpuPct = [double]::Parse($Matches[1], $invariant) }
                }
            }
            $r.topProcesses = $top
        } else {
            $r['cpuinfoError'] = 'dumpsys cpuinfo failed or returned nothing'
        }
    } catch {
        $r['cpuinfoError'] = $_.Exception.Message
    }

    return $r
}

if ($Android) {
    $result = Get-AndroidThermal -Serial $Serial -AdbPath $AdbPath
} else {
    $result = Get-WindowsThermal -SampleSeconds $SampleSeconds
}

if ($OutFile) {
    Write-PerformanceJson -Object $result -Path $OutFile
    Write-PerformanceLog ("Thermal state written to {0} (status {1}, cpuPerf {2}, cpuTemp {3}, battTemp {4})" -f $OutFile, $result.status, $result.cpuPerformancePct, $result.cpuPackageTempC, $result.batteryTempC)
} else {
    ConvertTo-Json -InputObject $result -Depth 20
}
