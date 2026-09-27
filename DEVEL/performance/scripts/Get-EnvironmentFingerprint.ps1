<#
.SYNOPSIS
Captures the environment fingerprint of the Windows host, or of an Android device, as a
flat JSON object.

.DESCRIPTION
Emits one JSON object whose keys follow the fingerprint contract, <category>.<name>, with
string values only: booleans are "true"/"false" and numbers use the invariant culture. A
key is left out when its source is missing or its query failed; nothing is guessed.

Always: meta.fingerprintVersion ("1") and meta.capturedUtc (never diffed).

Windows:
  os.platform          "Windows"
  os.version           10.0.<CurrentBuild>, from HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion
  os.build             <CurrentBuild>.<UBR>, e.g. 26200.6584
  os.displayVersion    DisplayVersion, e.g. 25H2
  os.edition           EditionID
  os.pendingReboot     "true" when the Windows Update RebootRequired or the Component
                       Based Servicing RebootPending key exists
  os.latestHotfix      the newest Get-HotFix entry by InstalledOn (its HotFixID)
  driver.gpu<N>.version, driver.gpu<N>.date, hardware.gpu<N>
                       Win32_VideoController, 0-based in the order CIM returns adapters;
                       the date is yyyy-MM-dd
  hardware.cpu         Win32_Processor name
  hardware.logicalProcessors
  hardware.memoryGB    installed memory (Win32_PhysicalMemory), in GB
  hardware.deviceModel Win32_ComputerSystem manufacturer and model
  settings.powerPlan   powercfg /getactivescheme: the well-known scheme name for its GUID,
                       else the GUID
  toolchain.dotnetSdk  dotnet --version, run in win\win32\xpl\GnollHackTests so that its
                       global.json applies

Android (-Android, optional -Serial and -AdbPath), from getprop and dumpsys:
  os.platform          "Android"
  os.version           ro.build.version.release
  os.build             ro.build.version.incremental
  os.securityPatch     ro.build.version.security_patch
  os.fingerprint       ro.build.fingerprint
  hardware.deviceModel ro.product.model
  hardware.soc         ro.soc.model, else ro.hardware
  driver.gles          the GLES: line of dumpsys SurfaceFlinger

.EXAMPLE
.\Get-EnvironmentFingerprint.ps1 -OutFile fingerprint.json
.\Get-EnvironmentFingerprint.ps1 -Android -Serial R5CT1234 -OutFile fingerprint.json
#>
[CmdletBinding()]
param(
    [switch] $Android,
    [string] $Serial,
    [string] $AdbPath,
    [string] $OutFile,
    [string] $RepositoryRoot
)

$ErrorActionPreference = 'Stop'
. (Join-Path $PSScriptRoot 'Common.ps1')

$invariant = [System.Globalization.CultureInfo]::InvariantCulture

function Add-FingerprintValue {
    param([hashtable] $Map, [string] $Key, $Value)
    if ($null -eq $Value) { return }
    if ($Value -is [bool]) {
        if ($Value) { $Map[$Key] = 'true' } else { $Map[$Key] = 'false' }
        return
    }
    if ($Value -is [System.IFormattable]) {
        $text = $Value.ToString($null, $invariant)
    } else {
        $text = [string]$Value
    }
    $text = $text.Trim()
    if ($text) { $Map[$Key] = $text }
}

function Get-RegistryValue {
    param($Item, [string] $Name)
    if ($null -eq $Item) { return $null }
    $prop = $Item.PSObject.Properties[$Name]
    if ($null -eq $prop) { return $null }
    return $prop.Value
}

function Get-WindowsFingerprint {
    param([hashtable] $Map, [string] $RepoRoot)
    Add-FingerprintValue $Map 'os.platform' 'Windows'

    try {
        $cv = Get-ItemProperty -LiteralPath 'HKLM:\SOFTWARE\Microsoft\Windows NT\CurrentVersion' -ErrorAction Stop
        $major = Get-RegistryValue $cv 'CurrentMajorVersionNumber'
        $minor = Get-RegistryValue $cv 'CurrentMinorVersionNumber'
        $build = Get-RegistryValue $cv 'CurrentBuild'
        $ubr = Get-RegistryValue $cv 'UBR'
        if ($null -ne $major -and $null -ne $minor -and $null -ne $build) {
            Add-FingerprintValue $Map 'os.version' ('{0}.{1}.{2}' -f $major, $minor, $build)
        }
        if ($null -ne $build) {
            if ($null -ne $ubr) { Add-FingerprintValue $Map 'os.build' ('{0}.{1}' -f $build, $ubr) } else { Add-FingerprintValue $Map 'os.build' $build }
        }
        Add-FingerprintValue $Map 'os.displayVersion' (Get-RegistryValue $cv 'DisplayVersion')
        Add-FingerprintValue $Map 'os.edition' (Get-RegistryValue $cv 'EditionID')
    } catch { }

    Add-FingerprintValue $Map 'os.pendingReboot' (Test-PerformancePendingReboot)

    try {
        $hotfix = Get-HotFix -ErrorAction Stop | Where-Object { $null -ne $_.InstalledOn } |
            Sort-Object InstalledOn -Descending | Select-Object -First 1
        if ($null -ne $hotfix) { Add-FingerprintValue $Map 'os.latestHotfix' $hotfix.HotFixID }
    } catch { }

    try {
        $adapters = @(Get-CimInstance -ClassName Win32_VideoController -ErrorAction Stop)
        for ($i = 0; $i -lt $adapters.Count; $i++) {
            $a = $adapters[$i]
            Add-FingerprintValue $Map ('hardware.gpu{0}' -f $i) $a.Name
            Add-FingerprintValue $Map ('driver.gpu{0}.version' -f $i) $a.DriverVersion
            if ($null -ne $a.DriverDate) {
                Add-FingerprintValue $Map ('driver.gpu{0}.date' -f $i) (([datetime]$a.DriverDate).ToString('yyyy-MM-dd', $invariant))
            }
        }
    } catch { }

    try {
        $cpu = Get-CimInstance -ClassName Win32_Processor -ErrorAction Stop | Select-Object -First 1
        if ($null -ne $cpu) { Add-FingerprintValue $Map 'hardware.cpu' $cpu.Name }
    } catch { }
    Add-FingerprintValue $Map 'hardware.logicalProcessors' ([Environment]::ProcessorCount)

    try {
        $modules = @(Get-CimInstance -ClassName Win32_PhysicalMemory -ErrorAction Stop)
        $bytes = 0.0
        foreach ($m in $modules) { $bytes += [double]$m.Capacity }
        if ($bytes -le 0) {
            $cs = Get-CimInstance -ClassName Win32_ComputerSystem -ErrorAction Stop
            $bytes = [double]$cs.TotalPhysicalMemory
        }
        if ($bytes -gt 0) { Add-FingerprintValue $Map 'hardware.memoryGB' ([Math]::Round($bytes / 1GB, 1)) }
    } catch { }

    try {
        $cs = Get-CimInstance -ClassName Win32_ComputerSystem -ErrorAction Stop
        Add-FingerprintValue $Map 'hardware.deviceModel' (($cs.Manufacturer + ' ' + $cs.Model).Trim())
    } catch { }

    try {
        $call = Invoke-PerformanceNative -Exe 'powercfg' -Arguments @('/getactivescheme')
        if ($call.ExitCode -eq 0 -and $call.Output.Count -gt 0) {
            $line = ($call.Output -join ' ')
            if ($line -match '([0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{12})') {
                $guid = $Matches[1].ToLowerInvariant()
                $wellKnown = @{
                    '381b4222-f694-41f0-9685-ff5bb260df2e' = 'Balanced'
                    '8c5e7fda-e8bf-4a96-9a85-a6e23a8c635c' = 'High performance'
                    'a1841308-3542-4fab-bc81-f71556f20b4a' = 'Power saver'
                    'e9a42b02-d5df-448d-aa00-03f14749eb61' = 'Ultimate Performance'
                }
                if ($wellKnown.ContainsKey($guid)) { Add-FingerprintValue $Map 'settings.powerPlan' $wellKnown[$guid] }
                else { Add-FingerprintValue $Map 'settings.powerPlan' $guid }
            }
        }
    } catch { }

    # dotnet resolves global.json from its working directory, which follows the current
    # location for native commands.
    $dotnet = Get-Command dotnet -ErrorAction SilentlyContinue
    if ($null -ne $dotnet -and $RepoRoot) {
        $testsDir = Join-Path $RepoRoot 'win\win32\xpl\GnollHackTests'
        if (Test-Path -LiteralPath $testsDir) {
            Push-Location -LiteralPath $testsDir
            try {
                $call = Invoke-PerformanceNative -Exe $dotnet.Source -Arguments @('--version')
                if ($call.ExitCode -eq 0 -and $call.Output.Count -gt 0) {
                    Add-FingerprintValue $Map 'toolchain.dotnetSdk' ($call.Output | Select-Object -First 1)
                }
            } catch {
            } finally {
                Pop-Location
            }
        }
    }
}

function Get-AndroidFingerprint {
    param([hashtable] $Map, [string] $Serial, [string] $AdbPath)
    $adb = Resolve-PerformanceAdb -AdbPath $AdbPath
    $serialArgs = @()
    if ($Serial) { $serialArgs = @('-s', $Serial) }
    Add-FingerprintValue $Map 'os.platform' 'Android'

    # One getprop dump, lines of the form [key]: [value]
    $props = @{}
    $call = Invoke-PerformanceNative -Exe $adb -Arguments (@($serialArgs) + @('shell', 'getprop'))
    if ($call.ExitCode -eq 0) {
        foreach ($line in $call.Output) {
            if ($line -match '^\s*\[([^\]]+)\]:\s*\[(.*)\]\s*$') { $props[$Matches[1]] = $Matches[2] }
        }
    }
    $pairs = @(
        @('os.version', 'ro.build.version.release'),
        @('os.build', 'ro.build.version.incremental'),
        @('os.securityPatch', 'ro.build.version.security_patch'),
        @('os.fingerprint', 'ro.build.fingerprint'),
        @('hardware.deviceModel', 'ro.product.model')
    )
    foreach ($pair in $pairs) {
        if ($props.ContainsKey($pair[1])) { Add-FingerprintValue $Map $pair[0] $props[$pair[1]] }
    }
    if ($props.ContainsKey('ro.soc.model') -and $props['ro.soc.model'].Trim()) {
        Add-FingerprintValue $Map 'hardware.soc' $props['ro.soc.model']
    } elseif ($props.ContainsKey('ro.hardware')) {
        Add-FingerprintValue $Map 'hardware.soc' $props['ro.hardware']
    }

    $call = Invoke-PerformanceNative -Exe $adb -Arguments (@($serialArgs) + @('shell', 'dumpsys', 'SurfaceFlinger'))
    if ($call.ExitCode -eq 0) {
        foreach ($line in $call.Output) {
            if ($line -match '^\s*GLES:\s*(.+)$') {
                Add-FingerprintValue $Map 'driver.gles' $Matches[1]
                break
            }
        }
    }
}

if (-not $RepositoryRoot) {
    $RepositoryRoot = (Resolve-Path (Join-Path $PSScriptRoot '..\..\..')).Path
}

$values = @{}
if ($Android) {
    Get-AndroidFingerprint -Map $values -Serial $Serial -AdbPath $AdbPath
} else {
    Get-WindowsFingerprint -Map $values -RepoRoot $RepositoryRoot
}

$result = [ordered]@{
    'meta.fingerprintVersion' = '1'
    'meta.capturedUtc' = Get-PerformanceUtcStamp
}
foreach ($key in ($values.Keys | Sort-Object)) { $result[$key] = $values[$key] }

if ($OutFile) {
    Write-PerformanceJson -Object $result -Path $OutFile
    Write-PerformanceLog ("Environment fingerprint written to {0} ({1} keys)" -f $OutFile, $result.Count)
} else {
    ConvertTo-Json -InputObject $result -Depth 20
}
