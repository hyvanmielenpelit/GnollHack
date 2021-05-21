/*
 * callback.c
 * Copyright (c) Janne Gustafsson, 2021
 */
#include "callback.h"
#include "tradstdc.h"

extern int FDECL(GnollHackStart, (char*));

#ifdef WIN32
#define library_callbacks dll_callbacks
#else
#define library_callbacks lib_callbacks
#endif

extern struct callback_procs library_callbacks;
extern void FDECL(set_wincaps, (unsigned long, unsigned long));
