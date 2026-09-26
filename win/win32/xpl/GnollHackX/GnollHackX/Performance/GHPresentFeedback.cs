using System;
using System.Diagnostics.Tracing;
using System.Threading;

namespace GnollHackX.Performance
{
    /* Point events the trace backends mark. iOS signposts need a compile-time literal name
       for each, so the set is fixed. */
    public enum GHTraceEvent : int
    {
        CadenceChange = 1,      /* value: displayed frames per second after the change */
        RefreshChange = 2       /* value: measured refresh period in microseconds */
    }

    /* A platform's presentation feedback and trace markers. SetActive runs on the UI thread;
       the tick markers run on the UI thread and the paint markers on the paint thread. No
       member may throw or block. */
    public interface IGHPresentFeedback
    {
        void SetActive(bool active);
        void TickBegin(long frameId);
        void TickEnd(long frameId, GHPacingDecision pacing);
        void PaintBegin(long frameId);
        void FlushBegin(long frameId);
        void PaintEnd(long frameId);
        void Event(GHTraceEvent traceEvent, long value);
    }

    /* Routes the frame pipeline's markers to the platform backend and to an EventSource that
       ETW, EventPipe, and in-process listeners can consume. The platform backend is active
       exactly while the frame timeline is enabled. */
    public static class GHPresentFeedback
    {
        private static IGHPresentFeedback _platform = null;
        private static int _active = 0;

        public static void Register(IGHPresentFeedback platform)
        {
            _platform = platform;
        }

        public static bool IsActive { get { return Interlocked.CompareExchange(ref _active, 0, 0) != 0; } }

        /* Called on the UI thread whenever the feedback starts or stops, for app-side probes */
        public static Action<bool> ActiveChanged = null;

        /* Called once per display callback on the UI thread */
        public static void Sync()
        {
            bool wanted = GHFrameTimeline.IsEnabled;
            if (wanted == IsActive)
                return;
            Interlocked.Exchange(ref _active, wanted ? 1 : 0);
            IGHPresentFeedback platform = _platform;
            try
            {
                if (platform != null)
                    platform.SetActive(wanted);
            }
            catch (Exception)
            {
                /* A platform that cannot start its listeners still has the in-app timeline */
            }
            Action<bool> changed = ActiveChanged;
            try
            {
                if (changed != null)
                    changed(wanted);
            }
            catch (Exception)
            {
                /* A probe that fails to start must not stop the render loop */
            }
        }

        public static void TickBegin(long frameId)
        {
            if (frameId <= 0)
                return;
            GHRenderingEventSource.Log.TickBegin(frameId);
            IGHPresentFeedback platform = _platform;
            if (platform != null && IsActive)
                platform.TickBegin(frameId);
        }

        public static void TickEnd(long frameId, GHPacingDecision pacing)
        {
            if (frameId <= 0)
                return;
            GHRenderingEventSource.Log.TickEnd(frameId, (long)pacing);
            IGHPresentFeedback platform = _platform;
            if (platform != null && IsActive)
                platform.TickEnd(frameId, pacing);
        }

        public static void PaintBegin(long frameId)
        {
            if (frameId <= 0)
                return;
            GHRenderingEventSource.Log.PaintBegin(frameId);
            IGHPresentFeedback platform = _platform;
            if (platform != null && IsActive)
                platform.PaintBegin(frameId);
        }

        public static void FlushBegin(long frameId)
        {
            if (frameId <= 0)
                return;
            GHRenderingEventSource.Log.FlushBegin(frameId);
            IGHPresentFeedback platform = _platform;
            if (platform != null && IsActive)
                platform.FlushBegin(frameId);
        }

        public static void PaintEnd(long frameId)
        {
            if (frameId <= 0)
                return;
            GHRenderingEventSource.Log.PaintEnd(frameId);
            IGHPresentFeedback platform = _platform;
            if (platform != null && IsActive)
                platform.PaintEnd(frameId);
        }

        public static void Event(GHTraceEvent traceEvent, long value)
        {
            GHRenderingEventSource.Log.Marker((long)traceEvent, value);
            IGHPresentFeedback platform = _platform;
            if (platform != null && IsActive)
                platform.Event(traceEvent, value);
        }
    }

    /* Frame pipeline events for ETW (WPA, PresentMon's session) and EventPipe. Every event
       carries the FrameId, so the stages line up with the in-app timeline. Writes cost one
       enabled check when no listener is attached. */
    [EventSource(Name = "GnollHack-Rendering")]
    public sealed class GHRenderingEventSource : EventSource
    {
        public static readonly GHRenderingEventSource Log = new GHRenderingEventSource();

        private GHRenderingEventSource()
        {
        }

        [Event(1, Level = EventLevel.Informational)]
        public void TickBegin(long frameId)
        {
            if (IsEnabled())
                WriteEvent(1, frameId);
        }

        [Event(2, Level = EventLevel.Informational)]
        public void TickEnd(long frameId, long pacing)
        {
            if (IsEnabled())
                WriteEvent(2, frameId, pacing);
        }

        [Event(3, Level = EventLevel.Informational)]
        public void PaintBegin(long frameId)
        {
            if (IsEnabled())
                WriteEvent(3, frameId);
        }

        [Event(4, Level = EventLevel.Informational)]
        public void FlushBegin(long frameId)
        {
            if (IsEnabled())
                WriteEvent(4, frameId);
        }

        [Event(5, Level = EventLevel.Informational)]
        public void PaintEnd(long frameId)
        {
            if (IsEnabled())
                WriteEvent(5, frameId);
        }

        [Event(6, Level = EventLevel.Informational)]
        public void Marker(long kind, long value)
        {
            if (IsEnabled())
                WriteEvent(6, kind, value);
        }
    }
}
