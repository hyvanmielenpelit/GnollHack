#!/bin/sh
# Development launcher for a Steam Deck: runs the deployed GnollHackM and writes its output to a log file.
# Deploy-SteamDeck.ps1 installs it as <deploy folder>/run-gnollhack.sh, beside gnollhack/ and VERSION.
# <deploy folder>/dev.env, when present, is sourced first; every variable it sets is exported to the game.
DEPLOY_DIR=$(dirname "$(readlink -f "$0")")
LOG_DIR="$DEPLOY_DIR/logs"
KEEP_LOGS=20

mkdir -p "$LOG_DIR" || exit 1
LOG="$LOG_DIR/gnollhack-$(date +%Y%m%d-%H%M%S).log"
: >"$LOG" || exit 1
ln -sfn "$(basename "$LOG")" "$LOG_DIR/latest.log"
# The names hold no spaces, so ls output is safe to read line by line
ls -1t "$LOG_DIR"/gnollhack-*.log 2>/dev/null | tail -n +$((KEEP_LOGS + 1)) | while IFS= read -r old; do
    rm -f "$old"
done

{
    echo "=== GnollHack development launch, $(date -Iseconds)"
    if [ -f "$DEPLOY_DIR/VERSION" ]; then
        cat "$DEPLOY_DIR/VERSION"
    fi
    echo "kernel: $(uname -r)"
    if [ -r /etc/os-release ]; then
        (. /etc/os-release; echo "os: ${PRETTY_NAME:-unknown} (${VERSION_ID:-?}, build ${BUILD_ID:-?})")
    fi
    for name in SteamGamepadUI SteamDeck SteamAppId SteamOS XDG_SESSION_TYPE XDG_CURRENT_DESKTOP WAYLAND_DISPLAY DISPLAY; do
        printf '%s=%s\n' "$name" "$(printenv "$name" || echo '<unset>')"
    done
    echo "arguments: $*"
} >"$LOG" 2>&1

if [ -f "$DEPLOY_DIR/dev.env" ]; then
    {
        echo "--- dev.env"
        cat "$DEPLOY_DIR/dev.env"
        echo "---"
    } >>"$LOG" 2>&1
    set -a
    . "$DEPLOY_DIR/dev.env"
    set +a
fi

cd "$DEPLOY_DIR/gnollhack" || exit 1
echo "=== GnollHackM output" >>"$LOG"
./GnollHackM "$@" >>"$LOG" 2>&1
status=$?
echo "=== GnollHackM exited with status $status, $(date -Iseconds)" >>"$LOG"
exit $status
