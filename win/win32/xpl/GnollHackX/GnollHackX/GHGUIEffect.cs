using SkiaSharp;
using System;
using System.Collections.Generic;
using System.Text;
using System.Xml.Linq;
#if GNH_MAUI
using GnollHackM;
#else
using GnollHackX.Pages.Game;
#endif

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
            _animationFrequency = Math.Max(1, UIUtils.GetMainCanvasAnimationFrequency(_gamePage.MapRefreshRate));
        }

        public int Style { get { return _data.style; } }
        public int SubType { get { return _data.subtype; } }
        public int X { get { return _data.x; } }
        public int Y { get { return _data.y; } }
        public int X1 { get { return _data.x; } }
        public int Y1 { get { return _data.y; } }
        public int X2 { get { return _data.x2; } }
        public int Y2 { get { return _data.y2; } }
        public long CreatedAt { get { return _created_at_count; } }

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

        private static readonly SKColor GHWhiteMetalGray = new SKColor(220, 232, 244);
        private static readonly SKColor GHLightMetalGray = new SKColor(148, 154, 166);
        private static readonly SKColor GHMetalGray = new SKColor(109, 114, 124);
        private static readonly SKColor GHDarkMetalGray = new SKColor(56, 60, 68);
        private static readonly SKColor GHLightBrown = new SKColor(0x77, 0x5A, 0x2A);
        private static readonly SKColor GHBrown = new SKColor(0x66, 0x4A, 0x24);
        private static readonly SKColor GHDarkBrown = new SKColor(0x55, 0x3A, 0x1A);
        public SKColor GetBaseColor(long counter_value)
        {
            switch (_data.style)
            {
                case (int)gui_effect_types.GUI_EFFECT_SEARCH:
                case (int)gui_effect_types.GUI_EFFECT_WAIT:
                    return SKColors.White;
                case (int)gui_effect_types.GUI_EFFECT_POLEARM:
                    switch(_data.subtype)
                    {
                        case (int)gui_polearm_types.GUI_POLEARM_THRUSTED:
                        case (int)gui_polearm_types.GUI_POLEARM_POLEAXE:
                        case (int)gui_polearm_types.GUI_POLEARM_SPEAR:
                            return GHBrown;
                        case (int)gui_polearm_types.GUI_POLEARM_LANCE:
                            return GHMetalGray;
                        default:
                            return SKColors.White;
                    }
                default:
                    return SKColors.Red;
            }
        }
        public SKColor GetColor(long counter_value)
        {
            return GetTimedColor(GetBaseColor(counter_value), counter_value);
        }
        public SKColor GetBaseOutlineColor(long counter_value)
        {
            switch (_data.style)
            {
                case (int)gui_effect_types.GUI_EFFECT_SEARCH:
                case (int)gui_effect_types.GUI_EFFECT_WAIT:
                    return SKColors.White;
                case (int)gui_effect_types.GUI_EFFECT_POLEARM:
                    switch (_data.subtype)
                    {
                        case (int)gui_polearm_types.GUI_POLEARM_THRUSTED:
                        case (int)gui_polearm_types.GUI_POLEARM_POLEAXE:
                        case (int)gui_polearm_types.GUI_POLEARM_SPEAR:
                            return GHDarkBrown;
                        case (int)gui_polearm_types.GUI_POLEARM_LANCE:
                            return GHDarkMetalGray;
                        default:
                            return SKColors.Gray;
                    }
                default:
                    return SKColors.Red;
            }
        }
        public SKColor GetOutlineColor(long counter_value)
        {
            return GetTimedColor(GetBaseOutlineColor(counter_value), counter_value);
        }
        public SKColor GetBaseInnerColor(long counter_value)
        {
            switch (_data.style)
            {
                case (int)gui_effect_types.GUI_EFFECT_SEARCH:
                case (int)gui_effect_types.GUI_EFFECT_WAIT:
                    return SKColors.White;
                case (int)gui_effect_types.GUI_EFFECT_POLEARM:
                    switch (_data.subtype)
                    {
                        case (int)gui_polearm_types.GUI_POLEARM_THRUSTED:
                        case (int)gui_polearm_types.GUI_POLEARM_POLEAXE:
                        case (int)gui_polearm_types.GUI_POLEARM_SPEAR:
                            return GHLightBrown;
                        case (int)gui_polearm_types.GUI_POLEARM_LANCE:
                            return GHLightMetalGray;
                        default:
                            return SKColors.Gray;
                    }
                default:
                    return SKColors.Red;
            }
        }
        public SKColor GetInnerColor(long counter_value)
        {
            return GetTimedColor(GetBaseInnerColor(counter_value), counter_value);
        }
        public SKColor GetSecondaryBaseColor(long counter_value)
        {
            switch (_data.style)
            {
                case (int)gui_effect_types.GUI_EFFECT_SEARCH:
                case (int)gui_effect_types.GUI_EFFECT_WAIT:
                    return SKColors.White;
                case (int)gui_effect_types.GUI_EFFECT_POLEARM:
                    switch (_data.subtype)
                    {
                        case (int)gui_polearm_types.GUI_POLEARM_THRUSTED:
                        case (int)gui_polearm_types.GUI_POLEARM_POLEAXE:
                            return GHWhiteMetalGray;
                        case (int)gui_polearm_types.GUI_POLEARM_SPEAR:
                            return GHMetalGray;
                        case (int)gui_polearm_types.GUI_POLEARM_LANCE:
                            return GHBrown;
                        default:
                            return SKColors.White;
                    }
                default:
                    return SKColors.Red;
            }
        }
        public SKColor GetSecondaryColor(long counter_value)
        {
            return GetTimedColor(GetSecondaryBaseColor(counter_value), counter_value);
        }
        public SKColor GetSecondaryBaseOutlineColor(long counter_value)
        {
            switch (_data.style)
            {
                case (int)gui_effect_types.GUI_EFFECT_SEARCH:
                case (int)gui_effect_types.GUI_EFFECT_WAIT:
                    return SKColors.White;
                case (int)gui_effect_types.GUI_EFFECT_POLEARM:
                    switch (_data.subtype)
                    {
                        case (int)gui_polearm_types.GUI_POLEARM_THRUSTED:
                        case (int)gui_polearm_types.GUI_POLEARM_POLEAXE:
                            return GHDarkMetalGray;
                        case (int)gui_polearm_types.GUI_POLEARM_SPEAR:
                            return GHDarkMetalGray;
                        case (int)gui_polearm_types.GUI_POLEARM_LANCE:
                            return GHDarkBrown;
                        default:
                            return SKColors.White;
                    }
                default:
                    return SKColors.Crimson;
            }
        }
        public SKColor GetSecondaryOutlineColor(long counter_value)
        {
            return GetTimedColor(GetSecondaryBaseOutlineColor(counter_value), counter_value);
        }
        public SKColor GetSecondaryBaseInnerColor(long counter_value)
        {
            switch (_data.style)
            {
                case (int)gui_effect_types.GUI_EFFECT_SEARCH:
                case (int)gui_effect_types.GUI_EFFECT_WAIT:
                    return SKColors.White;
                case (int)gui_effect_types.GUI_EFFECT_POLEARM:
                    switch (_data.subtype)
                    {
                        case (int)gui_polearm_types.GUI_POLEARM_THRUSTED:
                        case (int)gui_polearm_types.GUI_POLEARM_POLEAXE:
                            return GHLightMetalGray;
                        case (int)gui_polearm_types.GUI_POLEARM_SPEAR:
                            return GHLightMetalGray;
                        case (int)gui_polearm_types.GUI_POLEARM_LANCE:
                            return GHLightBrown;
                        default:
                            return SKColors.White;
                    }
                default:
                    return SKColors.Crimson;
            }
        }
        public SKColor GetSecondaryInnerColor(long counter_value)
        {
            return GetTimedColor(GetSecondaryBaseInnerColor(counter_value), counter_value);
        }
        public SKColor GetSecondaryBaseInner2Color(long counter_value)
        {
            switch (_data.style)
            {
                case (int)gui_effect_types.GUI_EFFECT_SEARCH:
                case (int)gui_effect_types.GUI_EFFECT_WAIT:
                    return SKColors.White;
                case (int)gui_effect_types.GUI_EFFECT_POLEARM:
                    switch (_data.subtype)
                    {
                        case (int)gui_polearm_types.GUI_POLEARM_THRUSTED:
                        case (int)gui_polearm_types.GUI_POLEARM_POLEAXE:
                            return GHMetalGray;
                        default:
                            return SKColors.White;
                    }
                default:
                    return SKColors.Crimson;
            }
        }
        public SKColor GetSecondaryInner2Color(long counter_value)
        {
            return GetTimedColor(GetSecondaryBaseInner2Color(counter_value), counter_value);
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
