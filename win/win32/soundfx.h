/* GnollHack 4.0    soundfx.h */
/* Copyright (c) Janne Gustafsson, 2020.                  */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef SOUNDFX_H
#define SOUNDFX_H

extern boolean initialize_fmod_studio(void);
extern boolean load_fmod_banks(void);
extern boolean load_fmod_bank_from_resource(HINSTANCE, int);
extern boolean load_fmod_bank_from_file(HINSTANCE, char*);
extern boolean close_fmod_studio(void);

extern boolean fmod_play_music(struct ghsound_music_info);
extern boolean fmod_play_level_ambient_sound(struct ghsound_level_ambient_info);
extern boolean fmod_play_environment_ambient_sound(struct ghsound_environment_ambient_info);
extern boolean fmod_play_occupation_ambient_sound(struct ghsound_occupation_ambient_info);
extern boolean fmod_play_effect_ambient_sound(struct ghsound_effect_ambient_info);
extern boolean fmod_set_effect_ambient_volume(struct effect_ambient_volume_info);
extern boolean fmod_play_immediate_sound(struct ghsound_immediate_info);
extern boolean fmod_stop_all_sounds(struct stop_all_info);
extern int fmod_adjust_ghsound_general_volumes(float, float, float, float, float, float);
extern int fmod_add_ambient_ghsound(enum ghsound_types, float, void**);
extern int fmod_delete_ambient_ghsound(void*);
extern int fmod_set_ambient_ghsound_volume(void*, float);
#if 0
extern boolean fmod_stop_all_immediate_sounds(void);
#endif

#endif /* SOUNDFX_H */
