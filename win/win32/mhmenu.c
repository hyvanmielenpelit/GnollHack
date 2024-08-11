/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2024-08-11 */

/* GnollHack 4.0    mhmenu.c    $NHDT-Date: 1432512811 2015/05/25 00:13:31 $  $NHDT-Branch: master $:$NHDT-Revision: 1.48 $ */
/* Copyright (c) Alex Kompel, 2002                                */
/* GnollHack may be freely redistributed.  See license for details. */

#include "win10.h"
#include "winMS.h"
#include <assert.h>
#include "resource.h"
#include "mhmenu.h"
#include "mhmain.h"
#include "mhmsg.h"
#include "mhfont.h"
#include "mhdlg.h"

#define MENU_MARGIN 0
#define NHMENU_STR_SIZE BUFSZ
#define MIN_TABSTOP_SIZE 0
#define NUMTABS 15
#define TAB_SEPARATION 10 /* pixels between each tab stop */

#define DEFAULT_COLOR_BG_TEXT COLOR_WINDOW
#define DEFAULT_COLOR_FG_TEXT COLOR_WINDOWTEXT
#define DEFAULT_COLOR_BG_MENU COLOR_WINDOW
#define DEFAULT_COLOR_FG_MENU COLOR_WINDOWTEXT

#define CHECK_WIDTH 16
#define CHECK_HEIGHT 16

typedef struct mswin_menu_item {
    int glyph;
    ANY_P identifier;
    struct obj object_data;
    CHAR_P accelerator;
    CHAR_P group_accel;
    int attr;
    int color;
    char str[NHMENU_STR_SIZE];
    BOOLEAN_P presel;
    int count;
    BOOL has_focus;
    boolean is_animated;
    uint64_t miflags;
} NHMenuItem, *PNHMenuItem;

typedef struct mswin_gnollhack_menu_window {
    int type; /* MENU_TYPE_TEXT or MENU_TYPE_MENU */
    int how;  /* for menus: PICK_NONE, PICK_ONE, PICK_ANY */
    int style;

    union {
        struct menu_list {
            int size;            /* number of items in items[] */
            int allocated;       /* number of allocated slots in items[] */
            PNHMenuItem items;   /* menu items */
            char gacc[QBUFSZ];   /* group accelerators */
            BOOL counting;       /* counting flag */
            char prompt[QBUFSZ]; /* menu prompt */
            int tab_stop_size[NUMTABS]; /* tabstops to align option values */
            int menu_cx;                /* menu width */
        } menu;

        struct menu_text {
            TCHAR *text;
            char* attrs;
            char* colors;
            SIZE text_box_size;
        } text;
    };
    int result;
    int done;

    HBITMAP bmpChecked;
    HBITMAP bmpCheckedCount;
    HBITMAP bmpNotChecked;
    HDC bmpDC;

    BOOL is_active;
} NHMenuWindow, *PNHMenuWindow;

static WNDPROC wndProcListViewOrig = NULL;
static WNDPROC editControlWndProc = NULL;

#define NHMENU_IS_SELECTABLE(item) ((item).identifier.a_obj != NULL)
#define NHMENU_IS_SELECTED(item) ((item).count != 0)
#define NHMENU_HAS_GLYPH(item) ((item).glyph != NO_GLYPH)

INT_PTR CALLBACK MenuWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK NHMenuListWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK NHMenuTextWndProc(HWND, UINT, WPARAM, LPARAM);
static void onMSNHCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);
static BOOL onMeasureItem(HWND hWnd, WPARAM wParam, LPARAM lParam);
static BOOL onDrawItem(HWND hWnd, WPARAM wParam, LPARAM lParam);
static void LayoutMenu(HWND hwnd);
static void SetMenuType(HWND hwnd, int type);
static void SetMenuListType(HWND hwnd, int now);
static HWND GetMenuControl(HWND hwnd);
static void SelectMenuItem(HWND hwndList, PNHMenuWindow data, int item,
                           int count);
static void reset_menu_count(HWND hwndList, PNHMenuWindow data);
static BOOL onListChar(HWND hWnd, HWND hwndList, WORD ch);

/*-----------------------------------------------------------------------------*/
HWND
mswin_init_menu_window(int type)
{
    HWND ret;
    RECT rt;

    /* get window position */
    if (GetNHApp()->bAutoLayout) {
        SetRect(&rt, 0, 0, 0, 0);
    } else {
        mswin_get_window_placement(NHW_MENU, &rt);
    }

    /* create menu window object */
    ret = CreateDialog(GetNHApp()->hApp, MAKEINTRESOURCE(IDD_MENU),
                       GetNHApp()->hMainWnd, MenuWndProc);
    if (!ret) {
        panic("Cannot create menu window");
        return (HWND)0;
    }

    /* move it in the predefined position */
    if (!GetNHApp()->bAutoLayout) {
        MoveWindow(ret, rt.left, rt.top, rt.right - rt.left,
                   rt.bottom - rt.top, TRUE);
    }

    /* Set window caption */
    SetWindowText(ret, "Menu/Text");

    mswin_apply_window_style(ret);

    SetMenuType(ret, type);
    return ret;
}
/*-----------------------------------------------------------------------------*/
int
mswin_menu_window_select_menu(HWND hWnd, int how, MENU_ITEM_P **_selected,
                              BOOL activate)
{
    PNHMenuWindow data;
    int ret_val;
    MENU_ITEM_P *selected = NULL;
    int i;
    char *ap;

    assert(_selected != NULL);
    *_selected = NULL;
    ret_val = -1;

    data = (PNHMenuWindow) GetWindowLongPtr(hWnd, GWLP_USERDATA);

    /* force activate for certain menu types */
    if (data->type == MENU_TYPE_MENU
        && (how == PICK_ONE || how == PICK_ANY)) {
        activate = TRUE;
    }

    data->is_active = activate && !GetNHApp()->regGnollHackMode;
    animation_timers.general_animation_counter = 0UL;

    /* set menu type */
    SetMenuListType(hWnd, how);

    /* Ok, now give items a unique accelerators */
    if (data->type == MENU_TYPE_MENU) {
        char next_char = 'a';

        data->menu.gacc[0] = '\0';
        ap = data->menu.gacc;
        for (i = 0; i < data->menu.size; i++) {
            if (data->menu.items[i].accelerator != 0) {
                if (isalpha(data->menu.items[i].accelerator)) {
                    next_char = (char)(data->menu.items[i].accelerator + 1);
                }
            } else if (NHMENU_IS_SELECTABLE(data->menu.items[i])) {
                if (isalpha(next_char)) {
                    data->menu.items[i].accelerator = next_char;
                } else {
                    if (next_char > 'z')
                        next_char = 'A';
                    else if (next_char > 'Z')
                        next_char = 'a';

                    data->menu.items[i].accelerator = next_char;
                }

                next_char++;
            }
        }

        /* collect group accelerators */
        for (i = 0; i < data->menu.size; i++) {
            if (data->how != PICK_NONE) {
                if (data->menu.items[i].group_accel
                    && !strchr(data->menu.gacc,
                               data->menu.items[i].group_accel)) {
                    *ap++ = data->menu.items[i].group_accel;
                    *ap = '\x0';
                }
            }
        }

        reset_menu_count(NULL, data);
    }

    LayoutMenu(hWnd); // show dialog buttons

    if (activate) {
        mswin_popup_display(hWnd, &data->done, (data->how == PICK_NONE));
    } else {
        SetFocus(GetNHApp()->hMainWnd);
        mswin_layout_main_window(hWnd);
    }

    /* get the result */
    if (data->result != -1) {
        if (how == PICK_NONE) {
            if (data->result >= 0)
                ret_val = 0;
            else
                ret_val = -1;
        } else if (how == PICK_ONE || how == PICK_ANY) {
            /* count selected items */
            ret_val = 0;
            for (i = 0; i < data->menu.size; i++) {
                if (NHMENU_IS_SELECTABLE(data->menu.items[i])
                    && NHMENU_IS_SELECTED(data->menu.items[i])) {
                    ret_val++;
                }
            }
            if (ret_val > 0) {
                int sel_ind;

                selected =
                    (MENU_ITEM_P *) malloc(ret_val * sizeof(MENU_ITEM_P));
                if (!selected)
                {
                    panic("out of memory");
                    return 0;
                }

                sel_ind = 0;
                for (i = 0; i < data->menu.size; i++) {
                    if (NHMENU_IS_SELECTABLE(data->menu.items[i])
                        && NHMENU_IS_SELECTED(data->menu.items[i])) {
                        selected[sel_ind].item =
                            data->menu.items[i].identifier;
                        selected[sel_ind].count = data->menu.items[i].count;
                        sel_ind++;
                    }
                }
                ret_val = sel_ind;
                *_selected = selected;
            }
        }
    }

    if (activate) {
        data->is_active = FALSE;
        LayoutMenu(hWnd); // hide dialog buttons
        mswin_popup_destroy(hWnd);

        /* If we just used the permanent inventory window to pick something,
         * set the menu back to its display inventory state.
         */
        if (iflags.perm_invent && mswin_winid_from_handle(hWnd) == WIN_INVEN
            && how != PICK_NONE) {
            data->menu.prompt[0] = '\0';
            SetMenuListType(hWnd, PICK_NONE);
            for (i = 0; i < data->menu.size; i++)
                data->menu.items[i].count = 0;
            LayoutMenu(hWnd);
        }
    }

    //SetTimer(hWnd, 0, ANIMATION_TIMER_INTERVAL, NULL);

    return ret_val;
}
/*-----------------------------------------------------------------------------*/
INT_PTR CALLBACK
MenuWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PNHMenuWindow data = (PNHMenuWindow) GetWindowLongPtr(hWnd, GWLP_USERDATA);
    HWND control = GetDlgItem(hWnd, IDC_MENU_TEXT);
    TCHAR title[MAX_LOADSTRING];

    switch (message) {
    case WM_INITDIALOG: {

        HDC hdc = GetDC(control);
        data = (PNHMenuWindow)malloc(sizeof(NHMenuWindow));
        if (!data)
            return (INT_PTR)0;
        ZeroMemory(data, sizeof(NHMenuWindow));
        data->type = MENU_TYPE_TEXT;
        data->how = PICK_NONE;
        data->result = 0;
        data->done = 0;
        data->bmpChecked =
            LoadBitmap(GetNHApp()->hApp, MAKEINTRESOURCE(IDB_MENU_SEL));
        data->bmpCheckedCount =
            LoadBitmap(GetNHApp()->hApp, MAKEINTRESOURCE(IDB_MENU_SEL_COUNT));
        data->bmpNotChecked =
            LoadBitmap(GetNHApp()->hApp, MAKEINTRESOURCE(IDB_MENU_UNSEL));
        data->bmpDC = CreateCompatibleDC(hdc);
        data->is_active = FALSE;
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)data);

        /* set font for the text cotrol */
        cached_font* font = mswin_get_font(NHW_MENU, ATR_NONE, hdc, FALSE);
        SendMessage(control, WM_SETFONT,
            (WPARAM)font->hFont,
            (LPARAM)0);
        ReleaseDC(control, hdc);

        /* subclass edit control */
        editControlWndProc =
            (WNDPROC)GetWindowLongPtr(control, GWLP_WNDPROC);
        SetWindowLongPtr(control, GWLP_WNDPROC, (LONG_PTR)NHMenuTextWndProc);

        /* Even though the dialog has no caption, you can still set the title
           which shows on Alt-Tab */
        LoadString(GetNHApp()->hApp, IDS_APP_TITLE, title, MAX_LOADSTRING);
        SetWindowText(hWnd, title);

        /* set focus to text control for now */
        SetFocus(control);
    }
                      return FALSE;

    case WM_MSNH_COMMAND:
        onMSNHCommand(hWnd, wParam, lParam);
        break;

    case WM_SIZE: {
        RECT rt;
        LayoutMenu(hWnd);
        GetWindowRect(hWnd, &rt);
        ScreenToClient(GetNHApp()->hMainWnd, (LPPOINT)&rt);
        ScreenToClient(GetNHApp()->hMainWnd, ((LPPOINT)&rt) + 1);
        if (iflags.perm_invent && mswin_winid_from_handle(hWnd) == WIN_INVEN)
            mswin_update_window_placement(NHW_INVEN, &rt);
        else
            mswin_update_window_placement(NHW_MENU, &rt);
    }
                return FALSE;

    case WM_MOVE: {
        RECT rt;
        GetWindowRect(hWnd, &rt);
        ScreenToClient(GetNHApp()->hMainWnd, (LPPOINT)&rt);
        ScreenToClient(GetNHApp()->hMainWnd, ((LPPOINT)&rt) + 1);
        if (iflags.perm_invent && mswin_winid_from_handle(hWnd) == WIN_INVEN)
            mswin_update_window_placement(NHW_INVEN, &rt);
        else
            mswin_update_window_placement(NHW_MENU, &rt);
    }
                return FALSE;

    case WM_CLOSE:
        if (program_state.gameover) {
            data->result = -1;
            data->done = 1;
            program_state.stopprint++;
            return TRUE;
        }
        else
            return FALSE;

    case WM_COMMAND: {
        switch (LOWORD(wParam)) {
        case IDCANCEL:
            if (data->type == MENU_TYPE_MENU
                && (data->how == PICK_ONE || data->how == PICK_ANY)
                && data->menu.counting) {
                HWND list;
                int i;

                /* reset counter if counting is in progress */
                list = GetMenuControl(hWnd);
                i = ListView_GetNextItem(list, -1, LVNI_FOCUSED);
                if (i >= 0) {
                    SelectMenuItem(list, data, i, 0);
                }
                return TRUE;
            }
            else {
                data->result = -1;
                data->done = 1;
            }
            return TRUE;

        case IDOK:
            data->done = 1;
            data->result = 0;
            return TRUE;
        }
    } break;

    case WM_NOTIFY: {
        LPNMHDR lpnmhdr = (LPNMHDR)lParam;
        switch (LOWORD(wParam)) {
        case IDC_MENU_LIST: {
            if (!data || data->type != MENU_TYPE_MENU)
                break;

            switch (lpnmhdr->code) {
            case LVN_ITEMACTIVATE: {
                LPNMLISTVIEW lpnmlv = (LPNMLISTVIEW)lParam;
                if (data->how == PICK_ONE) {
                    if (lpnmlv->iItem >= 0 && lpnmlv->iItem < data->menu.size
                        && NHMENU_IS_SELECTABLE(
                            data->menu.items[lpnmlv->iItem])) {
                        SelectMenuItem(lpnmlv->hdr.hwndFrom, data,
                            lpnmlv->iItem, -1);
                        data->done = 1;
                        data->result = 0;
                        return TRUE;
                    }
                }
            } break;

            case NM_CLICK: {
                LPNMLISTVIEW lpnmitem = (LPNMLISTVIEW)lParam;
                if (lpnmitem->iItem == -1)
                    return 0;
                if (data->how == PICK_ANY) {
                    SelectMenuItem(
                        lpnmitem->hdr.hwndFrom, data, lpnmitem->iItem,
                        NHMENU_IS_SELECTED(data->menu.items[lpnmitem->iItem])
                        ? 0
                        : -1);
                }
                play_ui_sound(UI_SOUND_MENU_SELECT);
            } break;

            case LVN_ITEMCHANGED: {
                LPNMLISTVIEW lpnmlv = (LPNMLISTVIEW)lParam;
                if (lpnmlv->iItem == -1)
                    return 0;
                if (!(lpnmlv->uChanged & LVIF_STATE))
                    return 0;

                if (data->how == PICK_ONE || data->how == PICK_ANY) {
                    data->menu.items[lpnmlv->iItem].has_focus =
                        !!(lpnmlv->uNewState & LVIS_FOCUSED);
                    ListView_RedrawItems(lpnmlv->hdr.hwndFrom, lpnmlv->iItem,
                        lpnmlv->iItem);
                }

                /* update count for single-selection menu (follow the listview
                 * selection) */
                if (data->how == PICK_ONE) {
                    if (lpnmlv->uNewState & LVIS_SELECTED) {
                        SelectMenuItem(lpnmlv->hdr.hwndFrom, data,
                            lpnmlv->iItem, -1);
                    }
                }

                /* check item focus */
                if (data->how == PICK_ONE || data->how == PICK_ANY) {
                    data->menu.items[lpnmlv->iItem].has_focus =
                        !!(lpnmlv->uNewState & LVIS_FOCUSED);
                    ListView_RedrawItems(lpnmlv->hdr.hwndFrom, lpnmlv->iItem,
                        lpnmlv->iItem);
                }
            } break;

            case NM_KILLFOCUS:
                //reset_menu_count(lpnmhdr->hwndFrom, data);
                break;
            }
        } break;
        }
    } break;

    case WM_SETFOCUS:
        if (hWnd != GetNHApp()->hPopupWnd) {
            SetFocus(GetNHApp()->hMainWnd);
        }
        else {
            if (IsWindow(GetMenuControl(hWnd)))
                SetFocus(GetMenuControl(hWnd));
        }
        return FALSE;

    case WM_MEASUREITEM:
        if (wParam == IDC_MENU_LIST)
            return onMeasureItem(hWnd, wParam, lParam);
        else
            return FALSE;

    case WM_DRAWITEM:
        if (wParam == IDC_MENU_LIST)
            return onDrawItem(hWnd, wParam, lParam);
        else
            return FALSE;

    case WM_CTLCOLORSTATIC: { /* sent by edit control before it is drawn */
        HDC hdcEdit = (HDC)wParam;
        HWND hwndEdit = (HWND)lParam;
        if (hwndEdit == GetDlgItem(hWnd, IDC_MENU_TEXT)) {
            SetBkColor(hdcEdit, text_bg_brush ? text_bg_color
                : (COLORREF)GetSysColor(
                    DEFAULT_COLOR_BG_TEXT));
            SetTextColor(hdcEdit, text_fg_brush ? text_fg_color
                : (COLORREF)GetSysColor(
                    DEFAULT_COLOR_FG_TEXT));
            return (INT_PTR)(text_bg_brush
                ? text_bg_brush
                : SYSCLR_TO_BRUSH(DEFAULT_COLOR_BG_TEXT));
        }
        return FALSE;
    }

    case WM_CTLCOLORDLG:
        return (INT_PTR)(text_bg_brush
            ? text_bg_brush
            : SYSCLR_TO_BRUSH(DEFAULT_COLOR_BG_TEXT));

    case WM_DESTROY:
        if (data) {
            DeleteDC(data->bmpDC);
            DeleteObject(data->bmpChecked);
            DeleteObject(data->bmpCheckedCount);
            DeleteObject(data->bmpNotChecked);
            if (data->type == MENU_TYPE_TEXT) {
                if (data->text.text)
                    free(data->text.text);
                if (data->text.attrs)
                    free(data->text.attrs);
                if (data->text.colors)
                    free(data->text.colors);
            }
            free(data);
            SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)0);
        }
        return TRUE;
    case WM_TIMER:
        if (animation_timers.general_animation_counter == GH_LONG_MAX)
            animation_timers.general_animation_counter = 0UL;
        else
            animation_timers.general_animation_counter++;

        for (int i = 0; i < data->menu.size; i++)
        {
            if (data->menu.items[i].is_animated)
                ListView_RedrawItems(GetMenuControl(hWnd), i, i);
        }
        break;

    }
    return FALSE;
}
/*-----------------------------------------------------------------------------*/
void
onMSNHCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    PNHMenuWindow data;

    data = (PNHMenuWindow) GetWindowLongPtr(hWnd, GWLP_USERDATA);
    switch (wParam) {
    case MSNH_MSG_PUTSTR: {
        PMSNHMsgPutstr msg_data = (PMSNHMsgPutstr) lParam;
        HWND text_view;
        TCHAR wbuf[BUFSZ];
        size_t text_size;
        RECT text_rt;
        HGDIOBJ saveFont;
        HDC hdc;

        if (data->type != MENU_TYPE_TEXT)
            SetMenuType(hWnd, MENU_TYPE_TEXT);
        size_t oldsize = 0;
        size_t newsize = strlen(msg_data->text);

        if (!data->text.text) {
            text_size = strlen(msg_data->text) + 1 + (msg_data->append ? 0 : 2);
            data->text.text =
                (TCHAR *) malloc(text_size * sizeof(data->text.text[0]));
            data->text.attrs =
                (char*)malloc(text_size * sizeof(char));
            data->text.colors =
                (char*)malloc(text_size * sizeof(char));
            if (!data->text.text || !data->text.attrs || !data->text.colors)
            {
                if (data->text.text) free((genericptr_t)data->text.text);
                if (data->text.attrs) free((genericptr_t)data->text.attrs);
                if (data->text.colors) free((genericptr_t)data->text.colors);
                return;
            }
            ZeroMemory(data->text.text,
                       text_size * sizeof(data->text.text[0]));
            ZeroMemory(data->text.attrs,
                text_size * sizeof(char));
            memset((genericptr_t)data->text.colors, NO_COLOR,
                text_size * sizeof(char));
        } else {
            oldsize = _tcslen(data->text.text);
            text_size = _tcslen(data->text.text) + strlen(msg_data->text) + 1 + (msg_data->append ? 0 : 2);
            TCHAR* temptxt_ptr = (TCHAR *) realloc(
                (genericptr_t)data->text.text, text_size * sizeof(data->text.text[0]));
            char* tempattr_ptr = (char*)realloc(
                (genericptr_t)data->text.attrs, text_size * sizeof(char));
            char* tempcolor_ptr = (char*)realloc(
                (genericptr_t)data->text.colors, text_size * sizeof(char));
            if (!temptxt_ptr || !tempattr_ptr || !tempcolor_ptr)
            {
                if (temptxt_ptr) free((genericptr_t)temptxt_ptr);
                if (tempattr_ptr) free((genericptr_t)tempattr_ptr);
                if (tempcolor_ptr) free((genericptr_t)tempcolor_ptr);
                return;
            }
            else
            {
                data->text.text = temptxt_ptr;
                data->text.attrs = tempattr_ptr;
                data->text.colors = tempcolor_ptr;
            }
        }
        if (!data->text.text)
            break;

        char msgbuf[BUFSZ * 4] = "";
        write_CP437_to_buf_unicode(msgbuf, BUFSZ * 4, msg_data->text);

        _tcscat(data->text.text, NH_A2W(msgbuf /*msg_data->text*/, wbuf, BUFSZ));
        if(msg_data->attrs)
            memcpy(&data->text.attrs[oldsize], msg_data->attrs, newsize);
        else
            memset(&data->text.attrs[oldsize], msg_data->attr, newsize);

        if (msg_data->colors)
            memcpy(&data->text.colors[oldsize], msg_data->colors, newsize);
        else
            memset(&data->text.colors[oldsize], msg_data->color, newsize);

        if (!msg_data->append)
        {
            _tcscat(data->text.text, TEXT("\r\n"));
            memset(&data->text.attrs[oldsize + newsize], 0, 2);
            memset(&data->text.colors[oldsize + newsize], NO_COLOR, 2);
        }

        text_view = GetDlgItem(hWnd, IDC_MENU_TEXT);
        if (!text_view)
        {
            panic("cannot get text view window");
            return;
        }
        SetWindowText(text_view, data->text.text);

        /* calculate dimensions of the added line of text */
        hdc = GetDC(text_view);
        cached_font * font = mswin_get_font(NHW_MENU, ATR_NONE, hdc, FALSE);
        saveFont = SelectObject(hdc, font->hFont);
        DrawText(hdc, NH_A2W(msgbuf, wbuf, BUFSZ), strlen(msgbuf), &text_rt,
                DT_CALCRECT | DT_TOP | DT_LEFT | DT_NOPREFIX
                | DT_SINGLELINE);
        data->text.text_box_size.cx =
            max(text_rt.right - text_rt.left, data->text.text_box_size.cx);
        data->text.text_box_size.cy += text_rt.bottom - text_rt.top;
        SelectObject(hdc, saveFont);
        ReleaseDC(text_view, hdc);
    } break;

    case MSNH_MSG_STARTMENU: {
        int i;
        if (data->type != MENU_TYPE_MENU)
            SetMenuType(hWnd, MENU_TYPE_MENU);

        if (data->menu.items)
            free(data->menu.items);
        data->how = PICK_NONE;
        data->style = (int)lParam;
        data->menu.items = NULL;
        data->menu.size = 0;
        data->menu.allocated = 0;
        data->done = 0;
        data->result = 0;
        for (i = 0; i < NUMTABS; ++i)
            data->menu.tab_stop_size[i] = MIN_TABSTOP_SIZE;
    } break;

    case MSNH_MSG_ADDMENU: {
        PMSNHMsgAddMenu msg_data = (PMSNHMsgAddMenu) lParam;
        char *p, *p1;
        int new_item;
        HDC hDC;
        int column;
        HFONT saveFont;
        LONG menuitemwidth = 0;
        TEXTMETRIC tm;

        if (data->type != MENU_TYPE_MENU)
            break;

        if (data->menu.size == data->menu.allocated) {
            data->menu.allocated += 10;
            PNHMenuItem tempmenuitem_ptr = (PNHMenuItem) realloc(
                data->menu.items, data->menu.allocated * sizeof(NHMenuItem));
            if (!tempmenuitem_ptr)
                return;
            else
                data->menu.items = tempmenuitem_ptr;
        }
        if (!data->menu.items)
            return;

        new_item = data->menu.size;
        ZeroMemory(&data->menu.items[new_item],
                   sizeof(data->menu.items[new_item]));
        data->menu.items[new_item].glyph = msg_data->glyph;
        data->menu.items[new_item].identifier = *msg_data->identifier;
        data->menu.items[new_item].object_data = !msg_data->object ? zeroobj : *msg_data->object;
        data->menu.items[new_item].accelerator = msg_data->accelerator;
        data->menu.items[new_item].group_accel = msg_data->group_accel;
        data->menu.items[new_item].attr = msg_data->attr;
        data->menu.items[new_item].color = msg_data->color;
        data->menu.items[new_item].miflags = msg_data->miflags;

        char msgbuf[NHMENU_STR_SIZE] = "";
        write_CP437_to_buf_unicode(msgbuf, NHMENU_STR_SIZE, msg_data->str);
        (void)strncpy(data->menu.items[new_item].str, msgbuf,
                NHMENU_STR_SIZE);

    /* prevent & being interpreted as a mnemonic start */
        strNsubst(data->menu.items[new_item].str, "&", "&&", 0);
        data->menu.items[new_item].presel = msg_data->presel;

        /* calculate tabstop size */
        hDC = GetDC(hWnd);
        cached_font * font = mswin_get_font(NHW_MENU, msg_data->attr, hDC, FALSE);
        saveFont = SelectObject(hDC, font->hFont);
        GetTextMetrics(hDC, &tm);
        p1 = data->menu.items[new_item].str;
        p = strchr(data->menu.items[new_item].str, '\t');
        column = 0;
        for (;;) {
            TCHAR wbuf[BUFSZ];
            RECT drawRect;
            SetRect(&drawRect, 0, 0, 1, 1);
            if (p != NULL)
                *p = '\0'; /* for time being, view tab field as zstring */
            DrawText(hDC, NH_A2W(p1, wbuf, BUFSZ), strlen(p1), &drawRect,
                     DT_CALCRECT | DT_LEFT | DT_VCENTER | DT_EXPANDTABS
                         | DT_SINGLELINE);

            if (data->menu.items[new_item].attr & ATR_NOTABS)
            {
                menuitemwidth += drawRect.right - drawRect.left;
                break; /* Only one string with no tabs allowed */
            }
            else
            {
                data->menu.tab_stop_size[column] =
                    max(data->menu.tab_stop_size[column],
                        drawRect.right - drawRect.left);

                menuitemwidth += data->menu.tab_stop_size[column];
            }
            if (p != NULL)
                *p = '\t';
            else /* last string so, */
                break;

            /* add the separation only when not the last item */
            /* in the last item, we break out of the loop, in the statement
             * just above */
            menuitemwidth += TAB_SEPARATION;

            ++column;
            p1 = p + 1;
            p = strchr(p1, '\t');
        }
        SelectObject(hDC, saveFont);
        ReleaseDC(hWnd, hDC);

        /* calculate new menu width */
        data->menu.menu_cx =
            max(data->menu.menu_cx,
                CHECK_WIDTH + GetNHApp()->mapTile_X + menuitemwidth
                    + (tm.tmAveCharWidth + tm.tmOverhang) * 12);

        /* increment size */
        data->menu.size++;
    } break;

    case MSNH_MSG_ENDMENU: {
        PMSNHMsgEndMenu msg_data = (PMSNHMsgEndMenu) lParam;
        if (msg_data->text || msg_data->subtitle)
        {
            char buf[QBUFSZ * 2 + 3] = "";
            Sprintf(buf, "%s%s%s", msg_data->text ? msg_data->text : "", msg_data->text && msg_data->subtitle && strcmp(msg_data->text, "") && strcmp(msg_data->subtitle, "") ? " - " : "", msg_data->subtitle ? msg_data->subtitle : "");

            (void)strncpy(data->menu.prompt, buf,
                    sizeof(data->menu.prompt) - 1);

            HDC hDC = GetDC(hWnd);
            cached_font* font = mswin_get_font(NHW_MENU, ATR_BOLD, hDC, FALSE);
            HFONT saveFont = SelectObject(hDC, font->hFont);
            TEXTMETRIC tm;
            GetTextMetrics(hDC, &tm);

            data->menu.menu_cx =
                max(data->menu.menu_cx,
                    (tm.tmAveCharWidth + tm.tmOverhang) * (strlen(data->menu.prompt) + 1) + GetSystemMetrics(SM_CXVSCROLL));

            ReleaseDC(hWnd, hDC);

        } 
        else 
        {
            ZeroMemory(data->menu.prompt, sizeof(data->menu.prompt));
        }
    } break;

    case MSNH_MSG_RANDOM_INPUT: {
        PostMessage(GetMenuControl(hWnd),
            WM_MSNH_COMMAND, MSNH_MSG_RANDOM_INPUT, 0);
    } break;

    }
}

/*-----------------------------------------------------------------------------*/
void
LayoutMenu(HWND hWnd)
{
    PNHMenuWindow data;
    HWND menu_ok;
    HWND menu_cancel;
    RECT clrt, rt;
    POINT pt_elem, pt_ok, pt_cancel;
    SIZE sz_elem, sz_ok, sz_cancel;

    data = (PNHMenuWindow) GetWindowLongPtr(hWnd, GWLP_USERDATA);
    menu_ok = GetDlgItem(hWnd, IDOK);
    menu_cancel = GetDlgItem(hWnd, IDCANCEL);

    /* get window coordinates */
    GetClientRect(hWnd, &clrt);

    // OK button
    if (data->is_active) {
        GetWindowRect(menu_ok, &rt);
        if (data->type == MENU_TYPE_TEXT
            || (data->type == MENU_TYPE_MENU && data->how == PICK_NONE)) {
            sz_ok.cx = (clrt.right - clrt.left) - 2 * MENU_MARGIN;
        } else {
            sz_ok.cx = (clrt.right - clrt.left) / 2 - 2 * MENU_MARGIN;
        }
        sz_ok.cy = rt.bottom - rt.top;
        pt_ok.x = clrt.left + MENU_MARGIN;
        pt_ok.y = clrt.bottom - MENU_MARGIN - sz_ok.cy;
        ShowWindow(menu_ok, SW_SHOW);
        MoveWindow(menu_ok, pt_ok.x, pt_ok.y, sz_ok.cx, sz_ok.cy, TRUE);
    } else {
        sz_ok.cx = sz_ok.cy = 0;
        pt_ok.x = pt_ok.y = 0;
        ShowWindow(menu_ok, SW_HIDE);
    }

    // CANCEL button
    if (data->is_active
        && !(data->type == MENU_TYPE_TEXT
             || (data->type == MENU_TYPE_MENU && data->how == PICK_NONE))) {
        GetWindowRect(menu_ok, &rt);
        sz_cancel.cx = (clrt.right - clrt.left) / 2 - 2 * MENU_MARGIN;
        pt_cancel.x = (clrt.left + clrt.right) / 2 + MENU_MARGIN;
        sz_cancel.cy = rt.bottom - rt.top;
        pt_cancel.y = clrt.bottom - MENU_MARGIN - sz_cancel.cy;
        ShowWindow(menu_cancel, SW_SHOW);
        MoveWindow(menu_cancel, pt_cancel.x, pt_cancel.y, sz_cancel.cx,
                   sz_cancel.cy, TRUE);
    } else {
        sz_cancel.cx = sz_cancel.cy = 0;
        pt_cancel.x = pt_cancel.y = 0;
        ShowWindow(menu_cancel, SW_HIDE);
    }

    // main menu control
    pt_elem.x = clrt.left + MENU_MARGIN;
    pt_elem.y = clrt.top + MENU_MARGIN;
    sz_elem.cx = (clrt.right - clrt.left) - 2 * MENU_MARGIN;
    if (data->is_active) {
        sz_elem.cy = (clrt.bottom - clrt.top) - max(sz_ok.cy, sz_cancel.cy)
                     - 3 * MENU_MARGIN;
    } else {
        sz_elem.cy = (clrt.bottom - clrt.top) - 2 * MENU_MARGIN;
    }

    if (data->type == MENU_TYPE_MENU) {
        ListView_SetColumnWidth(
            GetMenuControl(hWnd), 0,
            max(clrt.right - clrt.left - GetSystemMetrics(SM_CXVSCROLL),
                0 /*data->menu.menu_cx*/));
    }

    MoveWindow(GetMenuControl(hWnd), pt_elem.x, pt_elem.y, sz_elem.cx,
               sz_elem.cy, TRUE);
}
/*-----------------------------------------------------------------------------*/
void
SetMenuType(HWND hWnd, int type)
{
    PNHMenuWindow data;
    HWND list, text;

    data = (PNHMenuWindow) GetWindowLongPtr(hWnd, GWLP_USERDATA);

    data->type = type;

    text = GetDlgItem(hWnd, IDC_MENU_TEXT);
    list = GetDlgItem(hWnd, IDC_MENU_LIST);
    if (data->type == MENU_TYPE_TEXT) {
        ShowWindow(list, SW_HIDE);
        EnableWindow(list, FALSE);
        EnableWindow(text, TRUE);
        ShowWindow(text, SW_SHOW);
        if (data->is_active)
            SetFocus(text);
    } else {
        ShowWindow(text, SW_HIDE);
        EnableWindow(text, FALSE);
        EnableWindow(list, TRUE);
        ShowWindow(list, SW_SHOW);
        if (data->is_active)
            SetFocus(list);
    }
    LayoutMenu(hWnd);
}
/*-----------------------------------------------------------------------------*/
void
SetMenuListType(HWND hWnd, int how)
{
    PNHMenuWindow data;
    RECT rt;
    DWORD dwStyles;
    char buf[BUFSZ];
    TCHAR wbuf[BUFSZ];
    int nItem;
    int i;
    HWND control;
    LVCOLUMN lvcol;
    //LRESULT fnt;

    data = (PNHMenuWindow) GetWindowLongPtr(hWnd, GWLP_USERDATA);
    if (data->type != MENU_TYPE_MENU)
        return;

    data->how = how;

    switch (how) {
    case PICK_NONE:
        dwStyles = WS_VISIBLE | WS_TABSTOP | WS_CHILD | WS_VSCROLL
                   | WS_HSCROLL | LVS_REPORT | LVS_OWNERDRAWFIXED
                   | LVS_SINGLESEL;
        break;
    case PICK_ONE:
        dwStyles = WS_VISIBLE | WS_TABSTOP | WS_CHILD | WS_VSCROLL
                   | WS_HSCROLL | LVS_REPORT | LVS_OWNERDRAWFIXED
                   | LVS_SINGLESEL;
        break;
    case PICK_ANY:
        dwStyles = WS_VISIBLE | WS_TABSTOP | WS_CHILD | WS_VSCROLL
                   | WS_HSCROLL | LVS_REPORT | LVS_OWNERDRAWFIXED
                   | LVS_SINGLESEL;
        break;
    default:
        panic("how should be one of PICK_NONE, PICK_ONE or PICK_ANY");
        return;
    };

    if (strlen(data->menu.prompt) == 0) {
        dwStyles |= LVS_NOCOLUMNHEADER;
    }

    GetWindowRect(GetDlgItem(hWnd, IDC_MENU_LIST), &rt);
    DestroyWindow(GetDlgItem(hWnd, IDC_MENU_LIST));
    control = CreateWindow(WC_LISTVIEW, NULL, dwStyles, rt.left, rt.top,
                           rt.right - rt.left, rt.bottom - rt.top, hWnd,
                           (HMENU) IDC_MENU_LIST, GetNHApp()->hApp, NULL);
    if (!control)
    {
        panic("cannot create menu control");
        return;
    }

    /* install the hook for the control window procedure */
    wndProcListViewOrig = (WNDPROC) GetWindowLongPtr(control, GWLP_WNDPROC);
    SetWindowLongPtr(control, GWLP_WNDPROC, (LONG_PTR) NHMenuListWndProc);

    /* set control colors */
    ListView_SetBkColor(control, menu_bg_brush ? menu_bg_color
                                               : (COLORREF) GetSysColor(DEFAULT_COLOR_BG_MENU)
    );
    ListView_SetTextBkColor(
        control, menu_bg_brush ? menu_bg_color : (COLORREF) GetSysColor(DEFAULT_COLOR_BG_MENU)
    );
    ListView_SetTextColor(
        control, menu_fg_brush ? menu_fg_color : (COLORREF) GetSysColor(DEFAULT_COLOR_FG_MENU)
    );

    /* set control font */
    HDC hDC = GetDC(control);
    cached_font* font = mswin_get_font(NHW_MENU, ATR_BOLD, hDC, FALSE);
    HFONT saveFont = font->hFont;
    SendMessage(control, WM_SETFONT, (WPARAM)saveFont, (LPARAM) 0);
    ReleaseDC(hWnd, hDC);

    /* add column to the list view */
    MonitorInfo monitorInfo;
    win10_monitor_info(hWnd, &monitorInfo);
    
    SendMessage(hWnd, WM_SETREDRAW, FALSE, (LPARAM)0);

    ZeroMemory(&lvcol, sizeof(lvcol));
    lvcol.mask = LVCF_WIDTH | LVCF_TEXT;
    lvcol.cx = monitorInfo.width;
    
    char querybuf[BUFSZ] = "";
    write_CP437_to_buf_unicode(querybuf, BUFSZ, data->menu.prompt);
    lvcol.pszText = NH_A2W(querybuf, wbuf, BUFSZ);
    
    ListView_InsertColumn(control, 0, &lvcol);
    
    /* add items to the list view */
    for (i = 0; i < data->menu.size; i++) {
        LVITEM lvitem;
        ZeroMemory(&lvitem, sizeof(lvitem));
        char msgbuf[BUFSZ] = "";
        write_CP437_to_buf_unicode(msgbuf, BUFSZ, data->menu.items[i].str);

        Sprintf(buf, "%c - %s", max(data->menu.items[i].accelerator, ' '),
            msgbuf);

        lvitem.mask = LVIF_PARAM | LVIF_STATE | LVIF_TEXT;
        lvitem.iItem = i;
        lvitem.iSubItem = 0;
        lvitem.state = data->menu.items[i].presel ? LVIS_SELECTED : 0;
        lvitem.pszText = NH_A2W(buf, wbuf, BUFSZ);
        lvitem.lParam = (LPARAM) &data->menu.items[i];
        nItem = (int) SendMessage(control, LB_ADDSTRING, (WPARAM) 0,
                                  (LPARAM) buf);
        if (ListView_InsertItem(control, &lvitem) == -1) {
            panic("cannot insert menu item");
            return;
        }
    }
    if (data->is_active)
        SetFocus(control);

    SendMessage(hWnd, WM_SETREDRAW, TRUE, (LPARAM)0);

}
/*-----------------------------------------------------------------------------*/
HWND
GetMenuControl(HWND hWnd)
{
    PNHMenuWindow data;

    data = (PNHMenuWindow) GetWindowLongPtr(hWnd, GWLP_USERDATA);

    /* We may continue getting window messages after a window's WM_DESTROY is
       called.  We need to handle the case that USERDATA has been freed. */
    if (data == NULL)
        return NULL;

    if (data->type == MENU_TYPE_TEXT) {
        return GetDlgItem(hWnd, IDC_MENU_TEXT);
    } else {
        return GetDlgItem(hWnd, IDC_MENU_LIST);
    }
}
/*-----------------------------------------------------------------------------*/
BOOL
onMeasureItem(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    LPMEASUREITEMSTRUCT lpmis;
    TEXTMETRIC tm;
    HGDIOBJ saveFont;
    HDC hdc;
    PNHMenuWindow data;
    RECT list_rect;
    int i;

    UNREFERENCED_PARAMETER(wParam);

    lpmis = (LPMEASUREITEMSTRUCT) lParam;
    data = (PNHMenuWindow) GetWindowLongPtr(hWnd, GWLP_USERDATA);
    GetClientRect(GetMenuControl(hWnd), &list_rect);

    hdc = GetDC(GetMenuControl(hWnd));
    cached_font * font = mswin_get_font(NHW_MENU, ATR_INVERSE, hdc, FALSE);
    saveFont = SelectObject(hdc, font->hFont);
    GetTextMetrics(hdc, &tm);

    double monitorScale = win10_monitor_scale(hWnd);
    /* Set the height of the list box items to max height of the individual
     * items */
    for (i = 0; i < data->menu.size; i++)
    {
#if 0
        if (NHMENU_HAS_GLYPH(data->menu.items[i])
            && !IS_MAP_ASCII(iflags.wc_map_mode)) 
        {
            lpmis->itemHeight =
                max(lpmis->itemHeight,
                    (UINT) max(tm.tmHeight + 4, (int)(monitorScale * ((double)MENU_TILE_Y + 2.0)) /*GetNHApp()->mapTile_Y*/));
        } else {
            lpmis->itemHeight =
                max(lpmis->itemHeight, (UINT) max(tm.tmHeight + 4, (int)(monitorScale * ((double)GLYPHLESS_MENU_HEIGHT))));
        }
#endif
        lpmis->itemHeight =
            max(lpmis->itemHeight, (UINT)max(tm.tmHeight + 8, (int)(monitorScale * ((double)GLYPHLESS_MENU_HEIGHT))));
    }

    /* set width to the window width, less scroll width */
    lpmis->itemWidth = list_rect.right - list_rect.left;

    SelectObject(hdc, saveFont);
    ReleaseDC(GetMenuControl(hWnd), hdc);
    return TRUE;
}
/*-----------------------------------------------------------------------------*/
BOOL
onDrawItem(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    LPDRAWITEMSTRUCT lpdis;
    PNHMenuItem item;
    PNHMenuWindow data;
    TEXTMETRIC tm;
    HGDIOBJ saveFont, normalFont;
    HDC tileDC[MAX_TILE_SHEETS] = { 0 };
    int ntile;
    int t_x, t_y;
    int x, y;
    TCHAR wbuf[BUFSZ];
    RECT drawRect;
    COLORREF OldBg, OldFg, NewBg;
    char *p, *p1;
    int column;
    int spacing = 0, BoldAverageCharWidth = 0, BoldOverhang = 0;

    int color, attr;
    double monitorScale = win10_monitor_scale(hWnd);

    UNREFERENCED_PARAMETER(wParam);

    lpdis = (LPDRAWITEMSTRUCT) lParam;

    /* If there are no list box items, skip this message. */
    if (lpdis->itemID == -1)
        return FALSE;

    data = (PNHMenuWindow) GetWindowLongPtr(hWnd, GWLP_USERDATA);

    item = &data->menu.items[lpdis->itemID];
    color = item->color;
    attr = item->attr;

    for(int i = 0; i < GetNHApp()->mapTileSheets; i++)
        tileDC[i] = CreateCompatibleDC(lpdis->hDC);

    cached_font* nfont = mswin_get_font(NHW_MENU, ATR_BOLD, lpdis->hDC, FALSE);
    saveFont = SelectObject(lpdis->hDC, nfont->hFont);

    GetTextMetrics(lpdis->hDC, &tm);
    spacing = tm.tmAveCharWidth;
    BoldAverageCharWidth = tm.tmAveCharWidth;
    BoldOverhang = tm.tmOverhang;

    cached_font* font = mswin_get_font(NHW_MENU, attr, lpdis->hDC, FALSE);
    normalFont = SelectObject(lpdis->hDC, font->hFont);
    NewBg = menu_bg_brush ? menu_bg_color : (COLORREF) GetSysColor(DEFAULT_COLOR_BG_MENU);
    OldBg = SetBkColor(lpdis->hDC, NewBg);
    OldFg = SetTextColor(lpdis->hDC,
                         menu_fg_brush
                             ? menu_fg_color
                             : (COLORREF) GetSysColor(DEFAULT_COLOR_FG_MENU)
    );

    GetTextMetrics(lpdis->hDC, &tm);

    int row_height = lpdis->rcItem.bottom - lpdis->rcItem.top - 2;

    int checkXScaled = (int)(CHECK_WIDTH * monitorScale);
    int checkYScaled = (int)(CHECK_HEIGHT * monitorScale);

    if (checkYScaled > row_height)
    {
        double scaling_factor = (double)row_height / (double)checkYScaled;
        checkXScaled = (int)(scaling_factor * (double)checkXScaled);
        checkYScaled = (int)(scaling_factor * (double)checkYScaled);
    }

    int tileWidth = GetNHApp()->mapTile_X;
    int tileHeight = GetNHApp()->mapTile_Y;
    int menuTileWidth = GetNHApp()->mapTile_X;
    int menuTileHeight = GetNHApp()->mapTile_Y / 2;

    int tileXScaled = (int)(menuTileWidth * monitorScale);
    int tileYScaled = (int)(menuTileHeight * monitorScale);

    if (tileYScaled > row_height)
    {
        double scaling_factor = (double)row_height / (double)tileYScaled;
        tileXScaled = (int)(scaling_factor * (double)tileXScaled);
        tileYScaled = (int)(scaling_factor * (double)tileYScaled);
    }

    /* set initial offset */
    x = lpdis->rcItem.left + 1;

    /* print check mark and letter */
    if (NHMENU_IS_SELECTABLE(*item)) {
        char buf[2];
        if (data->how != PICK_NONE) {
            HBITMAP bmpCheck;
            HBITMAP bmpSaved;

            switch (item->count) {
            case -1:
                bmpCheck = data->bmpChecked;
                break;
            case 0:
                bmpCheck = data->bmpNotChecked;
                break;
            default:
                bmpCheck = data->bmpCheckedCount;
                break;
            }

            y = (lpdis->rcItem.bottom + lpdis->rcItem.top - checkYScaled) / 2;
            bmpSaved = SelectBitmap(data->bmpDC, bmpCheck);
            SetStretchBltMode(lpdis->hDC, COLORONCOLOR);
            StretchBlt(lpdis->hDC, x, y, checkXScaled, checkYScaled,
                data->bmpDC, 0, 0,  CHECK_WIDTH, CHECK_HEIGHT, SRCCOPY);
            SelectObject(data->bmpDC, bmpSaved);
        }

        x += checkXScaled + spacing;

        if (item->accelerator != 0) {
            buf[0] = item->accelerator;
            buf[1] = '\x0';

            if (iflags.use_menu_color && menu_style_allows_menu_coloring(data->style)) {
                (void)get_menu_coloring(item->str, &color, &attr);
            }

            if (attr != ATR_NONE)
            {
                cached_font* menu_font = mswin_get_font(NHW_MENU, attr, lpdis->hDC, FALSE);
                SelectObject(lpdis->hDC, menu_font->hFont);
            }
            if (color != NO_COLOR)
                SetTextColor(lpdis->hDC, nhcolor_to_RGB(color));

            SetRect(&drawRect, x, lpdis->rcItem.top, lpdis->rcItem.right,
                    lpdis->rcItem.bottom);
            DrawText(lpdis->hDC, NH_A2W(buf, wbuf, 2), 1, &drawRect,
                     DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
        }
        x += BoldAverageCharWidth + BoldOverhang + spacing;
    } else {
        x += checkXScaled + BoldAverageCharWidth + BoldOverhang + 2 * spacing;
    }

    /* print glyph if present */
    if (NHMENU_HAS_GLYPH(*item)) 
    {
        if (!IS_MAP_ASCII(iflags.wc_map_mode))
        {
            HGDIOBJ saveBmp[MAX_TILE_SHEETS];
            for(int i = 0; i < GetNHApp()->mapTileSheets; i++)
                saveBmp[i] = SelectObject(tileDC[i], GetNHApp()->bmpMapTiles[i]);

            int signed_glyph = item->glyph;
            int glyph = abs(signed_glyph);
            boolean flip_tile = FALSE;
            if (signed_glyph < 0)
                flip_tile = TRUE;

            boolean is_full_size = !!(glyphtileflags[glyph] & GLYPH_TILE_FLAG_FULL_SIZED_ITEM) || !(glyphtileflags[glyph] & GLYPH_TILE_FLAG_HALF_SIZED_TILE);
            ntile = glyph2tile[glyph];
            enum autodraw_types autodraw = tile2autodraw[ntile];
            int multiplier = flip_tile ? -1 : 1;

            int source_top_added = 0;
            int source_height_deducted = 0;
            int applied_tileXScaled = tileXScaled;
            int x_added = 0;
            int enlargement_idx = tile2enlargement[ntile];

            if (enlargement_idx > 0)
            {
                int enl_height = enlargements[enlargement_idx].height_in_tiles;
                int enl_width = enlargements[enlargement_idx].width_in_tiles;
                int enl_x = enl_width == 3 ? 1 : enlargements[enlargement_idx].main_tile_x_coordinate;

                HDC hDCMem = CreateCompatibleDC(tileDC[0]);

                int width = tileWidth * enl_width;
                int height = tileHeight * enl_height;
                int n_sheet_idx = TILE_SHEET_IDX(ntile);
                t_x = TILEBMP_X(ntile) + (flip_tile ? tileWidth - 1 : 0);
                t_y = TILEBMP_Y(ntile);

                unsigned char* lpBitmapBits;
                BITMAPINFO bi;
                ZeroMemory(&bi, sizeof(BITMAPINFO));
                bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
                bi.bmiHeader.biWidth = width;
                bi.bmiHeader.biHeight = height;
                bi.bmiHeader.biPlanes = 1;
                bi.bmiHeader.biBitCount = 32;

                HBITMAP bitmap = CreateDIBSection(hDCMem, &bi, DIB_RGB_COLORS, (VOID**)&lpBitmapBits, NULL, 0);
                //HBITMAP bitmap = CreateCompatibleBitmap(hDCMem, width, height);
                HGDIOBJ oldbmp = SelectObject(hDCMem, bitmap);

                /* Main tile */
                if(flip_tile)
                    StretchBlt(hDCMem, (enl_width - 1 - enl_x) * tileWidth, tileHeight * (enl_height - 1), tileWidth, tileHeight,
                        tileDC[n_sheet_idx], t_x, t_y, -tileWidth, tileHeight, SRCCOPY);
                else
                    StretchBlt(hDCMem, enl_x * tileWidth, tileHeight * (enl_height - 1), tileWidth, tileHeight,
                        tileDC[n_sheet_idx], t_x, t_y, tileWidth, tileHeight, SRCCOPY);

                /* Enlargement tiles */
                for (int idx = 0; idx < NUM_POSITIONS_IN_ENLARGEMENT; idx++)
                {
                    if (enl_height == 1 && idx < 3)
                        continue;

                    if (enl_width == 2 && enl_x == 0 && (idx == 0 || idx == 3))
                        continue;

                    if (enl_width == 2 && enl_x == 1 && (idx == 2 || idx == 4))
                        continue;

                    char enltile = enlargements[enlargement_idx].position2tile[idx];
                    if (enltile >= 0)
                    {
                        int glyph = enltile + enlargement_offsets[enlargement_idx] /* enlargements[enlargement_idx].glyph_offset */ + GLYPH_ENLARGEMENT_OFF;
                        int etile = glyph2tile[glyph];
                        int e_sheet_idx = TILE_SHEET_IDX(etile);
                        t_x = TILEBMP_X(etile) + (flip_tile ? tileWidth - 1 : 0);
                        t_y = TILEBMP_Y(etile);
                        int target_x = 0;
                        int target_y = 0;

                        if (enl_height == 2)
                        {
                            target_y = idx < 3 ? 0 : tileHeight;
                        }

                        if (enl_width == 2 && enl_x == 0)
                        {
                            target_x = idx == 1 ? 0 : tileWidth;
                        }
                        else if (enl_width == 2 && enl_x == 1)
                        {
                            target_x = idx == 1 ? tileWidth : 0;
                        }
                        else if (enl_width == 3)
                        {
                            target_x = idx == 0 || idx == 3 ? 0 : idx == 1 ? tileWidth : 2 * tileWidth;
                        }

                        if (flip_tile)
                            StretchBlt(hDCMem, (enl_width - 1) * tileWidth - target_x, target_y, tileWidth, tileHeight,
                                tileDC[e_sheet_idx], t_x, t_y, -tileWidth, tileHeight, SRCCOPY);
                        else
                            StretchBlt(hDCMem, target_x, target_y, tileWidth, tileHeight,
                                tileDC[e_sheet_idx], t_x, t_y, tileWidth, tileHeight, SRCCOPY);
                    }
                }

                int applied_height = tileYScaled;
                int applied_width = (width * applied_height) / height;
                if (applied_width > tileXScaled)
                {
                    applied_height = (tileYScaled * tileXScaled) / applied_width;
                    applied_width = tileXScaled;
                }

                y = (lpdis->rcItem.bottom + lpdis->rcItem.top - applied_height) / 2;
                x_added = max(0, (tileXScaled - applied_width) / 2);

                SetStretchBltMode(lpdis->hDC, COLORONCOLOR);
                (*GetNHApp()->lpfnTransparentBlt)(lpdis->hDC, x + x_added, y,
                    applied_width, applied_height,
                    hDCMem, 0, 0, width, height,
                    TILE_BK_COLOR);

                SelectObject(hDCMem, oldbmp);
                DeleteDC(hDCMem);
                DeleteObject(bitmap);
                x += tileXScaled;
            }
            else
            {
                if (is_full_size)
                {
                    applied_tileXScaled = tileXScaled / 2;
                    x_added = (tileXScaled - applied_tileXScaled) / 2;
                }
                else
                {
                    if(item->miflags & MENU_FLAGS_ACTIVE)
                        source_top_added = 0;
                    else
                        source_top_added = tileHeight / 2;
                    source_height_deducted = tileHeight / 2;
                }

                int n_sheet_idx = TILE_SHEET_IDX(ntile);
                t_x = TILEBMP_X(ntile) + (flip_tile ? tileWidth - 1 : 0);
                t_y = TILEBMP_Y(ntile) + source_top_added; /* Use lower part of the tile only */

                y = (lpdis->rcItem.bottom + lpdis->rcItem.top - tileYScaled) / 2;

                SetStretchBltMode(lpdis->hDC, COLORONCOLOR);
                if (GetNHApp()->bmpMapTiles[n_sheet_idx] == GetNHApp()->bmpTiles[n_sheet_idx])
                {
                    /* using original GnollHack tiles - apply image transparently */
                    (*GetNHApp()->lpfnTransparentBlt)(lpdis->hDC, x + x_added, y,
                        applied_tileXScaled, tileYScaled,
                        tileDC[n_sheet_idx], t_x, t_y, multiplier * tileWidth, tileHeight - source_height_deducted, /* Use lower part of the tile only */
                        TILE_BK_COLOR);
                }
                else
                {
                    /* using custom tiles - simple blt */
                    StretchBlt(lpdis->hDC, x, y, tileXScaled, tileYScaled,
                        tileDC[n_sheet_idx], t_x, t_y, multiplier* GetNHApp()->mapTile_X, GetNHApp()->mapTile_Y - source_height_deducted, SRCCOPY);
                }

                double scale_factor = (double)applied_tileXScaled / (double)tileWidth;
                /* Autodraw for candelabrum */
                if (autodraws[autodraw].draw_type == AUTODRAW_DRAW_CANDELABRUM_CANDLES && item->object_data.otyp > STRANGE_OBJECT)
                {
                    int y_start = 0;
                    int x_start = 13;
                    int item_width = 6;
                    int item_height = 13;
                    int src_unlit_x = 0;
                    int src_unlit_y = 10;
                    int src_lit_x = 6 * (1 + (int)autodraws[autodraw].flags);
                    int src_lit_y = 10;
                    int cnt = 0;

                    for (int cidx = 0; cidx < min(7, item->object_data.special_quality); cidx++)
                    {
                        int src_x = 0, src_y = 0;
                        int dest_x = 0, dest_y = 0;
                        if (item->object_data.lamplit)
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
                        dest_x = (int)((double)(x_start + item_xpos * item_width) * scale_factor);

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

                        target_rt.left = x + x_added + dest_x;
                        target_rt.right = target_rt.left + (int)(scale_factor * (double)(source_rt.right - source_rt.left));
                        target_rt.top = y + dest_y;
                        target_rt.bottom = target_rt.top + (int)(scale_factor * (double)(source_rt.bottom - source_rt.top));

                        (*GetNHApp()->lpfnTransparentBlt)(
                            lpdis->hDC, target_rt.left, target_rt.top,
                            target_rt.right - target_rt.left, target_rt.bottom - target_rt.top, tileDC[a_sheet_idx], source_rt.left,
                            source_rt.top, source_rt.right - source_rt.left,
                            source_rt.bottom - source_rt.top, TILE_BK_COLOR);

                        cnt++;
                    }
                }
                else if (autodraws[autodraw].draw_type == AUTODRAW_DRAW_LARGE_FIVE_BRANCHED_CANDELABRUM_CANDLES && item->object_data.otyp > STRANGE_OBJECT)
                {
                    int y_start = 0;
                    int x_start = 0;
                    int x_padding = 13;
                    int item_width = 9;
                    int item_height = 31;
                    int src_unlit_x = 0;
                    int src_unlit_y = 0;
                    int src_lit_x = 9 * (1 + (int)autodraws[autodraw].flags);
                    int src_lit_y = 0;
                    int cnt = 0;

                    for (int cidx = 0; cidx < (int)min(objects[item->object_data.otyp].oc_special_quality, item->object_data.special_quality); cidx++)
                    {
                        int src_x = 0, src_y = 0;
                        int dest_x = 0, dest_y = 0;
                        if (item->object_data.lamplit)
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
                            dest_x = x_start + (int)((double)(29) * scale_factor);
                            dest_y = y_start + (int)((double)(0) * scale_factor);
                            break;
                        case 1:
                            dest_x = x_start + (int)((double)(18) * scale_factor);
                            dest_y = y_start + (int)((double)(4) * scale_factor);
                            break;
                        case 2:
                            dest_x = x_start + (int)((double)(40) * scale_factor);
                            dest_y = y_start + (int)((double)(3) * scale_factor);
                            break;
                        case 3:
                            dest_x = x_start + (int)((double)(8) * scale_factor);
                            dest_y = y_start + (int)((double)(14) * scale_factor);
                            break;
                        case 4:
                            dest_x = x_start + (int)((double)(50) * scale_factor);
                            dest_y = y_start + (int)((double)(15) * scale_factor);
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

                        target_rt.left = x + x_added + dest_x;
                        target_rt.right = target_rt.left + (int)(scale_factor * (double)(source_rt.right - source_rt.left));
                        target_rt.top = y + dest_y;
                        target_rt.bottom = target_rt.top + (int)(scale_factor * (double)(source_rt.bottom - source_rt.top));

                        (*GetNHApp()->lpfnTransparentBlt)(
                            lpdis->hDC, target_rt.left, target_rt.top,
                            target_rt.right - target_rt.left, target_rt.bottom - target_rt.top, tileDC[a_sheet_idx], source_rt.left,
                            source_rt.top, source_rt.right - source_rt.left,
                            source_rt.bottom - source_rt.top, TILE_BK_COLOR);

                        cnt++;
                    }
                }
                else if (autodraws[autodraw].draw_type == AUTODRAW_DRAW_JAR_CONTENTS && item->object_data.otyp > STRANGE_OBJECT && 0)
                {
                    int max_charge = get_obj_max_charge(&item->object_data);
                    double fill_percentage = (max_charge > 0 ? (double)item->object_data.charges / (double)max_charge : 0.0);

                    if (fill_percentage > 0.0)
                    {
                        HDC hDCjar = CreateCompatibleDC(tileDC[0]);

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
                        dest_y = 0;
                        dest_x = x_added;

                        int source_glyph = autodraws[autodraw].source_glyph;
                        int atile = glyph2tile[source_glyph];
                        int a_sheet_idx = TILE_SHEET_IDX(atile);
                        int at_x = TILEBMP_X(atile);
                        int at_y = TILEBMP_Y(atile);

                        int source_glyph2 = autodraws[autodraw].source_glyph2;
                        int atile2 = glyph2tile[source_glyph2];
                        int a2_sheet_idx = TILE_SHEET_IDX(atile2);
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
                                target_rt.right - target_rt.left, target_rt.bottom - target_rt.top, tileDC[a_sheet_idx], source_rt.left,
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

                            HDC hDCtemplate = CreateCompatibleDC(tileDC[0]);

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
                                tileDC[a_sheet_idx], source_rt.left, source_rt.top, width, height, SRCCOPY);

                            /* Color */
                            uint64_t draw_color = autodraws[autodraw].parameter1;
                            unsigned char blue = (&((unsigned char)draw_color))[0];
                            unsigned char green = (&((unsigned char)draw_color))[1];
                            unsigned char red = (&((unsigned char)draw_color))[2];

                            /* Draw */
                            int pitch = 4 * width; // 4 bytes per pixel but if not 32 bit, round pitch up to multiple of 4
                            int idx, x, y;

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
                            HDC hDCMem = CreateCompatibleDC(hDCjar);

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
                                hDCjar, (flip_tile ? jar_width - 1 : 0), 0, multiplier * jar_width, jar_height, SRCCOPY);

                            /* Create copy of tile to be drawn */
                            HDC hDCsemitransparent = CreateCompatibleDC(hDCjar);

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
                                tileDC[a2_sheet_idx], source_rt.left, source_rt.top, source_rt.right - source_rt.left, source_rt.bottom - source_rt.top, SRCCOPY);

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
                                (flip_tile ? tileWidth - 1 : 0), 0, multiplier * width,
                                height, TILE_BK_COLOR);

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
                                target_rt.right - target_rt.left, target_rt.bottom - target_rt.top, tileDC[a2_sheet_idx], source_rt.left,
                                source_rt.top, source_rt.right - source_rt.left,
                                source_rt.bottom - source_rt.top, TILE_BK_COLOR);
                        }


                        /* Finally, print jar to the menu */
                        target_rt.left = x + dest_x;
                        target_rt.right = target_rt.left + (int)(scale_factor * (double)(jar_width));
                        target_rt.top = y + dest_y;
                        target_rt.bottom = target_rt.top + (int)(scale_factor * (double)(jar_height));

                        (*GetNHApp()->lpfnTransparentBlt)(
                            lpdis->hDC, target_rt.left, target_rt.top,
                            target_rt.right - target_rt.left, target_rt.bottom - target_rt.top, 
                            hDCjar, 0, 0, jar_width, jar_height, TILE_BK_COLOR);

                        SelectObject(hDCjar, oldbmp_jar);
                        DeleteDC(hDCjar);
                        DeleteObject(newhBmp_jar);

                    }
                }

                /* Item property marks */
                if (item->object_data.opoisoned || item->object_data.elemental_enchantment > 0 || item->object_data.oeroded || item->object_data.oeroded2 
                    || item->object_data.exceptionality > 0 || item->object_data.mythic_prefix > 0 || item->object_data.mythic_suffix > 0)
                {
                    int y_start = 0;
                    int x_start = 0;
                    int mark_width = 8;
                    int marks_per_row = TILE_X / mark_width;
                    int mark_height = 24;
                    int y_padding = max(0, (tileYScaled - mark_height) / 2);
                    int mark_scaled_height = min(tileYScaled, mark_height);
                    double mark_scale_factor = (double)mark_scaled_height / (double)mark_height;
                    int mark_scaled_width = min(1, (int)((double)mark_width * mark_scale_factor));
                    int max_marks = (int)((double)applied_tileXScaled / (double)mark_scaled_width);
                    int src_x = 0;
                    int src_y = 0;
                    int cnt = 0;
                    int poisoned = (item->object_data.opoisoned);
                    int elemental_enchantment = (item->object_data.elemental_enchantment);
                    int exceptionality = (item->object_data.exceptionality);
                    int mythic_prefix = (item->object_data.mythic_prefix);
                    int mythic_suffix = (item->object_data.mythic_suffix);
                    int eroded = (item->object_data.oeroded);
                    int eroded2 = (item->object_data.oeroded2);

                    for (enum item_property_mark_types ipm_idx = 0; ipm_idx < MAX_ITEM_PROPERTY_MARKS; ipm_idx++)
                    {
                        if (cnt >= max_marks)
                            break;

                        int src_tile = ITEM_PROPERTY_MARKS + (ipm_idx / MAX_UI_TILE_8_x_24_COMPONENTS);
                        int src_x = ((ipm_idx % MAX_UI_TILE_8_x_24_COMPONENTS) % marks_per_row) * mark_width, src_y = ((ipm_idx % MAX_UI_TILE_8_x_24_COMPONENTS) / marks_per_row) * mark_height;
                        int dest_x = 0, dest_y = 0;

                        switch (ipm_idx)
                        {
                        case ITEM_PROPERTY_MARK_POISONED:
                            if (!(poisoned && is_poisonable(&item->object_data)))
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
                            if (!(eroded2 == 1 && is_corrodeable(&item->object_data)))
                                continue;
                            break;
                        case ITEM_PROPERTY_MARK_ROTTED:
                            if (!(eroded2 == 1 && is_rottable(&item->object_data)))
                                continue;
                            break;
                        case ITEM_PROPERTY_MARK_BURNT:
                            if (!(eroded == 1 && is_flammable(&item->object_data)))
                                continue;
                            break;
                        case ITEM_PROPERTY_MARK_RUSTY:
                            if (!(eroded == 1 && is_rustprone(&item->object_data)))
                                continue;
                            break;
                        case ITEM_PROPERTY_MARK_VERY_CORRODED:
                            if (!(eroded2 == 2 && is_corrodeable(&item->object_data)))
                                continue;
                            break;
                        case ITEM_PROPERTY_MARK_VERY_ROTTED:
                            if (!(eroded2 == 2 && is_rottable(&item->object_data)))
                                continue;
                            break;
                        case ITEM_PROPERTY_MARK_VERY_BURNT:
                            if (!(eroded == 2 && is_flammable(&item->object_data)))
                                continue;
                            break;
                        case ITEM_PROPERTY_MARK_VERY_RUSTY:
                            if (!(eroded == 2 && is_rustprone(&item->object_data)))
                                continue;
                            break;
                        case ITEM_PROPERTY_MARK_THOROUGHLY_CORRODED:
                            if (!(eroded2 == 3 && is_corrodeable(&item->object_data)))
                                continue;
                            break;
                        case ITEM_PROPERTY_MARK_THOROUGHLY_ROTTED:
                            if (!(eroded2 == 3 && is_rottable(&item->object_data)))
                                continue;
                            break;
                        case ITEM_PROPERTY_MARK_THOROUGHLY_BURNT:
                            if (!(eroded == 3 && is_flammable(&item->object_data)))
                                continue;
                            break;
                        case ITEM_PROPERTY_MARK_THOROUGHLY_RUSTY:
                            if (!(eroded == 3 && is_rustprone(&item->object_data)))
                                continue;
                            break;
                        case MAX_ITEM_PROPERTY_MARKS:
                        default:
                            continue;
                            break;
                        }

                        int item_xpos = ((int)tileWidth) / 2 - mark_width + (cnt % 2 ? 1 : -1) * ((cnt + 1) / 2) * mark_width;

                        dest_y = y_start + (int)((double)(y_padding) * mark_scale_factor);
                        dest_x = x_start + (int)((double)item_xpos * scale_factor);

                        int source_glyph = src_tile + GLYPH_UI_TILE_OFF;
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

                        target_rt.left = x + x_added + dest_x;
                        target_rt.right = target_rt.left + (int)(mark_scale_factor * (double)(source_rt.right - source_rt.left));
                        target_rt.top = y + dest_y;
                        target_rt.bottom = target_rt.top + (int)(mark_scale_factor * (double)(source_rt.bottom - source_rt.top));


                        (*GetNHApp()->lpfnTransparentBlt)(
                            lpdis->hDC, target_rt.left, target_rt.top,
                            target_rt.right - target_rt.left, target_rt.bottom - target_rt.top, tileDC[a_sheet_idx], source_rt.left,
                            source_rt.top, source_rt.right - source_rt.left,
                            source_rt.bottom - source_rt.top, TILE_BK_COLOR);

                        cnt++;
                    }
                }
                
                for(int i = 0; i < GetNHApp()->mapTileSheets; i++)
                    SelectObject(tileDC[i], saveBmp[i]);

                x += tileXScaled;
            }
        } 
        else 
        {
            const char *sel_ind;
            switch (item->count) 
            {
            case -1:
                sel_ind = "+";
                break;
            case 0:
                sel_ind = "-";
                break;
            default:
                sel_ind = "#";
                break;
            }

            SetRect(&drawRect, x, lpdis->rcItem.top,
                    min(x + BoldAverageCharWidth, lpdis->rcItem.right),
                    lpdis->rcItem.bottom);
            DrawText(lpdis->hDC, NH_A2W(sel_ind, wbuf, BUFSZ), 1, &drawRect,
                     DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            x += BoldAverageCharWidth;
        }
    }
    else 
    {
        /* no glyph - need to adjust so help window won't look to cramped */
        if (!IS_MAP_ASCII(iflags.wc_map_mode)) 
            x += tileXScaled;
        else
            x += BoldAverageCharWidth;
    }

    x += spacing;

    /* draw item text */
    p1 = item->str;
    p = strchr(item->str, '\t');
    column = 0;
    SetRect(&drawRect, x, lpdis->rcItem.top,
            item->attr & ATR_NOTABS ? lpdis->rcItem.right : min(x + data->menu.tab_stop_size[0], lpdis->rcItem.right),
            lpdis->rcItem.bottom);

    for (;;)
    {
        TCHAR wbuf[BUFSZ];
        if (p != NULL)
            *p = '\0'; /* for time being, view tab field as zstring */

        DrawText(lpdis->hDC, NH_A2W(p1, wbuf, BUFSZ), strlen(p1), &drawRect,
                 DT_LEFT | DT_VCENTER | DT_SINGLELINE);

        //DrawTextToRectangle(lpdis->hDC, p1, &drawRect, font->font_name, font->size, font->font_attributes, nhcolor_to_RGB(color), monitorScale);

        if (p != NULL)
            *p = '\t';
        else /* last string so, */
            break;

        p1 = p + 1;
        p = strchr(p1, '\t');
        drawRect.left = drawRect.right + TAB_SEPARATION;
        ++column;
        drawRect.right = min(drawRect.left + data->menu.tab_stop_size[column],
                             lpdis->rcItem.right);
    }

    /* draw focused item */
    //if (item->has_focus || (NHMENU_IS_SELECTABLE(*item) && data->menu.items[lpdis->itemID].count != -1)) //Always true
    {
        RECT client_rt;

        GetClientRect(lpdis->hwndItem, &client_rt);
        client_rt.right = min(client_rt.right, lpdis->rcItem.right);
        if (NHMENU_IS_SELECTABLE(*item)
            && data->menu.items[lpdis->itemID].count != 0
            && data->menu.items[lpdis->itemID].object_data.quan > 1
            && item->glyph != NO_GLYPH)
        {
            if (data->menu.items[lpdis->itemID].count == -1) 
            {
                _stprintf(wbuf, TEXT("Count: All"));
            } 
            else 
            {
                _stprintf(wbuf, TEXT("Count: %d"),
                          data->menu.items[lpdis->itemID].count);
            }

            /* TOOD: add blinking for blink text */

            cached_font * blink_font = mswin_get_font(NHW_MENU, ATR_BLINK, lpdis->hDC, FALSE);
            SelectObject(lpdis->hDC, blink_font->hFont);

            /* calculate text rectangle */
            SetRect(&drawRect, client_rt.left, lpdis->rcItem.top,
                    client_rt.right, lpdis->rcItem.bottom);
            DrawText(lpdis->hDC, wbuf, _tcslen(wbuf), &drawRect,
                     DT_CALCRECT | DT_RIGHT | DT_VCENTER | DT_SINGLELINE
                         | DT_NOPREFIX);

            /* erase text rectangle */
            drawRect.left =
                max(client_rt.left + 1,
                    client_rt.right - (drawRect.right - drawRect.left) - 10);
            drawRect.right = client_rt.right - 1;
            drawRect.top = lpdis->rcItem.top;
            drawRect.bottom = lpdis->rcItem.bottom;
            FillRect(lpdis->hDC, &drawRect,
                     menu_bg_brush ? menu_bg_brush
                                   : SYSCLR_TO_BRUSH(DEFAULT_COLOR_BG_MENU));

            /* draw text */
            DrawText(lpdis->hDC, wbuf, _tcslen(wbuf), &drawRect,
                     DT_RIGHT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
        }
    }
    
    if (item->has_focus) 
    {
        /* draw focus rect */
        RECT client_rt;

        GetClientRect(lpdis->hwndItem, &client_rt);
        SetRect(&drawRect, client_rt.left, lpdis->rcItem.top,
                client_rt.left + ListView_GetColumnWidth(lpdis->hwndItem, 0),
                lpdis->rcItem.bottom);
        DrawFocusRect(lpdis->hDC, &drawRect);
    }

    SetTextColor(lpdis->hDC, OldFg);
    SetBkColor(lpdis->hDC, OldBg);
    SelectObject(lpdis->hDC, saveFont);
    for(int i = 0; i < GetNHApp()->mapTileSheets; i++)
        DeleteDC(tileDC[i]);

    return TRUE;
}
/*-----------------------------------------------------------------------------*/
static int curcount = 0;

BOOL
onListChar(HWND hWnd, HWND hwndList, WORD ch)
{
    int i = 0;
    PNHMenuWindow data;
    int curIndex, topIndex, pageSize;
    boolean is_accelerator = FALSE;

    data = (PNHMenuWindow) GetWindowLongPtr(hWnd, GWLP_USERDATA);

    is_accelerator = FALSE;
    for (i = 0; i < data->menu.size; i++) {
        if (data->menu.items[i].accelerator == ch) {
            is_accelerator = TRUE;
            break;
        }
    }

    /* Don't use switch if input matched an accelerator.  Sometimes
     * accelerators can conflict with menu actions.  For example, when
     * engraving the extra choice of using fingers matches MENU_UNSELECT_ALL.
     */
    if (is_accelerator)
        goto accelerator;

    switch (ch) {
    case MENU_FIRST_PAGE:
        i = 0;
        ListView_SetItemState(hwndList, i, LVIS_FOCUSED, LVIS_FOCUSED);
        ListView_EnsureVisible(hwndList, i, FALSE);
        return -2;

    case MENU_LAST_PAGE:
        i = max(0, data->menu.size - 1);
        ListView_SetItemState(hwndList, i, LVIS_FOCUSED, LVIS_FOCUSED);
        ListView_EnsureVisible(hwndList, i, FALSE);
        return -2;

    case MENU_NEXT_PAGE:
        topIndex = ListView_GetTopIndex(hwndList);
        pageSize = ListView_GetCountPerPage(hwndList);
        curIndex = ListView_GetNextItem(hwndList, -1, LVNI_FOCUSED);
        /* Focus down one page */
        i = min(curIndex + pageSize, data->menu.size - 1);
        ListView_SetItemState(hwndList, i, LVIS_FOCUSED, LVIS_FOCUSED);
        /* Scrollpos down one page */
        i = min(topIndex + (2 * pageSize - 1), data->menu.size - 1);
        ListView_EnsureVisible(hwndList, i, FALSE);
        return -2;

    case MENU_PREVIOUS_PAGE:
        topIndex = ListView_GetTopIndex(hwndList);
        pageSize = ListView_GetCountPerPage(hwndList);
        curIndex = ListView_GetNextItem(hwndList, -1, LVNI_FOCUSED);
        /* Focus up one page */
        i = max(curIndex - pageSize, 0);
        ListView_SetItemState(hwndList, i, LVIS_FOCUSED, LVIS_FOCUSED);
        /* Scrollpos up one page */
        i = max(topIndex - pageSize, 0);
        ListView_EnsureVisible(hwndList, i, FALSE);
        break;

    case MENU_SELECT_ALL:
        if (data->how == PICK_ANY) {
            reset_menu_count(hwndList, data);
            for (i = 0; i < data->menu.size; i++) {
                SelectMenuItem(hwndList, data, i, -1);
            }
            return -2;
        }
        break;

    case MENU_UNSELECT_ALL:
        if (data->how == PICK_ANY) {
            reset_menu_count(hwndList, data);
            for (i = 0; i < data->menu.size; i++) {
                SelectMenuItem(hwndList, data, i, 0);
            }
            return -2;
        }
        break;

    case MENU_INVERT_ALL:
        if (data->how == PICK_ANY) {
            reset_menu_count(hwndList, data);
            for (i = 0; i < data->menu.size; i++) {
                SelectMenuItem(hwndList, data, i,
                               NHMENU_IS_SELECTED(data->menu.items[i]) ? 0
                                                                       : -1);
            }
            return -2;
        }
        break;

    case MENU_SELECT_PAGE:
        if (data->how == PICK_ANY) {
            int from, to;
            reset_menu_count(hwndList, data);
            topIndex = ListView_GetTopIndex(hwndList);
            pageSize = ListView_GetCountPerPage(hwndList);
            from = max(0, topIndex);
            to = min(data->menu.size, from + pageSize);
            for (i = from; i < to; i++) {
                SelectMenuItem(hwndList, data, i, -1);
            }
            return -2;
        }
        break;

    case MENU_UNSELECT_PAGE:
        if (data->how == PICK_ANY) {
            int from, to;
            reset_menu_count(hwndList, data);
            topIndex = ListView_GetTopIndex(hwndList);
            pageSize = ListView_GetCountPerPage(hwndList);
            from = max(0, topIndex);
            to = min(data->menu.size, from + pageSize);
            for (i = from; i < to; i++) {
                SelectMenuItem(hwndList, data, i, 0);
            }
            return -2;
        }
        break;

    case MENU_INVERT_PAGE:
        if (data->how == PICK_ANY) {
            int from, to;
            reset_menu_count(hwndList, data);
            topIndex = ListView_GetTopIndex(hwndList);
            pageSize = ListView_GetCountPerPage(hwndList);
            from = max(0, topIndex);
            to = min(data->menu.size, from + pageSize);
            for (i = from; i < to; i++) {
                SelectMenuItem(hwndList, data, i,
                               NHMENU_IS_SELECTED(data->menu.items[i]) ? 0
                                                                       : -1);
            }
            return -2;
        }
        break;

    case MENU_SEARCH:
        if (data->how == PICK_ANY || data->how == PICK_ONE) {
            char buf[BUFSZ];

            reset_menu_count(hwndList, data);
            if (mswin_getlin_window(GETLINE_MENU_SEARCH, ATR_NONE, NO_COLOR, "Search for:", buf, BUFSZ) == IDCANCEL) {
                Strcpy(buf, "\033");
            }
            if (data->is_active)
                SetFocus(hwndList); // set focus back to the list control
            if (!*buf || *buf == '\033')
                return -2;
            for (i = 0; i < data->menu.size; i++) {
                if (NHMENU_IS_SELECTABLE(data->menu.items[i])
                    && strstr(data->menu.items[i].str, buf)) {
                    if (data->how == PICK_ANY) {
                        SelectMenuItem(
                            hwndList, data, i,
                            NHMENU_IS_SELECTED(data->menu.items[i]) ? 0 : -1);
                    } else if (data->how == PICK_ONE) {
                        SelectMenuItem(hwndList, data, i, -1);
                        ListView_SetItemState(hwndList, i, LVIS_FOCUSED,
                                              LVIS_FOCUSED);
                        ListView_EnsureVisible(hwndList, i, FALSE);
                        break;
                    }
                }
            }
        } else {
            mswin_nhbell();
        }
        return -2;

    case ' ': {
        if (GetNHApp()->regGnollHackMode) {
            /* GnollHack mode: Scroll down one page,
               ends menu when on last page. */
            SCROLLINFO si;

            si.cbSize = sizeof(SCROLLINFO);
            si.fMask = SIF_POS | SIF_RANGE | SIF_PAGE;
            GetScrollInfo(hwndList, SB_VERT, &si);
            if ((si.nPos + (int) si.nPage) > (si.nMax - si.nMin)) {
                /* We're at the bottom: dismiss. */
                data->done = 1;
                data->result = 0;
                return -2;
            }
            /* We're not at the bottom: page down. */
            topIndex = ListView_GetTopIndex(hwndList);
            pageSize = ListView_GetCountPerPage(hwndList);
            curIndex = ListView_GetNextItem(hwndList, -1, LVNI_FOCUSED);
            /* Focus down one page */
            i = min(curIndex + pageSize, data->menu.size - 1);
            ListView_SetItemState(hwndList, i, LVIS_FOCUSED, LVIS_FOCUSED);
            /* Scrollpos down one page */
            i = min(topIndex + (2 * pageSize - 1), data->menu.size - 1);
            ListView_EnsureVisible(hwndList, i, FALSE);

            return -2;
        } else {
            /* Windows mode: ends menu for PICK_ONE/PICK_NONE
               select item for PICK_ANY */
            if (data->how == PICK_ONE || data->how == PICK_NONE) {
                data->done = 1;
                data->result = 0;
                return -2;
            } else if (data->how == PICK_ANY) {
                i = ListView_GetNextItem(hwndList, -1, LVNI_FOCUSED);
                if (i >= 0) {
                    SelectMenuItem(
                        hwndList, data, i,
                        NHMENU_IS_SELECTED(data->menu.items[i]) ? 0 : -1);
                }
            }
        }
    } break;

    accelerator:
    default:
        if (strchr(data->menu.gacc, ch)
            && !((ch == '0' || ch == '9' || ch == '8' || ch == '7' || ch == GOLD_SYM || ch == GOLD_SYM_ALTERNATE) && data->menu.counting))
        {
            /* matched a group accelerator */
            if (data->how == PICK_ANY || data->how == PICK_ONE) 
            {
                reset_menu_count(hwndList, data);
                for (i = 0; i < data->menu.size; i++) 
                {
                    if (NHMENU_IS_SELECTABLE(data->menu.items[i]) && data->menu.items[i].group_accel == ch)
                    {
                        if (data->how == PICK_ANY) 
                        {
                            SelectMenuItem(hwndList, data, i, NHMENU_IS_SELECTED(data->menu.items[i]) ? 0 : -1);
                        }
                        else if (data->how == PICK_ONE) 
                        {
                            SelectMenuItem(hwndList, data, i, -1);
                            data->result = 0;
                            data->done = 1;
                            return -2;
                        }
                    }
                }
                return -2;
            } 
            else
            {
                mswin_nhbell();
                return -2;
            }
        }

        if (isdigit((uchar) ch) && (data->menu.counting || !is_accelerator))
        {
            int count;
            i = ListView_GetNextItem(hwndList, -1, LVNI_FOCUSED);
#if 0
            if (i >= 0) //This system has been deactivated
            {
                curcount = 0;
                count = data->menu.items[i].count;
                if (count == -1)
                    count = 0;
                count *= 10L;
                count += (int) (ch - '0');
                if (count != 0) /* ignore leading zeros */ 
                {
                    data->menu.counting = TRUE;
                    data->menu.items[i].count = min(100000, count);
                    ListView_RedrawItems(hwndList, i,
                                         i); /* update count mark */
                }
            }
            else
#endif
            {
                count = curcount;
                if (count == -1)
                    count = 0;
                count *= 10L;
                count += (int)(ch - '0');
                if (count != 0) /* ignore leading zeros */
                {
                    data->menu.counting = TRUE;
                    curcount = min(1000000, count);
                }
            }
            return -2;
        }

        if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || is_accelerator) 
        {
            if (data->how == PICK_ANY || data->how == PICK_ONE)
            {
                topIndex = ListView_GetTopIndex(hwndList);
                if (topIndex < 0 || topIndex > data->menu.size) 
                    break; // impossible?

                int iter = topIndex;
                do
                {
                    i = iter % data->menu.size;
                    if (data->menu.items[i].accelerator == ch)
                    {
                        if (data->menu.items[i].object_data.quan > 0 && curcount >= data->menu.items[i].object_data.quan)
                            curcount = -1;

                        if (data->how == PICK_ANY)
                        {
                            SelectMenuItem(hwndList, data, i, curcount != 0 ? curcount : NHMENU_IS_SELECTED(data->menu.items[i]) ? 0 : -1);
                            ListView_SetItemState(hwndList, i, LVIS_FOCUSED, LVIS_FOCUSED);
                            ListView_EnsureVisible(hwndList, i, FALSE);
                            return -2;
                        }
                        else if (data->how == PICK_ONE) 
                        {
                            
                            SelectMenuItem(hwndList, data, i, curcount != 0 ? curcount : -1);
                            data->result = 0;
                            data->done = 1;
                            return -2;
                        }
                    }
                } while( (++iter % data->menu.size) != topIndex ); 
                curcount = 0;
            }
        }
        break;
    }

    reset_menu_count(hwndList, data);
    return -1;
}
/*-----------------------------------------------------------------------------*/
void
mswin_menu_window_size(HWND hWnd, LPSIZE sz)
{
    HWND control;
    PNHMenuWindow data;
    RECT rt, wrt;
    int extra_cx;

    data = (PNHMenuWindow) GetWindowLongPtr(hWnd, GWLP_USERDATA);
    if (data) {
        control = GetMenuControl(hWnd);

        /* get the control size */
        GetClientRect(control, &rt);
        sz->cx = rt.right - rt.left;
        sz->cy = rt.bottom - rt.top;

        /* calculate "extra" space around the control */
        GetWindowRect(hWnd, &wrt);
        extra_cx = (wrt.right - wrt.left) - sz->cx;

        if (data->type == MENU_TYPE_MENU) {
            sz->cx = data->menu.menu_cx + GetSystemMetrics(SM_CXVSCROLL);
        } else {
            /* Use the width of the text box */
            sz->cx = data->text.text_box_size.cx
                     + 2 * GetSystemMetrics(SM_CXVSCROLL);
        }
        sz->cx += extra_cx;
    } else {
        /* uninitilized window */
        GetClientRect(hWnd, &rt);
        sz->cx = rt.right - rt.left;
        sz->cy = rt.bottom - rt.top;
    }
}
/*-----------------------------------------------------------------------------*/
void
SelectMenuItem(HWND hwndList, PNHMenuWindow data, int item, int count)
{
    int i;

    if (item < 0 || item >= data->menu.size)
        return;

    if (data->how == PICK_ONE && count != 0) 
    {
        for (i = 0; i < data->menu.size; i++)
            if (item != i && data->menu.items[i].count != 0) 
            {
                data->menu.items[i].count = 0;
                ListView_RedrawItems(hwndList, i, i);
            };
    }

    data->menu.items[item].count = count;
    ListView_RedrawItems(hwndList, item, item);
    reset_menu_count(hwndList, data);
}
/*-----------------------------------------------------------------------------*/
void
reset_menu_count(HWND hwndList, PNHMenuWindow data)
{
    int i;
    data->menu.counting = FALSE;
    curcount = 0;
    if (IsWindow(hwndList)) {
        i = ListView_GetNextItem((hwndList), -1, LVNI_FOCUSED);
        if (i >= 0)
            ListView_RedrawItems(hwndList, i, i);
    }
}
/*-----------------------------------------------------------------------------*/
/* List window Proc */
LRESULT CALLBACK
NHMenuListWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HWND hWndParent = GetParent(hWnd);
    BOOL bUpdateFocusItem;

    /* we will redraw focused item whenever horizontal scrolling occurs
       since "Count: XXX" indicator is garbled by scrolling */
    bUpdateFocusItem = FALSE;

    switch (message) {
    case WM_KEYDOWN:
        if (wParam == VK_LEFT || wParam == VK_RIGHT)
            bUpdateFocusItem = TRUE;
        break;

    case WM_CHAR: /* filter keyboard input for the control */
        if (wParam > 0 && wParam < 256
            && onListChar(GetParent(hWnd), hWnd, (char) wParam) == -2) {
            return 0;
        } else {
            return 1;
        }
        break;

    case WM_SIZE:
    case WM_HSCROLL:
        bUpdateFocusItem = TRUE;
        break;

    case WM_SETFOCUS:
        if (GetParent(hWnd) != GetNHApp()->hPopupWnd) {
            SetFocus(GetNHApp()->hMainWnd);
        }
        return FALSE;

    case WM_MSNH_COMMAND:
        if (wParam == MSNH_MSG_RANDOM_INPUT) {
            char c = randomkey();
            if (c == '\n')
                PostMessage(hWndParent, WM_COMMAND, MAKELONG(IDOK, 0), 0);
            else if (c == '\033')
                PostMessage(hWndParent, WM_COMMAND, MAKELONG(IDCANCEL, 0), 0);
            else
                PostMessage(hWnd, WM_CHAR, c, 0);
            return 0;
        }
        break;

    }

    /* update focused item */
    if (bUpdateFocusItem) {
        int i;
        RECT rt;

        /* invalidate the focus rectangle */
        i = ListView_GetNextItem(hWnd, -1, LVNI_FOCUSED);
        if (i != -1) {
            ListView_GetItemRect(hWnd, i, &rt, LVIR_BOUNDS);
            InvalidateRect(hWnd, &rt, TRUE);
        }
    }

    /* call ListView control window proc */
    if (wndProcListViewOrig)
        return CallWindowProc(wndProcListViewOrig, hWnd, message, wParam,
                              lParam);
    else
        return 0;
}
/*-----------------------------------------------------------------------------*/
/* Text control window proc - implements scrolling without a cursor */
LRESULT CALLBACK
NHMenuTextWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HWND hWndParent = GetParent(hWnd);
    HDC hDC;
    RECT rc;

    switch (message) {
    case WM_ERASEBKGND:
        hDC = (HDC) wParam;
        GetClientRect(hWnd, &rc);
        FillRect(hDC, &rc, text_bg_brush
                               ? text_bg_brush
                               : SYSCLR_TO_BRUSH(DEFAULT_COLOR_BG_TEXT));
        return 0;

    case WM_KEYDOWN:
        switch (wParam) {
        /* close on space in Windows mode
           page down on space in GnollHack mode */
        case VK_SPACE: {
            SCROLLINFO si;

            si.cbSize = sizeof(SCROLLINFO);
            si.fMask = SIF_POS | SIF_RANGE | SIF_PAGE;
            GetScrollInfo(hWnd, SB_VERT, &si);
            /* If GnollHackmode and not at the end of the list */
            if (GetNHApp()->regGnollHackMode
                && (si.nPos + (int) si.nPage) <= (si.nMax - si.nMin))
                SendMessage(hWnd, EM_SCROLL, SB_PAGEDOWN, 0);
            else
                PostMessage(hWndParent, WM_COMMAND, MAKELONG(IDOK, 0), 0);
            return 0;
        }
        case VK_NEXT:
            SendMessage(hWnd, EM_SCROLL, SB_PAGEDOWN, 0);
            return 0;
        case VK_PRIOR:
            SendMessage(hWnd, EM_SCROLL, SB_PAGEUP, 0);
            return 0;
        case VK_UP:
            SendMessage(hWnd, EM_SCROLL, SB_LINEUP, 0);
            return 0;
        case VK_DOWN:
            SendMessage(hWnd, EM_SCROLL, SB_LINEDOWN, 0);
            return 0;
        }
        break;

    case WM_CHAR:
        switch(wParam) {
        case MENU_FIRST_PAGE:
            SendMessage(hWnd, EM_SCROLL, SB_TOP, 0);
            return 0;
        case MENU_LAST_PAGE:
            SendMessage(hWnd, EM_SCROLL, SB_BOTTOM, 0);
            return 0;
        case MENU_NEXT_PAGE:
            SendMessage(hWnd, EM_SCROLL, SB_PAGEDOWN, 0);
            return 0;
        case MENU_PREVIOUS_PAGE:
            SendMessage(hWnd, EM_SCROLL, SB_PAGEUP, 0);
            return 0;
        }
        break;

    /* edit control needs to know nothing of its focus */
    case WM_SETFOCUS:
        HideCaret(hWnd);
        return 0;

    case WM_MSNH_COMMAND:
        if (wParam == MSNH_MSG_RANDOM_INPUT) {
            char c = randomkey();
            if (c == '\n')
                PostMessage(hWndParent, WM_COMMAND, MAKELONG(IDOK, 0), 0);
            else if (c == '\033')
                PostMessage(hWndParent, WM_COMMAND, MAKELONG(IDCANCEL, 0), 0);
            else
                PostMessage(hWnd, WM_CHAR, c, 0);
            return 0;
        }
        break;

    }

    if (editControlWndProc)
        return CallWindowProc(editControlWndProc, hWnd, message, wParam,
                              lParam);
    else
        return 0;
}
/*-----------------------------------------------------------------------------*/
