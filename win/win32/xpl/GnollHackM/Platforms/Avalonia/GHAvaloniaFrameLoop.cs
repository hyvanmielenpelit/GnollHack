#if GNH_AVALONIA
using System;
using System.Collections.Generic;
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
       TopLevel.RequestAnimationFrame, estimates the refresh rate from the deltas that follow a
       frame a canvas control submitted, raises Avalonia's render timer rate when it lags the
       estimate, and gathers pacing statistics for the log. UI thread unless noted. */
    public static class GHAvaloniaFrameLoop
    {
        /* The GL control samples glXGetSyncValuesOML once every this many frames */
        public const int PresentSampleFrameInterval = 120;

        private const int ReportCapacity = 4096;
        private const int MaxRenderTimerFps = 240;

        private static readonly Action<TimeSpan> _onFrame = OnFrame;
        private static Avalonia.Controls.TopLevel _topLevel = null;
        private static volatile bool _running = false;
        private static bool _frameRequested = false;
        private static bool _hasLastFrameTime = false;
        private static TimeSpan _lastFrameTime;

        /* True while GHAvaloniaGamepad.OnFrame and GHApp.OnPlatformFrame run inside the frame callback; UI thread */
        private static bool _inFrameCallback = false;
        public static bool IsInFrameCallback { get { return _inFrameCallback; } }

        /* Set when a canvas control renders on the UI thread, which happens in the render pass that
           commits; read and cleared by the next frame callback */
        private static bool _frameSubmitted = false;

        /* Refresh-rate estimate from the recent frame deltas */
        private static readonly GHRefreshRateEstimator _estimator = new GHRefreshRateEstimator();
        private static float _estimatedRefreshRate = 0.0f;
        private static bool _gcConfigurationLogged = false;

        /* Statistics since the previous report; the present samples arrive on the render thread */
        private static readonly object _statsLock = new object();
        private static readonly double[] _frameDeltasMs = new double[ReportCapacity];
        private static int _frameDeltaCount = 0;
        /* Frame deltas left out of the estimate because the frame before them submitted nothing */
        private static int _unsubmittedFrameDeltaCount = 0;
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
        /* GHAvaloniaGamepad.OnFrame, part of the frame callback */
        private static readonly double[] _gamepadPollMs = new double[ReportCapacity];
        private static int _gamepadPollCount = 0;

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

        /* 0 until GHRefreshRateEstimator.MinimumCount frame deltas exist. Reflects the display while the
           swap interval is applied, or when it is within the snap tolerance of the monitor's refresh rate;
           otherwise it is the frame rate Avalonia's render timer delivers. */
        public static float EstimatedRefreshRate
        {
            get { return Volatile.Read(ref _estimatedRefreshRate); }
        }

        public static void Start(Avalonia.Controls.TopLevel topLevel)
        {
            if (topLevel == null)
                return;
            if (!_gcConfigurationLogged)
            {
                _gcConfigurationLogged = true;
                LogGcConfiguration();
            }
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
            bool previousSubmitted = _frameSubmitted;
            _frameSubmitted = false;
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
                        if (!previousSubmitted)
                            _unsubmittedFrameDeltaCount++;
                    }
                    /* After a frame that committed nothing, the interval was timed by Avalonia's
                       animation timer, not by the display */
                    if (previousSubmitted)
                        UpdateEstimate(deltaMs);
                }
            }
            _lastFrameTime = time;
            _hasLastFrameTime = true;

            MaybeAdjustRenderTimer();
            long callbackStart = Stopwatch.GetTimestamp();
            double gamepadMs = 0.0;
            _inFrameCallback = true;
            try
            {
                long gamepadStart = Stopwatch.GetTimestamp();
                try
                {
                    GHAvaloniaGamepad.OnFrame(gamepadStart);
                }
                catch (Exception ex)
                {
                    Debug.WriteLine(ex.Message);
                }
                gamepadMs = (Stopwatch.GetTimestamp() - gamepadStart) * 1000.0 / Stopwatch.Frequency;
                GHApp.OnPlatformFrame(EstimatedRefreshRate);
            }
            finally
            {
                _inFrameCallback = false;
            }
            double callbackMs = (Stopwatch.GetTimestamp() - callbackStart) * 1000.0 / Stopwatch.Frequency;
            lock (_statsLock)
            {
                if (_frameCallbackCount < ReportCapacity)
                    _frameCallbackMs[_frameCallbackCount++] = callbackMs;
                if (_gamepadPollCount < ReportCapacity)
                    _gamepadPollMs[_gamepadPollCount++] = gamepadMs;
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

        /* Called by the canvas controls' Render overrides; UI thread */
        public static void NoteFrameSubmitted()
        {
            _frameSubmitted = true;
        }

        /* UI thread; 0 when unknown */
        public static void SetMonitorRefreshRate(int hz)
        {
            _estimator.MonitorRefreshRate = hz > 0 ? hz : 0;
        }

        private static void UpdateEstimate(double deltaMs)
        {
            _estimator.VsyncPaced = SwapIntervalState == GHSwapIntervalState.Applied;
            if (!_estimator.AddDelta(deltaMs))
                return;

            float hz = _estimator.Published;
            Volatile.Write(ref _estimatedRefreshRate, hz);
            GHAvaloniaDeviceDisplay.Current?.OnRefreshRateEstimated(hz);
        }

        /* One log line with the effective gen0 budget and the GC mode */
        private static void LogGcConfiguration()
        {
            try
            {
                IReadOnlyDictionary<string, object> variables = GC.GetConfigurationVariables();
                List<string> keys = new List<string>();
                foreach (string key in variables.Keys)
                {
                    if (key.IndexOf("Gen0", StringComparison.OrdinalIgnoreCase) >= 0
                        || key.IndexOf("Concurrent", StringComparison.OrdinalIgnoreCase) >= 0
                        || key.IndexOf("Server", StringComparison.OrdinalIgnoreCase) >= 0)
                        keys.Add(key);
                }
                keys.Sort(StringComparer.Ordinal);

                StringBuilder sb = new StringBuilder("GC configuration: ");
                if (keys.Count == 0)
                    sb.Append("none reported");
                int i;
                for (i = 0; i < keys.Count; i++)
                {
                    if (i > 0)
                        sb.Append(", ");
                    sb.Append(keys[i]).Append('=').Append(Convert.ToString(variables[keys[i]], CultureInfo.InvariantCulture));
                }
                GHApp.MaybeWriteGHLog(sb.ToString());
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GC configuration unavailable: " + ex.Message);
            }
        }

        // WORKAROUND(WA-AVALONIA-0014): Avalonia's render timer keeps a stale RandR rate
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

        // WORKAROUND(WA-AVALONIA-0014): Avalonia's render timer keeps a stale RandR rate
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

        /* Two log lines covering the frames, GL draws and present samples since the previous report:
           the frame loop's, returned, and the GL controls', in glReport. "draw interval ms" includes the
           draws that re-present the last paint. The present samples are included only while the GLX swap
           interval is applied. Resets those statistics. */
        public static string GetReport(out string glReport)
        {
            GHSwapIntervalState swapIntervalState = SwapIntervalState;
            StringBuilder sb = new StringBuilder(200);
            StringBuilder glSb = new StringBuilder(240);
            sb.Append("Avalonia frame loop: EstimatedRefreshRate ").Append(EstimatedRefreshRate.ToString("F2", CultureInfo.InvariantCulture)).Append(" Hz");
            glSb.Append("Avalonia GL: SwapIntervalState ").Append(swapIntervalState.ToString());
            lock (_statsLock)
            {
                AppendStatistics(sb, "frame delta ms", _frameDeltasMs, _frameDeltaCount);
                sb.Append("; frame deltas without a submitted frame: ").Append(_unsubmittedFrameDeltaCount.ToString(CultureInfo.InvariantCulture));
                AppendStatistics(sb, "frame callback ms", _frameCallbackMs, _frameCallbackCount);
                AppendStatistics(sb, "gamepad poll ms", _gamepadPollMs, _gamepadPollCount);
                AppendStatistics(glSb, "draw interval ms", _glDrawIntervalsMs, _glDrawIntervalCount);
                AppendStatistics(glSb, "paint ms", _glPaintMs, _glPaintCount);
                if (swapIntervalState == GHSwapIntervalState.Applied)
                {
                    AppendStatistics(glSb, "OML UST per frame ms", _ustPerFrameMs, _presentSampleCount);
                    glSb.Append("; OML MSC per frame: ");
                    if (_presentSampleCount > 0)
                    {
                        glSb.Append("mean ").Append((_mscPerFrameSum / _presentSampleCount).ToString("F3", CultureInfo.InvariantCulture));
                        glSb.Append(", not 1 in ").Append((100.0 * _mscNotOneCount / _presentSampleCount).ToString("F1", CultureInfo.InvariantCulture)).Append(" %");
                    }
                    else
                    {
                        glSb.Append("no samples");
                    }
                }

                _frameDeltaCount = 0;
                _unsubmittedFrameDeltaCount = 0;
                _frameCallbackCount = 0;
                _gamepadPollCount = 0;
                _glDrawIntervalCount = 0;
                _glPaintCount = 0;
                _presentSampleCount = 0;
                _mscNotOneCount = 0;
                _mscPerFrameSum = 0.0;
            }
            glReport = glSb.ToString();
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
