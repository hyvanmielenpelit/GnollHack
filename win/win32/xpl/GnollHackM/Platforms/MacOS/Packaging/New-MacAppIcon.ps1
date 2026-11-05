<#
.SYNOPSIS
    Composes the macOS app icon master from the two layers of the iOS icon.

.DESCRIPTION
    Draws the iOS icon's background (Resources\AppIcon\iOS\appicon.png) as a rounded square in Apple's macOS 11+ icon
    grid: an 824 x 824 superellipse centered on a transparent 1024 x 1024 canvas, with a soft drop shadow in the
    margin. The shield (Resources\AppIcon\iOS\appiconios.png) is centered on it. package-mac.sh builds GnollHack.icns
    from the result, which Build-MacPackage.ps1 stages as packaging/appicon.png.

    Run it again when either iOS layer changes, and commit the output.

.PARAMETER BackgroundPath
    The background layer. Default: Resources\AppIcon\iOS\appicon.png in GnollHackM.

.PARAMETER ForegroundPath
    The foreground layer. Default: Resources\AppIcon\iOS\appiconios.png in GnollHackM.

.PARAMETER OutputPath
    The PNG to write. Default: Resources\AppIcon\macOS\appicon-macos.png in GnollHackM.

.PARAMETER ShieldScale
    The foreground's size as a fraction of the rounded square's width. Default: 0.78.

.EXAMPLE
    win\win32\xpl\GnollHackM\Platforms\MacOS\Packaging\New-MacAppIcon.ps1
#>
[CmdletBinding()]
param(
    [string]$BackgroundPath,
    [string]$ForegroundPath,
    [string]$OutputPath,
    [ValidateRange(0.3, 1.0)]
    [double]$ShieldScale = 0.78
)

Set-StrictMode -Version 3
$ErrorActionPreference = 'Stop'
Add-Type -AssemblyName System.Drawing

$GnollHackMDir = (Resolve-Path (Join-Path $PSScriptRoot '..\..\..')).Path
if (-not $BackgroundPath) { $BackgroundPath = Join-Path $GnollHackMDir 'Resources\AppIcon\iOS\appicon.png' }
if (-not $ForegroundPath) { $ForegroundPath = Join-Path $GnollHackMDir 'Resources\AppIcon\iOS\appiconios.png' }
if (-not $OutputPath) { $OutputPath = Join-Path $GnollHackMDir 'Resources\AppIcon\macOS\appicon-macos.png' }

foreach ($inputPath in @($BackgroundPath, $ForegroundPath))
{
    if (-not (Test-Path $inputPath))
    {
        throw ('New-MacAppIcon: {0} is missing.' -f $inputPath)
    }
}

# Apple's macOS 11+ app icon grid: a 1024 canvas, an 824 body, a 100-pixel margin for the shadow.
$CanvasSize = 1024
$BodySize = 824
$BodyOrigin = ($CanvasSize - $BodySize) / 2
$SuperellipseExponent = 5.0
$PathPoints = 720
$ShadowOffsetY = 10
$ShadowAlpha = 77
$ShadowDownscale = 8

# A superellipse |x|^n + |y|^n = 1, which approximates Apple's continuous-corner rounded square.
function New-BodyPath
{
    param([double]$Left, [double]$Top, [double]$Size)

    $radius = $Size / 2.0
    $centerX = $Left + $radius
    $centerY = $Top + $radius
    $power = 2.0 / $SuperellipseExponent
    $points = New-Object 'System.Drawing.PointF[]' $PathPoints
    for ($i = 0; $i -lt $PathPoints; $i++)
    {
        $angle = 2.0 * [Math]::PI * $i / $PathPoints
        $cos = [Math]::Cos($angle)
        $sin = [Math]::Sin($angle)
        $x = $centerX + $radius * [Math]::Sign($cos) * [Math]::Pow([Math]::Abs($cos), $power)
        $y = $centerY + $radius * [Math]::Sign($sin) * [Math]::Pow([Math]::Abs($sin), $power)
        $points[$i] = New-Object System.Drawing.PointF ([single]$x), ([single]$y)
    }
    $path = New-Object System.Drawing.Drawing2D.GraphicsPath
    $path.AddPolygon($points)
    return ,$path
}

function Set-HighQuality
{
    param([System.Drawing.Graphics]$Graphics)

    $Graphics.SmoothingMode = [System.Drawing.Drawing2D.SmoothingMode]::AntiAlias
    $Graphics.InterpolationMode = [System.Drawing.Drawing2D.InterpolationMode]::HighQualityBicubic
    $Graphics.PixelOffsetMode = [System.Drawing.Drawing2D.PixelOffsetMode]::HighQuality
    $Graphics.CompositingQuality = [System.Drawing.Drawing2D.CompositingQuality]::HighQuality
}

$background = [System.Drawing.Image]::FromFile((Resolve-Path $BackgroundPath).Path)
$foreground = [System.Drawing.Image]::FromFile((Resolve-Path $ForegroundPath).Path)
$canvas = New-Object System.Drawing.Bitmap $CanvasSize, $CanvasSize, ([System.Drawing.Imaging.PixelFormat]::Format32bppArgb)
try
{
    $graphics = [System.Drawing.Graphics]::FromImage($canvas)
    Set-HighQuality $graphics
    $graphics.Clear([System.Drawing.Color]::Transparent)

    # Shadow: GDI+ has no blur, so the body is drawn small and scaled up, which softens its edge.
    $shadowSize = $CanvasSize / $ShadowDownscale
    $shadow = New-Object System.Drawing.Bitmap $shadowSize, $shadowSize, ([System.Drawing.Imaging.PixelFormat]::Format32bppArgb)
    $shadowGraphics = [System.Drawing.Graphics]::FromImage($shadow)
    Set-HighQuality $shadowGraphics
    $shadowGraphics.Clear([System.Drawing.Color]::Transparent)
    $shadowPath = New-BodyPath ($BodyOrigin / $ShadowDownscale) (($BodyOrigin + $ShadowOffsetY) / $ShadowDownscale) ($BodySize / $ShadowDownscale)
    $shadowBrush = New-Object System.Drawing.SolidBrush ([System.Drawing.Color]::FromArgb($ShadowAlpha, 0, 0, 0))
    $shadowGraphics.FillPath($shadowBrush, $shadowPath)
    $shadowGraphics.Dispose()
    $graphics.DrawImage($shadow, 0, 0, $CanvasSize, $CanvasSize)
    $shadowBrush.Dispose()
    $shadowPath.Dispose()
    $shadow.Dispose()

    # Body: the background scaled to the body and filled through an antialiased path, which a clip region is not.
    $bodyImage = New-Object System.Drawing.Bitmap $BodySize, $BodySize, ([System.Drawing.Imaging.PixelFormat]::Format32bppArgb)
    $bodyGraphics = [System.Drawing.Graphics]::FromImage($bodyImage)
    Set-HighQuality $bodyGraphics
    $attributes = New-Object System.Drawing.Imaging.ImageAttributes
    $attributes.SetWrapMode([System.Drawing.Drawing2D.WrapMode]::TileFlipXY)
    $bodyRectangle = New-Object System.Drawing.Rectangle 0, 0, $BodySize, $BodySize
    $bodyGraphics.DrawImage($background, $bodyRectangle, 0, 0, $background.Width, $background.Height, [System.Drawing.GraphicsUnit]::Pixel, $attributes)
    $bodyGraphics.Dispose()
    $attributes.Dispose()
    $bodyBrush = New-Object System.Drawing.TextureBrush $bodyImage
    $bodyBrush.TranslateTransform([single]$BodyOrigin, [single]$BodyOrigin)
    $bodyPath = New-BodyPath $BodyOrigin $BodyOrigin $BodySize
    $graphics.FillPath($bodyBrush, $bodyPath)
    $bodyBrush.Dispose()
    $bodyPath.Dispose()
    $bodyImage.Dispose()

    # Shield, centered on the body.
    $shieldSize = [int][Math]::Round($BodySize * $ShieldScale)
    $shieldOrigin = [int][Math]::Round(($CanvasSize - $shieldSize) / 2.0)
    $graphics.DrawImage($foreground, $shieldOrigin, $shieldOrigin, $shieldSize, $shieldSize)
    $graphics.Dispose()

    $outputDirectory = Split-Path -Parent $OutputPath
    if (-not (Test-Path $outputDirectory))
    {
        New-Item -ItemType Directory -Force $outputDirectory | Out-Null
    }
    $canvas.Save($OutputPath, [System.Drawing.Imaging.ImageFormat]::Png)

    $corners = @($canvas.GetPixel(0, 0), $canvas.GetPixel($CanvasSize - 1, 0), $canvas.GetPixel(0, $CanvasSize - 1), $canvas.GetPixel($CanvasSize - 1, $CanvasSize - 1))
    $transparentCorners = @($corners | Where-Object { $_.A -eq 0 }).Count
    $centerAlpha = $canvas.GetPixel($CanvasSize / 2, $CanvasSize / 2).A
    Write-Host ('New-MacAppIcon: wrote {0} ({1}x{2}, shield {3} px). Transparent corners: {4} of 4. Center alpha: {5}.' -f $OutputPath, $canvas.Width, $canvas.Height, $shieldSize, $transparentCorners, $centerAlpha)
    if ($transparentCorners -ne 4 -or $centerAlpha -ne 255)
    {
        throw 'New-MacAppIcon: the corners must be transparent and the center opaque.'
    }
}
finally
{
    $canvas.Dispose()
    $foreground.Dispose()
    $background.Dispose()
}
