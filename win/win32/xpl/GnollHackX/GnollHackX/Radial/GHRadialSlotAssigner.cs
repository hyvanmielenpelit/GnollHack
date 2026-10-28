using System;
using System.Collections.Generic;

namespace GnollHackX
{
    /* Remembers the slot each context item had, per slot count and page, so that an item keeps
       its slot between openings while the set around it changes. The host keeps one instance
       for the game session. */
    public sealed class GHRadialSlotMemory
    {
        private readonly Dictionary<string, int> _slots = new Dictionary<string, int>(StringComparer.Ordinal);

        public int Count { get { return _slots.Count; } }

        private static string MakeKey(string id, int slotCount, int pageIndex)
        {
            return slotCount.ToString() + "|" + pageIndex.ToString() + "|" + id;
        }

        public bool TryGetSlot(string id, int slotCount, int pageIndex, out int slot)
        {
            slot = -1;
            if (id == null)
                return false;
            return _slots.TryGetValue(MakeKey(id, slotCount, pageIndex), out slot);
        }

        public void SetSlot(string id, int slotCount, int pageIndex, int slot)
        {
            if (id == null)
                return;
            string key = MakeKey(id, slotCount, pageIndex);
            if (!_slots.ContainsKey(key) && _slots.Count >= GHRadialTuning.SlotMemoryCapacity)
                _slots.Clear();
            _slots[key] = slot;
        }

        public void Clear()
        {
            _slots.Clear();
        }
    }

    /* Places items into fixed radial slots.
       - Fixed pages (the Toggles radial): every item has a compass direction and takes the slot
         nearest it; the slot count is the smaller of 8 and 12 that gives every item its own slot.
       - Context pages (the Actions radial's first pages): ContextSlotCount slots, or
         CrowdedSlotCount when more items are present, overflow onto further context pages. Fixed items (the left-side buttons) take the slot nearest their
         direction; context items keep their remembered slot when it is free, else take E, NE,
         SE, N, S, then the free slot nearest east.
       - More Commands pages: each 4x6 matrix page becomes two 12-slot pages, rows 0-2 and rows
         3-5; cell (column i, row j) takes slot (j mod 3) * 4 + i. Cells the caller dropped stay
         empty, so positions never shift.
       - Packed pages (the commands list): items in the given order, slotsPerPage to a page,
         clockwise from north with no gaps; only the last page has empty slots.
       Free of UI types, so that the unit tests compile it. */
    public static class GHRadialSlotAssigner
    {
        public static string GetContextItemId(int command, string label)
        {
            return "ctx:" + command.ToString() + ":" + (label ?? "");
        }

        public static string GetMoreItemId(int page, int column, int row)
        {
            return "more:" + page.ToString() + ":" + column.ToString() + ":" + row.ToString();
        }

        /* The slot nearest a fixed direction. Ties go toward the side the direction lies on: west
           for the western half, east otherwise, so that NW and SW stay symmetric about W. */
        public static int GetFixedSlot(double compassDegrees, int slotCount)
        {
            double a = GHRadialGeometry.NormalizeDegrees(compassDegrees);
            double bias = a > 180.0 ? GHRadialTuning.West : GHRadialTuning.East;
            return GHRadialGeometry.NearestSlot(a, slotCount, bias);
        }

        /* E, NE, SE, N, S (ties toward east), then every other slot by distance from east, ties
           to the lower index */
        public static int[] GetContextPreferenceOrder(int slotCount)
        {
            int[] order = new int[slotCount];
            bool[] used = new bool[slotCount];
            int count = 0;
            int i;
            for (i = 0; i < GHRadialTuning.ContextPreferredAngles.Length; i++)
            {
                int k = GHRadialGeometry.NearestSlot(GHRadialTuning.ContextPreferredAngles[i], slotCount, GHRadialTuning.East);
                if (k >= 0 && !used[k])
                {
                    used[k] = true;
                    order[count++] = k;
                }
            }
            while (count < slotCount)
            {
                int best = -1;
                double bestDelta = double.MaxValue;
                for (i = 0; i < slotCount; i++)
                {
                    if (used[i])
                        continue;
                    double d = Math.Abs(GHRadialGeometry.AngleDelta(GHRadialGeometry.SlotCenterDegrees(i, slotCount), GHRadialTuning.East));
                    if (d < bestDelta - 1e-9)
                    {
                        bestDelta = d;
                        best = i;
                    }
                }
                used[best] = true;
                order[count++] = best;
            }
            return order;
        }

        /* The smallest slot count of FixedPageSlotCounts, from FixedPageMinSlotCount up, that
           holds every item and gives each item with a direction its own nearest slot; 12 when
           none does */
        public static int ChooseFixedSlotCount(IList<GHRadialItem> items)
        {
            int itemCount = CountNonNull(items);
            int c;
            for (c = 0; c < GHRadialTuning.FixedPageSlotCounts.Length; c++)
            {
                int n = GHRadialTuning.FixedPageSlotCounts[c];
                if (n < GHRadialTuning.FixedPageMinSlotCount || itemCount > n)
                    continue;
                bool[] taken = new bool[n];
                bool fits = true;
                int i;
                for (i = 0; items != null && i < items.Count && fits; i++)
                {
                    GHRadialItem item = items[i];
                    if (item == null || !item.HasPreferredAngle)
                        continue;
                    int k = GetFixedSlot(item.PreferredAngle, n);
                    if (taken[k])
                        fits = false;
                    else
                        taken[k] = true;
                }
                if (fits)
                    return n;
            }
            return GHRadialTuning.MaxSlotCount;
        }

        /* A page of items with fixed directions, such as the toggles. Items without a direction
           fill the remaining slots clockwise from north. */
        public static GHRadialPage BuildFixedPage(string title, int groupIndex, IList<GHRadialItem> items)
        {
            int n = ChooseFixedSlotCount(items);
            GHRadialPage page = new GHRadialPage(title, n);
            page.GroupIndex = groupIndex;
            PlaceFixedItems(page, items);
            int i;
            for (i = 0; items != null && i < items.Count; i++)
            {
                GHRadialItem item = items[i];
                if (item == null || item.HasPreferredAngle)
                    continue;
                int k = FirstFreeSlot(page.Slots);
                if (k < 0)
                    break;
                page.Slots[k] = item;
            }
            return page;
        }

        /* The Actions radial's context pages. Fixed items (left-side buttons, with directions) and
           context items (in the core's rank order) share the first page: ContextSlotCount slots
           for up to that many items, else CrowdedSlotCount; at most CrowdedSlotCount fixed items
           are kept. Context items that do not fit go on further pages titled "<title> 2",
           "<title> 3", ..., sized the same way. Every page gets groupIndex. Memory may be null. */
        public static List<GHRadialPage> AssignContextPages(string title, int groupIndex, IList<GHRadialItem> fixedItems,
            IList<GHRadialItem> contextItems, GHRadialSlotMemory memory)
        {
            List<GHRadialPage> pages = new List<GHRadialPage>();
            List<GHRadialItem> fixedList = new List<GHRadialItem>();
            List<GHRadialItem> contextList = new List<GHRadialItem>();
            int i;
            /* A fixed item without a direction is placed like a context item ranked first */
            for (i = 0; fixedItems != null && i < fixedItems.Count; i++)
            {
                GHRadialItem item = fixedItems[i];
                if (item == null)
                    continue;
                if (!item.HasPreferredAngle)
                    contextList.Add(item);
                else if (fixedList.Count < GHRadialTuning.CrowdedSlotCount)
                    fixedList.Add(item);
            }
            for (i = 0; contextItems != null && i < contextItems.Count; i++)
            {
                if (contextItems[i] != null)
                    contextList.Add(contextItems[i]);
            }

            int total = fixedList.Count + contextList.Count;
            int firstCount = total <= GHRadialTuning.ContextSlotCount ? GHRadialTuning.ContextSlotCount : GHRadialTuning.CrowdedSlotCount;
            GHRadialPage first = new GHRadialPage(title, firstCount);
            first.GroupIndex = groupIndex;
            PlaceFixedItems(first, fixedList);
            int capacity = Math.Max(0, firstCount - fixedList.Count);
            int taken = Math.Min(capacity, contextList.Count);
            PlaceContextItems(first, 0, contextList, 0, taken, memory);
            pages.Add(first);

            int next = taken;
            while (next < contextList.Count)
            {
                int chunk = Math.Min(GHRadialTuning.CrowdedSlotCount, contextList.Count - next);
                int n = chunk <= GHRadialTuning.ContextSlotCount ? GHRadialTuning.ContextSlotCount : GHRadialTuning.CrowdedSlotCount;
                GHRadialPage page = new GHRadialPage((title ?? "") + " " + (pages.Count + 1).ToString(), n);
                page.GroupIndex = groupIndex;
                page.Group = title;
                PlaceContextItems(page, pages.Count, contextList, next, chunk, memory);
                pages.Add(page);
                next += chunk;
            }
            return pages;
        }

        public static GHRadialItem CreateCommandItem(GHRadialSourceItem source)
        {
            GHRadialItem item = new GHRadialItem();
            item.Id = source.Id ?? GetMoreItemId(source.Page, source.Column, source.Row);
            item.Label = source.Label;
            item.Description = source.Description;
            item.IconPath = source.IconPath;
            item.ShortcutText = source.ShortcutText;
            item.Kind = GHRadialItemKind.Command;
            item.ClosesOnActivate = true;
            item.Tag = source;
            return item;
        }

        /* Matrix pages in radial order: 1 .. currentMoreButtonPageMaxNumber - 1, then the wizard
           mode page 0 last. The bound is exclusive, as GamePage's own More Commands loops use it. */
        public static int[] GetMorePageOrder(int currentMoreButtonPageMaxNumber, bool wizardMode)
        {
            List<int> order = new List<int>();
            int p;
            for (p = 1; p < currentMoreButtonPageMaxNumber; p++)
                order.Add(p);
            if (wizardMode)
                order.Add(0);
            return order.ToArray();
        }

        /* The slot of a More Commands cell within its half page */
        public static int GetMoreSlot(int column, int row)
        {
            return (row % GHRadialTuning.MoreRowsPerRadialPage) * GHRadialTuning.MoreColumns + column;
        }

        /* Two 12-slot pages per matrix page in pageOrder, titled "<title> (1/2)" and "(2/2)" from
           pageTitles indexed by matrix page. The pages of the g-th matrix page get group index
           firstGroupIndex + g. Cells on pages outside pageOrder or outside the matrix are ignored;
           of two cells on one slot, the first wins. */
        public static List<GHRadialPage> BuildMorePages(IList<GHRadialSourceItem> cells, IList<int> pageOrder,
            IList<string> pageTitles, int firstGroupIndex)
        {
            List<GHRadialPage> pages = new List<GHRadialPage>();
            if (pageOrder == null)
                return pages;

            int halves = (GHRadialTuning.MoreRows + GHRadialTuning.MoreRowsPerRadialPage - 1) / GHRadialTuning.MoreRowsPerRadialPage;
            int g;
            for (g = 0; g < pageOrder.Count; g++)
            {
                int matrixPage = pageOrder[g];
                string title = pageTitles != null && matrixPage >= 0 && matrixPage < pageTitles.Count ? pageTitles[matrixPage] : null;
                if (title == null)
                    title = "";
                int firstIndex = pages.Count;
                int h;
                for (h = 0; h < halves; h++)
                {
                    GHRadialPage page = new GHRadialPage(title + " (" + (h + 1).ToString() + "/" + halves.ToString() + ")", GHRadialTuning.MorePageSlotCount);
                    page.Group = title;
                    page.GroupIndex = firstGroupIndex + g;
                    pages.Add(page);
                }

                int i;
                for (i = 0; cells != null && i < cells.Count; i++)
                {
                    GHRadialSourceItem cell = cells[i];
                    if (cell == null || cell.Page != matrixPage)
                        continue;
                    if (cell.Column < 0 || cell.Column >= GHRadialTuning.MoreColumns || cell.Row < 0 || cell.Row >= GHRadialTuning.MoreRows)
                        continue;
                    GHRadialPage page = pages[firstIndex + cell.Row / GHRadialTuning.MoreRowsPerRadialPage];
                    int slot = GetMoreSlot(cell.Column, cell.Row);
                    if (slot < 0 || slot >= page.SlotCount || page.Slots[slot] != null)
                        continue;
                    page.Slots[slot] = CreateCommandItem(cell);
                }
            }
            return pages;
        }

        /* The items in order, slotsPerPage to a page: item k of a page takes slot k, so reading
           clockwise from north follows the list. Every page has slotsPerPage slots, the last one
           with its remaining slots empty. Every page has the Name titlePrefix and the Counter
           "1/N" .. "N/N", counted within these pages, and the Title "<titlePrefix> <Counter>"; a
           single page has no counter and the Title titlePrefix. Every page gets groupIndex. Null
           items are skipped; no items give no pages. slotsPerPage is 6, 8 or 12. */
        public static List<GHRadialPage> BuildPackedPages(IList<GHRadialSourceItem> items, string titlePrefix, int slotsPerPage,
            int groupIndex)
        {
            if (Array.IndexOf(GHRadialTuning.FixedPageSlotCounts, slotsPerPage) < 0)
                throw new ArgumentOutOfRangeException("slotsPerPage", slotsPerPage, "Unsupported radial slot count");

            List<GHRadialPage> pages = new List<GHRadialPage>();
            List<GHRadialSourceItem> list = new List<GHRadialSourceItem>();
            int i;
            for (i = 0; items != null && i < items.Count; i++)
            {
                if (items[i] != null)
                    list.Add(items[i]);
            }
            if (list.Count == 0)
                return pages;

            string prefix = titlePrefix ?? "";
            int pageCount = (list.Count + slotsPerPage - 1) / slotsPerPage;
            int p;
            for (p = 0; p < pageCount; p++)
            {
                string counter = pageCount == 1 ? null : (p + 1).ToString() + "/" + pageCount.ToString();
                string title = counter == null ? prefix : prefix + " " + counter;
                GHRadialPage page = new GHRadialPage(title, slotsPerPage);
                page.Name = prefix;
                page.Counter = counter;
                page.Group = prefix;
                page.GroupIndex = groupIndex;
                int k;
                for (k = 0; k < slotsPerPage && p * slotsPerPage + k < list.Count; k++)
                    page.Slots[k] = CreateCommandItem(list[p * slotsPerPage + k]);
                pages.Add(page);
            }
            return pages;
        }

        private static void PlaceFixedItems(GHRadialPage page, IList<GHRadialItem> items)
        {
            int n = page.SlotCount;
            int i;
            for (i = 0; items != null && i < items.Count; i++)
            {
                GHRadialItem item = items[i];
                if (item == null || !item.HasPreferredAngle)
                    continue;
                int k = GetFixedSlot(item.PreferredAngle, n);
                if (page.Slots[k] != null)
                    k = NearestFreeSlot(page.Slots, item.PreferredAngle);
                if (k < 0)
                    return;
                page.Slots[k] = item;
            }
        }

        private static void PlaceContextItems(GHRadialPage page, int pageIndex, List<GHRadialItem> items, int start, int count,
            GHRadialSlotMemory memory)
        {
            int n = page.SlotCount;
            int[] placedAt = new int[count];
            int i;
            for (i = 0; i < count; i++)
                placedAt[i] = -1;

            /* Remembered slots first, in rank order */
            if (memory != null)
            {
                for (i = 0; i < count; i++)
                {
                    int slot;
                    if (memory.TryGetSlot(items[start + i].Id, n, pageIndex, out slot) && slot >= 0 && slot < n && page.Slots[slot] == null)
                    {
                        page.Slots[slot] = items[start + i];
                        placedAt[i] = slot;
                    }
                }
            }

            int[] order = GetContextPreferenceOrder(n);
            for (i = 0; i < count; i++)
            {
                if (placedAt[i] >= 0)
                    continue;
                int j;
                for (j = 0; j < order.Length; j++)
                {
                    if (page.Slots[order[j]] == null)
                    {
                        page.Slots[order[j]] = items[start + i];
                        placedAt[i] = order[j];
                        break;
                    }
                }
            }

            if (memory != null)
            {
                for (i = 0; i < count; i++)
                {
                    if (placedAt[i] >= 0)
                        memory.SetSlot(items[start + i].Id, n, pageIndex, placedAt[i]);
                }
            }
        }

        private static int NearestFreeSlot(GHRadialItem[] slots, double compassDegrees)
        {
            int n = slots.Length;
            int best = -1;
            double bestDelta = double.MaxValue;
            int k;
            for (k = 0; k < n; k++)
            {
                if (slots[k] != null)
                    continue;
                double d = Math.Abs(GHRadialGeometry.AngleDelta(GHRadialGeometry.SlotCenterDegrees(k, n), compassDegrees));
                if (d < bestDelta - 1e-9)
                {
                    bestDelta = d;
                    best = k;
                }
            }
            return best;
        }

        private static int FirstFreeSlot(GHRadialItem[] slots)
        {
            int k;
            for (k = 0; k < slots.Length; k++)
            {
                if (slots[k] == null)
                    return k;
            }
            return -1;
        }

        private static int CountNonNull(IList<GHRadialItem> items)
        {
            int n = 0;
            int i;
            for (i = 0; items != null && i < items.Count; i++)
            {
                if (items[i] != null)
                    n++;
            }
            return n;
        }
    }
}
