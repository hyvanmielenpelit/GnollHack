/* GnollHack 4.0	sdlmisc.h */
/* Copyright (c) Janne Gustafsson, 2021.				  */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef SDLMISC_H
#define SDLMISC_H

extern GLuint image_load_from_resource(HINSTANCE hInstance, int resource_id);
extern GLuint image_load(const char* filename);
extern SDL_Texture* sdl_texture_from_resource(SDL_Renderer* renderer, HINSTANCE hInstance, int resource_id);
extern SDL_Surface* sdl_surface_from_resource(HINSTANCE hInstance, int resource_id);

#endif /* SDLMISC_H */
