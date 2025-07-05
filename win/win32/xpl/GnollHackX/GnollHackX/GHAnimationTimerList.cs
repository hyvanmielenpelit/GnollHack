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

        //public GHAnimationTimerList Clone()
        //{
        //    GHAnimationTimerList clone = new GHAnimationTimerList();
        //    CopyTo(clone);
        //    return clone;
        //}
        public void CopyTo(GHAnimationTimerList clone, bool copyGeneral)
        {
            clone.u_action_animation_counter_on = u_action_animation_counter_on;
            clone.m_action_animation_counter_on = m_action_animation_counter_on;
            clone.explosion_animation_counter_on = explosion_animation_counter_on;
            zap_animation_counter_on.CopyTo(clone.zap_animation_counter_on, 0);
            special_effect_animation_counter_on.CopyTo(clone.special_effect_animation_counter_on, 0);

            if (copyGeneral)
                Interlocked.Exchange(ref clone.general_animation_counter, Interlocked.CompareExchange(ref general_animation_counter, 0L, 0L));
            
            clone.u_action_animation_counter = u_action_animation_counter;
            clone.m_action_animation_counter = m_action_animation_counter;
            clone.explosion_animation_counter = explosion_animation_counter;
            zap_animation_counter.CopyTo(clone.zap_animation_counter, 0);
            special_effect_animation_counter.CopyTo(clone.special_effect_animation_counter, 0);

            clone.m_action_animation_x = m_action_animation_x;
            clone.m_action_animation_y = m_action_animation_y;
            clone.explosion_animation_x = explosion_animation_x;
            clone.explosion_animation_y = explosion_animation_y;
            zap_animation_x.CopyTo(clone.zap_animation_x, 0);
            zap_animation_y.CopyTo(clone.zap_animation_y, 0);
            spef_action_animation_x.CopyTo(clone.spef_action_animation_x, 0);
            spef_action_animation_y.CopyTo(clone.spef_action_animation_y, 0);
            spef_action_animation_layer.CopyTo(clone.spef_action_animation_layer, 0);
        }
    }
}
