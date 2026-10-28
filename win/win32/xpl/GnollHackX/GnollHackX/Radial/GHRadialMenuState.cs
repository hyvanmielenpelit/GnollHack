using System;
using System.Diagnostics;

namespace GnollHackX
{
    public enum GHRadialPhase
    {
        Closed,
        Opening,
        Open,
        Closing
    }

    [Flags]
    public enum GHRadialEffects
    {
        None = 0,
        HighlightChanged = 1,
        PageChanged = 2,
        Activated = 4,
        Closed = 8,
        Refused = 16        /* a confirm on nothing, on an empty slot or on a disabled item */
    }

    /* What an input did, so that the host can play the matching sound */
    public struct GHRadialApplyResult
    {
        public GHRadialEffects Effects;
        public GHRadialItem Item;               /* the activated or refused item */
        public GHRadialCloseReason CloseReason; /* valid with Closed */

        public bool Has(GHRadialEffects effect)
        {
            return (Effects & effect) != 0;
        }
    }

    /* The state of one radial menu overlay: phase, current page, per-page highlight memory,
       stick selection and the animation timeline. Time is Stopwatch ticks passed in by the
       caller, so animations take the same time at any frame rate.
       - IsOpen turns false the moment the menu is confirmed or cancelled; IsVisible stays true
         until the closing animation ends.
       - OnActivate runs before OnClosed, after the menu has logically closed.
       - Tick advances the animation and sets NeedsFrame; at rest NeedsFrame is false.
       Knows nothing of controller buttons: the host translates them into GHRadialInput. Free of
       UI types, so that the unit tests compile it. Single-threaded: call it from the UI thread. */
    public sealed class GHRadialMenuState
    {
        private readonly double _ticksPerSecond;
        private readonly GHRadialSelector _selector = new GHRadialSelector();
        private readonly GHRadialGeometry _geometry = new GHRadialGeometry();
        private float[] _weights = new float[GHRadialTuning.MaxSlotCount];
        private int[] _pageHighlights = new int[0];

        private GHRadialMenuDefinition _def;
        private GHRadialPhase _phase;
        private int _pageIndex;
        private long _phaseStart;
        private float _closeFromOpacity;
        private float _closeFromScale = 1;
        private long _lastTick;

        private long _pageChangeStart;
        private int _previousPage = -1;
        private int _pageDirection;

        private long _flashStart;
        private int _flashSlot = -1;
        private int _flashPage = -1;

        private int _wedgeSlot = -1;
        private bool _wedgeTweening;
        private double _wedgeFrom;
        private double _wedgeTo;
        private long _wedgeStart;
        private float _wedgeAngle;
        private float _wedgeOpacity;

        private bool _dirty;
        private bool _needsFrame;
        private bool _pointerMode;
        private float _stickX;
        private float _stickY;
        private int _toggleMask;
        private bool _hasHighlightSound;
        private long _lastHighlightSound;

        private float _opacity;
        private float _ringScale = 1;
        private float _backdrop;
        private float _pageTransition = 1;
        private float _flash;

        public GHRadialMenuState() : this(Stopwatch.Frequency)
        {
        }

        public GHRadialMenuState(long ticksPerSecond)
        {
            _ticksPerSecond = ticksPerSecond > 0 ? ticksPerSecond : Stopwatch.Frequency;
        }

        /* Layout for pointer hit tests; the renderer updates it on every paint */
        public GHRadialGeometry Geometry { get { return _geometry; } }

        public GHRadialMenuDefinition Definition { get { return _def; } }
        public string MenuId { get { return _def == null ? null : _def.MenuId; } }
        public GHRadialPhase Phase { get { return _phase; } }
        public bool IsOpen { get { return _phase == GHRadialPhase.Opening || _phase == GHRadialPhase.Open; } }
        public bool IsVisible { get { return _phase != GHRadialPhase.Closed; } }
        public bool NeedsFrame { get { return _needsFrame; } }
        public int PageIndex { get { return _pageIndex; } }
        public int PageCount { get { return _def == null ? 0 : _def.PageCount; } }
        public GHRadialPage CurrentPage { get { return _def == null ? null : _def.GetPage(_pageIndex); } }
        public int HighlightedSlot { get { return _selector.HighlightedSlot; } }

        public GHRadialItem HighlightedItem
        {
            get
            {
                GHRadialPage page = CurrentPage;
                return page == null ? null : page.GetItem(_selector.HighlightedSlot);
            }
        }

        public bool IsMenuOpen(string menuId)
        {
            return IsOpen && _def != null && string.Equals(_def.MenuId, menuId, StringComparison.Ordinal);
        }

        /* True after mouse or touch input, until the stick is pushed past the center threshold */
        public bool IsPointerMode { get { return _pointerMode; } }
        public float StickX { get { return _stickX; } }
        public float StickY { get { return _stickY; } }
        public float StickMagnitude { get { return (float)_selector.LastMagnitude; } }
        public bool IsStickArmed { get { return _selector.IsArmed; } }

        public bool ShowStickGhost
        {
            get { return IsOpen && !_pointerMode && _selector.LastMagnitude >= GHRadialTuning.StickGhostMinMagnitude; }
        }

        /* Timeline values as of the last Tick or input */
        public float Opacity { get { return _opacity; } }
        public float RingScale { get { return _ringScale; } }
        public float BackdropOpacity { get { return _backdrop; } }
        public float PageTransition { get { return _pageTransition; } }       /* 1 when no page change is in progress */
        public int PreviousPageIndex { get { return _pageTransition < 1 ? _previousPage : -1; } }
        public int PageDirection { get { return _pageDirection; } }           /* +1 forward, -1 back */
        public float FlashIntensity { get { return _flash; } }
        public int FlashSlot { get { return _flash > 0 && _flashPage == _pageIndex ? _flashSlot : -1; } }
        public float WedgeAngle { get { return _wedgeAngle; } }
        public float WedgeOpacity { get { return _wedgeOpacity; } }

        public bool IsAnimating
        {
            get
            {
                return _phase == GHRadialPhase.Opening || _phase == GHRadialPhase.Closing || _pageTransition < 1 || _flash > 0;
            }
        }

        /* 0..1 highlight weight of a slot on the current page, eased linearly over
           HighlightEaseDuration; the renderer applies the curve */
        public float GetHighlightWeight(int slot)
        {
            return slot >= 0 && slot < _weights.Length ? _weights[slot] : 0;
        }

        /* The highlighted slot a page had when it was left, or -1 */
        public int GetPageHighlight(int pageIndex)
        {
            if (pageIndex == _pageIndex)
                return _selector.HighlightedSlot;
            return pageIndex >= 0 && pageIndex < _pageHighlights.Length ? _pageHighlights[pageIndex] : -1;
        }

        /* Requests one more frame, for example after a resize */
        public void MarkDirty()
        {
            _dirty = true;
        }

        /* Opens the menu on its first page with no highlight. Reopening during the closing
           animation continues from the current visibility. Replacing a menu that is still open
           calls no OnClosed: the host closes it first. */
        public bool Open(GHRadialMenuDefinition def, long nowTicks)
        {
            if (def == null || def.PageCount == 0)
                return false;
            int maxSlots = 0;
            int i;
            for (i = 0; i < def.PageCount; i++)
            {
                if (def.Pages[i] == null)
                    return false;
                maxSlots = Math.Max(maxSlots, def.Pages[i].SlotCount);
            }

            float startOpacity = 0;
            if (_phase != GHRadialPhase.Closed)
            {
                Evaluate(nowTicks);
                startOpacity = _opacity;
            }

            _def = def;
            _pageIndex = 0;
            _pageHighlights = new int[def.PageCount];
            for (i = 0; i < _pageHighlights.Length; i++)
                _pageHighlights[i] = -1;
            if (_weights.Length < maxSlots)
                _weights = new float[maxSlots];
            else
                Array.Clear(_weights, 0, _weights.Length);

            _selector.Reset(true);
            _previousPage = -1;
            _pageDirection = 0;
            _pageTransition = 1;
            _flashSlot = -1;
            _flashPage = -1;
            _flash = 0;
            _pointerMode = false;
            _stickX = 0;
            _stickY = 0;
            _wedgeSlot = -1;
            _wedgeTweening = false;
            _wedgeOpacity = 0;

            _phase = GHRadialPhase.Opening;
            _phaseStart = nowTicks - ToTicks(startOpacity * GHRadialTuning.OpenDuration);
            _lastTick = nowTicks;
            _toggleMask = ComputeToggleMask();
            _dirty = true;
            Evaluate(nowTicks);
            return true;
        }

        /* Closes with the closing animation and calls OnClosed. False when not open. */
        public bool Close(GHRadialCloseReason reason, long nowTicks)
        {
            return Close(reason, nowTicks, false);
        }

        /* immediate skips the closing animation, for example when the game ends */
        public bool Close(GHRadialCloseReason reason, long nowTicks, bool immediate)
        {
            if (!IsOpen)
            {
                if (immediate && _phase == GHRadialPhase.Closing)
                    FinishClose();
                return false;
            }
            GHRadialMenuDefinition def = _def;
            BeginClose(nowTicks);
            if (immediate)
                FinishClose();
            InvokeClosed(def, reason);
            return true;
        }

        /* Hides at once without callbacks, for shutdown */
        public void Reset()
        {
            _phase = GHRadialPhase.Closed;
            _opacity = 0;
            _backdrop = 0;
            _ringScale = 1;
            _pageTransition = 1;
            _flash = 0;
            _needsFrame = false;
            _dirty = false;
        }

        public GHRadialApplyResult Apply(GHRadialInput input, long nowTicks)
        {
            GHRadialApplyResult res = new GHRadialApplyResult();
            if (!IsOpen)
                return res;
            GHRadialPage page = CurrentPage;
            switch (input)
            {
            case GHRadialInput.Confirm:
                return Confirm(nowTicks);
            case GHRadialInput.Cancel:
            case GHRadialInput.OpenerPressed:
                Close(GHRadialCloseReason.Cancelled, nowTicks);
                res.Effects = GHRadialEffects.Closed;
                res.CloseReason = GHRadialCloseReason.Cancelled;
                return res;
            case GHRadialInput.PrevPage:
                return GoToPage(_pageIndex - 1, nowTicks);
            case GHRadialInput.NextPage:
                return GoToPage(_pageIndex + 1, nowTicks);
            case GHRadialInput.SwitchSet:
            {
                int target = _def.SwitchSetPageIndex;
                if (target <= 0 || target >= PageCount)
                    return res;
                return GoToPage(_pageIndex < target ? target : 0, nowTicks);
            }
            case GHRadialInput.PrevGroup:
                return GoToPage(FindPrevGroupStart(), nowTicks);
            case GHRadialInput.NextGroup:
                return GoToPage(FindNextGroupStart(), nowTicks);
            case GHRadialInput.StepClockwise:
                return HighlightResult(_selector.Step(1, page.Slots));
            case GHRadialInput.StepCounterClockwise:
                return HighlightResult(_selector.Step(-1, page.Slots));
            case GHRadialInput.JumpNorth:
                return HighlightResult(_selector.JumpToward(GHRadialTuning.North, page.Slots));
            case GHRadialInput.JumpSouth:
                return HighlightResult(_selector.JumpToward(GHRadialTuning.South, page.Slots));
            default:
                return res;
            }
        }

        /* x, y: the selecting stick, +X right, +Y down, 0..1 after the radial deadzone */
        public GHRadialApplyResult UpdateStick(double x, double y, double dtSeconds)
        {
            if (!IsOpen)
                return new GHRadialApplyResult();
            if (double.IsNaN(x) || double.IsInfinity(x))
                x = 0;
            if (double.IsNaN(y) || double.IsInfinity(y))
                y = 0;

            double mag = Math.Sqrt(x * x + y * y);
            if (_pointerMode && mag >= GHRadialTuning.CenterMagnitude)
            {
                _pointerMode = false;
                _dirty = true;
            }
            float fx = (float)x;
            float fy = (float)y;
            if (Math.Abs(fx - _stickX) + Math.Abs(fy - _stickY) >= GHRadialTuning.StickGhostMinMove)
            {
                _stickX = fx;
                _stickY = fy;
                if (!_pointerMode)
                    _dirty = true;
            }

            return HighlightResult(_selector.UpdateStick(x, y, dtSeconds, CurrentPage.Slots));
        }

        /* Mouse hover in canvas pixels: the ring band highlights, the hub clears, outside keeps */
        public GHRadialApplyResult PointerMove(float x, float y)
        {
            if (!IsOpen || !_geometry.IsValid)
                return new GHRadialApplyResult();
            SetPointerMode();
            float r;
            double a;
            GHRadialHitZone zone = _geometry.HitTest(x, y, out r, out a);
            GHRadialPage page = CurrentPage;
            bool changed = false;
            if (zone == GHRadialHitZone.Hub)
                changed = _selector.SetHighlight(-1);
            else if (zone == GHRadialHitZone.Ring)
                changed = _selector.SetFromPointer(GHRadialGeometry.SlotAt(a, page.SlotCount), page.Slots);
            return HighlightResult(changed);
        }

        /* Left click or tap in canvas pixels: a chip or dot pages, a filled slot activates, an
           empty slot does nothing, the hub or outside closes */
        public GHRadialApplyResult PointerPress(float x, float y, long nowTicks)
        {
            GHRadialApplyResult res = new GHRadialApplyResult();
            if (!IsOpen || !_geometry.IsValid)
                return res;
            SetPointerMode();

            int chip = _geometry.HitTestChip(x, y);
            if (chip != 0)
            {
                int target = _pageIndex + chip;
                if (target >= 0 && target < PageCount)
                    return GoToPage(target, nowTicks);
            }
            int dot = _geometry.HitTestDot(x, y, PageCount);
            if (dot >= 0)
                return GoToPage(dot, nowTicks);

            float r;
            double a;
            GHRadialHitZone zone = _geometry.HitTest(x, y, out r, out a);
            if (zone == GHRadialHitZone.Ring)
            {
                GHRadialPage page = CurrentPage;
                int slot = GHRadialGeometry.SlotAt(a, page.SlotCount);
                GHRadialItem item = page.GetItem(slot);
                if (item == null)
                {
                    res.Effects = GHRadialEffects.Refused;
                    return res;
                }
                bool changed = _selector.SetHighlight(slot);
                res = Confirm(nowTicks);
                if (changed)
                    res.Effects |= GHRadialEffects.HighlightChanged;
                return res;
            }

            Close(GHRadialCloseReason.Cancelled, nowTicks);
            res.Effects = GHRadialEffects.Closed;
            res.CloseReason = GHRadialCloseReason.Cancelled;
            return res;
        }

        /* Positive steps page forward, negative back */
        public GHRadialApplyResult PointerWheel(int steps, long nowTicks)
        {
            if (steps == 0 || !IsOpen)
                return new GHRadialApplyResult();
            return Apply(steps > 0 ? GHRadialInput.NextPage : GHRadialInput.PrevPage, nowTicks);
        }

        /* Advances the animation to nowTicks and decides NeedsFrame */
        public void Tick(long nowTicks)
        {
            if (_phase == GHRadialPhase.Closed)
            {
                _needsFrame = false;
                _dirty = false;
                return;
            }

            double dt = GHRadialGeometry.ClampDouble(Seconds(_lastTick, nowTicks), 0, GHRadialTuning.MaxTickStep);
            _lastTick = nowTicks;
            Evaluate(nowTicks);
            if (_phase == GHRadialPhase.Closed)
            {
                _needsFrame = false;
                _dirty = false;
                return;
            }

            bool animating = IsAnimating;
            float step = GHRadialTuning.HighlightEaseDuration > 0 ? (float)(dt / GHRadialTuning.HighlightEaseDuration) : 1f;
            GHRadialPage page = CurrentPage;
            int n = page.SlotCount;
            int hl = _selector.HighlightedSlot;
            int k;
            for (k = 0; k < n && k < _weights.Length; k++)
            {
                float target = k == hl ? 1f : 0f;
                _weights[k] = MoveToward(_weights[k], target, step);
                if (_weights[k] != target)
                    animating = true;
            }

            if (hl != _wedgeSlot)
            {
                if (hl >= 0)
                {
                    double to = GHRadialGeometry.SlotCenterDegrees(hl, n);
                    if (_wedgeOpacity <= 0)
                    {
                        _wedgeAngle = (float)to;
                        _wedgeTweening = false;
                    }
                    else
                    {
                        _wedgeFrom = _wedgeAngle;
                        _wedgeTo = _wedgeFrom + GHRadialGeometry.AngleDelta(to, _wedgeFrom);
                        _wedgeStart = nowTicks;
                        _wedgeTweening = true;
                    }
                }
                _wedgeSlot = hl;
            }
            if (_wedgeTweening)
            {
                double p = GHRadialTuning.HighlightEaseDuration > 0
                    ? GHRadialGeometry.Clamp01(Seconds(_wedgeStart, nowTicks) / GHRadialTuning.HighlightEaseDuration) : 1.0;
                _wedgeAngle = (float)GHRadialGeometry.NormalizeDegrees(_wedgeFrom + (_wedgeTo - _wedgeFrom) * EaseOutCubic(p));
                if (p >= 1)
                    _wedgeTweening = false;
                else
                    animating = true;
            }
            float wedgeTarget = hl >= 0 ? 1f : 0f;
            _wedgeOpacity = MoveToward(_wedgeOpacity, wedgeTarget, step);
            if (_wedgeOpacity != wedgeTarget)
                animating = true;

            if (IsOpen)
            {
                int mask = ComputeToggleMask();
                if (mask != _toggleMask)
                {
                    _toggleMask = mask;
                    _dirty = true;
                }
            }

            _needsFrame = animating || _dirty;
            _dirty = false;
        }

        /* Throttles the highlight tick sound to one per HighlightSoundMinInterval; true when the
           caller may play it now */
        public bool TryClaimHighlightSound(long nowTicks)
        {
            if (_hasHighlightSound && Seconds(_lastHighlightSound, nowTicks) < GHRadialTuning.HighlightSoundMinInterval)
                return false;
            _hasHighlightSound = true;
            _lastHighlightSound = nowTicks;
            return true;
        }

        public static double EaseOutCubic(double t)
        {
            t = GHRadialGeometry.Clamp01(t);
            double u = 1 - t;
            return 1 - u * u * u;
        }

        public static double EaseInQuad(double t)
        {
            t = GHRadialGeometry.Clamp01(t);
            return t * t;
        }

        public static float SmoothStep(float t)
        {
            if (t <= 0)
                return 0;
            if (t >= 1)
                return 1;
            return t * t * (3 - 2 * t);
        }

        private GHRadialApplyResult Confirm(long nowTicks)
        {
            GHRadialApplyResult res = new GHRadialApplyResult();
            GHRadialPage page = CurrentPage;
            int slot = _selector.HighlightedSlot;
            GHRadialItem item = page == null ? null : page.GetItem(slot);
            res.Item = item;
            if (item == null || !item.IsEnabled)
            {
                res.Effects = GHRadialEffects.Refused;
                return res;
            }

            _flashSlot = slot;
            _flashPage = _pageIndex;
            _flashStart = nowTicks;
            _dirty = true;
            res.Effects = GHRadialEffects.Activated;
            GHRadialMenuDefinition def = _def;
            if (item.ClosesOnActivate)
            {
                BeginClose(nowTicks);
                res.Effects |= GHRadialEffects.Closed;
                res.CloseReason = GHRadialCloseReason.Confirmed;
                try
                {
                    InvokeActivate(def, item);
                }
                finally
                {
                    InvokeClosed(def, GHRadialCloseReason.Confirmed);
                }
            }
            else
            {
                Evaluate(nowTicks);
                InvokeActivate(def, item);
            }
            return res;
        }

        private GHRadialApplyResult GoToPage(int index, long nowTicks)
        {
            GHRadialApplyResult res = new GHRadialApplyResult();
            if (_def == null || index < 0 || index >= PageCount || index == _pageIndex)
                return res;

            _pageHighlights[_pageIndex] = _selector.HighlightedSlot;
            _previousPage = _pageIndex;
            _pageDirection = index > _pageIndex ? 1 : -1;
            _pageChangeStart = nowTicks;
            _pageIndex = index;
            _selector.OnPageChanged(CurrentPage.Slots, _pageHighlights[index]);

            /* The new page shows its highlight at once; the crossfade carries the change */
            int hl = _selector.HighlightedSlot;
            int k;
            for (k = 0; k < _weights.Length; k++)
                _weights[k] = k == hl ? 1f : 0f;
            _wedgeSlot = hl;
            _wedgeTweening = false;
            _wedgeOpacity = hl >= 0 ? 1f : 0f;
            if (hl >= 0)
                _wedgeAngle = (float)GHRadialGeometry.SlotCenterDegrees(hl, CurrentPage.SlotCount);

            _flashSlot = -1;
            _toggleMask = ComputeToggleMask();
            _dirty = true;
            Evaluate(nowTicks);
            res.Effects = GHRadialEffects.PageChanged;
            return res;
        }

        /* The first page of the run of pages after the current page's group, or -1 */
        private int FindNextGroupStart()
        {
            GHRadialPage page = CurrentPage;
            int i;
            for (i = _pageIndex + 1; i < PageCount; i++)
            {
                if (_def.Pages[i].GroupIndex != page.GroupIndex)
                    return i;
            }
            return -1;
        }

        /* The first page of the run of pages before the current page's group, or -1 */
        private int FindPrevGroupStart()
        {
            int group = CurrentPage.GroupIndex;
            int start = _pageIndex;
            while (start > 0 && _def.Pages[start - 1].GroupIndex == group)
                start--;
            if (start == 0)
                return -1;
            int prevGroup = _def.Pages[start - 1].GroupIndex;
            int t = start - 1;
            while (t > 0 && _def.Pages[t - 1].GroupIndex == prevGroup)
                t--;
            return t;
        }

        private GHRadialApplyResult HighlightResult(bool changed)
        {
            GHRadialApplyResult res = new GHRadialApplyResult();
            if (changed)
            {
                _dirty = true;
                res.Effects = GHRadialEffects.HighlightChanged;
            }
            return res;
        }

        private void SetPointerMode()
        {
            if (!_pointerMode)
            {
                _pointerMode = true;
                _dirty = true;
            }
        }

        private void BeginClose(long nowTicks)
        {
            Evaluate(nowTicks);
            _closeFromOpacity = _opacity;
            _closeFromScale = _ringScale;
            _phase = GHRadialPhase.Closing;
            _phaseStart = nowTicks;
            _dirty = true;
            Evaluate(nowTicks);
        }

        private void FinishClose()
        {
            _phase = GHRadialPhase.Closed;
            _opacity = 0;
            _backdrop = 0;
            _pageTransition = 1;
            _flash = 0;
        }

        private void Evaluate(long nowTicks)
        {
            switch (_phase)
            {
            case GHRadialPhase.Opening:
            {
                double p = Progress(_phaseStart, nowTicks, GHRadialTuning.OpenDuration);
                _opacity = (float)p;
                _ringScale = (float)Lerp(GHRadialTuning.OpenStartScale, 1.0, EaseOutCubic(p));
                if (p >= 1)
                {
                    _phase = GHRadialPhase.Open;
                    _opacity = 1;
                    _ringScale = 1;
                }
                break;
            }
            case GHRadialPhase.Open:
                _opacity = 1;
                _ringScale = 1;
                break;
            case GHRadialPhase.Closing:
            {
                double q = Progress(_phaseStart, nowTicks, GHRadialTuning.CloseDuration);
                _opacity = (float)(_closeFromOpacity * (1 - q));
                _ringScale = (float)Lerp(_closeFromScale, GHRadialTuning.CloseEndScale, EaseInQuad(q));
                if (q >= 1)
                    FinishClose();
                break;
            }
            default:
                _opacity = 0;
                _ringScale = 1;
                break;
            }
            _backdrop = (float)(GHRadialTuning.BackdropMaxOpacity * _opacity);
            _pageTransition = _previousPage >= 0 ? (float)Progress(_pageChangeStart, nowTicks, GHRadialTuning.PageChangeDuration) : 1f;
            _flash = _flashSlot >= 0 ? (float)(1 - Progress(_flashStart, nowTicks, GHRadialTuning.ConfirmFlashDuration)) : 0f;
        }

        private int ComputeToggleMask()
        {
            GHRadialPage page = CurrentPage;
            if (page == null)
                return 0;
            int mask = 0;
            int k;
            for (k = 0; k < page.SlotCount && k < 31; k++)
            {
                GHRadialItem item = page.Slots[k];
                if (item != null && item.IsToggle && item.IsOn)
                    mask |= 1 << k;
            }
            return mask;
        }

        private static void InvokeActivate(GHRadialMenuDefinition def, GHRadialItem item)
        {
            Action<GHRadialItem> cb = def == null ? null : def.OnActivate;
            if (cb != null)
                cb(item);
        }

        private static void InvokeClosed(GHRadialMenuDefinition def, GHRadialCloseReason reason)
        {
            Action<GHRadialCloseReason> cb = def == null ? null : def.OnClosed;
            if (cb != null)
                cb(reason);
        }

        private double Seconds(long fromTicks, long toTicks)
        {
            return (toTicks - fromTicks) / _ticksPerSecond;
        }

        private long ToTicks(double seconds)
        {
            return (long)Math.Round(seconds * _ticksPerSecond);
        }

        private double Progress(long startTicks, long nowTicks, double duration)
        {
            if (!(duration > 0))
                return 1.0;
            return GHRadialGeometry.Clamp01(Seconds(startTicks, nowTicks) / duration);
        }

        private static double Lerp(double a, double b, double t)
        {
            return a + (b - a) * t;
        }

        private static float MoveToward(float value, float target, float step)
        {
            if (value < target)
                return Math.Min(target, value + step);
            if (value > target)
                return Math.Max(target, value - step);
            return value;
        }
    }
}
