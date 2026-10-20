using System;

namespace GnollHackM
{
    /* Refresh-rate estimate from the last WindowSize frame deltas. Vsync-paced frames use the median, which
       a missed frame does not move. Otherwise frames follow Avalonia's render timer irregularly, and GHApp's
       frame skipping needs the actual frame rate: the mean without the largest TrimPercent of the deltas, so
       that a hitch does not move it either. An estimate within SnapPercent of the monitor's refresh rate is
       that rate. Free of Avalonia types, so that the unit tests compile it; not thread-safe. */
    public sealed class GHRefreshRateEstimator
    {
        public const int WindowSize = 120;
        public const int MinimumCount = 60;
        public const int TrimPercent = 5;
        public const int SnapPercent = 5;
        public const float ChangeThresholdHz = 1.0f;

        private readonly double[] _deltasMs = new double[WindowSize];
        private readonly double[] _scratch = new double[WindowSize];
        private int _index = 0;
        private int _count = 0;
        private bool _hasPublished = false;
        private float _published = 0.0f;

        /* The median instead of the trimmed mean */
        public bool VsyncPaced { get; set; }

        /* The monitor's display-mode refresh rate; 0 when unknown */
        public float MonitorRefreshRate { get; set; }

        /* 0 until MinimumCount deltas exist */
        public float Published
        {
            get { return _published; }
        }

        /* Returns true when Published changed */
        public bool AddDelta(double deltaMs)
        {
            if (!(deltaMs > 0.0))
                return false;

            _deltasMs[_index] = deltaMs;
            _index = (_index + 1) % WindowSize;
            if (_count < WindowSize)
                _count++;
            if (_count < MinimumCount)
                return false;

            int n = _count;
            Array.Copy(_deltasMs, _scratch, n);
            Array.Sort(_scratch, 0, n);

            double periodMs;
            if (VsyncPaced)
            {
                periodMs = (n & 1) != 0 ? _scratch[n / 2] : (_scratch[n / 2 - 1] + _scratch[n / 2]) / 2.0;
            }
            else
            {
                int kept = n - (n * TrimPercent + 99) / 100;
                double sum = 0.0;
                int i;
                for (i = 0; i < kept; i++)
                    sum += _scratch[i];
                periodMs = sum / kept;
            }
            if (!(periodMs > 0.0))
                return false;

            float hz = (float)(1000.0 / periodMs);
            float monitor = MonitorRefreshRate;
            bool snapped = false;
            if (monitor > 0.0f && Math.Abs(hz - monitor) <= monitor * SnapPercent / 100.0f)
            {
                hz = monitor;
                snapped = true;
            }

            if (_hasPublished)
            {
                bool snappedChange = snapped && hz != _published;
                if (!snappedChange && Math.Abs(hz - _published) <= ChangeThresholdHz)
                    return false;
            }
            _hasPublished = true;
            _published = hz;
            return true;
        }

        public void Reset()
        {
            _index = 0;
            _count = 0;
            _hasPublished = false;
            _published = 0.0f;
        }
    }
}
