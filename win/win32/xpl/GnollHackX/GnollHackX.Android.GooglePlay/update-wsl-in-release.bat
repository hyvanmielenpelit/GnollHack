echo Start executing update-wsl-in-debug.bat

if not exist c:\wsl-in mkdir c:\wsl-in

if not exist c:\wsl-in\assetpack mkdir c:\wsl-in\assetpack
if not exist c:\wsl-in\assetpack\Assets mkdir c:\wsl-in\assetpack\Assets
del /Q c:\wsl-in\assetpack\Assets\*.*
if not exist c:\wsl-in\assetpack\Assets\banks mkdir c:\wsl-in\assetpack\Assets\banks
del /Q c:\wsl-in\assetpack\Assets\banks\*.*
if not exist c:\wsl-in\assetpack\aapt2output mkdir c:\wsl-in\assetpack\aapt2output
echo Copying banks to c:\wsl-in\assetpack\Assets\banks
copy ..\bank\Mobile\Master.bank c:\wsl-in\assetpack\Assets\banks
copy ..\bank\Mobile\Master.strings.bank c:\wsl-in\assetpack\Assets\banks
copy ..\bank\Mobile\Music.bank c:\wsl-in\assetpack\Assets\banks
copy ..\bank\Mobile\Preliminary.bank c:\wsl-in\assetpack\Assets\banks
copy ..\bank\Mobile\Auxiliary.bank c:\wsl-in\assetpack\Assets\banks
copy ..\bank\Mobile\Intro.bank c:\wsl-in\assetpack\Assets\banks

if not exist c:\wsl-in\assetpack2 mkdir c:\wsl-in\assetpack2
if not exist c:\wsl-in\assetpack2\Assets mkdir c:\wsl-in\assetpack2\Assets
del /Q c:\wsl-in\assetpack2\Assets\*.*
if not exist c:\wsl-in\assetpack2\Assets\banks mkdir c:\wsl-in\assetpack2\Assets\banks
del /Q c:\wsl-in\assetpack2\Assets\banks\*.*
if not exist c:\wsl-in\assetpack2\aapt2output mkdir c:\wsl-in\assetpack2\aapt2output
echo Copying text file to c:\wsl-in\assetpack2\Assets\banks
copy ..\xpl\GnollHackX\GnollHackX.Android.GooglePlay\OnDemand\AboutAssetPack.txt c:\wsl-in\assetpack2\Assets

rem if not exist c:\wsl-in\assetpack-maui mkdir c:\wsl-in\assetpack-maui
rem if not exist c:\wsl-in\assetpack-maui\Assets mkdir c:\wsl-in\assetpack-maui\Assets
rem del /Q c:\wsl-in\assetpack-maui\Assets\*.*
rem if not exist c:\wsl-in\assetpack-maui\Assets\banks mkdir c:\wsl-in\assetpack-maui\Assets\banks
rem del /Q c:\wsl-in\assetpack-maui\Assets\banks\*.*
rem if not exist c:\wsl-in\assetpack-maui\aapt2output mkdir c:\wsl-in\assetpack-maui\aapt2output
rem echo Copying banks to c:\wsl-in\assetpack-maui\Assets\banks
rem copy ..\bank\Mobile\Master.bank c:\wsl-in\assetpack-maui\Assets\banks
rem copy ..\bank\Mobile\Master.strings.bank c:\wsl-in\assetpack-maui\Assets\banks
rem copy ..\bank\Mobile\Music.bank c:\wsl-in\assetpack-maui\Assets\banks
rem copy ..\bank\Mobile\Preliminary.bank c:\wsl-in\assetpack-maui\Assets\banks
rem copy ..\bank\Mobile\Auxiliary.bank c:\wsl-in\assetpack-maui\Assets\banks
rem copy ..\bank\Mobile\Intro.bank c:\wsl-in\assetpack-maui\Assets\banks

rem if not exist c:\wsl-in\assetpack-maui2 mkdir c:\wsl-in\assetpack-maui2
rem if not exist c:\wsl-in\assetpack-maui2\Assets mkdir c:\wsl-in\assetpack-maui2\Assets
rem del /Q c:\wsl-in\assetpack-maui2\Assets\*.*
rem if not exist c:\wsl-in\assetpack-maui2\Assets\banks mkdir c:\wsl-in\assetpack-maui2\Assets\banks
rem del /Q c:\wsl-in\assetpack-maui2\Assets\banks\*.*
rem if not exist c:\wsl-in\assetpack-maui2\aapt2output mkdir c:\wsl-in\assetpack-maui2\aapt2output
rem echo Copying text file to c:\wsl-in\assetpack-maui2\Assets\banks
rem copy ..\xpl\GnollHackX\GnollHackX.Android.GooglePlay\OnDemand-Maui\AboutAssetPack.txt c:\wsl-in\assetpack-maui2\Assets

echo Start copying GnollHackX banks
if not exist ..\xpl\GnollHackX\GnollHackX.Android\banks mkdir ..\xpl\GnollHackX\GnollHackX.Android\banks
if not exist ..\xpl\GnollHackX\GnollHackX.iOS\banks mkdir ..\xpl\GnollHackX\GnollHackX.iOS\banks
del /Q ..\xpl\GnollHackX\GnollHackX.Android\banks\*.*
del /Q ..\xpl\GnollHackX\GnollHackX.iOS\banks\*.*
echo Copying banks to X Android banks
copy ..\bank\Mobile\*.* ..\xpl\GnollHackX\GnollHackX.Android\banks
echo Copying banks to X iOS banks
copy ..\bank\Mobile\*.* ..\xpl\GnollHackX\GnollHackX.iOS\banks

echo Start copying GnollHackM banks
if not exist ..\xpl\GnollHackM\Platforms\Android\banks mkdir ..\xpl\GnollHackM\Platforms\Android\banks
if not exist ..\xpl\GnollHackM\Platforms\iOS\banks mkdir ..\xpl\GnollHackM\Platforms\iOS\banks
del /Q ..\xpl\GnollHackM\Platforms\Android\banks\*.*
del /Q ..\xpl\GnollHackM\Platforms\iOS\banks\*.*
echo Copying banks to M Android banks
copy ..\bank\Mobile\*.* ..\xpl\GnollHackM\Platforms\Android\banks
echo Copying banks to M iOS banks
copy ..\bank\Mobile\*.* ..\xpl\GnollHackM\Platforms\iOS\banks

echo Copying aapt2 to wsl-in\assetpack
copy ..\xpl\GnollHackX\GnollHackX.Android.GooglePlay\aapt2 c:\wsl-in\assetpack
echo Copying aapt2 to wsl-in\assetpack2
copy ..\xpl\GnollHackX\GnollHackX.Android.GooglePlay\aapt2 c:\wsl-in\assetpack2
rem echo Copying aapt2 to wsl-in\assetpack-maui
rem copy ..\xpl\GnollHackX\GnollHackX.Android.GooglePlay\aapt2 c:\wsl-in\assetpack-maui
rem echo Copying aapt2 to wsl-in\assetpack-maui2
rem copy ..\xpl\GnollHackX\GnollHackX.Android.GooglePlay\aapt2 c:\wsl-in\assetpack-maui2
