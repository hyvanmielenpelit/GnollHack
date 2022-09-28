using System;
using System.Collections.Generic;
using System.Text;
using Xamarin.Forms;

namespace GnollHackClient
{
    // Pull request test. Thanks for the help Janne! Please delete this.
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
        private Color _glow = new Color(1, 1, 0.85, 0.1);
        protected override void OnPropertyChanged(string propertyName = null)
        {
            base.OnPropertyChanged(propertyName);
            _counter = (_counter + 1) % _animationLength;

            if (_counter < 80)
            {
               Device.BeginInvokeOnMainThread(() =>
               {
                   double x = (double)(_counter) / 80;
                   TextColor = new Color(
                       Color.Yellow.R * x + Color.White.R * (1 - x),
                       Color.Yellow.G * x + Color.White.G * (1 - x),
                       Color.Yellow.B * x + Color.White.B * (1 - x));

                   BackgroundColor = new Color(
                       _glow.R * x + Color.Transparent.R * (1 - x),
                       _glow.G * x + Color.Transparent.G * (1 - x),
                       _glow.B * x + Color.Transparent.B * (1 - x),
                       _glow.A * x + Color.Transparent.A * (1 - x));
               });
            }
            else if (_counter < 160)
            {
                Device.BeginInvokeOnMainThread(() =>
                {
                    double x = (double)(_counter - 80) / 80;
                    TextColor = new Color(
                        Color.White.R * x + Color.Yellow.R * (1 - x),
                        Color.White.G * x + Color.Yellow.G * (1 - x),
                        Color.White.B * x + Color.Yellow.B * (1 - x));
                    
                    BackgroundColor = new Color(
                        Color.Transparent.R * x + _glow.R * (1 - x),
                        Color.Transparent.G * x + _glow.G * (1 - x),
                        Color.Transparent.B * x + _glow.B * (1 - x),
                        Color.Transparent.A * x + _glow.A * (1 - x));
                });
            }
            else
            {
                Device.BeginInvokeOnMainThread(() =>
                {
                    TextColor = Color.White;
                    BackgroundColor = Color.Transparent;
                });
            }
        }
    }
}
