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
#include <SDL_opengl.h>


extern "C"
{

    static void
    die(const char* fmt, ...)
    {
        va_list ap;
        va_start(ap, fmt);
        vfprintf(stderr, fmt, ap);
        va_end(ap);
        fputs("\n", stderr);
        exit(EXIT_FAILURE);
    }


    GLuint
    image_load_from_resource(HINSTANCE hInstance, int resource_id)
    {
        int x, y, n;
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

        HGLOBAL m_hBuffer = ::GlobalAlloc(GMEM_MOVEABLE, imageSize);
        if (m_hBuffer)
        {
            void* pBuffer = ::GlobalLock(m_hBuffer);
            if (pBuffer)
            {
                CopyMemory(pBuffer, pResourceData, imageSize);

                data = stbi_load_from_memory((stbi_uc const*)pBuffer, imageSize, &x, &y, &n, 0);

                ::GlobalUnlock(m_hBuffer);
            }
            ::GlobalFree(m_hBuffer);
            m_hBuffer = NULL;
        }

        if (!data) die("failed to load image form memory");

        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        //glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
        return tex;
    }

    GLuint
    image_load(const char* filename)
    {
        int x, y, n;
        GLuint tex;
        unsigned char* data = stbi_load(filename, &x, &y, &n, 0);
        if (!data) die("failed to load image: %s", filename);

        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        //glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
        return tex;
    }


}

/* sdlmisc.cpp */