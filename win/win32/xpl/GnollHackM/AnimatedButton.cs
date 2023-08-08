using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackM
{
    public class AnimatedButton : Button
    {
        public AnimatedButton()
        {

        }

        private int _counter = 0;

        public BindableProperty AnimationCounterProperty =
            BindableProperty.Create(nameof(AnimationCounter), typeof(long), typeof(AnimatedButton), 0L);

        public long AnimationCounter
        {
            get { return (long)GetValue(AnimationCounterProperty); }
            set { SetValue(AnimationCounterProperty, value); }
        }

        private const int _animationLength = 240;
        private Color _glow = new Color(1, 1, 0.85f, 0.1f);
        protected override void OnPropertyChanged(string propertyName = null)
        {
            base.OnPropertyChanged(propertyName);
            _counter = (_counter + 1) % _animationLength;

            if (_counter < 80)
            {
               Device.BeginInvokeOnMainThread(() =>
               {
                   float x = (float)(_counter) / 80;
                   TextColor = new Color(
                       Colors.Yellow.Red * x + Colors.White.Red * (1 - x),
                       Colors.Yellow.Green * x + Colors.White.Green * (1 - x),
                       Colors.Yellow.Blue * x + Colors.White.Blue * (1 - x));

                   BackgroundColor = new Color(
                       _glow.Red * x + Colors.Transparent.Red * (1 - x),
                       _glow.Green * x + Colors.Transparent.Green * (1 - x),
                       _glow.Blue * x + Colors.Transparent.Blue * (1 - x),
                       _glow.Alpha * x + Colors.Transparent.Alpha * (1 - x));
               });
            }
            else if (_counter < 160)
            {
                Device.BeginInvokeOnMainThread(() =>
                {
                    float x = (float)(_counter - 80) / 80;
                    TextColor = new Color(
                        Colors.White.Red * x + Colors.Yellow.Red * (1 - x),
                        Colors.White.Green * x + Colors.Yellow.Green * (1 - x),
                        Colors.White.Blue * x + Colors.Yellow.Blue * (1 - x));
                    
                    BackgroundColor = new Color(
                        Colors.Transparent.Red * x + _glow.Red * (1 - x),
                        Colors.Transparent.Green * x + _glow.Green * (1 - x),
                        Colors.Transparent.Blue * x + _glow.Blue * (1 - x),
                        Colors.Transparent.Alpha * x + _glow.Alpha * (1 - x));
                });
            }
            else
            {
                Device.BeginInvokeOnMainThread(() =>
                {
                    TextColor = Colors.White;
                    BackgroundColor = Colors.Transparent;
                });
            }
        }
    }
}
