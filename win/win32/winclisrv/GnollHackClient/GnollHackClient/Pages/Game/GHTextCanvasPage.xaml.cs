using GnollHackCommon;
using SkiaSharp;
using SkiaSharp.Views.Forms;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace GnollHackClient.Pages.Game
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class GHTextCanvasPage : ContentPage
    {
        private GamePage _gamePage;
        public GHTextCanvasPage(GamePage gamePage, GHWindow window, List<GHPutStrItem> strs)
        {
            InitializeComponent();
            _gamePage = gamePage;

            lock (_gamePage.RefreshScreenLock)
            {
                _gamePage.RefreshScreen = false;
            }

            lock (TextScrollLock)
            {
                _textScrollOffset = 0;
            }

            TextWindowGlyphImage.Source = null;

            _textGlyphImageSource.ReferenceGamePage = _gamePage;
            _textGlyphImageSource.AutoSize = true;
            _textGlyphImageSource.ObjData = window.ObjData;
            _textGlyphImageSource.Glyph = window.Glyph;

            TextWindowGlyphImage.Source = TextGlyphImage;
            TextWindowGlyphImage.IsVisible = IsTextGlyphVisible;

            List<GHPutStrItem> items = null;
            if (window.WindowStyle == ghwindow_styles.GHWINDOW_STYLE_PAGER_GENERAL || window.WindowStyle == ghwindow_styles.GHWINDOW_STYLE_PAGER_SPEAKER)
            {
                items = new List<GHPutStrItem>();
                ClientUtils.ProcessAdjustedItems(items, strs);
            }
            else
                items = strs;

            lock (TextCanvas.TextItemLock)
            {
                TextTouchDictionary.Clear();
                TextCanvas.GHWindow = window;

                TextCanvas.PutStrItems = items;
            }

            TextGrid.IsVisible = true;

            Device.StartTimer(TimeSpan.FromSeconds(1f / 40), () =>
            {
                TextCanvas.InvalidateSurface();
                return true;
            });

        }

        private GlyphImageSource _textGlyphImageSource = new GlyphImageSource();

        public ImageSource TextGlyphImage
        {
            get
            {
                return _textGlyphImageSource;
            }
        }

        public bool IsTextGlyphVisible
        {
            get
            {
                return (Math.Abs(_textGlyphImageSource.Glyph) > 0 && _textGlyphImageSource.Glyph != _gamePage.NoGlyph);
            }
        }

        private int _firstDrawnTextItemIdx = -1;
        private int _lastDrawnTextItemIdx = -1;
        private float _totalTextHeight = 0;

        private object TextScrollLock = new object();
        private float _textScrollOffset = 0;

        private Dictionary<long, TouchEntry> TextTouchDictionary = new Dictionary<long, TouchEntry>();
        private object _savedTextSender = null;
        private SKTouchEventArgs _savedTextEventArgs = null;
        private DateTime _savedTextTimeStamp;
        private bool _textTouchMoved = false;

        private void TextCanvas_PaintSurface(object sender, SKPaintSurfaceEventArgs e)
        {
            SKImageInfo info = e.Info;
            SKSurface surface = e.Surface;
            SKCanvas canvas = surface.Canvas;
            float canvaswidth = TextCanvas.CanvasSize.Width;
            float canvasheight = TextCanvas.CanvasSize.Height;
            float x = 0, y = 0;
            string str;
            float scale = canvaswidth / (float)TextCanvas.Width;

            canvas.Clear();

            lock (TextCanvas.MenuItemLock)
            {
                if (TextCanvas.PutStrItems == null || TextCanvas.PutStrItems.Count == 0)
                    return;
            }

            using (SKPaint textPaint = new SKPaint())
            {
                textPaint.Typeface = App.UnderwoodTypeface;
                textPaint.TextSize = 30 * scale;
                textPaint.Style = SKPaintStyle.Fill;
                float minrowheight = textPaint.FontSpacing;
                float leftinnerpadding = 5;
                float curmenuoffset = 0;
                lock (TextScrollLock)
                {
                    curmenuoffset = _textScrollOffset;
                }
                y += curmenuoffset;
                double menuwidth = Math.Max(1, Math.Min(TextCanvas.Width, 600));
                float menuwidthoncanvas = (float)(menuwidth * scale);
                float leftmenupadding = Math.Max(0, (canvaswidth - menuwidthoncanvas) / 2);
                float rightmenupadding = leftmenupadding;
                float topPadding = 0;

                lock (TextCanvas.TextItemLock)
                {
                    int j = 0;
                    y += topPadding;
                    foreach (GHPutStrItem putstritem in TextCanvas.PutStrItems)
                    {
                        int pos = 0;
                        x = leftmenupadding + leftinnerpadding;
                        textPaint.Typeface = App.GetTypefaceByName(putstritem.TextWindowFontFamily);
                        textPaint.TextSize = (float)putstritem.TextWindowFontSize * scale;
                        y -= textPaint.FontMetrics.Ascent;
                        foreach (GHPutStrInstructions instr in putstritem.InstructionList)
                        {
                            if (putstritem.Text == null)
                                str = "";
                            else if (pos + instr.PrintLength <= putstritem.Text.Length)
                                str = putstritem.Text.Substring(pos, instr.PrintLength);
                            else if (putstritem.Text.Length - pos > 0)
                                str = putstritem.Text.Substring(pos, putstritem.Text.Length - pos);
                            else
                                str = "";
                            pos += str.Length;

                            textPaint.Color = ClientUtils.NHColor2SKColor(instr.Color < (int)nhcolor.CLR_MAX ? (nhcolor)instr.Color : nhcolor.CLR_WHITE);

                            string[] split = str.Split(' ');
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
                        }
                        j++;
                        y += textPaint.FontSpacing + textPaint.FontMetrics.Ascent;
                    }
                    _totalTextHeight = y - curmenuoffset;
                }
            }
        }

        private void TextCanvas_Touch(object sender, SKTouchEventArgs e)
        {
            lock (TextCanvas.TextItemLock)
            {
                switch (e?.ActionType)
                {
                    case SKTouchAction.Entered:
                        break;
                    case SKTouchAction.Pressed:
                        _savedTextSender = null;
                        _savedTextEventArgs = null;
                        _savedTextTimeStamp = DateTime.Now;

                        if (TextTouchDictionary.ContainsKey(e.Id))
                            TextTouchDictionary[e.Id] = new TouchEntry(e.Location, DateTime.Now);
                        else
                            TextTouchDictionary.Add(e.Id, new TouchEntry(e.Location, DateTime.Now));

                        if (TextTouchDictionary.Count > 1)
                            _textTouchMoved = true;
                        else
                        {
                            _savedTextSender = sender;
                            _savedTextEventArgs = e;
                        }

                        e.Handled = true;
                        break;
                    case SKTouchAction.Moved:
                        {
                            TouchEntry entry;
                            bool res = TextTouchDictionary.TryGetValue(e.Id, out entry);
                            if (res)
                            {
                                SKPoint anchor = entry.Location;

                                float diffX = e.Location.X - anchor.X;
                                float diffY = e.Location.Y - anchor.Y;
                                float dist = (float)Math.Sqrt((Math.Pow(diffX, 2) + Math.Pow(diffY, 2)));

                                if (TextTouchDictionary.Count == 1)
                                {
                                    if ((dist > 25 ||
                                        (DateTime.Now.Ticks - entry.PressTime.Ticks) / TimeSpan.TicksPerMillisecond > GHConstants.MoveOrPressTimeThreshold
                                           ))
                                    {
                                        /* Just one finger => Move the map */
                                        if (diffX != 0 || diffY != 0)
                                        {
                                            lock (TextScrollLock)
                                            {
                                                _textScrollOffset += diffY;
                                                if (_textScrollOffset > 0)
                                                    _textScrollOffset = 0;
                                                else if (_textScrollOffset < TextCanvas.CanvasSize.Height - _totalTextHeight)
                                                    _textScrollOffset = Math.Min(0, TextCanvas.CanvasSize.Height - _totalTextHeight);
                                            }
                                            TextTouchDictionary[e.Id].Location = e.Location;
                                            _textTouchMoved = true;
                                            _savedTextTimeStamp = DateTime.Now;
                                        }
                                    }
                                }
                            }
                            e.Handled = true;
                        }
                        break;
                    case SKTouchAction.Released:
                        {
                            _savedTextSender = null;
                            _savedTextEventArgs = null;
                            _savedTextTimeStamp = DateTime.Now;

                            TouchEntry entry;
                            bool res = TextTouchDictionary.TryGetValue(e.Id, out entry);
                            long elapsedms = (DateTime.Now.Ticks - entry.PressTime.Ticks) / TimeSpan.TicksPerMillisecond;

                            if (elapsedms <= GHConstants.MoveOrPressTimeThreshold && !_textTouchMoved)
                            {
                                /* Normal click -- Hide the canvas */
                                _gamePage.GenericButton_Clicked(sender, e, 27);
                                ClosePage();
                            }
                            if (TextTouchDictionary.ContainsKey(e.Id))
                                TextTouchDictionary.Remove(e.Id);
                            if (TextTouchDictionary.Count == 0)
                                _textTouchMoved = false;
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
        }

        private async void ClosePage()
        {
            TextGrid.IsEnabled = false;
            await _gamePage.Navigation.PopModalAsync(false);
        }

    }
}