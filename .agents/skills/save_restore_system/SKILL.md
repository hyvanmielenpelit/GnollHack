---
name: save_restore_system
description: Guidelines on modifying the save/load system, which is complex and heavily relies on struct alignment and file I/O operations that can easily break cross-platform saves.
---

# Save and Restore System

## Critical Rules
- **Do NOT change struct padding/alignment**: The save files (`.sav`) are binary dumps of C structs. Changing the size or order of fields in `struct monst`, `struct obj`, or `struct you` WILL break save compatibility.
- **Version Bumping**: If you modify a core struct, you MUST increment `EDITLEVEL` in `include/patchlevel.h`. Its comment states the rule directly: change `EDITLEVEL` when the changes to the game are save-file-breaking.
- **Use `bwrite`/`mread`**: Never use standard `fwrite`/`fread` for game state. Use the engine's wrappers.

## How Saving Works (`src/save.c`)
1. **`dosave()`**: Entry point. Checks if saving is permitted.
2. **File Creation**: Creates a file named after the player's ID and role.
3. **State Dumping**: Writes `u` (player struct), `invent` (inventory), `fmon` (monsters), and the current `level` struct.
4. **Compression**: The save file is often compressed depending on platform configuration.

## How Restoring Works (`src/restore.c`)
1. **`dorestore()`**: Entry point (`src/restore.c`). There is no `dorecover()` — that name does not exist in GnollHack.
2. **Version Check**: Compares the save file's version struct with the compiled binary's version. If they mismatch, restoration fails.
3. **State Loading**: Reconstructs pointers. Pointers in the binary dump are invalid; the restore code runs an address-fixup pass to re-link linked lists (`nmon`, `nobj`).

## Level Files and the Instance Lock

While a game runs, each visited level lives in its own file, `<lock>.<ledger>`, beside level 0,
which holds the checkpoint. In the app builds (`GNH_MOBILE`) `<lock>` is the user ID and
character name, with no process ID, so two processes playing one character would share every
file. Level 0 and every level file start with the writing process's ID (`hackpid`, the real
`getpid()`). It is compared only with files this process wrote in the same game
(`savestateinlock()`, and `getlev()` from `goto_level()` and `dosave0()`); recovery, restoring
from a save and bones ignore it, so a restarted process can always recover. Older builds stored
the constant 1, inherited from Gurr's Android port, so a level 0 holding 1 is from such a build.

`getlock()` therefore takes a per-character instance lock (`acquire_game_instance_lock()`: a
named mutex on Windows, `flock` on Linux, none on Android and iOS) and refuses the start when
another process holds it. `check_crash()` does not offer to recover a game that is still
running, and the saved-games menu does not delete one (`game_in_progress_elsewhere()`).
`gnollhack_exit()` releases the lock. The global `lock` keeps the previous game's name until
`getlock()` sets it, so `clearlocks()` does nothing until `getlock()` has set
`program_state.levelfiles_claimed`. Any new code that deletes level files before that point must
check `game_in_progress_elsewhere()` first.

## Modifying State
- If you add a new field to a struct, add it to the END of the struct to minimize padding shifts.
- If the new field contains pointers, you MUST add corresponding fixup logic in `restore.c`.
