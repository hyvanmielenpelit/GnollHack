using SkiaSharp;
using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackClient
{
    public class GHConditionText
    {
        private DisplayConditionTextData _data;
        private long _created_at_count;

        public GHConditionText(DisplayConditionTextData data, long created_at_count)
        {
            _data = data;
            _created_at_count = created_at_count;
        }

        public long CreatedAt { get { return _created_at_count; } }

        public float GetFinishTime()
        {
            return 2.0f;
        }
        public float GetFadeOutTime()
        {
            return 1.75f;
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

            return ((float)(counter_value - _created_at_count)) / 40.0f;
        }

        public long GetFinishCounterValue()
        {
            return (long)(GetFinishTime() * 40.0f) + _created_at_count;
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


        public string GetText(long counter_value)
        {
            return _data.text;
        }
        public string GetSampleText()
        {
            switch (_data.style)
            {
                default:
                    break;
            }
            return "Terminally Ill";
        }
        public float GetRelativeSampleTextSize(long counter_value)
        {
            switch (_data.style)
            {
                default:
                    break;
            }
            return 0.65f;
        }
        public SKTypeface GetTypeface(long counter_value)
        {
            return App.ImmortalTypeface;
        }
        public SKColor GetBaseColor(long counter_value)
        {
            switch (_data.style)
            {
                case 0:
                    return ClientUtils.NHColor2SKColor(_data.color);
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
        public float GetRelativeOutlineWidth(long counter_value)
        {
            return 1f / 10f;
        }
        public SKColor GetOutlineBaseColor(long counter_value)
        {
            return SKColors.Black;
        }
        public SKColor GetOutlineColor(long counter_value)
        {
            return GetTimedColor(GetOutlineBaseColor(counter_value), counter_value);
        }
    }
}
