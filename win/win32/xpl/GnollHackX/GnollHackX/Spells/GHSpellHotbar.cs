using System;
using System.Collections.Generic;
using System.Globalization;

namespace GnollHackX
{
    /* One spell row of the core's Cast menu as the Spells radial sees it */
    public sealed class GHSpellInfo
    {
        public int SpellId;              /* Glyph - spell tile offset */
        public int Glyph;
        public int Skill = -1;           /* index 0..11 into SchoolNames; -1 unknown */
        public int Level = int.MinValue; /* -1 minor cantrip, 0 major cantrip, 1..; MinValue unknown */
        public string Name, Description, LevelText, SuccessText, ManaText, CooldownText, CastsText;
        public bool CooldownActive;      /* CooldownText is "n/m" */
        public int NhColor;
        public bool Selectable;          /* a tap can select the row: Identifier != 0 and no MENU_FLAGS_LONG_TAP_ONLY */
        public int HotbarSlot;           /* 0 when not on the hotbar */
        public object MenuItem;          /* the GHMenuItem; opaque to the model */
    }

    /* Reads a Cast menu row as GHMenuItem splits it. The core writes a known spell as
       "Name (description) {SCHOOL LVL &success; N% &mana; X.X &cool; [left/]len &casts; C}", or with
       the words Success, Mana, Cool and Casts and the school mnemonics (Arc, Cle, ...) when the
       window port has no special symbols; LVL is c (minor cantrip), C (major cantrip) or the level,
       and C is a number or "Inf.". GHMenuItem gives the part before '(' as MainText, the text
       inside the parentheses as SuffixText and the text inside the braces as Suffix2Text. A
       forgotten spell is "Name (You cannot recall this spell)", with no braces. A spell without a
       description has no parentheses, so its whole row, braces included, is MainText. */
    public static class GHSpellMenuParser
    {
        public const string ForgottenText = "You cannot recall this spell";

        /* In P_ARCANE_SPELL..P_NECROMANCY_SPELL order */
        public static readonly string[] SchoolSymbols =
        {
            "&sparc;", "&spcle;", "&sphea;", "&spdiv;", "&spabj;", "&spmov;",
            "&sptra;", "&spenc;", "&spcon;", "&spcel;", "&spnat;", "&spnec;"
        };

        /* The school tokens of a menu built without special symbols, in the same order */
        public static readonly string[] SchoolMnemonics =
        {
            "Arc", "Cle", "Hea", "Div", "Abj", "Mov", "Tra", "Enc", "Con", "Cel", "Nat", "Nec"
        };

        public static readonly string[] SchoolNames =
        {
            "Arcane", "Clerical", "Healing", "Divination", "Abjuration", "Movement",
            "Transmutation", "Enchantment", "Conjuration", "Celestial", "Nature", "Necromancy"
        };

        public static int SchoolCount { get { return SchoolNames.Length; } }

        /* The school index of a symbol or mnemonic token; -1 when it is neither */
        public static int GetSchoolIndex(string token)
        {
            if (string.IsNullOrEmpty(token))
                return -1;
            int i;
            for (i = 0; i < SchoolSymbols.Length; i++)
            {
                if (string.Equals(token, SchoolSymbols[i], StringComparison.Ordinal)
                    || string.Equals(token, SchoolMnemonics[i], StringComparison.Ordinal))
                    return i;
            }
            return -1;
        }

        /* "" for a school out of range */
        public static string GetSchoolName(int skill)
        {
            if (skill < 0 || skill >= SchoolNames.Length)
                return "";
            return SchoolNames[skill];
        }

        /* "Level 1 Arcane Spell", "Minor Arcane Cantrip", "Major Arcane Cantrip"; null for an unknown
           school or level */
        public static string GetLevelText(int skill, int level)
        {
            if (skill < 0 || skill >= SchoolNames.Length)
                return null;
            string school = SchoolNames[skill];
            if (level == -1)
                return "Minor " + school + " Cantrip";
            if (level == 0)
                return "Major " + school + " Cantrip";
            if (level > 0)
                return "Level " + level.ToString(CultureInfo.InvariantCulture) + " " + school + " Spell";
            return null;
        }

        public static bool IsForgotten(string suffixText)
        {
            return suffixText != null && string.Equals(suffixText.Trim(), ForgottenText, StringComparison.Ordinal);
        }

        /* Fills Skill, Level, LevelText, SuccessText ("100%"), ManaText ("5.0"), CooldownText ("12"
           or "3/12"), CooldownActive and CastsText ("5" or "Inf.") from a Suffix2Text. On an
           unreadable string it returns false and leaves the stats null, Skill -1 and Level
           MinValue. */
        public static bool TryParseStats(string suffix2Text, GHSpellInfo info)
        {
            if (info == null)
                return false;
            ClearStats(info);
            if (string.IsNullOrWhiteSpace(suffix2Text))
                return false;

            string[] tokens = suffix2Text.Split(new char[] { ' ', '\t' }, StringSplitOptions.RemoveEmptyEntries);
            if (tokens.Length != 10)
                return false;

            int skill = GetSchoolIndex(tokens[0]);
            if (skill < 0)
                return false;
            int level;
            if (!TryParseLevel(tokens[1], out level))
                return false;

            string success = null, mana = null, cooldown = null, casts = null;
            int i;
            for (i = 2; i + 1 < tokens.Length; i += 2)
            {
                string key = tokens[i];
                string value = tokens[i + 1];
                if (key == "&success;" || key == "Success")
                {
                    if (success != null || !IsSuccessValue(value))
                        return false;
                    success = value;
                }
                else if (key == "&mana;" || key == "Mana")
                {
                    if (mana != null || !IsManaValue(value))
                        return false;
                    mana = value;
                }
                else if (key == "&cool;" || key == "Cool")
                {
                    if (cooldown != null || !IsCooldownValue(value))
                        return false;
                    cooldown = value;
                }
                else if (key == "&casts;" || key == "Casts")
                {
                    if (casts != null || !IsCastsValue(value))
                        return false;
                    casts = value;
                }
                else
                    return false;
            }
            if (success == null || mana == null || cooldown == null || casts == null)
                return false;

            info.Skill = skill;
            info.Level = level;
            info.LevelText = GetLevelText(skill, level);
            info.SuccessText = success;
            info.ManaText = mana;
            info.CooldownText = cooldown;
            info.CooldownActive = cooldown.IndexOf('/') >= 0;
            info.CastsText = casts;
            return true;
        }

        /* Fills Name, Description and the stats from a row's MainText, SuffixText and Suffix2Text.
           Returns false when the row is neither readable nor a forgotten spell's; the name and
           description are filled either way. */
        public static bool FillFromRow(GHSpellInfo info, string mainText, string suffixText, string suffix2Text)
        {
            if (info == null)
                return false;
            string main = mainText ?? "";
            string stats = suffix2Text;
            if (string.IsNullOrWhiteSpace(stats))
            {
                /* No description: the braces stayed in MainText */
                int open = main.IndexOf('{');
                if (open >= 0)
                {
                    int close = main.IndexOf('}', open + 1);
                    stats = close > open ? main.Substring(open + 1, close - open - 1) : "";
                    main = main.Substring(0, open);
                }
            }
            info.Name = main.Trim();
            info.Description = suffixText == null ? "" : suffixText.Trim();
            if (TryParseStats(stats, info))
                return true;
            return IsForgotten(suffixText);
        }

        /* A spell row's model; readable is false when the row is neither readable nor a forgotten
           spell's. A glyph below spellTileOff gives SpellId -1. */
        public static GHSpellInfo CreateSpellInfo(int glyph, int spellTileOff, long identifier, int nhColor,
            string mainText, string suffixText, string suffix2Text, object menuItem, out bool readable)
        {
            GHSpellInfo info = new GHSpellInfo();
            info.Glyph = glyph;
            info.SpellId = glyph >= spellTileOff ? glyph - spellTileOff : -1;
            info.NhColor = nhColor;
            info.Selectable = identifier != 0;
            info.MenuItem = menuItem;
            readable = FillFromRow(info, mainText, suffixText, suffix2Text);
            return info;
        }

        /* Whether the hub shows the cooldown row: a cooldown counting down, or a length above 0 */
        public static bool HasCooldown(GHSpellInfo info)
        {
            if (info == null || string.IsNullOrEmpty(info.CooldownText))
                return false;
            if (info.CooldownActive)
                return true;
            int len;
            return int.TryParse(info.CooldownText, NumberStyles.None, CultureInfo.InvariantCulture, out len) && len > 0;
        }

        private static void ClearStats(GHSpellInfo info)
        {
            info.Skill = -1;
            info.Level = int.MinValue;
            info.LevelText = null;
            info.SuccessText = null;
            info.ManaText = null;
            info.CooldownText = null;
            info.CooldownActive = false;
            info.CastsText = null;
        }

        private static bool TryParseLevel(string token, out int level)
        {
            level = int.MinValue;
            if (token == "c")
            {
                level = -1;
                return true;
            }
            if (token == "C")
            {
                level = 0;
                return true;
            }
            int n;
            if (IsDigits(token) && int.TryParse(token, NumberStyles.None, CultureInfo.InvariantCulture, out n) && n > 0)
            {
                level = n;
                return true;
            }
            return false;
        }

        private static bool IsDigits(string s)
        {
            if (string.IsNullOrEmpty(s))
                return false;
            int i;
            for (i = 0; i < s.Length; i++)
            {
                if (s[i] < '0' || s[i] > '9')
                    return false;
            }
            return true;
        }

        /* "100%", "-5%" */
        private static bool IsSuccessValue(string s)
        {
            if (s == null || s.Length < 2 || s[s.Length - 1] != '%')
                return false;
            string number = s.Substring(0, s.Length - 1);
            if (number[0] == '-')
                number = number.Substring(1);
            return IsDigits(number);
        }

        /* "5.0" */
        private static bool IsManaValue(string s)
        {
            if (string.IsNullOrEmpty(s))
                return false;
            int dot = s.IndexOf('.');
            if (dot < 0)
                return IsDigits(s);
            return IsDigits(s.Substring(0, dot)) && IsDigits(s.Substring(dot + 1));
        }

        /* "12" or "3/12" */
        private static bool IsCooldownValue(string s)
        {
            if (string.IsNullOrEmpty(s))
                return false;
            int slash = s.IndexOf('/');
            if (slash < 0)
                return IsDigits(s);
            return IsDigits(s.Substring(0, slash)) && IsDigits(s.Substring(slash + 1));
        }

        /* "5" or "Inf." */
        private static bool IsCastsValue(string s)
        {
            return s == "Inf." || IsDigits(s);
        }
    }

    /* The Spell Hotbar's rules: slots, pages and directions, the slot assignment shared with the
       core's set_spell_hotbar_slot, the Known Spells orders, and the hub
       texts of Place mode. Slots are 1..MaxSlots; 0 means not on the Hotbar. Pages and directions
       are 0-based, the direction clockwise from North. */
    public static class GHSpellHotbar
    {
        public const int MaxSlots = 24;
        public const int SlotsPerPage = 8;
        public const int PageCount = MaxSlots / SlotsPerPage;
        /* Known Spells uses the school ring above this many known spells */
        public const int SchoolRingThreshold = 16;

        private const char MiddleDot = (char)0x00B7;

        private static readonly string[] DirectionNames = { "North", "Northeast", "East", "Southeast", "South", "Southwest", "West", "Northwest" };

        /* A stored slot outside 1..MaxSlots reads as not on the Hotbar */
        public static int NormalizeSlot(int slot)
        {
            return slot >= 1 && slot <= MaxSlots ? slot : 0;
        }

        public static int GetSlot(GHSpellInfo spell)
        {
            return spell == null ? 0 : NormalizeSlot(spell.HotbarSlot);
        }

        public static bool IsOnHotbar(GHSpellInfo spell)
        {
            return GetSlot(spell) > 0;
        }

        /* -1 for a slot outside 1..MaxSlots */
        public static int SlotToPage(int slot)
        {
            if (NormalizeSlot(slot) == 0)
                return -1;
            return (slot - 1) / SlotsPerPage;
        }

        /* -1 for a slot outside 1..MaxSlots */
        public static int SlotToDirection(int slot)
        {
            if (NormalizeSlot(slot) == 0)
                return -1;
            return (slot - 1) % SlotsPerPage;
        }

        /* 0 for a page or direction out of range */
        public static int PageDirectionToSlot(int page, int dir)
        {
            if (page < 0 || page >= PageCount || dir < 0 || dir >= SlotsPerPage)
                return 0;
            return page * SlotsPerPage + dir + 1;
        }

        /* "" for a direction out of range */
        public static string GetDirectionName(int dir)
        {
            if (dir < 0 || dir >= DirectionNames.Length)
                return "";
            return DirectionNames[dir];
        }

        /* The page number and the direction's letter, A for North clockwise to H for Northwest: "1A" to "3H";
           "" for a slot outside 1..MaxSlots */
        public static string GetSlotCode(int slot)
        {
            if (NormalizeSlot(slot) == 0)
                return "";
            return (SlotToPage(slot) + 1).ToString() + (char)('A' + SlotToDirection(slot));
        }

        /* "Favorites 1C · East"; "" for a slot outside 1..MaxSlots */
        public static string GetSlotLabel(int slot)
        {
            if (NormalizeSlot(slot) == 0)
                return "";
            return "Favorites " + GetSlotCode(slot) + " " + MiddleDot + " " + GetDirectionName(SlotToDirection(slot));
        }

        public static bool UsesSchoolRing(int knownCount)
        {
            return knownCount > SchoolRingThreshold;
        }

        public static GHSpellInfo FindBySpellId(IList<GHSpellInfo> spells, int spellId)
        {
            if (spells == null)
                return null;
            int i;
            for (i = 0; i < spells.Count; i++)
            {
                GHSpellInfo spell = spells[i];
                if (spell != null && spell.SpellId == spellId)
                    return spell;
            }
            return null;
        }

        /* null for an empty slot or a slot outside 1..MaxSlots */
        public static GHSpellInfo FindBySlot(IList<GHSpellInfo> spells, int slot)
        {
            if (spells == null || NormalizeSlot(slot) == 0)
                return null;
            int i;
            for (i = 0; i < spells.Count; i++)
            {
                GHSpellInfo spell = spells[i];
                if (spell != null && GetSlot(spell) == slot)
                    return spell;
            }
            return null;
        }

        /* The lowest slot no spell holds; 0 when the Hotbar is full */
        public static int FirstEmptySlot(IList<GHSpellInfo> spells)
        {
            bool[] held = new bool[MaxSlots + 1];
            if (spells != null)
            {
                int i;
                for (i = 0; i < spells.Count; i++)
                    held[GetSlot(spells[i])] = true;
            }
            int slot;
            for (slot = 1; slot <= MaxSlots; slot++)
            {
                if (!held[slot])
                    return slot;
            }
            return 0;
        }

        /* The slot rule of set_spell_hotbar_slot. Slot 0 takes the spell off the Hotbar. Otherwise
           any other spell holding the slot gets the moved spell's old slot (a swap, or 0 when the
           moved spell had none), and the spell gets the slot. A slot outside 0..MaxSlots or an
           unknown spell id changes nothing. */
        public static void ApplySetSlot(IList<GHSpellInfo> spells, int spellId, int slot)
        {
            if (spells == null || slot < 0 || slot > MaxSlots)
                return;
            GHSpellInfo spell = FindBySpellId(spells, spellId);
            if (spell == null)
                return;
            if (slot == 0)
            {
                spell.HotbarSlot = 0;
                return;
            }
            int old = GetSlot(spell);
            int i;
            for (i = 0; i < spells.Count; i++)
            {
                GHSpellInfo other = spells[i];
                if (other != null && other != spell && GetSlot(other) == slot)
                    other.HotbarSlot = old;
            }
            spell.HotbarSlot = slot;
        }

        /* The Place mode line for a target slot: null when the target is empty or holds the
           carried spell, "swaps with X" when the carried spell is on the Hotbar, else "replaces X" */
        public static string GetPlaceHubLine(GHSpellInfo carried, GHSpellInfo occupant)
        {
            if (occupant == null || carried == null || occupant == carried || occupant.SpellId == carried.SpellId)
                return null;
            string name = occupant.Name ?? "";
            return IsOnHotbar(carried) ? "swaps with " + name : "replaces " + name;
        }

        /* Known spells per school, index 0..SchoolCount - 1; spells of an unknown school are not counted */
        public static int[] CountBySchool(IEnumerable<GHSpellInfo> spells)
        {
            int[] counts = new int[GHSpellMenuParser.SchoolCount];
            if (spells != null)
            {
                foreach (GHSpellInfo spell in spells)
                {
                    if (spell != null && spell.Skill >= 0 && spell.Skill < counts.Length)
                        counts[spell.Skill]++;
                }
            }
            return counts;
        }

        /* Flat Known Spells pages: school, then level (cantrips, -1 and 0, first), then name; an
           unknown school or level comes after the known ones */
        public static int CompareForFlatPages(GHSpellInfo a, GHSpellInfo b)
        {
            int c = SchoolSortKey(a.Skill).CompareTo(SchoolSortKey(b.Skill));
            if (c != 0)
                return c;
            return CompareForSchoolPages(a, b);
        }

        /* A school's pages: level (cantrips, -1 and 0, first), then name, then spell id */
        public static int CompareForSchoolPages(GHSpellInfo a, GHSpellInfo b)
        {
            int c = LevelSortKey(a.Level).CompareTo(LevelSortKey(b.Level));
            if (c != 0)
                return c;
            c = string.Compare(a.Name ?? "", b.Name ?? "", StringComparison.OrdinalIgnoreCase);
            if (c != 0)
                return c;
            return a.SpellId.CompareTo(b.SpellId);
        }

        /* The spells in the flat pages' order; null entries are left out */
        public static List<GHSpellInfo> GetFlatOrder(IEnumerable<GHSpellInfo> spells)
        {
            List<GHSpellInfo> list = new List<GHSpellInfo>();
            if (spells != null)
            {
                foreach (GHSpellInfo spell in spells)
                {
                    if (spell != null)
                        list.Add(spell);
                }
            }
            list.Sort(CompareForFlatPages);
            return list;
        }

        /* The spells of one school (skill) in that school's pages' order */
        public static List<GHSpellInfo> GetSchoolOrder(IEnumerable<GHSpellInfo> spells, int skill)
        {
            List<GHSpellInfo> list = new List<GHSpellInfo>();
            if (spells != null)
            {
                foreach (GHSpellInfo spell in spells)
                {
                    if (spell != null && spell.Skill == skill)
                        list.Add(spell);
                }
            }
            list.Sort(CompareForSchoolPages);
            return list;
        }

        private static int SchoolSortKey(int skill)
        {
            return skill >= 0 && skill < GHSpellMenuParser.SchoolCount ? skill : int.MaxValue;
        }

        private static long LevelSortKey(int level)
        {
            return level == int.MinValue ? long.MaxValue : level;
        }
    }

    /* The NEW badge: the spells learned since the last acknowledged Cast menu. The first
       baseline comes from the first update; spells stay marked across updates until Acknowledge,
       which the host calls when the Spells radial closes. Keyed by spell id; session memory only. */
    public sealed class GHSpellNewTracker
    {
        private readonly HashSet<int> _known = new HashSet<int>();
        private readonly HashSet<int> _marked = new HashSet<int>();
        private bool _hasBaseline;

        public bool HasBaseline { get { return _hasBaseline; } }

        /* Starts over: these spells are known and none is new */
        public void SetBaseline(IEnumerable<int> spellIds)
        {
            _known.Clear();
            _marked.Clear();
            if (spellIds != null)
            {
                foreach (int spellId in spellIds)
                    _known.Add(spellId);
            }
            _hasBaseline = true;
        }

        /* Returns the spells marked new, including those marked by earlier updates and not yet
           acknowledged. A spell no longer known loses its mark. Without a baseline the update
           becomes the baseline and nothing is new. */
        public HashSet<int> Update(IEnumerable<int> spellIds)
        {
            if (!_hasBaseline)
            {
                SetBaseline(spellIds);
                return new HashSet<int>();
            }
            HashSet<int> current = new HashSet<int>();
            if (spellIds != null)
            {
                foreach (int spellId in spellIds)
                    current.Add(spellId);
            }
            foreach (int spellId in current)
            {
                if (!_known.Contains(spellId))
                    _marked.Add(spellId);
            }
            _marked.IntersectWith(current);
            _known.Clear();
            _known.UnionWith(current);
            return new HashSet<int>(_marked);
        }

        public bool IsNew(int spellId)
        {
            return _marked.Contains(spellId);
        }

        public void Acknowledge()
        {
            _marked.Clear();
        }
    }
}
