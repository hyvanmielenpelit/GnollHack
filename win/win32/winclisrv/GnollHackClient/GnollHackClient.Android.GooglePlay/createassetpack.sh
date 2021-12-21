#!/bin/bash

#Arguments
#1st argument = (REQUIRED) asset pack name, such as assets.zip
#2nd argument = (REQUIRED) Custom package name, such as com.soundmindentertainment.gnollhack
#3rd argument = (OPTIONAL) Output directory, such as /mnt/c/outputdir

ASSETPACKNAME="assets.zip"
if [ $# -gt 0 ]; then
  echo "Using asset pack name $1."
  ASSETPACKNAME=$1
fi

CUSTOMPACKAGE="com.soundmindentertainment.gnollhack"
if [ $# -gt 1 ]; then
	echo "Using CUSTOMPACKAGE=$2"
	CUSTOMPACKAGE=$2
fi

if [ $# -gt 2 ]; then
	if [ -d "$3" ]; then
		echo "Output directory $3 exists. Good."
	else
		echo "Output directory $3 does not exist. Bad."
		exit 1
  	fi
fi

if [ -d "aapt2output" ]; then
	echo "Removing existing aapt2ouput directory."
	rm -r aapt2output
fi

echo "Creating aapt2ouput directory."
mkdir aapt2output

echo "Using aapt2 to create resources"
./aapt2 link --manifest AndroidManifest.xml --proto-format --output-to-dir --custom-package $CUSTOMPACKAGE -A Assets -o aapt2output

cd aapt2output

echo "Moving AndroidManifest.xml to manifest directory."
mkdir manifest
mv ./AndroidManifest.xml manifest

echo "Removing resources.pb file."
rm ./resources.pb

if test -f ../$ASSETPACKNAME; then
	echo "Removing existing assetpack $ASSETPACKNAME."
	rm ../$ASSETPACKNAME
fi

echo "Zipping assets to $ASSETPACKNAME."
zip -r ../$ASSETPACKNAME .

if [ $# -gt 2 ]; then
  echo "Using output directory $3. Copying $ASSETPACKNAME there."
  cp ../$ASSETPACKNAME $3
else
  echo "No output directory. $ASSETPACKNAME not copied anywhere."
fi

echo "Finished."