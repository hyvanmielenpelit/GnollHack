using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;
#if GNH_MAUI
using GnollHackM;
#endif

namespace GnollHackX
{
    public sealed class GHAnimationTimerList
    {
        public long general_animation_counter; /* This is special and uses Interlocked */

        public bool u_action_animation_counter_on;
        public bool m_action_animation_counter_on;
        public bool explosion_animation_counter_on;
        public bool[] zap_animation_counter_on = new bool[GHConstants.MaxPlayedZapAnimations];
        public bool[] special_effect_animation_counter_on = new bool[GHConstants.MaxPlayedSpecialEffects];

        public long u_action_animation_counter;
        public long m_action_animation_counter;
        public long explosion_animation_counter;
        public long[] zap_animation_counter = new long[GHConstants.MaxPlayedZapAnimations];
        public long[] special_effect_animation_counter = new long[GHConstants.MaxPlayedSpecialEffects];

        public long u_action_animation_counter_timestamp;
        public long m_action_animation_counter_timestamp;
        public long explosion_animation_counter_timestamp;
        public long[] zap_animation_counter_timestamp = new long[GHConstants.MaxPlayedZapAnimations];
        public long[] special_effect_animation_counter_timestamp = new long[GHConstants.MaxPlayedSpecialEffects];

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

        public void CalculateCounterValues(long counter)
        {
            if (u_action_animation_counter_on)
                u_action_animation_counter = counter - u_action_animation_counter_timestamp;
            if (m_action_animation_counter_on)
                m_action_animation_counter = counter - m_action_animation_counter_timestamp;
            if (explosion_animation_counter_on)
                explosion_animation_counter = counter - explosion_animation_counter_timestamp;
            for (int i = 0; i < zap_animation_counter.Length; i++)
            {
                if (zap_animation_counter_on[i])
                    zap_animation_counter[i] = counter - zap_animation_counter_timestamp[i];
            }
            for (int i = 0; i < special_effect_animation_counter.Length; i++)
            {
                if (special_effect_animation_counter_on[i])
                    special_effect_animation_counter[i] = counter - special_effect_animation_counter_timestamp[i];
            }
        }

        public const long CounterMaxValue = long.MaxValue;
        public void CalculateCounterTimeStampsOnOverflowToZero()
        {
            if (u_action_animation_counter_timestamp > 0L)
            {
                u_action_animation_counter = 0L;
                u_action_animation_counter_timestamp = u_action_animation_counter_timestamp - CounterMaxValue; /* Use negative timestamp upon overflow */
            }
            if (m_action_animation_counter_timestamp > 0L)
            {
                m_action_animation_counter = 0L;
                m_action_animation_counter_timestamp = m_action_animation_counter_timestamp - CounterMaxValue; /* Use negative timestamp upon overflow */
            }
            if (explosion_animation_counter_timestamp > 0L)
            {
                explosion_animation_counter = 0L;
                explosion_animation_counter_timestamp = explosion_animation_counter_timestamp - CounterMaxValue; /* Use negative timestamp upon overflow */
            }
            for (int i = 0; i < zap_animation_counter.Length; i++)
            {
                if (zap_animation_counter_timestamp[i] > 0L)
                {
                    zap_animation_counter[i] = 0L;
                    zap_animation_counter_timestamp[i] = zap_animation_counter_timestamp[i] - CounterMaxValue; /* Use negative timestamp upon overflow */
                }
            }
            for (int i = 0; i < special_effect_animation_counter.Length; i++)
            {
                if (special_effect_animation_counter_timestamp[i] > 0L)
                {
                    special_effect_animation_counter[i] = 0L;
                    special_effect_animation_counter_timestamp[i] = special_effect_animation_counter_timestamp[i] - CounterMaxValue; /* Use negative timestamp upon overflow */
                }
            }
        }

        //public GHAnimationTimerList Clone()
        //{
        //    GHAnimationTimerList clone = new GHAnimationTimerList();
        //    CopyTo(clone);
        //    return clone;
        //}
        public void CopyTo(GHAnimationTimerList clone, bool copyGeneral, bool copyCounterValues)
        {
            if (copyGeneral)
                Interlocked.Exchange(ref clone.general_animation_counter, Interlocked.CompareExchange(ref general_animation_counter, 0L, 0L));

            if (copyCounterValues)
            {
                clone.u_action_animation_counter = u_action_animation_counter;
                clone.m_action_animation_counter = m_action_animation_counter;
                clone.explosion_animation_counter = explosion_animation_counter;
                zap_animation_counter.CopyTo(clone.zap_animation_counter, 0);
                special_effect_animation_counter.CopyTo(clone.special_effect_animation_counter, 0);
            }

            clone.u_action_animation_counter_on = u_action_animation_counter_on;
            clone.m_action_animation_counter_on = m_action_animation_counter_on;
            clone.explosion_animation_counter_on = explosion_animation_counter_on;
            zap_animation_counter_on.CopyTo(clone.zap_animation_counter_on, 0);
            special_effect_animation_counter_on.CopyTo(clone.special_effect_animation_counter_on, 0);

            clone.u_action_animation_counter_timestamp = u_action_animation_counter_timestamp;
            clone.m_action_animation_counter_timestamp = m_action_animation_counter_timestamp;
            clone.explosion_animation_counter_timestamp = explosion_animation_counter_timestamp;
            zap_animation_counter_timestamp.CopyTo(clone.zap_animation_counter_timestamp, 0);
            special_effect_animation_counter_timestamp.CopyTo(clone.special_effect_animation_counter_timestamp, 0);

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
