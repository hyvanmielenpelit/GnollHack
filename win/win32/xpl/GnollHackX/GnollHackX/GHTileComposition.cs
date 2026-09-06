using Newtonsoft.Json;
using SkiaSharp;
using System;
using System.Collections.Generic;
using System.IO;
using System.Threading;
using System.Threading.Tasks;
#if GNH_MAUI
using GnollHackM;
#else
using Xamarin.Forms;
#endif

/* Namespace follows GHApp.cs: GnollHackX in both builds, with the MAUI-only
   types imported above, rather than GHConstants.cs's pattern of switching
   namespace. This file talks to GHApp far more than it talks to GHConstants. */
namespace GnollHackX
{
    /* How much optional visual richness to keep resident. Auto picks from the
       device's memory. Ultra is lossless for the chosen role. */
    public enum TileDetailTier
    {
        Auto = 0,
        Ultra,
        High,
        Medium,
        Low
    }

    /* One composed destination sheet: the partitions it holds, in the order
       they are written into it. */
    public class TileSheetPlan
    {
        public int SheetIndex { get; set; }
        public List<TilePartition> Partitions { get; private set; }
        /* Linear destination slots consumed so far, in tiles */
        public int TilesUsed { get; set; }

        public TileSheetPlan()
        {
            Partitions = new List<TilePartition>();
        }
    }

    /* The result of the budget solver: what to make resident and where. */
    public class TileCompositionPlan
    {
        public TilePartitionManifest Manifest { get; set; }
        public TileDetailTier Tier { get; set; }
        public List<TileSheetPlan> Sheets { get; private set; }

        /*
         * Player partitions held back from the initial composition.
         *
         * The tile sheets are composed on the loading screen, which runs before
         * the game library has parsed its configuration, so the character is
         * not known yet. Composing all 73 role combinations there would cost
         * about 10250 tiles for the ~140 the chosen character actually uses, so
         * they are deferred and ComposeRoleSheetsAsync() brings in the matching
         * ones as soon as the character is settled.
         */
        public List<TilePartition> DeferredPlayerPartitions { get; private set; }

        /*
         * Index of the first sheet composed for a character, or -1 while none
         * has been. Everything from here on belongs to the current character
         * and is replaced wholesale when a different one is chosen -- without
         * it, each game in a session would append another role's sheets and
         * MaxTileSheets would run out after a handful.
         */
        public int RoleSheetsStart { get; set; }

        /*
         * The role and race whose partitions are resident, or null while none
         * are. Gender and alignment are not recorded because they are not part
         * of the residency decision -- every gender, and for a Priest every
         * alignment, of this pair is composed together. See MatchesRole.
         */
        public string ResidentRole { get; set; }
        public string ResidentRace { get; set; }

        public int ResidentTiles { get; set; }
        public int SheetWidthInTiles { get; set; }
        public int MaxRowsPerSheet { get; set; }

        public TileCompositionPlan()
        {
            Sheets = new List<TileSheetPlan>();
            DeferredPlayerPartitions = new List<TilePartition>();
            RoleSheetsStart = -1;
        }

        public int ResidentPartitions
        {
            get
            {
                int count = 0;
                foreach (TileSheetPlan sheet in Sheets)
                    count += sheet.Partitions.Count;
                return count;
            }
        }

        public long EstimatedBytes
        {
            get
            {
                long bytes = 0;
                foreach (TileSheetPlan sheet in Sheets)
                {
                    int rows = (sheet.TilesUsed + SheetWidthInTiles - 1) / SheetWidthInTiles;
                    bytes += (long)SheetWidthInTiles * GHConstants.TileWidth
                             * rows * GHConstants.TileHeight * 4L;
                }
                return bytes;
            }
        }
    }

    /*
     * Dynamic tile sheet composition.
     *
     * The tile set compiler ships many small partition atlases instead of three
     * monolithic sheets. This class reads the manifest describing them, decides
     * which partitions this session actually needs, and composes them into one
     * to three runtime sheets, filling GHApp.TileSlots as it goes.
     *
     * Three rules keep the transient memory cost to one source atlas above the
     * final resident size, and all three are load bearing:
     *
     *   1. Sheets are composed strictly sequentially, never concurrently. Two
     *      full sheets in flight is 384 MiB and would OOM exactly the devices
     *      this exists to serve. _compositionLock is a semaphore of one.
     *   2. Each source bitmap is disposed as soon as its partition has been
     *      copied. LoadTilesetFromPlatformAssetsAsync deliberately does the
     *      opposite -- it keeps the bitmap alive through SKImage.FromBitmap --
     *      and copying that pattern into a hundred-partition loop would hold
     *      roughly a gigabyte at once.
     *   3. A destination sheet is composed exactly once and never appended to,
     *      which avoids Skia's copy-on-write penalty on a snapshotted surface
     *      and means the render thread never draws from a sheet being mutated.
     */
    /*
     * A tile-aligned rectangle in a destination sheet, in tile units rather
     * than pixels. CopyLinearRun reports the ground it covered this way so the
     * radial mask pass can cover exactly the same ground and nothing else --
     * a partition's last destination row is usually shared with the partition
     * that follows it.
     */
    public struct TileRectangle
    {
        public int X;
        public int Y;
        public int Width;
        public int Height;

        public TileRectangle(int x, int y, int width, int height)
        {
            X = x;
            Y = y;
            Width = width;
            Height = height;
        }
    }

    public static class GHTileComposition
    {
        private static readonly SemaphoreSlim _compositionLock = new SemaphoreSlim(1, 1);

        /* Destination slot 0 of sheet 0 is the missing tile placeholder that
           every non-resident tile resolves to, so composition starts at 1.
           GHApp.TileSlots is zero initialised, which is why an unassigned entry
           already points there and the render path needs no residency test. */
        public const int PlaceholderSlots = 1;

        public static TilePartitionManifest Manifest { get; private set; }

        /*
         * Read gnollhack_tilemanifest.json from the platform assets.
         *
         * Returns null, rather than throwing, on every failure path: a missing
         * or incompatible manifest is a fall back to the legacy sheets, not a
         * crash. The caller reports which path it took, because a silent
         * fallback would make every measurement in this feature meaningless.
         */
        public static async Task<TilePartitionManifest> LoadManifestAsync(int coreTotalTiles)
        {
            try
            {
                string json;
                using (Stream stream = await GHApp.PlatformService.GetPlatformAssetsStreamAsync(
                    GHConstants.AssetsTilesetDirectory, GHConstants.TileManifestFileName))
                {
                    if (stream == null)
                    {
                        GHApp.MaybeWriteGHLog("LoadManifestAsync: tile manifest asset not found.");
                        return null;
                    }
                    using (StreamReader reader = new StreamReader(stream))
                    {
                        json = await reader.ReadToEndAsync();
                    }
                }

                TilePartitionManifest manifest =
                    JsonConvert.DeserializeObject<TilePartitionManifest>(json);

                if (manifest == null || !manifest.IsCompatibleWith(coreTotalTiles))
                {
                    GHApp.MaybeWriteGHLog(
                        "LoadManifestAsync: tile manifest is incompatible with this build ("
                        + (manifest == null ? "unreadable" :
                           "formatVersion " + manifest.formatVersion
                           + ", totalTiles " + manifest.totalTiles
                           + " against " + coreTotalTiles)
                        + "). Falling back to legacy tile sheets.");
                    return null;
                }

                foreach (TilePartition partition in manifest.partitions)
                {
                    if (!partition.HasConsistentRuns())
                    {
                        GHApp.MaybeWriteGHLog(
                            "LoadManifestAsync: partition '" + partition.name
                            + "' has a run list that disagrees with its own tile count. "
                            + "Falling back to legacy tile sheets.");
                        return null;
                    }
                }

                Manifest = manifest;
                return manifest;
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("LoadManifestAsync: " + ex.Message);
                return null;
            }
        }

        /*
         * Decide which partitions to make resident, and in which destination
         * sheet each one lands.
         *
         * Sheet assignment obeys layer affinity first and size second: a map
         * layer is never split across two sheets, because a frame draws a layer
         * at a time and splitting one doubles that layer's texture binds. The
         * chosen player role is the single exception, and it is not really one:
         * the hero is one tile per frame, so its own sheet costs at most one
         * extra bind. That is what lets sheet 2 exist at all.
         *
         * role and race select the player partitions to keep; gender and
         * alignment are accepted but not filtered on, because both change in
         * play. See MatchesRole.
         * Passing null for role keeps every role, which is what Phase 3 of the
         * rollout does so that composition can be compared against the legacy
         * sheets with identical content.
         */
        public static TileCompositionPlan SolveBudget(TilePartitionManifest manifest,
            TileDetailTier tier, ulong totalMemory, int maxTextureSize,
            string role, string race, string gender, string align,
            bool deferPlayerRoles)
        {
            if (manifest == null)
                return null;

            TileCompositionPlan plan = new TileCompositionPlan();
            plan.Manifest = manifest;
            plan.Tier = tier == TileDetailTier.Auto ? ResolveAutoTier(totalMemory) : tier;

            /*
             * Sheet geometry is in practice fixed at 128 x 85 tiles,
             * 8192 x 8160 px. Three separate limits meet here and all three
             * land on 8192, so the arithmetic below has exactly one outcome
             * for every possible input:
             *
             *   - the partition atlases are laid out at a 128 tile pitch, so a
             *     wider sheet would not hold more per row;
             *   - TileSlot packs the y offset in 13 bits, making 8191 px the
             *     tallest addressable sheet, and GamePage's darkened tile cache
             *     key packs the source rectangle's top the same way;
             *   - MinBelievableMaxTextureSize is 8192, because the legacy
             *     sheets are 8192 x 6144 and every supported device renders
             *     them.
             *
             * The device's reported limit is therefore advisory. It is still
             * read and logged -- a device that really does report less from a
             * ready GPU context is worth knowing about -- but it no longer
             * shrinks the sheet, because a smaller sheet holds fewer tiles,
             * needs more sheets, and costs a texture bind per sheet every
             * frame. An iPad reporting a low figure during load produced nine
             * sheets and a frame rate around 20.
             *
             * The clamps are kept rather than replaced by the constant so that
             * a genuine constraint can be reintroduced in one place if a device
             * ever warrants it.
             */
            int usableSize = maxTextureSize > 0
                ? maxTextureSize : GHConstants.DefaultMaxTextureSize;
            if (usableSize < GHConstants.MinBelievableMaxTextureSize)
            {
                GHApp.MaybeWriteGHLog(string.Format(
                    "SolveBudget: reported texture limit {0} is below {1}; composing at "
                    + "{2} anyway, which the legacy sheets already prove this device can "
                    + "hold. A smaller sheet costs texture binds every frame.",
                    usableSize, GHConstants.MinBelievableMaxTextureSize,
                    GHConstants.DefaultMaxTextureSize));
                usableSize = GHConstants.DefaultMaxTextureSize;
            }
            usableSize = Math.Min(usableSize, TileSlot.MaxCoordinate + 1);
            plan.SheetWidthInTiles = Math.Min(GHConstants.MaxTileSheetWidthInTiles,
                Math.Max(1, usableSize / GHConstants.TileWidth));
            plan.MaxRowsPerSheet = Math.Max(1, usableSize / GHConstants.TileHeight);

            GHApp.MaybeWriteGHLog(string.Format(
                "SolveBudget: maxTextureSize {0}, sheet {1}x{2} tiles ({3}x{4} px), "
                + "{5} tiles per sheet, tier {6}",
                maxTextureSize, plan.SheetWidthInTiles, plan.MaxRowsPerSheet,
                plan.SheetWidthInTiles * GHConstants.TileWidth,
                plan.MaxRowsPerSheet * GHConstants.TileHeight,
                plan.SheetWidthInTiles * plan.MaxRowsPerSheet, plan.Tier));

            List<TilePartition> kept = new List<TilePartition>();
            foreach (TilePartition partition in manifest.partitions)
            {
                if (!IsResident(partition, plan.Tier, role, race))
                    continue;

                /* Bootstrap and core are never deferred whatever their metadata
                   says. A manifest written before the bootstrap partition
                   stopped carrying one role's name still labels it
                   "Archaeologist", which would make IsPlayerRole true and hold
                   back the 292 base player tiles every chargen menu draws. */
                if (deferPlayerRoles && partition.IsPlayerRole
                    && partition.partitionClass != "bootstrap"
                    && partition.partitionClass != "core")
                {
                    plan.DeferredPlayerPartitions.Add(partition);
                    continue;
                }

                kept.Add(partition);
            }

            if (!deferPlayerRoles)
            {
                plan.ResidentRole = role;
                plan.ResidentRace = race;
            }

            AssignToSheets(plan, kept);

            foreach (TileSheetPlan sheet in plan.Sheets)
            {
                foreach (TilePartition partition in sheet.Partitions)
                    plan.ResidentTiles += partition.tileCount;
            }

            return plan;
        }

        /*
         * Compose the tile sheets for one character, after the fact.
         *
         * Called once the role, race, gender and alignment are settled: for a
         * new game from ClientCallback_PlayerSelection, which the library
         * reaches before it shows any selection menu, and for a restored game
         * from the character recorded in the save header. Until this runs, the
         * character's action and animation tiles resolve to the placeholder;
         * the base player tiles live in the bootstrap partition and stay
         * resident throughout, so every chargen menu still draws correctly.
         *
         * New sheets are appended rather than replacing anything, and the slot
         * table is rebuilt from a copy and published whole, so a render thread
         * never sees a half-updated map. Sheets are published before the slot
         * table on purpose: for the instant between the two, the new sheet
         * exists with nothing pointing at it, which draws placeholders. The
         * other order would point tiles at a sheet that is not there yet.
         */
        public static async Task<bool> ComposeRoleSheetsAsync(TileCompositionPlan plan,
            string role, string race, string gender, string align,
            Action<double, string> progress)
        {
            if (plan == null || plan.DeferredPlayerPartitions.Count == 0)
                return true;
            if (string.IsNullOrEmpty(role))
                return false;

            /* Already resident. Compared on role and race only, to agree with
               MatchesRole: every gender and alignment of that pair is composed
               together, so a change of either needs no recomposition and must
               not trigger one. */
            if (!string.IsNullOrEmpty(plan.ResidentRole)
                && Matches(plan.ResidentRole, role)
                && Matches(plan.ResidentRace, race))
                return true;

            List<TilePartition> wanted = new List<TilePartition>();
            foreach (TilePartition partition in plan.DeferredPlayerPartitions)
            {
                if (MatchesRole(partition, role, race))
                    wanted.Add(partition);
            }

            if (wanted.Count == 0)
            {
                GHApp.MaybeWriteGHLog("ComposeRoleSheetsAsync: no partition matches "
                    + role + "/" + race + "/" + gender + "/" + align + ".");
                return false;
            }

            await _compositionLock.WaitAsync();
            try
            {
                /* A different character than last time: give back the sheets
                   the previous one occupied instead of appending beside them.
                   Starting several games in one session is ordinary, and each
                   would otherwise cost another sheet or two out of the eight
                   there are. */
                if (plan.RoleSheetsStart >= 0
                    && plan.Sheets.Count > plan.RoleSheetsStart)
                {
                    for (int i = plan.RoleSheetsStart; i < plan.Sheets.Count; i++)
                    {
                        GHApp.RetireTileSheet(i);
                        plan.ResidentTiles -= plan.Sheets[i].TilesUsed;
                    }
                    plan.Sheets.RemoveRange(plan.RoleSheetsStart,
                        plan.Sheets.Count - plan.RoleSheetsStart);

                    /* No character is resident from here until the new one is
                       composed. Leaving the old name in place would make a
                       later call for it short-circuit and render placeholders,
                       because its sheets have just been given back. */
                    plan.ResidentRole = null;
                    plan.ResidentRace = null;
                }

                int firstNewSheet = plan.Sheets.Count;
                if (plan.RoleSheetsStart < 0)
                    plan.RoleSheetsStart = firstNewSheet;
                AppendToNewSheets(plan, wanted);
                if (plan.Sheets.Count == firstNewSheet)
                {
                    GHApp.MaybeWriteGHLog("ComposeRoleSheetsAsync: no sheet available.");
                    return false;
                }

                /* Work on copies. The live arrays are read without the lock on
                   the render path, so they are never written in place. */
                int[] slots = (int[])GHApp.TileSlots.Clone();
                short[] partitionOf = (short[])GHApp.TileToPartition.Clone();

                /* Every deferred tile goes back to the placeholder first, so
                   that entries left by a previous character cannot survive into
                   the new table and point at a sheet that has been retired.
                   Slot 0 of sheet 0 is the placeholder, which is why zero is
                   the right reset value. */
                ClearDeferredPlayerSlots(plan, slots, partitionOf);

                for (int i = firstNewSheet; i < plan.Sheets.Count; i++)
                {
                    TileSheetPlan sheet = plan.Sheets[i];
                    if (progress != null)
                    {
                        progress((double)(i - firstNewSheet)
                            / (plan.Sheets.Count - firstNewSheet),
                            "Composing Character Tiles...");
                    }

                    SKImage composed = await ComposeSheetAsync(plan, sheet, slots,
                        partitionOf);
                    if (composed == null)
                    {
                        plan.Sheets.RemoveRange(firstNewSheet,
                            plan.Sheets.Count - firstNewSheet);
                        return false;
                    }

                    GHApp.PublishTileSheet(sheet.SheetIndex, composed);
                    plan.ResidentTiles += sheet.TilesUsed;
                }

                GHApp.PublishSlotTable(slots, partitionOf);

                plan.ResidentRole = role;
                plan.ResidentRace = race;

                if (progress != null)
                    progress(1.0, "Character tiles ready.");
                return true;
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("ComposeRoleSheetsAsync: " + ex.Message);
                return false;
            }
            finally
            {
                _compositionLock.Release();
            }
        }

        private static void ClearDeferredPlayerSlots(TileCompositionPlan plan,
            int[] slots, short[] partitionOf)
        {
            foreach (TilePartition partition in plan.DeferredPlayerPartitions)
            {
                foreach (TileRun run in partition.runs)
                {
                    for (int k = 0; k < run.tileCount; k++)
                    {
                        int tile = run.firstTile + k;
                        if (tile < 0 || tile >= slots.Length)
                            continue;
                        slots[tile] = 0;
                        partitionOf[tile] = GHApp.PlaceholderPartition;
                    }
                }
            }
        }

        /*
         * Put partitions into sheets appended after the ones already composed.
         * Unlike AssignToSheets this reserves no placeholder slot: the
         * placeholder belongs to sheet 0 and is already accounted for.
         */
        private static void AppendToNewSheets(TileCompositionPlan plan,
            List<TilePartition> partitions)
        {
            int capacity = plan.SheetWidthInTiles * plan.MaxRowsPerSheet;
            TileSheetPlan current = null;

            foreach (TilePartition partition in partitions)
            {
                if (current == null
                    || current.TilesUsed + partition.tileCount > capacity)
                {
                    if (plan.Sheets.Count >= GHConstants.MaxTileSheets)
                    {
                        GHApp.MaybeWriteGHLog(
                            "ComposeRoleSheetsAsync: out of tile sheets, dropping "
                            + partition.name + ".");
                        continue;
                    }
                    current = NewSheet(plan);
                }
                current.Partitions.Add(partition);
                current.TilesUsed += partition.tileCount;
            }
        }

        /*
         * Auto tier, from the device's total memory:
         *
         *     under 3.0 GB   Low
         *     3.0 - 3.5 GB   Medium
         *     3.5 - 4.0 GB   High
         *     4.0 GB and up  Ultra
         *
         * These are deliberately not the auxiliary GPU canvas thresholds. That
         * relationship held only while both sat at 4 GB; the two decisions are
         * about different resources and are now free to move independently.
         */
        public static TileDetailTier ResolveAutoTier(ulong totalMemory)
        {
            if (totalMemory < GHConstants.TileDetailLowThresholdInBytes)
                return TileDetailTier.Low;
            if (totalMemory < GHConstants.TileDetailMediumThresholdInBytes)
                return TileDetailTier.Medium;
            if (totalMemory < GHConstants.TileDetailHighThresholdInBytes)
                return TileDetailTier.High;
            return TileDetailTier.Ultra;
        }

        /*
         * Whether a partition is resident for the character being played.
         *
         * Residency is decided by the character, not by the tile detail tier.
         * Every tier currently keeps every tile, and the saving comes entirely
         * from the role and race filter below: for a gnoll Priest that is 15702
         * tiles rather than the whole set's 24220, about 200 MB.
         *
         * The tier is deliberately not consulted yet. It could only separate
         * the tiers by 81 tiles -- of 338 animations, 281 are classified
         * essential, 38 decorative and 19 standard, and dropping the decorative
         * ones would take every lit torch, brazier and fireplace out of the
         * dungeon for two megabytes. Four steps that differ by that much are
         * arithmetic dressed up as a quality setting.
         *
         * `tier` is kept in the signature because this is where the steps come
         * back once the tile set is larger and more animation is classified
         * standard or decorative. At that point each tier takes a different
         * share, and the partition's `animationClass` -- essential, standard or
         * decorative -- plus its layer are what it has to judge on.
         */
        private static bool IsResident(TilePartition partition, TileDetailTier tier,
            string role, string race)
        {
            if (partition == null)
                return false;

            /* Bootstrap is tested first, before the role filter, and is never
               dropped. It spans every role, so filtering it by the chosen
               character would throw away the 292 base player tiles that every
               chargen menu draws; older manifests label it with whichever role
               happened to come first, which makes this ordering load-bearing. */
            if (partition.partitionClass == "bootstrap")
                return true;

            if (partition.IsPlayerRole)
            {
                /* No role chosen yet: keep them all. */
                if (string.IsNullOrEmpty(role))
                    return true;
                if (!MatchesRole(partition, role, race))
                    return false;
            }

            /* Kept for the character, at every tier. See the note above the
               method for why the tier is not consulted and what has to change
               before it is. */
            return true;
        }

        /*
         * Whether a player partition belongs to the character being played.
         *
         * Role and race only. Gender and alignment are deliberately *not*
         * filtered even though the manifest carries both, because neither is
         * fixed for the life of a character: an amulet of change swaps gender,
         * and a helm of opposite alignment swaps alignment. Filtering on the
         * value chosen at character generation would leave the tiles for the
         * other gender -- and, for a Priest, the other two alignments --
         * unloaded, so the player would turn into placeholders the moment
         * either changed, with nothing to trigger a recomposition.
         *
         * The cost of keeping them all is small. Both genders of an ordinary
         * role is about 174 tiles instead of 87, and the worst case in the set
         * is a gnoll Priest at 1440 instead of 240 -- 29 MB -- because Priest
         * is the only role with alignment-specific art.
         */
        private static bool MatchesRole(TilePartition partition, string role,
            string race)
        {
            return Matches(partition.role, role)
                && Matches(partition.race, race);
        }

        private static bool Matches(string partitionValue, string chosen)
        {
            if (string.IsNullOrEmpty(partitionValue) || string.IsNullOrEmpty(chosen))
                return true;
            /* "any" is what the tile data writes for a role with no alignment
               or gender fan-out, and it matches whatever was chosen. */
            if (partitionValue == "any" || chosen == "any")
                return true;
            return string.Equals(partitionValue, chosen, StringComparison.OrdinalIgnoreCase);
        }

        /*
         * Group the kept partitions by layer and fill sheets layer by layer, so
         * that a layer's tiles stay together. The player role goes last and
         * gets whatever sheet it lands in, which in practice is its own.
         */
        private static void AssignToSheets(TileCompositionPlan plan,
            List<TilePartition> kept)
        {
            int capacity = plan.SheetWidthInTiles * plan.MaxRowsPerSheet;

            List<List<TilePartition>> layerGroups = new List<List<TilePartition>>();
            Dictionary<string, List<TilePartition>> byLayer =
                new Dictionary<string, List<TilePartition>>();
            List<TilePartition> playerPartitions = new List<TilePartition>();

            foreach (TilePartition partition in kept)
            {
                if (partition.IsPlayerRole)
                {
                    playerPartitions.Add(partition);
                    continue;
                }
                string layer = string.IsNullOrEmpty(partition.layer)
                    ? "other" : partition.layer;
                List<TilePartition> group;
                if (!byLayer.TryGetValue(layer, out group))
                {
                    group = new List<TilePartition>();
                    byLayer.Add(layer, group);
                    layerGroups.Add(group);
                }
                group.Add(partition);
            }

            /* Largest layer first, so the big ones get a sheet to themselves
               before the small ones fragment the space. */
            layerGroups.Sort((a, b) => TileTotal(b).CompareTo(TileTotal(a)));
            if (playerPartitions.Count > 0)
                layerGroups.Add(playerPartitions);

            TileSheetPlan current = NewSheet(plan);
            /* Slot 0 of sheet 0 belongs to the missing tile placeholder. */
            current.TilesUsed = PlaceholderSlots;

            foreach (List<TilePartition> group in layerGroups)
            {
                int groupTiles = TileTotal(group);

                /* Start a new sheet when the layer does not fit in what is left
                   of this one -- unless it cannot fit in an empty sheet either,
                   in which case it has to be split and there is nothing to be
                   gained by wasting the remaining space first. */
                if (current.TilesUsed + groupTiles > capacity
                    && groupTiles <= capacity
                    && plan.Sheets.Count < GHConstants.MaxTileSheets)
                {
                    current = NewSheet(plan);
                }

                foreach (TilePartition partition in group)
                {
                    if (current.TilesUsed + partition.tileCount > capacity)
                    {
                        if (plan.Sheets.Count >= GHConstants.MaxTileSheets)
                        {
                            GHApp.MaybeWriteGHLog(
                                "SolveBudget: out of tile sheets, dropping partition '"
                                + partition.name + "'.");
                            continue;
                        }
                        current = NewSheet(plan);
                    }
                    current.Partitions.Add(partition);
                    current.TilesUsed += partition.tileCount;
                }
            }
        }

        private static TileSheetPlan NewSheet(TileCompositionPlan plan)
        {
            TileSheetPlan sheet = new TileSheetPlan();
            sheet.SheetIndex = plan.Sheets.Count;
            plan.Sheets.Add(sheet);
            return sheet;
        }

        private static int TileTotal(List<TilePartition> partitions)
        {
            int total = 0;
            foreach (TilePartition partition in partitions)
                total += partition.tileCount;
            return total;
        }

        /*
         * Compose every sheet in the plan, sequentially, and publish the slot
         * table when they are all resident.
         *
         * progress is called with a 0..1 fraction and a description, on the
         * calling thread; the caller marshals it to the UI thread. Decoding and
         * drawing happen here, off the UI thread -- today's loading path decodes
         * on the awaiting main thread, which is what makes the loading screen
         * stutter while sheets load.
         */
        public static async Task<bool> ComposeAsync(TileCompositionPlan plan,
            Action<double, string> progress)
        {
            if (plan == null || plan.Sheets.Count == 0)
                return false;

            await _compositionLock.WaitAsync();
            try
            {
                int[] slots = new int[GHConstants.MaxTiles];
                short[] partitionOf = new short[GHConstants.MaxTiles];
                for (int i = 0; i < partitionOf.Length; i++)
                    partitionOf[i] = GHApp.PlaceholderPartition;

                for (int i = 0; i < plan.Sheets.Count; i++)
                {
                    TileSheetPlan sheet = plan.Sheets[i];
                    if (progress != null)
                    {
                        progress((double)i / plan.Sheets.Count,
                            string.Format("Composing Tile Sheet {0}/{1}...",
                                i + 1, plan.Sheets.Count));
                    }

                    SKImage composed = await ComposeSheetAsync(plan, sheet, slots,
                        partitionOf);
                    if (composed == null)
                        return false;

                    GHApp.PublishTileSheet(sheet.SheetIndex, composed);
                }

                GHApp.PublishSlotTable(slots, partitionOf);
                if (progress != null)
                    progress(1.0, "Tile sheets ready.");
                return true;
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("ComposeAsync: " + ex.Message);
                return false;
            }
            finally
            {
                _compositionLock.Release();
            }
        }

        /*
         * Compose one destination sheet.
         *
         * The sheet is trimmed to the rows actually used, so memory scales
         * continuously with the tile count rather than in 192 MiB steps.
         */
        private static async Task<SKImage> ComposeSheetAsync(TileCompositionPlan plan,
            TileSheetPlan sheet, int[] slots, short[] partitionOf)
        {
            int rows = (sheet.TilesUsed + plan.SheetWidthInTiles - 1)
                       / plan.SheetWidthInTiles;
            if (rows <= 0)
                return null;

            int widthInPixels = plan.SheetWidthInTiles * GHConstants.TileWidth;
            int heightInPixels = rows * GHConstants.TileHeight;

            SKBitmap destination = new SKBitmap(widthInPixels, heightInPixels,
                SKColorType.Rgba8888, SKAlphaType.Premul);

            using (SKCanvas canvas = new SKCanvas(destination))
            {
                canvas.Clear(SKColors.Transparent);

                int cursor = sheet.SheetIndex == 0 ? PlaceholderSlots : 0;
                /* Reused across partitions; CopyLinearRun refills it each time. */
                List<TileRectangle> destinationRects = new List<TileRectangle>();
                foreach (TilePartition partition in sheet.Partitions)
                {
                    SKBitmap source = await DecodePartitionAsync(partition);
                    if (source == null)
                    {
                        GHApp.MaybeWriteGHLog("ComposeSheetAsync: could not decode '"
                            + partition.file + "'.");
                        destination.Dispose();
                        return null;
                    }

                    try
                    {
                        destinationRects.Clear();
                        CopyLinearRun(canvas, source, partition, plan, cursor,
                            destinationRects);
                        /* Radial transparency is baked here rather than per
                           frame. It has to happen before the next partition is
                           copied, because destinationRects describes this
                           partition's ground only. */
                        if (partition.radial)
                            ApplyRadialMask(canvas, destinationRects);
                        AssignSlots(partition, sheet.SheetIndex, cursor, plan,
                            slots, partitionOf);
                    }
                    finally
                    {
                        /* Rule 2: dispose immediately, before the next decode.
                           Holding these would cost roughly a gigabyte. */
                        source.Dispose();
                    }

                    cursor += partition.tileCount;
                }
            }

            destination.SetImmutable();
            SKImage image = SKImage.FromBitmap(destination);
            if (image != null)
                GHApp.AddUsedBitmapBytes(image.Info.BytesSize64);
            return image;
        }

        private static async Task<SKBitmap> DecodePartitionAsync(TilePartition partition)
        {
            try
            {
                using (Stream stream = await GHApp.PlatformService.GetPlatformAssetsStreamAsync(
                    GHConstants.AssetsTilesetDirectory, partition.file))
                {
                    if (stream == null)
                        return null;
                    return SKBitmap.Decode(stream);
                }
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("DecodePartitionAsync (" + partition.file + "): "
                    + ex.Message);
                return null;
            }
        }

        /*
         * Copy a partition's atlas into the destination sheet.
         *
         * Both images lay their tiles out row-major, but not at the same pitch.
         * The atlas is `partition.widthInTiles` wide, which the tile set
         * compiler sets to min(128, tileCount); the destination is
         * `plan.SheetWidthInTiles`, which is derived from the device's texture
         * limit and is 64 on a 4096 px device. Reading the source at the
         * destination's pitch therefore works only when the two happen to
         * agree -- which they do at 128, and do not at 64, where 68 of the 110
         * partitions in the current set are read at the wrong stride and every
         * tile in them lands somewhere else. That is what mangled the map on a
         * texture-limited device while leaving desktop correct.
         *
         * So the copy walks segments that stay inside one row on both sides at
         * once. The whole-atlas DrawImage is kept for the case where the
         * pitches match and the destination cursor is row-aligned, which is
         * still the common one.
         */
        private static void CopyLinearRun(SKCanvas canvas, SKBitmap source,
            TilePartition partition, TileCompositionPlan plan, int dstStart,
            List<TileRectangle> destinationRects)
        {
            int count = partition.tileCount;
            int dstPitch = plan.SheetWidthInTiles;
            int srcPitch = partition.widthInTiles > 0
                ? partition.widthInTiles : dstPitch;

            using (SKImage sourceImage = SKImage.FromBitmap(source))
            {
                if (sourceImage == null)
                    return;

                if (srcPitch == dstPitch && dstStart % dstPitch == 0)
                {
                    /* Fast path: the whole atlas lands as one rectangle. */
                    DrawTileRect(canvas, sourceImage, 0, 0,
                        Math.Min(dstPitch, count),
                        (count + dstPitch - 1) / dstPitch, 0, dstStart / dstPitch,
                        destinationRects);
                    return;
                }

                int i = 0;
                while (i < count)
                {
                    int srcX = i % srcPitch;
                    int srcY = i / srcPitch;
                    int dstLinear = dstStart + i;
                    int dstX = dstLinear % dstPitch;
                    int dstY = dstLinear / dstPitch;

                    /* The longest span that stays within one source row and
                       one destination row. */
                    int n = Math.Min(count - i,
                                Math.Min(srcPitch - srcX, dstPitch - dstX));

                    DrawTileRect(canvas, sourceImage, srcX, srcY, n, 1,
                        dstX, dstY, destinationRects);
                    i += n;
                }
            }
        }

        private static void DrawTileRect(SKCanvas canvas, SKImage source,
            int srcTileX, int srcTileY, int widthInTiles, int heightInTiles,
            int dstTileX, int dstTileY, List<TileRectangle> destinationRects)
        {
            SKRect sourceRect = new SKRect(
                srcTileX * GHConstants.TileWidth,
                srcTileY * GHConstants.TileHeight,
                (srcTileX + widthInTiles) * GHConstants.TileWidth,
                (srcTileY + heightInTiles) * GHConstants.TileHeight);
            SKRect destinationRect = new SKRect(
                dstTileX * GHConstants.TileWidth,
                dstTileY * GHConstants.TileHeight,
                (dstTileX + widthInTiles) * GHConstants.TileWidth,
                (dstTileY + heightInTiles) * GHConstants.TileHeight);

            canvas.DrawImage(source, sourceRect, destinationRect
#if GNH_MAUI
                , new SKSamplingOptions(SKFilterMode.Nearest, SKMipmapMode.None)
#endif
                );

            if (destinationRects != null)
            {
                destinationRects.Add(new TileRectangle(dstTileX, dstTileY,
                    widthInTiles, heightInTiles));
            }
        }

        /*
         * Bake radial transparency into a partition's destination rectangles.
         *
         * M5_RADIAL_TRANSPARENCY is a species flag, so a radially-transparent
         * monster's tile is drawn radially every time it is drawn. That made
         * the old per-frame implementation pure waste: GamePage read the tile
         * back with PeekPixels and ran a per-pixel Math.Sqrt / Math.Pow loop
         * for a result that never varied. Doing it once, here, retires that
         * loop and the SKImage cache that existed to hide its cost.
         *
         * The mask reproduces GamePage.ProcessRadialTile's falloff:
         *
         *     r  = distance from (TileWidth / 2 - 0.5, TileHeight / 2 - 0.5)
         *     st = min(0.98, r * 0.0375)
         *     alpha *= 1 - st
         *
         * Applied with DstIn, which scales the destination by the mask's alpha
         * and leaves everything outside the drawn rectangles untouched.
         */
        private static readonly object _radialMaskLock = new object();
        private static SKImage _radialMaskImage;

        /*
         * The 64 x 96 mask tile, built once and kept.
         *
         * Unpremul with white RGB: under DstIn only the alpha channel is
         * consulted, and leaving RGB at full white keeps the image meaningful
         * if it is ever dumped for debugging.
         */
        private static SKImage GetRadialMaskImage()
        {
            lock (_radialMaskLock)
            {
                if (_radialMaskImage != null)
                    return _radialMaskImage;

                int width = GHConstants.TileWidth;
                int height = GHConstants.TileHeight;
                double midX = (double)width / 2.0 - 0.5;
                double midY = (double)height / 2.0 - 0.5;

                SKBitmap mask = new SKBitmap(width, height,
                    SKColorType.Rgba8888, SKAlphaType.Unpremul);
                int row, col;
                for (row = 0; row < height; row++)
                {
                    for (col = 0; col < width; col++)
                    {
                        double dx = (double)col - midX;
                        double dy = (double)row - midY;
                        double r = Math.Sqrt(dx * dx + dy * dy);
                        double semiTransparency = r * 0.0375; /* r_constant */
                        if (semiTransparency > 0.98)
                            semiTransparency = 0.98;
                        byte alpha = (byte)((double)0xFF * (1.0 - semiTransparency));
                        mask.SetPixel(col, row,
                            new SKColor(0xFF, 0xFF, 0xFF, alpha));
                    }
                }
                mask.SetImmutable();
                _radialMaskImage = SKImage.FromBitmap(mask);
                mask.Dispose();
                return _radialMaskImage;
            }
        }

        /*
         * Paint the mask over exactly the rectangles CopyLinearRun just wrote.
         *
         * The shader repeats on a 64 x 96 period anchored at the destination
         * sheet's origin, not at each rectangle's own top-left. That matters:
         * SKShaderTileMode.Repeat phases from the shader's local coordinate
         * space, so a shader left at its default origin would restart its
         * period inside every rectangle and offset the mask by a fraction of a
         * tile. Anchoring at (0, 0) needs no per-rectangle local matrix,
         * because the canvas has the identity matrix and every rectangle
         * CopyLinearRun produces is tile-aligned in the destination -- which is
         * also why a partition spanning two destination rows per source row is
         * safe here.
         */
        private static void ApplyRadialMask(SKCanvas canvas,
            List<TileRectangle> destinationRects)
        {
            if (destinationRects == null || destinationRects.Count == 0)
                return;

            SKImage mask = GetRadialMaskImage();
            if (mask == null)
                return;

            using (SKShader shader = mask.ToShader(SKShaderTileMode.Repeat,
                       SKShaderTileMode.Repeat
#if GNH_MAUI
                       /* The mapping is 1:1, so nearest is both exact and the
                          cheapest; it also removes any question of a filtered
                          sample bleeding across the repeat seam. */
                       , new SKSamplingOptions(SKFilterMode.Nearest, SKMipmapMode.None)
#endif
                       ))
            using (SKPaint paint = new SKPaint())
            {
                paint.Shader = shader;
                paint.BlendMode = SKBlendMode.DstIn;
                paint.IsAntialias = false;

                foreach (TileRectangle rect in destinationRects)
                {
                    canvas.DrawRect(new SKRect(
                        rect.X * GHConstants.TileWidth,
                        rect.Y * GHConstants.TileHeight,
                        (rect.X + rect.Width) * GHConstants.TileWidth,
                        (rect.Y + rect.Height) * GHConstants.TileHeight),
                        paint);
                }
            }
        }

        /*
         * Record where every tile of a partition ended up.
         *
         * The runs are walked in manifest order with a running index into the
         * partition, which is exactly the order the tile set compiler packed
         * them into the atlas. If those two orders ever disagree, every tile in
         * the partition renders as some other tile, silently -- hence the
         * assertion at the end, and the HasConsistentRuns() check at load.
         */
        private static void AssignSlots(TilePartition partition, int sheetIndex,
            int dstStart, TileCompositionPlan plan, int[] slots, short[] partitionOf)
        {
            int pitch = plan.SheetWidthInTiles;
            int indexInPartition = 0;

            foreach (TileRun run in partition.runs)
            {
                for (int k = 0; k < run.tileCount; k++)
                {
                    int tile = run.firstTile + k;
                    if (tile < 0 || tile >= slots.Length)
                        continue;

                    int slot = dstStart + indexInPartition + k;
                    slots[tile] = TileSlot.Pack(sheetIndex,
                        (slot % pitch) * GHConstants.TileWidth,
                        (slot / pitch) * GHConstants.TileHeight);
                    partitionOf[tile] = (short)partition.id;
                }
                indexInPartition += run.tileCount;
            }

            if (indexInPartition != partition.tileCount)
            {
                throw new Exception("Partition '" + partition.name
                    + "' covers " + indexInPartition + " tiles but claims "
                    + partition.tileCount + ".");
            }
        }
    }
}
