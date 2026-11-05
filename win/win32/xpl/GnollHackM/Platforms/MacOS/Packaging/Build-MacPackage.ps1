<#
.SYNOPSIS
    Builds GnollHack.app for macOS: a single-file osx-arm64 publish on Windows, then assembly, signing, optional
    notarization, a DMG and Steam depot content on the Mac over SSH.

.DESCRIPTION
    Publishes GnollHackM.csproj for osx-arm64 as a single file (native libraries and content left beside the
    apphost), stages the publish with the packaging files (LF copies of package-mac.sh, Info.plist.template,
    GnollHack.entitlements and the Steam templates, plus appicon.png), copies the tar file to the Mac with scp and runs
    package-mac.sh there. The zipped app, the DMG, SHA256SUMS and BUILD-INFO are copied back to
    <OutputRoot>\<version>-<build>\. With the Steam target, the depot content and its build scripts are copied back to
    its steam\ folder too, each file checked against its SHA-256 on the Mac, and stay on the Mac as well. A person
    uploads them with ContentBuilder or steamcmd from either; this script never uploads and never starts the game.

    The native engine, Platforms\MacOS\libs\libgnollhackmac.dylib, and the game data, Platforms\Android\gnh, come from a
    GnollHack.sln build with the macOS platform in Release, as for the development flavor.

    The Mac is reached through an OpenSSH host alias with key-based authentication: -SshHost, else the environment
    variable GNOLLHACK_MAC_SSH_HOST, else gnollhack-mac, as Build-AppleEngine.ps1 does. Every connection uses
    BatchMode.

    Errors are printed as "Build-MacPackage.ps1 : error GNHMP0nn: ..."; package-mac.sh's own errors are GNHMP1nn.

.PARAMETER Targets
    App (the zipped GnollHack.app), Dmg and Steam. Default: App.

.PARAMETER SigningIdentity
    The codesign identity, such as "Developer ID Application: Name (CUG8RS847T)"; - (the default) signs ad hoc, for
    builds that only run on the Mac that made them.

.PARAMETER NotaryProfile
    The notarytool keychain profile on the Mac. Empty (the default) skips notarization; it needs a Developer ID identity.

.PARAMETER Keychain
    The keychain on the Mac that holds the Developer ID identity and the notarytool profile. An SSH session cannot use
    the login keychain, which stays locked there, so a release uses a dedicated signing keychain. Default, when
    -SigningIdentity is not -: ~/Library/Keychains/gnollhack-signing.keychain-db. An empty string passes no keychain.
    Over SSH that fails, because the locked login keychain is used, so it serves only package-mac.sh run by hand in
    Terminal on the Mac.

.PARAMETER KeychainPasswordFile
    The file on the Mac with the signing keychain's password, readable only by its owner. Default:
    ~/.gnollhack/signing-keychain-password.

.PARAMETER BundleId
    CFBundleIdentifier. Default: com.soundmindgames.GnollHackMac.

.PARAMETER SteamAppId
    The Steam app id, for the Steam target.

.PARAMETER SteamDepotId
    The macOS depot id, for the Steam target.

.PARAMETER SteamBranch
    The branch the Steam build is set live on; empty sets nothing live.

.PARAMETER BuildDescription
    The Steam build description.

.PARAMETER SkipPublish
    Packages the existing staged publish instead of publishing again.

.PARAMETER NoSentryUpload
    Passes -p:GnhSentryUpload=false to the publish.

.PARAMETER DevCopy
    Also copies the app to ~/gnollhack-dev/GnollHack.app on the Mac, for testing.

.PARAMETER SshHost
    The OpenSSH host alias of the Mac.

.PARAMETER RemoteRoot
    The parent folder on the Mac, relative to the home folder. Default: gnollhack-build.

.PARAMETER OutputRoot
    The local parent folder of the results. Default: win\win32\xpl\GnollHackM\bin\MacPackages.

.EXAMPLE
    .\Build-MacPackage.ps1 -Targets App -DevCopy

.EXAMPLE
    .\Build-MacPackage.ps1 -Targets App,Dmg,Steam -SigningIdentity "Developer ID Application: Name (CUG8RS847T)" -NotaryProfile gnollhack-notary -SteamAppId 123 -SteamDepotId 124 -SteamBranch beta
#>
[CmdletBinding()]
param(
    [ValidateSet('App', 'Dmg', 'Steam')]
    [string[]]$Targets = @('App'),

    [string]$SigningIdentity = '-',

    [string]$NotaryProfile = '',

    [string]$Keychain = '~/Library/Keychains/gnollhack-signing.keychain-db',

    [string]$KeychainPasswordFile = '~/.gnollhack/signing-keychain-password',

    [string]$BundleId = 'com.soundmindgames.GnollHackMac',

    [string]$SteamAppId = '',

    [string]$SteamDepotId = '',

    [string]$SteamBranch = '',

    [string]$BuildDescription = '',

    [switch]$SkipPublish,

    [switch]$NoSentryUpload,

    [switch]$DevCopy,

    [string]$SshHost = '',

    [string]$RemoteRoot = 'gnollhack-build',

    [string]$OutputRoot = ''
)

Set-StrictMode -Version 2
$ErrorActionPreference = 'Stop'

function Stop-Build
{
    param([string]$Code, [string]$Message)
    Write-Host ('Build-MacPackage.ps1 : error {0}: {1}' -f $Code, $Message)
    exit 1
}

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
    Stop-Build 'GNHMP001' ('{0} was not found. It is part of Windows: install the OpenSSH Client under Settings > System > Optional features.' -f $Name)
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

# Runs a native tool with its standard error shown as text: Windows PowerShell would otherwise turn a redirected
# stderr line, such as codesign's "replacing existing signature", into a terminating error. Returns the exit code.
function Invoke-Native
{
    param([string]$FilePath, [string[]]$Arguments)
    $previous = $ErrorActionPreference
    $ErrorActionPreference = 'Continue'
    try
    {
        & $FilePath @Arguments 2>&1 | ForEach-Object { Write-Host $_ }
        return $LASTEXITCODE
    }
    finally
    {
        $ErrorActionPreference = $previous
    }
}

# A bash single-quoted word
function ConvertTo-ShellWord
{
    param([string]$Text)
    return "'" + $Text.Replace("'", "'\''") + "'"
}

# Compares the files under $Root with a shasum -a 256 list whose paths are relative to $Root. Returns the first
# missing, extra or different file as text, or $null when all match.
function Find-SteamContentMismatch
{
    param([string]$Root, [string]$HashList)
    $expected = @{}
    foreach ($line in [IO.File]::ReadAllLines($HashList))
    {
        if ($line -match '^([0-9a-f]{64}) [ *](.+)$')
        {
            $expected[$Matches[2]] = $Matches[1]
        }
    }
    if ($expected.Count -eq 0)
    {
        return ('{0} lists no files' -f $HashList)
    }
    $rootPath = (Resolve-Path $Root).Path.TrimEnd('\')
    $seen = @{}
    foreach ($file in (Get-ChildItem -LiteralPath (Join-Path $rootPath 'content') -Recurse -File -Force))
    {
        $relative = $file.FullName.Substring($rootPath.Length + 1).Replace('\', '/')
        if (-not $expected.ContainsKey($relative))
        {
            return ('extra file {0}' -f $relative)
        }
        if ((Get-FileHash -LiteralPath $file.FullName -Algorithm SHA256).Hash.ToLowerInvariant() -ne $expected[$relative])
        {
            return ('{0} differs' -f $relative)
        }
        $seen[$relative] = $true
    }
    foreach ($relative in $expected.Keys)
    {
        if (-not $seen.ContainsKey($relative))
        {
            return ('{0} is missing' -f $relative)
        }
    }
    return $null
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
# Paths, inputs and version
# ---------------------------------------------------------------------------------------------------

$PackagingDir = $PSScriptRoot
$GnollHackMDir = (Resolve-Path (Join-Path $PackagingDir '..\..\..')).Path
$RepoRoot = (Resolve-Path (Join-Path $GnollHackMDir '..\..\..\..')).Path
$Csproj = Join-Path $GnollHackMDir 'GnollHackM.csproj'
$AvaloniaDir = Join-Path $GnollHackMDir 'Platforms\Avalonia'

$EngineLibrary = Join-Path $GnollHackMDir 'Platforms\MacOS\libs\libgnollhackmac.dylib'
$GameData = Join-Path $GnollHackMDir 'Platforms\Android\gnh\nhdat'
foreach ($required in @($EngineLibrary, $GameData))
{
    if (-not (Test-Path $required))
    {
        Stop-Build 'GNHMP002' ('{0} is missing. Build win\win32\vs\GnollHack.sln with the macOS solution platform in Release.' -f $required)
    }
}
$AppIconMaster = Join-Path $GnollHackMDir 'Resources\AppIcon\macOS\appicon-macos.png'
if (-not (Test-Path $AppIconMaster))
{
    Stop-Build 'GNHMP002' ('{0} is missing. Make it with Platforms\MacOS\Packaging\New-MacAppIcon.ps1.' -f $AppIconMaster)
}

$projectText = [IO.File]::ReadAllText($Csproj)
$Version = [regex]::Match($projectText, '<ApplicationDisplayVersion>\s*([^<\s]+)\s*</ApplicationDisplayVersion>').Groups[1].Value
$Build = [regex]::Match($projectText, '<ApplicationVersion>\s*([^<\s]+)\s*</ApplicationVersion>').Groups[1].Value
if ([string]::IsNullOrEmpty($Version) -or [string]::IsNullOrEmpty($Build))
{
    Stop-Build 'GNHMP003' 'ApplicationDisplayVersion or ApplicationVersion was not found in GnollHackM.csproj.'
}

$Commit = (& git -C $RepoRoot rev-parse HEAD 2>$null)
if ($LASTEXITCODE -ne 0 -or [string]::IsNullOrEmpty($Commit))
{
    $Commit = 'unknown'
}
elseif ((& git -C $RepoRoot status --porcelain 2>$null | Measure-Object).Count -gt 0)
{
    $Commit = $Commit + '-dirty'
}

if ($Targets -contains 'Steam' -and ([string]::IsNullOrEmpty($SteamAppId) -or [string]::IsNullOrEmpty($SteamDepotId)))
{
    Stop-Build 'GNHMP004' 'The Steam target needs -SteamAppId and -SteamDepotId.'
}
if (-not [string]::IsNullOrEmpty($NotaryProfile) -and $SigningIdentity -eq '-')
{
    Stop-Build 'GNHMP004' 'Notarization needs a Developer ID identity in -SigningIdentity, not ad-hoc signing.'
}

if ([string]::IsNullOrEmpty($SshHost))
{
    $SshHost = $env:GNOLLHACK_MAC_SSH_HOST
}
if ([string]::IsNullOrEmpty($SshHost))
{
    $SshHost = 'gnollhack-mac'
}
if ($Keychain -notmatch '^[A-Za-z0-9._/~-]*$' -or $KeychainPasswordFile -notmatch '^[A-Za-z0-9._/~-]*$')
{
    Stop-Build 'GNHMP005' 'The keychain or its password file path contains characters other than letters, digits, ., _, -, / and ~.'
}
if ($SshHost -notmatch '^[A-Za-z0-9._-]+$' -or $RemoteRoot -notmatch '^[A-Za-z0-9._/-]+$')
{
    Stop-Build 'GNHMP005' 'The SSH host alias or the remote folder contains characters other than letters, digits, ., _, - and /.'
}

if ([string]::IsNullOrEmpty($OutputRoot))
{
    $OutputRoot = Join-Path $GnollHackMDir 'bin\MacPackages'
}
$OutputRoot = $ExecutionContext.SessionState.Path.GetUnresolvedProviderPathFromPSPath($OutputRoot)
$StageDir = Join-Path $OutputRoot 'stage'
$StagePublish = Join-Path $StageDir 'publish'
$StagePackaging = Join-Path $StageDir 'packaging'
$TarFile = Join-Path $OutputRoot 'stage.tar'
$VersionName = '{0}-{1}' -f $Version, $Build
$LocalResults = Join-Path $OutputRoot $VersionName

$Ssh = Find-Tool 'ssh' 'OpenSSH\ssh.exe'
$Scp = Find-Tool 'scp' 'OpenSSH\scp.exe'
$Tar = Find-Tool 'tar' 'tar.exe'
$SshOptions = @('-o', 'BatchMode=yes', '-o', 'ConnectTimeout=15')

Write-Host ('GnollHack {0} ({1}), {2}, targets {3}, identity {4}, notary profile {5}' -f $Version, $Build, $BundleId, ($Targets -join ','), $SigningIdentity, $(if ($NotaryProfile) { $NotaryProfile } else { 'none' }))

# ---------------------------------------------------------------------------------------------------
# Publish
# ---------------------------------------------------------------------------------------------------

if (-not $SkipPublish)
{
    if (Test-Path $StagePublish)
    {
        Remove-Item -Recurse -Force $StagePublish
    }
    $publishArgs = @('publish', '..\..\GnollHackM.csproj', '-p:GnhAvalonia=true', '-r', 'osx-arm64', '-f', 'net11.0', '-c', 'Release',
        '--self-contained', 'true', '-p:PublishSingleFile=true', '-p:IncludeNativeLibrariesForSelfExtract=false',
        '-p:IncludeAllContentForSelfExtract=false', '-o', $StagePublish, '-nologo')
    if ($NoSentryUpload)
    {
        $publishArgs += '-p:GnhSentryUpload=false'
    }
    Write-Host 'Publishing osx-arm64 as a single file'
    Push-Location $AvaloniaDir
    try
    {
        $publishExit = Invoke-Native 'dotnet' $publishArgs
        if ($publishExit -ne 0)
        {
            Stop-Build 'GNHMP006' ('dotnet publish failed with exit code {0}.' -f $publishExit)
        }
    }
    finally
    {
        Pop-Location
    }
}
elseif (-not (Test-Path (Join-Path $StagePublish 'GnollHackM')))
{
    Stop-Build 'GNHMP006' ('-SkipPublish was given, but {0} has no publish.' -f $StagePublish)
}

# ---------------------------------------------------------------------------------------------------
# Stage the packaging files and the runner
# ---------------------------------------------------------------------------------------------------

if (Test-Path $StagePackaging)
{
    Remove-Item -Recurse -Force $StagePackaging
}
New-Item -ItemType Directory -Force (Join-Path $StagePackaging 'steam') | Out-Null
Copy-AsLf (Join-Path $PackagingDir 'package-mac.sh') (Join-Path $StagePackaging 'package-mac.sh')
Copy-AsLf (Join-Path $PackagingDir 'Info.plist.template') (Join-Path $StagePackaging 'Info.plist.template')
Copy-AsLf (Join-Path $PackagingDir 'GnollHack.entitlements') (Join-Path $StagePackaging 'GnollHack.entitlements')
Copy-AsLf (Join-Path $PackagingDir 'steam\app_build_mac.vdf.template') (Join-Path $StagePackaging 'steam\app_build_mac.vdf.template')
Copy-AsLf (Join-Path $PackagingDir 'steam\depot_build_mac.vdf.template') (Join-Path $StagePackaging 'steam\depot_build_mac.vdf.template')
Copy-Item $AppIconMaster (Join-Path $StagePackaging 'appicon.png')

$RemoteInput = '$HOME/' + $RemoteRoot + '/package-in'
$RemoteOutput = '$HOME/' + $RemoteRoot + '/package/' + $VersionName
$runnerArgs = @(
    '--input', ('"' + $RemoteInput + '"'),
    '--output', ('"' + $RemoteOutput + '"'),
    '--version', (ConvertTo-ShellWord $Version),
    '--build', (ConvertTo-ShellWord $Build),
    '--bundle-id', (ConvertTo-ShellWord $BundleId),
    '--commit', (ConvertTo-ShellWord $Commit),
    '--identity', (ConvertTo-ShellWord $SigningIdentity),
    '--targets', (ConvertTo-ShellWord (($Targets | ForEach-Object { $_.ToLowerInvariant() }) -join ','))
)
if ($NotaryProfile)
{
    $runnerArgs += @('--notary-profile', (ConvertTo-ShellWord $NotaryProfile))
}
if ($Targets -contains 'Steam')
{
    $runnerArgs += @('--steam-app-id', (ConvertTo-ShellWord $SteamAppId), '--steam-depot-id', (ConvertTo-ShellWord $SteamDepotId),
        '--steam-branch', (ConvertTo-ShellWord $SteamBranch), '--steam-desc', (ConvertTo-ShellWord $BuildDescription))
}
if ($DevCopy)
{
    $runnerArgs += '--dev-copy'
}
if ($SigningIdentity -ne '-' -and -not [string]::IsNullOrEmpty($Keychain))
{
    # ~ is expanded by the shell on the Mac
    $runnerArgs += @('--keychain', ('"' + ($Keychain -replace '^~', '$HOME') + '"'),
        '--keychain-password-file', ('"' + ($KeychainPasswordFile -replace '^~', '$HOME') + '"'))
}
$runner = "#!/bin/bash`nset -euo pipefail`nbash `"$RemoteInput/packaging/package-mac.sh`" " + ($runnerArgs -join ' ') + "`n"
[IO.File]::WriteAllText((Join-Path $StagePackaging 'run.sh'), $runner, (New-Object Text.UTF8Encoding $false))

if (Test-Path $TarFile)
{
    Remove-Item -Force $TarFile
}
Invoke-Tool 'GNHMP007' 'Packing the stage' $Tar @('-cf', $TarFile, '-C', $StageDir, 'publish', 'packaging')

# ---------------------------------------------------------------------------------------------------
# On the Mac
# ---------------------------------------------------------------------------------------------------

Write-Host ('Copying to {0}' -f $SshHost)
if ((Invoke-Native $Ssh (@($SshOptions) + @($SshHost, 'true'))) -ne 0)
{
    Stop-Build 'GNHMP008' ('Cannot connect to the Mac {0}: it is asleep or unreachable, Remote Login is off, or the key is not set up.' -f $SshHost)
}
# No quotes in remote commands: Windows PowerShell mangles them on the way to ssh, and the remote paths are checked
# to hold no spaces
Invoke-Tool 'GNHMP009' 'Preparing the remote folder' $Ssh (@($SshOptions) + @($SshHost, ('rm -rf {0} && mkdir -p {0}' -f $RemoteInput)))
Invoke-Tool 'GNHMP009' 'Copying the stage' $Scp (@($SshOptions) + @($TarFile, ('{0}:{1}/package-in/stage.tar' -f $SshHost, $RemoteRoot)))
Invoke-Tool 'GNHMP009' 'Unpacking the stage' $Ssh (@($SshOptions) + @($SshHost, ('cd {0} && tar -xf stage.tar && rm stage.tar' -f $RemoteInput)))

Write-Host 'Packaging on the Mac'
Invoke-Tool 'GNHMP010' 'package-mac.sh' $Ssh (@($SshOptions) + @($SshHost, ('bash {0}/packaging/run.sh' -f $RemoteInput)))

# ---------------------------------------------------------------------------------------------------
# Results
# ---------------------------------------------------------------------------------------------------

if (Test-Path $LocalResults)
{
    Remove-Item -Recurse -Force $LocalResults
}
New-Item -ItemType Directory -Force $LocalResults | Out-Null
$remoteFiles = @('BUILD-INFO')
if ($Targets -contains 'App' -or $Targets -contains 'Dmg')
{
    $remoteFiles += 'SHA256SUMS'
}
if ($Targets -contains 'App')
{
    $remoteFiles += ('GnollHack-{0}-macos-arm64.zip' -f $Version)
}
if ($Targets -contains 'Dmg')
{
    $remoteFiles += ('GnollHack-{0}-macos-arm64.dmg' -f $Version)
}
foreach ($file in $remoteFiles)
{
    Invoke-Tool 'GNHMP011' ('Copying {0} back' -f $file) $Scp (@($SshOptions) + @(('{0}:{1}/package/{2}/{3}' -f $SshHost, $RemoteRoot, $VersionName, $file), $LocalResults))
}
$LocalSteam = Join-Path $LocalResults 'steam'
if ($Targets -contains 'Steam')
{
    foreach ($file in @('steam.tar', 'steam-content.sha256'))
    {
        Invoke-Tool 'GNHMP011' ('Copying {0} back' -f $file) $Scp (@($SshOptions) + @(('{0}:{1}/package/{2}/{3}' -f $SshHost, $RemoteRoot, $VersionName, $file), $LocalResults))
    }
    New-Item -ItemType Directory -Force $LocalSteam | Out-Null
    $steamTar = Join-Path $LocalResults 'steam.tar'
    $steamHashes = Join-Path $LocalResults 'steam-content.sha256'
    Invoke-Tool 'GNHMP011' 'Unpacking the Steam content' $Tar @('-xf', $steamTar, '-C', $LocalSteam)
    $mismatch = Find-SteamContentMismatch $LocalSteam $steamHashes
    if ($mismatch)
    {
        Stop-Build 'GNHMP012' ('The Steam content copied to Windows does not match the Mac: {0}. Run the build again.' -f $mismatch)
    }
    Move-Item -Force $steamHashes (Join-Path $LocalSteam 'steam-content.sha256')
    Remove-Item -Force $steamTar
    Invoke-Tool 'GNHMP011' 'Removing steam.tar on the Mac' $Ssh (@($SshOptions) + @($SshHost, ('rm -f {0}/package/{1}/steam.tar' -f $RemoteRoot, $VersionName)))
}

Write-Host ''
Write-Host ('Results: {0}' -f $LocalResults)
Write-Host ('On the Mac: ~/{0}/package/{1}' -f $RemoteRoot, $VersionName)
if ($DevCopy)
{
    Write-Host 'Test copy: ~/gnollhack-dev/GnollHack.app'
}
if ($Targets -contains 'Steam')
{
    Write-Host ('Steam content: {0}, checked against the Mac, and ~/{1}/package/{2}/steam on the Mac.' -f $LocalSteam, $RemoteRoot, $VersionName)
    Write-Host 'A person uploads it with ContentBuilder or steamcmd +run_app_build app_build_mac.vdf from either.'
}
