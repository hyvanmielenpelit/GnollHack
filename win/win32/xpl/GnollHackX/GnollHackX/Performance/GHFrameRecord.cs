using System;

namespace GnollHackX.Performance
{
    /* What the platform frame callback decided to do with one display refresh */
    public enum GHPacingDecision : byte
    {
        NotSet = 0,
        Rendered = 1,           /* rendered on the regular divisor pattern */
        RenderedCatchUp = 2,    /* rendered because the gap since the previous callback exceeded the target period */
        SkippedDivisor = 3,     /* skipped: not a multiple of the display-to-target divisor */
        SkippedModulo = 4,      /* skipped: the extra skip for a non-integer display-to-target ratio */
        AuxiliaryCanvas = 5,    /* a menu, text or command canvas is active instead of the map */
        Suspended = 6,
        PlatformLoopOff = 7,
        NoGamePage = 8,
        NoGame = 9,
        NoResolution = 10
    }

    /* What UpdateMainCanvas did after the tick decided to render */
    public enum GHInvalidateOutcome : byte
    {
        NotAttempted = 0,
        Invalidated = 1,
        Resizing = 2,
        RefreshOff = 3,
        NotVisible = 4,
        NoCanvasSize = 5
    }

    /* What became of the paint an invalidation requested */
    public enum GHPaintOutcome : byte
    {
        None = 0,
        Painted = 1,
        Coalesced = 2,          /* superseded by a later invalidation before any paint ran */
        OverlayVisible = 3,     /* a menu, text or command grid covers the map, or the game is not on */
        CleanedUp = 4,
        Reentrant = 5,
        MainCanvasOff = 6,
        CanvasTooSmall = 7,
        NoGame = 8,
        Failed = 9
    }

    /* Where DisplayedAtTicks came from */
    public enum GHPresentSource : byte
    {
        None = 0,
        Estimated = 1,
        Measured = 2
    }

    /* Content that appeared since the previous tick, from the requests the UI thread
       processed, plus MapUpdate, set by a paint that drew new map data */
    [Flags]
    public enum GHContentEvent : ushort
    {
        None = 0,
        FloatingText = 1,
        ScreenText = 2,         /* screen and popup texts */
        ConditionText = 4,
        GuiEffect = 8,
        ScreenFilter = 16,      /* screen filters and fades */
        Message = 32,           /* message history and top line */
        ViewChange = 64,        /* zoom and clipping */
        Window = 128,           /* windows, menus and pages shown or hidden */
        OtherRequest = 256,
        MapUpdate = 512
    }

    [Flags]
    public enum GHFrameFlags : byte
    {
        None = 0,
        DuplicateCallback = 1,  /* the platform reported the same frame time as the previous callback */
        OrphanPaint = 2         /* a paint ran with no pending invalidation, e.g. after a layout pass */
    }

    /* One display callback and everything that followed from it. Every tick field is in the
       Stopwatch domain; zero means "did not happen" or "unknown". PlatformFrameTicks is in
       Stopwatch units but with the platform's own epoch, so only its differences are
       meaningful. Written from the UI thread (tick fields) and the paint thread (paint
       fields); fields are never shared between the two writers. */
    public struct GHFrameRecord
    {
        public long FrameId;

        /* L0 display */
        public long VsyncTicks;
        public long ExpectedPresentTicks;
        public long PlatformFrameTicks;
        public long RefreshPeriodTicks;     /* measured, median of recent platform frame deltas */

        /* L1 callback */
        public long CallbackStartTicks;
        public long CallbackEndTicks;

        /* L3 content at the tick */
        public long MainCounter;
        public long GeneralCounter;

        /* L4 dispatch */
        public long InvalidateTicks;
        public long PaintStartTicks;

        /* L5 paint */
        public long LockAttemptTicks;
        public long LockResultTicks;
        public long DrawEndTicks;
        public long FlushEndTicks;
        public long PaintedMainCounter;
        public long PaintedGeneralCounter;
        public long PaintedMapGeneration;

        /* L6-L7 present */
        public long DisplayedAtTicks;

        /* UI-thread request work since the previous tick */
        public long RequestTicks;
        public GHContentEvent ContentEvents;

        public int GcCount0;
        public int GcCount1;
        public int GcCount2;
        public short TargetFps;
        public short AssumedRefreshHz;      /* the refresh rate the pacing logic believed in */

        public GHPacingDecision Pacing;
        public GHInvalidateOutcome Invalidate;
        public GHPaintOutcome Paint;
        public GHPresentSource PresentSource;
        public GHFrameFlags Flags;
        public bool LockAcquired;
        public bool PaintOnUiThread;
    }

    public enum GHCompositorSource : byte
    {
        None = 0,
        AndroidFrameMetrics = 1,
        WindowsDwm = 2
    }

    /* One frame as the platform's compositor side reported it, in the Stopwatch domain.
       Which fields are set depends on the source:
         Android FrameMetrics  IntendedVsync, Vsync, SyncStart, Completed, DroppedSinceLast,
                               FirstDrawFrame; GpuDuration and DeadlineRaw from API 31
         Windows DWM           IntendedVsync = Vsync = the latest vblank, RefreshPeriod,
                               Compose, RefreshCount, ComposedFrameCount
       Joined to the tick records afterwards, by vsync time and by sync start. */
    public struct GHCompositorFrame
    {
        public long IntendedVsyncTicks;
        public long VsyncTicks;
        public long SyncStartTicks;         /* when the RenderThread latches the newest TextureView frame */
        public long CompletedTicks;         /* intended vsync + total duration: buffer swapped */
        public long GpuDurationTicks;
        public long DeadlineRaw;            /* nanoseconds, exactly as the platform reports it */
        public long RefreshPeriodTicks;
        public long ComposeTicks;
        public long RefreshCount;
        public long ComposedFrameCount;
        public int DroppedSinceLast;        /* reports the platform itself dropped before this one */
        public GHCompositorSource Source;
        public bool FirstDrawFrame;
    }

    /* A pair of simultaneous readings of the platform clock and the Stopwatch */
    public struct GHClockAnchor
    {
        public long PlatformNanos;
        public long StopwatchTicks;
    }
}
