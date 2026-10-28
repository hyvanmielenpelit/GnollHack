using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Threading;
using GnollHackX;

#if GNH_MAUI
namespace GnollHackM
#else
using Xamarin.Essentials;

namespace GnollHackX.Pages.Game
#endif
{
    /* Where gamepad input goes on GamePage, in the order ResolveGamepadContext tests them. Replay and the contexts
       after it are map level, handled here; Radial and Dialog belong to the radial host and the dialog navigation. */
    internal enum GHGamepadMapContext
    {
        Inactive = 0,
        Radial,
        Dialog,
        Replay,
        NumberPad,
        GetPos,
        GetDir,
        Normal,
        Busy,
        UnknownWait,
    }

    /* Gameplay routing for the gamepad: context resolution, press ownership, the command gate and its pending slot,
       left-stick and D-pad walking, getpos cursor, getdir aim and D-pad pick, right-stick panning and overlay scrolling,
       the button tables, the Actions, Toggles and trigger radial definitions and the hint strip's content. Everything
       runs on the UI thread. */
    public partial class GamePage : IGHGamepadHandlingPage, IGHGamepadPopupPage, IGHGamepadFrameTickPage
    {
        /* A discrete command for the game: keys, the first tagged and the rest continuations */
        private sealed class GamepadCommand
        {
            public GHGamepadMapContext Target;  /* Normal, GetPos or GetDir */
            public int[] Keys;
        }

        /* A direction's state for the left stick or the D-pad: hysteresis, settle, repeat and the getpos jump */
        private sealed class GamepadStickDirection
        {
            public int Family;                  /* 0 none, 1 walk, 2 getpos cursor, 3 getdir aim */
            public bool Active;
            public int Sector = -1;             /* 0-7 clockwise from north */
            public long SectorSinceTicks;
            public long NextStepTicks;
            public bool FirstStep = true;
            public bool RunSent;
            public long FullPushSinceTicks;     /* 0 while not at full push */

            public void Reset()
            {
                Family = 0;
                Active = false;
                Sector = -1;
                SectorSinceTicks = 0;
                NextStepTicks = 0;
                FirstStep = true;
                RunSent = false;
                FullPushSinceTicks = 0;
            }
        }

        private const int GamepadFamilyNone = 0;
        private const int GamepadFamilyWalk = 1;
        private const int GamepadFamilyCursor = 2;
        private const int GamepadFamilyAim = 3;

        /* Per sector N, NE, E, SE, S, SW, W, NW: the direction key and the getpos 8-tile jump letter */
        private static readonly int[] GamepadSectorDirections = { -8, -9, -6, -3, -2, -1, -4, -7 };
        private static readonly int[] GamepadSectorJumpLetters = { 'K', 'U', 'L', 'N', 'J', 'B', 'H', 'Y' };
        /* The sector of a held D-pad set by (dy + 1) * 3 + (dx + 1), with +y down; -1 for none */
        private static readonly int[] GamepadDPadSectors = { 7, 0, 1, 6, -1, 2, 5, 4, 3 };
        /* The GHCommands of Rest, Search 20 and Extended, as GHApp identifies them, in their commands radial order */
        private static readonly int[] GamepadLeadingCommands = { -103, -102, (int)'#' };

        /* Bits of the held D-pad directions */
        private const int GamepadDPadUpBit = 1;
        private const int GamepadDPadDownBit = 2;
        private const int GamepadDPadLeftBit = 4;
        private const int GamepadDPadRightBit = 8;

        private readonly GHGamepadMapContext[] _gamepadButtonOwner = new GHGamepadMapContext[(int)GHGamepadButton.Count];
        private GHGamepadMapContext _gamepadContext = GHGamepadMapContext.Inactive;
        private bool _gamepadLeftModifierHeld = false;     /* L1: the layer of A, B, X, Y, Menu and the triggers */
        private bool _gamepadRightModifierHeld = false;    /* R1: the layer of the D-pad, the left stick, View and the triggers */
        private bool _gamepadViewWithRightModifier = false; /* R1 was held when View went down */
        private int _gamepadDPadHeld = 0;                   /* walking D-pad directions pressed in Normal or Busy */
        private int _gamepadDPadPrevHeld = 0;               /* _gamepadDPadHeld at the previous UpdateGamepadDPadWalk */
        private int _gamepadDPadOwed = 0;                   /* walking D-pad directions pressed since the last D-pad step */
        private int _gamepadDPadZoomHeld = 0;               /* D-pad directions pressed with R1 */
        private readonly GamepadStickDirection _gamepadDPadDirection = new GamepadStickDirection();
        private int _gamepadDirDPadHeld = 0;                /* D-pad directions pressed in GetDir and still held */
        private int _gamepadDirDPadPicked = 0;              /* D-pad directions pressed in GetDir since the last pick */
        private long _gamepadDirDPadSinceTicks = 0;         /* when the first of _gamepadDirDPadPicked went down */
        private int _gamepadHintContextCount = 0;           /* the context buttons as the hint strip last read them: 0, 1 or 2 for more */
        private int _gamepadLastSentSerial = -1;
        private long _gamepadBusySinceTicks = 0;
        private bool _gamepadInterruptSent = false;
        private GamepadCommand _gamepadPending = null;
        private long _gamepadPendingDeadline = 0;
        private readonly GamepadStickDirection _gamepadLeftDirection = new GamepadStickDirection();
        private bool _gamepadLeftStickDisarmed = false;
        private bool _gamepadRightStickDisarmed = false;
        private float _gamepadMessageScrollRemainder = 0f;
        private GHGamepadMapContext _gamepadRadialOpenedIn = GHGamepadMapContext.Normal;
        private GHRadialSlotMemory _gamepadSlotMemory = null;
        private WeakReference<GHGame> _gamepadSlotMemoryGame = null;
        private bool _gamepadStateSubscribed = false;
        private bool _gamepadHintsDirty = true;
        private int _gamepadHintKey = -1;
        private bool _gamepadHintShown = false;
        private List<GHGamepadHint> _gamepadHints = new List<GHGamepadHint>();          /* the strip's hints, read by its paint */
        private List<GHGamepadHint> _gamepadHintsBuild = new List<GHGamepadHint>();     /* built here, swapped in when it differs */
        private GHGamepadStyle _gamepadHintStyle = GHGamepadStyle.SteamDeck;            /* the style of _gamepadHints */
        private int _gamepadHintLTCommand = int.MinValue;   /* the trigger commands the strip's L2 and R2 labels show */
        private int _gamepadHintRTCommand = int.MinValue;
        private GHGamepadHintRenderer _gamepadHintRenderer;
        private float _gamepadHintCanvasHeight = 0f;        /* the HeightRequest the strip's paint last asked for, in DIP */
        private Action _applyGamepadHintCanvasHeight = null;

        /* Read by SetTargetClip on the game thread and by the paint thread */
        private int _gamepadPanHold = 0;
        private int _gamepadAimDir = 0;     /* getdir aim as a direction key (-1..-9), 0 for none */

        /* The right stick is panning: auto-center keeps the view where it is */
        private bool GamepadPanHold { get { return Interlocked.CompareExchange(ref _gamepadPanHold, 0, 0) != 0; } }

        /* The direction overlay's arrow index of the getdir aim (0 W, 1 N, 2 E, 3 S, 4 NW, 6 NE, 7 SE, 8 SW), or -1 */
        private int GetGamepadAimArrowIndex()
        {
            switch (Interlocked.CompareExchange(ref _gamepadAimDir, 0, 0))
            {
                case -4:
                    return 0;
                case -8:
                    return 1;
                case -6:
                    return 2;
                case -2:
                    return 3;
                case -7:
                    return 4;
                case -9:
                    return 6;
                case -3:
                    return 7;
                case -1:
                    return 8;
                default:
                    return -1;
            }
        }

        /* The clip's share of the map's screen position, as the map paint and IssueNHCommandViaTouch compute it: zero
           unless a clip is set and the map is larger than the canvas */
        private static float GetClipTermX(int clipX, bool largeMap, float tileWidth)
        {
            return clipX > 0 && largeMap ? (clipX - (GHConstants.MapCols - 1) / 2) * tileWidth : 0f;
        }

        private static float GetClipTermY(int clipX, int clipY, bool largeMap, float tileHeight)
        {
            return clipX > 0 && largeMap ? (clipY - GHConstants.MapRows / 2) * tileHeight : 0f;
        }

        private static long GamepadMsToTicks(int milliseconds)
        {
            return Stopwatch.Frequency * milliseconds / 1000;
        }

        private static bool IsGamepadGetPosCursor(game_cursor_types ctype)
        {
            return ctype > game_cursor_types.CURSOR_STYLE_GENERIC_CURSOR && ctype < game_cursor_types.CURSOR_STYLE_INVISIBLE;
        }

        private static bool IsGamepadMapLevel(GHGamepadMapContext ctx)
        {
            return ctx >= GHGamepadMapContext.Replay;
        }

        /* Normal and Busy alternate between every step, so a press that began in one may end in the other */
        private static GHGamepadMapContext GetGamepadOwnerFamily(GHGamepadMapContext ctx)
        {
            return ctx == GHGamepadMapContext.Busy ? GHGamepadMapContext.Normal : ctx;
        }

        private static int MapK(nh_keyfunc f)
        {
            return -100 - (int)f;
        }

        /* Context resolution */

        private GHGamepadMapContext ResolveGamepadContext()
        {
            GHGame game = GHApp.CurrentGHGame;
            if (game == null || !IsGameOn || _performanceSuiteMode || CanvasViewsShutDown || GHApp.PushingModalPage
                || !GHApp.IsPageOnTopOfModalNavigationStack(this) || LoadingGrid.IsVisible)
                return GHGamepadMapContext.Inactive;
            if (IsRadialMenuOpen)
                return GHGamepadMapContext.Radial;
            if (GetGamepadDialogContext() != GHGamepadDialogContext.None)
                return GHGamepadMapContext.Dialog;
            if (PlayingReplay)
                return GHGamepadMapContext.Replay;
            if (ShowNumberPad)
                return GHGamepadMapContext.NumberPad;

            switch (game.KeyWaitKind)
            {
                case GHKeyWaitKind.PosKey:
                    if (IsGamepadGetPosCursor(game.CursorTypeNow))
                        return GHGamepadMapContext.GetPos;
                    return GameEnteredMoveloop ? GHGamepadMapContext.Normal : GHGamepadMapContext.UnknownWait;
                case GHKeyWaitKind.Direction:
                    return GHGamepadMapContext.GetDir;
                case GHKeyWaitKind.None:
                    return GHGamepadMapContext.Busy;
                default:
                    return GHGamepadMapContext.UnknownWait;
            }
        }

        private void NoteGamepadContext(GHGamepadMapContext ctx, long now)
        {
            GHGamepadMapContext prev = _gamepadContext;
            if (ctx == prev)
                return;
            _gamepadContext = ctx;
            _gamepadHintsDirty = true;

            if (ctx == GHGamepadMapContext.Inactive)
            {
                ResetGamepadRoutingState(false);
                return;
            }

            bool wasMapLevel = IsGamepadMapLevel(prev);
            bool isMapLevel = IsGamepadMapLevel(ctx);
            /* A held stick never walks or pans into or out of a dialog or a radial */
            if (prev == GHGamepadMapContext.Radial || (wasMapLevel && !isMapLevel))
                DisarmGamepadSticks();
            if (!isMapLevel)
            {
                StopGamepadPanning();
                _gamepadLeftDirection.Reset();
            }
            if (ctx != GHGamepadMapContext.Normal && ctx != GHGamepadMapContext.Busy)
            {
                _gamepadLeftModifierHeld = false;
                _gamepadRightModifierHeld = false;
                _gamepadViewWithRightModifier = false;
                _gamepadDPadZoomHeld = 0;
                ClearGamepadDPadWalk();
            }
            ClearGamepadDirDPad();
            if (prev == GHGamepadMapContext.GetDir)
                Interlocked.Exchange(ref _gamepadAimDir, 0);
            if (ctx == GHGamepadMapContext.Busy)
            {
                _gamepadBusySinceTicks = now;
                _gamepadInterruptSent = false;
            }
            if (_gamepadPending != null && !CanGamepadPendingSurvive(_gamepadPending, ctx))
                _gamepadPending = null;
        }

        /* Clears every press, stick, layer and pending state; the radial closes and the hint strip hides */
        private void ResetGamepadRoutingState(bool unsubscribe)
        {
            if (IsRadialMenuOpen)
                DismissRadialMenu(GHRadialCloseReason.PageHidden);
            int i;
            for (i = 0; i < _gamepadButtonOwner.Length; i++)
                _gamepadButtonOwner[i] = GHGamepadMapContext.Inactive;
            _gamepadContext = GHGamepadMapContext.Inactive;
            _gamepadLeftModifierHeld = false;
            _gamepadRightModifierHeld = false;
            _gamepadViewWithRightModifier = false;
            _gamepadDPadZoomHeld = 0;
            ClearGamepadDPadWalk();
            ClearGamepadDirDPad();
            _gamepadPending = null;
            _gamepadInterruptSent = false;
            _gamepadMessageScrollRemainder = 0f;
            DisarmGamepadSticks();
            Interlocked.Exchange(ref _gamepadAimDir, 0);
            _gamepadHintsDirty = true;
            SetGamepadHintStripVisible(false);
            if (unsubscribe && _gamepadStateSubscribed)
            {
                GHGamepadService.StateChanged -= GamepadService_StateChanged;
                _gamepadStateSubscribed = false;
            }
        }

        private void DisarmGamepadSticks()
        {
            _gamepadLeftStickDisarmed = true;
            _gamepadRightStickDisarmed = true;
            _gamepadLeftDirection.Reset();
            StopGamepadPanning();
        }

        /* IGHGamepadHandlingPage, IGHGamepadPopupPage, IGHGamepadFrameTickPage */

        public bool HandleGamepadButton(GHGamepadButtonEvent e)
        {
            long now = Stopwatch.GetTimestamp();
            GHGamepadMapContext ctx = ResolveGamepadContext();
            NoteGamepadContext(ctx, now);
            int b = (int)e.Button;
            if (b < 0 || b >= _gamepadButtonOwner.Length)
                return false;

            /* Releases end the layers and the held D-pad directions wherever the press went */
            if (e.Kind == GHGamepadButtonEventKind.Released)
            {
                if (e.Button == GHGamepadButton.LB && _gamepadLeftModifierHeld)
                {
                    _gamepadLeftModifierHeld = false;
                    _gamepadHintsDirty = true;
                }
                else if (e.Button == GHGamepadButton.RB && _gamepadRightModifierHeld)
                {
                    _gamepadRightModifierHeld = false;
                    _gamepadHintsDirty = true;
                }
                else
                {
                    int dpadBit = GetGamepadDPadBit(e.Button);
                    _gamepadDPadHeld &= ~dpadBit;
                    _gamepadDPadZoomHeld &= ~dpadBit;
                    _gamepadDirDPadHeld &= ~dpadBit;
                }
            }

            /* Later events of a press go only to the context that got its Pressed */
            GHGamepadMapContext family = GetGamepadOwnerFamily(ctx);
            if (e.Kind == GHGamepadButtonEventKind.Pressed)
            {
                _gamepadButtonOwner[b] = family;
            }
            else if (_gamepadButtonOwner[b] != family)
            {
                if (e.Kind == GHGamepadButtonEventKind.Released)
                    _gamepadButtonOwner[b] = GHGamepadMapContext.Inactive;
                return ctx != GHGamepadMapContext.Inactive;
            }

            bool handled = true;
            switch (ctx)
            {
                case GHGamepadMapContext.Inactive:
                    handled = false;
                    break;
                case GHGamepadMapContext.Radial:
                    if (!HandleRadialGamepadButton(e) && e.Button == GHGamepadButton.Menu && e.Kind == GHGamepadButtonEventKind.Pressed)
                    {
                        /* The radial closed itself; Menu then opens the game menu as on the map */
                        GHGamepadMapContext after = ResolveGamepadContext();
                        NoteGamepadContext(after, now);
                        if (IsGamepadMapLevel(after))
                        {
                            _gamepadButtonOwner[b] = GetGamepadOwnerFamily(after);
                            OpenGamepadGameMenu();
                        }
                    }
                    break;
                case GHGamepadMapContext.Dialog:
                    handled = TryHandleDialogGamepadButton(e);
                    break;
                default:
                    HandleMapLevelGamepadButton(ctx, e, now);
                    break;
            }
            UpdateGamepadHintStrip(_gamepadContext);
            return handled;
        }

        public void UpdateGamepadSticks(GHGamepadSticks sticks, double dtSeconds)
        {
            long now = Stopwatch.GetTimestamp();
            GHGamepadMapContext ctx = ResolveGamepadContext();
            NoteGamepadContext(ctx, now);

            if (_gamepadLeftStickDisarmed && sticks.LeftMagnitude < GHConstants.GamepadStickReleaseMagnitude)
                _gamepadLeftStickDisarmed = false;
            if (_gamepadRightStickDisarmed && sticks.RightMagnitude < GHConstants.GamepadStickReleaseMagnitude)
                _gamepadRightStickDisarmed = false;

            switch (ctx)
            {
                case GHGamepadMapContext.Inactive:
                    break;
                case GHGamepadMapContext.Radial:
                    UpdateRadialSticks(sticks, dtSeconds);
                    break;
                case GHGamepadMapContext.Dialog:
                    UpdateDialogGamepadSticks(sticks, dtSeconds);
                    break;
                default:
                    UpdateMapGamepadSticks(ctx, sticks, dtSeconds, now);
                    break;
            }
        }

        public bool SendGamepadButtonToPopup(GHGamepadButtonEvent e)
        {
            return MessagePopup.HandleGamepadButton(e);
        }

        /* Every Poll, whatever the render loop, the setting or the window focus */
        public void GamepadFrameTick(long nowTicks)
        {
            TickRadialMenu();
            if (!GHGamepadService.IsAvailable && _gamepadPending == null && !_gamepadHintShown)
                return;

            if (!_gamepadStateSubscribed)
            {
                GHGamepadService.StateChanged += GamepadService_StateChanged;
                _gamepadStateSubscribed = true;
            }

            GHGamepadMapContext ctx = ResolveGamepadContext();
            NoteGamepadContext(ctx, nowTicks);
            if (ctx == GHGamepadMapContext.Radial && GetGamepadDialogContext() != GHGamepadDialogContext.None)
            {
                /* A dialog that appears over the radial wins */
                DismissRadialMenu(GHRadialCloseReason.Superseded);
                ctx = ResolveGamepadContext();
                NoteGamepadContext(ctx, nowTicks);
            }
            FlushGamepadPending(ctx, nowTicks);
            UpdateGamepadHintStrip(ctx);
        }

        private void GamepadService_StateChanged()
        {
            _gamepadHintsDirty = true;
            UpdateGamepadHintStrip(_gamepadContext);
        }

        /* The command gate and the pending slot */

        /* Whether the game waits for a key of the target context that the gamepad has not answered yet */
        private bool CanSendGamepadKey(GHGamepadMapContext target, out GHGame game, out int serial)
        {
            game = GHApp.CurrentGHGame;
            serial = 0;
            if (game == null || PlayingReplay)
                return false;
            int state = game.KeyWaitState;
            GHKeyWaitKind kind = GHGame.GetKeyWaitKind(state);
            serial = GHGame.GetKeyWaitSerial(state);
            if (serial == 0 || serial == _gamepadLastSentSerial)
                return false;
            switch (target)
            {
                case GHGamepadMapContext.Normal:
                    if (kind != GHKeyWaitKind.PosKey || !GameEnteredMoveloop || IsGamepadGetPosCursor(game.CursorTypeNow))
                        return false;
                    break;
                case GHGamepadMapContext.GetPos:
                    if (kind != GHKeyWaitKind.PosKey || !IsGamepadGetPosCursor(game.CursorTypeNow))
                        return false;
                    break;
                case GHGamepadMapContext.GetDir:
                    if (kind != GHKeyWaitKind.Direction)
                        return false;
                    break;
                case GHGamepadMapContext.UnknownWait:
                    if (kind != GHKeyWaitKind.GetChar && kind != GHKeyWaitKind.YnFunction)
                        return false;
                    break;
                default:
                    return false;
            }
            return !ShowNumberPad && GetGamepadDialogContext() == GHGamepadDialogContext.None;
        }

        /* One response per key, the first tagged with the wait's serial and the rest as its continuations */
        private void SendGamepadKeys(GHGame game, int serial, int[] keys)
        {
            _gamepadLastSentSerial = serial;
            int i;
            for (i = 0; i < keys.Length; i++)
            {
                GHResponse response = new GHResponse(game, GHRequestType.GetChar, keys[i]);
                response.WaitSerial = i == 0 ? serial : -serial;
                game.ResponseQueue.Enqueue(response);
            }
        }

        private bool TrySendGamepadKey(GHGamepadMapContext target, int key)
        {
            GHGame game;
            int serial;
            if (!CanSendGamepadKey(target, out game, out serial))
                return false;
            SendGamepadKeys(game, serial, new int[] { key });
            return true;
        }

        private bool TrySendGamepadKeyPair(GHGamepadMapContext target, int key1, int key2)
        {
            GHGame game;
            int serial;
            if (!CanSendGamepadKey(target, out game, out serial))
                return false;
            SendGamepadKeys(game, serial, new int[] { key1, key2 });
            return true;
        }

        /* Sends the command now when the gate is open; true also when it was dropped as unsendable */
        private bool TrySendGamepadCommand(GamepadCommand cmd)
        {
            GHGame game;
            int serial;
            if (!CanSendGamepadKey(cmd.Target, out game, out serial))
                return false;
            if (cmd.Keys != null && cmd.Keys.Length > 0)
                SendGamepadKeys(game, serial, cmd.Keys);
            return true;
        }

        /* A discrete button: sent now, else held in the pending slot, replacing any older pending command */
        private void SubmitGamepadCommand(GamepadCommand cmd, long now)
        {
            if (TrySendGamepadCommand(cmd))
            {
                _gamepadPending = null;
                return;
            }
            _gamepadPending = cmd;
            _gamepadPendingDeadline = now + GamepadMsToTicks(GHConstants.GamepadPendingCommandMs);
        }

        private void SubmitGamepadKeys(GHGamepadMapContext target, long now, params int[] keys)
        {
            GamepadCommand cmd = new GamepadCommand();
            cmd.Target = target;
            cmd.Keys = keys;
            SubmitGamepadCommand(cmd, now);
        }

        /* A pending command lives through Busy gaps and a closing radial, and dies in any other context */
        private static bool CanGamepadPendingSurvive(GamepadCommand cmd, GHGamepadMapContext ctx)
        {
            return ctx == cmd.Target || ctx == GHGamepadMapContext.Busy || ctx == GHGamepadMapContext.Radial;
        }

        private void FlushGamepadPending(GHGamepadMapContext ctx, long now)
        {
            GamepadCommand cmd = _gamepadPending;
            if (cmd == null)
                return;
            if (now >= _gamepadPendingDeadline || !CanGamepadPendingSurvive(cmd, ctx))
                _gamepadPending = null;
            else if (TrySendGamepadCommand(cmd))
                _gamepadPending = null;
        }

        /* B while the game is busy: one untagged Escape per busy period, read by the next wait, which stops running,
           traveling and occupations */
        private void InterruptGamepadBusy(long now)
        {
            if (_gamepadInterruptSent || now - _gamepadBusySinceTicks < GamepadMsToTicks(GHConstants.GamepadInterruptMinBusyMs))
                return;
            GHGame game = GHApp.CurrentGHGame;
            if (game == null || PlayingReplay)
                return;
            game.ResponseQueue.Enqueue(new GHResponse(game, GHRequestType.GetChar, GHConstants.CancelChar));
            _gamepadInterruptSent = true;
        }

        /* Map-level buttons */

        private void HandleMapLevelGamepadButton(GHGamepadMapContext ctx, GHGamepadButtonEvent e, long now)
        {
            switch (ctx)
            {
                case GHGamepadMapContext.Normal:
                case GHGamepadMapContext.Busy:
                    HandleNormalGamepadButton(ctx, e, now);
                    break;
                case GHGamepadMapContext.GetPos:
                    HandleGetPosGamepadButton(e, now);
                    break;
                case GHGamepadMapContext.GetDir:
                    HandleGetDirGamepadButton(e, now);
                    break;
                case GHGamepadMapContext.Replay:
                    HandleReplayGamepadButton(e);
                    break;
                case GHGamepadMapContext.NumberPad:
                    if (e.Button == GHGamepadButton.B && e.Kind == GHGamepadButtonEventKind.Pressed)
                        GenericButton_Clicked(this, EventArgs.Empty, GHConstants.CancelChar); /* also hides the pad */
                    break;
                case GHGamepadMapContext.UnknownWait:
                    if (e.Button == GHGamepadButton.B && e.Kind == GHGamepadButtonEventKind.Pressed)
                        TrySendGamepadKey(GHGamepadMapContext.UnknownWait, GHConstants.CancelChar);
                    break;
                default:
                    break;
            }
        }

        /* Normal and Busy. L1 and R1 are hold modifiers sampled when another button goes down: L1 for A, B, X, Y,
           Menu and the triggers, R1 for the D-pad, the left stick, View and the triggers. A trigger alone, or with
           both modifiers, runs its assigned quick action; with the opposite side's modifier it opens its quick-action
           radial, and with its own side's modifier its assign radial. In Busy, game input signals the game, which
           the command gate would otherwise hold back unseen. */
        private void HandleNormalGamepadButton(GHGamepadMapContext ctx, GHGamepadButtonEvent e, long now)
        {
            bool pressed = e.Kind == GHGamepadButtonEventKind.Pressed;
            bool mod = _gamepadLeftModifierHeld;
            bool rightMod = _gamepadRightModifierHeld;
            GHGamepadMapContext normal = GHGamepadMapContext.Normal;
            switch (e.Button)
            {
                case GHGamepadButton.LB:
                    if (pressed)
                    {
                        _gamepadLeftModifierHeld = true;
                        _gamepadHintsDirty = true;
                    }
                    break;
                case GHGamepadButton.RB:
                    if (pressed)
                    {
                        /* The D-pad zooms while R1 is held, and a walking direction must be pressed again */
                        _gamepadRightModifierHeld = true;
                        _gamepadHintsDirty = true;
                        ClearGamepadDPadWalk();
                    }
                    break;
                case GHGamepadButton.A:
                    if (!pressed)
                        break;
                    if (mod && rightMod)
                    {
                        /* Unused */
                    }
                    else if (mod)
                        OpenRadialMenu(BuildGamepadActionsRadial(ctx, true));
                    else
                        RunGamepadContextCommand(ctx);
                    break;
                case GHGamepadButton.B:
                    if (!pressed)
                        break;
                    if (mod && rightMod)
                    {
                        SignalGamepadBusyInput(ctx);
                        SubmitGamepadKeys(normal, now, GetCommandButtonKeySequence(-103)); /* Rest */
                    }
                    else if (mod)
                    {
                        SignalGamepadBusyInput(ctx);
                        SubmitGamepadKeys(normal, now, GHApp.MapCommand('.'));
                    }
                    else if (CloseGamepadOverlay())
                    {
                        /* Closed the Extended Status Bar or the message history */
                    }
                    else if (ctx == GHGamepadMapContext.Busy)
                    {
                        SignalGamepadBusyInput(ctx);
                        InterruptGamepadBusy(now);
                    }
                    else
                        SubmitGamepadKeys(normal, now, GHConstants.CancelChar);
                    break;
                case GHGamepadButton.X:
                    if (!pressed)
                        break;
                    SignalGamepadBusyInput(ctx);
                    SubmitGamepadKeys(normal, now, GHApp.MapCommand(mod ? ';' : ','));
                    break;
                case GHGamepadButton.Y:
                    if (!pressed)
                        break;
                    SignalGamepadBusyInput(ctx);
                    if (mod && rightMod)
                        SubmitGamepadKeys(normal, now, GetCommandButtonKeySequence(-102)); /* Search 20 */
                    else if (mod)
                        SubmitGamepadKeys(normal, now, GHApp.MapCommand('s'));
                    else
                        SubmitGamepadKeys(normal, now, GHApp.MapCommand(GHUtils.Ctrl('a'))); /* Repeat */
                    break;
                case GHGamepadButton.RT:
                    if (!pressed)
                        break;
                    if (mod && !rightMod)
                        OpenRadialMenu(BuildGamepadTriggerRadial(GHGamepadButton.RT, false));
                    else if (rightMod && !mod)
                        OpenRadialMenu(BuildGamepadTriggerRadial(GHGamepadButton.RT, true));
                    else
                        ExecuteGamepadBoundCommand(ctx, GHApp.GetGamepadTriggerCommand(GHGamepadButton.RT));
                    break;
                case GHGamepadButton.LT:
                    if (!pressed)
                        break;
                    if (rightMod && !mod)
                        OpenRadialMenu(BuildGamepadTriggerRadial(GHGamepadButton.LT, false));
                    else if (mod && !rightMod)
                        OpenRadialMenu(BuildGamepadTriggerRadial(GHGamepadButton.LT, true));
                    else
                        ExecuteGamepadBoundCommand(ctx, GHApp.GetGamepadTriggerCommand(GHGamepadButton.LT));
                    break;
                case GHGamepadButton.View:
                    /* A tap acts on release; R1 counts as it was when View went down */
                    if (pressed)
                        _gamepadViewWithRightModifier = rightMod;
                    else if (e.Kind == GHGamepadButtonEventKind.ShortPressed)
                    {
                        SignalGamepadBusyInput(ctx);
                        SubmitGamepadKeys(normal, now, GHApp.MapCommand(_gamepadViewWithRightModifier ? '*' : 'i'));
                    }
                    break;
                case GHGamepadButton.Menu:
                    if (!pressed)
                        break;
                    if (mod)
                    {
                        SignalGamepadBusyInput(ctx);
                        SubmitGamepadKeys(normal, now, GHApp.MapCommand('A'));
                    }
                    else
                        OpenGamepadGameMenu();
                    break;
                case GHGamepadButton.L3:
                    if (pressed)
                        CenterGamepadMapOnHero();
                    break;
                case GHGamepadButton.R3:
                    if (pressed)
                        OpenRadialMenu(BuildGamepadTogglesRadial());
                    break;
                case GHGamepadButton.DPadUp:
                case GHGamepadButton.DPadDown:
                case GHGamepadButton.DPadLeft:
                case GHGamepadButton.DPadRight:
                    HandleNormalGamepadDPad(e, rightMod);
                    break;
                case GHGamepadButton.L4:
                case GHGamepadButton.L5:
                case GHGamepadButton.R4:
                case GHGamepadButton.R5:
                    if (pressed)
                        ExecuteGamepadBoundCommand(ctx, GHApp.GetGamepadGripCommand(e.Button));
                    break;
                default:
                    break;
            }
        }

        /* A: none of the core's context buttons does nothing, one runs as its radial pick would, and several open the
           Actions radial on its context page */
        private void RunGamepadContextCommand(GHGamepadMapContext ctx)
        {
            int command;
            int count = CountGamepadContextCommands(GHApp.CurrentGHGame, false, out command);
            if (count == 1)
            {
                SignalGamepadBusyInput(ctx);
                ExecuteRadialCommand(command, GHGamepadMapContext.Normal);
            }
            else if (count > 1)
                OpenRadialMenu(BuildGamepadActionsRadial(ctx, false));
        }

        /* The core's context buttons, without the left-side ones, and the first one's command; -1 when tryOnly
           finds the lock taken */
        private static int CountGamepadContextCommands(GHGame game, bool tryOnly, out int firstCommand)
        {
            firstCommand = 0;
            if (game == null)
                return 0;
            int count = 0;
            bool lockTaken = false;
            try
            {
                if (tryOnly)
                    Monitor.TryEnter(game._contextMenuDataLock, ref lockTaken);
                else
                    Monitor.Enter(game._contextMenuDataLock, ref lockTaken);
                if (!lockTaken)
                    return -1;
                foreach (ContextMenuButton button in game._contextMenuData)
                {
                    if (button == null)
                        continue;
                    if (count == 0)
                        firstCommand = button.BtnCommand;
                    count++;
                }
            }
            finally
            {
                if (lockTaken)
                    Monitor.Exit(game._contextMenuDataLock);
            }
            return count;
        }

        /* The D-pad in Normal and Busy. Pressed with R1 it zooms; Up and Down scroll the message history while it is
           shown; otherwise its press joins the walking set that UpdateGamepadDPadWalk steps with. */
        private void HandleNormalGamepadDPad(GHGamepadButtonEvent e, bool rightMod)
        {
            bool pressed = e.Kind == GHGamepadButtonEventKind.Pressed;
            bool repeat = e.Kind == GHGamepadButtonEventKind.Repeat;
            int bit = GetGamepadDPadBit(e.Button);
            bool vertical = e.Button == GHGamepadButton.DPadUp || e.Button == GHGamepadButton.DPadDown;
            if (pressed && rightMod)
                _gamepadDPadZoomHeld |= bit;
            if ((_gamepadDPadZoomHeld & bit) != 0)
            {
                if (vertical && (pressed || repeat))
                    ZoomInOutInCurrentMode(e.Button == GHGamepadButton.DPadDown, false);
                else if (pressed && e.Button == GHGamepadButton.DPadLeft)
                    ToggleZoomMiniButton_Clicked(ToggleZoomMiniButton, EventArgs.Empty);
                else if (pressed && e.Button == GHGamepadButton.DPadRight)
                    SetZoomNormalInCurrentMode();
                return;
            }
            if (vertical && ForceAllMessages && (_gamepadDPadHeld & bit) == 0)
            {
                if (pressed || repeat)
                    ScrollMessages(e.Button == GHGamepadButton.DPadDown ? -GHConstants.GamepadMessageScrollStep : GHConstants.GamepadMessageScrollStep);
                return;
            }
            if (pressed)
            {
                _gamepadDPadHeld |= bit;
                _gamepadDPadOwed |= bit;
            }
        }

        private static int GetGamepadDPadBit(GHGamepadButton button)
        {
            switch (button)
            {
                case GHGamepadButton.DPadUp:
                    return GamepadDPadUpBit;
                case GHGamepadButton.DPadDown:
                    return GamepadDPadDownBit;
                case GHGamepadButton.DPadLeft:
                    return GamepadDPadLeftBit;
                case GHGamepadButton.DPadRight:
                    return GamepadDPadRightBit;
                default:
                    return 0;
            }
        }

        /* The compass sector 0-7 of a set of D-pad bits; opposite directions cancel, and -1 is no direction */
        private static int GetGamepadDPadSector(int bits)
        {
            int dx = ((bits & GamepadDPadRightBit) != 0 ? 1 : 0) - ((bits & GamepadDPadLeftBit) != 0 ? 1 : 0);
            int dy = ((bits & GamepadDPadDownBit) != 0 ? 1 : 0) - ((bits & GamepadDPadUpBit) != 0 ? 1 : 0);
            return GamepadDPadSectors[(dy + 1) * 3 + dx + 1];
        }

        private void ClearGamepadDPadWalk()
        {
            _gamepadDPadHeld = 0;
            _gamepadDPadPrevHeld = 0;
            _gamepadDPadOwed = 0;
            _gamepadDPadDirection.Reset();
        }

        /* D-pad walking, one step per press and per repeat in the direction of the held set, through the same gate as
           the left stick. A press waits the stick's settle time, so two presses a moment apart give one diagonal
           step. A press released before its step steps at once, through the pending slot, in the direction of the set
           it was part of. A release that leaves a direction held continues after the first repeat delay. The
           deflected left stick wins. */
        private void UpdateGamepadDPadWalk(GHGamepadMapContext ctx, long now)
        {
            GamepadStickDirection d = _gamepadDPadDirection;
            int held = _gamepadDPadHeld;
            int prevHeld = _gamepadDPadPrevHeld;
            _gamepadDPadPrevHeld = held;
            if ((ctx != GHGamepadMapContext.Normal && ctx != GHGamepadMapContext.Busy) || _gamepadRightModifierHeld
                || _gamepadLeftDirection.Active)
            {
                d.Reset();
                _gamepadDPadOwed = 0;
                return;
            }

            int sector = GetGamepadDPadSector(held);
            if ((_gamepadDPadOwed & ~held) != 0)
            {
                int paidSector = GetGamepadDPadSector(prevHeld | _gamepadDPadOwed);
                if (paidSector >= 0)
                {
                    SignalGamepadBusyInput(ctx);
                    SubmitGamepadKeys(GHGamepadMapContext.Normal, now, GamepadSectorDirections[paidSector]);
                }
                _gamepadDPadOwed = 0;
                if (sector >= 0)
                {
                    d.Family = GamepadFamilyWalk;
                    d.Active = true;
                    d.Sector = sector;
                    d.SectorSinceTicks = now;
                    d.FirstStep = false;
                    d.NextStepTicks = now + GamepadMsToTicks(GHConstants.GamepadWalkFirstRepeatDelayMs);
                }
            }
            if (sector < 0)
            {
                d.Reset();
                return;
            }

            if (sector != d.Sector)
            {
                bool fresh = d.Sector < 0 || _gamepadDPadOwed != 0;
                d.Family = GamepadFamilyWalk;
                d.Active = true;
                d.Sector = sector;
                d.SectorSinceTicks = now;
                d.FirstStep = fresh;
                d.NextStepTicks = fresh ? now : now + GamepadMsToTicks(GHConstants.GamepadWalkFirstRepeatDelayMs);
            }
            bool settled = !d.FirstStep || now - d.SectorSinceTicks >= GamepadMsToTicks(GHConstants.GamepadWalkSettleMs);
            if (!settled || now < d.NextStepTicks)
                return;
            if (TrySendGamepadKey(GHGamepadMapContext.Normal, GamepadSectorDirections[sector]))
            {
                d.NextStepTicks = now + GamepadMsToTicks(d.FirstStep ? GHConstants.GamepadWalkFirstRepeatDelayMs : GHConstants.GamepadWalkRepeatIntervalMs);
                d.FirstStep = false;
                _gamepadDPadOwed = 0;
            }
            else
            {
                /* A due step that the gate holds back signals the busy game */
                SignalGamepadBusyInput(ctx);
            }
        }

        private void ClearGamepadDirDPad()
        {
            _gamepadDirDPadHeld = 0;
            _gamepadDirDPadPicked = 0;
            _gamepadDirDPadSinceTicks = 0;
        }

        /* Direction selection: the D-pad picks a direction without A. The directions pressed within the walk's settle
           time of the first one form the pick, so two pressed together give a diagonal; a release before the settle
           picks at once. Opposite directions cancel and pick nothing. */
        private void UpdateGamepadDirDPad(GHGamepadMapContext ctx, long now)
        {
            int picked = _gamepadDirDPadPicked;
            if (picked == 0)
                return;
            if (ctx != GHGamepadMapContext.GetDir)
            {
                ClearGamepadDirDPad();
                return;
            }
            bool released = (picked & ~_gamepadDirDPadHeld) != 0;
            if (!released && now - _gamepadDirDPadSinceTicks < GamepadMsToTicks(GHConstants.GamepadWalkSettleMs))
                return;
            _gamepadDirDPadPicked = 0;
            int sector = GetGamepadDPadSector(picked);
            if (sector < 0)
                return;
            int dir = GamepadSectorDirections[sector];
            Interlocked.Exchange(ref _gamepadAimDir, dir);
            SubmitGamepadKeys(GHGamepadMapContext.GetDir, now, dir);
        }

        private void HandleGetPosGamepadButton(GHGamepadButtonEvent e, long now)
        {
            bool pressed = e.Kind == GHGamepadButtonEventKind.Pressed;
            GHGamepadMapContext getPos = GHGamepadMapContext.GetPos;
            switch (e.Button)
            {
                case GHGamepadButton.A:
                    if (pressed)
                        SubmitGamepadKeys(getPos, now, MapK(nh_keyfunc.NHKF_GETPOS_PICK));
                    break;
                case GHGamepadButton.B:
                    if (pressed && !CloseGamepadOverlay())
                        SubmitGamepadKeys(getPos, now, GHConstants.CancelChar);
                    break;
                case GHGamepadButton.X:
                    if (pressed)
                        SubmitGamepadKeys(getPos, now, MapK(nh_keyfunc.NHKF_GETPOS_OBJ_NEXT));
                    break;
                case GHGamepadButton.Y:
                    if (pressed)
                        SubmitGamepadKeys(getPos, now, MapK(nh_keyfunc.NHKF_GETPOS_SELF));
                    break;
                case GHGamepadButton.LB:
                    if (pressed)
                        SubmitGamepadKeys(getPos, now, MapK(nh_keyfunc.NHKF_GETPOS_INTERESTING_PREV));
                    break;
                case GHGamepadButton.RB:
                    if (pressed)
                        SubmitGamepadKeys(getPos, now, MapK(nh_keyfunc.NHKF_GETPOS_INTERESTING_NEXT));
                    break;
                case GHGamepadButton.LT:
                    if (pressed)
                        SubmitGamepadKeys(getPos, now, MapK(nh_keyfunc.NHKF_GETPOS_MON_PREV));
                    break;
                case GHGamepadButton.RT:
                    if (pressed)
                        SubmitGamepadKeys(getPos, now, MapK(nh_keyfunc.NHKF_GETPOS_MON_NEXT));
                    break;
                case GHGamepadButton.DPadUp:
                case GHGamepadButton.DPadDown:
                case GHGamepadButton.DPadLeft:
                case GHGamepadButton.DPadRight:
                    /* Cursor movement is never buffered */
                    if (pressed || e.Kind == GHGamepadButtonEventKind.Repeat)
                        TrySendGamepadKey(getPos, GetGamepadDPadDirection(e.Button));
                    break;
                case GHGamepadButton.Menu:
                    if (pressed)
                        OpenGamepadGameMenu();
                    break;
                case GHGamepadButton.L3:
                    if (pressed)
                        OpenRadialMenu(BuildGamepadActionsRadial(GHGamepadMapContext.GetPos, false));
                    break;
                case GHGamepadButton.R3:
                    if (pressed)
                        OpenRadialMenu(BuildGamepadTogglesRadial());
                    break;
                default:
                    break;
            }
        }

        private void HandleGetDirGamepadButton(GHGamepadButtonEvent e, long now)
        {
            bool pressed = e.Kind == GHGamepadButtonEventKind.Pressed;
            GHGamepadMapContext getDir = GHGamepadMapContext.GetDir;
            switch (e.Button)
            {
                case GHGamepadButton.A:
                    if (pressed)
                    {
                        int aim = Interlocked.CompareExchange(ref _gamepadAimDir, 0, 0);
                        SubmitGamepadKeys(getDir, now, aim != 0 ? aim : MapK(nh_keyfunc.NHKF_GETDIR_SELF));
                    }
                    break;
                case GHGamepadButton.B:
                    if (pressed && !CloseGamepadOverlay())
                        SubmitGamepadKeys(getDir, now, GHConstants.CancelChar);
                    break;
                case GHGamepadButton.Y:
                    if (pressed)
                        SubmitGamepadKeys(getDir, now, MapK(nh_keyfunc.NHKF_GETDIR_SELF));
                    break;
                case GHGamepadButton.LB:
                    if (pressed)
                        SubmitGamepadKeys(getDir, now, (int)'<');
                    break;
                case GHGamepadButton.RB:
                    if (pressed)
                        SubmitGamepadKeys(getDir, now, (int)'>');
                    break;
                case GHGamepadButton.DPadUp:
                case GHGamepadButton.DPadDown:
                case GHGamepadButton.DPadLeft:
                case GHGamepadButton.DPadRight:
                    /* Joins the pick that UpdateGamepadDirDPad submits */
                    if (pressed)
                    {
                        int bit = GetGamepadDPadBit(e.Button);
                        if (_gamepadDirDPadPicked == 0)
                            _gamepadDirDPadSinceTicks = now;
                        _gamepadDirDPadPicked |= bit;
                        _gamepadDirDPadHeld |= bit;
                    }
                    break;
                case GHGamepadButton.Menu:
                    if (pressed)
                        OpenGamepadGameMenu();
                    break;
                case GHGamepadButton.L3:
                    if (pressed)
                        OpenRadialMenu(BuildGamepadActionsRadial(GHGamepadMapContext.GetDir, false));
                    break;
                case GHGamepadButton.R3:
                    if (pressed)
                        OpenRadialMenu(BuildGamepadTogglesRadial());
                    break;
                default:
                    break;
            }
        }

        /* Mirrors the replay keyboard controls */
        private void HandleReplayGamepadButton(GHGamepadButtonEvent e)
        {
            if (e.Kind != GHGamepadButtonEventKind.Pressed)
                return;
            if (GotoTurnGrid.IsVisible)
            {
                if (e.Button == GHGamepadButton.A && GotoTurnOkButton.IsEnabled)
                    GotoTurnOkButton_Clicked(null, null);
                else if (e.Button == GHGamepadButton.B && GotoTurnCancelButton.IsEnabled)
                    GotoTurnCancelButton_Clicked(null, null);
                return;
            }
            switch (e.Button)
            {
                case GHGamepadButton.A:
                    if (ReplayPauseButton.IsEnabled)
                        ReplayPauseButton_Clicked(null, null);
                    break;
                case GHGamepadButton.B:
                    if (ReplayQuitButton.IsEnabled)
                        ReplayQuitButton_Clicked(null, null);
                    break;
                case GHGamepadButton.LB:
                case GHGamepadButton.LT:
                    if (ReplaySlowerButton.IsEnabled)
                        ReplaySlowerButton_Clicked(null, null);
                    break;
                case GHGamepadButton.RB:
                case GHGamepadButton.RT:
                    if (ReplayFasterButton.IsEnabled)
                        ReplayFasterButton_Clicked(null, null);
                    break;
                case GHGamepadButton.DPadRight:
                    if (ReplayNextButton.IsEnabled)
                        ReplayNextButton_Clicked(null, null);
                    break;
                case GHGamepadButton.Y:
                    if (ReplayGotoButton.IsVisible && ReplayGotoButton.IsEnabled)
                        ReplayGotoButton_Clicked(null, null);
                    break;
                default:
                    break;
            }
        }

        private static int GetGamepadDPadDirection(GHGamepadButton button)
        {
            switch (button)
            {
                case GHGamepadButton.DPadUp:
                    return -8;
                case GHGamepadButton.DPadDown:
                    return -2;
                case GHGamepadButton.DPadLeft:
                    return -4;
                default:
                    return -6;
            }
        }

        /* Closes the Extended Status Bar, else the message history; false when neither is open */
        private bool CloseGamepadOverlay()
        {
            if (ShowExtendedStatusBar)
            {
                ToggleExtendedStatusBar();
                return true;
            }
            if (ForceAllMessages)
            {
                ToggleMessageNumberButton_Clicked(null, EventArgs.Empty);
                return true;
            }
            return false;
        }

        /* The game menu, posted (R13); not in replays */
        private void OpenGamepadGameMenu()
        {
            if (PlayingReplay || !GameMenuButton.IsEnabled)
                return;
            OpenGameMenu();
        }

        /* A back grip's or a trigger quick action's GHCommand from the Settings pickers; Normal and Busy only */
        private void ExecuteGamepadBoundCommand(GHGamepadMapContext ctx, int ghCommand)
        {
            if (ghCommand == 0 || PlayingReplay)
                return;
            long now = Stopwatch.GetTimestamp();
            if (ghCommand < 0)
            {
                int[] keys = GetCommandButtonKeySequence(ghCommand);
                if (keys != null)
                {
                    SignalGamepadBusyInput(ctx);
                    SubmitGamepadKeys(GHGamepadMapContext.Normal, now, keys);
                }
                else if (ghCommand == -104)
                    OpenGamepadGameMenu();
                else if (ghCommand == -105 && TrySendGamepadKey(GHGamepadMapContext.Normal, MapK(nh_keyfunc.NHKF_COUNT2)))
                    DoShowNumberPad();
                return;
            }
            SignalGamepadBusyInput(ctx);
            SubmitGamepadKeys(GHGamepadMapContext.Normal, now, GHApp.MapCommand(ghCommand));
        }

        /* Game input in Busy, which the gate holds back: animations and output delays end early and running and
           traveling stop, as they do for queued touch input */
        private static void SignalGamepadBusyInput(GHGamepadMapContext ctx)
        {
            if (ctx != GHGamepadMapContext.Busy)
                return;
            GHGame game = GHApp.CurrentGHGame;
            if (game != null)
                game.SignalGamepadInput();
        }

        /* A command picked in the Actions radial, or A's only context command, gated for the context it came from */
        private void ExecuteRadialCommand(int cmd, GHGamepadMapContext openedIn)
        {
            GHGamepadMapContext target = openedIn == GHGamepadMapContext.GetPos || openedIn == GHGamepadMapContext.GetDir
                ? openedIn : GHGamepadMapContext.Normal;
            long now = Stopwatch.GetTimestamp();
            if (cmd < 0)
            {
                if (target != GHGamepadMapContext.Normal)
                    return;
                int[] keys = GetCommandButtonKeySequence(cmd);
                if (keys != null)
                    SubmitGamepadKeys(target, now, keys);
                else if (cmd == -104)
                    OpenGamepadGameMenu();
                return;
            }
            if (cmd != 0)
                SubmitGamepadKeys(target, now, cmd);
        }

        /* Sticks at map level */

        private void UpdateMapGamepadSticks(GHGamepadMapContext ctx, GHGamepadSticks sticks, double dtSeconds, long now)
        {
            /* Left stick: walk, run with R1, getpos cursor, getdir aim; then D-pad walking and the getdir D-pad pick */
            if (_gamepadLeftStickDisarmed)
                _gamepadLeftDirection.Reset();
            else
                UpdateGamepadLeftStick(ctx, sticks, now);
            UpdateGamepadDPadWalk(ctx, now);
            UpdateGamepadDirDPad(ctx, now);

            /* Right stick: scroll an open overlay, else pan the map */
            bool rightActive = !_gamepadRightStickDisarmed && sticks.RightMagnitude > 0f;
            if (rightActive && (ShowExtendedStatusBar || ForceAllMessages))
            {
                StopGamepadPanning();
                ScrollGamepadOverlay(sticks, dtSeconds);
            }
            else if (rightActive)
            {
                _gamepadMessageScrollRemainder = 0f;
                PanGamepadMap(sticks, dtSeconds);
            }
            else
            {
                _gamepadMessageScrollRemainder = 0f;
                StopGamepadPanning();
            }
        }

        private void UpdateGamepadLeftStick(GHGamepadMapContext ctx, GHGamepadSticks sticks, long now)
        {
            GamepadStickDirection d = _gamepadLeftDirection;
            int family = ctx == GHGamepadMapContext.Normal || ctx == GHGamepadMapContext.Busy ? GamepadFamilyWalk
                : ctx == GHGamepadMapContext.GetPos ? GamepadFamilyCursor
                : ctx == GHGamepadMapContext.GetDir ? GamepadFamilyAim : GamepadFamilyNone;
            float m = sticks.LeftMagnitude;
            bool engaged = d.Active ? m >= GHConstants.GamepadStickReleaseMagnitude : m >= GHConstants.GamepadStickActivateMagnitude;
            if (!engaged || family == GamepadFamilyNone)
            {
                d.Reset();
                return;
            }
            if (family != d.Family)
            {
                /* A new use of the stick starts over with the settle time */
                d.Reset();
                d.Family = family;
            }
            d.Active = true;

            int sector = GetGamepadStickSector(sticks.LeftX, sticks.LeftY, d.Sector);
            if (sector != d.Sector)
            {
                d.Sector = sector;
                d.SectorSinceTicks = now;
                d.NextStepTicks = now;
                d.FirstStep = true;
                d.RunSent = false;
                d.FullPushSinceTicks = 0;
            }
            bool settled = !d.FirstStep || m >= GHConstants.GamepadStickSettleBypassMagnitude
                || now - d.SectorSinceTicks >= GamepadMsToTicks(GHConstants.GamepadWalkSettleMs);
            int dir = GamepadSectorDirections[sector];

            switch (family)
            {
                case GamepadFamilyWalk:
                    if (!settled || d.RunSent || now < d.NextStepTicks)
                        break;
                    /* A due step or run that the gate holds back signals the busy game */
                    if (_gamepadRightModifierHeld)
                    {
                        /* Run: once per engaged direction */
                        if (TrySendGamepadKeyPair(GHGamepadMapContext.Normal, MapK(nh_keyfunc.NHKF_RUN), dir))
                            d.RunSent = true;
                        else
                            SignalGamepadBusyInput(ctx);
                    }
                    else if (TrySendGamepadKey(GHGamepadMapContext.Normal, dir))
                    {
                        d.NextStepTicks = now + GamepadMsToTicks(d.FirstStep ? GHConstants.GamepadWalkFirstRepeatDelayMs : GHConstants.GamepadWalkRepeatIntervalMs);
                        d.FirstStep = false;
                    }
                    else
                        SignalGamepadBusyInput(ctx);
                    break;
                case GamepadFamilyCursor:
                    if (m >= GHConstants.GamepadCursorJumpMagnitude)
                    {
                        if (d.FullPushSinceTicks == 0)
                            d.FullPushSinceTicks = now;
                    }
                    else
                    {
                        d.FullPushSinceTicks = 0;
                    }
                    if (!settled || now < d.NextStepTicks)
                        break;
                    if (d.FullPushSinceTicks != 0 && now - d.FullPushSinceTicks >= GamepadMsToTicks(GHConstants.GamepadCursorJumpHoldMs))
                    {
                        if (TrySendGamepadKey(GHGamepadMapContext.GetPos, GamepadSectorJumpLetters[sector]))
                        {
                            d.NextStepTicks = now + GamepadMsToTicks(GHConstants.GamepadCursorJumpIntervalMs);
                            d.FirstStep = false;
                        }
                    }
                    else if (TrySendGamepadKey(GHGamepadMapContext.GetPos, dir))
                    {
                        d.NextStepTicks = now + GamepadMsToTicks(d.FirstStep ? GHConstants.GamepadCursorFirstRepeatDelayMs : GHConstants.GamepadCursorRepeatIntervalMs);
                        d.FirstStep = false;
                    }
                    break;
                case GamepadFamilyAim:
                    /* Releasing the stick keeps the aim, so the thumb can move to A */
                    if (settled)
                        Interlocked.Exchange(ref _gamepadAimDir, dir);
                    break;
                default:
                    break;
            }
        }

        /* Compass sector 0-7 (N, NE, E, SE, S, SW, W, NW) of a stick vector with +Y down. Cardinal sectors are
           narrower than diagonal ones, and the current sector holds a little past its edges. */
        private static int GetGamepadStickSector(float x, float y, int currentSector)
        {
            double angle = Math.Atan2(x, -y) * 180.0 / Math.PI;
            if (angle < 0)
                angle += 360.0;
            if (currentSector >= 0 && currentSector < 8)
            {
                double holdHalfWidth = GetGamepadSectorHalfWidth(currentSector) + GHConstants.GamepadSectorHysteresisDegrees;
                if (Math.Abs(GetGamepadAngleDelta(angle, currentSector * 45.0)) <= holdHalfWidth)
                    return currentSector;
            }
            int sector;
            for (sector = 0; sector < 8; sector++)
            {
                if (Math.Abs(GetGamepadAngleDelta(angle, sector * 45.0)) <= GetGamepadSectorHalfWidth(sector))
                    return sector;
            }
            return 0;
        }

        private static double GetGamepadSectorHalfWidth(int sector)
        {
            return (sector % 2 == 0 ? GHConstants.GamepadCardinalSectorDegrees : 90.0 - GHConstants.GamepadCardinalSectorDegrees) / 2.0;
        }

        /* a - b in -180..180 degrees */
        private static double GetGamepadAngleDelta(double a, double b)
        {
            double d = (a - b) % 360.0;
            if (d > 180.0)
                d -= 360.0;
            else if (d < -180.0)
                d += 360.0;
            return d;
        }

        /* The stick moves the camera at a constant number of tiles per second whatever the zoom. The view keeps the
           screen center over the map, and auto-center does not move it while the stick is held. */
        private void PanGamepadMap(GHGamepadSticks sticks, double dtSeconds)
        {
            float m = sticks.RightMagnitude;
            if (m <= 0f || dtSeconds <= 0)
                return;
            float tilesPerSecond = GHConstants.GamepadPanBaseTilesPerSecond + GHConstants.GamepadPanExtraTilesPerSecond * m * m;
            float tileWidth, tileHeight, mapWidth, mapHeight;
            lock (_tileSizeLock)
            {
                tileWidth = _usedTileWidth;
                tileHeight = _usedTileHeight;
                mapWidth = _mapWidth;
                mapHeight = _mapHeight;
            }
            if (tileWidth <= 0f || tileHeight <= 0f)
                return;
            float dx = -(float)(sticks.RightX / m * tilesPerSecond * tileWidth * dtSeconds);
            float dy = -(float)(sticks.RightY / m * tilesPerSecond * tileHeight * dtSeconds);

            if (ZoomMiniMode)
            {
                lock (_mapOffsetLock)
                {
                    _mapMiniOffsetX = ClampGamepadPan(_mapMiniOffsetX, _mapMiniOffsetX + dx, -mapWidth, mapWidth);
                    _mapMiniOffsetY = ClampGamepadPan(_mapMiniOffsetY, _mapMiniOffsetY + dy, -mapHeight, mapHeight);
                }
                return;
            }

            int clipX, clipY;
            lock (_clipLock)
            {
                clipX = _clipX;
                clipY = _clipY;
            }
            float canvasWidth, canvasHeight;
            lock (_savedCanvasLock)
            {
                canvasWidth = _savedCanvasWidth;
                canvasHeight = _savedCanvasHeight;
            }
            bool largeMap = mapWidth > canvasWidth || mapHeight > canvasHeight;
            float termX = GetClipTermX(clipX, largeMap, tileWidth);
            float termY = GetClipTermY(clipX, clipY, largeMap, tileHeight);

            Interlocked.Exchange(ref _gamepadPanHold, 1);
            lock (_mapOffsetLock)
            {
                _targetClipOn = false;
                _mapOffsetX = ClampGamepadPan(_mapOffsetX, _mapOffsetX + dx, termX - mapWidth / 2, termX + mapWidth / 2);
                _mapOffsetY = ClampGamepadPan(_mapOffsetY, _mapOffsetY + dy, termY - mapHeight / 2, termY + mapHeight / 2);
            }
        }

        /* Within [low, high]; an offset already outside, from touch panning, may only move back toward the range */
        private static float ClampGamepadPan(float current, float next, float low, float high)
        {
            if (next > high)
                return current > high ? Math.Min(current, next) : high;
            if (next < low)
                return current < low ? Math.Max(current, next) : low;
            return next;
        }

        private void StopGamepadPanning()
        {
            Interlocked.Exchange(ref _gamepadPanHold, 0);
        }

        /* The right stick's vertical axis scrolls the Extended Status Bar or the message history */
        private void ScrollGamepadOverlay(GHGamepadSticks sticks, double dtSeconds)
        {
            if (dtSeconds <= 0)
                return;
            if (ShowExtendedStatusBar)
            {
                float diffY = -(float)(sticks.RightY * GHConstants.GamepadStatusScrollPixelsPerSecond * dtSeconds);
                lock (_statusOffsetLock)
                {
                    if (diffY < 0)
                    {
                        if (_statusLargestBottom > _statusClipBottom)
                            _statusOffsetY += -Math.Min(_statusLargestBottom - _statusClipBottom, -diffY);
                    }
                    else
                    {
                        _statusOffsetY += diffY;
                    }
                    if (_statusOffsetY > 0)
                        _statusOffsetY = 0;
                }
            }
            else if (ForceAllMessages)
            {
                _gamepadMessageScrollRemainder += -(float)(sticks.RightY * GHConstants.GamepadMessageScrollUnitsPerSecond * dtSeconds);
                int whole = (int)_gamepadMessageScrollRemainder;
                if (whole != 0)
                {
                    _gamepadMessageScrollRemainder -= whole;
                    ScrollMessages(whole);
                }
            }
        }

        /* Recenters the map on the hero, as turning auto-center back on does */
        private void CenterGamepadMapOnHero()
        {
            StopGamepadPanning();
            int ux, uy;
            lock (_uLock)
            {
                ux = _ux;
                uy = _uy;
            }
            if (GHUtils.isok(ux, uy))
                SetTargetClip(ux, uy, false, GHApp.CurrentGHGame?.MainCounterValue ?? 0);
        }

        /* Radial definitions */

        private static string GetGamepadUIIconPath(string fileName)
        {
            return "resource://" + GHApp.AppResourceName + ".Assets.UI." + fileName;
        }

        private static GHRadialItem CreateGamepadToggleItem(string id, string label, string description, string iconOff, string iconOn,
            double angle, Func<bool> isOn, Action toggle, bool playsOwnSound, bool closesOnActivate, bool isEnabled)
        {
            GHRadialItem item = new GHRadialItem();
            item.Id = id;
            item.Label = label;
            item.Description = description;
            item.IconPath = GetGamepadUIIconPath(iconOff);
            item.IconPathOn = iconOn == null ? null : GetGamepadUIIconPath(iconOn);
            item.Kind = GHRadialItemKind.Toggle;
            item.IsOnGetter = isOn;
            item.PreferredAngle = angle;
            item.PlaysOwnSound = playsOwnSound;
            item.ClosesOnActivate = closesOnActivate;
            item.IsEnabled = isEnabled;
            item.Tag = toggle;
            return item;
        }

        private const string GamepadMaxHitPointsToggleId = "l3:maxhp";
        private const string GamepadMaxManaToggleId = "l3:maxmana";

        /* R3: the eight toggles at fixed directions; L3 centers on the hero. The orb toggles are
           disabled when the orbs are not shown, and like an orb tap they are not saved; their icons are
           drawn orbs (DrawRadialOrbIcon), with the image as a fallback. */
        private GHRadialMenuDefinition BuildGamepadTogglesRadial()
        {
            bool orbsShown = ShowOrbs || !ClassicStatusBar;
            List<GHRadialItem> items = new List<GHRadialItem>(8);
            items.Add(CreateGamepadToggleItem("l3:statusbar", "Status Screen", "Shows the full status of your character",
                "you.png", null, GHRadialTuning.ExtendedStatusBarAngle,
                () => ShowExtendedStatusBar, ToggleExtendedStatusBar, false, false, true));
            items.Add(CreateGamepadToggleItem("l3:autocenter", "Auto-Center", "Keeps the map centered on your character",
                "stone-autocenter-off.png", "stone-autocenter-on.png", GHRadialTuning.AutoCenterAngle,
                () => !MapNoClipMode, ToggleAutoCenterMode, true, false, true));
            items.Add(CreateGamepadToggleItem("l3:lookmode", "Look Mode", "Map clicks look instead of acting",
                "stone-look-off.png", "stone-look-on.png", GHRadialTuning.LookModeAngle,
                () => MapLookMode, () => LookModeButton_Clicked(LookModeButton, EventArgs.Empty), true, false, true));
            items.Add(CreateGamepadToggleItem("l3:moremessages", "More Messages", "Shows the message history; the right stick scrolls it",
                "messages.png", null, GHRadialTuning.MoreMessagesAngle,
                () => ForceAllMessages, () => ToggleMessageNumberButton_Clicked(null, EventArgs.Empty), false, false, true));
            items.Add(CreateGamepadToggleItem("l3:ignorestopping", "Ignore Stopping", "Traveling is not interrupted by items, doors or engravings",
                "stone-ignore-off.png", "stone-ignore-on.png", GHRadialTuning.IgnoreStoppingAngle,
                () => MapIgnoreMode, () => ToggleIgnoreModeButton_BtnClicked(null, EventArgs.Empty), true, false, true));
            items.Add(CreateGamepadToggleItem("l3:autodig", "Auto-Dig", "Moving digs when you wield a digging tool",
                "stone-autodig-off.png", "stone-autodig-on.png", GHRadialTuning.AutoDigAngle,
                () => MapAutoDig, () => ToggleAutoDigButton_BtnClicked(null, EventArgs.Empty), true, false, !PlayingReplay));
            items.Add(CreateGamepadToggleItem(GamepadMaxHitPointsToggleId, "Max Hit Points", "Shows your maximum hit points in the health orb",
                "symbol-spell-healing.png", null, GHRadialTuning.MaxHitPointsAngle,
                () => ShowMaxHealthInOrb, () => ShowMaxHealthInOrb = !ShowMaxHealthInOrb, false, false, orbsShown));
            items.Add(CreateGamepadToggleItem(GamepadMaxManaToggleId, "Max Mana", "Shows your maximum mana in the mana orb",
                "symbol-mana.png", null, GHRadialTuning.MaxManaAngle,
                () => ShowMaxManaInOrb, () => ShowMaxManaInOrb = !ShowMaxManaInOrb, false, false, orbsShown));

            GHRadialMenuDefinition def = new GHRadialMenuDefinition();
            def.MenuId = "R3";
            def.OwningStick = 1;
            def.Pages.Add(GHRadialSlotAssigner.BuildFixedPage("Toggles", 0, items));
            def.ConfirmHint = "Toggle";
            def.CancelHint = "Close";
            def.OnActivate = OnGamepadRadialActivate;
            def.OnClosed = OnGamepadRadialClosed;
            ApplyGamepadRadialGlyphs(def);
            return def;
        }

        /* The name of a trigger as the Set radial titles show it */
        private static string GetGamepadTriggerName(GHGamepadButton trigger)
        {
            return trigger == GHGamepadButton.LT ? "L2" : "R2";
        }

        /* A trigger's eight quick-action slots, slot n at (n - 1) * 45 degrees from north; a None slot is disabled.
           The quick-action radial (gold) runs the picked slot's action; the assign radial (teal) makes the picked
           slot the one the trigger runs alone. Pressing the trigger again closes either. */
        private GHRadialMenuDefinition BuildGamepadTriggerRadial(GHGamepadButton trigger, bool assign)
        {
            string triggerName = GetGamepadTriggerName(trigger);
            List<GHRadialItem> items = new List<GHRadialItem>(GHApp.GamepadTriggerSlotCount);
            int slot;
            for (slot = 0; slot < GHApp.GamepadTriggerSlotCount; slot++)
            {
                int command = GHApp.GetGamepadTriggerSlotCommand(trigger, slot);
                SelectableShortcutButton shortcut = GHApp.GetGamepadCommandItem(command);
                GHRadialItem item = new GHRadialItem();
                item.Id = (assign ? "set:" : "quick:") + triggerName + ":" + slot.ToString();
                item.PreferredAngle = slot * 45.0;
                item.ClosesOnActivate = true;
                if (command == 0 || shortcut == null)
                {
                    item.Label = "None";
                    item.Kind = GHRadialItemKind.Command;
                    item.IsEnabled = false;
                    items.Add(item);
                    continue;
                }
                item.Label = shortcut.Label;
                item.Description = GetGamepadItemDescription(shortcut.Name, shortcut.Label);
                item.IconPath = string.IsNullOrEmpty(shortcut.ImageSourcePath) ? null : "resource://" + shortcut.ImageSourcePath;
                if (assign)
                {
                    int assignedSlot = slot;
                    item.Kind = GHRadialItemKind.Toggle;
                    item.IsOnGetter = () => GHApp.GetGamepadTriggerAssignedSlot(trigger) == assignedSlot;
                    item.Tag = (Action)(() => AssignGamepadTriggerSlot(trigger, assignedSlot));
                }
                else
                {
                    int slotCommand = command;
                    item.Kind = GHRadialItemKind.Command;
                    item.ShortcutText = command > 0 ? GHUtils.ConstructShortcutText(GHApp.MapCommand(command)) : null;
                    item.Tag = (Action)(() => ExecuteGamepadTriggerQuickAction(slotCommand));
                }
                items.Add(item);
            }

            GHRadialMenuDefinition def = new GHRadialMenuDefinition();
            def.MenuId = trigger == GHGamepadButton.LT ? "LT" : "RT";
            def.OwningStick = trigger == GHGamepadButton.LT ? 0 : 1;
            if (assign)
            {
                def.Pages.Add(GHRadialSlotAssigner.BuildFixedPage("Set " + triggerName, 0, items));
                def.Accent = GHRadialAccent.Teal;
                def.ConfirmHint = "Set";
                string hub = triggerName + " now: " + GHApp.GetGamepadCommandItem(GHApp.GetGamepadTriggerCommand(trigger)).Label;
                def.NoHighlightStickText = hub;
                def.NoHighlightPointerText = hub;
            }
            else
            {
                def.Pages.Add(GHRadialSlotAssigner.BuildFixedPage(trigger == GHGamepadButton.LT ? "Left Quick" : "Right Quick", 0, items));
                def.Accent = GHRadialAccent.Gold;
                def.ConfirmHint = "Use";
            }
            def.CancelHint = "Close";
            def.OnActivate = OnGamepadRadialActivate;
            def.OnClosed = OnGamepadRadialClosed;
            ApplyGamepadRadialGlyphs(def);
            return def;
        }

        /* The assign radial's pick: the trigger now runs that slot, saved at once */
        private void AssignGamepadTriggerSlot(GHGamepadButton trigger, int slot)
        {
            GHApp.SetGamepadTriggerAssignedSlot(trigger, slot);
            Preferences.Set(GHApp.GamepadTriggerAssignedPreferenceKey(trigger), slot);
            _gamepadHintsDirty = true;
        }

        /* The quick-action radial's pick, run as the trigger would run it; the radial has closed by now, so the
           context is the map's again */
        private void ExecuteGamepadTriggerQuickAction(int command)
        {
            GHGamepadMapContext ctx = ResolveGamepadContext();
            ExecuteGamepadBoundCommand(ctx == GHGamepadMapContext.Busy ? GHGamepadMapContext.Busy : GHGamepadMapContext.Normal, command);
        }

        /* The Actions radial: the context buttons with the left-side buttons, then the commands pages (Normal and Busy
           only: in getpos and getdir their letters would be misread). openOnCommands opens it on the first commands
           page when there is one. */
        private GHRadialMenuDefinition BuildGamepadActionsRadial(GHGamepadMapContext ctx, bool openOnCommands)
        {
            GHGamepadMapContext openedIn = ctx == GHGamepadMapContext.GetPos || ctx == GHGamepadMapContext.GetDir ? ctx : GHGamepadMapContext.Normal;
            _gamepadRadialOpenedIn = openedIn;
            bool withCommands = openedIn == GHGamepadMapContext.Normal;
            GHGame game = GHApp.CurrentGHGame;

            List<GHRadialItem> fixedItems = new List<GHRadialItem>(3);
            List<GHRadialItem> contextItems = new List<GHRadialItem>();
            if (game != null)
            {
                lock (game._contextMenuDataLock)
                {
                    foreach (ContextMenuButton button in game._contextMenuData)
                    {
                        if (button == null)
                            continue;
                        GHRadialSourceItem source = new GHRadialSourceItem();
                        source.Id = GHRadialSlotAssigner.GetContextItemId(button.BtnCommand, button.LblText);
                        source.Label = button.LblText;
                        source.Description = GetGamepadItemDescription(button.TargetText, button.LblText);
                        source.IconPath = button.ImgSourcePath;
                        source.Command = button.BtnCommand;
                        source.Page = -1;
                        source.Column = -1;
                        source.Row = -1;
                        source.ShortcutText = button.ShortcutText;
                        contextItems.Add(GHRadialSlotAssigner.CreateCommandItem(source));
                    }
                }
                if (withCommands)
                    AddGamepadLeftSideItems(game, fixedItems);
            }

            string title = openedIn == GHGamepadMapContext.GetPos ? "Targeting" : openedIn == GHGamepadMapContext.GetDir ? "Direction" : "Context";
            GHRadialMenuDefinition def = new GHRadialMenuDefinition();
            def.MenuId = "L3";
            def.OwningStick = 0;
            def.Pages.AddRange(GHRadialSlotAssigner.AssignContextPages(title, 0, fixedItems, contextItems, GetGamepadSlotMemory(game)));
            if (def.Pages.Count > 0 && def.Pages[0].FilledCount == 0)
                def.Pages[0].EmptyText = "Nothing to do here";

            if (withCommands)
            {
                bool wizardMode = EnableWizardMode;
                List<GHRadialSourceItem> commands = new List<GHRadialSourceItem>();
                /* Rest, Search 20 and Extended, which the list keeps at its end, come first in this order */
                GHRadialSourceItem[] leading = new GHRadialSourceItem[GamepadLeadingCommands.Length];
                lock (GHApp._moreBtnLock)
                {
                    int i;
                    for (i = 0; i < GHApp._moreBtnList.Count; i++)
                    {
                        GHCommandButtonRect buttonRect = GHApp._moreBtnList[i];
                        GHCommandButtonItem cbi = buttonRect.CommandButtonItem;
                        /* Back to Game and the count pad have no gamepad use */
                        if (cbi == null || cbi.GHCommand == -101 || cbi.GHCommand == -105 || (buttonRect.IsWizardModeCommand && !wizardMode))
                            continue;
                        GHRadialSourceItem source = new GHRadialSourceItem();
                        source.Id = "cmd:" + cbi.GHCommand.ToString() + ":" + cbi.Text;
                        source.Label = cbi.Text;
                        source.Description = null; /* the hub shows only the name */
                        source.IconPath = "resource://" + cbi.ImageSourcePath;
                        source.Command = cbi.MappedGHCommand;
                        source.Page = -1;
                        source.Column = -1;
                        source.Row = -1;
                        source.ShortcutText = cbi.ShortcutText;
                        int lead = Array.IndexOf(GamepadLeadingCommands, cbi.GHCommand);
                        if (lead < 0)
                            commands.Add(source);
                        else if (leading[lead] == null)
                            leading[lead] = source;
                    }
                }
                int insertAt = 0;
                foreach (GHRadialSourceItem source in leading)
                {
                    if (source != null)
                        commands.Insert(insertAt++, source);
                }
                /* The single commands page's list with the leading three moved first; group index 1 only tells the
                   pages from the context pages, since the radial host sends no group inputs */
                List<GHRadialPage> commandPages = GHRadialSlotAssigner.BuildPackedPages(commands, "Commands", GHRadialTuning.CommandPageSlotCount, 1);
                if (commandPages.Count > 0)
                {
                    def.SwitchSetPageIndex = def.Pages.Count;
                    def.Pages.AddRange(commandPages);
                    def.SecondaryHint = "Commands";
                    def.SecondaryHintOnSwitchedSet = "Context";
                    if (openOnCommands)
                        def.InitialPageIndex = def.SwitchSetPageIndex;
                }
            }

            def.ConfirmHint = "Select";
            def.CancelHint = "Close";
            def.GroupHint = null;
            def.OnActivate = OnGamepadRadialActivate;
            def.OnClosed = OnGamepadRadialClosed;
            ApplyGamepadRadialGlyphs(def);
            return def;
        }

        /* A radial item's description, or null when it would only repeat the label */
        private static string GetGamepadItemDescription(string description, string label)
        {
            if (string.IsNullOrWhiteSpace(description))
                return null;
            if (label != null && string.Equals(description.Trim(), label.Trim(), StringComparison.Ordinal))
                return null;
            return description;
        }

        /* Skills, Polearm and Wield Last / Unwield, visible when their on-screen buttons would be */
        private static void AddGamepadLeftSideItems(GHGame game, List<GHRadialItem> items)
        {
            bool skillsAvailable;
            lock (game.StatusFieldLock)
            {
                string skillText = game.StatusFields[(int)NhStatusFields.BL_SKILL].Text;
                skillsAvailable = skillText != null && skillText == "Skill";
            }
            if (skillsAvailable && GHApp.ShowSkillContextButton)
                items.Add(CreateGamepadFixedCommandItem("left:skills", "Skills", "View and advance your skills", "skill.png", 'S', GHRadialTuning.SkillsAngle));

            ObjectDataItem weapon;
            lock (game._weaponStyleObjDataItemLock)
            {
                weapon = game._weaponStyleObjDataItem[0];
            }
            if (weapon == null)
                return;
            if (weapon.IsAppliablePolearm && GHApp.ShowPolearmContextButton)
                items.Add(CreateGamepadFixedCommandItem("left:polearm", "Polearm", "Apply your polearm at a target", "polearm.png", GHUtils.Meta('P'), GHRadialTuning.PolearmAngle));
            if (weapon.PreviousWeaponFound || weapon.PreviousUnwield)
            {
                bool unwield = weapon.PreviousUnwield;
                items.Add(CreateGamepadFixedCommandItem("left:wieldlast", unwield ? "Unwield" : "Wield Last",
                    unwield ? "Empty your hands" : "Wield your previous weapon", unwield ? "unwield.png" : "wield.png",
                    GHUtils.Meta('W'), GHRadialTuning.WieldLastAngle));
            }
        }

        /* A command item with a fixed direction; the command is sent as the touch button sends it */
        private static GHRadialItem CreateGamepadFixedCommandItem(string id, string label, string description, string iconFile, int command, double angle)
        {
            GHRadialSourceItem source = new GHRadialSourceItem();
            source.Id = id;
            source.Label = label;
            source.Description = description;
            source.IconPath = GetGamepadUIIconPath(iconFile);
            source.Command = command;
            source.Page = -1;
            source.Column = -1;
            source.Row = -1;
            source.ShortcutText = GHUtils.ConstructShortcutText(command);
            GHRadialItem item = GHRadialSlotAssigner.CreateCommandItem(source);
            item.PreferredAngle = angle;
            return item;
        }

        /* One slot memory per game session */
        private GHRadialSlotMemory GetGamepadSlotMemory(GHGame game)
        {
            GHGame rememberedGame = null;
            if (_gamepadSlotMemory == null || _gamepadSlotMemoryGame == null || !_gamepadSlotMemoryGame.TryGetTarget(out rememberedGame)
                || !ReferenceEquals(rememberedGame, game))
            {
                _gamepadSlotMemory = new GHRadialSlotMemory();
                _gamepadSlotMemoryGame = game == null ? null : new WeakReference<GHGame>(game);
            }
            return _gamepadSlotMemory;
        }

        private static void ApplyGamepadRadialGlyphs(GHRadialMenuDefinition def)
        {
            def.Style = GHGamepadService.Style;
        }

        /* Toggles items carry an Action, Actions items their GHRadialSourceItem */
        private void OnGamepadRadialActivate(GHRadialItem item)
        {
            if (item == null)
                return;
            Action action = item.Tag as Action;
            if (action != null)
            {
                action();
                return;
            }
            GHRadialSourceItem source = item.Tag as GHRadialSourceItem;
            if (source != null)
                ExecuteRadialCommand(source.Command, _gamepadRadialOpenedIn);
        }

        /* Both sticks wait for the center before they walk or pan again */
        private void OnGamepadRadialClosed(GHRadialCloseReason reason)
        {
            DisarmGamepadSticks();
            _gamepadHintsDirty = true;
        }

        /* Hint strip */

        /* Shown while the gamepad is the active input, outside radials and the loading screen; the hints change only
           with the context, the L1 and R1 layers, the controller style, the dialog kind, the triggers' assigned
           actions or, on the map, whether there are none, one or several context buttons. The canvas repaints only
           when the hints differ from the shown ones. */
        private void UpdateGamepadHintStrip(GHGamepadMapContext ctx)
        {
            bool show = GHGamepadService.IsActive && ctx != GHGamepadMapContext.Inactive && ctx != GHGamepadMapContext.Radial
                && !IsRadialMenuOpen && !LoadingGrid.IsVisible;
            if (!show)
            {
                SetGamepadHintStripVisible(false);
                return;
            }

            /* An unmodeled wait is often a moment before a dialog appears: keep the current text */
            if (ctx != GHGamepadMapContext.UnknownWait)
            {
                GHGamepadStyle style = GHGamepadService.Style;
                GHGamepadDialogContext dialog = ctx == GHGamepadMapContext.Dialog ? GetGamepadDialogContext() : GHGamepadDialogContext.None;
                int layers = (_gamepadLeftModifierHeld ? 1 : 0) + (_gamepadRightModifierHeld ? 2 : 0);
                if (ctx == GHGamepadMapContext.Normal || ctx == GHGamepadMapContext.Busy)
                {
                    /* A lock the game thread holds keeps the last count */
                    int firstCommand;
                    int contextCount = CountGamepadContextCommands(GHApp.CurrentGHGame, true, out firstCommand);
                    if (contextCount >= 0)
                        _gamepadHintContextCount = Math.Min(contextCount, 2);
                }
                /* Settings and the assign radial change the triggers' actions */
                int ltCommand = GHApp.GetGamepadTriggerCommand(GHGamepadButton.LT);
                int rtCommand = GHApp.GetGamepadTriggerCommand(GHGamepadButton.RT);
                if (ltCommand != _gamepadHintLTCommand || rtCommand != _gamepadHintRTCommand)
                {
                    _gamepadHintLTCommand = ltCommand;
                    _gamepadHintRTCommand = rtCommand;
                    _gamepadHintsDirty = true;
                }
                int key = ((((int)GetGamepadOwnerFamily(ctx) * 64 + (int)dialog) * 16 + (int)style) * 4 + layers) * 4 + _gamepadHintContextCount;
                if (_gamepadHintsDirty || key != _gamepadHintKey)
                {
                    _gamepadHintsDirty = false;
                    _gamepadHintKey = key;
                    List<GHGamepadHint> built = _gamepadHintsBuild;
                    built.Clear();
                    BuildGamepadHints(built, ctx);
                    if (style != _gamepadHintStyle || !AreGamepadHintsEqual(built, _gamepadHints))
                    {
                        _gamepadHintsBuild = _gamepadHints;
                        _gamepadHints = built;
                        _gamepadHintStyle = style;
                        if (!CanvasViewsShutDown)
                            GamepadHintCanvas.InvalidateSurface();
                    }
                }
            }
            SetGamepadHintStripVisible(_gamepadHints.Count > 0);
        }

        private static bool AreGamepadHintsEqual(List<GHGamepadHint> a, List<GHGamepadHint> b)
        {
            if (a.Count != b.Count)
                return false;
            int i;
            for (i = 0; i < a.Count; i++)
            {
                GHGamepadHint x = a[i];
                GHGamepadHint y = b[i];
                if (x.Button != y.Button || x.Hold != y.Hold || x.HasButton2 != y.HasButton2 || x.Button2 != y.Button2
                    || x.Symbol != y.Symbol || x.Highlight != y.Highlight || x.LineBreakBefore != y.LineBreakBefore
                    || !string.Equals(x.Label, y.Label, StringComparison.Ordinal))
                    return false;
            }
            return true;
        }

        private void SetGamepadHintStripVisible(bool visible)
        {
            if (_gamepadHintShown == visible)
                return;
            _gamepadHintShown = visible;
            GamepadHintGrid.IsVisible = visible;
        }

        /* The strip's hints for a context; dialog hints come from the dialog navigation */
        private void BuildGamepadHints(List<GHGamepadHint> hints, GHGamepadMapContext ctx)
        {
            switch (ctx)
            {
                case GHGamepadMapContext.Dialog:
                    GetDialogGamepadHints(hints);
                    break;
                case GHGamepadMapContext.Normal:
                case GHGamepadMapContext.Busy:
                    if (_gamepadLeftModifierHeld && _gamepadRightModifierHeld)
                    {
                        AddGamepadHeldModifierHint(hints, GHGamepadHint.Pair(GHGamepadButton.LB, GHGamepadButton.RB, null));
                        hints.Add(new GHGamepadHint(GHGamepadButton.Y, "Search 20", false));
                        hints.Add(new GHGamepadHint(GHGamepadButton.B, "Rest", false));
                    }
                    else if (_gamepadLeftModifierHeld)
                    {
                        AddGamepadHeldModifierHint(hints, new GHGamepadHint(GHGamepadButton.LB, null, true));
                        hints.Add(new GHGamepadHint(GHGamepadButton.A, "Commands", false));
                        hints.Add(new GHGamepadHint(GHGamepadButton.B, "Wait", false));
                        hints.Add(new GHGamepadHint(GHGamepadButton.X, "Stash", false));
                        hints.Add(new GHGamepadHint(GHGamepadButton.Y, "Search", false));
                        hints.Add(new GHGamepadHint(GHGamepadButton.RT, "Quick", false));
                        hints.Add(new GHGamepadHint(GHGamepadButton.LT, "Set", false));
                        hints.Add(new GHGamepadHint(GHGamepadButton.Menu, "Abilities", false));
                    }
                    else if (_gamepadRightModifierHeld)
                    {
                        AddGamepadHeldModifierHint(hints, new GHGamepadHint(GHGamepadButton.RB, null, true));
                        hints.Add(new GHGamepadHint(GHGamepadButton.View, "Equipment", false));
                        hints.Add(new GHGamepadHint(GHGamepadButton.LT, "Quick", false));
                        hints.Add(new GHGamepadHint(GHGamepadButton.RT, "Set", false));
                        hints.Add(GHGamepadHint.ForSymbol(GHGamepadHintSymbol.DPad, "Zoom"));
                        hints.Add(GHGamepadHint.ForSymbol(GHGamepadHintSymbol.LeftStick, "Run"));
                    }
                    else
                    {
                        if (_gamepadHintContextCount > 0)
                            hints.Add(new GHGamepadHint(GHGamepadButton.A, _gamepadHintContextCount == 1 ? "Act" : "Actions", false));
                        hints.Add(new GHGamepadHint(GHGamepadButton.B, "Cancel", false));
                        hints.Add(new GHGamepadHint(GHGamepadButton.X, "Pick up", false));
                        hints.Add(new GHGamepadHint(GHGamepadButton.Y, "Repeat", false));
                        AddGamepadTriggerHint(hints, GHGamepadButton.LT, _gamepadHintLTCommand);
                        AddGamepadTriggerHint(hints, GHGamepadButton.RT, _gamepadHintRTCommand);
                        GHGamepadHint leftModifier = new GHGamepadHint(GHGamepadButton.LB, "More", true);
                        leftModifier.LineBreakBefore = true;
                        hints.Add(leftModifier);
                        hints.Add(new GHGamepadHint(GHGamepadButton.RB, "More", true));
                        hints.Add(new GHGamepadHint(GHGamepadButton.L3, "Center", false));
                        hints.Add(new GHGamepadHint(GHGamepadButton.R3, "Toggles", false));
                        hints.Add(new GHGamepadHint(GHGamepadButton.Menu, "Game menu", false));
                        hints.Add(new GHGamepadHint(GHGamepadButton.View, "Inventory", false));
                    }
                    break;
                case GHGamepadMapContext.GetPos:
                    hints.Add(new GHGamepadHint(GHGamepadButton.A, "Select", false));
                    hints.Add(new GHGamepadHint(GHGamepadButton.B, "Cancel", false));
                    hints.Add(GHGamepadHint.Pair(GHGamepadButton.LB, GHGamepadButton.RB, "Interesting"));
                    hints.Add(GHGamepadHint.Pair(GHGamepadButton.LT, GHGamepadButton.RT, "Monsters"));
                    hints.Add(new GHGamepadHint(GHGamepadButton.Y, "Self", false));
                    break;
                case GHGamepadMapContext.GetDir:
                    hints.Add(GHGamepadHint.ForSymbol(GHGamepadHintSymbol.DPad, "Pick"));
                    hints.Add(GHGamepadHint.ForSymbol(GHGamepadHintSymbol.LeftStick, "Aim"));
                    hints.Add(new GHGamepadHint(GHGamepadButton.A, "Confirm", false));
                    hints.Add(new GHGamepadHint(GHGamepadButton.B, "Cancel", false));
                    hints.Add(new GHGamepadHint(GHGamepadButton.LB, "Up", false));
                    hints.Add(new GHGamepadHint(GHGamepadButton.RB, "Down", false));
                    break;
                case GHGamepadMapContext.Replay:
                    hints.Add(new GHGamepadHint(GHGamepadButton.A, "Pause", false));
                    hints.Add(GHGamepadHint.Pair(GHGamepadButton.LB, GHGamepadButton.RB, "Speed"));
                    hints.Add(new GHGamepadHint(GHGamepadButton.B, "Quit", false));
                    break;
                case GHGamepadMapContext.NumberPad:
                    hints.Add(new GHGamepadHint(GHGamepadButton.B, "Cancel", false));
                    break;
                default:
                    break;
            }
        }

        /* The held modifier's badges and hold mark, highlighted and without a label, first */
        private static void AddGamepadHeldModifierHint(List<GHGamepadHint> hints, GHGamepadHint hint)
        {
            hint.Hold = true;
            hint.Highlight = true;
            hints.Add(hint);
        }

        /* A trigger and the label of its assigned action; nothing when the action is None */
        private static void AddGamepadTriggerHint(List<GHGamepadHint> hints, GHGamepadButton trigger, int command)
        {
            if (command == 0)
                return;
            SelectableShortcutButton item = GHApp.GetGamepadCommandItem(command);
            if (item == null || item.GHCommand == 0 || string.IsNullOrEmpty(item.Label))
                return;
            hints.Add(new GHGamepadHint(trigger, item.Label, false));
        }
    }
}
