---
name: glyph_tile_display
description: Guide to the GnollHack glyph-to-tile rendering pipeline. Covers glyph numbering, the multi-layer display system, the LayerInfo struct, tile sheet atlases, the Glyph2Tile mapping, double-buffered MapData, and how SkiaSharp renders tiles on the C# side.
---

# Glyph and Tile Display

## Critical Rules
- **Do NOT assume one glyph = one tile**. A single map cell on screen is composed of many layers (floor, feature, trap, object, monster, effects, UI overlays).
- **Always update via `newsym()`**. Do not try to draw directly to the screen from C game logic. Call `newsym(x, y)` to notify the display engine.
- **Double-buffering**: The map state is double-buffered to prevent tearing during UI updates.

## Layers

The layer set is defined by `enum layer_types` in `include/layer.h`. There are
`MAX_LAYERS` layers (currently 20), drawn back-to-front in enum order:

| # | Layer | Notes |
|---|-------|-------|
| 1 | `LAYER_FLOOR` | Dungeon floor, water, lava |
| 2 | `LAYER_CARPET` | |
| 3 | `LAYER_FLOOR_DOODAD` | Doodads underneath features and traps |
| 4 | `LAYER_FEATURE` | Walls, doors, stairs |
| 5 | `LAYER_TRAP` | |
| 6 | `LAYER_FEATURE_DOODAD` | Doodads above features and traps |
| 7 | `LAYER_BACKGROUND_EFFECT` | |
| 8 | `LAYER_CHAIN` | |
| 9 | `LAYER_OBJECT` | Items on the floor |
| 10 | `LAYER_MONSTER` | Creatures and the player |
| 11 | `LAYER_MISSILE` | |
| 12 | `LAYER_COVER_TRAP` | |
| 13 | `LAYER_COVER_OBJECT` | |
| 14 | `LAYER_COVER_FEATURE` | |
| 15 | `LAYER_LEASH` | |
| 16 | `LAYER_ENVIRONMENT` | |
| 17 | `LAYER_ZAP` | |
| 18 | `LAYER_GENERAL_EFFECT` | |
| 19 | `LAYER_MONSTER_EFFECT` | |
| 20 | `LAYER_GENERAL_UI` | |

> **Read the current list from `include/layer.h`, not from this table.** Layers
> have been added over time; `MAX_LAYERS` is the authoritative count, and adding
> a layer changes the marshalled struct size on both sides of the bridge.

## Struct Naming Across the Bridge

The same per-cell payload carries a different name on each side of the P/Invoke
boundary. This is a deliberate choice made during the port and a common source
of confusion:

| Side | Type | Location |
|------|------|----------|
| C | `struct layer_info` (`snake_case`, per the C naming rules) | `include/layer.h` |
| C# | `LayerInfo` (PascalCase, chosen when porting) | `win/win32/xpl/GnollHackX/GnollHackX/GHStructs.cs` |

The struct carries **glyphs, not tile IDs**: `layer_glyphs[MAX_LAYERS]` and
`layer_gui_glyphs[MAX_LAYERS]`, alongside `layer_flags`, `m_id` / `o_id` for
identity checks, and `memory_objchn` for remembered floor objects. Glyph-to-tile
translation happens after the struct crosses the bridge.

## C-Side Glyph Mapping (`src/tiledata.c`)
- **`glyph2tile[MAX_GLYPH]`**: Maps a glyph ID to a tile ID within a tile sheet.
  Declared in `include/hack.h`, defined in `src/tiledata.c`.

> **`src/tile.c` is obsolete.** It is a NetHack leftover that also defines
> `glyph2tile`, but it is **not compiled** — no `.vcxproj` in `win/win32/vs/`
> references it (they all build `$(SrcDir)tiledata.c`). Editing `tile.c` has no
> effect on the game. Always edit `src/tiledata.c`.

## C# Side Rendering
Map drawing lives in `win/win32/xpl/GnollHackX/GnollHackX/`:
- **`CustomCanvasView.cs`** and **`Controls/SwitchableCanvasView.xaml.cs`** —
  the SkiaSharp canvas views the map is drawn onto.
- **`MapData.cs`** — one grid entry per cell, holding a `LayerInfo Layers` field.
- **`GHGame.cs`** — receives `LayerInfo` from the native print-glyph callback and
  writes it into the map arrays.
- **Tile sheets**: `GHApp._tileMap` is an `SKImage[]` of atlas sheets. Source
  rectangles are computed by `GHApp.TileSheetX()` / `GHApp.TileSheetY()` from the
  `GHConstants` values (`TileWidth`, `TileHeight`, `NumberOfTilesPerSheet`,
  `MaxTileSheetWidthInTiles`), then blitted with SkiaSharp.
