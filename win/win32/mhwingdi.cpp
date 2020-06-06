/* 
 * mhwingdi.cpp
 * GnollHack Windows GDI+ Routines
 *
 * Copyright (c) Janne Gustafsson, 2020
 */

#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>

using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

extern "C" {

    HBITMAP
    loadPNGResource(HINSTANCE hInstance, int resource_id, COLORREF bkcolor)
    {
        HBITMAP hBmp = (HBITMAP)0;
        ULONG_PTR m_gdiplusToken;
        Color bkclr;
        bkclr.SetFromCOLORREF(bkcolor);

        GdiplusStartupInput gdiplusStartupInput;
        GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

        HRSRC hResource = ::FindResource(hInstance, MAKEINTRESOURCE(resource_id), "PNG");
        if (!hResource)
            return hBmp;

        DWORD imageSize = ::SizeofResource(hInstance, hResource);
        if (!imageSize)
            return hBmp;

        const void* pResourceData = ::LockResource(::LoadResource(hInstance, hResource));
        if (!pResourceData)
            return hBmp;

        HGLOBAL m_hBuffer = ::GlobalAlloc(GMEM_MOVEABLE, imageSize);
        Bitmap* m_pBitmap = NULL;
        if (m_hBuffer)
        {
            void* pBuffer = ::GlobalLock(m_hBuffer);
            if (pBuffer)
            {
                CopyMemory(pBuffer, pResourceData, imageSize);

                IStream* pStream = NULL;
                if (::CreateStreamOnHGlobal(m_hBuffer, FALSE, &pStream) == S_OK)
                {
                    Bitmap* m_pBitmap = Bitmap::FromStream(pStream);
                    pStream->Release();
                    if (m_pBitmap)
                    {
                        if (m_pBitmap->GetLastStatus() == Gdiplus::Ok)
                        {
                            m_pBitmap->GetHBITMAP(bkclr, &hBmp);
                            /* Success */
                        }
                        delete m_pBitmap;
                        m_pBitmap = NULL;
                    }
                }
                m_pBitmap = NULL;
                ::GlobalUnlock(m_hBuffer);
            }
            ::GlobalFree(m_hBuffer);
            m_hBuffer = NULL;
        }

        GdiplusShutdown(m_gdiplusToken);

        return hBmp;
    }



}

/* mhwingdi.cpp */
