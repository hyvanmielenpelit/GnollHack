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
#include "artifact.h"
#include "artilist.h"
#include "general.h"

#define NHMAP_FONT_NAME TEXT("Terminal")
#define NHMAP_TTFONT_NAME TEXT("Consolas")
#define MAXWINDOWTEXT 255

#define CURSOR_BLINK_IN_INTERVALS 25
#define CURSOR_HEIGHT 2 // pixels

#define DRAW_ORDER_SIZE ((NUM_POSITIONS_IN_ENLARGEMENT + 1) * (MAX_LAYERS - 1 + 2 * 2) + 1)


/* draw order definition */
struct draw_order_definition {
    enum layer_types layer;
    int enlargement_index;
    int tile_movement_index;
    uchar draw_to_buffer;
};


/* map window data */
typedef struct mswin_GnollHack_map_window {
    HWND hWnd;                  /* window */

    struct layer_info map[COLNO][ROWNO];    /* glyph map */
    boolean mapAnimated[COLNO][ROWNO];      /* animation flag for map */
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
    HBITMAP hBackBuffer;        /* back buffer bitmap */
    HDC backBufferDC;           /* back buffer drawing context */

    HDC tileDC;                /* tile drawing context */

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

        if (context.u_action_animation_counter_on)
        {
            if (context.u_action_animation_counter >= 0xCFFFFFFFUL)
                context.u_action_animation_counter = 0UL;
            else
                context.u_action_animation_counter++;
        }

        if (context.m_action_animation_counter_on)
        {
            if (context.m_action_animation_counter >= 0xCFFFFFFFUL)
                context.m_action_animation_counter = 0UL;
            else
                context.m_action_animation_counter++;
        }

        if (context.explosion_animation_counter_on)
        {
            if (context.explosion_animation_counter >= 0xCFFFFFFFUL)
                context.explosion_animation_counter = 0UL;
            else
                context.explosion_animation_counter++;
        }

        for (int spef_idx = 0; spef_idx < MAX_PLAYED_SPECIAL_EFFECTS; spef_idx++)
        {
            if (context.special_effect_animation_counter_on[spef_idx])
            {
                if (context.special_effect_animation_counter[spef_idx] >= 0xCFFFFFFFUL)
                    context.special_effect_animation_counter[spef_idx] = 0UL;
                else
                    (context.special_effect_animation_counter[spef_idx])++;
            }
        }

        for (int zap_anim_idx = 0; zap_anim_idx < MAX_PLAYED_ZAP_ANIMATIONS; zap_anim_idx++)
        {
            if (context.zap_animation_counter_on[zap_anim_idx])
            {
                if (context.zap_animation_counter[zap_anim_idx] >= 0xCFFFFFFFUL)
                    context.zap_animation_counter[zap_anim_idx] = 0UL;
                else
                    (context.zap_animation_counter[zap_anim_idx])++;
            }
        }

        for (int x = 1; x < COLNO; x++)
            for (int y = 0; y < ROWNO; y++) {
                if(data->mapAnimated[x][y])
                    dirty(data, x, y, FALSE);
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

    //D2D_Init();
    //D2D_InitMapRenderTargetOnCreate(hWnd, wParam, lParam);

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
    setDrawOrder(data);
}

static void
paintTile(PNHMapWindow data, int i, int j, RECT * rect)
{
    short ntile;
    int t_x, t_y;
    int glyph, signed_glyph;

    boolean flip_glyph = FALSE;
    data->mapAnimated[i][j] = FALSE;

    int current_cmap = get_current_cmap_type_index();
    boolean ispet = !!(data->map[i][j].layer_flags & LFLAGS_M_PET);
    boolean ispeaceful = !!(data->map[i][j].layer_flags & LFLAGS_M_PEACEFUL);
    boolean isyou = !!(data->map[i][j].layer_flags & LFLAGS_M_YOU);
    boolean issteed = !!(data->map[i][j].layer_flags & LFLAGS_M_RIDDEN);

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
    if (data->map[i][j].layer_glyphs[LAYER_FLOOR] == NO_GLYPH || data->map[i][j].layer_glyphs[LAYER_FLOOR] == cmap_to_glyph(S_unexplored))
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
    SetStretchBltMode(data->backBufferDC, COLORONCOLOR);
    SetStretchBltMode(hDCcopy, COLORONCOLOR);

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

            /* Set coordinates */
            if (enlarg_idx == -1)
            {
                enl_i = darkening_i = i;
                enl_j = darkening_j = j + tile_move_idx;
            }
            else if (enlarg_idx == 0)
            {
                enl_i = darkening_i = i - 1;
                enl_j = darkening_j = j + tile_move_idx;
            }
            else if (enlarg_idx == 1)
            {
                enl_i = darkening_i = i + 1;
                enl_j = darkening_j = j + tile_move_idx;
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

            if (enlarg_idx >= 0 && !isok(enl_i, enl_j))
                continue;

            int monster_layer_height = data->map[enl_i][enl_j].special_monster_layer_height;
            if (base_layer == LAYER_MONSTER || base_layer == LAYER_MONSTER_EFFECT)
            {
                if (monster_layer_height == 0 && draw_order[draw_index].tile_movement_index != 0)
                    continue;
                if (monster_layer_height < 0 && draw_order[draw_index].tile_movement_index == 1)
                    continue;
                if (monster_layer_height < 0 && draw_order[draw_index].tile_movement_index == -1 && enlarg_idx <= 1)
                    continue;
                if (monster_layer_height > 0 && draw_order[draw_index].tile_movement_index == -1)
                    continue;
            }

            boolean is_enl_you = !!(data->map[enl_i][enl_j].layer_flags & LFLAGS_M_YOU);
            genericptr_t m_stored = data->map[enl_i][enl_j].monster_comp_ptr;
            struct monst* m_here = m_at(enl_i, enl_j);
            struct monst* mtmp = is_enl_you ? &youmonst : (m_here == m_stored) ? m_here : (struct monst*)0;
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
                
            if (base_layer == LAYER_OBJECT || base_layer == LAYER_COVER_OBJECT)
            {
                struct obj* otmp = show_memory_objects ? level.locations[enl_i][enl_j].hero_memory_layers.memory_objchn : level.objects[enl_i][enl_j];

                /* If there is an object mimic, let it be first */
                if (has_obj_mimic && !show_memory_objects) /* The mimic has been added to memory objects */
                {
                    mimic_obj.nexthere = otmp;
                    otmp = &mimic_obj;
                }

                int objcnt = 0;
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


                if (base_layer == LAYER_OBJECT || base_layer == LAYER_COVER_OBJECT)
                {
                    otmp_round = obj_pile[MAX_SHOWN_OBJECTS - 1 - layer_round];

                    if (!otmp_round)
                        continue; /* next round */

                }

                boolean draw_in_front = (otmp_round && is_obj_drawn_in_front(otmp_round) /* && otmp_round->ox == u.ux && otmp_round->oy == u.uy */);
                if (base_layer == LAYER_COVER_OBJECT && !draw_in_front)
                    continue; /* next round */
                if (base_layer == LAYER_OBJECT && draw_in_front)
                    continue; /* next round */

                //int signed_bk_glyph = data->map[enl_i][enl_j].bkglyph;
                //int signed_main_glyph = data->map[enl_i][enl_j].glyph;

                if (base_layer == LAYER_OBJECT || base_layer == LAYER_COVER_OBJECT)
                    signed_glyph = otmp_round->glyph == NO_GLYPH || otmp_round->glyph == 0 ? NO_GLYPH : otmp_round->glyph;
                else
                    signed_glyph = data->map[enl_i][enl_j].layer_glyphs[base_layer];

                glyph = abs(signed_glyph);

                /* Kludge for the time being */
                if (base_layer == LAYER_FLOOR && glyph == NO_GLYPH)
                    glyph = cmap_to_glyph(S_unexplored);

                if (showing_detection || u.uswallow || (base_layer == LAYER_FLOOR && glyph == cmap_to_glyph(S_unexplored)))
                    skip_darkening = TRUE;

                boolean make_semi_transparent = FALSE;
                boolean make_invis_transparent = FALSE;
                boolean use_radial_transparency = FALSE;
                if (base_layer == LAYER_MONSTER)
                {
                    if (mtmp && (is_semi_transparent(mtmp->data)))
                    {
                        make_semi_transparent = TRUE;
                        if (is_radially_transparent(mtmp->data))
                            use_radial_transparency = TRUE;
                    }

                    if ((mtmp && ((!is_enl_you && is_invisible(mtmp) && canspotmon(mtmp)))) || (is_enl_you && Invis))
                        make_invis_transparent = TRUE;

                }

                /*
                 * Draw glyph
                 */
                if (glyph != NO_GLYPH)
                {
                    if (signed_glyph < 0)
                        flip_glyph = TRUE;
                    else
                        flip_glyph = FALSE;

                    int multiplier = flip_glyph ? -1 : 1;

                    /* Set position_index */
                    if (enlarg_idx == -1)
                    {
                        position_index = -1;
                    }
                    else if (enlarg_idx == 0)
                    {
                        position_index = flip_glyph ? 3 : 4;
                    }
                    else if (enlarg_idx == 1)
                    {
                        position_index = flip_glyph ? 4 : 3;
                    }
                    else if (enlarg_idx == 2)
                    {
                        position_index = flip_glyph ? 0 : 2;
                    }
                    else if (enlarg_idx == 3)
                    {
                        position_index = 1;
                    }
                    else if (enlarg_idx == 4)
                    {
                        position_index = flip_glyph ? 2 : 0;
                    }

                    int anim_frame_idx = -1, main_tile_idx = -1;
                    int tile_animation_idx = get_tile_animation_index_from_glyph(glyph);
                    boolean skip_drawing = FALSE;
                    boolean full_sized_item = !!(glyphtileflags[glyph] & GLYPH_TILE_FLAG_FULL_SIZED_ITEM) || glyph_is_monster(glyph); /* hallucinated statue */
                    boolean move_obj_to_middle = ((glyphtileflags[glyph] & GLYPH_TILE_FLAG_NORMAL_ITEM_AS_MISSILE) && !full_sized_item);
                    enum autodraw_types autodraw = AUTODRAW_NONE;
                    ntile = glyph2tile[glyph];
                    ntile = maybe_get_replaced_tile(ntile, i, j, data_to_replacement_info(signed_glyph, base_layer, otmp_round), &autodraw);
                    if(context.u_action_animation_counter_on && base_layer == LAYER_MONSTER && enl_i == u.ux && enl_j == u.uy)
                        ntile = maybe_get_animated_tile(ntile, tile_animation_idx, ANIMATION_PLAY_TYPE_PLAYED_SEPARATELY, context.u_action_animation_counter, &anim_frame_idx, &main_tile_idx, &data->mapAnimated[i][j], &autodraw);
                    else if (context.m_action_animation_counter_on && base_layer == LAYER_MONSTER && context.m_action_animation_x == enl_i && context.m_action_animation_y == enl_j)
                        ntile = maybe_get_animated_tile(ntile, tile_animation_idx, ANIMATION_PLAY_TYPE_PLAYED_SEPARATELY, context.m_action_animation_counter, &anim_frame_idx, &main_tile_idx, &data->mapAnimated[i][j], &autodraw);
                    else if (glyph_is_explosion(glyph))
                        ntile = maybe_get_animated_tile(ntile, tile_animation_idx, ANIMATION_PLAY_TYPE_PLAYED_SEPARATELY, context.explosion_animation_counter, &anim_frame_idx, &main_tile_idx, &data->mapAnimated[i][j], &autodraw);
                    else if (glyph_is_zap(glyph))
                    {
                        boolean zap_found = FALSE;
                        for (int zap_anim_idx = 0; zap_anim_idx < MAX_PLAYED_ZAP_ANIMATIONS; zap_anim_idx++)
                        {
                            if (context.zap_animation_counter_on[zap_anim_idx]
                                && enl_i == context.zap_animation_x[zap_anim_idx]
                                && enl_j == context.zap_animation_y[zap_anim_idx])
                            {
                                ntile = maybe_get_animated_tile(ntile, tile_animation_idx, ANIMATION_PLAY_TYPE_PLAYED_SEPARATELY, context.zap_animation_counter[zap_anim_idx], &anim_frame_idx, &main_tile_idx, &data->mapAnimated[i][j], &autodraw);
                                zap_found = TRUE;
                                break;
                            }
                        }
 
                        /* Otherwise, normal animation check */
                        if (!zap_found)
                            ntile = maybe_get_animated_tile(ntile, tile_animation_idx, ANIMATION_PLAY_TYPE_ALWAYS, data->interval_counter, &anim_frame_idx, &main_tile_idx, &data->mapAnimated[i][j], &autodraw);
                    }
                    else
                    {
                        /* Check for special effect animations */
                        boolean spef_found = FALSE;
                        for (int spef_idx = 0; spef_idx < MAX_PLAYED_SPECIAL_EFFECTS; spef_idx++)
                        {
                            if (context.special_effect_animation_counter_on[spef_idx]
                                && base_layer == context.spef_action_animation_layer[spef_idx]
                                && enl_i == context.spef_action_animation_x[spef_idx]
                                && enl_j == context.spef_action_animation_y[spef_idx])
                            {
                                ntile = maybe_get_animated_tile(ntile, tile_animation_idx, ANIMATION_PLAY_TYPE_PLAYED_SEPARATELY, context.special_effect_animation_counter[spef_idx], &anim_frame_idx, &main_tile_idx, &data->mapAnimated[i][j], &autodraw);
                                spef_found = TRUE;
                                break;
                            }
                        }
    
                        /* Otherwise, normal animation check */
                        if(!spef_found)
                            ntile = maybe_get_animated_tile(ntile, tile_animation_idx, ANIMATION_PLAY_TYPE_ALWAYS, data->interval_counter, &anim_frame_idx, &main_tile_idx, &data->mapAnimated[i][j], &autodraw);
                    }
                    if (enlarg_idx >= 0)
                    {
                        if (tile2enlargement[ntile] > 0)
                        {
                            int enl_tile_idx = enlargements[tile2enlargement[ntile]].position2tile[position_index];
                            if (enl_tile_idx >= 0)
                            {
                                autodraw = enlargements[tile2enlargement[ntile]].position_autodraw[position_index];
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
                                int enl_glyph = enl_tile_idx + addedindex + enlargements[tile2enlargement[ntile]].glyph_offset + GLYPH_ENLARGEMENT_OFF;
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
                        int base_t_x = TILEBMP_X(ntile);
                        int base_t_y = TILEBMP_Y(ntile);
                        t_x = base_t_x + (flip_glyph ? tileWidth - 1 : 0);
                        t_y = base_t_y;
                        if (layer_round > 0)
                            layer_round = layer_round;

                        double applicable_scaling_factor_x = print_first_directly_to_map ? x_scaling_factor : 1.0;
                        double applicable_scaling_factor_y = print_first_directly_to_map ? y_scaling_factor : 1.0;
                        double obj_scaling_factor = 1.0;
                        boolean is_obj_missile = FALSE;
                        boolean is_object = FALSE;

                        if (base_layer == LAYER_MISSILE && glyph_is_missile(glyph))
                        {
                            is_obj_missile = TRUE;
                            if (glyph_is_object_missile(glyph))
                            {
                                int otyp = (glyph - GLYPH_OBJ_MISSILE_OFF) / NUM_MISSILE_DIRS;
                                if (objects[otyp].oc_tile_floor_height > 0)
                                    obj_scaling_factor = ((double)objects[otyp].oc_tile_floor_height) / 48.0;
                            }
                            else if (glyph_is_artifact_missile(glyph))
                            {
                                int artidx = ((glyph - GLYPH_ARTIFACT_MISSILE_OFF) / NUM_MISSILE_DIRS) + 1;
                                if (artilist[artidx].tile_floor_height > 0)
                                    obj_scaling_factor = ((double)artilist[artidx].tile_floor_height) / 48.0;
                            }

                            /* This is a normal object used as a missile; now only lower part is used, object is moved upwards 1/4 of the tile */
                            if (move_obj_to_middle)
                            {
                                /* Take lower part only */
                                source_top_added = tileHeight / 2;
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
                                /* For all normal items, we use only lower part of the tile */
                                source_top_added =  (int)tileHeight / 2;
                                source_height_deducted = (int)tileHeight / 2;
                                dest_top_added = (int)(applicable_scaling_factor_y * ((double)GetNHApp()->mapTile_Y / 2.0));
                                dest_height_deducted = (int)(applicable_scaling_factor_y * ((double)GetNHApp()->mapTile_Y / 2.0));

                                /* Leave a little room for monster feet if not cover object */
                                if (base_layer == LAYER_OBJECT)
                                    dest_top_added += (int)(applicable_scaling_factor_y * (double)(objects_in_pit ? -PIT_BOTTOM_BORDER : -OBJECT_PILE_START_HEIGHT));

                                /* Pile the objects in order with two pixels in between */
                                if (layer_rounds > 1)
                                {
                                    dest_top_added += (int)(applicable_scaling_factor_y * ((double)-OBJECT_PILE_HEIGHT_DIFFERENCE * (double)(layer_rounds - 1 - layer_round)));
                                }

                                if (otmp_round && objects[otmp_round->otyp].oc_tile_floor_height > 0 && !showing_detection)
                                {
                                    obj_scaling_factor = ((double)objects[otmp_round->otyp].oc_tile_floor_height) / 48.0;
                                }

                                if (otmp_round && !showing_detection && objects_in_pit)
                                    obj_scaling_factor *= 0.75;
                            }
                        }
                        else if (base_layer == LAYER_MONSTER || base_layer == LAYER_MONSTER_EFFECT)
                        {
                            if (draw_order[draw_index].tile_movement_index == 0)
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
                            else if (draw_order[draw_index].tile_movement_index == -1)
                            {
                                if (monster_layer_height < 0)
                                {
                                    source_top_added = tileHeight - (abs(monster_layer_height) - PIT_BOTTOM_BORDER);
                                    source_height_deducted = tileHeight - (abs(monster_layer_height) - PIT_BOTTOM_BORDER);
                                    dest_top_added = 0;
                                    dest_height_deducted = (int)(applicable_scaling_factor_y * (double)(tileHeight - (abs(monster_layer_height) - PIT_BOTTOM_BORDER)));
                                }

                            }
                            else if (draw_order[draw_index].tile_movement_index == 1)
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

                        /* Scale object to be of oc_tile_floor_height height */
                        if ((is_obj_missile || is_object) && obj_scaling_factor != 1.0)
                        {
                            double scaled_height = (obj_scaling_factor * (double)GetNHApp()->mapTile_Y / 2.0);
                            double scaled_width = (obj_scaling_factor * (double)GetNHApp()->mapTile_X);

                            if(is_object)
                                dest_top_added += (int)(applicable_scaling_factor_y * ((double)GetNHApp()->mapTile_Y / 2.0 - scaled_height));
                            else
                                dest_top_added += (int)(applicable_scaling_factor_y * ((double)GetNHApp()->mapTile_Y / 2.0 - scaled_height) / 2.0);

                            dest_height_deducted += (int)(applicable_scaling_factor_y * ((double)GetNHApp()->mapTile_Y / 2.0 - scaled_height));
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
                                    data->backBufferDC, rect->left + (int)(x_scaling_factor * (double)(flip_glyph ? tileWidth - 1 : 0)), rect->top, multiplier * width, height, SRCCOPY);
                            }
                            else
                            {
                                StretchBlt(hDCMem, 0, 0, width, height,
                                    hDCcopy, (flip_glyph ? tileWidth - 1 : 0), 0, multiplier * tileWidth, tileHeight, SRCCOPY);
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
                                data->tileDC, base_t_x, base_t_y, tileWidth, tileHeight, SRCCOPY);

                            /* Draw semitransparency */
                            int pitch = 4 * width; // 4 bytes per pixel but if not 32 bit, round pitch up to multiple of 4
                            int idx, x, y;
                            double semi_transparency = make_semi_transparent && make_invis_transparent ? 0.5 * 0.85 : 0.5;

                            if (use_radial_transparency)
                            {
                                double mid_x = (double)width / 2.0 - 0.5;
                                double mid_y = (double)height / 2.0 - 0.5;
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
                                        data->xBackTile - dest_width_deducted, data->yBackTile - dest_height_deducted, hDCsemitransparent, (flip_glyph ? tileWidth - 1 : 0),
                                        source_top_added, multiplier * GetNHApp()->mapTile_X,
                                        GetNHApp()->mapTile_Y - source_height_deducted, TILE_BK_COLOR);
                                }
                                else
                                {
                                    (*GetNHApp()->lpfnTransparentBlt)(
                                        hDCcopy, dest_left_added, dest_top_added,
                                        GetNHApp()->mapTile_X - dest_width_deducted, GetNHApp()->mapTile_Y - dest_height_deducted, hDCsemitransparent,
                                        (flip_glyph ? tileWidth - 1 : 0), source_top_added, multiplier * GetNHApp()->mapTile_X,
                                        GetNHApp()->mapTile_Y - source_height_deducted, TILE_BK_COLOR);
                                }
                            }
                            else
                            {
                                if (print_first_directly_to_map)
                                {
                                    StretchBlt(
                                        data->backBufferDC, rect->left + dest_left_added, rect->top + dest_top_added,
                                        data->xBackTile - dest_width_deducted, data->yBackTile - dest_height_deducted, hDCsemitransparent, (flip_glyph ? width - 1 : 0),
                                        source_top_added, multiplier * width,
                                        height - source_height_deducted, SRCCOPY);
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
                                        (flip_glyph ? tileWidth - 1 : 0), source_top_added, multiplier * width,
                                        height - source_height_deducted, SRCCOPY);
                                }
                                opaque_background_drawn = TRUE;
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
                                        data->xBackTile - dest_width_deducted, data->yBackTile - dest_height_deducted, data->tileDC, t_x,
                                        t_y + source_top_added, multiplier * GetNHApp()->mapTile_X,
                                        GetNHApp()->mapTile_Y - source_height_deducted, TILE_BK_COLOR);
                                }
                                else
                                {
                                    (*GetNHApp()->lpfnTransparentBlt)(
                                        hDCcopy, dest_left_added, dest_top_added,
                                        GetNHApp()->mapTile_X - dest_width_deducted, GetNHApp()->mapTile_Y - dest_height_deducted, data->tileDC,
                                        t_x, t_y + source_top_added, multiplier * GetNHApp()->mapTile_X,
                                        GetNHApp()->mapTile_Y - source_height_deducted, TILE_BK_COLOR);
                                }
                            }
                            else
                            {
                                if (print_first_directly_to_map)
                                {
                                    StretchBlt(
                                        data->backBufferDC, rect->left + dest_left_added, rect->top + dest_top_added,
                                        data->xBackTile - dest_width_deducted, data->yBackTile - dest_height_deducted, data->tileDC, t_x,
                                        t_y + source_top_added, multiplier * GetNHApp()->mapTile_X,
                                        GetNHApp()->mapTile_Y - source_height_deducted, SRCCOPY);
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
                                        GetNHApp()->mapTile_X - dest_width_deducted, GetNHApp()->mapTile_Y - dest_height_deducted, data->tileDC,
                                        t_x, t_y + source_top_added, multiplier * GetNHApp()->mapTile_X,
                                        GetNHApp()->mapTile_Y - source_height_deducted, SRCCOPY);
                                }
                                opaque_background_drawn = TRUE;
                            }
                        }
                        /* 
                         * AUTODRAW START
                         */
                        if (autodraw > AUTODRAW_NONE)
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

#define NO_WALL_END_AUTODRAW(x,y) (!isok(x, y) || levl[x][y].hero_memory_layers.glyph == cmap_to_glyph(S_unexplored) || (IS_DOORJOIN(levl[x][y].typ) && !IS_TREE(levl[x][y].typ)) || levl[x][y].typ == DOOR || levl[x][y].typ == UNEXPLORED /*|| (levl[x][y].seenv & (SV4 | SV5 | SV6)) == 0 */)

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
                                                        target_rt.right - target_rt.left, target_rt.bottom - target_rt.top, data->tileDC, source_rt.left,
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
                                                        target_rt.right - target_rt.left, target_rt.bottom - target_rt.top, data->tileDC, source_rt.left,
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
                                int y_to_first_shelf = 43;
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
                                                target_rt.right - target_rt.left, target_rt.bottom - target_rt.top, data->tileDC, source_rt.left,
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
                                                target_rt.right - target_rt.left, target_rt.bottom - target_rt.top, data->tileDC, source_rt.left,
                                                source_rt.top, source_rt.right - source_rt.left,
                                                source_rt.bottom - source_rt.top, TILE_BK_COLOR);
                                        }
                                        cnt++;
                                    }
                                }
                            }
                            else if (autodraws[autodraw].draw_type == AUTODRAW_DRAW_WEAPON_RACK_CONTENTS && otmp_round)
                            {
                                int y_to_rack_top = 28;
                                int rack_start = 0; /* Assume weapons are drawn reasonably well in the center */
                                int rack_width = 48;
                                int rack_height = TILE_Y - y_to_rack_top;
                                int rack_item_spacing = 6;

                                int cnt = 0;

                                for (struct obj* contained_obj = otmp_round->cobj; contained_obj; contained_obj = contained_obj->nobj)
                                {
                                    int src_x = 0, src_y = (objects[contained_obj->otyp].oc_flags4 & O4_FULL_SIZED_BITMAP ? 0 : TILE_Y / 2);
                                    int dest_x = 0, dest_y = 0;
                                    int item_width = objects[contained_obj->otyp].oc_tile_floor_height ? objects[contained_obj->otyp].oc_tile_floor_height : TILE_Y / 2;
                                    int item_height = (item_width * TILE_Y) / TILE_X;
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
                                        data->tileDC, source_rt.left, source_rt.top, width, height, SRCCOPY);

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
                                            target_rt.right - target_rt.left, target_rt.bottom - target_rt.top, data->tileDC, source_rt.left,
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
                                            target_rt.right - target_rt.left, target_rt.bottom - target_rt.top, data->tileDC, source_rt.left,
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
                                    HDC hDCjar = CreateCompatibleDC(data->tileDC);

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
                                    int at_x = TILEBMP_X(atile);
                                    int at_y = TILEBMP_Y(atile);

                                    int source_glyph2 = autodraws[autodraw].source_glyph2;
                                    int atile2 = glyph2tile[source_glyph2];
                                    int a2t_x = TILEBMP_X(atile2);
                                    int a2t_y = TILEBMP_Y(atile2);

                                    RECT source_rt = { 0 };
                                    RECT target_rt = { 0 };

                                    /* First, background */
                                    if (1)
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
                                            target_rt.right - target_rt.left, target_rt.bottom - target_rt.top, data->tileDC, source_rt.left,
                                            source_rt.top, source_rt.right - source_rt.left,
                                            source_rt.bottom - source_rt.top, TILE_BK_COLOR);
                                    }
                                

                                    /* Second, contents */
                                    if (1)
                                    {
                                        /* Create copy of the contents tile */
                                        source_rt.left = at_x;
                                        source_rt.right = source_rt.left + tileWidth;
                                        source_rt.top = at_y + tileHeight / 2;
                                        source_rt.bottom = source_rt.top + tileHeight / 2;
                                        int width = source_rt.right - source_rt.left;
                                        int height = source_rt.bottom - source_rt.top;

                                        HDC hDCtemplate = CreateCompatibleDC(data->tileDC);

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
                                            data->tileDC, source_rt.left, source_rt.top, width, height, SRCCOPY);

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
                                    if (1)
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
                                            hDCjar, (flip_glyph ? jar_width - 1 : 0), 0, multiplier* jar_width, jar_height, SRCCOPY);

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
                                            data->tileDC, source_rt.left, source_rt.top, source_rt.right - source_rt.left, source_rt.bottom - source_rt.top, SRCCOPY);

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
                                            (flip_glyph ? tileWidth - 1 : 0), 0, multiplier * width,
                                            height, TILE_BK_COLOR);

                                        SelectObject(hDCsemitransparent, oldbmp_st);
                                        DeleteDC(hDCsemitransparent);
                                        DeleteObject(newhBmp_st);

                                        SelectObject(hDCMem, oldbmp);
                                        DeleteDC(hDCMem);
                                        DeleteObject(bitmap);
                                    }


                                    /* Fourth, opaque foreground */
                                    if (1)
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
                                            target_rt.right - target_rt.left, target_rt.bottom - target_rt.top, data->tileDC, source_rt.left,
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
                        }
                        /*
                         * AUTODRAW END
                         */

                        /* Item property marks */
                        if (((base_layer == LAYER_OBJECT || base_layer == LAYER_COVER_OBJECT) && otmp_round &&
                            (otmp_round->opoisoned || otmp_round->elemental_enchantment > 0 || otmp_round->oeroded || otmp_round->oeroded2 || otmp_round->exceptionality > 0))
                            ||
                            ((base_layer == LAYER_MISSILE) &&
                                (data->map[enl_i][enl_j].missile_poisoned || data->map[enl_i][enl_j].missile_elemental_enchantment > 0 || data->map[enl_i][enl_j].missile_eroded || data->map[enl_i][enl_j].missile_eroded2 || data->map[enl_i][enl_j].missile_exceptionality > 0))
                            )
                        {
                            int y_start = (base_layer == LAYER_MISSILE && !move_obj_to_middle ? tileHeight / 4 : dest_top_added);
                            int x_start = dest_left_added;
                            int mark_width = 8;
                            int marks_per_row = TILE_X / mark_width;
                            int mark_height = 24;
                            int src_x = 0;
                            int src_y = 0;
                            int cnt = 0;
                            int poisoned = (base_layer == LAYER_MISSILE ? data->map[enl_i][enl_j].missile_poisoned : otmp_round->opoisoned);
                            int elemental_enchantment = (base_layer == LAYER_MISSILE ? data->map[enl_i][enl_j].missile_elemental_enchantment : otmp_round->elemental_enchantment);
                            int exceptionality = (base_layer == LAYER_MISSILE ? data->map[enl_i][enl_j].missile_exceptionality : otmp_round->exceptionality);
                            int eroded = (base_layer == LAYER_MISSILE ? data->map[enl_i][enl_j].missile_eroded : otmp_round->oeroded);
                            int eroded2 = (base_layer == LAYER_MISSILE ? data->map[enl_i][enl_j].missile_eroded2 : otmp_round->oeroded2);
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
                                        target_rt.right - target_rt.left, target_rt.bottom - target_rt.top, data->tileDC, source_rt.left,
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
                                        target_rt.right - target_rt.left, target_rt.bottom - target_rt.top, data->tileDC, source_rt.left,
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
                        if (!cansee(darkening_i, darkening_j) || (data->map[darkening_i][darkening_j].layer_flags & LFLAGS_SHOWING_MEMORY))
                        {
                            if (1)
                            {
                                double multiplier = 1.0;
                                if (isyou)
                                {
                                    if(default_tileset_definition.you_darkening[current_cmap] > 0.0)
                                        multiplier *= default_tileset_definition.you_darkening[current_cmap];
                                    else
                                        multiplier *= 0.85;
                                }
                                else
                                {
                                    if (!levl[enl_i][enl_j].waslit)
                                    {
                                        if (default_tileset_definition.nonlit_darkening[current_cmap] > 0.0)
                                            multiplier *= default_tileset_definition.nonlit_darkening[current_cmap];
                                        else
                                            multiplier *= 0.35;
                                    }
                                    else
                                    {
                                        if (default_tileset_definition.lit_darkening[current_cmap] > 0.0)
                                            multiplier *= default_tileset_definition.lit_darkening[current_cmap];
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

                /* Draw main tile marker  */
                if (glyph != NO_GLYPH && base_layer == LAYER_MONSTER && enlarg_idx == -1 && tile_move_idx == 0 && !is_worm_tail)
                {
                    /* Draw main tile marker for enlarged creatures */
                    int enlargement_idx = tile2enlargement[ntile];
                    if (flags.show_tile_big_monster_target && enlargement_idx > 0 && enlargements[enlargement_idx].number_of_enlargement_tiles >= 1)
                    {
                        int mglyph = MAIN_TILE_MARK + GLYPH_UI_TILE_OFF;
                        int mtile = glyph2tile[mglyph];
                        t_x = TILEBMP_X(mtile);
                        t_y = TILEBMP_Y(mtile);

                        if (print_first_directly_to_map)
                        {
                            (*GetNHApp()->lpfnTransparentBlt)(
                                data->backBufferDC, rect->left, rect->top,
                                data->xBackTile, data->yBackTile, data->tileDC, t_x,
                                t_y, GetNHApp()->mapTile_X,
                                GetNHApp()->mapTile_Y, TILE_BK_COLOR);
                        }
                        else
                        {
                            (*GetNHApp()->lpfnTransparentBlt)(
                                hDCcopy, 0, 0,
                                GetNHApp()->mapTile_X, GetNHApp()->mapTile_Y, data->tileDC, t_x,
                                t_y, GetNHApp()->mapTile_X,
                                GetNHApp()->mapTile_Y, TILE_BK_COLOR);
                        }
                    }


                }
                
                int monster_glyph = data->map[enl_i][enl_j].layer_glyphs[LAYER_MONSTER];

                /* All UI related symbols and cursors */
                if (base_layer == LAYER_GENERAL_UI && enlarg_idx == -1)
                {
                    /* Add cursor first, as it can be the largest, and we do not want it to cover other UI elements */
                    if (i == data->xCur && j == data->yCur)
                    {
                        boolean cannotseeself = (i == u.ux && j == u.uy) && !canspotself();
                        if (!cannotseeself &&
                            ((!data->cursorOn && flags.blinking_cursor_on_tiles)
                            || (/*i == u.ux && j == u.uy &&*/ !flags.show_cursor_on_u)
                            ))
                        {
                            // Nothing, cursor is invisible
                        }
                        else
                        {
                            int anim_frame_idx = -1, main_tile_idx  = -1;
                            int cglyph = (cannotseeself && flags.active_cursor_style == CURSOR_STYLE_GENERIC_CURSOR ? CURSOR_STYLE_INVISIBLE : flags.active_cursor_style) + GLYPH_CURSOR_OFF;
                            int ctile = glyph2tile[cglyph];
                            int tile_animation_idx = get_tile_animation_index_from_glyph(cglyph);
                            ctile = maybe_get_replaced_tile(ctile, i, j, zeroreplacementinfo, (enum autodraw_types*)0);
                            ctile = maybe_get_animated_tile(ctile, tile_animation_idx, ANIMATION_PLAY_TYPE_ALWAYS, data->interval_counter, &anim_frame_idx, &main_tile_idx, &data->mapAnimated[i][j], (enum autodraw_types*)0);
                            t_x = TILEBMP_X(ctile);
                            t_y = TILEBMP_Y(ctile);

                            (*GetNHApp()->lpfnTransparentBlt)(
                                data->backBufferDC, rect->left, rect->top,
                                data->xBackTile, data->yBackTile, data->tileDC, t_x,
                                t_y, GetNHApp()->mapTile_X,
                                GetNHApp()->mapTile_Y, TILE_BK_COLOR);

                        }
                    }

                    /* Conditions, status marks, and buffs */
                    int condition_count = 0;
                    if (glyph_is_monster(monster_glyph) && mtmp && !is_worm_tail)
                    {
                        if (1)
                        {
                            /* Petmark and other status marks */
                            int mglyph = STATUS_MARKS + GLYPH_UI_TILE_OFF;
                            int mtile = glyph2tile[mglyph];
                            int ct_x = TILEBMP_X(mtile);
                            int ct_y = TILEBMP_Y(mtile);
                            int tiles_per_row = tileWidth / ui_tile_component_array[STATUS_MARKS].width;
                            int max_fitted_rows = (tileHeight - 4) / (ui_tile_component_array[STATUS_MARKS].height + 2);

                            for (enum game_ui_status_mark_types status_mark = STATUS_MARK_PET; status_mark < min(MAX_STATUS_MARKS, ui_tile_component_array[STATUS_MARKS].number); status_mark++)
                            {
                                boolean display_this_status_mark = FALSE;

                                switch (status_mark)
                                {
                                case STATUS_MARK_PET:
                                    if (!isyou && ispet)
                                        display_this_status_mark = TRUE;
                                    break;
                                case STATUS_MARK_PEACEFUL:
                                    if (!isyou && ispeaceful)
                                        display_this_status_mark = TRUE;
                                    break;
                                case STATUS_MARK_DETECTED:
                                    if (!isyou && data->map[i][j].layer_flags & LFLAGS_M_DETECTED)
                                        display_this_status_mark = TRUE;
                                    break;
                                case STATUS_MARK_PILE:
                                    //if (!isyou && data->map[i][j].layer_flags & LFLAGS_O_PILE)
                                    //    display_this_status_mark = TRUE;
                                    break;
                                case STATUS_MARK_HUNGRY:
                                    if ((isyou && u.uhs == HUNGRY)
                                        || (!isyou && ispet && mtmp->mextra && EDOG(mtmp) && monstermoves >= EDOG(mtmp)->hungrytime && EDOG(mtmp)->mhpmax_penalty == 0)
                                        )
                                        display_this_status_mark = TRUE;
                                    break;
                                case STATUS_MARK_WEAK:
                                    if (isyou && u.uhs == WEAK
                                        || (!isyou && ispet && mtmp->mextra && EDOG(mtmp) && monstermoves >= EDOG(mtmp)->hungrytime && EDOG(mtmp)->mhpmax_penalty > 0)
                                        )
                                        display_this_status_mark = TRUE;
                                    break;
                                case STATUS_MARK_FAINTING:
                                    if (isyou && u.uhs >= FAINTING)
                                        display_this_status_mark = TRUE;
                                    break;
                                case STATUS_MARK_BURDENED:
                                    if (isyou && u.carrying_capacity_level == SLT_ENCUMBER)
                                        display_this_status_mark = TRUE;
                                    break;
                                case STATUS_MARK_STRESSED:
                                    if (isyou && u.carrying_capacity_level == MOD_ENCUMBER)
                                        display_this_status_mark = TRUE;
                                    break;
                                case STATUS_MARK_STRAINED:
                                    if (isyou && u.carrying_capacity_level == HVY_ENCUMBER)
                                        display_this_status_mark = TRUE;
                                    break;
                                case STATUS_MARK_OVERTAXED:
                                    if (isyou && u.carrying_capacity_level == EXT_ENCUMBER)
                                        display_this_status_mark = TRUE;
                                    break;
                                case STATUS_MARK_OVERLOADED:
                                    if (isyou && u.carrying_capacity_level == OVERLOADED)
                                        display_this_status_mark = TRUE;
                                    break;
                                case STATUS_MARK_2WEP:
                                    if (isyou && u.twoweap)
                                        display_this_status_mark = TRUE;
                                    break;
                                case STATUS_MARK_SKILL:
                                    if (isyou && u.canadvanceskill)
                                        display_this_status_mark = TRUE;
                                    break;
                                case STATUS_MARK_SADDLED:
                                    if (!isyou && (data->map[i][j].layer_flags & LFLAGS_M_SADDLED))
                                        display_this_status_mark = TRUE;
                                    break;
                                case STATUS_MARK_LOW_HP:
                                case STATUS_MARK_CRITICAL_HP:
                                {
                                    if ((isyou && !flags.show_tile_u_hp_bar) || (ispet && !flags.show_tile_pet_hp_bar))
                                    {
                                        int relevant_hp_max = isyou ? (Upolyd ? u.mhmax : u.uhpmax) : mtmp->mhpmax;
                                        int low_threshold = min(relevant_hp_max / 2, max(4, relevant_hp_max / 3));
                                        if (relevant_hp_max < 4)
                                            low_threshold = 0;
                                        int critical_threshold = max(1, min(relevant_hp_max / 4, max(4, relevant_hp_max / 6)));
                                        if (relevant_hp_max < 2)
                                            critical_threshold = 0;

                                        int relevant_hp = isyou ? (Upolyd ? u.mh : u.uhp) : mtmp->mhp;
                                        if (status_mark == STATUS_MARK_CRITICAL_HP && relevant_hp <= critical_threshold)
                                            display_this_status_mark = TRUE;
                                        if (status_mark == STATUS_MARK_LOW_HP && relevant_hp <= low_threshold && relevant_hp > critical_threshold)
                                            display_this_status_mark = TRUE;
                                    }
                                    break;
                                }
                                case STATUS_MARK_SPEC_USED:
                                    if (!isyou && ispet && any_spec_used(mtmp))
                                        display_this_status_mark = TRUE;
                                    break;
                                case STATUS_MARK_TRAPPED:
                                    if ((isyou && u.utrap) || (!isyou && mtmp->mtrapped))
                                        display_this_status_mark = TRUE;
                                    break;
                                case STATUS_MARK_USTUCK:
                                    if (mtmp == u.ustuck && !u.uswallow)
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
                                        target_rt.right - target_rt.left, target_rt.bottom - target_rt.top, data->tileDC, source_rt.left,
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
                                if (!ispet && !isyou)
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
                                        unscaled_right - unscaled_left, unscaled_bottom - unscaled_top, data->tileDC, source_rt.left,
                                        source_rt.top, source_rt.right - source_rt.left,
                                        source_rt.bottom - source_rt.top, TILE_BK_COLOR);
                                    */

                                    (*GetNHApp()->lpfnTransparentBlt)(
                                        data->backBufferDC, target_rt.left, target_rt.top,
                                        target_rt.right - target_rt.left, target_rt.bottom - target_rt.top, data->tileDC, source_rt.left,
                                        source_rt.top, source_rt.right - source_rt.left,
                                        source_rt.bottom - source_rt.top, TILE_BK_COLOR);

                                    condition_count++;
                                }
                            }

                            /* Buffs */
                            for (int propidx = 1; propidx <= LAST_PROP; propidx++)
                            {
                                if (!context.properties[propidx].show_buff)
                                    continue;

                                if (isyou ? (u.uprops[propidx].intrinsic & TIMEOUT) == 0 : (mtmp->mprops[propidx] & M_TIMEOUT) == 0)
                                    continue;

                                mglyph = (propidx - 1) / BUFFS_PER_TILE + GLYPH_BUFF_OFF;
                                mtile = glyph2tile[mglyph];
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
                                    target_rt.right - target_rt.left, target_rt.bottom - target_rt.top, data->tileDC, source_rt.left,
                                    source_rt.top, source_rt.right - source_rt.left,
                                    source_rt.bottom - source_rt.top, TILE_BK_COLOR);

                                condition_count++;
                            }

                            /* Steed mark (you as small) */
                            if (isyou && issteed)
                            {
                                int anim_frame_idx = -1, main_tile_idx = -1;
                                int signed_mglyph = u_to_glyph();
                                boolean flip_rider = (signed_mglyph < 0);
                                mglyph = abs(signed_mglyph);
                                mtile = glyph2tile[mglyph];
                                int tile_animation_idx = get_tile_animation_index_from_glyph(mglyph);
                                mtile = maybe_get_replaced_tile(mtile, i, j, data_to_replacement_info(signed_mglyph, base_layer, otmp_round), (enum auto_drawtypes*)0);
                                mtile = maybe_get_animated_tile(mtile, tile_animation_idx, ANIMATION_PLAY_TYPE_ALWAYS, data->interval_counter, &anim_frame_idx, &main_tile_idx, &data->mapAnimated[i][j], (enum auto_drawtypes*)0);
                                int c_x = TILEBMP_X(mtile);
                                int c_y = TILEBMP_Y(mtile);
                                /* Define draw location in target */
                                int unscaled_left = 3;
                                int unscaled_right = unscaled_left + tileWidth / 2;
                                int unscaled_top = 3;
                                int unscaled_bottom = unscaled_top + tileHeight / 2;


                                /* Frame and background first */
                                HBRUSH hbr_frame = CreateSolidBrush(RGB(100, 50, 0));
                                HBRUSH hbr_background = CreateSolidBrush(RGB(200, 200, 200));

                                RECT frame_rect = { 0 };
                                frame_rect.left = rect->left + (int)(x_scaling_factor * (double)(unscaled_left - 1));
                                frame_rect.right = rect->left + (int)(x_scaling_factor * (double)(unscaled_right + 1));
                                frame_rect.top = rect->top + (int)(y_scaling_factor * (double)(unscaled_top - 1));
                                frame_rect.bottom = rect->top + (int)(y_scaling_factor * (double)(unscaled_bottom + 1));

                                FillRect(data->backBufferDC, &frame_rect, hbr_background);
                                FrameRect(data->backBufferDC, &frame_rect, hbr_frame);

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

                                int multiplier = flip_rider ? -1 : 1;

                                /*
                                (*GetNHApp()->lpfnTransparentBlt)(
                                    hDCcopy, unscaled_left, unscaled_top,
                                    unscaled_right - unscaled_left, unscaled_bottom - unscaled_top, data->tileDC, source_rt.left + (flip_rider ? tileWidth - 1 : 0),
                                    source_rt.top, multiplier * (source_rt.right - source_rt.left),
                                    source_rt.bottom - source_rt.top, TILE_BK_COLOR);
                                */

                                (*GetNHApp()->lpfnTransparentBlt)(
                                    data->backBufferDC, target_rt.left, target_rt.top,
                                    target_rt.right - target_rt.left, target_rt.bottom - target_rt.top, data->tileDC, source_rt.left + (flip_rider ? tileWidth - 1 : 0),
                                    source_rt.top, multiplier * (source_rt.right - source_rt.left),
                                    source_rt.bottom - source_rt.top, TILE_BK_COLOR);
                            }
                        }
                    }


                    /* Draw hit point bars */
                    if (mtmp && !is_worm_tail && (
                        (ispet && flags.show_tile_pet_hp_bar)
                        || (isyou && flags.show_tile_u_hp_bar)
                        || (!ispet && !isyou && flags.show_tile_mon_hp_bar && canspotmon(mtmp))
                        ))
                    {
                        int hp = isyou ? (Upolyd ? u.mh : u.uhp) : mtmp->mhp;
                        int hpmax = isyou ? (Upolyd ? u.mhmax : u.uhpmax) : mtmp->mhpmax;
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
                    }


                    /* Draw death and hit markers */
                    if (glyph_is_dying_monster(monster_glyph) || glyph_is_female_dying_monster(monster_glyph) || glyph_is_dying_player(monster_glyph))
                    {
                        int mglyph = DEATH_TILE + GLYPH_UI_TILE_OFF;
                        int mtile = glyph2tile[mglyph];
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
                            data->xBackTile, data->yBackTile, data->tileDC, t_x,
                            t_y, GetNHApp()->mapTile_X,
                            GetNHApp()->mapTile_Y, TILE_BK_COLOR);
                    }
                    else if (glyph_is_damage_receiving_monster(monster_glyph) || glyph_is_female_damage_receiving_monster(monster_glyph) || glyph_is_damage_receiving_player(monster_glyph) || data->map[i][j].layer_flags & LFLAGS_M_BEING_HIT)
                    {
                        unsigned long hit_text_bits = (data->map[i][j].layer_flags & LFLAGS_M_HIT_TILE_MASK);
                        unsigned long hit_text_num = hit_text_bits >> LFLAGS_M_HIT_TILE_MASK_BIT_OFFSET;
                        int mglyph = max(0, min(MAX_HIT_TILES - 1, (int)hit_text_num)) + GLYPH_HIT_TILE_OFF;
                        int mtile = glyph2tile[mglyph];
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
                            data->xBackTile, data->yBackTile, data->tileDC, t_x,
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

}


static void
paintGlyph(PNHMapWindow data, int i, int j, RECT * rect)
{
    if (1) // data->map[i][j].glyph >= 0)
    {

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
    draw_count++;

    int same_level_z_order_array[3] = { 0, 1, -1 };
    int different_level_z_order_array[3] = { 2, 4, 3 };

    /* Second, draw other layers on the same y */
    for (enum layer_types layer_idx = LAYER_FLOOR + 1; layer_idx < MAX_LAYERS; layer_idx++)
    {
        for (int enl_idx = 0; enl_idx <= 2; enl_idx++)
        {
            data->draw_order[draw_count].enlargement_index = same_level_z_order_array[enl_idx];
            data->draw_order[draw_count].layer = layer_idx;
            data->draw_order[draw_count].tile_movement_index = 0;
            draw_count++;

            if (layer_idx == LAYER_MONSTER || layer_idx == LAYER_MONSTER_EFFECT)
            {
                /* These are in fact not drawn */
                data->draw_order[draw_count].enlargement_index = same_level_z_order_array[enl_idx];
                data->draw_order[draw_count].layer = layer_idx;
                data->draw_order[draw_count].tile_movement_index = -1;
                draw_count++;

                /* These are drawn at the same time as lower positioned tiles */
                data->draw_order[draw_count].enlargement_index = different_level_z_order_array[enl_idx];
                data->draw_order[draw_count].layer = layer_idx;
                data->draw_order[draw_count].tile_movement_index = -1;
                draw_count++;
            }
        }
    }
    /* Mark to be drawn to back buffer and darkened if needed */
    /* Note these all use the darkness of the target tile, so they will be shaded similarly */
    /* Monster tile mark will be potentially darkened, other UI symbols come on the top undarkened */
    data->draw_order[draw_count - 1].draw_to_buffer = 1;

    /* Third, the three positions at y + 1, in reverse enl_pos / layer_idx order */
    for (enum layer_types layer_idx = LAYER_FLOOR + 1; layer_idx < MAX_LAYERS; layer_idx++)
    {
        for (int enl_idx = 0; enl_idx <= 2; enl_idx++)
        {
            data->draw_order[draw_count].enlargement_index = different_level_z_order_array[enl_idx];
            data->draw_order[draw_count].layer = layer_idx;
            data->draw_order[draw_count].tile_movement_index = 0;
            draw_count++;

            if (layer_idx == LAYER_MONSTER || layer_idx == LAYER_MONSTER_EFFECT)
            {
                /* These two are drawn at the same time as the higher positioned tiles */
                data->draw_order[draw_count].enlargement_index = same_level_z_order_array[enl_idx];
                data->draw_order[draw_count].layer = layer_idx;
                data->draw_order[draw_count].tile_movement_index = 1;
                draw_count++;

                data->draw_order[draw_count].enlargement_index = different_level_z_order_array[enl_idx];
                data->draw_order[draw_count].layer = layer_idx;
                data->draw_order[draw_count].tile_movement_index = 1;
                draw_count++;
            }
        }
    }
    /* Mark to be drawn to back buffer and darkened if needed */
    /* Note these all use the darkness of the tile below the target, so they will be shaded similarly */
    data->draw_order[draw_count - 1].draw_to_buffer = 1;

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
    data->mapDirty[x][y] = TRUE;

    RECT rt;
    nhcoord2display(data, x, y, &rt); //data->xCur, data->yCur

    InvalidateRect(data->hWnd, &rt, FALSE);

    /* Check REPLACEMENT_EVENT_UPDATE_FROM_BELOW */
    for (int dir = 0; dir < 4; dir++)
    {
        int rx = 0;
        int ry = 0;
        int dir_bit = 0;
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
                short tile_upside = glyph2tile[abs(data->map[rx][ry].glyph)];
                short bktile_upside = glyph2tile[abs(data->map[rx][ry].bkglyph)];
                short replacement_idx_upside = tile2replacement[tile_upside];
                short bk_replacement_idx_upside = tile2replacement[bktile_upside];

                short used_ridx = (i == 0 ? replacement_idx_upside : bk_replacement_idx_upside);
                short used_tile = (i == 0 ? tile_upside : bktile_upside);
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

    for (enum layer_types layer_idx = LAYER_FLOOR/*-2*/; layer_idx < MAX_LAYERS; layer_idx++)
    {
        int layer_rounds = 1;
        struct obj* otmp = (struct obj*)0;
        short ntile = -1;
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
                        enlarg = tile2enlargement[glyph2tile[abs(otmp->glyph)]]; // obj_to_glyph(otmp, rn2_on_display_rng))]];
                }
            }
            else
            {
                int anim_frame_idx = -1, main_tile_idx = -1;
                int signed_glyph = data->map[x][y].layer_glyphs[layer_idx];
                int glyph = abs(signed_glyph);
                flipped = (signed_glyph < 0);
                int ntile = glyph2tile[glyph];
                boolean mapanimateddummy = 0;
                enum autodraw_types autodraw = AUTODRAW_NONE;
                int tile_animation_idx = get_tile_animation_index_from_glyph(glyph);
                if (context.u_action_animation_counter_on && layer_idx == LAYER_MONSTER && x == u.ux && y == u.uy)
                    ntile = maybe_get_animated_tile(ntile, tile_animation_idx, ANIMATION_PLAY_TYPE_PLAYED_SEPARATELY, context.u_action_animation_counter, &anim_frame_idx, &main_tile_idx, &mapanimateddummy, &autodraw);
                else if (context.m_action_animation_counter_on && layer_idx == LAYER_MONSTER && context.m_action_animation_x == x && context.m_action_animation_y == y)
                    ntile = maybe_get_animated_tile(ntile, tile_animation_idx, ANIMATION_PLAY_TYPE_PLAYED_SEPARATELY, context.m_action_animation_counter, &anim_frame_idx, &main_tile_idx, &mapanimateddummy, &autodraw);
                else if (glyph_is_explosion(glyph))
                    ntile = maybe_get_animated_tile(ntile, tile_animation_idx, ANIMATION_PLAY_TYPE_PLAYED_SEPARATELY, context.explosion_animation_counter, &anim_frame_idx, &main_tile_idx, &mapanimateddummy, &autodraw);
                else if (glyph_is_zap(glyph))
                {
                    for (int zap_anim_idx = 0; zap_anim_idx < MAX_PLAYED_ZAP_ANIMATIONS; zap_anim_idx++)
                    {
                        if (context.zap_animation_counter_on[zap_anim_idx]
                            && x == context.zap_animation_x[zap_anim_idx]
                            && y == context.zap_animation_y[zap_anim_idx])
                        {
                            ntile = maybe_get_animated_tile(ntile, tile_animation_idx, ANIMATION_PLAY_TYPE_PLAYED_SEPARATELY, context.zap_animation_counter[zap_anim_idx], &anim_frame_idx, &main_tile_idx, &mapanimateddummy, &autodraw);
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
                        if (context.special_effect_animation_counter_on[spef_idx]
                            && layer_idx == context.spef_action_animation_layer[spef_idx]
                            && x == context.spef_action_animation_x[spef_idx]
                            && y == context.spef_action_animation_y[spef_idx])
                        {
                            ntile = maybe_get_animated_tile(ntile, tile_animation_idx, ANIMATION_PLAY_TYPE_PLAYED_SEPARATELY, context.special_effect_animation_counter[spef_idx], &anim_frame_idx, &main_tile_idx, &mapanimateddummy, &autodraw);
                            spef_found = TRUE;
                            break;
                        }
                    }

                    /* Otherwise, normal animation check */
                    if (!spef_found)
                        ntile = maybe_get_animated_tile(ntile, tile_animation_idx, ANIMATION_PLAY_TYPE_ALWAYS, data->interval_counter, &anim_frame_idx, &main_tile_idx, &mapanimateddummy, &autodraw);
                }
                enlarg = tile2enlargement[ntile];
            }

            int monster_layer_height = data->map[x][y].special_monster_layer_height;
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

    if (data->bAsciiMode || Is_rogue_level(&u.uz)) 
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
    HBITMAP savedBitmap = SelectObject(data->tileDC, GetNHApp()->bmpMapTiles);

    for (int i = 1; i < COLNO; i++)
        for (int j = 0; j < ROWNO; j++)
            if (data->mapDirty[i][j])
                paint(data, i, j);

    SelectObject(data->tileDC, savedBitmap);

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

