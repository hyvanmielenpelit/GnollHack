using System;
using System.Collections.Generic;
using System.Text;
#if GNH_MAUI
using GnollHackM;
#else
using GnollHackX.Pages.Game;
#endif
using SkiaSharp;

namespace GnollHackX
{
    public class GHScreenText
    {
        private DisplayScreenTextData _data;
        public long _created_at_count;
        private GamePage _gamePage;
        private int _animationFrequency;
        public bool HasSuperText { get { return _data.supertext != null; } }
        public bool HasSubText { get { return _data.subtext != null; } }

        public GHScreenText(DisplayScreenTextData data, long created_at_count, GamePage gamePage)
        {
            _data = data;
            _created_at_count = created_at_count;
            _gamePage = gamePage;
            _animationFrequency = Math.Max(1, UIUtils.GetMainCanvasAnimationFrequency(_gamePage.MapRefreshRate));
        }

        public float GetFinishTime()
        {
            switch(_data.style)
            {
                case (int)screen_text_types.SCREEN_TEXT_DEAD:
                case (int)screen_text_types.SCREEN_TEXT_ESCAPED:
                case (int)screen_text_types.SCREEN_TEXT_ASCENDED:
                case (int)screen_text_types.SCREEN_TEXT_QUIT:
                case (int)screen_text_types.SCREEN_TEXT_SPECIAL_END:
                    return 99999.0f;
                case (int)screen_text_types.SCREEN_TEXT_SAVING:
                    return 1.5f;
                case (int)screen_text_types.SCREEN_TEXT_BOSS_FIGHT:
                    return 2.0f;
                default:
                    break;
            }
            return 2.0f;
        }
        public float GetFadeOutTime()
        {
            switch (_data.style)
            {
                case (int)screen_text_types.SCREEN_TEXT_DEAD:
                case (int)screen_text_types.SCREEN_TEXT_ESCAPED:
                case (int)screen_text_types.SCREEN_TEXT_ASCENDED:
                case (int)screen_text_types.SCREEN_TEXT_QUIT:
                case (int)screen_text_types.SCREEN_TEXT_SPECIAL_END:
                    return 9999.0f;
                case (int)screen_text_types.SCREEN_TEXT_SAVING:
                    return 1.0f;
                case (int)screen_text_types.SCREEN_TEXT_BOSS_FIGHT:
                    return 1.5f;
            }
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

            return ((float)(counter_value - _created_at_count)) / (float)_animationFrequency;
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

        public string GetText(long counter_value)
        {
            return _data.text;
        }
        public string GetSuperText(long counter_value)
        {
            return _data.supertext;
        }
        public string GetSubText(long counter_value)
        {
            return _data.subtext;
        }

        public float GetMainTextSizeRelativeToScreenWidth(long counter_value)
        {
            switch(_data.style)
            {
                case (int)screen_text_types.SCREEN_TEXT_GENERAL:
                case (int)screen_text_types.SCREEN_TEXT_ENTER_DUNGEON_LEVEL:
                case (int)screen_text_types.SCREEN_TEXT_BOSS_FIGHT:
                    return 0.65f;
                case (int)screen_text_types.SCREEN_TEXT_GAIN_LEVEL:
                    return 0.70f;
                case (int)screen_text_types.SCREEN_TEXT_SAVING:
                    return 0.60f;
                case (int)screen_text_types.SCREEN_TEXT_DEAD:
                case (int)screen_text_types.SCREEN_TEXT_ESCAPED:
                case (int)screen_text_types.SCREEN_TEXT_ASCENDED:
                case (int)screen_text_types.SCREEN_TEXT_QUIT:
                case (int)screen_text_types.SCREEN_TEXT_SPECIAL_END:
                    return 0.60f;
                default:
                    return 0.70f;
            }
        }
        public float GetYOffsetPctOfScreen(long counter_value)
        {
            switch (_data.style)
            {
                case (int)screen_text_types.SCREEN_TEXT_GENERAL:
                case (int)screen_text_types.SCREEN_TEXT_ENTER_DUNGEON_LEVEL:
                case (int)screen_text_types.SCREEN_TEXT_GAIN_LEVEL:
                    return 0.0f;
                case (int)screen_text_types.SCREEN_TEXT_SAVING:
                    return 0.0f;
                case (int)screen_text_types.SCREEN_TEXT_DEAD:
                case (int)screen_text_types.SCREEN_TEXT_ESCAPED:
                case (int)screen_text_types.SCREEN_TEXT_ASCENDED:
                case (int)screen_text_types.SCREEN_TEXT_QUIT:
                case (int)screen_text_types.SCREEN_TEXT_SPECIAL_END:
                    return -0.20f;
                case (int)screen_text_types.SCREEN_TEXT_BOSS_FIGHT:
                    return -0.20f;
                default:
                    return -0.02f;
            }
        }
        public float GetMainTextMaxFontSize(long counter_value)
        {
            return 48.0f;
        }
        public float GetSuperTextSizeRelativeToMainText(long counter_value)
        {
            int maintextlen = _data.text != null ? _data.text.Length : 0;
            int supertextlen = _data.supertext != null ? _data.supertext.Length : 0;
            if (supertextlen == 0 || maintextlen == 0)
                return 1.0f;

            float relsize = Math.Min(0.75f, 0.75f * (float)maintextlen / (float)supertextlen);
            return relsize;
        }

        public float GetSubTextSizeRelativeToMainText(long counter_value)
        {
            int maintextlen = _data.text != null ? _data.text.Length : 0;
            int subtextlen = _data.subtext != null ? _data.subtext.Length : 0;
            if (subtextlen == 0 || maintextlen == 0)
                return 1.0f;

            float relsize = Math.Min(0.67f, 0.67f * (float)maintextlen / (float)subtextlen);
            return relsize;
        }
        public SKTypeface GetTextTypeface(long counter_value)
        {
            return GHApp.ImmortalTypeface;
        }

        public SKTypeface GetSuperTextTypeface(long counter_value)
        {
            return GHApp.ImmortalTypeface;
        }
        public SKTypeface GetSubTextTypeface(long counter_value)
        {
            return GHApp.ImmortalTypeface;
        }

        public static SKColor TransparentGold = new SKColor(255, 255, 0xD7, 180);
        public static SKColor TransparentWhite = new SKColor(255, 255, 255, 180);
        public static SKColor TransparentGray = new SKColor(208, 208, 192, 180);

        public SKColor GetTextBaseColor(long counter_value)
        {
            switch(_data.style)
            {
                case (int)screen_text_types.SCREEN_TEXT_GENERAL:
                    return UIUtils.NHColor2SKColor(_data.color, _data.attr);
                case (int)screen_text_types.SCREEN_TEXT_ENTER_DUNGEON_LEVEL:
                    return TransparentGold;
                case (int)screen_text_types.SCREEN_TEXT_GAIN_LEVEL:
                    return TransparentGold;
                case (int)screen_text_types.SCREEN_TEXT_SAVING:
                    return SKColors.Beige;
                case (int)screen_text_types.SCREEN_TEXT_QUIT:
                case (int)screen_text_types.SCREEN_TEXT_DEAD:
                    return SKColors.Red;
                case (int)screen_text_types.SCREEN_TEXT_ESCAPED:
                    return SKColors.White;
                case (int)screen_text_types.SCREEN_TEXT_ASCENDED:
                    return SKColors.LightBlue;
                case (int)screen_text_types.SCREEN_TEXT_SPECIAL_END:
                    return SKColors.Pink;
                case (int)screen_text_types.SCREEN_TEXT_BOSS_FIGHT:
                    return SKColors.Yellow;
                case (int)screen_text_types.SCREEN_TEXT_EXTRA_LIFE_SPENT:
                    return SKColors.Green;
                default:
                    break;
            }
            return TransparentGold;
        }

        public SKColor GetSuperTextBaseColor(long counter_value)
        {
            return TransparentWhite;
        }
        public SKColor GetSubTextBaseColor(long counter_value)
        {
            return TransparentGold;
        }

        private SKColor GetTimedColor(SKColor baseclr, long counter_value)
        {
            float vsecs = GetVSecs(counter_value);
            if (vsecs < GetFadeInTime())
            {
                byte val = (byte)((float)baseclr.Alpha * Math.Max(0.0f, Math.Min(1.0f, (vsecs) / GetFadeInLength())));
                SKColor clr = new SKColor(baseclr.Red, baseclr.Green, baseclr.Blue, val);
                return clr;
            }
            else if (IsFinished(counter_value))
                return SKColors.Transparent;
            else if (vsecs >= GetFadeOutTime())
            {
                byte val = (byte)((float)baseclr.Alpha * Math.Max(0.0f, Math.Min(1.0f, (GetFinishTime() - vsecs) / GetFadeOutLength())));
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

        public SKColor GetSuperTextColor(long counter_value)
        {
            return GetTimedColor(GetSuperTextBaseColor(counter_value), counter_value);
        }
        public SKColor GetSubTextColor(long counter_value)
        {
            return GetTimedColor(GetSubTextBaseColor(counter_value), counter_value);
        }
        public float GetRelativeTextOutlineWidth(long counter_value)
        {
            return 1f / 15f;
        }
        public float GetRelativeSuperTextOutlineWidth(long counter_value)
        {
            return 1f / 15f;
        }
        public float GetRelativeSubTextOutlineWidth(long counter_value)
        {
            return 1f / 15f;
        }
        public SKColor GetTextOutlineBaseColor(long counter_value)
        {
            return SKColors.Black;
        }
        public SKColor GetSuperTextOutlineBaseColor(long counter_value)
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
        public SKColor GetSuperTextOutlineColor(long counter_value)
        {
            return GetTimedColor(GetSuperTextOutlineBaseColor(counter_value), counter_value);
        }
        public SKColor GetSubTextOutlineColor(long counter_value)
        {
            return GetTimedColor(GetSubTextOutlineBaseColor(counter_value), counter_value);
        }
    }
}
