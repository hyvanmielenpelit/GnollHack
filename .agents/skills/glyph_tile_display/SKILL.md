---
name: glyph_tile_display
description: Guide to the GnollHack glyph-to-tile rendering pipeline. Covers glyph numbering, the multi-layer display system, the LayerInfo struct, tile sheet atlases, the Glyph2Tile mapping, double-buffered MapData, and how SkiaSharp renders tiles on the C# side.
---

# Glyph, Tile & Display Pipeline

GnollHack uses a multi-stage pipeline to render the game map: C core assigns **glyphs** → glyphs map to **tiles** via `Glyph2Tile[]` → tiles are drawn from **sprite sheet atlases** onto SkiaSharp canvases using a **20-layer** compositing system.

## Pipeline Overview

```
C Core (display.c)                    Native Bridge              C# Frontend (GamePage)
┌─────────────────┐                   ┌────────────┐             ┌──────────────────────┐
│ newsym(x,y)     │                   │ PrintGlyph │             │ MapData[x,y]         │
│ ↓               │                   │ callback   │             │  .Glyph              │
│ show_glyph()    │──LayerInfo───────→│ libproc.c  │────────────→│  .Layers             │
│ fill_glyph_info │  (per-tile data)  │            │             │  .Layers.layer_glyphs│
│ map_glyphinfo() │                   └────────────┘             │       [MAX_LAYERS]   │
└─────────────────┘                                              └────────┬─────────────┘
                                                                          │
                                           ┌──────────────────────────────┘
                                           ▼
                                    ┌──────────────────┐
                                    │ PaintSurface()   │
                                    │ For each layer:  │
                                    │  tile = G2T[glyph]│
                                    │  src = atlas rect │
                                    │  DrawBitmap(tile) │
                                    └──────────────────┘
```

---

## Glyph Numbering System

A **glyph** is a single integer that encodes both **what** to display and **which variant** (male/female, action pose, missile direction, etc.). Glyph ranges are defined in [display.h:313-387](file:///c:/hmp/GnollHack/include/display.h#L313):

| Offset Macro | Base | Content |
|-------------|------|---------|
| `GLYPH_MON_OFF` | 1 | Monster base tiles (NUM_MONSTERS entries) |
| `GLYPH_ATTACK_OFF` | +NUM_MONSTERS | Monster attack poses |
| `GLYPH_THROW_OFF` | +NUM_MONSTERS | Monster throw poses |
| `GLYPH_FIRE_OFF` | +NUM_MONSTERS | Monster ranged fire poses |
| `GLYPH_CAST_NODIR_OFF` | +NUM_MONSTERS | Non-directional casting |
| `GLYPH_CAST_DIR_OFF` | +NUM_MONSTERS | Directional casting |
| `GLYPH_SPECIAL_ATTACK_OFF` | +NUM_MONSTERS | Special attacks |
| `GLYPH_KICK_OFF` | +NUM_MONSTERS | Kick action |
| `GLYPH_PASSIVE_DEFENSE_OFF` | +NUM_MONSTERS | Passive defense |
| `GLYPH_DEATH_OFF` | +NUM_MONSTERS | Death animation |
| `GLYPH_FEMALE_MON_OFF` | +NUM_MONSTERS | Female monster variants |
| *(female action poses repeat)* | | |
| `GLYPH_INVIS_OFF` | | Invisible monster marker |
| `GLYPH_OBJ_OFF` | | Object tiles (NUM_OBJECTS entries) |
| `GLYPH_OBJ_MISSILE_OFF` | | Object missile direction variants |
| `GLYPH_CMAP_OFF` | | Dungeon feature tiles (walls, floors, doors) |
| `GLYPH_EXPLODE_OFF` | | Explosion tiles |
| `GLYPH_ZAP_OFF` | | Zap/ray tiles |
| `GLYPH_SWALLOW_OFF` | | Engulf tiles |
| `GLYPH_WARNING_OFF` | | Warning symbols |
| `GLYPH_STATUE_OFF` | | Statue tiles |
| `GLYPH_BODY_OFF` | | Corpse tiles |
| `GLYPH_ARTIFACT_OFF` | | Artifact-specific tiles |
| `GLYPH_PLAYER_OFF` | | Player character tiles |
| `GLYPH_ENLARGEMENT_OFF` | | Enlargement overlay tiles |
| `MAX_GLYPH` | | Total glyph count |

`NO_GLYPH` = 0 means "nothing to display."

**Key glyph macros** (also in display.h):
- `glyph_is_monster(glyph)` — true if glyph is in a monster range
- `glyph_is_object(glyph)` — true if glyph is an object
- `glyph_is_cmap(glyph)` — true if glyph is a dungeon feature
- `glyph_to_mon(glyph)` — extract monster index from glyph
- `glyph_to_obj(glyph)` — extract object index from glyph

---

## The 20-Layer Rendering System

Each map tile is composited from up to 20 layers, drawn bottom-to-top. Layers are defined in [layer.h:13-36](file:///c:/hmp/GnollHack/include/layer.h#L13):

```c
enum layer_types {
    LAYER_FLOOR = 0,          // Ground terrain
    LAYER_CARPET,             // Carpets/rugs
    LAYER_FLOOR_DOODAD,       // Floor decorations (below features)
    LAYER_FEATURE,            // Doors, stairs, altars, fountains
    LAYER_TRAP,               // Traps
    LAYER_FEATURE_DOODAD,     // Decorations above features
    LAYER_BACKGROUND_EFFECT,  // Background visual effects
    LAYER_CHAIN,              // Iron chain (ball & chain)
    LAYER_OBJECT,             // Items on the ground
    LAYER_MONSTER,            // Monsters
    LAYER_MISSILE,            // Projectiles in flight
    LAYER_COVER_TRAP,         // Cover on top of traps
    LAYER_COVER_OBJECT,       // Cover on top of objects
    LAYER_COVER_FEATURE,      // Cover on top of features
    LAYER_LEASH,              // Leash connections
    LAYER_ENVIRONMENT,        // Environmental overlay (rain, etc.)
    LAYER_ZAP,                // Zap/ray effects
    LAYER_GENERAL_EFFECT,     // General visual effects
    LAYER_MONSTER_EFFECT,     // Monster-specific effects
    LAYER_GENERAL_UI,         // UI overlays (cursor, selection)
    MAX_LAYERS                // = 20
};
```

---

## The LayerInfo Struct

Each call to `PrintGlyph` passes a `LayerInfo` struct from C to C#. This struct contains **per-layer glyph data** plus metadata.

**C definition** ([layer.h:42-92](file:///c:/hmp/GnollHack/include/layer.h#L42)):

```c
struct layer_info {
    int glyph;                          // Primary glyph (ASCII compatibility)
    int bkglyph;                        // Background glyph
    int layer_glyphs[MAX_LAYERS];       // Glyph for each of the 20 layers
    int layer_gui_glyphs[MAX_LAYERS];   // GUI-specific glyph per layer
    uint64_t layer_flags;               // LFLAGS_* bitfield
    unsigned m_id;                      // Monster unique ID
    unsigned o_id;                      // Object unique ID (boulder)
    struct obj* memory_objchn;          // Memory object chain
    short damage_displayed;             // Damage number to show
    short hit_tile;                     // Hit effect tile index
    // ... monster HP, rider glyph, status/condition bits,
    //     missile properties, leash coordinates, etc.
};
```

**C# mirror** ([GHConstants.cs:495](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX/GHConstants.cs#L495)): Exact `[StructLayout(LayoutKind.Sequential)]` match with `[MarshalAs]` arrays for P/Invoke marshalling.

**Layer flags** (LFLAGS_*): Location state — `LFLAGS_SHOWING_MEMORY`, `LFLAGS_CAN_SEE`, `LFLAGS_UXUY`, etc.

**Monster flags** (LMFLAGS_*): Per-monster rendering state — `LMFLAGS_PET`, `LMFLAGS_PEACEFUL`, `LMFLAGS_FLYING`, `LMFLAGS_BEING_HIT`, `LMFLAGS_KILLED`, etc.

---

## Glyph-to-Tile Mapping

The `Glyph2Tile[]` array maps each glyph integer to a tile index in the sprite sheet atlas.

**C side**: Generated by build tools, declared as `short glyph2tile[]` in tile source files. Size = `MAX_GLYPH`.

**C# side** ([GHApp.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX/GHApp.cs)):
- `Glyph2Tile` is a `int[]` array populated at startup via P/Invoke
- Protected by `Glyph2TileLock` (a `static readonly object`) for thread-safe access
- Read with `lock(Glyph2TileLock)` on game thread and UI thread

```csharp
lock (GHApp.Glyph2TileLock)
{
    int tile = GHApp.Glyph2Tile[glyph];
    // Use tile index to calculate sprite sheet coordinates
}
```

---

## MapData: Per-Tile Storage on C# Side

Each map cell is stored as a `MapData` struct ([MapData.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX/MapData.cs)):

```csharp
public struct MapData
{
    public int Glyph;              // Primary glyph
    public int BkGlyph;           // Background glyph
    public string Symbol;          // ASCII symbol
    public SKColor Color;          // Symbol color
    public uint Special;           // Special rendering flags
    public LayerInfo Layers;       // Full layer info (20 layers)
    public long GlyphPrintAnimationCounterValue;
    public bool NeedsUpdate;       // Dirty flag for re-rendering
    public bool MapAnimated;       // Has active animation
    public bool HasEnlargementOrAnimationOrSpecialHeight;
    public EngravingInfo Engraving; // Engraving data at this tile
}
```

**Double-buffering** (see maui_threading skill): The game thread writes to `_mapDataMaster` then copies to `_mapDataBuffer1`/`_mapDataBuffer2` under `lock(_mapDataBufferLock)`. The render thread reads `_mapDataCurrent` via `Monitor.TryEnter` (non-blocking).

---

## Tile Sheet Atlas System

Tiles are stored in large sprite sheet images (PNG files). Each tile occupies a fixed-size rectangle.

**Key C# constants** ([GHConstants.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX/GHConstants.cs)):
- `TileWidth` / `TileHeight` — pixel dimensions of each tile in the sheet
- `TilesPerRow` — number of tiles per row in the sprite sheet

**Tile coordinate calculation**:
```csharp
int tile = Glyph2Tile[glyph];
int sheetX = (tile % TilesPerRow) * TileWidth;
int sheetY = (tile / TilesPerRow) * TileHeight;
SKRect sourceRect = new SKRect(sheetX, sheetY,
    sheetX + TileWidth, sheetY + TileHeight);
```

The tile bitmaps are loaded as `SKBitmap` objects at startup and cached for the lifetime of the game session.

---

## The Rendering Loop (PaintSurface)

The main canvas `PaintSurface` handler in [GamePage.xaml.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX/Pages/Game/GamePage.xaml.cs) (starting ~line 7290):

1. Acquires shared data via `Monitor.TryEnter` (non-blocking)
2. Gets map data via `curGame.GetMapDataBuffer()` 
3. For each visible map cell (x, y):
   - For each layer (0 to MAX_LAYERS-1):
     - Gets the layer glyph: `layers.layer_gui_glyphs[layer]` (or `layer_glyphs[layer]` if no GUI glyph)
     - Skips if glyph is `NO_GLYPH`
     - Looks up tile: `Glyph2Tile[glyph]`
     - Calculates source rect from tile sheet
     - Draws tile to canvas at the correct screen position
4. Applies layer flags for special effects (transparency, darkening, detection glow)
5. Draws overlays: floating text, damage numbers, status indicators, cursor

**Important**: The rendering code runs on the **GPU thread** when GPU mode is enabled (`SKGLView`), so all shared data must be accessed via `Monitor.TryEnter` or `Interlocked` (see the maui_threading skill for details).

---

## Enlargement Tiles

Some glyphs reference **enlargement tiles** — tiles that are larger than the standard tile size (e.g., 2×2, 3×3 for large monsters like dragons). These are stored in the `GLYPH_ENLARGEMENT_OFF` range and composited as overlays on top of the base tile, extending into adjacent cells.

---

## Key Files

| File | Purpose |
|------|---------|
| [display.h](file:///c:/hmp/GnollHack/include/display.h) | Glyph offset macros, glyph interpretation macros, display constants |
| [layer.h](file:///c:/hmp/GnollHack/include/layer.h) | `enum layer_types` (20 layers), `struct layer_info`, layer flags |
| [display.c](file:///c:/hmp/GnollHack/src/display.c) | `newsym()`, `show_glyph()`, map update logic |
| [MapData.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX/MapData.cs) | C# per-tile storage struct |
| [GHConstants.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX/GHConstants.cs) | C# `LayerInfo` struct, `layer_types` enum, tile constants |
| [GHGame.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX/GHGame.cs) | `ClientCallback_PrintGlyph`, `SetMapSymbol`, double-buffered map data |
| [GHApp.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX/GHApp.cs) | `Glyph2Tile[]` array, `Glyph2TileLock`, tile bitmap loading |
| [GamePage.xaml.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX/Pages/Game/GamePage.xaml.cs) | `PaintSurface` handler — actual tile rendering loop |
| [callback.h](file:///c:/hmp/GnollHack/win/win32/xpl/libshare/callback.h) | `PrintGlyphCallback` typedef |
| [animation.h](file:///c:/hmp/GnollHack/include/animation.h) | Animation definitions and tile animation data |
