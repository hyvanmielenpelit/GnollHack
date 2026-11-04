<#
.SYNOPSIS
    Builds the GnollHack engine library for iOS (libgnollhackios.a) or macOS (libgnollhackmac.dylib) on a Mac over SSH.

.DESCRIPTION
    Run by gnollhackios.vcxproj and gnollhackmac.vcxproj. Packs the engine sources named in -SourceList, the headers of
    include\, win\win32\xpl\libshare\ and the platform folder, and an LF copy of engine.mk into a tar file in -IntDir,
    copies it to the Mac with scp, unpacks it into a fresh source tree, builds it there with make and clang, and copies
    the library back to -OutDir. Objects persist on the Mac between runs, and make recompiles a source only when it or
    a header is newer than its object. Every run of the WSL data pipeline rewrites date_unix.h, so a solution build
    usually recompiles everything.

    The Mac is reached through an OpenSSH host alias with key-based authentication: -SshHost, else the environment
    variable GNOLLHACK_MAC_SSH_HOST, else gnollhack-mac. Every connection uses BatchMode, so a missing key or an unknown
    host key fails at once instead of waiting for a password. The wiki page "Set Up SSH Access to the Mac for Native
    Libraries" describes the setup.

    Errors are printed in MSBuild's canonical format, as "Build-AppleEngine.ps1 : error GNHABnnn: ...".

.PARAMETER Platform
    ios or macos.

.PARAMETER Configuration
    Debug or Release.

.PARAMETER MinOS
    The deployment target: the minimum iOS or macOS version.

.PARAMETER IntDir
    The local working folder for the tar file and the generated files.

.PARAMETER OutDir
    The local folder the library is copied to.

.PARAMETER SourceList
    A file with the full path of one C source per line, written by the GnhWriteAppleSourceList target.

.PARAMETER SshHost
    The OpenSSH host alias of the Mac.

.PARAMETER RemoteRoot
    The build folder on the Mac, relative to the home folder. Default: gnollhack-build.

.PARAMETER Clean
    Deletes the objects and the library on the Mac before building.

.PARAMETER CleanOnly
    Deletes the local library and, when the Mac can be reached, the objects and the library on the Mac, then stops.

.PARAMETER PackageOnly
    Writes the tar file and stops, without connecting to the Mac.

.EXAMPLE
    .\Build-AppleEngine.ps1 -Platform macos -Configuration Release -MinOS 15.0 -SourceList obj\gnh-sources.txt -IntDir obj -OutDir out
#>
[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)][ValidateSet('ios', 'macos')][string]$Platform,
    [Parameter(Mandatory = $true)][ValidateSet('Debug', 'Release')][string]$Configuration,
    [Parameter(Mandatory = $true)][string]$MinOS,
    [Parameter(Mandatory = $true)][string]$IntDir,
    [Parameter(Mandatory = $true)][string]$OutDir,
    [string]$SourceList = '',
    [string]$SshHost = '',
    [string]$RemoteRoot = 'gnollhack-build',
    [switch]$Clean,
    [switch]$CleanOnly,
    [switch]$PackageOnly
)

Set-StrictMode -Version 2
$ErrorActionPreference = 'Stop'

# ---------------------------------------------------------------------------------------------------
# Helper functions
# ---------------------------------------------------------------------------------------------------

function Stop-Build
{
    param([string]$Code, [string]$Message)
    Write-Host ('Build-AppleEngine.ps1 : error {0}: {1}' -f $Code, $Message)
    exit 1
}

# Windows' own OpenSSH and bsdtar; Sysnative reaches System32 from a 32-bit host
function Find-Tool
{
    param([string]$Name, [string]$RelativePath)
    $command = Get-Command $Name -CommandType Application -ErrorAction SilentlyContinue | Select-Object -First 1
    if ($command)
    {
        return $command.Source
    }
    $fallback = Join-Path $env:SystemRoot (Join-Path 'Sysnative' $RelativePath)
    if (Test-Path $fallback)
    {
        return $fallback
    }
    Stop-Build 'GNHAB001' ('{0} was not found. It is part of Windows: install the OpenSSH Client under Settings > System > Optional features.' -f $Name)
}

function Invoke-Tool
{
    param([string]$Code, [string]$What, [string]$FilePath, [string[]]$Arguments)
    & $FilePath @Arguments
    if ($LASTEXITCODE -ne 0)
    {
        Stop-Build $Code ('{0} failed with exit code {1}.' -f $What, $LASTEXITCODE)
    }
}

function Invoke-Mac
{
    param([string]$Code, [string]$What, [string]$Command)
    Invoke-Tool $Code $What $script:Ssh (@($script:SshOptions) + @($script:SshHost, $Command))
}

# ---------------------------------------------------------------------------------------------------
# Settings
# ---------------------------------------------------------------------------------------------------

if ([string]::IsNullOrEmpty($SshHost))
{
    $SshHost = $env:GNOLLHACK_MAC_SSH_HOST
}
if ([string]::IsNullOrEmpty($SshHost))
{
    $SshHost = 'gnollhack-mac'
}
# Both are passed unquoted: the host to ssh and scp, the folder in commands for the Mac's shell
if ($SshHost -notmatch '^[A-Za-z0-9._@-]+$')
{
    Stop-Build 'GNHAB004' ("The SSH host '{0}' may contain only letters, digits and . _ @ -." -f $SshHost)
}
if ($RemoteRoot -notmatch '^[A-Za-z0-9._/-]+$')
{
    Stop-Build 'GNHAB004' ("The remote folder '{0}' may contain only letters, digits and . _ / -." -f $RemoteRoot)
}

$RepoRoot = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..\..\..\..')).TrimEnd('\')
$IntDir = [IO.Path]::GetFullPath($IntDir).TrimEnd('\')
$OutDir = [IO.Path]::GetFullPath($OutDir).TrimEnd('\')
if ($Platform -eq 'ios')
{
    $LibName = 'libgnollhackios.a'
    $PlatformDir = 'win\win32\xpl\gnollhackios'
    $Sdk = 'iphoneos'
}
else
{
    $LibName = 'libgnollhackmac.dylib'
    $PlatformDir = 'win\win32\xpl\gnollhackmac'
    $Sdk = 'macosx'
}
$RemoteDir = '{0}/{1}-{2}' -f $RemoteRoot.TrimEnd('/'), $Platform, $Configuration
$LocalLib = Join-Path $OutDir $LibName
$script:SshOptions = @('-o', 'BatchMode=yes', '-o', 'ConnectTimeout=15')

# ---------------------------------------------------------------------------------------------------
# Clean only
# ---------------------------------------------------------------------------------------------------

if ($CleanOnly)
{
    if (Test-Path $LocalLib)
    {
        Remove-Item $LocalLib -Force
    }
    $script:Ssh = Find-Tool 'ssh.exe' 'OpenSSH\ssh.exe'
    & $script:Ssh @script:SshOptions $SshHost ('rm -rf {0}/obj {0}/out' -f $RemoteDir)
    if ($LASTEXITCODE -ne 0)
    {
        Write-Host ('Build-AppleEngine.ps1 : warning GNHAB012: {0} was not cleaned on the Mac ({1}).' -f $RemoteDir, $SshHost)
    }
    exit 0
}

# ---------------------------------------------------------------------------------------------------
# Package
# ---------------------------------------------------------------------------------------------------

$DateHeader = Join-Path $RepoRoot (Join-Path $PlatformDir 'date_unix.h')
if (-not (Test-Path $DateHeader))
{
    Stop-Build 'GNHAB002' ('{0} is missing. The WSL data pipeline writes it (afterdroidutils.proj): build GnollHack.sln with the macOS or iPhone solution platform, which builds the *droid tools first.' -f $DateHeader)
}
if ([string]::IsNullOrEmpty($SourceList) -or -not (Test-Path $SourceList))
{
    Stop-Build 'GNHAB003' ("The source list '{0}' is missing. The GnhWriteAppleSourceList target writes it." -f $SourceList)
}

$RootPrefix = $RepoRoot + '\'
$Sources = New-Object System.Collections.Generic.List[string]
foreach ($line in [IO.File]::ReadAllLines($SourceList))
{
    $path = $line.Trim()
    if ($path.Length -eq 0)
    {
        continue
    }
    $full = [IO.Path]::GetFullPath($path)
    if (-not $full.StartsWith($RootPrefix, [StringComparison]::OrdinalIgnoreCase) -or -not (Test-Path $full))
    {
        Stop-Build 'GNHAB003' ('{0} is not a file in the repository.' -f $full)
    }
    $Sources.Add($full.Substring($RootPrefix.Length).Replace('\', '/'))
}
$Headers = New-Object System.Collections.Generic.List[string]
foreach ($dir in @('include', 'win\win32\xpl\libshare', $PlatformDir))
{
    foreach ($file in Get-ChildItem -Path (Join-Path $RepoRoot $dir) -Filter '*.h' -File)
    {
        $Headers.Add($file.FullName.Substring($RootPrefix.Length).Replace('\', '/'))
    }
}

New-Item -ItemType Directory -Force -Path $IntDir | Out-Null
$Utf8 = New-Object System.Text.UTF8Encoding($false)
# The Mac's make and shell need LF; the repository copy of engine.mk is CRLF on Windows
$Makefile = [IO.File]::ReadAllText((Join-Path $PSScriptRoot 'engine.mk')) -replace "`r`n", "`n"
[IO.File]::WriteAllText((Join-Path $IntDir 'engine.mk'), $Makefile, $Utf8)
[IO.File]::WriteAllText((Join-Path $IntDir 'gnh-sources.mk'), ('SOURCES := ' + ($Sources -join ' ') + "`n"), $Utf8)
$FileList = Join-Path $IntDir 'gnh-files.txt'
[IO.File]::WriteAllText($FileList, ((@($Sources) + @($Headers)) -join "`n") + "`n", $Utf8)

$TarFile = Join-Path $IntDir 'gnh-src.tar'
if (Test-Path $TarFile)
{
    Remove-Item $TarFile -Force
}
$Tar = Find-Tool 'tar.exe' 'tar.exe'
Invoke-Tool 'GNHAB005' 'Packing the sources' $Tar @('-c', '-f', $TarFile, '-C', $RepoRoot, '-T', $FileList)
Invoke-Tool 'GNHAB005' 'Packing the makefiles' $Tar @('-r', '-f', $TarFile, '-C', $IntDir, 'engine.mk', 'gnh-sources.mk')
Write-Host ('Build-AppleEngine.ps1: {0} sources and {1} headers packed into {2}' -f $Sources.Count, $Headers.Count, $TarFile)
if ($PackageOnly)
{
    exit 0
}

# ---------------------------------------------------------------------------------------------------
# Build on the Mac
# ---------------------------------------------------------------------------------------------------

$script:Ssh = Find-Tool 'ssh.exe' 'OpenSSH\ssh.exe'
$Scp = Find-Tool 'scp.exe' 'OpenSSH\scp.exe'

& $script:Ssh @script:SshOptions $SshHost ('xcrun --sdk {0} --show-sdk-path' -f $Sdk)
if ($LASTEXITCODE -eq 255)
{
    Stop-Build 'GNHAB006' ("Cannot connect to the Mac as '{0}'. Check that the Mac is awake, that Remote Login is on, and that 'ssh {0}' logs in without a password; see the wiki page Set Up SSH Access to the Mac for Native Libraries." -f $SshHost)
}
if ($LASTEXITCODE -ne 0)
{
    Stop-Build 'GNHAB007' ("The {0} SDK is not available on the Mac. Install Xcode, then run 'sudo xcode-select -s /Applications/Xcode.app/Contents/Developer' and 'sudo xcodebuild -license accept' on the Mac." -f $Sdk)
}

Invoke-Mac 'GNHAB008' 'Preparing the build folder on the Mac' ('mkdir -p {0}' -f $RemoteDir)
if ($Clean)
{
    Invoke-Mac 'GNHAB008' 'Cleaning the build folder on the Mac' ('rm -rf {0}/obj {0}/out' -f $RemoteDir)
}
# -C compresses on the wire: the tar file is about 18 MB, 3.5 MB compressed
Invoke-Tool 'GNHAB009' 'Copying the sources to the Mac' $Scp (@('-q', '-C') + @($script:SshOptions) + @($TarFile, ('{0}:{1}/gnh-src.tar' -f $SshHost, $RemoteDir)))
# A fresh tree, so that a file deleted from the repository cannot linger; tar keeps the files' times, so make still
# rebuilds only what changed
Invoke-Mac 'GNHAB009' 'Unpacking the sources on the Mac' ('rm -rf {0}/tree && mkdir -p {0}/tree && tar -xf {0}/gnh-src.tar -C {0}/tree' -f $RemoteDir)
Write-Host ('Build-AppleEngine.ps1: building {0} ({1}) on {2} in ~/{3}' -f $LibName, $Configuration, $SshHost, $RemoteDir)
Invoke-Mac 'GNHAB010' 'The engine build on the Mac' ('make -C {0}/tree -f engine.mk -j$(sysctl -n hw.ncpu) PLATFORM={1} CONFIG={2} MIN_OS={3} OBJDIR=../obj OUTDIR=../out' -f $RemoteDir, $Platform, $Configuration, $MinOS)

New-Item -ItemType Directory -Force -Path $OutDir | Out-Null
Invoke-Tool 'GNHAB011' 'Copying the library from the Mac' $Scp (@('-q') + @($script:SshOptions) + @(('{0}:{1}/out/{2}' -f $SshHost, $RemoteDir, $LibName), $LocalLib))
Invoke-Mac 'GNHAB011' 'Inspecting the library' ('lipo -info {0}/out/{1}' -f $RemoteDir, $LibName)
Write-Host ('Build-AppleEngine.ps1: {0} copied to {1}' -f $LibName, $LocalLib)
exit 0
