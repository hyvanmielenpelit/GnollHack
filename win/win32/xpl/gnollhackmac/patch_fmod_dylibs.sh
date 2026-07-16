#!/bin/bash
# patch_fmod_dylibs.sh — Patch FMOD macOS dylibs for Mac Catalyst
#
# Takes FMOD macOS SDK dylibs and patches their Mach-O LC_BUILD_VERSION
# platform tag from 'macos' to 'maccatalyst' using Apple's vtool.
# This is necessary because FMOD does not officially support Mac Catalyst.
#
# FMOD macOS dylibs only depend on AudioToolbox, CoreAudio, AudioUnit,
# and AVFoundation — all available on Mac Catalyst — so the patched
# libraries work at runtime.
#
# Usage: ./patch_fmod_dylibs.sh <fmod_macos_sdk_api_dir> <output_dir>
#
# Example:
#   ./patch_fmod_dylibs.sh \
#     "/path/to/FMOD Programmers API/api" \
#     "../GnollHackM/Platforms/MacCatalyst/libs"
#
# The FMOD SDK version MUST match the iOS version exactly.
# Currently: v2.03.14 (0x00020314) — see fmod.cs line 22.
#
# Prerequisites: Xcode command line tools (xcrun, vtool, install_name_tool,
#                codesign, otool, lipo)

set -euo pipefail

FMOD_API_DIR="${1:?Usage: $0 <fmod_macos_sdk_api_dir> <output_dir>}"
OUT_DIR="${2:?Usage: $0 <fmod_macos_sdk_api_dir> <output_dir>}"

# Mac Catalyst build version parameters
MIN_OS="14.0"
SDK_VER="17.0"
LD_TOOL_ID="4"
LD_TOOL_VER="19.0.0"

# Source files in the FMOD macOS SDK
CORE_SRC="$FMOD_API_DIR/core/lib/libfmod.dylib"
STUDIO_SRC="$FMOD_API_DIR/studio/lib/libfmodstudio.dylib"

# Validate source files exist
for f in "$CORE_SRC" "$STUDIO_SRC"; do
    if [ ! -f "$f" ]; then
        echo "ERROR: Source file not found: $f"
        exit 1
    fi
done

mkdir -p "$OUT_DIR"

echo "============================================"
echo "  FMOD macOS → Mac Catalyst Dylib Patcher"
echo "============================================"
echo ""

# --- Step 1: Show current platform tags ---
echo "--- Step 1: Verifying source dylib platforms ---"
echo ""
for f in "$CORE_SRC" "$STUDIO_SRC"; do
    echo "$(basename "$f"):"
    xcrun vtool -show "$f" 2>/dev/null | grep -i "platform" || echo "  (no platform info found)"
    echo ""
done

# --- Step 2: Patch platform tags ---
echo "--- Step 2: Patching platform tags to maccatalyst ---"
echo ""

CORE_OUT="$OUT_DIR/libfmod_maccatalyst.dylib"
STUDIO_OUT="$OUT_DIR/libfmodstudio_maccatalyst.dylib"

echo "Patching libfmod.dylib → libfmod_maccatalyst.dylib"
xcrun vtool \
    -set-build-version maccatalyst "$MIN_OS" "$SDK_VER" \
    -set-build-tool maccatalyst "$LD_TOOL_ID" "$LD_TOOL_VER" \
    -output "$CORE_OUT" \
    "$CORE_SRC"

echo "Patching libfmodstudio.dylib → libfmodstudio_maccatalyst.dylib"
xcrun vtool \
    -set-build-version maccatalyst "$MIN_OS" "$SDK_VER" \
    -set-build-tool maccatalyst "$LD_TOOL_ID" "$LD_TOOL_VER" \
    -output "$STUDIO_OUT" \
    "$STUDIO_SRC"
echo ""

# --- Step 3: Fix install names ---
echo "--- Step 3: Fixing install names ---"
echo ""

install_name_tool -id "@rpath/libfmod_maccatalyst.dylib" "$CORE_OUT"
echo "  libfmod_maccatalyst.dylib install name → @rpath/libfmod_maccatalyst.dylib"

install_name_tool -id "@rpath/libfmodstudio_maccatalyst.dylib" "$STUDIO_OUT"
echo "  libfmodstudio_maccatalyst.dylib install name → @rpath/libfmodstudio_maccatalyst.dylib"
echo ""

# --- Step 4: Update cross-references ---
echo "--- Step 4: Updating cross-references ---"
echo ""

# Check if libfmodstudio references libfmod and update if needed
OLD_FMOD_REF=$(otool -L "$STUDIO_OUT" | grep "libfmod\." | awk '{print $1}' || true)
if [ -n "$OLD_FMOD_REF" ]; then
    echo "  Updating libfmodstudio reference: $OLD_FMOD_REF → @rpath/libfmod_maccatalyst.dylib"
    install_name_tool -change "$OLD_FMOD_REF" "@rpath/libfmod_maccatalyst.dylib" "$STUDIO_OUT"
else
    echo "  No libfmod reference found in libfmodstudio (OK if statically linked)"
fi
echo ""

# --- Step 5: Re-sign ---
echo "--- Step 5: Re-signing (ad-hoc) ---"
echo ""

codesign --force --sign - "$CORE_OUT"
echo "  Signed: libfmod_maccatalyst.dylib"

codesign --force --sign - "$STUDIO_OUT"
echo "  Signed: libfmodstudio_maccatalyst.dylib"
echo ""

# --- Step 6: Verify ---
echo "--- Step 6: Verification ---"
echo ""

for f in "$CORE_OUT" "$STUDIO_OUT"; do
    name=$(basename "$f")
    echo "=== $name ==="
    echo "  Platform:"
    xcrun vtool -show "$f" 2>/dev/null | grep -i "platform" | sed 's/^/    /'
    echo "  Install name:"
    otool -D "$f" | tail -1 | sed 's/^/    /'
    echo "  Architectures:"
    lipo -info "$f" 2>/dev/null | sed 's/^/    /'
    echo "  Size: $(du -h "$f" | cut -f1)"
    echo ""
done

echo "  Dependencies of libfmodstudio_maccatalyst.dylib:"
otool -L "$STUDIO_OUT" | sed 's/^/    /'
echo ""

echo "============================================"
echo "  Done! Patched dylibs are in: $OUT_DIR"
echo "============================================"
