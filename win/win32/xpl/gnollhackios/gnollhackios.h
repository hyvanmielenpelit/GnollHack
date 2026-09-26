/*
 * gnollhackios.h
 * Copyright (c) Janne Gustafsson, 2022
 */

#ifndef GNOLLHACKIOS_H
#define GNOLLHACKIOS_H

extern void gnollhackios(void);

#ifdef __APPLE__
#include <stdint.h>

/*
 * os_signpost markers for the frame timeline (see gnollhackios.c).
 * os_signpost needs iOS 12, which the app's minimum exceeds; stating it
 * on the functions lets them call os_signpost at any deployment target.
 */
#define GH_SIGNPOST_AVAILABLE \
    __attribute__((availability(ios, introduced = 12.0)))

extern void gh_signpost_init(void) GH_SIGNPOST_AVAILABLE;
extern void gh_signpost_tick_begin(uint64_t frame_id) GH_SIGNPOST_AVAILABLE;
extern void gh_signpost_tick_end(uint64_t frame_id, int pacing)
    GH_SIGNPOST_AVAILABLE;
extern void gh_signpost_paint_begin(uint64_t frame_id) GH_SIGNPOST_AVAILABLE;
extern void gh_signpost_flush_begin(uint64_t frame_id) GH_SIGNPOST_AVAILABLE;
extern void gh_signpost_flush_end(uint64_t frame_id) GH_SIGNPOST_AVAILABLE;
extern void gh_signpost_paint_end(uint64_t frame_id) GH_SIGNPOST_AVAILABLE;
extern void gh_signpost_event(int kind, int64_t value) GH_SIGNPOST_AVAILABLE;
#endif

#endif
