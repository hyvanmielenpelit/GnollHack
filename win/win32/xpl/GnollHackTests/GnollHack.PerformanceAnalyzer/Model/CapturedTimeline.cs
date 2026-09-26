using System.Diagnostics;
using GnollHackX.Performance;

namespace GnollHack.PerformanceAnalyzer.Model
{
    /* Maps the capture's times to Stopwatch ticks of this machine. The app writes times in
       milliseconds relative to the first tick's callback start; GHSmoothnessMetrics converts
       ticks with this machine's Stopwatch.Frequency, so parsing at the local frequency
       reproduces the device's numbers whatever its own frequency was. Zero means "did not
       happen" in a GHFrameRecord, so every time is offset by BaseTicks to keep a capture
       time of 0 ms, or a slightly negative one such as a vsync before the first callback,
       away from zero. */
    public sealed class TimelineClock
    {
        public const long BaseSeconds = 100000;

        public long DeviceFrequency { get; }
        public long LocalFrequency { get; }
        public long BaseTicks { get; }

        /* The capturing device's absolute Stopwatch tick at capture time 0, from the
           "# OriginStopwatchTicks=" metadata line; null when the capture does not carry it */
        public long? OriginDeviceTicks { get; }

        public TimelineClock(long deviceFrequency, long? originDeviceTicks)
            : this(deviceFrequency, originDeviceTicks, Stopwatch.Frequency)
        {
        }

        public TimelineClock(long deviceFrequency, long? originDeviceTicks, long localFrequency)
        {
            DeviceFrequency = deviceFrequency > 0 ? deviceFrequency : localFrequency;
            OriginDeviceTicks = originDeviceTicks;
            LocalFrequency = localFrequency;
            BaseTicks = BaseSeconds * localFrequency;
        }

        /* A capture time, in ms since capture start, as local ticks */
        public long MsToTicks(double ms)
        {
            return BaseTicks + (long)Math.Round(ms * LocalFrequency / 1000.0);
        }

        public double TicksToMs(long ticks)
        {
            return (ticks - BaseTicks) * 1000.0 / LocalFrequency;
        }

        /* A duration in ms as local ticks, with no base */
        public long DurationMsToTicks(double ms)
        {
            return (long)Math.Round(ms * LocalFrequency / 1000.0);
        }

        public double DurationTicksToMs(long ticks)
        {
            return ticks * 1000.0 / LocalFrequency;
        }

        /* An absolute tick of the capturing device (e.g. a Windows QPC value) as capture ms */
        public double DeviceTicksToMs(double deviceTicks)
        {
            if (!OriginDeviceTicks.HasValue)
                throw new InvalidOperationException("the capture carries no OriginStopwatchTicks");
            return (deviceTicks - OriginDeviceTicks.Value) * 1000.0 / DeviceFrequency;
        }
    }

    /* A frame timeline read back from the app's CSV, ready for GHSmoothnessMetrics */
    public sealed class CapturedTimeline
    {
        public string Path { get; set; }
        public TimelineClock Clock { get; set; }
        public GHFrameRecord[] Records { get; set; } = new GHFrameRecord[0];
        public int Count { get; set; }
        public GHCompositorFrame[] Compositor { get; set; } = new GHCompositorFrame[0];
        public int CompositorCount { get; set; }
        public string CompositorPath { get; set; }

        /* '#' metadata: "Key=Value" tokens, a token after a bare word W stored as "W.Key" */
        public Dictionary<string, string> Metadata { get; set; } = new Dictionary<string, string>(StringComparer.OrdinalIgnoreCase);
        public List<string> MetadataLines { get; set; } = new List<string>();

        private Dictionary<long, int> _indexByFrameId;

        public int IndexOfFrameId(long frameId)
        {
            if (_indexByFrameId == null)
            {
                _indexByFrameId = new Dictionary<long, int>(Count);
                for (int i = 0; i < Count; i++)
                    _indexByFrameId[Records[i].FrameId] = i;
            }
            return _indexByFrameId.TryGetValue(frameId, out int idx) ? idx : -1;
        }

        public double Ms(long ticks)
        {
            return ticks == 0 ? double.NaN : Clock.TicksToMs(ticks);
        }
    }
}
