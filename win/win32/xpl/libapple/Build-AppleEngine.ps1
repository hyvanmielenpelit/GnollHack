<#
.SYNOPSIS
    Builds the GnollHack engine library for iOS (libgnollhackios.a) or macOS (libgnollhackmac.dylib) on a Mac over SSH.

.DESCRIPTION
    Run by gnollhackios.vcxproj and gnollhackmac.vcxproj. Packs the engine sources named in -SourceList, the headers of
    include\, win\win32\xpl\libshare\ and the platform folder, and LF copies of engine.mk and sync-tree.sh into a tar
    file in -IntDir, copies it to the Mac with scp, unpacks it there and copies into the build tree only the files whose
    content changed (sync-tree.sh), builds it with make and clang, and copies the library back to -OutDir. Objects
    persist on the Mac between runs, and make recompiles a source when it, a header, engine.mk or the compiler flags
    changed. Every run of the WSL data pipeline rewrites date_unix.h, so a solution build usually recompiles everything.

    For macOS, make also writes libgnollhackmac.dylib.dSYM with dsymutil whenever it is missing or older than the
    library. The script checks with dwarfdump that its arm64 UUID matches the library's, then copies it to -OutDir too.

    Each checkout has its own folder on the Mac, ~/<RemoteRoot>/<platform>-<Configuration>-<computer>-<hash>, where
    the hash is taken from the checkout's path, so worktrees and computers never share objects.

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
    The local folder the library, and for macOS its .dSYM, are copied to.

.PARAMETER SourceList
    A file with the full path of one C source per line, written by the GnhWriteAppleSourceList target.

.PARAMETER SshHost
    The OpenSSH host alias of the Mac.

.PARAMETER RemoteRoot
    The parent of the build folders on the Mac, relative to the home folder. Default: gnollhack-build.

.PARAMETER Clean
    Deletes the objects and the library on the Mac before building.

.PARAMETER CleanOnly
    Deletes the local library and its .dSYM and, when the Mac can be reached, this checkout's build folder on the Mac, then stops.

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

# Clang names files relative to the tree on the Mac; Visual Studio's Error List needs this checkout's paths
function Convert-MacDiagnostic
{
    param([string]$Line)
    $m = [regex]::Match($Line, '^(?<file>(?:src|include|sys|win|util)/[^:]+):(?<line>\d+):(?<col>\d+): (?<rest>(?:fatal error|error|warning|note): .*)$')
    if ($m.Success)
    {
        return ('{0}\{1}({2},{3}): {4}' -f $script:RepoRoot, $m.Groups['file'].Value.Replace('/', '\'), $m.Groups['line'].Value, $m.Groups['col'].Value, $m.Groups['rest'].Value)
    }
    $m = [regex]::Match($Line, '^(?<lead>In file included from |\s+from )(?<file>(?:src|include|sys|win|util)/[^:]+)(?<rest>:.*)$')
    if ($m.Success)
    {
        return ('{0}{1}\{2}{3}' -f $m.Groups['lead'].Value, $script:RepoRoot, $m.Groups['file'].Value.Replace('/', '\'), $m.Groups['rest'].Value)
    }
    return $Line
}

# The arm64 UUID that dwarfdump prints for a file on the Mac, or an empty string when it prints none
function Get-MacArm64Uuid
{
    param([string]$Path)
    $lines = @(& $script:Ssh @script:SshOptions $script:SshHost ('xcrun dwarfdump --uuid {0}' -f $Path))
    if ($LASTEXITCODE -ne 0)
    {
        return ''
    }
    foreach ($line in $lines)
    {
        $m = [regex]::Match([string]$line, '^UUID: (?<uuid>[0-9A-Fa-f-]+) \(arm64\) ')
        if ($m.Success)
        {
            return $m.Groups['uuid'].Value.ToUpperInvariant()
        }
    }
    return ''
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
# One folder per checkout: make decides what to rebuild from the objects already there
$Computer = ([string]$env:COMPUTERNAME).ToLowerInvariant() -replace '[^a-z0-9-]', '-'
if ([string]::IsNullOrEmpty($Computer))
{
    $Computer = 'pc'
}
$Sha = [Security.Cryptography.SHA256]::Create()
$RootHash = -join ($Sha.ComputeHash([Text.Encoding]::UTF8.GetBytes($RepoRoot.ToLowerInvariant()))[0..3] | ForEach-Object { $_.ToString('x2') })
$RemoteDir = '{0}/{1}-{2}-{3}-{4}' -f $RemoteRoot.TrimEnd('/'), $Platform, $Configuration, $Computer, $RootHash
$LocalLib = Join-Path $OutDir $LibName
$DsymName = $LibName + '.dSYM'
$LocalDsym = Join-Path $OutDir $DsymName
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
    if (($Platform -eq 'macos') -and (Test-Path $LocalDsym))
    {
        Remove-Item $LocalDsym -Recurse -Force
    }
    $script:Ssh = Find-Tool 'ssh.exe' 'OpenSSH\ssh.exe'
    & $script:Ssh @script:SshOptions $SshHost ('rm -rf {0}' -f $RemoteDir)
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
# The Mac's make and shell need LF; the repository copies of engine.mk and sync-tree.sh are CRLF on Windows
$Makefile = [IO.File]::ReadAllText((Join-Path $PSScriptRoot 'engine.mk')) -replace "`r`n", "`n"
[IO.File]::WriteAllText((Join-Path $IntDir 'engine.mk'), $Makefile, $Utf8)
$SyncScript = [IO.File]::ReadAllText((Join-Path $PSScriptRoot 'sync-tree.sh')) -replace "`r`n", "`n"
[IO.File]::WriteAllText((Join-Path $IntDir 'sync-tree.sh'), $SyncScript, $Utf8)
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
Invoke-Tool 'GNHAB005' 'Packing the makefiles' $Tar @('-r', '-f', $TarFile, '-C', $IntDir, 'engine.mk', 'gnh-sources.mk', 'sync-tree.sh')
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
    Invoke-Mac 'GNHAB008' 'Cleaning the build folder on the Mac' ('rm -rf {0}/obj {0}/out {0}/tree' -f $RemoteDir)
}
# -C compresses on the wire: the tar file is about 18 MB, 3.5 MB compressed
Invoke-Tool 'GNHAB009' 'Copying the sources to the Mac' $Scp (@('-q', '-C') + @($script:SshOptions) + @($TarFile, ('{0}:{1}/gnh-src.tar' -f $SshHost, $RemoteDir)))
# Unpacked beside the tree; sync-tree.sh then rewrites only changed files, so make never trusts Windows file times
Invoke-Mac 'GNHAB009' 'Unpacking the sources on the Mac' ('rm -rf {0}/incoming && mkdir -p {0}/incoming && tar -xf {0}/gnh-src.tar -C {0}/incoming && sh {0}/incoming/sync-tree.sh {0}/incoming {0}/tree' -f $RemoteDir)
Write-Host ('Build-AppleEngine.ps1: building {0} ({1}) on {2} in ~/{3}' -f $LibName, $Configuration, $SshHost, $RemoteDir)
try
{
    [Console]::OutputEncoding = $Utf8
}
catch
{
}
# 2>&1 on the Mac: Windows PowerShell 5.1 turns a native command's redirected stderr into errors
$MakeCommand = 'make -C {0}/tree -f engine.mk -j$(sysctl -n hw.ncpu) PLATFORM={1} CONFIG={2} MIN_OS={3} OBJDIR=../obj OUTDIR=../out 2>&1' -f $RemoteDir, $Platform, $Configuration, $MinOS
& $script:Ssh @script:SshOptions $SshHost $MakeCommand | ForEach-Object { Convert-MacDiagnostic $_ }
if ($LASTEXITCODE -ne 0)
{
    Stop-Build 'GNHAB010' ('The engine build on the Mac failed with exit code {0}.' -f $LASTEXITCODE)
}

New-Item -ItemType Directory -Force -Path $OutDir | Out-Null
Invoke-Tool 'GNHAB011' 'Copying the library from the Mac' $Scp (@('-q') + @($script:SshOptions) + @(('{0}:{1}/out/{2}' -f $SshHost, $RemoteDir, $LibName), $LocalLib))
Invoke-Mac 'GNHAB011' 'Inspecting the library' ('lipo -info {0}/out/{1}' -f $RemoteDir, $LibName)
Write-Host ('Build-AppleEngine.ps1: {0} copied to {1}' -f $LibName, $LocalLib)

if ($Platform -eq 'macos')
{
    if (Test-Path $LocalDsym)
    {
        Remove-Item $LocalDsym -Recurse -Force
    }
    $RemoteLib = '{0}/out/{1}' -f $RemoteDir, $LibName
    $RemoteDsym = '{0}/out/{1}' -f $RemoteDir, $DsymName
    $LibUuid = Get-MacArm64Uuid $RemoteLib
    $DsymUuid = Get-MacArm64Uuid $RemoteDsym
    if ([string]::IsNullOrEmpty($LibUuid))
    {
        Stop-Build 'GNHAB013' ('dwarfdump found no arm64 UUID in {0} on the Mac.' -f $RemoteLib)
    }
    if ([string]::IsNullOrEmpty($DsymUuid))
    {
        Stop-Build 'GNHAB013' ('dwarfdump found no arm64 UUID in {0} on the Mac.' -f $RemoteDsym)
    }
    if ($LibUuid -ne $DsymUuid)
    {
        Stop-Build 'GNHAB013' ('The arm64 UUID of {0} ({1}) does not match that of {2} ({3}).' -f $RemoteDsym, $DsymUuid, $RemoteLib, $LibUuid)
    }
    # -C compresses on the wire: DWARF compresses well
    Invoke-Tool 'GNHAB011' 'Copying the debug symbols from the Mac' $Scp (@('-q', '-C', '-r') + @($script:SshOptions) + @(('{0}:{1}' -f $SshHost, $RemoteDsym), $OutDir))
    Write-Host ('Build-AppleEngine.ps1: {0} (arm64 UUID {1}) copied to {2}' -f $DsymName, $DsymUuid, $LocalDsym)
}
exit 0
