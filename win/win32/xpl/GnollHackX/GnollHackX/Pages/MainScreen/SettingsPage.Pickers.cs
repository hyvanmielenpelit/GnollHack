using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Threading.Tasks;
using GnollHackX;
using SkiaSharp;

#if GNH_MAUI
using SkiaSharp.Views.Maui;
using Picker = Microsoft.Maui.Controls.Picker;

namespace GnollHackM
#else
using SkiaSharp.Views.Forms;
using Xamarin.Forms;
using GnollHackX.Controls;
using Picker = Xamarin.Forms.Picker;

namespace GnollHackX.Pages.MainScreen
#endif
{
    /* A on a picker with the controller: a radial for commands, which have icons, on PickerRadialCanvas, eight per page;
       a scrolling list popup for a plain list of GamepadPickerListMinItems or more; nothing for a shorter list, which A
       steps. Everything runs on the UI thread. */
    public partial class SettingsPage : IGHGamepadFrameTickPage
    {
        private const int GamepadPickerListMinItems = 2;
        private const double PickerListRowHeight = 36.0;
        private const double PickerListMaxVisibleRows = 8.5;        /* half a row shows at an edge, so a longer list plainly scrolls */
        private const double PickerListVerticalMargin = 140.0;      /* of the page, for the caption, frame and padding */
        private const int PickerListLayoutWaitMilliseconds = 40;
        private const int PickerListMaxLayoutWaits = 10;
        private const float PickerRadialTapSlopDips = 12f;          /* a touch that moves farther is a drag */

        private readonly GHRadialMenuState _pickerRadialState = new GHRadialMenuState();
        private GHRadialMenuRenderer _pickerRadialRenderer;
        private bool _pickerRadialCanvasShown;
        private long _pickerRadialTouchId = -1;
        private SKPoint _pickerRadialTouchStart;
        private bool _pickerRadialTouchDragged;
        private GHGamepadPageNavigator.Layer _pickerListLayer;

        /* Buttons held while a selector was open; their later events do not reach the page once it closes */
        private readonly bool[] _gamepadPickerHeld = new bool[(int)GHGamepadButton.Count];
        /* After a selector closes, a stick waits for the center before it acts on the page again */
        private bool _gamepadPickerLeftStickDisarmed;
        private bool _gamepadPickerRightStickDisarmed;

        /* False lets A step the picker */
        private bool OpenGamepadPickerSelector(Picker picker, Label label)
        {
            string title = label?.Text?.Trim();
            IList<SelectableShortcutButton> commands = picker.ItemsSource as IList<SelectableShortcutButton>;
            if (commands != null && commands.Count > 0)
            {
                OpenPickerRadial(picker, commands, title);
                return true;
            }
            if (picker.Items.Count >= GamepadPickerListMinItems)
            {
                OpenPickerList(picker, title);
                return true;
            }
            return false;
        }

        /* From InitializeGamepadNavigation */
        private void InitializePickerListNavigation()
        {
            _pickerListLayer = _gamepadNavigator.AddOverlay(() => PickerListGrid.IsVisible);
            _pickerListLayer.ScrollView = PickerListScrollView;
            _pickerListLayer.ScrollsContent = true;
            _pickerListLayer.Back = () =>
            {
                ClosePickerList();
                return true;
            };
            PickerListGrid.PropertyChanged += GamepadOverlay_PropertyChanged;
        }

        private void OpenPickerList(Picker picker, string title)
        {
            _pickerListLayer.Clear();
            PickerListLayout.Children.Clear();
            int count = picker.Items.Count;
            int selected = picker.SelectedIndex;
            View selectedRow = null;
            int i;
            for (i = 0; i < count; i++)
            {
                int index = i;
                Label text = new Label();
                text.Text = picker.Items[i];
                text.FontFamily = "Underwood";
                text.FontSize = 17;
                text.TextColor = i == selected ? GHColors.LightGreen : GHColors.White;
                text.VerticalOptions = LayoutOptions.Center;
                text.Margin = new Thickness(24, 0, 8, 0);
                Grid row = new Grid();
                row.HeightRequest = PickerListRowHeight;
                row.BackgroundColor = GHColors.Transparent;     /* hit-tested across the row, not only on its text */
                if (i == selected)
                {
                    Label marker = new Label();
                    marker.Text = ((char)0x2022).ToString();
                    marker.FontFamily = "LatoRegular";
                    marker.FontSize = 17;
                    marker.TextColor = GHColors.LightGreen;
                    marker.HorizontalOptions = LayoutOptions.Start;
                    marker.VerticalOptions = LayoutOptions.Center;
                    marker.Margin = new Thickness(8, 0, 0, 0);
                    row.Children.Add(marker);
                }
                row.Children.Add(text);
                Action choose = () => ChoosePickerListItem(picker, index);
#if GNH_AVALONIA
                // WORKAROUND(WA-AVALONIA-0024): ACM raises a tap when the pointer goes down
                AddPickerListRowPointer(row, choose);
#else
                TapGestureRecognizer tap = new TapGestureRecognizer();
                tap.Tapped += (s, e) => choose();
                row.GestureRecognizers.Add(tap);
#endif
                PickerListLayout.Children.Add(row);
                _pickerListLayer.Add(row, choose, null);
                if (i == selected)
                    selectedRow = row;
            }

            PickerListCaption.Text = title ?? "";
            double rowPitch = PickerListRowHeight + PickerListLayout.Spacing;
            double available = RootGrid.Height > PickerListVerticalMargin ? RootGrid.Height - PickerListVerticalMargin : PickerListRowHeight;
            PickerListScrollView.HeightRequest = Math.Min(available, Math.Min(count, PickerListMaxVisibleRows) * rowPitch);
            _pickerListLayer.SetInitialFocus(selectedRow);
            PickerListGrid.IsVisible = true;
            GHApp.PlayMenuSelectSound();
            if (selectedRow != null)
                ScrollPickerListAfterLayout(selectedRow);
        }

        private async void ScrollPickerListAfterLayout(View row)
        {
            try
            {
                int waits = 0;
                do
                {
                    await Task.Delay(PickerListLayoutWaitMilliseconds);
                    waits++;
                }
                while (row.Height <= 0 && waits < PickerListMaxLayoutWaits);
                if (PickerListGrid.IsVisible && ReferenceEquals(row.Parent, PickerListLayout))
                    GHGamepadFocusList.BringIntoView(PickerListScrollView, row, ScrollToPosition.MakeVisible);
            }
            catch (Exception ex)
            {
                GHApp.ReportCaughtException(ex);
            }
        }

        private void ChoosePickerListItem(Picker picker, int index)
        {
            picker.SelectedIndex = index;
            GHApp.PlayButtonClickedSound();
            ClosePickerList();
        }

        private void ClosePickerList()
        {
            if (!PickerListGrid.IsVisible)
                return;
            PickerListGrid.IsVisible = false;
            _pickerListLayer.Clear();
            PickerListLayout.Children.Clear();
#if GNH_AVALONIA
            _pickerListPressedRow = null;
#endif
            _gamepadPickerLeftStickDisarmed = true;
            _gamepadPickerRightStickDisarmed = true;
        }

        /* A tap or click outside the list closes it, leaving the value as it was */
        private void PickerListBackdrop_Tapped(object sender, EventArgs e)
        {
            ClosePickerList();
        }

#if GNH_AVALONIA
        private const double PickerListTapSlopDips = 12.0;  /* a pointer that moves farther is a scroll */

        /* The list row a pointer went down on, and where in it */
        private View _pickerListPressedRow;
        private Point _pickerListPressPoint;

        /* Chooses when the pointer comes up over the row it went down on, within the slop, so that a touch that
           scrolls the list chooses nothing */
        private void AddPickerListRowPointer(View row, Action choose)
        {
            PointerGestureRecognizer pointer = new PointerGestureRecognizer();
            pointer.PointerPressed += (s, e) =>
            {
                Point? p = e.GetPosition(row);
                _pickerListPressedRow = p.HasValue ? row : null;
                _pickerListPressPoint = p ?? default(Point);
            };
            pointer.PointerReleased += (s, e) =>
            {
                Point? p = e.GetPosition(row);
                bool pressedHere = ReferenceEquals(_pickerListPressedRow, row);
                _pickerListPressedRow = null;
                if (!pressedHere || !p.HasValue)
                    return;
                double dx = p.Value.X - _pickerListPressPoint.X;
                double dy = p.Value.Y - _pickerListPressPoint.Y;
                if (dx * dx + dy * dy <= PickerListTapSlopDips * PickerListTapSlopDips)
                    choose();
            };
            row.GestureRecognizers.Add(pointer);
        }
#endif

        private void OpenPickerRadial(Picker picker, IList<SelectableShortcutButton> commands, string title)
        {
            List<GHRadialSourceItem> sources = new List<GHRadialSourceItem>(commands.Count);
            int i;
            for (i = 0; i < commands.Count; i++)
            {
                SelectableShortcutButton command = commands[i];
                GHRadialSourceItem source = new GHRadialSourceItem();
                source.Id = "picker:" + i.ToString();
                source.Label = command.Label;
                source.Description = string.Equals(command.Name, command.Label, StringComparison.Ordinal) ? null : command.Name;
                source.IconPath = string.IsNullOrEmpty(command.ImageSourcePath) ? null : "resource://" + command.ImageSourcePath;
                source.Command = i;     /* the picker's index */
                source.Page = -1;
                source.Column = -1;
                source.Row = -1;
                sources.Add(source);
            }

            int slots = GHRadialTuning.CommandPageSlotCount;
            int selected = picker.SelectedIndex >= 0 && picker.SelectedIndex < commands.Count ? picker.SelectedIndex : 0;
            GHRadialMenuDefinition def = new GHRadialMenuDefinition();
            def.MenuId = "PICK";
            def.OwningStick = 0;
            def.Pages.AddRange(GHRadialSlotAssigner.BuildPackedPages(sources, title, slots, 0));
            def.InitialPageIndex = selected / slots;
            string currentText = "Now: " + commands[selected].Name;
            def.NoHighlightStickText = currentText;
            def.NoHighlightPointerText = currentText;
            def.ConfirmHint = "Set";
            def.CancelHint = "Close";
            def.Style = GHGamepadService.Style;
            def.OnActivate = item =>
            {
                GHRadialSourceItem source = item == null ? null : item.Tag as GHRadialSourceItem;
                if (source != null)
                    picker.SelectedIndex = source.Command;
            };
            def.OnClosed = reason =>
            {
                _gamepadPickerLeftStickDisarmed = true;
                _gamepadPickerRightStickDisarmed = true;
            };

            EnsurePickerRadialRenderer();
            if (!_pickerRadialState.Open(def, Stopwatch.GetTimestamp()))
                return;
            _pickerRadialTouchId = -1;
            _pickerRadialCanvasShown = true;
            PickerRadialCanvas.IsVisible = true;
            PickerRadialCanvas.InputTransparent = false;
            GHGamepadService.RadialTickPage = this;
            GHApp.PlayMenuSelectSound();
            PickerRadialCanvas.InvalidateSurface();
        }

        private void EnsurePickerRadialRenderer()
        {
            if (_pickerRadialRenderer != null)
                return;
            GHRadialMenuRenderer renderer = new GHRadialMenuRenderer();
            renderer.IconLookup = p => GHApp.GetCachedImageSourceBitmap(p, true);
            renderer.TitleTypeface = GHApp.ImmortalTypeface;
            renderer.LabelTypeface = GHApp.LatoRegular;
            renderer.BoldTypeface = GHApp.LatoBold;
            renderer.DescriptionTypeface = GHApp.UnderwoodTypeface;
            renderer.UseTextBlobCaching = GHApp.UseTextBlobCaching;
            _pickerRadialRenderer = renderer;
        }

        /* True when the event stops here. An open radial takes everything; the press that reveals the controller only
           reveals. An open list leaves the event to the navigator. */
        private bool TryHandleGamepadPickerButton(GHGamepadButtonEvent e)
        {
            int b = (int)e.Button;
            bool known = b >= 0 && b < _gamepadPickerHeld.Length;
            bool ended = e.Kind == GHGamepadButtonEventKind.Released || e.Kind == GHGamepadButtonEventKind.ShortPressed;
            if (_pickerRadialState.IsOpen)
            {
                if (known)
                    _gamepadPickerHeld[b] = !ended;
                if (!e.RevealsUi)
                    ApplyPickerRadialButton(e);
                return true;
            }
            if (PickerListGrid.IsVisible)
            {
                if (known)
                    _gamepadPickerHeld[b] = !ended;
                return false;
            }
            if (_gamepadPickerLeftStickDisarmed && IsGamepadLeftStickButton(e.Button))
                return true;
            if (!known || !_gamepadPickerHeld[b])
                return false;
            if (e.Kind == GHGamepadButtonEventKind.Pressed)
            {
                _gamepadPickerHeld[b] = false;
                return false;
            }
            if (ended)
                _gamepadPickerHeld[b] = false;
            return true;
        }

        private static bool IsGamepadLeftStickButton(GHGamepadButton button)
        {
            return button == GHGamepadButton.LeftStickUp || button == GHGamepadButton.LeftStickDown
                || button == GHGamepadButton.LeftStickLeft || button == GHGamepadButton.LeftStickRight;
        }

        /* Acts on Pressed, and on Repeat for D-pad Left and Right, as the map's radials do */
        private void ApplyPickerRadialButton(GHGamepadButtonEvent e)
        {
            bool pressed = e.Kind == GHGamepadButtonEventKind.Pressed;
            bool repeat = e.Kind == GHGamepadButtonEventKind.Repeat;
            switch (e.Button)
            {
            case GHGamepadButton.A:
                if (pressed)
                    ApplyPickerRadialInput(GHRadialInput.Confirm);
                break;
            case GHGamepadButton.B:
                if (pressed)
                    ApplyPickerRadialInput(GHRadialInput.Cancel);
                break;
            case GHGamepadButton.LB:
                if (pressed)
                    ApplyPickerRadialInput(GHRadialInput.PrevPage);
                break;
            case GHGamepadButton.RB:
                if (pressed)
                    ApplyPickerRadialInput(GHRadialInput.NextPage);
                break;
            case GHGamepadButton.DPadLeft:
                if (pressed || repeat)
                    ApplyPickerRadialInput(GHRadialInput.StepCounterClockwise);
                break;
            case GHGamepadButton.DPadRight:
                if (pressed || repeat)
                    ApplyPickerRadialInput(GHRadialInput.StepClockwise);
                break;
            case GHGamepadButton.DPadUp:
                if (pressed)
                    ApplyPickerRadialInput(GHRadialInput.JumpNorth);
                break;
            case GHGamepadButton.DPadDown:
                if (pressed)
                    ApplyPickerRadialInput(GHRadialInput.JumpSouth);
                break;
            default:
                break;
            }
        }

        /* False when the sticks stop here */
        private bool UpdateGamepadPickerSticks(GHGamepadSticks sticks, double dtSeconds)
        {
            if (_pickerRadialState.IsOpen)
            {
                /* The stick pushed further selects */
                GHRadialApplyResult res = sticks.RightMagnitude > sticks.LeftMagnitude
                    ? _pickerRadialState.UpdateStick(sticks.RightX, sticks.RightY, dtSeconds)
                    : _pickerRadialState.UpdateStick(sticks.LeftX, sticks.LeftY, dtSeconds);
                PlayPickerRadialSounds(res, Stopwatch.GetTimestamp());
                return false;
            }
            if (_gamepadPickerLeftStickDisarmed && sticks.LeftMagnitude < GHConstants.GamepadStickReleaseMagnitude)
                _gamepadPickerLeftStickDisarmed = false;
            if (_gamepadPickerRightStickDisarmed && sticks.RightMagnitude < GHConstants.GamepadStickReleaseMagnitude)
                _gamepadPickerRightStickDisarmed = false;
            return !_gamepadPickerRightStickDisarmed;
        }

        /* Every Poll while GHGamepadService.RadialTickPage is this page */
        public void GamepadFrameTick(long nowTicks)
        {
            if (!_pickerRadialState.IsVisible)
            {
                HidePickerRadialCanvas();
                return;
            }
            if (_pickerRadialState.IsOpen && !GHGamepadService.IsWindowActive)
                _pickerRadialState.Close(GHRadialCloseReason.Deactivated, nowTicks, true);
            _pickerRadialState.Tick(nowTicks);
            if (!_pickerRadialState.IsVisible)
                HidePickerRadialCanvas();
            else if (_pickerRadialState.NeedsFrame)
            {
                // WORKAROUND(WA-AVALONIA-0021): An animation frame that commits nothing waits for a 16 ms timer
                PickerRadialCanvas.InvalidateSurface();
            }
        }

        /* Escape closes an open selector; other keys do nothing while one is open */
        private bool TryHandleGamepadPickerKey(GHSpecialKey key)
        {
            if (_pickerRadialState.IsOpen)
            {
                if (key == GHSpecialKey.Escape)
                    ApplyPickerRadialInput(GHRadialInput.Cancel);
                return true;
            }
            if (PickerListGrid.IsVisible)
            {
                if (key == GHSpecialKey.Escape)
                    ClosePickerList();
                return true;
            }
            return false;
        }

        /* From the page's Disappearing: no callbacks, and the next opening makes a new renderer */
        private void ShutDownGamepadPickerSelectors()
        {
            ClosePickerList();
            _pickerRadialState.Reset();
            Array.Clear(_gamepadPickerHeld, 0, _gamepadPickerHeld.Length);
            _gamepadPickerLeftStickDisarmed = false;
            _gamepadPickerRightStickDisarmed = false;
            HidePickerRadialCanvas();
            GHRadialMenuRenderer renderer = _pickerRadialRenderer;
            _pickerRadialRenderer = null;
            if (renderer != null)
                renderer.Dispose();
        }

        private void HidePickerRadialCanvas()
        {
            _pickerRadialTouchId = -1;
            if (ReferenceEquals(GHGamepadService.RadialTickPage, this))
                GHGamepadService.RadialTickPage = null;
            if (!_pickerRadialCanvasShown)
                return;
            _pickerRadialCanvasShown = false;
            PickerRadialCanvas.InputTransparent = true;
            PickerRadialCanvas.IsVisible = false;
        }

        /* Input stops reaching the canvas at the logical close; the canvas hides when the fade ends */
        private void SyncPickerRadialCanvas()
        {
            if (!_pickerRadialState.IsVisible)
                HidePickerRadialCanvas();
            else if (!_pickerRadialState.IsOpen && _pickerRadialCanvasShown)
                PickerRadialCanvas.InputTransparent = true;
        }

        private void ApplyPickerRadialInput(GHRadialInput input)
        {
            long now = Stopwatch.GetTimestamp();
            GHRadialApplyResult res;
            try
            {
                res = _pickerRadialState.Apply(input, now);
            }
            finally
            {
                SyncPickerRadialCanvas();
            }
            PlayPickerRadialSounds(res, now);
        }

        private void PlayPickerRadialSounds(GHRadialApplyResult res, long now)
        {
            if (res.Has(GHRadialEffects.Activated))
                GHApp.PlayButtonClickedSound();
            else if (res.Has(GHRadialEffects.PageChanged))
                GHApp.PlayMenuSelectSound();
            else if (res.Has(GHRadialEffects.HighlightChanged) && _pickerRadialState.TryClaimHighlightSound(now))
                GHApp.PlayRadialHighlightSound();
        }

        private float GetPickerRadialDipScale(int canvasPixelWidth)
        {
            double width = PickerRadialCanvas.ThreadSafeWidth;
            if (width > 0 && canvasPixelWidth > 0)
                return (float)(canvasPixelWidth / width);
            float density = GHApp.DisplayDensity;
            return density > 0 ? density : 1.0f;
        }

        private void PickerRadialCanvas_PaintSurface(object sender, SKPaintSurfaceEventArgs e)
        {
            SKCanvas canvas = e.Surface.Canvas;
            GHRadialMenuRenderer renderer = _pickerRadialRenderer;
            if (renderer == null)
            {
                canvas.Clear(SKColors.Transparent);
                return;
            }
            renderer.UseTextBlobCaching = GHApp.UseTextBlobCaching;
            renderer.Paint(canvas, e.Info, _pickerRadialState, GetPickerRadialDipScale(e.Info.Width));
        }

        /* A tap or left click activates a slot, pages on a chip or dot and closes on the hub or outside; a drag
           highlights under the finger and activates only when released on the ring; a right click closes */
        private void PickerRadialCanvas_Touch(object sender, SKTouchEventArgs e)
        {
            if (e == null || !_pickerRadialState.IsOpen)
            {
                _pickerRadialTouchId = -1;
                return;
            }
            if (e.ActionType == SKTouchAction.WheelChanged)
                return;
            e.Handled = true;
            long now = Stopwatch.GetTimestamp();
            float x = e.Location.X;
            float y = e.Location.Y;
            switch (e.ActionType)
            {
            case SKTouchAction.Pressed:
                if (e.MouseButton == SKMouseButton.Right)
                {
                    _pickerRadialTouchId = -1;
                    ApplyPickerRadialInput(GHRadialInput.Cancel);
                    break;
                }
                if (e.MouseButton == SKMouseButton.Middle)
                    break;
                _pickerRadialTouchId = e.Id;
                _pickerRadialTouchStart = e.Location;
                _pickerRadialTouchDragged = false;
                PlayPickerRadialSounds(_pickerRadialState.PointerMove(x, y), now);
                break;
            case SKTouchAction.Moved:
                if (e.Id != _pickerRadialTouchId || !e.InContact)
                    break;
                if (!_pickerRadialTouchDragged)
                {
                    float slop = PickerRadialTapSlopDips * GetPickerRadialDipScale((int)_pickerRadialState.Geometry.Width);
                    float dx = x - _pickerRadialTouchStart.X;
                    float dy = y - _pickerRadialTouchStart.Y;
                    if (dx * dx + dy * dy > slop * slop)
                        _pickerRadialTouchDragged = true;
                }
                if (_pickerRadialTouchDragged)
                    PlayPickerRadialSounds(_pickerRadialState.PointerMove(x, y), now);
                break;
            case SKTouchAction.Released:
                if (e.Id != _pickerRadialTouchId)
                    break;
                _pickerRadialTouchId = -1;
                GHRadialGeometry g = _pickerRadialState.Geometry;
                float r;
                double a;
                if (!_pickerRadialTouchDragged || (g.IsValid && g.HitTest(x, y, out r, out a) == GHRadialHitZone.Ring))
                {
                    GHRadialApplyResult res;
                    try
                    {
                        res = _pickerRadialState.PointerPress(x, y, now);
                    }
                    finally
                    {
                        SyncPickerRadialCanvas();
                    }
                    PlayPickerRadialSounds(res, now);
                }
                break;
            case SKTouchAction.Cancelled:
                if (e.Id == _pickerRadialTouchId)
                    _pickerRadialTouchId = -1;
                break;
            default:
                break;
            }
        }
    }
}
