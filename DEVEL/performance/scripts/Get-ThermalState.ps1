<#
.SYNOPSIS
Reads the thermal and power state of the Windows host, or of an Android device, as JSON.

.DESCRIPTION
Emits one JSON object with the fields the analyzer's EnvJson reader expects:
status, headroomFraction, batteryTempC, cpuPackageTempC, gpuTempC, cpuPerformancePct,
cpuFrequencyMHz, isCharging, isLowPower, powerPlan, timestampUtc, plus source-specific
extras. Missing sensors are null, never guessed.

Windows signals, in order of value:
  cpuPerformancePct  \Processor Information(_Total)\% Processor Performance sampled over
                     -SampleSeconds. Sustained values under 100 at load indicate frequency
                     throttling; the analyzer's gate uses 90 as the floor. Needs no
                     elevation.
  cpuPackageTempC    MSAcpi_ThermalZoneTemperature (root\wmi), usually needs elevation and
                     is absent on many laptops; null otherwise.
  gpuTempC, gpuClockMHz, gpuThrottleReasons  from nvidia-smi when present.
  powerPlan, isCharging, refreshHz.

Android (-Android, optional -Serial): dumpsys thermalservice (status 0..6 mapped to
Nominal..Critical), dumpsys battery (temperature, charging).

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
    }

    try {
        $id = [Security.Principal.WindowsIdentity]::GetCurrent()
        $r.elevated = (New-Object Security.Principal.WindowsPrincipal($id)).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)
    } catch { }

    # Processor performance and frequency, averaged over the sample period. The
    # performance counter is the throttling signal that needs no elevation: it is the
    # actual frequency as a percentage of the nominal one, so a machine held at base
    # clock by heat reads well under 100 while under load.
    try {
        $samples = [Math]::Max(2, $SampleSeconds)
        $counters = @(
            '\Processor Information(_Total)\% Processor Performance',
            '\Processor Information(_Total)\Processor Frequency',
            '\Processor(_Total)\% Processor Time'
        )
        $data = Get-Counter -Counter $counters -SampleInterval 1 -MaxSamples $samples -ErrorAction Stop
        $performance = @(); $freq = @(); $util = @()
        foreach ($set in $data) {
            foreach ($s in $set.CounterSamples) {
                if ($s.Path -like '*% Processor Performance*') { $performance += $s.CookedValue }
                elseif ($s.Path -like '*Processor Frequency*') { $freq += $s.CookedValue }
                elseif ($s.Path -like '*% Processor Time*') { $util += $s.CookedValue }
            }
        }
        if ($performance.Count -gt 0) { $r.cpuPerformancePct = [Math]::Round(($performance | Measure-Object -Average).Average, 1) }
        if ($freq.Count -gt 0) { $r.cpuFrequencyMHz = [Math]::Round(($freq | Measure-Object -Average).Average, 0) }
        if ($util.Count -gt 0) { $r.cpuUtilizationPct = [Math]::Round(($util | Measure-Object -Average).Average, 1) }
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
            $q = & $smi.Source --query-gpu=temperature.gpu,clocks.sm,clocks.max.sm,clocks_throttle_reasons.active,utilization.gpu --format=csv,noheader,nounits 2>$null
            if ($LASTEXITCODE -eq 0 -and $q) {
                $parts = ($q | Select-Object -First 1) -split ','
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
        $scheme = (& powercfg /getactivescheme 2>$null | Select-Object -First 1)
        if ($scheme -match '\((.+)\)\s*$') { $r.powerPlan = $Matches[1] }
    } catch { }

    try {
        $bat = Get-CimInstance -ClassName Win32_Battery -ErrorAction Stop | Select-Object -First 1
        if ($null -ne $bat) {
            # BatteryStatus 2 = AC power; 1 = discharging
            $r.isCharging = ($bat.BatteryStatus -ne 1)
            $r['batteryPct'] = $bat.EstimatedChargeRemaining
        } else {
            $r.isCharging = $true
        }
    } catch { }

    try {
        $vc = Get-CimInstance -ClassName Win32_VideoController -ErrorAction Stop | Where-Object { $_.CurrentRefreshRate -gt 0 } | Select-Object -First 1
        if ($null -ne $vc) { $r.refreshHz = $vc.CurrentRefreshRate; $r['gpuName'] = $vc.Name }
    } catch { }

    try {
        $procs = Get-Process | Where-Object { $_.CPU -gt 0 } | Sort-Object CPU -Descending | Select-Object -First 5
        $r.topProcesses = @($procs | ForEach-Object { @{ name = $_.ProcessName; cpuSeconds = [Math]::Round($_.CPU, 1) } })
    } catch { }

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
    }
    $statusNames = @('Nominal', 'Light', 'Moderate', 'Severe', 'Critical', 'Emergency', 'Shutdown')

    $ts = & $adb @serialArgs shell dumpsys thermalservice 2>$null
    if ($LASTEXITCODE -eq 0 -and $ts) {
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

    $bat = & $adb @serialArgs shell dumpsys battery 2>$null
    if ($LASTEXITCODE -eq 0 -and $bat) {
        $joined = ($bat -join "`n")
        if ($joined -match 'temperature:\s*(\d+)') { $r.batteryTempC = ([int]$Matches[1]) / 10.0 }
        if ($joined -match 'AC powered:\s*(true|false)') { $ac = $Matches[1] -eq 'true' } else { $ac = $false }
        if ($joined -match 'USB powered:\s*(true|false)') { $usb = $Matches[1] -eq 'true' } else { $usb = $false }
        if ($joined -match 'Wireless powered:\s*(true|false)') { $wl = $Matches[1] -eq 'true' } else { $wl = $false }
        $r.isCharging = ($ac -or $usb -or $wl)
        if ($joined -match 'level:\s*(\d+)') { $r['batteryPct'] = [int]$Matches[1] }
    }

    $lp = & $adb @serialArgs shell settings get global low_power 2>$null
    if ($LASTEXITCODE -eq 0 -and $lp) { $r.isLowPower = (($lp | Select-Object -First 1).Trim() -eq '1') }

    # Current CPU frequency of the big cores, when the sysfs node is readable
    $freq = & $adb @serialArgs shell 'cat /sys/devices/system/cpu/cpu*/cpufreq/scaling_cur_freq 2>/dev/null' 2>$null
    if ($LASTEXITCODE -eq 0 -and $freq) {
        $vals = @($freq | ForEach-Object { $_.Trim() } | Where-Object { $_ -match '^\d+$' } | ForEach-Object { [double]$_ / 1000.0 })
        if ($vals.Count -gt 0) { $r.cpuFrequencyMHz = ($vals | Measure-Object -Maximum).Maximum; $r['cpuFrequenciesMHz'] = $vals }
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
    $result | ConvertTo-Json -Depth 20
}
