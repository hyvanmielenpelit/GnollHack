using GnollHackCommon;
using SkiaSharp;
using SkiaSharp.Views.Forms;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace GnollHackClient.Pages.Game
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class CommandCanvasPage : ContentPage
    {
        private GamePage _gamePage;
        public CommandCanvasPage(GamePage gamePage)
        {
            InitializeComponent();
            _gamePage = gamePage;

            Device.StartTimer(TimeSpan.FromSeconds(1f / 40), () =>
            {
                CommandCanvas.InvalidateSurface();
                return true;
            });

        }

        public object CommandButtonLock = new object();
        private Dictionary<long, TouchEntry> CommandTouchDictionary = new Dictionary<long, TouchEntry>();
        private object _savedCommandSender = null;
        private SKTouchEventArgs _savedCommandEventArgs = null;
        private DateTime _savedCommandTimeStamp;
        private bool _commandTouchMoved = false;

        private void CommandCanvas_PaintSurface(object sender, SKPaintSurfaceEventArgs e)
        {
            SKImageInfo info = e.Info;
            SKSurface surface = e.Surface;
            SKCanvas canvas = surface.Canvas;
            float canvaswidth = CommandCanvas.CanvasSize.Width;
            float canvasheight = CommandCanvas.CanvasSize.Height;
            float x = 0, y = 0;
            float scale = canvaswidth / (float)CommandCanvas.Width;
            float btnAreaWidth = canvaswidth / GHConstants.MoreButtonsPerRow;
            float btnAreaHeight = canvasheight / GHConstants.MoreButtonsPerColumn;

            canvas.Clear();

            using (SKPaint textPaint = new SKPaint())
            {
                textPaint.Color = SKColors.White;
                textPaint.Typeface = App.LatoRegular;
                textPaint.TextSize = 9.0f * scale;
                textPaint.TextAlign = SKTextAlign.Center;

                float btnImgRawWidth = Math.Min(btnAreaWidth, 80 * scale);
                float btnImgRawHeight = Math.Min(btnAreaHeight - textPaint.FontSpacing, 80 * scale);

                float btnImgWidth = Math.Min(btnImgRawWidth, btnImgRawHeight);
                float btnImgHeight = btnImgWidth;

                for (int i = 0; i < GHConstants.MoreButtonsPerRow; i++)
                {
                    for (int j = 0; j < GHConstants.MoreButtonsPerColumn; j++)
                    {
                        if (_gamePage._moreBtnMatrix[i, j] != null && _gamePage._moreBtnBitmaps[i, j] != null)
                        {
                            SKRect targetrect = new SKRect();
                            targetrect.Left = i * btnAreaWidth + Math.Max(0, (btnAreaWidth - btnImgWidth) / 2);
                            targetrect.Top = j * btnAreaHeight + Math.Max(0, (btnAreaHeight - btnImgHeight - textPaint.FontSpacing) / 2);
                            targetrect.Right = targetrect.Left + btnImgWidth;
                            targetrect.Bottom = targetrect.Top + btnImgHeight;
                            float text_x = (targetrect.Left + targetrect.Right) / 2;
                            float text_y = targetrect.Bottom - textPaint.FontMetrics.Ascent;

                            canvas.DrawBitmap(_gamePage._moreBtnBitmaps[i, j], targetrect);
                            canvas.DrawText(_gamePage._moreBtnMatrix[i, j].Text, text_x, text_y, textPaint);
                        }
                    }
                }
            }
        }

        private void CommandCanvas_Touch(object sender, SKTouchEventArgs e)
        {
            lock (CommandButtonLock)
            {
                switch (e?.ActionType)
                {
                    case SKTouchAction.Entered:
                        break;
                    case SKTouchAction.Pressed:
                        _savedCommandSender = null;
                        _savedCommandEventArgs = null;
                        _savedCommandTimeStamp = DateTime.Now;

                        if (CommandTouchDictionary.ContainsKey(e.Id))
                            CommandTouchDictionary[e.Id] = new TouchEntry(e.Location, DateTime.Now);
                        else
                            CommandTouchDictionary.Add(e.Id, new TouchEntry(e.Location, DateTime.Now));

                        if (CommandTouchDictionary.Count > 1)
                            _commandTouchMoved = true;
                        else
                        {
                            _savedCommandSender = sender;
                            _savedCommandEventArgs = e;
                        }

                        e.Handled = true;
                        break;
                    case SKTouchAction.Moved:
                        {
                            TouchEntry entry;
                            bool res = CommandTouchDictionary.TryGetValue(e.Id, out entry);
                            if (res)
                            {
                                SKPoint anchor = entry.Location;

                                float diffX = e.Location.X - anchor.X;
                                float diffY = e.Location.Y - anchor.Y;
                                float dist = (float)Math.Sqrt((Math.Pow(diffX, 2) + Math.Pow(diffY, 2)));

                                if (CommandTouchDictionary.Count == 1)
                                {
                                    if ((dist > 25 ||
                                        (DateTime.Now.Ticks - entry.PressTime.Ticks) / TimeSpan.TicksPerMillisecond > GHConstants.MoveOrPressTimeThreshold
                                           ))
                                    {
                                        /* Just one finger */
                                        if (diffX != 0 || diffY != 0)
                                        {
                                            /* Do something here on swipe */

                                            CommandTouchDictionary[e.Id].Location = e.Location;
                                            _commandTouchMoved = true;
                                            _savedCommandTimeStamp = DateTime.Now;
                                        }
                                    }
                                }
                            }
                            e.Handled = true;
                        }
                        break;
                    case SKTouchAction.Released:
                        {
                            _savedCommandSender = null;
                            _savedCommandEventArgs = null;
                            _savedCommandTimeStamp = DateTime.Now;

                            TouchEntry entry;
                            bool res = CommandTouchDictionary.TryGetValue(e.Id, out entry);
                            long elapsedms = (DateTime.Now.Ticks - entry.PressTime.Ticks) / TimeSpan.TicksPerMillisecond;

                            if (elapsedms <= GHConstants.MoveOrPressTimeThreshold && !_commandTouchMoved)
                            {
                                /* Normal click */
                                /* Select command here*/
                                float canvaswidth = CommandCanvas.CanvasSize.Width;
                                float canvasheight = CommandCanvas.CanvasSize.Height;
                                float btnAreaWidth = canvaswidth / GHConstants.MoreButtonsPerRow;
                                float btnAreaHeight = canvasheight / GHConstants.MoreButtonsPerColumn;
                                int btnX = (int)(e.Location.X / btnAreaWidth);
                                int btnY = (int)(e.Location.Y / btnAreaHeight);

                                if (btnX >= 0 && btnX < GHConstants.MoreButtonsPerRow && btnY >= 0 && btnY < GHConstants.MoreButtonsPerColumn)
                                {
                                    GHCommandButtonItem cbi = _gamePage._moreBtnMatrix[btnX, btnY];
                                    if (cbi != null)
                                    {
                                        if (cbi.Command >= 0)
                                            _gamePage?.GenericButton_Clicked(CommandCanvas, e, cbi.Command);
                                    }
                                }

                                ClosePage();
                            }
                            if (CommandTouchDictionary.ContainsKey(e.Id))
                                CommandTouchDictionary.Remove(e.Id);
                            if (CommandTouchDictionary.Count == 0)
                                _commandTouchMoved = false;
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
            MoreCommandsGrid.IsEnabled = false;
            await _gamePage.Navigation.PopModalAsync(false);
        }

    }
}