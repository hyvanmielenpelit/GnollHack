/* GnollHack 4.1.0 gnollhackios.c */
/* Copyright (c) Janne Gustafsson, 2022. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "gnollhackios.h"

#ifdef __APPLE__
#include <os/log.h>
#include <os/signpost.h>
#endif

void gnollhackios(void)
{

}

#ifdef __APPLE__
/*
 * os_signpost markers for the frame pipeline, called from the frame
 * timeline's iOS backend. Signpost names and format strings must be
 * literals compiled into this image, which is why each kind has its own
 * function. The frame id is the signpost id, so the Tick, Paint and Flush
 * intervals of one frame line up in Instruments.
 */
static os_log_t gh_render_log = NULL;

void gh_signpost_init(void)
{
    if (gh_render_log == NULL)
        gh_render_log = os_log_create("fi.hyvanmielenpelit.gnollhack",
                                      OS_LOG_CATEGORY_POINTS_OF_INTEREST);
}

static int gh_signposts_on(void) GH_SIGNPOST_AVAILABLE;

static int gh_signposts_on(void)
{
    return gh_render_log != NULL && os_signpost_enabled(gh_render_log);
}

void gh_signpost_tick_begin(uint64_t frame_id)
{
    if (gh_signposts_on())
        os_signpost_interval_begin(gh_render_log, (os_signpost_id_t)frame_id,
                                   "Tick", "frame %llu",
                                   (unsigned long long)frame_id);
}

void gh_signpost_tick_end(uint64_t frame_id, int pacing)
{
    if (gh_signposts_on())
        os_signpost_interval_end(gh_render_log, (os_signpost_id_t)frame_id,
                                 "Tick", "pacing %d", pacing);
}

void gh_signpost_paint_begin(uint64_t frame_id)
{
    if (gh_signposts_on())
        os_signpost_interval_begin(gh_render_log, (os_signpost_id_t)frame_id,
                                   "Paint", "frame %llu",
                                   (unsigned long long)frame_id);
}

void gh_signpost_flush_begin(uint64_t frame_id)
{
    if (gh_signposts_on())
        os_signpost_interval_begin(gh_render_log, (os_signpost_id_t)frame_id,
                                   "Flush", "frame %llu",
                                   (unsigned long long)frame_id);
}

void gh_signpost_flush_end(uint64_t frame_id)
{
    if (gh_signposts_on())
        os_signpost_interval_end(gh_render_log, (os_signpost_id_t)frame_id,
                                 "Flush");
}

void gh_signpost_paint_end(uint64_t frame_id)
{
    if (gh_signposts_on())
        os_signpost_interval_end(gh_render_log, (os_signpost_id_t)frame_id,
                                 "Paint");
}

/* kind: 1 = cadence change (value: displayed FPS),
         2 = refresh change (value: refresh period in microseconds),
         3 = callback cadence change (value: callback period in microseconds) */
void gh_signpost_event(int kind, int64_t value)
{
    if (!gh_signposts_on())
        return;

    switch (kind)
    {
    case 1:
        os_signpost_event_emit(gh_render_log, OS_SIGNPOST_ID_EXCLUSIVE,
                               "CadenceChange", "fps %lld", (long long)value);
        break;
    case 2:
        os_signpost_event_emit(gh_render_log, OS_SIGNPOST_ID_EXCLUSIVE,
                               "RefreshChange", "period_us %lld",
                               (long long)value);
        break;
    case 3:
        os_signpost_event_emit(gh_render_log, OS_SIGNPOST_ID_EXCLUSIVE,
                               "CallbackCadenceChange", "period_us %lld",
                               (long long)value);
        break;
    default:
        break;
    }
}
#endif
