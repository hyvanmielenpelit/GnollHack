/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-01-06 */

/* GnollHack 4.0    topl.c    $NHDT-Date: 1549333449 2019/02/05 02:24:09 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.44 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Michael Allison, 2009. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"

#ifdef TTY_GRAPHICS

#include "tcap.h"
#include "wintty.h"

#ifndef C /* this matches src/cmd.c */
#define C(c) (0x40 & (c) ? 0x1f & (c) : (0x80 | (0x1f & (c))))
#endif

STATIC_DCL void FDECL(redotoplin, (const char *, const char*, const char*));
STATIC_DCL void FDECL(topl_putsym, (int));
STATIC_DCL void FDECL(removetopl, (int));
STATIC_DCL void FDECL(msghistory_snapshot, (BOOLEAN_P));
STATIC_DCL void FDECL(free_msghistory_snapshot, (BOOLEAN_P));
STATIC_DCL void FDECL(toggle_topl_attr, (BOOLEAN_P, int, int));

/* support for topline colors */
STATIC_OVL void
toggle_topl_attr(on, attr, color)
boolean on;
int attr, color;
{
    if (on) {
        if (attr != ATR_NONE)
            term_start_attr(attr);
#ifdef TEXTCOLOR
        if (color != NO_COLOR)
            term_start_color(color);
#endif
    }
    else {
#ifdef TEXTCOLOR
        if (color != NO_COLOR)
            term_end_color();
#endif
        if (attr != ATR_NONE)
            term_end_attr(attr);
    }

#ifndef TEXTCOLOR
    nhUse(color);
#endif
}

int
tty_doprev_message()
{
    register struct WinDesc *cw = wins[WIN_MESSAGE];
    winid prevmsg_win;
    int i;

    if ((iflags.prevmsg_window != 's')
        && !ttyDisplay->inread) {           /* not single */
        if (iflags.prevmsg_window == 'f') { /* full */
            prevmsg_win = create_nhwindow(NHW_MENU);
            putstr(prevmsg_win, 0, "Message History");
            putstr(prevmsg_win, 0, "");
            cw->maxcol = cw->maxrow;
            i = cw->maxcol;
            do {
                if (cw->data[i] && strcmp(cw->data[i], ""))
                    putstr_ex2(prevmsg_win, cw->data[i], cw->datattrs[i], cw->datcolors[i], 0);
                i = (i + 1) % cw->rows;
            } while (i != cw->maxcol);
            putstr_ex2(prevmsg_win, toplines, toplineattrs, toplinecolors, 0);
            display_nhwindow(prevmsg_win, TRUE);
            destroy_nhwindow(prevmsg_win);
        } else if (iflags.prevmsg_window == 'c') { /* combination */
            do {
                morc = 0;
                if (cw->maxcol == cw->maxrow) {
                    ttyDisplay->dismiss_more = C('p'); /* ^P ok at --More-- */
                    redotoplin(toplines, toplineattrs, toplinecolors);
                    cw->maxcol--;
                    if (cw->maxcol < 0)
                        cw->maxcol = cw->rows - 1;
                    if (!cw->data[cw->maxcol])
                        cw->maxcol = cw->maxrow;
                } else if (cw->maxcol == (cw->maxrow - 1)) {
                    ttyDisplay->dismiss_more = C('p'); /* ^P ok at --More-- */
                    redotoplin(cw->data[cw->maxcol], cw->datattrs[cw->maxcol], cw->datcolors[cw->maxcol]);
                    cw->maxcol--;
                    if (cw->maxcol < 0)
                        cw->maxcol = cw->rows - 1;
                    if (!cw->data[cw->maxcol])
                        cw->maxcol = cw->maxrow;
                } else {
                    prevmsg_win = create_nhwindow(NHW_MENU);
                    putstr(prevmsg_win, 0, "Message History");
                    putstr(prevmsg_win, 0, "");
                    cw->maxcol = cw->maxrow;
                    i = cw->maxcol;
                    do {
                        if (cw->data[i] && strcmp(cw->data[i], ""))
                            putstr_ex2(prevmsg_win, cw->data[i], cw->datattrs[i], cw->datcolors[i], 0);
                        i = (i + 1) % cw->rows;
                    } while (i != cw->maxcol);
                    /* Do recursion here for colors */
                    putstr_ex2(prevmsg_win, toplines, toplineattrs, toplinecolors, 0);
                    display_nhwindow(prevmsg_win, TRUE);
                    destroy_nhwindow(prevmsg_win);
                }

            } while (morc == C('p'));
            ttyDisplay->dismiss_more = 0;
        } else { /* reversed */
            morc = 0;
            prevmsg_win = create_nhwindow(NHW_MENU);
            putstr(prevmsg_win, 0, "Message History");
            putstr(prevmsg_win, 0, "");
            putstr(prevmsg_win, 0, toplines);
            cw->maxcol = cw->maxrow - 1;
            if (cw->maxcol < 0)
                cw->maxcol = cw->rows - 1;
            do {
                putstr_ex2(prevmsg_win, cw->data[cw->maxcol], cw->datattrs[cw->maxcol], cw->datcolors[cw->maxcol], 0);
                cw->maxcol--;
                if (cw->maxcol < 0)
                    cw->maxcol = cw->rows - 1;
                if (!cw->data[cw->maxcol])
                    cw->maxcol = cw->maxrow;
            } while (cw->maxcol != cw->maxrow);

            display_nhwindow(prevmsg_win, TRUE);
            destroy_nhwindow(prevmsg_win);
            cw->maxcol = cw->maxrow;
            ttyDisplay->dismiss_more = 0;
        }
    } else if (iflags.prevmsg_window == 's') { /* single */
        ttyDisplay->dismiss_more = C('p'); /* <ctrl/P> allowed at --More-- */
        do {
            morc = 0;
            if (cw->maxcol == cw->maxrow)
                redotoplin(toplines, toplineattrs, toplinecolors);
            else if (cw->data[cw->maxcol])
                redotoplin(cw->data[cw->maxcol], cw->datattrs[cw->maxcol], cw->datcolors[cw->maxcol]);
            cw->maxcol--;
            if (cw->maxcol < 0)
                cw->maxcol = cw->rows - 1;
            if (!cw->data[cw->maxcol])
                cw->maxcol = cw->maxrow;
        } while (morc == C('p'));
        ttyDisplay->dismiss_more = 0;
    }
    return 0;
}

STATIC_OVL void
redotoplin(str, attrs, colors)
const char *str, *attrs, *colors;
{
    int otoplin = ttyDisplay->toplin;

    home();
    if (*str & 0x80) {
        /* kludge for the / command, the only time we ever want a */
        /* graphics character on the top line */
        g_putch((int)*str++, TRUE);
        ttyDisplay->curx++;
    }
    end_glyphout(); /* in case message printed during graphics output */
    putsyms_ex(str, attrs, colors);
    cl_end();
    ttyDisplay->toplin = 1;
    if (ttyDisplay->cury && otoplin != 3)
        more();
}

/* for use by tty_putstr() */
void
show_topl(str, attr, color)
const char *str;
int attr, color;
{
    struct WinDesc *cw = wins[WIN_MESSAGE];

    if (!(cw->flags & WIN_STOP)) {
        cw->curx = cw->cury = 0;
        home();
        cl_end();
        addtopl(str, attr, color);
    }
}

/* used by update_topl(); also by tty_putstr() */
void
remember_topl()
{
    register struct WinDesc *cw = wins[WIN_MESSAGE];
    int idx = cw->maxrow;
    size_t len = strlen(toplines) + 1;

    if ((cw->flags & WIN_LOCKHISTORY) || !*toplines)
        return;

    if (len > (size_t) cw->datlen[idx]) {
        if (cw->data[idx])
            free(cw->data[idx]);
        if (cw->datattrs[idx])
            free(cw->datattrs[idx]);
        if (cw->datcolors[idx])
            free(cw->datcolors[idx]);
        len += (8 - (len & 7)); /* pad up to next multiple of 8 */
        cw->data[idx] = (char *) alloc(len);
        cw->datattrs[idx] = (char*)alloc(len);
        cw->datcolors[idx] = (char*) alloc(len);
        cw->datlen[idx] = (short) len;
    }
    Strcpy(cw->data[idx], toplines);
    memcpy(cw->datattrs[idx], toplineattrs, len);
    memcpy(cw->datcolors[idx], toplinecolors, len);
    *toplines = '\0';
    memset(toplineattrs, ATR_NONE, sizeof(toplineattrs));
    memset(toplinecolors, NO_COLOR, sizeof(toplinecolors));
    toplineattrs[sizeof(toplineattrs) - 1] = toplinecolors[sizeof(toplinecolors) - 1] = 0;
    cw->maxcol = cw->maxrow = (idx + 1) % cw->rows;
}

void
addtopl(s, attr, color)
const char *s;
int attr, color;
{
    register struct WinDesc *cw = wins[WIN_MESSAGE];

    tty_curs(BASE_WINDOW, cw->curx + 1, cw->cury);

    end_glyphout(); /* in case message printed during graphics output */
    toggle_topl_attr(TRUE, attr, color);
    putsyms(s);
    toggle_topl_attr(FALSE, attr, color);

    cl_end();
    ttyDisplay->toplin = 1;
}

void
more()
{
    struct WinDesc *cw = wins[WIN_MESSAGE];

    /* avoid recursion -- only happens from interrupts */
    if (ttyDisplay->inmore++)
        return;
    if (iflags.debug_fuzzer)
        return;

    if (ttyDisplay->toplin) {
        tty_curs(BASE_WINDOW, cw->curx + 1, cw->cury);
        if (cw->curx >= CO - 8)
            topl_putsym('\n');
    }

    if (flags.standout)
        standoutbeg();
    putsyms(defmorestr);
    if (flags.standout)
        standoutend();

    xwaitforspace("\033 ");

    if (morc == '\033')
        cw->flags |= WIN_STOP;

    if (ttyDisplay->toplin && cw->cury) {
        docorner(1, cw->cury + 1);
        cw->curx = cw->cury = 0;
        home();
    } else if (morc == '\033') {
        cw->curx = cw->cury = 0;
        home();
        cl_end();
    }
    ttyDisplay->toplin = 0;
    ttyDisplay->inmore = 0;
}

void
update_topl(bp, attr, color)
register const char *bp;
int attr, color;
{
    register char *tl, *otl;
    register int n0;
    size_t len;
    int notdied = 1;
    struct WinDesc *cw = wins[WIN_MESSAGE];

    /* If there is room on the line, print message on same line */
    /* But messages like "You die..." deserve their own line */
    n0 = (int)strlen(bp);
    len = strlen(toplines);

    if ((ttyDisplay->toplin == 1 || (cw->flags & WIN_STOP))
        && cw->cury == 0
        && n0 + (int)len + 3 < CO - 8 /* room for --More-- */
        && (notdied = strncmp(bp, "You die", 7)) != 0) 
    {
        Strcat(toplines, "  ");
        Strcat(toplines, bp);
        memset(toplineattrs + len, attr, 2 + n0);
        memset(toplinecolors + len, color, 2 + n0);
        toplineattrs[len + 2 + n0] = toplinecolors[len + 2 + n0] = 0;
        cw->curx += 2;
        if (!(cw->flags & WIN_STOP))
            addtopl(bp, attr, color);
        return;
    } 
    else if (!(cw->flags & WIN_STOP)) 
    {
        if (ttyDisplay->toplin == 1) {
            more();
        } else if (cw->cury) { /* for when flags.toplin == 2 && cury > 1 */
            docorner(1, cw->cury + 1); /* reset cury = 0 if redraw screen */
            cw->curx = cw->cury = 0;   /* from home--cls() & docorner(1,n) */
        }
    }
    remember_topl();
    (void) strncpy(toplines, bp, TBUFSZ);
    toplines[TBUFSZ - 1] = 0;
    len = strlen(toplines);
    memset(toplineattrs, attr, len);
    memset(toplinecolors, color, len);
    toplineattrs[len] = toplinecolors[len] = 0;

    for (tl = toplines; n0 >= CO; ) {
        otl = tl;
        for (tl += CO - 1; tl != otl; --tl)
            if (*tl == ' ')
                break;
        if (tl == otl) {
            /* Eek!  A huge token.  Try splitting after it. */
            tl = index(otl, ' ');
            if (!tl)
                break; /* No choice but to spit it out whole. */
        }
        *tl++ = '\n';
        n0 = strlen(tl);
    }
    if (!notdied)
        cw->flags &= ~WIN_STOP;
    if (!(cw->flags & WIN_STOP))
        redotoplin(toplines, toplineattrs, toplinecolors);
}

STATIC_OVL
void
topl_putsym(c)
int c;
{
    register struct WinDesc *cw = wins[WIN_MESSAGE];

    if (cw == (struct WinDesc*) 0)
    {
        panic("Putsym window MESSAGE nonexistant");
        return;
    }

    switch (c) {
    case '\b':
        if (ttyDisplay->curx == 0 && ttyDisplay->cury > 0)
            tty_curs(BASE_WINDOW, CO, (int) ttyDisplay->cury - 1);
        backsp();
        ttyDisplay->curx--;
        cw->curx = ttyDisplay->curx;
        return;
    case '\n':
        cl_end();
        ttyDisplay->curx = 0;
        ttyDisplay->cury++;
        cw->cury = ttyDisplay->cury;
#ifdef WIN32CON
        if (skip_utf8)
            (void)putchar(c);
        else
            (void) doputchar(c, TRUE);
#endif
        break;
    default:
        if (ttyDisplay->curx == CO - 1)
            topl_putsym('\n'); /* 1 <= curx < CO; avoid CO */
#ifdef WIN32CON
        if (skip_utf8)
            (void)putchar(c);
        else
            (void) doputchar(c, TRUE);
#endif
        ttyDisplay->curx++;
    }
    cw->curx = ttyDisplay->curx;
    if (cw->curx == 0)
        cl_end();
#ifndef WIN32CON
    if (skip_utf8)
        (void)putchar(c);
    else
        (void) doputchar(c, TRUE);
#endif
}

void
putsyms(str)
const char *str;
{
    while (*str)
        topl_putsym(*str++);
}

void
putsyms_ex(str, attrs, colors)
const char* str, *attrs, *colors;
{
    int attr = ATR_NONE, color = NO_COLOR;
    while (*str)
    {
        if ((attrs && *attrs != attr) || (colors && color != *colors))
        {
            if(attr != ATR_NONE || color != NO_COLOR)
                toggle_topl_attr(FALSE, attr, color);
            attr = *attrs;
            color = *colors;
            toggle_topl_attr(TRUE, attr, color);
        }
        topl_putsym(*str);
        str++;
        attrs++;
        colors++;
    }
    if (attr != ATR_NONE || color != NO_COLOR)
        toggle_topl_attr(FALSE, attr, color);
}

STATIC_OVL void
removetopl(n)
register int n;
{
    /* assume addtopl() has been done, so ttyDisplay->toplin is already set */
    while (n-- > 0)
        putsyms("\b \b");
}

extern char erase_char; /* from xxxtty.c; don't need kill_char */

/* returns a single keystroke; also sets 'yn_number' */
char
tty_yn_function_ex(style, attr, color, glyph, title, query, resp, def, resp_desc, introline, ynflags)
int style UNUSED, attr UNUSED, color UNUSED, glyph UNUSED;
const char *title UNUSED, *query, *resp, *resp_desc UNUSED, *introline UNUSED;
char def;
unsigned long ynflags UNUSED;
/*
 *   Generic yes/no function. 'def' is the default (returned by space or
 *   return; 'esc' returns 'q', or 'n', or the default, depending on
 *   what's in the string. The 'query' string is printed before the user
 *   is asked about the string.
 *   If resp is NULL, any single character is accepted and returned.
 *   If not-NULL, only characters in it are allowed (exceptions:  the
 *   quitchars are always allowed, and if it contains '#' then digits
 *   are allowed); if it includes an <esc>, anything beyond that won't
 *   be shown in the prompt to the user but will be acceptable as input.
 */
{
    register char q;
    char rtmp[40];
    boolean digit_ok, allow_num, preserve_case = FALSE;
    struct WinDesc *cw = wins[WIN_MESSAGE];
    boolean doprev = 0;
    char prompt[BUFSZ + QBUFSZ];
    size_t len;

    yn_number = 0L;
    if (ttyDisplay->toplin == 1 && !(cw->flags & WIN_STOP))
        more();
    cw->flags &= ~WIN_STOP;
    ttyDisplay->toplin = 3; /* special prompt state */
    ttyDisplay->inread++;
    if (resp) {
        char *rb, respbuf[QBUFSZ];

        allow_num = (index(resp, '#') != 0);
        Strcpy(respbuf, resp);
        /* normally we force lowercase, but if any uppercase letters
           are present in the allowed response, preserve case;
           check this before stripping the hidden choices */
        for (rb = respbuf; *rb; ++rb)
            if ('A' <= *rb && *rb <= 'Z') {
                preserve_case = TRUE;
                break;
            }
        /* any acceptable responses that follow <esc> aren't displayed */
        if ((rb = index(respbuf, '\033')) != 0)
            *rb = '\0';
        (void) strncpy(prompt, query, QBUFSZ - 1);
        prompt[QBUFSZ - 1] = '\0';
        Sprintf(eos(prompt), " [%s]", respbuf);
        if (def)
            Sprintf(eos(prompt), " (%c)", def);
        /* not pline("%s ", prompt);
           trailing space is wanted here in case of reprompt */
        Strcat(prompt, " ");
        custompline_ex(attr, color, OVERRIDE_MSGTYPE | SUPPRESS_HISTORY, "%s", prompt);
    } else {
        /* no restriction on allowed response, so always preserve case */
        /* preserve_case = TRUE; -- moot since we're jumping to the end */
        Sprintf(prompt, "%s ", query);
        custompline_ex(attr, color, OVERRIDE_MSGTYPE | SUPPRESS_HISTORY, "%s", prompt);
        q = readchar();
        goto clean_up;
    }

    do { /* loop until we get valid input */
        q = readchar();
        if (!preserve_case)
            q = lowc(q);
        if (q == '\020') { /* ctrl-P */
            if (iflags.prevmsg_window != 's') {
                int sav = ttyDisplay->inread;
                ttyDisplay->inread = 0;
                (void) tty_doprev_message();
                ttyDisplay->inread = sav;
                tty_clear_nhwindow(WIN_MESSAGE);
                cw->maxcol = cw->maxrow;
                addtopl(prompt, attr, color);
            } else {
                if (!doprev)
                    (void) tty_doprev_message(); /* need two initially */
                (void) tty_doprev_message();
                doprev = 1;
            }
            q = '\0'; /* force another loop iteration */
            continue;
        } else if (doprev) {
            /* BUG[?]: this probably ought to check whether the
               character which has just been read is an acceptable
               response; if so, skip the reprompt and use it. */
            tty_clear_nhwindow(WIN_MESSAGE);
            cw->maxcol = cw->maxrow;
            doprev = 0;
            addtopl(prompt, attr, color);
            q = '\0'; /* force another loop iteration */
            continue;
        }
        digit_ok = allow_num && digit(q);
        if (q == '\033') {
            if (index(resp, 'q'))
                q = 'q';
            else if (index(resp, 'n'))
                q = 'n';
            else
                q = def;
            break;
        } else if (index(quitchars, q)) {
            q = def;
            break;
        }
        if (!index(resp, q) && !digit_ok) {
            tty_nhbell();
            q = (char) 0;
        } else if (q == '#' || digit_ok) {
            char z, digit_string[2];
            int n_len = 0;
            long value = 0;

            addtopl("#", attr, color), n_len++;
            digit_string[1] = '\0';
            if (q != '#') {
                digit_string[0] = q;
                addtopl(digit_string, attr, color), n_len++;
                value = q - '0';
                q = '#';
            }
            do { /* loop until we get a non-digit */
                z = readchar();
                if (!preserve_case)
                    z = lowc(z);
                if (digit(z)) {
                    value = (10 * value) + (z - '0');
                    if (value < 0)
                        break; /* overflow: try again */
                    digit_string[0] = z;
                    addtopl(digit_string, attr, color), n_len++;
                } else if (z == 'y' || index(quitchars, z)) {
                    if (z == '\033')
                        value = -1; /* abort */
                    z = '\n';       /* break */
                } else if (z == erase_char || z == '\b') {
                    if (n_len <= 1) {
                        value = -1;
                        break;
                    } else {
                        value /= 10;
                        removetopl(1), n_len--;
                    }
                } else {
                    value = -1; /* abort */
                    tty_nhbell();
                    break;
                }
            } while (z != '\n');
            if (value > 0)
                yn_number = value;
            else if (value == 0)
                q = 'n'; /* 0 => "no" */
            else {       /* remove number from top line, then try again */
                removetopl(n_len), n_len = 0;
                q = '\0';
            }
        }
    } while (!q);

 clean_up:
    if (yn_number)
        Sprintf(rtmp, "#%ld", yn_number);
    else
        (void) key2txt(q, rtmp);
    /* addtopl(rtmp, attr, color); -- rewrite toplines instead */
    Sprintf(toplines, "%s%s", prompt, rtmp);
    len = strlen(toplines);
    memset(toplineattrs, attr, len);
    memset(toplinecolors, color, len);
    toplineattrs[len] = toplinecolors[len] = 0;
#ifdef DUMPLOG
    dumplogmsg(toplines);
#endif
    ttyDisplay->inread--;
    ttyDisplay->toplin = 2;
    if (ttyDisplay->intr)
        ttyDisplay->intr--;
    if (wins[WIN_MESSAGE]->cury)
        tty_clear_nhwindow(WIN_MESSAGE);

    return q;
}

/* shared by tty_getmsghistory() and tty_putmsghistory() */
static char **snapshot_mesgs = 0;
static char** snapshot_mesg_attrs = 0;
static char** snapshot_mesg_colors = 0;

/* collect currently available message history data into a sequential array;
   optionally, purge that data from the active circular buffer set as we go */
STATIC_OVL void
msghistory_snapshot(purge)
boolean purge; /* clear message history buffer as we copy it */
{
    char *mesg, *mesg_color, * mesg_attr;
    int i, inidx, outidx;
    struct WinDesc *cw;

    /* paranoia (too early or too late panic save attempt?) */
    if (WIN_MESSAGE == WIN_ERR || !wins[WIN_MESSAGE])
        return;
    cw = wins[WIN_MESSAGE];

    /* flush toplines[], moving most recent message to history */
    remember_topl();

    /* for a passive snapshot, we just copy pointers, so can't allow further
       history updating to take place because that could clobber them */
    if (!purge)
        cw->flags |= WIN_LOCKHISTORY;

    snapshot_mesgs = (char **) alloc(((size_t)cw->rows + 1) * sizeof(char *));
    snapshot_mesg_colors = (char**)alloc(((size_t)cw->rows + 1) * sizeof(char*));
    snapshot_mesg_attrs = (char**)alloc(((size_t)cw->rows + 1) * sizeof(char*));
    outidx = 0;
    inidx = cw->maxrow;
    for (i = 0; i < cw->rows; ++i) {
        snapshot_mesgs[i] = (char *) 0;
        snapshot_mesg_colors[i] = (char *)0;
        snapshot_mesg_attrs[i] = (char *)0;
        mesg = cw->data[inidx];
        mesg_attr = cw->datattrs[inidx];
        mesg_color = cw->datcolors[inidx];
        if (mesg && *mesg) {
            snapshot_mesgs[outidx] = mesg;
            snapshot_mesg_attrs[outidx] = mesg_attr;
            snapshot_mesg_colors[outidx] = mesg_color;
            outidx++;
            if (purge) {
                /* we're taking this pointer away; subsequest history
                   updates will eventually allocate a new one to replace it */
                cw->data[inidx] = (char *) 0;
                cw->datattrs[inidx] = (char*)0;
                cw->datcolors[inidx] = (char*) 0;
                cw->datlen[inidx] = 0;
            }
        }
        inidx = (inidx + 1) % cw->rows;
    }
    snapshot_mesgs[cw->rows] = (char *) 0; /* sentinel */
    snapshot_mesg_attrs[cw->rows] = (char*)0; /* sentinel */
    snapshot_mesg_colors[cw->rows] = (char*)0; /* sentinel */

    /* for a destructive snapshot, history is now completely empty */
    if (purge)
        cw->maxcol = cw->maxrow = 0;
}

/* release memory allocated to message history snapshot */
STATIC_OVL void
free_msghistory_snapshot(purged)
boolean purged; /* True: took history's pointers, False: just cloned them */
{
    if (snapshot_mesgs) {
        /* snapshot pointers are no longer in use */
        if (purged) {
            int i;

            for (i = 0; snapshot_mesgs[i]; ++i)
            {
                free((genericptr_t)snapshot_mesgs[i]);
            }
        }

        free((genericptr_t) snapshot_mesgs), snapshot_mesgs = (char **) 0;

        /* history can resume being updated at will now... */
        if (!purged)
            wins[WIN_MESSAGE]->flags &= ~WIN_LOCKHISTORY;
    }
    if (snapshot_mesg_colors) {
        /* snapshot pointers are no longer in use */
        if (purged) {
            int i;
            for (i = 0; snapshot_mesg_colors[i]; ++i)
            {
                free((genericptr_t)snapshot_mesg_colors[i]);
            }
        }

        free((genericptr_t)snapshot_mesg_colors), snapshot_mesg_colors = (char**)0;

    }
    if (snapshot_mesg_colors) {
        /* snapshot pointers are no longer in use */
        if (purged) {
            int i;

            for (i = 0; snapshot_mesg_attrs[i]; ++i)
            {
                free((genericptr_t)snapshot_mesg_attrs[i]);
            }
        }

        free((genericptr_t)snapshot_mesg_attrs), snapshot_mesg_attrs = (char**)0;
    }
}

/*
 * This is called by the core save routines.
 * Each time we are called, we return one string from the
 * message history starting with the oldest message first.
 * When none are left, we return a final null string.
 *
 * History is collected at the time of the first call.
 * Any new messages issued after that point will not be
 * included among the output of the subsequent calls.
 */
char*
tty_getmsghistory_ex(attrs_ptr, colors_ptr, init)
char** attrs_ptr, ** colors_ptr;
boolean init;
{
    static int nxtidx;
    char *nextmesg;
    char *result = 0;

    if (attrs_ptr)
        *attrs_ptr = (char*)0;
    if (colors_ptr)
        *colors_ptr = (char*)0;

    if (init) 
    {
        msghistory_snapshot(FALSE);
        nxtidx = 0;
    }

    if (snapshot_mesgs) 
    {
        nextmesg = snapshot_mesgs[nxtidx];
        if (nextmesg) 
        {
            result = (char *) nextmesg;
            if (attrs_ptr && snapshot_mesg_attrs && snapshot_mesg_attrs[nxtidx] != 0)
                *attrs_ptr = snapshot_mesg_attrs[nxtidx];
            if (colors_ptr && snapshot_mesg_colors && snapshot_mesg_colors[nxtidx] != 0)
                *colors_ptr = snapshot_mesg_colors[nxtidx];
        } 
        else 
        {
            free_msghistory_snapshot(FALSE);
        }
        nxtidx++;
    }
    return result;
}

/*
 * This is called by the core savefile restore routines.
 * Each time we are called, we stuff the string into our message
 * history recall buffer. The core will send the oldest message
 * first (actually it sends them in the order they exist in the
 * save file, but that is supposed to be the oldest first).
 * These messages get pushed behind any which have been issued
 * since this session with the program has been started, since
 * they come from a previous session and logically precede
 * anything (like "Restoring save file...") that's happened now.
 *
 * Called with a null pointer to finish up restoration.
 *
 * It's also called by the quest pager code when a block message
 * has a one-line summary specified.  We put that line directly
 * into message history for ^P recall without having displayed it.
 */
void
tty_putmsghistory_ex(msg, attrs, colors, restoring_msghist)
const char *msg;
const char* attrs UNUSED, *colors UNUSED;
boolean restoring_msghist;
{
    static boolean initd = FALSE;
    int idx;
#ifdef DUMPLOG
    extern unsigned saved_pline_index; /* pline.c */
#endif

    if (restoring_msghist && !initd) 
    {
        /* we're restoring history from the previous session, but new
           messages have already been issued this session ("Restoring...",
           for instance); collect current history (ie, those new messages),
           and also clear it out so that nothing will be present when the
           restored ones are being put into place */
        msghistory_snapshot(TRUE);
        initd = TRUE;
#ifdef DUMPLOG
        /* this suffices; there's no need to scrub saved_pline[] pointers */
        saved_pline_index = 0;
#endif
    }

    if (msg) 
    {
        /* move most recent message to history, make this become most recent */
        remember_topl();
        Strcpy(toplines, msg);
        size_t len = strlen(msg);
        size_t len_attrs = min(max(0, sizeof(toplineattrs) - 1), len);
        size_t len_colors = min(max(0, sizeof(toplinecolors) - 1), len);
        if(attrs)
            memcpy(toplineattrs, attrs, len_attrs);
        else
            memset(toplineattrs, ATR_NONE, len_attrs);
        if(colors)
            memcpy(toplinecolors, colors, len_colors);
        else
            memset(toplinecolors, NO_COLOR, len_colors);
        toplineattrs[len_attrs] = toplinecolors[len_colors] = 0;
        //memset(toplineattrs, attr, len_attrs);
        //memset(toplinecolors, color, len_colors);
        //toplineattrs[len_attrs] = toplinecolors[len_colors] = 0;
#ifdef DUMPLOG
        dumplogmsg(toplines);
#endif
    } 
    else if (snapshot_mesgs) 
    {
        /* done putting arbitrary messages in; put the snapshot ones back */
        for (idx = 0; snapshot_mesgs[idx]; ++idx) 
        {
            remember_topl();
            Strcpy(toplines, snapshot_mesgs[idx]);
            size_t len = strlen(toplines);
            size_t len_attrs = min(max(0, sizeof(toplineattrs) - 1), len);
            size_t len_colors = min(max(0, sizeof(toplinecolors) - 1), len);
            memcpy(toplineattrs, snapshot_mesg_attrs[idx], len_attrs);
            memcpy(toplinecolors, snapshot_mesg_colors[idx], len_colors);
            toplineattrs[len_attrs] = toplinecolors[len_colors] = 0;
#ifdef DUMPLOG
            dumplogmsg(toplines);
#endif
        }
        /* now release the snapshot */
        free_msghistory_snapshot(TRUE);
        initd = FALSE; /* reset */
    }
}

#endif /* TTY_GRAPHICS */

/*topl.c*/
