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

### Example: `bitflags`
All bitfields should be converted to a `uint64_t` variable explicitly named `bitflags` rather than something more specific.

```c
// include/monst.h
struct monst {
    // ...
    uint64_t bitflags;
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

General macros should be defined to abstract the bitwise logic using the flags variable and bit values:
```c
#define get_flag(flags, bit) (((flags) & (bit)) != 0UL)
#define set_flag(flags, bit, value) ((value) ? ((flags) |= (bit)) : ((flags) &= ~(bit)))
#define toggle_flag(flags, bit) ((flags) ^= (bit))
```

Shortcut macros should be provided for the most usual gets and sets. The naming convention for getters is `is_mon_` for monsters (e.g. `is_mon_cloned_wizard`), `is_obj_` for objects (e.g. `is_obj_cursed`), and `is_levl_` for level flags (e.g. `is_levl_lamplit`). Other contexts can have similar shortcut macros (e.g., `u.uevent` flags might use `is_uevent_`).

For sets and toggles, use prefixes like `set_mon_`, `toggle_mon_`, etc.:
```c
#define is_mon_cloned_wizard(m)     get_flag((m)->bitflags, MON_BITFLAG_CLONED_WIZ)
#define set_mon_cloned_wizard(m, v) set_flag((m)->bitflags, MON_BITFLAG_CLONED_WIZ, (v))
#define toggle_mon_cloned_wizard(m) toggle_flag((m)->bitflags, MON_BITFLAG_CLONED_WIZ)
```

### Global Single-Instance Struct Macros
If a converted struct is instantiated exactly once as a global variable (such as `u.uhave`, `u.uevent`, and `u.uachieve`), the shortcut macros should **not** take the struct instance as a parameter. Instead, they must directly reference that global instance in the macro definition:
```c
#define is_uevent_minor_oracle()    get_flag(u.uevent.bitflags, UEVENT_BITFLAG_MINOR_ORACLE)
#define set_uevent_minor_oracle(v)  set_flag(u.uevent.bitflags, UEVENT_BITFLAG_MINOR_ORACLE, (v))
```
## Step-by-Step Conversion Guide

### 1. Identify and Group Bitfields
Find the struct containing the `Bitfield` declarations. Count the total number of bits required for 1-bit flags. 
- If replacing 1-bit flags: Group them into a `uint64_t`. While `uint64_t` can technically hold up to 64 flags, a maximum of 32 bits will be added per one `bitflags` variable. Do not use smaller fixed-length types like `uint32_t`.
- If replacing multi-bit bitfields (e.g., `Bitfield(wormno, 5)`): Promote these directly to a standalone `uchar` (which is `unsigned char`) field. There are no bitfields with a length more than 8 bits in the game, so a `uchar` is always sufficient. Place these `uchar` variables *after* the `uint64_t` flag variable and its associated `#define`s.

### 2. Define the Flags Field and Constants
Add the new field to the struct. All bitfields should be converted to a `uint64_t` explicitly named `bitflags`. A maximum of 32 bits will be added per one `bitflags` variable even though it is `uint64_t`. If the bitfield conversion requires more than one bitflags variable in a struct (e.g., more than 32 bits), then the next bitflags variable will be created and called `bitflags2` (or `bitflags3` for a third, and so on). For these subsequent variables, the value literals should be named accordingly, such as `_BITFLAG2_` (or `_BITFLAG3_`). 

The `#define` constants typically follow immediately after the field definition inside the struct, and any multi-bit `uchar` fields should follow after an empty line:
```c
    uint64_t bitflags;
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

**Best Practice:** Even though the `bitflags` variable is a `uint64_t`, always use the `UL` (unsigned long) suffix for literal values rather than `ULL` (unsigned long long). `UL`s are better portable and still can be 64 bits long, preventing integer overflow issues during bitwise operations or left-shifting.

**Important:** Do **not** use the bit shift operator (`<<`) to define flags unless there is a very strong case to use it (e.g., for the sake of specific visual clarity in certain contexts). Stick to explicit hex values as the primary standard.
```c
#define MON_BITFLAG_NONE      0x00000000UL
#define MON_BITFLAG_FLEEING   0x00000001UL
#define MON_BITFLAG_SLEEPING  0x00000002UL
#define MON_BITFLAG_BLINDED   0x00000004UL
```

### 3. Replace Accesses Across the Codebase
You must replace every read and write of the old bitfield. Search the codebase for the field name (e.g., `mflee`).

**Required:** You should use the general macros (`get_flag`, `set_flag`, `toggle_flag`) with the `bitflags` field and the appropriate constant, or create shortcut macros (e.g., `is_mon_fleeing`, `set_mon_fleeing`) for the most usual accesses. Direct bitwise operations (`&`, `|`, `~`, `^`) should not be sprinkled throughout the codebase.

*   **Reads**: `if (mon->mflee)` becomes `if (is_mon_fleeing(mon))` or `if (get_flag(mon->bitflags, MON_BITFLAG_FLEEING))`
*   **Sets/Clears**: `mon->mflee = value;` becomes `set_mon_fleeing(mon, value);` or `set_flag(mon->bitflags, MON_BITFLAG_FLEEING, value);`
*   **Toggles**: `mon->mflee = !mon->mflee;` becomes `toggle_mon_fleeing(mon);` or `toggle_flag(mon->bitflags, MON_BITFLAG_FLEEING);`

**Handling Equality/Inequality Comparisons:**
When a legacy bitfield is used in an equality or inequality comparison with boolean values (`TRUE`, `FALSE`) or binary integers (`1`, `0`), the conversion must completely absorb the comparison into the shortcut macro call. Do not leave the `==` or `!=` operators in the code.

For example, if you have a shortcut macro `is_mon_fleeing(mon)` for the legacy bitfield `mflee`:
- `mon->mflee == 1` or `mon->mflee == TRUE` becomes `is_mon_fleeing(mon)`
- `mon->mflee == 0` or `mon->mflee == FALSE` becomes `!is_mon_fleeing(mon)`
- `mon->mflee != 1` or `mon->mflee != TRUE` becomes `!is_mon_fleeing(mon)`
- `mon->mflee != 0` or `mon->mflee != FALSE` becomes `is_mon_fleeing(mon)`

**CRITICAL:** Never generate code like `is_mon_fleeing(mon) == TRUE`, `is_mon_fleeing(mon) == 1`, `is_mon_fleeing(mon) == FALSE`, or `is_mon_fleeing(mon) == 0`. The shortcut macro already returns a boolean-like result suitable for `if` statements and logical operations.

### 4. Handle Aliases
The codebase sometimes contains `#define` aliases pointing to a bitfield. When converting these:
- **1-bit bitfields:** If the bitfield is one bit, then a new literal value with the same value as the pointed bitfield should be created.
- **Multi-bit bitfields:** If it is a multibit bitfield, which is converted to `uchar`, then the aliases can be retained as is.

### 5. Update the Save File Version
Since you are changing the size, layout, and alignment of a core game struct, **you MUST increment the save file version**. 
Find `patchlevel.h` and update the version constants so the game correctly rejects old, incompatible save files instead of crashing or corrupting memory when trying to load them.

### 6. Update C# Mirror Structs and Enums
The C# side of GnollHack defines identical structs in `win/win32/xpl/GnollHackX/GnollHackX/GHConstants.cs` for P/Invoke interop. **You must modify these C# structs to perfectly mirror your new C struct layout.** Any padding, type size (`ulong` for `uint64_t`), or order changes must be exactly replicated in the C# definitions, or memory corruption will occur at the native bridge. 

Additionally, you must define corresponding C# `[Flags]` enums (based on `ulong`) in `GHConstants.cs` to map to the new C bitmask `#define`s.

### 7. Final Cleanup
Once all structs have been successfully converted away from the legacy bitfield architecture, the final step is to completely remove the `BITFIELDS` `#define` flag and the `Bitfield(x, n)` macro definitions from the codebase.
*   **C Core:** Remove `#define BITFIELDS` from `include/config.h` and the `Bitfield` macro definitions from `include/global.h`.
*   **C# Frontend:** Remove the `BITFIELDS` compilation constant from `.csproj` files (e.g., `GnollHackX.csproj`) and any `#if BITFIELDS` conditionals in `GHConstants.cs`.

## Best Practices & Pitfalls
- **Struct Padding:** When adding `uint64_t` bitflags variables, their location should consider struct padding and alignment by placing them early in the struct. However, while considering this, the location should also be as close to the original location as possible. This typically implies that the bitflags variables are placed after the last 64-bit variable near the beginning of the struct to minimize implicit padding.
- **Initialization:** Structures in GnollHack are often zero-initialized via `memset(&struct, 0, sizeof(struct))`. This correctly zeros out your new integer flag field, meaning you usually do not need to add explicit `obj->bitflags = 0;` initialization code.
- **Boolean Assignments:** With bitfields, code like `mon->mflee = (condition);` implicitly truncated the result to 1 or 0. With the new bitmask `set_flag` macro, you can just do: 
  `set_mon_fleeing(mon, (condition));`
  Do NOT do `mon->bitflags |= (condition);` unless you guarantee `condition` evaluates to exactly the bitmask value.
