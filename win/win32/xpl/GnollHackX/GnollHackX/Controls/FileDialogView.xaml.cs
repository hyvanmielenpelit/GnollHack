using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using GnollHackX;
using SkiaSharp;

#if GNH_MAUI
using Microsoft.Maui;
using Microsoft.Maui.Controls;
using Microsoft.Maui.Graphics;
using SkiaSharp.Views.Maui;

namespace GnollHackM
#else
using SkiaSharp.Views.Forms;
using Xamarin.Forms;

namespace GnollHackX.Controls
#endif
{
    /* A file dialog drawn inside the host page, for when the system's own cannot be used with a controller. It shows a
       GHFileDialogBehavior's rows and knows nothing about files. Created by GHFileDialogHost, which adds it to the
       page's root grid. A activates a row, X confirms, B goes up or cancels, Menu cancels. UI thread only. */
    public partial class FileDialogView : ContentView, IGHGamepadHandlingPage
    {
        private const double RowWidth = 620;
        private const double RowHeight = 56;
        private const double RowImageSize = 44;
        private const float GamepadHintFontSizeDips = 13f;
        private const int GamepadHintMinimumCount = 2;
        private const int LayoutWaitMilliseconds = 40;
        private const int MaxLayoutWaits = 10;

        private readonly GHFileDialogBehavior _behavior;
        private readonly GHGamepadPageNavigator _navigator = new GHGamepadPageNavigator();
        private readonly GHGamepadPageNavigator.Layer _statusLayer;
        private readonly List<RowImageButton> _rowButtons = new List<RowImageButton>();
        private readonly Dictionary<RowImageButton, GHFileRow> _rowsByButton = new Dictionary<RowImageButton, GHFileRow>();
        /* The paths of the rows entered, for putting the focus back on going up */
        private readonly Stack<string> _enteredPaths = new Stack<string>();
        private readonly List<GHGamepadHint> _hints = new List<GHGamepadHint>();
        private readonly List<GHGamepadHint> _paintHints = new List<GHGamepadHint>();
        private GHGamepadHintRenderer _hintRenderer;
        private IList<GHFileRow> _shownRows;
        private string _focusPathAfterRebuild;
        private bool _isOpen = false;
        private bool _refreshPending = false;

        public FileDialogView(GHFileDialogBehavior behavior)
        {
            InitializeComponent();
            _behavior = behavior;
            TitleLabel.Text = behavior.Title;
            ConfirmButton.Text = behavior.ConfirmText ?? "";
            ConfirmButton.IsVisible = behavior.ConfirmText != null;
            if (GHApp.DarkMode)
            {
                PanelFrame.BackgroundColor = GHColors.MsgBoxDarkModeBkgColor;
                StatusFrame.BackgroundColor = GHColors.MsgBoxDarkModeBkgColor;
            }

            _navigator.IsBusy = () => _behavior.IsBusy;
            GHGamepadPageNavigator.Layer main = _navigator.Main;
            main.ScrollView = RowsScrollView;
            main.ScrollsContent = true;
            main.ActsOnVisibleFocusOnly = true;
            main.Back = () =>
            {
                GoBack();
                return true;
            };
            _statusLayer = _navigator.AddOverlay(() => StatusGrid.IsVisible);
            _statusLayer.AddButton(StatusOkButton);
            _statusLayer.Back = () =>
            {
                _behavior.AcknowledgeStatus();
                return true;
            };
        }

        /* After the view is in the page's tree, because a focus entry's availability is read through its ancestors */
        public void Open()
        {
            if (_isOpen)
                return;
            _isOpen = true;
            _behavior.Changed += Behavior_Changed;
            GHGamepadService.StateChanged += GamepadService_StateChanged;
            _navigator.Attach();
            RefreshFromBehavior();
        }

        public void Close()
        {
            if (!_isOpen)
                return;
            _isOpen = false;
            _behavior.Changed -= Behavior_Changed;
            GHGamepadService.StateChanged -= GamepadService_StateChanged;
            _navigator.Detach();
            ClearRows();
            GamepadHintCanvas.IsVisible = false;
            if (_hintRenderer != null)
            {
                _hintRenderer.Dispose();
                _hintRenderer = null;
            }
        }

        /* Escape from a keyboard, as B */
        public void GoBack()
        {
            if (!_isOpen || _behavior.IsBusy)
                return;
            if (_behavior.StatusMessage != null)
            {
                _behavior.AcknowledgeStatus();
            }
            else if (_behavior.CanGoUp)
            {
                if (_enteredPaths.Count > 0)
                    _focusPathAfterRebuild = _enteredPaths.Pop();
                GHApp.PlayButtonClickedSound();
                ObserveAsync(_behavior.GoUpAsync());
            }
            else
            {
                GHApp.PlayButtonClickedSound();
                _behavior.Cancel();
            }
        }

        public bool HandleGamepadButton(GHGamepadButtonEvent e)
        {
            if (!_isOpen)
                return true;
            if (e.Button == GHGamepadButton.Menu && e.Kind == GHGamepadButtonEventKind.Pressed && !e.RevealsUi)
            {
                _behavior.Cancel();
                return true;
            }
            return _navigator.HandleButton(e);
        }

        public void UpdateGamepadSticks(GHGamepadSticks sticks, double dtSeconds)
        {
            if (_isOpen)
                _navigator.UpdateSticks(sticks, dtSeconds);
        }

        /* A behavior can change from inside a button's handling, so the view follows on a later turn of the UI thread */
        private void Behavior_Changed()
        {
            if (_refreshPending)
                return;
            _refreshPending = true;
            Action refresh = () =>
            {
                _refreshPending = false;
                try
                {
                    if (_isOpen)
                        RefreshFromBehavior();
                }
                catch (Exception ex)
                {
                    GHApp.MaybeWriteGHLog("FileDialogView: refreshing failed: " + ex.Message);
                }
            };
#if GNH_MAUI
            if (Dispatcher == null || !Dispatcher.Dispatch(refresh))
                refresh();
#else
            Device.BeginInvokeOnMainThread(refresh);
#endif
        }

        private void GamepadService_StateChanged()
        {
            RefreshHints();
        }

        private void RefreshFromBehavior()
        {
            SubtitleLabel.Text = _behavior.Subtitle ?? "";
            string notice = _behavior.IsBusy ? "Saving..." : _behavior.Notice;
            NoticeLabel.Text = notice ?? "";
            NoticeLabel.IsVisible = !string.IsNullOrEmpty(notice);
            ConfirmButton.IsEnabled = _behavior.CanConfirm && !_behavior.IsBusy;
            CancelButton.IsEnabled = !_behavior.IsBusy;

            IList<GHFileRow> rows = _behavior.Rows;
            bool focusAwaitsLayout = false;
            if (ReferenceEquals(rows, _shownRows))
                UpdateRowVisuals();
            else
                focusAwaitsLayout = RebuildRows(rows);

            string status = _behavior.StatusMessage;
            bool showStatus = status != null;
            StatusLabel.Text = status ?? "";
            if (StatusGrid.IsVisible != showStatus)
                StatusGrid.IsVisible = showStatus;
            /* Without a layout a restored focus would read as scrolled out of view, and move */
            if (focusAwaitsLayout)
                _navigator.Main.Focus.RefreshVisuals();
            else
                _navigator.RefreshVisuals();
            RefreshHints();
        }

        /* True when the focus went back to a row that is scrolled to once it has a layout */
        private bool RebuildRows(IList<GHFileRow> rows)
        {
            ClearRows();
            _shownRows = rows;
            GHGamepadPageNavigator.Layer main = _navigator.Main;
            main.Clear();
            GHGamepadFocusList.EntryOptions rowOptions = new GHGamepadFocusList.EntryOptions();
            rowOptions.Secondary = () =>
            {
                if (_behavior.CanConfirm)
                    Confirm();
            };

            RowImageButton focusButton = null;
            foreach (GHFileRow row in rows)
            {
                if (row.Kind == GHFileRowKind.Message)
                {
                    Label message = new Label();
                    message.Text = row.Name;
                    message.FontFamily = "LatoRegular";
                    message.FontSize = 15;
                    message.TextColor = GHColors.White;
                    message.HorizontalOptions = LayoutOptions.Center;
                    RowsLayout.Children.Add(message);
                    /* The focus rests here, so that a repeated A does not reach Confirm or Cancel */
                    main.Add(message, null, () => true, null, rowOptions);
                    continue;
                }

                RowImageButton button = CreateRowButton(row);
                button.BtnClicked += RowButton_Clicked;
                _rowButtons.Add(button);
                _rowsByButton[button] = row;
                RowsLayout.Children.Add(button);
                main.Add(button, button.PerformClick, null, null, rowOptions);
                if (_focusPathAfterRebuild != null && row.Path == _focusPathAfterRebuild)
                    focusButton = button;
            }
            main.Add(ConfirmButton, ConfirmButton.PerformClick, null, null, rowOptions);
            main.Add(CancelButton, CancelButton.PerformClick, null, null, rowOptions);
            UpdateRowVisuals();

            _focusPathAfterRebuild = null;
            if (focusButton != null && main.Focus.SetFocus(focusButton))
            {
                ScrollToAfterLayout(focusButton, rows);
                return true;
            }
            main.FocusInitial();
            _ = RowsScrollView.ScrollToAsync(0, 0, false);
            return false;
        }

        private async void ScrollToAfterLayout(RowImageButton button, IList<GHFileRow> rows)
        {
            try
            {
                int waits = 0;
                do
                {
                    await Task.Delay(LayoutWaitMilliseconds);
                    waits++;
                }
                while (button.Height <= 0 && waits < MaxLayoutWaits);
                if (_isOpen && ReferenceEquals(rows, _shownRows))
                    GHGamepadFocusList.BringIntoView(RowsScrollView, button, ScrollToPosition.MakeVisible);
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("FileDialogView: scrolling to a row failed: " + ex.Message);
            }
        }

        private RowImageButton CreateRowButton(GHFileRow row)
        {
            RowImageButton button = new RowImageButton();
            button.ImgSourcePath = "resource://" + GHApp.AppResourceName + ".Assets.UI." + (row.Kind == GHFileRowKind.File ? "manual.png" : "load.png");
            button.OmitLabelFromBreadcrumb = true;
            button.ImgHighFilterQuality = true;
            button.ImgWidth = RowImageSize;
            button.ImgHeight = RowImageSize;
            button.LblFontFamily = "LatoRegular";
            button.LblFontSize = 16;
            button.SubLblFontFamily = "LatoRegular";
            button.SubLblFontSize = 12;
            button.SubLblTextColor = GHColors.White;
            button.Padding = new Thickness(4);
            button.GridWidth = RowWidth;
            button.GridHeight = RowHeight;
#if GNH_MAUI
            button.MaximumWidthRequest = RowWidth;
            button.MaximumHeightRequest = RowHeight;
#else
            button.WidthRequest = RowWidth;
            button.HeightRequest = RowHeight;
#endif
            return button;
        }

        /* The rows' texts; a marked file is drawn green and says so */
        private void UpdateRowVisuals()
        {
            foreach (RowImageButton button in _rowButtons)
            {
                GHFileRow row;
                if (!_rowsByButton.TryGetValue(button, out row))
                    continue;
                button.LblText = row.Name;
                button.LblTextColor = row.IsMarked ? GHColors.LightGreen : GHColors.White;
                string detail = row.IsMarked ? (string.IsNullOrEmpty(row.Detail) ? "Selected" : "Selected · " + row.Detail) : row.Detail;
                button.SubLblText = detail ?? "";
                button.IsSubLblVisible = !string.IsNullOrEmpty(detail);
            }
        }

        private void ClearRows()
        {
            foreach (RowImageButton button in _rowButtons)
                button.BtnClicked -= RowButton_Clicked;
            _rowButtons.Clear();
            _rowsByButton.Clear();
            RowsLayout.Children.Clear();
            _shownRows = null;
        }

        private void RowButton_Clicked(object sender, EventArgs e)
        {
            RowImageButton button = sender as RowImageButton;
            GHFileRow row;
            if (button == null || !_rowsByButton.TryGetValue(button, out row) || !_behavior.CanActivate)
                return;
            GHApp.PlayButtonClickedSound();
            if (row.Kind != GHFileRowKind.File)
                _enteredPaths.Push(row.Path);
            ObserveAsync(_behavior.ActivateRowAsync(row));
        }

        private void Confirm()
        {
            if (_behavior.IsBusy || !_behavior.CanConfirm)
                return;
            GHApp.PlayButtonClickedSound();
            _behavior.Confirm();
        }

        private void ConfirmButton_Clicked(object sender, EventArgs e)
        {
            Confirm();
        }

        private void CancelButton_Clicked(object sender, EventArgs e)
        {
            GHApp.PlayButtonClickedSound();
            _behavior.Cancel();
        }

        private void StatusOkButton_Clicked(object sender, EventArgs e)
        {
            GHApp.PlayButtonClickedSound();
            _behavior.AcknowledgeStatus();
        }

        /* Behaviors report failures through their state; anything else would otherwise go unobserved */
        private static async void ObserveAsync(Task task)
        {
            try
            {
                await task;
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("FileDialogView: " + ex.Message);
            }
        }

        private void RefreshHints()
        {
            _hints.Clear();
            if (!_isOpen)
                return;
            if (_behavior.StatusMessage != null)
            {
                _hints.Add(new GHGamepadHint(GHGamepadButton.A, "OK", false));
            }
            else if (!_behavior.IsBusy)
            {
                _hints.Add(new GHGamepadHint(GHGamepadButton.A, "Select", false));
                if (_behavior.ConfirmText != null && _behavior.CanConfirm)
                    _hints.Add(new GHGamepadHint(GHGamepadButton.X, _behavior.ConfirmText, false));
                _hints.Add(new GHGamepadHint(GHGamepadButton.B, _behavior.CanGoUp ? "Back" : "Cancel", false));
            }
            bool show = GHGamepadService.IsActive;
            if (GamepadHintCanvas.IsVisible != show)
                GamepadHintCanvas.IsVisible = show;
            GamepadHintCanvas.InvalidateSurface();
        }

        private GHGamepadHintRenderer EnsureHintRenderer()
        {
            if (_hintRenderer == null)
            {
                GHGamepadHintRenderer renderer = new GHGamepadHintRenderer();
                renderer.LabelTypeface = GHApp.LatoRegular;
                renderer.BoldTypeface = GHApp.LatoBold;
                renderer.UseTextBlobCaching = GHApp.UseTextBlobCaching;
                _hintRenderer = renderer;
            }
            return _hintRenderer;
        }

        /* One line centered in the canvas, dropping the last hints while the row needs two lines */
        private void GamepadHintCanvas_PaintSurface(object sender, SKPaintSurfaceEventArgs e)
        {
            SKCanvas canvas = e.Surface.Canvas;
            canvas.Clear(SKColors.Transparent);
            if (!_isOpen || _hints.Count == 0)
                return;
            GHGamepadHintRenderer renderer = EnsureHintRenderer();
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
            float maxWidth = pixelWidth - 2f * fontSize * GHGamepadHintRenderer.BackingPadOfFont;
            GHGamepadStyle style = GHGamepadService.Style;
            _paintHints.Clear();
            _paintHints.AddRange(_hints);
            renderer.LayoutRow(_paintHints, style, fontSize, maxWidth);
            while (renderer.LineCount > 1 && _paintHints.Count > GamepadHintMinimumCount)
            {
                _paintHints.RemoveAt(_paintHints.Count - 1);
                renderer.LayoutRow(_paintHints, style, fontSize, maxWidth);
            }
            if (renderer.LineCount == 0)
                return;

            float bottomY = pixelHeight * 0.5f + renderer.RowHeight * 0.5f;
            renderer.DrawRow(canvas, _paintHints, style, pixelWidth * 0.5f, bottomY, 1.0f, true);
        }
    }
}
