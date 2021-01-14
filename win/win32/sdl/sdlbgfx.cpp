/*
 * sdlbgfx.c
 * GnollHack BGFX Routines
 *
 * Copyright (c) Janne Gustafsson, 2021
 */

#include "win10.h"
#include <windows.h>
#include "resource.h"

/* nuklear - 1.32.0 - public domain */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <limits.h>
#include <time.h>

#include <SDL.h>
#include <SDL_syswm.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include "sdlproc.h"


extern "C"
{
    boolean
    init_bgfx(PGHSdlApp sdlapp)
    {

        // Collect information about the window from SDL
        SDL_SysWMinfo wmi;
        SDL_VERSION(&wmi.version);
        if (!SDL_GetWindowWMInfo(sdlapp->win, &wmi)) {
            return 1;
        }

        //bgfx::init();
#if 0
        bgfx::PlatformData pd;
        // and give the pointer to the window to pd
        pd.ndt = wmi.info.win.hdc;
        pd.nwh = (void*)(uintptr_t)wmi.info.win.window;

        // Tell bgfx about the platform and window
        bgfx::setPlatformData(pd);

        // Render an empty frame
        bgfx::renderFrame();

        // Initialize bgfx
        bgfx::init();
        
        // Reset window
        bgfx::reset(1200, 800, BGFX_RESET_VSYNC);

        // Enable debug text.
        bgfx::setDebug(BGFX_DEBUG_TEXT /*| BGFX_DEBUG_STATS*/);

        // Set view rectangle for 0th view
        bgfx::setViewRect(0, 0, 0, uint16_t(sdlapp->win_width), uint16_t(sdlapp->win_height));

        // Clear the view rect
        bgfx::setViewClear(0,
            BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
            0x443355FF, 1.0f, 0);

        // Set empty primitive on screen
        bgfx::touch(0);


        /*
        Put this inside the event loop of SDL, to render bgfx output
        */
        bgfx::frame();


        /*
        And put this just before SDL_Quit()
        */
        bgfx::shutdown();
#endif
        return TRUE;

    }

}

