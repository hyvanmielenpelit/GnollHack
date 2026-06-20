---
name: save_restore_system
description: Guidelines on modifying the save/load system, which is complex and heavily relies on struct alignment and file I/O operations that can easily break cross-platform saves.
---

# Save and Restore System

## Critical Rules
- **Do NOT change struct padding/alignment**: The save files (`.sav`) are binary dumps of C structs. Changing the size or order of fields in `struct monst`, `struct obj`, or `struct you` WILL break save compatibility.
- **Version Bumping**: If you modify a core struct, you MUST increment the save file version in `patchlevel.h`.
- **Use `bwrite`/`mread`**: Never use standard `fwrite`/`fread` for game state. Use the engine's wrappers.

## How Saving Works (`src/save.c`)
1. **`dosave()`**: Entry point. Checks if saving is permitted.
2. **File Creation**: Creates a file named after the player's ID and role.
3. **State Dumping**: Writes `u` (player struct), `invent` (inventory), `fmon` (monsters), and the current `level` struct.
4. **Compression**: The save file is often compressed depending on platform configuration.

## How Restoring Works (`src/restore.c`)
1. **`dorecover()`**: Entry point.
2. **Version Check**: Compares the save file's version struct with the compiled binary's version. If they mismatch, restoration fails.
3. **State Loading**: Reconstructs pointers. Pointers in the binary dump are invalid; the restore code runs an address-fixup pass to re-link linked lists (`nmon`, `nobj`).

## Modifying State
- If you add a new field to a struct, add it to the END of the struct to minimize padding shifts.
- If the new field contains pointers, you MUST add corresponding fixup logic in `restore.c`.
