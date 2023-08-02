using SkiaSharp;
using System;
using System.Collections.Generic;
using System.Text;
using GnollHackX.Pages.Game;

namespace GnollHackX
{
    public class GHGUIEffect
    {
        private DisplayGUIEffectData _data;
        private long _created_at_count;
        private GamePage _gamePage;
        private int _animationFrequency;

        public GHGUIEffect(DisplayGUIEffectData data, long created_at_count, GamePage gamePage)
        {
            _data = data;
            _created_at_count = created_at_count;
            _gamePage = gamePage;
            _animationFrequency = Math.Max(1, ClientUtils.GetMainCanvasAnimationFrequency(_gamePage.MapRefreshRate));
        }

        public int X { get { return _data.x; } }
        public int Y { get { return _data.y; } }
        public long CreatedAt { get { return _created_at_count; } }
        public int Style { get { return _data.style; } }

        public float GetFinishTime()
        {
            return 0.5f;
        }
        public float GetFadeOutTime()
        {
            return 0.3f;
        }
        public float GetFadeInTime()
        {
            return 0.10f;
        }
        public float GetFadeInLength()
        {
            return GetFadeInTime();
        }
        public float GetFadeOutLength()
        {
            return GetFinishTime() - GetFadeOutTime();
        }

        public float GetVSecs(long counter_value)
        {
            if (counter_value < 5)
                return 999.0f;

            return ((float)(counter_value - _created_at_count)) / (float)_animationFrequency;
        }

        public bool IsVisible(long counter_value)
        {
            if (counter_value < _created_at_count || IsFinished(counter_value))
                return false;
            else
                return true;
        }

        public bool IsFinished(long counter_value)
        {
            if (counter_value < 5)
                return true; //Assume counter has exceeded max value and reverted to 0

            float vsecs = GetVSecs(counter_value);
            if (vsecs >= GetFinishTime())
                return true;
            else
                return false;
        }

        public SKPoint GetPosition(long counter_value)
        {
            SKPoint p = new SKPoint();
            p.X = _data.x;
            p.Y = _data.y;
            return p;
        }
        public SKColor GetBaseColor(long counter_value)
        {
            switch (_data.style)
            {
                case 0:
                    return SKColors.White;
                default:
                    break;
            }
            return SKColors.Red;
        }
        public SKColor GetColor(long counter_value)
        {
            return GetTimedColor(GetBaseColor(counter_value), counter_value);
        }

        public SKColor GetTimedColor(SKColor baseclr, long counter_value)
        {
            float vsecs = GetVSecs(counter_value);
            if (!IsVisible(counter_value))
                return SKColors.Transparent;
            else if (vsecs < GetFadeInTime())
            {
                byte val = (byte)(baseclr.Alpha * Math.Max(0.0f, Math.Min(1.0f, (vsecs) / GetFadeInLength())));
                SKColor clr = new SKColor(baseclr.Red, baseclr.Green, baseclr.Blue, val);
                return clr;
            }
            else if (vsecs >= GetFadeOutTime())
            {
                byte val = (byte)(baseclr.Alpha * Math.Max(0.0f, Math.Min(1.0f, (GetFinishTime() - vsecs) / GetFadeOutLength())));
                SKColor clr = new SKColor(baseclr.Red, baseclr.Green, baseclr.Blue, val);
                return clr;
            }
            else
                return baseclr;
        }
    }
}
