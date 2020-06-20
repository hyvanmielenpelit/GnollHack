/* GnollHack 4.0	soundfx.h */
/* Copyright (c) Janne Gustafsson, 2020.				  */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef SOUNDFX_H
#define SOUNDFX_H

extern boolean NDECL(initialize_fmod_studio);
extern boolean NDECL(load_fmod_banks);
extern boolean NDECL(close_fmod_studio);

extern boolean FDECL(fmod_play_music, (struct ghsound_music_info));
extern boolean FDECL(fmod_play_movement_sound, (struct ghsound_movement_info));
extern boolean FDECL(fmod_adjust_ghsound_general_volumes, (float, float, float));

#endif /* SOUNDFX_H */
