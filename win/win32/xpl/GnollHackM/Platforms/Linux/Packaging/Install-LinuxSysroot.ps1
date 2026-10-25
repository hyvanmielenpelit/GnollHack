<#
.SYNOPSIS
    Installs the glibc 2.27 sysroot that the Linux build of GnollHack compiles and links against.

.DESCRIPTION
    Downloads three pinned Ubuntu 18.04 packages (libc6, libc6-dev and linux-libc-dev), checks their SHA-256 values,
    and extracts them to /opt/gnollhack/sysroot-glibc-2.27 in a WSL distribution. gnollhacklinux.vcxproj,
    Build-LinuxRecover.ps1 and Build-LinuxSentryNative.ps1 build against this sysroot instead of the distribution's own
    glibc, so that the binaries load on glibc 2.27, the floor of the Linux packages, whichever Ubuntu builds them.

    Each package is tried from archive.ubuntu.com, then old-releases.ubuntu.com, then Launchpad, until one host returns
    a file with the pinned hash. Packages already downloaded with the right hash are not downloaded again.

    Afterwards the script compiles and links a test library against the sysroot, with the options the projects use, and
    checks that it needs no glibc symbol version above $MaxGlibcVersion. Only then does it write the stamp file
    gnollhack-sysroot.txt, which the builds check for.

    The script runs as root in WSL (wsl -u root), which asks for no password. Run it once in every distribution that
    builds the Linux version, and again when the pinned packages change.

.PARAMETER WslDistro
    The WSL distribution to install the sysroot in. Default: the default WSL distribution.

.EXAMPLE
    .\Install-LinuxSysroot.ps1

    Installs the sysroot in the default WSL distribution.

.EXAMPLE
    .\Install-LinuxSysroot.ps1 -WslDistro Ubuntu-24.04

    Installs the sysroot in Ubuntu-24.04.
#>
[CmdletBinding()]
param(
    [string]$WslDistro = ''
)

Set-StrictMode -Version 2
$ErrorActionPreference = 'Stop'

# Where the sysroot goes; gnollhacklinux.vcxproj (GnhLinuxSysroot) and the Build-Linux*.ps1 scripts use the same path
$SysrootPath = '/opt/gnollhack/sysroot-glibc-2.27'
$DebsPath = '/opt/gnollhack/sysroot-debs'

# The highest glibc symbol version a binary built against the sysroot may need: the floor of the Linux packages
$MaxGlibcVersion = '2.27'

# Ubuntu 18.04 (bionic) amd64 packages: file name, pool path and SHA-256, as published on Launchpad
$Packages = @(
    @{ File = 'libc6_2.27-3ubuntu1_amd64.deb'; Pool = 'g/glibc/'; Sha256 = '1e1eb86fd646aa68f7144ec692b837b6b352d215880c6a2d0c92c19d36938427' },
    @{ File = 'libc6-dev_2.27-3ubuntu1_amd64.deb'; Pool = 'g/glibc/'; Sha256 = 'e426c70a940a7d0c5c95823a5fd01f26bd8bcb08d109df2f8c96c439da8dc440' },
    @{ File = 'linux-libc-dev_4.15.0-213.224_amd64.deb'; Pool = 'l/linux/'; Sha256 = '06d9f16644d3a1b2ab7dd01730e833d516b6f575d975855f496eea6463fef2f6' }
)

# Download URLs, tried in order; {pool} and {file} are replaced. Bionic moves to old-releases when its support ends.
$DownloadUrls = @(
    'https://archive.ubuntu.com/ubuntu/pool/main/{pool}{file}',
    'https://old-releases.ubuntu.com/ubuntu/pool/main/{pool}{file}',
    'https://launchpad.net/ubuntu/+archive/primary/+files/{file}'
)

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

# Returns the WSL distribution to use: the given name, or the name of the default distribution. Docker Desktop's
# distributions are refused, since they cannot build anything.
function Resolve-WslDistro
{
    param([AllowEmptyString()][string]$Name)
    if ([string]::IsNullOrEmpty($Name))
    {
        # Not wsl.exe -l, whose UTF-16 output Windows PowerShell reads as interleaved null characters
        # @() lets wsl.exe finish, so that $LASTEXITCODE is its own exit code; Select-Object -First alone would stop it
        $resolved = @(& wsl.exe -e sh -c 'echo $WSL_DISTRO_NAME') | Select-Object -First 1
        if (($LASTEXITCODE -ne 0) -or [string]::IsNullOrWhiteSpace($resolved))
        {
            throw 'The default WSL distribution cannot run commands. Install Ubuntu in WSL, or pass -WslDistro.'
        }
        $Name = $resolved.Trim()
    }
    if ($Name -like 'docker-desktop*')
    {
        throw "'$Name' is Docker Desktop's WSL distribution, which cannot build GnollHack. Pass -WslDistro <name>, or make an Ubuntu distribution the default with 'wsl --set-default <name>'."
    }
    return $Name
}

# Runs a bash script in WSL as root: the variables as bash assignments, the common definitions, then the body.
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

    $fileName = 'gnollhack-sysroot-' + [guid]::NewGuid().ToString('N') + '.sh'
    $scriptPath = Join-Path ([System.IO.Path]::GetTempPath()) $fileName
    try
    {
        Write-TextFileLf -Path $scriptPath -Text $builder.ToString()
        $wslScriptPath = ConvertTo-WslPath $scriptPath
        # Tools write progress to stderr; Windows PowerShell 5.1 would turn each such line into a terminating error
        # under 'Stop', so the lines are shown as text and only the exit code decides success
        $previousPreference = $ErrorActionPreference
        $ErrorActionPreference = 'Continue'
        try
        {
            & wsl.exe -d $Distro -u root -e bash $wslScriptPath 2>&1 | ForEach-Object {
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

# ---------------------------------------------------------------------------------------------------
# Bash steps. Each runs after the variables from Invoke-WslScript and $BashCommon.
# ---------------------------------------------------------------------------------------------------

$BashCommon = @'
fail() { echo "error: $*" >&2; exit 1; }
warn() { echo "warning: $*" >&2; }
LIBDIR="$SYSROOT/usr/lib/x86_64-linux-gnu"
LINK_OPTIONS="--sysroot=$SYSROOT -B$LIBDIR -L$LIBDIR -L$SYSROOT/lib/x86_64-linux-gnu -Wl,-z,defs"
'@

# Tools: every missing package in one install command.
$BashCheck = @'
missing=()
for entry in curl:curl dpkg-deb:dpkg sha256sum:coreutils realpath:coreutils gcc:gcc objdump:binutils; do
    tool="${entry%%:*}"
    package="${entry#*:}"
    if ! command -v "$tool" >/dev/null 2>&1; then
        echo "'$tool' is not installed." >&2
        missing+=("$package")
    fi
done
if [ "${#missing[@]}" -gt 0 ]; then
    fail "install the missing tools with: wsl -d $WSL_DISTRO -u root -- apt-get install -y ${missing[*]}"
fi
echo "gcc: $(gcc --version | head -n 1)"
echo "glibc of the distribution: $(ldd --version | head -n 1)"
'@

# Download: each package from the first URL that returns the pinned hash.
$BashDownload = @'
mkdir -p "$DEBS"
while IFS='|' read -r file pool sha; do
    [ -n "$file" ] || continue
    dest="$DEBS/$file"
    if [ -f "$dest" ] && [ "$(sha256sum "$dest" | cut -d ' ' -f 1)" = "$sha" ]; then
        echo "$file: already downloaded"
        continue
    fi
    done_file=0
    while IFS= read -r template; do
        [ -n "$template" ] || continue
        url=${template//\{pool\}/$pool}
        url=${url//\{file\}/$file}
        rm -f "$dest.part"
        if curl -fsSL --retry 2 -o "$dest.part" "$url"; then
            actual=$(sha256sum "$dest.part" | cut -d ' ' -f 1)
            if [ "$actual" = "$sha" ]; then
                mv "$dest.part" "$dest"
                echo "$file: $url"
                done_file=1
                break
            fi
            warn "$url has SHA-256 $actual, expected $sha."
        else
            warn "$url could not be downloaded."
        fi
    done <<< "$URLS"
    rm -f "$dest.part"
    [ "$done_file" = "1" ] || fail "$file could not be downloaded with the pinned SHA-256 from any host."
done <<< "$PACKAGES"
'@

# Extract: a fresh sysroot, then the self-check, and the stamp file only when the check passes.
$BashExtract = @'
rm -rf "$SYSROOT"
mkdir -p "$SYSROOT"
while IFS='|' read -r file pool sha; do
    [ -n "$file" ] || continue
    if ! output=$(dpkg-deb -x "$DEBS/$file" "$SYSROOT" 2>&1); then
        printf '%s\n' "$output" | tail -n 5 >&2
        if grep -q 'Function not implemented' <<< "$output"; then
            fail "$WSL_DISTRO cannot extract packages, which happens with newer Ubuntu releases on WSL 1. Convert it with: wsl --set-version $WSL_DISTRO 2"
        fi
        fail "dpkg-deb could not extract $file."
    fi
    echo "Extracted $file"
done <<< "$PACKAGES"

# The development symlinks, such as usr/lib/x86_64-linux-gnu/libdl.so, are absolute and would resolve to the host's
# libraries; each becomes relative, pointing inside the sysroot
relinked=0
while IFS= read -r -d '' link; do
    target=$(readlink "$link")
    ln -sfn "$(realpath -m --relative-to="$(dirname "$link")" "$SYSROOT$target")" "$link"
    relinked=$((relinked + 1))
done < <(find "$SYSROOT" -type l -lname '/*' -print0)
echo "Made $relinked absolute symlinks relative"

# The functions that glibc re-versioned after 2.27, including the header redirects of _GNU_SOURCE, and functions of
# libdl and librt, which glibc 2.27 keeps outside libc
work=$(mktemp -d)
cat > "$work/check.c" <<'EOF'
#define _GNU_SOURCE
#include <dlfcn.h>
#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <termios.h>
double check_math(double x) { return pow(x, 2.5) + exp(x) + exp2(x) + log(x) + log2(x); }
int check_stat(const char *path) { struct stat st; return stat(path, &st); }
long check_scan(const char *s) { int a = 0; sscanf(s, "%d", &a); return a + strtol(s, NULL, 10); }
unsigned check_termios(const struct termios *t) { return (unsigned) cfgetospeed(t); }
int check_dl(void) { Dl_info info; return dladdr((void *) check_math, &info); }
int check_rt(const char *name) { return shm_open(name, O_RDONLY, 0); }
EOF
gcc -std=gnu17 --sysroot="$SYSROOT" -O2 -fPIC -c "$work/check.c" -o "$work/check.o"
gcc -shared $LINK_OPTIONS -o "$work/check.so" "$work/check.o" -lm -ldl -lrt
highest=$(objdump -T "$work/check.so" | grep -o 'GLIBC_[0-9][0-9.]*' | sed 's/^GLIBC_//' | sort -u -V | tail -n 1 || true)
rm -rf "$work"
[ -n "$highest" ] || fail "the test library has no GLIBC_ symbol versions; the sysroot is not used."
newest=$(printf '%s\n%s\n' "$highest" "$MAX_GLIBC" | sort -V | tail -n 1)
[ "$newest" = "$MAX_GLIBC" ] || fail "a test library built against the sysroot needs GLIBC_$highest, above GLIBC_$MAX_GLIBC."
echo "Test library built against the sysroot needs at most GLIBC_$highest"

{
    echo "GnollHack glibc $MAX_GLIBC sysroot, made by Install-LinuxSysroot.ps1 from:"
    while IFS='|' read -r file pool sha; do
        if [ -n "$file" ]; then
            echo "$sha  $file"
        fi
    done <<< "$PACKAGES"
} > "$SYSROOT/gnollhack-sysroot.txt"
echo "Sysroot ready in $SYSROOT"
'@

# ---------------------------------------------------------------------------------------------------
# 1. Prerequisites
# ---------------------------------------------------------------------------------------------------

Write-Step 'Checking prerequisites'

if ($null -eq (Get-Command 'wsl.exe' -ErrorAction SilentlyContinue))
{
    throw 'wsl.exe was not found. The sysroot is installed in WSL.'
}

$WslDistro = Resolve-WslDistro $WslDistro
Write-Host "WSL distribution: $WslDistro"

& wsl.exe -d $WslDistro -u root -e true
if ($LASTEXITCODE -ne 0)
{
    throw "The WSL distribution '$WslDistro' cannot run commands as root (exit code $LASTEXITCODE)."
}

$wslVariables = [ordered]@{
    WSL_DISTRO = $WslDistro
    SYSROOT    = $SysrootPath
    DEBS       = $DebsPath
    MAX_GLIBC  = $MaxGlibcVersion
    PACKAGES   = (($Packages | ForEach-Object { "$($_.File)|$($_.Pool)|$($_.Sha256)" }) -join "`n")
    URLS       = ($DownloadUrls -join "`n")
}

# ---------------------------------------------------------------------------------------------------
# 2-4. Linux steps in WSL
# ---------------------------------------------------------------------------------------------------

Write-Step "Checking the tools in $WslDistro"
Invoke-WslScript -StepName 'check' -Body $BashCheck -Variables $wslVariables -Common $BashCommon -Distro $WslDistro

Write-Step "Downloading the packages to $DebsPath"
Invoke-WslScript -StepName 'download' -Body $BashDownload -Variables $wslVariables -Common $BashCommon -Distro $WslDistro

Write-Step "Extracting and checking the sysroot in $SysrootPath"
Invoke-WslScript -StepName 'extract' -Body $BashExtract -Variables $wslVariables -Common $BashCommon -Distro $WslDistro

Write-Step "glibc $MaxGlibcVersion sysroot installed in $WslDistro"
Write-Host $SysrootPath
