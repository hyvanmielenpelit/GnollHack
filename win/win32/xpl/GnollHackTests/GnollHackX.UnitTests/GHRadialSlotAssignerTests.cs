using System;
using System.Collections.Generic;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers GHRadialSlotAssigner: the left-side buttons' fixed directions under 8 and 12 slots,
       the context preference order, the stability properties of Design C section 3 (the same set
       gives the same slots, adding or removing an item moves no other), the switch to 12 slots
       and the overflow page, the L3 page with R74's Center on Hero at the E slot, and the More
       Commands cell-to-slot mapping, dropped cells, page order, wizard pages last and the simple
       layout's page count (R16). */
    public class GHRadialSlotAssignerTests
    {
        private static GHRadialItem Item(string id)
        {
            return new GHRadialItem { Id = id, Label = id };
        }

        private static GHRadialItem Fixed(string id, double angle)
        {
            return new GHRadialItem { Id = id, Label = id, PreferredAngle = angle };
        }

        private static List<GHRadialItem> LeftButtons()
        {
            return new List<GHRadialItem>
            {
                Fixed("left:skills", GHRadialTuning.SkillsAngle),
                Fixed("left:polearm", GHRadialTuning.PolearmAngle),
                Fixed("left:wieldlast", GHRadialTuning.WieldLastAngle)
            };
        }

        private static List<GHRadialItem> Context(params string[] ids)
        {
            List<GHRadialItem> res = new List<GHRadialItem>();
            int i;
            for (i = 0; i < ids.Length; i++)
                res.Add(Item(ids[i]));
            return res;
        }

        private static List<GHRadialItem> Toggles()
        {
            return new List<GHRadialItem>
            {
                new GHRadialItem { Id = "l3:statusbar", Kind = GHRadialItemKind.Toggle, ClosesOnActivate = false, PreferredAngle = GHRadialTuning.ExtendedStatusBarAngle },
                new GHRadialItem { Id = "l3:autocenter", Kind = GHRadialItemKind.Toggle, ClosesOnActivate = false, PreferredAngle = GHRadialTuning.AutoCenterAngle },
                new GHRadialItem { Id = "l3:look", Kind = GHRadialItemKind.Toggle, ClosesOnActivate = true, PreferredAngle = GHRadialTuning.LookModeAngle },
                new GHRadialItem { Id = "l3:messages", Kind = GHRadialItemKind.Toggle, ClosesOnActivate = false, PreferredAngle = GHRadialTuning.MoreMessagesAngle },
                new GHRadialItem { Id = "l3:ignore", Kind = GHRadialItemKind.Toggle, ClosesOnActivate = false, PreferredAngle = GHRadialTuning.IgnoreStoppingAngle },
                new GHRadialItem { Id = "l3:autodig", Kind = GHRadialItemKind.Toggle, ClosesOnActivate = false, PreferredAngle = GHRadialTuning.AutoDigAngle }
            };
        }

        private static GHRadialSourceItem Cell(int page, int column, int row)
        {
            return new GHRadialSourceItem { Label = "c" + page + column + row, Command = 100 + row * 4 + column, Page = page, Column = column, Row = row };
        }

        private static List<GHRadialSourceItem> FullMatrix(params int[] pages)
        {
            List<GHRadialSourceItem> cells = new List<GHRadialSourceItem>();
            foreach (int p in pages)
            {
                int i, j;
                for (j = 0; j < GHRadialTuning.MoreRows; j++)
                {
                    for (i = 0; i < GHRadialTuning.MoreColumns; i++)
                        cells.Add(Cell(p, i, j));
                }
            }
            return cells;
        }

        [Fact]
        public void LeftButtons_KeepTheirDirectionsAtEightAndTwelveSlots()
        {
            Assert.Equal(6, GHRadialSlotAssigner.GetFixedSlot(GHRadialTuning.SkillsAngle, 8));
            Assert.Equal(7, GHRadialSlotAssigner.GetFixedSlot(GHRadialTuning.PolearmAngle, 8));
            Assert.Equal(5, GHRadialSlotAssigner.GetFixedSlot(GHRadialTuning.WieldLastAngle, 8));
            /* At 12 slots, NW and SW fall between two slots and go toward west: 300 and 240 */
            Assert.Equal(9, GHRadialSlotAssigner.GetFixedSlot(GHRadialTuning.SkillsAngle, 12));
            Assert.Equal(10, GHRadialSlotAssigner.GetFixedSlot(GHRadialTuning.PolearmAngle, 12));
            Assert.Equal(8, GHRadialSlotAssigner.GetFixedSlot(GHRadialTuning.WieldLastAngle, 12));
        }

        [Fact]
        public void ContextPreference_IsEastFirst()
        {
            Assert.Equal(new int[] { 2, 1, 3, 0, 4, 5, 7, 6 }, GHRadialSlotAssigner.GetContextPreferenceOrder(8));
            Assert.Equal(new int[] { 3, 2, 4, 0, 6, 1, 5, 7, 11, 8, 10, 9 }, GHRadialSlotAssigner.GetContextPreferenceOrder(12));
        }

        [Fact]
        public void ContextPage_PlacesLeftButtonsWestAndContextEast()
        {
            List<GHRadialPage> pages = GHRadialSlotAssigner.AssignContextPages("Context", 0, LeftButtons(), Context("a", "b", "c", "d", "e"), null);
            Assert.Single(pages);
            GHRadialPage p = pages[0];
            Assert.Equal(8, p.SlotCount);
            Assert.Equal("left:skills", p.Slots[6].Id);
            Assert.Equal("left:polearm", p.Slots[7].Id);
            Assert.Equal("left:wieldlast", p.Slots[5].Id);
            Assert.Equal("a", p.Slots[2].Id);
            Assert.Equal("b", p.Slots[1].Id);
            Assert.Equal("c", p.Slots[3].Id);
            Assert.Equal("d", p.Slots[0].Id);
            Assert.Equal("e", p.Slots[4].Id);
        }

        [Fact]
        public void SameSet_GivesSameSlots()
        {
            GHRadialSlotMemory memory = new GHRadialSlotMemory();
            GHRadialPage first = GHRadialSlotAssigner.AssignContextPages("Context", 0, LeftButtons(), Context("a", "b", "c"), memory)[0];
            GHRadialPage second = GHRadialSlotAssigner.AssignContextPages("Context", 0, LeftButtons(), Context("a", "b", "c"), memory)[0];
            GHRadialPage fresh = GHRadialSlotAssigner.AssignContextPages("Context", 0, LeftButtons(), Context("a", "b", "c"), null)[0];
            int k;
            for (k = 0; k < first.SlotCount; k++)
            {
                Assert.Equal(first.Slots[k] == null ? null : first.Slots[k].Id, second.Slots[k] == null ? null : second.Slots[k].Id);
                Assert.Equal(first.Slots[k] == null ? null : first.Slots[k].Id, fresh.Slots[k] == null ? null : fresh.Slots[k].Id);
            }
        }

        [Fact]
        public void AddingAnItem_MovesNoOther()
        {
            GHRadialSlotMemory memory = new GHRadialSlotMemory();
            GHRadialPage before = GHRadialSlotAssigner.AssignContextPages("Context", 0, LeftButtons(), Context("a", "b", "c"), memory)[0];
            /* The new item ranks first, but the others keep their slots */
            GHRadialPage after = GHRadialSlotAssigner.AssignContextPages("Context", 0, LeftButtons(), Context("x", "a", "b", "c"), memory)[0];
            foreach (string id in new string[] { "a", "b", "c", "left:skills", "left:polearm", "left:wieldlast" })
                Assert.Equal(before.IndexOf(id), after.IndexOf(id));
            Assert.Equal(0, after.IndexOf("x"));
        }

        [Fact]
        public void RemovingAnItem_MovesNoOther()
        {
            GHRadialSlotMemory memory = new GHRadialSlotMemory();
            GHRadialPage before = GHRadialSlotAssigner.AssignContextPages("Context", 0, LeftButtons(), Context("a", "b", "c", "d"), memory)[0];
            GHRadialPage after = GHRadialSlotAssigner.AssignContextPages("Context", 0, LeftButtons(), Context("a", "c", "d"), memory)[0];
            foreach (string id in new string[] { "a", "c", "d", "left:skills" })
                Assert.Equal(before.IndexOf(id), after.IndexOf(id));
            Assert.Null(after.Slots[before.IndexOf("b")]);
        }

        [Fact]
        public void RememberedSlot_YieldsToAFixedButton()
        {
            GHRadialSlotMemory memory = new GHRadialSlotMemory();
            /* With eight context items and no left buttons, "h" ends up west */
            GHRadialPage before = GHRadialSlotAssigner.AssignContextPages("Context", 0, null, Context("a", "b", "c", "d", "e", "f", "g", "h"), memory)[0];
            Assert.Equal(6, before.IndexOf("h"));
            GHRadialPage after = GHRadialSlotAssigner.AssignContextPages("Context", 0,
                new List<GHRadialItem> { Fixed("left:skills", GHRadialTuning.SkillsAngle) }, Context("h"), memory)[0];
            Assert.Equal(6, after.IndexOf("left:skills"));
            Assert.Equal(2, after.IndexOf("h"));
        }

        [Fact]
        public void IdsDistinguishGetposSets()
        {
            Assert.NotEqual(GHRadialSlotAssigner.GetContextItemId(1, "Next"), GHRadialSlotAssigner.GetContextItemId(2, "Next"));
            Assert.Equal("ctx:44:Pick Up", GHRadialSlotAssigner.GetContextItemId(44, "Pick Up"));
        }

        [Fact]
        public void NineItems_UseTwelveSlots()
        {
            List<GHRadialPage> eight = GHRadialSlotAssigner.AssignContextPages("Context", 0, LeftButtons(), Context("a", "b", "c", "d", "e"), null);
            Assert.Equal(8, eight[0].SlotCount);
            List<GHRadialPage> nine = GHRadialSlotAssigner.AssignContextPages("Context", 0, LeftButtons(), Context("a", "b", "c", "d", "e", "f"), null);
            Assert.Single(nine);
            Assert.Equal(12, nine[0].SlotCount);
            Assert.Equal(9, nine[0].FilledCount);
            Assert.Equal(9, nine[0].IndexOf("left:skills"));
            Assert.Equal(3, nine[0].IndexOf("a"));
        }

        [Fact]
        public void Overflow_GoesToASecondContextPage()
        {
            string[] ids = new string[12];
            int i;
            for (i = 0; i < ids.Length; i++)
                ids[i] = "c" + i;
            List<GHRadialPage> pages = GHRadialSlotAssigner.AssignContextPages("Context", 3, LeftButtons(), Context(ids), new GHRadialSlotMemory());
            Assert.Equal(2, pages.Count);
            Assert.Equal(12, pages[0].SlotCount);
            Assert.Equal(12, pages[0].FilledCount);
            Assert.Equal("Context 2", pages[1].Title);
            Assert.Equal(8, pages[1].SlotCount);
            Assert.Equal(3, pages[1].FilledCount);
            Assert.Equal(3, pages[0].GroupIndex);
            Assert.Equal(3, pages[1].GroupIndex);
            Assert.True(pages[1].IndexOf("c9") >= 0);
            Assert.Equal(-1, pages[0].IndexOf("c9"));
        }

        [Fact]
        public void EmptyContext_StillGivesOnePage()
        {
            List<GHRadialPage> pages = GHRadialSlotAssigner.AssignContextPages("Context", 0, null, null, null);
            Assert.Single(pages);
            Assert.Equal(8, pages[0].SlotCount);
            Assert.Equal(0, pages[0].FilledCount);
        }

        [Fact]
        public void Toggles_TakeSixSlotsAtTheirDirections()
        {
            GHRadialPage page = GHRadialSlotAssigner.BuildFixedPage("Toggles", 0, Toggles());
            Assert.Equal(6, page.SlotCount);
            Assert.Equal("l3:statusbar", page.Slots[0].Id);
            Assert.Equal("l3:autocenter", page.Slots[1].Id);
            Assert.Equal("l3:look", page.Slots[2].Id);
            Assert.Equal("l3:messages", page.Slots[3].Id);
            Assert.Equal("l3:ignore", page.Slots[4].Id);
            Assert.Equal("l3:autodig", page.Slots[5].Id);
        }

        [Fact]
        public void CenterOnHero_TakesTheEmptyEastSlot()
        {
            List<GHRadialItem> items = Toggles();
            GHRadialItem center = new GHRadialItem
            {
                Id = "l3:centeronhero",
                Kind = GHRadialItemKind.Command,
                ClosesOnActivate = true,
                PreferredAngle = GHRadialTuning.CenterOnHeroAngle
            };
            items.Add(center);
            GHRadialPage page = GHRadialSlotAssigner.BuildFixedPage("Toggles", 0, items);

            /* Eight slots: the toggles stay within 15 degrees of their directions, E is the action */
            Assert.Equal(8, page.SlotCount);
            Assert.Same(center, page.Slots[2]);
            Assert.Equal("l3:statusbar", page.Slots[0].Id);
            Assert.Equal("l3:autocenter", page.Slots[1].Id);
            Assert.Equal("l3:look", page.Slots[3].Id);
            Assert.Equal("l3:messages", page.Slots[4].Id);
            Assert.Equal("l3:ignore", page.Slots[5].Id);
            Assert.Null(page.Slots[6]);
            Assert.Equal("l3:autodig", page.Slots[7].Id);
            Assert.Equal(GHRadialItemKind.Command, page.Slots[2].Kind);
            Assert.True(page.Slots[2].ClosesOnActivate);
        }

        [Fact]
        public void ChooseFixedSlotCount_GrowsOnCollision()
        {
            Assert.Equal(6, GHRadialSlotAssigner.ChooseFixedSlotCount(Toggles()));
            List<GHRadialItem> crowded = new List<GHRadialItem>();
            int i;
            for (i = 0; i < 9; i++)
                crowded.Add(Fixed("f" + i, i * 40.0));
            Assert.Equal(12, GHRadialSlotAssigner.ChooseFixedSlotCount(crowded));
        }

        [Theory]
        [InlineData(0, 0, 0)]
        [InlineData(3, 0, 3)]
        [InlineData(0, 1, 4)]
        [InlineData(3, 2, 11)]
        [InlineData(0, 3, 0)]
        [InlineData(2, 5, 10)]
        public void MoreCell_MapsToFixedSlot(int column, int row, int slot)
        {
            Assert.Equal(slot, GHRadialSlotAssigner.GetMoreSlot(column, row));
        }

        [Fact]
        public void MorePages_SplitEachMatrixPageInTwo()
        {
            string[] titles = { "Wizard Mode Commands", "Common Commands", "Additional Commands", "Context and More Commands" };
            int[] order = GHRadialSlotAssigner.GetMorePageOrder(4, true);
            List<GHRadialPage> pages = GHRadialSlotAssigner.BuildMorePages(FullMatrix(0, 1, 2, 3), order, titles, 1);
            Assert.Equal(8, pages.Count);
            Assert.Equal("Common Commands (1/2)", pages[0].Title);
            Assert.Equal("Common Commands (2/2)", pages[1].Title);
            Assert.Equal("Wizard Mode Commands (1/2)", pages[6].Title);
            Assert.Equal("Wizard Mode Commands (2/2)", pages[7].Title);
            int[] groups = { 1, 1, 2, 2, 3, 3, 4, 4 };
            int i;
            for (i = 0; i < pages.Count; i++)
            {
                Assert.Equal(groups[i], pages[i].GroupIndex);
                Assert.Equal(12, pages[i].SlotCount);
                Assert.Equal(12, pages[i].FilledCount);
            }
            /* Row 4, column 1 of Common is on the second half, slot (4 mod 3) * 4 + 1 = 5 */
            GHRadialSourceItem tag = (GHRadialSourceItem)pages[1].Slots[5].Tag;
            Assert.Equal(1, tag.Page);
            Assert.Equal(1, tag.Column);
            Assert.Equal(4, tag.Row);
            Assert.Equal("more:1:1:4", pages[1].Slots[5].Id);
            Assert.Equal(GHRadialItemKind.Command, pages[1].Slots[5].Kind);
            Assert.True(pages[1].Slots[5].ClosesOnActivate);
        }

        [Fact]
        public void DroppedCells_StayEmptyWithoutShifting()
        {
            List<GHRadialSourceItem> cells = FullMatrix(1);
            /* The caller drops Back to Game at column 3, row 5 and an empty cell at column 0, row 1 */
            cells.RemoveAll(c => (c.Column == 3 && c.Row == 5) || (c.Column == 0 && c.Row == 1));
            List<GHRadialPage> pages = GHRadialSlotAssigner.BuildMorePages(cells, new int[] { 1 }, new string[] { "", "Common" }, 1);
            Assert.Equal(2, pages.Count);
            Assert.Null(pages[0].Slots[4]);
            Assert.Null(pages[1].Slots[11]);
            Assert.Equal(11, pages[0].FilledCount);
            Assert.Equal(11, pages[1].FilledCount);
            GHRadialSourceItem neighbor = (GHRadialSourceItem)pages[0].Slots[5].Tag;
            Assert.Equal(1, neighbor.Column);
            Assert.Equal(1, neighbor.Row);
        }

        [Fact]
        public void MorePages_IgnoreCellsOutsideTheOrderOrMatrix()
        {
            List<GHRadialSourceItem> cells = new List<GHRadialSourceItem>
            {
                Cell(2, 0, 0),
                Cell(1, 4, 0),
                Cell(1, 0, 6),
                Cell(1, 1, 1),
                Cell(1, 1, 1)
            };
            List<GHRadialPage> pages = GHRadialSlotAssigner.BuildMorePages(cells, new int[] { 1 }, null, 1);
            Assert.Equal(1, pages[0].FilledCount + pages[1].FilledCount);
            Assert.NotNull(pages[0].Slots[5]);
            Assert.Equal(" (1/2)", pages[0].Title);
        }

        [Fact]
        public void PageOrder_PutsWizardLastAndRespectsTheSimpleLayout()
        {
            Assert.Equal(new int[] { 1, 2, 3 }, GHRadialSlotAssigner.GetMorePageOrder(4, false));
            Assert.Equal(new int[] { 1, 2, 3, 0 }, GHRadialSlotAssigner.GetMorePageOrder(4, true));
            /* The simple layout has one matrix page fewer */
            Assert.Equal(new int[] { 1, 2 }, GHRadialSlotAssigner.GetMorePageOrder(3, false));
            Assert.Equal(new int[] { 1, 2, 0 }, GHRadialSlotAssigner.GetMorePageOrder(3, true));
        }

        [Fact]
        public void CreateCommandItem_CarriesTheSource()
        {
            GHRadialSourceItem src = new GHRadialSourceItem { Label = "Pray", Description = "Pray to your god", IconPath = "icon", Command = 7, Page = 2, Column = 1, Row = 3, ShortcutText = "M-p" };
            GHRadialItem item = GHRadialSlotAssigner.CreateCommandItem(src);
            Assert.Same(src, item.Tag);
            Assert.Equal("more:2:1:3", item.Id);
            Assert.Equal("Pray", item.Label);
            Assert.Equal("Pray to your god", item.Description);
            Assert.Equal("icon", item.IconPath);
            Assert.Equal("M-p", item.ShortcutText);
            src.Id = "custom";
            Assert.Equal("custom", GHRadialSlotAssigner.CreateCommandItem(src).Id);
        }

        [Fact]
        public void SlotMemory_ForgetsEverythingWhenFull()
        {
            GHRadialSlotMemory memory = new GHRadialSlotMemory();
            int i;
            for (i = 0; i < GHRadialTuning.SlotMemoryCapacity; i++)
                memory.SetSlot("id" + i, 8, 0, i % 8);
            Assert.Equal(GHRadialTuning.SlotMemoryCapacity, memory.Count);
            memory.SetSlot("id0", 8, 0, 3);
            Assert.Equal(GHRadialTuning.SlotMemoryCapacity, memory.Count);
            memory.SetSlot("new", 8, 0, 1);
            Assert.Equal(1, memory.Count);
            int slot;
            Assert.True(memory.TryGetSlot("new", 8, 0, out slot));
            Assert.Equal(1, slot);
            Assert.False(memory.TryGetSlot("new", 12, 0, out slot));
        }
    }
}
