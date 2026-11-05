<#
.SYNOPSIS
    Builds libgnhsentry.dylib, sentry-native for the macOS build, as a universal arm64 and x86_64 dylib on a Mac over
    SSH.

.DESCRIPTION
    Links libsentry-native.a from the Sentry NuGet package that GnollHackM restores into a dylib that exports only the
    functions named in gnhsentry.exports: the exports of win\win32\xpl\gnhsentrywin\gnhsentry.def except
    sentry_options_set_database_pathw, with the leading underscore of Mach-O symbol names. The package version is the
    GnhSentryVersion property in GnollHackM.csproj, which every Sentry and Sentry.Maui PackageReference must use as its
    Version. The sentry-native version in the library must be the one win\win32\xpl\gnhsentrywin\include\sentry.h
    declares, and the deployment target is GHMacDeploymentTarget in win\win32\vs\gnollhackmac.vcxproj.

    The library and an LF copy of gnhsentry.exports are copied to ~/<RemoteRoot>/sentry-native-<computer>-<hash> on
    the Mac, where the hash is taken from the checkout's path, as Build-AppleEngine.ps1 names its folders. The dylib is
    linked there with xcrun clang and checked: both architectures, exactly the listed exports in each slice, no
    dependency but libSystem, the deployment target as minos, and a linker-signed ad-hoc signature. Only then is it
    copied to Platforms\MacOS\libs\libgnhsentry.dylib, with sentry-native's license in
    Platforms\MacOS\licenses\sentry-native\. The folder on the Mac stays in place.

    Platforms\MacOS\libs\libgnhsentry.build.props then records the package and sentry-native versions, the SHA-256 of
    libsentry-native.a and of the dylib, a line-ending-independent hash of gnhsentry.exports, gnhsentry.def and
    include\sentry.h, the UUID of each slice, and the clang and macOS SDK versions. GnhSentryNative.targets compares
    them with the current inputs at build time.

    Run it after a Sentry upgrade, and after any edit to gnhsentry.def or gnhsentry.exports. The script never
    downloads anything: GnollHackM must have been restored, so that the Sentry package is in the NuGet package folder.

    The Mac is reached through an OpenSSH host alias with key-based authentication: -SshHost, else the environment
    variable GNOLLHACK_MAC_SSH_HOST, else gnollhack-mac. Every connection uses BatchMode.

    Errors are printed as "Build-MacSentryNative.ps1 : error GNHMS0nn: ...".

.PARAMETER SshHost
    The OpenSSH host alias of the Mac.

.PARAMETER RemoteRoot
    The parent of the build folder on the Mac, relative to the home folder. Default: gnollhack-build.

.EXAMPLE
    .\Build-MacSentryNative.ps1

    Builds, checks and copies libgnhsentry.dylib.
#>
[CmdletBinding()]
param(
    [string]$SshHost = '',
    [string]$RemoteRoot = 'gnollhack-build'
)

Set-StrictMode -Version 2
$ErrorActionPreference = 'Stop'

# ---------------------------------------------------------------------------------------------------
# Helper functions
# ---------------------------------------------------------------------------------------------------

function Stop-Build
{
    param([string]$Code, [string]$Message)
    Write-Host ('Build-MacSentryNative.ps1 : error {0}: {1}' -f $Code, $Message)
    exit 1
}

function Write-Step
{
    param([string]$Message)
    Write-Host ''
    Write-Host "==> $Message" -ForegroundColor Cyan
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
    Stop-Build 'GNHMS001' ('{0} was not found. It is part of Windows: install the OpenSSH Client under Settings > System > Optional features.' -f $Name)
}

# Runs a native tool with its standard error shown as text, so that Windows PowerShell does not turn it into a
# terminating error. Returns the exit code.
function Invoke-Native
{
    param([string]$FilePath, [string[]]$Arguments)
    $previous = $ErrorActionPreference
    $ErrorActionPreference = 'Continue'
    try
    {
        & $FilePath @Arguments 2>&1 | ForEach-Object { Write-Host "    $_" }
        return $LASTEXITCODE
    }
    finally
    {
        $ErrorActionPreference = $previous
    }
}

function Invoke-Tool
{
    param([string]$Code, [string]$What, [string]$FilePath, [string[]]$Arguments)
    $exitCode = Invoke-Native $FilePath $Arguments
    if ($exitCode -ne 0)
    {
        Stop-Build $Code ('{0} failed with exit code {1}.' -f $What, $exitCode)
    }
}

function Invoke-Mac
{
    param([string]$Code, [string]$What, [string]$Command)
    Invoke-Tool $Code $What $script:Ssh (@($script:SshOptions) + @($script:SshHost, $Command))
}

# Runs a command on the Mac and returns its standard output as an array of lines; the lines are shown only when the
# command fails
function Get-MacOutput
{
    param([string]$Code, [string]$What, [string]$Command)
    $previous = $ErrorActionPreference
    $ErrorActionPreference = 'Continue'
    try
    {
        $output = @(& $script:Ssh @script:SshOptions $script:SshHost $Command)
        $exitCode = $LASTEXITCODE
    }
    finally
    {
        $ErrorActionPreference = $previous
    }
    if ($exitCode -ne 0)
    {
        foreach ($line in $output)
        {
            Write-Host "    $line"
        }
        Stop-Build $Code ('{0} failed with exit code {1}.' -f $What, $exitCode)
    }
    return ,$output
}

# The export names listed in a module-definition file: the first word of each line after EXPORTS
function Get-DefExportNames
{
    param([string]$DefPath)
    $names = New-Object System.Collections.Generic.List[string]
    $inExports = $false
    foreach ($rawLine in [System.IO.File]::ReadAllLines($DefPath))
    {
        $line = $rawLine.Trim()
        if ($line.Length -eq 0 -or $line.StartsWith(';'))
        {
            continue
        }
        if ($line -cmatch '^(LIBRARY|NAME|EXPORTS|HEAPSIZE|SECTIONS|STACKSIZE|STUB|VERSION)(\s|$)')
        {
            $inExports = ($Matches[1] -ceq 'EXPORTS')
            continue
        }
        if ($inExports)
        {
            $names.Add(($line -split '[\s=@]')[0])
        }
    }
    return ,$names.ToArray()
}

# The sentry-native version a sentry.h declares, from its SENTRY_SDK_VERSION_MAJOR, _MINOR and _PATCH macros, each
# defined exactly once; $null when one is missing or repeated
function Get-SentryHeaderVersion
{
    param([string]$HeaderFile)
    $headerText = [System.IO.File]::ReadAllText($HeaderFile)
    $parts = New-Object System.Collections.Generic.List[string]
    foreach ($part in @('MAJOR', 'MINOR', 'PATCH'))
    {
        $pattern = '(?m)^[ \t]*#[ \t]*define[ \t]+SENTRY_SDK_VERSION_' + $part + '[ \t]+(\d+)[ \t]*\r?$'
        $found = [regex]::Matches($headerText, $pattern)
        if ($found.Count -ne 1)
        {
            return $null
        }
        $parts.Add($found[0].Groups[1].Value)
    }
    return ($parts -join '.')
}

# SHA-256 of a byte array as uppercase hex without dashes, the form Get-FileHash and MSBuild's GetFileHash emit
function Get-Sha256Hex
{
    param([byte[]]$Bytes)
    $sha = [System.Security.Cryptography.SHA256]::Create()
    try
    {
        $digest = $sha.ComputeHash($Bytes)
    }
    finally
    {
        $sha.Dispose()
    }
    return ([System.BitConverter]::ToString($digest)).Replace('-', '')
}

# The normalized text hash of the manifest: each file read as UTF-8 with any BOM dropped and CRLF turned into LF,
# the texts concatenated in order, encoded as UTF-8 without a BOM and hashed with SHA-256. GnhSentryNative.targets
# computes the same hash with its GnhGetNormalizedTextHash task.
function Get-NormalizedTextHash
{
    param([string[]]$Files)
    $builder = New-Object System.Text.StringBuilder
    foreach ($file in $Files)
    {
        [void]$builder.Append([System.IO.File]::ReadAllText($file).Replace("`r`n", "`n"))
    }
    $utf8 = New-Object System.Text.UTF8Encoding($false)
    return (Get-Sha256Hex -Bytes ($utf8.GetBytes($builder.ToString())))
}

# Uppercase hex SHA-256 of a file
function Get-FileSha256
{
    param([string]$FilePath)
    return (Get-FileHash -LiteralPath $FilePath -Algorithm SHA256).Hash.ToUpperInvariant()
}

# An LF copy of a text file, without a BOM
function Copy-AsLf
{
    param([string]$Source, [string]$Destination)
    $text = [IO.File]::ReadAllText($Source)
    $text = $text.Replace("`r`n", "`n")
    [IO.File]::WriteAllText($Destination, $text, (New-Object Text.UTF8Encoding $false))
}

# ---------------------------------------------------------------------------------------------------
# Settings and tools
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
if ($SshHost -notmatch '^[A-Za-z0-9._-]+$')
{
    Stop-Build 'GNHMS001' ("The SSH host '{0}' may contain only letters, digits and . _ -." -f $SshHost)
}
if ($RemoteRoot -notmatch '^[A-Za-z0-9._/-]+$')
{
    Stop-Build 'GNHMS001' ("The remote folder '{0}' may contain only letters, digits and . _ / -." -f $RemoteRoot)
}

$script:Ssh = Find-Tool 'ssh.exe' 'OpenSSH\ssh.exe'
$Scp = Find-Tool 'scp.exe' 'OpenSSH\scp.exe'
$Tar = Find-Tool 'tar.exe' 'tar.exe'
$script:SshOptions = @('-o', 'BatchMode=yes', '-o', 'ConnectTimeout=15')

# ---------------------------------------------------------------------------------------------------
# Paths, all derived from the location of this script: win\win32\xpl\GnollHackM\Platforms\MacOS\Packaging
# ---------------------------------------------------------------------------------------------------

$PackagingDir = $PSScriptRoot
$GnollHackMDir = [IO.Path]::GetFullPath((Join-Path $PackagingDir '..\..\..')).TrimEnd('\')
$RepoRoot = [IO.Path]::GetFullPath((Join-Path $GnollHackMDir '..\..\..\..')).TrimEnd('\')
$ProjectPath = Join-Path $GnollHackMDir 'GnollHackM.csproj'
$MacProjectPath = Join-Path $RepoRoot 'win\win32\vs\gnollhackmac.vcxproj'
$ExportsPath = Join-Path $PackagingDir 'gnhsentry.exports'
$GnhSentryDir = Join-Path $RepoRoot 'win\win32\xpl\gnhsentrywin'
$DefPath = Join-Path $GnhSentryDir 'gnhsentry.def'
$HeaderPath = Join-Path $GnhSentryDir 'include\sentry.h'
$LinuxLicensePath = Join-Path $GnollHackMDir 'Platforms\Linux\licenses\sentry-native\LICENSE'

$MacLibsDir = Join-Path $GnollHackMDir 'Platforms\MacOS\libs'
$MacLicenseDir = Join-Path $GnollHackMDir 'Platforms\MacOS\licenses\sentry-native'
$TargetDylibPath = Join-Path $MacLibsDir 'libgnhsentry.dylib'
$TargetLicensePath = Join-Path $MacLicenseDir 'LICENSE'
$ManifestPath = Join-Path $MacLibsDir 'libgnhsentry.build.props'

# One folder per checkout on the Mac, named as Build-AppleEngine.ps1 names its folders
$Computer = ([string]$env:COMPUTERNAME).ToLowerInvariant() -replace '[^a-z0-9-]', '-'
if ([string]::IsNullOrEmpty($Computer))
{
    $Computer = 'pc'
}
$Sha = [Security.Cryptography.SHA256]::Create()
$RootHash = -join ($Sha.ComputeHash([Text.Encoding]::UTF8.GetBytes($RepoRoot.ToLowerInvariant()))[0..3] | ForEach-Object { $_.ToString('x2') })
$RemoteDir = '{0}/sentry-native-{1}-{2}' -f $RemoteRoot.TrimEnd('/'), $Computer, $RootHash
$RemoteDylib = '{0}/libgnhsentry.dylib' -f $RemoteDir

$StageDir = Join-Path ([IO.Path]::GetTempPath()) ('gnhsentry-mac-{0}' -f $RootHash)
$TarFile = Join-Path $StageDir 'stage.tar'
$CopiedDylib = Join-Path $StageDir 'libgnhsentry.dylib.copied'

# ---------------------------------------------------------------------------------------------------
# 1. The inputs on Windows
# ---------------------------------------------------------------------------------------------------

Write-Step 'Reading the inputs'

if (-not (Test-Path -LiteralPath $ProjectPath -PathType Leaf))
{
    Stop-Build 'GNHMS002' ("GnollHackM.csproj not found at '{0}'. The script must stay in Platforms\MacOS\Packaging." -f $ProjectPath)
}
$project = New-Object System.Xml.XmlDocument
$project.Load($ProjectPath)
$versionNodes = @($project.SelectNodes('//PropertyGroup/GnhSentryVersion'))
if ($versionNodes.Count -ne 1 -or [string]::IsNullOrWhiteSpace($versionNodes[0].InnerText))
{
    Stop-Build 'GNHMS002' ("Expected one non-empty GnhSentryVersion property in '{0}', found {1}." -f $ProjectPath, $versionNodes.Count)
}
$SentryPackageVersion = $versionNodes[0].InnerText.Trim()
$sentryReferences = @($project.SelectNodes("//ItemGroup/PackageReference[@Include='Sentry.Maui' or @Include='Sentry']"))
if ($sentryReferences.Count -eq 0)
{
    Stop-Build 'GNHMS002' ("'{0}' has no PackageReference to Sentry or Sentry.Maui." -f $ProjectPath)
}
foreach ($reference in $sentryReferences)
{
    $referenceVersion = $reference.GetAttribute('Version')
    if ($referenceVersion -ne '$(GnhSentryVersion)')
    {
        Stop-Build 'GNHMS002' (("The PackageReference to {0} in '{1}' has Version=`"{2}`"; it must be " + 'Version="$(GnhSentryVersion)".') -f $reference.GetAttribute('Include'), $ProjectPath, $referenceVersion)
    }
}
Write-Host "Sentry $SentryPackageVersion (GnhSentryVersion)"

# gnhsentry.exports: the exports of gnhsentry.def without the wide-character path setter, in the same order
foreach ($required in @($DefPath, $ExportsPath))
{
    if (-not (Test-Path -LiteralPath $required -PathType Leaf))
    {
        Stop-Build 'GNHMS003' ("'{0}' is missing." -f $required)
    }
}
$defExports = Get-DefExportNames $DefPath
$ExpectedExports = @($defExports | Where-Object { $_ -ne 'sentry_options_set_database_pathw' } | ForEach-Object { '_' + $_ })
$ListedExports = @([IO.File]::ReadAllLines($ExportsPath) | ForEach-Object { $_.Trim() } | Where-Object { $_.Length -gt 0 })
if ($ExpectedExports.Count -eq 0)
{
    Stop-Build 'GNHMS003' ("No exports found in '{0}'." -f $DefPath)
}
$exportCount = [Math]::Max($ExpectedExports.Count, $ListedExports.Count)
for ($i = 0; $i -lt $exportCount; $i++)
{
    $expected = $null
    $listed = $null
    if ($i -lt $ExpectedExports.Count)
    {
        $expected = $ExpectedExports[$i]
    }
    if ($i -lt $ListedExports.Count)
    {
        $listed = $ListedExports[$i]
    }
    if ($expected -cne $listed)
    {
        Stop-Build 'GNHMS003' ("Line {0} of gnhsentry.exports is '{1}', but gnhsentry.def gives '{2}'. gnhsentry.exports lists every export of gnhsentry.def except sentry_options_set_database_pathw, in order, each with a leading underscore." -f ($i + 1), $listed, $expected)
    }
}
Write-Host "$($ListedExports.Count) exports (gnhsentry.exports matches gnhsentry.def)"

if (-not (Test-Path -LiteralPath $HeaderPath -PathType Leaf))
{
    Stop-Build 'GNHMS004' ("sentry-native's header not found at '{0}'." -f $HeaderPath)
}
$SentryNativeVersion = Get-SentryHeaderVersion $HeaderPath
if ($null -eq $SentryNativeVersion)
{
    Stop-Build 'GNHMS004' ("'{0}' does not define each of SENTRY_SDK_VERSION_MAJOR, _MINOR and _PATCH exactly once." -f $HeaderPath)
}
Write-Host "sentry-native $SentryNativeVersion (include\sentry.h)"

if (-not [string]::IsNullOrWhiteSpace($env:NUGET_PACKAGES))
{
    $NuGetRoot = $env:NUGET_PACKAGES
}
else
{
    $NuGetRoot = Join-Path $env:USERPROFILE '.nuget\packages'
}
$SentryNativeLib = Join-Path $NuGetRoot ('sentry\{0}\sentry-native\osx\libsentry-native.a' -f $SentryPackageVersion.ToLowerInvariant())
if (-not (Test-Path -LiteralPath $SentryNativeLib -PathType Leaf))
{
    Stop-Build 'GNHMS004' ("'{0}' is missing: restore GnollHackM first." -f $SentryNativeLib)
}
Write-Host $SentryNativeLib

# The universal library holds the string once per object that has it, in both slices
$libText = [System.Text.Encoding]::ASCII.GetString([System.IO.File]::ReadAllBytes($SentryNativeLib))
$libVersions = @([regex]::Matches($libText, 'sentry\.native\.dotnet/(\d+\.\d+\.\d+)') |
    ForEach-Object { $_.Groups[1].Value } |
    Sort-Object -Unique)
if ($libVersions.Count -ne 1)
{
    Stop-Build 'GNHMS004' ("Expected one sentry.native.dotnet/<version> string in '{0}', found: [{1}]." -f $SentryNativeLib, ($libVersions -join ', '))
}
if ($libVersions[0] -ne $SentryNativeVersion)
{
    Stop-Build 'GNHMS004' ("libsentry-native.a in Sentry {0} is sentry-native {1}, but include\sentry.h is {2}. Get sentry.h at tag {1} and run Compare-SentryNativeApi.ps1 -NewHeader <path>; see win\win32\xpl\gnhsentrywin\README.md." -f $SentryPackageVersion, $libVersions[0], $SentryNativeVersion)
}
$SentryNativeLibSha256 = Get-FileSha256 $SentryNativeLib
Write-Host "    sentry.native.dotnet/$($libVersions[0])"

if (-not (Test-Path -LiteralPath $MacProjectPath -PathType Leaf))
{
    Stop-Build 'GNHMS013' ("'{0}' is missing; it defines the deployment target." -f $MacProjectPath)
}
$macProject = New-Object System.Xml.XmlDocument
$macProject.Load($MacProjectPath)
$namespaces = New-Object System.Xml.XmlNamespaceManager($macProject.NameTable)
$namespaces.AddNamespace('msb', 'http://schemas.microsoft.com/developer/msbuild/2003')
$targetNodes = @($macProject.SelectNodes('//msb:PropertyGroup/msb:GHMacDeploymentTarget', $namespaces))
if ($targetNodes.Count -ne 1 -or $targetNodes[0].InnerText.Trim() -notmatch '^\d+(\.\d+){1,2}$')
{
    Stop-Build 'GNHMS013' ("Expected one GHMacDeploymentTarget property with a version such as 15.0 in '{0}', found {1}." -f $MacProjectPath, $targetNodes.Count)
}
$DeploymentTarget = $targetNodes[0].InnerText.Trim()
Write-Host "macOS $DeploymentTarget (GHMacDeploymentTarget)"

if (-not (Test-Path -LiteralPath $LinuxLicensePath -PathType Leaf))
{
    Stop-Build 'GNHMS007' ("sentry-native's license not found at '{0}'." -f $LinuxLicensePath)
}

# ---------------------------------------------------------------------------------------------------
# 2. Link on the Mac
# ---------------------------------------------------------------------------------------------------

Write-Step ('Linking libgnhsentry.dylib on {0} in ~/{1}' -f $SshHost, $RemoteDir)

if (Test-Path -LiteralPath $StageDir)
{
    Remove-Item -LiteralPath $StageDir -Recurse -Force
}
New-Item -ItemType Directory -Force -Path $StageDir | Out-Null
Copy-Item -LiteralPath $SentryNativeLib -Destination (Join-Path $StageDir 'libsentry-native.a')
Copy-AsLf $ExportsPath (Join-Path $StageDir 'gnhsentry.exports')
# The link script runs in its own folder on the Mac; its argument is the deployment target
$linkScript = @(
    '#!/bin/bash',
    'set -eu',
    'cd "$(dirname "$0")"',
    'rm -f empty.c empty.o libgnhsentry.dylib',
    ': > empty.c',
    'xcrun clang -arch arm64 -arch x86_64 -mmacosx-version-min="$1" -c empty.c -o empty.o',
    'xcrun clang -arch arm64 -arch x86_64 -mmacosx-version-min="$1" -dynamiclib \',
    '    -install_name @rpath/libgnhsentry.dylib -Wl,-force_load,libsentry-native.a \',
    '    -Wl,-exported_symbols_list,gnhsentry.exports -o libgnhsentry.dylib empty.o'
)
[IO.File]::WriteAllText((Join-Path $StageDir 'link.sh'), (($linkScript -join "`n") + "`n"), (New-Object Text.UTF8Encoding $false))
Invoke-Tool 'GNHMS005' 'Packing the library' $Tar @('-c', '-f', $TarFile, '-C', $StageDir, 'libsentry-native.a', 'gnhsentry.exports', 'link.sh')

if ((Invoke-Native $script:Ssh (@($script:SshOptions) + @($SshHost, 'true'))) -ne 0)
{
    Stop-Build 'GNHMS005' ("Cannot connect to the Mac as '{0}'. Check that the Mac is awake, that Remote Login is on, and that 'ssh {0}' logs in without a password; see the wiki page Set Up SSH Access to the Mac for Native Libraries." -f $SshHost)
}
# No quotes in remote commands: Windows PowerShell mangles them on the way to ssh, and the remote paths are checked
# to hold no spaces
Invoke-Mac 'GNHMS005' 'Preparing the remote folder' ('rm -rf {0} && mkdir -p {0}' -f $RemoteDir)
Invoke-Tool 'GNHMS005' 'Copying the library to the Mac' $Scp (@('-q', '-C') + @($script:SshOptions) + @($TarFile, ('{0}:{1}/stage.tar' -f $SshHost, $RemoteDir)))
Invoke-Mac 'GNHMS005' 'Unpacking the library on the Mac' ('cd {0} && tar -xf stage.tar && rm stage.tar' -f $RemoteDir)
Invoke-Mac 'GNHMS006' 'The link on the Mac' ('bash {0}/link.sh {1} 2>&1' -f $RemoteDir, $DeploymentTarget)

# ---------------------------------------------------------------------------------------------------
# 3. Checks on the Mac
# ---------------------------------------------------------------------------------------------------

Write-Step 'Checking the architectures'

$lipoOutput = Get-MacOutput 'GNHMS010' 'lipo -archs' ('lipo -archs {0}' -f $RemoteDylib)
$archs = @(($lipoOutput -join ' ') -split '\s+' | Where-Object { $_.Length -gt 0 } | Sort-Object)
if (($archs -join ' ') -ne 'arm64 x86_64')
{
    Stop-Build 'GNHMS010' ("lipo -archs lists [{0}] for libgnhsentry.dylib, not arm64 and x86_64." -f ($archs -join ', '))
}
Write-Host '    arm64 x86_64'

Write-Step 'Checking the exports'

$sortedExports = @($ListedExports | Sort-Object -CaseSensitive)
foreach ($arch in @('arm64', 'x86_64'))
{
    $nmOutput = Get-MacOutput 'GNHMS011' ('nm -gU -arch {0}' -f $arch) ('nm -gU -arch {0} {1}' -f $arch, $RemoteDylib)
    $symbols = @($nmOutput |
        ForEach-Object { [regex]::Match($_, '^\s*[0-9A-Fa-f]+\s+\S\s+(\S+)\s*$') } |
        Where-Object { $_.Success } |
        ForEach-Object { $_.Groups[1].Value } |
        Sort-Object -CaseSensitive -Unique)
    $missing = @($sortedExports | Where-Object { $symbols -cnotcontains $_ })
    $extra = @($symbols | Where-Object { $sortedExports -cnotcontains $_ })
    if ($missing.Count -gt 0 -or $extra.Count -gt 0)
    {
        Stop-Build 'GNHMS011' ("The {0} slice exports differ from gnhsentry.exports: missing [{1}], extra [{2}]." -f $arch, ($missing -join ', '), ($extra -join ', '))
    }
    Write-Host "    $arch`: $($symbols.Count) exports"
}

Write-Step 'Checking the dependencies'

$allowedLibraries = @('@rpath/libgnhsentry.dylib', '/usr/lib/libSystem.B.dylib')
$otoolLOutput = Get-MacOutput 'GNHMS012' 'otool -L' ('otool -L {0}' -f $RemoteDylib)
$libraries = @($otoolLOutput |
    ForEach-Object { [regex]::Match($_, '^\s+(\S.*?) \(compatibility version') } |
    Where-Object { $_.Success } |
    ForEach-Object { $_.Groups[1].Value } |
    Sort-Object -Unique)
$otherLibraries = @($libraries | Where-Object { $allowedLibraries -notcontains $_ })
$absentLibraries = @($allowedLibraries | Where-Object { $libraries -notcontains $_ })
if ($otherLibraries.Count -gt 0 -or $absentLibraries.Count -gt 0)
{
    Stop-Build 'GNHMS012' ("otool -L lists [{0}] for libgnhsentry.dylib; it must name only {1}." -f ($libraries -join ', '), ($allowedLibraries -join ' and '))
}
foreach ($library in $libraries)
{
    Write-Host "    $library"
}

Write-Step 'Checking the deployment target'

foreach ($arch in @('arm64', 'x86_64'))
{
    $loadCommands = Get-MacOutput 'GNHMS013' ('otool -arch {0} -l' -f $arch) ('otool -arch {0} -l {1}' -f $arch, $RemoteDylib)
    $minOsValues = New-Object System.Collections.Generic.List[string]
    $inBuildVersion = $false
    foreach ($line in $loadCommands)
    {
        if ($line -match '^\s*cmd\s+(\S+)')
        {
            $inBuildVersion = ($Matches[1] -eq 'LC_BUILD_VERSION')
        }
        elseif ($inBuildVersion -and $line -match '^\s*minos\s+(\S+)')
        {
            $minOsValues.Add($Matches[1])
        }
    }
    if ($minOsValues.Count -ne 1 -or $minOsValues[0] -ne $DeploymentTarget)
    {
        Stop-Build 'GNHMS013' ("The {0} slice has LC_BUILD_VERSION minos [{1}], not {2}." -f $arch, ($minOsValues -join ', '), $DeploymentTarget)
    }
    Write-Host "    $arch`: minos $($minOsValues[0])"
}

Write-Step 'Checking the signature'

# The linker signs the arm64 slice ad hoc; package-mac.sh signs the dylib again in the bundle
$codesignOutput = Get-MacOutput 'GNHMS014' 'codesign -dv' ('codesign -dv -a arm64 {0} 2>&1' -f $RemoteDylib)
$codeDirectory = @($codesignOutput |
    ForEach-Object { [regex]::Match($_, '^CodeDirectory .*flags=0x[0-9a-fA-F]+\(([^)]*)\)') } |
    Where-Object { $_.Success } |
    ForEach-Object { $_.Groups[1].Value })
$signatureLines = @($codesignOutput | Where-Object { $_ -match '^Signature=adhoc\s*$' })
if ($codeDirectory.Count -ne 1 -or ($codeDirectory[0] -split ',') -notcontains 'adhoc' -or ($codeDirectory[0] -split ',') -notcontains 'linker-signed' -or $signatureLines.Count -ne 1)
{
    foreach ($line in $codesignOutput)
    {
        Write-Host "    $line"
    }
    Stop-Build 'GNHMS014' 'The arm64 slice of libgnhsentry.dylib does not have a linker-signed ad-hoc signature.'
}
Write-Host "    arm64: flags ($($codeDirectory[0])), Signature=adhoc"

Write-Step 'Reading the UUIDs and the toolset'

$uuidOutput = Get-MacOutput 'GNHMS015' 'dwarfdump --uuid' ('dwarfdump --uuid {0}' -f $RemoteDylib)
$Uuids = @{}
foreach ($line in $uuidOutput)
{
    $m = [regex]::Match($line, '^UUID: ([0-9A-Fa-f]{8}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{12}) \((\S+)\)')
    if ($m.Success)
    {
        $Uuids[$m.Groups[2].Value] = $m.Groups[1].Value
    }
}
foreach ($arch in @('arm64', 'x86_64'))
{
    if (-not $Uuids.ContainsKey($arch))
    {
        Stop-Build 'GNHMS015' ("dwarfdump --uuid gives no UUID for the {0} slice of libgnhsentry.dylib." -f $arch)
    }
    Write-Host "    $arch`: $($Uuids[$arch])"
}

$clangOutput = Get-MacOutput 'GNHMS006' 'xcrun clang --version' 'xcrun clang --version | head -1'
$sdkOutput = Get-MacOutput 'GNHMS006' 'xcrun --show-sdk-version' 'xcrun --show-sdk-version'
$clangVersion = @($clangOutput | Where-Object { -not [string]::IsNullOrWhiteSpace($_) }) | Select-Object -First 1
$sdkVersion = @($sdkOutput | Where-Object { -not [string]::IsNullOrWhiteSpace($_) }) | Select-Object -First 1
if ($null -eq $clangVersion -or $null -eq $sdkVersion)
{
    Stop-Build 'GNHMS006' 'xcrun did not report the clang or macOS SDK version.'
}
$Toolset = '{0}, macOS SDK {1}' -f $clangVersion.Trim(), $sdkVersion.Trim()
Write-Host "    $Toolset"

# ---------------------------------------------------------------------------------------------------
# 4. Copy the checked dylib and the license
# ---------------------------------------------------------------------------------------------------

Write-Step 'Copying libgnhsentry.dylib and its license'

Invoke-Tool 'GNHMS016' 'Copying the dylib from the Mac' $Scp (@('-q') + @($script:SshOptions) + @(('{0}:{1}' -f $SshHost, $RemoteDylib), $CopiedDylib))
$shasumOutput = Get-MacOutput 'GNHMS016' 'shasum -a 256' ('shasum -a 256 {0}' -f $RemoteDylib)
$remoteHash = (($shasumOutput -join ' ').Trim() -split '\s+')[0].ToUpperInvariant()
$DylibSha256 = Get-FileSha256 $CopiedDylib
if ($remoteHash -ne $DylibSha256)
{
    Stop-Build 'GNHMS016' ("The copied dylib has SHA-256 {0}, but the dylib on the Mac has {1}. Run the script again." -f $DylibSha256, $remoteHash)
}

try
{
    foreach ($dir in @($MacLibsDir, $MacLicenseDir))
    {
        if (-not (Test-Path -LiteralPath $dir -PathType Container))
        {
            New-Item -ItemType Directory -Path $dir | Out-Null
        }
    }
    Move-Item -LiteralPath $CopiedDylib -Destination $TargetDylibPath -Force
    Copy-Item -LiteralPath $LinuxLicensePath -Destination $TargetLicensePath -Force
}
catch
{
    Stop-Build 'GNHMS016' ('Copying the dylib or the license failed: {0}' -f $_.Exception.Message)
}
Write-Host "Copied libgnhsentry.dylib to $MacLibsDir and the license to $MacLicenseDir"

# ---------------------------------------------------------------------------------------------------
# 5. The manifest that GnhSentryNative.targets checks
# ---------------------------------------------------------------------------------------------------

Write-Step 'Writing libgnhsentry.build.props'

$manifestProperties = [ordered]@{
    GnhMacSentryPackageVersion = $SentryPackageVersion
    GnhMacSentryNativeVersion = $SentryNativeVersion
    GnhMacSentryLibSha256 = $SentryNativeLibSha256
    GnhMacSentrySourceSha256 = Get-NormalizedTextHash @($ExportsPath, $DefPath, $HeaderPath)
    GnhMacSentryDylibSha256 = Get-FileSha256 $TargetDylibPath
    GnhMacSentryArm64Uuid = $Uuids['arm64']
    GnhMacSentryX64Uuid = $Uuids['x86_64']
    GnhMacSentryToolset = $Toolset
}
$manifestLines = New-Object System.Collections.Generic.List[string]
$manifestLines.Add('<!-- Written by Build-MacSentryNative.ps1; checked by GnhSentryNative.targets. Do not edit. -->')
$manifestLines.Add('<Project>')
$manifestLines.Add('  <PropertyGroup>')
foreach ($propertyName in $manifestProperties.Keys)
{
    $propertyValue = [System.Security.SecurityElement]::Escape([string]$manifestProperties[$propertyName])
    $manifestLines.Add("    <$propertyName>$propertyValue</$propertyName>")
    Write-Host "    $propertyName = $($manifestProperties[$propertyName])"
}
$manifestLines.Add('  </PropertyGroup>')
$manifestLines.Add('</Project>')
try
{
    [System.IO.File]::WriteAllText($ManifestPath, (($manifestLines -join "`r`n") + "`r`n"), (New-Object System.Text.UTF8Encoding($false)))
}
catch
{
    Stop-Build 'GNHMS016' ("Writing '{0}' failed: {1}" -f $ManifestPath, $_.Exception.Message)
}
Write-Host "Wrote $ManifestPath"
Remove-Item -LiteralPath $StageDir -Recurse -Force -ErrorAction SilentlyContinue

# ---------------------------------------------------------------------------------------------------
# 6. Summary
# ---------------------------------------------------------------------------------------------------

Write-Step "libgnhsentry.dylib from Sentry $SentryPackageVersion (sentry-native $SentryNativeVersion)"

$item = Get-Item -LiteralPath $TargetDylibPath
Write-Host $TargetDylibPath
Write-Host "    size      $($item.Length) bytes"
Write-Host "    sha256    $($DylibSha256.ToLowerInvariant())"
Write-Host "    arm64     $($Uuids['arm64'])"
Write-Host "    x86_64    $($Uuids['x86_64'])"
Write-Host "    macOS     $DeploymentTarget"
Write-Host "    toolset   $Toolset"
Write-Host $TargetLicensePath
Write-Host $ManifestPath
Write-Host ('On the Mac: ~/{0}' -f $RemoteDir)
exit 0
