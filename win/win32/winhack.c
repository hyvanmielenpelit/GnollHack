/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2021-09-14 */

/* GnollHack 4.0    winhack.c    $NHDT-Date: 1449488876 2015/12/07 11:47:56 $  $NHDT-Branch: GnollHack-3.6.0 $:$NHDT-Revision: 1.44 $ */
/* Copyright (C) 2001 by Alex Kompel      */
/* GnollHack may be freely redistributed.  See license for details. */

// winhack.cpp : Defines the entry point for the application.
//

#include "win10.h"
#include <process.h>
#include <math.h>

#include "winMS.h"
#include "hack.h"
#include "dlb.h"
#include "resource.h"
#include "mhmain.h"
#include "mhmap.h"

#if !defined(SAFEPROCS)
#error You must #define SAFEPROCS to build winhack.c
#endif

/* Borland and MinGW redefine "boolean" in shlwapi.h,
   so just use the little bit we need */
typedef struct _DLLVERSIONINFO {
    DWORD cbSize;
    DWORD dwMajorVersion; // Major version
    DWORD dwMinorVersion; // Minor version
    DWORD dwBuildNumber;  // Build number
    DWORD dwPlatformID;   // DLLVER_PLATFORM_*
} DLLVERSIONINFO;

//
// The caller should always GetProcAddress("DllGetVersion"), not
// implicitly link to it.
//

typedef HRESULT(CALLBACK *DLLGETVERSIONPROC)(DLLVERSIONINFO *);

/* end of shlwapi.h */

/* Minimal common control library version
Version     _WIN_32IE   Platform/IE
=======     =========   ===========
4.00        0x0200      Microsoft(r) Windows 95/Windows NT 4.0
4.70        0x0300      Microsoft(r) Internet Explorer 3.x
4.71        0x0400      Microsoft(r) Internet Explorer 4.0
4.72        0x0401      Microsoft(r) Internet Explorer 4.01
...and probably going on infinitely...
*/
#define MIN_COMCTLMAJOR 4
#define MIN_COMCTLMINOR 71
#define INSTALL_NOTES "http://www.GnollHack.org/v340/ports/download-win.html#cc"
/*#define COMCTL_URL
 * "http://www.microsoft.com/msdownload/ieplatform/ie/comctrlx86.asp"*/

static TCHAR *_get_cmd_arg(TCHAR *pCmdLine);
static HRESULT GetComCtlVersion(LPDWORD pdwMajor, LPDWORD pdwMinor);
BOOL WINAPI
_nhapply_image_transparent(HDC hDC, int x, int y, int width, int height,
                           HDC sourceDC, int s_x, int s_y, int s_width,
                           int s_height, UINT cTransparent);

// Global Variables:
NHWinApp _GnollHack_app;
extern int GUILaunched;     /* We tell shared startup code in windmain.c
                               that the GUI was launched via this */

#ifdef __BORLANDC__
#define _stricmp(s1, s2) stricmp(s1, s2)
#define _strdup(s1) strdup(s1)
#endif

// Forward declarations of functions included in this code module:
extern boolean FDECL(main, (int, char **));

#define MAX_CMDLINE_PARAM 255

int APIENTRY
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine,
        int nCmdShow)
{
    INITCOMMONCONTROLSEX InitCtrls;
    int argc;
    char *argv[MAX_CMDLINE_PARAM];
    size_t len;
    TCHAR *p;
    TCHAR wbuf[BUFSZ];
    char buf[BUFSZ];

    DWORD major, minor;
    /* OSVERSIONINFO osvi; */

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

#if defined(DEBUG) && defined(_DEBUG) && defined(GNH_CRTDBG) && defined(WIN32) && defined(_CRTDBG_MAP_ALLOC)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    /*
     * Get a set of valid safe windowport function
     * pointers during early startup initialization.
     *
     * When get_safe_procs is called with 0 as the param,
     * non-functional, but safe function pointers are set
     * for all windowport routines.
     *
     * When get_safe_procs is called with 1 as the param,
     * raw_print, raw_print_bold, and wait_synch, and nhgetch
     * are set to use C stdio routines via stdio_raw_print,
     * stdio_raw_print_bold, stdio_wait_synch, and
     * stdio_nhgetch.
     */
    windowprocs = *get_safe_procs(0);

    /*
     * Now we are going to override a couple
     * of the windowprocs functions so that
     * error messages are handled in a suitable
     * way for the graphical version.
     */
    windowprocs.win_raw_print = mswin_raw_print;
    windowprocs.win_raw_print_bold = mswin_raw_print_bold;
    windowprocs.win_wait_synch = mswin_wait_synch;

    win10_init();
    sys_early_init();
    mswin_init_platform();

    /* init application structure */
    _GnollHack_app.hApp = hInstance;
    _GnollHack_app.hAccelTable =
        LoadAccelerators(hInstance, (LPCTSTR) IDC_NETHACKW);
    _GnollHack_app.hMainWnd = NULL;
    _GnollHack_app.hPopupWnd = NULL;

    /* Process tiledata */
    int total_tiles = process_tiledata(2, (const char*)0, (int*)0, (uchar*)0);
    int total_sheets = min(MAX_TILE_SHEETS, (total_tiles - 1) / NUM_TILES_PER_SHEET + 1);
    int tiles_per_line[MAX_TILE_SHEETS] = { 0 };
    int tiles_in_sheet[MAX_TILE_SHEETS] = { 0 };
    
    _GnollHack_app.mapTileSheets = total_sheets;

    for (int i = 0; i < _GnollHack_app.mapTileSheets; i++)
    {
        tiles_in_sheet[i] = (i == total_sheets - 1 ? ((total_tiles - 1) % NUM_TILES_PER_SHEET) + 1 : NUM_TILES_PER_SHEET);
        tiles_per_line[i] = (int)ceil(sqrt(1.5 * ((double)(tiles_in_sheet[i]))));
        if (tiles_per_line[i] % 3)
            tiles_per_line[i] += (3 - (tiles_per_line[i] % 3));

        int resource_idx = IDB_PNG_TILES;
        switch (i)
        {
        case 1:
#if defined(IDB_PNG_TILES_2)
            resource_idx = IDB_PNG_TILES_2;
#endif
            break;
        case 2:
#if defined(IDB_PNG_TILES_3)
            resource_idx = IDB_PNG_TILES_3;
#endif // IDB_PNG_TILES_3
            break;
        case 3:
#if defined(IDB_PNG_TILES_4)
            resource_idx = IDB_PNG_TILES_4;
#endif // IDB_PNG_TILES_4
            break;
        }

        _GnollHack_app.bmpTiles[i] = LoadPNGFromResource(hInstance, resource_idx, TILE_BK_COLOR);
        if (_GnollHack_app.bmpTiles[i] == NULL)
        {
            panic("Cannot load tiles bitmap %d", i + 1);
            return 0;
        }
    }
    _GnollHack_app.bmpRip = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_RIP));
    if (_GnollHack_app.bmpRip == NULL)
    {
        panic("cannot load rip bitmap");
        return 0;
    }
   // _GnollHack_app.bmpSplash = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_SPLASH));
    _GnollHack_app.bmpSplash = LoadPNGFromResource(hInstance, IDB_PNG_GNHLOGO, TILE_BK_COLOR);
    if (_GnollHack_app.bmpSplash == NULL)
    {
        panic("cannot load splash bitmap");
        return 0;
    }
    _GnollHack_app.bmpFMOD = LoadPNGFromResource(hInstance, IDB_PNG_FMODLOGO, TILE_BK_COLOR);
    if (_GnollHack_app.bmpFMOD == NULL)
    {
        panic("cannot load FMOD bitmap");
        return 0;
    }

    for (int i = 0; i < _GnollHack_app.mapTileSheets; i++)
    {
        _GnollHack_app.bmpMapTiles[i] = _GnollHack_app.bmpTiles[i];
        _GnollHack_app.mapTilesPerLine[i] = tiles_per_line[i];
    }

    _GnollHack_app.mapTile_X = TILE_X;
    _GnollHack_app.mapTile_Y = TILE_Y;

    _GnollHack_app.bNoHScroll = FALSE;
    _GnollHack_app.bNoVScroll = FALSE;
    _GnollHack_app.saved_text = strdup("");

    _GnollHack_app.bAutoLayout = TRUE;
    _GnollHack_app.bWindowsLocked = TRUE;

    _GnollHack_app.bNoSounds = FALSE;
#if 0  /* GdiTransparentBlt does not render spash bitmap for whatever reason */
    /* use system-provided TransparentBlt for Win2k+ */
    ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&osvi);
    if (osvi.dwMajorVersion >= 5)
        _GnollHack_app.lpfnTransparentBlt = GdiTransparentBlt;
    else
#endif
        _GnollHack_app.lpfnTransparentBlt = _nhapply_image_transparent;

    // init controls
    if (FAILED(GetComCtlVersion(&major, &minor))) {
        char buf[TBUFSZ];
        Sprintf(buf, "Cannot load common control library.\n%s\n%s",
                "For further information, refer to the installation notes at",
                INSTALL_NOTES);
        panic(buf);
        return 0;
    }
    if (major < MIN_COMCTLMAJOR
        || (major == MIN_COMCTLMAJOR && minor < MIN_COMCTLMINOR)) {
        char buf[TBUFSZ];
        Sprintf(buf, "Common control library is outdated.\n%s %d.%d\n%s\n%s",
                "GnollHack requires at least version ", MIN_COMCTLMAJOR,
                MIN_COMCTLMINOR,
                "For further information, refer to the installation notes at",
                INSTALL_NOTES);
        panic(buf);
        return 0;
    }
    ZeroMemory(&InitCtrls, sizeof(InitCtrls));
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_LISTVIEW_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    /* get command line parameters */
    p = _get_cmd_arg(GetCommandLine());
    p = _get_cmd_arg(NULL); /* skip first paramter - command name */
    for (argc = 1; p && argc < MAX_CMDLINE_PARAM; argc++) {
        len = _tcslen(p);
        if (len > 0) {
            argv[argc] = _strdup(NH_W2A(p, buf, BUFSZ));
        } else {
            argv[argc] = "";
        }
        p = _get_cmd_arg(NULL);
    }
    GetModuleFileName(NULL, wbuf, BUFSZ);
    argv[0] = _strdup(NH_W2A(wbuf, buf, BUFSZ));

    if (argc == 2) {
        TCHAR *savefile = strdup(argv[1]);
        TCHAR *plname;
        for (p = savefile; *p && *p != '-'; p++)
            ;
        if (*p) {
            /* we found a '-' */
            plname = p + 1;
            for (p = plname; *p && *p != '.'; p++)
                ;
            if (*p) {
                if (strcmp(p + 1, "GnollHack-saved-game") == 0) {
                    *p = '\0';
                    argv[1] = "-u";
                    argv[2] = _strdup(plname);
                    argc = 3;
                }
            }
        }
        free(savefile);
    }
    GUILaunched = 1;

    /* let main do the argument processing */
    (void) main(argc, argv);

    mswin_exit_platform(EXIT_SUCCESS);

    return 0;
}

PNHWinApp
GetNHApp()
{
    return &_GnollHack_app;
}

TCHAR *
_get_cmd_arg(TCHAR *pCmdLine)
{
    static TCHAR *pArgs = NULL;
    TCHAR *pRetArg;
    BOOL bQuoted;

    if (!pCmdLine && !pArgs)
        return NULL;
    if (!pArgs)
        pArgs = pCmdLine;

    /* skip whitespace */
    for (pRetArg = pArgs; *pRetArg && _istspace(*pRetArg);
         pRetArg = CharNext(pRetArg))
        ;
    if (!*pRetArg) {
        pArgs = NULL;
        return NULL;
    }

    /* check for quote */
    if (*pRetArg == TEXT('"')) {
        bQuoted = TRUE;
        pRetArg = CharNext(pRetArg);
        pArgs = _tcschr(pRetArg, TEXT('"'));
    } else {
        /* skip to whitespace */
        for (pArgs = pRetArg; *pArgs && !_istspace(*pArgs);
             pArgs = CharNext(pArgs))
            ;
    }

    if (pArgs && *pArgs) {
        TCHAR *p;
        p = pArgs;
        pArgs = CharNext(pArgs);
        *p = (TCHAR) 0;
    } else {
        pArgs = NULL;
    }

    return pRetArg;
}

/* Get the version of the Common Control library on this machine.
   Copied from the Microsoft SDK
 */
HRESULT
GetComCtlVersion(LPDWORD pdwMajor, LPDWORD pdwMinor)
{
    HINSTANCE hComCtl;
    HRESULT hr = S_OK;
    DLLGETVERSIONPROC pDllGetVersion;

    if (IsBadWritePtr(pdwMajor, sizeof(DWORD))
        || IsBadWritePtr(pdwMinor, sizeof(DWORD)))
        return E_INVALIDARG;
    // load the DLL
    hComCtl = LoadLibrary(TEXT("comctl32.dll"));
    if (!hComCtl)
        return E_FAIL;

    /*
    You must get this function explicitly because earlier versions of the DLL
    don't implement this function. That makes the lack of implementation of
    the
    function a version marker in itself.
    */
    pDllGetVersion =
        (DLLGETVERSIONPROC) GetProcAddress(hComCtl, TEXT("DllGetVersion"));
    if (pDllGetVersion) {
        DLLVERSIONINFO dvi;
        ZeroMemory(&dvi, sizeof(dvi));
        dvi.cbSize = sizeof(dvi);
        hr = (*pDllGetVersion)(&dvi);
        if (SUCCEEDED(hr)) {
            *pdwMajor = dvi.dwMajorVersion;
            *pdwMinor = dvi.dwMinorVersion;
        } else {
            hr = E_FAIL;
        }
    } else {
        /*
        If GetProcAddress failed, then the DLL is a version previous to the
        one
        shipped with IE 3.x.
        */
        *pdwMajor = 4;
        *pdwMinor = 0;
    }
    FreeLibrary(hComCtl);
    return hr;
}

/* apply bitmap pointed by sourceDc transparently over
bitmap pointed by hDC */
BOOL WINAPI
_nhapply_image_transparent(HDC hDC, int x, int y, int width, int height,
                           HDC sourceDC, int s_x, int s_y, int s_width,
                           int s_height, UINT cTransparent)
{
    /* Don't use TransparentBlt; According to Microsoft, it contains a memory
     * leak in Window 95/98. */
    HDC hdcMem, hdcBack, hdcObject, hdcSave;
    COLORREF cColor;
    HBITMAP bmAndBack, bmAndObject, bmAndMem, bmSave;
    HBITMAP bmBackOld, bmObjectOld, bmMemOld, bmSaveOld;

    /* Create some DCs to hold temporary data. */
    hdcBack = CreateCompatibleDC(hDC);
    hdcObject = CreateCompatibleDC(hDC);
    hdcMem = CreateCompatibleDC(hDC);
    hdcSave = CreateCompatibleDC(hDC);

    /* this is bitmap for our pet image */
    bmSave = CreateCompatibleBitmap(hDC, width, height);

    /* Monochrome DC */
    bmAndBack = CreateBitmap(width, height, 1, 1, NULL);
    bmAndObject = CreateBitmap(width, height, 1, 1, NULL);

    /* resulting bitmap */
    bmAndMem = CreateCompatibleBitmap(hDC, width, height);

    /* Each DC must select a bitmap object to store pixel data. */
    bmBackOld = SelectObject(hdcBack, bmAndBack);
    bmObjectOld = SelectObject(hdcObject, bmAndObject);
    bmMemOld = SelectObject(hdcMem, bmAndMem);
    bmSaveOld = SelectObject(hdcSave, bmSave);

    /* copy source image because it is going to be overwritten */
    SetStretchBltMode(hdcSave, COLORONCOLOR);
    StretchBlt(hdcSave, 0, 0, width, height, sourceDC, s_x, s_y, s_width,
        s_height, SRCCOPY);

    /* Set the background color of the source DC to the color.
    contained in the parts of the bitmap that should be transparent */
    cColor = SetBkColor(hdcSave, cTransparent);

    /* Create the object mask for the bitmap by performing a BitBlt
    from the source bitmap to a monochrome bitmap. */
    BitBlt(hdcObject, 0, 0, width, height, hdcSave, 0, 0, SRCCOPY);

    /* Set the background color of the source DC back to the original
    color. */
    SetBkColor(hdcSave, cColor);

    /* Create the inverse of the object mask. */
    BitBlt(hdcBack, 0, 0, width, height, hdcObject, 0, 0, NOTSRCCOPY);

    /* Copy background to the resulting image  */
    BitBlt(hdcMem, 0, 0, width, height, hDC, x, y, SRCCOPY);

    /* Mask out the places where the source image will be placed. */
    BitBlt(hdcMem, 0, 0, width, height, hdcObject, 0, 0, SRCAND);

    /* Mask out the transparent colored pixels on the source image. */
    BitBlt(hdcSave, 0, 0, width, height, hdcBack, 0, 0, SRCAND);

    /* XOR the source image with the beckground. */
    BitBlt(hdcMem, 0, 0, width, height, hdcSave, 0, 0, SRCPAINT);

    /* blt resulting image to the screen */
    BitBlt(hDC, x, y, width, height, hdcMem, 0, 0, SRCCOPY);

    /* cleanup */
    DeleteObject(SelectObject(hdcBack, bmBackOld));
    DeleteObject(SelectObject(hdcObject, bmObjectOld));
    DeleteObject(SelectObject(hdcMem, bmMemOld));
    DeleteObject(SelectObject(hdcSave, bmSaveOld));

    DeleteDC(hdcMem);
    DeleteDC(hdcBack);
    DeleteDC(hdcObject);
    DeleteDC(hdcSave);

    return TRUE;
}
