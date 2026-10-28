using System;
using System.Collections.Generic;
using System.Diagnostics;
using GnollHackX;
using SkiaSharp;

#if GNH_MAUI
using SkiaSharp.Views.Maui;

namespace GnollHackM
#else
using SkiaSharp.Views.Forms;
using Xamarin.Essentials;

namespace GnollHackX.Pages.Game
#endif
{
    /* Radial menu host: shows one GHRadialMenuState on RadialCanvas, translates controller buttons, sticks, keys,
       mouse and touch into its inputs and plays its sounds. TickRadialMenu, called from the gamepad frame tick on
       every frame, advances the animation, repaints, hides the canvas after the fade and closes the menu when the
       page or the window loses the player. Everything runs on the UI thread. */
    public partial class GamePage
    {
        private const float RadialTapSlopDips = 12f;            /* a touch that moves farther is a drag */
        private const int RadialWheelNotch = 120;               /* wheel units per page */
        private const double RadialWheelIdleSeconds = 0.3;      /* the first wheel event after this pages at once */
        private const float GamepadHintFontSizeDips = 14f;      /* the hint strip's label size */
        private const float GamepadHintMaxWidthShare = 0.9f;    /* of the canvas width, backing included */
        private const float GamepadHintExtraHeightDips = 2f;    /* above the backing */

        private readonly GHRadialMenuState _radialState = new GHRadialMenuState();
        private GHRadialMenuRenderer _radialRenderer;
        private bool _radialCanvasShown;
        private long _radialTouchId = -1;
        private SKPoint _radialTouchStart;
        private bool _radialTouchDragged;
        private bool _radialHasPointerLocation;
        private SKPoint _radialPointerLocation;
        private int _radialWheelAccumulator;
        private long _radialLastWheelTicks;

        /* The orb toggle icons; separate from the map's paints, which may paint on another thread */
        private readonly SKPaint _radialOrbPaint = new SKPaint();
        private readonly SKPaint _radialOrbLayerPaint = new SKPaint();
        private readonly GHSkiaFontPaint _radialOrbTextPaint = new GHSkiaFontPaint();

        /* False from the moment of confirm or cancel, while the closing fade may still show */
        public bool IsRadialMenuOpen { get { return _radialState.IsOpen; } }

        public void OpenRadialMenu(GHRadialMenuDefinition def)
        {
            if (def == null || !CanOpenRadialMenu())
                return;
            if (EnsureRadialRenderer() == null)
                return;

            long now = Stopwatch.GetTimestamp();
            if (_radialState.IsOpen)
                _radialState.Close(GHRadialCloseReason.Superseded, now);
            if (!_radialState.Open(def, now))
            {
                SyncRadialCanvas();
                return;
            }

            ResetRadialPointerState();
            _radialCanvasShown = true;
            RadialCanvas.IsVisible = true;
            RadialCanvas.InputTransparent = false;
            GHApp.PlayMenuSelectSound();
            InvalidateRadialCanvas();
        }

        /* PageHidden, Deactivated and GameEnded hide at once; the other reasons fade out */
        public void DismissRadialMenu(GHRadialCloseReason reason)
        {
            if (!_radialState.IsOpen)
                return;
            bool immediate = reason == GHRadialCloseReason.PageHidden || reason == GHRadialCloseReason.Deactivated
                || reason == GHRadialCloseReason.GameEnded;
            try
            {
                _radialState.Close(reason, Stopwatch.GetTimestamp(), immediate);
            }
            finally
            {
                SyncRadialCanvas();
            }
        }

        /* Acts on Pressed, and on Repeat for D-pad Left and Right; every other event is swallowed. Menu closes the
           radial and returns false, so that the caller opens the game menu. */
        public bool HandleRadialGamepadButton(GHGamepadButtonEvent e)
        {
            if (!_radialState.IsOpen)
                return false;
            bool pressed = e.Kind == GHGamepadButtonEventKind.Pressed;
            bool repeat = e.Kind == GHGamepadButtonEventKind.Repeat;
            if (!pressed && !repeat)
                return true;

            GHRadialMenuDefinition def = _radialState.Definition;
            GHGamepadButton opener = GetRadialOpenerButton(def);
            bool isActionsMenu = opener == GHGamepadButton.L3;
            bool hasInput = false;
            GHRadialInput input = GHRadialInput.Confirm;
            switch (e.Button)
            {
            case GHGamepadButton.A:
                hasInput = pressed;
                input = GHRadialInput.Confirm;
                break;
            case GHGamepadButton.B:
                hasInput = pressed;
                input = GHRadialInput.Cancel;
                break;
            case GHGamepadButton.X:
                hasInput = pressed && isActionsMenu;
                input = GHRadialInput.SwitchSet;
                break;
            case GHGamepadButton.LB:
                hasInput = pressed;
                input = GHRadialInput.PrevPage;
                break;
            case GHGamepadButton.RB:
                hasInput = pressed;
                input = GHRadialInput.NextPage;
                break;
            case GHGamepadButton.L3:
            case GHGamepadButton.R3:
            case GHGamepadButton.LT:
            case GHGamepadButton.RT:
                /* The button that opened the radial closes it */
                hasInput = pressed && e.Button == opener;
                input = GHRadialInput.OpenerPressed;
                break;
            case GHGamepadButton.DPadLeft:
                hasInput = true;
                input = GHRadialInput.StepCounterClockwise;
                break;
            case GHGamepadButton.DPadRight:
                hasInput = true;
                input = GHRadialInput.StepClockwise;
                break;
            case GHGamepadButton.DPadUp:
                hasInput = pressed;
                input = GHRadialInput.JumpNorth;
                break;
            case GHGamepadButton.DPadDown:
                hasInput = pressed;
                input = GHRadialInput.JumpSouth;
                break;
            case GHGamepadButton.Menu:
                if (pressed)
                {
                    DismissRadialMenu(GHRadialCloseReason.Superseded);
                    return false;
                }
                break;
            default:
                /* Y is reserved; View and the back grips are swallowed */
                break;
            }
            if (hasInput)
                ApplyRadialInput(input);
            return true;
        }

        /* The stick pushed further selects; the owning stick wins a tie */
        public void UpdateRadialSticks(GHGamepadSticks sticks, double dtSeconds)
        {
            if (!_radialState.IsOpen)
                return;
            GHRadialMenuDefinition def = _radialState.Definition;
            bool owningRight = def != null && def.OwningStick == 1;
            bool useRight = sticks.RightMagnitude > sticks.LeftMagnitude
                || (sticks.RightMagnitude == sticks.LeftMagnitude && owningRight);
            GHRadialApplyResult res = useRight
                ? _radialState.UpdateStick(sticks.RightX, sticks.RightY, dtSeconds)
                : _radialState.UpdateStick(sticks.LeftX, sticks.LeftY, dtSeconds);
            PlayRadialSounds(res, Stopwatch.GetTimestamp());
        }

        /* Runs inside the Avalonia frame callback, so every radial repaint happens here */
        public void TickRadialMenu()
        {
            if (!_radialState.IsVisible)
            {
                if (_radialCanvasShown)
                    HideRadialCanvas();
                return;
            }

            if (_radialState.IsOpen)
            {
                if (CanvasViewsShutDown || !IsGameOn)
                    DismissRadialMenu(GHRadialCloseReason.GameEnded);
                else if (!GHApp.IsPageOnTopOfModalNavigationStack(this))
                    DismissRadialMenu(GHRadialCloseReason.PageHidden);
                else if (!GHGamepadService.IsWindowActive)
                    DismissRadialMenu(GHRadialCloseReason.Deactivated);
            }

            _radialState.Tick(Stopwatch.GetTimestamp());
            if (!_radialState.IsVisible)
            {
                HideRadialCanvas();
                return;
            }
            if (_radialState.NeedsFrame)
                InvalidateRadialCanvas();
        }

        /* Resets the state without callbacks and frees the radial and hint strip renderers, when the canvases shut down */
        public void ShutDownRadial()
        {
            _radialState.Reset();
            _radialTouchId = -1;
            GHRadialMenuRenderer renderer = _radialRenderer;
            _radialRenderer = null;
            if (renderer != null)
                renderer.Dispose();
            GHGamepadHintRenderer hintRenderer = _gamepadHintRenderer;
            _gamepadHintRenderer = null;
            if (hintRenderer != null)
                hintRenderer.Dispose();
            if (_radialCanvasShown)
            {
                _radialCanvasShown = false;
                RadialCanvas.InputTransparent = true;
                RadialCanvas.IsVisible = false;
            }
        }

        /* Game on, no replay, the page on top and no other overlay or dialog showing */
        private bool CanOpenRadialMenu()
        {
            return GHApp.CurrentGHGame != null && IsGameOn && !PlayingReplay && !CanvasViewsShutDown && !GHApp.PushingModalPage
                && GHApp.IsPageOnTopOfModalNavigationStack(this)
                && !LoadingGrid.IsVisible && !MenuGrid.IsVisible && !TextGrid.IsVisible && !YnGrid.IsVisible
                && !GetLineGrid.IsVisible && !PopupGrid.IsVisible && !MoreCommandsGrid.IsVisible && !TipView.IsVisible;
        }

        /* Requests that replace the map's input with a dialog, a page or the end of the game */
        private static bool ClosesRadialMenu(GHRequestType requestType)
        {
            switch (requestType)
            {
            case GHRequestType.ShowYnResponses:
            case GHRequestType.ShowDirections:
            case GHRequestType.GetLine:
            case GHRequestType.AskName:
            case GHRequestType.ShowMenuPage:
            case GHRequestType.ShowOutRipPage:
            case GHRequestType.DisplayWindowView:
            case GHRequestType.DisplayPopupText:
            case GHRequestType.ReturnToMainMenu:
            case GHRequestType.RestartGame:
            case GHRequestType.RestartReplay:
            case GHRequestType.RestartGameAfterPageDestruction:
                return true;
            default:
                return false;
            }
        }

        private static GHGamepadButton GetRadialOpenerButton(GHRadialMenuDefinition def)
        {
            if (def == null)
                return GHGamepadButton.L3;
            if (def.MenuId == "R3")
                return GHGamepadButton.R3;
            if (def.MenuId == "L3")
                return GHGamepadButton.L3;
            if (def.MenuId == "LT")
                return GHGamepadButton.LT;
            if (def.MenuId == "RT")
                return GHGamepadButton.RT;
            return def.OwningStick == 1 ? GHGamepadButton.R3 : GHGamepadButton.L3;
        }

        private GHRadialMenuRenderer EnsureRadialRenderer()
        {
            if (CanvasViewsShutDown)
                return null;
            if (_radialRenderer == null)
            {
                GHRadialMenuRenderer renderer = new GHRadialMenuRenderer();
                renderer.IconLookup = p => GHApp.GetCachedImageSourceBitmap(p, true);
                renderer.CustomIconDrawer = DrawRadialOrbIcon;
                renderer.TitleTypeface = GHApp.ImmortalTypeface;
                renderer.LabelTypeface = GHApp.LatoRegular;
                renderer.BoldTypeface = GHApp.LatoBold;
                renderer.DescriptionTypeface = GHApp.UnderwoodTypeface;
                renderer.UseTextBlobCaching = GHApp.UseTextBlobCaching;
                renderer.ShowKeyboardShortcuts = GHApp.ShowKeyboardShortcuts;
                _radialRenderer = renderer;
            }
            return _radialRenderer;
        }

        /* The Max Hit Points and Max Mana toggles show their orb as the map draws it, with the maximum always shown */
        private bool DrawRadialOrbIcon(SKCanvas canvas, GHRadialItem item, SKRect dest, bool enabled, bool highlighted)
        {
            bool mana;
            if (item.Id == GamepadMaxHitPointsToggleId)
                mana = false;
            else if (item.Id == GamepadMaxManaToggleId)
                mana = true;
            else
                return false;

            string val = "", maxval = "";
            float fill = 0.0f;
            GHGame curGame = GHApp.CurrentGHGame;
            if (curGame != null)
            {
                lock (curGame.StatusFieldLock)
                {
                    GetOrbValues(curGame.StatusFields, mana, out val, out maxval, out fill);
                }
            }

            int saveCount = -1;
            if (!enabled)
            {
                _radialOrbLayerPaint.ColorFilter = UIUtils.GrayedColorFilter;
                saveCount = canvas.SaveLayer(dest, _radialOrbLayerPaint);
            }
            DrawOrb(canvas, _radialOrbPaint, _radialOrbTextPaint, dest, mana ? SKColors.Blue : SKColors.Red, val, maxval, fill, true, highlighted && enabled);
            if (saveCount >= 0)
                canvas.RestoreToCount(saveCount);
            return true;
        }

        private void ResetRadialPointerState()
        {
            _radialTouchId = -1;
            _radialTouchDragged = false;
            _radialHasPointerLocation = false;
            _radialWheelAccumulator = 0;
            _radialLastWheelTicks = 0;
        }

        /* Input stops reaching the canvas at the logical close; the canvas hides when the fade ends */
        private void SyncRadialCanvas()
        {
            if (!_radialState.IsVisible)
            {
                HideRadialCanvas();
                return;
            }
            if (!_radialState.IsOpen && _radialCanvasShown && !RadialCanvas.InputTransparent)
                RadialCanvas.InputTransparent = true;
        }

        private void HideRadialCanvas()
        {
            _radialTouchId = -1;
            if (!_radialCanvasShown)
                return;
            _radialCanvasShown = false;
            RadialCanvas.InputTransparent = true;
            RadialCanvas.IsVisible = false;
        }

        private void InvalidateRadialCanvas()
        {
            if (!_radialCanvasShown || CanvasViewsShutDown)
                return;
            // WORKAROUND(WA-AVALONIA-0021): An animation frame that commits nothing waits for a 16 ms timer
            RadialCanvas.InvalidateSurface();
        }

        private void ApplyRadialInput(GHRadialInput input)
        {
            long now = Stopwatch.GetTimestamp();
            GHRadialApplyResult res;
            try
            {
                res = _radialState.Apply(input, now);
            }
            finally
            {
                SyncRadialCanvas();
            }
            PlayRadialSounds(res, now);
        }

        /* Confirm after OnActivate unless the item plays its own; Menu Select on paging; a throttled quiet tick on
           highlight changes; silence on cancel and on refused confirms */
        private void PlayRadialSounds(GHRadialApplyResult res, long now)
        {
            if (res.Has(GHRadialEffects.Activated))
            {
                if (res.Item != null && !res.Item.PlaysOwnSound)
                    GHApp.PlayButtonClickedSound();
            }
            else if (res.Has(GHRadialEffects.PageChanged))
            {
                GHApp.PlayMenuSelectSound();
            }
            else if (res.Has(GHRadialEffects.HighlightChanged))
            {
                if (_radialState.TryClaimHighlightSound(now))
                    GHApp.PlayRadialHighlightSound();
            }
        }

        /* Escape closes, Enter or Space confirms, Left and Right step, Up and Down jump, Page Up and Page Down page;
           other keys do nothing while the radial is open */
        private void HandleRadialSpecialKey(GHSpecialKey key)
        {
            if (!_radialState.IsOpen)
                return;
            switch (key)
            {
            case GHSpecialKey.Escape:
                ApplyRadialInput(GHRadialInput.Cancel);
                break;
            case GHSpecialKey.Enter:
            case GHSpecialKey.Space:
                ApplyRadialInput(GHRadialInput.Confirm);
                break;
            case GHSpecialKey.Left:
                ApplyRadialInput(GHRadialInput.StepCounterClockwise);
                break;
            case GHSpecialKey.Right:
                ApplyRadialInput(GHRadialInput.StepClockwise);
                break;
            case GHSpecialKey.Up:
                ApplyRadialInput(GHRadialInput.JumpNorth);
                break;
            case GHSpecialKey.Down:
                ApplyRadialInput(GHRadialInput.JumpSouth);
                break;
            case GHSpecialKey.PageUp:
                ApplyRadialInput(GHRadialInput.PrevPage);
                break;
            case GHSpecialKey.PageDown:
                ApplyRadialInput(GHRadialInput.NextPage);
                break;
            default:
                break;
            }
        }

        /* Canvas pixels per device-independent pixel */
        private float GetRadialDipScale(int canvasPixelWidth)
        {
            double width = RadialCanvas.ThreadSafeWidth;
            if (width > 0 && canvasPixelWidth > 0)
                return (float)(canvasPixelWidth / width);
            float density = GHApp.DisplayDensity;
            return density > 0 ? density : 1.0f;
        }

        private bool IsRadialRingPoint(float x, float y)
        {
            GHRadialGeometry g = _radialState.Geometry;
            if (!g.IsValid)
                return false;
            float r;
            double a;
            return g.HitTest(x, y, out r, out a) == GHRadialHitZone.Ring;
        }

        private GHGamepadHintRenderer EnsureGamepadHintRenderer()
        {
            if (CanvasViewsShutDown)
                return null;
            if (_gamepadHintRenderer == null)
            {
                GHGamepadHintRenderer renderer = new GHGamepadHintRenderer();
                renderer.LabelTypeface = GHApp.LatoRegular;
                renderer.BoldTypeface = GHApp.LatoBold;
                renderer.UseTextBlobCaching = GHApp.UseTextBlobCaching;
                _gamepadHintRenderer = renderer;
            }
            return _gamepadHintRenderer;
        }

        /* The gamepad hint strip, painted on the UI thread (UseGL is off): the row is centered at the bottom, at
           most GamepadHintMaxWidthShare of the width with its backing, and wraps onto a second line that grows
           upward; the canvas height follows the row */
        private void GamepadHintCanvas_PaintSurface(object sender, SKPaintSurfaceEventArgs e)
        {
            SKCanvas canvas = e.Surface.Canvas;
            canvas.Clear(SKColors.Transparent);
            List<GHGamepadHint> hints = _gamepadHints;
            if (hints.Count == 0 || CanvasViewsShutDown)
                return;
            GHGamepadHintRenderer renderer = EnsureGamepadHintRenderer();
            if (renderer == null)
                return;

            int pixelWidth = e.Info.Width;
            int pixelHeight = e.Info.Height;
            double dipWidth = GamepadHintCanvas.Width;
            float scale;
            if (dipWidth > 0 && pixelWidth > 0)
                scale = (float)(pixelWidth / dipWidth);
            else
                scale = GHApp.DisplayDensity > 0 ? GHApp.DisplayDensity : 1.0f;

            renderer.UseTextBlobCaching = GHApp.UseTextBlobCaching;
            float fontSize = GamepadHintFontSizeDips * scale;
            float maxWidth = pixelWidth * GamepadHintMaxWidthShare - 2f * fontSize * GHGamepadHintRenderer.BackingPadOfFont;
            renderer.LayoutRow(hints, _gamepadHintStyle, fontSize, maxWidth);
            if (renderer.LineCount == 0)
                return;

            float padY = renderer.BackingPadY;
            renderer.DrawRow(canvas, hints, _gamepadHintStyle, pixelWidth * 0.5f, pixelHeight - padY, 1.0f, true);

            /* The height the row needs, set after the paint and only when it changes; the layout does not depend on
               the height, so the next paint asks for the same */
            float neededDips = (float)Math.Ceiling((renderer.RowHeight + 2f * padY) / scale) + GamepadHintExtraHeightDips;
            if (Math.Abs(neededDips - _gamepadHintCanvasHeight) > 1f && Math.Abs(neededDips - GamepadHintCanvas.HeightRequest) > 1f)
            {
                _gamepadHintCanvasHeight = neededDips;
                if (_applyGamepadHintCanvasHeight == null)
                    _applyGamepadHintCanvasHeight = ApplyGamepadHintCanvasHeight;
                MainThread.BeginInvokeOnMainThread(_applyGamepadHintCanvasHeight);
            }
        }

        private void ApplyGamepadHintCanvasHeight()
        {
            float height = _gamepadHintCanvasHeight;
            if (height > 0f && !CanvasViewsShutDown && Math.Abs(height - GamepadHintCanvas.HeightRequest) > 1f)
            {
                GamepadHintCanvas.HeightRequest = height;
                /* Centered in the command bar, the strip's margin depends on its height */
                UpdateGamepadHintStripPosition();
            }
        }

        private void RadialCanvas_PaintSurface(object sender, SKPaintSurfaceEventArgs e)
        {
            SKCanvas canvas = e.Surface.Canvas;
            GHRadialMenuRenderer renderer = _radialRenderer;
            if (renderer == null || CanvasViewsShutDown)
            {
                canvas.Clear(SKColors.Transparent);
                return;
            }
            renderer.UseTextBlobCaching = GHApp.UseTextBlobCaching;
            renderer.ShowKeyboardShortcuts = GHApp.ShowKeyboardShortcuts;
            renderer.Paint(canvas, e.Info, _radialState, GetRadialDipScale(e.Info.Width));
        }

        /* A tap or left click activates a slot, pages on a chip or dot and closes on the hub or outside; a drag
           highlights under the finger and activates only when released on the ring; a right click closes */
        private void RadialCanvas_Touch(object sender, SKTouchEventArgs e)
        {
            if (e == null)
                return;
            if (!_radialState.IsOpen)
            {
                _radialTouchId = -1;
                return;
            }
            if (e.ActionType == SKTouchAction.WheelChanged)
                return; /* RadialCanvas_MouseWheel pages */

            e.Handled = true;
            long now = Stopwatch.GetTimestamp();
            float x = e.Location.X;
            float y = e.Location.Y;
            switch (e.ActionType)
            {
            case SKTouchAction.Pressed:
                if (e.MouseButton == SKMouseButton.Right)
                {
                    _radialTouchId = -1;
                    ApplyRadialInput(GHRadialInput.Cancel);
                    break;
                }
                if (e.MouseButton == SKMouseButton.Middle)
                    break;
                _radialTouchId = e.Id;
                _radialTouchStart = e.Location;
                _radialTouchDragged = false;
                PlayRadialSounds(_radialState.PointerMove(x, y), now);
                break;
            case SKTouchAction.Moved:
                if (e.Id != _radialTouchId || !e.InContact)
                    break;
                if (!_radialTouchDragged)
                {
                    float slop = RadialTapSlopDips * GetRadialDipScale((int)_radialState.Geometry.Width);
                    float dx = x - _radialTouchStart.X;
                    float dy = y - _radialTouchStart.Y;
                    if (dx * dx + dy * dy > slop * slop)
                        _radialTouchDragged = true;
                }
                if (_radialTouchDragged)
                    PlayRadialSounds(_radialState.PointerMove(x, y), now);
                break;
            case SKTouchAction.Released:
                if (e.Id != _radialTouchId)
                    break;
                _radialTouchId = -1;
                if (!_radialTouchDragged || IsRadialRingPoint(x, y))
                {
                    GHRadialApplyResult res;
                    try
                    {
                        res = _radialState.PointerPress(x, y, now);
                    }
                    finally
                    {
                        SyncRadialCanvas();
                    }
                    PlayRadialSounds(res, now);
                }
                break;
            case SKTouchAction.Cancelled:
                if (e.Id == _radialTouchId)
                    _radialTouchId = -1;
                break;
            default:
                break;
            }
        }

        /* Forward pages back, backward pages forward, one page per notch */
        private void RadialCanvas_MouseWheel(object sender, GHMouseWheelEventArgs e)
        {
            if (e == null || e.MouseWheelDelta == 0 || !_radialState.IsOpen)
                return;
            long now = Stopwatch.GetTimestamp();
            int delta = e.MouseWheelDelta;
            bool idle = _radialLastWheelTicks == 0 || now - _radialLastWheelTicks > (long)(RadialWheelIdleSeconds * Stopwatch.Frequency);
            _radialLastWheelTicks = now;
            if (idle || (_radialWheelAccumulator != 0 && Math.Sign(delta) != Math.Sign(_radialWheelAccumulator)))
                _radialWheelAccumulator = Math.Abs(delta) < RadialWheelNotch ? Math.Sign(delta) * RadialWheelNotch : delta;
            else
                _radialWheelAccumulator += delta;

            int steps = 0;
            while (_radialWheelAccumulator >= RadialWheelNotch)
            {
                steps--;
                _radialWheelAccumulator -= RadialWheelNotch;
            }
            while (_radialWheelAccumulator <= -RadialWheelNotch)
            {
                steps++;
                _radialWheelAccumulator += RadialWheelNotch;
            }
            if (steps != 0)
                PlayRadialSounds(_radialState.PointerWheel(steps, now), now);
        }

        /* Hover highlights; the first event after opening only records the position, so that a mouse resting
           over the ring does not preselect a slot */
        private void RadialCanvas_MousePointer(object sender, SKTouchEventArgs e)
        {
            if (e == null)
                return;
            SKPoint p = e.Location;
            bool moved = _radialHasPointerLocation && p != _radialPointerLocation;
            _radialHasPointerLocation = true;
            _radialPointerLocation = p;
            if (!_radialState.IsOpen || e.ActionType != SKTouchAction.Moved || !moved)
                return;
            PlayRadialSounds(_radialState.PointerMove(p.X, p.Y), Stopwatch.GetTimestamp());
        }
    }
}
