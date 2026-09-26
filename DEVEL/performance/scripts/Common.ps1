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
