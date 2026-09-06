using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using Newtonsoft.Json;

#if GNH_MAUI
namespace GnollHackM
#else
using Xamarin.Forms;

namespace GnollHackX
#endif
{
    /*
     * Dynamic tile sheet composition, data model.
     *
     * GnollHack used to ship three fixed 8192 px wide tile sheets holding every
     * tile in the game, and the physical location of a tile was derived from
     * its logical tile ID with bit arithmetic: sheet = ntile >> 13, and the
     * position within the sheet from the low 13 bits. That coupling is what
     * forced every tile in the game to be resident at all times.
     *
     * The tile set compiler now emits many small partition atlases instead, and
     * the game composes a small number of large runtime sheets out of the
     * partitions it actually needs. Logical tile IDs are completely unchanged;
     * only the mapping from a tile ID to a physical location becomes dynamic,
     * and that mapping lives in GHApp.TileSlots, packed by TileSlot below.
     */

    /*
     * One tile's physical location, packed into a single int.
     *
     *   bits 31-30  unused (always zero)
     *   bits 29-26  destination sheet index      (0 - 15, MaxTileSheets is 8)
     *   bits 25-13  x pixel offset in that sheet (0 - 8191)
     *   bits 12-0   y pixel offset in that sheet (0 - 8191)
     *
     * Four bytes per tile rather than the eight an equivalent struct would
     * take. GHApp.TileSlots is indexed with tile IDs on the rendering hot path
     * several thousand times per frame, so halving the table halves the number
     * of cache lines that touch.
     *
     * Both coordinates are therefore limited to 8191 px, and a composed sheet
     * must be sized to respect that. That is not automatic: sheets are sized
     * from GRContext.MaxTextureSize, which is commonly 16384, so SolveBudget
     * caps the usable size to MaxCoordinate + 1 before deriving the geometry.
     * Without that cap a tall sheet's lower rows wrap to y - 8192 and render
     * whatever art happens to sit there.
     *
     * The same 8191 ceiling is assumed by GamePage's darkened tile cache key,
     * which packs the source rectangle's top into 13 bits.
     */
    public static class TileSlot
    {
        public const int SheetShift = 26;
        public const int SheetMask = 0x0F;
        public const int XShift = 13;
        public const int CoordinateMask = 0x1FFF;

        /* Largest values that still fit in the packed representation */
        public const int MaxSheetIndex = SheetMask;
        public const int MaxCoordinate = CoordinateMask;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static int Pack(int sheetIdx, int x, int y)
        {
            return ((sheetIdx & SheetMask) << SheetShift)
                   | ((x & CoordinateMask) << XShift)
                   | (y & CoordinateMask);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static int SheetIdx(int packed)
        {
            return (packed >> SheetShift) & SheetMask;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static int X(int packed)
        {
            return (packed >> XShift) & CoordinateMask;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static int Y(int packed)
        {
            return packed & CoordinateMask;
        }
    }

    /*
     * One contiguous run of logical tile IDs inside a partition.
     *
     * A partition is a set of runs rather than a single range because the
     * tiles sharing a residency key are not contiguous: process_tiledata()
     * emits animations and enlargements interleaved by owner, so the 82
     * distinct keys are scattered over 526 runs whose median length is one
     * tile. Grouping the runs by key is what keeps the partition count at 82.
     */
    public class TileRun
    {
        public int firstTile { get; set; }
        public int tileCount { get; set; }
    }

    /*
     * One shipped partition atlas: the set of logical tile ID runs sharing one
     * residency key, packed densely and in ascending tile order at the same
     * 128 tile pitch as a destination sheet.
     *
     * Because the partition is compact in both its own atlas and its
     * destination range, composing it is one shifted-linear copy rather than
     * one call per run or per tile: source linear range [0, n) to destination
     * linear range [D, D + n). That decomposes into at most two DrawImage
     * calls per destination row the partition spans, and into a single
     * full width call when D happens to land on a row boundary.
     *
     * Field names are deliberately camelCase rather than PascalCase because
     * they are deserialised straight from gnollhack_tilemanifest.json, which
     * the tile set compiler writes.
     */
    public class TilePartition
    {
        public int id { get; set; }
        public string name { get; set; }

        /* Coarse grouping used by the budget solver: "core", "effects",
           "player-role", "bootstrap" and so on. Named partitionClass here
           because "class" is a C# keyword. */
        [JsonProperty("class")]
        public string partitionClass { get; set; }

        /* Lower is kept longer when memory is short */
        public int priority { get; set; }

        /*
         * The logical tile ID runs this partition holds, in ascending
         * firstTile order. The list is exhaustive and ordered: the j-th tile
         * of the partition, counted through the runs in order, sits at
         * position (j % 128, j / 128) of its atlas. That is what lets the
         * composer rebuild every tile's slot from the manifest alone.
         */
        public List<TileRun> runs { get; set; }

        /* Total number of tiles in the partition; must equal the sum of the
           runs' counts. The manifest carries it so that a disagreement between
           a partition's runs and its atlas is detectable without decoding. */
        public int tileCount { get; set; }

        /* Radial transparency has already been baked into this atlas */
        public bool radial { get; set; }

        /* Map layer the tiles belong to. A layer is never split across two
           destination sheets, so this is a hard constraint on assignment. */
        public string layer { get; set; }

        /* Shipped file name, relative to the tileset asset directory */
        public string file { get; set; }

        public int widthInTiles { get; set; }
        public int heightInTiles { get; set; }

        /* Set on player role partitions only */
        public string role { get; set; }
        public string race { get; set; }
        public string gender { get; set; }
        public string align { get; set; }

        /* Set on animation partitions only. animationIndex lets the frontend
           read the authoritative class from LibGetAnimationArrayElement rather
           than trusting the string. */
        public string animationClass { get; set; }
        public int animationIndex { get; set; }

        /* Height of this partition's source atlas, in tiles. Nothing is padded
           to a row boundary anywhere; the last row of the atlas is simply
           allowed to be partly empty. */
        public int RowsInSheet
        {
            get
            {
                return (tileCount + GHConstants.MaxTileSheetWidthInTiles - 1)
                       / GHConstants.MaxTileSheetWidthInTiles;
            }
        }

        /* True when the runs cover exactly tileCount tiles, in ascending order,
           with no overlap. A partition failing this disagrees with its own
           atlas and would render every one of its tiles as some other tile, so
           the composer refuses it and falls back to the legacy sheets. */
        public bool HasConsistentRuns()
        {
            if (runs == null || runs.Count == 0)
                return false;

            int total = 0;
            int previousEnd = -1;
            foreach (TileRun run in runs)
            {
                if (run.tileCount <= 0 || run.firstTile < 0
                    || run.firstTile <= previousEnd)
                    return false;
                previousEnd = run.firstTile + run.tileCount - 1;
                total += run.tileCount;
            }
            return total == tileCount;
        }

        public bool IsPlayerRole
        {
            get { return !string.IsNullOrEmpty(role); }
        }
    }

    /*
     * gnollhack_tilemanifest.json, as written by the tile set compiler and read
     * at load time by GHTileComposition.LoadManifestAsync().
     *
     * formatVersion and totalTiles exist so that partition atlases left over
     * from an older build are detected rather than silently rendering the wrong
     * tiles: totalTiles must agree with the C core's CountTotalTiles().
     */
    public class TilePartitionManifest
    {
        /* Bumped whenever the manifest schema changes incompatibly */
        public const int SupportedFormatVersion = 1;

        public int formatVersion { get; set; }
        public int totalTiles { get; set; }
        public int tileWidth { get; set; }
        public int tileHeight { get; set; }
        public int tilesPerRow { get; set; }
        public List<TilePartition> partitions { get; set; }

        public bool IsCompatibleWith(int coreTotalTiles)
        {
            return formatVersion == SupportedFormatVersion
                   && partitions != null
                   && partitions.Count > 0
                   && totalTiles == coreTotalTiles
                   && tileWidth == GHConstants.TileWidth
                   && tileHeight == GHConstants.TileHeight
                   && tilesPerRow == GHConstants.MaxTileSheetWidthInTiles;
        }
    }
}
