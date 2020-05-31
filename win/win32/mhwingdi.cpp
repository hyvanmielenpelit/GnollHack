/* wingdi.cpp
 * Copyright 2020 by Janne Gustafsson
 */

#include "fmod.hpp"

//#include <stdafx.h>
#include <windows.h>
//#include <objidl.h>
//#include <gdiplus.h>

//using namespace Gdiplus;
//#pragma comment (lib,"Gdiplus.lib")

extern "C" {

    HBITMAP
    loadPNG(HINSTANCE hInstance, LPCSTR lpBitmapName)
    {
        LPCSTR vIn = lpBitmapName;
        wchar_t* vOut = new wchar_t[strlen(vIn) + 1];
        mbstowcs_s(NULL, vOut, strlen(vIn) + 1, vIn, strlen(vIn));

        HBITMAP handle = (HBITMAP)0;
#if 0
        Bitmap* pngBitmap = Bitmap::FromResource(hInstance, (const wchar_t*)lpBitmapName); // (const WCHAR*)MAKEINTRESOURCE(IDB_GNOLLHACK_TILES));
        if (!pngBitmap)
        {
            return handle;
        }
        pngBitmap->GetHBITMAP(0, &handle);
#endif
        return handle;
    }

    /*
    Bitmap*
    BitmapFromResource()
    {
        return 0;
    }
    */
    HBITMAP
    loadPNG_FromFile(char* filename)
    {
        char* vIn = filename;
        wchar_t* vOut = new wchar_t[strlen(vIn) + 1];
        mbstowcs_s(NULL, vOut, strlen(vIn) + 1, vIn, strlen(vIn));

        HBITMAP handle = (HBITMAP)0;
#if 0
        Bitmap* pngBitmap = Bitmap::FromFile(vOut, FALSE); // (const WCHAR*)MAKEINTRESOURCE(IDB_GNOLLHACK_TILES));
        if (!pngBitmap)
        {
            return handle;
        }
        pngBitmap->GetHBITMAP(0, &handle);
#endif
        return handle;
    }


    void
    fmod_play_sound_example()
    {
            FMOD::System* system;
            FMOD::Sound* sound1;
            FMOD::Channel* channel = 0;
            FMOD_RESULT       result;
            unsigned int      version;
            void* extradriverdata = 0;

            /*
                Create a System object and initialize
            */
            result = FMOD::System_Create(&system);
            result = system->getVersion(&version);
            result = system->init(32, FMOD_INIT_NORMAL, extradriverdata);
            result = system->createSound("C:\\Users\\janne\\Test\\Sound Test\\PowerUp20.wav", FMOD_DEFAULT, 0, &sound1);
            result = sound1->setMode(FMOD_LOOP_OFF);    /* drumloop.wav has embedded loop points which automatically makes looping turn on, */
            result = system->playSound(sound1, 0, false, &channel);
            Sleep(1000);
            result = sound1->release();
    }



}

