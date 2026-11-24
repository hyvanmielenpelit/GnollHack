using System;
using System.Collections.Generic;
using System.Diagnostics;
using GnollHackX;

#if GNH_MAUI
using Microsoft.Maui.Controls;

namespace GnollHackM
#else
using Xamarin.Forms;

namespace GnollHackX.Pages.Game
#endif
{
    /* The gamepad Spells radial. While the controller is the active input, the core's Cast menu (GHMenuStartFlags.SpellCast)
       shows as a radial of the Hotbar and the Known Spells instead of the list, and the spell command menu that Y opens
       shows as the Actions radial. Both menus are answered with the response the list sends, through
       EnqueueMenuResponse: A selects a row with Count -1 and Y with Count -2, the list's long tap. The Hotbar is the rows'
       MENU_FLAGS_HOTBAR_SLOT marks; it is edited by answering the command menu with its radial-only Set Hotbar Slot row,
       whose Count is the slot, or with its Remove from Hotbar row. A menu answered by the radial closes it, and the menu
       the core shows next opens it again where the player was. Everything runs on the UI thread. */
    public partial class GamePage
    {
        private const string SpellBrowseRadialId = "SPELLS";
        private const string SpellPickRadialId = "SPELLPICK";
        private const string SpellActionsRadialId = "SPELLACT";
        private const string SpellPlaceRadialId = "SPELLPLACE";

        /* The spell command menu's identifiers, as doaltspellmenu adds them */
        private const long SpellCommandCast = 1;
        private const long SpellCommandCancel = 8;
        private const long SpellCommandSetHotbarSlot = 9;       /* SPELLCMD_SET_HOTBAR_SLOT, radial-only; its Count is the slot */
        private const long SpellCommandRemoveFromHotbar = 10;   /* SPELLCMD_REMOVE_FROM_HOTBAR, radial-only */

        private enum SpellRadialMode
        {
            Browse,     /* the Cast menu: Hotbar, Known Spells and Spell Commands */
            Pick,       /* the Cast menu: a spell for an empty Hotbar slot */
            Actions,    /* the spell command menu */
            Place,      /* the spell command menu: a Hotbar slot for its spell */
        }

        private enum SpellRadialAction
        {
            Spell,
            EmptySlot,
            School,
            MenuRow,            /* answers with MenuItem */
            AddToHotbar,
            RemoveFromHotbar,
            PlaceSlot,
        }

        /* A radial item's payload */
        private sealed class SpellRadialEntry
        {
            public SpellRadialAction Action;
            public GHSpellInfo Spell;
            public GHMenuItem MenuItem;
            public int HotbarSlot;          /* the slot of a Hotbar page item; 0 elsewhere */
            public int TargetPage = -1;     /* a school's first page */
            public bool UpperSide = true;   /* the glyph tile's side, as the list draws the row */
        }

        /* One menu that the radial answers */
        private sealed class SpellRadialSession
        {
            public GHMenuInfo MenuInfo;
            public GHPublishedWindow Window;
            public bool IsCommandMenu;
            public bool Answered;
            public SpellRadialMode Mode;
            public List<GHSpellInfo> Spells = new List<GHSpellInfo>();
            public List<GHMenuItem> Commands = new List<GHMenuItem>();  /* the Cast menu's command rows, or the command menu's core rows */
            public HashSet<int> NewSpellIds = new HashSet<int>();
            public int KnownFirstPage;                                  /* the first Known Spells page of the Browse definition */
            public int PickSlot;
            public GHSpellInfo ActionSpell;                             /* the command menu's spell */
            public GHMenuItem SetSlotRow;
            public GHMenuItem RemoveRow;
            public GHMenuItem CancelRow;
            public GHRadialMenuDefinition BrowseDefinition;
            public GHRadialMenuDefinition ActionsDefinition;
        }

        /* Per game session */
        private WeakReference<GHGame> _spellRadialGame = null;
        private int _spellRadialHotbarPage = 0;                         /* the Hotbar page last shown */
        private GHSpellNewTracker _spellNewTracker = new GHSpellNewTracker();
        private bool _spellRadialUnreadableLogged = false;
        private List<GHSpellInfo> _spellRadialLastSpells = null;        /* the last Cast menu's spells, for its command menu */

        /* Between one answered menu and the next */
        private bool _spellExpectActions = false;                       /* Y answered the Cast menu: its command menu shows as Actions */
        private int _spellPendingPickSlot = 0;                          /* Pick answered the Cast menu: its command menu is answered with this slot */
        private string _spellReturnItemId = null;                       /* the Cast menu that comes back highlights this item */
        private bool _spellReturnToHotbar = false;                      /* ... looking in the Hotbar pages first */
        private bool _spellReturnFlash = false;                         /* ... and flashes it */

        private static bool IsGamepadSpellRadial(GHRadialMenuDefinition def)
        {
            if (def == null)
                return false;
            string id = def.MenuId;
            return id == SpellBrowseRadialId || id == SpellPickRadialId || id == SpellActionsRadialId || id == SpellPlaceRadialId;
        }

        /* The side of an item's glyph tile; true for items that are not the Spells radial's */
        private static bool GetRadialGlyphUpperSide(GHRadialItem item)
        {
            SpellRadialEntry entry = item == null ? null : item.Tag as SpellRadialEntry;
            return entry == null || entry.UpperSide;
        }

        /* Sends Add New Spells to Hotbar to the running game, which reads it from GHApp */
        public void SetSpellHotbarAutoAdd()
        {
            if (PlayingReplay)
                return;
            GHGame curGame = GHApp.CurrentGHGame;
            curGame?.ResponseQueue.Enqueue(new GHResponse(curGame, GHRequestType.SetSpellHotbarAutoAdd));
        }

        /* Called from ShowMenuCanvas: true when the radial shows or has answered the menu, so that the canvas is not shown */
        private bool TryShowMenuAsGamepadSpellRadial(GHMenuInfo menuinfo, GHPublishedWindow ghwindow)
        {
            bool expectActions = _spellExpectActions;
            int pickSlot = _spellPendingPickSlot;
            _spellExpectActions = false;
            _spellPendingPickSlot = 0;
            if (menuinfo == null || ghwindow == null || PlayingReplay || menuinfo.SelectionHow != SelectionMode.Single)
                return false;
            EnsureSpellRadialGameSession();

            if ((menuinfo.StartFlags & (ulong)GHMenuStartFlags.SpellCast) != 0)
            {
                if (!GHGamepadService.IsActive)
                {
                    ClearSpellRadialReturn();
                    return false;
                }
                return ShowGamepadSpellRadial(menuinfo, ghwindow);
            }
            if (menuinfo.Style == ghmenu_styles.GHMENU_STYLE_SPELL_COMMAND && (expectActions || pickSlot > 0))
            {
                if (pickSlot > 0 && TryAnswerGamepadSpellPick(menuinfo, ghwindow, pickSlot))
                    return true;
                if (GHGamepadService.IsActive)
                    return ShowGamepadSpellActions(menuinfo, ghwindow);
            }
            return false;
        }

        /* Called on every frame tick: once the game waits for a command again, no menu is coming back to the radial */
        private void ExpireGamepadSpellRadialMemory()
        {
            if (_spellReturnItemId == null && !_spellExpectActions && _spellPendingPickSlot == 0)
                return;
            if (IsRadialMenuOpen)
                return;
            GHGame game = GHApp.CurrentGHGame;
            if (game == null || game.KeyWaitKind == GHKeyWaitKind.PosKey)
            {
                _spellExpectActions = false;
                _spellPendingPickSlot = 0;
                ClearSpellRadialReturn();
            }
        }

        /* Called on every frame tick: the Hotbar page shown is remembered for this game session, and X goes back to it */
        private void NoteGamepadSpellRadialPage()
        {
            if (!_radialState.IsOpen)
                return;
            GHRadialMenuDefinition def = _radialState.Definition;
            if (def == null || def.MenuId != SpellBrowseRadialId)
                return;
            int page = _radialState.PageIndex;
            if (page >= 0 && page < GHSpellHotbar.PageCount)
            {
                _spellRadialHotbarPage = page;
                def.SwitchSetReturnPageIndex = page;
            }
        }

        private void EnsureSpellRadialGameSession()
        {
            GHGame game = GHApp.CurrentGHGame;
            GHGame known;
            if (_spellRadialGame != null && _spellRadialGame.TryGetTarget(out known) && ReferenceEquals(known, game))
                return;
            _spellRadialGame = new WeakReference<GHGame>(game);
            _spellRadialHotbarPage = 0;
            _spellNewTracker = new GHSpellNewTracker();
            _spellRadialUnreadableLogged = false;
            _spellRadialLastSpells = null;
            _spellExpectActions = false;
            _spellPendingPickSlot = 0;
            ClearSpellRadialReturn();
        }

        private void ClearSpellRadialReturn()
        {
            _spellReturnItemId = null;
            _spellReturnToHotbar = false;
            _spellReturnFlash = false;
        }

        private void SetSpellRadialReturn(string itemId, bool toHotbar, bool flash)
        {
            _spellReturnItemId = itemId;
            _spellReturnToHotbar = toHotbar;
            _spellReturnFlash = flash;
        }

        /* A text window or a list menu that is still fading out goes at once, since the radial cannot open over it */
        private bool PrepareGamepadSpellRadialOpen()
        {
            if (DelayedTextHideOn && TextGrid.IsVisible)
            {
#if GNH_MAUI
                StopTextHideTimers();
#endif
                DelayedTextHideCancelled = true;
                DelayedTextHideOn = false;
                CompleteTextHide(false);
            }
            if (MenuHideOn && MenuGrid.IsVisible)
            {
#if GNH_MAUI
                StopMenuHideTimers();
#endif
                MenuHideCancelled = true;
                MenuHideOn = false;
                CompleteMenuHide(false);
            }
            return CanOpenRadialMenu();
        }

        /* Opens def for s with the highlight on slot of page, which flashes with flash; false when the radial cannot open */
        private bool OpenGamepadSpellRadial(GHRadialMenuDefinition def, int page, int slot, bool flash)
        {
            def.InitialPageIndex = page;
            OpenRadialMenu(def);
            if (!_radialState.IsOpen || !ReferenceEquals(_radialState.Definition, def))
                return false;
            long now = Stopwatch.GetTimestamp();
            if (slot >= 0)
            {
                if (flash)
                    _radialState.ReplaceDefinition(def, page, slot, now, true);
                else
                    _radialState.GoToPageAndSlot(page, slot, now);
            }
            InvalidateRadialCanvas();
            return true;
        }

        /* The Cast menu */

        private bool ShowGamepadSpellRadial(GHMenuInfo menuinfo, GHPublishedWindow ghwindow)
        {
            SpellRadialSession s = new SpellRadialSession();
            s.MenuInfo = menuinfo;
            s.Window = ghwindow;
            s.Mode = SpellRadialMode.Browse;
            ReadGamepadSpellCastMenu(s, menuinfo);
            if (s.Spells.Count == 0 || !PrepareGamepadSpellRadialOpen())
            {
                ClearSpellRadialReturn();
                return false;
            }
            _spellRadialLastSpells = s.Spells;

            List<int> spellIds = new List<int>(s.Spells.Count);
            foreach (GHSpellInfo spell in s.Spells)
                spellIds.Add(spell.SpellId);
            s.NewSpellIds = _spellNewTracker.Update(spellIds);

            GHRadialMenuDefinition def = BuildGamepadSpellBrowseDefinition(s);
            s.BrowseDefinition = def;
            int page, slot;
            bool flash;
            ResolveGamepadSpellRadialReturn(s, def, out page, out slot, out flash);
            return OpenGamepadSpellRadial(def, page, slot, flash);
        }

        /* The spell rows, by their glyphs in the spell tiles, and the command rows, by their negative identifiers; the
           separator and any other row are left out */
        private void ReadGamepadSpellCastMenu(SpellRadialSession s, GHMenuInfo menuinfo)
        {
            int spellOff = GHApp.SpellTileOff;
            int spellEnd = GHApp.SkillTileOff > spellOff ? GHApp.SkillTileOff : int.MaxValue;
            foreach (GHMenuItem mi in menuinfo.MenuItems)
            {
                if (mi == null)
                    continue;
                if (mi.Identifier < 0)
                {
                    s.Commands.Add(mi);
                    continue;
                }
                if (mi.Glyph < spellOff || mi.Glyph >= spellEnd || string.IsNullOrWhiteSpace(mi.Text))
                    continue;
                bool readable;
                GHSpellInfo spell = GHSpellMenuParser.CreateSpellInfo(mi.Glyph, spellOff, mi.Identifier, mi.NHColor,
                    mi.MainText, mi.SuffixText, mi.Suffix2Text, mi, out readable);
                spell.HotbarSlot = (mi.Flags & (ulong)MenuFlags.MENU_FLAGS_HOTBAR_SLOT) != 0 ? GHSpellHotbar.NormalizeSlot(mi.ItemStyle) : 0;
                if (!readable && !_spellRadialUnreadableLogged)
                {
                    _spellRadialUnreadableLogged = true;
                    GHApp.MaybeWriteGHLog("Spells radial: a Cast menu row could not be read: " + mi.Text);
                }
                s.Spells.Add(spell);
            }
        }

        /* Hotbar 1/3 to 3/3, then Known Spells and Spell Commands */
        private GHRadialMenuDefinition BuildGamepadSpellBrowseDefinition(SpellRadialSession s)
        {
            GHRadialMenuDefinition def = CreateGamepadSpellRadialDefinition(SpellBrowseRadialId, s);
            AddGamepadSpellHotbarPages(def, s, null);
            s.KnownFirstPage = def.Pages.Count;
            AddGamepadSpellKnownPages(def, s, SpellRadialMode.Browse);
            def.SwitchSetPageIndex = s.KnownFirstPage;
            def.SwitchSetReturnPageIndex = Math.Max(0, Math.Min(GHSpellHotbar.PageCount - 1, _spellRadialHotbarPage));
            def.SwitchSetEnabled = true;
            def.ConfirmHint = "Cast";
            def.CancelHint = "Close";
            def.OnItemAction = (item, page, slot) => OnGamepadSpellRadialItemAction(s, item, page);
            return def;
        }

        /* Known Spells for an empty Hotbar slot; B goes back to the slot */
        private GHRadialMenuDefinition BuildGamepadSpellPickDefinition(SpellRadialSession s)
        {
            GHRadialMenuDefinition def = CreateGamepadSpellRadialDefinition(SpellPickRadialId, s);
            AddGamepadSpellKnownPages(def, s, SpellRadialMode.Pick);
            def.ConfirmHint = "Choose";
            def.CancelHint = "Back";
            def.CancelConsumed = true;
            def.OnCancel = () => OnGamepadSpellPickCancel(s);
            return def;
        }

        private GHRadialMenuDefinition CreateGamepadSpellRadialDefinition(string menuId, SpellRadialSession s)
        {
            GHRadialMenuDefinition def = new GHRadialMenuDefinition();
            def.MenuId = menuId;
            def.OwningStick = 0;
            def.Accent = GHRadialAccent.Gold;
            def.PageWithinGroup = true;
            def.SetTabs = new string[] { "Hotbar", "Known Spells" };
            def.SetTabIcons = new string[] { GetGamepadUIIconPath("status-quick-spell.png"), GetGamepadUIIconPath("spells.png") };
            def.SwitchSetEnabled = false;
            def.FooterMaxLines = 3;
            def.OnActivate = item => OnGamepadSpellRadialActivate(s, item);
            def.OnClosed = reason => OnGamepadSpellRadialClosed(s, reason);
            ApplyGamepadRadialGlyphs(def);
            return def;
        }

        /* Browse: each slot's spell or an empty slot. Place (carried not null): every slot takes the carried spell. */
        private void AddGamepadSpellHotbarPages(GHRadialMenuDefinition def, SpellRadialSession s, GHSpellInfo carried)
        {
            int p, d;
            for (p = 0; p < GHSpellHotbar.PageCount; p++)
            {
                GHRadialPage page = CreateGamepadSpellPage("Hotbar", p, GHSpellHotbar.PageCount, GHRadialTuning.SpellPageSlotCount, 0);
                for (d = 0; d < GHSpellHotbar.SlotsPerPage && d < page.SlotCount; d++)
                {
                    int slot = GHSpellHotbar.PageDirectionToSlot(p, d);
                    GHSpellInfo spell = GHSpellHotbar.FindBySlot(s.Spells, slot);
                    if (carried != null)
                        page.Slots[d] = CreateGamepadSpellPlaceItem(carried, spell, slot);
                    else if (spell != null)
                        page.Slots[d] = CreateGamepadSpellItem(s, spell, "hb:spell:" + spell.SpellId.ToString(), SpellRadialMode.Browse, slot);
                    else
                        page.Slots[d] = CreateGamepadSpellEmptySlotItem(slot);
                }
                def.Pages.Add(page);
            }
        }

        /* Up to SchoolRingThreshold spells: flat pages in school, level and name order. More: the school ring, whose
           schools open their own pages. Spells of an unreadable school follow on Other Spells pages. Browse adds Spell
           Commands after the flat pages or the ring. */
        private void AddGamepadSpellKnownPages(GHRadialMenuDefinition def, SpellRadialSession s, SpellRadialMode mode)
        {
            const int knownGroup = 1;
            if (!GHSpellHotbar.UsesSchoolRing(s.Spells.Count))
            {
                AddGamepadSpellListPages(def, s, mode, GHSpellHotbar.GetFlatOrder(s.Spells), "Known Spells", knownGroup, -1);
                if (mode == SpellRadialMode.Browse)
                    AddGamepadSpellCommandsPage(def, s, knownGroup);
                return;
            }

            GHRadialPage ring = new GHRadialPage("Known Spells", GHRadialTuning.SchoolRingSlotCount);
            ring.GroupIndex = knownGroup;
            int ringIndex = def.Pages.Count;
            def.Pages.Add(ring);
            if (mode == SpellRadialMode.Browse)
                AddGamepadSpellCommandsPage(def, s, knownGroup);

            List<GHSpellInfo> others = new List<GHSpellInfo>();
            foreach (GHSpellInfo spell in s.Spells)
            {
                if (spell.Skill < 0 || spell.Skill >= GHSpellMenuParser.SchoolCount)
                    others.Add(spell);
            }
            if (others.Count > 0)
                AddGamepadSpellListPages(def, s, mode, GHSpellHotbar.GetFlatOrder(others), "Other Spells", knownGroup, -1);

            int[] counts = GHSpellHotbar.CountBySchool(s.Spells);
            int group = knownGroup + 1;
            int k;
            for (k = 0; k < GHSpellMenuParser.SchoolCount && k < ring.SlotCount; k++)
            {
                int firstPage = -1;
                if (counts[k] > 0)
                {
                    firstPage = def.Pages.Count;
                    AddGamepadSpellListPages(def, s, mode, GHSpellHotbar.GetSchoolOrder(s.Spells, k), GHSpellMenuParser.GetSchoolName(k), group, ringIndex);
                    group++;
                }
                ring.Slots[k] = CreateGamepadSpellSchoolItem(k, counts[k], firstPage);
            }
        }

        private void AddGamepadSpellListPages(GHRadialMenuDefinition def, SpellRadialSession s, SpellRadialMode mode, List<GHSpellInfo> spells,
            string name, int groupIndex, int parentPage)
        {
            int perPage = GHRadialTuning.SpellPageSlotCount;
            int pageCount = Math.Max(1, (spells.Count + perPage - 1) / perPage);
            int p, d;
            for (p = 0; p < pageCount; p++)
            {
                GHRadialPage page = CreateGamepadSpellPage(name, p, pageCount, perPage, groupIndex);
                page.ParentPageIndex = parentPage;
                for (d = 0; d < perPage; d++)
                {
                    int i = p * perPage + d;
                    if (i >= spells.Count)
                        break;
                    page.Slots[d] = CreateGamepadSpellItem(s, spells[i], "known:" + spells[i].SpellId.ToString(), mode, 0);
                }
                def.Pages.Add(page);
            }
        }

        /* The Cast menu's own commands at fixed directions, in the menu's order */
        private void AddGamepadSpellCommandsPage(GHRadialMenuDefinition def, SpellRadialSession s, int groupIndex)
        {
            if (s.Commands.Count == 0)
                return;
            int slotCount = s.Commands.Count <= GHRadialTuning.SpellPageSlotCount ? GHRadialTuning.SpellPageSlotCount : GHRadialTuning.MaxSlotCount;
            GHRadialPage page = new GHRadialPage("Spell Commands", slotCount);
            page.GroupIndex = groupIndex;
            int i;
            for (i = 0; i < s.Commands.Count && i < slotCount; i++)
                page.Slots[i] = CreateGamepadSpellMenuRowItem(s.Commands[i], "cmd:" + s.Commands[i].Identifier.ToString());
            def.Pages.Add(page);
        }

        /* "Name 1/3"; a single page has no counter */
        private static GHRadialPage CreateGamepadSpellPage(string name, int index, int count, int slotCount, int groupIndex)
        {
            string counter = count > 1 ? (index + 1).ToString() + "/" + count.ToString() : null;
            GHRadialPage page = new GHRadialPage(counter == null ? name : name + " " + counter, slotCount);
            page.Name = name;
            page.Counter = counter;
            page.Group = name;
            page.GroupIndex = groupIndex;
            return page;
        }

        /* hotbarSlot is the slot of a Hotbar page item, 0 elsewhere */
        private GHRadialItem CreateGamepadSpellItem(SpellRadialSession s, GHSpellInfo spell, string id, SpellRadialMode mode, int hotbarSlot)
        {
            GHMenuItem mi = spell.MenuItem as GHMenuItem;
            SpellRadialEntry entry = new SpellRadialEntry();
            entry.Action = SpellRadialAction.Spell;
            entry.Spell = spell;
            entry.MenuItem = mi;
            entry.HotbarSlot = hotbarSlot;
            entry.UpperSide = mi == null || mi.UseUpperSide;

            GHRadialItem item = new GHRadialItem();
            item.Id = id;
            item.Label = spell.Name;
            item.Glyph = spell.Glyph;
            item.NhColor = ToGamepadSpellNhColor(spell.NhColor);
            item.Kind = GHRadialItemKind.Command;
            item.IsEnabled = spell.Selectable && mi != null;
            item.ClosesOnActivate = true;
            item.FooterText = string.IsNullOrEmpty(spell.Description) ? "" : spell.Description;
            item.Tag = entry;

            GHRadialHubRun[] levelLine = GetGamepadSpellLevelLine(spell);
            if (mode == SpellRadialMode.Pick)
            {
                /* The core leaves the rows of forgotten and cooling spells without an identifier, so they cannot be answered */
                item.ShowsUnavailableText = true;
                item.HubTopLine = new GHRadialHubRun[] { CreateHubRun(null, "For " + GHSpellHotbar.GetSlotLabel(s.PickSlot), -1) };
                List<GHRadialHubRun[]> lines = new List<GHRadialHubRun[]>(2);
                if (levelLine != null)
                    lines.Add(levelLine);
                if (GHSpellHotbar.IsOnHotbar(spell))
                    lines.Add(new GHRadialHubRun[] { CreateHubRun(null, "moves from " + GHSpellHotbar.GetSlotLabel(spell.HotbarSlot), -1) });
                item.HubLines = lines;
                if (GHSpellHotbar.IsOnHotbar(spell))
                    item.Badge = GHRadialBadge.Star;
                return item;
            }

            item.ShowsUnavailableText = false;
            item.ItemActionHint = item.IsEnabled ? "Actions" : null;
            item.HubTopLine = GetGamepadSpellCooldownLine(spell);
            GHRadialHubRun[] statsLine = GetGamepadSpellStatsLine(spell);
            if (levelLine != null || statsLine != null)
            {
                List<GHRadialHubRun[]> lines = new List<GHRadialHubRun[]>(2);
                if (levelLine != null)
                    lines.Add(levelLine);
                if (statsLine != null)
                    lines.Add(statsLine);
                item.HubLines = lines;
            }
            if (s.NewSpellIds.Contains(spell.SpellId))
                item.Badge = GHRadialBadge.New;
            else if (hotbarSlot == 0 && GHSpellHotbar.IsOnHotbar(spell))
                item.Badge = GHRadialBadge.Star;
            return item;
        }

        private static GHRadialItem CreateGamepadSpellEmptySlotItem(int slot)
        {
            SpellRadialEntry entry = new SpellRadialEntry();
            entry.Action = SpellRadialAction.EmptySlot;
            entry.HotbarSlot = slot;

            GHRadialItem item = new GHRadialItem();
            item.Id = "hb:empty:" + slot.ToString();
            item.Kind = GHRadialItemKind.Placeholder;
            item.HubTitle = "Add Spell";
            item.HubLines = new List<GHRadialHubRun[]> { new GHRadialHubRun[] { CreateHubRun(null, GHSpellHotbar.GetSlotLabel(slot), -1) } };
            item.ItemActionHint = "Add Spell";
            item.ClosesOnActivate = false;
            item.FooterText = "Press A to choose a spell for this slot.";
            item.Tag = entry;
            return item;
        }

        private static GHRadialItem CreateGamepadSpellSchoolItem(int school, int count, int firstPage)
        {
            SpellRadialEntry entry = new SpellRadialEntry();
            entry.Action = SpellRadialAction.School;
            entry.TargetPage = firstPage;

            string name = GHSpellMenuParser.GetSchoolName(school);
            GHRadialItem item = new GHRadialItem();
            item.Id = "school:" + school.ToString();
            item.Label = name;
            item.IconPath = GetGamepadUIIconPath("symbol-spell-" + name.ToLowerInvariant() + ".png");
            item.Kind = GHRadialItemKind.Command;
            item.IsEnabled = count > 0 && firstPage >= 0;
            item.ShowsUnavailableText = false;
            item.ClosesOnActivate = false;
            item.Badge = count > 0 ? GHRadialBadge.Count : GHRadialBadge.None;
            item.BadgeText = count.ToString();
            string countText = count == 0 ? "No spells known" : count == 1 ? "1 spell" : count.ToString() + " spells";
            item.HubLines = new List<GHRadialHubRun[]> { new GHRadialHubRun[] { CreateHubRun(null, countText, -1) } };
            item.FooterText = "";
            item.Tag = entry;
            return item;
        }

        /* A menu row that the radial answers as it is: the Cast menu's commands and the command menu's core rows */
        private static GHRadialItem CreateGamepadSpellMenuRowItem(GHMenuItem mi, string id)
        {
            SpellRadialEntry entry = new SpellRadialEntry();
            entry.Action = SpellRadialAction.MenuRow;
            entry.MenuItem = mi;
            entry.UpperSide = mi.UseUpperSide;

            GHRadialItem item = new GHRadialItem();
            item.Id = id;
            item.Label = string.IsNullOrWhiteSpace(mi.TrimmedMainText) ? mi.Text.Trim() : mi.TrimmedMainText;
            item.Glyph = mi.IsGlyphVisible ? mi.Glyph : 0;
            item.NhColor = ToGamepadSpellNhColor(mi.NHColor);
            item.Kind = GHRadialItemKind.Command;
            item.ClosesOnActivate = true;
            item.Tag = entry;
            return item;
        }

        /* The cooldown row, magenta while the spell cools down; null for a spell without a cooldown */
        private static GHRadialHubRun[] GetGamepadSpellCooldownLine(GHSpellInfo spell)
        {
            if (!GHSpellMenuParser.HasCooldown(spell))
                return null;
            return new GHRadialHubRun[] { CreateHubRun("&cool;", spell.CooldownText, spell.CooldownActive ? (int)NhColor.CLR_MAGENTA : -1) };
        }

        /* The school symbol and "Level 1 Arcane"; null when the row could not be read */
        private static GHRadialHubRun[] GetGamepadSpellLevelLine(GHSpellInfo spell)
        {
            if (string.IsNullOrEmpty(spell.LevelText))
                return null;
            string symbol = spell.Skill >= 0 && spell.Skill < GHSpellMenuParser.SchoolSymbols.Length ? GHSpellMenuParser.SchoolSymbols[spell.Skill] : null;
            return new GHRadialHubRun[] { CreateHubRun(symbol, spell.LevelText, -1) };
        }

        /* Success, mana and casts; null when the row could not be read */
        private static GHRadialHubRun[] GetGamepadSpellStatsLine(GHSpellInfo spell)
        {
            if (spell.SuccessText == null || spell.ManaText == null || spell.CastsText == null)
                return null;
            return new GHRadialHubRun[]
            {
                CreateHubRun("&success;", spell.SuccessText, -1),
                CreateHubRun("&mana;", spell.ManaText, -1),
                CreateHubRun("&casts;", spell.CastsText, -1),
            };
        }

        private static GHRadialHubRun CreateHubRun(string symbol, string text, int nhColor)
        {
            GHRadialHubRun run = new GHRadialHubRun();
            run.Symbol = symbol;
            run.Text = text;
            run.NhColor = nhColor;
            return run;
        }

        /* NO_COLOR and anything out of range draw in the radial's default color */
        private static int ToGamepadSpellNhColor(int nhColor)
        {
            return nhColor < 0 || nhColor == (int)NhColor.NO_COLOR || nhColor >= (int)NhColor.CLR_MAX ? -1 : nhColor;
        }

        /* The page and slot of the item that the last answer remembered, searched first in its set; else the Hotbar page
           last shown, with no highlight */
        private void ResolveGamepadSpellRadialReturn(SpellRadialSession s, GHRadialMenuDefinition def, out int page, out int slot, out bool flash)
        {
            string id = _spellReturnItemId;
            bool toHotbar = _spellReturnToHotbar;
            bool wantFlash = _spellReturnFlash;
            ClearSpellRadialReturn();
            page = Math.Max(0, Math.Min(GHSpellHotbar.PageCount - 1, _spellRadialHotbarPage));
            slot = -1;
            flash = false;
            if (id == null)
                return;
            int first = toHotbar ? 0 : s.KnownFirstPage;
            int end = toHotbar ? s.KnownFirstPage : def.PageCount;
            int pass, p;
            for (pass = 0; pass < 2; pass++)
            {
                for (p = 0; p < def.PageCount; p++)
                {
                    bool inSet = p >= first && p < end;
                    if (inSet != (pass == 0))
                        continue;
                    int k = def.Pages[p].IndexOf(id);
                    if (k >= 0)
                    {
                        page = p;
                        slot = k;
                        flash = wantFlash;
                        return;
                    }
                }
            }
        }

        /* Answers */

        /* The selected row, as the list's OK sends it */
        private void AnswerGamepadSpellMenu(SpellRadialSession s, GHMenuItem mi, int count)
        {
            if (s.Answered || mi == null)
                return;
            s.Answered = true;
            mi.Count = count;
            List<GHMenuItem> resultlist = new List<GHMenuItem>(1);
            resultlist.Add(mi);
            EnqueueMenuResponse(s.Window, resultlist, false, 0);
        }

        /* A cancel, as the list's Cancel sends it; the command menu takes its Cancel row, which returns to the Cast menu */
        private void CancelGamepadSpellMenu(SpellRadialSession s)
        {
            if (s.Answered)
                return;
            if (s.IsCommandMenu && s.CancelRow != null)
            {
                AnswerGamepadSpellMenu(s, s.CancelRow, -1);
                return;
            }
            s.Answered = true;
            if (!s.IsCommandMenu)
            {
                _spellNewTracker.Acknowledge();
                ClearSpellRadialReturn();
            }
            EnqueueMenuResponse(s.Window, new List<GHMenuItem>(1), true, 0);
        }

        /* Cancel answers the menu; GameEnded leaves it; any other reason shows the same menu as the list, which the core
           still waits for */
        private void OnGamepadSpellRadialClosed(SpellRadialSession s, GHRadialCloseReason reason)
        {
            OnGamepadRadialClosed(reason);
            if (s.Answered || reason == GHRadialCloseReason.GameEnded)
                return;
            if (reason == GHRadialCloseReason.Cancelled)
            {
                CancelGamepadSpellMenu(s);
                return;
            }
            s.Answered = true;
            _spellExpectActions = false;
            _spellPendingPickSlot = 0;
            ClearSpellRadialReturn();
            if (!CanvasViewsShutDown && IsGameOn)
                ShowGamepadSpellMenuAsList(s.MenuInfo, s.Window);
        }

        private async void ShowGamepadSpellMenuAsList(GHMenuInfo menuinfo, GHPublishedWindow ghwindow)
        {
            try
            {
                await ShowMenuCanvas(menuinfo, ghwindow, false);
            }
            catch (Exception ex)
            {
                GHApp.ReportCaughtException(ex);
            }
        }

        private void OnGamepadSpellRadialActivate(SpellRadialSession s, GHRadialItem item)
        {
            SpellRadialEntry entry = item == null ? null : item.Tag as SpellRadialEntry;
            if (entry == null || s.Answered)
                return;
            switch (entry.Action)
            {
            case SpellRadialAction.Spell:
                if (s.Mode == SpellRadialMode.Pick)
                {
                    /* The long tap opens the command menu, which TryAnswerGamepadSpellPick answers with the slot */
                    _spellPendingPickSlot = s.PickSlot;
                    SetSpellRadialReturn("hb:spell:" + entry.Spell.SpellId.ToString(), true, true);
                    AnswerGamepadSpellMenu(s, entry.MenuItem, -2);
                }
                else
                {
                    _spellNewTracker.Acknowledge();
                    ClearSpellRadialReturn();
                    AnswerGamepadSpellMenu(s, entry.MenuItem, -1);
                }
                break;
            case SpellRadialAction.EmptySlot:
                EnterGamepadSpellPick(s, entry.HotbarSlot);
                break;
            case SpellRadialAction.School:
                if (entry.TargetPage >= 0)
                    _radialState.GoToPageAndSlot(entry.TargetPage, -1, Stopwatch.GetTimestamp());
                break;
            case SpellRadialAction.MenuRow:
                /* A command menu row keeps the return to the spell that Y remembered; a command that ends at the map
                   lets ExpireGamepadSpellRadialMemory drop it */
                if (!s.IsCommandMenu)
                    SetSpellRadialReturn(item.Id, false, false);
                AnswerGamepadSpellMenu(s, entry.MenuItem, -1);
                break;
            case SpellRadialAction.AddToHotbar:
                EnterGamepadSpellPlace(s);
                break;
            case SpellRadialAction.RemoveFromHotbar:
                RemoveGamepadSpellFromHotbar(s);
                break;
            case SpellRadialAction.PlaceSlot:
                if (s.SetSlotRow != null && s.ActionSpell != null)
                {
                    SetSpellRadialReturn("hb:spell:" + s.ActionSpell.SpellId.ToString(), true, true);
                    AnswerGamepadSpellMenu(s, s.SetSlotRow, entry.HotbarSlot);
                }
                break;
            default:
                break;
            }
        }

        /* Browse: Y on a spell is the list's long tap, Y on an empty Hotbar slot chooses a spell for it */
        private void OnGamepadSpellRadialItemAction(SpellRadialSession s, GHRadialItem item, int page)
        {
            SpellRadialEntry entry = item == null ? null : item.Tag as SpellRadialEntry;
            if (entry == null || s.Answered || s.Mode != SpellRadialMode.Browse)
                return;
            if (entry.Action == SpellRadialAction.EmptySlot)
            {
                EnterGamepadSpellPick(s, entry.HotbarSlot);
                return;
            }
            if (entry.Action != SpellRadialAction.Spell || !item.IsEnabled)
                return;
            _spellExpectActions = true;
            SetSpellRadialReturn(item.Id, page < s.KnownFirstPage, false);
            AnswerGamepadSpellMenu(s, entry.MenuItem, -2);
            DismissRadialMenu(GHRadialCloseReason.Confirmed);
        }

        private void EnterGamepadSpellPick(SpellRadialSession s, int slot)
        {
            if (GHSpellHotbar.NormalizeSlot(slot) == 0)
                return;
            s.Mode = SpellRadialMode.Pick;
            s.PickSlot = slot;
            _radialState.ReplaceDefinition(BuildGamepadSpellPickDefinition(s), 0, -1, Stopwatch.GetTimestamp(), false);
        }

        /* B on a school's page goes back to the ring; elsewhere back to the Hotbar slot */
        private bool OnGamepadSpellPickCancel(SpellRadialSession s)
        {
            GHRadialPage page = _radialState.CurrentPage;
            if (page != null && page.ParentPageIndex >= 0)
                return false;
            int slot = s.PickSlot;
            s.Mode = SpellRadialMode.Browse;
            s.PickSlot = 0;
            if (s.BrowseDefinition == null)
                return false;
            _radialState.ReplaceDefinition(s.BrowseDefinition, GHSpellHotbar.SlotToPage(slot), GHSpellHotbar.SlotToDirection(slot),
                Stopwatch.GetTimestamp(), false);
            return true;
        }

        /* A Pick's command menu: answered at once with Set Hotbar Slot; false when the menu has no such row */
        private bool TryAnswerGamepadSpellPick(GHMenuInfo menuinfo, GHPublishedWindow ghwindow, int slot)
        {
            GHMenuItem setRow = FindGamepadSpellRadialOnlyRow(menuinfo, SpellCommandSetHotbarSlot);
            if (setRow == null)
                return false;
            setRow.Count = slot;
            List<GHMenuItem> resultlist = new List<GHMenuItem>(1);
            resultlist.Add(setRow);
            EnqueueMenuResponse(ghwindow, resultlist, false, 0);
            return true;
        }

        private static GHMenuItem FindGamepadSpellRadialOnlyRow(GHMenuInfo menuinfo, long identifier)
        {
            foreach (GHMenuItem mi in menuinfo.RadialOnlyItems)
            {
                if (mi != null && mi.Identifier == identifier)
                    return mi;
            }
            return null;
        }

        /* The spell command menu */

        private bool ShowGamepadSpellActions(GHMenuInfo menuinfo, GHPublishedWindow ghwindow)
        {
            SpellRadialSession s = new SpellRadialSession();
            s.MenuInfo = menuinfo;
            s.Window = ghwindow;
            s.IsCommandMenu = true;
            s.Mode = SpellRadialMode.Actions;
            if (_spellRadialLastSpells != null)
                s.Spells = _spellRadialLastSpells;

            int glyph = ghwindow.Glyph;
            int spellId = glyph >= GHApp.SpellTileOff ? glyph - GHApp.SpellTileOff : -1;
            s.ActionSpell = GHSpellHotbar.FindBySpellId(s.Spells, spellId);
            if (s.ActionSpell == null)
            {
                s.ActionSpell = new GHSpellInfo();
                s.ActionSpell.SpellId = spellId;
                s.ActionSpell.Glyph = glyph;
                s.ActionSpell.Name = GetGamepadSpellNameFromHeader(menuinfo.Header);
                s.ActionSpell.Description = "";
                s.ActionSpell.NhColor = (int)NhColor.NO_COLOR;
            }

            foreach (GHMenuItem mi in menuinfo.MenuItems)
            {
                if (mi == null || mi.Identifier == 0 || mi.Identifier == SpellCommandCast)
                    continue;
                if (mi.Identifier == SpellCommandCancel)
                {
                    s.CancelRow = mi;
                    continue;
                }
                s.Commands.Add(mi);
            }
            s.SetSlotRow = FindGamepadSpellRadialOnlyRow(menuinfo, SpellCommandSetHotbarSlot);
            s.RemoveRow = FindGamepadSpellRadialOnlyRow(menuinfo, SpellCommandRemoveFromHotbar);

            if (!PrepareGamepadSpellRadialOpen())
                return false;
            GHRadialMenuDefinition def = BuildGamepadSpellActionsDefinition(s, ghwindow);
            s.ActionsDefinition = def;
            return OpenGamepadSpellRadial(def, 0, -1, false);
        }

        /* "What do you want to do with 'Force bolt'?" gives "Force bolt" */
        private static string GetGamepadSpellNameFromHeader(string header)
        {
            if (string.IsNullOrEmpty(header))
                return "";
            int open = header.IndexOf('\'');
            int close = open >= 0 ? header.LastIndexOf('\'') : -1;
            if (open >= 0 && close > open + 1)
                return header.Substring(open + 1, close - open - 1);
            return header;
        }

        /* The core's commands clockwise from North, then Add to or Move on Hotbar and Remove from Hotbar in the last two
           slots; the hub shows the spell */
        private GHRadialMenuDefinition BuildGamepadSpellActionsDefinition(SpellRadialSession s, GHPublishedWindow ghwindow)
        {
            GHSpellInfo spell = s.ActionSpell;
            bool onHotbar = GHSpellHotbar.IsOnHotbar(spell);
            int slotCount = s.Commands.Count + 2 <= GHRadialTuning.SpellActionsSlotCount ? GHRadialTuning.SpellActionsSlotCount : GHRadialTuning.MaxSlotCount;
            GHRadialPage page = new GHRadialPage("Actions", slotCount);
            int i;
            for (i = 0; i < s.Commands.Count && i < slotCount - 2; i++)
                page.Slots[i] = CreateGamepadSpellMenuRowItem(s.Commands[i], "act:" + s.Commands[i].Identifier.ToString());

            SpellRadialEntry addEntry = new SpellRadialEntry();
            addEntry.Action = SpellRadialAction.AddToHotbar;
            addEntry.UpperSide = s.SetSlotRow == null || s.SetSlotRow.UseUpperSide;
            GHRadialItem add = new GHRadialItem();
            add.Id = "act:hotbar";
            add.Label = onHotbar ? "Move on Hotbar" : "Add to Hotbar";
            add.Glyph = s.SetSlotRow != null && s.SetSlotRow.IsGlyphVisible ? s.SetSlotRow.Glyph : 0;
            add.IconPath = GetGamepadUIIconPath("status-quick-spell.png");
            add.Kind = GHRadialItemKind.Command;
            add.IsEnabled = s.SetSlotRow != null;
            add.ClosesOnActivate = false;
            add.Tag = addEntry;
            page.Slots[slotCount - 2] = add;

            SpellRadialEntry removeEntry = new SpellRadialEntry();
            removeEntry.Action = SpellRadialAction.RemoveFromHotbar;
            GHRadialItem remove = new GHRadialItem();
            remove.Id = "act:unhotbar";
            remove.Label = "Remove from Hotbar";
            remove.IconPath = GetGamepadUIIconPath("remove.png");
            remove.Kind = GHRadialItemKind.Command;
            remove.IsEnabled = s.RemoveRow != null && onHotbar;
            remove.ClosesOnActivate = true;
            remove.Tag = removeEntry;
            page.Slots[slotCount - 1] = remove;

            SpellRadialEntry hubEntry = new SpellRadialEntry();
            hubEntry.Action = SpellRadialAction.Spell;
            hubEntry.Spell = spell;
            hubEntry.UpperSide = ghwindow.UseUpperSide;
            GHRadialItem hub = new GHRadialItem();
            hub.Id = "act:spell";
            hub.Label = spell.Name;
            hub.Glyph = ghwindow.Glyph;
            hub.NhColor = ToGamepadSpellNhColor(spell.NhColor);
            hub.FooterText = string.IsNullOrEmpty(spell.Description) ? "" : spell.Description;
            hub.Tag = hubEntry;

            GHRadialMenuDefinition def = CreateGamepadSpellRadialDefinition(SpellActionsRadialId, s);
            def.Pages.Add(page);
            def.FixedHubItem = hub;
            def.ConfirmHint = "Select";
            def.CancelHint = "Back";
            return def;
        }

        /* Place mode: the Hotbar, on the spell's slot, else the first empty one, else Hotbar 1 North */
        private void EnterGamepadSpellPlace(SpellRadialSession s)
        {
            GHSpellInfo spell = s.ActionSpell;
            if (spell == null || s.SetSlotRow == null)
                return;
            int target = GHSpellHotbar.GetSlot(spell);
            if (target == 0)
                target = GHSpellHotbar.FirstEmptySlot(s.Spells);
            if (target == 0)
                target = 1;
            s.Mode = SpellRadialMode.Place;
            _radialState.ReplaceDefinition(BuildGamepadSpellPlaceDefinition(s), GHSpellHotbar.SlotToPage(target), GHSpellHotbar.SlotToDirection(target),
                Stopwatch.GetTimestamp(), false);
        }

        private GHRadialMenuDefinition BuildGamepadSpellPlaceDefinition(SpellRadialSession s)
        {
            GHSpellInfo spell = s.ActionSpell;
            bool onHotbar = GHSpellHotbar.IsOnHotbar(spell);
            GHRadialMenuDefinition def = CreateGamepadSpellRadialDefinition(SpellPlaceRadialId, s);
            AddGamepadSpellHotbarPages(def, s, spell);

            SpellRadialEntry carriedEntry = new SpellRadialEntry();
            carriedEntry.Action = SpellRadialAction.Spell;
            carriedEntry.Spell = spell;
            carriedEntry.UpperSide = s.Window == null || s.Window.UseUpperSide;
            GHRadialItem carried = new GHRadialItem();
            carried.Id = "place:carried";
            carried.Label = spell.Name;
            carried.Glyph = spell.Glyph;
            carried.Tag = carriedEntry;
            def.CarriedItem = carried;

            def.ConfirmHint = onHotbar ? "Move Here" : "Place Here";
            def.CancelHint = "Back";
            def.CancelConsumed = true;
            def.OnCancel = () => OnGamepadSpellPlaceCancel(s);
            if (onHotbar && s.RemoveRow != null)
                def.OnItemAction = (item, page, slot) => RemoveGamepadSpellFromHotbar(s);
            return def;
        }

        /* A Hotbar slot in Place mode: the slot's spell, or an empty slot, takes the carried spell */
        private GHRadialItem CreateGamepadSpellPlaceItem(GHSpellInfo carried, GHSpellInfo occupant, int slot)
        {
            bool carriedOnHotbar = GHSpellHotbar.IsOnHotbar(carried);
            SpellRadialEntry entry = new SpellRadialEntry();
            entry.Action = SpellRadialAction.PlaceSlot;
            entry.Spell = occupant;
            entry.HotbarSlot = slot;
            GHMenuItem mi = occupant == null ? null : occupant.MenuItem as GHMenuItem;
            entry.UpperSide = mi == null || mi.UseUpperSide;

            GHRadialItem item = new GHRadialItem();
            item.Id = "place:" + slot.ToString();
            item.Kind = occupant == null ? GHRadialItemKind.Placeholder : GHRadialItemKind.Command;
            item.Label = occupant == null ? null : occupant.Name;
            item.Glyph = occupant == null ? 0 : occupant.Glyph;
            item.ShowsSwapMark = occupant != null && occupant.SpellId != carried.SpellId;
            item.ClosesOnActivate = true;
            item.HubTopLine = new GHRadialHubRun[] { CreateHubRun(null, carriedOnHotbar ? "Move" : "Place", -1) };
            item.HubTitle = carried.Name ?? "";
            List<GHRadialHubRun[]> lines = new List<GHRadialHubRun[]>(2);
            lines.Add(new GHRadialHubRun[] { CreateHubRun(null, GHSpellHotbar.GetSlotLabel(slot), -1) });
            string swapLine = GHSpellHotbar.GetPlaceHubLine(carried, occupant);
            if (swapLine != null)
                lines.Add(new GHRadialHubRun[] { CreateHubRun(null, swapLine, -1) });
            item.HubLines = lines;
            item.ItemActionHint = carriedOnHotbar ? "Remove" : null;
            item.FooterText = string.IsNullOrEmpty(carried.Description) ? "" : carried.Description;
            item.Tag = entry;
            return item;
        }

        /* B in Place mode goes back to Actions on Add to or Move on Hotbar, without answering */
        private bool OnGamepadSpellPlaceCancel(SpellRadialSession s)
        {
            GHRadialMenuDefinition actions = s.ActionsDefinition;
            if (actions == null)
                return false;
            s.Mode = SpellRadialMode.Actions;
            GHRadialPage page = actions.GetPage(0);
            int slot = page == null ? -1 : page.IndexOf("act:hotbar");
            _radialState.ReplaceDefinition(actions, 0, slot, Stopwatch.GetTimestamp(), false);
            return true;
        }

        /* Answers the command menu with Remove from Hotbar; the Cast menu that comes back flashes the emptied slot */
        private void RemoveGamepadSpellFromHotbar(SpellRadialSession s)
        {
            GHSpellInfo spell = s.ActionSpell;
            int slot = GHSpellHotbar.GetSlot(spell);
            if (s.Answered || s.RemoveRow == null || slot == 0)
                return;
            SetSpellRadialReturn("hb:empty:" + slot.ToString(), true, true);
            AnswerGamepadSpellMenu(s, s.RemoveRow, -1);
            if (_radialState.IsOpen && IsGamepadSpellRadial(_radialState.Definition))
                DismissRadialMenu(GHRadialCloseReason.Confirmed);
        }
    }
}
