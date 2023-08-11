echo Start executing update-wsl-in.bat

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
echo Copying banks to c:\wsl-in\assetpack2\Assets\banks
copy ..\xpl\GnollHackX\GnollHackX.Android.GooglePlay\OnDemand\AboutAssetPack.txt c:\wsl-in\assetpack2\Assets

if not exist ..\xpl\GnollHackX\GnollHackX.Android\banks mkdir ..\xpl\GnollHackX\GnollHackX.Android\banks
if not exist ..\xpl\GnollHackX\GnollHackX.iOS\banks mkdir ..\xpl\GnollHackX\GnollHackX.iOS\banks
del /Q ..\xpl\GnollHackX\GnollHackX.Android\banks\*.*
del /Q ..\xpl\GnollHackX\GnollHackX.iOS\banks\*.*
echo Copying banks to Android banks
copy ..\bank\Mobile\*.* ..\xpl\GnollHackX\GnollHackX.Android\banks
echo Copying banks to iOS banks
copy ..\bank\Mobile\*.* ..\xpl\GnollHackX\GnollHackX.iOS\banks

echo Copying aapt2 to wsl-in\assetpack
copy ..\xpl\GnollHackX\GnollHackX.Android.GooglePlay\aapt2 c:\wsl-in\assetpack
echo Copying aapt2 to wsl-in\assetpack2
copy ..\xpl\GnollHackX\GnollHackX.Android.GooglePlay\aapt2 c:\wsl-in\assetpack2
