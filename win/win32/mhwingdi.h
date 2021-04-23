/* GnollHack 4.0    mhwingdi.h */
/* Copyright (c) Janne Gustafsson, 2020.                  */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef MHWINGDI_H
#define MHWINGDI_H

extern void NDECL(StartGdiplus);
extern void NDECL(StopGdiplus);
extern HBITMAP FDECL(LoadPNGFromResource, (HINSTANCE, int, COLORREF));
extern HBITMAP FDECL(LoadPNGFromResourceStandalone, (HINSTANCE, int, COLORREF)); /* Starts and stops GDI+ */
extern HBITMAP FDECL(LoadPNGFromFile, (char*, COLORREF));
extern void FDECL(AddFontToGdiPlus, (void*, size_t));
extern void FDECL(DrawTextToRectangle, (HDC, char*, RECT*, char*, float, int, COLORREF, double));

//extern void NDECL(D2D_Init);
//extern void FDECL(D2D_InitMapRenderTargetOnCreate, (HWND, WPARAM, LPARAM));

#endif /* MHWINGDI_H */
