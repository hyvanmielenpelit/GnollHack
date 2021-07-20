using System;
using System.Collections.Generic;
using System.Text;
using GnollHackCommon;
using SkiaSharp;

namespace GnollHackClient
{
    public class GHScreenText
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
            if (_data.style >= 3)
                return 20.0f;
            else
                return 2.0f;
        }
        public float GetFadeOutTime()
        {
            if (_data.style >= 3)
                return 19.0f;
            else
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
            switch(_data.style)
            {
                case 0:
                case 1:
                case 2:
                    return 0.85f;
                case 3:
                    return 0.60f;
                default:
                    return 0.70f;
            }
        }
        public float GetYOffsetPctOfScreen(long counter_value)
        {
            switch (_data.style)
            {
                case 0:
                case 1:
                case 2:
                    return 0.0f;
                case 3:
                    return 0.0f;
                default:
                    return -0.05f;
            }
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
            switch(_data.style)
            {
                case 0:
                    return ClientUtils.NHColor2SKColor((nhcolor)_data.color);
                case 1:
                    return TransparentGold;
                case 2:
                    return TransparentGold;
                case 3:
                    return SKColors.Beige;
                case 4:
                    return SKColors.Red;
                case 5:
                    return SKColors.White;
                case 6:
                    return SKColors.LightBlue;
                case 7:
                    return SKColors.Pink;
                default:
                    break;
            }
            return TransparentGold;
        }

        public SKColor GetSubTextBaseColor(long counter_value)
        {
            return TransparentWhite;
        }

        private SKColor GetTimedColor(SKColor baseclr, long counter_value)
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
            return GetTimedColor(GetTextBaseColor(counter_value), counter_value);
        }

        public SKColor GetSubTextColor(long counter_value)
        {
            return GetTimedColor(GetSubTextBaseColor(counter_value), counter_value);
        }
        public float GetRelativeTextOutlineWidth(long counter_value)
        {
            return 1f / 48f;
        }
        public float GetRelativeSubTextOutlineWidth(long counter_value)
        {
            return 1f / 48f;
        }
        public SKColor GetTextOutlineBaseColor(long counter_value)
        {
            return SKColors.Black;
        }
        public SKColor GetSubTextOutlineBaseColor(long counter_value)
        {
            return SKColors.Black;
        }
        public SKColor GetTextOutlineColor(long counter_value)
        {
            return GetTimedColor(GetTextOutlineBaseColor(counter_value), counter_value);
        }
        public SKColor GetSubTextOutlineColor(long counter_value)
        {
            return GetTimedColor(GetSubTextOutlineBaseColor(counter_value), counter_value);
        }
    }
}
