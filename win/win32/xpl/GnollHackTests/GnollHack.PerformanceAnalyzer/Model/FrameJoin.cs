using GnollHack.PerformanceAnalyzer.Readers;
using GnollHackX.Performance;

namespace GnollHack.PerformanceAnalyzer.Model
{
    /* What a join of measured display times onto a captured timeline did */
    public sealed class JoinReport
    {
        public string Source;
        public bool Available = true;
        public string UnavailableReason;
        public int PaintedFrames;               /* painted frames with a flush end: the join candidates */
        public int Matched;                     /* given a measured display time */
        public int Unmatched;                   /* left estimated */
        public int DroppedPresents;             /* matched to a present or frame that never reached the screen */
        public int GlThreadLeftEstimated;       /* Perfetto: painted off the UI thread, not joined */
        public int DisplayedBeforeReady;        /* measured display earlier than the flush end: a clock mismatch */
        public double OffsetMs = double.NaN;    /* Perfetto: capture ms minus trace ms */
        public int OffsetSamples;
        public Dictionary<long, string> JankTypeByFrameId = new Dictionary<long, string>();
        public Dictionary<long, string> PresentTypeByFrameId = new Dictionary<long, string>();
        public List<string> Notes = new List<string>();
    }

    /* Applies externally measured display times to parsed frame records: DisplayedAtTicks
       is set and PresentSource becomes Measured, which GHSmoothnessMetrics.Analyze keeps
       instead of its estimate.

       A frame whose present was measured but never reached the screen is given the display
       time of the next frame that did. Analyze then sees two frames on one refresh, keeps
       the later-ready one, and counts this one dropped, which is what happened. */
    public static class FrameJoin
    {
        /* A present more than this long after the flush belongs to something else */
        public const double MaxPresentLagMs = 250.0;

        /* The present for a frame is the first present of the kept swap chain at or after
           the frame's flush end. Requires the capture's OriginStopwatchTicks, since the
           timeline's times are relative and PresentMon's are absolute QPC values; on
           Windows QPC ticks are the app's Stopwatch ticks. */
        public static JoinReport ApplyPresentMon(CapturedTimeline t, PresentMonCapture pm)
        {
            JoinReport rep = new JoinReport { Source = "PresentMon " + System.IO.Path.GetFileName(pm.Path) };
            if (!pm.HasQpc)
            {
                rep.Available = false;
                rep.UnavailableReason = "the PresentMon CSV has no QPC time column; capture with --qpc_time";
                return rep;
            }
            if (!t.Clock.OriginDeviceTicks.HasValue)
            {
                rep.Available = false;
                rep.UnavailableReason = "the frame timeline has no '# OriginStopwatchTicks=' line, so its times cannot be placed on the QPC clock";
                return rep;
            }
            if (pm.DisplayColumn == null)
                rep.Notes.Add("no MsUntilDisplayed column: every present counts as not displayed");
            if (pm.OtherSwapChains.Count > 0)
                rep.Notes.Add("kept swap chain " + pm.KeptSwapChain + "; ignored " + string.Join(", ", pm.OtherSwapChains.Select(kv => kv.Key + " (" + kv.Value + " presents)")));

            double originMs = pm.TimesInMs ? t.Clock.OriginDeviceTicks.Value * 1000.0 / t.Clock.DeviceFrequency : 0;
            int np = pm.Presents.Count;
            double[] presentMs = new double[np];
            double[] displayMs = new double[np];
            for (int k = 0; k < np; k++)
            {
                PresentMonPresent p = pm.Presents[k];
                double baseMs = pm.TimesInMs ? p.Timestamp - originMs : t.Clock.DeviceTicksToMs(p.Timestamp);
                presentMs[k] = baseMs + p.PresentOffsetMs;
                displayMs[k] = p.Displayed ? baseMs + p.DisplayedOffsetMs : double.NaN;
            }
            int[] order = Enumerable.Range(0, np).OrderBy(k => presentMs[k]).ToArray();
            double[] sortedPresent = order.Select(k => presentMs[k]).ToArray();

            /* The display time of the first displayed present at or after position s */
            double NextDisplayed(int s)
            {
                for (int k = s; k < order.Length; k++)
                {
                    if (!double.IsNaN(displayMs[order[k]]))
                        return displayMs[order[k]];
                }
                return double.NaN;
            }

            for (int i = 0; i < t.Count; i++)
            {
                ref GHFrameRecord r = ref t.Records[i];
                if (r.Paint != GHPaintOutcome.Painted || r.FlushEndTicks == 0)
                    continue;
                rep.PaintedFrames++;
                double flushMs = t.Clock.TicksToMs(r.FlushEndTicks);
                int pos = LowerBound(sortedPresent, flushMs);
                if (pos >= sortedPresent.Length || sortedPresent[pos] - flushMs > MaxPresentLagMs)
                {
                    rep.Unmatched++;
                    continue;
                }
                double shown = displayMs[order[pos]];
                if (double.IsNaN(shown))
                {
                    rep.DroppedPresents++;
                    shown = NextDisplayed(pos + 1);
                    if (double.IsNaN(shown))
                    {
                        rep.Unmatched++;
                        continue;
                    }
                }
                if (shown < flushMs)
                    rep.DisplayedBeforeReady++;
                r.DisplayedAtTicks = t.Clock.MsToTicks(shown);
                r.PresentSource = GHPresentSource.Measured;
                rep.Matched++;
            }
            return rep;
        }

        /* The display time of a UI-thread frame is the present time (actual_ts + actual_dur)
           of the SurfaceFlinger frame whose token is the vsync token of the frame's GH.Tick.
           Trace-clock times are aligned to the capture by the median, over every GH.Tick
           matched to a record, of (CallbackStartMs - tick ts). A GL-thread paint reaches
           the screen in a later frame that the trace does not tie to its FrameId, so such
           frames keep their estimate and are counted. */
        public static JoinReport ApplyPerfetto(CapturedTimeline t, PerfettoCapture p)
        {
            JoinReport rep = new JoinReport { Source = "Perfetto " + p.DirectoryPath };
            List<PerfettoSlice> ticks = p.Slices.Where(s => s.Name == "GH.Tick" && s.FrameId.HasValue).ToList();
            List<double> offsets = new List<double>();
            foreach (PerfettoSlice s in ticks)
            {
                int idx = t.IndexOfFrameId(s.FrameId.Value);
                if (idx < 0 || t.Records[idx].CallbackStartTicks == 0)
                    continue;
                offsets.Add(t.Clock.TicksToMs(t.Records[idx].CallbackStartTicks) - s.Ts / 1e6);
            }
            if (offsets.Count == 0)
            {
                rep.Available = false;
                rep.UnavailableReason = "no GH.Tick slice carries a FrameId present in the frame timeline";
                return rep;
            }
            offsets.Sort();
            rep.OffsetSamples = offsets.Count;
            rep.OffsetMs = offsets.Count % 2 == 1
                ? offsets[offsets.Count / 2]
                : (offsets[offsets.Count / 2 - 1] + offsets[offsets.Count / 2]) / 2.0;

            /* Several layers can report one token; the latest present is when all of the
               frame's content is on screen */
            Dictionary<long, PerfettoFrame> byToken = new Dictionary<long, PerfettoFrame>();
            int duplicates = 0;
            foreach (PerfettoFrame f in p.Frames)
            {
                if (byToken.TryGetValue(f.Token, out PerfettoFrame existing))
                {
                    duplicates++;
                    if (f.PresentedAt > existing.PresentedAt)
                        byToken[f.Token] = f;
                }
                else
                {
                    byToken[f.Token] = f;
                }
            }
            if (duplicates > 0)
                rep.Notes.Add(duplicates + " frame rows share a token with another row; the latest present of each token is used");

            Dictionary<int, PerfettoFrame> frameOfRecord = new Dictionary<int, PerfettoFrame>();
            foreach (PerfettoSlice s in ticks)
            {
                if (!s.VsyncToken.HasValue)
                    continue;
                int idx = t.IndexOfFrameId(s.FrameId.Value);
                if (idx < 0 || !byToken.TryGetValue(s.VsyncToken.Value, out PerfettoFrame f))
                    continue;
                frameOfRecord[idx] = f;
                if (f.JankType != null)
                    rep.JankTypeByFrameId[s.FrameId.Value] = f.JankType;
                if (f.PresentType != null)
                    rep.PresentTypeByFrameId[s.FrameId.Value] = f.PresentType;
            }

            List<int> dropped = new List<int>();
            for (int i = 0; i < t.Count; i++)
            {
                ref GHFrameRecord r = ref t.Records[i];
                if (r.Paint != GHPaintOutcome.Painted || r.FlushEndTicks == 0)
                    continue;
                rep.PaintedFrames++;
                if (!r.PaintOnUiThread)
                {
                    rep.GlThreadLeftEstimated++;
                    rep.Unmatched++;
                    continue;
                }
                if (!frameOfRecord.TryGetValue(i, out PerfettoFrame f))
                {
                    rep.Unmatched++;
                    continue;
                }
                if (f.IsDropped)
                {
                    dropped.Add(i);
                    continue;
                }
                double shown = f.PresentedAt / 1e6 + rep.OffsetMs;
                if (shown < t.Clock.TicksToMs(r.FlushEndTicks))
                    rep.DisplayedBeforeReady++;
                r.DisplayedAtTicks = t.Clock.MsToTicks(shown);
                r.PresentSource = GHPresentSource.Measured;
                rep.Matched++;
            }

            foreach (int i in dropped)
            {
                rep.DroppedPresents++;
                long next = 0;
                for (int k = i + 1; k < t.Count && next == 0; k++)
                {
                    if (t.Records[k].PresentSource == GHPresentSource.Measured && t.Records[k].DisplayedAtTicks != 0)
                        next = t.Records[k].DisplayedAtTicks;
                }
                if (next == 0)
                {
                    rep.Unmatched++;
                    continue;
                }
                t.Records[i].DisplayedAtTicks = next;
                t.Records[i].PresentSource = GHPresentSource.Measured;
                rep.Matched++;
            }
            return rep;
        }

        private static int LowerBound(double[] sorted, double value)
        {
            int lo = 0, hi = sorted.Length;
            while (lo < hi)
            {
                int mid = (lo + hi) / 2;
                if (sorted[mid] < value)
                    lo = mid + 1;
                else
                    hi = mid;
            }
            return lo;
        }
    }
}
