/* 
 * mhwingdi.cpp
 * GnollHack Windows GDI+ Routines
 *
 * Copyright (c) Janne Gustafsson, 2020
 */

#include "win10.h"
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <string>
#include <codecvt>
#include <locale>
 //#include <d2d1.h>
//#include "wincodec.h"

using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

ULONG_PTR m_gdiplusToken = (ULONG_PTR)0;
PrivateFontCollection* pMainFontCollection = NULL;

#if 0
ID2D1Factory* g_pD2DFactory = NULL;
ID2D1HwndRenderTarget* g_pRenderTargetMap = NULL;
ID2D1DCRenderTarget* g_pRenderTargetBackBufferDC = NULL;
ID2D1DCRenderTarget* g_pRenderTargetTileDC = NULL;
ID2D1BitmapRenderTarget* g_bitmapRenderTarget = NULL;
#endif

extern "C" {

    /* GDI+ */
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

        if (!hInstance)
            return hBmp;

        Color bkclr;
        bkclr.SetFromCOLORREF(bkcolor);
#ifdef UNICODE
        HRSRC hResource = ::FindResource(hInstance, MAKEINTRESOURCE(resource_id), L"PNG");
#else
        HRSRC hResource = ::FindResource(hInstance, MAKEINTRESOURCE(resource_id), "PNG");
#endif
        if (!hResource)
            return hBmp;

        DWORD imageSize = ::SizeofResource(hInstance, hResource);
        if (!imageSize)
            return hBmp;

        const void* pResourceData = ::LockResource(::LoadResource(hInstance, hResource));
        if (!pResourceData)
            return hBmp;

        HGLOBAL m_hBuffer = ::GlobalAlloc(GMEM_MOVEABLE, imageSize);
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

    HBITMAP
    LoadPNGFromFile(char* filename, COLORREF bkcolor)
    {
        HBITMAP hBmp = (HBITMAP)0;
        Color bkclr;
        bkclr.SetFromCOLORREF(bkcolor);

        const size_t cSize = strlen(filename) + 1;
        std::wstring wfilename(cSize, L'#');
        mbstowcs(&wfilename[0], filename, cSize);

        Bitmap* m_pBitmap = Bitmap::FromFile(wfilename.c_str(), FALSE);
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
        return hBmp;
    }

    void
    AddFontToGdiPlus(void* data, size_t length)
    {
        if (pMainFontCollection == NULL)
            pMainFontCollection = new PrivateFontCollection();

        Status stat = pMainFontCollection->AddMemoryFont(data, (INT)length);
        stat = stat;
    }

    void
    DrawTextToRectangle(HDC hDC, char* text, RECT* pDrawRect, char* font_name, float fontHeightinPoints, int font_attributes, COLORREF color, double monitorScale)
    {
        const size_t cSize = strlen(text) + 1;
        std::wstring wtext(cSize, L'#');
        mbstowcs(&wtext[0], text, cSize);

        const size_t cSizeFontName = strlen(font_name) + 1;
        std::wstring wtextfontname(cSizeFontName, L'#');
        mbstowcs(&wtextfontname[0], font_name, cSizeFontName);

        Color clr;
        clr.SetFromCOLORREF(color);
        SolidBrush brush(clr);

        if (monitorScale == 0.0)
            monitorScale = 1.0;

        enum Gdiplus::FontStyle fs = Gdiplus::FontStyle::FontStyleRegular;
        if (font_attributes & 1 /*ATR_BOLD*/)
        {
            fs = Gdiplus::FontStyle::FontStyleBold;
        }
        else if (font_attributes & 4 /*ATR_ULINE*/)
        {
            fs = Gdiplus::FontStyle::FontStyleUnderline;
        }

        Font myFont(wtextfontname.c_str(), (REAL)fontHeightinPoints, fs, UnitPoint, pMainFontCollection);

        RectF lRect = RectF::RectF((REAL)pDrawRect->left, (REAL)pDrawRect->top, (REAL)((double)(pDrawRect->right - pDrawRect->left) * monitorScale), (REAL)((double)(pDrawRect->bottom - pDrawRect->top) * monitorScale));

        StringFormat* pStrFormat = StringFormat::GenericTypographic()->Clone();
        pStrFormat->SetAlignment(StringAlignment::StringAlignmentNear);
        pStrFormat->SetLineAlignment(StringAlignment::StringAlignmentCenter);

        Graphics* pGraphics = Graphics::FromHDC(hDC);
        pGraphics->SetTextRenderingHint(TextRenderingHintAntiAlias);
        pGraphics->DrawString(wtext.c_str(), (INT)(cSize - 1), &myFont, lRect, pStrFormat, &brush);

        //Pen pen(Color(255, 255, 255, 255));
        //pGraphics->DrawRectangle(&pen, lRect);
    }

}

/* mhwingdi.cpp */
