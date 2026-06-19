# Advanced C Documentation Patterns

This reference supplements the main [SKILL.md](file:///c:/hmp/GnollHack/.agents/skills/c_code_documentation/SKILL.md) with detailed guidance on documenting macros, structs, enums, complex control flow, cross-system interactions, and common NetHack/GnollHack idioms.

---

## Macro and Constant Documentation

GnollHack uses macros extensively for game data definitions, accessor patterns, and configuration. These are often the hardest parts of the code for newcomers to understand.

### Header Macros

Document groups of related `#define` macros with a block comment explaining the pattern:

```c
/*
 * Monster movement speed constants.
 * Speed determines how many movement points a monster gains per
 * game turn. 12 is the baseline "normal" speed (same as an
 * unhasted player). Higher values mean more actions per turn.
 */
#define SPEED_SLOW      3   /* Very slow: 1 move per 4 turns */
#define SPEED_NORMAL   12   /* Standard speed: 1 move per turn */
#define SPEED_FAST     18   /* Fast: 3 moves per 2 turns */
#define SPEED_VERY_FAST 24  /* Very fast: 2 moves per turn */
```

### Accessor Macros

Document macros that wrap field access or computation:

```c
/*
 * ACURR(attr) — Get the player's current effective attribute value
 *
 * Returns the player's attribute 'attr' (e.g., A_STR, A_DEX)
 * after applying all modifiers: base value + equipment bonuses
 * + spell effects + polymorph adjustments. The result is clamped
 * to the valid range for the attribute.
 *
 * 'attr' must be one of: A_STR, A_INT, A_WIS, A_DEX, A_CON, A_CHA
 */
#define ACURR(attr) (acurr(attr))
```

### Data Table Macros

GnollHack defines monsters, objects, and other game entities using dense macro invocations. When encountering these, add comments that explain the *game design intent* of specific parameter choices rather than re-documenting the macro format (which is covered by the `adding_game_content` skill):

```c
/* Fire giant — tougher than hill giants but vulnerable to cold.
 * Appears in Gehennom and volcanic levels. High strength allows
 * it to throw boulders. */
MON("fire giant", ...)
```

---

## Struct and Type Documentation

Document struct definitions in headers with a block comment explaining the struct's role and its most important fields:

```c
/*
 * struct monst — A single monster instance on the current level
 *
 * Every monster in the game (including pets and shopkeepers) is
 * represented by a monst struct. These are linked in several
 * lists:
 *   - fmon: all monsters on the current level (via nmon)
 *   - migrating_mons: monsters traveling between levels
 *
 * Key fields:
 *   mhp/mhpmax   -- Current and maximum hit points
 *   mx, my        -- Map coordinates on the current level
 *   data          -- Pointer to static type data (struct permonst
 *                    from the mons[] array)
 *   minvent       -- Linked list of objects in inventory
 *   mtame         -- Tameness level (0 = wild, >0 = pet)
 *   mpeaceful     -- If TRUE, won't attack the player
 *   msleeping     -- If nonzero, the monster is asleep
 */
struct monst {
    struct monst *nmon;     /* next monster in level list */
    struct permonst *data;  /* pointer to static monster type info */
    ...
};
```

### Enum Documentation

Document enums with both the enum's purpose and individual value meanings:

```c
/*
 * Object classes — categorize all items in the game.
 * Used for inventory display grouping, pickup filters,
 * and object-type-specific game logic.
 */
enum obj_class_types {
    ILLOBJ_CLASS = 0, /* invalid/placeholder object */
    WEAPON_CLASS,     /* melee and ranged weapons */
    ARMOR_CLASS,      /* body armor, shields, helmets, etc. */
    RING_CLASS,       /* magical rings worn on fingers */
    AMULET_CLASS,     /* magical amulets worn around neck */
    TOOL_CLASS,       /* misc. tools: keys, lamps, horns */
    FOOD_CLASS,       /* edible items and corpses */
    ...
};
```

---

## Documenting Complex Control Flow

Many GnollHack functions contain deeply nested conditionals or long switch statements that implement game mechanics. Use structural comments to divide the function into logical phases:

```c
int
use_item(obj)
struct obj *obj;
{
    /*
     * Phase 1: Validate the item can be used.
     * Check for cursed items, inappropriate polymorphed forms,
     * and environmental restrictions (underwater, swallowed).
     */
    if (obj->cursed && ...) {
        ...
    }

    /*
     * Phase 2: Apply the item's primary effect.
     * Branch based on object type to the appropriate handler.
     */
    switch (obj->otyp) {
    case POTION_OF_HEALING:
        /* Restore 1d8 HP (blessed: 2d8, cursed: 1d4) */
        heal_amount = ...;
        break;

    case SCROLL_OF_IDENTIFY:
        /* Let the player identify items in inventory.
         * Blessed: identifies all items.
         * Uncursed: identifies 1-3 items.
         * Cursed: identifies 1 item, may misidentify. */
        ...
        break;
    ...
    }

    /*
     * Phase 3: Post-use bookkeeping.
     * Consume the item (if consumable), update inventory
     * display, and charge the player one action turn.
     */
    ...
}
```

---

## Documenting Cross-System Interactions

GnollHack's systems are tightly coupled. When code in one subsystem calls into another, document the cross-cutting interaction:

```c
/*
 * This triggers the display subsystem to redraw the affected
 * tile, the sound system to play the appropriate sound effect,
 * and the animation system to queue a visual effect.
 * See display.c:newsym(), soundset.c, and animdef.c.
 */
newsym_with_flags(x, y, NEWSYM_FLAGS_KEEP_OLD);
play_sfx_sound_at_location(SFX_TRAP_TRIGGER, x, y);
```

---

## Common NetHack Idioms to Document

These recurring code patterns are specific to the NetHack/GnollHack codebase and should be documented when first encountered in a function:

### Random Number Functions

```c
/*
 * rn2(n) — Returns a uniformly random int in [0, n-1].
 * rn2(5) returns 0..4.
 * Common usage: "if (!rn2(3))" means "33% probability".
 */

/*
 * d(n, s) — Dice roll: sum of n rolls of an s-sided die.
 * d(2, 6) returns 2d6 (range 2-12, bell-curve distribution).
 */
```

### Object Lifecycle Safety

```c
/*
 * Obj_gone_p(otmp) — Check if an object has been freed.
 * Must be tested after any operation that might destroy the
 * object (e.g., useup(), obfree()). If TRUE, the pointer is
 * dangling and must not be dereferenced.
 */
```

### Safe Monster Iteration

```c
/*
 * Iterate over all monsters on the current dungeon level.
 * fmon is the head of a singly-linked list; nmon links to
 * the next monster. Always save the nmon pointer before
 * processing because monsters can be killed and removed
 * during iteration.
 */
for (mtmp = fmon; mtmp; mtmp = mtmp2) {
    mtmp2 = mtmp->nmon;
    if (DEADMONSTER(mtmp))
        continue;
    /* process mtmp */
}
```

### Global State Variables

When functions access key global state, document what the fields represent on first use if not obvious from context:

| Variable | Type | Purpose |
|----------|------|---------|
| `u` | `struct you` | The player character. `u.ux`/`u.uy` = position, `u.ulevel` = experience level, `u.uhunger` = hunger counter. |
| `level` | `dlevel_t` | Current dungeon level. `level.locations[x][y]` = terrain, `level.monsters[x][y]` = monster at tile. |
| `levl` | macro | Shorthand for `level.locations`. `levl[x][y].typ` = terrain type. |
| `flags` | `struct flag` | Game options and runtime state. `flags.debug` (aliased as `wizard`) = debug mode. |
| `context` | `struct context_info` | Transient per-turn state. `context.run` = auto-move mode, `context.botlx` = redraw status line. |

---

## Documenting Conditional Compilation

The codebase uses `#ifdef` blocks for platform and feature toggles. Document non-obvious ones:

```c
#ifdef INSURANCE
    /* INSURANCE: periodic checkpoint saves for crash recovery.
     * When enabled, the game auto-saves at regular intervals
     * so progress can be recovered after an unexpected exit. */
    save_currentstate();
#endif

#ifdef STEED
    /* STEED: enables the riding system. When defined, players
     * can mount tame monsters with the 'ride' command. */
    if (u.usteed) {
        ...
    }
#endif
```

For `#ifdef` blocks guarding platform-specific code, a brief inline comment is sufficient:

```c
#ifdef WIN32  /* Windows-specific path handling */
    ...
#endif
```
