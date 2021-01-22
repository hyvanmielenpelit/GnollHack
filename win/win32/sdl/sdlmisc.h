/* GnollHack 4.0	sdlmisc.h */
/* Copyright (c) Janne Gustafsson, 2021.				  */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef SDLMISC_H
#define SDLMISC_H

#ifdef SDL_GRAPHICS
SDL_Surface* sdl_surface_image_load_from_resource(HINSTANCE hInstance, int resource_id, int* x_ptr, int* y_ptr, size_t* n_ptr);
#endif
extern GLuint gl2_image_load_from_resource(HINSTANCE hInstance, int resource_id, int* x_ptr, int* y_ptr, size_t* n_ptr);
extern GLuint gl2_image_load(const char* filename, int* x_ptr, int* y_ptr, size_t* n_ptr);

#endif /* SDLMISC_H */
