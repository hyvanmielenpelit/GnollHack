using GnollHackCommon;
using SkiaSharp;
using SkiaSharp.Views.Forms;
using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace GnollHackClient.Pages.Game
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class GHMenuCanvasPage : ContentPage
    {
        private GamePage _gamePage;
        public GHMenuCanvasPage(GamePage gamePage, GHMenuInfo menuinfo, GHWindow ghwindow)
        {
            InitializeComponent();
            _gamePage = gamePage;

            lock (_menuHideCancelledLock)
            {
                _menuHideCancelled = true;
            }

            lock (_gamePage.RefreshScreenLock)
            {
                _gamePage.RefreshScreen = false;
            }

            lock (_menuDrawOnlyLock)
            {
                _menuDrawOnlyClear = true;
                _menuRefresh = false;
            }

            MenuTouchDictionary.Clear();

            MenuWindowGlyphImage.Source = null;

            _menuGlyphImageSource.ReferenceGamePage = _gamePage;
            _menuGlyphImageSource.AutoSize = true;
            _menuGlyphImageSource.ObjData = ghwindow.ObjData;
            _menuGlyphImageSource.Glyph = ghwindow.Glyph;

            MenuWindowGlyphImage.Source = MenuGlyphImage;
            MenuWindowGlyphImage.IsVisible = IsMenuGlyphVisible;

            //canvasView.GHWindow = ghwindow;
            //canvasView.MenuStyle = menuinfo.Style;
            //canvasView.SelectionHow = menuinfo.SelectionHow;
            MenuCanvas.GHWindow = ghwindow;
            MenuCanvas.MenuStyle = menuinfo.Style;
            MenuCanvas.SelectionHow = menuinfo.SelectionHow;
            MenuCanvas.SelectionIndex = -1;
            if (MenuCanvas.SelectionHow == SelectionMode.Single)
            {
                int idx = -1;
                foreach (GHMenuItem mi in menuinfo.MenuItems)
                {
                    idx++;
                    if (mi.Selected)
                    {
                        mi.Selected = false; /* Clear out, with single selection we are using SelectionIndex */
                        MenuCanvas.SelectionIndex = idx;
                    }
                }
            }

            if (menuinfo.Header == null)
            {
                MenuHeaderLabel.IsVisible = false;
                MenuHeaderLabel.Text = "";

            }
            else
            {
                MenuHeaderLabel.IsVisible = true;
                MenuHeaderLabel.Text = menuinfo.Header;
                MenuHeaderLabel.FontFamily = ClientUtils.MenuHeaderFontFamily(MenuCanvas.MenuStyle);
                MenuHeaderLabel.FontSize = ClientUtils.MenuHeaderFontSize(MenuCanvas.MenuStyle);
            }

            if (menuinfo.Subtitle == null)
            {
                MenuSubtitleLabel.IsVisible = false;
                MenuSubtitleLabel.Text = "";
            }
            else
            {
                MenuSubtitleLabel.IsVisible = true;
                MenuSubtitleLabel.Text = menuinfo.Subtitle;
                MenuSubtitleLabel.FontFamily = ClientUtils.MenuSubtitleFontFamily(MenuCanvas.MenuStyle);
                MenuSubtitleLabel.FontSize = ClientUtils.MenuSubtitleFontSize(MenuCanvas.MenuStyle);
            }

            ObservableCollection<GHMenuItem> newmis = new ObservableCollection<GHMenuItem>();
            if (menuinfo != null)
            {
                foreach (GHMenuItem mi in menuinfo.MenuItems)
                {
                    newmis.Add(mi);
                }
            }

            //canvasView.MenuItems = newmis;
            lock (MenuCanvas.MenuItemLock)
            {
                MenuCanvas.MenuItems = newmis;
            }

            lock (_menuDrawOnlyLock)
            {
                _menuDrawOnlyClear = false;
                _menuRefresh = true;
            }

            Device.StartTimer(TimeSpan.FromSeconds(1f / 40), () =>
            {
                if(_menuRefresh)
                    MenuCanvas.InvalidateSurface();
                return true;
            });

        }


        private GlyphImageSource _menuGlyphImageSource = new GlyphImageSource();

        public ImageSource MenuGlyphImage
        {
            get
            {
                return _menuGlyphImageSource;
            }
        }

        public bool IsMenuGlyphVisible
        {
            get
            {
                return (Math.Abs(_menuGlyphImageSource.Glyph) > 0 && _menuGlyphImageSource.Glyph != _gamePage.NoGlyph);
            }
        }

        private object _menuDrawOnlyLock = new object();
        private bool _menuDrawOnlyClear = false;
        private bool _menuRefresh = true;

        private SKColor _suffixTextColor = new SKColor(220, 220, 220);
        private SKColor _menuHighlightColor = new SKColor(0xFF, 0x88, 0x00, 0x88);
        private int _firstDrawnMenuItemIdx = -1;
        private int _lastDrawnMenuItemIdx = -1;
        private float _totalMenuHeight = 0;
        private void MenuCanvas_PaintSurface(object sender, SKPaintSurfaceEventArgs e)
        {
            //DebugWriteProfilingStopwatchTime("Draw Menu Canvas Start");

            SKImageInfo info = e.Info;
            SKSurface surface = e.Surface;
            SKCanvas canvas = surface.Canvas;
            GHCanvasView referenceCanvasView = MenuCanvas;
            float canvaswidth = referenceCanvasView.CanvasSize.Width;
            float canvasheight = referenceCanvasView.CanvasSize.Height;
            float x = 0, y = 0;
            string str;
            SKRect textBounds = new SKRect();
            float scale = canvaswidth / (float)referenceCanvasView.Width;

            canvas.Clear();
            lock (_menuDrawOnlyLock)
            {
                if (_menuDrawOnlyClear)
                    return;
            }

            lock (MenuCanvas.MenuItemLock)
            {
                if (referenceCanvasView.MenuItems == null)
                    return;
            }

            using (SKPaint textPaint = new SKPaint())
            {
                textPaint.Typeface = App.UnderwoodTypeface;
                textPaint.TextSize = 30 * scale;
                float minrowheight = textPaint.FontSpacing;
                float picturewidth = 64.0f * minrowheight / 48.0f;
                float picturepadding = 9 * scale;
                float leftinnerpadding = 5;
                float curmenuoffset = 0;
                lock (MenuScrollLock)
                {
                    curmenuoffset = _menuScrollOffset;
                }
                y += curmenuoffset;
                double menuwidth = Math.Max(1, Math.Min(MenuCanvas.Width, ClientUtils.MenuViewWidthRequest(referenceCanvasView.MenuStyle)));
                float menuwidthoncanvas = (float)(menuwidth * scale);
                float leftmenupadding = Math.Max(0, (canvaswidth - menuwidthoncanvas) / 2);
                float rightmenupadding = leftmenupadding;
                float accel_fixed_width = 10;
                bool first = true;
                int idx = -1;
                float bottomPadding = 0;
                float topPadding = 0;
                float maintext_x_start = 0;

                lock (MenuCanvas.MenuItemLock)
                {
                    bool has_pictures = false;
                    bool has_identifiers = false;
                    _firstDrawnMenuItemIdx = -1;
                    _lastDrawnMenuItemIdx = -1;
                    foreach (GHMenuItem mi in referenceCanvasView.MenuItems)
                    {
                        if (mi.Identifier != 0)
                            has_identifiers = true;

                        if (mi.IsGlyphVisible)
                            has_pictures = true;

                        if (has_identifiers && has_pictures)
                            break;
                    }

                    foreach (GHMenuItem mi in referenceCanvasView.MenuItems)
                    {
                        idx++;
                        x = leftmenupadding;
                        mi.DrawBounds.Left = x;
                        textPaint.Typeface = App.GetTypefaceByName(mi.FontFamily);
                        textPaint.TextSize = (float)mi.FontSize * scale;
                        textPaint.TextAlign = SKTextAlign.Left;

                        /* Bottom Padding */
                        if (((ulong)mi.MenuFlags & (ulong)MenuFlags.IsHeading) != 0)
                        {
                            bottomPadding = 3 * scale;
                        }
                        else
                        {
                            bottomPadding = 3 * scale;
                        }

                        /* Top Padding */
                        if (((ulong)mi.MenuFlags & (ulong)MenuFlags.IsHeading) != 0)
                        {
                            topPadding = 12 * scale;
                        }
                        else
                        {
                            topPadding = 3 * scale;
                        }


                        mi.DrawBounds.Top = y;
                        //if (mi.DrawBounds.Top >= canvasheight)
                        //    break;

                        if (first)
                        {
                            accel_fixed_width = textPaint.FontMetrics.AverageCharacterWidth + 3 * textPaint.MeasureText(" ");
                            _firstDrawnMenuItemIdx = idx;
                            maintext_x_start = leftmenupadding + leftinnerpadding + (has_identifiers ? accel_fixed_width : 0) + (has_pictures ? picturepadding + picturewidth + picturepadding : textPaint.FontMetrics.AverageCharacterWidth);
                            first = false;
                        }

                        int maintextrows = 1;
                        string[] split = mi.MainText.Split(' ');
                        float maintextspace = canvaswidth - rightmenupadding - maintext_x_start;
                        float calc_x_start = maintext_x_start;
                        int rowidx = -1;
                        foreach (string s in split)
                        {
                            bool nowrap = false;
                            if (string.IsNullOrWhiteSpace(s))
                                nowrap = true;
                            rowidx++;
                            string added_split_str = s + " ";
                            float printlength = textPaint.MeasureText(added_split_str);
                            float endposition = calc_x_start + printlength;
                            bool pastend = endposition > canvaswidth - rightmenupadding;
                            if (pastend && rowidx > 0 & !nowrap)
                            {
                                maintextrows++;
                                calc_x_start = maintext_x_start;
                                rowidx = -1;
                            }
                            else
                            {
                                calc_x_start = endposition;
                            }
                        }

                        float generallinepadding = Math.Max(0.0f, (minrowheight - (textPaint.FontSpacing) * ((float)maintextrows + (mi.IsSuffixTextVisible ? 0.8f : 0.0f))) / 2);

                        bool isselected = referenceCanvasView.SelectionHow == SelectionMode.Multiple ? mi.Selected :
                            referenceCanvasView.SelectionHow == SelectionMode.Single ? idx == referenceCanvasView.SelectionIndex : false;

                        float totalRowHeight = topPadding + bottomPadding + ((float)maintextrows + (mi.IsSuffixTextVisible ? 0.8f : 0.0f)) * (textPaint.FontSpacing) + 2 * generallinepadding;
                        float totalRowWidth = canvaswidth - leftmenupadding - rightmenupadding;

                        /* Selection rectangle */
                        if (isselected && !(y + totalRowHeight <= 0 || y >= canvasheight))
                        {
                            SKRect fillrect = new SKRect(x, y, x + totalRowWidth, y + totalRowHeight);
                            textPaint.Color = _menuHighlightColor;
                            textPaint.Style = SKPaintStyle.Fill;
                            canvas.DrawRect(fillrect, textPaint);
                        }

                        float singlelinepadding = Math.Max(0.0f, ((float)(maintextrows - 1) * (textPaint.FontSpacing)) / 2);
                        y += topPadding;
                        y += generallinepadding;
                        y -= textPaint.FontMetrics.Ascent;
                        x += leftinnerpadding;

                        if (has_identifiers)
                        {
                            str = mi.FormattedAccelerator;
                            textPaint.Color = SKColors.Gray;
                            if (!(y + singlelinepadding + textPaint.FontSpacing + textPaint.FontMetrics.Ascent <= 0 || y + singlelinepadding + textPaint.FontMetrics.Ascent >= canvasheight))
                                canvas.DrawText(str, x, y + singlelinepadding, textPaint);
                            x += accel_fixed_width;
                        }

                        if (has_pictures)
                        {
                            x += picturepadding;

                            /* Icon */
                            float glyph_start_y = mi.DrawBounds.Top + Math.Max(0, (totalRowHeight - minrowheight) / 2);
                            if (mi.IsGlyphVisible && !(glyph_start_y + minrowheight <= 0 || glyph_start_y >= canvasheight))
                            {
                                using (new SKAutoCanvasRestore(canvas, true))
                                {
                                    canvas.Translate(x, glyph_start_y);
                                    mi.GlyphImageSource.AutoSize = true;
                                    mi.GlyphImageSource.DoAutoSize();
                                    if (mi.GlyphImageSource.Height > 0)
                                        canvas.Scale(minrowheight / mi.GlyphImageSource.Height);
                                    mi.GlyphImageSource.DrawOnCanvas(canvas);
                                }
                            }
                            x += picturewidth + picturepadding;
                        }
                        else
                        {
                            x += textPaint.FontMetrics.AverageCharacterWidth;
                        }

                        /* Main text */
                        textPaint.Color = ClientUtils.NHColor2SKColor((nhcolor)mi.NHColor);
                        int split_idx_on_row = -1;
                        float start_x = x;
                        foreach (string split_str in split)
                        {
                            bool nowrap = false;
                            if (string.IsNullOrWhiteSpace(split_str))
                                nowrap = true;
                            split_idx_on_row++;
                            string added_split_str = split_str + " ";
                            float printlength = textPaint.MeasureText(added_split_str);
                            float endposition = x + printlength;
                            bool pastend = endposition > canvaswidth - rightmenupadding;
                            if (pastend && split_idx_on_row > 0 && !nowrap)
                            {
                                x = start_x;
                                y += textPaint.FontSpacing;
                                split_idx_on_row = 0;
                                endposition = x + printlength;
                            }

                            if (!(y + textPaint.FontSpacing + textPaint.FontMetrics.Ascent <= 0 || y + textPaint.FontMetrics.Ascent >= canvasheight))
                                canvas.DrawText(added_split_str, x, y, textPaint);

                            x = endposition;
                        }
                        y += textPaint.FontSpacing + textPaint.FontMetrics.Ascent;
                        x = start_x;

                        /* Suffix text */
                        if (mi.IsSuffixTextVisible)
                        {
                            textPaint.Color = _suffixTextColor;
                            textPaint.TextSize = 0.8f * textPaint.TextSize;
                            y -= textPaint.FontMetrics.Ascent;
                            if (!(y + textPaint.FontSpacing + textPaint.FontMetrics.Ascent <= 0 || y + textPaint.FontMetrics.Ascent >= canvasheight))
                                canvas.DrawText(mi.SuffixText, x, y, textPaint);
                            y += textPaint.FontSpacing + textPaint.FontMetrics.Ascent;
                        }

                        y += generallinepadding;

                        y += bottomPadding;
                        mi.DrawBounds.Bottom = y;
                        mi.DrawBounds.Right = canvaswidth - rightmenupadding;
                        _lastDrawnMenuItemIdx = idx;

                        /* Count circle */
                        if (mi.Count > 0 && !(mi.DrawBounds.Bottom <= 0 || mi.DrawBounds.Top >= canvasheight))
                        {
                            float circleradius = mi.DrawBounds.Height * 0.90f / 2;
                            float circlex = mi.DrawBounds.Right - circleradius - 5;
                            float circley = (mi.DrawBounds.Top + mi.DrawBounds.Bottom) / 2;
                            textPaint.Color = SKColors.Red;
                            canvas.DrawCircle(circlex, circley, circleradius, textPaint);
                            textPaint.TextAlign = SKTextAlign.Center;
                            textPaint.Color = SKColors.White;
                            str = mi.Count.ToString();
                            float maxsize = 1.0f * 2.0f * circleradius / (float)Math.Sqrt(2);
                            textPaint.TextSize = (float)mi.FontSize * scale;
                            textPaint.MeasureText(str, ref textBounds);
                            float scalex = textBounds.Width / maxsize;
                            float scaley = textBounds.Height / maxsize;
                            float totscale = Math.Max(scalex, scaley);
                            textPaint.TextSize = textPaint.TextSize / Math.Max(1.0f, totscale);
                            canvas.DrawText(str, circlex, circley - (textPaint.FontSpacing) / 2 - textPaint.FontMetrics.Ascent, textPaint);
                        }
                    }
                    _totalMenuHeight = y - curmenuoffset;
                }
            }

            //DebugWriteProfilingStopwatchTime("Draw Menu Canvas End");
        }

        private object MenuScrollLock = new object();
        private float _menuScrollOffset = 0;

        private Dictionary<long, TouchEntry> MenuTouchDictionary = new Dictionary<long, TouchEntry>();
        private object _savedMenuSender = null;
        private SKTouchEventArgs _savedMenuEventArgs = null;
        private DateTime _savedMenuTimeStamp;
        private bool _menuTouchMoved = false;
        private void MenuCanvas_Touch(object sender, SKTouchEventArgs e)
        {
            lock (_menuDrawOnlyLock)
            {
                if (_menuDrawOnlyClear)
                    return;
            }

            switch (e?.ActionType)
            {
                case SKTouchAction.Entered:
                    break;
                case SKTouchAction.Pressed:
                    _savedMenuSender = null;
                    _savedMenuEventArgs = null;
                    _savedMenuTimeStamp = DateTime.Now;

                    if (MenuTouchDictionary.ContainsKey(e.Id))
                        MenuTouchDictionary[e.Id] = new TouchEntry(e.Location, DateTime.Now);
                    else
                        MenuTouchDictionary.Add(e.Id, new TouchEntry(e.Location, DateTime.Now));

                    if (MenuTouchDictionary.Count > 1)
                        _menuTouchMoved = true;
                    else
                    {
                        _savedMenuSender = sender;
                        _savedMenuEventArgs = e;

                        Device.StartTimer(TimeSpan.FromSeconds(GHConstants.LongMenuTapThreshold), () =>
                        {
                            if (_savedMenuSender == null || _savedMenuEventArgs == null)
                                return false;

                            if (_savedMenuSender == null || _savedMenuEventArgs == null)
                                return false;
                            DateTime curtime = DateTime.Now;
                            if (curtime - _savedMenuTimeStamp < TimeSpan.FromSeconds(GHConstants.LongMenuTapThreshold * 0.8))
                                return false; /* Changed touch position */

                            MenuCanvas_LongTap(_savedMenuSender, _savedMenuEventArgs);
                            return false;
                        });
                    }

                    e.Handled = true;
                    break;
                case SKTouchAction.Moved:
                    {
                        TouchEntry entry;
                        bool res = MenuTouchDictionary.TryGetValue(e.Id, out entry);
                        if (res)
                        {
                            SKPoint anchor = entry.Location;

                            float diffX = e.Location.X - anchor.X;
                            float diffY = e.Location.Y - anchor.Y;
                            float dist = (float)Math.Sqrt((Math.Pow(diffX, 2) + Math.Pow(diffY, 2)));

                            if (MenuTouchDictionary.Count == 1)
                            {
                                if ((dist > 25 ||
                                    (DateTime.Now.Ticks - entry.PressTime.Ticks) / TimeSpan.TicksPerMillisecond > GHConstants.MoveOrPressTimeThreshold
                                       ))
                                {
                                    /* Just one finger => Move the map */
                                    if (diffX != 0 || diffY != 0)
                                    {
                                        lock (MenuScrollLock)
                                        {
                                            _menuScrollOffset += diffY;
                                            if (_menuScrollOffset > 0)
                                                _menuScrollOffset = 0;
                                            else if (_menuScrollOffset < MenuCanvas.CanvasSize.Height - _totalMenuHeight)
                                                _menuScrollOffset = Math.Min(0, MenuCanvas.CanvasSize.Height - _totalMenuHeight);
                                        }
                                        MenuTouchDictionary[e.Id].Location = e.Location;
                                        _menuTouchMoved = true;
                                        _savedMenuTimeStamp = DateTime.Now;
                                    }
                                }
                            }
                        }
                        e.Handled = true;
                    }
                    break;
                case SKTouchAction.Released:
                    {
                        _savedMenuSender = null;
                        _savedMenuEventArgs = null;
                        _savedMenuTimeStamp = DateTime.Now;

                        TouchEntry entry;
                        bool res = MenuTouchDictionary.TryGetValue(e.Id, out entry);
                        long elapsedms = (DateTime.Now.Ticks - entry.PressTime.Ticks) / TimeSpan.TicksPerMillisecond;

                        if (elapsedms <= GHConstants.MoveOrPressTimeThreshold && !_menuTouchMoved && MenuCanvas.SelectionHow != SelectionMode.None)
                        {
                            MenuCanvas_NormalClickRelease(sender, e);
                        }
                        if (MenuTouchDictionary.ContainsKey(e.Id))
                            MenuTouchDictionary.Remove(e.Id);
                        if (MenuTouchDictionary.Count == 0)
                            _menuTouchMoved = false;
                        e.Handled = true;
                    }
                    break;
                case SKTouchAction.Cancelled:
                    break;
                case SKTouchAction.Exited:
                    break;
                case SKTouchAction.WheelChanged:
                    break;
                default:
                    break;
            }
        }

        private GHMenuItem _countMenuItem = null;
        List<GHNumberPickItem> _countPickList = new List<GHNumberPickItem>();
        private void MenuCanvas_LongTap(object sender, SKTouchEventArgs e)
        {
            int selectedidx = -1;
            for (int idx = _firstDrawnMenuItemIdx; idx >= 0 && idx <= _lastDrawnMenuItemIdx; idx++)
            {
                if (idx >= MenuCanvas.MenuItems.Count)
                    return;
                if (e.Location.Y >= MenuCanvas.MenuItems[idx].DrawBounds.Top && e.Location.Y <= MenuCanvas.MenuItems[idx].DrawBounds.Bottom)
                {
                    selectedidx = idx;
                    break;
                }
            }

            if (selectedidx < 0)
                return;

            if (MenuCanvas.MenuItems[selectedidx].Identifier == 0)
                return;

            if (MenuCanvas.MenuItems[selectedidx].MaxCount <= 1)
                return;

            _countMenuItem = MenuCanvas.MenuItems[selectedidx];

            _menuTouchMoved = true; /* No further action upon release */
            if ((MenuCanvas.SelectionHow == SelectionMode.Multiple && !MenuCanvas.MenuItems[selectedidx].Selected)
                || (MenuCanvas.SelectionHow == SelectionMode.Single && selectedidx != MenuCanvas.SelectionIndex))
                MenuCanvas_NormalClickRelease(sender, e); /* Normal click selection first */

            if (_countMenuItem.MaxCount > 100)
            {
                MenuCountForegroundGrid.VerticalOptions = LayoutOptions.StartAndExpand;
                MenuCountForegroundGrid.Margin = new Thickness(0, 50, 0, 0);
                CountPicker.IsVisible = false;
                MenuCountEntry.IsVisible = true;
                if (_countMenuItem.Count == -1)
                    MenuCountEntry.Text = _countMenuItem.MaxCount.ToString();
                else
                    MenuCountEntry.Text = _countMenuItem.Count.ToString();
            }
            else
            {
                MenuCountForegroundGrid.VerticalOptions = LayoutOptions.CenterAndExpand;
                MenuCountForegroundGrid.Margin = new Thickness(0, 0, 0, 0);
                CountPicker.IsVisible = true;
                MenuCountEntry.IsVisible = false;
                _countPickList.Clear();
                _countPickList.Add(new GHNumberPickItem(-1, "All"));
                int countselindex = -1;
                if (_countMenuItem.Count == -1)
                    countselindex = 0;
                for (int i = 0; i <= (int)MenuCanvas.MenuItems[selectedidx].MaxCount; i++)
                {
                    _countPickList.Add(new GHNumberPickItem(i));
                    if (_countMenuItem.Count == i)
                        countselindex = i + 1;
                }
                CountPicker.ItemsSource = _countPickList;
                CountPicker.ItemDisplayBinding = new Binding("Name");
                CountPicker.SelectedIndex = countselindex;
            }

            MenuCountCaption.Text = (MenuCountEntry.IsVisible ? "Type" : "Select") + " Count for " + MenuCanvas.MenuItems[selectedidx].MainText;
            MenuCountBackgroundGrid.IsVisible = true;
        }

        private void MenuCanvas_NormalClickRelease(object sender, SKTouchEventArgs e)
        {
            lock (MenuCanvas.MenuItemLock)
            {
                if (MenuCanvas.MenuItems == null)
                    return;

                for (int idx = _firstDrawnMenuItemIdx; idx >= 0 && idx <= _lastDrawnMenuItemIdx; idx++)
                {
                    if (idx >= MenuCanvas.MenuItems.Count)
                        break;
                    if (e.Location.Y >= MenuCanvas.MenuItems[idx].DrawBounds.Top && e.Location.Y <= MenuCanvas.MenuItems[idx].DrawBounds.Bottom)
                    {
                        GHMenuItem mi = MenuCanvas.MenuItems[idx];
                        if (mi.Identifier == 0)
                        {
                            if (MenuCanvas.SelectionHow == SelectionMode.Multiple && (mi.MenuFlags & (ulong)MenuFlags.IsGroupHeading) != 0)
                            {
                                foreach (GHMenuItem o in MenuCanvas.MenuItems)
                                {
                                    if (o.GroupAccelerator == mi.HeadingGroupAccelerator)
                                    {
                                        if (!mi.HeadingUnselectGroup)
                                        {
                                            o.Selected = true;
                                            o.Count = -1;
                                        }
                                        else
                                        {
                                            o.Selected = false;
                                            o.Count = 0;
                                        }
                                    }
                                }
                                mi.HeadingUnselectGroup = !mi.HeadingUnselectGroup;
                            }
                        }
                        else
                        {
                            if (MenuCanvas.SelectionHow == SelectionMode.Multiple)
                            {
                                MenuCanvas.MenuItems[idx].Selected = !MenuCanvas.MenuItems[idx].Selected;
                                if (MenuCanvas.MenuItems[idx].Selected)
                                    MenuCanvas.MenuItems[idx].Count = -1;
                                else
                                    MenuCanvas.MenuItems[idx].Count = 0;
                            }
                            else
                            {
                                if (idx != MenuCanvas.SelectionIndex && MenuCanvas.SelectionIndex >= 0 && MenuCanvas.SelectionIndex < MenuCanvas.MenuItems.Count)
                                    MenuCanvas.MenuItems[MenuCanvas.SelectionIndex].Count = 0;

                                MenuCanvas.SelectionIndex = idx;
                                if (MenuCanvas.MenuItems[idx].Count == 0)
                                    MenuCanvas.MenuItems[idx].Count = -1;

                                /* Else keep the current selection number */
                            }
                        }
                        break;
                    }
                }
            }
        }

        private object _menuHideCancelledLock = new object();
        private bool _menuHideCancelled = false;
        private void MenuOKButton_Clicked(object sender, EventArgs e)
        {
            lock (_menuDrawOnlyLock)
            {
                _menuRefresh = false;
                _menuDrawOnlyClear = true;
            }

            _menuScrollOffset = 0;
            _menuHideCancelled = false;

            ConcurrentQueue<GHResponse> queue;
            List<GHMenuItem> resultlist = new List<GHMenuItem>();
            lock (MenuCanvas.MenuItemLock)
            {
                if (MenuCanvas.SelectionHow == SelectionMode.Multiple)
                {
                    foreach (GHMenuItem mi in MenuCanvas.MenuItems)
                    {
                        if (mi.Selected && mi.Count != 0)
                        {
                            resultlist.Add(mi);
                        }
                    }
                }
                else if (MenuCanvas.SelectionHow == SelectionMode.Single)
                {
                    if (MenuCanvas.SelectionIndex > -1 && MenuCanvas.SelectionIndex < MenuCanvas.MenuItems.Count)
                    {
                        GHMenuItem mi = MenuCanvas.MenuItems[MenuCanvas.SelectionIndex];
                        if (mi.Count != 0)
                        {
                            resultlist.Add(mi);
                        }
                    }
                }
            }

            if (ClientGame.ResponseDictionary.TryGetValue(_gamePage.ClientGame, out queue))
            {
                queue.Enqueue(new GHResponse(_gamePage.ClientGame, GHRequestType.ShowMenuPage, MenuCanvas.GHWindow, resultlist));
            }

            DelayedMenuHide();
        }

        private void MenuCancelButton_Clicked(object sender, EventArgs e)
        {
            lock (_menuDrawOnlyLock)
            {
                _menuRefresh = false;
                _menuDrawOnlyClear = true;
            }

            _menuScrollOffset = 0;
            _menuHideCancelled = false;

            ConcurrentQueue<GHResponse> queue;
            if (ClientGame.ResponseDictionary.TryGetValue(_gamePage.ClientGame, out queue))
            {
                queue.Enqueue(new GHResponse(_gamePage.ClientGame, GHRequestType.ShowMenuPage, MenuCanvas.GHWindow, new List<GHMenuItem>()));
            }

            DelayedMenuHide();
        }

        private async void DelayedMenuHide()
        {
            await _gamePage.Navigation.PopModalAsync(false);
            //Device.StartTimer(TimeSpan.FromSeconds(3f / 40), () =>
            //{
            //    lock (_menuHideCancelledLock)
            //    {
            //        if (_menuHideCancelled)
            //            return false;
            //    }

            //    MenuGrid.IsVisible = false;

            //    lock (_gamePage.RefreshScreenLock)
            //    {
            //        _gamePage.RefreshScreen = true;
            //    }

            //    return false;
            //});
        }

        private bool unselect_on_tap = false;
        private void MenuTapGestureRecognizer_Tapped(object sender, EventArgs e)
        {
            if (MenuCanvas.SelectionHow == SelectionMode.Multiple)
            {
                lock (MenuCanvas.MenuItemLock)
                {
                    foreach (GHMenuItem o in MenuCanvas.MenuItems)
                    {
                        if (o.Identifier != 0)
                        {
                            if (!unselect_on_tap)
                            {
                                o.Selected = true;
                                o.Count = -1;
                            }
                            else
                            {
                                o.Selected = false;
                                o.Count = 0;
                            }
                        }
                    }
                    unselect_on_tap = !unselect_on_tap;
                }
            }
        }

        private void MenuCountOkButton_Clicked(object sender, EventArgs e)
        {
            if (_countMenuItem != null)
            {
                if (MenuCountEntry.IsVisible)
                {
                    string str = MenuCountEntry.Text;
                    int value;
                    bool res = int.TryParse(str, out value);
                    if (res)
                    {
                        if (value < 0 || value > _countMenuItem.MaxCount)
                            _countMenuItem.Count = -1;
                        else
                            _countMenuItem.Count = value;
                    }
                    else
                    {
                        MenuCountEntry.TextColor = Color.Red;
                        MenuCountEntry.Focus();
                        return;
                    }
                }
                else
                {
                    if (CountPicker.SelectedIndex >= 0 && CountPicker.SelectedIndex < _countPickList.Count)
                    {
                        lock (MenuCanvas.MenuItemLock)
                        {
                            _countMenuItem.Count = _countPickList[CountPicker.SelectedIndex].Number;
                            _countMenuItem.Selected = _countMenuItem.Count != 0;
                        }
                    }
                }
            }
            MenuCountBackgroundGrid.IsVisible = false;
        }

        private void MenuCountCancelButton_Clicked(object sender, EventArgs e)
        {
            MenuCountBackgroundGrid.IsVisible = false;
        }

        private void MenuEntry_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (_countMenuItem != null)
            {
                MenuCountEntry.TextColor = Color.White;
            }
        }

        private void MenuEntry_Completed(object sender, EventArgs e)
        {
            if (_countMenuItem != null)
            {
                string str = MenuCountEntry.Text;
                int value;
                bool res = int.TryParse(str, out value);
                if (res)
                {
                    MenuCountEntry.TextColor = Color.Green;
                }
                else
                {
                    MenuCountEntry.TextColor = Color.Red;
                }
            }
        }
    }
}