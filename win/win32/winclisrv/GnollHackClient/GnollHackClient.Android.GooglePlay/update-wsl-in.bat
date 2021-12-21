if not exist c:\wsl-in mkdir c:\wsl-in
if not exist c:\wsl-in\assetpack mkdir c:\wsl-in\assetpack
if not exist c:\wsl-in\assetpack\Assets mkdir c:\wsl-in\assetpack\Assets
if not exist c:\wsl-in\assetpack\aapt2output mkdir c:\wsl-in\assetpack\aapt2output
copy ..\bank\Mobile\*.* c:\wsl-in\assetpack\Assets
copy ..\..\..\ext\Android\aapt2 c:\wsl-in\assetpack
copy ..\winclisrv\GnollHackClient\GnollHackClient.Android.GooglePlay\AndroidManifest.xml c:\wsl-in\assetpack
copy ..\winclisrv\GnollHackClient\GnollHackClient.Android.GooglePlay\createassetpack.sh c:\wsl-in\assetpack