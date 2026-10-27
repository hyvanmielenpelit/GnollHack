using System;
using System.Collections.Generic;

namespace GnollHackX
{
    /* Map FPS choices derived from the display refresh rate: the rate itself and its exact
       divisions of at least MinimumFps, so that every map frame stays on screen for a whole
       number of refreshes. Free of UI types, so that the unit tests compile it. */
    public static class GHMapFps
    {
        public const int MinimumFps = 20;
        public const int HalvedDefaultMinimumHz = 80; /* the default divisor is at least 2 from this rate */
        public const int DefaultDivisorStepHz = 50; /* the default divisor rises by one per step from 150 Hz */
        public const int MaximumDefaultDivisor = 6;
        public const int TimerLoopDefaultMaxFps = 60;
        public const int UnknownRateFps = 60;

        private static readonly int[] _standardRates = { 48, 50, 60, 72, 75, 90, 100, 120, 144, 165, 180, 200, 240, 300, 360 };

        /* A rate within 1.5 Hz of a standard rate is that rate: 59 and 61 are 60, 143 is 144 */
        public static int SnapRefreshRate(int roundedRefreshRate)
        {
            int i;
            for (i = 0; i < _standardRates.Length; i++)
            {
                if (Math.Abs(roundedRefreshRate - _standardRates[i]) * 2 <= 3)
                    return _standardRates[i];
            }
            return roundedRefreshRate;
        }

        /* 1 below HalvedDefaultMinimumHz, else refreshRate / DefaultDivisorStepHz, at least 2 and at most
           MaximumDefaultDivisor */
        public static int GetDefaultDivisor(int refreshRate)
        {
            if (refreshRate < HalvedDefaultMinimumHz)
                return 1;
            return Math.Max(2, Math.Min(MaximumDefaultDivisor, refreshRate / DefaultDivisorStepHz));
        }

        /* Ascending: every exact division refreshRate / n >= MinimumFps for n = 2, 3, ..., then the
           rate itself. A rate below MinimumFps offers only itself; 0 or less offers nothing. */
        public static List<int> GetOfferedFps(int refreshRate)
        {
            List<int> res = new List<int>();
            if (refreshRate <= 0)
                return res;

            int n;
            for (n = 2; refreshRate >= MinimumFps * n; n++)
            {
                if (refreshRate % n == 0)
                    res.Add(refreshRate / n);
            }
            res.Reverse();
            res.Add(refreshRate);
            return res;
        }

        /* The same rule as GetOfferedFps without allocating; called every frame */
        public static bool IsOffered(int fps, int refreshRate)
        {
            if (refreshRate <= 0 || fps <= 0)
                return false;
            if (fps == refreshRate)
                return true;
            return fps >= MinimumFps && fps < refreshRate && refreshRate % fps == 0;
        }

        /* The highest offered value at or below refreshRate / GetDefaultDivisor(refreshRate);
           without the platform render loop, also at most TimerLoopDefaultMaxFps. The lowest
           offered value when none qualifies; UnknownRateFps when the rate is 0 or less. */
        public static int GetDefaultFps(int refreshRate, bool platformLoop)
        {
            if (refreshRate <= 0)
                return UnknownRateFps;

            int divisor = GetDefaultDivisor(refreshRate);
            int lowest = refreshRate;
            int n;
            for (n = 1; n == 1 || refreshRate >= MinimumFps * n; n++)
            {
                if (refreshRate % n != 0)
                    continue;
                int fps = refreshRate / n;
                if (fps * divisor <= refreshRate && (platformLoop || fps <= TimerLoopDefaultMaxFps))
                    return fps;
                lowest = fps;
            }
            return lowest;
        }

        /* The stored setting (-1 = default) resolved against a refresh rate: the setting when it
           is offered, otherwise the default. An unknown rate keeps a positive setting, at most
           UnknownRateFps. */
        public static int GetEffectiveFps(int setting, int refreshRate, bool platformLoop)
        {
            if (refreshRate <= 0)
                return setting > 0 ? Math.Min(setting, UnknownRateFps) : UnknownRateFps;
            if (setting > 0 && IsOffered(setting, refreshRate))
                return setting;
            return GetDefaultFps(refreshRate, platformLoop);
        }

        /* General animation counter steps per frame: tile animations run at 30-40 steps a second,
           except below 30 FPS. 2 at 20 FPS or less, else 1 / ceil(fps / 40). */
        public static double GetTileAnimationFrameSpeed(int fps)
        {
            if (fps <= MinimumFps)
                return 2.0;
            return 1.0 / Math.Ceiling(fps / 40.0);
        }

        /* Main counter scale for GUI animations, which are designed at 60 FPS */
        public static double GetGuiAnimationMultiplier(int fps)
        {
            return fps > 0 ? 60.0 / fps : 1.0;
        }

        /* The default of the retired fixed-list Map FPS setting, reproduced to recognize it in a stored value */
        public static int GetLegacyDefaultFps(int roundedRefreshRate, bool platformLoop, bool isAvalonia)
        {
            if (isAvalonia && platformLoop && roundedRefreshRate >= 90 && roundedRefreshRate % 90 == 0)
                return 90;
            if (platformLoop && roundedRefreshRate >= 80 && roundedRefreshRate % 80 == 0)
                return 80;
            else if (platformLoop && roundedRefreshRate >= 72 && roundedRefreshRate % 72 == 0)
                return 72;
            else if (roundedRefreshRate >= 60)
                return 60;
            else if (roundedRefreshRate >= 40)
                return 40;
            else
                return 30;
        }

        /* The setting for a retired Map FPS enum index (-1 = default; the table is the retired enum order).
           The Settings page used to store the resolved default rather than -1, so the retired default level
           becomes -1 too: the value of GetLegacyDefaultFps at the refresh rate, or, while the rate is unknown
           (0 or less), any value that rule can give at 60 Hz or more -- 60, 72 and 80 with the platform render
           loop, and 90 with it on Avalonia. */
        public static int MigrateLegacyIndex(int legacyIndex, int roundedRefreshRate, bool platformLoop, bool isAvalonia)
        {
            int fps;
            switch (legacyIndex)
            {
                case 0: fps = 20; break;
                case 1: fps = 30; break;
                case 2: fps = 40; break;
                case 3: fps = 60; break;
                case 4: fps = 72; break;
                case 5: fps = 80; break;
                case 6: fps = 90; break;
                case 7: fps = 120; break;
                case 8: fps = 144; break;
                default: return -1;
            }
            bool wasDefaultLevel;
            if (roundedRefreshRate > 0)
                wasDefaultLevel = fps == GetLegacyDefaultFps(roundedRefreshRate, platformLoop, isAvalonia);
            else
                wasDefaultLevel = fps == 60 || (platformLoop && (fps == 72 || fps == 80 || (isAvalonia && fps == 90)));
            return wasDefaultLevel ? -1 : fps;
        }

        /* Timer interval in milliseconds; never longer than one frame at the given rate */
        public static uint GetIntervalMs(int fps)
        {
            return fps > 0 ? (uint)(1000 / fps) : 100U;
        }
    }
}
