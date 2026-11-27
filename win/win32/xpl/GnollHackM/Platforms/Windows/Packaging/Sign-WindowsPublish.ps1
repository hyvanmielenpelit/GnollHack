<#
.SYNOPSIS
    Authenticode-signs and verifies the WinUI 3 Steam publish folder.

.DESCRIPTION
    A ReadyToRun publish rewrites every managed assembly it compiles, which removes the upstream
    Authenticode signatures, and GnollHack's own files are not signed at all. Smart App Control blocks
    unsigned files it cannot judge safe. This script signs every unsigned .dll and .exe under the
    publish folder, leaves files that already carry a valid signature alone, and then verifies that
    every PE file has a valid signature.

    Signing uses either Azure Artifact Signing (a metadata.json and the client's
    Azure.CodeSigning.Dlib.dll, authenticated by the signed-in Azure user, for example after
    az login) or a code-signing certificate in the current user's certificate store.

    Exit code 0: every PE file has a valid signature. Exit code 1: anything else.

.PARAMETER PublishDir
    The folder to sign. Defaults to bin\Publish\Steam-win-x64 of the GnollHackM project.

.PARAMETER VerifyOnly
    Sign nothing; only verify.

.PARAMETER ArtifactSigningMetadata
    Artifact Signing metadata.json (Endpoint, CodeSigningAccountName, CertificateProfileName).
    Defaults to the GNOLLHACK_SIGN_METADATA environment variable.

.PARAMETER ArtifactSigningDlib
    The x64 Azure.CodeSigning.Dlib.dll of the Artifact Signing client tools.
    Defaults to the GNOLLHACK_SIGN_DLIB environment variable.

.PARAMETER CertificateThumbprint
    SHA-1 thumbprint of a code-signing certificate in Cert:\CurrentUser\My, used instead of
    Artifact Signing. Defaults to the GNOLLHACK_SIGN_THUMBPRINT environment variable.

.PARAMETER TimestampUrl
    RFC 3161 timestamp server. Defaults to the GNOLLHACK_SIGN_TIMESTAMP environment variable, else
    http://timestamp.acs.microsoft.com for Artifact Signing. Required with a certificate thumbprint.

.PARAMETER SignToolPath
    signtool.exe. Defaults to the newest x64 signtool.exe of the installed Windows SDKs.

.EXAMPLE
    & .\Platforms\Windows\Packaging\Sign-WindowsPublish.ps1

.EXAMPLE
    & .\Platforms\Windows\Packaging\Sign-WindowsPublish.ps1 -VerifyOnly
#>
[CmdletBinding()]
param(
    [string]$PublishDir,
    [switch]$VerifyOnly,
    [string]$ArtifactSigningMetadata,
    [string]$ArtifactSigningDlib,
    [string]$CertificateThumbprint,
    [string]$TimestampUrl,
    [string]$SignToolPath
)

$ErrorActionPreference = 'Stop'
$BatchSize = 30
$ArtifactSigningTimestampUrl = 'http://timestamp.acs.microsoft.com'

function Stop-Script([string]$Message)
{
    Write-Host $Message -ForegroundColor Red
    exit 1
}

# A variable set at user level after this window opened is read from the registry
function Get-SigningSetting([string]$Value, [string]$Name)
{
    if (-not [string]::IsNullOrWhiteSpace($Value))
    {
        return $Value.Trim()
    }
    $fromProcess = [Environment]::GetEnvironmentVariable($Name, 'Process')
    if (-not [string]::IsNullOrWhiteSpace($fromProcess))
    {
        return $fromProcess.Trim()
    }
    $fromUser = [Environment]::GetEnvironmentVariable($Name, 'User')
    if (-not [string]::IsNullOrWhiteSpace($fromUser))
    {
        return $fromUser.Trim()
    }
    return ''
}

function Find-SignTool
{
    $kits = Join-Path ${env:ProgramFiles(x86)} 'Windows Kits\10\bin'
    if (-not (Test-Path -LiteralPath $kits))
    {
        return ''
    }
    $candidates = @()
    foreach ($dir in Get-ChildItem -LiteralPath $kits -Directory)
    {
        $exe = Join-Path $dir.FullName 'x64\signtool.exe'
        $version = $null
        if ((Test-Path -LiteralPath $exe) -and [Version]::TryParse($dir.Name, [ref]$version))
        {
            $candidates += [pscustomobject]@{ Path = $exe; Version = $version }
        }
    }
    if ($candidates.Count -eq 0)
    {
        return ''
    }
    return ($candidates | Sort-Object Version | Select-Object -Last 1).Path
}

function Get-PeSignatures([string]$Root)
{
    $files = Get-ChildItem -LiteralPath $Root -Recurse -File | Where-Object { $_.Extension -eq '.dll' -or $_.Extension -eq '.exe' }
    foreach ($file in $files)
    {
        $signature = Get-AuthenticodeSignature -LiteralPath $file.FullName
        $signer = ''
        if ($null -ne $signature.SignerCertificate)
        {
            $signer = $signature.SignerCertificate.GetNameInfo([System.Security.Cryptography.X509Certificates.X509NameType]::SimpleName, $false)
        }
        [pscustomobject]@{
            Path = $file.FullName
            Relative = $file.FullName.Substring($Root.Length).TrimStart('\')
            Status = $signature.Status.ToString()
            Signer = $signer
        }
    }
}

# Publish folder
if ([string]::IsNullOrWhiteSpace($PublishDir))
{
    $PublishDir = Join-Path $PSScriptRoot '..\..\..\bin\Publish\Steam-win-x64'
}
$PublishDir = [System.IO.Path]::GetFullPath($PublishDir).TrimEnd('\')
if (-not (Test-Path -LiteralPath $PublishDir -PathType Container))
{
    Stop-Script "Publish folder not found: $PublishDir. Publish the Steam-win-x64 profile first."
}
if (-not (Test-Path -LiteralPath (Join-Path $PublishDir 'GnollHackM.exe')))
{
    Stop-Script "No GnollHackM.exe in $PublishDir; this does not look like a GnollHack publish folder."
}
Write-Host "Publish folder: $PublishDir"

$before = @(Get-PeSignatures $PublishDir)
$damaged = @($before | Where-Object { $_.Status -ne 'Valid' -and $_.Status -ne 'NotSigned' })
if ($damaged.Count -gt 0)
{
    Write-Host 'These files carry a signature that is not valid; they are not signed over:' -ForegroundColor Red
    foreach ($item in $damaged)
    {
        Write-Host ("  {0}: {1}" -f $item.Status, $item.Relative)
    }
    Stop-Script 'Publish again into an empty folder, then run this script again.'
}
$unsigned = @($before | Where-Object { $_.Status -eq 'NotSigned' })
Write-Host ("PE files: {0}, already signed: {1}, unsigned: {2}" -f $before.Count, ($before.Count - $unsigned.Count), $unsigned.Count)

if (-not $VerifyOnly -and $unsigned.Count -gt 0)
{
    $metadata = Get-SigningSetting $ArtifactSigningMetadata 'GNOLLHACK_SIGN_METADATA'
    $dlib = Get-SigningSetting $ArtifactSigningDlib 'GNOLLHACK_SIGN_DLIB'
    $thumbprint = Get-SigningSetting $CertificateThumbprint 'GNOLLHACK_SIGN_THUMBPRINT'
    $timestamp = Get-SigningSetting $TimestampUrl 'GNOLLHACK_SIGN_TIMESTAMP'
    $useArtifactSigning = ($metadata -ne '') -or ($dlib -ne '')
    $useCertificate = $thumbprint -ne ''

    if ($useArtifactSigning -and $useCertificate)
    {
        Stop-Script 'Both Artifact Signing (GNOLLHACK_SIGN_METADATA, GNOLLHACK_SIGN_DLIB) and a certificate (GNOLLHACK_SIGN_THUMBPRINT) are configured. Configure one.'
    }
    if (-not $useArtifactSigning -and -not $useCertificate)
    {
        Stop-Script 'No signing method is configured. For Azure Artifact Signing set GNOLLHACK_SIGN_METADATA and GNOLLHACK_SIGN_DLIB; for a certificate set GNOLLHACK_SIGN_THUMBPRINT and GNOLLHACK_SIGN_TIMESTAMP.'
    }

    $methodArgs = @()
    if ($useArtifactSigning)
    {
        if ($metadata -eq '' -or -not (Test-Path -LiteralPath $metadata -PathType Leaf))
        {
            Stop-Script "Artifact Signing metadata.json not found: '$metadata' (GNOLLHACK_SIGN_METADATA)."
        }
        if ($dlib -eq '' -or -not (Test-Path -LiteralPath $dlib -PathType Leaf))
        {
            Stop-Script "Azure.CodeSigning.Dlib.dll not found: '$dlib' (GNOLLHACK_SIGN_DLIB)."
        }
        if ($timestamp -eq '')
        {
            $timestamp = $ArtifactSigningTimestampUrl
        }
        # The dlib signs in through the Azure CLI when az is on PATH; an Azure CLI installed after this window opened is not
        if ($null -eq (Get-Command az -ErrorAction SilentlyContinue))
        {
            $azBin = Join-Path $env:ProgramFiles 'Microsoft SDKs\Azure\CLI2\wbin'
            if (Test-Path -LiteralPath (Join-Path $azBin 'az.cmd'))
            {
                $env:Path = $azBin + ';' + $env:Path
            }
        }
        $methodArgs = @('/dlib', $dlib, '/dmdf', $metadata)
        Write-Host "Signing with Azure Artifact Signing ($metadata)"
    }
    else
    {
        if ($timestamp -eq '')
        {
            Stop-Script 'A certificate needs a timestamp server: set GNOLLHACK_SIGN_TIMESTAMP or pass -TimestampUrl.'
        }
        $methodArgs = @('/sha1', $thumbprint, '/s', 'My')
        Write-Host "Signing with certificate $thumbprint"
    }

    if ([string]::IsNullOrWhiteSpace($SignToolPath))
    {
        $SignToolPath = Find-SignTool
    }
    if ($SignToolPath -eq '' -or -not (Test-Path -LiteralPath $SignToolPath -PathType Leaf))
    {
        Stop-Script 'signtool.exe not found. Install the Windows SDK or pass -SignToolPath.'
    }
    Write-Host "SignTool: $SignToolPath"
    Write-Host "Timestamp server: $timestamp"

    $paths = @($unsigned | ForEach-Object { $_.Path })
    for ($start = 0; $start -lt $paths.Count; $start += $BatchSize)
    {
        $end = [Math]::Min($start + $BatchSize, $paths.Count) - 1
        $batch = @($paths[$start..$end])
        Write-Host ("Signing files {0}-{1} of {2}" -f ($start + 1), ($end + 1), $paths.Count)
        $signArgs = @('sign', '/fd', 'SHA256', '/tr', $timestamp, '/td', 'SHA256') + $methodArgs + $batch
        & $SignToolPath @signArgs
        if ($LASTEXITCODE -ne 0)
        {
            Stop-Script "signtool failed with exit code $LASTEXITCODE; the folder is only partly signed. Fix the cause and run this script again."
        }
    }
}

# Verification
$after = @(Get-PeSignatures $PublishDir)
Write-Host ''
Write-Host 'Signatures by signer:'
foreach ($group in ($after | Where-Object { $_.Status -eq 'Valid' } | Group-Object Signer | Sort-Object Count -Descending))
{
    Write-Host ("  {0,5}  {1}" -f $group.Count, $group.Name)
}
Write-Host 'Files by status:'
foreach ($group in ($after | Group-Object Status | Sort-Object Name))
{
    Write-Host ("  {0,5}  {1}" -f $group.Count, $group.Name)
}
$invalid = @($after | Where-Object { $_.Status -ne 'Valid' })
if ($invalid.Count -gt 0)
{
    Write-Host ''
    Write-Host ("{0} PE files have no valid signature:" -f $invalid.Count) -ForegroundColor Red
    foreach ($item in $invalid)
    {
        Write-Host ("  {0}: {1}" -f $item.Status, $item.Relative)
    }
    Write-Host 'Do not upload this folder.' -ForegroundColor Red
    exit 1
}
Write-Host ''
Write-Host ("All {0} PE files have a valid signature." -f $after.Count) -ForegroundColor Green
exit 0
