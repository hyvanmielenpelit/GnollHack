/*
 * gnollhackios.h
 * Copyright (c) Janne Gustafsson, 2022
 */

#ifndef GNOLLHACKIOS_H
#define GNOLLHACKIOS_H

extern void gnollhackios(void);

#ifdef __APPLE__
#include <stdint.h>

/* os_signpost markers for the frame timeline (see gnollhackios.c) */
extern void gh_signpost_init(void);
extern void gh_signpost_tick_begin(uint64_t frame_id);
extern void gh_signpost_tick_end(uint64_t frame_id, int pacing);
extern void gh_signpost_paint_begin(uint64_t frame_id);
extern void gh_signpost_flush_begin(uint64_t frame_id);
extern void gh_signpost_flush_end(uint64_t frame_id);
extern void gh_signpost_paint_end(uint64_t frame_id);
extern void gh_signpost_event(int kind, int64_t value);
#endif

#endif