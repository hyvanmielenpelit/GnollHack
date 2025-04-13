echo Executing update-wsl-in-debug.bat

echo Preparing GnollHack tilesets
copy ..\tileset\gnollhack_64x96_transparent_32bits.png ..\tileset\gnollhack_64x96_transparent_32bits.ghpng
copy ..\tileset\gnollhack_64x96_transparent_32bits-2.png ..\tileset\gnollhack_64x96_transparent_32bits-2.ghpng

echo Copying tileset to c:\wsl-in\assetpack\Assets\tileset
copy ..\tileset\*.ghpng c:\wsl-in\assetpack\Assets\tileset

echo Copying GnollHackX banks
if not exist ..\xpl\GnollHackX\GnollHackX.Android\banks mkdir ..\xpl\GnollHackX\GnollHackX.Android\banks
if not exist ..\xpl\GnollHackX\GnollHackX.iOS\banks mkdir ..\xpl\GnollHackX\GnollHackX.iOS\banks
del /Q ..\xpl\GnollHackX\GnollHackX.Android\banks\*.*
del /Q ..\xpl\GnollHackX\GnollHackX.iOS\banks\*.*
echo Copying banks to X Android banks
copy ..\bank\Mobile\*.* ..\xpl\GnollHackX\GnollHackX.Android\banks
echo Copying banks to X iOS banks
copy ..\bank\Mobile\*.* ..\xpl\GnollHackX\GnollHackX.iOS\banks

echo Copying GnollHackX tileset
if not exist ..\xpl\GnollHackX\GnollHackX.Android\tileset mkdir ..\xpl\GnollHackX\GnollHackX.Android\tileset
if not exist ..\xpl\GnollHackX\GnollHackX.iOS\tileset mkdir ..\xpl\GnollHackX\GnollHackX.iOS\tileset
del /Q ..\xpl\GnollHackX\GnollHackX.Android\tileset\*.*
del /Q ..\xpl\GnollHackX\GnollHackX.iOS\tileset\*.*
echo Copying tileset to X Android tileset
copy ..\tileset\*.ghpng ..\xpl\GnollHackX\GnollHackX.Android\tileset
echo Copying tileset to X iOS tileset
copy ..\tileset\*.ghpng ..\xpl\GnollHackX\GnollHackX.iOS\tileset

echo Copying GnollHackM banks
if not exist ..\xpl\GnollHackM\Platforms\Android\banks mkdir ..\xpl\GnollHackM\Platforms\Android\banks
if not exist ..\xpl\GnollHackM\Platforms\iOS\banks mkdir ..\xpl\GnollHackM\Platforms\iOS\banks
if not exist ..\xpl\GnollHackM\Platforms\Windows\banks mkdir ..\xpl\GnollHackM\Platforms\Windows\banks
del /Q ..\xpl\GnollHackM\Platforms\Android\banks\*.*
del /Q ..\xpl\GnollHackM\Platforms\iOS\banks\*.*
del /Q ..\xpl\GnollHackM\Platforms\Windows\banks\*.*
echo Copying banks to M Android banks
copy ..\bank\Mobile\*.* ..\xpl\GnollHackM\Platforms\Android\banks
echo Copying banks to M iOS banks
copy ..\bank\Mobile\*.* ..\xpl\GnollHackM\Platforms\iOS\banks
echo Copying banks to M Windows banks
copy ..\bank\Desktop\*.* ..\xpl\GnollHackM\Platforms\Windows\banks

echo Copying GnollHackM tileset
if not exist ..\xpl\GnollHackM\Platforms\Android\tileset mkdir ..\xpl\GnollHackM\Platforms\Android\tileset
if not exist ..\xpl\GnollHackM\Platforms\iOS\tileset mkdir ..\xpl\GnollHackM\Platforms\iOS\tileset
if not exist ..\xpl\GnollHackM\Platforms\Windows\tileset mkdir ..\xpl\GnollHackM\Platforms\Windows\tileset
del /Q ..\xpl\GnollHackM\Platforms\Android\tileset\*.*
del /Q ..\xpl\GnollHackM\Platforms\iOS\tileset\*.*
del /Q ..\xpl\GnollHackM\Platforms\Windows\tileset\*.*
echo Copying tileset to M Android tileset
copy ..\tileset\*.ghpng ..\xpl\GnollHackM\Platforms\Android\tileset
echo Copying tileset to M iOS tileset
copy ..\tileset\*.ghpng ..\xpl\GnollHackM\Platforms\iOS\tileset
echo Copying tileset to M Windows tileset
copy ..\tileset\*.ghpng ..\xpl\GnollHackM\Platforms\Windows\tileset

del /Q ..\tileset\*.ghpng
