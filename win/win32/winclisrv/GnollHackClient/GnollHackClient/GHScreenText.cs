using System;
using System.Collections.Generic;
using System.Text;
using SkiaSharp;

namespace GnollHackClient
{
    class GHScreenText
    {
        private DisplayScreenTextData _data;
        public long _created_at_count;

        public bool HasSubText { get { return _data.subtext != null; } }

        public GHScreenText(DisplayScreenTextData data, long created_at_count)
        {
            _data = data;
            _created_at_count = created_at_count;
        }

        public float GetFinishTime()
        {
            return 2.0f;
        }
        public float GetFadeOutTime()
        {
            return 1.5f;
        }
        public float GetFadeInTime()
        {
            return 0.25f;
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
            if (counter_value < _created_at_count)
                return 999.0f;

            return ((float)(counter_value - _created_at_count)) / 40.0f;
        }

        public bool IsFinished(long counter_value)
        {
            if (counter_value < _created_at_count)
                return true;

            float vsecs = GetVSecs(counter_value);
            if (vsecs >= GetFinishTime())
                return true;
            else
                return false;
        }

        public bool IsSubTextAbove
        {
            get
            {
                switch (_data.style)
                {
                    case 1:
                    case 2:
                        return true;
                    default:
                        break;
                }
                return false;
            }
        }

        public string GetText(long counter_value)
        {
            return _data.text;
        }
        public string GetSubText(long counter_value)
        {
            return _data.subtext;
        }

        public float GetMainTextSizeRelativeToScreenWidth(long counter_value)
        {
            switch (_data.style)
            {
                case 1:
                    return 0.85f;
                default:
                    break;
            }
            return 0.85f;
        }
        public float GetMainTextMaxFontSize(long counter_value)
        {
            return 48.0f;
        }

        public float GetSubTextSizeRelativeToMainText(long counter_value)
        {
            return 0.5f;
        }
        public SKTypeface GetTextTypeface(long counter_value)
        {
            return App.ImmortalTypeface;
        }

        public SKTypeface GetSubTextTypeface(long counter_value)
        {
            return App.ImmortalTypeface;
        }

        public static SKColor TransparentGold = new SKColor(255, 255, 0xD7, 192);
        public static SKColor TransparentWhite = new SKColor(255, 255, 255, 192);

        public SKColor GetTextBaseColor(long counter_value)
        {
            return TransparentGold;
        }

        public SKColor GetSubTextBaseColor(long counter_value)
        {
            return TransparentWhite;
        }

        private SKColor GetColor(SKColor baseclr, long counter_value)
        {
            float vsecs = GetVSecs(counter_value);
            if (vsecs < GetFadeInTime())
            {
                byte val = (byte)(baseclr.Alpha * Math.Max(0.0f, Math.Min(1.0f, (vsecs) / GetFadeInLength())));
                SKColor clr = new SKColor(baseclr.Red, baseclr.Green, baseclr.Blue, val);
                return clr;
            }
            else if (IsFinished(counter_value))
                return SKColors.Transparent;
            else if (vsecs >= GetFadeOutTime())
            {
                byte val = (byte)(baseclr.Alpha * Math.Max(0.0f, Math.Min(1.0f, (GetFinishTime() - vsecs) / GetFadeOutLength())));
                SKColor clr = new SKColor(baseclr.Red, baseclr.Green, baseclr.Blue, val);
                return clr;
            }
            else
                return baseclr;
        }

        public SKColor GetTextColor(long counter_value)
        {
            return GetColor(GetTextBaseColor(counter_value), counter_value);
        }

        public SKColor GetSubTextColor(long counter_value)
        {
            return GetColor(GetSubTextBaseColor(counter_value), counter_value);
        }
        public float GetTextOutlineWidth(long counter_value)
        {
            return 0.0f;
        }
        public float GetSubTextOutlineWidth(long counter_value)
        {
            return 0.0f;
        }
        public SKColor GetTextOutlineColor(long counter_value)
        {
            return SKColors.Black;
        }
        public SKColor GetSubTextOutlineColor(long counter_value)
        {
            return SKColors.Black;
        }
    }
}
