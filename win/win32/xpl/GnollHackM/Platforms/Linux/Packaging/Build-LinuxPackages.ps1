<#
.SYNOPSIS
    Builds the Linux packages of GnollHack: an AppImage, a tar.gz and the Steam depot content.

.DESCRIPTION
    Publishes the Avalonia build for linux-x64, then assembles the packages in WSL, in a folder under
    the WSL home ($HOME/.cache/gnollhack-packaging), because Linux tools on /mnt/c paths are slow and
    lose file permissions. Finished outputs are copied to <OutputRoot>\<version>\, where the version
    is <ApplicationDisplayVersion>.<ApplicationVersion> from GnollHackM.csproj.

    The staged publish is checked against the glibc floor in $MaxGlibcVersion and the libstdc++ floors in
    $MaxGlibcxxVersion and $MaxCxxabiVersion, and the script stops if any ELF file needs a newer glibc or libstdc++.

    The script never downloads, uploads, publishes or signs in to anything. The pinned appimagetool
    and AppImage runtime must already be in $HOME/.cache/gnollhack-packaging/tools/ in WSL.

.PARAMETER Targets
    The packages to build: AppImage, TarGz, Steam. Default: all three.

.PARAMETER SteamRuntime
    The Steam Linux runtime the Steam content is built for: steamrt4 (default) or sniper. With steamrt4,
    every file of Platforms\Linux\libs-steamrt4\ is added to the Steam content root: libcef.so's two libraries
    that steamrt4 lacks, and their copyright file under licenses\.

.PARAMETER SteamAppId
    The Steam app ID for app_build_linux.vdf. When empty, the {APP_ID} placeholder stays in the file.

.PARAMETER SteamDepotId
    The Linux depot ID for both .vdf files. When empty, the {DEPOT_ID} placeholder stays in the files.

.PARAMETER SteamBranch
    The branch the Steam build is set live on. When empty, the {BRANCH} placeholder stays in the file.

.PARAMETER BuildDescription
    The Steam build description. When empty, the {BUILD_DESC} placeholder stays in the file.

.PARAMETER RunbookPath
    Optional Markdown file added to the tar.gz as gnollhack/README-LINUX-TEST.md.

.PARAMETER SkipPublish
    Uses the existing publish folder instead of running dotnet publish.

.PARAMETER KeepDebugInfo
    Ships libgnollhacklinux.so with its DWARF debug information. By default the staged copy is stripped with
    strip --strip-debug, which keeps its build ID and symbol table; the copy in Platforms\Linux\libs\, which the
    Sentry symbol upload reads, keeps its debug information either way.

.PARAMETER OutputRoot
    The folder that receives a <version> subfolder with the outputs.
    Default: <GnollHackM>\bin\LinuxPackages.

.PARAMETER WslDistro
    The WSL distribution that runs the Linux steps. Default: Ubuntu-24.04.

.EXAMPLE
    .\Build-LinuxPackages.ps1

    Publishes and builds the AppImage, the tar.gz and the Steam content for steamrt4.

.EXAMPLE
    .\Build-LinuxPackages.ps1 -Targets TarGz -SkipPublish -RunbookPath C:\path\to\runbook.md

    Builds only the tar.gz from the existing publish, with a test runbook inside it.

.EXAMPLE
    .\Build-LinuxPackages.ps1 -Targets Steam -SteamRuntime steamrt4 -SteamAppId $appId -SteamDepotId $depotId -SteamBranch linux-beta -BuildDescription 'Linux beta'

    Builds the Steam content for steamrt4, with filled-in SteamPipe build scripts.
#>
[CmdletBinding()]
param(
    [ValidateSet('AppImage', 'TarGz', 'Steam')]
    [string[]]$Targets = @('AppImage', 'TarGz', 'Steam'),

    [ValidateSet('sniper', 'steamrt4')]
    [string]$SteamRuntime = 'steamrt4',

    [string]$SteamAppId = '',

    [string]$SteamDepotId = '',

    [string]$SteamBranch = '',

    [string]$BuildDescription = '',

    [string]$RunbookPath = '',

    [switch]$SkipPublish,

    [switch]$KeepDebugInfo,

    [string]$OutputRoot = '',

    [string]$WslDistro = 'Ubuntu-24.04'
)

Set-StrictMode -Version 2
$ErrorActionPreference = 'Stop'

# Pinned AppImage tools, which must be present in WSL under $HOME/.cache/gnollhack-packaging/tools/:
# appimagetool 1.9.1 (github.com/AppImage/appimagetool, release 1.9.1, appimagetool-x86_64.AppImage) and the type-2
# runtime 20251108 (github.com/AppImage/type2-runtime, release 20251108, runtime-x86_64), saved under the names below.
# The hashes match the digests GitHub publishes for those release files.
$AppImageToolFile = 'appimagetool-1.9.1-x86_64.AppImage'
$AppImageToolSha256 = 'ed4ce84f0d9caff66f50bcca6ff6f35aae54ce8135408b3fa33abfc3cb384eb0'
$AppImageRuntimeFile = 'type2-runtime-20251108-x86_64'
$AppImageRuntimeSha256 = '2fca8b443c92510f1483a883f60061ad09b46b978b2631c807cd873a47ec260d'

# The oldest glibc the Linux packages support; every ELF file in the publish must load on it
$MaxGlibcVersion = '2.27'
# The system libstdc++ of the oldest distributions that glibc floor admits (GCC 8's, as in Debian 10, Ubuntu 18.04
# and RHEL 8); libstdc++ is not bundled
$MaxGlibcxxVersion = '3.4.25'
$MaxCxxabiVersion = '1.3.11'

# ---------------------------------------------------------------------------------------------------
# Helper functions
# ---------------------------------------------------------------------------------------------------

function Write-Step
{
    param([string]$Message)
    Write-Host ''
    Write-Host "==> $Message" -ForegroundColor Cyan
}

# Maps an absolute Windows path on a drive letter to its WSL path: C:\x\y becomes /mnt/c/x/y.
function ConvertTo-WslPath
{
    param([Parameter(Mandatory = $true)][string]$WindowsPath)
    $full = [System.IO.Path]::GetFullPath($WindowsPath).TrimEnd('\')
    $isDrivePath = $full -match '^([A-Za-z]):(\\.*)?$'
    if (-not $isDrivePath)
    {
        throw "Cannot map '$full' to a WSL path: only paths on a drive letter are supported."
    }
    $drive = $Matches[1].ToLowerInvariant()
    $rest = ''
    if ($Matches.ContainsKey(2))
    {
        $rest = $Matches[2].Replace('\', '/')
    }
    return "/mnt/$drive$rest"
}

# Quotes a value as a single-quoted bash word.
function ConvertTo-BashLiteral
{
    param([AllowEmptyString()][string]$Value)
    return "'" + $Value.Replace("'", "'\''") + "'"
}

# Escapes a value for a double-quoted VDF string.
function ConvertTo-VdfString
{
    param([AllowEmptyString()][string]$Value)
    return $Value.Replace('\', '\\').Replace('"', '\"')
}

# Writes text as UTF-8 without a BOM, with LF line endings.
function Write-TextFileLf
{
    param([string]$Path, [AllowEmptyString()][string]$Text)
    $lf = $Text.Replace("`r`n", "`n").Replace("`r", "`n")
    [System.IO.File]::WriteAllText($Path, $lf, (New-Object System.Text.UTF8Encoding($false)))
}

# Returns the text of the only element with the given name in an MSBuild project.
function Get-ProjectProperty
{
    param([System.Xml.XmlDocument]$Document, [string]$Name)
    $nodes = @($Document.SelectNodes("//*[local-name()='$Name']"))
    if ($nodes.Count -ne 1)
    {
        throw "Expected exactly one <$Name> in the project, found $($nodes.Count)."
    }
    return $nodes[0].InnerText.Trim()
}

# Fills the placeholders of a VDF template. An empty value leaves its placeholder in place.
function Expand-VdfTemplate
{
    param([string]$TemplatePath, [System.Collections.Specialized.OrderedDictionary]$Values)
    $text = [System.IO.File]::ReadAllText($TemplatePath, [System.Text.Encoding]::UTF8)
    foreach ($key in $Values.Keys)
    {
        $value = [string]$Values[$key]
        if (-not [string]::IsNullOrEmpty($value))
        {
            $text = $text.Replace('{' + $key + '}', (ConvertTo-VdfString $value))
        }
    }
    return $text
}

# Runs a bash script in WSL: the variables as bash assignments, the common definitions, then the body.
# The script goes to %TEMP% with LF endings and no BOM, and is deleted afterwards.
function Invoke-WslScript
{
    param(
        [string]$StepName,
        [string]$Body,
        [System.Collections.Specialized.OrderedDictionary]$Variables,
        [string]$Common,
        [string]$Distro
    )
    $builder = New-Object System.Text.StringBuilder
    [void]$builder.Append("#!/bin/bash`nset -euo pipefail`n")
    foreach ($name in $Variables.Keys)
    {
        [void]$builder.Append("$name=$(ConvertTo-BashLiteral ([string]$Variables[$name]))`n")
    }
    [void]$builder.Append($Common)
    [void]$builder.Append("`n")
    [void]$builder.Append($Body)
    [void]$builder.Append("`n")

    $fileName = 'gnollhack-packaging-' + [guid]::NewGuid().ToString('N') + '.sh'
    $scriptPath = Join-Path ([System.IO.Path]::GetTempPath()) $fileName
    try
    {
        Write-TextFileLf -Path $scriptPath -Text $builder.ToString()
        $wslScriptPath = ConvertTo-WslPath $scriptPath
        # Tools such as appimagetool write progress to stderr; Windows PowerShell 5.1 would turn each such line into a
        # terminating error under 'Stop', so the lines are shown as text and only the exit code decides success
        $previousPreference = $ErrorActionPreference
        $ErrorActionPreference = 'Continue'
        try
        {
            & wsl.exe -d $Distro -e bash $wslScriptPath 2>&1 | ForEach-Object {
                if ($_ -is [System.Management.Automation.ErrorRecord]) { $_.Exception.Message } else { "$_" }
            } | Out-Host
            $exitCode = $LASTEXITCODE
        }
        finally
        {
            $ErrorActionPreference = $previousPreference
        }
        if ($exitCode -ne 0)
        {
            throw "WSL step '$StepName' failed with exit code $exitCode."
        }
    }
    finally
    {
        if (Test-Path -LiteralPath $scriptPath)
        {
            Remove-Item -LiteralPath $scriptPath -Force
        }
    }
}

function Format-ByteSize
{
    param([long]$Bytes)
    return ('{0:N1} MB ({1} bytes)' -f ($Bytes / 1MB), $Bytes)
}

function Get-FolderSize
{
    param([string]$Path)
    $total = [long]0
    foreach ($file in @(Get-ChildItem -LiteralPath $Path -Recurse -File -Force))
    {
        $total += $file.Length
    }
    return $total
}

# ---------------------------------------------------------------------------------------------------
# Bash steps. Each runs after the variables from Invoke-WslScript and $BashCommon.
# ---------------------------------------------------------------------------------------------------

$BashCommon = @'
WORK="$HOME/.cache/gnollhack-packaging"
STAGE="$WORK/stage-$VERSION"
STAGED="$STAGE/gnollhack"
OUT="$STAGE/out"
TOOLS="$WORK/tools"
fail() { echo "error: $*" >&2; exit 1; }
warn() { echo "warning: $*" >&2; }
'@

# Staging: a clean copy of the publish with Linux permissions, and an empty output folder.
$BashStage = @'
for tool in rsync tar gzip sha256sum; do
    command -v "$tool" >/dev/null 2>&1 || fail "'$tool' is not installed in WSL."
done
[ -f "$PUBLISH_DIR/GnollHackM" ] || fail "GnollHackM is missing from the publish folder $PUBLISH_DIR."
mkdir -p "$STAGE"
echo "Staging $PUBLISH_DIR in $STAGED"
rsync -a --delete --chmod=D755,F644 "$PUBLISH_DIR/" "$STAGED/"
chmod 755 "$STAGED/GnollHackM"
if [ -f "$STAGED/createdump" ]; then
    chmod 755 "$STAGED/createdump"
else
    warn "createdump is missing from the publish."
fi
[ -f "$STAGED/libsentry.so" ] || warn "libsentry.so is missing from the publish; native crashes will not be reported."
for license in licenses/cef/LICENSE.txt licenses/cef/CREDITS.html licenses/cefglue/LICENSE.txt \
    licenses/sentry-native/LICENSE licenses/sentry-native/LICENSE-libunwind; do
    [ -f "$STAGED/$license" ] || warn "$license is missing from the publish."
done
# The engine ships without its DWARF; Sentry matches its crashes by the build ID to the full copy it was given
ENGINE="$STAGED/libgnollhacklinux.so"
if [ "$STRIP_DEBUG" = "1" ]; then
    for tool in strip readelf awk; do
        command -v "$tool" >/dev/null 2>&1 || fail "'$tool' is not installed in WSL."
    done
    [ -f "$ENGINE" ] || fail "libgnollhacklinux.so is missing from the publish folder $PUBLISH_DIR."
    build_id() { readelf -n "$1" 2>/dev/null | sed -n '/Build ID:/{s/.*Build ID: \([0-9a-f]*\).*/\1/p;q;}' || true; }
    megabytes() { awk -v b="$1" 'BEGIN { printf "%.1f MB", b / 1048576 }'; }
    id_before=$(build_id "$ENGINE")
    [ -n "$id_before" ] || fail "cannot read the build ID of $ENGINE."
    size_before=$(stat -c %s "$ENGINE")
    strip --strip-debug "$ENGINE"
    id_after=$(build_id "$ENGINE")
    [ "$id_after" = "$id_before" ] || fail "stripping changed the build ID of libgnollhacklinux.so from $id_before to '$id_after'."
    size_after=$(stat -c %s "$ENGINE")
    echo "Stripped libgnollhacklinux.so: $(megabytes "$size_before") -> $(megabytes "$size_after"), build ID $id_after"
else
    echo "Keeping the debug information of libgnollhacklinux.so (-KeepDebugInfo)"
fi
# Every ELF file must load on the oldest supported glibc and libstdc++
for tool in objdump od find; do
    command -v "$tool" >/dev/null 2>&1 || fail "'$tool' is not installed in WSL."
done
too_new=0
while IFS= read -r -d '' file; do
    [ "$(od -An -tx1 -N4 "$file" | tr -d ' \n')" = "7f454c46" ] || continue
    tags=$(objdump -T "$file" 2>/dev/null | grep -oE '(GLIBC|GLIBCXX|CXXABI)_[0-9][0-9.]*' || true)
    [ -n "$tags" ] || continue
    for floor in "GLIBC:$MAX_GLIBC" "GLIBCXX:$MAX_GLIBCXX" "CXXABI:$MAX_CXXABI"; do
        prefix=${floor%%:*}
        max=${floor#*:}
        need=$(printf '%s\n' "$tags" | sed -n "s/^${prefix}_//p" | sort -u -V | tail -n 1)
        [ -n "$need" ] || continue
        newest=$(printf '%s\n%s\n' "$need" "$max" | sort -V | tail -n 1)
        if [ "$newest" != "$max" ]; then
            echo "error: ${file#"$STAGED"/} needs ${prefix}_$need, above the ${prefix}_$max floor." >&2
            too_new=1
        fi
    done
done < <(find "$STAGED" -type f -print0)
[ "$too_new" = "0" ] || fail "the publish needs a newer glibc or libstdc++ than the floor; see the files above."
echo "Every ELF file needs at most GLIBC_$MAX_GLIBC, GLIBCXX_$MAX_GLIBCXX and CXXABI_$MAX_CXXABI"
rm -rf "$OUT"
mkdir -p "$OUT"
'@

# AppImage: the AppDir, then appimagetool with the pinned runtime and GitHub update information.
$BashAppImage = @'
APPIMAGETOOL="$TOOLS/$APPIMAGETOOL_FILE"
RUNTIME="$TOOLS/$RUNTIME_FILE"
check_pinned_file() {
    local path="$1"
    local expected="$2"
    local actual
    [ -f "$path" ] || fail "$path is missing. Put the pinned file there; its name and SHA-256 are set at the top of Build-LinuxPackages.ps1."
    actual=$(sha256sum "$path" | cut -d ' ' -f 1)
    [ "$actual" = "$expected" ] || fail "$path has SHA-256 $actual, expected $expected. Replace it with the pinned file."
}
check_pinned_file "$APPIMAGETOOL" "$APPIMAGETOOL_SHA256"
check_pinned_file "$RUNTIME" "$RUNTIME_SHA256"
[ -x "$APPIMAGETOOL" ] || chmod 755 "$APPIMAGETOOL"

APPDIR_ROOT="$STAGE/AppDir"
echo "Building the AppDir in $APPDIR_ROOT"
rm -rf "$APPDIR_ROOT"
mkdir -p "$APPDIR_ROOT"
chmod 755 "$APPDIR_ROOT"
cp -a "$STAGED" "$APPDIR_ROOT/gnollhack"
tr -d '\r' < "$PACKAGING_DIR/AppRun" > "$APPDIR_ROOT/AppRun"
chmod 755 "$APPDIR_ROOT/AppRun"
tr -d '\r' < "$PACKAGING_DIR/gnollhack.desktop" > "$APPDIR_ROOT/gnollhack.desktop"
chmod 644 "$APPDIR_ROOT/gnollhack.desktop"
[ -f "$STAGED/gnollhack.png" ] || fail "gnollhack.png is missing from the publish."
cp "$STAGED/gnollhack.png" "$APPDIR_ROOT/gnollhack.png"
chmod 644 "$APPDIR_ROOT/gnollhack.png"
ln -s gnollhack.png "$APPDIR_ROOT/.DirIcon"

cd "$OUT"
rm -f "$APPIMAGE_NAME" "$APPIMAGE_NAME.zsync"
echo "Building $APPIMAGE_NAME"
ARCH=x86_64 "$APPIMAGETOOL" --appimage-extract-and-run --runtime-file "$RUNTIME" \
    -u "gh-releases-zsync|hyvanmielenpelit|GnollHack|latest|GnollHack-*-x86_64.AppImage.zsync" \
    "$APPDIR_ROOT" "$APPIMAGE_NAME"
[ -f "$APPIMAGE_NAME" ] || fail "appimagetool did not create $APPIMAGE_NAME."
chmod 755 "$APPIMAGE_NAME"
if [ ! -f "$APPIMAGE_NAME.zsync" ]; then
    if command -v zsyncmake >/dev/null 2>&1; then
        echo "Writing $APPIMAGE_NAME.zsync with zsyncmake"
        zsyncmake -o "$APPIMAGE_NAME.zsync" "$APPIMAGE_NAME"
    else
        warn "appimagetool wrote no $APPIMAGE_NAME.zsync, and zsyncmake is not installed (sudo apt install zsync)."
    fi
fi
'@

# tar.gz: the staged copy as gnollhack/, with the optional runbook inside it.
$BashTarGz = @'
TAR_ROOT="$STAGE/tar"
echo "Building $TARGZ_NAME"
rm -rf "$TAR_ROOT"
mkdir -p "$TAR_ROOT"
cp -a "$STAGED" "$TAR_ROOT/gnollhack"
if [ -n "$RUNBOOK" ]; then
    tr -d '\r' < "$RUNBOOK" > "$TAR_ROOT/gnollhack/README-LINUX-TEST.md"
    chmod 644 "$TAR_ROOT/gnollhack/README-LINUX-TEST.md"
fi
rm -f "$OUT/$TARGZ_NAME"
tar -C "$TAR_ROOT" --owner=0 --group=0 --numeric-owner -czf "$OUT/$TARGZ_NAME" gnollhack
'@

# Steam: the staged copy as content/, the steamrt4 libraries when chosen, and the filled-in scripts.
$BashSteam = @'
STEAM_DIR="$OUT/steam"
echo "Assembling the Steam content in $STEAM_DIR"
rm -rf "$STEAM_DIR"
mkdir -p "$STEAM_DIR"
cp -a "$STAGED" "$STEAM_DIR/content"
if [ -n "$STEAMRT4_DIR" ]; then
    echo "Adding the steamrt4 libraries from $STEAMRT4_DIR"
    rsync -a --chmod=D755,F644 "$STEAMRT4_DIR/" "$STEAM_DIR/content/"
fi
cp "$APP_VDF" "$STEAM_DIR/app_build_linux.vdf"
cp "$DEPOT_VDF" "$STEAM_DIR/depot_build_linux.vdf"
chmod 644 "$STEAM_DIR/app_build_linux.vdf" "$STEAM_DIR/depot_build_linux.vdf"
'@

# Checksums for the AppImage and the tar.gz of this run, then the copy to the Windows output folder.
$BashFinish = @'
cd "$OUT"
sums=()
for name in "$APPIMAGE_NAME" "$TARGZ_NAME"; do
    if [ -f "$name" ]; then
        sums+=("$name")
    fi
done
if [ "${#sums[@]}" -gt 0 ]; then
    sha256sum -- "${sums[@]}" > SHA256SUMS
fi
echo "Copying the outputs to $WIN_OUT"
mkdir -p "$WIN_OUT"
for item in "$OUT"/*; do
    [ -e "$item" ] || continue
    name=$(basename "$item")
    rm -rf "$WIN_OUT/$name"
    cp -r "$item" "$WIN_OUT/"
done
'@

# ---------------------------------------------------------------------------------------------------
# Paths, all derived from the location of this script: Platforms\Linux\Packaging under GnollHackM
# ---------------------------------------------------------------------------------------------------

$PackagingDir = $PSScriptRoot
$LinuxDir = Split-Path -Parent $PackagingDir
$PlatformsDir = Split-Path -Parent $LinuxDir
$GnollHackMDir = Split-Path -Parent $PlatformsDir
# GnollHackM is in win\win32\xpl under the repository root
$RepoRoot = Split-Path -Parent (Split-Path -Parent (Split-Path -Parent (Split-Path -Parent $GnollHackMDir)))

$ProjectPath = Join-Path $GnollHackMDir 'GnollHackM.csproj'
$AvaloniaDir = Join-Path $PlatformsDir 'Avalonia'
$NativeLibraryPath = Join-Path $LinuxDir 'libs\libgnollhacklinux.so'
$NativeProjectPath = Join-Path $RepoRoot 'win\win32\vs\gnollhacklinux.vcxproj'
# WORKAROUND(WA-LINUX-0002): Steam Linux Runtime 4.0 lacks libraries libcef.so needs
$SteamRt4LibsDir = Join-Path $LinuxDir 'libs-steamrt4'
$AppVdfTemplatePath = Join-Path $PackagingDir 'steam\app_build_linux.vdf.template'
$DepotVdfTemplatePath = Join-Path $PackagingDir 'steam\depot_build_linux.vdf.template'
$PublishDir = Join-Path $GnollHackMDir 'bin\Release\net11.0\linux-x64\publish'

if (-not (Test-Path -LiteralPath $ProjectPath -PathType Leaf))
{
    throw "GnollHackM.csproj not found at '$ProjectPath'. The script must stay in GnollHackM\Platforms\Linux\Packaging."
}

if ([string]::IsNullOrEmpty($OutputRoot))
{
    $OutputRoot = Join-Path $GnollHackMDir 'bin\LinuxPackages'
}
$OutputRootFull = $ExecutionContext.SessionState.Path.GetUnresolvedProviderPathFromPSPath($OutputRoot)

$BuildAppImage = $Targets -contains 'AppImage'
$BuildTarGz = $Targets -contains 'TarGz'
$BuildSteam = $Targets -contains 'Steam'

# ---------------------------------------------------------------------------------------------------
# 1. Prerequisites
# ---------------------------------------------------------------------------------------------------

Write-Step 'Checking prerequisites'

if (-not (Test-Path -LiteralPath $NativeLibraryPath -PathType Leaf))
{
    throw "Missing '$NativeLibraryPath'. Build it first with '$NativeProjectPath'."
}

if ($null -eq (Get-Command 'wsl.exe' -ErrorAction SilentlyContinue))
{
    throw 'wsl.exe was not found. The Linux steps run in WSL.'
}
if ((-not $SkipPublish) -and ($null -eq (Get-Command 'dotnet' -ErrorAction SilentlyContinue)))
{
    throw 'dotnet was not found on PATH.'
}

if ($BuildAppImage)
{
    $unset = @()
    if ([string]::IsNullOrWhiteSpace($AppImageToolFile)) { $unset += '$AppImageToolFile' }
    if ([string]::IsNullOrWhiteSpace($AppImageToolSha256)) { $unset += '$AppImageToolSha256' }
    if ([string]::IsNullOrWhiteSpace($AppImageRuntimeFile)) { $unset += '$AppImageRuntimeFile' }
    if ([string]::IsNullOrWhiteSpace($AppImageRuntimeSha256)) { $unset += '$AppImageRuntimeSha256' }
    if ($unset.Count -gt 0)
    {
        throw "Set $($unset -join ', ') at the top of Build-LinuxPackages.ps1 to the pinned appimagetool and AppImage runtime."
    }
    foreach ($fileName in @($AppImageToolFile, $AppImageRuntimeFile))
    {
        if ($fileName -match '[\\/]')
        {
            throw "'$fileName' must be a file name in `$HOME/.cache/gnollhack-packaging/tools/, not a path."
        }
    }
    foreach ($hash in @($AppImageToolSha256, $AppImageRuntimeSha256))
    {
        if ($hash -notmatch '^[0-9A-Fa-f]{64}$')
        {
            throw "'$hash' is not a SHA-256 value (64 hexadecimal digits)."
        }
    }
    foreach ($asset in @('AppRun', 'gnollhack.desktop'))
    {
        if (-not (Test-Path -LiteralPath (Join-Path $PackagingDir $asset) -PathType Leaf))
        {
            throw "Missing '$(Join-Path $PackagingDir $asset)'."
        }
    }
}

$RunbookFull = ''
if (-not [string]::IsNullOrEmpty($RunbookPath))
{
    $RunbookFull = $ExecutionContext.SessionState.Path.GetUnresolvedProviderPathFromPSPath($RunbookPath)
    if (-not (Test-Path -LiteralPath $RunbookFull -PathType Leaf))
    {
        throw "The runbook '$RunbookFull' does not exist."
    }
    if (-not $BuildTarGz)
    {
        Write-Warning '-RunbookPath is used only by the TarGz target, which is not selected.'
    }
}

if ($BuildSteam)
{
    foreach ($template in @($AppVdfTemplatePath, $DepotVdfTemplatePath))
    {
        if (-not (Test-Path -LiteralPath $template -PathType Leaf))
        {
            throw "Missing '$template'."
        }
    }
    if (($SteamRuntime -eq 'steamrt4') -and (-not (Test-Path -LiteralPath $SteamRt4LibsDir -PathType Container)))
    {
        throw "-SteamRuntime steamrt4 needs the bundled libraries in '$SteamRt4LibsDir', which does not exist."
    }
}

& wsl.exe -d $WslDistro -e true
if ($LASTEXITCODE -ne 0)
{
    throw "The WSL distribution '$WslDistro' cannot run commands (exit code $LASTEXITCODE)."
}

# ---------------------------------------------------------------------------------------------------
# 2. Version
# ---------------------------------------------------------------------------------------------------

$projectXml = New-Object System.Xml.XmlDocument
$projectXml.Load($ProjectPath)
$DisplayVersion = Get-ProjectProperty -Document $projectXml -Name 'ApplicationDisplayVersion'
$BuildNumber = Get-ProjectProperty -Document $projectXml -Name 'ApplicationVersion'
if ($DisplayVersion -notmatch '^\d+(\.\d+)*$')
{
    throw "ApplicationDisplayVersion '$DisplayVersion' is not a dotted number."
}
if ($BuildNumber -notmatch '^\d+$')
{
    throw "ApplicationVersion '$BuildNumber' is not a number."
}
$Version = "$DisplayVersion.$BuildNumber"
$AppImageName = "GnollHack-$Version-x86_64.AppImage"
$TarGzName = "GnollHack-$Version-linux-x64.tar.gz"
$VersionOutputDir = Join-Path $OutputRootFull $Version

Write-Host "Version $Version; targets: $($Targets -join ', ')"

# ---------------------------------------------------------------------------------------------------
# 3. Publish
# ---------------------------------------------------------------------------------------------------

if ($SkipPublish)
{
    Write-Step "Skipping the publish; using '$PublishDir'"
}
else
{
    Write-Step 'Publishing GnollHackM for linux-x64 (Release, self-contained)'
    # Platforms\Avalonia holds the global.json that pins the SDK
    Push-Location -LiteralPath $AvaloniaDir
    try
    {
        & dotnet publish '..\..\GnollHackM.csproj' '-p:GnhAvalonia=true' -r linux-x64 -f net11.0 -c Release --self-contained true | Out-Host
        if ($LASTEXITCODE -ne 0)
        {
            throw "dotnet publish failed with exit code $LASTEXITCODE."
        }
    }
    finally
    {
        Pop-Location
    }
}

if (-not (Test-Path -LiteralPath (Join-Path $PublishDir 'GnollHackM') -PathType Leaf))
{
    throw "No publish found: '$(Join-Path $PublishDir 'GnollHackM')' is missing."
}

# ---------------------------------------------------------------------------------------------------
# 4-9. Linux steps in WSL
# ---------------------------------------------------------------------------------------------------

New-Item -ItemType Directory -Force -Path $VersionOutputDir | Out-Null

$AppVdfTempPath = ''
$DepotVdfTempPath = ''
$PlaceholdersLeft = @()

$wslVariables = [ordered]@{
    VERSION             = $Version
    APPIMAGE_NAME       = $AppImageName
    TARGZ_NAME          = $TarGzName
    PUBLISH_DIR         = (ConvertTo-WslPath $PublishDir)
    PACKAGING_DIR       = (ConvertTo-WslPath $PackagingDir)
    WIN_OUT             = (ConvertTo-WslPath $VersionOutputDir)
    APPIMAGETOOL_FILE   = $AppImageToolFile
    APPIMAGETOOL_SHA256 = $AppImageToolSha256.ToLowerInvariant()
    RUNTIME_FILE        = $AppImageRuntimeFile
    RUNTIME_SHA256      = $AppImageRuntimeSha256.ToLowerInvariant()
    RUNBOOK             = ''
    STRIP_DEBUG         = $(if ($KeepDebugInfo) { '0' } else { '1' })
    MAX_GLIBC           = $MaxGlibcVersion
    MAX_GLIBCXX         = $MaxGlibcxxVersion
    MAX_CXXABI          = $MaxCxxabiVersion
    STEAMRT4_DIR        = ''
    APP_VDF             = ''
    DEPOT_VDF           = ''
}
if ($RunbookFull -ne '')
{
    $wslVariables['RUNBOOK'] = ConvertTo-WslPath $RunbookFull
}

try
{
    if ($BuildSteam)
    {
        $vdfValues = [ordered]@{
            APP_ID       = $SteamAppId
            DEPOT_ID     = $SteamDepotId
            BUILD_DESC   = $BuildDescription
            BRANCH       = $SteamBranch
            CONTENT_ROOT = 'content'
        }
        $tempDir = [System.IO.Path]::GetTempPath()
        $AppVdfTempPath = Join-Path $tempDir ('gnollhack-app-' + [guid]::NewGuid().ToString('N') + '.vdf')
        $DepotVdfTempPath = Join-Path $tempDir ('gnollhack-depot-' + [guid]::NewGuid().ToString('N') + '.vdf')
        $appVdf = Expand-VdfTemplate -TemplatePath $AppVdfTemplatePath -Values $vdfValues
        $depotVdf = Expand-VdfTemplate -TemplatePath $DepotVdfTemplatePath -Values $vdfValues
        Write-TextFileLf -Path $AppVdfTempPath -Text $appVdf
        Write-TextFileLf -Path $DepotVdfTempPath -Text $depotVdf
        foreach ($match in [regex]::Matches($appVdf + $depotVdf, '\{(APP_ID|DEPOT_ID|BUILD_DESC|BRANCH)\}'))
        {
            if ($PlaceholdersLeft -notcontains $match.Value)
            {
                $PlaceholdersLeft += $match.Value
            }
        }
        $wslVariables['APP_VDF'] = ConvertTo-WslPath $AppVdfTempPath
        $wslVariables['DEPOT_VDF'] = ConvertTo-WslPath $DepotVdfTempPath
        if ($SteamRuntime -eq 'steamrt4')
        {
            $wslVariables['STEAMRT4_DIR'] = ConvertTo-WslPath $SteamRt4LibsDir
        }
    }

    Write-Step 'Staging the publish in WSL'
    Invoke-WslScript -StepName 'staging' -Body $BashStage -Variables $wslVariables -Common $BashCommon -Distro $WslDistro

    if ($BuildAppImage)
    {
        Write-Step 'Building the AppImage'
        Invoke-WslScript -StepName 'AppImage' -Body $BashAppImage -Variables $wslVariables -Common $BashCommon -Distro $WslDistro
    }

    if ($BuildTarGz)
    {
        Write-Step 'Building the tar.gz'
        Invoke-WslScript -StepName 'TarGz' -Body $BashTarGz -Variables $wslVariables -Common $BashCommon -Distro $WslDistro
    }

    if ($BuildSteam)
    {
        Write-Step "Assembling the Steam content ($SteamRuntime)"
        Invoke-WslScript -StepName 'Steam' -Body $BashSteam -Variables $wslVariables -Common $BashCommon -Distro $WslDistro
    }

    Write-Step 'Writing SHA256SUMS and copying the outputs'
    Invoke-WslScript -StepName 'checksums' -Body $BashFinish -Variables $wslVariables -Common $BashCommon -Distro $WslDistro
}
finally
{
    foreach ($tempPath in @($AppVdfTempPath, $DepotVdfTempPath))
    {
        if (($tempPath -ne '') -and (Test-Path -LiteralPath $tempPath))
        {
            Remove-Item -LiteralPath $tempPath -Force
        }
    }
}

# ---------------------------------------------------------------------------------------------------
# 10. Summary
# ---------------------------------------------------------------------------------------------------

Write-Step "Outputs of $Version"

$outputNames = @()
if ($BuildAppImage)
{
    $outputNames += $AppImageName
    $outputNames += "$AppImageName.zsync"
}
if ($BuildTarGz)
{
    $outputNames += $TarGzName
}
if ($BuildAppImage -or $BuildTarGz)
{
    $outputNames += 'SHA256SUMS'
}

foreach ($name in $outputNames)
{
    $path = Join-Path $VersionOutputDir $name
    if (Test-Path -LiteralPath $path -PathType Leaf)
    {
        $item = Get-Item -LiteralPath $path
        $hash = (Get-FileHash -LiteralPath $path -Algorithm SHA256).Hash.ToLowerInvariant()
        Write-Host $path
        Write-Host "    size    $(Format-ByteSize $item.Length)"
        Write-Host "    sha256  $hash"
    }
    else
    {
        Write-Host "$path    (not created)"
    }
}

if ($BuildSteam)
{
    $steamPath = Join-Path $VersionOutputDir 'steam'
    Write-Host $steamPath
    Write-Host "    size    $(Format-ByteSize (Get-FolderSize (Join-Path $steamPath 'content'))) of content"
    Write-Host "    runtime $SteamRuntime"
    # The Windows copy carries no Linux permissions; the WSL copy keeps the executable bits
    Write-Host "    in WSL  ~/.cache/gnollhack-packaging/stage-$Version/out/steam (with Linux permissions, for steamcmd in WSL)"
    if ($PlaceholdersLeft.Count -gt 0)
    {
        Write-Warning "The Steam build scripts still contain $($PlaceholdersLeft -join ', '). Fill them in before uploading."
    }
}
