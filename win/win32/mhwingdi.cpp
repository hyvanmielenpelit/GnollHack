/* 
 * mhwingdi.cpp
 * GnollHack Windows GDI+ Routines
 *
 * Copyright (c) Janne Gustafsson, 2020
 */

#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <d2d1.h>
#include "wincodec.h"

using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

ULONG_PTR m_gdiplusToken = (ULONG_PTR)0;

ID2D1Factory* g_pD2DFactory = NULL;
ID2D1HwndRenderTarget* g_pRenderTargetMap = NULL;
ID2D1DCRenderTarget* g_pRenderTargetBackBufferDC = NULL;
ID2D1DCRenderTarget* g_pRenderTargetTileDC = NULL;
ID2D1BitmapRenderTarget* g_bitmapRenderTarget = NULL;


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



    /* Direct2D */
    void
    D2D_Init()
    {
        if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &g_pD2DFactory))) 
        {
            return;
        }
    }

    void
    D2D_InitMapRenderTargetOnCreate(HWND hWnd, WPARAM wParam, LPARAM lParam)
    {
        LPCREATESTRUCT lpcs = (LPCREATESTRUCT)lParam;
        HRESULT hr = g_pD2DFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(lpcs->cx, lpcs->cy)),
            &g_pRenderTargetMap
        );
    }

#if 0
    void
    D2DResizeBitmap(HWND hWnd, WPARAM wParam, LPARAM lParam)
    {
        if (FAILED(g_pRenderTarget->Resize(D2D1::SizeU(LOWORD(lParam), HIWORD(lParam))))) 
        {
            return;
        }

        g_pRenderTarget->CreateCompatibleRenderTarget(&g_bitmapRenderTarget);

        g_bitmapRenderTarget->BeginDraw();
        g_bitmapRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::AliceBlue));
        g_bitmapRenderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(100, 100), 50, 50), g_pBlackBrush);
        g_bitmapRenderTarget->EndDraw();
    }
#endif

    void D2D_DrawBitmap(HWND hWnd, WPARAM wParam, LPARAM lParam)
    {
        HDC hDc;
        PAINTSTRUCT ps;
        LPCRECT lpRect;
        ID2D1Bitmap* bitmap;
        D2D1_RECT_F d2d1Rect;
        hDc = BeginPaint(hWnd, &ps);
        lpRect = &ps.rcPaint;
        d2d1Rect = D2D1::RectF((float)lpRect->left, (float)lpRect->top, (float)lpRect->right, (float)lpRect->bottom);
        g_bitmapRenderTarget->GetBitmap(&bitmap);

        g_pRenderTargetMap->BeginDraw();
        g_pRenderTargetMap->DrawBitmap(
            bitmap, d2d1Rect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
            d2d1Rect
        );
        g_pRenderTargetMap->EndDraw();

        EndPaint(hWnd, &ps);
    }

    HRESULT LoadResourceBitmap(
        ID2D1RenderTarget* pRenderTarget,
        IWICImagingFactory* pIWICFactory,
        LPCSTR resourceName,
        LPCSTR resourceType,
        UINT destinationWidth,
        UINT destinationHeight,
        ID2D1Bitmap** ppBitmap
    )
    {
        IWICBitmapDecoder* pDecoder = NULL;
        IWICBitmapFrameDecode* pSource = NULL;
        IWICStream* pStream = NULL;
        IWICFormatConverter* pConverter = NULL;
        IWICBitmapScaler* pScaler = NULL;

        HRSRC imageResHandle = NULL;
        HGLOBAL imageResDataHandle = NULL;
        void* pImageFile = NULL;
        DWORD imageFileSize = 0;

        // Locate the resource.
        imageResHandle = FindResource(GetModuleHandle(NULL), resourceName, resourceType);
        HRESULT hr = imageResHandle ? S_OK : E_FAIL;
        if (SUCCEEDED(hr))
        {
            // Load the resource.
            imageResDataHandle = LoadResource(GetModuleHandle(NULL), imageResHandle);

            hr = imageResDataHandle ? S_OK : E_FAIL;
        }

        if (SUCCEEDED(hr))
        {
            // Lock it to get a system memory pointer.
            pImageFile = LockResource(imageResDataHandle);

            hr = pImageFile ? S_OK : E_FAIL;
        }
        if (SUCCEEDED(hr))
        {
            // Calculate the size.
            imageFileSize = SizeofResource(GetModuleHandle(NULL), imageResHandle);

            hr = imageFileSize ? S_OK : E_FAIL;

        }

        if (SUCCEEDED(hr))
        {
            // Create a WIC stream to map onto the memory.
            hr = pIWICFactory->CreateStream(&pStream);
        }
        if (SUCCEEDED(hr))
        {
            // Initialize the stream with the memory pointer and size.
            hr = pStream->InitializeFromMemory(
                reinterpret_cast<BYTE*>(pImageFile),
                imageFileSize
            );
        }

        if (SUCCEEDED(hr))
        {
            // Create a decoder for the stream.
            hr = pIWICFactory->CreateDecoderFromStream(
                pStream,
                NULL,
                WICDecodeMetadataCacheOnLoad,
                &pDecoder
            );
        }

        if (SUCCEEDED(hr))
        {
            // Create the initial frame.
            hr = pDecoder->GetFrame(0, &pSource);
        }

        if (SUCCEEDED(hr))
        {
            // Convert the image format to 32bppPBGRA
            // (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
            hr = pIWICFactory->CreateFormatConverter(&pConverter);
        }

        if (SUCCEEDED(hr))
        {
            hr = pConverter->Initialize(
                pSource,
                GUID_WICPixelFormat32bppPBGRA,
                WICBitmapDitherTypeNone,
                NULL,
                0.f,
                WICBitmapPaletteTypeMedianCut
            );
        }

        if (SUCCEEDED(hr))
        {
            //create a Direct2D bitmap from the WIC bitmap.
            hr = pRenderTarget->CreateBitmapFromWicBitmap(
                pConverter,
                NULL,
                ppBitmap
            );
        }

        if(pDecoder)
            pDecoder->Release();

        if (pSource)
            pSource->Release();
        
        if (pStream)
            pStream->Release();

        if (pConverter)
            pConverter->Release();

        if (pScaler)
            pScaler->Release();

        return hr;
    }

}

/* mhwingdi.cpp */
