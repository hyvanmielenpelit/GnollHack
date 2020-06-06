/* GnollHack 4.0	soundfx.h */
/* Copyright (c) Janne Gustafsson, 2020.				  */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef SOUNDFX_H
#define SOUNDFX_H

extern boolean NDECL(initialize_fmod);
extern void NDECL(fmod_play_sound_example);
extern void NDECL(fmod_test_event);
extern void NDECL(fmod_event_example);
extern void NDECL(fmod_event_example_from_file);
extern void NDECL(fmod_orig_example);
extern void NDECL(fmod_stop_all_sounds);

extern boolean NDECL(initialize_fmod_studio);
extern boolean NDECL(load_fmod_banks);
extern boolean NDECL(close_fmod_studio);

extern boolean FDECL(fmod_play_music, (struct ghsound_music_info));

#endif /* SOUNDFX_H */
