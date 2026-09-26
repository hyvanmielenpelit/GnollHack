using System.Globalization;
using System.Text;
using GnollHack.PerformanceAnalyzer.Model;
using GnollHack.PerformanceAnalyzer.Readers;
using GnollHackX.Performance;

namespace GnollHack.PerformanceAnalyzer.Commands
{
    public sealed class SmoothnessResult
    {
        public InAppRun Run;
        public CapturedTimeline Timeline;
        public GHSmoothnessSummary Summary;
        public GHDisplayedFrame[] Displayed;
        public int DisplayedCount;
        public List<JoinReport> Joins = new List<JoinReport>();
        public ChangePoints.Result Changes;
        public List<string> Warnings = new List<string>();
    }

    /* Recomputes an in-app run's smoothness from its frame timeline, optionally with
       measured display times, and writes a Markdown report:

         smoothness <run.json> [--presentmon <csv>] [--process <exe name>]
                    [--perfetto <dir>] [--out <report.md>]

       run.json is an in-app record (schema v2); its frame timeline and compositor CSVs
       are the files it names. --presentmon joins PresentMon's presents of the kept swap
       chain by QPC time, which needs the timeline's "# OriginStopwatchTicks=" line;
       --perfetto joins SurfaceFlinger's present times from perfetto_frames.csv and
       perfetto_app_slices.csv in the directory. When both are given, PresentMon is
       applied first and Perfetto overrides the frames it matches. The analysis itself is
       GHSmoothnessMetrics.Analyze, the code the app runs. */
    public static class SmoothnessCommand
    {
        public const int WorstHitchCount = 10;
        private const int MaxStageRows = 16;

        public static int Run(Args a)
        {
            string runJson = a.Positional.Count > 0 ? a.Positional[0] : a.Get("run");
            if (runJson == null)
                throw new ArgumentException("smoothness needs the run JSON: smoothness <run.json> [options]");
            SmoothnessResult res = Analyze(runJson, a.Get("presentmon"), a.Get("process"), a.Get("perfetto"));
            string text = BuildReport(res);
            string outPath = a.Get("out");
            if (outPath != null)
            {
                File.WriteAllText(outPath, Csv.ToCrlf(text), new UTF8Encoding(false));
                Console.WriteLine("smoothness: wrote " + outPath);
            }
            else
            {
                Console.Write(text);
            }
            foreach (string w in res.Warnings)
                Console.Error.WriteLine("smoothness: warning: " + w);
            return 0;
        }

        public static SmoothnessResult Analyze(string runJson, string presentMonCsv, string process, string perfettoDir)
        {
            SmoothnessResult res = new SmoothnessResult();
            res.Run = InAppRun.Load(runJson);
            string timelinePath = res.Run.ResolveFile(res.Run.FrameTimelineFile);
            if (timelinePath == null)
            {
                string fallback = Path.Combine(res.Run.Directory, "frametimeline.csv");
                if (File.Exists(fallback))
                {
                    timelinePath = fallback;
                    res.Warnings.Add("the run record names no frame timeline; using frametimeline.csv next to it");
                }
            }
            if (timelinePath == null || !File.Exists(timelinePath))
                throw new FileNotFoundException("frame timeline CSV not found: " + (timelinePath ?? "(the run record names none)"));

            res.Timeline = FrameTimelineCsv.Read(timelinePath, res.Run.StopwatchFrequency);
            if (res.Run.StopwatchFrequency > 0 && res.Timeline.Clock.DeviceFrequency != res.Run.StopwatchFrequency)
                res.Warnings.Add("the timeline's StopwatchFrequency (" + res.Timeline.Clock.DeviceFrequency
                    + ") differs from the run record's clock (" + res.Run.StopwatchFrequency + ")");
            string compositorPath = res.Run.ResolveFile(res.Run.CompositorFramesFile);
            if (compositorPath != null && File.Exists(compositorPath))
            {
                res.Timeline.Compositor = CompositorFramesCsv.Read(compositorPath, res.Timeline.Clock);
                res.Timeline.CompositorCount = res.Timeline.Compositor.Length;
                res.Timeline.CompositorPath = compositorPath;
            }
            else if (compositorPath != null)
            {
                res.Warnings.Add("compositor frames CSV not found: " + compositorPath);
            }

            if (presentMonCsv != null)
                res.Joins.Add(FrameJoin.ApplyPresentMon(res.Timeline, PresentMonCsv.ReadPresents(presentMonCsv, process)));
            if (perfettoDir != null)
                res.Joins.Add(FrameJoin.ApplyPerfetto(res.Timeline, PerfettoFrames.ReadDirectory(perfettoDir)));
            foreach (JoinReport j in res.Joins)
            {
                if (!j.Available)
                    res.Warnings.Add(j.Source + ": join unavailable: " + j.UnavailableReason);
            }

            CapturedTimeline t = res.Timeline;
            res.Displayed = new GHDisplayedFrame[Math.Max(1, t.Count)];
            res.Summary = GHSmoothnessMetrics.Analyze(t.Records, t.Count, t.Compositor, t.CompositorCount, res.Displayed, out res.DisplayedCount);
            res.Changes = ChangePoints.Detect(t, res.Displayed, res.DisplayedCount);
            return res;
        }

        public static string BuildReport(SmoothnessResult res)
        {
            StringBuilder md = new StringBuilder();
            InAppRun run = res.Run;
            md.AppendLine("# Smoothness: " + (run.Scenario ?? "unspecified") + ", arm " + (run.Arm ?? "?"));
            md.AppendLine();
            md.AppendLine("Generated " + DateTime.UtcNow.ToString("u", CultureInfo.InvariantCulture)
                + " by GnollHack.PerformanceAnalyzer from run `" + (run.RunId ?? "?") + "` (" + (run.StartedUtc ?? "?") + " to " + (run.EndedUtc ?? "?") + ").");
            md.AppendLine();
            md.AppendLine("| Input | File |");
            md.AppendLine("|---|---|");
            md.AppendLine("| Run record | " + Path.GetFileName(run.SourcePath) + " |");
            md.AppendLine("| Frame timeline | " + Path.GetFileName(res.Timeline.Path) + " (" + res.Timeline.Count + " ticks, Stopwatch frequency "
                + res.Timeline.Clock.DeviceFrequency + (res.Timeline.Clock.OriginDeviceTicks.HasValue ? ", origin known" : ", no origin line") + ") |");
            md.AppendLine("| Compositor frames | " + (res.Timeline.CompositorPath != null ? Path.GetFileName(res.Timeline.CompositorPath) + " (" + res.Timeline.CompositorCount + " frames)" : "none") + " |");
            md.AppendLine();
            foreach (string w in res.Warnings)
                md.AppendLine("> **Warning:** " + w);
            if (res.Warnings.Count > 0)
                md.AppendLine();

            WriteJoins(md, res);
            WriteHeadline(md, res);
            WriteCauses(md, res);
            WriteContentEvents(md, res);
            WriteWorstHitches(md, res);
            WriteChangePoints(md, res);
            return md.ToString();
        }

        private static void WriteJoins(StringBuilder md, SmoothnessResult res)
        {
            if (res.Joins.Count == 0)
                return;
            md.AppendLine("## Measured display times");
            md.AppendLine();
            foreach (JoinReport j in res.Joins)
            {
                md.AppendLine("**" + j.Source + "**" + (j.Available ? "" : ": unavailable, " + j.UnavailableReason + "."));
                md.AppendLine();
                if (!j.Available)
                    continue;
                md.AppendLine("| Painted frames | Matched | Left estimated | Dropped presents | Shared presents | GL-thread left estimated | Displayed before ready | Trace offset ms | Offset spread ms |");
                md.AppendLine("|---|---|---|---|---|---|---|---|---|");
                md.AppendLine("| " + j.PaintedFrames + " | " + j.Matched + " | " + j.Unmatched + " | " + j.DroppedPresents
                    + " | " + j.SharedPresents + " | " + j.GlThreadLeftEstimated + " | " + j.DisplayedBeforeReady
                    + " | " + (double.IsNaN(j.OffsetMs) ? "n/a" : F(j.OffsetMs, 3) + " (" + j.OffsetSamples + " ticks)")
                    + " | " + (double.IsNaN(j.OffsetSpreadMs) ? "n/a" : F(j.OffsetSpreadMs, 3)) + " |");
                md.AppendLine();
                foreach (string n in j.Notes)
                    md.AppendLine("- " + n);
                if (j.Notes.Count > 0)
                    md.AppendLine();
            }
        }

        private static string HitchBand(double msPerSec)
        {
            if (msPerSec < 5)
                return "good (< 5)";
            if (msPerSec <= 10)
                return "warning (5 to 10)";
            return "critical (> 10)";
        }

        private static void WriteHeadline(StringBuilder md, SmoothnessResult res)
        {
            GHSmoothnessSummary s = res.Summary;
            Dictionary<string, double> app = res.Run.Smoothness;
            int measured = 0, estimated = 0;
            for (int j = 0; j < res.DisplayedCount; j++)
            {
                if (res.Displayed[j].Source == GHPresentSource.Measured)
                    measured++;
                else
                    estimated++;
            }

            md.AppendLine("## Headline");
            md.AppendLine();
            md.AppendLine("Hitch time ratio: **" + F(s.HitchRatioMsPerSec) + " ms/s, " + HitchBand(s.HitchRatioMsPerSec)
                + "** (Apple's bands: under 5 good, 5 to 10 warning, over 10 critical).");
            md.AppendLine();
            md.AppendLine("| Metric | Recomputed | App reported |");
            md.AppendLine("|---|---|---|");
            Row(md, "Displayed FPS", F(s.DisplayedFps, 2), app, "displayedFps");
            Row(md, "Tile animation FPS", F(s.TileAnimationFps, 2), app, "tileAnimationFps");
            Row(md, "Hitch time ratio (ms/s)", F(s.HitchRatioMsPerSec), app, "hitchRatioMsPerSec");
            Row(md, "Hitches", s.HitchCount.ToString(CultureInfo.InvariantCulture), app, "hitchCount");
            Row(md, "Pacing error RMS (ms)", F(s.PacingErrorRmsMs), app, "pacingErrorRmsMs");
            Row(md, "Pacing error P99 (ms)", F(s.PacingErrorP99Ms), app, "pacingErrorP99Ms");
            Row(md, "Judder (%)", F(s.JudderPct), app, "judderPct");
            Row(md, "Repeated refreshes per s", F(s.RepeatedRefreshesPerSec), app, "repeatedRefreshesPerSec");
            Row(md, "Ticks", s.TickCount.ToString(CultureInfo.InvariantCulture), app, "tickCount");
            Row(md, "Painted", s.PaintedCount.ToString(CultureInfo.InvariantCulture), app, "paintedCount");
            Row(md, "Displayed", s.DisplayedCount.ToString(CultureInfo.InvariantCulture), app, "displayedCount");
            Row(md, "Dropped (painted, never shown)", s.DroppedCount.ToString(CultureInfo.InvariantCulture), app, "droppedCount");
            Row(md, "Coalesced", s.CoalescedCount.ToString(CultureInfo.InvariantCulture), app, "coalescedCount");
            Row(md, "Paint not run", s.NotRunCount.ToString(CultureInfo.InvariantCulture), app, "notRunCount");
            Row(md, "Paused gaps", s.PausedGapCount.ToString(CultureInfo.InvariantCulture), app, "pausedGapCount");
            Row(md, "Window (ms, pauses excluded)", F(s.WindowMs, 1), app, "windowMs");
            Row(md, "Latency P50 (ms)", F(s.LatencyP50Ms), app, "latencyP50Ms");
            Row(md, "Latency P99 (ms)", F(s.LatencyP99Ms), app, "latencyP99Ms");
            Row(md, "Callback lateness P99 (ms)", F(s.CallbackLatenessP99Ms), app, "callbackLatenessP99Ms");
            Row(md, "Paint P50 (ms)", F(s.PaintP50Ms), app, "paintP50Ms");
            Row(md, "Paint P99 (ms)", F(s.PaintP99Ms), app, "paintP99Ms");
            Row(md, "GC count (gen 0)", s.GcCount.ToString(CultureInfo.InvariantCulture), app, "gcCount");
            Row(md, "GC pause (ms, total)", s.GcPauseDataAvailable ? F(s.GcPauseMs, 1) : "not reported", app, "gcPauseMs");
            Row(md, "Measured refresh (Hz)", F(s.MeasuredRefreshHz), app, "measuredRefreshHz");
            Row(md, "Callback rate (Hz)", s.CallbackRefreshHz > 0 ? F(s.CallbackRefreshHz) : "not recorded", app, "callbackRefreshHz");
            Row(md, "Assumed refresh (Hz)", F(s.AssumedRefreshHz, 0), app, "assumedRefreshHz");
            Row(md, "Target FPS", F(s.TargetFps, 0), app, "targetFps");
            md.AppendLine("| Assumed refresh mismatch (> 5 %) | " + (s.AssumedRefreshMismatch ? "**yes**" : "no") + " | "
                + (res.Run.AssumedRefreshMismatch ? "yes" : "no") + " |");
            md.AppendLine("| Present source | " + s.PresentSource + " (" + measured + " measured, " + estimated + " estimated) | "
                + (res.Run.PresentSource ?? "") + " |");
            md.AppendLine();
            if (s.AssumedRefreshMismatch)
                md.AppendLine("> **Warning:** the pacing logic assumed " + F(s.AssumedRefreshHz, 0) + " Hz while the panel ran at "
                    + F(s.MeasuredRefreshHz) + " Hz; its divisor pattern is pacing against the wrong rate.");
            if (estimated > 0 && measured > 0)
                md.AppendLine("> **Note:** display times are mixed: " + measured + " measured, " + estimated
                    + " estimated. A gap between a measured and an estimated frame carries the pipeline latency the estimate does not model.");
            if (!s.GcPauseDataAvailable)
                md.AppendLine("> **Note:** this capture has no GC pause time (the runtime does not report it, or the capture predates it); a late callback with a collection in its gap is charged to GC by the collection counts alone.");
            if (s.AssumedRefreshMismatch || (estimated > 0 && measured > 0) || !s.GcPauseDataAvailable)
                md.AppendLine();
        }

        /* The collections since the previous tick, e.g. "g0 2.4 ms" or "g2", or "" for none */
        private static string GcText(CapturedTimeline t, int i)
        {
            if (i <= 0)
                return "";
            GHFrameRecord r = t.Records[i], q = t.Records[i - 1];
            string gen = r.GcCount2 != q.GcCount2 ? "g2" : r.GcCount1 != q.GcCount1 ? "g1" : r.GcCount0 != q.GcCount0 ? "g0" : "";
            long pause = r.GcPauseTicks > q.GcPauseTicks ? r.GcPauseTicks - q.GcPauseTicks : 0;
            if (pause == 0)
                return gen;
            return (gen.Length > 0 ? gen + " " : "") + F(t.Clock.DurationTicksToMs(pause), 1) + " ms";
        }

        private static void Row(StringBuilder md, string label, string recomputed, Dictionary<string, double> app, string key)
        {
            string reported = app.TryGetValue(key, out double v) ? F(v) : "";
            md.AppendLine("| " + label + " | " + recomputed + " | " + reported + " |");
        }

        private static void WriteCauses(StringBuilder md, SmoothnessResult res)
        {
            GHSmoothnessSummary s = res.Summary;
            double total = 0;
            for (int i = 1; i < GHSmoothnessMetrics.CauseCount; i++)
                total += s.CauseMs[i];
            md.AppendLine("## Hitch causes");
            md.AppendLine();
            md.AppendLine("Every hitch and every judder frame is charged to the first pipeline stage that exceeded its budget. "
                + "Hitch ms is the time beyond the target period, summed over hitches only.");
            md.AppendLine();
            md.AppendLine("| Cause | Count | Hitch ms | Share of hitch time | App count | App hitch ms |");
            md.AppendLine("|---|---|---|---|---|---|");
            for (int i = 1; i < GHSmoothnessMetrics.CauseCount; i++)
            {
                string name = GHSmoothnessMetrics.CauseName((GHHitchCause)i);
                bool haveApp = res.Run.Causes.TryGetValue(name, out (int Count, double HitchMs) appCause);
                if (s.CauseCount[i] == 0 && s.CauseMs[i] == 0 && !haveApp)
                    continue;
                md.AppendLine("| " + name + " | " + s.CauseCount[i] + " | " + F(s.CauseMs[i], 1)
                    + " | " + (total > 0 ? F(100.0 * s.CauseMs[i] / total, 1) + " %" : "n/a")
                    + " | " + (haveApp ? appCause.Count.ToString(CultureInfo.InvariantCulture) : "")
                    + " | " + (haveApp ? F(appCause.HitchMs, 1) : "") + " |");
            }
            md.AppendLine();
            md.AppendLine("Unattributed share of hitch time: " + F(100.0 * s.UnattributedShare, 1) + " %.");
            md.AppendLine();
        }

        /* Hitch rate in gaps where a kind of content appeared, against gaps where nothing did.
           The p-value is Fisher's exact test, one-sided: the chance of at least that many
           hitches among the event gaps if hitches fell on event and quiet gaps alike. */
        private static void WriteContentEvents(StringBuilder md, SmoothnessResult res)
        {
            GHSmoothnessSummary s = res.Summary;
            md.AppendLine("## Content events");
            md.AppendLine();
            int quietGaps = s.QuietGapCount;
            int quietHitches = s.QuietHitchCount;
            bool any = false;
            for (int k = 0; k < GHSmoothnessMetrics.ContentEventKinds; k++)
                any |= s.EventGapCount[k] > 0;
            if (!any)
            {
                md.AppendLine("No content events were recorded (the capture predates them, or nothing appeared).");
                md.AppendLine();
                return;
            }
            double quietRate = quietGaps > 0 ? (double)quietHitches / quietGaps : double.NaN;
            md.AppendLine("A gap is the time between two displayed frames; an event belongs to the gap in which the UI thread received it (map updates: in which a paint first drew them). "
                + "Gaps with no event: " + quietGaps + ", of which " + quietHitches + " hitches ("
                + (double.IsNaN(quietRate) ? "n/a" : F(100.0 * quietRate, 1) + " %") + ").");
            md.AppendLine();
            md.AppendLine("| Event | Gaps | Hitches | Hitch rate | Relative to quiet gaps | p (Fisher, one-sided) |");
            md.AppendLine("|---|---|---|---|---|---|");
            for (int k = 0; k < GHSmoothnessMetrics.ContentEventKinds; k++)
            {
                int gaps = s.EventGapCount[k];
                if (gaps == 0)
                    continue;
                int hitches = s.EventHitchCount[k];
                double rate = (double)hitches / gaps;
                string relative = double.IsNaN(quietRate) ? "n/a"
                    : quietRate == 0 ? (hitches > 0 ? "quiet gaps had none" : "both none")
                    : F(rate / quietRate, 1) + " x";
                double p = quietGaps > 0 ? FisherGreater(hitches, gaps - hitches, quietHitches, quietGaps - quietHitches) : double.NaN;
                md.AppendLine("| " + GHSmoothnessMetrics.ContentEventName(k) + " | " + gaps + " | " + hitches
                    + " | " + F(100.0 * rate, 1) + " % | " + relative
                    + " | " + (double.IsNaN(p) ? "n/a" : (p < 0.0001 ? "< 0.0001" : F(p, 4))) + " |");
            }
            md.AppendLine();
            md.AppendLine("Kinds often arrive together (a hit can bring a floating text, a message and a map update in one gap), so a high rate for one kind can be carried by another; compare the worst-hitch tables' Events column.");
            md.AppendLine();
        }

        /* P(X >= a) for the top-left cell of the 2x2 table [a b; c d] with fixed margins
           (hypergeometric), computed with log factorials */
        public static double FisherGreater(int a, int b, int c, int d)
        {
            int n = a + b + c + d;
            int row1 = a + b, col1 = a + c;
            double[] logFact = new double[n + 1];
            for (int i = 1; i <= n; i++)
                logFact[i] = logFact[i - 1] + Math.Log(i);
            double logDenominator = logFact[n] - logFact[row1] - logFact[n - row1] - logFact[col1] - logFact[n - col1];
            int max = Math.Min(row1, col1);
            double p = 0;
            for (int x = a; x <= max; x++)
            {
                int bx = row1 - x, cx = col1 - x, dx = n - row1 - col1 + x;
                if (bx < 0 || cx < 0 || dx < 0)
                    continue;
                p += Math.Exp(-(logFact[x] + logFact[bx] + logFact[cx] + logFact[dx]) - logDenominator);
            }
            return Math.Min(1.0, p);
        }

        private static void WriteWorstHitches(StringBuilder md, SmoothnessResult res)
        {
            CapturedTimeline t = res.Timeline;
            md.AppendLine("## Worst hitches");
            md.AppendLine();
            List<int> hitches = new List<int>();
            for (int j = 1; j < res.DisplayedCount; j++)
            {
                if (res.Displayed[j].IsHitch)
                    hitches.Add(j);
            }
            if (hitches.Count == 0)
            {
                md.AppendLine("No hitches.");
                md.AppendLine();
                return;
            }
            hitches = hitches.OrderByDescending(j => res.Displayed[j].GapTicks - res.Displayed[j].TargetPeriodTicks)
                .ThenBy(j => j).Take(WorstHitchCount).ToList();

            Dictionary<int, GHDisplayedFrame> byRecord = new Dictionary<int, GHDisplayedFrame>();
            for (int j = 0; j < res.DisplayedCount; j++)
                byRecord[res.Displayed[j].RecordIndex] = res.Displayed[j];
            Dictionary<long, string> jank = new Dictionary<long, string>();
            foreach (JoinReport jr in res.Joins)
            {
                foreach (KeyValuePair<long, string> kv in jr.JankTypeByFrameId)
                    jank[kv.Key] = kv.Value;
            }

            md.AppendLine("Times are ms since the first tick's callback start. Each table covers the ticks from the frame shown before the gap to the frame that ended it.");
            md.AppendLine();
            int rank = 0;
            foreach (int j in hitches)
            {
                rank++;
                GHDisplayedFrame d = res.Displayed[j];
                GHDisplayedFrame prev = res.Displayed[j - 1];
                double gapMs = t.Clock.DurationTicksToMs(d.GapTicks);
                double overMs = t.Clock.DurationTicksToMs(d.GapTicks - d.TargetPeriodTicks);
                md.AppendLine("### " + rank + ". Frame " + d.FrameId + " at " + F(t.Ms(d.DisplayedAtTicks), 1) + " ms: gap "
                    + F(gapMs, 1) + " ms (+" + F(overMs, 1) + "), content step " + d.ContentStep + ", cause "
                    + GHSmoothnessMetrics.CauseName(d.Cause) + ", " + d.Source.ToString().ToLowerInvariant());
                md.AppendLine();
                md.AppendLine("| Frame | Vsync | Callback | Requests ms | GC | Events | Invalidate | Paint start | Lock attempt/result | Draw end | Flush end | Displayed | Pacing | Paint | Cause | Jank type |");
                md.AppendLine("|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|");
                int from = prev.RecordIndex;
                int to = d.RecordIndex;
                if (to - from + 1 > MaxStageRows)
                {
                    md.AppendLine("| ... " + (to - from + 1 - MaxStageRows) + " earlier ticks omitted | | | | | | | | | | | | | | | |");
                    from = to - MaxStageRows + 1;
                }
                for (int i = from; i <= to; i++)
                {
                    GHFrameRecord r = t.Records[i];
                    string displayed;
                    string cause = "";
                    if (byRecord.TryGetValue(i, out GHDisplayedFrame df))
                    {
                        displayed = T(t, df.DisplayedAtTicks) + (df.Source == GHPresentSource.Measured ? " (m)" : " (e)");
                        if (i == d.RecordIndex)
                            cause = GHSmoothnessMetrics.CauseName(df.Cause);
                    }
                    else
                    {
                        displayed = r.Paint == GHPaintOutcome.Painted && r.FlushEndTicks != 0 ? "dropped" : "";
                    }
                    string lockText = r.LockAttemptTicks == 0 ? "" : T(t, r.LockAttemptTicks) + " / " + T(t, r.LockResultTicks) + (r.LockAcquired ? "" : " (not acquired)");
                    md.AppendLine("| " + r.FrameId
                        + " | " + T(t, r.VsyncTicks)
                        + " | " + T(t, r.CallbackStartTicks) + (r.CallbackEndTicks != 0 ? " to " + T(t, r.CallbackEndTicks) : "")
                        + " | " + (r.RequestTicks != 0 ? F(t.Clock.DurationTicksToMs(r.RequestTicks), 2) : "")
                        + " | " + GcText(t, i)
                        + " | " + GHSmoothnessMetrics.ContentEventNames(r.ContentEvents)
                        + " | " + T(t, r.InvalidateTicks)
                        + " | " + T(t, r.PaintStartTicks) + (r.PaintStartTicks != 0 ? (r.PaintOnUiThread ? " UI" : " GL") : "")
                        + " | " + lockText
                        + " | " + T(t, r.DrawEndTicks)
                        + " | " + T(t, r.FlushEndTicks)
                        + " | " + displayed
                        + " | " + r.Pacing
                        + " | " + r.Paint
                        + " | " + cause
                        + " | " + (jank.TryGetValue(r.FrameId, out string jt) ? jt : "") + " |");
                }
                md.AppendLine();
            }
        }

        private static void WriteChangePoints(StringBuilder md, SmoothnessResult res)
        {
            ChangePoints.Result c = res.Changes;
            md.AppendLine("## Change points");
            md.AppendLine();
            md.AppendLine("PELT over " + F(ChangePoints.BucketMs, 0) + " ms buckets of displayed FPS and of pacing-error RMS, Gaussian mean-change cost, "
                + "minimum segment " + ChangePoints.MinSegmentBuckets + " buckets, penalty 2 sigma^2 ln(n) with sigma from the MAD of first differences "
                + "(floored; FPS penalty " + F(c.FpsPenalty, 1) + ", pacing penalty " + F(c.PacingPenalty, 2) + "). Buckets: " + c.Series.Count
                + ", of which " + c.Series.Missing.Count(m => m) + " hold a pause and are left out.");
            md.AppendLine();
            if (c.Series.Missing.Count(m => !m) < 2 * ChangePoints.MinSegmentBuckets)
            {
                md.AppendLine("The window is too short for change-point detection.");
                md.AppendLine();
            }
            else
            {
                md.AppendLine("| Segment | Start s | End s | Mean displayed FPS | Mean pacing error RMS ms |");
                md.AppendLine("|---|---|---|---|---|");
                int k = 0;
                foreach (ChangePoints.Segment s in c.Segments)
                {
                    k++;
                    md.AppendLine("| " + k + " | " + F(s.StartS) + " | " + F(s.EndS) + " | " + F(s.MeanDisplayedFps, 1) + " | " + F(s.MeanPacingErrorMs) + " |");
                }
                md.AppendLine();
                if (c.Boundaries.Count == 0)
                {
                    md.AppendLine("No change points.");
                    md.AppendLine();
                }
                foreach (ChangePoints.Boundary b in c.Boundaries)
                {
                    string which = b.InFps && b.InPacing ? "displayed FPS and pacing error" : b.InFps ? "displayed FPS" : "pacing error";
                    md.AppendLine("**Change at " + F(b.AtS) + " s** (" + which + "). Events within " + F(ChangePoints.EventWindowMs, 0) + " ms:"
                        + (b.Events.Count == 0 ? " none recorded." : ""));
                    const int maxEvents = 12;
                    foreach (ChangePoints.TimelineEvent e in b.Events.Take(maxEvents))
                        md.AppendLine("- " + F(e.AtMs / 1000.0, 3) + " s: " + e.Text);
                    if (b.Events.Count > maxEvents)
                        md.AppendLine("- and " + (b.Events.Count - maxEvents) + " more");
                    md.AppendLine();
                }
            }
            md.AppendLine("Thermal state: " + (res.Run.ThermalBefore?.Status ?? "unknown") + " before, " + (res.Run.ThermalAfter?.Status ?? "unknown")
                + " after. These come from the run record's before and after readings only; the timeline has no in-run thermal samples, "
                + "so a thermal change cannot be placed at a boundary.");
            md.AppendLine();
        }

        private static string T(CapturedTimeline t, long ticks)
        {
            return ticks == 0 ? "" : F(t.Clock.TicksToMs(ticks), 2);
        }

        private static string F(double v, int decimals = 2)
        {
            if (double.IsNaN(v))
                return "n/a";
            return v.ToString("F" + decimals, CultureInfo.InvariantCulture);
        }
    }
}
