/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2022-08-14 */

/* GnollHack 4.0    getline.c    $NHDT-Date: 1543830347 2018/12/03 09:45:47 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.37 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Michael Allison, 2006. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"

#ifdef TTY_GRAPHICS

#if !defined(MAC)
#define NEWAUTOCOMP
#endif

#include "wintty.h"
#include "func_tab.h"

char morc = 0; /* tell the outside world what char you chose */
STATIC_VAR boolean suppress_history;
STATIC_DCL boolean FDECL(ext_cmd_getlin_hook, (char *));

typedef boolean FDECL((*getlin_hook_proc), (char *));

STATIC_DCL void FDECL(hooked_tty_getlin_ex,
                      (int, int, int, const char *, char *, const char*, const char*, const char*, getlin_hook_proc));
extern int NDECL(extcmd_via_menu); /* cmd.c */

extern char erase_char, kill_char; /* from appropriate tty.c file */

/*
 * Read a line closed with '\n' into the array char bufp[BUFSZ].
 * (The '\n' is not stored. The string is closed with a '\0'.)
 * Reading can be interrupted by an escape ('\033') - now the
 * resulting string is "\033".
 */
void
tty_getlin_ex(style, attr, color, query, bufp, placeholder, linesuffix, introline)
int style, attr, color;
const char *query;
const char* placeholder;
const char* linesuffix;
const char* introline;
register char *bufp;
{
    suppress_history = FALSE;
    hooked_tty_getlin_ex(style, attr, color, query, bufp, placeholder, linesuffix, introline, (getlin_hook_proc) 0);
}

boolean skip_utf8 = FALSE;

STATIC_OVL void
hooked_tty_getlin_ex(style, attr, color, query, bufp, placeholder, linesuffix, introline, hook)
int style UNUSED, attr, color;
const char *query;
register char *bufp;
const char* placeholder;
const char* linesuffix;
const char* introline UNUSED;
getlin_hook_proc hook;
{
    register char *obufp = bufp;
    register int c;
    struct WinDesc *cw = wins[WIN_MESSAGE];
    boolean doprev = 0;
    char promptbuf[BUFSZ] = "";
    char promptcolors[BUFSZ];
    char promptattrs[BUFSZ];
    memset(promptcolors, color, sizeof(promptcolors));
    memset(promptattrs, attr, sizeof(promptattrs));

    //Do not show introline
    if (query)
        Sprintf(promptbuf, "%s", query);
    if (placeholder)
        Sprintf(eos(promptbuf), " [%s]", placeholder);
    if (linesuffix)
        Sprintf(eos(promptbuf), " %s", linesuffix);
    if (ttyDisplay->toplin == 1 && !(cw->flags & WIN_STOP))
        more();
    cw->flags &= ~WIN_STOP;
    ttyDisplay->toplin = 3; /* special prompt state */
    ttyDisplay->inread++;

    /* issue the prompt */
    custompline(OVERRIDE_MSGTYPE | SUPPRESS_HISTORY, "%s ", promptbuf);
#ifdef EDIT_GETLIN
    /* bufp is input/output; treat current contents (presumed to be from
       previous getlin()) as default input */
    addtopl(obufp, attr, color);
    bufp = eos(obufp);
#else
    /* !EDIT_GETLIN: bufp is output only; init it to empty */
    *bufp = '\0';
#endif

    skip_utf8 = TRUE;
    for (;;) {
        (void) fflush(stdout);
        Strcat(strcat(strcpy(toplines, promptbuf), " "), obufp);
        memcpy(toplineattrs, promptattrs, min(sizeof(toplineattrs), sizeof(promptattrs)));
        memcpy(toplinecolors, promptcolors, min(sizeof(toplinecolors), sizeof(promptcolors)));
        toplineattrs[sizeof(toplineattrs) - 1] = toplinecolors[sizeof(toplinecolors) - 1] = 0;
        c = pgetchar();
        if (c == '\033' || c == EOF) {
#ifdef UNIX
            if(c != EOF && tty_arrow_key_support_enabled())
                c = pgetchar(); /* Get another one */
#endif
            if ((c == '\033' || c == 0) && obufp[0] != '\0') {
                obufp[0] = '\0';
                bufp = obufp;
                tty_clear_nhwindow(WIN_MESSAGE);
                cw->maxcol = cw->maxrow;
                addtopl(promptbuf, attr, color);
                addtopl(" ", attr, color);
                addtopl(obufp, attr, color);
#ifdef UNIX
            } else if (c == 91 && tty_arrow_key_support_enabled()) {
                c = pgetchar(); /* Get third one */
                switch (c)
                {
                case 0: /* Alt+[ */
                    break;
                case 65: /* Up */
                    break;
                case 66: /* Down */
                    break;
                case 67: /* Right */
                    break;
                case 68: /* Left */
                    break;
                default:
                    break;
                }
                continue;
#endif
            } else {
                obufp[0] = '\033';
                obufp[1] = '\0';
                break;
            }
        }

        if (ttyDisplay->intr) {
            ttyDisplay->intr--;
            *bufp = 0;
        }
        if (c == '\020') { /* ctrl-P */
            if (iflags.prevmsg_window != 's') {
                int sav = ttyDisplay->inread;

                ttyDisplay->inread = 0;
                (void) tty_doprev_message();
                ttyDisplay->inread = sav;
                tty_clear_nhwindow(WIN_MESSAGE);
                cw->maxcol = cw->maxrow;
                addtopl(promptbuf, attr, color);
                addtopl(" ", attr, color);
                *bufp = 0;
                addtopl(obufp, attr, color);
            } else {
                if (!doprev)
                    (void) tty_doprev_message(); /* need two initially */
                (void) tty_doprev_message();
                doprev = 1;
                continue;
            }
        } else if (doprev && iflags.prevmsg_window == 's') {
            tty_clear_nhwindow(WIN_MESSAGE);
            cw->maxcol = cw->maxrow;
            doprev = 0;
            addtopl(promptbuf, attr, color);
            addtopl(" ", attr, color);
            *bufp = 0;
            addtopl(obufp, attr, color);
        }
        if (c == erase_char || c == '\b') {
            if (bufp != obufp) {
#ifdef NEWAUTOCOMP
                char *i;

#endif /* NEWAUTOCOMP */
                bufp--;
#ifndef NEWAUTOCOMP
                putsyms("\b \b"); /* putsym converts \b */
#else                             /* NEWAUTOCOMP */
                putsyms("\b");
                for (i = bufp; *i; ++i)
                    putsyms(" ");
                for (; i > bufp; --i)
                    putsyms("\b");
                *bufp = 0;
#endif                            /* NEWAUTOCOMP */
            } else
                tty_nhbell();
        } else if (c == '\n' || c == '\r') {
#ifndef NEWAUTOCOMP
            *bufp = 0;
#endif /* not NEWAUTOCOMP */
            break;
        } else if (' ' <= (unsigned char) c && c != '\177'
                   /* avoid isprint() - some people don't have it
                      ' ' is not always a printing char */
                   && (bufp - obufp < BUFSZ - 1 && bufp - obufp < COLNO)) {
#ifdef NEWAUTOCOMP
            char *i = eos(bufp);

#endif /* NEWAUTOCOMP */
            *bufp = c;
            bufp[1] = 0;
            putsyms_ex(bufp, promptattrs, promptcolors);
            bufp++;
            if (hook && (*hook)(obufp)) {
                putsyms_ex(bufp, promptattrs, promptcolors);
#ifndef NEWAUTOCOMP
                bufp = eos(bufp);
#else  /* NEWAUTOCOMP */
                /* pointer and cursor left where they were */
                for (i = bufp; *i; ++i)
                    putsyms("\b");
            } else if (i > bufp) {
                char *s = i;

                /* erase rest of prior guess */
                for (; i > bufp; --i)
                    putsyms(" ");
                for (; s > bufp; --s)
                    putsyms("\b");
#endif /* NEWAUTOCOMP */
            }
        } else if (c == kill_char || c == '\177') { /* Robert Viduya */
            /* this test last - @ might be the kill_char */
#ifndef NEWAUTOCOMP
            while (bufp != obufp) {
                bufp--;
                putsyms("\b \b");
            }
#else  /* NEWAUTOCOMP */
            for (; *bufp; ++bufp)
                putsyms(" ");
            for (; bufp != obufp; --bufp)
                putsyms("\b \b");
            *bufp = 0;
#endif /* NEWAUTOCOMP */
        } else
            tty_nhbell();
    }
    ttyDisplay->toplin = 2; /* nonempty, no --More-- required */
    ttyDisplay->inread--;
    clear_nhwindow(WIN_MESSAGE); /* clean up after ourselves */

    if (suppress_history) {
        /* prevent next message from pushing current promptbuf+answer into
           tty message history */
        *toplines = '\0';
        memset(toplineattrs, ATR_NONE, sizeof(toplineattrs));
        memset(toplinecolors, NO_COLOR, sizeof(toplinecolors));
        toplineattrs[sizeof(toplineattrs) - 1] = toplinecolors[sizeof(toplinecolors) - 1] = 0;
#ifdef DUMPLOG
    } else {
        /* needed because we've bypassed pline() */
        dumplogmsg(toplines);
#endif
    }

    skip_utf8 = FALSE;

    if (use_utf8_encoding())
    {
        convertUTF8toCP437(obufp, strlen(obufp)); /* Texts are stored internally as CP437 */
    }
}

void
xwaitforspace(s)
register const char *s; /* chars allowed besides return */
{
    register int c, x = ttyDisplay ? (int) ttyDisplay->dismiss_more : '\n';

    morc = 0;
    while (
#ifdef HANGUPHANDLING
        !program_state.done_hup &&
#endif
        (c = tty_nhgetch()) != EOF) {
        if (c == '\n' || c == '\r')
            break;

#ifdef UNIX
        if (c == '\033' && tty_arrow_key_support_enabled())
        {
            c = tty_nhgetch();
            if (c == 0 || c == '\033')
            {
                c = '\033';
                break;

            }
            else if (c == 91)
            {
                c = tty_nhgetch();
                break;  /* Break; regard arrow key etc. as an enter */
            }
        }

        if (c == EOF)
            break;

#endif

        if (iflags.cbreak) {
            if (c == '\033') {
                if (ttyDisplay)
                    ttyDisplay->dismiss_more = 1;
                morc = '\033';
                break;
            }
            if ((s && index(s, c)) || c == x || (x == '\n' && c == '\r')) {
                morc = (char) c;
                break;
            }
            tty_nhbell();
        }
    }
}

/*
 * Implement extended command completion by using this hook into
 * tty_getlin.  Check the characters already typed, if they uniquely
 * identify an extended command, expand the string to the whole
 * command.
 *
 * Return TRUE if we've extended the string at base.  Otherwise return FALSE.
 * Assumptions:
 *
 *    + we don't change the characters that are already in base
 *    + base has enough room to hold our string
 */
STATIC_OVL boolean
ext_cmd_getlin_hook(base)
char *base;
{
    int oindex, com_index;

    com_index = -1;
    for (oindex = 0; extcmdlist[oindex].ef_txt != (char *) 0; oindex++) {
        if (extcmdlist[oindex].flags & CMD_NOT_AVAILABLE)
            continue;
        if ((extcmdlist[oindex].flags & AUTOCOMPLETE)
            && !(!wizard && (extcmdlist[oindex].flags & WIZMODECMD))
            && !(!wizard && !discover && !CasualMode && (extcmdlist[oindex].flags & CASUALMODECMD))
            && !strncmpi(base, extcmdlist[oindex].ef_txt, strlen(base))) {
            if (com_index == -1) /* no matches yet */
                com_index = oindex;
            else /* more than 1 match */
                return FALSE;
        }
    }
    if (com_index >= 0) {
        Strcpy(base, extcmdlist[com_index].ef_txt);
        return TRUE;
    }

    return FALSE; /* didn't match anything */
}

/*
 * Read in an extended command, doing command line completion.  We
 * stop when we have found enough characters to make a unique command.
 */
int
tty_get_ext_cmd()
{
    int i;
    char buf[BUFSZ];

    if (iflags.extmenu)
        return extcmd_via_menu();

    suppress_history = TRUE;
    /* maybe a runtime option?
     * hooked_tty_getlin("#", buf,
     *                   (flags.cmd_comp && !in_doagain)
     *                      ? ext_cmd_getlin_hook
     *                      : (getlin_hook_proc) 0);
     */
    buf[0] = '\0';
    hooked_tty_getlin_ex(GETLINE_EXTENDED_COMMAND, ATR_NONE, NO_COLOR, "#", buf, 0, 0, 0, in_doagain ? (getlin_hook_proc) 0
                                           : ext_cmd_getlin_hook);
    (void) mungspaces(buf);
    if (buf[0] == 0 || buf[0] == '\033')
        return -1;

    for (i = 0; extcmdlist[i].ef_txt != (char *) 0; i++)
        if (!strcmpi(buf, extcmdlist[i].ef_txt))
            break;

    if (!in_doagain) {
        int j;
        for (j = 0; buf[j]; j++)
            savech(buf[j]);
        savech('\n');
    }

    if (extcmdlist[i].ef_txt == (char *) 0) {
        pline("%s: unknown extended command.", buf);
        i = -1;
    }

    return i;
}

#endif /* TTY_GRAPHICS */

/*getline.c*/
