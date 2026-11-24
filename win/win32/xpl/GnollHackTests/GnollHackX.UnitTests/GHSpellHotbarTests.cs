using System;
using System.Collections.Generic;
using System.Linq;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers GHSpellHotbar: the slot rule shared with set_spell_hotbar_slot (assign, swap,
       replace, remove, ignored input, stored values outside 1..24),
       slot / page / direction mapping, the school ring threshold, the Known Spells orders, the
       Place mode hub line, the first empty slot and the NEW tracker; and GHSpellMenuParser on
       Cast menu rows as GHMenuItem splits them, with and without special symbols. */
    public class GHSpellHotbarTests
    {
        private static GHSpellInfo Spell(int spellId, int slot, int skill = 0, int level = 1, string name = null)
        {
            GHSpellInfo spell = new GHSpellInfo();
            spell.SpellId = spellId;
            spell.HotbarSlot = slot;
            spell.Skill = skill;
            spell.Level = level;
            spell.Name = name ?? ("spell" + spellId);
            return spell;
        }

        private static string Dot
        {
            get { return ((char)0x00B7).ToString(); }
        }

        /* Slot rule */

        [Fact]
        public void ApplySetSlot_AssignsAnEmptySlot()
        {
            GHSpellInfo a = Spell(1, 0);
            GHSpellInfo b = Spell(2, 3);
            List<GHSpellInfo> spells = new List<GHSpellInfo> { a, b };

            GHSpellHotbar.ApplySetSlot(spells, 1, 5);

            Assert.Equal(5, a.HotbarSlot);
            Assert.Equal(3, b.HotbarSlot);
        }

        [Fact]
        public void ApplySetSlot_MovingOntoAFilledSlotSwaps()
        {
            GHSpellInfo a = Spell(1, 3);
            GHSpellInfo b = Spell(2, 5);
            List<GHSpellInfo> spells = new List<GHSpellInfo> { a, b };

            GHSpellHotbar.ApplySetSlot(spells, 1, 5);

            Assert.Equal(5, a.HotbarSlot);
            Assert.Equal(3, b.HotbarSlot);
        }

        [Fact]
        public void ApplySetSlot_FromNoSlotOntoAFilledSlotReplaces()
        {
            GHSpellInfo a = Spell(1, 0);
            GHSpellInfo b = Spell(2, 5);
            List<GHSpellInfo> spells = new List<GHSpellInfo> { a, b };

            GHSpellHotbar.ApplySetSlot(spells, 1, 5);

            Assert.Equal(5, a.HotbarSlot);
            Assert.Equal(0, b.HotbarSlot);
        }

        [Fact]
        public void ApplySetSlot_ZeroRemovesOnlyThatSpell()
        {
            GHSpellInfo a = Spell(1, 5);
            GHSpellInfo b = Spell(2, 6);
            List<GHSpellInfo> spells = new List<GHSpellInfo> { a, b };

            GHSpellHotbar.ApplySetSlot(spells, 1, 0);

            Assert.Equal(0, a.HotbarSlot);
            Assert.Equal(6, b.HotbarSlot);
        }

        [Fact]
        public void ApplySetSlot_SameSlotChangesNothing()
        {
            GHSpellInfo a = Spell(1, 5);
            GHSpellInfo b = Spell(2, 6);
            List<GHSpellInfo> spells = new List<GHSpellInfo> { a, b };

            GHSpellHotbar.ApplySetSlot(spells, 1, 5);

            Assert.Equal(5, a.HotbarSlot);
            Assert.Equal(6, b.HotbarSlot);
        }

        [Theory]
        [InlineData(-1)]
        [InlineData(25)]
        [InlineData(100)]
        public void ApplySetSlot_IgnoresASlotOutOfRange(int slot)
        {
            GHSpellInfo a = Spell(1, 3);
            GHSpellInfo b = Spell(2, 5);
            List<GHSpellInfo> spells = new List<GHSpellInfo> { a, b };

            GHSpellHotbar.ApplySetSlot(spells, 1, slot);

            Assert.Equal(3, a.HotbarSlot);
            Assert.Equal(5, b.HotbarSlot);
        }

        [Fact]
        public void ApplySetSlot_IgnoresAnUnknownSpellId()
        {
            GHSpellInfo a = Spell(1, 3);
            GHSpellInfo b = Spell(2, 5);
            List<GHSpellInfo> spells = new List<GHSpellInfo> { a, b };

            GHSpellHotbar.ApplySetSlot(spells, 99, 5);
            GHSpellHotbar.ApplySetSlot(spells, 99, 0);

            Assert.Equal(3, a.HotbarSlot);
            Assert.Equal(5, b.HotbarSlot);
        }

        [Fact]
        public void ApplySetSlot_ToleratesNullListAndEntries()
        {
            GHSpellHotbar.ApplySetSlot(null, 1, 5);

            GHSpellInfo a = Spell(1, 0);
            List<GHSpellInfo> spells = new List<GHSpellInfo> { null, a };
            GHSpellHotbar.ApplySetSlot(spells, 1, 5);
            Assert.Equal(5, a.HotbarSlot);
        }

        [Theory]
        [InlineData(30)]
        [InlineData(-4)]
        public void ApplySetSlot_AStoredSlotOutOfRangeCountsAsNone(int stored)
        {
            GHSpellInfo a = Spell(1, stored);
            GHSpellInfo b = Spell(2, 5);
            List<GHSpellInfo> spells = new List<GHSpellInfo> { a, b };

            GHSpellHotbar.ApplySetSlot(spells, 1, 5);

            Assert.Equal(5, a.HotbarSlot);
            Assert.Equal(0, b.HotbarSlot);
        }

        [Fact]
        public void ApplySetSlot_ASpellWithAStoredSlotOutOfRangeHoldsNothing()
        {
            GHSpellInfo a = Spell(1, 0);
            GHSpellInfo b = Spell(2, 30);
            List<GHSpellInfo> spells = new List<GHSpellInfo> { a, b };

            GHSpellHotbar.ApplySetSlot(spells, 1, 6);

            Assert.Equal(6, a.HotbarSlot);
            Assert.Equal(30, b.HotbarSlot);
        }

        [Theory]
        [InlineData(0, 0)]
        [InlineData(1, 1)]
        [InlineData(24, 24)]
        [InlineData(25, 0)]
        [InlineData(30, 0)]
        [InlineData(-1, 0)]
        public void StoredSlotsOutside1To24ReadAsNotOnTheHotbar(int stored, int expected)
        {
            GHSpellInfo a = Spell(1, stored);

            Assert.Equal(expected, GHSpellHotbar.NormalizeSlot(stored));
            Assert.Equal(expected, GHSpellHotbar.GetSlot(a));
            Assert.Equal(expected != 0, GHSpellHotbar.IsOnHotbar(a));
        }

        [Fact]
        public void FindBySlot_SkipsStoredSlotsOutOfRange()
        {
            GHSpellInfo a = Spell(1, 30);
            GHSpellInfo b = Spell(2, 4);
            List<GHSpellInfo> spells = new List<GHSpellInfo> { a, b };

            Assert.Null(GHSpellHotbar.FindBySlot(spells, 30));
            Assert.Null(GHSpellHotbar.FindBySlot(spells, 0));
            Assert.Null(GHSpellHotbar.FindBySlot(spells, 3));
            Assert.Same(b, GHSpellHotbar.FindBySlot(spells, 4));
        }

        [Fact]
        public void FindBySpellId_FindsOrReturnsNull()
        {
            GHSpellInfo a = Spell(1, 0);
            GHSpellInfo b = Spell(2, 0);
            List<GHSpellInfo> spells = new List<GHSpellInfo> { a, null, b };

            Assert.Same(b, GHSpellHotbar.FindBySpellId(spells, 2));
            Assert.Null(GHSpellHotbar.FindBySpellId(spells, 3));
            Assert.Null(GHSpellHotbar.FindBySpellId(null, 1));
        }

        /* Slots, pages and directions */

        [Theory]
        [InlineData(1, 0, 0)]
        [InlineData(2, 0, 1)]
        [InlineData(8, 0, 7)]
        [InlineData(9, 1, 0)]
        [InlineData(16, 1, 7)]
        [InlineData(17, 2, 0)]
        [InlineData(24, 2, 7)]
        public void SlotMapsToPageAndDirection(int slot, int page, int dir)
        {
            Assert.Equal(page, GHSpellHotbar.SlotToPage(slot));
            Assert.Equal(dir, GHSpellHotbar.SlotToDirection(slot));
            Assert.Equal(slot, GHSpellHotbar.PageDirectionToSlot(page, dir));
        }

        [Fact]
        public void EverySlotRoundTrips()
        {
            int slot;
            for (slot = 1; slot <= GHSpellHotbar.MaxSlots; slot++)
                Assert.Equal(slot, GHSpellHotbar.PageDirectionToSlot(GHSpellHotbar.SlotToPage(slot), GHSpellHotbar.SlotToDirection(slot)));
            Assert.Equal(3, GHSpellHotbar.PageCount);
        }

        [Theory]
        [InlineData(0)]
        [InlineData(25)]
        [InlineData(-1)]
        public void SlotOutOfRangeHasNoPageOrDirection(int slot)
        {
            Assert.Equal(-1, GHSpellHotbar.SlotToPage(slot));
            Assert.Equal(-1, GHSpellHotbar.SlotToDirection(slot));
            Assert.Equal("", GHSpellHotbar.GetSlotLabel(slot));
        }

        [Theory]
        [InlineData(-1, 0)]
        [InlineData(3, 0)]
        [InlineData(0, -1)]
        [InlineData(0, 8)]
        public void PageOrDirectionOutOfRangeHasNoSlot(int page, int dir)
        {
            Assert.Equal(0, GHSpellHotbar.PageDirectionToSlot(page, dir));
        }

        [Theory]
        [InlineData(0, "North")]
        [InlineData(1, "Northeast")]
        [InlineData(2, "East")]
        [InlineData(3, "Southeast")]
        [InlineData(4, "South")]
        [InlineData(5, "Southwest")]
        [InlineData(6, "West")]
        [InlineData(7, "Northwest")]
        [InlineData(8, "")]
        [InlineData(-1, "")]
        public void DirectionNames(int dir, string expected)
        {
            Assert.Equal(expected, GHSpellHotbar.GetDirectionName(dir));
        }

        [Fact]
        public void SlotLabelNamesPageAndDirection()
        {
            Assert.Equal("Hotbar 1 " + Dot + " North", GHSpellHotbar.GetSlotLabel(1));
            Assert.Equal("Hotbar 1 " + Dot + " East", GHSpellHotbar.GetSlotLabel(3));
            Assert.Equal("Hotbar 2 " + Dot + " West", GHSpellHotbar.GetSlotLabel(15));
            Assert.Equal("Hotbar 3 " + Dot + " Northwest", GHSpellHotbar.GetSlotLabel(24));
        }

        /* School ring */

        [Theory]
        [InlineData(0, false)]
        [InlineData(8, false)]
        [InlineData(16, false)]
        [InlineData(17, true)]
        [InlineData(40, true)]
        public void SchoolRingAbove16(int known, bool expected)
        {
            Assert.Equal(expected, GHSpellHotbar.UsesSchoolRing(known));
        }

        [Fact]
        public void CountBySchoolSkipsUnknownSchools()
        {
            List<GHSpellInfo> spells = new List<GHSpellInfo>
            {
                Spell(1, 0, skill: 0),
                Spell(2, 0, skill: 0),
                Spell(3, 0, skill: 11),
                Spell(4, 0, skill: -1),
                Spell(5, 0, skill: 12),
                null,
            };

            int[] counts = GHSpellHotbar.CountBySchool(spells);

            Assert.Equal(12, counts.Length);
            Assert.Equal(2, counts[0]);
            Assert.Equal(1, counts[11]);
            Assert.Equal(3, counts.Sum());
            Assert.Equal(0, GHSpellHotbar.CountBySchool(null).Sum());
        }

        /* Orders */

        [Fact]
        public void FlatOrderIsSkillThenLevelWithCantripsFirstThenName()
        {
            List<GHSpellInfo> spells = new List<GHSpellInfo>
            {
                Spell(1, 0, skill: 2, level: 1, name: "zap"),
                Spell(2, 0, skill: 1, level: 3, name: "bolt"),
                Spell(3, 0, skill: 1, level: 1, name: "Sleep"),
                Spell(4, 0, skill: 1, level: 1, name: "light"),
                Spell(5, 0, skill: 1, level: 0, name: "major"),
                Spell(6, 0, skill: 1, level: -1, name: "minor"),
                null,
                Spell(7, 0, skill: 2, level: 0, name: "cantrip"),
            };

            List<GHSpellInfo> order = GHSpellHotbar.GetFlatOrder(spells);

            Assert.Equal(new[] { 6, 5, 4, 3, 2, 7, 1 }, order.Select(s => s.SpellId).ToArray());
        }

        [Fact]
        public void FlatOrderPutsUnknownSchoolAndLevelLast()
        {
            List<GHSpellInfo> spells = new List<GHSpellInfo>
            {
                Spell(1, 0, skill: -1, level: int.MinValue, name: "forgotten"),
                Spell(2, 0, skill: 11, level: 5, name: "necro"),
                Spell(3, 0, skill: 0, level: int.MinValue, name: "odd"),
                Spell(4, 0, skill: 0, level: 7, name: "arc"),
            };

            List<GHSpellInfo> order = GHSpellHotbar.GetFlatOrder(spells);

            Assert.Equal(new[] { 4, 3, 2, 1 }, order.Select(s => s.SpellId).ToArray());
        }

        [Fact]
        public void NameOrderIgnoresCase()
        {
            List<GHSpellInfo> spells = new List<GHSpellInfo>
            {
                Spell(1, 0, skill: 1, level: 1, name: "charm"),
                Spell(2, 0, skill: 1, level: 1, name: "Bless"),
                Spell(3, 0, skill: 1, level: 1, name: "apport"),
                Spell(4, 0, skill: 1, level: 1, name: "Daze"),
            };

            List<GHSpellInfo> order = GHSpellHotbar.GetFlatOrder(spells);

            Assert.Equal(new[] { 3, 2, 1, 4 }, order.Select(s => s.SpellId).ToArray());
        }

        [Fact]
        public void SchoolOrderKeepsOneSchoolByLevelThenName()
        {
            List<GHSpellInfo> spells = new List<GHSpellInfo>
            {
                Spell(1, 0, skill: 3, level: 2, name: "b"),
                Spell(2, 0, skill: 4, level: 1, name: "other"),
                Spell(3, 0, skill: 3, level: 2, name: "A"),
                Spell(4, 0, skill: 3, level: -1, name: "z"),
                Spell(5, 0, skill: 3, level: 0, name: "y"),
                Spell(6, 0, skill: 3, level: 1, name: "c"),
            };

            List<GHSpellInfo> order = GHSpellHotbar.GetSchoolOrder(spells, 3);

            Assert.Equal(new[] { 4, 5, 6, 3, 1 }, order.Select(s => s.SpellId).ToArray());
            Assert.Empty(GHSpellHotbar.GetSchoolOrder(spells, 9));
            Assert.Empty(GHSpellHotbar.GetSchoolOrder(null, 3));
        }

        [Fact]
        public void OrderDoesNotChangeTheSource()
        {
            GHSpellInfo a = Spell(1, 0, skill: 2, name: "a");
            GHSpellInfo b = Spell(2, 0, skill: 1, name: "b");
            List<GHSpellInfo> spells = new List<GHSpellInfo> { a, b };

            GHSpellHotbar.GetFlatOrder(spells);

            Assert.Same(a, spells[0]);
            Assert.Same(b, spells[1]);
        }

        /* Place mode hub line */

        [Fact]
        public void PlaceHubLine_EmptyTargetIsNull()
        {
            Assert.Null(GHSpellHotbar.GetPlaceHubLine(Spell(1, 3), null));
            Assert.Null(GHSpellHotbar.GetPlaceHubLine(Spell(1, 0), null));
        }

        [Fact]
        public void PlaceHubLine_SwapsWhenCarriedHasASlot()
        {
            Assert.Equal("swaps with Sleep", GHSpellHotbar.GetPlaceHubLine(Spell(1, 3), Spell(2, 5, name: "Sleep")));
        }

        [Fact]
        public void PlaceHubLine_ReplacesWhenCarriedHasNoSlot()
        {
            Assert.Equal("replaces Sleep", GHSpellHotbar.GetPlaceHubLine(Spell(1, 0), Spell(2, 5, name: "Sleep")));
            Assert.Equal("replaces Sleep", GHSpellHotbar.GetPlaceHubLine(Spell(1, 30), Spell(2, 5, name: "Sleep")));
        }

        [Fact]
        public void PlaceHubLine_OwnSlotIsNull()
        {
            GHSpellInfo a = Spell(1, 3);
            Assert.Null(GHSpellHotbar.GetPlaceHubLine(a, a));
            Assert.Null(GHSpellHotbar.GetPlaceHubLine(a, Spell(1, 3)));
        }

        /* First empty slot */

        [Fact]
        public void FirstEmptySlot_EmptyHotbarIsSlot1()
        {
            Assert.Equal(1, GHSpellHotbar.FirstEmptySlot(new List<GHSpellInfo>()));
            Assert.Equal(1, GHSpellHotbar.FirstEmptySlot(null));
            Assert.Equal(1, GHSpellHotbar.FirstEmptySlot(new List<GHSpellInfo> { Spell(1, 0), Spell(2, 30), null }));
        }

        [Fact]
        public void FirstEmptySlot_FindsTheLowestGap()
        {
            List<GHSpellInfo> spells = new List<GHSpellInfo> { Spell(1, 1), Spell(2, 2), Spell(3, 4) };
            Assert.Equal(3, GHSpellHotbar.FirstEmptySlot(spells));
        }

        [Fact]
        public void FirstEmptySlot_FullHotbarIsZero()
        {
            List<GHSpellInfo> spells = new List<GHSpellInfo>();
            int slot;
            for (slot = 1; slot <= GHSpellHotbar.MaxSlots; slot++)
                spells.Add(Spell(slot, slot));
            spells.Add(Spell(100, 0));

            Assert.Equal(0, GHSpellHotbar.FirstEmptySlot(spells));

            spells[9].HotbarSlot = 0;
            Assert.Equal(10, GHSpellHotbar.FirstEmptySlot(spells));
        }

        /* NEW tracker */

        [Fact]
        public void NewTracker_MarksSpellsLearnedSinceTheBaseline()
        {
            GHSpellNewTracker tracker = new GHSpellNewTracker();
            tracker.SetBaseline(new[] { 1, 2 });

            HashSet<int> marked = tracker.Update(new[] { 1, 2, 3 });

            Assert.Equal(new[] { 3 }, marked.OrderBy(x => x).ToArray());
            Assert.True(tracker.IsNew(3));
            Assert.False(tracker.IsNew(1));
        }

        [Fact]
        public void NewTracker_KeepsMarksUntilAcknowledged()
        {
            GHSpellNewTracker tracker = new GHSpellNewTracker();
            tracker.SetBaseline(new[] { 1, 2 });
            tracker.Update(new[] { 1, 2, 3 });

            Assert.Equal(new[] { 3 }, tracker.Update(new[] { 1, 2, 3 }).OrderBy(x => x).ToArray());
            Assert.Equal(new[] { 3, 4 }, tracker.Update(new[] { 1, 2, 3, 4 }).OrderBy(x => x).ToArray());

            tracker.Acknowledge();

            Assert.Empty(tracker.Update(new[] { 1, 2, 3, 4 }));
            Assert.False(tracker.IsNew(3));
            Assert.Equal(new[] { 5 }, tracker.Update(new[] { 1, 2, 3, 4, 5 }).OrderBy(x => x).ToArray());
        }

        [Fact]
        public void NewTracker_ForgottenSpellLosesItsMark()
        {
            GHSpellNewTracker tracker = new GHSpellNewTracker();
            tracker.SetBaseline(new[] { 1 });
            tracker.Update(new[] { 1, 2 });

            Assert.Empty(tracker.Update(new[] { 1 }));
            Assert.False(tracker.IsNew(2));
        }

        [Fact]
        public void NewTracker_FirstUpdateWithoutBaselineBecomesTheBaseline()
        {
            GHSpellNewTracker tracker = new GHSpellNewTracker();
            Assert.False(tracker.HasBaseline);

            Assert.Empty(tracker.Update(new[] { 1, 2 }));
            Assert.True(tracker.HasBaseline);
            Assert.Equal(new[] { 3 }, tracker.Update(new[] { 1, 2, 3 }).OrderBy(x => x).ToArray());
        }

        [Fact]
        public void NewTracker_SetBaselineClearsMarks()
        {
            GHSpellNewTracker tracker = new GHSpellNewTracker();
            tracker.SetBaseline(new[] { 1 });
            tracker.Update(new[] { 1, 2 });

            tracker.SetBaseline(new[] { 1, 2 });

            Assert.False(tracker.IsNew(2));
            Assert.Empty(tracker.Update(new[] { 1, 2 }));
        }

        [Fact]
        public void NewTracker_ReturnsACopy()
        {
            GHSpellNewTracker tracker = new GHSpellNewTracker();
            tracker.SetBaseline(new int[0]);
            HashSet<int> marked = tracker.Update(new[] { 1 });

            marked.Clear();

            Assert.True(tracker.IsNew(1));
        }

        /* Menu parser: Suffix2Text values as GHMenuItem gives them (the text inside the braces) */

        [Fact]
        public void Parser_ReadsARowWithSpecialSymbols()
        {
            GHSpellInfo info = new GHSpellInfo();

            bool ok = GHSpellMenuParser.TryParseStats("&sparc; 1 &success; 100% &mana; 5.0 &cool; 0 &casts; 12", info);

            Assert.True(ok);
            Assert.Equal(0, info.Skill);
            Assert.Equal(1, info.Level);
            Assert.Equal("Level 1 Arcane Spell", info.LevelText);
            Assert.Equal("100%", info.SuccessText);
            Assert.Equal("5.0", info.ManaText);
            Assert.Equal("0", info.CooldownText);
            Assert.False(info.CooldownActive);
            Assert.Equal("12", info.CastsText);
            Assert.False(GHSpellMenuParser.HasCooldown(info));
        }

        [Fact]
        public void Parser_ReadsARowWithoutSpecialSymbols()
        {
            GHSpellInfo info = new GHSpellInfo();

            bool ok = GHSpellMenuParser.TryParseStats("Hea 3 Success 87% Mana 17.5 Cool 8 Casts 3", info);

            Assert.True(ok);
            Assert.Equal(2, info.Skill);
            Assert.Equal(3, info.Level);
            Assert.Equal("Level 3 Healing Spell", info.LevelText);
            Assert.Equal("87%", info.SuccessText);
            Assert.Equal("17.5", info.ManaText);
            Assert.Equal("8", info.CooldownText);
            Assert.False(info.CooldownActive);
            Assert.Equal("3", info.CastsText);
            Assert.True(GHSpellMenuParser.HasCooldown(info));
        }

        [Theory]
        [InlineData("&spnec; 4 &success; 0% &mana; 30.0 &cool; 3/12 &casts; 7")]
        [InlineData("Nec 4 Success 0% Mana 30.0 Cool 3/12 Casts 7")]
        public void Parser_ReadsACoolingDownRow(string suffix2)
        {
            GHSpellInfo info = new GHSpellInfo();

            Assert.True(GHSpellMenuParser.TryParseStats(suffix2, info));
            Assert.Equal(11, info.Skill);
            Assert.Equal("3/12", info.CooldownText);
            Assert.True(info.CooldownActive);
            Assert.True(GHSpellMenuParser.HasCooldown(info));
            Assert.Equal("0%", info.SuccessText);
        }

        [Theory]
        [InlineData("&spcle; 2 &success; 45% &mana; 10.0 &cool; 0 &casts; Inf.")]
        [InlineData("Cle 2 Success 45% Mana 10.0 Cool 0 Casts Inf.")]
        public void Parser_ReadsAnInfiniteCastsRow(string suffix2)
        {
            GHSpellInfo info = new GHSpellInfo();

            Assert.True(GHSpellMenuParser.TryParseStats(suffix2, info));
            Assert.Equal(1, info.Skill);
            Assert.Equal("Level 2 Clerical Spell", info.LevelText);
            Assert.Equal("Inf.", info.CastsText);
        }

        [Theory]
        [InlineData("&sparc; c &success; 100% &mana; 0.5 &cool; 0 &casts; Inf.", -1, "Minor Arcane Cantrip")]
        [InlineData("&sparc; C &success; 100% &mana; 1.0 &cool; 0 &casts; Inf.", 0, "Major Arcane Cantrip")]
        [InlineData("Arc c Success 100% Mana 0.5 Cool 0 Casts Inf.", -1, "Minor Arcane Cantrip")]
        [InlineData("Arc C Success 100% Mana 1.0 Cool 0 Casts Inf.", 0, "Major Arcane Cantrip")]
        [InlineData("&spdiv; 12 &success; 5% &mana; 120.0 &cool; 0 &casts; 1", 12, "Level 12 Divination Spell")]
        public void Parser_ReadsLevels(string suffix2, int level, string levelText)
        {
            GHSpellInfo info = new GHSpellInfo();

            Assert.True(GHSpellMenuParser.TryParseStats(suffix2, info));
            Assert.Equal(level, info.Level);
            Assert.Equal(levelText, info.LevelText);
        }

        [Theory]
        [InlineData("&sparc;", "Arc", 0, "Arcane")]
        [InlineData("&spcle;", "Cle", 1, "Clerical")]
        [InlineData("&sphea;", "Hea", 2, "Healing")]
        [InlineData("&spdiv;", "Div", 3, "Divination")]
        [InlineData("&spabj;", "Abj", 4, "Abjuration")]
        [InlineData("&spmov;", "Mov", 5, "Movement")]
        [InlineData("&sptra;", "Tra", 6, "Transmutation")]
        [InlineData("&spenc;", "Enc", 7, "Enchantment")]
        [InlineData("&spcon;", "Con", 8, "Conjuration")]
        [InlineData("&spcel;", "Cel", 9, "Celestial")]
        [InlineData("&spnat;", "Nat", 10, "Nature")]
        [InlineData("&spnec;", "Nec", 11, "Necromancy")]
        public void Parser_ReadsEverySchool(string symbol, string mnemonic, int skill, string school)
        {
            GHSpellInfo withSymbols = new GHSpellInfo();
            GHSpellInfo withWords = new GHSpellInfo();

            Assert.True(GHSpellMenuParser.TryParseStats(symbol + " 1 &success; 50% &mana; 5.0 &cool; 0 &casts; 2", withSymbols));
            Assert.True(GHSpellMenuParser.TryParseStats(mnemonic + " 1 Success 50% Mana 5.0 Cool 0 Casts 2", withWords));

            Assert.Equal(skill, withSymbols.Skill);
            Assert.Equal(skill, withWords.Skill);
            Assert.Equal("Level 1 " + school + " Spell", withSymbols.LevelText);
            Assert.Equal(symbol, GHSpellMenuParser.SchoolSymbols[skill]);
            Assert.Equal(school, GHSpellMenuParser.SchoolNames[skill]);
            Assert.Equal(skill, GHSpellMenuParser.GetSchoolIndex(symbol));
            Assert.Equal(skill, GHSpellMenuParser.GetSchoolIndex(mnemonic));
        }

        [Fact]
        public void Parser_SchoolTablesHaveTwelveEntries()
        {
            Assert.Equal(12, GHSpellMenuParser.SchoolSymbols.Length);
            Assert.Equal(12, GHSpellMenuParser.SchoolMnemonics.Length);
            Assert.Equal(12, GHSpellMenuParser.SchoolNames.Length);
            Assert.Equal(12, GHSpellMenuParser.SchoolCount);
        }

        [Fact]
        public void Parser_ToleratesSurroundingWhitespace()
        {
            GHSpellInfo info = new GHSpellInfo();

            Assert.True(GHSpellMenuParser.TryParseStats("  &spmov; 2  &success; 64% &mana; 7.5 &cool; 1/5 &casts; 4 ", info));
            Assert.Equal(5, info.Skill);
            Assert.Equal("1/5", info.CooldownText);
        }

        [Theory]
        [InlineData(null)]
        [InlineData("")]
        [InlineData("   ")]
        [InlineData("garbage")]
        [InlineData("&spxyz; 1 &success; 100% &mana; 5.0 &cool; 0 &casts; 12")]
        [InlineData("&sparc; * &success; 100% &mana; 5.0 &cool; 0 &casts; 12")]
        [InlineData("&sparc; 0 &success; 100% &mana; 5.0 &cool; 0 &casts; 12")]
        [InlineData("&sparc; 1 &success; 100 &mana; 5.0 &cool; 0 &casts; 12")]
        [InlineData("&sparc; 1 &success; 100% &mana; five &cool; 0 &casts; 12")]
        [InlineData("&sparc; 1 &success; 100% &mana; 5.0 &cool; 3/ &casts; 12")]
        [InlineData("&sparc; 1 &success; 100% &mana; 5.0 &cool; 0 &casts; Inf")]
        [InlineData("&sparc; 1 &success; 100% &mana; 5.0 &cool; 0")]
        [InlineData("&sparc; 1 &success; 100% &mana; 5.0 &cool; 0 &casts; 12 extra")]
        [InlineData("&sparc; 1 &success; 100% &success; 100% &cool; 0 &casts; 12")]
        [InlineData("&sparc; 1 &power; 100% &mana; 5.0 &cool; 0 &casts; 12")]
        public void Parser_RejectsAMalformedRowAndLeavesTheStatsNull(string suffix2)
        {
            GHSpellInfo info = new GHSpellInfo();
            info.Skill = 3;
            info.Level = 2;
            info.LevelText = "old";
            info.SuccessText = "old";
            info.ManaText = "old";
            info.CooldownText = "1/2";
            info.CooldownActive = true;
            info.CastsText = "old";

            Assert.False(GHSpellMenuParser.TryParseStats(suffix2, info));
            Assert.Equal(-1, info.Skill);
            Assert.Equal(int.MinValue, info.Level);
            Assert.Null(info.LevelText);
            Assert.Null(info.SuccessText);
            Assert.Null(info.ManaText);
            Assert.Null(info.CooldownText);
            Assert.False(info.CooldownActive);
            Assert.Null(info.CastsText);
            Assert.False(GHSpellMenuParser.HasCooldown(info));
        }

        [Fact]
        public void Parser_NullInfoIsFalse()
        {
            Assert.False(GHSpellMenuParser.TryParseStats("&sparc; 1 &success; 100% &mana; 5.0 &cool; 0 &casts; 12", null));
            Assert.False(GHSpellMenuParser.FillFromRow(null, "Force bolt ", "", ""));
        }

        /* Menu parser: whole rows as MainText, SuffixText and Suffix2Text */

        [Fact]
        public void FillFromRow_ReadsAKnownSpell()
        {
            GHSpellInfo info = new GHSpellInfo();

            bool ok = GHSpellMenuParser.FillFromRow(info, "Force bolt ", "Deals 2-12 damage to a target",
                "&sparc; 1 &success; 100% &mana; 5.0 &cool; 0 &casts; Inf.");

            Assert.True(ok);
            Assert.Equal("Force bolt", info.Name);
            Assert.Equal("Deals 2-12 damage to a target", info.Description);
            Assert.Equal("Level 1 Arcane Spell", info.LevelText);
        }

        [Fact]
        public void FillFromRow_ReadsAForgottenSpell()
        {
            GHSpellInfo info = new GHSpellInfo();

            bool ok = GHSpellMenuParser.FillFromRow(info, "Force bolt ", "You cannot recall this spell", "");

            Assert.True(ok);
            Assert.Equal("Force bolt", info.Name);
            Assert.Equal("You cannot recall this spell", info.Description);
            Assert.True(GHSpellMenuParser.IsForgotten(info.Description));
            Assert.Null(info.LevelText);
            Assert.Equal(-1, info.Skill);
            Assert.Equal(int.MinValue, info.Level);
        }

        [Fact]
        public void FillFromRow_ReadsBracesLeftInMainTextWhenThereIsNoDescription()
        {
            GHSpellInfo info = new GHSpellInfo();

            bool ok = GHSpellMenuParser.FillFromRow(info, "Light {&spdiv; C &success; 100% &mana; 1.0 &cool; 0 &casts; Inf.}", "", "");

            Assert.True(ok);
            Assert.Equal("Light", info.Name);
            Assert.Equal("", info.Description);
            Assert.Equal(3, info.Skill);
            Assert.Equal("Major Divination Cantrip", info.LevelText);
        }

        [Fact]
        public void FillFromRow_UnreadableRowKeepsNameAndDescription()
        {
            GHSpellInfo info = new GHSpellInfo();

            bool ok = GHSpellMenuParser.FillFromRow(info, " Odd spell ", " Something ", "not stats");

            Assert.False(ok);
            Assert.Equal("Odd spell", info.Name);
            Assert.Equal("Something", info.Description);
            Assert.Null(info.LevelText);
        }

        [Fact]
        public void CreateSpellInfo_FillsTheModelFromARow()
        {
            object menuItem = new object();
            bool readable;

            GHSpellInfo info = GHSpellMenuParser.CreateSpellInfo(5017, 5000, 0, 5, "Sleep ", "Puts a monster to sleep",
                "&spenc; 3 &success; 70% &mana; 15.0 &cool; 2/6 &casts; 9", menuItem, out readable);

            Assert.True(readable);
            Assert.Equal(17, info.SpellId);
            Assert.Equal(5017, info.Glyph);
            Assert.Equal(5, info.NhColor);
            Assert.False(info.Selectable);
            Assert.Same(menuItem, info.MenuItem);
            Assert.Equal(0, info.HotbarSlot);
            Assert.Equal("Sleep", info.Name);
            Assert.Equal(7, info.Skill);
            Assert.True(info.CooldownActive);

            GHSpellInfo selectable = GHSpellMenuParser.CreateSpellInfo(5000, 5000, 1, 0, "Force bolt ", "x",
                "garbage", null, out readable);
            Assert.False(readable);
            Assert.True(selectable.Selectable);
            Assert.Equal(0, selectable.SpellId);

            GHSpellInfo below = GHSpellMenuParser.CreateSpellInfo(10, 5000, 1, 0, "X ", "", "", null, out readable);
            Assert.Equal(-1, below.SpellId);
        }

        [Fact]
        public void GetLevelText_UnknownSchoolOrLevelIsNull()
        {
            Assert.Null(GHSpellMenuParser.GetLevelText(-1, 1));
            Assert.Null(GHSpellMenuParser.GetLevelText(12, 1));
            Assert.Null(GHSpellMenuParser.GetLevelText(0, -2));
            Assert.Null(GHSpellMenuParser.GetLevelText(0, int.MinValue));
            Assert.Equal("Level 5 Nature Spell", GHSpellMenuParser.GetLevelText(10, 5));
        }
    }
}
