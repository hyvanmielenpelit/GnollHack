#!/bin/sh
# Makes <tree> an exact copy of <incoming>, rewriting only the files whose content differs.
# A rewritten file gets the Mac's current time, so make rebuilds what changed and nothing else, whatever times the
# files carried from Windows. Run on the Mac by Build-AppleEngine.ps1: sh sync-tree.sh <incoming> <tree>
set -e
incoming=$(cd "$1" && pwd)
mkdir -p "$2"
tree=$(cd "$2" && pwd)
changed=$(mktemp)
trap 'rm -f "$changed"' EXIT

(cd "$incoming" && find . -type f) | while IFS= read -r f
do
    if ! cmp -s "$incoming/$f" "$tree/$f"
    then
        printf '%s\n' "$f" >> "$changed"
    fi
done

# WORKAROUND(WA-BUILD-0005): Xcode's GNU make 3.81 compares file times in whole seconds
# A file copied in the same second as an object the previous build compiled would look up to date
if [ -s "$changed" ]
then
    sleep 1
fi
while IFS= read -r f
do
    mkdir -p "$(dirname "$tree/$f")"
    cp "$incoming/$f" "$tree/$f"
done < "$changed"

(cd "$tree" && find . -type f) | while IFS= read -r f
do
    if [ ! -e "$incoming/$f" ]
    then
        rm -f "$tree/$f"
    fi
done
