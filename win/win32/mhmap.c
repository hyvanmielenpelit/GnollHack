/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2022-06-05 */

/* GnollHack 4.0    mhmap.c    $NHDT-Date: 1435002695 2015/06/22 19:51:35 $  $NHDT-Branch: master $:$NHDT-Revision: 1.56 $ */
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
#include "general.h"
#include <math.h>


#define NHMAP_FONT_NAME TEXT("Terminal")
#define NHMAP_TTFONT_NAME TEXT("Consolas")
#define MAXWINDOWTEXT 255

#define CURSOR_BLINK_IN_INTERVALS 25
#define CURSOR_HEIGHT 2 // pixels

/* Layers floor, leash, and zap do not have enlargements; monster and monster effect layers have 2 'tile movement' draw order slots */
#define DRAW_ORDER_SIZE ((NUM_POSITIONS_IN_ENLARGEMENT + 1) * (MAX_LAYERS - 3 + 2 * 2) + 3 + NUM_ZAP_SOURCE_DIRS + NUM_WORM_SOURCE_DIRS + NUM_CHAIN_SOURCE_DIRS)

#define IDX_LAYER_MONSTER_SHADOW LAYER_ENVIRONMENT

/* draw order definition */
struct draw_order_definition {
    enum layer_types layer;
    int enlargement_index;
    int tile_movement_index;
    int source_dir_index;
    uchar draw_to_buffer;
    uchar draw_monster_shadow;
};


/* map window data */
typedef struct mswin_GnollHack_map_window {
    HWND hWnd;                  /* window */

    struct layer_info map[COLNO][ROWNO];    /* glyph map */
    char mapAnimated[COLNO][ROWNO];      /* animation flag for map */
    boolean mapDirty[COLNO][ROWNO];         /* dirty flag for map */
    short printedObjectTile[COLNO][ROWNO][MAX_SHOWN_OBJECTS];       /* To check if there are any enlargements */
    short printedCoverObjectTile[COLNO][ROWNO][MAX_SHOWN_OBJECTS];  /* To check if there are any enlargements */

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
    boolean bUnicodeFont;       /* map font supports unicode page 437 */

    HFONT hBuffFont;             /* font for buffs */
    boolean bUnicodeBuffFont;    /* buff font supports unicode page 437 */

    int tileWidth;              /* width of tile in pixels at 96 dpi */
    int tileHeight;             /* height of tile in pixels at 96 dpi */
    double backScale;           /* scaling from source to back buffer */
    double frontScale;          /* scaling from back to front */
    double monitorScale;        /* from 96dpi to monitor dpi*/

    boolean cursorOn;
    int yNoBlinkCursor;         /* non-blinking cursor height inback buffer
                                   in pixels */
    int yBlinkCursor;           /* blinking cursor height inback buffer
                                   in pixels */

    int backWidth;              /* back buffer width */
    int backHeight;             /* back buffer height */
    HBITMAP hBackBuffer;        /* back buffer bitmap */
    HDC backBufferDC;           /* back buffer drawing context */

    HDC tileDC[MAX_TILE_SHEETS];                /* tile drawing context */

    void* backBufferRenderTarget;
    void* tileRenderTarget;

    struct draw_order_definition draw_order[DRAW_ORDER_SIZE];

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
static void dirty(PNHMapWindow data, int i, int j, boolean usePrinted);
static void setGlyph(PNHMapWindow data, int i, int j, struct layer_info layers);
static void clearAll(PNHMapWindow data);
static void setDrawOrder(PNHMapWindow data);

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
    mswin_set_window_timer(hWnd, flags.animation_frame_interval_in_milliseconds <= 0 ? ANIMATION_FRAME_INTERVAL : flags.animation_frame_interval_in_milliseconds);

    return hWnd;
}


void
mswin_set_window_timer(HWND hWnd, UINT interval)
{
    SetTimer(hWnd, 0, interval, NULL);
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
                    (u.uz.dlevel != 0 && Is_really_rogue_level(&u.uz));

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

    //if (1) // Always true
    {
        LOGFONT lgfnt;
        LOGFONT lgbufffnt;

        ZeroMemory(&lgfnt, sizeof(lgfnt));
        ZeroMemory(&lgbufffnt, sizeof(lgbufffnt));
        if (bText)
        {
            if (data->bFitToScreenMode) {
                lgfnt.lfHeight = -data->yBackTile;     // height of font
                lgfnt.lfWidth = 0;                     // average character width
            }
            else {
                lgfnt.lfHeight = -data->yBackTile;     // height of font
                lgfnt.lfWidth = -data->xBackTile;      // average character width
            }
        }
        else
        {
            lgfnt.lfHeight = -(data->yBackTile / 6);     // height of font
            lgfnt.lfWidth = -(data->xBackTile / 6);      // average character width
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

        lgbufffnt = lgfnt;
        if (!bText)
        {
            lgbufffnt.lfHeight = -(BUFF_TEXT_HEIGHT);     // height of font
            lgbufffnt.lfWidth = -(BUFF_TEXT_WIDTH);      // average character width
        }

        TEXTMETRIC textMetrics;
        HFONT font = NULL;
        HFONT bufffont = NULL;

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

        /* Just set buff font */
        if (bufffont != NULL)
            DeleteObject(bufffont);

        bufffont = CreateFontIndirect(&lgbufffnt);

        if (data->hMapFont)
            DeleteObject(data->hMapFont);

        data->hMapFont = font;
        data->bUnicodeFont = winos_font_support_cp437(data->hMapFont);

        if (data->hBuffFont)
            DeleteObject(data->hBuffFont);

        data->hBuffFont = bufffont;
        data->bUnicodeBuffFont = winos_font_support_cp437(data->hBuffFont);

        // set tile size to match font metrics
        if (bText)
        {
            data->xBackTile = textMetrics.tmAveCharWidth;
            data->yBackTile = textMetrics.tmHeight;
        }

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

    if (wnd_size.cx > 0 && wnd_size.cy > 0 && !bText && (data->bFitToScreenMode || flags.screen_scale_adjustment == -1.0)) 
    {
        double windowAspectRatio =
            (double) wnd_size.cx / (double) wnd_size.cy;

        double backAspectRatio = 
            (double) data->backWidth / (double) data->backHeight;

        if (windowAspectRatio > backAspectRatio)
            data->frontScale = (double) wnd_size.cy / (double) data->backHeight;
        else
            data->frontScale = (double) wnd_size.cx / (double) data->backWidth;

        if (flags.screen_scale_adjustment == -1.0)
            flags.screen_scale_adjustment = data->frontScale - 1.0;
    }
    else 
    {
        if (bText) 
        {
            data->frontScale = 1.0;
        }
        else 
        {
            data->frontScale = data->monitorScale / 1.25 * (1.0 + flags.screen_scale_adjustment);
        }

    }

    data->xFrontTile = (int) ((double) data->xBackTile * data->frontScale);
    data->yFrontTile = (int) ((double) data->yBackTile * data->frontScale);

    /* calcuate ASCII cursor height */
    data->yBlinkCursor = (int) ((double) CURSOR_HEIGHT * data->backScale);
    data->yNoBlinkCursor = data->yBackTile;

    /* set map origin point */
    data->map_orig.x =
        max(0, client_rt.left + (wnd_size.cx - data->xFrontTile * (COLNO - 1)) / 2);
    data->map_orig.y =
        max(0, client_rt.top + (wnd_size.cy - data->yFrontTile * ROWNO) / 2);

    data->map_orig.x -= data->map_orig.x % data->xFrontTile;
    data->map_orig.y -= data->map_orig.y % data->yFrontTile;

    // Set horizontal scroll

    data->xPageSize = min((COLNO - 1), wnd_size.cx / data->xFrontTile);

    GetNHApp()->bNoHScroll = (data->xPageSize == (COLNO - 1));

    data->xMin = 0;
    data->xMax = (COLNO - 1) - data->xPageSize;
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

    mswin_cliparound(data->xCur, data->yCur, TRUE);

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
    iflags.using_gui_tiles = FALSE;

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
        iflags.using_gui_tiles = TRUE;
        break;

    case MAP_MODE_TILES:
    default:
        data->bAsciiMode = FALSE;
        data->bFitToScreenMode = FALSE;
        data->tileWidth = GetNHApp()->mapTile_X;
        data->tileHeight = GetNHApp()->mapTile_Y;
        iflags.using_gui_tiles = TRUE;
        break;
    }

    mapSize.cx = data->tileWidth * (COLNO - 1);
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
            size.cx = data->xFrontTile * (COLNO - 1);
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
            size.cx = data->xFrontTile * (COLNO - 1);
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
                   max(1, min((COLNO - 1), data->xPos + 1
                                         + (LOWORD(lParam) - data->map_orig.x)
                                               / data->xFrontTile)),
                   max(0, min(ROWNO, data->yPos
                                         + (HIWORD(lParam) - data->map_orig.y)
                                               / data->yFrontTile)));
        return 0;

    case WM_LBUTTONDBLCLK:
    case WM_RBUTTONDOWN:
        NHEVENT_MS(CLICK_2,
                   max(1, min((COLNO - 1), data->xPos + 1
                                         + (LOWORD(lParam) - data->map_orig.x)
                                               / data->xFrontTile)),
                   max(0, min(ROWNO, data->yPos
                                         + (HIWORD(lParam) - data->map_orig.y)
                                               / data->yFrontTile)));
        return 0;

    case WM_DESTROY:
        if (data->hMapFont)
            DeleteObject(data->hMapFont);
        if (data->hBuffFont)
            DeleteObject(data->hBuffFont);
        if (data->hBackBuffer)
            DeleteBitmap(data->hBackBuffer);
        if (data->backBufferDC)
            DeleteDC(data->backBufferDC);
        free(data);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR) 0);
        break;

    case WM_TIMER:
    {
        if(animation_timers.general_animation_counter == GH_LONG_MAX)
            animation_timers.general_animation_counter = 0L;
        else
            animation_timers.general_animation_counter++;

        if (animation_timers.u_action_animation_counter_on)
        {
            if (animation_timers.u_action_animation_counter == GH_LONG_MAX)
                animation_timers.u_action_animation_counter = 0L;
            else
                animation_timers.u_action_animation_counter++;
        }

        if (animation_timers.m_action_animation_counter_on)
        {
            if (animation_timers.m_action_animation_counter == GH_LONG_MAX)
                animation_timers.m_action_animation_counter = 0L;
            else
                animation_timers.m_action_animation_counter++;
        }

        if (animation_timers.explosion_animation_counter_on)
        {
            if (animation_timers.explosion_animation_counter == GH_LONG_MAX)
                animation_timers.explosion_animation_counter = 0L;
            else
                animation_timers.explosion_animation_counter++;
        }

        for (int spef_idx = 0; spef_idx < MAX_PLAYED_SPECIAL_EFFECTS; spef_idx++)
        {
            if (animation_timers.special_effect_animation_counter_on[spef_idx])
            {
                if (animation_timers.special_effect_animation_counter[spef_idx] == GH_LONG_MAX)
                    animation_timers.special_effect_animation_counter[spef_idx] = 0L;
                else
                    (animation_timers.special_effect_animation_counter[spef_idx])++;
            }
        }

        for (int zap_anim_idx = 0; zap_anim_idx < MAX_PLAYED_ZAP_ANIMATIONS; zap_anim_idx++)
        {
            if (animation_timers.zap_animation_counter_on[zap_anim_idx])
            {
                if (animation_timers.zap_animation_counter[zap_anim_idx] == GH_LONG_MAX)
                    animation_timers.zap_animation_counter[zap_anim_idx] = 0L;
                else
                    (animation_timers.zap_animation_counter[zap_anim_idx])++;
            }
        }

        /* Dirty all animated tiles, unless hanging up */
        if (!program_state.animation_hangup && context.game_started)
        {
            program_state.animation_hangup = 1;
            for (int x = 1; x < COLNO; x++)
                for (int y = 0; y < ROWNO; y++)
                {
                    if (data->mapAnimated[x][y] == 1
                        || (data->mapAnimated[x][y] > 1 && (animation_timers.general_animation_counter % ((long)data->mapAnimated[x][y])) == 0L)
                        )
                        dirty(data, x, y, FALSE);
                }
        }

        boolean asciimode = (data->bAsciiMode || Is_really_rogue_level(&u.uz));
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

            if (animation_timers.general_animation_counter == GH_LONG_MAX)
                animation_timers.general_animation_counter = 0L;
            else
                animation_timers.general_animation_counter++;

            if (flags.blinking_cursor_on_tiles && animation_timers.general_animation_counter % CURSOR_BLINK_IN_INTERVALS == 0 && !program_state.animation_hangup)
            {
                if (flags.force_paint_at_cursor)
                {
                    flags.force_paint_at_cursor = FALSE;
                    //data->cursorOn = TRUE;
                }
                else
                    data->cursorOn = !data->cursorOn;

                dirty(data, data->xCur, data->yCur, FALSE);
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
                x = max(0, min((COLNO - 1), msg_data->x - data->xPageSize / 2));
            } else if (msg_data->x < data->xPos + data->xPageSize / 2) {
                x = max(0, min((COLNO - 1), msg_data->x - mcam));
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

            dirty(data, data->xCur, data->yCur, FALSE);
            dirty(data, msg_data->x, msg_data->y, FALSE);

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
            for (col = 1; col < COLNO; col++) {
                if (index >= msg_data->max_size)
                    break;
                if (data->map[col][row].glyph == NO_GLYPH) {
                    mgch = ' ';
                } else {
                    (void) mapglyph(data->map[col][row], &mgch, &color,
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
        size.cx = msg_data->x * (COLNO - 1);
        size.cy = msg_data->y * ROWNO;
        mswin_map_stretch(hWnd, &size, TRUE);
        break;
    }
    case MSNH_MSG_UPDATE_ALL_STATUSES_ON_MAP:
    {
        /* Maybe some data here to identify monsters to be updated  */
        dirty(data, u.ux, u.uy, FALSE);
        for (struct monst* mon = fmon; mon; mon = mon->nmon)
        {
            if (canseemon(mon))
                dirty(data, mon->mx, mon->my, FALSE);
        }
        break;
    }
    case MSNH_MSG_SET_ANIMATION_TIMER:
    {
        UINT* msg_data = (UINT*)lParam;
        mswin_set_window_timer(hWnd, *msg_data);
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
    animation_timers.general_animation_counter = 0;

    data->xFrontTile = GetNHApp()->mapTile_X;
    data->yFrontTile = GetNHApp()->mapTile_Y;
    data->tileWidth = GetNHApp()->mapTile_X;
    data->tileHeight = GetNHApp()->mapTile_Y;

    HDC hDC = GetDC(hWnd);
    data->backBufferDC = CreateCompatibleDC(hDC);
    for(int i = 0; i < GetNHApp()->mapTileSheets; i++)
        data->tileDC[i] = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);

    SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR) data);

    clearAll(data);
    setDrawOrder(data);
}

static void
paintTile(PNHMapWindow data, int i, int j, RECT * rect)
{
    if (program_state.freeing_dynamic_data == 1 || program_state.exiting > 0 || in_mklev || program_state.in_bones)
        return;

    int ntile;
    int t_x, t_y;
    int glyph, signed_glyph;

    boolean hflip_glyph = FALSE;
    boolean vflip_glyph = FALSE;
    data->mapAnimated[i][j] = FALSE;

    int current_cmap = get_current_cmap_type_index();
    boolean ispet = !!(data->map[i][j].layer_flags & LFLAGS_M_PET);
    boolean ispeaceful = !!(data->map[i][j].layer_flags & LFLAGS_M_PEACEFUL);
    boolean isyou = !!(data->map[i][j].layer_flags & LFLAGS_M_YOU);
    boolean issteed = !!(data->map[i][j].layer_flags & LFLAGS_M_RIDDEN);
    boolean monster_copied = FALSE;
    boolean monster_darkened = FALSE;
    boolean draw_monster_shadow = FALSE;
    boolean monster_shadow_drawn = FALSE;

    for (int idx = 0; idx < MAX_SHOWN_OBJECTS; idx++)
    {
        data->printedObjectTile[i][j][idx] = -1;
        data->printedCoverObjectTile[i][j][idx] = -1;
    }

    int enl_i = -1, enl_j = -1;
    int darkening_i = -1, darkening_j = -1;
    boolean enlarged = FALSE;
    short position_index = 0;


    /* enlarg_index form an u around the main tile X = -1:
     * 0 X 1
     * 2 3 4
    */

    /* Adjust draw_order for floor layer */
    if (data->map[i][j].layer_glyphs[LAYER_FLOOR] == NO_GLYPH || glyph_is_specific_cmap_or_its_variation(data->map[i][j].layer_glyphs[LAYER_FLOOR], S_unexplored))
        data->draw_order[0].draw_to_buffer = 1;
    else
        data->draw_order[0].draw_to_buffer = 0;

    struct draw_order_definition* draw_order = data->draw_order; // [(NUM_POSITIONS_IN_ENLARGEMENT + 1) * (MAX_LAYERS - 1) + 1] = { 0 };

    /* Create DIB Section for piling up, darkening, and otherwise manipulating individual tiles */
    HDC hDCcopy = CreateCompatibleDC(data->backBufferDC);

    unsigned char* lpBitmapBitsCopy;
    LONG tileWidth = GetNHApp()->mapTile_X;
    LONG tileHeight = GetNHApp()->mapTile_Y;

    BITMAPINFO binfo;
    ZeroMemory(&binfo, sizeof(BITMAPINFO));
    binfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    binfo.bmiHeader.biWidth = tileWidth;
    binfo.bmiHeader.biHeight = tileHeight;
    binfo.bmiHeader.biPlanes = 1;
    binfo.bmiHeader.biBitCount = 32;

    HBITMAP newhBmp = CreateDIBSection(hDCcopy, &binfo, DIB_RGB_COLORS, (VOID**)&lpBitmapBitsCopy, NULL, 0);
    HGDIOBJ oldbmp = SelectObject(hDCcopy, newhBmp);


    /* Create DIB Section for storing darkened monster, so it can be drawn later as a shadow */
    HDC hDCmonster = CreateCompatibleDC(data->backBufferDC);

    unsigned char* lpBitmapBitsMonster;
    BITMAPINFO binfo_monster;
    ZeroMemory(&binfo_monster, sizeof(BITMAPINFO));
    binfo_monster.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    binfo_monster.bmiHeader.biWidth = tileWidth;
    binfo_monster.bmiHeader.biHeight = tileHeight;
    binfo_monster.bmiHeader.biPlanes = 1;
    binfo_monster.bmiHeader.biBitCount = 32;
    HBITMAP newhBmpMonster = CreateDIBSection(hDCmonster, &binfo_monster, DIB_RGB_COLORS, (VOID**)&lpBitmapBitsMonster, NULL, 0);
    HGDIOBJ oldBmpMonster = SelectObject(hDCmonster, newhBmpMonster);

    int pitch_monster = 4 * tileWidth; // 4 bytes per pixel but if not 32 bit, round pitch up to multiple of 4
    int idx_monster, x_monster, y_monster;
    for (x_monster = 0; x_monster < tileWidth; x_monster++)
    {
        for (y_monster = 0; y_monster < tileHeight; y_monster++)
        {
            idx_monster = y_monster * pitch_monster;
            idx_monster += x_monster * 4;

            lpBitmapBitsMonster[idx_monster + 0] = TILE_BK_COLOR_BLUE;  // blue
            lpBitmapBitsMonster[idx_monster + 1] = TILE_BK_COLOR_GREEN; // green
            lpBitmapBitsMonster[idx_monster + 2] = TILE_BK_COLOR_RED;  // red 
        }
    }

    SetStretchBltMode(data->backBufferDC, COLORONCOLOR);
    SetStretchBltMode(hDCcopy, COLORONCOLOR);
    SetStretchBltMode(hDCmonster, COLORONCOLOR);

    boolean opaque_background_drawn = FALSE;
    boolean print_first_directly_to_map = TRUE;
    double x_scaling_factor = ((double)data->xBackTile / (double)tileWidth);
    double y_scaling_factor = ((double)data->yBackTile / (double)tileHeight);

    for (int draw_index = 0; draw_index < DRAW_ORDER_SIZE; draw_index++)
    {
        boolean skip_darkening = FALSE;
        enum layer_types base_layer = draw_order[draw_index].layer; //layer_idx; // layer_array[layer_idx];
        int layer_rounds = 1;
        if (base_layer == LAYER_OBJECT || base_layer == LAYER_COVER_OBJECT)
        {
            if (u.uswallow)
                continue;

            layer_rounds = MAX_SHOWN_OBJECTS;
        }

        /* Drawing order from back to front */
        int enlarg_idx = draw_order[draw_index].enlargement_index;
        int tile_move_idx = draw_order[draw_index].tile_movement_index;
        int source_dir_idx = draw_order[draw_index].source_dir_index;

            /* Set coordinates */
            if (enlarg_idx == -1)
            {
                enl_i = darkening_i = i;
                enl_j = darkening_j = j + tile_move_idx;
            }
            else if (enlarg_idx == 0)
            {
                enl_i = i - 1;
                enl_j = j + tile_move_idx;
                darkening_i = i;
                darkening_j = j + tile_move_idx;
            }
            else if (enlarg_idx == 1)
            {
                enl_i = i + 1;
                enl_j = j + tile_move_idx;
                darkening_i = i;
                darkening_j = j + tile_move_idx;
            }
            else if (enlarg_idx == 2)
            {
                enl_i = i - 1;
                enl_j = j + 1 + tile_move_idx;
                darkening_i = i;
                darkening_j = j + 1 + tile_move_idx;
            }
            else if (enlarg_idx == 3)
            {
                enl_i = darkening_i = i;
                enl_j = darkening_j = j + 1 + tile_move_idx;
            }
            else if (enlarg_idx == 4)
            {
                enl_i = i + 1;
                enl_j = j + 1 + tile_move_idx;
                darkening_i = i;
                darkening_j = j + 1 + tile_move_idx;
            }

            if (!isok(enl_i, enl_j))
                continue;

            if (!isok(darkening_i, darkening_j))
            {
                darkening_i = enl_i;
                darkening_j = enl_j;
            }

            int monster_layer_height = (int)data->map[enl_i][enl_j].special_monster_layer_height;
            if (base_layer == LAYER_MONSTER || base_layer == LAYER_MONSTER_EFFECT)
            {
                if (monster_layer_height == 0 && tile_move_idx != 0)
                    continue;
                if (monster_layer_height < 0 && tile_move_idx == 1)
                    continue;
                if (monster_layer_height < 0 && tile_move_idx == -1 && enlarg_idx <= 1)
                    continue;
                if (monster_layer_height > 0 && tile_move_idx == -1)
                    continue;
            }

            int relevant_darkening_cmap = level.locations[darkening_i][darkening_j].use_special_tileset ? (int)level.locations[darkening_i][darkening_j].special_tileset : current_cmap;
            boolean is_enl_you = !!(data->map[enl_i][enl_j].layer_flags & LFLAGS_M_YOU);
            unsigned m_id_stored = data->map[enl_i][enl_j].m_id;
            struct monst* m_here = m_at(enl_i, enl_j);
            struct monst* mtmp = is_enl_you ? &youmonst : (m_here && m_here->m_id == m_id_stored) ? m_here : (struct monst*)0;
            struct trap* trap_here = 0;
            boolean is_worm_tail = !!(data->map[enl_i][enl_j].layer_flags & LFLAGS_M_WORM_TAIL);
            struct obj* obj_pile[MAX_SHOWN_OBJECTS] = { 0 };
            boolean show_memory_objects = !!(data->map[enl_i][enl_j].layer_flags & LFLAGS_SHOWING_MEMORY);
            boolean showing_detection = !!(data->map[enl_i][enl_j].layer_flags & LFLAGS_SHOWING_DETECTION);
            boolean objects_in_pit = !!(data->map[enl_i][enl_j].layer_flags & LFLAGS_O_IN_PIT);

            if (enlarg_idx >= 0 && enlarg_idx != 3)
            {
                if (0 && !showing_detection)
                {
                    int relevant_i = i;
                    int relevant_j = enl_j;
                    boolean side_not_ok = FALSE;
                    if ((IS_ROCK(level.locations[relevant_i][relevant_j].typ) && !IS_TREE(level.locations[relevant_i][relevant_j].typ))
                        || (IS_DOOR(level.locations[relevant_i][relevant_j].typ) && (level.locations[relevant_i][relevant_j].doormask & (D_CLOSED | D_LOCKED)))
                        || data->map[relevant_i][relevant_j].glyph == S_unexplored
                        || (data->map[relevant_i][relevant_j].glyph == NO_GLYPH && data->map[relevant_i][relevant_j].bkglyph == NO_GLYPH)
  //                      || (base_layer == LAYER_MONSTER && ((is_enl_you && u.utrap && u.utraptype == TT_PIT)
  //                      || (!is_enl_you && mtmp && mtmp->mtrapped && (trap_here = t_at(enl_i, enl_j)) != 0 && (trap_here->ttyp == PIT || trap_here->ttyp == SPIKED_PIT))))
                        )
                        side_not_ok = TRUE;

                    boolean upper_side_not_ok = FALSE;
                    relevant_i = i;
                    relevant_j = j;
                    if (relevant_j < enl_j)
                    {
                        if ((IS_ROCK(level.locations[relevant_i][relevant_j].typ) && !IS_TREE(level.locations[relevant_i][relevant_j].typ))
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
            }

            /* Object mimic handling */
            boolean has_obj_mimic = FALSE;
            struct obj mimic_obj = zeroobj;
            if ((base_layer == LAYER_OBJECT || base_layer == LAYER_COVER_OBJECT) && m_here && (M_AP_TYPE(m_here) == M_AP_OBJECT))
            {
                int sensed = (Protection_from_shape_changers || sensemon(m_here));
                if (!sensed)
                {
                    has_obj_mimic = TRUE;
                    mimic_obj.otyp = m_here->mappearance;
                    mimic_obj.corpsenm = has_mcorpsenm(m_here) ? MCORPSENM(m_here) : PM_TENGU;
                    mimic_obj.glyph = obj_to_glyph(&mimic_obj, newsym_rn2);
                }
            }
                
            int objcnt = 0;
            if (base_layer == LAYER_OBJECT || base_layer == LAYER_COVER_OBJECT)
            {
                struct obj* otmp = show_memory_objects ? level.locations[enl_i][enl_j].hero_memory_layers.memory_objchn : level.objects[enl_i][enl_j];

                /* If there is an object mimic, let it be first */
                if (has_obj_mimic && !show_memory_objects) /* The mimic has been added to memory objects */
                {
                    mimic_obj.nexthere = otmp;
                    otmp = &mimic_obj;
                }

                for (struct obj* otmp2 = otmp; otmp2; otmp2 = otmp2->nexthere)
                {
                    if(showing_detection && !(otmp2->speflags & SPEFLAGS_DETECTED))
                        continue;

                    obj_pile[objcnt] = otmp2;
                    objcnt++;
                    if (objcnt >= MAX_SHOWN_OBJECTS)
                        break;
                }
            }

            for (int layer_round = 0; layer_round < layer_rounds; layer_round++)
            {
                struct obj* otmp_round = (struct obj*)0;//otmp;


                if ((base_layer == LAYER_OBJECT || base_layer == LAYER_COVER_OBJECT))
                {
                    if (source_dir_idx == 0)
                    {
                        otmp_round = obj_pile[MAX_SHOWN_OBJECTS - 1 - layer_round];

                        if (!otmp_round)
                            continue; /* next round */
                    }
                }

                boolean draw_in_front = (otmp_round && is_obj_drawn_in_front(otmp_round) /* && otmp_round->ox == u.ux && otmp_round->oy == u.uy */);
                if (base_layer == LAYER_COVER_OBJECT && !draw_in_front)
                    continue; /* next round */
                if (base_layer == LAYER_OBJECT && draw_in_front)
                    continue; /* next round */

                //int signed_bk_glyph = data->map[enl_i][enl_j].bkglyph;
                //int signed_main_glyph = data->map[enl_i][enl_j].glyph;

                boolean is_corner_tile = FALSE;
                boolean manual_hflip = FALSE;
                boolean manual_vflip = FALSE;
                int adj_x = enl_i; /* should be the same as enl_i */
                int adj_y = enl_j; /* should be the same as enl_j */
                if (source_dir_idx > 0 && enlarg_idx == -1 && tile_move_idx == 0)
                {
                    int adjacent_zap_glyph = NO_GLYPH;
                    unsigned long adjacent_layer_flags = 0UL;
                    switch ((source_dir_idx - 1) % NUM_ZAP_SOURCE_BASE_DIRS + 1)
                    {
                    case 1:
                        adj_x = i - 1;
                        adj_y = j - 1;
                        break;
                    case 2:
                        adj_x = i;
                        adj_y = j - 1;
                        break;
                    case 3:
                        adj_x = i + 1;
                        adj_y = j - 1;
                        break;
                    case 4:
                        adj_x = i + 1;
                        adj_y = j;
                        break;
                    case 5:
                        adj_x = i + 1;
                        adj_y = j + 1;
                        break;
                    case 6:
                        adj_x = i;
                        adj_y = j + 1;
                        break;
                    case 7:
                        adj_x = i - 1;
                        adj_y = j + 1;
                        break;
                    case 8:
                        adj_x = i - 1;
                        adj_y = j;
                        break;
                    default:
                        break;

                    }
                    if (!isok(adj_x, adj_y))
                        signed_glyph = NO_GLYPH;
                    else
                    {
                        if (base_layer == LAYER_ZAP)
                        {
                            adjacent_zap_glyph = data->map[adj_x][adj_y].layer_glyphs[LAYER_ZAP];
                            adjacent_layer_flags = data->map[adj_x][adj_y].layer_flags;

                            if (adjacent_zap_glyph == NO_GLYPH || !glyph_is_zap(adjacent_zap_glyph))
                                signed_glyph = NO_GLYPH;
                            else
                                signed_glyph = zap_glyph_to_corner_glyph(adjacent_zap_glyph, adjacent_layer_flags, source_dir_idx);

                            if (signed_glyph != NO_GLYPH)
                                is_corner_tile = TRUE;
                        }
                        else if (base_layer == LAYER_MONSTER)
                        {
                            /* Worm */
                            boolean is_adj_worm_tail = !!(data->map[adj_x][adj_y].layer_flags & LFLAGS_M_WORM_TAIL);
                            boolean is_adj_worm_seen = !!(data->map[adj_x][adj_y].layer_flags & LFLAGS_M_WORM_SEEN);
                            unsigned worm_id_stored = data->map[adj_x][adj_y].m_id;
                            struct monst* worm_here = m_at(adj_x, adj_y);
                            struct monst* worm = !is_adj_worm_tail ? (struct monst*)0 : is_adj_worm_seen ? ((worm_here && worm_here->m_id == worm_id_stored) ? worm_here : (struct monst*)0) : worm_here;
                            signed_glyph = NO_GLYPH;

                            if (worm && (cansee(enl_i, enl_j) || is_adj_worm_seen || (data->map[worm->mx][worm->my].layer_flags & LFLAGS_M_WORM_SEEN)))
                            {
                                if (is_long_worm_with_tail(worm->data) && !is_adj_worm_tail)
                                {
                                    signed_glyph = NO_GLYPH;
                                }
                                else if (worm->mnum == PM_LONG_WORM_TAIL || worm->mnum == PM_ELDER_LONG_WORM_TAIL || (is_long_worm_with_tail(worm->data) && is_adj_worm_tail))
                                {
                                    int signed_main_glyph = data->map[adj_x][adj_y].layer_glyphs[base_layer];
                                    int main_glyph = abs(signed_main_glyph);
                                    int anim_frame_idx = -1, main_tile_idx = -1;
                                    int tile_animation_index = get_tile_animation_index_from_glyph(main_glyph);
                                    enum autodraw_types autodraw = AUTODRAW_NONE;
                                    int main_tile = glyph2tile[main_glyph];
                                    char mapAnimatedDummy = FALSE;
                                    int anim_frame_idx_dummy = 0, main_tile_idx_dummy = 0;
                                    main_tile = maybe_get_replaced_tile(main_tile, adj_x, adj_y, data_to_replacement_info(signed_main_glyph, base_layer, 0, worm, data->map[adj_x][adj_y].layer_flags), &autodraw);

                                    if (animation_timers.m_action_animation_counter_on && base_layer == LAYER_MONSTER && animation_timers.m_action_animation_x == adj_x && animation_timers.m_action_animation_y == adj_y)
                                        main_tile = maybe_get_animated_tile(main_tile, tile_animation_index, ANIMATION_PLAY_TYPE_PLAYED_SEPARATELY, animation_timers.m_action_animation_counter, &anim_frame_idx_dummy, &main_tile_idx_dummy, &mapAnimatedDummy, &autodraw);

                                    int base_source_glyph = GLYPH_WORM_OFF;
                                    if (autodraw > 0)
                                    {
                                        base_source_glyph = autodraws[autodraw].source_glyph4;
                                    }

                                    if (worm->wormno >= 1 && worm->wormno < MAX_NUM_WORMS)
                                    {
                                        int wdir = get_wseg_dir_at(worm, adj_x, adj_y);
                                        int tilenum = -1;
                                        if (wdir % 2 == 1)
                                        {
                                            switch (source_dir_idx)
                                            {
                                            case 2:
                                                if (wdir == 7)
                                                {
                                                    //tilenum = GENERAL_TILE_WORM_IS_UP_GOING_DOWN_LEFT;
                                                    tilenum = 1; //GENERAL_TILE_WORM_IS_DOWN_GOING_UP_LEFT;
                                                    manual_vflip = TRUE;
                                                }
                                                else if (wdir == 5)
                                                {
                                                    //tilenum = GENERAL_TILE_WORM_IS_UP_GOING_DOWN_RIGHT;
                                                    tilenum = 3; // GENERAL_TILE_WORM_IS_UP_GOING_DOWN_RIGHT;
                                                    manual_hflip = FALSE;
                                                    manual_vflip = FALSE;
                                                }
                                                break;
                                            case 4:
                                                if (wdir == 1)
                                                {
                                                    //tilenum = GENERAL_TILE_WORM_IS_RIGHT_GOING_UP_LEFT;
                                                    tilenum = 0;  //GENERAL_TILE_WORM_IS_RIGHT_GOING_UP_LEFT;
                                                    manual_hflip = FALSE;
                                                    manual_vflip = FALSE;
                                                }
                                                else if (wdir == 7)
                                                {
                                                    //tilenum = GENERAL_TILE_WORM_IS_RIGHT_GOING_DOWN_LEFT;
                                                    tilenum = 0; // GENERAL_TILE_WORM_IS_RIGHT_GOING_UP_LEFT;
                                                    manual_hflip = FALSE;
                                                    manual_vflip = TRUE;
                                                }
                                                break;
                                            case 6:
                                                if (wdir == 1)
                                                {
                                                    //tilenum = GENERAL_TILE_WORM_IS_DOWN_GOING_UP_LEFT;
                                                    tilenum = 1; // GENERAL_TILE_WORM_IS_DOWN_GOING_UP_LEFT;
                                                    manual_hflip = FALSE;
                                                    manual_vflip = FALSE;
                                                }
                                                else if (wdir == 3)
                                                {
                                                    //tilenum = GENERAL_TILE_WORM_IS_DOWN_GOING_UP_RIGHT;
                                                    tilenum = 3; // GENERAL_TILE_WORM_IS_UP_GOING_DOWN_RIGHT;
                                                    manual_hflip = FALSE;
                                                    manual_vflip = TRUE;
                                                }
                                                break;
                                            case 8:
                                                if (wdir == 3)
                                                {
                                                    //tilenum = GENERAL_TILE_WORM_IS_LEFT_GOING_UP_RIGHT;
                                                    tilenum = 2; // GENERAL_TILE_WORM_IS_LEFT_GOING_DOWN_RIGHT;
                                                    manual_hflip = FALSE;
                                                    manual_vflip = TRUE;
                                                }
                                                else if (wdir == 5)
                                                {
                                                    //tilenum = GENERAL_TILE_WORM_IS_LEFT_GOING_DOWN_RIGHT;
                                                    tilenum = 2; // GENERAL_TILE_WORM_IS_LEFT_GOING_DOWN_RIGHT;
                                                    manual_hflip = FALSE;
                                                    manual_vflip = FALSE;
                                                }
                                                break;
                                            default:
                                                break;
                                            }
                                            if(tilenum > -1)
                                                signed_glyph = tilenum + base_source_glyph;
                                        }
                                    }
                                }
                            }
                        }
                        else if (base_layer == LAYER_CHAIN)
                        {
                            /* Chain */
                            if (data->map[adj_x][adj_y].layer_flags & LFLAGS_O_CHAIN)
                            {
                                signed_glyph = (source_dir_idx / 2 - 1) + GENERAL_TILE_CHAIN_IS_UP + GLYPH_GENERAL_TILE_OFF;
                            }
                            else
                                signed_glyph = NO_GLYPH;
                        }
                    }
                }
                else if (base_layer == LAYER_LEASH && enlarg_idx == -1 && cansee(enl_i, enl_j))
                {
                    signed_glyph = NO_GLYPH;
                    /* This is effectively a non-tile-related autodraw */

                    int ux = u.ux, uy = u.uy;
                    boolean something_to_draw = FALSE;

                    /* Check first if anything needs to be drawn */
                    if (isok(ux, uy))
                    {
                        for (struct monst* leashed_mon = fmon; leashed_mon; leashed_mon = leashed_mon->nmon)
                        {
                            if (leashed_mon->mleashed)
                            {
                                int mx = leashed_mon->mx, my = leashed_mon->my;
                                int dx = mx - ux, dy = my - uy;
                                if (dx == 0 && dy == 0)
                                    continue;
                                if (isok(mx, my))
                                {
                                    int min_x = min(mx, ux);
                                    int min_y = min(my, uy);
                                    int max_x = max(mx, ux);
                                    int max_y = max(my, uy);
                                    if (enl_i >= min_x && enl_i <= max_x && enl_j >= min_y && enl_j <= max_y)
                                    {
                                        something_to_draw = TRUE;
                                        break;
                                    }
                                }
                            }
                        }

                        if (isok(context.tether_x, context.tether_y))
                        {
                            int min_x = min(context.tether_x, ux);
                            int min_y = min(context.tether_y, uy);
                            int max_x = max(context.tether_x, ux);
                            int max_y = max(context.tether_y, uy);
                            if (enl_i >= min_x && enl_i <= max_x && enl_j >= min_y && enl_j <= max_y)
                            {
                                something_to_draw = TRUE;
                            }
                        }
                    }

                    if (something_to_draw)
                    {
                        HDC hDCleash = CreateCompatibleDC(data->backBufferDC);

                        unsigned char* lpBitmapBitsLeash;

                        BITMAPINFO binfo_leash;
                        ZeroMemory(&binfo_leash, sizeof(BITMAPINFO));
                        binfo_leash.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
                        binfo_leash.bmiHeader.biWidth = tileWidth;
                        binfo_leash.bmiHeader.biHeight = -tileHeight; /* To make y-axis to be in normal order, i.e., (0,0) is top-left corner  */
                        binfo_leash.bmiHeader.biPlanes = 1;
                        binfo_leash.bmiHeader.biBitCount = 32;

                        HBITMAP newhBmp_leash = CreateDIBSection(hDCleash, &binfo_leash, DIB_RGB_COLORS, (VOID**)&lpBitmapBitsLeash, NULL, 0);
                        HGDIOBJ oldbmp_leash = SelectObject(hDCleash, newhBmp_leash);

                        /* Make background transparent */
                        LONG width = GetNHApp()->mapTile_X;
                        LONG height = GetNHApp()->mapTile_Y;

                        int pitch = 4 * width; // 4 bytes per pixel but if not 32 bit, round pitch up to multiple of 4
                        int idx, x, y;
                        for (x = 0; x < width; x++)
                        {
                            for (y = 0; y < height; y++)
                            {
                                idx = y * pitch;
                                idx += x * 4;

                                lpBitmapBitsLeash[idx + 0] = TILE_BK_COLOR_BLUE;  // blue
                                lpBitmapBitsLeash[idx + 1] = TILE_BK_COLOR_GREEN; // green
                                lpBitmapBitsLeash[idx + 2] = TILE_BK_COLOR_RED;  // red 
                            }
                        }

                        double line_start_x_in_pixels = 0.0;
                        double line_start_y_in_pixels = 0.0;
                        double line_end_x_in_pixels = 0.0;
                        double line_end_y_in_pixels = 0.0;
                        double line_in_pixels_to_next_tile_from_x_start = 0.0;
                        double line_in_pixels_to_next_tile_from_y_start = 0.0;
                        double tile_x_start = (double)enl_i * (double)tileWidth;
                        double tile_y_start = (double)enl_j * (double)tileHeight;
                        double tile_x_end = ((double)(enl_i + 1) * (double)tileWidth) - 1.0;
                        double tile_y_end = ((double)(enl_j + 1) * (double)tileHeight) - 1.0;

                        /* Draw leashes for leashed monsters */
                        /* Kludge to include also thrown aklys in the same drawing routine */
                        struct monst tether_mon = zeromonst;
                        tether_mon.nmon = fmon;
                        tether_mon.mleashed = TRUE;
                        tether_mon.mx = context.tether_x;
                        tether_mon.my = context.tether_y;

                        for (struct monst* leashed_mon = &tether_mon; leashed_mon; leashed_mon = leashed_mon->nmon)
                        {
                            boolean is_tethered_weapon = (leashed_mon == &tether_mon);
                            if (leashed_mon->mleashed)
                            {
                                int mx = leashed_mon->mx, my = leashed_mon->my;
                                int dx = mx - ux, dy = my - uy;
                                if (dx == 0 && dy == 0)
                                    continue;

                                if (isok(mx, my))
                                {
                                    int min_x = min(mx, ux);
                                    int min_y = min(my, uy);
                                    int max_x = max(mx, ux);
                                    int max_y = max(my, uy);
                                    if (!(enl_i >= min_x && enl_i <= max_x && enl_j >= min_y && enl_j <= max_y))
                                    {
                                        continue;
                                    }

                                    /* Select appropriate start point */
                                    int start_x_in_tile = tileWidth / 2;
                                    int start_y_in_tile = tileHeight / 2;

                                    /* Select appropriate end point */
                                    int end_x_in_tile = tileWidth / 2;
                                    int end_y_in_tile = tileHeight / 2 + 20; /* Since many pets are small */

                                    if (is_tethered_weapon)
                                    {
                                        /* Adjusted by hand to match the graphics */
                                        if (dx == 0)
                                            end_x_in_tile = 29;
                                        else if (dx < 0)
                                            end_x_in_tile = tileWidth - 16;
                                        else
                                            end_x_in_tile = 16;

                                        if (dy == 0)
                                            end_y_in_tile = 51;
                                        else if (dy < 0)
                                            end_y_in_tile = tileHeight - 32;
                                        else
                                            end_y_in_tile = 32;
                                    }

                                    /* Draw the relevant leash */
                                    int leash_width = 3;
                                    for (int width_round = 0; width_round < leash_width; width_round++)
                                    {
                                        line_start_x_in_pixels = (double)ux * (double)tileWidth + (double)start_x_in_tile;
                                        line_start_y_in_pixels = (double)uy * (double)tileHeight + (double)start_y_in_tile;
                                        line_end_x_in_pixels = (double)mx * (double)tileWidth + (double)end_x_in_tile;
                                        line_end_y_in_pixels = (double)my * (double)tileHeight + (double)end_y_in_tile;

                                        if (dx && !dy)
                                        {
                                            if (width_round == 1)
                                            {
                                                line_start_y_in_pixels--;
                                                line_end_y_in_pixels--;
                                            }
                                            else if (width_round == 2)
                                            {
                                                line_start_y_in_pixels++;
                                                line_end_y_in_pixels++;
                                            }
                                        }
                                        else if (!dx && dy)
                                        {
                                            if (width_round == 1)
                                            {
                                                line_start_x_in_pixels--;
                                                line_end_x_in_pixels--;
                                            }
                                            else if (width_round == 2)
                                            {
                                                line_start_x_in_pixels++;
                                                line_end_x_in_pixels++;
                                            }
                                        }
                                        else
                                        {
                                            if (width_round == 1)
                                            {
                                                if (dy > 0)
                                                {
                                                    line_start_x_in_pixels++;
                                                    line_end_x_in_pixels++;
                                                    line_start_y_in_pixels--;
                                                    line_end_y_in_pixels--;
                                                }
                                                else
                                                {
                                                    line_start_x_in_pixels++;
                                                    line_end_x_in_pixels++;
                                                    line_start_y_in_pixels++;
                                                    line_end_y_in_pixels++;
                                                }
                                            }
                                            else if (width_round == 2)
                                            {
                                                if (dy > 0)
                                                {
                                                    line_start_x_in_pixels--;
                                                    line_end_x_in_pixels--;
                                                    line_start_y_in_pixels--;
                                                    line_end_y_in_pixels--;
                                                }
                                                else
                                                {
                                                    line_start_x_in_pixels--;
                                                    line_end_x_in_pixels--;
                                                    line_start_y_in_pixels++;
                                                    line_end_y_in_pixels++;
                                                }
                                            }
                                        }

                                        if (line_start_x_in_pixels < tile_x_start && line_end_x_in_pixels < tile_x_start)
                                            continue;
                                        if (line_start_y_in_pixels < tile_y_start && line_end_y_in_pixels < tile_y_start)
                                            continue;
                                        if (line_start_x_in_pixels > tile_x_end && line_end_x_in_pixels > tile_x_end)
                                            continue;
                                        if (line_start_y_in_pixels > tile_y_end && line_end_y_in_pixels > tile_y_end)
                                            continue;

                                        double dx_in_pixels = line_end_x_in_pixels - line_start_x_in_pixels;
                                        double dy_in_pixels = line_end_y_in_pixels - line_start_y_in_pixels;
                                        if (dx_in_pixels == 0.0 && dy_in_pixels == 0.0)
                                            continue;
                                        double dx_in_tiles_from_pixels = (dx_in_pixels / (double)tileWidth);
                                        double dy_in_tiles_from_pixels = (dy_in_pixels / (double)tileHeight);
                                        boolean dx_is_larger = (fabs(dx_in_tiles_from_pixels) >= fabs(dy_in_tiles_from_pixels));
                                        double for_start_value = 0.0;
                                        double for_end_value = 1.0;
                                        double for_increment = (dx_is_larger && dx_in_pixels != 0.0) || dy_in_pixels == 0.0 ? 1.0 / fabs(dx_in_pixels) : 1.0 / fabs(dy_in_pixels);

                                        for (double line_idx = for_start_value; line_idx <= for_end_value; line_idx += for_increment)
                                        {
                                            double cur_x_in_pixels = line_idx * (line_end_x_in_pixels - line_start_x_in_pixels) + line_start_x_in_pixels;
                                            double cur_y_in_pixels = line_idx * (line_end_y_in_pixels - line_start_y_in_pixels) + line_start_y_in_pixels;

                                            if (dx > 0 && cur_x_in_pixels > tile_x_end)
                                                break;
                                            else if (dx < 0 && cur_x_in_pixels < tile_x_start)
                                                break;
                                            else if (dy > 0 && cur_y_in_pixels > tile_y_end)
                                                break;
                                            else if (dy < 0 && cur_y_in_pixels < tile_y_start)
                                                break;
                                            else if (cur_x_in_pixels < tile_x_start || cur_x_in_pixels > tile_x_end)
                                                continue;
                                            else if (cur_y_in_pixels < tile_y_start || cur_y_in_pixels > tile_y_end)
                                                continue;

                                            int within_tile_x_in_pixels = (int)(cur_x_in_pixels - tile_x_start);
                                            int within_tile_y_in_pixels = (int)(cur_y_in_pixels - tile_y_start);
                                            int bitidx = within_tile_x_in_pixels * 4 + within_tile_y_in_pixels * (int)tileWidth * 4;
                                            int red = width_round == 0 ? 178 : 80;
                                            int green = width_round == 0 ? 96 : 60;
                                            int blue = width_round == 0 ? 9 : 40;
                                            lpBitmapBitsLeash[bitidx + 0] = blue;  // blue
                                            lpBitmapBitsLeash[bitidx + 1] = green;  // green
                                            lpBitmapBitsLeash[bitidx + 2] = red;  // red 
                                        }
                                    }
                                }
                            }

                        }

                        /* Draw the leash to the map */
                        (*GetNHApp()->lpfnTransparentBlt)(
                            data->backBufferDC, rect->left, rect->top, data->xBackTile, data->yBackTile,
                            hDCleash, 0, 0, tileWidth, tileHeight, TILE_BK_COLOR);

                        SelectObject(hDCleash, oldbmp_leash);
                        DeleteDC(hDCleash);
                        DeleteObject(newhBmp_leash);
                    }
                }
                else if (base_layer == LAYER_CHAIN && enlarg_idx == -1 && tile_move_idx == 0 && source_dir_idx == 0 && (data->map[adj_x][adj_y].layer_flags & LFLAGS_O_CHAIN))
                    signed_glyph = GENERAL_TILE_CHAIN_MAIN + GLYPH_GENERAL_TILE_OFF;
                else if (base_layer == LAYER_OBJECT || base_layer == LAYER_COVER_OBJECT)
                    signed_glyph = otmp_round->glyph == NO_GLYPH || otmp_round->glyph == 0 ? NO_GLYPH : otmp_round->glyph;
                else
                    signed_glyph = data->map[enl_i][enl_j].layer_glyphs[base_layer];

                glyph = abs(signed_glyph);

                /* Kludge for the time being */
                if (base_layer == LAYER_FLOOR && glyph == NO_GLYPH)
                    glyph = cmap_to_glyph(S_unexplored);

                if (showing_detection || u.uswallow || (base_layer == LAYER_FLOOR && glyph_is_specific_cmap_or_its_variation(glyph, S_unexplored)))
                    skip_darkening = TRUE;

                /*
                 * Draw glyph
                 */
                if (glyph != NO_GLYPH)
                {
                    /* Tile flips */
                    boolean tileflag_hflip = !!(glyphtileflags[glyph] & GLYPH_TILE_FLAG_FLIP_HORIZONTALLY);
                    boolean tileflag_vflip = !!(glyphtileflags[glyph] & GLYPH_TILE_FLAG_FLIP_VERTICALLY);

                    if (((signed_glyph < 0) != tileflag_hflip) != manual_hflip) /* XOR */
                        hflip_glyph = TRUE;
                    else
                        hflip_glyph = FALSE;

                    if (tileflag_vflip != manual_vflip) /* XOR */
                        vflip_glyph = TRUE;
                    else
                        vflip_glyph = FALSE;

                    int hmultiplier = hflip_glyph ? -1 : 1;
                    int vmultiplier = vflip_glyph ? -1 : 1;

                    /* Transparency */
                    boolean make_semi_transparent = FALSE;
                    boolean make_invis_transparent = FALSE;
                    boolean use_radial_transparency = FALSE;
                    boolean use_glass_transparency = FALSE;
                    if (base_layer == LAYER_MONSTER)
                    {
                        if (mtmp && (is_semi_transparent(mtmp->data)))
                        {
                            make_semi_transparent = TRUE;
                            if (is_radially_transparent(mtmp->data))
                                use_radial_transparency = TRUE;
                            if (is_glass_transparent(mtmp->data))
                                use_glass_transparency = TRUE;
                        }

                        if ((mtmp && ((!is_enl_you && is_invisible(mtmp) && canspotmon(mtmp)))) || (is_enl_you && Invis))
                            make_invis_transparent = TRUE;

                    }
                    else if (base_layer == LAYER_COVER_TRAP)
                        make_semi_transparent = TRUE;

                    /* Set position_index */
                    if (enlarg_idx == -1)
                    {
                        if (vflip_glyph)
                            position_index = 1;
                        else
                            position_index = -1;
                    }
                    else if (enlarg_idx == 0)
                    {
                        if (vflip_glyph)
                            position_index = hflip_glyph ? 0 : 2;
                        else
                            position_index = hflip_glyph ? 3 : 4;
                    }
                    else if (enlarg_idx == 1)
                    {
                        if (vflip_glyph)
                            position_index = hflip_glyph ? 2 : 0;
                        else
                            position_index = hflip_glyph ? 4 : 3;
                    }
                    else if (enlarg_idx == 2)
                    {
                        if (vflip_glyph)
                            position_index = hflip_glyph ? 3 : 4;
                        else
                            position_index = hflip_glyph ? 0 : 2;
                    }
                    else if (enlarg_idx == 3)
                    {
                        if (vflip_glyph)
                            position_index = -1;
                        else
                            position_index = 1;
                    }
                    else if (enlarg_idx == 4)
                    {
                        if (vflip_glyph)
                            position_index = hflip_glyph ? 4 : 3;
                        else
                            position_index = hflip_glyph ? 2 : 0;
                    }

                    int anim_frame_idx = -1, main_tile_idx = -1;
                    int tile_animation_idx = get_tile_animation_index_from_glyph(glyph);
                    boolean skip_drawing = FALSE;
                    boolean full_sized_item = !!(glyphtileflags[glyph] & GLYPH_TILE_FLAG_FULL_SIZED_ITEM) || glyph_is_monster(glyph); /* hallucinated statue */
                    boolean move_obj_to_middle = ((glyphtileflags[glyph] & GLYPH_TILE_FLAG_NORMAL_ITEM_AS_MISSILE) && !full_sized_item);
                    boolean does_not_cause_monster_shadow = FALSE;
                    boolean is_dropping_piercer = mtmp && (data->map[enl_i][enl_j].layer_flags & LFLAGS_M_DROPPING_PIERCER);

                    enum autodraw_types autodraw = AUTODRAW_NONE;
                    ntile = glyph2tile[glyph];
                    ntile = maybe_get_replaced_tile(ntile, i, j, data_to_replacement_info(signed_glyph, base_layer, otmp_round, mtmp, data->map[enl_i][enl_j].layer_flags), &autodraw);
                    if(animation_timers.u_action_animation_counter_on && base_layer == LAYER_MONSTER && enl_i == u.ux && enl_j == u.uy)
                        ntile = maybe_get_animated_tile(ntile, tile_animation_idx, ANIMATION_PLAY_TYPE_PLAYED_SEPARATELY, animation_timers.u_action_animation_counter, &anim_frame_idx, &main_tile_idx, &data->mapAnimated[i][j], &autodraw);
                    else if (animation_timers.m_action_animation_counter_on && ((!is_dropping_piercer && base_layer == LAYER_MONSTER) || (is_dropping_piercer && base_layer == LAYER_MISSILE)) && animation_timers.m_action_animation_x == enl_i && animation_timers.m_action_animation_y == enl_j)
                        ntile = maybe_get_animated_tile(ntile, tile_animation_idx, ANIMATION_PLAY_TYPE_PLAYED_SEPARATELY, animation_timers.m_action_animation_counter, &anim_frame_idx, &main_tile_idx, &data->mapAnimated[i][j], &autodraw);
                    else if (glyph_is_explosion(glyph))
                        ntile = maybe_get_animated_tile(ntile, tile_animation_idx, ANIMATION_PLAY_TYPE_PLAYED_SEPARATELY, animation_timers.explosion_animation_counter, &anim_frame_idx, &main_tile_idx, &data->mapAnimated[i][j], &autodraw);
                    else if (glyph_is_zap(glyph))
                    {
                        boolean zap_found = FALSE;
                        for (int zap_anim_idx = 0; zap_anim_idx < MAX_PLAYED_ZAP_ANIMATIONS; zap_anim_idx++)
                        {
                            if (animation_timers.zap_animation_counter_on[zap_anim_idx]
                                && adj_x == animation_timers.zap_animation_x[zap_anim_idx]
                                && adj_y == animation_timers.zap_animation_y[zap_anim_idx])
                            {
                                ntile = maybe_get_animated_tile(ntile, tile_animation_idx, ANIMATION_PLAY_TYPE_PLAYED_SEPARATELY, animation_timers.zap_animation_counter[zap_anim_idx], &anim_frame_idx, &main_tile_idx, &data->mapAnimated[i][j], &autodraw);
                                zap_found = TRUE;
                                break;
                            }
                        }
 
                        /* Otherwise, normal animation check */
                        if (!zap_found)
                            ntile = maybe_get_animated_tile(ntile, tile_animation_idx, ANIMATION_PLAY_TYPE_ALWAYS, animation_timers.general_animation_counter, &anim_frame_idx, &main_tile_idx, &data->mapAnimated[i][j], &autodraw);
                    }
                    else
                    {
                        /* Check for special effect animations */
                        boolean spef_found = FALSE;
                        for (int spef_idx = 0; spef_idx < MAX_PLAYED_SPECIAL_EFFECTS; spef_idx++)
                        {
                            if (animation_timers.special_effect_animation_counter_on[spef_idx]
                                && base_layer == animation_timers.spef_action_animation_layer[spef_idx]
                                && enl_i == animation_timers.spef_action_animation_x[spef_idx]
                                && enl_j == animation_timers.spef_action_animation_y[spef_idx])
                            {
                                ntile = maybe_get_animated_tile(ntile, tile_animation_idx, ANIMATION_PLAY_TYPE_PLAYED_SEPARATELY, animation_timers.special_effect_animation_counter[spef_idx], &anim_frame_idx, &main_tile_idx, &data->mapAnimated[i][j], &autodraw);
                                spef_found = TRUE;
                                break;
                            }
                        }
    
                        /* Otherwise, normal animation check */
                        if(!spef_found)
                            ntile = maybe_get_animated_tile(ntile, tile_animation_idx, ANIMATION_PLAY_TYPE_ALWAYS, animation_timers.general_animation_counter, &anim_frame_idx, &main_tile_idx, &data->mapAnimated[i][j], &autodraw);
                    }
                    if (enlarg_idx >= 0)
                    {
                        if (tile2enlargement[ntile] > 0)
                        {
                            int enl_tile_idx = enlargements[tile2enlargement[ntile]].position2tile[position_index];
                            if (enl_tile_idx >= 0)
                            {
                                autodraw = enlargements[tile2enlargement[ntile]].position_autodraw[position_index];
                                does_not_cause_monster_shadow = !!(enlargements[tile2enlargement[ntile]].position_flags[position_index] & ENLFLAGS_DOES_NOT_CAUSE_MONSTER_SHADOW);
                                int addedindex = 0;
                                if (enlargements[tile2enlargement[ntile]].number_of_animation_frames > 0)
                                {
                                    if (main_tile_idx == -1
                                        && anim_frame_idx >= 0
                                        && anim_frame_idx < enlargements[tile2enlargement[ntile]].number_of_animation_frames
                                        )
                                    {
                                        addedindex = anim_frame_idx * enlargements[tile2enlargement[ntile]].number_of_enlargement_tiles;
                                    }
                                    else if (main_tile_idx == 0
                                        && anim_frame_idx > 0
                                        && anim_frame_idx <= enlargements[tile2enlargement[ntile]].number_of_animation_frames)
                                    {
                                        addedindex = (anim_frame_idx - 1) * enlargements[tile2enlargement[ntile]].number_of_enlargement_tiles;
                                    }
                                    else if (main_tile_idx == enlargements[tile2enlargement[ntile]].number_of_animation_frames
                                        && anim_frame_idx >= 0
                                        && anim_frame_idx < enlargements[tile2enlargement[ntile]].number_of_animation_frames
                                        )
                                    {
                                        addedindex = anim_frame_idx * enlargements[tile2enlargement[ntile]].number_of_enlargement_tiles;
                                    }
                                }
                                int enl_glyph = enl_tile_idx + addedindex + enlargement_offsets[tile2enlargement[ntile]] /* enlargements[tile2enlargement[ntile]].glyph_offset*/ + GLYPH_ENLARGEMENT_OFF;
                                ntile = glyph2tile[enl_glyph]; /* replace */
                            }
                            else
                            {
                                skip_drawing = TRUE;
                            }
                        }
                        else
                        {
                            skip_drawing = TRUE;
                        }
                    }
                    else
                    {
                        /* Main tile */
                        /* Save printed tile for objects so enlargements can be checked later */
                        if (base_layer == LAYER_OBJECT)
                        {
                            data->printedObjectTile[i][j][MAX_SHOWN_OBJECTS - 1 - layer_round] = ntile;
                        }
                        else if (base_layer == LAYER_COVER_OBJECT)
                        {
                            data->printedCoverObjectTile[i][j][MAX_SHOWN_OBJECTS - 1 - layer_round] = ntile;
                        }
                    }

                    if (!skip_drawing)
                    {
                        //int dest_top_added = move_obj_to_middle || layer_rounds == 1 ? 0 :(int)(((double)(rect->bottom - rect->top)/((double)tileHeight / 2.0)) * -1.0 * (double)(MAX_LAYERS - 1 - layer_round) * 2.0);
                        int dest_top_added = 0;
                        int dest_height_deducted = 0;
                        int dest_left_added = 0;
                        int dest_width_deducted = 0;
                        int source_top_added = 0;
                        int source_height_deducted = 0;
                        int sheet_idx = TILE_SHEET_IDX(ntile);
                        int base_t_x = TILEBMP_X(ntile);
                        int base_t_y = TILEBMP_Y(ntile);
                        t_x = base_t_x + (hflip_glyph ? tileWidth - 1 : 0);
                        t_y = base_t_y + (vflip_glyph ? tileHeight - 1 : 0);
                        if (layer_round > 0)
                            layer_round = layer_round;

                        double applicable_scaling_factor_x = print_first_directly_to_map ? x_scaling_factor : 1.0;
                        double applicable_scaling_factor_y = print_first_directly_to_map ? y_scaling_factor : 1.0;
                        double obj_scaling_factor = 1.0;
                        boolean is_obj_missile = FALSE;
                        boolean is_object = FALSE;
                        int base_source_top_added = 0;

                        if (base_layer == LAYER_MISSILE && glyph_is_missile(glyph))
                        {
                            is_obj_missile = TRUE;
                            boolean use_floor_tile = FALSE;
                            if (glyph_is_object_missile(glyph))
                            {
                                int otyp = (glyph - GLYPH_OBJ_MISSILE_OFF) / NUM_MISSILE_DIRS;
                                use_floor_tile = has_otyp_floor_tile(otyp);
                                if (!has_otyp_floor_tile(otyp) && !has_otyp_height_clipping(otyp) && OBJ_TILE_HEIGHT(otyp) > 0)
                                    obj_scaling_factor = ((double)OBJ_TILE_HEIGHT(otyp)) / 48.0;
                            }
                            else if (glyph_is_artifact_missile(glyph))
                            {
                                int artidx = ((glyph - GLYPH_ARTIFACT_MISSILE_OFF) / NUM_MISSILE_DIRS) + 1;
                                use_floor_tile = has_artifact_floor_tile(artidx);
                                if (!has_artifact_floor_tile(artidx) && !has_artifact_height_clipping(artidx) && artilist[artidx].tile_floor_height > 0)
                                    obj_scaling_factor = ((double)artilist[artidx].tile_floor_height) / 48.0;
                            }

                            /* This is a normal object used as a missile; now only lower part is used, object is moved upwards 1/4 of the tile */
                            if (move_obj_to_middle)
                            {
                                /* Take lower part only */
                                source_top_added = use_floor_tile  ? 0 : tileHeight / 2;
                                source_height_deducted = tileHeight / 2;
                                dest_top_added = (int)(applicable_scaling_factor_y * ((double)GetNHApp()->mapTile_Y / 4.0));
                                dest_height_deducted = (int)(applicable_scaling_factor_y * ((double)GetNHApp()->mapTile_Y / 2.0));
                            }
                        }
                        else if (base_layer == LAYER_OBJECT || base_layer == LAYER_COVER_OBJECT)
                        {
                            is_object = TRUE;

                            if (!full_sized_item)
                            {
                                int used_item_height = (int)tileHeight / 2;
                                int base_source_height_deducted = used_item_height;
                                int base_dest_added_from_source = 0;
                                int artidx = otmp_round ? otmp_round->oartifact : 0;
                                boolean has_floor_tile = !otmp_round ? FALSE : artidx ? has_artifact_floor_tile(artidx) : has_obj_floor_tile(otmp_round);
                                boolean is_height_clipping = !otmp_round ? FALSE : artidx ? has_artifact_height_clipping(artidx) : has_obj_height_clipping(otmp_round);
                                int obj_height = get_obj_height(otmp_round);

                                /* For all normal items, we use only lower part of the tile */
                                if (otmp_round && (has_floor_tile || is_height_clipping) && !showing_detection && !Hallucination)
                                {
                                    source_top_added = has_floor_tile ? 0 : (int)tileHeight / 2;
                                    if (otmp_round && obj_height > 0 && obj_height < used_item_height && !showing_detection)
                                    {
                                        base_dest_added_from_source = used_item_height - obj_height;
                                        base_source_top_added += base_dest_added_from_source / 2;
                                        base_source_height_deducted += used_item_height - obj_height;
                                    }
                                }
                                else
                                {
                                    source_top_added = (int)tileHeight / 2;
                                    if (otmp_round && obj_height > 0 && !showing_detection && !Hallucination)
                                    {
                                        obj_scaling_factor = ((double)obj_height) / ((double)used_item_height);
                                    }
                                }

                                source_top_added += base_source_top_added;
                                source_height_deducted += base_source_height_deducted;

                                dest_top_added = (int)(applicable_scaling_factor_y * ((double)((int)tileHeight - used_item_height + base_dest_added_from_source)));
                                dest_height_deducted = (int)(applicable_scaling_factor_y * ((double)base_source_height_deducted));

                                if (otmp_round && (otmp_round->speflags & SPEFLAGS_CAUGHT_IN_LEAVES))
                                {
                                    dest_top_added -= OBJECT_CAUGHT_IN_LEAVES_HEIGHT;
                                }
                                else
                                {
                                    /* Leave a little room for monster feet if not cover object */
                                    if (base_layer == LAYER_OBJECT)
                                        dest_top_added += (int)(applicable_scaling_factor_y * (double)(objects_in_pit ? -PIT_BOTTOM_BORDER : -OBJECT_PILE_START_HEIGHT));

                                    /* Pile the objects in order with two pixels in between */
                                    if (layer_rounds > 1)
                                    {
                                        dest_top_added += (int)(applicable_scaling_factor_y * ((double)-OBJECT_PILE_HEIGHT_DIFFERENCE * (double)(max(0, layer_round - (layer_rounds - objcnt)) /*layer_rounds - 1 - layer_round*/)));
                                    }
                                    if (otmp_round && !showing_detection && objects_in_pit)
                                        obj_scaling_factor *= OBJECT_PIT_SCALING_FACTOR;
                                }
                            }
                        }
                        else if (base_layer == LAYER_MONSTER || base_layer == LAYER_MONSTER_EFFECT)
                        {
                            if (tile_move_idx == 0)
                            {
                                if (monster_layer_height > 0)
                                {
                                    source_top_added = monster_layer_height;
                                    source_height_deducted = monster_layer_height;
                                    dest_top_added = 0;
                                    dest_height_deducted = (int)(applicable_scaling_factor_y * monster_layer_height);
                                }
                                else if (monster_layer_height < 0)
                                {
                                    source_top_added = 0;
                                    source_height_deducted = abs(monster_layer_height) - PIT_BOTTOM_BORDER;
                                    dest_top_added = (int)(applicable_scaling_factor_y * ((double)abs(monster_layer_height) - (double)PIT_BOTTOM_BORDER));
                                    dest_height_deducted = (int)(applicable_scaling_factor_y * ((double)(abs(monster_layer_height) - PIT_BOTTOM_BORDER)));
                                }
                            }
                            else if (tile_move_idx == -1)
                            {
                                if (monster_layer_height < 0)
                                {
                                    source_top_added = tileHeight - (abs(monster_layer_height) - PIT_BOTTOM_BORDER);
                                    source_height_deducted = tileHeight - (abs(monster_layer_height) - PIT_BOTTOM_BORDER);
                                    dest_top_added = 0;
                                    dest_height_deducted = (int)(applicable_scaling_factor_y * (double)(tileHeight - (abs(monster_layer_height) - PIT_BOTTOM_BORDER)));
                                }

                            }
                            else if (tile_move_idx == 1)
                            {
                                if (monster_layer_height > 0)
                                {
                                    source_top_added = 0;
                                    source_height_deducted = tileHeight - monster_layer_height;
                                    dest_top_added = (int)(applicable_scaling_factor_y * (tileHeight - monster_layer_height));
                                    dest_height_deducted = (int)(applicable_scaling_factor_y * (tileHeight - monster_layer_height));
                                }
                            }
                        }

                        if ((enlarg_idx >= 0 || (base_layer >= LAYER_COVER_OBJECT && base_layer <= IDX_LAYER_MONSTER_SHADOW)) && !does_not_cause_monster_shadow)
                             draw_monster_shadow = TRUE;

                        /* Scale object to be of oc_tile_floor_height height */
                        if ((is_obj_missile || is_object) && obj_scaling_factor != 1.0)
                        {
                            double fulltileheight = (double)GetNHApp()->mapTile_Y / ((is_object && !full_sized_item) || move_obj_to_middle ? 2.0 : 1.0);
                            double scaled_height = (obj_scaling_factor * fulltileheight);
                            double scaled_width = (obj_scaling_factor * (double)GetNHApp()->mapTile_X);

                            if(is_object)
                                dest_top_added += (int)(applicable_scaling_factor_y * (fulltileheight - scaled_height));
                            else
                                dest_top_added += (int)(applicable_scaling_factor_y * (fulltileheight - scaled_height) / 2.0);

                            dest_height_deducted += (int)(applicable_scaling_factor_y * (fulltileheight - scaled_height));
                            dest_left_added += (int)(applicable_scaling_factor_x * (((double)GetNHApp()->mapTile_X - scaled_width) / 2.0));
                            dest_width_deducted += (int)(applicable_scaling_factor_x * ((double)GetNHApp()->mapTile_X - scaled_width));
                        }


                        if (make_semi_transparent || make_invis_transparent)
                        {
                            /* Create copy of background */
                            HDC hDCMem = CreateCompatibleDC(data->backBufferDC);

                            unsigned char* lpBitmapBits;
                            LONG width = rect->right - rect->left;
                            LONG height = rect->bottom - rect->top;

                            BITMAPINFO bi;
                            ZeroMemory(&bi, sizeof(BITMAPINFO));
                            bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
                            bi.bmiHeader.biWidth = width;
                            bi.bmiHeader.biHeight = height;
                            bi.bmiHeader.biPlanes = 1;
                            bi.bmiHeader.biBitCount = 32;

                            HBITMAP bitmap = CreateDIBSection(hDCMem, &bi, DIB_RGB_COLORS, (VOID**)&lpBitmapBits, NULL, 0);
                            HGDIOBJ oldbmp = SelectObject(hDCMem, bitmap);
                            if (!opaque_background_drawn || print_first_directly_to_map)
                            {
                                StretchBlt(hDCMem, 0, 0, width, height,
                                    data->backBufferDC, rect->left + (int)(x_scaling_factor * (double)(hflip_glyph ? tileWidth - 1 : 0)), 
                                    rect->top + (int)(y_scaling_factor * (double)(vflip_glyph ? tileHeight - 1 : 0)), hmultiplier * width, vmultiplier * height, SRCCOPY);
                            }
                            else
                            {
                                StretchBlt(hDCMem, 0, 0, width, height,
                                    hDCcopy, (hflip_glyph ? tileWidth - 1 : 0), (vflip_glyph ? tileHeight - 1 : 0), hmultiplier * tileWidth, vmultiplier * tileHeight, SRCCOPY);
                            }

                            /* Create copy of tile to be drawn */
                            HDC hDCsemitransparent = CreateCompatibleDC(data->backBufferDC);

                            unsigned char* lpBitmapBitsSemitransparent;

                            BITMAPINFO binfo_st;
                            ZeroMemory(&binfo_st, sizeof(BITMAPINFO));
                            binfo_st.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
                            binfo_st.bmiHeader.biWidth = width;
                            binfo_st.bmiHeader.biHeight = height;
                            binfo_st.bmiHeader.biPlanes = 1;
                            binfo_st.bmiHeader.biBitCount = 32;

                            HBITMAP newhBmp_st = CreateDIBSection(hDCsemitransparent, &binfo_st, DIB_RGB_COLORS, (VOID**)&lpBitmapBitsSemitransparent, NULL, 0);
                            HGDIOBJ oldbmp_st = SelectObject(hDCsemitransparent, newhBmp_st);
                            StretchBlt(hDCsemitransparent, 0, 0, width, height,
                                data->tileDC[sheet_idx], base_t_x, base_t_y, tileWidth, tileHeight, SRCCOPY);

                            /* Draw semitransparency */
                            int pitch = 4 * width; // 4 bytes per pixel but if not 32 bit, round pitch up to multiple of 4
                            int idx, x, y;
                            double base_transparency = use_glass_transparency ? 0.35 : 0.5;
                            double semi_transparency = make_semi_transparent && make_invis_transparent ? (1.0 - (1.0 - base_transparency) * 0.85) : base_transparency;

                            if (use_radial_transparency)
                            {
                                double mid_x = (double)width / 2.0 - 0.5;
                                double mid_y = (double)height / 2.0 - 0.5 - (base_layer == LAYER_MONSTER || base_layer == LAYER_MONSTER_EFFECT ? height * tile_move_idx + monster_layer_height : 0);
                                double r = 0;
                                for (x = 0; x < width; x++)
                                {
                                    for (y = 0; y < height; y++)
                                    {
                                        idx = y * pitch;
                                        idx += x * 4;
                                        r = sqrt(pow((double)x - mid_x, 2.0) + pow((double)y - mid_y, 2.0));
                                        semi_transparency = r * 0.0375;
                                        if(semi_transparency > 0.98)
                                            semi_transparency = 0.98;

                                        if(make_invis_transparent)
                                            semi_transparency = 1.0 - (1.0 - semi_transparency) * 0.85;

                                        if (lpBitmapBitsSemitransparent[idx + 0] == TILE_BK_COLOR_BLUE && lpBitmapBitsSemitransparent[idx + 1] == TILE_BK_COLOR_GREEN && lpBitmapBitsSemitransparent[idx + 2] == TILE_BK_COLOR_RED)
                                            continue;

                                        lpBitmapBitsSemitransparent[idx + 0] = (unsigned char)(((double)lpBitmapBitsSemitransparent[idx + 0]) * (1.0 - semi_transparency) + ((double)lpBitmapBits[idx + 0]) * (semi_transparency));  // blue
                                        lpBitmapBitsSemitransparent[idx + 1] = (unsigned char)(((double)lpBitmapBitsSemitransparent[idx + 1]) * (1.0 - semi_transparency) + ((double)lpBitmapBits[idx + 1]) * (semi_transparency));  // green
                                        lpBitmapBitsSemitransparent[idx + 2] = (unsigned char)(((double)lpBitmapBitsSemitransparent[idx + 2]) * (1.0 - semi_transparency) + ((double)lpBitmapBits[idx + 2]) * (semi_transparency));  // red 
                                    }
                                }
                            }
                            else
                            {
                                for (x = 0; x < width; x++)
                                {
                                    for (y = 0; y < height; y++)
                                    {
                                        idx = y * pitch;
                                        idx += x * 4;

                                        if (lpBitmapBitsSemitransparent[idx + 0] == TILE_BK_COLOR_BLUE && lpBitmapBitsSemitransparent[idx + 1] == TILE_BK_COLOR_GREEN && lpBitmapBitsSemitransparent[idx + 2] == TILE_BK_COLOR_RED)
                                            continue;

                                        lpBitmapBitsSemitransparent[idx + 0] = (unsigned char)(((double)lpBitmapBitsSemitransparent[idx + 0]) * (1.0 - semi_transparency) + ((double)lpBitmapBits[idx + 0]) * (semi_transparency));  // blue
                                        lpBitmapBitsSemitransparent[idx + 1] = (unsigned char)(((double)lpBitmapBitsSemitransparent[idx + 1]) * (1.0 - semi_transparency) + ((double)lpBitmapBits[idx + 1]) * (semi_transparency));  // green
                                        lpBitmapBitsSemitransparent[idx + 2] = (unsigned char)(((double)lpBitmapBitsSemitransparent[idx + 2]) * (1.0 - semi_transparency) + ((double)lpBitmapBits[idx + 2]) * (semi_transparency));  // red 
                                    }
                                }
                            }

                            if (opaque_background_drawn)
                            {
                                if (print_first_directly_to_map)
                                {
                                    (*GetNHApp()->lpfnTransparentBlt)(
                                        data->backBufferDC, rect->left + dest_left_added, rect->top + dest_top_added,
                                        data->xBackTile - dest_width_deducted, data->yBackTile - dest_height_deducted, hDCsemitransparent, (hflip_glyph ? tileWidth - 1 : 0),
                                        source_top_added + (vflip_glyph ? tileHeight - 1 : 0), hmultiplier * GetNHApp()->mapTile_X,
                                        vmultiplier * GetNHApp()->mapTile_Y - source_height_deducted, TILE_BK_COLOR);
                                }
                                else
                                {
                                    (*GetNHApp()->lpfnTransparentBlt)(
                                        hDCcopy, dest_left_added, dest_top_added,
                                        GetNHApp()->mapTile_X - dest_width_deducted, GetNHApp()->mapTile_Y - dest_height_deducted, hDCsemitransparent,
                                        (hflip_glyph ? tileWidth - 1 : 0), source_top_added + (vflip_glyph ? tileHeight - 1 : 0), hmultiplier * GetNHApp()->mapTile_X,
                                        vmultiplier * (GetNHApp()->mapTile_Y - source_height_deducted), TILE_BK_COLOR);
                                }
                            }
                            else
                            {
                                if (print_first_directly_to_map)
                                {
                                    (*GetNHApp()->lpfnTransparentBlt)(
                                        data->backBufferDC, rect->left + dest_left_added, rect->top + dest_top_added,
                                        data->xBackTile - dest_width_deducted, data->yBackTile - dest_height_deducted, hDCsemitransparent, (hflip_glyph ? width - 1 : 0),
                                        source_top_added + (vflip_glyph ? height - 1 : 0), hmultiplier * width,
                                        vmultiplier * (height - source_height_deducted), TILE_BK_COLOR);
                                }
                                else
                                {
                                    if (dest_left_added != 0 || dest_top_added != 0 || dest_width_deducted != 0 || dest_height_deducted != 0)
                                    {
                                        LONG width = GetNHApp()->mapTile_X;
                                        LONG height = GetNHApp()->mapTile_Y;

                                        int pitch = 4 * width; // 4 bytes per pixel but if not 32 bit, round pitch up to multiple of 4
                                        int idx, x, y;
                                        for (x = 0; x < width; x++)
                                        {
                                            for (y = 0; y < height; y++)
                                            {
                                                idx = y * pitch;
                                                idx += x * 4;

                                                lpBitmapBitsCopy[idx + 0] = TILE_BK_COLOR_BLUE;  // blue
                                                lpBitmapBitsCopy[idx + 1] = TILE_BK_COLOR_GREEN; // green
                                                lpBitmapBitsCopy[idx + 2] = TILE_BK_COLOR_RED;  // red 
                                            }
                                        }
                                    }

                                    StretchBlt(hDCcopy, dest_left_added, dest_top_added,
                                        GetNHApp()->mapTile_X - dest_width_deducted, GetNHApp()->mapTile_Y - dest_height_deducted, hDCsemitransparent,
                                        (hflip_glyph ? tileWidth - 1 : 0), source_top_added + (vflip_glyph ? tileHeight - 1 : 0), hmultiplier * width,
                                        vmultiplier * (height - source_height_deducted), SRCCOPY);
                                }
                                opaque_background_drawn = TRUE;
                            }

                            /* Copy to be a shadow */
                            if (base_layer == LAYER_MONSTER && enlarg_idx == -1 && tile_move_idx == 0 && !monster_copied)
                            {
                                monster_copied = TRUE;
                                StretchBlt(hDCmonster, dest_left_added, dest_top_added,
                                    GetNHApp()->mapTile_X - dest_width_deducted, GetNHApp()->mapTile_Y - dest_height_deducted, hDCsemitransparent,
                                    (hflip_glyph ? tileWidth - 1 : 0), source_top_added + (vflip_glyph ? tileHeight - 1 : 0), hmultiplier* width,
                                    vmultiplier * (height - source_height_deducted), SRCCOPY);
                            }

                            SelectObject(hDCsemitransparent, oldbmp_st);
                            DeleteDC(hDCsemitransparent);
                            DeleteObject(newhBmp_st);

                            SelectObject(hDCMem, oldbmp);
                            DeleteDC(hDCMem);
                            DeleteObject(bitmap);

                        }
                        else
                        {
                            if (opaque_background_drawn)
                            {
                                if (print_first_directly_to_map)
                                {
                                    (*GetNHApp()->lpfnTransparentBlt)(
                                        data->backBufferDC, rect->left + dest_left_added, rect->top + dest_top_added,
                                        data->xBackTile - dest_width_deducted, data->yBackTile - dest_height_deducted, data->tileDC[sheet_idx], t_x,
                                        t_y + source_top_added, hmultiplier * GetNHApp()->mapTile_X,
                                        vmultiplier * (GetNHApp()->mapTile_Y - source_height_deducted), TILE_BK_COLOR);
                                }
                                else
                                {
                                    (*GetNHApp()->lpfnTransparentBlt)(
                                        hDCcopy, dest_left_added, dest_top_added,
                                        GetNHApp()->mapTile_X - dest_width_deducted, GetNHApp()->mapTile_Y - dest_height_deducted, data->tileDC[sheet_idx],
                                        t_x, t_y + source_top_added, hmultiplier * GetNHApp()->mapTile_X,
                                        vmultiplier * (GetNHApp()->mapTile_Y - source_height_deducted), TILE_BK_COLOR);
                                }
                            }
                            else
                            {
                                if (print_first_directly_to_map)
                                {
                                    StretchBlt(
                                        data->backBufferDC, rect->left + dest_left_added, rect->top + dest_top_added,
                                        data->xBackTile - dest_width_deducted, data->yBackTile - dest_height_deducted, data->tileDC[sheet_idx], t_x,
                                        t_y + source_top_added, hmultiplier * GetNHApp()->mapTile_X,
                                        vmultiplier * (GetNHApp()->mapTile_Y - source_height_deducted), SRCCOPY);
                                }
                                else
                                {
                                    if (dest_left_added != 0 || dest_top_added != 0 || dest_width_deducted != 0 || dest_height_deducted != 0)
                                    {
                                        LONG width = GetNHApp()->mapTile_X;
                                        LONG height = GetNHApp()->mapTile_Y;

                                        int pitch = 4 * width; // 4 bytes per pixel but if not 32 bit, round pitch up to multiple of 4
                                        int idx, x, y;
                                        for (x = 0; x < width; x++)
                                        {
                                            for (y = 0; y < height; y++)
                                            {
                                                idx = y * pitch;
                                                idx += x * 4;

                                                lpBitmapBitsCopy[idx + 0] = TILE_BK_COLOR_BLUE;  // blue
                                                lpBitmapBitsCopy[idx + 1] = TILE_BK_COLOR_GREEN; // green
                                                lpBitmapBitsCopy[idx + 2] = TILE_BK_COLOR_RED;  // red 
                                            }
                                        }

                                    }

                                    StretchBlt(hDCcopy, dest_left_added, dest_top_added,
                                        GetNHApp()->mapTile_X - dest_width_deducted, GetNHApp()->mapTile_Y - dest_height_deducted, data->tileDC[sheet_idx],
                                        t_x, t_y + source_top_added, hmultiplier * GetNHApp()->mapTile_X,
                                        vmultiplier * (GetNHApp()->mapTile_Y - source_height_deducted), SRCCOPY);
                                }
                                opaque_background_drawn = TRUE;
                            }
                            if (base_layer == LAYER_MONSTER && enlarg_idx == -1 && tile_move_idx == 0 && !monster_copied)
                            {
                                monster_copied = TRUE;

                                StretchBlt(hDCmonster, dest_left_added, dest_top_added,
                                    GetNHApp()->mapTile_X - dest_width_deducted, GetNHApp()->mapTile_Y - dest_height_deducted, data->tileDC[sheet_idx],
                                    t_x, t_y + source_top_added, hmultiplier* GetNHApp()->mapTile_X,
                                    vmultiplier* (GetNHApp()->mapTile_Y - source_height_deducted), SRCCOPY);
                            }
                        }
                        /* 
                         * AUTODRAW START
                         */
                         boolean autodraw_u_punished = (base_layer == LAYER_MONSTER && enlarg_idx == -1 && tile_move_idx == 0 && i == u.ux && j == u.uy && uball && uchain);

                        if (autodraw > AUTODRAW_NONE || autodraw_u_punished)
                        {
                            if (autodraws[autodraw].draw_type == AUTODRAW_DRAW_REPLACE_WALL_ENDS)
                            {
                                for (int dir = 0; dir < 4; dir++)
                                {
                                    char dir_bit = 1 << dir;
                                    if (autodraws[autodraw].flags & dir_bit)
                                    {
                                        int rx = 0;
                                        int ry = 0;
                                        int corner_x[2] = { 0, 0 };
                                        int corner_y[2] = { 0, 0 };
                                        switch (dir)
                                        {
                                        case 0:
                                            rx = i - 1;
                                            ry = j;
                                            corner_x[0] = i;
                                            corner_y[0] = j - 1;
                                            corner_x[1] = i;
                                            corner_y[1] = j + 1;
                                            break;
                                        case 1:
                                            rx = i + 1;
                                            ry = j;
                                            corner_x[0] = i;
                                            corner_y[0] = j - 1;
                                            corner_x[1] = i;
                                            corner_y[1] = j + 1;
                                            break;
                                        case 2:
                                            rx = i;
                                            ry = j - 1;
                                            corner_x[0] = i - 1;
                                            corner_y[0] = j;
                                            corner_x[1] = i + 1;
                                            corner_y[1] = j;
                                            break;
                                        case 3:
                                            rx = i;
                                            ry = j + 1;
                                            corner_x[0] = i - 1;
                                            corner_y[0] = j;
                                            corner_x[1] = i + 1;
                                            corner_y[1] = j;
                                            break;
                                        default:
                                            break;
                                        }

                                        if (NO_WALL_END_AUTODRAW(rx, ry))
                                        {
                                            /* No action */
                                        }
                                        else
                                        {
                                            for (int corner = 0; corner <= 1; corner++)
                                            {
                                                int source_glyph = autodraws[autodraw].source_glyph;
                                                int atile = glyph2tile[source_glyph];
                                                int a_sheet_idx = TILE_SHEET_IDX(atile);
                                                int at_x = TILEBMP_X(atile);
                                                int at_y = TILEBMP_Y(atile);

                                                RECT source_rt = { 0 };
                                                switch (dir)
                                                {
                                                case 0: /* left */
                                                    if (NO_WALL_END_AUTODRAW(corner_x[corner], corner_y[corner]))
                                                    {
                                                        source_glyph = autodraws[autodraw].source_glyph2; /* S_vwall */
                                                        atile = glyph2tile[source_glyph];
                                                        a_sheet_idx = TILE_SHEET_IDX(atile);
                                                        at_x = TILEBMP_X(atile);
                                                        at_y = TILEBMP_Y(atile);
                                                    }
                                                    source_rt.left = at_x;
                                                    source_rt.right = source_rt.left + 12;
                                                    if (corner == 0)
                                                    {
                                                        source_rt.top = at_y;
                                                        source_rt.bottom = at_y + 18;
                                                    }
                                                    else
                                                    {
                                                        source_rt.top = at_y + 18;
                                                        source_rt.bottom = at_y + tileHeight;
                                                    }
                                                    break;
                                                case 1: /* right */
                                                    if (NO_WALL_END_AUTODRAW(corner_x[corner], corner_y[corner]))
                                                    {
                                                        source_glyph = autodraws[autodraw].source_glyph2; /* S_vwall */
                                                        atile = glyph2tile[source_glyph];
                                                        a_sheet_idx = TILE_SHEET_IDX(atile);
                                                        at_x = TILEBMP_X(atile);
                                                        at_y = TILEBMP_Y(atile);
                                                    }
                                                    source_rt.right = at_x + tileWidth;
                                                    source_rt.left = source_rt.right - 12;
                                                    if (corner == 0)
                                                    {
                                                        source_rt.top = at_y;
                                                        source_rt.bottom = at_y + 18;
                                                    }
                                                    else
                                                    {
                                                        source_rt.top = at_y + 18;
                                                        source_rt.bottom = at_y + tileHeight;
                                                    }
                                                    break;
                                                case 2: /* up */
                                                    if (NO_WALL_END_AUTODRAW(corner_x[corner], corner_y[corner]))
                                                    {
                                                        source_glyph = autodraws[autodraw].source_glyph3; /* S_hwall */
                                                        atile = glyph2tile[source_glyph];
                                                        a_sheet_idx = TILE_SHEET_IDX(atile);
                                                        at_x = TILEBMP_X(atile);
                                                        at_y = TILEBMP_Y(atile);
                                                    }
                                                    if (corner == 0)
                                                    {
                                                        source_rt.left = at_x;
                                                        source_rt.right = at_x + tileWidth / 2;
                                                    }
                                                    else
                                                    {
                                                        source_rt.left = at_x + tileWidth / 2;
                                                        source_rt.right = at_x + tileWidth;
                                                    }
                                                    source_rt.top = at_y;
                                                    source_rt.bottom = source_rt.top + 12;
                                                    break;
                                                case 3: /* down */
                                                    if (NO_WALL_END_AUTODRAW(corner_x[corner], corner_y[corner]))
                                                    {
                                                        source_glyph = autodraws[autodraw].source_glyph3; /* S_hwall */
                                                        atile = glyph2tile[source_glyph];
                                                        a_sheet_idx = TILE_SHEET_IDX(atile);
                                                        at_x = TILEBMP_X(atile);
                                                        at_y = TILEBMP_Y(atile);
                                                    }
                                                    if (corner == 0)
                                                    {
                                                        source_rt.left = at_x;
                                                        source_rt.right = at_x + tileWidth / 2;
                                                    }
                                                    else
                                                    {
                                                        source_rt.left = at_x + tileWidth / 2;
                                                        source_rt.right = at_x + tileWidth;
                                                    }
                                                    source_rt.top = at_y + 12;
                                                    source_rt.bottom = at_y + tileHeight;
                                                    break;
                                                default:
                                                    break;
                                                }

                                                RECT target_rt = { 0 };

                                                if (print_first_directly_to_map)
                                                {
                                                    target_rt.left = rect->left + (int)(((double)data->xBackTile / (double)tileWidth) * (double)(source_rt.left - at_x));
                                                    target_rt.right = rect->left + (int)(((double)data->xBackTile / (double)tileWidth) * (double)(source_rt.right - at_x));
                                                    target_rt.top = rect->top + (int)(((double)data->yBackTile / (double)tileHeight) * (double)(source_rt.top - at_y));
                                                    target_rt.bottom = rect->top + (int)(((double)data->yBackTile / (double)tileHeight) * (double)(source_rt.bottom - at_y));

                                                    (*GetNHApp()->lpfnTransparentBlt)(
                                                        data->backBufferDC, target_rt.left, target_rt.top,
                                                        target_rt.right - target_rt.left, target_rt.bottom - target_rt.top, data->tileDC[a_sheet_idx], source_rt.left,
                                                        source_rt.top, source_rt.right - source_rt.left,
                                                        source_rt.bottom - source_rt.top, TILE_BK_COLOR);
                                                }
                                                else
                                                {
                                                    target_rt.left = source_rt.left - at_x;
                                                    target_rt.right = source_rt.right - at_x;
                                                    target_rt.top = source_rt.top - at_y;
                                                    target_rt.bottom = source_rt.bottom - at_y;

                                                    (*GetNHApp()->lpfnTransparentBlt)(
                                                        hDCcopy, target_rt.left, target_rt.top,
                                                        target_rt.right - target_rt.left, target_rt.bottom - target_rt.top, data->tileDC[a_sheet_idx], source_rt.left,
                                                        source_rt.top, source_rt.right - source_rt.left,
                                                        source_rt.bottom - source_rt.top, TILE_BK_COLOR);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            else if (autodraws[autodraw].draw_type == AUTODRAW_DRAW_BOOKSHELF_CONTENTS && otmp_round)
                            {
                                int num_shelves = 4;
                                int y_to_first_shelf = 49;
                                int shelf_start = 8;
                                int shelf_width = 50;
                                int shelf_height = 10;
                                int shelf_border_height = 2;
                                int shelf_item_width = 5;
                                int src_book_x = 0;
                                int src_book_y = 0;
                                int src_scroll_x = 5;
                                int src_scroll_y = 0;
                                int cnt = 0;
                                int items_per_row = shelf_width / shelf_item_width;
                                
                                for (struct obj* contained_obj = otmp_round->cobj; contained_obj; contained_obj = contained_obj->nobj)
                                {
                                    int src_x = 0, src_y = 0;
                                    int dest_x = 0, dest_y = 0;
                                    if (contained_obj->oclass == SPBOOK_CLASS)
                                    {
                                        src_x = src_book_x;
                                        src_y = src_book_y;
                                    }
                                    else if (contained_obj->oclass == SCROLL_CLASS)
                                    {
                                        src_x = src_scroll_x;
                                        src_y = src_scroll_y;
                                    }
                                    else
                                        continue;

                                    for (int item_idx = 0; item_idx < contained_obj->quan; item_idx++)
                                    {
                                        int item_row = cnt / items_per_row;
                                        int item_xpos = cnt % items_per_row;

                                        if (item_row >= num_shelves)
                                            break;

                                        dest_y = y_to_first_shelf + item_row * (shelf_height + shelf_border_height);
                                        dest_x = shelf_start + item_xpos * shelf_item_width;

                                        int source_glyph = autodraws[autodraw].source_glyph;
                                        int atile = glyph2tile[source_glyph];
                                        int a_sheet_idx = TILE_SHEET_IDX(atile);
                                        int at_x = TILEBMP_X(atile);
                                        int at_y = TILEBMP_Y(atile);

                                        RECT source_rt = { 0 };
                                        source_rt.left = at_x + src_x;
                                        source_rt.right = source_rt.left + shelf_item_width;
                                        source_rt.top = at_y + src_y;
                                        source_rt.bottom = source_rt.top + shelf_height;

                                        RECT target_rt = { 0 };

                                        if (print_first_directly_to_map)
                                        {
                                            target_rt.left = rect->left + dest_x;
                                            target_rt.right = rect->left + dest_x + (int)(((double)data->xBackTile / (double)tileWidth) * (double)(source_rt.right - source_rt.left));
                                            target_rt.top = rect->top + dest_y;
                                            target_rt.bottom = rect->top + dest_y + (int)(((double)data->yBackTile / (double)tileHeight) * (double)(source_rt.bottom - source_rt.top));

                                            (*GetNHApp()->lpfnTransparentBlt)(
                                                data->backBufferDC, target_rt.left, target_rt.top,
                                                target_rt.right - target_rt.left, target_rt.bottom - target_rt.top, data->tileDC[a_sheet_idx], source_rt.left,
                                                source_rt.top, source_rt.right - source_rt.left,
                                                source_rt.bottom - source_rt.top, TILE_BK_COLOR);
                                        }
                                        else
                                        {
                                            target_rt.left = dest_x;
                                            target_rt.right = dest_x + source_rt.right - source_rt.left;
                                            target_rt.top = dest_y;
                                            target_rt.bottom = dest_y + source_rt.bottom - source_rt.top;

                                            (*GetNHApp()->lpfnTransparentBlt)(
                                                hDCcopy, target_rt.left, target_rt.top,
                                                target_rt.right - target_rt.left, target_rt.bottom - target_rt.top, data->tileDC[a_sheet_idx], source_rt.left,
                                                source_rt.top, source_rt.right - source_rt.left,
                                                source_rt.bottom - source_rt.top, TILE_BK_COLOR);
                                        }
                                        cnt++;
                                    }
                                }
                            }
                            else if (autodraws[autodraw].draw_type == AUTODRAW_DRAW_COFFIN_CONTENTS && otmp_round && 0) /* Deactivated for the time being */
                            {
                                int y_to_rack_top = 16;
                                int rack_width = 48;
                                int rack_start = (TILE_X - rack_width) / 2;
                                int rack_height = TILE_Y - y_to_rack_top;
                                int rack_item_spacing = 1;

                                int cnt = 0;

                                for (struct obj* contained_obj = otmp_round->cobj; contained_obj; contained_obj = contained_obj->nobj)
                                {
                                    if (cnt >= 12)
                                        break;

                                    int artidx = contained_obj ? contained_obj->oartifact : 0;
                                    boolean has_floor_tile = !contained_obj ? FALSE : artidx ? has_artifact_floor_tile(artidx) : has_obj_floor_tile(contained_obj);
                                    boolean is_height_clipping = !contained_obj ? FALSE : artidx ? has_artifact_height_clipping(artidx) : has_obj_height_clipping(contained_obj);
                                    int obj_height = get_obj_height(contained_obj);
                                    int src_x = 0, src_y = ((objects[contained_obj->otyp].oc_flags4 & O4_FULL_SIZED_BITMAP) || has_floor_tile ? 0 : TILE_Y / 2);
                                    int dest_x = 0, dest_y = 0;
                                    int item_width = has_floor_tile || is_height_clipping ? TILE_Y / 2 : obj_height ? obj_height : TILE_Y / 2;
                                    int item_height = has_floor_tile || is_height_clipping ? TILE_X : (item_width * TILE_X) / (TILE_Y / 2);
                                    int true_item_width = (has_floor_tile || is_height_clipping) && obj_height ? obj_height : item_width;
                                    int padding = (TILE_Y / 2 - rack_width) / 2;
                                    int vertical_padding = (TILE_X - item_height) / 2;

                                    //if (contained_obj->oclass != AMULET_CLASS && contained_obj->oclass != GEM_CLASS)
                                     //   continue;

                                    if (true_item_width > rack_width)
                                        continue;

                                    int move_px = 2;
                                    int move_idx = cnt > 0  ? (cnt - 1 % 8) + 1 : 0;
                                    int move_amt = cnt > 0 ? (cnt - 1 / 8) + 1 : 0;
                                    int y_move = move_idx < 4 ? -move_px * move_amt : move_idx > 5 ? move_px * move_amt : 0;
                                    int x_move = move_idx == 1 || move_idx == 4 || move_idx == 6 ? -move_px * move_amt : move_idx == 3 || move_idx == 5 || move_idx == 8 ? move_px * move_amt : 0;
                                    dest_y = y_to_rack_top + vertical_padding + y_move;
                                    dest_x = rack_start + (rack_width - item_width) / 2 + x_move;

                                    int source_glyph = obj_to_glyph(contained_obj, rn2);
                                    int atile = glyph2tile[source_glyph];
                                    int a_sheet_idx = TILE_SHEET_IDX(atile);
                                    int at_x = TILEBMP_X(atile);
                                    int at_y = TILEBMP_Y(atile);

                                    RECT source_rt = { 0 };
                                    source_rt.left = at_x + src_x;
                                    source_rt.right = source_rt.left + TILE_X;
                                    source_rt.top = at_y + src_y;
                                    source_rt.bottom = source_rt.top + (objects[contained_obj->otyp].oc_flags4 & O4_FULL_SIZED_BITMAP ? TILE_Y : TILE_Y / 2);

                                    int original_width = source_rt.right - source_rt.left;
                                    int original_height = source_rt.bottom - source_rt.top;
                                    int rotated_width = original_height;
                                    int rotated_height = original_width;

                                    HDC hDCMem = CreateCompatibleDC(data->backBufferDC);

                                    unsigned char* lpBitmapBits;
                                    LONG width = (LONG)original_width;
                                    LONG height = (LONG)original_height;

                                    BITMAPINFO bi;
                                    ZeroMemory(&bi, sizeof(BITMAPINFO));
                                    bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
                                    bi.bmiHeader.biWidth = width;
                                    bi.bmiHeader.biHeight = height;
                                    bi.bmiHeader.biPlanes = 1;
                                    bi.bmiHeader.biBitCount = 32;

                                    HBITMAP bitmap = CreateDIBSection(hDCMem, &bi, DIB_RGB_COLORS, (VOID**)&lpBitmapBits, NULL, 0);
                                    HGDIOBJ oldbmp = SelectObject(hDCMem, bitmap);
                                    StretchBlt(hDCMem, 0, 0, width, height,
                                        data->tileDC[a_sheet_idx], source_rt.left, source_rt.top, width, height, SRCCOPY);

                                    HDC hDCrotate = CreateCompatibleDC(data->backBufferDC);

                                    unsigned char* lpRotatedBitmapBits;
                                    BITMAPINFO bi2;
                                    ZeroMemory(&bi2, sizeof(BITMAPINFO));
                                    bi2.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
                                    bi2.bmiHeader.biWidth = rotated_width;
                                    bi2.bmiHeader.biHeight = rotated_height;
                                    bi2.bmiHeader.biPlanes = 1;
                                    bi2.bmiHeader.biBitCount = 32;

                                    HBITMAP hRotateBitmap = CreateDIBSection(hDCrotate, &bi2, DIB_RGB_COLORS, (VOID**)&lpRotatedBitmapBits, NULL, 0);
                                    HGDIOBJ oldbmp2 = SelectObject(hDCrotate, hRotateBitmap);

                                    int idx, orig_idx, x, y;
                                    long* long_lpRotatedBitmapBits = (long*)lpRotatedBitmapBits;
                                    long* long_lpBitmapBits = (long*)lpBitmapBits;
                                    for (x = 0; x < rotated_width; x++)
                                    {
                                        for (y = 0; y < rotated_height; y++)
                                        {
                                            idx = y * rotated_width + x;
                                            orig_idx = x * rotated_height + y;

                                            long_lpRotatedBitmapBits[idx] = long_lpBitmapBits[orig_idx];
                                        }
                                    }

                                    RECT target_rt = { 0 };

                                    if (print_first_directly_to_map)
                                    {
                                        target_rt.left = rect->left + dest_x;
                                        target_rt.right = rect->left + dest_x + item_width;
                                        target_rt.top = rect->top + dest_y;
                                        target_rt.bottom = rect->top + dest_y + (item_width * rotated_height) / rotated_width;

                                        (*GetNHApp()->lpfnTransparentBlt)(
                                            data->backBufferDC, target_rt.left, target_rt.top,
                                            target_rt.right - target_rt.left, target_rt.bottom - target_rt.top, hDCrotate, 0,
                                            0, rotated_width,
                                            rotated_height, TILE_BK_COLOR);
                                    }
                                    else
                                    {
                                        target_rt.left = dest_x;
                                        target_rt.right = dest_x + item_width;
                                        target_rt.top = dest_y;
                                        target_rt.bottom = dest_y + (item_width * rotated_height) / rotated_width;

                                        (*GetNHApp()->lpfnTransparentBlt)(
                                            hDCcopy, target_rt.left, target_rt.top,
                                            target_rt.right - target_rt.left, target_rt.bottom - target_rt.top, hDCrotate, 0,
                                            0, rotated_width,
                                            rotated_height, TILE_BK_COLOR);
                                    }

                                    SelectObject(hDCMem, oldbmp);
                                    DeleteDC(hDCMem);
                                    SelectObject(hDCrotate, oldbmp2);
                                    DeleteDC(hDCrotate);
                                    DeleteObject(hRotateBitmap);
                                    DeleteObject(bitmap);

                                    cnt++;
                                }
                            }
                            else if (autodraws[autodraw].draw_type == AUTODRAW_DRAW_WEAPON_RACK_CONTENTS && otmp_round)
                            {
                                int y_to_rack_top = 31;
                                int rack_start = 0; /* Assume weapons are drawn reasonably well in the center */
                                int rack_width = 48;
                                int rack_height = TILE_Y - y_to_rack_top;
                                int rack_item_spacing = 6;

                                int cnt = 0;

                                for (struct obj* contained_obj = otmp_round->cobj; contained_obj; contained_obj = contained_obj->nobj)
                                {
                                    int artidx = contained_obj ? contained_obj->oartifact : 0;
                                    boolean has_floor_tile = !contained_obj ? FALSE : artidx ? has_artifact_floor_tile(artidx) : has_obj_floor_tile(contained_obj);
                                    boolean is_obj_clipping = !contained_obj ? FALSE : artidx ? has_artifact_height_clipping(artidx) : has_obj_height_clipping(contained_obj);
                                    int obj_height = get_obj_height(contained_obj);
                                    int src_x = 0, src_y = ((objects[contained_obj->otyp].oc_flags4 & O4_FULL_SIZED_BITMAP) || has_floor_tile ? 0 : TILE_Y / 2);
                                    int dest_x = 0, dest_y = 0;
                                    int item_width = has_floor_tile || is_obj_clipping ? TILE_Y / 2 : obj_height ? obj_height : TILE_Y / 2;
                                    int item_height = has_floor_tile || is_obj_clipping ? TILE_X : (item_width * TILE_X) / (TILE_Y / 2);
                                    int padding = (TILE_Y / 2 - item_width) / 2;
                                    int vertical_padding = (TILE_X - item_height) / 2;
                                    if (contained_obj->oclass != WEAPON_CLASS)
                                        continue;

                                    int item_xpos = cnt / 2 * rack_item_spacing;
                                    if (item_xpos >= rack_width / 2)
                                        break;

                                    dest_y = y_to_rack_top + vertical_padding;
                                    dest_x = cnt % 2 == 0 ? rack_start + item_xpos + padding : TILE_X - item_width - rack_start - item_xpos - padding;

                                    int source_glyph = obj_to_glyph(contained_obj, rn2);
                                    int atile = glyph2tile[source_glyph];
                                    int a_sheet_idx = TILE_SHEET_IDX(atile);
                                    int at_x = TILEBMP_X(atile);
                                    int at_y = TILEBMP_Y(atile);

                                    RECT source_rt = { 0 };
                                    source_rt.left = at_x + src_x;
                                    source_rt.right = source_rt.left + TILE_X;
                                    source_rt.top = at_y + src_y;
                                    source_rt.bottom = source_rt.top + (objects[contained_obj->otyp].oc_flags4 & O4_FULL_SIZED_BITMAP ? TILE_Y : TILE_Y / 2);

                                    int original_width = source_rt.right - source_rt.left;
                                    int original_height = source_rt.bottom - source_rt.top;
                                    int rotated_width = original_height;
                                    int rotated_height = original_width;

                                    HDC hDCMem = CreateCompatibleDC(data->backBufferDC);

                                    unsigned char* lpBitmapBits;
                                    LONG width = (LONG)original_width;
                                    LONG height = (LONG)original_height;

                                    BITMAPINFO bi;
                                    ZeroMemory(&bi, sizeof(BITMAPINFO));
                                    bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
                                    bi.bmiHeader.biWidth = width;
                                    bi.bmiHeader.biHeight = height;
                                    bi.bmiHeader.biPlanes = 1;
                                    bi.bmiHeader.biBitCount = 32;

                                    HBITMAP bitmap = CreateDIBSection(hDCMem, &bi, DIB_RGB_COLORS, (VOID**)&lpBitmapBits, NULL, 0);
                                    HGDIOBJ oldbmp = SelectObject(hDCMem, bitmap);
                                    StretchBlt(hDCMem, 0, 0, width, height,
                                        data->tileDC[a_sheet_idx], source_rt.left, source_rt.top, width, height, SRCCOPY);

                                    HDC hDCrotate = CreateCompatibleDC(data->backBufferDC);

                                    unsigned char* lpRotatedBitmapBits;
                                    BITMAPINFO bi2;
                                    ZeroMemory(&bi2, sizeof(BITMAPINFO));
                                    bi2.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
                                    bi2.bmiHeader.biWidth = rotated_width;
                                    bi2.bmiHeader.biHeight = rotated_height;
                                    bi2.bmiHeader.biPlanes = 1;
                                    bi2.bmiHeader.biBitCount = 32;

                                    HBITMAP hRotateBitmap = CreateDIBSection(hDCrotate, &bi2, DIB_RGB_COLORS, (VOID**)&lpRotatedBitmapBits, NULL, 0);
                                    HGDIOBJ oldbmp2 = SelectObject(hDCrotate, hRotateBitmap);

                                    int idx, orig_idx, x, y;
                                    long* long_lpRotatedBitmapBits = (long*)lpRotatedBitmapBits;
                                    long* long_lpBitmapBits = (long*)lpBitmapBits;
                                    for (x = 0; x < rotated_width; x++)
                                    {
                                        for (y = 0; y < rotated_height; y++)
                                        {
                                            idx = y * rotated_width + x;
                                            orig_idx = x * rotated_height + y;

                                            long_lpRotatedBitmapBits[idx] = long_lpBitmapBits[orig_idx];
                                        }
                                    }

                                    RECT target_rt = { 0 };

                                    if (print_first_directly_to_map)
                                    {
                                        target_rt.left = rect->left + dest_x;
                                        target_rt.right = rect->left + dest_x + item_width;
                                        target_rt.top = rect->top + dest_y;
                                        target_rt.bottom = rect->top + dest_y + (item_width * rotated_height) / rotated_width;

                                        (*GetNHApp()->lpfnTransparentBlt)(
                                            data->backBufferDC, target_rt.left, target_rt.top,
                                            target_rt.right - target_rt.left, target_rt.bottom - target_rt.top, hDCrotate, 0,
                                            0, rotated_width,
                                            rotated_height, TILE_BK_COLOR);
                                    }
                                    else
                                    {
                                        target_rt.left = dest_x;
                                        target_rt.right = dest_x + item_width;
                                        target_rt.top = dest_y;
                                        target_rt.bottom = dest_y + (item_width * rotated_height) / rotated_width;

                                        (*GetNHApp()->lpfnTransparentBlt)(
                                            hDCcopy, target_rt.left, target_rt.top,
                                            target_rt.right - target_rt.left, target_rt.bottom - target_rt.top, hDCrotate, 0,
                                            0, rotated_width,
                                            rotated_height, TILE_BK_COLOR);
                                    }

                                    SelectObject(hDCMem, oldbmp);
                                    DeleteDC(hDCMem);
                                    SelectObject(hDCrotate, oldbmp2);
                                    DeleteDC(hDCrotate);
                                    DeleteObject(hRotateBitmap);
                                    DeleteObject(bitmap);

                                    cnt++;
                                }
                            }
                            else if (autodraws[autodraw].draw_type == AUTODRAW_DRAW_CANDELABRUM_CANDLES && otmp_round)
                            {
                                int y_start = dest_top_added;
                                int x_start = dest_left_added;
                                int x_padding = 13;
                                int item_width = 6;
                                int item_height = 13;
                                int src_unlit_x = 0;
                                int src_unlit_y = 10;
                                int src_lit_x = 6 * (1 + (int)autodraws[autodraw].flags);
                                int src_lit_y = 10;
                                int cnt = 0;

                                for (int cidx = 0; cidx < min(7, otmp_round->special_quality); cidx++)
                                {
                                    int src_x = 0, src_y = 0;
                                    int dest_x = 0, dest_y = 0;
                                    if (otmp_round->lamplit)
                                    {
                                        src_x = src_lit_x;
                                        src_y = src_lit_y;
                                    }
                                    else
                                    {
                                        src_x = src_unlit_x;
                                        src_y = src_unlit_y;
                                    }

                                    int item_xpos = cnt;

                                    dest_y = y_start;
                                    dest_x = x_start + (int)((double)(x_padding + item_xpos * item_width) * obj_scaling_factor);

                                    int source_glyph = autodraws[autodraw].source_glyph;
                                    int atile = glyph2tile[source_glyph];
                                    int a_sheet_idx = TILE_SHEET_IDX(atile);
                                    int at_x = TILEBMP_X(atile);
                                    int at_y = TILEBMP_Y(atile);

                                    RECT source_rt = { 0 };
                                    source_rt.left = at_x + src_x;
                                    source_rt.right = source_rt.left + item_width;
                                    source_rt.top = at_y + src_y;
                                    source_rt.bottom = source_rt.top + item_height;

                                    RECT target_rt = { 0 };

                                    if (print_first_directly_to_map)
                                    {
                                        target_rt.left = rect->left + (LONG)dest_x;
                                        target_rt.right = rect->left + (LONG)dest_x + (LONG)(((double)data->xBackTile / (double)tileWidth) * obj_scaling_factor * (double)(source_rt.right - source_rt.left));
                                        target_rt.top = rect->top + (LONG)dest_y;
                                        target_rt.bottom = rect->top + (LONG)dest_y + (LONG)(((double)data->yBackTile / (double)tileHeight) * obj_scaling_factor * (double)(source_rt.bottom - source_rt.top));

                                        (*GetNHApp()->lpfnTransparentBlt)(
                                            data->backBufferDC, target_rt.left, target_rt.top,
                                            target_rt.right - target_rt.left, target_rt.bottom - target_rt.top, data->tileDC[a_sheet_idx], source_rt.left,
                                            source_rt.top, source_rt.right - source_rt.left,
                                            source_rt.bottom - source_rt.top, TILE_BK_COLOR);
                                    }
                                    else
                                    {
                                        target_rt.left = dest_x;
                                        target_rt.right = (LONG)dest_x + (LONG)((double)(source_rt.right - source_rt.left) * obj_scaling_factor);
                                        target_rt.top = dest_y;
                                        target_rt.bottom = (LONG)dest_y + (LONG)((double)(source_rt.bottom - source_rt.top) * obj_scaling_factor);

                                        (*GetNHApp()->lpfnTransparentBlt)(
                                            hDCcopy, target_rt.left, target_rt.top,
                                            target_rt.right - target_rt.left, target_rt.bottom - target_rt.top, data->tileDC[a_sheet_idx], source_rt.left,
                                            source_rt.top, source_rt.right - source_rt.left,
                                            source_rt.bottom - source_rt.top, TILE_BK_COLOR);
                                    }
                                    cnt++;
                                }
                            }
                            else if (autodraws[autodraw].draw_type == AUTODRAW_DRAW_LARGE_FIVE_BRANCHED_CANDELABRUM_CANDLES && otmp_round)
                            {
                                int y_start = dest_top_added;
                                int x_start = dest_left_added;
                                int x_padding = 13;
                                int item_width = 9;
                                int item_height = 31;
                                int src_unlit_x = 0;
                                int src_unlit_y = 0;
                                int src_lit_x = 9 * (1 + (int)autodraws[autodraw].flags);
                                int src_lit_y = 0;
                                int cnt = 0;

                                for (int cidx = 0; cidx < min(objects[otmp_round->otyp].oc_special_quality, otmp_round->special_quality); cidx++)
                                {
                                    int src_x = 0, src_y = 0;
                                    int dest_x = 0, dest_y = 0;
                                    if (otmp_round->lamplit)
                                    {
                                        src_x = src_lit_x;
                                        src_y = src_lit_y;
                                    }
                                    else
                                    {
                                        src_x = src_unlit_x;
                                        src_y = src_unlit_y;
                                    }

                                    switch (cidx)
                                    {
                                    case 0:
                                        dest_x = x_start + (int)((double)(29) * obj_scaling_factor);
                                        dest_y = y_start + (int)((double)(0) * obj_scaling_factor);
                                        break;
                                    case 1:
                                        dest_x = x_start + (int)((double)(18) * obj_scaling_factor);
                                        dest_y = y_start + (int)((double)(4) * obj_scaling_factor);
                                        break;
                                    case 2:
                                        dest_x = x_start + (int)((double)(40) * obj_scaling_factor);
                                        dest_y = y_start + (int)((double)(3) * obj_scaling_factor);
                                        break;
                                    case 3:
                                        dest_x = x_start + (int)((double)(8) * obj_scaling_factor);
                                        dest_y = y_start + (int)((double)(14) * obj_scaling_factor);
                                        break;
                                    case 4:
                                        dest_x = x_start + (int)((double)(50) * obj_scaling_factor);
                                        dest_y = y_start + (int)((double)(15) * obj_scaling_factor);
                                        break;
                                    default:
                                        break;
                                    }

                                    int source_glyph = autodraws[autodraw].source_glyph;
                                    int atile = glyph2tile[source_glyph];
                                    int a_sheet_idx = TILE_SHEET_IDX(atile);
                                    int at_x = TILEBMP_X(atile);
                                    int at_y = TILEBMP_Y(atile);

                                    RECT source_rt = { 0 };
                                    source_rt.left = at_x + src_x;
                                    source_rt.right = source_rt.left + item_width;
                                    source_rt.top = at_y + src_y;
                                    source_rt.bottom = source_rt.top + item_height;

                                    RECT target_rt = { 0 };

                                    if (print_first_directly_to_map)
                                    {
                                        target_rt.left = rect->left + (LONG)dest_x;
                                        target_rt.right = rect->left + (LONG)dest_x + (LONG)(((double)data->xBackTile / (double)tileWidth) * obj_scaling_factor * (double)(source_rt.right - source_rt.left));
                                        target_rt.top = rect->top + (LONG)dest_y;
                                        target_rt.bottom = rect->top + (LONG)dest_y + (LONG)(((double)data->yBackTile / (double)tileHeight) * obj_scaling_factor * (double)(source_rt.bottom - source_rt.top));

                                        (*GetNHApp()->lpfnTransparentBlt)(
                                            data->backBufferDC, target_rt.left, target_rt.top,
                                            target_rt.right - target_rt.left, target_rt.bottom - target_rt.top, data->tileDC[a_sheet_idx], source_rt.left,
                                            source_rt.top, source_rt.right - source_rt.left,
                                            source_rt.bottom - source_rt.top, TILE_BK_COLOR);
                                    }
                                    else
                                    {
                                        target_rt.left = dest_x;
                                        target_rt.right = (LONG)dest_x + (LONG)((double)(source_rt.right - source_rt.left) * obj_scaling_factor);
                                        target_rt.top = dest_y;
                                        target_rt.bottom = (LONG)dest_y + (LONG)((double)(source_rt.bottom - source_rt.top) * obj_scaling_factor);

                                        (*GetNHApp()->lpfnTransparentBlt)(
                                            hDCcopy, target_rt.left, target_rt.top,
                                            target_rt.right - target_rt.left, target_rt.bottom - target_rt.top, data->tileDC[a_sheet_idx], source_rt.left,
                                            source_rt.top, source_rt.right - source_rt.left,
                                            source_rt.bottom - source_rt.top, TILE_BK_COLOR);
                                    }
                                    cnt++;
                                }
                            }
                            else if (autodraws[autodraw].draw_type == AUTODRAW_DRAW_JAR_CONTENTS && otmp_round)
                            {
                                int max_charge = get_obj_max_charge(otmp_round);
                                double fill_percentage = (max_charge > 0 ? (double)otmp_round->charges / (double)max_charge : 0.0);

                                if (fill_percentage > 0.0)
                                {
                                    HDC hDCjar = CreateCompatibleDC(data->tileDC[0]);

                                    unsigned char* lpBitmapBitsJar;
                                    int jar_width = tileWidth;
                                    int jar_height = tileHeight / 2;

                                    BITMAPINFO binfo_jar;
                                    ZeroMemory(&binfo_jar, sizeof(BITMAPINFO));
                                    binfo_jar.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
                                    binfo_jar.bmiHeader.biWidth = jar_width;
                                    binfo_jar.bmiHeader.biHeight = jar_height;
                                    binfo_jar.bmiHeader.biPlanes = 1;
                                    binfo_jar.bmiHeader.biBitCount = 32;

                                    HBITMAP newhBmp_jar = CreateDIBSection(hDCjar, &binfo_jar, DIB_RGB_COLORS, (VOID**)&lpBitmapBitsJar, NULL, 0);
                                    HGDIOBJ oldbmp_jar = SelectObject(hDCjar, newhBmp_jar);

                                    LONG width = GetNHApp()->mapTile_X;
                                    LONG height = GetNHApp()->mapTile_Y;

                                    /* Draw transparency color background */
                                    int jar_pitch = 4 * jar_width;
                                    int jar_idx;
                                    for (int x = 0; x < jar_width; x++)
                                    {
                                        for (int y = 0; y < jar_height; y++)
                                        {
                                            jar_idx = y * jar_pitch;
                                            jar_idx += x * 4;

                                            lpBitmapBitsJar[jar_idx + 0] = TILE_BK_COLOR_BLUE;  // blue
                                            lpBitmapBitsJar[jar_idx + 1] = TILE_BK_COLOR_GREEN; // green
                                            lpBitmapBitsJar[jar_idx + 2] = TILE_BK_COLOR_RED;  // red 
                                        }
                                    }

                                    int dest_x = 0, dest_y = 0;
                                    dest_y = dest_top_added;
                                    dest_x = dest_left_added;

                                    int source_glyph = autodraws[autodraw].source_glyph;
                                    int atile = glyph2tile[source_glyph];
                                    int a_sheet_idx = TILE_SHEET_IDX(atile);
                                    int at_x = TILEBMP_X(atile);
                                    int at_y = TILEBMP_Y(atile);

                                    int source_glyph2 = autodraws[autodraw].source_glyph2;
                                    int atile2 = glyph2tile[source_glyph2];
                                    int a2_sheet_idx = TILE_SHEET_IDX(atile);
                                    int a2t_x = TILEBMP_X(atile2);
                                    int a2t_y = TILEBMP_Y(atile2);

                                    RECT source_rt = { 0 };
                                    RECT target_rt = { 0 };

                                    /* First, background */
                                    //if (1) // Always true
                                    {
                                        source_rt.left = at_x;
                                        source_rt.right = source_rt.left + jar_width;
                                        source_rt.top = at_y;
                                        source_rt.bottom = source_rt.top + jar_height;

                                        target_rt.left = 0;
                                        target_rt.right = jar_width;
                                        target_rt.top = 0;
                                        target_rt.bottom = jar_height;

                                        (*GetNHApp()->lpfnTransparentBlt)(
                                            hDCjar, target_rt.left, target_rt.top,
                                            target_rt.right - target_rt.left, target_rt.bottom - target_rt.top, data->tileDC[a_sheet_idx], source_rt.left,
                                            source_rt.top, source_rt.right - source_rt.left,
                                            source_rt.bottom - source_rt.top, TILE_BK_COLOR);
                                    }
                                

                                    /* Second, contents */
                                    //if (1) // Always true
                                    {
                                        /* Create copy of the contents tile */
                                        source_rt.left = at_x;
                                        source_rt.right = source_rt.left + tileWidth;
                                        source_rt.top = at_y + tileHeight / 2;
                                        source_rt.bottom = source_rt.top + tileHeight / 2;
                                        int width = source_rt.right - source_rt.left;
                                        int height = source_rt.bottom - source_rt.top;

                                        HDC hDCtemplate = CreateCompatibleDC(data->tileDC[0]);

                                        unsigned char* lpBitmapBitsTemplate;

                                        BITMAPINFO binfo_st;
                                        ZeroMemory(&binfo_st, sizeof(BITMAPINFO));
                                        binfo_st.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
                                        binfo_st.bmiHeader.biWidth = width;
                                        binfo_st.bmiHeader.biHeight = height;
                                        binfo_st.bmiHeader.biPlanes = 1;
                                        binfo_st.bmiHeader.biBitCount = 32;

                                        HBITMAP newhBmp_st = CreateDIBSection(hDCtemplate, &binfo_st, DIB_RGB_COLORS, (VOID**)&lpBitmapBitsTemplate, NULL, 0);
                                        HGDIOBJ oldbmp_st = SelectObject(hDCtemplate, newhBmp_st);
                                        StretchBlt(hDCtemplate, 0, 0, width, height,
                                            data->tileDC[a_sheet_idx], source_rt.left, source_rt.top, width, height, SRCCOPY);

                                        /* Color */
                                        unsigned long draw_color = autodraws[autodraw].parameter1;
                                        unsigned char blue = (&((unsigned char)draw_color))[0];
                                        unsigned char green = (&((unsigned char)draw_color))[1];
                                        unsigned char red = (&((unsigned char)draw_color))[2];

                                        /* Draw */
                                        int pitch = 4 * width; // 4 bytes per pixel but if not 32 bit, round pitch up to multiple of 4
                                        int idx, x, y;
                                        double semi_transparency = 0.5;

                                        for (x = 0; x < width; x++)
                                        {
                                            for (y = 0; y < height; y++)
                                            {
                                                idx = y * pitch;
                                                idx += x * 4;

                                                if (lpBitmapBitsTemplate[idx + 0] == TILE_BK_COLOR_BLUE && lpBitmapBitsTemplate[idx + 1] == TILE_BK_COLOR_GREEN && lpBitmapBitsTemplate[idx + 2] == TILE_BK_COLOR_RED)
                                                    continue;
                                                else if (lpBitmapBitsTemplate[idx + 0] == 255 && lpBitmapBitsTemplate[idx + 1] == 255 && lpBitmapBitsTemplate[idx + 2] == 255)
                                                {
                                                    lpBitmapBitsTemplate[idx + 0] = blue;  // blue
                                                    lpBitmapBitsTemplate[idx + 1] = green;  // green
                                                    lpBitmapBitsTemplate[idx + 2] = red;  // red 
                                                }
                                                else
                                                {
                                                    double color_mult = (lpBitmapBitsTemplate[idx + 0] + lpBitmapBitsTemplate[idx + 1] + lpBitmapBitsTemplate[idx + 2]) / (3.0 * 255.0);
                                                    lpBitmapBitsTemplate[idx + 0] = (unsigned char)(((double)blue) * (color_mult));  // blue
                                                    lpBitmapBitsTemplate[idx + 1] = (unsigned char)(((double)green) * (color_mult));  // green
                                                    lpBitmapBitsTemplate[idx + 2] = (unsigned char)(((double)red) * (color_mult));  // red 
                                                }
                                            }
                                        }

                                        /* Bottom contents */
                                        int bottom_x = 21;
                                        int bottom_y = 38;
                                        int bottom_width = 21;
                                        int bottom_height = 10;

                                        int bottom_tx = 21;
                                        int bottom_ty = 35;

                                        source_rt.left = bottom_x;
                                        source_rt.right = source_rt.left + bottom_width;
                                        source_rt.top = bottom_y;
                                        source_rt.bottom = source_rt.top + bottom_height;

                                        target_rt.left = bottom_tx;
                                        target_rt.right = bottom_tx + bottom_width;
                                        target_rt.top = bottom_ty;
                                        target_rt.bottom = bottom_ty + bottom_height;

                                        (*GetNHApp()->lpfnTransparentBlt)(
                                            hDCjar, target_rt.left, target_rt.top,
                                            target_rt.right - target_rt.left, target_rt.bottom - target_rt.top, hDCtemplate, source_rt.left,
                                            source_rt.top, source_rt.right - source_rt.left,
                                            source_rt.bottom - source_rt.top, TILE_BK_COLOR);

                                        int full_y = 11;
                                        int empty_y = 35;
                                        int fill_pixel_top = (int)((double)(empty_y - full_y) * (1.0 - fill_percentage)) + full_y;
                                        int fill_pixels = empty_y - fill_pixel_top;
                                        if (fill_pixels > 0)
                                        {
                                            /* Middle contents */
                                            int middle_x = 21;
                                            int middle_y = 15;
                                            int middle_width = 21;
                                            int middle_height = 17;

                                            int middle_tx = 21;
                                            int middle_ty = fill_pixel_top + 4;
                                            int middle_twidth = middle_width;
                                            int middle_theight = fill_pixels + 1;

                                            source_rt.left = middle_x;
                                            source_rt.right = source_rt.left + middle_width;
                                            source_rt.top = middle_y;
                                            source_rt.bottom = source_rt.top + middle_height;

                                            target_rt.left = middle_tx;
                                            target_rt.right = middle_tx + middle_twidth;
                                            target_rt.top = middle_ty;
                                            target_rt.bottom = middle_ty + middle_theight;

                                            (*GetNHApp()->lpfnTransparentBlt)(
                                                hDCjar, target_rt.left, target_rt.top,
                                                target_rt.right - target_rt.left, target_rt.bottom - target_rt.top, hDCtemplate, source_rt.left,
                                                source_rt.top, source_rt.right - source_rt.left,
                                                source_rt.bottom - source_rt.top, TILE_BK_COLOR);

                                            /* Top contents */
                                            int top_x = 21;
                                            int top_y = 0;
                                            int top_width = 21;
                                            int top_height = 8;

                                            int top_tx = 21;
                                            int top_ty = fill_pixel_top;

                                            source_rt.left = top_x;
                                            source_rt.right = source_rt.left + top_width;
                                            source_rt.top = top_y;
                                            source_rt.bottom = source_rt.top + top_height;

                                            target_rt.left = top_tx;
                                            target_rt.right = top_tx + top_width;
                                            target_rt.top = top_ty;
                                            target_rt.bottom = top_ty + top_height;

                                            (*GetNHApp()->lpfnTransparentBlt)(
                                                hDCjar, target_rt.left, target_rt.top,
                                                target_rt.right - target_rt.left, target_rt.bottom - target_rt.top, hDCtemplate, source_rt.left,
                                                source_rt.top, source_rt.right - source_rt.left,
                                                source_rt.bottom - source_rt.top, TILE_BK_COLOR);
                                        }

                                        SelectObject(hDCtemplate, oldbmp_st);
                                        DeleteDC(hDCtemplate);
                                        DeleteObject(newhBmp_st);
                                    }

                                    /* Third, transparent foreground */
                                    //if (1) // Always true
                                    {
                                        source_rt.left = a2t_x;
                                        source_rt.right = source_rt.left + tileWidth;
                                        source_rt.top = a2t_y;
                                        source_rt.bottom = source_rt.top + tileHeight / 2;

                                        /* Create copy of background */
                                        HDC hDCMem = CreateCompatibleDC(data->backBufferDC);

                                        unsigned char* lpBitmapBits;
                                        LONG width = jar_width;
                                        LONG height = jar_height;

                                        BITMAPINFO bi;
                                        ZeroMemory(&bi, sizeof(BITMAPINFO));
                                        bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
                                        bi.bmiHeader.biWidth = width;
                                        bi.bmiHeader.biHeight = height;
                                        bi.bmiHeader.biPlanes = 1;
                                        bi.bmiHeader.biBitCount = 32;

                                        HBITMAP bitmap = CreateDIBSection(hDCMem, &bi, DIB_RGB_COLORS, (VOID**)&lpBitmapBits, NULL, 0);
                                        HGDIOBJ oldbmp = SelectObject(hDCMem, bitmap);

                                        StretchBlt(hDCMem, 0, 0, width, height,
                                            hDCjar, (hflip_glyph ? jar_width - 1 : 0), (vflip_glyph ? jar_height - 1 : 0), hmultiplier * jar_width, vmultiplier * jar_height, SRCCOPY);

                                        /* Create copy of tile to be drawn */
                                        HDC hDCsemitransparent = CreateCompatibleDC(data->backBufferDC);

                                        unsigned char* lpBitmapBitsSemitransparent;

                                        BITMAPINFO binfo_st;
                                        ZeroMemory(&binfo_st, sizeof(BITMAPINFO));
                                        binfo_st.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
                                        binfo_st.bmiHeader.biWidth = width;
                                        binfo_st.bmiHeader.biHeight = height;
                                        binfo_st.bmiHeader.biPlanes = 1;
                                        binfo_st.bmiHeader.biBitCount = 32;

                                        HBITMAP newhBmp_st = CreateDIBSection(hDCsemitransparent, &binfo_st, DIB_RGB_COLORS, (VOID**)&lpBitmapBitsSemitransparent, NULL, 0);
                                        HGDIOBJ oldbmp_st = SelectObject(hDCsemitransparent, newhBmp_st);
                                        StretchBlt(hDCsemitransparent, 0, 0, width, height,
                                            data->tileDC[a2_sheet_idx], source_rt.left, source_rt.top, source_rt.right - source_rt.left, source_rt.bottom - source_rt.top, SRCCOPY);

                                        /* Draw */
                                        int pitch = 4 * width; // 4 bytes per pixel but if not 32 bit, round pitch up to multiple of 4
                                        int idx, x, y;
                                        double semi_transparency = 0.70;

                                        for (x = 0; x < width; x++)
                                        {
                                            for (y = 0; y < height; y++)
                                            {
                                                idx = y * pitch;
                                                idx += x * 4;

                                                if (lpBitmapBitsSemitransparent[idx + 0] == TILE_BK_COLOR_BLUE && lpBitmapBitsSemitransparent[idx + 1] == TILE_BK_COLOR_GREEN && lpBitmapBitsSemitransparent[idx + 2] == TILE_BK_COLOR_RED)
                                                    continue;

                                                lpBitmapBitsSemitransparent[idx + 0] = (unsigned char)(((double)lpBitmapBitsSemitransparent[idx + 0]) * (1.0 - semi_transparency) + ((double)lpBitmapBits[idx + 0]) * (semi_transparency));  // blue
                                                lpBitmapBitsSemitransparent[idx + 1] = (unsigned char)(((double)lpBitmapBitsSemitransparent[idx + 1]) * (1.0 - semi_transparency) + ((double)lpBitmapBits[idx + 1]) * (semi_transparency));  // green
                                                lpBitmapBitsSemitransparent[idx + 2] = (unsigned char)(((double)lpBitmapBitsSemitransparent[idx + 2]) * (1.0 - semi_transparency) + ((double)lpBitmapBits[idx + 2]) * (semi_transparency));  // red 
                                            }
                                        }

                                        (*GetNHApp()->lpfnTransparentBlt)(
                                            hDCjar, 0, 0, jar_width, jar_height, hDCsemitransparent,
                                            (hflip_glyph ? tileWidth - 1 : 0), (vflip_glyph ? tileWidth - 1 : 0), hmultiplier * width,
                                            vmultiplier * height, TILE_BK_COLOR);

                                        SelectObject(hDCsemitransparent, oldbmp_st);
                                        DeleteDC(hDCsemitransparent);
                                        DeleteObject(newhBmp_st);

                                        SelectObject(hDCMem, oldbmp);
                                        DeleteDC(hDCMem);
                                        DeleteObject(bitmap);
                                    }


                                    /* Fourth, opaque foreground */
                                    //if (1) // Always true
                                    {
                                        source_rt.left = a2t_x;
                                        source_rt.right = source_rt.left + tileWidth;
                                        source_rt.top = a2t_y + tileHeight / 2;
                                        source_rt.bottom = source_rt.top + tileHeight / 2;

                                        target_rt.left = 0;
                                        target_rt.right = jar_width;
                                        target_rt.top = 0;
                                        target_rt.bottom = jar_height;

                                        (*GetNHApp()->lpfnTransparentBlt)(
                                            hDCjar, target_rt.left, target_rt.top,
                                            target_rt.right - target_rt.left, target_rt.bottom - target_rt.top, data->tileDC[a2_sheet_idx], source_rt.left,
                                            source_rt.top, source_rt.right - source_rt.left,
                                            source_rt.bottom - source_rt.top, TILE_BK_COLOR);
                                    }


                                    /* Finally, print jar to the map */
                                    target_rt.left = (LONG)dest_x;
                                    target_rt.right = (LONG)dest_x + (LONG)((double)(jar_width) * obj_scaling_factor);
                                    target_rt.top = (LONG)dest_y;
                                    target_rt.bottom = (LONG)dest_y + (LONG)((double)(jar_height) * obj_scaling_factor);

                                    if (print_first_directly_to_map)
                                    {
                                        target_rt.left += rect->left;
                                        target_rt.right += rect->left;
                                        target_rt.top += rect->top;
                                        target_rt.bottom += rect->top;

                                        (*GetNHApp()->lpfnTransparentBlt)(
                                            data->backBufferDC, target_rt.left, target_rt.top,
                                            target_rt.right - target_rt.left, target_rt.bottom - target_rt.top, 
                                            hDCjar, 0, 0, jar_width, jar_height, TILE_BK_COLOR);
                                    }
                                    else
                                    {
                                        (*GetNHApp()->lpfnTransparentBlt)(
                                            hDCcopy, target_rt.left, target_rt.top,
                                            target_rt.right - target_rt.left, target_rt.bottom - target_rt.top, 
                                            hDCjar, 0, 0, jar_width, jar_height, TILE_BK_COLOR);
                                    }

                                    SelectObject(hDCjar, oldbmp_jar);
                                    DeleteDC(hDCjar);
                                    DeleteObject(newhBmp_jar);

                                }
                            }
                            else if (autodraws[autodraw].draw_type == AUTODRAW_DRAW_FIGURINE && otmp_round)
                            {
                                /* Implement figurine drawing here, especially for big enlarged statues */
                                //int nglyph = otmp->corpsenm + (is_female_corpse_or_statue(otmp) ? GLYPH_FEMALE_STATUE_OFF : GLYPH_STATUE_OFF);

                            }
                            else if (autodraw_u_punished || (((autodraws[autodraw].draw_type == AUTODRAW_DRAW_CHAIN /* && otmp_round == uchain */) || (autodraws[autodraw].draw_type == AUTODRAW_DRAW_BALL /* && otmp_round == uball*/))))
                            {
                                if (uchain && uball && cansee(enl_i, enl_j))
                                {
                                    boolean is_chain = (autodraws[autodraw].draw_type == AUTODRAW_DRAW_CHAIN);
                                    xchar chain_x = 0;
                                    xchar chain_y = 0;
                                    boolean res_chain = get_obj_location(uchain, &chain_x, &chain_y, BURIED_TOO);
                                    xchar ball_x = 0;
                                    xchar ball_y = 0;
                                    boolean res_ball = get_obj_location(uball, &ball_x, &ball_y, BURIED_TOO);
                                    xchar u_x = u.ux;
                                    xchar u_y = u.uy;

                                    int chain_u_dx = (int)(u_x - chain_x);
                                    int chain_u_dy = (int)(u_y - chain_y);
                                    int chain_ball_dx = (int)(ball_x - chain_x);
                                    int chain_ball_dy = (int)(ball_y - chain_y);
                                    int u_ball_dx = (int)(u_x - chain_x);
                                    int u_ball_dy = (int)(u_y - chain_y);

                                    int source_glyph = autodraw_u_punished || autodraw == 0 ? ITEM_AUTODRAW_GRAPHICS + GLYPH_UI_TILE_OFF : autodraws[autodraw].source_glyph;
                                    int dir_idx = autodraws[autodraw].flags;
                                    int atile = glyph2tile[source_glyph];
                                    int a_sheet_idx = TILE_SHEET_IDX(atile);
                                    int at_x = TILEBMP_X(atile);
                                    int at_y = TILEBMP_Y(atile);
                                    double scale = (double)(rect->bottom - rect->top) / (double)tileHeight;

                                    for (int n = 0; n < 2; n++)
                                    {
                                        int relevant_dx = autodraw_u_punished ? sgn(n == 0 ? -chain_u_dx : 0) : is_chain ? sgn(n == 0 ? chain_u_dx : chain_ball_dx) : sgn(n == 0 ? 0 : -chain_ball_dx);
                                        int relevant_dy = autodraw_u_punished ? sgn(n == 0 ? -chain_u_dy : 0) : is_chain ? sgn(n == 0 ? chain_u_dy : chain_ball_dy) : sgn(n == 0 ? 1 : -chain_ball_dy);
                                        boolean hflip_link = !((relevant_dx > 0) != (relevant_dy > 0));
                                        boolean vflip_link = FALSE;
                                        int link_source_width = 16;
                                        int link_source_height = 16;
                                        double link_diff_x = relevant_dx && relevant_dy ? 5.35 : 10.0;
                                        double link_diff_y = relevant_dx && relevant_dy ? link_diff_x * 1.5 : 10.0;
                                        int mid_x = tileWidth / 2;
                                        int mid_y = tileHeight / 2;
                                        int dist_x = relevant_dx > 0 ? tileWidth - mid_x : mid_x;
                                        int dist_y = relevant_dy > 0 ? tileHeight - mid_y : mid_y;
                                        int links = (int) (relevant_dx && !relevant_dy && 0 ? (double)(dist_x - link_source_width / 2) / (double)link_diff_x :
                                            !relevant_dx && relevant_dy && 0 ? (double)(dist_y - link_source_height / 2) / (double)link_diff_y :
                                            2 + 1 +(int)min((double)(dist_y - link_source_height / 2) / (double)link_diff_y, (double)(dist_x - link_source_width / 2) / (double)link_diff_x)
                                            );

                                        if (!is_chain && !autodraw_u_punished && n == 0 && links > 1)
                                            links = 1;
                                        else if (autodraw_u_punished && n == 1 && links > 1)
                                            links = 1;

                                        if (dir_idx == 0)
                                        {
                                            if (relevant_dx || relevant_dy)
                                            {
                                                for (int m = 0; m < links; m++)
                                                {
                                                    boolean used_hflip_link = hflip_link;
                                                    if (m >= links && (relevant_dx < 0 || relevant_dy < 0))
                                                        used_hflip_link = !((-relevant_dx > 0) != (-relevant_dy > 0));

                                                    int source_width = link_source_width;
                                                    int source_height = link_source_height;
                                                    int within_tile_source_x = relevant_dx && relevant_dy ? 32 : relevant_dy ? 16 : 0;
                                                    int within_tile_source_y = 23 + ((m % 2) == 1 ? link_source_height : 0);
                                                    int target_left_added = (rect->right - rect->left) / 2 - (int)((double)source_width * scale / 2.0) + (int)(((double)relevant_dx * link_diff_x * (double)m) * scale);
                                                    int target_top_added = (rect->bottom - rect->top) / 2 - (int)((double)source_height * scale / 2.0) + (int)(((double)relevant_dy * link_diff_y * (double)m) * scale);
                                                    if (target_left_added < 0)
                                                    {
                                                        /* Cut off from left ==> Move source x right and reduce width to fix, flipped: just reduce width */
                                                        if(!used_hflip_link)
                                                            within_tile_source_x += (int)((double)-target_left_added / scale);

                                                        source_width -= (int)((double)-target_left_added / scale);
                                                        if (source_width <= 0)
                                                            continue;
                                                        target_left_added = 0;
                                                    }
                                                    if (target_top_added < 0)
                                                    {
                                                        within_tile_source_y += (int)((double)-target_top_added / scale);
                                                        source_height -= (int)((double)-target_top_added / scale);
                                                        if (source_height <= 0)
                                                            continue;
                                                        target_top_added = 0;
                                                    }
                                                    int target_x = rect->left + target_left_added;
                                                    int target_y = rect->top + target_top_added;
                                                    int target_width = (int)((double)source_width * scale);
                                                    int target_height = (int)((double)source_height * scale);
                                                    if (target_x + target_width > rect->right)
                                                    {
                                                        /* Cut off from right ==>Just reduce width to fix, flipped: Move source x right and reduce width to fix */
                                                        int diff = (int)((double)(target_x + target_width - rect->right) / scale);

                                                        if (used_hflip_link)
                                                            within_tile_source_x += diff;

                                                        source_width -= diff;
                                                        if (source_width <= 0)
                                                            continue;
                                                        target_width -= (target_x + target_width - rect->right);
                                                    }
                                                    if (target_y + target_height > rect->bottom)
                                                    {
                                                        int diff = (int)((double)(target_y + target_height - rect->bottom) / scale);
                                                        
                                                        source_height -= diff;
                                                        if (source_height <= 0)
                                                            continue;
                                                        target_height -= (target_y + target_height - rect->bottom);
                                                    }

                                                    int source_x = at_x + within_tile_source_x;
                                                    int source_y = at_y + within_tile_source_y;

                                                    (*GetNHApp()->lpfnTransparentBlt)(
                                                        data->backBufferDC, target_x, target_y,
                                                        target_width, target_height, data->tileDC[a_sheet_idx], source_x + (used_hflip_link ? source_width - 1 : 0),
                                                        source_y + (vflip_link ? source_height - 1 : 0), (used_hflip_link ? -1 : 1) * source_width,
                                                        (vflip_link ? -1 : 1) * source_height, TILE_BK_COLOR);
                                                }


                                            }
                                        }
                                        else if (dir_idx > 0)
                                        {
                                            if (relevant_dx && relevant_dy)
                                            {
                                                int added_source_x = 0, added_source_y = 0;
                                                int added_target_x = 0, added_target_y = 0;
                                                boolean draw_link = FALSE;

                                                if (relevant_dx < 0 && relevant_dy < 0)
                                                {
                                                    if (dir_idx == 2)
                                                    {
                                                        added_source_x = 8;
                                                        added_source_y = 8;
                                                        added_target_x = tileWidth - 8;
                                                        added_target_y = 0;
                                                        draw_link = TRUE;
                                                    }
                                                    else if (dir_idx == 3)
                                                    {
                                                        added_source_x = 0;
                                                        added_source_y = 0;
                                                        added_target_x = 0;
                                                        added_target_y = tileHeight - 8;
                                                        draw_link = TRUE;
                                                    }
                                                }
                                                else if (relevant_dx > 0 && relevant_dy < 0)
                                                {
                                                    if (dir_idx == 4)
                                                    {
                                                        added_source_x = 8;
                                                        added_source_y = 8;
                                                        added_target_x = 0;
                                                        added_target_y = 0;
                                                        draw_link = TRUE;
                                                    }
                                                    else if (dir_idx == 3)
                                                    {
                                                        added_source_x = 0;
                                                        added_source_y = 0;
                                                        added_target_x = tileWidth - 8;
                                                        added_target_y = tileHeight - 8;
                                                        draw_link = TRUE;
                                                    }
                                                }
                                                else if (relevant_dx < 0 && relevant_dy > 0)
                                                {
                                                    if (dir_idx == 2)
                                                    {
                                                        added_source_x = 0;
                                                        added_source_y = 0;
                                                        added_target_x = tileWidth - 8;
                                                        added_target_y = tileHeight - 8;
                                                        draw_link = TRUE;
                                                    }
                                                    else if (dir_idx == 1)
                                                    {
                                                        added_source_x = 8;
                                                        added_source_y = 8;
                                                        added_target_x = 0;
                                                        added_target_y = 0;
                                                        draw_link = TRUE;
                                                    }
                                                }
                                                else if (relevant_dx > 0 && relevant_dy > 0)
                                                {
                                                    if (dir_idx == 4)
                                                    {
                                                        added_source_x = 0;
                                                        added_source_y = 0;
                                                        added_target_x = 0;
                                                        added_target_y = tileHeight - 8;
                                                        draw_link = TRUE;
                                                    }
                                                    else if (dir_idx == 1)
                                                    {
                                                        added_source_x = 8;
                                                        added_source_y = 8;
                                                        added_target_x = tileWidth - 8;
                                                        added_target_y = 0;
                                                        draw_link = TRUE;
                                                    }
                                                }
                                                if (draw_link)
                                                {
                                                    int source_width = 8;
                                                    int source_height = 8;
                                                    int within_tile_source_x = 32 + added_source_x;
                                                    int within_tile_source_y = 23 + ((links + 1 % 2) == 1 ? link_source_height : 0) + added_source_y;
                                                    int target_x = rect->left + (int)((double)(added_target_x) * scale);
                                                    int target_y = rect->top + (int)((double)(added_target_y) * scale);
                                                    int target_width = (int)((double)source_width * scale);
                                                    int target_height = (int)((double)source_height * scale);
                                                    int source_x = at_x + within_tile_source_x;
                                                    int source_y = at_y + within_tile_source_y;

                                                    (*GetNHApp()->lpfnTransparentBlt)(
                                                        data->backBufferDC, target_x, target_y,
                                                        target_width, target_height, data->tileDC[a_sheet_idx], source_x + (hflip_link ? source_width - 1 : 0),
                                                        source_y + (vflip_link ? source_height - 1 : 0), (hflip_link ? -1 : 1)* source_width,
                                                        (vflip_link ? -1 : 1)* source_height, TILE_BK_COLOR);

                                                }

                                            }
                                        }
                                    }
                                }
                            }
                            else if (autodraws[autodraw].draw_type == AUTODRAW_DRAW_LONG_WORM && mtmp)
                            {  
                                /* Long worm here */
                                
                                int source_glyph_seg_end = autodraws[autodraw].source_glyph;
                                int source_glyph_seg_dir_out = autodraws[autodraw].source_glyph2;
                                int source_glyph_seg_dir_in = autodraws[autodraw].source_glyph2 + 4;
                                int source_glyph_seg_layer = autodraws[autodraw].source_glyph3;
                                int drawing_tail = autodraws[autodraw].flags;
                                double scale = (double)(rect->bottom - rect->top) / (double)tileHeight;
                                int wdir_out = get_wseg_dir_at(mtmp, enl_i, enl_j);
                                int wdir_in = get_reverse_prev_wseg_dir_at(mtmp, enl_i, enl_j);
                                boolean is_head = is_wseg_head(mtmp, enl_i, enl_j);
                                boolean is_tailend = is_wseg_tailend(mtmp, enl_i, enl_j);
                                for (int wlayer = 0; wlayer < 5; wlayer++)
                                {
                                    int source_glyph = NO_GLYPH;
                                    boolean hflip_seg = FALSE;
                                    boolean vflip_seg = FALSE;
                                    switch (wlayer)
                                    {
                                    case 0:
                                    case 2:
                                    case 4:
                                        if (is_head || is_tailend)
                                            continue;
                                        source_glyph = source_glyph_seg_layer + wlayer / 2;
                                        break;
                                    case 1:
                                        source_glyph = is_tailend ? NO_GLYPH : is_head ? source_glyph_seg_end : source_glyph_seg_dir_in;
                                        break;
                                    case 3:
                                        source_glyph = is_tailend ? source_glyph_seg_end : is_head ? NO_GLYPH : source_glyph_seg_dir_out;
                                        break;
                                    default:
                                        break;
                                    }

                                    if (source_glyph != NO_GLYPH)
                                    {
                                        int wdir = (wlayer == 1 ? wdir_in : wlayer == 3 ? wdir_out : 0);
                                        switch (wdir)
                                        {
                                        case 1:
                                            source_glyph += 2;
                                            hflip_seg = FALSE;
                                            vflip_seg = FALSE;
                                            break;
                                        case 2:
                                            source_glyph += 0;
                                            hflip_seg = FALSE;
                                            vflip_seg = FALSE;
                                            break;
                                        case 3:
                                            source_glyph += 3;
                                            hflip_seg = FALSE;
                                            vflip_seg = TRUE;
                                            break;
                                        case 4:
                                            source_glyph += 1;
                                            hflip_seg = TRUE;
                                            vflip_seg = FALSE;
                                            break;
                                        case 5:
                                            source_glyph += 3;
                                            hflip_seg = FALSE;
                                            vflip_seg = FALSE;
                                            break;
                                        case 6:
                                            source_glyph += 0;
                                            hflip_seg = FALSE;
                                            vflip_seg = TRUE;
                                            break;
                                        case 7:
                                            source_glyph += 2;
                                            hflip_seg = FALSE;
                                            vflip_seg = TRUE;
                                            break;
                                        case 8:
                                            source_glyph += 1;
                                            hflip_seg = FALSE;
                                            vflip_seg = FALSE;
                                            break;
                                        default:
                                            break;
                                        }

                                        int atile = glyph2tile[source_glyph];
                                        int a_sheet_idx = TILE_SHEET_IDX(atile);
                                        int at_x = TILEBMP_X(atile);
                                        int at_y = TILEBMP_Y(atile);

                                        int source_width = tileWidth;
                                        int source_height = tileHeight;
                                        int target_x = rect->left;
                                        int target_y = rect->top;
                                        int target_width = (int)((double)source_width * scale);
                                        int target_height = (int)((double)source_height * scale);
                                        int source_x = at_x;
                                        int source_y = at_y;

                                        (*GetNHApp()->lpfnTransparentBlt)(
                                            data->backBufferDC, target_x, target_y,
                                            target_width, target_height, data->tileDC[a_sheet_idx], source_x + (hflip_seg ? source_width - 1 : 0),
                                            source_y + (vflip_seg ? source_height - 1 : 0), (hflip_seg ? -1 : 1) * source_width,
                                            (vflip_seg ? -1 : 1) * source_height, TILE_BK_COLOR);
                                    }
                                }
                            }
                        }
                        /*
                         * AUTODRAW END
                         */

                        /* Item property marks */
                        if (((base_layer == LAYER_OBJECT || base_layer == LAYER_COVER_OBJECT) && otmp_round &&
                            (otmp_round->opoisoned || otmp_round->elemental_enchantment > 0 || otmp_round->mythic_prefix > 0 || otmp_round->mythic_suffix > 0 || otmp_round->oeroded || otmp_round->oeroded2 || otmp_round->exceptionality > 0))
                            ||
                            ((base_layer == LAYER_MISSILE) &&
                                (data->map[enl_i][enl_j].missile_poisoned || data->map[enl_i][enl_j].missile_elemental_enchantment > 0 
                                    || data->map[enl_i][enl_j].missile_eroded || data->map[enl_i][enl_j].missile_eroded2 || 
                                    data->map[enl_i][enl_j].missile_exceptionality > 0 || data->map[enl_i][enl_j].missile_mythic_prefix > 0 || data->map[enl_i][enl_j].missile_mythic_suffix > 0))
                            )
                        {
                            int y_start = (base_layer == LAYER_MISSILE && !move_obj_to_middle ? tileHeight / 4 : dest_top_added - (int)(applicable_scaling_factor_y * base_source_top_added));
                            int x_start = dest_left_added;
                            int mark_width = 8;
                            int marks_per_row = TILE_X / mark_width;
                            int mark_height = 24;
                            int src_x = 0;
                            int src_y = 0;
                            int cnt = 0;
                            int poisoned = (base_layer == LAYER_MISSILE ? data->map[enl_i][enl_j].missile_poisoned : otmp_round->opoisoned);
                            uchar elemental_enchantment = (base_layer == LAYER_MISSILE ? data->map[enl_i][enl_j].missile_elemental_enchantment : otmp_round->elemental_enchantment);
                            uchar exceptionality = (base_layer == LAYER_MISSILE ? data->map[enl_i][enl_j].missile_exceptionality : otmp_round->exceptionality);
                            uchar mythic_prefix = (base_layer == LAYER_MISSILE ? data->map[enl_i][enl_j].missile_mythic_prefix : otmp_round->mythic_prefix);
                            uchar mythic_suffix = (base_layer == LAYER_MISSILE ? data->map[enl_i][enl_j].missile_mythic_suffix : otmp_round->mythic_suffix);
                            uchar eroded = (base_layer == LAYER_MISSILE ? data->map[enl_i][enl_j].missile_eroded : (uchar)otmp_round->oeroded);
                            uchar eroded2 = (base_layer == LAYER_MISSILE ? data->map[enl_i][enl_j].missile_eroded2 : (uchar)otmp_round->oeroded2);
                            boolean corrodeable = (base_layer == LAYER_MISSILE ? !!(data->map[enl_i][enl_j].missile_flags & MISSILE_FLAGS_CORRODEABLE) : is_corrodeable(otmp_round));
                            boolean rottable = (base_layer == LAYER_MISSILE ? !!(data->map[enl_i][enl_j].missile_flags & MISSILE_FLAGS_ROTTABLE) : is_rottable(otmp_round));
                            boolean flammable = (base_layer == LAYER_MISSILE ? !!(data->map[enl_i][enl_j].missile_flags & MISSILE_FLAGS_FLAMMABLE) : is_flammable(otmp_round));
                            boolean rustprone = (base_layer == LAYER_MISSILE ? !!(data->map[enl_i][enl_j].missile_flags & MISSILE_FLAGS_RUSTPRONE) : is_rustprone(otmp_round));
                            boolean poisonable = (base_layer == LAYER_MISSILE ? !!(data->map[enl_i][enl_j].missile_flags & MISSILE_FLAGS_POISONABLE) : is_poisonable(otmp_round));

                            for (enum item_property_mark_types ipm_idx = 0; ipm_idx < MAX_ITEM_PROPERTY_MARKS; ipm_idx++)
                            {
                                if (cnt >= 8)
                                    break;

                                int src_x = (ipm_idx % marks_per_row) * mark_width, src_y = (ipm_idx / marks_per_row) * mark_height;
                                int dest_x = 0, dest_y = 0;

                                switch (ipm_idx)
                                {
                                case ITEM_PROPERTY_MARK_POISONED:
                                    if (!(poisoned && poisonable))
                                        continue;
                                    break;
                                case ITEM_PROPERTY_MARK_DEATH_MAGICAL:
                                    if (elemental_enchantment != DEATH_ENCHANTMENT)
                                        continue;
                                    break;
                                case ITEM_PROPERTY_MARK_FLAMING:
                                    if (elemental_enchantment != FIRE_ENCHANTMENT)
                                        continue;
                                    break;
                                case ITEM_PROPERTY_MARK_FREEZING:
                                    if (elemental_enchantment != COLD_ENCHANTMENT)
                                        continue;
                                    break;
                                case ITEM_PROPERTY_MARK_ELECTRIFIED:
                                    if (elemental_enchantment != LIGHTNING_ENCHANTMENT)
                                        continue;
                                    break;
                                case ITEM_PROPERTY_MARK_EXCEPTIONAL:
                                    if (exceptionality != EXCEPTIONALITY_EXCEPTIONAL)
                                        continue;
                                    break;
                                case ITEM_PROPERTY_MARK_ELITE:
                                    if (exceptionality != EXCEPTIONALITY_ELITE)
                                        continue;
                                    break;
                                case ITEM_PROPERTY_MARK_CELESTIAL:
                                    if (exceptionality != EXCEPTIONALITY_CELESTIAL)
                                        continue;
                                    break;
                                case ITEM_PROPERTY_MARK_PRIMORDIAL:
                                    if (exceptionality != EXCEPTIONALITY_PRIMORDIAL)
                                        continue;
                                    break;
                                case ITEM_PROPERTY_MARK_INFERNAL:
                                    if (exceptionality != EXCEPTIONALITY_INFERNAL)
                                        continue;
                                    break;
                                case ITEM_PROPERTY_MARK_MYTHIC:
                                    if ((mythic_prefix == 0 && mythic_suffix == 0) || (mythic_prefix > 0 && mythic_suffix > 0))
                                        continue;
                                    break;
                                case ITEM_PROPERTY_MARK_LEGENDARY:
                                    if (mythic_prefix == 0 || mythic_suffix == 0)
                                        continue;
                                    break;
                                case ITEM_PROPERTY_MARK_CORRODED:
                                    if (!(eroded2 == 1 && corrodeable))
                                        continue;
                                    break;
                                case ITEM_PROPERTY_MARK_ROTTED:
                                    if (!(eroded2 == 1 && rottable))
                                        continue;
                                    break;
                                case ITEM_PROPERTY_MARK_BURNT:
                                    if (!(eroded == 1  && flammable))
                                        continue;
                                    break;
                                case ITEM_PROPERTY_MARK_RUSTY:
                                    if (!(eroded == 1 && rustprone))
                                        continue;
                                    break;
                                case ITEM_PROPERTY_MARK_VERY_CORRODED:
                                    if (!(eroded2 == 2 && corrodeable))
                                        continue;
                                    break;
                                case ITEM_PROPERTY_MARK_VERY_ROTTED:
                                    if (!(eroded2 == 2 && rottable))
                                        continue;
                                    break;
                                case ITEM_PROPERTY_MARK_VERY_BURNT:
                                    if (!(eroded == 2 && flammable))
                                        continue;
                                    break;
                                case ITEM_PROPERTY_MARK_VERY_RUSTY:
                                    if (!(eroded == 2 && rustprone))
                                        continue;
                                    break;
                                case ITEM_PROPERTY_MARK_THOROUGHLY_CORRODED:
                                    if (!(eroded2 == 3 && corrodeable))
                                        continue;
                                    break;
                                case ITEM_PROPERTY_MARK_THOROUGHLY_ROTTED:
                                    if (!(eroded2 == 3 && rottable))
                                        continue;
                                    break;
                                case ITEM_PROPERTY_MARK_THOROUGHLY_BURNT:
                                    if (!(eroded == 3 && flammable))
                                        continue;
                                    break;
                                case ITEM_PROPERTY_MARK_THOROUGHLY_RUSTY:
                                    if (!(eroded == 3 && rustprone))
                                        continue;
                                    break;
                                case MAX_ITEM_PROPERTY_MARKS:
                                default:
                                    continue;
                                    break;
                                }

                                int item_xpos = ((int)tileWidth) / 2 - mark_width + (cnt % 2 ? 1 : -1) * ((cnt + 1) / 2) * mark_width;

                                dest_y = y_start + (int)(obj_scaling_factor * (double)(tileHeight / 4 + mark_height / 2 - mark_height));
                                dest_x = x_start + (int)(obj_scaling_factor * (double)item_xpos);

                                int source_glyph = ITEM_PROPERTY_MARKS + GLYPH_UI_TILE_OFF;
                                int atile = glyph2tile[source_glyph];
                                int a_sheet_idx = TILE_SHEET_IDX(atile);
                                int at_x = TILEBMP_X(atile);
                                int at_y = TILEBMP_Y(atile);

                                RECT source_rt = { 0 };
                                source_rt.left = at_x + src_x;
                                source_rt.right = source_rt.left + mark_width;
                                source_rt.top = at_y + src_y;
                                source_rt.bottom = source_rt.top + mark_height;

                                RECT target_rt = { 0 };

                                if (print_first_directly_to_map)
                                {
                                    target_rt.left = rect->left + dest_x;
                                    target_rt.right = rect->left + dest_x + (int)(((double)data->xBackTile / (double)tileWidth) * obj_scaling_factor * (double)(source_rt.right - source_rt.left));
                                    target_rt.top = rect->top + dest_y;
                                    target_rt.bottom = rect->top + dest_y + (int)(((double)data->yBackTile / (double)tileHeight) * obj_scaling_factor * (double)(source_rt.bottom - source_rt.top));

                                    (*GetNHApp()->lpfnTransparentBlt)(
                                        data->backBufferDC, target_rt.left, target_rt.top,
                                        target_rt.right - target_rt.left, target_rt.bottom - target_rt.top, data->tileDC[a_sheet_idx], source_rt.left,
                                        source_rt.top, source_rt.right - source_rt.left,
                                        source_rt.bottom - source_rt.top, TILE_BK_COLOR);
                                }
                                else
                                {
                                    target_rt.left = dest_x;
                                    target_rt.right = dest_x + source_rt.right - source_rt.left;
                                    target_rt.top = dest_y;
                                    target_rt.bottom = dest_y + source_rt.bottom - source_rt.top;

                                    (*GetNHApp()->lpfnTransparentBlt)(
                                        hDCcopy, target_rt.left, target_rt.top,
                                        target_rt.right - target_rt.left, target_rt.bottom - target_rt.top, data->tileDC[a_sheet_idx], source_rt.left,
                                        source_rt.top, source_rt.right - source_rt.left,
                                        source_rt.bottom - source_rt.top, TILE_BK_COLOR);
                                }
                                cnt++;
                            }                        
                        }
                    }
                }


                /* Darken and draw to buffer, opaque indicates something has been drawn to hDCcopy */
                if (((draw_order[draw_index].draw_to_buffer == 1)
                    || (draw_order[draw_index].draw_to_buffer == 2 && !print_first_directly_to_map))
                    && opaque_background_drawn)
                {
                    /* First, darkening of dark areas and areas drawn from memory */
                    if (!skip_darkening)
                    {
                        boolean ascension_radiance = ((data->map[darkening_i][darkening_j].layer_flags & LFLAGS_ASCENSION_RADIANCE) != 0);
                        if (!cansee(darkening_i, darkening_j)  || ascension_radiance || (data->map[darkening_i][darkening_j].layer_flags & LFLAGS_SHOWING_MEMORY))
                        {
                            //if (1) // Always true
                            {
                                double multiplier = 1.0;
                                if (isyou)
                                {
                                    if(default_tileset_definition.you_darkening[relevant_darkening_cmap] > 0.0)
                                        multiplier *= default_tileset_definition.you_darkening[relevant_darkening_cmap];
                                    else
                                        multiplier *= 0.85;
                                }
                                else
                                {
                                    boolean is_lit_unknown_wall = levl[darkening_i][darkening_j].waslit && IS_NON_STONE_WALL(levl[darkening_i][darkening_j].typ) && wall_angle(&levl[darkening_i][darkening_j]) == S_stone;
                                    if (!levl[darkening_i][darkening_j].waslit || is_lit_unknown_wall)
                                    {
                                        if (default_tileset_definition.nonlit_darkening[relevant_darkening_cmap] > 0.0)
                                            multiplier *= default_tileset_definition.nonlit_darkening[relevant_darkening_cmap];
                                        else
                                            multiplier *= 0.35;
                                    }
                                    else
                                    {
                                        if (default_tileset_definition.lit_darkening[relevant_darkening_cmap] > 0.0)
                                            multiplier *= default_tileset_definition.lit_darkening[relevant_darkening_cmap];
                                        else
                                            multiplier *= 0.65;
                                    }
                                }


                                if (print_first_directly_to_map)
                                {
                                    HDC hDCMem = CreateCompatibleDC(data->backBufferDC);

                                    unsigned char* lpBitmapBits;
                                    LONG width = rect->right - rect->left;
                                    LONG height = rect->bottom - rect->top;

                                    BITMAPINFO bi;
                                    ZeroMemory(&bi, sizeof(BITMAPINFO));
                                    bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
                                    bi.bmiHeader.biWidth = width;
                                    bi.bmiHeader.biHeight = height;
                                    bi.bmiHeader.biPlanes = 1;
                                    bi.bmiHeader.biBitCount = 32;

                                    HBITMAP bitmap = CreateDIBSection(hDCMem, &bi, DIB_RGB_COLORS, (VOID**)&lpBitmapBits, NULL, 0);
                                    HGDIOBJ oldbmp = SelectObject(hDCMem, bitmap);
                                    StretchBlt(hDCMem, 0, 0, width, height,
                                        data->backBufferDC, rect->left, rect->top, width, height, SRCCOPY);

                                    int pitch = 4 * width; // 4 bytes per pixel but if not 32 bit, round pitch up to multiple of 4
                                    int idx, x, y;
                                    if (ascension_radiance)
                                    {
                                        multiplier = min(1.0, 0.4 + sqrt((double)dist2(darkening_i, darkening_j, u.ux, u.uy)) / 12.0);
                                        for (x = 0; x < width; x++)
                                        {
                                            for (y = 0; y < height; y++)
                                            {
                                                idx = y * pitch;
                                                idx += x * 4;

                                                lpBitmapBits[idx + 0] = (unsigned char)(((double)lpBitmapBits[idx + 0]) * multiplier + 255 * (1.0 - multiplier));  // blue
                                                lpBitmapBits[idx + 1] = (unsigned char)(((double)lpBitmapBits[idx + 1]) * multiplier + 255 * (1.0 - multiplier)); // green
                                                lpBitmapBits[idx + 2] = (unsigned char)(((double)lpBitmapBits[idx + 2]) * multiplier + 255 * (1.0 - multiplier));  // red 
                                            }
                                        }
                                    }
                                    else
                                    {
                                        for (x = 0; x < width; x++)
                                        {
                                            for (y = 0; y < height; y++)
                                            {
                                                idx = y * pitch;
                                                idx += x * 4;

                                                lpBitmapBits[idx + 0] = (unsigned char)(((double)lpBitmapBits[idx + 0]) * multiplier);  // blue
                                                lpBitmapBits[idx + 1] = (unsigned char)(((double)lpBitmapBits[idx + 1]) * multiplier); // green
                                                lpBitmapBits[idx + 2] = (unsigned char)(((double)lpBitmapBits[idx + 2]) * multiplier);  // red 
                                            }
                                        }
                                    }
                                    StretchBlt(data->backBufferDC, rect->left, rect->top, width, height, hDCMem, 0, 0, width, height, SRCCOPY);
                                    SelectObject(hDCMem, oldbmp);
                                    DeleteDC(hDCMem);
                                    DeleteObject(bitmap);
                                }
                                else
                                {

                                    LONG width = GetNHApp()->mapTile_X;
                                    LONG height = GetNHApp()->mapTile_Y;

                                    int pitch = 4 * width; // 4 bytes per pixel but if not 32 bit, round pitch up to multiple of 4
                                    int idx, x, y;
                                    if (ascension_radiance)
                                    {
                                        multiplier = min(1.0, 0.4 + sqrt((double)dist2(darkening_i, darkening_j, u.ux, u.uy)) / 12.0);
                                        for (x = 0; x < width; x++)
                                        {
                                            for (y = 0; y < height; y++)
                                            {
                                                idx = y * pitch;
                                                idx += x * 4;

                                                if (lpBitmapBitsCopy[idx + 0] == TILE_BK_COLOR_BLUE && lpBitmapBitsCopy[idx + 1] == TILE_BK_COLOR_GREEN && lpBitmapBitsCopy[idx + 2] == TILE_BK_COLOR_RED)
                                                    continue;

                                                lpBitmapBitsCopy[idx + 0] = (unsigned char)(((double)lpBitmapBitsCopy[idx + 0]) * multiplier + 255 * (1.0 - multiplier));  // blue
                                                lpBitmapBitsCopy[idx + 1] = (unsigned char)(((double)lpBitmapBitsCopy[idx + 1]) * multiplier + 255 * (1.0 - multiplier)); // green
                                                lpBitmapBitsCopy[idx + 2] = (unsigned char)(((double)lpBitmapBitsCopy[idx + 2]) * multiplier + 255 * (1.0 - multiplier));  // red 
                                            }
                                        }
                                    }
                                    else
                                    {
                                        for (x = 0; x < width; x++)
                                        {
                                            for (y = 0; y < height; y++)
                                            {
                                                idx = y * pitch;
                                                idx += x * 4;

                                                if (lpBitmapBitsCopy[idx + 0] == TILE_BK_COLOR_BLUE && lpBitmapBitsCopy[idx + 1] == TILE_BK_COLOR_GREEN && lpBitmapBitsCopy[idx + 2] == TILE_BK_COLOR_RED)
                                                    continue;

                                                lpBitmapBitsCopy[idx + 0] = (unsigned char)(((double)lpBitmapBitsCopy[idx + 0]) * multiplier);  // blue
                                                lpBitmapBitsCopy[idx + 1] = (unsigned char)(((double)lpBitmapBitsCopy[idx + 1]) * multiplier); // green
                                                lpBitmapBitsCopy[idx + 2] = (unsigned char)(((double)lpBitmapBitsCopy[idx + 2]) * multiplier);  // red 
                                            }
                                        }
                                    }
                                }

                                /* Darken also copy of the monster */
                                if (monster_copied && !monster_darkened)
                                {
                                    LONG width = GetNHApp()->mapTile_X;
                                    LONG height = GetNHApp()->mapTile_Y;

                                    int pitch = 4 * width; // 4 bytes per pixel but if not 32 bit, round pitch up to multiple of 4
                                    int idx, x, y;
                                    for (x = 0; x < width; x++)
                                    {
                                        for (y = 0; y < height; y++)
                                        {
                                            idx = y * pitch;
                                            idx += x * 4;

                                            if (lpBitmapBitsMonster[idx + 0] == TILE_BK_COLOR_BLUE && lpBitmapBitsMonster[idx + 1] == TILE_BK_COLOR_GREEN && lpBitmapBitsMonster[idx + 2] == TILE_BK_COLOR_RED)
                                                continue;

                                            lpBitmapBitsMonster[idx + 0] = (unsigned char)(((double)lpBitmapBitsMonster[idx + 0]) * multiplier);  // blue
                                            lpBitmapBitsMonster[idx + 1] = (unsigned char)(((double)lpBitmapBitsMonster[idx + 1]) * multiplier); // green
                                            lpBitmapBitsMonster[idx + 2] = (unsigned char)(((double)lpBitmapBitsMonster[idx + 2]) * multiplier);  // red 
                                        }
                                    }
                                    monster_darkened = TRUE;
                                }
                            }
                        }
                    }

                    if (print_first_directly_to_map)
                    {
                        /* All drawn to the back buffer already */
                        print_first_directly_to_map = FALSE; /* Now draw to the drawing bitmap going forward */
                        opaque_background_drawn = FALSE; /* Indicates the drawing bitmap hasn't been drawn yet */
                    }
                    else
                    {
                        /* Second, draw the current contents of the drawing bitmap to the back buffer */
                        (*GetNHApp()->lpfnTransparentBlt)(
                            data->backBufferDC, rect->left, rect->top,
                            data->xBackTile, data->yBackTile, hDCcopy, 0,
                            0, GetNHApp()->mapTile_X,
                            GetNHApp()->mapTile_Y, TILE_BK_COLOR);

                        /* Set the drawing bitmap to be cleared next */
                        opaque_background_drawn = FALSE;
                    }
                }

                int monster_glyph = data->map[enl_i][enl_j].layer_glyphs[LAYER_MONSTER];

                if (draw_order[draw_index].draw_monster_shadow && monster_copied && draw_monster_shadow && !monster_shadow_drawn) /* Everything about monsters and objects should already be printed now to screen */
                {
                    /* Monster as transparent shadow */
                    monster_shadow_drawn = TRUE;

                    /* Create copy of background */
                    HDC hDCMem = CreateCompatibleDC(data->backBufferDC);

                    unsigned char* lpBitmapBits;
                    LONG width = rect->right - rect->left;
                    LONG height = rect->bottom - rect->top;

                    BITMAPINFO bi;
                    ZeroMemory(&bi, sizeof(BITMAPINFO));
                    bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
                    bi.bmiHeader.biWidth = width;
                    bi.bmiHeader.biHeight = height;
                    bi.bmiHeader.biPlanes = 1;
                    bi.bmiHeader.biBitCount = 32;

                    HBITMAP bitmap = CreateDIBSection(hDCMem, &bi, DIB_RGB_COLORS, (VOID**)&lpBitmapBits, NULL, 0);
                    HGDIOBJ oldbmp = SelectObject(hDCMem, bitmap);
                    StretchBlt(hDCMem, 0, 0, width, height,
                        data->backBufferDC, rect->left,
                        rect->top, width, height, SRCCOPY);

                    /* Draw monster shadow */
                    int pitch = 4 * width; // 4 bytes per pixel but if not 32 bit, round pitch up to multiple of 4
                    int idx, x, y;
                    double semi_transparency = 0.5;

                    for (x = 0; x < width; x++)
                    {
                        for (y = 0; y < height; y++)
                        {
                            idx = y * pitch;
                            idx += x * 4;

                            if (lpBitmapBitsMonster[idx + 0] == TILE_BK_COLOR_BLUE && lpBitmapBitsMonster[idx + 1] == TILE_BK_COLOR_GREEN && lpBitmapBitsMonster[idx + 2] == TILE_BK_COLOR_RED)
                                continue;

                            lpBitmapBitsMonster[idx + 0] = (unsigned char)(((double)lpBitmapBitsMonster[idx + 0]) * (1.0 - semi_transparency) + ((double)lpBitmapBits[idx + 0]) * (semi_transparency));  // blue
                            lpBitmapBitsMonster[idx + 1] = (unsigned char)(((double)lpBitmapBitsMonster[idx + 1]) * (1.0 - semi_transparency) + ((double)lpBitmapBits[idx + 1]) * (semi_transparency));  // green
                            lpBitmapBitsMonster[idx + 2] = (unsigned char)(((double)lpBitmapBitsMonster[idx + 2]) * (1.0 - semi_transparency) + ((double)lpBitmapBits[idx + 2]) * (semi_transparency));  // red 
                        }
                    }

                    (*GetNHApp()->lpfnTransparentBlt)(
                        data->backBufferDC, rect->left, rect->top,
                        data->xBackTile, data->yBackTile, hDCmonster, 0,
                        0, GetNHApp()->mapTile_X,
                        GetNHApp()->mapTile_Y, TILE_BK_COLOR);

                    SelectObject(hDCMem, oldbmp);
                    DeleteDC(hDCMem);
                    DeleteObject(bitmap);
                }
                else if (base_layer == LAYER_GENERAL_UI && enlarg_idx == -1 && tile_move_idx == 0)
                {
                    /* All UI related symbols and cursors */

                    boolean loc_is_you = (i == u.ux && j == u.uy);

                    /* Grid */
                    if (flags.show_grid)
                    {
                        HPEN curpen = CreatePen(PS_DOT, 1, RGB(0, 0, 0));
                        HGDIOBJ saveobj = SelectObject(data->backBufferDC, curpen);
                        MoveToEx(data->backBufferDC, rect->right - 1, rect->top, NULL);
                        LineTo(data->backBufferDC, rect->right - 1, rect->bottom - 1);
                        LineTo(data->backBufferDC, max(0, rect->left - 1), rect->bottom - 1);
                        SelectObject(data->backBufferDC, saveobj);
                        DeleteObject(curpen);
                    }

                    /* Chain lock mark */
                    if (loc_is_you && uball && uchain)
                    {
                        double scale = (double)(rect->bottom - rect->top) / (double)tileHeight;
                        int mglyph = ITEM_AUTODRAW_GRAPHICS + GLYPH_UI_TILE_OFF;
                        int mtile = glyph2tile[mglyph];
                        int m_sheet_idx = TILE_SHEET_IDX(mtile);
                        int source_x = TILEBMP_X(mtile) + 0;
                        int source_y = TILEBMP_Y(mtile) + 64;
                        int source_width = 32;
                        int source_height = 32;
                        int target_x = rect->left + (int)(2.0 * scale); // (int)((double)(tileWidth - source_width) * scale / 2.0);
                        int target_y = rect->bottom - (int)((double)(source_height + 2) * scale);
                        int target_width = (int)((double)source_width * scale);
                        int target_height = (int)((double)source_height * scale);

                        (*GetNHApp()->lpfnTransparentBlt)(
                            data->backBufferDC, target_x, target_y,
                            target_width, target_height, data->tileDC[m_sheet_idx], source_x,
                            source_y, source_width,
                            source_height, TILE_BK_COLOR);
                    }


                    boolean draw_character = FALSE;

                    /* Cursor */
                    if (i == data->xCur && j == data->yCur)
                    {
                        boolean cannotseeself = (loc_is_you) && !canspotself();
                        if (!cannotseeself &&
                            ((!data->cursorOn && flags.blinking_cursor_on_tiles)
                                || (/*i == u.ux && j == u.uy &&*/ !flags.show_cursor_on_u)
                                ))
                        {
                            // Nothing, cursor is invisible
                        }
                        else
                        {
                            int anim_frame_idx = -1, main_tile_idx = -1;
                            int cglyph = (cannotseeself && flags.active_cursor_style == CURSOR_STYLE_GENERIC_CURSOR ? CURSOR_STYLE_INVISIBLE : flags.active_cursor_style) + GLYPH_CURSOR_OFF;
                            int ctile = glyph2tile[cglyph];
                            int tile_animation_idx = get_tile_animation_index_from_glyph(cglyph);
                            ctile = maybe_get_replaced_tile(ctile, i, j, zeroreplacementinfo, (enum autodraw_types*)0);
                            ctile = maybe_get_animated_tile(ctile, tile_animation_idx, ANIMATION_PLAY_TYPE_ALWAYS, animation_timers.general_animation_counter, &anim_frame_idx, &main_tile_idx, &data->mapAnimated[i][j], (enum autodraw_types*)0);
                            int c_sheet_idx = TILE_SHEET_IDX(ctile);
                            t_x = TILEBMP_X(ctile);
                            t_y = TILEBMP_Y(ctile);

                            (*GetNHApp()->lpfnTransparentBlt)(
                                data->backBufferDC, rect->left, rect->top,
                                data->xBackTile, data->yBackTile, data->tileDC[c_sheet_idx], t_x,
                                t_y, GetNHApp()->mapTile_X,
                                GetNHApp()->mapTile_Y, TILE_BK_COLOR);

                        }
                    }

                    /* Monster target marker */
                    if (glyph_is_monster(monster_glyph) && mtmp && !is_worm_tail)
                    {
                        if (flags.show_tile_monster_target && !loc_is_you)
                        {
                            draw_character = TRUE;

                            int mglyph = MAIN_TILE_MARK + GLYPH_UI_TILE_OFF;
                            int mtile = glyph2tile[mglyph];
                            int m_sheet_idx = TILE_SHEET_IDX(mtile);
                            t_x = TILEBMP_X(mtile);
                            t_y = TILEBMP_Y(mtile);

                            (*GetNHApp()->lpfnTransparentBlt)(
                                data->backBufferDC, rect->left, rect->top,
                                data->xBackTile, data->yBackTile, data->tileDC[m_sheet_idx], t_x,
                                t_y, GetNHApp()->mapTile_X,
                                GetNHApp()->mapTile_Y, TILE_BK_COLOR);


                        }
                    }

                    /* Player marker */
                    if (flags.show_tile_u_mark && loc_is_you)
                    {
                        if(loc_is_you)
                            draw_character = TRUE;

                        int mglyph = U_TILE_MARK + GLYPH_UI_TILE_OFF;
                        int mtile = glyph2tile[mglyph];
                        int m_sheet_idx = TILE_SHEET_IDX(mtile);
                        t_x = TILEBMP_X(mtile);
                        t_y = TILEBMP_Y(mtile);

                        (*GetNHApp()->lpfnTransparentBlt)(
                            data->backBufferDC, rect->left, rect->top,
                            data->xBackTile, data->yBackTile, data->tileDC[m_sheet_idx], t_x,
                            t_y, GetNHApp()->mapTile_X,
                            GetNHApp()->mapTile_Y, TILE_BK_COLOR);
                    }

                    if (draw_character)
                    {

                        char ch;
                        WCHAR wch;
                        int color;
                        unsigned long special;
                        int mgch;
                        COLORREF OldFg;
                        COLORREF OldBg;

                        SetBkMode(data->backBufferDC, TRANSPARENT);

                        /* rely on GnollHack core helper routine */
                        (void)mapglyph(data->map[i][j], &mgch, &color,
                            &special, i, j);

                        ch = (char)mgch;

                        OldFg = SetTextColor(data->backBufferDC, nhcolor_to_RGB(color));
                        OldBg = SetBkColor(data->backBufferDC, RGB(0,0,0));

                        if (data->bUnicodeFont || SYMHANDLING(H_IBM) || SYMHANDLING(H_UNICODE)) {
                            if (SYMHANDLING(H_UNICODE))
                                wch = (WCHAR)mgch;
                            else if (SYMHANDLING(H_IBM))
                                wch = winos_ascii_to_wide((unsigned char)ch);
                            else
                                wch = (WCHAR)ch;

                            DrawTextW(data->backBufferDC, &wch, 1, rect,
                                DT_CENTER | DT_VCENTER | DT_NOPREFIX
                                | DT_SINGLELINE);
                        }
                        else 
                        {
                            DrawTextA(data->backBufferDC, &ch, 1, rect,
                                DT_CENTER | DT_VCENTER | DT_NOPREFIX
                                | DT_SINGLELINE);
                        }

                        SetTextColor(data->backBufferDC, OldFg);
                        SetBkColor(data->backBufferDC, OldBg);
                    }

                    /* Conditions, status marks, and buffs */
                    int condition_count = 0;
                    if (loc_is_you || (glyph_is_monster(monster_glyph) && mtmp && !is_worm_tail))
                    {
                        if (!mtmp)
                            mtmp = &youmonst;

                        //if (1) // Always true
                        {
                            /* Petmark and other status marks */
                            int mglyph = STATUS_MARKS + GLYPH_UI_TILE_OFF;
                            int mtile = glyph2tile[mglyph];
                            int m_sheet_idx = TILE_SHEET_IDX(mtile);
                            int ct_x = TILEBMP_X(mtile);
                            int ct_y = TILEBMP_Y(mtile);
                            int tiles_per_row = tileWidth / ui_tile_component_array[STATUS_MARKS].width;
                            int max_fitted_rows = (tileHeight - 4) / (ui_tile_component_array[STATUS_MARKS].height + 2);

                            for (int statusorder_idx = STATUS_MARK_PET; statusorder_idx < SIZE(statusmarkorder); statusorder_idx++)
                            {
                                enum game_ui_status_mark_types status_mark = statusmarkorder[statusorder_idx];
                                boolean display_this_status_mark = FALSE;

                                switch (status_mark)
                                {
                                case STATUS_MARK_TOWNGUARD_PEACEFUL:
                                    if (!loc_is_you && ispeaceful && !ispet && is_watch(mtmp->data))
                                        display_this_status_mark = TRUE;
                                    break;
                                case STATUS_MARK_TOWNGUARD_HOSTILE:
                                    if (!loc_is_you && !ispeaceful && !ispet && is_watch(mtmp->data))
                                        display_this_status_mark = TRUE;
                                    break;
                                case STATUS_MARK_PET:
                                    if (!loc_is_you && ispet)
                                        display_this_status_mark = TRUE;
                                    break;
                                case STATUS_MARK_PEACEFUL:
                                    if (!loc_is_you && ispeaceful && !ispet && !is_watch(mtmp->data))
                                        display_this_status_mark = TRUE;
                                    break;
                                case STATUS_MARK_DETECTED:
                                    if (!loc_is_you && data->map[i][j].layer_flags & LFLAGS_M_DETECTED)
                                        display_this_status_mark = TRUE;
                                    break;
                                case STATUS_MARK_PILE:
                                    //if (!loc_is_you && data->map[i][j].layer_flags & LFLAGS_O_PILE)
                                    //    display_this_status_mark = TRUE;
                                    break;
                                case STATUS_MARK_SATIATED:
                                    if (loc_is_you && u.uhs == SATIATED)
                                        display_this_status_mark = TRUE;
                                    break;
                                case STATUS_MARK_HUNGRY:
                                    if ((loc_is_you && u.uhs == HUNGRY)
                                        || (!loc_is_you && ispet && mtmp->mextra && EDOG(mtmp) && monstermoves >= EDOG(mtmp)->hungrytime && EDOG(mtmp)->mhpmax_penalty == 0)
                                        )
                                        display_this_status_mark = TRUE;
                                    break;
                                case STATUS_MARK_WEAK:
                                    if ((loc_is_you && u.uhs == WEAK)
                                        || (!loc_is_you && ispet && mtmp->mextra && EDOG(mtmp) && monstermoves >= EDOG(mtmp)->hungrytime && EDOG(mtmp)->mhpmax_penalty > 0)
                                        )
                                        display_this_status_mark = TRUE;
                                    break;
                                case STATUS_MARK_FAINTING:
                                    if (loc_is_you && u.uhs >= FAINTING)
                                        display_this_status_mark = TRUE;
                                    break;
                                case STATUS_MARK_BURDENED:
                                    if (loc_is_you && u.carrying_capacity_level == SLT_ENCUMBER)
                                        display_this_status_mark = TRUE;
                                    break;
                                case STATUS_MARK_STRESSED:
                                    if (loc_is_you && u.carrying_capacity_level == MOD_ENCUMBER)
                                        display_this_status_mark = TRUE;
                                    break;
                                case STATUS_MARK_STRAINED:
                                    if (loc_is_you && u.carrying_capacity_level == HVY_ENCUMBER)
                                        display_this_status_mark = TRUE;
                                    break;
                                case STATUS_MARK_OVERTAXED:
                                    if (loc_is_you && u.carrying_capacity_level == EXT_ENCUMBER)
                                        display_this_status_mark = TRUE;
                                    break;
                                case STATUS_MARK_OVERLOADED:
                                    if (loc_is_you && u.carrying_capacity_level == OVERLOADED)
                                        display_this_status_mark = TRUE;
                                    break;
                                case STATUS_MARK_2WEP:
                                    if (loc_is_you && u.twoweap)
                                        display_this_status_mark = TRUE;
                                    break;
                                case STATUS_MARK_SKILL:
                                    if (loc_is_you && u.canadvanceskill)
                                        display_this_status_mark = TRUE;
                                    break;
                                case STATUS_MARK_SADDLED:
                                    if (!loc_is_you && (mtmp->worn_item_flags & W_SADDLE) && !Hallucination)
                                        display_this_status_mark = TRUE;
                                    break;
                                case STATUS_MARK_LOW_HP:
                                case STATUS_MARK_CRITICAL_HP:
                                {
                                    if ((loc_is_you && !flags.show_tile_u_hp_bar) || (ispet && !flags.show_tile_pet_hp_bar))
                                    {
                                        int relevant_hp_max = loc_is_you ? (Upolyd ? u.mhmax : u.uhpmax) : mtmp->mhpmax;
                                        int low_threshold = min(relevant_hp_max / 2, max(4, relevant_hp_max / 3));
                                        if (relevant_hp_max < 4)
                                            low_threshold = 0;
                                        int critical_threshold = max(1, min(relevant_hp_max / 4, max(4, relevant_hp_max / 6)));
                                        if (relevant_hp_max < 2)
                                            critical_threshold = 0;

                                        int relevant_hp = loc_is_you ? (Upolyd ? u.mh : u.uhp) : mtmp->mhp;
                                        if (status_mark == STATUS_MARK_CRITICAL_HP && relevant_hp <= critical_threshold)
                                            display_this_status_mark = TRUE;
                                        if (status_mark == STATUS_MARK_LOW_HP && relevant_hp <= low_threshold && relevant_hp > critical_threshold)
                                            display_this_status_mark = TRUE;
                                    }
                                    break;
                                }
                                case STATUS_MARK_SPEC_USED:
                                    if (!loc_is_you && ispet && any_spec_used(mtmp))
                                        display_this_status_mark = TRUE;
                                    break;
                                case STATUS_MARK_TRAPPED:
                                    if ((loc_is_you && u.utrap) || (!loc_is_you && mtmp->mtrapped))
                                        display_this_status_mark = TRUE;
                                    break;
                                case STATUS_MARK_USTUCK:
                                    if (mtmp == u.ustuck && !u.uswallow)
                                        display_this_status_mark = TRUE;
                                    break;
                                case STATUS_MARK_INVENTORY:
                                    if (!loc_is_you && ispet && !mtmp->ispartymember && count_unworn_items(mtmp->minvent) > 0)
                                        display_this_status_mark = TRUE;
                                    break;
                                default:
                                    break;
                                }

                                if (display_this_status_mark)
                                {
                                    int within_tile_x = status_mark % tiles_per_row;
                                    int within_tile_y = status_mark / tiles_per_row;
                                    int c_x = ct_x + within_tile_x * ui_tile_component_array[STATUS_MARKS].width;
                                    int c_y = ct_y + within_tile_y * ui_tile_component_array[STATUS_MARKS].height;

                                    RECT source_rt = { 0 };
                                    source_rt.left = c_x;
                                    source_rt.right = c_x + ui_tile_component_array[STATUS_MARKS].width;
                                    source_rt.top = c_y;
                                    source_rt.bottom = c_y + ui_tile_component_array[STATUS_MARKS].height;

                                    /* Define draw location in target */
                                    int unscaled_left = tileWidth - 2 - ui_tile_component_array[STATUS_MARKS].width;
                                    int unscaled_right = unscaled_left + ui_tile_component_array[STATUS_MARKS].width;
                                    int unscaled_top = 2 + (2 + ui_tile_component_array[STATUS_MARKS].width) * condition_count;
                                    int unscaled_bottom = unscaled_top + ui_tile_component_array[STATUS_MARKS].height;

                                    RECT target_rt = { 0 };
                                    target_rt.left = rect->left + (int)(x_scaling_factor * (double)unscaled_left);
                                    target_rt.right = rect->left + (int)(x_scaling_factor * (double)unscaled_right);
                                    target_rt.top = rect->top + (int)(y_scaling_factor * (double)unscaled_top);
                                    target_rt.bottom = rect->top + (int)(y_scaling_factor * (double)unscaled_bottom);

                                    (*GetNHApp()->lpfnTransparentBlt)(
                                        data->backBufferDC, target_rt.left, target_rt.top,
                                        target_rt.right - target_rt.left, target_rt.bottom - target_rt.top, data->tileDC[m_sheet_idx], source_rt.left,
                                        source_rt.top, source_rt.right - source_rt.left,
                                        source_rt.bottom - source_rt.top, TILE_BK_COLOR);

                                    condition_count++;

                                    if (condition_count >= max_fitted_rows)
                                        break;
                                }
                            }

                            /* Conditions */
                            mglyph = CONDITION_MARKS + GLYPH_UI_TILE_OFF;
                            mtile = glyph2tile[mglyph];
                            m_sheet_idx = TILE_SHEET_IDX(mtile);
                            ct_x = TILEBMP_X(mtile);
                            ct_y = TILEBMP_Y(mtile);
                            tiles_per_row = tileWidth / ui_tile_component_array[CONDITION_MARKS].width;
                            max_fitted_rows = (tileHeight - 4) / (ui_tile_component_array[CONDITION_MARKS].height + 2);

                            /* Number is the same as condition bits */
                            unsigned long m_conditions = get_m_condition_bits(mtmp);
                            for (int cond = 0; cond < ui_tile_component_array[CONDITION_MARKS].number; cond++)
                            {
                                if (condition_count >= max_fitted_rows)
                                    break;

                                int condition_bit = 1 << cond;

#if 0
                                if (!ispet && !loc_is_you)
                                    m_conditions &= ~(BL_MASK_CONF | BL_MASK_STUN | BL_MASK_HALLU | BL_MASK_FEARFUL | BL_MASK_SLEEPING
                                        | BL_MASK_PARALYZED | BL_MASK_SLIME | BL_MASK_STONE | BL_MASK_STRNGL | BL_MASK_SUFFOC);
#endif

                                if (m_conditions & condition_bit)
                                {
                                    int within_tile_x = cond % tiles_per_row;
                                    int within_tile_y = cond / tiles_per_row;
                                    int c_x = ct_x + within_tile_x * ui_tile_component_array[CONDITION_MARKS].width;
                                    int c_y = ct_y + within_tile_y * ui_tile_component_array[CONDITION_MARKS].height;

                                    RECT source_rt = { 0 };
                                    source_rt.left = c_x;
                                    source_rt.right = c_x + ui_tile_component_array[CONDITION_MARKS].width;
                                    source_rt.top = c_y;
                                    source_rt.bottom = c_y + ui_tile_component_array[CONDITION_MARKS].height;

                                    /* Define draw location in target */
                                    int unscaled_left = tileWidth - 2 - ui_tile_component_array[CONDITION_MARKS].width;
                                    int unscaled_right = unscaled_left + ui_tile_component_array[CONDITION_MARKS].width;
                                    int unscaled_top = 2 + (2 + ui_tile_component_array[CONDITION_MARKS].width) * condition_count;
                                    int unscaled_bottom = unscaled_top + ui_tile_component_array[CONDITION_MARKS].height;

                                    RECT target_rt = { 0 };
                                    target_rt.left = rect->left + (int)(x_scaling_factor * (double)unscaled_left);
                                    target_rt.right = rect->left + (int)(x_scaling_factor * (double)unscaled_right);
                                    target_rt.top = rect->top + (int)(y_scaling_factor * (double)unscaled_top);
                                    target_rt.bottom = rect->top + (int)(y_scaling_factor * (double)unscaled_bottom);

                                    /*
                                    (*GetNHApp()->lpfnTransparentBlt)(
                                        hDCcopy, unscaled_left, unscaled_top,
                                        unscaled_right - unscaled_left, unscaled_bottom - unscaled_top, data->tileDC[m_sheet_idx], source_rt.left,
                                        source_rt.top, source_rt.right - source_rt.left,
                                        source_rt.bottom - source_rt.top, TILE_BK_COLOR);
                                    */

                                    (*GetNHApp()->lpfnTransparentBlt)(
                                        data->backBufferDC, target_rt.left, target_rt.top,
                                        target_rt.right - target_rt.left, target_rt.bottom - target_rt.top, data->tileDC[m_sheet_idx], source_rt.left,
                                        source_rt.top, source_rt.right - source_rt.left,
                                        source_rt.bottom - source_rt.top, TILE_BK_COLOR);

                                    condition_count++;
                                }
                            }

                            /* Buffs */
                            for (int propidx = 1; propidx <= LAST_PROP; propidx++)
                            {
                                if (!property_definitions[propidx].show_buff)
                                    continue;

                                long duration = loc_is_you ? (u.uprops[propidx].intrinsic & TIMEOUT) : (long)(mtmp->mprops[propidx] & M_TIMEOUT);
                                if (duration == 0L)
                                    continue;

                                mglyph = (propidx - 1) / BUFFS_PER_TILE + GLYPH_BUFF_OFF;
                                mtile = glyph2tile[mglyph];
                                m_sheet_idx = TILE_SHEET_IDX(mtile);
                                ct_x = TILEBMP_X(mtile);
                                ct_y = TILEBMP_Y(mtile);
                                tiles_per_row = tileWidth / BUFF_WIDTH;
                                max_fitted_rows = (tileHeight - 4) / (BUFF_HEIGHT + 2);

                                if (condition_count >= max_fitted_rows)
                                    break;

                                int smalltileidx = (propidx - 1) % BUFFS_PER_TILE;
                                int within_tile_x = smalltileidx % tiles_per_row;
                                int within_tile_y = smalltileidx / tiles_per_row;
                                int c_x = ct_x + within_tile_x * BUFF_WIDTH;
                                int c_y = ct_y + within_tile_y * BUFF_HEIGHT;

                                RECT source_rt = { 0 };
                                source_rt.left = c_x;
                                source_rt.right = c_x + BUFF_WIDTH;
                                source_rt.top = c_y;
                                source_rt.bottom = c_y + BUFF_HEIGHT;

                                /* Define draw location in target */
                                int unscaled_left = tileWidth - 2 - BUFF_WIDTH;
                                int unscaled_right = unscaled_left + BUFF_WIDTH;
                                int unscaled_top = 2 + (2 + BUFF_WIDTH) * condition_count;
                                int unscaled_bottom = unscaled_top + BUFF_HEIGHT;

                                RECT target_rt = { 0 };
                                target_rt.left = rect->left + (int)(x_scaling_factor * (double)unscaled_left);
                                target_rt.right = rect->left + (int)(x_scaling_factor * (double)unscaled_right);
                                target_rt.top = rect->top + (int)(y_scaling_factor * (double)unscaled_top);
                                target_rt.bottom = rect->top + (int)(y_scaling_factor * (double)unscaled_bottom);

                                (*GetNHApp()->lpfnTransparentBlt)(
                                    data->backBufferDC, target_rt.left, target_rt.top,
                                    target_rt.right - target_rt.left, target_rt.bottom - target_rt.top, data->tileDC[m_sheet_idx], source_rt.left,
                                    source_rt.top, source_rt.right - source_rt.left,
                                    source_rt.bottom - source_rt.top, TILE_BK_COLOR);

                                if (flags.show_buff_timer && (loc_is_you || is_tame(mtmp)))
                                {
                                    /* Add buff timer */
                                    char durbuf[BUFSZ] = "";
                                    Sprintf(durbuf, "%ld", min(999L, duration));
                                    HGDIOBJ savefont = SelectObject(data->backBufferDC, data->hBuffFont);
                                    COLORREF OldFg = 0;
                                    COLORREF OldBg = 0;
                                    COLORREF color = RGB(property_definitions[propidx].buff_text_color.r, property_definitions[propidx].buff_text_color.g, property_definitions[propidx].buff_text_color.b);

                                    OldFg = SetTextColor(data->backBufferDC, color);
                                    if(property_definitions[propidx].buff_text_needs_background)
                                        OldBg = SetBkColor(data->backBufferDC, RGB(property_definitions[propidx].buff_bk_color.r, property_definitions[propidx].buff_bk_color.g, property_definitions[propidx].buff_bk_color.b));

                                    RECT text_rt = target_rt;
                                    int diff = target_rt.right - target_rt.left - 3 * BUFF_TEXT_WIDTH; /* Fit*/
                                    int right_diff_max = rect->right - target_rt.right;
                                    int right_diff = max(0, min(right_diff_max, -diff / 2));
                                    int left_diff = max(0, diff + right_diff);
                                    text_rt.left = target_rt.left + left_diff;
                                    text_rt.right = target_rt.right + right_diff;

                                    if (property_definitions[propidx].buff_text_needs_background)
                                        SetBkMode(data->backBufferDC, OPAQUE);
                                    else
                                        SetBkMode(data->backBufferDC, TRANSPARENT);

                                    SetTextColor(data->backBufferDC, color);

                                    if (data->bUnicodeBuffFont)
                                    {
                                        WCHAR wbuf[BUFSZ];
                                        winos_ascii_to_wide_str(durbuf, wbuf, SIZE(wbuf));
                                        DrawTextW(data->backBufferDC, wbuf, strlen(durbuf), &text_rt,
                                            DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
                                    }
                                    else
                                    {
                                        DrawTextA(data->backBufferDC, durbuf, strlen(durbuf), &text_rt,
                                            DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
                                    }

                                    SetTextColor(data->backBufferDC, OldFg);
                                    if (property_definitions[propidx].buff_text_needs_background)
                                        SetBkColor(data->backBufferDC, OldBg);
                                    SelectObject(data->backBufferDC, savefont);
                                    SetBkMode(data->backBufferDC, TRANSPARENT);

#if 0
                                    int small_duration_pixels = min(BUFF_HEIGHT, duration);
                                    int medium_duration_pixels = min(BUFF_HEIGHT, (duration) / BUFF_HEIGHT);
                                    int large_duration_pixels = min(BUFF_HEIGHT, (duration) / (BUFF_HEIGHT * BUFF_HEIGHT));

                                    double fraction = duration >= BUFF_HEIGHT ? 1.0 : ((double)duration) / ((double)BUFF_HEIGHT);
                                    int bar_idx = large_duration_pixels > 0 ? 2 : medium_duration_pixels > 0 ? 1 : 0;
                                    int bar_pixels = bar_idx == 0 ? small_duration_pixels : bar_idx == 1 ? medium_duration_pixels : large_duration_pixels;


                                    RECT timer_rect;
                                    HGDIOBJ original = SelectObject(data->backBufferDC, GetStockObject(DC_PEN));
                                    SetDCPenColor(data->backBufferDC, color);

                                    int unscaled_top_timer = unscaled_top + BUFF_HEIGHT + 1;
                                    int unscaled_bottom_timer = unscaled_top_timer;
                                    int unscaled_left_timer = unscaled_left;
                                    int unscaled_right_timer = unscaled_left_timer + bar_pixels;
                                    timer_rect.bottom = rect->top + (int)(y_scaling_factor * (double)unscaled_bottom_timer);
                                    timer_rect.top = rect->top + (int)(y_scaling_factor * (double)unscaled_top_timer);
                                    timer_rect.left = rect->left + (int)(x_scaling_factor * (double)unscaled_left_timer);
                                    timer_rect.right = rect->left + (int)(x_scaling_factor * (double)unscaled_right_timer);
                                    MoveToEx(data->backBufferDC, timer_rect.left, timer_rect.bottom, (LPPOINT)NULL);
                                    LineTo(data->backBufferDC, timer_rect.right + 1, timer_rect.bottom);
                                    //FillRect(data->backBufferDC, &timer_rect, hbr_light);
                                    SelectObject(data->backBufferDC, original);
#endif

                                }

                                condition_count++;
                            }

                            /* Steed mark (you as small) */
                            if (loc_is_you && issteed)
                            {
                                int anim_frame_idx = -1, main_tile_idx = -1;
                                int signed_mglyph = u_to_glyph();
                                boolean flip_rider = (signed_mglyph < 0);
                                mglyph = abs(signed_mglyph);
                                mtile = glyph2tile[mglyph];
                                int tile_animation_idx = get_tile_animation_index_from_glyph(mglyph);
                                mtile = maybe_get_replaced_tile(mtile, i, j, data_to_replacement_info(signed_mglyph, base_layer, otmp_round, mtmp, data->map[enl_i][enl_j].layer_flags), (enum auto_drawtypes*)0);
                                mtile = maybe_get_animated_tile(mtile, tile_animation_idx, ANIMATION_PLAY_TYPE_ALWAYS, animation_timers.general_animation_counter, &anim_frame_idx, &main_tile_idx, &data->mapAnimated[i][j], (enum auto_drawtypes*)0);
                                m_sheet_idx = TILE_SHEET_IDX(mtile);
                                int c_x = TILEBMP_X(mtile);
                                int c_y = TILEBMP_Y(mtile);
                                /* Define draw location in target */
                                int unscaled_left = 2 + 2 + BUFF_WIDTH;
                                int unscaled_right = tileWidth - unscaled_left;
                                int unscaled_top = (tileHeight - ((unscaled_right - unscaled_left) * tileHeight) / tileWidth) / 2;
                                int unscaled_bottom = unscaled_top + ((unscaled_right - unscaled_left) * tileHeight) / tileWidth;

                                /* Frame and background first */
                                //HBRUSH hbr_frame = CreateSolidBrush(RGB(100, 50, 0));
                                //HBRUSH hbr_background = CreateSolidBrush(RGB(200, 200, 200));

                                RECT frame_rect = { 0 };
                                frame_rect.left = rect->left + (int)(x_scaling_factor * (double)(unscaled_left - 1));
                                frame_rect.right = rect->left + (int)(x_scaling_factor * (double)(unscaled_right + 1));
                                frame_rect.top = rect->top + (int)(y_scaling_factor * (double)(unscaled_top - 1));
                                frame_rect.bottom = rect->top + (int)(y_scaling_factor * (double)(unscaled_bottom + 1));

                                //FillRect(data->backBufferDC, &frame_rect, hbr_background);
                                //FrameRect(data->backBufferDC, &frame_rect, hbr_frame);

                                /* Now the actual picture */
                                RECT source_rt = { 0 };
                                source_rt.left = c_x;
                                source_rt.right = c_x + tileWidth;
                                source_rt.top = c_y;
                                source_rt.bottom = c_y + tileHeight;

                                RECT target_rt = { 0 };
                                target_rt.left = rect->left + (int)(x_scaling_factor * (double)unscaled_left);
                                target_rt.right = rect->left + (int)(x_scaling_factor * (double)unscaled_right);
                                target_rt.top = rect->top + (int)(y_scaling_factor * (double)unscaled_top);
                                target_rt.bottom = rect->top + (int)(y_scaling_factor * (double)unscaled_bottom);

                                int rhmultiplier = flip_rider ? -1 : 1;

                                (*GetNHApp()->lpfnTransparentBlt)(
                                    data->backBufferDC, target_rt.left, target_rt.top,
                                    target_rt.right - target_rt.left, target_rt.bottom - target_rt.top, data->tileDC[m_sheet_idx], source_rt.left + (flip_rider ? tileWidth - 1 : 0),
                                    source_rt.top, rhmultiplier* (source_rt.right - source_rt.left),
                                    source_rt.bottom - source_rt.top, TILE_BK_COLOR);
                            }
                        }
                    }


                    /* Draw hit point bars */
                    if (mtmp && !is_worm_tail && (
                        (ispet && flags.show_tile_pet_hp_bar)
                        || (loc_is_you && flags.show_tile_u_hp_bar)
                        || (!ispet && !loc_is_you && flags.show_tile_mon_hp_bar && canspotmon(mtmp))
                        ))
                    {
                        int hp = loc_is_you ? (Upolyd ? u.mh : u.uhp) : mtmp->mhp;
                        int hpmax = loc_is_you ? (Upolyd ? u.mhmax : u.uhpmax) : mtmp->mhpmax;
                        double fraction = (hpmax == 0 ? 0 : max(0, min(1, (double)hp / (double)hpmax)));
                        double r_mult = fraction <= 0.25 ? fraction * 2.0 + 0.5 : fraction <= 0.5 ? 1.0 : (1.0 - fraction) * 2.0;
                        double g_mult = fraction <= 0.25 ? 0 : fraction <= 0.5 ? (fraction - 0.25) * 4.0 : 1.0;
                        HBRUSH hbr_dark = CreateSolidBrush(RGB(0, 0, 0));
                        HBRUSH hbr_light = CreateSolidBrush(RGB((int)((double)255 * r_mult), (int)((double)255 * g_mult), 0));
                        RECT smaller_rect, even_smaller_rect;
                        smaller_rect.bottom = rect->bottom;
                        smaller_rect.top = rect->bottom - max(1, (rect->bottom - rect->top) / 12);
                        smaller_rect.left = rect->left;
                        smaller_rect.right = rect->right;
                        even_smaller_rect.bottom = smaller_rect.bottom - (smaller_rect.top <= smaller_rect.bottom - 3 ? 1 : 0);
                        even_smaller_rect.top = smaller_rect.top + (smaller_rect.top <= smaller_rect.bottom - 3 ? 1 : 0);
                        even_smaller_rect.left = smaller_rect.left + (smaller_rect.left <= smaller_rect.right - 3 ? 1 : 0);
                        even_smaller_rect.right = even_smaller_rect.left + (int)(fraction * (double)(smaller_rect.right - (smaller_rect.left <= smaller_rect.right - 3 ? 1 : 0) - even_smaller_rect.left));
                        FillRect(data->backBufferDC, &smaller_rect, hbr_dark);
                        FillRect(data->backBufferDC, &even_smaller_rect, hbr_light);
                        DeleteObject(hbr_dark);
                        DeleteObject(hbr_light);

                    }


                    /* Draw death and hit markers */
                    if (data->map[i][j].layer_flags & LFLAGS_M_KILLED) //glyph_is_dying_monster(monster_glyph) || glyph_is_female_dying_monster(monster_glyph) || glyph_is_dying_player(monster_glyph))
                    {
                        int mglyph = GENERAL_TILE_DEATH + GLYPH_GENERAL_TILE_OFF;
                        int mtile = glyph2tile[mglyph];
                        int m_sheet_idx = TILE_SHEET_IDX(mtile);
                        t_x = TILEBMP_X(mtile);
                        t_y = TILEBMP_Y(mtile);

                        /*
                        (*GetNHApp()->lpfnTransparentBlt)(
                            hDCcopy, 0, 0,
                            GetNHApp()->mapTile_X, GetNHApp()->mapTile_Y, data->tileDC, t_x,
                            t_y, GetNHApp()->mapTile_X,
                            GetNHApp()->mapTile_Y, TILE_BK_COLOR);
                                                    */
                        (*GetNHApp()->lpfnTransparentBlt)(
                            data->backBufferDC, rect->left, rect->top,
                            data->xBackTile, data->yBackTile, data->tileDC[m_sheet_idx], t_x,
                            t_y, GetNHApp()->mapTile_X,
                            GetNHApp()->mapTile_Y, TILE_BK_COLOR);
                    }
                    else if (data->map[i][j].layer_flags & LFLAGS_M_BEING_HIT)
                    {
                        short hit_text_num = data->map[i][j].hit_tile;
                        int mglyph = max(0, min(MAX_HIT_TILES - 1, (int)hit_text_num)) + GLYPH_HIT_TILE_OFF;
                        int mtile = glyph2tile[mglyph];
                        int m_sheet_idx = TILE_SHEET_IDX(mtile);
                        t_x = TILEBMP_X(mtile);
                        t_y = TILEBMP_Y(mtile);

                        /*
                        (*GetNHApp()->lpfnTransparentBlt)(
                            hDCcopy, 0, 0,
                            GetNHApp()->mapTile_X, GetNHApp()->mapTile_Y, data->tileDC[m_sheet_idx], t_x,
                            t_y, GetNHApp()->mapTile_X,
                            GetNHApp()->mapTile_Y, TILE_BK_COLOR);
                        */
                        (*GetNHApp()->lpfnTransparentBlt)(
                            data->backBufferDC, rect->left, rect->top,
                            data->xBackTile, data->yBackTile, data->tileDC[m_sheet_idx], t_x,
                            t_y, GetNHApp()->mapTile_X,
                            GetNHApp()->mapTile_Y, TILE_BK_COLOR);
                    }


                }

            }
        //}
    }

    SelectObject(hDCcopy, oldbmp);
    DeleteDC(hDCcopy);
    DeleteObject(newhBmp);

    SelectObject(hDCmonster, oldBmpMonster);
    DeleteDC(hDCmonster);
    DeleteObject(newhBmpMonster);
}


static void
paintGlyph(PNHMapWindow data, int i, int j, RECT * rect)
{
    //if (data->map[i][j].glyph >= 0) // Always true
    {

        char ch;
        WCHAR wch;
        int color;
        unsigned long special;
        nhsym mgch;
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
        (void) mapglyph(data->map[i][j], &mgch, &color,
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
        if (data->bUnicodeFont || SYMHANDLING(H_UNICODE) || SYMHANDLING(H_IBM)) { /* For H_IBM note that DrawTextA uses CP850, not CP437 */
            if (SYMHANDLING(H_UNICODE))
                wch = (WCHAR)mgch;
            else if (SYMHANDLING(H_IBM))
                wch = winos_ascii_to_wide((unsigned char)ch);
            else
                wch = (WCHAR)ch;

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

    if (context.game_started && i == data->xCur && j == data->yCur &&
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
    boolean layer_different = FALSE;
    for (enum layer_types layer_idx = LAYER_FLOOR; layer_idx < MAX_GLYPH; layer_idx++)
    {
        if (data->map[i][j].layer_glyphs[layer_idx] != layers.layer_glyphs[layer_idx])
        {
            layer_different = TRUE;
            break;
        }
    }

    if ((data->map[i][j].glyph != layers.glyph) || (data->map[i][j].glyph != layers.bkglyph) || layer_different
        || data->map[i][j].layer_flags != layers.layer_flags || data->map[i][j].damage_displayed != layers.damage_displayed 
        || data->map[i][j].special_monster_layer_height != layers.special_monster_layer_height
        )
    {
        dirty(data, i, j, TRUE);

        data->map[i][j] = layers;

        dirty(data, i, j, FALSE);
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
    for (int x = 1; x < COLNO; x++)
        for (int y = 0; y < ROWNO; y++) 
        {
            clear_layer_info(&data->map[x][y]);
            /*
            data->map[x][y] = zerolayerinfo;
            data->map[x][y].glyph = base_cmap_to_glyph(S_unexplored);
            data->map[x][y].bkglyph = NO_GLYPH;
            for (enum layer_types i = LAYER_FLOOR; i < MAX_LAYERS; i++)
            {
                data->map[x][y].layer_glyphs[i] = NO_GLYPH;
            }
            */
            data->mapDirty[x][y] = TRUE;
            data->mapAnimated[x][y] = 0;
        }
    InvalidateRect(data->hWnd, NULL, FALSE);
}

static void setDrawOrder(PNHMapWindow data)
{
    int draw_count = 0;
    /* First, draw floors, no enlargements here */
    data->draw_order[draw_count].enlargement_index = -1;
    data->draw_order[draw_count].layer = LAYER_FLOOR;
    data->draw_order[draw_count].tile_movement_index = 0;
    data->draw_order[draw_count].source_dir_index = 0;
    draw_count++;

    int same_level_z_order_array[3] = { 0, -1, 1 };
    int different_level_z_order_array[3] = { 2, 3, 4 };
    boolean draw_monster_shadow_placed = FALSE;

#define NUM_LAYER_PARTITIONS 7

    for (int layer_partition = 0; layer_partition < NUM_LAYER_PARTITIONS; layer_partition++)
    {
        enum layer_types layer_partition_start[NUM_LAYER_PARTITIONS + 1] = { LAYER_FLOOR + 1, LAYER_LEASH, LAYER_ENVIRONMENT, LAYER_ZAP, LAYER_GENERAL_EFFECT, LAYER_MONSTER_EFFECT, LAYER_GENERAL_UI, MAX_LAYERS };

        for (enum layer_types layer_idx = layer_partition_start[layer_partition]; layer_idx < layer_partition_start[layer_partition + 1]; layer_idx++)
        {
            /* Draw monster shadow */
            if (layer_idx == IDX_LAYER_MONSTER_SHADOW + 1 && !draw_monster_shadow_placed)
            {
                data->draw_order[draw_count - 1].draw_monster_shadow = 1;
                draw_monster_shadow_placed = TRUE;
            }

            for (int enl_idx = 0; enl_idx <= 2; enl_idx++)
            {
                if ((layer_idx == LAYER_LEASH || layer_idx == LAYER_ZAP) && same_level_z_order_array[enl_idx] != -1)
                {
                    continue;
                }

                data->draw_order[draw_count].enlargement_index = same_level_z_order_array[enl_idx];
                data->draw_order[draw_count].layer = layer_idx;
                data->draw_order[draw_count].tile_movement_index = 0;
                data->draw_order[draw_count].source_dir_index = 0;
                draw_count++;

                if (layer_idx == LAYER_MONSTER || layer_idx == LAYER_MONSTER_EFFECT)
                {
                    /* These are in fact not drawn */
                    data->draw_order[draw_count].enlargement_index = same_level_z_order_array[enl_idx];
                    data->draw_order[draw_count].layer = layer_idx;
                    data->draw_order[draw_count].tile_movement_index = -1;
                    data->draw_order[draw_count].source_dir_index = 0;
                    draw_count++;

                    /* These are drawn at the same time as lower positioned tiles */
                    data->draw_order[draw_count].enlargement_index = different_level_z_order_array[enl_idx];
                    data->draw_order[draw_count].layer = layer_idx;
                    data->draw_order[draw_count].tile_movement_index = -1;
                    data->draw_order[draw_count].source_dir_index = 0;
                    draw_count++;
                }

                if (layer_idx == LAYER_MONSTER && same_level_z_order_array[enl_idx] == -1)
                {
                    for (int i = 1; i <= NUM_WORM_SOURCE_DIRS; i++)
                    {
                        data->draw_order[draw_count].enlargement_index = same_level_z_order_array[enl_idx];
                        data->draw_order[draw_count].layer = layer_idx;
                        data->draw_order[draw_count].tile_movement_index = 0;
                        data->draw_order[draw_count].source_dir_index = i * 2;
                        draw_count++;
                    }
                }

                if (layer_idx == LAYER_CHAIN && same_level_z_order_array[enl_idx] == -1)
                {
                    for (int i = 1; i <= NUM_CHAIN_SOURCE_DIRS; i++)
                    {

                        data->draw_order[draw_count].enlargement_index = same_level_z_order_array[enl_idx];
                        data->draw_order[draw_count].layer = layer_idx;
                        data->draw_order[draw_count].tile_movement_index = 0;
                        data->draw_order[draw_count].source_dir_index = i * 2;
                        draw_count++;
                    }
                }

                if (layer_idx == LAYER_ZAP && same_level_z_order_array[enl_idx] == -1)
                {
                    /* From below (i == 3) is drawn below */
                    for (int i = 1; i <= NUM_ZAP_SOURCE_DIRS; i++)
                    {
                        if (((i - 1) % NUM_ZAP_SOURCE_BASE_DIRS + 1) >= 5 && ((i - 1) % NUM_ZAP_SOURCE_BASE_DIRS + 1) <= 7)
                            continue;
                        data->draw_order[draw_count].enlargement_index = same_level_z_order_array[enl_idx];
                        data->draw_order[draw_count].layer = layer_idx;
                        data->draw_order[draw_count].tile_movement_index = 0;
                        data->draw_order[draw_count].source_dir_index = i;
                        draw_count++;
                    }
                }
            }
        }
        /* Mark to be drawn to back buffer and darkened if needed */
        /* Note these all use the darkness of the target tile, so they will be shaded similarly */
        /* Monster tile mark will be potentially darkened, other UI symbols come on the top undarkened */
        data->draw_order[draw_count - 1].draw_to_buffer = 1;

        for (enum layer_types layer_idx = layer_partition_start[layer_partition]; layer_idx < layer_partition_start[layer_partition + 1]; layer_idx++)
        {
            for (int enl_idx = 0; enl_idx <= 2; enl_idx++)
            {
                if (layer_idx == LAYER_LEASH)
                {
                    continue;
                }
                else if (layer_idx == LAYER_ZAP)
                {
                    if (different_level_z_order_array[enl_idx] == 3)
                    {
                        /* Others (i == 1-4,8) have been drawn earlier; from below (i == 5,6,7) is drawn here */
                        for (int j = 0; j <= 1; j++)
                        {
                            for (int i = 5 + j * +NUM_ZAP_SOURCE_BASE_DIRS; i <= 7 + j * +NUM_ZAP_SOURCE_BASE_DIRS; i++)
                            {
                                data->draw_order[draw_count].enlargement_index = -1; // different_level_z_order_array[enl_idx];
                                data->draw_order[draw_count].layer = layer_idx;
                                data->draw_order[draw_count].tile_movement_index = 0;
                                data->draw_order[draw_count].source_dir_index = i;
                                draw_count++;
                            }
                        }
                    }
                    continue;
                }

                data->draw_order[draw_count].enlargement_index = different_level_z_order_array[enl_idx];
                data->draw_order[draw_count].layer = layer_idx;
                data->draw_order[draw_count].tile_movement_index = 0;
                data->draw_order[draw_count].source_dir_index = 0;
                draw_count++;

                if (layer_idx == LAYER_MONSTER || layer_idx == LAYER_MONSTER_EFFECT)
                {
                    /* These two are drawn at the same time as the higher positioned tiles */
                    data->draw_order[draw_count].enlargement_index = same_level_z_order_array[enl_idx];
                    data->draw_order[draw_count].layer = layer_idx;
                    data->draw_order[draw_count].tile_movement_index = 1;
                    data->draw_order[draw_count].source_dir_index = 0;
                    draw_count++;

                    data->draw_order[draw_count].enlargement_index = different_level_z_order_array[enl_idx];
                    data->draw_order[draw_count].layer = layer_idx;
                    data->draw_order[draw_count].tile_movement_index = 1;
                    data->draw_order[draw_count].source_dir_index = 0;
                    draw_count++;
                }
            }

        }
        /* Mark to be drawn to back buffer and darkened if needed */
        /* Note these all use the darkness of the tile below the target, so they will be shaded similarly */
        data->draw_order[draw_count - 1].draw_to_buffer = 1;
    }

}

static void dirtyAll(PNHMapWindow data)
{
    for (int i = 1; i < COLNO; i++)
        for (int j = 0; j < ROWNO; j++)
            data->mapDirty[i][j] = TRUE;

    InvalidateRect(data->hWnd, NULL, FALSE);
}

static void dirty(PNHMapWindow data, int x, int y, boolean usePrinted)
{
    if (program_state.freeing_dynamic_data == 1 || program_state.exiting > 0 || in_mklev || program_state.in_bones)
        return;

    data->mapDirty[x][y] = TRUE;

    RECT rt;
    nhcoord2display(data, x, y, &rt); //data->xCur, data->yCur

    InvalidateRect(data->hWnd, &rt, FALSE);

    for (int dir = 0; dir < 4; dir++)
    {
        int rx = 0;
        int ry = 0;
        unsigned long dir_bit = 0;
        switch (dir)
        {
        case 0:
            rx = x - 1;
            ry = y;
            dir_bit = REPLACEMENT_EVENT_UPDATE_FROM_RIGHT;
            break;
        case 1:
            rx = x + 1;
            ry = y;
            dir_bit = REPLACEMENT_EVENT_UPDATE_FROM_LEFT;
            break;
        case 2:
            rx = x;
            ry = y - 1;
            dir_bit = REPLACEMENT_EVENT_UPDATE_FROM_BELOW;
            break;
        case 3:
            rx = x;
            ry = y + 1;
            dir_bit = REPLACEMENT_EVENT_UPDATE_FROM_TOP;
            break;
        default:
            break;
        }
        if (isok(rx, ry))
        {
            for (int i = 0; i < 2; i++)
            {
                int tile_upside = abs(data->map[rx][ry].glyph) < MAX_GLYPH ? glyph2tile[abs(data->map[rx][ry].glyph)] : -1;
                int bktile_upside = abs(data->map[rx][ry].bkglyph) < MAX_GLYPH ? glyph2tile[abs(data->map[rx][ry].bkglyph)] : -1;
                short replacement_idx_upside = tile_upside >= 0 ? tile2replacement[tile_upside] : 0;
                short bk_replacement_idx_upside = bktile_upside >= 0 ? tile2replacement[bktile_upside] : 0;

                short used_ridx = (i == 0 ? replacement_idx_upside : bk_replacement_idx_upside);
                int used_tile = (i == 0 ? tile_upside : bktile_upside);
                if (used_ridx > 0)
                {
                    /* Update tile area */
                    if (replacements[used_ridx].replacement_events & dir_bit)
                    {
                        data->mapDirty[rx][ry] = TRUE;
                        RECT rt2;
                        nhcoord2display(data, rx, ry, &rt2); //data->xCur, data->yCur
                        InvalidateRect(data->hWnd, &rt2, FALSE);
                    }
                }
            }
        }
    }

    if (glyph_is_zap(data->map[x][y].layer_glyphs[LAYER_ZAP]))
    {
        int rx = x, ry = y;
        for (int source_dir_idx = 1; source_dir_idx <= NUM_ZAP_SOURCE_DIRS; source_dir_idx++)
        {
            if (zap_glyph_to_corner_glyph(data->map[x][y].layer_glyphs[LAYER_ZAP], data->map[x][y].layer_flags, source_dir_idx) != NO_GLYPH)
            {
                switch ((source_dir_idx - 1) % NUM_ZAP_SOURCE_BASE_DIRS + 1)
                {
                case 1:
                    rx = x + 1;
                    ry = y + 1;
                    break;
                case 2:
                    rx = x;
                    ry = y + 1;
                    break;
                case 3:
                    rx = x - 1;
                    ry = y + 1;
                    break;
                case 4:
                    rx = x - 1;
                    ry = y;
                    break;
                case 5:
                    rx = x - 1;
                    ry = y - 1;
                    break;
                case 6:
                    rx = x;
                    ry = y - 1;
                    break;
                case 7:
                    rx = x + 1;
                    ry = y - 1;
                    break;
                case 8:
                    rx = x + 1;
                    ry = y;
                    break;
                default:
                    break;
                }
                if (isok(rx, ry))
                {
                    data->mapDirty[rx][ry] = TRUE;
                    RECT rt2;
                    nhcoord2display(data, rx, ry, &rt2); //data->xCur, data->yCur
                    InvalidateRect(data->hWnd, &rt2, FALSE);
                }
            }
        }
    }

    if (data->map[x][y].layer_flags & LFLAGS_O_CHAIN)
    {
        int rx = x, ry = y;
        for (int source_dir_idx = 2; source_dir_idx <= NUM_CHAIN_SOURCE_DIRS * 2; source_dir_idx = source_dir_idx + 2)
        {
            switch (source_dir_idx)
            {
            case 2:
                rx = x;
                ry = y + 1;
                break;
            case 4:
                rx = x - 1;
                ry = y;
                break;
            case 6:
                rx = x;
                ry = y - 1;
                break;
            case 8:
                rx = x + 1;
                ry = y;
                break;
            default:
                break;
            }
            if (isok(rx, ry))
            {
                data->mapDirty[rx][ry] = TRUE;
                RECT rt2;
                nhcoord2display(data, rx, ry, &rt2); //data->xCur, data->yCur
                InvalidateRect(data->hWnd, &rt2, FALSE);
            }
        }
    }

    if (data->map[x][y].layer_flags & LFLAGS_M_WORM_TAIL)
    {
        int rx = x, ry = y;
        for (int source_dir_idx = 2; source_dir_idx <= NUM_WORM_SOURCE_DIRS * 2; source_dir_idx = source_dir_idx + 2)
        {
            switch (source_dir_idx)
            {
            case 2:
                rx = x;
                ry = y + 1;
                break;
            case 4:
                rx = x - 1;
                ry = y;
                break;
            case 6:
                rx = x;
                ry = y - 1;
                break;
            case 8:
                rx = x + 1;
                ry = y;
                break;
            default:
                break;
            }
            if (isok(rx, ry))
            {
                data->mapDirty[rx][ry] = TRUE;
                RECT rt2;
                nhcoord2display(data, rx, ry, &rt2); //data->xCur, data->yCur
                InvalidateRect(data->hWnd, &rt2, FALSE);
            }
        }
    }

    if (data->map[x][y].missile_flags & MISSILE_FLAGS_TETHERED)
    {
        int ux = data->map[x][y].leash_mon_x[MAXLEASHED], uy = data->map[x][y].leash_mon_y[MAXLEASHED];
        if (isok(ux, uy))
        {
            int min_x = min(ux, x);
            int min_y = min(uy, y);
            int max_x = max(ux, x);
            int max_y = max(uy, y);
            for (int rx = min_x; rx <= max_x; rx++)
            {
                for (int ry = min_y; ry <= max_y; ry++)
                {
                    data->mapDirty[rx][ry] = TRUE;
                    RECT rt2;
                    nhcoord2display(data, rx, ry, &rt2); //data->xCur, data->yCur
                    InvalidateRect(data->hWnd, &rt2, FALSE);
                }
            }
        }
    }

    if (data->map[x][y].layer_flags & LFLAGS_M_TETHERED)
    {
        int ux = data->map[x][y].leash_mon_x[MAXLEASHED], uy = data->map[x][y].leash_mon_y[MAXLEASHED];
        if (isok(ux, uy))
        {
            int min_x = min(ux, x);
            int min_y = min(uy, y);
            int max_x = max(ux, x);
            int max_y = max(uy, y);
            for (int rx = min_x; rx <= max_x; rx++)
            {
                for (int ry = min_y; ry <= max_y; ry++)
                {
                    data->mapDirty[rx][ry] = TRUE;
                    RECT rt2;
                    nhcoord2display(data, rx, ry, &rt2); //data->xCur, data->yCur
                    InvalidateRect(data->hWnd, &rt2, FALSE);
                }
            }
        }
    }

    if (data->map[x][y].layer_flags & LFLAGS_U_TETHERED)
    {
        for (int i = 0; i < MAXLEASHED; i++)
        {
            int mx = data->map[x][y].leash_mon_x[i], my = data->map[x][y].leash_mon_y[i];
            if (isok(mx, my))
            {
                int min_x = min(mx, x);
                int min_y = min(my, y);
                int max_x = max(mx, x);
                int max_y = max(my, y);
                for (int rx = min_x; rx <= max_x; rx++)
                {
                    for (int ry = min_y; ry <= max_y; ry++)
                    {
                        data->mapDirty[rx][ry] = TRUE;
                        RECT rt2;
                        nhcoord2display(data, rx, ry, &rt2); //data->xCur, data->yCur
                        InvalidateRect(data->hWnd, &rt2, FALSE);
                    }
                }
            }
        }
#if 0
        for (struct monst* leashed_mon = fmon; leashed_mon; leashed_mon = leashed_mon->nmon)
        {
            if (leashed_mon->mleashed)
            {
                int mx = leashed_mon->mx, my = leashed_mon->my;
                if (isok(mx, my))
                {
                    int min_x = min(mx, x);
                    int min_y = min(my, y);
                    int max_x = max(mx, x);
                    int max_y = max(my, y);
                    for (int rx = min_x; rx <= max_x; rx++)
                    {
                        for (int ry = min_y; ry <= max_y; ry++)
                        {
                            data->mapDirty[rx][ry] = TRUE;
                            RECT rt2;
                            nhcoord2display(data, rx, ry, &rt2); //data->xCur, data->yCur
                            InvalidateRect(data->hWnd, &rt2, FALSE);
                        }
                    }
                }
            }
        }
#endif
    }

    for (enum layer_types layer_idx = LAYER_FLOOR/*-2*/; layer_idx < MAX_LAYERS; layer_idx++)
    {
        int layer_rounds = 1;
        struct obj* otmp = (struct obj*)0;
        int ntile = -1;
        if (layer_idx == LAYER_OBJECT || layer_idx == LAYER_COVER_OBJECT)
        {
            layer_rounds = MAX_SHOWN_OBJECTS;
            if (!usePrinted)
            {
                if (!cansee(x, y) || (data->map[x][y].layer_flags & LFLAGS_SHOWING_MEMORY))
                    otmp = level.locations[x][y].hero_memory_layers.memory_objchn;
                else
                    otmp = level.objects[x][y];
            }
        }

        for (int layer_round = 0; layer_round < layer_rounds; layer_round++)
        {            
            int enlarg = 0;
            boolean flipped = 0;
            if (layer_idx == LAYER_OBJECT || layer_idx == LAYER_COVER_OBJECT)
            {
                if (usePrinted)
                {
                    if (layer_idx == LAYER_OBJECT)
                    {
                        ntile = data->printedObjectTile[x][y][layer_round];
                    }
                    else if (layer_idx == LAYER_COVER_OBJECT)
                    {
                        ntile = data->printedCoverObjectTile[x][y][layer_round];
                    }

                    if (ntile >= 0)
                        enlarg = tile2enlargement[ntile]; //[glyph2tile[abs(obj_to_glyph(otmp, rn2_on_display_rng))]];
                }
                else
                {
                    if (layer_round > 0 && otmp)
                        otmp = otmp->nexthere;

                    if (otmp)
                    {
                        int glyph = abs(otmp->glyph);
                        if (glyph <= 0 || glyph >= MAX_GLYPH)
                            continue;
                        int ntile = glyph2tile[glyph];
                        enum autodraw_types autodraw = AUTODRAW_NONE;
                        ntile = maybe_get_replaced_tile(ntile, x, y, data_to_replacement_info(otmp->glyph, layer_idx, otmp, m_at(x, y), data->map[x][y].layer_flags), &autodraw);
                        enlarg = tile2enlargement[ntile]; // obj_to_glyph(otmp, rn2_on_display_rng))]];
                    }
                }
            }
            else
            {
                char mapanimateddummy = 0;
                enum autodraw_types autodraw = AUTODRAW_NONE;
                int anim_frame_idx = -1, main_tile_idx = -1;
                int signed_glyph = data->map[x][y].layer_glyphs[layer_idx];
                int glyph = abs(signed_glyph);
                if (glyph <= 0 || glyph >= MAX_GLYPH)
                    continue;
                flipped = (signed_glyph < 0);
                int ntile = glyph2tile[glyph];
                ntile = maybe_get_replaced_tile(ntile, x, y, data_to_replacement_info(signed_glyph, layer_idx, (struct obj*)0, m_at(x, y), data->map[x][y].layer_flags), &autodraw);

                int tile_animation_idx = get_tile_animation_index_from_glyph(glyph);
                boolean is_dropping_piercer = m_at(x, y) && (data->map[x][y].layer_flags & LFLAGS_M_DROPPING_PIERCER);

                if (animation_timers.u_action_animation_counter_on && layer_idx == LAYER_MONSTER && x == u.ux && y == u.uy)
                    ntile = maybe_get_animated_tile(ntile, tile_animation_idx, ANIMATION_PLAY_TYPE_PLAYED_SEPARATELY, animation_timers.u_action_animation_counter, &anim_frame_idx, &main_tile_idx, &mapanimateddummy, &autodraw);
                else if (animation_timers.m_action_animation_counter_on && ((!is_dropping_piercer && layer_idx == LAYER_MONSTER) || (is_dropping_piercer && layer_idx == LAYER_MISSILE)) && animation_timers.m_action_animation_x == x && animation_timers.m_action_animation_y == y)
                    ntile = maybe_get_animated_tile(ntile, tile_animation_idx, ANIMATION_PLAY_TYPE_PLAYED_SEPARATELY, animation_timers.m_action_animation_counter, &anim_frame_idx, &main_tile_idx, &mapanimateddummy, &autodraw);
                else if (glyph_is_explosion(glyph))
                    ntile = maybe_get_animated_tile(ntile, tile_animation_idx, ANIMATION_PLAY_TYPE_PLAYED_SEPARATELY, animation_timers.explosion_animation_counter, &anim_frame_idx, &main_tile_idx, &mapanimateddummy, &autodraw);
                else if (glyph_is_zap(glyph))
                {
                    for (int zap_anim_idx = 0; zap_anim_idx < MAX_PLAYED_ZAP_ANIMATIONS; zap_anim_idx++)
                    {
                        if (animation_timers.zap_animation_counter_on[zap_anim_idx]
                            && x == animation_timers.zap_animation_x[zap_anim_idx]
                            && y == animation_timers.zap_animation_y[zap_anim_idx])
                        {
                            ntile = maybe_get_animated_tile(ntile, tile_animation_idx, ANIMATION_PLAY_TYPE_PLAYED_SEPARATELY, animation_timers.zap_animation_counter[zap_anim_idx], &anim_frame_idx, &main_tile_idx, &mapanimateddummy, &autodraw);
                            break;
                        }
                    }
                }
                else
                {
                    /* Check for special effect animations */
                    boolean spef_found = FALSE;
                    for (int spef_idx = 0; spef_idx < MAX_PLAYED_SPECIAL_EFFECTS; spef_idx++)
                    {
                        if (animation_timers.special_effect_animation_counter_on[spef_idx]
                            && layer_idx == animation_timers.spef_action_animation_layer[spef_idx]
                            && x == animation_timers.spef_action_animation_x[spef_idx]
                            && y == animation_timers.spef_action_animation_y[spef_idx])
                        {
                            ntile = maybe_get_animated_tile(ntile, tile_animation_idx, ANIMATION_PLAY_TYPE_PLAYED_SEPARATELY, animation_timers.special_effect_animation_counter[spef_idx], &anim_frame_idx, &main_tile_idx, &mapanimateddummy, &autodraw);
                            spef_found = TRUE;
                            break;
                        }
                    }

                    /* Otherwise, normal animation check */
                    if (!spef_found)
                        ntile = maybe_get_animated_tile(ntile, tile_animation_idx, ANIMATION_PLAY_TYPE_ALWAYS, animation_timers.general_animation_counter, &anim_frame_idx, &main_tile_idx, &mapanimateddummy, &autodraw);
                }
                enlarg = tile2enlargement[ntile];
            }

            int monster_layer_height = (int)data->map[x][y].special_monster_layer_height;
            if (monster_layer_height > 0 && isok(x, y - 1) && data->mapDirty[x][y - 1] == FALSE)
            {
                data->mapDirty[x][y - 1] = TRUE;
                nhcoord2display(data, x, y - 1, &rt);
                InvalidateRect(data->hWnd, &rt, FALSE);
            }

            if (enlarg > 0)
            {
                int enl_x = -1;
                int enl_y = -1;
                for (int i = 0; i < NUM_POSITIONS_IN_ENLARGEMENT; i++)
                {
                    if (enlargements[enlarg].position2tile[i] == -1)
                        continue;

                    int pos_mult = flipped ? -1 : 1;

                    if (i <= 2)
                        enl_x = x + pos_mult * (i - 1);
                    else if (i == 3)
                        enl_x = x - pos_mult;
                    else if (i == 4)
                        enl_x = x + pos_mult;

                    if (i <= 2)
                        enl_y = y - 1;
                    else
                        enl_y = y;

                    if (isok(enl_x, enl_y) && data->mapDirty[enl_x][enl_y] == FALSE)
                    {
                        data->mapDirty[enl_x][enl_y] = TRUE;
                        nhcoord2display(data, enl_x, enl_y, &rt);
                        InvalidateRect(data->hWnd, &rt, FALSE);
                    }

                    if (monster_layer_height > 0 && isok(enl_x, enl_y - 1) && data->mapDirty[enl_x][enl_y - 1] == FALSE)
                    {
                        data->mapDirty[enl_x][enl_y - 1] = TRUE;
                        nhcoord2display(data, enl_x, enl_y - 1, &rt);
                        InvalidateRect(data->hWnd, &rt, FALSE);
                    }
                    else if (monster_layer_height < 0 && isok(enl_x, min(y, enl_y + 1)) && data->mapDirty[enl_x][min(y, enl_y + 1)] == FALSE)
                    {
                        data->mapDirty[enl_x][min(y, enl_y + 1)] = TRUE;
                        nhcoord2display(data, enl_x, min(y, enl_y + 1), &rt);
                        InvalidateRect(data->hWnd, &rt, FALSE);
                    }
                }
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

    if (data->bAsciiMode || Is_really_rogue_level(&u.uz))
    {
        paintGlyph(data, i, j, &rect);
    } 
    else 
    {
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
    HBITMAP savedBitmap[MAX_TILE_SHEETS] = { 0 };
    
    for(int i = 0; i < GetNHApp()->mapTileSheets; i++)
        savedBitmap[i] = SelectObject(data->tileDC[i], GetNHApp()->bmpMapTiles[i]);

    for (int i = 1; i < COLNO; i++)
        for (int j = 0; j < ROWNO; j++)
            if (data->mapDirty[i][j])
                paint(data, i, j);

    for (int i = 0; i < GetNHApp()->mapTileSheets; i++)
        SelectObject(data->tileDC[i], savedBitmap[i]);

    PAINTSTRUCT ps;
    HDC hFrontBufferDC = BeginPaint(hWnd, &ps);

    /* stretch back buffer onto front buffer window */
    int frontWidth = (COLNO - 1) * data->xFrontTile;
    int frontHeight = ROWNO * data->yFrontTile;

    SetStretchBltMode(hFrontBufferDC, COLORONCOLOR);
    StretchBlt(hFrontBufferDC,
        data->map_orig.x - (data->xPos * data->xFrontTile),
        data->map_orig.y - (data->yPos * data->yFrontTile), frontWidth, frontHeight,
                data->backBufferDC, data->xBackTile, 0, data->backWidth - data->xBackTile, data->backHeight, SRCCOPY);

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
    lpOut->left = (x - 1 - data->xPos) * data->xFrontTile + data->map_orig.x;
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

#define zap_color(n) *color = iflags.use_color ? zap_type_definitions[n].color : NO_COLOR
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

}
     if ((offset = (g - GLYPH_FEMALE_BODY_OFF)) >= 0) 
     { /* a corpse */
         *ch = showsyms[(int)objects[CORPSE].oc_class + SYM_OFF_O];
         mon_color(offset);
     }
     else if ((offset = (g - GLYPH_BODY_OFF)) >= 0)
     { /* a corpse */
         *ch = showsyms[(int)objects[CORPSE].oc_class + SYM_OFF_O];
         mon_color(offset);
     }
     else if ((offset = (g - GLYPH_WARNING_OFF)) >= 0)
     { /* a warning flash */
        *ch = showsyms[offset + SYM_OFF_W];
        warn_color(offset);
     }
     else if ((offset = (g - GLYPH_SWALLOW_OFF)) >= 0) 
     { /* swallow */
        /* see swallow_to_glyph() in display.c */
        *ch = (uchar) showsyms[(S_sw_tl + (offset & 0x7)) + SYM_OFF_P];
        mon_color(offset >> 3);
     } 
     else if ((offset = (g - GLYPH_ZAP_OFF)) >= 0)
     { /* zap beam */
        /* see zapdir_to_glyph() in display.c */
        *ch = showsyms[(S_vbeam + (offset & 0x3)) + SYM_OFF_P];
        zap_color((offset >> 2));
     } 
     else if ((offset = (g - GLYPH_CMAP_OFF)) >= 0) 
     { /* cmap */
        int cmap_type_idx = offset / NUM_CMAP_TYPE_CHARS;
        int idx = offset - cmap_type_idx * NUM_CMAP_TYPE_CHARS + SYM_OFF_P;
        *ch = showsyms[idx];
        cmap_color(offset, cmap_type_idx);
     } 
     else if ((offset = (g - GLYPH_OBJ_MISSILE_OFF)) >= 0) 
     { /* object */
        *ch = showsyms[(int) objects[offset].oc_class + SYM_OFF_O];
        obj_color(offset);
     } 
     else if ((offset = (g - GLYPH_OBJ_OFF)) >= 0) 
     { /* object */
        *ch = showsyms[(int) objects[offset].oc_class + SYM_OFF_O];
     } 
     else 
     { /* a monster */
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

