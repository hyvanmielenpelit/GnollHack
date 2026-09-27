using GnollHack.PerformanceAnalyzer.Model;
using GnollHackX.Performance;

namespace GnollHack.PerformanceAnalyzer.Readers
{
    /* Reads the app's compositorframes*.csv (GHFrameTimeline.DumpCompositorFramesToCsv) onto
       the clock of the frame timeline it belongs to. Both files share one time origin; if
       the two carry different OriginStopwatchTicks lines, the compositor times are shifted
       onto the timeline's origin. */
    public static class CompositorFramesCsv
    {
        public static GHCompositorFrame[] Read(string path, TimelineClock clock)
        {
            string[] lines = File.ReadAllLines(path);
            Dictionary<string, string> meta = new Dictionary<string, string>(StringComparer.OrdinalIgnoreCase);
            int headerLine = -1;
            for (int i = 0; i < lines.Length; i++)
            {
                string l = lines[i].Trim();
                if (l.Length == 0)
                    continue;
                if (l.StartsWith("#"))
                {
                    FrameTimelineCsv.ParseMetadata(l, meta);
                    continue;
                }
                headerLine = i;
                break;
            }
            if (headerLine < 0)
                return new GHCompositorFrame[0];

            long frequency = FrameTimelineCsv.MetaLong(meta, "StopwatchFrequency") ?? clock.DeviceFrequency;
            long? origin = FrameTimelineCsv.MetaLong(meta, "OriginStopwatchTicks");
            long shift = 0;
            if (origin.HasValue && clock.OriginDeviceTicks.HasValue && frequency > 0)
                shift = clock.DurationMsToTicks((origin.Value - clock.OriginDeviceTicks.Value) * 1000.0 / frequency);

            Dictionary<string, int> c = Csv.HeaderIndex(Csv.Split(lines[headerLine]));
            List<GHCompositorFrame> frames = new List<GHCompositorFrame>(lines.Length);
            for (int i = headerLine + 1; i < lines.Length; i++)
            {
                if (lines[i].Trim().Length == 0 || lines[i].TrimStart().StartsWith("#"))
                    continue;
                string[] f = Csv.Split(lines[i]);
                GHCompositorFrame x = new GHCompositorFrame();
                x.Source = FrameTimelineCsv.Enum<GHCompositorSource>(f, c, "Source");
                x.IntendedVsyncTicks = Shift(FrameTimelineCsv.Time(f, c, "IntendedVsyncMs", clock), shift);
                x.VsyncTicks = Shift(FrameTimelineCsv.Time(f, c, "VsyncMs", clock), shift);
                x.SyncStartTicks = Shift(FrameTimelineCsv.Time(f, c, "SyncStartMs", clock), shift);
                x.CompletedTicks = Shift(FrameTimelineCsv.Time(f, c, "CompletedMs", clock), shift);
                x.GpuDurationTicks = FrameTimelineCsv.Duration(f, c, "GpuDurationMs", clock);
                x.DeadlineRaw = FrameTimelineCsv.Long(f, c, "DeadlineRaw");
                x.RefreshPeriodTicks = FrameTimelineCsv.Duration(f, c, "RefreshPeriodMs", clock);
                x.ComposeTicks = Shift(FrameTimelineCsv.Time(f, c, "ComposeMs", clock), shift);
                x.RefreshCount = FrameTimelineCsv.Long(f, c, "RefreshCount");
                x.ComposedFrameCount = FrameTimelineCsv.Long(f, c, "ComposedFrameCount");
                x.DroppedSinceLast = (int)FrameTimelineCsv.Long(f, c, "DroppedSinceLast");
                x.FirstDrawFrame = FrameTimelineCsv.Long(f, c, "FirstDrawFrame") != 0;
                frames.Add(x);
            }
            return frames.ToArray();
        }

        private static long Shift(long ticks, long shift)
        {
            return ticks == 0 ? 0 : ticks + shift;
        }
    }
}
