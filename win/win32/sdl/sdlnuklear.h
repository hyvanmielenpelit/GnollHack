/* GnollHack 4.0	sdlnuklear.h */
/* Copyright (c) Janne Gustafsson, 2021.				  */
/* GnollHack may be freely redistributed.  See license for details. */

#include "general.h"
#include "sdlproc.h"

#ifndef SDLNUKLEAR_H
#define SDLNUKLEAR_H

extern int StartNuklearExample(HINSTANCE hInstance);
extern int init_nuklear(HINSTANCE hInstance, PGHSdlApp sdlapp);
extern int shutdown_nuklear();
extern int nuklear_main_loop(PGHSdlApp sdlapp);
extern boolean nuklear_player_selection(PGHSdlApp sdlapp);
extern enum nuklear_main_menu_command nuklear_main_menu(PGHSdlApp sdlapp);
extern boolean nuklear_splash_screen(PGHSdlApp sdlapp);
extern struct nk_sdl* get_nuklear_sdl();
#if 0
extern boolean nuklear_splash_screen2(PGHSdlApp sdlapp);
#endif

#endif /* SDLNUKLEAR_H */

