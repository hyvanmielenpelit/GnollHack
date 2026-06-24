---
name: bitfield_conversion
description: Guidelines and patterns for converting non-portable C Bitfield macros into platform-portable explicit bitmask flags to fix cross-platform save game compatibility.
---

# Converting Bitfield Macros to Platform-Portable Code

## The Problem
GnollHack's legacy C code uses a `Bitfield(x, n)` macro (expanding to `unsigned x : n` or `uchar x`) extensively in core data structures like `struct monst`, `struct obj`, and `struct you`.

**C bitfields are not portable.** Their memory layout (padding, alignment, and bit-endianness) is implementation-defined. Because GnollHack's save system works by dumping these structs directly into binary save files (`.sav`), a save file generated on an x64 Windows machine might have a completely different binary layout for bitfields compared to an ARM64 Android device. This breaks cross-platform save compatibility.

## The Solution
Convert bitfields into `uint64_t` variables and use explicit bitwise operations (`|`, `&`, `~`) with defined masks.

### Example: `mon_bitflags`
An excellent pattern is to use a variable named like `mon_bitflags` (or `bitfields` if the context is clear). Use `uint64_t` to allow for up to 64 flags in a single variable.

```c
// include/monst.h
struct monst {
    // ...
    uint64_t mon_bitflags;
#define MON_BITFLAG_NONE                          0x00000000UL
#define MON_BITFLAG_RWRAITH                       0x00000001UL
#define MON_BITFLAG_CHAINED                       0x00000002UL
// ...
#define MON_BITFLAG_ADD_UNSUMMON_TIMER            0x00004000UL

    uchar m_lev;    // Multi-bit bitfield converted to uchar
    uchar m_id;     // Separated from the defines by an empty line
    // ...
};
```

Helper macros are then defined (e.g., in `include/mondata.h` or similar) to abstract the bitwise logic. The required naming convention is `get_flag_`, `set_flag_`, `clear_flag_`, and `toggle_flag_`:
```c
#define get_flag_cloned_wizard(m) (((m)->mon_bitflags & MON_BITFLAG_CLONED_WIZ) != 0UL)
#define set_flag_cloned_wizard(m) ((m)->mon_bitflags |= MON_BITFLAG_CLONED_WIZ)
#define clear_flag_cloned_wizard(m) ((m)->mon_bitflags &= ~MON_BITFLAG_CLONED_WIZ)
#define toggle_flag_cloned_wizard(m) ((m)->mon_bitflags ^= MON_BITFLAG_CLONED_WIZ)
```
## Step-by-Step Conversion Guide

### 1. Identify and Group Bitfields
Find the struct containing the `Bitfield` declarations. Count the total number of bits required for 1-bit flags. 
- If replacing 1-bit flags: Group them into a `uint64_t`. Using `uint64_t` allows for up to 64 flags and is consistent with existing code like `mon_flags`. Do not use smaller fixed-length types like `uint32_t`.
- If replacing multi-bit bitfields (e.g., `Bitfield(wormno, 5)`): Promote these directly to a standalone `uchar` (which is `unsigned char`) field. There are no bitfields with a length more than 8 bits in the game, so a `uchar` is always sufficient. Place these `uchar` variables *after* the `uint64_t` flag variable and its associated `#define`s.

### 2. Define the Flags Field and Constants
Add the new field to the struct. The variable should be named clearly, such as `mon_bitflags`, `obj_bitflags` or simply `bitfields` if the struct's context is clear. The `#define` constants typically follow immediately after the field definition inside the struct, and any multi-bit `uchar` fields should follow after an empty line:
```c
    uint64_t my_bitflags;
#define MY_BITFLAG_NONE      0x00000000UL
#define MY_BITFLAG_SOME_FLAG 0x00000001UL

    uchar multi_bit_field;
```

Create `#define` constants for each flag. **Requirement:** Every bitflag group must include a `_NONE` field with a value of `0x00000000UL` (e.g., `MON_BITFLAG_NONE`). This can be used for clarity when initializing or explicitly checking for no flags.

**Prefix Naming Convention:**
To avoid name collisions and maintain clarity, use the following standard prefixes for bitmask constants based on the struct they belong to:
*   `struct monst`: `MON_BITFLAG_` (e.g., `MON_BITFLAG_FLEEING`)
*   `struct obj`: `OBJ_BITFLAG_` (e.g., `OBJ_BITFLAG_RUSTPRONE`)
*   `struct rm`: `RM_BITFLAG_`
*   `struct levelflags`: `LEVEL_BITFLAG_`
*   `struct u_have`: `UHAVE_BITFLAG_`
*   `struct u_event`: `UEVENT_BITFLAG_`
*   `struct u_achieve`: `UACHIEVE_BITFLAG_`
*   `struct mapseen_feat`: `MAPSEEN_FEAT_BITFLAG_`
*   `struct mapseen_flags`: `MAPSEEN_BITFLAG_`
*   `struct material_definition`: `MATERIAL_BITFLAG_`
*   `struct q_score`: `QSCORE_BITFLAG_`
*   `struct macflags`: `MAC_BITFLAG_`
*   `struct wd_stack_frame`: `WD_STACK_FRAME_BITFLAG_`

**Best Practice:** Always use the `UL` (or `ULL` for 64-bit) suffix to prevent integer overflow issues when doing bitwise operations or left-shifting, matching existing patterns like `mon_flags`.

**Important:** Do **not** use the bit shift operator (`<<`) to define flags unless there is a very strong case to use it (e.g., for the sake of specific visual clarity in certain contexts). Stick to explicit hex values as the primary standard.
```c
#define MON_BITFLAG_NONE      0x00000000UL
#define MON_BITFLAG_FLEEING   0x00000001UL
#define MON_BITFLAG_SLEEPING  0x00000002UL
#define MON_BITFLAG_BLINDED   0x00000004UL
```

### 3. Replace Accesses Across the Codebase
You must replace every read and write of the old bitfield. Search the codebase for the field name (e.g., `mflee`).

**Required:** You MUST create abstraction macros (`get_flag_`, `set_flag_`, `clear_flag_`, `toggle_flag_`) for every 1-bit bitfield you convert. Direct bitwise operations (`&`, `|`, `~`, `^`) should not be sprinkled throughout the codebase.

*   **Reads**: `if (mon->mflee)` becomes `if (get_flag_fleeing(mon))`
*   **Sets**: `mon->mflee = 1;` becomes `set_flag_fleeing(mon);`
*   **Clears**: `mon->mflee = 0;` becomes `clear_flag_fleeing(mon);`
*   **Toggles**: `mon->mflee = !mon->mflee;` becomes `toggle_flag_fleeing(mon);`

### 4. Update the Save File Version
Since you are changing the size, layout, and alignment of a core game struct, **you MUST increment the save file version**. 
Find `patchlevel.h` and update the version constants so the game correctly rejects old, incompatible save files instead of crashing or corrupting memory when trying to load them.

### 5. Update C# Mirror Structs and Enums
The C# side of GnollHack defines identical structs in `win/win32/xpl/GnollHackX/GnollHackX/GHConstants.cs` for P/Invoke interop. **You must modify these C# structs to perfectly mirror your new C struct layout.** Any padding, type size (`ulong` for `uint64_t`), or order changes must be exactly replicated in the C# definitions, or memory corruption will occur at the native bridge. 

Additionally, you must define corresponding C# `[Flags]` enums (based on `ulong`) in `GHConstants.cs` to map to the new C bitmask `#define`s.

### 6. Final Cleanup
Once all structs have been successfully converted away from the legacy bitfield architecture, the final step is to completely remove the `BITFIELDS` `#define` flag and the `Bitfield(x, n)` macro definitions from the codebase.
*   **C Core:** Remove `#define BITFIELDS` from `include/config.h` and the `Bitfield` macro definitions from `include/global.h`.
*   **C# Frontend:** Remove the `BITFIELDS` compilation constant from `.csproj` files (e.g., `GnollHackX.csproj`) and any `#if BITFIELDS` conditionals in `GHConstants.cs`.

## Best Practices & Pitfalls
- **Struct Padding:** When adding `uint64_t` variables, their location should be as close to the original bitfield positions as possible, while still considering struct alignment. To minimize implicit padding, they should ideally be placed after other 64-bit variables towards the top of the struct.
- **Initialization:** Structures in GnollHack are often zero-initialized via `memset(&struct, 0, sizeof(struct))`. This correctly zeros out your new integer flag field, meaning you usually do not need to add explicit `obj->my_flags = 0;` initialization code.
- **Boolean Assignments:** Beware of code like `mon->mflee = (condition);`. With bitfields, this implicitly truncated the result to 1 or 0. With bitmasks, you must write: 
  `if (condition) { set_flag_fleeing(mon); } else { clear_flag_fleeing(mon); }`
  Do NOT do `mon->my_flags |= (condition);` unless you guarantee `condition` evaluates to exactly the bitmask value.
