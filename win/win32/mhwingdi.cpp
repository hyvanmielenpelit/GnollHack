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

/*
VOID OnPaint(HDC hdc)
{
    Graphics graphics(hdc);
    Pen      pen(Color(255, 0, 0, 255));
    graphics.DrawLine(&pen, 0, 0, 200, 100);
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
    HWND                hWnd;
    MSG                 msg;
    WNDCLASS            wndClass;
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;

    // Initialize GDI+.
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = WndProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hInstance;
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = TEXT("GettingStarted");

    RegisterClass(&wndClass);

    hWnd = CreateWindow(
        TEXT("GettingStarted"),   // window class name
        TEXT("Getting Started"),  // window caption
        WS_OVERLAPPEDWINDOW,      // window style
        CW_USEDEFAULT,            // initial x position
        CW_USEDEFAULT,            // initial y position
        CW_USEDEFAULT,            // initial x size
        CW_USEDEFAULT,            // initial y size
        NULL,                     // parent window handle
        NULL,                     // window menu handle
        hInstance,                // program instance handle
        NULL);                    // creation parameters

    ShowWindow(hWnd, iCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(gdiplusToken);
    return msg.wParam;
}  // WinMain

LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
    WPARAM wParam, LPARAM lParam)
{
    HDC          hdc;
    PAINTSTRUCT  ps;

    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        OnPaint(hdc);
        EndPaint(hWnd, &ps);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
} // WndProc
*/

