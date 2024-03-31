echo Start executing update-wsl-in-debug.bat

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

