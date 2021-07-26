using System;
using System.Collections.Generic;
using System.Text;
using GnollHackClient.Pages.Game;
using GnollHackCommon;
using SkiaSharp;
using SkiaSharp.Views.Forms;
using Xamarin.Forms;

namespace GnollHackClient
{

    class GHCanvasView : SKCanvasView
    {

        public GHCanvasView() : base()
        {

        }

        public GamePage _gamePage = null;

        public BindableProperty GeneralAnimationCounterProperty =
            BindableProperty.Create(nameof(GeneralAnimationCounter), typeof(long), typeof(GHCanvasView), 0L);

        public long GeneralAnimationCounter
        {
            get { return (long)GetValue(GeneralAnimationCounterProperty); }
            set { SetValue(GeneralAnimationCounterProperty, value); }
        }


        protected override void OnPropertyChanged(string propertyName = null)
        {
            base.OnPropertyChanged(propertyName);

            if(_gamePage != null)
            {
                bool refresh = true;
                lock (_gamePage.RefreshScreenLock)
                {
                    refresh = _gamePage.RefreshScreen;
                }
                if (refresh)
                    InvalidateSurface();

                /* Increment counters */
                lock (_gamePage.AnimationTimerLock)
                {
                    _gamePage.AnimationTimers.general_animation_counter++;
                    if (_gamePage.AnimationTimers.general_animation_counter < 0)
                        _gamePage.AnimationTimers.general_animation_counter = 0;

                    if (_gamePage.AnimationTimers.u_action_animation_counter_on)
                    {
                        _gamePage.AnimationTimers.u_action_animation_counter++;
                        if (_gamePage.AnimationTimers.u_action_animation_counter < 0)
                            _gamePage.AnimationTimers.u_action_animation_counter = 0;
                    }

                    if (_gamePage.AnimationTimers.m_action_animation_counter_on)
                    {
                        _gamePage.AnimationTimers.m_action_animation_counter++;
                        if (_gamePage.AnimationTimers.m_action_animation_counter < 0)
                            _gamePage.AnimationTimers.m_action_animation_counter = 0;
                    }

                    if (_gamePage.AnimationTimers.explosion_animation_counter_on)
                    {
                        _gamePage.AnimationTimers.explosion_animation_counter++;
                        if (_gamePage.AnimationTimers.explosion_animation_counter < 0)
                            _gamePage.AnimationTimers.explosion_animation_counter = 0;
                    }

                    int i;
                    for (i = 0; i < GHConstants.MaxPlayedZapAnimations; i++)
                    {
                        if (_gamePage.AnimationTimers.zap_animation_counter_on[i])
                        {
                            _gamePage.AnimationTimers.zap_animation_counter[i]++;
                            if (_gamePage.AnimationTimers.zap_animation_counter[i] < 0)
                                _gamePage.AnimationTimers.zap_animation_counter[i] = 0;
                        }
                    }

                    for (i = 0; i < GHConstants.MaxPlayedSpecialEffects; i++)
                    {
                        if (_gamePage.AnimationTimers.special_effect_animation_counter_on[i])
                        {
                            _gamePage.AnimationTimers.special_effect_animation_counter[i]++;
                            if (_gamePage.AnimationTimers.special_effect_animation_counter[i] < 0)
                                _gamePage.AnimationTimers.special_effect_animation_counter[i] = 0;
                        }
                    }

                    lock (_gamePage._floatingTextLock)
                    {
                        for (i = _gamePage._floatingTexts.Count - 1; i >= 0; i--)
                        {
                            if (_gamePage._floatingTexts[i].IsFinished(_gamePage.AnimationTimers.general_animation_counter))
                                _gamePage._floatingTexts.RemoveAt(i);
                        }
                    }

                    lock (_gamePage._screenTextLock)
                    {
                        if (_gamePage._screenText != null && _gamePage._screenText.IsFinished(_gamePage.AnimationTimers.general_animation_counter))
                            _gamePage._screenText = null;
                    }

                    lock (_gamePage.TargetClipLock)
                    {
                        if (_gamePage.AnimationTimers.general_animation_counter < _gamePage._targetClipStartCounterValue
                            || _gamePage.AnimationTimers.general_animation_counter > _gamePage._targetClipStartCounterValue + _gamePage._targetClipPanTime)
                            _gamePage._targetClipOn = false;

                        if (_gamePage._targetClipOn)
                        {
                            lock (_gamePage.MapOffsetLock)
                            {
                                _gamePage._mapOffsetX = _gamePage._originMapOffsetWithNewClipX * Math.Max(0.0f, 1.0f - (float)(_gamePage.AnimationTimers.general_animation_counter - _gamePage._targetClipStartCounterValue) / (float)_gamePage._targetClipPanTime);
                                _gamePage._mapOffsetY = _gamePage._originMapOffsetWithNewClipY * Math.Max(0.0f, 1.0f - (float)(_gamePage.AnimationTimers.general_animation_counter - _gamePage._targetClipStartCounterValue) / (float)_gamePage._targetClipPanTime);
                            }
                        }
                    }
                }
            }
        }
    }
}
