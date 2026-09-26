using System.Globalization;
using System.Text;
using GnollHack.PerformanceAnalyzer.Model;
using GnollHackX.Performance;

namespace GnollHack.PerformanceAnalyzer.Readers
{
    /* Reads and writes the app's frametimeline*.csv (GHFrameTimeline.DumpToCsv). Columns are
       found by header name, so a reordered or extended file still reads; a missing column
       reads as zero. Times come back as ticks of this machine's Stopwatch (TimelineClock).
       Enum columns are written by name and parsed by name, or by number as a fallback. */
    public static class FrameTimelineCsv
    {
        public const string Header =
            "FrameId,VsyncMs,ExpectedPresentMs,PlatformFrameMs,RefreshPeriodMs,CallbackStartMs,CallbackEndMs,"
            + "TargetFps,AssumedRefreshHz,Pacing,MainCounter,GeneralCounter,Invalidate,InvalidateMs,"
            + "Paint,PaintOnUiThread,PaintStartMs,LockAttemptMs,LockResultMs,LockAcquired,DrawEndMs,FlushEndMs,"
            + "PaintedMainCounter,PaintedGeneralCounter,PaintedMapGeneration,DisplayedAtMs,PresentSource,Flags,Gc0,Gc1,Gc2";

        /* fallbackFrequency is used when the file has no "# StopwatchFrequency=" line, e.g.
           the run JSON's clock.stopwatchFrequency */
        public static CapturedTimeline Read(string path, long fallbackFrequency = 0)
        {
            string[] lines = File.ReadAllLines(path);
            CapturedTimeline t = new CapturedTimeline { Path = path };
            int headerLine = -1;
            for (int i = 0; i < lines.Length; i++)
            {
                string l = lines[i].Trim();
                if (l.Length == 0)
                    continue;
                if (l.StartsWith("#"))
                {
                    t.MetadataLines.Add(l);
                    ParseMetadata(l, t.Metadata);
                    continue;
                }
                headerLine = i;
                break;
            }
            if (headerLine < 0)
                throw new InvalidDataException("frame timeline CSV has no header row: " + path);

            long frequency = MetaLong(t.Metadata, "StopwatchFrequency") ?? fallbackFrequency;
            long? origin = MetaLong(t.Metadata, "OriginStopwatchTicks");
            t.Clock = new TimelineClock(frequency, origin);

            Dictionary<string, int> c = Csv.HeaderIndex(Csv.Split(lines[headerLine]));
            if (!c.ContainsKey("FrameId") || !c.ContainsKey("CallbackStartMs"))
                throw new InvalidDataException("frame timeline CSV lacks FrameId or CallbackStartMs: " + path);

            List<GHFrameRecord> records = new List<GHFrameRecord>(lines.Length);
            for (int i = headerLine + 1; i < lines.Length; i++)
            {
                if (lines[i].Trim().Length == 0 || lines[i].TrimStart().StartsWith("#"))
                    continue;
                string[] f = Csv.Split(lines[i]);
                GHFrameRecord r = new GHFrameRecord();
                r.FrameId = Long(f, c, "FrameId");
                r.VsyncTicks = Time(f, c, "VsyncMs", t.Clock);
                r.ExpectedPresentTicks = Time(f, c, "ExpectedPresentMs", t.Clock);
                r.PlatformFrameTicks = Time(f, c, "PlatformFrameMs", t.Clock);
                r.RefreshPeriodTicks = Duration(f, c, "RefreshPeriodMs", t.Clock);
                r.CallbackStartTicks = Time(f, c, "CallbackStartMs", t.Clock);
                r.CallbackEndTicks = Time(f, c, "CallbackEndMs", t.Clock);
                r.TargetFps = (short)Long(f, c, "TargetFps");
                r.AssumedRefreshHz = (short)Long(f, c, "AssumedRefreshHz");
                r.Pacing = Enum<GHPacingDecision>(f, c, "Pacing");
                r.MainCounter = Long(f, c, "MainCounter");
                r.GeneralCounter = Long(f, c, "GeneralCounter");
                r.Invalidate = Enum<GHInvalidateOutcome>(f, c, "Invalidate");
                r.InvalidateTicks = Time(f, c, "InvalidateMs", t.Clock);
                r.Paint = Enum<GHPaintOutcome>(f, c, "Paint");
                r.PaintOnUiThread = Long(f, c, "PaintOnUiThread") != 0;
                r.PaintStartTicks = Time(f, c, "PaintStartMs", t.Clock);
                r.LockAttemptTicks = Time(f, c, "LockAttemptMs", t.Clock);
                r.LockResultTicks = Time(f, c, "LockResultMs", t.Clock);
                r.LockAcquired = Long(f, c, "LockAcquired") != 0;
                r.DrawEndTicks = Time(f, c, "DrawEndMs", t.Clock);
                r.FlushEndTicks = Time(f, c, "FlushEndMs", t.Clock);
                r.PaintedMainCounter = Long(f, c, "PaintedMainCounter");
                r.PaintedGeneralCounter = Long(f, c, "PaintedGeneralCounter");
                r.PaintedMapGeneration = Long(f, c, "PaintedMapGeneration");
                r.DisplayedAtTicks = Time(f, c, "DisplayedAtMs", t.Clock);
                r.PresentSource = Enum<GHPresentSource>(f, c, "PresentSource");
                r.Flags = (GHFrameFlags)Long(f, c, "Flags");
                r.GcCount0 = (int)Long(f, c, "Gc0");
                r.GcCount1 = (int)Long(f, c, "Gc1");
                r.GcCount2 = (int)Long(f, c, "Gc2");
                records.Add(r);
            }
            /* GHSmoothnessMetrics wants FrameId order, which is the order the app writes */
            records.Sort((a, b) => a.FrameId.CompareTo(b.FrameId));
            t.Records = records.ToArray();
            t.Count = t.Records.Length;
            return t;
        }

        /* Writes the same format GHFrameTimeline.DumpToCsv produces. The original metadata
           lines are kept when the timeline was read from a file. */
        public static void Write(string path, CapturedTimeline t)
        {
            StringBuilder sb = new StringBuilder();
            TimelineClock clock = t.Clock;
            if (t.MetadataLines.Count > 0)
            {
                foreach (string l in t.MetadataLines)
                    sb.Append(l).Append(Csv.Crlf);
            }
            else
            {
                sb.Append("# GHFrameTimeline v1").Append(Csv.Crlf);
                sb.Append("# StopwatchFrequency=" + clock.DeviceFrequency.ToString(CultureInfo.InvariantCulture)).Append(Csv.Crlf);
                if (clock.OriginDeviceTicks.HasValue)
                    sb.Append("# OriginStopwatchTicks=" + clock.OriginDeviceTicks.Value.ToString(CultureInfo.InvariantCulture)).Append(Csv.Crlf);
            }
            sb.Append(Header).Append(Csv.Crlf);

            long platformOrigin = 0;
            for (int i = 0; i < t.Count && platformOrigin == 0; i++)
                platformOrigin = t.Records[i].PlatformFrameTicks;

            for (int i = 0; i < t.Count; i++)
            {
                GHFrameRecord r = t.Records[i];
                sb.Append(string.Join(",", new string[]
                {
                    r.FrameId.ToString(CultureInfo.InvariantCulture),
                    TimeText(r.VsyncTicks, clock),
                    TimeText(r.ExpectedPresentTicks, clock),
                    r.PlatformFrameTicks == 0 ? "" : MsText(clock.DurationTicksToMs(r.PlatformFrameTicks - platformOrigin)),
                    r.RefreshPeriodTicks == 0 ? "" : MsText(clock.DurationTicksToMs(r.RefreshPeriodTicks)),
                    TimeText(r.CallbackStartTicks, clock),
                    TimeText(r.CallbackEndTicks, clock),
                    r.TargetFps.ToString(CultureInfo.InvariantCulture),
                    r.AssumedRefreshHz.ToString(CultureInfo.InvariantCulture),
                    r.Pacing.ToString(),
                    r.MainCounter.ToString(CultureInfo.InvariantCulture),
                    r.GeneralCounter.ToString(CultureInfo.InvariantCulture),
                    r.Invalidate.ToString(),
                    TimeText(r.InvalidateTicks, clock),
                    r.Paint.ToString(),
                    r.PaintOnUiThread ? "1" : "0",
                    TimeText(r.PaintStartTicks, clock),
                    TimeText(r.LockAttemptTicks, clock),
                    TimeText(r.LockResultTicks, clock),
                    r.LockAcquired ? "1" : "0",
                    TimeText(r.DrawEndTicks, clock),
                    TimeText(r.FlushEndTicks, clock),
                    r.PaintedMainCounter.ToString(CultureInfo.InvariantCulture),
                    r.PaintedGeneralCounter.ToString(CultureInfo.InvariantCulture),
                    r.PaintedMapGeneration.ToString(CultureInfo.InvariantCulture),
                    TimeText(r.DisplayedAtTicks, clock),
                    r.PresentSource.ToString(),
                    ((int)r.Flags).ToString(CultureInfo.InvariantCulture),
                    r.GcCount0.ToString(CultureInfo.InvariantCulture),
                    r.GcCount1.ToString(CultureInfo.InvariantCulture),
                    r.GcCount2.ToString(CultureInfo.InvariantCulture)
                })).Append(Csv.Crlf);
            }
            File.WriteAllText(path, sb.ToString(), new UTF8Encoding(false));
        }

        /* "# Key=Value Key2=Value2" and "# Prefix Key=Value"; a lone "# Word Value" is
           stored as Word=Value */
        internal static void ParseMetadata(string line, Dictionary<string, string> meta)
        {
            string body = line.TrimStart('#').Trim();
            if (body.Length == 0)
                return;
            string[] tokens = body.Split(new[] { ' ', '\t' }, StringSplitOptions.RemoveEmptyEntries);
            string prefix = null;
            for (int i = 0; i < tokens.Length; i++)
            {
                string tok = tokens[i];
                int eq = tok.IndexOf('=');
                if (eq > 0)
                {
                    string key = tok.Substring(0, eq);
                    meta[prefix != null ? prefix + "." + key : key] = tok.Substring(eq + 1);
                }
                else if (i == 0)
                {
                    prefix = tok;
                    if (tokens.Length == 2 && tokens[1].IndexOf('=') < 0)
                    {
                        meta[tok] = tokens[1];
                        return;
                    }
                }
            }
        }

        internal static long? MetaLong(Dictionary<string, string> meta, string key)
        {
            if (meta.TryGetValue(key, out string v)
                && long.TryParse(v, NumberStyles.Integer, CultureInfo.InvariantCulture, out long l))
                return l;
            return null;
        }

        private static string MsText(double ms)
        {
            return ms.ToString("0.000", CultureInfo.InvariantCulture);
        }

        private static string TimeText(long ticks, TimelineClock clock)
        {
            return ticks == 0 ? "" : MsText(clock.TicksToMs(ticks));
        }

        internal static long Time(string[] f, Dictionary<string, int> c, string name, TimelineClock clock)
        {
            if (!c.TryGetValue(name, out int i) || !Csv.TryDouble(Csv.Field(f, i), out double ms))
                return 0;
            return clock.MsToTicks(ms);
        }

        internal static long Duration(string[] f, Dictionary<string, int> c, string name, TimelineClock clock)
        {
            if (!c.TryGetValue(name, out int i) || !Csv.TryDouble(Csv.Field(f, i), out double ms))
                return 0;
            return clock.DurationMsToTicks(ms);
        }

        internal static long Long(string[] f, Dictionary<string, int> c, string name)
        {
            return c.TryGetValue(name, out int i) ? Csv.LongOr(f, i, 0) : 0;
        }

        internal static T Enum<T>(string[] f, Dictionary<string, int> c, string name) where T : struct, System.Enum
        {
            if (!c.TryGetValue(name, out int i))
                return default;
            string v = Csv.Field(f, i);
            if (v == null)
                return default;
            return System.Enum.TryParse(v.Trim(), true, out T value) ? value : default;
        }
    }
}
