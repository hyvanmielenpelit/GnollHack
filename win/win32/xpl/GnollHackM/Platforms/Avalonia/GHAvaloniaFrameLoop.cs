#if GNH_AVALONIA
using System;
using System.Diagnostics;
using System.Globalization;
using System.Reflection;
using System.Text;
using System.Threading;
using GnollHackX;

namespace GnollHackM
{
    /* State of the GLX swap interval on the GL views' drawables */
    public enum GHSwapIntervalState
    {
        Unknown,
        NotGlx,
        Applied,
        Rejected
    }

    /* The Avalonia build's platform frame loop. It drives GHApp from
       TopLevel.RequestAnimationFrame, estimates the refresh rate from the frame deltas,
       raises Avalonia's render timer rate when it lags the estimate, and gathers pacing
       statistics for the log. UI thread unless noted. */
    public static class GHAvaloniaFrameLoop
    {
        /* The GL control samples glXGetSyncValuesOML once every this many frames */
        public const int PresentSampleFrameInterval = 120;

        private const int EstimateWindowSize = 120;
        private const int EstimateMinimumCount = 60;
        private const float EstimateChangeThresholdHz = 1.0f;
        private const int ReportCapacity = 4096;
        private const int MaxRenderTimerFps = 240;

        private static readonly Action<TimeSpan> _onFrame = OnFrame;
        private static Avalonia.Controls.TopLevel _topLevel = null;
        private static volatile bool _running = false;
        private static bool _frameRequested = false;
        private static bool _hasLastFrameTime = false;
        private static TimeSpan _lastFrameTime;

        /* Refresh-rate estimate from the last EstimateWindowSize frame deltas */
        private static readonly double[] _estimateDeltasMs = new double[EstimateWindowSize];
        private static readonly double[] _estimateScratch = new double[EstimateWindowSize];
        private static int _estimateIndex = 0;
        private static int _estimateCount = 0;
        private static bool _estimatePublished = false;
        private static float _estimatedRefreshRate = 0.0f;

        /* Statistics since the previous report; the present samples arrive on the render thread */
        private static readonly object _statsLock = new object();
        private static readonly double[] _frameDeltasMs = new double[ReportCapacity];
        private static int _frameDeltaCount = 0;
        private static readonly double[] _ustPerFrameMs = new double[ReportCapacity];
        private static int _presentSampleCount = 0;
        private static int _mscNotOneCount = 0;
        private static double _mscPerFrameSum = 0.0;
        private static readonly double[] _reportScratch = new double[ReportCapacity];
        private static bool _hasLastPresentSample = false;
        private static long _lastUst = 0;
        private static long _lastMsc = 0;
        private static long _lastSbc = 0;

        /* GL control draws on the render thread (all GL controls together) and the UI thread's frame callback */
        private static readonly double[] _glDrawIntervalsMs = new double[ReportCapacity];
        private static int _glDrawIntervalCount = 0;
        private static long _lastGlDrawTicks = 0;
        private static readonly double[] _glPaintMs = new double[ReportCapacity];
        private static int _glPaintCount = 0;
        private static readonly double[] _frameCallbackMs = new double[ReportCapacity];
        private static int _frameCallbackCount = 0;

        private static int _swapIntervalState = (int)GHSwapIntervalState.Unknown;

        /* Avalonia's render timer, reached by reflection: AvaloniaLocator, IRenderLoop and
           SleepLoopRenderTimer are [PrivateApi], internal in the reference assemblies */
        private static object _renderTimer = null;
        private static PropertyInfo _desiredFpsProperty = null;
        private static bool _renderTimerUnavailable = false;
        private static long _lastRenderTimerCheckTicks = 0;

        /* Written by the render thread, read by the UI thread */
        public static GHSwapIntervalState SwapIntervalState
        {
            get { return (GHSwapIntervalState)Volatile.Read(ref _swapIntervalState); }
            set { Volatile.Write(ref _swapIntervalState, (int)value); }
        }

        /* 0 until EstimateMinimumCount frame deltas exist. Reflects the display only while the
           swap interval is applied; otherwise it is the frame rate Avalonia's render timer delivers. */
        public static float EstimatedRefreshRate
        {
            get { return Volatile.Read(ref _estimatedRefreshRate); }
        }

        public static void Start(Avalonia.Controls.TopLevel topLevel)
        {
            if (topLevel == null)
                return;
            _topLevel = topLevel;
            _hasLastFrameTime = false;
            _running = true;
            if (!_frameRequested)
            {
                _frameRequested = true;
                topLevel.RequestAnimationFrame(_onFrame);
            }
        }

        /* Any thread; the pending callback, if any, runs once more without re-arming */
        public static void Stop()
        {
            _running = false;
        }

        private static void OnFrame(TimeSpan time)
        {
            _frameRequested = false;
            Avalonia.Controls.TopLevel topLevel = _topLevel;
            if (!_running || topLevel == null)
            {
                _hasLastFrameTime = false;
                return;
            }

            /* Re-armed first: the callback runs before the compositor's update pass */
            _frameRequested = true;
            topLevel.RequestAnimationFrame(_onFrame);

            if (_hasLastFrameTime)
            {
                double deltaMs = (time - _lastFrameTime).TotalMilliseconds;
                if (deltaMs > 0.0)
                {
                    lock (_statsLock)
                    {
                        if (_frameDeltaCount < ReportCapacity)
                            _frameDeltasMs[_frameDeltaCount++] = deltaMs;
                    }
                    UpdateEstimate(deltaMs);
                }
            }
            _lastFrameTime = time;
            _hasLastFrameTime = true;

            MaybeAdjustRenderTimer();
            long callbackStart = Stopwatch.GetTimestamp();
            GHApp.OnPlatformFrame(EstimatedRefreshRate);
            double callbackMs = (Stopwatch.GetTimestamp() - callbackStart) * 1000.0 / Stopwatch.Frequency;
            lock (_statsLock)
            {
                if (_frameCallbackCount < ReportCapacity)
                    _frameCallbackMs[_frameCallbackCount++] = callbackMs;
            }
        }

        /* Render thread: one GL control draw starting at drawStartTicks (Stopwatch); paintMs is negative when
           the draw reused the previous paint */
        public static void RecordGlDraw(long drawStartTicks, double paintMs)
        {
            lock (_statsLock)
            {
                if (_lastGlDrawTicks != 0 && _glDrawIntervalCount < ReportCapacity)
                    _glDrawIntervalsMs[_glDrawIntervalCount++] = (drawStartTicks - _lastGlDrawTicks) * 1000.0 / Stopwatch.Frequency;
                _lastGlDrawTicks = drawStartTicks;
                if (paintMs >= 0.0 && _glPaintCount < ReportCapacity)
                    _glPaintMs[_glPaintCount++] = paintMs;
            }
        }

        private static void UpdateEstimate(double deltaMs)
        {
            _estimateDeltasMs[_estimateIndex] = deltaMs;
            _estimateIndex = (_estimateIndex + 1) % EstimateWindowSize;
            if (_estimateCount < EstimateWindowSize)
                _estimateCount++;
            if (_estimateCount < EstimateMinimumCount)
                return;

            /* Vsync-paced frames: the median delta, which a missed frame does not move. Otherwise frames follow
               Avalonia's render timer irregularly, and GHApp's frame skipping needs the actual frame rate: the mean. */
            int n = _estimateCount;
            double estimateMs;
            if (SwapIntervalState == GHSwapIntervalState.Applied)
            {
                Array.Copy(_estimateDeltasMs, _estimateScratch, n);
                Array.Sort(_estimateScratch, 0, n);
                estimateMs = (n & 1) != 0 ? _estimateScratch[n / 2] : (_estimateScratch[n / 2 - 1] + _estimateScratch[n / 2]) / 2.0;
            }
            else
            {
                double sum = 0.0;
                int i;
                for (i = 0; i < n; i++)
                    sum += _estimateDeltasMs[i];
                estimateMs = sum / n;
            }
            if (estimateMs <= 0.0)
                return;

            float hz = (float)(1000.0 / estimateMs);
            if (!_estimatePublished || Math.Abs(hz - EstimatedRefreshRate) > EstimateChangeThresholdHz)
            {
                _estimatePublished = true;
                Volatile.Write(ref _estimatedRefreshRate, hz);
                GHAvaloniaDeviceDisplay.Current?.OnRefreshRateEstimated(hz);
            }
        }

        /* Once a second while the swap interval is applied: raises SleepLoopRenderTimer.DesiredFps
           to twice the estimate when it is below 1.5 times the estimate (a stale RandR rate) */
        private static void MaybeAdjustRenderTimer()
        {
            if (_renderTimerUnavailable || SwapIntervalState != GHSwapIntervalState.Applied)
                return;
            float hz = EstimatedRefreshRate;
            if (hz <= 0.0f)
                return;
            long now = Stopwatch.GetTimestamp();
            if (_lastRenderTimerCheckTicks != 0 && now - _lastRenderTimerCheckTicks < Stopwatch.Frequency)
                return;
            _lastRenderTimerCheckTicks = now;

            try
            {
                if (_desiredFpsProperty == null)
                    LookUpRenderTimer();

                int desiredFps = (int)_desiredFpsProperty.GetValue(_renderTimer);
                if (desiredFps < 1.5f * hz)
                {
                    int targetFps = Math.Min(MaxRenderTimerFps, 2 * (int)Math.Round(hz));
                    if (targetFps > desiredFps)
                    {
                        _desiredFpsProperty.SetValue(_renderTimer, targetFps);
                        GHApp.MaybeWriteGHLog("Avalonia render timer: DesiredFps " + desiredFps + " -> " + targetFps
                            + " (estimated refresh rate " + hz.ToString("F2", CultureInfo.InvariantCulture) + " Hz)");
                    }
                }
            }
            catch (Exception ex)
            {
                _renderTimerUnavailable = true;
                Exception inner = ex is TargetInvocationException && ex.InnerException != null ? ex.InnerException : ex;
                GHApp.MaybeWriteGHLog("Avalonia render timer adjustment unavailable: " + inner.GetType().Name + ": " + inner.Message);
            }
        }

        /* AvaloniaLocator.Current.GetService(typeof(IRenderLoop)).Timer.DesiredFps */
        private static void LookUpRenderTimer()
        {
            const BindingFlags staticFlags = BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Static;
            const BindingFlags instanceFlags = BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance;

            Type locatorType = Type.GetType("Avalonia.AvaloniaLocator, Avalonia.Base", true);
            PropertyInfo currentProperty = locatorType.GetProperty("Current", staticFlags);
            if (currentProperty == null)
                throw new MissingMemberException("Avalonia.AvaloniaLocator", "Current");
            object locator = currentProperty.GetValue(null);
            if (locator == null)
                throw new InvalidOperationException("AvaloniaLocator.Current is null");

            Type renderLoopType = Type.GetType("Avalonia.Rendering.IRenderLoop, Avalonia.Base", true);
            MethodInfo getService = null;
            Type resolverType = Type.GetType("Avalonia.IAvaloniaDependencyResolver, Avalonia.Base", false);
            if (resolverType != null && resolverType.IsInstanceOfType(locator))
                getService = resolverType.GetMethod("GetService", instanceFlags, null, new Type[] { typeof(Type) }, null);
            if (getService == null)
                getService = locator.GetType().GetMethod("GetService", instanceFlags, null, new Type[] { typeof(Type) }, null);
            if (getService == null)
                throw new MissingMethodException(locator.GetType().FullName, "GetService");
            object renderLoop = getService.Invoke(locator, new object[] { renderLoopType });
            if (renderLoop == null)
                throw new InvalidOperationException("No IRenderLoop is registered");

            PropertyInfo timerProperty = renderLoop.GetType().GetProperty("Timer", instanceFlags);
            if (timerProperty == null)
                throw new MissingMemberException(renderLoop.GetType().FullName, "Timer");
            object timer = timerProperty.GetValue(renderLoop);
            if (timer == null)
                throw new InvalidOperationException("The render loop has no timer");

            PropertyInfo desiredFpsProperty = timer.GetType().GetProperty("DesiredFps", instanceFlags);
            if (desiredFpsProperty == null || desiredFpsProperty.PropertyType != typeof(int)
                || !desiredFpsProperty.CanRead || !desiredFpsProperty.CanWrite)
                throw new MissingMemberException(timer.GetType().FullName, "DesiredFps");

            _renderTimer = timer;
            _desiredFpsProperty = desiredFpsProperty;
        }

        /* Render thread. ust, msc and sbc come from glXGetSyncValuesOML, sampled every
           PresentSampleFrameInterval frames of one GL control; deltas are per window swap (SBC).
           UST is taken to be in microseconds (Mesa). */
        public static void RecordPresentSample(long ust, long msc, long sbc)
        {
            lock (_statsLock)
            {
                if (_hasLastPresentSample)
                {
                    long mscDelta = msc - _lastMsc;
                    long ustDelta = ust - _lastUst;
                    long sbcDelta = sbc - _lastSbc;
                    if (mscDelta > 0 && ustDelta > 0 && sbcDelta > 0 && _presentSampleCount < ReportCapacity)
                    {
                        _ustPerFrameMs[_presentSampleCount++] = (double)ustDelta / sbcDelta / 1000.0;
                        _mscPerFrameSum += (double)mscDelta / sbcDelta;
                        if (mscDelta != sbcDelta)
                            _mscNotOneCount++;
                    }
                }
                _lastUst = ust;
                _lastMsc = msc;
                _lastSbc = sbc;
                _hasLastPresentSample = true;
            }
        }

        /* One log line covering the frames and present samples since the previous report;
           resets those statistics */
        public static string GetReport()
        {
            StringBuilder sb = new StringBuilder(320);
            sb.Append("Avalonia frame loop: SwapIntervalState ").Append(SwapIntervalState.ToString());
            sb.Append(", EstimatedRefreshRate ").Append(EstimatedRefreshRate.ToString("F2", CultureInfo.InvariantCulture)).Append(" Hz");
            lock (_statsLock)
            {
                AppendStatistics(sb, "frame delta ms", _frameDeltasMs, _frameDeltaCount);
                AppendStatistics(sb, "frame callback ms", _frameCallbackMs, _frameCallbackCount);
                AppendStatistics(sb, "GL draw interval ms", _glDrawIntervalsMs, _glDrawIntervalCount);
                AppendStatistics(sb, "GL paint ms", _glPaintMs, _glPaintCount);
                AppendStatistics(sb, "OML UST per frame ms", _ustPerFrameMs, _presentSampleCount);
                sb.Append("; OML MSC per frame: ");
                if (_presentSampleCount > 0)
                {
                    sb.Append("mean ").Append((_mscPerFrameSum / _presentSampleCount).ToString("F3", CultureInfo.InvariantCulture));
                    sb.Append(", not 1 in ").Append((100.0 * _mscNotOneCount / _presentSampleCount).ToString("F1", CultureInfo.InvariantCulture)).Append(" %");
                }
                else
                {
                    sb.Append("no samples");
                }

                _frameDeltaCount = 0;
                _frameCallbackCount = 0;
                _glDrawIntervalCount = 0;
                _glPaintCount = 0;
                _presentSampleCount = 0;
                _mscNotOneCount = 0;
                _mscPerFrameSum = 0.0;
            }
            return sb.ToString();
        }

        /* Caller holds _statsLock */
        private static void AppendStatistics(StringBuilder sb, string name, double[] values, int count)
        {
            sb.Append("; ").Append(name).Append(": count ").Append(count.ToString(CultureInfo.InvariantCulture));
            if (count <= 0)
                return;

            double sum = 0.0;
            int i;
            for (i = 0; i < count; i++)
                sum += values[i];
            Array.Copy(values, _reportScratch, count);
            Array.Sort(_reportScratch, 0, count);

            sb.Append(", mean ").Append((sum / count).ToString("F3", CultureInfo.InvariantCulture));
            sb.Append(", p50 ").Append(Percentile(_reportScratch, count, 0.50).ToString("F3", CultureInfo.InvariantCulture));
            sb.Append(", p95 ").Append(Percentile(_reportScratch, count, 0.95).ToString("F3", CultureInfo.InvariantCulture));
            sb.Append(", p99 ").Append(Percentile(_reportScratch, count, 0.99).ToString("F3", CultureInfo.InvariantCulture));
        }

        /* Nearest-rank percentile of the first count entries of an ascending array */
        private static double Percentile(double[] sorted, int count, double fraction)
        {
            int index = (int)Math.Ceiling(fraction * count) - 1;
            if (index < 0)
                index = 0;
            else if (index >= count)
                index = count - 1;
            return sorted[index];
        }
    }
}
#endif
