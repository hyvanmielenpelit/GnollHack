using System;
using System.Collections.Generic;
using System.Text;
#if GNH_MAUI
using GnollHackM;
#endif

namespace GnollHackX
{
    public class GHAnimationTimerList
    {
        public bool u_action_animation_counter_on;
        public bool m_action_animation_counter_on;
        public bool explosion_animation_counter_on;
        public bool[] zap_animation_counter_on = new bool[GHConstants.MaxPlayedZapAnimations];
        public bool[] special_effect_animation_counter_on = new bool[GHConstants.MaxPlayedSpecialEffects];

        public long general_animation_counter;
        public long u_action_animation_counter;
        public long m_action_animation_counter;
        public long explosion_animation_counter;
        public long[] zap_animation_counter = new long[GHConstants.MaxPlayedZapAnimations];
        public long[] special_effect_animation_counter = new long[GHConstants.MaxPlayedSpecialEffects];

        public byte m_action_animation_x;
        public byte m_action_animation_y;
        public byte explosion_animation_x;
        public byte explosion_animation_y;
        public byte[] zap_animation_x = new byte[GHConstants.MaxPlayedZapAnimations];
        public byte[] zap_animation_y = new byte[GHConstants.MaxPlayedZapAnimations];
        public byte[] spef_action_animation_x = new byte[GHConstants.MaxPlayedSpecialEffects];
        public byte[] spef_action_animation_y = new byte[GHConstants.MaxPlayedSpecialEffects];
        public layer_types[] spef_action_animation_layer = new layer_types[GHConstants.MaxPlayedSpecialEffects];

        public GHAnimationTimerList()
        {

        }
    }
}
