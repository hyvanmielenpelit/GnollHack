<#
.SYNOPSIS
Checks the workaround and known-issue register in DEVEL/workarounds against its schema and
the code tags.

.DESCRIPTION
Parses every component file in DEVEL/workarounds (all *.md except README.md) and checks
each entry against the schema in README.md: ID format and uniqueness, required fields,
vocabularies, dates, the Checks list, and the required prose paragraphs. Workarounds
(WA-...) come before the <!-- known-issues --> marker line and known issues (KI-...) after
it; each kind has its own field set and vocabularies.

Unless -RegisterOnly is given, it also finds every WORKAROUND(WA-...) tag in the
repository with git grep and checks the two directions: every tag names an entry, and
every active or removable entry has a tag at each of its Code sites. Removed and
superseded entries must have no tags left. Known issues have no tags, and a
WORKAROUND(KI-...) tag is an error.

Warnings do not fail the run. They cover stale entries, field order, ID order, a leftover
"No entries." line, dangling Related IDs, and NuGet version drift: an entry whose fix is
merged or released upstream while GnollHackM.csproj references a version that the newest
check did not cover.

-CheckUpstream asks GitHub, through the gh CLI, for the state of each linked issue or pull
request and warns where it contradicts the entry. It needs the network and is not meant
for CI.

Exit codes: 0 no errors, 1 errors found, 2 the script could not run.

.EXAMPLE
.\DEVEL\workarounds\Test-WorkaroundRegister.ps1

.EXAMPLE
.\DEVEL\workarounds\Test-WorkaroundRegister.ps1 -RegisterOnly -CheckUpstream
#>
[CmdletBinding()]
param(
    [string] $RepositoryRoot,
    [switch] $RegisterOnly,
    [switch] $CheckUpstream,
    [int] $StaleDays = 180
)

$ErrorActionPreference = 'Stop'

if (-not $RepositoryRoot) { $RepositoryRoot = Join-Path $PSScriptRoot '..\..' }
if (-not (Test-Path -LiteralPath $RepositoryRoot -PathType Container)) {
    Write-Output "ERROR: repository root not found: $RepositoryRoot"
    exit 2
}
$root = (Resolve-Path -LiteralPath $RepositoryRoot).ProviderPath
$registerDir = Join-Path $root 'DEVEL\workarounds'
if (-not (Test-Path -LiteralPath $registerDir -PathType Container)) {
    Write-Output "ERROR: register directory not found: $registerDir"
    exit 2
}

$FieldOrder = @(
    'Status', 'Added', 'GnollHack version', 'Component', 'Package', 'Defective version',
    'Platforms', 'Upstream issue', 'Upstream status when added', 'Upstream status',
    'Fixed upstream in', 'Last checked', 'Code', 'Remove when', 'Related', 'Removed',
    'Superseded by'
)
$OptionalFields = @('Fixed upstream in', 'Related', 'Removed', 'Superseded by')
$StatusValues = @('active', 'removable', 'removed', 'superseded')
$UpstreamValues = @('unreported', 'reported', 'fixed-unreleased', 'released', 'wontfix', 'not-needed')
$CheckResults = @('still-needed', 'not-needed', 'unverified', 'removed')
$PlatformPattern = '^(Android|iOS|Mac \(iPad app\)|Windows|Linux)( \((Avalonia|MAUI)\))?$'
$IdPattern = 'WA-[A-Z]+-\d{4}'

# Known issues: their own fields and vocabularies; the upstream vocabulary is shared.
$KiFieldOrder = @(
    'Status', 'Added', 'GnollHack version', 'Component', 'Package', 'Defective version',
    'Platforms', 'Impact', 'Sentry', 'Upstream issue', 'Upstream status when added',
    'Upstream status', 'Fixed upstream in', 'Last checked', 'Resolve when', 'Worked around by',
    'Related', 'Resolved'
)
$KiOptionalFields = @('Fixed upstream in', 'Worked around by', 'Related', 'Resolved')
$KiStatusValues = @('open', 'worked-around', 'resolved', 'obsolete')
$KiImpactValues = @('crash', 'hang', 'data-loss', 'malfunction', 'performance', 'build')
$KiCheckResults = @('still-present', 'not-present', 'unverified', 'resolved')
$KiIdPattern = 'KI-[A-Z]+-\d{4}'
$AnyIdPattern = '(?:WA|KI)-[A-Z]+-\d{4}'
$AnyTagPattern = 'WORKAROUND\(((?:WA|KI)-[A-Z]+-\d{4})\)'
$KnownIssuesMarker = '<!-- known-issues -->'

$script:Messages = New-Object System.Collections.ArrayList

function Add-LintMessage {
    param([string] $Level, [string] $File, [int] $Line, [string] $Id, [string] $Text)
    [void]$script:Messages.Add([pscustomobject]@{
        Level = $Level; File = $File; Line = $Line; Id = $Id; Text = $Text
    })
}

function Test-IsoDate {
    param([string] $Text)
    $parsed = [datetime]::MinValue
    return [datetime]::TryParseExact($Text, 'yyyy-MM-dd',
        [Globalization.CultureInfo]::InvariantCulture,
        [Globalization.DateTimeStyles]::None, [ref]$parsed)
}

function ConvertTo-RepositoryPath {
    param([string] $Path)
    $normalized = $Path.Replace('\', '/')
    if ($normalized.StartsWith('./')) { $normalized = $normalized.Substring(2) }
    return $normalized.ToLowerInvariant()
}

# Returns the entries of one component file. Three regions are parsed separately: the
# header before the entries marker, each entry's field block (from the heading to the
# first blank line after the fields), and each entry's Checks list. Everything else in an
# entry is prose. Headings after the known-issues marker are known issues.
function Read-ComponentFile {
    param([string] $Path, [string] $DisplayPath)

    $lines = [IO.File]::ReadAllLines($Path, [Text.Encoding]::UTF8)
    $file = [pscustomobject]@{
        Path = $DisplayPath; Prefix = $null; Entries = @(); HasPlaceholder = $false
        PlaceholderLine = 0
    }

    $markerIndex = -1
    for ($i = 0; $i -lt $lines.Count; $i++) {
        if ($lines[$i].Trim() -eq '<!-- entries -->') { $markerIndex = $i; break }
        if ($lines[$i] -match '^- ID prefix: (WA-[A-Z]+)\s*$') { $file.Prefix = $Matches[1] }
        if ($lines[$i].Trim() -eq $KnownIssuesMarker) {
            Add-LintMessage 'ERROR' $DisplayPath ($i + 1) '' "the $KnownIssuesMarker marker comes before <!-- entries -->"
        }
    }
    if ($markerIndex -lt 0) {
        Add-LintMessage 'ERROR' $DisplayPath 1 '' 'missing the <!-- entries --> marker line'
        return $file
    }
    if (-not $file.Prefix) {
        Add-LintMessage 'ERROR' $DisplayPath 1 '' 'header has no "- ID prefix: WA-<NAME>" line'
    }

    $entries = New-Object System.Collections.ArrayList
    $entry = $null
    $mode = 'prose'
    $inKnownIssues = $false
    for ($i = $markerIndex + 1; $i -lt $lines.Count; $i++) {
        $text = $lines[$i]
        $lineNo = $i + 1

        if ($text.Trim() -eq $KnownIssuesMarker) {
            if ($inKnownIssues) {
                Add-LintMessage 'ERROR' $DisplayPath $lineNo '' "second $KnownIssuesMarker marker line"
            }
            $inKnownIssues = $true
            $entry = $null
            $mode = 'prose'
            continue
        }

        if ($text -match '^## ') {
            $kind = $null
            $headingId = $null
            $headingTitle = $null
            if ($text -match "^## ($IdPattern): (\S.*)$") {
                $kind = 'WA'; $headingId = $Matches[1]; $headingTitle = $Matches[2].Trim()
            }
            elseif ($text -match "^## ($KiIdPattern): (\S.*)$") {
                $kind = 'KI'; $headingId = $Matches[1]; $headingTitle = $Matches[2].Trim()
            }
            if ($kind) {
                if (($kind -eq 'WA' -and $inKnownIssues) -or ($kind -eq 'KI' -and -not $inKnownIssues)) {
                    Add-LintMessage 'ERROR' $DisplayPath $lineNo $headingId "$headingId is in the wrong section"
                }
                $entry = [pscustomobject]@{
                    Id = $headingId; Title = $headingTitle; File = $DisplayPath; Kind = $kind
                    Line = $lineNo; Fields = @{}; FieldSequence = New-Object System.Collections.ArrayList
                    Checks = New-Object System.Collections.ArrayList; HasChecksHeader = $false
                    HasSymptom = $false; HasWorkaround = $false; HasWhyNoWorkaround = $false
                }
                [void]$entries.Add($entry)
                $mode = 'fields-start'
            }
            else {
                if ($text -match '^## (WA|KI)-') {
                    Add-LintMessage 'ERROR' $DisplayPath $lineNo '' "malformed entry heading: $text"
                }
                $entry = $null
                $mode = 'prose'
            }
            continue
        }

        if ($text.Trim() -eq 'No entries.') {
            $file.HasPlaceholder = $true
            $file.PlaceholderLine = $lineNo
            continue
        }
        if ($null -eq $entry) { continue }

        if ($mode -eq 'fields-start') {
            if ($text.Trim() -eq '') { continue }
            $mode = 'fields'
        }
        if ($mode -eq 'fields') {
            if ($text.Trim() -eq '') { $mode = 'prose'; continue }
            if ($text -match '^- ([A-Za-z ]+): (.*)$') {
                $name = $Matches[1]
                $value = $Matches[2].Trim()
                $order = $FieldOrder
                if ($entry.Kind -eq 'KI') { $order = $KiFieldOrder }
                if ($order -notcontains $name) {
                    Add-LintMessage 'ERROR' $DisplayPath $lineNo $entry.Id "unknown field '$name'"
                }
                elseif ($entry.Fields.ContainsKey($name)) {
                    Add-LintMessage 'ERROR' $DisplayPath $lineNo $entry.Id "field '$name' given twice"
                }
                else {
                    $entry.Fields[$name] = [pscustomobject]@{ Value = $value; Line = $lineNo }
                    [void]$entry.FieldSequence.Add($name)
                }
            }
            else {
                Add-LintMessage 'ERROR' $DisplayPath $lineNo $entry.Id "line in the field block is not '- <Field>: <value>': $text"
            }
            continue
        }

        if ($mode -eq 'checks') {
            if ($text.Trim() -eq '') { $mode = 'prose'; continue }
            if ($text -match '^- ') {
                if ($text -match '^- (\S+) \| ([^|]+?) \| ([^|]+?)(?: \| (.*))?$') {
                    [void]$entry.Checks.Add([pscustomobject]@{
                        Date = $Matches[1]; Versions = $Matches[2].Trim(); Result = $Matches[3].Trim()
                        Line = $lineNo
                    })
                }
                else {
                    Add-LintMessage 'ERROR' $DisplayPath $lineNo $entry.Id "malformed check line, expected '- <date> | <versions> | <result> | <note>'"
                }
                continue
            }
            $mode = 'prose'
        }

        if ($text.Trim() -eq 'Checks:') {
            $entry.HasChecksHeader = $true
            $mode = 'checks'
            continue
        }
        if ($text -match '^\*\*Symptom\.\*\*') { $entry.HasSymptom = $true }
        if ($text -match '^\*\*Workaround\.\*\*') { $entry.HasWorkaround = $true }
        if ($text -match '^\*\*Why no workaround\.\*\*') { $entry.HasWhyNoWorkaround = $true }
    }

    $file.Entries = @($entries)
    return $file
}

# Checks one entry on its own. Checks that need every entry (duplicates, Superseded by,
# Worked around by, Related, code tags) run afterwards.
function Test-Entry {
    param($Entry, [string] $Prefix)

    $id = $Entry.Id
    $f = $Entry.File
    $fields = $Entry.Fields
    $isKnownIssue = ($Entry.Kind -eq 'KI')

    $order = $FieldOrder
    $optional = $OptionalFields
    $statusValues = $StatusValues
    $checkResults = $CheckResults
    $expectedPrefix = $Prefix
    if ($isKnownIssue) {
        $order = $KiFieldOrder
        $optional = $KiOptionalFields
        $statusValues = $KiStatusValues
        $checkResults = $KiCheckResults
        if ($Prefix) { $expectedPrefix = 'KI-' + $Prefix.Substring(3) }
    }

    if ($expectedPrefix -and -not $id.StartsWith("$expectedPrefix-")) {
        Add-LintMessage 'ERROR' $f $Entry.Line $id "ID prefix does not match the file's prefix $expectedPrefix"
    }

    foreach ($name in $order) {
        if ($optional -contains $name) { continue }
        if (-not $fields.ContainsKey($name)) {
            Add-LintMessage 'ERROR' $f $Entry.Line $id "missing required field '$name'"
        }
    }
    foreach ($name in @($fields.Keys)) {
        if ($fields[$name].Value -eq '') {
            Add-LintMessage 'ERROR' $f $fields[$name].Line $id "field '$name' is empty"
        }
    }

    $last = -1
    foreach ($name in $Entry.FieldSequence) {
        $index = [array]::IndexOf($order, $name)
        if ($index -lt $last) {
            Add-LintMessage 'WARNING' $f $fields[$name].Line $id "field '$name' is out of the schema order"
            break
        }
        $last = $index
    }

    $seeded = $false
    if ($fields.ContainsKey('Added')) {
        $added = $fields['Added']
        if ($added.Value -match '^(\S+)( \(from git history\))?$' -and (Test-IsoDate $Matches[1])) {
            $seeded = [bool]$Matches[2]
        }
        else {
            Add-LintMessage 'ERROR' $f $added.Line $id "Added must be an ISO date, optionally followed by ' (from git history)'"
        }
    }

    $status = $null
    if ($fields.ContainsKey('Status')) {
        $status = $fields['Status'].Value
        if ($statusValues -notcontains $status) {
            Add-LintMessage 'ERROR' $f $fields['Status'].Line $id "Status '$status' is not one of: $($statusValues -join ', ')"
        }
    }

    if ($fields.ContainsKey('GnollHack version')) {
        $v = $fields['GnollHack version']
        if ($v.Value -eq 'unknown') {
            if (-not $seeded -and -not $isKnownIssue) {
                Add-LintMessage 'ERROR' $f $v.Line $id "GnollHack version 'unknown' is allowed only on entries added '(from git history)'"
            }
        }
        elseif ($v.Value -notmatch '^\d+\.\d+ \(\d+\)$' -and $v.Value -notmatch '^game \d+\.\d+\.\d+-\d+$') {
            Add-LintMessage 'ERROR' $f $v.Line $id "GnollHack version must look like '3.30 (430)' or 'game 4.3.0-27'"
        }
    }

    if ($fields.ContainsKey('Defective version') -and $fields['Defective version'].Value -match '^unknown\b' -and -not $seeded) {
        Add-LintMessage 'ERROR' $f $fields['Defective version'].Line $id "Defective version 'unknown' is allowed only on entries added '(from git history)'"
    }

    if ($fields.ContainsKey('Platforms')) {
        $p = $fields['Platforms']
        $tokens = @($p.Value -split ', ')
        if (-not ($tokens.Count -eq 1 -and $tokens[0] -eq 'all')) {
            foreach ($token in $tokens) {
                if ($token -notmatch $PlatformPattern) {
                    Add-LintMessage 'ERROR' $f $p.Line $id "unknown platform '$token'"
                }
            }
        }
    }

    if ($isKnownIssue) {
        if ($fields.ContainsKey('Impact') -and $KiImpactValues -notcontains $fields['Impact'].Value) {
            Add-LintMessage 'ERROR' $f $fields['Impact'].Line $id "Impact '$($fields['Impact'].Value)' is not one of: $($KiImpactValues -join ', ')"
        }
        if ($fields.ContainsKey('Sentry') -and $fields['Sentry'].Value -ne 'none') {
            foreach ($sentryId in @($fields['Sentry'].Value -split ', ')) {
                if ($sentryId -notmatch '^[A-Z][A-Z0-9]*-[A-Z0-9]+$') {
                    Add-LintMessage 'ERROR' $f $fields['Sentry'].Line $id "Sentry '$sentryId' is not a Sentry short ID such as GNOLLHACK-HJ, or 'none'"
                }
            }
        }
    }

    $issueUrls = @()
    if ($fields.ContainsKey('Upstream issue')) {
        $u = $fields['Upstream issue']
        if ($u.Value -notmatch '^none( \(.+\))?$') {
            foreach ($url in @($u.Value -split ', ')) {
                if ($url -match '^https?://\S+$') { $issueUrls += $url }
                else { Add-LintMessage 'ERROR' $f $u.Line $id "Upstream issue '$url' is not a URL or 'none'" }
            }
        }
    }

    if ($fields.ContainsKey('Upstream status when added')) {
        $w = $fields['Upstream status when added']
        if ($w.Value -eq 'unknown') {
            if (-not $seeded) {
                Add-LintMessage 'ERROR' $f $w.Line $id "Upstream status when added 'unknown' is allowed only on entries added '(from git history)'"
            }
        }
        elseif ($UpstreamValues -notcontains $w.Value) {
            Add-LintMessage 'ERROR' $f $w.Line $id "Upstream status when added '$($w.Value)' is not one of: $($UpstreamValues -join ', '), unknown"
        }
    }

    $upstream = $null
    if ($fields.ContainsKey('Upstream status')) {
        $upstream = $fields['Upstream status'].Value
        if ($UpstreamValues -notcontains $upstream) {
            Add-LintMessage 'ERROR' $f $fields['Upstream status'].Line $id "Upstream status '$upstream' is not one of: $($UpstreamValues -join ', ')"
        }
        if ($upstream -eq 'unreported' -and $issueUrls.Count -gt 0) {
            Add-LintMessage 'WARNING' $f $fields['Upstream status'].Line $id "Upstream status is 'unreported' but an upstream issue is linked"
        }
    }

    if ($isKnownIssue) {
        $needsFix = ($upstream -eq 'fixed-unreleased' -or $upstream -eq 'released' -or $status -eq 'resolved')
        if ($needsFix -and -not $fields.ContainsKey('Fixed upstream in')) {
            Add-LintMessage 'ERROR' $f $Entry.Line $id "Fixed upstream in is required when Upstream status is fixed-unreleased or released, or Status is resolved"
        }
    }
    else {
        $needsFix = ($upstream -eq 'fixed-unreleased' -or $upstream -eq 'released' -or $status -eq 'removable')
        if ($needsFix -and -not $fields.ContainsKey('Fixed upstream in')) {
            Add-LintMessage 'ERROR' $f $Entry.Line $id "Fixed upstream in is required when Upstream status is fixed-unreleased or released, or Status is removable"
        }
    }

    if ($isKnownIssue) {
        if ($status -eq 'resolved') {
            if (-not $fields.ContainsKey('Resolved')) {
                Add-LintMessage 'ERROR' $f $Entry.Line $id "a resolved entry needs the Resolved field"
            }
            elseif ($fields['Resolved'].Value -notmatch '^(\S+), \S' -or -not (Test-IsoDate $Matches[1])) {
                Add-LintMessage 'ERROR' $f $fields['Resolved'].Line $id "Resolved must be '<ISO date>, <GnollHack version>'"
            }
        }
        elseif ($fields.ContainsKey('Resolved')) {
            Add-LintMessage 'ERROR' $f $fields['Resolved'].Line $id "Resolved is given but Status is '$status'"
        }
        if ($status -eq 'worked-around') {
            if (-not $fields.ContainsKey('Worked around by')) {
                Add-LintMessage 'ERROR' $f $Entry.Line $id "a worked-around entry needs the Worked around by field"
            }
            elseif ($fields['Worked around by'].Value -notmatch "^$IdPattern$") {
                Add-LintMessage 'ERROR' $f $fields['Worked around by'].Line $id "Worked around by must be one workaround ID"
            }
        }
        elseif ($fields.ContainsKey('Worked around by')) {
            Add-LintMessage 'ERROR' $f $fields['Worked around by'].Line $id "Worked around by is given but Status is '$status'"
        }
    }
    else {
        if ($status -eq 'removed') {
            if (-not $fields.ContainsKey('Removed')) {
                Add-LintMessage 'ERROR' $f $Entry.Line $id "a removed entry needs the Removed field"
            }
            elseif ($fields['Removed'].Value -notmatch '^(\S+), \S' -or -not (Test-IsoDate $Matches[1])) {
                Add-LintMessage 'ERROR' $f $fields['Removed'].Line $id "Removed must be '<ISO date>, <GnollHack version>'"
            }
        }
        elseif ($fields.ContainsKey('Removed')) {
            Add-LintMessage 'ERROR' $f $fields['Removed'].Line $id "Removed is given but Status is '$status'"
        }
        if ($status -eq 'superseded') {
            if (-not $fields.ContainsKey('Superseded by')) {
                Add-LintMessage 'ERROR' $f $Entry.Line $id "a superseded entry needs the Superseded by field"
            }
            elseif ($fields['Superseded by'].Value -notmatch "^$IdPattern$") {
                Add-LintMessage 'ERROR' $f $fields['Superseded by'].Line $id "Superseded by must be one workaround ID"
            }
        }
        elseif ($fields.ContainsKey('Superseded by')) {
            Add-LintMessage 'ERROR' $f $fields['Superseded by'].Line $id "Superseded by is given but Status is '$status'"
        }
    }

    $Entry | Add-Member -NotePropertyName Sites -NotePropertyValue @()
    if ($fields.ContainsKey('Code')) {
        $c = $fields['Code']
        if ($c.Value -notmatch '^none \(.+\)$') {
            $sites = @()
            foreach ($site in @($c.Value -split '; ')) {
                if ($site -match '^`([^`]+)` \((.+)\)$') {
                    $sites += [pscustomobject]@{ Path = $Matches[1]; Symbol = $Matches[2] }
                }
                else {
                    Add-LintMessage 'ERROR' $f $c.Line $id ('Code site is not "`<path>` (<symbol>)": ' + $site)
                }
            }
            $Entry.Sites = $sites
            if ($status -eq 'active' -or $status -eq 'removable') {
                foreach ($site in $sites) {
                    if (-not (Test-Path -LiteralPath (Join-Path $root $site.Path) -PathType Leaf)) {
                        Add-LintMessage 'ERROR' $f $c.Line $id "Code path does not exist: $($site.Path)"
                    }
                }
            }
        }
    }

    if (-not $Entry.HasChecksHeader -or $Entry.Checks.Count -eq 0) {
        Add-LintMessage 'ERROR' $f $Entry.Line $id "no Checks lines"
    }
    $previous = $null
    foreach ($check in $Entry.Checks) {
        if (-not (Test-IsoDate $check.Date)) {
            Add-LintMessage 'ERROR' $f $check.Line $id "check date '$($check.Date)' is not a valid ISO date"
        }
        elseif ($previous -and [string]::CompareOrdinal($check.Date, $previous) -lt 0) {
            Add-LintMessage 'ERROR' $f $check.Line $id "check lines must be oldest first"
        }
        else {
            $previous = $check.Date
        }
        if ($checkResults -notcontains $check.Result) {
            Add-LintMessage 'ERROR' $f $check.Line $id "check result '$($check.Result)' is not one of: $($checkResults -join ', ')"
        }
    }

    if ($fields.ContainsKey('Last checked')) {
        $lc = $fields['Last checked']
        if (-not (Test-IsoDate $lc.Value)) {
            Add-LintMessage 'ERROR' $f $lc.Line $id "Last checked must be an ISO date"
        }
        else {
            if ($Entry.Checks.Count -gt 0 -and $Entry.Checks[$Entry.Checks.Count - 1].Date -ne $lc.Value) {
                Add-LintMessage 'ERROR' $f $lc.Line $id "Last checked ($($lc.Value)) is not the date of the newest check line"
            }
            $age = ((Get-Date).Date - [datetime]::ParseExact($lc.Value, 'yyyy-MM-dd', [Globalization.CultureInfo]::InvariantCulture)).Days
            $staleStatus = 'active'
            if ($isKnownIssue) { $staleStatus = 'open' }
            if ($status -eq $staleStatus -and $age -gt $StaleDays) {
                Add-LintMessage 'WARNING' $f $lc.Line $id "last checked $age days ago; re-check"
            }
        }
    }

    if (-not $Entry.HasSymptom) {
        Add-LintMessage 'ERROR' $f $Entry.Line $id "missing the **Symptom.** paragraph"
    }
    if ($isKnownIssue) {
        if (-not $Entry.HasWhyNoWorkaround) {
            Add-LintMessage 'ERROR' $f $Entry.Line $id "missing the **Why no workaround.** paragraph"
        }
    }
    elseif (-not $Entry.HasWorkaround) {
        Add-LintMessage 'ERROR' $f $Entry.Line $id "missing the **Workaround.** paragraph"
    }
}

# Register files.
$files = @()
foreach ($item in @(Get-ChildItem -LiteralPath $registerDir -Filter '*.md' -File | Sort-Object Name)) {
    if ($item.Name -eq 'README.md') { continue }
    $files += Read-ComponentFile -Path $item.FullName -DisplayPath "DEVEL/workarounds/$($item.Name)"
}

$allEntries = @()
$byId = @{}
foreach ($file in $files) {
    $lastNumber = @{ WA = -1; KI = -1 }
    foreach ($entry in $file.Entries) {
        Test-Entry -Entry $entry -Prefix $file.Prefix
        $allEntries += $entry
        if ($byId.ContainsKey($entry.Id)) {
            $first = $byId[$entry.Id]
            Add-LintMessage 'ERROR' $entry.File $entry.Line $entry.Id "duplicate ID, first used at $($first.File):$($first.Line)"
        }
        else {
            $byId[$entry.Id] = $entry
        }
        $number = [int]$entry.Id.Substring($entry.Id.Length - 4)
        if ($number -le $lastNumber[$entry.Kind]) {
            Add-LintMessage 'WARNING' $entry.File $entry.Line $entry.Id "IDs in the file are not ascending"
        }
        $lastNumber[$entry.Kind] = $number
    }
    if ($file.HasPlaceholder -and $file.Entries.Count -gt 0) {
        Add-LintMessage 'WARNING' $file.Path $file.PlaceholderLine '' "'No entries.' line left in a file that has entries"
    }
}

foreach ($entry in $allEntries) {
    $fields = $entry.Fields
    if ($fields.ContainsKey('Superseded by')) {
        $target = $fields['Superseded by'].Value
        if ($target -match "^$IdPattern$" -and -not $byId.ContainsKey($target)) {
            Add-LintMessage 'ERROR' $entry.File $fields['Superseded by'].Line $entry.Id "Superseded by names $target, which does not exist"
        }
    }
    if ($fields.ContainsKey('Worked around by')) {
        $target = $fields['Worked around by'].Value
        if ($target -match "^$IdPattern$" -and -not $byId.ContainsKey($target)) {
            Add-LintMessage 'ERROR' $entry.File $fields['Worked around by'].Line $entry.Id "Worked around by names $target, which does not exist"
        }
    }
    if ($fields.ContainsKey('Related')) {
        foreach ($m in [regex]::Matches($fields['Related'].Value, $AnyIdPattern)) {
            if (-not $byId.ContainsKey($m.Value)) {
                Add-LintMessage 'WARNING' $entry.File $fields['Related'].Line $entry.Id "Related names $($m.Value), which does not exist"
            }
        }
    }
}

# NuGet version drift: only entries whose fix is merged or released upstream can become
# removable, or resolved, through a package bump.
$csprojPath = Join-Path $root 'win\win32\xpl\GnollHackM\GnollHackM.csproj'
if (Test-Path -LiteralPath $csprojPath) {
    $csproj = [IO.File]::ReadAllText($csprojPath)
    $csproj = [regex]::Replace($csproj, '<!--.*?-->', '', [Text.RegularExpressions.RegexOptions]::Singleline)
    $packageVersions = @{}
    foreach ($m in [regex]::Matches($csproj, '<PackageReference\s+Include="([^"]+)"\s+Version="([^"]+)"')) {
        $name = $m.Groups[1].Value
        if (-not $packageVersions.ContainsKey($name)) { $packageVersions[$name] = @() }
        $packageVersions[$name] += $m.Groups[2].Value
    }
    foreach ($entry in $allEntries) {
        $fields = $entry.Fields
        if (-not $fields.ContainsKey('Package') -or -not $fields.ContainsKey('Upstream status')) { continue }
        if (@('fixed-unreleased', 'released') -notcontains $fields['Upstream status'].Value) { continue }
        if (-not $entry.Fields.ContainsKey('Status')) { continue }
        $openStatuses = @('active', 'removable')
        if ($entry.Kind -eq 'KI') { $openStatuses = @('open', 'worked-around') }
        if ($openStatuses -notcontains $entry.Fields['Status'].Value) { continue }
        if ($entry.Checks.Count -eq 0) { continue }
        $checked = $entry.Checks[$entry.Checks.Count - 1].Versions
        foreach ($package in @($fields['Package'].Value -split ', ')) {
            if (-not $packageVersions.ContainsKey($package)) { continue }
            $covered = $false
            foreach ($version in $packageVersions[$package]) {
                if ($checked.Contains($version)) { $covered = $true }
            }
            if (-not $covered) {
                $suffix = 're-check'
                if ($entry.Kind -eq 'KI') { $suffix = 're-check, the upgrade may fix it' }
                Add-LintMessage 'WARNING' $entry.File $fields['Package'].Line $entry.Id "GnollHackM.csproj references $package $($packageVersions[$package] -join ' and '), which the newest check does not cover; $suffix"
            }
        }
    }
}

# Code tags.
$tagCount = 0
if (-not $RegisterOnly) {
    $git = Get-Command git -ErrorAction SilentlyContinue
    if (-not $git) {
        Write-Output 'ERROR: git is not on PATH; use -RegisterOnly to skip the code checks'
        exit 2
    }
    $grepArgs = @(
        '-c', 'core.quotePath=false', '-C', $root, 'grep', '-n', '-I', '--untracked', '-E',
        'WORKAROUND\((WA|KI)-[A-Z]+-[0-9]{4}\)', '--', '.',
        ':(exclude)DEVEL/workarounds', ':(exclude).agents', ':(exclude).claude'
    )
    $previousPreference = $ErrorActionPreference
    $ErrorActionPreference = 'Continue'
    $grepOutput = @(& $git.Source @grepArgs)
    $grepCode = $LASTEXITCODE
    $ErrorActionPreference = $previousPreference
    if ($grepCode -gt 1) {
        Write-Output "ERROR: git grep failed with exit code $grepCode in $root"
        exit 2
    }

    # Tag counts per ID and per file, both keyed by lowercased repository path.
    $tagsById = @{}
    foreach ($line in $grepOutput) {
        if ($line -notmatch '^(.+?):(\d+):(.*)$') { continue }
        $path = $Matches[1]
        $lineNo = [int]$Matches[2]
        foreach ($m in [regex]::Matches($Matches[3], $AnyTagPattern)) {
            $tagCount++
            $id = $m.Groups[1].Value
            if ($id.StartsWith('KI-')) {
                Add-LintMessage 'ERROR' $path $lineNo $id "known issues have no code tags"
                continue
            }
            if (-not $byId.ContainsKey($id)) {
                Add-LintMessage 'ERROR' $path $lineNo $id "code tag names an ID that is not in the register"
                continue
            }
            if (-not $tagsById.ContainsKey($id)) { $tagsById[$id] = @{} }
            $key = ConvertTo-RepositoryPath $path
            if (-not $tagsById[$id].ContainsKey($key)) { $tagsById[$id][$key] = 0 }
            $tagsById[$id][$key]++
        }
    }

    foreach ($entry in $allEntries) {
        if ($entry.Kind -eq 'KI') { continue }
        $status = $null
        if ($entry.Fields.ContainsKey('Status')) { $status = $entry.Fields['Status'].Value }
        $tags = @{}
        if ($tagsById.ContainsKey($entry.Id)) { $tags = $tagsById[$entry.Id] }
        if ($status -eq 'removed' -or $status -eq 'superseded') {
            foreach ($key in $tags.Keys) {
                Add-LintMessage 'ERROR' $entry.File $entry.Line $entry.Id "entry is $status but $key still has its tag"
            }
            continue
        }
        if ($status -ne 'active' -and $status -ne 'removable') { continue }
        if ($entry.Sites.Count -eq 0) { continue }
        if ($tags.Count -eq 0) {
            Add-LintMessage 'ERROR' $entry.File $entry.Line $entry.Id "no WORKAROUND($($entry.Id)) tag found in the code"
            continue
        }
        $sitesPerPath = @{}
        foreach ($site in $entry.Sites) {
            $key = ConvertTo-RepositoryPath $site.Path
            if (-not $sitesPerPath.ContainsKey($key)) { $sitesPerPath[$key] = 0 }
            $sitesPerPath[$key]++
        }
        foreach ($key in $sitesPerPath.Keys) {
            $found = 0
            if ($tags.ContainsKey($key)) { $found = $tags[$key] }
            if ($found -lt $sitesPerPath[$key]) {
                Add-LintMessage 'ERROR' $entry.File $entry.Fields['Code'].Line $entry.Id "$key has $found tag(s) for $($sitesPerPath[$key]) site(s)"
            }
        }
    }
}

# Upstream issue and pull request states, through the gh CLI.
if ($CheckUpstream) {
    $gh = Get-Command gh -ErrorAction SilentlyContinue
    if (-not $gh) {
        Add-LintMessage 'WARNING' '' 0 '' 'gh is not on PATH; -CheckUpstream skipped'
    }
    else {
        foreach ($entry in $allEntries) {
            $fields = $entry.Fields
            if (-not $fields.ContainsKey('Upstream issue') -or -not $fields.ContainsKey('Upstream status')) { continue }
            $upstream = $fields['Upstream status'].Value
            foreach ($url in @($fields['Upstream issue'].Value -split ', ')) {
                if ($url -notmatch '^https://github\.com/([^/]+)/([^/]+)/(issues|pull)/(\d+)') { continue }
                $repo = "$($Matches[1])/$($Matches[2])"
                $isPull = ($Matches[3] -eq 'pull')
                $number = $Matches[4]
                $ghKind = 'issue'
                if ($isPull) { $ghKind = 'pr' }
                $previousPreference = $ErrorActionPreference
                $ErrorActionPreference = 'Continue'
                $json = (& $gh.Source $ghKind view $number --repo $repo --json state) -join ''
                $ghCode = $LASTEXITCODE
                $ErrorActionPreference = $previousPreference
                if ($ghCode -ne 0 -or -not $json) {
                    Add-LintMessage 'WARNING' $entry.File $fields['Upstream issue'].Line $entry.Id "gh could not read $repo#$number"
                    continue
                }
                $state = (ConvertFrom-Json $json).state
                if ($isPull) {
                    if ($state -eq 'MERGED' -and @('unreported', 'reported') -contains $upstream) {
                        Add-LintMessage 'WARNING' $entry.File $fields['Upstream status'].Line $entry.Id "$repo#$number is merged but Upstream status is '$upstream'"
                    }
                    continue
                }
                if ($state -eq 'CLOSED' -and @('unreported', 'reported') -contains $upstream) {
                    Add-LintMessage 'WARNING' $entry.File $fields['Upstream status'].Line $entry.Id "$repo#$number is closed but Upstream status is '$upstream'"
                }
                if ($state -eq 'OPEN' -and $upstream -eq 'released') {
                    Add-LintMessage 'WARNING' $entry.File $fields['Upstream status'].Line $entry.Id "$repo#$number is open but Upstream status is 'released'"
                }
            }
        }
    }
}

$errors = 0
$warnings = 0
foreach ($message in ($script:Messages | Sort-Object File, Line)) {
    if ($message.Level -eq 'ERROR') { $errors++ } else { $warnings++ }
    $location = $message.File
    if ($message.Line -gt 0) { $location = "${location}:$($message.Line)" }
    $id = ''
    if ($message.Id) { $id = "$($message.Id): " }
    Write-Output ("{0} {1}: {2}{3}" -f $message.Level, $location, $id, $message.Text)
}
$workaroundCount = @($allEntries | Where-Object { $_.Kind -eq 'WA' }).Count
$knownIssueCount = @($allEntries | Where-Object { $_.Kind -eq 'KI' }).Count
$scope = 'register only'
if (-not $RegisterOnly) { $scope = "$tagCount code tag(s)" }
Write-Output ("{0} error(s), {1} warning(s); {2} workaround(s) and {3} known issue(s) in {4} files, {5}." -f $errors, $warnings, $workaroundCount, $knownIssueCount, $files.Count, $scope)

if ($errors -gt 0) { exit 1 }
exit 0
