#if GNH_MAUI
using System.Runtime.InteropServices;
using GnollHackX.Performance;

namespace GnollHackM
{
    /* os_signpost trace markers for Instruments, emitted through the shim in
       libgnollhackios (gnollhackios.c). Each interval uses the FrameId as its signpost id,
       so the Tick, Paint and Flush intervals of one frame line up with the timeline's
       records and with the Animation Hitches track. The display link's timestamps are
       captured by the render loop itself; iOS has no per-frame present feedback on the
       GLKView and CPU canvas paths.

       .NET for iOS links __Internal P/Invokes as direct native calls, so the shim calls
       are compiled only when GNH_IOS_SIGNPOSTS is defined, which requires a
       libgnollhackios.a built with the shim. */
    public sealed class PresentFeedbackiOS : IGHPresentFeedback
    {
#if GNH_IOS_SIGNPOSTS
        [DllImport("__Internal")]
        private static extern void gh_signpost_init();
        [DllImport("__Internal")]
        private static extern void gh_signpost_tick_begin(ulong frame_id);
        [DllImport("__Internal")]
        private static extern void gh_signpost_tick_end(ulong frame_id, int pacing);
        [DllImport("__Internal")]
        private static extern void gh_signpost_paint_begin(ulong frame_id);
        [DllImport("__Internal")]
        private static extern void gh_signpost_flush_begin(ulong frame_id);
        [DllImport("__Internal")]
        private static extern void gh_signpost_flush_end(ulong frame_id);
        [DllImport("__Internal")]
        private static extern void gh_signpost_paint_end(ulong frame_id);
        [DllImport("__Internal")]
        private static extern void gh_signpost_event(int kind, long value);

        private bool _initialized = false;
        private bool _flushOpen = false;
#endif

        public void SetActive(bool active)
        {
#if GNH_IOS_SIGNPOSTS
            if (active && !_initialized)
            {
                gh_signpost_init();
                _initialized = true;
            }
#endif
        }

        public void TickBegin(long frameId)
        {
#if GNH_IOS_SIGNPOSTS
            if (_initialized)
                gh_signpost_tick_begin((ulong)frameId);
#endif
        }

        public void TickEnd(long frameId, GHPacingDecision pacing)
        {
#if GNH_IOS_SIGNPOSTS
            if (_initialized)
                gh_signpost_tick_end((ulong)frameId, (int)pacing);
#endif
        }

        public void PaintBegin(long frameId)
        {
#if GNH_IOS_SIGNPOSTS
            if (_initialized)
                gh_signpost_paint_begin((ulong)frameId);
#endif
        }

        public void FlushBegin(long frameId)
        {
#if GNH_IOS_SIGNPOSTS
            if (_initialized)
            {
                gh_signpost_flush_begin((ulong)frameId);
                _flushOpen = true;
            }
#endif
        }

        /* Closes the Flush interval as well as the Paint interval */
        public void PaintEnd(long frameId)
        {
#if GNH_IOS_SIGNPOSTS
            if (_initialized)
            {
                if (_flushOpen)
                {
                    _flushOpen = false;
                    gh_signpost_flush_end((ulong)frameId);
                }
                gh_signpost_paint_end((ulong)frameId);
            }
#endif
        }

        public void Event(GHTraceEvent traceEvent, long value)
        {
#if GNH_IOS_SIGNPOSTS
            if (_initialized)
                gh_signpost_event((int)traceEvent, value);
#endif
        }
    }
}
#endif
