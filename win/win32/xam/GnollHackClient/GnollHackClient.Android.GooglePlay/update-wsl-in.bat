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
copy ..\xam\GnollHackClient\GnollHackClient.Android.GooglePlay\OnDemand\AboutAssetPack.txt c:\wsl-in\assetpack2\Assets

if not exist ..\xam\GnollHackClient\GnollHackClient.Android\banks mkdir ..\xam\GnollHackClient\GnollHackClient.Android\banks
if not exist ..\xam\GnollHackClient\GnollHackClient.iOS\banks mkdir ..\xam\GnollHackClient\GnollHackClient.iOS\banks
del /Q ..\xam\GnollHackClient\GnollHackClient.Android\banks\*.*
del /Q ..\xam\GnollHackClient\GnollHackClient.iOS\banks\*.*
echo Copying banks to Android banks
copy ..\bank\Mobile\*.* ..\xam\GnollHackClient\GnollHackClient.Android\banks
echo Copying banks to iOS banks
copy ..\bank\Mobile\*.* ..\xam\GnollHackClient\GnollHackClient.iOS\banks

echo Copying aapt2 to wsl-in\assetpack
copy ..\xam\GnollHackClient\GnollHackClient.Android.GooglePlay\aapt2 c:\wsl-in\assetpack
echo Copying aapt2 to wsl-in\assetpack2
copy ..\xam\GnollHackClient\GnollHackClient.Android.GooglePlay\aapt2 c:\wsl-in\assetpack2
