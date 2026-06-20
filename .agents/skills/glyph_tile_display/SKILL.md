---
name: glyph_tile_display
description: Guide to the GnollHack glyph-to-tile rendering pipeline. Covers glyph numbering, the multi-layer display system, the LayerInfo struct, tile sheet atlases, the Glyph2Tile mapping, double-buffered MapData, and how SkiaSharp renders tiles on the C# side.
---

# Glyph and Tile Display

## Critical Rules
- **Do NOT assume one glyph = one tile**. A single tile on screen is composed of multiple layers (Background, Floor, Feature, Monster, Object, etc.).
- **Always update via `newsym()`**. Do not try to draw directly to the screen from C game logic. Call `newsym(x, y)` to notify the display engine.
- **Double-buffering**: The map state is double-buffered to prevent tearing during UI updates.

## Layers (`LayerInfo`)
Each cell has 11 layers rendered back-to-front:
1. `Background` (Solid color or wallpaper)
2. `Floor` (Dungeon floor, water, lava)
3. `Feature` (Walls, doors, stairs)
4. `Object` (Items on the floor)
5. `Monster` (Creatures, player)
6. `Monster_Effect` (Status icons on monsters)
7. `Effect` (Spells, explosions)
8. `Cloud` (Gas, fog)
9. `Lighting` (Shadows, visibility masks)
10. `Cursor` (Player targeting cursor)
11. `UI` (Hover text, selection highlights)

## C-Side Mapping (`src/drawing.c`)
- **`glyph2tile[MAX_GLYPH]`**: Maps a glyph ID to a tile ID within a tileset.
- **`LayerInfo`**: Struct sent across the P/Invoke bridge containing the tile IDs for all 11 layers of a specific coordinate.

## C# Side Rendering (`win/win32/xpl/GnollHackX/GnollHackX/Views/MapCanvas.cs`)
- **SkiaSharp**: Uses `SKCanvas.DrawImage()` to render tile atlases.
- **`TileSet`**: Represents a PNG sprite sheet containing tiles of a specific size (e.g., 64x96).
- **`MapData`**: A grid of `LayerInfo` objects used by the `MapCanvas` to draw the visible screen.
