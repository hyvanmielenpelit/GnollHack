/* GnollHack 4.0	sdlnuklear.h */
/* Copyright (c) Janne Gustafsson, 2021.				  */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef SDLNUKLEAR_H
#define SDLNUKLEAR_H

extern int StartNuklearExample(HINSTANCE hInstance);
extern int init_nuklear(HINSTANCE hInstance, PGHSdlApp sdlapp);
extern int nuklear_main_loop(PGHSdlApp sdlapp);
extern boolean nuklear_player_selection(PGHSdlApp sdlapp);
extern int shutdown_nuklear(PGHSdlApp sdlapp);

#endif /* SDLNUKLEAR_H */

