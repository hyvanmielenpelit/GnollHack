using System;
using GnollHackM;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers GHItemTwinkle: eligibility by class, material, dknown and hallucination, the size tiers
       against the measured calibration items, opaque-pixel measurement, the per-key schedule, the
       envelope, keys, the star radius, and that the per-frame calls allocate nothing. */
    public class GHItemTwinkleTests
    {
        [Theory]
        [InlineData(obj_class_types.RING_CLASS, obj_material_types.MAT_GOLD)]
        [InlineData(obj_class_types.AMULET_CLASS, obj_material_types.MAT_IRON)]
        [InlineData(obj_class_types.WAND_CLASS, obj_material_types.MAT_WOOD)]
        [InlineData(obj_class_types.ARMOR_CLASS, obj_material_types.MAT_LEATHER)]
        [InlineData(obj_class_types.TOOL_CLASS, obj_material_types.MAT_WOOD)]
        [InlineData(obj_class_types.POTION_CLASS, obj_material_types.MAT_GLASS)]
        [InlineData(obj_class_types.SCROLL_CLASS, obj_material_types.MAT_PAPER)]
        [InlineData(obj_class_types.GEM_CLASS, obj_material_types.MAT_GLASS)]
        [InlineData(obj_class_types.GEM_CLASS, obj_material_types.MAT_GEMSTONE)]
        [InlineData(obj_class_types.GEM_CLASS, obj_material_types.MAT_HARD_CRYSTAL)]
        [InlineData(obj_class_types.GEM_CLASS, obj_material_types.MAT_NONE)]
        public void IsEligible_UnexaminedItems_Twinkle(obj_class_types oclass, obj_material_types material)
        {
            Assert.True(GHItemTwinkle.IsEligible((sbyte)oclass, (byte)material, false, false, false));
        }

        [Theory]
        [InlineData(obj_class_types.GEM_CLASS, obj_material_types.MAT_MINERAL)]
        [InlineData(obj_class_types.GEM_CLASS, obj_material_types.MAT_IRON)]
        [InlineData(obj_class_types.FOOD_CLASS, obj_material_types.MAT_VEGGY)]
        [InlineData(obj_class_types.WEAPON_CLASS, obj_material_types.MAT_IRON)]
        [InlineData(obj_class_types.ROCK_CLASS, obj_material_types.MAT_MINERAL)]
        [InlineData(obj_class_types.BALL_CLASS, obj_material_types.MAT_IRON)]
        [InlineData(obj_class_types.CHAIN_CLASS, obj_material_types.MAT_IRON)]
        [InlineData(obj_class_types.VENOM_CLASS, obj_material_types.MAT_LIQUID)]
        public void IsEligible_ExcludedClassesAndMaterials_DoNotTwinkle(obj_class_types oclass, obj_material_types material)
        {
            Assert.False(GHItemTwinkle.IsEligible((sbyte)oclass, (byte)material, false, false, false));
        }

        [Theory]
        [InlineData(obj_class_types.WEAPON_CLASS, obj_material_types.MAT_IRON)]
        [InlineData(obj_class_types.POTION_CLASS, obj_material_types.MAT_GLASS)]
        [InlineData(obj_class_types.TOOL_CLASS, obj_material_types.MAT_WOOD)]
        [InlineData(obj_class_types.ARMOR_CLASS, obj_material_types.MAT_LEATHER)]
        [InlineData(obj_class_types.SCROLL_CLASS, obj_material_types.MAT_PAPER)]
        public void IsEligible_Dknown_StopsTwinkling(obj_class_types oclass, obj_material_types material)
        {
            Assert.False(GHItemTwinkle.IsEligible((sbyte)oclass, (byte)material, false, true, false));
        }

        [Theory]
        [InlineData(obj_class_types.WAND_CLASS, obj_material_types.MAT_WOOD)]
        [InlineData(obj_class_types.RING_CLASS, obj_material_types.MAT_GOLD)]
        [InlineData(obj_class_types.AMULET_CLASS, obj_material_types.MAT_IRON)]
        [InlineData(obj_class_types.COIN_CLASS, obj_material_types.MAT_GOLD)]
        [InlineData(obj_class_types.GEM_CLASS, obj_material_types.MAT_GLASS)]
        [InlineData(obj_class_types.GEM_CLASS, obj_material_types.MAT_GEMSTONE)]
        [InlineData(obj_class_types.GEM_CLASS, obj_material_types.MAT_HARD_CRYSTAL)]
        [InlineData(obj_class_types.GEM_CLASS, obj_material_types.MAT_NONE)]
        public void AlwaysTwinkleItems_TwinkleWhateverTheirDknown(obj_class_types oclass, obj_material_types material)
        {
            Assert.True(GHItemTwinkle.IsAlwaysTwinkle((sbyte)oclass, (byte)material));
            Assert.True(GHItemTwinkle.IsEligible((sbyte)oclass, (byte)material, false, false, false));
            Assert.True(GHItemTwinkle.IsEligible((sbyte)oclass, (byte)material, false, true, false));
            Assert.False(GHItemTwinkle.IsEligible((sbyte)oclass, (byte)material, true, false, false));
        }

        [Theory]
        [InlineData(obj_class_types.GEM_CLASS, obj_material_types.MAT_MINERAL)]
        [InlineData(obj_class_types.GEM_CLASS, obj_material_types.MAT_IRON)]
        [InlineData(obj_class_types.WEAPON_CLASS, obj_material_types.MAT_IRON)]
        [InlineData(obj_class_types.POTION_CLASS, obj_material_types.MAT_GLASS)]
        [InlineData(obj_class_types.TOOL_CLASS, obj_material_types.MAT_IRON)]
        public void IsAlwaysTwinkle_OtherItems_False(obj_class_types oclass, obj_material_types material)
        {
            Assert.False(GHItemTwinkle.IsAlwaysTwinkle((sbyte)oclass, (byte)material));
        }

        [Fact]
        public void DknownIsMeaningful_CoinsAndListedClasses_ButNotAmulets()
        {
            /* Gold is mergeable, and mksobj clears dknown for mergeable objects */
            Assert.True(GHItemTwinkle.DknownIsMeaningful((sbyte)obj_class_types.COIN_CLASS));
            Assert.True(GHItemTwinkle.DknownIsMeaningful((sbyte)obj_class_types.RING_CLASS));
            Assert.False(GHItemTwinkle.DknownIsMeaningful((sbyte)obj_class_types.AMULET_CLASS));
        }

        [Theory]
        [InlineData(0, 1)]
        [InlineData(7, 8)]
        [InlineData(8, 8)]
        [InlineData(255, 8)]
        public void IncrementCount_StopsAtTheBudget(int count, int expected)
        {
            Assert.Equal((byte)expected, GHItemTwinkle.IncrementCount((byte)count));
        }

        [Theory]
        [InlineData(1, 0, 0, 0, true)]
        [InlineData(0, 3, 0, 0, true)]
        [InlineData(0, 0, 5, 0, true)]
        [InlineData(0, 0, 0, 1, true)]
        [InlineData(0, 0, 0, 0, false)]
        public void IsSpecialQuality_AnyQualityCounts(int elemental, int mythicPrefix, int mythicSuffix, int exceptionality, bool expected)
        {
            Assert.Equal(expected, GHItemTwinkle.IsSpecialQuality((byte)elemental, (byte)mythicPrefix, (byte)mythicSuffix, (byte)exceptionality));
        }

        [Theory]
        [InlineData(obj_material_types.MAT_GOLD, true)]
        [InlineData(obj_material_types.MAT_PLATINUM, true)]
        [InlineData(obj_material_types.MAT_ORICHALCUM, true)]
        [InlineData(obj_material_types.MAT_MITHRIL, true)]
        [InlineData(obj_material_types.MAT_ADAMANTIUM, true)]
        [InlineData(obj_material_types.MAT_CRYSTAL, true)]
        [InlineData(obj_material_types.MAT_HARD_CRYSTAL, true)]
        [InlineData(obj_material_types.MAT_SILVER, false)]
        [InlineData(obj_material_types.MAT_IRON, false)]
        [InlineData(obj_material_types.MAT_GLASS, false)]
        [InlineData(obj_material_types.MAT_GEMSTONE, false)]
        [InlineData(obj_material_types.MAT_MINERAL, false)]
        [InlineData(obj_material_types.MAT_NONE, false)]
        public void IsAlwaysTwinkleMaterial_PreciousMetalsAndCrystal(obj_material_types material, bool expected)
        {
            Assert.Equal(expected, GHItemTwinkle.IsAlwaysTwinkleMaterial((byte)material));
        }

        [Fact]
        public void SpecialItems_AlwaysTwinkle()
        {
            sbyte weapon = (sbyte)obj_class_types.WEAPON_CLASS;
            byte iron = (byte)obj_material_types.MAT_IRON;
            Assert.True(GHItemTwinkle.IsAlwaysTwinkle(weapon, iron, true));
            Assert.False(GHItemTwinkle.IsAlwaysTwinkle(weapon, iron, false));
            Assert.True(GHItemTwinkle.IsEligible(weapon, iron, false, true, false, true));
            Assert.True(GHItemTwinkle.IsEligible(weapon, iron, false, false, false, true));
            Assert.False(GHItemTwinkle.IsEligible(weapon, iron, false, true, false, false));
            /* A plain weapon never twinkles, even small and unexamined */
            Assert.False(GHItemTwinkle.IsEligible(weapon, iron, false, false, false, false));

            /* Special qualities override the class and material exclusions too */
            Assert.True(GHItemTwinkle.IsEligible((sbyte)obj_class_types.GEM_CLASS, (byte)obj_material_types.MAT_MINERAL, false, false, false, true));

            Assert.False(GHItemTwinkle.IsEligible(weapon, iron, true, false, false, true));
            Assert.False(GHItemTwinkle.IsEligible(weapon, iron, false, false, true, true));
        }

        [Fact]
        public void IsEligible_AmuletWithDknown_StillTwinkles()
        {
            /* Amulets are created with dknown 1, so dknown says nothing about them */
            Assert.True(GHItemTwinkle.IsEligible((sbyte)obj_class_types.AMULET_CLASS, (byte)obj_material_types.MAT_IRON, false, true, false));
        }

        [Fact]
        public void IsEligible_HallucinatedOrUChainOrUBall_NeverTwinkles()
        {
            int c;
            for (c = 0; c < (int)obj_class_types.MAX_OBJECT_CLASSES; c++)
            {
                Assert.False(GHItemTwinkle.IsEligible((sbyte)c, (byte)obj_material_types.MAT_GLASS, true, false, false));
                Assert.False(GHItemTwinkle.IsEligible((sbyte)c, (byte)obj_material_types.MAT_GLASS, false, false, true));
            }
        }

        [Theory]
        [InlineData(57f, 28.5f, GHItemTwinkle.TierSmall)]      /* wand */
        [InlineData(103f, 29f, GHItemTwinkle.TierSmall)]       /* arrow */
        [InlineData(110f, 30f, GHItemTwinkle.TierSmall)]       /* dart */
        [InlineData(158f, 19f, GHItemTwinkle.TierSmall)]       /* gem */
        [InlineData(166f, 21f, GHItemTwinkle.TierSmall)]       /* ring */
        [InlineData(215f, 25f, GHItemTwinkle.TierSmall)]       /* amulet */
        [InlineData(245f, 39f, GHItemTwinkle.TierSmall)]       /* dagger */
        [InlineData(256f, 18f, GHItemTwinkle.TierSmall)]       /* ioun stone */
        [InlineData(247f, 43f, GHItemTwinkle.TierMedium)]      /* runed dagger */
        [InlineData(273f, 23f, GHItemTwinkle.TierMedium)]      /* clove of garlic */
        [InlineData(339f, 40.7f, GHItemTwinkle.TierMedium)]    /* scroll */
        [InlineData(383f, 28f, GHItemTwinkle.TierMedium)]      /* clear potion */
        [InlineData(344f, 50f, GHItemTwinkle.TierNone)]        /* short sword */
        [InlineData(396f, 58f, GHItemTwinkle.TierNone)]        /* long sword */
        [InlineData(433f, 24f, GHItemTwinkle.TierNone)]        /* helmet */
        public void GetSizeTier_MatchesCalibration(float nativeArea, float nativeLength, byte expected)
        {
            Assert.Equal(expected, GHItemTwinkle.GetSizeTier(nativeArea, nativeLength));
        }

        private static void SetAlpha(byte[] buffer, int rowBytes, int x, int y, byte alpha)
        {
            buffer[y * rowBytes + x * 4 + 3] = alpha;
        }

        private static unsafe TileOpacity Measure(byte[] buffer, int rowBytes, int x0, int y0, int width, int height)
        {
            fixed (byte* p = buffer)
            {
                return GHItemTwinkle.MeasureOpaque(p, rowBytes, x0, y0, width, height);
            }
        }

        [Fact]
        public void MeasureOpaque_Block_CountsAreaAndBounds()
        {
            int rowBytes = 8 * 4;
            byte[] buffer = new byte[rowBytes * 6];
            int x, y;
            for (y = 1; y <= 2; y++)
            {
                for (x = 2; x <= 4; x++)
                    SetAlpha(buffer, rowBytes, x, y, 255);
            }

            TileOpacity res = Measure(buffer, rowBytes, 0, 0, 8, 6);

            Assert.True(res.Valid);
            Assert.Equal(6, res.Area);
            Assert.Equal(2, res.MinX);
            Assert.Equal(1, res.MinY);
            Assert.Equal(4, res.MaxX);
            Assert.Equal(2, res.MaxY);
        }

        [Fact]
        public void MeasureOpaque_AlphaThreshold_Is64()
        {
            int rowBytes = 8 * 4;
            byte[] buffer = new byte[rowBytes * 6];
            SetAlpha(buffer, rowBytes, 0, 0, 63);
            SetAlpha(buffer, rowBytes, 5, 3, 64);

            TileOpacity res = Measure(buffer, rowBytes, 0, 0, 8, 6);

            Assert.True(res.Valid);
            Assert.Equal(1, res.Area);
            Assert.Equal(5, res.MinX);
            Assert.Equal(3, res.MinY);
            Assert.Equal(5, res.MaxX);
            Assert.Equal(3, res.MaxY);
        }

        [Fact]
        public void MeasureOpaque_Empty_IsNotValid()
        {
            int rowBytes = 8 * 4;
            byte[] buffer = new byte[rowBytes * 6];

            TileOpacity res = Measure(buffer, rowBytes, 0, 0, 8, 6);

            Assert.False(res.Valid);
            Assert.Equal(0, res.Area);
        }

        [Fact]
        public void MeasureOpaque_OffsetRectangleWithRowPadding_MeasuresOnlyTheRectangle()
        {
            int rowBytes = 10 * 4 + 8;
            byte[] buffer = new byte[rowBytes * 8];
            int y;

            /* Inside the rectangle at (3, 2), 4 x 4 */
            SetAlpha(buffer, rowBytes, 4, 3, 255);
            SetAlpha(buffer, rowBytes, 6, 5, 255);

            /* Outside it */
            SetAlpha(buffer, rowBytes, 2, 3, 255);
            SetAlpha(buffer, rowBytes, 7, 3, 255);
            SetAlpha(buffer, rowBytes, 4, 1, 255);
            SetAlpha(buffer, rowBytes, 4, 6, 255);

            /* Row padding */
            for (y = 0; y < 8; y++)
            {
                buffer[y * rowBytes + 10 * 4 + 3] = 255;
                buffer[y * rowBytes + 10 * 4 + 7] = 255;
            }

            TileOpacity res = Measure(buffer, rowBytes, 3, 2, 4, 4);

            Assert.True(res.Valid);
            Assert.Equal(2, res.Area);
            Assert.Equal(1, res.MinX);
            Assert.Equal(1, res.MinY);
            Assert.Equal(3, res.MaxX);
            Assert.Equal(3, res.MaxY);
        }

        [Theory]
        [InlineData(GHItemTwinkle.TierNone, ItemTwinkleMode.Off, false)]
        [InlineData(GHItemTwinkle.TierNone, ItemTwinkleMode.SmallItems, false)]
        [InlineData(GHItemTwinkle.TierNone, ItemTwinkleMode.SmallAndMediumItems, false)]
        [InlineData(GHItemTwinkle.TierSmall, ItemTwinkleMode.Off, false)]
        [InlineData(GHItemTwinkle.TierSmall, ItemTwinkleMode.SmallItems, true)]
        [InlineData(GHItemTwinkle.TierSmall, ItemTwinkleMode.SmallAndMediumItems, true)]
        [InlineData(GHItemTwinkle.TierMedium, ItemTwinkleMode.Off, false)]
        [InlineData(GHItemTwinkle.TierMedium, ItemTwinkleMode.SmallItems, false)]
        [InlineData(GHItemTwinkle.TierMedium, ItemTwinkleMode.SmallAndMediumItems, true)]
        public void IsShown_FollowsModeAndTier(byte tier, ItemTwinkleMode mode, bool expected)
        {
            Assert.Equal(expected, GHItemTwinkle.IsShown(tier, (int)mode));
        }

        [Theory]
        [InlineData(0, 1.0, true)]
        [InlineData(2, 1.0, true)]
        [InlineData(3, 1.0, false)]
        [InlineData(0, 3.0, true)]
        [InlineData(3, 3.0, true)]
        [InlineData(4, 3.0, false)]
        public void IsAdmissionPhase_CoversAtLeastOneFrame(int phaseTick, double ticksPerFrame, bool expected)
        {
            Assert.Equal(expected, GHItemTwinkle.IsAdmissionPhase(phaseTick, ticksPerFrame));
        }

        [Fact]
        public void Schedule_OverManyKeys_HasFullDurationsAndBoundedGaps()
        {
            AssertSchedule(false, GHItemTwinkle.SlotTicks, GHItemTwinkle.JitterTicks);
        }

        [Fact]
        public void KnownSchedule_OverManyKeys_HasFullDurationsAndBoundedGaps()
        {
            AssertSchedule(true, GHItemTwinkle.KnownSlotTicks, GHItemTwinkle.KnownJitterTicks);
        }

        [Fact]
        public void KnownSchedule_CyclesNeverEqualNormalCycles()
        {
            uint key;
            for (key = 1; key <= 200; key++)
            {
                long tick;
                for (tick = 0; tick < 72000; tick += 37)
                {
                    GHItemTwinkle.TryGetActiveTwinkle(key, tick, false, out long normalCycle, out _);
                    GHItemTwinkle.TryGetActiveTwinkle(key, tick, true, out long knownCycle, out _);
                    Assert.Equal(0L, normalCycle & GHItemTwinkle.KnownCycleFlag);
                    Assert.NotEqual(0L, knownCycle & GHItemTwinkle.KnownCycleFlag);
                }
            }
        }

        private static void AssertSchedule(bool slow, int slotTicks, int jitterTicks)
        {
            const int keyCount = 500;
            const long tickCount = 72000;
            long gapSum = 0;
            long gapCount = 0;
            int k;
            for (k = 0; k < keyCount; k++)
            {
                uint key = k < keyCount / 2
                    ? (uint)(k + 1)
                    : GHItemTwinkle.GetKey(0, k % 80, k / 80);
                bool prevActive = false;
                bool runFromStart = false;
                int prevPhase = 0;
                int runLength = 0;
                long lastStart = -1;
                long tick;
                for (tick = 0; tick < tickCount; tick++)
                {
                    bool active = GHItemTwinkle.TryGetActiveTwinkle(key, tick, slow, out _, out int phaseTick);
                    if (active)
                    {
                        if (!prevActive)
                        {
                            if (tick > 0)
                                Assert.Equal(0, phaseTick);
                            runFromStart = phaseTick == 0;
                            runLength = 0;
                            if (runFromStart)
                            {
                                if (lastStart >= 0)
                                {
                                    long gap = tick - lastStart;
                                    Assert.True(gap > slotTicks - jitterTicks, "gap " + gap);
                                    Assert.True(gap < slotTicks + jitterTicks, "gap " + gap);
                                    gapSum += gap;
                                    gapCount++;
                                }
                                lastStart = tick;
                            }
                        }
                        else
                        {
                            Assert.Equal(prevPhase + 1, phaseTick);
                        }
                        runLength++;
                        prevPhase = phaseTick;
                    }
                    else if (prevActive && runFromStart)
                    {
                        Assert.Equal(GHItemTwinkle.DurationTicks, runLength);
                    }
                    prevActive = active;
                }
            }

            Assert.True(gapCount > 0);
            double meanGap = (double)gapSum / gapCount;
            Assert.InRange(meanGap, slotTicks * 0.9, slotTicks * 1.1);
        }

        [Fact]
        public void GetEnvelope_StaysInRangeAndPeaksEarly()
        {
            Assert.Equal(0f, GHItemTwinkle.GetEnvelope(GHItemTwinkle.DurationTicks));
            Assert.Equal(0f, GHItemTwinkle.GetEnvelope(GHItemTwinkle.DurationTicks + 1));
            Assert.Equal(0f, GHItemTwinkle.GetEnvelope(GHItemTwinkle.DurationTicks * 10));

            float max = -1f;
            int maxPhase = -1;
            int phase;
            for (phase = 0; phase < GHItemTwinkle.DurationTicks; phase++)
            {
                float value = GHItemTwinkle.GetEnvelope(phase);
                Assert.InRange(value, 0f, 1f);
                if (value > max)
                {
                    max = value;
                    maxPhase = phase;
                }
            }

            Assert.True(max > 0.95f, "max " + max);
            Assert.InRange(maxPhase, 5, 9);
        }

        [Fact]
        public void GetKey_UsesObjectIdOrPosition()
        {
            Assert.Equal(GHItemTwinkle.GetKey(42, 1, 2), GHItemTwinkle.GetKey(42, 30, 15));
            Assert.Equal(42u, GHItemTwinkle.GetKey(42, 1, 2));
            Assert.NotEqual(GHItemTwinkle.GetKey(0, 1, 2), GHItemTwinkle.GetKey(0, 2, 1));
            Assert.NotEqual(GHItemTwinkle.GetKey(0, 1, 2), GHItemTwinkle.GetKey(0, 1, 3));
            Assert.Equal(GHItemTwinkle.GetKey(0, 5, 7), GHItemTwinkle.GetKey(0, 5, 7));
        }

        [Theory]
        [InlineData(10f, 64f, 0.16 * 64)]
        [InlineData(25f, 64f, 0.6 * 25)]
        [InlineData(40f, 64f, 0.30 * 64)]
        public void GetPeakRadius_ClampsToTileWidth(float opaqueWidth, float tileWidth, double expected)
        {
            Assert.Equal(expected, GHItemTwinkle.GetPeakRadius(opaqueWidth, tileWidth), 4);
        }

        private static unsafe float RunHotPath(byte* pixels, int rowBytes, int iterations)
        {
            float sink = 0f;
            int i;
            for (i = 0; i < iterations; i++)
            {
                uint key = (uint)(i + 1);
                if (GHItemTwinkle.IsEligible((sbyte)obj_class_types.RING_CLASS, (byte)obj_material_types.MAT_GOLD, false, false, false))
                    sink += 1f;
                sink += GHItemTwinkle.GetSizeTier(i % 500, i % 60);
                if (GHItemTwinkle.TryGetActiveTwinkle(key, i, out long cycle, out int phaseTick))
                    sink += GHItemTwinkle.GetEnvelope(phaseTick);
                if (GHItemTwinkle.TryGetActiveTwinkle(key, i, true, out long knownCycle, out int knownPhase))
                    sink += knownPhase + (knownCycle & 1);
                if (GHItemTwinkle.IsAlwaysTwinkle((sbyte)obj_class_types.GEM_CLASS, (byte)obj_material_types.MAT_GLASS))
                    sink += 1f;
                sink += GHItemTwinkle.IncrementCount((byte)(i & 0xFF));
                bool special = GHItemTwinkle.IsSpecialQuality((byte)(i & 1), 0, 0, (byte)(i & 2))
                    || GHItemTwinkle.IsAlwaysTwinkleMaterial((byte)(i % (int)obj_material_types.MAX_MATERIAL_TYPES));
                if (GHItemTwinkle.IsEligible((sbyte)obj_class_types.WEAPON_CLASS, (byte)obj_material_types.MAT_IRON, false, true, false, special))
                    sink += 1f;
                GHItemTwinkle.GetAnchor(key, cycle, out float fx, out float fy);
                sink += fx + fy;
                sink += GHItemTwinkle.GetPeakRadius(i % 40, 64f);
                TileOpacity opacity = GHItemTwinkle.MeasureOpaque(pixels, rowBytes, 0, 0, 8, 8);
                sink += opacity.Area;
            }
            return sink;
        }

        [Fact]
        public unsafe void HotPath_AllocatesNothing()
        {
            int rowBytes = 8 * 4;
            byte[] buffer = new byte[rowBytes * 8];
            SetAlpha(buffer, rowBytes, 3, 4, 255);
            float sink;
            long before, after;
            fixed (byte* p = buffer)
            {
                sink = RunHotPath(p, rowBytes, 1);
                before = GC.GetAllocatedBytesForCurrentThread();
                sink += RunHotPath(p, rowBytes, 100000);
                after = GC.GetAllocatedBytesForCurrentThread();
            }

            Assert.Equal(before, after);
            Assert.True(sink > 0f);
        }
    }
}
