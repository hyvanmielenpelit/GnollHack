/* GnollHack 4.0	mhmap.c	$NHDT-Date: 1435002695 2015/06/22 19:51:35 $  $NHDT-Branch: master $:$NHDT-Revision: 1.56 $ */
/* Copyright (C) 2001 by Alex Kompel      */
/* GnollHack may be freely redistributed.  See license for details. */

#include "win10.h"
#include "winMS.h"
#include "winos.h"

#include "mhfont.h"
#include "mhinput.h"
#include "mhmap.h"
#include "mhmsg.h"
#include "resource.h"

#include "color.h"
#include "patchlevel.h"
#include "layer.h"

#define NHMAP_FONT_NAME TEXT("Terminal")
#define NHMAP_TTFONT_NAME TEXT("Consolas")
#define MAXWINDOWTEXT 255

#define ANIMATION_TIMER_INTERVAL 40 // milliseconds (25 FPS)
#define CURSOR_BLINK_IN_INTERVALS 25
#define CURSOR_HEIGHT 2 // pixels

/* map window data */
typedef struct mswin_GnollHack_map_window {
    HWND hWnd;                  /* window */

    struct layer_info map[COLNO][ROWNO];    /* glyph map */
    boolean mapAnimated[COLNO][ROWNO];      /* animation flag for map */
    boolean mapDirty[COLNO][ROWNO];         /* dirty flag for map */

    int mapMode;                /* current map mode */
    boolean bAsciiMode;         /* switch ASCII/tiled mode */
    boolean bFitToScreenMode;   /* switch Fit map to screen mode on/off */
    int xPos, yPos;             /* scroll position */
    int xPageSize, yPageSize;   /* scroll page size */
    int xMin, xMax, yMin, yMax; /* scroll range */
    int xCur, yCur;             /* position of the cursor */
    int xFrontTile, yFrontTile; /* size of tile in front buffer in pixels */
    int xBackTile, yBackTile;   /* size of tile in back buffer in pixels */
    POINT map_orig;             /* map origin point */

    HFONT hMapFont;             /* font for ASCII mode */
    boolean bUnicodeFont;       /* font supports unicode page 437 */

    int tileWidth;              /* width of tile in pixels at 96 dpi */
    int tileHeight;             /* height of tile in pixels at 96 dpi */
    double backScale;           /* scaling from source to back buffer */
    double frontScale;          /* scaling from back to front */
    double monitorScale;        /* from 96dpi to monitor dpi*/

    boolean cursorOn;
    unsigned long interval_counter;
    int yNoBlinkCursor;         /* non-blinking cursor height inback buffer
                                   in pixels */
    int yBlinkCursor;           /* blinking cursor height inback buffer
                                   in pixels */

    int backWidth;              /* back buffer width */
    int backHeight;             /* back buffer height */
    HBITMAP hBackBuffer;        /* back buffe bitmap */
    HDC backBufferDC;          /* back buffer drawing context */

    HDC tileDC;                /* tile drawing context */

} NHMapWindow, *PNHMapWindow;

static TCHAR szNHMapWindowClass[] = TEXT("MSGnollHackMapWndClass");
LRESULT CALLBACK MapWndProc(HWND, UINT, WPARAM, LPARAM);
static void register_map_window_class(void);
static void onMSNHCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);
static void onMSNH_VScroll(HWND hWnd, WPARAM wParam, LPARAM lParam);
static void onMSNH_HScroll(HWND hWnd, WPARAM wParam, LPARAM lParam);
static void onPaint(HWND hWnd);
static void onCreate(HWND hWnd, WPARAM wParam, LPARAM lParam);
static void nhcoord2display(PNHMapWindow data, int x, int y, LPRECT lpOut);
static void paint(PNHMapWindow data, int i, int j);
static void dirtyAll(PNHMapWindow data);
static void dirty(PNHMapWindow data, int i, int j);
static void setGlyph(PNHMapWindow data, int i, int j, struct layer_info layers);
static void clearAll(PNHMapWindow data);

#if (VERSION_MAJOR < 0) && (VERSION_MINOR < 4) && (PATCHLEVEL < 2)
static void nhglyph2charcolor(short glyph, uchar *ch, int *color);
#endif
extern boolean win32_cursorblink;       /* from sys\winnt\winnt.c */

HWND
mswin_init_map_window()
{
    static int run_once = 0;
    HWND hWnd;
    RECT rt;

    if (!run_once) {
        register_map_window_class();
        run_once = 1;
    }

    /* get window position */
    if (GetNHApp()->bAutoLayout) {
        SetRect(&rt, 0, 0, 0, 0);
    } else {
        mswin_get_window_placement(NHW_MAP, &rt);
    }

    /* create map window object */
    hWnd = CreateWindow(
        szNHMapWindowClass, /* registered class name */
        NULL,               /* window name */
        WS_CHILD | WS_HSCROLL | WS_VSCROLL | WS_CLIPSIBLINGS
            | WS_SIZEBOX,     /* window style */
        rt.left,              /* horizontal position of window */
        rt.top,               /* vertical position of window */
        rt.right - rt.left,   /* window width */
        rt.bottom - rt.top,   /* window height */
        GetNHApp()->hMainWnd, /* handle to parent or owner window */
        NULL,                 /* menu handle or child identifier */
        GetNHApp()->hApp,     /* handle to application instance */
        NULL);                /* window-creation data */
    if (!hWnd) {
        panic("Cannot create map window");
		return (HWND)0;
    }

    /* Set window caption */
    SetWindowText(hWnd, "Map");

    mswin_apply_window_style(hWnd);

    /* set cursor blink timer */
    SetTimer(hWnd, 0, ANIMATION_TIMER_INTERVAL, NULL);

    return hWnd;
}

void
mswin_map_stretch(HWND hWnd, LPSIZE map_size, BOOL redraw)
{
    /* check arguments */
    if (!IsWindow(hWnd) || !map_size || map_size->cx <= 0
        || map_size->cy <= 0)
        return;

    PNHMapWindow data = (PNHMapWindow) GetWindowLongPtr(hWnd, GWLP_USERDATA);

    /* calculate window size */
    RECT client_rt;
    GetClientRect(hWnd, &client_rt);

    SIZE wnd_size;
    wnd_size.cx = client_rt.right - client_rt.left;
    wnd_size.cy = client_rt.bottom - client_rt.top;

    // calculate back buffer scale
    data->monitorScale = win10_monitor_scale(hWnd);

    boolean bText = data->bAsciiMode ||
                    (u.uz.dlevel != 0 && Is_rogue_level(&u.uz));

    if (bText && !data->bFitToScreenMode)
        data->backScale = data->monitorScale;
    else
        data->backScale = 1.0;

    /* set back buffer tile size */
    if (bText && data->bFitToScreenMode) {
        data->xBackTile = wnd_size.cx / COLNO;
        data->yBackTile = wnd_size.cy / ROWNO;
        data->yBackTile = max(data->yBackTile, 12);
    } else {
        data->xBackTile = (int)(data->tileWidth * data->backScale);
        data->yBackTile = (int)(data->tileHeight * data->backScale);
    }

    if (bText) {
        LOGFONT lgfnt;

        ZeroMemory(&lgfnt, sizeof(lgfnt));
        if (data->bFitToScreenMode) {
            lgfnt.lfHeight = -data->yBackTile;     // height of font
            lgfnt.lfWidth = 0;                     // average character width
        } else {
            lgfnt.lfHeight = -data->yBackTile;     // height of font
            lgfnt.lfWidth = -data->xBackTile;      // average character width
        }
        lgfnt.lfEscapement = 0;                    // angle of escapement
        lgfnt.lfOrientation = 0;                   // base-line orientation angle
        lgfnt.lfWeight = FW_NORMAL;                // font weight
        lgfnt.lfItalic = FALSE;                    // italic attribute option
        lgfnt.lfUnderline = FALSE;                 // underline attribute option
        lgfnt.lfStrikeOut = FALSE;                 // strikeout attribute option
        lgfnt.lfCharSet = mswin_charset();         // character set identifier
        lgfnt.lfOutPrecision = OUT_DEFAULT_PRECIS; // output precision
        lgfnt.lfClipPrecision = CLIP_DEFAULT_PRECIS; // clipping precision
        if (data->bFitToScreenMode) {
            lgfnt.lfQuality = ANTIALIASED_QUALITY; // output quality
        } else {
            lgfnt.lfQuality = NONANTIALIASED_QUALITY; // output quality
        }
        if (iflags.wc_font_map && *iflags.wc_font_map) {
            lgfnt.lfPitchAndFamily = DEFAULT_PITCH; // pitch and family
            NH_A2W(iflags.wc_font_map, lgfnt.lfFaceName, LF_FACESIZE);
        } else {
            if (!data->bFitToScreenMode) {
                lgfnt.lfPitchAndFamily = FIXED_PITCH; // pitch and family
                NH_A2W(NHMAP_FONT_NAME, lgfnt.lfFaceName, LF_FACESIZE);
            } else {
                lgfnt.lfPitchAndFamily = DEFAULT_PITCH; // pitch and family
                NH_A2W(NHMAP_TTFONT_NAME, lgfnt.lfFaceName, LF_FACESIZE);
            }
        }

        TEXTMETRIC textMetrics;
        HFONT font = NULL;

        while (1) {

            if (font != NULL)
                DeleteObject(font);

            font = CreateFontIndirect(&lgfnt);

            SelectObject(data->backBufferDC, font);

            GetTextMetrics(data->backBufferDC, &textMetrics);

            if (!data->bFitToScreenMode)
                break;

            if ((textMetrics.tmHeight > data->yBackTile ||
                 textMetrics.tmAveCharWidth > data->xBackTile) &&
                lgfnt.lfHeight < -MIN_FONT_HEIGHT) {
                lgfnt.lfHeight++;
                continue;
            }

            break;
        }

        if (data->hMapFont)
            DeleteObject(data->hMapFont);

        data->hMapFont = font;

        data->bUnicodeFont = winos_font_support_cp437(data->hMapFont);

        // set tile size to match font metrics

        data->xBackTile = textMetrics.tmAveCharWidth;
        data->yBackTile = textMetrics.tmHeight;

    }

    int backWidth = COLNO * data->xBackTile;
    int backHeight = ROWNO * data->yBackTile;

    /* create back buffer */

    if (data->backWidth != backWidth || data->backHeight != backHeight) {

        HDC frontBufferDC = GetDC(hWnd);
        HBITMAP hBackBuffer = CreateCompatibleBitmap(frontBufferDC, backWidth, backHeight);
        ReleaseDC(hWnd, frontBufferDC);
        
        if (data->hBackBuffer != NULL) {
            SelectBitmap(data->backBufferDC, hBackBuffer);
            DeleteObject(data->hBackBuffer);
        }

        data->backWidth = backWidth;
        data->backHeight = backHeight;

        SelectBitmap(data->backBufferDC, hBackBuffer);
        data->hBackBuffer = hBackBuffer;
    }

    /* calculate front buffer tile size */

    if (wnd_size.cx > 0 && wnd_size.cy > 0 && !bText && data->bFitToScreenMode) {
        double windowAspectRatio =
            (double) wnd_size.cx / (double) wnd_size.cy;

        double backAspectRatio = 
            (double) data->backWidth / (double) data->backHeight;

        if (windowAspectRatio > backAspectRatio)
            data->frontScale = (double) wnd_size.cy / (double) data->backHeight;
        else
            data->frontScale = (double) wnd_size.cx / (double) data->backWidth;

    } else {

        if (bText) {
            data->frontScale = 1.0;
        } else {
            data->frontScale = data->monitorScale * (1.0 + flags.screen_scale_adjustment);
        }

    }

    data->xFrontTile = (int) ((double) data->xBackTile * data->frontScale);
    data->yFrontTile = (int) ((double) data->yBackTile * data->frontScale);

    /* calcuate ASCII cursor height */
    data->yBlinkCursor = (int) ((double) CURSOR_HEIGHT * data->backScale);
    data->yNoBlinkCursor = data->yBackTile;

    /* set map origin point */
    data->map_orig.x =
        max(0, client_rt.left + (wnd_size.cx - data->xFrontTile * COLNO) / 2);
    data->map_orig.y =
        max(0, client_rt.top + (wnd_size.cy - data->yFrontTile * ROWNO) / 2);

    data->map_orig.x -= data->map_orig.x % data->xFrontTile;
    data->map_orig.y -= data->map_orig.y % data->yFrontTile;

    // Set horizontal scroll

    data->xPageSize = min(COLNO, wnd_size.cx / data->xFrontTile);

    GetNHApp()->bNoHScroll = (data->xPageSize == COLNO);

    data->xMin = 0;
    data->xMax = COLNO - data->xPageSize;
    data->xPos = max(0, min(data->xMax, u.ux - (data->xPageSize / 2)));

    SCROLLINFO si;

    si.cbSize = sizeof(si);
    si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
    si.nMin = data->xMin;
    si.nMax = data->xMax;
    si.nPage = 1;
    si.nPos = data->xPos;
    SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);

    data->yPageSize = min(ROWNO, wnd_size.cy / data->yFrontTile);

    GetNHApp()->bNoVScroll = (data->yPageSize == ROWNO);

    data->yMin = 0;
    data->yMax = ROWNO - data->yPageSize;
    data->yPos = max(0, min(data->yMax, u.uy - (data->yPageSize / 2)));

    si.cbSize = sizeof(si);
    si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
    si.nMin = data->yMin;
    si.nMax = data->yMax;
    si.nPage = 1;
    si.nPos = data->yPos;
    SetScrollInfo(hWnd, SB_VERT, &si, TRUE);

    mswin_cliparound(data->xCur, data->yCur);

    if (redraw) {
        dirtyAll(data);
        InvalidateRect(hWnd, NULL, TRUE);
    }
}

/* set map mode */
int
mswin_map_mode(HWND hWnd, int mode)
{
    PNHMapWindow data;
    int oldMode;
    SIZE mapSize;

    data = (PNHMapWindow) GetWindowLongPtr(hWnd, GWLP_USERDATA);
    if (mode == data->mapMode)
        return mode;

    oldMode = data->mapMode;
    data->mapMode = mode;

    switch (data->mapMode) {
    case MAP_MODE_ASCII4x6:
        data->bAsciiMode = TRUE;
        data->bFitToScreenMode = FALSE;
        data->tileWidth = 4;
        data->tileHeight = 6;
        break;

    case MAP_MODE_ASCII6x8:
        data->bAsciiMode = TRUE;
        data->bFitToScreenMode = FALSE;
        data->tileWidth = 6;
        data->tileHeight = 8;
        break;

    case MAP_MODE_ASCII8x8:
        data->bAsciiMode = TRUE;
        data->bFitToScreenMode = FALSE;
        data->tileWidth = 8;
        data->tileHeight = 8;
        break;

    case MAP_MODE_ASCII16x8:
        data->bAsciiMode = TRUE;
        data->bFitToScreenMode = FALSE;
        data->tileWidth = 16;
        data->tileHeight = 8;
        break;

    case MAP_MODE_ASCII7x12:
        data->bAsciiMode = TRUE;
        data->bFitToScreenMode = FALSE;
        data->tileWidth = 7;
        data->tileHeight = 12;
        break;

    case MAP_MODE_ASCII8x12:
        data->bAsciiMode = TRUE;
        data->bFitToScreenMode = FALSE;
        data->tileWidth = 8;
        data->tileHeight = 12;
        break;

    case MAP_MODE_ASCII16x12:
        data->bAsciiMode = TRUE;
        data->bFitToScreenMode = FALSE;
        data->tileWidth = 16;
        data->tileHeight = 12;
        break;

    case MAP_MODE_ASCII12x16:
        data->bAsciiMode = TRUE;
        data->bFitToScreenMode = FALSE;
        data->tileWidth = 12;
        data->tileHeight = 16;
        break;

    case MAP_MODE_ASCII10x18:
        data->bAsciiMode = TRUE;
        data->bFitToScreenMode = FALSE;
        data->tileWidth = 10;
        data->tileHeight = 18;
        break;

    case MAP_MODE_ASCII_FIT_TO_SCREEN:
        data->bAsciiMode = TRUE;
        data->bFitToScreenMode = TRUE;
        data->tileWidth = 12;
        data->tileHeight = 16;
        break;

    case MAP_MODE_TILES_FIT_TO_SCREEN:
        data->bAsciiMode = FALSE;
        data->bFitToScreenMode = TRUE;
        data->tileWidth = GetNHApp()->mapTile_X;
        data->tileHeight = GetNHApp()->mapTile_Y;
        break;

    case MAP_MODE_TILES:
    default:
        data->bAsciiMode = FALSE;
        data->bFitToScreenMode = FALSE;
        data->tileWidth = GetNHApp()->mapTile_X;
        data->tileHeight = GetNHApp()->mapTile_Y;
        break;
    }

    mapSize.cx = data->tileWidth * COLNO;
    mapSize.cy = data->tileHeight * ROWNO;

    mswin_map_stretch(hWnd, &mapSize, TRUE);

    mswin_update_inventory(); /* for perm_invent to hide/show tiles */

    return oldMode;
}

/* register window class for map window */
void
register_map_window_class()
{
    WNDCLASS wcex;
    ZeroMemory(&wcex, sizeof(wcex));

    /* window class */
    wcex.style = CS_NOCLOSE | CS_DBLCLKS;
    wcex.lpfnWndProc = (WNDPROC) MapWndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = GetNHApp()->hApp;
    wcex.hIcon = NULL;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground =
        CreateSolidBrush(RGB(0, 0, 0)); /* set backgroup here */
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szNHMapWindowClass;

    if (!RegisterClass(&wcex)) {
        panic("cannot register Map window class");
		return;
    }
}

/* map window procedure */
LRESULT CALLBACK
MapWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PNHMapWindow data;

    data = (PNHMapWindow) GetWindowLongPtr(hWnd, GWLP_USERDATA);
    switch (message) {
    case WM_CREATE:
        onCreate(hWnd, wParam, lParam);
        break;

    case WM_MSNH_COMMAND:
        onMSNHCommand(hWnd, wParam, lParam);
        break;

    case WM_PAINT:
        onPaint(hWnd);
        break;

    case WM_SETFOCUS:
        /* transfer focus back to the main window */
        SetFocus(GetNHApp()->hMainWnd);
        break;

    case WM_HSCROLL:
        onMSNH_HScroll(hWnd, wParam, lParam);
        break;

    case WM_VSCROLL:
        onMSNH_VScroll(hWnd, wParam, lParam);
        break;

    case WM_MOUSEWHEEL: 
    {
        if (LOWORD(wParam) == MK_CONTROL)
        {
            short wheeldistance = HIWORD(wParam);
            if(wheeldistance < 0)
                flags.screen_scale_adjustment -= MOUSE_SCREEN_SCALE_ADJUSTMENT_STEP;
            else 
                flags.screen_scale_adjustment += MOUSE_SCREEN_SCALE_ADJUSTMENT_STEP;

            if (flags.screen_scale_adjustment < MIN_SCREEN_SCALE_ADJUSTMENT)
                flags.screen_scale_adjustment = MIN_SCREEN_SCALE_ADJUSTMENT;
            if (flags.screen_scale_adjustment > MAX_SCREEN_SCALE_ADJUSTMENT)
                flags.screen_scale_adjustment = MAX_SCREEN_SCALE_ADJUSTMENT;

            SIZE size;
            size.cx = data->xFrontTile * COLNO;
            size.cy = data->yFrontTile * ROWNO;
            mswin_map_stretch(hWnd, &size, TRUE);
        }
        else
        {
            short wheeldistance = HIWORD(wParam);
            WORD scrollcommand = (WORD)0;
            if(wheeldistance < 0)
                scrollcommand = SB_LINEDOWN;
            else
                scrollcommand = SB_LINEUP;
            WPARAM scroll_wparam = MAKEWPARAM(scrollcommand, 0);

            onMSNH_VScroll(hWnd, scroll_wparam, (LPARAM)0);
        }
    }
    break;

    case WM_SIZE: {
        RECT rt;
        SIZE size;

        if (data->bFitToScreenMode) {
            size.cx = LOWORD(lParam);
            size.cy = HIWORD(lParam);
        } else {
            /* mapping factor is unchaged we just need to adjust scroll bars
             */
            size.cx = data->xFrontTile * COLNO;
            size.cy = data->yFrontTile * ROWNO;
        }
        mswin_map_stretch(hWnd, &size, TRUE);

        /* update window placement */
        GetWindowRect(hWnd, &rt);
        ScreenToClient(GetNHApp()->hMainWnd, (LPPOINT) &rt);
        ScreenToClient(GetNHApp()->hMainWnd, ((LPPOINT) &rt) + 1);
        mswin_update_window_placement(NHW_MAP, &rt);
    } break;

    case WM_MOVE: {
        RECT rt;
        GetWindowRect(hWnd, &rt);
        ScreenToClient(GetNHApp()->hMainWnd, (LPPOINT) &rt);
        ScreenToClient(GetNHApp()->hMainWnd, ((LPPOINT) &rt) + 1);
        mswin_update_window_placement(NHW_MAP, &rt);
    }
        return FALSE;

    case WM_LBUTTONDOWN:
        NHEVENT_MS(CLICK_1,
                   max(0, min(COLNO, data->xPos
                                         + (LOWORD(lParam) - data->map_orig.x)
                                               / data->xFrontTile)),
                   max(0, min(ROWNO, data->yPos
                                         + (HIWORD(lParam) - data->map_orig.y)
                                               / data->yFrontTile)));
        return 0;

    case WM_LBUTTONDBLCLK:
    case WM_RBUTTONDOWN:
        NHEVENT_MS(CLICK_2,
                   max(0, min(COLNO, data->xPos
                                         + (LOWORD(lParam) - data->map_orig.x)
                                               / data->xFrontTile)),
                   max(0, min(ROWNO, data->yPos
                                         + (HIWORD(lParam) - data->map_orig.y)
                                               / data->yFrontTile)));
        return 0;

    case WM_DESTROY:
        if (data->hMapFont)
            DeleteObject(data->hMapFont);
        if (data->hBackBuffer)
            DeleteBitmap(data->hBackBuffer);
        if (data->backBufferDC)
            DeleteDC(data->backBufferDC);
        free(data);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR) 0);
        break;

    case WM_TIMER:
    {
        if(data->interval_counter >= 0xCFFFFFFFUL)
            data->interval_counter = 0UL;
        else
            data->interval_counter++;

        for (int x = 0; x < COLNO; x++)
            for (int y = 0; y < ROWNO; y++) {
                if(data->mapAnimated[x][y])
                    dirty(data, x, y);
            }

        boolean asciimode = (data->bAsciiMode || Is_rogue_level(&u.uz));
        if ((asciimode && !win32_cursorblink)
            || (!asciimode && (!flags.blinking_cursor_on_tiles
                    || (/*data->xCur == u.ux && data->yCur == u.uy &&*/ !flags.show_cursor_on_u && !flags.force_paint_at_cursor))
               )
            )
        {
            //Nothing
        }
        else
        {

            if (data->interval_counter >= 0xCFFFFFFFUL)
                data->interval_counter = 0UL;
            else
                data->interval_counter++;

            if (flags.blinking_cursor_on_tiles && data->interval_counter % CURSOR_BLINK_IN_INTERVALS == 0)
            {
                if (flags.force_paint_at_cursor)
                {
                    flags.force_paint_at_cursor = FALSE;
                    //data->cursorOn = TRUE;
                }
                else
                    data->cursorOn = !data->cursorOn;

                dirty(data, data->xCur, data->yCur);
            }
        }
        break;
    }
    case WM_DPICHANGED: {
        RECT rt;
        GetWindowRect(hWnd, &rt);
        ScreenToClient(GetNHApp()->hMainWnd, (LPPOINT)&rt);
        ScreenToClient(GetNHApp()->hMainWnd, ((LPPOINT)&rt) + 1);
        mswin_update_window_placement(NHW_MAP, &rt);
    } break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

/* on WM_COMMAND */
void
onMSNHCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    PNHMapWindow data;

    data = (PNHMapWindow) GetWindowLongPtr(hWnd, GWLP_USERDATA);
    switch (wParam) {
    case MSNH_MSG_PRINT_GLYPH: {
        PMSNHMsgPrintGlyph msg_data = (PMSNHMsgPrintGlyph) lParam;
        setGlyph(data, msg_data->x, msg_data->y, msg_data->layers);
    } break;

    case MSNH_MSG_CLIPAROUND: {
        PMSNHMsgClipAround msg_data = (PMSNHMsgClipAround) lParam;
        int x, y;
        BOOL scroll_x, scroll_y;
        int mcam = iflags.wc_scroll_margin;

        /* calculate if you should clip around */
        scroll_x =
            !GetNHApp()->bNoHScroll
            && (msg_data->x < (data->xPos + mcam)
                || msg_data->x > (data->xPos + data->xPageSize - mcam));
        scroll_y =
            !GetNHApp()->bNoVScroll
            && (msg_data->y < (data->yPos + mcam)
                || msg_data->y > (data->yPos + data->yPageSize - mcam));

        mcam += iflags.wc_scroll_amount - 1;
        /* get page size and center horizontally on x-position */
        if (scroll_x) {
            if (data->xPageSize <= 2 * mcam) {
                x = max(0, min(COLNO, msg_data->x - data->xPageSize / 2));
            } else if (msg_data->x < data->xPos + data->xPageSize / 2) {
                x = max(0, min(COLNO, msg_data->x - mcam));
            } else {
                x = max(0, min(COLNO, msg_data->x - data->xPageSize + mcam));
            }
            SendMessage(hWnd, WM_HSCROLL, (WPARAM) MAKELONG(SB_THUMBTRACK, x),
                        (LPARAM) NULL);
        }

        /* get page size and center vertically on y-position */
        if (scroll_y) {
            if (data->yPageSize <= 2 * mcam) {
                y = max(0, min(ROWNO, msg_data->y - data->yPageSize / 2));
            } else if (msg_data->y < data->yPos + data->yPageSize / 2) {
                y = max(0, min(ROWNO, msg_data->y - mcam));
            } else {
                y = max(0, min(ROWNO, msg_data->y - data->yPageSize + mcam));
            }
            SendMessage(hWnd, WM_VSCROLL, (WPARAM) MAKELONG(SB_THUMBTRACK, y),
                        (LPARAM) NULL);
        }
    } break;

    case MSNH_MSG_CLEAR_WINDOW:
        clearAll(data);
        break;

    case MSNH_MSG_CURSOR: {
        PMSNHMsgCursor msg_data = (PMSNHMsgCursor) lParam;

        if (flags.force_paint_at_cursor || data->xCur != msg_data->x || data->yCur != msg_data->y) 
        {
            if (flags.force_paint_at_cursor)
            {
                flags.force_paint_at_cursor = FALSE;
                //data->cursorOn = TRUE;
            }

            dirty(data, data->xCur, data->yCur);
            dirty(data, msg_data->x, msg_data->y);

            data->xCur = msg_data->x;
            data->yCur = msg_data->y;
        }
    } break;

    case MSNH_MSG_GETTEXT: {
        PMSNHMsgGetText msg_data = (PMSNHMsgGetText) lParam;
        size_t index;
        int col, row;
        int color;
        unsigned long special;
        int mgch;

        index = 0;
        for (row = 0; row < ROWNO; row++) {
            for (col = 0; col < COLNO; col++) {
                if (index >= msg_data->max_size)
                    break;
                if (data->map[col][row].glyph == NO_GLYPH) {
                    mgch = ' ';
                } else {
                    (void) mapglyph(data->map[col][row].glyph, &mgch, &color,
                                    &special, col, row);
                }
                msg_data->buffer[index] = mgch;
                index++;
            }
            if (index >= msg_data->max_size - 1)
                break;
            msg_data->buffer[index++] = '\r';
            msg_data->buffer[index++] = '\n';
        }
    } break;

	case MSNH_MSG_RANDOM_INPUT:
		nhassert(0); // unexpected
		break;

    case MSNH_MSG_STRETCH_MAP:
    {
        PMSNHMsgClipAround msg_data = (PMSNHMsgClipAround)lParam;
        SIZE size;
        size.cx = msg_data->x * COLNO;
        size.cy = msg_data->y * ROWNO;
        mswin_map_stretch(hWnd, &size, TRUE);
        break;
    }
    } /* end switch(wParam) */
}

/* on WM_CREATE */
void
onCreate(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    PNHMapWindow data;

    UNREFERENCED_PARAMETER(wParam);
    UNREFERENCED_PARAMETER(lParam);

    /* set window data */
    data = (PNHMapWindow) malloc(sizeof(NHMapWindow));
	if (!data)
	{
		panic("out of memory");
		return;
	}
    ZeroMemory(data, sizeof(NHMapWindow));

    data->hWnd = hWnd;

    data->bAsciiMode = FALSE;
    data->cursorOn = TRUE;
    data->interval_counter = 0;

    data->xFrontTile = GetNHApp()->mapTile_X;
    data->yFrontTile = GetNHApp()->mapTile_Y;
    data->tileWidth = GetNHApp()->mapTile_X;
    data->tileHeight = GetNHApp()->mapTile_Y;

    HDC hDC = GetDC(hWnd);
    data->backBufferDC = CreateCompatibleDC(hDC);
    data->tileDC = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);

    SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR) data);

    clearAll(data);

}

static void
paintTile(PNHMapWindow data, int i, int j, RECT * rect)
{
    short ntile;
    int t_x, t_y;
    int glyph, signed_glyph;
#ifdef USE_PILEMARK
    //int color;
    //unsigned long special;
    //int mgch;
#endif

    boolean flip_glyph = FALSE;
    data->mapAnimated[i][j] = FALSE;

/*
    if (glyph == NO_GLYPH && bkglyph == NO_GLYPH) {
        HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
        FillRect(data->backBufferDC, rect, blackBrush);
        DeleteObject(blackBrush);
    }
*/
    int enl_i = -1, enl_j = -1;
    boolean enlarged = FALSE;
    short frame_index = 0;

    /* running index and enlarg_index form an u around the main tile X = -1:
     * 0 X 1
     * 2 3 4
    */
    int layer = 0;
    boolean opaque_background_drawn = FALSE;
    for (int base_layer = 0; base_layer < 3; base_layer++)
    {
        if (base_layer == 0)
        {
            layer = 0;
            if (0) /* If cmap is transparent */
            {
                /* Draw background for transparent cmaps */
                opaque_background_drawn = TRUE;
            }
        }
        else if(base_layer >= 1 && base_layer <= 2)
        {
            for (int running_index = 0; running_index < MAX_FRAMES_PER_ENLARGEMENT + 1; running_index++)
            {
                /* Drawing order from back to front */
                int z_order_array[MAX_FRAMES_PER_ENLARGEMENT + 1] = { 0, 1, -1, 2, 3, 4 };
                int enlarg_idx = z_order_array[running_index];
                layer = base_layer * (MAX_FRAMES_PER_ENLARGEMENT + 1) + enlarg_idx + 1;

                /* Set coordinates */
                if (enlarg_idx == -1)
                {
                    enl_i = i;
                    enl_j = j;
                }
                else if (enlarg_idx == 0)
                {
                    enl_i = i - 1;
                    enl_j = j;
                }
                else if (enlarg_idx == 1)
                {
                    enl_i = i + 1;
                    enl_j = j;
                }
                else if (enlarg_idx == 2)
                {
                    enl_i = i - 1;
                    enl_j = j + 1;
                }
                else if (enlarg_idx == 3)
                {
                    enl_i = i;
                    enl_j = j + 1;
                }
                else if (enlarg_idx == 4)
                {
                    enl_i = i + 1;
                    enl_j = j + 1;
                }

                if (enlarg_idx >= 0)
                {
                    if (!isok(enl_i, enl_j))
                        continue;

                    int relevant_i = i;
                    int relevant_j = enl_j;
                    boolean side_not_ok = FALSE;
                    if (IS_ROCK(level.locations[relevant_i][relevant_j].typ)
                        || (IS_DOOR(level.locations[relevant_i][relevant_j].typ) && (level.locations[relevant_i][relevant_j].doormask & (D_CLOSED | D_LOCKED)))
                        || data->map[relevant_i][relevant_j].glyph == S_unexplored
                        || (data->map[relevant_i][relevant_j].glyph == NO_GLYPH && data->map[relevant_i][relevant_j].bkglyph == NO_GLYPH)
                        )
                        side_not_ok = TRUE;

                    boolean upper_side_not_ok = FALSE;
                    relevant_i = i;
                    relevant_j = j;
                    if (relevant_j < enl_j)
                    {
                        if (IS_ROCK(level.locations[relevant_i][relevant_j].typ)
                            || (IS_DOOR(level.locations[relevant_i][relevant_j].typ) && (level.locations[relevant_i][relevant_j].doormask & (D_CLOSED | D_LOCKED)))
                            || data->map[relevant_i][relevant_j].glyph == S_unexplored
                            || (data->map[relevant_i][relevant_j].glyph == NO_GLYPH && data->map[relevant_i][relevant_j].bkglyph == NO_GLYPH)
                            )
                            upper_side_not_ok = TRUE;
                    }
                    else
                        upper_side_not_ok = TRUE;

                    if (side_not_ok && upper_side_not_ok)
                        continue;
                }

                int signed_bk_glyph = data->map[enl_i][enl_j].bkglyph;
                int signed_main_glyph = data->map[enl_i][enl_j].glyph;

                int layer1signedglyph = signed_bk_glyph != NO_GLYPH ? signed_bk_glyph : (glyph_is_cmap(abs(signed_main_glyph)) || glyph_is_cmap_variation(abs(signed_main_glyph))) ? signed_main_glyph : NO_GLYPH;
                int layer2signedglyph = (glyph_is_cmap(abs(signed_main_glyph)) || glyph_is_cmap_variation(abs(signed_main_glyph))) ? (signed_bk_glyph != NO_GLYPH ? signed_main_glyph : NO_GLYPH) : signed_main_glyph;

                if(base_layer == 1)
                    signed_glyph = layer1signedglyph;
                else if(base_layer == 2)
                    signed_glyph = layer2signedglyph;

                glyph = abs(signed_glyph);


                /* Kludge for the time being */
                if (base_layer == 1 && glyph == NO_GLYPH && data->map[enl_i][enl_j].glyph == NO_GLYPH)
                    glyph = cmap_to_glyph(S_unexplored);

                if (signed_glyph < 0)
                    flip_glyph = TRUE;
                else
                    flip_glyph = FALSE;

                /* Set frame_index */
                if (enlarg_idx == -1)
                {
                    frame_index = -1;
                }
                else if (enlarg_idx == 0)
                {
                    frame_index = flip_glyph ? 3 : 4;
                }
                else if (enlarg_idx == 1)
                {
                    frame_index = flip_glyph ? 4 : 3;
                }
                else if (enlarg_idx == 2)
                {
                    frame_index = flip_glyph ? 0 : 2;
                }
                else if (enlarg_idx == 3)
                {
                    frame_index = 1;
                }
                else if (enlarg_idx == 4)
                {
                    frame_index = flip_glyph ? 2 : 0;
                }

#if 0
                if (bkglyph != NO_GLYPH) {
                    boolean skip_drawing = FALSE;
                    ntile = maybe_get_animated_tile(glyph2tile[bkglyph], data->interval_counter, &data->mapAnimated[i][j]);
                    if (enlarg_idx >= 0)
                    {
                        if (tile2enlargement[ntile] > 0)
                        {
                            int enl_tile_idx = enlargements[tile2enlargement[ntile]].frame2tile[frame_index];
                            if (enl_tile_idx >= 0)
                            {
                                int enl_glyph = enl_tile_idx + enlargements[tile2enlargement[ntile]].glyph_offset + GLYPH_ENLARGEMENT_OFF;
                                ntile = glyph2tile[enl_glyph]; /* replace */
                            }
                            else
                                skip_drawing = TRUE;
                        }
                        else
                            skip_drawing = TRUE;
                    }

                    if (!skip_drawing)
                    {
                        int multiplier = flip_bkglyph ? -1 : 1;
                        t_x = TILEBMP_X(ntile) + (flip_bkglyph ? TILE_X - 1 : 0);
                        t_y = TILEBMP_Y(ntile);

                        StretchBlt(data->backBufferDC, rect->left, rect->top,
                            data->xBackTile, data->yBackTile, data->tileDC,
                            t_x, t_y, multiplier * GetNHApp()->mapTile_X,
                            GetNHApp()->mapTile_Y, SRCCOPY);
                        layer++;
                    }
                }
#endif
                //int is_you_facing_right = (u.facing_right && glyph == u_to_glyph());
                int multiplier = flip_glyph ? -1 : 1;

                if ((glyph != NO_GLYPH) /*&& (glyph != bkglyph)*/) {
                    boolean skip_drawing = FALSE;
                    boolean move_obj_to_middle = ((glyphtileflags[glyph] & GLYPH_TILE_FLAG_NORMAL_ITEM_AS_MISSILE) && !(glyphtileflags[glyph] & GLYPH_TILE_FLAG_FULL_SIZED_ITEM));
                    ntile = maybe_get_animated_tile(glyph2tile[glyph], data->interval_counter, &data->mapAnimated[i][j]);
                    if (enlarg_idx >= 0)
                    {
                        if (tile2enlargement[ntile] > 0)
                        {
                            if (enl_j == j)
                                j = j;
                            int enl_tile_idx = enlargements[tile2enlargement[ntile]].frame2tile[frame_index];
                            if (enl_tile_idx >= 0)
                            {
                                int enl_glyph = enl_tile_idx + enlargements[tile2enlargement[ntile]].glyph_offset + GLYPH_ENLARGEMENT_OFF;
                                ntile = glyph2tile[enl_glyph]; /* replace */
                            }
                            else
                                skip_drawing = TRUE;
                        }
                        else
                            skip_drawing = TRUE;
                    }

                    if (!skip_drawing)
                    {
                        int dest_top_added = 0;
                        int dest_height_deducted = 0;
                        int source_top_added = 0;
                        int source_height_deducted = 0;
                        t_x = TILEBMP_X(ntile) + (flip_glyph ? TILE_X - 1 : 0);
                        t_y = TILEBMP_Y(ntile);
                        if (move_obj_to_middle)
                        {
                            dest_top_added = (rect->bottom - rect->top) / 4;
                            dest_height_deducted = (rect->bottom - rect->top) / 2;
                            source_top_added = TILE_Y / 2;
                            source_height_deducted = TILE_Y / 2;
                        }
                        SetStretchBltMode(data->backBufferDC, COLORONCOLOR);
                        if (opaque_background_drawn)
                        {
                            (*GetNHApp()->lpfnTransparentBlt)(
                                data->backBufferDC, rect->left, rect->top + dest_top_added,
                                data->xBackTile, data->yBackTile - dest_height_deducted, data->tileDC, t_x,
                                t_y + source_top_added, multiplier * GetNHApp()->mapTile_X,
                                GetNHApp()->mapTile_Y - source_height_deducted, TILE_BK_COLOR);
                        }
                        else
                        {
                            StretchBlt(data->backBufferDC, rect->left, rect->top + dest_top_added,
                                data->xBackTile, data->yBackTile - dest_height_deducted, data->tileDC,
                                t_x, t_y + source_top_added, multiplier * GetNHApp()->mapTile_X,
                                GetNHApp()->mapTile_Y - source_height_deducted, SRCCOPY);

                            opaque_background_drawn = TRUE;
                        }

                    }
                }

                if (base_layer == 2 && enlarg_idx == -1)
                {

                    /* Draw main tile marker for enlarged creatures */
                    int enlargement_idx = tile2enlargement[ntile];
                    if (enlargement_idx > 0 && enlargements[enlargement_idx].number_of_frames > 3)
                    {
                        int mglyph = MAIN_TILE_MARK + GLYPH_UI_TILE_OFF;
                        int mtile = glyph2tile[mglyph];
                        t_x = TILEBMP_X(mtile);
                        t_y = TILEBMP_Y(mtile);

                        SetStretchBltMode(data->backBufferDC, COLORONCOLOR);
                        (*GetNHApp()->lpfnTransparentBlt)(
                            data->backBufferDC, rect->left, rect->top,
                            data->xBackTile, data->yBackTile, data->tileDC, t_x,
                            t_y, GetNHApp()->mapTile_X,
                            GetNHApp()->mapTile_Y, TILE_BK_COLOR);
                    }
//    #ifdef USE_PILEMARK
                    /* rely on GnollHack core helper routine */
                    //(void) mapglyph(data->map[i][j].glyph, &mgch, &color, &special, i, j);

//                    if ((glyph != NO_GLYPH) && (data->map[i][j].layer_flags & LFLAGS_M_PET) // (special & MG_PET)
//    #else
//                    if ((glyph != NO_GLYPH) && glyph_is_pet(glyph)
//    #endif
                        if ((glyph != NO_GLYPH) && (data->map[i][j].layer_flags & LFLAGS_M_PET) // (special & MG_PET)
                            && iflags.wc_hilite_pet) {
                        /* apply pet mark transparently over
                            pet image */
    #if 1
                        HDC hdcPetMark;
                        HBITMAP bmPetMarkOld;

                        /* this is DC for petmark bitmap */
                        hdcPetMark = CreateCompatibleDC(data->backBufferDC);
                        bmPetMarkOld =
                            SelectObject(hdcPetMark, GetNHApp()->bmpPetMark);

                        SetStretchBltMode(data->backBufferDC, COLORONCOLOR);
                        (*GetNHApp()->lpfnTransparentBlt)(
                            data->backBufferDC, rect->left, rect->top,
                            data->xBackTile, data->yBackTile, hdcPetMark, 0, 0,
                            TILE_X, TILE_Y, TILE_BK_COLOR);
                        SelectObject(hdcPetMark, bmPetMarkOld);
                        DeleteDC(hdcPetMark);
    #endif
                    }
    #ifdef USE_PILEMARK
                    if ((glyph != NO_GLYPH) && (data->map[i][j].layer_flags & LFLAGS_O_PILE) //(special & MG_OBJPILE)
                        && iflags.hilite_pile) {
                        /* apply pilemark transparently over other image */
                        HDC hdcPileMark;
                        HBITMAP bmPileMarkOld;

                        /* this is DC for pilemark bitmap */
                        hdcPileMark = CreateCompatibleDC(data->backBufferDC);
                        bmPileMarkOld = SelectObject(hdcPileMark,
                            GetNHApp()->bmpPileMark);

                        SetStretchBltMode(data->backBufferDC, COLORONCOLOR);
                        (*GetNHApp()->lpfnTransparentBlt)(
                            data->backBufferDC, rect->left, rect->top,
                            data->xBackTile, data->yBackTile, hdcPileMark, 0, 0,
                            TILE_X, TILE_Y, TILE_BK_COLOR);
                        SelectObject(hdcPileMark, bmPileMarkOld);
                        DeleteDC(hdcPileMark);
                    }
    #endif

                    /* Draw death marker */
                    if (glyph_is_dying_monster(glyph) || glyph_is_female_dying_monster(glyph) || glyph_is_dying_player(glyph))
                    {
                        int mglyph = DEATH_TILE + GLYPH_UI_TILE_OFF;
                        int mtile = glyph2tile[mglyph];
                        t_x = TILEBMP_X(mtile);
                        t_y = TILEBMP_Y(mtile);

                        SetStretchBltMode(data->backBufferDC, COLORONCOLOR);
                        (*GetNHApp()->lpfnTransparentBlt)(
                            data->backBufferDC, rect->left, rect->top,
                            data->xBackTile, data->yBackTile, data->tileDC, t_x,
                            t_y, GetNHApp()->mapTile_X,
                            GetNHApp()->mapTile_Y, TILE_BK_COLOR);
                    }


                    if (i == data->xCur && j == data->yCur)
                    {
                        if (
                            (!data->cursorOn && flags.blinking_cursor_on_tiles)
                            || (/*i == u.ux && j == u.uy &&*/ !flags.show_cursor_on_u)
                            )
                        {
                            // Nothing, cursor is invisible
                        }
                        else
                        {
                            HBRUSH hbr_dark = CreateSolidBrush(RGB(0, 0, 0));
                            HBRUSH hbr_light = CreateSolidBrush(RGB(100, 50, 0));
                            HBRUSH hbr_light2 = CreateSolidBrush(RGB(50, 25, 0));
                            RECT smaller_rect, even_smaller_rect;
                            smaller_rect.top = rect->top + 1;
                            smaller_rect.bottom = rect->bottom - 1;
                            smaller_rect.left = rect->left + 1;
                            smaller_rect.right = rect->right - 1;
                            even_smaller_rect.top = rect->top + 2;
                            even_smaller_rect.bottom = rect->bottom - 2;
                            even_smaller_rect.left = rect->left + 2;
                            even_smaller_rect.right = rect->right - 2;
                            FrameRect(data->backBufferDC, rect, hbr_dark);
                            FrameRect(data->backBufferDC, &smaller_rect, hbr_light);
                            FrameRect(data->backBufferDC, &even_smaller_rect, hbr_light2);
                            //DrawFocusRect(data->backBufferDC, rect);
                            //DrawFocusRect(data->backBufferDC, &smaller_rect);
                        }
                    }
                }
            }
        }
    }
}


static void
paintGlyph(PNHMapWindow data, int i, int j, RECT * rect)
{
    if (data->map[i][j].glyph >= 0) {

        char ch;
        WCHAR wch;
        int color;
        unsigned long special;
        int mgch;
        HBRUSH back_brush;
        COLORREF OldFg;

        SetBkMode(data->backBufferDC, TRANSPARENT);

        HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
        FillRect(data->backBufferDC, rect, blackBrush);
        DeleteObject(blackBrush);

    #if (VERSION_MAJOR < 0) && (VERSION_MINOR < 4) && (PATCHLEVEL < 2)
        //nhglyph2charcolor(data->map[i][j].glyph, &ch, &color);
        //OldFg = SetTextColor(hDC, nhcolor_to_RGB(color));
    #else
        /* rely on GnollHack core helper routine */
        (void) mapglyph(data->map[i][j].glyph, &mgch, &color,
                        &special, i, j);
        ch = (char) mgch;
        if (((special & MG_PET) && iflags.hilite_pet)
            || ((special & (MG_DETECT | MG_BW_LAVA))
                && iflags.use_inverse)) {
            back_brush =
                CreateSolidBrush(nhcolor_to_RGB(CLR_GRAY));
            FillRect(data->backBufferDC, rect, back_brush);
            DeleteObject(back_brush);
            switch (color) {
            case CLR_GRAY:
            case CLR_WHITE:
                OldFg = SetTextColor(
                    data->backBufferDC, nhcolor_to_RGB(CLR_BLACK));
                break;
            default:
                OldFg =
                    SetTextColor(data->backBufferDC, nhcolor_to_RGB(color));
            }
        } else {
            OldFg = SetTextColor(data->backBufferDC, nhcolor_to_RGB(color));
        }
    #endif
        if (data->bUnicodeFont) {
            wch = winos_ascii_to_wide(ch);
            if (wch == 0x2591 || wch == 0x2592) {
                int level = 80;
                HBRUSH brush = CreateSolidBrush(RGB(level, level, level));
                FillRect(data->backBufferDC, rect, brush);
                DeleteObject(brush);
                level = (wch == 0x2591 ? 100 : 200);
                brush = CreateSolidBrush(RGB(level, level, level));
                RECT smallRect = { rect->left + 1, rect->top + 1,
                                    rect->right - 1, rect->bottom - 1 };
                FillRect(data->backBufferDC, &smallRect, brush);
                DeleteObject(brush);
            } else {
                DrawTextW(data->backBufferDC, &wch, 1, rect,
                    DT_CENTER | DT_VCENTER | DT_NOPREFIX
                    | DT_SINGLELINE);
            }
        } else {
            DrawTextA(data->backBufferDC, &ch, 1, rect,
                        DT_CENTER | DT_VCENTER | DT_NOPREFIX
                            | DT_SINGLELINE);
        }

        SetTextColor(data->backBufferDC, OldFg);
    }

    if (i == data->xCur && j == data->yCur &&
        (data->cursorOn || !win32_cursorblink )) {
        int yCursor = (win32_cursorblink ? data->yBlinkCursor :
                                           data->yNoBlinkCursor);
        PatBlt(data->backBufferDC, 
                rect->left, rect->bottom - yCursor,
                rect->right - rect->left,
                yCursor,
                DSTINVERT);
    }
}

static void setGlyph(PNHMapWindow data, int i, int j, struct layer_info layers)
{
    int fg = layers.glyph;
    int bg = layers.bkglyph;

    if ((data->map[i][j].glyph != layers.glyph) || (data->map[i][j].glyph != layers.bkglyph)) {
        dirty(data, i, j);

        data->map[i][j] = layers;

        dirty(data, i, j);
        /*
        data->mapDirty[i][j] = TRUE;

        RECT rect;
        nhcoord2display(data, i, j, &rect);
        InvalidateRect(data->hWnd, &rect, FALSE);
        */
    }
}

static void clearAll(PNHMapWindow data)
{
    for (int x = 0; x < COLNO; x++)
        for (int y = 0; y < ROWNO; y++) {
            data->map[x][y].glyph = NO_GLYPH;
            data->map[x][y].bkglyph = NO_GLYPH;
            for(int i = 0; i < MAX_LAYERS; i++)
                data->map[x][y].layer_glyphs[i] = NO_GLYPH;
            data->map[x][y].layer_flags = 0UL;
            data->map[x][y].object_data = zeroobj;
            data->map[x][y].monster_data = zeromonst;
            data->map[x][y].damage_displayed = 0;
            data->mapDirty[x][y] = TRUE;
            data->mapAnimated[x][y] = 0;
        }
    InvalidateRect(data->hWnd, NULL, FALSE);
}

static void dirtyAll(PNHMapWindow data)
{
    for (int i = 0; i < COLNO; i++)
        for (int j = 0; j < ROWNO; j++)
            data->mapDirty[i][j] = TRUE;

    InvalidateRect(data->hWnd, NULL, FALSE);
}

static void dirty(PNHMapWindow data, int x, int y)
{
    data->mapDirty[x][y] = TRUE;

    RECT rt;
    nhcoord2display(data, x, y, &rt); //data->xCur, data->yCur

    InvalidateRect(data->hWnd, &rt, FALSE);

    int enlarg = tile2enlargement[glyph2tile[abs(data->map[x][y].glyph)]];
    int bk_enlarg = tile2enlargement[glyph2tile[abs(data->map[x][y].bkglyph)]];
    if (enlarg > 0 || bk_enlarg > 0)
    {
        int enl_x = -1;
        int enl_y = -1;
        for (int i = 0; i < MAX_FRAMES_PER_ENLARGEMENT; i++)
        {
            if (enlargements[enlarg].frame2tile[i] == -1 && enlargements[bk_enlarg].frame2tile[i] == -1)
                continue;

            if (i <= 2)
                enl_x = x + i - 1;
            else if (i == 3)
                enl_x = x - 1;
            else if (i == 4)
                enl_x = x + 1;

            if (i <= 2)
                enl_y = y - 1;
            else
                enl_y = y;

            if (isok(enl_x, enl_y))
            {
                data->mapDirty[enl_x][enl_y] = TRUE;
                nhcoord2display(data, enl_x, enl_y, &rt);
                InvalidateRect(data->hWnd, &rt, FALSE);
            }
        }
    }
}

static void
paint(PNHMapWindow data, int i, int j)
{
    RECT rect;

    rect.left = i * data->xBackTile;
    rect.top = j * data->yBackTile;
    rect.right = rect.left + data->xBackTile;
    rect.bottom = rect.top + data->yBackTile;

    if (data->bAsciiMode || Is_rogue_level(&u.uz)) {
        paintGlyph(data, i, j, &rect);
    } else {
        paintTile(data, i, j, &rect);
    }

    data->mapDirty[i][j] = FALSE;
}


/* on WM_PAINT */
void
onPaint(HWND hWnd)
{
    PNHMapWindow data = (PNHMapWindow) GetWindowLongPtr(hWnd, GWLP_USERDATA);

    /* update back buffer */
    HBITMAP savedBitmap = SelectObject(data->tileDC, GetNHApp()->bmpMapTiles);

    for (int i = 0; i < COLNO; i++)
        for (int j = 0; j < ROWNO; j++)
            if (data->mapDirty[i][j])
                paint(data, i, j);

    SelectObject(data->tileDC, savedBitmap);

    PAINTSTRUCT ps;
    HDC hFrontBufferDC = BeginPaint(hWnd, &ps);

    /* stretch back buffer onto front buffer window */
    int frontWidth = COLNO * data->xFrontTile;
    int frontHeight = ROWNO * data->yFrontTile;

    SetStretchBltMode(hFrontBufferDC, COLORONCOLOR);
    StretchBlt(hFrontBufferDC,
        data->map_orig.x - (data->xPos * data->xFrontTile),
        data->map_orig.y - (data->yPos * data->yFrontTile), frontWidth, frontHeight,
                data->backBufferDC, 0, 0, data->backWidth, data->backHeight, SRCCOPY);

    EndPaint(hWnd, &ps);
}

/* on WM_VSCROLL */
void
onMSNH_VScroll(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    PNHMapWindow data;
    SCROLLINFO si;
    int yNewPos;
    int yDelta;

    UNREFERENCED_PARAMETER(lParam);

    /* get window data */
    data = (PNHMapWindow) GetWindowLongPtr(hWnd, GWLP_USERDATA);

    switch (LOWORD(wParam)) {
    /* User clicked shaft left of the scroll box. */
    case SB_PAGEUP:
        yNewPos = data->yPos - data->yPageSize;
        break;

    /* User clicked shaft right of the scroll box. */
    case SB_PAGEDOWN:
        yNewPos = data->yPos + data->yPageSize;
        break;

    /* User clicked the left arrow. */
    case SB_LINEUP:
        yNewPos = data->yPos - 1;
        break;

    /* User clicked the right arrow. */
    case SB_LINEDOWN:
        yNewPos = data->yPos + 1;
        break;

    /* User dragged the scroll box. */
    case SB_THUMBTRACK:
        yNewPos = HIWORD(wParam);
        break;

    default:
        yNewPos = data->yPos;
    }

    yNewPos = max(0, min(data->yMax, yNewPos));
    if (yNewPos == data->yPos)
        return;

    yDelta = yNewPos - data->yPos;
    data->yPos = yNewPos;

    ScrollWindowEx(hWnd, 0, -data->yFrontTile * yDelta, (CONST RECT *) NULL,
                   (CONST RECT *) NULL, (HRGN) NULL, (LPRECT) NULL,
                   SW_INVALIDATE | SW_ERASE);

    si.cbSize = sizeof(si);
    si.fMask = SIF_POS;
    si.nPos = data->yPos;
    SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
}

/* on WM_HSCROLL */
void
onMSNH_HScroll(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    PNHMapWindow data;
    SCROLLINFO si;
    int xNewPos;
    int xDelta;

    UNREFERENCED_PARAMETER(lParam);

    /* get window data */
    data = (PNHMapWindow) GetWindowLongPtr(hWnd, GWLP_USERDATA);

    switch (LOWORD(wParam)) {
    /* User clicked shaft left of the scroll box. */
    case SB_PAGEUP:
        xNewPos = data->xPos - data->xPageSize;
        break;

    /* User clicked shaft right of the scroll box. */
    case SB_PAGEDOWN:
        xNewPos = data->xPos + data->xPageSize;
        break;

    /* User clicked the left arrow. */
    case SB_LINEUP:
        xNewPos = data->xPos - 1;
        break;

    /* User clicked the right arrow. */
    case SB_LINEDOWN:
        xNewPos = data->xPos + 1;
        break;

    /* User dragged the scroll box. */
    case SB_THUMBTRACK:
        xNewPos = HIWORD(wParam);
        break;

    default:
        xNewPos = data->xPos;
    }

    xNewPos = max(0, min(data->xMax, xNewPos));
    if (xNewPos == data->xPos)
        return;

    xDelta = xNewPos - data->xPos;
    data->xPos = xNewPos;

    ScrollWindowEx(hWnd, -data->xFrontTile * xDelta, 0, (CONST RECT *) NULL,
                   (CONST RECT *) NULL, (HRGN) NULL, (LPRECT) NULL,
                   SW_INVALIDATE | SW_ERASE);

    si.cbSize = sizeof(si);
    si.fMask = SIF_POS;
    si.nPos = data->xPos;
    SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);
}

/* map GnollHack map coordinates to the screen location */
void
nhcoord2display(PNHMapWindow data, int x, int y, LPRECT lpOut)
{
    lpOut->left = (x - data->xPos) * data->xFrontTile + data->map_orig.x;
    lpOut->top = (y - data->yPos) * data->yFrontTile + data->map_orig.y;
    lpOut->right = lpOut->left + data->xFrontTile;
    lpOut->bottom = lpOut->top + data->yFrontTile;
}

#if (VERSION_MAJOR < 0) && (VERSION_MINOR < 4) && (PATCHLEVEL < 2)
/* map glyph to character/color combination */
void
nhglyph2charcolor(short g, uchar *ch, int *color)
{
    int offset;
#ifdef TEXTCOLOR

#define zap_color(n) *color = iflags.use_color ? zapcolors[n] : NO_COLOR
#define cmap_color(n,cmap_idx) *color = iflags.use_color ? defsyms[n].color[cmap_idx] : NO_COLOR
#define obj_color(n) \
    *color = iflags.use_color ? objects[n].oc_color : NO_COLOR
#define mon_color(n) *color = iflags.use_color ? mons[n].mcolor : NO_COLOR
#define pet_color(n) *color = iflags.use_color ? mons[n].mcolor : NO_COLOR
#define warn_color(n) \
    *color = iflags.use_color ? def_warnsyms[n].color : NO_COLOR

#else /* no text color */

#define zap_color(n)
#define cmap_color(n,cmap_idx)
#define obj_color(n)
#define mon_color(n)
#define pet_color(c)
#define warn_color(c)
    *color = CLR_WHITE;
#endif

    if ((offset = (g - GLYPH_WARNING_OFF)) >= 0) { /* a warning flash */
        *ch = showsyms[offset + SYM_OFF_W];
        warn_color(offset);
    } else if ((offset = (g - GLYPH_SWALLOW_OFF)) >= 0) { /* swallow */
        /* see swallow_to_glyph() in display.c */
        *ch = (uchar) showsyms[(S_sw_tl + (offset & 0x7)) + SYM_OFF_P];
        mon_color(offset >> 3);
    } else if ((offset = (g - GLYPH_ZAP_OFF)) >= 0) { /* zap beam */
        /* see zapdir_to_glyph() in display.c */
        *ch = showsyms[(S_vbeam + (offset & 0x3)) + SYM_OFF_P];
        zap_color((offset >> 2));
    } else if ((offset = (g - GLYPH_CMAP_OFF)) >= 0) { /* cmap */
        int cmap_type_idx = offset / CMAP_TYPE_CHAR_NUM;
        int idx = offset - cmap_type_idx * CMAP_TYPE_CHAR_NUM + SYM_OFF_P;
        *ch = showsyms[idx];
        cmap_color(offset, cmap_type_idx);
    } else if ((offset = (g - GLYPH_OBJ_MISSILE_OFF)) >= 0) { /* object */
        *ch = showsyms[(int) objects[offset].oc_class + SYM_OFF_O];
        obj_color(offset);
    } else if ((offset = (g - GLYPH_OBJ_INVENTORY_LIT_OFF)) >= 0) { /* object */
        *ch = showsyms[(int) objects[offset].oc_class + SYM_OFF_O];
        obj_color(offset);
    } else if ((offset = (g - GLYPH_OBJ_LIT_OFF)) >= 0) { /* object */
        *ch = showsyms[(int) objects[offset].oc_class + SYM_OFF_O];
        obj_color(offset);
    } else if ((offset = (g - GLYPH_OBJ_INVENTORY_OFF)) >= 0) { /* object */
        *ch = showsyms[(int) objects[offset].oc_class + SYM_OFF_O];
        obj_color(offset);
    } else if ((offset = (g - GLYPH_OBJ_OFF)) >= 0) { /* object */
        *ch = showsyms[(int) objects[offset].oc_class + SYM_OFF_O];
        obj_color(offset);
    } else if ((offset = (g - GLYPH_FEMALE_BODY_OFF)) >= 0) { /* a corpse */
        *ch = showsyms[(int) objects[CORPSE].oc_class + SYM_OFF_O];
        mon_color(offset);
    } else if ((offset = (g - GLYPH_FEMALE_PET_OFF)) >= 0) { /* a pet */
        *ch = showsyms[(int) mons[offset].mlet + SYM_OFF_M];
        pet_color(offset);
    } else if ((offset = (g - GLYPH_BODY_OFF)) >= 0) { /* a corpse */
        *ch = showsyms[(int) objects[CORPSE].oc_class + SYM_OFF_O];
        mon_color(offset);
    } else if ((offset = (g - GLYPH_PET_OFF)) >= 0) { /* a pet */
        *ch = showsyms[(int) mons[offset].mlet + SYM_OFF_M];
        pet_color(offset);
    } else { /* a monster */
        *ch = showsyms[(int) mons[g].mlet + SYM_OFF_M];
        mon_color(g);
    }
    // end of wintty code
}
#endif

/* map GnollHack color to RGB */
COLORREF
nhcolor_to_RGB(int c)
{
    if (c >= 0 && c < CLR_MAX)
        return GetNHApp()->regMapColors[c];
    return RGB(0x00, 0x00, 0x00);
}
