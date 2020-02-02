
# Build instructions

These instructions are written for a 64-bit Ubuntu installation.
Modifying them for other linux distributions should be little to no
work. If you're running Windows you're on your own.


## Preparations

 - Download and extract the Android SDK tools: [https://developer.android.com/studio/index.html#command-tools]()
 - Download and extract the Android NDK (r19c or later): [https://developer.android.com/ndk/downloads]()
 - Install JDK 8. Required by Android SDK manager.
 - Install `bison` and `flex`. Used by the native nethack build.
 - Check out NetHack-Android: `git clone https://github.com/gurrhack/NetHack-Android.git`
 - Check out ForkFront-Android: `git clone https://github.com/gurrhack/ForkFront-Android.git`
 - Create a file called `local.properties` in both NetHack-Android and ForkFront-Android, containing: `sdk.dir=/path/to/android-sdk`. Used by Gradle.

### Install Android build tools
 1. Make sure JAVA_HOME points to JDK 8.
 2. `cd /path/to/android-sdk/tools/bin`
 3. Update the sdk manager: `./sdkmanager --update`. If you get "NoClassDefFoundError" it's because you're not running JDK 8.
 4. Install the platform tools: `./sdkmanager --install "platforms;android-28"`


## Build

### Build the native nethack library

1. `cd /path/to/NetHack-Android/sys/android`
2. Open `Makefile.src` and change NDK to the appropriate path.
3. `sh ./setup.sh`
4. `cd ../..`
5. `make install`

### Build the Android application

1. `cd /path/to/NetHack-Android/sys/android`
2. `./gradlew build --include-build /path/to/ForkFront-Android`
3. `cd ./build/outputs/apk/debug`
3. Copy the APK file from this directory to your device.
5. On your device: locate the APK file, install it and run!

---
Happy hacking!
