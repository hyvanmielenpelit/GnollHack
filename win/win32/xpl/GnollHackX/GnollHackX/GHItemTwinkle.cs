using System;

#if GNH_MAUI
namespace GnollHackM
#else
namespace GnollHackX
#endif
{
    public enum ItemTwinkleMode
    {
        Off = 0,
        SmallItems = 1,
        SmallAndMediumItems = 2
    }

    public struct TileOpacity
    {
        public bool Valid;
        public int Area;                          /* opaque pixels in the measured source rectangle */
        public short MinX, MinY, MaxX, MaxY;      /* opaque bounds, relative to that rectangle */
    }

    /* Item twinkle policy: which floor items twinkle, their size tiers measured from the drawn art,
       and the per-item schedule and envelope. Free of UI types, so that the unit tests compile it. */
    public static class GHItemTwinkle
    {
        public const int SlotTicks = 360;            /* 6 s of 60 Hz GUI ticks */
        public const int JitterTicks = 180;          /* start lies in the first half of the slot: gaps 3-9 s */
        public const int KnownSlotTicks = 720;       /* known always-twinkle items: 12 s slots, gaps 6-18 s */
        public const int KnownJitterTicks = 360;
        public const long KnownCycleFlag = 1L << 62; /* marks known-rhythm cycles, which never equal normal ones */
        public const int DurationTicks = 24;         /* 0.4 s */
        public const float PeakFraction = 0.3f;
        public const int MaxTwinklesPerItem = 8;
        public const int MaxActiveTwinkles = 4;      /* backstop; the spacing below already keeps it to one */
        public const int MinStartSpacingTicks = 30;  /* at most two new twinkles a second on screen */
        public const byte TierNone = 0, TierSmall = 1, TierMedium = 2;
        public const int NativeTileWidth = 64;
        public const int SmallMaxArea = 260, SmallMaxLength = 40;    /* native tile pixels */
        public const int MediumMaxArea = 400, MediumMaxLength = 45;
        public const byte OpaqueAlpha = 64;

        /* True for the classes the engine creates with dknown 0: dknowns[] in src/mkobj.c, and coins,
           which are mergeable and so start at 0 too */
        public static bool DknownIsMeaningful(sbyte oclass)
        {
            switch ((obj_class_types)oclass)
            {
                case obj_class_types.COIN_CLASS:
                case obj_class_types.WAND_CLASS:
                case obj_class_types.RING_CLASS:
                case obj_class_types.POTION_CLASS:
                case obj_class_types.ARMOR_CLASS:
                case obj_class_types.MISCELLANEOUS_CLASS:
                case obj_class_types.REAGENT_CLASS:
                case obj_class_types.SCROLL_CLASS:
                case obj_class_types.GEM_CLASS:
                case obj_class_types.SPBOOK_CLASS:
                case obj_class_types.WEAPON_CLASS:
                case obj_class_types.TOOL_CLASS:
                case obj_class_types.FOOD_CLASS:
                    return true;
                default:
                    return false;
            }
        }

        /* Glass, crystal or gemstone (or unknown, as for a mimic without an object); gray stones and
           rocks are mineral and ores are metal */
        private static bool IsGemLikeMaterial(byte material)
        {
            switch ((obj_material_types)material)
            {
                case obj_material_types.MAT_NONE:
                case obj_material_types.MAT_GLASS:
                case obj_material_types.MAT_CRYSTAL:
                case obj_material_types.MAT_HARD_CRYSTAL:
                case obj_material_types.MAT_GEMSTONE:
                    return true;
                default:
                    return false;
            }
        }

        /* Precious metals and crystal; an item's material shows in how it looks */
        public static bool IsAlwaysTwinkleMaterial(byte material)
        {
            switch ((obj_material_types)material)
            {
                case obj_material_types.MAT_GOLD:
                case obj_material_types.MAT_PLATINUM:
                case obj_material_types.MAT_ORICHALCUM:
                case obj_material_types.MAT_MITHRIL:
                case obj_material_types.MAT_ADAMANTIUM:
                case obj_material_types.MAT_CRYSTAL:
                case obj_material_types.MAT_HARD_CRYSTAL:
                    return true;
                default:
                    return false;
            }
        }

        /* Elemental, mythic or exceptional; the same fields the map's item property marks show */
        public static bool IsSpecialQuality(byte elementalEnchantment, byte mythicPrefix, byte mythicSuffix, byte exceptionality)
        {
            return elementalEnchantment != 0 || mythicPrefix != 0 || mythicSuffix != 0 || exceptionality != 0;
        }

        public static bool IsAlwaysTwinkle(sbyte oclass, byte material)
        {
            return IsAlwaysTwinkle(oclass, material, false);
        }

        /* Wands, rings, amulets, coins, gems and special items twinkle whatever their dknown, more
           slowly once known */
        public static bool IsAlwaysTwinkle(sbyte oclass, byte material, bool special)
        {
            if (special)
                return true;

            switch ((obj_class_types)oclass)
            {
                case obj_class_types.WAND_CLASS:
                case obj_class_types.RING_CLASS:
                case obj_class_types.AMULET_CLASS:
                case obj_class_types.COIN_CLASS:
                    return true;
                case obj_class_types.GEM_CLASS:
                    return IsGemLikeMaterial(material);
                default:
                    return false;
            }
        }

        public static bool IsEligible(sbyte oclass, byte material, bool hallucinated, bool dknown, bool isUChainOrUBall)
        {
            return IsEligible(oclass, material, hallucinated, dknown, isUChainOrUBall, false);
        }

        public static bool IsEligible(sbyte oclass, byte material, bool hallucinated, bool dknown, bool isUChainOrUBall, bool special)
        {
            if (hallucinated || isUChainOrUBall)
                return false;
            if (IsAlwaysTwinkle(oclass, material, special))
                return true;
            if (dknown && DknownIsMeaningful(oclass))
                return false;

            switch ((obj_class_types)oclass)
            {
                case obj_class_types.FOOD_CLASS:
                case obj_class_types.COIN_CLASS:
                case obj_class_types.ROCK_CLASS:
                case obj_class_types.BALL_CLASS:
                case obj_class_types.CHAIN_CLASS:
                case obj_class_types.VENOM_CLASS:
                case obj_class_types.ILLOBJ_CLASS:
                case obj_class_types.RANDOM_CLASS:
                    return false;
                case obj_class_types.GEM_CLASS:
                    return IsGemLikeMaterial(material);
                default:
                    return true;
            }
        }

        /* Small by class, used only when a tile sheet's pixels cannot be read */
        public static bool IsFallbackSmall(sbyte oclass)
        {
            switch ((obj_class_types)oclass)
            {
                case obj_class_types.RING_CLASS:
                case obj_class_types.AMULET_CLASS:
                case obj_class_types.WAND_CLASS:
                case obj_class_types.GEM_CLASS:
                    return true;
                default:
                    return false;
            }
        }

        /* Area and longest side of the opaque art, in native tile pixels */
        public static byte GetSizeTier(float nativeArea, float nativeLength)
        {
            if (nativeArea <= SmallMaxArea && nativeLength <= SmallMaxLength)
                return TierSmall;
            if (nativeArea <= MediumMaxArea && nativeLength <= MediumMaxLength)
                return TierMedium;
            return TierNone;
        }

        /* mode is an ItemTwinkleMode value */
        public static bool IsShown(byte tier, int mode)
        {
            return tier != TierNone && tier <= mode;
        }

        /* Counts the pixels of a rectangle of a 32-bit pixel buffer whose alpha byte is at offset 3
           (Rgba8888 and Bgra8888) with alpha at least OpaqueAlpha, and their bounds */
        public static unsafe TileOpacity MeasureOpaque(byte* pixels, int rowBytes, int x0, int y0, int width, int height)
        {
            TileOpacity res = new TileOpacity();
            int area = 0;
            int minX = int.MaxValue, minY = int.MaxValue, maxX = -1, maxY = -1;
            int x, y;
            for (y = 0; y < height; y++)
            {
                byte* row = pixels + (long)(y0 + y) * rowBytes + (long)x0 * 4;
                for (x = 0; x < width; x++)
                {
                    if (row[x * 4 + 3] >= OpaqueAlpha)
                    {
                        area++;
                        if (x < minX)
                            minX = x;
                        if (x > maxX)
                            maxX = x;
                        if (y < minY)
                            minY = y;
                        if (y > maxY)
                            maxY = y;
                    }
                }
            }

            if (area > 0)
            {
                res.Valid = true;
                res.Area = area;
                res.MinX = (short)minX;
                res.MinY = (short)minY;
                res.MaxX = (short)maxX;
                res.MaxY = (short)maxY;
            }
            return res;
        }

        /* The object id, or the map position for an object without one */
        public static uint GetKey(uint oid, int x, int y)
        {
            if (oid != 0)
                return oid;
            return 0x80000000u | ((uint)x << 8) | (uint)y;
        }

        public static uint Hash(uint a, uint b)
        {
            unchecked
            {
                uint x = a ^ (b * 0x9e3779b9u);
                x ^= x >> 16;
                x *= 0x7feb352du;
                x ^= x >> 15;
                x *= 0x846ca68bu;
                x ^= x >> 16;
                x ^= x >> 16;
                x *= 0x7feb352du;
                x ^= x >> 15;
                x *= 0x846ca68bu;
                x ^= x >> 16;
                return x;
            }
        }

        /* Each key has one twinkle per slot of SlotTicks, starting within the first JitterTicks of the
           slot; the slots of different keys are offset from each other */
        public static bool TryGetActiveTwinkle(uint key, long tick, out long cycle, out int phaseTick)
        {
            return TryGetActiveTwinkle(key, tick, false, out cycle, out phaseTick);
        }

        /* slow selects the known rhythm, whose cycles carry KnownCycleFlag */
        public static bool TryGetActiveTwinkle(uint key, long tick, bool slow, out long cycle, out int phaseTick)
        {
            int slotTicks = slow ? KnownSlotTicks : SlotTicks;
            int jitterTicks = slow ? KnownJitterTicks : JitterTicks;
            long shifted = tick + Hash(key, slow ? 2u : 1u) % (uint)slotTicks;
            long slotCycle = shifted / slotTicks;
            int start = (int)(Hash(key, (uint)slotCycle) % (uint)jitterTicks);
            phaseTick = (int)(shifted % slotTicks) - start;
            cycle = slow ? (slotCycle | KnownCycleFlag) : slotCycle;
            return phaseTick >= 0 && phaseTick < DurationTicks;
        }

        /* Counts twinkles up to MaxTwinklesPerItem and stays there */
        public static byte IncrementCount(byte count)
        {
            return count < MaxTwinklesPerItem ? (byte)(count + 1) : (byte)MaxTwinklesPerItem;
        }

        /* True within the first ticks of a twinkle: at least two, and at least one frame's worth */
        public static bool IsAdmissionPhase(int phaseTick, double ticksPerFrame)
        {
            return phaseTick <= Math.Max(2, (int)Math.Ceiling(ticksPerFrame));
        }

        /* Rises quadratically to 1 at PeakFraction of the duration, then falls quadratically to 0 */
        public static float GetEnvelope(int phaseTick)
        {
            if (phaseTick < 0 || phaseTick >= DurationTicks)
                return 0f;

            float u = (phaseTick + 0.5f) / DurationTicks;
            float value;
            if (u < PeakFraction)
            {
                float a = 1f - u / PeakFraction;
                value = 1f - a * a;
            }
            else
            {
                float b = (u - PeakFraction) / (1f - PeakFraction);
                value = 1f - b * b;
            }
            return Math.Min(Math.Max(value, 0f), 1f);
        }

        public static float GetRotationDegrees(int phaseTick)
        {
            return -20f + 40f * phaseTick / (DurationTicks - 1);
        }

        /* Fractions of the opaque bounds, toward the upper right, jittered by up to 0.12 per cycle */
        public static void GetAnchor(uint key, long cycle, out float fx, out float fy)
        {
            uint h = Hash(key, (uint)cycle ^ 0x5bd1e995u);
            float j1 = ((h & 0xFFFFu) / 65535f - 0.5f) * 0.24f;
            float j2 = ((h >> 16) / 65535f - 0.5f) * 0.24f;
            fx = 0.62f + j1;
            fy = 0.30f + j2;
        }

        public static float GetPeakRadius(float opaqueWidth, float tileWidth)
        {
            return Math.Min(Math.Max(0.6f * opaqueWidth, 0.16f * tileWidth), 0.30f * tileWidth);
        }
    }
}
