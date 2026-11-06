<#
.SYNOPSIS
    Writes the Windows icon file of the Avalonia Windows flavor from the app icon.

.DESCRIPTION
    Scales Resources\AppIcon\appicon.png to each size Windows asks for, with high-quality bicubic filtering, and
    writes them as PNG entries of one .ico file. GnollHackM.csproj embeds the result as the executable's icon
    (ApplicationIcon) in the Avalonia win-* build. The sizes are those of GHAvaloniaWindowHooks.IconSizes, which
    builds the window icon at run time the same way.

    Run it again when appicon.png changes, and commit the output.

.PARAMETER SourcePath
    The source image. Default: Resources\AppIcon\appicon.png in GnollHackM.

.PARAMETER OutputPath
    The icon file to write. Default: Platforms\WindowsAvalonia\appicon.ico in GnollHackM.

.EXAMPLE
    win\win32\xpl\GnollHackM\Platforms\WindowsAvalonia\Packaging\New-WindowsAppIcon.ps1
#>
[CmdletBinding()]
param(
    [string]$SourcePath,
    [string]$OutputPath
)

Set-StrictMode -Version 3
$ErrorActionPreference = 'Stop'
Add-Type -AssemblyName System.Drawing

$GnollHackMDir = (Resolve-Path (Join-Path $PSScriptRoot '..\..\..')).Path
if (-not $SourcePath) { $SourcePath = Join-Path $GnollHackMDir 'Resources\AppIcon\appicon.png' }
if (-not $OutputPath) { $OutputPath = Join-Path $GnollHackMDir 'Platforms\WindowsAvalonia\appicon.ico' }

if (-not (Test-Path $SourcePath))
{
    throw ('New-WindowsAppIcon: {0} is missing.' -f $SourcePath)
}

# The sizes of GHAvaloniaWindowHooks.IconSizes. Windows picks the matching entry, and scales a single large one with
# point sampling.
$IconSizes = @(16, 20, 24, 32, 40, 48, 64, 96, 128, 256)

function Set-HighQuality
{
    param([System.Drawing.Graphics]$Graphics)

    $Graphics.SmoothingMode = [System.Drawing.Drawing2D.SmoothingMode]::AntiAlias
    $Graphics.InterpolationMode = [System.Drawing.Drawing2D.InterpolationMode]::HighQualityBicubic
    $Graphics.PixelOffsetMode = [System.Drawing.Drawing2D.PixelOffsetMode]::HighQuality
    $Graphics.CompositingQuality = [System.Drawing.Drawing2D.CompositingQuality]::HighQuality
}

# One PNG-encoded entry of the given size.
function New-IconEntry
{
    param([System.Drawing.Image]$Source, [int]$Size)

    $bitmap = New-Object System.Drawing.Bitmap $Size, $Size, ([System.Drawing.Imaging.PixelFormat]::Format32bppArgb)
    try
    {
        $graphics = [System.Drawing.Graphics]::FromImage($bitmap)
        Set-HighQuality $graphics
        $graphics.Clear([System.Drawing.Color]::Transparent)
        # TileFlipXY keeps the filter from blending transparent pixels from outside the source into its edges.
        $attributes = New-Object System.Drawing.Imaging.ImageAttributes
        $attributes.SetWrapMode([System.Drawing.Drawing2D.WrapMode]::TileFlipXY)
        $rectangle = New-Object System.Drawing.Rectangle 0, 0, $Size, $Size
        $pixel = [System.Drawing.GraphicsUnit]::Pixel
        $graphics.DrawImage($Source, $rectangle, 0, 0, $Source.Width, $Source.Height, $pixel, $attributes)
        $attributes.Dispose()
        $graphics.Dispose()

        $stream = New-Object System.IO.MemoryStream
        try
        {
            $bitmap.Save($stream, [System.Drawing.Imaging.ImageFormat]::Png)
            return ,$stream.ToArray()
        }
        finally
        {
            $stream.Dispose()
        }
    }
    finally
    {
        $bitmap.Dispose()
    }
}

$source = [System.Drawing.Image]::FromFile((Resolve-Path $SourcePath).Path)
try
{
    $entries = New-Object 'System.Collections.Generic.List[byte[]]'
    foreach ($size in $IconSizes)
    {
        $entries.Add((New-IconEntry $source $size))
    }
}
finally
{
    $source.Dispose()
}

# ICONDIR, one ICONDIRENTRY per size, then the PNG data in the same order.
$ico = New-Object System.IO.MemoryStream
$writer = New-Object System.IO.BinaryWriter $ico
try
{
    $writer.Write([uint16]0)
    $writer.Write([uint16]1)
    $writer.Write([uint16]$IconSizes.Count)
    $offset = 6 + 16 * $IconSizes.Count
    for ($i = 0; $i -lt $IconSizes.Count; $i++)
    {
        $size = $IconSizes[$i]
        # A width or height of 0 means 256.
        $dimension = if ($size -ge 256) { 0 } else { $size }
        $writer.Write([byte]$dimension)
        $writer.Write([byte]$dimension)
        $writer.Write([byte]0)
        $writer.Write([byte]0)
        $writer.Write([uint16]1)
        $writer.Write([uint16]32)
        $writer.Write([int]$entries[$i].Length)
        $writer.Write([int]$offset)
        $offset += $entries[$i].Length
    }
    foreach ($entry in $entries)
    {
        $writer.Write($entry)
    }
    $writer.Flush()
    $bytes = $ico.ToArray()
}
finally
{
    $writer.Dispose()
    $ico.Dispose()
}

$outputDirectory = Split-Path -Parent $OutputPath
if (-not (Test-Path $outputDirectory))
{
    New-Item -ItemType Directory -Force $outputDirectory | Out-Null
}
[System.IO.File]::WriteAllBytes($OutputPath, $bytes)

# Read the header back: the entry count, and each entry's data must be a PNG inside the file.
$written = [System.IO.File]::ReadAllBytes($OutputPath)
$count = [System.BitConverter]::ToUInt16($written, 4)
$pngEntries = 0
for ($i = 0; $i -lt $count; $i++)
{
    $entryOffset = [System.BitConverter]::ToInt32($written, 6 + 16 * $i + 12)
    $entryLength = [System.BitConverter]::ToInt32($written, 6 + 16 * $i + 8)
    $inside = $entryOffset + $entryLength -le $written.Length
    if ($inside -and $written[$entryOffset] -eq 0x89 -and $written[$entryOffset + 1] -eq 0x50)
    {
        $pngEntries++
    }
}
$sizeList = $IconSizes -join ', '
Write-Host ('New-WindowsAppIcon: wrote {0} ({1} bytes, {2} entries: {3}).' -f $OutputPath, $written.Length, $count, $sizeList)
if ($count -ne $IconSizes.Count -or $pngEntries -ne $IconSizes.Count)
{
    throw ('New-WindowsAppIcon: expected {0} PNG entries, found {1} of {2}.' -f $IconSizes.Count, $pngEntries, $count)
}
