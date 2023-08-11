echo Start executing copy-assetpack.bat

if not exist ..\xpl\GnollHackX\GnollHackX.Android\assetpacks mkdir ..\xpl\GnollHackX\GnollHackX.Android\assetpacks
copy c:\wsl-in\assetpack\installtimeassetpack.zip ..\xpl\GnollHackX\GnollHackX.Android\assetpacks
copy c:\wsl-in\assetpack2\ondemandassetpack.zip ..\xpl\GnollHackX\GnollHackX.Android\assetpacks