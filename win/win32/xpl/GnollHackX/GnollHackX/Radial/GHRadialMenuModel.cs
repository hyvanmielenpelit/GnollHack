using System;
using System.Collections.Generic;

namespace GnollHackX
{
    public enum GHRadialItemKind
    {
        Command,
        Toggle,
        Placeholder         /* an empty slot that can still be chosen: a dashed outline, a plus and "Empty" */
    }

    /* A small mark at the slice's outer edge */
    public enum GHRadialBadge
    {
        None,
        Star,               /* a gold star */
        New,                /* a green "NEW" pill */
        Count               /* a dark pill with BadgeText */
    }

    public enum GHRadialCloseReason
    {
        Confirmed,
        Cancelled,
        Superseded,
        PageHidden,
        Deactivated,
        GameEnded
    }

    /* What the host translates controller buttons and keys into */
    public enum GHRadialInput
    {
        Confirm,
        Cancel,
        OpenerPressed,
        PrevPage,
        NextPage,
        SwitchSet,
        PrevGroup,
        NextGroup,
        StepClockwise,
        StepCounterClockwise,
        JumpNorth,
        JumpSouth,
        OpenerReleased,
        ItemAction          /* the definition's ItemActionButton: OnItemAction on the highlighted slot */
    }

    /* One piece of a hub line: an optional special symbol image, then its text */
    public sealed class GHRadialHubRun
    {
        public string Symbol;                   /* a special symbol name, "&mana;"; null for none */
        public string Text;
        public int NhColor = -1;                /* -1 = the line's default color */
    }

    /* A More Commands cell or another command source, as plain data for the slot assigner.
       Column and Row are the cell in the 4x6 More Commands matrix of Page. */
    public sealed class GHRadialSourceItem
    {
        public string Id;
        public string Label;
        public string Description;
        public string IconPath;
        public int Command;
        public int Page;
        public int Column;
        public int Row;
        public string ShortcutText;     /* optional; shown in the hub when keyboard shortcuts are on */
    }

    public sealed class GHRadialItem
    {
        public const double NoPreferredAngle = -1;

        public string Id;               /* stable key: "ctx:44:Pick Up", "left:skills", "more:1:2:0", "l3:autodig" */
        public string Label;            /* short slice label */
        public string Description;      /* hub text */
        public string IconPath;         /* the icon, and a toggle's icon while off */
        public string IconPathOn;       /* a toggle's icon while on; IconPath when null */
        public string ShortcutText;
        public GHRadialItemKind Kind;
        public bool IsEnabled = true;
        public Func<bool> IsOnGetter;   /* toggles: read again on every paint */
        public bool ClosesOnActivate = true;
        public bool PlaysOwnSound;      /* the host plays no confirm sound after OnActivate */
        public double PreferredAngle = NoPreferredAngle;  /* fixed compass direction, or negative for none */
        public object Tag;              /* the host's payload */
        public int Glyph;                           /* 0 = none; drawn by the host's CustomIconDrawer */
        public int NhColor = -1;                    /* label, tile tint and hub title color; -1 = default */
        public GHRadialBadge Badge;
        public string BadgeText;                    /* for Count */
        public string ItemActionHint;               /* the Y label; null hides it */
        public bool ShowsSwapMark;                  /* dimmed with a swap mark while CarriedItem is over it */
        public bool ShowsUnavailableText = true;    /* false: a disabled item's hub omits UnavailableText */
        public GHRadialHubRun[] HubTopLine;         /* above the title; null for none */
        public List<GHRadialHubRun[]> HubLines;     /* replace Description in the hub when not null */
        public string FooterText;                   /* the description band; null for none */
        public GHRadialHubRun[] FooterTitle;        /* the band's first row, above FooterText; null for none */
        public string HubTitle;                     /* the hub's title; null for Label */
        public int SliceTintNhColor = -1;           /* tints the slice's fill and border; -1 = none */
        public bool IconGrayed;                     /* the icon is drawn grayed, also when the item is enabled */

        public bool HasPreferredAngle { get { return PreferredAngle >= 0; } }

        public bool IsToggle { get { return Kind == GHRadialItemKind.Toggle; } }

        public bool IsPlaceholder { get { return Kind == GHRadialItemKind.Placeholder; } }

        public bool IsOn
        {
            get
            {
                Func<bool> getter = IsOnGetter;
                return getter != null && getter();
            }
        }

        public string GetIconPath(bool isOn)
        {
            return isOn && !string.IsNullOrEmpty(IconPathOn) ? IconPathOn : IconPath;
        }
    }

    /* One ring of a radial menu. The slot count is fixed for the page, so angles never move
       while it is shown; a null slot is empty. Pages with the same GroupIndex in a row form a
       group, which PrevGroup and NextGroup skip over. */
    public sealed class GHRadialPage
    {
        public string Title;            /* the full title, counter included: "Commands 3/7" */
        public string Name;             /* the title without its counter: "Commands" */
        public string Counter;          /* the page's place in its run: "3/7"; null when it has none */
        public string Group;
        public int GroupIndex;
        public string EmptyText;        /* the hub text when no slot is filled */
        public int ParentPageIndex = -1;    /* Cancel goes back to this page; -1 for none */
        public readonly GHRadialItem[] Slots;

        public GHRadialPage(string title, int slotCount)
        {
            if (slotCount < 1)
                slotCount = 1;
            Title = title;
            Name = title;
            Group = title;
            Slots = new GHRadialItem[slotCount];
        }

        public bool HasCounter { get { return !string.IsNullOrEmpty(Counter); } }

        /* The name the hub shows below the counter, or the whole title when there is no counter */
        public string DisplayName { get { return HasCounter ? (Name ?? "") : Title; } }

        public int SlotCount { get { return Slots.Length; } }

        public int FilledCount
        {
            get
            {
                int n = 0;
                int i;
                for (i = 0; i < Slots.Length; i++)
                {
                    if (Slots[i] != null)
                        n++;
                }
                return n;
            }
        }

        public GHRadialItem GetItem(int slot)
        {
            return slot >= 0 && slot < Slots.Length ? Slots[slot] : null;
        }

        /* The slot holding the item with this Id, or -1 */
        public int IndexOf(string id)
        {
            if (id == null)
                return -1;
            int i;
            for (i = 0; i < Slots.Length; i++)
            {
                if (Slots[i] != null && string.Equals(Slots[i].Id, id, StringComparison.Ordinal))
                    return i;
            }
            return -1;
        }
    }

    public enum GHRadialAccent
    {
        Gold,
        Violet,
    }

    /* A whole radial menu as the host builds it for one opening. OnActivate, OnItemAction, OnCancel
       and OnClosed run on the thread that calls GHRadialMenuState, which is the UI thread. */
    public sealed class GHRadialMenuDefinition
    {
        public string MenuId;                           /* "L3" / "R3" */
        public List<GHRadialPage> Pages = new List<GHRadialPage>();
        public int OwningStick;                         /* 0 = left, 1 = right */
        public int SwitchSetPageIndex = -1;             /* SwitchSet jumps here, and back to page 0 from here on; -1 = unused */
        public int InitialPageIndex;                    /* the page Open shows first, clamped to the pages */
        public Action<GHRadialItem> OnActivate;
        public Action<GHRadialCloseReason> OnClosed;

        /* Hub texts */
        public string NoHighlightStickText = "Tilt the stick to choose";
        public string NoHighlightPointerText = "Point to choose";
        public string UnavailableText = "Unavailable";
        public string PromptVerb;                       /* the command asking for a direction; null for a plain hub */
        public string PromptObject;                     /* what it uses, shown under the verb; null for none */

        /* Hint bar labels; a null label hides that hint */
        public string ConfirmHint = "Select";           /* "Toggle" for L3 */
        public string CancelHint = "Close";
        public string SecondaryHint;                    /* "More commands" */
        public string SecondaryHintOnSwitchedSet;       /* "Context", shown from SwitchSetPageIndex on */
        public string GroupHint;                        /* "Group" */

        /* The hint and chip buttons, drawn as badges in Style, which the host sets from the controller */
        public GHGamepadStyle Style = GHGamepadStyle.SteamDeck;
        public GHGamepadButton ConfirmButton = GHGamepadButton.A;
        public GHGamepadButton CancelButton = GHGamepadButton.B;
        public GHGamepadButton SecondaryButton = GHGamepadButton.X;
        public GHGamepadButton PrevPageButton = GHGamepadButton.LB;
        public GHGamepadButton NextPageButton = GHGamepadButton.RB;
        public GHGamepadButton GroupButton = GHGamepadButton.LT;      /* shown as a pair with GroupButton2 */
        public GHGamepadButton GroupButton2 = GHGamepadButton.RT;

        /* The highlight colors: Gold for radials that act, Violet for the direction radial */
        public GHRadialAccent Accent = GHRadialAccent.Gold;

        public bool ReleaseConfirms;                    /* the held ConfirmButton opened it: its release confirms the highlight, or cancels without one */
        public bool StickArmedAtOpen;                   /* a stick already pushed selects at once */
        public bool SeeThrough;                         /* lighter backdrop, slices and hub, to keep the map readable */
        public GHGamepadHint[] ExtraHints;              /* more hint bar entries after the cancel hint */

        public GHGamepadButton OpenerButton = GHGamepadButton.Count;   /* Count = derive as today */
        public bool PageWithinGroup;                    /* PrevPage, NextPage, the chips and the dots stay within the current group's run */
        public string[] SetTabs;                        /* 2 labels; null for none */
        public string[] SetTabIcons;
        public bool SwitchSetEnabled = true;
        public int SwitchSetReturnPageIndex = -1;       /* SwitchSet from the switched set goes here; -1 = page 0 */
        public GHGamepadButton ItemActionButton = GHGamepadButton.Y;
        public Action<GHRadialItem, int, int> OnItemAction;   /* item (null on an empty slot), page, slot */
        public Func<bool> OnCancel;                     /* true: the host consumed B */
        public bool CancelConsumed;                     /* the host sets it with each ReplaceDefinition; the hint reads BackHint */
        public string BackHint = "Back";
        public GHRadialItem CarriedItem;                /* drawn as a ghost over the highlighted slot */
        public GHRadialItem FixedHubItem;               /* hub shows its Glyph and Label, whatever is highlighted */
        public int FooterMaxLines = 3;                  /* 0 = no description band */
        public int LabelMaxLines = 1;                   /* slice label lines; the Spells radials set 2 */
        public int HubTitleMaxLines = 1;                /* hub title lines; the Spells radials set 2 */
        public double HubLineMinScale = 0.7;            /* a hub line shrinks to fit down to this; the Spells radials set lower */
        public double HubLinesGapOfFont;                /* extra space before the first hub line, of the hub line font; 0 = none */

        public int PageCount { get { return Pages == null ? 0 : Pages.Count; } }

        public GHRadialPage GetPage(int index)
        {
            return Pages != null && index >= 0 && index < Pages.Count ? Pages[index] : null;
        }

        /* True when the pages form more than one group */
        public bool HasGroups
        {
            get
            {
                int i;
                for (i = 1; i < PageCount; i++)
                {
                    if (Pages[i] != null && Pages[0] != null && Pages[i].GroupIndex != Pages[0].GroupIndex)
                        return true;
                }
                return false;
            }
        }

        public bool IsSwitchedSetPage(int pageIndex)
        {
            return SwitchSetPageIndex > 0 && pageIndex >= SwitchSetPageIndex;
        }

        /* The set of pages that pageIndex belongs to: the pages before SwitchSetPageIndex, or the
           pages from it on; every page when SwitchSetPageIndex is unused. The page dots count
           within this set. */
        public void GetSetRange(int pageIndex, out int first, out int count)
        {
            int total = PageCount;
            if (SwitchSetPageIndex > 0 && SwitchSetPageIndex < total)
            {
                if (pageIndex >= SwitchSetPageIndex)
                {
                    first = SwitchSetPageIndex;
                    count = total - SwitchSetPageIndex;
                }
                else
                {
                    first = 0;
                    count = SwitchSetPageIndex;
                }
                return;
            }
            first = 0;
            count = total;
        }

        /* The run of pages in a row that share pageIndex's GroupIndex */
        public void GetGroupRange(int pageIndex, out int first, out int count)
        {
            GHRadialPage page = GetPage(pageIndex);
            if (page == null)
            {
                first = 0;
                count = PageCount;
                return;
            }
            int start = pageIndex;
            while (start > 0 && Pages[start - 1] != null && Pages[start - 1].GroupIndex == page.GroupIndex)
                start--;
            int end = pageIndex;
            while (end < PageCount - 1 && Pages[end + 1] != null && Pages[end + 1].GroupIndex == page.GroupIndex)
                end++;
            first = start;
            count = end - start + 1;
        }

        /* The pages that PrevPage, NextPage and the chips move among: the group's run with
           PageWithinGroup, else every page */
        public void GetPagingRange(int pageIndex, out int first, out int count)
        {
            if (PageWithinGroup)
            {
                GetGroupRange(pageIndex, out first, out count);
                return;
            }
            first = 0;
            count = PageCount;
        }

        /* The pages the page dots count: the group's run with PageWithinGroup, else the set */
        public void GetDotRange(int pageIndex, out int first, out int count)
        {
            if (PageWithinGroup)
                GetGroupRange(pageIndex, out first, out count);
            else
                GetSetRange(pageIndex, out first, out count);
        }

        /* True when some item has a FooterText, so that the description band is reserved */
        public bool HasFooters
        {
            get
            {
                if (FooterMaxLines <= 0)
                    return false;
                if (FixedHubItem != null && FixedHubItem.FooterText != null)
                    return true;
                int i;
                for (i = 0; i < PageCount; i++)
                {
                    GHRadialPage page = Pages[i];
                    if (page == null)
                        continue;
                    int k;
                    for (k = 0; k < page.SlotCount; k++)
                    {
                        GHRadialItem item = page.Slots[k];
                        if (item != null && item.FooterText != null)
                            return true;
                    }
                }
                return false;
            }
        }

        /* True when the header shows the two set tabs */
        public bool HasSetTabs { get { return SetTabs != null && SetTabs.Length >= 2; } }
    }
}
