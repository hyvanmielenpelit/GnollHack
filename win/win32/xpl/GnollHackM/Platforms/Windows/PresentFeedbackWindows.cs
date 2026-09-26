#if WINDOWS && GNH_MAUI
using System;
using System.Runtime.InteropServices;
using GnollHackX.Performance;

namespace GnollHackM
{
    /* DWM composition timing for the frame timeline. DWM reports times as QPC values,
       which are the Stopwatch's own ticks on Windows, so they need no conversion. The trace
       markers go through GHRenderingEventSource, which ETW already sees, so the marker
       members here do nothing. */
    public sealed class PresentFeedbackWindows : IGHPresentFeedback
    {
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        private struct UNSIGNED_RATIO
        {
            public uint uiNumerator;
            public uint uiDenominator;
        }

        /* dwmapi.h declares this under pshpack1 */
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        private struct DWM_TIMING_INFO
        {
            public uint cbSize;
            public UNSIGNED_RATIO rateRefresh;
            public ulong qpcRefreshPeriod;
            public UNSIGNED_RATIO rateCompose;
            public ulong qpcVBlank;
            public ulong cRefresh;
            public uint cDXRefresh;
            public ulong qpcCompose;
            public ulong cFrame;
            public uint cDXPresent;
            public ulong cRefreshFrame;
            public ulong cFrameSubmitted;
            public uint cDXPresentSubmitted;
            public ulong cFrameConfirmed;
            public uint cDXPresentConfirmed;
            public ulong cRefreshConfirmed;
            public uint cDXRefreshConfirmed;
            public ulong cFramesLate;
            public uint cFramesOutstanding;
            public ulong cFrameDisplayed;
            public ulong qpcFrameDisplayed;
            public ulong cRefreshFrameDisplayed;
            public ulong cFrameComplete;
            public ulong qpcFrameComplete;
            public ulong cFramePending;
            public ulong qpcFramePending;
            public ulong cFramesDisplayed;
            public ulong cFramesComplete;
            public ulong cFramesPending;
            public ulong cFramesAvailable;
            public ulong cFramesDropped;
            public ulong cFramesMissed;
            public ulong cRefreshNextDisplayed;
            public ulong cRefreshNextPresented;
            public ulong cRefreshesDisplayed;
            public ulong cRefreshesPresented;
            public ulong cRefreshStarted;
            public ulong cPixelsReceived;
            public ulong cPixelsDrawn;
            public ulong cBuffersEmpty;
        }

        /* Since Windows 8.1 the window handle must be null: the timing is system-wide */
        [DllImport("dwmapi.dll")]
        private static extern int DwmGetCompositionTimingInfo(IntPtr hwnd, ref DWM_TIMING_INFO pTimingInfo);

        private static readonly uint TimingInfoSize = (uint)Marshal.SizeOf(typeof(DWM_TIMING_INFO));

        /* DWM fails transiently, e.g. during a display mode change, so the query is retried
           on every callback and given up only after this many failures in a row */
        private const int MaxConsecutiveFailures = 120;

        /* Render-loop (UI) thread only */
        private static int _consecutiveFailures = 0;
        private static bool _timingUnavailable = false;
        private static ulong _lastRefreshCount = 0;

        /* Called by the render loop before the tick with CompositionTarget.Rendering's
           RenderingTime in Stopwatch units. Hands the latest vblank to the timeline as the
           tick's vsync and records DWM's refresh and composition counters, once per refresh. */
        public static void CaptureFrame(long renderingTimeTicks)
        {
            if (_timingUnavailable)
            {
                GHFrameTimeline.SetPendingPlatformFrame(0, 0, renderingTimeTicks);
                return;
            }

            DWM_TIMING_INFO info = new DWM_TIMING_INFO();
            info.cbSize = TimingInfoSize;
            int hr;
            try
            {
                hr = DwmGetCompositionTimingInfo(IntPtr.Zero, ref info);
            }
            catch (Exception)
            {
                hr = -1;
            }
            if (hr != 0)
            {
                if (++_consecutiveFailures >= MaxConsecutiveFailures)
                {
                    _timingUnavailable = true;
                    GnollHackX.GHApp.MaybeWriteGHLog("DwmGetCompositionTimingInfo failed " + MaxConsecutiveFailures
                        + " times in a row (HRESULT 0x" + hr.ToString("X8") + "); frame timeline continues without DWM timing");
                }
                GHFrameTimeline.SetPendingPlatformFrame(0, 0, renderingTimeTicks);
                return;
            }
            _consecutiveFailures = 0;

            long vblank = (long)info.qpcVBlank;
            GHFrameTimeline.SetPendingPlatformFrame(vblank, 0, renderingTimeTicks);

            if (info.cRefresh == _lastRefreshCount)
                return;
            _lastRefreshCount = info.cRefresh;

            GHCompositorFrame frame = new GHCompositorFrame();
            frame.Source = GHCompositorSource.WindowsDwm;
            frame.IntendedVsyncTicks = vblank;
            frame.VsyncTicks = vblank;
            frame.RefreshPeriodTicks = (long)info.qpcRefreshPeriod;
            frame.ComposeTicks = (long)info.qpcCompose;
            frame.RefreshCount = (long)info.cRefresh;
            frame.ComposedFrameCount = (long)info.cFrame;
            GHFrameTimeline.AddCompositorFrame(ref frame);
        }

        public void SetActive(bool active)
        {
            if (active)
            {
                _timingUnavailable = false;
                _consecutiveFailures = 0;
                _lastRefreshCount = 0;
            }
        }

        public void TickBegin(long frameId)
        {
        }

        public void TickEnd(long frameId, GHPacingDecision pacing)
        {
        }

        public void PaintBegin(long frameId)
        {
        }

        public void FlushBegin(long frameId)
        {
        }

        public void PaintEnd(long frameId)
        {
        }

        public void Event(GHTraceEvent traceEvent, long value)
        {
        }
    }
}
#endif
