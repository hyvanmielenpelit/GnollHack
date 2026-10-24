<#
.SYNOPSIS
    Builds gnhsentry.dll, sentry-native's inproc crash handler with GnollHack's crash shim, for the Windows build.

.DESCRIPTION
    Compiles gnhsentry.c and links it with sentry-native.lib from the Sentry NuGet package that GnollHackM restores,
    using the hybrid CRT: the static vcruntime and the dynamic UCRT, so the DLL needs no VCRUNTIME140.dll. The
    package version is the GnhSentryVersion property in GnollHackM.csproj, which every Sentry and Sentry.Maui
    PackageReference must use as its Version. The MSVC tools are found with vswhere.exe, the Windows SDK is the newest
    one installed under Windows Kits\10, and INCLUDE, LIB and PATH are set only for the tools this script runs.

    gnhsentry.c includes include\sentry.h, sentry-native's own header, whose SENTRY_SDK_VERSION_MAJOR, _MINOR and
    _PATCH macros give the verified sentry-native version. When the sentry.native.dotnet/<version> string in
    sentry-native.lib names another version, the script stops before compiling; README.md describes the upgrade.

    Intermediates go to win\win32\vs\obj\Release\x64\gnhsentry\ and the PDB to
    win\win32\vs\symbols\Release\x64\gnhsentry\. The DLL is checked there: no VCRUNTIME, MSVCP or ucrtbased
    dependency, every export of gnhsentry.def plus the three gnh_crash_ functions, and the sentry-native version
    string equal to the header's version. Only then is it copied to GnollHackM\Platforms\Windows\libs\gnhsentry.dll,
    with sentry-native's license in GnollHackM\Platforms\Windows\licenses\sentry-native\.

    After the copy, GnollHackM\Platforms\Windows\libs\gnhsentry.build.props records the package and sentry-native
    versions, the SHA-256 of the linked sentry-native.lib, of the DLL, of the PDB and of the compiler and linker
    arguments, a line-ending-independent hash of gnhsentry.c, gnhsentry.def and include\sentry.h, the Sentry debug ID
    of the PDB, and the MSVC and Windows SDK versions. GnhSentryNative.targets compares them with the current inputs
    at build time.

    The script never downloads anything. GnollHackM must have been restored, so that the Sentry package is in the
    NuGet package folder.

.EXAMPLE
    .\Build-GnhSentry.ps1

    Builds, checks and copies gnhsentry.dll.
#>
[CmdletBinding()]
param()

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

# Runs a native tool and returns its standard output as an array of lines. The lines are echoed unless -Quiet is
# given. Standard error goes straight to the console. Throws when the exit code is not zero.
function Invoke-NativeTool
{
    param(
        [Parameter(Mandatory = $true)][string]$Path,
        [string[]]$Arguments,
        [switch]$Quiet
    )
    $output = @(& $Path @Arguments)
    $exitCode = $LASTEXITCODE
    if (-not $Quiet)
    {
        foreach ($line in $output)
        {
            Write-Host "    $line"
        }
    }
    if ($exitCode -ne 0)
    {
        if ($Quiet)
        {
            foreach ($line in $output)
            {
                Write-Host "    $line"
            }
        }
        throw "$(Split-Path -Leaf $Path) failed with exit code $exitCode."
    }
    return ,$output
}

function Assert-FileExists
{
    param([string]$Path, [string]$Message)
    if (-not (Test-Path -LiteralPath $Path -PathType Leaf))
    {
        throw $Message
    }
}

# The export names listed in a module-definition file: the first word of each line after EXPORTS.
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
    if ($names.Count -eq 0)
    {
        throw "No exports found in '$DefPath'."
    }
    return ,$names.ToArray()
}

# The sentry-native version a sentry.h declares, from its SENTRY_SDK_VERSION_MAJOR, _MINOR and _PATCH macros, each
# defined exactly once.
function Get-SentryHeaderVersion
{
    param([string]$HeaderFile)
    $headerText = [System.IO.File]::ReadAllText($HeaderFile)
    $parts = foreach ($part in @('MAJOR', 'MINOR', 'PATCH'))
    {
        $pattern = '(?m)^[ \t]*#[ \t]*define[ \t]+SENTRY_SDK_VERSION_' + $part + '[ \t]+(\d+)[ \t]*\r?$'
        $found = [regex]::Matches($headerText, $pattern)
        if ($found.Count -ne 1)
        {
            throw "Expected one '#define SENTRY_SDK_VERSION_$part <number>' in '$HeaderFile', found $($found.Count)."
        }
        $found[0].Groups[1].Value
    }
    return (@($parts) -join '.')
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

# ---------------------------------------------------------------------------------------------------
# Paths, all derived from the location of this script: win\win32\xpl\gnhsentrywin under the repository root
# ---------------------------------------------------------------------------------------------------

$ScriptDir = $PSScriptRoot
$RepositoryRoot = Split-Path -Parent (Split-Path -Parent (Split-Path -Parent (Split-Path -Parent $ScriptDir)))
$GnollHackMDir = Join-Path $RepositoryRoot 'win\win32\xpl\GnollHackM'
$ProjectPath = Join-Path $GnollHackMDir 'GnollHackM.csproj'

$SourcePath = Join-Path $ScriptDir 'gnhsentry.c'
$DefPath = Join-Path $ScriptDir 'gnhsentry.def'
$HeaderPath = Join-Path $ScriptDir 'include\sentry.h'

$ObjDir = Join-Path $RepositoryRoot 'win\win32\vs\obj\Release\x64\gnhsentry'
$SymbolsDir = Join-Path $RepositoryRoot 'win\win32\vs\symbols\Release\x64\gnhsentry'
$ObjPath = Join-Path $ObjDir 'gnhsentry.obj'
$CompilerPdbPath = Join-Path $ObjDir 'vc.pdb'
$BuiltDllPath = Join-Path $ObjDir 'gnhsentry.dll'
$PdbPath = Join-Path $SymbolsDir 'gnhsentry.pdb'

$WindowsLibsDir = Join-Path $GnollHackMDir 'Platforms\Windows\libs'
$WindowsLicenseDir = Join-Path $GnollHackMDir 'Platforms\Windows\licenses\sentry-native'
$LinuxLicensePath = Join-Path $GnollHackMDir 'Platforms\Linux\licenses\sentry-native\LICENSE'
$TargetDllPath = Join-Path $WindowsLibsDir 'gnhsentry.dll'
$TargetLicensePath = Join-Path $WindowsLicenseDir 'LICENSE'
$ManifestPath = Join-Path $WindowsLibsDir 'gnhsentry.build.props'

Assert-FileExists $ProjectPath ("GnollHackM.csproj not found at '$ProjectPath'. " +
    'The script must stay in win\win32\xpl\gnhsentrywin.')
Assert-FileExists $SourcePath "gnhsentry.c not found at '$SourcePath'."
Assert-FileExists $DefPath "gnhsentry.def not found at '$DefPath'."
Assert-FileExists $HeaderPath "sentry-native's header not found at '$HeaderPath'."
Assert-FileExists $LinuxLicensePath "sentry-native's license not found at '$LinuxLicensePath'."

# ---------------------------------------------------------------------------------------------------
# 1. The versions, and sentry-native.lib from the restored Sentry package
# ---------------------------------------------------------------------------------------------------

Write-Step 'Finding sentry-native.lib'

$project = New-Object System.Xml.XmlDocument
$project.Load($ProjectPath)
$versionNodes = @($project.SelectNodes('//PropertyGroup/GnhSentryVersion'))
if ($versionNodes.Count -ne 1 -or [string]::IsNullOrWhiteSpace($versionNodes[0].InnerText))
{
    throw "Expected one non-empty GnhSentryVersion property in '$ProjectPath', found $($versionNodes.Count)."
}
$SentryPackageVersion = $versionNodes[0].InnerText.Trim()
$sentryReferences = @($project.SelectNodes("//ItemGroup/PackageReference[@Include='Sentry.Maui' or @Include='Sentry']"))
if ($sentryReferences.Count -eq 0)
{
    throw "'$ProjectPath' has no PackageReference to Sentry or Sentry.Maui."
}
foreach ($reference in $sentryReferences)
{
    $referenceVersion = $reference.GetAttribute('Version')
    if ($referenceVersion -ne '$(GnhSentryVersion)')
    {
        throw ("The PackageReference to $($reference.GetAttribute('Include')) in '$ProjectPath' has " +
            "Version=`"$referenceVersion`"; it must be " + 'Version="$(GnhSentryVersion)".')
    }
}
Write-Host "Sentry $SentryPackageVersion (GnhSentryVersion)"

$SentryNativeVersion = Get-SentryHeaderVersion $HeaderPath
Write-Host "sentry-native $SentryNativeVersion (include\sentry.h)"

if (-not [string]::IsNullOrWhiteSpace($env:NUGET_PACKAGES))
{
    $NuGetRoot = $env:NUGET_PACKAGES
}
else
{
    $NuGetRoot = Join-Path $env:USERPROFILE '.nuget\packages'
}
$SentryNativeLib = Join-Path $NuGetRoot "sentry\$SentryPackageVersion\sentry-native\win-x64\sentry-native.lib"
Assert-FileExists $SentryNativeLib "'$SentryNativeLib' is missing: restore GnollHackM first."
Write-Host $SentryNativeLib

$libText = [System.Text.Encoding]::ASCII.GetString([System.IO.File]::ReadAllBytes($SentryNativeLib))
$libVersions = @([regex]::Matches($libText, 'sentry\.native\.dotnet/(\d+\.\d+\.\d+)') |
    ForEach-Object { $_.Groups[1].Value } |
    Sort-Object -Unique)
if ($libVersions.Count -ne 1)
{
    throw ("Expected one sentry.native.dotnet/<version> string in '$SentryNativeLib', found: " +
        "[$($libVersions -join ', ')].")
}
$libVersion = $libVersions[0]
if ($libVersion -ne $SentryNativeVersion)
{
    throw ("sentry-native.lib in Sentry $SentryPackageVersion is sentry-native $libVersion, but include\sentry.h is " +
        "$SentryNativeVersion. Get sentry.h at tag $libVersion and run Compare-SentryNativeApi.ps1 " +
        '-NewHeader <path>; see README.md.')
}
$SentryNativeLibSha256 = Get-FileSha256 $SentryNativeLib
Write-Host "    sentry.native.dotnet/$libVersion"

# ---------------------------------------------------------------------------------------------------
# 2. MSVC and the Windows SDK
# ---------------------------------------------------------------------------------------------------

Write-Step 'Finding MSVC and the Windows SDK'

$vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
Assert-FileExists $vswhere "vswhere.exe not found at '$vswhere'. Install Visual Studio with the C++ desktop workload."

# -requires keeps installations without the C++ tools out, such as SQL Server Management Studio
$vswhereOutput = Invoke-NativeTool -Path $vswhere -Quiet -Arguments @(
    '-latest', '-prerelease', '-products', '*',
    '-requires', 'Microsoft.VisualStudio.Component.VC.Tools.x86.x64',
    '-property', 'installationPath')
$VsDir = @($vswhereOutput | Where-Object { -not [string]::IsNullOrWhiteSpace($_) }) | Select-Object -First 1
if ($null -eq $VsDir)
{
    throw 'vswhere.exe found no Visual Studio installation with the MSVC x64 tools.'
}
$VsDir = $VsDir.Trim()

$msvcVersionFile = Join-Path $VsDir 'VC\Auxiliary\Build\Microsoft.VCToolsVersion.default.txt'
Assert-FileExists $msvcVersionFile "'$msvcVersionFile' is missing."
$MsvcVersion = ([System.IO.File]::ReadAllText($msvcVersionFile)).Trim()
$MsvcDir = Join-Path $VsDir "VC\Tools\MSVC\$MsvcVersion"
$ToolsDir = Join-Path $MsvcDir 'bin\Hostx64\x64'
$Cl = Join-Path $ToolsDir 'cl.exe'
$Link = Join-Path $ToolsDir 'link.exe'
$Dumpbin = Join-Path $ToolsDir 'dumpbin.exe'
foreach ($tool in @($Cl, $Link, $Dumpbin))
{
    Assert-FileExists $tool "'$tool' is missing."
}
Write-Host "Visual Studio: $VsDir"
Write-Host "MSVC:          $MsvcVersion"

$KitsDir = Join-Path ${env:ProgramFiles(x86)} 'Windows Kits\10'
$KitsIncludeDir = Join-Path $KitsDir 'Include'
$KitsLibDir = Join-Path $KitsDir 'Lib'
if (-not (Test-Path -LiteralPath $KitsIncludeDir -PathType Container))
{
    throw "The Windows SDK is not installed: '$KitsIncludeDir' is missing."
}
$SdkVersion = Get-ChildItem -LiteralPath $KitsIncludeDir -Directory |
    Where-Object { $_.Name -match '^\d+\.\d+\.\d+\.\d+$' } |
    Where-Object { Test-Path -LiteralPath (Join-Path $KitsLibDir "$($_.Name)\um\x64") -PathType Container } |
    Sort-Object { [version]$_.Name } -Descending |
    Select-Object -First 1 -ExpandProperty Name
if ($null -eq $SdkVersion)
{
    throw "No Windows SDK under '$KitsDir' has both headers and x64 libraries."
}
Write-Host "Windows SDK:   $SdkVersion"

$BuildInclude = (@(
    (Join-Path $MsvcDir 'include'),
    (Join-Path $KitsIncludeDir "$SdkVersion\ucrt"),
    (Join-Path $KitsIncludeDir "$SdkVersion\um"),
    (Join-Path $KitsIncludeDir "$SdkVersion\shared")) -join ';')
$BuildLib = (@(
    (Join-Path $MsvcDir 'lib\x64'),
    (Join-Path $KitsLibDir "$SdkVersion\ucrt\x64"),
    (Join-Path $KitsLibDir "$SdkVersion\um\x64")) -join ';')

$previousInclude = $env:INCLUDE
$previousLib = $env:LIB
$previousPath = $env:PATH
try
{
    $env:INCLUDE = $BuildInclude
    $env:LIB = $BuildLib
    $env:PATH = "$ToolsDir;$previousPath"

    # ---------------------------------------------------------------------------------------------------
    # 3. Compile and link
    # ---------------------------------------------------------------------------------------------------

    Write-Step 'Compiling gnhsentry.c'

    foreach ($dir in @($ObjDir, $SymbolsDir))
    {
        if (-not (Test-Path -LiteralPath $dir -PathType Container))
        {
            New-Item -ItemType Directory -Path $dir | Out-Null
        }
    }
    foreach ($name in @('gnhsentry.obj', 'gnhsentry.dll', 'gnhsentry.lib', 'gnhsentry.exp'))
    {
        $stale = Join-Path $ObjDir $name
        if (Test-Path -LiteralPath $stale -PathType Leaf)
        {
            Remove-Item -LiteralPath $stale -Force
        }
    }

    # /MT here plus /NODEFAULTLIB:libucrt.lib and ucrt.lib at link time is the hybrid CRT: static vcruntime,
    # dynamic UCRT. /utf-8 reads include\sentry.h, which has a non-ASCII character in a comment, the same way on
    # every code page.
    $ClArguments = @(
        '/nologo', '/c', '/O2', '/MT', '/W4', '/Zi', '/guard:cf', '/utf-8',
        "/Fo$ObjPath", "/Fd$CompilerPdbPath", $SourcePath)
    Invoke-NativeTool -Path $Cl -Arguments $ClArguments | Out-Null

    Write-Step 'Linking gnhsentry.dll'

    # sentry-native.lib is compiled /MD: its __imp_ references to vcruntime functions resolve against the static
    # libvcruntime.lib, which link reports as LNK4217 or LNK4286. Those warnings are expected.
    $LinkArguments = @(
        '/NOLOGO', '/DLL', '/MACHINE:X64', "/DEF:$DefPath", "/OUT:$BuiltDllPath",
        '/DEBUG', "/PDB:$PdbPath", '/GUARD:CF', '/NODEFAULTLIB:MSVCRT', '/NODEFAULTLIB:libucrt.lib',
        $ObjPath, $SentryNativeLib,
        'libcmt.lib', 'libvcruntime.lib', 'ucrt.lib', 'dbghelp.lib', 'winhttp.lib', 'gdi32.lib',
        'synchronization.lib', 'kernel32.lib', 'user32.lib', 'advapi32.lib', 'shlwapi.lib', 'version.lib',
        'ole32.lib')
    $linkOutput = Invoke-NativeTool -Path $Link -Arguments $LinkArguments
    $unresolved = @($linkOutput | Where-Object { $_ -match '\bLNK(2001|2019)\b' })
    if ($unresolved.Count -gt 0)
    {
        throw "link reported $($unresolved.Count) unresolved external symbol(s)."
    }
    Assert-FileExists $BuiltDllPath "link did not create '$BuiltDllPath'."

    # ---------------------------------------------------------------------------------------------------
    # 4. Checks on the built DLL, before it is copied anywhere
    # ---------------------------------------------------------------------------------------------------

    Write-Step 'Checking dependencies'

    $dependentsOutput = Invoke-NativeTool -Path $Dumpbin -Quiet -Arguments @('/nologo', '/dependents', $BuiltDllPath)
    $dependencies = @($dependentsOutput |
        Where-Object { $_ -match '^\s+(\S+\.dll)\s*$' } |
        ForEach-Object { $_.Trim() })
    if ($dependencies.Count -eq 0)
    {
        throw "dumpbin /dependents lists no DLLs for '$BuiltDllPath'."
    }
    foreach ($dependency in $dependencies)
    {
        Write-Host "    $dependency"
    }
    $forbidden = @($dependencies | Where-Object { $_ -match '^(VCRUNTIME|MSVCP|ucrtbased)' })
    if ($forbidden.Count -gt 0)
    {
        throw "gnhsentry.dll depends on $($forbidden -join ', '); the hybrid CRT link did not take effect."
    }

    Write-Step 'Checking exports'

    $exportsOutput = Invoke-NativeTool -Path $Dumpbin -Quiet -Arguments @('/nologo', '/exports', $BuiltDllPath)
    $exported = @{}
    foreach ($line in $exportsOutput)
    {
        if ($line -match '^\s+\d+\s+[0-9A-Fa-f]+\s+[0-9A-Fa-f]{8}\s+(\S+)')
        {
            $exported[$Matches[1]] = $true
        }
    }
    $defExports = Get-DefExportNames $DefPath
    $expectedExports = @($defExports) + @('gnh_crash_prepare', 'gnh_crash_install', 'gnh_crash_uninstall')
    $missingExports = @($expectedExports | Where-Object { -not $exported.ContainsKey($_) })
    if ($missingExports.Count -gt 0)
    {
        throw "gnhsentry.dll does not export: $($missingExports -join ', ')."
    }
    Write-Host "    $($expectedExports.Count) expected exports present ($($exported.Count) exported in all)"

    Write-Step 'Checking the sentry-native version'

    $dllText = [System.Text.Encoding]::ASCII.GetString([System.IO.File]::ReadAllBytes($BuiltDllPath))
    $foundVersions = @([regex]::Matches($dllText, 'sentry\.native\.dotnet/(\d+\.\d+\.\d+)') |
        ForEach-Object { $_.Groups[1].Value } |
        Sort-Object -Unique)
    if ($foundVersions.Count -eq 0)
    {
        throw "gnhsentry.dll contains no sentry.native.dotnet/<version> string."
    }
    $otherVersions = @($foundVersions | Where-Object { $_ -ne $SentryNativeVersion })
    if ($otherVersions.Count -gt 0)
    {
        throw ("gnhsentry.dll contains sentry.native.dotnet/$($otherVersions -join ', '), but include\sentry.h is " +
            "$SentryNativeVersion. Get sentry.h at tag $($otherVersions[0]) and run Compare-SentryNativeApi.ps1 " +
            '-NewHeader <path>; see README.md.')
    }
    Write-Host "    sentry.native.dotnet/$SentryNativeVersion"

    Write-Step 'Reading the debug ID'

    # The CodeView record of the debug directory: Format: RSDS, {<GUID>}, <age>, <PDB path>
    $headersOutput = Invoke-NativeTool -Path $Dumpbin -Quiet -Arguments @('/nologo', '/headers', $BuiltDllPath)
    $codeViewRecords = @($headersOutput |
        ForEach-Object { [regex]::Match($_, 'Format: RSDS, \{([0-9A-Fa-f-]{36})\}, (\d+),') } |
        Where-Object { $_.Success })
    if ($codeViewRecords.Count -ne 1)
    {
        throw ("dumpbin /headers shows $($codeViewRecords.Count) RSDS CodeView records for '$BuiltDllPath', " +
            'expected one.')
    }
    $debugGuid = $codeViewRecords[0].Groups[1].Value.ToLowerInvariant()
    $debugAge = [System.UInt32]::Parse($codeViewRecords[0].Groups[2].Value)
    $DebugId = "$debugGuid-$($debugAge.ToString('x'))"
    Write-Host "    $DebugId"
}
finally
{
    $env:INCLUDE = $previousInclude
    $env:LIB = $previousLib
    $env:PATH = $previousPath
}

# ---------------------------------------------------------------------------------------------------
# 5. Copy the checked DLL and the license
# ---------------------------------------------------------------------------------------------------

Write-Step 'Copying gnhsentry.dll and its license'

foreach ($dir in @($WindowsLibsDir, $WindowsLicenseDir))
{
    if (-not (Test-Path -LiteralPath $dir -PathType Container))
    {
        New-Item -ItemType Directory -Path $dir | Out-Null
    }
}
Copy-Item -LiteralPath $BuiltDllPath -Destination $TargetDllPath -Force
Copy-Item -LiteralPath $LinuxLicensePath -Destination $TargetLicensePath -Force
Write-Host "Copied gnhsentry.dll to $WindowsLibsDir and the license to $WindowsLicenseDir"

# ---------------------------------------------------------------------------------------------------
# 6. The manifest that GnhSentryNative.targets checks
# ---------------------------------------------------------------------------------------------------

Write-Step 'Writing gnhsentry.build.props'

$utf8NoBom = New-Object System.Text.UTF8Encoding($false)
$buildArguments = (@($ClArguments) -join ' ') + "`n" + (@($LinkArguments) -join ' ')
$manifestProperties = [ordered]@{
    GnhSentryDllPackageVersion = $SentryPackageVersion
    GnhSentryDllNativeVersion = $SentryNativeVersion
    GnhSentryDllLibSha256 = $SentryNativeLibSha256
    GnhSentryDllSourceSha256 = Get-NormalizedTextHash @($SourcePath, $DefPath, $HeaderPath)
    GnhSentryDllSha256 = Get-FileSha256 $TargetDllPath
    GnhSentryDllPdbSha256 = Get-FileSha256 $PdbPath
    GnhSentryDllDebugId = $DebugId
    GnhSentryDllToolset = "MSVC $MsvcVersion, Windows SDK $SdkVersion"
    GnhSentryDllBuildArgsSha256 = Get-Sha256Hex -Bytes ($utf8NoBom.GetBytes($buildArguments))
}
$manifestLines = New-Object System.Collections.Generic.List[string]
$manifestLines.Add('<!-- Written by Build-GnhSentry.ps1; checked by GnhSentryNative.targets. Do not edit. -->')
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
[System.IO.File]::WriteAllText($ManifestPath, (($manifestLines -join "`r`n") + "`r`n"), $utf8NoBom)
Write-Host "Wrote $ManifestPath"

# ---------------------------------------------------------------------------------------------------
# 7. Summary
# ---------------------------------------------------------------------------------------------------

Write-Step "gnhsentry.dll from Sentry $SentryPackageVersion (sentry-native $SentryNativeVersion)"

$item = Get-Item -LiteralPath $TargetDllPath
$hash = (Get-FileHash -LiteralPath $TargetDllPath -Algorithm SHA256).Hash.ToLowerInvariant()
Write-Host $TargetDllPath
Write-Host "    size      $($item.Length) bytes"
Write-Host "    sha256    $hash"
Write-Host "    debug ID  $DebugId"
Write-Host $PdbPath
Write-Host $TargetLicensePath
Write-Host $ManifestPath
