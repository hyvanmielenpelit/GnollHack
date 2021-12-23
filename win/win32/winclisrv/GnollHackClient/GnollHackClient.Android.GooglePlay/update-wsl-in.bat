if not exist c:\wsl-in mkdir c:\wsl-in
if not exist c:\wsl-in\assetpack mkdir c:\wsl-in\assetpack
if not exist c:\wsl-in\assetpack\Assets mkdir c:\wsl-in\assetpack\Assets
del /Q c:\wsl-in\assetpack\Assets\*.*
if not exist c:\wsl-in\assetpack\Assets\banks mkdir c:\wsl-in\assetpack\Assets\banks
del /Q c:\wsl-in\assetpack\Assets\banks\*.*
if not exist c:\wsl-in\assetpack\aapt2output mkdir c:\wsl-in\assetpack\aapt2output
if not exist ..\winclisrv\GnollHackClient\GnollHackClient.Android\banks mkdir ..\winclisrv\GnollHackClient\GnollHackClient.Android\banks
copy ..\bank\Mobile\*.* c:\wsl-in\assetpack\Assets\banks
del /Q ..\bank\Mobile\*.* ..\winclisrv\GnollHackClient.Android\banks\*.*
copy ..\bank\Mobile\*.* ..\winclisrv\GnollHackClient\GnollHackClient.Android\banks
copy ..\..\..\ext\android\aapt2 c:\wsl-in\assetpack