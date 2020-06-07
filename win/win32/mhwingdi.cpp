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

ULONG_PTR m_gdiplusToken = (ULONG_PTR)0;

extern "C" {

    void StopGdiplus()
    {
        if (m_gdiplusToken)
            GdiplusShutdown(m_gdiplusToken);

        m_gdiplusToken = (ULONG_PTR)0;
    }

    void StartGdiplus()
    {
        StopGdiplus(); /* Just in case */

        GdiplusStartupInput gdiplusStartupInput;
        GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
    }

    HBITMAP
    LoadPNGFromResource(HINSTANCE hInstance, int resource_id, COLORREF bkcolor)
    {
        /* REMEMBER TO START GDI+ FIRST */
        HBITMAP hBmp = (HBITMAP)0;
        Color bkclr;
        bkclr.SetFromCOLORREF(bkcolor);

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

        return hBmp;
    }


    HBITMAP
    LoadPNGFromResourceStandalone(HINSTANCE hInstance, int resource_id, COLORREF bkcolor)
    {
        StartGdiplus();
        HBITMAP res = LoadPNGFromResource(hInstance, resource_id, bkcolor);
        StopGdiplus();
        return res;
    }



}

/* mhwingdi.cpp */
