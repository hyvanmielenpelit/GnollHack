<#
.SYNOPSIS
    Copies the staged Linux build of GnollHack from WSL to a Steam Deck over SSH, with a logging launcher.

.DESCRIPTION
    Copies the staged tree that Build-LinuxPackages.ps1 left in WSL ($HOME/.cache/gnollhack-packaging/
    stage-<version>/gnollhack), so any of its targets will do, for example -Targets TarGz. The engine in that tree is
    stripped unless the build used -KeepDebugInfo. The copy uses rsync over SSH, and tar over SSH when the Deck or
    WSL has no rsync. The script never builds, and never launches the game: Gaming Mode starts it, so that gamescope
    and Steam Input are in play.

    Layout on the Deck, under the Deck user's home:
        <RemoteDir>/gnollhack/          the staged build, replaced on every deploy
        <RemoteDir>/run-gnollhack.sh    the launcher, the target of a Non-Steam Game shortcut
        <RemoteDir>/VERSION             version, repository state at deploy time, engine, deploy time
        <RemoteDir>/logs/               one log per run, the last 20 kept, and latest.log
        <RemoteDir>/dev.env             optional; sourced by the launcher, never touched by this script
    Game data in ~/.local/share/GnollHack is never touched.

    The Deck can be reached over any IP route:
        Wi-Fi, same network as the PC     -DeckHost steamdeck, steamdeck.local, or the Deck's IPv4 address
        Wired, through the router         the same, with a USB-C Ethernet adapter or dock on the Deck
        Wired, Deck straight to the PC    -DeckHost 10.42.0.1, with the Deck's wired connection set to
                                          "Shared to other computers" in Desktop Mode
    -DeckHost is resolved on Windows, which resolves .local names over mDNS; WSL is given the IPv4 address. The
    Deck's SSH host key is checked under -HostKeyAlias, not under the address, so every route shares one trusted key.

    The script runs ssh in batch mode: it never asks for or handles a password, and refuses a host key it does not
    already trust. One-time setup, done by a person:
        1. On the Deck, in Desktop Mode, in Konsole: passwd (sets the deck password), then
           sudo systemctl enable --now sshd
        2. In the WSL distribution: ssh-keygen -t ed25519, if there is no key yet.
        3. Run .\Deploy-SteamDeck.ps1 (with -DeckHost if needed). The first run stops at the connection check and
           prints the exact command for the Deck's current address, for example
               wsl -d Ubuntu-20.04 ssh-copy-id -o HostKeyAlias=steamdeck deck@192.0.2.10
           Run it, confirm the host key and enter the deck password once.
        4. After the first deploy, in Desktop Mode, in Steam: Add a Game, Add a Non-Steam Game, and browse to
           /home/deck/gnollhack-dev/run-gnollhack.sh. The shortcut then appears in the Gaming Mode library.

.PARAMETER DeckHost
    The Deck's host name, .local name or IPv4 address. Default: steamdeck, SteamOS's default host name.

.PARAMETER DeckUser
    The user on the Deck. Default: deck.

.PARAMETER HostKeyAlias
    The name the Deck's SSH host key is trusted under in WSL's known_hosts. It names the device, not the route;
    give a second Deck its own alias. Default: steamdeck.

.PARAMETER RemoteDir
    The deploy folder, relative to the Deck user's home. No segment may start with a dot. Default: gnollhack-dev.

.PARAMETER Version
    The staged version to deploy. Default: <ApplicationDisplayVersion>.<ApplicationVersion> from GnollHackM.csproj.

.PARAMETER SshKeyPath
    The WSL path of the private key to use. Default: ssh's default keys in WSL.

.PARAMETER WslDistro
    The WSL distribution that holds the staged build and runs ssh. Default: the default WSL distribution.

.PARAMETER DryRun
    Shows what rsync would copy, and writes nothing on the Deck.

.PARAMETER Force
    Deploys even while GnollHackM runs on the Deck.

.EXAMPLE
    .\Deploy-SteamDeck.ps1

    Deploys the staged build of the current version to steamdeck.

.EXAMPLE
    .\Deploy-SteamDeck.ps1 -DryRun

    Lists what would be copied, and writes nothing on the Deck.

.EXAMPLE
    .\Deploy-SteamDeck.ps1 -DeckHost steamdeck.local

    Finds the Deck over mDNS, for a router that does not register DHCP host names.

.EXAMPLE
    .\Deploy-SteamDeck.ps1 -DeckHost 10.42.0.1

    Deploys over a direct cable, with the Deck's wired connection in shared mode.

.EXAMPLE
    .\Deploy-SteamDeck.ps1 -DeckHost 192.0.2.10

    Deploys to the Deck's address, as shown in Settings > Internet on the Deck.

.EXAMPLE
    .\Build-LinuxPackages.ps1 -Targets TarGz -NoSentryUpload; .\Deploy-SteamDeck.ps1

    Builds and stages the current version, then deploys it.
#>
[CmdletBinding()]
param(
    [string]$DeckHost = 'steamdeck',

    [string]$DeckUser = 'deck',

    [string]$HostKeyAlias = 'steamdeck',

    [string]$RemoteDir = 'gnollhack-dev',

    [string]$Version = '',

    [string]$SshKeyPath = '',

    [string]$WslDistro = '',

    [switch]$DryRun,

    [switch]$Force
)

Set-StrictMode -Version 2
$ErrorActionPreference = 'Stop'

# ---------------------------------------------------------------------------------------------------
# Helper functions
# ---------------------------------------------------------------------------------------------------

function Write-Step
{
    param([string]$Message)
    Write-Host ''
    Write-Host "==> $Message" -ForegroundColor Cyan
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

# Throws unless the value matches the pattern, case-sensitively.
function Assert-Parameter
{
    param([string]$Name, [AllowEmptyString()][string]$Value, [string]$Pattern, [string]$Expected)
    if ($Value -cnotmatch $Pattern)
    {
        throw "-$Name '$Value' is not valid: expected $Expected."
    }
}

# ---------------------------------------------------------------------------------------------------
# Bash steps. Each runs after the variables from Invoke-WslScript and $BashCommon.
# ---------------------------------------------------------------------------------------------------

$BashCommon = @'
WORK="$HOME/.cache/gnollhack-packaging"
STAGED="$WORK/stage-$VERSION/gnollhack"
fail() { echo "error: $*" >&2; exit 1; }
warn() { echo "warning: $*" >&2; }
'@

# The parameter checks on the Windows side keep every value used unquoted in a remote command free of characters
# a shell would interpret.
$BashDeploy = @'
for tool in ssh tar; do
    command -v "$tool" >/dev/null 2>&1 || fail "'$tool' is not installed in WSL."
done
if [ ! -f "$STAGED/GnollHackM" ]; then
    existing=""
    for dir in "$WORK"/stage-*; do
        if [ -d "$dir" ]; then
            existing="$existing ${dir##*/}"
        fi
    done
    fail "No staged build at $STAGED. Staged builds in WSL:${existing:- none}. Run Build-LinuxPackages.ps1 first (any target, for example -Targets TarGz), or pass -Version."
fi

SSH_OPTS=(-o BatchMode=yes -o ConnectTimeout=10 -o "HostKeyAlias=$HOST_KEY_ALIAS")
if [ -n "$SSH_KEY" ]; then
    SSH_OPTS+=(-i "$SSH_KEY")
fi
TARGET="$DECK_USER@$DECK_ADDRESS"
remote() { ssh "${SSH_OPTS[@]}" "$TARGET" "$@"; }

# Connection check
echo "Connecting to $TARGET, with the host key checked as '$HOST_KEY_ALIAS'"
if ! check_errors=$(remote true 2>&1 >/dev/null); then
    printf '%s\n' "$check_errors" >&2
    case "$check_errors" in
    *"REMOTE HOST IDENTIFICATION HAS CHANGED"*)
        fail "The device at $DECK_ADDRESS is not the one trusted as '$HOST_KEY_ALIAS': a reinstalled Deck, or another device. Find out which before changing ~/.ssh/known_hosts in WSL. A second Deck needs its own -HostKeyAlias."
        ;;
    *"Host key verification failed"*|*"Permission denied"*)
        copy_id="wsl -d $WSL_DISTRO ssh-copy-id"
        if [ -n "$SSH_KEY" ]; then
            copy_id="$copy_id -i $SSH_KEY.pub"
        fi
        copy_id="$copy_id -o HostKeyAlias=$HOST_KEY_ALIAS $TARGET"
        fail "The one-time SSH setup for $TARGET is missing. Run this, confirm the Deck's host key and enter the deck password once:
    $copy_id
If WSL has no SSH key yet, first run: wsl -d $WSL_DISTRO ssh-keygen -t ed25519"
        ;;
    *)
        where="$DECK_HOST at $DECK_ADDRESS"
        if [ "$DECK_HOST" = "$DECK_ADDRESS" ]; then
            where="the Deck at $DECK_ADDRESS"
        fi
        fail "Cannot reach $where. Check that the Deck is awake, on the same network or cable as this PC, and has sshd enabled (sudo systemctl enable --now sshd, in Desktop Mode)."
        ;;
    esac
fi

REMOTE_HOME=$(remote 'printf %s "$HOME"')
[[ "$REMOTE_HOME" =~ ^/[A-Za-z0-9._/-]*$ ]] || fail "Unexpected home folder on the Deck: '$REMOTE_HOME'."
REMOTE="$REMOTE_HOME/$REMOTE_DIR"

if remote 'pgrep -x GnollHackM' >/dev/null; then
    if [ "$FORCE" = "1" ]; then
        warn "GnollHackM is running on the Deck; deploying anyway (-Force)."
    else
        fail "GnollHackM is running on the Deck; quit it, or pass -Force."
    fi
fi

ENGINE="unknown"
if command -v readelf >/dev/null 2>&1 && [ -f "$STAGED/libgnollhacklinux.so" ]; then
    sections=$(readelf -S "$STAGED/libgnollhacklinux.so" 2>/dev/null || true)
    case "$sections" in
    *.debug_info*) ENGINE="with debug information" ;;
    "") ;;
    *) ENGINE="stripped" ;;
    esac
fi

if [ "$DRY_RUN" = "0" ]; then
    remote "mkdir -p $REMOTE/gnollhack"
fi

if command -v rsync >/dev/null 2>&1 && remote 'command -v rsync' >/dev/null 2>&1; then
    DRY_RUN_FLAGS=""
    if [ "$DRY_RUN" = "1" ]; then
        DRY_RUN_FLAGS="--dry-run --itemize-changes"
    fi
    echo "Copying $STAGED to $TARGET:$REMOTE/gnollhack/ with rsync"
    # DRY_RUN_FLAGS is unquoted, so that it vanishes when empty
    rsync -a --delete --compress --info=stats1 ${DRY_RUN_FLAGS} -e "ssh ${SSH_OPTS[*]}" \
        "$STAGED/" "$TARGET:$REMOTE/gnollhack/"
else
    warn "rsync is missing in WSL or on the Deck; copying the whole build with tar."
    if [ "$DRY_RUN" = "1" ]; then
        echo "Dry run: would copy $(du -sh "$STAGED" | cut -f1) from $STAGED to $TARGET:$REMOTE/gnollhack/ with tar."
    else
        remote "rm -rf $REMOTE/gnollhack.new && mkdir -p $REMOTE/gnollhack.new"
        tar -C "$STAGED" -cf - . | remote "tar -C $REMOTE/gnollhack.new -xf -"
        remote "rm -rf $REMOTE/gnollhack.old; if [ -d $REMOTE/gnollhack ]; then mv $REMOTE/gnollhack $REMOTE/gnollhack.old; fi; mv $REMOTE/gnollhack.new $REMOTE/gnollhack && rm -rf $REMOTE/gnollhack.old"
    fi
fi

# dev.env and logs/ are left as they are
if [ "$DRY_RUN" = "0" ]; then
    tr -d '\r' < "$LAUNCHER_SRC" | remote "cat > $REMOTE/run-gnollhack.sh.new && chmod 755 $REMOTE/run-gnollhack.sh.new && mv $REMOTE/run-gnollhack.sh.new $REMOTE/run-gnollhack.sh"
    printf '%s\n' "version: $VERSION" "commit: $COMMIT" "engine: $ENGINE" "deployed: $(date -Iseconds)" \
        | remote "cat > $REMOTE/VERSION"
fi

ssh_command="wsl -d $WSL_DISTRO ssh -o HostKeyAlias=$HOST_KEY_ALIAS"
if [ -n "$SSH_KEY" ]; then
    ssh_command="$ssh_command -i $SSH_KEY"
fi
echo ""
if [ "$DRY_RUN" = "1" ]; then
    echo "Dry run: nothing was written on the Deck."
else
    echo "Deployed $VERSION ($ENGINE engine) to $TARGET."
fi
echo "Deploy folder: $REMOTE"
echo "Launcher:      $REMOTE/run-gnollhack.sh (the target of the Non-Steam Game shortcut)"
echo "Logs:          $REMOTE/logs/"
echo "Follow the newest log from Windows with:"
echo "    $ssh_command $TARGET tail -F $REMOTE/logs/latest.log"
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
$LauncherPath = Join-Path $PackagingDir 'steamdeck\run-gnollhack.sh'

if (-not (Test-Path -LiteralPath $ProjectPath -PathType Leaf))
{
    throw "GnollHackM.csproj not found at '$ProjectPath'. The script must stay in GnollHackM\Platforms\Linux\Packaging."
}
if (-not (Test-Path -LiteralPath $LauncherPath -PathType Leaf))
{
    throw "Missing '$LauncherPath'."
}

# ---------------------------------------------------------------------------------------------------
# 1. Parameters and prerequisites
# ---------------------------------------------------------------------------------------------------

Assert-Parameter 'DeckHost' $DeckHost '^[A-Za-z0-9]([A-Za-z0-9.-]*[A-Za-z0-9])?$' 'a host name, a .local name or an IPv4 address'
Assert-Parameter 'DeckUser' $DeckUser '^[a-z_][a-z0-9_-]*$' 'a Linux user name in lowercase'
Assert-Parameter 'HostKeyAlias' $HostKeyAlias '^[a-z0-9]([a-z0-9.-]*[a-z0-9])?$' 'a lowercase name such as steamdeck'
Assert-Parameter 'RemoteDir' $RemoteDir '^[A-Za-z0-9_-][A-Za-z0-9._-]*(/[A-Za-z0-9_-][A-Za-z0-9._-]*)*$' 'a folder relative to the home folder, with no spaces and no segment starting with a dot'
if (-not [string]::IsNullOrEmpty($Version))
{
    Assert-Parameter 'Version' $Version '^\d+(\.\d+)*$' 'a dotted number'
}
if (-not [string]::IsNullOrEmpty($SshKeyPath))
{
    Assert-Parameter 'SshKeyPath' $SshKeyPath '^[A-Za-z0-9_./~+-]+$' 'a WSL path with no spaces or shell characters'
}

Write-Step 'Checking prerequisites'

if ($null -eq (Get-Command 'wsl.exe' -ErrorAction SilentlyContinue))
{
    throw 'wsl.exe was not found. The staged build is in WSL, and ssh runs there.'
}

$WslDistro = Resolve-WslDistro $WslDistro
Write-Host "WSL distribution: $WslDistro"

& wsl.exe -d $WslDistro -e true
if ($LASTEXITCODE -ne 0)
{
    throw "The WSL distribution '$WslDistro' cannot run commands (exit code $LASTEXITCODE)."
}

# ---------------------------------------------------------------------------------------------------
# 2. Version
# ---------------------------------------------------------------------------------------------------

if ([string]::IsNullOrEmpty($Version))
{
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
}
Write-Host "Version $Version"

# ---------------------------------------------------------------------------------------------------
# 3. The Deck's address, resolved on Windows: WSL does not resolve .local names
# ---------------------------------------------------------------------------------------------------

Write-Step "Resolving $DeckHost"

# IPAddress.TryParse also accepts shorthand such as 10 (0.0.0.10), 192.0.2 (192.0.0.2), 0x0A000001 and octal
# 010.0.0.1, and Dns.GetHostAddresses returns such a parse without asking DNS, so only a plain dotted quad counts as
# an address and anything else that parses is refused.
$DeckAddress = ''
$parsedAddress = $null
$isAddress = [System.Net.IPAddress]::TryParse($DeckHost, [ref]$parsedAddress)
if ($isAddress)
{
    if ($DeckHost -cnotmatch '^(0|[1-9]\d{0,2})(\.(0|[1-9]\d{0,2})){3}$')
    {
        throw "-DeckHost '$DeckHost' is not a full IPv4 address: write all four numbers, such as 192.0.2.10."
    }
    $DeckAddress = $DeckHost
    Write-Host "Deck address: $DeckAddress"
}
else
{
    $addresses = @()
    try
    {
        $addresses = @([System.Net.Dns]::GetHostAddresses($DeckHost) |
            Where-Object { $_.AddressFamily -eq [System.Net.Sockets.AddressFamily]::InterNetwork } |
            ForEach-Object { $_.ToString() })
    }
    catch
    {
        $addresses = @()
    }
    if ($addresses.Count -eq 0)
    {
        $ipHint = "the Deck's IP address from Settings > Internet on the Deck"
        if ($DeckHost -like '*.local')
        {
            $hint = "Try -DeckHost with $ipHint."
        }
        elseif ($DeckHost.Contains('.'))
        {
            $hint = "Try -DeckHost steamdeck.local, or $ipHint."
        }
        else
        {
            $hint = "Try -DeckHost $DeckHost.local, or $ipHint."
        }
        throw "Cannot resolve '$DeckHost' to an IPv4 address on Windows. $hint"
    }
    # IPv6 addresses are left out: a link-local one carries a Windows interface index that means nothing in WSL
    $DeckAddress = $addresses[0]
    Write-Host "Deck address: $DeckAddress ($DeckHost)"
    if ($addresses.Count -gt 1)
    {
        Write-Host "All IPv4 addresses of ${DeckHost}: $($addresses -join ', '). -DeckHost <address> picks another."
    }
}

# ---------------------------------------------------------------------------------------------------
# 4. Repository state for VERSION. It is the state at deploy time, which need not match the staged build.
# ---------------------------------------------------------------------------------------------------

$Commit = 'unknown'
if ($null -ne (Get-Command 'git' -ErrorAction SilentlyContinue))
{
    $previousPreference = $ErrorActionPreference
    $ErrorActionPreference = 'Continue'
    try
    {
        $hash = @(& git -C $RepoRoot rev-parse --short HEAD 2>$null) | Select-Object -First 1
        if (($LASTEXITCODE -eq 0) -and (-not [string]::IsNullOrWhiteSpace($hash)))
        {
            $Commit = $hash.Trim()
            $changes = @(& git -C $RepoRoot status --porcelain 2>$null)
            if (($LASTEXITCODE -eq 0) -and ($changes.Count -gt 0))
            {
                $Commit = "$Commit (uncommitted changes)"
            }
        }
    }
    finally
    {
        $ErrorActionPreference = $previousPreference
    }
}

# ---------------------------------------------------------------------------------------------------
# 5. Deploy
# ---------------------------------------------------------------------------------------------------

$dryRunValue = '0'
if ($DryRun)
{
    $dryRunValue = '1'
}
$forceValue = '0'
if ($Force)
{
    $forceValue = '1'
}

$variables = [ordered]@{
    VERSION        = $Version
    DECK_HOST      = $DeckHost
    DECK_ADDRESS   = $DeckAddress
    DECK_USER      = $DeckUser
    HOST_KEY_ALIAS = $HostKeyAlias
    REMOTE_DIR     = $RemoteDir
    SSH_KEY        = $SshKeyPath
    DRY_RUN        = $dryRunValue
    FORCE          = $forceValue
    COMMIT         = $Commit
    WSL_DISTRO     = $WslDistro
    LAUNCHER_SRC   = (ConvertTo-WslPath $LauncherPath)
}

Write-Step "Deploying $Version to $DeckUser@$DeckAddress"
Invoke-WslScript -StepName 'deploy' -Body $BashDeploy -Variables $variables -Common $BashCommon -Distro $WslDistro
