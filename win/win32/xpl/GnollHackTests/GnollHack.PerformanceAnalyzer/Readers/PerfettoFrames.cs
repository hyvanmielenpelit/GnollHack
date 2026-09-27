namespace GnollHack.PerformanceAnalyzer.Readers
{
    /* One row of perfetto_frames.csv (DEVEL/performance/perfetto/export_frames.sql):
       the app's frame as SurfaceFlinger's frame timeline saw it. Times in trace-clock ns. */
    public sealed class PerfettoFrame
    {
        public long Token;
        public long ActualTs;
        public long ActualDur;
        public long? ExpectedTs;
        public long? ExpectedDur;
        public string LayerName;
        public string PresentType;
        public string OnTimeFinish;
        public string GpuComposition;
        public string JankType;
        public string PredictionType;

        public long PresentedAt { get { return ActualTs + ActualDur; } }
        public bool IsDropped
        {
            get { return PresentType != null && PresentType.IndexOf("Dropped", StringComparison.OrdinalIgnoreCase) >= 0; }
        }
    }

    /* One row of perfetto_app_slices.csv (export_app_slices.sql): a GH.Tick, GH.Paint or
       GH.Flush section with the in-app FrameId and, for ticks, the Choreographer frame
       token of the doFrame that contains it */
    public sealed class PerfettoSlice
    {
        public long Ts;
        public long Dur;
        public string Name;
        public string ThreadName;
        public long? FrameId;
        public long? VsyncToken;
    }

    public sealed class PerfettoCapture
    {
        public string DirectoryPath;
        public List<PerfettoFrame> Frames = new List<PerfettoFrame>();
        public List<PerfettoSlice> Slices = new List<PerfettoSlice>();
        public int SkippedFrameRows;
        public int SkippedSliceRows;
    }

    /* Reads the two CSVs trace_processor_shell exports for Capture-AndroidFrames.ps1. The
       shell prints a header row, then comma-separated values with strings double-quoted
       and NULL as "[NULL]"; any line before the header row is ignored, and a row that
       does not parse is counted and skipped. */
    public static class PerfettoFrames
    {
        public const string FramesFile = "perfetto_frames.csv";
        public const string SlicesFile = "perfetto_app_slices.csv";

        public static PerfettoCapture ReadDirectory(string directory)
        {
            string frames = Path.Combine(directory, FramesFile);
            string slices = Path.Combine(directory, SlicesFile);
            if (!File.Exists(frames) || !File.Exists(slices))
                throw new FileNotFoundException("Perfetto directory needs " + FramesFile + " and " + SlicesFile + ": " + directory);
            PerfettoCapture cap = new PerfettoCapture { DirectoryPath = directory };
            ReadFrames(frames, cap);
            ReadSlices(slices, cap);
            return cap;
        }

        public static void ReadFrames(string path, PerfettoCapture cap)
        {
            foreach (string[] f in Rows(path, "token", out Dictionary<string, int> c))
            {
                if (!Csv.TryLong(Get(f, c, "token"), out long token)
                    || !Csv.TryLong(Get(f, c, "actual_ts"), out long ts)
                    || !Csv.TryLong(Get(f, c, "actual_dur"), out long dur))
                {
                    cap.SkippedFrameRows++;
                    continue;
                }
                PerfettoFrame x = new PerfettoFrame { Token = token, ActualTs = ts, ActualDur = dur };
                if (Csv.TryLong(Get(f, c, "expected_ts"), out long ets))
                    x.ExpectedTs = ets;
                if (Csv.TryLong(Get(f, c, "expected_dur"), out long edur))
                    x.ExpectedDur = edur;
                x.LayerName = Get(f, c, "layer_name");
                x.PresentType = Get(f, c, "present_type");
                x.OnTimeFinish = Get(f, c, "on_time_finish");
                x.GpuComposition = Get(f, c, "gpu_composition");
                x.JankType = Get(f, c, "jank_type");
                x.PredictionType = Get(f, c, "prediction_type");
                cap.Frames.Add(x);
            }
        }

        public static void ReadSlices(string path, PerfettoCapture cap)
        {
            foreach (string[] f in Rows(path, "ts", out Dictionary<string, int> c))
            {
                if (!Csv.TryLong(Get(f, c, "ts"), out long ts))
                {
                    cap.SkippedSliceRows++;
                    continue;
                }
                PerfettoSlice s = new PerfettoSlice { Ts = ts };
                if (Csv.TryLong(Get(f, c, "dur"), out long dur))
                    s.Dur = dur;
                s.Name = Get(f, c, "name");
                s.ThreadName = Get(f, c, "thread_name");
                if (Csv.TryLong(Get(f, c, "frame_id"), out long id))
                    s.FrameId = id;
                if (Csv.TryLong(Get(f, c, "vsync_token"), out long token))
                    s.VsyncToken = token;
                cap.Slices.Add(s);
            }
        }

        private static string Get(string[] f, Dictionary<string, int> c, string name)
        {
            return c.TryGetValue(name, out int i) ? Csv.Field(f, i) : null;
        }

        /* The rows after the first line whose fields include keyColumn */
        private static IEnumerable<string[]> Rows(string path, string keyColumn, out Dictionary<string, int> columns)
        {
            string[] lines = File.ReadAllLines(path);
            columns = null;
            int start = lines.Length;
            for (int i = 0; i < lines.Length; i++)
            {
                string[] h = Csv.Split(lines[i]);
                if (h.Any(x => x.Trim().Equals(keyColumn, StringComparison.OrdinalIgnoreCase)))
                {
                    columns = Csv.HeaderIndex(h);
                    start = i + 1;
                    break;
                }
            }
            if (columns == null)
                throw new InvalidDataException("no header row with a '" + keyColumn + "' column: " + path);
            List<string[]> rows = new List<string[]>();
            for (int i = start; i < lines.Length; i++)
            {
                if (lines[i].Trim().Length == 0)
                    continue;
                rows.Add(Csv.Split(lines[i]));
            }
            return rows;
        }
    }
}
