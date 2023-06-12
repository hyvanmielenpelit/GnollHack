#!/bin/bash

#Arguments
#1st argument = (REQUIRED) asset pack name, such as ondemandassetpack.zip
#2nd argument = (REQUIRED) Custom package name, such as com.soundmindentertainment.gnollhack
#3rd argument = (OPTIONAL) Android package format file, such as androidpackageformat.txt
#4th argument = (OPTIONAL) Output directory, such as /mnt/c/outputdir

ASSETPACKNAME="installtimepack.zip"
if [ $# -gt 0 ]; then
  echo "Using asset pack name $1."
  ASSETPACKNAME=$1
fi

TEMPASSETPACKNAME="temp-$1"

CUSTOMPACKAGE="com.soundmindentertainment.gnollhack"
if [ $# -gt 1 ]; then
	echo "Using CUSTOMPACKAGE=$2"
	CUSTOMPACKAGE=$2
fi

ANDROIDPACKAGEFORMATFILE=""
if [ $# -gt 2 ]; then
	echo "Using ANDROIDPACKAGEFORMATFILE=$3"
	ANDROIDPACKAGEFORMATFILE=$3
fi

OUTPUTDIR=""
if [ $# -gt 3 ]; then
	OUTPUTDIR=$4
	if [ -d "$OUTPUTDIR" ]; then
		echo "Output directory $OUTPUTDIR exists. Good."
	else
		echo "Output directory $OUTPUTDIR does not exist. Bad."
		exit 1
  	fi
fi

ANDROIDPACKAGEFORMAT=""
if [ -z "$ANDROIDPACKAGEFORMATFILE" ]; then
	echo "No ANDROIDPACKAGEFORMATFILE specified. Assuming aab. Continuing the process."
else
	echo "ANDROIDPACKAGEFORMATFILE is specified as $ANDROIDPACKAGEFORMATFILE, checking if exists."
	if test -f $ANDROIDPACKAGEFORMATFILE; then
		echo "$ANDROIDPACKAGEFORMATFILE exists, checking contents."
		ANDROIDPACKAGEFORMAT=`cat $ANDROIDPACKAGEFORMATFILE | tr -d '\n\r'`
		if [ "$ANDROIDPACKAGEFORMAT" = "aab" ]; then
			echo "Android package format is $ANDROIDPACKAGEFORMAT. Continuing the process."
		else
			echo "Android package format is $ANDROIDPACKAGEFORMAT. Quitting the process."
			exit 0
		fi
	else
		echo "$ANDROIDPACKAGEFORMATFILE does not exists. Continuing the process anyways."
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

if test -f ../$TEMPASSETPACKNAME; then
	echo "Removing existing assetpack $TEMPASSETPACKNAME."
	rm ../$TEMPASSETPACKNAME
fi

echo "Zipping assets to $TEMPASSETPACKNAME."
zip -r ../$TEMPASSETPACKNAME .

echo "Copying $TEMPASSETPACKNAME to $ASSETPACKNAME."
cp ../$TEMPASSETPACKNAME ../$ASSETPACKNAME

if [ -z "$OUTPUTDIR" ]; then
  echo "No output directory. $ASSETPACKNAME not copied anywhere."
else
  echo "Using output directory $OUTPUTDIR. Copying $ASSETPACKNAME there."
  cp ../$ASSETPACKNAME $OUTPUTDIR
fi

echo "Finished."