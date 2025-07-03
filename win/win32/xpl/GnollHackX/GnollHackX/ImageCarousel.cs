using SkiaSharp;
using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using System.Text;

#if GNH_MAUI
using GnollHackX;
using SkiaSharp.Views.Maui;
using SkiaSharp.Views.Maui.Controls;

namespace GnollHackM
#else
using SkiaSharp.Views.Forms;
using Xamarin.Forms;
using Xamarin.Essentials;

namespace GnollHackX
#endif
{
    public struct CarouselBitmap
    {
        public string ResourcePath;
        public SKImage Bitmap;
        public TextAlignment HorizontalFitAlignment;
        public TextAlignment VerticalFitAlignment;

        public CarouselBitmap(string resourcePath, TextAlignment hAlign, TextAlignment vAlign)
        {
            ResourcePath = resourcePath;
            HorizontalFitAlignment = hAlign;
            VerticalFitAlignment = vAlign;
            Bitmap = null;
        }
    }

    public class ImageCarousel : SKCanvasView
    {
        public ImageCarousel() : base()
        {
            PaintSurface += Base_PaintSurface;
        }

        CarouselBitmap[] _caruselBitmaps = new CarouselBitmap[]
        {
            new CarouselBitmap(GHApp.AppResourceName + ".Assets.main-menu-ranger.jpg", TextAlignment.Center, TextAlignment.Center),
            new CarouselBitmap(GHApp.AppResourceName + ".Assets.main-menu-dwarf.jpg", TextAlignment.Center, TextAlignment.Center),
            new CarouselBitmap(GHApp.AppResourceName + ".Assets.main-menu-gnoll.jpg", TextAlignment.Center, TextAlignment.Center),
        };

        private bool _inited = false;
        public void Init()
        {
            if (_inited)
                return;

            Assembly assembly = GetType().GetTypeInfo().Assembly;
            for (int i = 0; i < _caruselBitmaps.Length; i++)
            {
                using (Stream stream = assembly.GetManifestResourceStream(_caruselBitmaps[i].ResourcePath))
                {
                    if (stream != null)
                    {
                        SKBitmap res = SKBitmap.Decode(stream);
                        res.SetImmutable();
                        _caruselBitmaps[i].Bitmap = SKImage.FromBitmap(res);
                    }
                }
            }

            _inited = true; 
        }

        public readonly long _refreshFrequency = (long)UIUtils.GetGeneralAnimationFrequency();
        public const long _slideDurationInMilliseconds = 15000;
        public const long _transitionDurationInMilliseconds = 3000;

        private readonly object _counterValueLock = new object();
        private long _counterValue;
        public long CounterValue { get { lock (_counterValueLock) { return _counterValue; } } set { lock (_counterValueLock) { _counterValue = value; } } }

        private readonly object _timerIsOnLock = new object();
        private bool _timerIsOn = false;
        public bool CheckTimerOnAndSetTrue { get { lock (_timerIsOnLock) { bool oldValue = _timerIsOn; _timerIsOn = true; return oldValue; } } }
        public bool TimerIsOn { get { lock (_timerIsOnLock) { return _timerIsOn; } } set { lock (_timerIsOnLock) { _timerIsOn = value; } } }
        public void Play()
        {
            if (!_inited)
                Init();

            InvalidateSurface();
            if(!CheckTimerOnAndSetTrue)
            {
#if GNH_MAUI
                    var timer = Microsoft.Maui.Controls.Application.Current.Dispatcher.CreateTimer();
                    timer.Interval = TimeSpan.FromSeconds(1.0 / _refreshFrequency);
                    timer.IsRepeating = true;
                    timer.Tick += (s, e) => { DoIncrementCounter(); if (!TimerIsOn) timer.Stop(); };
                    timer.Start();
#else
                Device.StartTimer(TimeSpan.FromSeconds(1.0 / _refreshFrequency), () =>
                {
                    DoIncrementCounter();
                    return TimerIsOn;
                });
#endif
            }
        }

        private void DoIncrementCounter()
        {
            MainThread.BeginInvokeOnMainThread(() =>
            {
                long counter;
                lock (_counterValueLock)
                {
                    _counterValue++;
                    if (_counterValue >= (long)int.MaxValue)
                        _counterValue = 0;
                    counter = _counterValue;
                }
                byte alpha = GetSecondBitmapAlpha(counter);
                byte prevalpha = GetSecondBitmapAlpha(counter - 1);
                if (alpha > 0 || prevalpha > 0)
                    InvalidateSurface();
            });
        }

        public void Stop()
        {
            TimerIsOn = false;
        }

        private void UpdateCarousel()
        {
            InvalidateSurface();
        }

        private double _currentWidth = 0;
        private double _currentHeight = 0;
        protected override void OnSizeAllocated(double width, double height)
        {
            base.OnSizeAllocated(width, height);
            if (width != _currentWidth || height != _currentHeight)
            {
                _currentWidth = width;
                _currentHeight = height;
                UpdateCarousel();
            }
        }

        private bool _initialDraw = true;

        private void Base_PaintSurface(object sender, SKPaintSurfaceEventArgs e)
        {
            if (!_initialDraw && !TimerIsOn)
                return;

            SKImageInfo info = e.Info;
            SKSurface surface = e.Surface;
            SKCanvas canvas = surface.Canvas;
            float canvaswidth = info.Width;
            float canvasheight = info.Height;

            canvas.Clear();

            int cnt = _caruselBitmaps.Length;
            if (cnt > 0)
            {
                SKRect sourceRect, targetRect;
                float source_width, source_height;
                float target_x, target_y, target_width, target_height;
                float scale_x, scale_y, scale;
                TextAlignment halign;
                TextAlignment valign;
                using (SKPaint bmpPaint = new SKPaint())
                {
                    int idx = GetFirstBitmapIndex();
                    if(_caruselBitmaps[idx].Bitmap != null)
                    {
                        source_width = (float)_caruselBitmaps[idx].Bitmap.Width;
                        source_height = (float)_caruselBitmaps[idx].Bitmap.Height;
                        sourceRect = new SKRect(0, 0, source_width, source_height);
                        scale_x = canvaswidth / source_width;
                        scale_y = canvasheight / source_height;
                        scale = Math.Max(scale_x, scale_y);
                        target_width = scale * source_width;
                        target_height = scale * source_height;
                        halign = _caruselBitmaps[idx].HorizontalFitAlignment;
                        valign = _caruselBitmaps[idx].VerticalFitAlignment;
                        switch (halign)
                        {
                            case TextAlignment.Start:
                                target_x = 0;
                                break;
                            default:
                            case TextAlignment.Center:
                                target_x = (canvaswidth - target_width) / 2;
                                break;
                            case TextAlignment.End:
                                target_x = (canvaswidth - target_width);
                                break;
                        }
                        switch (valign)
                        {
                            case TextAlignment.Start:
                                target_y = 0;
                                break;
                            default:
                            case TextAlignment.Center:
                                target_y = (canvasheight - target_height) / 2;
                                break;
                            case TextAlignment.End:
                                target_y = (canvasheight - target_height);
                                break;
                        }
                        targetRect = new SKRect(target_x, target_y, target_x + target_width, target_y + target_height);
                        canvas.DrawImage(_caruselBitmaps[idx].Bitmap, sourceRect, targetRect, bmpPaint);

                        if (cnt > 1)
                        {
                            byte alpha = GetSecondBitmapAlpha(CounterValue);
                            if (alpha > 0)
                            {
                                int idx2 = GetSecondBitmapIndex();
                                if (_caruselBitmaps[idx2].Bitmap != null)
                                {
                                    source_width = (float)_caruselBitmaps[idx2].Bitmap.Width;
                                    source_height = (float)_caruselBitmaps[idx2].Bitmap.Height;
                                    sourceRect = new SKRect(0, 0, source_width, source_height);
                                    scale_x = canvaswidth / source_width;
                                    scale_y = canvasheight / source_height;
                                    scale = Math.Max(scale_x, scale_y);
                                    target_width = scale * source_width;
                                    target_height = scale * source_height;
                                    halign = _caruselBitmaps[idx2].HorizontalFitAlignment;
                                    valign = _caruselBitmaps[idx2].VerticalFitAlignment;
                                    switch (halign)
                                    {
                                        case TextAlignment.Start:
                                            target_x = 0;
                                            break;
                                        default:
                                        case TextAlignment.Center:
                                            target_x = (canvaswidth - target_width) / 2;
                                            break;
                                        case TextAlignment.End:
                                            target_x = (canvaswidth - target_width);
                                            break;
                                    }
                                    switch (valign)
                                    {
                                        case TextAlignment.Start:
                                            target_y = 0;
                                            break;
                                        default:
                                        case TextAlignment.Center:
                                            target_y = (canvasheight - target_height) / 2;
                                            break;
                                        case TextAlignment.End:
                                            target_y = (canvasheight - target_height);
                                            break;
                                    }
                                    targetRect = new SKRect(target_x, target_y, target_x + target_width, target_y + target_height);

                                    byte oldalpha = bmpPaint.Color.Alpha;
                                    bmpPaint.Color = bmpPaint.Color.WithAlpha(alpha);
                                    canvas.DrawImage(_caruselBitmaps[idx2].Bitmap, sourceRect, targetRect, bmpPaint);
                                    bmpPaint.Color = bmpPaint.Color.WithAlpha(oldalpha);
                                }
                            }
                        }
                    }
                }
            }
            _initialDraw = false;
        }

        private int GetFirstBitmapIndex()
        {
            int cnt = _caruselBitmaps.Length;
            if(cnt == 0)
                return 0;
            long idx = CounterValue * 1000 / (_slideDurationInMilliseconds * _refreshFrequency);
            return (int)idx % cnt;
        }

        private int GetSecondBitmapIndex()
        {
            int cnt = _caruselBitmaps.Length;
            if (cnt == 0)
                return 0;
            long firstidx = GetFirstBitmapIndex();
            return (int)(firstidx + 1) % cnt;
        }

        private byte GetSecondBitmapAlpha(long counter)
        {
            long slidedurationinticks = (_slideDurationInMilliseconds * _refreshFrequency) / 1000;
            long transitiondurationinticks = (_transitionDurationInMilliseconds * _refreshFrequency) / 1000;
            long countercycleticks = counter % slidedurationinticks;
            double opacity = Math.Min(1.0, (double)Math.Max(0, transitiondurationinticks - (slidedurationinticks - countercycleticks)) / (double)transitiondurationinticks);
            return (byte)(255.0 * opacity);
        }
    }
}
