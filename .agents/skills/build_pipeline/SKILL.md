---
name: build_pipeline
description: Guide on building the GnollHack native C libraries, processing assets with makedefs/levcomp/dgncomp, and compiling the C# .NET MAUI frontend.
---

# GnollHack Build and Compilation Pipeline

This guide outlines the compilation phases of GnollHack, from compiling the native C backend to preparing binary game assets and building the C# frontend clients.

## Compilation Phases Overview

The GnollHack build process runs in three main logical steps:
1. **Utility Code Compilation**: Building tools used to compile raw text data into game database assets.
2. **Asset Pre-processing**: Running the utility tools on source data files to output binary runtime databases.
3. **App & Library Building**: Compiling the native core library and linking it with the .NET application frontend.

---

## 1. Visual Studio Solution & Projects

All C/C++ native targets and pre-processing tasks are configured in:
* **[win/win32/vs/GnollHack.sln](file:///c:/hmp/GnollHack/win/win32/vs/GnollHack.sln)**

### Utility Projects
Before compiling the game library, the following utility console applications are built:
* **makedefs**: Builds definitions from data files (monsters, objects, etc.) and generates configuration header files (like `pm.h`, `onames.h`).
* **levcomp**: The level compiler (`lev_comp`). Translates human-readable level descriptions in `dat/` into binary `.lev` dungeon files.
* **dgncomp**: The dungeon compiler (`dgn_comp`). Translates dungeon layout maps in `dat/` into binary `.dgn` files.
* **dlb**: NetHack's database library packager. Bundles individual compiled files into virtual directory container files.

### Game Library Projects
* **gnollhackwin**: Compiles the native engine as a Windows DLL (`gnollhack.dll`).
* **gnollhackdroid**: Compiles the native engine as a shared object library (`libgnollhack.so`) for Android architectures.
* **gnollhackios**: Compiles the native engine as a static archive library (`libgnollhack.a`) for iOS deployment.

---

## 2. Asset Pre-processing Pipeline

Raw files inside **[dat/](file:///c:/hmp/GnollHack/dat)** (such as dungeon descriptions, rumors, quest text files) must be converted into compiled structures:
* Pre-build scripts or Visual Studio MSBuild targets invoke the compiled utilities.
* Example: `lev_comp` reads level description scripts and generates compiled layout files.
* Example: `makedefs -d` compiles main descriptions database.
* The output files are stored under `binary/` or packed into dynamic package containers using `dlb`.

---

## 3. Compiling the C# Frontend (`GnollHackM`)

Once the native libraries and compiled assets are built, the .NET MAUI client can be compiled.

### Using the .NET CLI
Run commands from the repository root:
* **Windows Target**:
  ```powershell
  dotnet build win/win32/xpl/GnollHackM/GnollHackM.csproj -c Debug -f net9.0-windows10.0.19041.0
  ```
* **Android Target**:
  ```powershell
  dotnet build win/win32/xpl/GnollHackM/GnollHackM.csproj -c Debug -f net9.0-android
  ```
* **iOS Target**:
  ```powershell
  dotnet build win/win32/xpl/GnollHackM/GnollHackM.csproj -c Debug -f net9.0-ios
  ```

### Build Script Configuration
You can also trigger a complete build of the solution configurations using:
* **[win/win32/vs/build.bat](file:///c:/hmp/GnollHack/win/win32/vs/build.bat)**
Ensure the Visual Studio environment is loaded (e.g. through the Developer Command Prompt or via `VS150COMNTOOLS` environment variable).
