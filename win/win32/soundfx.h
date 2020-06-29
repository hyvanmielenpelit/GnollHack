/* GnollHack 4.0	soundfx.h */
/* Copyright (c) Janne Gustafsson, 2020.				  */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef SOUNDFX_H
#define SOUNDFX_H

extern boolean NDECL(initialize_fmod_studio);
extern boolean NDECL(load_fmod_banks);
extern boolean NDECL(close_fmod_studio);

extern boolean FDECL(fmod_play_music, (struct ghsound_music_info));
extern boolean FDECL(fmod_play_level_ambient_sound, (struct ghsound_level_ambient_info));
extern boolean FDECL(fmod_play_immediate_sound, (struct ghsound_immediate_info));
extern int FDECL(fmod_adjust_ghsound_general_volumes, (float, float, float, float));
extern int FDECL(fmod_add_ambient_ghsound, (enum ghsound_types, float, void**));
extern int FDECL(fmod_delete_ambient_ghsound, (void*));
extern int FDECL(fmod_set_ambient_ghsound_volume, (void*, float));

#endif /* SOUNDFX_H */
