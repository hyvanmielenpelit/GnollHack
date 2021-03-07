/*
 * sdlmisc.cpp
 * GnollHack Miscellaneous C++ Routines
 *
 * Copyright (c) Janne Gustafsson, 2021
 */

#pragma comment( lib, "OpenGL32" )          

#include "win10.h"
#include <windows.h>
#include <string>
#include <codecvt>
#include <locale>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "resource.h"
#include "hack.h"
#include "patchlevel.h"

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

#if defined(SDL_GRAPHICS)
#include <SDL.h>
#if defined(GL2_GRAPHICS)
#include <SDL_opengl.h>
#elif defined(GLES2_GRAPHICS)
#include <SDL_opengles2.h>
#endif
#elif defined(GLFW_GRAPHICS)
#include <glfw3.h>
#endif

extern "C"
{
#ifdef GL2_GRAPHICS
    GLuint
    gl2_image_load_from_resource(HINSTANCE hInstance, int resource_id, int *x_ptr, int *y_ptr, size_t *n_ptr)
    {
        if (!x_ptr || !y_ptr || !n_ptr)
            return 0;

        *x_ptr = 0, *y_ptr = 0;

        GLuint tex;
        unsigned char* data = 0;

        HRSRC hResource = ::FindResource(hInstance, MAKEINTRESOURCE(resource_id), "PNG");
        if (!hResource)
            return 0;

        DWORD imageSize = ::SizeofResource(hInstance, hResource);
        if (!imageSize)
            return 0;

        const void* pResourceData = ::LockResource(::LoadResource(hInstance, hResource));
        if (!pResourceData)
            return 0;

        int comps;
        data = stbi_load_from_memory((stbi_uc const*)pResourceData, imageSize, x_ptr, y_ptr, &comps, STBI_rgb_alpha);

        if (!data) 
            return 0;

        size_t memsize = (size_t)(*x_ptr * *y_ptr * STBI_rgb_alpha);
        *n_ptr = memsize;

        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, *x_ptr, *y_ptr, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        //glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
        return tex;
    }

    GLuint
    gl2_image_load(const char* filename, int* x_ptr, int* y_ptr, size_t* n_ptr)
    {
        if (!x_ptr || !y_ptr || !n_ptr)
            return 0;

        *x_ptr = 0, *y_ptr = 0;

        GLuint tex;
        int comps;
        unsigned char* data = stbi_load(filename, x_ptr, y_ptr, &comps, STBI_rgb_alpha);
        *n_ptr = (size_t )(*x_ptr * *y_ptr * STBI_rgb_alpha);
        if (!data) 
            return 0;

        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, *x_ptr, *y_ptr, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        //glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
        return tex;
    }
#endif

#ifdef SDL_GRAPHICS
    SDL_Surface*
    sdl_surface_image_load_from_resource(HINSTANCE hInstance, int resource_id, int* x_ptr, int* y_ptr, size_t* n_ptr)
    {
        if (!x_ptr || !y_ptr || !n_ptr)
            return 0;

        *x_ptr = 0, * y_ptr = 0;
        unsigned char* data = 0;

        HRSRC hResource = ::FindResource(hInstance, MAKEINTRESOURCE(resource_id), "PNG");
        if (!hResource)
            return 0;

        DWORD imageSize = ::SizeofResource(hInstance, hResource);
        if (!imageSize)
            return 0;

        const void* pResourceData = ::LockResource(::LoadResource(hInstance, hResource));
        if (!pResourceData)
            return 0;

        int comps;
        data = stbi_load_from_memory((stbi_uc const*)pResourceData, imageSize, x_ptr, y_ptr, &comps, STBI_rgb_alpha);

        if (!data)
            return 0;

        size_t memsize = (size_t)(*x_ptr * *y_ptr * 4);
        *n_ptr = memsize;
        unsigned char* datacopy = (unsigned char*)malloc(memsize);
        CopyMemory(datacopy, data, memsize);

        SDL_Surface* retptr = SDL_CreateRGBSurfaceWithFormatFrom(datacopy, *x_ptr, *y_ptr, 32, (*x_ptr) * 4, SDL_PIXELFORMAT_RGBA8888);
        stbi_image_free(data);
        return retptr;
    }
#endif

}




/* sdlmisc.cpp */