#if GNH_MAUI
using System;
using Android.OS;
using Android.Views;
using GnollHackX.Performance;
using Microsoft.Maui.ApplicationModel;
using AndroidWindow = Android.Views.Window;

namespace GnollHackM
{
    /* HWUI frame metrics and Perfetto trace markers for the frame timeline.

       FrameMetrics describes the window's RenderThread frames, which composite the map
       whether it is an SKCanvasView bitmap or an SKGLTextureView texture. Its timestamps
       are System.nanoTime, the same clock as Choreographer's frameTimeNanos, and are
       converted with the timeline's clock anchor. The listener runs on its own
       HandlerThread and only while the timeline is enabled.

       Trace sections appear in Perfetto and systrace next to SurfaceFlinger's frame
       timeline; the FrameId counter tracks tie them to the timeline's records. */
    public sealed class PresentFeedbackAndroid : Java.Lang.Object, AndroidWindow.IOnFrameMetricsAvailableListener, IGHPresentFeedback
    {

        private HandlerThread _thread = null;
        private Handler _handler = null;
        private AndroidWindow _window = null;

        /* Each section is opened and closed on one thread, so each thread has its own flag */
        private bool _tickSectionOpen = false;
        private bool _paintSectionOpen = false;
        private bool _flushSectionOpen = false;

        public void SetActive(bool active)
        {
            if (active)
                Start();
            else
                Stop();
        }

        private void Start()
        {
            if (_window != null)
                return;
            AndroidWindow window = Platform.CurrentActivity?.Window;
            if (window == null)
                return;
            _thread = new HandlerThread("GHFrameMetrics");
            _thread.Start();
            _handler = new Handler(_thread.Looper);
            window.AddOnFrameMetricsAvailableListener(this, _handler);
            _window = window;
        }

        private void Stop()
        {
            AndroidWindow window = _window;
            _window = null;
            if (window != null)
            {
                try
                {
                    window.RemoveOnFrameMetricsAvailableListener(this);
                }
                catch (Exception)
                {
                    /* The window may already be gone */
                }
            }
            _thread?.QuitSafely();
            _thread = null;
            _handler = null;
        }

        public void OnFrameMetricsAvailable(AndroidWindow window, FrameMetrics frameMetrics, int dropCountSinceLastInvocation)
        {
            if (frameMetrics == null)
                return;
            try
            {
                long intended = frameMetrics.GetMetric((int)FrameMetricsId.IntendedVsyncTimestamp);
                long vsync = frameMetrics.GetMetric((int)FrameMetricsId.VsyncTimestamp);

                /* The stage durations run back to back from the intended vsync; the
                   RenderThread latches the newest TextureView frame during sync */
                long beforeSync = frameMetrics.GetMetric((int)FrameMetricsId.UnknownDelayDuration)
                    + frameMetrics.GetMetric((int)FrameMetricsId.InputHandlingDuration)
                    + frameMetrics.GetMetric((int)FrameMetricsId.AnimationDuration)
                    + frameMetrics.GetMetric((int)FrameMetricsId.LayoutMeasureDuration)
                    + frameMetrics.GetMetric((int)FrameMetricsId.DrawDuration);
                long total = frameMetrics.GetMetric((int)FrameMetricsId.TotalDuration);

                GHCompositorFrame frame = new GHCompositorFrame();
                frame.Source = GHCompositorSource.AndroidFrameMetrics;
                frame.IntendedVsyncTicks = GHFrameTimeline.PlatformNanosToTicks(intended);
                frame.VsyncTicks = GHFrameTimeline.PlatformNanosToTicks(vsync);
                frame.SyncStartTicks = GHFrameTimeline.PlatformNanosToTicks(intended + beforeSync);
                frame.CompletedTicks = GHFrameTimeline.PlatformNanosToTicks(intended + total);
                frame.DroppedSinceLast = dropCountSinceLastInvocation;
                frame.FirstDrawFrame = frameMetrics.GetMetric((int)FrameMetricsId.FirstDrawFrame) != 0;
                if (OperatingSystem.IsAndroidVersionAtLeast(31))
                {
                    frame.GpuDurationTicks = NanosToTicks(frameMetrics.GetMetric((int)FrameMetricsId.GpuDuration));
                    frame.DeadlineRaw = frameMetrics.GetMetric((int)FrameMetricsId.Deadline);
                }
                GHFrameTimeline.AddCompositorFrame(ref frame);
            }
            catch (Exception)
            {
                /* A diagnostic must never throw out of a platform callback */
            }
            finally
            {
                frameMetrics.Dispose();
            }
        }

        private static long NanosToTicks(long nanos)
        {
            return (long)Math.Round(nanos * (double)System.Diagnostics.Stopwatch.Frequency / 1000000000.0);
        }

        private static bool TraceOn
        {
            get { return OperatingSystem.IsAndroidVersionAtLeast(29) && Android.OS.Trace.IsEnabled; }
        }

        /* Every Trace call below runs only after TraceOn, which requires API 29, has held */
#pragma warning disable CA1416
        public void TickBegin(long frameId)
        {
            if (!TraceOn)
                return;
            Android.OS.Trace.BeginSection("GH.Tick");
            Android.OS.Trace.SetCounter("GH.FrameId", frameId);
            _tickSectionOpen = true;
        }

        public void TickEnd(long frameId, GHPacingDecision pacing)
        {
            if (!_tickSectionOpen)
                return;
            _tickSectionOpen = false;
            Android.OS.Trace.SetCounter("GH.Pacing", (long)pacing);
            Android.OS.Trace.EndSection();
        }

        public void PaintBegin(long frameId)
        {
            if (!TraceOn)
                return;
            Android.OS.Trace.BeginSection("GH.Paint");
            Android.OS.Trace.SetCounter("GH.PaintFrameId", frameId);
            _paintSectionOpen = true;
        }

        public void FlushBegin(long frameId)
        {
            if (!_paintSectionOpen)
                return;
            Android.OS.Trace.BeginSection("GH.Flush");
            _flushSectionOpen = true;
        }

        public void PaintEnd(long frameId)
        {
            if (_flushSectionOpen)
            {
                _flushSectionOpen = false;
                Android.OS.Trace.EndSection();
            }
            if (_paintSectionOpen)
            {
                _paintSectionOpen = false;
                Android.OS.Trace.EndSection();
            }
        }

        public void Event(GHTraceEvent traceEvent, long value)
        {
            if (!TraceOn)
                return;
            switch (traceEvent)
            {
            case GHTraceEvent.CadenceChange:
                Android.OS.Trace.SetCounter("GH.DisplayedFps", value);
                break;
            case GHTraceEvent.RefreshChange:
                Android.OS.Trace.SetCounter("GH.RefreshPeriodUs", value);
                break;
            }
        }
#pragma warning restore CA1416
    }
}
#endif
