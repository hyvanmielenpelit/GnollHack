﻿using System;
using System.Collections.Generic;
using System.Text;
using SkiaSharp;
using GnollHackCommon;

namespace GnollHackClient
{
    public class GHFloatingText
    {
        private DisplayFloatingTextData _data;
        public long _created_at_count;

        public GHFloatingText(DisplayFloatingTextData data, long created_at_count)
        {
            _data = data;
            _created_at_count = created_at_count;
        }

        public SKPoint GetInitialPoint()
        {
            return new SKPoint(0.5f, 0.25f);
        }
        public SKPoint GetVelocity(long counter_value)
        {
            return new SKPoint(0.0f, -2.0f);
        }
        public float GetFinishTime()
        {
            return 0.75f;
        }
        public float GetFadeOutTime()
        {
            return 0.5f;
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
        public SKPoint GetPosition(long counter_value)
        {
            SKPoint i = GetInitialPoint();
            SKPoint v = GetVelocity(counter_value);
            float vsecs = GetVSecs(counter_value);
            SKPoint p = new SKPoint();
            p.X = _data.x + i.X + v.X * vsecs;
            p.Y = _data.y + i.Y + v.Y * vsecs;
            return p;
        }
        public string GetText(long counter_value)
        {
            return _data.text;
        }
        public float GetRelativeTextSize(long counter_value)
        {
            switch (_data.style)
            {
                case 1:
                case 2:
                case 5:
                case 6:
                    return 0.75f;
               default:
                    break;
            }
            return 0.6f;
        }
        public SKTypeface GetTypeface(long counter_value)
        {
            return App.LatoBold;
        }
        public SKColor GetBaseColor(long counter_value)
        {
            switch(_data.style)
            {
                case 0:
                    return ClientUtils.NHColor2SKColor((nhcolor)_data.color);
                case 1:
                    return SKColors.Green;
                case 2:
                    return SKColors.Red;
                case 3:
                    return SKColors.Gold;
                case 4:
                    return SKColors.Orange;
                case 5:
                    return SKColors.Blue;
                case 6:
                    return SKColors.Magenta;
                case 7:
                    return SKColors.LightGreen;
                case 8:
                    return SKColors.LightPink;
                case 9:
                    return SKColors.Cyan;
                case 10:
                    return SKColors.Crimson;
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
            if (vsecs < GetFadeInTime())
            {
                byte val = (byte)(baseclr.Alpha * Math.Max(0.0f, Math.Min(1.0f, (vsecs) / GetFadeInLength())));
                SKColor clr =  new SKColor(baseclr.Red, baseclr.Green, baseclr.Blue, val);
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
        public float GetRelativeOutlineWidth(long counter_value)
        {
            return 1f / 48f;
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
