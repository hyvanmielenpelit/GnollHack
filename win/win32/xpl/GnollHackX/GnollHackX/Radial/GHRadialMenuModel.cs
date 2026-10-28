using System;
using System.Collections.Generic;

namespace GnollHackX
{
    public enum GHRadialItemKind
    {
        Command,
        Toggle
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
        JumpSouth
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

        public bool HasPreferredAngle { get { return PreferredAngle >= 0; } }

        public bool IsToggle { get { return Kind == GHRadialItemKind.Toggle; } }

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
        public string Title;
        public string Group;
        public int GroupIndex;
        public string EmptyText;        /* the hub text when no slot is filled */
        public readonly GHRadialItem[] Slots;

        public GHRadialPage(string title, int slotCount)
        {
            if (slotCount < 1)
                slotCount = 1;
            Title = title;
            Group = title;
            Slots = new GHRadialItem[slotCount];
        }

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

    /* A whole radial menu as the host builds it for one opening. OnActivate and OnClosed run on
       the thread that calls GHRadialMenuState, which is the UI thread. */
    public sealed class GHRadialMenuDefinition
    {
        public string MenuId;                           /* "L3" / "R3" */
        public List<GHRadialPage> Pages = new List<GHRadialPage>();
        public int OwningStick;                         /* 0 = left, 1 = right */
        public int SwitchSetPageIndex = -1;             /* SwitchSet jumps here, and back to page 0 from here on; -1 = unused */
        public Action<GHRadialItem> OnActivate;
        public Action<GHRadialCloseReason> OnClosed;

        /* Hub texts */
        public string NoHighlightStickText = "Tilt the stick to choose";
        public string NoHighlightPointerText = "Point to choose";
        public string UnavailableText = "Unavailable";

        /* Hint bar labels; a null label hides that hint */
        public string ConfirmHint = "Select";           /* "Toggle" for L3 */
        public string CancelHint = "Close";
        public string SecondaryHint;                    /* "More commands" */
        public string SecondaryHintOnSwitchedSet;       /* "Context", shown from SwitchSetPageIndex on */
        public string GroupHint;                        /* "Group" */

        /* Button names, which the host sets from the controller style */
        public string ConfirmGlyph = "A";
        public string CancelGlyph = "B";
        public string SecondaryGlyph = "X";
        public string PrevPageGlyph = "L1";
        public string NextPageGlyph = "R1";
        public string GroupGlyph = "LT/RT";

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
    }
}
