/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2022-08-14 */

/* GnollHack 4.0	mgetline.c	$NHDT-Date: 1432512797 2015/05/25 00:13:17 $  $NHDT-Branch: master $:$NHDT-Revision: 1.10 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Kenneth Lorber, Kensington, Maryland, 2015. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"
#include "mactty.h"
#include "macwin.h"
#include "macpopup.h"
#include "func_tab.h"

extern int NDECL(extcmd_via_menu); /* cmd.c */

typedef Boolean FDECL((*key_func), (unsigned char));

int
get_line_from_key_queue(char *bufp)
{
    *bufp = 0;
    if (try_key_queue(bufp)) {
        while (*bufp) {
            if (*bufp == 10 || *bufp == 13) {
                *bufp = 0;
            }
            bufp++;
        }
        return true;
    }
    return false;
}

static void
topl_getlin_ex(int style UNUSED, int attr, int color, const char *query, char *bufp, const char* placeholder, const char* linesuffix, const char* introline, Boolean ext)
{
    if (get_line_from_key_queue(bufp))
        return;

    char promptbuf[PBUFSZ] = "";
    //Do not show introline
    //if (introline && *introline)
    //    Sprintf(promptbuf, "%s", introline);
    if (query)
    {
        if (*promptbuf)
            Strcat(promptbuf, " ");
        Sprintf(eos(promptbuf), "%s", query);
    }
    if (placeholder)
        Sprintf(eos(promptbuf), " [%s]", placeholder);
    if (linesuffix)
        Sprintf(eos(promptbuf), " %s", linesuffix);
    enter_topl_mode(attr, color, promptbuf);
    while (topl_key(nhgetch(), ext))
        ;
    leave_topl_mode(bufp);
}

/*
 * Read a line closed with '\n' into the array char bufp[BUFSZ].
 * (The '\n' is not stored. The string is closed with a '\0'.)
 * Reading can be interrupted by an escape ('\033') - now the
 * resulting string is "\033".
 */
void
mac_getlin_ex(int style, int attr, int color, const char *query, char *bufp, const char* placeholder, const char* linesuffix, const char* introline)
{
    topl_getlin_ex(style, attr, color, query, bufp, placeholder, linesuffix, introline, false);
}

/* Read in an extended command - doing command line completion for
 * when enough characters have been entered to make a unique command.
 * This is just a modified getlin() followed by a lookup.   -jsb
 */
int
mac_get_ext_cmd()
{
    char bufp[BUFSZ];
    int i;

    if (iflags.extmenu)
        return extcmd_via_menu();
    topl_getlin_ex(GETLINE_EXTENDED_COMMAND, ATR_NONE, NO_COLOR, "# ", bufp, (char*)0, (char*)0, (char*)0, true);
    for (i = 0; extcmdlist[i].ef_txt != (char *) 0; i++)
        if (!strcmp(bufp, extcmdlist[i].ef_txt))
            break;
    if (extcmdlist[i].ef_txt == (char *) 0)
        i = -1; /* not found */

    return i;
}

/* macgetline.c */
