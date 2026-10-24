<#
.SYNOPSIS
    Builds libsentry.so, sentry-native's inproc crash handler for the Linux build of GnollHack.

.DESCRIPTION
    Builds a pinned sentry-native checkout in WSL with CMake (inproc backend, no transport, shared library), in
    $HOME/.cache/gnollhack-sentry-native/build, which is recreated on every run. The sentry-native version is read
    from the vendored header win\win32\xpl\gnhsentrywin\include\sentry.h (SENTRY_SDK_VERSION_MAJOR, _MINOR and
    _PATCH), and the checkout must match it twice over: the tag of that version must resolve to the pinned commit,
    and the checkout's include/sentry.h must equal the vendored header, ignoring line endings.

    The library is stripped with strip --strip-unneeded, which keeps its build ID, and checked: no libcurl or
    libstdc++ among its NEEDED entries, no glibc symbol version above GLIBC_2.27, and sentry_init and
    sentry_options_set_handler_strategy exported. Only then is it copied to Platforms\Linux\libs\libsentry.so, with
    sentry-native's license and the license of its vendored libunwind in Platforms\Linux\licenses\sentry-native\.
    Next to the library, Platforms\Linux\libs\libsentry.build.props records the sentry-native version, the pinned
    commit, the normalized hash of the vendored header, the SHA-256 of libsentry.so and its GNU build ID, which
    GnhSentryNative.targets checks at build time.

    The script never downloads anything. The build tools and the pinned checkout must already be in WSL; when they are
    not, the script stops with the command that installs or clones them.

.PARAMETER WslDistro
    The WSL distribution that builds the library. Default: Ubuntu-20.04 (glibc 2.31); the library must still need no glibc
    symbol version above GLIBC_2.27, the floor of the Linux packages.

.PARAMETER SourceDir
    The sentry-native checkout in WSL. A leading $HOME or ~ is expanded inside WSL.
    Default: $HOME/.cache/gnollhack-sentry-native/sentry-native.

.EXAMPLE
    .\Build-LinuxSentryNative.ps1

    Builds libsentry.so in Ubuntu-20.04 from the default checkout.

.EXAMPLE
    .\Build-LinuxSentryNative.ps1 -SourceDir /root/src/sentry-native

    Builds libsentry.so from a checkout in another folder.
#>
[CmdletBinding()]
param(
    [string]$WslDistro = 'Ubuntu-20.04',

    [string]$SourceDir = '$HOME/.cache/gnollhack-sentry-native/sentry-native'
)

Set-StrictMode -Version 2
$ErrorActionPreference = 'Stop'

# Pinned sentry-native commit: the commit of the tag that win\win32\xpl\gnhsentrywin\include\sentry.h names
$SentryNativeCommit = 'd043b2bcbdc24eb9fd3a17d0fe0a510526e5902a'

# The highest glibc symbol version the library may need
$MaxGlibcVersion = '2.27'

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

# Writes text as UTF-8 without a BOM, with LF line endings.
function Write-TextFileLf
{
    param([string]$Path, [AllowEmptyString()][string]$Text)
    $lf = $Text.Replace("`r`n", "`n").Replace("`r", "`n")
    [System.IO.File]::WriteAllText($Path, $lf, (New-Object System.Text.UTF8Encoding($false)))
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

    $fileName = 'gnollhack-sentry-native-' + [guid]::NewGuid().ToString('N') + '.sh'
    $scriptPath = Join-Path ([System.IO.Path]::GetTempPath()) $fileName
    try
    {
        Write-TextFileLf -Path $scriptPath -Text $builder.ToString()
        $wslScriptPath = ConvertTo-WslPath $scriptPath
        # CMake and the compiler write warnings to stderr; Windows PowerShell 5.1 would turn each such line into a
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

# Reads MAJOR.MINOR.PATCH from the SENTRY_SDK_VERSION_MAJOR, _MINOR and _PATCH defines of a sentry.h, each of which
# must occur exactly once.
function Get-SentryNativeVersion
{
    param([Parameter(Mandatory = $true)][string]$HeaderPath)
    $text = [System.IO.File]::ReadAllText($HeaderPath)
    $parts = @()
    foreach ($part in @('MAJOR', 'MINOR', 'PATCH'))
    {
        $found = [regex]::Matches($text, "#define\s+SENTRY_SDK_VERSION_$part\s+(\d+)")
        if ($found.Count -ne 1)
        {
            throw "Expected one '#define SENTRY_SDK_VERSION_$part <number>' in '$HeaderPath', found $($found.Count)."
        }
        $parts += $found[0].Groups[1].Value
    }
    return ($parts -join '.')
}

# Hashes text files independently of their line endings: each file read as UTF-8 (BOM stripped) with CRLF turned
# into LF, the files concatenated in order, encoded as UTF-8 without a BOM, SHA-256 as uppercase hex.
function Get-NormalizedTextHash
{
    param([Parameter(Mandatory = $true)][string[]]$TextFiles)
    $builder = New-Object System.Text.StringBuilder
    foreach ($textFile in $TextFiles)
    {
        [void]$builder.Append([System.IO.File]::ReadAllText($textFile).Replace("`r`n", "`n"))
    }
    $bytes = (New-Object System.Text.UTF8Encoding($false)).GetBytes($builder.ToString())
    $sha256 = [System.Security.Cryptography.SHA256]::Create()
    try
    {
        $digest = $sha256.ComputeHash($bytes)
    }
    finally
    {
        $sha256.Dispose()
    }
    return ([System.BitConverter]::ToString($digest)).Replace('-', '')
}

# Reads the GNU build ID of an ELF file as lowercase hex. Assumes the 20-byte SHA-1 note that Ubuntu's linker emits
# by default: the note header namesz 4, descsz 20, type NT_GNU_BUILD_ID (3), name "GNU\0", must occur exactly once,
# and the 20 bytes after it are the ID. A 16-byte note (--build-id=md5 or uuid) is not found and throws.
function Get-ElfBuildId
{
    param([Parameter(Mandatory = $true)][string]$ElfPath)
    $bytes = [System.IO.File]::ReadAllBytes($ElfPath)
    # ISO-8859-1 maps every byte to the char of the same value, so an ordinal string search is a byte search
    $latin1 = [System.Text.Encoding]::GetEncoding(28591)
    $content = $latin1.GetString($bytes)
    $noteHeader = $latin1.GetString([byte[]](0x04, 0, 0, 0, 0x14, 0, 0, 0, 0x03, 0, 0, 0, 0x47, 0x4E, 0x55, 0))
    $index = $content.IndexOf($noteHeader, [System.StringComparison]::Ordinal)
    if ($index -lt 0)
    {
        throw "'$ElfPath' has no 20-byte GNU build-ID note."
    }
    if ($content.IndexOf($noteHeader, $index + 1, [System.StringComparison]::Ordinal) -ge 0)
    {
        throw "'$ElfPath' has more than one 20-byte GNU build-ID note header."
    }
    $start = $index + $noteHeader.Length
    if ($start + 20 -gt $bytes.Length)
    {
        throw "The GNU build-ID note of '$ElfPath' is truncated."
    }
    $id = New-Object System.Text.StringBuilder
    for ($i = $start; $i -lt $start + 20; $i++)
    {
        [void]$id.Append($bytes[$i].ToString('x2'))
    }
    return $id.ToString()
}

# ---------------------------------------------------------------------------------------------------
# Bash steps. Each runs after the variables from Invoke-WslScript and $BashCommon.
# ---------------------------------------------------------------------------------------------------

$BashCommon = @'
WORK="$HOME/.cache/gnollhack-sentry-native"
BUILD_DIR="$WORK/build"
OUT="$WORK/out"
LIB="$OUT/libsentry.so"
case "$SOURCE_DIR" in
    '$HOME'|'$HOME/'*) SOURCE_DIR="$HOME${SOURCE_DIR#\$HOME}" ;;
    '~'|'~/'*) SOURCE_DIR="$HOME${SOURCE_DIR#\~}" ;;
esac
CLONE_COMMAND="git clone --depth 1 --branch $SENTRY_VERSION https://github.com/getsentry/sentry-native.git $SOURCE_DIR"
fail() { echo "error: $*" >&2; exit 1; }
warn() { echo "warning: $*" >&2; }
build_id() { readelf -n "$1" 2>/dev/null | sed -n '/Build ID:/{s/.*Build ID: \([0-9a-f]*\).*/\1/p;q;}' || true; }
'@

# Tools and source: every missing package in one install command, then the pinned checkout.
$BashCheck = @'
missing=()
for entry in cmake:cmake make:make gcc:gcc g++:g++ git:git strip:binutils readelf:binutils objdump:binutils \
    nm:binutils sha256sum:coreutils sort:coreutils awk:gawk; do
    tool="${entry%%:*}"
    package="${entry#*:}"
    if ! command -v "$tool" >/dev/null 2>&1; then
        echo "'$tool' is not installed." >&2
        case " ${missing[*]:-} " in
            *" $package "*) ;;
            *) missing+=("$package") ;;
        esac
    fi
done
if [ "${#missing[@]}" -gt 0 ]; then
    fail "install the missing tools with: wsl -d $WSL_DISTRO -- sudo apt-get install -y ${missing[*]}"
fi
echo "cmake: $(cmake --version | head -n 1)"
echo "gcc:   $(gcc --version | head -n 1)"

if [ ! -f "$SOURCE_DIR/CMakeLists.txt" ]; then
    if [ -e "$SOURCE_DIR" ]; then
        fail "$SOURCE_DIR is not a sentry-native checkout. Remove it, then run in WSL: $CLONE_COMMAND"
    fi
    fail "the sentry-native source is missing. Run in WSL: $CLONE_COMMAND"
fi
head_commit=$(git -C "$SOURCE_DIR" rev-parse HEAD 2>/dev/null || true)
[ -n "$head_commit" ] || fail "$SOURCE_DIR is not a git checkout. Remove it, then run in WSL: $CLONE_COMMAND"
if [ "$head_commit" != "$SENTRY_COMMIT" ]; then
    fail "HEAD of $SOURCE_DIR is $head_commit, not the pinned $SENTRY_COMMIT ($SENTRY_VERSION). Run in WSL: rm -rf $SOURCE_DIR && $CLONE_COMMAND"
fi
tag_commit=$(git -C "$SOURCE_DIR" rev-parse "refs/tags/$SENTRY_VERSION^{commit}" 2>/dev/null || true)
[ "$tag_commit" = "$SENTRY_COMMIT" ] || fail "tag $SENTRY_VERSION in $SOURCE_DIR is not the pinned $SENTRY_COMMIT."
[ -f "$SOURCE_DIR/include/sentry.h" ] || fail "$SOURCE_DIR/include/sentry.h is missing."
checkout_header_hash=$(sed 's/\r$//' "$SOURCE_DIR/include/sentry.h" | sha256sum)
vendored_header_hash=$(sed 's/\r$//' "$VENDORED_HEADER" | sha256sum)
if [ "$checkout_header_hash" != "$vendored_header_hash" ]; then
    share_header='\\wsl.localhost\'"$WSL_DISTRO$(printf '%s' "$SOURCE_DIR" | tr '/' '\\')"'\include\sentry.h'
    echo "error: $SOURCE_DIR/include/sentry.h differs from the vendored $VENDORED_HEADER." >&2
    echo "Compare the API and update the vendored copy on Windows with:" >&2
    echo "    $COMPARE_API_SCRIPT -NewHeader $share_header" >&2
    exit 1
fi
echo "include/sentry.h matches the vendored header"
for file in LICENSE vendor/libunwind/COPYING; do
    [ -f "$SOURCE_DIR/$file" ] || fail "$SOURCE_DIR/$file is missing."
done
echo "sentry-native $SENTRY_VERSION at $head_commit in $SOURCE_DIR"
'@

# Build: a fresh build folder, so no option survives from an earlier configuration.
$BashBuild = @'
rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"
cmake -S "$SOURCE_DIR" -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=RelWithDebInfo \
    -DSENTRY_BACKEND=inproc -DSENTRY_TRANSPORT=none -DSENTRY_BUILD_SHARED_LIBS=ON \
    -DSENTRY_BUILD_TESTS=OFF -DSENTRY_BUILD_EXAMPLES=OFF -DSENTRY_ENABLE_INSTALL=OFF
cmake --build "$BUILD_DIR" --parallel
[ -f "$BUILD_DIR/libsentry.so" ] || fail "the build did not create $BUILD_DIR/libsentry.so."
'@

# Strip and check: the stripped copy in $OUT must keep the build ID and pass every check before it is copied.
$BashStripAndCheck = @'
megabytes() { awk -v b="$1" 'BEGIN { printf "%.1f MB", b / 1048576 }'; }
rm -rf "$OUT"
mkdir -p "$OUT"
cp -L "$BUILD_DIR/libsentry.so" "$LIB"
id_before=$(build_id "$LIB")
[ -n "$id_before" ] || fail "cannot read the build ID of $LIB."
size_before=$(stat -c %s "$LIB")
strip --strip-unneeded "$LIB"
id_after=$(build_id "$LIB")
[ "$id_after" = "$id_before" ] || fail "stripping changed the build ID of libsentry.so from $id_before to '$id_after'."
size_after=$(stat -c %s "$LIB")
echo "Stripped libsentry.so: $(megabytes "$size_before") -> $(megabytes "$size_after")"

needed=$(readelf -d "$LIB" | sed -n 's/.*(NEEDED).*Shared library: \[\(.*\)\].*/\1/p')
echo "NEEDED:"
printf '    %s\n' $needed
for name in $needed; do
    case "$name" in
        libcurl*|libstdc++*) fail "libsentry.so needs $name." ;;
    esac
done

highest_glibc=$(objdump -T "$LIB" | grep -o 'GLIBC_[0-9][0-9.]*' | sed 's/^GLIBC_//' | sort -u -V | tail -n 1 || true)
[ -n "$highest_glibc" ] || fail "objdump -T shows no GLIBC_ symbol versions in $LIB."
echo "Highest glibc version: GLIBC_$highest_glibc"
newest=$(printf '%s\n%s\n' "$highest_glibc" "$MAX_GLIBC" | sort -V | tail -n 1)
[ "$newest" = "$MAX_GLIBC" ] || fail "libsentry.so needs GLIBC_$highest_glibc, above the GLIBC_$MAX_GLIBC floor."

exported=$(nm -D --defined-only "$LIB" | awk '{ print $NF }')
for symbol in sentry_init sentry_options_set_handler_strategy; do
    grep -qx "$symbol" <<< "$exported" || fail "libsentry.so does not export $symbol."
    echo "Exports $symbol"
done

echo "Build ID: $id_after"
echo "SHA-256:  $(sha256sum "$LIB" | cut -d ' ' -f 1)"
'@

# Copy the checked library and the two licenses to the Windows tree. The licenses get CRLF line endings, as text files
# have in the Windows working tree.
$BashCopy = @'
mkdir -p "$WIN_LIBS_DIR" "$WIN_LICENSE_DIR"
cp "$LIB" "$WIN_LIBS_DIR/libsentry.so"
sed 's/\r$//; s/$/\r/' "$SOURCE_DIR/LICENSE" > "$WIN_LICENSE_DIR/LICENSE"
sed 's/\r$//; s/$/\r/' "$SOURCE_DIR/vendor/libunwind/COPYING" > "$WIN_LICENSE_DIR/LICENSE-libunwind"
echo "Copied libsentry.so to $WIN_LIBS_DIR and the licenses to $WIN_LICENSE_DIR"
'@

# ---------------------------------------------------------------------------------------------------
# Paths, all derived from the location of this script: Platforms\Linux\Packaging under GnollHackM
# ---------------------------------------------------------------------------------------------------

$PackagingDir = $PSScriptRoot
$LinuxDir = Split-Path -Parent $PackagingDir
$PlatformsDir = Split-Path -Parent $LinuxDir
$GnollHackMDir = Split-Path -Parent $PlatformsDir

$ProjectPath = Join-Path $GnollHackMDir 'GnollHackM.csproj'
$LibsDir = Join-Path $LinuxDir 'libs'
$LicenseDir = Join-Path $LinuxDir 'licenses\sentry-native'
$LibraryPath = Join-Path $LibsDir 'libsentry.so'
$ManifestPath = Join-Path $LibsDir 'libsentry.build.props'

$GnhSentryWinDir = [System.IO.Path]::GetFullPath((Join-Path $GnollHackMDir '..\gnhsentrywin'))
$VendoredHeaderPath = Join-Path $GnhSentryWinDir 'include\sentry.h'
$CompareApiScriptPath = Join-Path $GnhSentryWinDir 'Compare-SentryNativeApi.ps1'

if (-not (Test-Path -LiteralPath $ProjectPath -PathType Leaf))
{
    throw "GnollHackM.csproj not found at '$ProjectPath'. The script must stay in GnollHackM\Platforms\Linux\Packaging."
}

if (-not (Test-Path -LiteralPath $VendoredHeaderPath -PathType Leaf))
{
    throw "The vendored sentry-native header was not found at '$VendoredHeaderPath'."
}

$SentryNativeVersion = Get-SentryNativeVersion -HeaderPath $VendoredHeaderPath

# ---------------------------------------------------------------------------------------------------
# 1. Prerequisites
# ---------------------------------------------------------------------------------------------------

Write-Step 'Checking prerequisites'

if ([string]::IsNullOrWhiteSpace($SourceDir))
{
    throw '-SourceDir must not be empty.'
}

if ($null -eq (Get-Command 'wsl.exe' -ErrorAction SilentlyContinue))
{
    throw 'wsl.exe was not found. The build runs in WSL.'
}

& wsl.exe -d $WslDistro -e true
if ($LASTEXITCODE -ne 0)
{
    throw "The WSL distribution '$WslDistro' cannot run commands (exit code $LASTEXITCODE)."
}

$wslVariables = [ordered]@{
    WSL_DISTRO         = $WslDistro
    SENTRY_VERSION     = $SentryNativeVersion
    SENTRY_COMMIT      = $SentryNativeCommit
    MAX_GLIBC          = $MaxGlibcVersion
    SOURCE_DIR         = $SourceDir
    VENDORED_HEADER    = (ConvertTo-WslPath $VendoredHeaderPath)
    COMPARE_API_SCRIPT = $CompareApiScriptPath
    WIN_LIBS_DIR       = (ConvertTo-WslPath $LibsDir)
    WIN_LICENSE_DIR    = (ConvertTo-WslPath $LicenseDir)
}

# ---------------------------------------------------------------------------------------------------
# 2-5. Linux steps in WSL
# ---------------------------------------------------------------------------------------------------

Write-Step "Checking the build tools and the sentry-native $SentryNativeVersion checkout in $WslDistro"
Invoke-WslScript -StepName 'check' -Body $BashCheck -Variables $wslVariables -Common $BashCommon -Distro $WslDistro

Write-Step 'Building libsentry.so (RelWithDebInfo, inproc backend, no transport)'
Invoke-WslScript -StepName 'build' -Body $BashBuild -Variables $wslVariables -Common $BashCommon -Distro $WslDistro

Write-Step 'Stripping and checking libsentry.so'
Invoke-WslScript -StepName 'strip and check' -Body $BashStripAndCheck -Variables $wslVariables -Common $BashCommon -Distro $WslDistro

Write-Step 'Copying libsentry.so and its licenses'
Invoke-WslScript -StepName 'copy' -Body $BashCopy -Variables $wslVariables -Common $BashCommon -Distro $WslDistro

# ---------------------------------------------------------------------------------------------------
# 6. Manifest: what the copied library was built from, for GnhSentryNative.targets
# ---------------------------------------------------------------------------------------------------

Write-Step 'Writing libsentry.build.props'

if (-not (Test-Path -LiteralPath $LibraryPath -PathType Leaf))
{
    throw "'$LibraryPath' was not created."
}

$libraryHash = (Get-FileHash -LiteralPath $LibraryPath -Algorithm SHA256).Hash.ToUpperInvariant()
$buildId = Get-ElfBuildId -ElfPath $LibraryPath
$headerHash = Get-NormalizedTextHash -TextFiles @($VendoredHeaderPath)
$manifestLines = @(
    '<!-- Written by Build-LinuxSentryNative.ps1; checked by GnhSentryNative.targets. Do not edit. -->',
    '<Project>',
    '  <PropertyGroup>',
    "    <GnhLibSentryNativeVersion>$SentryNativeVersion</GnhLibSentryNativeVersion>",
    "    <GnhLibSentryCommit>$SentryNativeCommit</GnhLibSentryCommit>",
    "    <GnhLibSentryHeaderSha256>$headerHash</GnhLibSentryHeaderSha256>",
    "    <GnhLibSentrySha256>$libraryHash</GnhLibSentrySha256>",
    "    <GnhLibSentryBuildId>$buildId</GnhLibSentryBuildId>",
    '  </PropertyGroup>',
    '</Project>',
    ''
)
$utf8NoBom = New-Object System.Text.UTF8Encoding($false)
[System.IO.File]::WriteAllText($ManifestPath, ($manifestLines -join "`r`n"), $utf8NoBom)
Write-Host "Wrote $ManifestPath (build ID $buildId)"

# ---------------------------------------------------------------------------------------------------
# 7. Summary
# ---------------------------------------------------------------------------------------------------

Write-Step "libsentry.so from sentry-native $SentryNativeVersion"

$expectedFiles = @($LibraryPath, $ManifestPath, (Join-Path $LicenseDir 'LICENSE'), (Join-Path $LicenseDir 'LICENSE-libunwind'))
foreach ($path in $expectedFiles)
{
    if (-not (Test-Path -LiteralPath $path -PathType Leaf))
    {
        throw "'$path' was not created."
    }
}

$item = Get-Item -LiteralPath $LibraryPath
$hash = $libraryHash.ToLowerInvariant()
Write-Host $LibraryPath
Write-Host "    size     $(Format-ByteSize $item.Length)"
Write-Host "    sha256   $hash"
Write-Host "    build ID $buildId"
Write-Host $ManifestPath
Write-Host $LicenseDir
