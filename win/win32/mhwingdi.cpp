/* wingdi.cpp
 * Copyright 2020 by Janne Gustafsson
 */

//#include <stdafx.h>
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>

using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

extern "C" {

    HBITMAP
    loadPNG(HINSTANCE hInstance, LPCSTR lpBitmapName)
    {
        LPCSTR vIn = lpBitmapName;
        wchar_t* vOut = new wchar_t[strlen(vIn) + 1];
        mbstowcs_s(NULL, vOut, strlen(vIn) + 1, vIn, strlen(vIn));

        HBITMAP handle = (HBITMAP)0;
        Bitmap* pngBitmap = Bitmap::FromResource(hInstance, (const wchar_t*)lpBitmapName); // (const WCHAR*)MAKEINTRESOURCE(IDB_GNOLLHACK_TILES));
        if (!pngBitmap)
        {
            return handle;
        }
        pngBitmap->GetHBITMAP(0, &handle);
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
        Bitmap* pngBitmap = Bitmap::FromFile(vOut, FALSE); // (const WCHAR*)MAKEINTRESOURCE(IDB_GNOLLHACK_TILES));
        if (!pngBitmap)
        {
            return handle;
        }
        pngBitmap->GetHBITMAP(0, &handle);
        return handle;
    }

}

