/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2024-08-11 */

/* GnollHack 4.0    mhmsgwnd.c    $NHDT-Date: 1432512812 2015/05/25 00:13:32 $  $NHDT-Branch: master $:$NHDT-Revision: 1.32 $ */
/* Copyright (C) 2001 by Alex Kompel      */
/* GnollHack may be freely redistributed.  See license for details. */

#include "winMS.h"
#include "mhmsgwnd.h"
#include "mhmsg.h"
#include "mhfont.h"

#define MSG_WRAP_TEXT

#define MSG_VISIBLE_LINES max(iflags.wc_vary_msgcount, 1)
#define MAX_MSG_LINES 128
#define MSG_LINES (int) min(iflags.msg_history, MAX_MSG_LINES)
#define MAXWINDOWTEXT TBUFSZ

#define DEFAULT_COLOR_BG_MSG COLOR_WINDOW
#define DEFAULT_COLOR_FG_MSG COLOR_WINDOWTEXT

#define MORE "--More--"

struct window_line {
    int attr;
    int color;
    char text[MAXWINDOWTEXT + 1];
    char attrs[MAXWINDOWTEXT + 1];
    char colors[MAXWINDOWTEXT + 1];
};

typedef struct mswin_gnollhack_message_window {
    size_t max_text;
    struct window_line window_text[MAX_MSG_LINES];
    int lines_last_turn; /* lines added during the last turn */
    int lines_not_seen;  /* lines not yet seen by user after last turn or
                            --More-- */
    int nevermore;       /* We want no more --More-- prompts */

    int xChar;  /* horizontal scrolling unit */
    int yChar;  /* vertical scrolling unit */
    int xUpper; /* average width of uppercase letters */
    int xPos;   /* current horizontal scrolling position */
    int yPos;   /* current vertical scrolling position */
    int xMax;   /* maximum horizontal scrolling position */
    int yMax;   /* maximum vertical scrolling position */
    int xPage;  /* page size of horizontal scroll bar */
} NHMessageWindow, *PNHMessageWindow;
#define LINE_PADDING_LEFT(data)  (data->xChar * (2 - data->xPos))
#define LINE_PADDING_RIGHT(data)  (0)

static TCHAR szMessageWindowClass[] = TEXT("MSNHMessageWndClass");
LRESULT CALLBACK NHMessageWndProc(HWND, UINT, WPARAM, LPARAM);
static void register_message_window_class(void);
static void onMSNHCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);
static void onMSNH_VScroll(HWND hWnd, WPARAM wParam, LPARAM lParam);
#ifndef MSG_WRAP_TEXT
static void onMSNH_HScroll(HWND hWnd, WPARAM wParam, LPARAM lParam);
#endif
static COLORREF setMsgTextColor(HDC hdc, int gray, int nhcolor);
static void onPaint(HWND hWnd);
static void onCreate(HWND hWnd, WPARAM wParam, LPARAM lParam);
static BOOL can_append_text(HWND hWnd, /* int attr, int color, */ const char* text);
/* check if text can be appended to the last line without wrapping */

static BOOL more_prompt_check(HWND hWnd);
/* check if "--more--" prompt needs to be displayed */

#ifdef USER_SOUNDS
extern void play_sound_for_message(const char *str);
#endif

HWND
mswin_init_message_window()
{
    static int run_once = 0;
    HWND ret;
    DWORD style;
    RECT rt;

    if (!run_once) {
        register_message_window_class();
        run_once = 1;
    }

    /* get window position */
    if (GetNHApp()->bAutoLayout) {
        SetRect(&rt, 0, 0, 0, 0);
    } else {
        mswin_get_window_placement(NHW_MESSAGE, &rt);
    }

#ifdef MSG_WRAP_TEXT
    style = WS_CHILD | WS_CLIPSIBLINGS | WS_VSCROLL | WS_SIZEBOX;
#else
    style = WS_CHILD | WS_CLIPSIBLINGS | WS_VSCROLL | WS_HSCROLL | WS_SIZEBOX;
#endif

    ret = CreateWindowEx(
        WS_EX_CLIENTEDGE, szMessageWindowClass, /* registered class name */
        NULL,                                   /* window name */
        style,                                  /* window style */
        rt.left,              /* horizontal position of window */
        rt.top,               /* vertical position of window */
        rt.right - rt.left,   /* window width */
        rt.bottom - rt.top,   /* window height */
        GetNHApp()->hMainWnd, /* handle to parent or owner window */
        NULL,                 /* menu handle or child identifier */
        GetNHApp()->hApp,     /* handle to application instance */
        NULL);                /* window-creation data */

    if (!ret)
    {
        panic("Cannot create message window");
        return (HWND)0;
    }
    /* Set window caption */
    SetWindowText(ret, "Messages");

    mswin_apply_window_style(ret);

    return ret;
}

void
register_message_window_class()
{
    WNDCLASS wcex;
    ZeroMemory(&wcex, sizeof(wcex));

    wcex.style = CS_NOCLOSE;
    wcex.lpfnWndProc = (WNDPROC) NHMessageWndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = GetNHApp()->hApp;
    wcex.hIcon = NULL;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = message_bg_brush
                             ? message_bg_brush
                             : SYSCLR_TO_BRUSH(DEFAULT_COLOR_BG_MSG);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szMessageWindowClass;

    RegisterClass(&wcex);
}

LRESULT CALLBACK
NHMessageWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
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
        SetFocus(GetNHApp()->hMainWnd);
        break;

#ifndef MSG_WRAP_TEXT
    case WM_HSCROLL:
        onMSNH_HScroll(hWnd, wParam, lParam);
        break;
#endif

    case WM_VSCROLL:
        onMSNH_VScroll(hWnd, wParam, lParam);
        break;

    case WM_DESTROY: {
        PNHMessageWindow data;
        data = (PNHMessageWindow) GetWindowLongPtr(hWnd, GWLP_USERDATA);
        free(data);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR) 0);
    } break;

    case WM_SIZE: {
        SCROLLINFO si;
        int xNewSize;
        int yNewSize;
        PNHMessageWindow data;
        RECT rt;

        data = (PNHMessageWindow) GetWindowLongPtr(hWnd, GWLP_USERDATA);

        xNewSize = LOWORD(lParam);
        yNewSize = HIWORD(lParam);

        if (xNewSize > 0 || yNewSize > 0) {
#ifndef MSG_WRAP_TEXT
            data->xPage = xNewSize / data->xChar;
            data->xMax = max(0, (int) (1 + data->max_text - data->xPage));
            data->xPos = min(data->xPos, data->xMax);

            ZeroMemory(&si, sizeof(si));
            si.cbSize = sizeof(si);
            si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
            si.nMin = 0;
            si.nMax = data->max_text;
            si.nPage = data->xPage;
            si.nPos = data->xPos;
            SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);
#endif

            data->yMax = MSG_LINES - 1;
            data->yPos = min(data->yPos, data->yMax);

            ZeroMemory(&si, sizeof(si));
            si.cbSize = sizeof(si);
            si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
            si.nMin = MSG_VISIBLE_LINES;
            si.nMax = data->yMax + MSG_VISIBLE_LINES - 1;
            si.nPage = MSG_VISIBLE_LINES;
            si.nPos = data->yPos;
            SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
        }

        /* update GnollHack internal window position */
        GetWindowRect(hWnd, &rt);
        ScreenToClient(GetNHApp()->hMainWnd, (LPPOINT) &rt);
        ScreenToClient(GetNHApp()->hMainWnd, ((LPPOINT) &rt) + 1);
        mswin_update_window_placement(NHW_MESSAGE, &rt);

        /* redraw window - it does not handle incremental resizing too well */
        InvalidateRect(hWnd, NULL, TRUE);
    } break;

    case WM_MOVE: {
        RECT rt;
        GetWindowRect(hWnd, &rt);
        ScreenToClient(GetNHApp()->hMainWnd, (LPPOINT) &rt);
        ScreenToClient(GetNHApp()->hMainWnd, ((LPPOINT) &rt) + 1);
        mswin_update_window_placement(NHW_MESSAGE, &rt);
    } break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void
onMSNHCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    PNHMessageWindow data;

    data = (PNHMessageWindow) GetWindowLongPtr(hWnd, GWLP_USERDATA);
    switch (wParam) {
    case MSNH_MSG_PUTSTR: {
        /* Add the passed in message to the existing text.  Support the
         * adding of text that ends in newline.  A newline in text
         * will force any subsequent text that is added to be added on
         * a new output line.
         *
         * TODO: Text can be added with newlines occurring within the text not
         *       just at the end.  As currently implemented, this can cause
         *       the text to be rendered such that the text following the
         *       newline is rendered on a new line.  This can cause a poor
         *       user experience when the user has set only a single text line
         *       for the message window.  In this case, the user will not see
         *       any line other then the last line of text and the --MORE--
         *       message thus missing any text that appears before the last
         *       embedded newline.  This does not meet the requirements of the
         *       message window.
         *       This code should be changed to do the right thing and split
         *       the text so that only lines that end in newlines are added to
         *       the stored window text.
         */
        PMSNHMsgPutstr msg_data = (PMSNHMsgPutstr) lParam;
        SCROLLINFO si;
        char *p;
        char msgbuf[UTF8BUFSZ] = "";

        if (!msg_data->text)
            return;

        write_CP437_to_buf_unicode(msgbuf, UTF8BUFSZ, msg_data->text);

        int current_length = (int)strlen(data->window_text[MSG_LINES - 1].text);
        int remaining_length = MAXWINDOWTEXT - current_length;
        int text_length = (int)strlen(msg_data->text);
        int used_length = max(0, min(text_length, remaining_length));
        int last_point = min(current_length + used_length, MAXWINDOWTEXT);

        if (msg_data->append == 1)
        {
            /* Forcibly append to line, even if we pass the edge */
            strncat(data->window_text[MSG_LINES - 1].text, msgbuf, remaining_length);

            if(msg_data->attrs)
                memcpy(data->window_text[MSG_LINES - 1].attrs + current_length, msg_data->attrs, used_length);
            else
                memset(data->window_text[MSG_LINES - 1].attrs + current_length, msg_data->attr, used_length);

            if (msg_data->colors)
                memcpy(data->window_text[MSG_LINES - 1].colors + current_length, msg_data->colors, used_length);
            else
                memset(data->window_text[MSG_LINES - 1].colors + current_length, msg_data->color, used_length);

            data->window_text[MSG_LINES - 1].attrs[last_point] = data->window_text[MSG_LINES - 1].colors[last_point] = 0;
        } 
        else if (msg_data->append < 0) 
        {
            /* remove that many chars */
            int newend = max(current_length + msg_data->append, 0);
            data->window_text[MSG_LINES - 1].text[newend] = '\0';
            data->window_text[MSG_LINES - 1].attrs[newend] = data->window_text[MSG_LINES - 1].colors[newend] = 0;
        }
        else 
        {
            if (!(msg_data->attr & ATR_STAY_ON_LINE) && can_append_text(hWnd, /* msg_data->attr, msg_data->color, */ msgbuf))
            {
                strncat(data->window_text[MSG_LINES - 1].text, "  ", remaining_length);
                if (current_length <= MAXWINDOWTEXT - 2)
                {
                    memset(data->window_text[MSG_LINES - 1].attrs + current_length, ATR_NONE, 2);
                    memset(data->window_text[MSG_LINES - 1].colors + current_length, NO_COLOR, 2);
                }

                current_length = (int)strlen(data->window_text[MSG_LINES - 1].text);
                remaining_length = MAXWINDOWTEXT - current_length;
                used_length = max(0, min(text_length, remaining_length));
                last_point = min(current_length + used_length, MAXWINDOWTEXT);

                strncat(data->window_text[MSG_LINES - 1].text, msgbuf, remaining_length);

                if (msg_data->attrs)
                    memcpy(data->window_text[MSG_LINES - 1].attrs + current_length, msg_data->attrs, used_length);
                else
                    memset(data->window_text[MSG_LINES - 1].attrs + current_length, msg_data->attr, used_length);

                if (msg_data->colors)
                    memcpy(data->window_text[MSG_LINES - 1].colors + current_length, msg_data->colors, used_length);
                else
                    memset(data->window_text[MSG_LINES - 1].colors + current_length, msg_data->color, used_length);

                data->window_text[MSG_LINES - 1].attrs[last_point] = data->window_text[MSG_LINES - 1].colors[last_point] = 0;
            } 
            else 
            {
                /* check for "--more--" */
                if (!data->nevermore && !(msg_data->attr & ATR_STAY_ON_LINE) && more_prompt_check(hWnd)) 
                {
                    int okkey = 0;
                    char tmptext[MAXWINDOWTEXT + 1];
                    char tmpattrs[MAXWINDOWTEXT + 1];
                    char tmpcolors[MAXWINDOWTEXT + 1];

                    // @@@ Ok respnses

                    /* save original text */
                    Strcpy(tmptext, data->window_text[MSG_LINES - 1].text);
                    memcpy(tmpattrs, data->window_text[MSG_LINES - 1].attrs, MAXWINDOWTEXT + 1);
                    memcpy(tmpcolors, data->window_text[MSG_LINES - 1].colors, MAXWINDOWTEXT + 1);

                    /* text could end in newline so strip it */
                    strip_newline(data->window_text[MSG_LINES - 1].text);
                    current_length = (int)strlen(data->window_text[MSG_LINES - 1].text);
                    remaining_length = MAXWINDOWTEXT - current_length;

                    /* append more prompt and indicate the update */
                    strncat(data->window_text[MSG_LINES - 1].text, MORE, remaining_length);
                    int morelen = (int)strlen(MORE);
                    if (current_length <= MAXWINDOWTEXT - morelen)
                    {
                        memset(data->window_text[MSG_LINES - 1].attrs + current_length, ATR_NONE, morelen);
                        memset(data->window_text[MSG_LINES - 1].colors + current_length, NO_COLOR, morelen);
                    }

                    InvalidateRect(hWnd, NULL, TRUE);

                    /* get the input */
                    while (!okkey) 
                    {
                        int c = mswin_nhgetch();

                        switch (c) 
                        {
                        /* space or enter */
                        case ' ':
                        case '\015':
                            okkey = 1;
                            break;
                        /* ESC */
                        case '\033':
                            data->nevermore = 1;
                            okkey = 1;
                            break;
                        default:
                            break;
                        }
                    }

                    /* restore original text */
                    Strcpy(data->window_text[MSG_LINES - 1].text, tmptext);
                    memcpy(data->window_text[MSG_LINES - 1].attrs, tmpattrs, MAXWINDOWTEXT + 1);
                    memcpy(data->window_text[MSG_LINES - 1].colors, tmpcolors, MAXWINDOWTEXT + 1);

                    data->lines_not_seen = 0;
                }

                /* check if the string is empty */
                for (p = data->window_text[MSG_LINES - 1].text;
                     *p && isspace((uchar) *p); p++)
                    ;

                if (!(msg_data->attr & ATR_STAY_ON_LINE))
                {
                    if (*p) 
                    {
                        /* last string is not empty - scroll up */
                        memmove(&data->window_text[0], &data->window_text[1], (MSG_LINES - 1) * sizeof(data->window_text[0]));
                    }
                }
                else
                {
                    /* clear the line instead */
                    Strcpy(data->window_text[MSG_LINES - 1].text, "");
                    memset(data->window_text[MSG_LINES - 1].attrs, ATR_NONE, MAXWINDOWTEXT);
                    memset(data->window_text[MSG_LINES - 1].colors, NO_COLOR, MAXWINDOWTEXT);
                    data->window_text[MSG_LINES - 1].attrs[MAXWINDOWTEXT] = data->window_text[MSG_LINES - 1].colors[MAXWINDOWTEXT] = 0;
                    data->window_text[MSG_LINES - 1].attrs[0] = data->window_text[MSG_LINES - 1].colors[0] = 0;
                }

                /* append new text to the end of the array */
                data->window_text[MSG_LINES - 1].attr = msg_data->attr;
                data->window_text[MSG_LINES - 1].color = msg_data->color;
                (void)strncpy(data->window_text[MSG_LINES - 1].text, msgbuf, MAXWINDOWTEXT);

                used_length = max(0, min(text_length, MAXWINDOWTEXT));
                last_point = min(used_length, MAXWINDOWTEXT);
                if (msg_data->attrs)
                    memcpy(data->window_text[MSG_LINES - 1].attrs, msg_data->attrs, used_length);
                else
                    memset(data->window_text[MSG_LINES - 1].attrs, msg_data->attr, used_length);

                if (msg_data->colors)
                    memcpy(data->window_text[MSG_LINES - 1].colors, msg_data->colors, used_length);
                else
                    memset(data->window_text[MSG_LINES - 1].colors, msg_data->color, used_length);

                data->window_text[MSG_LINES - 1].attrs[last_point] = data->window_text[MSG_LINES - 1].colors[last_point] = 0;

                if (!(msg_data->attr & ATR_STAY_ON_LINE))
                {
                    data->lines_not_seen++;
                    data->lines_last_turn++;
                }
            }
        }

        /* reset V-scroll position to display new text */
        data->yPos = data->yMax;

        ZeroMemory(&si, sizeof(si));
        si.cbSize = sizeof(si);
        si.fMask = SIF_POS;
        si.nPos = data->yPos;
        SetScrollInfo(hWnd, SB_VERT, &si, TRUE);

        /* update window content */
        InvalidateRect(hWnd, NULL, TRUE);

#ifdef USER_SOUNDS
        if (!GetNHApp()->bNoSounds)
            play_sound_for_message(msgbuf);
#endif
    } break;

    case MSNH_MSG_CLEAR_WINDOW: {
        data->lines_last_turn = 0;
        data->lines_not_seen = 0;
        data->nevermore = 0;
        break;
    }
    case MSNH_MSG_CARET:
        /* Create or destroy a caret */
        if (*(int *) lParam)
            CreateCaret(hWnd, NULL, 0, data->yChar);
        else {
            DestroyCaret();
            /* this means we just did something interactive in this window, so
               we
               don't need a --More-- for the lines above.
               */
            data->lines_not_seen = 0;
        }
        break;

    case MSNH_MSG_GETTEXT: {
        PMSNHMsgGetText msg_data = (PMSNHMsgGetText) lParam;
        int i;
        size_t buflen;

        buflen = 0;
        for (i = 0; i < MSG_LINES; i++)
            if (*data->window_text[i].text) 
            {
                (void)strncpy(&msg_data->buffer[buflen], data->window_text[i].text, msg_data->max_size - buflen);
                memcpy(&msg_data->attrs[buflen], data->window_text[i].attrs, msg_data->max_size - buflen);
                memcpy(&msg_data->colors[buflen], data->window_text[i].colors, msg_data->max_size - buflen);

                buflen += strlen(data->window_text[i].text);
                if (buflen >= msg_data->max_size)
                    break;

                (void)strncpy(&msg_data->buffer[buflen], "\r\n", msg_data->max_size - buflen);
                memset(&msg_data->attrs[buflen], ATR_NONE, msg_data->max_size - buflen);
                memset(&msg_data->colors[buflen], NO_COLOR, msg_data->max_size - buflen);

                buflen += 2;
                if (buflen > msg_data->max_size)
                    break;
            }

        msg_data->buffer[msg_data->max_size] = msg_data->attrs[msg_data->max_size] = msg_data->colors[msg_data->max_size] = 0;
    } break;

    case MSNH_MSG_RANDOM_INPUT:
        nhassert(0); // unexpected
        break;

    } /* switch( wParam ) */
}

void
onMSNH_VScroll(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    PNHMessageWindow data;
    SCROLLINFO si;
    int yInc;

    UNREFERENCED_PARAMETER(lParam);

    /* get window data */
    data = (PNHMessageWindow) GetWindowLongPtr(hWnd, GWLP_USERDATA);

    ZeroMemory(&si, sizeof(si));
    si.cbSize = sizeof(si);
    si.fMask = SIF_PAGE | SIF_POS;
    GetScrollInfo(hWnd, SB_VERT, &si);

    switch (LOWORD(wParam)) {
    // User clicked the shaft above the scroll box.

    case SB_PAGEUP:
        yInc = -(int) si.nPage;
        break;

    // User clicked the shaft below the scroll box.

    case SB_PAGEDOWN:
        yInc = si.nPage;
        break;

    // User clicked the top arrow.

    case SB_LINEUP:
        yInc = -1;
        break;

    // User clicked the bottom arrow.

    case SB_LINEDOWN:
        yInc = 1;
        break;

    // User dragged the scroll box.

    case SB_THUMBTRACK:
        yInc = HIWORD(wParam) - data->yPos;
        break;

    default:
        yInc = 0;
    }

    // If applying the vertical scrolling increment does not
    // take the scrolling position out of the scrolling range,
    // increment the scrolling position, adjust the position
    // of the scroll box, and update the window. UpdateWindow
    // sends the WM_PAINT message.

    if (yInc = max(MSG_VISIBLE_LINES - data->yPos,
                   min(yInc, data->yMax - data->yPos))) {
        data->yPos += yInc;
        /* ScrollWindowEx(hWnd, 0, -data->yChar * yInc,
                (CONST RECT *) NULL, (CONST RECT *) NULL,
                (HRGN) NULL, (LPRECT) NULL, SW_INVALIDATE | SW_ERASE);
        */
        InvalidateRect(hWnd, NULL, TRUE);

        ZeroMemory(&si, sizeof(si));
        si.cbSize = sizeof(si);
        si.fMask = SIF_POS;
        si.nPos = data->yPos;
        SetScrollInfo(hWnd, SB_VERT, &si, TRUE);

        UpdateWindow(hWnd);
    }
}

#ifndef MSG_WRAP_TEXT
void
onMSNH_HScroll(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    PNHMessageWindow data;
    SCROLLINFO si;
    int xInc;

    /* get window data */
    data = (PNHMessageWindow) GetWindowLongPtr(hWnd, GWLP_USERDATA);

    ZeroMemory(&si, sizeof(si));
    si.cbSize = sizeof(si);
    si.fMask = SIF_PAGE;
    GetScrollInfo(hWnd, SB_HORZ, &si);

    switch (LOWORD(wParam)) {
    // User clicked shaft left of the scroll box.

    case SB_PAGEUP:
        xInc = -(int) si.nPage;
        break;

    // User clicked shaft right of the scroll box.

    case SB_PAGEDOWN:
        xInc = si.nPage;
        break;

    // User clicked the left arrow.

    case SB_LINEUP:
        xInc = -1;
        break;

    // User clicked the right arrow.

    case SB_LINEDOWN:
        xInc = 1;
        break;

    // User dragged the scroll box.

    case SB_THUMBTRACK:
        xInc = HIWORD(wParam) - data->xPos;
        break;

    default:
        xInc = 0;
    }

    // If applying the horizontal scrolling increment does not
    // take the scrolling position out of the scrolling range,
    // increment the scrolling position, adjust the position
    // of the scroll box, and update the window.

    if (xInc = max(-data->xPos, min(xInc, data->xMax - data->xPos))) {
        data->xPos += xInc;
        ScrollWindowEx(hWnd, -data->xChar * xInc, 0, (CONST RECT *) NULL,
                       (CONST RECT *) NULL, (HRGN) NULL, (LPRECT) NULL,
                       SW_INVALIDATE | SW_ERASE);

        ZeroMemory(&si, sizeof(si));
        si.cbSize = sizeof(si);
        si.fMask = SIF_POS;
        si.nPos = data->xPos;
        SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);
        UpdateWindow(hWnd);
    }
}
#endif // MSG_WRAP_TEXT

COLORREF
setMsgTextColor(HDC hdc, int gray, int nhcolor)
{
    COLORREF fg, color1, color2; // , adjcolor;
    if (nhcolor == NO_COLOR)
    {
        if (gray)
        {
            if (message_bg_brush)
            {
                color1 = message_bg_color;
                color2 = message_fg_color;
            }
            else 
            {
                color1 = (COLORREF)GetSysColor(DEFAULT_COLOR_BG_MSG);
                color2 = (COLORREF)GetSysColor(DEFAULT_COLOR_FG_MSG);
            }
            /* Make a "gray" color by taking the average of the individual R,G,B
               components of two colors. Thanks to Jonathan del Strother */
            fg = RGB((GetRValue(color1) + GetRValue(color2)) / 2,
                (GetGValue(color1) + GetGValue(color2)) / 2,
                (GetBValue(color1) + GetBValue(color2)) / 2);
        }
        else 
        {
            fg = message_fg_brush ? message_fg_color
                : (COLORREF)GetSysColor(DEFAULT_COLOR_FG_MSG);
        }
    }
    else
    {
        if (gray)
        {
            if (message_bg_brush)
            {
                color1 = message_bg_color;
                color2 = GetNHApp()->regMapColors[nhcolor];
            }
            else
            {
                color1 = (COLORREF)GetSysColor(DEFAULT_COLOR_BG_MSG);
                color2 = GetNHApp()->regMapColors[nhcolor];
            }
            fg = RGB((GetRValue(color1) + GetRValue(color2)) / 2,
                (GetGValue(color1) + GetGValue(color2)) / 2,
                (GetBValue(color1) + GetBValue(color2)) / 2);
        }
        else
        {
            fg = GetNHApp()->regMapColors[nhcolor];
        }
    }

    return SetTextColor(hdc, fg);
}

void
onPaint(HWND hWnd)
{
    PAINTSTRUCT ps;
    HDC hdc;
    PNHMessageWindow data;
    RECT client_rt, draw_rt;
    int FirstLine, LastLine;
    int i, y;
    HGDIOBJ oldFont;
    TCHAR wbuf[MAXWINDOWTEXT + 2];
    size_t wlen, wlen2, wlen3;
    COLORREF OldBg, OldFg;

    hdc = BeginPaint(hWnd, &ps);

    OldBg = SetBkColor(
        hdc, message_bg_brush ? message_bg_color
                              : (COLORREF) GetSysColor(DEFAULT_COLOR_BG_MSG));
    OldFg = setMsgTextColor(hdc, 0, NO_COLOR);

    data = (PNHMessageWindow) GetWindowLongPtr(hWnd, GWLP_USERDATA);

    GetClientRect(hWnd, &client_rt);

    if (!IsRectEmpty(&ps.rcPaint)) 
    {
        FirstLine = max(
            0, data->yPos - (client_rt.bottom - ps.rcPaint.top) / data->yChar
                   + 1);
        LastLine =
            min(MSG_LINES - 1,
                data->yPos
                    - (client_rt.bottom - ps.rcPaint.bottom) / data->yChar);
        y = min(ps.rcPaint.bottom, client_rt.bottom);
        LONG client_width = client_rt.right - LINE_PADDING_RIGHT(data) - LINE_PADDING_LEFT(data);

        for (i = LastLine; i >= FirstLine; i--) 
        {
            char tmptext[MAXWINDOWTEXT + 1];

            draw_rt.left = LINE_PADDING_LEFT(data);
            draw_rt.right = client_rt.right - LINE_PADDING_RIGHT(data);
            draw_rt.top = y - data->yChar;
            draw_rt.bottom = y;
            
            cached_font * font = mswin_get_font(NHW_MESSAGE, data->window_text[i].attr, hdc, FALSE);
            oldFont = SelectObject(hdc, font->hFont);

            /* convert to UNICODE stripping newline */
            Strcpy(tmptext, data->window_text[i].text);
            strip_newline(tmptext);
            NH_A2W(tmptext, wbuf, sizeof(wbuf));
            wbuf[MAXWINDOWTEXT + 1] = '\0';
            wlen = _tcslen(wbuf);
#ifdef MSG_WRAP_TEXT
            TCHAR* wp = wbuf;
            char* ap = data->window_text[i].attrs;
            char* cp = data->window_text[i].colors;
            TCHAR* wp2 = wp;
            char* ap2 = ap;
            char* cp2 = cp;
            LONG x = 0;
            /* Calculate the length of the whole line */
            DrawText(hdc, wbuf, wlen, &draw_rt, DT_NOPREFIX | DT_CALCRECT);
            if (draw_rt.right > client_rt.right - LINE_PADDING_RIGHT(data))
            {
                LONG width = max(1, draw_rt.right - draw_rt.left);
                LONG mult = max(1, client_width / width);
                y -= mult * (draw_rt.bottom - draw_rt.top);
            }

            LONG line_height = draw_rt.bottom - draw_rt.top;
            LONG line_start_y = y;

            while (*wp)
            {
                /* set attribute on based on *ap */
                setMsgTextColor(hdc, i < (MSG_LINES - data->lines_last_turn), *cp /* data->window_text[i].color */);
                /* Go forward until a different attribute or color */
                do
                {
                    wp2++;
                    ap2++;
                    cp2++;
                } while (!(!*wp2 || *ap2 != *ap || *cp2 != *cp));

                TCHAR wc = *wp2; /* Store the char at the pointer */
                *wp2 = 0; /* Cut the string at the pointer */
                wlen2 = _tcslen(wp); /* Calculate the length of the cut string */

                /* Start of word wrap */
                TCHAR* wp3 = wp;
                char* ap3 = ap;
                char* cp3 = cp;

                while (*wp3)
                {
                    draw_rt.left = LINE_PADDING_LEFT(data) + x;
                    draw_rt.right = client_rt.right - LINE_PADDING_RIGHT(data) - x;
                    draw_rt.top = y - data->yChar;
                    draw_rt.bottom = y;

                    TCHAR* wp4 = wp3;
                    char* ap4 = ap3;
                    char* cp4 = cp3;
                    do
                    {
                        wp4++;
                        ap4++;
                        cp4++;
                    } while (!(!*wp4 || *wp4 == ' ' || *wp4 == '\n'));

                    TCHAR wcww = *wp4;
                    *wp4 = 0;
                    wlen3 = _tcslen(wp3); /* Calculate the length of the cut string */

                    /* Find out how large the bounding rectangle of the text is */
                    //DrawText(hdc, wp /* wbuf */, wlen2, &draw_rt, DT_NOPREFIX | DT_WORDBREAK | DT_CALCRECT);
                    DrawText(hdc, wp3, wlen3, &draw_rt, DT_NOPREFIX | DT_CALCRECT);

                    if (draw_rt.right > client_rt.right - LINE_PADDING_RIGHT(data))
                    {
                        x = 0;
                        y += (draw_rt.bottom - draw_rt.top);
                        LONG tmp_width = draw_rt.right - draw_rt.left;
                        draw_rt.left = LINE_PADDING_LEFT(data);
                        draw_rt.right = draw_rt.left + tmp_width;
                    }

                    /* move that rectangle up, so that the bottom remains at the same height */
                    draw_rt.top = y - (draw_rt.bottom - draw_rt.top);
                    draw_rt.bottom = y;

                    LONG width = draw_rt.right - draw_rt.left;
                    x += width;

                    /* Now really draw it */
                    //DrawText(hdc, wp /* wbuf */, wlen2, &draw_rt, DT_NOPREFIX | DT_WORDBREAK);
                    DrawText(hdc, wp3 /* wbuf */, wlen3, &draw_rt, DT_NOPREFIX);

                    *wp4 = wcww;
                    wp3 = wp4;
                    ap3 = ap4;
                    cp3 = cp4;
                }

#if 0
                /* Find out the cursor (caret) position */
                if (i == MSG_LINES - 1) 
                {
                    int nnum, numfit;
                    SIZE size = { 0 };
                    TCHAR* nbuf;
                    int nlen;

                    nbuf = wp; // wbuf;
                    nlen = wlen2; // wlen;
                    while (nlen) 
                    {
                        /* Get the number of characters that fit on the line */
                        GetTextExtentExPoint(hdc, nbuf, nlen,
                            draw_rt.right - draw_rt.left,
                            &numfit, NULL, &size);
                        /* Search back to a space */
                        nnum = numfit;
                        if (numfit < nlen) 
                        {
                            while (nnum > 0 && nbuf[nnum] != ' ')
                                nnum--;
                            /* If no space found, break wherever */
                            if (nnum == 0)
                                nnum = numfit;
                        }
                        nbuf += nnum;
                        nlen -= nnum;
                        if (*nbuf == ' ') 
                        {
                            nbuf++;
                            nlen--;
                        }
                    }
                    /* The last size is the size of the last line. Set the caret
                       there.
                       This will fail automatically if we don't own the caret
                       (i.e.,
                       when not in a question.)
                     */
                    SetCaretPos(draw_rt.left + size.cx,
                        draw_rt.bottom - data->yChar);
                }
#endif
                /* Find out the cursor (caret) position */
                if (i == MSG_LINES - 1)
                {
                    SetCaretPos(LINE_PADDING_LEFT(data) + x, y - data->yChar);
                }

                *wp2 = wc; /* Restore the char */

                wp = wp2;
                ap = ap2;
                cp = cp2;
            }
#else
            setMsgTextColor(hdc, i < (MSG_LINES - data->lines_last_turn), data->window_text[i].color);
            DrawText(hdc, wbuf, wlen, &draw_rt, DT_NOPREFIX);
            SetCaretPos(draw_rt.left + size.cx, draw_rt.bottom - data->yChar);
#endif
            SelectObject(hdc, oldFont);
            y = line_start_y - line_height;
        }
    }
    SetTextColor(hdc, OldFg);
    SetBkColor(hdc, OldBg);
    EndPaint(hWnd, &ps);
}

void
onCreate(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    PNHMessageWindow data;
    SIZE dummy;

    UNREFERENCED_PARAMETER(wParam);
    UNREFERENCED_PARAMETER(lParam);

    /* set window data */
    data = (PNHMessageWindow) malloc(sizeof(NHMessageWindow));
    if (!data)
    {
        panic("out of memory");
        return;
    }
    ZeroMemory(data, sizeof(NHMessageWindow));
    data->max_text = MAXWINDOWTEXT;
    SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR) data);

    /* re-calculate window size (+ font size) */
    mswin_message_window_size(hWnd, &dummy);
}

void
mswin_message_window_size(HWND hWnd, LPSIZE sz)
{
    HDC hdc;
    HGDIOBJ saveFont;
    TEXTMETRIC tm;
    PNHMessageWindow data;
    RECT rt, client_rt;

    data = (PNHMessageWindow) GetWindowLongPtr(hWnd, GWLP_USERDATA);
    if (!data)
        return;

    /* -- Calculate the font size -- */
    /* Get the handle to the client area's device context. */
    hdc = GetDC(hWnd);
    cached_font * font = mswin_get_font(NHW_MESSAGE, ATR_NONE, hdc, FALSE);
    saveFont = SelectObject(hdc, font->hFont);

    /* Extract font dimensions from the text metrics. */
    GetTextMetrics(hdc, &tm);
    data->xChar = tm.tmAveCharWidth;
    data->xUpper = (tm.tmPitchAndFamily & 1 ? 3 : 2) * data->xChar / 2;
    data->yChar = tm.tmHeight + tm.tmExternalLeading;
    data->xPage = 1;

    /* Free the device context.  */
    SelectObject(hdc, saveFont);
    ReleaseDC(hWnd, hdc);

    /* -- calculate window size -- */
    GetWindowRect(hWnd, &rt);
    sz->cx = rt.right - rt.left;
    sz->cy = rt.bottom - rt.top;

    /* set size to accomodate MSG_VISIBLE_LINES and
       horizontal scroll bar (difference between window rect and client rect
       */
    GetClientRect(hWnd, &client_rt);
    sz->cy = sz->cy - (client_rt.bottom - client_rt.top)
             + data->yChar * MSG_VISIBLE_LINES;
}

/* check if text can be appended to the last line without wrapping */
BOOL
can_append_text(HWND hWnd, /* int attr, int color, */ const char* text)
{
    PNHMessageWindow data;
    char tmptext[MAXWINDOWTEXT + 1];
    HDC hdc;
    HGDIOBJ saveFont;
    RECT draw_rt;
    BOOL retval = FALSE;

    data = (PNHMessageWindow) GetWindowLongPtr(hWnd, GWLP_USERDATA);

    /* cannot append if lines_not_seen is 0 (beginning of the new turn */
    if (data->lines_not_seen == 0)
        return FALSE;

    /* cannot append text with different attrbutes */
    //if (data->window_text[MSG_LINES - 1].attr != attr)
    //    return FALSE;
    //if (data->window_text[MSG_LINES - 1].color != color)
    //    return FALSE;

    /* cannot append if current line ends in newline */
    if (str_end_is(data->window_text[MSG_LINES - 1].text, "\n"))
        return FALSE;

    /* check if the maximum string langth will be exceeded */
    if (strlen(data->window_text[MSG_LINES - 1].text) + 2
            + /* space characters */
            strlen(text) + strlen(MORE)
        >= MAXWINDOWTEXT)
        return FALSE;

    /* check if the text is going to fit into a single line */
    Strcpy(tmptext, data->window_text[MSG_LINES - 1].text);
    Strcat(tmptext, "  ");
    Strcat(tmptext, text);
    strip_newline(tmptext);
    Strcat(tmptext, MORE);

    hdc = GetDC(hWnd);
    cached_font * font = mswin_get_font(NHW_MESSAGE, data->window_text[MSG_LINES - 1].attr, hdc, FALSE);
    saveFont = SelectObject(hdc, font->hFont);
    GetClientRect(hWnd, &draw_rt);
    draw_rt.left += LINE_PADDING_LEFT(data);
    draw_rt.right -= LINE_PADDING_RIGHT(data);
    draw_rt.bottom = draw_rt.top; /* we only need width for the DrawText */
    DrawText(hdc, tmptext, strlen(tmptext), &draw_rt, DT_NOPREFIX | DT_WORDBREAK | DT_CALCRECT);

    /* we will check against 1.5 of the font size in order to determine
       if the text is single-line or not - just to be on the safe size */
    retval = (draw_rt.bottom - draw_rt.top) < (data->yChar + data->yChar / 2);

    /* free device context */
    SelectObject(hdc, saveFont);
    ReleaseDC(hWnd, hdc);
    return retval;
}

/* check if "--more--" prompt needs to be displayed
   basically, check if the lines not seen are going to find in the message
   window
*/
BOOL
more_prompt_check(HWND hWnd)
{
    PNHMessageWindow data;
    HDC hdc;
    HGDIOBJ saveFont;
    RECT client_rt, draw_rt;
    int i;
    int remaining_height;
    char tmptext[MAXWINDOWTEXT + 1];

    data = (PNHMessageWindow) GetWindowLongPtr(hWnd, GWLP_USERDATA);

    if (data->lines_not_seen == 0)
        return FALSE; /* don't bother checking - nothig to "more" */
    if (data->lines_not_seen >= MSG_LINES)
        return TRUE; /* history size exceeded - always more */

    GetClientRect(hWnd, &client_rt);
    remaining_height = client_rt.bottom - client_rt.top;

    hdc = GetDC(hWnd);
    cached_font * font = mswin_get_font(NHW_MESSAGE, ATR_NONE, hdc, FALSE);
    saveFont = SelectObject(hdc, font->hFont);
    for (i = 0; i < data->lines_not_seen; i++) {
        /* we only need width for the DrawText */
        SetRect(&draw_rt,
            client_rt.left + LINE_PADDING_LEFT(data), client_rt.top,
            client_rt.right - LINE_PADDING_RIGHT(data), client_rt.top);
        font = mswin_get_font(NHW_MESSAGE,
                        data->window_text[MSG_LINES - i - 1].attr, hdc, FALSE);
        SelectObject(hdc, font->hFont);

        Strcpy(tmptext, data->window_text[MSG_LINES - i - 1].text);
        strip_newline(tmptext);

        if (i == 0)
            Strcat(tmptext, MORE);

        remaining_height -=
            DrawText(hdc, tmptext, strlen(tmptext), &draw_rt,
                     DT_NOPREFIX | DT_WORDBREAK | DT_CALCRECT);
        if (remaining_height <= 0)
            break;
    }

    /* free device context */
    SelectObject(hdc, saveFont);
    ReleaseDC(hWnd, hdc);
    return (remaining_height
            <= 0); /* TRUE if lines_not_seen take more that window height */
}
