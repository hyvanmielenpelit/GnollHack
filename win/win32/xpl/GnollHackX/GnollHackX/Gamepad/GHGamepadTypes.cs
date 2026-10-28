using System;

namespace GnollHackX
{
    /* Physical controls, then the virtual stick directions that GHGamepadService derives from the
       stick vectors. A/B/X/Y are positional: A = south, B = east, X = west, Y = north. */
    public enum GHGamepadButton
    {
        A = 0,
        B,
        X,
        Y,
        LB,
        RB,
        LT,
        RT,
        View,
        Menu,
        L3,
        R3,
        DPadUp,
        DPadDown,
        DPadLeft,
        DPadRight,
        L4,
        L5,
        R4,
        R5,
        LeftStickUp,
        LeftStickDown,
        LeftStickLeft,
        LeftStickRight,
        RightStickUp,
        RightStickDown,
        RightStickLeft,
        RightStickRight,
        Count
    }

    /* Per press: Pressed, then any LongPressed and Repeat events, then Released.
       ShortPressed follows Released only when no LongPressed fired and the press was not cancelled. */
    public enum GHGamepadButtonEventKind
    {
        Pressed,
        Released,
        ShortPressed,
        LongPressed,
        Repeat
    }

    public struct GHGamepadButtonEvent
    {
        public GHGamepadButton Button;
        public GHGamepadButtonEventKind Kind;
        public int PressId;         /* the same for every event of one press */
        public long Timestamp;      /* Stopwatch ticks */
        public double HeldSeconds;  /* 0 for Pressed */
        public int RepeatCount;     /* 1, 2, ... for Repeat */
        public bool IsCancelled;    /* Released only: focus loss, controller switch, disable or target change */
        public bool RevealsUi;      /* the gamepad became the active input with this Poll */
    }

    /* Stick vectors are after the radial deadzone, in -1..1, with +X right and +Y down */
    public struct GHGamepadSticks
    {
        public float LeftX;
        public float LeftY;
        public float RightX;
        public float RightY;
        public float LeftMagnitude;     /* 0..1 */
        public float RightMagnitude;    /* 0..1 */
        public float LeftTrigger;       /* 0..1, after the trigger deadzone */
        public float RightTrigger;      /* 0..1, after the trigger deadzone */
        public bool IsNeutral;
    }

    public enum GHGamepadStyle
    {
        Generic,
        Xbox,
        PlayStation,
        Nintendo,
        SteamDeck
    }

    /* Consumers branch on these, never on the backend */
    public struct GHGamepadCapabilities
    {
        public bool HasAnalogSticks;        /* false: sticks report 8 directions */
        public bool HasAnalogTriggers;      /* false: LT and RT arrive as buttons */
        public bool HasGrips;               /* L4, L5, R4, R5 or paddles */
        public bool HasStickClicks;
        public bool HasRightStickFullPush;  /* digital only: a separate full-push signal */
    }

    public sealed class GHGamepadControllerInfo
    {
        public int Id;
        public string Name;
        public string BackendName;
        public string Details;              /* for logs: device, VID:PID, Steam slot */
        public GHGamepadStyle Style;
        public GHGamepadCapabilities Capabilities;
        public bool IsAuxiliary;            /* never becomes the active controller; its buttons go to the active one */
        public float InnerDeadzone = 0.15f; /* radial stick deadzone applied by the service */
    }

    public sealed class GHGamepadTiming
    {
        public static readonly GHGamepadTiming Default = new GHGamepadTiming();

        public double LongPressSeconds = 0.50;
        public double RepeatDelaySeconds = 0.40;
        public double RepeatIntervalSeconds = 0.09;
        public double StallSeconds = 0.25;          /* a longer gap between Polls resyncs the backends */
        public double MaxStickDeltaSeconds = 0.1;   /* dtSeconds passed to consumers is clamped to this */
        public float OuterDeadzone = 0.95f;
        public float TriggerDeadzone = 0.05f;
        public float TriggerPress = 0.55f;
        public float TriggerRelease = 0.35f;
        public float DirectionPress = 0.6f;         /* axis component needed to press a virtual stick direction */
        public float DirectionShare = 0.5f;         /* and its share of the magnitude, so a diagonal presses two */
        public float DirectionRelease = 0.4f;
    }

    /* The shape of a hint badge (GHGamepadGlyphs.GetBadgeKind) */
    public enum GHGamepadBadgeKind
    {
        FaceButton,         /* white circle, black letter */
        PlayStationSymbol,  /* white circle with a drawn cross, circle, square or triangle */
        Bumper,             /* L1/R1: a wide shoulder button with its label (L1, LB or L) */
        Trigger,
        StickClick,         /* L3/R3: a stick cap */
        Grip,
        Menu,               /* three horizontal lines */
        View,               /* two overlapping squares */
        DPad,
        Stick,
    }

    /* A hint badge standing for a group of controls instead of one button */
    public enum GHGamepadHintSymbol
    {
        None,
        DPad,
        LeftStick,
        RightStick,
    }

    /* One hint-strip entry: one or two button badges (or a symbol) and a label. Hold adds an outlined
       up arrow after the badges; Highlight draws the badges and that arrow highlighted, e.g. a held
       modifier. LineBreakBefore starts a hint row's second line at this entry. */
    public struct GHGamepadHint
    {
        public GHGamepadButton Button;
        public string Label;
        public bool Hold;
        public bool HasButton2;
        public GHGamepadButton Button2;
        public GHGamepadHintSymbol Symbol;  /* drawn instead of Button when not None */
        public bool Highlight;
        public bool LineBreakBefore;

        public GHGamepadHint(GHGamepadButton button, string label, bool hold)
        {
            Button = button;
            Label = label;
            Hold = hold;
            HasButton2 = false;
            Button2 = button;
            Symbol = GHGamepadHintSymbol.None;
            Highlight = false;
            LineBreakBefore = false;
        }

        public static GHGamepadHint Pair(GHGamepadButton first, GHGamepadButton second, string label)
        {
            GHGamepadHint hint = new GHGamepadHint(first, label, false);
            hint.HasButton2 = true;
            hint.Button2 = second;
            return hint;
        }

        public static GHGamepadHint ForSymbol(GHGamepadHintSymbol symbol, string label)
        {
            GHGamepadHint hint = new GHGamepadHint(GHGamepadButton.A, label, false);
            hint.Symbol = symbol;
            return hint;
        }
    }

    /* Implemented by pages that take gamepad input when they are on top of the modal stack */
    public interface IGHGamepadHandlingPage
    {
        bool HandleGamepadButton(GHGamepadButtonEvent e);
        void UpdateGamepadSticks(GHGamepadSticks sticks, double dtSeconds);
    }

    /* Implemented by pages whose in-page message popup handles gamepad input itself */
    public interface IGHGamepadPopupPage
    {
        bool SendGamepadButtonToPopup(GHGamepadButtonEvent e);
    }

    /* Called once per frame on every Poll path, whether or not the gamepad is enabled or focused */
    public interface IGHGamepadFrameTickPage
    {
        void GamepadFrameTick(long nowTicks);
    }

    /* The service's side of a backend; all calls on the UI thread */
    public interface IGHGamepadInputSink
    {
        void ControllerAdded(GHGamepadControllerInfo info);
        void ControllerRemoved(int controllerId);
        void Button(int controllerId, GHGamepadButton button, bool down, long timestamp);      /* physical buttons only */
        void Stick(int controllerId, int stick, float x, float y, long timestamp);             /* 0 left, 1 right; -1..1 raw, +Y down */
        void Trigger(int controllerId, bool right, float value, long timestamp);               /* analog backends only; 0..1 raw */
    }

    public interface IGHGamepadBackend
    {
        string Name { get; }
        bool Start(IGHGamepadInputSink sink, out string error);    /* once, on the UI thread */
        void Stop();
        void Pump(long nowTicks);                                  /* start of every Poll: deliver queued input */
        void Resync(long nowTicks);                                /* apply input silently; held buttons stay silent until released */
    }
}
