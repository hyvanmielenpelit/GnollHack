<#
.SYNOPSIS
    Builds recover, the standalone save recovery utility, for the Linux build of GnollHack.

.DESCRIPTION
    Copies include\, sys\unix\ and util\recover.c from this repository to $HOME/.cache/gnollhack-recover in WSL, which
    is recreated on every run, configures them with sys/unix/setup.sh and the release hints in sys/unix/hints/linux,
    and compiles recover.c with exactly the CC, CFLAGS, LFLAGS and LIBS of the generated util/Makefile, with
    GAME=gnollhack. The repository is never written to, except for the finished binary.

    The binary is stripped with strip --strip-all, which keeps its build ID, and checked: libc.so.6 is its only NEEDED
    entry, no glibc symbol version is above $MaxGlibcVersion, and it prints its usage line. Only then is it copied to
    Platforms\Linux\gnh\recover, which GnollHackM.csproj ships as gnh/recover. Rerun the script after a change to
    util/recover.c or to the save file headers it reads.

    The script never downloads anything. When a build tool is missing in WSL, it stops with the command that installs
    it.

.PARAMETER WslDistro
    The WSL distribution that builds the utility. Default: Ubuntu-20.04 (glibc 2.31), which also builds
    libgnollhacklinux.so.

.EXAMPLE
    .\Build-LinuxRecover.ps1

    Builds recover in Ubuntu-20.04 and copies it to Platforms\Linux\gnh\recover.
#>
[CmdletBinding()]
param(
    [string]$WslDistro = 'Ubuntu-20.04'
)

Set-StrictMode -Version 2
$ErrorActionPreference = 'Stop'

# The highest glibc symbol version the utility may need: the floor of the Linux packages
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

    $fileName = 'gnollhack-recover-' + [guid]::NewGuid().ToString('N') + '.sh'
    $scriptPath = Join-Path ([System.IO.Path]::GetTempPath()) $fileName
    try
    {
        Write-TextFileLf -Path $scriptPath -Text $builder.ToString()
        $wslScriptPath = ConvertTo-WslPath $scriptPath
        # The compiler writes warnings to stderr; Windows PowerShell 5.1 would turn each such line into a terminating
        # error under 'Stop', so the lines are shown as text and only the exit code decides success
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
    return ('{0:N1} KB ({1} bytes)' -f ($Bytes / 1KB), $Bytes)
}

# ---------------------------------------------------------------------------------------------------
# Bash steps. Each runs after the variables from Invoke-WslScript and $BashCommon.
# ---------------------------------------------------------------------------------------------------

$BashCommon = @'
WORK="$HOME/.cache/gnollhack-recover"
TREE="$WORK/tree"
OUT="$WORK/out"
BIN="$OUT/recover"
fail() { echo "error: $*" >&2; exit 1; }
build_id() { readelf -n "$1" 2>/dev/null | sed -n '/Build ID:/{s/.*Build ID: \([0-9a-f]*\).*/\1/p;q;}' || true; }
'@

# Tools and sources: every missing package in one install command.
$BashCheck = @'
missing=()
for entry in gcc:gcc make:make strip:binutils readelf:binutils objdump:binutils sed:sed awk:gawk \
    sha256sum:coreutils sort:coreutils; do
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
echo "gcc: $(gcc --version | head -n 1)"
for path in util/recover.c include/global.h sys/unix/setup.sh sys/unix/Makefile.utl sys/unix/hints/linux; do
    [ -f "$REPO_DIR/$path" ] || fail "$REPO_DIR/$path is missing."
done
'@

# Build: a fresh copy of the sources, configured with the release hints, and recover.c compiled with the flags of the
# generated util/Makefile. "make recover" is not used: recover.o depends on ../include/date.h, whose double-colon
# rule always rebuilds makedefs.
$BashBuild = @'
rm -rf "$WORK"
mkdir -p "$TREE/src" "$TREE/doc" "$TREE/dat" "$TREE/util" "$TREE/sys" "$OUT"
cp -r "$REPO_DIR/include" "$TREE/include"
cp -r "$REPO_DIR/sys/unix" "$TREE/sys/unix"
cp "$REPO_DIR/util/recover.c" "$TREE/util/recover.c"
# A Windows checkout has CRLF; the shell scripts, makefiles and hints run in bash and make
for file in "$TREE"/sys/unix/*.sh "$TREE"/sys/unix/Makefile.* "$TREE"/sys/unix/hints/*; do
    if [ -f "$file" ]; then
        sed -i 's/\r$//' "$file"
    fi
done
(cd "$TREE" && sh sys/unix/setup.sh sys/unix/hints/linux)
cd "$TREE/util"
# Prints a variable of the generated Makefile without passing it through a shell
printf 'print-%%: ; @: $(info $($*))\n' > print.mk
make_var() { make -s --no-print-directory -f Makefile -f print.mk GAME=gnollhack "print-$1"; }
cc=$(make_var ACTUAL_CC)
cflags=$(make_var CFLAGS)
lflags=$(make_var LFLAGS)
libs=$(make_var LIBS)
echo "CC:     $cc"
echo "CFLAGS: $cflags"
echo "LFLAGS: $lflags"
echo "LIBS:   $libs"
# eval: the flags hold shell quoting (-DHACKDIR=\"...\"), which make would hand to sh the same way
eval "$cc $cflags -c recover.c -o recover.o"
eval "$cc $lflags -o recover recover.o $libs"
[ -f recover ] || fail "the build did not create $TREE/util/recover."
'@

# Strip and check: the stripped copy in $OUT must keep the build ID and pass every check before it is copied.
$BashStripAndCheck = @'
cp "$TREE/util/recover" "$BIN"
id_before=$(build_id "$BIN")
[ -n "$id_before" ] || fail "cannot read the build ID of $BIN."
size_before=$(stat -c %s "$BIN")
strip --strip-all "$BIN"
id_after=$(build_id "$BIN")
[ "$id_after" = "$id_before" ] || fail "stripping changed the build ID of recover from $id_before to '$id_after'."
echo "Stripped recover: $size_before -> $(stat -c %s "$BIN") bytes"

needed=$(readelf -d "$BIN" | sed -n 's/.*(NEEDED).*Shared library: \[\(.*\)\].*/\1/p' | tr '\n' ' ' | sed 's/ $//')
echo "NEEDED: $needed"
[ "$needed" = "libc.so.6" ] || fail "recover needs '$needed', not only libc.so.6."

highest_glibc=$(objdump -T "$BIN" | grep -o 'GLIBC_[0-9][0-9.]*' | sed 's/^GLIBC_//' | sort -u -V | tail -n 1 || true)
[ -n "$highest_glibc" ] || fail "objdump -T shows no GLIBC_ symbol versions in $BIN."
echo "Highest glibc version: GLIBC_$highest_glibc"
newest=$(printf '%s\n%s\n' "$highest_glibc" "$MAX_GLIBC" | sort -V | tail -n 1)
[ "$newest" = "$MAX_GLIBC" ] || fail "recover needs GLIBC_$highest_glibc, above the GLIBC_$MAX_GLIBC floor."

usage=$("$BIN" 2>&1 || true)
grep -q 'Usage:' <<< "$usage" || fail "recover did not print its usage line: $usage"
echo "Usage line: $(grep -m 1 'Usage:' <<< "$usage")"

echo "Build ID: $id_after"
echo "SHA-256:  $(sha256sum "$BIN" | cut -d ' ' -f 1)"
'@

# Copy the checked binary to the Windows tree.
$BashCopy = @'
mkdir -p "$WIN_GNH_DIR"
cp "$BIN" "$WIN_GNH_DIR/recover"
echo "Copied recover to $WIN_GNH_DIR"
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
$GnhDir = Join-Path $LinuxDir 'gnh'
$RecoverPath = Join-Path $GnhDir 'recover'

if (-not (Test-Path -LiteralPath $ProjectPath -PathType Leaf))
{
    throw "GnollHackM.csproj not found at '$ProjectPath'. The script must stay in GnollHackM\Platforms\Linux\Packaging."
}
if (-not (Test-Path -LiteralPath (Join-Path $RepoRoot 'util\recover.c') -PathType Leaf))
{
    throw "util\recover.c not found under '$RepoRoot'."
}

# ---------------------------------------------------------------------------------------------------
# 1. Prerequisites
# ---------------------------------------------------------------------------------------------------

Write-Step 'Checking prerequisites'

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
    WSL_DISTRO  = $WslDistro
    MAX_GLIBC   = $MaxGlibcVersion
    REPO_DIR    = (ConvertTo-WslPath $RepoRoot)
    WIN_GNH_DIR = (ConvertTo-WslPath $GnhDir)
}

# ---------------------------------------------------------------------------------------------------
# 2-5. Linux steps in WSL
# ---------------------------------------------------------------------------------------------------

Write-Step "Checking the build tools in $WslDistro"
Invoke-WslScript -StepName 'check' -Body $BashCheck -Variables $wslVariables -Common $BashCommon -Distro $WslDistro

Write-Step 'Building recover with the release hints (sys/unix/hints/linux)'
Invoke-WslScript -StepName 'build' -Body $BashBuild -Variables $wslVariables -Common $BashCommon -Distro $WslDistro

Write-Step 'Stripping and checking recover'
Invoke-WslScript -StepName 'strip and check' -Body $BashStripAndCheck -Variables $wslVariables -Common $BashCommon -Distro $WslDistro

Write-Step 'Copying recover'
Invoke-WslScript -StepName 'copy' -Body $BashCopy -Variables $wslVariables -Common $BashCommon -Distro $WslDistro

# ---------------------------------------------------------------------------------------------------
# 6. Summary
# ---------------------------------------------------------------------------------------------------

Write-Step 'recover'

if (-not (Test-Path -LiteralPath $RecoverPath -PathType Leaf))
{
    throw "'$RecoverPath' was not created."
}

$item = Get-Item -LiteralPath $RecoverPath
$hash = (Get-FileHash -LiteralPath $RecoverPath -Algorithm SHA256).Hash.ToLowerInvariant()
Write-Host $RecoverPath
Write-Host "    size    $(Format-ByteSize $item.Length)"
Write-Host "    sha256  $hash"
