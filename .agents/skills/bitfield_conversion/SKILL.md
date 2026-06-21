---
name: bitfield_conversion
description: Guidelines and patterns for converting non-portable C Bitfield macros into platform-portable explicit bitmask flags to fix cross-platform save game compatibility.
---

# Converting Bitfield Macros to Platform-Portable Code

## The Problem
GnollHack's legacy C code uses a `Bitfield(x, n)` macro (expanding to `unsigned x : n` or `uchar x`) extensively in core data structures like `struct monst`, `struct obj`, and `struct you`.

**C bitfields are not portable.** Their memory layout (padding, alignment, and bit-endianness) is implementation-defined. Because GnollHack's save system works by dumping these structs directly into binary save files (`.sav`), a save file generated on an x64 Windows machine might have a completely different binary layout for bitfields compared to an ARM64 Android device. This breaks cross-platform save compatibility.

## The Solution
Convert bitfields into standard, fixed-size integer types (e.g., `uint32_t`, `uint64_t`) and use explicit bitwise operations (`|`, `&`, `~`) with defined masks. 

### Example: `mon_flags`
An excellent existing example is `mon_flags` in `include/monst.h`. Instead of adding new `Bitfield` declarations, a `uint64_t mon_flags;` was added.

```c
// include/monst.h
uint64_t mon_flags;

// Flag definitions
#define MON_FLAGS_NONE                          0x00000000UL
#define MON_FLAGS_RWRAITH                       0x00000001UL
#define MON_FLAGS_CHAINED                       0x00000002UL
// ...
#define MON_FLAGS_ADD_UNSUMMON_TIMER            0x00004000UL
```

Helper macros are then defined (e.g., in `include/mondata.h` or similar) to abstract the bitwise logic:
```c
#define is_cloned_wizard(m) (((m)->mon_flags & MON_FLAGS_CLONED_WIZ) != 0L)
#define set_cloned_wizard(m) ((m)->mon_flags |= MON_FLAGS_CLONED_WIZ)
#define clear_cloned_wizard(m) ((m)->mon_flags &= ~MON_FLAGS_CLONED_WIZ)
```

## Step-by-Step Conversion Guide

### 1. Identify and Group Bitfields
Find the struct containing the `Bitfield` declarations. Count the total number of bits required for 1-bit flags. 
- If replacing 1-bit flags: Group them into a single `uint8_t`, `uint16_t`, `uint32_t`, or `uint64_t` based on the count.
- If replacing multi-bit bitfields (e.g., `Bitfield(wormno, 5)`): It is almost always better to promote these to a full `uint8_t` or `uint16_t` standalone field rather than using complex bitshifting masks, unless memory is extremely tight.

### 2. Define the Flags Field and Constants
Add the new field to the struct:
```c
uint32_t my_flags;
```

Create `#define` constants for each flag. **Best Practice:** Always use the `UL` (or `ULL` for 64-bit) suffix to prevent integer overflow issues when doing bitwise operations or left-shifting.
```c
#define MY_FLAGS_FLEEING   0x00000001UL
#define MY_FLAGS_SLEEPING  0x00000002UL
#define MY_FLAGS_BLINDED   0x00000004UL
// Or using bit shifts:
#define MY_FLAGS_FLEEING   (1UL << 0)
#define MY_FLAGS_SLEEPING  (1UL << 1)
```

### 3. Replace Accesses Across the Codebase
You must replace every read and write of the old bitfield. Search the codebase for the field name (e.g., `mflee`).

*   **Reads**: `if (mon->mflee)` becomes `if ((mon->my_flags & MY_FLAGS_FLEEING) != 0)`
*   **Sets**: `mon->mflee = 1;` becomes `mon->my_flags |= MY_FLAGS_FLEEING;`
*   **Clears**: `mon->mflee = 0;` becomes `mon->my_flags &= ~MY_FLAGS_FLEEING;`
*   **Toggles**: `mon->mflee = !mon->mflee;` becomes `mon->my_flags ^= MY_FLAGS_FLEEING;`

**Highly Recommended:** Create abstraction macros to make the replacements cleaner and less error-prone, as shown in the `mon_flags` example above.

### 4. Update the Save File Version
Since you are changing the size, layout, and alignment of a core game struct, **you MUST increment the save file version**. 
Find `patchlevel.h` and update the version constants so the game correctly rejects old, incompatible save files instead of crashing or corrupting memory when trying to load them.

## Best Practices & Pitfalls
- **Struct Padding:** When adding fixed-size integers, consider struct alignment. Place `uint64_t` fields before `uint32_t` or `uint8_t` fields to minimize implicit padding.
- **Initialization:** Structures in GnollHack are often zero-initialized via `memset(&struct, 0, sizeof(struct))`. This correctly zeros out your new integer flag field, meaning you usually do not need to add explicit `obj->my_flags = 0;` initialization code.
- **Boolean Assignments:** Beware of code like `mon->mflee = (condition);`. With bitfields, this implicitly truncated the result to 1 or 0. With bitmasks, you must write: 
  `if (condition) { set_fleeing(mon); } else { clear_fleeing(mon); }`
  Do NOT do `mon->my_flags |= (condition);` unless you guarantee `condition` evaluates to exactly the bitmask value.
