using SkiaSharp;
using System;
using System.Collections.Generic;
using System.Text;
#if GNH_MAUI
using GnollHackM;
#else
using GnollHackX.Pages.Game;
#endif

namespace GnollHackX
{
    public class GHScreenFilter
    {
        private DisplayScreenFilterData _data;
        private long _created_at_count;
        private GamePage _gamePage;
        private int _animationFrequency;

        public GHScreenFilter(DisplayScreenFilterData data, long created_at_count, GamePage gamePage)
        {
            _data = data;
            _created_at_count = created_at_count;
            _gamePage = gamePage;
            _animationFrequency = Math.Max(1, ClientUtils.GetMainCanvasAnimationFrequency(_gamePage.MapRefreshRate));
        }

        public long CreatedAt { get { return _created_at_count; } }

        public float GetFinishTime()
        {
            return 2.0f;
        }
        public float GetFadeOutEndTime()
        {
            return 0.35f;
        }
        public float GetFadeOutTime()
        {
            return 0.20f;
        }
        public float GetFadeInTime()
        {
            return 0.15f;
        }
        public float GetFadeInLength()
        {
            return GetFadeInTime();
        }
        public float GetFadeOutLength()
        {
            return GetFadeOutEndTime() - GetFadeOutTime();
        }

        public float GetVSecs(long counter_value)
        {
            if (counter_value < 5)
                return 999.0f;

            return ((float)(counter_value - _created_at_count)) / (float)_animationFrequency;
        }

        public long GetFinishCounterValue()
        {
            return (long)(GetFinishTime() * (float)_animationFrequency) + _created_at_count;
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

        public SKColor GetBaseColor(long counter_value)
        {
            switch (_data.style)
            {
                case 0:
                    return ClientUtils.NHColor2SKColor(_data.color, _data.attr).WithAlpha(128);
                default:
                    break;
            }
            return SKColors.Red.WithAlpha(128);
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
            else if (vsecs >= GetFadeOutEndTime())
            {
                return SKColors.Transparent;
            }
            else if (vsecs >= GetFadeOutTime())
            {
                byte val = (byte)(baseclr.Alpha * Math.Max(0.0f, Math.Min(1.0f, (GetFadeOutEndTime() - vsecs) / GetFadeOutLength())));
                SKColor clr = new SKColor(baseclr.Red, baseclr.Green, baseclr.Blue, val);
                return clr;
            }
            else
                return baseclr;
        }
    }
}
