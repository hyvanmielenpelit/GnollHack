<#
.SYNOPSIS
    Compares the part of sentry-native's API that GnollHack uses between two sentry.h files, and optionally updates
    the vendored header.

.DESCRIPTION
    The used API is every function exported by gnhsentry.def, every sentry_ function declared extern in
    GnollHackM\GHSentryNative.cs and GnollHackM\Platforms\Avalonia\GHAvaloniaSentryNative.cs, sentry_handle_exception
    (called by gnhsentry.c, not exported), and the types sentry_value_t, sentry_ucontext_t,
    sentry_handler_strategy_t, sentry_options_t, sentry_transport_t and sentry_envelope_t. gnhsentry.c itself is not
    scanned: the compiler checks it against include\sentry.h.

    Each header is reduced to its declarations: comments, preprocessor lines, extern "C" openings and the contents
    of string literals are removed, whitespace is collapsed, and the text is split into statements at each ';' outside
    braces. A function's declaration is every statement that is not a typedef and contains the name followed by '('.
    A type's declaration is every typedef ending in the type's name, plus, for a typedef of the form
    'typedef struct|union|enum <tag> <type>;', every statement that starts with that tag's body. Several matches,
    such as platform variants, are joined with ' || '. Line endings, comments and formatting therefore do not count
    as changes.

    The versions of both headers are printed from SENTRY_SDK_VERSION, or as "unknown" when a header does not define
    it. Each used name whose declaration differs is reported as CHANGED, and each name the new header lacks as
    REMOVED. A name the old header lacks means the symbol list or the parser is wrong; it is reported, and the
    script exits with code 2.

    Exit codes: 0 when the used API is unchanged, 1 when it changed, 2 on any error.

    With -Update, the new header is copied over include\sentry.h beside this script, with CRLF line endings and no
    BOM, when the used API is unchanged, or when it changed and -AcceptChanges is given. README.md describes the
    whole upgrade.

.PARAMETER NewHeader
    The sentry.h of the sentry-native version to compare with, for example the one in the WSL checkout.

.PARAMETER OldHeader
    The sentry.h to compare against. Default: include\sentry.h beside this script, the vendored header.

.PARAMETER Update
    Copies -NewHeader over the vendored header when the used API is unchanged. -OldHeader must then be the vendored
    header.

.PARAMETER AcceptChanges
    Lets -Update copy the header although the used API changed. gnhsentry.c, GHSentryNative.cs and
    GHAvaloniaSentryNative.cs must already match the new declarations.

.EXAMPLE
    .\Compare-SentryNativeApi.ps1 -NewHeader \\wsl.localhost\Ubuntu-20.04\root\.cache\gnollhack-sentry-native\sentry-native\include\sentry.h

    Reports the differences in the used API between the vendored header and the WSL checkout's header.

.EXAMPLE
    .\Compare-SentryNativeApi.ps1 -NewHeader C:\Temp\sentry.h -Update

    Replaces the vendored header with C:\Temp\sentry.h if the used API is unchanged.
#>
[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)][string]$NewHeader,

    [string]$OldHeader,

    [switch]$Update,

    [switch]$AcceptChanges
)

Set-StrictMode -Version 2
$ErrorActionPreference = 'Stop'

# Windows PowerShell 5.1 leaves $PSScriptRoot empty in parameter defaults
if ([string]::IsNullOrEmpty($OldHeader))
{
    $OldHeader = Join-Path $PSScriptRoot 'include\sentry.h'
}

# The types of the used API, by their typedef names
$UsedTypeNames = @(
    'sentry_value_t', 'sentry_ucontext_t', 'sentry_handler_strategy_t', 'sentry_options_t', 'sentry_transport_t',
    'sentry_envelope_t')

# The one function gnhsentry.c calls that gnhsentry.def does not export
$UnexportedFunctionNames = @('sentry_handle_exception')

# ---------------------------------------------------------------------------------------------------
# Helper functions
# ---------------------------------------------------------------------------------------------------

function Write-Step
{
    param([string]$Message)
    Write-Host ''
    Write-Host "==> $Message" -ForegroundColor Cyan
}

function Assert-FileExists
{
    param([string]$Path, [string]$Message)
    if (-not (Test-Path -LiteralPath $Path -PathType Leaf))
    {
        throw $Message
    }
}

# The sentry_ export names listed in a module-definition file: the first word of each line after EXPORTS.
function Get-DefSentryExportNames
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
            $exportName = ($line -split '[\s=@]')[0]
            if ($exportName -cmatch '^sentry_\w+$')
            {
                $names.Add($exportName)
            }
        }
    }
    if ($names.Count -eq 0)
    {
        throw "No sentry_ exports found in '$DefPath'."
    }
    return ,$names.ToArray()
}

# The sentry_ functions a C# file declares as extern
function Get-CSharpExternNames
{
    param([string]$CSharpPath)
    $text = [System.IO.File]::ReadAllText($CSharpPath)
    $names = @([regex]::Matches($text, '\bextern\b[^;(]*?\b(sentry_\w+)\s*\(') | ForEach-Object { $_.Groups[1].Value })
    if ($names.Count -eq 0)
    {
        throw "No extern sentry_ functions found in '$CSharpPath'."
    }
    return ,$names
}

# The version a header names in SENTRY_SDK_VERSION, or "unknown"
function Get-HeaderVersionText
{
    param([string]$HeaderFile)
    $headerText = [System.IO.File]::ReadAllText($HeaderFile)
    $found = [regex]::Match($headerText, '(?m)^[ \t]*#[ \t]*define[ \t]+SENTRY_SDK_VERSION[ \t]+"([^"\r\n]*)"')
    if ($found.Success)
    {
        return $found.Groups[1].Value
    }
    return 'unknown'
}

# A header's text reduced to statements: no comments, preprocessor lines, extern "C" openings, string contents or
# API macros, whitespace collapsed, split at each ';' outside braces
function Get-HeaderStatements
{
    param([string]$HeaderFile)
    $text = [System.IO.File]::ReadAllText($HeaderFile).Replace("`r`n", "`n")
    $text = [regex]::Replace($text, '/\*.*?\*/', ' ', 'Singleline')
    $text = [regex]::Replace($text, '//[^\n]*', ' ')
    # Preprocessor lines without a continuation first, then the continued ones once joined
    $text = [regex]::Replace($text, '(?m)^\s*#.*?(?<!\\)$', ' ')
    $text = [regex]::Replace($text, '\\\r?\n', ' ')
    $text = [regex]::Replace($text, '(?m)^\s*#.*$', ' ')
    # Before the string literals, which would turn "C" into ""
    $text = [regex]::Replace($text, 'extern "C"\s*\{', ' ')
    $text = [regex]::Replace($text, '"(?:[^"\\\r\n]|\\.)*"', '""')
    $text = [regex]::Replace($text, '\s+', ' ')
    $text = [regex]::Replace($text, '\s*([(),*;{}\[\]])\s*', '$1')
    $text = [regex]::Replace($text, '\bSENTRY_EXPERIMENTAL_API\b', '')
    $text = [regex]::Replace($text, '\bSENTRY_API\b', '')

    $statements = New-Object System.Collections.Generic.List[string]
    $delimiters = [char[]]@('{', '}', ';')
    $depth = 0
    $start = 0
    $position = $text.IndexOfAny($delimiters, 0)
    while ($position -ge 0)
    {
        $delimiter = $text[$position]
        if ($delimiter -eq '{')
        {
            $depth++
        }
        elseif ($delimiter -eq '}')
        {
            if ($depth -gt 0)
            {
                $depth--
            }
        }
        elseif ($depth -eq 0)
        {
            $statement = $text.Substring($start, $position + 1 - $start).Trim()
            if ($statement.Length -gt 0)
            {
                $statements.Add($statement)
            }
            $start = $position + 1
        }
        $position = $text.IndexOfAny($delimiters, $position + 1)
    }
    return ,$statements.ToArray()
}

# The declarations of the given functions and types in a header, as a hashtable from name to declaration text.
# Names without a declaration are absent.
function Get-HeaderDeclarations
{
    param([string]$HeaderFile, [string[]]$FunctionNames, [string[]]$TypeNames)
    $statements = Get-HeaderStatements $HeaderFile
    $typedefStatements = @($statements | Where-Object { $_ -cmatch '^typedef\b' })
    $otherStatements = @($statements | Where-Object { $_ -cnotmatch '^typedef\b' })
    $declarations = @{}

    foreach ($functionName in $FunctionNames)
    {
        $functionPattern = '\b' + [regex]::Escape($functionName) + '\('
        $parts = New-Object System.Collections.Generic.List[string]
        foreach ($statement in $otherStatements)
        {
            if ($statement -cmatch $functionPattern)
            {
                $parts.Add($statement)
            }
        }
        if ($parts.Count -gt 0)
        {
            $declarations[$functionName] = $parts -join ' || '
        }
    }

    foreach ($typeName in $TypeNames)
    {
        $typedefPattern = '\b' + [regex]::Escape($typeName) + ';$'
        $bodilessPattern = '^typedef (struct|union|enum) (\w+) ' + [regex]::Escape($typeName) + ';$'
        $typedefs = New-Object System.Collections.Generic.List[string]
        foreach ($statement in $typedefStatements)
        {
            if ($statement -cmatch $typedefPattern)
            {
                $typedefs.Add($statement)
            }
        }
        if ($typedefs.Count -eq 0)
        {
            continue
        }
        $parts = New-Object System.Collections.Generic.List[string]
        $parts.AddRange($typedefs)
        foreach ($typedef in $typedefs)
        {
            $bodiless = [regex]::Match($typedef, $bodilessPattern)
            if ($bodiless.Success)
            {
                # The separate body of the tag, such as union sentry_value_u{...};
                $bodyPattern = '^' + $bodiless.Groups[1].Value + ' ' + [regex]::Escape($bodiless.Groups[2].Value) +
                    '\{'
                foreach ($statement in $otherStatements)
                {
                    if ($statement -cmatch $bodyPattern)
                    {
                        $parts.Add($statement)
                    }
                }
            }
        }
        $declarations[$typeName] = $parts -join ' || '
    }

    return $declarations
}

# ---------------------------------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------------------------------

$exitCode = 2
try
{
    $ScriptDir = $PSScriptRoot
    $GnollHackMDir = Join-Path (Split-Path -Parent $ScriptDir) 'GnollHackM'
    $DefPath = Join-Path $ScriptDir 'gnhsentry.def'
    $VendoredHeaderPath = Join-Path $ScriptDir 'include\sentry.h'
    $CSharpPaths = @(
        (Join-Path $GnollHackMDir 'GHSentryNative.cs'),
        (Join-Path $GnollHackMDir 'Platforms\Avalonia\GHAvaloniaSentryNative.cs'))

    $newHeaderPath = $ExecutionContext.SessionState.Path.GetUnresolvedProviderPathFromPSPath($NewHeader)
    $oldHeaderPath = $ExecutionContext.SessionState.Path.GetUnresolvedProviderPathFromPSPath($OldHeader)
    Assert-FileExists $newHeaderPath "The new header '$newHeaderPath' does not exist."
    Assert-FileExists $oldHeaderPath "The old header '$oldHeaderPath' does not exist."
    Assert-FileExists $DefPath "gnhsentry.def not found at '$DefPath'."
    foreach ($csharpPath in $CSharpPaths)
    {
        Assert-FileExists $csharpPath "'$csharpPath' does not exist."
    }
    $comparesVendoredHeader = ([System.IO.Path]::GetFullPath($oldHeaderPath) -eq
        [System.IO.Path]::GetFullPath($VendoredHeaderPath))
    if ($Update -and -not $comparesVendoredHeader)
    {
        throw "-Update replaces '$VendoredHeaderPath', so -OldHeader must be that file, not '$oldHeaderPath'."
    }

    # -------------------------------------------------------------------------------------------------
    # The used names
    # -------------------------------------------------------------------------------------------------

    $usedFunctionSet = New-Object System.Collections.Generic.SortedSet[string] -ArgumentList (
        [System.StringComparer]::Ordinal)
    foreach ($name in (Get-DefSentryExportNames $DefPath))
    {
        [void]$usedFunctionSet.Add($name)
    }
    foreach ($csharpPath in $CSharpPaths)
    {
        foreach ($name in (Get-CSharpExternNames $csharpPath))
        {
            [void]$usedFunctionSet.Add($name)
        }
    }
    foreach ($name in $UnexportedFunctionNames)
    {
        [void]$usedFunctionSet.Add($name)
    }
    $usedFunctionNames = @($usedFunctionSet)
    $sortedTypeNames = @($UsedTypeNames | Sort-Object -Unique)

    # -------------------------------------------------------------------------------------------------
    # Comparison
    # -------------------------------------------------------------------------------------------------

    Write-Step 'Headers'
    Write-Host "Old: sentry-native $(Get-HeaderVersionText $oldHeaderPath)  $oldHeaderPath"
    Write-Host "New: sentry-native $(Get-HeaderVersionText $newHeaderPath)  $newHeaderPath"
    Write-Host "Used API: $($usedFunctionNames.Count) functions, $($sortedTypeNames.Count) types"

    $oldDeclarations = Get-HeaderDeclarations $oldHeaderPath $usedFunctionNames $sortedTypeNames
    $newDeclarations = Get-HeaderDeclarations $newHeaderPath $usedFunctionNames $sortedTypeNames

    Write-Step 'Differences in the used API'

    $missingNames = New-Object System.Collections.Generic.List[string]
    $changedCount = 0
    foreach ($name in (@($usedFunctionNames) + @($sortedTypeNames)))
    {
        if (-not $oldDeclarations.ContainsKey($name))
        {
            $missingNames.Add($name)
            Write-Host "MISSING  $name is not declared in the old header" -ForegroundColor Red
            continue
        }
        if (-not $newDeclarations.ContainsKey($name))
        {
            $changedCount++
            Write-Host "REMOVED  $name" -ForegroundColor Yellow
            Write-Host "    old: $($oldDeclarations[$name])"
            continue
        }
        if ($oldDeclarations[$name] -cne $newDeclarations[$name])
        {
            $changedCount++
            Write-Host "CHANGED  $name" -ForegroundColor Yellow
            Write-Host "    old: $($oldDeclarations[$name])"
            Write-Host "    new: $($newDeclarations[$name])"
        }
    }

    if ($missingNames.Count -gt 0)
    {
        throw ("$($missingNames.Count) used name(s) not found in the old header: $($missingNames -join ', '). " +
            'The symbol list or the header parser of this script is wrong.')
    }

    if ($changedCount -eq 0)
    {
        Write-Host 'No change in the used API.'
        $exitCode = 0
    }
    else
    {
        Write-Host "$changedCount of $($usedFunctionNames.Count + $sortedTypeNames.Count) used names changed."
        $exitCode = 1
    }

    # -------------------------------------------------------------------------------------------------
    # Update
    # -------------------------------------------------------------------------------------------------

    if ($Update)
    {
        Write-Step 'Updating include\sentry.h'
        if ($exitCode -eq 1 -and -not $AcceptChanges)
        {
            Write-Host ('The used API changed, so the vendored header was not updated. Update gnhsentry.c, ' +
                'GHSentryNative.cs and GHAvaloniaSentryNative.cs to the new declarations, then rerun with ' +
                '-Update -AcceptChanges.') -ForegroundColor Yellow
        }
        else
        {
            if ($exitCode -eq 1)
            {
                Write-Host ('-AcceptChanges: gnhsentry.c, GHSentryNative.cs and GHAvaloniaSentryNative.cs must ' +
                    'already match the new declarations.') -ForegroundColor Yellow
            }
            $headerText = [System.IO.File]::ReadAllText($newHeaderPath).Replace("`r`n", "`n").Replace("`n", "`r`n")
            $utf8NoBom = New-Object System.Text.UTF8Encoding($false)
            [System.IO.File]::WriteAllText($VendoredHeaderPath, $headerText, $utf8NoBom)
            Write-Host "Copied $newHeaderPath to $VendoredHeaderPath"
        }
    }
    elseif ($AcceptChanges)
    {
        Write-Host '-AcceptChanges has no effect without -Update.'
    }
}
catch
{
    Write-Host "ERROR: $($_.Exception.Message)" -ForegroundColor Red
    $exitCode = 2
}

exit $exitCode
