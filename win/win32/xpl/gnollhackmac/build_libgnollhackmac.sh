#!/bin/bash
# build_libgnollhackmac.sh — Build GnollHack C engine for Mac Catalyst (universal)
#
# Creates a universal (arm64 + x86_64) static library for Mac Catalyst.
# This is analogous to libgnollhackios.a but targets the macabi platform triple.
#
# The source file list is derived from gnollhackios.vcxproj (VS remote build project).
# The preprocessor defines match the ARM64 Release configuration.
#
# Usage: ./build_libgnollhackmac.sh <gnollhack_root> [Debug|Release]
#
# Example:
#   ./build_libgnollhackmac.sh ~/vcremote/C/hmp/GnollHack Release
#
# Prerequisites: Xcode command line tools (clang, ar, lipo)

set -euo pipefail

GNOLLHACK_ROOT="${1:?Usage: $0 <gnollhack_root> [Debug|Release]}"
CONFIG="${2:-Release}"
OUTPUT_DIR="$GNOLLHACK_ROOT/win/win32/xpl/GnollHackM/Platforms/MacCatalyst/libs"
SDK_PATH=$(xcrun --sdk macosx --show-sdk-path)

# Preprocessor defines (matching gnollhackios.vcxproj ARM64 Release/Debug)
COMMON_DEFINES="-DGNH_MOBILE -DGNH_IOS -DUSE_TILES -DDLB -DLIB_GRAPHICS \
    -DSAFEPROCS -DNOTTYGRAPHICS -DGNOLLHACK_MAIN_PROGRAM \
    -DEXIT_THREAD_ON_EXIT -DDUMPLOG -DDUMPHTML"

if [ "$CONFIG" = "Debug" ]; then
    OPT_FLAGS="-O0 -g"
    EXTRA_DEFINES="-D_DEBUG -DDEBUG"
else
    OPT_FLAGS="-O2"
    EXTRA_DEFINES=""
fi

# Include paths
INCLUDE_FLAGS="-I$GNOLLHACK_ROOT/include \
    -I$GNOLLHACK_ROOT/win/win32/xpl/gnollhackios \
    -I$GNOLLHACK_ROOT/win/win32/xpl/libshare"

# Common compiler flags
COMMON_FLAGS="-isysroot $SDK_PATH \
    -iframework $SDK_PATH/System/iOSSupport/System/Library/Frameworks \
    $COMMON_DEFINES $EXTRA_DEFINES $INCLUDE_FLAGS $OPT_FLAGS \
    -Wno-deprecated-declarations"

# Directory shortcuts (matching vcxproj variables)
SRC_DIR="$GNOLLHACK_ROOT/src"
SYS_SHARE_DIR="$GNOLLHACK_ROOT/sys/share"
WIN_SHARE_DIR="$GNOLLHACK_ROOT/win/share"
LIB_SHARE_DIR="$GNOLLHACK_ROOT/win/win32/xpl/libshare"
LIB_IOS_DIR="$GNOLLHACK_ROOT/win/win32/xpl/gnollhackios"

# Source files — exact list from gnollhackios.vcxproj lines 245-373
# All src/*.c files
SRC_FILES=$(find "$SRC_DIR" -maxdepth 1 -name "*.c" -type f | sort)

# sys/share files
SYS_SHARE_FILES="$SYS_SHARE_DIR/ioctl.c
$SYS_SHARE_DIR/posixregex.c
$SYS_SHARE_DIR/unixtty.c"

# win/share files
WIN_SHARE_FILES="$WIN_SHARE_DIR/safeproc.c"

# libshare files
LIB_SHARE_FILES="$LIB_SHARE_DIR/libproc.c
$LIB_SHARE_DIR/callback.c
$LIB_SHARE_DIR/gnhapi.c
$LIB_SHARE_DIR/gnhmain.c
$LIB_SHARE_DIR/gnhunix.c"

# iOS-specific files
LIB_IOS_FILES="$LIB_IOS_DIR/gnollhackios.c"

# Combine all sources
ALL_SOURCES="$SRC_FILES
$SYS_SHARE_FILES
$WIN_SHARE_FILES
$LIB_SHARE_FILES
$LIB_IOS_FILES"

mkdir -p "$OUTPUT_DIR"

echo "============================================"
echo "  GnollHack Mac Catalyst C Engine Builder"
echo "============================================"
echo "  Config:  $CONFIG"
echo "  SDK:     $SDK_PATH"
echo "  Output:  $OUTPUT_DIR"
echo ""

# Count source files
FILE_COUNT=$(echo "$ALL_SOURCES" | wc -l | tr -d ' ')
echo "  Sources: $FILE_COUNT files"
echo ""

# Build for each architecture
for ARCH in arm64 x86_64; do
    echo "=== Building for $ARCH ==="
    OBJDIR=$(mktemp -d)
    COMPILED=0

    for src in $ALL_SOURCES; do
        if [ ! -f "$src" ]; then
            echo "  WARNING: Source file not found, skipping: $src"
            continue
        fi
        base=$(basename "$src" .c)
        clang -target ${ARCH}-apple-ios14.0-macabi \
              $COMMON_FLAGS \
              -c "$src" -o "$OBJDIR/$base.o" 2>&1 | head -5
        COMPILED=$((COMPILED + 1))
    done

    echo "  Compiled $COMPILED files for $ARCH"
    ar rcs "$OUTPUT_DIR/libgnollhackmac_${ARCH}.a" "$OBJDIR"/*.o
    echo "  Archived: libgnollhackmac_${ARCH}.a"
    rm -rf "$OBJDIR"
    echo ""
done

# Create universal fat library
echo "=== Creating universal library ==="
lipo -create \
    "$OUTPUT_DIR/libgnollhackmac_arm64.a" \
    "$OUTPUT_DIR/libgnollhackmac_x86_64.a" \
    -output "$OUTPUT_DIR/libgnollhackmac.a"

# Clean up single-arch files
rm "$OUTPUT_DIR/libgnollhackmac_arm64.a" "$OUTPUT_DIR/libgnollhackmac_x86_64.a"

echo ""
echo "=== Verification ==="
echo "  File: $OUTPUT_DIR/libgnollhackmac.a"
file "$OUTPUT_DIR/libgnollhackmac.a"
lipo -info "$OUTPUT_DIR/libgnollhackmac.a"
echo "  Size: $(du -h "$OUTPUT_DIR/libgnollhackmac.a" | cut -f1)"
echo ""
echo "============================================"
echo "  Done! Library: $OUTPUT_DIR/libgnollhackmac.a"
echo "============================================"
