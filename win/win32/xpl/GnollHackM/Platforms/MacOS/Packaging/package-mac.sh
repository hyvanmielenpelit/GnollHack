#!/bin/bash
# Assembles GnollHack.app from a single-file osx publish, signs it with the hardened runtime, and optionally
# notarizes and staples it, makes a DMG and prepares Steam depot content. Runs on the Mac; Build-MacPackage.ps1
# copies it there with LF line endings, together with the publish and the packaging files, and runs it.
# Never uploads to Steam and never starts the game.
#
#   package-mac.sh --input <dir> --output <dir> --version <x.y> --build <n> --bundle-id <id> --commit <sha>
#                  [--identity <codesign identity, - for ad hoc>] [--notary-profile <notarytool keychain profile>]
#                  [--targets app,dmg,steam] [--steam-app-id <id>] [--steam-depot-id <id>] [--steam-branch <name>]
#                  [--steam-desc <text>] [--dev-copy]
#
# <input> holds publish/ (the dotnet publish output) and packaging/ (Info.plist.template, GnollHack.entitlements,
# appicon.png, steam/*.template). Results go to <output>: the zipped app, the DMG, SHA256SUMS, BUILD-INFO,
# symbols/ and steam/.
# Errors are printed as "package-mac.sh : error GNHMP1nn: ..." and end the script with exit code 1.

set -euo pipefail

fail()
{
    echo "package-mac.sh : error $1: $2" >&2
    exit 1
}

INPUT=""
OUTPUT=""
VERSION=""
BUILD=""
BUNDLE_ID=""
COMMIT=""
IDENTITY="-"
NOTARY_PROFILE=""
TARGETS="app"
STEAM_APP_ID=""
STEAM_DEPOT_ID=""
STEAM_BRANCH=""
STEAM_DESC=""
DEV_COPY=0

while [ $# -gt 0 ]; do
    case "$1" in
    --input) INPUT="$2"; shift 2 ;;
    --output) OUTPUT="$2"; shift 2 ;;
    --version) VERSION="$2"; shift 2 ;;
    --build) BUILD="$2"; shift 2 ;;
    --bundle-id) BUNDLE_ID="$2"; shift 2 ;;
    --commit) COMMIT="$2"; shift 2 ;;
    --identity) IDENTITY="$2"; shift 2 ;;
    --notary-profile) NOTARY_PROFILE="$2"; shift 2 ;;
    --targets) TARGETS="$2"; shift 2 ;;
    --steam-app-id) STEAM_APP_ID="$2"; shift 2 ;;
    --steam-depot-id) STEAM_DEPOT_ID="$2"; shift 2 ;;
    --steam-branch) STEAM_BRANCH="$2"; shift 2 ;;
    --steam-desc) STEAM_DESC="$2"; shift 2 ;;
    --dev-copy) DEV_COPY=1; shift ;;
    *) fail GNHMP101 "unknown argument $1" ;;
    esac
done

[ -n "$INPUT" ] && [ -n "$OUTPUT" ] && [ -n "$VERSION" ] && [ -n "$BUILD" ] && [ -n "$BUNDLE_ID" ] \
    || fail GNHMP101 "--input, --output, --version, --build and --bundle-id are required"

case "$OUTPUT" in
/ | "$HOME" | "$HOME/") fail GNHMP101 "--output must be a folder of its own, not $OUTPUT" ;;
esac

PUBLISH="$INPUT/publish"
PACKAGING="$INPUT/packaging"
ENTITLEMENTS="$PACKAGING/GnollHack.entitlements"
[ -x "$PUBLISH/GnollHackM" ] || [ -f "$PUBLISH/GnollHackM" ] || fail GNHMP102 "$PUBLISH/GnollHackM is missing"
[ -f "$PACKAGING/Info.plist.template" ] || fail GNHMP102 "$PACKAGING/Info.plist.template is missing"
[ -f "$ENTITLEMENTS" ] || fail GNHMP102 "$ENTITLEMENTS is missing"

has_target()
{
    case ",$TARGETS," in
    *",$1,"*) return 0 ;;
    *) return 1 ;;
    esac
}

NOTARIZE=0
if [ -n "$NOTARY_PROFILE" ]; then
    [ "$IDENTITY" != "-" ] || fail GNHMP103 "notarization needs a Developer ID identity, not ad-hoc signing"
    NOTARIZE=1
fi

ARCH_NAME="macos-arm64"
BASE_NAME="GnollHack-$VERSION-$ARCH_NAME"
WORK="$OUTPUT/work"
APP="$WORK/GnollHack.app"
CONTENTS="$APP/Contents"
NOTARY_IDS=""

rm -rf "$OUTPUT"
mkdir -p "$CONTENTS/MacOS" "$CONTENTS/Resources" "$OUTPUT/symbols"

# --- Assemble: Mach-O files to Contents/MacOS, symbols aside, everything else to Contents/Resources ---
echo "Assembling GnollHack.app"
for entry in "$PUBLISH"/* "$PUBLISH"/.[!.]*; do
    [ -e "$entry" ] || continue
    name="$(basename "$entry")"
    if [ -d "$entry" ]; then
        ditto "$entry" "$CONTENTS/Resources/$name"
    elif [[ "$name" == *.pdb ]] || [[ "$name" == *.dSYM ]]; then
        ditto "$entry" "$OUTPUT/symbols/$name"
    elif file -b "$entry" | grep -q "Mach-O"; then
        ditto "$entry" "$CONTENTS/MacOS/$name"
    else
        ditto "$entry" "$CONTENTS/Resources/$name"
    fi
done
chmod 755 "$CONTENTS/MacOS/"*

sed -e "s/{BUNDLE_ID}/$BUNDLE_ID/g" -e "s/{VERSION}/$VERSION/g" -e "s/{BUILD}/$BUILD/g" \
    "$PACKAGING/Info.plist.template" > "$CONTENTS/Info.plist"
plutil -lint "$CONTENTS/Info.plist" > /dev/null || fail GNHMP104 "the generated Info.plist is not valid"
printf 'APPL????' > "$CONTENTS/PkgInfo"

# --- Icon: an iconset from appicon.png; the 1024 slot is scaled up when the master is smaller ---
echo "Making GnollHack.icns"
ICONSET="$WORK/GnollHack.iconset"
mkdir -p "$ICONSET"
for size in 16 32 128 256 512; do
    sips -z "$size" "$size" "$PACKAGING/appicon.png" --out "$ICONSET/icon_${size}x${size}.png" > /dev/null
    double=$((size * 2))
    sips -z "$double" "$double" "$PACKAGING/appicon.png" --out "$ICONSET/icon_${size}x${size}@2x.png" > /dev/null
done
iconutil -c icns -o "$CONTENTS/Resources/GnollHack.icns" "$ICONSET" || fail GNHMP105 "iconutil could not make the icon"

# --- Sign: every Mach-O in Contents/MacOS, the executable last, then the bundle; never --deep ---
xattr -cr "$APP"
sign()
{
    local target="$1"
    shift
    if [ "$IDENTITY" = "-" ]; then
        codesign --force --options runtime "$@" --sign - "$target"
    else
        codesign --force --timestamp --options runtime "$@" --sign "$IDENTITY" "$target"
    fi
}
echo "Signing with identity $IDENTITY"
for f in "$CONTENTS/MacOS/"*; do
    [ "$(basename "$f")" = "GnollHackM" ] && continue
    sign "$f" || fail GNHMP106 "signing $(basename "$f") failed"
done
sign "$CONTENTS/MacOS/GnollHackM" --entitlements "$ENTITLEMENTS" || fail GNHMP106 "signing GnollHackM failed"
sign "$APP" --entitlements "$ENTITLEMENTS" || fail GNHMP106 "signing the bundle failed"
codesign --verify --strict --deep --verbose=2 "$APP" || fail GNHMP107 "codesign --verify rejects the bundle"

# notarize <file> <label>: submits, waits, and fails with the log unless Accepted
notarize()
{
    local file="$1"
    local label="$2"
    local result="$WORK/notary-$label.plist"
    echo "Notarizing $label"
    xcrun notarytool submit "$file" --keychain-profile "$NOTARY_PROFILE" --wait --output-format plist > "$result" \
        || fail GNHMP108 "notarytool could not submit $label"
    local status id
    status="$(plutil -extract status raw "$result" 2>/dev/null || echo unknown)"
    id="$(plutil -extract id raw "$result" 2>/dev/null || echo unknown)"
    NOTARY_IDS="$NOTARY_IDS $label=$id"
    if [ "$status" != "Accepted" ]; then
        xcrun notarytool log "$id" --keychain-profile "$NOTARY_PROFILE" || true
        fail GNHMP109 "notarization of $label ended as $status (submission $id)"
    fi
}

if [ "$NOTARIZE" = "1" ]; then
    ditto -c -k --keepParent "$APP" "$WORK/notarize-app.zip"
    notarize "$WORK/notarize-app.zip" app
    xcrun stapler staple "$APP" || fail GNHMP110 "stapling the app failed"
    spctl --assess --type execute -vv "$APP" || fail GNHMP111 "Gatekeeper rejects the stapled app"
fi

if has_target app; then
    ditto -c -k --keepParent "$APP" "$OUTPUT/$BASE_NAME.zip"
fi

# --- DMG: the app and a link to /Applications ---
if has_target dmg; then
    echo "Making the DMG"
    STAGE="$WORK/dmg"
    mkdir -p "$STAGE"
    ditto "$APP" "$STAGE/GnollHack.app"
    ln -s /Applications "$STAGE/Applications"
    DMG="$OUTPUT/$BASE_NAME.dmg"
    hdiutil create -volname GnollHack -srcfolder "$STAGE" -ov -format UDZO "$DMG" > /dev/null \
        || fail GNHMP112 "hdiutil could not make the DMG"
    if [ "$IDENTITY" != "-" ]; then
        codesign --force --timestamp --sign "$IDENTITY" "$DMG" || fail GNHMP106 "signing the DMG failed"
    fi
    if [ "$NOTARIZE" = "1" ]; then
        notarize "$DMG" dmg
        xcrun stapler staple "$DMG" || fail GNHMP110 "stapling the DMG failed"
    fi
fi

# --- Steam: depot content and build scripts; uploading is a person's step ---
if has_target steam; then
    [ -n "$STEAM_APP_ID" ] && [ -n "$STEAM_DEPOT_ID" ] || fail GNHMP113 "the steam target needs --steam-app-id and --steam-depot-id"
    echo "Preparing Steam content"
    STEAM="$OUTPUT/steam"
    mkdir -p "$STEAM/content"
    ditto "$APP" "$STEAM/content/GnollHack.app"
    for kind in app depot; do
        sed -e "s|{APP_ID}|$STEAM_APP_ID|g" -e "s|{DEPOT_ID}|$STEAM_DEPOT_ID|g" -e "s|{BRANCH}|$STEAM_BRANCH|g" \
            -e "s|{BUILD_DESC}|$STEAM_DESC|g" -e "s|{CONTENT_ROOT}|content|g" \
            "$PACKAGING/steam/${kind}_build_mac.vdf.template" > "$STEAM/${kind}_build_mac.vdf"
    done
fi

if [ "$DEV_COPY" = "1" ]; then
    rm -rf "$HOME/gnollhack-dev/GnollHack.app"
    mkdir -p "$HOME/gnollhack-dev"
    ditto "$APP" "$HOME/gnollhack-dev/GnollHack.app"
fi

# --- Checksums and build information ---
(
    cd "$OUTPUT"
    files=""
    for f in "$BASE_NAME.zip" "$BASE_NAME.dmg"; do
        [ -f "$f" ] && files="$files $f"
    done
    if [ -n "$files" ]; then
        # shellcheck disable=SC2086
        shasum -a 256 $files > SHA256SUMS
    fi
)
{
    echo "version=$VERSION"
    echo "build=$BUILD"
    echo "bundleId=$BUNDLE_ID"
    echo "commit=$COMMIT"
    echo "identity=$IDENTITY"
    echo "notarized=$NOTARIZE"
    echo "notarySubmissions=${NOTARY_IDS# }"
    echo "targets=$TARGETS"
    echo "date=$(date -u +%Y-%m-%dT%H:%M:%SZ)"
} > "$OUTPUT/BUILD-INFO"

echo "Done: $OUTPUT"
if has_target steam; then
    echo "Upload, as a person: steamcmd +login <account> +run_app_build \"$OUTPUT/steam/app_build_mac.vdf\" +quit"
fi
