/* GnollHack 4.0    mhwingdi.h */
/* Copyright (c) Janne Gustafsson, 2020.                  */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef MHWINGDI_H
#define MHWINGDI_H

extern void StartGdiplus(void);
extern void StopGdiplus(void);
extern HBITMAP LoadPNGFromResource(HINSTANCE, int, COLORREF);
extern HBITMAP LoadPNGFromResourceStandalone(HINSTANCE, int, COLORREF); /* Starts and stops GDI+ */
extern HBITMAP LoadPNGFromFile(char*, COLORREF);
extern void AddFontToGdiPlus(void*, size_t);
extern void DrawTextToRectangle(HDC, char*, RECT*, char*, float, int, COLORREF, double);

//extern void D2D_Init(void);
//extern void D2D_InitMapRenderTargetOnCreate(HWND, WPARAM, LPARAM);

#endif /* MHWINGDI_H */
